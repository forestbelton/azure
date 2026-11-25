#include "common.h"

void func_800478B8(u8 *);
s32 func_800644B8(s32);
UnkNode *func_8003FD64(s32, void *);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
s32 func_8004491C(UnkNode *, u32);
s32 func_80053EF0(s32);
s32 func_800A56E0(s32);
u8 *func_800A3F28(u16, u16, u8 *, u8 *);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
s32 func_s63_8002441C(u8 *p);
void func_s63_80024648(u8 *);
void func_s63_80024B20(u8 *p);
s32 func_s63_80024D58(u8 *);
extern u8 D_80083498[];
extern u8 D_80082E80[];
extern u16 D_80082E94;

/* The bank's five-state cutscene dispatcher, a JUMP TABLE on *(s16 *)(p + 0xA)
   over cases 0..4 (jtbl_s63_80024008, 5 words = 5 cases + 0 pad, bound 5, 5
   distinct). The range check's `beqz' lands on the function's COMMON TAIL,
   not on a table entry, so there is NO default among the five and no
   `default:' label is written; the arms emit in source order 0,1,2,3,4,
   matching the table's ascending addresses. State 0 arms D_800814A8's +0x102
   flag, clears its +0xF4 and publishes the node's own position triple into b,
   then FALLS THROUGH into state 1, which decrements D_800814A8's +0xA6
   counter, spawns the 0x302 node that gets func_s63_80024648 as its step and
   func_s63_8002441C as its renderer, and offsets both copies of the position
   by func_8003DE58's projection. State 2 fires func_800A56E0 with 0x4300 or
   0x300 off func_80053EF0(4). State 3 spawns the second 0x302 node --
   func_s63_80024B20 as its step, func_s63_80024D58 as its renderer -- and
   clears its eight six-byte trail rows. State 4 walks every actor
   func_800A3F28 returns, calls func_8009CE1C on the ones without 0x2000 in
   +0x1C, and tears the actor down. 256 words, 0 differing.

   NO TWIN AT ANY RATIO. The donor is the callee-set grep's
   csrc/ovl_dng_s28.c func_s28_80025738 / csrc/ovl_dng_s15.c pair -- same
   `s = *(u8 **)(p + 0x0); o = s - 0x20;' dispatcher skeleton, same
   func_8003DE58 call spelling, same `(*(u16 *)(... + 0x2A) >> 9) & 7'.

   TWO FACTS, both priced from this body at 0 differing:

   * STATE 3's PAYLOAD POINTER `g' MUST BE ASSIGNED BEFORE THE
     func_8004491C CALL, not after it -- 6 aligned differences the other way,
     every row the one local sitting in $a1 where the target has $s1. Assigned
     after the call the pseudo crosses no call, `allocno_calls_crossed' is 0
     and `find_reg' takes the lowest caller-saved register; assigned before
     it, callee-saved is forced. A single-register caller-versus-callee-saved
     residue is a question about which calls the pseudo spans, not about
     priority.
   * STATE 2's GATE IS `if (f() != 2) { 0x300 } else { 0x4300 }' and not the
     equal-first form, which is 3 aligned differences -- the `bne'/`beq'
     inversion with the two argument sets exchanged.

   NOT MEASURABLE, so do not read either as a fact: the three `out' zero
   stores as one chained assignment or as three statements, and state 3's
   trail clear likewise. Both spellings emit the same stores in the same
   order, so 0-versus-0 here says the tools cannot see the difference, not
   that the target leaves it free. */
