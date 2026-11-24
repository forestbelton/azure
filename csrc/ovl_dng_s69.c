#include "common.h"

s32 func_8004491C(UnkNode *, u32);
UnkNode *func_s69_800249DC(UnkVec *b);
UnkNode *func_8003FC64(s32);
s32 func_800CEEFC();
extern u8 D_80027460[];
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
void func_s69_800245A0(u8 *, UnkVec *, u8 *, u8 *);

void func_s69_800260D4(u8 *p, UnkVec *v, UnkSlot *s);
UnkNode *func_s69_8002614C(s16 x, s16 y, s16 z, s16 a, s16 leaf);
extern u8 D_80045340[];
extern u8 D_800287B0[];
extern s16 D_80027450;
UnkNode *func_8003FD64(s32, void *);
s32 func_800BCB04(s32, s32, s16);
extern s16 D_80027452;
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
void func_s69_80024E60(u8 *p, UnkVec *b, UnkSlot *s);
void func_800B835C(u8 *, s32 *, s32, s32);
extern u8 D_80027454[];
extern u8 D_80027490[];
extern u8 D_800274A8[];
void func_s69_80024328(u8 *, UnkVec *, u8 *, u8 *);
extern u8 D_8002746C[];

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80024020);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80024328);

/* Spawn the 0x12 tint node: func_800B835C over {0x1000340, 0x200020} on this
   bank's D_8002746C, func_s69_80024328 as the new node's step, the caller's
   mover position copied in, the slot pointed at D_80027460 with 0xC00
   scales, the acting node less 0x20 stashed at the node's +0x28, and the
   payload returned. 55 words, 0 differing, FIRST COMPILE.
   csrc/ovl_dng_s56.c's landed func_s56_8002453C is the twin (dngtwin 0.902):
   one func_800B835C call instead of two, func_8003FC64(0x12) instead of (2),
   and +0x28 instead of +0x2C.

   TWO SPELLING FACTS, both measured here: func_800B835C must come FIRST,
   before the mover copies -- moved after them it is 57 words / 56
   differing, which is the OPPOSITE of func_s69_80024EF8 below, where the
   same call has to come last. And the tail must be the conditional
   expression `return (n != NULL) ? (u8 *)n + 0x20 : NULL;': as an
   `if (n != NULL) { return ...; } return NULL;' it is 56 words /
   13 differing. */
u8 *func_s69_800244C4(u8 *m, u8 *act)
{
    UnkNode *n;
    UnkVec *v;
    u8 *r;
    s32 arr[2];

    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        arr[0] = 0x1000340;
        arr[1] = 0x200020;
        func_800B835C(D_8002746C, arr, 1, 0);
        v = n->unk_08;
        n->unk_10 = (u32)func_s69_80024328;
        *(s16 *)((u8 *)v + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)v + 0x6) = *(u16 *)(m + 0x6);
        *(s16 *)((u8 *)v + 0xA) = *(u16 *)(m + 0xA);
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0x8) = (u32)D_80027460;
        *(s16 *)(r + 0x1E) = 0xC00;
        *(s16 *)(r + 0x1C) = 0xC00;
        *(u8 **)((u8 *)n + 0x28) = act - 0x20;
    }
    return (n != NULL) ? (u8 *)n + 0x20 : NULL;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_800245A0);

