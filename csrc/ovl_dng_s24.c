#include "common.h"

s32 rand(void);
UnkNode *func_8003FC64(s32);
extern u8 D_s24_80024C78[];
extern u8 D_80045340[];
extern u8 D_800DEF08[];
void func_800478B8(u8 *);
void func_8003DB94(u8 *, u8 *, s16);
s32 func_800D36D0(u8 *, UnkVec *, UnkSlot *);
void func_s24_80024020(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_s24_80024264(u8 *p, UnkVec *b, u8 *c, u8 *d);

/* The per-frame step func_s24_80024134 installs on its 0x212 node: bump the
   +0x14 counter on the node at the payload's +0x8, advance the UnkVec's x/y by
   their velocities, accumulate the +0x4 angle by the z velocity and set the z
   position to that angle less func_800644B8's sine of 0x800 / p+0x2 * p+0x0
   scaled by 0xC000, spin the renderable's +0x1A by 0x300, then run the +0x0
   frame counter up and retire when it passes the +0x2 lifetime.
   69 words, 0 differing, FIRST COMPILE.
   TWIN: csrc/ovl_dng_s43.c's landed func_s43_80024044 (dngtwin 0.847, 75
   words).  This body is 6 words SHORTER and the missing statements are the
   twin's +0x1C/+0x1E scale pair, which this bank has not got -- so the
   "shorter means missing statements, not missing references" rule held.  The
   payload fields move from s43's +0x0/+0x6/+0x8/+0xC to +0x8/+0x2/+0x2/+0x4,
   and the sine is scaled by 0xC000 rather than 0x4000.
   PRICED AFTER THE FACT, because a first-compile landing measures nothing.
   One fact, two frees:
     * THE FRAME-COUNTER BUMP GOES LAST, immediately before the retire test,
       and NOT second as in the twin: in the twin's position it is 70 words /
       59 differing.  Adjacent to the test, cse forwards the stored value and
       the `sll 16'/`sra 16' pair at 800240E4 is the sign extension of it;
       bumped early, the test reloads with `lh' and the body gains a word.
       This is a STATEMENT-ORDER fact, the class the brief says nothing about a
       twin predicts, and it inverted against the twin.
     * MEASURED AND FREE, so NOT why this lands: `((... >> 4) * 3) << 14' for
       the kept `(... >> 4) * 0xC000' is 69 words / 0 differing -- the same
       tree, so NOT MEASURABLE rather than free.
     * MEASURED AND FREE: the retire test written
       `*(s16 *)(p + 0x2) < *(s16 *)(p + 0x0)' instead of `>' is 69 words /
       0 differing.  The comparison-operand lever does not bite here, though
       it is worth 2 rows on func_s24_800244F0's loop in the same unit. */
void func_s24_80024020(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)(*(u8 **)(p + 0x8) + 0x14) = *(u16 *)(*(u8 **)(p + 0x8) + 0x14) + 1;
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    *(s32 *)(p + 0x4) += b->unk_14;
    b->unk_08 = *(s32 *)(p + 0x4) -
                ((func_800644B8(0x800 / *(s16 *)(p + 0x2) * *(s16 *)(p + 0x0)) >> 4) * 0xC000);
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x300;
    *(s16 *)(p + 0x0) = *(u16 *)(p + 0x0) + 1;
    if (*(s16 *)(p + 0x0) > *(s16 *)(p + 0x2))
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212 node whose step is func_s24_80024020: payload +0x8 the
   caller's record, +0x0 the frame counter at 0, +0x2 the caller's lifetime
   argument, +0x4 the mover's z; dress the slot with a 0x808080 tint, the
   bank-local D_s24_80024C78 model, 0x7E02 at +0x12, bit 8 set in +0x14, a
   random `rand() % 0x1000' spin at +0x1A and 0x800 scale, register it with
   func_8004491C against D_80045340, copy the caller's UnkVec into the node's
   own and seed the payload's +0x4 from its z.
   76 words, 0 differing, FIRST COMPILE.
   TWIN: csrc/ovl_dng_s43.c's landed func_s43_80024170 (dngtwin 0.967, 75
   words).  Both of its facts hold: the `r = (u8 *)n + 0x20' payload pointer,
   and the jitter as a SIGNED `rand() % 0x1000' -- its bgez / addiu 0xFFF /
   sra 12 / sll 12 / subu quintet is 5 of these 76 words.  The deltas off the
   listing: a THIRD parameter, the s16 lifetime stored at +0x2, which is why
   this frame saves five callee-saved registers where the twin saves four; the
   payload fields move; the model is bank-local rather than the resident
   D_800DE720; 0x7E02 rather than 0x7E00 at +0x12; 0x800 rather than 0x400
   scale; and `*(u32 *)(r + 0x4) = k->unk_08' where the twin writes +0xC.
   PRICED AFTER THE FACT: `rand() & 0xFFF' instead of `rand() % 0x1000' is
   70 words / 64 differing, so the twin's recorded signed-remainder fact holds
   here with a figure of its own.  Everything else in this body is ASSERTED,
   not measured. */
UnkNode *func_s24_80024134(u8 *p, UnkVec *q, s16 d)
{
    UnkNode *n;
    UnkVec *k;
    u8 *m;
    u8 *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s24_80024020;
        r = (u8 *)n + 0x20;
        *(u8 **)(r + 0x8) = p;
        *(s16 *)(r + 0x0) = 0;
        *(s16 *)(r + 0x2) = d;
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        *(u32 *)(m + 0x8) = (u32)D_s24_80024C78;
        *(s16 *)(m + 0x12) = 0x7E02;
        *(s16 *)(m + 0x14) = *(u16 *)(m + 0x14) | 0x100;
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1E) = 0x800;
        *(s16 *)(m + 0x1C) = 0x800;
        func_8004491C(n, (u32)D_80045340);
        k = n->unk_08;
        *k = *q;
        *(u32 *)(r + 0x4) = k->unk_08;
        return n;
    }
    return NULL;
}

