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
void func_s58_80024374(u8 *p);
s32 func_s58_80024124(u8 *p, UnkVec *v);

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
extern s16 D_800269B4;
extern u8 D_s58_80024004[];
extern u8 D_80083780[];
extern u8 D_800DECF8[];
extern u8 D_800DEC70[];
void func_8003DB94(void *, void *, s32);
void func_s58_80024874();
extern u32 D_800814A8;
extern u8 D_s58_80024024[];
void func_800B8FC8(u8 *, s32 *, s32 *, s32, s32);
extern u8 D_80045340[];
extern u8 D_s58_80026978[];
UnkNode *func_8003FC64(s32);
void func_s58_8002525C(u8 *p);

/* func_s58_80024F80's spawn stepped one frame: spend one of the payload's
   +0x2C frames, raise the D_800269B4 tick, fade the renderable's two low
   colour bytes to the remaining life over the original at +0x2E (times 128),
   bleed +0xE up and +0x6 down, and when the counter runs out release the
   owner's busy-table slot at +0x64 and raise the two 0x8000 kill bits.
   42 words, 0 differing, first compile.  The busy-table release is
   func_s58_8002525C's claim undone -- same `index * 2 + base + 0x64` integer
   sum, with the slot index kept at the payload's +0x50 and the owner at
   +0x7C.  `*((u8 *)s + 0xE) += 4` and `*(s16 *)((u8 *)s + 0x6) -= 2` each
   emit the UNSIGNED load (`lbu`/`lhu`) because a read-modify-write of a
   narrow lvalue does not need the sign bits, while the separate `>= -0x6`
   test is a signed `lh`. */
UnkNode *func_8003FD64(s32, void *);

void func_800478B8(u8 *);
s16 func_800A45D8(u16, u16, s32);
s16 func_800BCB04h(s32, s32, s16) __asm__("func_800BCB04");
void func_s58_80024AF8(u8 *p, UnkVec *b, u8 *c, s16 x, s16 y, s16 z);
void func_s58_800244E4(u8 *p, UnkVec *b, u8 *c);
extern u8 D_800DE870[];

void func_s58_80024CA4(u8 *p, UnkVec *vv, u8 *cc, s16 x, s16 y, s16 z);
void func_s58_80024F80(u8 *p, UnkVec *vv, u8 *cc, s16 x, s16 y, s16 z);
void func_s58_8002553C(u8 *a, UnkVec *b, u8 *c);
extern u8 D_80082E80[];
extern s16 D_80082E86;
extern u16 D_80082E94;
extern Unk80083462 D_80083460;
extern u8 D_80026990[];
extern u8 D_8002699C[];
extern u8 D_800269A8[];
s32 func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
void func_800A56E0(s32);
void func_800542BC(void);

void func_s58_8002407C(u8 *p, UnkVec *v, UnkSlot *s)
{
    s16 c;
    s32 k;

    c = *(u16 *)(p + 0x2C) - 1;
    k = c * 128 / *(s16 *)(p + 0x2E);
    D_800269B4 = 1;
    *(s16 *)(p + 0x2C) = c;
    *((u8 *)s + 0xE) += 4;
    *((u8 *)s + 0xD) = k;
    *((u8 *)s + 0xC) = k;
    if (*(s16 *)((u8 *)s + 0x6) >= -0x6)
    {
        *(s16 *)((u8 *)s + 0x6) -= 2;
    }
    if (*(s16 *)(p + 0x2C) <= 0)
    {
        *(s16 *)(*(s16 *)(p + 0x50) * 2 + *(s32 *)(p + 0x7C) + 0x64) = 0;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
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
s32 func_s58_80024124(u8 *p, UnkVec *v)
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

/* Spend 8 of the payload's +0x32 life counter, raise the engine's D_800269B4
   tick, and when the counter runs out set the 0x8000 kill bit in the node's
   -0x2 flags and in D_800814A0.  20 words, 0 differing.  The shape is
   func_800999D4 (csrc/ovl_dungeon.c) plus the D_800269B4 store.

   THE DECREMENT MUST BE READ INTO AN `s16` LOCAL *BEFORE* THE D_800269B4
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
   `s32` instead loses the `sll 16`/`bgtz` pair: 19 words, 19 differing. */
void func_s58_80024374(u8 *p)
{
    s16 t;

    t = *(s16 *)(p + 0x32) - 8;
    D_800269B4 = 1;
    *(s16 *)(p + 0x32) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* A 0x211 spawner: seed the new node's mover from the parent `a`'s, jitter
   each axis by a fresh rand() & 0x1F and the caller's own offset
   less 0x10, then park func_s58_80024374 as the step and
   func_s58_80024124 as the draw, with `b` at the payload's +0x14, `d` at
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
void func_s58_800243C4(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s58_80024374;
        *(s16 *)((u8 *)n->unk_08 + 0x2) =
            *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0xf) + (s16)(dx - 0x8);
        *(s16 *)((u8 *)n->unk_08 + 0x6) =
            *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0xf) + (s16)(dy - 0x8);
        *(s16 *)((u8 *)n->unk_08 + 0xA) =
            *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x1F) + (s16)(dz - 0x10);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = d;
        func_8004491C(n, (u32)func_s58_80024124);
        *(u32 *)(p + 0x8) = c;
    }
}

