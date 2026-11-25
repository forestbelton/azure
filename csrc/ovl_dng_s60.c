#include "common.h"

void func_800478B8(u8 *);
void func_s60_8002548C(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u32 D_800814A0;

typedef struct UnkDngS60PolySP
{
    u8 *unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s16 unk_10;
    s16 unk_12;
    s16 unk_14;
    u8 pad_16[0x2];
    s16 unk_18;
    s16 unk_1A;
    s16 unk_1C;
    u8 pad_1E[0x2];
    s16 unk_20;
    s16 unk_22;
    s16 unk_24;
    u8 pad_26[0x2];
    s16 unk_28;
    s16 unk_2A;
    s16 unk_2C;
    u8 pad_2E[0x2];
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    u8 pad_3C[0x4];
    s32 unk_40;
    s32 unk_44;
    s32 unk_48;
    u8 pad_4C[0x4];
    s32 unk_50;
    s32 unk_54;
    s32 unk_58;
    u8 pad_5C[0x4];
    s32 unk_60;
    s32 unk_64;
    s32 unk_68;
    u8 pad_6C[0x4];
    u16 unk_70;
    u16 unk_72;
    u16 unk_74;
    u16 unk_76;
    u16 unk_78;
    u16 unk_7A;
    u16 unk_7C;
    u16 unk_7E;
} UnkDngS60PolySP;
UnkNode *func_8003FD64(s32, void *);
void func_s60_80024050(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u8 D_80083498[];
extern u32 D_800261E4;
void func_8003DB94(void *, void *, s32);
extern u8 D_800DEB70[];
extern u8 D_80045340[];
extern u8 D_800DDC40[];
void func_s60_800251E8(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_s60_80024D84(u8 *, UnkVec *, u8 *);
void func_s60_800257B8(void);
extern u32 D_800814A8;
void func_s60_8002560C(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_800644B8(s32);
s32 func_80064584(s32);
extern u8 D_80026180[];
extern u32 D_80026208;
UnkNode *func_8003FC64(s32);
s16 func_800BCAD0(u8 *);
void func_s60_80025340(u8 *p);
void func_s60_80025940(u8 *p, UnkVec *b, u8 *c, u8 *d);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
extern u8 D_80082E80[];
extern s16 D_80083228;
extern u8 *D_800E3D18;

/* THE ACTOR DRIVER: a three-state machine on `*(s16 *)(p + 0xC)' that walks a
   homing bolt from the caster's hand slot to the aiming reticle. State 0
   dresses the node through D_80045340 and falls through into state 1, which
   tracks the caster's +0xC slot projection into the mover (plus func_8003DE58's
   three deltas, or z - 0x20 when the slot's +0x14 0x8000 flag is up) and arms
   the 0xA-frame countdown at +0xE once the room record's 0x80 bit admits it.
   State 2 brightens the renderable by 8, takes the reticle at D_80082E80 and
   the D_800E3D18 facing entry for the camera's octant as the destination,
   eases the mover toward it over the countdown with a `* 170' lift on z, and
   on the last frame snaps to the destination and retires the actor.
   220 words, 0 differing, FIRST COMPILE. OWNS NO TABLE -- the dispatch is a
   balanced compare tree on {0,1,2} (`beq 1' / `slti 2' / `beqz' / `beq 2'),
   because three arms are under cc1psx's five-case threshold.

   TWIN: the landed func_800B8830 in csrc/ovl_dungeon_800B6AC4.c (xsibs 0.831,
   252 words), which is this routine in the resident overlay. Everything
   structural came from it: the `||' gate shape, the `s16 out[4]' stack array,
   the `(pos - b->unk_0N) / *(s16 *)(p + 0xE)' easing and the `>> 9 & 7' index
   into D_800E3D18 (which cc1psx folds to the listing's `sra 7' / `andi 0x1C').
   This copy keeps the destination in b->unk_0C..unk_14 where the twin uses a
   `s32 pos[3]' local, reads the reticle height off `D_800814A8 + 0x88' rather
   than `p + 0x32', passes D_80082E80 as func_8003DE58's slot, and has no
   trail spawn.

   MEASURED AND FREE at 220 words / 0 differing, so NEITHER is why this lands:
     the twin's INDEX form for the state bump, `(*((s16 *)p + 6))++'  220 / 0
     the twin's gate spelled `!((flag & 0x8000) == 0 && call == 0)'   220 / 0
   The twin prices its index form at 26 aligned differences; that fact is about
   the bump in its RETIRE tail, next to the `lw %lo(D_800814A0)' whose delay
   slot it fills, and this copy has no bump there -- so the fact is real on the
   twin and vacuous here. `z' as its own `u16' local IS load-bearing by
   construction: the `else' arm's `addiu $v0, $v1, -0x20' reuses the value
   loaded from the mover, which a re-read of `b + 0xA' cannot give. */
void func_s60_80024050(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkSlot *s;
    u8 *v;
    s16 out[4];
    u16 z;
    s32 u;
    s32 i;

    switch (*(s16 *)(p + 0xC))
    {
    case 0:
        func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
        *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) + 1;
        /* fallthrough */
    case 1:
        s = (UnkSlot *)(*(UnkNode **)p)->unk_0C;
        if ((s->unk_14 & 0x8000) != 0 || func_8003DE58((u8 *)s->unk_08, s, out, 0) != 0)
        {
            v = (u8 *)(*(UnkNode **)p)->unk_08;
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)(v + 0x2);
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)(v + 0x6);
            z = *(u16 *)(v + 0xA);
            *(s16 *)((u8 *)b + 0xA) = z;
            if ((s->unk_14 & 0x8000) == 0)
            {
                *(s16 *)((u8 *)b + 0x2) += out[0];
                *(s16 *)((u8 *)b + 0x6) += out[1];
                *(s16 *)((u8 *)b + 0xA) += out[2];
            }
            else
            {
                *(s16 *)((u8 *)b + 0xA) = z - 0x20;
            }
        }
        if ((**(u16 **)(p + 0x8) & 0x80) != 0)
        {
            *(s16 *)(p + 0xE) = 0xA;
            *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) + 1;
        }
        break;
    case 2:
        u = c[0xE] + 8;
        c[0xE] = u;
        c[0xD] = u;
        c[0xC] = u;
        v = (u8 *)(*(UnkNode **)(p + 0x4))->unk_08;
        b->unk_0C = *(s32 *)(v + 0x0);
        b->unk_10 = *(s32 *)(v + 0x4);
        b->unk_14 = (*(s16 *)(D_800814A8 + 0x88) - 0x50) << 16;
        i = ((D_80083228 + *(s16 *)(D_800814A8 + 0x2A) + 0x100) >> 9) & 7;
        if (func_8003DE58(((u8 **)D_800E3D18)[i], (UnkSlot *)D_80082E80, out, 0) != 0)
        {
            b->unk_0C += out[0] << 16;
            b->unk_10 += out[1] << 16;
            b->unk_14 += out[2] << 16;
        }
        b->unk_00 += (b->unk_0C - b->unk_00) / *(s16 *)(p + 0xE);
        b->unk_04 += (b->unk_10 - b->unk_04) / *(s16 *)(p + 0xE);
        b->unk_08 += (b->unk_14 - b->unk_08) / *(s16 *)(p + 0xE) -
                     (func_800644B8(*(s16 *)(p + 0xE) * 170) << 7);
        (*(s16 *)(p + 0xE))--;
        if (*(s16 *)(p + 0xE) <= 0)
        {
            b->unk_00 = b->unk_0C;
            b->unk_04 = b->unk_10;
            b->unk_08 = b->unk_14;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        break;
    }
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x300;
}

/* Spawn the 0x12 node that carries this object's own D_800261E4 sprite data:
   install func_s60_80024050 as the per-frame driver, zero the three halfwords
   at the payload's +0xC..+0x10, park the caller's two records at +0x20/+0x28
   with the dresser D_80083498 between them, then grey the renderable 0x40
   flat, scale it 0x1800 square, spin it by `rand() % 0x1000' and raise the
   +0x10 0x20 and +0x14 0x10C bits with the 0x7DCF pattern word.
   60 words, 0 differing, FIRST COMPILE.

   No dngtwin or xsibs hit; written off the landed 0x212 spawners
   (func_s10_80024130 in csrc/ovl_dng_s10.c, func_s13_8002421C in
   csrc/ovl_dng_s13.c), which supply the `rand() % 0x1000' signed modulo (the
   `bgez' / `addiu 0xFFF' / `sra 12' / `sll 12' / `subu' run) and the idiom
   order. The payload pointer `t' is what gives the listing's
   `addiu $v0, $v1, 0x20', and the FIRST record store stays spelled off the
   node (`*(u8 **)((u8 *)n + 0x20) = p') because combine folds it back onto
   `0x20($v1)' by itself -- exactly as func_s22_80024824 records.

   MEASURED AND FREE at 60 words / 0 differing, so it is NOT why this lands:
   `t = (u8 *)(n + 1)' in place of `t = n->payload' (structs.h records the two
   as the same `addu $2,$4,32', and the measurement agrees). */
void func_s60_800243C0(u8 *p, u8 *q)
{
    UnkNode *n;
    u8 *t;
    u8 *m;

    n = func_8003FD64(0x12, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s60_80024050;
        t = n->payload;
        *(s16 *)(t + 0xC) = 0;
        *(s16 *)(t + 0xE) = 0;
        *(s16 *)(t + 0x10) = 0;
        *(u8 **)((u8 *)n + 0x20) = p;
        *(u8 **)(t + 0x4) = D_80083498;
        *(u8 **)(t + 0x8) = q;
        m = (u8 *)n->unk_0C;
        *(u32 *)(m + 0x8) = D_800261E4;
        m[0xE] = 0x40;
        m[0xD] = 0x40;
        m[0xC] = 0x40;
        *(s16 *)(m + 0x1E) = 0x1800;
        *(s16 *)(m + 0x1C) = 0x1800;
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x12) = 0x7DCF;
        *(u16 *)(m + 0x10) |= 0x20;
        *(u16 *)(m + 0x14) |= 0x10C;
    }
}

