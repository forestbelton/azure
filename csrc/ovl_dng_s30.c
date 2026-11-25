#include "common.h"

s32 func_8009D218_2(u8 *, s32) __asm__("func_8009D218");
s32 func_800A6870(s16);
void func_800AD4D0(u8 *);
s32 func_800AD568(u8 *);
UnkNode *func_800B4C7C(u16, u8 *, s16, s16);
void func_s30_80024024(u8 *p, u8 k);

/* Raise the actor's +0x64 pool by func_800A6870's scale of the second
   parameter plus 4, half again when the actor's +0x28 has bit 0, then
   republish it with func_800AD568, float the 0x8004 number and refresh with
   func_800AD4D0. All of it gated on func_8009D218 coming back zero.
   39 words, 0 differing, first compile.

   TWIN: the landed func_s27_80024050 in csrc/ovl_dng_s27.c (1.000 to two
   stubbed clones, 0.949 to this). Deltas, all off this listing: the gate
   argument is 1 not 4, the scale bonus is +4 not +8, the +0x28 bit is 1 not
   4, and the fraction is `>> 1' (`sll 16'/`sra 17') not `>> 2'.

   Of the twin's two stated facts:
     - HOLDS: the second parameter is `u8', which is what makes the
       `andi $a0, $s1, 0xFF' ahead of func_800A6870 the parameter's own
       truncation; and func_8009D218 is reached through the project's
       TWO-argument alias.
     - HOLDS on the WIDTH, WEAKER on the PAIR: the twin calls its strongest
       fact "the pair of `s16' locals with the plain `u = t;' ahead of the
       gate", priced at 38 words / 28 differing for one variable. Measured
       here: `s32 t; s32 u;' is 37 words / 27 differing, so the `s16' WIDTH
       is load-bearing and reproduces the twin's price to one -- but ONE
       variable with `t += (s16)t >> 1;' and no `u' at all is 39 words /
       0 differing. The two-variable spelling is FREE on this copy. 
   THIS COPY'S OWN CONSTANT: 0x1 -> 0x4 at 1 of 4 sites, 0x4 -> 0x10.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_800478B8(u8 *);

UnkNode *func_8003FC64(s32);
void func_8003DB94(void *, void *, s32);
s32 func_80045C34(u8 *, UnkVec *, UnkSlot *);
extern u8 D_80025AF0[];
extern u8 D_80025B38[];
extern u8 D_800DEC70[];
extern u8 D_80045340[];
void func_s30_800240C0(u8 *p, UnkVec *b, u8 *c);
s32 func_s30_800241D4(u8 *, UnkVec *, UnkSlot *);
void func_s30_80024710(u8 *p, UnkVec *b, u8 *c);
UnkNode *func_s30_800247AC(u8 *p, UnkVec *b, s16 k);
void func_s30_80024A08(u8 *p, UnkVec *b, u8 *c);
UnkNode *func_s30_80024AF4(u8 *p, UnkVec *b, u8 *c);
void func_s30_80024C48(u8 *p, UnkVec *b, u8 *c);
void func_s30_80025098(u8 *p, UnkVec *b, u8 *c);
UnkNode *func_s30_8002523C(u8 *p, UnkVec *b);
UnkNode *func_s30_80024F0C(u8 *p, UnkVec *b);
UnkNode *func_s30_80024640(u8 *p, UnkVec *b);
void func_s30_80024024_3(u8 *, u8, u8 *) __asm__("func_s30_80024024");
s32 func_800A44E0(u16, u16, s16, u32);
s32 func_800BCB04(s32, s32, s16);
s32 func_800A56E0(s32);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s32 D_8008346C;

void func_s30_80024024(u8 *p, u8 k)
{
    s16 t;
    s16 u;

    if (func_8009D218_2(p, 4) == 0)
    {
        t = func_800A6870(k) + 16;
        u = t;
        if ((p[0x28] & 1) != 0)
        {
            u = t + (t >> 1);
        }
        *(s16 *)(p + 0x64) = *(u16 *)(p + 0x64) + u;
        func_800AD568(p);
        func_800B4C7C(0x8004, p, *(s16 *)(p + 0x64), 1);
        func_800AD4D0(p);
    }
}

/* The step of the node func_s30_80024640 spawns: bump the +0xC counter on the
   actor at the payload's +0x0 and the payload's own +0x4 frame counter, then
   fade the renderable's +0xA byte up to 0x80 over the first 0x10 frames, hold
   it, and ramp it down past 0x20, while the +0x8 sprite number cycles through
   0x7DCF/0x7E00/0x7E01/0x7E02 on the counter mod 4. Retires the actor past
   frame 0x30.
   69 words, 0 differing. Landed off its SPAWNER, which this unit already had:
   func_s30_80024640 writes +0x4, +0x6, +0x8 and +0xA of the payload, so no
   offset needed guessing.

   TWO PRICED FACTS:
     `p[0xA] = p[0x4] * 8' -- a BYTE read of the counter      69 w,  0  kept
       ... `*(s16 *)(p + 0x4) * 8'                            66 w,  5
     `-0x80 - (*(s16 *)(p + 0x4) - 0x20) * 8'                 69 w,  0  kept
       ... `-0x80 - *(s16 *)(p + 0x4) * 8 + 0x100'            68 w,  2
   The byte read is the brief's lbu-versus-lh tell and it is worth 3 WORDS, not
   just rows: the halfword read needs its own sll16/sra16 and the arm then
   cannot share the 0x80 constant. The subtraction fact is the brief's
   `A + K - B' versus `A - (B - K)' rule firing at 2 rows with nothing moved.

   MEASURED AND FREE, so it is NOT why this lands: the 0x30 retire arm written
   as an early `return' with the rest of the body unnested rather than as the
   `else' of an if/else (69 words, 0 aligned differences either way). The four
   sprite-number arms are a COMPARE-CHAIN switch, not a table, and they are
   written ascending because the listing's arm addresses ascend; that order is
   priced on this unit's func_s30_800247AC, which has the identical switch. */
