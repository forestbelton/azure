# The data in main.bin

Companion to `docs/overlays.md`, which covers this container's two code blocks.
Numbers here were measured on 2026-11-01 against `data/main/main.bin`
(sha1 69f63236b7ab2260e2e9aa2048ce7aa540bdbe6d); re-measure rather than trust
them. Scripts to reproduce every figure are described inline.

`main.bin` is 2,578,432 bytes (0x275800), LBA 1817 on the disc, and the
ISO9660 directory record agrees with `spec.yaml` exactly. It is a container of
sector-aligned resources, not a program.

## The headline: 2.0 MB of it is dead

`[0x20940, bin]` used to be one 2,100,928-byte blob — 81% of the container and
the single largest unmapped run in the project. It is now three segments, and
**the last of them, 2,069,472 bytes (0x28420..0x221800), holds nothing at
all**. It is named `unread_filler` for the same reason `slus_006.14`'s tail is
named `tail_unloaded`: so splat stops calling it unknown and nobody spends
another pass on it.

The full argument is in the comment on the segment in `splat/main.bin.yaml`.
In short:

**The game never reads it.** A PCSX-Redux CD-ROM command log covering boot,
title, town and dungeon — 133 seeks, 88 distinct — hits `main.bin` at exactly
eleven offsets:

```
+0x0  +0x3000  +0x4800  +0x20000  +0x20800  +0x21800
      +0x221800  +0x226800  +0x257000  +0x258800  +0x25d000
```

The one gap in that list is this run, to the sector. Reproduce with
`python3 tools/cdrom-log-decode.py scratch/cdrom.log`. There is also nothing to
reach it *with*: neither `slus_006.14` nor `main.bin` holds the sector numbers
(67, 1091, 1101, 1198, 1201, 1210) or the byte offsets (0x221800, 0x226800,
0x257000, 0x258800, 0x25d000) of the known resources as a u16 or u32 table
anywhere, so the loader has no resource directory that could name it.

**It is still real, deliberately mastered content.** All 1011 of its CD sectors
(LBA 1897..2907) carry a **valid EDC**, computed over bytes 16..2071 per the
Mode 2 Form 1 layout — EDC at 2072, *not* 2064, which is the mistake to avoid;
validate the checker on a known-good sector before trusting it. So this is not
a dump artifact and `make containers` has to reproduce it byte for byte.

**It is indistinguishable from uniform random.**

| measure | the filler | `slus` `tail_unloaded` | true-random control |
|---|---|---|---|
| bytes | 2,069,472 | 176,128 | 200,000 |
| entropy | 7.9999 | 7.9989 | 7.9992 |
| chi-square (255 df) | 218.5 | 268.7 | 222.4 |
| zero fraction | 0.00392 | 0.00374 | 0.00397 |

Longest zero run in the whole 2 MB is **2 bytes**, and a 128-byte-window scan
finds **0 anomalous windows out of 16,167**. It is homogeneous end to end —
there are no internal boundaries to find. And it is the *same fingerprint* as
`tail_unloaded`, which `docs/data-map.md` reached the same dead end on. Two
regions, one explanation.

### What it is not, each tested rather than assumed

- **Not compressed game data** — it is incompressible. zlib -9 returns 100.03%
  of input, xz 100.02%; both *expand* it.
- **Not any PRNG fill.** Berlekamp-Massey over its bit sequence gives linear
  complexity exactly n/2+1 (1001 at 2000 bits, 4001 at 8000, 10001 at 20000),
  which is the random signature — no LFSR produced it. Solving
  `x[n+1] = a*x[n] + c mod 2^32` from three consecutive words predicts 2 of
  4095 at every alignment and endianness tried, so no 32-bit word LCG did.
- **Not SPU-ADPCM and not XA-ADPCM.** The 16-byte-block test (byte 0 shift<=12
  and filter<=4, byte 1 flags<=7) scores 0.2543 / 0.0324 against a random
  baseline of 0.254 / 0.031, at every phase; and 0 of 16,167 128-byte groups
  show the XA sound-group header redundancy. The control is below: the same
  test scores 0.99 on this container's actual ADPCM.
- **Not images** — no TIM header at any 4-aligned offset in it.
- **Not a copy of anything else on the disc.** Six 24-byte needles taken across
  it each occur **exactly once** in all 298 MB of `az.bin`: here.
- **Not the disc's padding convention.** `/STR/DUMMY_.STR` and
  `/STRT/DUMMYM.STR` are 99.6% zeros — 16 bytes of 0x0c then zero fill, one
  sector repeated 166 times. This is nothing like them.
- **Not a file-wide pre-fill.** Every *other* gap in `main.bin` is zeros
  (0x24440..0x28420, 0x268b6c, 0x2719e0 are 90-99.9% zero). Only this one run
  was filled this way, so it was filled deliberately and separately.

### The one structural fact

It ends at 0x221800, and **0x221800 - 0x21800 = exactly 0x200000**. Read
together with the segment before it, the resource the game loads at +0x21800
sits at the head of a round 2 MiB reservation whose unwritten remainder is this
run. That makes it a reserved hole holding the images at +0x221800 and beyond
on fixed sector offsets — which is why it is in the container at all.

Why the remainder is random rather than zero is not established, and nothing in
the decompilation depends on it. **Do not spend time on it.**

## The rest of the old blob

The 31,456 bytes ahead of the filler are real and the game does read them.

