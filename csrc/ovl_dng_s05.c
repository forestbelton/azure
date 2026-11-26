#include "common.h"

s32 rand(void);
void func_800478B8(u8 *);
s32 func_8004491C(UnkNode *, u32);
extern u8 D_80045340[];

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s05", func_s05_8002401C);

/* The smoke node's per-frame step, installed as the +0x10 step by
   func_s05_8002401C's case-1 spawn loop (still an INCLUDE_ASM, parked at
   docs/dng-parks/func_s05_8002401C.c), so the four-argument signature comes
   from that call site and the fourth argument is unused here. Raises bit
   0x8000 in the WORD at +0xC of the record at the actor's +0x0, advances the
   caller's vector by its own velocity half, then runs a two-case compare
   chain on +0x4C: state 0 counts the +0x48 timer down and re-registers the
   renderable through func_8004491C when it expires, state 1 hands the
   renderable to func_800478B8, clears its +0x4/+0x5 once +0x14 reports
   0x6000, and fades its +0xC/+0xD/+0xE colour bytes by a fresh
   (rand() & 0xF) + 9 until the red byte would pass that step, at which point
   it clears the colour word and retires. 85 words, 0 differing. 1-member
   family (dngfam). func_80069EF8 is rand out of splat/common_syms.txt.

   LANDED ON THE FIRST COMPILE off csrc/ovl_dng_s12.c func_s12_80024850
   (dngtwin 0.864, +6 words). The twin's shape carried whole: the two-case
   compare chain, the `*(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) - 1' idiom,
   the `<= 0' guard spelled through the stored halfword, the retire pair and
   the chained colour fade. Deltas from it: the flag is a word at +0xC where
   the twin raises a halfword at +0x52, there is no +0x14 velocity decay, and
   the fade step is a fresh random value rather than the actor's +0x4A byte,
   which is why it needs a local.

   TWO OF THE TWIN'S FACTS RE-PRICED ON THIS BODY, since a first-compile
   landing measures nothing by itself:

   * THE COLOUR FADE MUST BE ONE CHAINED ASSIGNMENT, and the twin's figure
     reproduces to the digit. Written as three statements
     (`c[0xE] -= k; c[0xD] = c[0xE]; c[0xC] = c[0xE];') it is 86 words / 3
     aligned differences, the extra word a `move v1,v0' the chained form does
     not need. Chained, the stores come out +0xE, +0xD, +0xC, which is a
     chained assignment's right-to-left order.
   * THE COMPARE'S OPERAND ORDER IS **NOT MEASURABLE** HERE, not free. The
     twin prices `*(u8 *)(c + 0xC) <= *(s16 *)(p + 0x4A)' against the
     reversed spelling at 2 aligned differences; on this relink
     `k >= *(u8 *)(c + 0xC)' measures 85 words / 0 differing, because fold
     canonicalises `>=' into `<=' with the operands swapped and the two
     spellings are literally the same tree. Do not record this as free. */
void func_s05_80024D44(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 k;

    *(s32 *)(*(u8 **)p + 0xC) |= 0x8000;
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
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
        k = (rand() & 0xF) + 9;
        if (*(u8 *)(c + 0xC) <= k)
        {
            *(s32 *)(c + 0xC) = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            *(u8 *)(c + 0xC) = *(u8 *)(c + 0xD) = *(u8 *)(c + 0xE) -= k;
        }
        break;
    }
}

/* The heavier variant of the step above, installed as the +0x10 step by
   func_s05_8002401C's case-2 spawn loop. Same +0xC flag, then it advances
   the caller's vector and damps the two horizontal velocity words by an
   eighth while pulling the vertical one down by 0x4000 plus a random
   multiple of four; the whole tail from the +0x4C compare chain down is the
   body above verbatim with the fade step at (rand() & 0xF) + 4 instead of
   + 9. 101 words, 0 differing. 1-member family (dngfam).

   Written off csrc/ovl_dng_s12.c func_s12_80024850 (dngtwin 0.760, 10 words
   shorter than this one) and off the body above, which landed first. It
   compiled at 101 words / 3 aligned differences first time, and ONE spelling
   closed it:

   THE VERTICAL PULL MUST BE A COMPOUND SUBTRACTION OF THE WHOLE TERM:
     b->unk_14 -= 0x4000 + (rand() << 2);              101 words / 0
     b->unk_14 = b->unk_14 - 0x4000 - (rand() << 2);   101 words / 3
   The three rows are `lw'/`sll'/`addiu' transposed and the `addiu -0x4000'
   landing on the random term instead of on the loaded word. fold's
   `split_tree' (scratch/gcc-src/cdk-gcc-b18/fold-const.c:991, caller at
   4201) is the mechanism and it fires on BOTH spellings -- the difference is
   which side it splits. For `(a - C) - r' it splits arg0, finds the constant
   second under a MINUS, negates it and rebuilds `var - (arg1 - con)', i.e.
   `a - (r + C)'. For `a - (C + r)' it splits arg1 instead, finds the
   constant FIRST under a PLUS, and rebuilds `fold(arg0 - con) - var', i.e.
   `(a - C) - r' -- the form the target emits. So the compound operator is
   not cosmetic here: it is what puts the constant on the splittable side.

   NOT RE-PRICED on this body: the chained colour fade and the compare's
   operand order. Both are the body above's measurements and the tail is
   byte-identical between the two, so they carry by construction rather than
   by evidence. */
void func_s05_80024E98(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 k;

    *(s32 *)(*(u8 **)p + 0xC) |= 0x8000;
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    b->unk_0C -= b->unk_0C >> 3;
    b->unk_10 -= b->unk_10 >> 3;
    b->unk_14 -= 0x4000 + (rand() << 2);
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
        k = (rand() & 0xF) + 4;
        if (*(u8 *)(c + 0xC) <= k)
        {
            *(s32 *)(c + 0xC) = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            *(u8 *)(c + 0xC) = *(u8 *)(c + 0xD) = *(u8 *)(c + 0xE) -= k;
        }
        break;
    }
}
