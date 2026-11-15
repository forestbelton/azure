# The data in slus_006.14

Companion to `docs/where-we-are.md`, which covers the code. Numbers here were
re-measured on 2026-11-14 against `data/slus_006.14`
(sha1 e6bfbb95ff6676899e077481221d73ddd4d3bf52); re-measure rather than trust
them.

**`splat split` now reports 0 bytes unknown.** Every byte of the file belongs
to a named subsegment, and `make containers` stays byte-identical on all four
containers. What changed on 2026-11-01, and the evidence for it, is below.

**Unknown is not the same as owned, and OWNED is what is left.** A `data` or
`rodata` subsegment is accounted for but still lives in `asm/data/*.s` as
`dlabel`s rather than as C. As of 2026-11-14 the game's own unowned bytes are
**61,580 in four subsegments**, down from 70,308, and 968 of those four
cannot be owned in C at all: the three string pools went
to C that day (`csrc/str/speakers.c`, `csrc/str/spells.c`,
`csrc/str/commands.c`, 6,160 bytes and 237 objects), then `game_gp` as
`csrc/game_gp.c` (60 bytes, 10 objects, the project's first `.sdata`
subsegment) and `gp_data`'s `.sdata` run as `csrc/gp_data.c` (2,508 bytes,
287 objects). No `rodata` subsegment of the game's is left, and the only
`data` ones are `game_data` and a 968-byte run of zeros. See "What is left, in order" below for the
rest and for the two things that block it.

## The file is not the program

The PS-X EXE header at file 0x0 gives:

| field | value |
|-------|-------|
| `t_addr` | 0x8002D000 |
| `t_size` | 0x54800 (346,112) |
| `pc0` | 0x80033930 (`__SN_ENTRY_POINT`) |
| `d_addr`/`d_size`/`b_addr`/`b_size` | all 0 |

So the loaded image is **file 0x800-0x55000, vram 0x8002D000-0x80081800**, and
the last **176,128 bytes (0x2B000, 33.6% of the 512 KB file) are never read by
the PSX loader**. They are the `tail_unloaded` subsegment.

The boundary is exact to the byte and needs no inference: the image's last
non-zero byte is at file 0x54C35, 0x54C36-0x55000 is zero padding, and 0x55000
is non-zero immediately.

What the tail is was **not** established, only what it is not:

- **not SPU-ADPCM** -- in 16-byte blocks the flag byte is <= 7 in 3.0% of
  blocks against a 3.1% random baseline, at every 4-byte phase tried.
- **not a VAB/SEQ/VAG/TIM resource** -- no `VABp`, `pBAV`, `SEQp`, `pQES`,
  `VAGp`, `pGAV` magic occurs anywhere in the file, and the only `MThd`/`MTrk`
  is the 16 bytes at file 0x68FC, inside the image.
- entropy is a flat ~7.8 bits/byte with ~0.3% zero across all 172 KB, so it is
  compressed or encrypted content rather than structure.

Nothing in the image can reach it, so it does not matter for the
decompilation. Do not spend time on it.

**2026-11-01: it has a twin, and the twin is dead too.** `main.bin` carries
2,069,472 bytes at 0x28420..0x221800 with the *same* fingerprint as this tail
-- entropy 7.9999 against 7.9989, chi-square 218.5 against 268.7, zero
fraction 0.00392 against 0.00374, all three matching a true-random control --
and there the case is closed independently: the CD-ROM command log shows the
game seeking straight past it, and all 1011 of its sectors carry a valid EDC,
so it is deliberately mastered filler in a reserved hole. That run also
answers two of the open questions about this one: it is incompressible (zlib
and xz both expand it) and Berlekamp-Massey gives it linear complexity n/2+1,
so "compressed or encrypted" above is too generous -- no LFSR or word LCG
produced it either. See `docs/main-bin-data.md`.

## The binary states its own layout

libsn/SNMAIN is the first object in link order, so its `.data` is the first
`.data` in the image: 36 bytes at file 0x3E01C / vram 0x8006A81C. All 36 bytes
match our `psyq/lib/libsn.lib` copy exactly, and SNMAIN exports a name for
every word, so the **SN module descriptor can be read out in full**:

| vram | name | value |
|---|---|---|
| 0x8006A81C | (unnamed) | 0 |
| 0x8006A820 | `__heapbase` | 0 |
| 0x8006A824 | `__heapsize` | 0 |
| 0x8006A828 | `__text` | 0x80033930 |
| 0x8006A82C | `__textlen` | 0x36EEC |
| 0x8006A830 | `__data` | 0x8006A81C |
| 0x8006A834 | `__datalen` | 0x16178 |
| 0x8006A838 | `__bss` | 0x800815C0 |
| 0x8006A83C | `__bsslen` | 0x71A0 |

It self-validates twice over, which is why it is worth trusting:

- `__text` + `__textlen` = 0x8006A81C, which is **exactly** `__data`.
- `__data` + `__datalen` = 0x80080994, which is **exactly** `_gp` -- the
  `gp_value` in `splat/slus_006.14.yaml`, derived independently from
  $gp-relative code.

It also pins `.bss`, which is not otherwise visible in the file:
**0x800815C0-0x80088760**. The image ends at 0x80081800, so the file carries
the first 0x240 bytes of `.bss` as zeros -- which is why the image's last
non-zero byte is 0x54C35 (vram 0x80081435), just below `__bss`. `__main`
zeroes the rest. Both `.bss` clusters this doc used to list from reference
counting (0x80081E90-0x80084960 and 0x800869A8-0x80086F5C) fall inside that
range; the isolated references running up to 0x800FC000 are overlays
(`docs/overlays.md`), not `.bss`.

So the image is:

```
0x80033930  .text   0x36EEC
0x8006A81C  .data   0x16178   (ends exactly at _gp)
0x80080994  _gp               (.sdata/.sbss follow)
0x800815C0  .bss    0x71A0    (first 0x240 bytes in the file, zero)
0x80081800  end of image
```

The `.rodata` that precedes `.text` in the file (0x800-0x7130) is a separate
group the splat ld script emits ahead of the text; see "two data groups" below.

## What splat sees now

| type | bytes | % of file |
|---|---|---|
| `bin` (`tail_unloaded`, outside the image) | 176,128 | 33.59% |
| `c` | 167,596 | 31.97% |
| `data` | 94,024 | 17.93% |
| `asm` | 57,408 | 10.95% |
| `.rodata` (owned by a sibling C object) | 25,208 | 4.81% |
| `header` (outside the image) | 2,048 | 0.39% |
| `rodata` (still disassembled) | 1,876 | 0.36% |
| **unknown** | **0** | **0.00%** |