void func_s30_800240C0(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(*(u8 **)p + 0xC) = *(u16 *)(*(u8 **)p + 0xC) + 1;
    *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
    if (*(s16 *)(p + 0x4) >= 0x30)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    else
    {
        if (*(s16 *)(p + 0x4) < 0x10)
        {
            p[0xA] = p[0x4] * 8;
        }
        else if (*(s16 *)(p + 0x4) < 0x20)
        {
            p[0xA] = 0x80;
        }
        else
        {
            p[0xA] = -0x80 - (*(s16 *)(p + 0x4) - 0x20) * 8;
        }
        switch (*(s16 *)(p + 0x4) % 4)
        {
        case 0:
            *(s16 *)(p + 0x8) = 0x7DCF;
            break;
        case 1:
            *(s16 *)(p + 0x8) = 0x7E00;
            break;
        case 2:
            *(s16 *)(p + 0x8) = 0x7E01;
            break;
        case 3:
            *(s16 *)(p + 0x8) = 0x7E02;
            break;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s30", func_s30_800241D4);

/* Spawns the 0x212 node that runs func_s30_800240C0 as its per-frame step and
   func_s30_800241D4 as its renderer, with the caller's record at the payload's
   +0x0, a zeroed +0x4/+0x6 counter pair, sprite 0x7DCF at +0x8, a zero fade
   byte at +0xA, 0x1000 scale on both axes and the caller's mover copied whole.
   52 words, 0 differing, first compile.

   DONOR: the landed func_s28_80025614 in csrc/ovl_dng_s28.c, the same bank's
   0x212 spawn one object over, which dngtwin does not rank for this stub at
   all -- what named csrc/ovl_dng_s28.c was the 0.943 row dngtwin prints for
   this unit's func_s30_80025390. Its `q = (u8 *)n + 0x20' payload view is the
   whole of this body.

   TWO PRICED FACTS:
     `q = (u8 *)n + 0x20' as a POINTER LOCAL                  52 w,  0  kept
       ... the five stores named off `n' as +0x20..+0x2A      51 w, 13
     func_8004491C called BEFORE the payload stores           52 w,  0  kept
       ... called last, after the mover copy                  45 w, 29
   The pointer local reproduces func_s22_80024824's fact and its mechanism:
   the missing word is the target's `addiu $v0, $s0, 0x20', and combine folds
   the q+0 store back onto `0x20($s0)' by itself, which is why the local is
   invisible at that one site.
     `*(s16 *)(m + 0x1E) = 0x1000' before the +0x1C store     52 w,  0  kept
       ... the other order                                    52 w,  2
   Same direction and nearly the same price as func_s28_80025614's own 4. */
UnkNode *func_s30_80024640(u8 *p, UnkVec *b)
{
    UnkNode *n;
    u8 *q;
    u8 *m;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s30_800240C0;
        func_8004491C(n, (u32)func_s30_800241D4);
        q = (u8 *)n + 0x20;
        *(u8 **)q = p;
        *(s16 *)(q + 0x4) = 0;
        *(s16 *)(q + 0x6) = 0;
        *(s16 *)(q + 0x8) = 0x7DCF;
        q[0xA] = 0;
        m = (u8 *)n->unk_0C;
        *(s16 *)(m + 0x1E) = 0x1000;
        *(s16 *)(m + 0x1C) = 0x1000;
        *n->unk_08 = *b;
        return n;
    }
    return NULL;
}

/* A per-frame step function: bump the +0x14 counter on the node at the
   actor's +0x0 and the actor's own +0x4 counter, drag the mover's +0x8 down
   by 0xA0000, hand the renderable to func_800478B8, and when its +0x14
   reports 0x6000 mark the owning actor retired and set the same bit in
   D_800814A0.
   39 words, 0 differing, first compile.

   TWIN: the landed func_s13_8002418C in csrc/ovl_dng_s13.c (dngtwin 0.933,
   36 words). Its head statement, its gate and its whole retire block carry
   verbatim; the three extra words are this copy's own middle, where the
   twin's `+0x1E' scale pair is replaced by the actor's +0x4 counter bump and
   the `b->unk_08 -= 0xA0000'. The twin's note on D_800814A0 being the PLAIN
   `u32' scalar (structs.h, no declaration needed) holds here too and gives
   the `lui %hi'/`lw %lo' pair, with the `lui' hoisted into the gate's delay
   slot. The twin's own priced fact -- the scale pair as a read-back, not a
   second sum -- does not apply: this copy has no scale pair.

   ONLY THE STATEMENT ORDER IS LOAD-BEARING. Priced on this body, all at
   39 words:

     the +0x4 counter bump BEFORE the +0x8 drag               0   kept
       ... the two swapped, the drag first                    8
     `b->unk_08 -= 0xA0000;'                                  0   kept
       ... `+= -0xA0000;'                                     0   free
       ... `*(s32 *)((u8 *)b + 0x8) -= 0xA0000;'              0   free
       ... second parameter `u8 *b', `*(s32 *)(b + 0x8)'      0   free
     the +0x4 bump read through `u16'                         0   kept
       ... read through `s16'                                 0   free
     the head reached through `*(u8 **)p' twice               0   kept
       ... hoisted into a `u8 *n' local                       0   free
     the gate read through `u16', the retire store `u16'      0   kept
       ... either one through `s16'                           0   free

   So the UnkVec second parameter is the twin's spelling rather than a
   measured one, and the only thing this listing actually constrains is that
   the +0x8 drag is emitted last of the three. The drag is a `lui 0xFFF6' plus
   `addu' because 0xFFF60000 is outside `addiu' range; the `-=' and `+= -'
   forms are the same tree. 
   THIS COPY'S OWN CONSTANT: 0x14 -> 0xc at 2 of 3 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s30_80024710(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(*(u8 **)p + 0xc) = *(u16 *)(*(u8 **)p + 0xc) + 1;
    *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
    b->unk_08 -= 0xA0000;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawns the 0x212 node that runs the already-landed func_s30_80024710 as its
   step: a renderable dressed from D_800DEC70 at white 0x80 with the +0x14 0xC
   and 0x100 bits and the +0x10 0x60 bits set, its sprite number chosen from
   0x7DCF/0x7E00/0x7E01/0x7E02 on the CALLER's +0x12 mod 4, a random rotation,
   0x1000 scale, D_80045340 as the per-slot walk, and the caller's mover copied
   whole and then pushed along the third parameter's heading by a random
   0x20..0x30 step.
   151 words, 0 differing, first compile.

   DONOR: the landed func_s28_80025614 in csrc/ovl_dng_s28.c for the spawn
   skeleton; the tail's two trig pushes are this body's own.

   TWO PRICED FACTS:
     `v = n->unk_08' as a local, with the mover copy and both
       pushes named off it                                   151 w,  0  kept
       ... `*n->unk_08 = *b' and `n->unk_08->unk_00 += ...'  153 w, 35
     `s16 d' for the random 0x20..0x30 step                  151 w,  0  kept
       ... `s32 d'                                           149 w,  3
   The local is worth 2 words and 35 rows: without it the target's single
   `lw $s2, 0x8($s3)' becomes three reloads and $s2 is not reused for the
   mover. The `s16' width is the brief's narrow-call-result rule -- the value
   feeds a `mult' and the target sign-extends it from 16 bits.
     `rand() % 17' -- a SIGNED modulo                        151 w,  0  kept
       ... `(rand() & 0xF) + 0x20'                           139 w, 40
   Twelve words of magic-multiply quintet, the same shape func_s22_800240F8
   records for its `rand() % 0x1000'.

   MEASURED AND FREE, so neither is why this lands: `((x >> 4) * d) << 8' for
   `(x >> 4) * d * 0x100' (151 / 0 -- the brief's precondition holds, the
   candidate already emits a separate `sll' so the choice is free); and the
   `% 4' switch's four arms are ascending because the listing's arm addresses
   ascend, which no tool can see. */
UnkNode *func_s30_800247AC(u8 *p, UnkVec *b, s16 k)
{
    UnkNode *n;
    u8 *q;
    u8 *m;
    UnkVec *v;
    s16 d;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s30_80024710;
        *(u8 **)((u8 *)n + 0x20) = *(u8 **)p;
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x4) = 0;
        *(s16 *)(q + 0x6) = 0;
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x60;
        *(u16 *)(m + 0x14) |= 0x100;
        switch (*(s16 *)(p + 0x12) % 4)
        {
        case 0:
            *(s16 *)(m + 0x12) = 0x7DCF;
            break;
        case 1:
            *(s16 *)(m + 0x12) = 0x7E00;
            break;
        case 2:
            *(s16 *)(m + 0x12) = 0x7E01;
            break;
        case 3:
            *(s16 *)(m + 0x12) = 0x7E02;
            break;
        }
        func_8003DB94(m, D_800DEC70, 0);
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1E) = 0x1000;
        *(s16 *)(m + 0x1C) = 0x1000;
        func_8004491C(n, (u32)D_80045340);
        v = n->unk_08;
        *v = *b;
        d = rand() % 17 + 0x20;
        v->unk_00 += (func_800644B8(k) >> 4) * d * 0x100;
        v->unk_04 += (func_80064584(k) >> 4) * d * 0x100;
        return n;
    }
    return NULL;
}

