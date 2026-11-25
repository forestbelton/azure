#include "common.h"

typedef union UnkDngS14Word
{
    s32 w;
    s16 h[2];
} UnkDngS14Word;
typedef struct UnkDngS14LineSP
{
    u8 pad_00[0x18];
    u8 *unk_18;
    u8 pad_1C[0x48];
    u16 unk_64;
    u16 unk_66;
    u16 unk_68;
    u8 pad_6A[0x2];
    u16 unk_6C;
    u16 unk_6E;
    u16 unk_70;
    u8 pad_72[0x12];
    s32 unk_84;
    s32 unk_88;
    u8 pad_8C[0x28];
    s32 unk_B4;
    u8 pad_B8[0x20];
    u16 unk_D8;
    u16 unk_DA;
    u16 unk_DC;
    u16 unk_DE;
    u8 pad_E0[0x14];
    s32 unk_F4;
    s32 unk_F8;
} UnkDngS14LineSP;
s16 func_800A2CB8(u8 *, u8 *);
s16 func_800A41F0(u8 *);
u16 func_800A6D30();
s32 func_800C87C4(u8 *, s16, s16);
extern u8 D_800E3D68;
extern u8 *D_800E3D7C;

/* 75 words, 0 differing. Walks the resident actor ring at D_800E3D7C, and
   for each actor the cursor admits whose grid cell is within 2 of (x, y)
   calls the 0x800C87C4 report with a 0x10 chance -- 0xFF when D_800E3D68
   is saturated -- and a rank of (d >> 2) + 0x10 plus two random bits. */
UnkNode *func_8003FC64(s32);
void func_8003DB94(void *, void *, s32);
s32 func_8004491C(UnkNode *, u32);
void func_800478B8(u8 *);
s32 func_800644B8(s32);
s32 func_80064584(s32);
s16 func_800A45D8(u16, u16, s32);
void func_s15_80024264(u8 *v, u8 *q, u8 k);
void func_s15_8002418C(u8 *p, UnkVec *q, u8 *m);
void func_s15_80024384(u8 *p, UnkVec *b, u8 *c);
void func_s15_80024730(u8 *, UnkVec *, u8 *);
extern u8 D_800DEDB0[];
extern u8 D_80045340[];
extern u8 D_s15_80025368[];
extern u32 D_800814A0;

void func_s15_80024060(s16 x, s16 y, u8 *c, u8 d)
{
    u8 *p;
    u8 *e;
    s32 t;
    s32 t2;
    s16 chance;
    s16 rank;

    p = D_800E3D7C;
    do
    {
        if ((s16)func_800A2CB8(c, p) != 0)
        {
            e = *(u8 **)(p - 0x14);
            t = e[0x24] - x;
            if (t < 0)
            {
                t = -t;
            }
            if (t < 3)
            {
                t2 = e[0x25] - y;
                if (t2 < 0)
                {
                    t2 = -t2;
                }
                if (t2 < 3 && (s16)func_800A41F0(p) != 0)
                {
                    rank = (d >> 2) + 0x10 + (func_800A6D30() & 3);
                    chance = 0x10;
                    if (D_800E3D68 == 0xFF)
                    {
                        chance = 0xFF;
                    }
                    func_800C87C4(p, chance, rank);
                }
            }
        }
        p = *(u8 **)(p + 0x5C) + 0x20;
    } while (p != D_800E3D7C);
}

/* The fading-marker step: bumps the owner's +0x14 hit counter, raises the
   marker's +0x1C/+0x1E scale by 0x400 and fades its three colour bytes by 4,
   retires the node when the fade reaches zero, pulls the vector's z down by 8
   and republishes through func_800478B8, then retires again if the
   renderable's +0x14 shows 0x6000. 54 words, 0 differing.

   THE WHOLE +0x1E/+0x1C READ-MODIFY-WRITE MUST BE COMPLETE -- LOAD, ADD AND
   BOTH STORES -- BEFORE THE +0xE/+0xD/+0xC BYTE BLOCK. Split so that only the
   load and the add precede the byte block and the two `sh' follow it, the two
   independent chains come out transposed: `lbu'/`lhu' and the two `addiu'
   exchanged, 54 words / 4 aligned differences, in BOTH chain orders and under
   all 16 combinations of array-ref versus cast spelling on the two loads and
   the two stores (bit-identical residue every time, so that is one fact, not
   sixteen). Whether the stored value is carried in an `s16' local or re-read
   from memory for the +0x1C store is FREE -- measured 0 both ways on this
   body at 54 words. An earlier run priced the local form at 4; that figure
   was taken while the gate below was still wrong and is RETRACTED.

   THE GATE NEEDS THE `(u8)' TRUNCATION. `if ((u8)a == 0)' is the target's
   `andi $v0, $v0, 0xFF'; `if (a == 0)' on the `s32' tests the raw value,
   53 words / 13 aligned differences. `u8 a;' with a bare `a == 0' is the
   same code, measured 0.

   Shape read off csrc/ovl_dng_b30.c's func_b30_801745F0 for the tail gate
   (`func_800478B8(c)' one-argument, then the 0x6000 test) and off the
   mirror-image sibling stub func_s15_80024730 for the scale/fade pairing. */
