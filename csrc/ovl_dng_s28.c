#include "common.h"

s32 func_8009D218_2(u8 *, s32) __asm__("func_8009D218");
s32 func_800A6870(s16);
void func_800AD4D0(u8 *);
s32 func_800AD568(u8 *);
UnkNode *func_800B4C7C(u16, u8 *, s16, s16);
void func_800478B8(u8 *);
s32 func_800A44E0(u16, u16, s16, u32);
s32 func_800BCB04(s32, s32, s16);
s32 func_800A56E0(s32);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s32 D_8008346C;
void func_s28_80024024_3(u8 *, u8, u8 *) __asm__("func_s28_80024024");
UnkNode *func_8003FC64(s32);
void func_8003DB94(void *, void *, s32);
s32 func_8004491C(UnkNode *, u32);
s32 func_80045C34(u8 *, UnkVec *, UnkSlot *);
extern u8 D_80025EE4[];
void func_s28_80024FD4(u8 *p, UnkVec *b);
UnkNode *func_s28_800253D0(u8 *p, UnkVec *b);

/* Raise the actor's +0x64 pool by func_800A6870's scale of the second
   parameter plus 8, doubled when the actor's +0x28 has bit 0, then republish
   it with func_800AD568, float the 0x8004 number and refresh with
   func_800AD4D0. All of it gated on func_8009D218 coming back zero.
   37 words, 0 differing.

   TWIN: the landed func_s30_80024024 in csrc/ovl_dng_s30.c (dngtwin 0.974,
   39 words; func_s25_80024028 is identical to it at the same ratio and
   width). Deltas, all off this listing: the gate argument is 1 not 4, the
   scale bonus is +8 not +16, and this copy's fraction arm is a straight
   DOUBLING where the twin's is `t + (t >> 1)' -- that one arm is the whole
   two-word difference, since the twin spends `sll 16'/`sra 17'/`addu' where
   this has a bare `sll $a1, $v1, 1'.

   Of the twin's facts, the `u8' second parameter and the TWO-argument
   func_8009D218 alias both HOLD. Its `s16' pair HOLDS, and more strongly
   than on the twin, which found the pair free: here the widths carry the
   `addiu $v1, $v0, 0x8' ahead of the +0x28 gate and the `addu $a1, $v1,
   $zero' in the branch's delay slot, and `s32' for BOTH drops that move
   (36 words, 26 differing) because the two pseudos coalesce.

   THE DOUBLING IDIOM IS NOT FREE AT A SUB-WORD DESTINATION, which
   contradicts the "`i * 0x400' and `i << 10' are the same tree" lever --
   that rule holds for an `int' destination, not here. Measured:

     `s16 t; s16 u;' with `u = t << 1;'                 37 w,  0   kept
       ... `u = t + t;'                                 37 w,  0   also free
       ... `u = t * 2;'                                 38 w, 22
       ... `u = (s16)(t * 2);'                          38 w, 22
       ... `u = u * 2;'                                 38 w, 22
     `u16 t; u16 u;' with `u = t * 2;'                  37 w,  7
     `u16 t; s32 u;' with `u = t * 2;'                  37 w,  6
     `s16 t; s32 u;' with `u = t * 2;'                  38 w, 27
     `s32 t; s32 u;' with `u = t * 2;'                  36 w, 26
     `s32 t; s16 u;' with `u = t * 2;'                  37 w,  0
     `s32 t; u16 u;' with `u = t * 2;'                  37 w,  0

   The 38-word rows are `*2' narrowing through `convert_to_integer' to the
   `s16' destination and costing `sll 16'/`srl 15' where `<< 1' and `+ t'
   do not narrow at all. FOUR of the eleven spellings measure 0, so the
   target does NOT pin the pair's widths on its own -- it pins only that the
   destination of the doubling is sub-word OR the idiom is not `*2'. The
   twin's `s16'/`s16' is kept because it is the twin's and it is one of the
   four. */
void func_s28_800242FC(u8 *p, UnkVec *b, u8 *c);
s32 func_s28_80024A64(u8 *p, UnkVec *b, u8 *s);
s32 func_80064584(s32);
s32 func_800644B8(s32);
extern u8 D_80045340[];
extern u8 D_800DEC70[];
s32 rand(void);
typedef struct UnkDngS28PolySP
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
} UnkDngS28PolySP;

void func_s28_80024024(u8 *p, u8 k)
{
    s16 t;
    s16 u;

    if (func_8009D218_2(p, 1) == 0)
    {
        t = func_800A6870(k) + 8;
        u = t;
        if ((p[0x28] & 1) != 0)
        {
            u = t << 1;
        }
        *(s16 *)(p + 0x64) = *(u16 *)(p + 0x64) + u;
        func_800AD568(p);
        func_800B4C7C(0x8004, p, *(s16 *)(p + 0x64), 1);
        func_800AD4D0(p);
    }
}

/* A per-frame step function: bump the +0x14 counter on the node at the
   actor's +0x0 and the actor's own +0x4 counter, drag the mover's +0x8 down
   by 0xA0000, hand the renderable to func_800478B8, and when its +0x14
   reports 0x6000 mark the owning actor retired and set the same bit in
   D_800814A0.
   39 words, 0 differing, first compile.

   TWIN: the landed func_s13_8002418C in csrc/ovl_dng_s13.c (dngtwin 0.933,
   36 words). Its head statement, its gate and its whole retire block carry
   verbatim; the three extra words are this copy's own middle, where the
   twin's `+0x1E' scale pair is replaced by the actor's +0x4 counter bump and
   the `b->unk_08 -= 0xA0000'. The twin's note on D_800814A0 being the PLAIN
   `u32' scalar (structs.h, no declaration needed) holds here too and gives
   the `lui %hi'/`lw %lo' pair, with the `lui' hoisted into the gate's delay
   slot. The twin's own priced fact -- the scale pair as a read-back, not a
   second sum -- does not apply: this copy has no scale pair.

   ONLY THE STATEMENT ORDER IS LOAD-BEARING. Priced on this body, all at
   39 words:

     the +0x4 counter bump BEFORE the +0x8 drag               0   kept
       ... the two swapped, the drag first                    8
     `b->unk_08 -= 0xA0000;'                                  0   kept
       ... `+= -0xA0000;'                                     0   free
       ... `*(s32 *)((u8 *)b + 0x8) -= 0xA0000;'              0   free
       ... second parameter `u8 *b', `*(s32 *)(b + 0x8)'      0   free
     the +0x4 bump read through `u16'                         0   kept
       ... read through `s16'                                 0   free
     the head reached through `*(u8 **)p' twice               0   kept
       ... hoisted into a `u8 *n' local                       0   free
     the gate read through `u16', the retire store `u16'      0   kept
       ... either one through `s16'                           0   free

   So the UnkVec second parameter is the twin's spelling rather than a
   measured one, and the only thing this listing actually constrains is that
   the +0x8 drag is emitted last of the three. The drag is a `lui 0xFFF6' plus
   `addu' because 0xFFF60000 is outside `addiu' range; the `-=' and `+= -'
   forms are the same tree. */
