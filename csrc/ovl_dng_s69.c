#include "common.h"

s32 rand(void);

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
void func_s69_800245A0(u8 *p, UnkVec *b, u8 *c, u8 *d);

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
void func_s69_80024328(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u8 D_8002746C[];

u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
u8 *func_s69_800244C4(u8 *m, u8 *act);
void func_s69_80025D28(u8 *p, UnkVec *b, UnkSlot *s);
void func_s69_80025840(u8 *s, s16 *b, s16 lo, s16 hi);
void func_s69_8002590C(u8 *s, s16 d);
void func_s69_800257D0(u8 *d, u8 *s);
void func_s69_8002593C(u8 *p, UnkVec *b, u8 *r);
void func_s69_80025B78(u8 *, UnkVec *, u8 *);
UnkNode *func_s69_80025FCC(s16 x, s16 y, s16 z, s16 a);
s16 func_800A07D0(s16, s16, s16, s16);
s16 func_800A45D8(u16, u16, s32);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
void func_800B653C(u8 *, s16);
extern u32 D_800814A8;
extern u8 D_800287A2;
extern u32 D_800287A4;
extern s8 D_8002758C[];
extern Unk80083462 D_80083460;
extern s16 D_800287A0;
extern s32 D_8002744C;
extern u8 D_80028260[];
extern u8 D_80028780[];
void func_80040490(void *, RECT *);
void func_8003F80C(u32, s32, s32, s16);
s32 func_800A56E0(s32);
typedef struct UnkDngS69Rec18
{
    s16 h[9];
} UnkDngS69Rec18;
typedef struct UnkDngS69SP
{
    u8 pad_00[0x40];
    VECTOR unk_40;
    MATRIX unk_50;
    u8 pad_70[0x60];
    MATRIX unk_D0;
} UnkDngS69SP;

/* The bank's six-state cutscene driver, run off *(s16 *)(p + 0xA). State 0
   opens: a func_80040490 RECT over D_80028260, func_8003F80C on D_80028780,
   D_800287A0 raised, D_8002744C cleared, the actor's +0x9 published into
   D_800287A2 and its +0x0 into D_800287A4, the player's +0xF4 cleared, +0xA8
   taken from the actor's +0x8 and +0x102 raised -- then FALLS THROUGH into
   state 1, which waits for the view's own +0x0 to carry 0x80, spawns the 0x12
   tint node through this unit's func_s69_800244C4, arms the -1/0x20 fade pair
   at +0x20/+0x22 and the 0x10 countdown at +0x1C, drops the player's +0xA6
   and plays cue 0x300. State 2 waits out +0x1C, state 3 clears D_800287A0 and
   arms a 0x20 countdown at +0x1A, state 4 eases D_80083160's +0xA8/+0xAA
   colour bytes towards 0x80 over that countdown and snaps them to 0x80 at the
   end, state 5 retires the actor and drops D_80083460's +0xA counter once
   this bank's D_80027452 reads zero. Every state ends in the shared tail,
   which -- while +0x20 is negative -- eases the same two colour bytes towards
   0x20 over the +0x22 countdown, and always clears D_80027452.
   194 words, 0 differing.

   THIS FUNCTION OWNS jtbl_s69_80024008. jtcheck.py: 6 words = 6 cases + 0
   pad, dispatch bound 6, 6 distinct targets. Entries, in table order:

       case 0 -> .Ls69_80024068     case 3 -> .Ls69_800241B0
       case 1 -> .Ls69_80024108     case 4 -> .Ls69_800241D4
       case 2 -> .Ls69_8002418C     case 5 -> .Ls69_80024250

   The six entry addresses are strictly ascending, so the arms are written
   ascending 0..5, which is their source order. No two cases share an arm and
   there is no default: the `sltiu $v0, $v1, 0x6' gate sends everything else
   to the shared tail. Cases 2 and 4 both reach the `+0xA' bump through their
   own copy, written out twice here because jump2 cross-joins them.

   ARM ORDER SETTLED TWO WAYS, because neither ovlverify nor aligndiff can
   see it. (a) PER ARM: every one of the six arms carries a distinguishing
   non-branch instruction, so none of them is in the invisible
   masked-jump-plus-common-code class -- the shortest, case 2, is still
   `lhu 0x1C' / `addiu -1' / `sh 0x1C' / `sll 16' and no other arm touches
   +0x1C. Note that this had to be established arm by arm: the table having
   six DISTINCT entries says only that no two entries share a label, which is
   not the same question. (b) INDEPENDENTLY, a standalone `tools/cdkgcc -S'
   of this file emits the table as `.word $L3, $L4, $L7, $L9, $L10, $L12' with
   those six labels appearing in the text in that same order and no trailing
   pad word, which is the ascending order the ROM table has.

   NO TWIN. dngtwin's best carved sibling is func_s62_80024064 at 0.892 and
   func_s55_80024078 at 0.879, both still INCLUDE_ASM. Written from the
   listing; csrc/ovl_dng_b08.c's landed func_b08_8017451C supplied the
   D_80083160 colour-ease idiom (`g[0xA8] += (0x80 - g[0xA8]) / <countdown>').
   Both of ITS stated facts re-measured here:
     HOLDS, and more strongly than it states it: `g = (u8 *)&D_80083160' must
     be taken ONCE BEFORE the switch, not per arm. Split into one local for
     state 4 and one for the shared tail it is 195 WORDS / 30 aligned
     differences. Note this is the OPPOSITE of what D_800814A8 wants in this
     same body (fact 4 below) -- one object materialised up front, the other
     one local per arm, 40 words apart.
     EQUIVALENT, not free and not a fact: its countdown spelling
     `(s16)(--*(u16 *)(p + 0x96)) <= 0'. All three countdowns here written
     that way instead of `(*(s16 *)(p + 0x1A) -= 1) <= 0' are also
     194 words / 0 differing -- the two spellings are the same tree.

   FOUR FACTS, all measured on this body, and three of them are one mechanism
   -- sched1 hoisting a load above a store it cannot be proved to alias:

   (1) THE FOUR `+0xA' STATE BUMPS MUST BE u16 ARRAY REFS, `((u16 *)p)[5] =
   ((u16 *)p)[5] + 1'. As the plain cast `*(s16 *)(p + 0xA) = *(u16 *)(p + 0xA)
   + 1' it is 194 words / 2 aligned differences: state 3's `lhu 0xA' then sits
   BELOW the `sh %lo(D_800287A0)' instead of above it. The array ref is what
   sets MEM_IN_STRUCT_P (the flag needs a PLUS_EXPR on top), and
   true_dependence's fixed-scalar-versus-varying-struct exception then frees
   the load to move above the fixed-address store.

   (2) STATE 0's `D_800287A2 = p[0x9]' MUST COME BEFORE `D_8002744C = 0'.
   The other way round it is 194 words / 6 aligned differences. The two stores
   swap back (differing symbols never alias, so sched reorders them freely)
   but the `lbu 0x9' cannot climb past the store that precedes it in source
   order, and the D_800814A8 load behind it is dragged down with it.

   (3) STATE 0's `D_800287A4 = *(u32 *)(p + 0x0)' MUST COME BEFORE THE +0xF4
   CLEAR *AND* THAT CLEAR MUST BE THE ARRAY REF `((s32 *)n)[0x3D] = 0'. THIS
   IS A CONJUNCTION -- each half alone is 194 words / 2 aligned differences
   (the `sw 0xF4' one slot early) and only both together reach 0, which is why
   a one-at-a-time sweep misses it. Same mechanism as (1): the order puts the
   load first and the array ref lets the two stores swap back.

   (4) D_800814A8 IS A SEPARATE POINTER LOCAL PER ARM. State 1's must be
   assigned BEFORE the +0x1C store, so its `lw' can fill its own load delay
   with that `sh': written inline at the use it is 195 WORDS / 8 aligned
   differences, a real extra word for the `nop'. And state 0's and state 1's
   must be DIFFERENT locals -- sharing one is 194 words / 13 aligned
   differences, because one pseudo spanning both arms takes a register neither
   arm's scratch register is. csrc/ovl_dng_b08.c records the same thing for
   &D_80083460 ("FIVE separate pointer locals, one per arm"). State 0's third
   use, the +0x102 store, is NOT through that local: it is inline, and that is
   what makes the target load D_800814A8 twice in state 0. */
void func_s69_80024C38(UnkNode *a, s16 b, s32 c);
s32 func_800C95C0(u8 *, UnkVec *, UnkSlot *);
extern u8 D_80027484[];
void func_s69_80024B00(u8 *p, UnkVec *v);

extern u8 *D_800E3D7C;
extern u8 *D_800E3D18;
extern u8 D_80082E80[];
extern u8 D_80083780[];
extern s16 D_80083228;
void func_800B8D64(s16, s16, s16);
s32 func_800644B8(s32);
s32 func_80064584(s32);
UnkNode *func_s69_800255B8(s16 x, s16 y, s16 z, s16 ang);
s32 func_8003FA44(s32);
s32 func_800C9034(u8 *, UnkVec *, UnkSlot *);
extern u8 D_800274C0[];
extern u8 D_80027580[];
extern u8 D_80083498[];

void func_s69_80024020(u8 *p)
{
    u8 *g;
    u8 *h;
    u8 *n;
    u8 *n2;
    s32 arr[2];

    g = (u8 *)&D_80083160;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        arr[0] = 0x10003A0;
        arr[1] = 0x400020;
        func_80040490(D_80028260, (RECT *)arr);
        func_8003F80C((u32)D_80028780, 0x7AC0, 1, 2);
        D_800287A0 = 1;
        D_800287A2 = p[0x9];
        n = (u8 *)D_800814A8;
        D_8002744C = 0;
        D_800287A4 = *(u32 *)(p + 0x0);
        ((s32 *)n)[0x3D] = 0;
        n[0xA8] = p[0x8];
        *(u8 *)((u8 *)D_800814A8 + 0x102) = 1;
        ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
    case 1:
        if ((**(u16 **)(p + 0x4) & 0x80) != 0)
        {
            if ((*(u8 **)(p + 0xC) = func_s69_800244C4(
                     *(u8 **)(*(u8 **)(p + 0x0) - 0x18), *(u8 **)(p + 0x0))) != NULL)
            {
                *(s16 *)(p + 0x20) = -1;
                *(s16 *)(p + 0x22) = 0x20;
                n2 = (u8 *)D_800814A8;
                *(s16 *)(p + 0x1C) = 0x10;
                *(s16 *)(n2 + 0xA6) -= 1;
                ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
                func_800A56E0(0x300);
            }
        }
        break;
    case 2:
        if ((*(s16 *)(p + 0x1C) -= 1) < 0)
        {
            ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
        }
        break;
    case 3:
        D_800287A0 = 0;
        *(s16 *)(p + 0x1A) = 0x20;
        *(s16 *)(p + 0x20) = 0;
        ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
        break;
    case 4:
        g[0xA8] += (0x80 - g[0xA8]) / *(s16 *)(p + 0x1A);
        g[0xAA] += (0x80 - g[0xAA]) / *(s16 *)(p + 0x1A);
        if ((*(s16 *)(p + 0x1A) -= 1) <= 0)
        {
            g[0xAA] = 0x80;
            g[0xA8] = 0x80;
            ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
        }
        break;
    case 5:
        if (D_80027452 == 0)
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
        g[0xA8] += (0x20 - g[0xA8]) / *(s16 *)(p + 0x22);
        g[0xAA] += (0x20 - g[0xAA]) / *(s16 *)(p + 0x22);
        if ((*(s16 *)(p + 0x22) -= 1) <= 0)
        {
            *(s16 *)(p + 0x20) = 0;
        }
    }
    D_80027452 = 0;
}