/* The nine-node burst func_s56_80024388 fires: allocate nine 0x2 nodes, give
   each func_s69_800245A0 as its step and D_80027460 as its slot's +0x8
   record, copy the caller's mover position into both the new mover's position
   and velocity and into the payload's +0x38 triple, scale the last node 0x800
   and the rest 0x200, ramp the grey down by 4 per node and return the last
   node allocated. No xsibs twin; the spawner skeleton is
   csrc/ovl_dng_s21.c's func_s21_8002495C (same func_8003FC64 /
   `n->unk_10 = step' / func_8004491C(n, (u32)func_800CEEFC) order).

   Three spellings were measured here, and each is worth real rows:

     * `q = (u8 *)n + 0x20' must be computed AFTER both calls. Before them it
       is live across a call, takes a SEVENTH callee-saved register and the
       body is 74 words / 63 differing.
     * the 0x200-vs-0x800 scale is an `if'/`else' with the `sh' DUPLICATED PER
       ARM, which cross-jumping merges back. A value in a temp collapses to a
       pre-loaded 0x800 plus a conditional overwrite: 72 words / 45 differing.
     * each position group is ONE CHAINED assignment,
       `*(q + 0x38) = *(m + 0xE) = *(m + 0x2) = *(u16 *)(b + 0x2)', emitted
       innermost-first. A `u16' local holding the loaded halfword gives the
       same one `lhu' but swaps $v0 and $v1 between `m' and the value:
       13 differing with every opcode right.

   73 words, 0 differing.
   THIS COPY'S OWN CONSTANT: 0x2 -> 0x12 at 1 of 4 sites, 0x38 -> 0x1c, 0x3a -> 0x1e, 0x3c -> 0x20, 0xc -> 0xd, 0x54 -> 0x38 at all 2 sites, 0x4c -> 0x30.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
UnkNode *func_s69_800249DC(UnkVec *b)
{
    UnkNode *n;
    u8 *m;
    u8 *r;
    u8 *q;
    s32 i;

    for (i = 0; i < 9; i++)
    {
        n = func_8003FC64(18);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s69_800245A0;
            func_8004491C(n, (u32)func_800CEEFC);
            m = (u8 *)n->unk_08;
            q = (u8 *)n + 0x20;
            *(s16 *)(q + 0x1c) = *(s16 *)(m + 0xE) = *(s16 *)(m + 0x2) =
                *(u16 *)((u8 *)b + 0x2);
            *(s16 *)(q + 0x1e) = *(s16 *)(m + 0x12) = *(s16 *)(m + 0x6) =
                *(u16 *)((u8 *)b + 0x6);
            *(s16 *)(q + 0x20) = *(s16 *)(m + 0x16) = *(s16 *)(m + 0xA) =
                *(u16 *)((u8 *)b + 0xA);
            r = (u8 *)n->unk_0C;
            *(u32 *)(r + 0x8) = (u32)D_80027460;
            if (i != 8)
            {
                *(s16 *)(r + 0x1E) = 0x200;
            }
            else
            {
                *(s16 *)(r + 0x1E) = 0x800;
            }
            r[0xd] = -0x80 - ((8 - i) << 2);
            *(s16 *)(r + 0x1C) = *(u16 *)(r + 0x1E);
            *(s16 *)(q + 0x38) = i;
            if (i == 8)
            {
                *(s16 *)(q + 0x38) = 7;
            }
            *(s16 *)(q + 0x30) = 8;
        }
    }
    return n;
}

/* The 0x212 spark's +0x10 step: bump this bank's D_80027452 counter, then
   turn the mover towards func_800BCB04's bearing or -- if it is already past
   it -- glide one frame, the glide taking its velocity from the resident
   D_8006CCD8/D_8006CCE8 direction pair scaled by the payload's +0x30
   countdown and a 0xC0-relative climb on the third axis. The +0x30 countdown
   falls by 8 a frame and raises the node's redraw bit at zero.
   csrc/ovl_dungeon_800CABA8.c's func_800CAED0 is the template (xsibs 0.676);
   this copy's glide is the table lookup where the resident one bleeds the
   deltas by a quarter. 78 words, 0 differing.

   THE ONE SPELLING FACT, and it is a dependence not a shape: the three mover
   reads feeding func_800BCB04 must be ARRAY REFS, `((u16 *)v)[1]', `[3]',
   `[5]'. Written `*(u16 *)((u8 *)v + 0x2)' the MEMs lose MEM_IN_STRUCT_P
   (docs/matching-levers.md:34602 -- the flag needs a PLUS_EXPR on top, not a
   struct), `true_dependence' keeps the edge from the `sh %lo(D_80027452)'
   store to each argument load, the store cannot sink into the jal's delay
   slot and the body is 8 differing. `((SVECTOR *)v)->vy' also drops the edge
   but loads with `lh' for the target's `lhu': 2 differing.

   Measured and FREE at 78 words / 0 differing, so neither is the reason this
   lands: the countdown written as two statements rather than
   `if ((*(s16 *)(p + 0x30) -= 8) <= 0)', and the two table reads written
   `*(s16 *)((u8 *)D_8006CCD8 + idx * 2)' rather than as array subscripts.
   LOAD-BEARING, measured: `D_80027452++' must come BEFORE the
   func_800BCB04 call -- after it is 18 differing. */
