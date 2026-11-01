#include "common.h"

/* 18 words against 19: the target carries an extra 'addu $v1, $v0, $zero' copy before storing 1 into D_800287C8, with the constant materialised back in the loop's branch delay slot. Three spellings measured (a local for the 1, 'while' against 'do/while', the two stores swapped), all 18 words and 10-11 DIFF. Reload artifact; permuter.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

/* Declarations this park needs; they live in csrc/ovl_main_80020000.c.
   Repeated here so the park compiles STANDALONE, which is what
   tools/decomp/permimport.py needs -- without them a permrun reports a
   silent `base=?` and searches nothing. */
extern s32 D_800287C8;
extern s32 D_800287E0;

void func_800213C4(s32 a)
{
    func_800214A4();
    do
    {
    } while (func_80069BF8(a) == 0);
    D_800287E0 = 0;
    D_800287C8 = 1;
}
