#include "common.h"

s32 rand(void);
UnkNode *func_8003FC64(s32);
void func_8003DB94(void *, void *, s32);
void func_800478B8(u8 *);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
s32 func_800A44E0(u16, u16, s16, u32);
s32 func_800BCB04(s32, s32, s16);
void func_800A56E0(s32);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
s32 func_80053EF0(s32);
void func_800419EC(u16, u32);
s32 func_80045C34(u8 *, UnkVec *, UnkSlot *);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_800DDC40[];
extern s32 D_8008346C;
extern u8 D_80045340[];
extern u8 D_800DED70[];
extern u8 D_s45_80024F74[];
void func_s45_80024048(u8 *p, UnkVec *b, u8 *c);
void func_s45_80024288(u8 *p, UnkVec *b, u8 *c);
UnkNode *func_s45_80024138(u8 *p);
UnkNode *func_s45_800243CC(u8 *p, UnkVec *b);

/* The per-frame step func_s45_80024138 installs on its 0x212 node: bump the
   +0x14 counter on the object at the payload's +0x0, spend one tick of the
   renderable through func_800478B8 and retire the node once its +0x14 reports
   0x6000, then walk the UnkVec outwards -- sine and cosine of the payload's
   +0x8 bearing, each scaled by the +0xA radius and taken down 8 bits, added to
   the vector's +0x2 and +0x6 -- while the +0xA height falls 4 a frame.
   60 words, 0 differing.
   DONOR: the landed func_s60_8002560C in csrc/ovl_dng_s60.c, reached by
   dngcallee on func_800478B8 / func_800644B8 / func_80064584 / D_800814A0 (4
   of 4, and its save order s1,s2,ra,s0 is this listing's exactly). Its gate
   and teardown carry verbatim; the deltas are that this bank reads the +0x14
   flag off `c' rather than building the position from a D_80026208 origin, and
   that the two products are `>> 8' here where s60 has `* 0x100'.
   THE ONE FACT FOUGHT FOR: the +0xA height statement is LAST, after the
   func_80064584 product, not between the two products. Written second it is
   60 words / 8 aligned differences -- every row the four-instruction +0xA
   block, which sched1 then fills the FIRST multiply's mult/mflo shadow with
   instead of the second's. The listing shows it inside the second shadow,
   which no source order puts it in directly; it gets there only by being the
   statement after.
   MEASURED AND FREE, so it is NOT why this lands: the object at +0x0 held in
   a `u8 *q' local versus `*(u8 **)(p + 0x0)' re-dereferenced at both of its
   uses -- 60 words / 0 aligned differences either way, because the two reads
   sit in one basic block with no store between them and cse unifies them
   whatever the source says. */
void func_s45_80024048(u8 *p, UnkVec *b, u8 *c)
{
    u8 *q;

    q = *(u8 **)(p + 0x0);
    *(s16 *)(q + 0x14) = *(u16 *)(q + 0x14) + 1;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    *(s16 *)((u8 *)b + 0x2) =
        *(u16 *)((u8 *)b + 0x2) + ((func_800644B8(*(s16 *)(p + 0x8)) >> 4) * *(s16 *)(p + 0xA) >> 8);
    *(s16 *)((u8 *)b + 0x6) =
        *(u16 *)((u8 *)b + 0x6) + ((func_80064584(*(s16 *)(p + 0x8)) >> 4) * *(s16 *)(p + 0xA) >> 8);
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) - 4;
}

