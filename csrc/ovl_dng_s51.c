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
s32 func_80069EF8();
void func_s51_80024734();
s32 func_s51_800244E4(u8 *p, UnkVec *v);

void func_s51_800249A0(u8 *p);
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
extern s16 D_8002571C;

extern u8 D_80045340[];
extern u8 D_800256E0[];
UnkNode *func_8003FC64(s32);
void func_s51_80024104(u8 *p);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s51", func_s51_8002405C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s51", func_s51_80024104);

/* func_s51_80024104's own spawner, run as a step: allocate a 0x212 node, give
   it 0xB frames at the payload's +0x2C and the owner `a` at +0x7C, park
   func_s51_80024104 as its step, then clear the renderable -- 0x20 at +0x10,
   0 at +0x06, the 0xC bits in +0x14, a black colour and a zero scale -- seed
   its mover from the owner's name record and point the slot's +0x8 at the
   node's own +0x58, into which the 12-byte D_800256E0 record is copied.
   67 words, 0 differing, first compile.  The skeleton is func_800D4AB0
   (csrc/ovl_dungeon_800CABA8.c): same 0x212/func_8004491C/D_80045340 sequence
   and the same `n->unk_0C` RE-READ after the mover stores, which is what
   keeps the second `lw 0xC($s0)`.

   Two shapes differ from that twin and both are read off this listing.  The
   mover copy is THREE ALIGNED WORDS, not the twin's three halfwords: `m` and
   `s` are unrelated pointers gcc cannot prove disjoint, so each `sw` pins the
   next `lw` and the three load-delay slots stay `nop`.  And the 12-byte
   record copy is a struct whose type has ALIGNMENT 1 (`u8 b[0xC]`), which is
   what forces the `lwl`/`lwr` + `swl`/`swr` pairs instead of aligned `lw`s --
   the same fact UnkDungeonName12 carries in csrc/ovl_dungeon_decls.h.
   THIS COPY'S OWN CONSTANT: 0x2c -> 0x2a, 0x7c -> 0x60, 0x58 -> 0x92 at all 2 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s51_800243D8(u8 *a)
{
    UnkNode *n;
    u8 *p;
    u8 *s;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x2a) = 0xB;
        *(u8 **)(p + 0x60) = a;
        n->unk_10 = (u32)func_s51_80024104;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_06 = 0;
        r->unk_14 |= 0xC;
        s = *(u8 **)(a - 0x18);
        m = n->unk_08;
        m->unk_00 = *(s32 *)(s + 0x0);
        m->unk_04 = *(s32 *)(s + 0x4);
        m->unk_08 = *(s32 *)(s + 0x8);
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0;
        r->unk_1C = 0;
        *((u8 *)r + 0xE) = 0;
        *((u8 *)r + 0xD) = 0;
        *((u8 *)r + 0xC) = 0;
        *(UnkDngS58Name12 *)((u8 *)n + 0x92) = *(UnkDngS58Name12 *)D_800256E0;
        r->unk_08 = (u32)((u8 *)n + 0x92);
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
s32 func_s51_800244E4(u8 *p, UnkVec *v)
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s51", func_s51_80024734);

/* A 0x211 spawner: seed the new node's mover from the parent `a`'s, jitter
   each axis by a fresh func_80069EF8() & 0x1F and the caller's own offset
   less 0x10, then park func_s51_80024734 as the step and
   func_s51_800244E4 as the draw, with `b` at the payload's +0x14, `d` at
   +0x32 and `c` at +0x8.  72 words, 0 differing.  The twin is
   csrc/ovl_dungeon_800D50FC.c's func_800D6AD4 -- the same 0x211 spawner
   without the jitter.

   THE THREE STACK OFFSETS ARE `s16` AND THE SUBTRACTION WEARS AN `(s16)`
   CAST, and both halves of that are load-bearing:

   * `s32 dx/dy/dz` is 66 words and 71 differing.  An `s32` stack parameter
     keeps its incoming MEM as its rtl and is rematerialised with a
     `lw 0x48($sp)` at its single use; the promoted `s16` goes through a
     pseudo, which register allocation then parks in $s2/$s3/$s4.  The six
     missing words are those three registers' save/restore pairs.  (Same
     mechanism, opposite sign, as func_800D6AD4's own note.)
   * `s16` with a bare `(dx - 0x10)` is 74 words and 20 aligned differences:
     fold's associate step splits `(X + jitter) + (dx + -0x10)` into
     `(X + jitter + dx) + -0x10` and the `addiu` lands on the running sum
     instead of on the parameter.  The NOP_EXPR the cast leaves blocks that
     split, which is the whole of the remaining 20 rows -- with it the
     `andi` falls into the `lhu`'s load-delay slot and the destination's
     `lw 0x8($s0)` moves ahead of the first `addu` on its own.
   THIS COPY'S OWN CONSTANT: 0x1f -> 0xf at 2 of 3 sites, -0x10 -> -0x8 at 2 of 3 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s51_80024798(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s51_80024734;
        *(s16 *)((u8 *)n->unk_08 + 0x2) =
            *(u16 *)((u8 *)a->unk_08 + 0x2) + (func_80069EF8() & 0xf) + (s16)(dx - 0x8);
        *(s16 *)((u8 *)n->unk_08 + 0x6) =
            *(u16 *)((u8 *)a->unk_08 + 0x6) + (func_80069EF8() & 0xf) + (s16)(dy - 0x8);
        *(s16 *)((u8 *)n->unk_08 + 0xA) =
            *(u16 *)((u8 *)a->unk_08 + 0xA) + (func_80069EF8() & 0x1F) + (s16)(dz - 0x10);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = d;
        func_8004491C(n, (u32)func_s51_800244E4);
        *(u32 *)(p + 0x8) = c;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s51", func_s51_800248B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s51", func_s51_8002492C);

/* Spend 8 of the payload's +0x32 life counter, raise the engine's D_8002571C
   tick, and when the counter runs out set the 0x8000 kill bit in the node's
   -0x2 flags and in D_800814A0.  20 words, 0 differing.  The shape is
   func_800999D4 (csrc/ovl_dungeon.c) plus the D_8002571C store.

   THE DECREMENT MUST BE READ INTO AN `s16` LOCAL *BEFORE* THE D_8002571C
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
   THIS COPY'S OWN CONSTANT: 0x32 -> 0x2a at all 2 sites, -0x8 -> -0x1.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s51_800249A0(u8 *p)
{
    s16 t;

    t = *(s16 *)(p + 0x2a) - 1;
    D_8002571C = 1;
    *(s16 *)(p + 0x2a) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s51", func_s51_800249F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s51", func_s51_80024B38);
