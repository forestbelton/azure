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