/* Push one semi-transparent POLY_GT3 for the three VECTORs sitting in the
   0x1F800000 scratchpad at +0x30/+0x40/+0x50: offset each by the D_80026208
   world origin into the three SVECTORs at +0x10/+0x18/+0x20, project them with
   RotAverage3, and chain the primitive into D_80083160's +0x8D0 ring with
   addPrim when the average depth is under 0x1E0. The three arguments are
   packed 0xRRGGBB colours, one per vertex. Returns 0.
   176 words, 0 differing.
   THIS BODY ADDRESSES A CONSTANT BASE (0x1F800000). `ovlverify --raw' reports
   exactly 6 differing words and ALL SIX are relocation fields --
   `%hi'/`%lo(D_80083160)', `%hi'/`%lo(D_80026208)', `jal GetTPage' and
   `jal RotAverage3' -- so every scratchpad displacement is byte-identical and
   nothing is hidden by the mask. `relcand' agrees on all four symbols.

   DONORS: func_s22_8002445C in csrc/ovl_dng_s22.c for the scratchpad-struct
   shape, the `(u32)sp->unk_04 < 0x1E0' gate and the addPrim chain, and
   func_s14_800246D8 in csrc/ovl_dng_s14.c for the ARGUMENT FORM. s22's
   comment states its eight RotAverage3 addresses are `addiu $aN, $s4, imm'
   off `&sp->unk_XX'; this copy is the OTHER polarity and wants s14's
   `(SVECTOR *)((u32)sp | 0x10)', which is what the listing's
   `addu $aN, $s4, $zero' / `ori $aN, $aN, imm' pairs are. Also unlike s22,
   `D_80083160.unk_00' is read DIRECT into one `u8 *base' local (one `lw %lo'
   in the listing), not through a `&D_80083160' pointer local.

   THE ONE PRICED FACT, and it was the whole residue: THE SIX UV STORES GO IN
   PER-VERTEX ORDER `u0, v0, u1, v1, u2, v2'. Measured at 176 words:
     per-vertex `u0,v0,u1,v1,u2,v2' before the colours          176 w,  0
     `u0,v1' first and `v0,u1,u2,v2' after the colour stores    176 w, 10
   The 10 rows are the two `addiu' constant materialisations moving 14 words
   and the four `sb' permuting; nothing else in the body changes. The colour
   stores themselves are `r0,g0,r1,g1,r2,g2' then `b0,b1,b2', which is what
   the listing's `sra 16' / `sra 8' pairs followed by three bare `sb $sN' show.

   The 0x1F800000 struct is a POINTER LOCAL ($s4 holds the base) and the
   scratchpad fields are STRUCT FIELDS rather than plain casts; that is what
   lets the single `lw %lo(D_80026208)' survive nine `sh' stores, since
   true_dependence (sched.c:835) excuses a non-varying, non-in-struct load
   against a varying in-struct store. A plain-cast spelling would reload it. */