void func_s28_800240B8(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
    b->unk_08 -= 0xA0000;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212 node func_s28_800242FC fires off at its second phase: a
   renderable dressed from D_800DEC70 whose packed colour word the caller hands
   over at +0xC, the 0x7DCF clut, 0xC and 0x100 raised in +0x14 and 0x20 in
   +0x10, a random 12-bit rotation and 0xC00 scale on both axes, D_80045340 as
   the per-slot walk and func_s28_800240B8 as the per-frame step; the caller's
   record at the payload's +0x0 with +0x4/+0x6 cleared, and the caller's mover
   copied in and then kicked along a random 12-bit heading. Returns the node,
   or NULL when the pool is empty.
   106 words, 0 differing, FIRST COMPILE.

   DONORS, two, neither a dngtwin hit. The 0x212 skeleton is
   func_s28_80025614/func_s28_800253D0 in THIS FILE. The D_80045340 +
   D_800DEC70 dress block and `func_8004491C(n, (u32)D_80045340)' -- note the
   second argument is a DATA symbol, not a function -- came from the landed
   func_b09_80170DDC in csrc/ovl_dng_b09.c, found by one grep of D_80045340
   across csrc/; `extern u8 D_80045340[];' and `extern u8 D_800DEC70[];' are
   that unit's declarations verbatim.

   FACTS MEASURED ON THIS BODY FROM THE 0 STATE:

     `s16 t' for the random heading                   106 w,  0   kept
       ... `s32 t'                                    105 w, 33
     `func_80064584(t) >> 4'                          106 w,  0   kept
       ... `func_80064584(t) / 16'                    109 w, 20
     `* 4608'                                         106 w,  0   kept
       ... `(x * 9) << 9'                             106 w,  0   free

   THE `s16 t' ROW IS THE BODY. The heading is `rand() % 0x1000' handed to both
   func_800644B8 and func_80064584, and the target sign-extends it with
   `sll 16'/`sra 16' before the first call -- which only happens if the value
   lives in a narrow signed pseudo. With `s32 t' the extension is absent, the
   body is one word short and the whole call pair rotates, 33 rows. This is the
   brief's "an `lhu' followed by `sll 16'/`sra 16' means an `s16' LOCAL" with
   no load involved at all: a COMPUTED value narrowed the same way says the
   same thing.

   The `/ 16' row is three words of signed-divide bias (`srl 31'/`addu'/
   `sra 4') where `>> 4' is one `sra', so a right shift and a power-of-two
   divide are NOT interchangeable on a signed value -- the lever file's
   "unsigned divide by a power of two simply IS the srl" does not extend to
   the signed case. `* 4608' and `(x * 9) << 9' are the same tree (the listing
   shows `sll 3'/`addu'/`sll 9' either way), so the plain constant is kept for
   readability.

   SIGNATURE NOTE, AND IT IS A PROTOTYPE CHANGE: the third parameter is a
   PACKED COLOUR WORD, not a pointer. func_s28_800242FC's second phase writes
   the three colour bytes at c+0xC/+0xD/+0xE and then passes `*(s32 *)(c +
   0xC)' here, which this body stores with one `sw' at the new renderable's
   +0xC -- the same three bytes func_s28_800253D0 sets as
   `m[0xE] = m[0xD] = m[0xC] = 0x80'. Spelled `u8 *c' with
   `*(u8 **)(m + 0xC) = c' the emitted word is identical, so the two are NOT
   MEASURABLE against each other (106 / 0 both ways) and only the caller makes
   the s32 reading the honest one.

   DECLARATION NOTE: `s32 rand(void);' was missing from this unit entirely --
   func_s28_80025614 has been calling it implicitly since it landed, and
   declcheck --src flags it. Adding it is measured FREE on ALL TWELVE bodies in
   the unit, the six already-landed ones included. */
UnkNode *func_s28_80024154(u8 *p, UnkVec *b, s32 col)
{
    UnkNode *n;
    u8 *q;
    u8 *m;
    UnkVec *v;
    s16 t;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s28_800240B8;
        q = (u8 *)n + 0x20;
        *(u8 **)q = p;
        *(s16 *)(q + 0x4) = 0;
        *(s16 *)(q + 0x6) = 0;
        m = (u8 *)n->unk_0C;
        *(s32 *)(m + 0xC) = col;
        *(s16 *)(m + 0x12) = 0x7DCF;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x20;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8003DB94(m, D_800DEC70, 0);
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1E) = 0xC00;
        *(s16 *)(m + 0x1C) = 0xC00;
        func_8004491C(n, (u32)D_80045340);
        v = n->unk_08;
        *v = *b;
        t = rand() % 0x1000;
        v->unk_00 += (func_80064584(t) >> 4) * 4608;
        v->unk_04 += (func_800644B8(t) >> 4) * 4608;
        return n;
    }
    return NULL;
}

/* The spark's per-frame step, installed by func_s28_80024FD4: bump the +0x14
   counter on the node at the payload's +0x0, the payload's own +0x6 frame and
   +0x8 slot counters, and spin its two +0xC/+0xE headings by 0x80; then
   dispatch on the frame counter. Under 8 and in 0x28..0x2F it raises
   0x10000000 in the owner's +0x1C and paints both the renderable at the
   owner's -0x14 and the caller's own view with the slot counter's residue
   modulo 7 -- the /4, the (/2)%2 and the %2 bits -- scaled by the frame
   counter ramping up and then down; in 8..0x27 the same three bits are painted
   flat at 192 and a child node is spawned through func_s28_80024154 with the
   view's packed colour; past 0x30 it clears 0x10000000 and retires the owner
   into D_800814A0.
   293 words, 0 differing.

   NO EXTERNAL DONOR. The four-arm compare chain and the modulo-7 colour chain
   are func_s28_800250E8's, already at 0 differing in this file, which is where
   `s16 k' for the residue and `s16 h' for `k / 2' written AFTER the +0xC store
   come from; the head bumps and the retire block are
   func_s28_800240B8's. Only the two scale forms and the arm-B temp were
   derived here. THE FUNCTION OWNS NO JUMP TABLE: the dispatch is a compare
   chain of three `slti' (8, 0x28, 0x30) and the listing has no `.section
   .rodata'.

   FACTS MEASURED ON THIS BODY FROM THE 0 STATE:

   | spelling                                              | words | rows |
   |-------------------------------------------------------|-------|------|
   | the ramp as `(f * 3) << 3' / `(f * 3) << 3' etc.      |  293  |   0  |
   |   ... written `f * 8' and `f * 24'                    |  286  | 114  |
   |   ... the operand order swapped, scale first          |  287  | 156  |
   | arm B's product through an `s32 t' local              |  293  |   0  |
   |   ... the expression written out at both stores        |  293  |   2  |
   | `h = k / 2;' AFTER arm B's two +0xC stores            |  293  |   0  |
   |   ... ahead of them                                   |  293  |  21  |
   |   ... between them                                    |  293  |  26  |

   THE SCALE ROW IS fold's MULT REASSOCIATION AND IT IS THE BODY. The target
   computes the scale first and then multiplies: `lh 0x6'/`sll 3'/
   `mult $t1, $v0', i.e. `(k / 4) * (f * 8)'. Written exactly that way, fold
   pulls the constant out of the inner product and emits
   `((k / 4) * 8) * f' instead -- the `sll 3' lands on `k / 4' and the body
   comes out SEVEN WORDS SHORT at 114 rows. `(f << 3)' is an LSHIFT_EXPR, which
   fold's MULT_EXPR reassociation cannot reach, so the grouping survives; the
   same trick gives `* 24' as `(f * 3) << 3'. So: WHERE A PRODUCT OF TWO
   VARIABLES CONTAINS A CONSTANT FACTOR ON ONE SIDE, THE PARENTHESES DO NOT
   HOLD -- spell the constant factor as a SHIFT to pin it to its own operand.
   This is the opposite precondition from the lever file's "a multiply the
   listing shows as sll/addu/sll wants the plain constant", which is about a
   product with no second variable, and from func_s28_800250E8's `* 128' row
   40 words up, which is a constant against a single variable and free.

   THE ARM-B TEMP IS NOT A WIDTH OR A SCOPE FACT. Arm B stores one value to
   both the view and the renderable at each of +0xC/+0xD/+0xE; with the
   expression written out twice, cse unifies it but the first store is
   scheduled THREE SLOTS EARLY, ahead of the `k / 2' chain instead of after it,
   2 rows. One `s32 t' reused for all three pairs, three separate `s32'
   temps, a single temp on the +0xC pair only, and a `u8' temp ALL measure
   293 / 0, so the temp's type, count and scope are NOT MEASURABLE -- only its
   presence is. */