The `shiftjis` row is gone: that subsegment type is no longer used anywhere in
this container. Every string pool is C now, which is also why `.rodata` went
from 1,400 bytes to 25,208 and `rodata` from 11,640 to 1,876. Of the 1,876
that are left, 1,852 are the thirteen PSY-Q library `.rdata` runs at
0x69F4-0x7130, which stay assembly by the same policy that keeps the library
code assembly; the other 24 are `tbl_660C` and `smf_magic`.

### Why splat's own total says 79.58%

`splat split` reports `Split 417 KB (79.58%) in defined segments` and lists
only `header`, `shiftjis`, `asm`, `c` and `bin`. That is **a splat bug, not a
gap in the map**, and it is worth knowing because the headline number now
*falls* as the map improves.

`Statistics.print_statistics` sums everything handed to `add_size`, and that
comes from each segment's `statistics` property. `CommonSegAsm`, `CommonSegC`,
`CommonSegBin` and `PSXSegShiftjis` inherit it from `Segment`, which returns
`{self.type: size}`. But `CommonSegData` -- and `CommonSegRodata`, which
subclasses it, and `CommonSegBss` -- inherit from **`CommonSegGroup`**, whose
override aggregates `self.subsegments`. A `data` or `rodata` subsegment is a
leaf with no subsegments, so the aggregation returns an empty dict and its
bytes are never registered:

| segtype | `statistics` comes from | registered? |
|---|---|---|
| `asm`, `c`, `bin`, `shiftjis`, `header` | `Segment` | yes |
| `data`, `rodata`, `.data`, `.rodata`, `.bss` | `CommonSegGroup` | **no** |

So the report line counts 417,220 of 524,288 bytes and silently drops 107,068
-- exactly this project's `data` (94,028) + `rodata` (11,640) + `.rodata`
(1,400). 417,220 + 107,068 = 524,288.

This was always happening, just invisibly: before 2026-11-01 the dropped
bytes were only 2,888 (the jump-table `.rodata` plus two psyq `data`
subsegments), which is why the old report summed to 78.51% + 20.94% = 99.45%
rather than 100%. Converting unnamed `bin` blobs to `data` moved their bytes
out of `unknown` *and* out of the total, so "Split ... in defined segments"
went **down** from 78.51% to 79.58% by less than the 20.94% that left the
unknown line.

**What to trust.** The `unknown` line is correct -- `CommonSegBin` registers
properly, and an unnamed `bin` is what `statistics_type` maps to `"unk"`
(`segtypes/common/bin.py:38`), so `unknown: 0 B (0.00%)` means what it says.
For the rest, use the accounting table above rather than splat's total.

## The library stamp

Every PSY-Q library carries exactly one 8-byte record

```
50 73 <id> 15 9C <ck> 40 00        ASCII "Ps", then a library id
```

at **offset 0 of one designated module's section**. All 18 libraries in
`psyq/lib` carry one, which makes the id a lookup table:

| id | library / module | id | library / module |
|---|---|---|---|
| 00 | libapi/C114 | 09 | libgte/MSC00 |
| 01 | libc/A63 | 0A | libgun/GUN |
| 02 | libc2/PUTS | 0B | libmath/FERR |
| 03 | libcard/INIT | 0E | libpress/LIBPRESS |
| 04 | libcd/BIOS | 0F | libsio/SIO |
| 05 | libcomb/COMB | 10 | libsnd/SSINIT |
| 06 | libetc/VSYNC | 11 | libspu/S_INI |
| 07 | libgpu/SYS | 14 | libtap/TAP |
| 08 | libgs/GS_001 | 18 | libmcrd/LIBMCRD |

Seven occur in this executable. Because the stamp is at section offset 0, each
one marks a section start exactly:

| file | id | what it marks |
|---|---|---|
| 0x37E80 | 09 | libgte/MSC00 `.text` -- confirms an existing placement |
| 0x3D608 | 01 | libc/A63 `.text` -- confirms an existing placement |
| 0x3D7C4 | 00 | libapi/C114 `.text` -- confirms an existing placement |
| 0x4D4B8 | 06 | libetc/VSYNC `.data` |
| 0x4E6A8 | 04 | libcd/BIOS `.data` |
| 0x53F74 | 07 | libgpu/SYS `.data` |
| 0x540E4 | 03 | libcard/INIT `.data` (8 bytes: the stamp is the whole section) |

**There is no libspu stamp (11) and no libsnd stamp (10) anywhere in the
file**, which is a finding rather than a gap -- see the next section.

## Scanning `.data` the way libscan scans `.text`

`azure libscan` matches library modules against the executable with
relocation masking, but only over `.text`. Applying the same method to each
module's `.data`/`.rdata`/`.sdata` places the whole library data region at
module granularity, with **zero mismatching words** in every row below. Tiny
all-zero sections match anywhere, so hits are filtered to those with at least
8 distinct non-zero invariant words.

| file range | module | bytes | distinct invariant words |
|---|---|---|---|
| 0x3E01C-0x3E040 | libsn/SNMAIN | 36 | (exact, whole section) |
| 0x4CCEC-0x4D158 | libspu/S_INI | 1132 | (exact, see below) |
| 0x4D158-0x4D1B4 | libspu/SPU | 92 | 12 |
| 0x4D1C0-0x4D4B8 | libspu/S_RMP | 760 | 122 |
| 0x4D4B8-0x4D4D0 | libetc/VSYNC | 24 | 6 |
| 0x4D4D0-0x4E5C4 | libetc/INTR | 4340 | 25 |
| 0x4E618-0x4E628 | libcd CD-register table | 16 | shared by every `C_0xx` |
| 0x4E6A8-0x4E9C0 | libcd/BIOS | 792 | 152 |
| 0x4EA7C-0x4EACC | libcd/C_011 | 80 | 18 |
| 0x4EACC-0x4F2CE | libgte/SINCOS | 2050 | 504 |
| 0x4F2E0-0x4F460 | libgte/SQRTBL | 384 | 96 |
| 0x4F460-0x4F770 | libgte/MTX_006 | 784 | 29 |
| 0x4F770-0x53770 | libgte/CSTBL | 16384 | 4092 |
| 0x53770-0x53F72 | libgte/RATAN | 2050 | 513 |
| 0x53F74-0x540E4 | libgpu/SYS | 368 | 15 |
| 0x540E4-0x540EC | libcard/INIT | 8 | the stamp |
| 0x540EC-0x5416D | libc/CTYPE0 | 129 | 16 |
| 0x54170-0x54188 | libapi/COUNTER | 24 | 6 |

