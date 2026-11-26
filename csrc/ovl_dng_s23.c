#include "common.h"

s32 rand(void);

void func_800478B8(u8 *);
void func_s23_80024550(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u32 D_800814A0;

void func_8003DB94(void *, void *, s32);
UnkNode *func_8003FC64(s32);
s32 func_8004491C(UnkNode *, u32);
void func_s23_80024024(u8 *p, UnkVec *b, u8 *c, u8 *d);
UnkNode *func_s23_800240CC(u8 *p, UnkVec *b, s16 k);
void func_s23_800241D8(u8 *p, UnkVec *b, u8 *c, u8 *d);
UnkNode *func_s23_80024264(u8 *p, UnkVec *b);
void func_s23_80024370(u8 *p, UnkVec *b, u8 *c, u8 *d);
UnkNode *func_s23_8002441C(u8 *p, UnkVec *b);
UnkNode *func_s23_800245CC(u8 *p, UnkVec *b);
extern u8 D_80024E40[];
extern u8 D_80024E88[];
extern u8 D_80024EF4[];
extern u8 D_80024FC4[];
extern u8 D_80045340[];

/* The step function func_s23_800240CC installs: bump the owning actor's +0x14
   frame counter, advance the caller's vector by its own velocity half
   (+0x0 += +0xC and so on), spin the renderable's +0x1A by 0x300, then count
   the instance's +0x10 up and retire it once it passes the +0x32 limit
   func_s23_800240CC stored there.
   42 words, 0 differing. First compile; it is a LEAF with no frame at all.

   NO FACT ON THIS BODY IS LOAD-BEARING. All three plausible ones measured and
   FREE at 42 words / 0 differing, so none of them is the reason it lands:
     the comparison written `*(s16 *)(p + 0x12) < *(s16 *)(p + 0x10)'
     the three `b' updates written as plain `*(s32 *)((u8 *)b + K)' casts
     the actor pointer hoisted into a `u8 *a' local instead of read twice
   That is what a pressure-free leaf looks like -- it has no callee-saved
   registers to fight over, so do not carry any of these to a body that has. */
void func_s23_80024024(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)(*(u8 **)(p + 0xC) + 0x14) = *(u16 *)(*(u8 **)(p + 0xC) + 0x14) + 1;
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x300;
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    if (*(s16 *)(p + 0x10) > *(s16 *)(p + 0x12))
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212 sprite node that runs func_s23_80024024, park the owning
   actor at its +0x2C, zero the +0x30 counter and keep the caller's limit at
   +0x32; dress the renderable white, hang D_80024E88 off the pattern slot,
   jitter its +0x1A by rand() % 0x1000 and scale it 0x800 square; then publish
   the caller's vector into the node's own UnkVec unchanged.
   67 words, 0 differing.

   Same 0x212-spawner shape as func_s23_80024264 below and the landed
   func_s28_80025614 (csrc/ovl_dng_s28.c); func_80069EF8 is `rand' out of
   splat/common_syms.txt, so no declaration and no address.

   THE UnkVec PUBLISH INVERTS AGAINST THE OTHER THREE SPAWNERS IN THIS UNIT,
   which is worth more than any other fact here. This body is the ONLY one of
   the four that wants the struct assignment. Measured on this body:
     `*n->unk_08 = *b;' -- gcc's block move                          67 w,  0
     the same six fields written out one at a time                   77 w, 39
   and the other three want six fields: func_s23_80024264 65 w / 33 for the
   copy against 67 w / 0 for the fields, func_s23_800245CC 58 w / 32 against
   60 w / 0, func_s23_8002441C 72 w / 31 against 77 w / 0.
   I DO NOT KNOW THE DISCRIMINATOR AND MY FIRST GUESS WAS WRONG: I wrote that
   it was the constant added to z, since 80024264 and 800245CC each add one
   and so cannot be a pure block move -- but func_s23_8002441C adds NO
   constant and still measures 72 w / 31 for the struct assignment. So the
   constant is not it. Read the listing: a block move is four `lw' then four
   `sw' with no `nop' between them, six statements are `lw'/`nop'/`sw' six
   times over, and that tells you which you have before you compile.
   Also measured here:
     the `q = (u8 *)n + 0x20' local kept, three stores off it        67 w,  0
     the same three stores written off `(u8 *)n + 0x2C/0x30/0x32'    66 w, 54
     the third parameter declared `s32' rather than `s16'            67 w,  6
     the `m + 0x8' store moved after the rand()                      67 w, 13
   The m+0x8 store APPEARS in rand's delay slot in the listing; it is written
   BEFORE the rand in the source and sched sinks it, which is the brief's
   delay-slot trap and is why the listing order is not the source order. */
UnkNode *func_s23_800240CC(u8 *p, UnkVec *b, s16 k)
{
    UnkNode *n;
    u8 *q;
    u8 *m;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s23_80024024;
        q = (u8 *)n + 0x20;
        *(u8 **)(q + 0xC) = p;
        *(s16 *)(q + 0x10) = 0;
        *(s16 *)(q + 0x12) = k;
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        *(u8 **)(m + 0x8) = D_80024E88;
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1E) = 0x800;
        *(s16 *)(m + 0x1C) = 0x800;
        func_8004491C(n, (u32)D_80045340);
        *n->unk_08 = *b;
        return n;
    }
    return NULL;
}

/* The 0x212 sprite's step function with a second counter: bump the owning
   node's +0x14 frame counter AND this instance's own +0x4 counter, hand the
   renderable to func_800478B8, and when its +0x14 reports 0x6000 retire the
   owning actor and set the same bit in D_800814A0.
   35 words, 0 differing.

   This is the already-landed func_s23_80024550 below in this unit, plus the
   four-word `p + 0x4' bump, and it landed on the first compile.
   MEASURED: the two bumps' ORDER is load-bearing -- the `p + 0x0' chain must
   come FIRST; with the `p + 0x4' bump first it is 34 words / 32 differing,
   because cse then keeps the `lw $v1, 0x0($s1)' load after the first store and
   sched has nothing to fill the delay slot with.
   Measured and FREE at 35 words / 0 differing, so NOT the reason this lands:
   the cast widths on the `p + 0x4' bump, either way round
   (`*(u16 *) = *(s16 *) + 1' scores the same 0). */
void func_s23_800241D8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)(*(u8 **)(p + 0x0) + 0x14) = *(u16 *)(*(u8 **)(p + 0x0) + 0x14) + 1;
    *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212 sprite node that runs func_s23_800241D8, park the owning
   actor at its +0x20 and zero the +0x24 counter, dress the renderable white
   at 0x2000/0x1000/0xC00 out of this object's own D_80024EF4, then publish
   the caller's vector into the node's own UnkVec with 0x500000 added to z.
   67 words, 0 differing.

   Shape taken from the landed func_s28_80025614 (csrc/ovl_dng_s28.c, 0.814
   by LEADS) and func_s13_8002421C (csrc/ovl_dng_s13.c); neither device of
   either was needed. MEASURED here, all four on this body:
     the `q = n->unk_08' local kept and every field written off it   67 w,  0
     the same six fields written off `n->unk_08' at each site        71 w, 30
     `*q = *b;' then `q->unk_08 = q->unk_08 + 0x500000;'             65 w, 33
     the gate inverted to `if (n == NULL) return NULL;'              67 w, 47
     the +0x20 and +0x24 stores in the other order                   67 w,  2
   The dress-site fact runs the way the brief predicts for sites ON ONE PATH:
   shared local 0, one-per-site 30. And the six fields must be six statements
   -- the struct copy is two words SHORT because it becomes a block move.
   Measured and FREE at 67 words / 0 differing, so NOT the reason this lands:
   writing the constant on the left, `0x500000 + b->unk_08'. */