void func_s28_800242FC(u8 *p, UnkVec *b, u8 *c)
{
    u8 *r;
    s16 k;
    s16 h;
    s32 t;

    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(p + 0x6) = *(u16 *)(p + 0x6) + 1;
    *(s16 *)(p + 0x8) = *(u16 *)(p + 0x8) + 1;
    *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) + 0x80;
    *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 0x80;
    if (*(s16 *)(p + 0x6) < 8)
    {
        *(s32 *)(*(u8 **)(p + 0x10) + 0x1C) |= 0x10000000;
        k = *(s16 *)(p + 0x8) % 7 + 1;
        r = *(u8 **)(*(u8 **)(p + 0x10) - 0x14);
        r[0xC] = (k / 4) * (*(s16 *)(p + 0x6) << 3) - 0x80;
        h = k / 2;
        r[0xD] = (h % 2) * (*(s16 *)(p + 0x6) << 3) - 0x80;
        r[0xE] = (k % 2) * (*(s16 *)(p + 0x6) << 3) - 0x80;
        c[0xC] = (k / 4) * ((*(s16 *)(p + 0x6) * 3) << 3);
        c[0xD] = (h % 2) * ((*(s16 *)(p + 0x6) * 3) << 3);
        c[0xE] = (k % 2) * ((*(s16 *)(p + 0x6) * 3) << 3);
    }
    else if (*(s16 *)(p + 0x6) < 0x28)
    {
        *(s32 *)(*(u8 **)(p + 0x10) + 0x1C) |= 0x10000000;
        k = *(s16 *)(p + 0x8) % 7 + 1;
        r = *(u8 **)(*(u8 **)(p + 0x10) - 0x14);
        t = (k / 4) * 192;
        c[0xC] = t;
        r[0xC] = t;
        h = k / 2;
        t = (h % 2) * 192;
        c[0xD] = t;
        r[0xD] = t;
        t = (k % 2) * 192;
        c[0xE] = t;
        r[0xE] = t;
        func_s28_80024154(*(u8 **)p, b, *(s32 *)(c + 0xC));
    }
    else if (*(s16 *)(p + 0x6) < 0x30)
    {
        *(s32 *)(*(u8 **)(p + 0x10) + 0x1C) |= 0x10000000;
        k = *(s16 *)(p + 0x8) % 7 + 1;
        r = *(u8 **)(*(u8 **)(p + 0x10) - 0x14);
        r[0xC] = (k / 4) * ((0x30 - *(s16 *)(p + 0x6)) << 3) - 0x80;
        h = k / 2;
        r[0xD] = (h % 2) * ((0x30 - *(s16 *)(p + 0x6)) << 3) - 0x80;
        r[0xE] = (k % 2) * ((0x30 - *(s16 *)(p + 0x6)) << 3) - 0x80;
        c[0xC] = (k / 4) * (((0x30 - *(s16 *)(p + 0x6)) * 3) << 3);
        c[0xD] = (h % 2) * (((0x30 - *(s16 *)(p + 0x6)) * 3) << 3);
        c[0xE] = (k % 2) * (((0x30 - *(s16 *)(p + 0x6)) * 3) << 3);
    }
    else
    {
        *(s32 *)(*(u8 **)(p + 0x10) + 0x1C) &= ~0x10000000;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Draw one semi-transparent POLY_GT4 for the four world positions the caller
   has already left in the 0x1F800000 scratchpad at +0x30/+0x40/+0x50/+0x60:
   the 0x7DCF clut on GetTPage(0, 3, 0x300, 0x100), a U span of eight texels
   starting at the sprite index times 8 plus 0x80, V 0x40 on the top edge and
   0x7F on the bottom, vertices 0 and 1 black and vertices 2 and 3 taking the
   caller's +0xC/+0xD/+0xE colour bytes, projected with RotAverage4 (ten
   arguments, so six go on the stack) and chained into D_80083160's +0x8D0 ring
   when the averaged z is under 0x1E0. Returns 0.
   181 words, 0 differing.

   DONOR: the landed func_s60_80024A30 in csrc/ovl_dng_s60.c -- 213 words, the
   same "four-vertex version" of a scratchpad POLY_GT4 -- found by dngcallee,
   whose 3-of-3 tier (D_80083160, GetTPage, RotAverage4) has a clean gap to a
   73-function 2-of-3 tier, and confirmed by `func_80065590' occurring in
   exactly one file in csrc/. Its `UnkDngS60PolySP' is this object's scratchpad
   layout FIELD FOR FIELD and is copied into this unit as `UnkDngS28PolySP'
   (it is file-local there too, not in structs.h). Its `(u32)sp | 0xNN'
   argument idiom, its per-vertex UV order and its `addPrim(sp->unk_00 +
   sp->unk_04 * 4, l)' tail all carried unchanged; what is new here is the
   computed U, the black top edge and the caller's colour on the bottom.

   FACTS MEASURED ON THIS BODY FROM THE 0 STATE:

     `s16 k' for the U base                          181 w,  0   kept
       ... `s32 k'                                   181 w,  6
       ... no local, the parameter reassigned
           as `a = a * 8 + 0x80;'                    181 w,  0   free
     the UV stores BEFORE the colour stores          181 w,  0   kept
       ... the colour stores first                   181 w, 35
     `k' assigned after `l->tpage = GetTPage(...)'   181 w,  0   kept
       ... anywhere BEFORE the GetTPage call         180 w, 14
       ... any of four slots after it                181 w,  6 or 0

   THE WIDTH ROW IS A SCHEDULE, NOT A CAST, AND THAT IS NEW. With `s32 k' every
   one of the 181 words is correct and the three insns of `k' (`sll 16'/
   `sra 13'/`addiu 0x80') come out FOURTEEN SLOTS LATE -- after the six
   `addu'/`ori' pairs that materialise RotAverage4's stack arguments, instead of
   immediately after the GetTPage call. Everything in that block has
   INSN_PRIORITY 1 (sched.c's `priority' takes the longest dependence chain
   with cost-1 contributions, and no load or multiply is upstream), so
   `rank_for_schedule' falls through to its INSN_LUID tie-break -- and the
   HImode truncation changes the pre-sched insn order enough to flip it. FIVE
   source positions for the statement, both declaration orders and the inline
   form all give the IDENTICAL 6 rows, which is the brief's "if two candidate
   spellings each leave the same residue" signal: the position was never the
   fault, the width was.

   Reassigning the parameter measures 0 as well, so the two are NOT MEASURABLE
   against each other and `s16 k' is kept only because it reads better. Note
   the OTHER direction of the same fact: the three k insns moved into the
   PRE-call block come out one word SHORT, because regmove then folds the
   parameter's entry copy `addu $s0, $a0, $zero' into the `sll 16' -- the
   brief's "a missing register-to-register copy is a parameter that dies at its
   copy", here caused by hoisting rather than by a cursor.

   ADDRESSES A CONSTANT BASE (0x1F800000): `ovlverify --raw' reports 4
   differing words and all four are relocations (D_80083160's %hi/%lo, `jal
   GetTPage', `jal RotAverage4'); no displacement differs. Same report as the
   donor's, which lists 6 for its own symbol set. */
