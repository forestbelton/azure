#!/usr/bin/env python3
"""relocheck.py -- does a LANDED body reference the SAME data symbols the
target does? `ovlverify' cannot tell you, and a clean landing can be wrong.

## THE GAP THIS CLOSES

`ovlverify' compares words with the relocated fields MASKED, because a
relocation's value is decided at link time and the candidate's cannot match
the ROM's until it is linked. That is the right call for code, and it means
**the identity of a relocated symbol is invisible to it**. A dress site that
loads the wrong bank-local table measures 0 differing and the body is still
wrong.

Found by agent-h46 in batch 14, on a function that landed at 365/365 and 0
differing: `func_b31_801716F4' has three facing-table dress sites, and the
0x80000 arm must load `D_8017555C' where the other two load `D_80175554'.
Writing `D_80175554' at all three ALSO measures 365 words and 0 aligned
differences. `ovlverify --raw' does not settle it either -- raw 87 differing
is exactly the relocation COUNT, and a wrong symbol of the same kind keeps
the count. The listing's relocations were the only evidence, and nothing
checked them.

## WHAT IT COMPARES

The multiset of RAM data symbols (`D_8xxxxxxx') each function references, taken
from the splat listing (the target) and from the built assembly maspsx
produced (what we actually compile). A multiset, not a sequence, because
sched and reorg move loads around freely and an order difference is not a
defect -- but a symbol that appears in one side and not the other is.

Deliberately NOT checked here: `func_*' call targets, which `ovlverify'
already pins through the `jal' opcode and whose names can legitimately differ
between the listing's address form and a real name from `common_syms.txt'
(`func_80065420' against `RotTransPers'), and `%lo' displacement forms, which
the two sides spell differently. Those need the name map; data symbols do
not, so this stays sound without one.

A unit must have been BUILT for its assembly to exist, so run this after
`make check', never instead of it.

Usage:
    python3 tools/decomp/relocheck.py                  # every built unit
    python3 tools/decomp/relocheck.py ovl_dng_b31      # one unit
"""
import collections
import glob
import os
import re
import sys

PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
# Only RAM data symbols, `D_8xxxxxxx' and the bank-local `D_<tag>_8xxxxxxx'.
# Two kinds of name are deliberately out of scope because the two sides cannot
# spell them the same, and including them produced 71 false reports on the
# first run:
#
#   D_1F8000xx   the scratchpad. C writes it as an absolute constant
#                (`*(s32 *)0x1F80013C'), so the build has no symbol at all
#                where the listing names one. 69 of the 71.
#   jtbl_*       a LANDED jump table is cc1psx's own `$L95'-style local label,
#                not the listing's `jtbl_' name, so a landed table owner
#                always looks like it dropped its table. The remaining 2.
#                `ovlverify' pins a table's CONTENT through the words and the
#                dispatcher checks its PLACEMENT at the build gate, so this
#                tool is not the thing that should chase them.
SYM = re.compile(r"\b(D_(?:[a-z0-9]+_)?8[0-9A-Fa-f]{7})\b")


def listing_syms(unit, func):
    p = os.path.join(PROJ, "asm", "nonmatchings", unit, func + ".s")
    try:
        t = open(p, errors="replace").read()
    except OSError:
        return None
    # A jump table BELONGS to the function, it is not a reference to one, and
    # the listing defines it in a `.section .rodata' block at the head.
    t = re.sub(r"\.section \.rodata.*?\.section \.text", "", t, flags=re.S)
    return collections.Counter(m.group(1) for m in SYM.finditer(t))


def built_syms(unit):
    """{func: Counter(symbols)} from the built assembly, per function body."""
    p = os.path.join(PROJ, "build", "csrc", unit + ".s")
    try:
        lines = open(p, errors="replace").read().split("\n")
    except OSError:
        return None
    out, cur, buf = {}, None, []
    for ln in lines:
        m = re.match(r"^(func_\w+):", ln)
        if m:
            cur, buf = m.group(1), []
            continue
        if re.match(r"^\s*\.end\b", ln) and cur:
            out[cur] = collections.Counter(x.group(1) for x in SYM.finditer("\n".join(buf)))
            cur = None
            continue
        if cur is not None:
            buf.append(ln)
    return out


def check(unit):
    built = built_syms(unit)
    if built is None:
        return None
    rows = []
    for func, got in sorted(built.items()):
        want = listing_syms(unit, func)
        if want is None:
            continue
        missing = want - got
        extra = got - want
        if missing or extra:
            rows.append((func, missing, extra))
    return len(built), rows


def main():
    units = sys.argv[1:] or sorted(
        os.path.basename(p)[:-2]
        for p in glob.glob(os.path.join(PROJ, "build", "csrc", "ovl_dng_*.s")))
    nf = nb = 0
    for unit in units:
        r = check(unit)
        if r is None:
            print("%-22s NOT BUILT -- run make check first" % unit)
            continue
        n, rows = r
        nf += n
        for func, missing, extra in rows:
            nb += 1
            print("%s: %s" % (unit, func))
            if missing:
                print("    the LISTING references, the build does not: %s"
                      % ", ".join(sorted(missing.elements())))
            if extra:
                print("    the BUILD references, the listing does not: %s"
                      % ", ".join(sorted(extra.elements())))
    print("\n%d landed bodies checked, %d with a data-symbol mismatch" % (nf, nb))


if __name__ == "__main__":
    main()
