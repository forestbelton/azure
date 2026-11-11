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

/* The widget physics state machine: five states on the s16 at +0x08, driving
   a 16.16 position at +0x00/+0x04 from a velocity at +0x0C/+0x10. Note that
   the word at +0x00 and the halfword at +0x02 OVERLAP -- +0x02 is the integer
   part of that fixed-point position, which is what case 3 reads.

   CASE 3's GUARD IS A CONDITIONAL EXPRESSION, NOT AN `if`, and that is the
   whole function. `v < 0 ? -v : v` folds to an ABS_EXPR and the MIPS backend
   emits abssi2, which is ONE insn whose own template prints
   `bgez / <nop> / subu` (config/mips/mips.md:1766, template at :1781, the
   nop being the `%#`). Written as `if (v < 0) { v = -v; }` the branch stays a
   real branch and the function sits at 13 DIFF: reorg then has a slot to fill
   and fills it, and the q[3] store never sinks into the `bnez`. Six
   scheduling-based attacks failed on this because there was never a delay
   slot to control -- the nop belongs to the instruction.
   `__builtin_abs(...)` also measures 0; `v = v < 0 ? -v : v;` as its own
   statement before the q[3] store does NOT -- it has to be inside the
   comparison.

   Case 1 is a cross-jumped if/else, not a delta: the STORE goes in each arm
   so jump.c merges the common `addu`/`sw` tail and leaves one `lui` per arm.
   Case 2 is an init and an override rather than an if/else. */
void func_80089A38(s32 *p, s32 *q, s32 arg2)
{
    s16 *r;

    r = (s16 *)p[0];
    func_800478B8(arg2);
    if (((u16 *)r)[19] & 1)
    {
        ((s16 *)p)[4] = 0xFF;
    }
    switch (((s16 *)p)[4])
    {
    case 0:
        break;
    case 1:
        {
            s32 d;

            q[0] += q[3];
            q[1] += q[4];
            d = q[3];
            if (((s16 *)p)[6] != 0)
            {
                q[3] = d - 0x10000;
            }
            else
            {
                q[3] = d + 0x10000;
            }
            q[4] += 0x8000;
            if (q[3] == 0)
            {
                ((s16 *)p)[4]++;
            }
        }
        break;
    case 2:
        {
            s32 v;

            v = 0xE8;
            if (((s16 *)p)[6] != 0)
            {
                v = 0x58;
            }
            ((s16 *)q)[1] = v;
            ((s16 *)q)[3] = r[18] * 16 + 0xA8;
            if (r[16] == 0x20)
            {
                s32 t;

                t = -0x20000;
                if (((s16 *)p)[6] != 0)
                {
                    t = 0x20000;
                }
                q[3] = t;
                q[4] = -0x40000;
                ((s16 *)p)[4]++;
            }
        }
        break;
    case 3:
        {
            s32 d;
            s32 v;

            q[0] += q[3];
            q[1] += q[4];
            d = q[3];
            q[3] = d + (d >> 3);
            v = ((s16 *)q)[1];
            if ((v < 0 ? -v : v) >= 0x169)
            {
                ((s16 *)p)[4] = 0xFF;
            }
        }
        break;
    case 0xFF:
        ((u16 *)p)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
        break;
    }
}

