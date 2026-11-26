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

## 12 OF 83 PARKS HAVE A CORRUPTED BASELINE, AND NOBODY HAD SWEPT FOR IT

A park's figures are its whole value -- the first instruction in every park brief
is *never re-run a priced cell*. So a park whose baseline no longer reproduces is
worse than no park: it sends the next agent to attack rows that are not the
body's fault.

An agent found one: `func_lo16_8001EF0C` was recorded at **264/4**, and one of
those four rows came from `parksplice` dropping the park's own **refined**
`UnkDungeonSlot` (with `s8 unk_02`, which that park's own fact is about) in
favour of the unit's coarser `u8 unk_02`. **Its real baseline is 264/3.**

I sweept all 83 parks. **Twelve are affected:**

    TYPEDEF the unit defines differently   2   func_lo16_8001EF0C, func_lo16_8001CFB8
    PROTOTYPE the unit declares differently 10  func_b27_80174BEC, func_b29_80172FC0,
                                               func_b31f_8016FCE4, func_b32_80174428,
                                               func_b55_80170F84, func_lo16_800165B8,
                                               func_lo20_80026EC0, func_lo20_80027A20,
                                               func_s27_80024E64, func_s72_800247C0

> **The prototype warning already existed and printed loudly.** It had simply
> never been run across the corpus, so ten parks were carrying it unnoticed --
> including `func_b31f_8016FCE4`, which an agent spent a session on today. **A
> warning nobody aggregates is a warning nobody sees.**

The typedef case was silent until now, and is the sharper hazard, because a
field's width changes promotion and sign-extension rather than just a call.
`parksplice` now reports a DIFFERING tag definition as loudly as a prototype,
and prints both spellings side by side; an IDENTICAL one stays a quiet drop,
because that genuinely is nothing.

**What to do with an affected park:** re-measure its baseline FIRST and correct
its header, before trusting any cell in its table. Dropping the park's
declaration is still the only option that compiles -- the two cannot both be
present -- so this is not a bug to fix away, it is a fact to surface.

**And the general rule:** every tool warning in this project needs a
corpus-wide sweep, not just a per-invocation print. This is the same failure as
the masked `ovlverify` count and the OK-without-write `dngland` -- a check that
passes wrongly, or warns where nobody is reading.

## THREE PARKS COULD NOT BE MEASURED AT ALL, AND NO INVENTORY SHOWED IT

Beyond the twelve parks with a corrupted baseline, three could not be spliced at
all -- so their recorded figures were simply unreachable. Each failed for a
different reason and all three were invisible, because a park's figures are
recorded when it compiles and nothing re-checks them afterwards.

**1. The unit DEFINES a function the park declares.** `drop_known` matched the
unit's declarations only, and a definition's head does not end in `;`, so when a
sibling landing turned a stub into a definition the park's prototype survived and
the splice died on `conflicting types`. Fixed by matching a definition too -- the
definition is the strongest statement of the truth, so the park's prototype goes,
loudly. **`func_b29_80172FC0` went from unmeasurable to 492 words / 7 rows.**

**2. A MULTI-BODY park's definition against the UNIT's prototype.** A park that
joins fragments carries several definitions, and the unit's preamble may declare
one of them differently -- `conflicting types` with nothing in the PARK to drop.
The reverse direction is now handled: the unit's prototype for a function the
park defines is removed **in the spliced copy only, never in `csrc/`**, together
with that function's `INCLUDE_ASM` (or the assembler reports `symbol is already
defined` from `include/labels.inc`, which names the symbol but not the fact that
two files are providing it). **`func_lo20_80026EC0` now measures** -- at 17 words,
which is its HEAD FRAGMENT, so that park needs `pairverify`, not `ovlverify`.

**3. A genuinely broken DECLARATIONS block.** `func_lo20_80027A20` still fails on
undeclared identifiers. That is the park's own content, not a tool fault, and it
needs an agent to repair the block before any figure in it means anything.

> **Two of the three were tool faults and one is park content.** The pattern
> worth keeping: when a park will not splice, read WHICH error -- `conflicting
> types` is a declaration reconciliation the tool should do, `symbol is already
> defined` is a stub the tool should remove, and an undeclared identifier is the
> park's own block.

Regression-checked after all three changes: `func_s25_800244A0` 117/2,
`func_s29_80024088` 81/3, `func_b31f_8016FCE4` 332/3 -- all unchanged.

## A BACKLOG ITEM, MEASURED AND DEFERRED: 290 `incompatible pointer type` WARNINGS

`declcheck` reports **290** of them across the tree, concentrated in a few
units:

    ovl_dng_s21  29    ovl_dng_s41  13
    ovl_dng_s20  29    ovl_dng_s37  13
    ovl_dng_b23  27    ovl_dng_b42  12
    ovl_dng_s18  18    ovl_dng_b31  12

Almost all are the same class: a body declares `RotTransPers`,
`SetDrawArea` or `LoadImage`'s out-parameters as `s32 *` and casts to match,
where `psyq/include/libgte.h` declares them `long *`.

> **An agent measured the fix as FREE on one body: spelled `long`/`(long *)`
> it is 122 words / 0 differing, byte-identical output, and `declcheck` goes
> clean.** So this is a real cleanup with a known-zero cost, not a risk.

**Why it is not being done piecemeal:** it touches committed units, and every
one needs re-verification plus a container gate afterwards. Doing it while
agents hold those units is how a good landing gets reverted by mistake -- which
happened once today for exactly this reason. **It wants one dedicated batch
with no other work in flight**, and it is worth doing because a warning class
this large is noise that hides the one warning that matters.

**And the inheritance path is worth knowing:** these spread by DONOR. An agent
copying a landed body's spelling copies its warnings verbatim; one reported
inheriting six that way and fixing them in its own copy for free. So the count
grows with replication unless a donor is cleaned.

## THE REMAINING FRESH WORK, AS OF 2026-11-26 EVENING

**15 units, 24 functions, roughly 6,900 words**, from a `provision_batch
--dry-run` over every unclaimed unit (not from a stub count -- see the entry
above about the day I got that wrong):

    s51  1 fn  746w     s11  2 fn  605w     b18  2 fn  386w
    b17  2 fn  722w     b24  1 fn  530w     s13  2 fn  386w
    s04  2 fn  676w     s35  2 fn  474w     b04  1 fn  377w
    s60  2 fn  623w     s20  2 fn  457w     s69  2 fn  159w
                        b37  1 fn  451w     b09  1 fn  150w
                                            b40  1 fn  105w

Plus whatever the five units in flight hand back. **Re-run the dry-run rather
than trusting this table** -- it is a snapshot and the file says so.

> **The small units at the bottom are not filler.** `b09` at 150 words and
> `b40` at 105 are one body each, and a one-body unit with a landed sibling has
> been the cheapest landing in this sweep all day.

## AND THE 290-WARNING CLEANUP IS STILL DEFERRED, DELIBERATELY

Three independent bodies have now measured the fix FREE (122/0, 153/0, and one
agent spelling `RotTransPers`'s out-parameters `(long *)` with `declcheck` going
3 warnings to 0 at an unchanged 153/0). So the cost is known to be zero.

I looked at starting it while five agents held unrelated units, and stopped:
the top eight units are ~20 careful cast edits each, every one needs its body
re-verified and the whole thing needs a container gate, and **a half-done pass
is worse than none** because the remaining warnings still hide the one that
matters. The shape of the fix is now recorded, so the batch is cheap when it
gets its own slot:

    arg 2 of RotTransSV wants `SVECTOR *', arg 3 wants `long *'
    (the sites pass scratchpad addresses cast `(s32 *)`)
    RotTransPers / SetDrawArea / LoadImage out-parameters want `long *'
    against libgte.h, where the bodies declare `s32 *' and cast to match

**Do it as one pass with nothing else in flight**, unit by unit, re-verifying
each unit's bodies and gating once at the end.

## dngclone CARRIES THE DONOR'S SYMBOL NAMES INTO THE REPLICA'S PROSE -- SIX INSTANCES

A replicated body's comment can name the DONOR unit's functions as though they
were local. `csrc/ovl_dng_s09.c` said the body was "installed as the +0x10 step
by **func_s12_80024020's** spawn loops and by its case 4" -- and nothing in
`ovl_dng_s12` installs it. `func_s09_80024020` does, at lines 117 and 242 of the
same file.

**An agent read that sentence as naming the real caller.** It happened to point
at the right DONOR, so the residue helped rather than hurt that time, which is
exactly why it survived this long -- a wrong comment that gives the right answer
by accident does not get reported.

A sweep for a comment in one unit naming another unit's function as an
*installer* finds **six**:

    ovl_dng_b08   func_b09_80172BAC's case ...
    ovl_dng_b19   func_b15_80170D28 installs ...
    ovl_dng_b55   func_b01_80170A44 installs ...
    ovl_dng_s08   func_s09_80024020's case ...
    ovl_dng_s09   func_s12_80024020's spawn loops ...   <- FIXED 2026-11-26
    ovl_dng_s68   func_s12_80024020 case ...

**ALL FIVE CHECKED OUT AS LEGITIMATE. The sweep found ONE defect, not six, and
the caution above was the thing that mattered.** Verified one at a time:

    b08   "THE TWIN IS THE LANDED func_b09_80172BAC (csrc/ovl_dng_b09.c)"
          -- correctly names a twin, with its file. Not an installer claim.
    s08   "func_s09_80024020 (itself the relink of func_s12_80024020)"
          -- correctly names the donor CHAIN, explicitly.
    s68   "func_s12_80024020 (the same skeleton, the same dress block ...)"
          -- correctly names the donor.
    b19   my pattern matched inside a function BODY, not a comment sentence.
    b55   same -- a definition, not an installer claim.

So two of the six hits were my regex matching code rather than prose, and three
were comments doing exactly the right thing: naming the donor and saying it is
the donor. **Only `ovl_dng_s09` actually claimed a foreign function as its
installer**, and the difference is one word -- those three say "the twin is" or
"the relink of", and s09 said "installed by".

> **The lesson is about my sweep, not about the comments.** A pattern for
> `func_<other>_<addr>'s case` or `... installs` cannot tell a donor
> ATTRIBUTION from an installer CLAIM, because both name a foreign function in
> the same grammatical position. I reported six and hedged that some might be
> legitimate; the hedge was load-bearing, and mass-editing would have replaced
> five true sentences with false ones.

**If you sweep for this again, match on the CLAIM and not the name:** `installed
.{0,40}by func_<other>_` is the shape that is wrong, and `the twin is`, `the
relink of`, `the donor` are the shapes that are right.

## `parkcheck --splice` DOES NOT HAVE THE GAP THAT WAS REPORTED

An agent reported that `--splice` "is not checking against the committed unit's
own declarations", because its park passed the check while its `DECLARATIONS`
block still named a typedef the unit already has -- and its own HAND splice into
a pristine `csrc/` failed with `redefinition of struct UnkDngS58Name12`.

**Checked: the tool is right and the hand splice is the wrong comparison.**
`parksplice` reconciles a park's declarations against the unit before compiling
-- that is `drop_known`, which is the whole point of it, and on this park it
reports `dropped 9 declaration(s) the unit already has`. The splice then
measures **746 words / 94 aligned differences**, which is exactly the figure the
park records. A hand splice does not run that reconciliation, so its failure
says nothing about the check.

> **Worth stating because the instinct was right and the conclusion was not.**
> Testing a tool against a manual re-implementation of it only works if the
> manual version does what the tool does. Here the step it skipped IS the
> feature. If I had taken the report at face value I would have logged a bug in
> the one check that found five broken parks today, and someone would have
> "fixed" working code.

**What a park must still satisfy** is that it is self-contained *given* that
reconciliation: everything the body needs, with duplicates the unit also
declares being harmlessly dropped. That is what `--splice` verifies and it is
the right contract.

## FOUR PARKS CARRY FOUR DIFFERENT SIGNATURES FOR `func_800A4688` -- RESOLVE IT BEFORE LANDING ANY OF THEM

The committed definition at `csrc/ovl_dungeon_8009F644.c:4167` is

    s32 func_800A4688(u16 px, u16 py, u16 face, u16 ang, s32 flag)

and it is declared **nowhere in `include/`**. Its own comment says the width is
unwitnessed because nothing called it. Four parks now call it, each through a
unit-local `__asm__` alias so the definition stays untouched -- and the four
aliases **disagree**:

    (u16, u16, s16, s16, u32)
    (u16, u16, s16, s16, s32)
    (s32, s32, s16, s16, s32)
    (s32, s32, s16, s32, s32)

**They agree on exactly one thing: argument 3 is a SIGNED halfword**, and three
of the four give the same evidence for it -- the call site sign-extends with
`sll`/`sra` where a `u16` parameter would emit `andi ..., 0xFFFF`. Two also cite
a bare `lh` for argument 4 against a `u16`'s `lhu`.

> **Nobody is wrong yet, because none of these has landed and an alias only has
> to be right at its own call site.** A parameter whose argument is already in
> the right form at one site cannot be distinguished there, so `s32` and `u16`
> are often both consistent with one listing. The disagreement is evidence that
> **three of the four are under-determined, not that any is false.**

**What to do when one of those parks lands:** take the union of the four call
sites, and for each parameter keep the NARROWEST width that every site's
instruction pair permits. Argument 3 is settled. Arguments 1, 2, 4 and 5 need
the sites compared against each other, which no single agent could do because
each saw one. **And at that point the committed definition should probably be
corrected rather than aliased four more times** -- it has no other caller in the
tree, so changing it is cheap, but it is a landed body and the change needs its
own verification and container gate.

## THE THREE MECHANISMS NOBODY HAS ESTABLISHED, AND THE DUMP THAT WOULD SETTLE EACH

Today's work added 195 lines to `docs/matching-levers.md` that flag a
contradiction, a narrowing, a retraction, a precondition or an inference -- and
agents were disciplined about saying which attributions they had NOT proved.
Three of those are still open, and they are buried one per entry, so they are
collected here.

Each has a **measured figure** that is not in doubt and a **named pass** that is
not established. All three need a single-function `-da` probe, which every agent
judged not worth the detour once its body was at 0 or its park was priced. That
judgment was right each time; the questions are collected so that someone
holding a dump for another reason can answer one cheaply.

**1. Why a pointer-base MERGE costs 49 rows** (`func_b37_801732A4`, landed).
Two separate pointer bases are 451/0; one shared base is 451/49, every row a
`$s2`/`$s3` rotation. The inference is that merging gives the one allocno the
references of both arms and lifts it past `c` in `allocno_compare`'s ranking,
while two locals with disjoint ranges both fit in `$s3`. **The reference counts
and the ranking are not established** -- `.greg` on a single-function probe
would print them.

**2. Why a shared `goto` tail collapses ELEVEN words** (`func_b37_801732A4`).
Written out in six reaching arms it is 451/0; a shared `goto bump` is **440/28**,
eleven words SHORT. Both forms emit one `j` per arm and `jump2` merges the
written-out copies into the single block the target has, so on the obvious
accounting they should be equal length. **What collapses the eleven is not
established**; the arms differ in the written-out form because each holds a
symbol address in its own local, which is the likeliest place to look and is a
guess. `.jump2` would settle it.

**3. Why the target gives a non-conflicting value its own hard register**
(`func_s11_80024004`, parked at 481/502). The park's whole cause -- that the
target spills a parameter because nine callee-saved registers are live and the
candidate needs only eight -- rests on this, and the park says so, marking its
cause an INFERENCE rather than a measurement. **It is a `find_reg` scan-order
question in `global.c` and it is open.**

> **None of the three changes a figure or a landing.** All three are "why",
> where the "what" is measured. They are listed because an unestablished
> mechanism that is LABELLED unestablished is an asset -- the next person knows
> exactly what to spend a dump on -- while the same mechanism stated as fact is
> the thing that produced two confident wrong answers today.
