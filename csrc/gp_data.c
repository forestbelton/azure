/*
 * The small-data run above `_gp`: 287 objects, 2,508 bytes.
 *
 * Covers rom 0x05426C..0x054C38 (vram 0x80080A6C..0x80081438). That upper
 * bound is measured twice and the two agree: the last non-zero byte in the
 * old `gp_data` blob is at rom 0x54C35, and `dataverify.py`'s NEXT YAML
 * BOUNDARY for these definitions is 0x54C38. Above it is `.sbss` and the
 * over-rounded tail of `t_size`, 968 bytes of zero that no C definition can
 * supply -- a tentative definition is NOBITS and contributes nothing -- so
 * that part stays the `gp_sbss` subsegment.
 *
 *
 * WHY THIS IS A UNIT OF ITS OWN
 *
 * The same reason as `csrc/game_gp.c`, and that file has the proof: over this
 * region `csrc/unk_72A8.c` and `csrc/unk_1C540.c` each reach some objects
 * `$gp`-relatively and others absolutely, which means each of them is more
 * than one original translation unit here. `objdump -r build/csrc/*.o |
 * grep GPREL16` names the defining unit for the objects that are reached that
 * way -- 68 across this run and `game_gp` -- and they fall into two
 * interleaved monotone runs, `.sdata` and `.sbss`, which one object file
 * cannot produce. So the definitions live in a unit with no code, where they
 * cannot perturb a relocation, until the text carve lands and they move into
 * their real units.
 *
 *
 * THE THREE THINGS THE SHAPE OF THIS FILE IS FORCED BY
 *
 * 1. `-G0`, set for this one object in the Makefile. The run interleaves
 *    objects of 8 bytes and fewer with 24 larger ones, and at the `-G8` the
 *    original used, those two groups go to `.sdata` and `.data` -- two
 *    sections, one subsegment, impossible. At `-G0` everything is `.data`.
 *    The cost is that the section name here is not the original's; the bytes
 *    and their order are.
 *
 * 2. ONE OBJECT PER `dlabel`, sized to its SPAN -- the rule
 *    docs/owning-data.md §8 states, and here it is also checkable: the
 *    listing's emitted bytes equal the span for 282 of the 283 `dlabel`s, the
 *    exception being `D_80080B6C`, whose `.asciz "#"` is two bytes inside a
 *    four-byte span.
 *
 * 3. NO ARRAY MAY START AT AN ADDRESS THAT IS NOT 4-ALIGNED. gcc 2.7.2's
 *    `DATA_ALIGNMENT` (mips.h) raises every ARRAY_TYPE, RECORD_TYPE and
 *    UNION_TYPE to `BITS_PER_WORD`, and `__attribute__((aligned(1)))` does
 *    NOT lower it again -- measured, the directive is simply ignored and
 *    `.align 2` is emitted anyway. Scalars keep their natural alignment. So a
 *    multi-element object at a non-4-aligned address is written as one scalar
 *    per element, the later ones under invented `D_` names. Three objects
 *    need it: D_80080A8A and D_80080AD2 (two bytes each) and D_80080B0A
 *    (three halfwords). Get it wrong and the drift starts at the NEXT
 *    object, not at the one with the bad type -- which is what
 *    `dataverify.py`'s `layout` column is for.
 *
 *
 * THE TYPES ARE THE LISTING'S ELEMENT WIDTHS, NOT THE TREE'S DECLARATIONS
 *
 * docs/owning-data.md §8 rule 2 says to use the type the unit already
 * declared, because writing a different one is a `conflicting types` error
 * and that error is the gate working. That gate does not exist here: the
 * definitions are in a unit of their own, so nothing would conflict. Adopting
 * the declarations anyway was measured and deliberately not done -- of the 96
 * objects the tree declares, 32 are scalars whose size equals the span and 19
 * are arrays that divide it, but 22 are struct types (`Unk41284`, `CVECTOR`)
 * that would have to be laid out field by field, 21 are scalars SMALLER than
 * their span and would each need a value object plus a pad object, and 2 have
 * conflicting declarations already. That is 45 hand-written layouts, each a
 * chance to be byte-identical and wrong, for a file that dissolves into the
 * real units when the text carve lands.
 *
 * So every object here carries the listing's element width and its full span,
 * and every object the tree declares carries that declaration in a comment
 * above it. Nothing is lost and nothing is invented; a later pass that wants
 * the declared types has them in place, with the three groups above naming
 * what each costs.
 *
 * `#include "types.h"` and not `"common.h"` for the same reason
 * `csrc/str/spells.c` does it: the declarations must not meet the definitions.
 *
 *
 * VERIFYING A CHANGE
 *
 *     python3 tools/decomp/dataverify.py csrc/gp_data.c --window gp_data
 *
 * compiles this at `-G0`, compares every object against `data/slus_006.14`
 * with relocated words masked, and checks each object's offset inside `.data`
 * against its vram. Three words are relocated and each was read against the
 * listing: D_80080A90 and both halves of D_80080B34/D_80080B40 hold
 * `.word D_8014A000` and `.word D_80080B3C`. The scalar one needs its address
 * taken -- `(u32)&D_80080B3C`, not a cast of its value.
 */

