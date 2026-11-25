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

void func_s55_8002660C(u8 *s, s16 *b, s16 lo, s16 hi);
void func_s55_800266D0(u8 *s, s16 d);
void func_s55_80026548(u8 *, UnkElem *);
void func_s55_8002589C(UnkNode *a, s16 b, s32 c);
s32 func_800A44E0(u16, u16, s16, u32);
u8 *func_8009B4B0(u8 *, u16, u16);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
s32 func_800C9034(u8 *, UnkVec *, UnkSlot *);
extern u8 *D_800E3D7C;
extern u8 D_80082E80[];
extern u16 D_80082E94;
extern u8 D_80083780[];
extern s16 D_8002966C;
extern u8 D_8002966E;
extern u32 D_80029670;
void func_80040490(void *, RECT *);
void func_8003F80C(u32, s32, s32, s16);
s32 func_800A56E0(s32);
s32 func_80053EF0(s32);
u8 *func_s55_80024578(u8 *m, u8 *act);
extern u32 D_800814A8;
extern u8 D_80028F68[];
extern u8 D_8002964C[];
s32 func_s55_80026384(s16, s16, s16, s16);
void func_s55_80025604(s16, s16, s16);
extern u8 D_80028220[];
s32 func_800C95C0(u8 *, UnkVec *, UnkSlot *);
extern u8 D_8002822C[];

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s55", func_s55_80024024);

/* The bank's SEVEN-state cutscene driver, run off *(s16 *)(p + 0xA) and the
   relink of csrc/ovl_dng_s69.c's landed six-state func_s69_80024020
   (dngtwin 0.884). State 0 opens: a func_80040490 RECT over D_80028F68,
   func_8003F80C on D_8002964C, D_8002966C raised, the actor's +0x9 published
   into D_8002966E and its +0x0 into D_80029670, the player's +0xF4 pointed at
   this bank's D_800281FC, +0x96 armed at 0x14, +0xA8 taken from the actor's
   +0x8 and +0x102 raised -- then FALLS THROUGH into state 1, which waits for
   the view's own +0x0 to carry 0x80, spawns the node through this unit's
   func_s55_80024578 and arms the 0x20 countdown at +0x1A with the -1/0x20
   fade pair at +0x20/+0x22. State 2 waits for the spawned node's +0x2C to
   read 1, arms a 0x18 countdown at +0x1C and FALLS THROUGH into state 3,
   which waits for that countdown to fall below 0xB, drops the player's +0xA6
   and plays cue 0x4300 or 0x300 depending on func_80053EF0(4). State 4 waits
   for D_8002966C to clear and re-arms +0x1A, state 5 eases D_80083160's
   +0xA9/+0xAA colour bytes towards 0x80 over that countdown and snaps them to
   0x80 at the end, state 6 retires the actor and drops D_80083460's +0xA
   counter once this bank's D_800281F8 reads zero. Every state ends in the
   shared tail, which -- while +0x20 is negative -- eases the same two colour
   bytes towards 0x20 over the +0x22 countdown, and always clears D_800281F8.
   211 words, 0 differing.

   THIS FUNCTION OWNS jtbl_s55_80024008. jtcheck.py: 7 words = 7 cases + 0
   pad, dispatch bound 7, 7 distinct targets. Entries, in table order:

       case 0 -> .Ls55_800240C0     case 4 -> .Ls55_80024244
       case 1 -> .Ls55_80024168     case 5 -> .Ls55_80024270
       case 2 -> .Ls55_800241BC     case 6 -> .Ls55_800242EC
       case 3 -> .Ls55_800241E4

   The seven entry addresses are strictly ascending, so the arms are written
   ascending 0..6, which is their source order. ARM ORDER SETTLED TWO WAYS,
   because neither ovlverify nor aligndiff can see it. (a) PER ARM: all seven
   arms carry a distinguishing non-branch instruction and no two are
   word-identical after cross-joining, so none is in the invisible
   masked-jump-plus-common-code class. (b) INDEPENDENTLY, a standalone
   `tools/cdkgcc -S' of this file emits the table as
   `.word $L4, $L5, $L8, $L10, $L14, $L16, $L18' with those seven labels
   appearing in the text in that same order and no trailing pad word.
   Cases 1 and 4 share the tail of their +0xA bump through one label and
   cases 3 and 5 share another; both are jump2 cross-joins of bumps written
   out per arm, not hand-settled arms.

   THE TWIN'S FOUR FACTS, re-measured here -- THREE HELD AND ONE INVERTED:

   (1) HOLDS. The +0xA state bumps must be u16 ARRAY REFS,
   `((u16 *)p)[5] = ((u16 *)p)[5] + 1'.

   (3) INVERTED, and it is the single largest fact on this body. The twin
   requires its +0xF4 store to be the ARRAY REF `((s32 *)n)[0x3D] = 0' so
   that MEM_IN_STRUCT_P lets the fixed-address D_800287A4 store swap back
   past it. Here the target keeps the source order, so the +0xF4 store must
   be the PLAIN CAST `*(u32 *)(n + 0xF4) = (u32)D_800281FC'. As the array ref
   it is 211 words / 11 aligned differences, with the whole D_80029670 store
   emitted AFTER the +0xF4 store instead of before it. The twin's other half
   of that conjunction -- `D_80029670 = *(u32 *)(p + 0x0)' before the +0xF4
   store -- HOLDS.
   AND A WARNING ABOUT THAT 11, because this body caught me out: measured
   mid-fight, with state 3's cue gate still wrong, the array ref read 14 and
   moving `n = (u8 *)D_800814A8' from before the D_80029670 store to after it
   read 15 then 14, which looks exactly like a one-row ordering fact. Priced
   from this body at 0, that ordering is 211 / 0 -- FREE in both directions.
   It was never a fact; it was perturbing the same allocation the array ref
   decides. Do not inherit it.

   (4) HOLDS. D_800814A8 is a separate pointer local per arm (`n' in state 0,
   `n2' in state 3), and state 0's third use, the +0x102 store, is inline
   rather than through `n' -- which is what makes the target load D_800814A8
   twice in state 0.

   The twin's fact (2) is VACUOUS here: it is about a `D_8002744C = 0' store
   this bank's state 0 does not have.

   THE OTHER FACT, and it is the which-arm-is-the-`then' lever: state 3's cue
   gate must be `if (func_80053EF0(4) != 2) { func_800A56E0(0x300); } else
   { func_800A56E0(0x4300); }'. Written the natural way round, `== 2' with
   0x4300 as the then-arm, it is 211 words / 3 aligned differences -- a `bne'
   for the target's `beq' and the 0x4300 out of the branch's delay slot.
   NOT CONSTRAINED, measured: the arm order is everything and the FORM is
   free. `func_800A56E0(func_80053EF0(4) != 2 ? 0x300 : 0x4300)' is also 211
   / 0, and the same ternary written `== 2 ? 0x4300 : 0x300' is the same 3
   rows -- so do not read the if/else here as a measured choice. A `s32 cue'
   local assigned in both arms is 213 WORDS / 19 and is the one form that
   loses.

   TWO MORE STATEMENT-ORDER FACTS, both of which I expected to be free and
   both of which are LOAD-BEARING -- priced from this body at 0: state 5's
   two 0x80 snap stores must be +0xA9 then +0xAA (the other way round is
   211 / 2, and note the twin writes ITS pair the other way round), and state
   1's three countdown stores must be +0x1A, +0x20, +0x22 (written
   +0x20, +0x22, +0x1A, which is the twin's order with +0x1A appended, it is
   211 / 8). Adjacent independent stores are an order to PRICE, not to read
   off the listing, and on this body the listing's order happens to be the
   source's for both. */