void func_s15_8002418C(u8 *p, UnkVec *q, u8 *m)
{
    s32 a;

    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(m + 0x1E) = *(u16 *)(m + 0x1E) + 0x400;
    *(s16 *)(m + 0x1C) = *(u16 *)(m + 0x1E);
    a = m[0xE] - 4;
    m[0xE] = a;
    m[0xD] = a;
    m[0xC] = a;
    if ((u8)a == 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    *(s16 *)((u8 *)q + 0xA) = *(u16 *)((u8 *)q + 0xA) - 8;
    func_800478B8(m);
    if (*(u16 *)(m + 0x14) & 0x6000)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x212 spawner for func_s15_8002418C's fading marker: allocates the node,
   stores the owner at +0x20, dresses the renderable from D_800DEDB0 with the
   caller's colour byte, scale 0x1800 and tpage 0x7E0B, then seeds the mover
   from `q' with the z jittered by rand() % 0x11 - 8. 72 words, 0 differing,
   first compile, read off csrc/ovl_dng_s14.c's func_s14_8002425C (0.803) with
   csrc/ovl_dng_s27.c's func_s27_800241F8 for the statement order.

   The divisor really is 0x11: the listing's `lui 0x7878'/`ori 0x7879' magic
   multiply with `mfhi'/`sra 3' and a `* 17' back-multiply is expand_divmod's
   strength reduction of a LITERAL 17, so no variable divide is involved.

   Measured and FREE at 72 words / 0 differing, so neither is why this lands:
   a separate `m2' local for `n->unk_08' instead of reusing `m' (0), and
   writing `rand() % 0x11 + *(u16 *)(q + 0xA) - 8' (0). Note the first of
   those INVERTS on func_s15_800247DC 100 words below, where reusing `m'
   costs 60 words / 9 differences -- same unit, opposite answer. */
void func_s15_80024264(u8 *v, u8 *q, u8 k)
{
    UnkNode *n;
    u8 *m;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s15_8002418C;
        m = (u8 *)n->unk_0C;
        *(u8 **)((u8 *)n + 0x20) = v;
        m[0xE] = k;
        m[0xD] = k;
        m[0xC] = k;
        func_8003DB94(m, D_800DEDB0, 0);
        *(s16 *)(m + 0x1E) = 0x1800;
        *(s16 *)(m + 0x1C) = 0x1800;
        *(s16 *)(m + 0x12) = 0x7E0B;
        *(u16 *)(m + 0x10) |= 0x60;
        *(u16 *)(m + 0x14) |= 0x10C;
        func_8004491C(n, (u32)D_80045340);
        m = (u8 *)n->unk_08;
        *(s16 *)(m + 0x2) = *(u16 *)(q + 0x2);
        *(s16 *)(m + 0x6) = *(u16 *)(q + 0x6);
        *(s16 *)(m + 0xA) = *(u16 *)(q + 0xA) + rand() % 0x11 - 8;
    }
}

/* The three-spark ring handler: states 0..3 off a compare chain (no jump
   table), with states 1 and 2 each sweeping three points of a circle --
   func_800644B8/func_80064584 at p+0x8 plus a 16.16 angle, scaled by p+0x6
   and offset from the mover's +0xE/+0x12 -- and spawning a marker through
   func_s15_80024264 wherever func_800A45D8 reports the cell clear. Default
   parks the state at 3. 192 words, 0 differing. No twin at any ratio and no
   dngnear donor (ovl_dng_s15 has no landed in-unit neighbour to invert), so
   this is decompiled from the listing.

   THE LOOP COUNTER MUST BE ONE LOCAL SHARED BY BOTH ARMS WHILE THE 16.16
   ANGLE ACCUMULATOR MUST BE DECLARED INSIDE EACH ARM. That is the whole
   residue and it is `allocno_compare' crossing a power of two: the shared
   counter collects 7 weighted references per arm, 14 in all, so
   floor_log2(14) is 3 where each per-arm accumulator stays at 7 and scores 2.
   Measured, all at 192 words:
     counter shared / accumulator per arm                      0  <- kept
     both at function scope                                   12
     accumulator shared / counter per arm                     12
     both at function scope + `do { i++; } while (0)'          0
   The last row is the same win bought with a device, so the SCOPE is what
   makes the device unnecessary; there is no device in this body. Every other
   lever tried moved nothing: 16 statement orders for the two initialisers,
   `while' versus `for' versus a comma initialiser, `i = 0; j = i;' and
   `j = 0; i = j;', `3 > i' for the test, and a `do { } while (0)' on the
   accumulator instead (32). All of those are either 4 or 12 and the 4s were
   measured before this body reached 0, so only the table above is priced. */
void func_s15_80024384(u8 *p, UnkVec *b, u8 *c)
{
    s32 i;
    s32 s;

    *(s16 *)(*(u8 **)(p + 0xC) + 0x14) = *(u16 *)(*(u8 **)(p + 0xC) + 0x14) + 1;
    *(s16 *)(p + 0x2) = *(u16 *)(p + 0x2) + 1;
    switch (*(s16 *)(p + 0x0))
    {
    case 0:
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) - 4;
        func_s15_80024264(*(u8 **)(p + 0xC), (u8 *)b, p[0x10]);
        if (*(s16 *)(p + 0x2) >= 8)
        {
            *(s16 *)(p + 0x2) = 0;
            *(s16 *)(p + 0x0) = *(u16 *)(p + 0x0) + 1;
        }
        break;
    case 1:
    {
        s32 j;

        i = 0;
        j = 0;
        *(s16 *)(p + 0x6) = *(u16 *)(p + 0x6) + 6;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 0x10;
        *(s16 *)(p + 0x8) = *(u16 *)(p + 0x8) + *(u16 *)(p + 0xA);
        for (; i < 3; i++)
        {
            s = j >> 16;
            *(s16 *)((u8 *)b + 0x2) =
                *(u16 *)((u8 *)b + 0xE) +
                (((func_800644B8(*(s16 *)(p + 0x8) + s) >> 4) * *(s16 *)(p + 0x6)) >> 8);
            *(s16 *)((u8 *)b + 0x6) =
                *(u16 *)((u8 *)b + 0x12) +
                (((func_80064584(*(s16 *)(p + 0x8) + s) >> 4) * *(s16 *)(p + 0x6)) >> 8);
            if (func_800A45D8(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                              *(s16 *)((u8 *)b + 0xA)) == 0)
            {
                func_s15_80024264(*(u8 **)(p + 0xC), (u8 *)b, p[0x10]);
            }
            j += 0x5550000;
        }
        if (*(s16 *)(p + 0x2) >= 0x20)
        {
            *(s16 *)(p + 0x2) = 0;
            *(s16 *)(p + 0x0) = *(u16 *)(p + 0x0) + 1;
        }
        break;
    }
    case 2:
    {
        s32 j;

        i = 0;
        j = 0;
        *(s16 *)(p + 0x6) = *(u16 *)(p + 0x6) + 6;
        *(s16 *)(p + 0x8) = *(u16 *)(p + 0x8) + *(u16 *)(p + 0xA);
        for (; i < 3; i++)
        {
            s = j >> 16;
            *(s16 *)((u8 *)b + 0x2) =
                *(u16 *)((u8 *)b + 0xE) +
                (((func_800644B8(*(s16 *)(p + 0x8) + s) >> 4) * *(s16 *)(p + 0x6)) >> 8);
            *(s16 *)((u8 *)b + 0x6) =
                *(u16 *)((u8 *)b + 0x12) +
                (((func_80064584(*(s16 *)(p + 0x8) + s) >> 4) * *(s16 *)(p + 0x6)) >> 8);
            if (func_800A45D8(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                              *(s16 *)((u8 *)b + 0xA)) == 0)
            {
                func_s15_80024264(*(u8 **)(p + 0xC), (u8 *)b, p[0x10]);
            }
            j += 0x5550000;
        }
        p[0x10] = p[0x10] - 8;
        if (*(s16 *)(p + 0x2) >= 8)
        {
            *(s16 *)(p + 0x2) = 0;
            *(s16 *)(p + 0x0) = *(u16 *)(p + 0x0) + 1;
        }
        break;
    }
    case 3:
        (*(u8 **)(p + 0xC))[0x16] = 1;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    default:
        *(s16 *)(p + 0x0) = 3;
        break;
    }
}

