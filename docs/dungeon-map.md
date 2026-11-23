# Where the code is in DUNGEON.BIN

The container is 27,813,888 bytes (0x1A86800), sha1 `27f9fcc3...`, and until
2026-11-09 exactly one of its code overlays was mapped. It holds **116 more
code objects**, stored as 399 copies.

**Fixed findings only**, like docs/overlays.md: offsets, load addresses and
object counts are properties of the image. How much of it is decompiled is not
recorded here -- `tools/decomp/progress.py` and `tools/decomp/guard.py` are the
authority, and `tools/decomp/dungeon_objects.py` re-derives every number below
from the image.

## Why the old answer was wrong, twice

`splat/dungeon.bin.yaml` used to say "there is only ONE code overlay in this
container", on this evidence: assuming the 0x80088760 base at the other seek
offsets "scores 0-45 against this one's 353".

The scores were right. The inference was not. A bank that loads at 0x80016000
or 0x8016e800 scores nothing at the 0x80088760 base **because the base is
wrong**, so a low score there is not evidence of absence. The prologue vote --
counting a block's own `jal` targets that land on its own `addiu $sp,$sp,-N`
prologues -- can only rank candidate bases for a span you already suspect.
Used as a search it has no power, and that is how the banked overlays came to
be filed as texture records.

`tools/decomp/codemap.py` replaced that test and found the code (see below).
But a sweep is still a search: its window is sector granular, so a family
stored as seven relinked copies smears across deltas 0x6000 apart, and the
object count it reported was a **bound** -- "212 hits, at most 118 distinct" --
not a count. The bound was 2.5x too high.

**The count comes from the loaders, not from a sweep.** The dungeon overlay is
decompiled, so the two functions that read code out of this container are
readable C and both compute their LBA arithmetically. Subtracting the
container's start LBA turns each formula into a list of file offsets, and every
one of them can then be checked instead of guessed.

## The test that found the code in the first place

`codemap.py` scores a delta on whether internal `jal` targets land **two words
after a `jr $ra`**. cc1psx emits functions back to back, so those two words are
the previous function's return and its delay slot, and the word after them is a
function start -- which is the only thing a `jal` inside a linked object may
target. On the dungeon overlay:

| test | correct delta | runner-up |
|---|---|---|
| prologue vote | 1484 | 374 |
| function-start | 456 of 456 (100%) | 4% |

    python3 tools/decomp/codemap.py data/dungeon/dungeon.bin --distinct
    python3 tools/decomp/codemap.py data/dungeon/dungeon.bin --pin 0xa3d40 0xf7700

A sweep hit is sector-granular: read it as "code is in here", then pin the exact
ends with `--pin`.

## The map

| | objects | copies | distinct words |
|---|---|---|---|
| 1. the dungeon overlay | 1 | 1 | 85,616 (split, landed) |
| 2. `func_800A0B94`'s banks | 47 | 7 each | 181,252 |
| 3. `func_800A0B94`'s fixed `a == 0x31` | 1 | 1 | 12,097 |
| 4. `func_800A982C`'s 0x80024000 objects | 66 | 1 each | 93,021 |
| 5. the 0x80016000 overlay | 1 | 2 | 9,451 |
| 6. the 0x80020000 overlay | 1 | 1 | 8,674 |
| 7. the head block | -- | -- | dead residue, see below |

**304,495 words of distinct code are not yet split**, stored as 1,573,372
words -- a 5.2x duplication factor. For scale, the whole project through
2026-11-22 is 215,893 words.

None of it is shared with `town.bin` or `main.bin`, and almost none of it is
already written: by opcode stream, 3.5% of the banked family and 0.2% of the
0x80024000 family match a function landed anywhere in the project. This is
reading, not renaming.

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

The overlay's CD read is 187 sectors, 0xa3000..0x100800 (docs/town-map.md §1),
so about 0x9100 bytes of the overlay's own data follow the text end.

### 2. The per-slot banks -- 47 objects, 329 copies, the bulk of the work

`func_800A0B94` (`csrc/ovl_dungeon_8009F644.c`) is the loader, and its
descriptor is three lines of landed C:

    w = 0xC;                                   /* 12 sectors = 0x6000 bytes */
    t = D_80170800 - s[3] * 0x6000;            /* s[3] is the slot, 0..6 */
    n = (a - 1) * 84 + (m = s[3] * 12 + 0x4340);

