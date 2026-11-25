#!/usr/bin/env python3
"""dngnear.py -- leads from the CALL GRAPH, which the ratio provably cannot reach.

## THE GAP THIS CLOSES

`dngtwin` ranks on mnemonic similarity inside a 0.75-1.33 WORD-COUNT BAND, so a
donor that is much shorter or much longer than the target is invisible to it at
ANY ratio. Two bodies in batch 28 landed off exactly such a donor:

  * `func_b16_80173904` (121 words). Its only offered lead was 0.609 and was
    not used. The real donor is `func_b18_8017406C` at 77 words -- 44 short,
    outside the band -- reached by landing the DRIVER `func_b16_80173738` off
    its twin `func_b18_801741A0`, then reading THAT twin's CALLER.
  * the same relationship named the alias split, the dress skeleton and the
    declaration block, for zero measurements.

The driver/spawner pair is a stable unit in this block and the relationship
SURVIVES THE RELINK even when the word counts diverge past any shape metric.

## THE INFERENCE

For a stub S in unit U, with an in-unit neighbour N (S calls N, or N calls S):

    if N is LANDED and has a landed twin T in unit V,
    then whatever in V bears the SAME call relation to T
    is a donor for S -- at any word count, at any ratio.

It is a structural argument, not a statistical one, which is why it reaches the
cases three measured statistical approaches to blends all failed on.

## WHERE IT RETURNS NOTHING, AND WHY -- MEASURED ON ITS FIRST BATCH

The inference needs a LANDED neighbour to invert, so its yield scales with how
much of your unit is already done. Run over batch 29's five units:

    ovl_dng_s51   31 donors      ovl_dng_b33   14      ovl_dng_b09   12
    ovl_dng_s13    0 donors      ovl_dng_s15    0

s13 and s15 are mostly unlanded, so their stubs' in-unit call neighbours are
themselves stubs and there is nothing to invert. **A unit early in its life
gets nothing from this tool**; a unit most of the way landed gets the most. That
is the opposite of the ratio's behaviour, which is best when a near-identical
sibling exists anywhere in the tree, so the two are complements rather than
substitutes -- and when both come up empty, the FIELD-ORDER GREP is the third
recipe (see docs/matching-levers.md).

## WHAT IT DOES NOT CLAIM

A donor, not a twin: no ratio is printed for the S-to-donor pair because none
is meaningful. Confirm by reading the donor's SKELETON -- its callee sequence
and its dress sites -- not by expecting a word count to agree.
"""
import collections, json, os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dngtwin

PROJ = "/home/case/projects/azure"
HUBMAX = 4       # see the hub note in main(); 2 discarded the known-good case
TOPDONORS = 10   # was 6; a correct forwarder filter cost the known-good donor
                 # its second path, and it fell below a 6-row cut. See NOTE.
VIAMIN = 40      # a smaller landed neighbour is a generic forwarder -- see NOTE
UBIQUITY = 0.5   # a donor offered for more than this share of a unit's stubs
                 # carries no information about any one of them
SWEEP = os.path.join(PROJ, "docs", "dng-twinsweep.json")
FUNC = re.compile(r"\bfunc_[A-Za-z][A-Za-z0-9]*_[0-9A-Fa-f]{8}\b")
DEFN = re.compile(r"^[A-Za-z_][A-Za-z0-9_ \t*]*?\b(func_[A-Za-z][A-Za-z0-9]*_"
                  r"[0-9A-Fa-f]{8})\s*\([^;]*$")
STUB = re.compile(r'INCLUDE_ASM\("[^"]*",\s*(func_\w+)\)')
JAL = re.compile(r"\bjal\s+(func_[A-Za-z][A-Za-z0-9]*_[0-9A-Fa-f]{8})")


def tag_of(f):
    m = re.match(r"^func_([A-Za-z][A-Za-z0-9]*)_", f)
    return m.group(1) if m else None


def unit_graph(tag):
    """-> (landed set, stub set, {caller: {callee}}) for one unit."""
    p = os.path.join(PROJ, "csrc", "ovl_dng_%s.c" % tag)
    if not os.path.exists(p):
        return set(), set(), {}
    txt = open(p, errors="replace").read()
    lines = txt.split("\n")
    stubs = set(STUB.findall(txt))
    landed, edges, cur = set(), collections.defaultdict(set), None
    for ln in lines:
        m = DEFN.match(ln)
        if m and m.group(1) not in stubs:
            cur = m.group(1)
            landed.add(cur)
            continue
        if cur:
            for c in FUNC.findall(ln):
                if c != cur:
                    edges[cur].add(c)
    # a stub's callees come from its listing, which still exists
    for s in stubs:
        lp = os.path.join(PROJ, "asm", "nonmatchings", "ovl_dng_%s" % tag,
                          s + ".s")
        if os.path.exists(lp):
            edges[s] |= set(JAL.findall(open(lp, errors="replace").read()))
    return landed, stubs, edges


