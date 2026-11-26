#include "common.h"

typedef struct UnkDng264D4Vec
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 pad_06[0x2];
} UnkDng264D4Vec;
typedef struct UnkDng264D4SP
{
    u8 pad_00[0x30];
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    u8 pad_3C[0x28];
    s32 unk_64;
    s32 unk_68;
    s32 unk_6C;
    SVECTOR unk_70[4];
    u8 pad_90[0x8];
    UnkDng264D4Vec unk_98[4];
} UnkDng264D4SP;
s32 func_80065F90(s32, s32);
s32 func_80064710(s32);

/* Turn a delta between two points into a two-angle bearing at out[0..1]
   with out[2] zeroed: out[0] is minus func_80065F90 (ratan2) of the y and z
   deltas, out[1] is func_80065F90 of the x delta against func_80064710's
   (square root) hypotenuse of the other two, and each divisor is guarded to
   1 when it comes out zero. 53 words, 0 differing.

   No twin to read: dngtwin's only hit is func_s53_8002401C at 1.000, which
   is this function's CLONE and still an INCLUDE_ASM. Written from the
   listing; func_80065F90's and func_80064710's prototypes are
   csrc/ovl_dungeon_8009F644.c's and csrc/ovl_dungeon_800D50FC.c's.

   THE ONE FACT, and it is a storage fact rather than a shape one: the three
   deltas live in a `s32 d[3]' STACK ARRAY, not in three locals. The target
   saves only $s0 and $ra and spills the deltas to sp+0x10/0x14/0x18, which
   is an array; as three `s32' locals they land in $s1/$s2/$s3, the frame
   grows to 0x38 and it is 51 words / 49 aligned differences. With the array
   in place the PARAMETER STYLE IS FREE: prototyped `s16' parameters and a
   K&R definition with `s16' declarators both measure 53 words / 0 differing.
   The lw-plus-sll/sra pairs on the two stack arguments are not evidence of
   the K&R form -- declared `s32' with `(s16)' casts instead, combine
   narrows them to `lh' and the body is 49 words / 17. */
