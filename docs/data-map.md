# The data in slus_006.14

Companion to `docs/where-we-are.md`, which covers the code. Numbers here were
measured on 2026-10-30 against `data/slus_006.14`
(sha1 e6bfbb95ff6676899e077481221d73ddd4d3bf52); re-measure rather than trust
them.

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
the PSX loader**. They are now the `tail_unloaded` subsegment.

The boundary is exact to the byte and needs no inference: the image's last
non-zero byte is at file 0x54C35, 0x54C36-0x55000 is zero padding, and 0x55000
is non-zero immediately. `azure libscan`'s `--end` default already documented
0x55000 as "the end of the loaded image per the PS-EXE header"; the splat spec
did not until 2026-10-30, which is why `where-we-are.md` used to report the
tail as 175,552 bytes of "high-entropy data (packed assets)" inside the image.

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

## What is left

`splat split` reports 109,784 bytes (20.94%) unknown. All of it is inside the
image, in twelve `bin` blobs:

| file range | vram | bytes | what |
|---|---|---|---|
| 0x000800-0x000DC0 | 8002D000 | 1,472 | 72% Shift-JIS: debug format strings, then a character-name table from 0x848 |
| 0x000EFC-0x002164 | 8002D6FC | 4,712 | 90% Shift-JIS, plus a pointer table at 0x1DF9 |
| 0x0057F8-0x006614 | 80031FF8 | 3,612 | tail of the item pool; 90 strings held back, see below |
| 0x0068FC-0x00690C | 800330FC | 16 | `MThd`/`MTrk` -- a MIDI header, owner unknown |
| 0x0069F4-0x006B60 | 800331F4 | 364 | structured; 11 inbound refs |
| 0x031CC4-0x031F8C | 8005E4C4 | 712 | **code**, not data |
| 0x032934-0x032F40 | 8005F134 | 1,548 | **code** |
| 0x033234-0x033618 | 8005FA34 | 996 | **code** |
| 0x034BD0-0x0353D8 | 800613D0 | 2,056 | **code** |
| 0x0362B4-0x0363A8 | 80062AB4 | 244 | **code** |
| 0x03E01C-0x0541D0 | 8006A81C | 90,548 | mixed; see below |
| 0x054250-0x055000 | 80080A50 | 3,504 | `.data`/`.sdata`/`.sbss` around `_gp` |

The pool at **0x2810-0x57F8 is done**: 322 `shiftjis` subsegments under
`str/item/`, 12,266 bytes.

The five middle blobs are **5,556 bytes of library code**, not data: each is
100% plausible MIPS opcodes with real prologues (`addiu $sp, $sp, -N` / `sw
$ra`). `azure libscan psyq --min-invariant 4` places none of them, the same
negative result the two large game-code blocks gave before they were carved.
They want `asm`/`c` subsegments, not data work.

## Inside the 90 KB blob (0x3E01C-0x541D0)

This is 74% of what remains and it is not one thing. Measured by entropy, zero
density, pointer-word density and inbound auto-symbols:

| file range | what |
|---|---|
| 0x3E01C-0x3EA6C | pointer/descriptor tables -- ~110 image-pointer words per KB, 63 inbound refs |
| 0x3EA6E-0x400426 | packed dialogue pool: full-width English and some Japanese, entries **not** aligned |
| 0x40041C-0x40FAC | pointer tables and mixed records |
| 0x40FC0-0x41620 | text, 38 strings |
| 0x41620-0x426BC | structured, pointer-heavy |
| 0x426BC-0x448C6 | tutorial/help text pool |
| 0x4702C-0x4CCEC | 23,744 bytes of uniform fixed-size records, entropy ~4.8, 10.7% zero |
| 0x4CC48-... | the directory for the above: 8-byte entries whose second word points into 0x80077xxx-0x80079xxx |
| 0x4EACC-0x4F2CE | **a sine table**: 1025 `u16`, `round(4096 * sin(i*pi/2048))` for i in 0..1024, max abs error 0.50 |
| 0x4F2E0-0x4F460 | 192 monotonic `u16`, 4096..8175 -- probably the same table's continuation |
| 0x53770-0x53F72 | 1025 monotonic `u16`, 0..512; fits neither sine nor a linear ramp (best linear fit is off by 47) |
| 0x53F7C-0x541C4 | the `$gp` small-data cluster -- 60 inbound auto-symbols, immediately below `_gp` = 0x80080994 |