/* The step of the node func_s30_80024AF4 spawns: bump the actor's +0xC and the
   payload's +0x4, spin the renderable's +0x1A by 0x480, drag the mover's +0x8
   down by 0xA0000, hand the renderable to func_800478B8, and then either fade
   its three colour bytes down by 0xC each (stopping at zero) or, past frame
   0x11, retire the actor into D_800814A0.
   59 words, 0 differing, first compile.

   DONOR: this unit's own already-landed func_s30_80024710, which has the same
   head bumps, the same `b->unk_08 -= 0xA0000' drag and the same retire block;
   only the +0x1A spin and the three byte fades are new. That is the brief's
   "land the small siblings in your own unit first" and it needed no external
   lead at all.

   ONE PRICED FACT, and it is the twin's: the +0x1A spin and the +0x8 drag are
   emitted in that order with the drag's store sunk into func_800478B8's delay
   slot. func_s30_80024710 prices its own statement order at 8 rows; here the
   order is read straight off the listing and not re-priced, because the drag
   is the last of the three in both bodies.

   MEASURED AND FREE, so it is NOT why this lands: `if (c[0xC])' for
   `if (c[0xC] != 0)' (59 words, 0 aligned differences). */
void func_s30_80024A08(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(*(u8 **)p + 0xC) = *(u16 *)(*(u8 **)p + 0xC) + 1;
    *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x480;
    b->unk_08 -= 0xA0000;
    func_800478B8(c);
    if (*(s16 *)(p + 0x4) < 0x11)
    {
        if (c[0xC] != 0)
        {
            c[0xC] -= 0xC;
        }
        if (c[0xD] != 0)
        {
            c[0xD] -= 0xC;
        }
        if (c[0xE] != 0)
        {
            c[0xE] -= 0xC;
        }
    }
    else
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawns the 0x212 node that runs func_s30_80024A08 as its step: the actor at
   the payload's +0x0 (read THROUGH the caller's payload, not the payload
   itself), a zeroed +0x4/+0x6 pair, the caller renderable's +0xC colour word
   copied over and every non-zero channel clamped to 0xC0, dressed from
   D_80025B38, 0x1400 scale and the caller's +0x1A rotation.
   85 words, 0 differing, first compile.

   DONOR: the landed func_s28_80025614 in csrc/ovl_dng_s28.c.

   ONE PRICED FACT:
     `*(s16 *)(m + 0x1A) = *(u16 *)(c + 0x1A)' BEFORE the two
       0x1400 scale stores                                    85 w,  0  kept
       ... after them, immediately before func_8004491C        85 w,  7
   The listing cannot be read for this -- sched sinks the +0x1A store into
   func_8004491C's delay slot either way -- so it is a measured position, not a
   transcribed one. That is the brief's "a store's position inside a dress
   block is not readable" firing at 7 rows.

   MEASURED AND FREE, so it is NOT why this lands: `if (m[0xC])' for
   `if (m[0xC] != 0)' at all three channels (85 / 0). */
UnkNode *func_s30_80024AF4(u8 *p, UnkVec *b, u8 *c)
{
    UnkNode *n;
    u8 *q;
    u8 *m;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s30_80024A08;
        *(u8 **)((u8 *)n + 0x20) = *(u8 **)p;
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x4) = 0;
        *(s16 *)(q + 0x6) = 0;
        m = (u8 *)n->unk_0C;
        *(s32 *)(m + 0xC) = *(s32 *)(c + 0xC);
        if (m[0xC] != 0)
        {
            m[0xC] = 0xC0;
        }
        if (m[0xD] != 0)
        {
            m[0xD] = 0xC0;
        }
        if (m[0xE] != 0)
        {
            m[0xE] = 0xC0;
        }
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x60;
        func_8003DB94(m, D_80025B38, 0);
        *(s16 *)(m + 0x1A) = *(u16 *)(c + 0x1A);
        *(s16 *)(m + 0x1E) = 0x1400;
        *(s16 *)(m + 0x1C) = 0x1400;
        func_8004491C(n, (u32)func_80045C34);
        *n->unk_08 = *b;
        return n;
    }
    return NULL;
}

/* The step of the node func_s30_80024F0C spawns, and this unit's secondary
   dispatcher: bump the actor's +0xC, the payload's +0x10 frame counter and its
   +0x12 slot counter, spin the renderable's +0x1A by 0x300, place its three
   colour bytes from the slot counter's residue modulo 7 -- the /4, the (/2)%2
   and the %2 bits each scaled by 128 -- and then dispatch on the payload's
   +0xE phase. Phase 0 sets the +0x1C/+0x1E scale from 0x1400 / +0x14 times the
   frame counter and integrates the mover's velocity until the counter passes
   +0x14; phase 1 waits 0xC frames; phase 2 spawns one func_s30_80024AF4 node
   and TWO func_s30_800247AC nodes at a random heading and that heading plus
   half a turn, then waits to frame 0x21; phase 3 retires the actor once the
   renderable's +0x14 reports 0x6000.
   177 words, 0 differing, first compile.

   The 4-case switch is a BRANCH TREE, not a table: the dispatch tests `== 1',
   then `< 2', then `== 0', then `== 2' and `== 3', with the default falling to
   the epilogue. There is no jtbl symbol and nothing for this unit's rodata.

   DONOR: the landed func_s28_800250E8 in csrc/ovl_dng_s28.c -- the same
   modulo-7 colour chain and the same phase-tree shape one object over, 186
   words -- which dngtwin does not rank for this stub (it reports none at 0.70)
   and which the s28 pairing found for free.

   THREE PRICED FACTS, and the first two reproduce the donor's direction:
     `s16 h' for `k / 2'                                     177 w,  0  kept
       ... `s32 h'                                           176 w, 20
     case 0's gate `p+0x10 >= p+0x14'                        177 w,  0  kept
       ... `p+0x14 <= p+0x10'                                177 w,  2
     case 1's two stores as +0xE THEN +0x10                  177 w,  0  kept
       ... +0x10 then +0xE, i.e. cases 0/2's order           172 w, 12
   The third is the one worth carrying: cases 0 and 2 end with
   `p+0x10 = 0; p+0xE += 1;' and jump2 cross-joins them into one block, but
   case 1 must write the SAME TWO STORES IN THE OPPOSITE ORDER or it joins that
   block too and the body comes out five words short. Two arms of one switch
   want opposite orders, which is the brief's "two bodies in one unit can want
   opposite answers" inside a single body. */
