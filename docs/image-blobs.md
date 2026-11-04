# Image blobs: the record format, and how to find every one on the disc

What the engine's texture/palette container looks like, where the blobs are,
and what `tools/scan-img.py` can and cannot tell you about them.

**Fixed findings only.** Record semantics and VRAM rectangles are properties of
the images; colour depth and palette choice are *not* in the data and are
called out as guesses where they appear.

## The loader

`func_80041344` (`csrc/unk_14934.c`) walks a blob as a stream of records:

```
u16 type      1..9; 0 or >9 ends the stream (the switch's `default: return`)
u16 size      the record's own byte size; 0 also ends the stream
u32 w0        payload offset, from the start of the blob -- every arm reads it
u32 w1, w2    meaning depends on the type
```

The C indexes the switch as `op = *(u16 *)p - 1`, so **C `case N` is on-disc
type `N + 1`**. `func_80041588` is the asynchronous sibling over the same
format, with the differences noted below.

| type | arm | what it does |
|---|---|---|
| 1 | `case 0` | texture, LZ-decompressed into the work buffer, then `LoadImage` |
| 2 | `case 1` | palette; `w2`'s low halfword is passed to `func_8003F80C` verbatim |
| 3 | `case 2` | payload, LZ-decompressed to **RAM**: `D_80080A8C + w1` (sibling: `w1` absolute) |
| 4 | `case 3` | payload, `memcpy` to RAM; `func_80041344` copies `w2 + w1` bytes to `D_80080A8C`, the sibling copies `w2` bytes to `w1` |
| 5 | `case 4` | texture, uncompressed, `LoadImage` straight from the blob |
| 6 | `case 5` | palette, flags `|= 2` |
| 7 | `case 6` | palette; `OR 0x8000` into entries `1 .. (w1>>16 << 4) - 1` first, then flags `|= 4` (the sibling ORs 2) |
| 8 | `case 7` | palette; per group of 16, skip entry 0 and `OR 0x8000` into the other 15, then flags `|= 2` |
| 9 | `case 8` | pointer-table fixup: `w1`'s low halfword pairs of words, each pair's second word gets the blob's load base added |

For types 1 and 5, `RECT` sits at `p + 8` -- `x`, `y`, `w`, `h`, with `w` in
16-bit VRAM words as `LoadImage` takes it. For the palette types, `p + 8` is
the tpage parameter, `p + 0xA` the palette count (of 16 colours each) and
`p + 0xC` the flags. `func_8003F80C` (`csrc/unk_12DEC.c:167`) turns the first
two into the upload rectangle; its flag word is **bit 0 = skip the
`LoadImage`**, **bit 1 = skip the VRAM-ownership bookkeeping in
`D_80082EC0`**. `func_8004068C` is the decompressor, ported in
`tools/make-img.py`.

### Record sizes are not uniform

Each arm reads only as far as it needs: type 3 stops at `w1`, so **its records
are 12 bytes**, and type 9's are 10. Treating 16 as the minimum silently drops
the whole compressed-payload family (59 records across 37 blobs, the audio
among them).

### Two sloppy shapes that are in the game

Both are real, both are in the shipped data, and a validator that rejects them
is wrong:

* **Half-height texture records.** 27 blobs carry two type-1 records at
  `(768, 0, 64, 128)` and `(768, 64, 64, 128)` whose streams each unpack to
  8192 bytes -- 64 rows, not 128. There is no second entry point: the only
  reader of `D_8006CDC8` is `func_8003E1C4`, which goes straight to
  `func_80041284` and `func_80041344`, and `arg1` is never advanced inside the
  walk, so both records decompress to the same scratch address and both
  `LoadImage` calls really do transfer 16384 bytes from an 8192-byte source.

  It still lands correctly, because the two records overlap by exactly the
  amount each is short:

  ```
  rec0  y=0,  h=128   rows   0..63  = rec0's data,  rows  64..127 = stale arena
  rec1  y=64, h=128   rows  64..127 = rec1's data,  rows 128..191 = stale arena
  ```

  Row 64 onwards is written twice and the second write wins, so the
  `(768, 0, 64, 128)` slot ends up entirely correct and the only casualty is
  the 64x64-word region below it, at `(768, 128)`. The texture page that
  defines the surrounding tiles (lba `0x1fa6`) leaves that slot empty, though
  a few other blobs do load 32-word tiles there.

  Two things confirm the halves are the top and bottom of one tile rather than
  two separate images: joining them gives a seamless 128-row texture, and the
  same VRAM slot loaded full-height from lba `0x1fa6` holds two stacked 64-row
  monster frames -- the same tile layout, delivered as one 16384-byte record
  instead of two 8192-byte ones. The split is how a single tile gets replaced
  frame by frame; `h` is simply the slot's height rather than the record's,
  and the measured row count is what to extract.
