#!/usr/bin/env python3
"""park.py FUNC "one-line reason" -- move a near-miss out of the unit and into
docs/near-misses/, leaving an INCLUDE_ASM stub behind.

The park file is what tools/decomp/permimport.py reads, so parking a function
is also what makes it available to the permuter. The stub keeps the build
byte-identical while the search runs.

Works on any overlay unit: the unit is whichever csrc/*.c actually defines
FUNC, so this does not need to be told. The resident executable's parks
predate this and are already landed.
"""
import io
import os
import re
import sys

PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
PARKS = os.path.join(PROJ, "docs/near-misses")


def unit_defining(func):
    """The csrc unit that defines FUNC, and its splat unit name."""
    import glob
    pat = re.compile(r"^[A-Za-z_][\w \*]*?\b" + re.escape(func) + r"\s*\([^;]*?\)\n\{",
                     re.M)
    hits = [p for p in sorted(glob.glob(os.path.join(PROJ, "csrc", "*.c")))
            if pat.search(io.open(p, encoding="utf-8").read())]
    if len(hits) != 1:
        raise SystemExit("park.py: %s is defined in %d csrc units%s" %
                         (func, len(hits),
                          (": " + ", ".join(os.path.basename(h) for h in hits))
                          if hits else " (still an INCLUDE_ASM stub?)"))
    return hits[0], os.path.basename(hits[0])[:-2]


def main():
    if len(sys.argv) < 3:
        raise SystemExit("usage: park.py FUNC \"reason\"")
    func, reason = sys.argv[1], sys.argv[2]
    UNIT, unit = unit_defining(func)
    STUB = 'INCLUDE_ASM("asm/nonmatchings/%s", %%s);\n' % unit
    src = io.open(UNIT, encoding="utf-8").read()
    # the definition: a line starting at column 0 whose header names func,
    # through the closing brace at column 0.
    m = re.search(r"^[A-Za-z_][\w \*]*?\b" + re.escape(func) +
                  r"\s*\([^;]*?\)\n\{.*?\n\}\n", src, re.S | re.M)
    if not m:
        raise SystemExit("park.py: no definition of %s in %s" %
                         (func, os.path.relpath(UNIT, PROJ)))
    body = m.group(0)
    # THE HEADER COMMENT MUST COME FIRST, before the includes: parkstats.py
    # splits a park at the FIRST `*/` and treats everything after it as the
    # body, so an `#include` above the comment is dropped from the measurement
    # and the park reads as "does not build". This wrote the includes first
    # until 2026-11-11 and every park it made needed hand-reordering.
    #
    # The first line must also be exactly `func_XXXXXXXX (unit)` -- parkstats
    # finds the function and unit with that pattern, and will happily match a
    # `func_... (something)` further down the prose instead.
    #
    # The 0x80400000 twin's parks also need main_twin.h, and ONLY they do:
    # it declares the same objects as csrc/main.h at this block's own
    # addresses, so including it in a resident park would be wrong.
    inc = '#include "common.h"\n'
    if unit == "ovl_main_80400000":
        inc += '#include "main_twin.h"\n'
    io.open(os.path.join(PARKS, func + ".c"), "w", encoding="utf-8").write(
        '/* %s (%s)\n *\n * MEASURED. %s\n *\n'
        ' * Parked by tools/decomp/park.py; the live comment is in csrc/%s.c,\n'
        ' * which also carries the declarations this needs. */\n\n'
        '%s\n%s' % (func, unit, reason, unit, inc, body))
    note = "/* NEAR MISS: %s\n   Source in docs/near-misses/%s.c. */\n" % (reason, func)
    io.open(UNIT, "w", encoding="utf-8").write(
        src[:m.start()] + note + STUB % func + src[m.end():])
    print("parked %s" % func)


if __name__ == "__main__":
    main()
