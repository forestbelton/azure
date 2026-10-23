#!/usr/bin/env python3
"""carve.py UNIT FIRST LAST NEWUNIT [TAILUNIT] -- split a C unit in three.

Cuts the functions FIRST..LAST (inclusive) out of csrc/UNIT.c into
csrc/NEWUNIT.c, leaving everything after them in csrc/TAILUNIT.c. Prints the
splat subsegment lines and the Makefile addition to apply.

This exists because doing it by hand goes wrong in the same two ways every
time:

  * the preamble is not a fixed number of lines. Each unit has its own
    `#include` plus prototype block, and slicing at a count borrowed from
    another file cuts into a function body -- which is exactly how a tail file
    came out starting mid-function and failing to parse.
  * a comment block immediately above a function belongs to it, so the cut has
    to be made above the comment, not between the comment and the function.

Both are mechanical, so they belong here rather than in a one-off script.

    python3 tools/decomp/carve.py unk_11940 func_8003F240 func_8003F240 \\
        gprel_12A40 unk_12A70

Nothing is written unless every boundary is found. Run `make splat` and
`make check` afterwards; the carve itself only moves text.
"""
import os
import re
import sys

PROJ = os.environ.get(
    "AZURE_ROOT",
    os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))),
)

DEF = r"^[A-Za-z_][A-Za-z0-9_ \*]*\b%s\s*\([^;]*\)\s*\{"
STUB = r'^INCLUDE_ASM\("[^"]+",\s*%s\)\s*;'


def preamble_end(lines):
    """Index of the first function definition or stub -- the preamble ends there."""
    for i, l in enumerate(lines):
        if re.match(r"^[A-Za-z_][A-Za-z0-9_ \*]*\bfunc_[0-9A-Fa-f]+\([^;]*\)\s*\{", l) \
                or re.match(r'^INCLUDE_ASM\(', l):
            return i
    return 0


def find_start(lines, func):
    """Line index where `func` begins, including a comment block above it."""
    at = None
    for i, l in enumerate(lines):
        if re.match(DEF % re.escape(func), l) or re.match(STUB % re.escape(func), l):
            at = i
            break
    if at is None:
        return None
    # walk back over a block comment that belongs to this function
    j = at
    while j > 0 and lines[j - 1].strip() == "":
        j -= 1
    if j > 0 and lines[j - 1].rstrip().endswith("*/"):
        k = j - 1
        while k > 0 and not lines[k].lstrip().startswith("/*"):
            k -= 1
        if lines[k].lstrip().startswith("/*"):
            j = k
    return j


def find_end(lines, func):
    """Line index just past `func`'s body (its closing brace at column 0)."""
    at = None
    for i, l in enumerate(lines):
        if re.match(DEF % re.escape(func), l) or re.match(STUB % re.escape(func), l):
            at = i
            break
    if at is None:
        return None
    if re.match(STUB % re.escape(func), lines[at]):
        return at + 1
    depth = 0
    for i in range(at, len(lines)):
        depth += lines[i].count("{") - lines[i].count("}")
        if depth == 0 and i > at:
            return i + 1
        if depth == 0 and lines[i].rstrip().endswith("}"):
            return i + 1
    return None


def main():
    if len(sys.argv) not in (5, 6):
        print((__doc__ or "").strip())
        return 2
    unit, first, last, newunit = sys.argv[1:5]
    tailunit = sys.argv[5] if len(sys.argv) == 6 else None

    path = os.path.join(PROJ, "csrc", unit + ".c")
    lines = open(path).read().split("\n")
    pre = preamble_end(lines)
    a = find_start(lines, first)
    b = find_end(lines, last)
    if a is None:
        print("carve.py: %s not found in %s" % (first, unit))
        return 1
    if b is None:
        print("carve.py: could not find the end of %s in %s" % (last, unit))
        return 1
    if not (pre <= a < b <= len(lines)):
        print("carve.py: boundaries out of order (preamble %d, %d..%d)" % (pre, a, b))
        return 1

    preamble = "\n".join(lines[:pre]).rstrip()
    head = "\n".join(lines[:a]).rstrip()
    body = "\n".join(lines[a:b]).strip("\n")
    tail = "\n".join(lines[b:]).strip("\n")

    def repoint(text, name):
        return text.replace('asm/nonmatchings/%s"' % unit,
                            'asm/nonmatchings/%s"' % name)

    open(path, "w").write(head + "\n")
    with open(os.path.join(PROJ, "csrc", newunit + ".c"), "w") as fh:
        fh.write("/* Carved out of %s; see docs/addressing-and-translation-units.md.\n"
                 " * Replace this header with the evidence for the split. */\n" % unit)
        fh.write(preamble + "\n\n" + repoint(body, newunit) + "\n")
    if tailunit and tail:
        with open(os.path.join(PROJ, "csrc", tailunit + ".c"), "w") as fh:
            fh.write("/* The remainder of %s after the %s carve-out. Named for its\n"
                     " * start offset. */\n" % (unit, newunit))
            fh.write(preamble + "\n\n" + repoint(tail, tailunit) + "\n")

    print("preamble: lines 1..%d" % pre)
    print("%-16s lines 1..%d" % (unit, a))
    print("%-16s lines %d..%d" % (newunit, a + 1, b))
    if tailunit:
        print("%-16s lines %d..%d" % (tailunit, b + 1, len(lines)))
    print()
    print("now add to splat/slus_006.14.yaml, after the %s entry:" % unit)
    print("      - [0x?????, c, %s]" % newunit)
    if tailunit:
        print("      - [0x?????, c, %s]" % tailunit)
    print("and to the MASPSXFLAGS_G8 list in the Makefile: build/csrc/%s.o"
          % newunit)
    return 0


if __name__ == "__main__":
    sys.exit(main())
