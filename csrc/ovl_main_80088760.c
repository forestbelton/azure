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

/* Draws the two 256x256 background tiles of the options screen, for every
   sibling in the widget chain. Each tile is an SPRT plus a draw-mode
   primitive that selects its texture page, both taken from the +0x8D0 bump
   allocator and linked into the OT at +0x82C.

   SetSprt/SetShadeTex/GetClut/GetTPage are the real libgpu FUNCTIONS -- they
   are `jal`s here and splat/common_syms.txt names all four. func_80067F20 is
   libgpu's SetDrawMode on five witnesses: five arguments matching its
   signature, (dfe=0, dtd=0, tpage, tw=NULL) usage, a 0xC-byte allocation
   which is sizeof(DR_MODE), and SetDrawArea/SetDrawOffset named at 0x80067E2C
   and 0x80067EB0 immediately before it. It is called by the project's own
   name because common_syms.txt does not name it and nine call sites in three
   other units already use func_80067F20; naming it there is a separate,
   project-wide change.

   TWO SEPARATE, PROPERLY TYPED LOCALS, not one reused `u8 *`. That is worth
   14 words of register noise and nothing else: with one pointer serving both
   primitives its live range spans both `addPrim`s, so gcc cannot mask it
   destructively and the `(u_long)p & 0xFFFFFF` inside each `setaddr(ot, p)`
   needs a fresh register, which pushes the OT base out into $a2/$a0. With `s`
   and `dm` each dying at its own addPrim the target's `and $s0, $s0, $s1` is
   available and every register falls into place. Spelling the cast `(u32)q`
   does not help -- it is the live range, not the type of the expression.

   Two orderings matter as well, 3 words each: `j = 1` is written BEFORE `y`
   and `tx` at the loop head, and the halfword stores go in the listing's
   order grouped by value (x0, then w/h together, then u0v0, then y0) rather
   than in field order -- the same grouping rule as func_80053604 in
   csrc/unk_264E0.c. `v = y - w[3]` is its own block-scope local, so it gets a
   register of its own and its load can be hoisted above the 0x100 pair. */
s32 func_8008896C(u8 *p, u16 *w)
{
    Unk80083160 *g;
    SPRT *s;
    DR_MODE *dm;
    s32 i;
    s32 j;
    s32 y;
    s32 tx;
    u8 *next;

    g = &D_80083160;
    while (1)
    {
        for (i = 0; i < 2; i++)
        {
            j = 1;
            y = i << 8;
            tx = 0x280;
            do
            {
                s32 v;

                s = (SPRT *)*(u8 **)(g->unk_00 + 0x8D0);
                *(u8 **)(g->unk_00 + 0x8D0) = (u8 *)s + 0x14;
                *(u32 *)&s->r0 = 0x808080;
                SetSprt(s);
                SetShadeTex(s, 1);
                s->x0 = j << 8;
                v = y - w[3];
                s->w = 0x100;
                s->h = 0x100;
                *(s16 *)&s->u0 = 0;
                s->y0 = v;
                s->clut = GetClut(0, 0x1F0);
                addPrim(g->unk_00 + 0x82C, s);
                dm = (DR_MODE *)*(u8 **)(g->unk_00 + 0x8D0);
                *(u8 **)(g->unk_00 + 0x8D0) = (u8 *)dm + 0xC;
                func_80067F20((u32 *)dm, 0, 0, GetTPage(1, 0, tx, y), 0);
                addPrim(g->unk_00 + 0x82C, dm);
                tx -= 0x80;
            } while (--j >= 0);
        }
        next = *(u8 **)(p - 8);
        if (next == 0)
        {
            break;
        }
        p = next + 0x20;
        w = *(u16 **)(next + 8);
    }
    return 0;
}

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

