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
extern const u8 D_s46_80025B38[];
extern u8 D_800DDC40[];
s32 func_800A4778(u16, u16, s16, s32);
s32 func_800A56E0(s32);
void func_80047784(u8 *, s32, s32);
void func_s46_800247B8_3(u8 *, UnkVec *, u8 *) __asm__("func_s46_800247B8");
void func_s46_80024AEC_3(u8 *, UnkVec *, u8 *) __asm__("func_s46_80024AEC");
void func_s46_80024D34_3(u8 *, UnkVec *, u8 *) __asm__("func_s46_80024D34");
void func_800D4AB0_3(u8 *, UnkVec *, u8 *) __asm__("func_800D4AB0");
extern u8 D_s46_80024004[];
extern u8 D_s46_80024024[];
extern u8 D_80025B40[];
extern u8 D_80083498[];
extern u8 D_800DEC00[];
extern s16 D_80083228;
extern s32 D_8008346C;
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
UnkNode *func_8003FD64(s32, u8 *);
s16 func_800A48F0(s8 *, s8, s8);
s32 func_800A6D30(void);
void *func_800990FC(void);
void *func_80099194(u8 *, u8 *);
u8 *func_80099290(u8 *);
void *func_80099734(u8 *, u8 *);
s32 func_800A5720(void *);
typedef struct UnkDngS58Blk32
{
    u8 b[0x20];
} UnkDngS58Blk32;
typedef struct UnkDngS58Rec
{
    s16 a;
    u16 b;
} UnkDngS58Rec;
typedef struct UnkDngS46Blk48
{
    s32 w[0xC];
} UnkDngS46Blk48;

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

/* The +0x12 decoration node's step (func_s46_80024AEC below installs it):
   spin the slot's z rotation by 0x2BC a frame, then -- on the payload's
   +0x5A counter -- grow BOTH scale halves by 0xC8 while the counter is above
   0x28 and shrink them by 0xBE once it is below 0x15, leaving the 0x15..0x28
   band alone; then a 1-in-3 draw sets the slot's 0x80 flag only when the draw
   comes up zero AND the flag is currently clear, and clears it otherwise;
   then the usual spend-one-frame / kill-at-zero teardown.
   60 words, 0 differing.  Two compiles.

   NO TWIN by shape.  dngtwin finds no carved sibling at 0.70; dngcallee's
   2-of-2 symbol overlap (D_80025B60, D_800814A0) is func_s46_80024C0C and
   func_s46_800246C4, both LANDED in this same file.  The head (the
   D_80025B60 raise sitting BETWEEN a +0x5A load and its test) and the whole
   teardown are func_s46_80024C0C's verbatim; only the middle is new.

   TWO MEASURED FACTS.

   1. Worth 2 words and 52 aligned rows: the counter local is `s32', not
      `s16'.  `s16 t = *(s16 *)(p + 0x5A);' loads with `lhu' and pays
      `sll 16'/`sra 16' before the first `slti' -- 62 words / 52 differing --
      and because the extension lands in $v1 the two `slti's and everything
      after them shift two words.  As `s32' the load is the target's single
      `lh $a0' and the extension is free.  THE TELL for this one is a
      `sll'/`sra' pair against the target's bare `lh'.
   2. Worth 2 words and 11 aligned rows: the scale chain is
      `r->unk_1C = r->unk_1E = <expr>', so the right-to-left evaluation
      stores +0x1E first as the target does.  Written the other way round
      (`r->unk_1E = r->unk_1C = <expr>') it is 62 words / 11 differing.

   MEASURED AND FREE, so NOT the reason this lands -- do not spend budget
   preserving it: the flag gate written as a value `?:' instead of two
   `if'/`else' stores,
   `r->unk_14 = ((f & 0x80) != 0 || k != 0) ? (f & 0xFF7F) : (f | 0x80);'
   is also 60 words / 0 aligned rows.  That is a THIRD case for the project's
   "`?:' is not neutral" rule: both arms here are a binary op on the SAME
   shared operand `f' with different constants, the store is single either
   way, and jump2 cross-joins what the `if' form duplicates.  Kept as
   `if'/`else' only because that is what the general rule prescribes for two
   independent sets. */
