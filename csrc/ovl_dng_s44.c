#include "common.h"

s32 rand(void);

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
extern u8 D_800E3D20;
extern u8 D_80025B1C;
void func_s44_80024024(u8 *p);

/* The camera step the installer below parks in D_80083178's +0xB4 slot.
   `p' is the record it fills, D_80083178 + 0xB8. Ease the engine's +0x98
   bearing towards the request's over the frames left at +0x24, run that
   counter down with a -0x80 floor, spend one frame of D_800E3D20, copy the
   mover's (x, z, bearing) into the record's +0x4/+0x6/+0x8 and ease the three
   +0xA4/+0xA6/+0xA8 axes a quarter of the way towards them -- the third only
   while this bank's D_80025B1C is set, which is then re-armed from whether
   +0xA8 has caught up -- then halve the two +0x94/+0x96 rates.
   82 words, 0 differing, FIRST COMPILE.
   NO CARVED SIBLING AND NO RESIDENT TWIN BY EITHER TOOL: dngtwin's only hit
   is the 1.000 clone func_s44_80024024, which is still an INCLUDE_ASM. The
   body came from the DATA SYMBOL: `grep -n D_80083178 csrc/*.c' names
   csrc/ovl_dungeon_800C2824.c's landed func_800C75D0, whose own header
   already says it is the step installed at +0xB4 and that `p' is
   D_80083178 + 0xB8. That is a 128-word body against this 82, a size ratio
   no shape matcher can see past. The 46 missing words are MISSING STATEMENTS:
   the +0x34 pending-request retirement, the `else D_800E3D20 = 0' on the
   first gate, and the func_800BCB04 call with its 0x200 clamp that computes
   the bearing instead of copying +0xA.
   Of func_800C75D0's three stated facts:
     - HOLDS, far more cheaply: every read of +0x0C is RE-DEREFERENCED, never
       cached in a local. A cached `m = *(u8 **)(p + 0xC);' is 82 words / 3
       aligned differences here, where the twin priced the same change at 124
       words against 128 with 109 differing. Without its +0x34 block there
       are only two `lw 0xC' in the target, not four, so three rows is the
       whole fact.
     - HOLDS and reproduces exactly: the two trailing halvings read `lhu' and
       sign-extend at the shift (`sll 16'/`sra 18'), so `*(s16 *)' and
       `(s16)*(u16 *)' are byte-identical -- both 82 words / 0, because the
       `sh' truncates either way.
     - HOLDS: D_80083178 is reached through a `u8 *' LOCAL. Inline as
       `*(s16 *)((u8 *)&D_80083178 + 0x98)' at each of the seven sites it is
       82 words / 38 aligned differences.
   Measured and FREE: the -0x80 clamp read back out of memory rather than off
   an `s16' local (82 / 0 either way), and `D_800E3D20--' for
   `D_800E3D20 -= 1'. */
