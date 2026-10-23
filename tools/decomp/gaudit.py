#!/usr/bin/env python3
"""gaudit.py -- check the carved -G8/-G2 units for symbols that would come out
$gp-relative when the target reaches them absolutely.

Run it before landing a function into a carved unit, and after any change to a
`-G` assignment in the Makefile.

The mechanism, from docs/addressing-and-translation-units.md: gas emits a
GPREL16 relocation for any symbol whose `.extern SYM,size` record gives a size
in (0, G], and cc1psx emits that record from the C declaration. **gas never
sees the symbol's address** -- the symbol is undefined at assembly time -- so
the declared SIZE is the whole of the decision. A unit built with
MASPSXFLAGS_G8 therefore turns every <=8-byte extern it names into a
$gp-relative access, including ones the original reached absolutely.

So for each function still left as assembly in a carved unit, this finds the
symbols it reaches through a %hi/%lo pair (i.e. absolutely, in the original)
and reports any that include/structs.h declares as a scalar of <= G bytes. The
fix is always to widen the declaration past -G -- a real struct, or an
incomplete array -- so cc1psx compiler-splits the address itself.

A symbol OUTSIDE gp +/- 0x8000 is the worse case, not the safer one: it cannot
have been $gp-relative in the original, and the GPREL16 field cannot even hold
its offset. An earlier version of this rule had that backwards.

This only looks at functions still in assembly. A landed function's addressing
is already proven by `make check`, which is the real authority.
"""
import glob
import os
import re
import sys

PROJ = os.environ.get(
    "AZURE_ROOT",
    os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))),
)

SIZES = {"u8": 1, "s8": 1, "u16": 2, "s16": 2, "u32": 4, "s32": 4, "f32": 4}
GP = 0x80080994


def carved_units(makefile):
    """{unit: G} for every unit given a nonzero -G in the Makefile."""
    out = {}
    pat = r"((?:build/csrc/\S+\.o[ \\\n]*)+):\s*MASPSXFLAGS\s*:=\s*\$\((MASPSXFLAGS_G[0-9]+)\)"
    for block, flag in re.findall(pat, makefile):
        g = int(flag.rsplit("_G", 1)[1])
        for m in re.finditer(r"build/csrc/(\w+)\.o", block):
            # the Makefile's own usage example names a placeholder unit
            if m.group(1) != "some_unit":
                out[m.group(1)] = g
    return out


def scalar_decls(header):
    """{symbol: type} for every extern declared as a scalar (<= 4 bytes)."""
    return {
        m.group(2): m.group(1)
        for m in re.finditer(
            r"^extern\s+(u8|s8|u16|s16|u32|s32|f32)\s+(D_[0-9A-Fa-f]+)\s*;",
            header,
            re.M,
        )
    }


def main():
    mk = open(os.path.join(PROJ, "Makefile")).read()
    hdr = open(os.path.join(PROJ, "include", "structs.h")).read()
    units = carved_units(mk)
    scal = scalar_decls(hdr)

    problems = 0
    for unit, g in sorted(units.items()):
        listings = sorted(glob.glob(os.path.join(PROJ, "asm/nonmatchings", unit, "*.s")))
        rows = []
        for path in listings:
            text = open(path).read()
            for sym in sorted(set(re.findall(r"%(?:hi|lo)\((D_[0-9A-Fa-f]+)\)", text))):
                typ = scal.get(sym)
                if typ is None or SIZES[typ] > g:
                    continue
                off = int(sym[2:], 16) - GP
                where = (
                    "outside gp window (%+#x): GPREL16 cannot hold it either" % off
                    if abs(off) >= 0x8000
                    else "inside gp window (%+#x)" % off
                )
                rows.append((sym, typ, os.path.basename(path)[:-2], where))
        if rows:
            problems += len(rows)
            print("%s (-G%d), %d function(s) still asm:" % (unit, g, len(listings)))
            for sym, typ, fn, where in rows:
                print("  %-14s declared %-3s (%d byte%s) -- reached absolutely by %s"
                      % (sym, typ, SIZES[typ], "" if SIZES[typ] == 1 else "s", fn))
                print("  %-14s %s" % ("", where))
                print("  %-14s FIX: declare it past -G%d so cc1psx splits the address"
                      % ("", g))
            print()

    print("%d carved unit(s), %d scalar extern(s) declared, %d problem(s)"
          % (len(units), len(scal), problems))
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main())
