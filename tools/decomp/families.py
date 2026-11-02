#!/usr/bin/env python3
"""families.py UNIT -- group a unit's remaining stubs into FAMILIES.

A family is a set of functions with the same instruction shape and different
constants: the same code emitted per message id, per state offset, per table.
They were the single biggest yield in main.bin's card overlay -- 20 message
wrappers landed from one generator, and five 37-word initialisers from one
shape -- so finding them before sweeping by address is worth doing.

The signature masks exactly the fields a family is allowed to differ in:

  * `j`/`jal` targets -- a family calls different callees;
  * every i-type immediate and `lui` half -- a family uses different offsets,
    counts and symbols.

Everything else, including register numbers and branch displacements, must
match word for word. That is deliberately strict: two functions with the same
masked signature have the same control flow and the same allocation, so
decoding one gives you the other's source with the constants swapped.

    python3 tools/decomp/families.py ovl_town              # families by yield
    python3 tools/decomp/families.py ovl_town --min 3      # >=3 members
    python3 tools/decomp/families.py ovl_town --show func_800A4898
    python3 tools/decomp/families.py ovl_town --all        # include landed fns

--show prints the family's members side by side, one row per instruction, with
the masked-out immediates spelled out per member, so what varies is read off
rather than diffed by hand. A row where every member agrees is shared source;
a row where they differ is a constant to parameterise.

Counting note: a function already written in C is excluded by default, so the
report is remaining work. --all includes everything, which is how you check
whether a stub's family has a landed member to copy from.
"""
import argparse
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import asmlib  # noqa: E402

PROJ = asmlib.PROJ
ROW = re.compile(r"\s*/\* [0-9A-F]+ ([0-9A-F]{8}) ([0-9A-F]{8}) \*/\s+(.*)")

# i-type opcodes whose immediate is a constant a family may vary, plus lui.
ITYPE = {0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
         0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2A, 0x2B,
         0x2E, 0x30, 0x31, 0x38, 0x39}


def mask(w):
    op = w >> 26
    if op in (2, 3):
        return op << 26
    if op in ITYPE:
        return w & 0xFFFF0000
    return w


def read_unit(unit):
    """{func: [(vaddr, word, text)]} for every text function splat emitted."""
    d = os.path.join(PROJ, "asm", "nonmatchings", unit)
    if not os.path.isdir(d):
        raise SystemExit(f"no such unit listing: {d}")
    out = {}
    for name in sorted(os.listdir(d)):
        if not name.endswith(".s"):
            continue
        rows, inside = [], False
        for ln in open(os.path.join(d, name)):
            if ln.startswith("glabel "):
                inside = True
                continue
            if ln.startswith("endlabel"):
                inside = False
                continue
            if not inside:
                continue
            m = ROW.match(ln)
            if m:
                rows.append((int(m.group(1), 16),
                             int.from_bytes(bytes.fromhex(m.group(2)), "little"),
                             m.group(3).strip()))
        if rows:
            out[name[:-2]] = rows
    return out


def stubs(unit):
    """The functions still carrying an INCLUDE_ASM in csrc/<unit>.c."""
    p = os.path.join(PROJ, "csrc", unit + ".c")
    if not os.path.exists(p):
        return None
    return set(re.findall(r'INCLUDE_ASM\("[^"]*",\s*(\w+)\)', open(p).read()))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("unit")
    ap.add_argument("--min", type=int, default=2, help="minimum members")
    ap.add_argument("--all", action="store_true",
                    help="include functions already written in C")
    ap.add_argument("--show", metavar="FUNC",
                    help="print FUNC's family member by member")
    args = ap.parse_args()

    fns = read_unit(args.unit)
    todo = stubs(args.unit)
    if todo is not None and not args.all:
        fns = {k: v for k, v in fns.items() if k in todo}

    fam = {}
    for f, rows in fns.items():
        sig = tuple(mask(w) for _, w, _ in rows)
        fam.setdefault(sig, []).append(f)

    if args.show:
        sig = None
        for s, members in fam.items():
            if args.show in members:
                sig = s
                break
        if sig is None:
            raise SystemExit(f"{args.show} is not in {args.unit}"
                             f"{'' if args.all else ' (or is already C)'}")
        members = sorted(fam[sig])
        ref = fns[members[0]]
        # The rows that actually differ between members are the whole content
        # of a family: everything else is shared source. Transposed -- one
        # row per member -- because a 68-member family printed the other way
        # round is 68 columns wide and unreadable.
        vary = [i for i in range(len(sig))
                if len({fns[m][i][1] for m in members}) > 1]
        print(f"family of {len(members)}, {len(sig)} words, "
              f"{len(vary)} varying")
        print()
        print("  the shared shape (" + members[0] + "):")
        for i in range(len(sig)):
            print(f"  {i:4d} {'VARIES' if i in vary else '      '}  "
                  + ref[i][2])
        if not vary:
            print("\n  nothing varies: these are byte-identical functions.")
            return
        print()
        cols = [f"w{i}" for i in vary]
        w = max(14, max(len(c) for c in cols) + 2)
        print("  " + "function".ljust(16)
              + "".join(c.rjust(w) for c in cols))
        for m in members:
            cells = []
            for i in vary:
                word, text = fns[m][i][1], fns[m][i][2]
                sym = re.search(r"%lo\((\w+)\)", text)
                imm = re.search(r"(-?0x[0-9A-Fa-f]+)\s*$", text)
                if sym:
                    cells.append(sym.group(1).rjust(w))
                elif imm:
                    cells.append(imm.group(1).rjust(w))
                else:
                    cells.append(("%08X" % word).rjust(w))
            print("  " + m.ljust(16) + "".join(cells))
        return

    rows = []
    for sig, members in fam.items():
        if len(members) < args.min:
            continue
        exact = {}
        for m in members:
            exact.setdefault(tuple(w for _, w, _ in fns[m]), []).append(m)
        rows.append((len(members) * len(sig), len(members), len(sig),
                     max(len(v) for v in exact.values()), sorted(members)))
    rows.sort(reverse=True)

    print(f"{args.unit}: {len(fns)} functions considered, "
          f"{len(rows)} families with >={args.min} members")
    print(f"{'words':>6s} {'memb':>5s} {'size':>5s} {'ident':>6s}  representative")
    tot_f = tot_w = 0
    for total, n, size, ident, members in rows:
        tot_f += n
        tot_w += total
        print(f"{total:6d} {n:5d} {size:5d} {ident:6d}  {members[0]}"
              + (f"  (+{n - 1} more)" if n > 1 else ""))
    print(f"\n{tot_f} functions / {tot_w} words in families "
          f"({100.0 * tot_w / max(1, sum(len(v) for v in fns.values())):.1f}% "
          f"of the words considered)")
    print("`--show FUNC` to read one family's varying constants off.")


if __name__ == "__main__":
    main()
