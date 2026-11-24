#include "common.h"

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b15", func_b15_80171080);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b15", func_b15_801711D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b15", func_b15_801713A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b15", func_b15_801719DC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b15", func_b15_80172AAC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b15", func_b15_801732C4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b15", func_b15_80173A14);

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b15_80173C20(void)
{
    func_800AD058();
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b15", func_b15_80173C40);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b15", func_b15_80173CD4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b15", func_b15_8017409C);

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
