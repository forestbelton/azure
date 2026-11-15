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

Take the numbers from `progress.py`. As of 2026-11-14 the block is open, 67 of
863 functions are landed at 0 differing, all four containers are
byte-identical, `guard.py` accounts for every stub, the whole-unit
`ovlverify.py` sweep reports 67 of 67 clean, and `clang-format` has **no**
complaints about `csrc/ovl_dungeon.c` -- keep that at zero, it is a better
baseline than `ovl_town.c`'s 31 and it costs nothing to hold.

What the tools say is left, and where to start:

* `nearsibs.py --groups` reports 22 sets totalling 5,496 words. The richest is
  four 166-word functions at `func_800D21CC`/`800D29D8`/`800D3234`/`800D4634`,
  and the whole `0x800D1C70..0x800D4634` region is one cluster -- FOUR sets and
  THIRTEEN functions, 1,690 words, which looks like one translation unit with
  several near-identical entities. Start there.
* The second richest is `0x800BD45C..0x800C37C8`: seven members of 73..90
  words, 565 words, spanning two address runs, with a six-member 434-word set
  interleaved with it.
* `families.py` reports 35 functions / 3,231 words in exact-shape families.
  The installer families described above came out of it; what is left is
  mostly pairs.
* 761 functions have no twin at all, 81,140 words. That is the bulk, and
  `docs/town-sweep.md`'s cluster economics is the answer to it.

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