void func_s58_800244E4(u8 *p, UnkVec *b, u8 *c)
{
    s32 h;

    b->unk_00 += *(s32 *)(p + 0x8C);
    *(s32 *)(p + 0x8C) += *(s32 *)(p + 0x98);
    D_800269B4 = 1;
    if (func_800BCB04h(((u16 *)b)[1], ((u16 *)b)[3], ((s16 *)b)[5]) < 0x200 &&
        func_800A45D8(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA)) != 0)
    {
        b->unk_00 -= *(s32 *)(p + 0x8C);
        *(s32 *)(p + 0x8C) = 0;
        *(s32 *)(p + 0x98) = 0;
    }
    b->unk_04 += *(s32 *)(p + 0x90);
    *(s32 *)(p + 0x90) += *(s32 *)(p + 0x9C);
    if (func_800BCB04h(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA)) < 0x200 &&
        func_800A45D8(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA)) != 0)
    {
        b->unk_04 -= *(s32 *)(p + 0x90);
        *(s32 *)(p + 0x90) = 0;
        *(s32 *)(p + 0x9C) = 0;
    }
    b->unk_08 += *(s32 *)(p + 0x94);
    *(s32 *)(p + 0x94) += *(s32 *)(p + 0xA0);
    *(s32 *)(p + 0x8C) = *(s32 *)(p + 0x8C) * 96 / 100;
    *(s32 *)(p + 0x90) = *(s32 *)(p + 0x90) * 96 / 100;
    h = *(s16 *)((u8 *)b + 0xA);
    if (func_800BCB04h(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                       *(u16 *)((u8 *)b + 0xA) - 4) -
            0x10 <
        h)
    {
        *(s32 *)(p + 0x94) = 0;
        *(s16 *)((u8 *)b + 0xA) = func_800BCB04h(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                                                 *(u16 *)((u8 *)b + 0xA) - 4) -
                                  0x11;
        *(s16 *)((u8 *)b + 0x8) = 0;
        if (*(s16 *)(p + 0x2A) == 0)
        {
            *(s16 *)(p + 0x2A) = 1;
            *(s32 *)(p + 0x8C) = (((rand() & 0x7F) << 11) + 0xFFFE0000) * 2;
            *(s32 *)(p + 0x90) = (((rand() & 0x7F) << 11) + 0xFFFE0000) * 2;
        }
    }
    *((u8 *)c + 0xC) = *((u8 *)c + 0xD) = *((u8 *)c + 0xE) =
        (*(s16 *)(p + 0x2C) << 7) / *(s16 *)(p + 0x2E);
    if (++*(s16 *)(p + 0x34) == 3)
    {
        func_800478B8(c);
        *(s16 *)(p + 0x34) = 0;
    }
    *(u16 *)(c + 0x1C) += 0xC8;
    if (*(u16 *)(c + 0x1C) > 0x1000)
    {
        *(u16 *)(c + 0x1C) = 0x1000;
    }
    *(u16 *)(c + 0x1E) += 0xC8;
    if (*(u16 *)(c + 0x1E) > 0x1000)
    {
        *(u16 *)(c + 0x1E) = 0x1000;
    }
    if (--*(s16 *)(p + 0x2C) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

void func_s58_80024874(u8 *p, UnkVec *b, u8 *c)
{
    s16 t;
    s32 h;

    t = *(u16 *)(p + 0x2C) - 1;
    D_800269B4 = 1;
    *(s16 *)(p + 0x2C) = t;
    func_800478B8(c);
    b->unk_00 += *(s32 *)(p + 0x8C);
    *(s32 *)(p + 0x8C) += *(s32 *)(p + 0x98);
    if (func_800A45D8(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA)) != 0)
    {
        b->unk_00 -= *(s32 *)(p + 0x8C);
        *(s32 *)(p + 0x8C) = 0;
        *(s32 *)(p + 0x98) = 0;
    }
    b->unk_04 += *(s32 *)(p + 0x90);
    *(s32 *)(p + 0x90) += *(s32 *)(p + 0x9C);
    if (func_800A45D8(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA)) != 0)
    {
        b->unk_04 -= *(s32 *)(p + 0x90);
        *(s32 *)(p + 0x90) = 0;
        *(s32 *)(p + 0x9C) = 0;
    }
    b->unk_08 += *(s32 *)(p + 0x94);
    *(s32 *)(p + 0x94) += *(s32 *)(p + 0xA0);
    h = *(s16 *)((u8 *)b + 0xA);
    if (func_800BCB04h(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                       *(u16 *)((u8 *)b + 0xA) - 4) -
            0x10 <
        h)
    {
        *(s32 *)(p + 0x94) = 0;
        *(s32 *)(p + 0x90) = 0;
        *(s32 *)(p + 0x8C) = 0;
        *(s16 *)((u8 *)b + 0xA) = func_800BCB04h(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                                                 *(u16 *)((u8 *)b + 0xA) - 4) -
                                  0x11;
        *(s16 *)((u8 *)b + 0x8) = 0;
        *(s16 *)(p + 0x2C) = 0;
    }
    if (*(s16 *)(p + 0x2C) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* func_s65_80024104's spawn stepped one frame, the sibling of
   func_s65_8002405C two listings up and the same skeleton with no division:
   spend one of the payload's +0x2A frames, raise the D_800269B4 tick, clamp
   the renderable's three colour bytes down to 0x40 once they exceed it, then
   fade all three by 5 a frame until they reach 0, and raise the two 0x8000
   kill bits when the counter runs out (this one releases no busy-table slot).
   39 words, 0 differing.  No twin at any ratio; the shape came off the
   listing with func_s65_8002405C, landed an hour earlier in this file,
   supplying the idiom and the symbols.

   BOTH LOCALS ARE LOAD-BEARING AND NEITHER IS IN THE 8002405C TWIN'S
   COMMENT.  `c` must be its own `s16` local: written in place as
   `*(s16 *)(p + 0x2A) = *(u16 *)(p + 0x2A) - 1;` the body is still 39 words
   but 9 differing, because the tick's `%hi` then takes $a0, `p` keeps it
   instead of being copied to $a1, and the whole head reorders -- the
   target's opening `addu $a1, $a0, $zero` is the copy that costs.  And the
   fade's new value must go through the `u8` local `t`: spelled as the
   read-modify-write plus two same-base reloads
   (`*((u8 *)s + 0xE) -= 5; *((u8 *)s + 0xD) = *((u8 *)s + 0xE); ...`) cse
   forwards the stored value but regmove leaves a `move $v1, $v0` behind, for
   40 words / 18 differing. 
   THIS COPY'S OWN CONSTANT: 0x2a -> 0x2c at all 3 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s58_80024A5C(u8 *p, UnkVec *v, UnkSlot *s)
{
    s16 c;
    u8 t;

    c = *(u16 *)(p + 0x2c) - 1;
    D_800269B4 = 1;
    *(s16 *)(p + 0x2c) = c;
    if (*((u8 *)s + 0xC) > 0x40)
    {
        *((u8 *)s + 0xE) = 0x40;
        *((u8 *)s + 0xD) = 0x40;
        *((u8 *)s + 0xC) = 0x40;
    }
    if (*((u8 *)s + 0xC) != 0)
    {
        t = *((u8 *)s + 0xE) - 5;
        *((u8 *)s + 0xE) = t;
        *((u8 *)s + 0xD) = t;
        *((u8 *)s + 0xC) = t;
    }
    if (*(s16 *)(p + 0x2c) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

void func_s58_80024AF8(u8 *p, UnkVec *b, u8 *c, s16 x, s16 y, s16 z)
{
    UnkNode *n;
    u8 *q;
    u8 *b;
    UnkVec *m;
    UnkSlot *r;
    s16 c;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        c = (rand() & 7) + 0x20;
        *(s16 *)(q + 0x2C) = c;
        *(s16 *)(q + 0x2E) = c;
        n->unk_10 = (u32)func_s58_800244E4;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x60;
        r->unk_14 |= 0xC;
        r->unk_06 = 0;
        r->unk_14 |= 0x2;
        m = n->unk_08;
        b = D_80083780;
        *(s16 *)((u8 *)m + 0x2) = x;
        *(s16 *)((u8 *)m + 0x6) = y;
        *(s16 *)((u8 *)m + 0xA) = z;
        *(s16 *)((u8 *)m + 0x2) += *(u16 *)(b + 0x2);
        *(s16 *)((u8 *)m + 0x6) += *(u16 *)(b + 0x6);
        *(s16 *)((u8 *)m + 0xA) += *(u16 *)(b + 0xA);
        *(s16 *)((u8 *)m + 0x2) += (rand() & 0x1F) + -0x10;
        *(s16 *)((u8 *)m + 0x6) += (rand() & 0x1F) + -0x10;
        *(s16 *)((u8 *)m + 0xA) += (rand() & 0x1F) + -0x10;
        *(s32 *)(q + 0x94) = rand();
        *(s32 *)(q + 0xA0) = 0x1000;
        r = (UnkSlot *)n->unk_0C;
        r->unk_1C = 0xC00;
        r->unk_1E = 0xC00;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_12 = 0x7DCF;
        r->unk_14 |= 0x100;
        func_8003DB94(r, D_800DE870, 0);
    }
}

/* func_s58_80024F80 with three constants changed, and nothing else: this
   object has two of the burst spawner.  183 words, 0 differing -- the two
   listings' opcode streams are identical and differ in exactly four words,
   the 0x4 -> 0xF life, the 0x1000 -> 0x800 scale (twice) and the
   D_800DECF8 -> D_800DEC70 slot template.  Every spelling fact in
   func_s58_80024F80's comment above was re-measured here and holds. */
void func_s58_80024CA4(u8 *p, UnkVec *vv, u8 *cc, s16 x, s16 y, s16 z)
{
    UnkDngS58Rec rec[8];
    UnkNode *n;
    u8 *q;
    u8 *b;
    UnkVec *m;
    UnkSlot *r;

    *(UnkDngS58Blk32 *)rec = *(UnkDngS58Blk32 *)D_s58_80024004;
    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x2C) = 0xF;
        n->unk_10 = (u32)func_s58_80024874;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_14 |= 0xC;
        r->unk_1A = (rand() & 7) << 9;
        r->unk_06 = 0;
        m = n->unk_08;
        b = D_80083780;
        *(s16 *)((u8 *)m + 0x2) = x;
        *(s16 *)((u8 *)m + 0x6) = y;
        *(s16 *)((u8 *)m + 0xA) = z;
        *(s16 *)((u8 *)m + 0x2) += *(u16 *)(b + 0x2);
        *(s16 *)((u8 *)m + 0x6) += *(u16 *)(b + 0x6);
        *(s16 *)((u8 *)m + 0xA) += *(u16 *)(b + 0xA);
        *(s16 *)((u8 *)m + 0x2) += (rand() & 0x3F) + -0x20;
        *(s16 *)((u8 *)m + 0x6) += (rand() & 0x3F) + -0x20;
        *(s16 *)((u8 *)m + 0xA) += (rand() & 0x3F) + -0x40;
        *(s32 *)(q + 0x94) = 0xFFFA0000;
        *(s32 *)(q + 0xA0) = 0x20000;
        *(s32 *)(q + 0x8C) = rec[*(s16 *)(p + 0x26)].a << 18;
        *(s32 *)(q + 0x90) = rec[*(s16 *)(p + 0x26)].b << 18;
        *(s32 *)(q + 0x8C) += 0xFFF80000 + ((rand() & 0x3FFF) << 6);
        *(s32 *)(q + 0x90) += 0xFFF80000 + ((rand() & 0x3FFF) << 6);
        *(s32 *)(q + 0x94) += 0xFFF80000 + ((rand() & 0x3FFF) << 6);
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0x800;
        r->unk_1C = 0x800;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_12 = 0x7DCE;
        r->unk_14 |= 0x100;
        func_8003DB94(r, D_800DEC70, 0);
    }
}

