#include "common.h"

/* 58 words against 57, 23 DIFF. The dispatch is the problem: the target tests st==1 and st==3 with BEQ to blocks placed AFTER the chain, sets r=5 in the second beq's delay slot and jumps to the exit. Four structures measured -- plain if/else-if chain (56w/44, one word short, gcc merges the r=5 path into the exit), st!=1 first (56w/44, the FIRST test then matches), fully nested != (56w/44), a switch (58w/23), and this goto chain (58w/23). The bodies are right: state 1 re-registers the events then checks func_80069C38 and func_80069C18's bit 4 before advancing D_800287C8, state 3 clears one of the two D_80084118 words chosen by D_800287CC and returns 3, anything else returns 5.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

/* Declarations this park needs; they live in csrc/ovl_main_80020000.c.
   Repeated here so the park compiles STANDALONE, which is what
   tools/decomp/permimport.py needs -- without them a permrun reports a
   silent `base=?` and searches nothing. */
extern s32 D_800287C8;
extern s32 D_800287CC;
extern s32 D_800287E0;
extern s32 D_80084118;

s32 func_80021298(void)
{
    s32 st = D_800287C8;
    s32 r = 0;
    s32 *t;

    if (st == 0)
    {
        goto out;
    }
    if (st == 1)
    {
        goto one;
    }
    if (st == 3)
    {
        goto three;
    }
    r = 5;
    goto out;
one:
    func_80021538();
    if (func_80069C38(D_800287CC) != 0 && (func_80069C18(D_800287CC != 0) & 4) != 0)
    {
        D_800287E0 = st;
        D_800287C8 = D_800287C8 + 1;
    }
    else
    {
        r = 5;
        D_800287C8 = 0;
    }
    goto out;
three:
    t = &D_80084118;
    if (D_800287CC != 0)
    {
        t++;
    }
    r = 3;
    t[0] = 0;
    D_800287C8 = 0;
out:
    return r;
}