/* The widget's own state machine, six live states on the s16 at +0x08 plus an
   empty one. State 1 counts down and walks the 16.16 value at +0x0C toward
   zero; 2 eases the halfword at +0x02 of the second argument toward 0xA0; 0x10
   latches it and branches on two fields of the object at p[0]; 0x20 sets or
   clears bit 0x80 of the third argument's +0x14; 0x21 ramps +0x00 by +0x0C
   while +0x00 stays under 0x1FFFFFF; 0xFF marks the widget dirty.

   `case 0: break;` IS LOAD-BEARING, and it is the whole function. Without it
   gcc builds a different case-node tree -- two plain equality tests for the
   {1, 2} subtree instead of the target's equality-then-range-test pair -- and
   the result is 130 words against 132 with 116 of them differing, because
   every branch offset downstream shifts. With it: 0 differing, first try.
   An empty case is not a no-op to this compiler; it is a node in
   balance_case_nodes' tree and it changes how every sibling is tested.
   func_80089A38 has the same empty case 0 for the same reason.

   Two smaller shapes, both forced by the listing: state 0x10 stores the
   VALUE it compared (`c = r[16]; if (c == 0x20) ... p->h4 = c;`) rather than
   the literal 0x20, which is why the target has `sh $a2` and not a fresh
   `addiu`; and state 2 reads the +0x02 halfword twice, signed for the
   subtraction and unsigned for the addend, which is one `s16` field used two
   ways rather than two fields. */
void func_80088EDC(s32 *p, s32 *q, u16 *s)
{
    s16 *r;

    r = (s16 *)p[0];
    if (((u16 *)r)[19] & 1)
    {
        ((s16 *)p)[4] = 0xFF;
    }
    switch (((s16 *)p)[4])
    {
    case 0:
        break;
    case 1:
        if (--((s16 *)p)[5] <= 0)
        {
            s32 d;

            q[0] += q[3];
            d = q[3] - 0x10000;
            q[3] = d;
            if (d == 0)
            {
                ((s16 *)p)[5] = 4;
                ((s16 *)p)[4] = 2;
            }
        }
        break;
    case 2:
        ((s16 *)q)[1] += (0xA0 - ((s16 *)q)[1]) >> 1;
        if (--((s16 *)p)[5] <= 0)
        {
            ((s16 *)p)[4] = 0x10;
        }
        break;
    case 0x10:
        {
            s32 c;

            ((s16 *)q)[1] = 0xA0;
            c = r[16];
            if (c == 0x20)
            {
                if (r[18] == ((s16 *)p)[6])
                {
                    ((s16 *)p)[4] = c;
                }
                else
                {
                    q[3] = -0x80000;
                    ((s16 *)p)[4] = 0x21;
                }
            }
        }
        break;
    case 0x20:
        if ((((u16 *)r)[17] >> 3) & 1)
        {
            s[10] |= 0x80;
        }
        else
        {
            s[10] &= 0xFF7F;
        }
        break;
    case 0x21:
        {
            s32 x;

            x = q[0];
            if (x <= 0x1FFFFFF)
            {
                q[0] = x + q[3];
                q[3] += 0x20000;
            }
        }
        break;
    default:
        break;
    case 0xFF:
        ((u16 *)p)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
        break;
    }
}

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

/* func_8008896C's shadow pass: the same two tiles per widget, but drawn three
   times at offsets k = 2, 1, 0 in 0x505050, semi-transparent instead of
   shade-textured, and out of texture page bank 3 instead of 0. Everything
   that function's comment says about the two primitive locals, the init
   order and the halfword grouping applies here unchanged -- written that way
   it matched first try.

   func_80066640 is SetSemiTrans and func_80066668 (used by func_8008896C) is
   SetShadeTex, both named in splat/common_syms.txt, and libgpu.h declares
   both as real functions rather than macros, which is why they are `jal`s.
   The extra loop level costs `w` its saved register: with k, i and j all
   live, gcc spills the second parameter to its argument home and reloads it
   per iteration, which is the `lw $t0, 0x44($sp)` in the body. */