/* Spawns the 0x212 node that runs func_s45_80024048: the caller's payload at
   the new payload's +0x0, a zeroed +0x4, a 6 at +0x6, a `rand() % 0x1000'
   bearing at +0x8 and a 0x10 radius at +0xA, a renderable dressed from
   D_800DED70 at 0x80/0x80 with a zeroed +0xE, sprite 0x7DCF, the +0x14 0xC and
   0x100 bits, the +0x10 0x20 bit and a 0x1000 square scale, and the node's
   UnkVec copied wholesale off the caller's +0x20 with its +0xA overwritten
   from the caller's +0x16. Takes ONE argument -- func_s45_80024578 calls it as
   `func_s45_80024138(p)' from cases 7 and 8, and $a1 is never read.
   84 words, 0 differing.
   DONOR: the landed func_s30_80024F0C in csrc/ovl_dng_s30.c (dngtwin 0.732).
   THE ONE FACT FOUGHT FOR: the node's UnkVec must be held in a `UnkVec *r'
   LOCAL across the struct copy and the +0xA store. Reading `n->unk_08' again
   for the +0xA store is 85 words / 11 aligned differences -- one extra
   `lw 0x8(s1)' plus a $a0/$v1 exchange across the whole eight-word copy block,
   because the reload is a second set of the pointer pseudo. That is the
   opposite of this unit's own func_s45_80024048, where re-dereferencing is
   free; the discriminator is the struct assignment between the two uses, which
   invalidates cse's entry for the load.
   PRICED AND KEPT: the scale written as the chain
   `*(s16 *)(m + 0x1C) = *(s16 *)(m + 0x1E) = 0x1000' -- the other way round is
   84 words / 4 aligned differences, gcc emitting the INNER lvalue first. That
   is the donor's figure for the same fact to the digit, and it reproduces
   again on func_s45_800243CC below, so it is three witnesses in one direction.
   UNMEASURED, flagged as such: the `m[0xD]' / `m[0xC]' / `m[0xE] = 0' store
   order, which landed first try and whose alternatives were not priced. */
UnkNode *func_s45_80024138(u8 *p)
{
    UnkNode *n;
    u8 *q;
    u8 *m;
    UnkVec *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s45_80024048;
        q = (u8 *)n + 0x20;
        *(u8 **)q = p;
        *(s16 *)(q + 0x4) = 0;
        *(s16 *)(q + 0x6) = 6;
        *(s16 *)(q + 0x8) = rand() % 0x1000;
        *(s16 *)(q + 0xA) = 0x10;
        m = (u8 *)n->unk_0C;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        m[0xE] = 0;
        *(s16 *)(m + 0x12) = 0x7DCF;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x20;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8003DB94(m, D_800DED70, 0);
        *(s16 *)(m + 0x1C) = *(s16 *)(m + 0x1E) = 0x1000;
        func_8004491C(n, (u32)D_80045340);
        r = n->unk_08;
        *r = *(UnkVec *)*(u8 **)(p + 0x20);
        *(s16 *)((u8 *)r + 0xA) = *(u16 *)(p + 0x16);
        return n;
    }
    return NULL;
}

/* The four-state step func_s45_800243CC installs: bump the +0x14 counter on
   the object at the payload's +0x0, spin the renderable's +0x1A by 0x300, run
   the +0x10 frame counter up, then on the +0xE phase either integrate the
   UnkVec's three velocities and retire the leg at +0x14 frames (0), hold for
   +0x16 frames (1), sink the vector by the frame count and retire at 0x18 (2),
   or set the node's own +0x8000 retire bit and D_800814A0's (3).
   81 words, 0 differing, FIRST COMPILE. A leaf: no frame, no saves.
   NO DONOR WAS NEEDED. dngtwin finds no carved sibling at 0.70 and dngcallee
   is pure noise here -- its only symbol is D_800814A0, reached by 601
   functions, and the tool says so. The body came off the listing plus the
   SPAWNER forty lines below, which names every offset it reads: +0x14 is the
   budget it seeds from the caller's +0x12 and +0x16 the `0x1C - caller +0x10'
   dwell. Reading the spawner first meant no offset had to be guessed.
   THIS IS A COMPARE-CHAIN SWITCH, NOT A TABLE OWNER -- 4 cases over {0,1,2,3}
   compiled to gcc's balanced tree (`== 1', then `< 2', then `== 0', then
   `== 2', `== 3'), so it owns no .rodata and its ARM ORDER IS PRICED rather
   than invisible:
     arms ascending 0/1/2/3                          81 words,  0  kept
     case 1 written before case 0                    81 words, 20
   Twenty rows for one transposition, with no word gained, because a
   compare-chain's arms emit in SOURCE order while its decision tree is
   balanced on VALUE -- so moving case 1 up breaks the fall-through that lets
   case 0's and case 2's tails cross-join into one block.
   TWO SPELLINGS READ STRAIGHT OFF THE LISTING rather than guessed, both from
   cse forwarding the +0x10 store to the test that follows it: case 1 compares
   the ALREADY-INCREMENTED value with a sign extension (`sll 16'/`sra 16' off
   the stored pseudo, no `lh'), and case 2's `<< 16' takes the same pseudo with
   no extension at all because the shift discards the high half. Writing either
   as a fresh `*(s16 *)(p + 0x10)' read would have cost a reload the target
   does not have.
   NOTE the two retire orders DISAGREE between arms and that is the listing's:
   case 0 and case 2 write `p+0x10 = 0' then `p+0xE++', case 1 writes `p+0xE++'
   then `p+0x10 = 0'. Unmeasured -- flagged as such; it was taken from the
   emission order, not priced. */