The libgte run is contiguous to the byte: SINCOS, pad, SQRTBL, MTX_006, CSTBL
and RATAN tile 0x4EACC-0x53F72 exactly, and the two pad bytes run up to the
libgpu stamp at 0x53F74.

### The missing libspu stamp explains itself

`libspu/S_INI.data` is 1140 bytes in our copy and matches at 0x4CCEC with 8
mismatching words -- but **`S_INI.data[8:]` matches at 0x4CCEC with zero
mismatches over 1132 bytes**, and the 8 bytes dropped are precisely the
library stamp. The game linked an S_INI built without it. That also makes
libspu/SPU's `.data` land contiguously at 0x4D158 rather than overlapping.

Every S_INI export offset therefore shifts down by 8, which is how the
`_spu_*` names in `splat/slus_006.14_symbol_addrs.txt` were derived. Three
independent cross-checks on that shift:

- `_spu_RXX` lands at 0x80079958, the address reached by SPU, S_GAV, S_GKS,
  S_GR, S_SCA, S_SK, S_SR, S_SRA and S_SRMP -- exactly the register get/set
  modules that would share a register shadow, and *not* S_INI.
- `_spu_RQmask` lands at 0x8007951C, reached by S_INI and S_SK (SpuSetKey).
- `_spu_mem_mode_plus` lands at 0x80079980, reached by SPU, S_CRWA,
  S_M_INIT, S_M_UTIL and S_STSA -- the malloc/transfer modules.

This is the same story the five `unk_*` code blocks tell: the game's libspu
and libcd are a build we do not have a copy of.

## The libraries' `.rdata` at 0x69F4-0x7130

The same `.data` scan run over the region ahead of the text places it module
by module, zero mismatching words throughout. These were typed `data` until
2026-11-01, which was simply wrong -- they are `.rdata`.

| file range | module | bytes |
|---|---|---|
| 0x69F4-0x6A3C | libspu/SPU | 72 |
| 0x6A3C-0x6A7C | libspu/S_SCA -- `jtbl_8003323C`, `jtbl_8003325C` | 64 |
| 0x6A7C-0x6AB8 | libspu/unk_32934 -- `jtbl_8003327C` and the run at 0x6A9C | 60 |
| 0x6AB8-0x6AC8 | libetc/VSYNC (its whole `.rdata`, `"VSync: timeout\n"`) | 16 |
| 0x6AC8-0x6B34 | libetc/INTR | 108 |
| 0x6B34-0x6B60 | libetc/INTR_DMA | 44 |
| 0x6B60-0x6B78 | libcd/EVENT (`"CdInit: Init failed\n"`, unique match) | 24 |
| 0x6B78-0x6B80 | libcd/SYS (`"none"`, already `gCd_Str_None`) | 8 |
| 0x6B80-0x6DD0 | libcd/BIOS | 592 |
| 0x6DD0-0x6E14 | libcd/CDREAD | 68 |
| 0x6E14-0x6E2C | libcd/C_011 (`"DMA STATUS ERROR %x\n"`, unique match) | 24 |
| 0x6E2C-0x6EFC | libgpu/PRIM | 208 |
| 0x6EFC-0x7130 | libgpu/SYS | 564 |

Two things fell out of this. The **libcd/libgpu boundary was wrong**: it was
0x6E44, which is 24 bytes past the start of libgpu/PRIM's `.rdata`, so
`"tpage: (%d,%d,%d,%d)"` was filed under libcd. It is 0x6E2C now. And the two
jump-table runs at 0x6A3C and 0x6A7C resolve to **S_SCA** and **unk_32934**
from their targets (0x8005EE34/0x8005EEFC are file 0x325FC/0x326FC, inside
S_SCA; 0x8005F278/0x8005F358 are file 0x32A78/0x32B58, inside unk_32934) --
a third independent line of evidence that `unk_32934` is a libspu module.

## The three splat diagnostics, and what they were

`splat split` used to print three warnings. They were worth chasing: two were
real and one is permanent.

**0x6A3C and 0x6D30, "symbol at vram ... is a jumptable, indicating the start
of the rodata section _may_ be near here" -- both correct.** The blobs at
0x69F4 and 0x6B60 were typed `data` and really are `.rodata`. 0x6A3C, the
offset the first warning named, is exactly where libspu/SPU's `.rdata` ends
and the jump tables begin. Fixing the types produced the table above and
found the 0x6E44 boundary bug.

**0x54D98, same warning, in `gp_data` -- a permanent false positive.**
0x80081598 is a RAM pointer slot that libapi/PATCH loads from and stores to
(`asm/psyq/libapi/PATCH.s:13` and `:44`); it is zero in the file, and
spimdisasm classifies it as a jumptable because code jumps through it.
`.rodata` does not start there: 0x54D98 is past `__data`'s end (`_gp`,
0x54194) and just below `__bss` (0x800815C0 / file 0x54DC0), so it is
`.sbss`. Acting on the suggestion would be wrong. It cannot be suppressed
without `suggestion_rodata_section_start: False`, which would also silence
the check over `game_data`, so it is left to print. PATCH's `.sbss` is 16
bytes and exports no name, so the slot cannot be named either.

**What prints now.** Retyping the region to `rodata` replaced those with 14
"Rodata segment X may belong to the text segment Y" hints, which splat
suppresses once a rodata subsegment has a *sibling* -- a text subsegment of
the same name (`segtypes/common/rodata.py:34`). Naming each piece after its
owning module cleared 12 of them. The subsegment names in the yaml are
therefore load-bearing: rename one and its hint comes back.

Two hints remain, on `psyq/libcd/BIOS`, naming `BIOS_1` and `unk_34BD0`.
They are the honest answer and **must not be silenced**: our libcd has one
`BIOS` module and that 592-byte run is its whole `.rdata`, but the game's SDK
split BIOS's *text* into BIOS_1/2/3 plus unk_34BD0 and unk_362B4 while
leaving the `.rdata` in one run, so no single text subsegment owns it. Naming
the run after either module would make the warning go away by asserting
something false. That the hint names `unk_34BD0` at all is yet another
confirmation that it is a libcd BIOS sibling.

So the steady state is three messages: two that correctly flag an
unresolvable ownership, and one known-false `.sbss` jumptable.

## Library data symbols recovered

`splat/slus_006.14_symbol_addrs.txt` gained 42 real PSY-Q names from the
modules' own export tables, all off zero-mismatch matches: the eight SNMAIN
descriptor words, 13 `_spu_*` from S_INI, 11 from SPU, 3 from S_RMP,
libetc's `Hcount`, libcd's `CD_nopen`, libc's `_ctype_`, and the four libgte
tables.

