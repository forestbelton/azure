#!/usr/bin/env python3
"""Recover struct layouts by aggregating field accesses across all functions.

m2c reports every field as ->unkNN on a void *, because a single function
rarely reveals a whole struct. Correlating the accesses across all of them
does: each (offset, width) pair that shows up is a field, and the highest
offset bounds the struct.

This reads the splat assembly rather than Ghidra deliberately. Ghidra folds
`lui %hi(SYM)` + `lw 0x10($r)` into one reference to SYM+0x10, losing the
split between base and field offset -- which is exactly the information
needed. The assembly keeps them separate:

    lui   $v0, %hi(SYM)          ; base, high half
    addiu $s2, $v0, %lo(SYM)     ; $s2 now holds &SYM
    lw    $v1, 0x10($s2)         ; read SYM+0x10, 4 bytes

so tracking which register holds &SYM lets each later displaced access be
attributed to a field. Two shorter forms also appear: `lw $x, %lo(SYM)($r)`
and `lw $x, %gp_rel(SYM)($gp)`, both of which are plain accesses at offset 0.
"""

import argparse
import collections
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parent.parent

INSN = re.compile(r"^\s*/\* \w+ \w+ \w+ \*/\s+(\S+)\s*(.*?)\s*$")

WIDTH = {
    "lb": 1, "lbu": 1, "sb": 1,
    "lh": 2, "lhu": 2, "sh": 2,
    "lw": 4, "sw": 4, "lwl": 4, "lwr": 4, "swl": 4, "swr": 4,
    "lwc2": 4, "swc2": 4,
}
WRITES = {"sb", "sh", "sw", "swl", "swr", "swc2"}

# $s0-$s7, $fp, $gp, $sp survive a call; the rest do not.
CALL_SAFE = {"$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
             "$fp", "$gp", "$sp", "$ra"}

MEMREF = re.compile(r"^(\$\w+),\s*(.+)\((\$\w+)\)$")
HI = re.compile(r"^(\$\w+),\s*%hi\(([\w.]+)\)$")
LO_ADD = re.compile(r"^(\$\w+),\s*(\$\w+),\s*%lo\(([\w.]+)\)$")
IMM_ADD = re.compile(r"^(\$\w+),\s*(\$\w+),\s*(-?0x[0-9a-fA-F]+|-?\d+)$")
SYMREF = re.compile(r"%(?:lo|gp_rel)\(([\w.]+)\)")


def parse_int(text):
    text = text.strip()
    try:
        return int(text, 16) if text.lower().startswith(("0x", "-0x")) else int(text)
    except ValueError:
        return None


def scan_function(path, hits, callptr=None):
    pending_hi = {}        # reg -> symbol, from lui %hi(SYM)
    base = {}              # reg -> (symbol, addend), reg holds &SYM+addend
    loaded = {}            # reg -> (symbol, offset) most recently loaded into it
    func = path.stem

    for line in path.read_text().splitlines():
        m = INSN.match(line)
        if not m:
            continue
        mn, ops = m.group(1), m.group(2)
        mn = mn.rstrip()

        # --- memory access: the thing we are actually collecting -----------
        if mn in WIDTH:
            mo = MEMREF.match(ops)
            if mo:
                disp, rbase = mo.group(2), mo.group(3)
                sym = SYMREF.search(disp)
                kind = "write" if mn in WRITES else "read"
                if sym:
                    hits[sym.group(1)].append((0, WIDTH[mn], kind, func))
                elif rbase in base:
                    s, addend = base[rbase]
                    off = parse_int(disp)
                    if off is not None:
                        hits[s].append((addend + off, WIDTH[mn], kind, func))
                # A store to a tracked register does not change what it holds.
                if mn not in WRITES:
                    dest = mo.group(1)
                    base.pop(dest, None)
                    pending_hi.pop(dest, None)
                    loaded.pop(dest, None)
                    if WIDTH[mn] == 4 and not sym and rbase in base:
                        s_, addend = base[rbase]
                        off = parse_int(disp)
                        if off is not None:
                            loaded[dest] = (s_, addend + off)
            continue

        # --- base construction --------------------------------------------
        mo = HI.match(ops) if mn == "lui" else None
        if mo:
            pending_hi[mo.group(1)] = mo.group(2)
            base.pop(mo.group(1), None)
            continue

        mo = LO_ADD.match(ops) if mn in ("addiu", "addu", "addi") else None
        if mo:
            dst, src, sym = mo.groups()
            if pending_hi.get(src) == sym:
                base[dst] = (sym, 0)
            else:
                base.pop(dst, None)
            continue

        mo = IMM_ADD.match(ops) if mn in ("addiu", "addi") else None
        if mo:
            dst, src, imm = mo.groups()
            val = parse_int(imm)
            if src in base and val is not None:
                s, addend = base[src]
                base[dst] = (s, addend + val)
            else:
                base.pop(dst, None)
            continue

        if mn == "jalr" and callptr is not None:
            # A field loaded and then called through is a function pointer.
            target = ops.split(",")[-1].strip() or ops.strip()
            if target in loaded:
                callptr.add(loaded[target])

        if mn in ("jal", "jalr"):
            loaded.clear()
            for reg in list(base):
                if reg not in CALL_SAFE:
                    base.pop(reg)
            pending_hi.clear()
            continue

        # Anything else writing a tracked register invalidates it.
        first = ops.split(",")[0].strip()
        if first.startswith("$"):
            base.pop(first, None)
            pending_hi.pop(first, None)


