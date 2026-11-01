#include "common.h"

/* Retyped 2026-11-01 to UnkStrSet (see csrc/ovl_main_80020000.c): the record
   really is 24 bytes of five string pointers plus four bytes, which
   func_80022CD8 confirmed by needing the 0x15 field as a displacement. The
   struct typing does NOT fix this one -- still 27 words against 24, same
   duplicated inner-loop test.

   27 words against 24, and the register allocation ALREADY matches ($a3 maxlen, $t1 last, $a2 index, $t0 cursor). The three extra words are a duplicated inner-loop test: the target enters the inner loop with 'j' straight to the bottom test, the plain gcc 2.7.2 'while' shape, while every spelling here gets a guard copy of the test at the top AND the bottom one. Related, and probably the cause: 'last = i' is loop-invariant in the inner loop, and loop.c SINKS it out past the loop here (it lands in the bottom branch's delay slot, shared with the guard's target) where the target keeps it inside, in the if's delay slot. Three spellings measured: 'last = i' before the if, after it, and the if-body order swapped.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

void func_80022BE8(UnkStrSet *q)
{
    s32 maxlen = 0;
    s32 last = 0;
    s32 i;

    for (i = 0; i < 5; i++)
    {
        u8 *s = q->strs[i];
        s32 len = 0;

        while (*s != 0)
        {
            s++;
            len++;
            if (maxlen < len)
            {
                maxlen = len;
            }
            last = i;
        }
    }
    base[0x16] = maxlen;
    base[0x17] = last + 1;
}
