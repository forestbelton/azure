# Overlays: how the game's code is actually laid out

What the overlays are, where they load, and the mechanisms that apply to all
overlay code.

**Fixed findings only.** The counts here -- functions, words, code spans, vote
scores -- are properties of the images and do not change. How much of any of it
is decompiled is deliberately NOT recorded here: run
`python3 tools/decomp/progress.py` and `python3 tools/decomp/guard.py`, which
are the authority.

## The PSX model, in one paragraph

There is no virtual memory and no demand paging. The R3000 has no MMU for user
code: `0x80000000` is a fixed-offset cached window onto the 2 MB of physical
RAM, so a "virtual address" here is just a physical address with the cache bit
set. An **overlay** is therefore nothing clever -- it is a blob of code
compiled and linked for one fixed address range, read off the disc into that
range at runtime, and called. Nothing is relocated when it loads: every
address inside it was baked in at link time. Two overlays that occupy the same
range are simply mutually exclusive at runtime.

## There is no "load overlay" BIOS call

The kernel's loader functions are about whole programs, not overlays:

* `Load(path, header)` reads a PS-EXE into the address named in its own header;
* `Exec(header, argc, argv)` jumps into one;
* `LoadExec(path, sp, gp)` does both and does not come back.

Some games do use `Load`/`Exec` to swap in overlays, since a PS-EXE's header
carries its own load address. But the common pattern -- and the one Azure
Dreams uses -- is for the game to do it itself: seek with libcd, read the
sectors into a buffer at the right address, and then call a function pointer or
a known absolute address. The "overlay system" is ordinary file I/O plus the
linker having been told where things go. Nothing marks an overlay as special at
runtime.

## How the toolchain supports it

PSY-Q's `psylink` has explicit overlay support: the linker command file
declares overlay groups, each with a load address, and symbols inside resolve
at link time. The linker emits each overlay as its own binary image plus a
symbol table.

The consequence that matters for us: **one address can mean different code in
different overlays.** splat, and every decompilation convention built on it,
assumes an address identifies a thing. Any attempt to map these files has to
carry an overlay identity alongside each address.

## What this project's own binary shows

The resident executable `data/slus_006.14` is loaded at `0x8002D000` and ends
at `0x8008875F`. Every function the engine calls but does not contain sits
**immediately above that**, which is the classic layout -- resident engine low,
overlay region directly on top:

```
800894A0                                  one isolated entry
800990FC 80099194 80099290 80099734       a dense run, gaps of 0x98..0x8F4
8009A028 8009A21C 8009A3D0
800A0B94 800A1618 800A1BD0 800A32A4
800A48F0 800A5720 800A9A0C 800AA36C
800AA53C 800AA888 800AC3EC 800AC4C4
800AC82C                                  ...run ends ~0x800AC82C
800D5460                                  +0x28C34 -- a different region
801768AC                                  +0xA144C -- near the top of RAM
```

The run from `0x800990FC` to `0x800AC82C` spans about 80 KB with small gaps,
which reads as **one overlay** holding many functions that the engine calls.
`0x800D5460` and `0x801768AC` are far outside it -- either further overlay
regions or separately loaded blocks. (RAM ends at `0x80200000`, so
`0x801768AC` is near the top, where a large buffer would sit.)

What the engine actually calls out to is informative: func_80042BDC's message
idiom is `func_800990FC()` / `func_80099734()` / `func_80099194()` /
`func_80099290()` / `func_800A5720()` -- allocate a message, bind an entity and
a string to it, render, release. So the text and UI layer lives in the overlay
and the engine depends on it. The engine is not a self-contained program.

## Why we can match the engine without the overlays

Because nothing is relocated. A call into an overlay is a `jal` with a 26-bit
absolute target, so the only thing the engine needs is the *number*, and
`splat/slus_006.14_undefined_funcs_auto.txt` supplies it as a linker-script
assignment. That is exactly why the engine's functions can match byte-for-byte
against code that calls into a 1 MB overlay nobody has looked at.

The same mechanism is what makes the overlays hard: their code is full of
`jal`s back into the engine and references to engine globals, so they are not
independent either.

## Where the overlays load

Five code overlays are placed. None of the containers is loaded as a
unit -- they are 2.5/16.1/26.5 MB against 2 MB of RAM, and the disc is
addressed by absolute LBA, so a container is just a region of it.

| Container | Overlay at | Loads at | Code | Vote | Functions |
|---|---|---|---|---|---|
| `town.bin` | `+0xb000` | `0x80088760` | `+0xd630..+0x51fc0` | 536 | 1815 |
| `dungeon.bin` | `+0xa3000` | `0x80088760` | `+0xa3e28..+0xf7700` | 353 | 862 |
| `main.bin` | `+0x21800` | `0x80088760` | `+0x21968..+0x24110` | 29/29 | 26 |
| `main.bin` | `+0x25d000` | `0x80020000` | `+0x25d924..+0x264e20` | 130 | 241 |
| `main.bin` | `+0x269000` | `0x80400000` | `+0x2698a0..+0x2719e0` | 142 | 263 |

("Vote" is the jal-against-prologue agreement score that placed the overlay,
except for `main.bin+0x21800`, which is small enough that the score shown is
the stricter function-start test `tools/decomp/codemap.py` uses -- 29 of 29.
"Functions" is what splat emitted. The code spans end on the 16-byte boundary
splat's `__romPos` alignment requires -- see each yaml's comment and the rule
below.)

**2026-11-10: the table is the CODE, not the overlay.** The engine's overlay
load table at vram `0x80080C38` gives each of these reads a length, and every
one is longer than the code above: 278 sectors for town (`+0xb000..+0x95AC4`),
187 for dungeon (`+0xa3000..+0x100800`), and a *third* descriptor at
`0x80088760` that this document dismissed -- 14 sectors at
`main.bin+0x21800`, which `codemap.py --pin` places at that base at 5/5
(100%). So town's and dungeon's overlays carry data past their text, and there
is a fifth placed overlay. `docs/town-map.md` has the table, the evidence and
the follow-ups.

**2026-11-11: that fifth overlay is now split and in the build.** It is the
GAME OPTION screen, and the row for it is in the table above. See "The
`0x80088760` overlay in `main.bin`" below.

`0x80088760` is the first byte after the resident engine's last
(`0x8008875F`), which is the layout this document predicted. town and dungeon
share it, so they are mutually exclusive and an address there means different
code depending on which is resident -- the identity problem described above is
real and unavoidable. `main.bin`'s overlay is different: it loads *below* the
engine, into the `0x80010000` region the engine reaches with the out-of-image
constant-base-pointer idiom.

Each overlay is rodata first and code second, which is gcc 2.7.2 at `-G8`
emitting data ahead of functions (see `include/common.h`). That is why
`town.bin+0xb000` opens on Shift-JIS text and `main.bin+0x25d000` on an
"Assertion failed" string rather than on instructions.

**The method, since it needs no emulator and generalises.** Collect every
offset in a container whose word is `addiu sp,sp,-N` (a function prologue).
Then find the single offset-to-address delta with the most agreement, by
either of two votes:

1. against the 49 overlay function addresses in
   `slus_006.14_undefined_funcs_auto.txt` -- town scored 15/49, dungeon 12/49,
   both against a noise floor of 6;
2. against the block's *own* `jal` targets, which needs no symbol list at all
   -- town 536, dungeon 353, main 130, all against a noise floor of 0. This is
   the stronger test and the only one that worked for `main.bin`, whose code
   the engine barely calls (4/28 on the first test).

Disassembling a predicted function confirms it: `town.bin+0x2ec8c` at vma
`0x800AC3EC` is a clean prologue that builds `0x80083178` (our
`D_80083178`) and makes internal `jal`s to `0x800AC37C` / `0x800AC1B0`.