void func_s30_80024C48(u8 *p, UnkVec *b, u8 *c)
{
    s16 k;
    s16 h;
    s16 d;

    *(s16 *)(*(u8 **)p + 0xC) = *(u16 *)(*(u8 **)p + 0xC) + 1;
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    *(s16 *)(p + 0x12) = *(u16 *)(p + 0x12) + 1;
    k = *(s16 *)(p + 0x12) % 7 + 1;
    func_800478B8(c);
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x300;
    c[0xC] = (k / 4) * 128;
    h = k / 2;
    c[0xD] = (h % 2) * 128;
    c[0xE] = (k % 2) * 128;
    switch (*(s16 *)(p + 0xE))
    {
    case 0:
        *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) =
            0x1400 / *(s16 *)(p + 0x14) * *(s16 *)(p + 0x10);
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x14))
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 1;
        }
        break;
    case 1:
        if (*(s16 *)(p + 0x10) >= 0xC)
        {
            *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 1;
            *(s16 *)(p + 0x10) = 0;
        }
        break;
    case 2:
        func_s30_80024AF4(p, b, c);
        d = rand() % 0x1000;
        func_s30_800247AC(p, b, d);
        func_s30_800247AC(p, b, (d + 0x800) % 0x1000);
        if (*(s16 *)(p + 0x10) >= 0x21)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 1;
        }
        break;
    case 3:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* Spawns the 0x212 node that runs func_s30_80024C48 as its step: the caller's
   record at the payload's +0x0, a zeroed +0xE phase and +0x10 counter, a random
   0..6 at +0x12, the caller's +0x14 budget copied to +0x14, and a renderable
   dressed from D_80025AF0 at white 0x80 with sprite 0x7DCF, the +0x14 0xC and
   0x100 bits, the +0x10 0x20 bit, a random rotation and 0x1400 / +0x14 scale.
   99 words, 0 differing, first compile.

   DONOR: the landed func_s28_80025614 in csrc/ovl_dng_s28.c.

   TWO PRICED FACTS:
     the scale written as the CHAIN
       `*(s16 *)(m + 0x1C) = *(s16 *)(m + 0x1E) = 0x1400 / ...' 99 w, 0  kept
       ... the chain the other way round                        99 w, 4
     `rand() % 7' -- a SIGNED modulo                            99 w, 0  kept
       ... `rand() & 7'                                         89 w, 14
   The chain direction is the readable one: gcc emits the INNER lvalue first,
   so this spelling gives +0x1E then +0x1C, which is the listing's order, and
   one `div' feeds both stores. The same fact is 4 rows on this unit's
   func_s30_8002523C and 2 on func_s30_80025098, so it is priced three times in
   this file and holds in direction every time. */
