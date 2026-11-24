#include "common.h"

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
typedef struct UnkDng24EF4SP
{
    u8 pad_00[0x18];
    u8 *unk_18;
    u8 pad_1C[0x4];
    u8 *unk_20;
    u8 pad_24[0x4C];
    SVECTOR unk_70[4];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x28];
    s32 unk_C0;
} UnkDng24EF4SP;
typedef struct UnkDng26478
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x2];
} UnkDng26478;
extern UnkDng26478 D_80026478[];
extern u32 D_80026470;
extern u32 D_80026474;
extern s16 D_s20_8002632A;
void func_80067F20(u32 *, s32, s32, s32, s32);

typedef struct UnkDng255E8Vec
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 pad_06[0x2];
} UnkDng255E8Vec;
typedef struct UnkDng255E8SP
{
    u8 pad_00[0x70];
    SVECTOR unk_70[4];
    u8 pad_90[0x8];
    UnkDng255E8Vec unk_98[4];
} UnkDng255E8SP;
typedef struct UnkDng24ED0SP
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
} UnkDng24ED0SP;
typedef struct UnkDng265C8
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x2];
} UnkDng265C8;
UnkNode *func_8003FC64(s32);
s32 func_800CEEFC();
void func_s20_80024D70(u8 *p, UnkVec *v, u8 *c);

extern u16 D_s20_80026326;

extern u8 D_800E1D01[];
void func_80099844(u8 *, u8 *);
s32 func_8009D218_2(u8 *, s32) __asm__("func_8009D218");
s16 func_800A48F0_i(u8 *, s32, s32) __asm__("func_800A48F0");

extern u8 D_800263F8[];
extern u8 D_80045340[];
void func_8003DB94(void *, void *, s32);
void func_s20_80025868(u8 *p, UnkVec *v, u8 *c);

void func_800478B8(u8 *);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s20", func_s20_80024064);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s20", func_s20_80024804);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s20", func_s20_80024C80);

/* The +0x10 step of the 0x202 node func_s21_8002495C below spawns: bump the
   object's own u16 counter, fade the renderable's RGB triple at +0xC..+0xE by
   one eighth each, and raise the kill bit on the node flag at -0x2 and on
   D_800814A0 once the red channel drops under 8. csrc/ovl_dungeon_800CABA8.c's
   func_800CC744 is the closest resident shape -- the same two-arm fade and the
   same kill tail -- but this one has a single arm and the >> 3 in place of a
   division, so the body is written from the listing.

   The three fades are SEPARATE statements, not a chain: each reads its own
   byte, so there is no common value to carry. `>> 3' and not `/ 8': the
   bytes are u8 and the shift comes out `srl' with no sign correction. The
   guard reads the byte BACK (`lbu 0xC' a second time after the store), which
   is what `c[0xC] < 8' spelled on the array gives.

   30 words, 0 differing on the first compile. raw 6 == the 6 relocations:
   1 HI16 + 2 LO16 on D_s20_80026326 and the same three on D_800814A0. */