s32 func_s60_800244B0(s32 a, s32 b, s32 c)
{
    UnkDngS60PolySP *sp = (UnkDngS60PolySP *)0x1F800000;
    u8 *base;
    POLY_GT3 *l;

    base = D_80083160.unk_00;
    sp->unk_00 = base + 0xB0;
    l = *(POLY_GT3 **)(base + 0x8D0);
    *(POLY_GT3 **)(base + 0x8D0) = l + 1;
    setPolyGT3(l);
    setSemiTrans(l, 1);
    l->tpage = GetTPage(0, 3, 0x300, 0x100);
    l->clut = 0x7DCF;
    l->u0 = 0xC0;
    l->v0 = 0x7F;
    l->u1 = 0xFF;
    l->v1 = 0x40;
    l->u2 = 0xFF;
    l->v2 = 0x7F;
    l->r0 = a >> 16;
    l->g0 = a >> 8;
    l->r1 = b >> 16;
    l->g1 = b >> 8;
    l->r2 = c >> 16;
    l->g2 = c >> 8;
    l->b0 = a;
    l->b1 = b;
    l->b2 = c;
    sp->unk_10 = (sp->unk_30 + *(s32 *)(D_80026208 + 0x0)) >> 16;
    sp->unk_18 = (sp->unk_40 + *(s32 *)(D_80026208 + 0x0)) >> 16;
    sp->unk_20 = (sp->unk_50 + *(s32 *)(D_80026208 + 0x0)) >> 16;
    sp->unk_12 = (sp->unk_34 + *(s32 *)(D_80026208 + 0x4)) >> 16;
    sp->unk_1A = (sp->unk_44 + *(s32 *)(D_80026208 + 0x4)) >> 16;
    sp->unk_22 = (sp->unk_54 + *(s32 *)(D_80026208 + 0x4)) >> 16;
    sp->unk_14 = (sp->unk_38 + *(s32 *)(D_80026208 + 0x8)) >> 16;
    sp->unk_1C = (sp->unk_48 + *(s32 *)(D_80026208 + 0x8)) >> 16;
    sp->unk_24 = (sp->unk_58 + *(s32 *)(D_80026208 + 0x8)) >> 16;
    sp->unk_04 = RotAverage3((SVECTOR *)((u32)sp | 0x10), (SVECTOR *)((u32)sp | 0x18),
                             (SVECTOR *)((u32)sp | 0x20), (long *)((u32)sp | 0x70), (long *)((u32)sp | 0x74),
                             (long *)((u32)sp | 0x78), (long *)((u32)sp | 0x8), (long *)((u32)sp | 0xC));
    l->x0 = sp->unk_70;
    l->y0 = sp->unk_72;
    l->x1 = sp->unk_74;
    l->y1 = sp->unk_76;
    l->x2 = sp->unk_78;
    l->y2 = sp->unk_7A;
    if ((u32)sp->unk_04 < 0x1E0)
    {
        addPrim(sp->unk_00 + sp->unk_04 * 4, l);
    }
    return 0;
}

/* The same POLY_GT3 push as func_s60_800244B0 above with a different texture
   window: u0/v0 = 0xC0/0x7F, u1/v1 = 0xFF/0x40, u2/v2 = 0xC0/0x40.
   176 words, 0 differing, FIRST COMPILE -- func_s60_800244B0's body with four
   constants changed. ADDRESSES A CONSTANT BASE (0x1F800000); `--raw' gives the
   same 6 relocation-only differences as its sibling.
   Nothing here is separately measured; the UV-order fact is priced on
   func_s60_800244B0 and it carried. */
s32 func_s60_80024770(s32 a, s32 b, s32 c)
{
    UnkDngS60PolySP *sp = (UnkDngS60PolySP *)0x1F800000;
    u8 *base;
    POLY_GT3 *l;

    base = D_80083160.unk_00;
    sp->unk_00 = base + 0xB0;
    l = *(POLY_GT3 **)(base + 0x8D0);
    *(POLY_GT3 **)(base + 0x8D0) = l + 1;
    setPolyGT3(l);
    setSemiTrans(l, 1);
    l->tpage = GetTPage(0, 3, 0x300, 0x100);
    l->clut = 0x7DCF;
    l->u0 = 0xC0;
    l->v0 = 0x7F;
    l->u1 = 0xFF;
    l->v1 = 0x40;
    l->u2 = 0xC0;
    l->v2 = 0x40;
    l->r0 = a >> 16;
    l->g0 = a >> 8;
    l->r1 = b >> 16;
    l->g1 = b >> 8;
    l->r2 = c >> 16;
    l->g2 = c >> 8;
    l->b0 = a;
    l->b1 = b;
    l->b2 = c;
    sp->unk_10 = (sp->unk_30 + *(s32 *)(D_80026208 + 0x0)) >> 16;
    sp->unk_18 = (sp->unk_40 + *(s32 *)(D_80026208 + 0x0)) >> 16;
    sp->unk_20 = (sp->unk_50 + *(s32 *)(D_80026208 + 0x0)) >> 16;
    sp->unk_12 = (sp->unk_34 + *(s32 *)(D_80026208 + 0x4)) >> 16;
    sp->unk_1A = (sp->unk_44 + *(s32 *)(D_80026208 + 0x4)) >> 16;
    sp->unk_22 = (sp->unk_54 + *(s32 *)(D_80026208 + 0x4)) >> 16;
    sp->unk_14 = (sp->unk_38 + *(s32 *)(D_80026208 + 0x8)) >> 16;
    sp->unk_1C = (sp->unk_48 + *(s32 *)(D_80026208 + 0x8)) >> 16;
    sp->unk_24 = (sp->unk_58 + *(s32 *)(D_80026208 + 0x8)) >> 16;
    sp->unk_04 = RotAverage3((SVECTOR *)((u32)sp | 0x10), (SVECTOR *)((u32)sp | 0x18),
                             (SVECTOR *)((u32)sp | 0x20), (long *)((u32)sp | 0x70), (long *)((u32)sp | 0x74),
                             (long *)((u32)sp | 0x78), (long *)((u32)sp | 0x8), (long *)((u32)sp | 0xC));
    l->x0 = sp->unk_70;
    l->y0 = sp->unk_72;
    l->x1 = sp->unk_74;
    l->y1 = sp->unk_76;
    l->x2 = sp->unk_78;
    l->y2 = sp->unk_7A;
    if ((u32)sp->unk_04 < 0x1E0)
    {
        addPrim(sp->unk_00 + sp->unk_04 * 4, l);
    }
    return 0;
}