/* The 0x12 tint crawler's +0x10 step, three states on *(s16 *)(p + 0x2C)
   under this bank's D_80027452 frame counter. State 0 projects the owner
   node's slot into the payload's +0x24 offset triple with func_8003DE58
   (parking -0x40 at +0x28 if the slot has already raised 0x8000), adds the
   triple onto the owner's mover, spawns this unit's nine-node burst
   func_s69_800249DC and advances; state 1 fades the renderable's colour word
   up by 0x202020 a frame until its first byte reaches 0xC0 and then advances;
   state 2 fades it down by 0x101010 and retires the actor at zero.
   103 words, 0 differing, FIRST COMPILE.

   csrc/ovl_dng_s55.c's landed func_s55_800243C4 is the twin (dngtwin 0.962,
   109 words against 103). Deltas: this bank's D_80027452 for its D_800281F8,
   func_s69_800249DC for func_s55_80024ED4, state 2 subtracting 0x101010 for
   its 0x40404 -- and THE SIX MISSING WORDS, which are a real shape change,
   not references: this copy sets `*(s16 *)(p + 0x2E) = 1' UNCONDITIONALLY
   after the projection attempt and has NO `if (*(s16 *)(p + 0x2E) == 0)
   return;' re-test, so the just-projected path falls into the shared tail.
   Written the twin's way -- the store duplicated into both arms plus the
   re-test -- it compiles to the twin's own 109 words / 14 aligned
   differences.

   Of the twin's two stated facts:
     HOLDS, and reproduces its price to the digit: the owner node load
     `o = *(UnkNode **)(p + 0x8)' and `r = o->unk_0C' must sit ABOVE the
     +0x2E test, not inside it. Moved inside the gate it is 105 words /
     12 aligned differences -- the skip path needs $v0 still live.
     FREE here too: "the state is read twice at the top, once signed and once
     unsigned". This body drops the `u16 n' local entirely and writes state
     1's bump as `*(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1'; with the
     local it is also 103 words / 0 differing, so the second read is not why
     this lands. */