void func_s69_80024B00(u8 *p, UnkVec *v)
{
    s16 r;

    D_80027452++;
    r = func_800BCB04(((u16 *)v)[1], ((u16 *)v)[3], ((u16 *)v)[5] + 2);
    if (*(s16 *)((u8 *)v + 0xA) < r)
    {
        v->unk_00 -= (D_8006CCD8[*(s16 *)(p + 0x36)] * *(s16 *)(p + 0x30)) << 11;
        v->unk_04 -= (D_8006CCE8[*(s16 *)(p + 0x36)] * *(s16 *)(p + 0x30)) << 11;
        v->unk_08 += (0xC0 - *(s16 *)(p + 0x30)) << 10;
    }
    if ((*(s16 *)(p + 0x30) -= 8) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80024C38);

/* The 0x212 flash node's +0x10 step, the companion to func_s55_80025B5C
   above: bump this bank's D_80027452, then either retire -- if the node
   stashed at the payload's +0x8 has raised its own 0x8000 -- or copy that
   node's slot colour word into this slot's +0xC and mirror its mover into
   this one's. 38 words, 0 differing, FIRST COMPILE.

   No twin: dngtwin's only hit is func_s69_80024E60 at 1.000, this
   function's CLONE, still an INCLUDE_ASM. Written from the listing on this
   unit's own idioms (func_s55_80025B5C two listings up installs this
   function as the step and parks the owner node at the payload's +0x8).

   THREE FACTS, all measured here, and two of them are this family's
   recurring pair:
     `D_80027452 += 1' must come AFTER the `*(UnkNode **)(p + 0x8)' load.
     Before it -- which is the listing's own emission order, since the
     counter's `lhu' is scheduled first either way -- it is 41 words /
     15 aligned differences. What the order decides is whether the
     `sh %lo(D_80027452)' can sink into the gate's delay slot. There is no
     intervening call. The same fact holds the same way round in
     docs/dng-parks/func_s55_80025C80.c.
     The mover must be reached by RE-READING `*(UnkNode **)(p + 0x8)', not
     through `o'. Through `o' it is 36 words / 3 -- two words short, because
     the store to the slot's +0xC is what forces the reload.
     The 0x8000 gate must be the COMPONENT_REF `o->unk_1E'. As the plain cast
     `*(u16 *)((u8 *)o + 0x1E)' it is 39 words / 17: MEM_IN_STRUCT_P, the
     same mechanism csrc/ovl_dng_s25.c records for its four retire sites.
     Note the retire STORE beside it, `*(u16 *)(p - 0x2) |= 0x8000', is the
     other polarity and stays a plain cast, as it does there. */
void func_s69_80024E60(u8 *p, UnkVec *b, UnkSlot *s)
{
    UnkNode *o;
    UnkVec *m;

    o = *(UnkNode **)(p + 0x8);
    D_80027452 += 1;
    if ((o->unk_1E & 0x8000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    else
    {
        *(s32 *)((u8 *)s + 0xC) = *(s32 *)((u8 *)o->unk_0C + 0xC);
        m = (*(UnkNode **)(p + 0x8))->unk_08;
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)m + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)m + 0x6);
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)m + 0xA);
    }
}