s32 func_s28_80024790(s16 a, u8 *c)
{
    UnkDngS28PolySP *sp = (UnkDngS28PolySP *)0x1F800000;
    u8 *base;
    POLY_GT4 *l;
    s16 k;

    base = D_80083160.unk_00;
    sp->unk_00 = base + 0xB0;
    l = *(POLY_GT4 **)(base + 0x8D0);
    *(POLY_GT4 **)(base + 0x8D0) = l + 1;
    setPolyGT4(l);
    setSemiTrans(l, 1);
    l->tpage = GetTPage(0, 3, 0x300, 0x100);
    l->clut = 0x7DCF;
    k = a * 8 + 0x80;
    l->u0 = k;
    l->v0 = 0x40;
    l->u1 = k | 7;
    l->v1 = 0x40;
    l->u2 = k | 7;
    l->v2 = 0x7F;
    l->u3 = k;
    l->v3 = 0x7F;
    l->r0 = 0;
    l->g0 = 0;
    l->b0 = 0;
    l->r1 = 0;
    l->g1 = 0;
    l->b1 = 0;
    l->r2 = c[0xC];
    l->g2 = c[0xD];
    l->b2 = c[0xE];
    l->r3 = c[0xC];
    l->g3 = c[0xD];
    l->b3 = c[0xE];
    sp->unk_10 = sp->unk_30 >> 16;
    sp->unk_18 = sp->unk_40 >> 16;
    sp->unk_20 = sp->unk_50 >> 16;
    sp->unk_28 = sp->unk_60 >> 16;
    sp->unk_12 = sp->unk_34 >> 16;
    sp->unk_1A = sp->unk_44 >> 16;
    sp->unk_22 = sp->unk_54 >> 16;
    sp->unk_2A = sp->unk_64 >> 16;
    sp->unk_14 = sp->unk_38 >> 16;
    sp->unk_1C = sp->unk_48 >> 16;
    sp->unk_24 = sp->unk_58 >> 16;
    sp->unk_2C = sp->unk_68 >> 16;
    sp->unk_04 = RotAverage4((SVECTOR *)((u32)sp | 0x10), (SVECTOR *)((u32)sp | 0x18),
                             (SVECTOR *)((u32)sp | 0x20), (SVECTOR *)((u32)sp | 0x28),
                             (long *)((u32)sp | 0x70), (long *)((u32)sp | 0x74),
                             (long *)((u32)sp | 0x78), (long *)((u32)sp | 0x7C),
                             (long *)((u32)sp | 0x8), (long *)((u32)sp | 0xC));
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

/* The spark's per-slot walk, installed by func_s28_80024FD4: two eight-step
   rings of quads around the mover, drawn one at a time through
   func_s28_80024790 out of the 0x1F800000 scratchpad. The frame counter at the
   payload's +0x6 picks the ring geometry -- growing under 8, fixed at
   0x30/0x10 in 8..0x27, shrinking in 0x28..0x2F, collapsed past that -- and
   sets the depth of the v0/v1 edge; the first ring sweeps the +0xC heading
   with a jittered radius on v1 and v2 and a clean one on v0/v3, the second
   sweeps the +0xE heading carrying each row's far edge into the next row's
   near edge and wrapping the eighth row back onto the first. Returns 0.
   348 words, 0 differing.

   NO DONOR FOR THE BODY -- only the scratchpad layout, which is
   func_s28_80024790's `UnkDngS28PolySP' two listings up, and that is the whole
   reason the quad corners could be read at all: the four 16-byte slots at
   +0x30/+0x40/+0x50/+0x60 are the SAME four world positions
   func_s28_80024790 projects, so the two listings interpret each other and no
   offset needed guessing. Landing the caller first is what made this body
   readable.

   FACTS MEASURED ON THIS BODY FROM THE 0 STATE:

   | spelling                                               | words | rows |
   |--------------------------------------------------------|-------|------|
   | `s32 m0..m3' DECLARED BEFORE the `s16' locals           |  348  |   0  |
   |   ... after them                                        |  348  |  52  |
   | the wrap row as `m0 = sp->unk_30 = <expr>;'             |  348  |   0  |
   |   ... `m0 = <expr>; sp->unk_30 = m0;'                    |  347  |  15  |
   |   ... the same, but the +0x64 pair written m-first       |  347  |   4  |
   |   ... written as a store then a READ-BACK                |  348  |   0  |
   | the arms assigning `ro, ri, rn, jm' in that order        |  348  |   0  |
   |   ... `ri, ro, rn, jm'                                   |  347  |  16  |
   |   ... the other 22 permutations                 339-347 w, 21-111 |
   | `b->unk_00 + (<product> << 8)'                          |  348  |   0  |
   |   ... `(<product> << 8) + b->unk_00'                     |  348  |   2  |
   | `i * 512'                                               |  348  |   0  |
   |   ... `(i << 9)' at all ten sites                        |  348  |   0  |
   | `- 0xC00000' for the fixed-ring depth                    |  348  |   0  |
   |   ... `+ 0xFF400000'                                     |  348  |   0  |
   | `* 0x180000' for the growing-ring depth                  |  348  |   0  |
   |   ... `(x * 3 << 19)'                                    |  348  |   0  |

   THE DECLARATION-ORDER ROW IS A STACK-SLOT ORDER AND IT IS WORTH 8 BYTES OF
   FRAME. This body spills eight pseudos and ten callee-saved registers, so the
   frame is -0x68 with locals at sp+0x10..0x3F. Reload's `alter_reg' assigns
   stack slots in ASCENDING PSEUDO NUMBER, and gcc 2.7.2 creates a local's
   pseudo in `expand_decl' -- at its DECLARATION, not its first use. With the
   two `s16' locals declared first they take sp+0x10 and sp+0x18, the three
   SImode wrap values follow at 0x20/0x24/0x28, and the frame comes out -0x60:
   one 8-byte slot SMALLER, because a HImode slot is 8-aligned here and
   padding that lands after three words instead of before them is reclaimed.
   Every save offset then shifts and the residue reads as 52-83 rows of
   prologue and epilogue. Declaring the `s32' group first reproduces the
   target's 0x10/0x14/0x18 words, 0x20/0x28 halfwords, -0x68 frame exactly.
   Three different placements of `s32 i' around that group all measure 348 / 0,
   so only the s32-before-s16 grouping is load-bearing.

   THE WRAP ROW IS THE SURVIVING COPY. m3 wins `$fp' while m0..m2 are spilled,
   and the target emits `addu $v0, $v0, $v1' then `addu $fp, $v0, $zero' then
   stores `$fp'. Written `m3 = <expr>; sp->unk_64 = m3;' the sum coalesces
   straight into `$fp' and that copy is gone -- one word short. Assigning the
   scratchpad field FIRST and taking m3 out of it keeps the two pseudos apart.
   The chained `m3 = sp->unk_64 = <expr>;' and the explicit
   `sp->unk_64 = <expr>; m3 = sp->unk_64;' read-back are byte-identical, so
   they are NOT MEASURABLE against each other; the read-back is this family's
   idiom (func_s28_800250E8 prices it at its +0x1C store). Note the 4-row row:
   fixing only the +0x64 pair and leaving m0..m2 the other way is still one
   word short, so the fault is per-value and the fix had to be applied to all
   four.

   READ WHICH ROWS AGREE: the first compile was 347 words / 83 rows and almost
   every row was a prologue or epilogue `sw'/`lw' offset with the SAVE LIST and
   SAVE ORDER already byte-identical. That combination means the frame SIZE is
   wrong, not the allocation -- the brief's "a frame delta with an identical
   save list and order is the argument area" generalised: here it is the LOCAL
   area, and the cause was declaration order rather than a call's arity.

   An `s16 f = *(u16 *)(p + 0x6);' local for the growing arm's four
   expressions measures 348 / 0 as well, so the brief's "an `lhu' followed by
   `sll 16'/`sra 16' means an `s16' LOCAL, never a cast" does NOT hold here:
   the plain `*(s16 *)(p + 0x6)' written out four times produces the same
   `lhu'/`sll 16'/`sra 16' triple, because cse unifies the four reads and the
   signed-divide's bias is taken off bit 31 of the `sll 16'. The inline form is
   kept; the local is NOT MEASURABLE against it.

   `rand() % jm' in the first ring is a real `div' because the divisor is a
   variable; `rand() % 5' in the second is the 0x66666667 magic multiply. That
   is the lever file's "if you see any `div' in a listing, the source divided
   by a variable" with both halves in one function. */
s32 func_s28_80024A64(u8 *p, UnkVec *b, u8 *s)
{
    UnkDngS28PolySP *sp = (UnkDngS28PolySP *)0x1F800000;
    s32 m0;
    s32 m1;
    s32 m2;
    s32 m3;
    s16 ro;
    s16 ri;
    s16 rn;
    s16 jm;
    s16 u;
    s32 i;

    if (*(s16 *)(p + 0x6) < 8)
    {
        sp->unk_38 = sp->unk_48 = b->unk_08 - *(s16 *)(p + 0x6) * 0x180000;
        ro = *(s16 *)(p + 0x6) * 6;
        ri = *(s16 *)(p + 0x6) * 2;
        rn = *(s16 *)(p + 0x6) / 2;
        jm = *(s16 *)(p + 0x6) / 2 + 1;
    }
    else if (*(s16 *)(p + 0x6) < 0x28)
    {
        ro = 0x30;
        ri = 0x10;
        rn = 4;
        jm = 5;
        sp->unk_38 = sp->unk_48 = b->unk_08 - 0xC00000;
    }
    else if (*(s16 *)(p + 0x6) < 0x30)
    {
        sp->unk_38 = sp->unk_48 = b->unk_08 - (0x30 - *(s16 *)(p + 0x6)) * 0x180000;
        ro = (0x30 - *(s16 *)(p + 0x6)) * 6;
        ri = (0x30 - *(s16 *)(p + 0x6)) * 2;
        rn = (0x2F - *(s16 *)(p + 0x6)) / 2;
        jm = (0x2F - *(s16 *)(p + 0x6)) / 2 + 1;
    }
    else
    {
        jm = 0;
        rn = 0;
        ri = 0;
        ro = 0;
    }
    sp->unk_58 = sp->unk_68 = b->unk_08;
    for (i = 0; i < 8; i++)
    {
        u = rand() % jm;
        sp->unk_40 = b->unk_00 + ((func_800644B8(*(s16 *)(p + 0xC) + i * 512) >> 4) * (ro + u) << 8);
        sp->unk_44 = b->unk_04 + ((func_80064584(*(s16 *)(p + 0xC) + i * 512) >> 4) * (ro + u) << 8);
        sp->unk_50 = b->unk_00 + ((func_800644B8(*(s16 *)(p + 0xC) + i * 512) >> 4) * (ri + u) << 8);
        sp->unk_54 = b->unk_04 + ((func_80064584(*(s16 *)(p + 0xC) + i * 512) >> 4) * (ri + u) << 8);
        sp->unk_30 = sp->unk_60 =
            b->unk_00 + ((func_800644B8(*(s16 *)(p + 0xC) + (i + 1) * 512) >> 4) * rn << 8);
        sp->unk_34 = sp->unk_64 =
            b->unk_04 + ((func_80064584(*(s16 *)(p + 0xC) + (i + 1) * 512) >> 4) * rn << 8);
        func_s28_80024790(i, s);
    }
    u = rand() % 5;
    m0 = sp->unk_30 = b->unk_00 + ((func_800644B8(*(s16 *)(p + 0xE)) >> 4) * (ro + u) << 8);
    m1 = sp->unk_34 = b->unk_04 + ((func_80064584(*(s16 *)(p + 0xE)) >> 4) * (ro + u) << 8);
    m2 = sp->unk_60 = b->unk_00 + ((func_800644B8(*(s16 *)(p + 0xE) + 0x200) >> 4) * ri << 8);
    m3 = sp->unk_64 = b->unk_04 + ((func_80064584(*(s16 *)(p + 0xE) + 0x200) >> 4) * ri << 8);
    for (i = 0; i < 8; i++)
    {
        sp->unk_40 = sp->unk_30;
        sp->unk_44 = sp->unk_34;
        sp->unk_50 = sp->unk_60;
        sp->unk_54 = sp->unk_64;
        if (i == 7)
        {
            sp->unk_30 = m0;
            sp->unk_34 = m1;
            sp->unk_60 = m2;
            sp->unk_64 = m3;
        }
        else
        {
            u = rand() % 5;
            sp->unk_30 =
                b->unk_00 + ((func_800644B8(*(s16 *)(p + 0xE) + i * 512) >> 4) * (ro + u) << 8);
            sp->unk_34 =
                b->unk_04 + ((func_80064584(*(s16 *)(p + 0xE) + i * 512) >> 4) * (ro + u) << 8);
            sp->unk_60 =
                b->unk_00 + ((func_800644B8(*(s16 *)(p + 0xE) + (i + 1) * 512) >> 4) * ri << 8);
            sp->unk_64 =
                b->unk_04 + ((func_80064584(*(s16 *)(p + 0xE) + (i + 1) * 512) >> 4) * ri << 8);
        }
        func_s28_80024790(i, s);
    }
    return 0;
}

/* Spawn the 0x212 node that carries the ring flyer's spark: the caller's own
   parent record at the payload's +0x0, the caller's +0x12 budget at +0x8, a
   0x30 countdown at +0xA, two random 12-bit angles at +0xC/+0xE, the caller's
   +0x18 handle at +0x10, func_s28_800242FC as the per-frame step and
   func_s28_80024A64 as the per-slot walk, with the caller's mover copied
   wholesale into the node's own. Returns nothing.
   69 words, 0 differing, FIRST COMPILE.

   NO EXTERNAL DONOR WAS OPENED. dngtwin and dngcallee were not needed: this
   file already held func_s28_80025614 at 0 differing, which is the same 0x212
   spawn with a renderable dress block, so the func_8003FC64 head, the
   `q = (u8 *)n + 0x20' payload view, the `*n->unk_08 = *b' mover copy and the
   func_8004491C tail are its measured spellings with this listing's offsets.
   That is the brief's "land the small siblings in your own unit first" and it
   is the whole of this body.

   THE MOVER COPY MUST PRECEDE func_8004491C HERE AND MUST FOLLOW IT IN
   func_s28_800253D0, 180 words down the same file. Priced from the 0 state:

     `*n->unk_08 = *b;' BEFORE func_8004491C          69 w,  0   kept
       ... after it                                   68 w, 30
     the +0x8 store BEFORE the +0xA store             69 w,  0   kept
       ... the other order                            69 w,  5
     the +0x20 store named off `n' as +0x20           69 w,  0   kept
       ... named off `q' as +0x0                      69 w,  0   free

   The listing emits the +0xA store FIRST and sinks the +0x8 store into the
   rand() call's delay slot, so the 5-row row above is the brief's "a store's
   position inside a dress block is not readable" confirmed directly: reading
   the order off the asm gives the wrong answer and costs 5 rows.

   `rand' is func_80069EF8 (splat/common_syms.txt), so no address is written;
   `s32 rand(void);' had to be ADDED to this unit's block -- see the note on
   func_s28_80024154. ovlverify --raw reports 8 differing words and all eight
   are relocations (func_8003FC64, two rand, func_8004491C, and the %hi/%lo
   pairs for func_s28_800242FC and func_s28_80024A64); no displacement
   differs, and relcand agrees on all four call targets. */
void func_s28_80024FD4(u8 *p, UnkVec *b)
{
    UnkNode *n;
    u8 *q;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s28_800242FC;
        q = (u8 *)n + 0x20;
        *(u8 **)((u8 *)n + 0x20) = *(u8 **)p;
        *(s16 *)(q + 0x6) = 0;
        *(s16 *)(q + 0x8) = *(u16 *)(p + 0x12);
        *(s16 *)(q + 0xA) = 0x30;
        *(s16 *)(q + 0xC) = rand() % 0x1000;
        *(s16 *)(q + 0xE) = rand() % 0x1000;
        *(s32 *)(q + 0x10) = *(s32 *)(p + 0x18);
        *n->unk_08 = *b;
        func_8004491C(n, (u32)func_s28_80024A64);
    }
}

