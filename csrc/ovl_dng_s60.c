#include "common.h"

void func_800478B8(u8 *);
void func_s60_8002548C(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u32 D_800814A0;

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s60", func_s60_80024050);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s60", func_s60_800243C0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s60", func_s60_800244B0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s60", func_s60_80024770);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s60", func_s60_80024A30);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s60", func_s60_80024D84);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s60", func_s60_800251E8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s60", func_s60_80025340);

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
   for the `+ 1'. 
   THIS COPY'S OWN CONSTANT: 0x14 -> 0x1a at 2 of 3 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s60_8002548C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)(*(u8 **)(p + 0x0) + 0x1a) = *(u16 *)(*(u8 **)(p + 0x0) + 0x1a) + 1;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s60", func_s60_80025508);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s60", func_s60_8002560C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s60", func_s60_800257B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s60", func_s60_80025940);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s60", func_s60_80025A9C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s60", func_s60_80025C28);
