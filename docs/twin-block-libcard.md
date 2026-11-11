# The twin block statically links libcard

`main.bin`'s second code run, the 0x80400000 twin block, contains its **own
copy of PSY-Q's libcard** -- twelve library modules at the tail of its text,
currently sitting in `csrc/ovl_main_80400000.c` as twelve `INCLUDE_ASM`
stubs named `func_8040xxxx`, indistinguishable from game code.

Found 2026-11-10 by pointing `azure libscan` at the overlay containers rather
than only at `slus_006.14`. This is the ONLY such find: `slus_006.14` was
already fully attributed, and `town.bin`, `dungeon.bin` and `main.bin`'s live
0x80020000 overlay are clean (see "The other three are clean" below).

The whole point of this file is that the identification is **done and
recorded**, so `psyq/lib` is no longer needed to finish the twin block. The
libraries were only ever an analysis input -- they are not a link input, the
retail library code is already in the containers -- and this was the last
analysis they were still owed.

## The modules

File offsets are into `data/main/main.bin`; the segment starts at 0x269000
with `vram: 0x80400000`, so vram = offset + 0x80197000.

| offset | vram / current name | module | PSY-Q export | words |
|---|---|---|---|---|
| 0x271644 | `func_80408644` | libcard/A92  | `_card_status` | 4 |
| 0x271654 | `func_80408654` | libcard/A93  | `_card_wait`   | 4 |
| 0x271664 | `func_80408664` | libcard/C171 | `_card_info`   | 4 |
| 0x271674 | `func_80408674` | libcard/C172 | `_card_load`   | 4 |
| 0x271684 | `func_80408684` | libcard/CARD | `_card_clear`  | 14 |
| 0x2716bc | `func_804086BC` | libcard/A78  | `_card_write`  | 4 |
| 0x2716cc | `func_804086CC` | libcard/A80  | `_new_card`    | 4 |
| 0x2716dc | `func_804086DC` | *unmatched*  | -- | -- |
| 0x271730 | `func_80408730` | *unmatched*  | -- | -- |
| 0x271768 | `func_80408768` | libsnd/SSINIT_C or SSINIT_H | `SsInit` / `SsInitHot` | 12 |
| 0x271798 | `func_80408798` | libcard/A74  | `InitCARD2`    | 4 |
| 0x2717a8 | `func_804087A8` | libcard/A75  | `StartCARD2`   | 4 |
| 0x2717b8 | `func_804087B8` | libcard/A76  | `StopCARD2`    | 4 |
| 0x2717c8 | `func_804087C8` | libcard/END  | `_ExitCard`    | 32 |

The run is 0x271644-0x271848. Text ends at 0x2719dc, so 0x271848-0x2719dc
(0x194 bytes) is past `END` and unaccounted for.

## Why this is a real linked section and not libscan noise

- **The deltas cluster.** All eleven libcard modules land at one of four
  deltas from their `slus_006.14` offsets -- +0x23422c, +0x23424c, +0x234250,
  +0x23426c -- every one inside a 0x40 window. Eleven modules agreeing to
  within 0x40 is a contiguous linked libcard section whose internal ordering
  differs slightly from the main executable's (there `C171`/`C172` precede
  `A92`/`A93`; here they follow).
- **Two of the matches are large.** `CARD` is 14 words with 12 invariant,
  `END` is 32 words with 21 invariant. Those are not coincidences; contrast
  the 4-word stubs, which are only credible *because* of the neighbours.
- **It answers a question the project had already asked.**
  `csrc/ovl_main_80400000.c:33` records that "func_804087C8 matches
  func_80069D78 at 0.90 masked" and leaves it open. `func_80069D78` is
  `slus_006.14` offset 0x3d578, which is exactly where `libcard/END` sits in
  the executable. **Both are libcard/END** -- two copies of the same library
  module, linked into two different binaries. The 0.90 rather than 1.00 is
  the relocation difference.