/* The 0x212 burst spawner: copy the 8-entry D_s58_80024004 record table onto
   the stack, allocate a node with func_s58_80024874 as its step and 4 frames
   of life, seed its mover from the caller's (x, y, z) plus the three
   D_80083780 halfwords and a +-0x20/-0x40 jitter, set the three velocity
   words at +0x8C/+0x90/+0x94 and the +0xA0 from the table entry the owner's
   +0x26 selects plus a 14-bit jitter, then tint the renderable and fill its
   slot from the D_800DECF8 template.  183 words, 0 differing.

   SIX PARAMETERS: +0x68/+0x6C of the incoming frame are the 5th and 6th, so
   the second and third are unread.  `x`/`y`/`z` ARE `s16`: as `s32` the three
   mover stores lose their `addu $sX, $zero` promotion copies and the function
   is 183 words with 28 differing.

   The two fold facts, both measured here:

   * THE JITTER MUST BE A `+=` WITH THE CONSTANT INSIDE THE ADDEND.
     `*(u16 *)(m + 0x2) + (rand & 0x3F) - 0x20` associates the constant to the
     outside and emits `addu` before `addiu` where the target has
     `addiu $v1, $v1, -0x20` first: 183 words, 8 differing (3 transpositions
     plus the table rows below).  `+= (rand & 0x3F) + -0x20` and
     `(s16)(*(u16 *)(m + 0x2) - 0x20) + (rand & 0x3F)` both reach 0 for these
     rows.  Putting the subtraction on the left of the `+` so the field is
     loaded before the call costs the loaded value a callee-saved register:
     185 words, 101 differing.
   * THE STACK TABLE MUST BE AN ARRAY OF A 4-BYTE RECORD, NOT A `u8[0x20]`.
     Indexed as `*(s16 *)(tbl + *(s16 *)(p + 0x26) * 4)` the sum comes out
     `addu $v0, $v0, $s6` where the target has `addu $v0, $s6, $v0` -- base
     first -- and NONE of `&tbl[k * 4]`, `((s16 *)tbl)[k * 2]`,
     `tbl + 4 * k`, `k * 4 + tbl`, `k * 4 + (s32)tbl`, a `u8 *` local or an
     `s16 *` local moves it; two aligned rows either way.  `rec[k].a` and
     `rec[k].b` on a `{ s16 a; u16 b; }` array are 0.  Reading the second
     halfword through a cast instead of the field, as
     `*(u16 *)((u8 *)&rec[k] + 0x2)`, is 1 differing, so the FIELD is the
     lever and not the record type alone.

   The 0x20-byte copy is `u8 b[0x20]` so its alignment is 1 and the move is
   the inline `lwl`/`lwr` + `swl`/`swr` sequence (0x20 == 2 * MAX_MOVE_BYTES,
   so it is unrolled, not the loop func_s58_8002525C's 0x40 copy gets). */