/* The 0x212 spawner for func_s15_80024384's spark ring: allocates the node,
   zeroes the handler state block at n+0x20, parks the owner at +0x2C with the
   phase byte from p[0x9] and 0x40 at +0x30, then copies the mover's three
   position halfwords into BOTH the velocity (+0xE/+0x12/+0x16) and position
   (+0x2/+0x6/+0xA) slots. 43 words, 0 differing.

   EACH VELOCITY/POSITION PAIR IS A READ-BACK OF THE HALFWORD JUST STORED,
   NOT A LOCAL. `*(s16 *)(m + 0x2) = *(u16 *)(m + 0xE);' after storing q's
   value into m+0xE is the target's single `lhu' per pair; carrying the value
   in an `s16' local is 45 words / 13 aligned differences and three separate
   locals collapse to 40 words / 18.

   THE p[0x9] LOAD MUST BE A LOCAL ASSIGNED BETWEEN THE +0x22 AND +0x26 ZERO
   STORES. Its position is the lever, not the local: sweeping the eight slots
   of the zeroing block gives 4 / 2 / 0 / 2 / 3 / 4 / 5 and 44 words / 17 with
   the load left inline at its own store, because written there it is a
   successor of every one of those may-alias stores and pays an unfilled load
   delay. Priced after this body reached 0; an earlier sweep read one higher
   at every slot and is superseded.

   Measured and FREE at 43 words / 0 differing: writing the first zero store
   as `*(s16 *)((u8 *)n + 0x20) = 0' rather than through the `t' pointer --
   cse rewrites `0($v0)' back to `0x20($a1)' either way, so the two spellings
   emit the identical word. */