#include "types.h"

extern u8 D_8014A000[];
extern u32 D_80080B3C;

/* the tree declares: s32 D_80080A6C */
u32 D_80080A6C = 0x00000004;

u32 D_80080A70 = 0x00200000;

u32 D_80080A74[2] = {
    0x00001000, 0x00000001,
};

/* the tree declares: u32 D_80080A7C */
u32 D_80080A7C = 0x00038000;

/* the tree declares: u32 D_80080A80 */
u32 D_80080A80 = 0x00000000;

/* the tree declares: u8 D_80080A84 */
u8 D_80080A84 = 0x02;

/* the tree declares: u8 D_80080A85 */
u8 D_80080A85 = 0x00;

/* the tree declares: u8 D_80080A86 */
u8 D_80080A86 = 0x00;

/* the tree declares: u8 D_80080A87 */
u8 D_80080A87 = 0x02;

/* the tree declares: s8 D_80080A88 */
u8 D_80080A88 = 0x00;

/* the tree declares: s8 D_80080A89 */
u8 D_80080A89 = 0x00;

/* the tree declares: u8 D_80080A8A */
u8 D_80080A8A = 0x00;
u8 D_80080A8B = 0x00;

/* the tree declares: u32 D_80080A8C */
u32 D_80080A8C = 0x00000000;

/* the tree declares: u8 *D_80080A90 */
u32 D_80080A90 = (u32)D_8014A000;

/* the tree declares: s16 D_80080A94 */
u16 D_80080A94[2] = {
    0x7FFF, 0x0000,
};

/* the tree declares: s16 D_80080A98 */
u16 D_80080A98[2] = {
    0x4800, 0x0000,
};

/* the tree declares: u16 D_80080A9C */
u16 D_80080A9C[2] = {
    0x7800, 0x0000,
};

/* the tree declares: u8 D_80080AA0 */
u8 D_80080AA0[4] = {
    0x00, 0x00, 0x00, 0x00,
};

/* the tree declares: u16 D_80080AA4 */
u16 D_80080AA4 = 0x0000;

/* the tree declares: u16 D_80080AA6 */
u16 D_80080AA6 = 0x0000;

/* the tree declares: u16 D_80080AA8 */
u16 D_80080AA8 = 0x0000;

/* the tree declares: u8 D_80080AAA */
u16 D_80080AAA = 0x0000;

/* the tree declares: u16 D_80080AAC */
u32 D_80080AAC = 0x00000000;

/* the tree declares: u32 D_80080AB0 */
u32 D_80080AB0 = 0x00000000;

/* the tree declares: u8 D_80080AB4[] */
u32 D_80080AB4[2] = {
    0x8F826A82, 0x00008882,
};

/* the tree declares: s16 D_80080ABC */
u16 D_80080ABC = 0x0000;

/* the tree declares: s16 D_80080ABE */
u16 D_80080ABE = 0x0000;

/* the tree declares: u8 D_80080AC0 */
u32 D_80080AC0[2] = {
    0x0102C000, 0x0000075A,
};

/* the tree declares: u8 D_80080AC8 */
u32 D_80080AC8[2] = {
    0x0102C000, 0x00003A51,
};

/* the tree declares: u8 D_80080AD0 */
u8 D_80080AD0[2] = {
    0x00, 0x00,
};

/* the tree declares: u16 D_80080AD2 */
u8 D_80080AD2 = 0x00;
u8 D_80080AD3 = 0x00;

/* the tree declares: u16 D_80080AD4 */
u16 D_80080AD4[2] = {
    0x0000, 0x0000,
};

/* the tree declares: u32 D_80080AD8 */
u32 D_80080AD8 = 0x00000000;