void func_s45_80024288(u8 *p, UnkVec *b, u8 *c)
{
    u8 *q;

    q = *(u8 **)(p + 0x0);
    *(s16 *)(q + 0x14) = *(u16 *)(q + 0x14) + 1;
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x300;
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    switch (*(s16 *)(p + 0xE))
    {
    case 0:
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
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x16))
        {
            *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 1;
            *(s16 *)(p + 0x10) = 0;
        }
        break;
    case 2:
        b->unk_08 -= *(s16 *)(p + 0x10) << 16;
        if (*(s16 *)(p + 0x10) >= 0x18)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 1;
        }
        break;
    case 3:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}

/* Spawns the 0x212 node that runs func_s45_80024288: the caller's payload at
   the new payload's +0x0, a zeroed +0xE phase and +0x10 counter, a
   `rand() % 7' at +0x12, the caller's +0x12 budget at +0x14 and a
   `0x1C - caller +0x10' dwell at +0x16; a renderable dressed from this bank's
   own D_s45_80024F74 at white 0x80 with the +0x14 0xC and +0x10 0x60 bits, a
   random rotation and a `caller +0x10 * 0x100' square scale; then the caller's
   UnkVec copied in and its +0x14 velocity rescaled so the leg still covers
   `caller +0x10 << 19' in +0x14 frames.
   107 words, 0 differing.
   DONOR: the landed func_s30_80024F0C in csrc/ovl_dng_s30.c (dngtwin 0.806,
   99 words) -- the same 0x212 spawner, dressed differently.
   THE ONE FACT FOUGHT FOR, and it is the convert.c narrowing lever on a new
   operator: the scale must be `*(s16 *)(p + 0x10) * 0x100', NOT
   `*(s16 *)(p + 0x10) << 8'. The shift form is 107 words / 1 aligned
   difference -- a single `lhu' where the target has `lh', nothing else moving.
   Cause: the destination is `s16', so convert.c narrows the operand, and its
   LSHIFT_EXPR arm is guarded on TREE_UNSIGNED (type) while MULT_EXPR's is not,
   so only the shift spelling picks up the unsigned load. This is the third
   operator the same guard has now bitten on in this tree, and the diagnostic
   is always the same single `lhu'-for-`lh' row. The SAME lever, same
   direction, is worth +2 words and 23 rows in func_s45_80024578's walk arm, so
   it is priced twice in this file.
   PRICED AND KEPT: the scale as the chain
   `*(s16 *)(m + 0x1C) = *(s16 *)(m + 0x1E) = ...' -- reversed it is 107 words
   / 4 aligned differences, the donor's figure exactly, and the same 4 rows
   again on func_s45_80024138 above.
   PRICED AND KEPT: `rand() % 7', a SIGNED modulo with the 0x92492493 magic
   multiply. `rand() & 7' is 97 words / 12 aligned differences -- ten words
   short, since the mask needs no reciprocal. The donor priced the same fact at
   89 / 14, so direction carried and magnitude did not.
   MEASURED AND FREE, so NOT why this lands: nothing -- every spelling tried on
   this body moved it. */
UnkNode *func_s45_800243CC(u8 *p, UnkVec *b)
{
    UnkNode *n;
    u8 *q;
    u8 *m;
    UnkVec *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s45_80024288;
        q = (u8 *)n + 0x20;
        *(u8 **)q = p;
        *(s16 *)(q + 0xE) = 0;
        *(s16 *)(q + 0x10) = 0;
        *(s16 *)(q + 0x12) = rand() % 7;
        *(s16 *)(q + 0x14) = *(u16 *)(p + 0x12);
        *(s16 *)(q + 0x16) = 0x1C - *(u16 *)(p + 0x10);
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        *(u32 *)(m + 0x8) = (u32)D_s45_80024F74;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x60;
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1C) = *(s16 *)(m + 0x1E) = *(s16 *)(p + 0x10) * 0x100;
        func_8004491C(n, (u32)func_80045C34);
        r = n->unk_08;
        *r = *b;
        r->unk_14 =
            (r->unk_14 * *(s16 *)(q + 0x14) - (*(s16 *)(p + 0x10) << 19)) / *(s16 *)(q + 0x14);
        return n;
    }
    return NULL;
}

