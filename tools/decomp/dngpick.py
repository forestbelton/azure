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

Excluded by default:
  * EVERY FUNCTION THAT OWNS A JUMP TABLE IN A UNIT WITH NO `.rodata'
    SUBSEGMENT -- computed, not named. Such a function verifies at 0
    differing and then moves the container, because ovlverify measures a
    function's own words and not where its table lands. Today that is the two
    jump-table owners in ovl_dng_lo16, and they are blocked for DIFFERENT
    reasons (re-measured in batch 12, docs/dng-sweep.md):

      func_lo16_8001D4AC  owns jtbl_lo16_80016000, at OFFSET 0 of the region,
        where cc1psx's `.align 3' is a no-op. LANDABLE as soon as 0x2a3000 is
        attributed `.rodata, ovl_dng_lo16' instead of being a bare `data'
        subsegment. 75 words waiting on a one-line yaml change and a build.
      func_lo16_8001E824  owns jtbl_lo16_80016034, 4 mod 8. NEVER landable.
        final.c:1517 emits `ASM_OUTPUT_ALIGN (exact_log2 (BIGGEST_ALIGNMENT /
        BITS_PER_UNIT))' before every ADDR_VEC, and mips.h:1080 sets
        BIGGEST_ALIGNMENT 64, so every jump table this compiler emits is
        8-aligned -- there is no flag. A `c' split does not help: an
        8-aligned section cannot be placed at a 4-mod-8 address either.
        82 words, permanently INCLUDE_ASM.

    A 4-mod-8 table is fine while its owner is a STUB: measured over all 223
    stub jump tables, splat emits `.align 3' for the 222 that are 8-aligned
    and NO directive for the one that is not (jtbl_b31f_8016A8B4), which is
    why b31f is byte-identical today.
  * anything named with --busy, for a unit an agent still holds.

`ovl_dng_lo16' WAS EXCLUDED WHOLESALE AND THAT WAS WRONG. The blocker is
real but it covers 2 of its 76 stubs, 157 of its 9,538 words; the other 74
need nothing and were invisible to every batch pick for ten batches. The
other half of the old rationale -- that 73 of its stubs are one-member, so
the yield per agent is poor -- is a reason to RANK it last, which this script
already does by scoring copies x words, and not a reason to hide it. Skipping
a whole unit to express "expensive" silently converts it into "not work".

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
import json
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dngfam  # noqa: E402


def parked():
    """Functions with a file under docs/dng-parks/ -- EXCLUDED from picking.

    A park is a body that has been written and does NOT land, with the reason
    recorded.  Picking one spends an agent slot on work already proved blocked:
    in batch 25 `func_b38_8017087C' was handed to an agent whose unit could
    therefore only yield 3 of its 4 functions.  That park is 124 words at
    0 differing against a correctly-split listing and is blocked on the unit's
    `.rodata' needing to grow 4 bytes -- a layout decision, not a C problem, so
    no amount of agent time moves it.

    Re-picking a park is only right once its blocker is cleared, and that is a
    deliberate act; use --with-parks to include them.

    BUT A PARK'S LEAD SET AGES, AND THIS EXCLUSION USED TO BE PERMANENT.
    Measured 2026-11-24: 8 of the 16 parks (2,575 of 4,952 park words) now have
    a LANDED twin at >= 0.60 that did not exist when they were parked, six of
    them at 0.84 or better -- and `func_b38_8017087C' scores 0.952 against
    `func_b41_8017088C', which landed in the very batch that measured this. A
    park written against a smaller pool is not evidence about the pool we have
    now.

    The blocker CLASS decides whether a new twin helps, so run --parks and read
    it rather than trusting the ratio:

      * a park blocked on measurable C-LEVEL RESIDUE (an aligned-difference
        count) is re-pickable the moment a better donor exists -- 7 of those 8.
      * a park blocked on LAYOUT (`.rodata' sizing, a subsegment boundary) is
        NOT unblocked by any twin, however high. `func_b38_8017087C' is 0.952
        and still blocked, because its 4 bytes of `.rodata' are a yaml decision
        and no amount of C moves them. Its ratio is the most misleading number
        in the backlog.
    """
    d = os.path.join(PROJ, "docs", "dng-parks")
    try:
        return {f[:-2] for f in os.listdir(d)
                if f.startswith("func_") and f.endswith(".c")}
    except OSError:
        return set()


