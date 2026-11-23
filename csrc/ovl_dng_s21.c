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
extern UnkDng26478 D_800265C8[];
extern s16 D_80026476;
s32 func_80065420(SVECTOR *, s32 *, s32 *, s32 *);
void func_8006658C(void *, void *);
void func_80067F20(u32 *, s32, s32, s32, s32);

extern u32 D_800265C0;
extern u32 D_800265C4;
s32 func_800654B0(SVECTOR *, SVECTOR *, SVECTOR *, SVECTOR *, s32 *, s32 *, s32 *, s32 *, s32 *,
                  s32 *);
void func_80066708(POLY_G4 *);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s21", func_s21_80024050);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s21", func_s21_800247AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s21", func_s21_80024804);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s21", func_s21_800248E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s21", func_s21_8002495C);

/* The trail ring's renderer: one semi-transparent POLY_G4 and one DR_MODE per
   live entry of D_800265C8, each quad spanning entry `i', the next entry in
   its 0x10-wide ring, and the two matching entries of the second bank at
   &D_800265C8[0x10].  155 words, 0 differing, FIRST COMPILE.

   Written against func_s20_80025340's landed body in this same unit (the
   D_80083160/+0x8D0 cursor prologue, the `sp->unk_18' before `sp->unk_20'
   order, the GetTPage/func_80067F20/func_8006658C tail) plus
   func_s20_80026010's index arithmetic, so nothing here was fought for.

   Note the contrast with func_s20_80025340's trail-ring store, which needs an
   integer address and an explicit `<< 3': here the PLAIN ARRAY REF is right.
   `D_800265C8[i + 0x10]' folding to `i * 8 + (base + 0x80)' is exactly the
   target's `addiu $a3, $s4, 0x80', and an array ref off a loop-invariant
   SYMBOL also gives the `addu $v0, $v1, $s4' operand order that an array ref
   off a pointer LOCAL does not.

   The two colour pairs are CHAINED assignments: `*(u32 *)&q->r0 =
   *(u32 *)&q->r1 = D_800265C0' emits +0x0C before +0x04, which is the order
   the target has in both arms.  func_800654B0 is RotTransPers4 and
   func_80066708 is SetPolyG4 -- a real call, not the setPolyG4 macro, which
   is why the tag and code bytes are written after the four colour words. */
s32 func_s21_80024A68(void)
{
    UnkDng24EF4SP *sp = (UnkDng24EF4SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    POLY_G4 *q;
    DR_MODE *dm;
    s32 i;

    sp->unk_18 = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    for (i = 0; i < D_80026476; i++)
    {
        q = (POLY_G4 *)sp->unk_18;
        sp->unk_18 = (u8 *)(q + 1);
        *(u32 *)&sp->unk_70[0] = *(u32 *)&D_800265C8[i];
        *(u32 *)&sp->unk_70[1] = *(u32 *)&D_800265C8[(i & -0x10) + ((i + 1) & 0xF)];
        *(u32 *)&sp->unk_70[2] = *(u32 *)&D_800265C8[i + 0x10];
        *(u32 *)&sp->unk_70[3] = *(u32 *)&D_800265C8[(i & -0x10) + ((i + 1) & 0xF) + 0x10];
        sp->unk_70[1].vz = sp->unk_70[0].vz = D_800265C8[i].unk_04;
        sp->unk_70[3].vz = sp->unk_70[2].vz = D_800265C8[i + 0x10].unk_04;
        sp->unk_C0 = func_800654B0(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2], &sp->unk_70[3],
                                   (s32 *)&q->x0, (s32 *)&q->x1, (s32 *)&q->x2, (s32 *)&q->x3,
                                   &sp->unk_90, &sp->unk_94) -
                     0x8;
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            if (i % 0x20 < 0x10)
            {
                *(u32 *)&q->r0 = *(u32 *)&q->r1 = D_800265C0;
                *(u32 *)&q->r2 = *(u32 *)&q->r3 = D_800265C4;
            }
            else
            {
                *(u32 *)&q->r2 = *(u32 *)&q->r3 = D_800265C0;
                *(u32 *)&q->r0 = *(u32 *)&q->r1 = D_800265C4;
            }
            func_80066708(q);
            setSemiTrans(q, 1);
            func_8006658C(sp->unk_20 + sp->unk_C0 * 4, q);
            dm = (DR_MODE *)sp->unk_18;
            sp->unk_18 = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            func_8006658C(sp->unk_20 + sp->unk_C0 * 4, dm);
        }
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_18;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s21", func_s21_80024CD4);

/* One semi-transparent gouraud LINE_G2 and one DR_MODE per node of the tag
   chain: project the mover's own origin and the point it is interpolated
   towards through the 0x1F800000 scratchpad, average the two depths, and --
   on the forward arm -- record the interpolated point in this bank's trail
   ring at D_800265C8.  215 words, 0 differing.

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
   negative arm.  func_80065420 is RotTransPers; the `func_' spellings are
   what splat/dungeon.bin_undefined_funcs_auto.txt links. */
s32 func_s21_80024ED0(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkDng73E94SP *sp = (UnkDng73E94SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkDng26478 *t = D_800265C8;
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
            sp->unk_C0 = func_80065420((SVECTOR *)sp, (s32 *)&l->x0, &sp->unk_90, &sp->unk_94);
            sp->unk_00 = *(u16 *)((u8 *)v + 0xE) +
                         ((*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)v + 0xE)) * s->unk_06 >> 8);
            sp->unk_02 = *(u16 *)((u8 *)v + 0x12) +
                         ((*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)v + 0x12)) * s->unk_06 >> 8);
            sp->unk_04 = *(u16 *)((u8 *)v + 0x16) +
                         ((*(s16 *)((u8 *)v + 0xA) - *(s16 *)((u8 *)v + 0x16)) * s->unk_06 >> 8);
            if (D_80026476 < *(s16 *)(p + 0x22) + 0x10)
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
            sp->unk_C0 = func_80065420((SVECTOR *)sp, (s32 *)&l->x0, &sp->unk_90, &sp->unk_94);
            sp->unk_00 = *(u16 *)((u8 *)v + 0x2);
            sp->unk_02 = *(u16 *)((u8 *)v + 0x6);
            sp->unk_04 = *(u16 *)((u8 *)v + 0xA);
        }
        sp->unk_C0 =
            (sp->unk_C0 + func_80065420((SVECTOR *)sp, (s32 *)&l->x1, &sp->unk_90, &sp->unk_94)) >>
            1;
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            *(u32 *)&l->r0 = *(u32 *)s->unk_0C;
            setLineG2(l);
            setSemiTrans(l, 1);
            l->r1 = l->r0 >> 1;
            l->g1 = l->g0 >> 1;
            l->b1 = l->b0 >> 1;
            func_8006658C(sp->unk_20 + sp->unk_C0 * 4, l);
            dm = (DR_MODE *)sp->unk_18;
            sp->unk_18 = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            func_8006658C(sp->unk_20 + sp->unk_C0 * 4, dm);
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s21", func_s21_8002522C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s21", func_s21_800253F8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s21", func_s21_800254FC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s21", func_s21_800255E8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s21", func_s21_800259DC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s21", func_s21_80025BA0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s21", func_s21_80025E48);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s21", func_s21_800262AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s21", func_s21_80026404);
