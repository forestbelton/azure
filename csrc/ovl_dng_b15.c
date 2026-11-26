#include "common.h"

s32 rand(void);

s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
u8 *func_800A02AC(u8 *p, u16 x, u16 y);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
u16 func_800A6D30(void);
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s32 func_8009A8C0(u32 f, u8 *c, u8 *d, s16 y);
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
s16 func_800A0134(u8 *a, u8 *b);
s16 func_8009A540(s32 a, s32 x, s32 y, s16 z);
s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
void func_8009A3D0(s32 x, s32 y, s32 f);
void func_8009A21C(s32 a, s32 b, s32 c);
s32 func_8009A180(u8 *a, u8 *b);
s16 func_800BCB04h(s32, s32, s16) __asm__("func_800BCB04");
s32 func_b15_80172920(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s16 D_8006CD00[];
extern u8 D_80082E80[];
extern u16 D_80082EA4;
typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
extern UnkDngRoom2970 D_800E2970_s[] __asm__("D_800E2970");

extern s16 D_80083228;
extern u8 D_b15_80174634[];
extern u8 D_80174684[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b15_801721D8(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800AC7EC();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b15_801719DC();

s32 func_800AB778();

s32 func_800AD058();

s32 func_800AB464();

s32 func_800AB1C0(u8 *, UnkVec *, u8 *, u8 *);
void func_800AD594(u8 *, s32);
void func_800A4ACC(u8 *);
typedef struct UnkDng708A4SP
{
    u8 pad_00[0x4];
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    u8 pad_0A[0x12];
    u8 *unk_1C;
    u8 pad_20[0x4];
    u8 *unk_24;
    u8 pad_28[0xA8];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDng708A4SP;
typedef struct UnkDngB09A28
{
    s16 unk_00;
    s16 unk_02;
} UnkDngB09A28;
typedef struct UnkDngB09A28Tbl
{
    UnkDngB09A28 unk_00[8];
} UnkDngB09A28Tbl;

/* func_800D5AC0 (csrc/ovl_dungeon_800D50FC.c), byte for byte: one
   semi-transparent 1x1 tile and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad, whose layout here is the
   twin's exactly (+0x1C cursor, +0x24 OT base, +0xD0/+0xD4 out-parameters,
   +0x100 index). The three dead lbu's at +0x4/+0x5/+0x6 are the
   setRGB0(t, t->r0, t->g0, t->b0) echo of the colour word just copied in;
   the twin's own comment carries the measurements for that and for its three
   load-bearing orderings, which were not re-measured here because the first
   compile of the twin's body landed. 120 words, 0 differing. */
UnkNode *func_8003FD64(s32, void *);
s16 func_800A2CB8(u8 *a, u8 *b);
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);
void func_b15_80170A64(u8 *p, UnkVec *v);

extern u8 D_80083498[];
extern u8 D_80045340[];
s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
s32 func_800AA36C(u8 *, void *, u8 *, u8 *);
void func_b15_801713A8();
extern u8 D_8017468C[];
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
void func_800A9A04(u8 *);
s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
s32 func_800AA79C(u8 *, void *, u8 *, u8 *);
void func_b15_801743F0(u8 *p, u8 *b, u8 *c, u8 *d);
extern u8 D_8017466C[];
void func_800AD4D0(u8 *);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
void func_800A2B04(UnkVec *, s32, s32);
s32 func_800AB538();

void func_b15_801721D8(u8 *p, void *b, void *c, u8 *d);
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
s16 func_8009FB34(u16 x, u16 y);
s16 func_800A1C58(u8 *p);
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
void func_800AAF00(u8 *a, UnkVec *b, u8 *c, s32 t, void (*h)());
void func_b15_80171FC8(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_b15_80172AAC(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_b15_80172B8C(u8 *a, void *b, void *c, s32 d);
extern u8 D_8017467C[];
extern u8 D_80174644[];
extern u8 D_80174674[];
extern u8 D_80174694[];
extern s32 D_8008346C;
UnkNode *func_8003FC64(s32);
s32 func_8003F270(void);
s32 func_800A56E0(s32);
s32 func_800A05A4(u8 *p, s32 x, s32 y, s16 dir, s32 n);
s32 func_800A94A0(u8 *a, u8 *q, s32 c, u8 *e);
typedef struct UnkDngB15Name12
{
    u8 b[0xC];
} UnkDngB15Name12;

s32 func_b15_80170884(u8 *p, UnkVec *v)
{
    UnkDng708A4SP *sp = (UnkDng708A4SP *)0x1F800000;
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

/* The same fading-burst step as func_b15_80170BFC above, driven by the six
   velocities func_b15_80170D28 installs instead of by a halving: integrate
   the mover from the payload's +0x40/+0x44/+0x48, advance those three by
   +0x4C/+0x50/+0x54, and -- once the mover's two integer halves have come
   within 0x10 of the parent position parked at +0x36/+0x38 -- raise the
   node's and the engine's 0x8000 flags. Then the same colour scaling, with
   a constant 256 in place of func_b15_80170BFC's +0x34 divisor, and the
   same expiry on an 8-per-frame countdown. 102 words, 0 differing, first
   compile.

   THE PROXIMITY TEST IS A PLAIN INLINE `abs()' AND BOTH ARMS COME OUT IN
   PLACE. Each triple is `bgez'/real-`nop'/`negu' on one register, which is
   mips.md's two-word in-place arm of `abssi2' -- the subtraction dies at the
   `abs', so the allocator gives the `abs' its source's register and the nop
   is the pattern's own delay slot, not a missed fill
   (docs/matching-levers.md :5218, :19704). */
void func_b15_80170A64(u8 *p, UnkVec *v)
{
    v->unk_00 += *(s32 *)(p + 0x40);
    v->unk_04 += *(s32 *)(p + 0x44);
    v->unk_08 += *(s32 *)(p + 0x48);
    *(s32 *)(p + 0x40) += *(s32 *)(p + 0x4C);
    *(s32 *)(p + 0x44) += *(s32 *)(p + 0x50);
    *(s32 *)(p + 0x48) += *(s32 *)(p + 0x54);
    if (abs(*(s16 *)(p + 0x36) - *(s16 *)((u8 *)v + 0x2)) < 0x10 &&
        abs(*(s16 *)(p + 0x38) - *(s16 *)((u8 *)v + 0x6)) < 0x10)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    p[4] = p[0] * *(s16 *)(p + 0x32) / 256;
    p[5] = p[1] * *(s16 *)(p + 0x32) / 256;
    p[6] = p[2] * *(s16 *)(p + 0x32) / 256;
    *(s32 *)(p + 0x8) = *(s32 *)(p + 0x4);
    if ((*(s16 *)(p + 0x32) -= 8) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* One frame of a fading burst, and a leaf: func_800D5CA0
   (csrc/ovl_dungeon_800D50FC.c) byte for byte, with nothing relinked -- the
   only symbol it touches outside itself is D_800814A0, which is resident.
   Integrate the mover, halve each of its three velocities, scale the three
   colour bytes by the remaining life over the original life, copy the colour
   word forward, and when the life runs out raise the node's and the engine's
   0x8000 flags. The three `srl 31'/`addu'/`sra 1' triples are the signed
   `/= 2'; `>>= 1' would drop the correction. 75 words, 0 differing, first
   compile. func_b15_80170F2C below installs it as its step function. */
void func_b15_80170BFC(u8 *p, UnkVec *v)
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

/* A 0x211 spawner with a timed arrival: seed the new node's mover from the
   parent `a's, offset each axis by the caller's own `dx'/`dy'/`dz' (and the
   z by a further -0x64), keep the parent's un-offset position at the
   payload's +0x36/+0x38/+0x3A, then give the payload six velocities -- the
   offset undone over `f'/8 steps, halved at +0x40/+0x44/+0x48 and quartered
   at +0x4C/+0x50/+0x54 -- and park func_b15_80170A64 as the step and
   func_b15_80170884 as the draw. The skeleton is func_b06_80174214
   (csrc/ovl_dng_b06.c) and func_800D6AD4 (csrc/ovl_dungeon_800D50FC.c): same
   7-parameter shape, same `n->unk_08' re-dereferenced at every statement,
   same `*(u32 *)p = c; *(u32 *)(p + 0x8) = c;' tail. 129 words, 0 differing.

   THE SIX VELOCITY STORES ARE WRITTEN OUT IN FULL, WITH NO QUOTIENT LOCALS,
   and CSE does all the sharing. With `vx'/`vy'/`vz' holding the three
   divisions it is 127 words and 19 aligned differences: the three quotients
   become global allocnos, `vz' no longer gets $v1 as the divisor frees it,
   the first `/ 4' copy lands in $v1 instead of $v0 and is then small enough
   to schedule into the `mflo' delay slot, which eats the two words.

   THE DIVISOR IS WRITTEN INLINE AT EACH SITE, NOT HOISTED INTO A LOCAL.
   `dv = f / 8;' ahead of the group puts the whole `f / 8' expansion before
   the first numerator, so `sll $v0, $s3, 16' can no longer sit in front of
   the `bgez' and its `negu' can no longer take that branch's delay slot --
   and the three `div's then cluster at the top of the block: 123 words, 49
   aligned differences. Written at each site, operand order expands the
   numerator first and CSE reuses the quotient in $v1 for divisions two and
   three.

   `dx'/`dy'/`dz' AND `f' ARE `s16'. As `s32' the three stack parameters keep
   their incoming MEMs and are rematerialised at each use instead of being
   held in $s3/$s4/$s5, which is func_b06_80174214's own measurement; here
   they have three uses each, so it is not a near thing. */
void func_b15_80170D28(UnkNode *a, s16 b, u32 c, s16 f, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_b15_80170A64;
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2) + dx;
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6) + dy;
        *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA) + dz - 0x64;
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x36) = *(u16 *)((u8 *)a->unk_08 + 0x2);
        *(s16 *)(p + 0x38) = *(u16 *)((u8 *)a->unk_08 + 0x6);
        *(s16 *)(p + 0x3A) = *(u16 *)((u8 *)a->unk_08 + 0xA);
        *(s32 *)(p + 0x40) = -(dx << 16) / (f / 8) / 2;
        *(s32 *)(p + 0x44) = -(dy << 16) / (f / 8) / 2;
        *(s32 *)(p + 0x48) = -(dz << 16) / (f / 8) / 2;
        *(s32 *)(p + 0x4C) = -(dx << 16) / (f / 8) / 4;
        *(s32 *)(p + 0x50) = -(dy << 16) / (f / 8) / 4;
        *(s32 *)(p + 0x54) = -(dz << 16) / (f / 8) / 4;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = f;
        func_8004491C(n, (u32)func_b15_80170884);
        *(u32 *)p = c;
        *(u32 *)(p + 0x8) = c;
    }
}

/* The sibling spawner of func_b15_80170D28 above, and the near relink of
   func_800D5DCC (csrc/ovl_dungeon_800D50FC.c): the same 0x211 node, the same
   `n->unk_08' re-dereferenced at every statement, the same +0x6 life of 6 on
   `n->unk_0C' and the same `*(u32 *)p = c; *(u32 *)(p + 0x8) = c;' tail.
   Three deltas from that twin, all read off the listing: the position offset
   is the caller's own `dx'/`dy'/`dz' (z again less 0x64) rather than a
   D_800E2468 row, the random velocity is rand rather than rand()
   and is `<< 7' on all three axes rather than 6/6/5, and there is no
   constant drift along the row -- so the step is func_b15_80170BFC and the
   +0x32/+0x34 life is 5. 85 words, 0 differing.

   THE FOURTH PARAMETER IS NEVER READ IN THE BODY AND MUST STILL BE DECLARED.
   Only `dx'/`dy'/`dz' come off the stack, at 0x40/0x44/0x48 of a 0x30 frame,
   which makes them arguments five through seven; with six parameters `dx'
   arrives in $a3 and it is 6 aligned differences -- the $s2 prologue pair
   swaps for a `move' and the other two stack loads shift by four. It is
   named `f' because func_b15_80170D28, the same spawner with a timed
   arrival, reads its fourth parameter as the arrival time; here the +0x32
   and +0x34 life is the constant 5 instead, so the slot is only carried. */
void func_b15_80170F2C(UnkNode *a, s16 b, u32 c, s16 f, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_b15_80170BFC;
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2) + dx;
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6) + dy;
        *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA) + dz - 0x64;
        *(s16 *)((u8 *)n->unk_0C + 0x6) = 6;
        *(s32 *)((u8 *)n->unk_08 + 0xC) = ((rand() & 0x7FFF) - 0x4000) << 7;
        *(s32 *)((u8 *)n->unk_08 + 0x10) = ((rand() & 0x7FFF) - 0x4000) << 7;
        *(s32 *)((u8 *)n->unk_08 + 0x14) = ((rand() & 0x7FFF) - 0x4000) << 7;
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = 5;
        *(s16 *)(p + 0x34) = 5;
        func_8004491C(n, (u32)func_b15_80170884);
        *(u32 *)p = c;
        *(u32 *)(p + 0x8) = c;
    }
}