libcd/BIOS also exports `CD_com` at 0x8007AED1, which this project already
calls `gCdLastCommand`; the project name was kept. Worth knowing when reading
PSY-Q sources against this binary.

### The libgte tables, verified numerically as well as by content

- **`rsin_tbl`** (0x8007B2CC): 1025 `u16` = `round(4096 * sin(i*pi/2048))` for
  i in 0..1024, **max error 0** -- not 0.50; the earlier figure came from
  comparing against floats. Seven further entries are clamped to 4096 so a
  lookup may overshoot the quarter wave.
- **`SQRT`** (0x8007BAE0): 192 `u16`, 4096..8175, one octave of mantissa.
- **`rcossin_tbl`** (0x8007BF70): 16384 bytes, 4092 distinct invariant words.
- **`ratan_tbl`** (0x8007FF70): 1025 `u16`, 0..512. This is the table this doc
  used to describe as "1025 monotonic u16, 0..512; fits neither sine nor a
  linear ramp (best linear fit is off by 47)". It is RATAN's arctangent table,
  and `psyq/libgte/RATAN.s` indexes it.

**GEO_00 and GEO_01 reach `rsin_tbl` through bases biased by -0x800, -0x1000
and -0x1800** to select a quadrant. Those biased bases land inside the libspu
and libetc data runs, which is why splat invents symbols at 0x4D2CC, 0x4DACC
and 0x4E2CC. They are aliases of `rsin_tbl`, not objects, and an ownership
scan that trusts them will report libgte "intruding" into libspu and libetc.
It does not.

## The 90 KB blob was not what this doc said it was

0x3E01C-0x541D0 used to be one `bin` blob described as "mixed" game data with
a guessed internal table. Its **last third is PSY-Q library `.data`**. The
whole range is now eleven named subsegments, every boundary a multiple of 4
(which `subalign: 4` requires):

| file range | bytes | subsegment |
|---|---|---|
| 0x3E01C-0x3E040 | 36 | `psyq/libsn_data` |
| 0x3E040-0x4CCEC | 60,588 | `game_data` |
| 0x4CCEC-0x4D4B8 | 1,996 | `psyq/libspu_data` |
| 0x4D4B8-0x4E618 | 4,448 | `psyq/libetc_data` |
| 0x4E618-0x4EACC | 1,204 | `psyq/libcd_data` |
| 0x4EACC-0x53F74 | 21,672 | `psyq/libgte_data` |
| 0x53F74-0x540E4 | 368 | `psyq/libgpu_data` |
| 0x540E4-0x540EC | 8 | `psyq/libcard_data` |
| 0x540EC-0x54170 | 132 | `psyq/libc_data` |
| 0x54170-0x54194 | 36 | `psyq/libapi_data` |
| 0x54194-0x541D0 | 60 | `game_gp` |

29,900 of the 90,548 bytes -- a third of what this doc called the largest
remaining data problem -- are library data, now placed at module granularity.

### Two data groups, and the ordering rule

The libraries have data in *two* places: `.rdata` at 0x69F4-0x7130, ahead of
the text, and `.data` at 0x4CCEC-0x54194, after it. `_69F4` lays out as

```
0x69F4   rodata  psyq/libspu/SPU ... psyq/libgpu/SYS   (13 subsegments)
0x7130   text    everything
0x3E01C  data    psyq/libsn_data ... psyq/libapi_data, game_gp
0x541D0  rodata  names        (the whole name table, csrc/str/names.c)
0x5426C  data    gp_data, tail_unloaded
```

**The ordering rule, corrected.** This doc and two comments in
`slus_006.14.yaml` used to say that `ld_legacy_generation` groups a segment's
subsegments by section, that the `.data` group is emitted before the
`.rodata` group, and therefore that `.data` and `.rodata` subsegments cannot
be interleaved. **That is false in splat 0.50.1.** Explicitly declared
subsegments are emitted in declared *offset* order whatever their section,
and `_X_SECTION_START`/`_END` are only brackets dropped at the first and last
occurrence of each section. Three independent checks, all byte-identical:

- retyping `psyq/libcd` from `data` to `rodata`, between two `data`
  subsegments, changes nothing but the label position;
- merging the 0x800, 0xE30 and 0xE7C segments into one segment of four
  subsegments -- one `rodata` plus three dotted `.rodata` naming three
  *different* C units -- builds byte-identical;
- the layout above already interleaves: `data` at 0x3E01C, `rodata` at
  0x541D0, `data` at 0x54250, all in one segment.

What **is** grouped is the non-primary sections of `asm`/`c` subsegments:
their `.data`/`.rodata`/`.bss` come out in blocks after the text group. That
is the real constraint, and it is the one that bites -- an object has exactly
one `.rodata`, and a dotted `.rodata` subsegment can pull it to exactly one
offset, so a unit owning two *separated* `.rodata` runs must be cut into two
units. That is the `unk_1C540` case in `docs/open-matching-problems.md`, and
it is unaffected by any of the above.

Two consequences. First, the fifteen segments covering 0x800-0x69F4 exist
only to satisfy a constraint that is not there; they are left alone because
byte-identity is what matters, but do not add a segment just to put a
`.rodata` next to a `data`. Second, the three names the config used to hold
back -- `max`, `zu`, `kid` at 0x54254/0x5425C/0x54264, behind a 4-byte
pointer at 0x54250 -- split in place with no new segment, so the name list is
now nineteen and complete.

### What is left inside `game_data`

59 KB, still one subsegment, **666 objects**. Its internal structure is real
but **not pinned to the byte**, so it is recorded here rather than guessed at
in the yaml: pointer/descriptor tables near the start, three packed Shift-JIS
dialogue pools (roughly 0x3EA1C-0x40041C, 0x4101C-0x4161C and 0x4261C-0x4481C
at 512-byte resolution), and from ~0x47000 a long run of uniform fixed-size
records with an 8-byte-entry directory around 0x4CC48 pointing into
0x80077xxx-0x80079xxx. The run ends at 0x4CCEC, which is exactly where
libspu's data begins -- the one boundary in here that *is* byte-exact.

#### The ownership signal here is GOOD, which is the opposite of the town overlay

Measured 2026-11-14. `docs/owning-data.md` §8 records that `ovl_town_data` has
no unit oracle at all -- no asserts, no `$gp`, and a reference signal that is
noise, with "no contiguous three-way assignment" fitting. **This container is
not in that position**, and the difference decides how its carve is worked.

