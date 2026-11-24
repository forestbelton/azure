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
s32 func_80065F90(s32, s32);
s32 func_80064710(s32);

/* Turn a delta between two points into a two-angle bearing at out[0..1]
   with out[2] zeroed: out[0] is minus func_80065F90 (ratan2) of the y and z
   deltas, out[1] is func_80065F90 of the x delta against func_80064710's
   (square root) hypotenuse of the other two, and each divisor is guarded to
   1 when it comes out zero. 53 words, 0 differing.

   No twin to read: dngtwin's only hit is func_s53_8002401C at 1.000, which
   is this function's CLONE and still an INCLUDE_ASM. Written from the
   listing; func_80065F90's and func_80064710's prototypes are
   csrc/ovl_dungeon_8009F644.c's and csrc/ovl_dungeon_800D50FC.c's.

   THE ONE FACT, and it is a storage fact rather than a shape one: the three
   deltas live in a `s32 d[3]' STACK ARRAY, not in three locals. The target
   saves only $s0 and $ra and spills the deltas to sp+0x10/0x14/0x18, which
   is an array; as three `s32' locals they land in $s1/$s2/$s3, the frame
   grows to 0x38 and it is 51 words / 49 aligned differences. With the array
   in place the PARAMETER STYLE IS FREE: prototyped `s16' parameters and a
   K&R definition with `s16' declarators both measure 53 words / 0 differing.
   The lw-plus-sll/sra pairs on the two stack arguments are not evidence of
   the K&R form -- declared `s32' with `(s16)' casts instead, combine
   narrows them to `lh' and the body is 49 words / 17. */
void func_s53_8002401C(s16 x0, s16 y0, s16 z0, s16 x1, s16 y1, s16 z1, s16 *out)
{
    s32 d[3];
    s32 h;

    d[0] = x1 - x0;
    d[1] = y1 - y0;
    d[2] = z1 - z0;
    out[0] = -func_80065F90(d[1], (d[2] != 0) ? d[2] : 1);
    h = func_80064710(d[1] * d[1] + d[2] * d[2]);
    out[1] = func_80065F90(d[0], (h != 0) ? h : 1);
    out[2] = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s53", func_s53_800240F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s53", func_s53_80024494);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s53", func_s53_8002455C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s53", func_s53_80024700);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s53", func_s53_80024918);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s53", func_s53_800249E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s53", func_s53_80024B54);
