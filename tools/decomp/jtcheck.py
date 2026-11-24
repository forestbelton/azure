#!/usr/bin/env python3
"""jtcheck.py -- how many CASES does a jump table have, and is its last word a
pad?  Reads the listing; needs no build.

## WHY THIS EXISTS

The dispatcher's pre-batch table check used to count `.word' directives and
report that count as the entry count, then declare an EVEN count pad-free.
That reasoning is INVERTED, and it went into five briefs in batch 24 before two
agents caught it independently:

    .word count = cases + pad,   and the pad is present when the CASE count is
                                 ODD and another rodata object follows

so an even `.word' count is exactly the SIGNAL for an odd case count plus one
`0x00000000' pad -- the opposite of what the briefs said.  Measured:

    jtbl_b55_80170838   10 words =  9 cases + pad
    jtbl_b55_80170860    8 words =  7 cases + pad
    jtbl_b44_80170888    6 words =  5 cases + pad
    jtbl_b55_80170808   12 words = 12 cases, genuinely pad-free

The cost of believing the count is a phantom `case' in the C, a longer table,
and a wrong container that NO tool catches -- a table's words are masked.

## WHAT IT READS INSTEAD

Two independent facts out of the listing, and it reports when they disagree:

  1. THE DISPATCH BOUND.  `sltiu $vX, $vY, N' immediately before the table's
     `%hi'/`%lo' load gives N cases.  This is the authority.
  2. THE LAST WORD'S CONTENT.  A `.word 0x00000000' where every other entry is
     a `.word .Lxxx' label is alignment, not an arm.

A disagreement is reported rather than resolved: the bound can be absent when
the index is already known in range, and the zero word can be a legitimate
entry if some arm really is at address 0 (it never is here).

## THE "N DISTINCT TARGET(S)" COLUMN IS NOT AN ARM-ORDER TEST

It counts how many labels the table points at, i.e. how much SHARING there is
between entries.  It says nothing about whether an arm's CODE can be told apart
after cross-joining, and the dispatcher wrongly used it that way in two batch-26
briefs.

agent-h109 disproved it on a 7-entry table whose targets were ALL DISTINCT:
permuting two arms measured 434 words / 0 differing under `ovlverify' and 0
aligned differences under `aligndiff', while a standalone `tools/cdkgcc -S'
showed the emitted table's last two entries swapped.  Those arms were each just
`j <label>' plus `li 1', and both tools mask a `j' target field.

The real test is PER ARM: does this arm contain a distinguishing instruction
other than a masked `j'/branch target?  Walk each arm's block to its first
`j'/`jal'/branch and compare the word sequences with `j' targets masked.  An
arm that reduces to a masked jump plus instructions identical to another arm's
is invisible to all three tools no matter how distinct its table entry is.

Usage:
    python3 tools/decomp/jtcheck.py                      # every unit
    python3 tools/decomp/jtcheck.py ovl_dng_b55          # one unit
    python3 tools/decomp/jtcheck.py --func func_b44_80175018
"""
import glob
import io
import os
import re
import sys

PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
JT = re.compile(r"dlabel (jtbl_\w+)(.*?)enddlabel", re.S)
WORD = re.compile(r"\.word\s+(\S+)")
# `sltiu $v0, $v1, 0xC' -- the bound, in the instruction stream before the load
SLTIU = re.compile(r"sltiu\s+\$\w+,\s*\$\w+,\s*(0x[0-9a-fA-F]+|\d+)")


def tables(path):
    t = io.open(path, encoding="utf-8", errors="replace").read()
    out = []
    for m in JT.finditer(t):
        words = WORD.findall(m.group(2))
        labels = [w for w in words if not re.fullmatch(r"0x0+", w)]
        pads = len(words) - len(labels)
        # the bound nearest this table's %hi load
        bound = None
        hi = t.find("%%hi(%s)" % m.group(1))
        if hi != -1:
            pre = t[max(0, hi - 4000):hi]
            b = SLTIU.findall(pre)
            if b:
                bound = int(b[-1], 16) if b[-1].startswith("0x") else int(b[-1])
        out.append((m.group(1), len(words), len(labels), pads, bound,
                    sorted(set(labels))))
    return out


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    only = None
    if "--func" in sys.argv:
        only = args[0] if args else None
        args = []
    paths = []
    if only:
        paths = glob.glob(os.path.join(PROJ, "asm", "nonmatchings", "*", only + ".s"))
    else:
        units = args or ["*"]
        for u in units:
            paths += glob.glob(os.path.join(PROJ, "asm", "nonmatchings", u, "*.s"))
    ntab = ndis = 0
    for p in sorted(paths):
        for name, nw, ncase, pads, bound, distinct in tables(p):
            ntab += 1
            owner = os.path.basename(p)[:-2]
            flag = ""
            if bound is not None and bound != ncase:
                flag = "  <-- BOUND SAYS %d, LABELS SAY %d" % (bound, ncase)
                ndis += 1
            print("%-24s %-22s %2d words = %2d cases + %d pad   bound %s   %d distinct target(s)%s"
                  % (owner, name, nw, ncase, pads,
                     str(bound) if bound is not None else "?", len(distinct), flag))
    print("\n%d table(s); %d where the dispatch bound disagrees with the label count"
          % (ntab, ndis))
    return 1 if ndis else 0


if __name__ == "__main__":
    sys.exit(main())