void func_s55_80024078(u8 *p)
{
    u8 *g;
    u8 *n;
    u8 *n2;
    u8 *h;
    s32 arr[2];

    g = (u8 *)&D_80083160;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        arr[0] = 0x10003A0;
        arr[1] = 0x400020;
        func_80040490(D_80028F68, (RECT *)arr);
        func_8003F80C((u32)D_8002964C, 0x7AC0, 1, 2);
        D_8002966C = 1;
        D_8002966E = p[0x9];
        D_80029670 = *(u32 *)(p + 0x0);
        n = (u8 *)D_800814A8;
        *(u32 *)(n + 0xF4) = (u32)D_800281FC;
        *(s16 *)(n + 0x96) = 0x14;
        n[0xA8] = p[0x8];
        *(u8 *)((u8 *)D_800814A8 + 0x102) = 1;
        ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
    case 1:
        if ((**(u16 **)(p + 0x4) & 0x80) != 0)
        {
            if ((*(u8 **)(p + 0xC) = func_s55_80024578(
                     *(u8 **)(*(u8 **)(p + 0x0) - 0x18), *(u8 **)(p + 0x0))) != NULL)
            {
                *(s16 *)(p + 0x1A) = 0x20;
                *(s16 *)(p + 0x20) = -1;
                *(s16 *)(p + 0x22) = 0x20;
                ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
            }
        }
        break;
    case 2:
        if (*(s16 *)(*(u8 **)(p + 0xC) + 0x2C) != 1)
        {
            break;
        }
        *(s16 *)(p + 0x1C) = 0x18;
        ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
    case 3:
        if ((*(s16 *)(p + 0x1C) -= 1) < 0xB)
        {
            n2 = (u8 *)D_800814A8;
            *(s16 *)(n2 + 0xA6) -= 1;
            if (func_80053EF0(4) != 2)
            {
                func_800A56E0(0x300);
            }
            else
            {
                func_800A56E0(0x4300);
            }
            ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
        }
        break;
    case 4:
        if (D_8002966C == 0)
        {
            *(s16 *)(p + 0x1A) = 0x20;
            *(s16 *)(p + 0x20) = 0;
            ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
        }
        break;
    case 5:
        g[0xA9] += (0x80 - g[0xA9]) / *(s16 *)(p + 0x1A);
        g[0xAA] += (0x80 - g[0xAA]) / *(s16 *)(p + 0x1A);
        if ((*(s16 *)(p + 0x1A) -= 1) <= 0)
        {
            g[0xA9] = 0x80;
            g[0xAA] = 0x80;
            ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
        }
        break;
    case 6:
        if (D_800281F8 == 0)
        {
            h = (u8 *)&D_80083460;
            *(s32 *)(h + 0xC) = 0;
            *(s16 *)(h + 0xA) -= 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    if (*(s16 *)(p + 0x20) < 0)
    {
        g[0xA9] += (0x20 - g[0xA9]) / *(s16 *)(p + 0x22);
        g[0xAA] += (0x20 - g[0xAA]) / *(s16 *)(p + 0x22);
        if ((*(s16 *)(p + 0x22) -= 1) <= 0)
        {
            *(s16 *)(p + 0x20) = 0;
        }
    }
    D_800281F8 = 0;
}

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

/* The 0x212 beam's +0x10 step, three states on *(s16 *)(p + 0x2C) under this
   bank's D_800281F8 frame counter. State 0 opens the beam: the slot's +0x1E
   scale eases towards 0x1000 by 1/count, its three colour bytes ease towards
   0x80 less the payload's +0x3C bias, and the mover is dragged towards the
   payload's +0x14/+0x16/+0x18 target triple, all by 1/count; when the count
   runs out it resets the count to 0x10, snaps +0x1E to 0x1000 and the colour
   to 0x808080, and advances -- to state 2 directly if +0x3C is set, otherwise
   to state 1. State 1 asks this unit's func_s55_80026384 whether the
   +0x1C/+0x1E/+0x20 tile is clear along D_800E3D7C's +0x2A heading and, if
   so, runs func_s55_80025604 over it, then re-arms the 0x10 count and
   advances; with +0x38 set it skips straight to that. State 2 is the fade:
   the colour divided down by the count and the actor retired at zero. The
   shared tail, which every state reaches, rolls this bank's D_80028220 +0x9
   phase counter modulo 0x20 while neither +0x38 nor +0x3C is set.
   164 words, 0 differing.

   NO TWIN -- dngtwin finds no carved sibling at 0.60 and dngnear returns
   nothing for this stub. It is written from the listing on THIS UNIT'S OWN
   LANDED BODIES, which is what a no-twin body in a half-landed unit should
   use: func_s55_80025C80 two listings down supplies the
   `*(s16 *)x = *(u16 *)x + (<target> - *(s16 *)x) / <count>' ease and the
   three-byte colour publish, and func_s55_80025AC4 supplies the retire pair.
   The callee-set intersection (D_800281F8, D_80028220, D_800814A0,
   D_800E3D7C) names those same two siblings and nothing better.

   TWO FACTS, and the SECOND ONE IS THE WHOLE BODY. Both measured here.

   (1) STATE 0's AND STATE 2's COLOUR BYTE MUST BE TWO SEPARATE LOCALS, one
   per arm. Shared between the arms it is 163 WORDS -- one word SHORT -- and
   29 aligned differences: one pseudo spanning two disjoint arms takes a
   register neither arm wants, and the knock-on is that `b's own entry copy
   folds away. This is local-alloc.c:1759 (`combine_regs' will not tie an
   output to a dying input), and both of these locals are read-modify-write
   accumulators, which is the class that needs per-arm scope.

   (2) STATE 0's COLOUR EASE MUST BE `(0x80 - *(s16 *)(p + 0x3C) - c)'.
   Written `(0x80 - c - *(s16 *)(p + 0x3C))' -- which is how the listing's own
   `addiu 0x80' / `subu $v0, $v0, $a0' pair reads, since that pair subtracts
   the COLOUR first -- it is 164 words / 32 aligned differences.
   AND READ THE RESIDUE OF THAT 32 BEFORE YOU BELIEVE IT, because it is the
   most misleading one I have seen: it presents as a MISSING
   `addu $a3, $a1, $zero' (the parameter `b's entry copy) plus a whole-body
   `$a1'/`$a3' rotation through all six mover accesses and the 0x1000
   constant -- the textbook wrong-pseudo-count signature -- and `-da's
   `.greg' corroborates it, printing `b's allocno with `preferences: 5' and
   handing it $a1 where the target has $a3. None of that is the fault. One
   operand order in one subtraction moves all of it, including the position
   of the D_800281F8 bump's `lui'/`lhu' relative to the prologue.
   fold reassociates the two independent subtrahends, so the listing cannot
   tell you which side the source wrote.

   MEASURED AND FREE at 164 / 0, so it is NOT why this lands: the tail's
   double gate written as two nested `if's instead of `&&'. That is the
   `fold_truthop' precondition holding -- the two tests are on DIFFERENT
   memory locations (p+0x38 and p+0x3C), not two bit tests on one local, so
   there is nothing to merge and nesting buys nothing.

   LOAD-BEARING, measured, and both of them small enough to lose by accident:
   state 0's three colour stores must be +0xC, +0xD, +0xE in that order
   (+0xC, +0xE, +0xD, which is what func_s55_80025C80's own case 0 wants 40
   words away, is 164 / 2 here -- two bodies in one unit wanting opposite
   answers); and state 0's advance gate must be `if (*(s16 *)(p + 0x3C) != 0)
   { +0x2C = 2; } else { +0x2C += 1; }' -- the arms the other way round is
   164 / 5. */
void func_s55_800250B4(u8 *p, UnkVec *b, UnkSlot *s)
{
    u8 *q;
    u8 c;
    u8 c2;

    D_800281F8 += 1;
    switch (*(s16 *)(p + 0x2C))
    {
    case 0:
        *(s16 *)((u8 *)s + 0x1E) =
            *(u16 *)((u8 *)s + 0x1E) +
            (0x1000 - *(u16 *)((u8 *)s + 0x1E)) / *(s16 *)(p + 0x30);
        c = *(u8 *)((u8 *)s + 0xC);
        c = c + (0x80 - *(s16 *)(p + 0x3C) - c) / *(s16 *)(p + 0x30);
        *(u8 *)((u8 *)s + 0xC) = c;
        *(u8 *)((u8 *)s + 0xD) = c;
        *(u8 *)((u8 *)s + 0xE) = c;
        *(s16 *)((u8 *)b + 0x2) =
            *(u16 *)((u8 *)b + 0x2) +
            (*(s16 *)(p + 0x14) - *(s16 *)((u8 *)b + 0x2)) / *(s16 *)(p + 0x30);
        *(s16 *)((u8 *)b + 0x6) =
            *(u16 *)((u8 *)b + 0x6) +
            (*(s16 *)(p + 0x16) - *(s16 *)((u8 *)b + 0x6)) / *(s16 *)(p + 0x30);
        *(s16 *)((u8 *)b + 0xA) =
            *(u16 *)((u8 *)b + 0xA) +
            (*(s16 *)(p + 0x18) - *(s16 *)((u8 *)b + 0xA)) / *(s16 *)(p + 0x30);
        if ((*(s16 *)(p + 0x30) -= 1) <= 0)
        {
            *(s16 *)(p + 0x30) = 0x10;
            *(s16 *)((u8 *)s + 0x1E) = 0x1000;
            *(s32 *)((u8 *)s + 0xC) = 0x808080;
            if (*(s16 *)(p + 0x3C) != 0)
            {
                *(s16 *)(p + 0x2C) = 2;
            }
            else
            {
                *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
            }
        }
        break;
    case 1:
        if (*(s16 *)(p + 0x38) == 0)
        {
            if (func_s55_80026384(*(s16 *)(p + 0x1C), *(s16 *)(p + 0x1E), *(s16 *)(p + 0x20),
                                  *(s16 *)(D_800E3D7C + 0x2A)) == 0)
            {
                break;
            }
            func_s55_80025604(*(s16 *)(p + 0x1C), *(s16 *)(p + 0x1E), *(s16 *)(p + 0x20));
        }
        *(s16 *)(p + 0x30) = 0x10;
        *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
        break;
    case 2:
        c2 = *(u8 *)((u8 *)s + 0xC);
        c2 = c2 - c2 / *(s16 *)(p + 0x30);
        *(u8 *)((u8 *)s + 0xC) = c2;
        *(u8 *)((u8 *)s + 0xD) = c2;
        *(u8 *)((u8 *)s + 0xE) = c2;
        if ((*(s16 *)(p + 0x30) -= 1) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    if (*(s16 *)(p + 0x38) == 0 && *(s16 *)(p + 0x3C) == 0)
    {
        q = D_80028220;
        q[0x9] = q[0x9] + 1;
        if (q[0x9] >= 0x20)
        {
            q[0x9] = 0;
        }
    }
}

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

/* The 0x212 burst: spawn FOUR nodes on the caller, two of one kind and two of
   another, each at a random offset from the caller's own mover. Both loops run
   twice over func_8003FD64(0x212, a) and install this unit's
   func_s55_80025764 as the +0x10 step; the first pair gets D_80045340 through
   func_8004491C and a slot dressed with this bank's D_8002822C at +0x8, the
   caller's colour argument at +0xC, 0x1000 scales and 0x8 at +0x6, while the
   second pair gets func_800C95C0 and a payload armed with 0x20/0x20/1/0 at
   +0x0/+0x2/+0x4/+0x6 instead. Every node's mover is the caller's plus
   `rand() & 0x3F' less 0x20 on each of the three axes, its payload carries the
   caller's heading at +0x36 and a `(rand() & 0xFF) | 0x80' at +0x30.
   138 words, 0 differing.

   NO TWIN at any ratio. dngcallee's sharp hit (4 of 5 symbols, one function:
   csrc/ovl_dng_b03.c's func_b03_801725BC) was NOT needed and NOT used -- the
   whole skeleton is this unit's own landed func_s55_80025B5C 40 words up,
   which spawns a single 0x212 the same way. That is the in-unit-sibling route
   paying again on a body with no twin.

   func_80069EF8 IS `rand' (splat/common_syms.txt), and it is spelled that
   way; 10 of this body's 12 calls are it.

   TWO FACTS, both statement order, both measured here from a body at 0:

   (1) LOOP 1's SLOT PUBLISH MUST BE +0x8, +0xC, +0x1E, +0x1C, +0x6. Five
   orders measured: that one is 0, the sibling func_s55_80025B5C's own order
   (+0x8, +0x1E, +0x1C, +0x6, +0xC) is 4, the LISTING's own emission order
   (+0x1E, +0x1C, +0x8, +0xC, +0x6) is also 4, +0x1E/+0x1C/+0x8/+0x6/+0xC is
   6 and +0x6/+0x8/+0xC/+0x1C/+0x1E is 11. So the position IS measurable, the
   curve is not flat, and the listing's emission order is NOT the source's.
   What it decides is three statements UPSTREAM: with any other order loop 1's
   THIRD position store comes out `lw' / `nop' / `lhu 0xA' / `andi 0x3F' where
   the target has `lw' / `andi 0x3F' / `lhu 0xA' / `nop'. `-da's .sched2 trace
   shows why that is not reachable locally: both schedules are five slots and
   honour every latency, the two insns tie at `priority = 7' and `8', and the
   backward list scheduler fills the slot before the `addu' with whichever of
   them the TRAILING block has left available. Nothing in the three position
   stores themselves moves it; only what the scheduler sees after them does.

   (2) THE `- 0x20' MUST NOT SIT NEXT TO THE u16 LOAD. Written
   `*(u16 *)((u8 *)a->unk_08 + N) - 0x20 + (rand() & 0x3F)' all six position
   stores are 137 WORDS / 73 aligned differences. This is convert.c
   distributing the narrowing truncation into the sum: the inner subtraction
   goes through `trunc1' with `typex = unsigned short', fold rewrites
   `MINUS(half, 32)' as `PLUS(half, 65504)', and `-da's .sched2 confirms it
   exactly -- `(set (reg:SI 3 v1) (const_int 65504))' plus a register-register
   `addsi3' where the target has one `addiu $v1, $v1, -0x20'. Diagnostic as
   the levers file states it: a constant materialised into a register where
   the target has a negative `addiu' immediate.

   MEASURED AND FREE at 138 / 0, so none of these is why this lands:
   `(rand() & 0x3F)' written as the FIRST addend in all six position stores;
   loop 2's +0x0 store written `*(s16 *)((u8 *)n + 0x20) = 0x20' instead of
   through `q'; and both loops written `i = 2; do { ... } while (--i > 0);'
   instead of `for (i = 2; i > 0; i--)' -- the two loop forms are the same
   tree here, since the constant initial test folds away either way, so that
   last one is EQUIVALENT rather than a priced choice. */
void func_s55_8002589C(UnkNode *a, s16 b, s32 c)
{
    UnkNode *n;
    u8 *q;
    u8 *r;
    s32 i;

    for (i = 2; i > 0; i--)
    {
        n = func_8003FD64(0x212, a);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s55_80025764;
            func_8004491C(n, (u32)D_80045340);
            q = (u8 *)n + 0x20;
            *(s16 *)((u8 *)n->unk_08 + 0x2) =
                *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x3F) - 0x20;
            *(s16 *)((u8 *)n->unk_08 + 0x6) =
                *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x3F) - 0x20;
            *(s16 *)((u8 *)n->unk_08 + 0xA) =
                *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x3F) - 0x20;
            r = (u8 *)n->unk_0C;
            *(u32 *)(r + 0x8) = (u32)D_8002822C;
            *(s32 *)(r + 0xC) = c;
            *(s16 *)(r + 0x1E) = 0x1000;
            *(s16 *)(r + 0x1C) = 0x1000;
            *(s16 *)(r + 0x6) = 8;
            *(s16 *)(q + 0x36) = b;
            *(s16 *)(q + 0x30) = (rand() & 0xFF) | 0x80;
        }
    }
    for (i = 2; i > 0; i--)
    {
        n = func_8003FD64(0x212, a);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s55_80025764;
            func_8004491C(n, (u32)func_800C95C0);
            q = (u8 *)n + 0x20;
            *(s16 *)((u8 *)n->unk_08 + 0x2) =
                *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x3F) - 0x20;
            *(s16 *)((u8 *)n->unk_08 + 0x6) =
                *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x3F) - 0x20;
            *(s16 *)((u8 *)n->unk_08 + 0xA) =
                *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x3F) - 0x20;
            *(s16 *)(q + 0x0) = 0x20;
            *(s16 *)(q + 0x2) = 0x20;
            *(s16 *)(q + 0x4) = 1;
            *(s16 *)(q + 0x6) = 0;
            *(s16 *)(q + 0x36) = b;
            *(s16 *)(q + 0x30) = (rand() & 0xFF) | 0x80;
        }
    }
}

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