`dungeon.bin` starts at **LBA 12310** -- the overlay load table at 0x80080C40
reads LBA 12636 for dungeon.bin+0xa3000 -- so bank (a, i) is at file
**(0x4340 + 84(a-1) + 12i - 12310) * 0x800** and loads at **0x80170800 -
0x6000i**. 84 = 7 * 12, so one object's seven copies are consecutive on the
disc and a group starts every 0x2A000.

**All 329 predicted banks pin at the slot address the formula computes**
(`dungeon_objects.py --verify`), most of them at 100% of their internal
targets. The grid is code for **a = 1..45, 55 and 57**; the other rows up to
a = 72 hold texture records, and the head word is what separates them -- every
object in both families begins with a pointer to its own entry function, in its
own load window, at a word that is `addiu $sp, $sp, -N`.

File **0x995000..0x12ed000**, 47 groups of 0x2A000.

Three things the earlier reading of this family got wrong:

* **The entry offset is not 0x90.** It is per-object and ranges from +0x54 to
  +0x1564; +0x90 was one family's value read as the rule.
* **Text does not begin at the entry.** 13 of the 47 have functions *below*
  the entry pointer, reached only through the header's pointer tables, so
  splitting from the entry loses them.
* **The relink is visible and total.** Between adjacent copies of one object
  200 to 300 of 6144 words differ, every one of them a relocation: `jal` fields
  by 0x1800 per slot, pointer words by 0x6000 per slot, and `lui`/`addiu` pairs
  by the corresponding high/low halves. Everything after the object's own end
  is mastering filler and differs for no reason; a fingerprint taken over the
  full 0x6000 therefore reports copies as distinct, which is the other half of
  why 118 was too high.

Within the family the duplication is 1.13x by word: 1,628 functions over the 47
objects collapse to 1,348 distinct. The shared part is a small statically
linked library -- one 466-word routine in three variants appears in 39 of the
47, and an 81-word routine in 46 of them.

### 3. `a == 0x31`, the one fixed-address bank

The same loader special-cases two kinds out of the grid:

    a == 0x31: nsec 0x18, dest 0x8016A800, LBA 0x569C   -> file 0x1343000
    a == 0x38: nsec 0x24, dest 0x80164800, LBA 0x56B1   -> file 0x1358800

The first is a 48th object, 0xC000 long, pinning 35/36 at 0x8016A800 and
sharing no prefix with any of the 47. 153 functions, 12,097 words. It covers
slots 1 and 2 at once, which is why it is not in the grid.

The second is **not code**: no delta puts a `jal` target on a function start
anywhere in its 0x12000, in any 0x2000 sub-window. It is a 72 KB image
resource that happens to be read by the same call.

### 4. The 0x80024000 objects -- 66 of them, one copy each

`func_800A982C` (`csrc/ovl_dungeon_800A4B88.c`) is the second loader:

    q = (s32 *)0x80024000;
    t = (a - 1) * 12 + 0x607F;
    func_8003F6D4(0xC, q, D_800E58B0, t);

0xC sectors again, but a **fixed** destination -- so there is one copy of each,
no relink, and the whole family is work. File **0x1834800..0x19e5000** at a
0x6000 stride, a = 1..46, 49, 51..56, 58..63, 65..70, 72. 589 functions, 95,570
words; 555 distinct, so this family barely repeats itself, and it shares only
four functions with the banked family.

These are much smaller objects than the banks -- text runs 0x800 to 0x3300
bytes -- with the same shape: a header of pointers, then text.

**This family is what §4 of the old map saw.** "Five smaller objects at
0x1975000, 0x197b800, 0x199f000, 0x19c9000 and 0x19db000, all basing near
0x80020800" were five sweep windows that each straddle the start of a grid
object: a window beginning 0x3800 before an object that loads at 0x80024000
pins at 0x80020800, which is exactly what was reported.

### 5. The 0x80016000 overlay, stored twice

