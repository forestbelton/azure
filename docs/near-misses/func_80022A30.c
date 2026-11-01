#include "common.h"

/* 44 words against 46, and the two missing words are COPIES of hoisted constants: the target has 'addiu $t4, $zero, 5' then 'addu $t3, $t4, $zero', and the same pair for 6, where every spelling here CSEs the two 5s into one pseudo before loop.c sees them. Per docs/matching-levers.md:567, combine_movables matches two movables of equal value and mode and hoists one while the other becomes a copy -- so the target's two 5s survived cse1 as separate pseudos and this one's did not. The rest is right: an unaligned 4-byte copy of D_800200A0 to a stack buffer, then four iterations writing two 0xC-byte records each (byte 0 from the buffer, byte 1 = 0x48, word at 4 = 0x104058, bytes 0xA/0xB = 5 then 6), then 'p[-0x18] |= 0x80' and return p.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

/* Declarations this park needs; they live in csrc/ovl_main_80020000.c.
   Repeated here so the park compiles STANDALONE, which is what
   tools/decomp/permimport.py needs -- without them a permrun reports a
   silent `base=?` and searches nothing. */
typedef struct
{
    s32 unk_00;
    s32 unk_04;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkCardPrim;

typedef struct
{
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
} UnkCardPrim12;

/* Four bytes, alignment 1: func_80026128 copies this out of D_80028064 with
   lwl/lwr + swl/swr, which is what gcc emits for a struct it cannot assume is
   word-aligned. */
typedef struct
{
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
} UnkFour;
extern u8 D_800200A0[];

u8 *func_80022A30(u8 *p, s32 *q)
{
    u8 buf[4];
    s32 i;

    *(UnkFour *)buf = *(UnkFour *)D_800200A0;
    for (i = 0; i < 4; i++)
    {
        p[0] = buf[i];
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x104058;
        p[0xA] = 5;
        p[0xB] = 5;
        p += 0xC;
        p[0] = buf[i];
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x104058;
        p[0xA] = 6;
        p[0xB] = 6;
        p += 0xC;
    }
    p[-0x18] |= 0x80;
    return p;
}
