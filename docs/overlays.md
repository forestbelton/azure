# Overlays: how the game's code is actually laid out

Written 2026-10-22, after `docs/where-we-are.md` had spent a while implying the
game was nearly decompiled when what is nearly decompiled is the engine.

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
assignment. That is exactly why 162 functions can match byte-for-byte against
code that calls into a 1 MB overlay nobody has looked at.

The same mechanism is what makes the overlays hard: their code is full of
`jal`s back into the engine and references to engine globals, so they are not
independent either.

## Where the overlays actually load (answered 2026-10-26)

Three code overlays are now placed. None of the containers is loaded as a
unit -- they are 2.5/16.1/26.5 MB against 2 MB of RAM, and the disc is
addressed by absolute LBA, so a container is just a region of it.

| Container | Overlay at | Loads at | Code | Vote | Functions |
|---|---|---|---|---|---|
| `town.bin` | `+0xb000` | `0x80088760` | `+0xd630..+0x51fc0` | 536 | 1815 |
| `dungeon.bin` | `+0xa3000` | `0x80088760` | `+0xa3e28..+0xf7700` | 353 | 862 |
| `main.bin` | `+0x25d000` | `0x80020000` | `+0x25d924..+0x264e20` | 130 | 241 |
| `main.bin` | `+0x269000` | `0x80400000` | `+0x2698a0..+0x2719e0` | 142 | 263 |