void func_s44_8002441C(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_800478B8(u8 *);
UnkNode *func_8003FC64(s32);
s32 func_80064584(s32);
s32 func_800644B8(s32);
void func_8003DB94(void *, void *, s32);
UnkNode *func_8003FD64(s32, void *);
void func_s44_800241E4(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_s44_8002457C(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u8 D_80045340[];
extern u8 D_800DEC50[];
extern u8 D_800DEC70[];
extern u8 D_800DE39C[];
extern u8 D_80025B10[];
extern u8 D_800DEB70[];
extern u8 D_800DEDB0[];
void func_s44_80025094(u8 *p, UnkVec *b, u8 *c, u8 *d);
UnkNode *func_s44_80025270(u8 *p, UnkVec *q, s32 i);
void func_s44_80024A34(u8 *p, UnkVec *q, u8 *c);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
s32 func_800A44E0(u16, u16, s16, u32);
s32 func_800BCB04(s32, s32, s16);
void func_800A56E0(s32);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_800DDC40[];
extern s32 D_8008346C;
s32 func_800647A0(s32);
s32 func_80065F90(s32, s32);
void func_s44_80024C0C(u8 *p, UnkVec *b, u8 *c, u8 *d);
UnkNode *func_s44_80024804(u8 *w, UnkVec *r, s32 i);
UnkNode *func_s44_80024E80(u8 *p, UnkVec *q);

void func_s44_80024024(u8 *p)
{
    u8 *g;

    g = (u8 *)&D_80083178;
    if (*(s16 *)(p + 0x24) > 0)
    {
        *(s16 *)(g + 0x98) += (*(s16 *)(p + 0x26) - *(s16 *)(g + 0x98)) / *(s16 *)(p + 0x24);
    }
    *(s16 *)(p + 0x24) = *(u16 *)(p + 0x24) - 1;
    if (*(s16 *)(p + 0x24) < -0x80)
    {
        *(s16 *)(p + 0x24) = -0x80;
    }
    if (D_800E3D20 != 0)
    {
        D_800E3D20 -= 1;
    }
    *(s16 *)(p + 0x4) = *(u16 *)(*(u8 **)(p + 0xC) + 0x2);
    *(s16 *)(p + 0x6) = *(u16 *)(*(u8 **)(p + 0xC) + 0x6);
    *(s16 *)(p + 0x8) = *(u16 *)(*(u8 **)(p + 0xC) + 0xA);
    *(s16 *)(g + 0xA4) += (*(s16 *)(p + 0x4) - *(s16 *)(g + 0xA4)) >> 2;
    *(s16 *)(g + 0xA6) += (*(s16 *)(p + 0x6) - *(s16 *)(g + 0xA6)) >> 2;
    if (D_80025B1C != 0)
    {
        *(s16 *)(g + 0xA8) += (*(s16 *)(p + 0x8) - *(s16 *)(g + 0xA8)) >> 2;
    }
    D_80025B1C = *(s16 *)(g + 0xA8) != *(s16 *)(p + 0x8);
    *(s16 *)(g + 0x94) = *(s16 *)(g + 0x94) >> 2;
    *(s16 *)(g + 0x96) = *(s16 *)(g + 0x96) >> 2;
}

/* Install func_s44_80024024 as D_80083178's +0xB4 camera step and fill its
   request record at +0xB8: point the record's own +0x0 at +0x4, park the
   caller's mover at +0xC, clear +0x28 and +0x18, set the mode word at +0x14
   to 0xD, store the frame count at +0x26 and the bearing at +0x24, and
   return a two-bit result -- bit 0 when the mover CHANGED, bit 1 when the
   mode was not already 9 -- publishing the bearing into D_800E3D20 when bit
   0 is set. The first parameter is UNUSED here.
   30 words, 0 differing, FIRST COMPILE.
   NO CARVED SIBLING AND NO RESIDENT TWIN BY EITHER TOOL: dngtwin's only hit
   is the 1.000 clone func_s44_8002416C, still an INCLUDE_ASM. Found the same
   way as the step above -- off D_80083178 -- in the TAIL of
   csrc/ovl_dungeon_800C2824.c's landed func_800C77D0 (52 words), whose first
   30 words are a pair of abs range tests this copy does not have. The other
   deltas: 0xD at +0x14 where the resident writes 9 (the TEST is `!= 9' in
   both), and no `*(u32 *)(p + 0x34) = a' store, which is why the first
   parameter is dead and why the step above has no +0x34 block.
   Of func_800C77D0's facts:
     - HOLDS: TWO locals, `s' for the base and `q = s + 0xB8' for the record.
       Folding `s' away and spelling the three base-relative sites off `q'
       is 29 words / 25 aligned differences.
     - HOLDS as to the THIRD parameter, INVERTED as to the claim that the
       callee cannot see it. It says the third is `s16' and the fourth `s32'
       but that both are stored with `sh' so the callee is 0 differing either
       way. The fourth is indeed FREE here (30 words / 0 as `s16' or `s32'),
       but the third is NOT: as `s32 c' it is 29 words / 6 aligned
       differences, because `D_800E3D20 = c' adds an `sb' use and the
       parameter copy into $t1 goes with it.
   This copy's own measured facts: the +0x24 store must be the LAST statement,
   after the D_800E3D20 gate -- moved ahead of it, 29 words / 2 aligned
   differences -- and the 0xD store must follow the `!= 9' test, 26 words / 28
   the other way. 
   THIS COPY'S OWN CONSTANT: 0xd -> 0xc.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
s32 func_s44_8002416C(void *a, u8 *b, s16 c, s32 d)
{
    u8 *s;
    u8 *q;
    s32 r;

    s = (u8 *)&D_80083178;
    q = s + 0xB8;
    r = (*(u32 *)(q + 0xC) != (u32)b);
    *(u32 *)(q + 0xC) = (u32)b;
    *(u32 *)(q + 0x28) = 0;
    *(u32 *)(s + 0xB8) = (u32)(s + 0xBC);
    if (*(u32 *)(q + 0x14) != 9)
    {
        r |= 2;
    }
    *(u32 *)(q + 0x14) = 0xc;
    *(u32 *)(q + 0x18) = 0;
    *(s16 *)(q + 0x26) = d;
    *(u32 *)(s + 0xB4) = (u32)func_s44_80024024;
    if (r & 1)
    {
        D_800E3D20 = c;
    }
    *(s16 *)(q + 0x24) = c;
    return r;
}

/* A thrown sprite's per-frame step: bump the +0x14 counter on the node at the
   actor's +0x0 and the actor's own +0x4 frame cursor, hand the renderable to
   func_800478B8, then advance the mover by the bearing at +0xA scaled by the
   span still to run (+0x8 minus the cursor) -- cosine into +0x0, sine into
   +0x4 -- drop it by 0x20000 in +0x8, and retire the actor once the cursor
   passes the limit at +0x6.
   66 words, 0 differing, FIRST COMPILE.
   NO TWIN. dngtwin's best is 0.700 and 12 words short. What found the shape
   was the shared TAIL, which func_s44_8002457C two listings up had already
   landed in this unit (the `p - 0x2' retire flag plus D_800814A0), leaving
   only the two trig terms to read off the listing; the `>> 4' trig idiom is
   csrc/ovl_dng_s26.c's func_s26_800240D8 and csrc/ovl_dng_b44.c's.
   MEASURED FACTS:
     - THE 0x20000 GRAVITY STORE IS THE THIRD OF THE THREE, not the second.
       Between the two trig terms it is 66 words / 20 aligned differences,
       which is where the target's `lui $a0, 0xFFFE' / `lw 0x8($s0)' pair in
       the second `mult' delay comes from -- source order +0x0, +0x4, +0x8,
       and sched1 hoists the gravity load and store INTO that delay.
     - THE SPAN IS WRITTEN OUT AT BOTH SITES AND MUST NOT BE A LOCAL. An
       `s32 t = (*(s16 *)(p + 0x8) - *(s16 *)(p + 0x4)) << 8;' hoisted above
       the first trig call is 63 words / 59 aligned differences: the `lh'
       pair and `sll' batch once, the second `mult' loses its delay filler
       and the first call's return no longer coalesces.
     - THE GATE IS `+0x4 > +0x6', NOT `+0x6 < +0x4'. The same `slt' either
       way, but the operand EVALUATION order flips the two `lh's: 66 words /
       2 aligned differences.
   Measured and FREE at 66 words / 0 differing, so neither is why this lands:
   the front bump written as a struct member reference (which func_s44_8002457C
   below needs and this one does not -- no store intervenes here), and reading
   the +0x4 cursor back through `*(s16 *)' rather than `*(u16 *)' for the
   `+ 1'. */
void func_s44_800241E4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)(*(u8 **)(p + 0x0) + 0x14) = *(u16 *)(*(u8 **)(p + 0x0) + 0x14) + 1;
    *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
    func_800478B8(c);
    b->unk_00 += (func_80064584(*(s16 *)(p + 0xA)) >> 4) * ((*(s16 *)(p + 0x8) - *(s16 *)(p + 0x4)) << 8);
    b->unk_04 += (func_800644B8(*(s16 *)(p + 0xA)) >> 4) * ((*(s16 *)(p + 0x8) - *(s16 *)(p + 0x4)) << 8);
    b->unk_08 += -0x20000;
    if (*(s16 *)(p + 0x4) > *(s16 *)(p + 0x6))
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212 node that carries this effect's thrown sprite: the step
   func_s44_800241E4 above as its +0x10, the caller's actor saved in the
   payload's first word, the throw's frame cursor/limit/span laid in
   +0x24..+0x28 as 0, 8, 8, the bearing copied off the request's +0x2A,
   a 0x30/0x30/0xC0 tint and the 0x7DCF sprite on the dresser slot with its
   +0x14 0xC and 0x100 bits and +0x10 0x20 bit, D_800DEC70 through
   func_8003DB94, D_80045340 through func_8004491C, and the caller's whole
   six-word mover copied into the node's +0x08 record. Returns the node, or
   NULL when the allocator refuses.
   76 words, 0 differing.
   TWIN: the landed func_s27_800241F8 in csrc/ovl_dng_s27.c (dngtwin 0.805,
   73 words), which the callee-set grep also returns -- of
   `D_80045340 D_800DEC70 func_8003DB94 func_8003FC64 func_8004491C' it is one
   of seven landed bodies with all five and the only one whose frame and save
   list match this listing (-0x28, s0/s3/s2/ra/s1).
   ITS ONE STATED FACT HOLDS, and is worth more here than on the twin: THE
   NULL EXIT IS AN `else' ARM AFTER THE GATE, NOT AN EARLY RETURN. Written as
   `if (n == NULL) { return NULL; }' ahead of the body this is 77 words and 63
   aligned differences, where the twin measured the same change at 5 -- this
   body's gate has a six-word block copy behind it, so jump.c's swap moves far
   more than the twin's.
   THIS BODY'S OWN FACTS:
     - THE SIX-WORD MOVER COPY IS A STRUCT ASSIGNMENT, `*n->unk_08 = *q'.
       Written out as six member assignments it is 86 words / 37 aligned
       differences: gcc's block move emits lw/lw/lw/lw then sw/sw/sw/sw then
       lw/lw/sw/sw, and six separate statements interleave load and store
       instead.
     - THE PAYLOAD NEEDS A POINTER LOCAL for the four +0x24..+0x2A stores.
       Spelled off the node with folded displacements
       (`*(s16 *)((u8 *)n + 0x24)' and so on) it is 76 words / 18 aligned
       differences -- the target computes `addiu $v1, $s1, 0x20' once and
       displaces off it.
     - THE TINT BYTES GO 0xD, 0xC, 0xE. Descending (0xE, 0xD, 0xC, the twin's
       order) is 76 words / 5 aligned differences: the 0xC0 is materialised
       first and the shared 0x30 second, where the target does the reverse.
   Measured and FREE at 76 words / 0 differing, so it is NOT why this lands:
   the +0x20 actor store written off the payload local (`*(u8 **)w = v') as
   well as off the node. */
UnkNode *func_s44_800242EC(u8 *v, UnkVec *q, u8 *c)
{
    UnkNode *n;
    u8 *m;
    u8 *w;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s44_800241E4;
        w = n->payload;
        *(u8 **)((u8 *)n + 0x20) = v;
        *(s16 *)(w + 0x4) = 0;
        *(s16 *)(w + 0x6) = 8;
        *(s16 *)(w + 0x8) = 8;
        *(s16 *)(w + 0xA) = *(u16 *)(c + 0x2A);
        m = (u8 *)n->unk_0C;
        m[0xD] = 0x30;
        m[0xC] = 0x30;
        m[0xE] = 0xC0;
        *(s16 *)(m + 0x12) = 0x7DCF;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x20;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8003DB94(m, D_800DEC70, 0);
        *(s16 *)(m + 0x1E) = 0x1000;
        *(s16 *)(m + 0x1C) = 0x1000;
        func_8004491C(n, (u32)D_80045340);
        *n->unk_08 = *q;
    }
    else
    {
        return NULL;
    }
    return n;
}

/* The damage-number step this bank installs from func_s44_8002452C: bump the
   +0x14 counter on the node at the actor's +0x0, take a 0x10-byte TILE off
   D_80083160.unk_00's +0x8D0 primitive cursor, lay it out 0x140 x 0xE0 at
   (0, 0), tint it by the frames remaining at the record's +0x5 (0xC0 blue
   against a red and green of -0x40 - 0x30 per frame), hang it on the +0xB0 OT
   row semi-transparent, and retire the actor when that counter decrements to
   zero.
   68 words, 0 differing.
   TWIN: the landed func_8009BE2C in csrc/ovl_dungeon.c (84 words). dngtwin
   and xsibs both find NOTHING for this one at any ratio -- a size ratio of
   68 to 84 is past what shape matching sees -- and the CALLEE-SET grep found
   it in one shot and alone: of `AddPrim D_800814A0 D_80083160 SetTile', it is
   the only landed body with all four, and its frame and save list are
   byte-identical to this listing's (-0x28, s3/ra/s2/s1/s0). It is this body
   with the tint computed from a divide and an extra 0xC-byte tail primitive.
   OF ITS TWO STATED FACTS, ONE HOLDS AND ONE IS FREE:
     - HOLDS, and is the whole 6-word difference: THE TINT IS A LOCAL. With
       the expression written out at both of `q[0x5]' and `q[0x4]' this is 74
       words / 56 aligned differences, because the `sb' to `q + 0x5' is a
       store cse cannot prove disjoint from `c + 0x5', so the `lb' and the
       whole 0x30 multiply chain are emitted TWICE. As a chained
       `q[0x5] = q[0x4] = <expr>' it is 68 words / 18.
     - FREE HERE, where the twin priced it at 83 words of 84 and 36 aligned
       differences: `Unk80083160 *g = &D_80083160;' as a pointer local. Both
       spellings are 68 words / 0 differing. This body reads `unk_00' at only
       two sites against the twin's four, and at two the `lui'-in-$s2 /
       `lw %lo' pair the twin rejected is what the target has anyway.
     - And its WIDTH does not carry: the twin needs `u16' specifically; here
       `s32', `u16' and `u8' are all 68 words / 0 differing.
   THIS BODY'S OWN FACT, worth 16 differing: THE TINT IS COMPUTED BEFORE THE
   `q[0x6] = 0xC0' STORE. After it, 68 words / 11 aligned differences -- the
   `lb' of `c + 0x5' cannot hoist back above a store to `q + 0x6' it cannot be
   proved disjoint from, so the whole block schedules two slots late and the
   SetTile argument copy and the +0xE store move with it.
   And the retire counter is `s8': as `--*(u8 *)(c + 0x5)' it is 68 words /
   1 differing, an `andi 0xff' where the target zero-tests with `sll 24'. */
void func_s44_8002441C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *q;
    s32 v;

    *(s16 *)(*(u8 **)(p + 0x0) + 0x14) = *(u16 *)(*(u8 **)(p + 0x0) + 0x14) + 1;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0x10);
    *(s16 *)(q + 0xC) = 0x140;
    *(s16 *)(q + 0x8) = 0;
    *(s16 *)(q + 0xA) = 0;
    *(s16 *)(q + 0xE) = 0xE0;
    v = -0x40 - *(s8 *)(c + 0x5) * 0x30;
    q[0x6] = 0xC0;
    q[0x5] = v;
    q[0x4] = v;
    SetTile((TILE *)q);
    q[0x7] |= 2;
    AddPrim(D_80083160.unk_00 + 0xB0, q);
    if (--*(s8 *)(c + 0x5) == 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn this bank's 0x212 node for the effect func_s44_8002441C draws: install
   that as the node's +0x10 per-frame step, set the dresser slot's +0x5 mode
   byte to 4, and save the caller's one argument in the payload's first word.
   20 words, 0 differing, FIRST COMPILE.
   NO TWIN USED. dngtwin's best is 0.711 and every candidate is LONGER; the
   body came straight off the listing against the func_8003FC64(0x212) /
   `+0x10 install' skeleton that csrc/ovl_dng_b10.c's func_b10_80170CF8
   header names, which is the same four statements with a payload grown on it.
   The `lw 0xC($a0)' appearing BEFORE the `sw 0x10($a0)' in the listing is
   sched1 filling the beqz delay, not source order -- the three stores are in
   source order (+0x10, slot +0x5, payload +0x0). */
void func_s44_8002452C(u8 *a)
{
    UnkNode *n;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s44_8002441C;
        ((UnkSlot *)n->unk_0C)->unk_05 = 4;
        *(u8 **)n->payload = a;
    }
}

/* The 0x212 sprite's own step function, this bank's two-counter variant: bump
   the +0x14 counter on the node at the actor's +0x0, hand the renderable to
   func_800478B8, and when its +0x14 reports 0x6000 retire the owning actor,
   set the same bit in D_800814A0, and bump a SECOND counter at +0x18 on that
   same node (re-read, not carried across the call).
   33 words, 0 differing.
   TWIN: the landed func_s60_8002548C in csrc/ovl_dng_s60.c (dngtwin 0.875,
   31 words), itself func_s10_80024374 plus the front bump. Its note about
   D_800814A0 being the PLAIN scalar rather than an array alias holds here and
   is what gives the `lui'/`lw %lo' pair. The whole delta is the +0x18 bump in
   the arm, and it is NOT additive: spelled the twin's way -- a plain
   `*(s16 *)(*(u8 **)(p + 0x0) + 0x18)' -- it is 37 words / 18 aligned
   differences, two `nop's long, because the reload of the node pointer will
   not hoist above the `sh' to `p - 0x2' and sched1 has nothing to fill either
   load delay with.
   TWO THINGS ARE BOTH REQUIRED, measured separately:
     - the +0x18 access must be a struct MEMBER REFERENCE, which is what sets
       MEM_IN_STRUCT_P so true_dependence clears it against the non-struct
       `p - 0x2' store and the load hoists three slots. Through the SAME
       `UnkSlot *' local but cast back out --
       `*(s16 *)((u8 *)q + 0x18)' -- it is 35 words / 16, exactly the plain
       spelling's residue, so the pointer's TYPE buys nothing on its own.
     - the node must go into a POINTER LOCAL. The member reference written
       twice off `*(UnkSlot **)(p + 0x0)' with no local is 37 words / 18, the
       plain figure again.
   Measured and FREE at 33 words / 0 differing, so neither is why this lands:
   the FRONT bump written as a struct member reference too, and the `p - 0x2'
   flag set as `*(s16 *)(p - 0x2) = *(u16 *)(p - 0x2) | 0x8000' instead of the
   compound `|='. */
void func_s44_8002457C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)(*(u8 **)(p + 0x0) + 0x14) = *(u16 *)(*(u8 **)(p + 0x0) + 0x14) + 1;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        UnkSlot *q;

        q = *(UnkSlot **)(p + 0x0);
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        q->unk_18 = q->unk_18 + 1;
    }
}

