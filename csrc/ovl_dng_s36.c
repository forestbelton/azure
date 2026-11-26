#include "common.h"

s32 rand(void);

typedef struct UnkDngS36Scale
{
    u8 unk_00[0x18];
} UnkDngS36Scale;
extern u8 D_s36_80024004[];
void func_800478B8(u8 *);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s36", func_s36_8002401C);

/* One semi-transparent POLY_FT4 beam per node of the tag chain, this bank's
   variant: project the caller's UnkVec high halfwords as an SVECTOR twice
   through RotTransPers -- 0x20 below the position and then 0x40 higher --
   widen the quad by half the vertical span of the two projections, dress it
   0xC0C0C0 with GetTPage(0, 1, 0x2C0, 0x100) / GetClut(0x20, 0x1F8) and UVs
   derived from the payload's +0x52 halfword, and hang it on OT row (z - 8)
   when that is under 0x1E0.  145 words, 0 differing, second compile.

   TWIN: the landed func_s70_80024440 in csrc/ovl_dng_s70.c (dngtwin 0.796,
   139 words), whose three facts all carry: the projected pair must be
   `DVECTOR xy[2]' and not a cast-read `s32[2]'; the inner loop must pass
   `(long *)&xy[i]' and not walk a cursor local (the target's $s0/$s7 base and
   cursor are what loop.c makes of the array ref, not evidence of a cursor in
   the source); and the chained assignments emit their stores RIGHT TO LEFT,
   which is where every `t->x0 = t->x1 = ...' pair's order comes from.

   TWO FACTS MEASURED HERE:
     * THE SHARED UV SUBEXPRESSION MUST BE A NAMED LOCAL.  `c = (*(u8 *)(q +
       0x52) & 3) * 0x20;' once, then `c - 0x80' and `c - 0x61', because the
       in_struct byte stores into the prim kill a reload of `q + 0x52'.
       Written out twice -- the obvious transcription -- it is 154 words /
       29 aligned differences, nine words of recomputation.
     * ONE INVERSION OF THE TWIN, worth 3 words / 79 differing.  s70 computes
       `k = (xy[0].vy - xy[1].vy) >> 1' into a plain local and uses `k' in the
       x stores.  Here the half-span must be stored into the ADDRESS-TAKEN
       RotTransPers scratch (`w = k >> 1', `w' being the `(long *)&w' dummy
       passed as both out-parameters) and read back from the frame: that is
       what the listing's `sw' to 0x20($sp) followed by two `lhu' of the same
       slot is.  It is a variable, not a spill.  The twin's own spelling is
       142 words / 79 differing here.
   MEASURED AND FREE: `*(s16 *)(q + 0x52) >> 2' in place of
   `(s16) * (u16 *)(q + 0x52) >> 2' (0 either way). */