# The loaded image, from the ghidra_psx_ldr section layout. Anything outside
# it is an artifact of the register tracking rather than a real global.
IMAGE_LO, IMAGE_HI = 0x8002D000, 0x8008875F

CTYPE = {1: "u8", 2: "u16", 4: "u32"}


def build_regions(hits):
    extents = []
    skipped = []
    for sym, rows in hits.items():
        m = re.fullmatch(r"D_([0-9A-Fa-f]{8})", sym)
        if not m:
            continue
        base = int(m.group(1), 16)
        if not (IMAGE_LO <= base <= IMAGE_HI):
            skipped.append(sym)
            continue
        lo = base + min(off for off, _, _, _ in rows)
        hi = base + max(off + width - 1 for off, width, _, _ in rows)
        extents.append((lo, hi, base, sym, rows))
    extents.sort()
    regions = []
    for lo, hi, base, sym, rows in extents:
        if regions and lo <= regions[-1]["hi"]:
            r = regions[-1]
            r["hi"] = max(r["hi"], hi)
            r["labels"].append((base, sym))
            r["rows"] += [(base + o, w, k, f) for o, w, k, f in rows]
        else:
            regions.append({
                "lo": lo, "hi": hi, "labels": [(base, sym)],
                "rows": [(base + o, w, k, f) for o, w, k, f in rows],
            })
    return regions, sorted(skipped)


