# Sweeping dungeon.bin's overlay

How to continue decompiling `csrc/ovl_dungeon.c` from a cold start. Written
for a session with no memory of the previous ones.

Numbers rot; the tools are the authority.

    python3 tools/decomp/progress.py | grep ovl_dungeon      # words landed
    python3 tools/decomp/guard.py                            # stubs accounted for
    python3 tools/decomp/worklist.py ovl_dungeon --summary    # what is left, in bulk
    python3 tools/decomp/nearsibs.py ovl_dungeon --groups     # the clusters
    python3 tools/decomp/families.py ovl_dungeon --min 2      # exact-shape families

`docs/town-sweep.md` is the OPERATING PROCEDURE and everything in it applies
here: the nine-step loop, the whole-unit verification recipe, the agent brief,
and about sixty traps that have each cost something. Read it rather than
re-deriving it. This file carries only what is specific to this block.

`docs/overlays.md` has the lever list and the `-G0` facts.
`docs/landing-a-function.md` is the contract for one function.
`docs/dungeon-map.md` is the CONTAINER map -- the other code in this file,
which is not this overlay.

## What this block is

| | file | vram |
|---|---|---|
| rodata | 0xa3000..0xa3d40 | 0x80088760..0x800894A0 |
| text | 0xa3d40..0xf7700 | 0x800894A0..0x800DCE60 |

863 functions, 85,616 words. It is the largest single block in the project --
1.6x `ovl_town`, 2.0x the whole resident executable -- and it loads into the
SAME RAM slot as town.bin's overlay, so the two are mutually exclusive at
runtime and an address in this range means different code depending on which
is resident.

It became a `c` subsegment on 2026-11-14 (splat/dungeon.bin.yaml, which also
grew the `src_path: csrc` the conversion needs). Five of its functions are
`jr $ra; nop` and splat emitted those as real C; the other 858 arrived as
`INCLUDE_ASM`.

## THIS IS NOT TOWN'S OVERLAY, and that was measured before anything was written

The first question to ask of a newly opened block is whether it has already
been written somewhere else. `tools/decomp/xsibs.py` answers it: it compares
every stub's opcode stream against every function already landed in ANY
container, reading the landed side out of the reference images through each
container's ELF so that it does not depend on a listing surviving.

    python3 tools/decomp/xsibs.py ovl_dungeon

**20 of 863 functions have a landed near-sibling at ratio >= 0.88, 582 words
-- 0.7% of the block.** The two overlays share a handful of library-shaped
routines and nothing else. So this is 85,000 words of reading, not a renaming
job, and the per-function economics of `docs/town-sweep.md` apply unchanged.

The 0.7% was still worth taking first, and two of the three groups it found
paid for the run:

* **`func_800BCE7C` is `csrc/ovl_town.c`'s `func_8008CE08` word for word with
  every field displacement 0xE8 higher** -- 27 of its 80 words differ and all
  27 by exactly `+0xE8`. That fixes `UnkDungeonCastSP` as town's
  `UnkTownCastSP` behind a 0xE8-longer prefix, and the function landed from
  town's body with the offsets substituted.
* **Seven node walks are the resident engine's `func_80046398`** (csrc/
  unk_19434.c) recompiled once per visitor, in three variants: the fourth
  argument is `s->unk_06`, or a constant 0, or there is a fifth argument on
  the stack. 148 words for one reading.

## What is established about this block's types

Nothing is in `include/structs.h` yet and nothing should be while it is a
guess: 0x800DCE60 and up is dungeon-private and no engine unit references it.
The declarations live at the top of `csrc/ovl_dungeon.c`, the same rule
`ovl_town.c` follows.

* **`UnkDungeonActor`** -- the state-machine record. `+0x8C` is the handler
  the dispatcher runs next, and `+0x9A`/`+0x9B` are a state byte and the byte
  after it. Seventeen installers in three shapes write `+0x8C`, and two more
  clear it in the same breath as `+0x9A`, which is the witness that the two
  ranges are fields of one record.
* **`UnkDungeonCastSP`** -- the cast scratchpad, town's `UnkTownCastSP` at
  `+0xE8`; see above.
