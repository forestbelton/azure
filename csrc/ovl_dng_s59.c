#include "common.h"

s32 rand(void);

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
void func_s59_8002406C(u8 *p, UnkVec *v, UnkSlot *s, s16 d);

extern s16 D_80025FF4;
s32 func_800BCB04(s32, s32, s16);

UnkNode *func_8003FD64(s32, void *);
s32 func_s59_80024598(u8 *p, UnkVec *v);
void func_s59_800247E8(u8 *p, UnkVec *v);

typedef struct UnkDngS67Dirs
{
    u8 unk_00[0x20];
} UnkDngS67Dirs;

typedef struct UnkDngS59Name12
{
    u8 b[0xC];
} UnkDngS59Name12;
void func_s59_80024AB4(u8 *p, UnkVec *v, UnkSlot *s);
void func_s59_800249E0(u8 *p, UnkVec *v, UnkSlot *s);
extern u8 D_80025FE8[];
UnkNode *func_8003FC64(s32);
void func_8003DB94(void *, void *, s32);
void func_800478B8(u8 *);
extern u8 D_80045340[];
extern u8 D_800DECF8[];

/* One semi-transparent gouraud quad per slot element, with its own DR_MODE:
   compose the slot's rotation with the camera matrix in the 0x1F800000
   scratchpad, publish a 0xC-byte DR_MODE on the resident OT root, then
   RotAverage4 the four SVECTORs taken off the payload's +0x4C..+0x63 into a
   0x24-byte POLY_G4 at the +0x8D0 cursor and -- when the OT index is in range
   and at least one corner is on screen -- give it the payload's four colour
   words, the element's +0x1 flag byte and a second DR_MODE.
   310 words, 0 differing, FIRST COMPILE.

   The twin is the LANDED func_s46_80024198 in csrc/ovl_dng_s46.c (310 words,
   dngtwin 0.994), statement for statement, with the SAME -0x70 frame and the
   same nine saves s0..s7 in the same order.  Three operand deltas and nothing
   else: this copy reads its four interleaved vertex triples at p+0x4C
   (stride 6) where s46 reads them at p+0x20, takes its four colour words off
   p+0xC/+0x10/+0x14/+0x18 where s46 uses p+0x0/+0x4/+0x8/+0xC, and passes 0
   rather than 1 as func_80067F20's third argument at BOTH dress sites -- the
   same 0 func_s59_80024598 below uses.

   All three of the twin's priced facts were RE-MEASURED on this body and all
   three reproduce its verdict, two of them to the digit:
     - HOLDS: the four on-screen tests are `|' and not `||' -- as `||' this is
       313 words / 20 aligned differences / 143 differing, the same 143 the
       twin recorded, because short-circuiting splits the four sltiu pairs
       into separate blocks.
     - FREE, so it is NOT the reason this lands: the scale vector being ONE
       CHAINED assignment.  Three separate stores written vz, vy, vx also
       measure 310 words / 0 aligned differences.  Only the STORE ORDER
       matters and the chain merely supplies it.
     - HOLDS: `e = (u8 *)s->unk_08;' comes BEFORE `sp->unk_28 = s->unk_14;'.
       Swapped it is 310 words / 3 aligned differences, the twin's figure
       exactly.
   The twin's fourth fact (`&m' does not go through a pointer local) is NOT
   APPLICABLE here for the same reason it was there: `m' is only ever passed
   by address to ReadRotMatrix and CompMatrix, so there is no local to
   introduce. */
extern u8 D_80082E80[];
extern s16 D_80082E86;
extern u16 D_80082E94;
extern u8 D_800DDC40[];
extern u8 D_80025FD0[];
extern u8 D_80025FDC[];
s32 func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
s32 func_80053EF0(s32);
void func_800A56E0(s32);
void func_s59_800248A8(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz);
void func_s59_80024DD4(u8 *p, s32 c1, s32 c2, s16 x, s16 y, s16 z);
void func_s59_80024F60(u8 *p, UnkVec *v, s32 c, s16 x, s16 y, s16 z);

void func_s59_8002406C(u8 *p, UnkVec *v, UnkSlot *s, s16 d)
{
    UnkDng74334SP *sp = (UnkDng74334SP *)0x1F800000;
    Unk80083160 *g;
    MATRIX m;
    u8 *q;
    u8 f;
    u8 *e;

    g = &D_80083160;
    sp->unk_24 = g->unk_00 + 0xB0;
    sp->unk_74.t[0] = *(s16 *)((u8 *)v + 0x2);
    sp->unk_74.t[1] = *(s16 *)((u8 *)v + 0x6);
    sp->unk_74.t[2] = *(s16 *)((u8 *)v + 0xA);
    s->unk_14 |= 0x8000;
    PushMatrix();
    sp->unk_34.vx = sp->unk_34.vy = sp->unk_34.vz = 0x2000;
    sp->unk_A4.vx = s->unk_16;
    sp->unk_A4.vz = s->unk_1A;
    sp->unk_A4.vy = s->unk_18;
    RotMatrix(&sp->unk_A4, &sp->unk_74);
    ReadRotMatrix(&m);
    CompMatrix(&m, &sp->unk_74, &sp->unk_54);
    ScaleMatrix(&sp->unk_54, &sp->unk_34);
    SetTransMatrix(&sp->unk_54);
    SetRotMatrix(&sp->unk_54);
    e = (u8 *)s->unk_08;
    sp->unk_28 = s->unk_14;
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q + 0xC;
    func_80067F20((u32 *)q, 0, 0, GetTPage(0, 0, 0, 0), 0);
    addPrim(g->unk_00 + 0xB0, q);
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q + 0x24;
    sp->unk_B0[0].vx = *(u16 *)(p + 0x4C);
    sp->unk_B0[1].vx = *(u16 *)(p + 0x52);
    sp->unk_B0[2].vx = *(u16 *)(p + 0x58);
    sp->unk_B0[3].vx = *(u16 *)(p + 0x5E);
    sp->unk_B0[0].vy = *(u16 *)(p + 0x4E);
    sp->unk_B0[1].vy = *(u16 *)(p + 0x54);
    sp->unk_B0[2].vy = *(u16 *)(p + 0x5A);
    sp->unk_B0[3].vy = *(u16 *)(p + 0x60);
    sp->unk_B0[0].vz = *(u16 *)(p + 0x50);
    sp->unk_B0[1].vz = *(u16 *)(p + 0x56);
    sp->unk_B0[2].vz = *(u16 *)(p + 0x5C);
    sp->unk_B0[3].vz = *(u16 *)(p + 0x62);
    sp->unk_100 = RotAverage4(&sp->unk_B0[0], &sp->unk_B0[1], &sp->unk_B0[2], &sp->unk_B0[3],
                              (long *)(q + 0x8), (long *)(q + 0x10), (long *)(q + 0x18),
                              (long *)(q + 0x20), (long *)&sp->unk_D0, (long *)&sp->unk_D4) -
                  d - 6;
    if ((u32)sp->unk_100 < 0x1E0)
    {
        if (((u16)(*(u16 *)(q + 0x8) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0xA) + 0x20) < 0x121) |
            ((u16)(*(u16 *)(q + 0x10) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x12) + 0x20) < 0x121) |
            ((u16)(*(u16 *)(q + 0x18) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x1A) + 0x20) < 0x121) |
            ((u16)(*(u16 *)(q + 0x20) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x22) + 0x20) < 0x121))
        {
            s->unk_14 &= 0x7FFF;
            f = e[1];
            s->unk_0C[3] = f;
            if ((sp->unk_28 & 0x8) != 0)
            {
                if ((sp->unk_28 & 0x4) != 0)
                {
                    s->unk_0C[3] = f | 0x2;
                }
                else
                {
                    s->unk_0C[3] = f & 0xFD;
                }
            }
            *(s32 *)(q + 0x4) = *(s32 *)(p + 0xC);
            *(s32 *)(q + 0xC) = *(s32 *)(p + 0x10);
            *(s32 *)(q + 0x14) = *(s32 *)(p + 0x14);
            *(s32 *)(q + 0x1C) = *(s32 *)(p + 0x18);
            setPolyG4(q);
            setSemiTrans(q, 1);
            addPrim(sp->unk_24 + sp->unk_100 * 4, q);
            q = *(u8 **)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)q + 0xC;
            func_80067F20((u32 *)q, 0, 0, GetTPage(0, 1, 0, 0), 0);
            addPrim(sp->unk_24 + sp->unk_100 * 4, q);
        }
    }
    PopMatrix();
}

