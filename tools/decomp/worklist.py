#!/usr/bin/env python3
"""worklist.py UNIT -- what to decompile next, ranked by words per reading.

The overlay sweep's unit of work is a FAMILY, not a function: one reading
produces source that lands every member with its constants substituted. So the
ranking is total words in the family, not function size -- a 3x25 family is
worth more than a single 60-word function and costs the same to read.

    python3 tools/decomp/worklist.py ovl_town              # next 20 items
    python3 tools/decomp/worklist.py ovl_town --count 6    # one agent round
    python3 tools/decomp/worklist.py ovl_town --singles    # include 1-member
    python3 tools/decomp/worklist.py ovl_town --summary    # what is left, in bulk

Columns: `words` is the whole family, `memb` how many functions one reading
lands, `size` the per-function word count, and `ident` how many members are
BYTE-IDENTICAL to the representative (those need no substitution at all).

Nothing here is a judgement about difficulty. A 279-word item may be a day and
a 6-word item may be a minute; the point is only to spend each reading where
it covers the most words. docs/town-sweep.md has the procedure.
"""
import argparse
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import asmlib  # noqa: E402

PROJ = asmlib.PROJ
ROW = re.compile(r"\s*/\* [0-9A-F]+ [0-9A-F]{8} ([0-9A-F]{8}) \*/\s+(.*)")
ITYPE = {0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
         0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2A, 0x2B,
         0x2E, 0x30, 0x31, 0x38, 0x39}


def mask(w):
    op = w >> 26
    if op in (2, 3):
        return op << 26
    return (w & 0xFFFF0000) if op in ITYPE else w


def read_unit(unit):
    d = os.path.join(PROJ, "asm", "nonmatchings", unit)
    if not os.path.isdir(d):
        raise SystemExit("no such unit listing: " + d)
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
            if inside:
                m = ROW.match(ln)
                if m:
                    rows.append(int.from_bytes(bytes.fromhex(m.group(1)), "little"))
        if rows:
            out[name[:-2]] = rows
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("unit")
    ap.add_argument("--count", type=int, default=20)
    ap.add_argument("--singles", action="store_true",
                    help="include families of one, i.e. functions with no twin")
    ap.add_argument("--summary", action="store_true")
    args = ap.parse_args()

    fns = read_unit(args.unit)
    cpath = os.path.join(PROJ, "csrc", args.unit + ".c")
    src = open(cpath).read() if os.path.exists(cpath) else ""
    stubs = set(re.findall(r'INCLUDE_ASM\("[^"]*",\s*(\w+)\)', src))
    # A function with a park file is not available work: somebody has already
    # spent the reading and written down where it stopped. Read the park before
    # touching it, and do not let it head the list forever.
    parks = {f[:-2] for f in os.listdir(os.path.join(PROJ, "docs/near-misses"))
             if f.startswith("func_") and f.endswith(".c")}
    todo = {k: v for k, v in fns.items() if k in stubs and k not in parks}
    if parks & stubs:
        print("parked, not listed: %s  (docs/near-misses/)"
              % " ".join(sorted(parks & stubs)))

    fam = {}
    for f, ws in todo.items():
        fam.setdefault(tuple(mask(w) for w in ws), []).append(f)

    rows = []
    for sig, members in fam.items():
        exact = {}
        for m in members:
            exact.setdefault(tuple(todo[m]), []).append(m)
        rows.append((len(members) * len(sig), len(members), len(sig),
                     max(len(v) for v in exact.values()), sorted(members)))
    rows.sort(reverse=True)

    multi = [r for r in rows if r[1] > 1]
    single = [r for r in rows if r[1] == 1]
    if args.summary:
        print("%s: %d stubs, %d words left" % (args.unit, len(todo),
                                               sum(len(v) for v in todo.values())))
        print("  %d families of 2+: %d functions, %d words -- one reading each"
              % (len(multi), sum(r[1] for r in multi), sum(r[0] for r in multi)))
        print("  %d with no twin:   %d words -- one reading per function"
              % (len(single), sum(r[0] for r in single)))
        return

    shown = multi if not args.singles else multi + single
    print("%s: %d stubs left, %d words" % (args.unit, len(todo),
                                           sum(len(v) for v in todo.values())))
    print("%6s %5s %5s %6s  %s" % ("words", "memb", "size", "ident", "representative"))
    for total, n, size, ident, members in shown[:args.count]:
        extra = ("  siblings: " + " ".join(members[1:])) if n > 1 else ""
        print("%6d %5d %5d %6d  %s%s" % (total, n, size, ident, members[0], extra))
    if len(shown) > args.count:
        print("... %d more items (%d words). --count N for more."
              % (len(shown) - args.count,
                 sum(r[0] for r in shown[args.count:])))


if __name__ == "__main__":
    main()
