# The data in slus_006.14

Companion to `docs/where-we-are.md`, which covers the code. Numbers here were
re-measured on 2026-11-01 against `data/slus_006.14`
(sha1 e6bfbb95ff6676899e077481221d73ddd4d3bf52); re-measure rather than trust
them.

**`splat split` now reports 0 bytes unknown.** Every byte of the file belongs
to a named subsegment, and `make containers` stays byte-identical on all four
containers. What changed on 2026-11-01, and the evidence for it, is below.

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

| type | bytes | % of file | % of image |
|---|---|---|---|
| `c` | 167,596 | 31.97% | 48.42% |
| `data` | 105,692 | 20.16% | 30.54% |
| `asm` | 57,408 | 10.95% | 16.59% |
| `shiftjis` | 14,016 | 2.67% | 4.05% |
| `.rodata` (jump tables in C units) | 1,400 | 0.27% | 0.40% |
| `bin` (`tail_unloaded`, outside the image) | 176,128 | 33.59% | -- |
| `header` (outside the image) | 2,048 | 0.39% | -- |
| **unknown** | **0** | **0.00%** | **0.00%** |

Note that splat's own report line does not count `data` in its "Split N KB in
defined segments" total, so that line reads 79.57% while the accounting above
is 100.00%. The number to watch is the `unknown` line.

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

29,864 of the 90,548 bytes -- a third of what this doc called the largest
remaining data problem -- are library data, now placed at module granularity.

### Two data groups

The libraries have data in *two* places, which is a property of splat's
`ld_legacy_generation` and worth knowing before adding subsegments here. One
segment's ld script emits, in order: its leading `data` subsegments, then the
`.text` group, then the rest of its `data` subsegments, then the `.rodata`
group, then any trailing `data`. So `_69F4` lays out as

```
0x69F4   data   psyq/libspu, psyq/libetc, psyq/libcd, psyq/libgpu   (strings,
                                                       jump tables: .rodata)
0x7130   text   everything
0x3E01C  data   psyq/libsn_data ... psyq/libapi_data, game_gp
0x541D0  rodata str/name/*
0x54250  data   game_gp_data, tail_unloaded
```

`data` subsegments keep their relative order, so the 0x3E01C range can be cut
into as many pieces as the evidence supports. The `psyq/<lib>` names were
already taken by the first group, hence `psyq/<lib>_data` for the second.

### What is left inside `game_data`

59 KB, still one subsegment. Its internal structure is real but **not pinned
to the byte**, so it is recorded here rather than guessed at in the yaml:
pointer/descriptor tables near the start, three packed Shift-JIS dialogue
pools (roughly 0x3EA1C-0x40041C, 0x4101C-0x4161C and 0x4261C-0x4481C at
512-byte resolution), and from ~0x47000 a long run of uniform fixed-size
records with an 8-byte-entry directory around 0x4CC48 pointing into
0x80077xxx-0x80079xxx. The run ends at 0x4CCEC, which is exactly where
libspu's data begins -- the one boundary in here that *is* byte-exact.

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

| subsegment | file range | bytes | what |
|---|---|---|---|
| `str_pool_800` | 0x800-0xDC0 | 1,472 | 72% Shift-JIS: debug format strings, then a character-name table from 0x848 |
| `str_pool_EFC` | 0xEFC-0x1DF8 | 3,836 | 90% Shift-JIS, plus a `0x8004D0xx` pointer table at 0x1DF9 (stride 0x10) |
| `str_pool_1E10` | 0x1E10-0x2164 | 852 | same pool, after the jump-table segments |
| `str_item_tail` | 0x57F8-0x6614 | 3,612 | tail of the item pool; 90 strings held back, see below |
| `smf_magic` | 0x68FC-0x690C | 16 | `MThd`/`MTrk` with zero lengths |
| `game_gp` | 0x54194-0x541D0 | 60 | the `_gp` anchor; `_gp` = 0x80080994 is its first word |
| `game_gp_data` | 0x54250-0x55000 | 3,504 | `.data`/`.sdata`/`.sbss` above `_gp`, then zero fill |

`smf_magic`'s owner is no longer unknown: `csrc/unk_2879C.c` calls
`func_80058800(D_800330FC, ..., 4)`, a 4-byte compare, so these are two magic
strings a sequence loader validates against -- and unk_2879C is the very unit
whose jump tables end at 0x68FC, so it is that unit's own `.rodata`. Folding
it into the C file would need the compiler to emit the array after the jump
tables, which is not worth 16 bytes to find out; it stays a `data`
subsegment.

The pool at **0x2810-0x57F8 is done**: 322 `shiftjis` subsegments under
`str/item/`, 12,266 bytes.

## How to split a text pool (and when you cannot)

