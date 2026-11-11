/* Carved out of ovl_main_80088760; see docs/addressing-and-translation-units.md.
 * Replace this header with the evidence for the split. */
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

extern s32 D_8008B2B0;
extern s32 D_8008B2B4[];
extern s32 D_8008B2BC[];
extern s32 D_8008B2C4[];
extern s32 D_8008B2D4[];
extern s32 D_8008B2DC[];
extern s32 D_8008B2E4[];
extern s32 D_8008B2FC[];
extern u8 D_80078AF0[];
extern u32 D_80013180;
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

/* The 256x1 VRAM strip func_80089C24 copies onto its stack. It used to be
   splat's own `ovl_main_80088760_rect` subsegment; owning it here retires that
   subsegment and its generated listing.

   IT HAS TO LIVE IN THIS UNIT, AND FIRST. sizeof(RECT) is 8, so placing it at
   this unit's rodata offset 0 leaves jtbl_800888B4 at offset 8 -- already
   8-aligned, so the `.align 3` cc1psx emits before a jump table pads nothing.
   That is the same reason this unit was carved out at all; see
   docs/matching-levers.md, "cc1psx EMITS `.align 3` FOR JUMP TABLES". The
   unit's rodata consequently starts at 0x2194c, not 0x21954. */
const RECT D_800888AC = {0, 0x1F0, 0x100, 1};

