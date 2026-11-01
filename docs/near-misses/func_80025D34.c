#include "common.h"

/* 36 words against 37; see the comment in csrc/ovl_main_80020000.c for the
   measurement table. The missing word is a delay-slot nop after the first
   loop call. */

void func_8004F52C(s32 a);
void func_80023A00(s32 *p);
void func_80024EEC(s32 *p);
void func_80024184(s32 *p);

void func_80025D34(s32 *p)
{
    s32 *q = p + 8;
    s32 i;

    if (p != 0)
    {
        func_8004F52C(q[0]);
        i = 0;
        do
        {
            func_80023A00((s32 *)q[6]);
            func_80024EEC((s32 *)q[7]);
            func_80024184((s32 *)q[1]);
            q++;
        } while (++i < 5);
        ((u16 *)p)[15] |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}
