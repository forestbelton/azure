#include "common.h"

/* The arrayed primitive initialiser, 40 words, parked at 13 DIFF.
   docs/overlays.md has the family (six base offsets and counts, identical
   internal layout) and csrc/ovl_main_80020000.c carries the live comment.

   This source is 40 words with the register allocation already matching --
   $a0 for the prims pointer, $t6 for the shared one, $t3 for the base copy,
   $a2/$t0/$t1/$t2 for the induction variables. What is left is SCHEDULE and
   two register names: the target issues the `ptrs[i] = &ents[i]` pair early
   (its address `addu` and `sw` at words 13-14, between prims[0]'s load and
   its store, which is what pushes prims[0]'s value into $v1 instead of $v0),
   while this source leaves them at 11-12 and keeps $v1 live as the `e`
   pointer for the unk_04/unk_08 stores, so those two go through $v1 with
   displacements 4 and 8 instead of through $a2 with 0x8EC and 0x8F0.

   UPDATED 2026-11-01 after a 40-minute permuter run (130,470 candidates at
   54/s, base score 1305 -> best 280, NO match). The search did not solve it
   but it found one real improvement, now folded in: writing the unk_08 store
   as `c->ents[i].unk_08` rather than through the `e` local keeps that store
   on the $a2 induction variable instead of making $v1 live as an `e`
   pointer. 13 DIFF -> 11. This file is the new permuter base.

   Measured, all with the same struct spelling:

     no `e` local, ptrs first          39w, one word short, no base copy
     no `e` local, prims first         40w, 19 DIFF, ptrs scheduled late
     `e` local, ptrs first              40w, 13 DIFF
     + unk_08 via c->ents[i] (THIS)    40w, 11 DIFF
     `e` local, prims first            40w, 19 DIFF
     `e` for ptrs + c->ents[] fields   39w  (gcc CSEs them back onto `e`)
     `q` local for the prims pointer   39w, 39 DIFF
     separate base pointers in locals  38w, 40 DIFF

   The `e` local is what forces the base copy into $t3 at all -- without it
   gcc keeps the base in $a0 and the function is one word short -- so it
   cannot simply be dropped. Getting the two field stores back onto the $a2
   induction variable while keeping the copy is the remaining problem. */

typedef struct
{
    s32 unk_00;
    s32 unk_04;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkCardPrim;

typedef struct
{
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
} UnkCardPrim12;

typedef struct
{
    s32 unk_00;
    UnkCardPrim *unk_04;
    UnkCardPrim12 *unk_08;
    s32 unk_0C;
} UnkCardPair;

typedef struct
{
    u8 pad_000[0x8B0];
    u32 shared[3];
    u8 pad_00C[0x10];
    UnkCardPair *ptrs[7];
    UnkCardPair ents[7];
    u32 prims[7][4];
} UnkCardGrp8B0;

void func_80022B48(UnkCardGrp8B0 *c, s32 n)
{
    s32 i;

    for (i = 0; i < n; i++)
    {
        UnkCardPair *e = &c->ents[i];

        c->ptrs[i] = e;
        c->prims[i][0] = D_8002E5D8[0];
        c->prims[i][1] = D_8002E5D8[1];
        c->prims[i][2] = D_8002E5D8[2];
        c->prims[i][3] = D_8002E5D8[3];
        e->unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002E5E8[0];
    c->shared[1] = D_8002E5E8[1];
    c->shared[2] = D_8002E5E8[2];
}
