#include "common.h"

s32 func_8004491C(UnkNode *, u32);
void func_800478B8(u8 *);
s32 func_8009D218_2(u8 *, s32) __asm__("func_8009D218");
s32 func_800A6870(s16);
void func_800AD4D0(u8 *);
s32 func_800AD568(u8 *);
UnkNode *func_800B4C7C(u16, u8 *, s16, s16);
void func_8003DB94(void *, void *, s32);
UnkNode *func_8003FC64(s32);
void func_s25_80024028(u8 *p, u8 k);
void func_s25_800240C4(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_s25_80024230(u8 *, UnkVec *, u8 *, u8 *);
extern u8 D_80025238[];
extern u8 D_80045340[];
extern u8 D_800DEAE0[];
extern u8 D_800DDC40[];
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s32 D_8008346C;
s32 func_800A44E0(u16, u16, s16, u32);
s32 func_800BCB04(s32, s32, s16);
s32 func_800A56E0(s32);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
void func_s25_80024028_3(u8 *, u8, u8 *) __asm__("func_s25_80024028");
s32 func_s25_800244A0(u8 *, UnkVec *, s16, s16);
s32 func_s25_800246D0(u8 *, UnkVec *);

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
       0 differing. The two-variable spelling is FREE on this copy. */
void func_s25_80024028(u8 *p, u8 k)
{
    s16 t;
    s16 u;

    if (func_8009D218_2(p, 1) == 0)
    {
        t = func_800A6870(k) + 4;
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

/* The 0x7E07 sprite's per-frame step: bump the owning node's +0x14 frame
   counter, hand the renderable to func_800478B8, jitter its +0x1A by
   rand() % 0x1000, and re-dress it from the bank-local D_80025238 once its
   +0x14 reports 0x6000. 33 words, 0 differing, first compile.

   No carved sibling at 0.70 and xsibs finds nothing anywhere in this unit
   (its landed pool cannot see the 115 banked objects). Written from the
   listing; the shape is csrc/ovl_dng_s27.c's func_s27_8002417C head plus the
   rand() jitter, with the retire/publish tail replaced by the re-dress.

   func_80069EF8 IS `rand' -- splat/common_syms.txt names it and it needs no
   declaration. The `bgez'/`addiu 0xFFF'/`sra 12'/`sll 12'/`subu' run is a
   signed `% 0x1000', not a mask.

   Measured and FREE at 33 words / 0 differing, so it is NOT why this lands:
   hoisting `*(u8 **)(p + 0x0)' into a local instead of naming it twice (cse
   unifies the two loads either way -- the listing has one `lw 0x0($a0)'). */
void func_s25_800240C4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)(*(u8 **)(p + 0x0) + 0x14) = *(u16 *)(*(u8 **)(p + 0x0) + 0x14) + 1;
    func_800478B8(c);
    *(s16 *)(c + 0x1A) = rand() % 0x1000;
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        func_8003DB94(c, D_80025238, 0);
    }
}

/* Spawn the 0x212 node that carries one of this effect's white 0x7E07
   sprites: 0x800 scale both ways, the +0x14 0x100 bit, the bank-local
   D_80025238 as its dresser, D_80045340 through func_8004491C, and
   func_s25_800240C4 as its per-frame step, with the caller's record saved at
   +0x20 and the position copied off q. Returns the node, or NULL when the
   allocator refuses. 58 words, 0 differing, first compile.

   TWIN: the landed func_s27_800241F8 in csrc/ovl_dng_s27.c (0.983, same 58
   words, same frame `-0x28' and the same five saves in the same order).
   Deltas: D_80025238 for D_800DEC70, 0x7E07 for 0x7E09, 0x800 for 0x1000,
   and this unit's own step function.

   BOTH of the twin's stated facts HOLD, and both reproduce its prices:
     - the NULL exit is an `else' arm AFTER the whole gate, not an early
       return. Written as `if (n == NULL) { return NULL; }' ahead of the body
       it is 58 words / 38 differing -- the twin recorded 5 aligned and 38
       unaligned for the same spelling.
     - the second `n->unk_08' pointer is its OWN local, not a reuse of `m':
       reusing `m' is 58 words / 5 differing, which is the twin's figure to
       the digit.
   The twin's third fact -- the +0x20 store THIRD inside the gate, after the
   step pointer and the `m = n->unk_0C' load -- is carried unmeasured here;
   this listing's own order is the same and it landed first compile. */
