#include "common.h"

UnkNode *func_8003FC64(s32);
extern u8 D_800269FC[];
extern u8 D_800269D4[];
extern u8 D_800DF31C[];
void func_800B835C(u8 *, s32 *, s32, s32);
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
void func_s62_8002432C(u8 *p, UnkVec *b, u8 *c, u8 *d);

extern u16 D_800269F8;
extern u8 D_80045340[];
extern u8 D_800269EC[];
extern s16 D_80026B28;
extern u8 D_80027CA4[];
extern u8 *D_80027C98;
extern u8 D_800E3D20;
extern u8 D_80026BE4;
s32 func_8004491C(UnkNode *, u32);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
void func_800478B8(u8 *);
s32 func_s62_80025F54(s16 x, s16 y, s16 z, s16 a);
UnkNode *func_s62_80024B2C(s16 x, s16 y, s16 z, s16 a, s16 leaf);
void func_s62_80024AB4(u8 *p, UnkVec *v, UnkSlot *s);
void func_s62_8002654C(u8 *p);

void func_s62_80026240(u8 *d, u8 *s);
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

typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
typedef struct UnkDng73E94SP
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x12];
    u8 *unk_18;
    u8 pad_1C[0x4];
    u8 *unk_20;
    u8 pad_24[0x6C];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x28];
    s32 unk_C0;
} UnkDng73E94SP;
typedef struct UnkDng74334SP
{
    u8 pad_00[0xC];
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    Unk3DBD0W unk_18;
    u8 pad_1C[0x8];
    u8 *unk_24;
    u16 unk_28;
    u8 pad_2A[0xA];
    VECTOR unk_34;
    u8 pad_44[0x10];
    MATRIX unk_54;
    MATRIX unk_74;
    u8 pad_94[0x10];
    SVECTOR unk_A4;
    u8 pad_AC[0x4];
    SVECTOR unk_B0[4];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDng74334SP;
typedef struct UnkDngS72Dirs
{
    u8 unk_00[0x20];
} UnkDngS72Dirs;
typedef struct UnkDng25FB0Rot
{
    s16 m[3][3];
} UnkDng25FB0Rot;

s16 func_800A2CB8(u8 *a, u8 *b);
s16 func_800A41F0(u8 *);
s16 func_8009FD40(u8 *, u8 *);
s32 rand(void);
extern u8 D_80026AB0[];
void func_8003DB94(void *, void *, s32);
extern u8 D_80027750[];
extern u8 D_80027C74[];
extern s16 D_80027C94;
extern u8 D_80027C96;
extern s32 D_800269D0;
void func_80040490(void *, RECT *);
void func_8003F80C(u32, s32, s32, s16);
s32 func_800A56E0(s32);
extern u8 D_80083498[];
extern u8 D_80026B2C[];
extern u8 D_80026BBC[];
extern u8 D_80026BC8[];
s32 func_8003FA44(s32);
UnkNode *func_8003FD64(s32, void *);
s32 func_800A07D0i(s16, s16, s16, s16) __asm__("func_800A07D0");
s32 func_800C9034(u8 *, UnkVec *, UnkSlot *);
extern u8 D_80083780[];
u8 *func_s62_800244EC(u8 *m, u8 *act);
void func_s62_800262B0(u8 *s, s16 d);
void func_s62_800263F0(void);
void func_s62_80024DAC(u8 *, UnkVec *, UnkSlot *);

/* THE BANK'S ROOM SCRIPT: a five-state machine on the record's +0xA, with a
   shared tail that eases the two D_80083160 fade bytes at +0xA8/+0xA9 towards
   0x20 while the +0x20 countdown is negative, and clears this bank's
   D_800269F8 event counter on every call.
     state 0 publishes the room: a 0x3A0,0x100,0x20,0x40 RECT through
       func_80040490 on D_80027750, func_8003F80C over D_80027C74 at 0x7AC0,
       D_80027C94 = 1, D_800269D0 = 0, the record's +0x9 into D_80027C96, the
       resident's +0xF4 cleared, the record's +0x0 actor into D_80027C98, the
       record's +0x8 into the resident's +0xA8, func_s62_800263F0 to drop every
       actor's 0x2000 bit, then 0x14 at +0x96 and 1 at +0x102 -- and FALLS
       THROUGH into state 1;
     state 1 waits for the owning actor's +0x0 flag 0x80, spawns the 0x202 tint
       through func_s62_800244EC, parks it at the record's +0xC and arms the
       tail with +0x20 = -1 / +0x22 = 0x20 plus a 0x300 cue;
     state 2 waits for D_80027C94 to clear and arms +0x1A = 0x10;
     state 3 eases the same two fade bytes towards 0x80 over +0x1A frames and
       snaps them when it runs out;
     state 4 waits for D_800269F8, decrements D_80083460's +0xA, clears its
       +0xC and raises the kill bit on the record and on D_800814A0.
   178 words, 0 differing, SIX compiles.
   OWNS jtbl_s62_80024008 -- 6 .word = 5 cases + 1 pad, dispatch bound 5, no
   minval subtraction so the case set is {0,1,2,3,4}, five DISTINCT targets and
   no default arm of its own (the range-check `beqz' goes to the shared tail,
   which every arm reaches anyway). Entry order 0,1,2,3,4 is ascending by entry
   ADDRESS, so the arms are written ascending; the arms are not word-identical
   after cross-joining (they call different functions and differ in length), so
   the order is real and the 0-differing figure covers it.
   NO DONOR USED. dngcallee's best tier is 7 of 14 symbols with two members
   (func_s69_80024020, func_s55_80024078), both -0x28 with three saves against
   this -0x30 with five, and neither was opened; the body came off the listing.
   SIX FACTS, all priced from the landing:
     - LOAD-BEARING, 42 rows: D_80083160 must be reached through ONE `u8 *'
       LOCAL held for the whole function. Inline as
       `*(u8 *)((u8 *)&D_80083160 + 0xA9)' at all six sites it is 178 words /
       42 aligned differences. Same mechanism as func_s62_8002654C's
       D_80083178 fact in this unit, re-priced here.
     - LOAD-BEARING, 6 rows: state 0 must FALL THROUGH into state 1. With a
       `break' it is 175 words / 6 differences -- jump.c folds state 0's own
       copy of the +0xA increment into the shared one and the `j' to the tail
       goes.
     - LOAD-BEARING, 6 rows: in state 0 the D_80027C96 store must come BEFORE
       `D_800269D0 = 0'. The other way it is 178 words / 6 aligned
       differences, all of them the `lbu 0x9'/`lw D_800814A8' pair sliding
       below the D_800269D0 store.
     - LOAD-BEARING, 2 rows, AND THE LAST THING TO LAND: the actor pointer must
       be read into a LOCAL of its own BEFORE the +0xF4 store --
       `w = *(u8 **)p; <0xF4 store>; D_80027C98 = w;'. Written as the one
       statement `D_80027C98 = *(u8 **)p' after the 0xF4 store it is 178 words
       / 2 differences, with `sw $zero, 0xF4($a0)' two slots early. The
       mechanism is INSN_LUID: the plain-cast +0xF4 store and the
       fixed-address D_80027C98 store conflict in `memrefs_conflict_p', so
       their order is pinned to the source's, and the only free variable is the
       slot the store competes for against `lui %hi(D_80027C98)'. Both have
       priority 2, so `rank_for_schedule' (sched.c, the LUID tie-break at the
       bottom) decides on source position -- and splitting the load out gives
       the `lw' a LOWER LUID than the store, which is what moves the store two
       slots later. Moving the whole statement instead only overshoots: after
       the D_80027C98 store it is 178 / 2 the other way, at slot 49.
     - LOAD-BEARING, 1 row: state 4's gate must read D_800269F8 SIGNED. This
       unit declares `extern u16 D_800269F8' and four landed bodies here bump
       it unsigned, so the cast is written at the site rather than changing the
       declaration; as a plain `D_800269F8 == 0' it is `lhu' where the target
       has `lh', 178 words / 1 difference. The declaration was NOT touched.
     - LOAD-BEARING, 12 rows: the two countdowns' local must be `s16'. As
       `s32' it is 175 words / 12 differences -- the `sll 16' before each
       `bgtz' goes, which is the HImode promotion of the stored value. */
void func_s62_80024064(u8 *p)
{
    u8 *s;
    u8 *h;
    u8 *m;
    u8 *t;
    u8 *w;
    s16 n;
    s32 arr[2];

    s = (u8 *)&D_80083160;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        arr[0] = 0x10003A0;
        arr[1] = 0x400020;
        func_80040490(D_80027750, (RECT *)arr);
        func_8003F80C((u32)D_80027C74, 0x7AC0, 1, 2);
        D_80027C94 = 1;
        D_80027C96 = *(u8 *)(p + 0x9);
        D_800269D0 = 0;
        w = *(u8 **)p;
        *(s32 *)((u8 *)D_800814A8 + 0xF4) = 0;
        D_80027C98 = w;
        *(u8 *)((u8 *)D_800814A8 + 0xA8) = *(u8 *)(p + 0x8);
        func_s62_800263F0();
        *(s16 *)((u8 *)D_800814A8 + 0x96) = 0x14;
        *(u8 *)((u8 *)D_800814A8 + 0x102) = 1;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fall through */
    case 1:
        if ((*(u16 *)(*(u8 **)(p + 0x4)) & 0x80) != 0)
        {
            m = *(u8 **)p;
            t = func_s62_800244EC(*(u8 **)(m - 0x18), m);
            *(u8 **)(p + 0xC) = t;
            if (t != NULL)
            {
                *(s16 *)(p + 0x20) = -1;
                *(s16 *)(p + 0x22) = 0x20;
                func_800A56E0(0x300);
                *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            }
        }
        break;
    case 2:
        if (D_80027C94 == 0)
        {
            *(s16 *)(p + 0x1A) = 0x10;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 3:
        *(u8 *)(s + 0xA9) += (0x80 - *(u8 *)(s + 0xA9)) / *(s16 *)(p + 0x1A);
        *(u8 *)(s + 0xA8) += (0x80 - *(u8 *)(s + 0xA8)) / *(s16 *)(p + 0x1A);
        n = *(u16 *)(p + 0x1A) - 1;
        *(s16 *)(p + 0x1A) = n;
        if (n <= 0)
        {
            *(u8 *)(s + 0xA9) = 0x80;
            *(u8 *)(s + 0xA8) = 0x80;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        if (*(s16 *)&D_800269F8 == 0)
        {
            h = (u8 *)&D_80083460;
            *(s32 *)(h + 0xC) = 0;
            *(s16 *)(h + 0xA) = *(u16 *)(h + 0xA) - 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    if (*(s16 *)(p + 0x20) < 0)
    {
        *(u8 *)(s + 0xA9) += (0x20 - *(u8 *)(s + 0xA9)) / *(s16 *)(p + 0x22);
        *(u8 *)(s + 0xA8) += (0x20 - *(u8 *)(s + 0xA8)) / *(s16 *)(p + 0x22);
        n = *(u16 *)(p + 0x22) - 1;
        *(s16 *)(p + 0x22) = n;
        if (n <= 0)
        {
            *(s16 *)(p + 0x20) = 0;
        }
    }
    D_800269F8 = 0;
}

/* The step func_s62_800244EC parks, in three states on *(s16 *)(p + 0x2C).
   State 0 waits for func_8003DE58 to project the owning node's slot into the
   payload's +0x24 offset triple (or for that slot to retire), then adds the
   triple onto the owner's mover, re-dresses the actor through func_8004491C
   and advances; state 1 fades the renderable's colour word up by 0x202020 a
   frame until its first byte reaches 0xC0, then advances if
   func_s62_80025F54 accepts the mover's position at the D_80027C98
   record's +0x2A bearing; state 2 fades the word down by 0x40404 and retires
   the actor at zero. The owner node is reached at p+0x8, which is where
   func_s62_800244EC stashes `act - 0x20'.
   112 words, 0 differing, THREE compiles.
   TWIN: the landed func_s56_80024388 in csrc/ovl_dng_s56.c (dngtwin 0.778,
   109 words). Of its two stated facts:
     - HOLDS: the owner node load sits ABOVE the +0x2E test, which is what
       lets the already-projected path reach the shared tail on the hoisted
       $v0 while the just-projected path reloads it after the call. Moved
       inside the `== 0' arm it is 114 words / 12 aligned differences.
     - NOT APPLICABLE / INVERTED: it reads the state TWICE at the top, once
       signed and once unsigned, and feeds state 1's bump from the unsigned
       one. Here there is ONE signed `lh' at the top and the bump re-reads
       `lhu' in the block the two arms share. Adding the twin's second top
       read is 114 words / 22 aligned differences.
   THE TWO FACTS THAT COST THE THREE COMPILES, both about the head:
     - The state local must be `s32', not `s16'. As `s16' the load comes out
       `lhu' plus an `sll 16'/`sra 16' pair instead of `lh': 114 words / 7
       aligned differences.
     - The state READ must come BEFORE the counter bump. With
       `D_800269F8++' first the counter store schedules ahead of the `lh',
       reorg can no longer carry it into the `bnez' delay slot and steals the
       `b' parameter copy instead, leaving the load delay a `nop': 113 words
       / 11 aligned differences. This is csrc/ovl_dng_s46.c's
       func_s46_80024C0C fact with the global store on the OTHER side of the
       load. The SPLIT form that unit uses --
       `t = D_800269F8 + 1; st = load; D_800269F8 = t;' -- is also 112 words
       / 0, so the split is FREE and the fact is the ORDER alone.
   Measured and FREE, so neither is why this lands: the `st' local itself
   (read inline at all three gates, cse unifies it and the figure is
   unchanged), and `*(s32 *)(c + 0xC) += 0xFFFBFBFC' in place of
   `-= 0x40404'. LOAD-BEARING: the shared tail's mover load must be the
   re-dereferenced `(*(UnkNode **)(p + 0x8))->unk_08' -- reusing the hoisted
   `o' is 112 words / 25 aligned differences. */
void func_s62_8002432C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *o;
    UnkSlot *r;
    UnkVec *m;
    s32 st;

    st = *(s16 *)(p + 0x2C);
    D_800269F8++;
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
        func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
        *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
    }
    else if (st == 1)
    {
        if (*(u8 *)(c + 0xC) < 0xC0)
        {
            *(s32 *)(c + 0xC) += 0x202020;
        }
        else if (func_s62_80025F54(*(s16 *)((u8 *)b + 0x2), *(s16 *)((u8 *)b + 0x6),
                                   *(s16 *)((u8 *)b + 0xA), *(s16 *)(D_80027C98 + 0x2A)) != 0)
        {
            *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
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

/* The 0x202 tint node's spawner: run func_800B835C over {0x1000340,
   0x200020} on D_800DF31C and then over {0x1000360, 0x200020} on
   D_800269D4, park func_s62_8002432C as the new node's step, seed its mover
   from the caller's, point its slot at D_800269FC with a 0xC00 scale, stash
   the owning node at the payload's +0xC and return the payload.
   csrc/ovl_dungeon_800C2824.c's func_800C4AFC is the template: same stack
   `s32 arr[2]', same `func_800B835C(..., arr, 1, 0)', same mover copy. Three
   deltas, all off this listing: func_8003FC64(2) in place of
   func_8003FD64(0x202, D_80083498), TWO func_800B835C calls (the second
   rewrites arr[0] and arr[1], which is the second `sw 0x14($sp)'), and no
   payload pointer -- the node is reached at +0x10 and +0x2C directly and the
   return is `(n != NULL) ? (u8 *)n + 0x20 : NULL'.
   67 words, 0 differing, first compile.
   THIS COPY'S OWN CONSTANT: 0x1000360 -> 0x1200340, 0x2c -> 0x28.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
u8 *func_s62_800244EC(u8 *m, u8 *act)
{
    UnkNode *n;
    UnkVec *v;
    u8 *r;
    s32 arr[2];

    n = func_8003FC64(2);
    if (n != NULL)
    {
        arr[0] = 0x1000340;
        arr[1] = 0x200020;
        func_800B835C(D_800DF31C, arr, 1, 0);
        arr[0] = 0x1200340;
        arr[1] = 0x200020;
        func_800B835C(D_800269D4, arr, 1, 0);
        v = n->unk_08;
        n->unk_10 = (u32)func_s62_8002432C;
        *(s16 *)((u8 *)v + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)v + 0x6) = *(u16 *)(m + 0x6);
        *(s16 *)((u8 *)v + 0xA) = *(u16 *)(m + 0xA);
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0x8) = (u32)D_800269FC;
        *(s16 *)(r + 0x1E) = 0xC00;
        *(s16 *)(r + 0x1C) = 0xC00;
        *(u8 **)((u8 *)n + 0x28) = act - 0x20;
    }
    return (n != NULL) ? (u8 *)n + 0x20 : NULL;
}

/* A 0x202 sibling step with a scale ramp: bump the bank's counter, advance
   the mover by its velocity, add 0xC000 to the +0x14 component, re-send the
   slot through func_800478B8, fade all three colour bytes by one 1/(+0x30)
   step of a single computed value, ease both scale halves 1/(+0x30) of the
   way towards 0x400, and retire the actor when +0x30 runs out.
   69 words, 0 differing, TWO compiles.
   TWINS: the landed func_s56_80025460 (csrc/ovl_dng_s56.c, dngtwin 0.752,
   56 words) for the fade and the retirement, and the landed
   func_s21_800253F8 (csrc/ovl_dng_s21.c, 0.701, 65 words) for the
   func_800478B8 hand-off. Added here over both: the +0x1C/+0x1E scale ramp.
   Of their stated facts:
     - HOLDS (s56): the three colour bytes share ONE computed value.
       Re-reading per channel is 83 words / 16 aligned differences; the
       chained `[0] = [1] = [2] = ...' is 69 words / 4.
     - INVERTED (s21): its "three colour fades are separate statements" is
       the s56 shape's opposite and is the 16-difference row above. The
       discriminator is whether the fade divides (one value, three stores) or
       shifts per channel (three values).
     - FREE (s56): the three-statement retirement. `if (--*(s16 *)(p + 0x30)
       <= 0)' is also 69 words / 0.
   THE FACT THAT COST THE SECOND COMPILE, and it is a WIDTH, not a local: the
   scale ramp must not pass through an `s32'. Written
   `f = s->unk_1C; f += (0x400 - f) / n; s->unk_1C = f; s->unk_1E = f;' with
   `s32 f' the `lhu 0x1C' hoists into the first division's mflo hazard slot,
   above the three colour stores, and it is 68 words / 17 aligned
   differences. The kept memory form and the same body with `u16 f' are both
   69 words / 0, so the target does not choose between them -- only the
   32-bit local is wrong.
   LOAD-BEARING: `v->unk_08 += v->unk_14' must precede
   `v->unk_14 += 0xC000' -- reversed it is 68 words / 29 aligned
   differences. */
void func_s62_800245F8(u8 *p, UnkVec *v, UnkSlot *s)
{
    s32 e;
    s16 n;

    D_800269F8++;
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    v->unk_14 += 0xC000;
    func_800478B8((u8 *)s);
    e = s->unk_0C[0] - s->unk_0C[0] / *(s16 *)(p + 0x30);
    s->unk_0C[0] = e;
    s->unk_0C[1] = e;
    s->unk_0C[2] = e;
    s->unk_1C += (0x400 - s->unk_1C) / *(s16 *)(p + 0x30);
    s->unk_1E = s->unk_1C;
    n = *(u16 *)(p + 0x30) - 1;
    *(s16 *)(p + 0x30) = n;
    if (n <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The scattered 0x202 puff burst: spawn `cnt' nodes at (x, y, z + 0x10)
   stepped by func_s62_800245F8 just below, each flung at a random bearing in
   [a - 0xC00, a - 0x400) with speed rcos/rsin << 6 and a random +0x14 lift in
   [-0x10000, 0x10000), 0x1000 scales, a 0x808080 tint, one of eight
   D_80026AB0 sprites and a lifetime of 0x10..0x17 at the payload's +0x30.
   Returns the LAST node allocated -- NULL when `cnt' <= 0, which is why the
   `n = NULL' is needed and func_s62_80024938's fixed 32-iteration loop does
   not have one.
   95 words, 0 differing, THREE compiles.
   SIBLING: func_s62_80024938 above, landed first, which is the same
   allocate/install/func_8004491C/dress skeleton with a different step and
   different constants; its own facts carried here (the `q' payload local,
   `s16' bearing). dngcallee's 7-of-8 tier has seven members and none of them
   was needed.
   THE THREE FACTS, all priced from the landing:
     - `e = d - 0x800;' MUST BE ITS OWN STATEMENT. Written
       `ang = a + (d - 0x800)' it is 95 words / 5 aligned differences: fold
       reassociates `PLUS(a, MINUS(d, 0x800))' into `PLUS(MINUS(a, 0x800), d)',
       `a - 0x800' is then loop-invariant, loop.c hoists it into the preheader
       beside the `sra' and the final `addu' comes out with its operands
       exchanged. Across two statements there is nothing for fold to
       reassociate and combine will not re-merge them, because the merged form
       is no shorter. `(d - 0x800) + a' in one expression keeps the `addiu' in
       the loop but gives `addu $s0, $s0, $s4' for the target's
       `addu $s0, $s4, $s0', 95 words / 6 -- so the ADDEND ORDER is `a' first
       and is a separate fact from the split.
     - THE LOOP MUST COUNT DOWN FROM `cnt'. `for (i = 0; i < cnt; i++)' is 102
       words / 50 aligned differences and is NOT reversed by check_dbra_loop,
       because the `s16' bound needs an `sll'/`sra' that stays inside the loop;
       the extra pseudo then costs a spill of `x' to 0x10($sp). This is the
       counter-example to the "an emitted count-down loop means the source
       counts up" lever: the lever's precondition is that the bound is already
       a word, and a SUB-WORD bound inverts it.
     - `d' must be `s16'. As `s32' it is 92 words / 7 aligned differences -- the
       `sll 16'/`sra 16' pair after the `addiu -0x400' goes, and that pair is
       the narrowing of the random offset, not of `a'. */
UnkNode *func_s62_8002470C(s16 x, s16 y, s16 z, s16 a, s16 cnt)
{
    UnkNode *n;
    UnkSlot *r;
    UnkVec *m;
    u8 *q;
    s32 i;
    s32 ang;
    s32 e;
    s16 d;

    n = NULL;
    for (i = cnt; i > 0; i--)
    {
        n = func_8003FC64(0x202);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s62_800245F8;
            func_8004491C(n, (u32)D_80045340);
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = x;
            *(s16 *)((u8 *)m + 0x6) = y;
            *(s16 *)((u8 *)m + 0xA) = z + 0x10;
            d = (rand() & 0x7FF) - 0x400;
            e = d - 0x800;
            ang = a + e;
            m->unk_0C = func_80064584(ang) << 6;
            m->unk_10 = func_800644B8(ang) << 6;
            m->unk_14 = (rand() & 0x1FFFF) - 0x10000;
            r = (UnkSlot *)n->unk_0C;
            r->unk_1E = 0x1000;
            r->unk_1C = 0x1000;
            *(u32 *)r->unk_0C = 0x808080;
            func_8003DB94(r, D_80026AB0, rand() & 0x7);
            q = (u8 *)n + 0x20;
            *(s16 *)(q + 0x30) = (rand() & 0x7) + 0x10;
            *(s16 *)(q + 0x36) = a;
        }
    }
    return n;
}

/* A 0x202 sibling step: bump the bank's counter, advance the mover by its
   velocity, add 0x2000 of gravity to the +0x14 component, fade all three of
   the slot's colour bytes by one sixteenth of a single computed value, and
   retire the actor when the payload's +0x30 counter runs out. A leaf -- no
   frame, and `p' lives in $t0 because $a0 is reused as a temp.
   44 words, 0 differing, FIRST COMPILE.
   TWIN: the landed func_s56_80025460 in csrc/ovl_dng_s56.c (dngtwin 0.800,
   56 words). Mine is 12 words SHORTER and all twelve are MISSING STATEMENTS,
   not missing references: the twin's two rotation bumps (+0x16 += +0x20,
   +0x18 += +0x22) are absent here and its 1/(+0x4C) division is a plain
   `>> 4'. Of its two stated facts:
     - HOLDS: the three colour bytes SHARE ONE COMPUTED VALUE -- one lbu, one
       srl/subu and three sb of the same register, so the fade is an `s32'
       local assigned once and stored three times. Re-reading the byte per
       channel is 49 words / 9 aligned differences; the chained
       `[0] = [1] = [2] = ...' is 44 words / 4, because a QImode store wipes
       cse's memory table and the chain cannot carry the value either.
     - FREE, so it does not constrain this body: it says the retirement is
       the three-statement `n = load - 1; store n; if (n <= 0)' form. Here
       `if (--*(s16 *)(p + 0x30) <= 0)' is also 44 words / 0.
   Also measured and FREE: the shared value declared `u8' instead of `s32'
   (44 / 0). LOAD-BEARING: `v->unk_08 += v->unk_14' must precede
   `v->unk_14 += 0x2000' -- reversed it is 43 words / 8 aligned
   differences. */
void func_s62_80024888(u8 *p, UnkVec *v, UnkSlot *s)
{
    s32 b;
    s16 n;

    D_800269F8++;
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    v->unk_14 += 0x2000;
    b = s->unk_0C[0] - (s->unk_0C[0] >> 4);
    s->unk_0C[0] = b;
    s->unk_0C[1] = b;
    s->unk_0C[2] = b;
    n = *(u16 *)(p + 0x30) - 1;
    *(s16 *)(p + 0x30) = n;
    if (n <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x202 ring burst: spawn THIRTY-TWO nodes at (x, y, z) stepped by
   func_s62_80024888 just above, giving each a velocity of one 32nd of a turn
   round the circle -- (rcos(a + 0x400) * rcos(i << 7)) >> 6 on the mover's
   +0xC, (rsin(a + 0x400) * rcos(i << 7)) >> 6 on +0x10 and
   rsin(i << 7) << 6 on +0x14 -- with 0x400 scales, D_800269EC at the slot's
   +0x8, a 0x808080 tint, 0x20 at +0x10 and the slot's 0xC flags, then 0xC at
   the payload's +0x30 and the bearing at +0x36. Returns the LAST node
   allocated, which is NULL if the 32nd allocation failed.
   95 words, 0 differing, FIRST COMPILE.
   DONOR: the landed func_s62_80024B2C in this same unit -- dngcallee's only
   6-of-6 row, against a next tier of 5/6 with fourteen members. Its spawn arm
   is this body's dress block with different constants (0x800 scales, 0x60 at
   +0x10, +0x50 = 0x10), and its driver arm is where the `(i << 7)' angle and
   the rcos/rsin pair come from. The frames differ (-0x40 with nine saves
   here, -0x58 with ten there), so only its SHAPE facts were taken.
   FOUR FACTS, all priced from the landing:
     - LOAD-BEARING: the payload needs a `u8 *q = (u8 *)n + 0x20' LOCAL. Spelled
       func_s62_80024B2C's way -- `*(s16 *)((u8 *)n + 0x50)' inline at both
       sites -- it is 94 words / 5 aligned differences, because the `addiu
       $v1, $s3, 0x20' goes and the two `sh' fold their offsets onto $s3.
       So the donor's inline spelling INVERTS here; it has one such site and
       this has two.
     - LOAD-BEARING: `a' must be an `s16' PARAMETER. As `s32' it is 93 words /
       33 aligned differences -- the `sll 16'/`sra 16' pair AND the `sh
       $a3, 0x10($sp)' spill both go, and the spill is what the +0x36 store
       reads back.
     - LOAD-BEARING: `a + 0x400' must be written INLINE at both call sites, not
       through a local. An `s32 ang = a + 0x400;' before the loop is 95 words /
       20 aligned differences: loop.c hoists the expression into $s5 by itself,
       and the local makes it a VAR_DECL that is live from the function's
       entry block instead, which rotates $s5/$s4.
     - NOT MEASURABLE: the slot's +0x8 and +0xC stores in either order are both
       95 words / 0 differing, because they are independent and sched1 emits
       them in the listing's order (0xC first) whichever way the source has
       them. The donor's order is kept.

   THIS SIGNATURE IS WRONG AND LANDING func_s62_80024DAC WILL REQUIRE WIDENING
   IT.  This body measures 95/95 at 0 differing with FOUR parameters, which is
   why it is committed that way -- but the still-parked func_s62_80024DAC
   CALLS IT WITH FIVE, twice: case 5 at listing address 0x80025770 and case 6
   at 0x80025A04, both passing `(rand() & 3) | 4' as `sw $v0, 0x10($sp)'.
   This body reads no fifth argument, so an unread `s32` fifth parameter is
   what the pair needs, and THIS BODY MUST BE RE-MEASURED at that point --
   adding a parameter changes the prologue's view of the argument registers.
   The agent that landed this tried the widening while working the park and
   reverted it, so csrc is self-consistent today and will stop being so the
   moment the park lands.  docs/dng-parks/func_s62_80024DAC.c records the same
   fact from the caller's end. */
UnkNode *func_s62_80024938(s16 x, s16 y, s16 z, s16 a)
{
    UnkNode *n;
    UnkSlot *r;
    UnkVec *m;
    u8 *q;
    s32 i;

    for (i = 0; i < 0x20; i++)
    {
        n = func_8003FC64(0x202);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s62_80024888;
            func_8004491C(n, (u32)D_80045340);
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = x;
            *(s16 *)((u8 *)m + 0x6) = y;
            *(s16 *)((u8 *)m + 0xA) = z;
            m->unk_0C = (func_80064584(a + 0x400) * func_80064584(i << 7)) >> 6;
            m->unk_10 = (func_800644B8(a + 0x400) * func_80064584(i << 7)) >> 6;
            m->unk_14 = func_800644B8(i << 7) << 6;
            r = (UnkSlot *)n->unk_0C;
            r->unk_1E = 0x400;
            r->unk_1C = 0x400;
            r->unk_08 = (u32)D_800269EC;
            *(u32 *)r->unk_0C = 0x808080;
            r->unk_10 = 0x20;
            r->unk_14 |= 0xC;
            q = (u8 *)n + 0x20;
            *(s16 *)(q + 0x30) = 0xC;
            *(s16 *)(q + 0x36) = a;
        }
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
void func_s62_80024AB4(u8 *p, UnkVec *v, UnkSlot *s)
{
    D_800269F8++;
    s->unk_0C[0] -= s->unk_0C[0] >> 2;
    s->unk_0C[1] -= s->unk_0C[1] >> 2;
    s->unk_0C[2] -= s->unk_0C[2] >> 2;
    if (s->unk_0C[0] < 4)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The recursive burst at this bank's D_80027CA4 aim point. Called with
   leaf != 0 it spawns ONE 0x202 node at (x, y, z) stepped by
   func_s62_80024AB4, with 0x800 scales, D_800269EC at the slot's +0x8, a
   0x808080 tint, 0x60 at +0x10, the slot's 0xC flags and 0x10 at the
   payload's +0x50, and returns it. Called with leaf == 0 it is the DRIVER:
   when D_80026B28 is exactly 3 it walks sixteen steps of one sixteenth of
   the way from (x, y, z) to D_80027CA4, offsetting each step by rcos/rsin of
   the caller's angle times rsin of (i << 7) >> 21, and recurses with
   leaf = 1 at each; then, if D_80026B28 is 0, it publishes (x, y, z) into
   D_80027CA4, and either way bumps D_80026B28.
   160 words, 0 differing, FIRST COMPILE.
   TWIN: the landed func_s69_8002614C in csrc/ovl_dng_s69.c (dngtwin 0.975,
   160 words, same frame -0x58 and the same ten saves). Deltas are this
   bank's relink only: 0x202 for 0x212, this bank's step, aim point and
   counter, 0x800 scales for 0x400 and 0x60 at +0x10 for 0x20.
   Of its three stated facts:
     - HOLDS, and reproduces its prices nearly to the digit: the aim point
       needs TWO SEPARATE `u8 *' LOCALS, one per basic block -- `g' in the
       delta block and `h' in the publish. One local shared by both blocks is
       12 aligned differences (the twin: 12); inline as
       `*(s16 *)(D_80027CA4 + 0x2)' the `+ 2' folds into the symbol address
       and the base is never materialised, 24 (the twin: 22).
     - HOLDS, but its price does NOT transfer: the `leaf' gate must be
       `if (leaf != 0)' with the spawn as the `then' arm. With the driver as
       the `then' arm it is 160 words / 67 aligned differences, where the
       twin measured the same inversion at 1.
     - INVERTED: the twin measured three `sx'/`sy'/`sz' locals copied from
       the parameters and used in the publish as FREE. Here they are 160
       words / 10 aligned differences; the target's three unconditional
       `sh' stores into the frame at 0x18/0x1A/0x1C are the parameters being
       SPILLED round the loop, not declared locals, so the publish must read
       the parameters directly. */
UnkNode *func_s62_80024B2C(s16 x, s16 y, s16 z, s16 a, s16 leaf)
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
        n = func_8003FC64(0x202);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s62_80024AB4;
            func_8004491C(n, (u32)D_80045340);
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = x;
            *(s16 *)((u8 *)m + 0x6) = y;
            *(s16 *)((u8 *)m + 0xA) = z;
            r = (UnkSlot *)n->unk_0C;
            r->unk_1E = 0x800;
            r->unk_1C = 0x800;
            r->unk_08 = (u32)D_800269EC;
            *(u32 *)r->unk_0C = 0x808080;
            r->unk_10 = 0x60;
            r->unk_14 |= 0xC;
            *(s16 *)((u8 *)n + 0x50) = 0x10;
        }
    }
    else
    {
        if (D_80026B28 == 3)
        {
            g = D_80027CA4;
            dx = (*(s16 *)(g + 0x2) - x) / 16;
            dy = (*(s16 *)(g + 0x6) - y) / 16;
            dz = (*(s16 *)(g + 0xA) - z) / 16;
            cx = x + dx;
            cy = y + dy;
            cz = z + dz;
            for (i = 0; i < 0x10; i++)
            {
                func_s62_80024B2C(cx + ((func_80064584(a) * func_800644B8(i << 7)) >> 21),
                                  cy + ((func_800644B8(a) * func_800644B8(i << 7)) >> 21), cz, a,
                                  1);
                cx += dx;
                cy += dy;
                cz += dz;
            }
        }
        if (D_80026B28 == 0)
        {
            h = D_80027CA4;
            *(s16 *)(h + 0x2) = x;
            *(s16 *)(h + 0x6) = y;
            *(s16 *)(h + 0xA) = z;
        }
        D_80026B28++;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_80024DAC);

/* Build the nine-link chain the room's state 1 asks for: reserve nine slots
   with func_8003FA44(9), then spawn nine type-2 nodes CHAINED through
   func_8003FD64 -- the first off D_80083498, each later one off its
   predecessor -- give each func_s62_80024DAC as its step and func_800C9034 as
   its dress, seat it at (x, y, z), point its slot at this bank's
   D_80026B2C + 0x10 per link with a 0x400/-0x400 facing pair, then fill the
   payload: the parent link from D_80026BBC's table (masked with 0x7FFFFFFF)
   or the previous node, 0x10 at +0x34, the bearing a+0x400 at +0x38, its
   octant at +0x44, D_80026BC8's per-link flag as 0xF or 0 at +0x3A, 1 at
   +0x36, the link index at +0x88, and the node itself into the PREVIOUS
   payload's +0x24. Then wrap the stored bearing into [-0x800,0x800) and
   snap it to func_800A07D0's bearing to D_80083780 when they are more than
   0x800 apart, record which side at +0x96, prime the eight-entry trail at
   +0x48..+0x84 with the seat position, seek the slot to frame 0 and dress it
   through func_s62_80026240. Returns the FIRST node, or 0 when the reservation
   fails.
   187 words, 0 differing, EIGHT compiles.
   NO DONOR: dngcallee's best tier is 4 of 9 symbols with seven members, which
   is the noise floor; the body came off the listing. The two callees it shares
   with this unit -- func_s62_800262B0 and func_s62_80026240 -- are the reason
   the smaller bodies were worth landing first: both were landed before this,
   and func_s62_800262B0's own `d' parameter is what told me the seek's second
   argument is 0 rather than absent.
   EIGHT FACTS, all RE-PRICED from the landing (four of them changed verdict
   between the mid-fight figure and the clean one, so take only these):
     - LOAD-BEARING, 11 rows: `D_80026BBC[i]' must be written out TWICE, once
       in the test and once in the index, with NO local. An `s32 k' holding it
       is 187 words / 11 aligned differences, all of them $v0/$v1 exchanged
       across the whole parent-link block -- the local's pseudo takes the
       load's destination where the target wants the ADDRESS there.
     - LOAD-BEARING, 9 and 6 rows, AND IT IS A POSITION NOT A HOIST:
       `g = D_80083780;' must sit INSIDE the loop, immediately before the
       func_800A07D0 call. Before the loop it is 9 aligned differences and as
       the FIRST statement of the loop body it is 6; both of those give `g'
       $s7 and the frame-array base $fp, where the target has them the other
       way round. loop.c hoists the address into the preheader in all three
       cases, so the word count never moves -- what changes is the weighted
       reference count flow.c gives `g' (a set at loop depth 1 counts 2), and
       only the latest position drops it below the array base in
       `allocno_compare'.
     - LOAD-BEARING, 6 rows: the clamp must be `__builtin_abs'. Written
       `t = ...; if (t < 0) { t = -t; }' it is 187 words / 6 differences -- the
       same `bgez'/`negu' pair, but the two loads of +0x38 and the `(s16)'
       widening schedule in the wrong order around it. Same word count, so
       this is a SCHEDULING fact, not a shape one.
     - LOAD-BEARING, 10 rows: the 12-bit sign extension of func_800A07D0's
       result must be assigned BACK TO `c'. With a second local `cc' it is 186
       words / 10 differences: a COND_EXPR whose arms read the variable it is
       assigned to needs a temp and then a copy, and that copy is the target's
       `addu $a0, $v0, $zero'. A fresh destination lets gcc expand the arms
       straight into it and the copy goes.
     - LOAD-BEARING, 4 rows: the high mask must be the SIGNED `-0x1000'.
       `0xFFFFF000' makes the expression unsigned, `convert.c' then distributes
       the halfword store's truncation into the `&', and you get
       `andi $v0, $a0, 0xF000' where the target has `addiu $v0, $zero, -0x1000'
       plus a full `and': 186 words / 4 differences. This is the convert.c
       narrowing lever with the sign the OTHER WAY from its usual statement --
       here the HEX form is the trap and the negative decimal is the fix,
       because the mask does not fit `andi' only while it stays signed.
     - LOAD-BEARING, 3 rows: func_800A07D0's result must be taken as `s32'
       through a unit-local `__asm__' alias. With the corpus's `s16'
       declaration it is 188 words / 3 differences, the extra word being the
       `sll 16'/`sra 16' the target does not have -- every use of the result is
       masked, so the widening is pure cost.
     - LOAD-BEARING, 7 rows: the trail loop must count DOWN from 7. As
       `for (j = 0; j < 8; j++)' it is 188 words / 7 differences and is NOT
       reversed, because the index feeds an address biv. Second witness in this
       unit against the "an emitted count-down loop means the source counts up"
       lever; the first is func_s62_8002470C.
     - FREE, both at 187 words / 0 differing: the parent link written as TWO
       stores in an if/else instead of through the `link' local (the two arms
       cross-join into one `sw'), and the `& 0xFFF' read taken inline inside
       the `if' instead of into a local before it. Both were priced at 1 word
       and 2 rows EARLIER in the fight, while the `cc' and `-0x1000' faults
       were still live -- they are the batch's "an improvement is not evidence"
       case, from the other direction. */
s32 func_s62_80025F54(s16 x, s16 y, s16 z, s16 a)
{
    u8 *g;
    u8 *q;
    u8 *prev;
    UnkVec *m;
    UnkSlot *r;
    s32 i;
    s32 j;
    s32 c;
    s32 d;
    u8 *link;
    s32 t;
    s32 v;
    UnkNode *arr[9];

    prev = NULL;
    if (func_8003FA44(9) == 0)
    {
        return 0;
    }
    for (i = 0; i < 9; i++)
    {
        arr[i] = func_8003FD64(2, (i == 0) ? (void *)D_80083498 : (void *)arr[i - 1]);
        arr[i]->unk_10 = (u32)func_s62_80024DAC;
        func_8004491C(arr[i], (u32)func_800C9034);
        m = arr[i]->unk_08;
        *(s16 *)((u8 *)m + 0x2) = x;
        *(s16 *)((u8 *)m + 0x6) = y;
        *(s16 *)((u8 *)m + 0xA) = z;
        r = (UnkSlot *)arr[i]->unk_0C;
        r->unk_16 = 0x400;
        r->unk_1A = a - 0x400;
        r->unk_08 = (u32)(D_80026B2C + i * 0x10);
        r->unk_10 = 0x60;
        r->unk_14 |= 0xC;
        q = (u8 *)arr[i] + 0x20;
        if (i != 0)
        {
            link = (D_80026BBC[i] != 0) ? (u8 *)((u32)arr[D_80026BBC[i] - 1] & 0x7FFFFFFF)
                                        : (u8 *)arr[i - 1];
            *(u8 **)(q + 0x20) = link;
        }
        *(s16 *)(q + 0x34) = 0x10;
        *(s16 *)(q + 0x38) = a + 0x400;
        *(s16 *)(q + 0x44) = (a >> 9) & 7;
        *(s16 *)(q + 0x3C) = 0;
        *(s16 *)(q + 0x3A) = (D_80026BC8[i] != 0) ? 0xF : 0;
        *(s16 *)(q + 0x36) = 1;
        *(s16 *)(q + 0x88) = i;
        if (prev != NULL)
        {
            *(u32 *)(prev + 0x24) = (u32)arr[i];
        }
        g = D_80083780;
        c = func_800A07D0i(*(s16 *)((u8 *)m + 0x2), *(s16 *)((u8 *)m + 0x6), *(s16 *)(g + 0x2),
                           *(s16 *)(g + 0x6));
        v = *(u16 *)(q + 0x38);
        *(s16 *)(q + 0x38) = ((v & 0x800) != 0) ? (v | 0xF800) : (v & 0x7FF);
        c = ((c & 0x800) != 0) ? (c | 0xF800) : (c & 0x7FF);
        d = (s16)c;
        t = __builtin_abs(*(s16 *)(q + 0x38) - d);
        if (t > 0x800)
        {
            *(s16 *)(q + 0x38) = (c & -0x1000) | (*(u16 *)(q + 0x38) & 0xFFF);
        }
        *(s16 *)(q + 0x96) = *(s16 *)(q + 0x38) < d;
        for (j = 7; j >= 0; j--)
        {
            *(s16 *)(q + 0x48 + j * 8) = *(u16 *)((u8 *)m + 0x2);
            *(s16 *)(q + 0x4A + j * 8) = *(u16 *)((u8 *)m + 0x6);
            *(s16 *)(q + 0x4C + j * 8) = *(u16 *)((u8 *)m + 0xA);
        }
        func_s62_800262B0((u8 *)r, 0);
        func_s62_80026240(q, *(u8 **)r);
        prev = q;
    }
    return (s32)arr[0];
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
void func_s62_80026240(u8 *d, u8 *s)
{
    *(UnkDngS69Rec18 *)d = *(UnkDngS69Rec18 *)s;
    *(s32 *)(d + 0x14) = *(s8 *)(s + 0x12);
    *(s32 *)(d + 0x18) = *(s8 *)(s + 0x13);
    *(s32 *)(d + 0x1C) = *(s8 *)(s + 0x14);
}

/* Seek a renderable to animation frame `d': store the frame index as the
   slot's signed +0x04 byte, clear the +0x05 sub-counter, point the slot's
   +0x00 element at its sheet's +0xC plus 22 bytes per frame, and drop both
   clamp flags (0x4000 and 0x2000) out of the +0x14 flag halfword.
   func_s62_800263C0 just below is this body's last statement on its own, and
   func_s62_800262F4 above is the stepping version that RAISES 0x4000.
   17 words, 0 differing, TWO compiles.
   No twin: dngfam gives one member and this is a two-statement widening of
   the landed func_s62_800263C0 in this same unit, which is where the `s16'
   parameter and the `* 22' spelling come from.
   LOAD-BEARING, measured: the flag clear must be the LAST statement. With it
   before the element-pointer store it is 17 words / 9 differing, and the nine
   rows are the `lw 0xC($v1)' sinking below the `andi'/`sh' pair instead of
   being hoisted above it -- the same figure whether the clear sits first,
   second or third, so it is the position RELATIVE TO the pointer chain and
   not the slot it occupies.
   LOAD-BEARING, measured: `d' must be an `s16' PARAMETER, as in
   func_s62_800263C0. As `s32' the `sll 16'/`sra 16' promotion pair goes and
   the body is 15 words / 17 differing. */
void func_s62_800262B0(u8 *s, s16 d)
{
    *(s8 *)(s + 0x4) = d;
    *(s8 *)(s + 0x5) = 0;
    *(s32 *)s = *(s32 *)(*(u8 **)(s + 0x8) + 0xC) + d * 22;
    *(u16 *)(s + 0x14) &= 0x9FFF;
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
void func_s62_800262F4(u8 *s, s16 *b, s16 lo, s16 hi)
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
void func_s62_800263C0(u8 *s, s16 d)
{
    *(s32 *)s = *(s32 *)(*(u8 **)(s + 0x8) + 0xC) + d * 22;
}

/* Walk the whole +0x5C actor ring from D_800814A8 and clear the 0x2000 bit in
   every node's +0x1E flag halfword.  21 words, 0 differing.

   The ring idiom is csrc/ovl_dng_b42.c's func_b42_801748FC (the `g = p - 0x20'
   node base at the top of the loop body) crossed with csrc/ovl_dng_s69.c's
   func_s69_8002501C fact (3) -- the cursor is loaded from D_800814A8 in a
   statement of its OWN and then advanced, so the head value passes through
   `p''s register and the test gets it as a copy.  Both facts re-priced here:

   | spelling                                              | words | differing |
   | as written below                                       |  21   |     0     |
   | `h = (u8 *)D_800814A8; p = *(u8 **)(h + 0x5C) + 0x20'  |  19   |    19     |
   | the store as `*(u16 *)(g + 0x1E) &= 0xDFFF'            |  19   |    20     |
   | no `g', `((u16 *)(p - 0x20))[0xF]' written inline      |  18   |    12     |

   THE ARRAY-REF STORE IS THE WHOLE SECOND WORD PAIR.  The target reads
   D_800814A8 once and keeps the ring head in TWO registers -- `addu $v0, $a0,
   $zero' for the entry test and `addu $a1, $v0, $zero' for the loop test.
   The second copy only exists if loop.c may hoist the loop test's read of the
   global above the `sh' in the body, and a PLAIN CAST store blocks that:
   true_dependence refuses to excuse an anonymous store against a
   fixed-address scalar, so the test re-reads the global inside the loop
   (`lw $v1, 0($a1)') and the body comes out two words short.  Written as an
   array ref MEM_IN_STRUCT_P is true, the hoist is allowed, and cse2 turns the
   hoisted load back into the copy.  This is the MEM_IN_STRUCT_P hoist
   polarity, measured on a 21-word body. */
void func_s62_800263F0(void)
{
    u8 *p;
    u8 *g;

    p = (u8 *)D_800814A8;
    p = *(u8 **)(p + 0x5C) + 0x20;
    while (p != (u8 *)D_800814A8)
    {
        g = p - 0x20;
        ((u16 *)g)[0xF] &= 0xDFFF;
        p = *(u8 **)(p + 0x5C) + 0x20;
    }
}

/* Pick the ring member `p' should react to: walk the whole +0x5C actor ring
   from D_800814A8 and return the first node that func_800A2CB8 and
   func_800A41F0 both admit, whose own kill bit (0x2000 at -0x2) is clear, and
   which either belongs to `p''s own +0x26 class (when that class is
   non-negative) or lies within `lim' of `p' by func_8009FD40; NULL if the
   walk completes. `lim' is 4 for a negative class and 0x100 otherwise, and
   since only the negative-class path reads it the 0x100 arm is unreachable.
   66 words, 0 differing, FOUR compiles.
   DONOR: the landed func_800A3D18 in csrc/ovl_dungeon_8009F644.c (132 words
   against this 66). No carved twin -- dngfam gives one member -- and the
   donor was found by intersecting `grep -ln' over the three callees, which
   names exactly three files; func_800A3D18 is the only body in any of them
   that calls all three. It supplied the `&&' chain over the two admission
   tests and the `e = *(u8 **)(c - 0x14)' / `+0x26' class compare verbatim.
   Its fact (1) -- the PARAMETER is the walker and the head is the local --
   does NOT apply here: this body walks from the global, as
   func_s62_800263F0 above does, so the ring idiom is that function's.
   THE TWO FACTS THAT COST THE COMPILES, both measured from the landing:
     - `lim' must be `s16'. As `s32' it is 65 words / 17 aligned differences:
       the HImode pseudo is what forces the second `lim' pseudo and so the
       `addu $s3, $a1, $zero' copy in the loop preheader, which is the missing
       word. The two 32-bit spellings that reach the same 65/17 and are
       therefore NOT the fault: `lim = 0x100;' with a bare `if' for the 4, and
       the ternary. Permuting the declaration order is also 65/17, so
       allocation priority is out -- the width is the whole fact.
     - `c = (u8 *)D_800814A8' must come BEFORE the `lim' if/else: 66 words /
       38 aligned differences after it. The if/else is what puts that read and
       the loop's entry test in DIFFERENT cse blocks; written after it, cse1
       unifies the two reads, the head survives in a register
       (`move $v1, $s0') and the %hi stops being hoisted into a callee-saved
       register, so every in-loop use rematerialises its own `lui' into the
       delay slots the target leaves as `nop'. This is func_s62_800263F0's
       19-word/19-difference row with a basic-block boundary, not a store,
       doing the blocking.
   LOAD-BEARING, measured from the landing: the non-negative class compare
   must be the `then' arm (`if (k >= 0)'). Inverted to `if (k < 0)' with the
   distance test first it is 66 words / 14 aligned differences, every row the
   two arms exchanged and the `bltz'/`bgez' flip. */
u8 *func_s62_80026444(u8 *p)
{
    u8 *c;
    u8 *e;
    s16 lim;
    s32 k;

    c = (u8 *)D_800814A8;
    if (*(s8 *)(p + 0x26) < 0)
    {
        lim = 4;
    }
    else
    {
        lim = 0x100;
    }
    c = *(u8 **)(c + 0x5C) + 0x20;
    while (c != (u8 *)D_800814A8)
    {
        if ((s16)func_800A2CB8((u8 *)D_800814A8, c) != 0 && (s16)func_800A41F0(c) != 0 &&
            (*(u16 *)(c - 0x2) & 0x2000) == 0)
        {
            k = *(s8 *)(p + 0x26);
            e = *(u8 **)(c - 0x14);
            if (k >= 0)
            {
                if (k == *(s8 *)(e + 0x26))
                {
                    return c;
                }
            }
            else if ((s16)func_8009FD40(e, p) < lim)
            {
                return c;
            }
        }
        c = *(u8 **)(c + 0x5C) + 0x20;
    }
    return NULL;
}

/* The camera step the installer below parks in D_80083178's +0xB4 slot.
   `p' is the record it fills, D_80083178 + 0xB8. Ease the engine's +0x98
   bearing towards the request's over the frames left at +0x24, run that
   counter down with a -0x80 floor, spend one frame of D_800E3D20, copy the
   mover's (x, z, bearing) into the record's +0x4/+0x6/+0x8 and ease the three
   +0xA4/+0xA6/+0xA8 axes a quarter of the way towards them -- the third only
   while this bank's D_80026BE4 is set, which is then re-armed from whether
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
void func_s62_8002654C(u8 *p)
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
    if (D_80026BE4 != 0)
    {
        *(s16 *)(g + 0xA8) += (*(s16 *)(p + 0x8) - *(s16 *)(g + 0xA8)) >> 2;
    }
    D_80026BE4 = *(s16 *)(g + 0xA8) != *(s16 *)(p + 0x8);
    *(s16 *)(g + 0x94) = *(s16 *)(g + 0x94) >> 2;
    *(s16 *)(g + 0x96) = *(s16 *)(g + 0x96) >> 2;
}

/* Install func_s62_8002654C as D_80083178's +0xB4 camera step and fill its
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
   the other way. */
s32 func_s62_80026694(void *a, u8 *b, s16 c, s32 d)
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
    *(u32 *)(q + 0x14) = 0xD;
    *(u32 *)(q + 0x18) = 0;
    *(s16 *)(q + 0x26) = d;
    *(u32 *)(s + 0xB4) = (u32)func_s62_8002654C;
    if (r & 1)
    {
        D_800E3D20 = c;
    }
    *(s16 *)(q + 0x24) = c;
    return r;
}