UnkNode *func_s30_80024F0C(u8 *p, UnkVec *b)
{
    UnkNode *n;
    u8 *q;
    u8 *m;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s30_80024C48;
        q = (u8 *)n + 0x20;
        *(u8 **)q = p;
        *(s16 *)(q + 0xE) = 0;
        *(s16 *)(q + 0x10) = 0;
        *(s16 *)(q + 0x12) = rand() % 7;
        *(s16 *)(q + 0x14) = *(u16 *)(p + 0x14);
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        *(s16 *)(m + 0x12) = 0x7DCF;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x20;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8003DB94(m, D_80025AF0, 0);
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1C) = *(s16 *)(m + 0x1E) = 0x1400 / *(s16 *)(q + 0x14);
        func_8004491C(n, (u32)func_80045C34);
        *n->unk_08 = *b;
        return n;
    }
    return NULL;
}

/* The step of the node func_s30_8002523C spawns: bump the actor's +0xC and the
   payload's +0x10 frame counter, place the renderable's three colour bytes from
   the frame counter's residue modulo 7 the same way func_s30_80024C48 does,
   spin its +0x1A by 0x300, set its +0x1C/+0x1E scale from 0x1400 / +0x14 times
   the frame counter, integrate the mover's three velocity words and retire the
   actor once the counter passes +0x14.
   105 words, 0 differing, first compile.

   DONORS, both already at 0 differing in csrc/ovl_dng_s28.c: func_s28_800250E8
   for the modulo-7 colour chain and func_s28_8002553C for the head bumps, the
   velocity integration and the retire gate.

   TWO PRICED FACTS, both reproducing a donor's direction:
     `s16 k' for the modulo-7 residue                        105 w,  0  kept
       ... `s32 k'                                           106 w, 58
     the tail gate `p+0x10 > p+0x14'                         105 w,  0  kept
       ... `p+0x14 < p+0x10'                                 105 w,  2
   The width is `shorten_binary_op': with `s16' operands gcc narrows
   TRUNC_MOD_EXPR to HImode and `(h % 2) * 128' collapses to one sll16/sra9
   pair. The gate's 2 rows are the LOAD ORDER, not the compare -- both
   spellings emit `slt $v0, [0x14], [0x10]' and only the kept one loads +0x10
   first, exactly as func_s28_8002553C records it.
     the scale chain `+0x1C = +0x1E = ...'                   105 w,  0  kept
       ... the chain the other way round                     105 w,  2 */
