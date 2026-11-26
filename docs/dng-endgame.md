# The DUNGEON.BIN endgame: the remaining work is PARKS, not fresh bodies

Written 2026-11-26 at 87.8% (6,563 of 6,825 functions, 262 stubs, 78 parks),
because the dispatch pattern that got here stops working shortly and it is
better to say so before the briefs start coming back empty.

## What is actually left

**Fresh, unparked, unheld work: about 28 functions in eight units.** Measured
with `provision_batch.py <unit> --dry-run`, which drops parks and fragments, so
these are the numbers that matter rather than the stub counts:

| unit | functions | words |
|---|---|---|
| b57 | 3 | 1,993 |
| s37 | 4 | 1,224 |
| s33 | 4 | 923 |
| s70 | 4 | 862 |
| b38 | 3 | 832 |
| b23 | 3 | 809 |
| b19 | 2 | 540 |
| s25 | 5 | 306 |

Plus whatever the five live agents do not land.

**Everything else -- the large majority of the 262 remaining stubs -- is parked
or fragmented**, and three units that look like big blocks are not:

* `lo20`: 40 stubs, but **33 are fragments** and provisioning yields nothing.
* `lo16`: 20 stubs, **19 parked** (and all six that could not even be
  measured are now measurable; two are word-exact).
* `b31f`: 11 stubs, **10 parked**, and the eleventh is the 849-word body whose
  jump table is 4-byte aligned and therefore cannot meet the gate at all.
* `b29`, `s56`, `b22`: 0 provisionable each. Their remaining stubs are all
  parks.

**So the stub count overstates the available work by roughly eight to one.**
Pick targets with the dry-run, never off `progress.py --by-unit`'s stub column
-- its `parked` column reads 0 even for units with nineteen park files, which
is how a whole afternoon's worth of bad picks happens.

## Why the brief has to change shape

A unit brief says "land these N stubs, here is your donor route". That is the
right shape while fresh bodies exist. For a park it is the wrong shape, because
a park already HAS a body, a measured residue and a priced table of rejected
spellings -- the work is not writing C, it is finding the one axis nobody has
probed.

**What has actually cracked parks today**, in order of how often:

1. **A lever that did not exist when the park was written.** The b07 park sat at
   one row with 26 spellings measured flat and a compiler-source argument that
   it was unreachable; it landed on two lines once someone read the *whole*
   predicate in `cse.c:838` and found the second disjunct. **So a park's
   "closed" axes are only closed against the levers of the day it was written.**
2. **A sibling landing in the same family.** The same park's residue was closed
   by an agent working the equivalent body in a different unit, who met the row
   fresh.
3. **A tool fix.** Six lo16 parks were unmeasurable for reasons that were in
   `parksplice`, not in the parks.

**None of the three is "think harder about the park".** A park brief should
therefore carry: the park's own priced table (so it is not re-run), the levers
added since its date, and the names of its family's landed siblings.

## The three bodies that can never meet the gate

Not parks -- held, and they should not be handed to anyone as work:

* `func_b31f_801730AC` (849w) -- its jump table is 4-byte aligned, so a C
  `switch` cannot place it.
* `func_b29_8017088C` (150w) -- splat labels a Shift-JIS rodata word as the
  function's first instruction, so the target word count is 151 and a
  byte-correct body can never measure 0.
* one b31 body with its table in `asm/data/` -- a compiled `switch` emits a
  second copy and the container shifts.

Everything else is reachable as far as anyone has shown.


## A tool change that made the park set WORSE, and why it is recorded here

Three of the six remaining unmeasurable parks fail on `conflicting types`
against a **project header** rather than against their unit --
`extern u8 D_800814A8[];` in a park against `include/structs.h:1242`'s
`extern u32 D_800814A8;`, with the unit perfectly innocent. `parksplice`'s
`drop_known` only ever consulted the unit, so none of its three drop rules
could see the clash.

**Widening `drop_known` to the project headers took the park set from 73
measured / 6 skipped to 55 / 24.** It was reverted immediately.

**Why it backfired, as far as one measurement shows:** the unit is a closed
list of what that unit declares, so "the unit already has this" is a safe
reason to drop a park's copy. `include/structs.h` and `csrc/ovl_dungeon_decls.h`
are the whole project's declarations, so a symbol appearing there does NOT mean
the park's declaration is redundant -- many parks deliberately re-declare a
shared symbol with the narrower or array type their body needs, and dropping
that leaves the body reaching for a type the header does not give it. Eighteen
parks depended on exactly that.

