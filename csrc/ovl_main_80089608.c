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

extern const RECT D_800888AC;
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

s32 func_80064584(s32);
s32 func_800644B8(s32);
void func_80089A14(s32 *p);
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


void func_80089608(s32 *p)
{
    /* r MUST be initialised here, ahead of the three tables, not assigned
       after them. The three initialisers are block copies out of rodata and
       each copy loop is its own basic block, so a statement after them puts
       the `lw` in the post-copy block; the target's `lw $t1, 0x0($s3)` sits
       in the ENTRY block, above .L80089644. Assigning r after the tables:
       260 words / 245 DIFF. */
    s16 *r = (s16 *)p[0];
    u8 *tbl1[10] = {
        &D_8008B074[0x00], &D_8008B074[0x78], &D_8008B074[0x54],
        &D_8008B074[0x60], &D_8008B074[0x6C], &D_8008B074[0xA8],
        &D_8008B074[0x9C], &D_8008B074[0x90], &D_8008B074[0xCC],
        &D_8008B074[0xC0],
    };
    s16 tbl2[10] = {100, 100, 60, 50, 20, -15, -40, -50, -80, -100};
    u32 tbl3[10] = {
        0xFFFFFF, 0xFF, 0xFF0000, 0xFF0000, 0xFF0000,
        0xFF, 0xFF, 0xFF, 0xFF00, 0xFF00,
    };
    s32 i;
    s32 *w;
    s32 *t;
    s16 *u;

    if (((u16 *)r)[19] & 1)
    {
        ((s16 *)p)[4] = 0xFF;
    }
    if (r[16] >= 2)
    {
        ((s16 *)p)[4] = 0xFF;
        return;
    }
    switch (((s16 *)p)[4])
    {
    case 0:
        p[1] += 0x20202;
        if (r[16] == 1)
        {
            ((u16 *)p)[4]++;
        }
        break;
    case 1:
        {
            s32 v;

            v = ++((s16 *)p)[5];
            if (v < 0x80)
            {
                /* RE-READ, not `v & 3`. v is the sign-extended value, so
                   `v & 3` masks the sll/sra result; re-reading the field
                   lets cse hand back the raw sum the `sh` stored, which is
                   the target's `andi $v0, $a0, 0x3`. It also keeps that raw
                   sum in its own register, which is what lets dbr sink the
                   `sh $a0, 0xA($s3)` into the branch's delay slot.
                   `v & 3`: 259 words / 7 DIFF. */
                if ((((u16 *)p)[5] & 3) == 0)
                {
                    p[1] += 0x10101;
                }
            }
            else if (v < 0x91)
            {
                /* The scratch IS the loop counter: the target holds this
                   value in $s2, which is i's register, and i is not live
                   here. A separate local lands in a temp instead.
                   And `i * 0x10101` is the wrong expansion -- synth_mult
                   picks ((i<<8)+i)<<8 + i, the target wants i + (i<<8) +
                   (i<<16) accumulated back into i. `p[1] += i * 0x10101`:
                   259 words / 14 DIFF. */
                i = (~p[1] & 0xFF) >> 1;
                i += (i << 8) + (i << 16);
                p[1] += i;
            }
            else if (v < 0xA0)
            {
                i = (p[1] & 0xFF) >> 3;
                i += (i << 8) + (i << 16);
                p[1] -= i;
            }
            else
            {
                ((s16 *)p)[4] = 0xFF;
                p[1] = 0;
            }
            ((u16 *)p)[7] -= 0xC;
        }
        break;
    case 0xFF:
        ((u16 *)p)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
        return;
    }
    for (i = 9; i >= 0; i--)
    {
        w = func_8003FC64(0x111);
        if (w != 0)
        {
            s32 x;
            s32 h;
            s32 l;

            w[4] = (s32)func_80089A14;
            t = (s32 *)w[3];
            /* w + 8 is the node's payload area, the same 0x20 offset
               func_8008A98C writes as w[8] and func_8008AE60 as
               *(Blk30 *)(w + 8). Three things about this line are
               load-bearing:
                 - it is a SEPARATE pointer, not `w += 8` at the end. With
                   the bump on w itself, w stays live to the last store and
                   global-alloc's priority (n_refs/live_length) puts t ahead
                   of it, so w and t swap $s0/$s1: 258 words / 54 DIFF.
                 - it is here, ABOVE the func_8004491C call, so u crosses
                   both calls and gets a saved register. Assigned after the
                   second scaling store instead: 259 words / 7 DIFF.
                 - it must not be folded into the use. `((s16 *)(w + 8))[6]
                   = i` makes gcc emit `sh $s2, 0x2C($s0)` and the target's
                   `addiu $s0, $s0, 0x20` disappears: 258 words / 67 DIFF. */
            u = (s16 *)(w + 8);
            func_8004491C((UnkNode *)w, (u32)func_80044BB0);
            /* h and l MUST be named locals. Written as one expression,
               `((x >> 12) - (x >> 13)) + 0xA0`, local-alloc's combine_regs
               (local-alloc.c:1350 iterates operands 1..n and ties the dest
               to the FIRST input that dies) ties the subu's result to the
               x>>12 operand, so the result and the `lw` of w[2] land in
               $v1/$v0 instead of the target's $v0/$v1 -- in both scaling
               groups. Inline expressions: 259 words / 8 DIFF. */
            x = func_80064584(((s16 *)p)[7]) * tbl2[i];
            h = x >> 12;
            l = x >> 13;
            ((s16 *)w[2])[1] = (h - l) + 0xA0;
            x = func_800644B8(((s16 *)p)[7]) * tbl2[i];
            h = x >> 12;
            l = x >> 14;
            ((s16 *)w[2])[3] = (h + l) + 0x78;
            ((s16 *)t)[15] = 0x1000;
            ((s16 *)t)[14] = 0x1000;
            if (i == 0)
            {
                ((s16 *)t)[13] = ((u16 *)p)[7];
            }
            else if (i == 1)
            {
                ((s16 *)t)[13] = -((u16 *)p)[7];
            }
            ((s16 *)t)[8] = 0x60;
            ((u16 *)t)[10] |= 0xC;
            t[2] = (s32)tbl1[i];
            ((u8 *)t)[4] = 0;
            ((u8 *)t)[5] = 0;
            t[3] = p[1] & tbl3[i];
            u[6] = i;
        }
    }
}

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
