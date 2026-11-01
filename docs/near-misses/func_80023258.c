#include "common.h"

/* NEAR MISS: 94 words, 70 DIFF, and nearly all of it is one register swap -- the target holds p in $t1 and the loop counter in $t2, every spelling here has them the other way round, and that shifts the register names through the whole body. Words 1-11 and 15-20 are otherwise instruction-for-instruction right, which took three levers: p[33] * 16 must be spelled '<< 4' (as '* 16' gcc reassociates the constant onto the (p[34] - p[35]) difference instead, 93w/85); p[35] needs a LOCAL, or gcc reloads it narrowed to 'lhu' for the 16-bit store instead of reusing the word it already loaded; and the division plus 0xA needs its own local, or fold moves the 0xA onto the (p[35] << 4) term. The remaining word is the store address: the target computes 'addiu $t0, $a3, 0x7C' off the stride-1 induction variable and stores through it, where the plain '((u8 *)p)[0x7C + i] = v' gives a one-word 'sb $v0, 124($a2)' (93w/49, the best DIFF measured) and '&((u8 *)p)[0x7C + i]' gives two words but computes 'i + 124' before adding p. Also measured: four groupings of the head expression, three store-pointer spellings, three declaration orders, and a copy of the parameter into a local (68 DIFF, the only thing that moved the swap at all).

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

typedef struct
{
    s32 unk_00;
    UnkCardPrim *unk_04;
    UnkCardPrim12 *unk_08;
    s32 unk_0C;
} UnkCardPair;

void func_80023258(s32 *p)
{
    s32 w = p[35];
    s32 d = ((p[34] - w) * (p[33] << 4)) / p[32] + 0xA;
    s32 i;

    ((UnkCardPair *)p[105])->unk_04->unk_0A = (w << 4) + d;
    for (i = 0; i < 3; i++)
    {
        s32 v = ((u8 *)p)[0x7C + i];
        u8 *q = (u8 *)p + 0x7C + i;

        if (i == p[34])
        {
            v += (8 - v) / (p[32] - p[33] + 1);
        }
        else if (v != 0)
        {
            v -= 1;
        }
        *q = v;
        ((UnkCardPair *)p[114 + i])->unk_04->unk_08 = ((u8 *)p)[0x7C + i] + 0x24;
        ((UnkCardPair *)p[118 + i])->unk_04->unk_08 = ((u8 *)p)[0x7C + i] + 0x8F;
        ((UnkCardPair *)p[106 + i])->unk_04->unk_08 = ((u8 *)p)[0x7C + i] + 9;
        ((UnkCardPair *)p[110 + i])->unk_04->unk_08 = ((u8 *)p)[0x7C + i] + 0x42;
        ((u8 *)((UnkCardPair *)p[114 + i])->unk_04)[0] = ((u8 *)p)[0x7C + i] * 3 + 0x68;
        ((u8 *)((UnkCardPair *)p[114 + i])->unk_04)[1] = ((u8 *)p)[0x7C + i] * 3 + 0x68;
        ((u8 *)((UnkCardPair *)p[114 + i])->unk_04)[2] = ((u8 *)p)[0x7C + i] * 4 + 0x60;
    }
    if (p[33] < p[32])
    {
        p[33]++;
    }
}