* **A raw texture that reads past its own blob.** The type-5 records at
  lba `0x395c` and `0x3964` want 16404 bytes from an 8-sector (16384-byte)
  blob, so the last 20 bytes come from whatever follows in the arena.

## Where the blobs are

The CD queue (`func_8003E4FC` kind 6, pumped through `func_8003E758`,
dispatched in `csrc/unk_11F0C.c:210`) takes 8-byte descriptors:

```
+0x00   (nsec << 23) | dest_phys_addr     dest == 0 means the D_80081480 arena
+0x04   LBA, straight into CdIntToPos, then CdRead(nsec, addr, 0x80)
```

`func_80041284` is the synchronous "load this descriptor and walk it" entry
point; `D_8006CDC8[]` is a 27-entry table of pointers to descriptors indexed by
image id (`func_8003E1C4(id)`), and `func_80048500` indexes
`D_800710E4[b] + a * 8`, which is where the 8-byte stride is visible.

**The executable only holds the boot and UI descriptors.** Scanning all of
`slus_006.14` plus both mapped code overlays finds 161 blobs / 329 textures.
The engine builds the rest at runtime as a container base plus an offset, so
they are not word pairs anywhere in the ROM. Sweeping the disc itself for
sectors that start a valid stream finds 121 more blobs and 355 more textures,
for 684 in 5.8 MiB. Nothing in either overlay's data is a descriptor, which
matches `docs/overlays.md`'s finding that no descriptor in the ROM targets a
code overlay.

## The tool

`tools/scan-img.py`. Default run scans the executable and the two overlays;
`--sweep` adds the whole-disc pass (about ten seconds).

```
uv run python tools/scan-img.py                        # manifest to stdout
uv run python tools/scan-img.py --sweep --json m.json
uv run python tools/scan-img.py --sweep --extract blobs/ --png pngs/
uv run python tools/scan-img.py --lba 0x3154 --nsec 8  # one blob, no scan
```

A candidate is kept only if its payload parses: types in range, clean
termination, every rectangle inside VRAM, every palette parameter in
`0x7000..0x7fff`, and -- the strongest filter -- every compressed record
unpacking to a whole number of rows that fits the record's declared height.
1301 descriptor-shaped word pairs are rejected by that, with no survivors that
look real on inspection.

### What the format does not tell you

**Colour depth is not in the record.** `w` is a VRAM word count, so the same
record is 4bpp at `w * 4` pixels wide or 8bpp at `w * 2`; the game knows which
from the drawing primitive's tpage. 4bpp is right for nearly everything --
the title background at lba `0xb66` is the 8bpp exception, and it renders as a
384x512 image.

**Nor is the palette.** A CLUT record carries up to 16 palettes and the
texture records never name one; the primitive's CLUT id picks it at draw time.
`--png` defaults to whichever palette gives the texture the most contrast,
which is for triage, not fidelity. `--palette N` and `--all-cluts` are there
for when it matters. Favouring saturation instead was measured and is worse:
it picks the lurid end of a palette set on the town atlases at lba `0x167e`.

77 of the 684 textures sit in blobs with no palette record at all, so they
cannot be rendered without supplying one from elsewhere.

### The record format is not the only asset path

`func_8003F8F8` walks a different table (`Unk3F8F8`: a pointer plus a `RECT`
per entry) and `func_8003F9E0` another (`UnkEntry`, palettes), both over data
already in RAM. Anything loaded through those is outside what this tool sees.
