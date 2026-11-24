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
void func_s38_80024B6C(u8 *p);
typedef struct UnkDngS58Blk32
{
    u8 b[0x20];
} UnkDngS58Blk32;
typedef struct UnkDngS58Rec
{
    s16 a;
    u16 b;
} UnkDngS58Rec;
typedef struct UnkDngS58Blk64
{
    u8 b[0x40];
} UnkDngS58Blk64;
typedef struct UnkDngS58Name12
{
    u8 b[0xC];
} UnkDngS58Name12;
extern s16 D_800259AC;

s32 func_s38_800248EC(u8 *p, UnkVec *v);

void func_s38_8002404C(u8 *, UnkVec *, UnkSlot *, s16);
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s38", func_s38_8002404C);

/* The engine's node walk, func_80046398 of csrc/unk_19434.c, relinked here
   with func_s38_8002404C as the visitor: it is variant A -- the slot's
   +0x06 as the visitor's fourth argument -- exactly as
   csrc/ovl_dungeon_800B6AC4.c's func_800BC054 has it, and xsibs scores it
   1.000 against slus_006.14's func_80046398. Nothing was changed.
   21 words, 0 differing, first compile. */
s32 func_s38_80024898(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_s38_8002404C(p, v, s, s->unk_06);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

/* One semi-transparent 1x1 TILE_1 and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad, with each node's colour word
   scaled by the payload's +0x32 life over 128.  148 words, 0 differing, first
   compile.  The twin is func_800D68F4 (csrc/ovl_dungeon_800D50FC.c), which is
   this walk with a constant colour; that function's long note on the shape --
   `addPrim` as a bitfield store, the two loop-hoisted masks in $s2/$s4,
   `setTile1` plus `setSemiTrans` folding to one `sb 0x6A` -- applies here
   unchanged.  The only relink is the scratchpad struct: this object's fields
   sit at +0x00/+0x02/+0x04, +0x18, +0x20, +0x90/+0x94 and +0xC0 where the
   resident original has them at +0x04/+0x06/+0x08, +0x1C, +0x24, +0xD0/+0xD4
   and +0x100, which is what UnkDng73E94SP already describes.  The twin's three
   dead colour `lbu`s are live here: they are `setRGB0`'s three operands, and
   the three `mult`/`bgez`/`addiu 0x7F`/`sra 7` runs are the signed `/ 128`. */
s32 func_s38_800248EC(u8 *p, UnkVec *v)
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
        sp->unk_C0 = RotTransPers((SVECTOR *)&sp->unk_00, (s32 *)&t->x0, &sp->unk_90, &sp->unk_94);
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            *(u32 *)&t->r0 = *(u32 *)(p + 0x8);
            setRGB0(t, t->r0 * *(s16 *)(p + 0x32) / 128, t->g0 * *(s16 *)(p + 0x32) / 128,
                    t->b0 * *(s16 *)(p + 0x32) / 128);
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s38", func_s38_80024B3C);

/* Spend 8 of the payload's +0x32 life counter, raise the engine's D_800259AC
   tick, and when the counter runs out set the 0x8000 kill bit in the node's
   -0x2 flags and in D_800814A0.  20 words, 0 differing.  The shape is
   func_800999D4 (csrc/ovl_dungeon.c) plus the D_800259AC store.

   THE DECREMENT MUST BE READ INTO AN `s16` LOCAL *BEFORE* THE D_800259AC
   STORE.  Every folded spelling -- `if ((*(s16 *)(p + 0x32) -= 8) <= 0)`
   with the store first, the early-return form, the counter behind a
   `s16 *` local, a `do { } while (0)` around the store, and the
   decrement/test split into two statements -- is 20 words and 11 differing,
   and the same local written AFTER the store is 11 too.  The eleven rows are
   one allocation: with the load after the store, the constant 1 is the only
   block-local quantity alive over $v0, so local-alloc hands it $v0 and
   global-alloc then gives `p` its incoming $a0; reading the counter first
   keeps $v0/$v1 busy across the constant's range, so the constant takes $a0
   and `p` is copied to $a1 by the target's entry `addu`.  That also buys the
   schedule: `addiu $a0, $zero, 1` fills the `lhu`'s load-delay slot, where
   the store-first forms spend a `nop` there.  Hoisting the result into an
   `s32` instead loses the `sll 16`/`bgtz` pair: 19 words, 19 differing.
   THIS COPY'S OWN CONSTANT: -0x8 -> -0xc.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s38_80024B6C(u8 *p)
{
    s16 t;

    t = *(s16 *)(p + 0x32) - 12;
    D_800259AC = 1;
    *(s16 *)(p + 0x32) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s38", func_s38_80024BBC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s38", func_s38_80024CE4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s38", func_s38_80024FA4);