void func_s30_80025098(u8 *p, UnkVec *b, u8 *c)
{
    s16 k;
    s16 h;

    *(s16 *)(*(u8 **)p + 0xC) = *(u16 *)(*(u8 **)p + 0xC) + 1;
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    k = *(s16 *)(p + 0x10) % 7 + 1;
    func_800478B8(c);
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x300;
    c[0xC] = (k / 4) * 128;
    h = k / 2;
    c[0xD] = (h % 2) * 128;
    c[0xE] = (k % 2) * 128;
    *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) =
        0x1400 / *(s16 *)(p + 0x14) * *(s16 *)(p + 0x10);
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    if (*(s16 *)(p + 0x10) > *(s16 *)(p + 0x14))
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawns the 0x212 node that runs func_s30_80025098 as its step: the caller's
   record at the payload's +0x0, a zeroed +0x10 counter, the caller's +0x14
   budget copied to +0x14, and a renderable dressed from D_80025AF0 at white
   0x80 with sprite 0x7DCF, the +0x14 0xC and 0x100 bits, the +0x10 0x20 bit, a
   random rotation and 0x1400 / +0x14 scale.
   85 words, 0 differing, first compile.

   This is func_s30_80024F0C minus the +0xE clear and the random 0..6 at +0x12;
   dngtwin ranks the landed func_s28_80025614 at 0.861 for it and that body is
   the donor for both.

   ONE PRICED FACT:
     the scale as the CHAIN `+0x1C = +0x1E = 0x1400 / ...'    85 w,  0  kept
       ... the chain the other way round                      85 w,  4
   The divisor is read off the PAYLOAD (`q + 0x14') and not off the caller
   (`p + 0x14'); the two are equal in value at this point and the listing's
   `lh $v1, 0x14($s2)' names $s2, the payload cursor. */
UnkNode *func_s30_8002523C(u8 *p, UnkVec *b)
{
    UnkNode *n;
    u8 *q;
    u8 *m;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s30_80025098;
        q = (u8 *)n + 0x20;
        *(u8 **)q = p;
        *(s16 *)(q + 0x10) = 0;
        *(s16 *)(q + 0x14) = *(u16 *)(p + 0x14);
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        *(s16 *)(m + 0x12) = 0x7DCF;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x20;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8003DB94(m, D_80025AF0, 0);
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1C) = *(s16 *)(m + 0x1E) = 0x1400 / *(s16 *)(q + 0x14);
        func_8004491C(n, (u32)func_80045C34);
        *n->unk_08 = *b;
        return n;
    }
    return NULL;
}

