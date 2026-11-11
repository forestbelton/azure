# What is in TOWN.BIN

Every byte of the container accounted for: which parts are assets, which are
code, and which are neither. Companion to `docs/image-blobs.md` (the texture
record format), `docs/overlays.md` (the code) and `docs/dungeon-map.md` (where the
code is in the other big container). `docs/town-sweep.md` is about
decompiling the overlay and is unaffected by any of this.

The container is **16,889,856 bytes (0x101B800), 8,247 sectors**, sha1
`b946a57a...`, and it begins at disc sector **3077**, so a file offset maps to
an absolute LBA as `3077 + offset / 0x800`. That mapping matters: the engine
addresses the disc by absolute LBA and a container is just a region of it.

**Fixed findings only.** Offsets, extents and record counts are properties of
the image. Everything below was measured on the shipped container, and the
tool that measures it is the authority:

    python3 tools/scan-container.py town.bin             # the map
    python3 tools/scan-container.py town.bin --anchors    # the inventory
    python3 tools/scan-container.py town.bin --symbols s.txt

(`python3`, not `uv run python`: it needs numpy, which like
`tools/decomp/codemap.py` it gets from the system interpreter and not from the
project venv.)

## The tool, and why it should be trusted

`tools/scan-container.py` runs `tools/scan-img.py`'s record walker and
`tools/decomp/codemap.py`'s function-start test over a container, adds the
formats neither looks for -- VAB sound banks, KCET sequences, the scenario
system's symbol tables -- and then classifies the residue. Its own classes and
how each one is recognised are in its docstring.

It reproduces two numbers this project established independently, which is the
reason to believe the rest of what it says:

| container | its `FILL` region | this project's earlier measurement |
|---|---|---|
| `slus_006.14` | 0x55000..0x80000, 176,128 bytes, entropy 7.9989, zero 0.00374, chi-square 268.7 | `tail_unloaded`, 176,128 bytes, 7.9989, 0.00374, 268.7 (`docs/data-map.md`) |
| `main.bin` | 0x28800..0x221800, entropy 7.9999, zero 0.00391, chi-square 218.6 | 0x28420..0x221800, 7.9999, 0.00392, 218.5 (`docs/main-bin-data.md`) |

(The `main.bin` start differs by 0x3E0 because the byte-exact growth only runs
where a 16-byte zero run brackets the region, and there is none there.)

## The six bands

| band | file | sectors | what it is |
|---|---|---|---|
| preamble | 0x0..0xb000 | 22 | one sequence and a truncated sound bank, shared with the other containers |
| overlay | 0xb000..0x96000 | 278 | the town code overlay -- **text, rodata AND data** |
| filler | 0x96000..0x20b000 | 746 | dead mastered filler, same as main.bin's |
| assets | 0x20b000..0x9fb000 | 4,064 | the game's art, music, script, and 48 small code objects |
| dev tail | 0x9fb000..0xf65800 | 2,773 | 71% zero; a PSY-Q debugger config file and fragments. No asset in it. |
| blob tail | 0xf65800..0x101b800 | 364 | 72 more image blobs, mostly compressed RAM payloads |

By class, over the whole container:

| class | sectors | % | class | sectors | % |
|---|---|---|---|---|---|
| `IMG` | 1,878 | 22.77 | `TEXT` | 272 | 3.30 |
| `ZERO` | 2,037 | 24.70 | `SEQ` | 157 | 1.90 |
| `DATA` (residue) | 1,834 | 22.24 | `SYMS` | 65 | 0.79 |
| `FILL` | 746 | 9.05 | `OVL` | 278 | 3.37 |
| `VAB` | 498 | 6.04 | `CODE` | 482 | 5.84 |

The assets themselves: **152 image blobs** (1,381 records, 436 textures, 886
palette records, 37 of the blobs carrying only compressed RAM payloads),
**17 VAB sound banks** (1,004,208 bytes), **34 KCET sequences** (286,224
bytes, 271 tracks), **2 symbol tables** (3,587 names) and **272 sectors of Shift-JIS
script**.

## 1. The overlay load table, and the overlay's real size

At vram **0x80080C38** (`slus_006.14` file 0x54438) sit three 8-byte CD queue
descriptors in the `{nsec << 23 | dest, LBA}` form `tools/scan-img.py`
documents, followed by a fourth of the same shape:

| vram | nsec | dest | LBA | what it reads |
|---|---|---|---|---|
| 0x80080C38 | 278 | 0x0088760 | 3099 | `town.bin` +0xb000 |
| 0x80080C40 | 187 | 0x0088760 | 12636 | `dungeon.bin` +0xa3000 |
| 0x80080C48 | 14 | 0x0088760 | 1884 | `main.bin` +0x21800 |
| 0x80080C50 | 133 | 0x0176800 | 280 | between `slus_006.14` and `main.bin` |

The first two are the seeks `docs/overlays.md` caught in a PCSX-Redux CD-ROM
log; this is the table they come from, and it states the lengths the log could
not.

**The town overlay is a 278-sector read, where splat maps 142 sectors of it.**
278 sectors is `+0xb000..+0x96000`, 569,344 bytes, so RAM gets
`0x80088760..0x80113760`. The file behind it is one byte-count shorter and the
two numbers agree on it: the filler run below begins at **0x95AC4**, and
`ceil((0x95AC4 - 0xb000) / 0x800)` is **exactly 278**. So the overlay's own
file is **0xb000..0x95AC4, 568,004 bytes**, the read rounds it up to a sector
boundary, and the last 1,340 bytes delivered to RAM are filler landing in what
is `.bss` anyway. Inside it:

```
0x0b000..0x0d524   rodata                      0x80088760..0x8008AC84
0x0d524..0x51fc0   text, 1815 functions        0x8008AC84..0x800CF720
0x51fc0..0x7dcb8   data                        0x800CF720..0x800FB418
0x7dcb8..0x95ac4   zero to the end of the file 0x800FB418..0x80113224
```

`splat/town.bin.yaml` ends the overlay at 0x51fc0 and files everything after
it as unnamed `bin`. Three independent measurements say the 0x51fc0..0x7dcb8
run is the overlay's own data:

1. **The symbol addresses.** The 979 `D_800?????` symbols the overlay's C
   references include 791 at or above 0x800CF720, and the lowest of them is
   **exactly 0x800CF720** -- the first byte after the text. They run up to
   0x800FE5D8, which is past the end of the read and therefore `.bss`, zeroed
   by the loader rather than stored.
2. **Function pointers.** 1,265 words in 0x51fc0..0x7e000 fall inside the
   overlay's own text range, and **934 of them (74%) land exactly two words
   after a `jr $ra`** -- a function start, by the test `codemap.py` is built
   on. The same test over 0x96000..0x120000 of the filler finds 7 candidates
   and **0 hits**.
3. **Self-pointers.** 8,399 words in the region point back into the region.

Three consequences outside this container, all cheap to check and worth
checking:

* **`main.bin+0x21800..0x28420` is a third overlay at 0x80088760.**
  `codemap.py --pin` gives it **5/5 (100%)** at that base. `docs/overlays.md`
  dismissed it ("12 KB / 14 prologues at +0x21000 -- density too low to be
  code"); the descriptor table says the game loads it, and the pin agrees.
  **Confirmed and split 2026-11-11.** It is the GAME OPTION screen: 360 bytes
  of rodata, 10,152 of text in 26 functions, 784 of data and 16,384 stored
  zeros, 0x21800..0x28420, which is exactly the descriptor's 14 sectors when
  rounded up. 29 of its 29 `jal` targets land two words after a `jr $ra`, and
  24 of those are in `slus_006.14` itself, so it was linked against the
  shipped engine. `docs/overlays.md` has the section.
* **The dungeon overlay's read is 187 sectors**, file 0xa3000..0x100800, so it
  too carries about 0x9100 bytes of data past the 0xf7700 text end
  `splat/dungeon.bin.yaml` stops at.
* **The 133-sector read at LBA 280 is in none of the four containers.**
  `spec.yaml` extracts `slus_006.14` as sectors 24..280 and `main.bin` from
  1817, so LBA 280 is in the gap. Its first sector holds
  `MDEC_rest:bad option(%d)`, `MDEC_in_sync`, `MDEC_out_sync` -- PSY-Q
  **libpress** -- so 272,384 bytes of resident library code is loaded from a
  region nothing in this repo currently extracts.

## 2. The preamble is shared, and its sound bank does not fit

`town.bin`'s first 0xb000 bytes are **byte-identical to `main.bin`'s first
0xb000**, and `dungeon.bin`'s diverge from it at exactly **0x7800** -- which is
where `docs/dungeon-map.md` §5 found that container's unplaced head code. So
all three containers are built on one 22-sector preamble and dungeon overwrites
part of it.

The preamble holds:

* **0x0..0x2938** -- a KCET/TSDB/KDT sequence, 13 tracks.
* **0x3000** -- a VAB header: 3 programs, 39 tones, 28 VAGs, header 0x1020
  bytes, `fsize` 0x1C310. The header is internally consistent (header size
  plus the summed VAG size table equals `fsize` to the byte, as it does for
  all 17 banks in this container), but only **0x8000 bytes are present**: the
  overlay starts at 0xb000 and the waveform is cut mid-sample. The bytes up to
  0xb000 really are SPU ADPCM.

No descriptor in `slus_006.14` or in either mapped overlay targets the
preamble, and its bank cannot be loaded from here at its declared length. The
reading this supports is a build artifact of how the containers are assembled;
the town's own banks are the 16 in the asset region.

## 3. The dead filler: 0x95AC4..0x20B000

1,529,148 bytes with the fingerprint `docs/main-bin-data.md` closed that
container's dead region with: **entropy 7.9999, zero fraction 0.00397,
chi-square 251.8**, and both zlib and xz *expand* it. `main.bin`'s twin
measures 7.9999 / 0.00391 / 218.6. It ends exactly where the first image blob
begins.

Same conclusion as there: deliberately mastered filler in a reserved hole.
Do not spend time on it.

## 4. The asset region: 0x20B000..0x9FB000

4,064 sectors, and where everything the game actually shows or plays lives:
80 image blobs, 33 sequences, 16 sound banks, 43 runs of code, 272 sectors of
script, both symbol tables, and 944 sectors of residue. Sequences, blobs,
banks and code are interleaved rather than grouped by type -- the ordering
looks like per-area bundles, but that is an impression from the layout and not
something measured.

### The script is in the container, in two languages

The Shift-JIS runs decode as the game's dialogue, written in **full-width
characters** -- which is what makes them findable: the ideographic space 0x8140
alone is a fifth of the bytes.

```
0x2ce000  Ｔｈｅｙ ｓａｙ ｃｈｉｌｄｒｅｎ’ｓ ｓｏｎｇｓ ａｒｅ ｍｅｓｓａｇｅｓ ｆｒｏｍ
0x62c000  Ｇｅｎｅｒａｌ Ｓｔｏｒｅ ２ / Ｗｅｌｃｏｍｅ ｔｏ ｔｈｅ Ｆｏｒｔｕｎｅ Ｔｅｌｌｉ…
0x86f800  台詞へ） / 　あらっ。◯くん。 / 　今日はお休みなんだ。
```

So the **Japanese script is still on the US disc** alongside the English one.
`0x86f800`'s block even opens with the word 台詞 ("lines", as in an actor's).
Blocks start `08 00 00 00` and mix text with binary records; the container
format inside a block is not worked out here.

### 3,587 of the original source's identifiers

Two tables of `struct { char name[32]; s32 value; }`, NUL-padded:

| file | records |
|---|---|
| 0x80C000..0x811F58 | 678 |
| 0x812000..0x82B914 | 2,909 |

**The name comes first.** The other field order parses just as well and is
wrong by one record everywhere; `docs/town-identifiers.md` has the two checks
that settle it.

They are the scenario system's symbol tables. By prefix, over both:
`F_` / `Fr_` / `Ft_` / `Ftt_` / `Fc_` / `Fp_` flags and callbacks (2,907),
`SSTP_` (138), `mamonogoya_` (128, "monster shed"), `FNO_` (113), `IMG_` (79),
`GOODS_` (56), `S_` (44), `V_` (25), `sn_` scenario switches (24), `PSN_` (21),
`ANM*` (13), `Arg_` (3), `Mode_` (2), `TB_` (2), 32 others. Representative
rows:

```
0      snf_false            11     PSN_DM_WESTWK        158    FNO_fukidasi_set
1      snf_truth            22     DFLT_END_PSN_DM      1002   F_zero
0      IMG_BKEW_N           41     SSTP_PLT_DM_SWIM     5271   GOODS_SAVE_FLG_00
```

`dungeon.bin` carries five smaller tables of the same shape (166 records,
`SSTP_PLT_DM_*` -- sprite/animation states); `main.bin` and `slus_006.14`
carry none. `--symbols` writes them all out.