Only 16,685 of its 90,548 bytes are accounted for by string detection, and
46,831 uncovered bytes are neither zero nor a control code, so this blob really
is mostly structured data.

## .bss is above the image

223 of the auto-symbols reference vram at or above 0x80081800, i.e. `.bss`,
which is not in the file at all. Two clusters hold most of it --
0x80081E90-0x80084960 (108 symbols) and 0x800869A8-0x80086F5C (64) -- and
isolated references run up to 0x800FC000, which is where the overlays live
(`docs/overlays.md`).

One knock-on of putting `tail_unloaded` in a `bin` subsegment: `bin` lands in
the `.data` group, so those bytes push the ld script's `_69F4_BSS_START` 176 KB
above the real `.bss`. That is cosmetic -- `.bss` emits no bytes and its
symbols come from `undefined_syms_auto` as absolute addresses -- but it is why
the bss region of the map file looks wrong.

## How to split a text pool (and when you cannot)

`ld_legacy_generation` groups a segment's subsegments by section, and **the
`.data` group is emitted before the `.rodata` group**. A `shiftjis` subsegment
is `.rodata` and a `bin` subsegment is `.data`, so:

- You **can** replace a whole `bin` with `shiftjis` subsegments when the blob
  is already its own segment. Then the `.rodata` group starts exactly at the
  segment's vram and nothing moves. This is how `str/credits/*` landed.
- You **cannot** interleave them. Any non-text bytes in the middle of the pool
  have to go in a segment of their own, the way the jump-table runs in
  `slus_006.14.yaml` do. Non-text bytes at the *end* of a blob also need their
  own segment, because a trailing `bin` would be emitted *first*.
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
| 0x002810-0x006614 | 11 | 8 bytes of Japanese at 0x5194 that both detectors missed, and a `u16` array `{1,2,3}` in the last 8 bytes (0x660C, already named `D_80032E0C`) |
| 0x000EFC-0x002164 | 54 | a `0x8004D0xx` pointer table at 0x1DF9 (stride 0x10), a colour triple at 0x1DD8, and a byte ramp at 0x2151 |
| 0x000800-0x000DC0 | 70 | 69 bytes of ASCII `printf` formats at 0x800 (`"MESSYS  %x \n"`, `"STEP    %x \n"`, `"XASTAT %x\n"`) and a 4-pointer table at 0xD98 |

`0x002810` was done this way on 2026-10-30: 322 strings, 12,266 bytes, split
at 0x57F8. What it cost, and what the next pool will cost too:

- **The pool had to be cut short.** Five strings start 2-aligned -- the
  composed-name nouns at 0x57FA, 0x586E, 0x596A, 0x5A62 and 0x60C6 -- and
  `subalign: 4` pads every input section to 4, so a subsegment boundary that
  is not a multiple of 4 shifts everything after it. 0x57F8-0x6614 keeps the
  remaining 90 strings as `bin`; splitting them needs one extra segment per
  misalignment. **Check `offset % 4 == 0` on every boundary before splitting.**
- **The `.shiftjis` directive could not hold the script's control bytes.** The
  descriptions embed 0x0A line breaks (and the dialogue pools in the 90 KB
  blob also use 0x11 and 0x08), which decode to C0 controls and ended the
  directive's line mid-string. `splat/ext/shiftjis.py` now escapes any
  character below 0x20, plus `"` and `\`, and `azure shiftjis` reverses
  exactly those escapes. This was the blocker for *every* remaining pool, so
  it is now cleared for all of them.
- **46 of the 412 strings came from the fallback scan, not `findstr`.** They
  are the Japanese **unidentified-item** names, never translated for the US
  release and still in the binary, built as colour-plus-noun pairs
  (`紺色の草`, `金色の種`, `薄い巻き物`, `四角いメガネ`). They appear as
  `str/item/jp_*`, named by offset because the slug would be empty.

Next: `0x000EFC-0x002164` (4,712 bytes, 54 uncovered non-zero bytes) and
`0x000800-0x000DC0` (1,472, 70), then the 90 strings left at 0x57F8. The
dialogue pools inside the 90 KB blob are harder -- their entries are not
aligned at all.
