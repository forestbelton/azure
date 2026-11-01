#include "common.h"

/* NEAR MISS: 82 words, 11 DIFF, and all eleven sit in one region -- the two switch arms that zero a word and the shared store they cross-jump into. It is the overlay's only function with a real JUMP TABLE: jtbl_80020060 has five entries for st 1..5, grouped 1+2, 3, 4+5, which a plain switch reproduces exactly. The target gives each arm its own 'sll $v1, $a0, 2' and cross-jumps only the two-word 'addu'/'sw' tail, so the shift stays live into the shared 'D_80028530[i] >= 0xB' check; every spelling here cross-jumps the shift as well, leaving the arms to pass only the symbol base and swapping $v0 and $v1 through the whole region. Four arm spellings measured (two direct stores, a pointer local in either arm, separate pointer locals in both) all give 82w/11, and so does declaring D_80028530/D_80028538 as arrays rather than scalars. Two things ARE load-bearing and right: the st==0 exit must be an enclosing 'if (st != 0)' with a single 'return st' at the end, because the target's early branch jumps to the shared 'addu $v0, $s0, $zero' instead of materialising a zero (as 'return st' early it is 82 words but 46 DIFF), and the two zeroing arms must reach the check through a 'goto', not each carry their own copy of it -- duplicated, cse forwards the stored 0 into arm 3's comparison and folds it away (83w/36).

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

s32 func_8002168C(void)
{
    s32 st = func_80021594();
    s32 i = func_8002190C();

    if (st != 0)
    {
        if ((&D_80028538)[i] == st)
        {
            (&D_80028530)[i]++;
        }
        else
        {
            (&D_80028530)[i] = 0;
        }
        (&D_80028538)[i] = st;
        switch (st)
        {
        case 1:
        case 2:
            (&D_800287D0)[i] = st;
            break;
        case 3:
            (&D_80028530)[i] = 0;
            goto check;
        case 4:
        case 5:
            (&D_80084118)[i] = 0;
        check:
            if ((&D_80028530)[i] >= 0xB)
            {
                (&D_800287D0)[i] = st;
                (&D_80028530)[i] = 0;
                (&D_80028538)[i] = 0;
            }
            break;
        }
        func_80020924(i);
    }
    return st;
}