* **`UnkDungeonRect`** -- the 4-halfword rectangle `func_800673A0` takes.
  Alignment 2 is the shape: an `s16[4]` cannot be moved with aligned `lw`, so
  the copy comes out as `lwl`/`lwr` + `swl`/`swr` pairs.

## The two hazards specific to this block

### include/structs.h ALREADY DECLARES five addresses inside this overlay

`0x800894A0`, `0x800A1BD0`, `0x800A48F0`, `0x800AC3EC` and `0x800AC4C4` are
declared in `include/structs.h`, because resident engine units call them --
and the engine reaches whichever overlay is loaded, so those declarations are
partly about THIS overlay's functions. Two consequences, and they pull in
opposite directions:

* The declaration is EVIDENCE. `func_800A1BD0` is declared
  `s32 func_800A1BD0(u8 *)`, which is what fixes its search key as a pointer
  rather than the `s32` the listing alone would allow -- and
  `csrc/unk_154BC.c` writes the same `+0xAC` field as a `u8 *`.
* The declaration may be WRONG for this overlay. `func_800A48F0` was declared
  `void`; its three callers here all test its result as a halfword, so it
  cannot be. It is now `s16` with the reasoning at the declaration, and all
  four containers stay byte-identical -- the engine's one call discards the
  result, which is why `void` had never been falsified.

A mismatch surfaces as a hard `conflicting types` error rather than as a byte
difference, so this hazard is loud. When it fires, read the engine's call site
before changing either side.

### SEVEN of the 28 jump tables are at 4 mod 8

`jtbl_80088D7C`, `800892CC`, `800892E4`, `8008938C`, `800893E4`, `80089454`
and `80089474`. While their functions are `INCLUDE_ASM` that costs nothing --
splat migrates each table into its function's listing and emits it verbatim --
but `cc1psx` writes `.align 3` before a table it compiles, so the first of
those seven to be landed needs a translation unit whose rodata STARTS at a
4-mod-8 address, where the directive is a no-op. That is the forced carve
`town.bin` took; `splat/town.bin.yaml` has the worked example and splat prints
the seven split offsets on every run.

So: **do not land one of those seven functions without carving the unit in the
same change**, and expect the symptom to be a container eight bytes long
rather than a wrong instruction.

**THE SEVEN FUNCTIONS, by name** (2026-11-15; derived once, so nobody has to
grep for them again), and all seven are still `INCLUDE_ASM`:

| table | function |
|---|---|
| `jtbl_80088D7C` | `func_8009F644` |
| `jtbl_800892CC` | `func_800B45E0` |
| `jtbl_800892E4` | `func_800B490C` |
| `jtbl_8008938C` | `func_800C2824` |
| `jtbl_800893E4` | `func_800C4F20` |
| `jtbl_80089454` | `func_800D50FC` |
| `jtbl_80089474` | `func_800D81D4` |

The agent brief template now screens batch lists against that set. On
2026-11-15 a batch was handed `func_800D50FC` by mistake; it compiled at
102/102 and 0 differing on the first try, and the agent recognised the
hazard and held it back rather than reporting a landing. Its body is worth
keeping for the carve.

**AND A JUMP TABLE AT 0 MOD 8 IS FINE.** `func_80095538` landed one
(`jtbl_80088AA0`) on 2026-11-15 and `build/dungeon.bin` is byte-identical:
there the `.align 3` is a no-op, and the `.word 0` splat shows after the 21
entries is regenerated by the NEXT table's own `.align 3`. So the rule is
about those seven addresses, not about jump tables.

## Two levers this block has already needed

Both are about where a function's `return` statements are, and both cost two
words per member across a family.

### A SINGLE trailing `return 0` keeps the branch that four returns collapse

The six flag probes (`func_800C7FFC` and siblings, 31 words each) end in a
test whose two arms return 1 and 0. Written

    if (A) { for (...) { if (B) return 1; } return 0; }
    if (C) { return 1; }
    return 0;

`jump.c` store-flags the LAST pair into one `sltu $v0, $zero, $v0` and the
function comes out 29 words. Written with one shared exit --

    if (A) { for (...) { if (B) return 1; } }
    else if (C) { return 1; }
    return 0;