**72 of these names are now attached to addresses.** The person state
machine's enumeration indexes two dispatch tables in `ovl_town`, which named
72 handler functions and replaced the dispatcher's `0x16` bound with
`DFLT_END_PSN_DM`; `include/town.h` carries the constants and
`docs/town-identifiers.md` carries the evidence and what the other 3,515 are
waiting on.

### The second tier of code, and the dev builds in it

`codemap.py --distinct` finds 51 code objects in the container; three of them
are the town overlay itself, and the other **48** are here, in three bands:

| band | objects | file span |
|---|---|---|
| 0x80011000..0x8001F800 | 41 | 0x2a8800..0x8d9000 |
| 0x806F????/0x8070???? | 6 | 0x7f0000..0x83f000 |
| 0x8011EC98 | 1 | 0x8e6800..0x8f1800 (pins 29/30, 97%, runner-up 17%) |

The 41 low-RAM objects are the tier that `docs/overlays.md` found assert
records in. Re-counting those records with a tighter pattern gives **85**, not
the 76 that document reports, and two more translation units:

| unit | records | unit | records |
|---|---|---|---|
| `main.c` | 54 | `sn_main.c` | 7 |
| `lshop.c` | 10 | `player.c` | 4 |
| `tobjs.c` | 8 | `flgtst.c` | 2 |

The 0x806F band **cannot run on a retail console**: 0x806F0000 is in the top
of the 8 MB DTL-H2000 window, the same place `main.bin`'s assert-enabled debug
twin lives (`docs/overlays.md`). Its internal `jal`s cluster on 0x807000xx, so
the base is real even though no single delta explains the whole span -- it is
several separately linked objects, not one. `sn_main.c`'s and `flgtst.c`'s
assert records are in it.

Across tier 2 as a whole, some of these objects load their assets **by host
filename** rather than by LBA, which only works on a dev machine. Every such
path in the container:

```
0x65281c  \vx035\data\town\sb43s000.bin     0x7f877c  \vx035\data\sn\SENGEI10.BIN
0x65283c  \vx035\data\town\sb06.bin           ...     SENGEI09 .. SENGEI00
0x8cd128  \vx035\data\town\st.bin           0x8d20fc  \vx035\data\sn\mv05.bin
```

`\vx035` is the project's own directory name. The eleven `SENGEI` paths are
inside the 0x806F band; the `town\` ones are not, so filename loading is not
by itself evidence that an object is the debug build. `st.bin` occurs six
times, five of them at a 0x1000 stride in 0x8cd127..0x8d1124, which is what a
set of relinked copies of one object looks like.

## 5. The dev tail: 0x9FB000..0xF65800

2,773 sectors, 1,956 of them (71%) **entirely zero**, and **not one image
blob, sound bank or sequence in the whole 5.4 MB**. What is there:

* **0x9FB000** -- a **PSY-Q debugger configuration file**, in ASCII:
  `#version / 4.112`, `#readram0`, `#writeram0` with the ranges
  `00000000-007FFFFF`, `1F800000-1F8003FF`, `80000000-807FFFFF`, then
  `#video`, `#colour attributes`, `#default tabs` (`68K`, `658`, `Z80`, `C`),
  `#window`, `#history`. 8 MB of RAM and a `#video 03 50 32` mode: a
  development board's settings, mastered onto the disc.
* **0xA0B000 onward** -- a **0x6000 period** of mostly-zero blocks with small
  non-zero islands at +0x2800, +0x4800, +0x5000 and +0x5800. The islands carry
  Shift-JIS katakana names -- `コウ` and `キューン`, the game's default
  protagonist and familiar -- and words that are pointers into
  0x80010248..0x80010270, the RAM band the tier-2 overlays load into. 192 of
  the 228 blocks are distinct.

0x6000 is exactly the size of one banked overlay slot (`docs/dungeon-map.md`
§2). The reading this suggests -- reserved slots that the retail build left
empty, holding fragments of a dev machine's state -- is a **hypothesis**; what
is measured is the period, the zero fraction, the names and the pointers.

Three single-island strays of the same kind sit inside the overlay's own zero
padding, at 0x82800 (3 bytes), 0x83000 (30 bytes, Shift-JIS fragments and the
pointer 0x80031B58) and 0x93000 (1 byte).

## 6. The blob tail: 0xF65800..0x101B800

