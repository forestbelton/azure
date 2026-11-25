#include "common.h"

s32 func_s70_80024440(u8 *p);

void func_800478B8(u8 *);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s70", func_s70_8002401C);

/* One semi-transparent POLY_FT4 beam per node of the tag chain: project the
   SVECTOR at the record's +0x1E/+0x22/+0x26, stepped down by half the +0x4E
   angle, twice through RotTransPers into a two-element DVECTOR array, widen
   the quad by half the vertical span of the two projections, and hang it on
   OT row (z - 8) when that is under 0x1E0. The prim slot is 0x34 bytes of
   D_80083160's +0x8D0 cursor even though a POLY_FT4 is 0x28. 139 words, 0
   differing.

   NO TWIN AT ANY RATIO and the ratio files are empty for it. The donor was
   found by resolving the listing's three `jal' targets through
   splat/common_syms.txt -- func_80065420 is RotTransPers, func_800666F4 is
   SetPolyFT4, func_80066640 is SetSemiTrans -- and grepping the REAL names,
   which names csrc/ovl_dungeon_800D50FC.c's func_800DA93C (133 words), the
   same +0x8D0-cursor POLY_FT4 tag-chain walk. Its fact that A CHAINED
   ASSIGNMENT EMITS ITS STORES RIGHT TO LEFT carried verbatim and is what
   gives every `t->x0 = t->x1 = ...' pair its order here.

   THREE FACTS, all priced from this body at 0 differing:

   * THE PROJECTED PAIR MUST BE `DVECTOR xy[2]', NOT `s32 xy[2]' READ WITH
     PLAIN CASTS -- 133 words / 21 aligned differences the other way. The
     member reference is MEM_IN_STRUCT_P, so the in_struct stores into `t'
     kill it and the target's reload of `xy[0].vx' between the two x stores
     survives; through a plain cast off the frame it is a fixed-address
     scalar, `fixed_scalar_and_varying_struct_p' excuses it and cse collapses
     the two loads.
   * THE INNER LOOP MUST PASS `(long *)&xy[i]', NOT WALK A CURSOR LOCAL --
     140 words / 30 aligned differences with a cursor. The array ref lets
     loop.c reduce the address and hoist its base into the callee-saved
     register the target spends on it; a cursor local takes that register
     instead and the 0xFFFFFF addPrim mask is then hoisted out of the walk
     loop as well, which is the two extra prologue words.
   * `t->u2 = t->u3 = t->u0 + *(u8 *)(q + 0x44)' AND NOT A LOCAL HOLDING THE
     FIRST BYTE -- a local is 7 aligned differences, because the in_struct
     byte stores kill the plain-cast reload of `q + 0x40' while the forwarded
     value of the `t->u0' store survives. The ADDEND ORDER is load-bearing
     too: `*(u8 *)(q + 0x44) + t->u0' is 3 rows. That order measured FREE (7
     rows either way) while the local fault was still live -- an equal row
     count hiding a different fault. */
s32 func_s70_80024440(u8 *p)
{
    Unk80083160 *g = &D_80083160;
    u8 *q;
    UnkNode *n;
    POLY_FT4 *t;
    s32 i;
    s32 z;
    s32 k;
    SVECTOR sv;
    DVECTOR xy[2];
    s32 w;

    while (1)
    {
        q = p;
        sv.vx = *(u16 *)(p + 0x1E);
        sv.vy = *(u16 *)(p + 0x22);
        sv.vz = *(u16 *)(p + 0x26);
        sv.vz = sv.vz - ((s16) * (u16 *)(p + 0x4E) >> 1);
        for (i = 0; i < 2; i++)
        {
            z = RotTransPers(&sv, (long *)&xy[i], (long *)&w, (long *)&w) - 8;
            sv.vz += *(u16 *)(q + 0x4E);
        }
        k = (xy[0].vy - xy[1].vy) >> 1;
        if ((u32)z < 0x1E0)
        {
            t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x34;
            *(u32 *)&t->r0 = 0xA0A0A0;
            SetPolyFT4(t);
            SetSemiTrans((u8 *)t, 1);
            t->tpage = *(u16 *)(q + 0x50);
            t->clut = *(u16 *)(q + 0x52);
            t->x0 = t->x1 = xy[0].vx + k;
            t->x2 = t->x3 = xy[0].vx - k;
            t->y0 = t->y2 = xy[0].vy;
            t->y1 = t->y3 = xy[1].vy;
            t->u0 = t->u1 = *(u8 *)(q + 0x40);
            t->u2 = t->u3 = t->u0 + *(u8 *)(q + 0x44);
            t->v0 = t->v2 = *(u8 *)(q + 0x42);
            t->v1 = t->v3 = t->v0 + *(u8 *)(q + 0x46);
            addPrim((u32 *)(g->unk_00 + 0xB0) + z, t);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s70", func_s70_8002466C);

/* The sprite's per-frame step: set bit 0x8000 in the +0x52 halfword of the
   node at the actor's +0x0, hand the renderable to func_800478B8, and when
   its +0x14 reports 0x6000 mark the owning actor retired and set the same bit
   in D_800814A0. 31 words, 0 differing, first compile.

   TWIN: the landed func_s10_80024224 in csrc/ovl_dng_s10.c (dngtwin 0.968,
   31 words). The only delta is that the counted node is at the actor's +0x0
   with an in-place flag set on its +0x52, where the twin uses +0xC and a
   `+ 1' counter bump; the twin's note about D_800814A0 being the PLAIN scalar
   rather than ovl_main_8008A288's array alias holds here too and is what
   gives the `lui'/`lw %lo' pair.

   First compile, so nothing in it is measured. */
void func_s70_80024ACC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s70", func_s70_80024B48);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s70", func_s70_80024C14);
