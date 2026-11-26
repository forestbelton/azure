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
UnkNode *func_s55_80024ED4(UnkVec *b);
s32 func_80065F90(s32, s32);
s32 func_80064710(s32);

void func_s55_8002660C(u8 *s, s16 *b, s16 lo, s16 hi);
void func_s55_800266D0(u8 *s, s16 d);
void func_s55_80026548(u8 *p, UnkElem *e);
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
s32 func_s55_80026384(s16 x, s16 y, s16 z, s16 h);
UnkNode *func_s55_80025604(s16 x, s16 y, s16 z);
extern u8 D_80028220[];
s32 func_800C95C0(u8 *, UnkVec *, UnkSlot *);
extern u8 D_8002822C[];

void func_s55_80025528(u8 *p, UnkVec *b);
s32 func_80064584(s32);
s32 func_800644B8(s32);
void func_8003E0C4(void *, void *, void *);
s32 func_8003FA44(s32);
extern u8 D_80028268[];
extern u8 D_80083498[];
s16 func_800A07D0(s16, s16, s16, s16);
extern u8 D_80028214[];
s32 func_80045C34(u8 *, UnkVec *, UnkSlot *);
void func_s55_80024728(u8 *p, UnkVec *b, UnkSlot *s);
UnkNode *func_s55_80025344(s16 x, s16 y, s16 z, s16 r);
extern s16 D_80083228;
extern u8 *D_800E3D18;
s32 func_800CEEFC(u8 *, UnkVec *, UnkSlot *);

/* Rotate an SVECTOR by the heading in `h' and project it: build a MATRIX with
   RotMatrix and a zero translation, then hand it, a two-word SVECTOR holding
   {0, h << 12}, and the caller's output vector to csrc/unk_113D0.c's
   func_8003E0C4, which is PushMatrix / SetTransMatrix / SetRotMatrix /
   RotTrans / PopMatrix. Its one caller is func_s55_80024728's state 1, which
   passes the payload's sign-extended +0xC..+0x10 triple and gets the world
   delta back at a stack VECTOR. 21 words, 0 differing.

   func_80065820 IS `RotMatrix' (splat/common_syms.txt), so the call needs no
   declaration at all and the first parameter's type is read off the library
   prototype rather than guessed. The project had no name for func_8003E0C4;
   its definition is csrc/unk_113D0.c's and its signature is copied from there.

   THE ONE FACT: the three translation stores must be `m.t[2]', `m.t[1]',
   `m.t[0]' -- DESCENDING. Ascending they emit ascending and it is 21 words /
   2 aligned differences. Adjacent independent stores are an order to price,
   not to read, and here the listing's order IS the source's.

   The frame fixes the storage: locals occupy sp+0x10..0x37, which is 40 bytes
   -- a 32-byte MATRIX plus 8. The 8-byte object takes two WORD stores
   (sw $zero, 0x30 and sw $v0, 0x34), so it is a two-element `s32' array
   cast at the call, which is this unit's own idiom in func_s55_80024578 and
   func_s55_80025B5C, and not an SVECTOR with halfword field assignments.
   `h' is `s32': the shift is a bare `sll $v0, $a1, 12' with no sll/sra
   promotion pair in front of it. */
void func_s55_80024024(SVECTOR *r, s32 h, void *out)
{
    MATRIX m;
    s32 arr[2];

    m.t[2] = 0;
    m.t[1] = 0;
    m.t[0] = 0;
    arr[0] = 0;
    arr[1] = h << 12;
    RotMatrix(r, &m);
    func_8003E0C4(&m, arr, out);
}

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