void func_s69_80024328(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *o;
    UnkSlot *r;
    UnkVec *m;
    s16 st;

    D_80027452 += 1;
    st = *(s16 *)(p + 0x2C);
    if (st == 0)
    {
        o = *(UnkNode **)(p + 0x8);
        r = (UnkSlot *)o->unk_0C;
        if (*(s16 *)(p + 0x2E) == 0)
        {
            if (func_8003DE58((u8 *)r->unk_08, r, (s16 *)(p + 0x24), 0) == NULL)
            {
                if ((r->unk_14 & 0x8000) != 0)
                {
                    *(s16 *)(p + 0x28) = -0x40;
                }
            }
            *(s16 *)(p + 0x2E) = 1;
        }
        m = (*(UnkNode **)(p + 0x8))->unk_08;
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)m + 0x2) + *(u16 *)(p + 0x24);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)m + 0x6) + *(u16 *)(p + 0x26);
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)m + 0xA) + *(u16 *)(p + 0x28);
        if (func_s69_800249DC(b) != NULL)
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
            *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
        }
    }
    else if (st == 2)
    {
        *(s32 *)(c + 0xC) -= 0x101010;
        if (*(u8 *)(c + 0xC) == 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

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

/* The bank's four-state cutscene mover, switched on *(s16 *)(p + 0x2C) as a
   compare chain over {0,1,2,3}. State 0 seeds the aim point from the party
   record D_80083780 and the view's +0x88, offsets it by func_8003DE58's
   projection of D_800E3D18's current record into D_80082E80's slot, publishes
   it at p+0x14..0x18 and FALLS THROUGH into state 1. State 1 eases the mover
   an eighth of the way to the aim point, scaled by p+0x38 + 1 and divided by
   the p+0x30 countdown, settles p+0x1C..0x20 a quarter of the way toward the
   mover, republishes them into the vector at b+0x2/0x6/0xA, and at the end of
   the countdown fires func_800B8D64, reloads the countdown with 5 and
   advances. State 2 bleeds the slot's +0xD by a p+0x30th a frame and on the
   last frame either tears the actor down or arms state 3's four-frame timer.
   State 3 spawns func_s69_8002614C's node at the aim point every frame that
   func_8003DE58 still projects, then fires func_s69_800255B8 once and tears
   down. 271 words, 0 differing. One-member family; no .rodata block, so the
   four-way switch is a compare chain (beq / slti / beqz / beq / beq) and this
   function owns no jump table.

   NO TWIN AT ANY RATIO -- dngtwin reports nothing at 0.60 for any stub in
   this unit. THE CALLEE-SET INTERSECTION FOUND THE DONOR ON THE FIRST TRY:
   grep -ln over func_800B8D64, D_80083780, func_8003DE58, D_800E3D18 and
   D_800814A0 names csrc/ovl_dungeon_800B6AC4.c in four of the five lists, and
   the donor is its landed func_800B8830 (252 words against this 271) -- same
   switch-with-fallthrough skeleton, same
   `func_8003DE58(((u8 **)D_800E3D18)[i], ...)` with the
   `((D_80083228 + *(s16 *)(view + 0x2A) + 0x100) >> 9) & 7` index, and same
   `*(u16 *)(p - 0x2) |= 0x8000; D_800814A0 |= 0x8000;` teardown. Neither
   dngnear's s55/s56/s62 donors nor dngcallee's rows were used.

   MEASURED, all from the body at 0 differing unless the words column moves:

     the three settle blocks written with NO temp, re-reading p+0x1C
     for the b-side store (as written)                        271 /  0
     one shared `s32 t' across the three settles              271 / 25
     a shared `s16 t'                                         271 /  0
     three separate `s32' locals                              271 /  0
     the settle's outer `+' operands swapped                  271 /  0  FREE
     state 0's `g = D_80083780' pointer local (as written)    271 /  0
     the same two reads inline off the symbol             270 words / 12
     state 3's first `g2 = D_80083780' inline                 271 / 21
     state 3's second `g3 = D_80083780' inline                271 /  4
     state 3's `sl = D_80082E80' inline                   272 words /  5
     the p+0x2C counter as `((s16 *)p)[0x16] = ((u16 *)p)[0x16] + 1'
     at all three sites (as written)                          271 /  0
     the same counter as a plain cast at all three sites  272 words /  9
     as a plain cast at STATE 0's site only                   271 /  0  FREE

   THE SHARED `s32 t' IS reg_n_sets, NOT ALLOCATION PRIORITY: with one local
   set three times the whole settle run comes out with $v0 and $v1 exchanged,
   every instruction in place -- `adjust_priority's `birthing_insn_p' boost
   only reaches a single-set pseudo. `s16 t' and three `s32' locals both land,
   so what the target constrains is the SET COUNT, not the width and not the
   number of locals. And note the trap: the operand-swap row above read 18
   aligned rows while the shared-t fault was still live and measures FREE from
   a clean body, which is exactly the mid-fight inflation the brief warns of.

   THE COUNTER'S ARRAY REF IS A DEPENDENCE, not a style: state 2's site must
   let `lhu 0x2C($s0)' be lifted ABOVE the `sh %lo(D_80027450)' store, and
   only the index form makes that MEM_IN_STRUCT_P so true_dependence drops the
   edge. State 1's site is cross-joined with it by jump2, so the two have to
   be spelled alike; state 0's is its own and is free either way.

   DERIVED FROM THE FRAME, not measured: `out' has FOUR elements so that `pos'
   lands at sp+0x20. The outgoing-argument area is 0x18 bytes (state 3's
   five-argument func_s69_8002614C call), locals start at sp+0x18, and a
   three-element `out' would put `pos' at sp+0x1E. Both arrays are `u16' --
   every read of them in the listing is an `lhu' -- so the two func_8003DE58
   calls take an `(s16 *)' cast rather than the unit's one declared signature
   being changed. */
void func_s69_800245A0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u16 out[4];
    u16 pos[3];
    u8 *g;
    u8 *g2;
    u8 *g3;
    u8 *sl;
    s32 i;

    D_80027452++;
    switch (*(s16 *)(p + 0x2C))
    {
    case 0:
        g = D_80083780;
        pos[0] = *(u16 *)(g + 0x2);
        pos[1] = *(u16 *)(g + 0x6);
        pos[2] = *(u16 *)(D_800E3D7C + 0x88) - 0x50;
        i = ((D_80083228 + *(s16 *)(D_800E3D7C + 0x2A) + 0x100) >> 9) & 7;
        func_8003DE58(((u8 **)D_800E3D18)[i], (UnkSlot *)D_80082E80, (s16 *)out, 0);
        pos[0] += out[0];
        *(s16 *)(p + 0x14) = pos[0];
        pos[1] += out[1];
        *(s16 *)(p + 0x16) = pos[1];
        pos[2] += out[2];
        *(s16 *)(p + 0x18) = pos[2];
        ((s16 *)p)[0x16] = ((u16 *)p)[0x16] + 1;
        /* fallthrough */
    case 1:
        *(s16 *)((u8 *)b + 0xE) =
            *(u16 *)((u8 *)b + 0xE) +
            (*(s16 *)(p + 0x1C) +
             (*(s16 *)(p + 0x14) - *(s16 *)(p + 0x1C)) / 8 * (*(s16 *)(p + 0x38) + 1) -
             *(s16 *)((u8 *)b + 0xE)) /
                *(s16 *)(p + 0x30);
        *(s16 *)((u8 *)b + 0x12) =
            *(u16 *)((u8 *)b + 0x12) +
            (*(s16 *)(p + 0x1E) +
             (*(s16 *)(p + 0x16) - *(s16 *)(p + 0x1E)) / 8 * (*(s16 *)(p + 0x38) + 1) -
             *(s16 *)((u8 *)b + 0x12)) /
                *(s16 *)(p + 0x30);
        *(s16 *)((u8 *)b + 0x16) =
            *(u16 *)((u8 *)b + 0x16) +
            (*(s16 *)(p + 0x20) +
             (*(s16 *)(p + 0x18) - *(s16 *)(p + 0x20)) / 8 * (*(s16 *)(p + 0x38) + 1) -
             *(s16 *)((u8 *)b + 0x16)) /
                *(s16 *)(p + 0x30);
        *(s16 *)(p + 0x1C) = *(u16 *)(p + 0x1C) + ((*(s16 *)((u8 *)b + 0xE) - *(s16 *)(p + 0x1C)) >> 2);
        *(s16 *)((u8 *)b + 0x2) = *(s16 *)(p + 0x1C);
        *(s16 *)(p + 0x1E) = *(u16 *)(p + 0x1E) + ((*(s16 *)((u8 *)b + 0x12) - *(s16 *)(p + 0x1E)) >> 2);
        *(s16 *)((u8 *)b + 0x6) = *(s16 *)(p + 0x1E);
        *(s16 *)(p + 0x20) = *(u16 *)(p + 0x20) + ((*(s16 *)((u8 *)b + 0x16) - *(s16 *)(p + 0x20)) >> 2);
        *(s16 *)((u8 *)b + 0xA) = *(s16 *)(p + 0x20);
        *(s16 *)(p + 0x30) = *(u16 *)(p + 0x30) - 1;
        if (*(s16 *)(p + 0x30) > 0)
        {
            return;
        }
        func_800B8D64(*(s16 *)(p + 0x14), *(s16 *)(p + 0x16), *(s16 *)(p + 0x18));
        *(s16 *)(p + 0x30) = 5;
        ((s16 *)p)[0x16] = ((u16 *)p)[0x16] + 1;
        return;
    case 2:
        c[0xD] -= c[0xD] / *(s16 *)(p + 0x30);
        *(s16 *)(p + 0x30) = *(u16 *)(p + 0x30) - 1;
        if (*(s16 *)(p + 0x30) > 0)
        {
            return;
        }
        if (*(s16 *)(p + 0x38) != 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        c[0xD] = 0;
        *(s16 *)(p + 0x34) = 4;
        D_80027450 = 0;
        ((s16 *)p)[0x16] = ((u16 *)p)[0x16] + 1;
        return;
    case 3:
        sl = D_80082E80;
        if (func_8003DE58(*(u8 **)(sl + 0x8), (UnkSlot *)sl, (s16 *)pos, 0) != 0)
        {
            g2 = D_80083780;
            func_s69_8002614C(*(u16 *)(g2 + 0x2) + pos[0], *(u16 *)(g2 + 0x6) + pos[1],
                              *(u16 *)(g2 + 0xA) + pos[2], *(s16 *)(D_800E3D7C + 0x2A), 0);
        }
        *(s16 *)(p + 0x34) = *(u16 *)(p + 0x34) - 1;
        if (*(s16 *)(p + 0x34) > 0)
        {
            return;
        }
        g3 = D_80083780;
        func_s69_800255B8(*(s16 *)(g3 + 0x2), *(s16 *)(g3 + 0x6), *(u16 *)(g3 + 0xA) - 0x20,
                          *(s16 *)(D_800E3D7C + 0x2A));
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return;
    }
}

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

/* The 0x212 burst: spawn FOUR nodes on the caller, two of one kind and two of
   another, each at a random offset from the caller's own mover. Both loops run
   twice over func_8003FD64(0x212, a) and install this unit's
   func_s69_80024B00 as the +0x10 step; the first pair gets D_80045340 through
   func_8004491C and a slot dressed with this bank's D_80027484 at +0x8, the
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
void func_s69_80024C38(UnkNode *a, s16 b, s32 c)
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
            n->unk_10 = (u32)func_s69_80024B00;
            func_8004491C(n, (u32)D_80045340);
            q = (u8 *)n + 0x20;
            *(s16 *)((u8 *)n->unk_08 + 0x2) =
                *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x3F) - 0x20;
            *(s16 *)((u8 *)n->unk_08 + 0x6) =
                *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x3F) - 0x20;
            *(s16 *)((u8 *)n->unk_08 + 0xA) =
                *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x3F) - 0x20;
            r = (u8 *)n->unk_0C;
            *(u32 *)(r + 0x8) = (u32)D_80027484;
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
            n->unk_10 = (u32)func_s69_80024B00;
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

/* The big +0x10 step of this bank's mover actor, two whole programs under
   `*(s32 *)(p + 0x20)'.

   With +0x20 clear it is the FREE-FLIGHT arm: ramp the slot's grey triple
   towards 0x80 in state 0 and away from it in states >= 2, both over the
   +0x30 countdown, then aim the mover's velocity at rcos/rsin of the +0x34
   bearing (damped by the sheet frame at the slot's +0x4 while that is under
   3), integrate it, dress the renderable through func_s69_80025840 /
   func_s69_800257D0, and -- while the +0x46 fuse is still alight -- divide
   the mover's x/z by 64 into the +0x3C/+0x3E cell pair, and when that cell
   differs from the +0x40/+0x42 pair the actor last occupied, either go to
   state 3 if func_800A45D8 says the cell is blocked or walk the resident
   D_800814A8 actor ring and func_8009CE1C every other actor standing on that
   cell within 0x40 of height. Finally spawn the 0x202 trail through
   func_s69_80025FCC and hand off to func_s69_80025B78.

   With +0x20 set it is the ATTACHED arm: the node whose address is parked
   there with its top bit stripped either retires this actor (its own 0x8000)
   or lends it everything -- its mover's position, its slot's 0x16..0x20
   rotation block and 0xC colour word -- and, when this bank's D_8002758C
   table agrees with the lender's sheet frame, pushes the matrix,
   func_s69_8002593C, RotTrans of the zero vector into the mover's velocity
   triple, and runs func_800B653C over a stack UnkVec built from it.
   359 words, 0 differing.

   NO TWIN. dngtwin finds nothing carved at 0.70 or better, xsibs has been
   useless on five consecutive s-units, and the callee-set grep over
   func_800A45D8 / func_8009CE1C / func_800B653C / RotTrans / func_800A07D0 /
   D_800814A8 intersects in NO file -- the resident units each hold one or two
   of them and no landed body holds three. Written from the listing.
   csrc/ovl_dng_b09.c's func_b09_801745BC supplied the D_800814A8 ring-walk
   idiom (`*(u8 **)(h + 0x5C) + 0x20', payloads at node + 0x20).

   THREE FACTS, all measured on this body, and the first is worth 102 rows:

   (1) THE TWO POINTERS EACH DO DOUBLE DUTY ACROSS THE TWO ARMS. The ring
   cursor and the attached arm's `o + 0x20' are ONE local `q'; the ring's
   actor record and the attached arm's `o->unk_0C' are ONE local `c'. Written
   as four separate locals it is 359 words / 102 ALIGNED DIFFERENCES, every
   row a uniform register shift: `e' loses its callee-saved register to $a0
   and p/v/r slide from $s2/$s3/$s4 down to $s1/$s2/$s4. Merging doubles each
   allocno's reference count, which is what lifts both above the parameters in
   allocno_compare. The target's save list ($s0..$s4) is the evidence and it
   can be read before any compile: FIVE callee-saved registers for three
   parameters means exactly two more long-lived pointers, not four.

   (2) THE CELL MASK MUST BE `& 0xFFFFFFE0', NOT `& 0xFFE0'. As `0xFFE0' the
   mask stays HImode, combine proves the low six bits of `x * 64' zero and
   turns the `+ 0x20' into `ori $aN, $aN, 0x20' where the target has
   `addiu $aN, $aN, 0x20': 359 words / 2 aligned differences, one row per
   argument. `(u16)(x * 64 + 0x20) & 0xFFE0' also reaches 0 on these two rows,
   so the fact is about forcing the AND into SImode, not about which spelling
   does it. NOTE THE SIGN against docs/matching-levers.md's existing entry,
   which is about a mask coming out as `li -64': the same `0xFFFFFF..' form
   fixes both, but the diagnostic here is an `ori' standing in for an `addiu'.

   (3) THE RING CURSOR IS LOADED FROM D_800814A8 IN A STATEMENT OF ITS OWN
   before it is advanced: `q = (u8 *)D_800814A8; q = *(u8 **)(q + 0x5C) +
   0x20;'. Folded into one expression it is 359 words / 5 aligned differences
   -- the head value then never occupies `q''s register, so the
   `addu $v0, $s0, $zero' the target needs to keep the ring head alive past
   `q''s redefinition is absent and a `nop' takes its place. This is the same
   mechanism csrc/ovl_dng_b03.c records as "D_800814A8 must be a pointer local
   assigned before" its use, reached from the opposite direction.

   Measured and NOT free, so they are facts too: `__builtin_abs' on the height
   delta rather than two range tests against +-0x40 (two tests are 359 / 9),
   and the +0x40/+0x42 cell publish written BEFORE the +0x46 fuse decrement
   (after it, 360 words / 19). */
void func_s69_8002501C(u8 *p, UnkVec *v, u8 *r)
{
    UnkVec lv;
    SVECTOR sv;
    long flag;
    UnkNode *o;
    UnkVec *m;
    u8 *c;
    u8 *q;
    s16 st;

    if (*(s32 *)(p + 0x20) == 0)
    {
        st = *(s16 *)(p + 0x2C);
        if (st == 0)
        {
            r[0xE] = r[0xD] = r[0xC] = r[0xC] + (0x80 - r[0xC]) / *(s16 *)(p + 0x30);
            if ((*(s16 *)(p + 0x30) -= 1) <= 0)
            {
                *(s32 *)(r + 0xC) = 0x808080;
                *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
            }
        }
        else if (st >= 2)
        {
            r[0xE] = r[0xD] = r[0xC] = r[0xC] - r[0xC] / *(s16 *)(p + 0x30);
            if ((*(s16 *)(p + 0x30) -= 1) <= 0)
            {
                *(s32 *)(r + 0xC) = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
                return;
            }
            if (*(s16 *)(p + 0x2C) >= 3)
            {
                return;
            }
        }
        v->unk_0C = func_80064584(*(s16 *)(p + 0x34)) << 8;
        v->unk_10 = func_800644B8(*(s16 *)(p + 0x34)) << 8;
        if (*(s8 *)(r + 0x4) < 3)
        {
            v->unk_0C -= v->unk_0C >> (*(s8 *)(r + 0x4) + 1);
            v->unk_10 -= v->unk_10 >> (*(s8 *)(r + 0x4) + 1);
        }
        v->unk_08 -= v->unk_14;
        v->unk_00 += v->unk_0C;
        v->unk_04 += v->unk_10;
        v->unk_08 += v->unk_14;
        *(s16 *)(r + 0x16) = 0x400;
        *(s16 *)(r + 0x1A) =
            func_800A07D0(0, 0, *(s16 *)((u8 *)v + 0xE), *(s16 *)((u8 *)v + 0x12)) - 0x400;
        func_s69_80025840(r, (s16 *)(p + 0x32), *(s16 *)(p + 0x38), *(s16 *)(p + 0x36));
        func_s69_800257D0(p, *(u8 **)(r + 0x0));
        if ((*(u16 *)(r + 0x14) & 0x4000) != 0)
        {
            r[0x4] = 0;
        }
        if (*(s16 *)(p + 0x46) > 0)
        {
            *(s16 *)(p + 0x3C) = *(s16 *)((u8 *)v + 0x2) / 64;
            *(s16 *)(p + 0x3E) = *(s16 *)((u8 *)v + 0x6) / 64;
            if (*(s32 *)(p + 0x40) != *(s32 *)(p + 0x3C))
            {
                if (func_800A45D8((*(s16 *)(p + 0x3C) * 64 + 0x20) & 0xFFFFFFE0,
                                  (*(s16 *)(p + 0x3E) * 64 + 0x20) & 0xFFFFFFE0,
                                  *(s16 *)((u8 *)v + 0xA)) != 0)
                {
                    *(s16 *)(p + 0x30) = 4;
                    *(s16 *)(p + 0x2C) = 3;
                    return;
                }
                q = (u8 *)D_800814A8;
                q = *(u8 **)(q + 0x5C) + 0x20;
                while (q != (u8 *)D_800814A8)
                {
                    c = *(u8 **)(q - 0x14);
                    if (c[0x24] == *(s16 *)(p + 0x3C) && c[0x25] == *(s16 *)(p + 0x3E) &&
                        __builtin_abs(*(s16 *)(q + 0x88) - *(s16 *)((u8 *)v + 0xA)) < 0x40)
                    {
                        func_8009CE1C(q, 0xC, D_800287A2, 0xA, (s16)(*(u16 *)(p + 0x44) << 9),
                                      D_800287A4, 2);
                    }
                    q = *(u8 **)(q + 0x5C) + 0x20;
                }
                *(s16 *)(p + 0x40) = *(u16 *)(p + 0x3C);
                *(s16 *)(p + 0x42) = *(u16 *)(p + 0x3E);
                if ((*(s16 *)(p + 0x46) -= 1) <= 0)
                {
                    *(s16 *)(p + 0x30) = 4;
                    *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
                }
            }
        }
        func_s69_80025FCC(*(s16 *)((u8 *)v + 0x2), *(s16 *)((u8 *)v + 0x6),
                          *(s16 *)((u8 *)v + 0xA), *(s16 *)(p + 0x34));
        func_s69_80025B78(p, v, r);
    }
    else
    {
        o = (UnkNode *)(*(u32 *)(p + 0x20) | 0x80000000);
        if ((o->unk_1E & 0x8000) != 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            m = o->unk_08;
            c = (u8 *)o->unk_0C;
            *(s16 *)((u8 *)v + 0x2) = *(u16 *)((u8 *)m + 0x2);
            *(s16 *)((u8 *)v + 0x6) = *(u16 *)((u8 *)m + 0x6);
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)m + 0xA);
            *(s16 *)(r + 0x16) = *(u16 *)(c + 0x16);
            *(s16 *)(r + 0x18) = *(u16 *)(c + 0x18);
            *(s16 *)(r + 0x1A) = *(u16 *)(c + 0x1A);
            *(s16 *)(r + 0x1C) = *(u16 *)(c + 0x1C);
            *(s16 *)(r + 0x1E) = *(u16 *)(c + 0x1E);
            *(s16 *)(r + 0x20) = *(u16 *)(c + 0x20);
            *(s32 *)(r + 0xC) = *(s32 *)(c + 0xC);
            q = (u8 *)o + 0x20;
            if (D_8002758C[*(s16 *)(p + 0x48)] >= 0 &&
                *(s8 *)(c + 0x4) == D_8002758C[*(s16 *)(p + 0x48)])
            {
                PushMatrix();
                func_s69_8002593C(p, v, r);
                sv.vx = 0;
                sv.vy = 0;
                sv.vz = 0;
                RotTrans(&sv, (VECTOR *)((u8 *)v + 0xC), &flag);
                PopMatrix();
                *(s16 *)((u8 *)&lv + 0x2) = *(u16 *)((u8 *)v + 0xC);
                *(s16 *)((u8 *)&lv + 0x6) = *(u16 *)((u8 *)v + 0x10);
                *(s16 *)((u8 *)&lv + 0xA) = *(u16 *)((u8 *)v + 0x14);
                lv.unk_0C = func_80064584(*(s16 *)(q + 0x34)) << 4;
                lv.unk_10 = func_800644B8(*(s16 *)(q + 0x34)) << 4;
                lv.unk_14 = 0;
                func_800B653C((u8 *)&lv, *(s16 *)(q + 0x34));
            }
            func_s69_8002590C(r, *(s8 *)(c + 0x4));
            func_s69_800257D0(p, *(u8 **)(r + 0x0));
        }
    }
}