/* The sixteen-state homing/charge step, and the owner of jtbl_s45_80024008.
   State 0 seeds the facing from the actor's +0x2A and whitens the tint; state
   1 reads the target through func_8003DE58, copies its position into the
   mover, then either locks on to the party member at
   `*(u8 **)(s + 0x60) - 0x18' -- caching that member, its owner and its +0x14
   peer into the payload's +0x20/+0x1C/+0x24 and its height and bearing into
   +0x16/+0x18, three abs deltas with the largest >> 4 as the frame count, the
   three velocities divided by it -- or walks up to eight tiles with
   func_800A44E0/func_800BCB04 and aims at the far end, and either way kicks a
   func_s45_800243CC trail off and jumps to state 2 or 11; states 3 and 12
   integrate the velocity, ring func_800A56E0/func_800542BC and (3 only) claim
   the owner's 0x100000 bit; 4 and 13 wait on func_80053EF0; 6, 7, 8 and 9 are
   the charge -- sinking the member, spinning its bearing, firing eight
   func_s45_80024138 puffs and finally the func_8009CE1C hit with the 0x100000
   bit released; 5, 14 and 15 are dwells and 10 the teardown.
   639 words, 0 differing.
   DONOR: the landed func_s43_80025954 in csrc/ovl_dng_s43.c, the SEVEN-state
   member of the same family. dngtwin found NOTHING at 0.70 for this body --
   it is a 639-word state machine against a 454-word one, far outside any
   length band, which is exactly the class dngcallee exists for. dngcallee
   scored 10 of 13 symbols with a clean gap to the 9/13 tier and named two
   donors; the save ORDER picked between them, s43's
   `s1,s6,ra,s8,s7,s5,s4,s3,s2,s0' being this listing's exactly where s44's
   differs. Every one of s43's priced facts carried: the shared `UnkVec *v'
   across the lock-on and walk arms, `(cx << 6) & 0xFFFFFFC0', `ox * 64',
   `(D_8006CCD8[dir] + 1) * 32', `u16 z' for the target's +0xA across the flag
   gate, the hoisted `i = 1', and `p+0x10 = 0' ONCE after the if/else.
   NOTE s43's hard-won `A - B - K - C' re-association fact DOES NOT ARISE here:
   this bank reads the party member's own +0x88 directly where s43 corrects it
   through D_800DDC40 and a 0x300000 lift, so there is no constant inside the
   subtraction to re-associate. That is a fact made IMPOSSIBLE by a shape
   difference rather than inverted.

   THIS FUNCTION OWNS A JUMP TABLE: jtbl_s45_80024008, 16 `.word' = 16 cases +
   0 pad. The dispatch bound is `sltiu $v0, $v1, 0x10' with no minval
   subtraction, so the case set is 0..15 with every value a real arm and NO
   default among the entries -- the range-check `beqz $v0, .Ls45_80024F3C'
   lands on the common tail (`p+0x14 = 0'), which is not a table entry. 14 of
   the 16 entries are distinct: cases 2 and 11 share one arm and cases 5 and 14
   share another.
   THE ARMS ARE WRITTEN IN ENTRY-ADDRESS ORDER, which is
     0, 1, 3, 4, 6, 7, 8, 9, 10, {2,11}, 12, 13, {5,14}, 15
   and which nobody would write from the case values. Case 0 FALLS THROUGH into
   case 1. This order is MEASURABLE on this body, because the arms call
   different functions and differ in length, so jump2 cannot collapse them:
     arms in entry-address order                   639 words,  0  kept
     cases 12 and 13 transposed, nothing else      639 words, 15
   and a standalone `tools/cdkgcc -S' confirms the emitted `.word' sequence --
   $L28 $L29 $L86 $L58 $L61 $L93 $L63 $L65 $L72 $L80 $L83 $L86 $L88 $L90 $L93
   $L95, with the two shared labels in the right slots, and the transposed
   variant emits $L90 before $L88 as expected. Do NOT read the clean 0 as
   evidence by itself: ovlverify and aligndiff both mask a `j' target and
   neither reads .rodata. The 15 rows and the emitted sequence are the
   evidence.

   THREE MORE FACTS PRICED FROM THE 0 STATE:
     the walk arm's `ox * 64' / `(D_8006CCD8[dir] + 1) * 32'
                                                   639 words,  0  kept
       ... the same written `(ox << 6)' / `((... + 1) << 5)'
                                                   641 words, 23
       Same convert.c narrowing guard as func_s45_800243CC's scale, two words
       dearer here because the destination is an `s16' field of `vv' and the
       hoisted constant needs a register.
     `(cx << 6) & 0xFFFFFFC0' in the func_800A44E0 call
                                                   639 words,  0  kept
       ... `& 0xFFC0'                              642 words,  7
       The mask must be the full 32-bit constant; the short form is an `andi'
       against a HImode -64 and costs three words materialising it.
     `p+0x10 = 0' and the func_s45_800243CC call written ONCE after case 1's
     if/else                                       639 words,  0  kept
       ... duplicated into both arms                643 words, 17
       jump2 cross-joins the two arms' `p+0xA = ...' stores by itself, so
       duplicating the tail only defeats the join. This is s43's fact holding.

   MEASURED AND FREE, so it is NOT why this lands: case 12's retire written
   `p+0xA++' before `p+0x10 = 0' rather than after -- 639 words / 0 aligned
   differences either way. Both orders reach the same shared block, which
   twelve arms cross-join into, and sched normalises the pair. Genuinely free
   rather than not-measurable: aligndiff can see an instruction transposition,
   and the word count did not move, so the join survived both spellings.

   THE ONE FACT FOUGHT FOR: in cases 2 and 11, the
   `*(s32 *)(p + 0x28) = *(s32 *)(*(u8 **)(p + 0x20) + 0x8)' statement comes
   FIRST in the if body, before the two retire stores. Written last -- which is
   the order the listing appears to show, since reorg puts its `sw' in the
   `j' delay slot -- it is 639 words / 7 aligned differences, with both `lw'
   sinking below the `p+0xA' read. A store's position inside a block whose tail
   feeds a delay slot is not readable off the listing; it has to be priced. */