/* The four-vertex version: one semi-transparent POLY_GT4 for the four VECTORs
   at scratchpad +0x30/+0x40/+0x50/+0x60, projected with RotAverage4 (ten
   arguments, so six go on the stack), with one packed colour per vertex.
   213 words, 0 differing, FIRST COMPILE off func_s60_800244B0.
   ADDRESSES A CONSTANT BASE (0x1F800000); `ovlverify --raw' reports 6
   differing words and all six are relocations (`D_80083160', `D_80026208',
   `jal GetTPage', `jal RotAverage4') -- no displacement differs.
   The UV order is per-vertex `u0,v0,u1,v1,u2,v2,u3,v3' exactly as its
   three-vertex sibling prices it at 10 aligned differences; this body shares
   the UnkDngS60PolySP typedef, which covers both layouts. */
s32 func_s60_80024A30(s32 a, s32 b, s32 c, s32 d)
{
    UnkDngS60PolySP *sp = (UnkDngS60PolySP *)0x1F800000;
    u8 *base;
    POLY_GT4 *l;

    base = D_80083160.unk_00;
    sp->unk_00 = base + 0xB0;
    l = *(POLY_GT4 **)(base + 0x8D0);
    *(POLY_GT4 **)(base + 0x8D0) = l + 1;
    setPolyGT4(l);
    setSemiTrans(l, 1);
    l->tpage = GetTPage(0, 3, 0x300, 0x100);
    l->clut = 0x7DCF;
    l->u0 = 0xC0;
    l->v0 = 0x40;
    l->u1 = 0xFF;
    l->v1 = 0x40;
    l->u2 = 0xC0;
    l->v2 = 0x7F;
    l->u3 = 0xFF;
    l->v3 = 0x7F;
    l->r0 = a >> 16;
    l->g0 = a >> 8;
    l->r1 = b >> 16;
    l->g1 = b >> 8;
    l->r2 = c >> 16;
    l->g2 = c >> 8;
    l->r3 = d >> 16;
    l->g3 = d >> 8;
    l->b0 = a;
    l->b1 = b;
    l->b2 = c;
    l->b3 = d;
    sp->unk_10 = (sp->unk_30 + *(s32 *)(D_80026208 + 0x0)) >> 16;
    sp->unk_18 = (sp->unk_40 + *(s32 *)(D_80026208 + 0x0)) >> 16;
    sp->unk_20 = (sp->unk_50 + *(s32 *)(D_80026208 + 0x0)) >> 16;
    sp->unk_28 = (sp->unk_60 + *(s32 *)(D_80026208 + 0x0)) >> 16;
    sp->unk_12 = (sp->unk_34 + *(s32 *)(D_80026208 + 0x4)) >> 16;
    sp->unk_1A = (sp->unk_44 + *(s32 *)(D_80026208 + 0x4)) >> 16;
    sp->unk_22 = (sp->unk_54 + *(s32 *)(D_80026208 + 0x4)) >> 16;
    sp->unk_2A = (sp->unk_64 + *(s32 *)(D_80026208 + 0x4)) >> 16;
    sp->unk_14 = (sp->unk_38 + *(s32 *)(D_80026208 + 0x8)) >> 16;
    sp->unk_1C = (sp->unk_48 + *(s32 *)(D_80026208 + 0x8)) >> 16;
    sp->unk_24 = (sp->unk_58 + *(s32 *)(D_80026208 + 0x8)) >> 16;
    sp->unk_2C = (sp->unk_68 + *(s32 *)(D_80026208 + 0x8)) >> 16;
    sp->unk_04 = RotAverage4((SVECTOR *)((u32)sp | 0x10), (SVECTOR *)((u32)sp | 0x18),
                             (SVECTOR *)((u32)sp | 0x20), (SVECTOR *)((u32)sp | 0x28),
                             (long *)((u32)sp | 0x70), (long *)((u32)sp | 0x74), (long *)((u32)sp | 0x78),
                             (long *)((u32)sp | 0x7C), (long *)((u32)sp | 0x8), (long *)((u32)sp | 0xC));
    l->x0 = sp->unk_70;
    l->y0 = sp->unk_72;
    l->x1 = sp->unk_74;
    l->y1 = sp->unk_76;
    l->x2 = sp->unk_78;
    l->y2 = sp->unk_7A;
    l->x3 = sp->unk_7C;
    l->y3 = sp->unk_7E;
    if ((u32)sp->unk_04 < 0x1E0)
    {
        addPrim(sp->unk_00 + sp->unk_04 * 4, l);
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s60", func_s60_80024D84);

/* The step of the node func_s60_80025340 spawns: bump the owning node's +0x1A
   counter and this instance's +0x6 frame, dispatch `*(s16 *)(p + 0x4)' through
   jtbl_s60_80024008, then -- for every case but 4 -- walk the thirty-entry
   triple array in the payload, adding the +0x8 drift to each +0x46 entry,
   bumping each +0xA entry by `1 + i / 6' and, from i = 6 on, sliding each
   +0x82 entry by `5 - i' (or by -0x13 from i = 0x18). Case 4 retires the actor
   and returns. 86 words, 0 differing. A LEAF: no frame, no $ra save.

   THIS FUNCTION OWNS jtbl_s60_80024008. FIVE CASES, MINVAL 0, ONE PAD WORD.
   The dispatch is a bare `sltiu $v0, $v1, 0x5' with NO subtraction before it,
   so the case set is {0,1,2,3,4} and entry k is case k. Entries, all five
   distinct and ascending, so the arms are written ascending 0..4:
     0 .Ls60_8002523C   1 .Ls60_8002525C   2 .Ls60_80025268
     3 .Ls60_80025274   4 .Ls60_8002529C   + .word 0 pad
   THERE IS NO DEFAULT AMONG THE ENTRIES: the range check's
   `beqz $v0, .Ls60_800252C0' goes to the function's COMMON TAIL, which is not
   any entry's label, so all five entries are real arms. An explicit
   `default: break;' measures 86 words / 0 differing -- FREE, and that is the
   confirmation, not a reason to write it.
   Cases 1, 2 and 3 are the same gate on three different bounds (0xC, 4, 8) and
   case 0's own copy of the 8-bound gate is what jump2 cross-joins into case 3
   (`j .Ls60_80025280' from cases 1 and 2, case 0's `j .Ls60_80025288').

   THREE PRICED FACTS, all re-measured from the 0 state:
   | spelling                                                 | words | rows |
   |----------------------------------------------------------|-------|------|
   | the tail loop INDEXED, `*(s16 *)(p + 0x46 + i * 2)'      |   86  |   0  |
   |   ... a `q = p' cursor with `q += 2' at the bottom       |   86  |   7  |
   | `k + (5 - i)' -- the subtraction BRACKETED               |   86  |   0  |
   |   ... `k + 5 - i'                                        |   86  |   2  |
   | the +0x82 store written out in BOTH arms off `k'         |   86  |   0  |
   |   ... assigned back to `k' and stored once after         |   86  |   3  |
   The cursor row is the loop-base lever from the other side: with three
   offsets off one incremented pointer, loop.c initialises the biv at
   `p + 0x82' and every displacement comes out negative (`addiu a1,a3,130',
   `lhu v0,-60(a1)'); written indexed, combine_givs leaves the base at `p' and
   the target's `addu $a1, $a3, $zero' appears. `k + 5 - i' is fold-const's
   `associate:' arm -- split_tree succeeds on the `VAR + CON' left operand and
   reassociates to `k - (i - 5)', giving `addiu v1,a2,-5' where the target has
   `addiu $v0, $v1, 0x5'; bracketing the `5 - i' keeps the constant on the
   load's side. The third row is a pseudo count: the target holds `k' in $v1
   and the result in $v0, which only two separate stores give. */
void func_s60_800251E8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 i;
    s32 k;

    *(s16 *)(*(u8 **)(p + 0x0) + 0x1A) = *(u16 *)(*(u8 **)(p + 0x0) + 0x1A) + 1;
    *(s16 *)(p + 0x6) = *(u16 *)(p + 0x6) + 1;
    switch (*(s16 *)(p + 0x4))
    {
    case 0:
        *(s16 *)(p + 0x8) = *(u16 *)(p + 0x8) - 0xC0;
        if (*(s16 *)(p + 0x6) >= 8)
        {
            *(s16 *)(p + 0x6) = 0;
            *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
        }
        break;
    case 1:
        if (*(s16 *)(p + 0x6) >= 0xC)
        {
            *(s16 *)(p + 0x6) = 0;
            *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
        }
        break;
    case 2:
        if (*(s16 *)(p + 0x6) >= 4)
        {
            *(s16 *)(p + 0x6) = 0;
            *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x6) >= 8)
        {
            *(s16 *)(p + 0x6) = 0;
            *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
        }
        break;
    case 4:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return;
    }
    for (i = 0; i < 30; i++)
    {
        *(s16 *)(p + 0x46 + i * 2) = *(u16 *)(p + 0x46 + i * 2) + *(u16 *)(p + 0x8);
        *(s16 *)(p + 0xA + i * 2) = *(u16 *)(p + 0xA + i * 2) + 1 + i / 6;
        if (i >= 6)
        {
            k = *(s16 *)(p + 0x82 + i * 2);
            if (i < 0x18)
            {
                *(s16 *)(p + 0x82 + i * 2) = k + (5 - i);
            }
            else
            {
                *(s16 *)(p + 0x82 + i * 2) = k - 0x13;
            }
        }
    }
}