/* Spawn the 0x212 flash node on the caller's node: func_s69_80024E60 as its
   +0x10 step, D_80045340 through func_8004491C, the caller's mover position
   copied into the new mover, this bank's D_80027454 at the slot's +0x8 with
   0x2800 scales, a plain 0x808080 tint, 0x8 at +0x6 and `b' + 0x400 as the
   slot's +0x1A rotation, the caller's node stashed at the payload's +0x8 and
   `b' at +0x36, then func_800B835C run twice over the stack pair
   {0x1400340, 0x200020}. 73 words, 0 differing, FIRST COMPILE -- the
   func_800B835C/`s32 arr[2]' skeleton is csrc/ovl_dng_s56.c's landed
   func_s56_8002453C.

   LOAD-BEARING, measured here: the two `arr' assignments and both
   func_800B835C calls must come LAST, after the payload stores. Moved up to
   just after func_8004491C -- which is where the s56 sibling has them -- it
   is 74 words / 54 differing. Measured and FREE at 73 words / 0 differing:
   the 0x808080 store written before the +0x1A store instead of after. */
void func_s69_80024EF8(UnkNode *a, s16 b)
{
    UnkNode *n;
    u8 *r;
    u8 *q;
    s32 arr[2];

    n = func_8003FD64(0x212, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s69_80024E60;
        func_8004491C(n, (u32)D_80045340);
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2);
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6);
        *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA);
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0x8) = (u32)D_80027454;
        *(s16 *)(r + 0x1E) = 0x2800;
        *(s16 *)(r + 0x1C) = 0x2800;
        *(s16 *)(r + 0x6) = 8;
        *(s16 *)(r + 0x1A) = b + 0x400;
        *(s32 *)(r + 0xC) = 0x808080;
        q = (u8 *)n + 0x20;
        *(UnkNode **)(q + 0x8) = a;
        *(s16 *)(q + 0x36) = b;
        arr[0] = 0x1400340;
        arr[1] = 0x200020;
        func_800B835C(D_80027490, arr, 0, 0);
        func_800B835C(D_800274A8, arr, 1, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_8002501C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_800255B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_800257D0);

/* A renderable's facing step: point the slot's +0x00 element at its sheet's
   +0xC plus 22 bytes per frame of the signed +0x04 index, step that index by
   one towards the sign of *b, then clamp it into [lo, hi] -- raising the
   slot's 0x4000 flag when it was clamped and clearing it when it was not.
   The three clamp arms share the one `sh 0x14($t0)' because jump2
   cross-jumps them. No twin anywhere: dngtwin finds nothing above 0.62
   landed, and the resident units have nothing at this shape.
   51 words, 0 differing, FIRST COMPILE.

   LOAD-BEARING, measured: the `lo' test must be the FIRST arm. With the
   `hi' test first it is 51 words / 11 differing. Measured and FREE at 51
   words / 0 differing, so neither is why this lands: the 0x4000 flag store
   written before the clamp store in each arm rather than after, and the
   whole body written through `UnkSlot *s' field accesses (unk_00/unk_04/
   unk_08/unk_14) instead of `u8 *s' with casts -- the slot type is
   witnessed here by +0x04 as a signed byte, +0x08 as a pointer and +0x14 as
   the flag halfword, and it measures identically either way. */