void func_s45_80024578(u8 *p, UnkVec *b, u8 *c)
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
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0xE) = (*(u16 *)(s + 0x2A) >> 9) & 7;
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
            *(s32 *)(p + 0x20) = (s32)v;
            *(s32 *)(p + 0x1C) = (s32) * (u8 **)(s + 0x60);
            *(s32 *)(p + 0x24) = *(s32 *)(*(u8 **)(s + 0x60) - 0x14);
            *(s16 *)(p + 0x16) = *(u16 *)(*(u8 **)(p + 0x1C) + 0x88);
            *(s16 *)(p + 0x18) = *(u16 *)(*(u8 **)(p + 0x1C) + 0x2A);
            out[0] = abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)b + 0x6));
            out[2] = abs(*(s16 *)(*(u8 **)(s + 0x60) + 0x88) - *(s16 *)((u8 *)b + 0xA));
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
            b->unk_0C = (v->unk_00 - b->unk_00) / *(s16 *)(p + 0x12);
            b->unk_10 = (v->unk_04 - b->unk_04) / *(s16 *)(p + 0x12);
            b->unk_14 = ((*(s16 *)(*(u8 **)(s + 0x60) + 0x88) << 16) - b->unk_08) /
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
            v = &vv;
            *(s16 *)((u8 *)v + 0x2) = ox * 64 + (D_8006CCD8[*(s16 *)(p + 0xE)] + 1) * 32;
            *(s16 *)((u8 *)v + 0x6) = oy * 64 + (D_8006CCE8[*(s16 *)(p + 0xE)] + 1) * 32;
            *(s16 *)((u8 *)v + 0xA) =
                func_800BCB04(ox * 64 + 0x20 & 0xFFE0, oy * 64 + 0x20 & 0xFFE0, -0x400);
            out[0] = abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)b + 0x6));
            out[2] = abs(*(s16 *)((u8 *)v + 0xA) - *(s16 *)((u8 *)b + 0xA));
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
            b->unk_0C = (v->unk_00 - b->unk_00) / *(s16 *)(p + 0x12);
            b->unk_10 = (v->unk_04 - b->unk_04) / *(s16 *)(p + 0x12);
            b->unk_14 = (v->unk_08 - b->unk_08) / *(s16 *)(p + 0x12);
            *(s16 *)(p + 0xA) = 0xB;
        }
        *(s16 *)(p + 0x10) = 0;
        func_s45_800243CC(p, b);
        break;
    case 3:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            func_800A56E0(0x300);
            func_800542BC();
            *(s16 *)(p + 0x1A) = 0;
            if ((*(s32 *)(*(u8 **)(p + 0x1C) + 0x14) & 0x100000) == 0)
            {
                *(s32 *)(*(u8 **)(p + 0x1C) + 0x14) |= 0x100000;
                *(s16 *)(p + 0x1A) = 1;
            }
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        if (func_80053EF0(4) == 1)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 6:
        *(s16 *)(*(u8 **)(p + 0x20) + 0xA) =
            *(u16 *)(*(u8 **)(p + 0x20) + 0xA) - *(u16 *)(p + 0x10);
        *(s16 *)(*(u8 **)(p + 0x1C) + 0x2A) = *(u16 *)(*(u8 **)(p + 0x1C) + 0x2A) + 0x200;
        if (*(s16 *)(p + 0x10) >= 0x1C)
        {
            *(s16 *)(*(u8 **)(p + 0x24) + 0x1A) = *(u16 *)(*(u8 **)(p + 0x24) + 0x1A) + 0x800;
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 7:
        *(s16 *)(*(u8 **)(p + 0x20) + 0xA) =
            *(u16 *)(*(u8 **)(p + 0x20) + 0xA) + *(s16 *)(p + 0x10) * 4;
        *(s16 *)(*(u8 **)(p + 0x1C) + 0x2A) = *(u16 *)(*(u8 **)(p + 0x1C) + 0x2A) + 0x200;
        if (*(s16 *)(*(u8 **)(p + 0x20) + 0xA) >=
            *(s16 *)(p + 0x16) - D_800DDC40[(*(u8 **)(p + 0x1C))[0x13]])
        {
            *(s16 *)(*(u8 **)(p + 0x20) + 0xA) =
                *(u16 *)(p + 0x16) - D_800DDC40[(*(u8 **)(p + 0x1C))[0x13]];
            func_800419EC(8, 0x10);
            for (i = 0; i < 8; i++)
            {
                func_s45_80024138(p);
            }
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 8:
        *(s16 *)(*(u8 **)(p + 0x1C) + 0x2A) = *(u16 *)(*(u8 **)(p + 0x1C) + 0x2A) + 0x200;
        if (*(s16 *)(p + 0x10) == 2)
        {
            for (i = 0; i < 8; i++)
            {
                func_s45_80024138(p);
            }
        }
        if (*(s16 *)(p + 0x10) >= 0xC)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 9:
        if (*(s16 *)(p + 0x10) >= 0x4)
        {
            *(s32 *)(*(u8 **)(p + 0x20) + 0x8) = *(s32 *)(p + 0x28);
            *(s16 *)(*(u8 **)(p + 0x1C) + 0x2A) = *(u16 *)(p + 0x18);
            if (*(s16 *)(p + 0x1A) != 0)
            {
                *(s32 *)(*(u8 **)(p + 0x1C) + 0x14) &= 0xFFEFFFFF;
            }
            *(s16 *)(*(u8 **)(p + 0x24) + 0x1A) = *(u16 *)(*(u8 **)(p + 0x24) + 0x1A) - 0x800;
            func_8009CE1C(*(u8 **)(s + 0x60), 0x18, p[0x9], 4, (s16)(*(u16 *)(p + 0xE) << 9),
                          (u32)s, 1);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 10:
        if (*(s16 *)(p + 0x14) == 0)
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    case 2:
    case 11:
        func_s45_800243CC(p, b);
        if (*(s16 *)(p + 0x10) >= 0xF)
        {
            *(s32 *)(p + 0x28) = *(s32 *)(*(u8 **)(p + 0x20) + 0x8);
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            *(s16 *)(p + 0x10) = 0;
        }
        break;
    case 12:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            func_800A56E0(0x300);
            func_800542BC();
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 13:
        if (func_80053EF0(4) == 1)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
    case 14:
        if (*(s16 *)(p + 0x10) >= 0x7)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 15:
        if (*(s16 *)(p + 0x10) >= 0x1C)
        {
            func_800A56E0(0x74);
            *(s16 *)(p + 0xA) = 0xA;
            *(s16 *)(p + 0x10) = 0;
        }
        break;
    }
    *(s16 *)(p + 0x14) = 0;
}