/* The 0x212 actor's root step, five states on *(s16 *)(p + 0x2C) under a
   top-level gate on the node pointer stashed at the payload's +0x20. With no
   stashed node it bumps this bank's D_800281F8, runs func_s55_8002660C over
   the +0x32 facing and func_s55_80026548 over the slot's element, then
   dispatches: state 0 drags the mover's +0x08 down by 0x10000 a frame, fades
   the slot's three colour bytes toward 0x80 by 1/count, flips the +0x32
   parity and on the count reaching zero -- and only if the slot raised
   0x4000 -- seeds the whole flight from D_800E3D7C (+0x88 ceiling, +0x2A
   direction bits 9..11) and the party position bytes at D_80082E80's
   +0x24/+0x25, and advances; state 1 projects D_80082E80's own slot through
   func_8003DE58 and either offsets the mover by the payload's +0x24 triple
   or snaps it to D_80083780; state 2 walks one tile at a time, asking
   func_800A44E0 whether the next tile along the +0x40 heading is passable,
   easing the mover toward the tile centre by 1/count, clamping the height to
   the ceiling less 0x10, and at count 2 asking func_8009B4B0 for the tile's
   actor and running func_8009CE1C on it; state 0x10 is the ballistic fall.
   With a stashed node the step just mirrors that node's mover and slot. The
   tail installs func_800C9034 once. 449 words, 0 differing.

   This is docs/dng-parks/func_s55_80025C80.c resurrected, and the park's
   449-word body was correct except for ONE access spelling. Its residue was
   2 aligned differences, a transposed pair in the delay slot of case 0's
   0x4000 gate -- the target puts `addiu $a1, $zero, 0x8' there and the park
   put the head of the D_800E3D7C load chain. The park diagnosed a sched2
   INSN_LUID tie and concluded it was unreachable from the statement order.
   The diagnosis is right and the conclusion is wrong.

   THE ONE FACT, and it is a dependence not an allocation: case 0's p+0x38
   store must be the ARRAY REF `((s16 *)p)[0x1C] = 8'. As the plain cast
   `*(s16 *)(p + 0x38) = 8' it is 449 words / 2. Read as an array ref the MEM
   gets MEM_IN_STRUCT_P (it needs a PLUS_EXPR on top -- the same mechanism
   func_s55_80025764 records in this unit, with the polarity reversed), so
   alias.c's fixed-scalar-versus-varying-struct rule drops the edge between
   this store and the FIXED-address read of D_800E3D7C beside it. Without
   MEM_IN_STRUCT_P that edge forces a sched order in which the `high' insn
   and the constant 8 cannot trade places.

   WHY `-da' SETTLED IT in one compile, which the park records as NOT TRIED:
   the .sched2 ready-list trace prints both insns at `priority = 1' and shows
   that insn_cost for the `high'->`lw' link is 1, so rank_for_schedule's
   last-scheduled CLASS test cannot separate them either (`link == 0 ||
   insn_cost (...) == 1' both give class 3) and the tie falls straight to
   INSN_LUID -- i.e. to source order after all. No device is involved.

   MEASURED, the 2x2, and it is NOT a conjunction: with the array ref the
   statement's POSITION is FREE -- 449 / 0 with the store either side of
   `r = D_800E3D7C'. With the plain cast BOTH positions are 449 / 2, and they
   are DIFFERENT two rows: after the read it is the delay-slot pair above,
   before it the store itself moves two slots early, because sched then has a
   real store-to-load edge to honour. The park recorded that second spelling
   as "MEASURED AND FREE ... case 0's p+0x38 store ahead of the D_800E3D7C
   read" on the strength of the unchanged COUNT; it is a different fault.

   TWO OF THE PARK'S STATEMENT-ORDER FACTS RE-PRICED from this body at 0, and
   both HELD two rows cheaper than the park's mid-fight figures:
   `*(s16 *)(b+0x16) = 8' before the b+0x12 store is 450 words / 12 (park:
   450/14), and `D_800281F8 += 1' before the p+0x20 load is 450 / 10 (park:
   450/12). The second is this family's recurring split-decrement fact and it
   lands on the AFTER side here, as it does in func_s55_80025AC4 above.
   The park's other seven rows are INHERITED, not re-priced here. */
