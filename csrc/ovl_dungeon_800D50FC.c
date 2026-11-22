#include "common.h"
#include "ovl_dungeon_decls.h"

void func_800D92C0(u8 *p, void *b, u8 *c, u8 *d);

/* dungeon.bin's overlay, translation unit 4 of 4: func_800D50FC to the next
 * boundary.  THE SPLIT IS FORCED BY JUMP-TABLE ALIGNMENT, not by size --
 * see splat/dungeon.bin.yaml for the offsets and why each one is where it
 * is.  The prototypes below are what this unit used to get for free by
 * being in the same file as the definitions; they are generated from those
 * definition lines, so they say exactly what the single-unit compile said.
 */

void func_80093E74(u8 *p, void *b, void *c, u8 *d);
void func_800999D4(u8 *p);
s32 func_8009A180(u8 *a, u8 *b);
s32 func_8009A66C(u32 f, u8 *c, u8 *d, s16 y);
s32 func_8009B164(u16 a, UnkVec *b, u8 *c);
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
void func_800A020C(s32 f, u8 *d);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
s32 func_800A2B5C(void *d);
s16 func_800A45D8(u16 x, u16 y, s32 c);
s16 func_800A48F0(s8 *p, s8 kind, s8 c);
void func_800A9A04(u8 *p);
s32 func_800A9E70(UnkDungeonActor *p, void *b, u8 *c, u8 *d);
s32 func_800AA36C(u8 *p, void *b, u8 *c, u8 *d);
s32 func_800AA79C(UnkDungeonActor *p, void *b, u8 *c, u8 *d);
void func_800AA888(u8 *p, void *b, u8 *c, u8 *d);
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
void func_800B274C(u8 *p, UnkVec *b, u8 *c);
s32 func_800BA384(u8 *p, UnkVec *v, UnkSlot *s);
void func_800CAFDC(UnkNode *a, s16 b, s32 col);
void func_800D21CC(UnkDungeonActor *a, UnkVec *b, u8 *c, u8 *d);
void func_800D29D8(UnkDungeonActor *a, UnkVec *b, u8 *c, u8 *d);
void func_800D45D0(UnkDungeonActor *p, void *b, void *c, void *d);
void func_800D4BD4(UnkNode *n, u8 *b, u8 *c, u8 *d);
void func_800D4CB0(u8 *p, UnkVec *b, u8 *c);

/* func_800D50FC -- **102 of 102 words, 0 DIFFERING**, 2026-11-20.

   This function owns `jtbl_80089454`, which sits at rodata offset 0xCF4 --
   4 mod 8 -- and `cc1psx` writes `.align 3` before every table it compiles.
   Compiled inside one big ovl_dungeon.c the table therefore gains four bytes
   of padding the original build does not have, `.rodata` grows and
   `build/dungeon.bin` comes out eight bytes long.  See
   docs/dungeon-sweep.md, "SEVEN of the 28 jump tables are at 4 mod 8", and
   the worked carve in splat/town.bin.yaml.

   **So this body is correct and complete and must not be merged until the
   unit is carved in the same change.**  It is parked here so the carve is a
   purely structural job with no decompilation left in it.

   The body came out of `tools/m2c` plus retyping, at 0 differing on the
   FIRST compile -- which is the same thing that happened on 2026-11-15 when
   a batch was handed this function by mistake.  Nothing was fought for:

     * the switch is on `*(u16 *)(p + 0x1E) & 7` with the eight table entries
       grouping as 0/4 -> the random-velocity arm, 3/7 -> the zero arm, and
       1/2/5/6 -> an empty arm that falls through to the shared tail.  The
       empty `case` labels have to be written out: without them the compiler
       builds a different table.
     * the three `+=` statements on b+0x0/+0x4/+0x8 are each their own
       statement; the target's four loads bunched ahead of the three adds are
       sched1's doing, not the source's.
     * `n` is `s16` and the test is `n <= 0`, which is what gives
       `sll $v0, $v0, 16` + `bgtz` rather than a compare.

   DECLARATIONS
   s32 func_80069EF8();
   END DECLARATIONS
   (D_800814A0 is already declared.  The unit also already carries
   `void func_800D50FC();` at the top, which C89 makes compatible with this
   three-parameter definition, and the only use of the name is
   `n->unk_10 = (u32)func_800D50FC;` -- an address, so the arity does not
   matter to it.)

*/
void func_800D50FC(u8 *p, u8 *b, u8 *c)
{
    s32 i;
    s16 n;

    i = *(u16 *)(p + 0x1E) & 7;
    switch (i)
    {
    case 1:
    case 2:
    case 5:
    case 6:
        break;
    case 0:
    case 4:
        *(s32 *)(b + 0xC) = ((func_80069EF8() & 0x3FFF) - 0x2000) << 5;
        *(s32 *)(b + 0x10) = ((func_80069EF8() & 0x3FFF) - 0x2000) << 5;
        break;
    case 3:
    case 7:
        *(s32 *)(b + 0x10) = 0;
        *(s32 *)(b + 0xC) = 0;
        break;
    }
    *(s32 *)(b + 0x0) += *(s32 *)(b + 0xC);
    *(s32 *)(b + 0x4) += *(s32 *)(b + 0x10);
    *(s32 *)(b + 0x8) += *(s32 *)(b + 0x14);
    c[0xC] = p[0xC] * *(s16 *)(p + 0x1E) / *(s16 *)(p + 0x20);
    c[0xD] = p[0xD] * *(s16 *)(p + 0x1E) / *(s16 *)(p + 0x20);
    c[0xE] = p[0xE] * *(s16 *)(p + 0x1E) / *(s16 *)(p + 0x20);
    n = *(u16 *)(p + 0x1E) - 1;
    *(u16 *)(p + 0x1E) = n;
    if (n <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The trail step of func_800D5460's effect node: carry the owner's mover
   plus the view's +0x3C..+0x40 offset into this node's mover, and for the
   first 0x14 frames spawn four 0x212 sparks jittered around it. The tail is
   func_800D4CB0's countdown-and-kill with D_80083460's live count dropped.
   115 words, 0 differing.

   THE JITTER'S CONSTANT MUST BE PARENTHESISED ONTO THE RANDOM OPERAND.
   `*(u16 *)(m + 2) + (rand() & 0x3F) - 0x1F` is 115 words at 4
   differing: the `addu` comes out before the `addiu -0x1F` because fold
   moves the additive constant onto the memory operand. Written
   `+ ((rand() & 0x3F) - 0x1F)` the swap goes the other way and the
   order is the target's -- docs/matching-levers.md :19632, "write the
   constant on the operand the listing does NOT have it on".
   `(x - 0x1F) + rand` and `x - 0x1F + rand` are 118 words / 115 differing,
   because the subtraction is then evaluated ahead of the call.

   func_800D4BD4 TAKES FOUR PARAMETERS, NOT THREE. This call sets $a3 in the
   jal's own delay slot (`addu $a3, $s6, $zero`), and the fourth argument is
   this handler's third parameter. The landed definition at func_800D4BD4
   declares three and never reads a fourth, so it had to be widened; it is
   still 55 words at 0 differing with the extra parameter, and it has no
   other caller in the unit. */
void func_800D5294(u8 *p, UnkVec *b, u8 *c)
{
    UnkNode *q;
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *g;
    s32 i;
    s16 t;

    q = *(UnkNode **)(p + 0x24);
    *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)q->unk_08 + 0x2) + *(u16 *)(p + 0x3C);
    *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)q->unk_08 + 0x6) + *(u16 *)(p + 0x3E);
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)q->unk_08 + 0xA) + *(u16 *)(p + 0x40);
    t = *(u16 *)(p + 0x16) + 1;
    *(s16 *)(p + 0x16) = t;
    if (t < 0x14)
    {
        for (i = 0; i < 4; i++)
        {
            n = func_8003FC64(0x212);
            if (n != NULL)
            {
                func_800D4BD4(n, p, (u8 *)b, c);
                r = (u8 *)n + 0x20;
                *(s16 *)(r + 0x1E) = 0x20;
                *(s16 *)(r + 0x20) = 0x20;
                n->unk_10 = (u32)func_800D50FC;
                m = n->unk_08;
                *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)m + 0x2) + ((rand() & 0x3F) - 0x1F);
                *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)m + 0x6) + ((rand() & 0x3F) - 0x1F);
                *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)m + 0xA) - (rand() & 0x1F);
                *(s32 *)((u8 *)m + 0x14) = -((u16)rand() + 0x10000) << 1;
            }
        }
    }
    t = *(u16 *)(p + 0x1E) - 1;
    *(s16 *)(p + 0x1E) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
    }
}

/* func_800D4FC8 with func_800D5294 and a 0x32 life. 77 words, 0 differing. */
void func_800D5460(UnkNode *src, s32 colour, u16 snd)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;
    u8 *g;

    /* `&colour` IS LOAD-BEARING and costs no code. The target keeps this
       parameter in its incoming home slot at 0x24($sp) -- stored once in the
       jal's delay slot, reloaded once -- and assign_parms (function.c:3775)
       gives a register parameter a MEM for its DECL_RTL at -O2 in exactly
       three cases: TREE_ADDRESSABLE, TREE_SIDE_EFFECTS, or a BLKmode type.
       A struct is ruled out by the call sites, which pass integer constants
       with `lui`/`ori`, so the original took its address. Without this the
       parameter takes a third callee-saved register and pays the
       save/restore pair: 78 words. `volatile` reaches 77 with 3 differing --
       the prologue permuted -- and the address-take reaches 0. */
    (void)&colour;
    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        *(UnkNode **)(p + 0x24) = src;
        n->unk_10 = (u32)func_800D5294;
        *(s16 *)(p + 0x1E) = 0x32;
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_14 |= 0xC;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)src->unk_08 + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)src->unk_08 + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)src->unk_08 + 0xA);
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        *(s32 *)(p + 0xC) = *(s32 *)((u8 *)r + 0xC) = colour;
        r->unk_12 = 0x7DCE;
        r->unk_14 |= 0x100;
        func_8003DB94(r, D_800DECF8, 0);
        if (snd != 0)
        {
            func_800A56E0(snd);
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
}

/* The party view's per-frame refresh: push the record's +0x1C flags through
   func_800A020C, re-send the renderable's facing cell whenever either the
   record's or the engine's angle moved, copy four halfword pairs out of the
   record and its +0x08 block, and finally raise the dirty bit -- with a
   -1 on the record's +0xB0 for the nine tables that are its own.

   FOUR SPELLINGS WERE EACH WORTH WORDS, all measured:
   * `w = q + 0x20;` A SECOND TIME, used only by the `w[0xB0] -= 1;` in the
     table arm. The target spills that pointer to 0x10($sp) at its definition
     and reloads it into $a3 at the one use, which is a TENTH long-lived
     value and is what makes the frame 0x40 rather than 0x38 -- with the one
     `e` the frame is eight bytes shorter and all 24 save/restore
     displacements differ: 182 words, 28 differing.
   * `s16 k` for the facing cell. The `s32` form is one word short and has no
     `addu $v1, $s0, $zero`; the s16's widening copy at the compare is that
     word, exactly as func_80093E74's note records. `u16` and `u8` are 28 and
     29 differing, `s8` is two words long.
   * `* 4`, not `<< 2`, on the D_800DCECC byte: `<< 2` shortens into
     `lbu`/`sll 24`/`sra 22` where the target has `lb`/`sll 2`.
   * `s16 n` for the record's +0x2A angle. As `s32` the two `lh`s come out
     in the opposite order (3 differing); binding BOTH angles to locals, or
     moving `n` inside the arm, are the other two spellings that reach 0.

   182 words, 0 differing. */
void func_800D5594(u8 *a, u8 *b, u8 *c)
{
    u8 *q;
    u8 *e;
    u8 *r;
    u8 *v;
    u8 *g;
    u8 *t;
    u8 *w;
    s16 k;
    s16 n;

    q = *(u8 **)(a + 0xAC);
    e = q + 0x20;
    w = q + 0x20;
    r = *(u8 **)(q + 0xC);
    v = *(u8 **)(q + 0x8);
    func_800A020C(*(s32 *)(e + 0x1C), c + 0xC);
    if ((*(u32 *)(e + 0x1C) & 0x20) == 0)
    {
        func_800478B8(c);
    }
    if (*(u8 **)(r + 0x2C) == D_800E2400)
    {
        func_800478B8(c);
        func_800478B8(c);
    }
    n = *(s16 *)(e + 0x2A);
    if (*(s16 *)(a + 0x2A) != n)
    {
        *(u16 *)(a + 0x2A) = *(u16 *)(e + 0x2A);
        func_80047784(c, (*(u8 **)(c + 0x2C))[((D_80083228 + n + 0x100) >> 9) & 7], 0);
    }
    g = (u8 *)&D_80083160;
    k = ((*(s16 *)(g + 0xC8) + *(s16 *)(a + 0x2A) + 0x100) >> 9) & 7;
    if (*(s16 *)(a + 0x94) != k)
    {
        func_80047784(c, (*(u8 **)(c + 0x2C))[k], 0);
        *(s16 *)(a + 0x94) = k;
    }
    *(u16 *)(c + 0x14) = *(u16 *)(r + 0x14);
    *(u16 *)(b + 0x2) = *(u16 *)(v + 0x2);
    *(u16 *)(b + 0x6) = *(u16 *)(v + 0x6);
    *(u16 *)(b + 0xA) = *(u16 *)(v + 0xA);
    *(s16 *)(c + 0x6) = D_800DCECC[((*(s16 *)(g + 0xC8) + *(s16 *)(a + 0x2A) + 0x100) >> 9) & 7] * 4;
    *(u16 *)(c + 0x1C) = *(u16 *)(r + 0x1C);
    *(u16 *)(c + 0x1E) = *(u16 *)(r + 0x1E);
    *(u16 *)(c + 0x12) = *(u16 *)(r + 0x12);
    if ((*(u16 *)(q + 0x1E) & 0x8000) == 0)
    {
        t = *(u8 **)(r + 0x2C);
        if (t == D_800E23F8 || t == D_800E2408 || t == D_800E2420 || t == D_800E2428 || t == D_800E2450 ||
            t == D_800E2458 || t == D_800E2460 || t == D_800E2440 || t == D_800E2438)
        {
            *(u16 *)(a - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            w[0xB0] -= 1;
            return;
        }
        if ((s16)func_80042900((s8 *)e, 0xA) == 0)
        {
            return;
        }
    }
    *(u16 *)(a - 0x2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

/* A node's +0x10 handler, so three parameters off func_80040190's cast. The
   masked value is stored unconditionally -- the `sh` sits in the `beqz`'s
   delay slot -- which is the `&=` followed by a test of the same field.
   27 words, 0 differing. */
void func_800D586C(u8 *p, UnkVec *m, u8 *r)
{
    func_800478B8(r);
    *(u16 *)(r + 0x14) &= 0x6000;
    if (*(u16 *)(r + 0x14) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* One frame of the 0x12 spark emitter: re-point the renderable's sprite
   whenever the eight-way facing changes, step it, and on the +0x96 countdown
   expiring throw a 0x12 node at the mover's position offset by the slot
   projection func_8003DE58 computes, give it func_800D586C as its step, and
   retire this one.

   `k` IS `s16`, AND THAT IS THE WHOLE WORD. The eight-way index is used in
   three places -- the +0x94 compare, the sprite-table subscript and the
   +0x94 store -- and as `s32` the compare and the subscript read $s0
   directly: 121 words, 106 differing (one short, everything after the
   missing word shifted). As `s16` each INT use pays a sign extension that
   `nonzero_bits` then deletes because of the `& 7`, and what is left is the
   target's `addu $v1, $s0, $zero`, shared by the compare and the subscript.
   An explicit `(s16)k` in the test alone does NOT buy it -- still 121 / 106.

   The spawn block is func_800D4BD4's shape: `n->unk_0C` is READ TWICE,
   because the halfword stores through the vector pointer wipe cse's memory
   table. 122 words, 0 differing. */
void func_800D58D8(u8 *p, UnkVec *b, u8 *c)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *e;
    u8 *r;
    u8 *t;
    s16 k;
    s16 out[4];

    k = ((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7;
    if (*(s16 *)(p + 0x94) != k)
    {
        t = *(u8 **)(c + 0x2C);
        if (t != NULL)
        {
            func_80047738(c, t[k], *(s8 *)(c + 0x4));
        }
        *(s16 *)(p + 0x94) = k;
    }
    func_800478B8(c);
    if (--*(s16 *)(p + 0x96) <= 0)
    {
        n = func_8003FC64(0x12);
        if (n != NULL)
        {
            func_8004491C(n, (u32)D_80045340);
            r = (u8 *)n->unk_0C;
            *(s16 *)(r + 0x10) = 0x20;
            *(u16 *)(r + 0x14) |= 0xC;
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)b + 0x2);
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)b + 0x6);
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)b + 0xA);
            r = (u8 *)n->unk_0C;
            *(s16 *)(r + 0x1E) = 0x1800;
            *(s16 *)(r + 0x1C) = 0x1800;
            r[0xE] = 0x80;
            r[0xD] = 0x80;
            r[0xC] = 0x80;
            *(s16 *)(r + 0x6) = *(u16 *)(c + 0x6);
            n->unk_10 = (u32)func_800D586C;
            e = *(UnkSlot **)(p - 0x14);
            if (func_8003DE58((u8 *)e->unk_08, e, out, 0) != NULL)
            {
                *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)m + 0x2) + out[0];
                *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)m + 0x6) + out[1];
                *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)m + 0xA) + out[2];
            }
            func_8003DB94((u8 *)n->unk_0C, D_800DE870, 0);
        }
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* One semi-transparent 1x1 tile and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad. func_800D68F4 is this
   function again, byte for byte. The family is ovl_town's func_800BB218 --
   this walk with a 0x180-iteration inner loop and a constant colour -- and
   csrc/ovl_town_800BF3F0.c's func_800BFFF4 is the LINE_G2 version, which
   carries the long note on the shape.

   THE OT LINKS ARE `addPrim`, i.e. bitfield stores through P_TAG's
   `addr:24`; a hand-written `|` cannot produce the
   `lw`/`and 0xFF000000`/`and 0xFFFFFF`/`or`/`sw` quintets, and
   csrc/unk_19434.c:92-99 says why. The two masks are loop-hoisted into
   $s2/$s4.

   THE THREE DEAD `lbu`s AT +0x4/+0x5/+0x6 ARE `setRGB0(t, t->r0, t->g0,
   t->b0)` -- an echo of the colour word just copied in. cse deletes the
   three STORES as redundant, because it knows the word that was written,
   and the three loads survive tightly packed where the target has them.
   They are NOT bare `volatile` reads: that spelling does keep the loads,
   but gcc 2.7.2's `read_dependence` pairs volatile only with volatile
   (scratch/gcc-src/cdk-gcc-b18/sched.c:822), so they take priority 0, drift
   to wherever the ready list puts them, and let the first addPrim's
   `lw 0x0($s0)` hoist to the top of the block -- 120 words and 36
   differing, the right length and the wrong function. Omitting them is 117.

   `setTile1` plus `setSemiTrans(t, 1)` is ONE `sb 0x6A`: cse folds the
   `getcode` back to the 0x68 just stored and drops the dead first store.
   `setlen(t, 2)` plus `setcode(t, 0x6A)` is identical.

   Three orderings are load-bearing. The colour word is copied BEFORE the tag
   setup (6 differing the other way: with `setlen` first, $v0 carries both
   the constant and the loaded word and the `lw 0x8($s3)` cannot issue
   first). `sp->unk_1C` is written BEFORE `sp->unk_24` -- 20 differing
   swapped, and the whole $s3/$s4 split between `p` and the 0xFF000000 mask
   turns over -- even though the target STORES them in the other order. And
   `p = (u8 *)n + 0x20;` comes before `v = n->unk_08;`: 4 differing swapped.

   120 words, 0 differing. */
s32 func_800D5AC0(u8 *p, UnkVec *v)
{
    UnkDungeonSP *sp = (UnkDungeonSP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    TILE_1 *t;
    DR_MODE *dm;

    sp->unk_1C = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_24 = D_80083160.unk_00 + 0xB0;
    while (1)
    {
        sp->unk_04 = *(u16 *)((u8 *)v + 0x2);
        sp->unk_06 = *(u16 *)((u8 *)v + 0x6);
        sp->unk_08 = *(u16 *)((u8 *)v + 0xA);
        t = (TILE_1 *)sp->unk_1C;
        sp->unk_1C = (u8 *)(t + 1);
        sp->unk_100 = RotTransPers((SVECTOR *)&sp->unk_04, (s32 *)&t->x0, &sp->unk_D0, &sp->unk_D4);
        if ((u32)sp->unk_100 < 0x1E0)
        {
            *(u32 *)&t->r0 = *(u32 *)(p + 0x8);
            setTile1(t);
            setSemiTrans(t, 1);
            setRGB0(t, t->r0, t->g0, t->b0);
            addPrim(sp->unk_24 + sp->unk_100 * 4, t);
            dm = (DR_MODE *)sp->unk_1C;
            sp->unk_1C = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            addPrim(sp->unk_24 + sp->unk_100 * 4, dm);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_1C;
    return 0;
}

/* One frame of a fading burst, and a leaf: integrate the mover, halve each
   of its three velocities, scale the three colour bytes by the remaining
   life over the original life, copy the colour word forward, and when the
   life runs out raise the node's and the engine's 0x8000 flags. The three
   `srl 31`/`addu`/`sra 1` triples are a signed divide by two, which is
   what `/= 2` gives; `>>= 1` would drop the correction.
   75 words, 0 differing. */
void func_800D5CA0(u8 *p, UnkVec *v)
{
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    v->unk_0C /= 2;
    v->unk_10 /= 2;
    v->unk_14 /= 2;
    p[4] = p[0] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    p[5] = p[1] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    p[6] = p[2] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    *(s32 *)(p + 0x8) = *(s32 *)(p + 0x4);
    if (--*(s16 *)(p + 0x32) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x211 node func_800D5CA0 steps off the parent node `a`: copy the
   parent mover's three integer halves with the z lifted 0x14, nudge x and y by
   the D_800E2468 row the angle `b` selects, give the mover a random velocity on
   all three axes and a constant drift along the same row, then park
   func_800D5AC0 as the step and hang `c` off the view at +0x00 and +0x08.

   `n->unk_08` IS RE-DEREFERENCED AT EVERY STATEMENT, not cached -- the eight
   `lw 0x8($s1)` are the source, the same way func_800CAFDC above re-reads
   `a->unk_08`.

   THE ROW ENTRIES ARE `* 16`, NOT `<< 4`. Between a sign-extended load and a
   narrowing store a `<< k` turns `lh` into `lhu`; `* (1 << k)` does not, and
   that is the only difference between 1 differing and 0.

   `b` IS NARROW AND THAT IS WHAT MAKES THE SHIFT LOGICAL. As `s32` the index
   comes out `sra $s0, $s2, 7` against the target's `srl` (1 differing); `s16`,
   `u32`, and `s32` with an explicit `(u32)` cast are all 0, and `u16` is 112
   words / 107 differing. `s16` is taken because the one place `b` is stored is
   the `s16` at the view's +0x14; the function has no caller anywhere, in C or
   in asm, so nothing witnesses it. 109 words, 0 differing. */
void func_800D5DCC(UnkNode *a, s16 b, u8 *c)
{
    UnkNode *n;
    s16 *t;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800D5CA0;
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2);
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6);
        *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA) - 0x14;
        t = D_800E2468[(b >> 9) & 7];
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)n->unk_08 + 0x2) + (t[0] * 16);
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)n->unk_08 + 0x6) + (t[1] * 16);
        *(s16 *)((u8 *)n->unk_0C + 0x6) = 6;
        *(s32 *)((u8 *)n->unk_08 + 0xC) = ((rand() & 0x7FFF) - 0x4000) << 6;
        *(s32 *)((u8 *)n->unk_08 + 0x10) = ((rand() & 0x7FFF) - 0x4000) << 6;
        *(s32 *)((u8 *)n->unk_08 + 0x14) = ((rand() & 0x7FFF) - 0x4000) << 5;
        *(s32 *)((u8 *)n->unk_08 + 0xC) += t[0] * 0x160000;
        *(s32 *)((u8 *)n->unk_08 + 0x10) += t[1] * 0x160000;
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = 7;
        *(s16 *)(p + 0x34) = 7;
        func_8004491C(n, (u32)func_800D5AC0);
        *(u8 **)((u8 *)n + 0x20) = c;
        *(u8 **)(p + 0x8) = c;
    }
}