-- the branch survives and it is 31 words, 0 differing. Measured: bool return,
explicit `else`, inverted test, `goto`, a local, and a `do`/`while` loop all
give 29 and 9 differing; only the shared exit reaches 0. The `return` in each
arm is also what keeps `$v0` live into the epilogue, so the else arm's mask
chain lands in `$v1`/`$a0` rather than `$v0`/`$v1` -- which is
`docs/matching-levers.md`'s "Duplicate the tail per arm" read from the other
side.

### A VALUE REUSED FOR TWO PURPOSES IS TWO LOCALS

`func_800C8844` and its two siblings (47 words) draw a random number, take it
modulo a byte field and compare the remainder against a parameter. Written
with one variable --

    roll = func_800A6D30();
    if (p[3] != 0) { roll = roll % p[3]; }

-- the pseudo lives from the call to the compare, which is longer than the
promoted parameter, so local-alloc hands `$v1` to the parameter and `$a0` to
the remainder, the `addu $a0, $s0, $zero` for the next call fills a delay slot
that should hold a `nop`, and the function comes out 46 words and 23
differing. Split into `s32 r` for the draw and `s16 roll` for the remainder it
is 47 and 0. Both types are forced: the `s32` is where the
`andi $v0, 0xFFFF` comes from, since `func_800A6D30` is declared `u16` and the
widening happens at the assignment.

Eleven spellings were measured on that one function; the two that differ only
in which local holds what are the two that matter.

## The state of the sweep

Take the numbers from `progress.py`. As of 2026-11-14 the block is open, **126
of 863 functions are landed at 0 differing** and two are parked, all four
containers are byte-identical, `guard.py` accounts for every stub, the
whole-unit `ovlverify.py` sweep is clean, and `clang-format` has **no**
complaints about `csrc/ovl_dungeon.c` -- keep that at zero, it is a better
baseline than `ovl_town.c`'s 31 and it costs nothing to hold.

**What the first day of sweeping actually bought, and why.** 4,317 words came
out of three readings and their substitutions:

* the `0x800D1C70..0x800D4634` cluster, fifteen of sixteen functions and
  2,375 words. Three entity families, each with a spawner, a per-frame step
  driver and a +0x8C handler, plus a shared four-state glide machine in four
  copies and a walk, a fade-out and a fourth spawner. Read `func_800D21CC`,
  `func_800D1E34`, `func_800D20D8` and `func_800D1C70` and the other eleven
  are substitutions.
* the kill handlers, eleven of thirteen and 819 words, spread over
  `0x800BD45C..0x800C4220`. One skeleton -- the D_800E3D7C identity test, the
  cached-RAM pointer gate, the D_800DDE84 lookup, and a close on
  func_80098B38 and the engine's +0x0A counter -- with a different middle per
  entity. Read `func_800BD72C` and the rest are middles.
* the 67 landed with the carve itself.

**The one park.** `func_800BD45C` and `func_800BD5C4` are the kill skeleton
plus the message chain and sit at 90 of 90 words, 2 differing: one copy
reversed around func_800990FC's handle. `docs/near-misses/func_800BD45C.c`
has twelve measured spellings and the cse reason they all fail. Landing it
lands both, 180 words.

**THE REUSED-TEMPORARY SEAM IS TWELVE-SIXTEENTHS CLOSED** (2026-11-14,
commit "Close the reused-temporary seam"). The idiom -- a message handle used
once as an argument and once after a later call -- matched nineteen functions
in this overlay and nowhere else. Fifteen are now landed; three resisted and
are the ONLY ones left in it, each with a measured residue:

* `func_8008F428`, 177/177 words, **15 differing**. Thirteen are the two masks
  in the `p[0x9B] == 1` arm -- the target materialises both before touching
  D_80081484, every spelling tried materialises each at its use. Five
  statement orders and a `u32` local per constant move nothing.
* `func_8009255C`, 162/162, **38 differing**, all of them the shift caused by
  where `(u8 *)&D_80083160`'s `lui`/`addiu` land. The bind: the field at
  `g + 0xC8` is read TWICE in case 0 and the target does not common them,
  which only happens when `g` is assigned in a DIFFERENT basic block --
  and assigning it inside the arm folds the address and commons the second
  read away (159 words, three short).
