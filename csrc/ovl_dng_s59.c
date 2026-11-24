#include "common.h"

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s59", func_s59_8002406C);

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
        sp->unk_C0 = RotTransPers((SVECTOR *)sp, (s32 *)&t->x0, &sp->unk_90, &sp->unk_94);
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s59", func_s59_800249E0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s59", func_s59_80024AB4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s59", func_s59_80024B98);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s59", func_s59_80024C7C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s59", func_s59_80024D10);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s59", func_s59_80024DD4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s59", func_s59_80024F60);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s59", func_s59_8002512C);