("Vote" is the jal-against-prologue agreement score that placed the overlay,
which is what the first version of this table reported in a column labelled
"Functions"; "Functions" is what splat actually emitted. The code spans are
the corrected 16-byte-aligned ends, not the shorter ones 77b2c94 reported --
see each yaml's comment.)

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

## The fourth overlay: `main.bin+0x269000` at `0x80400000` (2026-11-01)

This block was filed here as unresolvable -- "191 prologues, so certainly
code, whose base is unknown: only 58 internal jal targets and a best score of
6 against a floor of 5." **That measurement was wrong, not the method.** Run
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
  previous dead ends in this project turned out to be; see TODO.md.
* **Source line numbers.** Every assert site passes `__LINE__` as an
  immediate, which brackets function order and rough line structure within
  each unit.
* **Real identifiers**, from the assert expressions: `card_first_flg`,
  `(int)gsw->check_sum`.

splat also suggests a rodata file split at `+0x26907C` from jump-table
alignment, which is an independent witness for a unit boundary there.

The subsystem is the memory card: `bu00:`/`bu10:`, `TMP`, `ERROR` /
`NOTHING` / `UN FORMAT` / `NEW READY` / `READY` / `NOW CHECK`, `CARD%d %s`,
`CARD STATUS0 %x`, `illegal data`. Its external calls go to 65 distinct
engine targets, many already named -- `printf`, `exit`, `strcat`, `rand`,
`TestEvent`, `_card_wait` (libcard is resident at `0x3d3f8..0x3d578` with
module names). Its 69 private data symbols live at
`0x80027E20..0x8002D000`, a region no engine C file references.

### The assert-record census across all four containers

Same marker, scanned over every container:

| container | records | translation units named |
|---|---|---|
| `main.bin` | 10 | `c_server.c` (x2), `card_mes.c`, `card_opt.c`, `card_win.c`, `cardbk.c`, `cardinfo.c`, `cardslot.c`, `cardsme.c`, `memory.c` |
| `town.bin` | 76 | `main.c` (54), `lshop.c` (10), `tobjs.c` (8), `player.c` (4) |
| `dungeon.bin` | 0 | -- |
| `slus_006.14` | 0 | -- |

Two things follow. **None of town's 76 records is inside the `0x80088760`
overlay** -- they are all at `+0x2ab000` and beyond, in a second tier of
small low-RAM overlays, so the big town overlay has no oracle either. And
**dungeon was built with asserts out entirely**, so there is nothing of this
kind to recover there.

Town's tier 2 also leaks identifiers: `((u_long)p - (u_long)ost_w)/
sizeof(OSETDT)`, `((u_long)pp - (u_long)osel_w)/ sizeof(OSETDT*)`,
`cdhd_cnt`, `s0[0]`, `s1[0]`, `s[0]` -- two variable names and a typedef.

### How much code is left in each container

Scanning for code outside the placed overlays, by 4 KB chunk with at least
four prologues and three `jr $ra`:

| container | unplaced code | runs | largest |
|---|---|---|---|
| `main.bin` | ~0 | 1 (noise) | 12 KB / 14 prologues at `+0x21000` -- density too low to be code |
| `town.bin` | 0.64 MB | 59 | 69 KB / 291 prologues at `+0x649000` |
| `dungeon.bin` | 6.13 MB | 401 | 364 KB / 715 prologues at `+0xe2d000`, then 360 KB / 598 at `+0xcdd000` |

So **`main.bin` is structurally complete** with this block placed: both of
its code blocks are split and nothing else in the container is code.

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

1. **Find the archive structure.** Partly done. The engine's CD queue takes
   8-byte descriptors, `{nsec << 23 | dest, LBA}`, with `dest == 0` meaning
   "the D_80081480 bump arena"; the pump is `func_8003E758` and the queue
   dispatcher `func_8003E4FC`. The 49 descriptors at `0x80081020..0x800811a8`
   are the *audio* table (KCET sequences and VAB banks, verified by the magic
   bytes at each source offset), and `D_8006E6F4` is a 2-entry table of
   descriptor pairs selecting the town or dungeon sound bank. No descriptor in
   the ROM targets a code overlay, so those are built at runtime.
2. **Recover each entry's load address.** Done for the three code overlays
   above, by prologue/jal fingerprinting rather than from any table. Still
   open for the asset entries. Either from the container's own
   table, or by taking a code entry's internal `jal` targets and finding the
   base that makes them land on its own function starts.
3. **Then** a splat spec per overlay, with its own vram base, and a way to keep
   two overlays' identical addresses apart. **Done for the three code
   overlays** (2026-10-26). The "keep identical addresses apart" worry turned
   out not to bite: each container links from its own generated script, and the
   script names its object files explicitly, so `build/town.bin.ld` pulls in
   `asm/ovl_town.o` and nothing else. town's and dungeon's overlays never meet,
   and neither reaches the `slus_006.14` link. Calls out of an overlay into the
   resident engine arrive as plain absolute symbol definitions in that
   container's `_undefined_funcs_auto.txt` -- addresses, not objects -- so
   there is no second source tree to build and no conflict to resolve.

   Three things have to be right in the segment definition, and getting any of
   them wrong is what parked the segments between 77b2c94 and eec0572:

   * **`rodata`, not `.rodata`.** The leading dot routes a subsegment to a
     SIBLING C OBJECT and extracts nothing. With no `csrc/ovl_town.c` to route
     to, nothing is emitted and every reference into the overlay's own rodata
     comes out undefined -- which is all
     `undefined reference to jtbl_80089900` ever was.
   * **A following segment.** Without one the code subsegment runs to the end
     of the container; town.bin's became a single 4.3-million-line file.
   * **A text end on a 16-byte boundary.** splat emits
     `__romPos = ALIGN(__romPos, 16)` after a code segment, so a boundary off
     that grid pads the container. Find the end from the last `jr $ra`, not
     from where prologue fingerprinting stopped: all three of 77b2c94's figures
     were short of the real end (town 0x51ed8 against 0x51fc0, dungeon 0xf8000
     against 0xf7700, main 0x264c90 against 0x264e20).

   What this split actually yields: 1815 functions in town's overlay, 862 in
   dungeon's, 241 in main's -- against 178 in the resident engine. `make
   containers` rebuilds all four containers and checks them.

Sizes, for scale: the heuristic in `docs/where-we-are.md` puts about 8.4 MB of
code-bearing blocks across the three containers against 216 KB in the resident
executable. Even discounting it heavily for packed data, the content dwarfs the
engine.

## The unit map for the card overlay (2026-11-01)

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
naming `c_server.c`, in two functions** (`func_80020A80` at lines 185-226 and
`func_80020F18` at 263-277), while its other eight units have none. So the
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

The nine stay in order and do not overlap, which the alignment did not
enforce. Eight functions follow `memory.c` in the live block and 23 follow it
in the twin; in the twin those are PSY-Q library stubs (16-byte bodies at
16-byte spacing), not one of the nine units.

The anchored live functions themselves, which are the ones to start from
because their unit is known outright rather than bracketed:

| unit | anchored live functions |
|---|---|
| `c_server.c` | `func_80020A80`, `func_80020F18` (its own asserts), `func_80021594`, `func_8002191C`, `func_80021EB8`, `func_80021EE0` |
| `card_mes.c` | `func_80022D60` |
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

### What not to use the twin for

* **Its external `jal` targets are not retail engine addresses.** It calls
  printf at `0x8007C040` and exit at `0x8007BEF0`; in the shipped executable
  those land inside libgte's `rcossin_tbl`. It was linked against a different
  engine build.
* **It is not itself the goal.** It is dead code on retail. The live
  `0x80020000` overlay is what gets decompiled; the twin exists to say what
  the units are.

## Building the card overlay: the flags, and the first function (2026-11-01)

`main.bin`'s `0x80020000` overlay is now a `c` subsegment in the build, with
one function compiled from C and `main.bin` still byte-identical. All four
containers stay byte-identical.

### The overlays are -G0, and that is forced, not chosen

The resident executable is built at cc1psx `-G8`. The overlays are `-G0`.

Two reasons to believe it before measuring anything. There are **0
`$gp`-relative references across ~200K instructions** of split overlay code.
And there could not have been any: `$gp` is `0x80080994` and GPREL16 reaches
+/-32K from it, i.e. `0x80078994..0x80088994`, while this overlay keeps its
data at `0x80027E20..0x8002D000` and town's and dungeon's live at
`0x80088760` and above. psylink would have rejected a GPREL16 in any of them.

What `-G` actually changes here has nothing to do with `$gp` in the end. At
`-G8` cc1psx records `.extern SYM,4` for a 4-byte global and emits a bare
symbol reference, which the assembler expands into a `lui $at` / `%lo($at)`
pair **per use**. At `-G0` cc1psx emits its own `%hi`/`%lo` split address into
a general register, which can be hoisted and shared -- the hoisted-lui shape
this overlay uses everywhere. On `func_80020924` that is the difference
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
the `high` is emitted -- which is what landed `func_80020924`.

### One source reproduces both builds

The same C source compiles to the live block at `-G0` and to the **debug twin
at `-G8`**, both with 0 differing words:

| block | flags | words | shape |
|---|---|---|---|
| live `func_80020924` | `-O2 -G0` | 24 | `lui $v1` hoisted, shared by both stores |
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

### `.rodata` WITH the dot, once a C sibling exists

town.bin.yaml and dungeon.bin.yaml both carry a hard-won note saying to use
`rodata` and **not** `.rodata`, because the dot "routes a subsegment to a
sibling C object and extracts nothing". Converting this overlay to `c` makes
splat refuse the undotted form outright: *"The `rodata` section was not
prefixed with a dot, which is required for the rodata migration feature to
work properly and avoid build errors due to duplicated symbols at link-time."*

Both are the same rule from opposite sides. The dot routes rodata to a sibling
C unit, so it is wrong when there is no such unit and required when there is.
The earlier note is right for town and dungeon, which are still `asm`, and
stops being right the moment either of them is converted.

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

### func_80020924

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

Two of the overlay's 241 functions were free: `func_80021904` and
`func_8002519C` are `jr $ra; nop`, and splat emits them as real C rather than
`INCLUDE_ASM`. So the overlay stands at 3 of 241.

## Sweeping the card overlay (2026-11-01)

**81 of 241 functions, 902 of 7487 words, `make containers` byte-identical on
all four.** `tools/decomp/ovlverify.py` for the fast loop, `make build/main.bin`
as the gate.

### The carve is not possible yet, and does not block the sweep

The plan said to carve the file into the nine units first. That was the wrong
order, and the rodata is what shows it.

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

* **20 one-line message wrappers** (`func_80022E74..func_8002316C`, 10 words
  each, `0x28` apart) around `card_mes.c`'s `func_80022D60(a, id, kind, p)`,
  one per message id 0..19. Decoded mechanically from which argument register
  each wrapper writes; the register copies (`addu $a2, $a1, $zero` where kind
  equals id, `addu $a3, $a2, $zero` where both are 0) are gcc reusing an
  equal value, not source. All 20 landed from one generator.
* **5 three-pointer primitive initialisers** (37 words each).
* **13 small accessor/setter/wrapper groups** of 2-20 words.

### Four levers this overlay needed

1. **The pointer-local hoist** (`docs/matching-levers.md:3173`), per site.
   `func_80020924` and `func_8002137C` both store to a global on one branch
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
difference between 0 and 2 differing words in `func_80021EB8`.

### The one near-miss left

`func_80022B48` and its five siblings (40 words each, 240 words together) are
the arrayed form of the 37-word initialiser, at six base offsets with six
counts (`0x8B0`/7, `0x184`/18, `0x240`/10, `0xB20`/21, `0x44`/4, `0xD4`/4) and
an identical internal layout at all six. The shape is worked out and recorded
in `csrc/ovl_main_80020000.c`; three spellings have been measured (38w/40
DIFF, 39w/37, 39w/40) and the open question is which induction variables the
target derives, since it keeps the base in `$t3` with only `$a3 = 0` above the
loop guard and everything else in the preheader.

## The levers this sweep keeps needing

These are not overlay-specific -- they are ordinary gcc 2.7.2 behaviour that
the engine's own functions happened not to exercise much -- but each has now
cost two or more iterations in this unit, so they are collected here. Counts
are how many functions in `csrc/ovl_main_80020000.c` each one landed.

**1. Put the table base in a local before indexing it.** (5 functions)
`&TBL[i]` written directly lets gcc fold a constant field offset into the
symbol's `%lo` and lets it compute `i * stride` before materialising the base;
assigning the table to a local first forces the base out as its own quantity,
materialised first. Symptoms, all one or two words:

* `D_800280B4[p[2]].unk_15` with the base inline gives `lbu $v0, 0($v0)` where
  the target has `lbu $v0, 0x15($v0)` -- the 0x15 went into the relocation.
  With `UnkStrSet *t = D_800280B4; r = &t[p[2]];` it matches
  (func_80022488, func_80022CD8).
* `D_80083E98[idx]` inline emits the shift before the `lui`/`addiu`; through a
  local it emits the base first and fills the load-delay slot with the
  `addiu` (func_80021D54, func_8002219C, func_800254E4).
* The same thing one step further on: `row = tbl` as a separate copy makes the
  materialised address and the walking pointer two quantities, which is the
  extra `addu` the target has (func_80021A04). See matching-levers.md's
  `p += x` is one long quantity note.

**2. A value used only after a call must not be computed before it.**
(3 functions) Written above the call it is live across it, takes a
callee-saved register, and costs that register's save and restore -- two
words, sometimes three. func_800251A4's 1-or-3 flag (37 words against 35),
func_80021A84's table pointer, func_80023E14's table base. The fix is always
to move the assignment below the last call that precedes its first use.

**3. Write the store into both arms of an if/else and let jump.c cross-jump
them.** (3 functions) One store of a local after the join keeps the local
live across the arms' calls; a store in each arm is merged by cross-jumping,
usually into the following call's delay slot. func_80025230, func_8002570C,
func_80025848.

**4. Declaration order decides which callee-saved register each local gets,
and the order the setup is emitted in.** (6 functions) The target's first
instructions are the cheapest way to read this off: whichever local is
initialised first in the listing is declared first. func_80021EB8 (`sum`
before the cursor, 2 DIFF), func_80023194 and func_800239B0 (counter before
cursor, plus a local COPY of the parameter, 6 DIFF), func_8002191C,
func_80025E54, func_80021A04.

**5. `x++` is not `x = 1` even when x is provably 0.** (1 function, but it
unblocked three other things) func_80021A04's mismatch flag increments; with
the increment form gcc stops hoisting the flag's `= 0` out of the enclosing
loop, which is what made the rest of that function's allocation fall into
place.

**6. Index a cursor by an integer offset, not by walking a pointer, when the
displacement is large.** (2 functions) `q = p; ... q[0x98] ...; q++` lets gcc
fold the 0x98 into the base (`addiu $s0, $s2, 0x260`, displacement 0); an
integer `off` with `p[off + 0x98]` keeps the displacement where the target has
it (func_800240EC, func_80025DC8).

**7. A trailing correction in the listing can be the compiler's, not the
source's.** func_8002191C increments its count before the loop, again in the
loop's delay slot, and decrements after -- which reads like a source `n--` and
is loop rotation of `do { n++; ... } while (...)`. Writing the `n--` makes it
40 words / 9 DIFF; leaving it out, 39 / 0.

**8. Branch LAYOUT order is part of the source.** Which arm the compiler
places first follows the source's test sense. func_800254E4 is one word short
with `if (x != 0) A else B` and exact with `if (x == 0) B else A`, because the
target reaches A by a taken `bnez` rather than by falling through.

## The second sweep: 82 of the overlay's functions in one pass (2026-11-01)

The sweep that follows the one above took the card overlay from 192 of 241
functions to **220**, and from roughly 54% to **86.2% of its 7487 words (6457
landed)**. Every function in the unit that had not been looked at before is
now either landed or parked; the 28 remaining stubs are all near-misses with
measurement tables in `docs/near-misses/`.

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
branch target's load uses. func_80020F18 went from 121 words against 114 with
nothing else differing, to exact; func_80020A80, the overlay's second largest
function at 264 words, then came out zero-differing on the FIRST spelling --
eight `OpenEvent` calls inside the `EnterCriticalSection` pair, each asserting
the handle is not -1, then eight `EnableEvent` calls each asserting a non-zero
result. All sixteen `%hi` values stay live in their own callee-saved register
across the calls, which is why that function saves `$s0..$s7` and `$fp`.

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
the tail past the `func_8002168C`/`func_80021904` pair; the held-key steps are
-2 and -4; one has two flags rather than one.

### Nine more levers

**9. An unused RETURN VALUE looks like a dead address or copy in `$v0`.**
(3 functions) func_80023FF0, func_80022D60 and func_800217D4 all end with a
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

**11. `addu` operand order follows RTL canonicalisation, so pre-scale the
offset.** (func_800269B4) gcc canonicalises `(plus (mult ...) (reg))` with the
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
func_80021594's two five-entry function-pointer tables verify clean as local
array initialisers -- 62 words, 0 differing -- and still break the build,
because gcc puts a function's constants in `.rodata` right after that
function and func_80021594 sits *before* this unit's first `INCLUDE_RODATA`
line, so they land at 0x80020000 and push every later block down. The
initialiser lever is only available where the function's position in the file
already puts its constants in the slot they belong to. The shape that works
there keeps both `INCLUDE_RODATA` blocks and COPIES from the extern --
`UnkFnTab5 a = D_80020034;` -- which is what the listing was showing all
along: batched `lw`/`sw` pairs are gcc's block move, not an initialiser being
materialised.

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