/* Spawn the 0x12 node that runs func_s60_800251E8 and draws through
   func_s60_80024D84: carry the caller's owner record forward from its own
   +0x20, zero the payload's +0x4..+0x8 header, then seed thirty triples --
   +0xA[i] = 0, +0x46[i] = `(i % 6) * 682 + rand() % 33 - 0x10' (682 is
   0x1000/6, one sixth of a turn per column) and +0x82[i] = `rand() % 9 - 4'.
   83 words, 0 differing, FIRST COMPILE.

   The three magic multiplies are read off the SHIFT, not the constant:
   0x2AAAAAAB with no shift after `mfhi' is `/ 6' (the `% 6' column index),
   0x3E0F83E1 with `sra 3' is `/ 33', and 0x38E38E39 with `sra 1' is `/ 9'
   (the same constant is the magic for `/ 72' at `sra 4').
   The loop is INDEXED off the payload pointer, `*(s16 *)(t + 0x46 + i * 2)',
   the same form func_s60_800251E8 prices at 7 aligned differences against a
   cursor -- and the two bodies walk the same three arrays.

   ONE PRICED FACT: the +0x46 seed is `i % 6 * 682 + rand() % 33 - 0x10' with
   the `- 0x10' OUTSIDE the modulo's parentheses. Measured at 83 words:
     `i % 6 * 682 + rand() % 33 - 0x10'                        83 w,  0
     `i % 6 * 682 + (rand() % 33 - 0x10)'                      83 w,  8
   Same fold-const `associate:' mechanism as func_s60_800251E8's bracketing
   fact with the sign flipped: here the constant must stay at the TOP of the
   sum so the `addiu -0x10' lands on the accumulated product, and bracketing
   it onto the modulo puts it on the `rand()' result instead. */