/* This bank's root step for the lock-on flyer: bump the +0x12 frame counter,
   then dispatch on the +0xA state. State 0 seeds the heading out of the actor's
   +0x2A and whitens the renderable; state 1 re-reads the slot through
   func_8003DE58, publishes the mover's position, and then AIMS -- at the
   locked-on target's record if the actor has one at +0x60, otherwise by walking
   up to eight cells along the heading with func_800A44E0 and func_800BCB04 and
   aiming at the last cell that admitted it. Either way the per-axis velocity is
   the span over a /16 step count floored at 1, it hands off to
   func_s30_80024F0C or func_s30_8002523C, and states 2 and 6 integrate that
   velocity until the step count runs out. States 3, 4 and 5 are the timed
   teardown: state 3 spawns a func_s30_80024640 trail node, state 4 calls this
   unit's own func_s30_80024024, and state 5 clears D_8008346C and retires the
   actor.
   448 words, 0 differing, FIRST COMPILE.

   OWNS jtbl_s30_80024008: 7 words, 7 cases, NO pad, dispatch bound
   `sltiu $v0, $v1, 0x7' and no minval subtraction, so the case set is 0..6 and
   all 7 entries are real arms. Entries in order: 0 .Ls30_80025400,
   1 .Ls30_8002542C, 2 .Ls30_8002591C, 3 .Ls30_80025974, 4 .Ls30_80025998,
   5 .Ls30_800259D0, 6 .Ls30_80025A08. All 7 distinct and their addresses
   ascend, so the arms are written ascending 0..6; case 0 falls through into
   case 1. The range-check `beqz $v0, .Ls30_80025A58' lands on the function's
   COMMON TAIL, the `*(s16 *)(p + 0xC) = 0' after the switch, which is not a
   table entry -- so there is NO default label to write, and an explicit
   `default: break;' measures 448 / 0, i.e. not measurable.

   DONOR: the landed func_s28_80025738 in csrc/ovl_dng_s28.c, dngtwin 0.943 at
   449 words, and its frame is byte-for-byte this one's -- `addiu $sp, -0x60'
   with s0..s7/fp/ra and the same 40-byte local area in the same order
   (UnkVec vv at sp+0x10, u16 out[4] at sp+0x28, the spilled s16 oy at
   sp+0x30). Every budget fact therefore transfers, and all eleven of the
   twin's statements carried.

   WHAT THIS COPY CHANGES, all off this listing: the frame counter is +0x12 and
   not +0x10, the heading is +0x10 and not +0xE, the step count is +0x14 and not
   +0x12, the tail clear and state 5's gate are +0xC and not +0x14; state 3
   gains a `func_s30_80024640(p, b)' spawn and its threshold drops from 0x10 to
   0xC; state 4's threshold rises from 0x30 to 0x31; the lock-on arm DROPS the
   twin's `*(s32 *)(p + 0x18) = (s32)*(u8 **)(s + 0x60)' statement; and the two
   hand-off callees are this unit's.

   THE DEAD `lui $v0, (0x80070000 >> 16)' at 80025658 IS NOT SOURCE. 0x8007 is
   %hi(D_8006CCD8); it is the walk loop's hoisted table high left behind once
   the loop body's uses were rematerialised as full lui/addiu pairs, and $v0 is
   overwritten by the loop's first instruction. func_s28_80025738 and
   func_s13_80024C88 record the same artefact.

   FACTS RE-PRICED ON THIS BODY FROM THE 0 STATE (aligned differences; words
   given where the spelling changes them). Five of the twin's figures reproduce
   to the digit, which is why this landed first compile:

   | spelling                                                 | words | rows |
   |----------------------------------------------------------|-------|------|
   | `s16 xx' / `s16 yy' for the walk cursor            KEPT  |  448  |   0  |
   |   ... both `s32'                                         |  438  |  155 |
   | ONE `UnkVec *v' shared by the lock-on and walk arms KEPT |  448  |   0  |
   |   ... a separate local in the lock-on arm                |  448  |  10  |
   | `u16 z' for w+0xA across state 1's flag gate       KEPT  |  448  |   0  |
   |   ... naming `*(u16 *)(w + 0xA)' in both arms            |  450  |   7  |
   | state 1's `p+0x12 = 0' ONCE after the if/else      KEPT  |  448  |   0  |
   |   ... duplicated into both arms                          |  447  |   6  |
   | case 6's `p+0xA = 5' BEFORE `p+0x12 = 0'           KEPT  |  448  |   0  |
   |   ... the other order                                    |  453  |  20  |
   | `(cx << 6) & 0xFFFFFFC0' into func_800A44E0        KEPT  |  448  |   0  |
   |   ... `& 0xFFC0' against the same u16 prototype          |  451  |   7  |
   | `ox * 64' / `oy * 64' into the aim point           KEPT  |  448  |   0  |
   |   ... `(ox << 6)' / `(oy << 6)'                          |  446  |  27  |
   | func_s30_80024024 through a THREE-argument alias   KEPT  |  448  |   0  |
   |   ... the landed two-argument name at that site          |  448  |   1  |

   WHERE THE TWIN'S MAGNITUDES DO AND DO NOT TRANSFER. `z', the duplicated
   `p+0x12 = 0', case 6's store order and the three-argument alias reproduce
   func_s28_80025738's 7, 6, 20 and 1 EXACTLY. The two walk-block arithmetic
   facts hold in SIGN but not in size: `& 0xFFFFFFC0' is 7 rows here against
   s28's 61, and `* 64' is 27 against s28's 79 -- s28 explains its own inflation
   by its 32-bit velocity arithmetic putting the aim block downstream of the
   mask, and this copy is nearer s25's and s13's 7 and 29. Treat a donor's
   magnitude as a lead and its sign as the fact. The shared-`v' fact is 10 rows
   here against s28's 5, and the twin's `do { v = &vv; } while (0);' barrier is
   NOT in this body either: with one shared `v' it is worth nothing.

   MEASURED AND FREE, so none of these is why this lands: an explicit
   `default: break;' (448 / 0 -- NOT MEASURABLE, the tools cannot see a label
   that is not in the table); and `i = 1' left in the walk arm's `for'
   initialiser rather than hoisted ahead of the three vv stores (448 / 0 here,
   where func_s28_80025738 prices the hoist at 2 -- an INVERSION to free, and
   the brief's reference-count-and-statement-order class is exactly where
   inversions live). */
