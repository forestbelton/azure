#include "common.h"

void func_800478B8(u8 *);
void func_s08_80024688(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_s08_800247D8(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u8 D_80045340[];
s32 rand(void);
UnkNode *func_8003FD64(s32, void *);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
s32 func_800A05A4(u8 *, s32, s32, s16, s32);
s32 func_800A3820(s16);
s32 func_800A56E0(s32);
s32 func_800BCB04(s32, s32, s16);
extern s32 D_8008346C;
extern u8 D_80083498[];
extern u8 D_800DE990[];
extern u8 D_800DEA68[];
extern u8 D_800DECF8[];

/* This bank's six-state cutscene director and the relink of
   csrc/ovl_dng_s09.c func_s09_80024020 (itself the relink of
   func_s12_80024020): a JUMP TABLE on *(s16 *)(p + 0xA) over cases 0..5
   (jtbl_s08_80024008, 6 words = 6 cases + 0 pad, dispatch bound 6, 6 distinct
   targets). THIS FUNCTION OWNS THAT TABLE. The arms are written ascending and
   a standalone `tools/cdkgcc -O2 -G0 -S' emits the six entries in ascending
   text order, which is the listing's ascending entry-ADDRESS order; the range
   check's `beqz' lands on the epilogue, which is not one of the six entries,
   so no `default:' is written.

   Before the switch, every state but 0 and 5 spawns SEVENTEEN 0x312 smoke
   nodes running func_s08_80024688, jittered +-0xFF<<13 / <<11 around the
   caller's vector and dressed from D_800DEA68. State 0 registers the party
   record through func_800A05A4, publishes its projected position into b,
   seeds the +0x50 timer at 8 and divides the three velocity words by it;
   state 1 advances b and reloads the timer at 0xC; state 2 either retires to
   state 5 or reloads at 0x14; state 3 spawns, on every third timer tick, the
   single 0x312 node that gets func_s08_800247D8 as its step and
   D_800DE990 as its dress, then reloads at 0xD; state 4 runs the timer down
   and calls func_8009CE1C; state 5 tears the actor down. 410 words,
   0 differing. One-member family (dngfam).

   DONOR: func_s09_80024020, named by `dngcallee' at 13 of 15 symbols with a
   clean gap to the 10/15 tier and by `dngtwin' at 0.804, and the two tools
   agreed on the FILE. Its frame is identical -- -0x58 with the same ten saves
   in the same order, `w' spilled to 0x28($sp) in both -- so its budget facts
   carry, and all four of its recorded facts held here unmeasured. The 41-word
   gap is accounted for entirely by absent statements: this relink has no
   0x8000/func_8009CE1C escape in case 0, and its case 3 spawns ONE node
   rather than looping twenty-one. func_80069EF8 is rand out of
   splat/common_syms.txt.

   THREE FACTS, all priced from this finished body:

   * CASE 3's JITTER SUM GOES ON THE LEFT OF THE `+'.
     `((((rand() & 0x1FF) + (rand() & 0x1FF) - 0x1FE) << 13) >> 1) + b->unk_00'
     is 0 and `b->unk_00 + (...)' is 410 words / 4 aligned differences, all
     four rows register-only: the `addu's output ties to the dying
     callee-saved accumulator only when that accumulator is operand 0, so the
     sum lands in $s0 where the other order leaves it in $v0 and the `sw'
     names the wrong register.
     AND THE FIRST SPAWN LOOP WANTS THE OPPOSITE ORDER, ALSO MEASURED:
     `b->unk_00 + (((rand() & 0x1FF) - 0xFF) << 13)' is 0 there and the
     flipped form is 2 aligned differences, the same two-row shape
     (`addu'/`sw' naming $v0 where the target names $v1). I first wrote this
     down as "free at the first loop" and the measurement contradicted me, so
     BOTH sites are load-bearing and the target simply wrote them in opposite
     orders. The order is readable off the listing -- the `addu's rs is the
     operand the source wrote first -- and must be priced per SITE.
     Preconditions, from `docs/matching-levers.md': the order survives only
     because both operands are their own insns at combine time (the lever at
     "A TEMPORARY FOR THE ADDED OPERAND DECIDES `addu's OPERAND ORDER" -- its
     third clause), the load here being forced into its own insn by the
     separate `lw' for the store's base.
   * CASE 3's GATE DIVIDES A SIGNED HALFWORD. `*(s16 *)(p + 0x50) % 3 == 0'
     is 0; `*(u16 *)(p + 0x50) % 3 == 0' is 407 words / 16 aligned differences,
     three words short, because the unsigned magic multiply is a shorter
     sequence than the 0x55555556 signed one the listing has. The listing's
     `sra $v1, $v1, 31' is the tell that the dividend is sign-extended.
   * MEASURED FREE, so NOT the reason this lands: case 5's tail written
     `else { if (timer > 0) break; ... }' instead of `else if (timer <= 0)'
     is 410 words / 0 differing. */
void func_s08_80024020(u8 *p, UnkVec *b, u8 *c)
{
    u8 *s;
    u8 *o;
    u8 *w;
    UnkNode *n;
    u8 *q;
    u8 *e;
    s32 i;
    s32 r;
    s16 out[4];

    s = *(u8 **)(p + 0x0);
    o = *(u8 **)(s - 0x14);
    w = s - 0x20;
    if (*(s16 *)(p + 0xA) != 0 && *(s16 *)(p + 0xA) < 5)
    {
        for (i = 0x10; i >= 0; i--)
        {
            n = func_8003FD64(0x312, D_80083498);
            if (n != NULL)
            {
                e = (u8 *)n + 0x20;
                q = (u8 *)n->unk_0C;
                n->unk_10 = (u32)func_s08_80024688;
                *(s32 *)((u8 *)n->unk_08 + 0x0) = b->unk_00 + (((rand() & 0x1FF) - 0xFF) << 13);
                *(s32 *)((u8 *)n->unk_08 + 0x4) = b->unk_04 + (((rand() & 0x1FF) - 0xFF) << 13);
                *(s32 *)((u8 *)n->unk_08 + 0x8) = b->unk_08 + (((rand() & 0x1FF) - 0xFF) << 11);
                *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x1000;
                *(s16 *)(q + 0x10) = 0x60;
                *(u8 **)(q + 0x0) = D_800DEA68;
                *(u16 *)(q + 0x14) |= 0xC;
                *(s32 *)(q + 0x8) = *(s32 *)(D_800DEA68 + 0x4);
                *(u8 *)(q + 0x4) = 0;
                *(u8 *)(q + 0x5) = 0;
                *(s32 *)(q + 0xC) = 0xF08080;
                *(u8 **)((u8 *)n + 0x20) = p;
                *(s16 *)(e + 0x48) = rand() & 3;
                *(s16 *)(e + 0x4A) = 0x18;
                *(s16 *)(e + 0x4C) = 0;
            }
        }
    }
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        r = func_800A05A4(s, *(u8 *)(o + 0x24), *(u8 *)(o + 0x25), *(s16 *)(s + 0x2A),
                          (s16)func_800A3820(8));
        *(s32 *)(s + 0x60) = r;
        if (r == 0)
        {
            *(u8 *)(s + 0x72) = *(u8 *)(o + 0x24);
            *(u8 *)(s + 0x73) = *(u8 *)(o + 0x25);
        }
        else
        {
            q = *(u8 **)((u8 *)r - 0x14);
            *(u8 *)(s + 0x72) = *(u8 *)(q + 0x24);
            *(u8 *)(s + 0x73) = *(u8 *)(q + 0x25);
        }
        if (func_8003DE58(*(u8 **)(*(u8 **)(w + 0xC) + 0x8), (UnkSlot *)*(u8 **)(w + 0xC), out, 0) ==
            0)
        {
            out[0] = out[1] = out[2] = 0;
        }
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(*(u8 **)(w + 0x8) + 0x2) + out[0];
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(*(u8 **)(w + 0x8) + 0x6) + out[1];
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)(*(u8 **)(w + 0x8) + 0xA) + out[2];
        *(s16 *)(p + 0x50) = 8;
        *(s16 *)((u8 *)b + 0xE) = (*(s8 *)(s + 0x72) * 0x40) - (*(u16 *)((u8 *)b + 0x2) - 0x20);
        b->unk_0C /= *(s16 *)(p + 0x50);
        *(s16 *)((u8 *)b + 0x12) = (*(s8 *)(s + 0x73) * 0x40) - (*(u16 *)((u8 *)b + 0x6) - 0x20);
        b->unk_10 /= *(s16 *)(p + 0x50);
        *(s16 *)((u8 *)b + 0x16) =
            func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                          *(u16 *)(*(u8 **)(w + 0x8) + 0xA) - 0x30) -
            (*(u16 *)((u8 *)b + 0xA) + 0xB0);
        b->unk_14 /= *(s16 *)(p + 0x50);
        func_800A56E0(0x300);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 1:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
        if (*(s16 *)(p + 0x50) <= 0)
        {
            *(s16 *)(p + 0x50) = 0xC;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 2:
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
        if (*(s16 *)(p + 0x50) <= 0)
        {
            if (*(s32 *)(s + 0x60) == 0)
            {
                *(s16 *)(p + 0x50) = 8;
                *(s16 *)(p + 0xA) = 5;
            }
            else
            {
                *(s16 *)(p + 0x50) = 0x14;
                *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            }
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x50) % 3 == 0)
        {
            n = func_8003FD64(0x312, D_80083498);
            if (n != NULL)
            {
                n->unk_10 = (u32)func_s08_800247D8;
                func_8004491C(n, (u32)D_80045340);
                q = (u8 *)n->unk_0C;
                *(s32 *)((u8 *)n->unk_08 + 0x0) =
                    ((((rand() & 0x1FF) + (rand() & 0x1FF) - 0x1FE) << 13) >> 1) + b->unk_00;
                *(s32 *)((u8 *)n->unk_08 + 0x4) =
                    ((((rand() & 0x1FF) + (rand() & 0x1FF) - 0x1FE) << 13) >> 1) + b->unk_04;
                *(s32 *)((u8 *)n->unk_08 + 0x8) = b->unk_08 + 0x100000;
                *(s32 *)((u8 *)n->unk_08 + 0x14) = 0x40000;
                *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0xA00;
                *(s16 *)(q + 0x10) = 0x20;
                *(u8 **)(q + 0x0) = D_800DE990;
                *(u16 *)(q + 0x14) |= 0xE;
                *(s32 *)(q + 0x8) = *(s32 *)(D_800DE990 + 0x4);
                *(u8 *)(q + 0x4) = 0;
                *(u8 *)(q + 0x5) = 0;
                *(s32 *)(q + 0xC) = 0;
                e = (u8 *)n + 0x20;
                *(u8 **)((u8 *)n + 0x20) = p;
                *(s16 *)(e + 0x48) = 8;
                *(s16 *)(e + 0x4C) = 0;
            }
        }
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
        if (*(s16 *)(p + 0x50) <= 0)
        {
            *(s16 *)(p + 0x50) = 0xD;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
        if (*(s16 *)(p + 0x50) <= 0)
        {
            if (*(s32 *)(s + 0x60) != 0)
            {
                func_8009CE1C(*(u8 **)(s + 0x60), 0xA, *(u8 *)(p + 0x9), 2, *(s16 *)(s + 0x2A),
                              (u32)s, 2);
            }
            *(s16 *)(p + 0x50) = 0x10;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
        if ((*(s16 *)(p + 0x52) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x52) = *(u16 *)(p + 0x52) & 0x7FFF;
        }
        else if (*(s16 *)(p + 0x50) <= 0)
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* The 0x312 smoke node's per-frame step, installed as the +0x10 step by
   func_s08_80024020's pre-switch spawn loop and by func_s08_800247D8's case-1
   loop, so the four-argument signature is fixed by those sites taking its
   address and `d' is unused. Sets bit 0x8000 in the +0x52 halfword of the
   record at the actor's +0x0, then runs a two-case compare chain on +0x4C:
   state 0 counts the +0x48 timer down and re-registers the renderable through
   func_8004491C when it expires, state 1 advances the caller's two horizontal
   position words by their velocity halves, hands the renderable to
   func_800478B8, clears its +0x4/+0x5 once +0x14 reports 0x6000, and fades
   its +0xC/+0xD/+0xE colour bytes by the actor's own +0x4A step until the red
   byte would pass it, at which point it clears the colour word and retires.
   84 words, 0 differing, FIRST COMPILE. One-member family (dngfam).

   DONOR: csrc/ovl_dng_s09.c func_s09_80024C84 (91 words), which `dngcallee'
   named at 4 of 4 symbols and whose frame is identical (-0x20, s1/ra/s0 in
   that store order). csrc/ovl_dng_s12.c func_s12_80024850 is the same body.
   The 7-word gap is absent statements, not absent references: this relink has
   no +0x14 velocity decay and advances only TWO position words, and those two
   adds sit INSIDE case 1 rather than before the switch (the listing puts them
   after .Ls08_80024714, which is case 1's entry).

   TWO FACTS, both priced from this finished body:

   * THE DONOR'S COLOUR-FADE CHAINING FACT IS **FREE** HERE, AND THAT IS AN
     INVERSION OF ITS STATED PRICE. func_s09_80024C84 prices a three-way chain
     at 0 against three statements at 92 words / 3 rows, and the s05 relink
     reproduced that "to the digit". This body's fade is a TWO-way chain plus
     a separate +0xE statement, and splitting it
     (`c[0xD] -= k; c[0xC] = c[0xD];') is 84 words / 0 aligned differences.
     The donor's mechanism -- a `move' the chained form does not need -- has
     nothing to bite on when only two bytes share the value, so this is not a
     counter-example to the lever, it is outside its precondition. The chained
     form is kept because it is the family idiom, not because it is measured.
   * THE COMPARE PUTS THE COLOUR BYTE ON THE LEFT AND THE STEP IS READ TWICE,
     ONCE SIGNED AND ONCE UNSIGNED: the gate is
     `*(u8 *)(c + 0xC) <= *(s16 *)(p + 0x4A)' (the listing's `lh' at 0x4A)
     while the two subtractions read `*(u8 *)(p + 0x4A)' (its `lbu'), and the
     second read survives because the `sb' between them clears
     `last_mem_set'. Carried from the donor at its recorded price and NOT
     re-measured here, so treat it as inherited. */
void func_s08_80024688(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) - 1;
        if (*(s16 *)(p + 0x48) <= 0)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 1:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        func_800478B8(c);
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(u8 *)(c + 0x4) = 0;
            *(u8 *)(c + 0x5) = 0;
        }
        if (*(u8 *)(c + 0xC) <= *(s16 *)(p + 0x4A))
        {
            *(s32 *)(c + 0xC) = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            *(u8 *)(c + 0xC) = *(u8 *)(c + 0xD) -= *(u8 *)(p + 0x4A);
            *(u8 *)(c + 0xE) -= *(u8 *)(p + 0x4A) * 2;
        }
        break;
    }
}

/* The 0x312 burst node's per-frame step, installed as the +0x10 step by
   func_s08_80024020's case 3, so the four-argument signature is fixed by that
   site taking its address and `d' is unused. Sets the same +0x52 flag as
   func_s08_80024688, counts the +0x48 timer down UNCONDITIONALLY before the
   switch, then runs a three-case balanced compare chain on +0x4C: state 0
   hands the renderable to func_800478B8 and brightens its colour bytes by
   0x20 while the red byte is still at or below 0x80, reloading the timer at
   0x10 when it expires; state 1 damps the caller's +0x14 velocity word
   towards zero by 0x8000, advances +0x8 by it, and on expiry spawns
   TWENTY-ONE 0x312 nodes running func_s08_80024688, dressed from D_800DECF8
   and jittered in +0x8/+0xC/+0x10; state 2 fades the colour bytes by 0x10
   until the red byte would pass it and then retires the actor. 172 words,
   0 differing. One-member family (dngfam).

   DONORS: the spawn loop and the dress block are func_s09_80024020's case-3
   loop (the 0x312/D_800DECF8 one), which is how the +0x8/+0xC/+0x10 offsets
   and the `D_800DECF8' + `*(s32 *)(D_800DECF8 + 0x4)' naming came for free;
   the retire/fade tail is func_s09_80024C84's. `dngtwin' found NO carved
   sibling at 0.70 for this body and `dngcallee' put it at 5 of 6 symbols
   across three units, so the route here was the IN-UNIT one: this body is
   named by func_s08_80024020's case 3 and it names func_s08_80024688, and
   writing all three in one file is what interpreted the offsets.

   THREE FACTS, all priced from this finished body:

   * CASE 1's LOOP COUNTER AND CASE 2's FADE STEP ARE ONE LOCAL, AND THAT IS
     THE WHOLE 23-ROW RESIDUE. With two locals (`s32 i' for the loop, `s32 k'
     for the step) the body is 172 words / 23 aligned differences -- a clean
     whole-body $s2/$s3 EXCHANGE, `p' taking $s2 where the target has $s3 and
     the counter taking $s3 where the target has $s2, with the two prologue
     save slots swapped to match. One local is 0. This is the "one local too
     many" rung of the register ladder, not an allocation-priority fault:
     PERMUTING THE TWO DECLARATIONS (`k' before `i') measures the IDENTICAL 23
     rows, which rules priority out before any `-da'.
   * CASE 2's STEP MUST BE THAT LOCAL AND NOT THE LITERAL 0x10, and the reason
     is `fold', not allocation. Written out as
     `if (*(u8 *)(c + 0xC) <= 0x10) ... -= 0x10' it is 172 words / 23 rows of
     which two are SHAPE: fold canonicalises a comparison against an
     INTEGER_CST into `< 0x11' and cc1psx emits `sltiu $v0, $v0, 0x11' with an
     inverted `beqz', where the target has a register-register
     `slt $v0, $s2, $v0' and `bnez'. A non-constant operand 0 has no immediate
     form for `slt's rs field, so the local is the only spelling that keeps
     it; cse then still propagates the known 0x10 into the subtraction's
     immediate, which is why the fade reads `addiu $v0, $v0, -0x10' anyway.
   * THE OWNER STORE GOES LAST IN THE LOOP BODY.
     `*(u8 **)((u8 *)n + 0x20) = *(u8 **)p;' after the +0x48/+0x4A/+0x4C
     stores is 0; moved to the donor's position (before them) it is 171 words
     / 12 aligned differences -- ONE WORD SHORT, because its `lw $v0, 0x0($s3)'
     is then free to be scheduled into the last `jal rand' delay slot the
     target leaves filled by the `rand() & 3' store instead. Note the store
     reads *(u8 **)p, the record pointer, where the donor stores `p' itself. */
void func_s08_800247D8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;
    u8 *q;
    u8 *e;
    s32 i;

    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) - 1;
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        func_800478B8(c);
        if (*(u8 *)(c + 0xC) <= 0x80)
        {
            *(u8 *)(c + 0xC) = *(u8 *)(c + 0xD) = *(u8 *)(c + 0xE) += 0x20;
        }
        if (*(s16 *)(p + 0x48) <= 0)
        {
            *(s16 *)(p + 0x48) = 0x10;
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 1:
        b->unk_14 += 0x8000;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x48) <= 0)
        {
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
            for (i = 0x14; i >= 0; i--)
            {
                n = func_8003FD64(0x312, D_80083498);
                if (n != NULL)
                {
                    e = (u8 *)n + 0x20;
                    q = (u8 *)n->unk_0C;
                    n->unk_10 = (u32)func_s08_80024688;
                    *(s32 *)((u8 *)n->unk_08 + 0x0) = b->unk_00;
                    *(s32 *)((u8 *)n->unk_08 + 0x4) = b->unk_04;
                    *(s32 *)((u8 *)n->unk_08 + 0x8) = b->unk_08 + ((rand() & 0x1F) << 16);
                    *(s32 *)((u8 *)n->unk_08 + 0xC) = ((rand() & 0xFFF) - 0x7FF) << 8;
                    *(s32 *)((u8 *)n->unk_08 + 0x10) = ((rand() & 0xFFF) - 0x7FF) << 8;
                    *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x1000;
                    *(s16 *)(q + 0x10) = 0x20;
                    *(u8 **)(q + 0x0) = D_800DECF8;
                    *(u16 *)(q + 0x14) |= 0xC;
                    *(s32 *)(q + 0x8) = *(s32 *)(D_800DECF8 + 0x4);
                    *(u8 *)(q + 0x4) = 0;
                    *(u8 *)(q + 0x5) = 0;
                    *(s32 *)(q + 0xC) = 0x808080;
                    *(s16 *)(e + 0x48) = rand() & 3;
                    *(s16 *)(e + 0x4A) = 0xC;
                    *(s16 *)(e + 0x4C) = 0;
                    *(u8 **)((u8 *)n + 0x20) = *(u8 **)p;
                }
            }
        }
        break;
    case 2:
        i = 0x10;
        if (*(u8 *)(c + 0xC) <= i)
        {
            *(s32 *)(c + 0xC) = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            *(u8 *)(c + 0xC) = *(u8 *)(c + 0xD) = *(u8 *)(c + 0xE) -= i;
        }
        break;
    }
}