void func_s60_80025340(u8 *p)
{
    UnkNode *n;
    u8 *t;
    s32 i;

    n = func_8003FD64(0x212, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s60_800251E8;
        t = n->payload;
        *(u8 **)((u8 *)n + 0x20) = *(u8 **)(p + 0x20);
        *(s16 *)(t + 0x4) = 0;
        *(s16 *)(t + 0x6) = 0;
        *(s16 *)(t + 0x8) = 0;
        for (i = 0; i < 30; i++)
        {
            *(s16 *)(t + 0xA + i * 2) = 0;
            *(s16 *)(t + 0x46 + i * 2) = i % 6 * 682 + rand() % 33 - 0x10;
            *(s16 *)(t + 0x82 + i * 2) = rand() % 9 - 4;
        }
        func_8004491C(n, (u32)func_s60_80024D84);
    }
}

/* The 0x212 sprite's own step function: bump the owning node's +0x14 frame
   counter, hand the renderable to func_800478B8, and when its +0x14 reports
   0x6000 mark the owning actor retired and set the same bit in D_800814A0.
   31 words, 0 differing.

   This is the landed func_s10_80024374 in csrc/ovl_dng_s10.c (0.862 by
   dngtwin.py) plus the four-word counter bump on `*(u8 **)(p + 0x0)' at the
   front; that twin's note about D_800814A0 being the PLAIN scalar rather than
   an array alias holds here too and is what gives the `lui' / `lw %lo' pair.
   Measured and FREE at 31 words / 0 differing, so it is NOT the reason this
   lands: reading the counter back through `*(s16 *)' instead of `*(u16 *)'
   for the `+ 1'. 
   THIS COPY'S OWN CONSTANT: 0x14 -> 0x1a at 2 of 3 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s60_8002548C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)(*(u8 **)(p + 0x0) + 0x1a) = *(u16 *)(*(u8 **)(p + 0x0) + 0x1a) + 1;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212 node that runs the already-landed func_s60_8002548C: park
   the caller's actor at the node's +0x20, dress the renderable white out of
   D_800DEB70, scale it 0x1800 square, raise the +0x14 0xC and +0x10 0x20 bits,
   publish the caller's x and y into the node's own UnkVec and bias its z down
   by half the D_800DDC40 footprint for the owner's +0x13 kind, shifted into
   the high half. 65 words, 0 differing, FIRST COMPILE.

   dngtwin's best was func_s23_80024264 at 0.712 (csrc/ovl_dng_s23.c); what
   this is actually built from is that body's priced table plus
   func_s44_80025270 in csrc/ovl_dng_s44.c. s23's dress-site fact carries
   verbatim -- `q = n->unk_08' is ONE pointer local with every field written
   off it, which is the brief's "sites ON ONE PATH" case -- and s44 supplies
   the `+0x14 then +0x10' order of the two `|='.

   TWO PRICED FACTS, re-measured from the 0 state:
   | spelling                                                 | words | rows |
   |----------------------------------------------------------|-------|------|
   | `|= 0xC' on +0x14 BEFORE `|= 0x20' on +0x10              |   65  |   0  |
   |   ... the other order                                    |   65  |   7  |
   | the z bias as `(tbl[k] >> 1) << 16'                      |   65  |   0  |
   |   ... as `tbl[k] << 15' (csrc/ovl_dng_b29.c's spelling)  |   64  |   7  |
   The second row is the diagnostic worth keeping: TWO SHIFTS in the listing
   (`srl 1' then `sll 16') mean two shifts in the source. `<< 15' folds them
   into one instruction, so the body comes out a word SHORT -- and it is not
   the same arithmetic either, since `>> 1 << 16' drops the low bit. */
void func_s60_80025508(u8 *p, UnkVec *b)
{
    UnkNode *n;
    u8 *m;
    UnkVec *q;

    n = func_8003FD64(0x212, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s60_8002548C;
        m = (u8 *)n->unk_0C;
        *(u8 **)((u8 *)n + 0x20) = p;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        func_8003DB94(m, D_800DEB70, 0);
        *(s16 *)(m + 0x1E) = 0x1800;
        *(s16 *)(m + 0x1C) = 0x1800;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x20;
        q = n->unk_08;
        q->unk_00 = b->unk_00;
        q->unk_04 = b->unk_04;
        q->unk_08 = b->unk_08 - ((D_800DDC40[(*(u8 **)(p + 0x14))[0x13]] >> 1) << 16);
        func_8004491C(n, (u32)D_80045340);
    }
}

/* The step of the node func_s60_800257B8 spawns: age the +0x4 frame and fade
   the renderable by 0x10 a frame once it passes 9, advance the +0x8 radius by
   0x100 and the +0x6 spin by one, shrink the +0xA angle by `8 + rand() % 9',
   grow the sprite scale by 0x40 a frame while it is under 0x400, hand the
   renderable to func_800478B8, and then either retire the actor (when its
   +0x14 reports 0x6000) or place the mover on the D_80026208 origin at the
   +0x8 radius and +0x6 angle. 107 words, 0 differing, FIRST COMPILE.

   Written off func_s60_800257B8 above, which BUILDS the payload this reads --
   the same `(func_800644B8(angle) >> 4) * radius * 0x100' placement and the
   same `*(s32 *)(D_80026208 + K)' origin, so no offset needed guessing. That
   is the in-unit route and it is why this compiled first time.
   `sltiu $v0, $v0, 0x400' on the scale read is combine narrowing a signed
   compare of a zero-extended halfword, not an unsigned source compare.

   ONE PRICED FACT: the fade writes its three bytes from a LOCAL.
     `u = c[0xE] - 0x10; c[0xE] = u; c[0xD] = u; c[0xC] = u;'   107 w,  0
     `c[0xE] -= 0x10; c[0xD] = c[0xE]; c[0xC] = c[0xE];'        108 w,  3
   The read-back form is a word LONG: cse forwards the stored byte to the
   first re-read but the second needs its own `lbu'. */
