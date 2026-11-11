#include "common.h"

/* main.bin's overlay at +0x21800, loaded to 0x80088760 by the third
   descriptor in the engine's load table at vram 0x80080C38 -- 14 sectors
   from LBA 1884. It is the GAME OPTION screen; docs/overlays.md has how the
   block was placed and why the rodata is one dotted subsegment.

   Built at cc1psx -G0, like every overlay unit, via the Makefile's
   `build/csrc/ovl_%.o` pattern. Measured rather than assumed: 0 $gp refs and
   0 `lui $at` across the block's 115 `%hi`s, which is cc1psx's own
   hoisted-lui shape and not the assembler-expanded -G8 one.

   It shares the 0x80088760 slot with town.bin's and dungeon.bin's overlays,
   so the three are mutually exclusive at runtime. That costs nothing here:
   each container links from its own build/*.ld and never sees another
   container's objects. */

/* The array spelling of D_800814A0, which this overlay's "mark the widget
   dirty" idiom needs. Both halves of that idiom update a global in place
   beside a u16 store, and the array form is what keeps cse from caching the
   load across the store, so the target's unfilled load-delay nop survives --
   see csrc/main.h's note on the same declaration, and docs/matching-levers.md
   section 5. The lever is PER SITE; every site in this unit wants the array
   form. */
extern u32 D_800814A0_a[1] __asm__("D_800814A0");

s32 func_8004EE30(s32 *);

void func_80088BD0(s32 *);

void func_800888C8(void) {
}

void func_800888D0(s32 *p)
{
    if (((u16 *)p)[-1] & 0x2000)
    {
        func_80088BD0(p);
        ((u16 *)p)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_80088930(s32 *p)
{
    s32 *q;

    q = p - 8;
    if (func_8004EE30(q))
    {
        q[4] = (s32)func_800888D0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_8008896C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_80088B48);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_80088BD0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_80088EDC);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80088760", D_80088760);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_800890EC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_80089608);

void func_80089A14(s32 *p)
{
    ((u16 *)p)[-1] |= 0x8000;
    D_800814A0_a[0] |= 0x8000;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_80089A38);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_80089C24);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80088760", D_80088790);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80088760", D_800887B8);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80088760", D_800887CC);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80088760", D_800888AC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_8008A288);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_8008A794);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_8008A98C);

void func_8008AA40(s16 **p)
{
    if (p[1][11] & 0x8000)
    {
        ((u16 *)p)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_8008AA84);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_8008AAE4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_8008AB44);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_8008ABD4);

void func_8008AC70(s16 **p)
{
    if (p[0][11] & 0x8000)
    {
        ((u16 *)p)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_8008ACB4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_8008AD58);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_8008AE60);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_8008AEEC);

void func_8008B030(s16 **p)
{
    if (p[3][11] & 0x8000)
    {
        ((u16 *)p)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}