/* Spawn the twelve-node chain this bank's state 3 fires: reserve twelve 0x2
   nodes with func_8003FA44 and bail returning NULL if it refuses, then for
   each i allocate off D_80083498 for the first node and off node 0 for the
   rest, give each func_s69_8002501C as its +0x10 step and func_800C9034 as
   its func_8004491C record, place the mover at (x, y, z) pushed 0x20 units
   along D_8006CCD8/D_8006CCE8's direction pair for the caller's heading,
   dress the slot at 0x1000 scale with this bank's D_800274C0 row i and the
   0xC flag bits, park the node's PARENT at q+0x20 -- D_80027580[i] names
   which earlier node of the chain, masked 0x7FFFFFFF, or the previous one
   when that byte is zero -- and seed the payload's own fields from
   D_80082E80's +0x24/+0x25 bytes. Returns node 0. 134 words, 0 differing.
   One-member family, no jump table.

   NO TWIN AT ANY RATIO. THE DONOR WAS THE IN-UNIT SIBLING func_s69_800249DC,
   the nine-node burst four listings up: its chained-assignment idiom and its
   spawner skeleton are what this is. dngcallee's four 5-of-9 rows
   (func_b09_80173230, func_b36_801727B4, func_b37_80172840,
   func_b40_801729A0) were available and were not needed; dngnear had nothing.

   MEASURED, from the body at 0 differing:

     the three mover stores in offset order 0x2, 0x6, 0xA
     (as written)                                            134 /  0
     order 0x2, 0xA, 0x6 (the order the listing emits)    135 words /  4
     order 0x2, 0xA, 0x6 with the 0xA store written
     `((s16 *)m)[5]'                                         134 /  0
     both direction scales as `* 0x20' (as written)          134 /  0
     both as `<< 5'                                          134 /  7
     the two D_80082E80 bytes as chained assignments
     `*(s16 *)(q + 0x40) = *(s16 *)(q + 0x3C) = ...'         134 /  0
     the same four stores written separately             138 words / 18
     the `src' gate written `if (i != 0)' first              134 /  0
     written `if (i == 0)' first                         133 words /  5
     the +0x8 row address as `i * 0x10 + (u32)D_800274C0'    134 /  0
     as the pointer sum `(u32)(D_800274C0 + i * 0x10)'       134 /  0  FREE

   THE STORE ORDER IS THE INTERESTING ONE and it is a sched1 transposition
   read backwards: the listing emits 0x2, then the D_8006CCE8 load, then 0xA,
   then 0x6, so the obvious reading is a source order of 0x2, 0xA, 0x6 -- and
   that is one word LONG, because the `lh' of D_8006CCE8 then takes a nop
   instead of the z store. Plain ascending order lets sched1 sink the 0xA
   store into that load's delay slot itself. The array-ref spelling of the
   0xA store reaches 0 from the other order by dropping the same dependence,
   so BOTH spellings land and the target does not choose between them.

   AND THE FOUR SEPARATE D_80082E80 STORES COST FOUR WORDS, not an allocation:
   each `sh' to the q-based address invalidates cse's entry for the fixed
   D_80082E80 byte, so the second read of each byte survives as a second
   `lbu' with its own load-delay nop. The chained assignment gives the target's
   single `lbu' feeding two stores. */
