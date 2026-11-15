/*
 * The sixty bytes at `_gp` itself: a zero SVECTOR, four VRAM rectangles, four
 * scalars and one debug string.
 *
 * Covers rom 0x054194..0x0541D0 (vram 0x80080994..0x800809D0). `_gp` is
 * 0x80080994, so this run is the first thing in the small-data area and
 * libsn/SNMAIN's own module descriptor pins it: `__data` + `__datalen` is
 * exactly `_gp`.
 *
 *
 * WHY THIS IS A UNIT OF ITS OWN, AND WHAT THAT UNIT IS NOT
 *
 * It is NOT a claim that the original had a translation unit here. It is the
 * opposite: a marker that our carve cannot say which units these objects
 * belonged to, because over these sixty bytes `csrc/unk_72A8.c` is provably
 * MORE THAN ONE original translation unit. The proof is two relocation shapes
 * sixteen bytes apart, read out of our own object, which reproduces the
 * reference byte for byte:
 *
 *     00000008b8  R_MIPS_HI16    D_800809A8      <- absolute
 *     00000013a4  R_MIPS_GPREL16 D_800809B8      <- $gp-relative
 *
 * ASPSX could only use `$gp` for a symbol whose size it knew, which means
 * only for a symbol defined in the unit being assembled
 * (docs/addressing-and-translation-units.md). So the original unit holding
 * the function at +0x13A4 DEFINED D_800809B8, and the original unit holding
 * the `ClearImage(D_800809A8, ...)` call at +0x8B8 did NOT define
 * D_800809A8. Both functions are inside our `unk_72A8`, so `unk_72A8` is at
 * least two original units here -- and a single object file contributes a
 * single contiguous `.data` run, so no assignment of this run to our 38 units
 * can be right. `csrc/gprel_72A8.c` is in the same position for D_80080994
 * (absolute, `.extern D_80080994, 8`) against D_8008099C (`$gp`-relative).
 *
 * This is the same conclusion docs/data-map.md reaches statistically for
 * `game_data`, reached here by proof instead, on a run small enough to read.
 * When the text is carved into its original units, these definitions move
 * into them and this file goes away.
 *
 * Nothing here perturbs a reference, which is the point of putting them in a
 * unit that has no code: the three symbols the other units hold as tentative
 * definitions (`u32 D_8008099C;`, `u32 D_800809B8;`, `u32 D_800809BC;`) stay
 * COMMON in those objects and resolve to the definitions here, exactly as
 * they resolved to `asm/data/game_gp.data.o` before. Defining them in those
 * units INSTEAD would flip their absolute neighbours to GPREL16: gas goes
 * `$gp`-relative for a symbol it can see is small and local, and
 * D_800809A8, D_800809B0, D_800809C0, D_800809C4 and D_80080994 all have to
 * stay absolute.
 *
 *
 * THE SECTION IS `.sdata`, NOT `.data`
 *
 * This unit is `-G8` -- the default for `csrc/*.c`, and what the original
 * used -- so every object here is 8 bytes or fewer and cc1psx emits them all
 * under `.sdata`. That needs `auto_link_sections` to list `".sdata"`, which
 * splat does not do by default; see the note in splat/slus_006.14.yaml. The
 * alternative, a `-G0` unit whose objects land in `.data`, also links and is
 * also byte-identical, but it would put the run in a section the original did
 * not use and would not prove out the path `gp_data` needs next.
 *
 * ORDER IS THE LAYOUT: cc1psx emits file-scope objects in source order and
 * gives each one `.align 2`, so moving a definition moves the bytes.
 */

#include "common.h"

/* The identity rotation handed to RotMatrix by func_80033D54 in
   csrc/gprel_72A8.c -- the one object here the $gp oracle does place, since
   that unit is first in link order and reaches it at all. */
SVECTOR D_80080994 = {0, 0, 0, 0};

/* Written as well as read: `return D_8008099C = a;` in csrc/gprel_72A8.c, so
   this run is writable small data and not .rdata, which is what rules out
   folding it into csrc/str/names.c's read-only run immediately above. */
u32 D_8008099C = 0;

/* Four VRAM rectangles in the bottom-right corner of the frame buffer,
   x 0x3C0/0x3E0 at y 0x1DE. The first is unreferenced and byte-identical to
   the second; the last is split in two because csrc/unk_72A8.c reads its
   halves as words rather than as a RECT. */
RECT D_800809A0[1] = {{0x3C0, 0x1DE, 0x20, 0x22}};
RECT D_800809A8[1] = {{0x3C0, 0x1DE, 0x20, 0x22}};
RECT D_800809B0[1] = {{0x3E0, 0x1DE, 0x20, 0x21}};
u32 D_800809B8 = 0x01DE03E0;
u32 D_800809BC = 0x00100030;

/* Reached only by address, `(u32) D_800809C0`, so it stays a byte array. */
u8 D_800809C0[4] = {1, 0, 0, 0};

/* "XXX\n", passed by address to func_80043FB8 twice. */
u8 D_800809C4[8] = "XXX\n";

/* No code reaches this one; `game_data` holds two pointers to it, at rom
   0x03E45C and 0x03E5B8, and they are the reason it has a symbol at all. */
u32 D_800809CC = 0;
