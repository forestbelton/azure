#include "common.h"

/* 23 words, 2 DIFF, register names only: the target holds the tested p[0] in $a1 and reuses it for the call's second argument, this holds it in $v0 and moves zero into $a1. Two spellings measured (the if reversed, a local temp), 2 and 4 DIFF.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

void func_80027528(s32 *p)
{
    s32 v;

    func_80027C90(p - 8);
    v = p[0];
    if (v == 0)
    {
        func_80025C7C((s32 *)p[5], 0);
    }
    else
    {
        func_80026868((s32 *)p[5]);
    }
}
