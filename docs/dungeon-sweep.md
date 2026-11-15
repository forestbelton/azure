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

    cp csrc/ovl_dungeon.c /tmp/claude-1000/agent-<name>.c
    python3 tools/decomp/ovlverify.py /tmp/claude-1000/agent-<name>.c func_X func_Y

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