void func_s69_80025840(u8 *s, s16 *b, s16 lo, s16 hi)
{
    *(s32 *)s = *(s32 *)(*(u8 **)(s + 0x8) + 0xC) + *(s8 *)(s + 0x4) * 22;
    if (*b > 0)
    {
        *(s8 *)(s + 0x4) = *(s8 *)(s + 0x4) + 1;
    }
    else if (*b < 0)
    {
        *(s8 *)(s + 0x4) = *(s8 *)(s + 0x4) - 1;
    }
    if (*(s8 *)(s + 0x4) < lo)
    {
        *(s8 *)(s + 0x4) = lo;
        *(u16 *)(s + 0x14) |= 0x4000;
    }
    else if (hi < *(s8 *)(s + 0x4))
    {
        *(s8 *)(s + 0x4) = hi;
        *(u16 *)(s + 0x14) |= 0x4000;
    }
    else
    {
        *(u16 *)(s + 0x14) &= 0xBFFF;
    }
}

/* Point the slot's +0x00 element at its sheet's +0xC plus 22 bytes per frame
   of `d' -- func_s69_80025840's head without the step or the clamp.
   12 words, 0 differing, FIRST COMPILE.

   LOAD-BEARING, measured: `d' must be an `s16' PARAMETER. As `s32' the
   `sll'/`sra' promotion pair disappears and the body is 10 words /
   12 differing. Measured and FREE at 12 words / 0 differing: `d * 11 * 2'
   in place of `d * 22' -- fold reassociates it to the same
   sll/addu/sll/subu/sll chain. */
void func_s69_8002590C(u8 *s, s16 d)
{
    *(s32 *)s = *(s32 *)(*(u8 **)(s + 0x8) + 0xC) + d * 22;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_8002593C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80025A34);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80025AAC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80025B78);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80025D28);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80025FCC);

/* The +0x10 step of the 0x202 burst node this bank's recursive spawner parks:
   bump the bank's u16 counter, fade the slot's RGB triple at +0xC..+0xE by one
   quarter each, and raise the kill bit on the node flag at -0x2 and on
   D_800814A0 once the red channel drops under 4.
   30 words, 0 differing, FIRST COMPILE.
   TWIN: the landed func_s21_800248E4 in csrc/ovl_dng_s21.c (dngtwin 0.967,
   30 words). Deltas: this bank's counter, `>> 2' for its `>> 3' and a guard
   of 4 for its 8. Of its three stated facts:
     - HOLDS: the three fades are SEPARATE statements, not a chain. Written
       `[0] = [1] = [2] = [0] - ([0] >> 2)' it is 26 words / 17 aligned
       differences -- the chain has one value where the target has three
       lbu/srl/subu runs.
     - HOLDS: the guard RE-READS the byte (a second `lbu 0xC'). Guarding on
       the computed value through an `s32' local is 29 words / 18 aligned
       differences.
     - FREE, so its reason is weaker than stated: it says `>> 2' and not
       `/ 4' because the bytes are u8 and the shift needs no sign
       correction. Both spellings are 30 words / 0 -- an unsigned divide by
       a power of two is the same `srl', so nothing selects the shift.
   This copy's own measured facts: the counter bump must be the FIRST
   statement (after the three fades it is 33 words / 23 aligned differences),
   and the kill tail is FREE as an `if' or as an early `return' with the
   teardown flat (30 / 0 either way).
   The third parameter is `UnkSlot *', which is what csrc/ovl_dng_s69.c
   already declares for this family's other member; as `u8 *c' with
   `c[0xC]' casts it is also 30 words / 0. */
