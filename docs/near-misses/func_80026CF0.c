#include "common.h"

/* 34 words, 11 DIFF: $s0 and $s1 are swapped -- the target keeps the parameter in $s1 and the D_80027E68 base in $s0, every spelling here does the reverse. Three measured (the table inline at both call sites, hoisted into a local declared before or after the UnkCardPair temp, and the parameter copied to a local). Allocation only; the instruction sequence is identical.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

void func_80026CF0(s32 *p)
{
    UnkCardPair *q;

    q = (UnkCardPair *)p[0x1A];
    q->unk_04->unk_08 = 0x40;
    q->unk_04->unk_0A = 0x20;
    func_8004CBFC(p + 1, D_80027E68);
    q = (UnkCardPair *)p[0x1B];
    q->unk_04->unk_08 = 0xC0;
    q->unk_04->unk_0A = 0x20;
    func_8004CBFC(p + 4, D_80027E68);
}