UnkNode *func_s23_80024264(u8 *p, UnkVec *b)
{
    UnkNode *n;
    u8 *m;
    UnkVec *q;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s23_800241D8;
        m = (u8 *)n->unk_0C;
        *(u8 **)((u8 *)n + 0x20) = p;
        *(s16 *)((u8 *)n + 0x24) = 0;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        func_8003DB94(m, D_80024EF4, 0);
        *(s16 *)(m + 0x1C) = 0x2000;
        *(s16 *)(m + 0x1E) = 0x1000;
        *(s16 *)(m + 0x1A) = 0xC00;
        func_8004491C(n, (u32)D_80045340);
        q = n->unk_08;
        q->unk_00 = b->unk_00;
        q->unk_04 = b->unk_04;
        q->unk_08 = b->unk_08 + 0x500000;
        q->unk_0C = b->unk_0C;
        q->unk_10 = b->unk_10;
        q->unk_14 = b->unk_14;
        return n;
    }
    return NULL;
}

/* The step function func_s23_8002441C installs: bump the owning actor's +0x14
   frame counter and this instance's own +0x4 counter, then on frame 8 spawn
   the follow-up node through func_s23_80024264 and on frame 0x10 retire the
   owning actor; hand the renderable to func_800478B8 either way.
   43 words, 0 differing. First compile.

   NO TWIN AT ANY RATIO (LEADS tops out at 0.659) -- read off its own
   listing, with func_s23_800241D8 above for the counter-bump idiom.

   IT IS A `switch', NOT AN if/else-if CHAIN, even with only two arms. Two
   cases 8 apart is far too sparse for gcc 2.7.2's jump table, so
   `expand_end_case' emits a two-compare tree -- `beq' to each ARM with the
   fall-through going to the common tail -- and that is why there is no
   `jtbl_s23_*' for this function. An if/else-if chain inverts both tests and
   branches AROUND each arm instead. Measured, all on this body:
     the `switch' with cases in the order 8 then 0x10                43 w,  0
     the same two arms as `if (...) else if (...)'                   41 w, 23
     the `switch' with the cases written 0x10 then 8                 40 w, 23
     the func_s23_80024264 call's second argument passed as NULL     43 w,  1
   The case ORDER is the source's: a comparison tree tests in the order the
   cases are written, so this is a real spelling and not a cross-join.
   The last row is worth noting as a MEASURABLE argument: `b' reaches
   func_s23_80024264 already in $a1 and the target emits no setup for it at
   all, so the call looks like a one-argument call in the listing -- but
   passing anything else costs a word of setup, which is how you can tell. */