void func_80089C24(void)
{
    /* 0x18 bytes, not 8: only pos[0] and pos[1] are ever read (func_8008A98C
       takes a[0] and a[1], a 16.16 x/y pair), but the frame has b at sp+0x28
       and the only way to get the 0x10-byte gap above this block is for the
       block itself to be 0x18 wide. Every later displacement depends on it. */
    s32 pos[6];
    BlkB b;
    BlkA a;
    u16 buf[0x100];
    RECT r;
    s32 *w;
    s32 *node;
    s32 i;
    s32 *vol;
    s32 t0;
    s32 t1;
    s32 t2;

    node = NULL;
    r = D_800888AC;
    /* DEVICE: an empty `do { } while (0)`, worth 1 word and the whole
       function's alignment. Without it cse carries the block move's
       destination address forward and keeps `&r` alive in $s0 across the
       volume block, so the two StoreImage/LoadImage calls share one hoisted
       address plus a `move` instead of each recomputing `addiu $a0, $sp,
       0x280` -- 410 words, 388 differing. The label this emits is the cse
       extended-basic-block boundary that stops the equivalence. It emits no
       code and does not warn under -Wall. Legal spellings ruled out, all of
       them 410/388, are listed in the header. */
    do
    {
    } while (0);
    /* Before `D_80080A84 = 1`, which is 2 words: it decides whether the
       0x100 or the `lui %hi(D_80080A84)` is issued first. */
    t0 = 0x100;
    D_80080A84 = 1;
    /* The pointer-local hoist, worth 20 words. Assigned HERE, between the
       flag store and the first volume read: that is where the target
       materialises `lui %hi(D_8008B2F0)` and its `lo_sum`, so [0] reaches the
       array as a %lo displacement off the high register while [1] and [2] use
       the materialised base. Written as `D_8008B2F0[k] = tk` at each store
       the same five instructions come out in the wrong blocks (22 DIFF);
       assigned one statement earlier, 9 DIFF. */
    vol = D_8008B2F0;

    if (*(s32 *)&D_80080A98 != 0x7FFF)
    {
        t0 = *(s32 *)&D_80080A98 >> 7;
    }
    vol[0] = t0;
    t1 = 0x100;
    if (*(s32 *)&D_80080A9C != 0x7FFF)
    {
        t1 = *(s32 *)&D_80080A9C >> 7;
    }
    vol[1] = t1;
    t2 = 0x100;
    if (*(s32 *)&D_80080A94 != 0x7FFF)
    {
        t2 = *(s32 *)&D_80080A94 >> 7;
    }
    vol[2] = t2;

    func_80053DA8(0x200);
    func_8006733C(&r, buf);
    func_80067014(0);
    i = 0xFF;
    do
    {
        /* The STP bit over a 0x200-byte CLUT read back by func_8006733C
           (StoreImage) and written by func_800672D8 (LoadImage). THE s32
           TEMP IS LOAD-BEARING: `buf[i] |= ~0x7FFF` lets combine carry the
           `sh`'s 0xFFFF mask back onto the constant, which makes it fit
           `ori`'s immediate and costs the hoisted `addiu $a0, $zero,
           -0x8000` -- one word short. Through an s32 the ior stays SImode,
           0xFFFF8000 needs a register, and loop hoists it. */
        s32 v;

        v = buf[i] | ~0x7FFF;
        buf[i] = v;
    } while (--i >= 0);
    func_800672D8(&r, buf);

    w = func_8003FC64(0x11);
    if (w != NULL)
    {
        s16 *c;

        w[4] = (s32)func_8008A288;
        func_8004491C((UnkNode *)w, (u32)func_8008A794);
        node = w + 8;
        ((s16 *)w[2])[3] = 0xF0;
        ((s16 *)node)[2] = 0x20;
        ((s16 *)node)[10] = D_80013184[0] >> 5;
        ((s16 *)node)[7] = D_80013180[0];
        ((s16 *)node)[8] = D_80013181[0];
        ((s16 *)node)[9] = D_80013182[0];
        c = (s16 *)node;
        i = 2;
        do
        {
            if (c[7 + i] >= 0xFF)
            {
                c[7 + i] = 0x100;
            }
        } while (--i >= 0);
        /* Tested `== 0` so the `|= 2` arm is the FALLTHROUGH, as in the
           target; written the other way round the two arms swap places in
           memory and the `bnez` becomes a `beqz`. */
        if ((D_80013186[0] & 1) == 0)
        {
            ((u16 *)node)[11] |= 2;
        }
        else
        {
            ((u16 *)node)[11] &= 0xFFFD;
        }
    }
    func_80053428((Unk80083120 *)node);

    pos[0] = 0x400000;
    pos[1] = 0xA00000;
    func_8008A98C(pos, (s32 *)func_8008AC70, (s32)D_80078AF0, (s32)node);
    ((s16 *)pos)[1] = 0x94;
    ((s16 *)pos)[3] = 0x44;
    func_8008A98C(pos, (s32 *)func_8008ACB4, (s32)D_800786E8, (s32)node);

    /* This group is written in REVERSE field order, worth 4 words: the
       scheduler emits it in source order either way, and the target's order
       is 0x6C, 0x6A, 0x60, 0x5C, 0x50. The two fields the loop does not
       touch (0x66, 0x68) still come first. */
    a.unk_16 = 0xD0;
    a.unk_18 = 1;
    a.unk_1C = 0;
    a.unk_1A = 0x7C80;
    a.unk_10 = 0x808080;
    a.unk_0C = (s32)node;
    a.unk_00 = 0;
    i = 26;
    do
    {
        /* unk_14 BEFORE unk_02, worth 6 words: the order the two ramps are
           written in decides which of $s1/$s3 each strength-reduced
           induction variable gets, and the order of their two `sh`s. */
        a.unk_14 = i * 8 + 0x30;
        a.unk_04 = D_8008B2FC[i];
        a.unk_02 = i * 2 + 0x1E;
        func_8008AE60((s32 *)func_8008AEEC, (Blk30 *)&a);
    } while (--i >= 0);

    a.unk_14 = 0x74;
    a.unk_16 = 0xC;
    a.unk_04 = D_8008B2B0;
    func_8008AE60((s32 *)func_8008B030, (Blk30 *)&a);

    a.unk_14 = 0x20;
    for (i = 0; i < 2; i++)
    {
        a.unk_16 = i * 0xC + 0x20;
        a.unk_04 = D_8008B2B4[i];
        func_8008AE60((s32 *)func_8008B030, (Blk30 *)&a);
    }

    /* An in-place update, not a fresh constant: the target reads the field
       back with `lhu`, which is why unk_16 is u16. */
    a.unk_16 -= 6;
    for (i = 0; i < 2; i++)
    {
        a.unk_20 = 0;
        a.unk_24 = i;
        a.unk_14 = i * 0x40 + 0xBE;
        a.unk_04 = D_8008B2BC[i];
        func_8008AE60((s32 *)func_8008AD58, (Blk30 *)&a);
    }

    a.unk_14 = 0x20;
    i = 3;
    do
    {
        a.unk_16 = i * 0xC + 0x40;
        a.unk_04 = D_8008B2C4[i];
        func_8008AE60((s32 *)func_8008B030, (Blk30 *)&a);
    } while (--i >= 0);

    i = 1;
    do
    {
        a.unk_14 = i * 0x40 + 0xBE;
        a.unk_04 = D_8008B2D4[i];
        func_8008AE60((s32 *)func_8008B030, (Blk30 *)&a);
    } while (--i >= 0);

    a.unk_14 = 0x20;
    for (i = 0; i < 2; i++)
    {
        a.unk_16 = i * 0xC + 0x78;
        a.unk_04 = D_8008B2DC[i];
        func_8008AE60((s32 *)func_8008B030, (Blk30 *)&a);
    }

    a.unk_14 = 0xB2;
    i = 2;
    do
    {
        a.unk_16 = i * 0xC + 0x7E;
        a.unk_04 = D_8008B2E4[i];
        func_8008AE60((s32 *)func_8008B030, (Blk30 *)&a);
    } while (--i >= 0);

    /* The nine stores of this first Blk28 go in the TARGET'S order, which is
       neither field order nor its reverse (46 and 45 DIFF against 37): the
       0x1C that three of them share is materialised once and its last two
       uses sit at the end, after the two stores that need no constant.
       Same rule as func_8008896C's halfword grouping in this unit. */
    b.unk_0C = 0x1C;
    b.unk_10 = 0x100;
    b.unk_14 = 2;
    b.unk_16 = 1;
    b.unk_08 = 0x202020;
    b.unk_00 = 0;
    b.unk_04 = (s32)node;
    b.unk_12 = 0x1C;
    b.unk_0E = 0x1C;
    func_8008ABD4((s32 *)func_8008AA40, (Blk28 *)&b);

    /* unk_12 before unk_0E on these two: unk_0E's `sh` is the one the target
       puts in the `jal`'s delay slot, so it is written last. */
    b.unk_12 = 0x34;
    b.unk_0E = 0x3C;
    func_8008ABD4((s32 *)func_8008AA40, (Blk28 *)&b);

    b.unk_12 = 0x2E;
    b.unk_0E = 0x74;
    func_8008ABD4((s32 *)func_8008AA40, (Blk28 *)&b);

    b.unk_0C = 0x20;
    b.unk_0E = 0xA6;
    b.unk_10 = 0x40;
    b.unk_12 = 0x18;
    func_8008ABD4((s32 *)func_8008AB44, (Blk28 *)&b);

    b.unk_0C = 0xCA;
    b.unk_10 = 0x40;
    b.unk_12 = 8;
    b.unk_16 = 0;
    /* The loop COUNTER is the block's own +0x18 field -- which is what the
       handlers read as p[6] -- so each iteration reloads and restores it.
       That is the `lw`/`sw 0x40($sp)` pair around every call, not a spill. */
    b.unk_18 = 2;
    do
    {
        b.unk_08 = 0xFF << (b.unk_18 * 8);
        b.unk_0E = b.unk_18 * 12 + 0x7E;
        func_8008ABD4((s32 *)func_8008AA84, (Blk28 *)&b);
    } while (--b.unk_18 >= 0);

    /* unk_12 before unk_08, worth 4 words: it decides that the `8` is
       materialised into its own register before the 0xFFFFFF store. */
    b.unk_12 = 8;
    b.unk_08 = 0xFFFFFF;
    b.unk_14 = 0;
    b.unk_16 = 0;
    b.unk_18 = 2;
    do
    {
        b.unk_0E = (b.unk_18 + 1) * 12 + 0x40;
        func_8008ABD4((s32 *)func_8008AAE4, (Blk28 *)&b);
    } while (--b.unk_18 >= 0);
}

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