void func_s69_800260D4(u8 *p, UnkVec *v, UnkSlot *s)
{
    D_80027452++;
    s->unk_0C[0] -= s->unk_0C[0] >> 2;
    s->unk_0C[1] -= s->unk_0C[1] >> 2;
    s->unk_0C[2] -= s->unk_0C[2] >> 2;
    if (s->unk_0C[0] < 4)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The recursive burst at the bank's D_800287B0 aim point. Called with
   leaf != 0 it spawns ONE 0x212 node at (x, y, z) with func_s69_800260D4 as
   its step, 0x400 scales, D_80027460 at the slot's +0x8, a 0x808080 tint,
   0x20 at +0x10, the slot's 0xC flags and 0x10 at the payload's +0x30, and
   returns it. Called with leaf == 0 it is the DRIVER: when this bank's
   D_80027450 is exactly 3 it walks sixteen steps of one sixteenth of the
   way from (x, y, z) to D_800287B0, offsetting each step by rcos/rsin of
   the caller's angle times rsin of (i << 7) >> 21, and recurses with
   leaf = 1 at each; then, if D_80027450 is 0, it publishes (x, y, z) into
   D_800287B0, and either way bumps D_80027450. 160 words, 0 differing.

   NO TWIN. xsibs' best is func_800DAEF4 at 0.430 and it is a different
   function; dngtwin finds nothing above 0.31 anywhere. Written from the
   listing.

   THE ONE SPELLING FACT, measured three ways: D_800287B0 needs TWO SEPARATE
   `u8 *' LOCALS, one per basic block -- `g' in the delta block and `h' in
   the publish block. With a single local shared by both blocks it is 12
   differing, all of them register names (the one pointer gets $a1 in both
   places where the target has $a2 in the delta block and reuses the `lui's
   $v0 in the publish block). Written inline as
   `*(s16 *)(D_800287B0 + 0x2)' the `+ 2' folds into the symbol address and
   the base is never materialised at all: 22 differing.

   Measured and FREE at 160 words / 0 differing, so neither is the reason
   this lands: three `sx'/`sy'/`sz' locals copied from the parameters at the
   top and used in the publish -- the target's three unconditional `sh'
   stores into the frame look like declared locals and the frame arithmetic
   fits them, but reading the parameters directly, as the body does, measures
   identically -- and an explicit `(s16)' cast on the `cz' recursion
   argument. LOAD-BEARING but cheap, measured: the `leaf' gate must be
   `if (leaf != 0)' with the spawn as the `then' arm; inverted it is
   1 differing. */
UnkNode *func_s69_8002614C(s16 x, s16 y, s16 z, s16 a, s16 leaf)
{
    UnkNode *n;
    UnkSlot *r;
    UnkVec *m;
    s16 dx;
    s16 dy;
    s16 dz;
    s16 cx;
    s16 cy;
    s16 cz;
    s32 i;
    u8 *g;
    u8 *h;

    n = NULL;
    if (leaf != 0)
    {
        n = func_8003FC64(0x212);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s69_800260D4;
            func_8004491C(n, (u32)D_80045340);
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = x;
            *(s16 *)((u8 *)m + 0x6) = y;
            *(s16 *)((u8 *)m + 0xA) = z;
            r = (UnkSlot *)n->unk_0C;
            r->unk_1E = 0x400;
            r->unk_1C = 0x400;
            r->unk_08 = (u32)D_80027460;
            *(u32 *)r->unk_0C = 0x808080;
            r->unk_10 = 0x20;
            r->unk_14 |= 0xC;
            *(s16 *)((u8 *)n + 0x50) = 0x10;
        }
    }
    else
    {
        if (D_80027450 == 3)
        {
            g = D_800287B0;
            dx = (*(s16 *)(g + 0x2) - x) / 16;
            dy = (*(s16 *)(g + 0x6) - y) / 16;
            dz = (*(s16 *)(g + 0xA) - z) / 16;
            cx = x + dx;
            cy = y + dy;
            cz = z + dz;
            for (i = 0; i < 0x10; i++)
            {
                func_s69_8002614C(cx + ((func_80064584(a) * func_800644B8(i << 7)) >> 21),
                                  cy + ((func_800644B8(a) * func_800644B8(i << 7)) >> 21), cz, a,
                                  1);
                cx += dx;
                cy += dy;
                cz += dz;
            }
        }
        if (D_80027450 == 0)
        {
            h = D_800287B0;
            *(s16 *)(h + 0x2) = x;
            *(s16 *)(h + 0x6) = y;
            *(s16 *)(h + 0xA) = z;
        }
        D_80027450++;
    }
    return n;
}