/* The four-state step func_s24_800243BC installs: bump the +0x14 counter on
   the node at the payload's +0x8, run the payload's +0x2 frame counter up,
   call func_800478B8 on the renderable, then do one of four things to its
   colour bytes and +0x1C/+0x1E scale by the payload's +0x0 state, advancing
   that state every time +0x2 reaches the +0x4 period.  State 3 calls
   func_800478B8 a second time and does NOT run the period test.
   86 words, 0 differing.  NO TWIN: dngtwin's best is 0.797
   (func_s29_8002558C) and xsibs finds nothing at all; it came off the listing.
   NOTE this body is a COMPARE-CHAIN switch, not a table owner -- the unit's
   one jump table belongs to func_s24_800244F0.
   FOUR FACTS, each priced on its own from the 0 state:
     * THE PERIOD TEST IS WRITTEN OUT IN EACH OF STATES 0, 1 AND 2, not once
       after the switch.  Written once, with `return' in state 3 and in
       `default', it is 88 words / 23 differing / 10 aligned rows.  Three
       copies is what the ROM's layout demands: jump2 cross-joins them --
       state 1's and state 2's merge at their shared `sh $v0, 0x1E($s0)' and
       state 0's at the test itself -- so the merged test lands BEFORE state
       3's block, and state 3's `break' then falls through into the epilogue.
       Written once it can only land AFTER state 3.
     * `c[0xC] = c[0xD] = c[0xE] = c[0xE] + 1' AND NOT the other nesting: a
       chained assignment stores RIGHT TO LEFT, so the innermost lvalue is the
       one emitted first.  `c[0xE] = c[0xD] = c[0xC] = ...' at all three sites
       is 86 words / 6 differing, every row an `sb' displacement.
     * `*(s16 *)(p + 0x2) * 0x100' AND NOT `<< 8'.  The shift form emits
       `lhu $v0, 0x2($s1)' where the ROM has `lh', because convert.c's
       LSHIFT_EXPR arm is guarded on TREE_UNSIGNED and MULT_EXPR's is not; the
       narrowing context is the `s16' destination.  At state 1's site alone it
       is 86 words / 1 differing.
     * STATE 0'S `+0x1C' STATEMENT COMES BEFORE ITS `+0x1E' ONE, which is the
       REVERSE of the emitted order -- the ROM stores +0x1E first.  Source
       order +0x1E then +0x1C is 85 words / 12 aligned rows.  Two independent
       halfword stores off one base get swapped by the scheduler here, so do
       NOT read this pair's order off the listing.  (This is the unit's one
       instance of the brief's "a store's position inside a block is not
       readable" caution.) */