def emit(hits, callptr):
    regions, skipped = build_regions(hits)
    # Absolute addresses of fields proven to be called through.
    fnptrs = set()
    for sym, off in callptr:
        m = re.fullmatch(r"D_([0-9A-Fa-f]{8})", sym)
        if m:
            fnptrs.add(int(m.group(1), 16) + off)

    print("#ifndef STRUCTS_H")
    print("#define STRUCTS_H")
    print()
    print('#include "types.h"')
    print()
    print("/* GENERATED by tools/fieldscan.py -- then hand-maintained.")
    print(" *")
    print(" * Layouts derived by aggregating every field access across the")
    print(" * decompiled assembly. Names are placeholders: a struct is named")
    print(" * for its address and a field for its offset, because nothing here")
    print(" * is evidence of what anything means. Rename as understanding")
    print(" * arrives -- but do not regenerate over your renames.")
    print(" *")
    print(" * Each field is a real observed access. Gaps are bytes nothing has")
    print(" * touched yet, so the padding is a floor on the size, not a fact.")
    print(" * Where one offset is read at several widths the widest wins and")
    print(" * the others are noted; those are the ones to check first.")
    print(" */")

    structs = [r for r in regions if len({(a, w) for a, w, _, _ in r["rows"]}) > 1]
    scalars = [r for r in regions if r not in structs]

    for r in sorted(structs, key=lambda r: r["lo"]):
        fields = {}
        widths = {}
        for addr, width, kind, _ in r["rows"]:
            widths.setdefault(addr, set()).add(width)
            fields[addr] = max(fields.get(addr, 0), width)
        name = "Unk%08X" % r["lo"]
        size = r["hi"] - r["lo"] + 1
        print()
        print("/* %#010x..%#010x, %#x bytes, %d field(s)%s */"
              % (r["lo"], r["hi"], size, len(fields),
                 "" if len(r["labels"]) == 1 else
                 "; labels: " + ", ".join(s for _, s in r["labels"])))
        print("typedef struct %s {" % name)

        # Pass one: decide which accesses survive. A narrower access inside a
        # wider field is a partial read of it, not a field of its own, so it is
        # reported and dropped rather than shifting everything after it.
        kept, dropped = [], {}
        cursor = r["lo"]
        for addr in sorted(fields):
            if addr < cursor:
                dropped.setdefault(cursor_owner, []).append(addr)
                continue
            kept.append((addr, fields[addr]))
            cursor = addr + fields[addr]
            cursor_owner = addr

        # Pass two: a long constant-stride run among the surviving fields may
        # be an array -- but that cannot be decided from what is recorded here.
        # Only constant displacements are collected; a real indexed array would
        # be reached through a register offset that never appears. So note the
        # pattern and still emit the observed fields.
        runs = {}
        i = 0
        while i < len(kept):
            j = i
            while (j + 1 < len(kept)
                   and kept[j + 1][1] == kept[i][1]
                   and kept[j + 1][0] - kept[j][0] == kept[i][1]):
                j += 1
            if j - i + 1 >= 8:
                runs[kept[i][0]] = (j - i + 1, kept[i][1])
            i = j + 1 if j > i else i + 1

        cursor = r["lo"]
        for addr, width in kept:
            if addr > cursor:
                print("    u8 pad_%02X[%#x];" % (cursor - r["lo"], addr - cursor))
            if addr in runs:
                count, w = runs[addr]
                print("    /* the next %d fields run at a constant %#x stride;"
                      " this may be an array */" % (count, w))
            off = addr - r["lo"]
            note = ""
            if len(widths[addr]) > 1:
                note = "  /* also read as %s */" % "/".join(
                    str(w) for w in sorted(widths[addr]) if w != width)
            if addr in fnptrs:
                print("    void (*unk_%02X)(void);%s"
                      % (off, note or "  /* called through */"))
            else:
                print("    %-3s unk_%02X;%s" % (CTYPE[width], off, note))
            for d in dropped.get(addr, []):
                print("    /* +%#-5x is a %s-byte access inside this field */"
                      % (d - r["lo"],
                         "/".join(str(w) for w in sorted(widths[d]))))
            cursor = addr + width
        if cursor <= r["hi"]:
            print("    u8 pad_%02X[%#x];" % (cursor - r["lo"], r["hi"] - cursor + 1))
        print("} %s;" % name)

    print()
    print("/* Objects with a single observed field: plain globals so far. */")
    for r in sorted(scalars, key=lambda r: r["lo"]):
        width = max(w for _, w, _, _ in r["rows"])
        sym = r["labels"][0][1]
        print("extern %-3s %s;" % (CTYPE.get(width, "u32"), sym))

    print()
    print("/* The multi-field objects above, by their base label. */")
    for r in sorted(structs, key=lambda r: r["lo"]):
        print("extern Unk%08X %s;" % (r["lo"], r["labels"][0][1]))

    print()
    print("#endif /* STRUCTS_H */")
    if skipped:
        print()
        print("/* excluded, base outside the loaded image %#x..%#x: %s */"
              % (IMAGE_LO, IMAGE_HI, " ".join(skipped)))


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("-n", "--limit", type=int, default=20)
    ap.add_argument("--min-fields", type=int, default=1)
    ap.add_argument("--symbol", help="show full detail for one symbol")
    ap.add_argument("--emit", action="store_true",
                    help="emit candidate typedefs and externs as C")
    args = ap.parse_args()

    hits = collections.defaultdict(list)
    files = sorted((ROOT / "asm/nonmatchings").glob("*/*.s"))
    # Plus the whole-file asm subsegments: main and the PSY-Q library modules
    # live there, and main is where the game-state struct is driven from.
    files += sorted(f for f in (ROOT / "asm").glob("*.s"))
    if not files:
        raise SystemExit("no per-function asm; run `make splat`")
    callptr = set()
    for f in files:
        scan_function(f, hits, callptr)

    if args.emit:
        emit(hits, callptr)
        return 0

    if args.symbol:
        rows = hits.get(args.symbol)
        if not rows:
            raise SystemExit("no accesses recorded for " + args.symbol)
        by_off = collections.defaultdict(list)
        for off, width, kind, func in rows:
            by_off[(off, width)].append((kind, func))
        print("%s: %d accesses, %d distinct fields\n"
              % (args.symbol, len(rows), len(by_off)))
        print("%-10s %-6s %-6s %-6s %s" % ("offset", "width", "reads", "writes", "functions"))
        for (off, width) in sorted(by_off):
            entries = by_off[(off, width)]
            r = sum(1 for k, _ in entries if k == "read")
            w = len(entries) - r
            fns = sorted({f for _, f in entries})
            shown = ", ".join(fns[:3]) + (" +%d" % (len(fns) - 3) if len(fns) > 3 else "")
            print("%#-10x %-6d %-6d %-6d %s" % (off, width, r, w, shown))
        return 0

    print("%-18s %7s %7s %9s %9s  %s"
          % ("symbol", "access", "fields", "span", "maxoff", "widths"))
    ranked = []
    for sym, rows in hits.items():
        fields = {(off, width) for off, width, _, _ in rows}
        if len(fields) < args.min_fields:
            continue
        maxoff = max(off for off, _, _, _ in rows)
        minoff = min(off for off, _, _, _ in rows)
        widths = sorted({w for _, w, _, _ in rows})
        ranked.append((len(fields), len(rows), sym, minoff, maxoff, widths))
    ranked.sort(reverse=True)
    for nf, na, sym, lo, hi, widths in ranked[:args.limit]:
        print("%-18s %7d %7d %9s %#9x  %s"
              % (sym, na, nf, "%#x..%#x" % (lo, hi), hi,
                 "/".join(str(w) for w in widths)))
    print("\n%d symbols touched in total; %d with >=%d distinct fields"
          % (len(hits), len(ranked), args.min_fields))

    # Labels whose accessed ranges overlap are one object that acquired
    # several base labels. That happens two ways: a function computes an
    # interior address, or -- visible as negative offsets -- gcc materialises
    # a base at one label and reaches lower addresses with signed
    # displacements. Either way the real extent is the union, so merge them
    # into regions rather than reporting pairs.
    extents = []
    for sym, rows in hits.items():
        m = re.fullmatch(r"D_([0-9A-Fa-f]{8})", sym)
        if not m:
            continue
        base = int(m.group(1), 16)
        lo = base + min(off for off, _, _, _ in rows)
        hi = base + max(off + width - 1 for off, width, _, _ in rows)
        extents.append((lo, hi, base, sym))
    extents.sort()

    regions = []
    for lo, hi, base, sym in extents:
        if regions and lo <= regions[-1]["hi"]:
            regions[-1]["hi"] = max(regions[-1]["hi"], hi)
            regions[-1]["labels"].append((base, sym))
        else:
            regions.append({"lo": lo, "hi": hi, "labels": [(base, sym)]})

    multi = [r for r in regions if len(r["labels"]) > 1]
    if multi:
        print("\n%d region(s) covered by more than one label -- each is one "
              "object:" % len(multi))
        for r in sorted(multi, key=lambda r: r["lo"] - r["hi"]):
            size = r["hi"] - r["lo"] + 1
            print("  %#010x..%#010x  size %#-7x labels: %s"
                  % (r["lo"], r["hi"], size,
                     ", ".join("%s%s" % (sym, "" if base == r["lo"]
                                         else "(+%#x)" % (base - r["lo"]))
                               for base, sym in r["labels"])))

    conflicts = []
    for sym, rows in hits.items():
        widths = collections.defaultdict(set)
        for off, width, _, _ in rows:
            widths[off].add(width)
        for off, ws in sorted(widths.items()):
            if len(ws) > 1:
                conflicts.append((sym, off, sorted(ws)))
    if conflicts:
        print("\n%d field(s) accessed at more than one width -- a union, a "
              "narrower read of a wider field, or adjacent fields:"
              % len(conflicts))
        for sym, off, ws in conflicts[:12]:
            print("  %-18s +%#-6x widths %s"
                  % (sym, off, "/".join(str(w) for w in ws)))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