/* the tree declares: u32 D_80080ADC */
u32 D_80080ADC[5] = {
    0x00000000, 0x04000000, 0x0000395C, 0x04000000, 0x00003964,
};

/* the tree declares: u8 D_80080AF0[] */
u8 D_80080AF0[3] = {
    0x00, 0x00, 0x00,
};

/* the tree declares: u8 D_80080AF3 */
u8 D_80080AF3 = 0xFF;

/* the tree declares: u32 *D_80080AF4[]; u32 *D_80080AF4big[4] __asm__("D_80080AF4"); u32 *D_80080AF4s[2] __asm__("D_80080AF4") */
u32 D_80080AF4[2] = {
    0x00000000, 0x00000000,
};

/* the tree declares: s16 D_80080AFC */
u32 D_80080AFC = 0x00000000;

/* the tree declares: u16 D_80080B00 */
u32 D_80080B00[2] = {
    0x00000000, 0x00000000,
};

/* the tree declares: u16 D_80080B08 */
u16 D_80080B08 = 0x0000;

/* the tree declares: u16 D_80080B0A */
u16 D_80080B0A = 0x0000;
u16 D_80080B0C = 0x0020;
u16 D_80080B0E = 0x0080;

/* the tree declares: u16 D_80080B10 */
u16 D_80080B10 = 0x0000;

/* the tree declares: u16 D_80080B12 */
u16 D_80080B12 = 0x0000;

/* the tree declares: u16 D_80080B14 */
u32 D_80080B14 = 0x00000000;

/* the tree declares: u8 D_80080B18[] */
u8 D_80080B18[3] = {
    0x00, 0x02, 0x05,
};

/* the tree declares: u8 D_80080B1B */
u8 D_80080B1B = 0x00;

/* the tree declares: u8 D_80080B1C */
u8 D_80080B1C[4] = {
    0x00, 0x00, 0x00, 0x00,
};

/* the tree declares: u8 D_80080B20[] */
u32 D_80080B20[2] = {
    0x03030100, 0x02020202,
};

/* the tree declares: CVECTOR D_80080B28[] */
u32 D_80080B28 = 0x008C9C9C;

/* the tree declares: CVECTOR D_80080B2C[] */
u32 D_80080B2C = 0x009CBCBC;

/* the tree declares: u8 D_80080B30 */
u8 D_80080B30[4] = {
    0x01, 0x03, 0x02, 0x04,
};

/* the tree declares: u8 D_80080B34[] */
u32 D_80080B34[2] = {
    (u32)&D_80080B3C, 0x8002E4C0,
};

u32 D_80080B3C = 0x00000000;

/* the tree declares: u8 D_80080B40[] */
u32 D_80080B40[2] = {
    (u32)&D_80080B3C, 0x8002E540,
};

/* the tree declares: u8 D_80080B48[]; u8 D_80080B48s[8] __asm__("D_80080B48") */
u32 D_80080B48[2] = {
    0x04825000, 0x00006076,
};

/* the tree declares: u8 D_80080B50[] */
u32 D_80080B50 = 0x00001300;

/* the tree declares: u8 D_80080B54[8] */
u32 D_80080B54 = 0x00808080;

/* the tree declares: u32 D_80080B58 */
u32 D_80080B58 = 0x0000FD80;

/* the tree declares: u32 D_80080B5C */
u32 D_80080B5C = 0x00000000;

/* the tree declares: u32 D_80080B60 */
u32 D_80080B60 = 0x00000000;

/* the tree declares: u32 D_80080B64 */
u32 D_80080B64 = 0x00000400;

/* the tree declares: u8 D_80080B68[8] */
u32 D_80080B68 = 0x01010116;

/* the tree declares: u8 D_80080B6C[8] */
char D_80080B6C[4] = "#";

/* the tree declares: u8 D_80080B70[] */
u8 D_80080B70[4] = {
    0x81, 0x40, 0x00, 0x00,
};

u32 D_80080B74[2] = {
    0x93827482, 0x00008582,
};

u32 D_80080B7C = 0x00000000;

/* the tree declares: u32 D_80080B80 */
u32 D_80080B80 = 0x0098B8B8;

/* the tree declares: u32 D_80080B84 */
u32 D_80080B84 = 0x00586868;

/* the tree declares: u8 D_80080B88[8] */
u32 D_80080B88[2] = {
    0xFFF8FFE9, 0x0010002E,
};

