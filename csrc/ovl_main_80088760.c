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

/* The overlay's own data run at 0x8008B074..0x8008B380, which splat emits as
   asm/data/ovl_main_80088760_data.data.s. */
extern s32 D_8008B2F0[];

/* 0x80053858 IS A FUNCTION -- slus+0x27058 sits two words after a `jr $ra`
   and opens on `addiu $sp, $sp, -0x48` -- but this overlay only ever takes
   its address, never `jal`s it, so splat classifies the reference as data and
   main.bin_undefined_syms_auto.txt defines it as `D_80053858`. The __asm__
   label keeps the C honest about what it is while linking against the name
   splat actually emits; it is the same device csrc/main.h uses for
   D_800814A0_a. Declaring it `type:func` in main.bin_symbol_addrs.txt instead
   does not work: the address is outside this container, so splat either
   errors ("unable to determine a segment") or, with `absolute:True`, stops
   emitting the assignment and the link loses the symbol. */
extern void func_80053858(void) __asm__("D_80053858");
extern void func_80053A88(void) __asm__("D_80053A88");
extern void func_80044BB0(void) __asm__("D_80044BB0");

/* Two adjacent u16s and a u16 table, all in the overlay's own data run.
   D_8008B36A has no definition in asm/data/ovl_main_80088760_data.data.s:
   it is 2-aligned, so spimdisasm cannot label it, and
   main.bin_undefined_syms_auto.txt supplies it as an absolute instead. That
   is the project's standing arrangement for a 2-aligned symbol -- see
   D_80082E6A and friends in the resident units. */
extern u16 D_8008B368;
extern u16 D_8008B36A;
extern u16 D_8008B36C[];

s32 func_8004EE30(s32 *);
s32 *func_8003FC64(s32);

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

void func_80088B48(s32 *p)
{
    s16 *q;

    q = (s16 *)p[1];
    if (q[16] == 4)
    {
        ((u16 *)p)[11] &= 0xFFFD;
    }
    if (((s16 *)p)[0] == 0)
    {
        /* `q[18] * 16`, NOT `q[18] << 4`. The product is expanded too late for
           combine's force_to_mode to carry the `sh`'s 0xFFFF mask back to the
           load, so the sign-extending `lh` survives; written as a shift the
           mask reaches the load and it becomes `lhu`. One word.
           `s32 t = q[18];` first, or any s32 temp anywhere in the chain, does
           the same thing for the same reason. */
        ((s16 *)p)[7] = q[18] * 16 + 0xA0;
        if (((u16 *)q)[19] & 1)
        {
            ((u16 *)p)[-1] |= 0x8000;
            D_800814A0_a[0] |= 0x8000;
        }
    }
}

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

s32 *func_8008A98C(s32 *a, s32 *b, s32 c, s32 d)
{
    s32 *w;
    s32 *t;

    w = func_8003FC64(0x136);
    if (w != 0)
    {
        w[4] = (s32)b;
        ((s32 *)w[2])[0] = a[0];
        ((s32 *)w[2])[1] = a[1];
        func_8004491C((UnkNode *)w, (u32)func_80044BB0);
        t = (s32 *)w[3];
        ((s16 *)t)[15] = 0x1000;
        ((s16 *)t)[14] = 0x1000;
        t[2] = c;
        t[3] = 0x808080;
        w[8] = (s32)d;
    }
    return w;
}

void func_8008AA40(s16 **p)
{
    if (p[1][11] & 0x8000)
    {
        ((u16 *)p)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_8008AA84(s32 *p)
{
    u16 *q;

    q = (u16 *)p[1];
    ((s16 *)p)[8] = (s16)q[7 + p[6]] >> 2;
    if (((s16 *)q)[11] & 0x8000)
    {
        ((u16 *)p)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_8008AAE4(s32 *p)
{
    s32 *t;
    s16 *q;

    t = D_8008B2F0;
    q = (s16 *)p[1];
    ((s16 *)p)[8] = t[p[6]] >> 2;
    if (q[11] & 0x8000)
    {
        ((u16 *)p)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_8008AB44(s32 *p)
{
    s16 *q;
    s32 i;
    s32 v;

    q = (s16 *)p[1];
    p[2] = 0;
    i = 2;
    do
    {
        v = q[7 + i];
        if (v >= 0x100)
        {
            v = 0xFF;
        }
        p[2] += v << (i * 8);
    } while (--i >= 0);
    ((u16 *)p)[10] = ((u16 *)q)[10];
    if (q[11] & 0x8000)
    {
        ((u16 *)p)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_8008ABD4(s32 *p, Blk28 *src)
{
    s32 *w;

    w = func_8003FC64(1);
    if (w != 0)
    {
        *(Blk28 *)(w + 8) = *src;
        w[4] = (s32)p;
        func_8004491C((UnkNode *)w, (u32)func_80053A88);
    }
}

void func_8008AC70(s16 **p)
{
    if (p[0][11] & 0x8000)
    {
        ((u16 *)p)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_8008ACB4(s32 *p, s16 *out)
{
    s16 *q;

    q = (s16 *)p[0];
    if (q[6] == 0 && (((u16 *)q)[11] & 2))
    {
        out[1] = D_8008B36A;
    }
    else
    {
        out[1] = D_8008B368;
    }
    out[3] = D_8008B36C[q[6]];
    if (q[11] & 0x8000)
    {
        ((u16 *)p)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_8008AD58);

void func_8008AE60(s32 *p, Blk30 *src)
{
    s32 *w;

    w = func_8003FC64(1);
    if (w != 0)
    {
        *(Blk30 *)(w + 8) = *src;
        w[4] = (s32)p;
        func_8004491C((UnkNode *)w, (u32)func_80053858);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_8008AEEC);

void func_8008B030(s16 **p)
{
    if (p[3][11] & 0x8000)
    {
        ((u16 *)p)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}