s32 func_s36_80024798(u8 *p, UnkVec *v)
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
        sv.vz = *(u16 *)((u8 *)v + 0xA) - 0x20;
        for (i = 0; i < 2; i++)
        {
            z = RotTransPers(&sv, (long *)&xy[i], (long *)&w, (long *)&w) - 8;
            sv.vz += 0x40;
        }
        k = xy[0].vy - xy[1].vy;
        if ((u32)z < 0x1E0)
        {
            t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x34;
            *(u32 *)&t->r0 = 0xC0C0C0;
            SetPolyFT4(t);
            SetSemiTrans((u8 *)t, 1);
            t->tpage = GetTPage(0, 1, 0x2C0, 0x100);
            t->clut = GetClut(0x20, 0x1F8);
            w = k >> 1;
            t->x0 = t->x1 = xy[0].vx + w;
            t->x2 = t->x3 = xy[0].vx - w;
            t->y0 = t->y2 = xy[0].vy;
            t->y1 = t->y3 = xy[1].vy;
            c = (*(u8 *)(q + 0x52) & 3) * 0x20;
            t->u0 = t->u1 = c - 0x80;
            t->u2 = t->u3 = c - 0x61;
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

/* The thrown object's shadow/mover step: raise 0x8000 in the node's +0x10,
   count the +0x48 timer down and, while it is still positive, advance the
   payload's three +0x1C.. positions by its three +0x34.. velocities; then ease
   the caller's UnkVec a fifth of the way towards that position on each axis,
   hand the renderable to func_800478B8, clear its +0x04/+0x05 pair once its
   flags report 0x6000, and either fade its colour triple by 0xC0C0C or, once
   the first channel passes 0x60, retire the actor.  97 words, 0 differing,
   second compile.

   Landed off the two bodies already in this unit -- func_s36_80024B60's
   `e = *(u8 **)p' head and its `*(u16 *)(p - 0x2) |= 0x8000; D_800814A0 |=
   0x8000' teardown are verbatim here -- plus plain reading.  Its only symbols
   are shared by 115 bodies, so the callee set named nothing.

   THE ONE FAULT from first compile, and it is worth knowing because it is
   invisible in the word count: THE DIVISOR IS 5, NOT 10.  0x66666667 with
   `sra 1' after the `mfhi' is /5 and the same multiplier with `sra 2' is /10,
   so a wrong divisor here costs exactly one word per site and nothing else --
   97 words, 3 differing, one row per axis.

   ONE MEASURED FACT: the tail gate is written with the RETIRE arm as the
   `then' (`if (s->unk_0C[0] >= 0x60) { flags } else { += 0xC0C0C }').
   Inverted -- `if (< 0x60) { += } else { flags }', which is the more natural
   reading of the listing's fallthrough -- it is 14 aligned differences, the
   whole of both arms plus the branch.
   MEASURED AND FREE: `(s16) * (u16 *)(p + 0x48) > 0' for the timer test
   (0 either way, as in func_s36_80024B60). */
void func_s36_800249DC(u8 *p, UnkVec *v, UnkSlot *s)
{
    u8 *e;

    e = *(u8 **)p;
    *(u32 *)(e + 0x10) |= 0x8000;
    *(u16 *)(p + 0x48) -= 1;
    if (*(s16 *)(p + 0x48) > 0)
    {
        *(s32 *)(p + 0x1C) += *(s32 *)(p + 0x34);
        *(s32 *)(p + 0x20) += *(s32 *)(p + 0x38);
        *(s32 *)(p + 0x24) += *(s32 *)(p + 0x3C);
    }
    v->unk_00 += (*(s32 *)(p + 0x1C) - v->unk_00) / 5;
    v->unk_04 += (*(s32 *)(p + 0x20) - v->unk_04) / 5;
    v->unk_08 += (*(s32 *)(p + 0x24) - v->unk_08) / 5;
    func_800478B8((u8 *)s);
    if ((s->unk_14 & 0x6000) != 0)
    {
        s->unk_04 = 0;
        s->unk_05 = 0;
    }
    if (s->unk_0C[0] >= 0x60)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    else
    {
        *(s32 *)s->unk_0C += 0xC0C0C;
    }
}

/* The thrown object's per-frame step: raise 0x8000 in the +0x10 word of the
   node at the payload's +0x0, bump the +0x4E frame counter, then run the
   three-case +0x4C state machine -- state 0 seeds the 0x1F/0x1F pair and
   falls into state 1, which counts +0x48 up and publishes +0x48 * 0x30 at
   +0x4A until it reaches 4, and state 2 retires the actor once +0x4E passes
   0xF -- and finish by publishing the +0x4E counter's low two bits as a
   0x80 + n * 0x20 size at +0x40 and its next two bits as n * 0x20 at +0x42.
   71 words, 0 differing, FIRST COMPILE.

   Plain decompilation off its own listing; no twin and 451 bodies share its
   only symbol, so the callee set is no help.  The state machine IS a `switch'
   -- the `beq 1' / `slti 2' / `beqz' tree is emit_case_nodes' balanced tree,
   not an if/else chain -- and case 0 FALLS THROUGH into case 1.

   ONE MEASURED FACT: the +0x46 store before the +0x44 store.  Ascending is
   2 differing.
   MEASURED AND FREE, so none of these is the reason this lands: the
   `u8 *e = *(u8 **)p' local for the node pointer (0 either way), and the
   `(s16) * (u16 *)(p + N)' cast idiom at all three sites where the listing
   shows `lhu' + `sll 16' + `sra 16' instead of `lh' -- plain `*(s16 *)(p + N)'
   is 0 differing at every one of them, individually and together, so the
   zero-extending load is NOT evidence of a cast in the source. */
void func_s36_80024B60(u8 *p, UnkVec *v, UnkSlot *s)
{
    u8 *e;

    e = *(u8 **)p;
    *(u32 *)(e + 0x10) |= 0x8000;
    *(u16 *)(p + 0x4E) += 1;
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        *(u16 *)(p + 0x4E) = 0;
        *(u16 *)(p + 0x48) = 0;
        *(u16 *)(p + 0x46) = 0x1F;
        *(u16 *)(p + 0x44) = 0x1F;
        *(u16 *)(p + 0x4C) += 1;
        /* fallthrough */
    case 1:
        *(u16 *)(p + 0x48) += 1;
        *(u16 *)(p + 0x4A) = *(s16 *)(p + 0x48) * 0x30;
        if (*(s16 *)(p + 0x48) >= 4)
        {
            *(u16 *)(p + 0x4C) += 1;
        }
        break;
    case 2:
        if (*(s16 *)(p + 0x4E) >= 0xF)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    *(u16 *)(p + 0x40) = (*(u16 *)(p + 0x4E) & 3) * 0x20 + 0x80;
    *(u16 *)(p + 0x42) = (*(s16 *)(p + 0x4E) >> 2) * 0x20;
}

/* The beam's second renderable, four stacked semi-transparent POLY_FT4s per
   chain node: build five SVECTORs around the payload's +0xC..+0x10 position
   (four at the corners of a 0x40 box, the fifth 0x20 in on x and y),
   RotTransPersN all five, jitter four DVECTORs off the fifth projection by
   `rand() % d' with d a quarter of the vertical span, and dress each of the
   four quads 0x808080 with GetTPage(0, 1, 0x2C0, 0x100) /
   GetClut(0xA0, 0x1F7) on OT row (sz[4] + sz[j]) >> 3 when that is under
   0x1E0.  252 words, 0 differing, fourth compile.

   NO TWIN at any ratio; the body came off this unit's own func_s36_80024798,
   which had already brought the +0x8D0 cursor, the inlined addPrim and the
   chained-store order into the file, plus the psyq prototype for
   RotTransPersN (SVECTOR *, DVECTOR *, u_short *, u_short *, u_short *, long)
   -- six arguments, which is why the outgoing-argument area is 0x18 and the
   first local sits at 0x18($sp).

   THE DECLARATION ORDER IS READABLE OFF THE FRAME and is part of the answer:
   every array slot is 8-aligned, so the gaps at 0x4A, 0x5A, 0x6A and 0x84 fix
   the order as SVECTOR sv[5] (0x18), s16 sz[5] (0x40), s16 pp[5] (0x50),
   s16 fl[5] (0x60), DVECTOR out[5] (0x70), DVECTOR q[5] (0x88).

   THREE MERGED LOCALS, all three measured from this body at 0 differing, and
   each one presents as a mass register exchange with nothing moved:
     * `r = p' AT THE TOP OF THE WALK.  The payload reads for the SVECTORs and
       the UVs go through the copy and the chain walk through `p'.  Without it
       -- one pointer for both -- it is 244 words / 37 aligned differences.
     * ONE COUNTER FOR THE sv LOOP AND THE SEGMENT LOOP.  Two separate `s32'
       counters are 73 aligned differences; the target's single $s4 is one
       variable reused, and both loops count DOWN (the biv is used in
       arithmetic, so check_dbra_loop cannot have reversed an up-counting
       source).
     * ONE VARIABLE FOR THE `>> 2' AND `>> 4' DIVISORS.  The j-loop's
       quarter-span and the k-loop's sixteenth-span must be the same local:
       separate ones are 32 aligned differences, because only the merged live
       range spans the m loop's `rand()' call and so needs the callee-saved
       register the target spends on it.
   The second and third are what spill the hoisted 0xFFFFFF addPrim mask to
   the frame the way the target does; keeping it in a register is the
   signature of one local too few here. */
s32 func_s36_80024C7C(u8 *p)
{
    Unk80083160 *g = &D_80083160;
    u8 *r;
    UnkNode *n;
    POLY_FT4 *t;
    s32 i;
    s32 k;
    s32 m;
    s32 z;
    s32 d;
    SVECTOR sv[5];
    s16 sz[5];
    s16 pp[5];
    s16 fl[5];
    DVECTOR out[5];
    DVECTOR q[5];

    while (1)
    {
        sv[4].vx = *(u16 *)(p + 0xC) + 0x20;
        sv[4].vy = *(u16 *)(p + 0xE) + 0x20;
        sv[4].vz = *(u16 *)(p + 0x10);
        r = p;
        for (i = 3; i >= 0; i--)
        {
            sv[i].vx = *(u16 *)(r + 0xC) + (i >> 1) * 0x40;
            sv[i].vy = *(u16 *)(r + 0xE) + (i % 2) * 0x40;
            sv[i].vz = *(u16 *)(r + 0x10) + *(u16 *)(r + 0x4A);
        }
        RotTransPersN(sv, out, (u_short *)sz, (u_short *)pp, (u_short *)fl, 5);
        q[0] = out[4];
        for (i = 3; i >= 0; i--)
        {
            z = (sz[4] + sz[i]) >> 3;
            if ((u32)z < 0x1E0)
            {
                d = (out[i].vy - out[4].vy) >> 2;
                if (d == 0)
                {
                    d = 1;
                }
                for (m = 4; m > 0; m--)
                {
                    q[m].vy = out[4].vy + d * m;
                    q[m].vx = out[i].vx + rand() % d - (d >> 1);
                }
                for (k = 3; k >= 0; k--)
                {
                    t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
                    *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x34;
                    *(u32 *)&t->r0 = 0x808080;
                    SetPolyFT4(t);
                    SetSemiTrans((u8 *)t, 1);
                    t->tpage = GetTPage(0, 1, 0x2C0, 0x100);
                    t->clut = GetClut(0xA0, 0x1F7);
                    t->y0 = t->y2 = q[k].vy;
                    t->y1 = t->y3 = q[k + 1].vy;
                    d = (out[i].vy - out[4].vy) >> 4;
                    if (d == 0)
                    {
                        d = 1;
                    }
                    t->x0 = q[k].vx - d;
                    t->x1 = q[k + 1].vx - d;
                    t->x2 = t->x0 + d;
                    t->x3 = t->x1 + d;
                    t->u0 = t->u2 = *(u8 *)(r + 0x40);
                    t->u1 = t->u3 = t->u0 + *(u8 *)(r + 0x44);
                    t->v0 = t->v1 = *(u8 *)(r + 0x42);
                    t->v2 = t->v3 = t->v0 + *(u8 *)(r + 0x46);
                    addPrim((u32 *)(g->unk_00 + 0xB0) + z, t);
                }
            }
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

/* Copy this bank's six-entry {u16, u16} scale table into the frame and write
   the entry the caller's +0x50 state selects into the renderable's +0x1C/+0x1E
   scale pair, clamping an out-of-range index to 0.  47 words, 0 differing,
   FIRST COMPILE.

   NO TWIN at any ratio and nothing shares a symbol with it (D_s36_80024004 is
   bank-local).  The donor is csrc/ovl_dng_s67.c's landed func_s67_80024C50,
   found by its comment rather than by a ratio: the same alignment-1 block move
   of a bank table into a frame struct, the same `<< 2' index.

   THREE FACTS, all priced from this body at 0 differing:
     * `u8 *d = t.unk_00' as a POINTER LOCAL.  Indexing through `t.unk_00'
       directly is 49 words / 13 aligned differences -- two words and a
       register exchange for one folded pointer.
     * The index is `i << 2', not `i * 4': 4 differing.  s67 priced the same
       fact at 2 on its own body.
     * The 0x1C store before the 0x1E store: swapped is 4 differing.
   MEASURED AND FREE, so neither is the reason this lands:
   `memcpy(d, D_s36_80024004, 0x18)' in place of the struct assignment (0
   either way, where s67 priced the same swap at 18 -- it has stack parameters
   for the scheduler to sink past the block move and this one has none), and
   `if ((u16)i >= 6)' in place of `if (i < 0 || i >= 6)' (0 either way; both
   reach build_range_check's `andi 0xFFFF' + `sltiu 6'). */
void func_s36_8002506C(u8 *p, s16 i)
{
    UnkDngS36Scale t;
    u8 *d = t.unk_00;
    UnkSlot *s;

    t = *(UnkDngS36Scale *)D_s36_80024004;
    if (i < 0 || i >= 6)
    {
        i = 0;
    }
    s = (UnkSlot *)*(u32 *)(p - 0x14);
    s->unk_1C = *(u16 *)(d + (i << 2));
    s->unk_1E = *(u16 *)(d + (i << 2) + 2);
}