void func_s60_8002560C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 u;

    *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
    if (*(s16 *)(p + 0x4) >= 0xA)
    {
        u = c[0xE] - 0x10;
        c[0xE] = u;
        c[0xD] = u;
        c[0xC] = u;
    }
    *(s16 *)(p + 0x8) = *(u16 *)(p + 0x8) + 0x100;
    *(s16 *)(p + 0x6) = *(u16 *)(p + 0x6) + 1;
    *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) - 8 - rand() % 9;
    if (*(u16 *)(c + 0x1C) < 0x400)
    {
        u = *(u16 *)(c + 0x1E) + 0x40;
        *(s16 *)(c + 0x1E) = u;
        *(s16 *)(c + 0x1C) = u;
    }
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    else
    {
        b->unk_00 =
            *(s32 *)(D_80026208 + 0x0) + (func_800644B8(*(s16 *)(p + 0x8)) >> 4) * *(s16 *)(p + 0x6) * 0x100;
        b->unk_04 =
            *(s32 *)(D_80026208 + 0x4) + (func_80064584(*(s16 *)(p + 0x8)) >> 4) * *(s16 *)(p + 0x6) * 0x100;
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)(D_80026208 + 0xA) + *(u16 *)(p + 0xA);
    }
}

/* Spawn the 0x212 node that runs func_s60_8002560C: seed its payload with a
   0x20 radius, a `rand() % 0x1000' bearing and a `-(rand() % 32)' tilt, dress
   the renderable white out of this object's own D_80026180 at 0x200 square
   with the +0x14 0xC and +0x10 0x20 bits, and place the node's UnkVec on the
   D_80026208 origin offset by sine and cosine of the bearing scaled by the
   radius, with the tilt added to the origin's +0xA.
   98 words, 0 differing, FIRST COMPILE. Takes NO arguments -- the listing sets
   `$a0 = 0x212' as its first instruction, so its callers'
   `jal func_s60_800257B8' with no argument setup is not an omission.

   Written off the landed 0x212 spawners in this bank (func_s44_80025270 in
   csrc/ovl_dng_s44.c for the `|= 0xC' / `|= 0x20' order and the dress
   sequence). The `sra $v0, $v0, 4' on each func_800644B8 result is a bare
   signed `>> 4' with NO bias correction in the listing, so it is a shift and
   not a `/ 16'.

   MEASURED AND FREE at 98 words / 0 differing, so NEITHER is why this lands:
     the product written `((... >> 4) * radius) << 8' rather than `* 0x100'
     the tilt written `0 - rand() % 32' rather than `-(rand() % 32)'
   The first narrows the `mult / mflo / sll K' lever: it bites when the SHIFT
   would have to become the multiply, and here the `sll 8' is a separate
   instruction after `mflo' either way, so both spellings give the identical
   three-instruction run. */
void func_s60_800257B8(void)
{
    UnkNode *n;
    u8 *t;
    u8 *m;
    u8 *v;

    n = func_8003FD64(0x212, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s60_8002560C;
        t = n->payload;
        *(s16 *)(t + 0x4) = 0;
        *(s16 *)(t + 0x6) = 0x20;
        *(s16 *)(t + 0x8) = rand() % 0x1000;
        *(s16 *)(t + 0xA) = -(rand() % 32);
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        func_8003DB94(m, D_80026180, 0);
        *(s16 *)(m + 0x1E) = 0x200;
        *(s16 *)(m + 0x1C) = 0x200;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x20;
        v = (u8 *)n->unk_08;
        *(s32 *)(v + 0x0) =
            *(s32 *)(D_80026208 + 0x0) + (func_800644B8(*(s16 *)(t + 0x8)) >> 4) * *(s16 *)(t + 0x6) * 0x100;
        *(s32 *)(v + 0x4) =
            *(s32 *)(D_80026208 + 0x4) + (func_80064584(*(s16 *)(t + 0x8)) >> 4) * *(s16 *)(t + 0x6) * 0x100;
        *(s16 *)(v + 0xA) = *(u16 *)(D_80026208 + 0xA) + *(u16 *)(t + 0xA);
        func_8004491C(n, (u32)D_80045340);
    }
}

/* The step of the node func_s60_80025A9C spawns: bump the owning node's +0x1A
   counter and this instance's +0x4E frame, then dispatch `*(s16 *)(p + 0x4C)'
   through jtbl_s60_80024020. Cases 0, 1 and 2 each fire func_s60_800257B8
   twice; case 0 also drops the party record's +0x2A heading by 0x200, case 1
   advances the three-word position at +0x34 by the velocity at +0x40, and each
   of cases 0..3 rolls the state on once its frame counter passes a per-state
   bound (8, 0xC, 4, 8). Case 4 retires the actor. 87 words, 0 differing,
   FIRST COMPILE.

   THIS FUNCTION OWNS jtbl_s60_80024020. FIVE CASES, MINVAL 0, ONE PAD WORD.
   `sltiu $v0, $v1, 0x5' with no subtraction, so the set is {0,1,2,3,4}:
     0 .Ls60_800259A0   1 .Ls60_800259D0   2 .Ls60_80025A24
     3 .Ls60_80025A40   4 .Ls60_80025A68   + .word 0 pad
   All five entries distinct and ascending, so the arms are written ascending.
   NO DEFAULT AMONG THE ENTRIES -- the range check's
   `beqz $v0, .Ls60_80025A88' goes straight to the EPILOGUE, which is no
   entry's label; an explicit `default: break;' measures 87 / 0, FREE.
   Case 0's `j .Ls60_80025A40' is NOT a shared arm: case 0 writes out the same
   8-bound gate as case 3 and jump2 cross-joins its whole trailing block into
   case 3's. The same cross-join folds cases 1, 2 and 3 onto one
   `.Ls60_80025A54' advance block.

   MEASURED AND FREE at 87 words / 0 differing, so it is NOT why this lands:
   the heading drop written `*(s16 *)(D_800814A8 + 0x2A) -= 0x200' instead of
   the explicit read-modify-write. (csrc/ovl_dng_b05.c prices a POINTER LOCAL
   for D_800814A8 on its own body; this copy reads the global twice in one
   basic block and cse unifies them either way, so no local is needed.) */