The four `str_*` subsegments above are `data`, not `shiftjis`. That is enough
for splat to account for them, and spimdisasm finds the ASCII strings, but the
Shift-JIS comes out as `.short`/`.word` rather than readable text. Turning
them into `shiftjis` subsegments is still worth doing; the rules have not
changed.

`ld_legacy_generation` groups a segment's subsegments by section, and **the
`.data` group is emitted before the `.rodata` group**. A `shiftjis`
subsegment is `.rodata` and a `bin` or `data` subsegment is `.data`, so:

- You **can** replace a whole blob with `shiftjis` subsegments when it is
  already its own segment. Then the `.rodata` group starts exactly at the
  segment's vram and nothing moves. This is how `str/credits/*` landed.
- You **cannot** interleave them. Any non-text bytes in the middle of the pool
  have to go in a segment of their own, the way the jump-table runs in
  `slus_006.14.yaml` do. Non-text bytes at the *end* of a blob also need their
  own segment, because a trailing `.data` subsegment would be emitted *first*.
- A `shiftjis` subsegment runs to the next subsegment's offset and zero-fills
  the remainder, so trailing NUL padding needs no entry of its own -- it is
  absorbed as each string's `.fill`.

### The coverage test

Whether a pool is splittable is decidable, and the test does not depend on any
one string detector's recall:

> Take the union of every detector's strings over the blob. Mark their bytes
> covered. **Every uncovered byte must be zero.** If one is not, it is either a
> string you missed or real data, and it is printed for you to look at.

This matters because the detectors disagree. `azure findstr` targets
full-width Latin and skips kanji runs; a kana/kanji-aware byte-pair scanner
finds 1,398 bytes *fewer* overall because it breaks runs at the script's
control bytes (0x0A newline, and the 0x11/0x08 pair) and can drift out of
phase. Neither is a ceiling, and Ghidra's string model does better than
either -- so use the union plus the zero test, not a single tool's output.
Applied to the credits block the test came back clean (160 uncovered bytes,
all zero), which is why that split was safe.

### What the test says about the remaining pools

| blob | uncovered non-zero bytes | blocker |
|---|---|---|
| `str_item_tail` 0x57F8-0x6614 | 11 | 8 bytes of Japanese at 0x5194 that both detectors missed, and a `u16` array `{1,2,3}` in the last 8 bytes (0x660C, already named `D_80032E0C`) |
| `str_pool_EFC` 0xEFC-0x2164 | 54 | a `0x8004D0xx` pointer table at 0x1DF9 (stride 0x10), a colour triple at 0x1DD8, and a byte ramp at 0x2151 |
| `str_pool_800` 0x800-0xDC0 | 70 | 69 bytes of ASCII `printf` formats at 0x800 (`"MESSYS  %x \n"`, `"STEP    %x \n"`, `"XASTAT %x\n"`) and a 4-pointer table at 0xD98 |

`0x002810` was done this way on 2026-10-30: 322 strings, 12,266 bytes, split
at 0x57F8. What it cost, and what the next pool will cost too:

- **The pool had to be cut short.** Five strings start 2-aligned -- the
  composed-name nouns at 0x57FA, 0x586E, 0x596A, 0x5A62 and 0x60C6 -- and
  `subalign: 4` pads every input section to 4, so a subsegment boundary that
  is not a multiple of 4 shifts everything after it. 0x57F8-0x6614 keeps the
  remaining 90 strings as one `data` subsegment; splitting them needs one
  extra segment per misalignment. **Check `offset % 4 == 0` on every boundary
  before splitting.** The same rule governed every boundary in the 0x3E01C
  rewrite above, and is why `psyq/libsn_data` ends at 0x3E040 rather than at
  0x3E03E, where `unk_72A8.c`'s biased array base points.
- **The `.shiftjis` directive could not hold the script's control bytes.** The
  descriptions embed 0x0A line breaks (and the dialogue pools in `game_data`
  also use 0x11 and 0x08), which decode to C0 controls and ended the
  directive's line mid-string. `splat/ext/shiftjis.py` now escapes any
  character below 0x20, plus `"` and `\`, and `azure shiftjis` reverses
  exactly those escapes. This was the blocker for *every* remaining pool, so
  it is now cleared for all of them.
- **46 of the 412 strings came from the fallback scan, not `findstr`.** They
  are the Japanese **unidentified-item** names, never translated for the US
  release and still in the binary, built as colour-plus-noun pairs
  (`紺色の草`, `金色の種`, `薄い巻き物`, `四角いメガネ`). They appear as
  `str/item/jp_*`, named by offset because the slug would be empty.

Next: `str_pool_EFC` (3,836 bytes + 852 after the jump tables, 54 uncovered
non-zero bytes) and `str_pool_800` (1,472, 70), then the 90 strings left in
`str_item_tail`. The dialogue pools inside `game_data` are harder -- their
entries are not aligned at all.
