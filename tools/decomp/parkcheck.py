#!/usr/bin/env python3
"""parkcheck.py [FILE...] -- structural check on docs/dungeon-parks/*.c.

These files are a function BODY under a hand-written header comment. They are
not compiled by anything, so nothing catches a malformed one -- and on
2026-11-16 three of thirty-five were malformed at once, two of them by my own
edits. The failure is always the same: an inner `/*` inside the header, which
in C terminates the OUTER comment at the first following `*/` and turns the
rest of the header into code.

It survives unnoticed because every tool that reads a park extracts from the
DEFINITION LINE onward and never sees the header, so the body measures fine
while the file as a whole is not valid C.

    python3 tools/decomp/parkcheck.py            # every park, exit 1 if any is bad
    python3 tools/decomp/parkcheck.py FILE ...   # just these
"""
import glob
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(os.path.dirname(HERE)))


def check(path):
    """Return a complaint string, or None."""
    try:
        s = open(path, encoding="utf-8").read()
    except OSError as e:
        return str(e)
    depth = 0
    for m in re.finditer(r"/\*|\*/", s):
        if m.group() == "/*":
            if depth:
                line = s.count("\n", 0, m.start()) + 1
                return ("inner `/*` at line %d -- it ends the header comment "
                        "early and orphans the rest" % line)
            depth = 1
        else:
            if not depth:
                line = s.count("\n", 0, m.start()) + 1
                return "stray `*/` at line %d" % line
            depth = 0
    if depth:
        return "unterminated comment"
    name = os.path.basename(path)[:-2]
    if not re.search(r"^[A-Za-z_][\w \*]*\b%s\s*\(" % re.escape(name), s, re.M):
        return ("no line defining %s at column 0 -- the merge scripts find the "
                "body by that line" % name)
    return None


def main():
    files = sys.argv[1:] or sorted(
        glob.glob(os.path.join(PROJ, "docs/dungeon-parks/func_*.c")))
    bad = 0
    for p in files:
        c = check(p)
        if c:
            print("%s: %s" % (os.path.relpath(p, PROJ), c))
            bad += 1
    print("%d park%s checked, %d bad" % (len(files), "" if len(files) == 1 else "s", bad))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
