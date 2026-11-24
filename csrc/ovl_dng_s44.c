#include "common.h"

typedef struct UnkDng264D4Vec
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 pad_06[0x2];
} UnkDng264D4Vec;
typedef struct UnkDng264D4SP
{
    u8 pad_00[0x30];
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    u8 pad_3C[0x28];
    s32 unk_64;
    s32 unk_68;
    s32 unk_6C;
    SVECTOR unk_70[4];
    u8 pad_90[0x8];
    UnkDng264D4Vec unk_98[4];
} UnkDng264D4SP;
extern u8 D_800E3D20;
extern u8 D_80025B1C;
void func_s44_80024024(u8 *p);

/* The camera step the installer below parks in D_80083178's +0xB4 slot.
   `p' is the record it fills, D_80083178 + 0xB8. Ease the engine's +0x98
   bearing towards the request's over the frames left at +0x24, run that
   counter down with a -0x80 floor, spend one frame of D_800E3D20, copy the
   mover's (x, z, bearing) into the record's +0x4/+0x6/+0x8 and ease the three
   +0xA4/+0xA6/+0xA8 axes a quarter of the way towards them -- the third only
   while this bank's D_80025B1C is set, which is then re-armed from whether
   +0xA8 has caught up -- then halve the two +0x94/+0x96 rates.
   82 words, 0 differing, FIRST COMPILE.
   NO CARVED SIBLING AND NO RESIDENT TWIN BY EITHER TOOL: dngtwin's only hit
   is the 1.000 clone func_s44_80024024, which is still an INCLUDE_ASM. The
   body came from the DATA SYMBOL: `grep -n D_80083178 csrc/*.c' names
   csrc/ovl_dungeon_800C2824.c's landed func_800C75D0, whose own header
   already says it is the step installed at +0xB4 and that `p' is
   D_80083178 + 0xB8. That is a 128-word body against this 82, a size ratio
   no shape matcher can see past. The 46 missing words are MISSING STATEMENTS:
   the +0x34 pending-request retirement, the `else D_800E3D20 = 0' on the
   first gate, and the func_800BCB04 call with its 0x200 clamp that computes
   the bearing instead of copying +0xA.
   Of func_800C75D0's three stated facts:
     - HOLDS, far more cheaply: every read of +0x0C is RE-DEREFERENCED, never
       cached in a local. A cached `m = *(u8 **)(p + 0xC);' is 82 words / 3
       aligned differences here, where the twin priced the same change at 124
       words against 128 with 109 differing. Without its +0x34 block there
       are only two `lw 0xC' in the target, not four, so three rows is the
       whole fact.
     - HOLDS and reproduces exactly: the two trailing halvings read `lhu' and
       sign-extend at the shift (`sll 16'/`sra 18'), so `*(s16 *)' and
       `(s16)*(u16 *)' are byte-identical -- both 82 words / 0, because the
       `sh' truncates either way.
     - HOLDS: D_80083178 is reached through a `u8 *' LOCAL. Inline as
       `*(s16 *)((u8 *)&D_80083178 + 0x98)' at each of the seven sites it is
       82 words / 38 aligned differences.
   Measured and FREE: the -0x80 clamp read back out of memory rather than off
   an `s16' local (82 / 0 either way), and `D_800E3D20--' for
   `D_800E3D20 -= 1'. */
void func_s44_80024024(u8 *p)
{
    u8 *g;

    g = (u8 *)&D_80083178;
    if (*(s16 *)(p + 0x24) > 0)
    {
        *(s16 *)(g + 0x98) += (*(s16 *)(p + 0x26) - *(s16 *)(g + 0x98)) / *(s16 *)(p + 0x24);
    }
    *(s16 *)(p + 0x24) = *(u16 *)(p + 0x24) - 1;
    if (*(s16 *)(p + 0x24) < -0x80)
    {
        *(s16 *)(p + 0x24) = -0x80;
    }
    if (D_800E3D20 != 0)
    {
        D_800E3D20 -= 1;
    }
    *(s16 *)(p + 0x4) = *(u16 *)(*(u8 **)(p + 0xC) + 0x2);
    *(s16 *)(p + 0x6) = *(u16 *)(*(u8 **)(p + 0xC) + 0x6);
    *(s16 *)(p + 0x8) = *(u16 *)(*(u8 **)(p + 0xC) + 0xA);
    *(s16 *)(g + 0xA4) += (*(s16 *)(p + 0x4) - *(s16 *)(g + 0xA4)) >> 2;
    *(s16 *)(g + 0xA6) += (*(s16 *)(p + 0x6) - *(s16 *)(g + 0xA6)) >> 2;
    if (D_80025B1C != 0)
    {
        *(s16 *)(g + 0xA8) += (*(s16 *)(p + 0x8) - *(s16 *)(g + 0xA8)) >> 2;
    }
    D_80025B1C = *(s16 *)(g + 0xA8) != *(s16 *)(p + 0x8);
    *(s16 *)(g + 0x94) = *(s16 *)(g + 0x94) >> 2;
    *(s16 *)(g + 0x96) = *(s16 *)(g + 0x96) >> 2;
}