/* the tree declares: u8 D_80080B90[8] */
u32 D_80080B90 = 0x00A0A0A0;

/* the tree declares: u32 D_80080B94 */
u32 D_80080B94 = 0x00000001;

/* the tree declares: s32 D_80080B98 */
u32 D_80080B98 = 0x00000000;

/* the tree declares: Unk41284 D_80080B9C */
u32 D_80080B9C[2] = {
    0x1891AC98, 0x00001DCA,
};

u32 D_80080BA4[2] = {
    0x04800000, 0x00001DC1,
};

/* the tree declares: Unk41284 D_80080BAC */
u32 D_80080BAC[2] = {
    0x04820000, 0x00001DC1,
};

/* the tree declares: Unk41284 D_80080BB4 */
u32 D_80080BB4[2] = {
    0x05020000, 0x00001375,
};

/* the tree declares: Unk41284 D_80080BBC */
u32 D_80080BBC[2] = {
    0x0C020000, 0x00001F20,
};

/* the tree declares: Unk41284 D_80080BC4 */
u32 D_80080BC4[2] = {
    0x0C020000, 0x00001F38,
};

/* the tree declares: Unk41284 D_80080BCC */
u32 D_80080BCC[2] = {
    0x0B020000, 0x00001F50,
};

/* the tree declares: Unk41284 D_80080BD4 */
u32 D_80080BD4[2] = {
    0x03020000, 0x00001F6F,
};

/* the tree declares: Unk41284 D_80080BDC */
u32 D_80080BDC[2] = {
    0x03020000, 0x00001F75,
};

/* the tree declares: Unk41284 D_80080BE4 */
u32 D_80080BE4[2] = {
    0x03020000, 0x00001F7B,
};

/* the tree declares: Unk41284 D_80080BEC */
u32 D_80080BEC[2] = {
    0x03020000, 0x00001F81,
};

/* the tree declares: Unk41284 D_80080BF4 */
u32 D_80080BF4[2] = {
    0x03020000, 0x00001F87,
};

/* the tree declares: Unk41284 D_80080BFC */
u32 D_80080BFC[2] = {
    0x03020000, 0x00001F8D,
};

/* the tree declares: Unk41284 D_80080C04 */
u32 D_80080C04[2] = {
    0x03020000, 0x00001F93,
};

/* the tree declares: Unk41284 D_80080C0C */
u32 D_80080C0C[2] = {
    0x03020000, 0x00001F99,
};

/* the tree declares: Unk41284 D_80080C14 */
u32 D_80080C14[2] = {
    0x03020000, 0x00001F9F,
};

/* the tree declares: Unk41284 D_80080C1C */
u32 D_80080C1C[2] = {
    0x0091AC98, 0x00001FFB,
};

u32 D_80080C24[2] = {
    0x11800000, 0x00001FA6,
};

u32 D_80080C2C[2] = {
    0x19000000, 0x00001FC9,
};

u32 D_80080C34 = 0x00000000;

u32 D_80080C38[2] = {
    0x8B088760, 0x00000C1B,
};

u32 D_80080C40[2] = {
    0x5D888760, 0x0000315C,
};

u32 D_80080C48[2] = {
    0x07088760, 0x0000075C,
};

/* the tree declares: u8 D_80080C50 */
u32 D_80080C50[2] = {
    0x42976800, 0x00000118,
};

u32 D_80080C58[2] = {
    0x03000000, 0x00003FDE,
};

u32 D_80080C60[2] = {
    0x01000000, 0x00004295,
};

u32 D_80080C68[2] = {
    0x02000000, 0x00003FA3,
};

u32 D_80080C70[54] = {
    0x09800000, 0x00003FC3, 0x07000000, 0x00004287, 0x0E800000, 0x00003F86,
    0x09000000, 0x00003FE9, 0x0A000000, 0x00004003, 0x09000000, 0x0000401E,
    0x08800000, 0x00004037, 0x09000000, 0x00004050, 0x08000000, 0x0000406A,
    0x09800000, 0x0000407F, 0x09000000, 0x00004097, 0x08800000, 0x000040B2,
    0x09000000, 0x000040CA, 0x09000000, 0x000040E1, 0x09000000, 0x000040FA,
    0x09800000, 0x00004111, 0x09000000, 0x0000412D, 0x08000000, 0x00004146,
    0x08800000, 0x0000415B, 0x08000000, 0x00004171, 0x08800000, 0x00004188,
    0x09000000, 0x000041A0, 0x09800000, 0x000041B9, 0x09800000, 0x000041D3,
    0x0A000000, 0x000041ED, 0x08000000, 0x00004209, 0x02000000, 0x00003FBF,
};

