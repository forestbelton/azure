#!/usr/bin/env python3
"""Rank a carved stub's twins among the LANDED CARVED bodies -- the pool
every twin search in this sweep has been blind to.

## WHY THIS EXISTS: `xsibs.py` CANNOT SEE 115 OF THE 116 CARVED OBJECTS

`xsibs.py` builds its landed side out of the REFERENCE IMAGES: `nm -n` over
each container's ELF for the symbol list, `objdump -h` for the section map,
and the image bytes for the instructions. That is the right design for
resident code and it is structurally unable to work here. The 116 carved
objects are OVERLAYS SHARING ONE RAM WINDOW: 115 of them live at 0x8017xxxx
or 0x8002xxxx, all at the same addresses, all in the one dungeon.bin ELF. So
`nm -n` interleaves 115 objects' symbols at colliding addresses, the
`vma <= a < vma + size` section lookup takes whichever section comes first,
and the "end of a function is the next symbol" rule measures from one
object's symbol to a different object's. The entries either fall out of the
word-count window or carry another object's bytes.

Measured, not inferred. `xsibs.landed_streams(18, ...)` returns 3,366
functions:

    town.bin       1283
    dungeon.bin     948      <- of which 189 are carved, ALL `func_lo16_*'
    slus_006.14     823
    main.bin        312

`lo16` is the one carved segment that is NOT in the shared slot -- it sits at
0x8001xxxx -- which is exactly why it is the only one that survives. Every
landed body in the 115 banked objects, around 2,500 functions by batch 12, is
absent from every `xsibs` run any agent has made. The brief has been telling
agents "a landed `ovl_dng_*` sibling can be a better twin than the resident
original" and then handing them a tool that cannot find one.

## WHAT THIS COMPARES INSTEAD

Listing against listing. `asm/nonmatchings/ovl_dng_*/<func>.s` exists for a
LANDED function too -- splat regenerates it from the ROM and does not prune
it, which `dngpick`'s docstring records as a hazard for counting work. Here
it is the asset: a landed function's listing is the original target assembly,
so a stub's listing and a landed sibling's listing are the same kind of
evidence, and a high ratio means the C already written in `csrc/` is nearly
your answer.

The signature is the OPCODE SEQUENCE: mnemonics only, registers and operands
dropped. That is deliberately looser than `dngfam`'s family key, which keeps
registers and is the right key for "these are the same function, clone it".
`dngfam`'s docstring records that matching on opcodes alone reported 1052
false copies -- different source compiling to the same control flow. Those
false copies are what this tool is for. It does not claim a clone; it claims
a body worth reading. A lead that IS in the same `dngfam` family is marked
`[CLONE]`, because then the answer is not to read it but to run `dngclone`.

## WHAT THIS TOOL CANNOT SEE: A BLEND. USE THE CALLEE-SET GREP FOR THOSE

A carved body is sometimes a BLEND -- the head of one landed or resident
function, the tail of another, and a region with no twin at all. Its
whole-body ratio against each source is LOW, because each source explains
only a fraction, so it does not appear in the ranked list at any threshold
and the function reads as original work.

`func_b40_80173C20` (218 words, landed at 0 differing) is the case: its
message head is `func_b21_80175E14` (22 words, ratio 0.183) and its
four-state machine and tail are `func_b35_801728C4` (121 words, ratio 0.555).
Neither is reachable from a ratio.

**A LONGEST-RUN OR COVERAGE METRIC WAS TRIED AND DOES NOT DISCRIMINATE.**
Measured, so that nobody builds it twice:

* Longest common contiguous run: the real blend's best run is 20 opcodes
  against `func_b35_801728C4` and 11 against `func_b21_80175E14`. The
  matching blocks are short and scattered -- 20, 13, 13, 8, 7, 5, 7 -- so no
  run threshold separates a blend source from a coincidence.
* Coverage of the CANDIDATE by the target, over blocks of 5+ opcodes:
  `func_b21_80175E14` scores 0.73 and is found -- along with **62 other
  candidates** at 0.55 or better, several scoring HIGHER than either true
  source. The spans are all nearly the full body. It finds bodies built from
  the same idioms, not the blend.
* The same coverage with REGISTERS KEPT (`dngfam.sig_insn` as the token):
  **zero** candidates at 0.45. A blend's sources were allocated inside their
  own functions, so they share opcodes and not registers.

Registers are too strict and bare opcodes too loose, with no threshold in
between. So there is no `--blend` mode: the method that works is semantic,
not statistical. Account for your body's WHOLE CALLEE SET, and when the
matched twin leaves a callee unexplained, grep on that callee alone:

    grep -ln RotTransPers4 csrc/ovl_dungeon*.c csrc/ovl_dng_*.c

That is how both known blends were found. See "WHEN A CARVED BODY'S CALLEE
SET SPANS TWO RESIDENT FUNCTIONS, SUSPECT A BLEND" in
`docs/matching-levers.md`.

## WHEN YOUR BODY IS SHORTER THAN ITS TWIN, REFERENCES WERE REMOVED

The high-ratio pick has one failure mode and the word-count delta predicts
it, which is why it is printed. `func_b42_80173730` is 178 words at ratio
0.947 against a 198-word twin, and the 20 words it is MISSING are what cost
its agent the batch's only real fight: the twin's absent arm carried THREE
references on one pointer, so flow counts that pointer at 7 references where
the twin had 10, `global.c`'s `allocno_compare` ranks it below its neighbour,
and the two swap registers -- 19 aligned differences, every row that one
exchange.

**BUT READ THE TWIN FOR MISSING STATEMENTS BEFORE REACHING FOR THE REFERENCE
LEVER.** Batch 14 measured five more shorter-than-twin bodies and in four the
deletion was a WHOLE BLOCK whose values died with it, so nothing was lost and
no allocation work was needed: a dropped dress arm (-6 words, first compile),
a dropped record lookup and its two locals (-14, two compiles), a dropped
countdown (-17, first compile), a dropped gate arm (-6, first compile). Only
`func_b42_80173730` lost references the SURVIVING code still used, and only
that one needed the duplicate.

So the flag is a prompt, not a diagnosis. Ask first which statements the twin
has that your listing does not; the reference lever applies only when the
removed block held references to a value the rest of the body still uses.
`func_b43_801729D4`'s agent went to `allocno_compare` on the strength of this
warning and found the real answer was two absent statements -- reading for
those first is what made it a two-compile landing.

When the reference lever IS the answer: find a block the twin reaches by a
`goto` or by falling into a shared label and WRITE IT OUT TWICE. jump2
cross-jumps the copies back into the single block the target has, so the
duplication costs no words and pays the references back.

**AND AIM FOR THE NEXT POWER OF TWO, NOT FOR "MORE".** `allocno_compare`'s
priority is `floor_log2(n_refs) * n_refs / live_length`, so references buy
nothing until one crosses a power-of-two boundary. On that body 7 references
over 84 insns scores `2*7/84 = 0.167` and loses to a neighbour's
`3*9/114 = 0.237`; the duplicate took it to 8 over 90, and 8 is the first
count whose `floor_log2` is 3, so `3*8/90 = 0.267` wins. SIX references would
have been worth no more than seven. When `allocno_compare` is the residue,
count to the next power of two and stop.

A POSITIVE delta is the ordinary case and means the relink ADDED something --
an arm, a state, a call. Additive deltas are cheap at any size: one body in
this batch gained a whole new state over its twin and still landed on the
first compile.

## HOW TO READ A RATIO

    1.000       same control flow, different registers or constants. The C is
                almost certainly the same shape; expect to change operands.
    0.90-0.99   a few instructions' delta -- an extra arm, one gate, a
                bank-local table in place of a resident one.
    0.75-0.90   same family of routine, real differences. Worth reading.
    < 0.75      a lead, not a twin. Read the call sequence instead.

A ratio is a reason to OPEN the landed C, never a reason to copy it. The
device rule applies: a twin's `do { } while (0)`, its spread stores and its
split statements are fixes for ITS allocation. Strip them, compile, measure,
and add back only what your own listing asks for.

Usage:
    python3 tools/decomp/dngtwin.py func_b09_80171728 [func...]
    python3 tools/decomp/dngtwin.py --unit ovl_dng_b09      # every stub
    python3 tools/decomp/dngtwin.py FUNC --top 8 --ratio 0.60 --stubs
    python3 tools/decomp/dngtwin.py --sweep       # score every stub, cache it

## THE SWEEP REPRICES THE REMAINING WORK, AND `dngpick` RANKS WITHOUT IT

`--sweep` scores all 1,205 remaining stubs of 18 words or more. First run:

    0.98+         55 stubs    10,343 words
    0.90-0.97    118 stubs    25,595 words
    0.80-0.89     95 stubs    15,847 words
    0.70-0.79     72 stubs     9,227 words
    none         865 stubs   153,550 words

So 173 stubs and 35,938 words are within a few instructions of a body already
written in C. `dngpick` ranks units by `copies x words`, which is the right
proxy for replication and says nothing about this second multiplier -- and
the units where it is largest are exactly the ones that ranking buries,
because they are low-replication:

    b43   68% of its stub words have a >= 0.90 landed sibling
    b06   53%        b14  53%        b41  48%        b36  46%
    b15   42%        b31  37%        b38  35%        b21  34%

`dngpick` now READS this cache and reports it per unit. It does not re-rank
on it: words-per-agent-run is still the objective and a cheap unit with no
replication can still be worth less than an expensive one with eleven copies.
The two numbers belong side by side in front of whoever picks the batch.

Re-run `--sweep` after a batch lands; the cache is a snapshot and every
landing adds to the pool it scores against.
"""
import argparse
import collections
import difflib
import glob
import hashlib
import json
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dngfam  # noqa: E402