void func_8008A288(s32 *p)
{
    Unk80083160 *g;
    /* `d` AND `c` ARE EACH ONE VARIABLE DOING TWO JOBS, and that is worth ten
       words of register names -- five each, measured by splitting them.
       `d` is the slider step in case 2's first half and the wall-colour
       LEVEL in its second; `c` is the slider index in both arms and the
       ASSEMBLED COLOUR afterwards.  The two live ranges never overlap, so one
       pseudo covers both, and the pseudo inherits the register the later
       (loop-weighted, and so higher-priority) half wants: `d` becomes $a3
       because the level does, `c` becomes $a2 because the colour does.  Split
       into four locals the step lands in $a0 and the index in $a1, because
       `prune_preferences` only reserves $v0/$v1 against them and `find_reg`
       then takes the lowest free register -- global.c:956.  Nothing about the
       logic changes; this is purely which quantity the allocator sees. */
    s32 d;
    s32 i;
    s32 c;

    D_80080A84 = 1;
    g = &D_80083160;
    switch (((s16 *)p)[4])
    {
    case 0:
        ((s16 *)p)[4]++;
        /* fallthrough */
    case 1:
        if ((s16)func_80053428((Unk80083120 *)p) != 0)
        {
            ((s16 *)p)[5] = 0x12C;
            ((s16 *)p)[4]++;
        }
        break;
    case 2:
        if (g->unk_10 & 0x1000)
        {
            func_80053DA8(0x502);
            ((s16 *)p)[6] = (((s16 *)p)[6] + 6) % 7;
        }
        else if (g->unk_10 & 0x4000)
        {
            func_80053DA8(0x502);
            ((s16 *)p)[6] = (((s16 *)p)[6] + 1) % 7;
        }
        else if (g->unk_10 & 0xA000)
        {
            if (((s16 *)p)[6] == 0)
            {
                u16 f;

                func_80053DA8(0x502);
                f = ((u16 *)p)[11];
                if (f & 2)
                {
                    ((u16 *)p)[11] = f & 0xFFFD;
                }
                else
                {
                    ((u16 *)p)[11] = f | 2;
                }
            }
        }
        if (g->unk_08 & 0x8000)
        {
            d = -8;
        }
        else
        {
            s32 m;

            m = g->unk_08 & 0x2000;
            d = (m != 0) << 3;
        }
        if (d != 0)
        {
            i = ((s16 *)p)[6];
            if (i > 0)
            {
                /* THE DISPATCH IS GOTO-SHAPED, and that is the whole
                   -3-word length deficit this park used to carry.  The
                   target lays BOTH arms out of line and reaches the second
                   through an explicit `j`, which an `if/else if` chain can
                   never produce: a C then-arm is always inline, so the
                   target's then-arm must consist of nothing but a jump.
                   `if (i < 4) goto rgb;` expands to jumpifnot + `j rgb`,
                   which jump.c's jump-around-jump rule folds to the
                   non-inverted `bnez $v0, rgb` at 0x8008A40C; `if (i >= 7)
                   goto armdone;` + `goto other;` folds the same way to the
                   `beqz -> join` / `j other` pair at 0x8008A418..0x8008A424.
                   Measured: the `if/else if` spelling is 318 words and 223
                   DIFF, this one 323 and 20. */
                if (i < 4)
                {
                    goto rgb;
                }
                if (i >= 7)
                {
                    goto armdone;
                }
                goto other;
                rgb:
                {
                    s32 *t;
                    s32 x;
                    s32 *base;

                    /* `c` FIRST, then the base in its own local.  The base
                       has to be a local so the `la` is emitted ahead of the
                       `sll` (0x8008A428 before 0x8008A430) -- with
                       `&D_8008B2F0[c]` the address is one rtl insn of equal
                       scheduling depth to the `sll` and loses the tie-break.
                       And `c` has to come first so dbr steals IT, not the
                       `la`, into the dispatch branch's delay slot at
                       0x8008A410.  Measured: `&D_8008B2F0[c]` is 23 DIFF,
                       base-before-c is 129. */
                    c = i - 1;
                    base = D_8008B2F0;
                    t = base + c;
                    x = *t + d;
                    *t = x;
                    if (x < 0)
                    {
                        *t = 0;
                    }
                    else if (x >= 0x101)
                    {
                        *t = 0x100;
                    }
                    if (c == 1)
                    {
                        func_80053DA8(0x516);
                    }
                    *(s32 *)&D_80080A98 = D_8008B2F0[0] < 0x100 ? D_8008B2F0[0] << 7 : 0x7FFF;
                    *(s32 *)&D_80080A9C = D_8008B2F0[1] < 0x100 ? D_8008B2F0[1] << 7 : 0x7FFF;
                    *(s32 *)&D_80080A94 = D_8008B2F0[2] < 0x100 ? D_8008B2F0[2] << 7 : 0x7FFF;
                    func_80053DF0(D_80080A98);
                    func_80053E14(D_80080A9C);
                    func_80053DCC(D_80080A94);
                }
                goto armdone;
                other:
                {
                    s32 x;

                    /* RE-READ p[6] here rather than use `i`.  The target
                       opens this arm with `lh $v0, 0xC($s0)` + nop
                       (0x8008A508/0x50C) and only then forms c; spelling it
                       `i - 4` lets the live `i` serve, dbr steals the
                       subtract into the dispatch delay slot and the arm
                       loses three words.  Measured: `i - 4` is 319 words,
                       173 DIFF. */
                    c = ((s16 *)p)[6] - 4;
                    x = ((u16 *)p)[c + 7] + d;
                    ((s16 *)p)[c + 7] = x;
                    if ((s16)x < 0)
                    {
                        ((s16 *)p)[c + 7] = 0;
                    }
                    else if ((s16)x >= 0x101)
                    {
                        ((s16 *)p)[c + 7] = 0x100;
                    }
                }
                armdone:
                ;
            }
        }
        if ((g->unk_08 & 9) == 9)
        {
            if (g->unk_10 & 2)
            {
                ((u16 *)p)[10] = (((u16 *)p)[10] + 1) & 3;
            }
        }
        if (g->unk_10 & 0x840)
        {
            s32 t;

            s32 r;
            s32 gg;
            s32 b;
            s32 u;
            u8 *q;
            u8 *q2;
            d = ((s16 *)p)[10] << 5;
            /* The `do { } while (0)` is a BLOCK BOUNDARY, not punctuation --
               docs/matching-levers.md:994, "a degenerate loop is a block
               boundary you can put anywhere". It is worth one word and three
               DIFF here (319/223 -> 320/220) by changing the loop notes, and
               therefore the reference weights, over the three colour
               components. decomp-permuter found it independently in 244,242
               candidates over 50 minutes (base=3230 -> best=2530) and that
               was its ENTIRE gain: reproducing this one construct by hand
               gives byte-identical output to its best candidate. */
            do
            {
                c = D_80013180 & 0xFF000000;
                r = ((s16 *)p)[7];
                t = 0xFF;
                if (r < 0x100)
                {
                    t = r;
                }
                /* THE GREEN COMPONENT IS AN IF/ELSE, not the
                   default-then-override that the red one is, and the
                   difference is a delay slot.  The target has `sll` in the
                   branch's slot at 0x8008A5DC with the `ori` after it; that
                   slot can only be filled from a THREAD (reorg.c's backward
                   scan refuses the `sll` either way, because the `slti`
                   between them writes the register one of them touches --
                   reorg.c:3086).  As an if/else the `sll` is the taken
                   thread's only insn, so dbr lifts it into the slot,
                   retargets the branch at the join and deletes the arm's
                   `j`, which is exactly the target's four words.  Written
                   as `b = gg << 8; if (gg >= 0x100) b = 0xFF00;` the slot
                   stays empty and the function is 324 words.

                   The ARMS ADD INTO `u` rather than assigning a `b` that a
                   later `t = t + b` consumes: the target's `addu $a0,$a1,$v0`
                   at 0x8008A5E4 writes a register `t` does not own, so the
                   sum is its own quantity.  Assigning `b` and adding
                   afterwards is 13 DIFF; this is 10. */
                gg = ((s16 *)p)[8];
                if (gg < 0x100)
                {
                    u = t + (gg << 8);
                }
                else
                {
                    u = t + 0xFF00;
                }
                b = ((s16 *)p)[9];
            } while (0);
            if (b < 0x100)
            {
                c = c + (u + (b << 16));
            }
            else
            {
                /* THE TEMPORARY IS A CROSS-JUMP BLOCKER.  Written
                   `c = (c + 0xFF0000) + t;` fold reassociates it to
                   `c + (t + 0xFF0000)`, which is the same tree as the
                   other arm's `c + (t + (b << 16))`, and jump.c
                   cross-jumps the shared two-insn tail away -- 320 words,
                   119 DIFF.  Splitting it over two statements keeps the
                   target's `addu $v0,$a2,$v0` / `addu $a2,$v0,$a0` at
                   0x8008A60C. */
                s32 w;

                w = c + 0xFF0000;
                c = w + u;
            }
            /* 0x80013180/84/86 ARE ONE OBJECT REACHED THROUGH A POINTER
               LOCAL, twice.  The target shares a single `lui` across all
               three offsets inside each of the two regions (0x8008A614 and
               0x8008A6A8 each serve 0x3180, 0x3184 and 0x3186 off $a0),
               which three separate `extern` scalars cannot do -- `%hi` of a
               different symbol is different rtl, so each access pays its own
               `lui`.  A base whose low half is zero costs ONE insn, so
               `q = (u8 *)0x80010000` + displacements gives the shared base;
               this is the ovl_town precedent at csrc/ovl_town.c:32817.  TWO
               locals, not one: the target re-materialises the base in the
               update region, so one local covering both is a word short
               (322 words, 81 DIFF).  The first `c` read and the closing
               `D_800814A4 = *(u8 *)0x80013186;` keep the SYMBOL spelling, because
               there the target's `lui` and its load share a register --
               the tie that says "scalar", see the azure-addressing-shapes
               note. */
            q = (u8 *)0x80010000;
            if (q[0x3184] != d)
            {
                D_80080A8A = 1;
            }
            if (*(u32 *)(q + 0x3180) != c)
            {
                D_80080A8A = 1;
            }
            if (((u16 *)p)[11] & 2)
            {
                if (q[0x3186] & 1)
                {
                    D_80080A8A = 1;
                }
            }
            else
            {
                if ((q[0x3186] & 1) == 0)
                {
                    D_80080A8A = 1;
                }
            }
            if (D_80080A8A == 1)
            {
                q2 = (u8 *)0x80010000;
                q2[0x3184] = d;
                D_800814AC = d;
                *(u32 *)(q2 + 0x3180) = c;
                D_80081494 = c;
                if (((u16 *)p)[11] & 2)
                {
                    q2[0x3186] &= 0xFE;
                }
                else
                {
                    q2[0x3186] |= 1;
                }
                D_800814A4 = *(u8 *)0x80013186;
            }
            ((u16 *)p)[11] |= 0x8000;
            ((s16 *)p)[4]++;
        }
        break;
    case 3:
        func_80053DA8(0x1200);
        ((s16 *)p)[1] = 0;
        ((s16 *)p)[2] = 0x10;
        ((s16 *)p)[4]++;
        /* fallthrough */
    case 4:
        if ((s16)func_80053604((Unk80083120 *)p) != 0)
        {
            func_80088BD0();
            ((u16 *)p)[-1] |= 0x8000;
            D_800814A0_a[0] |= 0x8000;
        }
        break;
    }
}

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
