#include "common.h"

typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
typedef struct UnkDng708A4SP
{
    u8 pad_00[0x4];
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    u8 pad_0A[0x12];
    u8 *unk_1C;
    u8 pad_20[0x4];
    u8 *unk_24;
    u8 pad_28[0xA8];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDng708A4SP;
typedef struct UnkDngB09A28
{
    s16 unk_00;
    s16 unk_02;
} UnkDngB09A28;
typedef struct UnkDngB09A28Tbl
{
    UnkDngB09A28 unk_00[8];
} UnkDngB09A28Tbl;
void func_800478B8(u8 *);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80024048);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80024130);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80024224);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_800242A0);

/* 27 words, 0 differing. */
/* The 0x312 spark's own step function: hand the renderable to func_800478B8,
   and when its +0x14 reports 0x6000 mark the owning actor retired and set the
   same bit in D_800814A0. No xsibs twin at ratio 0.40; written from the
   listing. D_800814A0 is the PLAIN scalar here, not ovl_main_8008A288's
   `D_800814A0_a[0]` array alias -- the `lui`/`lw %lo` pair is what the
   non-array spelling emits. 27 words, 0 differing, first compile. */
void func_s10_80024374(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_800243E0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_800244A4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80024550);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_800246AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80024790);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80024930);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80024CFC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80024E44);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80024EF8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80025020);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80025090);
