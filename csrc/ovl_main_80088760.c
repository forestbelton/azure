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
void func_800478B8(s32);

/* NO ARGUMENT. func_8008A288 calls this with $a0 holding whatever
   func_80053604 left in it and no setup of its own, which only works if the
   callee takes nothing. func_800888D0 appears to pass its own parameter, but
   that is a coincidence of allocation: p is still live in $a0 there, so the
   argument move is a no-op gcc deletes. Declaring a parameter here would make
   func_8008A288 emit a real `addu $a0, ...` that the target does not have. */
void func_80088BD0(void);

extern RECT D_800888AC;
extern s32 D_8008B2B0;
extern s32 D_8008B2B4[];
extern s32 D_8008B2BC[];
extern s32 D_8008B2C4[];
extern s32 D_8008B2D4[];
extern s32 D_8008B2DC[];
extern s32 D_8008B2E4[];
extern s32 D_8008B2FC[];
extern u8 D_80078AF0[];
extern u8 D_80013180[];
extern u8 D_80013181[];
extern u8 D_80013182[];
extern u8 D_80013184[];
extern u8 D_80013186[];
extern u8 D_800786E8[];

s32 func_8008A794(u8 *, u16 *);
void func_8008A288(s32 *);
s32 *func_8008A98C(s32 *, s32 *, s32, s32);
void func_8008AA40(s16 **);
void func_8008AA84(s32 *);
void func_8008AAE4(s32 *);
void func_8008AB44(s32 *);
void func_8008ABD4(s32 *, Blk28 *);
void func_8008AC70(s16 **);
void func_8008ACB4(s32 *, s16 *);
void func_8008AD58(s32 *);
void func_8008AE60(s32 *, Blk30 *);
void func_8008AEEC(s32 *);
void func_8008B030(s16 **);

/* THE COLOUR BYTES WANT THE ARRAY SPELLING. The target keeps each
   `lui %hi(X)` glued to its own `lbu %lo(X)`, in source order, with the
   load-delay nop after the one feeding the `>> 5`. As five `extern u8 X;`
   scalars -- or as five `*(u8 *)0x8001318N` constants -- gcc hoists all four
   loads into four separate registers, fills that delay slot and comes out one
   word SHORT. An array reference sets MEM_IN_STRUCT_P, which is what keeps
   the loads dependent on the stores between them. 409 words / 56 DIFF against
   408 / 359 for either of the other two; the single biggest step here.
   They sit outside the loaded image, so docs' usual constant-address rule
   would have applied -- it does not. */

/* The 0x30-byte widget descriptor func_8008AE60 copies into the node it
   allocates: +0x00/+0x02 are the state machine's state and timer,
   +0x0C the parent node, +0x10 the 24-bit colour, +0x14/+0x16 the
   position. */
typedef struct BlkA
{
    s16 unk_00;
    s16 unk_02;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s16 unk_14;
    u16 unk_16;
    s16 unk_18;
    s16 unk_1A;
    s16 unk_1C;
    s16 unk_1E;
    s32 unk_20;
    s32 unk_24;
    s32 unk_28;
    s32 unk_2C;
} BlkA;

/* The 0x28-byte one func_8008ABD4 copies: +0x04 the parent node, +0x08 the
   colour, +0x18 the row index its handlers read as p[6]. */
typedef struct BlkB
{
    s16 unk_00;
    s16 unk_02;
    s32 unk_04;
    s32 unk_08;
    s16 unk_0C;
    s16 unk_0E;
    s16 unk_10;
    s16 unk_12;
    s16 unk_14;
    s16 unk_16;
    s32 unk_18;
    s32 unk_1C;
    s32 unk_20;
    s32 unk_24;
} BlkB;

extern u8 D_8008B074[];
extern u8 D_8008B230[];
extern u8 D_8008B270[];
s32 func_8008896C(u8 *, u16 *);
void func_800890EC(s32 *, s32 *);
void func_80088EDC(s32 *, s32 *, u16 *);
void func_80088B48(s32 *);
void func_80089A38(s32 *, s32 *, s32);
void func_80089608(s32 *);
extern void func_80044BB0(void) __asm__("D_80044BB0");
extern void func_80053A88(void) __asm__("D_80053A88");

void func_800888C8(void) {
}