void func_s58_80024F80(u8 *p, UnkVec *vv, u8 *cc, s16 x, s16 y, s16 z)
{
    UnkDngS58Rec rec[8];
    UnkNode *n;
    u8 *q;
    u8 *b;
    UnkVec *m;
    UnkSlot *r;

    *(UnkDngS58Blk32 *)rec = *(UnkDngS58Blk32 *)D_s58_80024004;
    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x2C) = 4;
        n->unk_10 = (u32)func_s58_80024874;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_14 |= 0xC;
        r->unk_1A = (rand() & 7) << 9;
        r->unk_06 = 0;
        m = n->unk_08;
        b = D_80083780;
        *(s16 *)((u8 *)m + 0x2) = x;
        *(s16 *)((u8 *)m + 0x6) = y;
        *(s16 *)((u8 *)m + 0xA) = z;
        *(s16 *)((u8 *)m + 0x2) += *(u16 *)(b + 0x2);
        *(s16 *)((u8 *)m + 0x6) += *(u16 *)(b + 0x6);
        *(s16 *)((u8 *)m + 0xA) += *(u16 *)(b + 0xA);
        *(s16 *)((u8 *)m + 0x2) += (rand() & 0x3F) + -0x20;
        *(s16 *)((u8 *)m + 0x6) += (rand() & 0x3F) + -0x20;
        *(s16 *)((u8 *)m + 0xA) += (rand() & 0x3F) + -0x40;
        *(s32 *)(q + 0x94) = 0xFFFA0000;
        *(s32 *)(q + 0xA0) = 0x20000;
        *(s32 *)(q + 0x8C) = rec[*(s16 *)(p + 0x26)].a << 18;
        *(s32 *)(q + 0x90) = rec[*(s16 *)(p + 0x26)].b << 18;
        *(s32 *)(q + 0x8C) += 0xFFF80000 + ((rand() & 0x3FFF) << 6);
        *(s32 *)(q + 0x90) += 0xFFF80000 + ((rand() & 0x3FFF) << 6);
        *(s32 *)(q + 0x94) += 0xFFF80000 + ((rand() & 0x3FFF) << 6);
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

/* The 0x212 spawner behind func_s58_8002553C's step, and the owner of this
   object's eight-slot busy table at +0x64.  Copy the 0x40-byte
   D_s58_80024024 record table onto the stack, raise the D_800269B4 tick, scan
   the eight slots for a free one, and on finding it mark the slot, place the
   record through func_800B8FC8, spawn a 0x212 node with func_s58_8002407C as
   its step, clear and tint its renderable, seed its mover from the player
   record and copy the same 12-byte D_s58_80026978 record func_s58_8002553C
   copies; then spend one frame of the owner's own +0x2C counter and raise the
   two 0x8000 kill bits when it runs out.  184 words, 0 differing.

   THE `i * 8` MUST BE COMPUTED INTO ITS OWN `s32` LOCAL BEFORE THE `buf +`:
   `buf + i * 8`, `(u8 *)&buf[i * 8]`, `(u8 *)buf + i * 8`, `i * 8 + buf` and
   `buf + (s32)i * 8` are all 184 words and 3 differing -- one aligned
   transposition, the `addiu $v0, $sp, 0x20` emitted ahead of the
   `sll $v1, $a0, 3` where the target has the shift first, which also swaps
   the pair's registers.  Written through a local the shift is emitted first
   and the pair matches; `k + buf` is identical to `buf + k`, so the operand
   order of the sum is NOT the lever, only whether the index is its own
   statement.  `(u8 *)(i * 8 + (s32)buf)` is 1 differing -- it fixes the
   order and loses the sum's own operand order instead.

   The 0x40-byte copy is a struct whose alignment is 1 (u8 b[0x40]), which is
   what makes expand_block_move emit the `andi 0x3` runtime alignment test and
   BOTH loops -- the `lwl`/`lwr` arm and the aligned `lw` arm.  The mover copy
   is three serialised aligned words for the same reason as in
   func_s58_8002553C. */
void func_s58_8002525C(u8 *p)
{
    SVECTOR w;
    u8 buf[0x40];
    UnkNode *n;
    u8 *q;
    u8 *e;
    u8 *s;
    UnkVec *m;
    UnkSlot *r;
    s16 i;
    s32 k;

    *(UnkDngS58Blk64 *)buf = *(UnkDngS58Blk64 *)D_s58_80024024;
    D_800269B4 = 1;
    for (i = 0; i < 8; i++)
    {
        if (*(s16 *)(i * 2 + (s32)p + 0x64) == 0)
        {
            break;
        }
    }
    if (i != 8)
    {
        *(s16 *)(i * 2 + (s32)p + 0x64) = 1;
        k = i * 8;
        e = buf + k;
        w.vx = *(u16 *)(e + 0x0) + ((s16) * (u16 *)(e + 0x4) >> 1);
        w.vy = *(u16 *)(e + 0x2) + 0x30;
        func_800B8FC8((u8 *)D_800814A8, (s32 *)e, (s32 *)&w, 1, 1);
        n = func_8003FC64(0x212);
        if (n != NULL)
        {
            q = (u8 *)n + 0x20;
            *(s16 *)(q + 0x2C) = 7;
            *(s16 *)(q + 0x2E) = 7;
            *(s16 *)(q + 0x50) = i;
            *(u8 **)(q + 0x7C) = p;
            n->unk_10 = (u32)func_s58_8002407C;
            func_8004491C(n, (u32)D_80045340);
            r = (UnkSlot *)n->unk_0C;
            r->unk_06 = 4;
            r->unk_14 &= ~0xC;
            s = *(u8 **)(D_800814A8 - 0x18);
            m = n->unk_08;
            m->unk_00 = *(s32 *)(s + 0x0);
            m->unk_04 = *(s32 *)(s + 0x4);
            m->unk_08 = *(s32 *)(s + 0x8);
            r = (UnkSlot *)n->unk_0C;
            r->unk_1E = 0x1000;
            r->unk_1C = 0x1000;
            *((u8 *)r + 0xE) = 0x80;
            *((u8 *)r + 0xD) = 0x80;
            *((u8 *)r + 0xC) = 0x80;
            *(UnkDngS58Name12 *)((u8 *)n + 0x58) = *(UnkDngS58Name12 *)D_s58_80026978;
            r->unk_08 = (u32)((u8 *)n + 0x58);
            q[0x40] = e[0] - 0x40;
            q[0x41] = e[2];
        }
    }
    if (--*(s16 *)(p + 0x2C) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* func_s58_8002525C's own spawner, run as a step: allocate a 0x212 node, give
   it 0xB frames at the payload's +0x2C and the owner `a` at +0x7C, park
   func_s58_8002525C as its step, then clear the renderable -- 0x20 at +0x10,
   0 at +0x06, the 0xC bits in +0x14, a black colour and a zero scale -- seed
   its mover from the owner's name record and point the slot's +0x8 at the
   node's own +0x58, into which the 12-byte D_s58_80026978 record is copied.
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
   the same fact UnkDungeonName12 carries in csrc/ovl_dungeon_decls.h. */
void func_s58_8002553C(u8 *a, UnkVec *b, u8 *c)
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
        *(s16 *)(p + 0x2C) = 0xB;
        *(u8 **)(p + 0x7C) = a;
        n->unk_10 = (u32)func_s58_8002525C;
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
        *(UnkDngS58Name12 *)((u8 *)n + 0x58) = *(UnkDngS58Name12 *)D_s58_80026978;
        r->unk_08 = (u32)((u8 *)n + 0x58);
    }
}

