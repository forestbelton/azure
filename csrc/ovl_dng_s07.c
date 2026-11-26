#include "common.h"

void func_800478B8(u8 *);
s32 func_8004491C(UnkNode *, u32);
extern u8 D_80045340[];
extern u8 D_800DEC70[];
extern u8 D_800DED28[];
extern u32 D_800814A0;

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s07", func_s07_80024020);

/* One semi-transparent POLY_FT4 beam per node of the tag chain, this bank's
   variant: project the caller's UnkVec high halfwords as an SVECTOR twice
   through RotTransPers -- 0xE below the position and then 0x1C higher --
   widen the quad by half the vertical span of the two projections, bump the
   payload's +0x52 animation counter and wrap it to 8 at 0xC, dress the quad
   0x808080 with GetTPage(0, 1, 0x2C0, 0x100) / GetClut(0, 0x1F8) and UVs
   derived from +0x52, and hang it on OT row (z - 8) when that is under
   0x1E0. 152 words, 0 differing, FIRST COMPILE. 1 copy (dngfam.py).

   DONOR: the landed func_s36_80024798 in csrc/ovl_dng_s36.c, found by
   tools/decomp/dngcallee.py at 6 of 6 symbols with a clean gap to the 5/6
   tier (dngtwin independently ranks it 0.929 at 145 words). Its frame is
   byte-identical to this one -- -0x50 with the same nine saves in the same
   order -- and ALL FIVE of its facts carried. The deltas to it are the four
   constants above, the POLY_FT4 size 0x28 against its 0x34, the u0/u1 pair
   being the bare `c' here where s36 has `c - 0x80', and the new +0x52
   counter block.

   THREE FACTS RE-PRICED ON THIS BODY, all with aligndiff (figures include
   register-only rows):

   | spelling                                             | words | aligned |
   | as written below                                     |  152  |    0    |
   | `k = (xy[0].vy - xy[1].vy) >> 1' into a plain local, |  150  |    8    |
   |   s36's donor s70's spelling, with `k' in the x      |       |         |
   |   stores and no write-back to the scratch            |       |         |
   | the +0x52 counter block BEFORE `k = ...'             |  154  |   14    |
   | the two UV subexpressions written out twice instead  |  161  |   19    |
   |   of through the shared local `c'                    |       |         |

   So s36's INVERSION of s70 holds here too, at 2 words / 8 rows rather than
   its 3 words / 79: the half-span must be stored into the ADDRESS-TAKEN
   RotTransPers scratch (`w = k >> 1', `w' being the `(long *)&w' dummy
   passed as both out-parameters) and read back from the frame -- the `sw' to
   0x20($sp) followed by two `lhu' of the same slot is a variable, not a
   spill. And s36's shared-UV-local fact holds at 9 words / 19 rows.

   The +0x52 counter is the one piece with no donor. It reads as
   `*(s16 *)(q + 0x52) += 1;' then a separate `if (... >= 0xC)' on the SAME
   lvalue: cse forwards the stored halfword to the test, which is why the
   listing sign-extends a register (`sll 16' / `sra 16') instead of issuing a
   second `lh'. */