/* The ring-of-four flyer's own step function: bump the node's +0x14, the
   actor's +0x10 frame counter and its +0x12 slot counter, spin the
   renderable's +0x1A by 0x300, and then dispatch on the actor's +0xE phase.
   Phase 0 integrates the mover's velocity until the +0x14 budget runs out;
   phases 1 and 2 place the renderable's three colour bytes from the slot
   counter's residue modulo 7 -- the /4, the (/2)%2 and the %2 bits each
   scaled by 128 -- with phase 1 additionally fading them by the frame
   counter, driving the +0x1C/+0x1E scale down from 0x19 and handing off to
   func_s28_80024FD4; phase 3 waits for the renderable's +0x14 to report
   0x6000 and then retires the actor into D_800814A0.
   186 words, 0 differing.

   NO TWIN AT ANY RATIO -- LEADS.txt reports nothing at the 0.60 floor -- and
   the callee-set intersection could not help either: its only distinctive
   symbols are D_800814A0 and func_800478B8, which 96 landed functions share,
   so the recipe returns a tier with no gap. dngnear returns nothing for this
   unit before OR after three landings, because its landed neighbours are in
   my private copy and not in csrc/. What landed it instead was THIS FILE:
   func_s28_800240B8 and func_s28_8002553C were already at 0 differing above
   it and are the same step-function family, so the head bumps, the +0x1A
   spin, the 32-bit velocity integration and the whole phase-3 retire block
   are their measured spellings transplanted. Only the modulo-7 chain, the
   colour arithmetic and the phase tree were derived from the listing.

   The 4-case switch is a BRANCH TREE, not a table: the dispatch tests `== 1',
   then `< 2', then `== 0', then `== 2' and `== 3', with the default falling
   to the epilogue. There is no jtbl symbol and nothing for this unit's rodata
   -- gcc only builds a table from more cases than this.

   FACTS MEASURED ON THIS BODY FROM THE 0 STATE, both spellings priced:

   | spelling                                                | words | rows |
   |---------------------------------------------------------|-------|------|
   | `s16 k' for the modulo-7 residue                         |  186  |   0  |
   |   ... `s32 k'                                           |  184  |  91  |
   | `s16 h' for `k / 2'                                      |  186  |   0  |
   |   ... `s32 h'                                           |  184  |  62  |
   | `h = k / 2;' written AFTER the +0xC colour store         |  186  |   0  |
   |   ... ahead of it, at the top of the arm                |  190  |  58  |
   | the +0x10 bump BEFORE the +0x12 bump                     |  186  |   0  |
   |   ... the other order                                   |  187  |  17  |
   | `(k % 2) * 128 - (*(s16 *)(p + 0x10) * 8 - 0x80)'        |  186  |   0  |
   |   ... `- *(s16 *)(p + 0x10) * 8 + 0x80' unbracketed     |  186  |  29  |
   | the +0x1C scale as a READ-BACK of the +0x1E store        |  186  |   0  |
   |   ... the same sum written out twice                    |  190  |  10  |
   | case 0's gate `p+0x10 >= p+0x14'                         |  186  |   0  |
   |   ... `p+0x14 <= p+0x10'                                |  186  |   2  |

   THE TWO WIDTH ROWS ARE `shorten_binary_op' AND THEY ARE THE BODY. With
   `s16' operands gcc narrows TRUNC_MOD_EXPR to HImode, so `(h % 2) * 128'
   collapses to one `sll 16'/`sra 9' pair; with `s32' it is a full SImode mod
   and the arm is two words shorter and 62 rows out. `/' is NOT narrowed the
   same way, which is why `(k / 4) * 128' keeps its plain `sra 2'/`sll 7' in
   the same statement -- the asymmetry inside one expression is the tell, and
   it is what names the locals' width before any compile.

   `* 128' AND `<< 7' ARE FREE HERE (186 / 0 either way), which is the lever
   file's power-of-two rule holding for an `int'-typed product. Note that it
   does NOT hold at a sub-word destination: see func_s28_80024024 above in
   this same file, where `t * 2' and `t << 1' are 22 aligned differences
   apart. The precondition is the DESTINATION's width, not the operand's.

   MEASURED AND FREE, so none of these is why this lands: `h' dropped and
   written inline as `((k / 2) % 2)' (186 / 0 -- so the local is a readability
   choice and only its WIDTH matters); `k' taken from the un-stored sum
   `(s16)(*(u16 *)(p + 0x12) + 1) % 7 + 1' with the +0x12 store moved after it,
   rather than read back out of +0x12 (186 / 0, cse forwards either way);
   `*(u16 *)(c + 0x14) &= ~0x100' for `&= 0xFEFF'; and `(k / 4) << 7' for
   `(k / 4) * 128'. The +0x1C read-back is also NOT the only spelling that
   works -- a chained `*(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = ...' and an
   `s16' or `s32' temp holding the sum both measure 186 / 0; the read-back was
   kept because it is this family's idiom, measured as such on
   func_s13_8002418C, and the 10-row row above is only against writing the sum
   TWICE. */