/* Is the tile one step along the record's facing blocked? The direction
   decodes out of the mover's +0x2A exactly as func_8009B164 decodes it, and
   D_800E2468 is the interleaved (dx, dz) table for the eight directions.
   58 words, 0 differing.

   THREE SPELLINGS, ALL MEASURED, AND EACH ONE IS WORTH REGISTERS RATHER
   THAN WORDS -- the function is 58 words under every spelling below.
   (1) THE TWO SUMS ARE `s32` LOCALS AND THE TRUNCATION IS AN EXPLICIT
   `& 0xFFFF` INTO AN `s32`. Through `u16 x`/`u16 y` the HImode pseudo
   outranks nothing: the row pointer wins `$v1`, the x sum is pushed to
   `$a0`, and 17 words differ with every opcode already right. The mask form
   gives the row pointer `$a1` -- the incoming `b` register -- and the sums
   `$v1`, which is the target. Writing the sums straight into the u16 locals
   is the same 17.
   (2) `t[0] * 64`, NOT `t[0] << 6`. The shift lets the final mask propagate
   back through force_to_mode and the table reads come out `lhu` where the
   target has `lh`; `* 64` is a MULT, which force_to_mode does not narrow.
   Four loads change, no word count.
   (3) THE TABLE BASE IS ITS OWN STATEMENT, ahead of the index. Inline --
   `D_800E2468[(... >> 9) & 7]` -- fold puts the ADDR_EXPR second, the
   `srl`/`andi` are emitted before the `lui`/`addiu`, and that is 4 words.
   And the tail is TWO `if`s with a `||`, not three nested tests: nested,
   `r` takes `$a0` and the reloaded +0xA takes `$v1` instead of the other
   way round, 6 differing. */
s32 func_800D5F80(u8 *p, u8 *b)
{
    s16 *t;
    s16(*tb)[2];
    s32 k;
    s32 u;
    s32 u0;
    s32 u1;
    s16 c;
    s32 x;
    s32 y;
    s16 r;

    u = *(s16 *)(p + 0xA) - 0x20;
    c = u;
    tb = D_800E2468;
    k = (*(u16 *)(b + 0x2A) >> 9) & 7;
    t = tb[k];
    u0 = *(s16 *)(p + 0x2) + t[0] * 64;
    x = u0 & 0xFFFF;
    u1 = *(s16 *)(p + 0x6) + t[1] * 64;
    y = u1 & 0xFFFF;
    if (func_800A45D8(x, y, c) != 0)
    {
        return 1;
    }
    r = (s16)func_800BCB04(x, y, c);
    if (r >= 0x200 || *(s16 *)(p + 0xA) - 0x40 >= r)
    {
        return 1;
    }
    return 0;
}

/* State 0x18. The two `(s16)` casts are the whole shape of the tests:
   func_800A2B5C is declared s32 and func_800A48F0 s16, and every test here is
   on the low halfword. 66 words, 0 differing. */
void func_800D6068(u8 *p, void *b, u8 *c, u8 *d)
{
    d[0x71] &= 0x7F;
    if ((D_80083462 & 0x2000) == 0)
    {
        if ((s16)func_800A2B5C(d) == 0)
        {
            if ((s16)func_800A2B5C(d) == 0)
            {
                if ((s16)func_800A48F0((s8 *)d, 0x1B, 0) < 0)
                {
                    *(u16 *)(d + 0x46) &= 0x7FFF;
                }
                else
                {
                    *(s32 *)(p + 0x8C) = 0;
                    p[0x9A] = 0x18;
                    p[0x9B] = 0;
                    *(s16 *)(p + 0x96) = 0;
                    p[0xB3] = 0;
                    p[0xB1] = 0;
                    p[0xB2] = 0;
                    *(s32 *)(d + 0x1C) &= ~0x40000;
                    *(u16 *)(p + 0x98) |= 8;
                    func_80042B68((s8 *)d, 5);
                    func_800A4ACC(d);
                    d[0x6D]--;
                }
            }
        }
    }
}

/* The ladder/rope strip: compose the slot's rotation with the camera's own
   matrix in the 0x1F800000 scratchpad, then walk the strip's far end back
   towards its near end 0x10 units at a time, RotAverage4-ing a POLY_F4 per
   rung into the +0x8D0 cursor, and close with the one short segment that is
   left over.  func_800BA414 above is the same emit block with a different
   quad; the four on-screen tests are `|`, not `||`, for the reason its note
   gives, and the two OT links are `addPrim` -- P_TAG bitfield stores, which
   a hand-written `|` cannot produce.

   THREE SPELLINGS ARE LOAD-BEARING, and together they are the whole landing
   (399 words / 32 aligned differences before them, 400 / 0 after).

   THE SCALE VECTOR IS ONE CHAINED ASSIGNMENT.  `sp->unk_34.vx = sp->unk_34.vy
   = sp->unk_34.vz = 0x2000;` emits +0x3C, +0x38, +0x34, which is the target's
   order; three separate statements emit +0x34, +0x38, +0x3C and the stores
   do not realign.  Same rule as func_800BA384's note.

   `q[4]` IS DECLARED BEFORE THE MATRIX.  The target's frame puts the four
   halfwords at $sp+0x28 and ReadRotMatrix's MATRIX at $sp+0x30; declaring
   the MATRIX first puts it at +0x28 and `q` at +0x48, and every one of the
   eleven `q` accesses then reads the wrong displacement.

   `&m` GOES THROUGH A POINTER LOCAL, and that is the 400th word.  Written
   `ReadRotMatrix(&m); CompMatrix(&m, ...)` the address is two separate
   `addiu $a0,$sp,0x30`, which is one word SHORT; `MATRIX *mp = &m;` makes it
   one pseudo live across the call, so it takes $s0 and the two call sites
   become `addu $a0,$s0,$zero`.  400 words, 0 differing. */
void func_800D6170(u8 *p, UnkVec *v, UnkSlot *s, s16 d)
{
    UnkDungeonSP6 *sp = (UnkDungeonSP6 *)0x1F800000;
    Unk80083160 *g;
    s16 q[4];
    MATRIX m;
    MATRIX *mp;
    u8 *t;
    s32 i;

    g = &D_80083160;
    sp->unk_24 = g->unk_00 + 0xB0;
    sp->unk_74.t[0] = *(s16 *)((u8 *)v + 0x2);
    sp->unk_74.t[1] = *(s16 *)((u8 *)v + 0x6);
    sp->unk_74.t[2] = *(s16 *)((u8 *)v + 0xA);
    s->unk_14 |= 0x8000;
    PushMatrix();
    sp->unk_34.vx = sp->unk_34.vy = sp->unk_34.vz = 0x2000;
    sp->unk_A4.vx = s->unk_16;
    sp->unk_A4.vz = s->unk_1A;
    sp->unk_A4.vy = s->unk_18;
    RotMatrix(&sp->unk_A4, &sp->unk_74);
    mp = &m;
    ReadRotMatrix(mp);
    CompMatrix(mp, &sp->unk_74, &sp->unk_54);
    ScaleMatrix(&sp->unk_54, &sp->unk_34);
    SetTransMatrix(&sp->unk_54);
    SetRotMatrix(&sp->unk_54);
    sp->unk_28 = s->unk_14;
    for (i = 0; i < 4; i++)
    {
        q[i] = *(u16 *)(p + 0x54 + i * 2);
    }
    while (abs(q[0] - q[2]) >= 0x11)
    {
        sp->unk_B0[0].vx = *(u16 *)(p + 0x44);
        sp->unk_B0[1].vx = *(u16 *)(p + 0x46);
        sp->unk_B0[2].vx = *(u16 *)(p + 0x48);
        sp->unk_B0[3].vx = *(u16 *)(p + 0x4A);
        sp->unk_B0[0].vy = *(u16 *)(p + 0x4C);
        sp->unk_B0[1].vy = *(u16 *)(p + 0x4E);
        sp->unk_B0[2].vy = *(u16 *)(p + 0x50);
        sp->unk_B0[3].vy = *(u16 *)(p + 0x52);
        sp->unk_B0[0].vz = q[2] - 0x10;
        sp->unk_B0[1].vz = q[3] - 0x10;
        sp->unk_B0[2].vz = q[2];
        sp->unk_B0[3].vz = q[3];
        q[2] -= 0x10;
        q[3] -= 0x10;
        t = *(u8 **)(g->unk_00 + 0x8D0);
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x18;
        sp->unk_100 = RotAverage4(&sp->unk_B0[0], &sp->unk_B0[1], &sp->unk_B0[2], &sp->unk_B0[3],
                                  (s32 *)(t + 0x8), (s32 *)(t + 0xC), (s32 *)(t + 0x10),
                                  (s32 *)(t + 0x14), &sp->unk_D0, &sp->unk_D4) -
                      d - 6;
        if ((u32)sp->unk_100 < 0x1E0)
        {
            if (((u16)(*(u16 *)(t + 0x8) + 0x20) < 0x181 && (u16)(*(u16 *)(t + 0xA) + 0x20) < 0x121) |
                ((u16)(*(u16 *)(t + 0xC) + 0x20) < 0x181 && (u16)(*(u16 *)(t + 0xE) + 0x20) < 0x121) |
                ((u16)(*(u16 *)(t + 0x10) + 0x20) < 0x181 && (u16)(*(u16 *)(t + 0x12) + 0x20) < 0x121) |
                ((u16)(*(u16 *)(t + 0x14) + 0x20) < 0x181 && (u16)(*(u16 *)(t + 0x16) + 0x20) < 0x121))
            {
                s->unk_14 &= 0x7FFF;
                *(s32 *)(t + 0x4) = *(s32 *)((u8 *)s + 0xC);
                SetPolyF4(t);
                addPrim(sp->unk_24 + sp->unk_100 * 4, t);
            }
        }
    }
    if (q[0] != q[2])
    {
        sp->unk_B0[0].vx = *(u16 *)(p + 0x44);
        sp->unk_B0[1].vx = *(u16 *)(p + 0x46);
        sp->unk_B0[2].vx = *(u16 *)(p + 0x48);
        sp->unk_B0[3].vx = *(u16 *)(p + 0x4A);
        sp->unk_B0[0].vy = *(u16 *)(p + 0x4C);
        sp->unk_B0[1].vy = *(u16 *)(p + 0x4E);
        sp->unk_B0[2].vy = *(u16 *)(p + 0x50);
        sp->unk_B0[3].vy = *(u16 *)(p + 0x52);
        sp->unk_B0[0].vz = q[0];
        sp->unk_B0[1].vz = q[1];
        sp->unk_B0[2].vz = q[2];
        sp->unk_B0[3].vz = q[3];
        t = *(u8 **)(g->unk_00 + 0x8D0);
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x18;
        sp->unk_100 = RotAverage4(&sp->unk_B0[0], &sp->unk_B0[1], &sp->unk_B0[2], &sp->unk_B0[3],
                                  (s32 *)(t + 0x8), (s32 *)(t + 0xC), (s32 *)(t + 0x10),
                                  (s32 *)(t + 0x14), &sp->unk_D0, &sp->unk_D4) -
                      d - 6;
        if ((u32)sp->unk_100 < 0x1E0)
        {
            if (((u16)(*(u16 *)(t + 0x8) + 0x20) < 0x181 && (u16)(*(u16 *)(t + 0xA) + 0x20) < 0x121) |
                ((u16)(*(u16 *)(t + 0xC) + 0x20) < 0x181 && (u16)(*(u16 *)(t + 0xE) + 0x20) < 0x121) |
                ((u16)(*(u16 *)(t + 0x10) + 0x20) < 0x181 && (u16)(*(u16 *)(t + 0x12) + 0x20) < 0x121) |
                ((u16)(*(u16 *)(t + 0x14) + 0x20) < 0x181 && (u16)(*(u16 *)(t + 0x16) + 0x20) < 0x121))
            {
                s->unk_14 &= 0x7FFF;
                *(s32 *)(t + 0x4) = *(s32 *)((u8 *)s + 0xC);
                SetPolyF4(t);
                addPrim(sp->unk_24 + sp->unk_100 * 4, t);
            }
        }
    }
    PopMatrix();
}

s32 func_800D67B0(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_800D6170(p, v, s, 0);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

/* One frame of the two-record fade. THE STEP IS RE-READ, NOT CACHED: a local
   for it is 58 words against 60, 59 differing -- the parameter never gets its
   own $a3 copy and the divide loses the register copy gcc keeps only when the
   quotient is written back into the dividend. 60 words, 0 differing. */
void func_800D6804(u8 *p, u8 *q)
{
    *(s32 *)(q + 0x8) += *(s32 *)(q + 0x14);
    *(s32 *)(q + 0x14) = *(s32 *)(q + 0x14) * 3 / 4;
    p[4] = p[0] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    p[5] = p[1] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    p[6] = p[2] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    *(s32 *)(p + 0x8) = *(s32 *)(p + 0x4);
    if (--*(s16 *)(p + 0x32) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* func_800D5AC0 again, byte for byte. 120 words, 0 differing. */
s32 func_800D68F4(u8 *p, UnkVec *v)
{
    UnkDungeonSP *sp = (UnkDungeonSP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    TILE_1 *t;
    DR_MODE *dm;

    sp->unk_1C = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_24 = D_80083160.unk_00 + 0xB0;
    while (1)
    {
        sp->unk_04 = *(u16 *)((u8 *)v + 0x2);
        sp->unk_06 = *(u16 *)((u8 *)v + 0x6);
        sp->unk_08 = *(u16 *)((u8 *)v + 0xA);
        t = (TILE_1 *)sp->unk_1C;
        sp->unk_1C = (u8 *)(t + 1);
        sp->unk_100 = RotTransPers((SVECTOR *)&sp->unk_04, (s32 *)&t->x0, &sp->unk_D0, &sp->unk_D4);
        if ((u32)sp->unk_100 < 0x1E0)
        {
            *(u32 *)&t->r0 = *(u32 *)(p + 0x8);
            setTile1(t);
            setSemiTrans(t, 1);
            setRGB0(t, t->r0, t->g0, t->b0);
            addPrim(sp->unk_24 + sp->unk_100 * 4, t);
            dm = (DR_MODE *)sp->unk_1C;
            sp->unk_1C = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            addPrim(sp->unk_24 + sp->unk_100 * 4, dm);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_1C;
    return 0;
}

/* A 0x211 spawner that seeds its mover from the parent's and then offsets
   each axis.

   THE FIFTH ARGUMENT IS `s16`, AND THAT IS TWO WORDS. Declared s32 the stack
   parameter is rematerialised at its single use instead of being held in
   $s4, the function saves one callee-saved register fewer, and it comes out
   79 against 81 with 77 differing. The caller only ever passes `x >> 16`,
   whose sign extension gcc proves redundant, so no shift pair appears at the
   call site to witness the type -- the callee's word count is the only
   witness. The +0xC/+0x10 clear is a CHAINED assignment: as two statements
   the second reloads the mover, 83 words / 30 differing. 81 words,
   0 differing. */
void func_800D6AD4(UnkNode *a, u32 b, s16 dx, s16 dy, s16 dz, s16 f)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800D6804;
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2);
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6);
        *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA);
        *(s16 *)((u8 *)n->unk_08 + 0x2) += dx;
        *(s16 *)((u8 *)n->unk_08 + 0x6) += dy;
        *(s16 *)((u8 *)n->unk_08 + 0xA) += dz;
        *(s32 *)((u8 *)n->unk_08 + 0xC) = *(s32 *)((u8 *)n->unk_08 + 0x10) = 0;
        *(s32 *)((u8 *)n->unk_08 + 0x14) = 0xFFFA0000 - (f << 18);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x32) = 0x14;
        *(s16 *)(p + 0x34) = 0x14;
        func_8004491C(n, (u32)func_800D68F4);
        *(u32 *)p = b;
        *(u32 *)(p + 0x8) = b;
    }
}

/* The 0x1A-frame close of the 0x211 burst: bleed the parent's +0x14 delta
   into the mover at half rate, re-key the two +0x54/+0x56 heights, walk the
   four 2-byte pairs at +0x44/+0x4C back towards zero on the odd frames below
   0x13, and on frame 0xF fire 0x51 copies of func_800D6AD4 down a 4.0-per-
   step column.

   THREE SPELLINGS, each measured as a single ablation off the landed body.
   (1) THE +0x14 POINTER IS READ BEFORE THE GUARD: inside the `if` its `lw`
   cannot share the `lh`'s load-delay slot and it is 108 words / 97
   differing. (2) THE COLUMN OFFSET IS A GIV,
   `(i * -0x40000) >> 16`, NOT AN ACCUMULATOR. As `z = 0; ... z -= 0x40000;`
   the 0xFFFC0000 constant is a loop invariant, loop.c hoists it into a SIXTH
   callee-saved register and the function is 108 words -- the two extra words
   are $s3's save and restore. loop.c builds the giv's own update after
   move_movables has run, so the same `lui` inside a giv update is never
   offered for hoisting. 108 / 106, and five accumulator spellings (`-=`,
   `+= -C`, `z = z - C`, `z = z + 0xFFFC0000`, the update in the third `for`
   clause) all measure the same. (3) THE TWO HEIGHTS ARE A CHAINED
   ASSIGNMENT: as two statements the +0x56 store flushes cse's memory table,
   +0x16 is loaded twice and it is 108 / 85. The four counters are written as SUBSCRIPTS off
   p at every site, not walked: a `q = p; q += 2;` cursor folds the +0x4C
   into the biv and the displacements come out -8 and 0.
   106 words, 0 differing. */