UnkNode *func_8003FD64(s32, void *);
extern u8 D_80083498[];
void func_s53_800240F0(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_8004491C(UnkNode *, u32);
extern u8 D_80045340[];
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
extern u32 D_800814A8;
extern u8 D_80082E80[];
extern s16 D_80083228;
extern u8 *D_800E3D18;
s32 func_800644B8(s32);
extern u32 D_800814A0;
UnkNode *func_8003FC64(s32);
void func_s53_80024918(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_s53_80024700(u8 *p);
void func_s53_8002455C(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u8 D_800DE938[];
void func_8003DB94(void *, void *, s32);
s32 func_80064584(s32);
void func_800478B8(u8 *);

void func_s53_8002401C(s16 x0, s16 y0, s16 z0, s16 x1, s16 y1, s16 z1, s16 *out)
{
    s32 d[3];
    s32 h;

    d[0] = x1 - x0;
    d[1] = y1 - y0;
    d[2] = z1 - z0;
    out[0] = -func_80065F90(d[1], (d[2] != 0) ? d[2] : 1);
    h = func_80064710(d[1] * d[1] + d[2] * d[2]);
    out[1] = func_80065F90(d[0], (h != 0) ? h : 1);
    out[2] = 0;
}

/* THE ACTOR DRIVER: four states on `*(s16 *)(p + 0xC)'. State 0 dresses the
   node through D_80045340 and falls through into state 1, which tracks the
   caster's slot projection into the mover (plus func_8003DE58's three deltas,
   or z - 0x20 when the slot's +0x14 0x8000 flag is up) and arms the 0xA-frame
   countdown at +0x10 once the room record's 0x80 bit admits it. State 2 grows
   the renderable 0x200 a frame, takes the +0x4 node's mover plus the camera
   height as the destination offset by the D_800E3D18 facing entry for the
   camera octant, eases the mover toward it over the countdown with a `* 170'
   lift on z, and on the last frame snaps to the destination, re-arms the
   countdown to 2 and advances. State 3 keeps growing and brightening until
   that countdown runs out, then retires the actor.
   233 words, 0 differing, FIRST COMPILE.
   OWNS NO TABLE -- four arms is under cc1psx's five-case threshold, so the
   dispatch is a balanced compare tree on {0,1,2,3}.

   TWIN: the landed func_s60_80024050 in csrc/ovl_dng_s60.c (dngtwin 0.865,
   220 words), the same driver one bank over. Everything structural came from
   it: the `||' gate, the `s16 out[4]' stack array, the `u16 z' local, the
   `(dest - b->unk_0N) / *(s16 *)(p + 0x10)' easing and the `>> 9 & 7' index
   into D_800E3D18 (cc1psx folds it to `sra 7' / `andi 0x1C'). This copy's
   countdown is at +0x10 and its frame counter at +0x12 where the twin uses
   +0xE and +0x10, it gains a fourth state, and its state-2 teardown re-arms
   instead of retiring.

   THE ONE MEASURED FACT: state 2's `*(s16 *)(p + 0x10) = 2;' is the LITERAL
   2, and that is why it emits `sh $s1' -- cse's record_jump_equiv knows
   $s1 == 2 inside the `case 2' arm, so the literal costs no word. Lifting
   the switch value into an `s32' local and storing THAT instead is 234 words
   / 7 aligned differences. */
void func_s53_800240F0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkSlot *s;
    u8 *v;
    s16 out[4];
    u16 z;
    s32 u;
    s32 i;

    switch (*(s16 *)(p + 0xC))
    {
    case 0:
        func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
        *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) + 1;
        /* fallthrough */
    case 1:
        s = (UnkSlot *)(*(UnkNode **)p)->unk_0C;
        if ((s->unk_14 & 0x8000) != 0 || func_8003DE58((u8 *)s->unk_08, s, out, 0) != 0)
        {
            v = (u8 *)(*(UnkNode **)p)->unk_08;
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)(v + 0x2);
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)(v + 0x6);
            z = *(u16 *)(v + 0xA);
            *(s16 *)((u8 *)b + 0xA) = z;
            if ((s->unk_14 & 0x8000) == 0)
            {
                *(s16 *)((u8 *)b + 0x2) += out[0];
                *(s16 *)((u8 *)b + 0x6) += out[1];
                *(s16 *)((u8 *)b + 0xA) += out[2];
            }
            else
            {
                *(s16 *)((u8 *)b + 0xA) = z - 0x20;
            }
        }
        if ((**(u16 **)(p + 0x8) & 0x80) != 0)
        {
            *(s16 *)(p + 0x10) = 0xA;
            *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) + 1;
        }
        break;
    case 2:
        *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + 0x200;
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);
        v = (u8 *)(*(UnkNode **)(p + 0x4))->unk_08;
        b->unk_0C = *(s32 *)(v + 0x0);
        b->unk_10 = *(s32 *)(v + 0x4);
        b->unk_14 = (*(s16 *)(D_800814A8 + 0x88) - 0x50) << 16;
        i = ((D_80083228 + *(s16 *)(D_800814A8 + 0x2A) + 0x100) >> 9) & 7;
        if (func_8003DE58(((u8 **)D_800E3D18)[i], (UnkSlot *)D_80082E80, out, 0) != 0)
        {
            b->unk_0C += out[0] << 16;
            b->unk_10 += out[1] << 16;
            b->unk_14 += out[2] << 16;
        }
        b->unk_00 += (b->unk_0C - b->unk_00) / *(s16 *)(p + 0x10);
        b->unk_04 += (b->unk_10 - b->unk_04) / *(s16 *)(p + 0x10);
        b->unk_08 += (b->unk_14 - b->unk_08) / *(s16 *)(p + 0x10) -
                     (func_800644B8(*(s16 *)(p + 0x10) * 170) << 7);
        (*(s16 *)(p + 0x10))--;
        if (*(s16 *)(p + 0x10) <= 0)
        {
            b->unk_00 = b->unk_0C;
            b->unk_04 = b->unk_10;
            b->unk_08 = b->unk_14;
            *(s16 *)(p + 0x10) = 2;
            *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) + 1;
        }
        break;
    case 3:
        *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + 0x200;
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);
        u = c[0xE] + 8;
        c[0xE] = u;
        c[0xD] = u;
        c[0xC] = u;
        (*(s16 *)(p + 0x10))--;
        if (*(s16 *)(p + 0x10) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        break;
    }
    *(s16 *)(p + 0x12) = *(u16 *)(p + 0x12) + 1;
}

