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
s32 func_800BCB04(s32, s32, s16);
extern s16 D_800281F8;
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];

s32 func_8004491C(UnkNode *, u32);
extern u8 D_80045340[];
UnkNode *func_8003FD64(s32, void *);
void func_s55_80025AC4(u8 *p, UnkVec *b, UnkSlot *s);
void func_800B835C(u8 *, s32 *, s32, s32);
extern u8 D_800281FC[];
extern u8 D_80028238[];
extern u8 D_80028250[];

UnkNode *func_8003FC64(s32);
extern u8 D_80028208[];
void func_s55_800243C4(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u8 D_800DF334[];

u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
UnkNode *func_s55_80024ED4(UnkVec *);
s32 func_80065F90(s32, s32);
s32 func_80064710(s32);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s55", func_s55_80024024);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s55", func_s55_80024078);

/* The 0x12 tint crawler's +0x10 step, three states on *(s16 *)(p + 0x2C)
   under this bank's D_800281F8 frame counter. State 0 waits for
   func_8003DE58 to project the owner node's slot into the payload's +0x24
   offset triple (or for that slot to raise 0x8000, which also parks -0x40 at
   +0x28), then adds the triple onto the owner's mover, spawns the burst
   through this unit's func_s55_80024ED4 and advances; state 1 fades the
   renderable's colour word up by 0x202020 a frame until its first byte
   reaches 0xC0 and then advances; state 2 fades it down by 0x40404 and
   retires the actor at zero. 109 words, 0 differing, FIRST COMPILE.

   csrc/ovl_dng_s56.c's landed func_s56_80024388 is the twin (dngtwin 0.927,
   109 words against 109): this copy is that body with +0x48 -> +0x2C,
   +0x4A -> +0x2E, +0x40/0x42/0x44 -> +0x24/0x26/0x28, +0xC -> +0x8,
   D_80027330 -> D_800281F8, func_s56_80027204 -> func_s55_80024ED4, and
   state 2 subtracting 0x40404 where the twin subtracts 0x101010. No other
   delta, and the twin's four-parameter signature already sat in this unit's
   declaration block.

   THE TWIN STATES TWO FACTS AND IT LANDED FIRST COMPILE, so both were
   assertions. Re-measured here:
     HOLDS -- the owner node load `*(UnkNode **)(p + 0x8)' must sit ABOVE the
     +0x2E test, not inside it. Moved inside the gate it is 111 words /
     12 aligned differences (96 under ovlverify). That hoist is what lets the
     already-projected path reach the shared tail on the live $v0 while the
     just-projected path reloads after the call.
     FREE -- "the state is read twice at the top, once signed and once
     unsigned". Dropping the `u16 n' local and writing state 1's bump as
     `*(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1' is 109 words /
     0 differing, so the two reads are not why this lands; gcc emits the
     `lh'/`lhu' pair either way.
   Also measured and FREE at 109 / 0: the `s16 st' local dropped and the
   three arms testing `*(s16 *)(p + 0x2C)' inline. */