/* Spawn the 0x212 node that carries this effect's smoke sprite: the step
   func_s44_8002457C below as its +0x10, the caller node's own +0x0 link saved
   in the payload's first word and the payload halfword at +0x24 cleared, a
   flat 0x808080 tint word and the 0x7DCF sprite on the dresser slot with its
   +0x14 0xC and 0x100 bits and +0x10 0x60 bit, D_800DEC50 through
   func_8003DB94, D_80045340 through func_8004491C, and the caller's whole
   six-word mover copied into the node's +0x08 record. Returns the node, or
   NULL when the allocator refuses.
   68 words, 0 differing.
   THIS IS func_s44_800242EC ABOVE, five listings up in this unit, with
   func_8003FD64 and a second argument in place of func_8003FC64, one tint
   WORD in place of three bytes, 0x60 for 0x20 on +0x10, no throw cursor
   block, and the payload's first word taken from the caller's +0x0 instead of
   the caller itself. dngtwin and xsibs find NOTHING for it at any ratio; it
   cost two measurements once 242EC was in the file, which is the whole case
   for doing a unit in one copy.
   BOTH OF 242EC's FACTS CARRY: the NULL exit as an `else' arm (as an early
   return, 68 words / 49 aligned differences) and the mover copy as a struct
   assignment.
   ITS OWN FACT, and the one that costs the frame: THE PAYLOAD +0x20 COPY MUST
   BE WRITTEN BEFORE THE +0x24 CLEAR AND BEFORE `m' IS LOADED. With `m' loaded
   first this is 70 words / 69 aligned differences and FIVE callee-saved
   registers against the target's four, because `p' then has to stay live past
   the dresser-slot load and cannot hand its register to `m'. With the +0x24
   clear first but `m' still last it is 70 words / 69 as well: the `lw' of
   `p + 0x0' cannot hoist above a store to `n + 0x24' it cannot be proved
   disjoint from, so `p' survives the same way. The target reuses $s0 for
   both, which is what the four-register prologue is. */
