#include "common.h"

s32 rand(void);

typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
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
typedef struct UnkDng74334SP
{
    u8 pad_00[0xC];
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    Unk3DBD0W unk_18;
    u8 pad_1C[0x8];
    u8 *unk_24;
    u16 unk_28;
    u8 pad_2A[0xA];
    VECTOR unk_34;
    u8 pad_44[0x10];
    MATRIX unk_54;
    MATRIX unk_74;
    u8 pad_94[0x10];
    SVECTOR unk_A4;
    u8 pad_AC[0x4];
    SVECTOR unk_B0[4];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDng74334SP;
void func_s46_80024198(u8 *p, UnkVec *v, UnkSlot *s, s16 d);

UnkNode *func_8003FC64(s32);
s32 func_8004491C(UnkNode *, u32);
void func_s46_800249FC();
extern u8 D_80045340[];
extern u8 D_80025B48[];
extern s16 D_80025B60;
extern u8 D_800DE870[];
void func_8003DB94(void *, void *, s32);
void func_800478B8(u8 *);
s32 func_80064584(s32);
s32 func_800644B8(s32);
typedef struct UnkDngS46Name12
{
    u8 b[0xC];
} UnkDngS46Name12;

/* The bank's "random value below n": six masked draws from rand, picked by
   the size of n, each retried until it falls under n -- with a negative
   clamp on n that the twins do not have.
   75 words, 0 differing, FIRST COMPILE.

   The twin is the LANDED func_s72_8002458C in csrc/ovl_dng_s72.c (73 words,
   dngtwin 0.811), and func_s42_80024590 / func_b31f_8016ADB8 / func_b25_
   8017165C are the same 73-word body.  The whole of this copy's +2 words is
   the `bgez $s0' / `addu $s0, $zero, $zero' pair at the head: `if (n < 0)
   n = 0;'.  Same -0x18 frame, same single save $s0, so the twin's facts
   transfer.  func_80069EF8 is `rand' (splat/common_syms.txt), so no
   declaration is needed and no address is spelled.

   The twin stated two facts and priced neither; both MEASURED HERE and both
   HOLD:
     - `r = 0' is initialised BEFORE the `n != 0' test and the LAST `do' loop
       has NO `return' of its own, so the n == 0 exit and the last arm's
       fall-through share one `addu $v0, $v1, $zero'.  Giving the last loop
       its own `return r;' is 76 words / 70 differing.
     - the retry test is a signed `slt', so n and r are both s32.
   One fact of this copy's own, measured: the clamp must come BEFORE `r = 0'.
   After it, it is 75 words / 5 differing -- the clamp's branch then lands
   with $v1 already live and reorg fills the delay slots the other way. */
s32 func_s46_8002406C(s32 n)
{
    s32 r;

    if (n < 0)
    {
        n = 0;
    }
    r = 0;
    if (n != 0)
    {
        if (n < 5)
        {
            do
            {
                r = (rand() & 0xC) >> 2;
            } while (r >= n);
            return r;
        }
        if (n < 0x11)
        {
            do
            {
                r = (rand() & 0xF0) >> 4;
            } while (r >= n);
            return r;
        }
        if (n < 0x41)
        {
            do
            {
                r = (rand() & 0xFC) >> 2;
            } while (r >= n);
            return r;
        }
        if (n < 0x101)
        {
            do
            {
                r = (rand() & 0xFF0) >> 4;
            } while (r >= n);
            return r;
        }
        if (n < 0x1001)
        {
            do
            {
                r = rand() & 0xFFF;
            } while (r >= n);
            return r;
        }
        do
        {
            r = rand() & 0xFFFF;
        } while (r >= n);
    }
    return r;
}

/* One semi-transparent gouraud quad per slot element, with its own DR_MODE:
   compose the slot's rotation with the camera matrix in the 0x1F800000
   scratchpad, publish a 0xC-byte DR_MODE on the resident OT root, then
   RotAverage4 the four SVECTORs taken off the payload's +0x20..+0x37 into a
   0x24-byte POLY_G4 at the +0x8D0 cursor and -- when the OT index is in range
   and at least one corner is on screen -- give it the payload's four colour
   words, the element's +0x1 flag byte and a second DR_MODE.
   310 words, 0 differing, FIRST COMPILE.

   The twin is the LANDED func_s67_8002408C in csrc/ovl_dng_s67.c (310 words,
   dngtwin 0.974): identical statement for statement except that this copy
   reads its four vertices off INTERLEAVED triples at p+0x20 (stride 6) where
   s67 reads them at p+0x54, and takes its four colour words off p+0x0/+0x4/
   +0x8/+0xC where s67 uses p+0xC/+0x10/+0x14/+0x18.  Same -0x70 frame, same
   nine saves s0..s7 in the same order, so its allocation facts transfer.

   The twin took three facts as given and never priced them; all three are
   MEASURED HERE, on this body:
     - HOLDS: the four on-screen tests are `|' and not `||' -- as `||' this
       is 311 words / 143 differing, because short-circuiting splits the
       four sltiu pairs into separate blocks.
     - FREE, so it is NOT the reason this lands: the scale vector being ONE
       CHAINED assignment.  Three separate stores written vz, vy, vx also
       measure 310 words / 0 differing.  The twin's claim is weaker than
       stated; only the STORE ORDER matters and the chain merely supplies it.
     - HOLDS: `e = (u8 *)s->unk_08;' comes BEFORE `sp->unk_28 = s->unk_14;'.
       Swapped it is 310 words / 3 differing.
   The fourth twin fact (`&m' does not go through a pointer local) is NOT
   APPLICABLE as a separate spelling here: `m' is only ever passed by address
   to ReadRotMatrix and CompMatrix, so there is no local to introduce. */
void func_s46_80024198(u8 *p, UnkVec *v, UnkSlot *s, s16 d)
{
    UnkDng74334SP *sp = (UnkDng74334SP *)0x1F800000;
    Unk80083160 *g;
    MATRIX m;
    u8 *q;
    u8 f;
    u8 *e;

    g = &D_80083160;
    sp->unk_24 = g->unk_00 + 0xB0;
    sp->unk_74.t[0] = *(s16 *)((u8 *)v + 0x2);
    sp->unk_74.t[1] = *(s16 *)((u8 *)v + 0x6);
    sp->unk_74.t[2] = *(s16 *)((u8 *)v + 0xA);
    s->unk_14 |= 0x8000;
    PushMatrix();
    sp->unk_34.vx = sp->unk_34.vy = sp->unk_34.vz = 0x2000;
    sp->unk_A4.vx = s->unk_16;
    sp->unk_A4.vz = s->unk_1A;
    sp->unk_A4.vy = s->unk_18;
    RotMatrix(&sp->unk_A4, &sp->unk_74);
    ReadRotMatrix(&m);
    CompMatrix(&m, &sp->unk_74, &sp->unk_54);
    ScaleMatrix(&sp->unk_54, &sp->unk_34);
    SetTransMatrix(&sp->unk_54);
    SetRotMatrix(&sp->unk_54);
    e = (u8 *)s->unk_08;
    sp->unk_28 = s->unk_14;
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q + 0xC;
    func_80067F20((u32 *)q, 0, 1, GetTPage(0, 0, 0, 0), 0);
    addPrim(g->unk_00 + 0xB0, q);
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q + 0x24;
    sp->unk_B0[0].vx = *(u16 *)(p + 0x20);
    sp->unk_B0[1].vx = *(u16 *)(p + 0x26);
    sp->unk_B0[2].vx = *(u16 *)(p + 0x2C);
    sp->unk_B0[3].vx = *(u16 *)(p + 0x32);
    sp->unk_B0[0].vy = *(u16 *)(p + 0x22);
    sp->unk_B0[1].vy = *(u16 *)(p + 0x28);
    sp->unk_B0[2].vy = *(u16 *)(p + 0x2E);
    sp->unk_B0[3].vy = *(u16 *)(p + 0x34);
    sp->unk_B0[0].vz = *(u16 *)(p + 0x24);
    sp->unk_B0[1].vz = *(u16 *)(p + 0x2A);
    sp->unk_B0[2].vz = *(u16 *)(p + 0x30);
    sp->unk_B0[3].vz = *(u16 *)(p + 0x36);
    sp->unk_100 = RotAverage4(&sp->unk_B0[0], &sp->unk_B0[1], &sp->unk_B0[2], &sp->unk_B0[3],
                              (long *)(q + 0x8), (long *)(q + 0x10), (long *)(q + 0x18),
                              (long *)(q + 0x20), (long *)&sp->unk_D0, (long *)&sp->unk_D4) -
                  d - 6;
    if ((u32)sp->unk_100 < 0x1E0)
    {
        if (((u16)(*(u16 *)(q + 0x8) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0xA) + 0x20) < 0x121) |
            ((u16)(*(u16 *)(q + 0x10) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x12) + 0x20) < 0x121) |
            ((u16)(*(u16 *)(q + 0x18) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x1A) + 0x20) < 0x121) |
            ((u16)(*(u16 *)(q + 0x20) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x22) + 0x20) < 0x121))
        {
            s->unk_14 &= 0x7FFF;
            f = e[1];
            s->unk_0C[3] = f;
            if ((sp->unk_28 & 0x8) != 0)
            {
                if ((sp->unk_28 & 0x4) != 0)
                {
                    s->unk_0C[3] = f | 0x2;
                }
                else
                {
                    s->unk_0C[3] = f & 0xFD;
                }
            }
            *(s32 *)(q + 0x4) = *(s32 *)(p + 0x0);
            *(s32 *)(q + 0xC) = *(s32 *)(p + 0x4);
            *(s32 *)(q + 0x14) = *(s32 *)(p + 0x8);
            *(s32 *)(q + 0x1C) = *(s32 *)(p + 0xC);
            setPolyG4(q);
            setSemiTrans(q, 1);
            addPrim(sp->unk_24 + sp->unk_100 * 4, q);
            q = *(u8 **)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)q + 0xC;
            func_80067F20((u32 *)q, 0, 1, GetTPage(0, 1, 0, 0), 0);
            addPrim(sp->unk_24 + sp->unk_100 * 4, q);
        }
    }
    PopMatrix();
}