/* The 0x12 chain link's +0x10 step and the biggest body in this unit: three
   states on *(s16 *)(p + 0x2C), under a top-level split on whether the
   payload's +0x8 holds a predecessor link.

   State 0 installs func_800CEEFC through func_8004491C and FALLS THROUGH into
   state 1. State 1, with no predecessor, flies the head of the chain: it
   turns the vector from the mover to the payload's +0x14/+0x16/+0x18 target
   into a bearing pair through this unit's func_s55_80024654, sign-extends the
   twelve-bit current bearing at +0xC/+0xE/+0x10 and the new one, splices the
   new bearing's top nibble back from the old one whenever the two differ by
   0x801 or more (the wrap guard), eases the current bearing a quarter of the
   way towards it, and turns that into a world delta through this unit's
   func_s55_80024024, which it adds onto the mover. When the +0x30 countdown
   runs out it re-seeds the target from D_80083780 and the party's own slot,
   projected through func_8003DE58 against the direction-indexed sheet at
   D_800E3D18, and raises +0x2E. With +0x2E at 1 it instead drags the mover
   onto the target by 1/count, and at the end of that count snaps it there,
   spawns the burst through this unit's func_s55_80025344 four times -- once
   with heading 0 and then with 3 and 2 -- and advances. With a predecessor it
   follows that link's mover a quarter of the way, takes the darker of the two
   slots' three colour bytes, and retires when the link has. State 2 is the
   fade. Every state that does not retire eases the mover's velocity triple a
   quarter of the way towards its position triple, on alternate frames of the
   +0x34 counter -- a block the target has THREE TIMES, once per arm, because
   each copy falls into a different continuation and jump2 cannot cross-join
   them. 491 words, 0 differing.

   NO TWIN at any ratio, and jtcheck reports no table in this unit: the
   three-case switch is a compare chain. Written from the listing on this
   unit's own landed bodies -- func_s55_80025C80 for the switch skeleton and
   the `((s16 *)p)[N]' array-ref idiom, func_s55_800250B4 for the 1/count
   ease, func_s55_80025AC4 for the retire pair -- plus the two functions I
   landed just before it, func_s55_80024024 and func_s55_80025344, which are
   its own callees. That is the in-unit route at its strongest: the two
   callees' parameter lists are what fixed this body's argument types, and it
   came in at 492 words / 9 aligned differences on the FIRST COMPILE.

   TWO FACTS CARRIED IT FROM 9 TO 0, and both are already in this unit.

   (1) THE +0x34 COUNTER BUMP MUST BE THE ARRAY REF
   `((s16 *)p)[0x1A] = ((u16 *)p)[0x1A] + 1'. As the plain cast
   `*(s16 *)(p + 0x34) = *(u16 *)(p + 0x34) + 1' it is 491 words / 5 aligned
   differences: the `sh %lo(D_800281F8)' cannot move below the +0x34 load and
   the whole five-instruction prologue group emits in a different order.
   MEM_IN_STRUCT_P on the varying store lets alias.c's
   fixed-scalar-versus-varying-struct rule drop the edge to the fixed-address
   counter. Identical mechanism, identical polarity and the same `D_800281F8'
   partner as func_s55_80025528 above -- two bodies, one cause, which is the
   difference between a number and an explanation.

   (2) STATE 2's COLOUR NEEDS A `u8 c' LOCAL. Written as a read-back --
   `*(u8 *)(s + 0xD) = *(u8 *)(s + 0xC)' -- it is 492 WORDS, one long, and 4
   aligned differences, the extra word a `move' the target does not have.
   Same class as func_s55_80024ED4's three read locals, and the same spelling
   the landed func_s55_800250B4 uses forty listings up.

   PRICED FROM THE BODY AT 0, LOAD-BEARING:
     the twelve-bit sign extension must be a CONDITIONAL EXPRESSION with ONE
     store. Writing just the FIRST of its six sites as an `if'/`else' with a
     store in both arms is 492 / 6 -- the same `reg_n_sets'/cross-join
     mechanism as func_s55_80025604's, which needed the identical form.
     the wrap guard must be `__builtin_abs(...) >= 0x801'; as two
     comparisons it is 489 words / 7.
     D_80083780 must be a pointer local in the re-seed: inline it is
     490 / 13.
     the sheet must be an ARRAY SUBSCRIPT,
     `((u8 **)D_800E3D18)[(... >> 9) & 7]'. Transcribing the folded form the
     listing shows -- `*(u8 **)(D_800E3D18 + ((... >> 7) & 0x1C))' -- is
     491 / 2. combine folds `(y & 7) * 4' into `(x >> 7) & 0x1C'; the source
     cannot be read back off it.
     the predecessor's mover and slot must be reached by RE-READING
     `*(UnkNode **)(p + 0x8)', not through the local `n' the NULL gate uses.
     Through `n' each is 489 words / 8, and the two faults are independent.
     The same fact, the same way round, is in func_s55_80025AC4 above.

   MEASURED AND FREE at 491 / 0, so none of these is why it lands:
     the sign-extension mask written `| 0xF800' instead of `| 0xFFFFF800' at
     all six sites -- the store is a halfword, so combine narrows either one;
     `r2 = D_800E3D7C' as a pointer local versus the symbol inline at both
     reads (the OPPOSITE of D_80083780 thirty words away, which is 13 rows --
     two symbols in one block wanting different answers);
     `for (j = 3; j > 1; j--)' for `j >= 2', which is the same tree;
     and `n->unk_1E' written as the plain cast `*(u16 *)((u8 *)n + 0x1E)'.
     THAT LAST ONE IS AN INVERSION OF A SIBLING'S FACT AND WORTH THE INK:
     func_s55_80025AC4 in this same unit prices the plain cast at 39 words /
     17 and insists on the COMPONENT_REF, and csrc/ovl_dng_s25.c records the
     same for four sites. Here it is free in both directions, because the
     gate sits in a block of its own with a CODE_LABEL between it and the
     fixed-address store it would otherwise have to serialise against. So
     "what the rest of this unit does" is not evidence, even for
     MEM_IN_STRUCT_P. */