UnkNode *func_s44_80024600(u8 *p, UnkVec *q)
{
    UnkNode *n;
    u8 *m;

    n = func_8003FD64(0x212, p - 0x20);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s44_8002457C;
        *(u32 *)((u8 *)n + 0x20) = *(u32 *)(p + 0x0);
        *(s16 *)((u8 *)n + 0x24) = 0;
        m = (u8 *)n->unk_0C;
        *(u32 *)(m + 0xC) = 0x808080;
        *(s16 *)(m + 0x12) = 0x7DCF;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x60;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8003DB94(m, D_800DEC50, 0);
        *(s16 *)(m + 0x1E) = 0x1000;
        *(s16 *)(m + 0x1C) = 0x1000;
        func_8004491C(n, (u32)D_80045340);
        *n->unk_08 = *q;
    }
    else
    {
        return NULL;
    }
    return n;
}

/* A spiralling sprite's per-frame step: bump the +0x14 counter on the node at
   the actor's +0x0, then advance the mover on all three axes from the two
   angles at +0xE and +0x10 -- cos(+0xE)*cos(+0x10) and cos(+0xE)*sin(+0x10)
   scaled by 0x1C into +0x0 and +0x4, sin(+0xE) scaled by 0x1C00 into +0x8.
   61 words, 0 differing, FIRST COMPILE.
   NO TWIN AT ANY RATIO by either tool, and the callee-set grep is useless
   here -- `func_800644B8 func_80064584' alone matches 89 landed bodies. What
   landed it was the two bodies already in this unit: func_s44_800241E4's
   front bump and `>> 4' trig idiom verbatim, with three terms instead of two
   and a 0x1C/0x1C00 scale in place of the `<< 8' span. The `* 0x1C' is the
   target's `sll 3' / `subu' / `sll 2' and `* 0x1C00' its `sll 3' / `subu' /
   `sll 10'; writing either as a shift chain is not needed.
   First compile, so nothing in it is measured -- except that the THREE
   REPEATED TRIG CALLS ARE REAL: func_800644B8(+0xE) is called twice and
   func_80064584(+0xE) once, five calls for three terms, and the target has
   all five. */
void func_s44_80024710(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)(*(u8 **)(p + 0x0) + 0x14) = *(u16 *)(*(u8 **)(p + 0x0) + 0x14) + 1;
    b->unk_00 += (func_800644B8(*(s16 *)(p + 0xE)) >> 4) * (func_800644B8(*(s16 *)(p + 0x10)) >> 4) * 0x1C;
    b->unk_04 += (func_800644B8(*(s16 *)(p + 0xE)) >> 4) * (func_80064584(*(s16 *)(p + 0x10)) >> 4) * 0x1C;
    b->unk_08 += (func_80064584(*(s16 *)(p + 0xE)) >> 4) * 0x1C00;
}

