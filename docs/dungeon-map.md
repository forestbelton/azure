# Where the code is in DUNGEON.BIN

The container is 27,813,888 bytes (0x1A86800), sha1 `27f9fcc3...`, and until
2026-11-09 exactly one of its code overlays was mapped. It has at least four
more families, and about two hundred further code objects.

**Fixed findings only**, like docs/overlays.md: offsets, load addresses and
object counts are properties of the image. How much of it is decompiled is not
recorded here -- `tools/decomp/progress.py` and `tools/decomp/guard.py` are the
authority.

## Why the old answer was wrong

`splat/dungeon.bin.yaml` used to say "there is only ONE code overlay in this
container", on this evidence: assuming the 0x80088760 base at the other seek
offsets "scores 0-45 against this one's 353".

The scores were right. The inference was not. A bank that loads at 0x80016000
or 0x8016e800 scores nothing at the 0x80088760 base **because the base is
wrong**, so a low score there is not evidence of absence. The prologue vote --
counting a block's own `jal` targets that land on its own `addiu $sp,$sp,-N`
prologues -- can only rank candidate bases for a span you already suspect.
Used as a search it has no power, and that is how ~195 banked overlays came to
be filed as texture records.

## The test that does work

`tools/decomp/codemap.py` scores a delta on whether internal `jal` targets land
**two words after a `jr $ra`**. cc1psx emits functions back to back, so those
two words are the previous function's return and its delay slot, and the word
after them is a function start -- which is the only thing a `jal` inside a
linked object may target. On the dungeon overlay:

| test | correct delta | runner-up |
|---|---|---|
| prologue vote | 1484 | 374 |
| function-start | 456 of 456 (100%) | 4% |

That margin is what makes a blind sweep over 26 MB possible. It assumes nothing
about the load address beyond "it is in RAM", and nothing about size, alignment
or layout.

    python3 tools/decomp/codemap.py data/dungeon/dungeon.bin --distinct
    python3 tools/decomp/codemap.py data/dungeon/dungeon.bin --pin 0xa3d40 0xf7700

A sweep hit is sector-granular: read it as "code is in here", then pin the exact
ends with `--pin`, the way the text-end note in the yaml does it.

## The map

### 1. The dungeon overlay -- SPLIT, in the build, byte-identical

| | file | vram |
|---|---|---|
| rodata | 0xa3000..0xa3d40 | 0x80088760..0x800894A0 |
| text | 0xa3d40..0xf7700 | 0x800894A0..0x800DCE60 |

863 functions, 85,616 words. Pins at 456/456 (100%), runner-up 4%.

The text start was corrected from +0xa3e28 to +0xa3d40 on 2026-11-09 -- the
58-word error the yaml had documented against itself since the block was first
split. Both ends were re-checked before the boundary moved: the word at
+0xa3d40 is `27bdffe0`, the four words below it are the `01c801c0 / 0010001c`
2-word record pattern, and the last `jr $ra` is at +0xf76f8 with its delay slot
at +0xf76fc. `make containers` is byte-identical with the corrected boundary and
func_800894A0 now has a listing of its own.

### 2. The banked per-slot overlays -- ~195 objects, the bulk of the work

File **0x9bd800..0x135e000** (a 9.63 MB span) holds code objects of **0x6000
bytes each**, stored as **seven relinked copies** -- one per RAM slot. Slot
bases descend by 0x6000 across roughly 0x8014b000..0x80175800; copy *i* sits
0x6000 lower in RAM and 0x6000 further on in the file, and groups repeat at a
0x2A000 stride (7 x 0x6000).

`func_800A0B94`, in the dungeon overlay itself, is the loader:

    dest = 0x80170800 - i * 0x6000
    nsec = 0xC                              (12 sectors = 0x6000 bytes)
    LBA  = 0x4340 + 84 * (floor - 1) + 12 * i

then `func_8003F6D4` / `func_8003E4FC` kind 6 -- the CD queue's
`{nsec << 23 | dest, LBA}` descriptors (see tools/scan-img.py). 84 = 7 * 12, so
a floor's seven resources are consecutive.

Three independent confirmations that these are code and that the copies are
relinked, not merely similar:

1. **A single bank pins at 100%.** 0xe2d000..0xe33000 gives vram
   0x80170800..0x80176800 at 22/22 internal targets, runner-up 10%; the next
   bank, 0xe33000..0xe39000, gives 0x8016a800 at 22/22. Pinned as one 363 KB
   span instead, the same region smears across deltas 0x6000 apart and peaks at
   54% -- which is what a sequence of relinked copies looks like to a test that
   assumes one base.