/* Install func_s44_80024024 as D_80083178's +0xB4 camera step and fill its
   request record at +0xB8: point the record's own +0x0 at +0x4, park the
   caller's mover at +0xC, clear +0x28 and +0x18, set the mode word at +0x14
   to 0xD, store the frame count at +0x26 and the bearing at +0x24, and
   return a two-bit result -- bit 0 when the mover CHANGED, bit 1 when the
   mode was not already 9 -- publishing the bearing into D_800E3D20 when bit
   0 is set. The first parameter is UNUSED here.
   30 words, 0 differing, FIRST COMPILE.
   NO CARVED SIBLING AND NO RESIDENT TWIN BY EITHER TOOL: dngtwin's only hit
   is the 1.000 clone func_s44_8002416C, still an INCLUDE_ASM. Found the same
   way as the step above -- off D_80083178 -- in the TAIL of
   csrc/ovl_dungeon_800C2824.c's landed func_800C77D0 (52 words), whose first
   30 words are a pair of abs range tests this copy does not have. The other
   deltas: 0xD at +0x14 where the resident writes 9 (the TEST is `!= 9' in
   both), and no `*(u32 *)(p + 0x34) = a' store, which is why the first
   parameter is dead and why the step above has no +0x34 block.
   Of func_800C77D0's facts:
     - HOLDS: TWO locals, `s' for the base and `q = s + 0xB8' for the record.
       Folding `s' away and spelling the three base-relative sites off `q'
       is 29 words / 25 aligned differences.
     - HOLDS as to the THIRD parameter, INVERTED as to the claim that the
       callee cannot see it. It says the third is `s16' and the fourth `s32'
       but that both are stored with `sh' so the callee is 0 differing either
       way. The fourth is indeed FREE here (30 words / 0 as `s16' or `s32'),
       but the third is NOT: as `s32 c' it is 29 words / 6 aligned
       differences, because `D_800E3D20 = c' adds an `sb' use and the
       parameter copy into $t1 goes with it.
   This copy's own measured facts: the +0x24 store must be the LAST statement,
   after the D_800E3D20 gate -- moved ahead of it, 29 words / 2 aligned
   differences -- and the 0xD store must follow the `!= 9' test, 26 words / 28
   the other way. 
   THIS COPY'S OWN CONSTANT: 0xd -> 0xc.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
s32 func_s44_8002416C(void *a, u8 *b, s16 c, s32 d)
{
    u8 *s;
    u8 *q;
    s32 r;

    s = (u8 *)&D_80083178;
    q = s + 0xB8;
    r = (*(u32 *)(q + 0xC) != (u32)b);
    *(u32 *)(q + 0xC) = (u32)b;
    *(u32 *)(q + 0x28) = 0;
    *(u32 *)(s + 0xB8) = (u32)(s + 0xBC);
    if (*(u32 *)(q + 0x14) != 9)
    {
        r |= 2;
    }
    *(u32 *)(q + 0x14) = 0xc;
    *(u32 *)(q + 0x18) = 0;
    *(s16 *)(q + 0x26) = d;
    *(u32 *)(s + 0xB4) = (u32)func_s44_80024024;
    if (r & 1)
    {
        D_800E3D20 = c;
    }
    *(s16 *)(q + 0x24) = c;
    return r;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s44", func_s44_800241E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s44", func_s44_800242EC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s44", func_s44_8002441C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s44", func_s44_8002452C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s44", func_s44_8002457C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s44", func_s44_80024600);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s44", func_s44_80024710);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s44", func_s44_80024804);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s44", func_s44_80024A34);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s44", func_s44_80024C0C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s44", func_s44_80024E80);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s44", func_s44_80025094);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s44", func_s44_80025270);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s44", func_s44_80025408);
