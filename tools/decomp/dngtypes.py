#!/usr/bin/env python3
"""dngtypes.py -- find donors by TYPEDEF NAME, a route that has landed twice.

## WHY THIS EXISTS

Two bodies landed this week off a typedef name and nothing else, and no tool
reported either of them:

  * `func_s02_800249A0` (93 words, first compile). Its own unit carried an
    UNUSED `UnkDngS58Name12` typedef. Grepping that name named
    csrc/ovl_dng_s58.c, whose landed `func_s58_8002553C` is the exact
    skeleton -- and the typedef's alignment-1 is what produces the body's
    `lwl`/`lwr` run. dngtwin's best row (0.743) was never opened.
  * `func_s22_8002445C` (242 words). The donor `func_s14_800246D8` scores
    BELOW 0.60 and is invisible to every lead file, but its typedef
    `UnkDngS14LineSP` names a scratchpad struct that is this body's layout
    with two vertices where this one has three.

Both are the same observation: **a typedef is a layout, and a layout is shared
by exactly the family that uses that layout.** The name travels with the unit
that first needed it, so it points at the donor even when no ratio and no
callee set does.

An UNUSED typedef in a unit is the sharpest form: somebody declared it because
the unit's data needs it, and the body that uses it is somewhere else.

    python3 tools/decomp/dngtypes.py --unit s22      # this unit's typedefs
    python3 tools/decomp/dngtypes.py --unused        # every unused one, all units
    python3 tools/decomp/dngtypes.py UnkDngS14LineSP # who declares and uses it
"""
import argparse
import collections
import glob
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dngclone  # noqa: E402

PROJ = dngclone.PROJ
# `} Name;' is how every typedef in this tree closes.
CLOSE = re.compile(r"^\}\s*([A-Za-z_][A-Za-z0-9_]*)\s*;", re.M)


def sources():
    out = sorted(glob.glob(os.path.join(PROJ, "csrc", "*.c")))
    h = os.path.join(PROJ, "include", "structs.h")
    if os.path.exists(h):
        out.append(h)
    return out


def scan():
    """(declared_in, used_in) -- {name: [file]} each."""
    declared = collections.defaultdict(list)
    used = collections.defaultdict(list)
    texts = {}
    for p in sources():
        t = open(p, errors="replace").read()
        texts[p] = t
        for m in CLOSE.finditer(t):
            declared[m.group(1)].append(p)
    for name in declared:
        rx = re.compile(r"\b%s\b" % re.escape(name))
        for p, t in texts.items():
            # a declaration is two mentions (the struct tag and the close);
            # anything beyond that in the same file is a USE.
            n = len(rx.findall(t))
            if p in declared[name]:
                if n > 2:
                    used[name].append(p)
            elif n:
                used[name].append(p)
    return declared, used


def rel(p):
    return os.path.relpath(p, PROJ)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("name", nargs="?", help="a typedef name to look up")
    ap.add_argument("--unit", help="unit TAG (s22) or basename")
    ap.add_argument("--unused", action="store_true",
                    help="every typedef nobody uses -- the sharpest donor leads")
    a = ap.parse_args()
    declared, used = scan()

    if a.name:
        d = declared.get(a.name)
        if not d:
            print("%s: not declared anywhere" % a.name)
            return 1
        print("%s\n    declared in: %s" % (a.name, ", ".join(rel(x) for x in d)))
        u = used.get(a.name, [])
        print("    used in:     %s" % (", ".join(rel(x) for x in u) if u
                                       else "NOBODY -- this is a donor lead, "
                                            "find the body whose layout it is"))
        return 0

    if a.unit:
        tag = a.unit
        cand = [os.path.join(PROJ, "csrc", "ovl_dng_%s.c" % tag),
                os.path.join(PROJ, "csrc", "%s.c" % tag),
                os.path.join(PROJ, "csrc", tag)]
        path = next((c for c in cand if os.path.exists(c)), None)
        if path is None:
            print("dngtypes: no unit file for %r" % tag)
            return 2
        mine = [n for n, d in declared.items() if path in d]
        if not mine:
            print("%s declares no typedef" % rel(path))
            return 0
        print("%s declares %d typedef(s):" % (rel(path), len(mine)))
        for n in sorted(mine):
            u = [x for x in used.get(n, []) if x != path]
            here = path in used.get(n, [])
            mark = "" if here else "   <-- UNUSED HERE: a donor lead"
            print("    %-26s used in: %s%s"
                  % (n, ", ".join(rel(x) for x in u) if u else "nobody else",
                     mark))
        return 0

    rows = [(n, d) for n, d in declared.items() if not used.get(n)]
    print("%d typedef(s) declared and used by NOBODY -- each is a donor lead, "
          "because the layout exists for a reason:" % len(rows))
    for n, d in sorted(rows):
        print("    %-26s declared in %s" % (n, ", ".join(rel(x) for x in d)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