s32 func_s07_800248F8(u8 *p, UnkVec *v)
{
    Unk80083160 *g = &D_80083160;
    u8 *q;
    UnkNode *n;
    POLY_FT4 *t;
    s32 i;
    s32 z;
    s32 k;
    s32 c;
    SVECTOR sv;
    DVECTOR xy[2];
    s32 w;

    while (1)
    {
        q = p;
        sv.vx = *(u16 *)((u8 *)v + 0x2);
        sv.vy = *(u16 *)((u8 *)v + 0x6);
        sv.vz = *(u16 *)((u8 *)v + 0xA) - 0xE;
        for (i = 0; i < 2; i++)
        {
            z = RotTransPers(&sv, (long *)&xy[i], (long *)&w, (long *)&w) - 8;
            sv.vz += 0x1C;
        }
        k = xy[0].vy - xy[1].vy;
        *(s16 *)(q + 0x52) += 1;
        if (*(s16 *)(q + 0x52) >= 0xC)
        {
            *(s16 *)(q + 0x52) = 8;
        }
        if ((u32)z < 0x1E0)
        {
            t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x28;
            *(u32 *)&t->r0 = 0x808080;
            SetPolyFT4(t);
            SetSemiTrans((u8 *)t, 1);
            t->tpage = GetTPage(0, 1, 0x2C0, 0x100);
            t->clut = GetClut(0, 0x1F8);
            w = k >> 1;
            t->x0 = t->x1 = xy[0].vx + w;
            t->x2 = t->x3 = xy[0].vx - w;
            t->y0 = t->y2 = xy[0].vy;
            t->y1 = t->y3 = xy[1].vy;
            c = (*(u8 *)(q + 0x52) & 3) * 0x20;
            t->u0 = t->u1 = c;
            t->u2 = t->u3 = c + 0x1F;
            c = (*(s16 *)(q + 0x52) >> 2) * 0x20;
            t->v0 = t->v2 = c - 0x80;
            t->v1 = t->v3 = c - 0x61;
            addPrim((u32 *)(g->unk_00 + 0xB0) + z, t);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
    }
    return 0;
}

/* The step function func_s07_80024020 installs on the ten 0x312 smoke nodes
   it spawns: advance the node's own UnkVec by its velocity half, raise
   0x8000 in the owning actor's +0x10 word, and zero the velocity once the
   node's position rounds to the payload's target tile (+0x4/+0x6); then a
   three-state machine on +0x4C with the +0x48 countdown -- state 0 re-tags
   the node onto D_80045340 when the countdown runs out, state 1 either
   accelerates the fall and re-dresses the slot from D_800DEC70/D_800DED28 by
   the +0x48 parity or fades the slot's scale and greys, and state 2 (which
   state 1 falls through into) reports the node dead through the actor's -0x2
   word and D_800814A0. 164 words, 0 differing, fourth compile.

   NO CARVED TWIN at any ratio (dngtwin reports nothing for this stub).
   Written from the listing plus two donors found with
   tools/decomp/dngcallee.py at 4 of 6 symbols: csrc/ovl_dng_s26.c
   func_s26_800240D8, whose frame is identical (-0x20, s2/s1/ra/s0) and whose
   `case 3:' arm IS this body's state-2 tail verbatim -- func_800478B8(c),
   the `c->unk_14 & 0x6000' gate, `*(u16 *)(p - 0x2) |= 0x8000' and
   `D_800814A0 |= 0x8000' -- and csrc/ovl_dng_b30.c func_b30_80175F70 for the
   `func_8004491C(n, (u32)D_80045340)' re-tag and the `c[0xF]' countdown
   shape. The `*(s32 *)(*(u8 **)p + 0x10) |= 0x8000' head is s26's
   `*(s16 *)(*(u8 **)p + 0x14)' bump one field over.

   FOUR FACTS MEASURED ON THIS BODY, with aligndiff (figures include
   register-only rows):

   | spelling                                              | words | aligned |
   | as written below                                      |  164  |    0    |
   | the three velocity zero stores ASCENDING 0xC/0x10/0x14|  164  |    4    |
   | `c->unk_08 = c->unk_00->unk_04' with no pointer local |  166  |    5    |
   | `e' and the two arms storing c->unk_00 directly       |       |         |
   | `c->unk_08 = e->unk_04' written AFTER the two zero    |  165  |    6    |
   |   byte stores instead of before them                  |       |         |
   | `*(s16 *)(p + 0x4A) >= c->unk_0C[0]' instead of       |  164  |    2    |
   |   `c->unk_0C[0] <= *(s16 *)(p + 0x4A)'                |       |         |

   The two that cost words are the interesting pair and they are the SAME
   mechanism read from both ends. csrc/ovl_dungeon.c func_80099C58's comment
   states that `r->unk_08 = r->unk_00->unk_04' must be the FIRST of the slot
   stores; here the stronger form holds -- the element table must go into a
   POINTER LOCAL that both arms assign and the +0x8 store must read
   `e->unk_04' and not `c->unk_00->unk_04'. Through the field, gcc emits a
   real `lw $v0, 0x0($s1)' reload of the just-stored pointer where the target
   reads `lw $v0, 0x4($v0)' straight off the register, costing a word and a
   load-delay `nop'; and with the local but the load written last, sched
   cannot hoist it above the two in_struct byte stores and pays the `nop'
   anyway.

   The comparison operand order is the smallest and the easiest to get wrong:
   the emitted `slt $v0, $v0, $v1' does NOT tell you which side the C wrote,
   and naming `c->unk_0C[0]' first is what puts its `lbu' ahead of the
   `lh 0x4A' -- 2 rows the other way with every instruction otherwise in
   place.

   NOT MEASURABLE, so do not read the form below as a measurement: state 1
   falling THROUGH into state 2 rather than duplicating state 2's block at
   its end. Both spell 164 words / 0 aligned differences, because jump2
   cross-joins the two copies into the single block the listing shows. The
   fall-through is the reading, not the result.
   MEASURED AND FREE: `*(u32 *)c->unk_0C *= 4' before or after the
   `c->unk_1C = c->unk_1E = 0xC00' pair (164 / 0 either way; sched1
   normalises the order, and the `lw' of +0xC is hoisted to the top of the
   block in both). */
void func_s07_80024B58(u8 *p, UnkVec *b, UnkSlot *c)
{
    UnkElem *e;

    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    *(s32 *)(*(u8 **)p + 0x10) |= 0x8000;
    if (*(s16 *)((u8 *)b + 0x2) / 64 == *(s16 *)(p + 0x4) && *(s16 *)((u8 *)b + 0x6) / 64 == *(s16 *)(p + 0x6))
    {
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
    }
    *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) - 1;
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        if (*(s16 *)(p + 0x48) <= 0)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 1:
        func_800478B8((u8 *)c);
        if ((c->unk_14 & 0x6000) != 0)
        {
            c->unk_04 = 0;
            c->unk_05 = 0;
        }
        if (c->unk_0C[0] <= *(s16 *)(p + 0x4A))
        {
            b->unk_0C *= 5;
            b->unk_10 *= 5;
            b->unk_14 *= 8;
            *(u32 *)c->unk_0C *= 4;
            c->unk_1C = c->unk_1E = 0xC00;
            if ((*(u16 *)(p + 0x48) & 1) != 0)
            {
                e = (UnkElem *)D_800DEC70;
            }
            else
            {
                e = (UnkElem *)D_800DED28;
            }
            c->unk_00 = e;
            c->unk_08 = e->unk_04;
            c->unk_04 = 0;
            c->unk_05 = 0;
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        else
        {
            c->unk_1C = c->unk_1E = c->unk_1E - 0x200;
            c->unk_0C[0] -= *(u8 *)(p + 0x4A);
            c->unk_0C[1] = c->unk_0C[2] = c->unk_0C[2] - (*(s16 *)(p + 0x4A) >> 2);
        }
    case 2:
        func_800478B8((u8 *)c);
        if ((c->unk_14 & 0x6000) != 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}