/* Spawn one of the three 0x212 trailing sparks the burst above lays down,
   indexed by `i': the step func_s44_80024710 above as its +0x10, the caller
   payload's first word copied into the node's, a tint that darkens with the
   index (-0x60 - 0x40 per step on red and green, 0xC0 blue), the 0x7DCF
   sprite, this bank's own D_80025B10 parked at the slot's +0x8, the +0x14 0xC
   and 0x100 bits and +0x10 0x20 bit, a random 0..0xFFF spin, a scale that
   grows with the index (0x500 + 0x100 per step) both ways, D_80045340 through
   func_8004491C, the caller's whole six-word mover copied into the node's
   +0x08 record, the two aim angles copied from the caller payload's +0xE and
   +0x10, and the mover pulled back along all three axes by (i + 2) times the
   trig of those angles. Returns the node, or NULL.
   140 words, 0 differing.
   NO TWIN AT ANY RATIO. It is func_s44_80024E80 above with the aim solution
   replaced by three trig terms taken straight from func_s44_80024710's, so
   both halves were already in this unit.
   THREE FACTS, and the first two are the whole fight. All prices are measured
   against this body at 0 differing.
     - THE INDEX IS NARROWED TO 16 BITS EXACTLY ONCE, INTO AN `s32' LOCAL, AND
       THE PARAMETER IS `s32'. The index is used at three sites in THREE
       DIFFERENT cse blocks (the tint, the scale past the `rand' modulo's
       `bgez', and the `i + 2' past the dress call), so three separate casts
       cannot be shared -- and two of them are then deleted outright by
       combine, because `sb' and `sh' truncate their inputs and the
       sign-extension of a value about to be truncated is dead. The target
       computes `sll 16' / `sra 16' ONCE and carries the result in a seventh
       callee-saved register. Measured:
         `s16 i' as the parameter with no local at all (prototype and
         func_s44_80024E80's argument changed to match): 139 words / 35
         aligned differences, with NO `sll'/`sra' anywhere;
         `s32 i' with `((s16)i ...)' written out at the three sites: 138 / 40;
         `s32 i' with `s16 k = i;' instead of `s32 k = (s16)i;': 138 / 40.
       An old-style K&R `s16 i' definition does not help either. And
       func_s44_80024E80 is 133 words / 0 differing under BOTH prototypes --
       with `s16' the caller truncates from the prototype, with `s32' it
       truncates from the explicit `(s16)i' in the argument -- so the caller
       cannot tell you which it is.
     - THE PAYLOAD POINTER MUST BE TAKEN AS THE GATE'S FIRST STATEMENT. Taken
       at its first USE instead, after the dress block, its live range no
       longer overlaps the `r' parameter's, `r's register is handed on to it,
       and the body is 137 words / 45 aligned differences with SEVEN
       callee-saved registers against the target's eight. The eight values
       that must be live across the trig calls are what the -0x30 prologue is.
     - `return n' GOES INSIDE THE GATE, with `return NULL' as the
       fall-through. As an `else' arm with a shared `return n' after -- which
       is what func_s44_800242EC and func_s44_80024E80 above both need -- it is
       141 words / 9 aligned differences: the `addu $v0, $s4, $zero' can no
       longer be scheduled up into the last `mult's latency and a `j' appears
       over the NULL block. `return n' inside the gate WITH an
       `else { return NULL; }' is also 140 / 0, so only the position of the
       `return n' is load-bearing, not which form the NULL exit takes. */
UnkNode *func_s44_80024804(u8 *w, UnkVec *r, s32 i)
{
    UnkNode *n;
    UnkVec *v;
    u8 *m;
    u8 *u;
    s32 k;

    n = func_8003FD64(0x212, w - 0x20);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s44_80024710;
        u = n->payload;
        *(u32 *)((u8 *)n + 0x20) = *(u32 *)(w + 0x0);
        k = (s16)i;
        m = (u8 *)n->unk_0C;
        m[0xD] = -0x60 - (k << 6);
        m[0xC] = -0x60 - (k << 6);
        m[0xE] = 0xC0;
        *(s16 *)(m + 0x12) = 0x7DCF;
        *(u32 *)(m + 0x8) = (u32)D_80025B10;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x20;
        *(u16 *)(m + 0x14) |= 0x100;
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1E) = (k << 8) + 0x500;
        *(s16 *)(m + 0x1C) = (k << 8) + 0x500;
        func_8004491C(n, (u32)D_80045340);
        v = n->unk_08;
        *v = *r;
        *(s16 *)(u + 0xE) = *(u16 *)(w + 0xE);
        *(s16 *)(u + 0x10) = *(u16 *)(w + 0x10);
        v->unk_00 -= ((func_800644B8(*(s16 *)(u + 0xE)) >> 4) * (func_800644B8(*(s16 *)(u + 0x10)) >> 4) *
                      (k + 2))
                     << 3;
        v->unk_04 -= ((func_800644B8(*(s16 *)(u + 0xE)) >> 4) * (func_80064584(*(s16 *)(u + 0x10)) >> 4) *
                      (k + 2))
                     << 3;
        v->unk_08 -= ((func_80064584(*(s16 *)(u + 0xE)) >> 4) * (k + 2)) << 11;
        return n;
    }
    return NULL;
}

/* The burst's detonation: retire the three sparks hanging off the actor's
   +0x14..+0x1C by setting 0x8000 in each node's +0x1E (and in D_800814A0),
   re-dress the slot from D_800DEDB0 with a flat 0xC0 tint, 0x800 scale both
   ways and the +0x14 0xC / +0x10 0x20 bits, give it a random 0..0xFFF spin,
   then kick the mover off in a random direction -- two more `rand() % 0x1000'
   angles, cos/cos and cos/sin scaled by 0x10 into +0x0 and +0x4 and sin
   scaled by 0x1000 into +0x8 -- and spawn the smoke through
   func_s44_80024600 above.
   118 words, 0 differing.
   NO TWIN AT ANY RATIO. Landed off the listing; the three trig terms are
   func_s44_80024710's idiom again and func_s44_80024600 was already in the
   file.
   THE ONE FACT, worth 2 aligned differences: THE RETIREMENT LOOP IS INDEXED,
   NOT CURSORED. Written as `for (j = 0; j < 3; j++) { n = ((UnkNode **)(p +
   0x14))[j]; ... }' it is 118 words / 0; with an explicit `u8 *e' cursor and
   `*(UnkNode **)(e + 0x14)', 118 / 2. The difference is where the hoisted
   `lui $a3, %hi(D_800814A0)' lands: with an indexed source the cursor is a
   derived induction variable that loop.c's strength reduction creates AT
   `loop_start', so the invariant `lui' is emitted BETWEEN `j = 0' and the
   cursor's initialisation, which is the target's order; a source-level cursor
   is an ordinary preheader assignment and the `lui' goes after both.
   `e = p;' hoisted out of the `for' is 118 / 7 and a `while' loop with the
   cursor is 118 / 2, so this is the induction variable and not the statement
   order.
   And the two `rand() % 0x1000' angles are `s16' LOCALS: the modulo result is
   an `s32' expression, so the assignment into an `s16' is a real narrowing
   and gives the target's `sll 16' / `sra 16' pair on each -- unlike
   func_s44_80024804's index, where the value arrives already narrow. The
   third `rand() % 0x1000' goes straight to an `sh' and needs no local.
   Not separately measured: the widths were right on the first compile. */
