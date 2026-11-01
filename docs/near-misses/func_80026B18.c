#include "common.h"

/* 56 words against 58, 50 DIFF: the target carries TWO extra induction variables -- a byte cursor advancing 0xC from p and a separate 0xC-stride offset starting at 4 -- where the 'i * 0xC + 4' and 'i * 0xC + 8' forms here get folded into one. Two measured originally: the folded form (56w/51) and explicit cursors advanced at the bottom of the loop (62w/53, worse).

   UPDATED 2026-11-01: func_8004CBFC takes THREE arguments
   (csrc/unk_1C540.c:2791), and the third is the UnkCardPair this function
   writes through -- the target loads it into $a2 at word 23, BEFORE the b[i]
   test, uses $a2 for the else arm's `q->unk_00 = 0`, and leaves it there for
   the call. That is now folded in and takes the park from 51 DIFF to 50. The
   same discovery landed func_80026CF0 outright and func_800269B4 before it.

   Four more cursor shapes measured with the third argument in place: an
   integer `off` plus a `u8 *cur` (60w/50), either one alone (56w/50), and the
   cursor declared before `any` (60w/55). The target really does carry FIVE
   induction variables -- c at stride 4, b at stride 4, p at stride 4 for the
   `[0x1A + i]` load, an integer offset from 4 at stride 0xC for the call, and
   a byte cursor from p at stride 0xC used with displacement 8 -- and the last
   two differ by a constant, so gcc folds them into one giv whichever way they
   are spelled here. One for the permuter. The shape is right: two iterations, each testing c[i] then b[i], calling func_8004CBFC(p + 0xC*i + 4, D_80027E68) on the true side and clearing both the UnkCardPair at p[0x1A + i] and the word at p + 0xC*i + 8 on the false one, with 'any = 1' set unconditionally once c[i] is non-zero and func_800269B4 called at the end if it was.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

/* Declarations this park needs; they live in csrc/ovl_main_80020000.c.
   Repeated here so the park compiles STANDALONE, which is what
   tools/decomp/permimport.py needs -- without them a permrun reports a
   silent `base=?` and searches nothing. */
extern u8 D_80027E68[];

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

void func_8004CBFC(u32 *p, u8 *q, u32 *r);
void func_800269B4(s32 *p);

void func_80026B18(u8 *p, s32 *b, s32 *c)
{
    s32 any = 0;
    s32 off = 4;
    s32 i;

    for (i = 0; i < 2; i++)
    {
        if (c[i] != 0)
        {
            UnkCardPair *q = (UnkCardPair *)((s32 *)p)[0x1A + i];

            any = 1;
            if (b[i] != 0)
            {
                func_8004CBFC((u32 *)(p + off), D_80027E68, (u32 *)q);
            }
            else
            {
                q->unk_00 = 0;
                *(s32 *)(p + off + 4) = 0;
            }
        }
        off += 0xC;
    }
    if (any != 0)
    {
        func_800269B4((s32 *)p);
    }
}
