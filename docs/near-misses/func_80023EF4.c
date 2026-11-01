#include "common.h"

/* 40 words, 9 DIFF -- one of the six identical arrayed primitive
   initialisers. The family, the shape and the full measurement table live in
   docs/near-misses/func_80022B48.c; this is the same source at base offset
   0x240 with 10 entries, and it comes out at exactly the same 9 differing
   words, so whatever unlocks one unlocks all six.

   What is left in all of them: the target keeps ONE pointer in $t3 for both
   the loop and the three trailing `shared` stores, with $a0 free as a scratch
   for the prims address. Here `c` stays in $a0 for the loop and `b` takes
   $t3 for the tail, so the prims address lands in $v1 and the `i = 0` init
   and the base copy are swapped around the `blez`. Eight more structural
   spellings measured 2026-11-01 on func_80022B48, all 40w/9: `n` copied to a
   local, `while` against `for`, `b` declared after `i`, `c->ents + i` for the
   ptrs store, `&c->shared[0]` for the unk_08 value, and (worse) a third base
   copy at 41w/41 and the two field stores swapped at 40w/10.

   Parked by hand; the live comment is in csrc/ovl_main_80020000.c, which also
   carries the declarations this needs. */

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
    u8 pad_000[0x240];
    u32 shared[3];
    u8 pad_00C[0x10];
    UnkCardPair *ptrs[10];
    UnkCardPair ents[10];
    u32 prims[10][4];
} UnkCardGrp;

void func_80023EF4(UnkCardGrp *c, s32 n)
{
    UnkCardGrp *b = c;
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