* ~~`func_800B7774`, 244/251, **147 differing**. Seven words short, all in one
  `while` loop whose exit test the target DUPLICATES above the loop ...
  `jump.c:599`'s `duplicate_loop_exit_test` is the lever.~~
  **THIS DIAGNOSIS WAS WRONG, and it was a dispatcher lead, not an agent's
  reading of the listing.** `func_800B7774` LANDED at 251 words, 0 differing
  on 2026-11-15, and neither the loop nor cross-jumping was involved at any
  point. The guard is produced correctly by any plain `while` here, because
  the loop's `*(u32 *)(q + 0x18)` operand is cse-forwarded from the `sw` that
  precedes the guard -- so the guard's `sltu` reads the stored register while
  the bottom test reloads, the two tails differ BY CONSTRUCTION, and
  `find_cross_jump` can never fire. `docs/matching-levers.md`'s cross-jumping
  mechanism is sound; it simply does not apply to this function.

  The seven words were six independent one- and two-word misses scattered
  across the function, and the three that generalised are now catalogue
  entries: the number of source locals holding one global's address deciding
  whether its `%hi` and `%lo` share a register (51 differing and one word);
  where a global is READ deciding a delay-slot fill independently of the
  dependent store (5 differing); and an extra `addu` copy surviving into the
  output being an `optimize_reg_copy_1` residue that only a loop NOTE between
  the copy and the source's death can block.

  **The lesson for the dispatcher is the one worth keeping:** a seven-word
  deficit concentrated near a loop invited a single-mechanism explanation, and
  a named gcc function with a plausible precondition made it feel verified.
  It was never measured. A lead handed down with a `jump.c` line number reads
  as established and costs an agent its whole budget chasing it -- so say
  "unverified guess" when it is one.

