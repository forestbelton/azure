#!/usr/bin/env python3
"""lstcheck.py -- does every INCLUDE_ASM in a unit have a listing to include?

## THE GAP THIS CLOSES

A missing listing under `asm/nonmatchings/' does NOT break the build, so
nothing notices it. Found by agent-h93 in batch 23: `csrc/ovl_dng_b31f.c' had
98 `INCLUDE_ASM's and `asm/nonmatchings/ovl_dng_b31f/' held 97 listings --
`func_b31f_80174674.s' had been gone since batch 16, five batches earlier.

Why the build stays green:

  * `asm/nonmatchings/' is GENERATED and untracked by git, so a lost file
    leaves no trace in the history and `git status' is clean.
  * a listing is not a make dependency of the object that includes it, so an
    object built while the listing still existed is simply REUSED. All four
    containers stayed byte-identical across five batches.

The cost is latent and lands on whoever touches the unit next: an `ovlverify'
of it dies in the assembler (`can't open ... for reading'), which is how h93
found it while verifying a clone member there, and the first edit to that `.c'
breaks the build outright. One landable clone member was blocked on it.

So: a clean build is NOT evidence that a unit's listings are intact. Run this
before dispatching a batch, not after an agent trips over it.

## WHAT IT COMPARES

Per unit, the SET of names in `INCLUDE_ASM("...", <name>)' against the set of
`<name>.s' basenames in that unit's nonmatchings directory, and reports each
direction:

  MISSING   an INCLUDE_ASM with no listing -- the real defect.  The unit
            cannot be re-verified and the next edit to it will not build.
  ORPHAN    a listing with no INCLUDE_ASM -- normal and NOT reported by
            default: splat emits a listing for every function it detects,
            including the ones already landed in C.  Use --orphans to see
            them; a large orphan count is just a well-landed unit.

Usage:
    python3 tools/decomp/lstcheck.py                 # every unit
    python3 tools/decomp/lstcheck.py ovl_dng_b31f    # one unit
    python3 tools/decomp/lstcheck.py --orphans       # also list orphans
"""
import glob
import io
import os
import re
import sys

PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
INC = re.compile(r'INCLUDE_ASM\(\s*"[^"]*"\s*,\s*([A-Za-z_][A-Za-z0-9_]*)\s*\)')


def included(unit):
    p = os.path.join(PROJ, "csrc", unit + ".c")
    try:
        t = io.open(p, encoding="utf-8", errors="replace").read()
    except OSError:
        return None
    # a commented-out INCLUDE_ASM is not a build input
    t = re.sub(r"/\*.*?\*/", "", t, flags=re.S)
    t = re.sub(r"//[^\n]*", "", t)
    return {m.group(1) for m in INC.finditer(t)}


def listings(unit):
    d = os.path.join(PROJ, "asm", "nonmatchings", unit)
    return {os.path.basename(p)[:-2] for p in glob.glob(os.path.join(d, "*.s"))}


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    show_orphans = "--orphans" in sys.argv[1:]
    units = args or sorted(
        os.path.basename(p)[:-2]
        for p in glob.glob(os.path.join(PROJ, "csrc", "ovl_dng_*.c")))
    nunits = nbad = nmissing = 0
    for unit in units:
        inc = included(unit)
        if inc is None:
            print("%-22s NO SUCH UNIT" % unit)
            continue
        nunits += 1
        lst = listings(unit)
        missing = sorted(inc - lst)
        orphan = sorted(lst - inc)
        if missing:
            nbad += 1
            nmissing += len(missing)
            print("%s: %d INCLUDE_ASM(s) with NO LISTING" % (unit, len(missing)))
            for f in missing:
                print("    %s  -- asm/nonmatchings/%s/%s.s absent" % (f, unit, f))
        if show_orphans and orphan:
            print("%s: %d listing(s) with no INCLUDE_ASM (landed, normal)"
                  % (unit, len(orphan)))
    print("\n%d unit(s) checked, %d with a missing listing (%d listing(s) total)"
          % (nunits, nbad, nmissing))
    return 1 if nbad else 0


if __name__ == "__main__":
    sys.exit(main())