/* Walk the tag chain from `p` and hand every node to func_s46_80024198 with a
   zero fourth argument.  21 words, 0 differing, first compile.

   The twin is func_b06_80173E08 two listings up in THIS unit -- the same
   walk with a different per-node call -- and csrc/ovl_dungeon_800D50FC.c's
   func_800D67B0 is the resident original, identical down to the
   `s = (UnkSlot *)n->unk_0C;` cast.  Nothing was fought for; the loop is a
   `for (;;)` with the NULL test in the middle, which is what puts the
   `addiu $s0, $a2, 0x20` in the `beqz`'s delay slot. */
s32 func_s46_80024670(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_s46_80024198(p, v, s, 0);
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

/* Spend one frame of the payload's +0x5A life counter, raise the bank's
   D_80025B60 busy flag, fade all four of the record's 3-byte colours at
   +0x0..+0xE from the originals at +0x10..+0x1E in proportion to the counter
   over its +0x5C limit, and kill the node when the counter runs out.
   61 words, 0 differing, SECOND compile.

   The twin is the LANDED func_s67_80024834 in csrc/ovl_dng_s67.c (57 words,
   dngtwin 0.898), equal with func_s59_80024B98.  Both are leaves with NO
   frame at all, so there is no save list to compare and the allocation facts
   carry on shape alone.  The deltas: the counter pair is +0x5A/+0x5C not
   +0x3A/+0x3C, the flag is the bank-local D_80025B60 not D_800261B0, and the
   fade runs from the originals at +0x10..+0x12 into +0x0..+0x2 at stride 4
   rather than +0x1C -> +0xC.  The whole +4 words is the s16 index.

   Of the twin's two priced facts:
     - HOLDS: the counter decrement is SPLIT AROUND the flag store -- `t =
       load - 1; D_80025B60 = 1; store t;'.  As one statement after the flag
       store it is 61 words / 7 differing (not the twin's one word short,
       because here the extra scratch is already bought by the index).
     - PARTLY INVERTED: its "indexed, not a walking pointer" holds in that
       the base is recomputed from `p' every iteration -- but the subscript
       may NOT be written `p[i * 4 + K]'.  That is 61 words / 1 differing, the
       single row being `addu $a0, $a0, $a1' where the target has `addu $a0,
       $a1, $a0': index-first instead of base-first.  `(p + i * 4)[K]' does
       NOT fix it (also 61 / 1, same row, because fold reassociates it back).
       A loop-local `c = p + i * 4;' with `c[0x0]'/`c[0x10]' is what gives the
       base-first `addu', and is 61 / 0.  This is the lever at
       matching-levers.md "A TWO-STEP INDEX WRITTEN `(p + i)[K]' AND
       `p[i + K]'" with a measured correction: the parenthesised form is NOT
       equivalent to the pointer local here.
   This copy's own measured fact: the index is `s16'.  As `s32' the two
   sign-extension pairs vanish and it is 57 words / 57 differing. */
void func_s46_800246C4(u8 *p)
{
    s16 i;
    u16 t;
    u8 *c;

    t = *(u16 *)(p + 0x5A) - 1;
    D_80025B60 = 1;
    *(u16 *)(p + 0x5A) = t;
    for (i = 0; i < 4; i++)
    {
        c = p + i * 4;
        c[0x0] = c[0x10] * *(s16 *)(p + 0x5A) / *(s16 *)(p + 0x5C);
        c[0x1] = c[0x11] * *(s16 *)(p + 0x5A) / *(s16 *)(p + 0x5C);
        c[0x2] = c[0x12] * *(s16 *)(p + 0x5A) / *(s16 *)(p + 0x5C);
    }
    if (*(s16 *)(p + 0x5A) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The bank's 0x212 firework spawner: seed the payload's +0x5A/+0x5C life
   pair to 0xA, pick a 3-bit mask with func_s46_8002406C(8) and build a
   0x303030 colour with 0x90 in each channel the mask selects, publish it
   through +0x10..+0x1C into the payload's own +0x0..+0xC, install
   func_s46_800246C4 as the step and func_s46_80024670 as the renderer, then
   turn two random 12-bit angles into a velocity: the caller's +0x56 speed
   scaled by cos/sin of the first angle gives +0x24 and an s16 intermediate,
   and that intermediate scaled by cos/sin of the second gives +0x20/+0x22.
   +0x26/+0x28/+0x2A are those three each jittered by
   func_s46_8002406C(0x10) - 8, +0x2C..+0x36 are cleared, the mover is
   seeded from the caller's vector and the renderable gets a 0x1000 scale
   and a 0x80/0x80/0x80 colour.  145 words, 0 differing, FIRST COMPILE.

   NO TWIN BY EITHER TOOL -- dngtwin finds no carved sibling at 0.70 and
   `xsibs --min-words 18 --ratio 0.70 --top 200' finds no landed body at any
   ratio.  The route in was the CALLEE SET: func_8003FC64 + func_8004491C +
   func_80064584/func_800644B8, which lands on the LANDED func_b18_8017406C
   (csrc/ovl_dng_b18.c, 77 words) for the spawner skeleton and on
   csrc/unk_113D0.c's `(p->unk_00 * func_80064584(p->unk_38)) >> 12' for the
   angle idiom.  func_s46_80024AEC and func_s46_80024D34, landed above in
   this unit, supplied the rest -- this body needed NO new declaration
   beyond func_80064584/func_800644B8 and the three s46 self-references.

   Four facts measured after the fact, since nothing here was fought for:
     - HOLDS, 1 word and 141 differing: `r' IS ASSIGNED TWICE (the twin's
       fact).  A single `r' held across the +0x24/+0x20/+0x22 stores is 146
       words / 141 differing.
     - HOLDS, 133 differing: the angle intermediate is an `s16'.  As `s32'
       the `sll 16'/`sra 16' pair before the second `mult' disappears and
       every later allocation moves: 145 words / 133 differing.
     - HOLDS, 1 word and 51 differing: the jitter is `load + call() - 8',
       with the constant OUTSIDE.  As `load - 8 + call()' fold's `associate:'
       moves the -8 onto the call result and it is 146 words / 51 differing
       -- the same trap priced in func_s46_80024D34 below.
     - HOLDS, 6 differing: the six cleared halfwords go DESCENDING
       +0x36 -> +0x2C.  Ascending is 145 words / 6 differing.
   MEASURED AND FREE, so NOT the reason this lands, and a CORRECTION to
   func_b18_8017406C's stated fact: the payload's +0x0 store spelled off `n'
   as `*(s32 *)((u8 *)n + 0x20)' rather than off `p' as `*(s32 *)(p + 0x0)'.
   Both are 145 words / 0 differing -- combine folds `(plus (plus n 0x20) 0)'
   either way and emits `sw $v1, 0x20($s3)' off the node base regardless, so
   the listing's choice of base register is NOT evidence about the source
   here.  The twin's fact may still hold on the twin's own shape; it does
   not transfer. */
void func_s46_800247B8(u8 *a, UnkVec *b, u8 *c, s16 d)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;
    s32 k;
    s32 x;
    s32 y;
    s16 h;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x5A) = 0xA;
        *(s16 *)(p + 0x5C) = 0xA;
        k = func_s46_8002406C(8);
        *(s32 *)(p + 0x18) = 0x303030;
        if ((k & 0x1) != 0)
        {
            p[0x18] = 0x90;
        }
        if ((k & 0x2) != 0)
        {
            p[0x19] = 0x90;
        }
        if ((k & 0x4) != 0)
        {
            p[0x1A] = 0x90;
        }
        *(s32 *)(p + 0x10) = 0;
        *(s32 *)(p + 0x14) = 0;
        *(s32 *)(p + 0x1C) = *(s32 *)(p + 0x18);
        *(s32 *)((u8 *)n + 0x20) = *(s32 *)(p + 0x10);
        *(s32 *)(p + 0x4) = *(s32 *)(p + 0x14);
        *(s32 *)(p + 0x8) = *(s32 *)(p + 0x18);
        *(s32 *)(p + 0xC) = *(s32 *)(p + 0x1C);
        n->unk_10 = (u32)func_s46_800246C4;
        func_8004491C(n, (u32)func_s46_80024670);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_14 |= 0xC;
        x = rand() & 0xFFF;
        h = (*(s16 *)(a + 0x56) * func_80064584(x)) >> 12;
        *(s16 *)(p + 0x24) = (*(s16 *)(a + 0x56) * func_800644B8(x)) >> 12;
        y = rand() & 0xFFF;
        *(s16 *)(p + 0x20) = (h * func_80064584(y)) >> 12;
        *(s16 *)(p + 0x22) = (h * func_800644B8(y)) >> 12;
        *(s16 *)(p + 0x26) = *(u16 *)(p + 0x20) + func_s46_8002406C(0x10) - 8;
        *(s16 *)(p + 0x28) = *(u16 *)(p + 0x22) + func_s46_8002406C(0x10) - 8;
        *(s16 *)(p + 0x2A) = *(u16 *)(p + 0x24) + func_s46_8002406C(0x10) - 8;
        *(s16 *)(p + 0x36) = 0;
        *(s16 *)(p + 0x34) = 0;
        *(s16 *)(p + 0x32) = 0;
        *(s16 *)(p + 0x30) = 0;
        *(s16 *)(p + 0x2E) = 0;
        *(s16 *)(p + 0x2C) = 0;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)b + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)b + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)b + 0xA);
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s46", func_s46_800249FC);