/* Spawn the 0x12 node that drives this bank's bolt: install
   func_s53_800240F0 as the per-frame driver, zero the payload's +0xC/+0xE,
   park the caller's record at the node's +0x20 with the dresser D_80083498
   at the payload's +0x4 and the caller's second record at +0x8, then white
   the renderable 0x80 flat, scale it 0x1000 square, give it the 0x7E00
   sprite, hand it the caller's sprite data at +0x8 and raise the +0x10 0x20
   and +0x14 0x10C bits.  50 words, 0 differing, FIRST COMPILE.

   Written off the landed func_s60_800243C0 in csrc/ovl_dng_s60.c (dngtwin
   0.818, 60 words), the same 0x12 spawn: the payload pointer `t' is what
   gives the listing's `addiu $v0, $v1, 0x20', and the first record store
   stays spelled off the node because combine folds it back onto
   `0x20($v1)' by itself.

   MEASURED AND FREE at 50 words / 0 differing, so NEITHER is why this lands:
     the `*(u32 *)(m + 0x8) = r' sprite store moved to the END of the
       dress block instead of its start                      50 w, 0  free
     the first record store spelled `*(u8 **)t = p' off the payload
       pointer instead of `*(u8 **)((u8 *)n + 0x20)'         50 w, 0  free */
void func_s53_80024494(u8 *p, u8 *q, u32 r)
{
    UnkNode *n;
    u8 *t;
    u8 *m;

    n = func_8003FD64(0x12, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s53_800240F0;
        t = n->payload;
        *(s16 *)(t + 0xC) = 0;
        *(s16 *)(t + 0xE) = 0;
        *(u8 **)((u8 *)n + 0x20) = p;
        *(u8 **)(t + 0x4) = D_80083498;
        *(u8 **)(t + 0x8) = q;
        m = (u8 *)n->unk_0C;
        *(u32 *)(m + 0x8) = r;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        *(s16 *)(m + 0x1E) = 0x1000;
        *(s16 *)(m + 0x1C) = 0x1000;
        *(s16 *)(m + 0x12) = 0x7E00;
        *(u16 *)(m + 0x10) |= 0x20;
        *(u16 *)(m + 0x14) |= 0x10C;
    }
}

/* The step of the node func_s53_80024700 spawns: bump the owning
   renderable's +0x1A spin, advance the +0x2E phase by 0x100, re-seat the
   mover at the parked +0xC..+0x14 point, then offset it by ApplyMatrixSV of
   the (rcos, rsin, 0) radius vector through the payload's own MATRIX at
   +0x4; clear the one-shot flag at +0x30, fade the renderable 8 a frame and
   shrink it 0x100 a frame, and retire the actor when the fade reaches zero
   or func_800478B8 leaves the +0x14 0x6000 bits up.  105 words, 0 differing.

   THE ONE MEASURED FACT: the fade countdown goes through a `u8' local --
   `u = c[0xE] - 8; c[0xE] = u; c[0xD] = u; c[0xC] = u; if (u != 0)' -- which
   is what makes the gate `andi $v0, $v0, 0xFF' on the stored value instead
   of a fresh `lbu'. Re-reading `c[0xE]' in the gate is 105 words / 6 aligned
   differences. Also measured and equally good at 105 words / 0 differing, so
   the target does not constrain the choice: an `s32 u' with
   `if ((u8)u != 0)', and an `s32 u' with `if ((u & 0xFF) != 0)'. */