File **0x2a3000**, base **0x80016000** (delta 0x7FD73000), pins at **62/62
(100%)**, runner-up 6%. 75 functions, 9,451 words; text 0x2a31d8..0x2ac584.

This is the blob the yaml dismissed as "pointers into 0x8001d4xx, so it belongs
to the low-RAM region that main.bin's overlay occupies". Those pointers are a
switch jump table, and under the pinned delta they point **into this object
itself** (0x8001d4fc maps to file 0x2aa4fc).

The copy at **+0x51e800** is byte-identical over **0x30000 bytes**
(0x2a3000..0x2d3000 == 0x51e800..0x54e800) -- a far longer run than the 38,144
bytes first compared.

### 6. The 0x80020000 overlay -- same slot as main.bin's, different code

File **0x1829800..0x1834800**, base **0x8001E800** (delta 0x7E7F5000), pins at
**94/108 (87%)**. 229 functions, 10,230 words; a 5-word header at 0x1829800,
text 0x1829814..0x18337ec, which runs up to the first object of the 0x80024000
family at 0x1834800.

**It starts at 0x1829800, not the 0x182b000 this file first gave it**, and so
loads at 0x8001E800. The delta is the same either way; 0x182b000 was simply
where a sweep window happened to begin. Walking back from it the code runs
unbroken through 0x182aff0 to 0x1829814, under that header, under 0x600 bytes
of zero padding to the sector boundary, and all five header pointers land
inside the object at this base.

0x80020000 is where main.bin's memory-card overlay loads
(`csrc/ovl_main_80020000.c`), but this is **not** that object: 13% byte
equality over the first 0x8000, and main.bin's overlay text head does not occur
anywhere in dungeon.bin. So the two are mutually exclusive occupants of one
low-RAM slot, exactly as town's and dungeon's overlays are of 0x80088760.

### 7. The head block -- real code, and nothing loads it

File **0x7804..0xbdd4**, 32 functions, 2,631 words. **It is dead build
residue, and it should not be split.**

Three measurements say so, and they replace the "base UNRESOLVED" the previous
version of this file ended on.

1. **It is nine copies of a family the overlay already contains.** Every one of
   its 32 functions falls into a four-function group repeated nine times, and
   the groups match landed overlay code by opcode stream: the 53-word member at
   0.944 against `func_800D4BD4`, the 66-word member at 0.839 against
   `func_800D4FC8`/`func_800D5460`, the ~110-word member at 0.902-0.926 against
   `func_800D5294`/`func_800D4DE8`. Those are the node-burst effect spawners in
   `csrc/ovl_dungeon_800CABA8.c`. The block is nine more instances of the same
   source.
2. **The handler each instance calls does not exist on this disc.** The nine
   groups call 0x8010ae28, 0x8010b30c, 0x8010b864, 0x8010bd54, 0x8010c274,
   0x8010c7a8, 0x8010cd00, 0x8010d230 and 0x8010d810 -- one each, with four
   arguments set up, so they are real calls. 0x8010xxxx is above the dungeon
   overlay's last byte (0x800DCE60). Searching all four containers for a delta
   that puts those nine addresses on function starts, and again for one that
   puts them on prologues, reaches 5 of 9 at best. The code they call was
   linked into a build whose overlay was larger and is not shipped.
3. **It sits in the shared preamble.** `town.bin` and `main.bin` are
   byte-identical over their first 0xb000 and dungeon.bin diverges from them at
   exactly 0x7800 (docs/town-map.md §2); 0xc000..0xe800 is byte-identical to
   `town.bin` again. The block was written over part of a 22-sector preamble
   that is VAB sound data no descriptor reads.

**Two claims the previous version made about this block were artifacts of
scanning data as code.** It does *not* call 22 addresses inside the dungeon
overlay: restricted to words inside real function bodies, the block has 16
distinct `jal` targets -- seven in the resident engine
(0x8003a7c4, 0x8003cf18, 0x80040a18, 0x8006dc5c, 0x8006dd28, 0x8006f508,
0x800750e0) and the nine above. The 0x8008axxx and 0x800bxxxx "targets" were
words in the pointer tables between the groups, and the tables from 0xbdd4 on
are town-overlay content that both containers carry at the same offsets.

## All 116 are carved, 2026-11-22

