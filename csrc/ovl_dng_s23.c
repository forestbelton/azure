#include "common.h"

void func_800478B8(u8 *);
void func_s23_80024550(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u32 D_800814A0;

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s23", func_s23_80024024);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s23", func_s23_800240CC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s23", func_s23_800241D8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s23", func_s23_80024264);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s23", func_s23_80024370);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s23", func_s23_8002441C);

/* The 0x212 sprite's own step function: bump the owning node's +0x14 frame
   counter, hand the renderable to func_800478B8, and when its +0x14 reports
   0x6000 mark the owning actor retired and set the same bit in D_800814A0.
   31 words, 0 differing.

   This is the landed func_s10_80024374 in csrc/ovl_dng_s10.c (0.862 by
   dngtwin.py) plus the four-word counter bump on `*(u8 **)(p + 0x0)' at the
   front; that twin's note about D_800814A0 being the PLAIN scalar rather than
   an array alias holds here too and is what gives the `lui' / `lw %lo' pair.
   Measured and FREE at 31 words / 0 differing, so it is NOT the reason this
   lands: reading the counter back through `*(s16 *)' instead of `*(u16 *)'
   for the `+ 1'. */
void func_s23_80024550(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)(*(u8 **)(p + 0x0) + 0x14) = *(u16 *)(*(u8 **)(p + 0x0) + 0x14) + 1;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s23", func_s23_800245CC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s23", func_s23_800246BC);