void func_s24_80024264(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)(*(u8 **)(p + 0x8) + 0x14) = *(u16 *)(*(u8 **)(p + 0x8) + 0x14) + 1;
    *(s16 *)(p + 0x2) = *(u16 *)(p + 0x2) + 1;
    func_800478B8(c);
    switch (*(s16 *)(p + 0x0))
    {
    case 0:
        c[0xC] = c[0xD] = c[0xE] = c[0xE] + 1;
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + 0x100;
        *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + 0x10;
        if (*(s16 *)(p + 0x2) >= *(s16 *)(p + 0x4))
        {
            *(s16 *)(p + 0x2) = 0;
            *(s16 *)(p + 0x0) = *(u16 *)(p + 0x0) + 1;
        }
        break;
    case 1:
        c[0xC] = c[0xD] = c[0xE] = c[0xE] + 1;
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + 0x80;
        *(s16 *)(c + 0x1E) = *(s16 *)(p + 0x2) * 0x100;
        if (*(s16 *)(p + 0x2) >= *(s16 *)(p + 0x4))
        {
            *(s16 *)(p + 0x2) = 0;
            *(s16 *)(p + 0x0) = *(u16 *)(p + 0x0) + 1;
        }
        break;
    case 2:
        c[0xC] = c[0xD] = c[0xE] = c[0xE] + 6;
        *(s16 *)(c + 0x1E) = (*(s16 *)(p + 0x2) + 0x10) * 0x100;
        if (*(s16 *)(p + 0x2) >= *(s16 *)(p + 0x4))
        {
            *(s16 *)(p + 0x2) = 0;
            *(s16 *)(p + 0x0) = *(u16 *)(p + 0x0) + 1;
        }
        break;
    case 3:
        func_800478B8(c);
        break;
    }
}

/* Spawn the 0x212 node whose step is func_s24_80024264: payload +0x8 the
   caller's record, +0x0 the state at 0, +0x2 the frame counter at 0, +0x4 the
   period at 0x10; dress the slot with 0x7E02 at +0x12, zeroed colour bytes and
   zeroed +0x1C/+0x1E scale, 0xC set in +0x14, 0x20 set in +0x10 and then 0x100
   also set in +0x14, push D_800DEF08 through func_8003DB94 with a random
   `rand() % 0x10' frame, register it with func_8004491C against the FUNCTION
   func_800D36D0 rather than D_80045340, and copy the caller's UnkVec into the
   node's own.
   77 words, 0 differing, FIRST COMPILE.
   NO CARVED TWIN -- dngtwin's best is 0.753.  It was written off the resident
   csrc/ovl_dungeon_800CABA8.c's landed func_800D2CB4, which spawns the same
   kind of entity: that body gave the dress ORDER verbatim (+0x12, then
   +0x14 |= 0xC, then +0x10 |= 0x20, then +0x14 |= 0x100) and the
   func_800D36D0 list tag.  That tag is the thing to notice: func_8004491C
   takes a FUNCTION here where this unit's other spawner passes D_80045340,
   and csrc/ovl_dungeon_800CABA8.c's own comment records the same oddity.
   PRICED AFTER THE FACT: `rand() & 0xF' instead of `rand() % 0x10' is
   69 words / 67 differing, so the signed remainder is load-bearing here as
   well as in func_s24_80024134.
   NOTE, and it is asserted rather than measured: the +0x14 read-modify-write
   appears twice with the +0x10 one between, and the ROM's second one reuses
   the register rather than reloading -- which is what the plain cast through
   `u8 *m' gives, because a plain-cast word store off a pointer does not
   invalidate the cse equivalence the first store recorded. */
UnkNode *func_s24_800243BC(u8 *p, UnkVec *q)
{
    UnkNode *n;
    UnkVec *k;
    u8 *m;
    u8 *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s24_80024264;
        r = (u8 *)n + 0x20;
        *(u8 **)(r + 0x8) = p;
        *(s16 *)(r + 0x0) = 0;
        *(s16 *)(r + 0x2) = 0;
        *(s16 *)(r + 0x4) = 0x10;
        m = (u8 *)n->unk_0C;
        *(s16 *)(m + 0x12) = 0x7E02;
        m[0xE] = 0;
        m[0xD] = 0;
        m[0xC] = 0;
        *(s16 *)(m + 0x1E) = 0;
        *(s16 *)(m + 0x1C) = 0;
        *(s16 *)(m + 0x14) = *(u16 *)(m + 0x14) | 0xC;
        *(s16 *)(m + 0x10) = *(u16 *)(m + 0x10) | 0x20;
        *(s16 *)(m + 0x14) = *(u16 *)(m + 0x14) | 0x100;
        func_8003DB94(m, D_800DEF08, rand() % 0x10);
        func_8004491C(n, (u32)func_800D36D0);
        k = n->unk_08;
        *k = *q;
        return n;
    }
    return NULL;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s24", func_s24_800244F0);