void func_s53_8002455C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *v;
    SVECTOR a;
    SVECTOR e;
    u8 u;

    v = *(u8 **)p;
    *(s16 *)(v + 0x1A) = *(u16 *)(v + 0x1A) + 1;
    *(s16 *)(p + 0x2E) = *(u16 *)(p + 0x2E) + 0x100;
    b->unk_00 = b->unk_0C;
    b->unk_04 = b->unk_10;
    b->unk_08 = b->unk_14;
    a.vx = (func_800644B8(*(s16 *)(p + 0x2E)) >> 4) * *(s16 *)(p + 0x2C) >> 8;
    a.vy = (func_80064584(*(s16 *)(p + 0x2E)) >> 4) * *(s16 *)(p + 0x2C) >> 8;
    a.vz = 0;
    PushMatrix();
    ApplyMatrixSV((MATRIX *)(p + 0x4), &a, &e);
    PopMatrix();
    *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + e.vx;
    *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + e.vy;
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + e.vz;
    if (p[0x30] != 0)
    {
        p[0x30] = 0;
    }
    u = c[0xE] - 8;
    c[0xE] = u;
    c[0xD] = u;
    c[0xC] = u;
    if (u != 0)
    {
        *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - 0x100;
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);
        *(s16 *)(c + 0x1A) = *(u16 *)(p + 0x2E);
        func_800478B8(c);
        if ((*(u16 *)(c + 0x14) & 0x6000) == 0)
        {
            return;
        }
    }
    *(u16 *)(p - 0x2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

/* Spawn the 0x212 node that runs func_s53_8002455C: hand the child the
   parent's renderable at the payload's +0x0 and a copy of the parent's
   MATRIX at +0x4, seed its +0x2C radius from the parent's +0x78 and its
   +0x2E phase from the parent's +0x76, raise the one-shot flag at +0x30,
   grey the renderable 0x40 flat, dress it from D_800DE938, scale it 0x2000
   square, take its +0x16/+0x18 from the payload's (still zero) +0x24/+0x26
   nibbles, raise the +0x14 0xC and +0x10 0x20 bits and spin it by the phase,
   then place the child's mover at the parent's position offset by
   ApplyMatrixSV of the (rcos, rsin, 0) radius vector through that MATRIX.
   134 words, 0 differing.

   TWO MEASURED ORDER FACTS, both found by exhaustive statement-order sweeps:
   written in the obvious order the body is 134 words / 33 aligned
   differences, and nothing but order moved it.

     the three payload stores in the order +0x2C, +0x2E, +0x30
       -- that is, the `t[0x30] = 1' LAST                   134 w,  0  kept
          ... +0x30 in the middle                           134 w, 16
          ... +0x30 first                                   134 w, 34
     the five dress statements in the order +0x16, +0x18, +0x1A,
       then `|= 0x20' on +0x10, then `|= 0xC' on +0x14      134 w,  0  kept
          ... +0x18 before +0x16                            134 w,  4
          ... the two `|=' in the other order               134 w,  8
          ... +0x1A last, which is the natural reading of the
              listing, where its store sits in the jal's
              delay slot                                    134 w, 17
   NOT MEASURABLE: where `t = n->payload' is written -- first, between the
   record store and the MATRIX copy, or after it; all three are 134 / 0.

   The +0x16/+0x18 reads are `(s16) * (u16 *)(t + 0x24) >> 12': the u16 load
   plus the s16 cast is what gives `lhu' / `sll 16' / `sra 28'. Spelled
   `*(s16 *)(t + 0x24) >> 12' it is `lh' / `sra 12' and one word short. */
void func_s53_80024700(u8 *p)
{
    UnkNode *n;
    u8 *t;
    u8 *m;
    u8 *v;
    SVECTOR a;
    SVECTOR b;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s53_8002455C;
        t = n->payload;
        *(u8 **)((u8 *)n + 0x20) = *(u8 **)p;
        *(MATRIX *)((u8 *)n + 0x24) = *(MATRIX *)(p + 0x4C);
        *(s16 *)(t + 0x2C) = *(u16 *)(p + 0x78);
        *(s16 *)(t + 0x2E) = *(u16 *)(p + 0x76);
        t[0x30] = 1;
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x40;
        m[0xD] = 0x40;
        m[0xC] = 0x40;
        func_8003DB94(m, D_800DE938, 0);
        *(s16 *)(m + 0x1E) = 0x2000;
        *(s16 *)(m + 0x1C) = 0x2000;
        *(s16 *)(m + 0x16) = (s16) * (u16 *)(t + 0x24) >> 12;
        *(s16 *)(m + 0x18) = (s16) * (u16 *)(t + 0x26) >> 12;
        *(s16 *)(m + 0x1A) = *(u16 *)(t + 0x2E);
        *(u16 *)(m + 0x10) |= 0x20;
        *(u16 *)(m + 0x14) |= 0xC;
        func_8004491C(n, (u32)D_80045340);
        v = (u8 *)n->unk_08;
        *(s32 *)(v + 0x0) = *(s32 *)(v + 0xC) = *(s32 *)(p + 0x34);
        *(s32 *)(v + 0x4) = *(s32 *)(v + 0x10) = *(s32 *)(p + 0x38);
        *(s32 *)(v + 0x8) = *(s32 *)(v + 0x14) = *(s32 *)(p + 0x3C);
        a.vx = (func_800644B8(*(s16 *)(t + 0x2E)) >> 4) * *(s16 *)(t + 0x2C) >> 8;
        a.vy = (func_80064584(*(s16 *)(t + 0x2E)) >> 4) * *(s16 *)(t + 0x2C) >> 8;
        a.vz = 0;
        PushMatrix();
        ApplyMatrixSV((MATRIX *)((u8 *)n + 0x24), &a, &b);
        PopMatrix();
        *(s16 *)(v + 0x2) = *(u16 *)(v + 0x2) + b.vx;
        *(s16 *)(v + 0x6) = *(u16 *)(v + 0x6) + b.vy;
        *(s16 *)(v + 0xA) = *(u16 *)(v + 0xA) + b.vz;
    }
}

