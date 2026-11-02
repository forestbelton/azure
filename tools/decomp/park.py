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
    # carry the declarations the park needs for permimport's cpppsx pass
    io.open(os.path.join(PARKS, func + ".c"), "w", encoding="utf-8").write(
        '#include "common.h"\n\n/* %s\n\n   Parked by tools/decomp/park.py; the'
        ' live comment is in csrc/%s.c, which also carries the\n'
        '   declarations this needs. */\n\n%s' % (reason, unit, body))
    note = "/* NEAR MISS: %s\n   Source in docs/near-misses/%s.c. */\n" % (reason, func)
    io.open(UNIT, "w", encoding="utf-8").write(
        src[:m.start()] + note + STUB % func + src[m.end():])
    print("parked %s" % func)


if __name__ == "__main__":
    main()