# THE HEADLINE RESIDUE: MATCH ACROSS A LINE BREAK, THEN TAKE THE LAST.
#
# The original pattern had a LITERAL SPACE in "ALIGNED DIFFERENCE" and park
# headlines wrap ("... 25 ALIGNED\n   DIFFERENCES"), so it matched NO headline
# and fell through to the first unwrapped number in the file -- a rejected
# spelling's price for one park (125 where the truth is 25) and ANOTHER
# FUNCTION'S figure for another (23 where the truth is 5; park files quote a
# sibling member's state as context). `\s+' is the actual fix; taking the last
# match is what survives append-only UPDATE sections on top of it.
#
# Park files follow a convention I did not notice when I wrote this: the
# current residue is written with ALIGNED in CAPS near the top, rejected
# spellings in the priced table use lowercase, and an agent that NARROWS a
# park appends a new caps figure rather than editing the old one. So a park
# carries its whole history and the newest figure is last.
#
# My first version was case-insensitive and took the FIRST match. On
# func_s21_80025BA0 -- narrowed 23 -> 16 -> 5 by two earlier agents -- that
# reported **23** where the truth was **5**, and the brief I generated from it
# told an agent the body was four times further from landing than it is. The
# agent caught it ("my measurement puts it at 5, not the 23 in my brief").
# Reading the oldest figure systematically under-rates exactly the parks that
# have had the most work done on them.
ALIGNED = re.compile(r"(\d+)\s+ALIGNED\s+(?:DIFFERENCE|[Dd]ifference)")
ALIGNED_ANY = re.compile(r"(\d+)\s+aligned\s+difference", re.I)
# A LAYOUT blocker has to be matched NARROWLY. The first version of this
# matched a bare "rodata" or "layout" and fired on 7 of 16 parks, including one
# whose text says "no `rodata' block and no `jtbl_' symbol" -- i.e. it asserts
# the OPPOSITE -- and another that merely warns about "layout inflation" it
# avoided. A false LAYOUT flag is worse than none: it tells me to skip a park
# that a new twin genuinely unblocks.
# A LAYOUT MENTION MUST ASSERT A REQUIREMENT, not merely name a subsegment.
# Matching bare "subsegment" flagged func_s01_80024CFC, whose two mentions are
# NEGATIVE -- "ovl_dng_s01 has no .rodata subsegment at all" and "from the
# segment's `data' subsegment" -- so I told its agent to check for a layout
# blocker that was never there. It landed the body as plain C in one pass.
# The genuine layout park says "the `.rodata' subsegment HAS TO end at ..."
# and "the unit's `.rodata' MUST grow by 4 bytes": a constraint, not a fact.
LAYOUT = re.compile(
    r"(?:\.?rodata|subsegment)[^.\n]{0,40}(?:has to|must|needs|would have to)"
    r"|(?:has to|must|needs|would have to)[^.\n]{0,40}(?:\.?rodata|subsegment)"
    r"|layout decision", re.I)