void func_s28_800250E8(u8 *p, UnkVec *b, u8 *c)
{
    s16 k;
    s16 h;

    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    *(s16 *)(p + 0x12) = *(u16 *)(p + 0x12) + 1;
    k = *(s16 *)(p + 0x12) % 7 + 1;
    func_800478B8(c);
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x300;
    switch (*(s16 *)(p + 0xE))
    {
    case 0:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x14))
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 1;
        }
        break;
    case 1:
        *(s16 *)(c + 0x12) = 0;
        *(u16 *)(c + 0x14) &= 0xFEFF;
        c[0xC] = (k / 4) * 128 - (*(s16 *)(p + 0x10) * 8 - 0x80);
        h = k / 2;
        c[0xD] = (h % 2) * 128 - (*(s16 *)(p + 0x10) * 8 - 0x80);
        c[0xE] = (k % 2) * 128 - (*(s16 *)(p + 0x10) * 8 - 0x80);
        *(s16 *)(c + 0x1E) = (0x19 - *(s16 *)(p + 0x10)) * 256;
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);
        if (*(s16 *)(p + 0x10) >= 0x10)
        {
            func_s28_80024FD4(p, b);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 1;
        }
        break;
    case 2:
        c[0xC] = (k / 4) * 128;
        h = k / 2;
        c[0xD] = (h % 2) * 128;
        c[0xE] = (k % 2) * 128;
        if (*(s16 *)(p + 0x10) >= 0x20)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 1;
        }
        break;
    case 3:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* Spawn the 0x212 node for the ring-of-four flyer: a renderable dressed from
   D_80025EE4 at white 0x80, the 0x7E00 sprite with the +0x14 0x100 bit set, a
   random 12-bit rotation and 0x400 scale on both axes, func_80045C34 as the
   per-slot walk and func_s28_800250E8 as the per-frame step; the caller's
   record at the payload's +0x0, a random phase in 0..6 at +0x12, the caller's
   +0x12 budget at +0x14 and its +0x18 handle at +0x18, with the caller's mover
   copied wholesale. Returns the node, or NULL when the pool is empty.
   91 words, 0 differing, FIRST COMPILE.

   DONOR: func_s28_80025614 in THIS FILE, already at 0 differing -- the same
   0x212 spawn for this bank's trail, 73 words. Every statement here is one of
   its measured spellings with this listing's offsets and two additions: the
   `rand() % 7' phase and the +0x12/+0x14/+0x18 payload stores. Its own
   priced facts carried without re-measurement being needed, except the one
   below, which INVERTS against the sibling 180 words UP the file.

   FACTS MEASURED ON THIS BODY FROM THE 0 STATE:

     `*n->unk_08 = *b;' AFTER func_8004491C           91 w,  0   kept
       ... before it                                  91 w, 24
     the NULL return as the function's LAST line      91 w,  0   kept
       ... an early `if (n == NULL) return NULL;'     90 w, 78

   TWO BODIES IN ONE UNIT WANT OPPOSITE ANSWERS ON THE SAME STATEMENT, and
   this is the pair. func_s28_80024FD4 above needs the mover copy BEFORE
   func_8004491C (68 words / 30 rows after it); this body needs it AFTER (91
   words / 24 rows before it). The discriminator is visible before any
   compile: this body RETURNS the node, so `n' has to live past the call and
   lands in a callee-saved register either way, while func_s28_80024FD4
   returns void and its `n' dies at the call -- so there the copy's position
   decides whether `n' crosses it at all. "What the rest of this unit does" is
   not evidence; the figures are the evidence.

   The early-return row is the same shape and price direction as
   func_s28_80025614's (72 words / 3 rows there), only much larger here,
   because this body's `j' to the shared epilogue is what carries the return
   value out of the success arm. */
UnkNode *func_s28_800253D0(u8 *p, UnkVec *b)
{
    UnkNode *n;
    u8 *q;
    u8 *m;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        n->unk_10 = (u32)func_s28_800250E8;
        *(u8 **)q = p;
        *(s16 *)(q + 0xE) = 0;
        *(s16 *)(q + 0x10) = 0;
        *(s16 *)(q + 0x12) = rand() % 7;
        *(s16 *)(q + 0x14) = *(u16 *)(p + 0x12);
        *(s32 *)(q + 0x18) = *(s32 *)(p + 0x18);
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        *(s16 *)(m + 0x12) = 0x7E00;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8003DB94(m, D_80025EE4, 0);
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1E) = 0x400;
        *(s16 *)(m + 0x1C) = 0x400;
        func_8004491C(n, (u32)func_80045C34);
        *n->unk_08 = *b;
        return n;
    }
    return NULL;
}

/* Another of this bank's per-frame step functions, the one func_s28_80025614
   installs on its 0x212 node: bump the +0x14 counter on the node at the
   actor's +0x0 and the actor's own +0x10 frame counter, hand the renderable
   to func_800478B8 and spin its +0x1A rotation by 0x300, integrate the
   mover's three velocity words, and once the frame counter passes the actor's
   +0x14 budget mark the actor retired and set the same bit in D_800814A0.
   54 words, 0 differing, first compile.

   NO EXTERNAL DONOR WAS USED OR NEEDED. dngtwin's best is func_s67_800246F0
   at 0.755 and it was never opened: by the time this came up, THIS FILE
   already held func_s28_800240B8 at 0 differing (the same head bump, the same
   retire block) and func_s28_80025738 at 0 differing (the same 32-bit
   velocity integration, as its states 2 and 6). Every statement here is one
   of those two bodies' measured spellings with this listing's offsets. That
   is the brief's "land the small siblings in your own unit first" paying off
   for the second time in this unit -- make it eight for eight.

   FACTS MEASURED ON THIS BODY, both spellings priced, all at 54 words:

     the gate as `*(s16 *)(p + 0x10) > *(s16 *)(p + 0x14)'     0   kept
       ... as `*(s16 *)(p + 0x14) < *(s16 *)(p + 0x10)'        2
     the three velocity adds as `b->unk_00 += b->unk_0C'       0   kept
       ... as plain `*(s32 *)' casts off `b'                   0   free

   The gate's 2 rows are the LOAD ORDER, not the compare: both spellings emit
   `slt $v0, [0x14], [0x10]', and only the kept one loads +0x10 into $v1
   before +0x14 into $v0, which is the order the target has. A comparison
   whose two sides are both memory is a statement about which side the source
   names FIRST, and it is worth reading off the listing rather than deriving.
   Same fact, same price, same direction as func_s28_800250E8's case 0 gate. */