void func_s30_80025390(u8 *p, UnkVec *b, u8 *c)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *q4;
    UnkVec *v;
    u16 z;
    s32 i;
    s32 h;
    s32 cx;
    s32 cy;
    s16 xx;
    s16 yy;
    s16 ox;
    UnkVec vv;
    u16 out[4];
    s16 oy;

    s = *(u8 **)(p + 0x0);
    *(s16 *)(p + 0x12) = *(u16 *)(p + 0x12) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s16 *)(p + 0x12) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0x10) = (*(u16 *)(s + 0x2A) >> 9) & 7;
        *(s32 *)(c + 0xC) = 0x808080;
        /* fall through */
    case 1:
        o = s - 0x20;
        if (func_8003DE58(*(u8 **)(*(u8 **)(o + 0xC) + 0x8), (UnkSlot *)*(u8 **)(o + 0xC),
                          (s16 *)out, 0) == 0 &&
            (*(u16 *)(*(u8 **)(o + 0xC) + 0x14) & 0x8000) == 0)
        {
            break;
        }
        w = *(u8 **)(o + 0x8);
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(w + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(w + 0x6);
        z = *(u16 *)(w + 0xA);
        *(s16 *)((u8 *)b + 0xA) = z;
        if ((*(u16 *)(*(u8 **)(o + 0xC) + 0x14) & 0x8000) == 0)
        {
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + out[0];
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + out[1];
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + out[2];
        }
        else
        {
            *(s16 *)((u8 *)b + 0xA) = z - 0x40;
        }
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        if (*(u8 **)(s + 0x60) != NULL)
        {
            v = (UnkVec *)*(u8 **)(*(u8 **)(s + 0x60) - 0x18);
            out[0] = abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)b + 0x6));
            out[2] = abs(*(s16 *)(*(u8 **)(s + 0x60) + 0x88) - *(s16 *)((u8 *)b + 0xA));
            *(s16 *)(p + 0x14) = out[0];
            for (i = 1; i < 3; i++)
            {
                if ((s16)out[i] > *(s16 *)(p + 0x14))
                {
                    *(s16 *)(p + 0x14) = out[i];
                }
            }
            *(s16 *)(p + 0x14) = (s16) * (u16 *)(p + 0x14) >> 4;
            if (*(s16 *)(p + 0x14) == 0)
            {
                *(s16 *)(p + 0x14) = 1;
            }
            b->unk_0C = (v->unk_00 - b->unk_00) / *(s16 *)(p + 0x14);
            b->unk_10 = (v->unk_04 - b->unk_04) / *(s16 *)(p + 0x14);
            b->unk_14 = ((*(s16 *)(*(u8 **)(s + 0x60) + 0x88) << 16) - b->unk_08) /
                        *(s16 *)(p + 0x14);
            func_s30_80024F0C(p, b);
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        else
        {
            q4 = *(u8 **)(o + 0xC);
            xx = q4[0x24];
            yy = q4[0x25];
            ox = xx;
            oy = yy;
            for (i = 0; i < 8; i++)
            {
                cx = xx;
                cy = yy;
                if ((s16)func_800A44E0((cx << 6) & 0xFFFFFFC0, (cy << 6) & 0xFFFFFFC0,
                                       *(s16 *)(s + 0x88),
                                       (s16)(*(s16 *)(p + 0x10) << 9)) != 0)
                {
                    break;
                }
                h = func_800BCB04(((cx + D_8006CCD8[*(s16 *)(p + 0x10)]) << 6) + 0x20 & 0xFFE0,
                                  ((cy + D_8006CCE8[*(s16 *)(p + 0x10)]) << 6) + 0x20 & 0xFFE0,
                                  (s16)(*(u16 *)(s + 0x88) - 0x20));
                if ((s16)h >= 0x201)
                {
                    break;
                }
                if ((s16)(h - *(u16 *)(s + 0x88)) < -0x3F)
                {
                    break;
                }
                xx = xx + (D_8006CCD8[*(s16 *)(p + 0x10)] & 0xFFFF);
                yy = yy + (D_8006CCE8[*(s16 *)(p + 0x10)] & 0xFFFF);
                oy = yy;
                ox = xx;
            }
            v = &vv;
            i = 1;
            *(s16 *)((u8 *)v + 0x2) = ox * 64 + (D_8006CCD8[*(s16 *)(p + 0x10)] + 1) * 32;
            *(s16 *)((u8 *)v + 0x6) = oy * 64 + (D_8006CCE8[*(s16 *)(p + 0x10)] + 1) * 32;
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)b + 0xA) + 0x20;
            out[0] = abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)b + 0x6));
            out[2] = abs(*(s16 *)((u8 *)v + 0xA) - *(s16 *)((u8 *)b + 0xA));
            *(s16 *)(p + 0x14) = out[0];
            for (; i < 3; i++)
            {
                if ((s16)out[i] > *(s16 *)(p + 0x14))
                {
                    *(s16 *)(p + 0x14) = out[i];
                }
            }
            *(s16 *)(p + 0x14) = (s16) * (u16 *)(p + 0x14) >> 4;
            if (*(s16 *)(p + 0x14) == 0)
            {
                *(s16 *)(p + 0x14) = 1;
            }
            b->unk_0C = (v->unk_00 - b->unk_00) / *(s16 *)(p + 0x14);
            b->unk_10 = (v->unk_04 - b->unk_04) / *(s16 *)(p + 0x14);
            b->unk_14 = (v->unk_08 - b->unk_08) / *(s16 *)(p + 0x14);
            func_s30_8002523C(p, b);
            *(s16 *)(p + 0xA) = 6;
        }
        *(s16 *)(p + 0x12) = 0;
        break;
    case 2:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x12) >= *(s16 *)(p + 0x14))
        {
            func_800A56E0(0x300);
            *(s16 *)(p + 0x12) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x12) >= 0xC)
        {
            func_s30_80024640(p, b);
            *(s16 *)(p + 0x12) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        if (*(s16 *)(p + 0x12) >= 0x31)
        {
            func_s30_80024024_3(*(u8 **)(s + 0x60), p[0x9], s);
            *(s16 *)(p + 0x12) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if (*(s16 *)(p + 0xC) == 0)
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    case 6:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x12) >= *(s16 *)(p + 0x14))
        {
            *(s16 *)(p + 0xA) = 5;
            *(s16 *)(p + 0x12) = 0;
        }
        break;
    }
    *(s16 *)(p + 0xC) = 0;
}