s32 func_8008A794(u8 *p, u16 *w)
{
    Unk80083160 *g;
    SPRT *s;
    DR_MODE *dm;
    s32 i;
    s32 j;
    s32 k;
    s32 y;
    s32 tx;
    u8 *next;

    g = &D_80083160;
    while (1)
    {
        for (k = 2; k >= 0; k--)
        {
            for (i = 0; i < 2; i++)
            {
                j = 1;
                y = i << 8;
                tx = 0x280;
                do
                {
                    s32 v;

                    s = (SPRT *)*(u8 **)(g->unk_00 + 0x8D0);
                    *(u8 **)(g->unk_00 + 0x8D0) = (u8 *)s + 0x14;
                    *(u32 *)&s->r0 = 0x505050;
                    SetSprt(s);
                    SetSemiTrans(s, 1);
                    s->x0 = (j << 8) + k;
                    v = (y - w[3]) + k;
                    s->w = 0x100;
                    s->h = 0x100;
                    *(s16 *)&s->u0 = 0;
                    s->y0 = v;
                    s->clut = GetClut(0, 0x1F0);
                    addPrim(g->unk_00 + 0x82C, s);
                    dm = (DR_MODE *)*(u8 **)(g->unk_00 + 0x8D0);
                    *(u8 **)(g->unk_00 + 0x8D0) = (u8 *)dm + 0xC;
                    func_80067F20((u32 *)dm, 0, 0, GetTPage(1, 3, tx, y), 0);
                    addPrim(g->unk_00 + 0x82C, dm);
                    tx -= 0x80;
                } while (--j >= 0);
            }
        }
        next = *(u8 **)(p - 8);
        if (next == 0)
        {
            break;
        }
        p = next + 0x20;
        w = *(u16 **)(next + 8);
    }
    return 0;
}

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

void func_8008AD58(s32 *p)
{
    u16 t[2];
    s16 *q;
    s32 on;

    q = (s16 *)p[3];
    t[0] = 2;
    t[1] = 1;
    /* `on = 0` first, and the else arm as an `if` that only ever STORES 1.
       Written symmetrically (`on = p[9] == 0;` in the else) the else arm
       becomes `sltiu` and the then arm's `addu $a0, $zero, $zero` has no
       reason to exist -- reorg puts that zeroing in the `beqz`'s delay slot
       precisely because both paths can run it. 2 words. */
    on = 0;
    if (((u16 *)q)[11] & t[p[8]])
    {
        on = p[9] != 0;
    }
    else if (p[9] == 0)
    {
        on = 1;
    }
    /* Each arm declares its OWN `v`. One shared function-scope local is the
       same 66 words with every opcode, immediate and displacement right and
       4 REGISTERS wrong: the else arm's constant and its loaded p[4] swap
       $v0/$a0. A block-scope variable is allocated before a function-scope
       one in gcc 2.7.2, which is what puts them back -- see
       docs/compiler-version.md's note on the two tie-breaks. */
    if (on)
    {
        s32 v;

        v = p[4] + 0x101010;
        p[4] = v;
        if (v > 0x808080)
        {
            p[4] = 0x808080;
        }
    }
    else
    {
        s32 v;

        v = p[4] - 0x101010;
        p[4] = v;
        if (v <= 0x30302F)
        {
            p[4] = 0x303030;
        }
    }
    if (q[11] & 0x8000)
    {
        ((u16 *)p)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

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

void func_8008AEEC(s32 *p)
{
    s16 *h;
    s16 *q;

    h = (s16 *)p;
    q = (s16 *)p[3];
    h[1]--;
    switch (h[0])
    {
    case 0:
        if (h[1] <= 0)
        {
            p[4] = 0xC0C0C0;
            ((u16 *)p)[14] |= 1;
            h[0]++;
        }
        break;
    case 1:
        {
            s32 v;

            v = p[4] - 0x101010;
            p[4] = v;
            if (v == 0)
            {
                h[1] = 0x3C;
                h[0]++;
            }
        }
        break;
    case 2:
        if (h[1] <= 0)
        {
            h[0]++;
        }
        break;
    case 3:
        {
            s32 v;

            v = p[4] + 0x101010;
            p[4] = v;
            if (v == 0xC0C0C0)
            {
                p[4] = 0x808080;
                h[1] = 0x5A;
                h[0] = 0;
                ((u16 *)p)[14] &= 0xFFFE;
            }
        }
        break;
    }
    if (q[11] & 0x8000)
    {
        ((u16 *)p)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_8008B030(s16 **p)
{
    if (p[3][11] & 0x8000)
    {
        ((u16 *)p)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}