UnkNode *func_s69_800255B8(s16 x, s16 y, s16 z, s16 ang)
{
    UnkNode *arr[12];
    void *src;
    u8 *m;
    u8 *r;
    u8 *q;
    s32 i;

    if (func_8003FA44(0xC) == 0)
    {
        return NULL;
    }
    for (i = 0; i < 0xC; i++)
    {
        if (i != 0)
        {
            src = arr[0];
        }
        else
        {
            src = D_80083498;
        }
        arr[i] = func_8003FD64(0x2, src);
        arr[i]->unk_10 = (u32)func_s69_8002501C;
        func_8004491C(arr[i], (u32)func_800C9034);
        m = (u8 *)arr[i]->unk_08;
        *(s16 *)(m + 0x2) = x + (D_8006CCD8[(ang >> 9) & 7] * 0x20);
        *(s16 *)(m + 0x6) = y + (D_8006CCE8[(ang >> 9) & 7] * 0x20);
        *(s16 *)(m + 0xA) = z;
        r = (u8 *)arr[i]->unk_0C;
        *(s16 *)(r + 0x20) = 0x1000;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(u32 *)(r + 0x8) = i * 0x10 + (u32)D_800274C0;
        *(s16 *)(r + 0x10) = 0x20;
        *(s16 *)(r + 0x16) = 0x400;
        *(s16 *)(r + 0x1A) = ang - 0x400;
        *(u16 *)(r + 0x14) |= 0xC;
        q = (u8 *)arr[i] + 0x20;
        if (i != 0)
        {
            if (D_80027580[i] != 0)
            {
                *(s32 *)(q + 0x20) = (s32)arr[D_80027580[i] - 1] & 0x7FFFFFFF;
            }
            else
            {
                *(s32 *)(q + 0x20) = (s32)arr[i - 1];
            }
        }
        *(s16 *)(q + 0x30) = 4;
        *(s16 *)(q + 0x36) = 0xF;
        *(s16 *)(q + 0x32) = 1;
        *(s16 *)(q + 0x34) = ang;
        *(s16 *)(q + 0x48) = i;
        *(s16 *)(q + 0x46) = 8;
        *(s16 *)(q + 0x40) = *(s16 *)(q + 0x3C) = D_80082E80[0x24];
        *(s16 *)(q + 0x42) = *(s16 *)(q + 0x3E) = D_80082E80[0x25];
    }
    return arr[0];
}