u32 D_80080D48[2] = {
    0x04000000, 0x00003FD6,
};

u32 D_80080D50[2] = {
    0x04800000, 0x0000427E,
};

u32 D_80080D58[2] = {
    0x04000000, 0x00003154,
};

u32 D_80080D60[2] = {
    0x02800000, 0x00003FE4,
};

u32 D_80080D68[2] = {
    0x04000000, 0x00003FFB,
};

u32 D_80080D70[2] = {
    0x03800000, 0x00004017,
};

u32 D_80080D78[2] = {
    0x03800000, 0x00004030,
};

u32 D_80080D80[2] = {
    0x04000000, 0x00004048,
};

u32 D_80080D88[2] = {
    0x04000000, 0x00004062,
};

u32 D_80080D90[2] = {
    0x02800000, 0x0000407A,
};

u32 D_80080D98[2] = {
    0x02800000, 0x00004092,
};

u32 D_80080DA0[2] = {
    0x04800000, 0x000040A9,
};

u32 D_80080DA8[2] = {
    0x03800000, 0x000040C3,
};

u32 D_80080DB0[2] = {
    0x02800000, 0x000040DC,
};

u32 D_80080DB8[2] = {
    0x03800000, 0x000040F3,
};

u32 D_80080DC0[2] = {
    0x02800000, 0x0000410C,
};

u32 D_80080DC8[2] = {
    0x04800000, 0x00004124,
};

u32 D_80080DD0[2] = {
    0x03800000, 0x0000413F,
};

u32 D_80080DD8[2] = {
    0x02800000, 0x00004156,
};

u32 D_80080DE0[2] = {
    0x02800000, 0x0000416C,
};

u32 D_80080DE8[2] = {
    0x03800000, 0x00004181,
};

u32 D_80080DF0[2] = {
    0x03800000, 0x00004199,
};

u32 D_80080DF8[2] = {
    0x03800000, 0x000041B2,
};

u32 D_80080E00[2] = {
    0x03800000, 0x000041CC,
};

u32 D_80080E08[2] = {
    0x03800000, 0x000041E6,
};

u32 D_80080E10[2] = {
    0x04000000, 0x00004201,
};

u32 D_80080E18[4] = {
    0x0A800000, 0x0000396C, 0x07800000, 0x00003981,
};

/* the tree declares: Unk41284 D_80080E28 */
u32 D_80080E28[4] = {
    0x05000000, 0x00000B5C, 0x05000000, 0x00003993,
};

u32 D_80080E38[2] = {
    0x30800000, 0x00000B66,
};

u32 D_80080E40[2] = {
    0x01800000, 0x00000BC7,
};

u32 D_80080E48[2] = {
    0x42176800, 0x000039CD,
};

u32 D_80080E50[2] = {
    0x43975800, 0x00001A93,
};

u32 D_80080E58[18] = {
    0x22175800, 0x00001B49, 0x00800000, 0x00004297, 0x00800000, 0x00003FB1,
    0x01000000, 0x00003FA7, 0x01000000, 0x00003FA9, 0x01000000, 0x00003FAB,
    0x01000000, 0x00003FAD, 0x01000000, 0x00003FAF, 0x00000000, 0x00003033,
};

/* the tree declares: Unk41284 D_80080EA0 */
u32 D_80080EA0[12] = {
    0x0F000000, 0x00001A60, 0x0A800000, 0x00001A7E, 0x01000000, 0x00001D4C,
    0x08000000, 0x00001D75, 0x07000000, 0x00001D85, 0x03000000, 0x00001D97,
};

/* the tree declares: Unk41284 D_80080ED0 */
u32 D_80080ED0[2] = {
    0x0C000000, 0x00001054,
};

/* the tree declares: Unk41284 D_80080ED8 */
u32 D_80080ED8[8] = {
    0x05800000, 0x00001049, 0x08800000, 0x00001D4E, 0x01800000, 0x00003990,
    0x02016000, 0x0000431B,
};

u32 D_80080EF8[16] = {
    0x01800000, 0x00002BCD, 0x02000000, 0x00002BD0, 0x02000000, 0x00002BD4,
    0x01000000, 0x00002BD8, 0x02000000, 0x00002BDA, 0x02000000, 0x00002BDE,
    0x01000000, 0x00002BE2, 0x01000000, 0x00002BE4,
};