void func_s44_80024A34(u8 *p, UnkVec *q, u8 *c)
{
    UnkNode *n;
    s32 j;
    s16 a;
    s16 b;

    for (j = 0; j < 3; j++)
    {
        n = ((UnkNode **)(p + 0x14))[j];
        if (n != NULL)
        {
            n->unk_1E |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
    func_8003DB94(c, D_800DEDB0, 0);
    c[0xE] = 0xC0;
    c[0xD] = 0xC0;
    c[0xC] = 0xC0;
    *(s16 *)(c + 0x1E) = 0x800;
    *(s16 *)(c + 0x1C) = 0x800;
    *(u16 *)(c + 0x14) |= 0xC;
    *(u16 *)(c + 0x10) |= 0x20;
    *(s16 *)(c + 0x1A) = rand() % 0x1000;
    a = rand() % 0x1000;
    b = rand() % 0x1000;
    q->unk_00 += ((func_800644B8(a) >> 4) * (func_800644B8(b) >> 4)) << 4;
    q->unk_04 += ((func_800644B8(a) >> 4) * (func_80064584(b) >> 4)) << 4;
    q->unk_08 += (func_80064584(a) >> 4) << 12;
    func_s44_80024600(p, q);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s44", func_s44_80024C0C);

/* Spawn the 0x212 node that carries this effect's homing burst: the step
   func_s44_80024C0C below as its +0x10, the caller node's +0x0 link in the
   payload's first word and +0x24 cleared, a flat 0x80 tint, the 0x7DCF
   sprite, D_800DE39C parked at the slot's +0x8, the +0x14 0x100 bit, a random
   0..0xFFF spin at +0x1A, 0x400 scale both ways, D_80045340 through
   func_8004491C; then the caller's whole six-word mover is copied into the
   node's +0x08 record and its last three words overwritten from the actor's
   +0x20..+0x28 target point, the two aim angles are solved from the delta
   (func_800647A0 of the squared horizontal distance into func_80065F90
   against the height for the pitch at +0xE, and the raw delta for the yaw at
   +0x10, with +0x12 flagged when the x delta is negative), and three
   func_s44_80024804 results are laid into +0x14..+0x1C. Returns the node, or
   NULL when the allocator refuses.
   133 words, 0 differing.
   NO TWIN AT ANY RATIO; the callee-set grep's eight 4-of-7 hits are all
   -0x38 frames against this listing's, which is -0x38 with NINE callee-saved
   registers, so none of them can lend an allocation fact. It is
   func_s44_80024600 above, in this unit, with a bigger dress block and the
   aim solution grown on the end.
   func_s44_800242EC's two facts carry again: the NULL exit as an `else' arm
   and the mover copy as a struct assignment.
   ITS OWN FACT, worth 3 words and 9 aligned differences: THE LOOP COUNTER IS
   `s32', NOT `s16', and the `s16' is on func_s44_80024804's PARAMETER. As
   `s16 i' the body is 136 words -- a `sll 16' / `sra 16' pair plus a `move'
   truncating the counter at every bump -- where the target truncates ONCE, at
   the argument, which is what its `sll $a2, $s0, 16' / `sra $a2, $a2, 16' in
   the call's dress is. And the two initialisers go `i' THEN `e': with
   `e = w;' as its own statement ahead of the `for' it is 133 words / 2
   aligned differences, the two `move's exchanged, so they are written
   `for (i = 0, e = w; ...)'. */
UnkNode *func_s44_80024E80(u8 *p, UnkVec *q)
{
    UnkNode *n;
    UnkVec *r;
    u8 *m;
    u8 *w;
    u8 *e;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 i;

    n = func_8003FD64(0x212, p - 0x20);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s44_80024C0C;
        w = n->payload;
        *(u32 *)((u8 *)n + 0x20) = *(u32 *)(p + 0x0);
        *(s16 *)(w + 0x4) = 0;
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        *(s16 *)(m + 0x12) = 0x7DCF;
        *(u32 *)(m + 0x8) = (u32)D_800DE39C;
        *(u16 *)(m + 0x14) |= 0x100;
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1E) = 0x400;
        *(s16 *)(m + 0x1C) = 0x400;
        func_8004491C(n, (u32)D_80045340);
        r = n->unk_08;
        *r = *q;
        r->unk_0C = *(s32 *)(p + 0x20);
        r->unk_10 = *(s32 *)(p + 0x24);
        r->unk_14 = *(s32 *)(p + 0x28);
        dx = *(s32 *)(p + 0x20) - q->unk_00;
        dy = *(s32 *)(p + 0x24) - q->unk_04;
        dz = *(s32 *)(p + 0x28) - q->unk_08;
        *(s16 *)(w + 0xE) =
            func_80065F90(func_800647A0(((dx >> 12) * (dx >> 12) + (dy >> 12) * (dy >> 12)) << 4), dz >> 4);
        *(s16 *)(w + 0x10) = func_80065F90(dx >> 4, dy >> 4);
        if (dx < 0)
        {
            *(s16 *)(w + 0x12) = 1;
        }
        for (i = 0, e = w; i < 3; i++)
        {
            *(u32 *)(e + 0x14) = (u32)func_s44_80024804(w, r, (s16)i);
            e += 4;
        }
    }
    else
    {
        return NULL;
    }
    return n;
}

/* The flying sprite's per-frame step and its three-state machine: bump the
   +0x14 counter on the node at the actor's +0x0, hand the renderable to
   func_800478B8, run the +0x6 frame counter and spin the slot's +0x1A by
   0x200, then dispatch on +0x4. State 0 turns the +0xA speed up by 1 or 2 on
   alternate frames, brightens the slot's three tint bytes by 3, grows it to
   +0x6 << 5 both ways and lifts the mover's +0xA by 2, THEN FALLS THROUGH
   into state 1, which advances to the next state once +0x6 reaches 0x20.
   State 2 waits for +0x6 to reach the limit at +0x8, then spawns
   func_s44_80024E80's burst and retires the actor. Every path that does not
   retire runs the shared tail, which steers the mover's +0x2/+0x6 by the
   bearing at +0xC scaled by +0xA.
   119 words, 0 differing, FIRST COMPILE.
   NO TWIN AT ANY RATIO. Landed off the listing with the three devices this
   unit had already paid for: func_s44_800241E4's front bump and `>> 4' trig
   idiom and func_s44_8002457C's D_800814A0 retire pair.
   TWO THINGS ARE WORTH WRITING DOWN, both read off the listing rather than
   measured:
     - IT IS A `switch', NOT AN if/else-if CHAIN. The target dispatches with
       `beq 1' / `slti 2' / `beq 0' / `beq 2' -- gcc's comparison TREE, which
       only a `switch' statement builds; three sequential compares is what the
       chain would give. Three cases is below the jump-table threshold, so
       this function owns no rodata.
     - CASE 0 FALLS THROUGH INTO CASE 1 with no `break', which is why state
       0's tail and state 1's gate share the one `.Ls44_80025190' label.
   Both repeated values in state 0 need a LOCAL -- the brightened tint byte
   and the `+0x6 << 5' scale are each stored to three and two slots -- because
   the first store is to the SLOT and the re-read would be from the ACTOR,
   two different base registers cse cannot prove disjoint. Not separately
   measured; this is the same mechanism func_s44_8002441C's comment prices at
   56 aligned differences. */
void func_s44_80025094(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 t;

    *(s16 *)(*(u8 **)(p + 0x0) + 0x14) = *(u16 *)(*(u8 **)(p + 0x0) + 0x14) + 1;
    func_800478B8(c);
    *(s16 *)(p + 0x6) = *(u16 *)(p + 0x6) + 1;
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x200;
    switch (*(s16 *)(p + 0x4))
    {
    case 0:
        if ((*(u16 *)(p + 0x6) & 1) != 0)
        {
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 2;
        }
        else
        {
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        t = c[0xE] + 3;
        c[0xE] = t;
        c[0xD] = t;
        c[0xC] = t;
        t = *(s16 *)(p + 0x6) << 5;
        *(s16 *)(c + 0x1E) = t;
        *(s16 *)(c + 0x1C) = t;
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) - 2;
    case 1:
        if (*(s16 *)(p + 0x6) >= 0x20)
        {
            *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
            *(s16 *)(p + 0x6) = 0;
        }
        break;
    case 2:
        if (*(s16 *)(p + 0x6) >= *(s16 *)(p + 0x8))
        {
            func_s44_80024E80(p, b);
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        break;
    }
    *(s16 *)((u8 *)b + 0x2) =
        *(u16 *)((u8 *)b + 0xE) + ((func_80064584(*(s16 *)(p + 0xC)) >> 4) * *(s16 *)(p + 0xA) >> 8);
    *(s16 *)((u8 *)b + 0x6) =
        *(u16 *)((u8 *)b + 0x12) + ((func_800644B8(*(s16 *)(p + 0xC)) >> 4) * *(s16 *)(p + 0xA) >> 8);
}

/* Spawn one of the eight 0x212 petals the flying sprite's state 2 lays down,
   indexed by `i': the step func_s44_80025094 above as its +0x10, the caller
   actor in the payload's first word, the petal's own cursor block at
   +0x24..+0x2C (0 frames, a 7-per-index limit at +0x28 and a 0x600-per-index
   bearing at +0x2C), the caller mover's last three words copied into
   +0x40..+0x48, a flat 0x20 tint and the 0x7DCF sprite with the +0x14 0xC and
   0x100 bits and +0x10 0x20 bit, D_800DEB70 through func_8003DB94, a random
   0..0xFFF spin, 0x400 scale both ways, D_80045340 through func_8004491C, the
   caller's whole six-word mover into the node's +0x08 record, and that
   record's first two words copied forward into its +0xC and +0x10.
   102 words, 0 differing, FIRST COMPILE.
   NO TWIN AT 0.62 OR BETTER, and the two 0.6x rows are both b-bank bodies of
   the wrong width. It compiled first time because it is the fourth spawner in
   this unit: func_s44_800242EC's `else'-arm NULL exit, struct-assignment mover
   copy and payload pointer local, func_s44_80024600's func_8003FD64 call
   shape, and func_s44_80024804's `s32' parameter with a single `k = (s16)i'
   local, all unchanged. That last one is why this is here as a fact rather
   than a guess: the two `sh' stores at +0x28 and +0x2C truncate, so three
   separate `(s16)i' casts would all be deleted by combine and the target's
   one `sll 16' / `sra 16' would be missing -- see func_s44_80024804's comment
   for the measured prices.
   First compile, so nothing in this body is separately measured. */
UnkNode *func_s44_80025270(u8 *p, UnkVec *q, s32 i)
{
    UnkNode *n;
    UnkVec *v;
    u8 *m;
    u8 *u;
    s32 k;

    n = func_8003FD64(0x212, p - 0x20);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s44_80025094;
        u = n->payload;
        k = (s16)i;
        *(u8 **)((u8 *)n + 0x20) = p;
        *(s16 *)(u + 0x8) = k * 7;
        *(s16 *)(u + 0x4) = 0;
        *(s16 *)(u + 0x6) = 0;
        *(s16 *)(u + 0xA) = 0;
        *(s16 *)(u + 0xC) = k * 0x600;
        *(u32 *)(u + 0x20) = *(u32 *)((u8 *)q + 0xC);
        *(u32 *)(u + 0x24) = *(u32 *)((u8 *)q + 0x10);
        *(u32 *)(u + 0x28) = *(u32 *)((u8 *)q + 0x14);
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x20;
        m[0xD] = 0x20;
        m[0xC] = 0x20;
        *(s16 *)(m + 0x12) = 0x7DCF;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x20;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8003DB94(m, D_800DEB70, 0);
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1E) = 0x400;
        *(s16 *)(m + 0x1C) = 0x400;
        func_8004491C(n, (u32)D_80045340);
        v = n->unk_08;
        *v = *q;
        v->unk_0C = v->unk_00;
        v->unk_10 = v->unk_04;
        return n;
    }
    return NULL;
}

