#include "common.h"

/* 56 words against 58: the target carries TWO extra induction variables -- a byte cursor advancing 0xC from p and a separate 0xC-stride offset starting at 4 -- where the 'i * 0xC + 4' and 'i * 0xC + 8' forms here get folded into one. Two measured: the folded form (56w/51) and explicit cursors advanced at the bottom of the loop (62w/53, worse). The shape is right: two iterations, each testing c[i] then b[i], calling func_8004CBFC(p + 0xC*i + 4, D_80027E68) on the true side and clearing both the UnkCardPair at p[0x1A + i] and the word at p + 0xC*i + 8 on the false one, with 'any = 1' set unconditionally once c[i] is non-zero and func_800269B4 called at the end if it was.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

void func_80026B18(u8 *p, s32 *b, s32 *c)
{
    s32 any = 0;
    s32 i;

    for (i = 0; i < 2; i++)
    {
        if (c[i] != 0)
        {
            any = 1;
            if (b[i] != 0)
            {
                func_8004CBFC((s32 *)(p + i * 0xC + 4), D_80027E68);
            }
            else
            {
                ((UnkCardPair *)((s32 *)p)[0x1A + i])->unk_00 = 0;
                *(s32 *)(p + i * 0xC + 8) = 0;
            }
        }
    }
    if (any != 0)
    {
        func_800269B4((s32 *)p);
    }
}