def main():
    # --unit's VALUE must not fall through into `want'; it did, and the tool
    # printed a row for "ovl_dng_b33  (ovl_dng_None)" before anything useful.
    argv, want, unit = sys.argv[1:], [], None
    i = 0
    while i < len(argv):
        a = argv[i]
        if a == "--unit" and i + 1 < len(argv):
            unit = argv[i + 1].replace("ovl_dng_", "")
            i += 2
            continue
        if not a.startswith("-"):
            want.append(a)
        i += 1
    # NOT the stub sweep: it holds only STUBS, and the neighbour whose twin we
    # need is by definition LANDED. Looking it up there found nothing on the
    # very case this tool was written for, which is how the bug was caught --
    # rank against the pool directly instead.
    pool = dngtwin.carved(18)
    cache, tcache = {}, {}

    def twins_of(N):
        if N not in tcache:
            me = pool.get(N)
            tcache[N] = ([] if me is None else
                         [(r, nm, ot) for r, ot, nm, _w, _fk, is_stub
                          in dngtwin.rank(me, {k: v for k, v in pool.items()
                                               if k != N}, False, 0.60, 4)
                          if not is_stub])
        return tcache[N]

    def graph(tag):
        if tag not in cache:
            cache[tag] = unit_graph(tag)
        return cache[tag]

    if unit:
        _, stubs, _ = graph(unit)
        want += sorted(stubs)
    if not want:
        raise SystemExit("usage: dngnear.py FUNC... | --unit ovl_dng_<tag>")

    # TWO PASSES, because the strongest suppression is CROSS-STUB.
    #
    # Measured 2026-11-24 on ovl_dng_b09: a 25-word two-gate forwarder
    # (func_b09_80172F74) is a call neighbour of EVERY stub in the unit, its
    # twin is 1.000 in several units, and so the same donor was offered -- and
    # flagged STRONG by the corroboration ranking -- for all of them. The
    # agent's words: "that neighbour's twin has the same relation to
    # everything, so the row carries no information". It is the hub problem
    # again, but on the NEIGHBOUR side, where HUBMAX cannot see it.
    #
    # So: a donor offered for more than UBIQUITY of a unit's stubs is dropped,
    # and a via-neighbour under VIAMIN words is not used at all. Both make the
    # tool quieter and neither can hide a specific lead.
    per_stub, nbr_count = {}, {}
    for S in want:
        tag = tag_of(S)
        landed, stubs, edges = graph(tag)
        nbrs = set(e for e in edges.get(S, ()) if tag_of(e) == tag)
        nbrs |= set(c for c, cs in edges.items() if S in cs and tag_of(c) == tag)
        nbrs = sorted(n for n in nbrs if n in landed)
        small = [n for n in nbrs
                 if pool.get(n) is not None and len(pool[n][1]) < VIAMIN]
        nbrs = [n for n in nbrs if n not in small]
        found, seen = collections.defaultdict(list), collections.defaultdict(set)
        for N in nbrs:
            for ratio, T, vtag in twins_of(N)[:3]:
                vl, vs, ve = graph(vtag)
                if T not in vl:
                    continue
                rel = sorted(set(ve.get(T, ())) | set(
                    c for c, cs in ve.items() if T in cs))
                rel = [r for r in rel if tag_of(r) == vtag and r in vl and r != T]
                if not rel or len(rel) > HUBMAX:
                    continue
                for r in rel:
                    key = (N, T, vtag)
                    if key not in seen[r]:
                        seen[r].add(key)
                        found[r].append((N, T, ratio, vtag, len(rel)))
        per_stub[S] = (found, len(nbrs), small)
        for r in found:
            nbr_count[r] = nbr_count.get(r, 0) + 1

    nstubs = max(len(want), 1)
    ubiq = {r for r, c in nbr_count.items() if c > UBIQUITY * nstubs and nstubs > 2}

    for S in want:
        found, nnbr, small = per_stub[S]
        print("\n%s  (ovl_dng_%s)" % (S, tag_of(S)))
        if small:
            print("    (ignoring %d neighbour(s) under %d words -- generic "
                  "forwarders carry no relation: %s)"
                  % (len(small), VIAMIN, ", ".join(small)))
        # GROUPED BY VIA-NEIGHBOUR, NOT RANKED BY DONOR.
        #
        # This was a ranked donor list and the ranking was the wrong design.
        # Ranking needs a scalar, the only scalars available are the twin
        # ratio and the relative count, and NEITHER measures what makes a
        # donor right -- the known-good case (func_b18_8017406C for
        # func_b16_80173904) has one path and three relatives, so it sorted
        # 11th behind ten single-path donors with fewer relatives, and raising
        # the cut to 10 did not reach it. The tool's job is to ENUMERATE the
        # structural candidates and show the reasoning; choosing among them is
        # the agent's, with the listing in hand.
        kept = {r: v for r, v in found.items() if r not in ubiq}
        bygroup = collections.defaultdict(list)
        for r, paths in kept.items():
            for N, T, ratio, vtag, n in paths:
                bygroup[(N, T, ratio, vtag)].append(r)
        hits = 0
        for (N, T, ratio, vtag), rs in sorted(
                bygroup.items(), key=lambda kv: (len(kv[1]), -kv[0][2])):
            hits += 1
            print("    via %s (landed here) -> twin %s %.3f in %s"
                  % (N, T, ratio, vtag))
            for r in sorted(rs):
                extra = ("  [also reached via %d other path(s)]"
                         % (len(kept[r]) - 1) if len(kept[r]) > 1 else "")
                print("        DONOR %-24s csrc/ovl_dng_%s.c%s"
                      % (r, vtag, extra))
        drop = len(found) - len(kept)
        if drop:
            print("    (%d donor(s) suppressed: offered for most of this "
                  "unit's stubs, so uninformative)" % drop)
        if not hits:
            if nnbr == 0:
                print("    no usable LANDED in-unit call neighbour -- nothing "
                      "to invert")
            else:
                print("    %d landed neighbour(s), but none has a landed twin "
                      "with a named relative" % nnbr)


if __name__ == "__main__":
    main()
