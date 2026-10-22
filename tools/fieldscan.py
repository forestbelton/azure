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


def scan_function(path, hits):
    pending_hi = {}        # reg -> symbol, from lui %hi(SYM)
    base = {}              # reg -> (symbol, addend), reg holds &SYM+addend
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

        if mn in ("jal", "jalr"):
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


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("-n", "--limit", type=int, default=20)
    ap.add_argument("--min-fields", type=int, default=1)
    ap.add_argument("--symbol", help="show full detail for one symbol")
    args = ap.parse_args()

    hits = collections.defaultdict(list)
    files = sorted((ROOT / "asm/nonmatchings").glob("*/*.s"))
    if not files:
        raise SystemExit("no per-function asm; run `make splat`")
    for f in files:
        scan_function(f, hits)

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
