#include "common.h"

/* 18 words against 19: the target copies the materialised base into the induction variable ('addu $v1, $v0, $zero') and seeds the sum from the counter's zero ('addu $a1, $a2, $zero'); every spelling folds one of the two away. Four measured (indexed against pointer-walk, counter declared first or not, guarded do/while), all 18 words.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

/* Declarations this park needs; they live in csrc/ovl_main_80020000.c.
   Repeated here so the park compiles STANDALONE, which is what
   tools/decomp/permimport.py needs -- without them a permrun reports a
   silent `base=?` and searches nothing. */
extern s32 D_80028570[][10];

s32 func_800219B8(s32 n)
{
    s32 sum = 0;
    s32 i;

    for (i = 0; i < n; i++)
    {
        sum += D_80028570[i][6];
    }
    return 0xF - sum / 0x2000;
}