void func_s20_80024D70(u8 *p, UnkVec *v, u8 *c)
{
    D_s20_80026326++;
    c[0xC] -= c[0xC] >> 3;
    c[0xD] -= c[0xD] >> 3;
    c[0xE] -= c[0xE] >> 3;
    if (c[0xC] < 8)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x202 spawner for func_s20_80024D70's fading marker.
   csrc/ovl_dungeon_800CABA8.c's func_800CC5F0 is the line-for-line template:
   the same func_8003FC64(0x202), the same `n->unk_10 = step' ahead of
   func_8004491C(n, (u32)func_800CEEFC), and the same mover-then-renderable
   fill. The difference is that nothing here is a literal -- the new node's
   mover is SEEDED FROM AN EXISTING ONE (`v') and its renderable's scale,
   flags, colour word and +0x8 pointer are COPIED FROM AN EXISTING ONE (`q'),
   which is why it returns the node.

   The three velocity halfwords at +0xE/+0x12/+0x16 are the source's position
   MINUS its own +0xE/+0x12/+0x16, and each re-reads the position halfword it
   already stored (two `lhu 0x2' in the stream) -- written out rather than
   carried in a local, which is what keeps the reload.

   Both parameters are `u8 *': the first is read at the mover's halfword
   offsets and the second at the renderable's, and neither is witnessed as a
   struct here beyond those offsets.

   67 words, 0 differing on the first compile. raw 6 == the 6 relocations:
   R_MIPS_26 on func_8003FC64 and func_8004491C, and HI16/LO16 pairs on
   func_s20_80024D70 and func_800CEEFC. */
UnkNode *func_s20_80024DE8(u8 *v, u8 *q)
{
    UnkNode *n;
    u8 *m;
    u8 *r;

    n = func_8003FC64(0x202);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s20_80024D70;
        func_8004491C(n, (u32)func_800CEEFC);
        m = (u8 *)n->unk_08;
        *(s16 *)(m + 0x2) = *(u16 *)(v + 0x2);
        *(s16 *)(m + 0x6) = *(u16 *)(v + 0x6);
        *(s16 *)(m + 0xA) = *(u16 *)(v + 0xA);
        *(s16 *)(m + 0xE) = *(u16 *)(v + 0x2) - *(u16 *)(v + 0xE);
        *(s16 *)(m + 0x12) = *(u16 *)(v + 0x6) - *(u16 *)(v + 0x12);
        *(s16 *)(m + 0x16) = *(u16 *)(v + 0xA) - *(u16 *)(v + 0x16);
        r = (u8 *)n->unk_0C;
        *(s16 *)(r + 0x1C) = *(u16 *)(q + 0x1C);
        *(s16 *)(r + 0x1E) = *(u16 *)(q + 0x1E);
        *(u32 *)(r + 0xC) = *(u32 *)(q + 0xC);
        *(u32 *)(r + 0x8) = *(u32 *)(q + 0x8);
        *(s16 *)(r + 0x14) = *(u16 *)(q + 0x14);
    }
    return n;
}

/* The trail ring's renderer: one semi-transparent POLY_G4 and one DR_MODE per
   live entry of D_80026478, each quad spanning entry `i', the next entry in
   its 0x10-wide ring, and the two matching entries of the second bank at
   &D_80026478[0x10].  155 words, 0 differing, FIRST COMPILE.

   Written against func_s20_80025340's landed body in this same unit (the
   D_80083160/+0x8D0 cursor prologue, the `sp->unk_18' before `sp->unk_20'
   order, the GetTPage/func_80067F20/AddPrim tail) plus
   func_s20_80026010's index arithmetic, so nothing here was fought for.

   Note the contrast with func_s20_80025340's trail-ring store, which needs an
   integer address and an explicit `<< 3': here the PLAIN ARRAY REF is right.
   `D_80026478[i + 0x10]' folding to `i * 8 + (base + 0x80)' is exactly the
   target's `addiu $a3, $s4, 0x80', and an array ref off a loop-invariant
   SYMBOL also gives the `addu $v0, $v1, $s4' operand order that an array ref
   off a pointer LOCAL does not.

   The two colour pairs are CHAINED assignments: `*(u32 *)&q->r0 =
   *(u32 *)&q->r1 = D_80026470' emits +0x0C before +0x04, which is the order
   the target has in both arms.  RotTransPers4 is RotTransPers4 and
   SetPolyG4 is SetPolyG4 -- a real call, not the setPolyG4 macro, which
   is why the tag and code bytes are written after the four colour words. */
s32 func_s20_80024EF4(void)
{
    UnkDng24EF4SP *sp = (UnkDng24EF4SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    POLY_G4 *q;
    DR_MODE *dm;
    s32 i;

    sp->unk_18 = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    for (i = 0; i < D_s20_8002632A; i++)
    {
        q = (POLY_G4 *)sp->unk_18;
        sp->unk_18 = (u8 *)(q + 1);
        *(u32 *)&sp->unk_70[0] = *(u32 *)&D_80026478[i];
        *(u32 *)&sp->unk_70[1] = *(u32 *)&D_80026478[(i & -0x10) + ((i + 1) & 0xF)];
        *(u32 *)&sp->unk_70[2] = *(u32 *)&D_80026478[i + 0x10];
        *(u32 *)&sp->unk_70[3] = *(u32 *)&D_80026478[(i & -0x10) + ((i + 1) & 0xF) + 0x10];
        sp->unk_70[1].vz = sp->unk_70[0].vz = D_80026478[i].unk_04;
        sp->unk_70[3].vz = sp->unk_70[2].vz = D_80026478[i + 0x10].unk_04;
        sp->unk_C0 = RotTransPers4(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2], &sp->unk_70[3],
                                   (s32 *)&q->x0, (s32 *)&q->x1, (s32 *)&q->x2, (s32 *)&q->x3,
                                   &sp->unk_90, &sp->unk_94) -
                     0x8;
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            if (i % 0x20 < 0x10)
            {
                *(u32 *)&q->r0 = *(u32 *)&q->r1 = D_80026470;
                *(u32 *)&q->r2 = *(u32 *)&q->r3 = D_80026474;
            }
            else
            {
                *(u32 *)&q->r2 = *(u32 *)&q->r3 = D_80026470;
                *(u32 *)&q->r0 = *(u32 *)&q->r1 = D_80026474;
            }
            SetPolyG4(q);
            setSemiTrans(q, 1);
            AddPrim(sp->unk_20 + sp->unk_C0 * 4, q);
            dm = (DR_MODE *)sp->unk_18;
            sp->unk_18 = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            AddPrim(sp->unk_20 + sp->unk_C0 * 4, dm);
        }
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_18;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s20", func_s20_80025160);

/* One semi-transparent gouraud LINE_G2 and one DR_MODE per node of the tag
   chain: project the mover's own origin and the point it is interpolated
   towards through the 0x1F800000 scratchpad, average the two depths, and --
   on the forward arm -- record the interpolated point in this bank's trail
   ring at D_80026478.  215 words, 0 differing.

   The twin is csrc/ovl_dng_s16.c's func_s16_8002416C (148 words, landed),
   which is the same walk with a TILE_1 in place of the LINE_G2 and no
   interpolation; its three measured orderings were re-used and hold here --
   `sp->unk_18' is assigned BEFORE `sp->unk_20' even though the target STORES
   them the other way round, and `p = (u8 *)n + 0x20;' comes before
   `v = n->unk_08;'.

   THE ONE SPELLING FACT: the trail-ring index is `(u32)t', an INTEGER
   address, with an explicit `<< 3' -- not `t[... + 0x10]'.  Two things ride
   on that and both were measured here:

     `t[*(s16 *)(p + 0x22) + 0x10].unk_00 = ...'  folds (i + 0x10) * 8 into
         i * 8 + 0x80 and puts the 0x80 in the store's displacement, which
         DELETES the target's three `addiu $v0, $v0, 0x10': 212 words,
         99 aligned differences.
     `*(s16 *)((u8 *)t + ((... + 0x10) << 3))'    keeps the three `addiu' but
         `pointer_int_sum' always puts the pointer operand FIRST, so the
         address comes out `addu $v0, $s5, $v0' where the target has
         `addu $v0, $v0, $s5': 215 words, 3 aligned differences, and writing
         the sum the other way round in the source does not move it.
     `*(s16 *)(((... + 0x10) << 3) + (u32)t)'     both ints, so the written
         order survives: 0.  `(u8 *)((... ) << 3) + (u32)t' is also 0.

   The arm order is load-bearing: `if (s->unk_06 < 0)' first is 212 words and
   185 differing -- the target falls through on `>= 0' and `bltz's to the
   negative arm.  RotTransPers is RotTransPers; the `func_' spellings are
   what splat/dungeon.bin_undefined_funcs_auto.txt links. */
s32 func_s20_80025340(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkDng73E94SP *sp = (UnkDng73E94SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkDng26478 *t = D_80026478;
    UnkNode *n;
    LINE_G2 *l;
    DR_MODE *dm;

    sp->unk_18 = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    while (1)
    {
        l = (LINE_G2 *)sp->unk_18;
        sp->unk_18 = (u8 *)(l + 1);
        if (s->unk_06 >= 0)
        {
            sp->unk_00 = *(u16 *)((u8 *)v + 0xE);
            sp->unk_02 = *(u16 *)((u8 *)v + 0x12);
            sp->unk_04 = *(u16 *)((u8 *)v + 0x16);
            sp->unk_C0 = RotTransPers((SVECTOR *)sp, (s32 *)&l->x0, &sp->unk_90, &sp->unk_94);
            sp->unk_00 = *(u16 *)((u8 *)v + 0xE) +
                         ((*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)v + 0xE)) * s->unk_06 >> 8);
            sp->unk_02 = *(u16 *)((u8 *)v + 0x12) +
                         ((*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)v + 0x12)) * s->unk_06 >> 8);
            sp->unk_04 = *(u16 *)((u8 *)v + 0x16) +
                         ((*(s16 *)((u8 *)v + 0xA) - *(s16 *)((u8 *)v + 0x16)) * s->unk_06 >> 8);
            if (D_s20_8002632A < *(s16 *)(p + 0x22) + 0x10)
            {
                *(s16 *)(((*(s16 *)(p + 0x22) + 0x10) << 3) + (u32)t) = sp->unk_00;
                *(s16 *)(((*(s16 *)(p + 0x22) + 0x10) << 3) + (u32)t + 0x2) = sp->unk_02;
                *(s16 *)(((*(s16 *)(p + 0x22) + 0x10) << 3) + (u32)t + 0x4) = sp->unk_04;
            }
        }
        else
        {
            sp->unk_00 = *(u16 *)((u8 *)v + 0x2) -
                         ((*(s16 *)((u8 *)v + 0xE) - *(s16 *)((u8 *)v + 0x2)) * s->unk_06 >> 8);
            sp->unk_02 = *(u16 *)((u8 *)v + 0x6) -
                         ((*(s16 *)((u8 *)v + 0x12) - *(s16 *)((u8 *)v + 0x6)) * s->unk_06 >> 8);
            sp->unk_04 = *(u16 *)((u8 *)v + 0xA) -
                         ((*(s16 *)((u8 *)v + 0x16) - *(s16 *)((u8 *)v + 0xA)) * s->unk_06 >> 8);
            sp->unk_C0 = RotTransPers((SVECTOR *)sp, (s32 *)&l->x0, &sp->unk_90, &sp->unk_94);
            sp->unk_00 = *(u16 *)((u8 *)v + 0x2);
            sp->unk_02 = *(u16 *)((u8 *)v + 0x6);
            sp->unk_04 = *(u16 *)((u8 *)v + 0xA);
        }
        sp->unk_C0 =
            (sp->unk_C0 + RotTransPers((SVECTOR *)sp, (s32 *)&l->x1, &sp->unk_90, &sp->unk_94)) >>
            1;
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            *(u32 *)&l->r0 = *(u32 *)s->unk_0C;
            setLineG2(l);
            setSemiTrans(l, 1);
            l->r1 = l->r0 >> 1;
            l->g1 = l->g0 >> 1;
            l->b1 = l->b0 >> 1;
            AddPrim(sp->unk_20 + sp->unk_C0 * 4, l);
            dm = (DR_MODE *)sp->unk_18;
            sp->unk_18 = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            AddPrim(sp->unk_20 + sp->unk_C0 * 4, dm);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_18;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s20", func_s20_8002569C);

/* The +0x10 step of func_s21_800254FC's 0x202 node: bump the counter,
   integrate the mover's velocity into its position, damp the two horizontal
   velocity words by an eighth while adding a constant 0x4000 to the third,
   fade the renderable's RGB triple by a quarter, hand the renderable to
   func_800478B8 and kill the node when the +0x1A countdown runs out.
   csrc/ovl_town.c's func_800A86BC is the twin -- the same three vector adds,
   the same func_800478B8 hand-off and the same kill tail -- and its two
   measured notes both hold here: `--*(s16 *)(p + 0x1A) <= 0' gives the
   `lhu'/`addiu -1'/`sh'/`sll 16'/`bgtz' run with no `sra', and the vector
   updates are order-pinned.

   `>> 3' on the two velocity words is SIGNED (`sra'): UnkVec's fields are
   s32. `>> 2' on the three colour bytes is UNSIGNED (`srl'): they are u8.
   The three colour fades are separate statements, as in func_s21_800248E4.

   65 words, 0 differing on the first compile. raw 7 == the 7 relocations:
   1 HI16 + 2 LO16 on D_s20_80026326, the same three on D_800814A0, and
   R_MIPS_26 on func_800478B8. */
void func_s20_80025868(u8 *p, UnkVec *v, u8 *c)
{
    D_s20_80026326++;
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    v->unk_0C -= v->unk_0C >> 3;
    v->unk_10 -= v->unk_10 >> 3;
    v->unk_14 += 0x4000;
    c[0xC] -= c[0xC] >> 2;
    c[0xD] -= c[0xD] >> 2;
    c[0xE] -= c[0xE] >> 2;
    func_800478B8(c);
    if (--*(s16 *)(p + 0x1A) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x202 spawner for func_s20_80025868's puff.
   csrc/ovl_dungeon_800B6AC4.c's func_800B8D64 is the line-for-line template
   -- the same three s16 position parameters stored straight to the mover's
   +0x2/+0x6/+0xA, the same func_8004491C(n, (u32)D_80045340) tag, the same
   `0x1E' then `0x1C' scale pair and `|= 0xC' on the flag halfword -- with
   0x202 for its 0x212, a func_8003DB94 template in place of the bare +0x8
   pointer, and a random spin.

   THE THREE PARAMETERS ARE `s16' AND GO STRAIGHT INTO THE `sh': no masking
   appears in the target, which is what a narrow parameter stored to a
   halfword gives.

   `*(u32 *)(r + 0xC) = 0x808080;' COMES BEFORE func_8003DB94, which is the
   same ordering fact func_800CC5F0 records: the store rides in the call's
   delay slot. The two 0x1000 scales are a literal written twice, from one
   `addiu', and NOT the read-back func_800CC5F0 and func_800CC88C use -- the
   target has no `lhu 0x1E' between them. The spin is
   `rand() & 0x7'.

   59 words, 0 differing on the first compile. raw 10 == the 10 relocations:
   R_MIPS_26 on func_8003FC64, func_8004491C, rand and
   func_8003DB94, and HI16/LO16 pairs on D_80045340, func_s20_80025868 and
   D_800263F8. */
void func_s20_8002596C(s16 x, s16 y, s16 z)
{
    UnkNode *n;
    u8 *m;
    u8 *r;

    n = func_8003FC64(0x202);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s20_80025868;
        func_8004491C(n, (u32)D_80045340);
        m = (u8 *)n->unk_08;
        *(s16 *)(m + 0x2) = x;
        *(s16 *)(m + 0x6) = y;
        *(s16 *)(m + 0xA) = z;
        *(s16 *)(m + 0xE) = 0;
        *(s16 *)(m + 0x12) = 0;
        *(s16 *)(m + 0x16) = rand() & 0x7;
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0xC) = 0x808080;
        func_8003DB94(r, D_800263F8, 0);
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(s16 *)(r + 0x10) = 0x20;
        *(u16 *)(r + 0x14) |= 0xC;
        *(s16 *)((u8 *)n + 0x3A) = 0x10;
    }
}

/* The +0x10 step of the 0x202 node func_s21_80025BA0 spawns: rebase the four
   corner records at the payload's +0x34..+0x53 on the mover's own origin into
   the scratchpad quad at 0x1F800070, build a rotation-only matrix from the
   slot's +0x16/+0x18 on the stack, push it, run the four corners through
   RotTransSV, pop, integrate the mover's velocity, write the rotated
   corners back onto the payload biased by the mover again, fade the six
   colour bytes at +0x54..+0x5A by one 1/(+0x1A) step each, and kill the node
   when the +0x1A counter runs out.

   NO TWIN OUTSIDE ITS OWN FAMILY. tools/decomp/dngfam.py's only sibling is
   func_s20_80025A58, which is the identical body with every payload offset
   4 lower and its counter inside the object (D_s20_80026326) instead of out
   of image (D_s20_80026326); the best landed non-family neighbour is 0.363.
   docs/dng-parks/func_s20_80025A58.c, agent-h13's park on that sibling, is
   where the addressing convention and six of the spellings below come from:
   a scratchpad STORE is `sp->unk_NN' through a struct pointer at
   0x1F800000 and a scratchpad ADDRESS PASSED AS AN ARGUMENT is the integer
   constant `(SVECTOR *)0x1F800070' off the same hoisted `lui' (stores as
   constant addresses too: 275 words, 258 differing), the stack MATRIX's
   translation is zeroed t[2], t[1], t[0] DESCENDING (ascending, 2 rows), and
   the colour fade re-reads `*(s16 *)(p + 0x1A)' at all six sites so the
   six lbu/lh/div/mflo runs hoist into one block.

   THE COUNTER BUMP IS SPLIT AROUND THE 0x48 CORNER, AND THAT IS THE WHOLE
   OF THE LAST SEVEN WORDS. Written as a plain `D_s20_80026326++' anywhere in the
   vz block the body is 253 words and never better than 7 aligned
   differences, all of them in one window where the counter's lhu/addiu/sh
   and the 0x48 corner's lhu/lhu/subu/sh interleave in the wrong order:
   sched.c's backward list scheduler ties both stores on INSN_PRIORITY (a
   store's destination is a MEM, so birthing_insn_p can never boost it) and
   falls to INSN_LUID, i.e. source order, which a single statement cannot
   split finely enough. Reading the counter BEFORE the corner, computing the
   corner into a local, storing the counter BETWEEN them and storing the
   corner LAST puts the four LUID groups in the order the target wants and is
   0 differing. Measured on this body, all 253 words:

     this spelling                                                     0
     `D_s20_80026326++' after the 0x48 corner (h13's park body)            7
     `D_s20_80026326++' before it                                         13
     the counter's read hoisted, its STORE after the corner's store     2
     `c = D; ...corner...; D = c + 1;' with the corner stored in place  7
     `c = D + 1; ...corner...; D = c;'                                  7
     the counter inside the corner statement as a comma expression,
         in any of four positions                                     13
     one shared s32 local for the 0x48 and 0x50 subtractions          11
     the same for all four vz corners / for all twelve corners    19 / 51

   The last three rows are the boost-removal experiment h13's park proposes
   and they are all negative, which is the evidence that the tie is decided
   before any boost: a local written twice does have reg_n_sets > 1 at flow
   time, and it makes the window worse, not better. THE SAME SPLIT SHOULD
   LAND func_s20_80025A58, whose residue is these same seven words.

   253 words, 0 differing. raw 15 == the 15 relocations: 1 HI16 + 2 LO16 on
   D_s20_80026326, the same three on D_800814A0, and R_MIPS_26 on PushMatrix,
   RotMatrix, SetTransMatrix, SetRotMatrix, RotTransSV four times and
   PopMatrix.
   THIS COPY'S OWN CONSTANT: 0x34 -> 0x30 at all 2 sites, 0x3c -> 0x38 at all 2 sites, 0x44 -> 0x40 at all 2 sites, 0x4c -> 0x48 at all 2 sites, 0x36 -> 0x32 at all 2 sites, 0x3e -> 0x3a at all 2 sites, 0x46 -> 0x42 at all 2 sites, 0x4e -> 0x4a at all 2 sites, 0x38 -> 0x34 at all 2 sites, 0x40 -> 0x3c at all 2 sites, 0x48 -> 0x44 at all 2 sites, 0x50 -> 0x4c at all 2 sites, 0x54 -> 0x50 at all 2 sites, 0x55 -> 0x51 at all 2 sites, 0x56 -> 0x52 at all 2 sites, 0x58 -> 0x54 at all 2 sites, 0x59 -> 0x55 at all 2 sites, 0x5a -> 0x56 at all 2 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s20_80025A58(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkDng255E8SP *sp = (UnkDng255E8SP *)0x1F800000;
    SVECTOR rot;
    MATRIX m;
    s32 t;
    s16 n;
    u16 c;

    sp->unk_70[0].vx = *(u16 *)(p + 0x30) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[1].vx = *(u16 *)(p + 0x38) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[2].vx = *(u16 *)(p + 0x40) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[3].vx = *(u16 *)(p + 0x48) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[0].vy = *(u16 *)(p + 0x32) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[1].vy = *(u16 *)(p + 0x3a) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[2].vy = *(u16 *)(p + 0x42) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[3].vy = *(u16 *)(p + 0x4a) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[0].vz = *(u16 *)(p + 0x34) - *(u16 *)((u8 *)v + 0xA);
    sp->unk_70[1].vz = *(u16 *)(p + 0x3c) - *(u16 *)((u8 *)v + 0xA);
    c = D_s20_80026326;
    t = *(u16 *)(p + 0x44) - *(u16 *)((u8 *)v + 0xA);
    D_s20_80026326 = c + 1;
    sp->unk_70[2].vz = t;
    sp->unk_70[3].vz = *(u16 *)(p + 0x4c) - *(u16 *)((u8 *)v + 0xA);
    PushMatrix();
    rot.vx = s->unk_16;
    rot.vy = s->unk_18;
    rot.vz = 0;
    m.t[2] = 0;
    m.t[1] = 0;
    m.t[0] = 0;
    RotMatrix(&rot, &m);
    SetTransMatrix(&m);
    SetRotMatrix(&m);
    RotTransSV((SVECTOR *)0x1F800070, (s32 *)0x1F800098, (s32 *)0x1F800094);
    RotTransSV((SVECTOR *)0x1F800078, (s32 *)0x1F8000A0, (s32 *)0x1F800094);
    RotTransSV((SVECTOR *)0x1F800080, (s32 *)0x1F8000A8, (s32 *)0x1F800094);
    RotTransSV((SVECTOR *)0x1F800088, (s32 *)0x1F8000B0, (s32 *)0x1F800094);
    PopMatrix();
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    *(s16 *)(p + 0x30) = sp->unk_98[0].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x38) = sp->unk_98[1].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x40) = sp->unk_98[2].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x48) = sp->unk_98[3].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x32) = sp->unk_98[0].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x3a) = sp->unk_98[1].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x42) = sp->unk_98[2].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x4a) = sp->unk_98[3].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x34) = sp->unk_98[0].unk_04 + *(u16 *)((u8 *)v + 0xA);
    *(s16 *)(p + 0x3c) = sp->unk_98[1].unk_04 + *(u16 *)((u8 *)v + 0xA);
    *(s16 *)(p + 0x44) = sp->unk_98[2].unk_04 + *(u16 *)((u8 *)v + 0xA);
    *(s16 *)(p + 0x4c) = sp->unk_98[3].unk_04 + *(u16 *)((u8 *)v + 0xA);
    p[0x50] -= p[0x50] / *(s16 *)(p + 0x1A);
    p[0x51] -= p[0x51] / *(s16 *)(p + 0x1A);
    p[0x52] -= p[0x52] / *(s16 *)(p + 0x1A);
    p[0x54] -= p[0x54] / *(s16 *)(p + 0x1A);
    p[0x55] -= p[0x55] / *(s16 *)(p + 0x1A);
    p[0x56] -= p[0x56] / *(s16 *)(p + 0x1A);
    n = *(u16 *)(p + 0x1A) - 1;
    *(s16 *)(p + 0x1A) = n;
    if (n <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s20", func_s20_80025E4C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s20", func_s20_80026010);

/* Apply status kind 0x17 at a strength scaled off the second parameter, then
   print the D_800E1D01 message, both gated on func_8009D218 coming back zero.
   csrc/ovl_dungeon_800C2824.c's func_800A48F0/func_80099844 pairs are the
   shape; func_8009D218 takes TWO arguments here, not the three
   csrc/ovl_dungeon_decls.h declares, so it rides a unit-local alias.

   THE SECOND PARAMETER IS `u8' AND THE SHIFT IS ITS OWN STATEMENT, and both
   halves are load-bearing -- together they are 9 of the 27 words:

   * `s32 d' with `((d & 0xFF) >> 2) + 2' is 9 differing. It gives `sra' for
     the target's `srl' (1 word) and, worse, swaps the two callee-saved
     registers: `p' lands in $s0 and `d' in $s1 where the target has them the
     other way round, and the argument setup then comes out in the other
     order too (8 words). The register order is global_alloc's priority
     `floor_log2 (n_refs) * n_refs / live_length': `p' is 4 refs over 21
     insns and `d' 2 over 7, so `p' sorts first and takes $s0. A `u8'
     parameter costs an extra flow-counted set for the truncation, which
     carries `d' past it. `u32 d' fixes only the `srl' -- still 9.
   * `e = d; e >>= 2;' as two statements for `e = d >> 2' is worth 4 words on
     top of that. With the shift in the argument expression the and/shift
     chain's temporaries are each set ONCE, so adjust_priority's
     birthing_insn_p boost lifts them to max_priority and sched.c's backward
     scan places them after the $a0/$a1 argument moves; a temporary written
     by two statements has reg_n_sets > 1, loses the boost, and the chain
     comes out first, which is the target. A device-free spelling: the `andi
     0xFF' in the target IS the u8 parameter's own truncation.
     `u32 d' plus a redundant `d &= 0xFFFF;' that combine deletes measures
     0 as well, and is the same two mechanisms bought with a device instead.

   27 words, 0 differing. raw 5 == the 5 relocations: R_MIPS_26 on
   func_8009D218, func_800A48F0 and func_80099844, and a HI16/LO16 pair on
   D_800E1D01.
   THIS COPY'S OWN CONSTANT: 0x4 -> 0x2, 0x17 -> 0x16, 0x2 -> 0x4 at 1 of 2 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s20_800262B8(u8 *p, u8 d)
{
    u32 e;

    if (func_8009D218_2(p, 2) == 0)
    {
        e = d;
        e >>= 2;
        if ((s16)func_800A48F0_i(p, 0x16, e + 4) != 0)
        {
            func_80099844(p, D_800E1D01);
        }
    }
}