void func_s55_800243C4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *o;
    UnkSlot *r;
    UnkVec *m;
    s16 st;
    u16 n;

    D_800281F8 += 1;
    st = *(s16 *)(p + 0x2C);
    n = *(u16 *)(p + 0x2C);
    if (st == 0)
    {
        o = *(UnkNode **)(p + 0x8);
        r = (UnkSlot *)o->unk_0C;
        if (*(s16 *)(p + 0x2E) == 0)
        {
            if (func_8003DE58((u8 *)r->unk_08, r, (s16 *)(p + 0x24), 0) != NULL)
            {
                *(s16 *)(p + 0x2E) = 1;
            }
            else if ((r->unk_14 & 0x8000) != 0)
            {
                *(s16 *)(p + 0x2E) = 1;
                *(s16 *)(p + 0x28) = -0x40;
            }
            if (*(s16 *)(p + 0x2E) == 0)
            {
                return;
            }
        }
        m = (*(UnkNode **)(p + 0x8))->unk_08;
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)m + 0x2) + *(u16 *)(p + 0x24);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)m + 0x6) + *(u16 *)(p + 0x26);
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)m + 0xA) + *(u16 *)(p + 0x28);
        if (func_s55_80024ED4(b) != NULL)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
            *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
        }
    }
    else if (st == 1)
    {
        if (*(u8 *)(c + 0xC) < 0xC0)
        {
            *(s32 *)(c + 0xC) += 0x202020;
        }
        else
        {
            *(s16 *)(p + 0x2C) = n + 1;
        }
    }
    else if (st == 2)
    {
        *(s32 *)(c + 0xC) -= 0x40404;
        if (*(u8 *)(c + 0xC) == 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

/* Spawn the 0x12 tint node: func_800B835C over {0x1000340, 0x200020} on this
   bank's D_800DF334, func_s55_800243C4 as the new node's step, the caller's
   mover position copied in, the slot pointed at D_80028208 with 0xC00
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
u8 *func_s55_80024578(u8 *m, u8 *act)
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
        func_800B835C(D_800DF334, arr, 1, 0);
        v = n->unk_08;
        n->unk_10 = (u32)func_s55_800243C4;
        *(s16 *)((u8 *)v + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)v + 0x6) = *(u16 *)(m + 0x6);
        *(s16 *)((u8 *)v + 0xA) = *(u16 *)(m + 0xA);
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0x8) = (u32)D_80028208;
        *(s16 *)(r + 0x1E) = 0xC00;
        *(s16 *)(r + 0x1C) = 0xC00;
        *(u8 **)((u8 *)n + 0x28) = act - 0x20;
    }
    return (n != NULL) ? (u8 *)n + 0x20 : NULL;
}

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
void func_s55_80024654(s16 x0, s16 y0, s16 z0, s16 x1, s16 y1, s16 z1, s16 *out)
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s55", func_s55_80024728);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s55", func_s55_80024ED4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s55", func_s55_800250B4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s55", func_s55_80025344);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s55", func_s55_80025528);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s55", func_s55_80025604);

/* The 0x212 spark's +0x10 step: bump this bank's D_800281F8 counter, then
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
   struct), `true_dependence' keeps the edge from the `sh %lo(D_800281F8)'
   store to each argument load, the store cannot sink into the jal's delay
   slot and the body is 8 differing. `((SVECTOR *)v)->vy' also drops the edge
   but loads with `lh' for the target's `lhu': 2 differing.

   Measured and FREE at 78 words / 0 differing, so neither is the reason this
   lands: the countdown written as two statements rather than
   `if ((*(s16 *)(p + 0x30) -= 8) <= 0)', and the two table reads written
   `*(s16 *)((u8 *)D_8006CCD8 + idx * 2)' rather than as array subscripts.
   LOAD-BEARING, measured: `D_800281F8++' must come BEFORE the
   func_800BCB04 call -- after it is 18 differing. */
void func_s55_80025764(u8 *p, UnkVec *v)
{
    s16 r;

    D_800281F8++;
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s55", func_s55_8002589C);

/* The 0x212 flash node's +0x10 step, the companion to func_s55_80025B5C
   above: bump this bank's D_800281F8, then either retire -- if the node
   stashed at the payload's +0x8 has raised its own 0x8000 -- or copy that
   node's slot colour word into this slot's +0xC and mirror its mover into
   this one's. 38 words, 0 differing, FIRST COMPILE.

   No twin: dngtwin's only hit is func_s69_80024E60 at 1.000, this
   function's CLONE, still an INCLUDE_ASM. Written from the listing on this
   unit's own idioms (func_s55_80025B5C two listings up installs this
   function as the step and parks the owner node at the payload's +0x8).

   THREE FACTS, all measured here, and two of them are this family's
   recurring pair:
     `D_800281F8 += 1' must come AFTER the `*(UnkNode **)(p + 0x8)' load.
     Before it -- which is the listing's own emission order, since the
     counter's `lhu' is scheduled first either way -- it is 41 words /
     15 aligned differences. What the order decides is whether the
     `sh %lo(D_800281F8)' can sink into the gate's delay slot. There is no
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
void func_s55_80025AC4(u8 *p, UnkVec *b, UnkSlot *s)
{
    UnkNode *o;
    UnkVec *m;

    o = *(UnkNode **)(p + 0x8);
    D_800281F8 += 1;
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

/* Spawn the 0x212 flash node on the caller's node: func_s55_80025AC4 as its
   +0x10 step, D_80045340 through func_8004491C, the caller's mover position
   copied into the new mover, this bank's D_800281FC at the slot's +0x8 with
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
void func_s55_80025B5C(UnkNode *a, s16 b)
{
    UnkNode *n;
    u8 *r;
    u8 *q;
    s32 arr[2];

    n = func_8003FD64(0x212, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s55_80025AC4;
        func_8004491C(n, (u32)D_80045340);
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2);
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6);
        *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA);
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0x8) = (u32)D_800281FC;
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
        func_800B835C(D_80028238, arr, 0, 0);
        func_800B835C(D_80028250, arr, 1, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s55", func_s55_80025C80);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s55", func_s55_80026384);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s55", func_s55_80026548);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s55", func_s55_800265D0);

/* A renderable's facing step: point the slot's +0x00 element at its sheet's
   +0xC plus 24 bytes per frame of the signed +0x04 index, step that index by
   one towards the sign of *b, then clamp it into [lo, hi] -- raising the
   slot's 0x4000 flag when it was clamped and clearing it when it was not.
   The three clamp arms share the one `sh 0x14($t0)' because jump2
   cross-jumps them. 49 words, 0 differing, FIRST COMPILE.

   csrc/ovl_dng_s72.c's landed func_s72_80026060 is the twin (dngtwin 0.940)
   and the ONLY delta is the stride: 24 here against the twin's 22, which is
   exactly the 2-word gap dngtwin reported -- `* 24' is sll/addu/sll where
   `* 22' is sll/addu/sll/subu/sll. The same stride appears in
   func_s55_800266D0 below.

   THE TWIN'S FACT HOLDS AND REPRODUCES ITS FIGURE TO THE DIGIT: the `lo'
   test must be the FIRST arm -- with the `hi' test first it is 49 words /
   11 aligned differences, the twin's own 11. Its two free facts reproduce as
   free (49 / 0): the 0x4000 store written before the clamp store in each
   arm.
   LOAD-BEARING and NOT in the twin's table: `lo' and `hi' must be `s16'
   parameters. As `s32' the body is 43 words / 11, six words of sll/sra
   promotion short -- the same fact func_s72_8002612C records for its own
   `d'.
   Measured and FREE at 49 / 0: `(*(s8 *)(s + 0x4) * 3) << 3' in place of
   `* 24' -- fold reassociates both to the same sll/addu/sll. */
void func_s55_8002660C(u8 *s, s16 *b, s16 lo, s16 hi)
{
    *(s32 *)s = *(s32 *)(*(u8 **)(s + 0x8) + 0xC) + *(s8 *)(s + 0x4) * 24;
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

/* func_s55_8002660C's head without the step or the clamp: point the slot's
   +0x00 element at its sheet's +0xC plus 24 bytes per frame of `d'.
   10 words, 0 differing, FIRST COMPILE. This is csrc/ovl_dng_s72.c's landed
   func_s72_8002612C with the same 24-byte stride as func_s55_8002660C above
   in place of that unit's 22, which is why it is 10 words where the twin is
   12.

   THE TWIN'S FACT HOLDS: `d' must be an `s16' PARAMETER. As `s32' the
   sll/sra promotion pair disappears and the body is 8 words / 2 aligned
   differences -- the twin records 10 words / 12 for its own stride, so the
   mechanism reproduces and only the figures differ with the length.
   The twin's free fact reproduces as free at 10 / 0: `(d * 3) << 3' in place
   of `d * 24'. */
void func_s55_800266D0(u8 *s, s16 d)
{
    *(s32 *)s = *(s32 *)(*(u8 **)(s + 0x8) + 0xC) + d * 24;
}