void func_s46_800249FC(u8 *p, UnkVec *b, UnkSlot *r)
{
    s32 t;
    s16 k;
    u16 f;

    r->unk_1A += 0x2BC;
    t = *(s16 *)(p + 0x5A);
    D_80025B60 = 1;
    if (t >= 0x29)
    {
        r->unk_1C = r->unk_1E = r->unk_1E + 0xC8;
    }
    else if (t < 0x15)
    {
        r->unk_1C = r->unk_1E = r->unk_1E - 0xBE;
    }
    k = func_s46_8002406C(3);
    f = r->unk_14;
    if ((f & 0x80) != 0 || k != 0)
    {
        r->unk_14 = f & 0xFF7F;
    }
    else
    {
        r->unk_14 = f | 0x80;
    }
    *(u16 *)(p + 0x5A) -= 1;
    if (*(s16 *)(p + 0x5A) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

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

/* The chase actor's four-state step, installed by func_s46_800255D0 below
   (`n2->unk_10 = (u32)func_s46_80024E5C').  Raise D_80025B60; while the state
   at +0x58 is under 3, repaint the slot 0x3C/0x3C/0x3C and then force one of
   the three channels to 0x80 from the low three bits of a +0x72 counter that
   advances by 1 and skips every multiple of 8.  State 0 counts seven frames
   at +0x5A and then re-dresses the slot through func_80047784 and drops the
   mover 0x5A.  State 1 is the approach: bail to the kill tail if
   func_800A4778 reports a hit; with +0x60 set, pick a flight time from the
   Manhattan distance to the target's mover, split the remaining delta into a
   per-frame step over (t - 0xB / -0x9 / -0x4 / t) frames depending on how
   many frames are left, advance the mover by it, and on the last frame go to
   state 2 with +0x62 = 6 and +0x56 = 0x1E; with +0x60 clear, drift by the
   +0x98/+0x9C/+0xA0 velocity for 0xA frames and then kill.  State 2 runs the
   impact: one func_s46_80024AEC burst on frame 2, the slot's scale decayed by
   0xC8 a frame to a floor of 0, extra func_s46_800247B8 puffs at 0x1E/0x28/
   0x2D, and state 3 at 0x32.  State 3 fades out, calls func_800D4AB0 on frame
   1 and kills at 0x1B.  Every non-kill exit re-sends the slot through
   func_800478B8.
   477 words, 0 differing, 0 aligned rows.  Six compiles.

   NO TWIN.  dngtwin finds no carved sibling at 0.70 and dngcallee's best is
   4-of-9 across four unrelated bodies.  The shape was read off the listing;
   the only borrowed facts are the `+0x5A/-0x2/D_800814A0' kill tail, which is
   func_s46_800246C4's, and the s16 local-narrowing tell from
   func_s46_800249FC above.

   THE FIGHT WAS FOUR THINGS, ALL MEASURED INDEPENDENTLY FROM THE ZERO
   BASELINE.

   1. `extern const u8 D_s46_80025B38[];' -- the `const' IS LOAD-BEARING and
      is worth 8 aligned rows.  State 0 stores the table's address into
      `c + 0x2C' and then passes `D_s46_80025B38[0]' to func_80047784.  With
      the data declared non-const, sched1 cannot prove the store independent
      of the byte load, so it must keep the `lbu' after the `sw'; the shared
      `lui' then has to survive to the second use, the whole address lands in
      a callee-saved register and the delay slot takes the `addiu' instead of
      the `sw'.  Non-const, store before the call: 477 words / 8 rows.
      Non-const, store after the call: 477 / 5, with the base in $s0 instead
      of $v0.  Rewriting the store as an ARRAY REF (`((u32 *)c)[0xB] = ...')
      is 477 / 8 -- IDENTICAL to the plain cast, so MEM_IN_STRUCT_P is NOT
      the lever here; RTX_UNCHANGING_P is.
   2. `ax'/`ay' must be LOCALS loaded before the four-arm divisor ladder,
      not `*(s32 *)(m + 0x0)' inside each arm, and that is worth 2 words,
      35 aligned rows and 0x8 OF FRAME: inlined, the body is 479 words with a
      -0x30 frame instead of -0x28.  The target loads them in the block that
      ENDS with the ladder's first `bnez', which no per-arm spelling can
      reach because cse does not hoist across a label.
   3. The two deltas in the flight-time formula must be INLINE in one
      expression, not `dx'/`dz' locals: as locals gcc has both pairs of `lh's
      available at once, fills the load delays with each other and the body
      is 472 words / 61 rows -- FIVE WORDS SHORT.  The target's two `nop's
      after each `lh' pair are the evidence that nothing else was ready.
   4. `u32 st = *(u16 *)(p + 0x58);' before `D_80025B60 = 1;' -- the same
      load-then-store-then-test shape func_s46_80024C0C prices, and worth
      1 word and 16 rows here.  Read inline in the gate it is 478 / 16: the
      flag store schedules ahead of the `lhu' and the third parameter's
      `addu $s3, $a2, $zero' is pushed out of the prologue.  `u32' and not
      `s32', because the dispatch is `sltiu'.

   The three kill sites are written out in full, three times, each with its
   own `return' -- jump2 cross-joins them into the one block at
   .Ls46_80025578 by itself, as the project's "four plain `return -1's" note
   predicts.  No `goto' and no shared exit label.

   The loop counters are `s16 i', not `s32': every one of the five
   `for' loops emits `sll 16'/`sra 16' on the bumped counter before its
   `slti'.  This is the OPPOSITE of fact 1 in func_s46_800249FC above -- the
   narrowing is wanted here and not there -- so read the listing, do not
   apply either as a rule.

   ARITY ALIASES: func_s46_800247B8, func_s46_80024AEC, func_s46_80024D34 and
   func_800D4AB0 are all called with THREE arguments here and the listing sets
   only $a0/$a1/$a2, while their landed definitions take four (or, for
   func_800D4AB0, one).  The `_3' aliases in the declaration block keep those
   definitions untouched; K&R definitions are not prototypes, so the original
   could legally call them short. */
void func_s46_80024E5C(u8 *p, UnkVec *v, u8 *c)
{
    u8 *m;
    s32 ax;
    s32 ay;
    s32 az;
    u32 st;
    s16 t;
    s16 q;
    s16 i;

    st = *(u16 *)(p + 0x58);
    D_80025B60 = 1;
    if (st < 3)
    {
        c[0xE] = 0x3C;
        c[0xD] = 0x3C;
        c[0xC] = 0x3C;
        *(s16 *)(p + 0x72) = *(u16 *)(p + 0x72) + 1;
        if ((*(u16 *)(p + 0x72) & 0x7) == 0)
        {
            *(s16 *)(p + 0x72) = *(u16 *)(p + 0x72) + 1;
        }
        if ((*(u16 *)(p + 0x72) & 0x1) != 0)
        {
            c[0xC] = 0x80;
        }
        if ((*(u16 *)(p + 0x72) & 0x2) != 0)
        {
            c[0xD] = 0x80;
        }
        if ((*(u16 *)(p + 0x72) & 0x4) != 0)
        {
            c[0xE] = 0x80;
        }
    }
    if (*(s16 *)(p + 0x58) == 0)
    {
        t = *(u16 *)(p + 0x5A);
        *(s16 *)(p + 0x5A) = t + 1;
        if (t >= 6)
        {
            *(u32 *)(c + 0x2C) = (u32)D_s46_80025B38;
            func_80047784(c, D_s46_80025B38[0], 0);
            *(s16 *)(p + 0x5A) = 0;
            *(s16 *)(p + 0x58) = *(u16 *)(p + 0x58) + 1;
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)v + 0xA) - 0x5A;
        }
    }
    if (*(s16 *)(p + 0x58) == 1)
    {
        if ((s16)func_800A4778(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                               *(s16 *)((u8 *)v + 0xA), *(s32 *)(p + 0x88)) != 0)
        {
            *(s16 *)(*(u8 **)(p + 0x8C) + 0x86) = 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        if (*(s16 *)(p + 0x60) != 0)
        {
            m = *(u8 **)(*(u8 **)(p + 0x88) - 0x18);
            *(s16 *)(c + 0x6) = 8;
            if (*(s16 *)(p + 0x5A) == 0)
            {
                q = __builtin_abs(*(s16 *)(m + 0x2) - *(s16 *)((u8 *)v + 0x2)) / 0x40 +
                    __builtin_abs(*(s16 *)(m + 0x6) - *(s16 *)((u8 *)v + 0x6)) / 0x40;
                *(s16 *)(p + 0x5A) = q * 2 + 0xB;
            }
            az = *(s32 *)(m + 0x8) - ((D_800DDC40[(*(u8 **)(p + 0x88))[0x13]] >> 1) << 16);
            ax = *(s32 *)(m + 0x0);
            ay = *(s32 *)(m + 0x4);
            if (*(s16 *)(p + 0x5A) >= 0xE)
            {
                v->unk_0C = (ax - v->unk_00) / (*(s16 *)(p + 0x5A) - 0xB);
                v->unk_10 = (ay - v->unk_04) / (*(s16 *)(p + 0x5A) - 0xB);
                v->unk_14 = (az - v->unk_08) / (*(s16 *)(p + 0x5A) - 0xB);
            }
            else if (*(s16 *)(p + 0x5A) >= 0xB)
            {
                v->unk_0C = (ax - v->unk_00) / (*(s16 *)(p + 0x5A) - 0x9);
                v->unk_10 = (ay - v->unk_04) / (*(s16 *)(p + 0x5A) - 0x9);
                v->unk_14 = (az - v->unk_08) / (*(s16 *)(p + 0x5A) - 0x9);
            }
            else if (*(s16 *)(p + 0x5A) >= 0x5)
            {
                v->unk_0C = (ax - v->unk_00) / (*(s16 *)(p + 0x5A) - 0x4);
                v->unk_10 = (ay - v->unk_04) / (*(s16 *)(p + 0x5A) - 0x4);
                v->unk_14 = (az - v->unk_08) / (*(s16 *)(p + 0x5A) - 0x4);
            }
            else
            {
                v->unk_0C = (ax - v->unk_00) / *(s16 *)(p + 0x5A);
                v->unk_10 = (ay - v->unk_04) / *(s16 *)(p + 0x5A);
                v->unk_14 = (az - v->unk_08) / *(s16 *)(p + 0x5A);
            }
            v->unk_00 += v->unk_0C;
            v->unk_04 += v->unk_10;
            v->unk_08 += v->unk_14;
            *(s16 *)(p + 0x5A) = *(u16 *)(p + 0x5A) - 1;
            if (*(s16 *)(p + 0x5A) == 0)
            {
                *(s16 *)(p + 0x58) = 2;
                *(s16 *)(p + 0x62) = 6;
                *(s16 *)(p + 0x5A) = 0;
                v->unk_14 = 0;
                v->unk_10 = 0;
                v->unk_0C = 0;
                *(s16 *)(p + 0x56) = 0x1E;
            }
        }
        else
        {
            *(s16 *)(p + 0x5A) = 0xA;
            *(s16 *)(p + 0x5C) = *(u16 *)(p + 0x5C) - 1;
            v->unk_00 += *(s32 *)(p + 0x98);
            v->unk_04 += *(s32 *)(p + 0x9C);
            v->unk_08 += *(s32 *)(p + 0xA0);
            if (*(s16 *)(p + 0x5C) <= 0)
            {
                *(s16 *)(*(u8 **)(p + 0x8C) + 0x86) = 1;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
                return;
            }
        }
        if (*(s16 *)(p + 0x5A) >= 0x5)
        {
            for (i = 0; i < 4; i++)
            {
                func_s46_80024D34_3(p, v, c);
            }
        }
    }
    if (*(s16 *)(p + 0x58) == 2)
    {
        *(s16 *)(p + 0x5A) = *(u16 *)(p + 0x5A) + 1;
        if (*(s16 *)(p + 0x5A) == 2)
        {
            func_s46_80024AEC_3(p, v, c);
            func_800A56E0(0x300);
        }
        if (*(s16 *)(p + 0x5A) >= 0xA)
        {
            if (*(u16 *)(c + 0x1C) != 0)
            {
                q = *(u16 *)(c + 0x1C) - 0xC8;
                if (q < 0)
                {
                    q = 0;
                }
                *(s16 *)(c + 0x1C) = q;
                *(s16 *)(c + 0x1E) = q;
            }
        }
        if (*(s16 *)(p + 0x5A) >= 0x1E)
        {
            *(s16 *)(p + 0x56) = *(u16 *)(p + 0x56) + 3;
            func_s46_800247B8_3(p, v, c);
            if (*(s16 *)(p + 0x5A) >= 0x28)
            {
                func_s46_800247B8_3(p, v, c);
            }
            if (*(s16 *)(p + 0x5A) >= 0x2D)
            {
                func_s46_800247B8_3(p, v, c);
            }
        }
        if (*(s16 *)(p + 0x5A) >= 0x32)
        {
            *(s16 *)(p + 0x58) = 3;
            *(s16 *)(p + 0x5A) = 0;
            *(u16 *)(c + 0x14) |= 0x80;
        }
    }
    if (*(s16 *)(p + 0x58) == 3)
    {
        *(s16 *)(p + 0x5A) = *(u16 *)(p + 0x5A) + 1;
        if (*(s16 *)(p + 0x5A) < 0x12)
        {
            *(s16 *)(p + 0x56) = *(u16 *)(p + 0x56) - 3;
            for (i = 0; i < 5; i++)
            {
                func_s46_800247B8_3(p, v, c);
            }
        }
        if (*(s16 *)(p + 0x5A) == 0x10)
        {
            for (i = 0; i < 2; i++)
            {
                func_s46_800247B8_3(p, v, c);
            }
        }
        if (*(s16 *)(p + 0x5A) == 0x11)
        {
            for (i = 0; i < 3; i++)
            {
                func_s46_800247B8_3(p, v, c);
            }
        }
        if (*(s16 *)(p + 0x5A) == 0x12)
        {
            for (i = 0; i < 4; i++)
            {
                func_s46_800247B8_3(p, v, c);
            }
        }
        if (*(s16 *)(p + 0x5A) == 1)
        {
            func_800D4AB0_3(p, v, c);
        }
        if (*(s16 *)(p + 0x5A) >= 0x1B)
        {
            *(s16 *)(*(u8 **)(p + 0x8C) + 0x86) = 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
    }
    func_800478B8(c);
}

/* THE BANK'S MASTER SCRIPT -- D_s46_80024000 holds its address -- AND IT
   OWNS THE UNIT'S ONE JUMP TABLE AND READS THE 0x20-BYTE D_s46_80024004
   DIRECTION TABLE ABOVE IT.  jtbl_s46_80024058: 5 `.word's = 5 cases + 0 pad,
   dispatch `sltiu $v1, $a0, 0x5' with NO minval subtraction, so the case set
   is 0..4.  The entry list in table order, which is also entry-ADDRESS order
   and also source order:

     case 0  .Ls46_800256C0      case 3  .Ls46_800259E4
     case 1  .Ls46_80025714      case 4  .Ls46_80025AAC
     case 2  .Ls46_80025760

   THE DEFAULT IS THE EPILOGUE, so there is no `default:' arm: the range
   check's `beqz' target and the epilogue's first word are both
   .Ls46_80025B10.  Case 0 falls through into case 1.  VERIFIED BY COMPILING,
   not by word count -- a standalone `tools/cdkgcc -S' emits `.rdata' /
   `.align 3' / five `.word $Ln' in the same ascending arm order, with the
   range check branching to the epilogue label.
   346 words, 0 differing, 0 aligned rows.  Four compiles.

   What it does: case 0 dresses the owner's slot white at 0x1000 scale through
   func_8003DB94/D_800DEC00, raises D_80025B60 and latches the owner's facing
   (bits 9..11 of the heading at o + 0x2A) at p + 0x7E.  Case 1 tracks the
   owner's mover and waits for the 0x80 bit on *(u16 **)(p + 0x4).  Case 2
   spawns the 0x112 chase actor, seeds its +0x98/+0x9C step from the facing
   row of D_s46_80024004, copies the owner's whole slot into it, installs
   func_s46_80024E5C as its step, dresses it from the bank's D_80025B40 facing
   table and remembers it at p + 0xA8 / p + 0xAC with p + 0x88 = 0x63.  Case 3
   mirrors the actor's mover back onto the caller until the actor dies, then
   prints the bank's D_s46_80024024 string through the
   func_800990FC/func_80099734/func_80099194/func_80099290/func_800A5720
   chain.  Case 4 waits 0xB frames and retires, clearing D_8008346C unless
   D_80025B60 is still up.

   NO TWIN.  dngtwin's best is func_s32_80025374 at 0.748 and dngcallee's is
   func_b37_80172840 at 10-of-23, and NEITHER was used: both have a different
   prologue (-0x58 with ten saves against this -0x50 with seven).  The shape
   came from csrc/ovl_dng_s40.c's LANDED func_s40_80024F10 for the head and
   cases 0/1/4, and from csrc/ovl_dng_b26.c's LANDED func_b26_80173614 for the
   string chain and the `*(u8 **)(slot + 0x2C) = e;' +
   `func_80047784(slot, e[((D_80083228 + heading + 0x100) >> 9) & 7], 0)'
   dress pair, which it carries verbatim.

   FOUR MEASURED FACTS, each re-priced ALONE from the zero baseline.

   1. THE DECLARATION ORDER OF `o' AND `n2' IS LOAD-BEARING, WORTH 23 ALIGNED
      ROWS, and this is the one that is easy to miss because every row is a
      register name.  With `o' declared AFTER `n2' the body is 346 words /
      23 rows and the residue is a clean whole-body exchange of $s2 and $s3:
      `o' takes $s3 and `n2' takes $s2.  Both are global allocnos of
      comparable priority (`o' 14 refs over the whole body, `n2' 10 refs over
      two disjoint ranges), so `allocno_compare' falls through to its
      tie-break, which is the ALLOCNO NUMBER -- and that follows the pseudo
      number, which follows DECLARATION ORDER.  Declaring `o' first is the
      whole fix and costs nothing.
      NOTE FOR THE NEXT READER: csrc/ovl_dng_s40.c's header records eight
      permutations of ITS declaration order measuring identically, i.e. a
      NON-witness.  Both are true: the order only matters when two allocnos
      actually tie.
      The dead end this replaced: splitting case 3's node into its own local
      `n3' also puts `o' in $s2 and gets to 346 / 2 -- but then `n3' is no
      longer a global allocno, local-alloc gives it $v0, and the target has
      $s3.  Case 2's and case 3's node ARE the same variable.
   2. `n' MUST BE A LOCAL with two uses, worth 7 aligned rows.  Written as
      `m = (u8 *)((UnkNode *)(o - 0x20))->unk_08;' twice, combine folds the
      displacement into each load (`lw $s0, -0x18($s2)') and the target's
      shared `addiu $v0, $s2, -0x20' disappears: 346 / 7.  (That spelling
      ALSO fixes fact 1 by itself, by giving `o' a 15th reference -- which is
      how the two were disentangled.)
   3. The payload stores go +0x84, +0x88, +0x8C in that order, worth 1 word
      and 5 rows.  With +0x8C before +0x88 the `lw $v0, 0x60($s2)' has no
      store to fill its load delay and takes a `nop': 347 / 5.
   4. Case 4's counter restore must be an ARRAY REF,
      `((s16 *)p)[0x42] = ((u16 *)p)[0x42] - 1;', worth 1 word and 12 rows.
      As a plain cast (`*(s16 *)(p + 0x84) = *(u16 *)(p + 0x84) - 1;') sched1
      cannot prove the store independent of the `lh' of D_80025B60 two lines
      later, so the store cannot sink past it, the gate's `bnez' loses the
      `lui' that should fill its delay slot and the body is 347 / 12.  THIS
      one IS MEM_IN_STRUCT_P -- note that the same lever was measured FREE on
      func_s46_80024E5C above, where `const' on the data was the answer
      instead.  Two witnesses and one non-witness for the same flag in one
      unit.

   The 0x30-byte slot copy is a real struct assignment, not eleven stores:
   `addiu $t0, $a2, 0x30' plus a 16-byte unrolled `bne' loop is gcc's block
   move for a 48-byte 4-aligned type, which is what the new UnkDngS46Blk48
   struct in the declaration block is for.  The five slot stores that PRECEDE
   it (0x80 x3 at
   +0xC..+0xE, 0x1000 x2 at +0x1C/+0x1E) are overwritten by it and are dead in
   the original too; gcc keeps them because it cannot see through the block
   move.  Do not "fix" that.

   `D_s46_80024004' is declared `extern u8 []' and the copy goes through
   `*(UnkDngS58Blk32 *)' -- align 1, which is what gives the target's
   lwl/lwr/swl/swr pairs; splat emits the symbol as `.asciz " "' + `.align 2',
   so the 1-alignment is real and not a guess.  MEASURED AND FREE: `const' on
   that declaration is 346 words / 0 rows either way, unlike `const' on
   D_s46_80025B38 in func_s46_80024E5C above, which is worth 8 rows.  Left
   non-const because splat emits the object into `.section .data', not
   .rodata. */
void func_s46_800255D0(u8 *p, UnkVec *v, u8 *c)
{
    UnkDngS58Rec tbl[8];
    u8 *o;
    UnkNode *n;
    u8 *m;
    u8 *s;
    UnkNode *n2;
    UnkVec *w;
    u8 *q;
    u8 *e;
    u8 *r;
    u8 *f;
    void *h;
    void *hm;

    o = *(u8 **)p;
    *(UnkDngS58Blk32 *)tbl = *(UnkDngS58Blk32 *)D_s46_80024004;
    n = (UnkNode *)(o - 0x20);
    m = (u8 *)n->unk_08;
    s = (u8 *)n->unk_0C;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)(c + 0xC) = 0x808080;
        *(s16 *)(c + 0x1E) = 0x1000;
        *(s16 *)(c + 0x1C) = 0x1000;
        func_8003DB94(c, D_800DEC00, 0);
        D_80025B60 = 1;
        *(s16 *)(p + 0x7E) = (((u16 *)o)[0x15] >> 9) & 0x7;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fallthrough */
    case 1:
        *(s16 *)((u8 *)v + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)v + 0x6) = *(u16 *)(m + 0x6);
        *(s16 *)((u8 *)v + 0xA) = *(u16 *)(m + 0xA);
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        *(s16 *)(p + 0x84) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 2:
        n2 = func_8003FD64(0x112, D_80083498);
        if (n2 != NULL)
        {
            q = (u8 *)n2 + 0x20;
            *(s32 *)(q + 0x98) = tbl[*(s16 *)(p + 0x7E)].a << 16;
            *(s32 *)(q + 0x9C) = tbl[*(s16 *)(p + 0x7E)].b << 16;
            *(s16 *)(q + 0x58) = 0;
            *(s32 *)(q + 0x84) = (s32)o;
            *(s32 *)(q + 0x88) = *(s32 *)(o + 0x60);
            *(s32 *)(q + 0x8C) = (s32)p;
            q[0x6A] = p[0x9];
            *(s16 *)(q + 0x6C) = *(u16 *)(p + 0x7E);
            e = *(u8 **)(o - 0x14);
            p[0xA0] = e[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x7E)];
            p[0xA1] = e[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x7E)];
            if (*(s32 *)(o + 0x60) != 0)
            {
                *(s16 *)(q + 0x90) = *(s8 *)(o + 0x72);
                *(s16 *)(q + 0x92) = *(s8 *)(o + 0x73);
                *(s16 *)(q + 0x94) = e[0x24];
                *(s16 *)(q + 0x96) = e[0x25];
                *(s16 *)(q + 0x60) = 1;
                *(s16 *)(q + 0x5A) = 0;
                func_800A48F0((s8 *)*(u8 **)(o + 0x60), 0x1D,
                              (p[0x9] >> 2) + 3 + (func_800A6D30() & 0x3));
            }
            else
            {
                *(s16 *)(q + 0x5C) = 0x20;
                *(s16 *)(q + 0x5A) = 0;
                *(s16 *)(q + 0x60) = 0;
            }
            *(s16 *)(p + 0x86) = 0;
            r = (u8 *)n2->unk_0C;
            r[0xE] = 0x80;
            r[0xD] = 0x80;
            r[0xC] = 0x80;
            *(s16 *)(r + 0x1E) = 0x1000;
            *(s16 *)(r + 0x1C) = 0x1000;
            *(UnkDngS46Blk48 *)r = *(UnkDngS46Blk48 *)s;
            *(s16 *)(r + 0x10) = 0x20;
            *(u16 *)(r + 0x14) |= 0xC;
            n2->unk_10 = (u32)func_s46_80024E5C;
            func_8004491C(n2, (u32)D_80045340);
            f = D_80025B40;
            *(u8 **)(r + 0x2C) = f;
            func_80047784(r, f[((D_80083228 + *(s16 *)(o + 0x2A) + 0x100) >> 9) & 7], 0);
            w = n2->unk_08;
            w->unk_00 = v->unk_00;
            w->unk_04 = v->unk_04;
            w->unk_08 = v->unk_08;
            *(u32 *)(p + 0xA8) = (u32)n2;
            *(u32 *)(p + 0xAC) = (u32)w;
            *(s16 *)(p + 0x88) = 0x63;
        }
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 3:
        if (*(s16 *)(p + 0x88) == 0x63)
        {
            n2 = *(UnkNode **)(p + 0xA8);
            w = *(UnkVec **)(p + 0xAC);
            if ((n2->unk_1E & 0x8000) != 0)
            {
                *(s16 *)(p + 0x88) = 0;
            }
            else
            {
                v->unk_00 = w->unk_00;
                v->unk_04 = w->unk_04;
                v->unk_08 = w->unk_08;
            }
        }
        if (*(s16 *)(p + 0x86) != 0)
        {
            *(s16 *)(p + 0x84) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            if (*(s32 *)(o + 0x60) != 0)
            {
                h = func_800990FC();
                hm = h;
                h = func_80099734(*(u8 **)(o + 0x60), h);
                h = func_80099194(D_s46_80024024, h);
                func_80099290(h);
                func_800A5720(hm);
            }
        }
        break;
    case 4:
        *(s16 *)(p + 0x84) = *(u16 *)(p + 0x84) + 1;
        if (*(s16 *)(p + 0x84) >= 0xB)
        {
            ((s16 *)p)[0x42] = ((u16 *)p)[0x42] - 1;
            if (D_80025B60 == 0)
            {
                D_8008346C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            else
            {
                D_80025B60 = 0;
            }
        }
        break;
    }
}
