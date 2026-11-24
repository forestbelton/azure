#include "common.h"

s32 func_8009D218_2(u8 *, s32) __asm__("func_8009D218");
s32 func_800A6870(s16);
void func_800AD4D0(u8 *);
s32 func_800AD568(u8 *);
UnkNode *func_800B4C7C(u16, u8 *, s16, s16);
void func_s30_80024024(u8 *p, u8 k);

/* Raise the actor's +0x64 pool by func_800A6870's scale of the second
   parameter plus 4, half again when the actor's +0x28 has bit 0, then
   republish it with func_800AD568, float the 0x8004 number and refresh with
   func_800AD4D0. All of it gated on func_8009D218 coming back zero.
   39 words, 0 differing, first compile.

   TWIN: the landed func_s27_80024050 in csrc/ovl_dng_s27.c (1.000 to two
   stubbed clones, 0.949 to this). Deltas, all off this listing: the gate
   argument is 1 not 4, the scale bonus is +4 not +8, the +0x28 bit is 1 not
   4, and the fraction is `>> 1' (`sll 16'/`sra 17') not `>> 2'.

   Of the twin's two stated facts:
     - HOLDS: the second parameter is `u8', which is what makes the
       `andi $a0, $s1, 0xFF' ahead of func_800A6870 the parameter's own
       truncation; and func_8009D218 is reached through the project's
       TWO-argument alias.
     - HOLDS on the WIDTH, WEAKER on the PAIR: the twin calls its strongest
       fact "the pair of `s16' locals with the plain `u = t;' ahead of the
       gate", priced at 38 words / 28 differing for one variable. Measured
       here: `s32 t; s32 u;' is 37 words / 27 differing, so the `s16' WIDTH
       is load-bearing and reproduces the twin's price to one -- but ONE
       variable with `t += (s16)t >> 1;' and no `u' at all is 39 words /
       0 differing. The two-variable spelling is FREE on this copy. 
   THIS COPY'S OWN CONSTANT: 0x1 -> 0x4 at 1 of 4 sites, 0x4 -> 0x10.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s30_80024024(u8 *p, u8 k)
{
    s16 t;
    s16 u;

    if (func_8009D218_2(p, 4) == 0)
    {
        t = func_800A6870(k) + 16;
        u = t;
        if ((p[0x28] & 1) != 0)
        {
            u = t + (t >> 1);
        }
        *(s16 *)(p + 0x64) = *(u16 *)(p + 0x64) + u;
        func_800AD568(p);
        func_800B4C7C(0x8004, p, *(s16 *)(p + 0x64), 1);
        func_800AD4D0(p);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_800240C0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_800241D4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_80024640);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_80024710);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_800247AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_80024A08);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_80024AF4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_80024C48);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_80024F0C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_80025098);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_8002523C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_80025390);