void func_s23_80024370(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)(*(u8 **)(p + 0x0) + 0x14) = *(u16 *)(*(u8 **)(p + 0x0) + 0x14) + 1;
    *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
    switch (*(s16 *)(p + 0x4))
    {
    case 0x8:
        func_s23_80024264(*(u8 **)(p + 0x0), b);
        break;
    case 0x10:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
    func_800478B8(c);
}

/* Spawn the 0x212 sprite node that runs func_s23_80024370, park the owning
   actor at its +0x20 and zero the +0x24 counter; dress the renderable out of
   this object's own D_80024E40 with the blue channel pulled down to 0xC0,
   scale it 0x2000 by 0x1000, set the 0x7DCF pattern word and raise three
   flag bits; then publish the caller's vector unchanged.
   77 words, 0 differing. First compile.

   Same 0x212-spawner shape as the other three in this unit and as the landed
   func_s13_8002421C (csrc/ovl_dng_s13.c, which LEADS ranks 0.699); its
   combined `|= 0x10C' is the one spelling of its own that does NOT carry here.
   Measured on this body:
     m[0xE] written 0x80 and then overwritten 0xC0                   77 w,  0
     the overwritten m[0xE] = 0x80 dropped                           75 w, 50
     the two `m + 0x14' ors kept apart, 0x20 between them            77 w,  0
     the two combined into one `|= 0x10C' as the s13 twin writes it  74 w, 35
     the six UnkVec fields written out                               77 w,  0
     `*n->unk_08 = *b;' -- and this body adds NO constant to z       72 w, 31
   THE DEAD STORE IS REAL AND IT IS 50 ROWS. gcc 2.7.2 does not eliminate a
   dead store to memory, so `m[0xE] = 0x80' survives its own overwrite and
   both `sb's are in the ROM; it must be written BEFORE the 0xC0, which is
   the same ordering rule as the brief's zero-store note. */
