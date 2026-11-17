#!/usr/bin/env python3
"""parksplice.py FUNC OUT.c -- write the CURRENT unit with FUNC's park spliced in.

parkresidue.py does this into a temp directory and throws it away, which is
right for a ranking sweep and wrong for everything else.  Two jobs need the
spliced unit to PERSIST:

  * `permimport.py --source` wants a file that defines the function and
    compiles, and for a parked function that is exactly this;
  * an agent picking up a park wants to start from the measured body rather
    than re-deriving the splice and the declaration block by hand.

Both were being done by copy-paste, and a splice done by hand is where the
declarations get dropped -- that is how 19 of 34 parks stopped compiling.

    python3 tools/decomp/parksplice.py func_800C5FA8 /tmp/x/ovl_dungeon.c
    python3 tools/decomp/aligndiff.py /tmp/x/ovl_dungeon.c func_800C5FA8

KEEP THE BASENAME `ovl_dungeon.c`.  ovlverify.py picks which container ELF to
resolve against from the source file's BASENAME; anything else silently falls
back to build/town.bin.elf.  This script refuses another name rather than let
that happen quietly.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import parkresidue as pr


def main():
    if len([a for a in sys.argv[1:] if not a.startswith("--")]) != 2:
        print(__doc__)
        return 2
    func, out = sys.argv[1], sys.argv[2]
    alt = "--alt" in sys.argv
    if os.path.basename(out) != "ovl_dungeon.c":
        print("%s: the output basename must be ovl_dungeon.c (ovlverify picks "
              "the container ELF from it); got %r" % (out, os.path.basename(out)))
        return 2
    park = os.path.join(pr.PROJ, "docs/dungeon-parks/%s.c" % func)
    if not os.path.exists(park):
        print("%s: no park at %s" % (func, park))
        return 1
    body = pr.body_of(park, func)
    if body is None:
        print("%s: no definition line in the park" % func)
        return 1
    unit = open(pr.UNIT, encoding="utf-8").read()
    stub = 'INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", %s);' % func
    if unit.count(stub) != 1:
        print("%s: not a stub in the unit (landed already?)" % func)
        return 1
    decls = pr.decls_of(park) + pr.preamble_of(park, func)
    # SAME placement and prototype-adoption logic as parkresidue.measure, and
    # it has to stay the same: this file is what an agent picks up and what
    # permimport --source compiles, so a splice that differs from the one the
    # ranking measured hands them a different function to work on.
    cands = list(pr.splices(unit, stub, body, decls))
    text = cands[-1] if alt else cands[0]
    d = os.path.dirname(os.path.abspath(out))
    if d:
        os.makedirs(d, exist_ok=True)
    open(out, "w").write(text)
    print("%s: spliced into %s (%d declaration line(s); %d placement(s) "
          "available -- if it does not compile, try %s --alt)"
          % (func, out, len(decls.strip().split("\n")) if decls else 0,
             len(cands), os.path.basename(__file__)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