- **The landed C calls two of them.** `func_80408654(0)` / `func_80408654(1)`
  at `ovl_main_80400000.c:137,138,154,155,164,165` is `_card_wait(0/1)`, and
  `func_80408674(D_804094EC)` at :326 is `_card_load(...)`. Those calls now
  read as the libcard API they are.

The two unmatched functions in the 0x8c gap at 0x2716dc are most likely
libcard modules from the build this project has no copy of -- `A79`
(`_card_read`), `A88` (`_card_chan`), `INIT` (`StartCARD`/`InitCARD`/
`StopCARD`), `PATCH` or `FORMAT` are the unplaced candidates in
`psyq/lib/libcard.lib` -- or game code. `libscan` names neither at any
`--min-invariant` down to 2.

## What is left to do

Nothing that needs `psyq/lib`. Whenever twin block work resumes:

1. Re-mark 0x271644-0x271848 as `psyq/libcard/*` + `psyq/libsnd/*` asm
   subsegments in `splat/main.bin.yaml`, instead of letting the single
   `[0x2698a0, c, ovl_main_80400000]` subsegment swallow them.
2. Name them in `splat/main.bin_symbol_addrs.txt`, which currently holds **no
   entry at all** in 0x804086xx-0x804088xx, and drop the matching
   `INCLUDE_ASM` lines from `csrc/ovl_main_80400000.c`.
3. Decide `SSINIT_C` vs `SSINIT_H`. The two modules are byte-identical over
   these 12 words, so the libraries cannot settle it; the caller can.

Do not try to decompile these as game functions. That is the cost this file
exists to prevent -- twelve of the twin block's 262 functions are not the
game's code, and 99 were landed before anyone knew which.

## Related gap in the main executable

`libcard/END`'s export `_ExitCard` is **not** in
`splat/slus_006.14_symbol_addrs.txt` either, though the other ten libcard
names here all are. `func_80069D78` is that module; it can be renamed
independently of any twin block work.

## The other three are clean

For the record, so nobody re-runs these scans:

| container | range scanned | result |
|---|---|---|
| `slus_006.14` | 0x800-0x55000 | fully attributed; 160 modules confirmed at their configured offsets, 0 moved |
| `main.bin` live overlay | 0x25d000-0x269000 | clean -- one 9-word `libsnd/SSGNC` hit at 0x264dd0 falls inside landed `c main/memory` text |
| `town.bin` | 0xb000-0x7e000 | clean -- both hits (0x1cef4, 0x3a0dc) fall inside landed `c ovl_town`, at inconsistent deltas (+0x9d4, +0x9f34) |
| `dungeon.bin` | 0xa3000-0xf7700 | clean -- 0 modules located |

`azure libmatch psyq` over `slus_006.14`: **162/163 exact (99.4%), mean fuzzy
0.998**, reproducing the figure in `docs/where-we-are.md`.

`libscan`'s two standing residuals on `slus_006.14` are both explained and
neither is work:

- **6 "not currently in the config"** are duplicate modules shipped in two
  libraries, over byte ranges already attributed to the other copy:
  `libcard/C112` = `libapi/C112`, `libc2/EXIT` = `libc/A56`, `libc2/MEMMOVE`
  = `libc/MEMMOVE`, `libspu/S_IH` = `libsnd/UT_RON`, `libspu/S_W` =
  `libspu/S_R`, and `libgte/SMP_06` inside `libgte/SMP_05`. No new bytes.
- **11 "in the config but not located"** are five invented `unk_*` labels
  (`libspu/unk_31CC4`, `unk_32934`, `unk_33234`, `libcd/unk_34BD0`,
  `unk_362B4`), which are not module names and so cannot be located by
  construction, plus `libcd/BIOS_1`, `BIOS_2`, `BIOS_3`, `libcd/C_010`,
  `libetc/VMODE` and `libsn/SNMAIN`, which come from the libcd/libspu/libetc
  build this project has no copy of. `docs/where-we-are.md` already records
  both causes.
