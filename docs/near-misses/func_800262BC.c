#include "common.h"

/* 55 words, 3 DIFF: in the failure block the target computes the call's argument 'addiu $a0, $s0, -0x20' BEFORE finishing the func_800265B8 address and schedules the 'sw $v0, 0x34($s0)' into the call's delay slot; every spelling here emits the store before the argument. Four measured originally: the two branch senses (== 0 first is right -- it took this from 28 DIFF to 3), the argument hoisted into a local inside the block, and the store moved after the call (56w/45).

   RE-MEASURED 2026-11-01: all six permutations of the failure block's three
   statements, plus the argument in a local declared inside the block and at
   function scope. The source order it is parked with is the best by a wide
   margin -- store/call/h is 55w/3, and the next best is call/h/store at
   56w/44; anything that puts `h` before the call is 57 or 58 words. Both
   argument-local spellings are also 55w/3, so the residue is the pre-reload
   scheduler interleaving two independent single-set chains (the function
   address and the `p - 8` argument) and nothing the source distinguishes
   between them. Everything else matches, including the 0x80010000 constant base for both the D_80010208 halfword read and the two pointer arguments.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

/* Declarations this park needs; they live in csrc/ovl_main_80020000.c.
   Repeated here so the park compiles STANDALONE, which is what
   tools/decomp/permimport.py needs -- without them a permrun reports a
   silent `base=?` and searches nothing. */
void func_800261C0(s32 *p);
void func_80026240(s32 *p);
void func_800265B8(s32 *p);

void func_800262BC(s32 *p)
{
    s32 h;

    func_80026128((u8 *)0x80010000, p[11], p[9]);
    if (func_80021F18(p[11], (u8 *)0x80010000) == 0)
    {
        p[13] = (s32)func_800265B8;
        func_80022EEC((s32)(p - 8));
        h = (s32)func_800261C0;
    }
    else
    {
        func_80024F3C((s32 *)p[1], p[11], 3);
        func_80026270((s32 **)p);
        func_800241D4((s32 *)p[p[11] + 3], 1);
        if (((s16 *)0x80010000)[0x104] != 0)
        {
            func_800A6104();
            h = (s32)func_80026240;
        }
        else
        {
            func_80053DA8(0x503);
            h = (s32)func_800265B8;
        }
    }
    p[-4] = h;
    func_80020984();
    p[16] = 0;
}