/* The step of the node func_s53_800249E4 spawns: bump the owning
   renderable's +0x1A spin, publish `func_800644B8(phase << 8) >> 6' to the
   payload's +0x78, integrate the +0x40..+0x48 step into the +0x34..+0x3C
   position, hand off to func_s53_80024700, and retire once the +0x74 frame
   reaches 9.  51 words, 0 differing, FIRST COMPILE.

   THE ONE LOAD-BEARING SPELLING: `(u32)func_800644B8(...) >> 6' -- the cast
   is what makes the shift `srl'. Without it the shift is `sra' and the body
   is 51 words / 1 aligned difference.

   MEASURED AND FREE at 51 words / 0 differing: the `u8 *v' local for
   `*(u8 **)p' -- spelling both halves of the +0x1A bump inline is also 0. */
void func_s53_80024918(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *v;

    v = *(u8 **)p;
    *(s16 *)(v + 0x1A) = *(u16 *)(v + 0x1A) + 1;
    *(s16 *)(p + 0x78) = (u32)func_800644B8(*(s16 *)(p + 0x74) << 8) >> 6;
    *(s32 *)(p + 0x34) += *(s32 *)(p + 0x40);
    *(s32 *)(p + 0x38) += *(s32 *)(p + 0x44);
    *(s32 *)(p + 0x3C) += *(s32 *)(p + 0x48);
    func_s53_80024700(p);
    *(s16 *)(p + 0x74) = *(u16 *)(p + 0x74) + 1;
    if (*(s16 *)(p + 0x74) >= 9)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212 node that runs func_s53_80024918: park the caller's record
   at the payload's +0x0, clear the payload's +0x74 frame and seed its +0x76
   phase from the caller, copy the start point `b' into BOTH the payload's
   +0x1C..+0x24 and its +0x34..+0x3C, the end point `c' into +0x4..+0xC, the
   per-frame step `(c - b) >> 3' into +0x40..+0x48, and finally the caller's
   by-value MATRIX and SVECTOR into the payload's +0x4C and +0x6C.
   92 words, 0 differing, FIRST COMPILE.

   THE PROTOTYPE IS THE POINT, and it is read off the CALLER rather than this
   listing: the last two parameters are a MATRIX and an SVECTOR BY VALUE.
   MATRIX has `long t[3]' so its alignment is 4 and it copies with eight
   plain lw/sw in two groups of four; SVECTOR is four shorts, alignment 2, so
   mips.c's block move copies it with lwl/lwr + swl/swr even though both ends
   are word-aligned. That pair of unaligned words at the end of an otherwise
   aligned 40-byte argument block is the whole signature.

   THE ONE LOAD-BEARING SPELLING: each of the three start-point fields is
   stored TWICE from one load, written as a chained assignment
   `*(s32 *)(t + 0x34) = *(s32 *)(t + 0x1C) = b->unk_00;' -- right to left,
   so +0x1C stores first. Six separate statements re-read `b' each time and
   that is 98 words / 27 aligned differences (+6 words).

   MEASURED AND FREE at 92 words / 0 differing: the two block copies spelled
   off the payload pointer (`*(MATRIX *)(t + 0x4C) = m') instead of off the
   node (`*(MATRIX *)((u8 *)n + 0x6C) = m') -- combine folds either. */
void func_s53_800249E4(u8 *a, UnkVec *b, UnkVec *c, s16 d, MATRIX m, SVECTOR r)
{
    UnkNode *n;
    u8 *t;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s53_80024918;
        t = n->payload;
        *(u8 **)((u8 *)n + 0x20) = a;
        *(s16 *)(t + 0x74) = 0;
        *(s16 *)(t + 0x76) = d;
        *(s32 *)(t + 0x34) = *(s32 *)(t + 0x1C) = b->unk_00;
        *(s32 *)(t + 0x38) = *(s32 *)(t + 0x20) = b->unk_04;
        *(s32 *)(t + 0x3C) = *(s32 *)(t + 0x24) = b->unk_08;
        *(s32 *)(t + 0x4) = c->unk_00;
        *(s32 *)(t + 0x8) = c->unk_04;
        *(s32 *)(t + 0xC) = c->unk_08;
        *(s32 *)(t + 0x40) = (c->unk_00 - b->unk_00) >> 3;
        *(s32 *)(t + 0x44) = (c->unk_04 - b->unk_04) >> 3;
        *(s32 *)(t + 0x48) = (c->unk_08 - b->unk_08) >> 3;
        *(MATRIX *)((u8 *)n + 0x6C) = m;
        *(SVECTOR *)((u8 *)n + 0x8C) = r;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s53", func_s53_80024B54);