PROJ = dngfam.PROJ
MNE = re.compile(r"^\s*([a-z][a-z0-9._]*)")


def opsig(insns):
    return [m.group(1) for m in (MNE.match(i) for i in insns) if m]


def famkey(insns):
    """dngfam's own family key, to mark a lead that is an exact clone."""
    return hashlib.md5("|".join(dngfam.sig_insn(x)
                                for x in insns).encode()).hexdigest()


OBJDUMP = os.environ.get("AZURE_OBJDUMP", "mips-linux-gnu-objdump")
OBJ_FN = re.compile(r"^[0-9a-f]+ <(func_[A-Za-z0-9_]+)>:")
OBJ_ROW = re.compile(r"^\s+[0-9a-f]+:\t(.*)")


def unalias(t):
    """Undo the two expansions `-M no-aliases' makes that a listing does not.

    MEASURED, because getting this wrong is silent: a splat listing prints
    `nop' and `b', while `-M no-aliases' prints `sll zero,zero,0x0' and
    `beq zero,zero,...'. On one unit that is 126 nops against 0, and 193 `beq'
    against 5 -- so almost every delay slot and unconditional branch
    mismatched and the 0.98+ band came out EMPTY. `-M no-aliases' is still
    required for everything else (`move'/`li' against `addu'/`addiu'); these
    two just have to come back.
    """
    t = re.sub(r"^sll\s+zero,zero,0x0$", "nop", t)
    return re.sub(r"^beq\s+zero,zero,", "b ", t)