/* This bank's spawner for the 0x12 decoration node: 0x3C at the payload's
   +0x5A and +0x5C, the caller's +0x88 word copied across, func_s46_800249FC
   as the node's step, the renderable cleared to 0x40/0xC with a 6-ish 0xA at
   +0x06, the mover seeded from the caller's vector, and then -- through a
   RE-READ of n->unk_0C -- a 0x50/0x50/0x50 over r+0xC..0xE, two zero
   halfwords at +0x1C/+0x1E and the 12-byte D_80025B48 record copied to the
   node's own +0x96 for the slot's +0x08 to point at.
   72 words, 0 differing, FIRST COMPILE.

   The twin is the LANDED func_b04_80174800 in csrc/ovl_dng_b04.c (82 words,
   dngtwin 0.818), and through it func_b20_80173CFC / func_b08_801741D0.
   Same -0x20 frame, same three saves $s0/$s1/$s2 in the same order.  THE 10
   MISSING WORDS ARE MISSING STATEMENTS, not missing references: the twin's
   three colour bytes over p[0..2], its `r->unk_1A = rand() & 0xFFF' spin and
   its `d'-sourced +0x1C/+0x1E are all absent here -- the two halfwords are
   plain zeroes and `c' and `d' are unused in this copy.  The seed differs too
   (+0x5A/+0x5C/0x3C and a +0x88 word copy in place of its +0x1A/0x41), and
   the record lands at n + 0x96 rather than its n + 0x48.

   Of the twin's three priced facts:
     - HOLDS: the record type has ALIGNMENT 1.  `u8 b[0xC]' gives the
       `lwl'/`lwr' + `swl'/`swr' pairs; as `u32 b[3]' the copy collapses to
       three aligned `lw'/`sw' pairs, 66 words / 22 differing.
     - NOT APPLICABLE: its p[0]-first colour statement order.  This copy has
       no colour bytes at all.  The analogous order fact here is the three
       0x50 bytes, which must be written DESCENDING 0xE, 0xD, 0xC -- ascending
       is 72 words / 2 differing.
     - FREE here as there: the mover reads spelled `*(s16 *)' rather than
       `*(u16 *)', still 72 / 0.
   Also measured and FREE, so not load-bearing: `r->unk_06 = 0xA' after
   `r->unk_14 |= 0xC' instead of before, 72 / 0. */