/* Dress a 0x20-byte record from one 22-byte animation frame: the frame's
   first 18 bytes verbatim, then its three signed bytes at +0x12..+0x14
   widened into words at the record's +0x14, +0x18 and +0x1C. The source is
   the slot's +0x00 element pointer that func_s69_80025840 / func_s69_8002590C
   step 22 bytes per frame; all three callers (func_s69_8002501C twice,
   func_s69_80025AAC once) pass `*(u8 **)(slot + 0x0)'.
   28 words, 0 differing.

   dngfam: 2 members, func_s62_80026240 is the other. The two listings are
   BYTE-IDENTICAL in all 28 instruction words and NEITHER names a single
   func_/D_/jtbl_ symbol beyond its own glabel, so there is no bank-local
   symbol and no per-copy immediate to get wrong here; dngclone --dry-run
   verifies the member at 28/28/0 and relcand reads `ok (0 data symbols,
   0 calls)' on both.

   THE ONE FACT, and it is what the whole body turns on: the 18-byte head is a
   struct assignment through an 18-byte type OF HALFWORDS, `s16 h[9]'. The
   alignment is the mechanism -- align 2 makes expand_block_move emit four
   unaligned lwl/lwr/swl/swr word pairs and then an ALIGNED, SIGN-EXTENDING
   `lh'/`sh' for the trailing halfword, which is exactly the target. Measured
   alternatives:
     `u8 b[0x12]' (align 1): 29 words / 4 aligned differences -- the trailing
       halfword splits into lb/lb/sb/sb.
     `u8 b[0x10]' plus a separate `*(s16 *)(d + 0x10) = *(s16 *)(s + 0x10)':
       28 words / 1 aligned difference, the lone row being `lhu' where the
       target has `lh'. A plain HImode move never sign-extends; only the block
       move's tail does. */