/* One frame of a projected, fading sprite: wind the renderable's three
   +0x1A/+0x1C/+0x1E words on, ease its three colour bytes by the payload's
   remaining life over its original life, project the mover through
   RotTransPers and set c+0x6 to the depth less 8, then raise the node's and
   the engine's 0x8000 flags when the life runs out. 84 words, 0 differing.

   NO TWIN ANYWHERE: dngtwin finds no carved sibling at 0.70 and xsibs finds
   no resident one at all (this is one of the 12 of this unit's 23 stubs it
   returns nothing for). Written from the listing, with two facts carried in
   from func_b23_80170874 (csrc/ovl_dng_b23.c), which is the same projection
   idiom at a different payload.

   THE FRAME FACT CARRIES AND IT IS WORTH TWO WORDS: RotTransPers' second
   out-parameter is EIGHT bytes, `s32 sxy[2]', which is what puts the third
   and fourth at sp+0x20 and sp+0x24. Declared `s32 sxy' it is 84 words /
   2 aligned differences. Confirmed on a -0x38 frame here against
   func_b23_80170874's, so the fact is the ARGUMENT's, not that body's
   allocation.

   THE WIND BLOCK'S STATEMENT ORDER IS +0x1C, +0x1E, +0x1A -- NOT the order
   the three `sh's come out in, which is +0x1C, +0x1A, +0x1E. All six orders
   measured, 84 words each:
       1C 1E 1A   0      <- kept
       1E 1C 1A   4
       1C 1A 1E   8
       1A 1E 1C  10
       1A 1C 1E  12
       1E 1A 1C  12
   The two `-0x1F4' statements are adjacent in the source and sched splits
   them apart; written adjacent in the emitted order they are 8.

   THE REPEATED `(*(s16 *)(p + 0x1C) - *(s16 *)(p + 0x1A))' IS WRITTEN OUT
   THREE TIMES, once per colour byte. Hoisted into a local it is 80 words /
   28 aligned differences -- the four missing words are the two `lh's the
   target reloads for the second and third divides, so cse does NOT unify
   them across the `div's and the source really does repeat the expression.

   Measured and FREE at 84 / 0, so it is not the reason this lands, and it
   WEAKENS func_b23_80170874's second carried fact: the countdown as
   `if ((s16)(--*(u16 *)(p + 0x1A)) <= 0)' against a plain `-= 1;' statement
   followed by `if (*(s16 *)(p + 0x1A) <= 0)'. The twin prices its combined
   form as load-bearing on its own body; on this frame the two are
   byte-identical, because with nothing between the decrement and the test
   cse keeps the value and the `sll'/`bgtz' pair comes out either way. */
