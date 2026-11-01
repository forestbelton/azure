#include "common.h"

/* The arrayed primitive initialiser, 40 words, parked at 9 DIFF.
   docs/overlays.md has the family (six base offsets and counts, identical
   internal layout) and csrc/ovl_main_80020000.c carries the live comment.

   UPDATED 2026-11-01. Two permuter runs (130,470 candidates then 171,731,
   base 1305 -> 280 and 545 -> 125, NO match either time) plus a by-hand
   sweep got this from 13 DIFF to 9. The lever that did it is the SECOND base
   pointer: `UnkCardGrp8B0 *b = c;` used for the three trailing `shared`
   stores while the loop keeps using `c`.

   Why it matters: without it gcc keeps the base in $a0 for the whole
   function and the function is 39 words -- one short, with no `addu $t3,
   $a0, $zero` at word 0. With it, `b` takes $t3, the copy is emitted, and
   the loop body then matches the target instruction for instruction once the
   `e` local is dropped and both UnkCardPair fields are written through
   `c->ents[i]`:

     variant A (no `e` local, everything via c->ents[i]) is the target
     EXACTLY, shifted one word -- same four induction variables in the same
     roles ($t1 the 0x8E8 offset, $t0 the ptrs cursor, $a2 the stride-16
     cursor, $t2 the prims offset), and the two field stores on $a2 at
     displacements 0x8EC/0x8F0 rather than through a live `e`.

   What is left is nine words: the loop still bases its addressing on $a0
   (where `c` lives) instead of $t3, so the prims address goes to $v1 instead
   of $a0, and the `i = 0` init and the base copy are swapped around the
   `blez`. The target uses ONE base pointer in $t3 everywhere with $a0 free
   as a scratch; here `c` is in $a0 for the loop and `b` in $t3 for the tail.

   Measured, all with the same struct spelling:

     `e` local, ptrs first                        40w, 13 DIFF
     + unk_08 via c->ents[i]                      40w, 13 DIFF
     permuter's best (prims0 first, new_var)      40w, 13 DIFF
     no `e`, everything via c->ents[i]            39w, 40 DIFF
     no `e`, c->ents + i for ptrs                 39w, 40 DIFF
     no `e`, prims pointer local                  39w, 40 DIFF
     `e` only for the ptrs store                  39w, 40 DIFF
     + `b = c` used INSIDE the loop, c after      40w, 15 DIFF
     + `b = c` used everywhere                    39w, 40 DIFF
     + `b = c`, loop uses c, tail uses b (THIS)   40w,  9 DIFF
     ... with `i` declared before `b`             40w,  9 DIFF
     ... plus a prims pointer local               40w, 11 DIFF
     ... plus c->shared hoisted to a local        40w, 18 DIFF
     a ptrs cursor local declared first           40w, 24 DIFF
     c->shared hoisted, prims0 first              40w, 18 DIFF
     prims pointer local declared outside loop    39w, 39 DIFF

   Hoisting `c->shared` by hand is always worse -- gcc already hoists it to
   $t6 -- and so is giving the prims address its own local. This file is the
   permuter's base. */

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
    UnkCardGrp8B0 *b = c;
    s32 i;

    for (i = 0; i < n; i++)
    {
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = D_8002E5D8[0];
        c->prims[i][1] = D_8002E5D8[1];
        c->prims[i][2] = D_8002E5D8[2];
        c->prims[i][3] = D_8002E5D8[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    b->shared[0] = D_8002E5E8[0];
    b->shared[1] = D_8002E5E8[1];
    b->shared[2] = D_8002E5E8[2];
}
