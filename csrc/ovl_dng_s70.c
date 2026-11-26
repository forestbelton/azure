#include "common.h"

s32 func_s70_80024440(u8 *p);

void func_800478B8(u8 *);

s32 rand(void);
void func_s70_8002466C(u8 *p);
void func_s70_80024ACC(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_s70_80024B48(u8 *p);
s32 func_s70_80024C14(u8 *p);
UnkNode *func_8003FD64(s32, void *);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
void func_800419EC(u16, u32);
s32 func_8004491C(UnkNode *, u32);
s32 func_800644B8(s32);
s32 func_80053EF0(s32);
u8 *func_800A3F28(u16, u16, u8 *, u8 *);
s32 func_800A56E0(s32);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_80082E80[];
extern u16 D_80082E94;
extern u8 D_80083498[];
extern u8 D_80083780[];
extern u8 D_80045340[];
extern u8 D_800DEA68[];

/* The bank's five-state cutscene dispatcher and the OWNER OF
   jtbl_s70_80024008 (5 words = 5 cases + 0 pad, dispatch bound 5, 5 distinct
   targets). A JUMP TABLE on *(s16 *)(p + 0xA); the `sltiu' has NO minval
   subtraction before it, so the case set is 0..4, and the range-check `beqz'
   lands on the function's COMMON TAIL rather than on a table entry, so there
   is NO default among the five and no `default:' label is written. The arms
   emit in source order 0,1,2,3,4, which is also the table's ascending entry
   order -- verified by standalone compile, see ARM ORDER below. State 0 arms
   D_800814A8's +0x102 flag, clears its +0xF4, publishes the node's position
   triple into b and FALLS THROUGH into state 1, which decrements D_800814A8's
   +0xA6 counter and spawns the 0x302 node that gets func_s70_8002466C as its
   step and func_s70_80024440 as its renderer, offsetting both copies of the
   position by func_8003DE58's projection. State 2 fires func_800A56E0 with
   0x4300 or 0x300 off func_80053EF0(4). State 3 cues func_800419EC(0x18, 0x10)
   once at +0x50 == 2, then spawns the second 0x302 node -- func_s70_80024B48
   as its step, func_s70_80024C14 as its renderer -- dressing it from the
   D_8006CCD8/D_8006CCE8 facing pair. State 4 walks every actor func_800A3F28
   returns, calls func_8009CE1C on the ones without 0x2000 in +0x1C, and tears
   the actor down. 265 words, 0 differing.

   DONOR: csrc/ovl_dng_s63.c's landed func_s63_8002401C (256 words, dngtwin
   0.883, and dngcallee's single 13-of-16-symbol row with a clean gap to the
   next tier). Its frame is identical (-0x40, six saves) though the save ORDER
   differs, and it is this body statement for statement with four deltas, all
   read off the listing: state 3 gains the func_800419EC(0x18, 0x10) cue; state
   3's dress reads the two facing tables into +0xC/+0xE/+0x10 where s63 writes
   plain `(D_80082E80[0x24] << 6) + 0x20' at +0x4/+0x6/+0x8 and has a trail
   clear this body has not; state 3's countdown is 0x10 against s63's 0x3D; and
   func_8009CE1C's FOURTH argument is 0xC here against s63's 0xA.

   THE ONE FACT THAT COST ANYTHING, and it is worth 60 rows: `e' IS ONE LOCAL
   SHARED BY STATE 1 AND STATE 3, assigned in state 1 BEFORE the func_8004491C
   call and in state 3 AFTER it. The donor uses TWO locals and assigns state
   1's after the call; spelled that way this body is 265 words / 60 ALIGNED
   ROWS, every row a uniform four-register rotation (candidate s2/s3/s1/s0
   against target s3/s1/s2/s1) with nothing moved and no word gained. One
   pseudo set in two disjoint arms is ONE allocno, and because state 1's set
   crosses func_8004491C that allocno is forced callee-saved and takes $s0 in
   both arms; as two pseudos neither crosses a call in the arm that matters,
   both land in $a0/$a1, and every callee-saved name in the body shifts one
   position. The residue is the textbook "wrong register within a class" shape
   and the answer was ONE LOCAL TOO MANY, exactly as the brief's ladder says.

   AND ITS POSITION IS A SEPARATE FACT, worth 7 rows and one word: `e' must be
   assigned AFTER state 1's `if (func_8003DE58(...) == 0)' block, not before
   it. Assigned before, the `addiu $s0, $s1, 0x20' fills the `beqz $s1' delay
   slot, the `addiu $a2, $sp, 0x20' it displaces moves down into the load
   hazard the target leaves as a `nop', and the body comes out 264 words / 7
   rows. Assigned after the block it sits at the join, and reorg lifts it into
   the `bnez $v0' delay slot out of the target thread -- which is where the ROM
   has it.

   DONOR FACTS RE-PRICED HERE, BOTH HELD, one to the digit:

   * state 2's gate as `if (f() != 2) { 0x300 } else { 0x4300 }' -- the
     equal-first form is 265 words / 3 ROWS, the donor's recorded figure
     exactly, and the three rows are the `beq'/`bne' inversion plus the two
     argument constants exchanged.
   * the three `out' zero stores as one chained assignment -- the donor calls
     this NOT MEASURABLE and it is NOT MEASURABLE here either.

   MEASURED AND FREE, so NOT the reason this lands: the facing-table lookup
   written as `(u16)D_8006CCD8[out[0]]' instead of the corpus's
   `*(u16 *)(out[0] * 2 + (s32)D_8006CCD8)' is 265 words / 0 rows -- BYTE
   IDENTICAL. The `(s32)' cast idiom is kept only for consistency with the five
   landed bodies that read the same two tables; on this body its stated
   mechanism, forcing the index into the `addu's first operand, is NOT
   MEASURABLE, because a register-plus-register `addu' is commutative and gcc
   canonicalises the operands whatever the source order.

   ARM ORDER, since ovlverify and relcand are both blind to it. Standalone
   `tools/cdkgcc -O2 -G0 -S' emits the table as [$L3, $L4, $L8, $L12, $L16] at
   strictly ascending arm-block addresses $L3 < $L4 < $L8 < $L12 < $L16;
   substituting the ROM's five ascending arm labels gives
   [80024080, 800240E8, 800241E8, 80024224, 80024310], which IS the ROM's entry
   sequence. Five words either side, no pad, 1:1 positional. */
void func_s70_8002401C(u8 *p, UnkVec *b)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *e;
    u8 *h;
    u8 *k;
    u8 *d;
    UnkNode *m;
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
            e = (u8 *)m + 0x20;
            m->unk_10 = (u32)func_s70_8002466C;
            func_8004491C(m, (u32)func_s70_80024440);
            *(s32 *)(e + 0x1C) = *(s32 *)((u8 *)b + 0x0) =
                *(s32 *)(*(u8 **)(o + 0x8) + 0x0) + (out[0] << 16);
            *(s32 *)(e + 0x20) = *(s32 *)((u8 *)b + 0x4) =
                *(s32 *)(*(u8 **)(o + 0x8) + 0x4) + (out[1] << 16);
            *(s32 *)(e + 0x24) = *(s32 *)((u8 *)b + 0x8) =
                *(s32 *)(*(u8 **)(o + 0x8) + 0x8) + (out[2] << 16);
            *(u8 **)((u8 *)m + 0x20) = p;
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
        if (*(s16 *)(p + 0x50) == 2)
        {
            func_800419EC(0x18, 0x10);
        }
        if (*(s16 *)(p + 0x50) > 0)
        {
            break;
        }
        m = func_8003FD64(0x302, D_80083498);
        if (m != NULL)
        {
            m->unk_10 = (u32)func_s70_80024B48;
            func_8004491C(m, (u32)func_s70_80024C14);
            e = (u8 *)m + 0x20;
            out[0] = (*(u16 *)(D_800814A8 + 0x2A) >> 9) & 7;
            *(s16 *)(e + 0xC) = D_80082E80[0x24] + *(u16 *)(out[0] * 2 + (s32)D_8006CCD8);
            *(s16 *)(e + 0xE) = D_80082E80[0x25] + *(u16 *)(out[0] * 2 + (s32)D_8006CCE8);
            *(s16 *)(e + 0x10) = *(u16 *)((u8 *)b + 0xA);
            *(u8 **)(e + 0x0) = p;
        }
        *(s16 *)(p + 0x50) = 0x10;
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
                        func_8009CE1C(h, 0x10, *(u8 *)(p + 0x9), 0xC, *(s16 *)(s + 0x2A), (u32)s, 2);
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

/* The step of the 0x302 node func_s70_8002401C spawns in its state 1, so the
   signature is fixed by that call site. Bumps the +0x48 frame counter, sets
   bit 0x8000 in the +0x52 halfword of the record at the actor's +0x0, then
   runs a FOUR-CASE COMPARE CHAIN on *(s16 *)(p + 0x4C) -- no .rodata, no jump
   table, this function owns none. State 0 seeds the GetTPage/GetClut pair into
   +0x50/+0x52 and the 0x1F pair into +0x44/+0x46 and FALLS THROUGH into state
   1, which drifts the +0x1C/+0x20/+0x24 position triple a 0x4000th of the way
   toward the party record D_80083780 offset by func_8003DE58's projection of
   D_80082E80's slot and pulls +0x24 down by func_800644B8 of the counter's
   eleventh. State 2 is the same drift at 0x8000, state 3 tears the actor down.
   The tail publishes the counter's low two bits and its signed quarter into
   +0x40/+0x42, both biased by 0x80, and spawns `c + 1' D_800DEA68 particle
   nodes stepped by func_s70_80024ACC. 280 words, 0 differing, FIRST COMPILE.

   DONOR: csrc/ovl_dng_s63.c's landed func_s63_80024648 (279 words, dngtwin
   0.970, the same unit dngcallee named for func_s70_8002401C). THREE deltas,
   all values rather than shapes: GetClut's first argument is 0xC0 here against
   s63's 0x80; the +0x40 publish gains a `+ 0x80' bias that s63 does not have;
   and the particle payload is D_800DEA68 rather than D_800DEC00. Everything
   else is the donor's text.

   TWO OF THE DONOR'S FACTS RE-PRICED HERE AND BOTH HELD TO THE DIGIT, which
   is why this landed on the first compile:

   * THE `volatile' ON THE +0x4C PAYLOAD STORE IS STILL WORTH THE LAST WORD.
     Without it this body is 278 words / 6 ALIGNED ROWS -- the donor's exact
     figure -- and the six rows are the `addiu $v0, $s1, 0x20' deleted and the
     0x20 + 0x4C folded into one 0x6C displacement. combine calls
     `init_recog_no_volatile' (combine.c:502) so `volatile_ok == 0' and
     `recog_for_combine' will not re-recognise a store whose MEM is volatile.
     DEVICE, not macro residue: the minimal form is one qualifier.
   * the dress block's order -- `*(s32 *)(q + 0x8)' written BEFORE the q+0x4,
     q+0x5 and q+0xC stores -- is kept as the donor has it and was not
     re-priced here, because the body reaches 0 with it.

   NOT RE-PRICED, kept from the donor for the same reason: the separate pointer
   locals per switch arm for both D_80082E80 and D_80083780, the
   (counter << 11) / 10 division written inline, state 2's count gate as
   `>= 0xB -> 4 else 9', and the two `sb $zero' as two ascending statements.
   `out' has FOUR elements and every read of it in the listing is `lh', so it
   is `s16' and takes no cast at the func_8003DE58 call. GetTPage, GetClut and
   rand are the listing's func_80066460, func_8006649C and func_80069EF8,
   resolved through splat/common_syms.txt. */
void func_s70_8002466C(u8 *p)
{
    u8 *q;
    u8 *e;
    u8 *r;
    u8 *t;
    u8 *r2;
    u8 *t2;
    UnkNode *n;
    s32 c;
    s32 i;
    s16 out[4];

    *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) + 1;
    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    c = -1;
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        *(s16 *)(p + 0x50) = GetTPage(0, 1, 0x2C0, 0x100);
        *(s16 *)(p + 0x52) = GetClut(0xC0, 0x1F7);
        *(s16 *)(p + 0x44) = *(s16 *)(p + 0x46) = 0x1F;
        *(s16 *)(p + 0x48) = 0;
        *(s16 *)(p + 0x4E) = 0x30;
        *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        /* fall through */
    case 1:
        c = 0xE;
        r = D_80082E80;
        if (func_8003DE58(*(u8 **)(r + 0x8), (UnkSlot *)r, out, 0) != 0)
        {
            t = D_80083780;
            *(s32 *)(p + 0x1C) += (out[0] + *(s16 *)(t + 0x2) - *(s16 *)(p + 0x1E)) << 14;
            *(s32 *)(p + 0x20) += (out[1] + *(s16 *)(t + 0x6) - *(s16 *)(p + 0x22)) << 14;
            *(s32 *)(p + 0x24) += (out[2] + *(s16 *)(t + 0xA) - *(s16 *)(p + 0x26)) << 14;
            *(s32 *)(p + 0x24) -= func_800644B8((*(s16 *)(p + 0x48) << 11) / 10) << 9;
        }
        if (*(s16 *)(p + 0x48) >= 0xA)
        {
            *(s16 *)(p + 0x4A) = 0x10;
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 2:
        if (*(s16 *)(p + 0x48) >= 0xB)
        {
            c = 4;
        }
        else
        {
            c = 9;
        }
        *(s16 *)(p + 0x4E) = *(u16 *)(p + 0x4E) + 2;
        r2 = D_80082E80;
        if (func_8003DE58(*(u8 **)(r2 + 0x8), (UnkSlot *)r2, out, 0) != 0)
        {
            t2 = D_80083780;
            *(s32 *)(p + 0x1C) += (out[0] + *(s16 *)(t2 + 0x2) - *(s16 *)(p + 0x1E)) << 15;
            *(s32 *)(p + 0x20) += (out[1] + *(s16 *)(t2 + 0x6) - *(s16 *)(p + 0x22)) << 15;
            *(s32 *)(p + 0x24) += (out[2] + *(s16 *)(t2 + 0xA) - *(s16 *)(p + 0x26)) << 15;
        }
        if (*(s16 *)(p + 0x48) >= 0xD)
        {
            *(s16 *)(p + 0x48) = 8;
        }
        *(s16 *)(p + 0x4A) = *(u16 *)(p + 0x4A) - 1;
        if (*(s16 *)(p + 0x4A) <= 0 && *(s16 *)(p + 0x48) == 0xC)
        {
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 3:
        c = 5;
        if (*(s16 *)(p + 0x48) == 0xF)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    *(s16 *)(p + 0x40) = ((*(u16 *)(p + 0x48) & 3) << 5) + 0x80;
    *(s16 *)(p + 0x42) = (((s16) * (u16 *)(p + 0x48) >> 2) << 5) + 0x80;
    for (i = c; i >= 0; i--)
    {
        n = func_8003FD64(0x312, D_80083498);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s70_80024ACC;
            func_8004491C(n, (u32)D_80045340);
            q = (u8 *)n->unk_0C;
            *(s32 *)((u8 *)n->unk_08 + 0x0) = *(s32 *)(p + 0x1C) + (((rand() & 0x3FF) - 0x1FF) << 12);
            *(s32 *)((u8 *)n->unk_08 + 0x4) = *(s32 *)(p + 0x20) + (((rand() & 0x3FF) - 0x1FF) << 12);
            *(s32 *)((u8 *)n->unk_08 + 0x8) = *(s32 *)(p + 0x24) + (((rand() & 0x3FF) - 0x1FF) << 11);
            *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x1000;
            *(s16 *)(q + 0x10) = 0x60;
            *(u8 **)(q + 0x0) = D_800DEA68;
            *(u16 *)(q + 0x14) |= 0xC;
            *(s32 *)(q + 0x8) = *(s32 *)(D_800DEA68 + 0x4);
            *(u8 *)(q + 0x4) = 0;
            *(u8 *)(q + 0x5) = 0;
            *(s32 *)(q + 0xC) = 0x808080;
            *(u8 **)((u8 *)n + 0x20) = *(u8 **)p;
            e = (u8 *)n + 0x20;
            *(volatile s16 *)(e + 0x4C) = 0;
        }
    }
}

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

/* The step of the second 0x302 node, the one func_s70_8002401C spawns in its
   state 3 with func_s70_80024C14 as its renderer. Sets bit 0x8000 in the +0x52
   halfword of the record at the actor's +0x0, bumps the +0x48 frame counter,
   and runs a TWO-CASE compare chain on *(s16 *)(p + 0x4C) -- two cases is below
   gcc 2.7.2's `case_values_threshold' so this is a comparison tree and the
   function owns no table. Case 0 resets the counter, seeds +0x4/+0x6 from the
   +0xC/+0xE cell pair scaled by 64 and biased 0x20, re-bases +0xC/+0xE on the
   same pair less seven, bumps the state, and FALLS THROUGH into case 1, which
   is the shared retire gate: at 0x20 frames it sets 0x8000 in the actor's -0x2
   and in D_800814A0. Anything outside {0,1} returns at once, which is why the
   range check's `j' goes to the epilogue and the two arms share the tail.
   51 words, 0 differing.

   NO DONOR. dngtwin finds no carved sibling at any ratio and dngcallee's only
   symbol is D_800814A0, which 588 functions touch -- the noise tier. Derived
   from the listing; the D_800814A0 retire pair is the in-unit landed
   func_s70_80024ACC's idiom.

   THE ONE FACT, worth 4 rows, and it is convert.c: THE TWO SCALES MUST BE
   `* 64', NOT `<< 6'. Spelled `(*(s16 *)(p + 0xC) << 6) + 0x20' into an
   `*(s16 *)' destination this body is 51 words / 4 ALIGNED ROWS, and all four
   rows are `lhu' where the target has `lh' with nothing else moving. The store
   is a NARROWING CONTEXT, so convert.c distributes the truncation into the sum
   and converts the operands to unsigned short; its LSHIFT_EXPR arm is guarded
   on `TREE_UNSIGNED (type)' and its MULT_EXPR arm is not, so the multiply
   keeps the operand signed and the shift does not. All four sites are the same
   fault, and the diagnostic really is just the load's signedness.

   NOT MEASURABLE, so do not read it as free: the `*(u16 *)(p + 0x48) = 0'
   reset written first in case 0 rather than after the four scale stores. Both
   spellings are 51 words / 0 differing and BYTE IDENTICAL -- the store is
   independent of them and sched1 places it third either way, so the tools
   cannot see the difference at all. The listing's position is not evidence
   about the source here. */
void func_s70_80024B48(u8 *p)
{
    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    *(u16 *)(p + 0x48) = *(u16 *)(p + 0x48) + 1;
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        *(u16 *)(p + 0x48) = 0;
        *(s16 *)(p + 0x4) = *(s16 *)(p + 0xC) * 64 + 0x20;
        *(s16 *)(p + 0x6) = *(s16 *)(p + 0xE) * 64 + 0x20;
        *(s16 *)(p + 0xC) = (*(s16 *)(p + 0xC) - 7) * 64;
        *(s16 *)(p + 0xE) = (*(s16 *)(p + 0xE) - 7) * 64;
        *(u16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
    case 1:
        if (*(s16 *)(p + 0x48) >= 0x20)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s70", func_s70_80024C14);