void func_800888D0(s32 *p)
{
    if (((u16 *)p)[-1] & 0x2000)
    {
        func_80088BD0();
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

void func_80088BD0(void)
{
    u8 *root = 0;
    u8 *tbl[3] = { &D_8008B074[0xFC], &D_8008B074[0xF0], &D_8008B074[0x108] };
    s32 *n;
    u8 *w;
    u8 *c;
    UnkSlot *sl;
    s32 i;
    s32 a;
    s32 b;

    D_80080A84 = 1;
    n = func_8003FC64(0x11);
    if (n != 0)
    {
        root = (u8 *)n + 0x20;
        n[4] = (s32)func_800890EC;
        func_8004491C((UnkNode *)n, (u32)func_8008896C);
        *(s16 *)(root + 0x22) = 0x708;
        *(s16 *)(n[2] + 6) = 0xF0;
        *(s16 *)(root + 0x1A) = 0;
        *(s16 *)(root + 0x1C) = 0x40;
    }
    i = 2;
    a = 0x3C;
    b = 0xC8;
    do
    {
        n = func_8003FC64(0x111);
        ((u8 **)(root + 4))[i] = (u8 *)n;
        if (n != 0)
        {
            w = (u8 *)n + 0x20;
            sl = (UnkSlot *)n[3];
            n[4] = (s32)func_80088EDC;
            func_8004491C((UnkNode *)n, (u32)func_80044BB0);
            *(s16 *)(n[2] + 2) = -0x40;
            *(s16 *)(n[2] + 6) = b;
            *(s32 *)(n[2] + 0xC) = 0x150000;
            *(s16 *)((u8 *)sl + 0x1E) = 0x1000;
            *(s16 *)((u8 *)sl + 0x1C) = 0x1000;
            sl->unk_08 = (u32)tbl[i];
            sl->unk_04 = 0;
            sl->unk_05 = 0;
            *(s32 *)((u8 *)sl + 0xC) = 0x808080;
            n[8] = (s32)root;
            *(s16 *)(w + 0xC) = i;
            *(s16 *)(w + 0xA) = a;
        }
        a -= 0x1E;
        b -= 0x10;
    } while (--i >= 0);
    n = func_8003FC64(1);
    if (n != 0)
    {
        n[4] = (s32)func_80088B48;
        func_8004491C((UnkNode *)n, (u32)func_80053A88);
        c = (u8 *)n + 0x20;
        *(s16 *)(c + 0xC) = 0x60;
        *(s16 *)(c + 0xE) = -0x100;
        *(s16 *)(c + 0x10) = 0x80;
        *(s16 *)(c + 0x12) = 0x10;
        *(s16 *)(c + 0x14) = 0;
        *(s16 *)(c + 0x16) = 3;
        *(s32 *)(c + 8) = 0x606060;
        *(s16 *)((u8 *)n + 0x20) = 0;
        *(s32 *)(c + 4) = (s32)root;
    }
    i = 1;
    do
    {
        n = func_8003FC64(0x111);
        ((u8 **)(root + 0x10))[i] = (u8 *)n;
        if (n != 0)
        {
            w = (u8 *)n + 0x20;
            sl = (UnkSlot *)n[3];
            n[4] = (s32)func_80089A38;
            func_8004491C((UnkNode *)n, (u32)func_80044BB0);
            *(s16 *)(n[2] + 6) = 0xE9;
            if (i != 0)
            {
                *(s16 *)(n[2] + 2) = -0x15;
                *(s32 *)(n[2] + 0xC) = 0x160000;
                func_8003DB94((UnkSlot *)sl, (UnkElem *)D_8008B270, 0);
            }
            else
            {
                *(s16 *)(n[2] + 2) = 0x155;
                *(s32 *)(n[2] + 0xC) = 0xFFEA0000;
                func_8003DB94((UnkSlot *)sl, (UnkElem *)D_8008B230, 0);
            }
            *(s32 *)(n[2] + 0x10) = 0xFFF80000;
            *(s16 *)((u8 *)sl + 0x1E) = 0x1000;
            *(s16 *)((u8 *)sl + 0x1C) = 0x1000;
            *(s32 *)((u8 *)sl + 0xC) = 0x808080;
            *(s32 *)(w + 0) = (s32)root;
            *(s16 *)(w + 0xC) = i;
        }
    } while (--i >= 0);
    n = func_8003FC64(1);
    if (n != 0)
    {
        n[4] = (s32)func_80089608;
        n[8] = (s32)root;
        *(s16 *)((u8 *)n + 0x2E) = 0xE00;
    }
}

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


INCLUDE_ASM("asm/nonmatchings/ovl_main_80088760", func_800890EC);