void func_s69_800257D0(u8 *d, u8 *s)
{
    *(UnkDngS69Rec18 *)d = *(UnkDngS69Rec18 *)s;
    *(s32 *)(d + 0x14) = *(s8 *)(s + 0x12);
    *(s32 *)(d + 0x18) = *(s8 *)(s + 0x13);
    *(s32 *)(d + 0x1C) = *(s8 *)(s + 0x14);
}

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

/* Build the attached actor's drawing matrix in the 0x1F800000 scratchpad:
   park the mover's position triple as a VECTOR at +0x40, read the current
   rotation matrix out of the GTE into a stack MATRIX (the read is discarded --
   only its side effect of latching the GTE matters), lay an IDENTITY 0x1000
   matrix at +0x50, turn it by the slot's +0x16 / +0x1A / +0x18 Euler angles
   in X, Z, Y order, hang the +0x40 translation on it, compose it with the
   caller's own matrix at `p' into +0xD0 and install that as the GTE's
   translation and rotation matrices. 62 words, 0 differing, FIRST COMPILE.

   NO TWIN: dngtwin finds nothing carved at 0.70 or better and dngfam puts it
   in a family of one. Written from the listing; eight of its nine calls have
   REAL NAMES out of splat/common_syms.txt and are spelled with them --
   ReadRotMatrix, RotMatrixX/Y/Z, TransMatrix, CompMatrix, SetTransMatrix,
   SetRotMatrix -- so no declaration block entry is needed for any of them.
   Note the install order: SetTransMatrix (0x80064D80) comes BEFORE
   SetRotMatrix (0x80064CF0), which is the reverse of the address order.
   csrc/ovl_dng_s21.c's scratchpad convention is the addressing model:
   `UnkDngS69SP *sp = (UnkDngS69SP *)0x1F800000' for the stores.

   THREE FACTS, all measured here:
     The nine identity stores must be written DESCENDING -- the three
     diagonal cells m[2][2], m[1][1], m[0][0] and then the six off-diagonals
     m[2][1] down to m[0][1]. Ascending it is 62 words / 12 aligned
     differences. This is the same descending-store idiom
     csrc/ovl_dng_s21.c records for its stack matrix's translation.
     ReadRotMatrix must come AFTER the three +0x40 stores, not before them:
     before, it is 63 WORDS / 37 aligned differences, because the `sh' of the
     third store is what fills the jal's delay slot.
     The three Euler angles must be read as `u16' (`lhu'). As `s16' they come
     out `lh' and it is 62 words / 3 aligned differences, one row per call.

   NOT MEASURABLE, which is different from free: writing the four passed
   scratchpad addresses as `&sp->unk_50' / `&sp->unk_40' / `&sp->unk_D0'
   instead of the integer constants `(MATRIX *)0x1F800050' etc. also reads
   62 words / 0 differing -- but `sp' is a constant pointer, so fold reduces
   both spellings to the same integer constant before any pass can tell them
   apart. csrc/ovl_dng_s21.c's park states the integer-constant form as a
   measured requirement; on THIS body the tools cannot see the difference, so
   that sentence is not confirmed here either way. */
void func_s69_8002593C(u8 *p, UnkVec *b, u8 *r)
{
    UnkDngS69SP *sp = (UnkDngS69SP *)0x1F800000;
    MATRIX lm;

    sp->unk_40.vx = *(s16 *)((u8 *)b + 0x2);
    sp->unk_40.vy = *(s16 *)((u8 *)b + 0x6);
    sp->unk_40.vz = *(s16 *)((u8 *)b + 0xA);
    ReadRotMatrix(&lm);
    sp->unk_50.m[2][2] = 0x1000;
    sp->unk_50.m[1][1] = 0x1000;
    sp->unk_50.m[0][0] = 0x1000;
    sp->unk_50.m[2][1] = 0;
    sp->unk_50.m[2][0] = 0;
    sp->unk_50.m[1][2] = 0;
    sp->unk_50.m[1][0] = 0;
    sp->unk_50.m[0][2] = 0;
    sp->unk_50.m[0][1] = 0;
    RotMatrixX(*(u16 *)(r + 0x16), (MATRIX *)0x1F800050);
    RotMatrixZ(*(u16 *)(r + 0x1A), (MATRIX *)0x1F800050);
    RotMatrixY(*(u16 *)(r + 0x18), (MATRIX *)0x1F800050);
    TransMatrix((MATRIX *)0x1F800050, (VECTOR *)0x1F800040);
    CompMatrix((MATRIX *)0x1F800050, (MATRIX *)p, (MATRIX *)0x1F8000D0);
    SetTransMatrix((MATRIX *)0x1F8000D0);
    SetRotMatrix((MATRIX *)0x1F8000D0);
}

/* The +0x10 step of a burst node: bump this bank's u16 counter, halve the
   slot's RGB triple at +0xC..+0xE, and raise the kill bit on the node flag at
   -0x2 and on D_800814A0 once the red channel drops under 8.
   30 words, 0 differing, FIRST COMPILE.

   TWIN: func_s69_800260D4, already landed two listings DOWN IN THIS SAME FILE
   (dngtwin 0.967, 30 words against 30); the same body also sits landed as
   func_s21_800248E4, func_s62_80024AB4 and func_s20_80024D70. Deltas: `>> 1'
   for its `>> 2' and a guard of 8 for its 4 -- same bank counter, since the
   twin is in this unit. Nothing else.

   ALL FOUR of the twin's facts HOLD and reproduce its prices to the digit:
     - the three fades are SEPARATE statements. Chained
       `[0] = [1] = [2] = [0] - ([0] >> 1)' it is 26 words / 17 aligned
       differences.
     - the guard RE-READS the byte. Guarding on the computed value through an
       `s32' local is 29 words / 18 aligned differences.
     - the counter bump must be the FIRST statement. After the three fades it
       is 33 words / 23 aligned differences.
     - FREE, as the twin also found: `/ 2' for `>> 1' is 30 words /
       0 differing. An unsigned divide by a power of two simply IS the `srl',
       so nothing selects the shift. */
void func_s69_80025A34(u8 *p, UnkVec *v, UnkSlot *s)
{
    D_80027452++;
    s->unk_0C[0] -= s->unk_0C[0] >> 1;
    s->unk_0C[1] -= s->unk_0C[1] >> 1;
    s->unk_0C[2] -= s->unk_0C[2] >> 1;
    if (s->unk_0C[0] < 8)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80025AAC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80025B78);