def landed_objs(minw):
    """{func: (tag, [instruction text])} for LANDED bodies, from the objects.

    WHY THIS EXISTS, and it is not an optimisation. `dngtwin' ranks a stub
    against LANDED CARVED bodies, and it used to read their SPLAT LISTINGS --
    which only survive by accident. The Makefile keeps a listing only while an
    `INCLUDE_ASM' names it, so a landed function's listing is deleted; the ones
    this tool was reading were stale leftovers from before those functions
    landed. A full `make splat' regenerates from scratch and prunes them, and
    on 2026-11-24 that emptied the landed pool completely: the sweep reported
    1,050 stubs and ZERO siblings in every ratio band, which looks like the
    block running out of cheap work and is really the corpus being gone.

    The objects are the sound source. `build/csrc/UNIT.o' exists for every
    landed body and the four containers are byte-identical, so that code IS
    the target's. `-M no-aliases' is required: without it objdump prints
    `move'/`li' where a listing has `addu'/`addiu' and no mnemonic matches.
    """
    out = {}
    for o in sorted(glob.glob(os.path.join(PROJ, "build", "csrc",
                                           "ovl_dng_*.o"))):
        tag = os.path.basename(o)[len("ovl_dng_"):-2]
        try:
            txt = subprocess.run(
                [OBJDUMP, "-d", "--no-show-raw-insn", "-M", "no-aliases", o],
                capture_output=True, text=True, check=True).stdout
        except (OSError, subprocess.CalledProcessError):
            continue
        cur, buf = None, []
        for ln in txt.split("\n"):
            m = OBJ_FN.match(ln)
            if m:
                if cur and len(buf) >= minw:
                    out[cur] = (tag, buf)
                cur, buf = m.group(1), []
                continue
            r = OBJ_ROW.match(ln)
            if r and cur is not None:
                buf.append(unalias(r.group(1).strip()))
        if cur and len(buf) >= minw:
            out[cur] = (tag, buf)
    return out