The constraint to satisfy is that one object file contributes one `.data` run,
and the linker lays those runs out in link order -- so an assignment of
objects to units must be NON-DECREASING in link order when read in address
order. Take the 127 objects of `game_data` that exactly one C unit mentions,
in address order, and ask for the longest non-decreasing run of link indices:

| | objects | longest non-decreasing run |
|---|---|---|
| observed | 127 | **116** |
| same multiset, shuffled (2,000 trials) | 127 | mean 68.2, p95 71, max 76 |

116 against a null whose maximum over two thousand shuffles is 76. The
reference signal in this container carries the layout, and `use is not
ownership` -- true as a statement about any single object -- does not make the
aggregate useless here the way it does in town.

#### What the eleven exceptions are, and why they are the actual blocker

Eight of the eleven objects that fall outside that run are `unk_72A8`, at
0x8007216C, 0x800721C0, 0x8007386C, 0x80073AAC, 0x80073B54, 0x80073DC4,
0x800776FC and 0x80077784 -- all near the TOP of the region, while
`unk_72A8`'s other 54 single-referenced objects are at the bottom,
0x8006A847..0x8006B230. The remaining three are `D_8006ADC0` (`unk_264E0`),
`D_80071FF0` (`unk_24FCC`) and `D_800777AC` (`unk_108F0`).

That is not a flaw in the signal. **`unk_72A8` is not a translation unit.** It
is 305 functions and 8,464 words, and so is `unk_1C540` at 268 functions --
the two of them are a third of the game's code. Both are carves by ADDRESSING
MODE (`docs/addressing-and-translation-units.md`), not by original file, and a
unit that is really a dozen original units has its data scattered through the
region accordingly. Giving a 305-function object file one contiguous `.data`
window would be a statement about the original source that is almost certainly
false, and would have to be undone later.

#### THE TEXT CARVE IS NOT AVAILABLE, and that is measured

An earlier version of this section said the order of work was to carve the
text into its original translation units first, using the `$gp` membership
rule, and that `tools/tuscan.py` needed one new input to do it. The input was
built -- `tuscan.py --source objects` reads the modes out of `build/csrc/*.o`,
which is sound because the container is byte-identical -- and the answer came
back too thin to carve with:

    $ python3 tools/tuscan.py --source objects --cores
    67 of 1053 function(s) make a $gp-relative access at all, naming 68
    symbol(s) -> 29 PROVEN core(s)
    89 of 1053 function(s) are inside a proven core (8.5%)
    widest gap BETWEEN two cores: 170 function(s)

**Only 67 of 1053 functions make a `$gp`-relative access at all.** The rule is
sound and the tool now runs, but most functions never touch small data, so
most of them carry no evidence of membership in anything. 29 proven cores
against roughly 43 units, 8.5% of the block covered, and boundary windows up
to 170 functions wide.

Read `--cores` and not the default report. The default grouping uses negative
evidence too -- a function addressing a symbol absolutely is not in the unit
that defines it -- and on this container that yields 43 runs and then reports
10 symbols reached `$gp`-relatively from TWO of them, which is a
contradiction. It is not a bug: the original source could declare a symbol
`extern` above its own definition and get the absolute form inside the
defining unit, which `docs/addressing-and-translation-units.md` already proves
happens here. `--cores` uses positive evidence only and reports the gaps as
gaps.

#### So `game_data` is not blocked -- it is a data-only unit, like the gp region

With no carve available, the choice is the one `csrc/game_gp.c` and
`csrc/gp_data.c` already made twice: put the definitions in a unit that holds
no code, where they cannot perturb a relocation, and say plainly that the
per-unit attribution is undetermined rather than encoding a guess in the yaml.
A data-only unit is also FORCED here for a second reason, the same one that
forced `-G0` on `csrc/gp_data.c`: defining a small `game_data` object inside
one of the `-G8` units would make its size known, and a `.extern` of 8 bytes
or fewer at `-G8` asks gas for a GPREL16 -- which cannot reach, since
`game_data` starts 0x16154 below `_gp` and GPREL16 spans +/-32K.

The 116-of-127 reference signal above is still worth having; it is just not
load-bearing for the landing. It is the thing to check a later per-unit carve
against, once there is one.

#### Do not start at the bottom expecting `gprel_72A8`

`gprel_72A8` is first in link order but has no single-referenced object
anywhere in `game_data`; the first 35 belong to `unk_72A8`. The region's first
byte, 0x3E040 / 0x8006A840, is therefore `unk_72A8`'s, which means the first
tranche is one of the two units that should be carved before it is written.

## The five unidentified code blocks

5,556 bytes in five gaps between placed PSY-Q modules. They are now `asm`
subsegments, which is both what the bytes are and what stops splat calling
them unknown:

| subsegment | file range | bytes |
|---|---|---|
| `psyq/libspu/unk_31CC4` | 0x31CC4-0x31F8C | 712 |
| `psyq/libspu/unk_32934` | 0x32934-0x32F40 | 1,548 |
| `psyq/libspu/unk_33234` | 0x33234-0x33618 | 996 |
| `psyq/libcd/unk_34BD0` | 0x34BD0-0x353D8 | 2,056 |
| `psyq/libcd/unk_362B4` | 0x362B4-0x363A8 | 244 |

`azure libscan psyq` names none of them at any `--min-invariant`, down to 2,
and no run of unplaced modules in our libraries matches any of them by size
unambiguously. The **library** is nevertheless settled, not guessed from link
position: each block was checked against the data it reaches, and in every
case that data is private state shared only with modules of one library.

- `unk_31CC4` -- `_spu_RQmask` and `_spu_env` with S_INI and S_SK, `_spu_RXX`
  with nine more libspu modules.
- `unk_32934` -- `_spu_voice_centerNote` with `unk_33234`, `_spu_RXX` as
  above. Its jump tables sit in libspu's own `.data` at 0x6A7C and 0x6AA8 and
  point back into this block.
- `unk_33234` -- `_spu_mem_mode_plus` with SPU, S_CRWA, S_M_INIT, S_M_UTIL
  and S_STSA.
- `unk_34BD0` -- 10 of its 12 symbols with libcd/BIOS_1, including
  `CD_nopen`, and 0x8007AF78 and 0x8007B078 with nothing else at all.
- `unk_362B4` -- 0x8007B178 and 0x8007B191 with libcd/BIOS_1/2/3, and
  0x8007B15C with nothing else at all.

Link position agrees with all five. BIOS_1/2/3 are themselves marked
UNVERIFIED in the yaml as a newer-SDK subdivision of the single `BIOS` module
our libcd has; `unk_34BD0` and `unk_362B4` are two more pieces of that same
subdivision.