/* The 0x202 node's +0x10 step: bump this bank's D_80027452 counter, then
   rebuild the mover's six words from the payload's angle triple -- p+0x38 as
   the pitch (times 0x40), p+0x36 as the yaw, p+0x3E and p+0x40 as the two
   radii -- writing position into b->unk_00..unk_08 and velocity into
   b->unk_0C..unk_14, advance the yaw by 2 and the pitch by 1, fade the slot's
   +0xD by an eighth, and on the p+0x30 countdown reaching zero raise the kill
   bit at p-0x2 and on D_800814A0. The p+0x34 frame counter is bumped on every
   path. 169 words, 0 differing. One-member family, no jump table.

   NO TWIN AT ANY RATIO. THE DONOR WAS THE IN-UNIT SIBLING func_s69_80024B00,
   two listings up, and nothing else: its comment already prices the exact
   fact this body needed, because it is the same bank counter in the same
   position. dngcallee's twelve 3-of-4 rows and dngnear's s20/s21 donors were
   read and not needed.

   THE WHOLE FIGHT WAS ONE LINE, and it is a CONJUNCTION of two separate
   levers on it -- one-at-a-time pricing reaches neither:

     a = ((s16 *)p)[0x1C] * 0x40      (as written)        169 /  0
     a = ((s16 *)p)[0x1C] << 6                            169 /  1
     a = *(s16 *)(p + 0x38) * 0x40                        169 / 12
     a = *(s16 *)(p + 0x38) << 6                          169 / 13

   The ARRAY REF is true_dependence: through a plain cast the `sh
   %lo(D_80027452)' store keeps its edge to the `lh 0x38($s5)' load, so the
   store cannot sink into the first func_800644B8 call's delay slot and the
   counter bump is emitted whole before the angle is read -- the identical
   mechanism func_s69_80024B00 records at 8 differing. The MULTIPLY is
   convert.c: `LSHIFT_EXPR's narrowing arm is guarded on TREE_UNSIGNED and
   MULT_EXPR's is not, so the shift form lets gcc drop the load's sign and
   emits `lhu' for the target's `lh', with nothing else moving.

   MEASURED AND FREE, so it is NOT the reason this lands: the +0xD fade
   written `((u8 *)s)[0xD] -= ((u8 *)s)[0xD] >> 3' rather than through
   UnkSlot's `unk_0C[1]' (169 / 0 either way). */
void func_s69_80025D28(u8 *p, UnkVec *b, UnkSlot *s)
{
    s16 a;

    D_80027452++;
    a = ((s16 *)p)[0x1C] * 0x40;
    b->unk_00 = (*(s16 *)(p + 0x1C) << 16) +
                ((func_800644B8(a) * func_80064584(*(s16 *)(p + 0x36) + 0x400) *
                  *(s16 *)(p + 0x3E)) >>
                 7);
    b->unk_04 = (*(s16 *)(p + 0x1E) << 16) +
                ((func_800644B8(a) * func_800644B8(*(s16 *)(p + 0x36) + 0x400) *
                  *(s16 *)(p + 0x3E)) >>
                 7);
    b->unk_08 = (*(s16 *)(p + 0x20) << 16) + ((func_80064584(a) * *(s16 *)(p + 0x3E)) << 5);
    b->unk_0C = (*(s16 *)(p + 0x1C) << 16) +
                ((func_800644B8(a) * func_80064584(*(s16 *)(p + 0x36) + 0x400) *
                  *(s16 *)(p + 0x3E)) >>
                 7) +
                ((func_80064584(*(s16 *)(p + 0x36) + 0x800) * *(s16 *)(p + 0x40)) << 6);
    b->unk_10 = (*(s16 *)(p + 0x1E) << 16) +
                ((func_800644B8(a) * func_800644B8(*(s16 *)(p + 0x36) + 0x400) *
                  *(s16 *)(p + 0x3E)) >>
                 7) +
                ((func_800644B8(*(s16 *)(p + 0x36) + 0x800) * *(s16 *)(p + 0x40)) << 6);
    b->unk_14 = (*(s16 *)(p + 0x20) << 16) + ((func_80064584(a) * *(s16 *)(p + 0x3E)) << 5);
    *(s16 *)(p + 0x40) = *(u16 *)(p + 0x40) + 2;
    *(s16 *)(p + 0x38) = *(u16 *)(p + 0x38) + 1;
    s->unk_0C[1] -= s->unk_0C[1] >> 3;
    *(s16 *)(p + 0x30) = *(u16 *)(p + 0x30) - 1;
    if (*(s16 *)(p + 0x30) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    *(s16 *)(p + 0x34) = *(u16 *)(p + 0x34) + 1;
}

/* Spawn the 0x202 node at (x, y, z): func_s69_80025D28 as its +0x10 step,
   func_800CEEFC through func_8004491C, (x, y, z) written into BOTH the new
   mover's position and its velocity triple, 0x200 scales, 0x20 grey at the
   slot's +0xD, a plain 0xC in the slot's +0x14 flag word, this bank's
   D_80027460 as the slot's +0x8 record, 0x20 at the slot's +0x10 and the
   payload's +0x30, 0x10 at +0x3E, `a' at +0x36, 0x18 at +0x40, a rand() roll
   at +0x38 and (x, y, z) again at +0x1C..+0x20. The node is returned.
   66 words, 0 differing, FIRST COMPILE.

   NO USABLE TWIN. dngtwin's best carved sibling is func_lo16_8001F354 at
   0.729 and func_s21_800254FC at 0.720, both different spawners; dngfam puts
   this body in a family of one. Written from the listing on this unit's own
   spawner idiom (func_s69_800249DC two listings up: the same func_8003FC64 /
   `n->unk_10 = step' / func_8004491C(n, (u32)func_800CEEFC) order and the
   same `q = (u8 *)n + 0x20' payload cursor).

   TWO FACTS, both measured here:
     The slot's +0x14 is a PLAIN STORE of 0xC, not the `|= 0xC' every other
     spawner in this family uses (func_s21_800254FC, func_s69_8002614C). As
     `*(u16 *)(r + 0x14) |= 0xC' it is 67 words / 10 aligned differences.
     The `*(s16 *)(q + 0x40) = 0x18' store must come BEFORE the rand() call,
     because its `sh' is what fills the jal's delay slot. Moved after the call
     it is 66 words / 5 aligned differences.

   Measured and FREE at 66 words / 0 differing, so neither is why this lands:
   the three position/velocity pairs written as six SEPARATE stores in the
   listing's order instead of three chained assignments, and `q = (u8 *)n +
   0x20' computed before the two calls instead of after them -- which is the
   opposite of func_s69_800245A0's priced fact in this unit, where the same
   cursor costs 63 differences when it is live across a call. Here nothing
   between the calls uses it. */
UnkNode *func_s69_80025FCC(s16 x, s16 y, s16 z, s16 a)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *q;

    n = func_8003FC64(0x202);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s69_80025D28;
        func_8004491C(n, (u32)func_800CEEFC);
        m = n->unk_08;
        q = (u8 *)n + 0x20;
        *(s16 *)((u8 *)m + 0x2) = *(s16 *)((u8 *)m + 0xE) = x;
        *(s16 *)((u8 *)m + 0x6) = *(s16 *)((u8 *)m + 0x12) = y;
        *(s16 *)((u8 *)m + 0xA) = *(s16 *)((u8 *)m + 0x16) = z;
        r = (u8 *)n->unk_0C;
        *(s16 *)(r + 0x1E) = 0x200;
        *(s16 *)(r + 0x1C) = 0x200;
        r[0xD] = 0x20;
        *(s16 *)(r + 0x14) = 0xC;
        *(u32 *)(r + 0x8) = (u32)D_80027460;
        *(s16 *)(r + 0x10) = 0x20;
        *(s16 *)(q + 0x30) = 0x20;
        *(s16 *)(q + 0x3E) = 0x10;
        *(s16 *)(q + 0x36) = a;
        *(s16 *)(q + 0x40) = 0x18;
        *(s16 *)(q + 0x38) = rand();
        *(s16 *)(q + 0x1C) = x;
        *(s16 *)(q + 0x1E) = y;
        *(s16 *)(q + 0x20) = z;
    }
    return n;
}

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