def park_report():
    """Print each park with its CURRENT best twin and its recorded blocker."""
    import dngtwin
    pool = dngtwin.carved(18)
    d = os.path.join(PROJ, "docs", "dng-parks")
    rows = []
    for fn in sorted(os.listdir(d)):
        if not fn.endswith(".c"):
            continue
        name = fn[:-2]
        txt = open(os.path.join(d, fn), errors="replace").read()
        # Prefer the CAPS headline convention; fall back to any case, and say
        # so. Requiring caps alone made three parks that write their headline
        # in lowercase read "blocker not parsed" -- which is worse than an
        # uncertain figure, because it hides a 7-row body entirely.
        hits = [int(x) for x in ALIGNED.findall(txt)]
        loose = False
        if not hits:
            hits = [int(x) for x in ALIGNED_ANY.findall(txt)]
            loose = bool(hits)
        resid = hits[-1] if hits else None
        # show the narrowing when there is one -- it says who has worked on it
        prog = (" (was %s)" % " -> ".join(str(h) for h in hits[:-1])
                if len(hits) > 1 and len(set(hits)) > 1 else "")
        if loose:
            prog += " [no CAPS headline -- figure is a guess, read the file]"
        layout = bool(LAYOUT.search(txt))
        me = pool.get(name)
        best = ([r for r in dngtwin.rank(
                    me, {k: v for k, v in pool.items() if k != name},
                    False, 0.60, 1) if not r[5]] if me else [])
        rows.append((best[0][0] if best else 0.0, name,
                     len(me[1]) if me else 0,
                     best[0][2] if best else None,
                     best[0][1] if best else None, resid, layout, prog))
    rows.sort(reverse=True)
    print("%d park(s), %d words" % (len(rows), sum(r[2] for r in rows)))
    print("a twin only helps a park blocked on C-LEVEL RESIDUE; a LAYOUT "
          "blocker is immune to it\n")
    # FAMILY MEMBERSHIP, because I wasted a whole agent-run for want of it.
    # func_s69_80024C38 was dispatched as a 4-row park; its 2-member family's
    # other member was on a DIFFERENT agent's list in the same batch, that
    # agent landed it, dngclone replicated the body in and verified it at 0,
    # and the park landed itself. dngpick already dedupes families within a
    # batch -- but over STUB rows, and parks are excluded from the picker, so
    # the two mechanisms never met. Printing it here is the cheap fix.
    import dngfam as _df
    # families() is {digest: [(unit, func, words)]} over STUB listings, which
    # is exactly the right scope: a sibling that is still a stub is one an
    # agent could be assigned, and a sibling already landed would have
    # replicated into this park already.
    fam = {}
    try:
        for grp in _df.families().values():
            if len(grp) > 1:
                names = [f for _u, f, _w in grp]
                for n in names:
                    fam[n] = names
    except Exception as e:
        print("  (family check unavailable: %s)" % e)
    for ratio, name, w, nm, vtag, resid, layout, prog in rows:
        twin = ("%.3f %s (%s)" % (ratio, nm, vtag)) if nm else "no twin at 0.60"
        why = ("LAYOUT -- a twin does NOT unblock this" if layout and resid is None
               else "%d aligned%s" % (resid, prog) if resid is not None
               else "blocker not parsed")
        if layout and resid is not None:
            why += " + mentions layout, read the file"
        mem = fam.get(name)
        famnote = ""
        if mem and len(mem) > 1:
            famnote = ("  [FAMILY of %d: %s -- DO NOT assign if any sibling is "
                       "picked; it lands by replication]"
                       % (len(mem), ", ".join(m for m in mem if m != name)))
        print("  %-22s %4dw  %-34s %s%s" % (name, w, twin, why, famnote))


def stub_rows(skip_parks=True):
    """[(copies, words, yield, func, unit tag, family key)] per remaining stub."""
    stubs = dngfam.stubs()
    if skip_parks:
        stubs = stubs - parked()
    out = []
    for key, g in dngfam.families(landed=False).items():
        mem = [(c, n, w) for c, n, w in g if n in stubs]
        if not mem:
            continue
        for _c, n, w in mem:
            out.append((len(mem), w, len(mem) * w, n, n.split("_")[1], key))
    return out


PROJ = dngfam.PROJ


def rodata_units():
    """Unit tags that have a `.rodata' subsegment in the splat config."""
    y = open(os.path.join(PROJ, "splat", "dungeon.bin.yaml")).read()
    return set(re.findall(r"\.rodata,\s*ovl_dng_(\w+)", y))


def owns_jtbl(tag, func):
    try:
        f = os.path.join(PROJ, "asm", "nonmatchings",
                         "ovl_dng_%s" % tag, "%s.s" % func)
        return "jtbl_" in open(f, errors="replace").read()
    except OSError:
        return False


def drop_homeless_jtbl(rows):
    """(kept rows, [(func, words)]) -- remove jump-table owners that cannot land.

    A function that owns a `.rodata' jump table in a segment with no
    `.rodata' subsegment VERIFIES AT 0 DIFFERING and still breaks the
    container: ovlverify measures the function's own words, and the table is
    appended past the code rather than landing at its address. That cost a
    `size differs by +0x120' once already, so no agent should ever be handed
    one of these.
    """
    have = rodata_units()
    kept, blocked = [], []
    for r in rows:
        if r[4] not in have and owns_jtbl(r[4], r[3]):
            blocked.append((r[3], r[1]))
        else:
            kept.append(r)
    return kept, blocked


def cheapness():
    """{func: best landed-carved ratio} from dngtwin's sweep cache, or {}.

    A SECOND MULTIPLIER THIS SCRIPT DOES NOT RANK ON, AND SHOULD NOT HIDE.
    `copies x words' prices replication. It says nothing about whether the
    body is already written: `dngtwin.py --sweep' finds 173 stubs and 35,938
    words within a few instructions of a landed carved sibling, and the units
    where that fraction is largest are the ones this ranking buries, because
    being cheap and being replicated are independent and the cheap ones tend
    to be one-member groups:

        b43  68% of its stub words at >= 0.90      b06 53%   b14 53%
        b41  48%   b36 46%   b15 42%   b31 37%     b38 35%   b21 34%

    Reported, not ranked on. Words per agent-run is still the objective and a
    cheap unit with no replication can be worth less than an expensive one
    with eleven copies -- but that is a judgement for whoever reads the pick,
    and it cannot be made against a number the tool does not print. If the
    cache is missing or stale this returns {} and the pick is unchanged;
    re-run `dngtwin.py --sweep' after each batch.
    """
    try:
        d = json.load(open(os.path.join(PROJ, "docs", "dng-twinsweep.json")))
    except (OSError, ValueError):
        return {}
    return {k: (v["best"][0][0] if v["best"] else 0.0) for k, v in d.items()}


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