u32 D_80080F38[2] = {
    0x01800000, 0x00002BE6,
};

/* the tree declares: u8 D_80080F40[8] */
u32 D_80080F40[8] = {
    0x01000000, 0x00002BE9, 0x01800000, 0x00002BEB, 0x02000000, 0x00002BEE,
    0x01000000, 0x00002BF2,
};

/* the tree declares: u8 D_80080F60[8] */
u32 D_80080F60[14] = {
    0x02000000, 0x00002BF4, 0x02000000, 0x00002BF8, 0x02000000, 0x00002BFC,
    0x01000000, 0x00002C00, 0x01000000, 0x00002C02, 0x02000000, 0x00002C04,
    0x01800000, 0x00002C08,
};

/* the tree declares: u8 D_80080F98[] */
u32 D_80080F98[2] = {
    0x02000000, 0x00002C0B,
};

u32 D_80080FA0[8] = {
    0x01000000, 0x00002C0F, 0x02000000, 0x00002C11, 0x01000000, 0x00002C15,
    0x01000000, 0x00002C17,
};

u32 D_80080FC0[2] = {
    0x02000000, 0x00002C19,
};

u32 D_80080FC8[2] = {
    0x01000000, 0x00002C1D,
};

u32 D_80080FD0[2] = {
    0x01800000, 0x00002C1F,
};

u32 D_80080FD8[2] = {
    0x01800000, 0x00002C22,
};

u32 D_80080FE0[2] = {
    0x01000000, 0x00002C25,
};

u32 D_80080FE8[2] = {
    0x01000000, 0x00002C27,
};

u32 D_80080FF0[8] = {
    0x02000000, 0x00002C29, 0x02000000, 0x00002C2D, 0x02000000, 0x00002C31,
    0x01000000, 0x00002C35,
};

/* the tree declares: u8 D_80081010[8] */
u32 D_80081010[2] = {
    0x01800000, 0x00002C37,
};

/* the tree declares: u8 D_80081018 */
u32 D_80081018[2] = {
    0x01000000, 0x00002C3A,
};

u32 D_80081020[2] = {
    0x049C4640, 0x00003F55,
};

u32 D_80081028[2] = {
    0x039C4640, 0x00003E9A,
};

u32 D_80081030[2] = {
    0x041C4640, 0x00003EA1,
};

u32 D_80081038[2] = {
    0x049C4640, 0x00003EA9,
};

u32 D_80081040[2] = {
    0x041C4640, 0x00003EB2,
};

u32 D_80081048[2] = {
    0x031C4640, 0x00003EBA,
};

u32 D_80081050[2] = {
    0x031C4640, 0x00003EC0,
};

u32 D_80081058[2] = {
    0x049C4640, 0x00003EC6,
};

u32 D_80081060[2] = {
    0x059C4640, 0x00003ECF,
};

u32 D_80081068[2] = {
    0x031B9000, 0x00003EDA,
};

u32 D_80081070[2] = {
    0x3A800000, 0x00003EE0,
};

u32 D_80081078[2] = {
    0x029BEE40, 0x000018E3,
};

u32 D_80081080[2] = {
    0x021BEE40, 0x00001470,
};

u32 D_80081088[2] = {
    0x021BEE40, 0x000011E4,
};

u32 D_80081090[2] = {
    0x019BEE40, 0x00001486,
};

u32 D_80081098[2] = {
    0x021BEE40, 0x000016E7,
};

u32 D_800810A0[2] = {
    0x021BEE40, 0x00001955,
};

u32 D_800810A8[2] = {
    0x031BEE40, 0x00001978,
};

u32 D_800810B0[2] = {
    0x019BEE40, 0x000018FC,
};

u32 D_800810B8[2] = {
    0x021BEE40, 0x000019A6,
};

u32 D_800810C0[2] = {
    0x021BEE40, 0x000011C8,
};

u32 D_800810C8[2] = {
    0x029BEE40, 0x000011CC,
};

u32 D_800810D0[2] = {
    0x031BEE40, 0x000011D1,
};

u32 D_800810D8[2] = {
    0x031BEE40, 0x000011D7,
};

u32 D_800810E0[2] = {
    0x039BEE40, 0x000011DD,
};

u32 D_800810E8[2] = {
    0x021BEE40, 0x00001458,
};

u32 D_800810F0[2] = {
    0x029BEE40, 0x0000145C,
};