void func_s55_80024728(u8 *p, UnkVec *b, UnkSlot *s)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *r2;
    u8 *g;
    u8 c;
    s32 j;
    s16 d[3];
    s32 v[3];
    s16 o[3];

    D_800281F8 += 1;
    ((s16 *)p)[0x1A] = ((u16 *)p)[0x1A] + 1;
    switch (*(s16 *)(p + 0x2C))
    {
    case 0:
        func_8004491C((UnkNode *)(p - 0x20), (u32)func_800CEEFC);
        *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
    case 1:
        n = *(UnkNode **)(p + 0x8);
        if (n == NULL)
        {
            if (*(s16 *)(p + 0x2E) == 0)
            {
                func_s55_80024654(*(s16 *)((u8 *)b + 0x2), *(s16 *)((u8 *)b + 0x6),
                                  *(s16 *)((u8 *)b + 0xA), *(s16 *)(p + 0x14),
                                  *(s16 *)(p + 0x16), *(s16 *)(p + 0x18), o);
                *(s16 *)(p + 0xC) = ((*(u16 *)(p + 0xC) & 0x800) != 0) ? (*(u16 *)(p + 0xC) | 0xFFFFF800) : (*(u16 *)(p + 0xC) & 0x7FF);
                *(s16 *)(p + 0xE) = ((*(u16 *)(p + 0xE) & 0x800) != 0) ? (*(u16 *)(p + 0xE) | 0xFFFFF800) : (*(u16 *)(p + 0xE) & 0x7FF);
                *(s16 *)(p + 0x10) = ((*(u16 *)(p + 0x10) & 0x800) != 0) ? (*(u16 *)(p + 0x10) | 0xFFFFF800) : (*(u16 *)(p + 0x10) & 0x7FF);
                o[0] = ((*(u16 *)&o[0] & 0x800) != 0) ? (*(u16 *)&o[0] | 0xFFFFF800) : (*(u16 *)&o[0] & 0x7FF);
                o[1] = ((*(u16 *)&o[1] & 0x800) != 0) ? (*(u16 *)&o[1] | 0xFFFFF800) : (*(u16 *)&o[1] & 0x7FF);
                o[2] = ((*(u16 *)&o[2] & 0x800) != 0) ? (*(u16 *)&o[2] | 0xFFFFF800) : (*(u16 *)&o[2] & 0x7FF);
                if (__builtin_abs(o[0] - *(s16 *)(p + 0xC)) >= 0x801)
                {
                    o[0] = (*(u16 *)(p + 0xC) & 0xF000) | (o[0] & 0xFFF);
                }
                if (__builtin_abs(o[1] - *(s16 *)(p + 0xE)) >= 0x801)
                {
                    o[1] = (*(u16 *)(p + 0xE) & 0xF000) | (o[1] & 0xFFF);
                }
                if (__builtin_abs(o[2] - *(s16 *)(p + 0x10)) >= 0x801)
                {
                    o[2] = (*(u16 *)(p + 0x10) & 0xF000) | (o[2] & 0xFFF);
                }
                *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) + ((o[0] - *(s16 *)(p + 0xC)) >> 2);
                *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + ((o[1] - *(s16 *)(p + 0xE)) >> 2);
                *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + ((o[2] - *(s16 *)(p + 0x10)) >> 2);
                func_s55_80024024((SVECTOR *)(p + 0xC), *(s16 *)(p + 0x3A), v);
                b->unk_00 += v[0];
                b->unk_04 += v[1];
                b->unk_08 += v[2];
                *(s16 *)(p + 0x3A) = 0xA0;
                if ((*(s16 *)(p + 0x30) -= 1) <= 0)
                {
                    g = D_80083780;
                    o[0] = *(u16 *)(g + 0x2);
                    o[1] = *(u16 *)(g + 0x6);
                    r2 = D_800E3D7C;
                    o[2] = *(u16 *)(r2 + 0x88) - 0x50;
                    func_8003DE58(((u8 **)D_800E3D18)[((D_80083228 + *(s16 *)(r2 + 0x2A) + 0x100) >> 9) & 7],
                                  (UnkSlot *)D_80082E80, d, 0);
                    o[0] = o[0] + d[0];
                    *(s16 *)(p + 0x14) = o[0];
                    o[1] = o[1] + d[1];
                    *(s16 *)(p + 0x16) = o[1];
                    o[2] = o[2] + d[2];
                    *(s16 *)(p + 0x18) = o[2];
                    *(s16 *)(p + 0x2E) = 1;
                    *(s16 *)(p + 0x30) = 8;
                }
                if (*(u8 *)((u8 *)s + 0xC) < 0x80)
                {
                    *(s32 *)((u8 *)s + 0xC) += 0x80808;
                }
            }
            else if (*(s16 *)(p + 0x2E) == 1)
            {
                if (*(s16 *)(p + 0x30) > 0)
                {
                    *(s16 *)((u8 *)b + 0x2) =
                        *(u16 *)((u8 *)b + 0x2) +
                        (*(s16 *)(p + 0x14) - *(s16 *)((u8 *)b + 0x2)) / *(s16 *)(p + 0x30);
                    *(s16 *)((u8 *)b + 0x6) =
                        *(u16 *)((u8 *)b + 0x6) +
                        (*(s16 *)(p + 0x16) - *(s16 *)((u8 *)b + 0x6)) / *(s16 *)(p + 0x30);
                    *(s16 *)((u8 *)b + 0xA) =
                        *(u16 *)((u8 *)b + 0xA) +
                        (*(s16 *)(p + 0x18) - *(s16 *)((u8 *)b + 0xA)) / *(s16 *)(p + 0x30);
                }
                if ((*(s16 *)(p + 0x30) -= 1) <= 0)
                {
                    *(s16 *)(p + 0x30) = 0;
                    *(s16 *)((u8 *)b + 0x2) = *(u16 *)(p + 0x14);
                    *(s16 *)((u8 *)b + 0x6) = *(u16 *)(p + 0x16);
                    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(p + 0x18);
                    if (func_s55_80025344(*(s16 *)(p + 0x14), *(s16 *)(p + 0x16),
                                          *(u16 *)(p + 0x18) + 0x10, 0) != NULL)
                    {
                        for (j = 3; j >= 2; j--)
                        {
                            func_s55_80025344(*(s16 *)(p + 0x14), *(s16 *)(p + 0x16),
                                              *(u16 *)(p + 0x18) + 0x10, j);
                        }
                        *(s16 *)(p + 0x30) = 8;
                        *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
                    }
                }
            }
            if ((*(u16 *)(p + 0x34) & 1) != 0)
            {
                b->unk_0C += (b->unk_00 - b->unk_0C) >> 2;
                b->unk_10 += (b->unk_04 - b->unk_10) >> 2;
                b->unk_14 += (b->unk_08 - b->unk_14) >> 2;
            }
        }
        else
        {
            if ((n->unk_1E & 0x8000) != 0)
            {
                *(s16 *)(p + 0x30) = 2;
                *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
            }
            m = (*(UnkNode **)(p + 0x8))->unk_08;
            b->unk_00 += (m->unk_00 - b->unk_00) >> 2;
            b->unk_04 += (m->unk_04 - b->unk_04) >> 2;
            b->unk_08 += (m->unk_08 - b->unk_08) >> 2;
            if ((*(u16 *)(p + 0x34) & 1) != 0)
            {
                b->unk_0C += (b->unk_00 - b->unk_0C) >> 2;
                b->unk_10 += (b->unk_04 - b->unk_10) >> 2;
                b->unk_14 += (b->unk_08 - b->unk_14) >> 2;
            }
            r = (u8 *)(*(UnkNode **)(p + 0x8))->unk_0C;
            if (*(u8 *)(r + 0xC) < *(u8 *)((u8 *)s + 0xC))
            {
                *(u8 *)((u8 *)s + 0xC) = *(u8 *)(r + 0xC);
            }
            if (*(u8 *)(r + 0xD) < *(u8 *)((u8 *)s + 0xD))
            {
                *(u8 *)((u8 *)s + 0xD) = *(u8 *)(r + 0xD);
            }
            if (*(u8 *)(r + 0xE) < *(u8 *)((u8 *)s + 0xE))
            {
                *(u8 *)((u8 *)s + 0xE) = *(u8 *)(r + 0xE);
            }
            if (*(u8 *)((u8 *)s + 0xC) < 0x80 - (*(s16 *)(p + 0x38) << 3))
            {
                *(s32 *)((u8 *)s + 0xC) += 0x80808;
            }
        }
        break;
    case 2:
        if ((*(u16 *)(p + 0x34) & 1) != 0)
        {
            b->unk_0C += (b->unk_00 - b->unk_0C) >> 2;
            b->unk_10 += (b->unk_04 - b->unk_10) >> 2;
            b->unk_14 += (b->unk_08 - b->unk_14) >> 2;
        }
        c = *(u8 *)((u8 *)s + 0xC);
        c = c - c / *(s16 *)(p + 0x30);
        *(u8 *)((u8 *)s + 0xC) = c;
        *(u8 *)((u8 *)s + 0xD) = c;
        *(u8 *)((u8 *)s + 0xE) = c;
        if ((*(s16 *)(p + 0x30) -= 1) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* Spawn the 0x12 chain: sixteen nodes, each stepped by this unit's
   func_s55_80024728, threaded onto each other through the payload's +0x8 and
   numbered at +0x38. The first link is allocated against the resident
   D_80083498 and the rest against their predecessor, and the first is kind
   0x12 where the rest are 0x212. Every node's mover gets the caller's
   position in BOTH its +0x2/+0x6/+0xA and its +0xE/+0x12/+0x16 triple, its
   slot gets this bank's D_80028214 with 0x800 scales, and its payload a 0x10
   countdown. Only link 0 seeds the flight: it offsets the target point by
   func_80064584/func_800644B8 of the bearing func_800A07D0 returns from
   D_80083780 to the caller, drops 0x40 on the third axis, runs
   func_s55_80024654 to turn that into the +0x2C bearing pair, and dresses
   through func_800B835C. It returns the last node, or the previous one if an
   allocation fails. 120 words, 0 differing.

   NO TWIN -- dngtwin reports no carved sibling anywhere in this unit. Written
   off this unit's own landed func_s55_8002589C and func_s55_80025B5C for the
   `func_8003FD64' / `n->unk_10' / `q = (u8 *)n + 0x20' spawn skeleton, and
   off func_s55_80025C80 for the `r = D_80083780' pointer-local idiom.
   func_800A07D0's `s16' signature is csrc/ovl_dungeon_800CABA8.c's.

   FOUR FACTS, all measured here from a body at 0.

   (1) THE THREE MOVER READS NEED THREE SEPARATE LOCALS, one per axis. With
   ONE `u16 v' reused for all three it is 120 words / 2 aligned differences --
   the `lw $s0, 0x8($s2)' that loads the mover moves one slot later. A local
   assigned three times carries three `reg_n_sets' and loses
   `adjust_priority's `birthing_insn_p' boost; three single-set locals keep
   it. Declaring the shared local `s16' instead of `u16' is the same 2, so it
   is the SET COUNT and not the width. SIXTEEN spellings of the surrounding
   dress block -- both access forms for `n->unk_08' and `n->unk_10' crossed
   with four statement orders -- all measure the IDENTICAL 120 / 2, which is
   how I know the two rows are not a dependence or an order.
   NOT MEASURABLE, and worth saying so rather than calling it free: writing
   the +0xE/+0x12/+0x16 stores as read-backs of the +0x2/+0x6/+0xA stores
   with no locals at all is also 120 / 0. cse forwards the stored value, so
   the emitted code is identical and the tools cannot choose between them.

   (2) D_80083780 MUST BE A POINTER LOCAL. Written inline at both reads it is
   120 / 3: cc1psx keeps the `%hi' and folds the `%lo' into the first load's
   offset, so the second load comes off a base that is `symbol + 2' and the
   `addiu $v0, $v0, %lo(D_80083780)' the target has disappears. This is the
   folded-offset trap the brief names, seen from the other side.

   (3) THE ALLOCATION FAILURE MUST BE AN `else' ARM, not an early return.
   `if (n == NULL) { return prev; }' before the body is 119 WORDS -- one
   short -- and 9 aligned differences, because gcc then puts the return
   INLINE after the gate where the target has it out of line between the loop
   body and the loop increment. One `else' moves all nine.

   (4) THE KIND TERNARY IS `(i != 0) ? 0x212 : 0x12'. Written the natural way
   round, `(i == 0) ? 0x12 : 0x212', it is 120 / 3 -- the branch opcode
   inverted and the two `addiu's exchanged. The same inversion holds in
   func_s55_80025344 below at the same price, so it is the family's spelling
   and not this body's. */
UnkNode *func_s55_80024ED4(UnkVec *b)
{
    UnkNode *n;
    UnkNode *prev;
    UnkVec *m;
    u8 *r;
    u8 *q;
    u8 *g;
    u16 v;
    u16 v2;
    u16 v3;
    s16 h;
    s32 i;
    s32 arr[2];

    prev = NULL;
    g = D_80083780;
    h = func_800A07D0(*(s16 *)(g + 0x2), *(s16 *)(g + 0x6), *(s16 *)((u8 *)b + 0x2),
                      *(s16 *)((u8 *)b + 0x6));
    for (i = 0; i < 0x10; i++)
    {
        n = func_8003FD64((i != 0) ? 0x212 : 0x12,
                          (prev != NULL) ? (void *)prev : (void *)D_80083498);
        if (n != NULL)
        {
            q = (u8 *)n + 0x20;
            n->unk_10 = (u32)func_s55_80024728;
            m = n->unk_08;
            v = *(u16 *)((u8 *)b + 0x2);
            *(s16 *)((u8 *)m + 0x2) = v;
            *(s16 *)((u8 *)m + 0xE) = v;
            v2 = *(u16 *)((u8 *)b + 0x6);
            *(s16 *)((u8 *)m + 0x6) = v2;
            *(s16 *)((u8 *)m + 0x12) = v2;
            v3 = *(u16 *)((u8 *)b + 0xA);
            *(s16 *)((u8 *)m + 0xA) = v3;
            *(s16 *)((u8 *)m + 0x16) = v3;
            r = (u8 *)n->unk_0C;
            *(u32 *)(r + 0x8) = (u32)D_80028214;
            *(s16 *)(r + 0x1E) = 0x800;
            *(s16 *)(r + 0x1C) = 0x800;
            *(s16 *)(q + 0x30) = 0x10;
            if (i == 0)
            {
                *(s16 *)(q + 0x14) = *(u16 *)((u8 *)m + 0x2) + (func_80064584(h) >> 5);
                *(s16 *)(q + 0x16) = *(u16 *)((u8 *)m + 0x6) + (func_800644B8(h) >> 5);
                *(s16 *)(q + 0x18) = *(u16 *)((u8 *)m + 0xA) - 0x40;
                func_s55_80024654(*(s16 *)((u8 *)m + 0x2), *(s16 *)((u8 *)m + 0x6),
                                  *(s16 *)((u8 *)m + 0xA), *(s16 *)(q + 0x14),
                                  *(s16 *)(q + 0x16), *(s16 *)(q + 0x18),
                                  (s16 *)((u8 *)n + 0x2C));
                arr[0] = 0x1200340;
                arr[1] = 0x200020;
                func_800B835C(D_800DF334, arr, 1, 0);
            }
            *(UnkNode **)(q + 0x8) = prev;
            prev = n;
            *(s16 *)(q + 0x38) = i;
        }
        else
        {
            return prev;
        }
    }
    return n;
}

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

/* Spawn the 0x212 ring: sixteen nodes stepped by this unit's landed
   func_s55_800250B4 with func_80045C34 through func_8004491C, each one placed
   at (x, y, z) offset by cos/sin of i * 0x100 scaled by the caller's `r', its
   slot given this bank's D_80028220 at +0x8 with a 0x1000 scale and an
   i * 0x100 + 0x400 rotation, its +0x16/+0x18 tilt pair set from the same
   trig divided by six at a 12-bit fixed point, and its payload armed with the
   same point, the ring index at +0x38 and `r' / `r' + 4 at +0x3C / +0x30.
   It returns the last node. 121 words, 0 differing.

   NO TWIN. Written off this unit's own landed func_s55_8002589C and
   func_s55_80025B5C for the spawn skeleton; func_80045C34's signature is
   csrc/ovl_dungeon_800CABA8.c's. Its one caller is func_s55_80024728's
   state 1, below, which is where the four `s16' parameters come from.

   FIVE FACTS, all measured here from a body at 0.

   (1) THE DIVISION IS BY 6 AND THE `>> 12' IS SOURCE. There is no `div' in
   the listing, so the divisor is a literal; the magic is
   `lui 0x2AAA' / `ori 0xAAAB' = 0x2AAAAAAB, which is ceil(2^32 / 6), and
   gcc 2.7.2's expmed.c:3048 emits `(hi >> post_shift) - (x >> 31)' with the
   SHIFT BEFORE the subtraction. The listing has `mfhi' / `subu' / `sra 12',
   i.e. the shift AFTER, so post_shift is 0 and the `sra 12' belongs to the
   program: `(-(f(a) << 12) / 6) >> 12'. Spelled as one division by
   0x6000 = 6 * 4096 it is 121 words / 28 aligned differences, because that
   divisor's own post_shift then lands inside the magic sequence. Reading the
   pass source settled this in one grep where a sweep would not have found it.

   (2) THE FINAL SUM NEEDS ITS OWN LOCAL. With one `s16 cx' carrying all
   three stages -- the trig, the `r' scaling and the `x +' -- it is 121 / 2,
   the final `addu's destination and operand order (`addu $s3, $s3, $v1' for
   the target's `addu $s3, $v1, $s3'). Three `reg_n_sets' again, as in
   func_s55_80024ED4 above: splitting the last stage off into `cx' leaves
   `c0' with two sets and `cx' with one, and the order appears. With the
   split in place the operand order is then MEASURABLE and load-bearing:
   `cx = x + c0' lands and `cx = c0 + x' is 121 / 2. With the merged local
   both orders read the same 2, so one-at-a-time pricing of the operand order
   from the unsplit body says "free" and is wrong -- the brief's
   "an improvement is not evidence" with the sign reversed.

   (3) `c0' AND `c1' MUST BE `s16'. As `s32' the body is 117 words / 23: four
   sll/sra promotion pairs vanish and the two `mult' operands change.

   (4) THE KIND TERNARY IS `(r != 0) ? 0x212 : 0x12'; `(r == 0) ? 0x12 : 0x212'
   is 121 / 3, the same figure and the same mechanism as
   func_s55_80024ED4's.

   (5) THE SCALING IS A SHIFT, NOT A DIVIDE: `c0 + (c0 * r >> 2)'. A bare
   `sra $v0, $v1, 2' with no `srl 31' bias correction in front of it cannot
   be a signed division by 4.

   MEASURED AND FREE at 121 / 0, so it is not why this lands: the +0x2 store
   written before the `cx' assignment rather than after it. */
UnkNode *func_s55_80025344(s16 x, s16 y, s16 z, s16 r)
{
    UnkNode *n;
    UnkVec *m;
    u8 *t;
    u8 *q;
    s32 a;
    s16 c0;
    s16 c1;
    s16 cx;
    s16 cy;
    s32 i;

    for (i = 0; i < 0x10; i++)
    {
        n = func_8003FC64((r != 0) ? 0x212 : 0x12);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s55_800250B4;
            func_8004491C(n, (u32)func_80045C34);
            a = i * 0x100;
            m = n->unk_08;
            c0 = func_80064584(a) >> 7;
            c0 = c0 + (c0 * r >> 2);
            c1 = func_800644B8(a) >> 7;
            c1 = c1 + (c1 * r >> 2);
            q = (u8 *)n + 0x20;
            cx = x + c0;
            *(s16 *)((u8 *)m + 0x2) = cx;
            *(s16 *)(q + 0x1C) = cx;
            cy = y + c1;
            *(s16 *)((u8 *)m + 0x6) = cy;
            *(s16 *)(q + 0x1E) = cy;
            *(s16 *)((u8 *)m + 0xA) = z;
            *(s16 *)(q + 0x20) = z;
            t = (u8 *)n->unk_0C;
            *(u32 *)(t + 0x8) = (u32)D_80028220;
            *(s16 *)(t + 0x1C) = 0x1000;
            *(s16 *)(t + 0x1A) = a + 0x400;
            *(s16 *)(t + 0x16) = (-(func_800644B8(a) << 12) / 6) >> 12;
            *(s16 *)(t + 0x18) = ((func_80064584(a) << 12) / 6) >> 12;
            *(s16 *)(q + 0x14) = cx;
            *(s16 *)(q + 0x16) = cy;
            *(s16 *)(q + 0x18) = z;
            *(s16 *)(q + 0x38) = i;
            *(s16 *)(q + 0x30) = r + 4;
            *(s16 *)(q + 0x3C) = r;
        }
    }
    return n;
}