All three start offsets are confirmed against live hardware behaviour. A
PCSX-Redux CD-ROM command log (`Log` window -> `Special` -> "Log CD-ROM
commands", plus `-stdout`) shows the game seeking LBA 3027 =
`main.bin+0x25d000` during boot, LBA 3099 = `town.bin+0xb000` on entering the
town, and LBA 12636 = `dungeon.bin+0xa3000` on entering a dungeon -- the last
being the most-requested address of the whole dungeon session, at 21 seeks,
which is what reloading an overlay per floor looks like.

`dungeon.bin` holds only one code overlay. Its seeks repeat at `0xa3000` plus
`0x200000` strides, as if the container were chunked in 2 MB units, but
assuming the same base at those offsets scores 0-45 against `+0xa3000`'s 353:
they are assets and low-RAM data. `+0x2a3000` holds pointers into
`0x8001d4xx`, so it belongs to the same low-RAM region `main.bin`'s overlay
occupies, and `+0x9f000` / `+0x4a3000` are texture records.

`0x801768AC`, the isolated address near the top of RAM, is in none of the
spans. `0x800D5460` is, in dungeon's.

## The `0x80088760` overlay in `main.bin`: the GAME OPTION screen

Mapped 2026-11-11, split, and byte-identical in the build. This is the third
descriptor in the engine's load table and the one the 2026-11-10 note above
left as a follow-up. It is the only overlay that loads into the shared
`0x80088760` slot from `main.bin`, which is why nothing about `main.bin`'s own
layout hinted at it: both of the container's *other* code blocks load
somewhere else entirely.

**What loads it.** The third descriptor at vram `0x80080C48` is
`0x07088760 / 0x0000075C` in the `{nsec << 23 | dest, LBA}` form: 14 sectors
to `0x80088760` from LBA 1884. `spec.yaml` extracts `main.bin` from sector
1817, so the read starts at `(1884 - 1817) * 0x800 = 0x21800` and runs
`14 * 0x800 = 0x7000` bytes into `0x80088760..0x8008F760`. The PCSX-Redux
CD-ROM command log already showed a seek to `main.bin+0x21800`; this
descriptor is where that seek comes from, and it supplies the length the log
could not. Unlike the `0x80400000` twin below, **this one is live on retail
hardware.**

**What it is.** Its rodata is the options menu's own string pool:
`GAME OPTION`, `  TO FAMILIAR`, `ATTACK`, `ON`, `OFF`, ` MAGIC EFFECT`,
` SOUND EFFECT`, ` BGM`, `VOLUME`, `MAX`, `MIN`, `  WALL COLOR`, `DUNGEON`,
then `B`/`G`/`R` and fourteen more one-character strings for the per-glyph
renderer. The pointer table at `+0x24350..+0x24408`, in the block's data run,
indexes exactly those strings at `0x800887F4..0x800888A8`.

**Why the base is `0x80088760`.** `codemap.py --pin` reports 5/5 (100%), but
five internal targets is a thin sample, so three further tests by hand:

1. **29 of 29 `jal` targets land exactly two words after an `0x03E00008`.**
   Five are internal -- `0x80088BD0`, `0x80089C24`, `0x8008A98C`,
   `0x8008ABD4`, `0x8008AE60` -- and each is also an `addiu $sp, $sp, -N`
   prologue. The other **24 are resident, and all 24 sit two words after a
   `jr $ra` in `data/slus_006.14` itself.** That second half is the strong
   test and it is one this project had no occasion to run before: it says the
   object was linked against *the shipped engine*. The `0x80400000` twin is
   exactly the opposite case -- it calls `printf` at an address that lands
   inside libgte's `rcossin_tbl` in the retail executable -- so the two blocks
   in this container now bracket the test from both ends.
2. **The data run's self-pointers.** `+0x242D4..+0x2434C` resolve to
   `0x8008B188..0x8008B270`, inside the run's own `0x8008B070..0x8008B380`.
   `0x8008B270` is the upper bound `docs/main-bin-data.md` used to attribute
   to the tables below the overlay; it was this table all along.
3. **The addresses the code builds.** `lui 0x8008` / `lui 0x8009` pairs give
   41 distinct addresses inside the stored image, 28 above the read end (so
   `.bss`, zeroed by the loader rather than stored), and **zero** in the
   `0x8008F380..0x8008F760` gap the sector rounding drags in.

**The boundaries, and the sector count as an independent check.**

```
0x21800..0x21968   rodata, 360 bytes                0x80088760..0x800888C8
0x21968..0x24110   text, 10,152 bytes, 26 functions 0x800888C8..0x8008B070
0x24110..0x24420   data, 784 bytes                  0x8008B070..0x8008B380
0x24420..0x28420   16,384 stored zero bytes         0x8008B380..0x8008F380
```

The file is therefore `0x21800..0x28420`, 27,680 bytes, and
`ceil((0x28420 - 0x21800) / 0x800)` is **exactly 14** -- the descriptor's own
sector count, arrived at from the image without using it. The read rounds up
past the file end, so its last 992 bytes are the head of `main.bin`'s dead
filler run, landing in RAM above anything the overlay touches. That is the
same shape as town's 278-sector read and its `ovl_town_read_tail`.

Two boundary details worth having written down:

* **The text starts on an empty function.** `0x21968` is `jr $ra; nop` and
  nothing else -- a `void f(void) {}` -- which is what puts the rodata/text
  seam there rather than at the next prologue at `0x21970`. The rodata's last
  object is the five-word jumptable at `0x21954`, which is also the offset
  splat's own "likely file split" suggestion names; the suggestion is
  advisory here and declining it is correct.
* **The 16 KB of stored zeros is real and must be reproduced.** Nothing
  references it with a `lui`/`addiu` pair, but it is inside the overlay's own
  file and the sector count proves the loader pulls it in. `splat` keeps it
  as its own `bin` rather than 4,096 lines of `.word 0x00000000`. Note that
  the resident side *does* reference into it -- `D_8008CA34` and `D_8008DAB4`
  were already in `main.bin_undefined_syms_auto.txt` before this block was
  mapped -- which is consistent with the slot being scratch RAM for whichever
  overlay is resident.

**The identity problem, and why it costs nothing here.** This block,
`town.bin+0xb000` and `dungeon.bin+0xa3000` all load at `0x80088760`, so an
address in `0x80088760..` means different code depending on which is
resident. That does not collide in the build: each container's link takes its
inputs from its own `build/*.ld` and never sees another container's objects
(the link rules pass no object files on the command line), which is why
`D_80088760` can be defined by `ovl_town`, `ovl_dungeon` and this unit at
once. None of the 26 function addresses collides with a `town` or `dungeon`
overlay function address either, but that is luck, not a constraint.

### The rodata contract for this unit

It started as `asm` and is `c` as of 2026-11-11. The arrangement that works is
**one dotted `.rodata` over the whole range** plus a separate
`ovl_main_80088760_data` for the data run, and the three arrangements that do
not are worth knowing, because each failed differently:

* **Undotted `rodata` with a `c` text subsegment does not link.** The two
  jumptables hold `.L` labels in the text. A whole-subsegment `asm` listing
  emits those through splat's `jlabel` macro, which is `.global`, so an
  undotted rodata object can reach them -- that is why the `asm` shape worked.
  A per-function nonmatchings listing emits a plain local `.L80089594:`
  instead, and the rodata object gets 13 undefined references.
* **Two dotted `.rodata` subsegments, one per jumptable, silently drops one.**
  splat keys a `c` section's siblings by section name, so a unit has exactly
  one `.rodata` sibling; the earlier of the two is discarded and
  `jtbl_80088770` ends up defined nowhere.
* **One dotted subsegment over everything is right**, and the attribution is
  what makes it safe: each of the seven rodata symbols is referenced by
  exactly one function, and the mapping is non-decreasing in both rodata
  address and function index. splat migrates the two jumptables into their
  functions' own listings and the other five ride as `INCLUDE_RODATA`. Note
  that the C unit has to be DELETED and regenerated to get those
  `INCLUDE_RODATA` lines -- splat writes the file only when it creates it.

**This is what blocks `func_80089608`, and the block is structural.** That
function's three stack tables are LOCAL ARRAY INITIALISERS -- ten pointers,
ten `s16`, ten `u32`, which cc1psx emits to rodata and block-copies to the
stack, the `s16` one through `lwl`/`lwr` because it is only 2-byte aligned.
Landing it therefore makes the C unit own `0x80088790..0x800887F4`, and the
`INCLUDE_RODATA` for `D_800887CC` currently covers those bytes too, so the
unit would emit them twice. Splitting the symbol works -- `size:0x28` plus an
explicit `D_800887F4 = 0x800887F4;` in `main.bin_symbol_addrs.txt` ends the
object at the right place, and `size` alone does not -- but it leaves the
0xB8-byte string pool with no owner: nothing in the TEXT references it (the
pointer table in the overlay's `.data` does, as raw `.word` literals), so
splat can attribute it to no function and writes it nowhere. And it cannot be
given its own undotted subsegment, because that would need dotted `.rodata`
on both sides of it for the two jumptables, which is the second failure above.

**REFINED 2026-11-11, and the earlier advice here was not enough.** It said to
land the two jumptable owners first and then carve freely. Landing them is
necessary but it does not unblock anything, because the obstruction is not the
jumptables -- it is that **every byte of this rodata except the string pool
turns out to be C-owned**, and the string pool sits in the middle of it:

```
0x21800  D_80088760     func_80088BD0's  u8 *tbl[3] initialiser
0x21810  jtbl_80088770  func_800890EC's  switch table
0x21830  D_80088790     func_80089608's  u8 *[10] initialiser
0x21858  D_800887B8     func_80089608's  s16[10]  initialiser (2-byte aligned)
0x2186C  D_800887CC     func_80089608's  u32[10]  initialiser
0x21894  the string pool          <-- the ONLY thing no function owns
0x2194C  D_800888AC     func_80089C24's  initialiser
0x21954  jtbl_800888B4  func_8008A288's  switch table
```

A unit gets exactly one dotted `.rodata` sibling, so an undotted subsegment
for the pool would need dotted rodata on BOTH sides of it. No carve exists.

The actual fix is the one `docs/owning-data.md` §4.7 already records for the
card overlay: **own the `.data` pointer table in C and cc1psx emits the
strings itself, at these addresses.** There, the 0x80020000 overlay's 59
Shift-JIS messages are the string literals of that unit's own `u8 *` tables
and needed no symbol, no subsegment and no `INCLUDE_RODATA`. The same shape is
here -- the table at `+0x24350` in this overlay's `data` subsegment is what
points at the pool, as raw `.word` literals -- so bringing that table into the
C unit is what retires the pool as a separate object. Then one dotted
`.rodata` covers `0x21800..0x21968` with splat writing not a byte of it, which
is exactly how `splat/main.bin.yaml` already describes the 0x80020000 segment.

**RESOLVED 2026-11-11 BY CARVING, which is what this document should have
said from the start.** Two things were blocking integration and both have a
precedent in this repo that I missed:

1. **Jump-table alignment.** cc1psx emits `.align 3` before every jump table,
   and `jtbl_800888B4` has to sit at 0x800888B4, which is 4 mod 8 -- so inside
   one big unit it gained four bytes of pad the reference does not have, the
   unit's `.rodata` went 0x168 -> 0x16C, and that moved `__romPos` and every
   later segment (an 89%-different container, first bad word at +0x21800).
   `docs/matching-levers.md`'s "cc1psx EMITS `.align 3` FOR JUMP TABLES" has
   the mechanism and a one-command screen; `docs/where-we-are.md` records
   ovl_town being split into THREE units for exactly this. THE FIX IS A UNIT
   WHOSE RODATA STARTS AT THE TABLE: `.align 3` at section offset 0 emits
   nothing. splat had been suggesting the exact split point, `[0x21954,
   .rodata]`, since the first split of this overlay.

2. **One dotted `.rodata` sibling per unit** is a per-UNIT limit, so carving
   also buys more of them -- which is what lets the string pool have an
   UNDOTTED subsegment of its own between two C-owned ranges. The pool needed
   one more thing: a declared symbol. Nothing in any text references it (the
   pointer table in this overlay's `.data` uses raw `.word` literals), so
   spimdisasm had no reference to infer one from and the undotted object came
   out holding only D_800888AC. `D_800887F4 = 0x800887F4;` in
   main.bin_symbol_addrs.txt is what makes the 0xB8 bytes disassemble, as
   `.asciz` strings.

So this overlay is now FOUR objects rather than one:

```
ovl_main_80088760      rodata 0x21800..0x21830   text 0x21968..0x226a8
ovl_main_80089608      rodata 0x21830..0x21894   text 0x226a8..0x23328
ovl_main_80088760_str  rodata 0x21894..0x21954   (undotted; the pool + D_800888AC)
ovl_main_8008A288      rodata 0x21954..0x21968   text 0x23328..0x24114
```

and `func_80089608` and `func_8008A288` are both landed. The remaining
`func_800890EC` owns `jtbl_80088770` at 0x80088770, which IS 8-aligned, so it
is unaffected by (1).

Positive evidence that the layout will land: `func_80089608` matches at 259 of
259 words with its three initialisers in place, and `verify.py` reports the
only residue as two relocated words whose addend differs --
`addiu $a2, $v0, %lo(D_800887B8)` and `%lo(D_800887CC)`. Those are gcc's own
labels for tables 2 and 3, and they fall at exactly the original addresses, so
cc1psx reproduces this rodata's layout label for label.

## The fourth overlay: `main.bin+0x269000` at `0x80400000`

This block looked unresolvable at first: 191 prologues, so certainly code, but
only 58 internal jal targets and a best score of 6 against a floor of 5.
**That measurement was wrong, not the method.** Run
over the full `+0x269000..+0x272000` span the block has 654 jal sites to 265
distinct targets, and the second vote -- its own jal targets against its own
`addiu sp,sp,-N` prologues -- scores **142 at `0x80400000` against a next-best
of 33.** The lesson generalises to the containers still unmapped: when this
test comes back at noise, re-check the span and the prologue predicate before
concluding the base is unrecoverable.

Four confirmations beyond the vote:

1. The function-pointer table at `+0x269058` holds `0x804010F0`,
   `0x804013D0`, `0x804013A8`, `0x804012B0`, and the last two are prologues
   at exactly the file offsets that base predicts.
2. The code builds addresses as `lui $at, 0x8041` -- e.g.
   `lui $at, 0x8041; sw $v0, -0x6B14($at)` -> `0x804094EC`, inside the
   block's own data.
3. Text ends at `+0x2719dc` and the `.data` right after it is
   `0x80409140` / `0x8040914C` / `0x80409158` / `0x80409164`.
4. `0x80400000` is upper RAM on an 8 MB DTL-H2000 development board. On
   retail hardware RAM stops at `0x80200000`, so nothing can load this
   block -- which is why the CD-ROM command log shows no seek for it, and
   why the earlier search for a live load address could not succeed. It is
   **dead code on the shipped disc.** It still has to be reproduced
   byte-for-byte, because it is in the container.

### It is a debug build of the `0x80020000` overlay, and that makes it an oracle

The two blocks are the same source compiled twice. 50.4% of the live
overlay's 7487 words are covered by shared masked instruction runs with this
block, the longest 121 words, and the function pointers correspond one-to-one
under a small shift consistent with assert calls being inserted
(`0x800210F0` -> `0x804010F0`, `0x80021298` -> `0x804012B0`). The debug build
has asserts compiled in and the live one does not.

What that buys, and it is the thing this project has never had for any
segment:

* **Translation unit names.** The `Assertion failed: file "%s", line %d\n`
  marker is followed, word-aligned, by the filename. The debug block carries
  nine -- `c_server.c`, `card_mes.c`, `card_opt.c`, `card_win.c`,
  `cardbk.c`, `cardinfo.c`, `cardslot.c`, `cardsme.c`, `memory.c` -- against
  the live overlay's one (`c_server.c`). Unit boundaries are where four
  previous dead ends in this project turned out to be.
* **Source line numbers.** Every assert site passes `__LINE__` as an
  immediate, which brackets function order and rough line structure within
  each unit.
* **Real identifiers.** Not from the `Assertion failed` records themselves,
  which carry only file and line, but from the debug prints beside them:
  `printf("%s = %d\n", #x, x)` and its `%x` sibling, which put the stringised
  expression in rodata next to the format. Those two formats are
  `D_80400038` / `D_80400114`, and a full scan of the block's rodata
  (`0x80400000..0x804008A0`, every NUL-terminated string, not just the long
  ones) finds **exactly four** such operands. All four are now carried into
  the live block where a live counterpart exists:

  | identifier | twin site | twin operand | live site | live spelling |
  |---|---|---|---|---|
  | `card_first_flg` | func_80400E34 | `D_8013BB6C` | start_card_server | `card_first_flg[0]`, a macro over `D_80084130` |
  | `cnt` | func_80401B30, func_804022C8 | `$s1` / `$s0` local | format_card, erase_save_file | *nothing* -- twin-only |
  | `c` | func_80401FEC | `$s1` local | check_gsw_checksum | `c` |
  | `(int)gsw->check_sum` | func_80401F98, func_80401FEC | `0x204($a0)` | set_gsw_checksum, check_gsw_checksum | `gsw[GSW_CHECK_SUM]` |

  Three lessons in that table. **A named global need not be in either block:**
  `card_first_flg` is engine `.bss` in both links, so the name has to be a
  `#define` over the splat symbol (include/structs.h) until that region is
  split, and the incomplete-array declaration the live addressing needs is why
  it reads `card_first_flg[0]`. **A named local can be twin-only:** `cnt`
  exists only to be printed, so the live build has no counter in either
  function and there is nothing to rename -- the recovered name belongs to the
  twin's own future decompilation, and is recorded at those two INCLUDE_ASM
  sites. **A `->` names a member without giving a type:** the `(int)` cast
  says `check_sum` is not plainly `int`, and the live source takes an
  `s32 *gsw`, so the member can only be named as its word index (csrc/main.h).
  `gsw` itself was already adopted as the parameter name from this same
  string.

splat also suggests a rodata file split at `+0x26907C` from jump-table
alignment, which is an independent witness for a unit boundary there.

The subsystem is the memory card: `bu00:`/`bu10:`, `TMP`, `ERROR` /
`NOTHING` / `UN FORMAT` / `NEW READY` / `READY` / `NOW CHECK`, `CARD%d %s`,
`CARD STATUS0 %x`, `illegal data`. Its external calls go to 65 distinct
engine targets, many already named -- `printf`, `exit`, `strcat`, `rand`,
`TestEvent`, `_card_wait` (libcard is resident at `0x3d3f8..0x3d578` with
module names). Its private data lives at `0x80027E20..0x8002BB6C`, a region no
engine C file references -- and as of 2026-11-09 `csrc/ovl_main_80020000.c`
defines all 15,692 bytes of it as 90 objects; `docs/owning-data.md` §7 is the
inventory. (The 0x494 bytes from there to `0x8002C000` are the pad to the next
sector; the `0x8002D000` this used to say was a guess at the far end.)

### The assert-record census across all four containers

Same marker, scanned over every container:

| container | records | translation units named |
|---|---|---|
| `main.bin` | 10 | `c_server.c` (x2), `card_mes.c`, `card_opt.c`, `card_win.c`, `cardbk.c`, `cardinfo.c`, `cardslot.c`, `cardsme.c`, `memory.c` |
| `town.bin` | 76 -- **85**, see below | `main.c` (54), `lshop.c` (10), `tobjs.c` (8), `player.c` (4) |
| `dungeon.bin` | 0 | -- |
| `slus_006.14` | 0 | -- |

Two things follow. **None of town's 76 records is inside the `0x80088760`
overlay** -- they are all at `+0x2ab000` and beyond, in a second tier of
small low-RAM overlays, so the big town overlay has no oracle either. And
**dungeon was built with asserts out entirely**, so there is nothing of this
kind to recover there.

Re-counting town's with a pattern anchored on the NUL terminator gives **85**
records and two more units -- `sn_main.c` (7) and `flgtst.c` (2) -- both in the
`0x806F0000` dev-only band (`docs/town-map.md` §4).

Town's tier 2 also leaks identifiers: `((u_long)p - (u_long)ost_w)/
sizeof(OSETDT)`, `((u_long)pp - (u_long)osel_w)/ sizeof(OSETDT*)`,
`cdhd_cnt`, `s0[0]`, `s1[0]`, `s[0]` -- two variable names and a typedef.

### How much code is left in each container

Scanning for code outside the placed overlays, by 4 KB chunk with at least
four prologues and three `jr $ra`:

| container | unplaced code | runs | largest |
|---|---|---|---|
| `main.bin` | 0 | 1 (**not** noise) | 12 KB / 14 prologues at `+0x21000` -- **this call was wrong twice over**: that run is the 14-sector overlay at `0x80088760`, now split, and the prologue count was low only because the scan started below its text. See above. |
| `town.bin` | 0.64 MB | 59 | 69 KB / 291 prologues at `+0x649000` |
| `dungeon.bin` | 6.13 MB | 401 | 364 KB / 715 prologues at `+0xe2d000`, then 360 KB / 598 at `+0xcdd000` |

So **`main.bin` is structurally complete**: all three of its code blocks --
`+0x21800` at `0x80088760`, `+0x25d000` at `0x80020000` and `+0x269000` at
`0x80400000` -- are split, and nothing else in the container is code.

Town's remaining 59 runs are separately linked low-RAM overlays, and the
second vote is much weaker on them than on the big four: `+0x2ab000` scores
42 against a floor of 9 (-> `0x80015800`), `+0x64b000` only 12 against 3
(-> `0x80016000`). They are small, so there are few internal jals to vote
with. 54 copies of `main.c` across town suggests many separate links of one
source, which has to be checked before trusting any of them.

Dungeon's 6.13 MB is an overestimate: long stretches of it are near-identical
repeated 20 KB / 36-prologue blocks at `0x6000` stride, and the container has
chunks repeating at `0x27B800`. The two large blocks are unambiguously real.

Do not try to trace this with PCSX-Redux Lua breakpoints: on the
`8a86b7ec` AppImage build, `PCSX.addBreakpoint` installs, reports itself
enabled, and never fires -- not even on `VSync` -- with `-debugger
-interpreter` and verified-correct addresses. The CD-ROM command log works and
costs nothing.

## If we ever map more

In rough order:

1. **The archive structure.** The engine's CD queue takes
   8-byte descriptors, `{nsec << 23 | dest, LBA}`, with `dest == 0` meaning
   "the D_80081480 bump arena"; the pump is `func_8003E758` and the queue
   dispatcher `func_8003E4FC`. The 49 descriptors at `0x80081020..0x800811a8`
   are the *audio* table (KCET sequences and VAB banks, verified by the magic
   bytes at each source offset), and `D_8006E6F4` is a 2-entry table of
   descriptor pairs selecting the town or dungeon sound bank.

   This used to end "no descriptor in the ROM targets a code overlay, so
   those are built at runtime." **That was wrong, and it was wrong about the
   49-entry table only.** A separate four-entry table of the same shape sits
   at `0x80080C38`, and three of its four entries ARE code overlays:
   `town.bin+0xb000`, `dungeon.bin+0xa3000` and `main.bin+0x21800`, all to
   `0x80088760`. See `docs/town-map.md` §1 for the table and the
   `0x80088760` section above for what the third one turned out to be.
2. **Recovering an entry's load address.** Prologue/jal fingerprinting, not a
   table: that is what placed four of the five code overlays above. (The
   fifth, `main.bin+0x21800`, was found the other way round -- from the
   engine's own load table at `0x80080C38`, which turned out to exist after
   all and is described in `docs/town-map.md` §1. It gives a destination and a
   length for four reads, and three of the five overlays are among them.)
   Either read it from the container's own table, or take a code entry's
   internal `jal` targets and find the base that makes them land on its own
   function starts.
3. **A splat spec per overlay**, with its own vram base. The "keep two
   overlays' identical addresses apart" worry turned out not to bite: each
   container links from its own generated script, and the
   script names its object files explicitly, so `build/town.bin.ld` pulls in
   `asm/ovl_town.o` and nothing else. town's and dungeon's overlays never meet,
   and neither reaches the `slus_006.14` link. Calls out of an overlay into the
   resident engine arrive as plain absolute symbol definitions in that
   container's `_undefined_funcs_auto.txt` -- addresses, not objects -- so
   there is no second source tree to build and no conflict to resolve.

   Three things have to be right in the segment definition, and getting any of
   them wrong is what parked the segments between 77b2c94 and eec0572:

   * **The rodata dot tracks whether a C sibling exists.** The dot routes a
     subsegment to a SIBLING C OBJECT, so `rodata` is right for an `asm`
     overlay and `.rodata` is required for a `c` one. Full rule and both
     failure messages under "The rodata dot" below.
   * **A following segment.** Without one the code subsegment runs to the end
     of the container; town.bin's became a single 4.3-million-line file.
   * **A text end on a 16-byte boundary.** splat emits
     `__romPos = ALIGN(__romPos, 16)` after a code segment, so a boundary off
     that grid pads the container. Find the end from the last `jr $ra`, not
     from where prologue fingerprinting stopped: all three of 77b2c94's figures
     were short of the real end (town 0x51ed8 against 0x51fc0, dungeon 0xf8000
     against 0xf7700, main 0x264c90 against 0x264e20).

   `make containers` rebuilds all four containers and checks each against its
   reference sha1.

Sizes, for scale: the heuristic in `docs/where-we-are.md` puts about 8.4 MB of
code-bearing blocks across the three containers against 216 KB in the resident
executable. Even discounting it heavily for packed data, the content dwarfs the
engine.

## The unit map for the card overlay

`tools/assertmap.py` turns the twin into a translation-unit map for the live
`0x80020000` overlay. It is the overlay replacement for `tools/tuscan.py`,
which proposes units from `$gp`-relative accesses and therefore has nothing to
read here: the overlays are `-G0`.

Three signals, in decreasing strength, and one structural constraint. The
constraint is tuscan's: the linker lays each object's text out contiguously,
so a unit must be a contiguous run of functions in address order.

1. **Assert sites.** 37 of them across the twin's 9 units, each naming its
   unit and its `__LINE__`.
2. **Own-rodata references.** Each unit's string literals sit in its own
   object's `.rodata`, and the linker emitted the units' rodata in the same
   order as their text, so a function reaching into unit X's rodata run was
   compiled in X. 33 functions carry this, and **none of them references more
   than one unit's rodata** -- the signal is perfectly clean here.
3. **Own-data references**, corroboration only. `.bss` here holds shared
   globals as well as per-unit statics, so the ranges overlap
   (`cardbk.c` reaches `0x805030EF`, `cardinfo.c` `0x80408EC8..0x8040B516`)
   and it cannot set a boundary. It is what argues the twin's first eight
   functions belong to `c_server.c`: they share `0x804094E8`/`0x804094EC` with
   functions inside that unit's run and have no rodata of their own.

In the twin all nine units come out **contiguous and in strict address order
with zero interleaving**, which is what link order looks like and is a check
the signals were not forced to pass.

### Propagating to the live block

The two builds are the same source in the same link order, so their function
sequences align. The tool runs an order-preserving global alignment over
masked instruction words -- `jal` targets and i-type immediates dropped, since
those differ between the links -- and carries each unit label across.
**184 of the live block's 241 functions pair.**

The live block is not a plain `NDEBUG` build. It keeps **24 assert sites, all
naming `c_server.c`, in two functions** (`open_card_events` at lines 185-226 and
`close_card_events` at 263-277), while its other eight units have none. So the
shipped build compiled `c_server.c` with asserts live and the rest without --
a per-file difference worth knowing before writing any of those files. Those
24 sites are a signal of the live block's own, and they **agree with the
propagated map everywhere the two overlap**, which is an independent check on
the alignment that the alignment could not have arranged.

But only the ASSERT signal transfers to the live block, not the own-rodata
one. Signal 2 needs one rodata anchor per unit to partition a block, and the
live block has exactly one, because its other eight units' assert strings were
never emitted. Running it there put every rodata address and every
rodata-referencing function in `c_server.c`, broke contiguity at five indices
and contradicted the propagation at `func_800258C8`. The twin is the only
block signal 2 can be applied to; `tools/assertmap.py` now says so in code.

The scoring matters and the obvious choice is wrong. difflib's symmetric
`ratio()` pairs only 171, and worse, it drops the anchors that matter: the
twin functions carrying the information are exactly the ones holding asserts,
and their live counterparts are missing a block per assert. A symmetric ratio
counts that insertion against both sides and **24 of the 33 anchored functions
failed to pair, leaving 9 live functions labelled and three units unplaced.**
Normalising matched length by the shorter body instead treats the extra blocks
as insertion: 16 anchors carry over and only `cardbk.c` needs bracketing.

Anchors that do not pair are still constrained, because the alignment is
monotonic: an unpaired twin anchor sits between the nearest paired twin
indices either side of it, so its live counterpart sits between their live
partners. That is what places `cardbk.c`, none of whose three anchors paired.

### The map

Live-block function indices (0-based, address order). "starts in" is the
interval the unit's own first function must lie in.

| unit | starts in | first anchor | last anchor | twin lines | anchors paired |
|---|---|---|---|---|---|
| `c_server.c` | [0, 11] | [7, 11] | [56, 58] | 180-267 | 4/14 |
| `card_mes.c` | [59, 65] | 65 | 65 | 665 | 1/1 |
| `card_opt.c` | [66, 99] | 99 | 107 | 337 | 3/3 |
| `card_win.c` | [108, 114] | 114 | 114 | 299-303 | 1/1 |
| `cardbk.c` | [115, 122] | [121, 122] | [122, 126] | 378 | 0/3 |
| `cardinfo.c` | [127, 134] | [132, 134] | [166, 173] | 416 | 1/3 |
| `cardslot.c` | [174, 202] | 202 | 204 | 310-339 | 3/3 |
| `cardsme.c` | [205, 215] | 215 | [227, 230] | 238 | 2/3 |
| `memory.c` | [231, 232] | [231, 232] | 232 | 371-387 | 1/2 |

**SUPERSEDED for this overlay as of 2026-11-10 -- see "Carving the nine"
below, which anchors all nine units directly off the twin's `__FILE__`
strings and contradicts three of this table's results.** The method stays
general; the numbers here do not.

The nine stay in order and do not overlap, which the alignment did not
enforce. Eight functions follow `memory.c` in the live block and 23 follow it
in the twin; in the twin those are PSY-Q library stubs (16-byte bodies at
16-byte spacing), not one of the nine units.

The anchored live functions themselves, which are the ones to start from
because their unit is known outright rather than bracketed:

| unit | anchored live functions |
|---|---|
| `c_server.c` | `open_card_events`, `close_card_events` (its own asserts), `poll_card_events`, `count_card_files`, `set_gsw_checksum`, `check_gsw_checksum` |
| `card_mes.c` | `open_card_message` |
| `card_opt.c` | `func_800238D4`, `func_80023C44`, `func_80023D64` |
| `card_win.c` | `func_80023FF0` |
| `cardinfo.c` | `func_800258C8` |
| `cardslot.c` | `func_80026ED0`, `func_80026FB4`, `func_80026FD4` |
| `cardsme.c` | `func_80027364`, `func_800277BC` |
| `memory.c` | `func_80027BC0` |

The twin's line maxima are lower bounds on each file's length, and
`card_mes.c` reaching line 665 while its only anchor is one function says most
of that file's functions are among the 33 unanchored indices before it. That
is an independent check on any proposed split.

Finally, the live block's own rodata partitions cleanly as far as the anchors
reach: `0x80020000`, `0x80020028`, `0x80020034`, `0x80020048` and `0x80020084`
belong to `c_server.c` and `0x800200A8` to `cardinfo.c`, none disputed. Six of
the block's ten rodata symbols is thin, but it is the mechanism a per-unit
rodata split will use, and it needs more anchored functions rather than a
different method.

### Carving the nine, 2026-11-10

The nine units are now nine files in `csrc/main/`, all four containers
byte-identical. The per-unit text sizes sum to `0x74FC` and the rodata to
`0x924`, which are the single object's figures exactly.

**The bracket table above is superseded where the two disagree.** It was
built by pairing twin bodies with difflib, and three of its results are
wrong: `card_mes.c` does not start in `[59, 65]` (it starts at 46),
`func_80023C44` and `func_80023D64` are `card_win.c`'s rather than
`card_opt.c`'s, and `cardbk.c` is not an eight-function range. The table is
kept because its METHOD is still the general one; the sections below are
what to use for this overlay.

#### The twin names all nine units directly, which beats pairing

Every one of the nine `__FILE__` strings is in the twin's rodata, in link
order: `0x8040002C` `c_server.c`, `0x80400570` `card_mes.c`, `0x804005C8`
`card_opt.c`, `0x80400620` `card_win.c`, `0x80400654` `cardbk.c`,
`0x80400760` `cardinfo.c`, `0x804007CC` `cardslot.c`, `0x80400800`
`cardsme.c`, `0x80400894` `memory.c`. A twin function that REFERENCES one of
them is in that unit -- no alignment, no scoring:

| unit | twin function | live index |
|---|---|---|
| `c_server.c` | `func_80400A10` | 8 |
| `card_mes.c` | `func_80402F84` | 65 |
| `card_opt.c` | `func_80403BC4` | [94, 101] |
| `card_win.c` | `func_80404364` | 114 |
| `cardbk.c` | `func_80404C84` | [120, 122] |
| `cardinfo.c` | `func_804057F0` | [128, 135] |
| `cardslot.c` | `func_804076C8` and two more | 202 |
| `cardsme.c` | `func_80407C10` | [210, 217] |
| `memory.c` | `func_804083FC`, `func_804084DC` | [230, 235] |

That is the first anchor `cardbk.c` has ever had. The live indices come from
the twin-to-live DRIFT PROFILE: over the 95 pairs `csrc/main_twin.h` records,
the drift is piecewise constant, so an unpaired twin function's live index is
bracketed by the nearest pair on each side. The profile also shows where the
twin has functions the retail build does not -- it jumps by FOURTEEN between
live 120 and 122, which is `cardbk.c`.

#### A duplicated helper means a separate file

Three helpers exist in SIX copies each, one per window-owning unit, and the
copies differ only in their constants:

| helper | live indices |
|---|---|
| root-pair initialiser | 53, 94, 103, 128, 195, 210 |
| `UNK_CARD_GROUP` init loop | 60, 96, 111, 130, 200, 212 |
| `func_800491F4` ordering wrapper | 59, 95, 110, 129, 199, 211 |

Two more families are BYTE-identical, which is stronger still: the centred-
rect helper at 92, 105 and 208, and the flag-check stepper at 137 and 172.
No translation unit defines the same function twice, so each family's members
are in distinct units -- six ordered clusters, `{53,59,60}` `{92,94,95,96}`
`{103,105,110,111}` `{128,129,130}` `{195,199,200}` `{208,210,211,212}`, one
per unit, which forces a boundary between each adjacent pair.

Matching the clusters against the twin anchors identifies them as
`card_mes`, `card_opt`, `card_win`, `cardinfo`, `cardslot`, `cardsme`. So
`cardbk.c` is the one unit of the seven with no window -- consistent with it
having a single live function.

The group struct is then a per-unit type, which makes every function that
names one an anchor. That is what settles `func_80023C44`/`func_80023D64`:
they reach the `0x240` group, the one `card_win.c`'s `func_80023FF0`
constructs.

#### The boundaries, and which are inferences

| unit | indices | top boundary |
|---|---|---|
| `c_server.c` | 0..45 | INFERRED in (45, 53]: `card_mes.c` owns 53, and 46..52 are all message-window helpers while 43..45 are all card-file operations |
| `card_mes.c` | 46..87 | DETERMINED: 87 reaches the `0x8B0` group, 88 the `0x184` |
| `card_opt.c` | 88..102 | DETERMINED: 101 reaches the `0x184` group, 102 writes that window's head fields, 103 is `card_win.c`'s root-pair copy |
| `card_win.c` | 103..121 | DETERMINED: 121 reaches the `0x240` group, 122 is `cardbk.c`'s |
| `cardbk.c` | 122 | DETERMINED: its twin anchor maps to [120, 122]; `card_win.c` owns 120 and 121, `cardinfo.c` owns 123 |
| `cardinfo.c` | 123..171 | INFERRED in (164, 172]: its data run holds the three objects 167, 168 and 171 use, and 172 is the flag-check stepper's second copy |
| `cardslot.c` | 172..205 | DETERMINED: 205 wraps `cardslot.c`'s `func_80026B18`, 206 reaches the `0x0D4` group |
| `cardsme.c` | 206..227 | INFERRED in (227, 230]: 227 uses `D_8002809C`, and 228..231 all work on `memory.c`'s `D_8002BB08` payload |
| `memory.c` | 228..240 | end of block |

Five determined, four inferred inside narrow windows. Any order-respecting
cut builds byte-identical, so an inferred boundary is a claim about the
original source, not about the output.

#### Rodata is per unit; so is data, and it is nearly all determined

The rodata partitions exactly, in link order, and only four units have any:

| unit | rodata |
|---|---|
| `c_server.c` | `0x80020000..0x800200A0` -- the assert pair, the two handler tables, `MEM_CARD0/1`, the six name strings |
| `card_mes.c` | `0x800200A0`, four bytes (`00 01 02 03`) |
| `cardbk.c` | `0x800200A4`, the one-character string (`" "`) |
| `cardinfo.c` | `0x800200A8..0x800200C0` -- `func_800258C8`'s own four-entry switch table, whose entries are addresses inside cardinfo's OWN text, then `SAVE_ICON_VRAM_RECT` |
| `memory.c` | `0x800200C0..0x80020924` -- the 59 message strings, flushed out of the eight tables' initialisers; this is the INFERRED boundary, the alternative being `cardsme.c` |

The switch table is the hard anchor here: a compiler-generated jump table
goes in the object holding the switch, and its four entries all land in
`0x80024390..0x800261C0`, cardinfo's text. So the pool is owned by a unit at
or after `cardinfo.c`, and no earlier.

**The data region is per unit in two groups** -- `0x80027E20..0x80028530`
tables, `0x80028530..0x8002BB6C` zero state -- which is just the ordinary
data-then-bss shape, so the partition exists whatever we know of it. What we
know is most of it.

**The method, and its one trap.** A data object belongs to the unit that
DEFINES it, not the one that uses it, so the referencing unit is a hint and
not a proof. What turns the hint into evidence is that link order forces a
unit's objects to be CONTIGUOUS and in unit order: if ownership tracked
usage, the referencing units read down the address list would come out
non-decreasing, and if it did not, they would be scrambled. They come out
non-decreasing almost everywhere, and the places they do not are exactly the
places with an independent anchor.

**The state run is fully determined. 27 objects with a single referencing
unit, zero order violations:**

| unit | state block | objects |
|---|---|---|
| `c_server.c` | `0x80028530..0x800287E8` | 19, incl. the 8 event vars, `card_dir`, `card_st`, `card_slot`, `card_evflg`, `gCardState` |
| `card_mes.c` | `0x800287E8..0x800291D0` | 3 |
| `card_opt.c` | `0x800291D0..0x80029618` | 1 |
| `card_win.c` | `0x80029618..0x8002A990` | 2 |
| `cardbk.c` | -- | none |
| `cardinfo.c` | `0x8002A990..0x8002B850` | 2 |
| `cardslot.c` | `0x8002B850..0x8002B968` | 2 |
| `cardsme.c` | `0x8002B968..0x8002BB08` | 1 |
| `memory.c` | `0x8002BB08..0x8002BB6C` | 1 |

A non-decreasing run of 27 over nine units is about 1 in 1e19 by chance, so
this is not an alignment that happened to work. `cardbk.c` owning nothing is
the expected answer, not a hole: it is the one unit with no window, and it
has a single live function.

**And the partition predicts the two residue words, which is independent
confirmation.** `docs/owning-data.md` SS6 records `0xF97F8BEF` at
`0x8002A98C` and `0x6F7C7327` at `0x8002B964` as high-entropy words sitting
inside the zero region, read by nothing, each just past the end of a buffer
whose true length is witnessed by its `func_8003FE78` word count. Under the
table above they are the gaps between unit blocks: every unit's state block
begins on an 8-byte boundary, exactly two interior block ends are not
8-aligned -- card_win's at `0x8002A98C` and cardslot's at `0x8002B964` -- and
both carry a residue word, while all five 8-aligned interior boundaries carry
none. Two for two with no exceptions. So the residue is inter-unit `.bss`
alignment padding, written out of the linker's buffer rather than zero-filled.
(memory's end is misaligned too and has no residue, correctly: nothing
follows it but the 0x494-byte pad to the sector boundary.)

**The tables run is determined except one cut.** It falls in three zones:

| zone | span | status |
|---|---|---|
| head | `0x80027E20..0x80027F60` | `c_server.c`, DETERMINED -- `MEM_CARD_NAMES` and `MEM_CARD_FILENAMES` point at literals inside c_server's own rodata block, a literal is emitted into the unit that uses it, and no unit precedes unit 1; the rest follows by contiguity |
| middle | `0x80027F60..0x800280B4` | DETERMINED -- 18 objects, zero violations: card_mes 1, card_opt 1, card_win 2, cardbk 1, cardinfo 5, cardslot 6, cardsme 2 |
| tail | `0x800280B4..0x80028530` | ONE OPEN CUT between `cardsme.c` and `memory.c` |

The head is also where the defines-is-not-uses trap shows itself, and it is
worth keeping in mind because it is the whole reason this needs an argument:
`D_80027E68` is the second object in the run and only `cardslot.c` reads it,
`SAVE_ICON_CLUTS` is third and only `cardinfo.c` reads it. Both are
c_server's, used by units six and seven away.

The tail is the eight message-pool tables plus the sixteen animation payloads
above them. It is `cardsme.c`'s or `memory.c`'s: the middle zone ends at
cardsme, and the pool is the segment's last rodata. `gCardMessages` and
`D_800282A4` share `MSG_EMPTY`, of which the pool holds one copy, so those two
and everything between them are ONE unit.

**It is placed in `memory.c`, and that is an inference.** Parsimony picks it:
the cut at `0x800280B4` is the only position that needs no second choice,
since cardsme owning part of the tail leaves "how much" open. Three things
were tried against it and gave nothing:

* **code references into the pool.** Pairing every `lui`/`%lo` in the
  overlay's 7487 words of text against the 59 string starts finds ZERO.
  The pool is reached only through the tables' data relocations, so the
  own-rodata signal that settled the text boundaries is silent here.
* **function pointers in the tail.** The only ones in the whole tables run
  are `D_8002809C`'s five, which point into cardsme's text -- and
  `D_8002809C` is already determined cardsme's, below the tail.
* **the twin.** It puts this text in five other units entirely; see below.

The one counter-signal is that `D_800282CC` is read by `cardsme.c` while
nothing in the tail is read by `memory.c` at all. It is weak: this overlay has
two PROVEN cross-unit data uses already, both in c_server's block. And the
twin's `cardsme.c` strings are debug-only ASCII (`ERROR`, `NOW CHECK`,
`CARD1 %s`) that retail does not carry at all, so "cardsme already holds
text" does not transfer either. If better evidence turns up, the block moves
to `cardsme.c` mechanically and the build will not notice.

**Correction, 2026-11-10.** An earlier pass over this section replaced the
claim "both runs check out object by object" with the opposite, that the
referencing map "jumps around link order freely" and seven of the eight cuts
would be invention. That was an over-correction and it was wrong. The map
jumps only in c_server's head block and inside the tail; across the 45
objects in between it does not jump once. The original claim was right, and
what it was missing was this argument rather than evidence.

#### The data IS carved, and a unit can only own the FIRST data run

Done 2026-11-10. `csrc/ovl_main_80020000.c` went from 966 lines to 168, and
every unit now carries its own data:

| where | what | lines |
|---|---|---|
| the nine `csrc/main/*.c` | each unit's `.data` block, appended at the END of its file | 24 objects |
| `csrc/main/memory.c` | the 59 messages and the eight tables that point at them, `0x800280B4..0x80028530` -- the one INFERRED data boundary | 18 objects |
| `csrc/ovl_main_80020000.c` | the state run `0x80028530..0x8002BB6C`, the overlay's `.bss` which cannot be `.bss` | 30 objects |

**The rule that decides the split, and it is forced, not chosen.** Every
explicit `.data` subsegment sits at a rom address above all of the segment's
text, so the nine auto-emitted `main/*.o(.data)` lines always come FIRST in
the DATA group, ahead of any data file's. The image has two runs per unit, so
the units take the first run -- the tables -- and the second has to live in
one object after them. The reverse is not available: there is no way to get a
data file's `.data` ahead of the units'.

**Why nothing further is carved.** Two reasons, neither of them the linker:

1. **One object contributes one `.data` run**, so both runs per unit needs two
   objects per unit. That builds -- measured by moving `D_8002BB08` into a
   `csrc/main/memory_state.c` with one yaml line, four containers
   byte-identical -- but it invents a second translation unit per real one,
   and the original had nine.
2. **Real `.bss` is out**, which would otherwise let a unit hold both runs,
   for the reason `docs/owning-data.md` SS6 gives first: the run holds the two
   non-zero residue words, and a NOBITS section would write zeros over both.

An earlier version of this section said the build could not express per-unit
data at all, and blamed one DATA group per segment with `.bss` ahead of
`.text`. Both halves were wrong. Legacy generation emits one input-section
line per subsegment IN SUBSEGMENT ORDER, so the DATA group takes as many
objects as the yaml lists, in the order it lists them; and the group order is
`section_order`, a yaml option settable per segment, which splat's source
notes it is *legacy* generation that ignores.

**The carve is self-verifying in one place that matters.** Moving the message
tables into `memory.c` moves the 59 string literals with them, because cc1psx
emits a literal into the unit whose initialiser uses it. So the 0x864-byte
pool at `0x800200C0..0x80020924` is now produced by gcc's deferred-constant
flush over `memory.c`'s eight initialisers rather than transcribed, and it
comes out byte-exact -- every string at its ROM address, with the empty string
deduplicated across all 60 slots that hold it.

#### The twin's UI text is per unit -- and retail reorganised it

Dumped 2026-11-10, and it settles what the twin can and cannot do for data.
Carving the twin's rodata at the nine `__FILE__` anchors (each unit's block
opens with its own copy of the assert format string, so a block starts 0x28
bytes before its filename) shows the twin is the JAPANESE build, with its UI
text distributed across five units:

| twin unit | its Shift-JIS strings | the retail table they become |
|---|---|---|
| `c_server.c` | 29 messages, `0x80400148..0x80400545` -- データの消去に失敗しました。, 空きブロックが足りません。, メモリーカードが壊れています。 ... | `gCardMessages` |
| `card_mes.c` | リプレイ / 消す / コピー / ロード | `D_800282BC` = REPLAY/DELETE/COPY/LOAD |
| `card_opt.c` | 使用できません。/ 回 / 冒険中 | `D_800282A4` = `MSG_CANT_USE` |
| `cardbk.c` | 名前 / 貯金 / 冒険 / 回 / 時間 / 建物 / 不思議な塔 / 魔物の塔 / Ｆから冒険再開 / メモリーカードに他のゲームのデータがあります。 / あと ... つデータがセーブできます。 | `D_800283A8`, `D_800283B8`, `D_800283EC`, `D_800283F4` |
| `cardinfo.c` | アザーライフ・アザードリームス　ＮＯ． | `D_80028294` = `MSG_AZURE_DREAMS`/`D`/`T`/`COLON` |

Two useful things and one dead end come out of it.

**`cardbk.c` is the saved-game summary panel.** 名前 (name), 貯金 (savings),
冒険 ... 回 (adventures, count), 時間 (time), 建物 (buildings), plus the two
tower names and "resume the adventure from F". That is the per-slot summary
box, and it is the first statement of what this unit is FOR rather than where
it sits. It also explains the retail shape: fourteen of its functions are
compiled out and one survives, because the summary panel is drawn from data
the retail build fills in elsewhere.

**It also gives the Japanese original of most of the English messages**, which
is better provenance for the `MSG_*` macro names in
`csrc/ovl_main_80020000.c` than the English text alone -- 消す is "delete"
flatly, so `MSG_DELETE` is right, and 冒険中 ("adventuring") is not a UI
label at all.

**The dead end: the twin cannot settle retail's data ownership.** In the twin
these tables live in units 1, 2, 3, 5 and 6. In retail they cannot: the pool
is one contiguous block after cardinfo's rodata, and the retail rodata of
units 1, 2, 3 and 5 is fully accounted for by other things (the assert pair
and handler tables, four bytes, nothing, one space). So the retail source
CENTRALISED the UI text into a single unit, and the twin predates that
change. Unlike the text boundaries -- where the twin's filenames were
decisive -- here it is evidence that the two builds differ, which is why the
tail cut above stays open.

#### Three things the carve needs that are not the source split

* `build/csrc/main/%.o` needs its own `-G0` pair. The `ovl_%` pattern does
  not match it, and the Makefile comment about carving `ovl_town` is the same
  trap one directory over.
* it also needs `-Icsrc`, because `#include "main.h"` resolves against
  `csrc/main/` first.
* `build/main.bin` does not depend on `build/main.bin.ld`, so a yaml change
  needs `splat split splat/main.bin.yaml` run by hand. `make` alone links
  against the stale script and reports undefined symbols that are already
  fixed.

### Carving the twin, 2026-11-13

The twin is now **ten files in `csrc/main_twin/`** plus a tail, all four
containers byte-identical. Per-unit `.text` sums to `0x8140` and `.rodata` to
`0x8A0`, which are the single unit's figures exactly.

Three independent things fix the boundaries, and they agree:

1. **The nine `__FILE__` strings.** Each original unit carries its own
   `ASSERT_FMT` copy followed by its own filename, and all nine are in the
   twin's rodata in link order. So **each unit's rodata slice STARTS at its
   assert pair** -- that partitions the 0x8A0 window with no inference at all,
   and `csrc/ovl_main_80400000_rodata.c`'s group comments had already marked
   the runs. The windows are self-checking: a wrong cut makes a unit's
   `.rodata` the wrong size and the container the wrong length.
2. **The twin-to-live pair map** in `csrc/main_twin.h`. A twin function whose
   counterpart is recorded inherits that counterpart's unit, read off the live
   block's own yaml ranges. 149 of the 263 are tagged that way, and the nine
   runs come out contiguous and in strict address order with no violations.
3. **The live block's own boundaries**, already carved in `csrc/main/`.

| unit | text from | boundary above it |
|---|---|---|
| `c_server.c` a | `0x804008A0` | -- |
| `c_server.c` b | `0x804017EC` | NOT a source boundary: see below |
| `card_mes.c` | `0x80402508` | INFERRED in `(0x804022C8, 0x80402508]`: four functions, none naming a `__FILE__` string or touching either unit's rodata; one of them is live `erase_tmp_file`'s twin |
| `card_opt.c` | `0x804034D8` | DETERMINED: paired on both sides |
| `card_win.c` | `0x80403DA0` | DETERMINED |
| `cardbk.c` | `0x804046C8` | INFERRED, but tightly: `func_80404C84` names `cardbk.c`, and the drift profile jumps by FOURTEEN here, which is exactly the count if cardbk starts at `0x804046C8` -- 15 twin functions for its 1 live one |
| `cardinfo.c` | `0x80404EA0` | DETERMINED |
| `cardslot.c` | `0x804069FC` | DETERMINED |
| `cardsme.c` | `0x8040789C` | DETERMINED |
| `memory.c` | `0x804081AC` | INFERRED in `(0x80407F68, 0x804081AC]`: `func_804080A4` reaches cardsme's rodata slice, so cardsme keeps it |
| tail (library) | `0x8040855C` | the PSY-Q objects, see below |

Six determined, three inferred inside narrow windows, same as the live block.
Any order-respecting cut builds byte-identical, so an inferred boundary is a
claim about the original source and not about the output.

#### `c_server.c` is TWO units, and that is the toolchain, not the source

Its rodata is `[7 objects][jtbl_8040007C][0x4B8 more]` with the table in the
MIDDLE. At -G8 cc1psx appends the text stream after all file-scope data
(`mips.c:4181`), so a compiler-emitted jump table always lands LAST in its
unit, and one unit cannot produce that order -- `docs/owning-data.md` §9.1.
Splitting at `func_804016D0`, the function whose switch emits the table, is
the same move `ovl_town` makes at `func_800BEBA4`. It is NOT a claim that the
original had ten units.

#### What the carve needs that the source split does not

- **Makefile rules.** `build/csrc/main_twin/%.o` needs every override the
  parent unit had: `CFLAGS_DEFAULT` (-G8), `tools/psx272gcc`, `--expand-div`,
  and `tools/decomp/jtbl_align.py`. It matches neither `ovl_%` nor `main/%`,
  both of which are -G0. This is the trap the `ovl_%` comment describes.
- **An explicit `section(".rodata")` on every rodata object.** The old
  rodata unit was -G0, where this never arises. At -G8 a const object of 8
  bytes or less goes to `.sdata`, which no subsegment collects, and the link
  fails with *defined in discarded section `.sdata`*. 104 objects needed it.
- **Cross-unit declarations.** Nothing in the block is `static`, but 42
  functions were only visible because everything shared a file. They are
  declared at the head of the unit that needs them, not added to
  `main_twin.h`, because they are not part of the block's interface.
- **`INCLUDE_ASM` paths** move with the function, to
  `asm/nonmatchings/main_twin/<unit>`.

#### Two things the carve turned up

- **`guard.py` only globbed one directory level**, `csrc/*.c` and
  `asm/nonmatchings/*/*.s`, so it had never seen `csrc/main/` either. Fixed to
  recurse. `parkstats.py` and `nmcheck.py` matched a unit name as `(\w+)`,
  which a unit with a slash in it fails -- latent since `main/c_server`.
- **The tail is not `memory.c`.** `main_twin.h` pairs `func_804086CC` and
  `func_80408730` with live `func_80027C60` and `func_80027D7C`, which sit
  inside `csrc/main/memory.c` -- but `docs/twin-block-libcard.md` identifies
  the first as `libcard/A80 _new_card`. Both can be true only if the LIVE
  overlay also links libcard at its tail and the live carve swept it into
  `memory.c`. That is a question about `csrc/main/memory.c`, not the twin, and
  it is open.

#### Still to do

The `.data` window, `0x804089E0..0x8040C628`, is still owned whole by the tail
unit. Its per-unit partition is known -- two runs, tables then state, each in
unit order, which is the shape §"The data IS carved" describes for the live
block, and the state run is a clean non-decreasing sweep across all nine with
zero order violations. Applying it moves no bytes, so byte-identity cannot
check it; it is a claim about the original and wants the same care the live
block's partition got.

### What not to use the twin for

* **Its external `jal` targets are not retail engine addresses.** It calls
  printf at `0x8007C040` and exit at `0x8007BEF0`; in the shipped executable
  those land inside libgte's `rcossin_tbl`. It was linked against a different
  engine build.
* **It is not itself the goal.** It is dead code on retail. The live
  `0x80020000` overlay is what gets decompiled; the twin exists to say what
  the units are.

## Building an overlay as C: the flags

### The overlays are -G0, and that is forced, not chosen

The resident executable is built at cc1psx `-G8`. The overlays are `-G0`.

Two reasons to believe it before measuring anything. There are **0
`$gp`-relative references across ~200K instructions** of split overlay code.
And there could not have been any: `$gp` is `0x80080994` and GPREL16 reaches
+/-32K from it, i.e. `0x80078994..0x80088994`, while this overlay keeps its
data at `0x80027E20..0x8002BB6C` and town's and dungeon's live at
`0x80088760` and above. psylink would have rejected a GPREL16 in any of them.

What `-G` actually changes here has nothing to do with `$gp` in the end. At
`-G8` cc1psx records `.extern SYM,4` for a 4-byte global and emits a bare
symbol reference, which the assembler expands into a `lui $at` / `%lo($at)`
pair **per use**. At `-G0` cc1psx emits its own `%hi`/`%lo` split address into
a general register, which can be hoisted and shared -- the hoisted-lui shape
this overlay uses everywhere. On `set_card_slot` that is the difference
between 26 words and 24.

A consequence worth stating, because it retires one of this project's main
declaration-level levers for all overlay code. The symbolic-vs-split choice
documented in `docs/addressing-and-translation-units.md` is a test of a
global's declared size against `-G`: at `-G8` a scalar or an array of <= 8
bytes gets the symbolic form and anything larger gets the split. At `-G0` no
declared size is ever `<= G`, so **every** global access takes the split form
whatever the declaration says. Spelling a global `extern T X[];` instead of as
a scalar cannot change the addressing shape in an overlay. The levers that do
still work are the ones about WHICH register the address lands in and WHERE
the `high` is emitted -- which is what landed `set_card_slot`.

### One source reproduces both builds

The same C source compiles to the live block at `-G0` and to the **debug twin
at `-G8`**, both with 0 differing words:

| block | flags | words | shape |
|---|---|---|---|
| live `set_card_slot` | `-O2 -G0` | 24 | `lui $v1` hoisted, shared by both stores |
| twin `func_804008A0` | `-O2 -G8` | 26 | `lui $at` re-expanded at each of three uses |

So the twin is not a different compiler and not only an asserts-on build: it
is the same source at a different `-G`, with `c_server.c`'s asserts kept in
both and the other eight units' kept only in the twin. **That gives every
function in this overlay two independent targets from one source.** A shape
that matches one build and not the other is wrong, and the two disagree about
exactly the things `-G` controls, which is where global-access mistakes live.

(`tools/decomp/ovlverify.py` is verify.py's job for an overlay function, whose
target has to come out of the one big listing rather than
`asm/nonmatchings/<unit>/<func>.s`. It takes `--G` so both channels can be
checked, and masks the fields an unlinked candidate object cannot have filled
in -- `jal` targets and `%hi`/`%lo` -- the way advise.py's aligned verdict
does.)

### The rodata dot

An overlay still split as `asm` must say `rodata` and **not** `.rodata`,
because the dot "routes a subsegment to a sibling C object and extracts
nothing": with no `csrc/ovl_<name>.c` to route to, nothing is emitted and every
reference into the overlay's own rodata comes out undefined, which is all
`undefined reference to jtbl_80089900` ever was. Converting an overlay to `c`
makes splat refuse the undotted form outright: *"The `rodata` section was not
prefixed with a dot, which is required for the rodata migration feature to
work properly and avoid build errors due to duplicated symbols at link-time."*

Both are the same rule from opposite sides: the dot is wrong when there is no
sibling C unit and required when there is. So the conversion from `asm` to `c`
adds the dot in the same change that creates the C file, and an overlay left at
`asm` must not carry it.

With the dot, splat migrates each rodata symbol into the nonmatching listing
of the single function that uses it, which is what puts `jtbl_80020060` where
it belongs without any manual placement.

### Per-unit flags in the Makefile

    CFLAGS_G0 := -O2 -G0 -Iinclude -Ipsyq/include
    MASPSXFLAGS_G0 := ... --use-comm-section -G0
    build/csrc/ovl_main_80020000.o: CFLAGS := $(CFLAGS_G0)
    build/csrc/ovl_main_80020000.o: MASPSXFLAGS := $(MASPSXFLAGS_G0)

maspsx gets `-G0` as well, so that it makes no small-data decisions of its own
on a unit that has none. Note this is a `CFLAGS` override, which the Makefile
had no mechanism for before -- only `MASPSXFLAGS` was overridable per unit.

### set_card_slot

24 words, the first function in the overlay. Its C is in
`csrc/ovl_main_80020000.c` with the reasoning in the comment; the lever is
`docs/matching-levers.md:3173`, **spent at the two stores and not at the
read**.

Storing straight to the global puts each `%hi` beside its own store, so the
`else` branch materialises a second `lui` and the function comes out 25 words.
Assigning the symbol to a pointer local after the three calls moves the `%hi`
to the assignment, ahead of the branch, where both stores share `$v1` and
reorg pulls it into the `beqz` delay slot. The trailing read stays a bare
symbol so it keeps its own re-materialised `lui $v0`, which is what the
listing has. The local costs no instruction -- combine folds the `%lo` back
into the stores.

Worth knowing when a sweep starts: splat emits a `jr $ra; nop` function as
real C rather than `INCLUDE_ASM`, so the trivial ones are free -- here
`print_card_status` and `func_8002519C`.

## Sweeping an overlay

`tools/decomp/ovlverify.py` is the fast loop; the container build
(`make build/main.bin`, `make build/town.bin`, ...) is the gate.

### The sweep produces the anchors that pin the carve, not the other way round

The card overlay's plan said to carve the file into its nine units first. That
was the wrong order, and the rodata is what shows it.

A unit emits one contiguous `.rodata`, so the 11 rodata labels in
`0x80020000..0x80020924` must partition across the units in text order. They
do, and it is a real check: listing each label with the index of the first
function that references it gives **8, 8, 22, 22, 23, 27, 54, 123, 156, 170**
-- strictly non-decreasing in rodata address, which a wrong unit order would
break. Two functions sharing a label are in the same unit, so idx 54 and 57
(both reaching `0x800200A0`) merge, which extends `c_server.c` past its last
alignment-bracketed anchor and pins its end to **idx 57 or 58**, one index of
slack.

That is the ONLY boundary anywhere near pinned. Carving nine units needs nine
exact text boundaries and a three-way split of the rodata -- `c_server.c` to
`0x800200A4`, `cardbk.c` the four bytes at `0x800200A4`, `cardinfo.c` the rest
including the 2156-byte Shift-JIS message pool at `0x800200B8`, which is
full-width Shift-JIS card-UI text ("failed to delete data!", "Do you wish to
delete", "Cannot overwrite this data!"). Those attributions follow from the
ordering, but the text boundaries do not.

So the dependency runs the other way: **the sweep produces the anchors that
pin the carve.** And at `-G0` the carve buys much less than it did for the
engine -- the declared-size addressing lever does not exist there (see above),
so unit membership no longer changes the shape of a global access. One
combined file is also safe for rodata ORDER, because first-use order within
it equals the target's rodata order as long as function order is preserved,
which splat already does. What a carve is still needed for is a faithful
source tree and per-unit rodata placement, and the risk it carries is
colliding `static`s -- nothing seen so far.

### Where the yield was

Families. Grouping the stubs by masked instruction signature (`jal` targets
and i-type immediates dropped) found 15 families among the 177 stubs, and
three of them were most of a day's progress:

* **20 one-line message wrappers** (`msg_format_confirm..msg_delete_failed`,
  10 words each, `0x28` apart) around `card_mes.c`'s `open_card_message(a, id,
  kind, p)`, one per message id 0..19. Decoded mechanically from which
  argument register each wrapper writes; the register copies (`addu $a2, $a1,
  $zero` where kind equals id, `addu $a3, $a2, $zero` where both are 0) are
  gcc reusing an equal value, not source. All 20 landed from one generator.
* **5 three-pointer primitive initialisers** (37 words each).
* **13 small accessor/setter/wrapper groups** of 2-20 words.

### Four levers this overlay needed

1. **The pointer-local hoist** (`docs/matching-levers.md:3173`), per site.
   `set_card_slot` and `on_card_timeout` both store to a global on one branch
   and read or store it on another; through the symbol each `%hi` is emitted
   beside its own store and the second branch materialises a second `lui`,
   one word long. Through a local assigned just before the branch the `%hi`
   goes ahead of it, both arms share the register, and reorg pulls it into the
   branch delay slot.
2. **The array-spelling aliasing lever** (`include/structs.h`'s
   `extern u32 D_800814B0[1]`, `docs/matching-levers.md` section 5), and this
   unit needs **both spellings of one symbol**. `func_80026FB4` updates
   `D_800814A0` in place beside a `u16` store and the target INTERLEAVES the
   two, which the scalar form permits. `func_80023A00` does the same two
   updates and the target does NOT interleave -- 20 words with a load-delay
   nop against 19 interleaved -- so it needs the array form, where both sides
   are in-struct, cse's `invalidate` drops the cached load and sched1 cannot
   hoist. `extern u32 D_800814A0_a[1] __asm__("D_800814A0");` gives the array
   spelling without touching the shared declaration every engine unit uses.
   Neither a cast-to-pointer index nor a pointer local reaches it: both stay
   at 19 words.
3. **A struct copy is not a word copy.** `*a = *(UnkCardPrim *)D_8002E5D8`
   compiles to gcc's block move -- three loads then three stores off a
   materialised base -- while the target has four independent `lw`/`sw` pairs
   with load-delay nops. Four separate word assignments through a `u32 *` are
   what match. The templates stay `extern u32 []` as `include/structs.h`
   declares them; retyping them would reach every engine unit.
4. **Where a loop-invariant address is computed decides its register and its
   schedule.** In the 37-word family the second template's base must be a
   local assigned before the copies (`u32 *s2 = D_8002E5E8;`): then its live
   range overlaps the first copy, it takes `$a3` instead of reusing `$v0`, and
   sched1 fills the first copy's load-delay nops with it -- 37 words against
   39 with the nops left in place.

And two shapes worth knowing for the next sweep: a downcounted `do`/`while`
wants its increment in the CONDITION (`} while (++i < 10);` is 9 words,
`i++;` in the body is 10 -- the body form hoists the increment above the load
and pays a load-delay nop), and two initialisers in a row want the cheaper one
first, since `s32 sum = 0;` ahead of `s32 *q = gsw + 0x82;` is the whole
difference between 0 and 2 differing words in `set_gsw_checksum`.

## Levers overlay code keeps needing

These are not overlay-specific -- they are ordinary gcc 2.7.2 behaviour that
the engine's own functions happened not to exercise much -- but each cost two
or more iterations in the card overlay, so they are collected here. The count
in brackets on each is how many functions in `csrc/ovl_main_80020000.c` it
landed, i.e. how much weight it has carried.

**1. Put the table base in a local before indexing it.** (5 functions)
`&TBL[i]` written directly lets gcc fold a constant field offset into the
symbol's `%lo` and lets it compute `i * stride` before materialising the base;
assigning the table to a local first forces the base out as its own quantity,
materialised first. Symptoms, all one or two words:

* `gCardMessages[p[2]].unk_15` with the base inline gives `lbu $v0, 0($v0)`
  where the target has `lbu $v0, 0x15($v0)` -- the 0x15 went into the
  relocation. With `UnkStrSet *t = gCardMessages; r = &t[p[2]];` it matches
  (func_80022488, func_80022CD8).
* `D_80083E98[idx]` inline emits the shift before the `lui`/`addiu`; through a
  local it emits the base first and fills the load-delay slot with the
  `addiu` (load_save_slot_info, erase_save_file, func_800254E4).
* The same thing one step further on: `row = tbl` as a separate copy makes the
  materialised address and the walking pointer two quantities, which is the
  extra `addu` the target has (card_file_exists). See matching-levers.md's
  `p += x` is one long quantity note.

**2. A value used only after a call must not be computed before it.**
(3 functions) Written above the call it is live across it, takes a
callee-saved register, and costs that register's save and restore -- two
words, sometimes three. func_800251A4's 1-or-3 flag (37 words against 35),
format_card's table pointer, func_80023E14's table base. The fix is always
to move the assignment below the last call that precedes its first use.

**3. Write the store into both arms of an if/else and let jump.c cross-jump
them.** (3 functions) One store of a local after the join keeps the local
live across the arms' calls; a store in each arm is merged by cross-jumping,
usually into the following call's delay slot. func_80025230, func_8002570C,
func_80025848.

**4. Declaration order decides which callee-saved register each local gets,
and the order the setup is emitted in.** (6 functions) The target's first
instructions are the cheapest way to read this off: whichever local is
initialised first in the listing is declared first. set_gsw_checksum (`sum`
before the cursor, 2 DIFF), func_80023194 and func_800239B0 (counter before
cursor, plus a local COPY of the parameter, 6 DIFF), count_card_files,
func_80025E54, card_file_exists.

**5. `x++` is not `x = 1` even when x is provably 0.** (1 function, but it
unblocked three other things) card_file_exists's mismatch flag increments; with
the increment form gcc stops hoisting the flag's `= 0` out of the enclosing
loop, which is what made the rest of that function's allocation fall into
place.

**6. Index a cursor by an integer offset, not by walking a pointer, when the
displacement is large.** (2 functions) `q = p; ... q[0x98] ...; q++` lets gcc
fold the 0x98 into the base (`addiu $s0, $s2, 0x260`, displacement 0); an
integer `off` with `p[off + 0x98]` keeps the displacement where the target has
it (func_800240EC, func_80025DC8).

**7. A trailing correction in the listing can be the compiler's, not the
source's.** count_card_files increments its count before the loop, again in the
loop's delay slot, and decrements after -- which reads like a source `n--` and
is loop rotation of `do { n++; ... } while (...)`. Writing the `n--` makes it
40 words / 9 DIFF; leaving it out, 39 / 0.

**8. Branch LAYOUT order is part of the source.** Which arm the compiler
places first follows the source's test sense. func_800254E4 is one word short
with `if (x != 0) A else B` and exact with `if (x == 0) B else A`, because the
target reaches A by a taken `bnez` rather than by falling through.

## Findings from the card overlay's second sweep

The pass that finished the card overlay landed 82 functions at once.

The last function to fall, `func_80022774`, took four passes and about a
thousand compiled variants. Its park spent three of those passes believing a
false obstruction, and the note on it is worth keeping: a reference marker is
the LAST CONSUMER of the insn producing its operand, so under a backward
scheduler the clock the marker is picked at is the clock that load launches.
Two jobs were competing for one scheduling hole, which is not a thing the
listing shows. The fix was a device that consumes nothing -- a hard-register
pin -- rather than a cleverer marker.

Its last stretch -- seven functions, no permuter -- was worked a different
way, and the way generalises: open the compiler's per-pass RTL dumps
(`cpppsx` then `cc1psx -O2 -G0 -da`, dumps under
`~/.cache/azure-psyq/.../Temp/`) and find the pass that makes the decision,
rather than searching spellings. In all seven, the park's MEASUREMENT TABLE
was sound and the park's STATED CAUSE was wrong -- "reload artifact" and
"scheduling" being the two attributions that misled most often. A park's
numbers are fact; its diagnosis is a guess that has been wrong seven times
running.

Two findings did most of the work, and both are worth knowing before touching
another overlay.

### `func_80069DF8` is `noreturn`, and that is what unlocks the asserts

The assert sites the translation-unit map used to identify `c_server.c` back
in Phase 1 are a bracketed pair of calls:

```c
if (!cond)
{
    func_80069F18(D_80020000, D_80020028, __LINE__);
    func_80069DF8(1);
}
```

Declared ordinarily, gcc assumes the body falls through into whatever follows,
so after the two calls it re-materialises the next global's `%hi` -- one
wasted word per assert site. Declare the abort as

```c
void func_80069DF8(s32 code) __attribute__((noreturn));
```

and the `lui` that reorg puts in each test's `bnez` delay slot is the one the
branch target's load uses. close_card_events went from 121 words against 114
with nothing else differing, to exact; open_card_events, the overlay's second
largest function at 264 words, then came out zero-differing on the FIRST
spelling -- eight `OpenEvent` calls inside the `EnterCriticalSection` pair,
each asserting the handle is not -1, then eight `EnableEvent` calls each
asserting a non-zero result. All sixteen `%hi` values stay live in their own
callee-saved register across the calls, which is why that function saves
`$s0..$s7` and `$fp`.

`town.bin` has 76 assert records against this overlay's 10, including 54 in
`main.c`; the lever should pay there many times over.

### Decode one of a family and the rest are nearly free

Five of the functions landed here are the same input handler at different
state offsets: func_800265B8, func_80022524, func_80025584, func_8002789C and
func_8002593C, 527 words between them. The first took a dozen spellings; the
other four took one or two each, because three levers transfer whole:

* the repeat counter's `>= N` arm comes FIRST, with the plain increment as the
  `else`, since the target branches forward into the increment;
* the two exit tests are separate `if`s -- as `r == 0 || r == 1` gcc folds
  them into `sltiu r, 2`;
* each direction assignment is written out in BOTH arms of its 0x8000/0x2000
  pair so jump.c cross-jumps them into one tail.

Where they differ is instructive about how little of a family is actually
shared: one's 0x20 case returns, another's falls through, a third jumps into
the tail past the `func_8002168C`/`print_card_status` pair; the held-key steps
are
-2 and -4; one has two flags rather than one.

### Nine more levers

**9. An unused RETURN VALUE looks like a dead address or copy in `$v0`.**
(3 functions) func_80023FF0, open_card_message and func_800217D4 all end with a
`lui`/`addiu` pair or an `addu $v0, $sN, $zero` that nothing reads -- in
func_800217D4's case split across a `beqz`'s delay slot on one exit. Every
caller ignores the value; as `void` the function is one or two words short.
Declare the return and the words appear.

**10. gcc's allocation priority is `floor_log2(n_refs) * freq / live_length`,
and a reference count is a lever.** func_80023C44's last 18 differing words
were nothing but `$s0` and `$s1` swapped: with three references the
D_80083EA4 base scored 1/27 against `p`'s 3/58 and lost the register.
Advancing that base IN PLACE -- spelling the second access as a byte offset,
`*(s32 *)((u8 *)D_80083EA4[p[0]] + 0x20)` -- is a fourth reference, lifts it
to 2/27, and wins it `$s0`. This is also the one lever that did NOT reach the
func_80022B48 family, where the base is a parameter: gcc keeps a parameter in
its incoming register unless a higher-priority allocno wants it, and no source
spelling raises the competitor enough.

**11. `addu` operand order follows RTL canonicalisation, and pre-scaling the
offset is NOT enough -- the pointer has to leave the expression.** Corrected
in town.bin's overlay, where a family needed `addu $s0, $a2, $a1` with the
scaled value first and `&q[i]`, `(u8 *)q + i * 0x10`, `i * 0x10 + (u8 *)q` and
`(i << 4) + (u8 *)q` all produced the operands the other way round. Pointer
arithmetic NORMALISES the order: `int + pointer` and `pointer + int` are the
same tree to the front end, so which one you write changes nothing. Making
both operands the same type is what preserves source order --
`(UnkTownVec16 *)(i * 0x10 + (s32)q)` matched. The original note, below, is
the same mechanism seen through a case where the cast happened to be there
already. (func_800269B4) gcc canonicalises `(plus (mult ...) (reg))` with the
shift first, so `p[26 + k]` gives `addu $v0, $v0, $s2` where the target has
`addu $v0, $s2, $v0`. Four base-first spellings of the same index all leave
that word wrong; only applying the scale first, `(u8 *)p + k * 4` with the
0x68 as a displacement, fixes it. Relatedly, parenthesise an offset you want
folded into the scaled value: `(u8 *)p + (k * 12 + 4)`, not
`(u8 *)p + k * 12 + 4`.

**12. A flat array indexed `[i * n + j]` adds `j` before the base; a 2-D array
does not.** (func_800269B4) `D_80028068[p[9] * 2 + i]` emits
`addu $v0, $v0, $s0` then `+= $s4`, six words wrong across three evaluations;
`extern u8 D_80028068[][2]` with `[p[9]][i]` adds the base first.

**13. A loop needs to BE a loop for loop.c to give it a preheader.**
(func_8002225C) Written `if (*s == ' ') do { s++; } while (*s == ' ');` gcc
shares one register for the 0x20 all three of the function's comparisons
test against; written as a plain `while`, loop rotation hoists a second copy
into the skip loop's own preheader, which is what the target has. `while`,
`for`, and either order of the body's increments all match.

**14. An 8-byte pair copy is a struct assignment, not two field
assignments.** (func_8002789C) gcc's aligned `movstrsi` is `lw`/`lw` then
`sw`/`sw`; two field assignments come out interleaved as `lw`/`sw`/`lw`/`sw`
and one word longer. Needs a 4-aligned 8-byte struct (`UnkTwoWords`) -- the
alignment-1 `UnkEight` gives `lwl`/`lwr`.

**15. When a symbol is relocated at an OFFSET, the source used a struct.**
(func_80024390) The target holds `&D_80083EA4` -- which is `D_80083E98 + 0xC`
-- in a callee-saved register and derives the row base as `that - 0xC` in
`$fp`. That only happens if gcc saw ONE symbol with field offsets, so the row
is a struct whose first access in the function is the field at +0xC. The unit
already declared `D_80083E98` as `s32 [][32]` for five other functions, so
this one gets an `__asm__` alias: `extern UnkSaveRow D_80083E98_s[]
__asm__("D_80083E98")`. Through two separate symbols each access materialises
its own `%hi` and the function is five words longer.

**16. `flags & (1 << i)` needs the mask in a local.** (func_80024390)
Otherwise combine rewrites it to `(flags >> i) & 1`. The flags word wants a
local too, and which of the two gets one settles the order of the loop
preheader's two hoists.

**17. A clamp's branch sense is `x >= N ? hi : x`.** (func_80024390, twice)
The target branches on the FALSE case with the clamped constant in the delay
slot; `x < N ? x : hi` branches on the true case and is three words wrong
each time.

### And one piece of bookkeeping that bites twice

`INCLUDE_RODATA` is not the only way a rodata symbol reaches the link. splat
migrates a symbol into the listing of the one function that references it, so
LANDING that function deletes the symbol and the link fails with
`undefined reference`. It happened to D_80020084 in the first sweep and to
D_800200A4 -- the string `" "` -- in this one. The fix both times: define the
block in C at the position it belongs to in the rodata stream, which for
D_800200A4 is immediately after `INCLUDE_RODATA(D_800200A0)`.

The mirror-image trap is gcc emitting a function's OWN constants there.
poll_card_events's two five-entry function-pointer tables verify clean as local
array initialisers -- 62 words, 0 differing -- and still break the build,
because gcc puts a function's constants in `.rodata` right after that
function and poll_card_events sits *before* this unit's first `INCLUDE_RODATA`
line, so they land at 0x80020000 and push every later block down. The
initialiser lever is only available where the function's position in the file
already puts its constants in the slot they belong to. The shape that works
there keeps both `INCLUDE_RODATA` blocks and COPIES from the extern --
`UnkFnTab5 a = gSwCardEventHandlers;` -- which is what the listing was showing
all along: batched `lw`/`sw` pairs are gcc's block move, not an initialiser
being materialised.

### The permuter is for ORDERINGS, and a good base is most of the work

The six-function arrayed-primitive family resisted two sessions of hand
restructuring and two 50-minute permuter runs, and then fell in 7 minutes and
15,022 candidates. What changed was the BASE it was given.

The residue was a register assignment: the targets keep the group pointer in
`$t3`, copied from `$a0` at word 0, leaving `$a0` free as the scratch for the
prims address. Reading that as "the function needs a second base pointer" is
natural and wrong -- a `b = c` used for the trailing stores does force the
copy, and does take the park from 13 differing words to 9, but leaves `c` in
`$a0` for the loop so the prims address still lands in `$v1`. Sixteen
spellings around that idea all stop at 9.

The actual answer is one reordering inside the loop:

```c
v = D_8002E5D8[0];        /* read it FIRST, into a local */
c->ptrs[i] = &c->ents[i];
c->prims[i][0] = v;
```

With the load first the allocator reaches the target's assignment on its own,
and all six functions go to zero. Nobody was going to guess that; a search
over statement orderings finds it immediately. The lesson is the division of
labour: work out the SHAPE by hand -- the struct spelling, which fields go
through which cursor, the branch senses -- and hand the ordering to the
permuter. The two earlier runs failed because they searched from a base whose
shape was still wrong (an `e` local keeping a live pointer, so the two field
stores were off the stride-16 induction variable); the successful run searched
from a base where only the order was wrong.