void func_s55_80025C80(u8 *p, UnkVec *b, UnkSlot *s)
{
    UnkNode *o;
    UnkSlot *q;
    UnkVec *m;
    u8 *g;
    u8 *g2;
    u8 *r;
    u8 *r2;
    u8 *r3;
    u8 *e;
    u8 c;
    s32 t;
    s32 t2;
    s32 u;
    s32 u2;

    o = *(UnkNode **)(p + 0x20);
    D_800281F8 += 1;
    if (o == NULL)
    {
        func_s55_8002660C((u8 *)s, (s16 *)(p + 0x32), *(s16 *)(p + 0x38), *(s16 *)(p + 0x36));
        func_s55_80026548(p, s->unk_00);
        switch (*(s16 *)(p + 0x2C))
        {
        case 0:
            b->unk_08 -= 0x10000;
            c = *(u8 *)((u8 *)s + 0xC);
            c = c + (0x80 - c) / *(s16 *)(p + 0x30);
            *(u8 *)((u8 *)s + 0xC) = c;
            *(u8 *)((u8 *)s + 0xE) = c;
            *(u8 *)((u8 *)s + 0xD) = c;
            *(s16 *)(p + 0x32) = (*(u16 *)(p + 0x30) ^ 1) & 1;
            *(s16 *)(p + 0x30) = *(u16 *)(p + 0x30) - 1;
            if (*(s16 *)(p + 0x30) > 0)
            {
                break;
            }
            if ((s->unk_14 & 0x4000) == 0)
            {
                break;
            }
            r = D_800E3D7C;
            ((s16 *)p)[0x1C] = 8;
            *(s16 *)(p + 0x36) = 0xF;
            *(s16 *)(p + 0x32) = 1;
            *(s16 *)(p + 0x3A) = *(u16 *)(r + 0x88);
            *(s16 *)(p + 0x40) = (*(u16 *)(r + 0x2A) >> 9) & 7;
            g = D_80082E80;
            *(s16 *)(p + 0x3C) = *(u8 *)(g + 0x24);
            *(s16 *)(p + 0x3E) = *(u8 *)(g + 0x25);
            *(s16 *)(p + 0x42) = 8;
            *(s16 *)(p + 0x30) = 3;
            *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
            break;
        case 1:
            g2 = D_80082E80;
            e = func_8003DE58(*(u8 **)(g2 + 0x8), (UnkSlot *)g2, (s16 *)(p + 0x24), 0);
            *(s16 *)(p + 0x30) = *(u16 *)(p + 0x30) - 1;
            if (*(s16 *)(p + 0x30) <= 0 || e == NULL || (*(u16 *)(g2 + 0x14) & 0x8000) != 0)
            {
                if ((D_80082E94 & 0x8000) != 0)
                {
                    r2 = D_80083780;
                    *(s16 *)((u8 *)b + 0x2) = *(u16 *)(r2 + 0x2);
                    *(s16 *)((u8 *)b + 0x6) = *(u16 *)(r2 + 0x6);
                    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(r2 + 0xA) - 0x10;
                }
                *(s16 *)(p + 0x30) = 0;
                *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
            }
            else
            {
                r3 = D_80083780;
                *(s16 *)((u8 *)b + 0x2) = *(u16 *)(r3 + 0x2) + *(u16 *)(p + 0x24);
                *(s16 *)((u8 *)b + 0x6) = *(u16 *)(r3 + 0x6) + *(u16 *)(p + 0x26);
                *(s16 *)((u8 *)b + 0xA) = *(u16 *)(r3 + 0xA) + *(u16 *)(p + 0x28);
            }
            break;
        case 2:
            if (*(s16 *)(p + 0x30) == 0)
            {
                if ((s16)func_800A44E0((*(s16 *)(p + 0x3C) * 64 + 0x20) & 0xFFFFFFE0,
                                       (*(s16 *)(p + 0x3E) * 64 + 0x20) & 0xFFFFFFE0,
                                       (s16)(*(u16 *)(p + 0x3A) - 0x20),
                                       (s16)(*(u16 *)(p + 0x40) << 9)) != 0 ||
                    (*(s16 *)(p + 0x42) = *(u16 *)(p + 0x42) - 1) <= 0)
                {
                    *(s16 *)(p + 0x2C) = 0x10;
                    *(s16 *)((u8 *)b + 0xE) = D_8006CCD8[*(s16 *)(p + 0x40)] * 8;
                    *(s16 *)((u8 *)b + 0x12) = D_8006CCE8[*(s16 *)(p + 0x40)] * 8;
                    *(s16 *)((u8 *)b + 0x16) = 8;
                    *(s16 *)(p + 0x30) = 8;
                }
                else
                {
                    *(s16 *)(p + 0x3C) = *(u16 *)(p + 0x3C) + (u16)D_8006CCD8[*(s16 *)(p + 0x40)];
                    *(s16 *)(p + 0x3E) = *(u16 *)(p + 0x3E) + (u16)D_8006CCE8[*(s16 *)(p + 0x40)];
                    *(s16 *)(p + 0x30) = 3;
                }
            }
            u = *(s16 *)(p + 0x3C) * 64;
            t = *(s16 *)((u8 *)b + 0x2) - 0x20;
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + (u - t) / *(s16 *)(p + 0x30);
            u2 = *(s16 *)(p + 0x3E) * 64;
            t2 = *(s16 *)((u8 *)b + 0x6) - 0x20;
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + (u2 - t2) / *(s16 *)(p + 0x30);
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + 5;
            if (*(s16 *)((u8 *)b + 0xA) > *(s16 *)(p + 0x3A) - 0x10)
            {
                *(s16 *)((u8 *)b + 0xA) = *(u16 *)(p + 0x3A) - 0x10;
            }
            *(s16 *)(p + 0x30) = *(u16 *)(p + 0x30) - 1;
            if (*(s16 *)(p + 0x30) == 2)
            {
                e = func_8009B4B0(D_800E3D7C, *(u16 *)(p + 0x3C), *(u16 *)(p + 0x3E));
                if (e != NULL)
                {
                    func_8009CE1C(e, 0xC, D_8002966E, 9, (s16)(*(u16 *)(p + 0x40) << 9), D_80029670, 5);
                }
            }
            func_s55_8002589C((UnkNode *)(p - 0x20), *(s16 *)(p + 0x40), 0xF0);
            break;
        case 0x10:
            b->unk_00 += b->unk_0C;
            b->unk_04 += b->unk_10;
            b->unk_08 += b->unk_14;
            b->unk_0C -= b->unk_0C >> 2;
            b->unk_10 -= b->unk_0C >> 2;
            if (*(s16 *)(p + 0x3A) - 0x10 < *(s16 *)((u8 *)b + 0xA))
            {
                *(s16 *)((u8 *)b + 0xA) = *(u16 *)(p + 0x3A) - 0x10;
            }
            *(u8 *)((u8 *)s + 0xC) = *(u8 *)((u8 *)s + 0xC) - *(u8 *)((u8 *)s + 0xC) / *(s16 *)(p + 0x30);
            *(u8 *)((u8 *)s + 0xD) = *(u8 *)((u8 *)s + 0xD) - *(u8 *)((u8 *)s + 0xD) / *(s16 *)(p + 0x30);
            *(u8 *)((u8 *)s + 0xE) = *(u8 *)((u8 *)s + 0xE) - *(u8 *)((u8 *)s + 0xE) / *(s16 *)(p + 0x30);
            *(s16 *)(p + 0x30) = *(u16 *)(p + 0x30) - 1;
            if (*(s16 *)(p + 0x30) <= 0)
            {
                D_8002966C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
                return;
            }
            if (*(s8 *)((u8 *)s + 0x4) == *(s16 *)(p + 0x36))
            {
                *(s16 *)(p + 0x32) = -1;
            }
            else if (*(s8 *)((u8 *)s + 0x4) == *(s16 *)(p + 0x38))
            {
                *(s16 *)(p + 0x32) = 1;
            }
            func_s55_8002589C((UnkNode *)(p - 0x20), *(s16 *)(p + 0x40), 0xF0);
            break;
        }
    }
    else if ((o->unk_1E & 0x8000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return;
    }
    else
    {
        m = o->unk_08;
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)m + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)m + 0x6);
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)m + 0xA);
        q = (UnkSlot *)(*(UnkNode **)(p + 0x20))->unk_0C;
        func_s55_800266D0((u8 *)s, *(s8 *)((u8 *)q + 0x4));
        func_s55_80026548(p, s->unk_00);
        *(s16 *)((u8 *)s + 0x1C) = *(u16 *)((u8 *)q + 0x1C);
        *(s16 *)((u8 *)s + 0x1E) = *(u16 *)((u8 *)q + 0x1E);
        *(s16 *)((u8 *)s + 0x20) = *(u16 *)((u8 *)q + 0x20);
        *(s32 *)((u8 *)s + 0xC) = *(s32 *)((u8 *)q + 0xC);
        *(s16 *)((u8 *)s + 0x16) = *(u16 *)((u8 *)q + 0x16);
        *(s16 *)((u8 *)s + 0x18) = *(u16 *)((u8 *)q + 0x18);
        *(s16 *)((u8 *)s + 0x1A) = *(u16 *)((u8 *)q + 0x1A);
    }
    if (*(s16 *)(p + 0x2E) == 0)
    {
        func_8004491C((UnkNode *)(p - 0x20), (u32)func_800C9034);
        *(s16 *)(p + 0x2E) = *(u16 *)(p + 0x2E) + 1;
    }
}

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