void func_s28_8002553C(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    func_800478B8(c);
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x300;
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    if (*(s16 *)(p + 0x10) > *(s16 *)(p + 0x14))
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212 node that carries this bank's flyer trail: a renderable
   dressed from D_80025EE4 at white 0x80, the 0x7E00 sprite with the +0x14
   0x100 bit set, a random starting rotation and 0x400 scale on both axes,
   func_80045C34 as the per-slot walk, func_s28_8002553C as the per-frame
   step, the caller's record at the node's +0x20 with the caller's +0x12
   budget copied to +0x34, and the caller's mover copied wholesale into the
   node's own. Returns the node, or NULL when the pool is empty.
   73 words, 0 differing.

   DONORS, two, and NEITHER is a dngtwin hit (this body is not in LEADS.txt at
   all): the skeleton is the landed func_s13_8002421C in csrc/ovl_dng_s13.c --
   the same 0x212 spawn one bank over, 69 words -- and the one thing it does
   not have came from func_800C4944 in csrc/ovl_dungeon_800C2824.c, whose
   comment names `q = (u8 *)n + 0x20' as a node-payload view. That is the
   whole of this body's fight.

   THE PAYLOAD VIEW IS THE ONLY LOAD-BEARING SPELLING, and it is worth the
   frame. Priced from the 0 state:

     `q = (u8 *)n + 0x20;' with the +0x30/+0x34 stores
     written `*(s16 *)(q + 0x10)' / `*(s16 *)(q + 0x14)'    73 w,  0   kept
       ... both named off `n' as +0x30 and +0x34            72 w, 10
     the mover copy as `*n->unk_08 = *b;'                   73 w,  0   kept
       ... six field stores through `n->unk_08->unk_NN'     83 w, 27
     `m[0xE] = m[0xD] = m[0xC] = 0x80' DESCENDING           73 w,  0   kept
       ... ascending 0xC / 0xD / 0xE                        73 w,  4
     `*(s16 *)(m + 0x1E) = 0x400;' BEFORE the +0x1C store   73 w,  0   kept
       ... the other order                                  73 w,  4
     the NULL return written as the function's LAST line    73 w,  0   kept
       ... an early `if (n == NULL) return NULL;'           72 w,  3
     a separate `u8 *m' for the renderable cursor           73 w,  0   kept
       ... reusing the parameter `p' as the cursor          73 w,  0   free
     `q' assigned ahead of the step-pointer store           73 w,  0   kept
       ... after it, after the +0x20 store, or all three
           payload stores named off `q' instead of two      73 w,  0   free

   THE PARAMETER-REUSE FACT IS REAL BUT CONJOINED, and the honest reading is
   that `q' subsumes it. On the FIRST compile, with a separate `m' and no `q',
   this was 74 words / 40 rows and the fault was a FOURTH callee-saved
   register: the target saves s0/s1/s2 only, so there are three long-lived
   values (p, b, n) and `m' must share p's register once p dies. Reusing the
   parameter as the cursor -- the brief's own free fix -- took it to 72 words /
   10 rows and fixed the frame. But once `q' is in place the coalescing
   happens by itself and the separate `m' is byte-identical, so the kept body
   uses the natural `m'. Price a frame fault against the OTHER facts still
   missing before writing it down as the mechanism; this one would have been
   recorded as load-bearing by any run that stopped at the first 0.

   The brief's own `-da'/allocno route was not needed for the frame fault and
   would have misled: counting saves against the parameter list is what named
   it, in one reading of the prologue, which is what the brief's "saves minus
   parameters is the number of long-lived locals" line is for.

   SIGNATURE NOTES. func_80069EF8 is `rand' (splat/common_syms.txt), so no
   declaration and no address. func_80045C34's landed definition is in
   csrc/unk_19434.c and its real `s32 (u8 *, UnkVec *, UnkSlot *)' prototype
   is used rather than an invented one, with the `(u32)' cast at the
   func_8004491C site that the rest of the tree uses. D_80025EE4 is this
   object's OWN rodata at 0x80025EE4 (splat's
   dungeon.bin_undefined_syms_auto.txt), declared `extern u8 []' the way
   csrc/ovl_dng_s25.c declares its in-unit D_80025238. */
UnkNode *func_s28_80025614(u8 *p, UnkVec *b)
{
    UnkNode *n;
    u8 *q;
    u8 *m;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        n->unk_10 = (u32)func_s28_8002553C;
        *(u8 **)q = p;
        *(s16 *)(q + 0x10) = 0;
        *(s16 *)(q + 0x14) = *(u16 *)(p + 0x12);
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        *(s16 *)(m + 0x12) = 0x7E00;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8003DB94(m, D_80025EE4, 0);
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1E) = 0x400;
        *(s16 *)(m + 0x1C) = 0x400;
        func_8004491C(n, (u32)func_80045C34);
        *n->unk_08 = *b;
        return n;
    }
    return NULL;
}

/* This bank's root step for the lock-on flyer: bump the +0x10 frame counter,
   then dispatch on the +0xA state. State 0 seeds the heading out of the
   actor's +0x2A and whitens the renderable; state 1 re-reads the slot through
   func_8003DE58, publishes the mover's position, and then AIMS -- at the
   locked-on target's record if the actor has one at +0x60, otherwise by
   walking up to eight cells along the heading with func_800A44E0 and
   func_800BCB04 and aiming at the last cell that admitted it. Either way the
   per-axis velocity is the span over a /16 step count floored at 1, it hands
   off to func_s28_800253D0 or func_s28_80025614, and states 2 and 6 integrate
   that velocity until the step count runs out. States 3, 4 and 5 are the
   timed teardown, with state 4 calling this unit's own func_s28_80024024 and
   state 5 clearing D_8008346C and retiring the actor.
   449 words, 0 differing.

   OWNS jtbl_s28_80024008: 7 words, 7 cases, NO pad, dispatch bound
   `sltiu $v0, $v1, 0x7' -- so all 7 entries are real arms and none is a
   default pad. Entries in order: 0 .Ls28_800257A8, 1 .Ls28_800257D4,
   2 .Ls28_80025CD0, 3 .Ls28_80025D28, 4 .Ls28_80025D44, 5 .Ls28_80025D7C,
   6 .Ls28_80025DB4. All 7 distinct, their addresses ascend, so the arms are
   written ascending 0..6; case 0 falls through into case 1. The range-check
   `beqz $v0, .Ls28_80025E04' lands on the function's COMMON TAIL (the
   `*(s16 *)(p + 0x14) = 0' after the switch), which is not a table entry, so
   there is no default label to write -- confirmed by the grep the brief asks
   for, and an explicit `default: break;' measures 449 / 0, i.e. free.

   DONOR, AND THE PROLOGUE PICKED IT OVER EVERY RATIO. dngtwin's three leads
   are func_s14_80024AE4 (0.694), func_s15_80024CD4 (0.683) and
   func_s13_80024C88 (0.654) and ALL THREE have a -0x48 frame. This body's is
   -0x60 with s0..s7/fp/ra and a 40-byte local area, which is byte-for-byte
   func_s25_800247D4's in csrc/ovl_dng_s25.c -- 656 words, eight states, the
   same walk loop, and the body the CALLEE-SET INTERSECTION names at 8 of 8
   symbols with a clean gap to the next tier. It is 207 words larger than this
   one and no ratio at any floor can see that pair. The shape came from s25;
   the last 5 rows came from func_s13_80024C88's table, which is the same root
   step in a third bank and prices the facts a second time.

   WHAT THIS COPY DROPS relative to s25: the two `% 0x1000' angle bumps in the
   head, state 0's +0x20 clear and its ten-slot clearing loop, the whole
   D_800DDC40 height correction (this bank aims at `*(u8 **)(s + 0x60)'s own
   +0x88 instead), the child-spawn state and the four retire sites. What it
   ADDS: `*(s32 *)(p + 0x18) = (s32)*(u8 **)(s + 0x60)', the `+ 0x20' on the
   aim point's +0xA, and the lift of the whole velocity to 32-bit `UnkVec'
   field arithmetic in states 1, 2 and 6 where s25 works in 16-bit halves.

   THE DEAD `lui $v0, (0x80070000 >> 16)' at 80025A0C IS NOT SOURCE. 0x8007 is
   %hi(D_8006CCD8); it is the walk loop's hoisted table high left behind once
   the loop body's uses were rematerialised as full `lui'/`addiu' pairs, and
   $v0 is overwritten by the loop's first instruction. func_s13_80024C88
   records the same artefact at its own 80024FAC and the same conclusion.

   FACTS MEASURED ON THIS BODY FROM THE 0 STATE, both spellings priced
   (aligned differences; words given because several spellings change them):

   | spelling                                                 | words | rows |
   |----------------------------------------------------------|-------|------|
   | ONE `UnkVec *v' shared by the lock-on and the walk arm    |  449  |   0  |
   |   ... a separate `u8 *g' in the lock-on arm               |  449  |   5  |
   | `(cx << 6) & 0xFFFFFFC0' into func_800A44E0               |  449  |   0  |
   |   ... `& 0xFFC0' against the same u16 prototype           |  452  |  61  |
   | `ox * 64' / `oy * 64' into the aim point                  |  449  |   0  |
   |   ... `(ox << 6)' / `(oy << 6)'                           |  447  |  79  |
   | the aim point's +0xA as `*(u16 *)(b + 0xA) + 0x20'        |  449  |   0  |
   |   ... without the `+ 0x20'                                |  448  |  58  |
   | `*(s32 *)(p + 0x18) = (s32)*(u8 **)(s + 0x60);'           |  449  |   0  |
   |   ... the statement absent                                |  446  |  61  |
   | case 6's `p+0xA = 5' BEFORE `p+0x10 = 0'                  |  449  |   0  |
   |   ... the other order                                     |  454  |  20  |
   | `u16 z' for w+0xA across state 1's flag gate              |  449  |   0  |
   |   ... naming `*(u16 *)(w + 0xA)' in both arms             |  451  |   7  |
   | state 1's `p+0x10 = 0' ONCE after the if/else             |  449  |   0  |
   |   ... duplicated into both arms                           |  448  |   6  |
   | the arm-B `i = 1' hoisted ahead of the three vv stores    |  449  |   0  |
   |   ... left in the `for' init                              |  449  |   2  |
   | `(D_8006CCD8[dir] + 1) * 32'                              |  449  |   0  |
   |   ... `((D_8006CCD8[dir] + 1) << 5)'                      |  449  |   2  |
   | func_s28_80024024 reached through a THREE-argument alias  |  449  |   0  |
   |   ... the landed two-argument name at that site           |  449  |   1  |

   THE TWIN'S DEVICE IS FREE HERE AND THE TWO FACTS ARE CONJOINED, which is
   the thing to carry forward. s25 prices `do { v = &vv; } while (0);' at 5
   rows and explains it as a scheduling barrier that lets reorg steal
   `addiu $a3, $sp, 0x10' into the `beqz .Ls28_80025B3C' delay slot. On the
   FIRST compile here, with a separate `u8 *g', the residue was exactly 10
   rows: 5 for the barrier and 5 for `g' holding $a2 where the target holds
   $a3. Adding the wrapper took it to 5; sharing ONE `v' across both arms took
   it to 0 -- and with the shared `v' the wrapper is worth NOTHING (449 / 0
   with it, without it, and with `if (1)'). So the shared allocno subsumes the
   barrier: `v' picks up the lock-on arm's set and four references, which is
   what gives it $a3 and what gives the pointer assignment a scheduling anchor
   of its own. The device is NOT in the landed body. s13 prices the same
   shared-local fact at 9 rows on its body and reaches the same conclusion by
   a different route ($s0 rather than $a3), so this is three bodies agreeing.

   WHERE THE DONOR'S PRICES DIVERGE FROM MINE, all in the same direction --
   this copy is far more sensitive to the walk block's arithmetic than either
   of the other two banks: `& 0xFFFFFFC0' is 61 rows here against s25's 7 and
   s13's 7, and `* 64' is 79 against s25's 29 and s13's 7. Both are SELECTION
   facts in the brief's taxonomy and both HOLD in sign; it is only the
   magnitude that does not transfer, because this body's 32-bit velocity
   arithmetic puts the whole aim block downstream of the mask's `andi' pair.
   Treat a donor's magnitude as a lead and its SIGN as the fact.

   MEASURED AND FREE, so none of these is why this lands: `oy = yy; ox = xx;'
   at the bottom of the walk loop in either order (s25 records it free,
   s13 records its own order at 21 rows -- s25's reading is the one that
   reproduces here); `(u16)D_8006CCD8[dir]' for `D_8006CCD8[dir] & 0xFFFF';
   states 2 and 6's three adds as plain casts off `b' rather than
   `b->unk_00 += b->unk_0C'; cases 3 and 4's two stores in either order;
   case 5's `*(u16 *)(p - 0x2) |= 0x8000' through an `s16 *' instead;
   `(s16)h > 0x200' for `>= 0x201'; an explicit `default: break;'; and BOTH
   declaration-order permutations tried (`v' ahead of `w', and `ox'/`oy'
   exchanged around `out[4]'), which rules out the pseudo-number tie-break
   and leaves the statement order as the whole of the ordering facts above. */
