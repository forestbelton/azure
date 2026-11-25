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
void func_800478B8(u8 *);

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

/* A per-frame step function: bump the +0x14 counter on the node at the
   actor's +0x0 and the actor's own +0x4 counter, drag the mover's +0x8 down
   by 0xA0000, hand the renderable to func_800478B8, and when its +0x14
   reports 0x6000 mark the owning actor retired and set the same bit in
   D_800814A0.
   39 words, 0 differing, first compile.

   TWIN: the landed func_s13_8002418C in csrc/ovl_dng_s13.c (dngtwin 0.933,
   36 words). Its head statement, its gate and its whole retire block carry
   verbatim; the three extra words are this copy's own middle, where the
   twin's `+0x1E' scale pair is replaced by the actor's +0x4 counter bump and
   the `b->unk_08 -= 0xA0000'. The twin's note on D_800814A0 being the PLAIN
   `u32' scalar (structs.h, no declaration needed) holds here too and gives
   the `lui %hi'/`lw %lo' pair, with the `lui' hoisted into the gate's delay
   slot. The twin's own priced fact -- the scale pair as a read-back, not a
   second sum -- does not apply: this copy has no scale pair.

   ONLY THE STATEMENT ORDER IS LOAD-BEARING. Priced on this body, all at
   39 words:

     the +0x4 counter bump BEFORE the +0x8 drag               0   kept
       ... the two swapped, the drag first                    8
     `b->unk_08 -= 0xA0000;'                                  0   kept
       ... `+= -0xA0000;'                                     0   free
       ... `*(s32 *)((u8 *)b + 0x8) -= 0xA0000;'              0   free
       ... second parameter `u8 *b', `*(s32 *)(b + 0x8)'      0   free
     the +0x4 bump read through `u16'                         0   kept
       ... read through `s16'                                 0   free
     the head reached through `*(u8 **)p' twice               0   kept
       ... hoisted into a `u8 *n' local                       0   free
     the gate read through `u16', the retire store `u16'      0   kept
       ... either one through `s16'                           0   free

   So the UnkVec second parameter is the twin's spelling rather than a
   measured one, and the only thing this listing actually constrains is that
   the +0x8 drag is emitted last of the three. The drag is a `lui 0xFFF6' plus
   `addu' because 0xFFF60000 is outside `addiu' range; the `-=' and `+= -'
   forms are the same tree. 
   THIS COPY'S OWN CONSTANT: 0x14 -> 0xc at 2 of 3 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s30_80024710(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(*(u8 **)p + 0xc) = *(u16 *)(*(u8 **)p + 0xc) + 1;
    *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
    b->unk_08 -= 0xA0000;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_800247AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_80024A08);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_80024AF4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_80024C48);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_80024F0C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_80025098);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_8002523C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_80025390);