void func_s46_80024AEC(u8 *a, UnkVec *b, u8 *c, s16 d)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x5A) = 0x3C;
        *(s16 *)(p + 0x5C) = 0x3C;
        *(s32 *)(p + 0x88) = *(s32 *)(a + 0x88);
        n->unk_10 = (u32)func_s46_800249FC;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x40;
        r->unk_06 = 0xA;
        r->unk_14 |= 0xC;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)b + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)b + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)b + 0xA);
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0xE) = 0x50;
        *((u8 *)r + 0xD) = 0x50;
        *((u8 *)r + 0xC) = 0x50;
        r->unk_1E = 0;
        r->unk_1C = 0;
        *(UnkDngS46Name12 *)((u8 *)n + 0x96) = *(UnkDngS46Name12 *)D_80025B48;
        r->unk_08 = (u32)((u8 *)n + 0x96);
    }
}

/* The +0x10 step func_s46_80024D34 installs: raise D_80025B60, re-send the
   slot through func_800478B8 on every fourth frame of the +0x5A counter,
   grow both scale halves by 0x32 up to a 0x2000 ceiling, spend one frame of
   the counter and fade the slot's 3-byte colour as `counter * 0x80 / limit',
   then kill the node both when the counter runs out and when the
   renderable's own 0x8000 bit comes up.
   74 words, 0 differing.  Two compiles.

   NO TWIN BY EITHER TOOL -- dngtwin finds no carved sibling at 0.70 and
   `xsibs --min-words 18 --ratio 0.70' finds no landed body at any ratio.
   The shape came from the CALLEE/OFFSET set: func_800478B8 plus the
   `+0x5A/-0x2/D_800814A0' teardown that func_s46_800246C4 above already
   carries, which puts it in the same family as the LANDED func_s67_800246F0
   (csrc/ovl_dng_s67.c, 52 words) -- the same D_8002xxxx flag raise,
   func_800478B8 hand-off, `counter * 0x80 / limit' fade and kill tail at
   +0x3A/+0x3C instead of +0x5A/+0x5C.  Added here: the fourth-frame gate on
   the call, the two 0x32 scale ramps with their 0x2000 ceilings, and the
   SECOND kill on the renderable's own 0x8000 flag.

   Of func_s67_800246F0's facts:
     - NOT APPLICABLE: its decrement split around the flag store.  Here the
       flag raise is at the head and the decrement is 40 words later;
       `*(u16 *)(p + 0x5A) -= 1;' as one statement is correct.
     - HOLDS: the three colour bytes are ONE chained `[0] = [1] = [2]'.
     - INVERTED in form, not in mechanism: it says the fade reads the counter
       back out of memory with `lh' because the intervening call clobbers the
       register.  There is no call between here, so the read-back is what
       produces the `sll 16'/`sra 9' -- cse substitutes the just-stored
       register and the `<< 7' still needs the sign extension.  Same source,
       different reason.

   THE ONE MEASURED FACT, worth 1 word: the gate's halfword must go through
   a LOCAL -- `t = *(u16 *)(p + 0x5A); D_80025B60 = 1; if ((t & 0x3) == 0)'.
   Read inline in the `if', the flag store schedules BEFORE the `lhu', reorg
   can no longer move it into the `bnez' delay slot (it will not carry a
   store forward past a load it cannot disambiguate), and the load delay
   takes a `nop' instead of the `addiu $a0, $zero, 0x1': 75 words / 72
   differing.  Moving the store after the gate entirely is 74 words / 18
   differing, so it is the position BETWEEN the load and the test that the
   target has. */