void func_s63_8002401C(u8 *p, UnkVec *b)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *e;
    u8 *g;
    u8 *h;
    u8 *k;
    u8 *d;
    UnkNode *m;
    s32 i;
    s16 out[4];

    s = *(u8 **)(p + 0x0);
    o = s - 0x20;
    *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u8 *)(D_800814A8 + 0x102) = 1;
        *(s32 *)(D_800814A8 + 0xF4) = 0;
        *(s32 *)((u8 *)b + 0x0) = *(s32 *)(*(u8 **)(o + 0x8) + 0x0);
        *(s32 *)((u8 *)b + 0x4) = *(s32 *)(*(u8 **)(o + 0x8) + 0x4);
        *(s32 *)((u8 *)b + 0x8) = *(s32 *)(*(u8 **)(o + 0x8) + 0x8);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fall through */
    case 1:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        w = (u8 *)D_800814A8;
        *(s16 *)(p + 0x50) = 0xA;
        *(s16 *)(w + 0xA6) = *(u16 *)(w + 0xA6) - 1;
        *(u8 *)(w + 0xA8) = *(u8 *)(p + 0x8);
        m = func_8003FD64(0x302, D_80083498);
        if (m != NULL)
        {
            if (func_8003DE58(*(u8 **)(*(u8 **)(o + 0xC) + 0x8), (UnkSlot *)*(u8 **)(o + 0xC),
                              out, 0) == 0)
            {
                out[0] = out[1] = out[2] = 0;
            }
            m->unk_10 = (u32)func_s63_80024648;
            func_8004491C(m, (u32)func_s63_8002441C);
            *(u8 **)((u8 *)m + 0x20) = p;
            e = (u8 *)m + 0x20;
            *(s32 *)(e + 0x1C) = *(s32 *)((u8 *)b + 0x0) =
                *(s32 *)(*(u8 **)(o + 0x8) + 0x0) + (out[0] << 16);
            *(s32 *)(e + 0x20) = *(s32 *)((u8 *)b + 0x4) =
                *(s32 *)(*(u8 **)(o + 0x8) + 0x4) + (out[1] << 16);
            *(s32 *)(e + 0x24) = *(s32 *)((u8 *)b + 0x8) =
                *(s32 *)(*(u8 **)(o + 0x8) + 0x8) + (out[2] << 16);
        }
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 2:
        if (*(s16 *)(p + 0x50) > 0)
        {
            break;
        }
        if (func_80053EF0(4) != 2)
        {
            func_800A56E0(0x300);
        }
        else
        {
            func_800A56E0(0x4300);
        }
        *(s16 *)(p + 0x50) = 0xD;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 3:
        if (*(s16 *)(p + 0x50) > 0)
        {
            break;
        }
        m = func_8003FD64(0x302, D_80083498);
        if (m != NULL)
        {
            g = (u8 *)m + 0x20;
            m->unk_10 = (u32)func_s63_80024B20;
            func_8004491C(m, (u32)func_s63_80024D58);
            out[0] = (*(u16 *)(D_800814A8 + 0x2A) >> 9) & 7;
            *(s16 *)(g + 0x4) = (D_80082E80[0x24] << 6) + 0x20;
            *(s16 *)(g + 0x6) = (D_80082E80[0x25] << 6) + 0x20;
            *(s16 *)(g + 0x8) = *(u16 *)((u8 *)b + 0xA);
            for (i = 7; i >= 0; i--)
            {
                *(s16 *)(g + i * 6 + 0x10) = *(s16 *)(g + i * 6 + 0x12) = 0;
            }
            *(u8 **)(g + 0x0) = p;
            *(s16 *)(g + 0x40) = 0;
        }
        *(s16 *)(p + 0x50) = 0x3D;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 4:
        if (*(s16 *)(p + 0x50) == 8)
        {
            h = (u8 *)D_800814A8;
            if (h != NULL)
            {
                k = h;
                while (1)
                {
                    h = func_800A3F28(D_80082E80[0x24], D_80082E80[0x25], k, h);
                    if (h == NULL)
                    {
                        break;
                    }
                    if ((*(s32 *)(h + 0x1C) & 0x2000) == 0)
                    {
                        func_8009CE1C(h, 0x10, *(u8 *)(p + 0x9), 0xA, *(s16 *)(s + 0x2A), (u32)s, 2);
                    }
                }
            }
        }
        if ((D_80082E94 & 0x8000) != 0 || *(s16 *)(p + 0x50) < 0)
        {
            if ((*(s16 *)(p + 0x52) & 0x8000) != 0)
            {
                *(s16 *)(p + 0x52) = *(u16 *)(p + 0x52) & 0x7FFF;
            }
            else
            {
                d = (u8 *)&D_80083460;
                *(s16 *)(d + 0xA) = *(u16 *)(d + 0xA) - 1;
                *(s32 *)(d + 0xC) = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
        }
        break;
    }
}

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
s32 func_s63_8002441C(u8 *p)
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s63", func_s63_80024648);

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
void func_s63_80024AA4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x302 trail node's step: shift the eight six-byte rows down one, settle
   each row's +0x12 by its +0x14, decrement the +0x42 countdown and run a
   four-case COMPARE CHAIN on *(s16 *)(p + 0x40) -- no .rodata block, so this
   function owns no jump table. State 0 clears the countdown and the +0xC
   colour and FALLS THROUGH into state 1, which ramps the colour by 0xC0C0C,
   parks func_800644B8 of the countdown's +0x42 angle in +0x3A and advances
   once the colour passes 0x60605F. State 2 rebuilds the rows' +0x14 spacing
   from a quadratic in the row index, pushes any row whose gap to its
   successor exceeds a per-row ceiling, bleeds the colour and advances on the
   last frame. State 3 is the standard teardown. 142 words, 0 differing.

   NO DONOR OF ANY KIND -- no twin at any ratio, no usable callee-set row,
   nothing in the lead files. Written from the listing with the compiler as
   the authority on two shapes, both read out of
   scratch/gcc-src/gcc-2.7.2.3/loop.c.

   SIX FACTS, all priced from this body at 0 differing:

   * ALL FOUR LOOPS SHARE ONE COUNTER `i'. Four separate locals is 20 aligned
     differences of pure register rename with nothing moved -- the textbook
     allocation-fault residue, and the cure is merging the variables, not a
     device. The target holds every one of the four counters in $a1.
   * THE TWO STRIDED VALUES IN STATE 2 ARE SOURCE VARIABLES, `c = 0xD0' then
     `c -= 0x18' and `d = 0x160' then `d -= 0x10', NOT the expressions
     `i * 0x18 + 0x28' and `i * 0x10 + 0x100'. As expressions they are 7
     aligned differences, because fold splits the constant out of
     `(giv + C) - mem' and the giv loses its +0x28. The DIAGNOSTIC is the
     preheader's order: `record_giv' PREPENDS to `bl->giv', so reduced-giv
     initialisers are emitted in REVERSE recording order and therefore always
     AFTER the loop's own source insns. A strided value whose initialiser is
     emitted BEFORE the counter's is a plain variable; one emitted after it is
     a giv.
   * BOTH ARE COMMA INITIALISERS, `for (i = 7, c = 0xD0; ...)'. As a
     statement ahead of the `for' the two inits come out in the other order,
     3 aligned differences.
   * `*(u16 *)(p + i * 6 + 0x12) + (d + (d >> 1))' WITH THE INNER GROUP
     PARENTHESISED -- ungrouped it associates as `(mem + d) + (d >> 1)' and
     is 4 aligned differences.
   * STATE 0's +0x40 INCREMENT IS ITS LAST STATEMENT, not its first: first it
     is 10 aligned differences, because reorg then steals the `lhu'/`addiu'
     out of the arm and into the compare chain's empty delay slots.
   * LOOP 1's TWO ROW OFFSETS GO THROUGH LOCALS `a' and `b'. Inline as
     address arithmetic the two address givs are reduced into one cursor and
     the body is 134 words / 16 aligned differences; assigned to locals the
     multiplies are recomputed from the counter each iteration, which is what
     the target does. */
void func_s63_80024B20(u8 *p)
{
    s32 i;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;

    for (i = 0; i < 7; i++)
    {
        a = i * 6;
        b = (i + 1) * 6;
        *(s16 *)(p + a + 0x10) = *(u16 *)(p + b + 0x10);
    }
    for (i = 7; i >= 0; i--)
    {
        *(s16 *)(p + i * 6 + 0x12) = *(u16 *)(p + i * 6 + 0x12) + *(u16 *)(p + i * 6 + 0x14);
    }
    *(s16 *)(p + 0x42) = *(u16 *)(p + 0x42) - 1;
    switch (*(s16 *)(p + 0x40))
    {
    case 0:
        *(s16 *)(p + 0x42) = 0;
        *(u32 *)(p + 0xC) = 0;
        *(s16 *)(p + 0x40) = *(u16 *)(p + 0x40) + 1;
        /* fall through */
    case 1:
        *(u32 *)(p + 0xC) += 0xC0C0C;
        *(s16 *)(p + 0x3A) = func_800644B8(*(s16 *)(p + 0x42) << 9) >> 7;
        if (*(s32 *)(p + 0xC) > 0x60605F)
        {
            *(s16 *)(p + 0x42) = 0x38;
            *(s16 *)(p + 0x40) = *(u16 *)(p + 0x40) + 1;
        }
        break;
    case 2:
        for (i = 7, c = 0xD0; i >= 0; i--)
        {
            *(s16 *)(p + i * 6 + 0x14) = c - *(u16 *)(p + 0x42) + (i * i >> 2) * 20;
            c -= 0x18;
        }
        for (i = 6, d = 0x160; i >= 0; i--)
        {
            e = (i + 1) * 6;
            if (*(s16 *)(p + e + 0x12) - *(s16 *)(p + i * 6 + 0x12) > d)
            {
                *(s16 *)(p + i * 6 + 0x12) = *(u16 *)(p + i * 6 + 0x12) + (d + (d >> 1));
            }
            d -= 0x10;
        }
        if (*(s16 *)(p + 0x42) < 8)
        {
            *(u32 *)(p + 0xC) += 0xFFF3F3F4;
        }
        if (*(s16 *)(p + 0x42) <= 0)
        {
            *(s16 *)(p + 0x40) = *(u16 *)(p + 0x40) + 1;
        }
        break;
    case 3:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s63", func_s63_80024D58);