> **So the three header-clash parks need the OPPOSITE of a drop: the park's
> declaration has to stay and the HEADER's has to be the one not in scope**,
> which a splicer cannot arrange. They are per-park repairs -- reconcile the
> park's body to the header's type, or rename its local declaration -- not a
> tool fix.

Two incidental findings from the attempt, both worth keeping:

* `_PROJ` computed with two `dirname`s from `tools/decomp/<file>.py` lands on
  `<root>/tools`, not the root. Every header open then failed into an
  `except OSError: pass` and the change appeared to do nothing at all, which
  cost a round of debugging before the regression was even visible. **A silent
  fallback hid a wrong path, and then a wrong path hid a bad idea.**
* A sweep of 79 parks is the only way to see a change like this. Measuring the
  three parks the change targeted showed them still skipping, which read as
  "no effect" rather than "no effect here, catastrophe elsewhere".
  **Re-measure the WHOLE set after touching a shared splicer.**


## MEASURED: the "sibling landing" route is CLOSED for every current park

This note lists three things that have cracked parks, in order of how often:
a lever that did not exist when the park was written, a sibling landing in the
same family, and a tool fix. **The middle one is measured and it is empty.**

    park family sizes (dngfam.families)   {1: 82, 2: 1}
    parks with a LANDED family sibling    0 of 83

**Eighty-two of the eighty-three parks are ONE-MEMBER families.** That is not a
coincidence: a family with a landed member gets its body replicated by
`dngclone`, so it never stays parked. Parks are, almost by construction, the
functions with no copy anywhere.

> **So for the parks that remain, only two of the three routes are live: a NEW
> LEVER, or a TOOL FIX.** Do not write a park brief that tells an agent to look
> for its family's landed siblings -- `dngfam` has none to offer.

### And `dngfam` would not have found the one sibling that DID crack a park

`func_b07_80170E54`'s park was closed by `func_b08_80170E7C` -- the same
function in another unit, which that agent described as "the park's body with
the symbols relinked". **`dngfam` puts `func_b07_80170E54` in a family of
one.** The normalisation that makes two bodies one family did not match across
those two units, so no amount of consulting `dngfam` would have pointed from
one to the other.

**What did point there was `jtcheck`: both bodies carry the same 12-entry jump
table.** So for finding a park's real siblings, **a shared JUMP TABLE is a
better signal than `dngfam`'s instruction families**, and
`docs/dng-shared-tables.md` is the file to consult rather than `dngfam`.

### One honest correction about how this was measured

My first pass at this printed "83 of 83 parks are in a multi-member family",
which was a bad loop of mine -- it counted every park found in any family,
size-1 included, and labelled the total "multi-member". The figure that matters
(0 with a landed sibling) was right in both passes, but the sentence around it
was wrong until I spot-checked three parks against their own headers, which
state their family size. **A park header that records `dngfam` says
"one member" is a check on the tooling, not just a note to the reader.**

## I ASSERTED "NO FRESH CODE LEFT" WITHOUT RUNNING THE DRY-RUN, AND IT WAS FALSE

Recorded because this document already warns about exactly this confusion and I
made it anyway, in a BRIEF, three times running.

After two units completed in a row I told three consecutive agents *"There is no
fresh code left in DUNGEON.BIN -- every remaining stub is a PARK"* and shaped
their briefs around park work. Then I counted, and the counts disagree:

    INCLUDE_ASM stubs in csrc/        225
      ... that have a park file        84
      ... with NO park file          141

and `provision_batch --dry-run` on the units holding most of those 141:

    lo20    0 fn      0w   fragments:33     <- all fragments, correctly 0
    s24     5 fn    790w
    s45     5 fn    971w
    s49     5 fn    627w
    s54     5 fn    601w
    s66     5 fn    658w
            -----------
            25 fn  3,647w of FRESH, PROVISIONABLE WORK

> **The cost was three briefs pointed at the hardest work in the project while
> 3,647 words of ordinary stubs sat unclaimed.** Park work yields a closed axis;
> a fresh unit yields functions.

**The rule, and it is already written above:** `provision_batch --dry-run` is the
only authority on what is provisionable. A stub count is not, a park count is
not, and "the last two units completed" is not. **Run it before every claim
about what is left, including -- especially -- a claim inside a brief.**

The failure mode is specific: `lo20` alone holds 33 stubs that are all
fragments, so a bare stub count overstates the work by a third on its own, while
the park files undercount it because a fresh stub has no park. **Neither number
is the answer and the dry-run is.** Note also that a dry-run over units with
LIVE AGENTS prints `0 fn 0w` after refusing to overwrite their copies -- so
read the refusal lines, and dry-run the units nobody is in.
