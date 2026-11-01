#include "common.h"

/* 14 words, 6 DIFF, all register names: target has the cursor in $a1 and the
   counter in $v1, this has them swapped. See csrc/ovl_main_80020000.c for the
   seven spellings measured and why the indexed loop must stay. */

s32 func_80021EE0(s32 *gsw)
{
    s32 sum = 0;
    s32 *q = gsw + 0x82;
    s32 i;

    for (i = 0; i < 0x177E; i++)
    {
        sum ^= q[i];
    }
    return gsw[0x81] == sum;
}
