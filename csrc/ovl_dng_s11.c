#include "common.h"

s32 rand(void);
void func_800478B8(u8 *);
extern u8 D_80045340[];
extern u32 D_800814A0;

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s11", func_s11_80024004);

/* The 0x312 node step installed by func_s11_80024004 (still an INCLUDE_ASM
   here, parked at docs/dng-parks/func_s11_80024004.c): mark the owning node's
   +0x52 with 0x8000, then run a two-state machine on the node's own +0x4C.
   State 0 counts +0x48 down and, at zero, cues func_8004491C with D_80045340
   and advances. State 1 hands the renderable to func_800478B8, clears its two
   low bytes once its +0x14 flags show 0x6000, advances the position by the
   velocity, gravity-steps the z velocity by 0x8000 + a 12-bit random, and once
   the +0x4E timer expires either fades the colour triple down by +0x4A or
   retires the actor and sets 0x8000 in D_800814A0.
   103 words, 0 differing, FIRST COMPILE. Donor: func_s26_800240D8
   (csrc/ovl_dng_s26.c), named by dngcallee.py at 5 of 5 symbols with the same
   -0x20 frame and the same s1/ra/s2/s0 save order; its switch skeleton, its
   *(u16 *)(*(u8 **)p + 0x52) |= 0x8000 mark, its descending unk_0C[2]/[1]/[0]
   fade and its D_800814A0 teardown are all verbatim. dngtwin found NOTHING
   for this stub at ratio 0.70.

   PRICED AFTER THE LANDING, from this body at 0 differing, with the file
   restored and diffed back to byte-identical after each edit. Figures are
   ovlverify masked differences and aligndiff aligned rows; they agreed on
   every row, and the aligndiff totals include register-only R rows.

   LOAD-BEARING:
   * the +0x4A compare must read a SIGNED HALFWORD and the subtraction a
     PLAIN BYTE -- two widths at one address. Spelling the compare p[0x4A]
     is 102 words (one SHORT) and 16 aligned rows: the target has lh for the
     slt and lbu for the subu, and the byte form lets combine prove the
     operand non-negative and drop a word.
   * the arms of that gate must be >= with the fade first, not < with the
     retire first: 17 aligned rows inverted, every row the moved block and
     the flipped branch.
   * the three colour bytes must be written +0xE, +0xD, +0xC (descending):
     ascending is 2 aligned rows.

   MEASURED AND FREE, so NOT the reason this lands:
   * 0x8000 + (rand() & 0xFFF) against (rand() & 0xFFF) + 0x8000 -- 0 either
     way; fold canonicalises the constant and the ori is forced because
     0x8000 is outside addiu's range.
   * the subtrahend spelled *(s16 *)(p + 0x4A) instead of p[0x4A] -- 0 either
     way, because the u8 destination narrows it back to lbu. This is the same
     character that costs 16 rows in the COMPARE, so the width is a fact
     about the comparison only.
   * g declared s32 instead of u8 -- 0.
   * the +0x4E guard written < 1 instead of <= 0 -- 0. */
void func_s11_800247DC(u8 *p, UnkVec *b, u8 *c)
{
    u8 g;

    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) - 1;
        if (*(s16 *)(p + 0x48) <= 0)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 1:
        func_800478B8(c);
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(u8 *)(c + 0x4) = 0;
            *(u8 *)(c + 0x5) = 0;
        }
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        if (b->unk_14 != 0)
        {
            b->unk_08 += b->unk_14;
            b->unk_14 += 0x8000 + (rand() & 0xFFF);
        }
        *(s16 *)(p + 0x4E) = *(u16 *)(p + 0x4E) - 1;
        if (*(s16 *)(p + 0x4E) <= 0)
        {
            if (*(u8 *)(c + 0xC) >= *(s16 *)(p + 0x4A))
            {
                g = *(u8 *)(c + 0xE) - p[0x4A];
                *(u8 *)(c + 0xE) = g;
                *(u8 *)(c + 0xD) = g;
                *(u8 *)(c + 0xC) = g;
            }
            else
            {
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
        }
        break;
    }
}