/* The 0x212 ring shard's +0x10 step, installed by func_s55_80025604 below:
   glide the mover one frame, bleed the two horizontal velocity components by
   3/16 a frame (`>> 3' plus `>> 4'), bump this bank's D_800281F8 counter, and
   retire the node when the +0x30 countdown runs out OR when the two bled
   components have decayed to within 4 of cancelling. 55 words, 0 differing.

   NO TWIN -- dngtwin reports no carved sibling for any stub in this unit. The
   glide block is this unit's own landed func_s55_80025C80's case 0x10 almost
   verbatim (`b->unk_00 += b->unk_0C' and the two following), with that body's
   `>> 2' decay replaced by the `>> 3' plus `>> 4' pair, and the retire pair is
   func_s55_80025AC4's. The in-unit route; nothing else was needed.

   THE ONE FACT, AND IT IS THE WHOLE BODY: the +0x30 countdown must be the
   ARRAY REF `((s16 *)p)[0x18] -= 1'. As the plain cast
   `*(s16 *)(p + 0x30) -= 1' it is 56 WORDS -- one long -- and 21 aligned
   differences. The array ref gives the MEM MEM_IN_STRUCT_P, so alias.c's
   fixed-scalar-versus-varying-struct rule drops the edge between the
   `sh %lo(D_800281F8)' store and this varying store, and reorg sinks the
   counter's store into the `blez's delay slot -- the extra word is the `nop'
   that slot is left as otherwise. Same mechanism and same polarity as
   func_s55_80025C80's `((s16 *)p)[0x1C] = 8' twelve listings up, and the
   opposite polarity to func_s55_80025764's argument loads.

   THE BUMP'S POSITION IS FREE HERE, which is worth saying because it is
   priced at 15 and 18 aligned differences on two other bodies in this unit
   (func_s55_80025AC4 and func_s55_80025764). With the array ref in place
   `D_800281F8 += 1' measures 55 / 0 at the top, in the middle and last. Once
   the store can reach the delay slot its RTL position no longer decides
   anything -- so that fact is about those bodies' dependences, not about this
   family, and a body at 0 is the only place it could have been seen.

   MEASURED AND FREE at 55 / 0, so neither is why this lands: the three
   position updates written as plain casts instead of struct fields, and the
   retire test written as two separate arms instead of one `||'. (The second
   is FREE and not NOT-MEASURABLE: the two arms compile to the same 55 words.)

   LOAD-BEARING, all re-priced from this body at 0: the two decays must be
   STRUCT FIELDS (as plain casts 55 / 2 -- the opposite of the position
   updates beside them, two spellings in one statement group wanting opposite
   answers); the decay of +0xC must come before the decay of +0x10 (reversed
   54 words / 5); the +0x8 update must sit BEFORE both decays, not between
   them (54 / 38); the three position updates must be +0x0, +0x4, +0x8 in that
   order (+0x4 first is 54 / 14); each decay must be ONE subtraction of a sum,
   not two subtractions (`x = x - (x >> 3) - (x >> 4)' is 55 / 23); and the
   cancellation test must be `__builtin_abs' and not a pair of comparisons
   (53 words / 5). */
