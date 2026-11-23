#!/usr/bin/env python3
"""dngpick.py -- choose the next batch's units and functions by WORDS.

WHY NOT dngfam.py's RANKING ANY MORE. That tool ranks FAMILIES, and it was
the right lever while replication dominated: at the carve 1199 of 2575 stubs
were copies of a sibling and a 31-word body with 17 copies was worth more
than a 500-word one. It is not the right lever now. Of the 1,495 stubs left:

    1,176 one-member groups    212,075 words   89% of what remains
      131 multi-member groups   27,278 words   across 317 stubs

So ranking by family yield ranks the wrong 11%. This packs each unit by
`copies * words' over ALL its stubs -- which is the same objective, since a
one-member group simply scores its own words -- and reports the units that
pack best. Families still sort first on equal words, because a clone is free.

Units excluded by default:
  * `ovl_dng_lo16' -- its second jump table is 4 mod 8 relative to the rodata
    start, so a function of its that owns one cannot land until that segment
    gets a matching `c' split (docs/dng-sweep.md). 73 of its stubs are
    one-member, so there is no cheap work there either.
  * anything named with --busy, for a unit an agent still holds.

Usage:
    python3 tools/decomp/dngpick.py [-n 5] [--budget 850] [--busy u1,u2]
"""
import argparse
import collections
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dngfam  # noqa: E402


def stub_rows():
    """[(copies, words, yield, func, unit tag)] for every remaining stub."""
    stubs = dngfam.stubs()
    out = []
    for g in dngfam.families(landed=False).values():
        mem = [(c, n, w) for c, n, w in g if n in stubs]
        if not mem:
            continue
        for _c, n, w in mem:
            out.append((len(mem), w, len(mem) * w, n, n.split("_")[1]))
    return out


def pack(rows, budget, limit):
    """Greedy: best yield per word first, then biggest, to the word budget."""
    pick, w = [], 0
    for r in sorted(rows, key=lambda r: (-r[0], -r[1])):
        if w + r[1] > budget or len(pick) >= limit:
            continue
        pick.append(r)
        w += r[1]
    return pick, w


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-n", type=int, default=5, help="how many units")
    ap.add_argument("--budget", type=int, default=850,
                    help="words of representative body per agent")
    ap.add_argument("--limit", type=int, default=7,
                    help="most functions to give one agent")
    ap.add_argument("--busy", default="",
                    help="comma-separated unit tags an agent still holds")
    ap.add_argument("--skip", default="lo16",
                    help="comma-separated unit tags to leave alone")
    a = ap.parse_args()

    busy = {t for t in (a.busy.split(",") if a.busy else []) if t}
    skip = {t for t in (a.skip.split(",") if a.skip else []) if t} | busy

    by_unit = collections.defaultdict(list)
    for r in stub_rows():
        by_unit[r[4]].append(r)

    scored = []
    for tag, rows in by_unit.items():
        if tag in skip:
            continue
        pick, w = pack(rows, a.budget, a.limit)
        if pick:
            scored.append((sum(r[2] for r in pick), w, tag, pick))
    scored.sort(reverse=True)

    total = 0
    for y, w, tag, pick in scored[:a.n]:
        total += y
        print('  "%s": [%s],   # yield %d, %d words'
              % (tag, ",".join('(%d,%d,"%s")' % (r[0], r[1], r[3])
                               for r in pick), y, w))
    print("\n%d unit(s), %d words of yield" % (min(a.n, len(scored)), total))
    if busy:
        print("held by a live agent and skipped: %s" % ", ".join(sorted(busy)))


if __name__ == "__main__":
    main()
