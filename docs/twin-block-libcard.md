# The twin block statically links libcard

`main.bin`'s second code run, the 0x80400000 twin block, contains its **own
copy of PSY-Q's libcard** -- twelve library modules at the tail of its text,
sitting in `csrc/ovl_main_80400000.c` as twelve `INCLUDE_ASM` stubs named
`func_8040xxxx`.

**What was already known:** that the block's last 23 functions are PSY-Q
library code and not decompilation work, which is why the unit header and
`docs/where-we-are.md` both put the real remaining work at 239 rather than
262, and why `func_804087C8`'s resident twin `_ExitCard` (0x80069D78) was
already tied to `asm/psyq/libcard/END.s`. **What is new here** is WHICH modules the
other eleven are, and their real PSY-Q export names.

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
- **It extends an identification the project had already made.**
  `csrc/ovl_main_80400000.c:33` records that `func_804087C8` matches
  `_ExitCard` (0x80069D78) at 0.90 masked and that its listing is
  `asm/psyq/libcard/END.s` -- so that one module was already placed, and the
  0.90 rather than 1.00 is the relocation difference. The scan confirms it
  independently from the library bytes and places the ELEVEN NEIGHBOURS the
  note stopped at, which is what turns "the tail is library code" into a
  module-by-module account.
- **The landed C calls two of them.** `func_80408654(0)` / `func_80408654(1)`
  at `ovl_main_80400000.c:137,138,154,155,164,165` is `_card_wait(0/1)`, and
  `func_80408674(D_804094EC)` at :326 is `_card_load(...)`. Those calls now
  read as the libcard API they are.

The two unmatched functions in the 0x8c gap at 0x2716dc are **very likely
the twin's own `libcard/INIT`**, from the build this project has no copy of.
`libscan` names neither at any `--min-invariant` down to 2, so this is
inference from shape, not a content match -- but it is a tight fit:

- `func_80408730` is 0x38 bytes, which is `StartCARD`'s size in the resident
  `INIT` **exactly**; `func_804086DC` is 0x54 against `InitCARD`'s 0x58, one
  word shorter.
- The gap occupies the same slot in the module order. In `slus_006.14`,
  `INIT` sits between `A80` and `A74`; here the gap sits between `A80` and
  `A74` too, with only `SSINIT` between it and `A74`.
- `0x54 + 0x38 = 0x8c`, the whole gap, with no third function where the
  resident `INIT`'s `StopCARD` would be -- consistent with a build that
  drops it, and with `func_8003D56C` calling only `InitCARD` and
  `StartCARD`.

**CONFIRMED ON CONTENT, 2026-11-13, and the names are `InitCARD` and
`StartCARD`.** Both are landed C now, and what they CALL settles it without
needing the libcard build this project lacks:

    func_804086DC(long pad_enable)      func_80408730(void)
    {                                   {
        ChangeClearPAD(0);                  EnterCriticalSection();
        EnterCriticalSection();             func_804087A8();   /* StartCARD2 */
        func_80408798(pad_enable);          ChangeClearPAD(0);
        func_804088A0();                    ExitCriticalSection();
        func_80408958();                }
        ExitCriticalSection();
    }

`func_80408798` is `B0:0x4A init_card`, `func_804088A0` is `B0:0x4B
start_card` and `func_804087A8` is `A75 StartCARD2` -- each a three-
instruction BIOS stub in this same table. A critical section wrapped around
`init_card`/`start_card` IS `InitCARD`; one wrapped around `StartCARD2` IS
`StartCARD`. That is a content match in the only sense available, and it
agrees with the size and slot-order arguments above rather than resting on
them.

**A WARNING THAT COST A WRONG CONCLUSION.** `csrc/main_twin.h` carries
`/* func_8002XXXX */` comments in this region, and in the REST of that header
such a comment is a verified twin-to-live counterpart. Here it is a SYMMAP
VOTE, and two of them are wrong: `func_804086CC` is voted `func_80027C60` and
`func_80408730` is voted `func_80027D7C`. Neither holds -- `func_804086CC` is
three instructions (`addiu $t2,0xB0 / jr $t2 / addiu $t1,0x50`), a BIOS stub,
against 0x30 of game code; `func_80408730` is 0x38 against 0x54. Reading
those votes as pairings produced the claim that the LIVE overlay links
libcard into `csrc/main/memory.c`, which is false: live `func_80027C60` is
`func_8005863C(); D_80080A84 = 1; start_card_server();`, plain game code.
`libscan` had already matched `func_804086CC` correctly as `A80 _new_card`.
The header now says which comments in this block are votes.

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

Do not try to decompile these as game functions. The block's tail was
already excluded from the work as library code, so nothing was mis-landed;
what this file adds is that the exclusion can now be stated per module and
per name instead of as "the last 23".

## The resident executable's libcard run is now fully named

Closed 2026-11-10. `libcard/END`'s export `_ExitCard` was missing from
`splat/slus_006.14_symbol_addrs.txt`, and so were all three of
`libcard/INIT`'s. Four names added:

| vram | name | module |
|---|---|---|
| 0x80069c90 | `InitCARD`  | libcard/INIT +0x000 |
| 0x80069ce8 | `StartCARD` | libcard/INIT +0x058 |
| 0x80069d20 | `StopCARD`  | libcard/INIT +0x090 |
| 0x80069d78 | `_ExitCard` | libcard/END  +0x000 |

`INIT` needed the module's own export offsets, because it exports three
symbols and `azure lib --list` prints them in record order, not address
order -- it reads `StartCARD InitCARD StopCARD`, which would have mis-named
two of the three if taken at face value. `PsyQObject.text_exports()` gives
each export's offset within `.text`, so the mapping is read rather than
guessed; `libcard/A92` -> `_card_status` and `libcard/END` -> `_ExitCard`,
both already known, were used as controls. The run is contiguous and the
arithmetic closes: `A80` ends at 0x80069c90 where `InitCARD` begins, and
`INIT`'s 0xb8 bytes end at 0x80069d48, which is `InitCARD2`.

Renaming these two is what revealed that `kernel.h` declares the whole
low-level card API -- `void InitCARD(long val)`, `long StartCARD(void)`,
`long StopCARD(void)`, `_bu_init`, `_card_*` -- so `include/structs.h`'s
hand-added `func_80069C90`/`func_80069CE8` declarations were redundant and
are gone. It also explained `func_8003D56C`'s "unused" parameter: it is
`InitCARD`'s `val`, forwarded, and forwarding a parameter already in `$a0`
emits no code. `func_8003D56C` now reads `InitCARD(arg0)` and all four
containers stay byte-identical.

Note for anyone grepping `psyq/include`: those headers are CRLF with
extended-ASCII, so plain `grep` calls them binary and silently reports
nothing. `grep -a` is required, and a missing `-a` is why these were thought
undeclared at first.

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
