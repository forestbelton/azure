#include "common.h"

/* 46 words, 4 DIFF, register naming only: the target materialises the comparison's 1 into $v0 and COPIES it into $s3 for the flag ('addu $s3, $v0, $zero', which reorg then puts in the beq's delay slot), while every spelling here puts the 1 straight into $s3 and compares against that. Four measured: flag initialised at its declaration or assigned before the if, the test as == with the 1-assignment in the then arm, as != with it in the else, and a shared 'one' local. Initialising the loop counter and cursor AFTER the if is what took it from 13 DIFF to 4 and must be kept.

   RE-MEASURED 2026-11-01 with the narrower-destination lever from
   azure-matching-tips (a copy survives as a second register only if cse
   cannot see it as a same-mode register copy). `u8 flag` TRADES three
   differing words for three others and stays at 4: it fixes words 7-9
   exactly -- the tested value in $v1, the 1 materialised into $v0, the
   compare against $v0 -- but the flag still gets its own `li $s3, 1` instead
   of the target's copy, the prologue's $s3 and $ra saves come out swapped,
   and the u8 costs an `andi $a1, $s3, 0xff` at the call where the target has
   `addu $a1, $s3, $zero`. `u16` behaves the same; `s8` and `s16` are 47
   words. So the four words are two separate problems and no single
   declaration reaches both.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

void func_80025030(s32 *p)
{
    s32 flag = 1;
    s32 off;
    s32 i;

    if (p[0x12] != 1)
    {
        flag = 0;
        func_80023A50((s32 *)p[6], p[0x11]);
    }
    off = 0;
    i = 0;
    do
    {
        if (i == p[10])
        {
            func_800241D4((s32 *)p[off + 1], flag);
        }
        else
        {
            func_80024274((s32 *)p[off + 1]);
        }
        off++;
    } while (++i < 5);
    func_80024F3C((s32 *)p[7], p[10], p[0x11]);
}