72 image blobs in 364 sectors. The last 111 sectors are a dense run of small
blobs carrying only type-3/type-4 records -- compressed or copied RAM payloads
rather than textures -- one or two records each, 3 to 7 KB apiece.

## What is still unidentified

1,834 sectors (22%) come out as `DATA`: structured bytes in no format this tool
knows. Read that as "not yet identified", not "not an asset". The largest runs,
with what the first bytes look like:

| file | sectors | first impression |
|---|---|---|
| 0x8fb000..0x98d800 | 293 | 15-bit colour ramps at 0x8fb000, 0x963000 and 0x973000 (`0000 0421 0842 0c63` is a grey ramp), high-entropy between them. Candidate: raw texture pages and palettes reached through `func_8003F8F8`/`func_8003F9E0` rather than a record stream -- the "other asset path" `docs/image-blobs.md` names in its last section. |
| 0x747000..0x78a800 | 135 | a table of u32s at the head (0x28, 0xf2, 0x3218, 0x83d8), then high-entropy |
| 0x78b800..0x7c5000 | 115 | `TOTAL`, `SCORE` and digit strings -- a dev object's rodata |
| 0xfbf800..0xfe4000 | 73 | between two blob runs; parses as no record stream |
| 0x80a800..0x82c000 | 67 | Japanese text and the two symbol tables' surroundings |
| 0x239000..0x254800 | 55 | a descending s16 ramp (-39, -1137, -2226, -3283, …) |
| 0x649000..0x65a800 | 35 | assert format strings and records -- a tier-2 object's rodata |

Three of those seven are plainly a code object's rodata, which the sweep misses
because its window is 0x6000 and sector-granular; `codemap.py --pin` is how to
put a real boundary on any of them.

## What the yaml says now

`splat/town.bin.yaml` was two unnamed `bin` blobs either side of the code.
It now declares all six bands, and `splat split` reports
**`unknown: 0 B (0.00%)`** -- an unnamed `bin` is what `statistics_type` maps
to `"unk"` (`docs/data-map.md`), so naming them is what empties that line.

The structural change is that the code segment absorbed the overlay's data as
a `data` subsegment, 0x51fc0..0x7e000. It ends at 0x7e000 rather than the last
non-zero byte at 0x7dcb8 because splat emits
`__romPos = ALIGN(__romPos, 16)` after a code segment and a boundary at
0x7dcb8 would pad the container eight bytes long -- the same trap the text end
hit. 0x7e000..0x96000, the rest of the 278-sector read, is `ovl_town_read_tail`.

**757 of the overlay's data symbols stopped being absolute addresses.** They
are now real definitions in `asm/data/ovl_town_data.data.s` and
`town.bin_undefined_syms_auto.txt` lost every one of them. What is left above
0x800CF720 divides exactly as the boundary predicts: **13 inside the data
segment, every one of them unaligned** -- byte-level interior references of
the `D_800D185D` kind, which splat can mint a symbol for but not a `dlabel`
-- and **102 at or above 0x800FB760**, which is the overlay's `.bss` and is
not in the file at all.

One thing to know before touching this again: with the data split, spimdisasm
emits the two person-state dispatch tables as `.word <handler>`, so the 72
names from `docs/town-identifiers.md` had to go into
`town.bin_symbol_addrs.txt` as well as into the C. Renaming a function that a
`data` subsegment points at now breaks the link until splat is told the new
name.

## Next

* **Carve the overlay's data.** Started: 22 objects, 1,416 bytes of 180,288,
  and set up as parks -- `tools/decomp/datawork.py` is the queue and the brief
  generator, `tools/decomp/dataverify.py` is the gate. `docs/owning-data.md`
  §8 is the method: why it goes to `ovl_town.c` as a contiguous prefix, why
  this overlay has no unit oracle, and the traps. 4,657 `dlabel`s to go.
* **Extract LBA 280..413.** 133 sectors of libpress that `spec.yaml` does not
  see at all.
* **Check `dungeon.bin`'s data tail**, from the descriptor table above.
  `main.bin+0x21800`, the other half of this item, is **done**: it is a code
  overlay, split 2026-11-11, and `docs/overlays.md` has it.
* **The 3,587 names.** 72 are attached (`docs/town-identifiers.md`); the rest
  index tables in the tier-2 overlays, so decompiling those is what unlocks
  them. The `FNO_` group -- 113 values at 100..212, a dispatch table indexed
  from 100 -- is the sharpest fingerprint to look for.