void func_s60_80025940(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)(*(u8 **)(p + 0x0) + 0x1A) = *(u16 *)(*(u8 **)(p + 0x0) + 0x1A) + 1;
    *(s16 *)(p + 0x4E) = *(u16 *)(p + 0x4E) + 1;
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        func_s60_800257B8();
        func_s60_800257B8();
        *(s16 *)(D_800814A8 + 0x2A) = *(u16 *)(D_800814A8 + 0x2A) - 0x200;
        if (*(s16 *)(p + 0x4E) >= 8)
        {
            *(s16 *)(p + 0x4E) = 0;
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 1:
        func_s60_800257B8();
        func_s60_800257B8();
        *(s32 *)(p + 0x34) = *(s32 *)(p + 0x34) + *(s32 *)(p + 0x40);
        *(s32 *)(p + 0x38) = *(s32 *)(p + 0x38) + *(s32 *)(p + 0x44);
        *(s32 *)(p + 0x3C) = *(s32 *)(p + 0x3C) + *(s32 *)(p + 0x48);
        if (*(s16 *)(p + 0x4E) >= 0xC)
        {
            *(s16 *)(p + 0x4E) = 0;
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 2:
        func_s60_800257B8();
        func_s60_800257B8();
        if (*(s16 *)(p + 0x4E) >= 4)
        {
            *(s16 *)(p + 0x4E) = 0;
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x4E) >= 8)
        {
            *(s16 *)(p + 0x4E) = 0;
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 4:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}

/* Spawn the 0x212 node that runs func_s60_80025940: park the caller's actor at
   +0x20, zero the state and frame at the payload's +0x4C/+0x4E, copy the start
   vector `b' into both the +0x1C target and the +0x34 cursor, take each end's
   height from func_800BCAD0 (falling back to `b->unk_08' when it reads over
   0x200), then set the three +0x40 velocities to a twelfth of the span and
   publish the cursor's address in D_80026208 -- which is what
   func_s60_800257B8 and func_s60_8002560C read as their world origin.
   99 words, 0 differing.

   No twin at any ratio; written from the unit's other spawners. The magic
   0x2AAAAAAB appears three times with `sra $vX, $vY, 1' after `mfhi', so the
   divisor is 12, not the 6 the same constant means at shift 0.

   THE RESIDUE WAS ENTIRELY IN THE THREE DIVISIONS AND IT TOOK A CONJUNCTION.
   The three `mult's are output-dependent through HI, so their order IS the
   source order and cannot be dismissed as a scheduling artefact: x (+0x40),
   then z (+0x48), then y (+0x44). But the target's `lw 0x4($s3)' /
   `lw 0x4($s1)' -- the y span's operands -- sit ABOVE `sw $v0, 0x48($s0)',
   and memrefs_conflict_p returns 1 for two PLUS(REG, CONST) addresses on
   different pseudos, so no spelling lets a load cross that store: the z STORE
   has to be LATER in the source than the y LOADS. That forces the z quotient
   into a local of its own. Measured at 99 words:
   | spelling                                                 | words | rows |
   |----------------------------------------------------------|-------|------|
   | z into `w', stored after the y statement, with           |   99  |   0  |
   |   `D_80026208' assigned between the z quotient and y     |       |      |
   |   ... all three divisions stored in place                |   99  |  45  |
   |   ... z into `w' but D_80026208 written first            |   99  |  16  |
   |   ... z into `w' but D_80026208 written last             |   99  |  18  |
   |   ... the z quotient reusing `u' instead of its own `w'  |   99  |  29  |
   | `u = b->unk_00' once for the +0x1C and +0x34 stores      |   99  |   0  |
   |   ... `b->unk_00' named at both sites                    |  103  |   6  |
   | the height gate assigning `u' then TWO stores after it   |   99  |   0  |
   |   ... both stores written out in both arms               |  102  |   6  |
   NONE of the four division rows reaches 0 on its own and the kept row is a
   PRODUCT: the local `w' at the wrong D_80026208 position is 16 or 18, and
   the right position with `u' reused is 29. A one-at-a-time sweep of either
   variable alone never gets below 16.
   The two four-word rows are one mechanism twice: a struct-field read named at
   two sites is RE-LOADED, because the plain-cast store between them is a
   conflicting varying MEM, so the target's single `lw' needs one local. On the
   height gate the local also buys the shape: with one store after the join,
   reorg lifts `sw $v0, 0x24($s0)' out of the target block into the `j''s delay
   slot and leaves the fall-through its own copy, which is the listing's
   duplicated +0x24 store with a SHARED +0x3C store below it. */
void func_s60_80025A9C(u8 *p, UnkVec *b, UnkVec *c)
{
    UnkNode *n;
    u8 *t;
    s32 h;
    s32 u;
    s32 w;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        *(u8 **)((u8 *)n + 0x20) = p;
        t = n->payload;
        n->unk_10 = (u32)func_s60_80025940;
        *(s16 *)(t + 0x4C) = 0;
        *(s16 *)(t + 0x4E) = 0;
        u = b->unk_00;
        *(s32 *)(t + 0x1C) = u;
        *(s32 *)(t + 0x34) = u;
        u = b->unk_04;
        *(s32 *)(t + 0x20) = u;
        *(s32 *)(t + 0x38) = u;
        h = func_800BCAD0((u8 *)b);
        if (h > 0x200)
        {
            u = b->unk_08;
        }
        else
        {
            u = h << 16;
        }
        *(s32 *)(t + 0x24) = u;
        *(s32 *)(t + 0x3C) = u;
        *(s32 *)(t + 0x4) = c->unk_00;
        *(s32 *)(t + 0x8) = c->unk_04;
        h = func_800BCAD0((u8 *)c);
        if (h > 0x200)
        {
            *(s32 *)(t + 0xC) = b->unk_08;
        }
        else
        {
            *(s32 *)(t + 0xC) = h << 16;
        }
        *(s32 *)(t + 0x40) = (c->unk_00 - b->unk_00) / 12;
        w = (*(s32 *)(t + 0xC) - *(s32 *)(t + 0x24)) / 12;
        D_80026208 = (u32)(t + 0x34);
        *(s32 *)(t + 0x44) = (c->unk_04 - b->unk_04) / 12;
        *(s32 *)(t + 0x48) = w;
        func_s60_80025340((u8 *)n);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s60", func_s60_80025C28);