`splat/dungeon.bin.yaml` now has a `code` segment for every one of them, and
all four containers are byte-identical. `tools/decomp/dungeon_carve.py`
generates that block -- run it and diff if a boundary is ever in doubt -- and
`--check` prints the objects whose cross-checks still complain.

    python3 tools/decomp/dungeon_carve.py --check
    python3 tools/decomp/dungeon_carve.py        # the YAML block

**2,566 asm stubs, 543,607 words known, 39.7% landed.** Each object is carved
`data` (its leading table of pointers into its own text) then `c` (text, to
the last `jr $ra` and its delay slot). Everything after the text -- animation
tables, further pointer tables, embedded assets, mastering filler -- stays
`bin` and is promoted per object by whoever decompiles it. One rule for 116
objects instead of 116 judgements, and it costs nothing, because a `bin`
segment reproduces its bytes exactly.

Five things the carve had to get right, each of which cost a build:

1. **`symbol_name_format` per segment.** 47 objects load at 0x80170800 and 66
   at 0x80024000, so without it they all generate `func_801708E8` and collide
   in this container's single link. `b01_$VRAM` gives `func_b01_801708E8`.
   Only the ROM case is overridden: out-of-segment references have no segment
   at all and fall through to the global format, which is what keeps
   `func_8003A7C4` shared instead of aliased 113 times.
2. **`exclusive_ram_id` names the RAM SPACE, not the object.** splat keys its
   overlay table by ROM *inside* the id and falls through to every other id
   when an address misses in its own, so 116 ids did the opposite of
   isolating them -- a branch label inside s62 came out `.Llo20_80026BE4` and
   the link came apart on hundreds of undefined `.L` symbols. Two ids:
   `dng_slot` and `dng_low`.
3. **`--no-check-sections` on the link.** These objects share RAM by design;
   overlapping VMAs are correct, and placement is by LMA throughout.
4. **The text start is NOT "the first prologue".** Nine objects open with a
   leaf function that has none, and that rule swallows them into the data
   subsegment where nothing disassembles them. It is the word after BOTH the
   last pointer into the object's own window and the last word that is not an
   R3000 instruction. Neither test alone is enough: a pointer like
   `0x80173a08` decodes as a fine `lb`, `0x01000340` is an `sll` with a
   non-zero rs that only rabbitizer rejects, `0xfffefffe` is a MIPS III `sd`
   that rabbitizer accepts and the R3000 does not have, and `0x0bb82000` is a
   valid `j` to 0x8EE08000, which is not RAM.
5. **`make` does not know about regenerated listings.** A `.c` file's
   `INCLUDE_ASM` dependencies are invisible to it, so a re-split leaves every
   `build/csrc/*.o` stale and the link fails on names from the PREVIOUS split.
   `rm -f build/csrc/ovl_dng_*.o` after `splat split`. Deleting the `.c` files
   too is what makes splat rewrite them when a boundary moves -- it skips a
   file that already exists, so a stale `INCLUDE_ASM` list survives otherwise.

**Two objects still have a complaint, and `--check` prints them rather than
hiding them.** b31f and lo20, the two largest, carry rodata ISLANDS between
functions -- Shift-JIS strings in lo20 at +0x5020, 1.6% of b31f's text across
eleven runs. spimdisasm emits `.word` for what it cannot decode, so the bytes
round-trip and the container is byte-identical; the cost is that the function
after each island loses its own symbol. Each is worth a rodata subsegment when
somebody reads it.

## The trap in this container

**Shift-JIS text decodes as valid MIPS.** A full-width Shift-JIS byte pair
`82xx` has top byte 0x82, so the word's opcode field is 0x20 -- `lb` -- and an
instruction-validity sieve reports 100% over a block of pure text. Two sectors
of the string table at +0xd800 and +0xe000 read as "100% valid, looks like
code"; they are the character-name block, byte-identical to the one `town.bin`
carries at the same offset. Density alone over-reports code here. The
function-start test is not fooled, because text produces no self-consistent
delta, which is the other reason to prefer it.

The second form of the same trap is quieter: **a word that decodes as `jal` is
not a call unless it is inside a function.** Both wrong claims in §7 came from
sweeping `jal` targets over a span that was 60% data.