def carved(minw):
    """{func: (unit tag, [opcode], famkey, is_stub)} over stubs AND landed bodies.

    Stubs come from their splat listings; landed bodies from the built objects
    (see `landed_objs'). `famkey' is only comparable WITHIN one source, since
    the two spell operands differently (`$v0' against `v0', `-0x18' against
    `-24'), so a cross-source clone flag is not claimed -- the RATIO is
    mnemonic-only and is unaffected.
    """
    stubs = dngfam.stubs()
    out = {}
    for d in sorted(glob.glob(os.path.join(PROJ, "asm", "nonmatchings",
                                           "ovl_dng_*"))):
        tag = os.path.basename(d)[len("ovl_dng_"):]
        for f in sorted(glob.glob(os.path.join(d, "*.s"))):
            name = os.path.basename(f)[:-2]
            b = dngfam.body(f, name)
            if len(b) < minw:
                continue
            out[name] = (tag, opsig(b), famkey(b), name in stubs)
    for name, (tag, b) in landed_objs(minw).items():
        if name in out:
            continue
        out[name] = (tag, opsig(b), "obj:" + famkey(b), False)
    return out


def rank(target, pool, want_stubs, ratio, top):
    tag, sig, fk, _ = target
    n = len(sig)
    rows = []
    for name, (otag, osig, ofk, is_stub) in pool.items():
        if is_stub and not want_stubs:
            continue
        if not (0.75 * n <= len(osig) <= 1.33 * n):
            continue
        sm = difflib.SequenceMatcher(None, sig, osig, autojunk=False)
        if sm.quick_ratio() < ratio:
            continue
        r = sm.ratio()
        if r < ratio:
            continue
        rows.append((r, otag, name, len(osig), ofk == fk, is_stub))
    rows.sort(reverse=True)
    return rows[:top]


CACHE = os.path.join(PROJ, "docs", "dng-twinsweep.json")


def sweep(pool, floor):
    """{func: {tag, words, best:[(ratio, name, tag, same_family)]}} for EVERY stub.

    The multiset overlap of two mnemonic sequences over the longer length is
    an UPPER BOUND on difflib's ratio, so rejecting on it is sound: a
    candidate it drops cannot have passed the real threshold. Without that
    prefilter this is 1,205 stubs x 830 landed bodies of `SequenceMatcher`
    and takes about an hour; with it, under three minutes.
    """
    import bisect
    landed = [(n, t, sg, fk) for n, (t, sg, fk, st) in pool.items() if not st]
    lms = [(n, t, collections.Counter(sg), sg, fk) for n, t, sg, fk in landed]
    order = sorted(range(len(lms)), key=lambda i: len(lms[i][3]))
    lens = [len(lms[i][3]) for i in order]
    out = {}
    for n, (t, sg, fk, st) in sorted(pool.items()):
        if not st:
            continue
        L, ms = len(sg), collections.Counter(sg)
        lo = bisect.bisect_left(lens, int(0.75 * L))
        hi = bisect.bisect_right(lens, int(1.34 * L) + 1)
        best = []
        for i in order[lo:hi]:
            ln, lt, lc, ls, lfk = lms[i]
            if sum((ms & lc).values()) / max(L, len(ls)) < floor:
                continue
            r = difflib.SequenceMatcher(None, sg, ls, autojunk=False).ratio()
            if r >= floor:
                best.append((round(r, 3), ln, lt, lfk == fk))
        best.sort(reverse=True)
        out[n] = {"tag": t, "words": L, "best": best[:3]}
    return out