| segment | range | bytes | what |
|---|---|---|---|
| `monsters` | 0x20800-0x21268 | 2,664 | the string pool, **decoded 2026-11-09** |
| `main_tables_21268` | 0x21268-0x28420 | 29,112 | tables, then zero pad from 0x24440 |

**The string pool is done.** `asrc/monsters.s` is all 134 of its entries as
readable `.shiftjisz` directives, one `hrodata` subsegment, replacing 17
one-string `shiftjis` segments plus a 2,368-byte `str/main/pool_tail` — 18
linker-script segment blocks for 2.6 KB of text, two thirds of it never
decoded. From 0x208A4 the pool runs in groups of three: an egg description,
the monster NAME in capitals, then a colour in lower case. Full width is how
the US release renders Latin text in the Japanese font: `0x8260`-`0x8279` are
`A`-`Z`, `0x8281`-`0x829A` are `a`-`z`, `0x8140` is space, `0x8144` a period.

Two things the consolidation turned up:

* **One of the 17 old segments was misnamed.** `[0x20904, shiftjis,
  str/main/cinnabar]` spanned 32 bytes, which is TWO strings, and put
  `STR_MAIN_CINNABAR` on the first of them. 0x20904 is `GOLEM`; `cinnabar` is
  at 0x20910 and had no label at all. Both have their own now. The other
  sixteen names are reproduced exactly.
* **The pool ends at 0x21268, not 0x21280.** Its last entry is `Ultimate` at
  0x21254, whose NUL is at 0x21264, and 0x21265..0x21268 is `C4 0D 0E` — the
  original toolchain's buffer showing through where it padded the pool up to
  the 4-aligned table, the fifth instance of that in the project and the first
  outside a rodata/text boundary (`docs/owning-data.md` §4.8). The 24 bytes
  from 0x21268 are table words, not text — the 20-byte records whose stride
  runs 0x2127C, 0x21290, 0x212A4, … leave 0x21268..0x2127C as one more
  20-byte unit ahead of them — so `main_tables` starts there now.

`main_tables_21268` is pointer and index tables (0x21268..0x21800), then 11 KB
of unidentified fixed-size records (0x21840..0x24340), then a pointer table
(0x24340..0x24400), then zero padding from 0x24440. 144 of its words are
pointers into 0x8002c094..0x8008b270 — into the resident executable's own RAM —
so it is a relocated resource rather than a standalone file. The game seeks
both +0x20800 and +0x21800, so it is read in at least two pieces.

## What splat sees now

Re-measured 2026-11-09, after the 0x80020000 overlay took over the whole of
its own data (`docs/owning-data.md` §7). These are subsegment spans summed
straight out of `splat/main.bin.yaml`, which is why they add up to the
container exactly; splat's own summary rounds to KB and lumps the named and
unnamed `bin` together.

| type | bytes | % of container | what |
|---|---|---|---|
| `bin` (named) | 2,358,680 | 91.48% | identified resources |
| `bin` (unnamed) | 132,640 | 5.14% | the two blobs below |
| `c` | 63,036 | 2.44% | both overlays' text (29,948 + 33,088) |
| `.data` | 15,692 | 0.61% | the 0x80020000 overlay's data, all of it |
| `.rodata` + `rodata` | 4,548 | 0.18% | both overlays' rodata |
| `hrodata` | 2,664 | 0.10% | `asrc/monsters.s`, the string pool |
| `data` | 1,172 | 0.05% | `ovl_main_80020000_pad` |

Down from 2,250,432 bytes (87.3%) unknown before the first pass over this
container, and from 149,024 (5.78%) before the overlay's data was claimed.
Read splat's `unknown` line and not its "in defined segments" total —
`docs/data-map.md` explains why that headline number is unreliable.

## The 130 KB still unknown, and what it is

| blob | bytes | what it is |
|---|---|---|
| `[0x3800, bin]` | 116,736 | **the VAB waveform area** — identified, see below |
| `[0x2719e0, bin]` | 15,904 | 90.1% zeros; debug-twin data + padding |

Two rows gone since 2026-11-01. `[0x265000, bin]`, 16,384 bytes, is not
"overlay data + padding" awaiting a decision any more: 0x265000..0x268b6c is
the upper half of the 0x80020000 overlay's `.data` and is defined in
`csrc/ovl_main_80020000.c`, and 0x268b6c..0x269000 is the 1,172-byte pad to the
sector boundary, now its own named `data` subsegment. The 480-byte
`[0x264e20, bin]` went the same way. The remaining `[0x2719e0, bin]` is the
debug twin's data and is the same job on the other block.

**`[0x3800, bin]` is the sound bank's samples and should simply be named.** The
VAB header at 0x3000 is `pBAV`, version 7, 3 programs, 39 tones, 28 VAGs, with
`total_size` 0x1C310 — so the VAB spans 0x3000..0x1F310 and the splat segment
at 0x3000 (2,048 bytes) is only its header sector, exactly as
`tools/parse-vab.py` describes. The blob after it is the waveform area, and it
measures as ADPCM rather than merely being assumed to be: **99.3% of its 7,088
16-byte blocks are valid SPU-ADPCM** on the same test that scores 0.254 on the
filler. Sample data actually runs to 0x1FAF0 — 2,016 bytes past the VAB's
declared `total_size` — and then zero-pads to the 0x20000 sector boundary.

So of the 130 KB, 114 KB is a known quantity awaiting a name and the other
16 KB is the debug twin's data and zero padding. **There is no unexplained bulk
data left in `main.bin`.**