void func_800D6C18(u8 *p, u8 *m)
{
    s32 i;
    u8 *r;

    r = *(u8 **)(p + 0x14);
    if (*(s16 *)(p + 0xC) >= 0xF)
    {
        *(s32 *)(m + 0x14) += *(s32 *)(r + 0x14) / 2;
    }
    *(s16 *)(p + 0x54) = *(s16 *)(p + 0x56) = *(u16 *)(m + 0x16) + 0x10;
    if (*(s16 *)(p + 0xC) < 0x13 && (*(u16 *)(p + 0xC) & 1) == 0)
    {
        for (i = 0; i < 4; i++)
        {
            if (*(s16 *)(p + i * 2 + 0x44) < 0)
            {
                *(s16 *)(p + i * 2 + 0x44) = *(u16 *)(p + i * 2 + 0x44) + 1;
            }
            if (*(s16 *)(p + i * 2 + 0x44) > 0)
            {
                *(s16 *)(p + i * 2 + 0x44) = *(u16 *)(p + i * 2 + 0x44) - 1;
            }
            if (*(s16 *)(p + i * 2 + 0x4C) < 0)
            {
                *(s16 *)(p + i * 2 + 0x4C) = *(u16 *)(p + i * 2 + 0x4C) + 1;
            }
            if (*(s16 *)(p + i * 2 + 0x4C) > 0)
            {
                *(s16 *)(p + i * 2 + 0x4C) = *(u16 *)(p + i * 2 + 0x4C) - 1;
            }
        }
    }
    if (*(s16 *)(p + 0xC) == 0xF && *(s16 *)(p + 0x10) == 0)
    {
        for (i = 0; i < 0x51; i++)
        {
            func_800D6AD4((UnkNode *)(p - 0x20), 0xE0E0E0, 0, 0, (i * -0x40000) >> 16, (s16)i);
        }
    }
    if (--*(s16 *)(p + 0xC) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The eight-way ring burst: eight 0x112 nodes, each a copy of the caller's
   0x30-byte renderable template with func_800D6C18 in its +0x10 slot,
   func_800D67B0 as its node call, and the +0x44..+0x52 counter pairs seeded
   from the sine and cosine of its own angle and of the next one.
   139 words, 0 differing.

   THREE LEVERS, and all three are about how cc1psx expands something that
   looks like a constant.
   (1) THE TEMPLATE COPY GOES THROUGH A WORD-ALIGNED STRUCT. `memcpy` on two
   `u8 *` is alignment 1, so cc1psx emits a runtime `or`/`andi 3` test, an
   lwl/lwr path AND an aligned path: 163 words. `*(UnkDungeonWord48 *)r =
   *(UnkDungeonWord48 *)src;` picks the expansion off TYPE_ALIGN and gives
   the target's single four-register loop.
   (2) THE 3 IS A LOCAL. `x * 3 >> 12` is `sll`/`addu`/`sra`, because
   expand_mult synthesises a constant multiply; the target has
   `mult $s7, $v0` against a 3 hoisted into a callee-saved register, which
   only happens when the multiplier is a variable at expand time. And the
   operand order is `c * f(...)`, not `f(...) * c`.
   (3) THE SECOND ANGLE IS A GIV, NOT AN ACCUMULATOR. As `ang = 0x200;` with
   `ang += 0x200;` at the loop bottom it is a SOURCE statement and sits ahead
   of loop.c's hoisted D_800E23D8 base in the preheader; written
   `(i + 1) * 0x200` at both uses loop.c strength-reduces it and its
   initialisation lands after the hoist, which is 4 differing.
   `i << 9` is NOT reduced and is right as written.

   The four zero stores are written 0x5A down to 0x54: ascending is
   2 differing, because they fill the last `mult`'s latency in reverse. */
void func_800D6DC0(u8 *a, u8 *m, void *src, u8 *e)
{
    UnkNode *n;
    UnkVec *v;
    u8 *w;
    u8 *r;
    s32 i;
    s32 t;
    s32 c;

    c = 3;
    for (i = 0; i < 8; i++)
    {
        n = func_8003FD64(0x112, D_80083498);
        if (n != NULL)
        {
            w = (u8 *)n + 0x20;
            *(s16 *)(w + 0xC) = 0x1E;
            *(s16 *)(w + 0xE) = 0x1E;
            *(u8 **)(w + 0x14) = m;
            *(s16 *)(w + 0x10) = i;
            r = (u8 *)n->unk_0C;
            n->unk_10 = (u32)func_800D6C18;
            *(UnkDungeonWord48 *)r = *(UnkDungeonWord48 *)src;
            *(s16 *)(r + 0x10) = 0x20;
            *(u16 *)(r + 0x14) |= 0xC;
            func_8004491C(n, (u32)func_800D67B0);
            v = n->unk_08;
            *(s16 *)((u8 *)v + 0x2) = *(u16 *)(m + 0x2);
            *(s16 *)((u8 *)v + 0x6) = *(u16 *)(m + 0x6);
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)(m + 0xA) + 0x28;
            *(s16 *)(r + 0x1E) = 0x1000;
            *(s16 *)(r + 0x1C) = 0x1000;
            r[0xE] = 0xFA;
            r[0xD] = 0xFA;
            r[0xC] = 0xFA;
            t = i << 9;
            *(s16 *)(w + 0x44) = *(s16 *)(w + 0x48) = c * func_80064584(t) >> 12;
            *(s16 *)(w + 0x46) = *(s16 *)(w + 0x4A) = c * func_80064584((i + 1) * 0x200) >> 12;
            *(s16 *)(w + 0x4C) = *(s16 *)(w + 0x50) = c * func_800644B8(t) >> 12;
            *(s16 *)(w + 0x4E) = *(s16 *)(w + 0x52) = c * func_800644B8((i + 1) * 0x200) >> 12;
            *(s16 *)(w + 0x5A) = 0;
            *(s16 *)(w + 0x58) = 0;
            *(s16 *)(w + 0x56) = 0;
            *(s16 *)(w + 0x54) = 0;
            *(s16 *)((u8 *)v + 0x16) = 0;
            *(u8 **)(r + 0x2C) = D_800E23D8;
            func_80047784(r, D_800E23D8[((D_80083228 + *(s16 *)(e + 0x2A) + 0x100) >> 9) & 7], 0);
        }
    }
}

/* Copy the three integer halves of the mover at +0xB0 into b, run the node
   call, and close on func_800999D4's countdown tail. 36 words, 0 differing. */
void func_800D6FEC(u8 *p, u8 *b, u8 *c)
{
    u8 *m;

    m = *(u8 **)(p + 0xB0);
    *(s16 *)(b + 0x2) = *(s16 *)(m + 0x2);
    *(s16 *)(b + 0x6) = *(s16 *)(m + 0x6);
    *(s16 *)(b + 0xA) = *(s16 *)(m + 0xA);
    func_800478B8(c);
    if (--*(s16 *)(p + 0x96) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* func_800D5CA0 again, byte for byte. 75 words, 0 differing. */
void func_800D707C(u8 *p, UnkVec *v)
{
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    v->unk_0C /= 2;
    v->unk_10 /= 2;
    v->unk_14 /= 2;
    p[4] = p[0] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    p[5] = p[1] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    p[6] = p[2] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    *(s32 *)(p + 0x8) = *(s32 *)(p + 0x4);
    if (--*(s16 *)(p + 0x32) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Emit one POLY_FT4 per sprite cell of the renderable the actor's +0xA4 slot
   array holds for the caller's +0x6 id: build the quad's four corners in the
   scratchpad from the cell's 0xC-byte descriptor, project them with
   RotTransPers4, bias by the view's +0xB8 screen offset, and -- when any
   corner lands inside the 384x288 guard band -- fill the primitive's UVs,
   patch the group's code byte and addPrim it at the scratchpad's +0xC0 depth.
   Returns the advanced cursor. 454 words, 0 differing; seqcheck `seq=OK`,
   `arith=OK`, raw 8 == relocdump 8, with ten constant-address scratchpad
   words the raw comparison verifies.

   `RotTransPers4` IS SPELLED BY ITS LIBRARY NAME. splat calls it
   func_800654B0 (dungeon.bin_undefined_funcs_auto.txt:134) but
   splat/common_syms.txt:115 carries `RotTransPers4 = 0x800654b0` and
   build/dungeon.bin links that script since 2026-11-15, so the psyq
   declaration in psyq/include/libgte.h:363 is the one to use and no local
   declaration may be added.

   FOUR SPELLINGS ARE LOAD-BEARING, each measured by ablation from the landed
   body with tools/decomp/aligndiff.py.
   (1) THE TWO LOOPS SHARE ONE COUNTER. Separate `i` and `j` are two
   allocnos, so global-alloc hands them two hard registers and the whole
   function rotates: still 454 words but 28 aligned differences, with `e` in
   $s7 rather than $fp, the counter in $fp rather than $s6 and the primitive
   cursor in $s6 rather than $s7. One variable reused across the two disjoint
   loops is one allocno and one register.
   (2) `x` AND `y` MUST BE `s16`. The corner value's HImode destination is
   what stops combine folding `lbu`/`sll 24`/`sra 24` into a single `lb`: as
   `s32` the function is 450 words -- four short, two per axis -- and 42
   differing. The second corner is then `x - sp->unk_10` written out in each
   arm, which keeps the `lhu` on the word field and lets cross-jumping merge
   only the two `sh` at +0x88/+0x78.
   (3) THE TWO FLIP TESTS READ THE HIGH CORNER FIRST. `q->x0 > q->x3` and
   `q->y0 > q->y3`, not `q->x3 < q->x0` / `q->y3 < q->y0`: the `slt` is the
   same either way, but the operand evaluated first is the one that gets
   $v0/$v1 and the earlier load slot. The inverted forms are 6 and 2
   differing.
   (4) THE CODE BYTE IS A LOCAL AND ITS PATCH IS AN `if`/`else`. `t[0x1]`
   re-read at each use is 459 words / 18 differing, because the `sb` into
   r[0xF] is a QImode store and wipes cse's whole memory table; and
   `w = v | 2; if ((f & 4) == 0) { w = v & 0xFD; }` -- the shape reorg's
   delay-slot fill makes look right -- is 8 differing, a three-register
   rotation. The plain two-arm `if`/`else` is the one that lands.

   AND THE Y AXIS IS NOT THE X AXIS' MIRROR. Its `else` arm is
   `(s8)t[0x3] - sp->unk_10A` where the x axis' is `(s8)t[0x2] +
   sp->unk_108`; writing the symmetric `+` is 1 differing. */
POLY_FT4 *func_800D71A8(u8 *p, s32 a, u8 *b, u8 *c, POLY_FT4 *q)
{
    UnkDungeonSP7 *sp = (UnkDungeonSP7 *)0x1F800000;
    u8 *e;
    u8 *r;
    u8 *t;
    s32 i;
    s32 k;
    s32 v;
    s32 w;
    s16 x;
    s16 y;

    e = NULL;
    for (i = 0; i < *(s16 *)(p + 0x9E); i++)
    {
        if (*(u8 **)(p + 0xA4 + i * 4) != NULL)
        {
            e = *(u8 **)(p + 0xA4 + i * 4) + 0x20;
            if (*(u16 *)(c + 0x6) == *(u16 *)(e + 0x6))
            {
                break;
            }
        }
    }
    for (i = 0; i < *(s16 *)(e + 0x2); i++)
    {
        r = &e[i * 0x30 + 0x8];
        t = *(u8 **)(r + 0x8);
        while (1)
        {
            if ((t[0x0] & 0x20) == 0)
            {
                sp->unk_08 = t[0x8];
                sp->unk_0C = t[0x9];
                sp->unk_10 = t[0xA];
                sp->unk_14 = t[0xB];
                if (((t[0x0] ^ sp->unk_24) & 1) != 0)
                {
                    x = -(s8)t[0x2] - sp->unk_108;
                    sp->unk_70[0].vx = sp->unk_70[2].vx = x;
                    sp->unk_70[1].vx = sp->unk_70[3].vx = x - sp->unk_10;
                }
                else
                {
                    x = (s8)t[0x2] + sp->unk_108;
                    sp->unk_70[0].vx = sp->unk_70[2].vx = x;
                    sp->unk_70[1].vx = sp->unk_70[3].vx = x + sp->unk_10;
                }
                if (((c[0x0] ^ *(u16 *)(b + 0x14)) & 1) != 0)
                {
                    sp->unk_70[0].vx = sp->unk_70[2].vx = sp->unk_70[2].vx - (s8)c[0x2];
                    sp->unk_70[1].vx = sp->unk_70[3].vx = sp->unk_70[3].vx - (s8)c[0x2];
                }
                else
                {
                    sp->unk_70[0].vx = sp->unk_70[2].vx = sp->unk_70[2].vx + (s8)c[0x2];
                    sp->unk_70[1].vx = sp->unk_70[3].vx = sp->unk_70[3].vx + (s8)c[0x2];
                }
                if (((t[0x0] ^ sp->unk_24) & 2) != 0)
                {
                    y = -(s8)t[0x3] - sp->unk_10A;
                    sp->unk_70[0].vy = sp->unk_70[1].vy = y;
                    sp->unk_70[2].vy = sp->unk_70[3].vy = y - sp->unk_14;
                }
                else
                {
                    y = (s8)t[0x3] - sp->unk_10A;
                    sp->unk_70[0].vy = sp->unk_70[1].vy = y;
                    sp->unk_70[2].vy = sp->unk_70[3].vy = y + sp->unk_14;
                }
                if (((c[0x0] ^ *(u16 *)(b + 0x14)) & 2) != 0)
                {
                    sp->unk_70[0].vy = sp->unk_70[1].vy = sp->unk_70[1].vy - (s8)c[0x3];
                    sp->unk_70[2].vy = sp->unk_70[3].vy = sp->unk_70[3].vy - (s8)c[0x3];
                }
                else
                {
                    sp->unk_70[0].vy = sp->unk_70[1].vy = sp->unk_70[1].vy + (s8)c[0x3];
                    sp->unk_70[2].vy = sp->unk_70[3].vy = sp->unk_70[3].vy + (s8)c[0x3];
                }
                RotTransPers4(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2], &sp->unk_70[3],
                              (s32 *)&sp->unk_F0[0], (s32 *)&sp->unk_F0[1], (s32 *)&sp->unk_F0[2],
                              (s32 *)&sp->unk_F0[3], &sp->unk_90, &sp->unk_94);
                q->x0 = sp->unk_F0[0].vx + sp->unk_B8;
                q->y0 = sp->unk_F0[0].vy + sp->unk_BA;
                q->x1 = sp->unk_F0[1].vx + sp->unk_B8;
                q->y1 = sp->unk_F0[1].vy + sp->unk_BA;
                q->x2 = sp->unk_F0[2].vx + sp->unk_B8;
                q->y2 = sp->unk_F0[2].vy + sp->unk_BA;
                q->x3 = sp->unk_F0[3].vx + sp->unk_B8;
                q->y3 = sp->unk_F0[3].vy + sp->unk_BA;
                if (((u16)(q->x0 + 0x20) < 0x181 && (u16)(q->y0 + 0x20) < 0x121) |
                    ((u16)(q->x1 + 0x20) < 0x181 && (u16)(q->y1 + 0x20) < 0x121) |
                    ((u16)(q->x2 + 0x20) < 0x181 && (u16)(q->y2 + 0x20) < 0x121) |
                    ((u16)(q->x3 + 0x20) < 0x181 && (u16)(q->y3 + 0x20) < 0x121))
                {
                    setlen(q, 9);
                    sp->unk_10 = sp->unk_10 + sp->unk_08;
                    if ((sp->unk_10 & 0x100) != 0)
                    {
                        sp->unk_10 = sp->unk_10 - 1;
                    }
                    sp->unk_14 = sp->unk_14 + sp->unk_0C;
                    if ((sp->unk_14 & 0x100) != 0)
                    {
                        sp->unk_14 = sp->unk_14 - 1;
                    }
                    sp->unk_14 = sp->unk_14 * 0x100;
                    sp->unk_0C = sp->unk_0C * 0x100;
                    *(u32 *)&q->u0 = (sp->unk_0C + sp->unk_08) +
                                     ((*(u16 *)(b + 0x12) + *(u16 *)(t + 0x6)) << 16);
                    *(u16 *)&q->u1 = sp->unk_0C + sp->unk_10;
                    k = *(u16 *)(b + 0x10);
                    if (k != 0)
                    {
                        q->tpage = k + (*(u16 *)(t + 0x4) & 0xFF9F);
                    }
                    else
                    {
                        q->tpage = *(u16 *)(t + 0x4);
                    }
                    *(u16 *)&q->u2 = sp->unk_14 + sp->unk_08;
                    *(u16 *)&q->u3 = sp->unk_14 + sp->unk_10;
                    if (q->x0 > q->x3)
                    {
                        q->u1--;
                        q->u3--;
                    }
                    if (q->y0 > q->y3)
                    {
                        q->v2--;
                        q->v3--;
                    }
                    v = t[0x1];
                    r[0xF] = v;
                    if ((*(u16 *)(r + 0x14) & 0x8) != 0)
                    {
                        if ((*(u16 *)(r + 0x14) & 0x4) != 0)
                        {
                            w = v | 2;
                        }
                        else
                        {
                            w = v & 0xFD;
                        }
                        r[0xF] = w;
                    }
                    if ((*(u16 *)(r + 0x14) & 0x10) != 0)
                    {
                        r[0xF] = r[0xF] | 1;
                    }
                    else
                    {
                        r[0xF] = r[0xF] & 0xFE;
                    }
                    *(u32 *)&q->r0 = *(u32 *)(r + 0xC);
                    if ((*(u16 *)(e + 0x4) & 0x8000) == 0)
                    {
                        addPrim((u32 *)sp->unk_20 + sp->unk_C0, q);
                    }
                    q++;
                }
            }
            if (*(s8 *)t < 0)
            {
                break;
            }
            t += 0xC;
        }
    }
    return q;
}

/* `&p[i * 0x30 + 0x8]` AND NOT A WALKED OFFSET. With `off = 8;` before the
   loop and `off += 0x30` in the body, the `addiu $s1, $zero, 0x8` is emitted
   ahead of the loop guard (5 differing); as a subscript the 8 is loop.c's
   giv initial value and lands in the preheader, after the guard, where the
   target has it. `p + i * 0x30 + 8` rather than `&p[...]` is 2 differing.
   40 words, 0 differing. */
void func_800D78C0(u8 *p)
{
    s32 i;

    if (**(u16 **)(p + 0x98) & 0x8000)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    for (i = 0; i < *(s16 *)(p + 0x2); i++)
    {
        func_800478B8(&p[i * 0x30 + 0x8]);
    }
}

/* 20 words, 0 differing. */
void func_800D7960(u8 *p, void *b, u8 *c)
{
    if (p[0x9B] == 0xFF)
    {
        *(u16 *)(p - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    func_800478B8(c);
}

/* func_800D7960 with the renderer's 0x6000 bits as the gate.
   21 words, 0 differing. */
void func_800D79B0(u8 *p, void *b, u8 *c)
{
    if (*(u16 *)(c + 0x14) & 0x6000)
    {
        *(u16 *)(p - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    func_800478B8(c);
}

s32 func_800D7A04(void)
{
    return 0;
}

s32 func_800D7A0C(void)
{
    return 0;
}

/* Spawn a kind-2 node carrying the caller's record and park func_800D7A78 at
   its +0x10. THE NODE HAS TO BE HELD AS `u8 *`, not `UnkNode *`: through the
   struct pointer the install's `n->unk_10` is MEM_IN_STRUCT_P, sched1 lifts
   the handler's `lui` into the counter load's delay slot, the D_80083460
   base and the handler address stop sharing $v0, and the function comes out
   24 words against 25 with 14 differing. 25 words, 0 differing. */
void func_800D7A14(u8 *p)
{
    u8 *n;
    u8 *g;

    n = (u8 *)func_8003FD64(2, D_80083498);
    if (n != NULL)
    {
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
        *(u32 *)(n + 0x10) = (u32)func_800D7A78;
        *(u8 **)(n + 0x2C) = p;
    }
}

/* The 0x600-flash burst: project the scene's own slot once with
   func_8003DE58, then spawn EIGHT 0x312 nodes per frame while the state
   halfword at +0x00 is 0 or 1, and tear the counter down again in state
   0xFF.  p is the actor view (n + 0x20), which is why the state flag is
   reached as `*(u16 *)(p - 2)` -- that is the owning UnkNode's unk_1E.

   FOUR SPELLINGS WERE WORTH WORDS, all measured against this body:

   * `n->unk_10 = f;` as a COMPONENT_REF is 3 differing.  The store has to
     go through a CAST -- `*(u32 *)((u8 *)n + 0x10) = f;` -- or sched1
     floats the `lhu 0xA($s4)` of the D_80083460 counter above it, because
     MEM_IN_STRUCT_P on the field store makes the two MEMs independent.
     This is docs/matching-levers.md's "A GLOBAL'S LOAD FLOATS OVER A
     STRUCT-TYPED FIELD STORE, AND NOT OVER A CAST ONE", read from the
     other side.

   * the view stores must be written +0x18, +0x1C, +0x2A, +0x34.  The
     listing EMITS them 0x18, 0x2A, 0x1C, 0x34, and writing that order is
     17 differing; 0x2A first is also 17 and moving 0x2A last is 173 words
     and 63.

   * `f` and `g` exist so that loop.c hoists the two addresses in the
     target's order.  `*(u16 *)((u8 *)&D_80083460 + 0xA) += 1;` written
     inline is a one-word constant-base `lui` inside the loop and 171
     words; a `g` assigned at its point of use is not hoisted either
     (move_movables wants threshold*savings*lifetime >= insn_count and the
     lifetime is 1), 173 words; `g` at the top of the loop body IS hoisted
     but then lands AHEAD of func_800D7D30's address, because the preheader
     keeps body order -- 175 words.  Holding the handler address in `f` at
     the top of the body puts it first again.

   * `h` is a SECOND local for the same base in the 0xFF arm.  Sharing `g`
     with the loop makes it one global allocno and the arm gets a
     callee-saved register where the target uses $v1: 24 differing.

   Settled by measurement elsewhere in the body: `r` is read before `m`
   (n->unk_0C then n->unk_08), and r->unk_08 is assigned BEFORE the two
   zero bytes at +0x04/+0x05 -- the other order costs a load-delay `nop`,
   175 words.  174 words, 0 differing, seqcheck seq=OK arith=OK. */
void func_800D7A78(u8 *p, UnkVec *b, u8 *c)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    UnkSlot *s;
    u8 *d;
    u8 *g;
    u8 *h;
    u32 f;
    u8 *v;
    s32 i;
    s16 out[4];

    s = (UnkSlot *)D_80082E80;
    if (func_8003DE58((u8 *)s->unk_08, s, out, 0) != NULL)
    {
        d = D_80083780;
        *(s16 *)(p + 4) = *(u16 *)(d + 2) + out[0];
        *(s16 *)(p + 6) = *(u16 *)(d + 6) + out[1];
        *(s16 *)(p + 8) = *(u16 *)(d + 0xA) + out[2] + 0x20;
    }
    switch (*(s16 *)p)
    {
    case 0:
        func_800A56E0(0x600);
        *(s16 *)(p + 2) = 0x3C;
        *(s16 *)p += 1;
    case 1:
        i = 7;
        do
        {
            f = (u32)func_800D7D30;
            g = (u8 *)&D_80083460;
            n = func_8003FD64(0x312, D_80083498);
            if (n != NULL)
            {
                *(u32 *)((u8 *)n + 0x10) = f;
                *(u16 *)(g + 0xA) += 1;
                func_8004491C(n, (u32)D_80045340);
                r = (UnkSlot *)n->unk_0C;
                m = n->unk_08;
                m->unk_14 = 0xFFFC0000;
                r->unk_1E = 0x600;
                r->unk_1C = 0x600;
                r->unk_10 = 0x20;
                r->unk_00 = (UnkElem *)D_800DEA68;
                r->unk_14 |= 0xC;
                r->unk_08 = ((UnkElem *)D_800DEA68)->unk_04;
                r->unk_04 = 0;
                r->unk_05 = 0;
                *(u32 *)r->unk_0C = *(u32 *)(p + 0xC);
                *(UnkDungeonRect *)((u8 *)n + 0x2C) = *(UnkDungeonRect *)(p + 4);
                *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)(p + 8);
                v = n->payload;
                *(s16 *)(v + 0x18) = (i << 9) + *(s16 *)(p + 2) * 40;
                *(u32 *)(v + 0x1C) = 0x180000 - ((0x3C - *(s16 *)(p + 2)) << 13);
                *(s16 *)(v + 0x2A) = 6;
                *(u32 *)(v + 0x34) = (*(s16 *)(p + 2) - 0x3C) << 12;
            }
        } while (--i >= 0);
        *(s16 *)(p + 2) -= 1;
        if (*(s16 *)(p + 2) <= 0)
        {
            *(s16 *)(p + 2) = 8;
            *(s16 *)p = 0xFF;
        }
        break;
    case 0xFF:
        *(s16 *)(p + 2) -= 1;
        if (*(s16 *)(p + 2) <= 0)
        {
            h = (u8 *)&D_80083460;
            *(u16 *)(h + 0xA) -= 1;
            *(u16 *)(p - 2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* The dust/smoke particle step, two states on +0x4C. State 0 integrates the
   mover's three 16.16 axes, re-aims the sprite's +0x2/+0x6 integer halves off
   rsin/rcos of +0x18 scaled by +0x1E, bleeds +0x14 by 0x20000 with a random
   0..0x1FE00 kick, spins +0x18 on by 0x80 plus a random 0..0x3F, runs the
   +0x28/+0x34 accumulator into +0x1C, fades the three colour bytes by a
   sixteenth and promotes to state 1 once the sprite has risen past
   +0x10 - 0x60; state 1 releases the D_80083460 slot and kills the view.

   THE +0x1C ACCUMULATION MUST BE TWO COMPLETE ARMS OVER THE FIELD, NOT A
   LOCAL SHIFTED IN ONE ARM. With `t = p->28 + p->34; p->28 = t; if (p->1C <=
   0) t >>= 2; p->1C += t;` the two accumulator loads are scheduled up into
   the middle of the +0x18 update and the whole chain lands in $a1/$a0 where
   the target has $v1/$v0: 130 words, 12 differing. `p->28 += p->34;` followed
   by an `if`/`else` that each re-read +0x28 is 0 -- cse forwards the store
   back, so the re-reads cost nothing and the value's live range no longer
   spans the +0x18 store. The in-place form plus a read-back into the local is
   13 differing.
   130 words, 0 differing. */
void func_800D7D30(u8 *p, UnkVec *b, u8 *c)
{
    u8 *g;

    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        func_800478B8(c);
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            c[0x4] = 0;
            c[0x5] = 0;
        }
        *(s16 *)((u8 *)b + 0x2) =
            *(u16 *)(p + 0xC) + ((func_800644B8(*(s16 *)(p + 0x18)) * *(s16 *)(p + 0x1E)) >> 12);
        *(s16 *)((u8 *)b + 0x6) =
            *(u16 *)(p + 0xE) + ((func_80064584(*(s16 *)(p + 0x18)) * *(s16 *)(p + 0x1E)) >> 12);
        b->unk_14 += ((rand() & 0xFF) << 9) - 0x20000;
        *(s16 *)(p + 0x18) = *(u16 *)(p + 0x18) + 0x80 + (rand() & 0x3F);
        *(s32 *)(p + 0x28) += *(s32 *)(p + 0x34);
        if (*(s32 *)(p + 0x1C) <= 0)
        {
            *(s32 *)(p + 0x1C) += *(s32 *)(p + 0x28) >> 2;
        }
        else
        {
            *(s32 *)(p + 0x1C) += *(s32 *)(p + 0x28);
        }
        c[0xC] -= c[0xC] >> 4;
        c[0xD] -= c[0xD] >> 4;
        c[0xE] -= c[0xE] >> 4;
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            c[0x4] = 0;
            c[0x5] = 0;
        }
        if (*(s16 *)((u8 *)b + 0xA) < *(s16 *)(p + 0x10) - 0x60)
        {
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 1:
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}

/* Bump the D_80083460 counter at +0xA, allocate a kind-2 node and seed its
   view from the three halfwords of D_80083780.

   D_80083780 NEEDS A POINTER LOCAL, assigned immediately before its first
   use: read as three raw `*(u16 *)(D_80083780 + N)` the first offset folds
   into the `%lo` addend and cse derives the other two with an extra
   `addiu $v1, $v1, 2`, which is 5 differing. 32 words, 0 differing. */
void func_800D7F38(void)
{
    UnkNode *n;
    u8 *g;
    u8 *p;
    u8 *b;

    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
    n = func_8003FD64(2, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800D7FB8;
        p = (u8 *)n + 0x20;
        b = D_80083780;
        *(s16 *)(p + 0x4) = *(u16 *)(b + 0x2);
        *(s16 *)(p + 0x6) = *(u16 *)(b + 0x6);
        *(s16 *)(p + 0x8) = *(u16 *)(b + 0xA) - 0x30;
    }
}

/* The 0x312 spark storm: state 0 throws 151 nodes at func_800D81D4 with the
   seeded (x, y, z) from the installer and alternating +0x4A facings, then
   falls through into state 1's countdown; state 0xFF bleeds D_80083460's
   +0xA counter and raises the two 0x8000 flags. 135 words, 0 differing.

   CASE 0 FALLS THROUGH INTO CASE 1: the loop's `bgez` drops straight into
   .L800D8130. With a `break` the `j`/`nop` pair is two extra words.

   `q = (u8 *)n + 0x20` MUST BE ASSIGNED BEFORE THE func_8004491C CALL, even
   though its only use is the +0x4A store at the bottom. Assigned at the use
   it is a block-local quantity in $v1 and the function loses the sixth
   callee-saved register -- 133 words against 135 with 135 differing.

   The slot fill is func_8003DB94's body inlined with idx 0, and its NINE
   STORES ARE IN A FIXED ORDER: 0x1E, 0x1C, 0x10, 0x00, 0x14, 0x08, 0x04,
   0x05, 0x0C. Measured: 0x00,04,05,08,0C,1E,1C,10,14 is 135/14;
   1E,1C,10,0,14,4,5,C,8 is 136/86; 1E,1C,10,0,4,5,C,8,14 is 135/21.
   `r = (u8 *)n->unk_0C` also has to be read BEFORE the three n->unk_08
   stores -- after the first one it is 135/20, because the `lw 0xC($s0)` then
   misses the first load-delay hole. */
void func_800D7FB8(u8 *p)
{
    UnkNode *n;
    u8 *q;
    u8 *r;
    u8 *g;
    s32 i;

    switch (*(s16 *)p)
    {
    case 0:
        *(s16 *)(p + 0x2) = 0x3C;
        *(s16 *)p += 1;
        func_800A56E0(0x600);
        for (i = 0x96; i >= 0; i--)
        {
            n = func_8003FD64(0x312, D_80083498);
            if (n != NULL)
            {
                q = (u8 *)n + 0x20;
                n->unk_10 = (u32)func_800D81D4;
                func_8004491C(n, (u32)D_80045340);
                r = (u8 *)n->unk_0C;
                *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)(p + 0x4);
                *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)(p + 0x6);
                *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)(p + 0x8);
                *(s16 *)(r + 0x1E) = 0x400;
                *(s16 *)(r + 0x1C) = 0x400;
                *(s16 *)(r + 0x10) = 0x20;
                *(u8 **)(r + 0x0) = D_800DEC00;
                *(u16 *)(r + 0x14) |= 0xC;
                *(s32 *)(r + 0x8) = *(s32 *)(D_800DEC00 + 0x4);
                r[0x4] = 0;
                r[0x5] = 0;
                *(s32 *)(r + 0xC) = 0;
                *(UnkDungeonName8 *)((u8 *)n + 0x2C) = *(UnkDungeonName8 *)(p + 0x4);
                *(s16 *)(q + 0x4A) = (i & 1) ? 1 : -1;
                *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)(p + 0x8);
            }
        }
        /* FALLS THROUGH: the loop's exit branch drops straight into case 1 */
    case 1:
        if (--*(s16 *)(p + 0x2) <= 0)
        {
            *(s16 *)(p + 0x2) = 8;
            *(s16 *)p = 0xFF;
        }
        break;
    case 0xFF:
        if (--*(s16 *)(p + 0x2) <= 0)
        {
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) -= 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* func_800D81D4 -- **239 of 239 words, 0 DIFFERING**, 2026-11-20.

   The fourth of the seven 4-mod-8 jump-table functions: it owns
   `jtbl_80089474` at rodata offset 0xD14.  cc1psx writes `.align 3` before
   every table it compiles, so inside one big ovl_dungeon.c this table gains
   four bytes of padding the original does not have and the container comes
   out eight bytes long.  **Do not merge it until the unit is carved in the
   same change** -- see docs/dungeon-parks/func_800D50FC.c and
   docs/dungeon-sweep.md, "SEVEN of the 28 jump tables are at 4 mod 8".

   Seven cases, a sine/cosine position update off `p + 0x18`, and a shared
   tail.  m2c gave a clean draft at 238 words / 22 rows; FOUR facts took it
   to 0, and all four are about operand WIDTH or ASSOCIATION rather than
   structure:

   1. `p + 0x4A` IS AN `lh`, NOT AN `lhu` -- `*(s16 *)` for every read of it.
      3 rows.
   2. `p + 0x1E` IS ALSO AN `lh`.  3 rows (two reads in the tail).
   3. `c + 0x10` IS A HALFWORD STORE: `*(u16 *)(c + 0x10) = 0x60;`, not
      `c[0x10] = 0x60;`.  1 row.
   4. THE TWO ASSOCIATION FACTS, which are the interesting ones:

        ((0x20 - *(s16 *)(p + 0x48)) << 3) * *(s16 *)(p + 0x4A)

      with the `<< 3` WRITTEN AS A SHIFT AND PARENTHESISED, because
      `(0x20 - x) * 8 * y` reassociates: the target emits `subu` / `lh 0x4A`
      / `sll 3` / `mult`, and the `* 8` spelling folds the 8 into the
      multiply instead, which is 238 words / 19 rows.  Measured, all on the
      same body: `x * 8 * y` 238/19, `(x * 8) * y` 238/19, `8 * x * y`
      238/19, `x * y * 8` 239/16, `y * (x << 3)` 239/8, and
      `(x << 3) * y` **239 words and 7 rows**, which is the one below.

        ((-*(s16 *)(p + 0x48) << 4) + 0x1E0)

      NEGATE THEN SHIFT.  The target is `negu $v0, $v0` then
      `sll $v0, $v0, 4`; `x * -0x10` and `-(x << 4)` both emit the shift
      first and are 239/4.  `(-x) * 0x10` measures identically to
      `(-x) << 4`, so either spelling of the shift is fine once the negation
      is on the inside.  Two places, 4 rows.

   Also worth keeping: case 0 FALLS THROUGH into case 1, and cases 1, 2, 3
   and 4 all end by sharing the `*(u16 *)(p + 0x4C) += 1;` increment, which
   jump2 cross-jumps -- writing each one out is correct and costs nothing.
   `func_80069EF8() % 96` is a signed modulo and comes out as the target's
   `div`/`mfhi` pair.

   DECLARATIONS
   NONE -- func_800478B8, func_80064710, func_80069EF8, func_800644B8 and
   func_80064584 all ride implicit declarations exactly as the surrounding
   landed code does, and D_800814A0 is already declared.
   END DECLARATIONS

*/
void func_800D81D4(u8 *p, u8 *b, u8 *c)
{
    s32 i;
    s32 n;

    *(u16 *)(p + 0x48) -= 1;
    func_800478B8(c);
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        *(u16 *)(p + 0x48) = 0x10;
        i = func_80069EF8() % 96 - 0x30;
        *(s32 *)(b + 0x14) = (i << 0x10) / *(s16 *)(p + 0x48);
        *(s32 *)(p + 0x28) = (func_80064710(0x900 - i * i) << 0x10) / *(s16 *)(p + 0x48);
        *(u16 *)(p + 0x18) = func_80069EF8();
        *(u16 *)(p + 0x4C) += 1;
    case 1:
        *(s32 *)(b + 0x8) += *(s32 *)(b + 0x14);
        *(s32 *)(p + 0x1C) += *(s32 *)(p + 0x28);
        *(s32 *)(c + 0xC) += 0x40404;
        *(u16 *)(p + 0x18) += ((0x20 - *(s16 *)(p + 0x48)) << 3) * *(s16 *)(p + 0x4A);
        if (*(s16 *)(p + 0x48) < 0)
        {
            *(u16 *)(p + 0x48) = 0x14;
            *(u16 *)(p + 0x4C) += 1;
        }
        break;
    case 2:
        *(u16 *)(p + 0x18) += *(s16 *)(p + 0x4A) * 0x140;
        if (*(s16 *)(p + 0x48) <= 0)
        {
            *(u16 *)(p + 0x48) = 0x1C;
            *(u16 *)(p + 0x4C) += 1;
        }
        break;
    case 3:
        *(s32 *)(b + 0x8) -= *(s32 *)(b + 0x14) >> 1;
        *(s32 *)(p + 0x1C) -= *(s32 *)(p + 0x28) >> 1;
        *(s32 *)(c + 0xC) += 0x30303;
        *(u16 *)(p + 0x18) += ((0x1C - *(s16 *)(p + 0x48)) * 8 + 0x140) * *(s16 *)(p + 0x4A);
        if (*(s16 *)(p + 0x48) <= 0)
        {
            *(u16 *)(p + 0x48) = 0;
            *(u16 *)(p + 0x4C) += 1;
        }
        break;
    case 4:
        *(u16 *)(p + 0x18) += ((-*(s16 *)(p + 0x48)) * 0x10 + 0x1E0) * *(s16 *)(p + 0x4A);
        *(s32 *)(c + 0xC) += 0x40404;
        if (c[0xC] >= 0xF1)
        {
            *(u16 *)(c + 0x10) = 0x60;
            *(s16 *)(b + 0x16) = -(func_80069EF8() & 3);
            *(u16 *)(p + 0x4C) += 1;
        }
        break;
    case 5:
        *(s32 *)(b + 0x8) += *(s32 *)(b + 0x14);
        *(s32 *)(b + 0x14) -= (func_80069EF8() & 0xFFF) << 6;
        *(s32 *)(p + 0x1C) -= 0x8000;
        *(u16 *)(p + 0x18) += ((-*(s16 *)(p + 0x48)) * 0x10 + 0x1E0) * *(s16 *)(p + 0x4A);
        *(s32 *)(c + 0xC) += 0xFFE7E7E8;
        if (c[0xC] < 0x18)
        {
            *(u16 *)(p + 0x4C) += 1;
        }
        break;
    case 6:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
    *(s16 *)(b + 0x2) = *(s16 *)(p + 0xC) + (func_800644B8(*(s16 *)(p + 0x18)) * *(s16 *)(p + 0x1E) >> 0xC);
    *(s16 *)(b + 0x6) = *(s16 *)(p + 0xE) + (func_80064584(*(s16 *)(p + 0x18)) * *(s16 *)(p + 0x1E) >> 0xC);
}

/* Park the actor into its 0x2E "knocked out" state: stash the +0x10 handler
   and the current state byte in the shadow record at +0x20, install
   func_800D8728/func_800D8C64, re-skin from D_800E262C by the facing, and
   clear two bits of the renderable's +0x14 flags.

   THE ADDRESS ARITHMETIC IS THE WHOLE RESIDUE. `e += p[0xA8];` -- the sum
   written back INTO the pointer local -- is what gives the `lw` $v0 and the
   `lbu` $v1. Spelled `e[p[0xA8] + 0xF9] = 1;` the add is a separate pseudo,
   local-alloc ties it to the BYTE (combine.c's simplify_rtx canonicalisation
   puts the folded `(subreg:SI (reg:QI))` first in any commutative pair, so
   the byte is always operand 0 and always wins the tie), the byte takes $v0
   and the two loads come out exchanged: 2 differing, and no spelling of the
   subscript moves it -- `*(e + b + 0xF9)`, `*(e + 0xF9 + b)`, `e[0xF9 + b]`,
   `(e + 0xF9)[b]`, `*(&e[0xF9] + b)`, `*(u8 *)((s32)e + b + 0xF9)` and a
   separate `s32 b` local all measure 102/2.

   With the `+=` form the second read of p[0xA8] must be PLAIN. An earlier
   candidate needed `*(volatile u8 *)` to stop cse commoning it with the
   `if (p[0xA8] != 0)` guard; that is only true while the add is a third
   pseudo. Here cse's substitution is what feeds the `+=`, and `volatile`
   costs the last word (102/1), as does a trailing `do { } while (0)`.

   `*(s16 *)(c + 0x12) = 0;` must still sit BETWEEN the two +0x14 masks --
   written before both, flow deletes the first `sh` and the function is 99
   words -- and `e = D_800E3D7C;` must be a pointer local, which is what puts
   the `lw` ahead of the byte load.

   func_800D8728 needs a declaration above this function; it is address-taken
   here and defined below. */
u8 *func_800D8590(u8 *p)
{
    u8 *g;
    u8 *n;
    u8 *c;
    u8 *a;
    u8 *t;
    u8 *e;

    g = (u8 *)&D_80083460;
    n = p - 0x20;
    if (*(s32 *)(g + 0x10) == (s32)n)
    {
        *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
    }
    c = *(u8 **)(n + 0xC);
    if (p[0x13] == 0)
    {
        func_80094E34();
    }
    else
    {
        if (p[0x13] == 0x2B)
        {
            func_80042B68((s8 *)p, 0x1A);
        }
        if (p[0x13] == 0x15)
        {
            if (p[0xA7] != 0)
            {
                if (p[0xA8] != 0)
                {
                    e = D_800E3D7C;
                    e += p[0xA8];
                    e[0xF9] = 1;
                }
            }
        }
        bzero(n + 0xAC, 0x24);
        a = n + 0x20;
        *(u32 *)(a + 0xAC) = *(u32 *)(n + 0x10);
        *(s32 *)(n + 0x10) = (s32)func_800D8728 & 0x7FFFFFFF;
        *(s16 *)(a + 0xA8) = p[0x13];
        p[0x13] = 0x2E;
        a[0x9A] = 0xFF;
        *(s8 *)(a + 0x9C) = -1;
        *(void **)(a + 0x8C) = func_800D8C64;
        t = D_800E262C;
        *(u8 **)(c + 0x2C) = t;
        func_8003DB94(c, *(void **)(t + (((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 7) & 0x1C)), 0);
        *(s16 *)(a + 0xA0) = 0x14;
    }
    *(u16 *)(c + 0x14) |= 0x8000;
    *(s16 *)(c + 0x12) = 0;
    *(u16 *)(c + 0x14) &= 0xFFF3;
    return p;
}

/* The dungeon actor's SECOND per-frame driver, func_800B274C (:18193) one
   state machine over: past the engine's 0x2000 gate it runs func_800A9E70,
   then the record's own +0x8C continuation, then the +0x9A entry of the
   D_800E264C table, re-aims the tile when the +0x6D facing moved under it,
   re-skins the slot for the eight-way facing, steps the mover by its
   velocity, ages the 0xE state's +0xA0 countdown, runs the +0x9D bob counter
   and finally leashes +0x92/+0x88 to the ground height func_800BCB04 reports.
   335 words, 0 differing.

   THE RECORD IS ITS OWN ACTOR and is read through TWO ALIASES, exactly as
   func_800B274C's comment records: `q` carries +0x1C, +0x88 and the SECOND
   +0x2A, `p` carries everything else including the first +0x2A.

   THE FACING INDEX NEEDS TWO *NARROWER* COPIES, AND THAT IS THE WHOLE
   LANDING. The `& 7` index is read three times -- the +0x94 compare and the
   +0x2C subscript, the +0x94 store, and the D_8006CCF8 test -- and the target
   holds it in THREE registers, $s0, $a0 and $s5, the last live across
   func_8003DB94 and the reason the function saves a sixth callee-saved
   register. A same-width `s32 k = v;` is copy-propagated away: all four
   placements of one extra `s32` local measure the identical 332 words / 18
   aligned differences, three words short with no $s5 at all. Declaring the
   two copies NARROWER makes each `(set (reg:HI) (subreg:HI ...))`, which
   cse's `make_regs_eqv` never equates (docs/matching-levers.md, "A `u8`
   destination makes a register copy INVISIBLE to cse"), and MIPS emits it as
   the plain `addu` the target has: 335 words, 3 differing. The two copies
   must not share a MODE -- s16/s16, u8/u16, u16/u8, s16/s8 and s8/s16 are all
   3 differing, while u8/u8 and u16/u16 are 336 / 109, because cse then
   equates the two copies with each other.

   THE SECOND +0x8C HANDLER NEEDS ITS OWN LOCAL. One `h` reused for both reads
   is a single global-alloc quantity and takes $v1 at both sites; the target's
   second site is $v0. Two locals make them local-alloc quantities and the
   second gets $v0 -- that one split is the last 3 differing, on the
   `lw`/`beqz`/`jalr` triple alone.

   THE `r < q88` ARM CARRIES ITS OWN COPY OF THE THREE-STORE TAIL, AND ITS
   TRAILING +0x92 STORE IS WHAT KEEPS IT. func_800B274C puts
   `*(s16 *)(p + 0x92) = r - *(u16 *)(q + 0x88);` in one arm and the
   `b->unk_14 = 0` / `|= 0x8000000` / `p[0x9D] = 0` tail AFTER the if/else.
   Here the tail is written out in BOTH arms with the +0x92 store LAST in the
   second: that makes the two arms' final insns differ, so jump2's
   cross-jumping cannot merge them. Written the template's way the tails are
   identical, cross-jumping branches into the middle of the first arm, and the
   function is 325 words / 30 aligned differences -- seven short.

   THE +0x20 GATE IS SPELLED `== 0` with the 0x7000 store in the `else`; the
   `!= 0` form lays the arms out the other way round and loses the `j`/`nop`
   pair, two words. The +0xA0 window is one range test, `>= -7 && <= 0`, which
   folds to the HImode `addiu 7` / `andi 0xFFFF` / `sltiu 8` the target has. */
void func_800D8728(u8 *p, UnkVec *b, u8 *c)
{
    void (*h)(u8 *, UnkVec *, u8 *, u8 *);
    void (*h2)(u8 *, UnkVec *, u8 *, u8 *);
    u8 *q;
    u8 *t;
    void **e;
    s16 dir;
    s32 r;
    s32 v;
    s16 k1;
    s16 k2;

    *(s16 *)(c + 0x12) = 0;
    q = p;
    if ((D_80083462 & 0x2000) != 0)
    {
        h = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
        if (h == (void (*)(u8 *, UnkVec *, u8 *, u8 *))func_800D8C64)
        {
            h(p, b, c, p);
        }
        else
        {
            p[0x71] &= 0x7F;
        }
        return;
    }
    dir = (s8)p[0x6D];
    if (func_800A9E70((UnkDungeonActor *)p, b, c, p) != 0)
    {
        return;
    }
    h2 = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
    if (h2 != NULL)
    {
        h2(p, b, c, p);
    }
    D_800E264C[p[0x9A]](p, b, c, p);
    if (dir != *(s8 *)(p + 0x6D))
    {
        func_800AA36C(p, b, c, p);
    }
    if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
    {
        v = ((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7;
        k1 = v;
        k2 = v;
        if (*(s16 *)(p + 0x94) != k1)
        {
            e = *(void ***)(c + 0x2C);
            if (e != NULL)
            {
                func_8003DB94(c, e[k1], *(s8 *)(c + 0x4));
            }
            *(s16 *)(p + 0x94) = v;
        }
        if (D_8006CCF8[k2] != 0)
        {
            *(u16 *)(c + 0x14) |= 1;
        }
        else
        {
            *(u16 *)(c + 0x14) &= 0xFFFE;
        }
        if ((*(s32 *)(q + 0x1C) & 0x20) == 0)
        {
            if ((*(u16 *)(c + 0x14) & 0x40) == 0)
            {
                func_800478B8(c);
            }
        }
        else
        {
            *(u16 *)(c + 0x14) |= 0x7000;
        }
        func_800A020C(*(s32 *)(q + 0x1C), c + 0xC);
    }
    else if ((*(u16 *)(c + 0x14) & 0x800) != 0)
    {
        *(u16 *)(c + 0x14) &= 0x8FFF;
    }
    else
    {
        *(u16 *)(c + 0x14) |= 0x7000;
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    if (p[0x9A] == 0xE)
    {
        *(s16 *)(p + 0xA0) = *(u16 *)(p + 0xA0) - 1;
        if (*(s16 *)(p + 0xA0) == 2)
        {
            *(u16 *)(p + 0x98) |= 1;
            t = D_800E260C;
            *(u8 **)(c + 0x2C) = t;
            func_8003DB94(c, *(void **)(t + (((D_80083228 + *(s16 *)(q + 0x2A) + 0x100) >> 7) & 0x1C)), 0);
        }
        if (*(s16 *)(p + 0xA0) == 0)
        {
            b->unk_14 = 0xFFE00000;
        }
        if (*(s16 *)(p + 0xA0) >= -7 && *(s16 *)(p + 0xA0) <= 0)
        {
            b->unk_14 += 0x40000;
        }
        else
        {
            b->unk_14 = 0;
        }
        if (*(s16 *)(p + 0xA0) < -7)
        {
            *(u16 *)(p + 0x98) &= 0xFFFE;
            *(s16 *)(p + 0xA0) = (rand() & 0x3F) | 0x40;
        }
    }
    else
    {
        *(u16 *)(p + 0x98) &= 0xFFFE;
    }
    if ((*(u16 *)(p + 0x98) & 0x8) != 0)
    {
        p[0x9D] = 0;
    }
    else
    {
        b->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
        p[0x9D]++;
    }
    *(s32 *)(p + 0x90) += b->unk_14;
    if ((*(u16 *)(p + 0x98) & 0x4) == 0)
    {
        r = func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(u16 *)(q + 0x88) - 0x20);
        if ((s16)r < 0x200)
        {
            if (*(s16 *)(p + 0x92) + *(s16 *)(q + 0x88) < (s16)r)
            {
                *(u32 *)(q + 0x1C) &= 0xF7FFFFFF;
            }
            else if ((s16)r >= *(s16 *)(q + 0x88))
            {
                *(s32 *)(p + 0x90) = 0;
                b->unk_14 = 0;
                *(u32 *)(q + 0x1C) |= 0x8000000;
                p[0x9D] = 0;
            }
            else
            {
                b->unk_14 = 0;
                *(u32 *)(q + 0x1C) |= 0x8000000;
                p[0x9D] = 0;
                *(s16 *)(p + 0x92) = r - *(u16 *)(q + 0x88);
            }
            if ((*(u32 *)(q + 0x1C) & 0x40000000) != 0)
            {
                *(u32 *)(q + 0x1C) &= 0xBFFFFFFF;
                r = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, *(u16 *)(q + 0x88) - 0x20);
                *(u16 *)(p + 0x92) += *(u16 *)(q + 0x88) - r;
                *(u16 *)(q + 0x88) = r;
            }
        }
        else
        {
            *(u32 *)(q + 0x1C) &= 0xF7FFFFFF;
        }
    }
    else
    {
        *(u32 *)(q + 0x1C) &= 0xF7FFFFFF;
    }
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(q + 0x88) + *(u16 *)(p + 0x92);
    *(u16 *)(c + 0x14) |= 0x40;
}

/* The +0x8C handler at the head of the 0x800D8C64 machine: the per-frame
   update that func_800DA398 / func_800D991C / func_800DA660 and nine other
   tails hand the actor back to.  Four early exits on the D_80083462 flag
   word and d+0x1C's bits, the +0x9A state byte set to 0xE, and the
   D_800E262C facing re-dress -- written out at all three sites, since
   cross-jumping merges the first and the last into the shared tail by
   itself.

   THE func_8009FB34 RESULT MUST LAND IN AN `s16` LOCAL, NOT AN `s32`.
   The listing stores it with a bare `addu $v1, $v0, $zero` / `sb` and only
   sign-extends LATER, at the `(s8)n` room index.  Through an `s32 n` the
   conversion is paid at the assignment instead -- `sll 16` / `sra 16`
   before the `sb` -- and that is 251 words, 106 differing (every branch
   offset past it shifts by one).  With `s16 n` it is 250 words, 0
   differing on the first compile.  (docs/matching-levers.md, "A CALL
   RESULT'S DECLARED TYPE DECIDES WHERE ITS `sll`/`sra` LANDS".)

   250 words, 0 differing, seqcheck seq=OK arith=OK. */
void func_800D8C64(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;
    s32 v;
    u16 x;

    if ((D_80083462 & 0x1000) != 0)
    {
        p[0x9A] = 0xE;
        func_800D904C(p, b, c, d);
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C((UnkDungeonActor *)p, b, c, d);
        if (*(u8 **)(c + 0x2C) != (u8 *)D_800E262C)
        {
            *(u8 **)(c + 0x2C) = (u8 *)D_800E262C;
            func_8003DB94(c, D_800E262C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == (u8 *)D_800E262C)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, NULL) != 0)
        {
            return;
        }
    }
    if ((D_80083462 & 0x2000) == 0)
    {
        if ((*(s32 *)(d + 0x1C) & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if (p[0x9A] != 0xE)
        {
            if (*(u8 **)(c + 0x2C) != (u8 *)D_800E262C)
            {
                *(u8 **)(c + 0x2C) = (u8 *)D_800E262C;
                func_8003DB94(c, D_800E262C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                              0);
            }
            p[0x9A] = 0xE;
        }
        *(u16 *)(p + 0x98) &= ~0xC;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, NULL) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            *(s32 *)(p + 0x90) = 0;
            *(u16 *)(p + 0x98) &= ~1;
            b->unk_14 = 0;
            func_800AA888(p, b, c, d);
            func_800DA660((UnkDungeonActor *)p, b, c, d);
            return;
        }
        if ((s16)func_800A1C58(d) != 0)
        {
            func_800AAB10(p, b, c, d);
        }
    }
    n = func_8009FB34(c[0x24], c[0x25]);
    c[0x26] = n;
    if (*(s8 *)(d + 0x6D) > 0)
    {
        if ((*(s32 *)(d + 0x1C) & 0x20) != 0)
        {
            func_800A9A0C(d);
            return;
        }
        func_800D92C0(p, b, c, d);
        return;
    }
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x2000) == 0)
    {
        if ((s8)n < 0 || (D_800E2970_s[(s8)n].unk_0C & 2) == 0)
        {
            if ((v & 0x430) == 0)
            {
                if ((s16)func_8009FD7C(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25]) != 0)
                {
                    *(s16 *)(d + 0x2A) = func_800A0818(c[0x24], c[0x25], D_80082E80[0x24],
                                                       D_80082E80[0x25], &x);
                }
            }
        }
    }
    if ((D_80083462 & 0x2000) != 0)
    {
        return;
    }
    if ((*(u16 *)(c + 0x14) & 0x40) != 0)
    {
        return;
    }
    if (*(u8 **)(c + 0x2C) == (u8 *)D_800E262C)
    {
        return;
    }
    if ((*(u16 *)(p + 0x98) & 1) != 0)
    {
        return;
    }
    *(u8 **)(c + 0x2C) = (u8 *)D_800E262C;
    func_8003DB94(c, D_800E262C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

/* The actor's walk step: re-skin the renderable from D_800E260C when the
   view changed (or when its +0x14 still carries 0x6000), step the engine's
   0x20-radius move over the path byte pair at d + *(s16 *)(d + 0x8A), bump
   that cursor, and either stop at state 0xF or hand on to func_800D9820.
   157 words, 0 differing.

   THREE SPELLING FACTS, each measured on this function.
   (1) `c[0x24]` and `c[0x25]` ARE TWO LOCALS, read once and passed to both
   func_8009A3D0 and func_800A0818. Read inline at each call they are
   reloaded after the first call (a call invalidates cse's memory table):
   156 words, 118 differing.
   (2) The row address is BASE-FIRST, `*(d + *(s16 *)(d + 0x8A) + 0x74)`.
   `d[*(s16 *)(d + 0x8A) + 0x74]` is an index-first `addu $v0, $v0, $s1`
   where the target has `addu $v0, $s1, $v0` -- three sites.
   (3) The divisor guard needs its own `s32` local: `if (d[0x71] > 0)` on
   the `u8 *` is folded to `!= 0`, which is a `beqz` plus a register copy
   where the target has a bare `blez` -- 158 words. Assigning `n = d[0x71]`
   first keeps the comparison signed on an SImode pseudo. The second entry
   test is `d[0x71] > *(s16 *)(d + 0x8A)`, not `*(s16 *)(d + 0x8A) <
   d[0x71]`: the operands' load order is visible and the other way round is
   2 differing.

   DEVICE: `x + ((x + x) - x - x)` on func_800A0818's first argument. It is
   the phantom zero of docs/matching-levers.md :11189 -- cse keeps the three
   insns, flow counts them as references on `x`, and combine deletes them
   (simplify_binary_operation's `X * C - X` and `X - X` cases), so it emits
   nothing and ties nothing. Its job here is purely allocation: without it
   `d` outranks `x` in global-alloc and the two swap hard registers, `d`
   taking $s0 and `x` $s1 where the target has $s1 and $s0 -- 157 words, 29
   differing, every one of them that rename (and `t`, which shares `x`'s
   register, renames with it). What the function does without it: the same
   work in the same instructions, with two callee-saved registers
   exchanged. It does NOT warn under -Wall: nothing is read uninitialised.
   Measured alternatives, all 157 words unless stated:
     `__asm__("" ::"r"(x));` after the two assignments  -- 2 differing: it
       buys the same rename but, being a real insn until sched, makes the
       `lbu 0x24` the block's critical path and swaps it ahead of the
       `lw 0x1C` (docs/matching-levers.md :14047).  Four placements of that
       marker (between x and y, on y as well, on d as well, before the
       assignments) are 2, 158/117, 2 and 47.
     the phantom on func_8009A3D0's first argument instead -- 7 differing.
     the phantom as its own statement, `x = x + ((x + x) - x - x);` -- 2.
     `u8 x, y` 158/117; `s16 x, y` 29; declaring x and y first 29; dropping
       the `e` local 29; a block-scoped `v` per arm 29; inlining `v` 29.
     merging `t` into `x` 54; merging `r` into `y` 158/100; both 158/110.
     a local for the 0x300/0x3000 select 158/117; duplicating the
       func_8009A3D0 call into the two arms 160/116.
*/
void func_800D904C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *e;
    s32 r;
    s32 t;
    s32 n;
    s32 v;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        e = *(u8 **)(c + 0x2C);
        if (e != D_800E260C)
        {
            *(u8 **)(c + 0x2C) = D_800E260C;
            v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
            func_8003DB94(c, ((u8 **)D_800E260C)[v], 0);
        }
        else if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
            func_8003DB94(c, ((u8 **)e)[v], 0);
        }
        x = c[0x24];
        y = c[0x25];
        func_8009A3D0(x, y, (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        r = func_800A0818(/* DEVICE: the phantom zero. See the header. */
                          x + ((x + x) - x - x), y, *(d + *(s16 *)(d + 0x8A) + 0x74), *(d + *(s16 *)(d + 0x8A) + 0x7C),
                          (u16 *)(p + 0x98));
        t = func_8009A66C((s16)r, c, d, 0x20);
        c[0x24] = *(d + *(s16 *)(d + 0x8A) + 0x74);
        c[0x25] = *(d + *(s16 *)(d + 0x8A) + 0x7C);
        *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) + 1;
        func_8009A21C(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(s16 *)(d + 0x2A) = r;
        if ((s16)t == 3 && ((D_80083462 & 0x80) != 0 || (*(u16 *)(c + 0x14) & 0x8000) != 0))
        {
            p[0x9A] = 0xF;
        }
        else
        {
            func_800D9820(p, b, c, d);
        }
        *(s32 *)(p + 0x8C) = 0;
        *(u32 *)(d + 0x1C) |= 0x40000000;
        if ((D_80083462 & 0x80) != 0)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        else
        {
            *(s16 *)(p + 0x96) = 8;
            n = d[0x71];
            if (n > 0)
            {
                *(s16 *)(p + 0x96) = 8 / n;
            }
        }
    }
}

/* 344 words, 0 differing.

   THE TABLE ADDRESS IS A LOCAL THAT THE INDEX ADD WRITES BACK INTO, so the
   `%lo' insn's destination has reg_n_sets != 1, `birthing_insn_p'
   (sched.c:2512) fails on it, and the `%hi'/`%lo' pair is no longer forced to
   sit immediately before its consumer -- it floats to the top of the block,
   which is the only thing that lets QTY_CMP_PRI give the c[0x24] load $v0 and
   the address chain $v1 the way the target has them.  Written with the lookup
   inline (`c[0x24] += *(u8 *)(kk + (s32)D_8006CCD8)') it is 8 aligned
   differences, and with a cancelled `t1++; t1--;' pair buying the second set
   instead it is 5: combine folds the pair, re-places a REG_DEAD note on a
   `use' insn it inserts at the ternary's join label, and the second death
   drops the local out of local-alloc (local-alloc.c:472) so the chain never
   becomes one quantity.  A read-modify-write set costs no instruction and
   makes no second death (flow.c:2578).  The second lookup must stay inline:
   giving D_8006CCE8 the same local is 343 words, because cse knows
   D_8006CCE8 is D_8006CCD8 + 0x10 and folds the second `%hi'/`%lo' away.

   The four devices are all load-bearing, re-priced by deletion at 0 rows:
   the `cc2' do-while 4, the duplicated arm `if (n)' 3, the
   `do { return; } while (0);' 4, the duplicated arm `if (d)' around
   func_800BCB04 91.  Every condition is definitely assigned where it stands.
 */
void func_800D92C0(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *g2;
    u8 *w;
    s16 *tbl;
    s16 f;
    s16 i;
    s16 m;
    s32 s;
    s32 n;
    u8 *t80;
    s32 j;
    s32 k;
    s32 r;
    s32 v;
    s32 kk;
    s32 t1;
    s32 cc2;
    s32 x;
    s32 y;

    g = (u8 *)&D_80083460;
    f = 0;
    if ((*(u16 *)(g + 0x2) & 0x4000) != 0 || *(s8 *)(d + 0x71) >= 0)
    {
        func_800A9A0C(d);
        return;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
    {
        return;
    }
    w = p + 0x9C;
    func_800A19E4((s32)c, d, 3, 6, w);
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x410) != 0)
    {
        if ((v & 0x400) != 0)
        {
            cc2 = 0x80000000;
            if (*(s32 *)(d + 0x14) >= 0)
            {
                do
                {
                    *(s32 *)(d + 0x14) |= cc2;
                } while (0);
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + ((func_800A6D30() & 7) << 9);
            }
        }
    }
    else
    {
        if (*(s8 *)(d + 0x71) >= 0)
        {
            return;
        }
        if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
        {
            return;
        }
        func_800A19E4((s32)c, d, 3, 6, w);
        v = *(s32 *)(d + 0x1C);
        if ((v & 0x2000) != 0)
        {
            if ((v & 0x20000) != 0)
            {
                t80 = D_80082E80;
                k = ((d[0x45] + ((s16)(*(u16 *)(D_800814A8 + 0x2A)) >> 9)) & 7) * 2;
                x = t80[0x24] + *(u16 *)(k + (s32)D_8006CCD8);
                y = t80[0x25] + *(u16 *)(k + (s32)D_8006CCE8);
                if (c[0x24] == (u16)x && c[0x25] == (u16)y)
                {
                    d[0x71] &= 0x7F;
                    return;
                }
                m = func_800A0818(c[0x24], c[0x25], x, y, (u16 *)(p + 0x98));
                *(s16 *)(d + 0x2A) = m;
                if ((s16)func_8009A66C(m, c, d, 0x20) <= 0)
                {
                    *(s16 *)(d + 0x2A) = func_800A0818(c[0x24], c[0x25], D_80082E80[0x24],
                                                       D_80082E80[0x25], (u16 *)(p + 0x98));
                }
                if (func_8009FD7C(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25]) != 0)
                {
                    f = 1;
                }
            }
            else
            {
                func_800A0E6C(c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
        else
        {
            j = *(s8 *)(c + 0x26);
            if (j >= 0 && (D_800E2970_s[j].unk_0C & 2) != 0)
            {
                func_800A0E6C(c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
            else if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_800A0E6C(c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
    }
    i = 0;
    tbl = D_8006CD00;
    do
    {
        n = *(s16 *)(d + 0x2A);
        if ((*(u16 *)(p + 0x98) & 2) != 0)
        {
            s = n - tbl[i];
        }
        else
        {
            s = n + tbl[i];
        }
        if (n)
        {
            r = func_8009A66C((s16)s, c, d, 0x20);
        }
        else
        {
            r = func_8009A66C((s16)s, c, d, 0x20);
        }
        if ((s16)r > 0)
        {
            if (i >= 3 && f != 0)
            {
                d[0x71] &= 0x7F;
                return;
            }
            *(s16 *)(d + 0x2A) = s;
            *(d + (d[0x71] & 0x7F) + 0x74) = c[0x24];
            *(d + (d[0x71] & 0x7F) + 0x7C) = c[0x25];
            d[0x71]++;
            func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            kk = (*(u16 *)(d + 0x2A) >> 8) & 0xE;
            t1 = (s32)D_8006CCD8;
            t1 += kk;
            c[0x24] += *(u8 *)t1;
            c[0x25] += *(u8 *)(kk + (s32)D_8006CCE8);
            func_8009A21C(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            break;
        }
        if (i == 0 && *(u16 *)(D_80082E80 + 0x24) != *(u16 *)(c + 0x24))
        {
            if ((s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                do
                {
                    return;
                } while (0);
            }
        }
        i++;
    } while (i < 8);
    if (i >= 8)
    {
        d[0x71] &= 0x7F;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A9A0C(d);
    }
    else
    {
        p[0x9C] = c[0x26];
        d[0x6D]--;
        g2 = (u8 *)&D_80083460;
        *(u16 *)(g2 + 0x8) = *(u16 *)(g2 + 0x8) + 1;
        if (*(s8 *)(d + 0x6D) == 0)
        {
            d[0x71] &= 0x7F;
            return;
        }
        if (d)
        {
            i = func_800BCB04(c[0x24] * 64 | 0x20, c[0x25] * 64 | 0x20,
                              *(u16 *)(d + 0x88) - 0x20);
        }
        else
        {
            i = func_800BCB04(c[0x24] * 64 | 0x20, c[0x25] * 64 | 0x20,
                              *(u16 *)(d + 0x88) - 0x20);
        }
        if (i < 0x200)
        {
            *(s16 *)(d + 0x88) = i;
        }
    }
}

/* State 0x10, and hand c the D_800E260C entry the facing at d+0x2A selects.
   The table base is a POINTER LOCAL because it is used twice -- stored at
   c+0x2C and indexed -- and the index is a byte offset (`andi 0x1C` after the
   shift), so it is added to the `u8 *` rather than written as a subscript.
   25 words, 0 differing. */
void func_800D9820(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    t = D_800E260C;
    *(u8 **)(c + 0x2C) = t;
    func_8003DB94(c, *(void **)(t + (((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 7) & 0x1C)), 0);
}

/* A two-gate installer of func_800D45D0's shape, except that the inner gate's
   failure RETURNS rather than falling into the D_80083462 test: the target's
   `blez` goes to the epilogue while the `beqz` of the first gate goes to the
   flag test. 38 words, 0 differing. */
void func_800D9884(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_800D8C64;
        *(s32 *)(p + 0x90) = 0;
    }
    if ((D_80083462 & 0x80) != 0)
    {
        *(s16 *)(p + 0x92) = 0;
    }
}

/* 191 words, 0 differing. THE SPELLING FACT: the two numerators are NOT
   written the same way. `(c[0x24] << 6) - (*(s16 *)((u8 *)b + 0x2) - 0x20)`
   is reassociated by fold() into `((c[0x24] << 6) + 0x20) - b2`, emitting
   `addiu $v0, $v0, 32` where the target has `addiu $v1, $v1, -0x20`.
   Breaking the fold with a temp gives the right insn but the WRONG LOAD
   ORDER (`lh` before `lbu`): with the fold broken both operand chains are
   the same depth and the scheduler's tie-break puts the `lh` first. Only
   the three-statement form `u = c[0x24] << 6; w = b2 - 0x20; u -= w;` gets
   both -- `u -= w` is its own statement so nothing folds, and the
   `u = c[0x24] << 6` statement makes the lbu chain win the schedule.
   `u = c[0x24];` is NOT enough: combine folds it into the `<< 6` and the
   `lbu` sinks again (4 differing). THE SECOND NUMERATOR MUST STAY
   TWO-STATEMENT -- there the `lh` has the mflo bubble to fall into, and the
   extra statement costs 1 word and 145 differing. Nine spellings measured,
   1 to 4 differing each.

   THE TAIL GUARD IS THREE NESTED `if`s, NOT AN `&&` CHAIN. `fold_truthop`
   merges chained bit tests on one operand, so
   `(v & 0x2000) == 0 && (v & 0x410) == 0 && (v & 0x20000) != 0` collapses to
   a single `(v & 0x22410) != 0x20000` and the function comes out 3 words
   short.

   The cases FALL THROUGH into one another and each bumps +0x9B separately;
   the shared tail is the code after the switch, which the default arm
   reaches by `j`. func_800DA398 looks like a twin and is not: its
   `p[0x9B] += 1;` tail has to be a `goto` label inside the switch. */
void func_800D991C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 u;
    s32 w;
    s32 v;
    u16 x;

    switch (p[0x9B])
    {
    case 0:
        *(u16 *)(p + 0x98) |= 8;
        b->unk_14 = 0xFFF00000;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s32 *)(p + 0xA4) = 0;
        p[0x9B] += 1;
    case 1:
        *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xA4);
        if (*(s16 *)(p + 0x96) != 0)
        {
            u = c[0x24] << 6;
            w = *(s16 *)((u8 *)b + 0x2) - 0x20;
            u -= w;
            b->unk_0C = (u << 16) / *(s16 *)(p + 0x96);
            w = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - w) << 16) / *(s16 *)(p + 0x96);
            *(s32 *)(p + 0xA4) += b->unk_14;
            b->unk_14 += 0x48000;
        }
        *(s32 *)(p + 0x90) += *(s32 *)(p + 0xA4);
        if (*(s16 *)(p + 0x96) < 2)
        {
            *(s32 *)(p + 0x90) = 0;
            *(u16 *)(p + 0x98) &= ~8;
            *(s32 *)(d + 0x1C) |= 0x8000000;
            p[0x9B] += 1;
        }
    case 2:
        if ((*(s32 *)(d + 0x1C) & 0x8000000) != 0)
        {
            *(u16 *)(p + 0x98) &= ~8;
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            *(u8 **)(c + 0x2C) = (u8 *)D_800E262C;
            func_8003DB94(c, D_800E262C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            p[0x9B] += 1;
        }
        break;
    }

    if ((*(s16 *)(p + 0x96) -= 1) > 0)
    {
        return;
    }
    b->unk_14 = 0;
    b->unk_10 = 0;
    b->unk_0C = 0;
    func_800A2B04(b, c[0x24], c[0x25]);
    func_800AD594(d, 4);
    func_800A4ACC(d);
    g = (u8 *)&D_80083460;
    if (*(s16 *)(g + 0x8) != 0)
    {
        *(u16 *)(g + 0x8) -= 1;
    }
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x2000) == 0)
    {
        if ((v & 0x410) == 0)
        {
            if ((v & 0x20000) != 0)
            {
                *(s16 *)(d + 0x2A) =
                    func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
            }
        }
    }
    if ((s16)func_800AD9B4(c, d) > 0)
    {
        *(u32 *)(p + 0x8C) = (u32)func_800D8C64;
        func_800A9A04(d);
    }
}

void func_800D9C18(UnkDungeonActor *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            p->unk_8C = func_800D8C64;
        }
    }
}

/* The 0x800D1C70..0x800D4634 state machine again, cut from func_800D21CC /
   func_800D29D8: same three-case switch, same `stop:` fallthrough into
   func_800AAA54, same D_80083460+0x10 / handler-install tail. Four things
   differ from that pair: the D_8006CCD8 index is a LOCAL (it is read in both
   case 0 and case 1, and the listing holds it in $s4 across the call), case 0
   scales by 18 and case 1 by 14 instead of 15, case 0's step count is a
   predicate on d+0x1C rather than a constant 8, and case 2 scales by
   `<< 15 >> 1` rather than dividing by the step count. 192 words, 0
   differing.

   `if (a->unk_96 > 0) a->unk_96--;` and `if (--a->unk_96 > 0)` are NOT
   interchangeable and this function uses both: the separate-compare form in
   case 1 (`lh` + `lhu`), the pre-decrement form in case 2
   (`lhu`/`addiu`/`sh`/`sll 16`/`bgtz`). */
void func_800D9C7C(UnkDungeonActor *a, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 goal;
    s32 now;
    s32 k;

    k = (*(u16 *)(d + 0x6A) >> 9) & 7;
    switch (a->unk_9B)
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = D_8006CCD8[k] << 18;
        b->unk_10 = D_8006CCE8[k] << 18;
        a->unk_9B++;
        if (d[0x28] != 0)
        {
            if (*(u16 *)(c + 0x14) & 0x8000)
            {
                a->unk_96 = 0;
                a->unk_9B = 2;
                return;
            }
            a->unk_96 = (*(s32 *)(d + 0x1C) & 0x228) ? 8 : -1;
            b->unk_0C -= b->unk_0C / 4;
            b->unk_10 -= b->unk_10 / 4;
        }
        else
        {
            goto stop;
        }
        /* fallthrough */
    case 1:
        b->unk_0C -= D_8006CCD8[k] << 14;
        b->unk_10 -= D_8006CCE8[k] << 14;
        if (a->unk_96 > 0)
        {
            a->unk_96--;
        }
        else if (*(u16 *)(c + 0x14) & 0x6000)
        {
            a->unk_96 = 0;
        }
        if (a->unk_96 != 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
        stop:
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(a, b, c, 0);
        }
        else
        {
            a->unk_96 = 8;
            a->unk_9B++;
        }
        return;
    case 2:
        if (a->unk_96 != 0)
        {
            goal = c[0x24] << 6;
            now = *(s16 *)((u8 *)b + 0x2) - 0x20;
            goal -= now;
            b->unk_0C = (goal << 15) >> 1;
            goal = c[0x25] << 6;
            now = *(s16 *)((u8 *)b + 0x6);
            now -= 0x20;
            goal -= now;
            b->unk_10 = (goal << 15) >> 1;
        }
        if (--a->unk_96 > 0)
        {
            return;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        a->unk_8C = func_800D8C64;
        return;
    }
}

void func_800D9F7C(void)
{
    func_800AD058();
}

void func_800D9F9C(void)
{
    func_800AB538();
}

void func_800D9FBC(void)
{
    func_800AB778();
}

void func_800D9FDC(UnkDungeonActor *p)
{
    if (func_800ABDBC() != 0)
    {
        p->unk_8C = func_800D8C64;
    }
}

/* The 0x800D8C64 machine's OTHER three-state tail, state index at +0x9B --
   func_800DA398 below is the same switch over the same record, so read its
   comment first. Two things differ here and both are measured:

   NO `goto`: EVERY ARM WRITES ITS OWN `p[0x9B] += 1; return;` AND ITS OWN
   `tail`, and cross-jumping puts the single copies back. Writing state 0's
   bump as `goto bump` into state 1's -- which is func_800DA398's shape, and
   the shape the listing's `j .L800DA31C` looks like -- is 223 words with
   172 differing, and it is wrong TWICE over. (1) jump2 then merges state 0's
   `sh $v0, 0xA($v1)` with state 1's identical one instead, which costs the
   `sh` and the load-delay `nop` the target has. (2) the duplicate bump is
   two more references on `p`: at 10 refs over 312 insns `p` scores
   3*10/312 = 0.096 against the `&D_80083460` local's 1*3/30 = 0.100, loses
   the allocno_compare tie and takes $s4 while that local takes $s3 -- the
   whole function's saved registers come out swapped. At 12 refs p scores
   0.115 and the order is the target's. Eleven other spellings of the
   increment alone (postfix, prefix, a value local, a `u16 *` k, declaration
   order, an earlier assignment) all stayed at 223/172: the reference count
   was the lever, not the increment.

   State 1's `&= ~0x200` is NOT func_800DA398's `~0x208`, and state 1 reaches
   that tail when the renderable's +0x14 already carries 0x8000.
   225 words, 0 differing. */
void func_800DA014(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *k;
    u8 *m;
    u8 *e;
    s32 v;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = (u8 *)D_800E262C;
        func_8003DB94(c, D_800E262C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        p[0x9B] += 1;
        return;
    case 1:
        g = (u8 *)&D_80083460;
        if ((*(u16 *)(g + 0x2) & 0x1000) != 0)
        {
            return;
        }
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, NULL) != 0)
        {
            return;
        }
        if (d[0x25] == 0)
        {
            if ((*(u16 *)(g + 0x2) & 0x2008) != 0)
            {
                return;
            }
            func_800AA79C((UnkDungeonActor *)p, b, c, d);
            return;
        }
        if ((s16)func_800A2C34(d) != 0)
        {
            return;
        }
        v = *(s32 *)(d + 0x1C);
        if ((v & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if ((v & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_800DA660((UnkDungeonActor *)p, b, c, d);
            return;
        }
        if (*(s8 *)(d + 0x6D) == 0)
        {
            return;
        }
        if ((s16)func_800A2C34(d) != 0 &&
            (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
        {
            return;
        }
        func_800A9A0C(d);
        func_800A9A04(d);
        if ((s16)func_80042900((s8 *)d, 1) != 0)
        {
            e = D_80082E80;
            if (*(s8 *)(c + 0x26) != *(s8 *)(e + 0x26) || *(s8 *)(c + 0x26) < 0)
            {
                if ((s16)func_8009FD40(e, c) >= 2)
                {
                    goto again;
                }
            }
            if ((func_800A6D30() & 7) == 0)
            {
                func_80042B68((s8 *)d, 1);
            }
        }
    again:
        if ((s16)func_80042900((s8 *)d, 1) != 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = (u8 *)D_800E262C;
        func_8003DB94(c, D_800E262C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s32 *)(d + 0x1C) &= ~0x200;
            *(u32 *)(p + 0x8C) = (u32)func_800D8C64;
            return;
        }
        k = (u8 *)&D_80083460;
        *(u16 *)(k + 0xA) += 1;
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        m = (u8 *)&D_80083460;
        *(u16 *)(m + 0xA) -= 1;
        *(s32 *)(d + 0x1C) &= ~0x200;
        *(u32 *)(p + 0x8C) = (u32)func_800D8C64;
        return;
    }
}

/* The three-state tail of the 0x800D8C64 machine, switching on the state
   index at +0x9B: state 0 re-dresses the renderable from the D_800E262C
   facing table, state 1 is the long refusal chain, and state 2 hands the
   actor back to func_800D8C64.

   THE `p[0x9B] += 1;` TAIL IS A `goto` LABEL INSIDE THE SWITCH, NOT A
   STATEMENT AFTER IT.  Written after the switch with `break` in states 0
   and 1 it is laid out LAST, after state 2's body, and that is 178 words
   with 148 differing -- the whole function shifts by one word from the
   switch tree onwards.  Inside the switch, between state 1's body and
   state 2's, it lands where the target has it and everything else falls
   into place.  (docs/matching-levers.md, "WITH NO `default`, A SHARED TAIL
   MUST BE A `goto` LABEL INSIDE THE SWITCH".)

   `&D_80083460` is THREE separate pointer locals, one per state: sharing
   one makes it a global allocno and states 0 and 2 then use the
   callee-saved $s4 the target only gives state 1.

   func_800A9A04 had to be WIDENED from `void func_800A9A04(void)` to take
   a `u8 *`: this is its only caller anywhere and it sets $a0 before the
   `jal`.  The parameter is unread, so that function is still 2 words,
   0 differing.

   178 words, 0 differing, seqcheck seq=OK arith=OK. */
void func_800DA398(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *k;
    s32 v;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        *(u8 **)(c + 0x2C) = (u8 *)D_800E262C;
        func_8003DB94(c, D_800E262C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        goto bump;
    case 1:
        g = (u8 *)&D_80083460;
        if ((*(u16 *)(g + 0x2) & 0x1000) != 0)
        {
            return;
        }
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, NULL) != 0)
        {
            return;
        }
        if ((s16)func_800A2C34(d) != 0)
        {
            return;
        }
        v = *(s32 *)(d + 0x1C);
        if ((v & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if ((v & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_800DA660((UnkDungeonActor *)p, b, c, d);
            return;
        }
        if (*(s8 *)(d + 0x6D) == 0)
        {
            return;
        }
        if ((s16)func_800A2C34(d) != 0 &&
            (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
        {
            return;
        }
        func_800A9A0C(d);
        func_800A9A04(d);
        if (d[0x25] == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = (u8 *)D_800E262C;
        func_8003DB94(c, D_800E262C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(u16 *)(g + 0xA) += 1;
    bump:
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            k = (u8 *)&D_80083460;
            *(u16 *)(k + 0xA) -= 1;
            *(s32 *)(d + 0x1C) &= ~0x208;
            *(u32 *)(p + 0x8C) = (u32)func_800D8C64;
        }
        return;
    }
}

void func_800DA660(UnkDungeonActor *p, void *b, void *c, void *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            p->unk_8C = func_800D8C64;
        }
    }
}

void func_800DA6C4(UnkDungeonActor *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            p->unk_8C = func_800D8C64;
        }
    }
}

/* FOUR PARAMETERS, forwarded. func_800AC480 reads all four argument
   registers; this caller only passes them through, so the wrong arity was
   invisible until that function was read. 14 words, 0 differing either way. */
void func_800DA728(UnkDungeonActor *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AC480((u8 *)p, b, c, d) != 0)
    {
        p->unk_8C = func_800D8C64;
    }
}

void func_800DA760(UnkDungeonActor *p)
{
    if (func_800AC5C8() != 0)
    {
        p->unk_8C = func_800D8C64;
    }
}

/* FOUR PARAMETERS, forwarded, exactly as the func_800AC480 pair next door:
   func_800ACE34 reads $a0 and $a2 and this caller only passes them through.
   14 words, 0 differing either way. */
void func_800DA798(UnkDungeonActor *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800ACE34((u8 *)p, b, c, d) != 0)
    {
        p->unk_8C = func_800D8C64;
    }
}

void func_800DA7D0(UnkDungeonActor *p)
{
    if (func_800AB464() > 0)
    {
        p->unk_8C = func_800D8C64;
    }
}

void func_800DA808(UnkDungeonActor *p)
{
    if (func_800AC7EC() > 0)
    {
        p->unk_8C = func_800D8C64;
    }
}

/* A 2x8 nested countdown; the inner `i = 7` reinit sits in the OUTER back
   branch's delay slot, which is what the two nested `for`s give. The source
   object is an alignment-1 8-byte struct, so the copy is the `lwl`/`lwr` +
   `swl`/`swr` pair. 63 words, 0 differing. */
void func_800DA840(UnkDungeonName8 *src, s16 a)
{
    UnkNode *n;
    u8 *q;
    s32 i;
    s32 j;
    s32 t;

    for (j = 1; j >= 0; j--)
    {
        for (i = 7; i >= 0; i--)
        {
            n = func_8003FC64(2);
            if (n != NULL)
            {
                q = (u8 *)n + 0x20;
                *(s16 *)q = 0;
                *(s16 *)(q + 0x2) = 0x20;
                *(s16 *)(q + 0x8) = j;
                t = i << 9;
                if (j == 0)
                {
                    t += 0x100;
                }
                *(s16 *)(q + 0xA) = t;
                *(UnkDungeonName8 *)(q + 0x20) = *src;
                *(s16 *)(q + 0x6) = GetClut(a << 4, 0x1F8);
                n->unk_10 = (u32)func_800DAB50;
                func_8004491C(n, (u32)func_800DA93C);
            }
        }
    }
}

/* One semi-transparent 32x32 textured quad per node of the tag chain: project
   the four corners at +0x28/+0x30/+0x38/+0x40, average their depths, and hang
   the POLY_FT4 on the OT at that depth. The UV pair comes from the mover's y
   read as a sprite-sheet index -- `y % 8`, then column `% 4` and row `/ 4`,
   each scaled by 32. func_800D5AC0 above is the TILE_1 version of this walk
   and csrc/ovl_town_800BF3F0.c's func_800BFFF4 the LINE_G2 one; both carry
   longer notes on the shape, and the two addPrim masks hoisted into $s3/$s5
   are theirs.

   THE EIGHT UV STORES ARE FOUR CHAINED ASSIGNMENTS, AND THAT IS 8 DIFFERING.
   A chained assignment emits its stores RIGHT-TO-LEFT, which is the target's
   `sb 0x14` before `sb 0xC` and `sb 0x1D` before `sb 0xD`; eight separate
   statements in ascending order give the pairs the other way round at the same
   133 words.

   AND THE u GROUP MUST BE STORED BEFORE THE v IS COMPUTED. `w` is
   address-taken -- it is the out-parameter RotTransPers gets twice -- so the
   target RELOADS it with `lw 0x10($sp)` for the `/ 4`, which only happens
   because the four `sb`s in between wipe cse's memory table. With both the
   `% 4` and the `/ 4` computed before any store (a single `setUV4`), cse keeps
   `w` in a register and the function is 127 words of 133.

   133 words, 0 differing. */
s32 func_800DA93C(u8 *p)
{
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    POLY_FT4 *t;
    s32 z;
    s32 w;
    s32 u;
    s32 v;

    while (1)
    {
        t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)(t + 1);
        z = RotTransPers((SVECTOR *)(p + 0x28), (s32 *)&t->x0, &w, &w);
        z += RotTransPers((SVECTOR *)(p + 0x30), (s32 *)&t->x1, &w, &w);
        z += RotTransPers((SVECTOR *)(p + 0x38), (s32 *)&t->x2, &w, &w);
        z += RotTransPers((SVECTOR *)(p + 0x40), (s32 *)&t->x3, &w, &w);
        z = (z >> 2) - 8;
        if ((u32)z < 0x1E0)
        {
            w = 0xFF - *(s16 *)(p + 0x2);
            *(u32 *)&t->r0 = *(u32 *)(p + 0x10);
            SetPolyFT4(t);
            SetSemiTrans((u8 *)t, 1);
            t->tpage = *(u16 *)(p + 0x4);
            t->clut = *(u16 *)(p + 0x6);
            w = (s16)(*(s16 *)(p + 0x2) % 8);
            u = (w % 4) * 32;
            t->u0 = t->u1 = u - 0x80;
            t->u2 = t->u3 = u - 0x61;
            v = (w / 4) * 32;
            t->v0 = t->v2 = v + 0x40;
            t->v1 = t->v3 = v + 0x5F;
            addPrim((u32 *)(g->unk_00 + 0xB0) + z, t);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
    }
    return 0;
}

void func_800DAB50(u8 *p)
{
    u8 rec[0x30];
    s32 i;
    s32 off;
    s32 x;
    s32 y;
    s32 d;

    *(s16 *)(rec + 0x28) = 0;
    *(s16 *)(rec + 0x20) = 0;
    if (*(s16 *)(p + 8) != 0)
    {
        *(s16 *)(rec + 0x22) = 0x30;
        *(s16 *)(rec + 0x2A) = *(u16 *)(p + 2) + 0x30;
    }
    else
    {
        *(s16 *)(rec + 0x22) = (*(u16 *)(p + 2) << 1) + 0x40;
        *(s16 *)(rec + 0x2A) = 0x30;
    }
    i = 0;
    *(s16 *)(rec + 0x24) = func_800644B8((*(s16 *)(p + 2) - 0x20) << 6) >> 6;
    *(u8 **)rec = rec + 0x20;
    *(s16 *)(rec + 0x2C) = 0;
    *(s16 *)(rec + 0x18) = 2;
    *(s16 *)(rec + 0xC) = *(u16 *)(p + 0xA);
    off = 0x28;
    *(s32 *)(rec + 8) = 0;
    *(s32 *)(rec + 0x14) = 0;
    *(s32 *)(rec + 0x10) = 0;
    *(s16 *)(rec + 0x1A) = 0;
    for (; i < 2; i++)
    {
        *(u8 **)(rec + 4) = p + off;
        func_800DBA90(rec);
        *(u16 *)(rec + 0xC) += 0x200;
        off += 0x10;
    }
    for (i = 3; i >= 0; i--)
    {
        *(u16 *)(p + i * 8 + 0x28) += *(u16 *)(p + 0x20);
        *(u16 *)(p + i * 8 + 0x2A) += *(u16 *)(p + 0x22);
        *(u16 *)(p + i * 8 + 0x2C) += *(u16 *)(p + 0x24);
    }
    switch (*(s16 *)p)
    {
    case 0:
        *(s32 *)(p + 0x10) = 0;
        *(s16 *)(p + 4) = GetTPage(0, 1, 0x2C0, 0x100);
        *(s32 *)(p + 0xC) = 0x242424;
        *(u16 *)p += 1;
    case 1:
        *(s32 *)(p + 0x10) += *(s32 *)(p + 0xC);
        if (p[0x13] != 0)
        {
            *(s16 *)(p + 4) = GetTPage(0, 3, 0x2C0, 0x100);
            *(s32 *)(p + 0x10) = 0xFFFFFF;
            *(s32 *)(p + 0xC) = 0x101010;
            *(u16 *)p += 1;
        }
        break;
    case 2:
        *(s32 *)(p + 0x10) -= *(s32 *)(p + 0xC);
        if (p[0x10] == 0x7F)
        {
            *(s16 *)(p + 4) = GetTPage(0, 3, 0x2C0, 0x100);
            *(s32 *)(p + 0x10) = 0xA0A0A0;
            *(s32 *)(p + 0xC) = 0x101010;
            *(u16 *)p += 1;
        }
        break;
    case 3:
        *(s32 *)(p + 0x10) -= *(s32 *)(p + 0xC);
        if (*(s32 *)(p + 0xC) > 0x80808)
        {
            *(s32 *)(p + 0xC) = *(s32 *)(p + 0xC) - 0x10101;
        }
        if (*(s32 *)(p + 0x10) < 0)
        {
            *(s32 *)(p + 0x10) = 0;
            *(u16 *)(p - 2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    x = *(u16 *)(p + 2);
    i = ((s16)x << 2) + ((s16)x >> 1);
    /* ONE load of p+0xA, before the test, into an `s32` local of its own.
       Loading it inside each arm gives the right word count but two
       `lh 0xA($s2)` where the target has one, and lets sched1 lift the
       `lh 0x8($s2)` into the load-delay slot of `lhu 0x2($s2)`: 11 differing.
       With the load hoisted, the three-deep sll/sra/sra/addu chain is the
       only thing ready and both the load-delay slot and the `bnez` delay
       slot stay empty, which is the target's pair of nops. The local must
       not be `x` (reusing it splits the `lhu`/`sll` pair across $v1 and $v0,
       2 differing) and must not be `s16` (191 words). */
    d = *(s16 *)(p + 0xA);
    if (*(s16 *)(p + 8) == 0)
    {
        y = d - i;
    }
    else
    {
        y = d + i;
    }
    *(s16 *)(p + 0xA) = y;
    *(s16 *)(p + 2) = *(u16 *)(p + 2) - 1;
}

/* Spawn the 0x312 effect at the mover's position. THE NODE POINTER IS BUMPED
   IN PLACE -- `n += 0x20` -- rather than taken into a second local: the
   target's `addiu $s0, $s0, 0x20` reuses the one register, which two locals
   do not share. The three `n->unk_08` reads are written out rather than
   cached. 44 words, 0 differing. */
void func_800DAE44(u8 *p, s16 v)
{
    u8 *n;

    n = (u8 *)func_8003FD64(0x312, D_80083498);
    if (n != NULL)
    {
        *(u32 *)(n + 0x10) = (u32)func_800DAEF4;
        *(s16 *)(*(u8 **)(n + 0x8) + 0x2) = *(u16 *)(p + 0x2);
        *(s16 *)(*(u8 **)(n + 0x8) + 0x6) = *(u16 *)(p + 0x6);
        *(s16 *)(*(u8 **)(n + 0x8) + 0xA) = *(u16 *)(p + 0xA);
        n += 0x20;
        *(s16 *)(n + 0x10) =
            func_800BCB04(*(u16 *)(p + 0x2), *(u16 *)(p + 0x6), (s16)(*(u16 *)(p + 0xA) - 0x80));
        *(s16 *)(n + 0x48) = v;
    }
}

/* Thirteen kind-0x312 nodes scattered around the caller's view: each gets
   func_800DB164 as its +0x10 step, a position jittered by
   (rand % 64 + rand % 64 - 0x40) / 2 on each of the two axes, a 0xC06060
   tint out of D_800DEA68's descriptor, and a per-node speed divided by
   (rand & 3) + 4.  The tail is the +0x48 countdown and the 0x8000 flags,
   func_800B63A8's ending one field over.  156 words, 0 differing.

   TWO STATEMENT ORDERS ARE THE WHOLE FIGHT, and both are load-delay slots.
   `*(u32 *)(r + 0x8) = *(u32 *)(D_800DEA68 + 4);` must be written BEFORE the
   two `r[0x4] = r[0x5] = 0` byte stores: written after them the `lw` has no
   filler and the function is 157 words with a `nop`.  Likewise
   `*(s16 *)(w + 0x10) = *(u16 *)(v + 0xA);` must come BEFORE
   `*(s16 *)(w + 0x48) = 4;`, which lets sched hoist the `lhu` over the
   constant -- the other order is a second `nop`, 158 words.  The two stores
   still come out 0x48 then 0x10, because sched reorders two stores to
   disjoint displacements off one base.

   The +0x10 store to the actor view is DUPLICATED in the original: it is
   written once before the divide and once at the end of the loop body, from
   the same `*(u16 *)(v + 0xA)`.  Both are in the target. */
void func_800DAEF4(u8 *p, UnkVec *v, u8 *c)
{
    UnkNode *n;
    u8 *r;
    u8 *w;
    s16 t;
    s32 i;

    for (i = 0xC; i >= 0; i--)
    {
        n = func_8003FD64(0x312, D_80083498);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_800DB164;
            func_8004491C(n, (u32)D_80045340);
            r = (u8 *)n->unk_0C;
            w = (u8 *)n + 0x20;
            *(s16 *)((u8 *)n->unk_08 + 0x2) =
                *(u16 *)((u8 *)v + 0x2) + ((rand() % 64 + rand() % 64 - 0x40) / 2);
            *(s16 *)((u8 *)n->unk_08 + 0x6) =
                *(u16 *)((u8 *)v + 0x6) + ((rand() % 64 + rand() % 64 - 0x40) / 2);
            *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)(p + 0x10);
            *(s16 *)(w + 0x10) = *(u16 *)((u8 *)v + 0xA);
            n->unk_08->unk_14 = (v->unk_08 - (*(s16 *)(p + 0x10) << 16)) / ((rand() & 3) + 4);
            *(s16 *)(r + 0x1C) = 0x1000;
            *(s16 *)(r + 0x1E) = 0x200;
            *(u16 *)(r + 0x14) |= 0xC;
            if (rand() & 1)
            {
                *(u16 *)(r + 0x14) |= 0x1;
            }
            *(s16 *)(r + 0x10) = 0x60;
            *(s32 *)(r + 0xC) = 0xC06060;
            *(u8 **)r = D_800DEA68;
            *(u32 *)(r + 0x8) = *(u32 *)(D_800DEA68 + 4);
            r[0x4] = 0;
            r[0x5] = 0;
            *(s16 *)(w + 0x10) = *(u16 *)((u8 *)v + 0xA);
            *(s16 *)(w + 0x48) = 4;
        }
    }
    t = *(u16 *)(p + 0x48) - 1;
    *(s16 *)(p + 0x48) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The spawned marker's +0x10 step: a three-case state machine on +0x4C,
   driven by the +0x48 countdown the function spends on entry. Case 0 jitters
   the renderable's two scales, case 1 bleeds the colour word and the mover's
   +0x14 until the view's height passes +0x10, case 2 kills the node. The
   tail adds the remaining delta once the state has left 0.

   EACH RANDOM DELTA IS ITS OWN STATEMENT. Written inline,
   `field - ((rand() & 0xFF) + 0x300)`, fold reassociates it into
   `(field - 0x300) - draw` and the constant is folded into the field's own
   `addiu` -- 6 differing, three words per arm in the wrong order. With the
   delta computed first the target's `andi` / `lhu` / `addiu 0x300` / `subu`
   comes out directly. (Holding the 0x300 in a local works too, and so does
   an s16 delta; all four spellings measured 0.)

   BOTH `*(s16 *)(p + 0x48) <= 0` TESTS ARE THE SAME SOURCE. In case 2 no
   call intervenes, so cse forwards the stored value and the test is a bare
   `sll 16` on the register; in case 0 the two rand calls kill the
   equivalence and the field is reloaded with `lh`. 94 words, 0 differing. */
void func_800DB164(u8 *p, UnkVec *v, UnkSlot *r)
{
    s32 d;

    *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) - 1;
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        d = (rand() & 0xFF) + 0x300;
        *(s16 *)((u8 *)r + 0x1C) = *(u16 *)((u8 *)r + 0x1C) - d;
        d = (rand() & 0xFF) + 0x200;
        *(s16 *)((u8 *)r + 0x1E) = *(u16 *)((u8 *)r + 0x1E) + d;
        if (*(s16 *)(p + 0x48) <= 0)
        {
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 1:
        *(s32 *)((u8 *)r + 0xC) += 0xFFF3F3F4;
        v->unk_14 -= (rand() & 0xFF) << 10;
        if (*(s16 *)((u8 *)v + 0xA) < *(s16 *)(p + 0x10))
        {
            *(s16 *)(p + 0x48) = 4;
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 2:
        if (*(s16 *)(p + 0x48) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    if (*(s16 *)(p + 0x4C) != 0)
    {
        v->unk_08 += v->unk_14;
        func_800478B8((u8 *)r);
    }
}

/* Two passes of four 0x312 nodes each, the outer counter stepping by 3 so
   that it is 3 then 0, which is what the `sltu $zero, $s1` stored at +0x24 of
   each payload reports.

   THE FOURTH PARAMETER IS NARROW. Declared s32 the function is still 81 words
   but 13 differing, with `d` and the two loop invariants rotated through
   $s4..$s7 the other way. Nothing calls this function anywhere in the
   overlay, so the width is not witnessed and s16 is the guess. THE THREE
   READS OFF THE SECOND ARGUMENT ARE IN SOURCE ORDER +0x28, +0x14, +0x12, and
   the +0x28 copy must come FIRST: a load off `b` cannot be lifted over a
   store through `r`, so with the 0x1E/0x1C scale written ahead of it cc1psx
   emits a load-delay `nop` and the function is 83 words. 81 words,
   0 differing. */
void func_800DB2DC(u8 *a, u8 *b, u8 *c, s16 d)
{
    UnkNode *n;
    u8 *q;
    u8 *r;
    s32 i;
    s32 j;

    for (j = 3; j >= 0; j -= 3)
    {
        for (i = 3; i >= 0; i--)
        {
            n = func_8003FD64(0x312, D_80083498);
            if (n != NULL)
            {
                n->unk_10 = (u32)func_800DB420;
                *(s16 *)((u8 *)n + 0x4A) = *(u16 *)(c + 0x2A);
                r = (u8 *)n->unk_0C;
                *(u32 *)(r + 0x28) = *(u32 *)(b + 0x28);
                *(s16 *)(r + 0x1E) = 0x1000;
                *(s16 *)(r + 0x1C) = 0x1000;
                *(s16 *)(r + 0x14) = *(u16 *)(b + 0x14);
                *(s16 *)(r + 0x12) = *(u16 *)(b + 0x12) - 0x80;
                *(u32 *)(r + 0xC) = 0x808080;
                *(u16 *)(r + 0x14) |= 0xC;
                *(u8 **)((u8 *)n + 0x20) = a;
                *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)(a + 0x2);
                *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)(a + 0x6);
                *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)(a + 0xA);
                q = (u8 *)n + 0x20;
                *(s16 *)(q + 0x6) = d;
                *(s16 *)(q + 0x4) = (j != 0);
                *(u32 *)(r + 0x2C) = *(u32 *)(b + 0x2C);
            }
        }
    }
}

/* A node's +0x10 step for the four-per-pass sparks func_800DB2DC spawns:
   jitter the mover's three axes within +/-5 of the source record's own
   position every frame, then run a three-state machine off the payload's
   +0x4 -- state 0 tints the renderable 0x303030 at scale 0x60, state 1
   installs D_80045340 and pushes the facing through func_80047784, state 2
   calls func_800478B8 and retires the node when the +0x6 countdown reaches
   zero.

   The jitter is `rand() % 11 - 5`: the magic 0x2E8BA2E9 with
   `mfhi`/`sra 1` and a `sra 31` sign correction is tools/decomp/magic.py's
   divisor 11, and the `lw 0x0($s2)` before each of the three reads is the
   source record re-dereferenced -- the call between them invalidates it.

   THE TWO STORES IN STATE 0 ARE IN THE ORDER +0x10 THEN +0x0C. The other
   way the 0x303030 `lui` does not reach the earlier branch's delay slot and
   the pair comes out `lui $v0`/`sw` instead of `lui $v1`/`sw`: 126 words,
   5 differing. 126 words, 0 differing. */
void func_800DB420(u8 *p, UnkVec *m, u8 *c)
{
    *(s16 *)((u8 *)m + 0x2) = *(u16 *)(*(u8 **)p + 0x2) + rand() % 11 - 5;
    *(s16 *)((u8 *)m + 0x6) = *(u16 *)(*(u8 **)p + 0x6) + rand() % 11 - 5;
    *(s16 *)((u8 *)m + 0xA) = *(u16 *)(*(u8 **)p + 0xA) + rand() % 11 - 5;
    switch (*(s16 *)(p + 0x4))
    {
    case 0:
        *(s16 *)(c + 0x10) = 0x60;
        *(u32 *)(c + 0xC) = 0x303030;
        *(s16 *)(p + 0x4) += 1;
        break;
    case 1:
        func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
        func_80047784(c, (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s16 *)(p + 0x4) += 1;
        break;
    case 2:
        func_800478B8(c);
        if (--*(s16 *)(p + 0x6) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* 18 words, 0 differing. */
void func_800DB618(u8 *p)
{
    if (*(s32 *)(p + 0x18) >= *(s32 *)(p + 0x1C))
    {
        *(s32 *)(p + 0x18) -= *(s32 *)(p + 0x1C);
    }
    else
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* One semi-transparent textured quad per step of a straight segment, for
   every node of the tag chain. The segment runs from the record's
   +0x08/+0x0A/+0x0C to its +0x10/+0x12/+0x14 in `*(s16 *)(p + 0x36)` steps;
   each step builds func_800DBA90's 0x20-byte matrix record, rotates the
   +0x26-scaled unit vector through it, widens the result by the +0x2E/+0x30
   half-extents into the four corners, projects them with RotTransPers4 and
   hangs the POLY_FT4 on the OT at the returned depth less 8. The colour at
   +0x18 steps down by 0x080808 per quad until it reaches 0x080807, and the
   two facing angles at +0x28/+0x22 advance by +0x2A/+0x24. 268 words,
   0 differing.

   THE THREE INTERPOLATED AXES ARE `d * i / n`, NOT ACCUMULATORS, AND THAT IS
   THREE WORDS. loop.c strength-reduces each product into a giv whose `= 0`
   lands in the loop PREHEADER -- after the `bltz` guard -- and whose `+= d`
   lands at the loop bottom, in the reverse order the three were created.
   Hand-written accumulators zeroed in the `for` init are 262 words of 268
   with 49 aligned differences: the init is then in the GUARD's own basic
   block, where sched1 spends the three `addu`s on load-delay slots that the
   target leaves as `nop`s, and the three givs' registers rotate. The copy
   chain `addu $s6, $zero, $zero` / `addu $s5, $s6` / `addu $s4, $s5` is what
   a giv init looks like after cse unifies three zeros.

   THE LOOP BOUND IS READ OFF `q`, NOT `p`. Both are the same pointer -- `q`
   is a plain copy and the two live in different callee-saved registers all
   the way through -- but cse substitutes `p` into the duplicated top test,
   which is why the guard is `lh $v1, 0x36($s2)` and the bottom test is
   `lh $v0, 0x36($s1)`. Spelling the bound on `p` is 1 differing, exactly
   that word.

   `ang += *(s16 *)(q + 0x2A)` GOES BEFORE THE PRIMITIVE FETCH. After it,
   the `lh`/`addu` pair falls into the RotTransPers4 call's delay slot and
   the `sw $v0, 0x24($sp)` that belongs there moves out: 268 words,
   21 aligned differences.

   D_80083160 IS A POINTER LOCAL AND REMATERIALISES. All nine callee-saved
   registers are taken ($s0..$s7 and $fp), so `g` gets none and reload emits
   its `lui`/`addiu` at each of the three uses, which is the shape
   func_800A7CC4's note records for the same symbol. The 0xFFFFFF addPrim
   mask is loop-hoisted into a pseudo and spilled to 0xA8($sp); the
   0xFF000000 half is rematerialised with a bare `lui`.

   The primitive cursor advances by 0x34, not sizeof(POLY_FT4): the stride is
   written as a byte count on the u32, the way this unit's other +0x8D0 users
   do it. `SVECTOR vec` and `u16 vec[3]` are byte-identical here, because
   every read of it feeds an `sh` and combine picks `lhu` either way. */
s32 func_800DB660(u8 *p)
{
    SVECTOR v[4];
    SVECTOR v2[4];
    SVECTOR vec;
    u8 rec[0x20];
    s32 w;
    Unk80083160 *g = &D_80083160;
    POLY_FT4 *t;
    UnkNode *n;
    u8 *q;
    s32 i;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 ang;
    s32 zz;
    s32 col;
    s32 z;

    while (1)
    {
        v[0].vx = v[1].vx = -*(s16 *)(p + 0x2E) >> 1;
        v[0].vz = v[2].vz = -*(s16 *)(p + 0x30) >> 1;
        v[2].vx = v[3].vx = v[0].vx + *(u16 *)(p + 0x2E);
        v[1].vz = v[3].vz = v[0].vz + *(u16 *)(p + 0x30);
        v[0].vy = v[1].vy = v[2].vy = v[3].vy = 0;
        i = 0;
        zz = *(s16 *)(p + 0x22);
        q = p;
        ang = *(s16 *)(p + 0x28);
        col = *(s32 *)(p + 0x18);
        dx = *(s16 *)(p + 0x10) - *(s16 *)(p + 0x8);
        dy = *(s16 *)(p + 0x12) - *(s16 *)(p + 0xA);
        dz = *(s16 *)(p + 0x14) - *(s16 *)(p + 0xC);
        for (; i <= *(s16 *)(q + 0x36); i++)
        {
            vec.vx = (func_800644B8(ang) * *(s16 *)(q + 0x26)) >> 12;
            vec.vy = vec.vz = 0;
            *(u8 **)rec = (u8 *)&vec;
            *(u8 **)(rec + 0x4) = (u8 *)&vec;
            *(s16 *)(rec + 0xC) = *(u16 *)(q + 0x2C);
            *(s16 *)(rec + 0x10) = *(u16 *)(q + 0x8) + dx * i / *(s16 *)(q + 0x36);
            *(s16 *)(rec + 0x12) = *(u16 *)(q + 0xA) + dy * i / *(s16 *)(q + 0x36);
            *(s16 *)(rec + 0x14) = *(u16 *)(q + 0xC) + dz * i / *(s16 *)(q + 0x36) +
                                   ((func_800644B8(zz) * *(s16 *)(q + 0x20)) >> 12);
            *(s16 *)(rec + 0x18) = 1;
            *(s16 *)(rec + 0x1A) = 0;
            func_800DBA90(rec);
            v2[0].vx = v2[1].vx = vec.vx - ((s16)(*(u16 *)(q + 0x2E)) >> 1);
            v2[2].vx = v2[3].vx = v2[0].vx + *(u16 *)(q + 0x2E);
            v2[0].vz = v2[2].vz = vec.vz - ((s16)(*(u16 *)(q + 0x30)) >> 1);
            v2[1].vz = v2[3].vz = v2[0].vz + *(u16 *)(q + 0x30);
            v2[0].vy = v2[1].vy = v2[2].vy = v2[3].vy = vec.vy;
            zz += *(s16 *)(q + 0x24);
            ang += *(s16 *)(q + 0x2A);
            t = *(POLY_FT4 **)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x34;
            z = RotTransPers4(&v2[0], &v2[1], &v2[2], &v2[3], (s32 *)&t->x0, (s32 *)&t->x1,
                              (s32 *)&t->x2, (s32 *)&t->x3, &w, &w) -
                8;
            if ((u32)z < 0x1E0)
            {
                *(u32 *)&t->r0 = col;
                SetPolyFT4(t);
                SetSemiTrans((u8 *)t, 1);
                t->tpage = *(u16 *)(q + 0x32);
                t->clut = *(u16 *)(q + 0x34);
                t->u0 = t->u1 = q[0x0];
                t->u2 = t->u3 = q[0x0] + q[0x4];
                t->v0 = t->v2 = q[0x2];
                t->v1 = t->v3 = q[0x2] + q[0x6];
                addPrim((u32 *)(g->unk_00 + 0xB0) + z, t);
            }
            if (col > 0x80807)
            {
                col += 0xFFF7F7F8;
            }
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
    }
    return 0;
}

/* Stack the caller's record through the GTE: push, set the translation from
   +0x10/+0x12/+0x14, build the rotation according to the +0x1A mode, then
   RotTrans every source SVECTOR at *(p) into the halfword triple at
   *(p + 4). 100 words, 0 differing.

   THE IDENTITY FILL'S INNER COUNTER IS `flag` ITSELF -- the same local whose
   address is handed to RotTrans later. That address-take makes it
   TREE_ADDRESSABLE, so it lives in its stack slot and the inner loop reloads
   it, recomputing `j * 2 + row` every iteration. With a separate `s32 j` the
   counter stays in a register, loop.c reduces the subscript to a pointer walk
   (`addiu $v0, $a0, 4`) and the function is 94 words / 71 differing. The six
   missing words are exactly the four 0x40($sp) accesses and the sll/addu pair.

   The 0x1A == 1 test is NESTED inside the `!= 0x8000` arm, not a separate
   top-level `if`: both the `== 0` arm and the `== 0x8000` arm branch straight
   to the SetRotMatrix call and never reach it. */
void func_800DBA90(u8 *p)
{
    MATRIX m;
    VECTOR v;
    s32 flag;
    s32 i;

    PushMatrix();
    m.t[0] = *(s16 *)(p + 0x10);
    m.t[1] = *(s16 *)(p + 0x12);
    m.t[2] = *(s16 *)(p + 0x14);
    SetTransMatrix(&m);
    if (*(u16 *)(p + 0x1A) == 0)
    {
        RotMatrix((SVECTOR *)(p + 0x8), &m);
    }
    else if (*(u16 *)(p + 0x1A) != 0x8000)
    {
        for (i = 2; i >= 0; i--)
        {
            for (flag = 2; flag >= 0; flag--)
            {
                m.m[i][flag] = (i == flag) ? 0x1000 : 0;
            }
        }
        if (*(u16 *)(p + 0x1A) == 1)
        {
            RotMatrixX(*(s16 *)(p + 0x8), &m);
            RotMatrixY(*(s16 *)(p + 0xA), &m);
            RotMatrixZ(*(s16 *)(p + 0xC), &m);
        }
    }
    SetRotMatrix(&m);
    for (i = 0; i < *(s16 *)(p + 0x18); i++)
    {
        RotTrans((SVECTOR *)(*(u8 **)p + i * 8), &v, &flag);
        *(s16 *)(*(u8 **)(p + 0x4) + i * 8) = v.vx;
        *(s16 *)(*(u8 **)(p + 0x4) + i * 8 + 0x2) = v.vy;
        *(s16 *)(*(u8 **)(p + 0x4) + i * 8 + 0x4) = v.vz;
    }
    PopMatrix();
}

/* Draw one digit strip: format two numbers into stack buffers through
   func_8004E57C, then walk the first string and blit each character with
   MoveImage, the glyph's x chosen from the engine's per-character
   attribute table and stepping left by 4 each time.

   SIX PARAMETERS, AND THE LAST TWO ARE BOTH s16. The sixth arrives on the
   stack and is read with `lw` and then `sll 16`/`sra 16` -- a PROMOTED s16
   stack parameter, where a plain s32 gets rematerialised at its use and
   costs the function a word. The fifth is also s16 and the witness is the
   `sll`/`sra` where it is passed on; as u16 it is an `andi $a2, $s5,
   0xFFFF` instead, which is the same length and one differing word.

   THE WALKER IS WHAT IS PASSED TO THE FIRST func_8004E57C, not the array.
   Passing `t` and walking a separate `s` makes two pseudos and an
   `addu $s0, $s2, $zero` between them. 79 words, 0 differing. */
void func_800DBC20(s32 a, s32 b, u16 c, s16 x, s16 y, s16 n)
{
    UnkDungeonRect r;
    u8 t[0x10];
    u8 u[0x10];
    u8 *s;

    s = t;
    func_8004E57C(a, c, (s32)s);
    func_8004E57C(b, c, (s32)u);
    if (t[0] != 0)
    {
        do
        {
            if (n != 0)
            {
                r.v[0] = (D_800808ED[*s] & 4) ? (*s * 4 + 0x100) : 0x1C0;
            }
            else
            {
                r.v[0] = (D_800808ED[*s] & 4) ? (*s * 4 + 0x100) : 0x1E8;
            }
            r.v[1] = 0x1C0;
            r.v[2] = 0x4;
            r.v[3] = 0x8;
            MoveImage(r.v, x, y);
            s++;
            x -= 4;
        } while (*s != 0);
    }
}

/* func_800DBC20 with the other glyph row: 0x1EC for the fallback, 0x1B0
   for the row and a height of 0x10. 79 words, 0 differing. */
void func_800DBD5C(s32 a, s32 b, u16 c, s16 x, s16 y, s16 n)
{
    UnkDungeonRect r;
    u8 t[0x10];
    u8 u[0x10];
    u8 *s;

    s = t;
    func_8004E57C(a, c, (s32)s);
    func_8004E57C(b, c, (s32)u);
    if (t[0] != 0)
    {
        do
        {
            if (n != 0)
            {
                r.v[0] = (D_800808ED[*s] & 4) ? (*s * 4 + 0x100) : 0x1C0;
            }
            else
            {
                r.v[0] = (D_800808ED[*s] & 4) ? (*s * 4 + 0x100) : 0x1EC;
            }
            r.v[1] = 0x1B0;
            r.v[2] = 0x4;
            r.v[3] = 0x10;
            MoveImage(r.v, x, y);
            s++;
            x -= 4;
        } while (*s != 0);
    }
}

/* Hand one of the overlay's two stored rectangles to the engine. The copy
   to a local is in the source, not an artefact: MoveImage takes a
   writable `s16 *`. */
void func_800DBE98(void)
{
    UnkDungeonRect r = D_80089490;

    MoveImage(r.v, 0x1C2, 0x19C);
}

void func_800DBEE8(void)
{
    UnkDungeonRect r = D_80089498;

    MoveImage(r.v, 0x1C2, 0x19C);
}

/* Drop bit 0 of a flag word in the object D_800E5910 points at.

   THE BASE LOCAL IS LOAD-BEARING AND SO IS ITS SECOND USE. As one
   expression the two constants fold and the access becomes `lw 0x74`; split
   across a local, combine would still propagate the `addiu` into the memory
   address -- except that the local feeds BOTH the load and the store, and
   combine does not substitute a def with two uses. 9 words, 0 differing. */
void func_800DBF38(void)
{
    u8 *p;

    p = D_800E5910 + 0x20;
    *(s32 *)(p + 0x54) &= ~1;
}

/* func_800DBF38's object, gated. 14 words, 0 differing. */
void func_800DBF5C(void)
{
    u8 *p;

    p = D_800E5910 + 0x20;
    if (*(s32 *)(p + 0x5C) == 0)
    {
        *(s32 *)(p + 0x54) |= 1;
    }
}

/* 27 words, 0 differing. */
void func_800DBF94(u8 *p)
{
    u8 *g;

    g = (u8 *)D_80081468;
    if (*(s16 *)(g + 0x4) != *(s32 *)p)
    {
        func_800DBD5C(*(s16 *)(g + 0x4), *(s32 *)p, 2, 0x18C, 0x1B8, 0);
        *(s32 *)p = *(s16 *)(g + 0x4);
    }
}

/* 30 words, 0 differing. */
void func_800DC000(u8 *p)
{
    if (*(s32 *)(D_800814A8 + 0xFC) != *(s32 *)(p + 0x4))
    {
        func_800DBD5C(*(s32 *)(D_800814A8 + 0xFC), *(s32 *)(p + 0x4), 8, 0x1A8, 0x1E0, 0);
        *(s32 *)(p + 0x4) = *(s32 *)(D_800814A8 + 0xFC);
    }
}

/* The `srl 31`/`addu`/`sra 1` trio is the signed `/ 2`.
   15 words, 0 differing. */
void func_800DC078(u8 *p, s16 v)
{
    s32 n;

    if (v < 0x80)
    {
        n = v / 2 + 1;
    }
    else
    {
        n = 0x40;
    }
    p[0x2] = 0x57 - n;
    p[0xA] = n;
}

void func_800DC0B4(void)
{
}

/* Republish the two glyph indices whenever the engine's record has moved on.
   THE COMPARISON IS WRITTEN GLOBAL-FIRST: the other order is 2 differing,
   both `beq` operands swapped, and the two loads come out in the same order
   either way -- so the register order of the branch is the only witness.
   43 words, 0 differing. */
void func_800DC0BC(u8 *p, u8 **q)
{
    if (*(u8 *)(D_800814A8 + 0x28) != *(s32 *)(p + 0x10))
    {
        func_800DC078(q[0], *(u8 *)(D_800814A8 + 0x28));
        *(s32 *)(p + 0x10) = *(u8 *)(D_800814A8 + 0x28);
    }
    if (*(u8 *)(D_800814A8 + 0x29) != *(s32 *)(p + 0x14))
    {
        func_800DC078(q[0] + 0xC, *(u8 *)(D_800814A8 + 0x29));
        *(s32 *)(p + 0x14) = *(u8 *)(D_800814A8 + 0x29);
    }
}

/* `u32 t` for the `sltiu`, and `-(s32)t / 2 - 1` for the signed halving.
   20 words, 0 differing. */
void func_800DC168(u8 *p, u8 *q)
{
    u32 t;
    u8 *r;
    s16 v;

    t = *(u8 *)(D_800814A8 + 0x29);
    if (t != *(s32 *)(p + 0x14))
    {
        r = *(u8 **)(q + 0x4);
        if (t < 0x80)
        {
            v = -(s32)t / 2 - 1;
        }
        else
        {
            v = -0x40;
        }
        *(s16 *)(r + 0x8) = v;
    }
}

/* 16 words, 0 differing. */
void func_800DC1B8(void *arg0)
{
    u8 *p;

    p = D_800E5910 + 0x20;
    *(s32 *)(p + 0x18) = *(s32 *)(p + 0x18) % 32 + 0x20;
    *(void **)(p + 0x2C) = arg0;
}

/* The three-step flash driver for the pair of screen records `q` points at:
   a +0x28 phase counter advanced by the 2-bit field at the top of
   `*(u16 **)(q + 0x8)`, a +0x20/+0x24 pair of flags that allocates and frees
   the +0x80 slot through func_8004DC14/func_8004E99C, a colour ramp written
   into the three bytes `*(u8 **)(q + 0x4)` and the +0x84 record point at, and
   a +0x18 frame budget that resets everything when it runs out.  The tail
   republishes the two D_800814A8 glyph indices through func_800DBD5C and
   func_800DBC20 whenever the engine's record has moved on -- func_800DC0BC's
   comparison, written GLOBAL-FIRST.  239 words, 0 differing.

   FOUR SPELLING FACTS.

   (1) THE THREE COLOUR BYTES ARE A CHAINED ASSIGNMENT WITH THE POINTER
   WRITTEN OUT THREE TIMES, AND THAT IS NOT WHAT THE TARGET WANTS.  gcc
   evaluates an assignment's destination address before its RHS, so
   `**(u8 **)(q+4) = *(..+1) = *(..+2) = v` does put the three address
   loads ahead of the branch that computes v -- which is where the target
   has them, with two `addu` copies of one `lw` -- but a three-deep chain of
   NARROW lvalues makes store_expr return the MEM, and the middle assignment
   then RELOADS it: `sb 2($a0)` / `lbu 2($a1)` / `sb 1($a2)` / `sb 0($a0)`,
   two words too many, 240/231.  A `(u8)` or `(s8)` cast on the value, and
   reversing the chain, are byte-identical to it.  Three separate statements
   instead are 239 words and 232 differing, because a QImode store wipes
   cse's memory table and each statement reloads `*(u8 **)(q + 0x4)`.  What
   works is THREE POINTER LOCALS assigned before the branch and one store
   each: that is the only spelling measured that has three live base pseudos
   AND no reload.  The SECOND group, off `*(u8 **)(p + 0x3C) + 0x84`, keeps
   the chain, because there the value is already a `u8` (`**(u8 **)(q+4)`)
   so nothing is truncated and no reload is emitted.

   (2) THE MASKED COMPARE NEEDS ITS OWN LOCAL.  `(**(u16 **)(q + 0x8) &
   0x7FF) < 0x400` written inline compiles to `sltiu` -- combine proves the
   masked value non-negative -- where the target has `slti`.  An `s32` local
   for the masked value separates the mask from the compare and restores it
   (docs/matching-levers.md, "A MASKED COMPARE IS UNSIGNED").

   (3) `g = *(u8 **)(p + 0x40);` MUST BE ITS OWN STATEMENT, and it is the
   third callee-saved register.  Written inline as the store's destination
   -- `*(s16 *)(*(u8 **)(p + 0x40) + 0x88) = ...` -- the `lw` lands AFTER
   strlen and needs no saved register, so the function saves two
   callee-saved registers instead of three and comes out 237 words.  Bound
   to a local before the call it lives across it, takes a third $s register
   and the function is 239.

   (4) CASE 0'S THREE FIELD UPDATES ARE IN OFFSET ORDER 0x20, 0x24, 0x28.
   All six permutations were measured: this one is 0 differing, 0x24/0x20/
   0x28 is 4, 0x20/0x28/0x24 and 0x28/0x24/0x20 are 6, and the other two are
   8 -- the two `xori` chains pair up and the load order is what shows it. */
void func_800DC1F8(u8 *p, u8 *q)
{
    u8 *g;
    s32 n;
    s32 t;
    s32 u;
    s32 v;
    u8 *cr;
    u8 *cg;
    u8 *cb;

    if (*(s32 *)(p + 0x18) != 0)
    {
        switch (*(s32 *)(p + 0x28))
        {
        case 0:
            if (((**(u16 **)(q + 0x8) >> 10) & 3) == 1)
            {
                func_80053DA8(0x504);
                *(s32 *)(p + 0x20) ^= 1;
                *(s32 *)(p + 0x24) ^= 1;
                *(s32 *)(p + 0x28) += 1;
            }
        case 1:
            if (((**(u16 **)(q + 0x8) >> 10) & 3) == 2)
            {
                *(s32 *)(p + 0x1C) = 0x10;
                *(s32 *)(p + 0x28) += 1;
            }
            break;
        case 2:
            if (((**(u16 **)(q + 0x8) >> 10) & 3) == 3)
            {
                func_80053DA8(0x504);
                *(s32 *)(p + 0x28) = 0;
                *(s32 *)(p + 0x24) ^= 1;
                *(s32 *)(p + 0x20) ^= 1;
            }
            break;
        }
        if (*(s32 *)(p + 0x20) != 0)
        {
            if (*(s32 *)(p + 0x24) != 0)
            {
                func_8004E99C(*(s32 *)(*(u8 **)(p + 0x3C) + 0x80));
                func_800DBEE8();
                g = *(u8 **)(p + 0x40);
                *(s16 *)(g + 0x88) =
                    (3 - ((u32)strlen(*(u8 **)(p + 0x2C)) >> 1)) * 6 + 0xA4;
                *(s32 *)(*(u8 **)(p + 0x3C) + 0x80) = func_8004DC14(*(u8 **)(p + 0x2C), 6);
                if (*(s32 *)(*(u8 **)(p + 0x3C) + 0x80) == 0)
                {
                    return;
                }
            }
            else
            {
                func_8004E99C(*(s32 *)(*(u8 **)(p + 0x3C) + 0x80));
                *(s32 *)(*(u8 **)(p + 0x3C) + 0x80) = 0;
                *(s32 *)(p + 0x10) = 0;
                *(s32 *)(p + 0x14) = 0;
                func_800DBE98();
            }
            *(s32 *)(p + 0x20) = 0;
        }
        cr = *(u8 **)(q + 0x4);
        cg = *(u8 **)(q + 0x4);
        cb = *(u8 **)(q + 0x4);
        t = (*(s16 *)*(u8 **)(q + 0x8) - 0x400) & 0x7FF;
        n = 0x400 - t;
        u = **(u16 **)(q + 0x8) & 0x7FF;
        v = (u < 0x400 ? n >> 4 : n >> 3) - 0x80;
        cb[2] = v;
        cg[1] = v;
        cr[0] = v;
        **(u8 **)(*(u8 **)(p + 0x3C) + 0x84) = *(*(u8 **)(*(u8 **)(p + 0x3C) + 0x84) + 1) =
            *(*(u8 **)(*(u8 **)(p + 0x3C) + 0x84) + 2) = **(u8 **)(q + 0x4);
        if (*(s32 *)(p + 0x1C) == 0)
        {
            **(u16 **)(q + 0x8) += 0x80;
            **(u16 **)(*(u8 **)(p + 0x3C) + 0x88) += 0x80;
            if ((*(s32 *)(p + 0x18) -= 1) == 0)
            {
                *(s32 *)(p + 0x28) = 0;
                **(u16 **)(q + 0x8) = 0;
                **(u16 **)(*(u8 **)(p + 0x3C) + 0x88) = 0x800;
                **(u8 **)(q + 0x4) = *(*(u8 **)(q + 0x4) + 1) = *(*(u8 **)(q + 0x4) + 2) = 0x80;
                **(u8 **)(*(u8 **)(p + 0x3C) + 0x84) =
                    *(*(u8 **)(*(u8 **)(p + 0x3C) + 0x84) + 1) =
                        *(*(u8 **)(*(u8 **)(p + 0x3C) + 0x84) + 2) = 0x80;
            }
        }
        else
        {
            *(s32 *)(p + 0x1C) -= 1;
        }
    }
    if (*(s32 *)(p + 0x24) == 0)
    {
        if (*(u8 *)(D_800814A8 + 0x28) != *(s32 *)(p + 0x10))
        {
            func_800DBD5C(*(u8 *)(D_800814A8 + 0x28), *(s32 *)(p + 0x10), 3, 0x1CA, 0x19C, 1);
        }
        if (*(u8 *)(D_800814A8 + 0x29) != *(s32 *)(p + 0x14))
        {
            func_800DBC20(*(u8 *)(D_800814A8 + 0x29), *(s32 *)(p + 0x14), 3, 0x1DA, 0x1A4, 1);
        }
    }
}

/* 29 words, 0 differing. */
void func_800DC5B4(u8 *p)
{
    if (*(u8 *)(D_800814A8 + 0x11) != *(s32 *)(p + 0x30))
    {
        func_800DBD5C(*(u8 *)(D_800814A8 + 0x11), *(s32 *)(p + 0x30), 2, 0x18A, 0x190, 0);
        *(s32 *)(p + 0x30) = *(u8 *)(D_800814A8 + 0x11);
    }
}

/* One half-extent into four fields of the record at +0x8 and its negation
   into a fifth.

   THE NODE IS RE-READ FOR THE LAST STORE. Dereferenced inline at all five
   the unit gets five `lw`s -- a halfword store through an unrelated pointer
   kills the equivalence every time -- and cached in one local it gets one;
   the target has exactly two. 10 words, 0 differing. */
void func_800DC628(u8 *p, s32 v)
{
    u8 *q;

    q = *(u8 **)(p + 0x8);
    *(s16 *)(q + 0x58) = v;
    *(s16 *)(q + 0x34) = v;
    *(s16 *)(q + 0x28) = v;
    *(s16 *)(q + 0xE) = v;
    q = *(u8 **)(p + 0x8);
    *(s16 *)(q + 0x1A) = -v;
}

/* Step the tag builder's facing toward the target angle in units of 0x80.

   THE STORE THROUGH D_800E5910 IS WRITTEN IN BOTH ARMS. Hoisted into one
   assignment after the if/else it is 53 words / 9 differing: the call result
   needs a copy into $v1 and the pointer load's delay slot gets filled.
   Duplicated into the two arms, cross-jumping puts the tail back. The `>> 1`
   is UNSIGNED, so the flag word is read as u32. func_800DC628's second
   parameter had to be WIDENED from s16 to s32: the target passes $s0 raw with
   no `sll 16`/`sra 16`. 53 words, 0 differing. */
void func_800DC650(u8 *p)
{
    u8 *s;
    s32 cur;
    s32 t;
    s32 d;

    s = p + 0x3C;
    cur = *(s16 *)(*(u8 **)(s + 0x8) + 0xE);
    t = *(s16 *)((*(s32 *)(p + 0x50) & 2) + (u8 *)D_800E2934);
    d = t - cur;
    if (d < 0)
    {
        if (d < -0x80)
        {
            cur -= 0x80;
        }
        else
        {
            cur = t;
        }
    }
    else
    {
        if (d < 0x81)
        {
            cur = t;
        }
        else
        {
            cur += 0x80;
        }
    }
    if (cur != t)
    {
        *(s32 *)(p + 0x58) = 1;
        *(s32 *)(D_800E5910 + 0xC) = (s32)func_800DCA1C(*(u8 **)(p + 0x3C), 0);
    }
    else
    {
        *(s32 *)(D_800E5910 + 0xC) =
            (s32)func_800DCA1C(*(u8 **)(p + 0x3C), ((*(u32 *)(p + 0x50) >> 1) ^ 1) & 1);
    }
    func_800DC628(s, cur);
}

/* Ease the tag window's two screen coordinates toward the pair D_800E2924
   holds for the current corner.

   ONE LOCAL PER READ-BACK, NOT ONE REUSED. With a single `s32 v` for both
   halves the two disjoint live ranges become ONE quantity whose priority then
   beats the record pointer's: `q` lands in $a1 and the read-back in $a0,
   exactly swapped, 9 differing. THE STEP IS A TERNARY WITH A SHARED TAIL:
   written as two complete store arms the `>> 2`, the add and the store are
   not cross-jumped and the function is 70 words. And the two table halfwords
   are read into u16 LOCALS up front -- read inline, combine folds the `lhu`
   and the `sll 16`/`sra 16` into one `lh`. 66 words, 0 differing. */
void func_800DC724(u8 *p)
{
    u8 *q;
    s32 i;
    u16 tx;
    u16 tz;
    s32 d;
    s32 k;
    s32 cx;
    s32 cz;

    i = *(s32 *)(p + 0x50);
    tx = D_800E2924[i][0];
    tz = D_800E2924[i][1];
    q = *(u8 **)(*(u8 **)(p + 0x3C) + 0x4);
    *(s32 *)(p + 0x58) = 0;
    d = (s16)tx - *(s16 *)(q + 0x8);
    if (d != 0)
    {
        *(s32 *)(p + 0x58) = 1;
        cx = *(s16 *)(q + 0x8);
        k = (d > 0) ? (d + 3) : (d - 3);
        *(s16 *)(q + 0x8) = cx + (k >> 2);
    }
    d = (s16)tz - *(s16 *)(q + 0xA);
    if (d != 0)
    {
        *(s32 *)(p + 0x58) = 1;
        cz = *(s16 *)(q + 0xA);
        k = (d > 0) ? (d + 3) : (d - 3);
        *(s16 *)(q + 0xA) = cz + (k >> 2);
    }
    func_800DC650(p);
    if (*(s32 *)(p + 0x58) == 0 && *(s32 *)(p + 0x50) != *(s32 *)(p + 0x54))
    {
        if (((*(s32 *)(p + 0x54) ^ *(s32 *)(p + 0x50)) & 2) != 0)
        {
            func_80053DA8(0x507);
        }
        *(s32 *)(p + 0x50) = *(s32 *)(p + 0x54);
    }
}

/* The three flag states of func_800DBF38's object. ONE FUNCTION-SCOPE LOCAL
   FOR THE VALUE IS WHAT COSTS IT: written `v = p[0x54]; ...; p[0x54] = v;`
   with one `v` reused in two arms, the value chain lands in $v1 and the two
   mask constants in $v0 -- which makes the second constant anti-dependent on
   the arm's own `ori`, so sched2 cannot lift it above the `ori`, both arms
   end `[li -3][and]` and jump2 cross-jumps two insns instead of one: 22 words
   against 23, 13 differing. Updating the field in place keeps each arm's
   chain local to its block. Separate per-arm locals also reach 0.
   23 words, 0 differing. */
void func_800DC82C(u8 *p)
{
    s32 t;

    *(s32 *)(p + 0x5C) = 0;
    t = **(s32 **)(p + 0x38);
    if (t == 0)
    {
        *(s32 *)(p + 0x54) |= 3;
    }
    else if (t == 1)
    {
        *(s32 *)(p + 0x54) |= 1;
        *(s32 *)(p + 0x54) &= ~2;
    }
    else
    {
        *(s32 *)(p + 0x5C) = 1;
        *(s32 *)(p + 0x54) &= ~1;
        *(s32 *)(p + 0x54) &= ~2;
    }
}

/* func_800DBF38's object again, with the counter its +0x38 points at
   advanced modulo 3.

   THE TAIL CALL TAKES `p`, and that is the whole residue: func_800DC82C's
   own listing reads 0x38, 0x54 and 0x5C off $a0, and passing nothing is 28
   words / 13 differing with every difference a register name -- p lands in
   $v1 instead of the $a0 the argument ties it to, and the three values below
   it all shift down one. Six spellings of the two updates are all 13
   differing, so the allocation is not reachable from the statement form.
   28 words, 0 differing. */
void func_800DC888(void)
{
    u8 *p;

    p = D_800E5910 + 0x20;
    **(s32 **)(p + 0x38) += 1;
    **(s32 **)(p + 0x38) %= 3;
    func_800DC82C(p);
}

/* The per-frame tag pass: run D_800E2934[1..7] over the primitive buffer at
   +0x3C, 0x10 bytes a slot, then the fixed tail and whatever is parked at
   +0x4C. The first table entry is never called -- the loop starts at 1 and
   gcc drops the entry guard, which is why there is no test above the loop.
   36 words, 0 differing. */
void func_800DC8F8(u8 *p, UnkVec *b, u8 *c)
{
    s32 i;

    for (i = 1; i < 8; i++)
    {
        D_800E2934[i](p, *(u8 **)(p + 0x3C) + (i << 4));
    }
    func_800DC724(p);
    if (*(u32 *)(p + 0x4C) != 0)
    {
        (*(void (**)(u8 *))(p + 0x4C))(p);
    }
}

/* BINDING THE FIRST VALUE TO A LOCAL IS WHAT ORDERS THE TWO `lui`s. The
   target materialises %hi(D_8008146C) before %hi(D_800814A8); written
   `*(s32 *)(p + 0x20) = ~D_8008146C;` as a statement the two come out the
   other way round and the function is 37 words / 2 differing -- and no order
   of the statements or of the declarations moves it (four spellings,
   byte-identical output each time). Moving the store itself ahead of
   `r = (u8 *)D_800814A8;` is worse, 38 words: a store through `p` may alias
   the global, so the load has to be re-issued after it. 37 words,
   0 differing. */
void func_800DC988(u8 *p)
{
    u8 *q;
    u8 *r;
    s32 v;

    v = ~D_8008146C;
    r = (u8 *)D_800814A8;
    q = p + 0x20;
    *(s32 *)(p + 0x20) = v;
    *(s32 *)(q + 0x4) = ~*(s32 *)(r + 0xFC);
    *(s32 *)(q + 0x10) = ~r[0x28];
    *(s32 *)(q + 0x14) = ~r[0x29];
    *(s32 *)(q + 0x30) = ~r[0x11];
    func_800DC82C(q);
    do
    {
        func_800DC724(q);
    } while (*(s32 *)(q + 0x58) != 0);
}

/* Thread one of the two index lists into a backwards chain through each
   0x10-byte record's +0xC, and hand back the last record.

   THE CURSOR IS A SECOND VARIABLE. Walking the selected table pointer itself
   merges the guard's read with the loop's and loses the target's
   `addu $a1, $v1, $zero`: 26 words against 27, 20 differing. `for (q = p; ...)`
   over the same body is 27 / 12 -- the copy has to sit AFTER the guard test.
   `*q` is read twice per iteration because the `sw` between them flushes
   cse. 27 words, 0 differing. */
u8 *func_800DCA1C(u8 *base, s32 which)
{
    u8 *p;
    u8 *q;
    u8 *r;

    r = NULL;
    if (which != 0)
    {
        p = D_800E2960;
    }
    else
    {
        p = D_800E2954;
    }
    if (*p < 9)
    {
        q = p;
        do
        {
            *(u8 **)(base + *q * 0x10 + 0xC) = r;
            r = base + *q * 0x10;
            q++;
        } while (*q < 9);
    }
    return r;
}

/* The record at +0x4 is RE-READ for every store, which is the four
   `lw 0x4($a0)`. 15 words, 0 differing. */
void func_800DCA88(u8 *p, s32 flag)
{
    if (flag != 0)
    {
        *(s16 *)(*(u8 **)(p + 0x4) + 0x8) = -0x8C;
    }
    else
    {
        *(s16 *)(*(u8 **)(p + 0x4) + 0x8) = -0x22;
    }
    *(s16 *)(*(u8 **)(p + 0x4) + 0xA) = -0x52;
    *(s16 *)(*(u8 **)(p + 0x4) + 0xC) = 0x200;
}

/* Wire the nine 16-byte records at p[0] to the arrays at p[1], p[2] and
   p[3]. THE DESTINATION IS A STRUCT ARRAY, NOT POINTER ARITHMETIC: its `addu`
   is INDEX-FIRST, and C pointer arithmetic can only emit base-first. The
   source operands stay pointer arithmetic and come out base-first, which is
   what the target has. The second loop initialises `i` BEFORE `j`, in the
   for-init; with `j = 0;` on its own line ahead of the loop the two inits
   swap, 2 differing. 72 words, 0 differing. */
void func_800DCAC4(u8 *p, s32 b)
{
    s32 i;
    s32 j;

    for (i = 0; i < 9; i++)
    {
        ((UnkDungeonDCAC4 *)*(u8 **)p)[i].unk_04 = (u32)(*(u8 **)(p + 0x4) + i * 0x10);
    }
    for (i = 0, j = 0; i < 9; i++)
    {
        ((UnkDungeonDCAC4 *)*(u8 **)p)[i].unk_08 = (u32)(*(u8 **)(p + 0x8) + j);
        j += 0xC;
    }
    for (i = 0; i < 9; i++)
    {
        ((UnkDungeonDCAC4 *)*(u8 **)p)[i].unk_00 = ((u32 *)*(u8 **)(p + 0xC))[i];
    }
    *(s16 *)(*(u8 **)(p + 0x8) + 0x12) = 0x5F;
    *(s16 *)(*(u8 **)(p + 0x8) + 0x1E) = 0x61;
    *(s16 *)(*(u8 **)(p + 0x8) + 0x44) = -0xF;
    *(s16 *)(*(u8 **)(p + 0x4) + 0x88) = 0x9A;
    *(s16 *)(*(u8 **)(p + 0x4) + 0x8A) = 0x69;
    *(s16 *)(*(u8 **)(p + 0x8) + 0x68) = -0xF;
    *(s16 *)(*(u8 **)(p + 0x8) + 0x60) = 0x800;
    if (b != 0)
    {
        func_800DC628(p, 0x400);
    }
    else
    {
        func_800DC628(p, 0);
    }
}

/* Rebuild the three tag chains and hand back the cursor func_800DCA1C stopped
   at; func_800DBE98's own result is dropped. 22 words, 0 differing. */
s32 func_800DCBE4(u8 *p, s32 flag)
{
    s32 r;

    func_800DCA88(p, flag);
    func_800DCAC4(p, flag);
    r = (s32)func_800DCA1C(*(u8 **)p, flag);
    func_800DBE98();
    return r;
}

/* The engine's two GPU-primitive templates stamped into nine 0x10-byte and
   nine 0xC-byte slots off the record's +0x04 and +0x08 pointers.

   BOTH TEMPLATES ARE REACHED BY SUBSCRIPT, not through a `u32 *` pointer
   local: the subscript keeps the symbol's own `high` for element 0 --
   `lw %lo(D_8002E5D8)($t3)` -- while cse materialises the full address for
   elements 1..3. The pointer local eats that `%lo` and gives `lw 0x0($t0)` in
   both copies: 46 words, 9 differing. Mixing the two is 48 / 47. The
   destination base is re-read out of the record for every store.
   46 words, 0 differing. */
void func_800DCC3C(u8 *p)
{
    s32 i;

    for (i = 0; i < 9; i++)
    {
        *(u32 *)(i * 0x10 + *(u32 *)(p + 4)) = D_8002E5D8[0];
        *(u32 *)(i * 0x10 + *(u32 *)(p + 4) + 4) = D_8002E5D8[1];
        *(u32 *)(i * 0x10 + *(u32 *)(p + 4) + 8) = D_8002E5D8[2];
        *(u32 *)(i * 0x10 + *(u32 *)(p + 4) + 0xC) = D_8002E5D8[3];
        *(u8 *)(i * 0x10 + *(u32 *)(p + 4) + 0xF) = 4;
        *(u32 *)(i * 0xC + *(u32 *)(p + 8)) = D_8002E5E8[0];
        *(u32 *)(i * 0xC + *(u32 *)(p + 8) + 4) = D_8002E5E8[1];
        *(u32 *)(i * 0xC + *(u32 *)(p + 8) + 8) = D_8002E5E8[2];
    }
}

/* 17 words, 0 differing. */
s32 func_800DCCF4(u8 *p, void *a)
{
    *(u8 **)(p + 0x4) = D_800E2828;
    *(u8 **)(p + 0x8) = D_800E28B8;
    *(void **)(p + 0x0) = a;
    *(u8 **)(p + 0xC) = D_800E2804;
    func_800DCC3C(p);
    return 1;
}

/* Allocate the tag-builder node and wire its handler.

   THREE PLACEMENT FACTS. (1) `p = (u8 *)n + 0x20` must sit ABOVE the
   `if (b != 0)`: inside the arm it is a once-set pseudo with one use,
   update_equiv_regs sinks its `addiu` to the store and $s2 never enters the
   frame -- 50 words against 53. (2) 0x800133A0 is a CONSTANT ADDRESS, stored
   as a value at +0x58 and dereferenced for the call's second argument; the
   `lui`/`ori` pair rather than `lui`/`addiu` is what says so. (3) The load
   has to be read into a local BEFORE the store, or the store is emitted ahead
   of the `lw` instead of into the `jal`'s delay slot (3 differing).
   53 words, 0 differing. */
UnkNode *func_800DCD38(void)
{
    UnkNode *n;
    u8 *p;
    u8 *q;
    u32 b;
    s32 c;

    n = func_8003FC64(0);
    if (n != NULL)
    {
        D_800E5910 = (u8 *)n;
        p = (u8 *)n + 0x20;
        b = func_8004B404(0x100);
        if (b != 0)
        {
            q = (u8 *)n + 0x5C;
            func_800DCCF4(q, (void *)b);
            c = *(s32 *)0x800133A0;
            *(u8 **)(p + 0x38) = (u8 *)0x800133A0;
            n->unk_0C = func_800DCBE4(q, c);
            func_800DC988(n);
            n->unk_10 = (u32)func_800DC8F8;
            func_8004491C(n, (u32)D_8004CAA0);
            return n;
        }
        n->unk_1E |= 0x8000;
        D_800814A0 |= 0x8000;
        n = NULL;
    }
    return n;
}

/* `UnkNode *` IS WHAT LETS THE SCHEDULER INTERLEAVE. With `u8 *n` and
   `*(u16 *)(p + 0x1E) |= 0x8000`, the load of D_800814A0 cannot move above
   the `sh` and the function is 22 words with a load-delay nop; as a
   COMPONENT_REF, MEM_IN_STRUCT_P separates it from the scalar global and
   both `|=` interleave. 21 words, 0 differing. */
void func_800DCE0C(UnkNode *n)
{
    func_8004B1A4(*(s32 *)((u8 *)n + 0x5C));
    D_800E5910 = NULL;
    n->unk_1E |= 0x8000;
    D_800814A0 |= 0x8000;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800D50FC", D_80089490);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800D50FC", D_80089498);