**THE KILL-HANDLER FAMILY IS THE BIGGEST NAMED SEAM LEFT: 21 members, 3,295
words** (2026-11-14, after the parallel wave). `func_800BD72C` is the bare
skeleton and nineteen copies are landed; every remaining member opens with the
same four blocks and differs only in its tail. Find them with

    grep -l 'func_8008D330\|func_8008D344' asm/nonmatchings/ovl_dungeon/*.s

and cross-reference against the `INCLUDE_ASM` list. Four of them landed on the
FIRST COMPILE from the template alone (func_800C4324, func_800C30D4, and
func_800BE360 after one arm flip), so this is transcription, not
investigation -- batch it with the skeleton named in the brief.

**AND RUN `xsibs.py` AGAIN BEFORE ANYTHING ELSE.** It found `func_800B653C` at
0.969 against town's landed `func_800ABC00`; that one was a 99-word function
landed on the first compile by reading town's C and changing four constants.
A cross-container sibling at >= 0.95 is close to free, and the tool's output
goes stale as the unit fills, so re-run it rather than trusting the numbers in
this file.

What the tools say is left, and where to go next:

* `nearsibs.py --groups` is down to 16 sets and 2,807 words. The largest left
  are the five-member `func_800BF210`..`func_800BF6D0` run (379 words), the
  `func_800BEB30`/`func_800BEDEC` pair (337) and `func_800D5AC0`/`func_800D68F4`
  (240). The five-member run is the best value.
* `families.py` reports 22 functions / 1,760 words in exact-shape families.
* `func_800D3738` is 527 words and is the one function the 0x800D1C70 cluster
  is still missing -- a singleton, and the largest in the region.
* The rest have no twin: that is the bulk, and `docs/town-sweep.md`'s cluster
  economics is the answer to it.

**Two things this block has already taught that generalise**, both now in
`docs/matching-levers.md`: a four-word `if` arm whose only content is a load
is a bare `volatile` read, and when the residue is two callee-saved registers
swapped, read `.lreg`'s ref/length lines and solve for the reference count
rather than sweeping spellings -- eleven natural rewrites on `func_800D21CC`
left both numbers unchanged to the reference.

## SETTLED: where a parameter lives

This section used to be the block's open question -- two functions had been
lost to a parameter the target keeps in its incoming home slot. It is
answered, it cost 312 words to leave unanswered, and the full write-up is in
`docs/matching-levers.md` under "WHERE A PARAMETER LIVES". In short:

* A REGISTER parameter is in a pseudo unless the SOURCE made it addressable.
  `assign_parms` (function.c:3775) gives it the home-slot MEM only for
  `TREE_ADDRESSABLE`, `volatile`, or a BLKmode type -- there is no cost path,
  so the allocator never chooses memory for one. `func_800D5460`'s second
  argument is addressable, and `(void)&colour;` says so at no cost in code.
* A STACK parameter (the fifth and up) prefers memory, because
  `regclass.c:786` DISCOUNTS `mem_cost` for a pseudo whose init loads from
  its own slot. `func_800DBC20`'s sixth argument is a promoted `s16` -- `lw`
  then `sll 16`/`sra 16` -- and declaring it `s32` with an explicit cast is
  a word short.

Both pairs are landed. The general lesson for the rest of this block: when
the residue is "which register did this parameter get", read the parameter's
own declaration off the load and the call, not the allocator.

## PARALLELISING THIS BLOCK: one copy of the unit per agent

The whole overlay is ONE file, `csrc/ovl_dungeon.c`, so agents cannot each
edit it. They do not need to. `tools/decomp/ovlverify.py` takes the source
path as its first argument, so an agent can work entirely on its own copy:

    mkdir -p /tmp/claude-1000/agent-<name>
    cp csrc/ovl_dungeon.c /tmp/claude-1000/agent-<name>/ovl_dungeon.c
    python3 tools/decomp/ovlverify.py /tmp/claude-1000/agent-<name>/ovl_dungeon.c func_X

**AND EACH AGENT NEEDS ITS OWN `AZURE_SCRATCH`.** `asmlib.compile_asm`
derives its intermediate `.s`/`.o` path from the source file's BASENAME inside
`$AZURE_SCRATCH` (default `/tmp/azure-decomp`). Since the basename has to be
`ovl_dungeon.c` -- see the next paragraph -- every agent otherwise writes the
same `ovl_dungeon.s`, and two concurrent compiles corrupt each other. Two
agents in the 2026-11-14 wave hit it independently:

    export AZURE_SCRATCH=/tmp/claude-1000/agent-<name>/sp

The failures are loud and misleading: `unrecognized opcode 'macro'`, `junk at
end of line`, `symbol already defined` on a run of functions the agent never
touched. They read as a source defect and are not one. A compile that merely
finishes first could in principle hand back another agent's object silently,
so this is a correctness fix, not just an annoyance.

**THE COPY'S BASENAME MATTERS, and this recipe used to get it wrong.**
`ovlverify.py` picks the container ELF from the SOURCE FILE'S BASENAME
(`stem in CONTAINER`). A copy called `agent-c4.c` falls back to
`build/town.bin.elf`. For a single named function that still has its listing
nothing goes wrong, which is why this went unnoticed for two waves; in a
whole-unit sweep it produces dozens of `NO TARGET` lines and at least one
plausible-looking false failure (`func_800C838C` measured 25 against 31 words
that way, and 31/31/0 when named properly). Keep the copy named
`ovl_dungeon.c` in a directory of its own.

That measures against the reference image exactly as it does in the real
unit, with no build and no shared state. Each agent hands back the C it
landed and the measured word counts; the dispatcher merges, runs
`ovlverify` again on the real file, `clang-format`, `make build/dungeon.bin`
and commits. **Verify every landing yourself after merging** -- an agent's
copy can drift from the unit while it works, and two agents can both add the
same declaration.

**Tell every agent not to run `make`.** It regenerates `asm/nonmatchings/`
and deletes the listing of every function that is already landed; ovlverify
then takes that function's extent from the next ELF symbol, measures past
its end, and reports plausible-looking numbers. An agent that builds will
quietly poison its own measurements and everyone else's.

**Batch by mechanism, and name the mechanism in the brief.**
`docs/matching-levers.md` at `:9195` has the measured costs: a function whose
mechanism is already named costs 130-230K tokens and lands, an open-ended one
costs about 315K and comes back short. Ten functions sharing one recipe came
back all-MATCH in a single 152K run. The batches that suit this block are the
near-sibling sets (one reading, N substitutions), the parked near-misses (the
analysis is already written, hand them the file), and runs of small functions
sorted by size.

**Build only this container while others are working.**
`make build/dungeon.bin` links and checks dungeon.bin alone; `make
containers` does all four and will report a town.bin mismatch that belongs to
whoever is editing town, not to you.

## 2026-11-15: THE SEAMS ARE SPENT, AND THE PERMUTER IS THE REMAINING EDGE

`progress.py` as of 2026-11-15: **565 of 863 landed, 298 stubs, 29.5%, no
parks in the unit**. The whole-unit `ovlverify.py` sweep is clean over all
563 names, `clang-format` still has no complaints, and
`build/dungeon.bin` is byte-identical.

**Every structural seam in this block is now closed, and the tools say so
rather than a guess.** On the 298 stubs that remain:

* `families.py ovl_dungeon` -- **0 families with >= 2 members.**
* `nearsibs.py ovl_dungeon --ratio 0.85` -- **3 pairs, 651 words**, the
  largest `func_800B8FC8`/`func_800B835C` at 0.891.
* `xsibs.py` was exhausted the day the block opened: 19 functions, 550
  words, nearly all landed.

**THAT xsibs CLAIM WAS WRONG, AND IT STAYED WRONG FOR A DAY.** It rested on
the 0.85/0.88 runs only. At `--ratio 0.60` the same tool, on the same block,
reports **18 functions and 5,972 words** -- and the leads are real, not noise:
`func_800CF8E4` (932 words) is `csrc/ovl_town.c`'s `func_800AC4C4` rebuilt
around a 6-byte map cell instead of a bare `u16`, and reading it that way took
a cold function to **932 of 932 words, 8 differing** on the first serious
compile. The 0.602 score badly undersells it: the object loop, the four edge
loops, the vertex block, the LINE_F4 block and the backface tail are all
town's shape, and town's own scheduler levers transfer verbatim.

Five parks still carry untried templates from that run:

| words | park | template |
| --- | --- | --- |
| 932 | `func_800CF8E4` | `town.bin:func_800AC4C4` 0.602 |
| 528 | `func_800C6B40` | `slus:func_800453E0` 0.628 |
| 390 | `func_800D07C8` | `slus:func_80045CC4` **0.832** |
| 352 | `func_800B9144` | `slus:func_8003D0F0` 0.645 |
| 211 | `func_800D0DE0` | `town.bin:func_800ABEEC` 0.716 |

`func_800D07C8` at 0.832 is the sharpest: it was reconstructed BY HAND with no
template and parked at 388 of 390 words with 378 differing.

**Read the ratio as a floor on structural similarity, not as a confidence
score.** A 0.60 sweep over this block costs about ten minutes of CPU and is
the cheapest lead generator left. Note also that the tool scans the asm
listings, so most rows in a 0.60 run name functions that are already landed --
filter against `csrc` before reading the list as work.

The kill-handler family, the 0x800D1C70 cluster and the reused-temporary
idiom are all done. What is left is 298 singletons, and the size profile has
moved with them: the smallest remaining stub is 53 words, the median is
around 140, and 204 of the 298 are over 120 words. **Batch sizes have to
come down with it** -- eight functions per agent at 85-95 words, fewer above
that.

**THE PERMUTER IS WORTH MORE THAN ANOTHER HAND SWEEP, AND THE NUMBERS ARE
NOT CLOSE.** `func_800AC480` sat at 82 of 82 words with four differing after
thirty-three measured spellings across two sessions -- an equal-depth
sched1 tie that no source order reaches. `permimport.py --source <the agent's
own unit copy>` followed by `permrun.py --minutes 45 --jobs 6` found
`base=120 -> best=0` in **two minutes**, and the winning idea (hold the
constant in a local so `fold` cannot reassociate, instead of splitting the
statement) generalises -- it is in `docs/matching-levers.md` now.

The workflow that follows from that:

1. Agents land what they can and **save every residue's best candidate** to
   its own file. A residue with no saved candidate cannot be permuted, so
   the brief now asks for `/tmp/.../func_XXXXXXXX_best.c` explicitly.
2. Agents do NOT run the permuter. `permrun.py`'s jobs are a machine-wide
   budget of 20 and only one search runs at a time; with five agents going,
   that budget belongs to the dispatcher.
3. The dispatcher queues the residues and runs them one at a time between
   merges. `permimport.py --source` takes the agent's whole unit copy, so
   there is nothing to extract by hand.

Two parks are open at the time of writing, both with saved candidates and
both queued:

* `func_800A31D0`, 52 of 53 words, 23 differing. The missing word is the
  `nop` after `lhu 0xA($a3)`: the target does not interleave the two counter
  read-modify-writes and every hand spelling does.
* `func_800C0230`, 117 of 117, 26 differing, and the whole residue is one
  delay slot -- the target fills the cached-RAM gate's `bnez` from the
  fall-through and leaves `lui %hi(D_80012090)` at the head of the next
  block; `reorg` here steals that `lui` into the slot instead. Every word
  from the following `lw` to the end is identical.

## 2026-11-15, LATER: 791 OF 863, AND WHAT THE PERMUTER IS AND IS NOT FOR

`progress.py`: **791 of 863 landed, 72 stubs, 73.4%**, from 565 earlier the
same day. (As of 2026-11-16 it is **795 of 863, 68 stubs, 75.2%** -- see the
/tmp entry below for the four that got it there and the detour they took.) The whole-unit `ovlverify.py` sweep is clean, `clang-format` has no
complaints, and `build/dungeon.bin` is byte-identical.

**THE PERMUTER DOES NOT CRACK A SMALL PURE-ALLOCATION RESIDUE. IT IS FOR
MISSING OR MISPLACED INSTRUCTIONS.** This is now measured on both sides and it
is the single most expensive thing to get wrong, because a 30-minute search
that cannot win looks exactly like one that has not won yet.

* `func_800C0404`, base 60, **best 60 after 132,425 iterations.** The residue
  was entirely register numbers at identical word counts.
* `func_8009A8C0`, base 825 -> **best 525**, and the real value was not the
  525: splitting that answer into its independent edits and measuring each one
  took the function to **27**. A permuter answer is a bag of ideas, not a
  patch -- take it apart.
* `func_800B06F0` closed **outright at 0** from a length-shifted base.
* `func_800C9AAC`: nothing.

So the triage rule before queueing a residue: if `aligndiff` says the word
counts match and every difference is a register number, the permuter will not
help and `.greg`/allocation work is the only route. If the candidate is short
or long by a word or two inside one block and the structure is proved
elsewhere, queue it.

**THE PARK-CLOSING ROUNDS DECAYED CLEANLY, AND THAT IS WHEN TO STOP.** Five
successive agents whose whole job was to close existing parks returned 4 of 5,
3 of 5, 2 of 5, 1 of 5, 0 of 5. The decay is the signal: the parks that close
under a fresh reading close in the first two rounds, and after that the same
budget is worth more on cold functions. A sixth round was justified only
because `xsibs --ratio 0.60` had produced genuinely new templates, which is a
different kind of lead from "read it again".

**A PROVED-UNREACHABLE RESIDUE EXISTS IN THIS BLOCK, AND IT IS FOUR WORDS.**
The `|= 0x80000000` gate's `$v0`/`$v1` swap has now been seen in three
functions -- `func_8009C93C`, `func_800AAF00`, `func_800D92C0` -- with every
spelling swept in each. **Subtract those four words before judging whether a
function containing that gate can close**, or you will read a landing-shaped
residue as a failing one.

Open parks at the time of writing, both with saved candidates:

* `func_8008ACDC`, **739 of 741 words, 19 aligned differences.** The whole
  residue is two words at `0x8008AFB0` in the facing compare: a missing
  `andi $v0, $v0, 0xFF` and a missing `addu $v0, $a0, $zero`. Twenty spellings
  of that one expression were measured (the best, `u8 i; want = i << 9`, is
  the saved body); `s32 i` costs 229, `i = n[1]; i &= 7;` costs 243. Queued
  for the permuter -- one word off in one 40-word block is the shape that
  wins.
* `func_800CF8E4`, **932 of 932 words, 8 differing**, a register swap of two
  hoisted constants. Word counts match and every difference is a register
  number, so by the rule above this is NOT permuter food; it needs `.greg`
  work.

## 2026-11-16: /tmp IS NOT DURABLE, AND IT COST FOUR LANDED FUNCTIONS

Between one working day and the next, a routine cleanup wiped everything under
`/tmp/claude-1000`. That took, in one go:

* **Four closed functions that had never been reported.** The park-closing
  agent's last status line was "All four closed" -- `func_800D07C8`,
  `func_800C6B40`, `func_800B9144` and `func_800D0DE0`, the four 0.60-ratio
  xsibs leads -- and its run was cut off by a rate limit before it wrote its
  report. The bodies and the measurements went with the directory.

  **These four were recovered**, and how is worth knowing: an agent's
  conversation is held by the session, not by its working directory, so
  `SendMessage` to its agent id resumed it and it re-emitted all four bodies
  from its own transcript. Each was re-spliced and re-verified from scratch
  rather than trusted, and all four landed. That route is only open while the
  session still holds the agent -- `ListAgents` showing nothing means it is
  gone -- and it does not recover everything: the same trick on a second agent
  got its levers back but not the 478-line decode it had written to a file,
  because the file's text had been produced by a series of patches rather than
  emitted whole.
* Five agents' unit copies, every saved `func_XXXXXXXX_best.c`, and the
  agents' own transcripts, which were the other way back to those bodies.
* A permuter run in progress, and the dispatcher's own park candidate for
  `func_8009A540` at 75 of 75 words with 23 aligned differences.

Nothing in the repo was lost. **Everything lost was lost because the brief
told agents to keep their working files in `/tmp`.**

**So the rule is now: a candidate goes in `docs/dungeon-parks/func_XXXXXXXX.c`,
in the repo, the moment it is the best one -- not at the end of the run.** One
file per function, so concurrent agents never collide on a path. The agents do
not stage or commit it; the dispatcher does. The earlier instruction in this
sweep's brief -- "Do not park anything in docs/near-misses/; just report it" --
was wrong and is withdrawn. It was meant to keep a churning parallel sweep out
of the repo's park tooling, and the cost of that tidiness was four functions.

The new directory is `docs/dungeon-parks/`, not `docs/near-misses/`, because
the two hold different things. A near-miss file is a self-contained park
source that `parkstats.py` and `nmcheck.py` compile on their own; a dungeon
park is a function BODY that needs the ~1500 lines of declarations at the top
of `csrc/ovl_dungeon.c`, so putting one in `docs/near-misses/` makes
`parkstats.py --check` fail on a file it cannot read. `docs/dungeon-parks/`
has its own README and its headers are hand-written on purpose: the part worth
keeping is the list of spellings already measured and rejected, with their
scores, which is the thing that stops the next reader paying for the same
sweep twice.

**And the brief template itself now lives in the repo**, at
`tools/decomp/dungeon-brief.txt`, for the same reason: the previous copy was in
the session scratchpad and went with everything else.

**The second lesson is about reporting cadence.** An agent that finishes four
functions and reports none has banked nothing. The brief now asks for a
one-line interim message the moment a function reaches `0 differing`, naming
the function and the measurement. It costs the agent almost nothing and it is
the only thing that survives a run that is cut off -- by a cleanup, a rate
limit, or anything else.

## What has NOT been done

* **The overlay's DATA is not owned.** The splat segment stops at the end of
  text (`0xf7700` / `0x800DCE60`), so everything the block keeps above that
  address is an `extern` resolved out of
  `splat/dungeon.bin_undefined_syms_auto.txt`. `docs/owning-data.md` is the
  procedure; `main.bin`'s two overlays are the worked examples.
* **The unit is not carved.** One `c` subsegment stands in for what were many
  translation units. The rodata partition is the anchor set that pins the
  carve -- `docs/overlays.md`, "The sweep produces the anchors that pin the
  carve" -- and the seven misaligned jump tables above will force the first
  cut whether or not the anchors are ready.
* **The rest of the container.** `docs/dungeon-map.md` has ~195 banked
  per-slot overlays, a 0x80016000 low-RAM overlay, a second 0x80020000
  occupant and an unplaced head region, none of them split. That is where most
  of this container's 6.1 MB of unplaced code is, and pinning the bank starts
  across `0x9bd800..0x135e000` is the one piece of that map still to do.