void func_s46_80024C0C(u8 *p, UnkVec *b, UnkSlot *r)
{
    u16 t;

    t = *(u16 *)(p + 0x5A);
    D_80025B60 = 1;
    if ((t & 0x3) == 0)
    {
        func_800478B8((u8 *)r);
    }
    r->unk_1C += 0x32;
    if (r->unk_1C > 0x2000)
    {
        r->unk_1C = 0x2000;
    }
    r->unk_1E += 0x32;
    if (r->unk_1E > 0x2000)
    {
        r->unk_1E = 0x2000;
    }
    *(u16 *)(p + 0x5A) -= 1;
    r->unk_0C[0] = r->unk_0C[1] = r->unk_0C[2] = (*(s16 *)(p + 0x5A) << 7) / *(s16 *)(p + 0x5C);
    if (*(s16 *)(p + 0x5A) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    if ((r->unk_14 & 0x8000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* This bank's spawner for the 0x212 decoration node: 0xE at the payload's
   +0x5A and +0x5C, func_s46_80024C0C as the node's step, the renderable set
   to 0x60 with 0xC and then 0x2 raised in its flags, the mover seeded from
   the caller's vector with each channel jittered by -0x10 + (rand() & 0x1F),
   and then -- through a RE-READ of n->unk_0C -- 0xC00 as both scale halves,
   a 0x80/0x80/0x80 over r+0xC..0xE, 0x7DCF at +0x12, 0x100 raised in the
   flags again and the record registered with func_8003DB94 against
   D_800DE870.
   74 words, 0 differing.  Six compiles, and both faults are recorded below.

   The twin is the LANDED func_b18_8017406C in csrc/ovl_dng_b18.c (77 words,
   dngtwin 0.715, itself written off the resident func_800B2230) -- the same
   func_8003FC64(0x212) / +0x10 install / func_8004491C(n, D_80045340) /
   func_8003DB94(r, <tbl>, ...) skeleton.  Frames differ (-0x20 with three
   saves here against its -0x18 with two), so only its SHAPE facts were
   taken:
     - HOLDS: `r' IS ASSIGNED TWICE.  The target reloads n->unk_0C after the
       stores through `m', and the second block's `lw $a0, 0xC($s1)' is that
       reload.
     - NOT APPLICABLE: its "+0x00 store is spelled off `n', not off `v'".
       This copy makes no payload byte stores at all.
   The twin's own jitter is `rand() & 1' as an ARGUMENT, so it says nothing
   about the three jittered channels here, and both of this body's faults are
   its own:

   FAULT 1, worth 2 words and 48 differing: the jitter must be written
   `load + ((rand() & 0x1F) - 0x10)', NOT `load - 0x10 + (rand() & 0x1F)'.
   fold's `associate:' SWAPS which operand carries the additive constant, so
   the natural reading emits `addiu $v0, $v0, -0x10' on the rand value where
   the target has `addiu $v1, $v1, -0x10' on the loaded halfword.  Written
   the natural way the whole add is also narrowed to HImode, the constant
   becomes `ori $s0, $zero, 0xFFF0' in a FOURTH callee-saved register and the
   frame grows to -0x28 with $s0..$s3: 74 words / 68 differing.  Hoisting the
   sum into an `s32' local fixes the mode but not the operand (72 / 48), and
   so does `*(u16 *)' on the store (74 / 65) or `*(s16 *)' on the load
   (72 / 48).  Only the parenthesisation gets both.

   FAULT 2, worth 2 words and the whole remaining 48: `r->unk_10 = 0x60;'
   must sit BETWEEN the two flag raises, not before them.  With the two
   `|= 0xC' and `|= 0x2' adjacent, flow.c's `last_mem_set' dead-store
   elimination deletes the FIRST `sh $v0, 0x14($a0)' -- nothing between them
   touches memory -- and combine then folds the pair into one
   `ori $v0, $v0, 0xE'.  The 0x10 store is a memory reference, which resets
   `last_mem_set' and makes both 0x14 stores survive; sched1 then hoists it
   back above both, which is why the listing reads 0x10, 0xC, 0x14, 0x2,
   0x14 and hides the cause.  Spelling the second raise as the plain cast
   `*(u16 *)((u8 *)r + 0x14) |= 0x2' does NOT help (74 / 68 with fault 1
   present, and the `ori 0xE' still there): it is not an aliasing question.
   The two raises as a single `|= 0xE' would be the same one store, so this
   is a real pair of statements with a real statement between them. */
void func_s46_80024D34(u8 *a, UnkVec *b, u8 *c, s16 d)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x5A) = 0xE;
        *(s16 *)(p + 0x5C) = 0xE;
        n->unk_10 = (u32)func_s46_80024C0C;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_14 |= 0xC;
        r->unk_10 = 0x60;
        r->unk_14 |= 0x2;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)b + 0x2) + ((rand() & 0x1F) - 0x10);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)b + 0x6) + ((rand() & 0x1F) - 0x10);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)b + 0xA) + ((rand() & 0x1F) - 0x10);
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0xC00;
        r->unk_1C = 0xC00;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_12 = 0x7DCF;
        r->unk_14 |= 0x100;
        func_8003DB94(r, D_800DE870, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s46", func_s46_80024E5C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s46", func_s46_800255D0);