UnkNode *func_s25_80024148(u8 *v, UnkVec *q)
{
    UnkNode *n;
    u8 *m;
    u8 *m2;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s25_800240C4;
        m = (u8 *)n->unk_0C;
        *(u8 **)((u8 *)n + 0x20) = v;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        func_8003DB94(m, D_80025238, 0);
        *(s16 *)(m + 0x1E) = 0x800;
        *(s16 *)(m + 0x1C) = 0x800;
        *(s16 *)(m + 0x12) = 0x7E07;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8004491C(n, (u32)D_80045340);
        m2 = (u8 *)n->unk_08;
        *(s16 *)(m2 + 0x2) = *(u16 *)((u8 *)q + 0x2);
        *(s16 *)(m2 + 0x6) = *(u16 *)((u8 *)q + 0x6);
        *(s16 *)(m2 + 0xA) = *(u16 *)((u8 *)q + 0xA);
    }
    else
    {
        return NULL;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s25", func_s25_80024230);

/* The same spawner for this effect's BLACK 0x1000-scale sprite: no sprite
   id at +0x12, `|= 0xC' on +0x14 instead of 0x100, the resident D_800DEAE0
   as its dresser and func_s25_80024230 as its step. 55 words, 0 differing,
   first compile, AND IT LANDED BECAUSE func_s25_80024148 WAS ALREADY THERE
   -- same body, three words shorter, no new declarations.

   dngtwin ranks func_s25_80024148 at 0.938 with `+3 vs yours' and the three
   missing words are exactly the two the sprite-id store costs
   (`addiu $v1, $zero, 0x7E07' + `sh') plus the colour constant
   (`addiu $v0, $zero, 0x80' becomes `$zero'): MISSING STATEMENTS, not
   missing references, which is the usual case for a shorter body.
   Its twin's two facts carry unchanged; see that comment for their prices. */
UnkNode *func_s25_80024264(u8 *v, UnkVec *q)
{
    UnkNode *n;
    u8 *m;
    u8 *m2;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s25_80024230;
        m = (u8 *)n->unk_0C;
        *(u8 **)((u8 *)n + 0x20) = v;
        m[0xE] = 0;
        m[0xD] = 0;
        m[0xC] = 0;
        func_8003DB94(m, D_800DEAE0, 0);
        *(s16 *)(m + 0x1E) = 0x1000;
        *(s16 *)(m + 0x1C) = 0x1000;
        *(u16 *)(m + 0x14) |= 0xC;
        func_8004491C(n, (u32)D_80045340);
        m2 = (u8 *)n->unk_08;
        *(s16 *)(m2 + 0x2) = *(u16 *)((u8 *)q + 0x2);
        *(s16 *)(m2 + 0x6) = *(u16 *)((u8 *)q + 0x6);
        *(s16 *)(m2 + 0xA) = *(u16 *)((u8 *)q + 0xA);
    }
    else
    {
        return NULL;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s25", func_s25_80024340);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s25", func_s25_800244A0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s25", func_s25_80024674);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s25", func_s25_800246D0);

/* THE EFFECT'S ROOT STEP, an eight-state machine on *(s16 *)(p + 0xA).
   Every frame it bumps the +0x10 timer and spins the +0x16/+0x18 phases by
   0x40/0x80 modulo 0x1000. State 0 resets the timer, clears the ten node
   slots at +0x24 and whitens the renderable; state 1 projects the owning
   slot through func_8003DE58, then either aims at the lock-on target
   (D_800DDC40's per-kind height, three abs deltas, the largest >> 4 as the
   flight time, s16 velocities into b+0xE/0x12/0x16) or -- with no target --
   walks up to eight tiles along the +0xE heading through func_800A44E0 /
   func_800BCB04, lands on the last passable tile and aims there with s32
   velocities into b+0xC/0x10/0x14; state 2 advances the mover, republishes
   it, and on arrival spawns the ten func_s25_800244A0 children at -25*i and
   plays 0x300; states 3/4 fade the renderable up by 0x40404 and grow it;
   state 5 fades it back down, hands off to func_s25_80024028 and retires
   every child; state 6 clears D_8008346C and retires the actor; state 7 is
   state 2's drift for the no-target arm. 656 words, 0 differing.

   THIS FUNCTION OWNS jtbl_s25_80024008, EIGHT ENTRIES AND NO PAD WORD --
   confirmed by tools/decomp/jtcheck.py and by the dispatch bound
   `sltiu $v0, $v1, 0x8': 0 .Ls25_8002488C, 1 .Ls25_800248D4,
   2 .Ls25_80024DF0, 3 .Ls25_80024F30, 4 .Ls25_80024FB0, 5 .Ls25_80024FD8,
   6 .Ls25_800250E0, 7 .Ls25_80025118. All eight are distinct labels, so all
   eight are real cases; case 0 falls through into case 1 and case 3 into
   case 4.

   NO TWIN BY EITHER TOOL. dngtwin's best is the still-stubbed
   func_s27_80024E64 at 0.755; xsibs.py finds NOTHING for any stub in this
   unit (0 of 9 at ratio 0.70 against 3366 landed functions) -- the fourth
   consecutive s-unit where it is useless. The callee-set grep
   (func_800A44E0 / func_800BCB04 / D_8006CCD8 / D_800DDC40) found no landed
   body either: func_800A44E0 and D_800DDC40 appear in NO csrc/ovl_dng_*.c at
   all. What it did find is the resident overlay's two users of the same
   idioms -- func_800A36B4 and func_800A05A4 in
   csrc/ovl_dungeon_8009F644.c -- and the walk block here is written off
   their measured spellings: `((x << 6) + 0x20) & 0xFFE0', `(s16)(z - 0x20)',
   and `D_8006CCD8[dir] & 0xFFFF' for the unsigned step against the plain
   signed read inside the func_800BCB04 argument. The gate idiom in state 1's
   head is csrc/ovl_dungeon.c:5684's verbatim. So: the shape came from the
   LISTING, and the route that supplied the spellings was the callee-set
   grep, not shape matching.

   FACTS MEASURED ON THIS BODY, both spellings priced (aligned differences):

   | spelling                                               | words | rows |
   |--------------------------------------------------------|-------|------|
   | the four retire sites as `UnkNode *r; r->unk_1E |= '   |  656  |   0  |
   |   ... as `*(u16 *)((u8 *)r + 0x1E) |= ' (plain cast)   |  665  |  63  |
   | the state-2 child loop passes `i * -25'                |  656  |   0  |
   |   ... a separate `s16 t = -225; t += 25;' counter       |  656  |   7  |
   | the two narrowing stores use `* 64' / `* 32'           |  656  |   0  |
   |   ... written `<< 6' / `<< 5'                          |  654  |  29  |
   | `(cx << 6) & 0xFFFFFFC0' into func_800A44E0            |  656  |   0  |
   |   ... `& 0xFFC0' with the same u16 prototype            |  659  |   7  |
   | a `u16 z' local for the +0xA read across state 1's gate|  656  |   0  |
   |   ... naming `*(u16 *)(w + 0xA)' in both arms          |  658  |   7  |
   | `UnkVec *v = &vv' and every vv access through it       |  656  |   0  |
   |   ... `&vv' inline at every access                     |  655  |  40  |
   | the two mover copies as `*(UnkVec *)q = *b'            |  656  |   0  |
   |   ... six word-at-a-time field stores                  |  668  | 117  |
   | a SEPARATE local per node/pointer temp (r..r4, q..q4)  |  656  |   0  |
   |   ... one `q' and one `r' reused across the states     |  655  |  27  |
   | state 0's `p+0x20 = 0' AFTER the `p+0xE' store         |  656  |   0  |
   |   ... before it (the listing's own emission order)     |  656  |   2  |
   | the arm-B `i = 1' hoisted ahead of the three vv stores |  656  |   0  |
   |   ... left in the `for' init                           |  656  |   2  |
   | DEVICE: `do { v = &vv; } while (0);'                   |  656  |   0  |
   |   ... the plain assignment                             |  656  |   5  |

   THE FOUR RETIRE SITES ARE THE BIG ONE AND THE MECHANISM IS
   MEM_IN_STRUCT_P. `r->unk_1E |= 0x8000' is a COMPONENT_REF, so sched's
   true_dependence excuses it against the fixed-address scalar D_800814A0
   beside it and the `lw %lo(D_800814A0)' hoists into the `lhu 0x1E' load
   delay while the `ori' fills the `lw's. Through a plain cast the two
   conflict, each read-modify-write serialises behind its own `nop', and the
   function is nine words longer. Note case 6's `*(u16 *)(p - 0x2) |= 0x8000'
   is the OTHER polarity and must stay a plain cast: the target serialises
   there, with the `nop' at 80025108.

   THE DEVICE IS A SCHEDULING BARRIER, NOT A REFERENCE BUMP, and it is
   probably MACRO RESIDUE. reorg fills the `beqz .Ls25_80024C54' delay slot
   at 80024BE4 by stealing the first insn of the branch target, and that
   insn has to be `addiu $a3, $sp, 0x10' -- `addiu $s0, $zero, 1' is not
   eligible because $s0 is live on the fall-through. sched2 puts the `li 1'
   first unless the pointer assignment is fenced, and then reorg emits a
   `nop' and keeps one copy of the `addiu' instead of two. Hoisting `i = 1'
   out of the `for' init is worth 2 of those 5 rows and the wrapper the
   other 3; the two are a CONJUNCTION -- neither alone reaches 0. `if (1)'
   in place of the `do' measures 5, i.e. the same as no device, which is the
   lever file's own rule about `if (1)' not being a loop.

   MEASURED AND FREE, so neither is why this lands: `oy = yy; ox = xx;' in
   either order at the bottom of the walk loop (656 / 0 both ways), and
   spelling the three b velocity stores as plain casts rather than
   `b->unk_0C' (656 / 0).

   THREE SIGNATURE NOTES. func_8009D218 is reached through the project's
   two-argument alias as everywhere else in this family. func_s25_80024028 is
   called from state 5 with THREE arguments -- $a0/$a1/$a2 are all set at
   80025054 -- while its own landed definition above takes two, so this unit
   carries a three-argument alias for that one site; the landed definition is
   untouched. func_800A44E0's landed prototype in
   csrc/ovl_dungeon_8009F644.c is kept as-is and the 32-bit mask constant is
   what absorbs it: as `& 0xFFC0' convert.c distributes the truncation to the
   `u16' parameter into the BIT_AND, the constant becomes HImode -64, and
   cc1psx emits `li $t0, -64' plus two `and's where the target has two
   `andi $reg, $reg, 0xFFC0'. A unit-local `s32, s32, s16, s32' alias also
   measures 656 / 0 and is the same bytes; the mask was preferred because it
   makes no claim about the callee's types. */
void func_s25_800247D4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *s;
    u8 *o;
    u8 *g;
    u8 *w;
    u8 *q;
    u8 *q2;
    u8 *q3;
    u8 *q4;
    u8 *n;
    UnkVec *v;
    UnkNode *r;
    UnkNode *r2;
    UnkNode *r3;
    UnkNode *r4;
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
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    *(s16 *)(p + 0x16) = (*(s16 *)(p + 0x16) + 0x40) % 0x1000;
    *(s16 *)(p + 0x18) = (*(s16 *)(p + 0x18) + 0x80) % 0x1000;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0xE) = (*(u16 *)(s + 0x2A) >> 9) & 7;
        *(s32 *)(p + 0x20) = 0;
        for (i = 9; i >= 0; i--)
        {
            *(s32 *)(p + 0x24 + i * 4) = 0;
        }
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
            g = *(u8 **)(*(u8 **)(s + 0x60) - 0x18);
            out[0] = abs(*(s16 *)(g + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)(g + 0x6) - *(s16 *)((u8 *)b + 0x6));
            out[2] = abs(*(s16 *)(g + 0xA) -
                         (D_800DDC40[(*(u8 **)(s + 0x60))[0x13]] >> 1) -
                         *(s16 *)((u8 *)b + 0xA));
            *(s16 *)(p + 0x12) = out[0];
            for (i = 1; i < 3; i++)
            {
                if ((s16)out[i] > *(s16 *)(p + 0x12))
                {
                    *(s16 *)(p + 0x12) = out[i];
                }
            }
            *(s16 *)(p + 0x12) = (s16) * (u16 *)(p + 0x12) >> 4;
            if (*(s16 *)(p + 0x12) == 0)
            {
                *(s16 *)(p + 0x12) = 1;
            }
            *(s16 *)((u8 *)b + 0xE) =
                (*(s16 *)(g + 0x2) - *(s16 *)((u8 *)b + 0x2)) / *(s16 *)(p + 0x12);
            *(s16 *)((u8 *)b + 0x12) =
                (*(s16 *)(g + 0x6) - *(s16 *)((u8 *)b + 0x6)) / *(s16 *)(p + 0x12);
            *(s16 *)((u8 *)b + 0x16) =
                (*(s16 *)(g + 0xA) - (D_800DDC40[(*(u8 **)(s + 0x60))[0x13]] >> 1) -
                 *(s16 *)((u8 *)b + 0xA)) /
                *(s16 *)(p + 0x12);
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
                                       (s16)(*(s16 *)(p + 0xE) << 9)) != 0)
                {
                    break;
                }
                h = func_800BCB04(((cx + D_8006CCD8[*(s16 *)(p + 0xE)]) << 6) + 0x20 & 0xFFE0,
                                  ((cy + D_8006CCE8[*(s16 *)(p + 0xE)]) << 6) + 0x20 & 0xFFE0,
                                  (s16)(*(u16 *)(s + 0x88) - 0x20));
                if ((s16)h >= 0x201)
                {
                    break;
                }
                if ((s16)(h - *(u16 *)(s + 0x88)) < -0x3F)
                {
                    break;
                }
                xx = xx + (D_8006CCD8[*(s16 *)(p + 0xE)] & 0xFFFF);
                yy = yy + (D_8006CCE8[*(s16 *)(p + 0xE)] & 0xFFFF);
                oy = yy;
                ox = xx;
            }
            do
            {
                v = &vv;
            } while (0);
            i = 1;
            *(s16 *)((u8 *)v + 0x2) =
                ox * 64 + (D_8006CCD8[*(s16 *)(p + 0xE)] + 1) * 32;
            *(s16 *)((u8 *)v + 0x6) =
                oy * 64 + (D_8006CCE8[*(s16 *)(p + 0xE)] + 1) * 32;
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)b + 0xA);
            out[0] = abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)b + 0x6));
            out[2] = abs(*(s16 *)((u8 *)v + 0xA) - *(s16 *)((u8 *)b + 0xA));
            *(s16 *)(p + 0x12) = out[0];
            for (; i < 3; i++)
            {
                if ((s16)out[i] > *(s16 *)(p + 0x12))
                {
                    *(s16 *)(p + 0x12) = out[i];
                }
            }
            *(s16 *)(p + 0x12) = (s16) * (u16 *)(p + 0x12) >> 4;
            if (*(s16 *)(p + 0x12) == 0)
            {
                *(s16 *)(p + 0x12) = 1;
            }
            b->unk_0C = (v->unk_00 - b->unk_00) / *(s16 *)(p + 0x12);
            b->unk_10 = (v->unk_04 - b->unk_04) / *(s16 *)(p + 0x12);
            b->unk_14 = (v->unk_08 - b->unk_08) / *(s16 *)(p + 0x12);
            *(s16 *)(p + 0xA) = 7;
        }
        *(s32 *)(p + 0x1C) = (s32)func_s25_80024148(p, b);
        *(s16 *)(p + 0x10) = 0;
        break;
    case 2:
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + *(u16 *)((u8 *)b + 0xE);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + *(u16 *)((u8 *)b + 0x12);
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + *(u16 *)((u8 *)b + 0x16);
        n = *(u8 **)(p + 0x1C);
        if (n != NULL)
        {
            *(UnkVec *)*(u8 **)(n + 0x8) = *b;
        }
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0x16) = 0;
            *(s16 *)(p + 0x18) = 0x800;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            g = *(u8 **)(*(u8 **)(s + 0x60) - 0x18);
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)(g + 0x2);
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)(g + 0x6);
            *(s16 *)((u8 *)b + 0xA) =
                *(u16 *)(g + 0xA) - (D_800DDC40[(*(u8 **)(s + 0x60))[0x13]] >> 1);
            *(s32 *)(p + 0x20) = func_s25_800246D0(p, b);
            for (i = 9; i >= 0; i--)
            {
                *(s32 *)(p + 0x24 + i * 4) = func_s25_800244A0(p, b, i * -25, i);
            }
            func_800A56E0(0x300);
        }
        break;
    case 3:
        q = *(u8 **)(*(u8 **)(p + 0x20) + 0xC);
        *(s32 *)(q + 0xC) += 0x40404;
        q2 = *(u8 **)(*(u8 **)(p + 0x20) + 0xC);
        *(s16 *)(q2 + 0x1E) = *(u16 *)(q2 + 0x1E) + 0x80;
        *(s16 *)(q2 + 0x1C) = *(u16 *)(q2 + 0x1E);
        r = *(UnkNode **)(p + 0x1C);
        if (r != NULL && *(s16 *)(p + 0x10) == 1)
        {
            r->unk_1E |= 0x8000;
            D_800814A0 |= 0x8000;
            *(s32 *)(p + 0x1C) = 0;
        }
        /* fall through */
    case 4:
        if (*(s16 *)(p + 0x10) >= 0x20)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        q = *(u8 **)(*(u8 **)(p + 0x20) + 0xC);
        *(s32 *)(q + 0xC) += 0xFFFBFBFC;
        q2 = *(u8 **)(*(u8 **)(p + 0x20) + 0xC);
        *(s16 *)(q2 + 0x1C) = *(u16 *)(q2 + 0x1C) - 0x80;
        q3 = *(u8 **)(*(u8 **)(p + 0x20) + 0xC);
        *(s16 *)(q3 + 0x1E) = *(u16 *)(q3 + 0x1E) - 0x80;
        if (*(s16 *)(p + 0x10) >= 0x20)
        {
            func_s25_80024028_3(*(u8 **)(s + 0x60), p[0x9], s);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            r2 = *(UnkNode **)(p + 0x20);
            if (r2 != NULL)
            {
                r2->unk_1E |= 0x8000;
                D_800814A0 |= 0x8000;
                *(s32 *)(p + 0x20) = 0;
            }
            for (i = 9; i >= 0; i--)
            {
                r3 = *(UnkNode **)(p + 0x24 + i * 4);
                if (r3 != NULL)
                {
                    r3->unk_1E |= 0x8000;
                    D_800814A0 |= 0x8000;
                    *(s32 *)(p + 0x24 + i * 4) = 0;
                }
            }
        }
        break;
    case 6:
        if (*(s16 *)(p + 0x14) == 0)
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    case 7:
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + *(u16 *)((u8 *)b + 0xE);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + *(u16 *)((u8 *)b + 0x12);
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + *(u16 *)((u8 *)b + 0x16);
        n = *(u8 **)(p + 0x1C);
        if (n != NULL)
        {
            *(UnkVec *)*(u8 **)(n + 0x8) = *b;
        }
        if (*(s16 *)(p + 0x10) > *(s16 *)(p + 0x12))
        {
            r4 = *(UnkNode **)(p + 0x1C);
            if (r4 != NULL)
            {
                r4->unk_1E |= 0x8000;
                D_800814A0 |= 0x8000;
                *(s32 *)(p + 0x1C) = 0;
            }
            *(s16 *)(p + 0xA) = 6;
            *(s16 *)(p + 0x10) = 0;
        }
        break;
    }
    *(s16 *)(p + 0x14) = 0;
}