## The smaller pieces

**All four `str_*` rows are gone as of 2026-11-14**; they are C units now,
and the table below is what is left. `tbl_660C` and `smf_magic` are `rodata`
subsegments because they sit among the C units' jump-table `.rodata` runs;
the other three are `data`.

| subsegment | file range | bytes | objects | what |
|---|---|---|---|---|
| `tbl_660C` | 0x660C-0x6614 | 8 | 1 | a `u16` array `{1,2,3}`, `D_80032E0C`, behind the item pool |
| `smf_magic` | 0x68FC-0x690C | 16 | 1 | `MThd`/`MTrk` with zero lengths |
| `game_data` | 0x3E040-0x4CCEC | 60,588 | 666 | see the section above |
| `gp_sbss` | 0x54C38-0x55000 | 968 | 67 | `.sbss` and the 0x800-rounded tail of `t_size`, all zero; NOT ownable in C at any `-G` |

And the seven that are DONE:

| subsegment | file range | bytes | objects | C unit |
|---|---|---|---|---|
| `str_pool_800` | 0x800-0xDC0 | 1,472 | 107 | `csrc/str/speakers.c` |
| `str_pool_EFC` | 0xEFC-0x1DF8 | 3,836 | 103 | `csrc/str/spells.c` |
| `str_pool_1E10` | 0x1E10-0x2164 | 852 | 36 | `csrc/str/commands.c` |
| the item pool | 0x2810-0x660C | 15,868 | 405 | `csrc/str/items.c` |
| `names` | 0x541D0-0x5426C | 156 | 20 | `csrc/str/names.c` |
| `game_gp` | 0x54194-0x541D0 | 60 | 10 | `csrc/game_gp.c` |
| `gp_data` (`.sdata` part) | 0x5426C-0x54C38 | 2,508 | 287 | `csrc/gp_data.c` |

`gp_data` is **not** the game's alone, which is why it is no longer called
`game_gp_data` -- libapi/PATCH's 16-byte `.sbss` is in there, and it is the
source of the permanent 0x54D98 diagnostic above.

### `game_gp` is DONE, and it proved the thing `game_data` was argued into

**Landed 2026-11-14 as `csrc/game_gp.c`**, 60 bytes, 10 objects, a
`.sdata` subsegment. What it cost, and the two results worth carrying:

**1. `unk_72A8` is PROVABLY more than one original translation unit, and the
proof is two relocations sixteen bytes apart.** Our build reproduces the
reference byte for byte, so the relocation shapes in our own objects are the
original's:

    build/csrc/unk_72A8.o
      000008b8  R_MIPS_HI16     D_800809A8      <- absolute
      000013a4  R_MIPS_GPREL16  D_800809B8      <- $gp-relative

ASPSX could only use `$gp` for a symbol whose size it knew, which means only
for a symbol defined in the unit being assembled. So the original unit holding
the function at +0x13A4 DEFINED `D_800809B8`, and the original unit holding
the `ClearImage(D_800809A8, ...)` call at +0x8B8 did NOT define
`D_800809A8` -- two symbols sixteen bytes apart in one 60-byte run, both
reached from inside our `unk_72A8`. One object file contributes one contiguous
`.data` run, so no assignment of this run to our 38 units can be right.
`csrc/gprel_72A8.c` is in the same position for `D_80080994` (absolute, with
`.extern D_80080994, 8` in its own assembly) against `D_8008099C`
(`$gp`-relative).

That is the statistical argument this doc makes for `game_data`, made as a
proof instead, on a run small enough to read end to end. **It also sharpens
it:** the obstacle is not that the reference signal is weak, it is that two of
our 38 units are not units.

**2. A data-only unit cannot perturb a reference, and that is why it is the
right shape while the carve is pending.** Putting these definitions into
`gprel_72A8.c` and `unk_72A8.c` instead would FLIP their absolute neighbours
to GPREL16 -- gas goes `$gp`-relative for a symbol it can see is small and
local, and `D_800809A8`, `D_800809B0`, `D_800809C0`, `D_800809C4` and
`D_80080994` all have to stay absolute. A unit with no code has no relocations
of its own, so the only thing that changes is which object supplies the bytes.
The three symbols those units hold as tentative definitions
(`u32 D_8008099C;`, `u32 D_800809B8;`, `u32 D_800809BC;`) stay COMMON in their
objects and resolve to the new definitions, exactly as they resolved to
`asm/data/game_gp.data.o` before; every relocation in both consumers was
checked unchanged after the landing.

**3. `.sdata` works, and the option is in.**
`auto_link_sections: [".data", ".rodata", ".bss", ".sdata"]` is now in the
yaml. The unit is `-G8` -- the default for `csrc/*.c`, and what the original
used -- so all ten objects are 8 bytes or fewer and cc1psx emits them under
`.sdata`; the explicit `.sdata` subsegment places that section at the declared
offset, in declared order, next to `csrc/str/names.o(.rodata)` above it. A
`-G0` unit emitting to `.data` would also have linked, but it would put the
run in a section the original did not use.

The run is writable, so it is `.sdata` and not `.rdata`: `csrc/gprel_72A8.c`
has `return D_8008099C = a;`. That is what rules out the tempting reading --
that `game_gp` plus `csrc/str/names.c`'s 156 read-only bytes immediately above
it form one contiguous `.rdata` run ending where `gp_data` begins.

### `gp_data` is DONE too, and the alignment rule it turned up is general

All measured 2026-11-14.

