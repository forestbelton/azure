#include "common.h"

void func_800478B8(u8 *);
s32 func_8004491C(UnkNode *, u32);
void func_s09_80024C84(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u8 D_80045340[];

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s09", func_s09_80024020);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s09", func_s09_8002472C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s09", func_s09_80024868);

/* The 0x312 smoke node's per-frame step, installed as the +0x10 step by
   func_s12_80024020's spawn loops and by its case 4, so the four-argument
   signature is fixed by those call sites. Sets bit 0x8000 in the +0x52
   halfword of the record at the actor's +0x0, decays the caller's +0x14
   velocity component by 0x20000 unless it has already reached zero, advances
   the vector by its own velocity half, then runs a two-case compare chain on
   +0x4C: state 0 counts the +0x48 timer down and re-registers the renderable
   through func_8004491C when it expires, state 1 hands the renderable to
   func_800478B8, clears its +0x4/+0x5 once +0x14 reports 0x6000, and fades
   its +0xC/+0xD/+0xE colour bytes by the +0x4A step until the red byte would
   pass the actor's own level, at which point it clears the colour word and
   retires. 91 words, 0 differing. 2-member family with
   func_s09_80024C84 (dngfam); I did not touch the sibling.

   NO TWIN AT ANY RATIO. Landed off the IN-UNIT route plus one donor row:
   every idiom but the fade came from func_s12_80024F40 below (the +0x52 flag,
   the retire pair, the two-case compare chain), and the
   `b->unk_00 += b->unk_0C' triple from csrc/ovl_dng_s23.c
   func_s23_80024024, which the callee-set overlap on
   D_80045340 + D_800814A0 + func_8004491C + func_800478B8 named.

   FOUR FACTS, all priced from this body:

   * THE FADE/RETIRE GATE'S ARMS WERE THE WRONG WAY ROUND and that was 17 of
     the 19 aligned differences this body started at -- one character, and the
     residue was the whole subu block emitted in the wrong position with an
     inverted branch, exactly as the brief describes.
   * THE COMPARE PUTS THE STRUCT FIELD ON THE LEFT:
     `*(u8 *)(c + 0xC) <= *(s16 *)(p + 0x4A)' is 0 and
     `*(s16 *)(p + 0x4A) >= *(u8 *)(c + 0xC)' is 2 aligned differences, a pure
     transposition of the two loads, because operand 0 of a comparison is
     expanded first.
   * THE +0x14 DECAY NEEDS A LOCAL: read into `v', test, decay, store back
     unconditionally. Written in place as
     `if (b->unk_14 != 0) { b->unk_14 -= 0x20000; }' it is 5 aligned
     differences, because the target's store at the join is unconditional.
   * THE COLOUR FADE IS ONE CHAINED ASSIGNMENT, not three statements -- three
     statements are 92 words / 3 aligned differences. Chained, the stores come
     out +0xE, +0xD, +0xC, which is a chained assignment's right-to-left
     order. */
void func_s09_80024C84(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 v;

    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    v = b->unk_14;
    if (v != 0)
    {
        v -= 0x20000;
    }
    b->unk_14 = v;
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
        if (*(u8 *)(c + 0xC) <= *(s16 *)(p + 0x4A))
        {
            *(s32 *)(c + 0xC) = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            *(u8 *)(c + 0xC) = *(u8 *)(c + 0xD) = *(u8 *)(c + 0xE) -= *(u8 *)(p + 0x4A);
        }
        break;
    }
}
