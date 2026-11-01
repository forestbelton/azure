#include "common.h"

/* NEAR MISS: 75 words, 2 DIFF -- everything matches but the ORDER of the two stores that end the st==3 block. The target has 'u[0] = 1' then 'D_800287C8 = 0'; in that order gcc cross-jumps the trailing 'D_800287C8 = 0' with the identical store that ends the st==1 block, which drops the function to 74 words, and the only orders that keep 75 put one of the two pairs the wrong way round. Four things ARE load-bearing and already right: the dispatch has to be a GOTO CHAIN with the bodies placed after it (a switch on any case grouping roots the tree at 1 instead of testing 0 and 2 by equality first, and the inner split must be spelled '(u32)st >= 3' to get the sltiu); the D_80084118 selection is a pointer advanced by the condition, not '(&D_80084118)[cond]'; the two blocks need SEPARATE pointer locals, because with one shared local the st==1 block's lo_sum lands in a different register from its 'lui' (3 DIFF) where the target ties them; and 'st' must be read into a local before 'r' so the %hi(D_800287C8) stays live in $a1 for both stores. Measured: switch with five case groupings (75w/63 each), two-stage early-goto plus switch (70w/66), explicit unsigned split with inline bodies (73w/62), goto chain with shared local (74w/50), the three reorderings of each block's tail, and 'r = 2' moved to the end of the st==3 block (74w/47, gcc hoists it back above the stores).

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

s32 func_800210F0(void)
{
    s32 st = D_800287C8;
    s32 r = 0;
    s32 *t;
    s32 *u;

    if (st == 0)
    {
        goto out;
    }
    if (st == 2)
    {
        goto out;
    }
    if ((u32)st >= 3)
    {
        goto high;
    }
    if (st == 1)
    {
        goto one;
    }
    r = 5;
    goto out;
high:
    if (st == 3)
    {
        goto three;
    }
    r = 5;
    goto out;
one:
    t = &D_80084118;
    if (D_800287CC != 0)
    {
        t++;
    }
    if (t[0] != 0)
    {
        r = 1;
        D_800287C8 = 0;
    }
    else if ((func_80069C18(0) & 1) != 0 || (func_80069C18(1) & 1) != 0)
    {
        func_800214A4();
        while (func_80069C08(D_800287CC) == 0)
        {
        }
        D_800287E0 = 0;
        D_800287C8 = 3;
    }
    goto out;
three:
    u = &D_80084118;
    if (D_800287CC != 0)
    {
        u++;
    }
    r = 2;
    D_800287C8 = 0;
    u[0] = 1;
out:
    return r;
}