void func_s28_80025738(u8 *p, UnkVec *b, u8 *c)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *q4;
    UnkVec *v;
    u16 z;
    s32 i;
    s32 h;
    s32 cx;
    s32 cy;
    s16 xx;
    s16 yy;
    s16 ox;
    UnkVec vv;
    u16 out[4];
    s16 oy;

    s = *(u8 **)(p + 0x0);
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0xE) = (*(u16 *)(s + 0x2A) >> 9) & 7;
        *(s32 *)(c + 0xC) = 0x808080;
        /* fall through */
    case 1:
        o = s - 0x20;
        if (func_8003DE58(*(u8 **)(*(u8 **)(o + 0xC) + 0x8), (UnkSlot *)*(u8 **)(o + 0xC),
                          (s16 *)out, 0) == 0 &&
            (*(u16 *)(*(u8 **)(o + 0xC) + 0x14) & 0x8000) == 0)
        {
            break;
        }
        w = *(u8 **)(o + 0x8);
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(w + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(w + 0x6);
        z = *(u16 *)(w + 0xA);
        *(s16 *)((u8 *)b + 0xA) = z;
        if ((*(u16 *)(*(u8 **)(o + 0xC) + 0x14) & 0x8000) == 0)
        {
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + out[0];
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + out[1];
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + out[2];
        }
        else
        {
            *(s16 *)((u8 *)b + 0xA) = z - 0x40;
        }
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        if (*(u8 **)(s + 0x60) != NULL)
        {
            *(s32 *)(p + 0x18) = (s32) * (u8 **)(s + 0x60);
            v = (UnkVec *)*(u8 **)(*(u8 **)(s + 0x60) - 0x18);
            out[0] = abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)b + 0x6));
            out[2] = abs(*(s16 *)(*(u8 **)(s + 0x60) + 0x88) - *(s16 *)((u8 *)b + 0xA));
            *(s16 *)(p + 0x12) = out[0];
            for (i = 1; i < 3; i++)
            {
                if ((s16)out[i] > *(s16 *)(p + 0x12))
                {
                    *(s16 *)(p + 0x12) = out[i];
                }
            }
            *(s16 *)(p + 0x12) = (s16) * (u16 *)(p + 0x12) >> 4;
            if (*(s16 *)(p + 0x12) == 0)
            {
                *(s16 *)(p + 0x12) = 1;
            }
            b->unk_0C = (v->unk_00 - b->unk_00) / *(s16 *)(p + 0x12);
            b->unk_10 = (v->unk_04 - b->unk_04) / *(s16 *)(p + 0x12);
            b->unk_14 = ((*(s16 *)(*(u8 **)(s + 0x60) + 0x88) << 16) - b->unk_08) /
                        *(s16 *)(p + 0x12);
            func_s28_800253D0(p, b);
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        else
        {
            q4 = *(u8 **)(o + 0xC);
            xx = q4[0x24];
            yy = q4[0x25];
            ox = xx;
            oy = yy;
            for (i = 0; i < 8; i++)
            {
                cx = xx;
                cy = yy;
                if ((s16)func_800A44E0((cx << 6) & 0xFFFFFFC0, (cy << 6) & 0xFFFFFFC0,
                                       *(s16 *)(s + 0x88),
                                       (s16)(*(s16 *)(p + 0xE) << 9)) != 0)
                {
                    break;
                }
                h = func_800BCB04(((cx + D_8006CCD8[*(s16 *)(p + 0xE)]) << 6) + 0x20 & 0xFFE0,
                                  ((cy + D_8006CCE8[*(s16 *)(p + 0xE)]) << 6) + 0x20 & 0xFFE0,
                                  (s16)(*(u16 *)(s + 0x88) - 0x20));
                if ((s16)h >= 0x201)
                {
                    break;
                }
                if ((s16)(h - *(u16 *)(s + 0x88)) < -0x3F)
                {
                    break;
                }
                xx = xx + (D_8006CCD8[*(s16 *)(p + 0xE)] & 0xFFFF);
                yy = yy + (D_8006CCE8[*(s16 *)(p + 0xE)] & 0xFFFF);
                oy = yy;
                ox = xx;
            }
            v = &vv;
            i = 1;
            *(s16 *)((u8 *)v + 0x2) = ox * 64 + (D_8006CCD8[*(s16 *)(p + 0xE)] + 1) * 32;
            *(s16 *)((u8 *)v + 0x6) = oy * 64 + (D_8006CCE8[*(s16 *)(p + 0xE)] + 1) * 32;
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)b + 0xA) + 0x20;
            out[0] = abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)b + 0x6));
            out[2] = abs(*(s16 *)((u8 *)v + 0xA) - *(s16 *)((u8 *)b + 0xA));
            *(s16 *)(p + 0x12) = out[0];
            for (; i < 3; i++)
            {
                if ((s16)out[i] > *(s16 *)(p + 0x12))
                {
                    *(s16 *)(p + 0x12) = out[i];
                }
            }
            *(s16 *)(p + 0x12) = (s16) * (u16 *)(p + 0x12) >> 4;
            if (*(s16 *)(p + 0x12) == 0)
            {
                *(s16 *)(p + 0x12) = 1;
            }
            b->unk_0C = (v->unk_00 - b->unk_00) / *(s16 *)(p + 0x12);
            b->unk_10 = (v->unk_04 - b->unk_04) / *(s16 *)(p + 0x12);
            b->unk_14 = (v->unk_08 - b->unk_08) / *(s16 *)(p + 0x12);
            func_s28_80025614(p, b);
            *(s16 *)(p + 0xA) = 6;
        }
        *(s16 *)(p + 0x10) = 0;
        break;
    case 2:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            func_800A56E0(0x300);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x10) >= 0x10)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        if (*(s16 *)(p + 0x10) >= 0x30)
        {
            func_s28_80024024_3(*(u8 **)(s + 0x60), p[0x9], s);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if (*(s16 *)(p + 0x14) == 0)
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    case 6:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            *(s16 *)(p + 0xA) = 5;
            *(s16 *)(p + 0x10) = 0;
        }
        break;
    }
    *(s16 *)(p + 0x14) = 0;
}