2. **The word diff between adjacent banks is the relink.** Of 6144 words,
   5839 are identical, 93 differ by exactly 0x6000 (relocated pointer words)
   and 176 by 0x1800 -- which is 0x6000 >> 2, the `jal` target field.
3. **Each bank's first word points to its own entry function**, which is what
   the loader returns (`lw $s4, 0x0($s1)`). At 0xe2d000 that word is
   0x80170890, and file 0xe2d090 is `addiu $sp, $sp, -0x40`. The head words run
   0x80170890 / 0x8016a890 / 0x80164890 / 0x8015e890 / 0x80158890 across
   consecutive banks -- base + 0x90 every time.

**Only one copy of each object is work.** A sweep at sector granularity reports
212 hits and at most 118 distinct objects by a relink-invariant fingerprint
(immediates, jump targets and relocated pointer words masked out); families of
up to six copies are observed directly. 118 is an **upper bound** -- the hit
offsets are sector-granular and not all phase-aligned to bank starts, which
splits some families. Pinning exact bank starts across the region is the one
piece of this map still to do, and it is what turns the bound into a count.

### 3. The 0x80016000 low-RAM overlay

File **+0x2a3000**, base **0x80016000** (delta 0x7FD73000), pins at **62/62
(100%)**, runner-up 6%. Sweep extent 0x29f800..0x2b0800.

This is the blob the yaml dismissed as "pointers into 0x8001d4xx, so it belongs
to the low-RAM region that main.bin's overlay occupies". Those pointers are a
switch jump table, and under the pinned delta they point **into this object
itself** (0x8001d4fc maps to file 0x2aa4fc). Its first real prologue is at
+0x2a3078.

There is a byte-identical copy at **+0x51e800** (38,144 bytes compared).

### 4. The 0x80020000 overlay -- same slot as main.bin's, different code

File ~**0x1829800..0x1833800**, base **0x80020000** (delta 0x7E7F5000), pins at
**94/108 (87%)**.

0x80020000 is where main.bin's memory-card overlay loads
(`csrc/ovl_main_80020000.c`), but this is **not** that object: 13% byte
equality over the first 0x8000, and main.bin's overlay text head does not occur
anywhere in dungeon.bin. So the two are mutually exclusive occupants of one
low-RAM slot, exactly as town's and dungeon's overlays are of 0x80088760.

Five smaller objects follow at 0x1975000, 0x197b800, 0x199f000, 0x19c9000 and
0x19db000, all basing near 0x80020800 with 10-13 internal targets each.

### 5. The head region -- real code, base UNRESOLVED

File roughly **0x7800..0xf000**, inside what the yaml declares `[0x0, bin]`.

It is certainly code. `0x7804` is a clean prologue followed by saved registers,
an angle counter wrapping at 0x1000, and `jal 0x8006dd28`; `0x8000` carries the
`div` / `bnez` / `break 0x7` / `break 0x6` sequence that is gcc's
divide-by-zero and INT_MIN/-1 trap pair, which cannot arise by chance. Of its
27 distinct call targets in the resident engine, **18 land exactly two words
after a `jr $ra` in slus_006.14** -- checked against the real image, not
inferred.

It also calls **22 addresses inside the main dungeon overlay**
(0x800894A0..0x800DCE60), so it is resident at the same time as that overlay and
therefore cannot load at 0x80088760.

**Its own base is not determined.** It makes no self-calls that the
function-start test can use, so there is nothing for the test to score; the best
delta over the span explains 17 of 36 "internal" targets (47%) and would place
the block inside the resident engine, so it is wrong. The ten remaining targets
cluster in 0x800ff814..0x8010d810 at a suspiciously uniform ~0x500 spacing and
do not resolve to function starts at any delta, so they are probably record
fields that decode as `jal` rather than real calls. Finding this one's base
needs a different anchor -- a function-pointer table elsewhere that names it, or
a CD-ROM log seek for the offset.

## The trap in this container

**Shift-JIS text decodes as valid MIPS.** A full-width Shift-JIS byte pair
`82xx` has top byte 0x82, so the word's opcode field is 0x20 -- `lb` -- and an
instruction-validity sieve reports 100% over a block of pure text. Two sectors
of the string table at +0xd800 and +0xe000 read as "100% valid, looks like
code"; they are the character-name block, 81% identical to the one town.bin
carries at the same offset. Density alone over-reports code here. The
function-start test is not fooled, because text produces no self-consistent
delta, which is the other reason to prefer it.