void func_b15_80171080(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    SVECTOR sv;
    s32 sxy[2];
    s32 pz;
    s32 flag;

    *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) - 0x1F4;
    *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - 0x1F4;
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x190;
    c[0xC] = p[0x2] * (*(s16 *)(p + 0x1C) - *(s16 *)(p + 0x1A)) / *(s16 *)(p + 0x1C);
    c[0xD] = p[0x3] * (*(s16 *)(p + 0x1C) - *(s16 *)(p + 0x1A)) / *(s16 *)(p + 0x1C);
    c[0xE] = p[0x4] * (*(s16 *)(p + 0x1C) - *(s16 *)(p + 0x1A)) / *(s16 *)(p + 0x1C);
    sv.vx = *(u16 *)((u8 *)b + 0x2);
    sv.vy = *(u16 *)((u8 *)b + 0x6);
    sv.vz = *(u16 *)((u8 *)b + 0xA);
    *(s16 *)(c + 0x6) = RotTransPers(&sv, sxy, &pz, &flag) - 8;
    if ((s16)(--*(u16 *)(p + 0x1A)) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x112 spawner: allocate the node, park func_b15_801713A8 as its
   handler and D_b15_80174634 as its facing table, set one of three flag
   pairs off `a & 3', optionally re-dress through func_800A48F0 with
   D_80174684, and hand the result to func_800A9C18 and func_800AA36C.
   118 words, 0 differing, ON THE FIRST COMPILE.

   THE TWIN IS THE LANDED func_b16_80170878 (csrc/ovl_dng_b16.c), 120 words,
   BYTE-IDENTICAL FRAME (addiu $sp, -0x38; $s0..$s7 + $ra in the same order).
   Yours is 2 WORDS SHORTER AND IT IS A MISSING STATEMENT, not missing
   references: the twin's `*(s16 *)(w + 0x92) = -0x20;' before the
   func_800AA36C call is absent here. Everything else is operands --
   `p[0x13]' is 15 against the twin's 16, and the handler, the facing table,
   the re-dress table and the +0x8C step are this bank's
   func_b15_801713A8 / D_b15_80174634 / D_80174684 / func_b15_801719DC.
   The twin's own +0x1C 0x40000 flag IS here.

   Two of the twin's unmeasured spellings priced here, both LOAD-BEARING:
   `w' is a SEPARATE local aliasing `p' and collapsing the two into one is
   117 words / 8 aligned differences; and the third arm's gate is `~3', with
   `(a & 0xFFFC) == 0' at 116 words / 4, because the target's
   `and'/`sll 16'/`bnez' needs the constant to stay a sign-extended -4. */
u8 *func_b15_801711D0(s16 a, u16 b, u16 c, s16 d)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *p;
    u8 *w;
    s32 k;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        w = p;
        n->unk_10 = (u32)func_b15_801713A8;
        p[0x13] = 15;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x25) = c;
        *(u8 **)((u8 *)r + 0x2C) = D_b15_80174634;
        k = a & 3;
        *((u8 *)r + 0x24) = b;
        if (k == 1)
        {
            *(u32 *)(p + 0x14) |= 0x6000;
            *(u32 *)(p + 0x1C) |= 0x6000;
        }
        else if (k >= 2)
        {
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
        }
        else if ((a & ~3) == 0)
        {
            if ((*(u32 *)(p + 0x14) & 0x200) == 0)
            {
                if ((func_800A6D30() & 1) != 0)
                {
                    func_800A48F0((s8 *)p, 1, (func_800A6D30() & 0x3F) | 0x20);
                    *(u8 **)((u8 *)r + 0x2C) = D_80174684;
                }
            }
        }
        func_800A9C18(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_b15_801719DC;
        *(u32 *)(p + 0x1C) |= 0x40000;
        func_800AA36C(w, m, (u8 *)r, p);
    }
    return p;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b15", func_b15_801713A8);

/* The banked room script's +0x8C idle handler -- the body three of this unit's
   landed installers arm at p + 0x8C -- and the relinked twin of the landed
   func_b23_80171760 (csrc/ovl_dng_b23.c, 350 words, dngtwin 0.914).
   379 words, 0 differing, ON THE FIRST COMPILE.

   IT OWNS jtbl_b15_80170808, the 12-entry table at rodata offset 0x0,
   8-aligned, no pad: cases 1 2 3 -> the func_800AAF00 arm, 4 10 11 -> the
   `default' label, 5 6 7 -> the func_800A0818 / D_800814A8 arm, 8 AND 9 ->
   one shared func_b15_80172920 / func_b15_80172AAC arm, 12 -> func_800A9A0C
   written as a `case' label inside the `w[0x9A] != 0x11' arm. The emitted
   table was read back out of a standalone cc1psx run and its twelve entries
   fall in the target's equivalence classes exactly.

   SAME FRAME AS b23 (-0x38, $s0..$s4 + $ra) BUT A DIFFERENT PARAMETER MAP:
   b23 is p->$s0, b->$s3, c->$s1, d->$s2 and this copy is p->$s2, b->$s0,
   c->$s1, d->$s3, so the save ORDER differs (b23 stores s0,s3,s1,s2,ra,s4;
   here s2,s0,s1,s3,ra,s4). Per the brief's rule that is a warning that b23's
   ALLOCATION facts will not carry, and the one it fought for does not.

   b23's ONE FOUGHT-FOR FACT IS FREE HERE. b23 needs its three `goto deflt'
   gates to share ONE block after the switch and prices three written-out
   copies at 66 aligned differences, every row a $s0/$s1 exchange. On this
   body the three written-out copies measure 379 words / 0 differing -- jump.c
   cross-joins them by itself because `c' sits at a different reference count
   -- so the plain goto-free C below is what is kept. Cases 8 and 9 written
   out as two identical arms is also 379 / 0, same mechanism.

   DELTAS FROM b23, all read off the listing: no state-0xE rand seed; both
   dress tails gain `c[0x5] = 1' and the five-store +0xA4..+0xB8 reset and
   pass 1 as func_80047784's third argument where b23 passes 0; the +0x80000
   arm gains the +0x92 subtraction, the same reset and func_b15_801743F0
   where b23 calls func_b23_80174598 alone; cases 8 and 9 share one arm; the
   `d + 0x1C |= 0x40000' raise is new. The relink is a rename: D_80174EF0 ->
   D_80174684, D_80174EE8 -> D_8017467C, D_80174E88 -> D_b15_80174634,
   D_80174E98 -> D_80174644, D_80174EE0 -> D_80174674, and every one was read
   back per `%hi'/`%lo' site; relcand.py reports ok on 11 data symbols and 23
   calls.

   FIVE FACTS MEASURED, BOTH SPELLINGS PRICED:

   | spelling                                        | words | aligned |
   | as written below                                |  379  |    0    |
   | `case 12' as its own case after cases 1 2 3     |  379  |   10    |
   |   rather than inside the `!= 0x11' arm          |       |         |
   | the +0xA4 -> +0xA8 copy written LAST of the     |  379  |    4    |
   |   five reset stores instead of first, at both   |       |         |
   |   dress tails -- 2 per site, the `lw'/`sh' pair |       |         |
   |   transposed, which is the price this unit's    |       |         |
   |   own func_b15_80173C40 already records         |       |         |
   | the 0x40000 raise before the +0x98 mask         |  381  |   13    |
   | no `w' local -- D_800814A8 + 0x9A read inline   |  380  |   16    |
   | no `m' local -- the func_800A0818 result stored |  379  |   25    |
   |   straight into d + 0x2A, with `w' kept         |       |         |

   FOUR PLAUSIBLE FACTS MEASURED AND FREE, so they are NOT why this lands:
   the `e'/`e2'/`e3' facing-table locals (all three replaced by the bare
   D_8017467C / D_b15_80174634 symbol is 379 / 0, as b06 measured for the
   same idiom -- cse reuses the just-stored pointer either way); the `v'
   local holding d + 0x1C in the tail (379 / 0); the `d + 0x64' gate written
   as two nested `if's instead of one `&&' (379 / 0, which is the narrowed
   fold_truthop rule -- the two tests are on different objects); and the
   written-out `deflt' and case-8/9 arms above. */
void func_b15_801719DC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;
    s32 v;
    u16 x;
    u8 *e;
    u8 *e2;
    u8 *e3;
    s32 m;
    u8 *w;

    if ((D_80083462 & 0x1000) != 0)
    {
        p[0x9A] = 0xE;
        func_b15_80171FC8(p, b, c, d);
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_80174684)
        {
            e = D_8017467C;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_80174684)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, D_8017467C) != 0)
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
            e2 = D_b15_80174634;
            if (*(u8 **)(c + 0x2C) != e2)
            {
                *(u8 **)(c + 0x2C) = e2;
                func_80047784(c, e2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 1);
                c[0x5] = 1;
                *(s32 *)(p + 0xA8) = *(s32 *)(p + 0xA4);
                *(s16 *)(p + 0xA6) = 0;
                *(s16 *)(p + 0xB8) = 0;
                *(s32 *)(p + 0xAC) = 0;
                *(s32 *)(p + 0xB0) = 0;
            }
            p[0x9A] = 0xE;
        }
        *(u16 *)(p + 0x98) &= ~0xC;
        *(s32 *)(d + 0x1C) |= 0x40000;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_80174644) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            *(u16 *)(p + 0x92) -= *(u16 *)(p + 0xA6);
            *(s32 *)(p + 0xA8) = *(s32 *)(p + 0xA4);
            *(s16 *)(p + 0xA6) = 0;
            *(s16 *)(p + 0xB8) = 0;
            *(s32 *)(p + 0xAC) = 0;
            *(s32 *)(p + 0xB0) = 0;
            func_b15_801743F0(p, (u8 *)b, c, d);
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
        if (*(u16 *)(c + 0x24) == D_80082EA4)
        {
            func_b15_801721D8(p, b, c, d);
            return;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 &&
                (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if ((s16)func_b15_80172B8C(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_b15_801721D8(p, b, c, d);
                return;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 8:
        case 9:
            if ((s16)func_b15_80172920(p, b, c, d) != 0)
            {
                return;
            }
            func_b15_80172AAC(p, b, c, d);
            return;
        case 5:
        case 6:
        case 7:
            m = func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
            w = (u8 *)D_800814A8;
            *(s16 *)(d + 0x2A) = m;
            if (w[0x9A] != 0x11)
            {
            case 12:
                func_800A9A0C(d);
                return;
            }
        case 1:
        case 2:
        case 3:
            func_800AAF00(p, b, c, (s32)D_80174674, func_b15_801719DC);
            return;
        default:
            func_b15_801721D8(p, b, c, d);
            return;
        }
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
                    *(s16 *)(d + 0x2A) =
                        func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
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
    e3 = D_b15_80174634;
    if (*(u8 **)(c + 0x2C) == e3)
    {
        return;
    }
    *(u8 **)(c + 0x2C) = e3;
    func_80047784(c, e3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 1);
    c[0x5] = 1;
    *(s32 *)(p + 0xA8) = *(s32 *)(p + 0xA4);
    *(s16 *)(p + 0xA6) = 0;
    *(s16 *)(p + 0xB8) = 0;
    *(s32 *)(p + 0xAC) = 0;
    *(s32 *)(p + 0xB0) = 0;
}

/* The banked room script's walk step with a same-tile short circuit, the
   relinked twin of func_b02_8017169C (csrc/ovl_dng_b02.c) -- itself the
   trimmed form of the resident func_800D904C (csrc/ovl_dungeon_800D50FC.c):
   no func_8009A66C call, no `(s16)t == 3' arm, func_800A0818 called AFTER
   func_8009A21C with the `x'/`y' locals straight, and the D_b15_80174634
   dress carrying only the `!=' arm. The one thing this copy adds over that
   twin is an arm that is in none of its five relinked siblings: when the
   cursor's path byte pair already equals c + 0x24/0x25 the step is skipped
   and p + 0x92 / d + 0x88 are slid by 0x20 instead. 132 words, 0 differing.

   THE d + 0x88 READ MUST BE HOISTED INTO `w' AHEAD OF THE p + 0x92 STORE.
   Written as three read-modify-writes in listing order the target's
   `lhu 0x88($s0)' cannot move up into the delay slot of `lhu 0x92($s2)':
   $s2 is `p' and $s0 is `d', so the scheduler will not lift a load across a
   store through the other pointer, and the slot takes a nop -- 133 words,
   87 differing (every word after it shifted). With `w' the two loads are
   adjacent in the RTL before either store and the slot fills, which is the
   whole one-word delta. */
void func_b15_80171FC8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 n;
    s32 w;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_b15_80174634)
        {
            *(u8 **)(c + 0x2C) = D_b15_80174634;
            func_80047784(c, D_b15_80174634[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        if (c[0x24] == *(d + *(s16 *)(d + 0x8A) + 0x74) && c[0x25] == *(d + *(s16 *)(d + 0x8A) + 0x7C))
        {
            w = *(u16 *)(d + 0x88) - 0x20;
            *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + 0x20;
            *(s16 *)(d + 0x88) = w;
            *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) + 1;
        }
        else
        {
            x = c[0x24];
            y = c[0x25];
            func_8009A3D0(x, y, (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            c[0x24] = *(d + *(s16 *)(d + 0x8A) + 0x74);
            c[0x25] = *(d + *(s16 *)(d + 0x8A) + 0x7C);
            *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) + 1;
            func_8009A21C(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            *(s16 *)(d + 0x2A) = func_800A0818(x, y, c[0x24], c[0x25], (u16 *)(p + 0x98));
        }
        p[0x9A] = 0xF;
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

/* The banked room script's walk step, the relinked twin of the resident
   overlay's func_800D92C0 (csrc/ovl_dungeon_800D50FC.c) with three extra
   pre-loop arms: the +0x400/+0x10 pair that re-aims through func_800A02AC or
   func_800A04F0, and the +0x2000/+0x20000 arm that compares the cursor
   against D_80082E80's. Everything after that is func_800D92C0's eight-step
   D_8006CD00 sweep. 466 words, 0 differing.

   THE SECOND PARAMETER IS NEVER READ IN THE BODY AND MUST STILL BE DECLARED
   AND PASSED ON. func_b15_80172920's call sets only $a0, $a2 and $a3, so its
   $a1 is this function's own incoming $a1, reaching it untouched; the
   parameter is there to carry it.

   func_800BCB04 HAS TO COME THROUGH THE s16 ALIAS, AND IT DECIDES THE WHOLE
   FUNCTION'S REGISTER ASSIGNMENT. `i = func_800BCB04h(...)' is a halfword
   assignment into `i' itself; with the landed `s32 func_800BCB04' and an
   `(s16)' cast the result needs a temporary instead, which takes `i's
   allocno from 17 references over 113 insns down to 14 over 107. The
   priority global.c's allocno_compare computes is
   floor_log2(refs)*refs/live_length, so 4*17/113 beats `d's 6*85/860 and `i'
   is allocated first and takes $s1, while 3*14/107 does not and `i', `d' and
   `c' come out rotated: 466 words, 126 differing.

   AND THE LOOP'S HEADING VALUE SHARES `e' WITH THE TWO ACTOR LOOKUPS. In its
   own local of any type or spelling it is 3 differing -- the `lh' of d + 0x2A
   lands in $a0 where the target has $v1 -- because global.c's
   expand_preferences gives it `s's hard-register preference (it dies in the
   insn that sets `s') and `s' prefers $a0, where the `sll'/`sra' feeding
   func_8009A8C0's first argument is local-allocated. Sharing `e', whose other
   live range is in the pre-loop arms, gives the allocno the hard-register
   conflicts that put $a0 out of reach, and find_reg keeps $v1.

   Measured, on this body, by deletion: writing the 0x20000 arm's
   D_80082E80 through a local is 465 words / 23 differing, and the resident
   twin's own note explains why the D_8006CCE8 lookup must stay inline. The
   first gate with the func_800A9A0C call as the *taken* arm rather than the
   if-body is 11 differing. The tail's D_80083460 through `g' instead of its
   own `g2' is 3 differing. The `(s16)' casts on the three s32-returning
   callees are load-bearing -- func_8009A8C0 2 words, func_b15_80172920 1,
   func_8009A180 1 -- while those on the s16-returning func_800A0134,
   func_8009FD7C and func_8009A540 measure 0 and are left off.

   D_800E2970 is indexed as an array of a 0x14-byte record so the +0xC read
   keeps its own displacement; written as `*(u16 *)(D_800E2970 + j * 0x14 +
   0xC)' the constant folds into the `%lo' and the displacement goes to 0. */
void func_b15_801721D8(u8 *p, void *b, void *c, u8 *d)
{
    u8 *g;
    s32 cc2;
    u8 *g2;
    s32 e;
    u8 *t;
    u8 *t80;
    u16 *w;
    s16 *tbl;
    s16 f;
    s16 i;
    s16 m;
    s32 s;
    s32 v;
    s32 j;
    s32 k;
    s32 kk;
    s32 t1;
    s32 x;
    s32 y;

    g = (u8 *)&D_80083460;
    f = 0;
    if ((*(u16 *)(g + 0x2) & 0x4000) != 0 || *(s8 *)(d + 0x71) >= 0)
    {
        if (d[0x12] >= 2 || (s16)func_b15_80172920(p, b, c, d) == 0)
        {
            func_800A9A0C(d);
            return;
        }
        if (*(u8 **)(g + 0xC) == d)
        {
            *(u16 *)(d + 0x46) = 0xC008;
        }
        return;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
    {
        return;
    }
    func_800A19E4((s32)c, d, 3, 6, p + 0x9C);
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x410) != 0)
    {
        if ((v & 0x400) != 0)
        {
            e = (s32)func_800A02AC(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25));
            if (e != 0)
            {
                t = *(u8 **)(e - 0x14);
                *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t[0x24], t[0x25], (u16 *)(p + 0x98));
                d[0x71] &= 0x7F;
                return;
            }
            /* DEVICE: the constant in a local, and the read-modify-write
               wrapped in a do-while(0). Worth 4 differing: without it the
               function still builds at 466 words but the loaded word and
               the `lui 0x8000' swap $v0 and $v1. Plain
               `*(s32 *)(d + 0x14) |= 0x80000000;' is 4 differing, as are
               `= *(s32 *)(d + 0x14) | 0x80000000', `|= (s32)0x80000000' and
               hoisting the loaded value into a local of its own. No
               uninitialised read, so nothing warns under -Wall. Same
               construct, same place, as the landed func_800D92C0's `cc2'. */
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
        else
        {
            if (func_800A04F0_r(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25), *(s16 *)(d + 0x2A)) != NULL)
            {
                d[0x71] &= 0x7F;
                return;
            }
        }
    }
    else if ((v & 0x2000) != 0)
    {
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((v & 0x20000) != 0)
            {
                k = ((d[0x45] + ((s16)(*(u16 *)(D_800814A8 + 0x2A)) >> 9)) & 7) * 2;
                x = D_80082E80[0x24] + *(u16 *)(k + (s32)D_8006CCD8);
                y = D_80082E80[0x25] + *(u16 *)(k + (s32)D_8006CCE8);
                if (*(u8 *)(c + 0x24) == (u16)x && *(u8 *)(c + 0x25) == (u16)y)
                {
                    d[0x71] &= 0x7F;
                    return;
                }
                w = (u16 *)(p + 0x98);
                m = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), x, y, w);
                *(s16 *)(d + 0x2A) = m;
                if ((s16)func_8009A8C0(m, c, d, 0x20) <= 0)
                {
                    *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), D_80082E80[0x24], D_80082E80[0x25], w);
                }
                if (func_8009FD7C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), D_80082E80[0x24], D_80082E80[0x25]) != 0)
                {
                    f = 1;
                }
            }
            else
            {
                func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
    }
    else
    {
        j = *(s8 *)(c + 0x26);
        if (j >= 0 && (D_800E2970_s[j].unk_0C & 2) != 0)
        {
            func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
        }
        else if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            e = (s32)func_800A04F0_r(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25), *(s16 *)(d + 0x2A));
            if (e != 0 && (*(s32 *)(e + 0x1C) & 0x2000) != 0 && func_800A0134((u8 *)e, d) < 0x81 &&
                func_8009A540(((s16)(*(u16 *)(d + 0x2A)) >> 9) & 0xFFFF, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25),
                              (s16)(*(u16 *)(d + 0x88) - 0x20)) != 0)
            {
                d[0x71] &= 0x7F;
                return;
            }
            if ((*(s32 *)(d + 0x1C) & 0x20000) != 0)
            {
                t80 = D_80082E80;
                *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t80[0x24], t80[0x25],
                                                   (u16 *)(p + 0x98));
                if (func_8009FD7C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t80[0x24], t80[0x25]) != 0)
                {
                    if (func_800A0134((u8 *)D_800814A8, d) < 0x81 &&
                        func_8009A540(((s16)(*(u16 *)(d + 0x2A)) >> 9) & 0xFFFF, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25),
                                      (s16)(*(u16 *)(d + 0x88) - 0x20)) != 0)
                    {
                        d[0x71] &= 0x7F;
                        return;
                    }
                }
            }
            else
            {
                func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
    }
    i = 0;
    tbl = D_8006CD00;
    do
    {
        e = *(s16 *)(d + 0x2A);
        if ((*(u16 *)(p + 0x98) & 2) != 0)
        {
            s = e - tbl[i];
        }
        else
        {
            s = e + tbl[i];
        }
        if ((s16)func_8009A8C0((s16)s, (u8 *)c, d, 0x20) > 0)
        {
            if (i >= 3 && f != 0)
            {
                d[0x71] &= 0x7F;
                return;
            }
            *(s16 *)(d + 0x2A) = s;
            *(d + (d[0x71] & 0x7F) + 0x74) = *(u8 *)(c + 0x24);
            *(d + (d[0x71] & 0x7F) + 0x7C) = *(u8 *)(c + 0x25);
            d[0x71]++;
            func_8009A3D0(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            kk = (*(u16 *)(d + 0x2A) >> 8) & 0xE;
            /* The table address is a local that the index add writes back
               into, exactly as the landed func_800D92C0 needs it: the `%lo'
               insn's destination then has reg_n_sets != 1. Written inline as
               `*(u8 *)(kk + (s32)D_8006CCD8)' it is 8 differing. The second
               lookup must stay inline -- cse knows D_8006CCE8 is
               D_8006CCD8 + 0x10 and folds its `%hi'/`%lo' away. */
            t1 = (s32)D_8006CCD8;
            t1 += kk;
            *(u8 *)(c + 0x24) += *(u8 *)t1;
            *(u8 *)(c + 0x25) += *(u8 *)(kk + (s32)D_8006CCE8);
            func_8009A21C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            break;
        }
        if (i == 0 && D_80082EA4 != *(u16 *)(c + 0x24))
        {
            if ((s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                /* DEVICE: the return wrapped in a do-while(0). Worth 4
                   differing. A plain `return;' here leaves reorg filling
                   only one of the three branches into the `i++' join with a
                   copy of `addiu $v0, $s1, 0x1', where the target has all
                   three, and puts a `nop' in the third branch's delay slot
                   instead. No uninitialised read, so nothing warns under
                   -Wall. Same construct, same place, as the landed
                   func_800D92C0. */
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
        return;
    }
    *(u16 *)(d + 0x46) &= 0x7FFF;
    p[0x9C] = *(u8 *)(c + 0x26);
    d[0x6D]--;
    g2 = (u8 *)&D_80083460;
    *(u16 *)(g2 + 0x8) += 1;
    if (*(s8 *)(d + 0x6D) == 0)
    {
        d[0x71] &= 0x7F;
        return;
    }
    i = func_800BCB04h((*(u8 *)(c + 0x24) << 6) | 0x20, (*(u8 *)(c + 0x25) << 6) | 0x20, (s16)(*(u16 *)(d + 0x88) - 0x20));
    if (i < 0x200)
    {
        *(s16 *)(d + 0x88) = i;
    }
}

/* Can the record step onto the slot it is facing, and if so close on it:
   func_b04_80172270 (csrc/ovl_dng_b04.c) with its D_80175140 re-dress and
   func_80047784 call removed and d[0x85] set to 4 rather than 0 -- so the
   whole func_800A04F0 / func_800A2CB8 / func_800A0134 / func_800A2B5C
   interrogation, the func_800C7930 push, state 0x11 and the func_8009C93C
   re-aim are that function's verbatim. 99 words, 0 differing, first compile.
   Three of the twin's facts were carried over unmeasured because of that:
   the 8-byte `unused' local that makes the frame 0x40, the shared `-1' exit
   living in the SECOND func_800A2B5C test, and func_800A04F0 coming through
   the unit's `_r' alias with an `s16' fourth parameter.

   THIS UNIT'S FORWARD DECLARATION OF THIS FUNCTION HAD TO BE WIDENED from
   `s32 func_b15_80172920(u8 *p, void *b, void *c, u8 *d)' to the real
   `(u8 *p, UnkVec *b, u8 *c, u8 *d)'. Its only caller, func_b15_801721D8
   below, passes `void *' arguments that convert implicitly, so no call site
   changed; every landed body in this unit was re-verified afterwards and all
   are still 0 differing, func_b15_801721D8's 466 words included. */
s32 func_b15_80172920(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *t;
    s32 unused[2];

    d[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
    {
        goto fail;
    }
    t = func_800A04F0_r(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A));
    if ((s16)func_800A2CB8(d, t) == 0)
    {
        return 0;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
    {
        goto fail;
    }
    if ((*(u16 *)(d + 0x46) & 0x8000) == 0 && (*(u16 *)(g + 0x2) & 0x8) != 0)
    {
        goto fail;
    }
    if ((u16)(0x40 + -func_800A0134(t, d)) > 0x80)
    {
        return 0;
    }
    if ((s16)func_800A2B5C(d) != 0)
    {
        goto fail;
    }
    func_800C7930(d - 0x20, (u8 *)b, 8, 0x300);
    if ((s16)func_800A2B5C(d) != 0)
    {
    fail:
        return -1;
    }
    p[0x9A] = 0x11;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    d[0x84] = 0x7C;
    d[0x85] = 4;
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    return 1;
}

/* State 8..10's second half, the 0.982 relink of the landed func_b01_80172548
   (csrc/ovl_dng_b01.c, 56 words) -- read that comment for the shape: the
   two func_800A2B5C tests on either side of the func_800C7930 push, both
   compared as halfwords, and the actor stores in listing order 0x8C, 0x9A,
   0x9B. Two deltas, both read off this unit's own listing:

   1. THE `*(s16 *)(p + 0x96) = 0' STORE IS ABSENT HERE. b01 emits
      `sh $zero, 0x96($s1)' after the 0x9B store; this listing goes straight
      from `sb $zero, 0x9B($s1)' to the 0x6D reload.
   2. `d[0x85] = 6', not b01's 0. The listing has
      `addiu $v0, $zero, 0x6' / `sb $v0, 0x85($s0)' where b01 has
      `sb $zero, 0x85($s0)'. 6 is this object's constant throughout --
      func_b31f_8016C720 above carries the same 0x4 -> 0x6 re-dress.

   56 words, 0 differing, on the first compile with those two changes.

   MEASURED AND FREE, so it is NOT the reason this lands: the D_80083462
   gate. Written through func_b31f_8016C720's pointer form instead,
   `*(u16 *)((u8 *)&D_80083460 + 0x2) & 0x2000', this body is also 56 words
   and 0 aligned differences -- gcc folds the two to the same `lui %hi' /
   `lhu %lo' pair, so the listing cannot tell them apart and the symbol
   spelling below is a readability choice only.

   MEASURED AND LOAD-BEARING: the two tail stores in the order 0x84 then
   0x85. Swapped they are 56 words and 4 aligned differences.

   No jump table and no rodata of its own -- the listing opens at `glabel'. 
   THIS COPY'S OWN CONSTANT: 0x6 -> 0x4.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b15_80172AAC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    d[0x71] &= 0x7F;
    if ((D_80083462 & 0x2000) != 0)
    {
        return;
    }
    if ((s16)func_800A2B5C(d) != 0)
    {
        return;
    }
    func_800C7930(d - 0x20, (u8 *)b, 8, 0x300);
    if ((s16)func_800A2B5C(d) != 0)
    {
        return;
    }
    *(s32 *)(p + 0x8C) = 0;
    p[0x9A] = 0x11;
    p[0x9B] = 0;
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, NULL);
    d[0x84] = 0x7C;
    d[0x85] = 4;
}

/* THE PARAMETER COPY HAS TO LEAVE THE PROLOGUE'S UNSCHEDULABLE PREFIX. The
   target saves and copies its four parameters in the order $s1, $s2, $s3,
   $s0 -- parameter 0 LAST -- and the leading run of (set pseudo hard_reg)
   insns is pinned by sched.c and emitted in LUID order, so no spelling
   inside those four copies can reorder them. A plain `p = a;` in the body
   is the cure: combine folds parameter 0's prologue copy into the body copy,
   the surviving move is a user insn inside sched's region, and sched2's
   entry block (all priority 1, ranked on INSN_LUID) therefore puts it after
   the other three. Used directly, with no local, the function is 8
   differing; the same copy made of parameter 1 instead is also 8.

   Two other measured facts. `if ((s16)d != 0)` is the halfword test the
   target's `sll $v0, $s3, 16` / `bnez` is: a plain `if (d != 0)` is 80 words
   and 59 differing. The case arms are in the target's own order -- case 0,
   case 2, then case 1 falling into `default` -- and putting case 2 first is
   79 words and 47 differing. `s32 r` with `(s16)` casts at its three uses
   also measures 0; `s16 r` is kept because it needs none of them.
   81 words, 0 differing. */
s32 func_b15_80172B8C(u8 *a, void *b, void *c, s32 d)
{
    u8 *p;
    s16 r;

    p = a;
    r = func_800ADDA0(b, c, p, 3, 6, (s32)(p + 0x9C));
    if (r < 0)
    {
        return 0;
    }
    if ((s16)d != 0)
    {
        func_b15_801721D8(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b15_801721D8(p, b, c, p);
        return 0;
    case 1:
        p[0x71] &= 0x7F;
        if ((s16)func_800A2BDC(p) != 0)
        {
            *(u16 *)(p + 0x46) &= 0x7FFF;
            return 0;
        }
    default:
        p[0x71] &= 0x7F;
        if ((D_80083462 & 8) == 0)
        {
            return 1;
        }
        *(u16 *)(p + 0x46) &= 0x7FFF;
        return 0;
    }
}

/* func_800D9884 (csrc/ovl_dungeon_800D50FC.c) minus its trailing D_80083462
   test: the outer gate's failure falls through to the epilogue and the inner
   gate's failure returns, so both branches land on the same word, and the
   installed handler is the bank-local func_b15_801719DC. 31 words,
   0 differing, first compile. */
void func_b15_80172CD0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b15_801719DC;
        *(s32 *)(p + 0x90) = 0;
    }
}

/* A two-gate handler installer: the outer gate is called with no arguments at
   all -- no instruction sets up $a0..$a3 before it -- so the four incoming
   registers reach it untouched, and the inner gate is handed the third and
   fourth parameters. The second parameter is never mentioned. The inner
   result is read as a halfword: `sll 16` with no `sra` and a `blez` is
   `(s16)x > 0`. Same shape as the landed func_800D9C18 in
   csrc/ovl_dungeon_800D50FC.c. 25 words, 0 differing.

   The handler field is written through a `u8 *` rather than UnkDungeonActor,
   whose typedef lives in csrc/ovl_dungeon_decls.h and is not in this unit's
   context; `*(u32 *)(p + 0x8C) = (u32)f` is the spelling the landed
   func_800D991C tail already uses for the same store. */
void func_b15_80172D4C(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b15_801719DC;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b15", func_b15_80172DB0);

/* The actor's +0x9B six-state despawn sequence: state 0 picks the slot triple
   out of (d + 0x46) & 0x3FFF and hands it to func_800A94A0, state 1 waits on
   func_8003F270 and falls into state 2's +0x96 countdown, which sprays five
   0xC0C0C0 func_b15_80170D28 sparks per frame and, on the odd frames above
   0x13, spawns one 0x212 node driven by func_b15_80171080, state 3 sprays
   twenty func_b15_80170F2C sparks, state 4 waits out D_8008346C and state 5
   does the D_b15_80174634 re-dress, drops D_80083460's live count and
   re-installs func_b15_801719DC. 468 words, 0 differing.

   IT OWNS TWO TABLES. jtbl_b15_80170850 is the 6-entry state switch at rodata
   offset 0x48, all six entries distinct (states 0..5, `default' is the bare
   return). jtbl_b15_80170868 is the 7-entry table at offset 0x60 for state
   0's FIRST switch, cases 1 2 3 5 6 7 over the 0x3FFF field with case 4
   sharing the `default' arm's label; all seven entries are distinct and the
   seventh is a real case, NOT a pad -- the table ends at 0xbe1084, which is
   where splat/dungeon.bin.yaml ends this unit's .rodata, so nothing follows
   it to align against. Both were read back out of a standalone cc1psx run and
   both match the target's entry structure.

   NO TWIN AT dngtwin's FLOOR, and xsibs finds nothing for any of this unit's
   four stubs. This is a BLEND, found by the callee-set grep: `0xC0C0C0' plus
   func_8003FC64 intersects on csrc/ovl_dng_b06.c, whose landed
   func_b06_80172970 (280 words) is this body's state 0 verbatim, its state 1,
   and the heads and tails of states 2, 4 and 5; and the 12-byte blob copy to
   n + 0x48 names csrc/ovl_dng_b08.c, whose landed func_b08_801741D0 is the
   0x212-node block. Neither has a whole-body ratio to this one.

   ALL FIVE OF b06's TRANSFERABLE FACTS HELD, FOUR WITHIN ONE OF ITS FIGURES
   across a frame mismatch (-0x40 and seven saves here against b06's -0x38 and
   six) -- they are all SELECTION facts:

   | spelling                                     | words | aligned | b06 |
   | as written below                             |  468  |    0    |  0  |
   | `s32 k' / `u32 k'                            |  467  |    6    |  7  |
   | `u8 k'                                       |  468  |    1    |  1  |
   | `s8 k'                                       |  468  |    0    |  0  |
   | inner three-case switch written 1, 2, 3      |  470  |    9    | 10  |
   | `D_8008346C = 0' after the 0xA6 decrement    |  469  |    8    |  9  |
   | the D_8006DE24 record indexed inline         |  468  |    1    |  1  |

   THE ONE COMPILE THIS NEEDED: `r = (UnkSlot *)n->unk_0C' MUST COME BEFORE
   the 12-byte blob copy, not after it. After, the body is 469 words and 36
   aligned differences -- the `lw 0xC($s1)' lands below the `swl'/`swr' group
   and takes a `nop' with it, and the three blob registers rename. That is
   also the one place a table owner's price misleads: ovlverify read 181
   differing for those 36 rows, because the extra word shifts every later
   block and ovlverify compares position by position.

   SEVEN MORE FACTS MEASURED, BOTH SPELLINGS PRICED:

   | spelling                                     | words | aligned |
   | UnkDngB15Name12 as `u32 b[0x3]'              |  462  |   18    |
   |   -- alignment 4 gets the aligned block move |       |         |
   |   where this target wants the `lwl'/`swl'    |       |         |
   |   pair group, so the type must stay all-`u8' |       |         |
   | two `if's instead of __builtin_abs on the    |  477  |   25    |
   |   d + 0x72 / d + 0x73 pair                   |       |         |
   | no `np' local -- the 0x19 pair written as    |  469  |   28    |
   |   `(u8 *)n + 0x3A' / `+ 0x3C'                |       |         |
   | no `g2' local for D_80083460 in state 5      |  467  |   18    |
   | no `m' local -- the three vector stores      |  469  |    8    |
   |   written through `n->unk_08' directly       |       |         |
   | the loop counters as `s32 cnt'               |  466  |   13    |
   | the three 0x80 bytes written 2, 3, 4         |  468  |    2    |

   AND THE ARM ORDER OF A JUMP-TABLE SWITCH IS NOT MEASURABLE HERE, which is
   worth knowing before the next agent trusts a zero. Writing state 0's FIRST
   switch ascending (5/1, 6/2, 7/3 instead of 7/3, 6/2, 5/1) measures 468
   words / 0 aligned differences and 0 under ovlverify -- but the standalone
   compile shows it emits the three `k = 1' stubs in the other order, so
   entries 4, 5 and 6 of jtbl_b15_80170868 come out reversed and the table
   that ships is WRONG. Both tools mask the R_MIPS_26 field of a `j' and
   neither looks at .rodata at all, so the descending order below is settled
   off the listing's own table and off b06's, not off a measurement. The
   compare-chain switch in the `else' has no table and does price its order,
   at 9.

   TWO PLAUSIBLE FACTS MEASURED AND FREE, so they are NOT why this lands: a
   local holding c + 0x14 across state 2's three tests (468 / 0 when all three
   uses are replaced together -- though replacing only ONE of the three is 4,
   which is how a half-applied edit reads as a fact), and `r->unk_14 |= 0xC'
   written before the +0xC clear rather than after (468 / 0; sched1 hoists the
   `lhu 0x14' to the top of the group either way). */
void func_b15_801732C4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk8006DE24 *tbl;
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *g;
    u8 *e;
    u8 *q;
    u8 *g2;
    u8 *dr;
    u8 *np;
    s16 k;
    s32 i;
    s32 x;
    s32 y;
    u16 cnt;

    k = 0;
    switch (p[0x9B])
    {
    case 0:
        if ((*(u32 *)(d + 0x1C) & 0x2000) != 0)
        {
            switch (*(u16 *)(d + 0x46) & 0x3FFF)
            {
            case 7:
                k = 1;
            case 3:
                g = d + 0xE;
                break;
            case 6:
                k = 1;
            case 2:
                g = d + 0xB;
                break;
            case 5:
                k = 1;
            case 1:
                g = d + 0x8;
                break;
            default:
                g = NULL;
                break;
            }
        }
        else
        {
            switch (*(u16 *)(d + 0x46) & 0x3FFF)
            {
            case 3:
                g = d + 0xE;
                break;
            case 2:
                g = d + 0xB;
                break;
            case 1:
                g = d + 0x8;
                break;
            default:
                g = NULL;
                break;
            }
        }
        if (g[0] != 0)
        {
            *(u16 *)(p + 0x98) &= 0xFF7F;
            if (k != 0)
            {
                e = (u8 *)D_800814A8;
                *(s32 *)(d + 0x60) = (s32)e;
                q = *(u8 **)(e - 0x14);
                d[0x72] = q[0x24];
                d[0x73] = q[0x25];
            }
            else
            {
                tbl = D_8006DE24;
                i = g[0];
                e = (u8 *)&tbl[i];
                if (e[0x12] == 2)
                {
                    e = *(u8 **)(d + 0x60);
                    if (e != NULL)
                    {
                        q = *(u8 **)(e - 0x14);
                        d[0x72] = q[0x24];
                        d[0x73] = q[0x25];
                    }
                }
                else
                {
                    *(s32 *)(d + 0x60) = func_800A05A4(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A), 0x10);
                    x = *(s8 *)(d + 0x72);
                    y = *(s8 *)(d + 0x73);
                    d[0x72] = __builtin_abs(x);
                    d[0x73] = __builtin_abs(y);
                }
            }
            if (func_800A94A0(d, g, k, p + 0x98) != 0)
            {
                p[0x9B]++;
            }
        }
        else
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            D_8008346C = 0;
            *(u16 *)(D_800814A8 + 0xA6) -= 1;
            func_800A4ACC(d);
            d[0x6D] -= 1;
            *(s32 *)(p + 0x8C) = (s32)func_b15_801719DC;
            d[0x73] = 0;
            d[0x72] = 0;
            *(u16 *)(d + 0x46) &= 0x7FFF;
        }
        break;
    case 1:
        if (func_8003F270() != 0)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            break;
        }
        *(u16 *)(c + 0x14) &= 0xF7FF;
        p[0x9B]++;
        func_800A56E0(0x703);
    case 2:
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x96) = 0;
            p[0x9B]++;
            break;
        }
        if (*(s8 *)(c + 0x4) == 2 && (*(u16 *)(c + 0x14) & 0x1000) != 0)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            *(s16 *)(p + 0x96) = 0x16;
        }
        if (*(s8 *)(c + 0x4) < 2)
        {
            break;
        }
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
        {
            *(s16 *)(p + 0x96) = 0;
            p[0x9B]++;
            *(u16 *)(c + 0x14) &= 0xF7FF;
        }
        if (*(s16 *)(p + 0x96) >= 7)
        {
            for (cnt = 0; cnt < 5; cnt++)
            {
                func_b15_80170D28((UnkNode *)(p - 0x20), 0, 0xC0C0C0, (rand() & 0xFF) | 0x80,
                                  (rand() & 0x7F) - 0x40, (rand() & 0x7F) - 0x40,
                                  (rand() & 0x7F) - 0x40);
            }
        }
        if ((*(u16 *)(p + 0x96) & 1) == 0)
        {
            break;
        }
        if (*(s16 *)(p + 0x96) < 0x13)
        {
            break;
        }
        n = func_8003FC64(0x212);
        if (n == NULL)
        {
            break;
        }
        np = (u8 *)n + 0x20;
        *(s16 *)(np + 0x1A) = 0x19;
        *(s16 *)(np + 0x1C) = 0x19;
        n->unk_10 = (u32)func_b15_80171080;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)b + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)b + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)b + 0xA) - 0x60;
        r = (UnkSlot *)n->unk_0C;
        *(UnkDngB15Name12 *)((u8 *)n + 0x48) = *(UnkDngB15Name12 *)D_80174694;
        r->unk_08 = (u32)((u8 *)n + 0x48);
        r->unk_10 = 0x40;
        r->unk_1E = 0x32C8;
        r->unk_1C = 0x32C8;
        r->unk_06 = 0x64;
        *(s32 *)((u8 *)r + 0xC) = 0;
        r->unk_14 |= 0xC;
        np[0x4] = 0x80;
        np[0x3] = 0x80;
        np[0x2] = 0x80;
        break;
    case 3:
        if ((*(s8 *)(c + 0x4) == 5 && (*(u16 *)(c + 0x14) & 0x1000) != 0) ||
            (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            *(s16 *)(p + 0x96) = 0x14;
            *(u16 *)(p + 0x98) |= 0x80;
            p[0x9B]++;
            for (cnt = 0; cnt < 0x14; cnt++)
            {
                func_b15_80170F2C((UnkNode *)(p - 0x20), 0, 0xC0C0C0, (rand() & 0xFF) | 0x80, 0, 0,
                                  0);
            }
        }
        break;
    case 4:
        if (D_8008346C == 0)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        if ((s16)(--*(u16 *)(p + 0x96)) > 0 && (*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        *(s16 *)(p + 0x96) = 0;
        *(u16 *)(c + 0x14) &= 0xF7FF;
        p[0x9B]++;
        break;
    case 5:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        dr = D_b15_80174634;
        if (*(u8 **)(c + 0x2C) != dr)
        {
            *(u8 **)(c + 0x2C) = dr;
            func_80047784(c, dr[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 2);
            c[0x5] = 1;
            *(s32 *)(p + 0xA8) = *(s32 *)(p + 0xA4);
            *(s16 *)(p + 0xA6) = 0;
            *(s16 *)(p + 0xB8) = 0;
            *(s32 *)(p + 0xAC) = 0;
            *(s32 *)(p + 0xB0) = 0;
        }
        g2 = (u8 *)&D_80083460;
        if (*(s32 *)(g2 + 0xC) != 0)
        {
            break;
        }
        *(u16 *)(g2 + 0xA) -= 1;
        *(s32 *)(p + 0x8C) = (s32)func_b15_801719DC;
        func_800A4ACC(d);
        d[0x73] = 0;
        d[0x72] = 0;
        d[0x6D] -= 1;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A56E0(0xB4);
        break;
    }
}

/* Four-state walk-and-stop, the banked twin of the resident
   func_800D4634 (csrc/ovl_dungeon_800CABA8.c) -- xsibs put that family at
   0.747 and it is the whole shape; dngtwin found no landed carved sibling
   (its two hits, func_b34_80173D34 and func_b20_801730E0, are both still
   INCLUDE_ASM). 131 words, 0 differing.

   Five deltas from the resident, all read off the listing: state 0 and
   state 1 ZERO b->unk_0C / b->unk_10 where the resident seeds them from
   D_8006CCD8 / D_8006CCE8 and then decays them by a quarter; state 0's
   d[0x28] arm picks the timer off the actor flags -- 8 when
   d + 0x1C has any of 0x228 and -1 otherwise -- where the resident always
   writes 8; state 1 decrements the timer ONLY while it is positive and
   otherwise clears it on a 0x6000 facing, then returns when it is nonzero,
   where the resident decrements unconditionally; the stop block hands
   func_800AAA54 the bank-local D_8017466C rather than NULL; and the tail has
   no `a->unk_9A = 0xE' (so none of the resident's `do { } while (0)' either).
   The timer is 2 here against the resident's 8 at the state-1 re-arm.

   THE ONE LOAD-BEARING SPELLING IS THAT 8-OR-(-1) PICK, AND IT IS A
   TERNARY, NOT AN `if'/`else'. The target puts -1 in the branch's delay slot
   and 8 after it, with the value in its own register ($v1) while the flag
   word stays in $v0: `((... & 0x228) != 0) ? 8 : -1' is 131 words /
   0 differing, the same ternary written `== 0 ? -1 : 8' is 3 aligned
   differences, and BOTH `if'/`else' orders -- `!= 0' with 8 first and
   `== 0' with -1 first -- are 5, because gcc canonicalises the gate and
   emits `bnez' with the 8 in the delay slot whichever way round it is
   written. An explicit `s32 t;' assigned in the two arms and stored once
   also measures 0, so what the target needs is the separate pseudo, not the
   ternary as such.

   Measured and FREE at 131 / 0, so it is not the reason this lands: state
   2's `if ((*(s16 *)(p + 0x96) -= 1) > 0)' split into a decrement statement
   and a plain test; cse keeps the `sll'/`bgtz' pair either way. */
void func_b15_80173A14(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;

    switch (p[0x9B])
    {
    case 0:
        func_800AD4D0(d);
        b->unk_10 = 0;
        b->unk_0C = 0;
        p[0x9B]++;
        if (d[0x28] != 0)
        {
            if (*(u16 *)(c + 0x14) & 0x8000)
            {
                *(s16 *)(p + 0x96) = 0;
                p[0x9B] = 2;
                return;
            }
            *(s16 *)(p + 0x96) = ((*(s32 *)(d + 0x1C) & 0x228) != 0) ? 8 : -1;
        }
        else
        {
            goto stop;
        }
        /* fallthrough */
    case 1:
        b->unk_10 = 0;
        b->unk_0C = 0;
        if (*(s16 *)(p + 0x96) > 0)
        {
            *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        }
        else if (*(u16 *)(c + 0x14) & 0x6000)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        if (*(s16 *)(p + 0x96) != 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
        stop:
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(p, b, c, D_8017466C);
        }
        else
        {
            *(s16 *)(p + 0x96) = 2;
            p[0x9B]++;
        }
        return;
    case 2:
        if (*(s16 *)(p + 0x96) != 0)
        {
            b->unk_10 = 0;
            b->unk_0C = 0;
        }
        if ((*(s16 *)(p + 0x96) -= 1) > 0)
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
        *(u32 *)(p + 0x8C) = (u32)func_b15_801719DC;
        return;
    }
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b15_80173C20(void)
{
    func_800AD058();
}

/* A reset: hand the four incoming registers to func_800AB538 untouched --
   no instruction sets up $a0..$a3 before the `jal', which is what the
   empty-paren declaration buys -- then clear p + 0xA6/+0xAC/+0xB0 and copy
   p + 0xA4 forward to p + 0xA8. The same four fields func_b15_80173CD4's
   and func_b15_8017409C's +0x80000 arms touch, minus the +0x92 subtraction
   and the +0xB8 clear. 15 words, 0 differing.

   THE +0xA4 -> +0xA8 COPY IS THE FIRST STATEMENT, although its `sw' is the
   LAST word before the epilogue -- the target hoists the `lw' above the
   `sh' and leaves the `sw' where the three clears put it. All four positions
   measured at 15 words: first is 0 differing and each of the other three is
   2, the `lw'/`sh' pair transposed. That is the same rule the two +0x80000
   arms in this unit obey, where the copy sits second because the +0x92
   subtraction reads first; here nothing reads before it. */
void func_b15_80173C40(u8 *p)
{
    func_800AB538();
    *(s32 *)(p + 0xA8) = *(s32 *)(p + 0xA4);
    *(s16 *)(p + 0xA6) = 0;
    *(s32 *)(p + 0xAC) = 0;
    *(s32 *)(p + 0xB0) = 0;
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b15_80173C7C(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b15_80173C9C(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b15_801719DC;
    }
}

/* The three-state dress/refuse/undress chain on p + 0x9B. 242 words,
   0 differing.

   THE TWIN IS THE LANDED func_b42_80173A90 (csrc/ovl_dng_b42.c), 228 words,
   BYTE-IDENTICAL FRAME (addiu $sp, -0x28; $s0..$s4 + $ra in the same order),
   and this body is it statement for statement with four local changes: the
   two bank-local facing tables are D_80174684 (state 0) and D_8017468C
   (state 1's re-dress), the handler is func_b15_801719DC, the installer is
   this unit's own func_b15_801743F0, and the +0x80000 arm gains SIX
   STATEMENTS the twin has not got -- the +0x92/+0xA6 subtraction, four zero
   stores and the +0xA4 -> +0xA8 copy -- plus one more,
   `*(s32 *)(d + 0x1C) |= 0x40000;', after the re-dress's func_80047784.

   THE ORDER OF THOSE SIX IS LOAD-BEARING AND IT IS NOT THE ORDER THE STORES
   COME OUT IN. The +0xA8 copy is the SECOND statement, right after the
   subtraction; written LAST -- which is where its `sw' lands, in the jal's
   delay slot -- it is 242 words / 9 aligned differences, every one of them
   in this block: the target loads +0x92, +0xA6, +0xA4 in that order with the
   `move a2' ahead of them, and with the copy last the two `lhu's swap and
   the `lw'/`move a3' pair sinks past the first zero store.

   DEVICE KEPT, AND IT IS THE TWIN'S: state 0's bump is wrapped in a
   `do { } while (0)'. Without it this body is 242 words / 18 aligned
   differences and every one is `p' and `b' holding each other's
   callee-saved register -- the same residue and the same count the twin
   measured, so the SIX extra `p' references in the +0x80000 arm do NOT lift
   `p' past `b' in allocno_compare on their own. Probably macro residue; the
   twin's comment has the flow.c mechanism.

   THE PER-ARM BUMP HOLDS TOO: as a shared `goto bump' it is 240 words /
   32 aligned differences here (the twin measured 226 / 29 on its own body).

   Measured and FREE at 242 / 0, so it is not the reason this lands:
   `*(u16 *)(p + 0x92) -= *(u16 *)(p + 0xA6);' against the long-hand
   `= *(u16 *)(p + 0x92) - ...'. The four separate `&D_80083460' pointer
   locals are the twin's and were not re-measured. */
void func_b15_80173CD4(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_80174684;
        func_80047784(c, D_80174684[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        do
        {
            p[0x9B] += 1;
        } while (0);
        return;
    case 1:
        if ((s16)func_80042900((s8 *)d, 1) != 0)
        {
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
                func_800AA79C(p, b, c, d);
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
                *(u16 *)(p + 0x92) -= *(u16 *)(p + 0xA6);
                *(s32 *)(p + 0xA8) = *(s32 *)(p + 0xA4);
                *(s16 *)(p + 0xA6) = 0;
                *(s16 *)(p + 0xB8) = 0;
                *(s32 *)(p + 0xAC) = 0;
                *(s32 *)(p + 0xB0) = 0;
                func_b15_801743F0(p, (u8 *)b, c, d);
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
        }
        *(u8 **)(c + 0x2C) = D_8017468C;
        func_80047784(c, D_8017468C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b15_801719DC;
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
        *(u32 *)(p + 0x8C) = (u32)func_b15_801719DC;
        return;
    }
}

/* The same three-state tail as func_b15_80173CD4 above, but with state 1's
   d[0x25] test opening the arm as well as closing it, so the dress / flag /
   bump tail is written out twice. 213 words, 0 differing, ON THE FIRST
   COMPILE.

   THE TWIN IS THE LANDED func_b29_801741CC (csrc/ovl_dng_b29.c), 213 words,
   BYTE-IDENTICAL FRAME and the SAME WORD COUNT. Two deltas, both read off
   the listing: state 2 here does NOT clear d + 0x1C (the twin's
   `*(s32 *)(d + 0x1C) &= ~8;' is absent, which is what frees $v1 for the
   &D_80083460 pointer and swaps the tail's $v0/$v1 against the twin's), and
   the +0x80000 arm is this unit's SIX-statement block rather than the twin's
   three -- the same block func_b15_80173CD4 uses, with the same offsets.

   THE +0xA8 COPY IS THE SECOND STATEMENT OF THAT BLOCK, and that fact
   reproduces func_b15_80173CD4's price to the digit: written last it is
   213 words / 9 aligned differences, all of them the two `lhu's swapping and
   the `lw'/`move a3' pair sinking past the first zero store. Two bodies, two
   frames, the same 9.

   MEASURED AND FREE at 213 / 0, so it is NOT the reason this lands, and it
   INVERTS the twin's own claim: the `p[0x9B] += 1' bump as a `goto bump'
   label inside the switch against the bump written out per arm. The twin
   cites 177 differing for the per-arm form on func_b05_80173BEC; here the
   two forms are byte-identical, because with three bump sites and no
   intervening stores jump2 cross-joins the copies back into one. The
   `goto' is kept only because it is the twin's spelling. */
void func_b15_8017409C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *j;
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
        *(u8 **)(c + 0x2C) = D_80174684;
        func_80047784(c, D_80174684[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        goto bump;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_8017468C;
            func_80047784(c, D_8017468C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(s32 *)(d + 0x1C) |= 0x40000;
            j = (u8 *)&D_80083460;
            *(u16 *)(j + 0xA) += 1;
            goto bump;
        }
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
            *(u16 *)(p + 0x92) -= *(u16 *)(p + 0xA6);
            *(s32 *)(p + 0xA8) = *(s32 *)(p + 0xA4);
            *(s16 *)(p + 0xA6) = 0;
            *(s16 *)(p + 0xB8) = 0;
            *(s32 *)(p + 0xAC) = 0;
            *(s32 *)(p + 0xB0) = 0;
            func_b15_801743F0(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_8017468C;
        func_80047784(c, D_8017468C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(u16 *)(g + 0xA) += 1;
    bump:
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        k = (u8 *)&D_80083460;
        *(u16 *)(k + 0xA) -= 1;
        *(u32 *)(p + 0x8C) = (u32)func_b15_801719DC;
        return;
    }
}

/* The four-argument installer's banked twin: func_800DA660 in the resident
   overlay is its first half verbatim, and this one adds the gate's `else`.
   50 words, 0 differing.
   THE `(s16)` CAST ON func_800AD9B4 IS THE ONE LOAD-BEARING SPELLING: the
   target tests the result as a halfword, `sll $v0, 16` then `blez` with no
   `sra`. Written `func_800AD9B4(c, d) > 0` it is 49 words / 38 differing.
   Measured and equal at 50 words / 0 differing, so none of these is the
   reason it lands: passing the four arguments explicitly to func_800AC82C
   (they are already in $a0..$a3, and the declaration is empty-paren, so the
   call emits the bare `jal` either way); hoisting D_b15_80174634 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b15_801743F0(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b15_801719DC;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_80174684 && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_b15_80174634;
        func_80047784(c, D_b15_80174634[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    }
}

/* A two-gate handler installer: the outer gate is called with no arguments at
   all -- no instruction sets up $a0..$a3 before it -- so the four incoming
   registers reach it untouched, and the inner gate is handed the third and
   fourth parameters. The second parameter is never mentioned. The inner
   result is read as a halfword: `sll 16` with no `sra` and a `blez` is
   `(s16)x > 0`. Same shape as the landed func_800D9C18 in
   csrc/ovl_dungeon_800D50FC.c. 25 words, 0 differing.

   The handler field is written through a `u8 *` rather than UnkDungeonActor,
   whose typedef lives in csrc/ovl_dungeon_decls.h and is not in this unit's
   context; `*(u32 *)(p + 0x8C) = (u32)f` is the spelling the landed
   func_800D991C tail already uses for the same store. */
void func_b15_801744B8(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b15_801719DC;
        }
    }
}

/* 14 words, 0 differing. */
void func_b15_8017451C(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b15_801719DC;
    }
}

/* 14 words, 0 differing. */
void func_b15_80174554(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b15_801719DC;
    }
}

/* 14 words, 0 differing. */
void func_b15_8017458C(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b15_801719DC;
    }
}

/* 14 words, 0 differing. */
void func_b15_801745C4(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b15_801719DC;
    }
}

/* 14 words, 0 differing. */
void func_b15_801745FC(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b15_801719DC;
    }
}