UnkNode *func_s23_8002441C(u8 *p, UnkVec *b)
{
    UnkNode *n;
    u8 *m;
    UnkVec *q;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s23_80024370;
        m = (u8 *)n->unk_0C;
        *(u8 **)((u8 *)n + 0x20) = p;
        *(s16 *)((u8 *)n + 0x24) = 0;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        m[0xE] = 0xC0;
        func_8003DB94(m, D_80024E40, 0);
        *(s16 *)(m + 0x1C) = 0x2000;
        *(s16 *)(m + 0x1E) = 0x1000;
        *(s16 *)(m + 0x12) = 0x7DCF;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x20;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8004491C(n, (u32)D_80045340);
        q = n->unk_08;
        q->unk_00 = b->unk_00;
        q->unk_04 = b->unk_04;
        q->unk_08 = b->unk_08;
        q->unk_0C = b->unk_0C;
        q->unk_10 = b->unk_10;
        q->unk_14 = b->unk_14;
        return n;
    }
    return NULL;
}

/* The 0x212 sprite's own step function: bump the owning node's +0x14 frame
   counter, hand the renderable to func_800478B8, and when its +0x14 reports
   0x6000 mark the owning actor retired and set the same bit in D_800814A0.
   31 words, 0 differing.

   This is the landed func_s10_80024374 in csrc/ovl_dng_s10.c (0.862 by
   dngtwin.py) plus the four-word counter bump on `*(u8 **)(p + 0x0)' at the
   front; that twin's note about D_800814A0 being the PLAIN scalar rather than
   an array alias holds here too and is what gives the `lui' / `lw %lo' pair.
   Measured and FREE at 31 words / 0 differing, so it is NOT the reason this
   lands: reading the counter back through `*(s16 *)' instead of `*(u16 *)'
   for the `+ 1'. */
void func_s23_80024550(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)(*(u8 **)(p + 0x0) + 0x14) = *(u16 *)(*(u8 **)(p + 0x0) + 0x14) + 1;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212 sprite node that runs the already-landed
   func_s23_80024550, park the owning actor at its +0x20 (and NOT the +0x24
   counter the other three spawners zero), dress the renderable white out of
   this object's own D_80024FC4 at 0x1400 square, then publish the caller's
   vector with 0xA00000 added to z.
   60 words, 0 differing. First compile, and the shortest of this unit's four
   0x212 spawners -- it is func_s23_80024264 with the +0x24 store, the +0x1A
   jitter and one scale constant removed.

   Measured on this body:
     the six UnkVec fields written out                               60 w,  0
     `*q = *b;' then `q->unk_08 = q->unk_08 + 0xA00000;'             58 w, 32
     the two 0x1400 scale stores in the order +0x1E then +0x1C       60 w,  0
     the same two in the other order                                 60 w,  2
   Measured and FREE at 60 words / 0 differing, so NOT the reason this lands:
   `m = (u8 *)n->unk_0C' moved after the +0x20 store. (On this body `m' never
   outlives the func_8003DB94 call, so it sits in $a0 and competes with
   nothing -- the same move on a body whose `m' crosses two calls would not
   be free.) */
UnkNode *func_s23_800245CC(u8 *p, UnkVec *b)
{
    UnkNode *n;
    u8 *m;
    UnkVec *q;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s23_80024550;
        m = (u8 *)n->unk_0C;
        *(u8 **)((u8 *)n + 0x20) = p;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        *(s16 *)(m + 0x1E) = 0x1400;
        *(s16 *)(m + 0x1C) = 0x1400;
        func_8003DB94(m, D_80024FC4, 0);
        func_8004491C(n, (u32)D_80045340);
        q = n->unk_08;
        q->unk_00 = b->unk_00;
        q->unk_04 = b->unk_04;
        q->unk_08 = b->unk_08 + 0xA00000;
        q->unk_0C = b->unk_0C;
        q->unk_10 = b->unk_10;
        q->unk_14 = b->unk_14;
        return n;
    }
    return NULL;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s23", func_s23_800246BC);