void func_s55_80025528(u8 *p, UnkVec *b)
{
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    b->unk_0C -= (b->unk_0C >> 3) + (b->unk_0C >> 4);
    b->unk_10 -= (b->unk_10 >> 3) + (b->unk_10 >> 4);
    D_800281F8 += 1;
    if ((((s16 *)p)[0x18] -= 1) <= 0 ||
        __builtin_abs(*(s16 *)((u8 *)b + 0xE) + *(s16 *)((u8 *)b + 0x12)) < 4)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn a ring of 0x20 shards around (x, y, z): each is a 0x212 node with
   func_s55_80025528 as its +0x10 step and func_800C95C0 through
   func_8004491C, its mover put at the caller's point with a velocity of
   cos/sin of 0x80 per shard scaled by 1 << 11 and a fixed 0xFFFE0000 on the
   third axis, and a payload armed with 0x20/0x20/1/0 at +0x0/+0x2/+0x4/+0x6
   and 0x20 at +0x30. It returns the LAST node it allocated, which the one
   caller -- this unit's landed func_s55_800250B4 -- discards.
   88 words, 0 differing.

   NO TWIN. Written off this unit's own landed func_s55_8002589C, which
   spawns 0x212 nodes in a `for (i = ...; i > 0; i--)' loop the same way and
   supplies the q = (u8 *)n + 0x20 payload idiom and the +0x0/+0x2/+0x4/+0x6
   arming verbatim. func_80064584 and func_800644B8 are the resident
   cos/sin pair (csrc/unk_113D0.c's func_8003E02C uses them that way); the
   `& 0x800' / `| 0xFFFFF800' / `& 0x7FF' triple is a hand-written
   sign-extension of their 12-bit result, and the source really does call the
   routine THREE times per axis -- once for the test and once in each arm.

   SIGNATURE CHANGE, AND IT IS REPORTED SEPARATELY: this unit's declaration
   block had `void func_s55_80025604(s16, s16, s16)'. The body ends
   `addu $v0, $s2, $zero' before the epilogue, which a void function does not
   emit, so the return type is `UnkNode *'. func_s55_800250B4 calls it as a
   statement and re-verifies at 164 words / 0 differing with the new
   declaration.

   THE ONE FACT, and it is a REFERENCE COUNT, not an allocation priority.
   Each axis must be ONE store of a CONDITIONAL EXPRESSION,
   `*(s32 *)((u8 *)m + 0xC) = (cond ? (f(a) | 0xFFFFF800) : (f(a) & 0x7FF))
   << 11'. Written as an `if'/`else' with the store inside BOTH arms it is
   88 words / 17 aligned differences -- a clean whole-body $s0/$s1 exchange
   between `m' and the loop's cos/sin argument, nothing moved and no word
   gained. The EMITTED CODE IS THE SAME either way, because jump2 cross-joins
   the two arms' shared `sll'/`sw' pair; what differs is what local-alloc saw
   BEFORE that join.

   `-da' SETTLED IT IN ONE COMPILE and the arithmetic is exact. In the losing
   form `.lreg' gives `m' 18 references across 36 insns and the strength-
   reduced `i * 0x80' giv 17 across 57, so `allocno_compare' ranks them
   4*18/36 = 2.000 against 4*17/57 = 1.193 and `.greg's allocation order line
   reads `87 142 86 ...' -- `m' first, taking $s0. The duplicated store is
   TWO insns of `m', and flow.c:2122 counts each once per enclosing loop, so
   it is FOUR of those 18 references. Removing it takes `m' to 14, which drops
   it a power of two to 3*14/36 = 1.167, just under the giv's 1.193, and the
   order reverses. This is the brief's cliff arithmetic running the other way:
   the usual remedy is to ADD references to win a register, and here the fix
   is to stop paying for references that only exist before a cross-join.

   AND THE DECLARATION ORDER IS NOT THE LEVER: all 24 permutations of the four
   locals measure the IDENTICAL 88 words / 17 aligned differences. That is
   what ruled out allocation priority before `-da' was run, and it is the
   check the brief asks for first on a two-register exchange.

   MEASURED AND REJECTED, 88 / 17 -- the same figure as the duplicated store,
   which is the tell that it does not touch the cause: an `s32 t' local
   assigned in both arms with ONE store after them, and the same with a
   separate `t2' per axis. One store is not enough; the value must never be a
   named local, because a local set in two arms carries two `reg_n_sets' and
   loses `adjust_priority's birthing boost. */
UnkNode *func_s55_80025604(s16 x, s16 y, s16 z)
{
    UnkNode *n;
    UnkVec *m;
    u8 *q;
    s32 i;

    for (i = 0; i < 0x20; i++)
    {
        n = func_8003FC64(0x212);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s55_80025528;
            func_8004491C(n, (u32)func_800C95C0);
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = x;
            *(s16 *)((u8 *)m + 0x6) = y;
            *(s16 *)((u8 *)m + 0xA) = z;
            *(s32 *)((u8 *)m + 0xC) =
                (((func_80064584(i * 0x80) & 0x800) != 0) ? (func_80064584(i * 0x80) | 0xFFFFF800)
                                                          : (func_80064584(i * 0x80) & 0x7FF))
                << 11;
            *(s32 *)((u8 *)m + 0x10) =
                (((func_800644B8(i * 0x80) & 0x800) != 0) ? (func_800644B8(i * 0x80) | 0xFFFFF800)
                                                          : (func_800644B8(i * 0x80) & 0x7FF))
                << 11;
            *(s32 *)((u8 *)m + 0x14) = 0xFFFE0000;
            q = (u8 *)n + 0x20;
            *(s16 *)(q + 0x0) = 0x20;
            *(s16 *)(q + 0x2) = 0x20;
            *(s16 *)(q + 0x4) = 1;
            *(s16 *)(q + 0x6) = 0;
            *(s16 *)(q + 0x30) = 0x20;
        }
    }
    return n;
}

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

/* Spawn the 0x12 cluster: eighteen nodes, all stepped by this unit's landed
   func_s55_80025C80, each at the caller's point with a 0x400 rotation at the
   slot's +0x16 and the caller's heading plus 0x400 at +0x1A, 0x1000 scales,
   0x20 at +0x10 and 0xC raised in the slot's flag halfword, and the slot's
   sheet taken from this bank's D_80028268 sixteen bytes per node. Node 0 is
   allocated against the resident D_80083498 and the rest against node 0,
   which they also stash at their payload's +0x20. It refuses outright if
   func_8003FA44 says the 0x12 pool is unavailable, UNWINDS every node it has
   already made if one allocation fails, and otherwise finishes by running
   this unit's func_s55_80025B5C on node 0 and returning it.
   113 words, 0 differing.

   NO TWIN. Written off this unit's own landed func_s55_8002589C and
   func_s55_80025B5C; func_8003FA44's signature is csrc/unk_12DEC.c's and the
   `extern u8 D_80083498[]' spelling with `func_8003FD64(kind, D_80083498)' is
   csrc/ovl_dng_b11.c's and csrc/ovl_dng_b01.c's.

   The return type stays `s32' as this unit's declaration block already had
   it -- the one caller, the landed func_s55_800250B4, tests it against 0 --
   so node 0 goes back through a `(s32)' cast rather than by widening the
   prototype. The value IS measurable: returning `arr[1]' instead is
   113 / 1.

   THE ONE FACT, AND IT IS A HOIST CONTEST THAT `-da' SETTLED IN ONE COMPILE.
   D_80028268 must be written INLINE in the loop, with no pointer local.
   Given a local -- `u8 *g = D_80028268;' before the loop, which is the
   spelling func_s55_80024ED4 above REQUIRES and which this unit uses
   everywhere -- the body is 111 WORDS, two short, and 9 aligned differences:
   the table ends up rematerialised inside the loop and $fp holds
   func_s55_80025C80's address instead, which is the exact mirror of the
   target.

   `.lreg' prices it to one part in forty. Both candidates have THREE
   loop-weighted references, so `allocno_compare' can only separate them on
   live length: my local (pseudo 89) spans 114 insns because it is born in
   the function's first block, and loop.c's own hoist of the step symbol
   (pseudo 113) spans 108 because loop.c emits it into the loop PREHEADER,
   after `i = 0'. 1*3/114 = 0.0263 against 1*3/108 = 0.0278, and `.greg's
   allocation order line reads `... 113 89' -- the step first. Write the
   symbol inline and BOTH are loop.c movables born in the preheader, and it
   is the table that keeps $fp. Confirming the mechanism rather than the
   spelling: `g' assigned immediately before its USE, inside the loop, is
   also 113 / 0.

   MEASURED AND FREE at 111 / 9 -- the same figure as the losing spelling,
   which is the tell that it changes nothing: `(u32)&func_s55_80025C80' in
   place of `(u32)func_s55_80025C80'.

   The unwind must be the `else' arm of the allocation gate, not an early
   return: as `if (arr[i] == NULL) { unwind; return 0; }' gcc puts the unwind
   inline after the gate and the body is 110 words / 93 differences, with the
   whole tail relocated. Same lever as func_s55_80024ED4's fact (3) and far
   more expensive here because the unwind is a loop. (Priced mid-fight,
   before the hoist was fixed; the direction is the transferable part.) */
s32 func_s55_80026384(s16 x, s16 y, s16 z, s16 h)
{
    UnkNode *arr[0x12];
    UnkVec *m;
    u8 *r;
    u8 *q;
    s32 i;

    if (func_8003FA44(0x12) == 0)
    {
        return 0;
    }
    for (i = 0; i < 0x12; i++)
    {
        arr[i] = func_8003FD64(0x12, (i != 0) ? (void *)arr[0] : (void *)D_80083498);
        if (arr[i] != NULL)
        {
            arr[i]->unk_10 = (u32)func_s55_80025C80;
            m = arr[i]->unk_08;
            *(s16 *)((u8 *)m + 0x2) = x;
            *(s16 *)((u8 *)m + 0x6) = y;
            *(s16 *)((u8 *)m + 0xA) = z;
            r = (u8 *)arr[i]->unk_0C;
            *(s16 *)(r + 0x16) = 0x400;
            *(s16 *)(r + 0x1A) = h + 0x400;
            *(s16 *)(r + 0x20) = 0x1000;
            *(s16 *)(r + 0x1E) = 0x1000;
            *(s16 *)(r + 0x1C) = 0x1000;
            *(u32 *)(r + 0x8) = (u32)(D_80028268 + i * 0x10);
            *(s16 *)(r + 0x10) = 0x20;
            *(u16 *)(r + 0x14) |= 0xC;
            q = (u8 *)arr[i] + 0x20;
            if (i != 0)
            {
                *(UnkNode **)(q + 0x20) = arr[0];
            }
            *(s16 *)(q + 0x30) = 0x10;
            *(s16 *)(q + 0x34) = h;
            *(s16 *)(q + 0x36) = 7;
        }
        else
        {
            for (i = i - 1; i >= 0; i--)
            {
                D_800814A0 |= 0x8000;
                arr[i]->unk_1E |= 0x8000;
            }
            return 0;
        }
    }
    func_s55_80025B5C(arr[0], h);
    return (s32)arr[0];
}

/* Build the slot's 0x12-element transform in place: RotMatrix from the
   element's own SVECTOR, ScaleMatrix by its +0x10/+0x12/+0x14 triple, and the
   +0x8/+0xA/+0xC triple widened into the matrix's translation vector. The
   payload `p' IS the MATRIX -- t[0..2] are its +0x14/+0x18/+0x1C -- which is
   what makes the three `sw' of sign-extended halfwords the last thing it
   does. 34 words, 0 differing, FIRST COMPILE.

   func_80065820 is `RotMatrix' and func_80064BC0 is `ScaleMatrix'
   (splat/common_syms.txt), so this body needs no new declaration: the
   argument types come from psyq/include/libgte.h and the parameter list was
   already in this unit's declaration block, put there by the landed
   func_s55_80025C80 that calls it.

   Its caller passes `s->unk_00', and `UnkElem' as include/structs.h has it is
   only 8 bytes, so every access past +0x4 goes through a `(u8 *)e' cast
   rather than a field. The usage witnessed here is an SVECTOR at +0x0, a
   translation at +0x8/+0xA/+0xC and a scale at +0x10/+0x12/+0x14; I have NOT
   widened the typedef, because the one landed caller's own declaration is
   what fixes the parameter type and the struct is shared with other units.

   FIRST COMPILE, so nothing in it is measured and there is no fact to claim.
   The one thing worth recording is the frame: locals are sp+0x10..0x1F, which
   is a 16-byte VECTOR, so the scale triple goes through a `VECTOR' and not
   through three loose words. */
void func_s55_80026548(u8 *p, UnkElem *e)
{
    VECTOR v;

    RotMatrix((SVECTOR *)e, (MATRIX *)p);
    v.vx = *(s16 *)((u8 *)e + 0x10);
    v.vy = *(s16 *)((u8 *)e + 0x12);
    v.vz = *(s16 *)((u8 *)e + 0x14);
    ScaleMatrix((MATRIX *)p, &v);
    *(s32 *)(p + 0x14) = *(s16 *)((u8 *)e + 0x8);
    *(s32 *)(p + 0x18) = *(s16 *)((u8 *)e + 0xA);
    *(s32 *)(p + 0x1C) = *(s16 *)((u8 *)e + 0xC);
}

/* func_s55_800266D0's body with a frame reset in front of it: park the signed
   frame index at the slot's +0x4, clear the +0x5 sub-counter, point the slot's
   +0x00 element at its sheet's +0xC plus 24 bytes per frame of that index, and
   drop the slot's 0x4000 and 0x2000 flags. 15 words, 0 differing.

   No twin: dngtwin reports no carved sibling anywhere in this unit, and this
   is the unit's one tiny stub. Written off this unit's own landed
   func_s55_800266D0 four listings down, which is the +0x00 store verbatim
   with the same 24-byte stride, plus func_s55_8002660C's
   `*(u16 *)(s + 0x14)' flag idiom. The in-unit route again: no offset in this
   body needed guessing.

   TWO FACTS, both measured here from a body at 0. The shape was right on the
   first compile and all eleven of its rows were these two orders.

   (1) THE +0x14 MASK MUST COME LAST, after the +0x00 store. Written in
   address order (+0x4, +0x5, +0x14, +0x00) it is 15 words / 11 aligned
   differences. A sweep of all eleven orderings that start with the +0x4 or
   the +0x00 store gives 0 / 7 / 9 / 10 / 11 / 12 / 12 plus four rows that are
   16 WORDS -- one word long -- all four of which put the +0x00 store first.
   So the position is measurable, the curve has a single minimum and nothing
   else reaches it. What the order decides is which quantity the sheet pointer
   competes with: with the mask last, the +0x14 halfword and the `* 24'
   product share $v0 and the pointer keeps $v1, which is the target's
   assignment.

   (2) THE SUM MUST BE `<deref> + d * 24', the DEREF FIRST. The product first
   is 15 / 2 at this statement order, and the two rows are the final `addu's
   destination and operand order -- `addu $v1, $v1, $v0' where the target has
   it and `addu $v0, $v0, $v1' where it does not -- with every other word in
   place. Note the interaction, because it is a trap: at the WRONG statement
   order the two sum orders read 11 (deref first) and 9 (product first), so
   the losing form looks better there. An improvement is not evidence; priced
   from the body at 0 the deref-first form is the only one that lands. */
void func_s55_800265D0(u8 *s, s16 d)
{
    *(s8 *)(s + 0x4) = d;
    *(s8 *)(s + 0x5) = 0;
    *(s32 *)s = *(s32 *)(*(u8 **)(s + 0x8) + 0xC) + d * 24;
    *(u16 *)(s + 0x14) &= 0x9FFF;
}

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