/* THE ACTOR DRIVER, and the biggest body in this unit. Bumps the actor's own
   +0x10 frame counter and dispatches *(s16 *)(p + 0xA) through
   jtbl_s44_80024008; case 0 falls through into case 1; five of the seven arms
   end with the same two-statement advance (+0x10 = 0, +0xA += 1) which jump2
   cross-joins into one block; every arm leaves through the shared
   *(s16 *)(p + 0x14) = 0 after the switch.
   450 words, 0 differing.

   THIS FUNCTION OWNS jtbl_s44_80024008: 7 .word entries = 7 cases + 0 pad,
   dispatch bound `sltiu $v0, $v1, 0x7', 7 DISTINCT targets, and jtcheck.py
   agrees with the listing. The range-check `beqz' goes to .Ls44_80025AD8,
   which is NOT any of the seven entries, so there is no default-label padding
   and all seven are real arms -- the `beqz' grep settles it.

   THE TWIN, and it is nearly the whole answer: the landed func_s15_80024CD4 in
   csrc/ovl_dng_s15.c (421 words, 6-case table). dngtwin and xsibs both find
   NOTHING for this body at any ratio. What found it was a CALLEE SYMBOL --
   `grep -rn func_8003DE58 csrc/*.c' -- and its case 1 is line-for-line this
   one's: the same `o = s - 0x20', the same two-term `&&' gate on
   func_8003DE58's return and the slot's +0x14 0x8000 bit, the same three
   mover halfword copies with the `z - 0x40' else arm, the same
   `(**(u16 **)(p + 0x4) & 0x80)' break, and the same eight-step walk over
   D_8006CCD8 / D_8006CCE8.
   ALL FOUR OF ITS SETTLED SOURCE-SHAPE FACTS CARRY, and are unmeasured here
   because they were already right on the first compile: the func_800A44E0 cell
   masks are `& 0xFFFFFFC0' and not `& 0xFFC0'; the walk's output is `ox * 64'
   and not `(ox << 6)'; `xx' before `yy' and `ox = xx; oy = yy;' in that order
   at the loop tail; and the cursors are TWO pairs of locals, not one.
   Its dead `lui $v0, 0x8007' in the else-arm preheader -- a %hi of
   D_8006CCD8/D_8006CCE8 whose %lo has been rematerialised away -- appears here
   too, and comes for free from reusing the one `UnkVec *v' local for the
   then-arm pointer.

   FOUR FACTS OF THIS BODY'S OWN, all measured against it at 0 differing:
     - THE WALK'S OUTPUT VECTOR IS A STACK LOCAL REACHED THROUGH A POINTER.
       The twin writes into the actor at `p + 0x30'; this copy writes into a
       `UnkVec' on the stack, and it must go through `v = &vv;' with every
       access off `v'. Addressed directly as `vv' / `*(s16 *)((u8 *)&vv + K)'
       the body is 447 words and 47 aligned differences: the three halfword
       stores and the two read-backs go off `$sp' with folded displacements
       where the target computes `addiu $a1, $sp, 0x20' once and displaces off
       it, and the three missing words are that base and the two reloads it
       saves.
     - CASE 2's `8' IS AN `s16' LOCAL OF ITS OWN. As `s32 k' it is 449 words /
       5 aligned differences; reusing the `s32 i' loop counter for it is
       449 / 7. The missing word is a `nop': on the `s16' spelling the
       narrowing makes `k's `addiu $s0, $zero, 8' a separate HImode value, the
       second `(k - *(s16 *)(p + 0x10))' becomes destructive on it, and sched1
       then has nothing left to fill the first trig result's load delay with --
       which is the `nop' at 0x800257F0 that both wider spellings fill.
     - CASE 2's GRAVITY STORE IS THE THIRD OF THE THREE MOVER UPDATES, exactly
       as in func_s44_800241E4 above: +0x0, +0x4, then +0x8. Written second,
       between the two trig terms, it is 450 words / 8 aligned differences,
       because sched1 then cannot hoist the gravity load and store into the
       SECOND `mult's latency.
     - AND THE WALK'S OUTPUT STORES GO +0xC, +0x10, +0x14 ON `b'. With +0x14
       (the `- 0x200000') written second it is 452 words / 8 aligned
       differences: the `lui $v1, 0xFFE0' no longer materialises early enough
       to fill the slot ahead of the `lw 0x4($a1)', and two words are spent
       recovering.
   The third argument to func_s44_80025270 is cast `(s16)i' at the call site,
   because that callee's parameter is `s32' and narrows it itself -- see
   func_s44_80024804's comment for why. */