void func_s15_80024684(u8 *p, u8 *q)
{
    UnkNode *n;
    u8 *t;
    u8 *m;
    s16 h;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s15_80024384;
        t = (u8 *)n + 0x20;
        *(s16 *)t = 0;
        *(s16 *)(t + 0x2) = 0;
        h = p[0x9];
        *(s16 *)(t + 0x6) = 0;
        *(s16 *)(t + 0x8) = 0;
        *(s16 *)(t + 0xA) = 0;
        *(u8 **)(t + 0xC) = p;
        t[0x10] = 0x40;
        *(s16 *)(t + 0x4) = h;
        m = (u8 *)n->unk_08;
        *(s16 *)(m + 0xE) = *(u16 *)(q + 0x2);
        *(s16 *)(m + 0x2) = *(u16 *)(m + 0xE);
        *(s16 *)(m + 0x12) = *(u16 *)(q + 0x6);
        *(s16 *)(m + 0x6) = *(u16 *)(m + 0x12);
        *(s16 *)(m + 0x16) = *(u16 *)(q + 0xA);
        *(s16 *)(m + 0xA) = *(u16 *)(m + 0x16);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s15", func_s15_80024730);

/* The 0x212 spawner for func_s15_80024730's rising marker: same shape as
   func_s15_80024264 above but dressed from the unit-local D_s15_80025368 at
   scale 0x4000 with colour 0x80, and the mover seeded straight from `q' with
   no jitter. 59 words, 0 differing, first compile, read off
   csrc/ovl_dng_s27.c's func_s27_800241F8 (0.906).

   THE SEPARATE `m2' LOCAL FOR `n->unk_08' IS LOAD-BEARING HERE: reusing `m'
   is 60 words / 9 aligned differences. That is the opposite of
   func_s15_80024264, where the same choice measures 0 either way.

   AND THE TWO `|=' GO IN OFFSET ORDER: `+0x10 |= 0x60' before
   `+0x14 |= 0x10C' is the target's interleaved `lhu'/`lhu'/`ori'/`ori';
   swapped it is 59 words / 9 aligned differences. */
void func_s15_800247DC(u8 *v, u8 *q)
{
    UnkNode *n;
    u8 *m;
    u8 *m2;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s15_80024730;
        m = (u8 *)n->unk_0C;
        *(u8 **)((u8 *)n + 0x20) = v;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        func_8003DB94(m, D_s15_80025368, 0);
        *(s16 *)(m + 0x1E) = 0x4000;
        *(s16 *)(m + 0x1C) = 0x4000;
        *(s16 *)(m + 0x12) = 0x7E0B;
        *(u16 *)(m + 0x10) |= 0x60;
        *(u16 *)(m + 0x14) |= 0x10C;
        func_8004491C(n, (u32)D_80045340);
        m2 = (u8 *)n->unk_08;
        *(s16 *)(m2 + 0x2) = *(u16 *)(q + 0x2);
        *(s16 *)(m2 + 0x6) = *(u16 *)(q + 0x6);
        *(s16 *)(m2 + 0xA) = *(u16 *)(q + 0xA);
    }
}