u32 D_800810F8[2] = {
    0x039BEE40, 0x00001461,
};

u32 D_80081100[2] = {
    0x041BEE40, 0x00001468,
};

u32 D_80081108[2] = {
    0x009BEE40, 0x00001774,
};

u32 D_80081110[2] = {
    0x011BEE40, 0x0000179E,
};

u32 D_80081118[2] = {
    0x019BEE40, 0x0000183E,
};

u32 D_80081120[2] = {
    0x019BEE40, 0x00001810,
};

u32 D_80081128[2] = {
    0x021BEE40, 0x000014D4,
};

u32 D_80081130[2] = {
    0x031BEE40, 0x000014EE,
};

u32 D_80081138[2] = {
    0x019BEE40, 0x00001581,
};

u32 D_80081140[2] = {
    0x031BEE40, 0x00001678,
};

u32 D_80081148[2] = {
    0x059BEE40, 0x000015D1,
};

u32 D_80081150[2] = {
    0x049BEE40, 0x00001584,
};

u32 D_80081158[2] = {
    0x029BEE40, 0x00001620,
};

u32 D_80081160[2] = {
    0x009BEE40, 0x000011E8,
};

u32 D_80081168[2] = {
    0x009BEE40, 0x00001873,
};

u32 D_80081170[2] = {
    0x021BEE40, 0x000011EC,
};

u32 D_80081178[2] = {
    0x019BEE40, 0x000011E9,
};

u32 D_80081180[2] = {
    0x031BEE40, 0x00004219,
};

u32 D_80081188[2] = {
    0x031BEE40, 0x00000719,
};

u32 D_80081190[2] = {
    0x031B9000, 0x000010E2,
};

u32 D_80081198[2] = {
    0x3A000000, 0x000010E8,
};

u32 D_800811A0[2] = {
    0x1B800000, 0x00000722,
};

u32 D_800811A8[2] = {
    0x019BC000, 0x0000071F,
};

u32 D_800811B0[2] = {
    0x12000000, 0x00003E55,
};

u32 D_800811B8[2] = {
    0x011BD020, 0x00003E53,
};

u32 D_800811C0[2] = {
    0x0F800000, 0x00003E7B,
};

u32 D_800811C8[2] = {
    0x011BDE40, 0x00003E79,
};

u32 D_800811D0[2] = {
    0x0E000000, 0x000013B3,
};

u32 D_800811D8[2] = {
    0x011BDE40, 0x000013B1,
};

u32 D_800811E0[2] = {
    0x02800000, 0x00001737,
};

u32 D_800811E8[2] = {
    0x011BDE40, 0x00001735,
};

u32 D_800811F0[2] = {
    0x12000000, 0x000010BE,
};

u32 D_800811F8[2] = {
    0x011BD020, 0x000010BC,
};

u32 D_80081200[2] = {
    0x0D800000, 0x0000154C,
};

u32 D_80081208[2] = {
    0x011BDE40, 0x0000154A,
};

u32 D_80081210[2] = {
    0x0F800000, 0x000016B4,
};

u32 D_80081218[2] = {
    0x011BDE40, 0x000016B2,
};

u32 D_80081220[2] = {
    0x0E800000, 0x0000164B,
};

u32 D_80081228[2] = {
    0x011BDE40, 0x00001649,
};

u32 D_80081230[2] = {
    0x0B000000, 0x0000192D,
};

u32 D_80081238[2] = {
    0x011BDE40, 0x0000192B,
};

u32 D_80081240[2] = {
    0x07800000, 0x000014BB,
};

u32 D_80081248[2] = {
    0x011BDE40, 0x000014B9,
};

u32 D_80081250[2] = {
    0x05000000, 0x000015BC,
};

u32 D_80081258[2] = {
    0x011BDE40, 0x000015BA,
};

u32 D_80081260[2] = {
    0x0F800000, 0x000015FD,
};

u32 D_80081268[2] = {
    0x011BDE40, 0x000015FB,
};

u32 D_80081270[2] = {
    0x07000000, 0x0000139B,
};

u32 D_80081278[2] = {
    0x011BDE40, 0x00001399,
};

u32 D_80081280[2] = {
    0x0A800000, 0x000017C3,
};

u32 D_80081288[2] = {
    0x011BDE40, 0x000017C1,
};

u32 D_80081290[2] = {
    0x03000000, 0x000013AB,
};

u32 D_80081298[2] = {
    0x011BDE40, 0x000013A9,
};