**The ownership oracle costs one command.** The `$gp` membership rule says a
unit that reaches a global `$gp`-relatively is the unit that defined it
(`docs/addressing-and-translation-units.md`), and for these two subsegments
it does not need `tools/tuscan.py` or any disassembly -- our build reproduces
the original's addressing modes byte for byte, so the relocations in our own
objects are the answer:

    for o in build/csrc/*.o; do
      mips64-linux-gnu-objdump -r $o | grep GPREL16
    done

**68 of the 359 objects are named that way, and they form TWO interleaved
monotone runs, not one.** Read in address order the link indices look like
noise -- the longest non-decreasing run is 41 of 68 -- until you notice every
unit owns objects in two separated bands:

| unit | link idx | low band | high band |
|---|---|---|---|
| `gprel_72A8` | 0 | 0x8008099C | -- |
| `gprel_10308` | 3 | 0x80080AA4..0x80080AB0 | -- |
| `unk_108F0` | 4 | 0x80080ABC..0x80080ABE | -- |
| `gprel_11B4C` | 9 | 0x80080AD0 | 0x800814D0 |
| `unk_11F0C` | 12 | 0x80080AD2 | 0x800814D4 |
| `unk_12A70` | 14 | -- | 0x800814B0..0x800814B4 |
| `gprel_17B7C` | 24 | 0x80080AF3 | 0x80081508 |
| `unk_19434` | 27 | 0x80080B08..0x80080B0A | -- |
| `gprel_1AB38` | 28 | -- | 0x80081518..0x80081526 |
| `unk_1AF38` | 30 | 0x80080B10 | 0x80081530 |
| `unk_1C540` | 31 | 0x80080B1B | 0x80081558 |

Each band is monotone in link order on its own. They are `.sdata` and
`.sbss`, which are two output sections, and one object file contributes one
run to each -- so this region is two partitions to find, not one.

**Where the boundary is, measured rather than assumed.** The last non-zero
byte in `gp_data` is at file 0x54C35, vram 0x80081435, and everything from
there to the end of the image is zero. That confirms the split this doc
already recorded from the library scan:

| | file | vram | bytes | non-zero |
|---|---|---|---|---|
| `.sdata` part of `gp_data` | 0x5426C-0x54C38 | 0x80080A6C-0x80081438 | 2,508 | 1,333 |
| `.sbss` part | 0x54C38-0x54DC0 | 0x80081438-0x800815C0 | 392 | 0 |
| zero fill to the image end | 0x54DC0-0x55000 | 0x800815C0-0x80081800 | 576 | 0 |

**So the ownable part is 2,508 bytes, not 3,476, and `.sbss` is why.** A C
definition of an `.sbss` object is a tentative definition: it goes to
`.comm`/`.sbss`, which is NOBITS and contributes no file bytes. Own those in
C and the container comes out 392 bytes short. The 968 bytes above 0x54C38
should stay a `data` subsegment of zeros, and their symbols stay in
`slus_006.14_undefined_syms_auto.txt` where they are now.

**Landed 2026-11-14 as `csrc/gp_data.c`**, 287 objects, 2,508 bytes, the same
data-only shape as `game_gp` and for the same reason -- `unk_1C540` holds
sixteen of the `$gp`-named objects here and is a 268-function
addressing-mode carve, so that file's proof applies unchanged. The 968 bytes
above it are now the `gp_sbss` subsegment.

**The boundary was derived twice, independently, and the two agree.** The
last non-zero byte in the old blob is at rom 0x54C35; `dataverify.py`'s NEXT
YAML BOUNDARY for the definitions is 0x54C38 -- one number from the image,
one from the compiled object's section size.

**`tools/decomp/dataverify.py` needed one `WINDOWS` entry and nothing else.**
`("gp_data", 0x5426C, 0x80080A6C, "data/slus_006.14")`, and the tool written
for the town overlay works on the executable unchanged: 287 objects, layout
ok, bytes OK, three relocations listed for reading against the listing.
`D_80080A90` holds `.word D_8014A000`; `D_80080B34` and `D_80080B40` both
hold `.word D_80080B3C`, which is a scalar here and so needs its address
taken -- `(u32)&D_80080B3C`, not a cast of its value, which is §8 rule 6.

**NO ARRAY MAY START AT AN ADDRESS THAT IS NOT 4-ALIGNED, and this is
general.** gcc 2.7.2's `DATA_ALIGNMENT` (mips.h) raises every `ARRAY_TYPE`,
`RECORD_TYPE` and `UNION_TYPE` to `BITS_PER_WORD`.
`__attribute__((aligned(1)))` does **not** lower it again -- measured, the
attribute is simply ignored and `.align 2` is emitted anyway. Scalars keep
their natural alignment, so a multi-element object at a non-4-aligned address
has to be written as one scalar per element, with invented `D_` names for the
ones after the first; three objects here needed it (`D_80080A8A`,
`D_80080AD2`, `D_80080B0A`). The failure mode is the one `dataverify.py`'s
`layout` column exists for: the drift appears at the object AFTER the one
with the wrong type.

**`-G0`, and this is the one top-level `csrc` unit that is.** The run
interleaves objects of 8 bytes and fewer with 24 larger ones, and at the
`-G8` the original used those two groups go to `.sdata` and `.data` -- two
sections, which one subsegment cannot place. `csrc/game_gp.c` is not in the
Makefile rule: all of its objects are small, so it stays `-G8` and `.sdata`
like the original. The cost here is that the section name is not the
original's; the bytes and their order are.

**The types are the listing's element widths, not the tree's declarations,
and that is a decision.** §8 rule 2 says to use the declared type because a
mismatch is a `conflicting types` error and that error is the gate. Here the
definitions are in a unit of their own, so no such gate exists, and adopting
the declarations was measured rather than skipped: of the 96 objects the tree
declares, 32 are scalars whose size equals the span and 19 are arrays that
divide it, but **22 are struct types** (`Unk41284`, `CVECTOR`) needing a
field-by-field layout, **21 are scalars SMALLER than their span** and would
each need a value object plus a pad object, and **2 already have conflicting
declarations**. That is 45 hand-written layouts, each a chance to be
byte-identical and wrong, for definitions that move into their real units
when the text carve lands. So all 96 declarations are recorded in comments
above the objects they describe, and a later pass has them in place.

`smf_magic`'s owner is no longer unknown: `csrc/unk_2879C.c` calls
`func_80058800(D_800330FC, ..., 4)`, a 4-byte compare, so these are two magic
strings a sequence loader validates against -- and unk_2879C is the very unit
whose jump tables end at 0x68FC, so it is that unit's own `.rodata`. Folding
it into the C file would need the compiler to emit the array after the jump
tables, which is not worth 16 bytes to find out; it stays a `rodata`
subsegment.

The pool at **0x2810-0x57F8 is done**: 322 `shiftjis` subsegments under
`str/item/`, 12,266 bytes.

## How to split a text pool: SOLVED, and the pools are all done

**Superseded 2026-11-14.** This section used to describe converting the
`str_*` blobs to `shiftjis` subsegments. That route is gone: the `shiftjis`
subsegment type is no longer used in this container at all, and every pool is
a C unit whose strings cc1psx emits itself, which is `docs/owning-data.md`
§4.7's route. What is below is what the conversions actually cost.

### The coverage test, which is what makes the split safe

Whether a pool is splittable is decidable, and the test does not depend on any
one string detector's recall:

> Take the union of every detector's strings over the blob. Mark their bytes
> covered. **Every uncovered byte must be zero.** If one is not, it is either a
> string you missed or real data, and it is printed for you to look at.

**For all three 2026-11-14 pools there was a second, stronger test available,
and it is the one to reach for first.** Every string in each pool is pointed
at from a table inside `game_data`, so the object boundaries can be read out
of the image rather than inferred from a detector:

| pool | NUL-walked starts | pointer targets from `game_data` | agree | uncovered non-zero |
|---|---|---|---|---|
| `str_pool_800` names, 0x848-0xD94 | 100 | 100 | yes | 0 |
| `str_pool_EFC`, 0xEFC-0x1DD8 | 101 | 101 | yes | 0 |
| `str_pool_1E10`, 0x1E10-0x2150 | 35 | 35 | yes | 0 |

Two independent derivations landing on the same set is what makes this safe,
and it mattered: **exactly one target per pool is a relocated symbol and the
rest are bare `.word` literals**, so a wrong boundary would have changed
nothing the linker could complain about. It would have been silent.

The old "what the test says" table is retired with the pools. One of its three
rows was wrong on its own terms: it listed `str_pool_EFC` as blocked partly by
"a `0x8004D0xx` pointer table at 0x1DF9 (stride 0x10)". There is no such
table. 0x1DF8 is `jtbl_8002E5F8`, func_8004CECC's six-entry jump table, in the
NEXT subsegment, read one byte early.

### Rules that carried over

- **`subalign: 4` is the real constraint on any subsegment boundary**, and it
  is not the section-order rule this section used to state (see "the ordering
  rule, corrected" above). Every boundary must be a multiple of 4.
- **The unit must be `-G0`**, or objects of 8 bytes or fewer go to `.sdata`
  and the linker script does not collect it. The `build/csrc/str/%.o` rules in
  the Makefile do this.
- **Spell every non-ASCII byte as `\xNN`**, never as a bare character after an
  escape.
- **The U+3000 IDEOGRAPHIC SPACE is load-bearing and invisible.**
- **A C file can only put ONE symbol at an address.** Where splat had invented
  a `D_` name beside the readable one, the readable name goes in
  `splat/slus_006.14_symbol_addrs.txt` with `defined:True` -- that is also what
  takes the symbol OUT of `slus_006.14_undefined_syms_auto.txt`, which is the
  measure of the thing. All three pools' symbols are out of it now.

### Two rules learned on 2026-11-14

**TRAILING PADDING GOES BOTH WAYS, and which applies depends on what follows
the last object.** `docs/owning-data.md` §4.6 says the last entry carries the
pool's own padding in an explicit array size, because nothing follows it to
align against. That is half the rule.

* `csrc/str/spells.c` is the §4.6 case. Its last object, `D_8002E5E8`, is the
  three words the code reads, and the pool's fourth word is padding. 3,832 is
  already 4-aligned, so SUBALIGN(4) supplies nothing, and the unit has to
  declare `[4]`.
* `csrc/str/speakers.c` is the opposite. Its last object is a 10-byte
  byte-aligned struct, and a 2-byte pad object after it made the unit 1,474
  rather than 1,472 -- cc1psx's `.align 2` ahead of that object had ALREADY
  supplied the two bytes, and the object then added two more. Dropping it
  leaves `.rodata` at 1,470 and SUBALIGN(4) in the generated linker script
  supplies the rest.

So: if the last object ends 4-aligned, declare the pad; if it does not, do
not. The gate is the same either way --
`mips64-linux-gnu-size -A build/csrc/str/<name>.o` against the subsegment
span.

**`const` CANNOT ALWAYS BE PROPAGATED TO THE CONSUMERS, AND THIS IS MEASURED.**
A pool object must be defined `const` or cc1psx emits it under `.data`. When
the object has a `D_` name other units reach, the obvious follow-through is to
make the shared declaration `const` too, as `csrc/str/names.c`'s `D_80080A50`
did. For `D_8002E5D8` and `D_8002E5E8` that **changes gcc 2.7.2's codegen**:
six units copy them out through `u32 *` locals, and const-qualifying those
locals so the header could take the const left `csrc/unk_1C540.c` 8 bytes
short -- shifting every jump table behind it -- and broke main.bin at
0x25D0A8 and town.bin at 0x031828.

The fix is to keep the const inside the defining unit: `csrc/str/spells.c`
includes `include/types.h` rather than `include/common.h`, so the `const`
definition and `include/structs.h`'s `extern u32 D_8002E5D8[];` are never in
one translation unit. `csrc/str/speakers.c` did NOT need this -- its five
named objects are only read through explicit casts and struct assignments, so
`include/structs.h` takes the `const` there and all four containers stay
byte-identical. **Try the header first; fall back to types.h when the gate
says no.**

### What it cost before

`0x002810` was done on 2026-10-30: 322 strings, 12,266 bytes, split at
0x57F8, and the tail folded into `csrc/str/items.c` later.

- **The pool had to be cut short.** Five strings start 2-aligned -- the
  composed-name nouns at 0x57FA, 0x586E, 0x596A, 0x5A62 and 0x60C6 -- and
  `subalign: 4` pads every input section to 4. **Check `offset % 4 == 0` on
  every boundary before splitting.** The same rule governed every boundary in
  the 0x3E01C rewrite above, and is why `psyq/libsn_data` ends at 0x3E040
  rather than at 0x3E03E, where `unk_72A8.c`'s biased array base points.
- **46 of the 412 strings came from the fallback scan, not `findstr`.** They
  are the Japanese **unidentified-item** names, never translated for the US
  release and still in the binary, built as colour-plus-noun pairs
  (`紺色の草`, `金色の種`, `薄い巻き物`, `四角いメガネ`).

`main.bin`'s own pool, 0x20800-0x21268, went the whole way on 2026-11-09 and
is the cleanest example of the shape: 134 strings, 2,664 bytes, **one**
`.rodata` subsegment and no `data` chunks at all (`csrc/str/monsters.c`). See
`docs/main-bin-data.md`.

## What is left, in order

1. **`game_data`, 60,588 bytes, 666 objects** -- blocked on carving the text
   into original translation units first; see the section above for the
   measurement and the one tool it needs.
2. **Nothing, in the gp region.** `game_gp` and `gp_data`'s `.sdata` part
   both landed on 2026-11-14. `gp_sbss`, the 968 bytes above 0x54C38, is not
   work: a C definition cannot emit it at any `-G`.
3. **`tbl_660C` and `smf_magic`, 24 bytes** -- not blocked on anything, just
   not worth a translation unit each. Folding `smf_magic` into
   `csrc/unk_2879C.c` would need the compiler to emit the array after that
   unit's jump tables.
