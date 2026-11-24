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

`--busy` DOES TWO THINGS, AND THE SECOND ONE MATTERS MORE. It skips the named
units, and it also skips every stub that shares a FAMILY with a stub in one
of them -- because a landing in unit X clones into its family members
wherever they live, so handing agent B a member of agent A's family means
whichever finishes first silently does B's work. That is not hypothetical:
batch 8 gave one agent three ovl_dng_b28 stubs and cloning another agent's
family landed all three mid-run.

Units excluded by default:
  * `ovl_dng_lo16' -- its second jump table is 4 mod 8 relative to the rodata
    start, so a function of its that owns one cannot land until that segment
    gets a matching `c' split (docs/dng-sweep.md). 73 of its stubs are
    one-member, so there is no cheap work there either.
  * anything named with --busy, for a unit an agent still holds.

ANYTHING THAT JOINS TWO OF THIS PROJECT'S "REMAINING WORK" VIEWS MUST
FILTER THROUGH `dngfam.stubs()`. They do not agree: `dngfam` reads csrc's
INCLUDE_ASM lines and is the authority; `xsibs.py` reads
asm/nonmatchings/<unit>/, which splat regenerates for LANDED functions too.
An analysis that fed xsibs' output straight into a batch plan reported 458
twinned stubs where there were 419, and 94,863 words where there were 44,220,
because it was counting functions landed in batch 1. This tool starts from
stub_rows(), which starts from stubs().

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
    """[(copies, words, yield, func, unit tag, family key)] per remaining stub."""
    stubs = dngfam.stubs()
    out = []
    for key, g in dngfam.families(landed=False).items():
        mem = [(c, n, w) for c, n, w in g if n in stubs]
        if not mem:
            continue
        for _c, n, w in mem:
            out.append((len(mem), w, len(mem) * w, n, n.split("_")[1], key))
    return out


def batch_yield(pick):
    """Words this selection is really worth, counting each family ONCE.

    A `copies * words' yield per row DOUBLE-COUNTS when two members of one
    family are both handed to the same agent: whichever lands first clones
    into the other, so the second is not additional yield. agent-h19 was
    given func_b44_80174A6C and func_b44_80174BC8, which `dngfam' calls one
    2-member family -- they differ in two immediates, so they are not even
    copies -- and the brief told it 348 words where the truth was 174. It
    landed both separately and nothing was lost, but the figure was wrong and
    an agent reading it would mis-price its own list.

    So: a family contributes `members * words' once, and the picked rows of
    that family contribute nothing further.
    """
    seen, total = set(), 0
    for r in pick:
        if r[5] in seen:
            continue
        seen.add(r[5])
        total += r[0] * r[1]
    return total


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

    rows = stub_rows()
    # Families already reachable from a busy unit: a landing there clones into
    # every member, so those members are not free work for anyone else.
    busy_fams = {r[5] for r in rows if r[4] in busy}
    by_unit = collections.defaultdict(list)
    held = 0
    for r in rows:
        if r[4] in skip:
            continue
        if r[5] in busy_fams:
            held += 1
            continue
        by_unit[r[4]].append(r)

    scored = []
    for tag, rows_u in by_unit.items():
        pick, w = pack(rows_u, a.budget, a.limit)
        if pick:
            scored.append((batch_yield(pick), w, tag, pick))
    scored.sort(reverse=True)

    # THE PICKED UNITS MUST NOT SHARE A FAMILY WITH EACH OTHER EITHER.
    # `--busy' guards against a family spanning THIS batch and a live agent's
    # unit; it says nothing about two units inside the same batch. Batch 9
    # picked b12, b18 and b20, all three of which hold a member of one
    # 345-word family, so three agents were given one family and whichever
    # landed first cloned into the other two. That is the third time this
    # class has cost an agent part of its run, so the batch is now built by
    # taking units in rank order and skipping any whose selection would
    # re-use a family already claimed.
    chosen, claimed, dropped = [], set(), []
    for y, w, tag, pick in scored:
        fams = {r[5] for r in pick}
        if fams & claimed:
            dropped.append(tag)
            continue
        # re-pack without the families already claimed, in case only part of
        # the unit's selection collides
        chosen.append((y, w, tag, pick))
        claimed |= fams
        if len(chosen) >= a.n:
            break

    total = 0
    for y, w, tag, pick in chosen:
        total += y
        print('  "%s": [%s],   # yield %d, %d words'
              % (tag, ",".join('(%d,%d,"%s")' % (r[0], r[1], r[3])
                               for r in pick), y, w))
    print("\n%d unit(s), %d words of yield" % (len(chosen), total))
    if dropped:
        print("skipped, a family they hold is already claimed in this batch: "
              "%s" % ", ".join(dropped[:8]))
    if busy:
        print("held by a live agent and skipped: %s" % ", ".join(sorted(busy)))
        print("plus %d stub(s) elsewhere sharing a family with one of those "
              "units -- a landing there would clone into them" % held)


if __name__ == "__main__":
    main()