def pack(rows, budget, limit, cheap=None, size_first=False):
    """Greedy to the word budget: CHEAPEST-FIRST, then most replicated.

    THIS USED TO SORT BY `(-copies, -words)' AND THAT WAS MEASURED WRONG IN
    BATCH 13. Ranking by replication is right for choosing a UNIT and says
    nothing about whether a body is already written in landed C, so this
    function packed each unit with its BIGGEST functions -- which meant that
    on the rare occasion the unit ranking did reach a cheap unit, the pick
    took that unit's expensive functions anyway.

    Batch 12 (size-first) against batch 13 (cheapness-first), same five-agent
    shape and the same word budget:

        batch 12   23 functions   19 landed, 3 parked    yields 1008-1200
        batch 13   24 functions   24 landed, 0 parked    yields  815-1027
                                  20 of them on the FIRST COMPILE

    The yields are the same to within noise and the landing rate is not, so
    there is no trade here to balance: a function whose body already exists
    at ratio 0.90+ is simply cheaper to land for the same words. The tiers
    are coarse on purpose -- inside a tier, replication is still the right
    tiebreak, because a clone is free.

    `cheap` is {func: best landed-sibling ratio} from dngtwin's sweep; with
    none, or with `size_first`, this falls back to the old ordering. See
    docs/matching-levers.md on what the ratio does NOT tell you: it predicts
    how much is shared, not that what differs is cheap.
    """
    def tier(r):
        if size_first or not cheap:
            return 0
        c = cheap.get(r[3], 0.0)
        for i, lo in enumerate((0.98, 0.95, 0.90, 0.80, 0.70)):
            if c >= lo:
                return i
        return 9

    pick, w = [], 0
    for r in sorted(rows, key=lambda r: (tier(r), -r[0], -r[1])):
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
    ap.add_argument("--skip", default="",
                    help="comma-separated unit tags to leave alone")
    ap.add_argument("--size-first", action="store_true",
                    help="the pre-batch-13 ordering: pack each unit by "
                         "descending words and ignore the twin sweep. "
                         "Measured worse (see pack()); kept for comparison")
    a = ap.parse_args()

    busy = {t for t in (a.busy.split(",") if a.busy else []) if t}
    skip = {t for t in (a.skip.split(",") if a.skip else []) if t} | busy

    rows = stub_rows()
    # Drop the functions whose jump table has nowhere to land. This is the one
    # failure the per-function verifier cannot see, so it must be filtered
    # here rather than trusted to an agent.
    rows, blocked = drop_homeless_jtbl(rows)
    # A --busy TAG THAT MATCHES NO UNIT USED TO BE A SILENT NO-OP, and the
    # footer printed "held by a live agent and skipped: <tag>" anyway -- so an
    # invocation with the basename (`ovl_dng_b02') instead of the tag (`b02')
    # reported protecting a live agent's unit while picking it as the batch's
    # top row. That is the clone-collision failure for the fourth time, now
    # wearing a confirmation message. Normalise both spellings and refuse an
    # unknown one: a protective flag must never be satisfiable by a typo.
    known = {r[4] for r in rows}
    def norm(tags):
        out = set()
        for t in tags:
            c = t[len("ovl_dng_"):] if t.startswith("ovl_dng_") else t
            if c not in known:
                raise SystemExit(
                    "dngpick: `%s' matches no unit with stubs. Units are named "
                    "by TAG (`b02'), not by basename. Known tags with stubs: %s"
                    % (t, ", ".join(sorted(known))))
            out.add(c)
        return out
    busy = norm(busy)
    skip = norm(skip - {"lo16"}) | ({"lo16"} if "lo16" in skip else set())
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

    cheap = cheapness()
    scored = []
    for tag, rows_u in by_unit.items():
        pick, w = pack(rows_u, a.budget, a.limit, cheap, a.size_first)
        if pick:
            scored.append((batch_yield(pick), w, tag, pick))
    # RANK UNITS BY THEIR PICK'S *CHEAP* YIELD FIRST, total yield second.
    # Packing each unit cheapest-first is not enough on its own: the unit
    # RANKING was still pure yield, so the first run of this change picked a
    # unit with 0 of 840 words at >= 0.90 (its cheap work had landed the batch
    # before) while b43 sat unpicked at 74%. Expected words landed per
    # agent-run is yield x P(land), and batch 13 measured P(land) at 24/24 for
    # cheap work against batch 12's 19/23 for expensive -- so a unit's cheap
    # words are worth more than its total. Units with no cheap rows score 0
    # here and are still ordered among themselves by total yield, which is
    # what fills a batch once the cheap work runs out.
    def cheap_yield(pick):
        seen, tot = set(), 0
        for r in pick:
            if r[5] in seen or cheap.get(r[3], 0.0) < 0.90:
                continue
            seen.add(r[5])
            tot += r[0] * r[1]
        return tot

    if a.size_first or not cheap:
        scored.sort(reverse=True)
    else:
        scored.sort(key=lambda t: (cheap_yield(t[3]), t[0]), reverse=True)

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
        # THE REST OF THE UNIT. This ranking scores a unit by TWIN YIELD, so a
        # large body with no twin at any ratio contributes nothing and never
        # reaches `pick' -- while being, often, the biggest thing in the unit.
        # Three batches running I discovered such a body only after dispatch:
        # an 834-word driver that then parked word-exact at 5 rows, and two
        # more at 873 and 531 words found while composing the next batch.
        # The unit choice is sound; the function list was the incomplete part.
        # So print everything else that is still a stub here, because the
        # agent should be given the whole unit and decide, not handed the
        # ratio's view of it.
        picked = {r[3] for r in pick}
        rest = sorted(((r[1], r[3]) for r in rows
                       if r[4] == tag and r[3] not in picked), reverse=True)
        # NB: do not bind `w' in the loop below -- it is the unit's picked
        # word count and the next print() uses it. The first version of this
        # block did, and reported "74 of 176" for a 701-word pick.
        if rest:
            print("        REST OF THE UNIT, still stubs (%d, %d words) -- "
                  "offer these too:" % (len(rest), sum(w for w, _ in rest)))
            for rw, rf in rest:
                c = cheap.get(rf, 0.0)
                print("            %-24s %5dw  %s"
                      % (rf, rw, ("%.3f" % c) if c else "no twin"))
        if cheap:
            near = [(r[3], cheap.get(r[3], 0.0)) for r in pick]
            nw = sum(r[1] for r in pick if cheap.get(r[3], 0.0) >= 0.90)
            hits = ", ".join("%s %.2f" % (f.split("_", 2)[2], c)
                             for f, c in sorted(near, key=lambda x: -x[1])
                             if c >= 0.80)
            print("        %d of %d picked words have a >= 0.90 landed "
                  "sibling%s" % (nw, w, (" -- " + hits) if hits else ""))
    print("\n%d unit(s), %d words of yield" % (len(chosen), total))
    if cheap:
        # The cheapest units NOT picked. This ranking cannot see them, so it
        # prints them rather than leaving the reader to re-derive the sweep.
        per = collections.defaultdict(lambda: [0, 0])
        for r in rows:
            a = per[r[4]]
            a[0] += r[1]
            if cheap.get(r[3], 0.0) >= 0.90:
                a[1] += r[1]
        taken = {t for _, _, t, _ in chosen}
        rest = sorted(((v[1], v[0], t) for t, v in per.items()
                       if t not in taken and v[1]),
                      reverse=True)[:6]
        if rest:
            print("not picked, but their words are already written in landed "
                  "C (dngtwin --sweep):")
            for nw, w, t in rest:
                print("  %-5s %5d of %5d stub words at >= 0.90 (%.0f%%)"
                      % (t, nw, w, 100.0 * nw / w))
    else:
        print("no docs/dng-twinsweep.json -- run `dngtwin.py --sweep' to see "
              "which of these units is already written in landed C")
    if dropped:
        print("skipped, a family they hold is already claimed in this batch: "
              "%s" % ", ".join(dropped[:8]))
    if blocked:
        print("jump table with nowhere to land, NOT offered to any agent: %s"
              % ", ".join("%s (%d words)" % b for b in sorted(blocked)))
    if busy:
        print("held by a live agent and skipped: %s" % ", ".join(sorted(busy)))
        print("plus %d stub(s) elsewhere sharing a family with one of those "
              "units -- a landing there would clone into them" % held)


if __name__ == "__main__":
    main()