void func_s44_80025408(u8 *p, UnkVec *b, u8 *c)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *q2;
    UnkVec *v;
    u16 z;
    s32 i;
    s16 k;
    s32 h;
    s32 cx;
    s32 cy;
    s16 xx;
    s16 yy;
    UnkVec vv;
    s16 ox;
    u16 out[4];
    s16 oy;

    s = *(u8 **)(p + 0x0);
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0x16) = 0;
        *(s16 *)(p + 0x18) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0xE) = (*(u16 *)(s + 0x2A) >> 9) & 7;
        *(s32 *)(c + 0xC) = 0x808080;
        func_s44_8002416C(s - 0x20, (u8 *)b, 8, 0x300);
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
            b->unk_0C = v->unk_00;
            b->unk_10 = v->unk_04;
            b->unk_14 = v->unk_08 - (D_800DDC40[*(u8 *)(*(u8 **)(s + 0x60) + 0x13)] << 15);
        }
        else
        {
            q2 = *(u8 **)(o + 0xC);
            xx = q2[0x24];
            yy = q2[0x25];
            ox = xx;
            oy = yy;
            for (i = 0; i < 8; i++)
            {
                cx = xx;
                cy = yy;
                if ((s16)func_800A44E0((cx << 6) & 0xFFFFFFC0, (cy << 6) & 0xFFFFFFC0,
                                       *(s16 *)(s + 0x88), (s16)(*(u16 *)(p + 0xE) << 9)) != 0)
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
                ox = xx;
                oy = yy;
            }
            v = &vv;
            *(s16 *)((u8 *)v + 0x2) = ox * 64 + (D_8006CCD8[*(s16 *)(p + 0xE)] + 1) * 32;
            *(s16 *)((u8 *)v + 0x6) = oy * 64 + (D_8006CCE8[*(s16 *)(p + 0xE)] + 1) * 32;
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)b + 0xA);
            b->unk_0C = v->unk_00;
            b->unk_10 = v->unk_04;
            b->unk_14 = b->unk_08 - 0x200000;
        }
        func_s44_800242EC(p, b, s);
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 2:
        k = 8;
        b->unk_00 += ((func_80064584(*(s16 *)(s + 0x2A)) >> 4) * (k - *(s16 *)(p + 0x10))) << 8;
        b->unk_04 += ((func_800644B8(*(s16 *)(s + 0x2A)) >> 4) * (k - *(s16 *)(p + 0x10))) << 8;
        b->unk_08 += -0x20000;
        if (*(s16 *)(p + 0x10) == 5)
        {
            func_800A56E0(0x300);
            for (i = 0; i < 8; i++)
            {
                func_s44_80025270(p, b, (s16)i);
            }
        }
        if (*(s16 *)(p + 0x10) >= 9)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x10) < 0xD)
        {
            b->unk_08 += -0x20000;
        }
        if (*(s16 *)(p + 0x10) < 0x3C)
        {
            break;
        }
        func_s44_8002452C(p);
        if (*(u8 **)(s + 0x60) != NULL)
        {
            v = (UnkVec *)*(u8 **)(*(u8 **)(s + 0x60) - 0x18);
            b->unk_0C = (v->unk_00 - b->unk_00) / 32;
            b->unk_10 = (v->unk_04 - b->unk_04) / 32;
            b->unk_14 =
                (v->unk_08 - (D_800DDC40[*(u8 *)(*(u8 **)(s + 0x60) + 0x13)] << 15) - b->unk_08) / 32;
        }
        else
        {
            b->unk_0C = (b->unk_0C - b->unk_00) / 32;
            b->unk_10 = (b->unk_10 - b->unk_04) / 32;
            b->unk_14 = (b->unk_14 - b->unk_08) / 32;
        }
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 4:
        b->unk_00 = b->unk_00 + b->unk_0C;
        b->unk_04 = b->unk_04 + b->unk_10;
        b->unk_08 = b->unk_08 + b->unk_14;
        if (*(s16 *)(p + 0x10) >= 0x20)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if (*(s16 *)(p + 0x16) < 8)
        {
            break;
        }
        if (*(s16 *)(p + 0x18) < 8)
        {
            break;
        }
        if (*(u8 **)(s + 0x60) != NULL)
        {
            func_8009CE1C(*(u8 **)(s + 0x60), 0x18, p[0x9], 2, (s16)(*(u16 *)(p + 0xE) << 9), (u32)s,
                          1);
        }
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 6:
        if (*(s16 *)(p + 0x14) == 0)
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    *(s16 *)(p + 0x14) = 0;
}