/* The beam line: one semi-transparent LINE_G2 from the payload's +0x22/+0x26
   /+0x2A origin to its +0x3A/+0x3E/+0x42 endpoint, interpolated by the +0x10
   phase and coloured from a 15-entry table, projected through the 0x1F800000
   scratchpad and chained into D_80083160's +0x8D0 primitive ring with the
   addPrim macro. 259 words, 0 differing.

   THIS FUNCTION OWNS jtbl_s15_80024008, 15 entries: cases 0..6 share the
   forward arm, case 7 is the flat middle, cases 8..14 the reverse arm.

   FOUR SPELLING FACTS, each re-measured on this body, all 259 words:

   THE SWITCH INDEX HAS TO BE AN INT-TYPED TREE. `switch (*(s16 *)(p + 0x10))'
   with cases 1..15 does the minval subtraction in HImode and costs two words
   (`sll 16' / `sra 16' after the `addiu -1'): gcc 2.7.2's
   c_expand_start_case runs get_unwidened over the promoted s16 and strips the
   promotion back off, so index_type stays `short'. Subtracting in the source
   and numbering the cases 0..14 keeps it `int' and gives the target's
   `lh' / `addiu -1' / `sltiu 0xF'. 260 words, 215 differing spelled the
   other way.

   THE INTERPOLATOR IS A 16.16 UNION, AND THAT IS THE `andi 0xFFFF' / `or' /
   `and 0xFFFF0000' TRIPLE. `u.h[1] = delta; u.h[0] = 0;' is two
   store_bit_field inserts into one register-resident word -- bitpos 16 then
   bitpos 0 -- and the first axis collapses to a bare `sll 16' only because
   the previous value is undefined there. Without the union the triple is
   simply absent: 255 words, 139 differing. The shift and the multiply must
   then be SEPARATE statements (`u.w >>= 3; u.w *= k;'): as one expression the
   shift lands in a different register from `u.w' and it is 107 differing.

   THE TWO COLOUR MULTIPLIERS ARE VARIABLES. `(st + 1) * 0xF' synthesises at
   expand time (`sll 4' / `subu'); `c = 0xF; (st + 1) * c' gives the target's
   `addiu $v0, $zero, 0xF' and `mult $v1, $v0', and TWO separate locals are
   needed because cse has to keep the 0xF register alive for the reverse
   arm's `0xF - st'. Read as macro residue.

   THE THREE COLOURS ARE COMPUTED INTO LOCALS BEFORE ANY OF THEM IS STORED,
   so the single `lh 0x10($t0)' serves both multiplies and the green ramp; a
   store to the primitive invalidates cse's memory table and each later read
   reloads. And in the forward arm `l->g1 = 0x40 - g;' comes BEFORE the three
   0x40 stores: after them, 0x40 is materialised twice and the function is
   260 words / 37 differing. That one statement move is the last 37.

   AND THE TWO SCRATCHPAD ARGUMENT ADDRESSES ARE `(u32)sp | 0x64', NOT THE
   INTEGER CONSTANT. Written `(SVECTOR *)0x1F800064' the four addresses each
   get their own `lui' instead of the target's `ori $aN, $s3, imm' off the
   pointer local: 264 words, 91 differing. The two OUTPUT addresses stay
   `&sp->unk_84' / `&sp->unk_88', which is where the target's two `addiu'
   come from.
   THIS COPY'S OWN CONSTANT: 0x22 -> 0x1a, 0x26 -> 0x1e, 0x2a -> 0x22, 0x3a -> 0x32, 0x3e -> 0x36, 0x42 -> 0x3a.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
s32 func_s15_800248C8(u8 *p)
{
    UnkDngS14LineSP *sp = (UnkDngS14LineSP *)0x1F800000;
    u8 *base;
    LINE_G2 *l;
    UnkDngS14Word u;

    base = D_80083160.unk_00;
    sp->unk_18 = base + 0xB0;
    l = *(LINE_G2 **)(base + 0x8D0);
    *(LINE_G2 **)(base + 0x8D0) = l + 1;
    setLineG2(l);
    sp->unk_64 = *(u16 *)(p + 0x1a);
    sp->unk_66 = *(u16 *)(p + 0x1e);
    sp->unk_68 = *(u16 *)(p + 0x22);
    sp->unk_6C = *(u16 *)(p + 0x32);
    sp->unk_6E = *(u16 *)(p + 0x36);
    sp->unk_70 = *(u16 *)(p + 0x3a);
    switch (*(s16 *)(p + 0x10) - 1)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        u.h[1] = sp->unk_6C - sp->unk_64;
        u.h[0] = 0;
        u.w >>= 3;
        u.w *= *(s16 *)(p + 0x10) + 1;
        sp->unk_6C = sp->unk_64 + (u.w >> 16);
        u.h[1] = sp->unk_6E - sp->unk_66;
        u.h[0] = 0;
        u.w >>= 3;
        u.w *= *(s16 *)(p + 0x10) + 1;
        sp->unk_6E = sp->unk_66 + (u.w >> 16);
        u.h[1] = sp->unk_70 - sp->unk_68;
        u.h[0] = 0;
        u.w >>= 3;
        u.w *= *(s16 *)(p + 0x10) + 1;
        sp->unk_70 = sp->unk_68 + (u.w >> 16);
        {
            s32 c;
            s32 c2;
            s32 r;
            s32 g;
            s32 b;

            c = 0xF;
            r = (*(s16 *)(p + 0x10) + 1) * c + 0x40;
            c2 = 0x17;
            b = (*(s16 *)(p + 0x10) + 1) * c2 + 0x40;
            g = (7 - *(s16 *)(p + 0x10)) << 3;
            l->g1 = 0x40 - g;
            l->r0 = 0x40;
            l->g0 = 0x40;
            l->b0 = 0x40;
            l->r1 = r;
            l->b1 = b;
        }
        break;
    case 7:
        l->r0 = 0x40;
        l->g0 = 0x40;
        l->b0 = 0x40;
        l->r1 = 0x7F;
        l->g1 = 0;
        l->b1 = 0xFF;
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        u.h[1] = sp->unk_6C - sp->unk_64;
        u.h[0] = 0;
        u.w >>= 3;
        u.w *= *(s16 *)(p + 0x10) - 7;
        sp->unk_64 = sp->unk_64 + (u.w >> 16);
        u.h[1] = sp->unk_6E - sp->unk_66;
        u.h[0] = 0;
        u.w >>= 3;
        u.w *= *(s16 *)(p + 0x10) - 7;
        sp->unk_66 = sp->unk_66 + (u.w >> 16);
        u.h[1] = sp->unk_70 - sp->unk_68;
        u.h[0] = 0;
        u.w >>= 3;
        u.w *= *(s16 *)(p + 0x10) - 7;
        sp->unk_68 = sp->unk_68 + (u.w >> 16);
        {
            s32 c;
            s32 c2;
            s32 r;
            s32 g;
            s32 b;

            c = 0xF;
            r = (*(s16 *)(p + 0x10) - 7) * c + 0x40;
            c2 = 0x17;
            b = (*(s16 *)(p + 0x10) - 7) * c2 + 0x40;
            g = (c - *(s16 *)(p + 0x10)) << 3;
            l->g1 = 0;
            l->g0 = g;
            l->r1 = 0x7F;
            l->b1 = 0xFF;
            l->r0 = r;
            l->b0 = b;
        }
        break;
    }
    sp->unk_F4 = RotTransPers((SVECTOR *)((u32)sp | 0x64), (s32 *)((u32)sp | 0xD8), &sp->unk_84, &sp->unk_88);
    sp->unk_F8 = RotTransPers((SVECTOR *)((u32)sp | 0x6C), (s32 *)((u32)sp | 0xDC), &sp->unk_84, &sp->unk_88);
    l->x0 = sp->unk_D8;
    l->y0 = sp->unk_DA;
    l->x1 = sp->unk_DC;
    l->y1 = sp->unk_DE;
    sp->unk_B4 = (sp->unk_F4 + sp->unk_F8) / 2;
    if ((u32)sp->unk_B4 < 0x1E0)
    {
        addPrim(sp->unk_18 + sp->unk_B4 * 4, l);
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s15", func_s15_80024CD4);