/* Walk the tag chain from `p` and hand every node to func_s59_8002406C with a
   zero fourth argument.  21 words, 0 differing, first compile.

   The twin is func_b06_80173E08 two listings up in THIS unit -- the same
   walk with a different per-node call -- and csrc/ovl_dungeon_800D50FC.c's
   func_800D67B0 is the resident original, identical down to the
   `s = (UnkSlot *)n->unk_0C;` cast.  Nothing was fought for; the loop is a
   `for (;;)` with the NULL test in the middle, which is what puts the
   `addiu $s0, $a2, 0x20` in the `beqz`'s delay slot. */
s32 func_s59_80024544(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_s59_8002406C(p, v, s, 0);
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

/* One semi-transparent 1x1 tile and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad, with the tile's colour word
   copied off the payload at +0x8 and then faded: each of the three bytes is
   scaled by the payload's s16 counter at +0x32 over its s16 limit at +0x34.

   The twin is the LANDED func_s16_8002416C in csrc/ovl_dng_s16.c, also 148
   words -- same scratchpad slots, same addPrim pair, same
   setTile1/setSemiTrans fold -- differing in ONE token: the fade's divisor is
   the loaded `*(s16 *)(p + 0x34)' here where s16 divides by the constant
   0x100. That is why both come out at 148: the variable divide's
   lh/div/mflo triple costs exactly what the constant divide's
   bgez/addiu-0xFF/sra correction does.

   Landed on the FIRST COMPILE, so the sibling's three measured orderings were
   taken as given and are NOT re-measured here -- the colour word and its
   three scalings before the tag setup, `sp->unk_18' assigned before
   `sp->unk_20', and `p = (u8 *)n + 0x20;' before `v = n->unk_08;'.
   148 words, 0 differing. */
s32 func_s59_80024598(u8 *p, UnkVec *v)
{
    UnkDng73E94SP *sp = (UnkDng73E94SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    TILE_1 *t;
    DR_MODE *dm;

    sp->unk_18 = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    while (1)
    {
        sp->unk_00 = *(u16 *)((u8 *)v + 0x2);
        sp->unk_02 = *(u16 *)((u8 *)v + 0x6);
        sp->unk_04 = *(u16 *)((u8 *)v + 0xA);
        t = (TILE_1 *)sp->unk_18;
        sp->unk_18 = (u8 *)(t + 1);
        sp->unk_C0 = RotTransPers((SVECTOR *)sp, (long *)&t->x0, (long *)&sp->unk_90, (long *)&sp->unk_94);
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            *(u32 *)&t->r0 = *(u32 *)(p + 0x8);
            t->r0 = t->r0 * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
            t->g0 = t->g0 * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
            t->b0 = t->b0 * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
            setTile1(t);
            setSemiTrans(t, 1);
            addPrim(sp->unk_20 + sp->unk_C0 * 4, t);
            dm = (DR_MODE *)sp->unk_18;
            sp->unk_18 = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            addPrim(sp->unk_20 + sp->unk_C0 * 4, dm);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_18;
    return 0;
}

/* Turn the mover towards func_800BCB04's bearing -- gliding one frame by the
   payload's +0x48 step, but only while the mover is still below that bearing
   -- and run the +0x32 countdown down by 4 a frame, raising the node's redraw
   bit and D_800814A0's at zero. csrc/ovl_dungeon_800CABA8.c's func_800CAED0
   is the twin: the same bearing test over 67 words, with a full three-axis
   glide where this has one axis, no `else' arm and a 4-step countdown instead
   of 8.

   THE THREE ARGUMENT LOADS ARE THREE LOCALS, ASSIGNED BEFORE THE D_80025FF4
   STORE, IN THE ORDER +0xA, +0x2, +0x6. Written the twin's way, with the three
   loads inside the call, it is 6 differing at the right 48 words, every row
   the `li 1'/`sh' pair and the three `lhu's trading places. The cause is a
   true dependence, read out of `tools/decomp/sched.py --rtl' and not
   inferred: gcc 2.7.2 cannot disambiguate the `mem:HI (lo_sum D_80025FF4)'
   store from `mem:HI (s0 + 10)', so every argument load carries insn 17 --
   the store -- in its dependency list and can never be scheduled above it.
   calls.c emits a call's argument register loads at the very END of its
   expansion, so the only way to get them above the store is to evaluate them
   in an earlier statement.

   Measured and rejected, all at 48 words: the twin's spelling 6; the call
   inlined into the comparison 6; `*(s16 *)&D_80025FF4 = 1' 6; hoisting only
   the +0xA load 4; hoisting all three in source order +0x2, +0x6, +0xA 3 --
   the three `lhu's then come out in that order against the target's +0xA
   first, the tie breaking on LUID, which is why the +0xA load is written
   first. `do { D_80025FF4 = 1; } while (0)' is 49 words and 44 differing.
   The `+ 2' stays inside the argument: its `addiu' sits AFTER the store in
   the target.

   Macro residue rather than a device -- three loads named and then handed to
   one call is what a bearing-test macro leaves behind. 48 words,
   0 differing. */
void func_s59_800247E8(u8 *p, UnkVec *v)
{
    s32 x;
    s32 y;
    s32 z;
    s16 r;

    z = *(u16 *)((u8 *)v + 0xA);
    x = *(u16 *)((u8 *)v + 0x2);
    y = *(u16 *)((u8 *)v + 0x6);
    D_80025FF4 = 1;
    r = func_800BCB04(x, y, z + 2);
    if (*(s16 *)((u8 *)v + 0xA) < r)
    {
        v->unk_08 += *(s32 *)(p + 0x48);
    }
    if ((*(s16 *)(p + 0x32) -= 4) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* A 0x211 spawner: seed the new node's mover from the parent `a''s, jitter each
   axis by a fresh rand() & 0x1F and the caller's own offset less 0x10, then
   park func_s59_800247E8 as the step and func_s59_80024598 as the draw, with
   `b' at the payload's +0x14, `d' at BOTH +0x32 and +0x34, a
   rand() + 0x10000 step at +0x48 and `c' at +0x8.

   The twin is the landed func_s16_80024488 in csrc/ovl_dng_s16.c, 72 words to
   this 78: the six extra are the second `sh' of `d' into +0x34 -- which rides
   in func_8004491C's delay slot, so it is written BEFORE that call -- and the
   five-word rand()/+0x10000 store that follows it. Landed on the FIRST
   COMPILE, so the twin's two measured facts were taken as given and are not
   re-measured here: the three stack offsets are `s16' (s32 is 66 words and 71
   differing in the sibling, the three $s2/$s3/$s5 save-restore pairs missing)
   and the subtraction wears its own `(s16)' cast to stop fold associating the
   `addiu -0x10' onto the running sum (74 words, 20 differing there).

   Both installed handlers are this unit's own and both are DEFINED ABOVE this
   point in the file, so no forward declaration was needed. 78 words,
   0 differing. */
void func_s59_800248A8(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *q;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s59_800247E8;
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x1F) + (s16)(dx - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x1F) + (s16)(dy - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x1F) + (s16)(dz - 0x10);
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x14) = b;
        *(s16 *)(q + 0x32) = d;
        *(s16 *)(q + 0x34) = d;
        func_8004491C(n, (u32)func_s59_80024598);
        *(s32 *)(q + 0x48) = rand() + 0x10000;
        *(u32 *)(q + 0x8) = c;
    }
}

/* The +0x10 step func_s59_80024F60 installs: spend one frame of the payload's
   +0x38 counter, raise the bank's D_80025FF4 busy flag, fade the slot's
   3-byte colour at +0xC..+0xE as `counter * 0x80 / limit', then re-place the
   mover at the payload's own (+0x44, +0x46, +0x48) seed offset by the
   halfwords of the UnkVec the spawner parked at +0x40, and kill the node when
   the counter runs out.  53 words, 0 differing, FIRST COMPILE.

   NO TWIN BY EITHER TOOL -- dngtwin finds no carved sibling at 0.70.  Its
   shape came from the CALLEE/OFFSET SET instead: the +0x38/+0x3A counter pair
   with the D_80025FF4 raise and the `+0x5A'-family `-0x2 / D_800814A0'
   teardown is this object's own idiom, and func_s59_80024C7C three listings
   up is the same tick with one colour.  The +0x40/+0x44/+0x46/+0x48 half of
   it is simply the inverse of func_s59_80024F60's spawn, which is why the two
   were written together.

   THREE FACTS, ALL MEASURED ON THIS BODY, all three HOLD:
     - The counter decrement is SPLIT AROUND the D_80025FF4 store -- `t = load
       - 1; D_80025FF4 = 1; store t;'.  As one statement with the flag store
       first, reading the counter back out of memory for the divide, it is
       53 words / 20 aligned differences.
     - The quotient goes through an `s32' local, so the whole
       `sll 16 / sra 9 / div / mflo' chain lands BEFORE the D_80025FF4 store.
       Written inline at the chained assignment it is 53 words / 17 aligned
       differences.
     - The +0x40 pointer goes through a local AND that local is assigned
       BEFORE the three seed stores.  Re-read inline at each of the three adds
       it is 56 words / 7 aligned differences; as a local assigned AFTER the
       seed stores it is 54 words / 2.
   The three colour bytes are one chained `[0] = [1] = [2]' assignment, as
   func_s59_80024C7C's comment already established for this unit. */
void func_s59_800249E0(u8 *p, UnkVec *v, UnkSlot *s)
{
    u16 t;
    s32 c;
    UnkVec *b;

    t = *(u16 *)(p + 0x38) - 1;
    c = ((s16)t << 7) / *(s16 *)(p + 0x3A);
    D_80025FF4 = 1;
    *(u16 *)(p + 0x38) = t;
    s->unk_0C[0] = s->unk_0C[1] = s->unk_0C[2] = c;
    b = *(UnkVec **)(p + 0x40);
    *(s16 *)((u8 *)v + 0x2) = *(u16 *)(p + 0x44);
    *(s16 *)((u8 *)v + 0x6) = *(u16 *)(p + 0x46);
    *(s16 *)((u8 *)v + 0xA) = *(u16 *)(p + 0x48);
    *(s16 *)((u8 *)v + 0x2) += *(u16 *)((u8 *)b + 0x2);
    *(s16 *)((u8 *)v + 0x6) += *(u16 *)((u8 *)b + 0x6);
    *(s16 *)((u8 *)v + 0xA) += *(u16 *)((u8 *)b + 0xA);
    if (*(s16 *)(p + 0x38) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The +0x10 step func_s59_80024DD4 installs: raise D_80025FF4, spend one
   frame of the payload's +0x38 counter, hand the slot to func_800478B8 every
   SECOND frame off a +0x3C two-frame divider, fade the slot's 3-byte colour
   as `counter * 0x80 / limit', integrate the mover's +0x14 velocity word into
   its +0x8 position, and kill the node when the counter runs out.
   57 words, 0 differing, FIRST COMPILE.

   The twin is the LANDED func_s67_800246F0 in csrc/ovl_dng_s67.c (52 words,
   dngtwin 0.807).  Same shape; the deltas are this bank's relink (counter
   +0x3A/+0x3C -> +0x38/+0x3A, flag D_800261B0 -> D_80025FF4), ONE integrated
   axis instead of two, and the five extra words of the +0x3C divider that
   gates the func_800478B8 call.  Frames differ (-0x20 with s0/s1/s2 here
   against the twin's -0x18), so only its shape facts were taken.

   THE FAMILY'S SPLIT-DECREMENT FACT INVERTS HERE, and that is the one thing
   worth carrying off this body.  func_s59_80024B98, func_s59_80024C7C and the
   s67 twin all need `t = load - 1; D_80025FF4 = 1; store t;'.  On THIS body
   the split is 57 words / 5 aligned differences and the plain
   `D_80025FF4 = 1; *(s16 *)(p + 0x38) -= 1;' is 0.  The discriminator is the
   intervening CALL: func_800478B8 clobbers the value, so the fade reloads the
   counter with `lh' (the twin's comment says exactly this) and nothing needs
   the pre-store value live across the global store -- so block 0 never wants
   the third scratch register the split buys.  A relinked copy that drops the
   call should expect the fact back.

   Measured and FREE, so neither is the reason this lands: writing the divider
   as `*(s16 *)(p + 0x3C) += 1;' then a separate `== 2' test rather than
   `if (++*(s16 *)(p + 0x3C) == 2)' (57 / 0), and the kill tail written as an
   early `return' with the teardown flat afterwards rather than as an `if'
   (57 / 0). */
void func_s59_80024AB4(u8 *p, UnkVec *v, UnkSlot *s)
{
    D_80025FF4 = 1;
    *(s16 *)(p + 0x38) -= 1;
    if (++*(s16 *)(p + 0x3C) == 2)
    {
        *(s16 *)(p + 0x3C) = 0;
        func_800478B8((u8 *)s);
    }
    s->unk_0C[0] = s->unk_0C[1] = s->unk_0C[2] = (*(s16 *)(p + 0x38) << 7) / *(s16 *)(p + 0x3A);
    *(s32 *)((u8 *)v + 0x8) += *(s32 *)((u8 *)v + 0x14);
    if (*(s16 *)(p + 0x38) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spend one frame of the payload's +0x3A life counter, raise the bank's
   D_80025FF4 busy flag, fade all four of the record's 3-byte colours at
   +0xC..+0x1A in proportion to the counter over its +0x3C limit, and kill the
   node when the counter runs out.  57 words, 0 differing.

   The twin is csrc/ovl_dungeon_800D50FC.c's func_800D6804, which is the same
   fade and the same `D_800814A0 |= 0x8000' kill tail over ONE colour at a
   +0x32/+0x34 counter pair; the relink moves the counter to +0x3A/+0x3C and
   turns the three scalings into a four-element loop.

   THE TWO SPELLING FACTS, both measured on this body:

     The counter decrement is SPLIT AROUND the D_80025FF4 store -- `t = load
     - 1; D_80025FF4 = 1; store t;' -- so the load is live across the global
     store and block 0 needs a THIRD scratch register, which takes $a0 away
     from `p' and is exactly the target's `addu $a2, $a0, $zero' parameter
     copy.  Written as one statement (`*(s16 *)(p + 0x3A) -= 1;' either side
     of the store) it is 56 words / 57 differing -- one word SHORT, with
     every register shifted down one.  Probably macro residue: a tick macro
     whose store the flag set interleaves.

     The loop is indexed, not a walking pointer: `p[i * 4 + 0xC]' keeps the
     giv's base at `p' with displacements 0xC..0x1E, where `c = p; ...;
     c += 4' lets gcc bias the base to `p + 0xE' and store at -2/-1/0
     (56 words / 57 differing).
   THIS COPY'S OWN CONSTANT: 0x3a -> 0x38 at all 6 sites, 0x3c -> 0x3a at all 3 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s59_80024B98(u8 *p)
{
    s32 i;
    u16 t;

    t = *(u16 *)(p + 0x38) - 1;
    D_80025FF4 = 1;
    *(u16 *)(p + 0x38) = t;
    for (i = 0; i < 4; i++)
    {
        p[i * 4 + 0xC] = p[i * 4 + 0x1C] * *(s16 *)(p + 0x38) / *(s16 *)(p + 0x3a);
        p[i * 4 + 0xD] = p[i * 4 + 0x1D] * *(s16 *)(p + 0x38) / *(s16 *)(p + 0x3a);
        p[i * 4 + 0xE] = p[i * 4 + 0x1E] * *(s16 *)(p + 0x38) / *(s16 *)(p + 0x3a);
    }
    if (*(s16 *)(p + 0x38) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The same tick as func_s67_80024834 for a record with ONE 3-byte colour: one
   frame off the +0x3A counter, D_80025FF4 raised, the slot's +0xC..+0xE faded
   as `counter * 0x80 / limit', the +0x8C byte counter stepped down, the
   slot's +0x1A spun by the payload's +0x40, and the node killed when the
   counter runs out.  37 words, 0 differing.

   Landed off func_s67_80024834's two facts, already in this unit: the
   decrement is split around the D_80025FF4 store for the same reason, and the
   three colour bytes are ONE CHAINED assignment written `[0] = [1] = [2]',
   which is what emits 0xE, 0xD, 0xC in that order.

   THIS BODY'S OWN FACT: the quotient also goes through a local, so the whole
   `sll 16 / sra 9 / div / mflo' chain lands BEFORE the D_80025FF4 store.
   With the division written inline at the three-byte assignment the store
   sinks no further than the second insn: 37 words, 18 differing.
   THIS COPY'S OWN CONSTANT: 0x3a -> 0x38 at all 3 sites, 0x3c -> 0x3a, 0x8c -> 0x84, 0x40 -> 0x44.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s59_80024C7C(u8 *p, UnkVec *v, UnkSlot *s)
{
    u16 t;
    s32 c;

    t = *(u16 *)(p + 0x38) - 1;
    c = ((s16)t << 7) / *(s16 *)(p + 0x3a);
    D_80025FF4 = 1;
    *(u16 *)(p + 0x38) = t;
    s->unk_0C[0] = s->unk_0C[1] = s->unk_0C[2] = c;
    p[0x84] -= 1;
    s->unk_1A += *(u16 *)(p + 0x44);
    if (*(s16 *)(p + 0x38) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* THIS FUNCTION OWNS jtbl_s59_80024008 (20 entries at rodata offset 0x0,
   .align 3).  The +0x10 step for a 20-frame flash: raise D_80025FF4, spend
   one frame of the payload's +0x38 counter, and look the remaining count up
   in a 20-way table that ramps the slot's 3-byte colour 0x20 / 0x35 / 0x50 /
   0x65 / 0x80 and back down twice; counts at or above 20 leave the colour
   alone.  Then spin the slot's +0x1A by 0x190 and kill the node when the
   counter runs out.  49 words, 0 differing, FIRST COMPILE.

   NO TWIN BY EITHER TOOL -- dngtwin finds no carved sibling at 0.70.  Found
   off the OFFSET SET: the +0x38/+0x3A counter, the D_80025FF4 raise and the
   `-0x2 / D_800814A0' teardown are this object's own idiom (see
   func_s59_80024C7C and func_s59_800249E0 above), and the table itself gave
   the arms.

   ARM ORDER, settled off the LISTING'S ENTRY ADDRESSES and then confirmed
   against the table tools/cdkgcc -S emits, NOT off a clean 0: arms emit in
   SOURCE order, the listing's five distinct targets ascend
   .Ls59_80024D5C < D64 < D6C < D74 < D7C, so the arms are written
   0x20, 0x35, 0x50, 0x65, 0x80 in that order.  The emitted table's entry
   classes then match the listing's position for position.
   AND A CORRECTION TO THE STANDING WARNING: on THIS body arm order is NOT
   invisible to the measuring tools.  Writing the five arms descending reads
   49 words / 4 differing under ovlverify and 4 aligned differences under
   aligndiff (while the emitted table reverses as expected), because each arm
   carries a DIFFERENT constant into the shared store's delay slot and those
   `addiu $v0, const' words are unmasked.  The warning holds only where the
   arms are indistinguishable in the text.

   THREE FACTS, ALL MEASURED ON THIS BODY, all three HOLD:
     - The chained colour store is written out IN EACH ARM and cross-jumping
       merges the five copies -- that is the `j .Ls59_80024D80' + constant in
       the delay slot the listing shows.  One shared store after the switch,
       reached through an `s32 c' seeded to -1 and a `c >= 0' guard, is
       52 words / 18 aligned differences.
     - The D_80025FF4 store sits BETWEEN `t = load - 1' and the store of `t'.
       Moved after the counter store it is 49 words / 2 aligned differences.
     - The decrement goes through the `u16 t' local that the switch then
       sign-extends.  Inlined as `switch ((s16)(*(s16 *)(p + 0x38) -= 1))' it
       is 49 words / 11 aligned differences -- the parameter copy
       `addu $a1, $a0, $zero' is the first row lost. */
void func_s59_80024D10(u8 *p, UnkVec *v, UnkSlot *s)
{
    u16 t;

    t = *(u16 *)(p + 0x38) - 1;
    D_80025FF4 = 1;
    *(u16 *)(p + 0x38) = t;
    switch ((s16)t)
    {
    case 0:
    case 9:
    case 10:
    case 19:
        s->unk_0C[0] = s->unk_0C[1] = s->unk_0C[2] = 0x20;
        break;
    case 1:
    case 8:
    case 11:
    case 18:
        s->unk_0C[0] = s->unk_0C[1] = s->unk_0C[2] = 0x35;
        break;
    case 2:
    case 7:
    case 12:
    case 17:
        s->unk_0C[0] = s->unk_0C[1] = s->unk_0C[2] = 0x50;
        break;
    case 3:
    case 6:
    case 13:
    case 16:
        s->unk_0C[0] = s->unk_0C[1] = s->unk_0C[2] = 0x65;
        break;
    case 4:
    case 5:
    case 14:
    case 15:
        s->unk_0C[0] = s->unk_0C[1] = s->unk_0C[2] = 0x80;
        break;
    }
    s->unk_1A += 0x190;
    if (*(s16 *)(p + 0x38) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* A 0x212 spawner: 0x14 at the payload's +0x38 and +0x3A, func_s59_80024AB4
   as the node's step, the renderable set to 0x20 with 0xC raised in its
   flags, the mover seeded from the caller's (x, y, z) then offset by the
   caller's own +0x7C/+0x7E/+0x80 halfwords and jittered by
   -0x7 + (rand() & 0xF) per channel, 0x20000 into the mover's +0x14, and then
   -- through a RE-READ of n->unk_0C -- 0x1000 as both scale halves, a
   0x80/0x80/0x80 over r+0xC..0xE, 0x7DCE at +0x12, 0x100 raised in the flags
   again and the record registered with func_8003DB94 against D_800DECF8.
   99 words, 0 differing, FIRST COMPILE.

   SIX PARAMETERS: +0x40/+0x44 of the incoming frame are the 5th and 6th, so
   the second and third are unread.

   The twin is the LANDED func_s58_80024F80 / func_s58_80024CA4 pair in
   csrc/ovl_dng_s58.c (183 words), which dngtwin cannot see at all -- it
   ranked csrc/ovl_dng_s46.c's func_s46_80024D34 (0.832) and three stubs.
   s46's body is the right skeleton and WRONG on the jitter; the s58 pair is
   the right jitter and three times the size.  The route that found it was the
   OFFSET-SET grep on `D_800DECF8' plus the `rand() & 0xF' shape, not a ratio.

   THREE FACTS, ALL MEASURED ON THIS BODY, all three HOLD, and they are the
   three the s58 pair priced:
     - The jitter is a `+=' with the constant INSIDE the addend:
       `+= (rand() & 0xF) + -0x7'.  Written
       `= *(u16 *)(m + k) + (rand() & 0xF) - 0x7' at all three channels,
       fold's `associate:' moves the constant to the rand value and it is
       99 words / 9 aligned differences.
     - `x'/`y'/`z' ARE `s16'.  As `s32' the frame shrinks to -0x28, the three
       mover stores lose their promotion copies, and it is 99 words /
       21 aligned differences.
     - The seed and the +0x7C base offset are SEPARATE statements.  Collapsed
       to one store per channel (`= x + *(u16 *)(p + 0x7C)') it is 94 words /
       15 aligned differences; the first three `sh' survive only because the
       intervening `lhu' of the base resets flow.c's `last_mem_set'.
   Measured and FREE, so the target does not constrain it: the order of
   `r->unk_10 = 0x20', `r->unk_14 |= 0xC' and `r->unk_06 = 0'.  All three
   orderings tried measure 99 words / 0 aligned differences, so the listing's
   0x14-load / 0x10 / 0x06 / 0x14-store sequence is sched1's and says nothing
   about the source -- unlike func_s46_80024D34, where a store had to sit
   between two raises of the SAME field. */
void func_s59_80024DD4(u8 *p, s32 c1, s32 c2, s16 x, s16 y, s16 z)
{
    UnkNode *n;
    u8 *q;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x38) = 0x14;
        *(s16 *)(q + 0x3A) = 0x14;
        n->unk_10 = (u32)func_s59_80024AB4;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_14 |= 0xC;
        r->unk_06 = 0;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = x;
        *(s16 *)((u8 *)m + 0x6) = y;
        *(s16 *)((u8 *)m + 0xA) = z;
        *(s16 *)((u8 *)m + 0x2) += *(u16 *)(p + 0x7C);
        *(s16 *)((u8 *)m + 0x6) += *(u16 *)(p + 0x7E);
        *(s16 *)((u8 *)m + 0xA) += *(u16 *)(p + 0x80);
        *(s16 *)((u8 *)m + 0x2) += (rand() & 0xF) + -0x7;
        *(s16 *)((u8 *)m + 0x6) += (rand() & 0xF) + -0x7;
        *(s16 *)((u8 *)m + 0xA) += (rand() & 0xF) + -0x7;
        *(s32 *)((u8 *)m + 0x14) = 0x20000;
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_12 = 0x7DCE;
        r->unk_14 |= 0x100;
        func_8003DB94(r, D_800DECF8, 0);
    }
}

/* A second 0x212 spawner, for the node func_s59_800249E0 steps: 0x1E at the
   payload's +0x38 and +0x3A, the caller's UnkVec pointer parked at +0x40 and
   its (x, y, z) at +0x44/+0x46/+0x48, func_s59_800249E0 as the step, the
   renderable set to 0x20 with 0xC raised and 6 at +0x6; then each of the
   three seed halfwords jittered by -0x10 + (rand() & 0x1F), copied into the
   mover and offset by the UnkVec's own halfwords; then 0x800 as both scale
   halves, 0x80/0x80/0x80 over r+0xC..0xE, the 12 bytes at D_80025FE8 copied
   to n+0xA2 and that address published as the slot's +0x8.
   115 words, 0 differing.

   NO TWIN BY EITHER TOOL at a useful ratio -- dngtwin's best is a 94-word
   stub at 0.746.  Landed off func_s59_80024DD4 above, which had just brought
   func_8003FC64, D_80045340 and the whole 0x212 skeleton into this unit, plus
   csrc/ovl_dng_s58.c's jitter fact; the 12-byte copy is the s58/s46
   `Name12' idiom (a struct of `u8 b[0xC]', alignment 1, so the move is the
   inline lwl/lwr + swl/swr pair rather than a loop).

   THE ONE FAULT, worth 5 differing / 3 aligned differences: `n->unk_10 =
   (u32)func_s59_800249E0;' must come AFTER the four +0x40..+0x48 stores, so
   that `sw $s5, 0x40($s0)' is emitted before the `jal func_8004491C' and the
   `sw $v0, 0x10($s2)' rides in its delay slot.  Written first -- which is
   where every other spawner in this family puts it, and where the listing's
   early `lui'/`addiu' of the handler address makes it look like it belongs --
   the two stores trade places.

   Measured and FREE, so neither is the reason this lands: the destination of
   the 12-byte copy spelled `q + 0x82' instead of `(u8 *)n + 0xA2'
   (115 / 0 -- the listing's `addiu $v0, $s2, 0xA2' is not evidence of which),
   and the jitter fact HOLDS as it does on func_s59_80024DD4: the
   constant-outside spelling of ONE channel is 115 words / 2 aligned
   differences. */
void func_s59_80024F60(u8 *p, UnkVec *v, s32 c, s16 x, s16 y, s16 z)
{
    UnkNode *n;
    u8 *q;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x38) = 0x1E;
        *(s16 *)(q + 0x3A) = 0x1E;
        *(s16 *)(q + 0x44) = x;
        *(s16 *)(q + 0x46) = y;
        *(s16 *)(q + 0x48) = z;
        *(s32 *)(q + 0x40) = (s32)v;
        n->unk_10 = (u32)func_s59_800249E0;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_14 |= 0xC;
        r->unk_06 = 6;
        m = n->unk_08;
        *(s16 *)(q + 0x44) += (rand() & 0x1F) + -0x10;
        *(s16 *)(q + 0x46) += (rand() & 0x1F) + -0x10;
        *(s16 *)(q + 0x48) += (rand() & 0x1F) + -0x10;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(q + 0x44);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(q + 0x46);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(q + 0x48);
        *(s16 *)((u8 *)m + 0x2) += *(u16 *)((u8 *)v + 0x2);
        *(s16 *)((u8 *)m + 0x6) += *(u16 *)((u8 *)v + 0x6);
        *(s16 *)((u8 *)m + 0xA) += *(u16 *)((u8 *)v + 0xA);
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0x800;
        r->unk_1C = 0x800;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        *(UnkDngS59Name12 *)((u8 *)n + 0xA2) = *(UnkDngS59Name12 *)D_80025FE8;
        r->unk_08 = (u32)((u8 *)n + 0xA2);
    }
}

/* The object's master state machine, a five-way jump table on the node's +0xA.
   State 0 arms the engine (0 at D_800814A8 + 0xF4, 0x14 at +0x96, 6 at
   D_80082E86), latches the party mover's (+0x2, +0x6, +0xA) into the payload's
   +0x7C..+0x81 seed, raises the bank's D_80025FF4 busy flag and falls through
   into state 1, which waits for bit 0x80 of **(p + 0x4) then books the
   0x21-frame cutscene, decrements the engine's +0xA6 counter, publishes p[0x8]
   at +0xA8 and picks a +0x34 variant from (D_800814A8 + 0x2A) >> 9.  State 2
   is the body: it spends the +0x2C countdown, fires the 0x300 (or 0x4300, when
   func_80053EF0(4) is 2) cue at frame 0x1A, throws two func_s59_800248A8
   sparks a frame while the counter is 5..23, raises the func_s59_80024D10 node
   at 0x18, and then -- below 6, skipping 3 -- maps the counter to a ring index
   k, writes the 2k'th and (2k+1)'th of the payload's eight +0x4C vertex
   triples as the mover/seed midpoint, raises one func_s59_80024544 quad per
   ring at 4, 2 and 1 and runs the three func_s59_80024DD4 lerps (6, 12 and 2
   steps over the triples at 0x4C/0x58, 0x58/0x64 and 0x64/0x70).  State 3
   sprays func_s59_80024F60 once per 15 units of D_800DDC40's entry for the
   engine's +0x60 actor and retires to state 4 on D_80082E94's 0x8000 or the
   counter running out; state 4 clears the engine, drops D_80083460's +0xA
   counter and raises the two 0x8000 kill bits.  934 words, 0 differing.

   THIS FUNCTION OWNS jtbl_s59_80024058: five words, cases 0..4, entries
   .Ls59_80025190 / .Ls59_800251EC / .Ls59_80025258 / .Ls59_80025E74 /
   .Ls59_80025F44, NO pad word, bound `sltiu 0x5'.  Case 0 FALLS THROUGH into
   case 1 and every other arm ends at the epilogue, so `default:' needs no
   label.  The arm order is NOT taken from the word count -- a standalone
   `tools/cdkgcc -S' of this unit emits five DISTINCT labels in ascending
   entry-address order whose first instructions are
   `lui %hi(D_800814A8)' / `lw 0x4(p)' / `lhu 0x2C(p)' / `lh 0x2C(p)' /
   `lui %hi(D_80025FF4)', one for one with the listing's five arm heads.  No
   label repeats, so no arms are shared.

   NO TWIN BY SHAPE: dngtwin finds no carved sibling at ratio 0.70 OR 0.50.
   dngcallee named the donor outright -- func_s58_80025D68 in
   csrc/ovl_dng_s58.c at 13 of 18 symbols with a clean gap to the next tier
   (11/18).  It is the same five-state jump-table driver at 772 words against
   this 934, which is why no length-banded metric could reach it.  Its frame
   differs (-0x58 with s0..s6 + ra against this -0x40 with s0..s5 + ra), so
   its ALLOCATION facts were not inherited; what carried was its SHAPE -- the
   +0x2C bump before the switch, the case 0 fall-through, the
   `(D_800814A8 + 0x2A) >> 9 & 7' variant pick, the
   `h = D_80082E80; func_8003DF74(*(u8 **)(h + 8), (UnkSlot *)h, ...)' probe
   and the whole of state 4 -- and ONE of its priced facts, re-measured here:

     - HOLDS: state 1's p accesses are ARRAY REFS (`((s16 *)p)[0x16]' and
       friends).  As plain casts the body is 935 words / 7 aligned rows.  The
       donor prices the same fact at 773/18 and 774/19 for two of them
       separately, so the DIRECTION transferred and the magnitude did not.
       Same mechanism it records: the two D_800814A8 reads have to hoist past
       p's stores, and a scalar-global MEM only clears a varying MEM that is
       in a struct.

   SEVEN FACTS MEASURED ON THIS BODY, all re-priced from the 0-differing
   version so none of them is a mid-fight figure:

   | spelling kept                                  | rejected form      | words/rows |
   | the three spawn blocks' four colour words      | 0x1C,0x24,0x28,0x20|  931 / 62  |
   |   stored ASCENDING 0x1C,0x20,0x24,0x28         |                    |            |
   | n->unk_10 written AFTER all four colour copies | before the last two|  934 / 48  |
   | an `s32 t' holding the FIRST +0x4C term, with  | no temp            |  934 / 13  |
   |   `d2 = D_80083780' after it                   |                    |            |
   | the three lerp arguments in s16 locals before   | inline in the call |  936 / 51  |
   |   the call (priced on loop 1 alone)            |                    |            |
   | the six D_80083780 reads as ARRAY REFS         | plain casts        |  931 / 33  |
   | D_800DDC40 INLINE in state 3's loop bound      | a `u8 *' local     |  935 / 33  |
   | `func_80053EF0(4) != 2 ? 0x300 : 0x4300'       | `== 2 ? 0x4300 :   |  934 /  3  |
   |                                                |   0x300'           |            |

   AND TWO MEASURED FREE, so neither is the reason this lands: the first
   +0x4C term as `(s16)*(u16 *)(p + 0x2E) / 2' rather than
   `*(s16 *)(p + 0x2E) / 2' (934 / 0 both -- cc1psx emits the same
   `lhu'/`sll'/`sra' pair either way because combine does not fold the
   sign_extend into an `lh' here, and I had asserted the cast was worth two
   words before the finished body disproved it), and the 5..23 range gate read
   through `u16' rather than `s16' (934 / 0 both -- the gate becomes
   `addiu -5'/`sltiu 0x13' and the load is `lhu' for either pointer type).

   THE COLOUR BLOCK IS FOUR SEPARATE `s32' ASSIGNMENTS, NOT A STRUCT COPY, AND
   THE WORD COUNT CANNOT TELL YOU THAT.  A 16-byte struct copy
   (`*(Blk16 *)(q + 0xC) = *(Blk16 *)(q + 0x1C)') is 934 words -- exactly
   right -- and 90 aligned rows, because move_by_pieces forces its address
   into a single-use pseudo, combine then folds `q = n + 0x20' into it, and
   all eight words address off `n' where the ROM addresses off `q'.  Four
   plain assignments keep the base.  This is the "right count, wrong
   structure" case: read the base register, not the length.

   WHY ASCENDING ORDER IS WORTH 62 ROWS, which is the one mechanism here worth
   carrying.  Every store through a pointer local calls cse's
   `invalidate_memory' with `nonscalar' set, and that drops every entry whose
   address varies (cse.c, `invalidate_memory'), so by the time the four copies
   are reached cse knows NONE of the four colour words and emits four real
   `lw's.  reload's own cse then rewrites a load into `addu rX, rY, $zero'
   whenever the value is still live in a hard register, and it is the SCHEDULED
   ORDER of those four loads that decides how many: ascending source order puts
   the 0x1C load first, so it copies out of the 0xE0E0E0 register and clobbers
   the 0x804020 one, leaving the 0x24/0x28 loads as real loads -- two copies
   and two `lw's, which is the ROM.  With 0x1C,0x24,0x28,0x20 the first load is
   0x20's, its destination IS the register already holding the value, the set
   is a no-op and gets deleted, and the block comes out one word short.
   A `reload_cse' no-op deletion is therefore a WORD, not a register row.

   AND THE `s32 t' IS A SCHEDULING FIX, NOT A VALUE.  It buys nothing
   arithmetically -- `t' has one use and costs no instruction -- but it gives
   `d2 = D_80083780' a later position, and that is what puts the symbol's
   `lui'/`addiu' pair after the first term's six words instead of before them.
   Measured in a 15-line standalone probe of just that block: the pair lands
   three slots BEFORE the `lhu' with no temp and six slots AFTER it with one,
   and the ROM has it six after.  The probe also says the fix is NOT the store
   order -- all 36 permutations of the two groups' three stores leave the pair
   before the `lhu' -- and not the read spelling, the local's type, or a
   `do { } while (0)' round the assignment.  The standalone figure and the
   unit's agree exactly, 934/0 both.

   The 2k'th and (2k+1)'th triples are addressed `p + k * 12' and
   `p + (k * 2 + 1) * 6'; written that way cc1psx computes `k * 2' once and
   shares it, which is the `sll'/`addu'/`sll 2' plus `addiu 1' the listing
   shows.  State 3's loop is `for (i = 0; i > -D_800DDC40[x[0x13]]; i -= 15)'
   -- the entry test folds to `beqz' on the byte, the byte is re-read each
   iteration because the call invalidates cse, and loop.c hoists the table's
   address into $s4 with the `addu $s4, $a0, $zero' copy that a local would
   not produce.  `rand' is the real name of func_80069EF8 and is spelled that
   way; the relocation audit is 66 for 66 against the listing with every
   `%hi'/`%lo' symbol and every `jal' target matching by multiset. */
void func_s59_8002512C(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *q;
    u8 *d0;
    u8 *d1;
    u8 *d2;
    u8 *h0;
    u8 *h1;
    u8 *h2;
    u8 *e1;
    u8 *e2;
    u8 *x;
    u8 *g;
    UnkVec *w;
    s32 i;
    s32 k;
    s16 ax;
    s16 ay;
    s16 az;
    s32 t;

    *(s16 *)(p + 0x38) = *(u16 *)(p + 0x38) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s32 *)(D_800814A8 + 0xF4) = 0;
        *(s16 *)(D_800814A8 + 0x96) = 0x14;
        D_80082E86 = 6;
        d0 = D_80083780;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0x7C) = *(u16 *)(d0 + 0x2);
        *(s16 *)(p + 0x7E) = *(u16 *)(d0 + 0x6);
        D_80025FF4 = 1;
        *(s16 *)(p + 0x80) = *(u16 *)(d0 + 0xA);
    case 1:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        ((s16 *)p)[0x16] = 0x21;
        *(s16 *)(D_800814A8 + 0xA6) = *(u16 *)(D_800814A8 + 0xA6) - 1;
        *(u8 *)(D_800814A8 + 0xA8) = p[0x8];
        ((s16 *)p)[0x1A] = (*(u16 *)(D_800814A8 + 0x2A) >> 9) & 7;
        ((s16 *)p)[0x5] = ((u16 *)p)[0x5] + 1;
        ((s16 *)p)[0x1C] = 0;
        break;
    case 2:
        if (--*(s16 *)(p + 0x2C) <= 0)
        {
            *(s16 *)(p + 0x2C) = 0x10;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            break;
        }
        if (*(s16 *)(p + 0x2C) == 0x1A)
        {
            func_800A56E0(func_80053EF0(4) != 2 ? 0x300 : 0x4300);
        }
        if (*(s16 *)(p + 0x2C) > 4 && *(s16 *)(p + 0x2C) < 0x18)
        {
            h0 = D_80082E80;
            if (func_8003DF74(*(u8 **)(h0 + 0x8), (UnkSlot *)h0, (s16 *)(p + 0x2E), 0) != 0)
            {
                for (i = 0; i < 2; i++)
                {
                    func_s59_800248A8((UnkNode *)(D_800814A8 - 0x20), 0, 0xE04040, (rand() & 0x3F) | 0x40,
                                      *(s16 *)(p + 0x2E), *(s16 *)(p + 0x30), *(s16 *)(p + 0x32));
                }
            }
        }
        if (*(s16 *)(p + 0x2C) == 0x18)
        {
            h1 = D_80082E80;
            if (func_8003DF74(*(u8 **)(h1 + 0x8), (UnkSlot *)h1, (s16 *)(p + 0x2E), 0) != 0)
            {
                n = func_8003FC64(0x212);
                if (n != NULL)
                {
                    q = (u8 *)n + 0x20;
                    *(s16 *)(q + 0x38) = 0x14;
                    *(s16 *)(q + 0x3A) = 0x14;
                    n->unk_10 = (u32)func_s59_80024D10;
                    func_8004491C(n, (u32)D_80045340);
                    r = (UnkSlot *)n->unk_0C;
                    r->unk_10 = 0x60;
                    r->unk_14 |= 0xC;
                    d1 = D_80083780;
                    m = n->unk_08;
                    *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0x2E) + *(u16 *)(d1 + 0x2);
                    *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0x30) + *(u16 *)(d1 + 0x6);
                    *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x32) + *(u16 *)(d1 + 0xA);
                    r = (UnkSlot *)n->unk_0C;
                    r->unk_1E = 0x1000;
                    r->unk_1C = 0x1000;
                    *((u8 *)r + 0xE) = 0x20;
                    *((u8 *)r + 0xD) = 0x20;
                    *((u8 *)r + 0xC) = 0x20;
                    *(UnkDngS59Name12 *)((u8 *)n + 0xA2) = *(UnkDngS59Name12 *)D_80025FD0;
                    r->unk_08 = (u32)((u8 *)n + 0xA2);
                }
            }
        }
        if (*(s16 *)(p + 0x2C) >= 0xA)
        {
            break;
        }
        if (*(s16 *)(p + 0x2C) >= 6)
        {
            break;
        }
        if (*(s16 *)(p + 0x2C) == 3)
        {
            break;
        }
        k = (*(s16 *)(p + 0x2C) == 4);
        if (*(s16 *)(p + 0x2C) == 2)
        {
            k = 2;
        }
        if (*(s16 *)(p + 0x2C) == 1)
        {
            k = 3;
        }
        h2 = D_80082E80;
        if (func_8003DF74(*(u8 **)(h2 + 0x8), (UnkSlot *)h2, (s16 *)(p + 0x2E), 0) != 0)
        {
            if (*(s16 *)(p + 0x2C) == 1)
            {
                *(s16 *)(p + 0x32) = -0xC;
            }
            e1 = p + k * 12;
            e2 = p + (k * 2 + 1) * 6;
            t = *(s16 *)(p + 0x2E) / 2;
            d2 = D_80083780;
            *(s16 *)(e1 + 0x4C) = t + (((s16 *)d2)[1] - *(s16 *)(p + 0x7C)) / 2;
            *(s16 *)(e1 + 0x4E) = *(s16 *)(p + 0x30) / 2 + (((s16 *)d2)[3] - *(s16 *)(p + 0x7E)) / 2;
            *(s16 *)(e1 + 0x50) = *(s16 *)(p + 0x32) / 2 + (((s16 *)d2)[5] - *(s16 *)(p + 0x80)) / 2;
            *(s16 *)(e2 + 0x4C) = *(s16 *)(p + 0x2E) / 4 + (((s16 *)d2)[1] - *(s16 *)(p + 0x7C)) / 2;
            *(s16 *)(e2 + 0x4E) = *(s16 *)(p + 0x30) / 4 + (((s16 *)d2)[3] - *(s16 *)(p + 0x7E)) / 2;
            *(s16 *)(e2 + 0x50) = *(s16 *)(p + 0x32) / 4 + (((s16 *)d2)[5] - *(s16 *)(p + 0x80)) / 2;
        }
        if (*(s16 *)(p + 0x2C) == 4)
        {
            n = func_8003FC64(0x212);
            if (n != NULL)
            {
                q = (u8 *)n + 0x20;
                *(s32 *)(q + 0x1C) = 0xE0E0E0;
                *(s32 *)(q + 0x20) = 0xE0E0E0;
                *(s32 *)(q + 0x24) = 0x804020;
                *(s32 *)(q + 0x28) = 0x804020;
                *(s16 *)(q + 0x38) = 0x14;
                *(s16 *)(q + 0x3A) = 0x14;
                *(s32 *)(q + 0xC) = *(s32 *)(q + 0x1C);
                *(s32 *)(q + 0x10) = *(s32 *)(q + 0x20);
                *(s32 *)(q + 0x14) = *(s32 *)(q + 0x24);
                *(s32 *)(q + 0x18) = *(s32 *)(q + 0x28);
                n->unk_10 = (u32)func_s59_80024B98;
                func_8004491C(n, (u32)func_s59_80024544);
                r = (UnkSlot *)n->unk_0C;
                r->unk_10 = 0x20;
                r->unk_14 |= 0xC;
                *(s16 *)(q + 0x4C) = *(u16 *)(p + 0x58);
                *(s16 *)(q + 0x4E) = *(u16 *)(p + 0x5A);
                *(s16 *)(q + 0x50) = *(u16 *)(p + 0x5C);
                *(s16 *)(q + 0x52) = *(u16 *)(p + 0x4C);
                *(s16 *)(q + 0x54) = *(u16 *)(p + 0x4E);
                *(s16 *)(q + 0x56) = *(u16 *)(p + 0x50);
                *(s16 *)(q + 0x58) = *(u16 *)(p + 0x5E);
                *(s16 *)(q + 0x5A) = *(u16 *)(p + 0x60);
                *(s16 *)(q + 0x5C) = *(u16 *)(p + 0x62);
                *(s16 *)(q + 0x5E) = *(u16 *)(p + 0x52);
                *(s16 *)(q + 0x60) = *(u16 *)(p + 0x54);
                *(s16 *)(q + 0x62) = *(u16 *)(p + 0x56);
                m = n->unk_08;
                *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0x7C);
                *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0x7E);
                *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x80);
                r = (UnkSlot *)n->unk_0C;
                r->unk_1E = 0x1000;
                r->unk_1C = 0x1000;
                *((u8 *)r + 0xE) = 0x80;
                *((u8 *)r + 0xD) = 0x80;
                *((u8 *)r + 0xC) = 0x80;
                *(UnkDngS59Name12 *)((u8 *)n + 0xA2) = *(UnkDngS59Name12 *)D_80025FDC;
                r->unk_08 = (u32)((u8 *)n + 0xA2);
            }
        }
        if (*(s16 *)(p + 0x2C) == 2)
        {
            n = func_8003FC64(0x212);
            if (n != NULL)
            {
                q = (u8 *)n + 0x20;
                *(s32 *)(q + 0x1C) = 0xE0E0E0;
                *(s32 *)(q + 0x20) = 0xE0E0E0;
                *(s32 *)(q + 0x24) = 0x804020;
                *(s32 *)(q + 0x28) = 0x804020;
                *(s16 *)(q + 0x38) = 0x14;
                *(s16 *)(q + 0x3A) = 0x14;
                *(s32 *)(q + 0xC) = *(s32 *)(q + 0x1C);
                *(s32 *)(q + 0x10) = *(s32 *)(q + 0x20);
                *(s32 *)(q + 0x14) = *(s32 *)(q + 0x24);
                *(s32 *)(q + 0x18) = *(s32 *)(q + 0x28);
                n->unk_10 = (u32)func_s59_80024B98;
                func_8004491C(n, (u32)func_s59_80024544);
                r = (UnkSlot *)n->unk_0C;
                r->unk_10 = 0x20;
                r->unk_14 |= 0xC;
                *(s16 *)(q + 0x4C) = *(u16 *)(p + 0x64);
                *(s16 *)(q + 0x4E) = *(u16 *)(p + 0x66);
                *(s16 *)(q + 0x50) = *(u16 *)(p + 0x68);
                *(s16 *)(q + 0x52) = *(u16 *)(p + 0x58);
                *(s16 *)(q + 0x54) = *(u16 *)(p + 0x5A);
                *(s16 *)(q + 0x56) = *(u16 *)(p + 0x5C);
                *(s16 *)(q + 0x58) = *(u16 *)(p + 0x6A);
                *(s16 *)(q + 0x5A) = *(u16 *)(p + 0x6C);
                *(s16 *)(q + 0x5C) = *(u16 *)(p + 0x6E);
                *(s16 *)(q + 0x5E) = *(u16 *)(p + 0x5E);
                *(s16 *)(q + 0x60) = *(u16 *)(p + 0x60);
                *(s16 *)(q + 0x62) = *(u16 *)(p + 0x62);
                m = n->unk_08;
                *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0x7C);
                *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0x7E);
                *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x80);
                r = (UnkSlot *)n->unk_0C;
                r->unk_1E = 0x1000;
                r->unk_1C = 0x1000;
                *((u8 *)r + 0xE) = 0x80;
                *((u8 *)r + 0xD) = 0x80;
                *((u8 *)r + 0xC) = 0x80;
                *(UnkDngS59Name12 *)((u8 *)n + 0xA2) = *(UnkDngS59Name12 *)D_80025FDC;
                r->unk_08 = (u32)((u8 *)n + 0xA2);
            }
        }
        if (*(s16 *)(p + 0x2C) == 1)
        {
            n = func_8003FC64(0x212);
            if (n != NULL)
            {
                q = (u8 *)n + 0x20;
                *(s32 *)(q + 0x1C) = 0xE0E0E0;
                *(s32 *)(q + 0x20) = 0xE0E0E0;
                *(s32 *)(q + 0x24) = 0x804020;
                *(s32 *)(q + 0x28) = 0x804020;
                *(s16 *)(q + 0x38) = 0x14;
                *(s16 *)(q + 0x3A) = 0x14;
                *(s32 *)(q + 0xC) = *(s32 *)(q + 0x1C);
                *(s32 *)(q + 0x10) = *(s32 *)(q + 0x20);
                *(s32 *)(q + 0x14) = *(s32 *)(q + 0x24);
                *(s32 *)(q + 0x18) = *(s32 *)(q + 0x28);
                n->unk_10 = (u32)func_s59_80024B98;
                func_8004491C(n, (u32)func_s59_80024544);
                r = (UnkSlot *)n->unk_0C;
                r->unk_10 = 0x20;
                r->unk_14 |= 0xC;
                *(s16 *)(q + 0x4C) = *(u16 *)(p + 0x70);
                *(s16 *)(q + 0x4E) = *(u16 *)(p + 0x72);
                *(s16 *)(q + 0x50) = *(u16 *)(p + 0x74);
                *(s16 *)(q + 0x52) = *(u16 *)(p + 0x64);
                *(s16 *)(q + 0x54) = *(u16 *)(p + 0x66);
                *(s16 *)(q + 0x56) = *(u16 *)(p + 0x68);
                *(s16 *)(q + 0x58) = *(u16 *)(p + 0x76);
                *(s16 *)(q + 0x5A) = *(u16 *)(p + 0x78);
                *(s16 *)(q + 0x5C) = *(u16 *)(p + 0x7A);
                *(s16 *)(q + 0x5E) = *(u16 *)(p + 0x6A);
                *(s16 *)(q + 0x60) = *(u16 *)(p + 0x6C);
                *(s16 *)(q + 0x62) = *(u16 *)(p + 0x6E);
                m = n->unk_08;
                *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0x7C);
                *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0x7E);
                *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x80);
                r = (UnkSlot *)n->unk_0C;
                r->unk_1E = 0x1000;
                r->unk_1C = 0x1000;
                *((u8 *)r + 0xE) = 0x80;
                *((u8 *)r + 0xD) = 0x80;
                *((u8 *)r + 0xC) = 0x80;
                *(UnkDngS59Name12 *)((u8 *)n + 0xA2) = *(UnkDngS59Name12 *)D_80025FDC;
                r->unk_08 = (u32)((u8 *)n + 0xA2);
            }
        }
        if (*(s16 *)(p + 0x2C) == 4)
        {
            for (i = 0; i < 6; i++)
            {
                ax = (*(u16 *)(p + 0x58) + (*(s16 *)(p + 0x4C) - *(s16 *)(p + 0x58)) * i / 6) * 2;
                ay = (*(u16 *)(p + 0x5A) + (*(s16 *)(p + 0x4E) - *(s16 *)(p + 0x5A)) * i / 6) * 2;
                az = (*(u16 *)(p + 0x5C) + (*(s16 *)(p + 0x50) - *(s16 *)(p + 0x5C)) * i / 6) * 2;
                func_s59_80024DD4(p, (s32)v, (s32)s, ax, ay, az);
            }
        }
        if (*(s16 *)(p + 0x2C) == 2)
        {
            for (i = 0; i < 12; i++)
            {
                ax = (*(u16 *)(p + 0x64) + (*(s16 *)(p + 0x58) - *(s16 *)(p + 0x64)) * i / 12) * 2;
                ay = (*(u16 *)(p + 0x66) + (*(s16 *)(p + 0x5A) - *(s16 *)(p + 0x66)) * i / 12) * 2;
                az = (*(u16 *)(p + 0x68) + (*(s16 *)(p + 0x5C) - *(s16 *)(p + 0x68)) * i / 12) * 2;
                func_s59_80024DD4(p, (s32)v, (s32)s, ax, ay, az);
            }
        }
        if (*(s16 *)(p + 0x2C) == 1)
        {
            for (i = 0; i < 2; i++)
            {
                ax = (*(u16 *)(p + 0x70) + (*(s16 *)(p + 0x64) - *(s16 *)(p + 0x70)) * i / 2) * 2;
                ay = (*(u16 *)(p + 0x72) + (*(s16 *)(p + 0x66) - *(s16 *)(p + 0x72)) * i / 2) * 2;
                az = (*(u16 *)(p + 0x74) + (*(s16 *)(p + 0x68) - *(s16 *)(p + 0x74)) * i / 2) * 2;
                func_s59_80024DD4(p, (s32)v, (s32)s, ax, ay, az);
            }
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x2C) == 0x10)
        {
            x = *(u8 **)(D_800814A8 + 0x60);
            if (x != NULL)
            {
                w = *(UnkVec **)(x - 0x18);
                for (i = 0; i > -D_800DDC40[x[0x13]]; i -= 15)
                {
                    func_s59_80024F60(p, w, (s32)s, 0, 0, i);
                }
            }
        }
        if ((D_80082E94 & 0x8000) != 0 || --*(s16 *)(p + 0x2C) < 0)
        {
            *(s16 *)(p + 0xA) = 4;
        }
        break;
    case 4:
        if (D_80025FF4 == 0)
        {
            g = (u8 *)&D_80083460;
            *(s32 *)(g + 0xC) = 0;
            D_80082E86 = 0;
            *(s16 *)(g + 0xA) = *(u16 *)(g + 0xA) - 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            D_80025FF4 = 0;
        }
        break;
    }
}