u32 D_800812A0[2] = {
    0x0F800000, 0x00001A14,
};

u32 D_800812A8[2] = {
    0x011BDE40, 0x00001A12,
};

u32 D_800812B0[2] = {
    0x0F800000, 0x00001A3E,
};

u32 D_800812B8[2] = {
    0x011BDE40, 0x00001A3C,
};

u32 D_800812C0[2] = {
    0x00800000, 0x00000759,
};

/* the tree declares: u8 D_800812C8[] */
u32 D_800812C8[2] = {
    0x00816000, 0x00001CD5,
};

/* the tree declares: u8 D_800812D0[] */
u32 D_800812D0[10] = {
    0x00816000, 0x00001C1C, 0x02016000, 0x00001C12, 0x02016000, 0x00001BED,
    0x00816000, 0x00001CE8, 0x04016000, 0x00001CF5,
};

/* the tree declares: u8 D_800812F8 */
u32 D_800812F8[2] = {
    0x09816000, 0x0000115C,
};

u32 D_80081300 = 0x00004881;

u32 D_80081304[2] = {
    0x8F826182, 0x00009882,
};

u32 D_8008130C[2] = {
    0x83826882, 0x00008582,
};

u32 D_80081314[2] = {
    0x95826682, 0x00009982,
};

u32 D_8008131C[2] = {
    0x74827982, 0x00000000,
};

u32 D_80081324[2] = {
    0x68826A82, 0x00006382,
};

u32 D_8008132C[2] = {
    0xA2829294, 0x00009091,
};

u32 D_80081334[2] = {
    0xCC82CE97, 0x00009091,
};

u32 D_8008133C[2] = {
    0xA282958D, 0x00009091,
};

u32 D_80081344[2] = {
    0xA282C290, 0x00009091,
};

u32 D_8008134C[2] = {
    0xA282D490, 0x00009091,
};

u32 D_80081354[2] = {
    0xA2829294, 0x0000C08E,
};

u32 D_8008135C[2] = {
    0xCC82CE97, 0x0000C08E,
};

u32 D_80081364[2] = {
    0xA282958D, 0x0000C08E,
};

u32 D_8008136C[2] = {
    0xA282C290, 0x0000C08E,
};

u32 D_80081374[2] = {
    0xA282D490, 0x0000C08E,
};

u32 D_8008137C[2] = {
    0x81826B82, 0x00009282,
};

u32 D_80081384[2] = {
    0x85827282, 0x00008182,
};

u32 D_8008138C[2] = {
    0xA2829294, 0x0000ED8E,
};

u32 D_80081394[2] = {
    0xA282958D, 0x0000ED8E,
};

u32 D_8008139C[2] = {
    0xA282C290, 0x0000ED8E,
};

u32 D_800813A4[2] = {
    0xA282D490, 0x0000ED8E,
};

u32 D_800813AC[2] = {
    0xAD82508B, 0x0000CA8B,
};

u32 D_800813B4[2] = {
    0xA2829889, 0x0000CA8B,
};

u32 D_800813BC[2] = {
    0xA282798C, 0x0000CA8B,
};

u32 D_800813C4[2] = {
    0xA282648F, 0x0000CA8B,
};

u32 D_800813CC[2] = {
    0xA282C38C, 0x0000E997,
};

u32 D_800813D4 = 0x00005082;

u32 D_800813D8[2] = {
    0x85827182, 0x00008482,
};

u32 D_800813E0[2] = {
    0x69838F83, 0x00000000,
};

u32 D_800813E8[2] = {
    0x7D835E83, 0x00005383,
};

u32 D_800813F0 = 0x0000828F;

u32 D_800813F4 = 0x0000F18F;

u32 D_800813F8 = 0x0000958C;

u32 D_800813FC[2] = {
    0xDC91BB8D, 0x00000020,
};

u32 D_80081404[2] = {
    0x5B818B83, 0x00207983,
};

u32 D_8008140C[2] = {
    0xBE8BE18A, 0x00000000,
};

u32 D_80081414 = 0x0020E997;

u32 D_80081418[2] = {
    0xBB8F8590, 0x00000000,
};

u32 D_80081420[2] = {
    0xAB82AA8A, 0x0000A895,
};

u32 D_80081428 = 0x0000CA8B;

u32 D_8008142C = 0x0000ED8E;

u32 D_80081430 = 0x0000C08E;

u32 D_80081434 = 0x00009091;