def load_cache():
    """The sweep's cached result, or None. Callers must tolerate None."""
    try:
        return json.load(open(CACHE))
    except (OSError, ValueError):
        return None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("funcs", nargs="*")
    ap.add_argument("--unit", help="rank every remaining stub of this unit")
    ap.add_argument("--min-words", type=int, default=18)
    ap.add_argument("--ratio", type=float, default=0.70)
    ap.add_argument("--top", type=int, default=5)
    ap.add_argument("--stubs", action="store_true",
                    help="also report STILL-STUB siblings -- not an answer, "
                         "but it tells you two stubs are one job")
    ap.add_argument("--sweep", action="store_true",
                    help="score EVERY remaining stub and write %s; "
                         "dngpick reads it to report how cheap a unit's "
                         "words are" % os.path.relpath(CACHE, PROJ))
    a = ap.parse_args()

    pool = carved(a.min_words)
    if a.sweep:
        res = sweep(pool, a.ratio)
        json.dump(res, open(CACHE, "w"), indent=0, sort_keys=True)
        tiers = [(0.98, "0.98+"), (0.90, "0.90-0.97"),
                 (0.80, "0.80-0.89"), (a.ratio, "%.2f-0.79" % a.ratio)]
        n = collections.Counter()
        w = collections.Counter()
        for v in res.values():
            lab = "none"
            if v["best"]:
                lab = next(l for t, l in tiers if v["best"][0][0] >= t)
            n[lab] += 1
            w[lab] += v["words"]
        print("%d stubs >= %d words, %d words"
              % (sum(n.values()), a.min_words, sum(w.values())))
        for _, lab in tiers + [(0, "none")]:
            print("  %-10s %4d stubs  %7d words" % (lab, n[lab], w[lab]))
        print("wrote %s" % os.path.relpath(CACHE, PROJ))
        return
    want = list(a.funcs)
    if a.unit:
        tag = a.unit[len("ovl_dng_"):] if a.unit.startswith("ovl_dng_") else a.unit
        want += sorted(n for n, (t, _, _, s) in pool.items() if t == tag and s)
    if not want:
        raise SystemExit("name a function, or --unit ovl_dng_<tag>")

    print("pool: %d carved listings >= %d words, %d landed"
          % (len(pool), a.min_words,
             sum(1 for v in pool.values() if not v[3])), file=sys.stderr)
    for f in want:
        if f not in pool:
            print("%-26s not a carved listing >= %d words" % (f, a.min_words))
            continue
        me = pool[f]
        # A target compares against itself at 1.000 and against its own
        # family at 1.000 too; drop only itself.
        sub = {k: v for k, v in pool.items() if k != f}
        print("\n%s  %d words  (ovl_dng_%s)" % (f, len(me[1]), me[0]))
        rows = rank(me, sub, a.stubs, a.ratio, a.top)
        if not rows:
            print("    no carved sibling at ratio >= %.2f -- try xsibs for "
                  "a RESIDENT twin, then the callee-set grep (see WHAT THIS "
                  "TOOL CANNOT SEE)" % a.ratio)
        for r, otag, name, nw, same_fam, is_stub in rows:
            mark = " [CLONE -- run dngclone, do not read it]" if same_fam else ""
            mark += " [STILL A STUB -- assembly, not an answer]" if is_stub else ""
            # The delta is the TWIN's words minus yours, so a POSITIVE
            # delta means YOUR body is the shorter one -- the relink deleted
            # code and took its references with it. That is the direction
            # that costs rows; see the section above.
            d = nw - len(me[1])
            if d == 1:
                # MEASURED, batch 19: a ONE-word delta is usually a compare
                # constant, not a statement. `if (x < 0)' is a single `bgez'
                # where `if (x < 2)' is `slti'+`beqz'. func_b12_80172290
                # landed first compile once its gate was read as `< 0' where
                # the twin has `< 2' -- the twin's spelling measures 217/7, and
                # an agent sent hunting for a missing statement would not have
                # looked at the branch mnemonic.
                mark = (" [1 WORD SHORTER -- TWO candidates, both one word: "
                        "a COMPARE CONSTANT against zero (bgez/bltz/blez/"
                        "bgtz/beqz/bnez cost one word, a non-zero compare "
                        "two), or a SINGLE STORE whose value is already in a "
                        "register (an `sh $zero' is one word). Check the "
                        "branch mnemonics AND the store list]") + mark
            elif d > 1:
                mark = (" [YOURS IS %d WORDS SHORTER -- find the MISSING "
                        "STATEMENTS first, see WHEN YOUR BODY IS SHORTER]"
                        % d) + mark
            print("    %.3f  %-24s %4d words (%+d vs yours)  "
                  "csrc/ovl_dng_%s.c%s" % (r, name, nw, d, otag, mark))


if __name__ == "__main__":
    main()
