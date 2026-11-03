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

STUB = r'^INCLUDE_ASM\("[^"]+",\s*%s\)\s*;'

# A signature's opening line, brace or no brace. The old DEF pattern required
# `{` on the SAME line, which this project never writes: its style is Allman
# (see the clang-format config), so the brace is always on its own line.
# Consequence, found on 2026-11-03 by carving ovl_town: preamble_end() matched
# no definition at all, fell through to the first INCLUDE_ASM, and swallowed
# every Allman definition above it into the "preamble" -- which carve.py then
# COPIED INTO ALL THREE UNITS. func_8008AC84 ended up defined three times and
# the link would have failed on a duplicate symbol. find_start() and find_end()
# had the same blindness, so carve.py could only ever cut at a STUB; a landed
# function was not carvable.
SIG = r"^[A-Za-z_][A-Za-z0-9_ \*]*\b(func_[0-9A-Fa-f]+)\s*\("


def is_def(lines, i, func=None):
    """Does lines[i] open a function DEFINITION (not a declaration)?

    Decided by what comes first scanning forward: a `{` means definition, a
    `;` means it was a declaration. Both forms can run over several lines --
    this unit has a declaration whose first line carries no `;` at all, which
    is the benign off-by-one the sweep recipe documents, and it must NOT be
    read as a definition here.
    """
    m = re.match(SIG, lines[i])
    if not m or (func is not None and m.group(1) != func):
        return False
    for j in range(i, min(i + 12, len(lines))):
        if "{" in lines[j]:
            return True
        if ";" in lines[j]:
            return False
    return False


def preamble_end(lines):
    """Index of the first function definition or stub -- the preamble ends there."""
    for i, l in enumerate(lines):
        if is_def(lines, i) or re.match(r'^INCLUDE_ASM\(', l):
            return i
    return 0


def find_start(lines, func):
    """Line index where `func` begins, including a comment block above it."""
    at = None
    for i, l in enumerate(lines):
        if is_def(lines, i, func) or re.match(STUB % re.escape(func), l):
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
        if is_def(lines, i, func) or re.match(STUB % re.escape(func), l):
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
    print()
    print("CHECK THESE THREE THINGS, each of which silently broke the ovl_town")
    print("carve on 2026-11-03 before it was caught:")
    print()
    print("1. AN OVERLAY UNIT NEEDS THE -G0 OVERRIDE. The Makefile now carries a")
    print("   `build/csrc/ovl_%.o:` PATTERN rule for CFLAGS and MASPSXFLAGS, so a")
    print("   new ovl_* unit is covered automatically -- but a unit named anything")
    print("   else is NOT, and inherits the resident's -G8. The first symptom was a")
    print("   landed 27-word function compiling to 29 words with cc1psx's own")
    print("   output IDENTICAL in both contexts, and the container 0xC0 bytes long.")
    print("   (A RESIDENT unit needs no Makefile change; that part was always true.)")
    print()
    print("2. THE NEW UNITS NEED DECLARATIONS FOR WHAT THEY CALL ACROSS THE CUT.")
    print("   A callee DEFINED above the cut is only a definition, not a")
    print("   declaration, so the unit that keeps the call sites loses its")
    print("   prototype. Taking an ADDRESS fails loudly (`undeclared`); a plain")
    print("   CALL rides an implicit declaration and SILENTLY CHANGES CODEGEN.")
    print("   After carving, build each unit and add a prototype for every")
    print("   `undeclared` name, then re-measure every landed function in all")
    print("   three units -- not just the one you were aiming at.")
    print()
    print("3. SPLAT NEEDS .rodata SUBSEGMENTS, NOT JUST `c` ONES, if the unit owns")
    print("   rodata. Each new unit's rodata must start and end on the boundaries")
    print("   its own symbols imply; splat's own `find_file_boundaries` suggestions")
    print("   in the yaml comments are usually exactly right. Then `make splat`.")
    print()
    print("If it was carved because its")
    print("functions reach a global $gp-relatively, say so in C instead: give")
    print("the unit a file-scope tentative definition of that object, e.g.")
    print("      u32 D_80081234;          /* no extern, no initialiser */")
    print("cc1psx emits .comm, maspsx files it under sbss_entries and rewrites")
    print("this unit's accesses to %gp_rel at the project default -G0. The")
    print("MASPSXFLAGS_G8/_G2 escape hatches were removed on 2026-11-03.")
    print("An ADDEND access (SYM+N) needs a .size record instead; see")
    print("docs/addressing-and-translation-units.md.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