void func_s58_80025648(u8 *p, UnkVec *b, u8 *c)
{
    s16 t;

    *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) - 1;
    *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + 0x7C;
    *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + 0x54;
    t = *(u16 *)(p + 0x34) + 1;
    D_800269B4 = 1;
    *(s16 *)(p + 0x34) = t;
    if (t == 3)
    {
        if (++*(s16 *)(p + 0x50) < 7)
        {
            *(s16 *)(p + 0x34) = 0;
            p[0x40] += 0x18;
            if (p[0x40] == 0x60)
            {
                p[0x40] = 0;
                p[0x41] += 0x20;
            }
        }
    }
    if (*(s16 *)(p + 0x2C) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

void func_s58_80025724(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) - 1;
    *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) - 0x300;
    *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - 0x200;
    p[0x40] -= 0x18;
    if (p[0x40] == 0xE8)
    {
        p[0x40] = 0x18;
        p[0x41] -= 0x20;
    }
    if (*(s16 *)(p + 0x2C) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

void func_s58_800257B0(u8 *p, UnkVec *b, u8 *c)
{
    s32 h;
    s16 t;
    s16 u;
    s16 v;
    u8 *d;

    b->unk_00 += *(s32 *)(p + 0x8C);
    *(s32 *)(p + 0x8C) += *(s32 *)(p + 0x98);
    D_800269B4 = 1;
    if (func_800A45D8(((u16 *)b)[1], ((u16 *)b)[3], ((s16 *)b)[5]) != 0)
    {
        b->unk_00 -= *(s32 *)(p + 0x8C);
        *(s32 *)(p + 0x8C) = 0;
        *(s32 *)(p + 0x98) = 0;
    }
    b->unk_04 += *(s32 *)(p + 0x90);
    *(s32 *)(p + 0x90) += *(s32 *)(p + 0x9C);
    if (func_800A45D8(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA)) != 0)
    {
        b->unk_04 -= *(s32 *)(p + 0x90);
        *(s32 *)(p + 0x90) = 0;
        *(s32 *)(p + 0x9C) = 0;
    }
    b->unk_08 += *(s32 *)(p + 0x94);
    *(s32 *)(p + 0x94) += *(s32 *)(p + 0xA0);
    h = *(s16 *)((u8 *)b + 0xA);
    if (func_800BCB04h(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                       *(u16 *)((u8 *)b + 0xA) - 4) -
            0x10 <
        h)
    {
        *(s32 *)(p + 0x94) = 0;
        *(s32 *)(p + 0x90) = 0;
        *(s32 *)(p + 0x8C) = 0;
        *(s16 *)((u8 *)b + 0xA) = func_800BCB04h(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                                                 *(u16 *)((u8 *)b + 0xA) - 4) -
                                  0x11;
        *(s16 *)((u8 *)b + 0x8) = 0;
        *(s16 *)(p + 0x2C) = 0;
    }
    t = *(s16 *)(p + 0x50);
    *(s16 *)(p + 0x50) = t + 1;
    if (t >= 3)
    {
        u = *(u16 *)(c + 0x1C);
        v = *(u16 *)(c + 0x1E);
        u -= 0x100;
        if (u < 0)
        {
            u = 0;
        }
        v -= 0x100;
        if (v < 0)
        {
            v = 0;
        }
        *(s16 *)(c + 0x1C) = u;
        *(s16 *)(c + 0x1E) = v;
    }
    d = D_80083780;
    func_s58_80024AF8(p, b, c, (s16)(*(u16 *)((u8 *)b + 0x2) - *(u16 *)(d + 0x2)),
                      (s16)(*(u16 *)((u8 *)b + 0x6) - *(u16 *)(d + 0x6)),
                      (s16)(*(u16 *)((u8 *)b + 0xA) - *(u16 *)(d + 0xA)));
    if (--*(s16 *)(p + 0x2C) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

void func_s58_80025A80(u8 *p, UnkVec *b, u8 *c)
{
    s32 h;
    s16 t;
    s16 u;
    s16 v;
    u8 *d;

    b->unk_00 += *(s32 *)(p + 0x8C);
    *(s32 *)(p + 0x8C) += *(s32 *)(p + 0x98);
    D_800269B4 = 1;
    if (func_800A45D8(((u16 *)b)[1], ((u16 *)b)[3], ((s16 *)b)[5]) != 0)
    {
        b->unk_00 -= *(s32 *)(p + 0x8C);
        *(s32 *)(p + 0x8C) = 0;
        *(s32 *)(p + 0x98) = 0;
    }
    b->unk_04 += *(s32 *)(p + 0x90);
    *(s32 *)(p + 0x90) += *(s32 *)(p + 0x9C);
    if (func_800A45D8(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA)) != 0)
    {
        b->unk_04 -= *(s32 *)(p + 0x90);
        *(s32 *)(p + 0x90) = 0;
        *(s32 *)(p + 0x9C) = 0;
    }
    b->unk_08 += *(s32 *)(p + 0x94);
    *(s32 *)(p + 0x94) += *(s32 *)(p + 0xA0);
    h = *(s16 *)((u8 *)b + 0xA);
    if (func_800BCB04h(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                       *(u16 *)((u8 *)b + 0xA) - 4) -
            0x10 <
        h)
    {
        *(s32 *)(p + 0x94) = 0;
        *(s32 *)(p + 0x90) = 0;
        *(s32 *)(p + 0x8C) = 0;
        *(s16 *)((u8 *)b + 0xA) = func_800BCB04h(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                                                 *(u16 *)((u8 *)b + 0xA) - 4) -
                                  0x11;
        *(s16 *)((u8 *)b + 0x8) = 0;
        *(s16 *)(p + 0x2C) = 0;
    }
    t = *(s16 *)(p + 0x50);
    *(s16 *)(p + 0x50) = t + 1;
    if (t >= 3)
    {
        u = *(u16 *)(c + 0x1C);
        v = *(u16 *)(c + 0x1E);
        u -= 0x100;
        if (u < 0)
        {
            u = 0;
        }
        v -= 0x100;
        if (v < 0)
        {
            v = 0;
        }
        if (v == 0 || u == 0)
        {
            *(s16 *)(p + 0x2C) = 0;
        }
        *(s16 *)(c + 0x1C) = u;
        *(s16 *)(c + 0x1E) = v;
    }
    d = D_80083780;
    func_s58_80024AF8(p, b, c, (s16)(*(u16 *)((u8 *)b + 0x2) - *(u16 *)(d + 0x2)),
                      (s16)(*(u16 *)((u8 *)b + 0x6) - *(u16 *)(d + 0x6)),
                      (s16)(*(u16 *)((u8 *)b + 0xA) - *(u16 *)(d + 0xA)));
    if (--*(s16 *)(p + 0x2C) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The object's master state machine, a five-way jump table on the node's
   +0xA: state 0 arms the engine (0 at D_800814A8 + 0xF4, 0x21 at +0x96,
   6 at D_80082E86) and falls through into state 1, which waits for bit 0x80
   of **(p + 0x4), then books the 0x2D-frame cutscene, decrements the engine's
   +0xA6 counter, publishes p[0x8] at +0xA8, picks a +0x26 variant from
   (D_800814A8 + 0x2A) >> 9, spawns func_s58_8002553C and starts the 0x300
   cue; state 2 projects the caster's hand through func_8003DF74 and, on the
   way down a 0x27-frame countdown, spawns the func_s58_80025648 node at 0x26,
   the func_s58_80024A5C node under 6, two func_s58_80024AF8 bursts under 7
   and two more at the midpoint of +0xC..+0x11 and +0x12..+0x17 under 3,
   latching the mover into +0x12/+0x14/+0x16 each frame; state 3 forks on the
   engine's +0x60 flag -- set, it fires four func_s58_80024CA4 and four
   func_s58_80024F80 bursts and raises the two trail emitters
   (func_s58_800257B0, func_s58_80025A80) at 0x10; clear, it raises the
   func_s58_80025724 node at 0x10 and two more func_s58_80024AF8 bursts; and
   state 4 clears the engine, drops D_80083460's +0xA counter and retires the
   node.  772 words, 0 differing.

   THIS FUNCTION OWNS jtbl_s58_80024068: five words, cases 0..4, entries
   .Ls58_80025E58 / .Ls58_80025E90 / .Ls58_80025F18 / .Ls58_800262CC /
   .Ls58_800268F4, no padding, bound `sltiu 0x5'.  Case 0 FALLS THROUGH into
   case 1 and every other arm ends at the epilogue, so `default:' needs no
   label.

   EVERY SYMBOL-ADDRESS BASE NEEDS ITS OWN LOCAL, ONE PER BLOCK.  This is the
   whole of the allocation work on this body and it is worth 99 aligned rows:

   | D_80083780 and D_80082E80 written as              | words | aligned |
   | one local per spawn block (d1..d5) and per        |       |         |
   |   func_8003DF74 site (h1, h2)                     |  772  |   13    |
   | one shared `d' local and one shared `h'           |  778  |  122    |
   | the bare symbol at every site                     |  775  |   52    |

   A local used in five blocks is a GLOBAL allocno, so global.c assigns it
   after local-alloc has already taken $a0 for the block-local quantities, and
   it lands in $a1 where the target has $a0 -- every mover store in all five
   blocks then names the wrong register, which is the 122.  A local used in
   ONE block is a local-alloc quantity and gets $a0.  The bare symbol fixes
   the register and loses the addressing instead: combine folds the base to
   `D_80083780 + 2' and the other two reads become 4 and 8 off it, 5 rows per
   site.

   THE LAST 13 ROWS WERE A STATEMENT ORDER INSIDE STATE 1, and the sweep that
   found it is the point: the body sat at 772 words / 13 aligned through five
   SET changes (a copy of `p' into each of three different existing locals,
   an `s16' temp on the +0xA bump, an `s16' temp on the +0x26 value) and six
   spellings (each of the five p-accesses cast-vs-array-ref, the gate
   likewise) with EVERY ONE measuring exactly 13.  A full 720-point sweep of
   the six statements' order has 0 at
   `+0x20, +0xA6, +0xA8, +0x26, +0xA, +0x2C' -- the +0x26 store BEFORE the
   +0xA bump -- 2 at `+0x20, +0xA6, +0x2C, +0xA8, +0x26, +0xA', 5 at two
   others, and 13 at the order that reads naturally.  So a flat sweep over
   spellings is not evidence that order is reachable; it is evidence that the
   sweep was over the wrong axis.

   The 13 rows were one `addu $a0, $s4, $zero' in the wrong slot.  Worth
   knowing for next time: a parameter's base register can be RENAMED after
   allocation.  reload's own cse replaces $s4 with $a0 in every MEM that
   follows the argument copy, so where sched1 puts that copy decides how many
   of state 1's accesses are addressed off $a0 -- the rtl at `.regmove' has
   `(reg/v:SI 80)' in all seven of them and the assembly has two different
   registers.  Do not read a base-register residue in a block that ends in a
   call as an allocation fault; it is a schedule.

   STATE 1'S FIVE p-ACCESSES ARE ARRAY REFS (`((s16 *)p)[0x10]' and friends).
   Measured: the +0x20 store as a cast deref is 773 words / 18 and the +0xA
   bump as a cast deref is 774 / 19, while the +0x2C store, the +0x26 store,
   the `p[0x8]' read and the gate are free either way.  Same MEM_IN_STRUCT_P
   mechanism as func_s58_800244E4's gate: the two D_800814A8 reads have to
   hoist past p's stores, and a scalar-global MEM only clears a varying MEM
   that is in a struct.

   The +0x20/+0x2E/+0x4C stores are written off `n' directly where the two
   trail-emitter spawns use a `q = (u8 *)n + 0x20' local, because that is how
   the listing addresses them (`sh $v0, 0x4C($s3)' against
   `sh $v0, 0x2C($s1)').  The stack record table is func_s58_80024F80's
   `UnkDngS58Rec rec[8]' copied from D_s58_80024004, indexed `rec[k].a' and
   `rec[k].b' -- that body's note prices the field access against the cast at
   1 row and the `u8[0x20]' type at 2, and both hold here. */
void func_s58_80025D68(u8 *p, UnkVec *b, u8 *c)
{
    UnkDngS58Rec rec[8];
    UnkNode *n;
    u8 *q;
    u8 *d1;
    u8 *d2;
    u8 *d3;
    u8 *d4;
    u8 *d5;
    u8 *g;
    u8 *h1;
    u8 *h2;
    UnkVec *m;
    UnkSlot *r;
    s32 i;
    s16 e;
    s16 t;

    *(UnkDngS58Blk32 *)rec = *(UnkDngS58Blk32 *)D_s58_80024004;
    *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s32 *)(D_800814A8 + 0xF4) = 0;
        *(s16 *)(D_800814A8 + 0x96) = 0x21;
        D_80082E86 = 6;
        t = *(u16 *)(p + 0xA) + 1;
        D_800269B4 = 1;
        *(s16 *)(p + 0xA) = t;
    case 1:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        ((s16 *)p)[0x10] = 0x2D;
        *(s16 *)(D_800814A8 + 0xA6) = *(u16 *)(D_800814A8 + 0xA6) - 1;
        *(u8 *)(D_800814A8 + 0xA8) = p[0x8];
        ((s16 *)p)[0x13] = (*(u16 *)(D_800814A8 + 0x2A) >> 9) & 7;
        ((s16 *)p)[0x5] = ((u16 *)p)[0x5] + 1;
        ((s16 *)p)[0x16] = 0;
        func_s58_8002553C(p, b, c);
        func_800A56E0(0x300);
        func_800542BC();
        break;
    case 2:
        if (*(s16 *)(p + 0x20) < 0x27)
        {
            h1 = D_80082E80;
            if (func_8003DF74(*(u8 **)(h1 + 0x8), (UnkSlot *)h1, (s16 *)(p + 0xC), 0) != 0)
            {
                if (*(s16 *)(p + 0x20) == 0x26)
                {
                    n = func_8003FC64(0x212);
                    if (n != NULL)
                    {
                        *(s16 *)((u8 *)n + 0x4C) = 0x21;
                        n->unk_10 = (u32)func_s58_80025648;
                        func_8004491C(n, (u32)D_80045340);
                        r = (UnkSlot *)n->unk_0C;
                        r->unk_10 = 0x20;
                        r->unk_14 |= 0xC;
                        r->unk_06 = -4;
                        d1 = D_80083780;
                        m = n->unk_08;
                        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0xC);
                        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0xE);
                        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x10);
                        *(s16 *)((u8 *)m + 0x2) += *(u16 *)(d1 + 0x2);
                        *(s16 *)((u8 *)m + 0x6) += *(u16 *)(d1 + 0x6);
                        *(s16 *)((u8 *)m + 0xA) += *(u16 *)(d1 + 0xA);
                        r->unk_1C = 0x900;
                        r->unk_1E = 0x604;
                        *((u8 *)r + 0xE) = 0x80;
                        *((u8 *)r + 0xD) = 0x80;
                        *((u8 *)r + 0xC) = 0x80;
                        *(UnkDngS58Name12 *)((u8 *)n + 0x58) = *(UnkDngS58Name12 *)D_80026990;
                        r->unk_08 = (u32)((u8 *)n + 0x58);
                    }
                }
                if (*(s16 *)(p + 0x20) < 6)
                {
                    n = func_8003FC64(0x212);
                    if (n != NULL)
                    {
                        *(s16 *)((u8 *)n + 0x4C) = 1;
                        n->unk_10 = (u32)func_s58_80024A5C;
                        func_8004491C(n, (u32)D_80045340);
                        r = (UnkSlot *)n->unk_0C;
                        r->unk_10 = 0x20;
                        r->unk_14 |= 0xC;
                        r->unk_06 = -4;
                        d2 = D_80083780;
                        m = n->unk_08;
                        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0xC);
                        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0xE);
                        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x10);
                        *(s16 *)((u8 *)m + 0x2) += *(u16 *)(d2 + 0x2);
                        *(s16 *)((u8 *)m + 0x6) += *(u16 *)(d2 + 0x6);
                        *(s16 *)((u8 *)m + 0xA) += *(u16 *)(d2 + 0xA);
                        r->unk_1C = 0x1800;
                        r->unk_1E = 0x1000;
                        *((u8 *)r + 0xE) = 0x80;
                        *((u8 *)r + 0xD) = 0x80;
                        *((u8 *)r + 0xC) = 0x80;
                        *(UnkDngS58Name12 *)((u8 *)n + 0x58) = *(UnkDngS58Name12 *)D_8002699C;
                        r->unk_08 = (u32)((u8 *)n + 0x58);
                    }
                }
                if (*(s16 *)(p + 0x20) < 7)
                {
                    for (i = 0; i < 2; i++)
                    {
                        func_s58_80024AF8(p, b, c, *(s16 *)(p + 0xC), *(s16 *)(p + 0xE),
                                          *(s16 *)(p + 0x10));
                    }
                }
                if (*(s16 *)(p + 0x20) < 3)
                {
                    *(s16 *)(p + 0x18) = (*(s16 *)(p + 0xC) + *(s16 *)(p + 0x12)) / 2;
                    *(s16 *)(p + 0x1A) = (*(s16 *)(p + 0xE) + *(s16 *)(p + 0x14)) / 2;
                    *(s16 *)(p + 0x1C) = (*(s16 *)(p + 0x10) + *(s16 *)(p + 0x16)) / 2;
                    for (i = 0; i < 2; i++)
                    {
                        func_s58_80024AF8(p, b, c, *(s16 *)(p + 0x18), *(s16 *)(p + 0x1A),
                                          *(s16 *)(p + 0x1C));
                    }
                }
                *(s16 *)(p + 0x12) = *(u16 *)(p + 0xC);
                *(s16 *)(p + 0x14) = *(u16 *)(p + 0xE);
                *(s16 *)(p + 0x16) = *(u16 *)(p + 0x10);
            }
        }
        if (--*(s16 *)(p + 0x20) <= 0)
        {
            *(s16 *)(p + 0x20) = 0x10;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 3:
        if (*(s32 *)(D_800814A8 + 0x60) != 0)
        {
            if (*(s16 *)(p + 0x20) >= 0xD)
            {
                h2 = D_80082E80;
                if (func_8003DF74(*(u8 **)(h2 + 0x8), (UnkSlot *)h2, (s16 *)(p + 0xC), 0) != 0)
                {
                    for (i = 0; i < 4; i++)
                    {
                        func_s58_80024CA4(p, b, c, *(s16 *)(p + 0xC), *(s16 *)(p + 0xE),
                                          *(s16 *)(p + 0x10));
                    }
                    for (i = 0; i < 4; i++)
                    {
                        func_s58_80024F80(p, b, c, *(s16 *)(p + 0xC), *(s16 *)(p + 0xE),
                                          *(s16 *)(p + 0x10));
                    }
                    e = *(s16 *)(p + 0x20);
                    if (e == 0x10)
                    {
                        n = func_8003FC64(0x212);
                        if (n != NULL)
                        {
                            q = (u8 *)n + 0x20;
                            *(s16 *)(q + 0x2C) = 0x1E;
                            *(s16 *)(q + 0x2E) = 0x1E;
                            n->unk_10 = (u32)func_s58_800257B0;
                            func_8004491C(n, (u32)D_80045340);
                            r = (UnkSlot *)n->unk_0C;
                            r->unk_10 = 0x20;
                            r->unk_14 |= 0xC;
                            *(s32 *)(q + 0x94) = 0xFFF20000;
                            *(s32 *)(q + 0xA0) = 0x20000;
                            *(s32 *)(q + 0x8C) = rec[*(s16 *)(p + 0x26)].a << 19;
                            *(s32 *)(q + 0x90) = rec[*(s16 *)(p + 0x26)].b << 19;
                            *(s32 *)(q + 0x8C) += 0xFFFC0000 + ((rand() & 0x1FFF) << 6);
                            *(s32 *)(q + 0x90) += 0xFFFC0000 + ((rand() & 0x1FFF) << 6);
                            *(s32 *)(q + 0x94) += 0xFFFC0000 + ((rand() & 0x1FFF) << 6);
                            d3 = D_80083780;
                            m = n->unk_08;
                            *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0xC);
                            *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0xE);
                            *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x10) - 0x10;
                            *(s16 *)((u8 *)m + 0x2) += *(u16 *)(d3 + 0x2);
                            *(s16 *)((u8 *)m + 0x6) += *(u16 *)(d3 + 0x6);
                            *(s16 *)((u8 *)m + 0xA) += *(u16 *)(d3 + 0xA);
                            r->unk_1E = 0x1000;
                            r->unk_1C = 0x1000;
                            *((u8 *)r + 0xE) = 0x80;
                            *((u8 *)r + 0xD) = 0x80;
                            *((u8 *)r + 0xC) = 0x80;
                            *(UnkDngS58Name12 *)((u8 *)n + 0x58) = *(UnkDngS58Name12 *)D_800269A8;
                            r->unk_08 = (u32)((u8 *)n + 0x58);
                        }
                        if (*(s16 *)(p + 0x20) == e)
                        {
                            n = func_8003FC64(0x212);
                            if (n != NULL)
                            {
                                q = (u8 *)n + 0x20;
                                *(s16 *)(q + 0x2C) = 0x1E;
                                *(s16 *)(q + 0x2E) = 0x1E;
                                n->unk_10 = (u32)func_s58_80025A80;
                                func_8004491C(n, (u32)D_80045340);
                                r = (UnkSlot *)n->unk_0C;
                                r->unk_10 = 0x20;
                                r->unk_14 |= 0xC;
                                *(s32 *)(q + 0x94) = 0xFFF20000;
                                *(s32 *)(q + 0xA0) = 0x20000;
                                *(s32 *)(q + 0x8C) = rec[*(s16 *)(p + 0x26)].a << 18;
                                *(s32 *)(q + 0x90) = rec[*(s16 *)(p + 0x26)].b << 18;
                                *(s32 *)(q + 0x8C) += 0xFFF80000 + ((rand() & 0x3FFF) << 6);
                                *(s32 *)(q + 0x90) += 0xFFF80000 + ((rand() & 0x3FFF) << 6);
                                *(s32 *)(q + 0x94) += 0xFFF80000 + ((rand() & 0x3FFF) << 6);
                                d4 = D_80083780;
                                m = n->unk_08;
                                *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0xC);
                                *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0xE);
                                *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x10) - 0x10;
                                *(s16 *)((u8 *)m + 0x2) += *(u16 *)(d4 + 0x2);
                                *(s16 *)((u8 *)m + 0x6) += *(u16 *)(d4 + 0x6);
                                *(s16 *)((u8 *)m + 0xA) += *(u16 *)(d4 + 0xA);
                                r->unk_1E = 0x1000;
                                r->unk_1C = 0x1000;
                                *((u8 *)r + 0xE) = 0x80;
                                *((u8 *)r + 0xD) = 0x80;
                                *((u8 *)r + 0xC) = 0x80;
                                *(UnkDngS58Name12 *)((u8 *)n + 0x58) = *(UnkDngS58Name12 *)D_800269A8;
                                r->unk_08 = (u32)((u8 *)n + 0x58);
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if (*(s16 *)(p + 0x20) == 0x10)
            {
                n = func_8003FC64(0x212);
                if (n != NULL)
                {
                    q = (u8 *)n + 0x20;
                    *(s16 *)(q + 0x2C) = 7;
                    *(s16 *)(q + 0x2E) = 7;
                    n->unk_10 = (u32)func_s58_80025724;
                    func_8004491C(n, (u32)D_80045340);
                    r = (UnkSlot *)n->unk_0C;
                    r->unk_10 = 0x20;
                    r->unk_14 |= 0xC;
                    d5 = D_80083780;
                    m = n->unk_08;
                    *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0xC);
                    *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0xE);
                    *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x10);
                    *(s16 *)((u8 *)m + 0x2) += *(u16 *)(d5 + 0x2);
                    *(s16 *)((u8 *)m + 0x6) += *(u16 *)(d5 + 0x6);
                    *(s16 *)((u8 *)m + 0xA) += *(u16 *)(d5 + 0xA);
                    r->unk_1C = 0x1800;
                    r->unk_1E = 0x1000;
                    *((u8 *)r + 0xE) = 0x80;
                    *((u8 *)r + 0xD) = 0x80;
                    *((u8 *)r + 0xC) = 0x80;
                    *(UnkDngS58Name12 *)((u8 *)n + 0x58) = *(UnkDngS58Name12 *)D_8002699C;
                    r->unk_08 = (u32)((u8 *)n + 0x58);
                }
            }
            if (*(s16 *)(p + 0x20) >= 0xD)
            {
                for (i = 0; i < 2; i++)
                {
                    func_s58_80024AF8(p, b, c, *(s16 *)(p + 0xC), *(s16 *)(p + 0xE),
                                      (s16)(*(u16 *)(p + 0x10) - 8));
                }
            }
        }
        if ((D_80082E94 & 0x8000) != 0 || --*(s16 *)(p + 0x20) < 0)
        {
            *(s16 *)(p + 0xA) = 4;
        }
        break;
    case 4:
        if (D_800269B4 == 0)
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
            D_800269B4 = 0;
        }
        break;
    }
}
