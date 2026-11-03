#!/usr/bin/env python3
"""landedshapes.py [unit] -- stubs whose OPCODE SHAPE is already landed.

`families.py` groups only among REMAINING stubs, so it cannot tell you that a
stub is the same shape as a function already in csrc/. Those are the cheapest
work left: the landed body is a known-good starting point, and the per-member
edit is usually a symbol or a constant.

Two passes:

  --exact   raw words equal, jal fields included. A hit lands verbatim --
            rename the landed body and verify. (Expect none while families are
            being landed whole; this is a check, not a generator.)
  default   opcode shape only, registers and immediates masked. A hit is not a
            copy, but it tells you which landed function to start from.

Neither pass proves anything on its own. `ovlverify.py` is still the
measurement and `make build/<unit>.bin` is still the authority.

    python3 tools/decomp/landedshapes.py ovl_town
    python3 tools/decomp/landedshapes.py ovl_town --exact
"""
import collections
import glob
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(os.path.dirname(HERE)))

WORD = re.compile(r"^\s*/\* [0-9A-Fa-f]+ [0-9A-Fa-f]{8} ([0-9A-Fa-f]{8}) \*/")


def words_of(path):
    out = []
    for line in open(path):
        m = WORD.match(line)
        if m:
            out.append(m.group(1))
    return out


def shape(words, exact):
    if exact:
        return ",".join(words)
    out = []
    for w in words:
        # the listing prints little-endian bytes; rebuild the instruction word
        v = int.from_bytes(bytes.fromhex(w), "little")
        op = v >> 26
        out.append("R%02x" % (v & 0x3F) if op == 0 else "%02x" % op)
    return ",".join(out)


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    exact = "--exact" in sys.argv
    unit = args[0] if args else "ovl_town"

    asm = os.path.join(PROJ, "asm/nonmatchings", unit)
    csrc = os.path.join(PROJ, "csrc", unit + ".c")
    if not os.path.isdir(asm) or not os.path.exists(csrc):
        sys.exit("no such unit: %s" % unit)

    src = open(csrc).read()
    stubs = set(re.findall(
        r'INCLUDE_ASM\("asm/nonmatchings/%s", (func_[0-9A-Fa-f]+)\);' % unit, src))

    groups = collections.defaultdict(list)
    for p in sorted(glob.glob(os.path.join(asm, "func_*.s"))):
        w = words_of(p)
        if w:
            groups[shape(w, exact)].append((os.path.basename(p)[:-2], len(w)))

    rows = []
    for members in groups.values():
        names = [f for f, _ in members]
        st = [f for f in names if f in stubs]
        landed = [f for f in names if f not in stubs]
        if st and landed:
            rows.append((members[0][1], landed, st))

    rows.sort(key=lambda r: -r[0] * len(r[2]))
    for n, landed, st in rows:
        print("%4d words x %2d stub(s)  start from %-14s : %s"
              % (n, len(st), landed[0], " ".join(st)))
    print()
    print("%s: %d group(s), %d stub(s), %d words with a landed %s match"
          % (unit, len(rows), sum(len(s) for _, _, s in rows),
             sum(n * len(s) for n, _, s in rows),
             "exact" if exact else "shape"))


main()
