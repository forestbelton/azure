#include "common.h"

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
void func_s18_800241A8(u8 *p, UnkVec *v, UnkSlot *s, s16 d);

extern u8 D_800E1CB0[];
extern u8 D_s18_80024004[];
extern u8 D_s18_80024034[];
s32 func_8009D218_2(u8 *, s32) __asm__("func_8009D218");
s32 func_800A6870(s16);
s16 func_800A48F0_i(u8 *, s32, s32) __asm__("func_800A48F0");
void func_80099844(u8 *, u8 *);
void *func_800990FC(void);
void *func_80099194(u8 *, u8 *);
void *func_80099734(u8 *, u8 *);
u8 *func_80099290(u8 *);
s32 func_800A5720(void *);

/* Apply status kind 0x13 at a strength func_800A6870 derives from the second
   parameter, then -- gated on the node's +0x14 & 0x4000 on both paths --
   print the resident D_800E1CB0 message if the status took, or assemble the
   three-part bank-local message D_s18_80024004 / name / D_s18_80024034 if it
   did not.

   The gate is csrc/ovl_dng_s20.c's func_s20_800262B8 (func_8009D218 through
   a TWO-argument alias, func_800A48F0 through an s16-returning one) and the
   dialog chain is csrc/ovl_dng_b21.c's func_b21_80175E14 with one extra
   func_80099194 in front of the func_80099734 name insert.

   The second parameter is `u8', which is what makes the
   `andi $a0, $s0, 0xFF' ahead of the func_800A6870 call its own truncation
   rather than a mask -- s20's entry measures the s32 spelling at 9 differing
   in its own body; nothing had to be fought for here.
   60 words, 0 differing, first compile.

   D_s18_80024004 and D_s18_80024034 ARE EXTERNS, NOT DEFINITIONS: they sit in
   this object's `data' subsegment, 0x1894800..0x1894838, below the `.rodata'
   one that owns D_s17_80024058.
   THIS COPY'S OWN CONSTANT: 0x2 -> 0x4, 0x13 -> 0x14, 0x4 -> 0x2.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
extern s16 D_8002694C;
s32 func_800BCB04(s32, s32, s16);

typedef struct UnkDngS18TileSP
{
    u8 pad_00[0x4];
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    u8 pad_0A[0x12];
    u8 *unk_1C;
    u8 pad_20[0x4];
    u8 *unk_24;
    u8 pad_28[0xC];
    VECTOR unk_34;
    u8 pad_44[0x10];
    MATRIX unk_54;
    MATRIX unk_74;
    u8 pad_94[0x10];
    SVECTOR unk_A4;
    u8 pad_AC[0x24];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDngS18TileSP;
UnkNode *func_8003FD64(s32, void *);

typedef struct UnkDngS18Elem
{
    u8 unk_00[4];
    u8 unk_04[4];
    u8 unk_08[4];
} UnkDngS18Elem;
typedef struct UnkDngS18Blk32
{
    u8 b[0x20];
} UnkDngS18Blk32;
typedef struct UnkDngS18Rec
{
    s16 a;
    u16 b;
} UnkDngS18Rec;

s32 rand(void);
UnkNode *func_8003FC64(s32);
s32 func_80064584(s32);
s32 func_800644B8(s32);

extern UnkDngS18Elem D_80026940;

extern UnkDngS18Elem D_s18_80026934;
extern u8 D_80045340[];
extern u8 D_800DDC40[];
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s32 D_8008346C;
u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
s32 func_800A4778(u16, u16, s16, s32);
s32 func_800A56E0(s32);
void func_s18_800240B8_3(u8 *, u8, u8 *) __asm__("func_s18_800240B8");

void func_s18_800240B8(u8 *p, u8 d)
{
    void *m;
    void *h;

    if (func_8009D218_2(p, 4) == 0)
    {
        if ((s16)func_800A48F0_i(p, 0x14, (s8)(func_800A6870(d) + 2)) != 0)
        {
            if ((*(s32 *)(p + 0x14) & 0x4000) != 0)
            {
                func_80099844(p, D_800E1CB0);
            }
        }
        else if ((*(s32 *)(p + 0x14) & 0x4000) != 0)
        {
            h = func_800990FC();
            m = h;
            h = func_80099194(D_s18_80024004, h);
            h = func_80099734(p, h);
            h = func_80099194(D_s18_80024034, h);
            func_80099290(h);
            func_800A5720(m);
        }
    }
}

/* One frame of the slot's element list: for every record in the 0xC-byte chain
   hanging off `s->unk_08', compose the slot's rotation with the camera matrix
   in the 0x1F800000 scratchpad and then draw EITHER a semi-transparent
   textured quad (when the payload's +0x4E byte is set) OR a semi-transparent
   flat line plus its own DR_MODE (when it is clear), both off the four
   corners the payload keeps at +0x58..+0x6F.  The quad half re-reads the
   element's own UV/CLUT/TPAGE bytes and corrects the right/bottom edge by one
   texel; the line half just averages the corners pairwise.  The matrix stack
   is popped once, after the whole list.
   510 words, 0 differing.

   TWIN: the LANDED func_s17_80024188 in csrc/ovl_dng_s17.c, 351 words.
   dngtwin scored it 0.685 -- fifteen thousandths BELOW its 0.70 floor -- so
   it reported `no carved sibling'.  What found it was the DATA/CALLEE route:
   this unit's own already-present but unused `UnkDng74334SP' typedef, plus
   `setPolyFT4' being used in exactly one other file.  Same (p, v, s, d)
   signature, same -0x70 frame with the same nine saves s0..s7/fp in the same
   order, and block A below is func_s17_80024188 statement for statement with
   the vertices moved from p+0x50 to p+0x58.  What is NEW here is the element
   LOOP, the second (line) draw, and s17's `(e[0] & 0x20)' gate being replaced
   by the payload's +0x4E byte.

   THE ELEMENT LIST IS ONE SOURCE POINTER AND loop.c MANUFACTURES THE SECOND
   REGISTER.  The target runs two registers over the list, $s6 at e+0 and $s3
   at e+1, both bumped by 0xC; that is ONE biv plus a loop.c giv, not two
   source locals.  Measured, all three spellings:
     - one `e' with s17's offsets (e[1], e+4, e+6, e[8..0xB]): 510 words,
       0 aligned differences -- loop.c reduces the e+1 base itself.
     - a SECOND source local `c = e + 1' advanced explicitly beside `e':
       516 words / 67 rows (`c += 0xC' first) or 69 (`e += 0xC' first).  With
       `c' a real biv, loop.c reduces a THIRD base at e+4, which is a tenth
       long-lived value for nine callee-saved registers, so `d' spills to a
       new 8-byte stack slot and comes back in $t2.
     - `c = e + 1' recomputed at the top of the loop body: 508 words / 60 rows
       -- one biv, so jump.c inverts the exit test and folds the `j' away and
       the loop tail comes out TWO WORDS SHORT.
   The 2-word deficit is the diagnostic: a loop whose tail is short by exactly
   the `j'/increment pair has one induction variable where the target has two.

   FIVE MORE FACTS, ALL MEASURED ON THIS BODY:
     - HOLDS: the `p[0x4E]' gate is TWO SEQUENTIAL `if's, not an if/else.
       As `if (...) {A} else {B}' it is 506 words / 9 rows: 4 words shorter,
       because the second arm's own re-load and re-test of p[0x4E] vanish.
     - HOLDS: block B's colour word `*(s32 *)(q + 0x4)' is written BEFORE
       setLineF2/setSemiTrans.  After them it is 510 words / 14 rows.
     - HOLDS: block B's DR_MODE prim gets its OWN local `r'.  Reusing `q' is
       510 words / 7 rows, every row the last addPrim's second half, where the
       target does an in-place `and $s0, $s0, $s5' on a pointer that dies
       there and the reused `q' forces `and $v1, $s0, $s5' instead.
       func_s18_800249F4 below prices the identical fact at the identical 7.
     - HOLDS: the four on-screen tests are `|' and not `||' -- as `||' this is
       513 words / 21 rows, which is s59's recorded 20 for the same sentence.
     - HOLDS: the two edge corrections are written `(q + 0x8) > (q + 0x20)'
       and `(q + 0xA) > (q + 0x22)', i.e. the LOW offset on the left.  Both
       flipped to `(q + 0x20) < (q + 0x8)' it is 510 words / 8 rows with
       nothing moved -- the comparison-operand-order lever, on a second body.
     - HOLDS: `e = (u8 *)s->unk_08;' comes BEFORE `sp->unk_28 = s->unk_14;'.
       Swapped it is 510 words / 15 rows (s59 priced the same sentence at 3).
     - HOLDS: `f = e[1]' goes through a local.  Inlining `e[1]' at all three
       sites is 515 words / 14 rows.
   MEASURED AND FREE, so it is NOT the reason this lands: the scale vector as
   ONE CHAINED assignment.  Three separate stores written vz, vy, vx also
   measure 510 words / 0 -- only the store ORDER matters, exactly as s59's
   comment says.  This is FREE and not merely unmeasurable: the two source
   trees differ and the output does not.

   This function owns NO jump table: its listing has no `.section .rodata'.
*/
void func_s18_800241A8(u8 *p, UnkVec *v, UnkSlot *s, s16 d)
{
    UnkDng74334SP *sp = (UnkDng74334SP *)0x1F800000;
    Unk80083160 *g;
    MATRIX m;
    u8 *q;
    u8 *r;
    u8 *e;
    u8 f;

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
    for (;;)
    {
        if (p[0x4E] != 0)
        {
            q = *(u8 **)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)q + 0x28;
            sp->unk_0C.w = e[8];
            sp->unk_10.w = e[9];
            sp->unk_14.w = e[10];
            sp->unk_18.w = e[11];
            sp->unk_B0[0].vx = *(u16 *)(p + 0x58);
            sp->unk_B0[1].vx = *(u16 *)(p + 0x5A);
            sp->unk_B0[2].vx = *(u16 *)(p + 0x5C);
            sp->unk_B0[3].vx = *(u16 *)(p + 0x5E);
            sp->unk_B0[0].vy = *(u16 *)(p + 0x60);
            sp->unk_B0[1].vy = *(u16 *)(p + 0x62);
            sp->unk_B0[2].vy = *(u16 *)(p + 0x64);
            sp->unk_B0[3].vy = *(u16 *)(p + 0x66);
            sp->unk_B0[0].vz = *(u16 *)(p + 0x68);
            sp->unk_B0[1].vz = *(u16 *)(p + 0x6A);
            sp->unk_B0[2].vz = *(u16 *)(p + 0x6C);
            sp->unk_B0[3].vz = *(u16 *)(p + 0x6E);
            sp->unk_100 = RotAverage4(&sp->unk_B0[0], &sp->unk_B0[1], &sp->unk_B0[2], &sp->unk_B0[3],
                                      (s32 *)(q + 0x8), (s32 *)(q + 0x10), (s32 *)(q + 0x18),
                                      (s32 *)(q + 0x20), &sp->unk_D0, &sp->unk_D4) -
                          d - 6;
            if ((u32)sp->unk_100 < 0x1E0)
            {
                if (((u16)(*(u16 *)(q + 0x8) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0xA) + 0x20) < 0x121) |
                    ((u16)(*(u16 *)(q + 0x10) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x12) + 0x20) < 0x121) |
                    ((u16)(*(u16 *)(q + 0x18) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x1A) + 0x20) < 0x121) |
                    ((u16)(*(u16 *)(q + 0x20) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x22) + 0x20) < 0x121))
                {
                    setPolyFT4(q);
                    s->unk_14 &= 0x7FFF;
                    sp->unk_14.w += sp->unk_0C.w;
                    if ((sp->unk_14.w & 0x100) != 0)
                    {
                        sp->unk_14.w -= 1;
                    }
                    sp->unk_18.w += sp->unk_10.w;
                    if ((sp->unk_18.w & 0x100) != 0)
                    {
                        sp->unk_18.w -= 1;
                    }
                    sp->unk_10.w <<= 8;
                    sp->unk_18.w <<= 8;
                    if (s->unk_12 != 0)
                    {
                        if ((sp->unk_28 & 0x100) != 0)
                        {
                            *(u16 *)(q + 0xE) = s->unk_12;
                        }
                        else
                        {
                            *(u16 *)(q + 0xE) = s->unk_12 + *(u16 *)(e + 0x6);
                        }
                    }
                    else
                    {
                        *(u16 *)(q + 0xE) = *(u16 *)(e + 0x6);
                    }
                    *(u16 *)(q + 0xC) = sp->unk_10.h + sp->unk_0C.h;
                    *(u16 *)(q + 0x14) = sp->unk_10.h + sp->unk_14.h;
                    if (s->unk_10 != 0)
                    {
                        *(u16 *)(q + 0x16) = s->unk_10 + (*(u16 *)(e + 0x4) & 0xFF9F);
                    }
                    else
                    {
                        *(u16 *)(q + 0x16) = *(u16 *)(e + 0x4);
                    }
                    *(u16 *)(q + 0x1C) = sp->unk_18.h + sp->unk_0C.h;
                    *(u16 *)(q + 0x24) = sp->unk_18.h + sp->unk_14.h;
                    if (*(s16 *)(q + 0x8) > *(s16 *)(q + 0x20))
                    {
                        q[0x14] -= 1;
                        q[0x24] -= 1;
                    }
                    if (*(s16 *)(q + 0xA) > *(s16 *)(q + 0x22))
                    {
                        q[0x1D] -= 1;
                        q[0x25] -= 1;
                    }
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
                    *(s32 *)(q + 0x4) = *(s32 *)s->unk_0C;
                    addPrim(sp->unk_24 + sp->unk_100 * 4, q);
                }
            }
        }
        if (p[0x4E] == 0)
        {
            q = *(u8 **)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)q + 0x10;
            sp->unk_B0[0].vx = sp->unk_B0[2].vx = (*(s16 *)(p + 0x58) + *(s16 *)(p + 0x5C)) / 2;
            sp->unk_B0[1].vx = sp->unk_B0[3].vx = (*(s16 *)(p + 0x5A) + *(s16 *)(p + 0x5E)) / 2;
            sp->unk_B0[0].vy = sp->unk_B0[2].vy = (*(s16 *)(p + 0x60) + *(s16 *)(p + 0x64)) / 2;
            sp->unk_B0[1].vy = sp->unk_B0[3].vy = (*(s16 *)(p + 0x62) + *(s16 *)(p + 0x66)) / 2;
            sp->unk_B0[0].vz = sp->unk_B0[2].vz = (*(s16 *)(p + 0x68) + *(s16 *)(p + 0x6C)) / 2;
            sp->unk_B0[1].vz = sp->unk_B0[3].vz = (*(s16 *)(p + 0x6A) + *(s16 *)(p + 0x6E)) / 2;
            sp->unk_100 = RotAverage4(&sp->unk_B0[0], &sp->unk_B0[1], &sp->unk_B0[2], &sp->unk_B0[3],
                                      (s32 *)(q + 0x8), (s32 *)(q + 0xC), (s32 *)(q + 0x8),
                                      (s32 *)(q + 0xC), &sp->unk_D0, &sp->unk_D4) -
                          d - 6;
            if ((sp->unk_28 & 0x8) != 0)
            {
                if ((sp->unk_28 & 0x4) != 0)
                {
                    s->unk_0C[3] |= 0x2;
                }
                else
                {
                    s->unk_0C[3] &= 0xFD;
                }
            }
            *(s32 *)(q + 0x4) = *(s32 *)s->unk_0C;
            setLineF2(q);
            setSemiTrans(q, 1);
            addPrim(sp->unk_24 + sp->unk_100 * 4, q);
            r = *(u8 **)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)r + 0xC;
            func_80067F20((u32 *)r, 0, 0, GetTPage(0, 1, 0, 0), 0);
            addPrim(sp->unk_24 + sp->unk_100 * 4, r);
        }
        if (*(s8 *)e < 0)
        {
            break;
        }
        e += 0xC;
    }
    PopMatrix();
}

/* Walk the tag chain from `p` and hand every node to func_s18_800241A8 with a
   zero fourth argument.  21 words, 0 differing, first compile.

   The twin is func_b06_80173E08 two listings up in THIS unit -- the same
   walk with a different per-node call -- and csrc/ovl_dungeon_800D50FC.c's
   func_800D67B0 is the resident original, identical down to the
   `s = (UnkSlot *)n->unk_0C;` cast.  Nothing was fought for; the loop is a
   `for (;;)` with the NULL test in the middle, which is what puts the
   `addiu $s0, $a2, 0x20` in the `beqz`'s delay slot. */
s32 func_s18_800249A0(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_s18_800241A8(p, v, s, 0);
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

/* Walk the tag chain from `p' and, per node, draw one semi-transparent 1x1
   tile plus its own DR_MODE for every entry of the payload's +0x14-long
   parallel arrays -- x at p+0x16, y at p+0x44, z at p+0x72, all u16 at
   stride 2 -- projected through the slot's rotation composed with the camera
   matrix in the 0x1F800000 scratchpad at a 0x1000 scale.  Each tile takes the
   payload's colour word at +0x0 and then fades all three bytes by the
   payload's s16 counter at +0x10 over its s16 limit at +0x12, which is
   func_s18_80024DA0's fade above.  PushMatrix/PopMatrix bracket each node.
   211 words, 0 differing.

   NO TWIN BY EITHER TOOL -- dngtwin finds no carved sibling at 0.70 and the
   resident (p, v, s) bodies that call both ScaleMatrix and RotTransPers
   (func_800A7CC4, func_800B06F0, func_800B51C8) are different routines.  Its
   shape came from THIS UNIT instead: the matrix chain and the two-prim dress
   are func_s18_800241A8's above, the tag-chain walk is func_s18_800249A0's,
   and the fade plus `setTile1'/`setSemiTrans' is func_s18_80024DA0's.

   FOUR FACTS MEASURED ON THIS BODY:
     - HOLDS: `sp->unk_1C = *(u8 **)(g->unk_00 + 0x8D0);' is written BEFORE
       `sp->unk_24 = g->unk_00 + 0xB0;' even though the target STORES unk_24
       first -- the scheduler swaps the two stores, but it cannot lift the
       0x8D0 load above a store through a different base register.  Written
       in store order it is 212 words / 6 rows: one extra `nop' in the load
       delay slot and the $v0/$v1 pair exchanged.
     - HOLDS: `g' is declared and initialised BEFORE `sp'.  The other way is
       212 words / 10 rows -- the same 6 plus 4 prologue rows, because the
       order of the two `lui's decides the order of the $s1 and $fp saves.
     - HOLDS: the loop counter is `s32'.  As `s16' it is 215 words / 85 rows.
     - HOLDS: the DR_MODE prim gets its OWN local `r'.  Reusing `q' is 211
       words / 7 rows -- the SAME figure and the same in-place `and $s0, $s0,
       $s4' that func_s18_800241A8 above measures at 7.  Two bodies, one
       price; this one is worth carrying.
     - HOLDS: `setTile1'/`setSemiTrans' come AFTER the three fade statements.
       Before them it is 211 words / 14 rows.
   MEASURED AND FREE, so neither is the reason this lands:
     - the three array reads spelled `((u16 *)(p + 0x16))[i]' instead of
       `*(u16 *)(p + 0x16 + i * 2)' -- 211 words / 0 either way; loop.c builds
       the same single giv at p + i*2 from both trees.  FREE, not
       unmeasurable: the source trees differ and the output does not.
     - the scale vector as one chained assignment.  Three separate stores
       vz, vy, vx also measure 211 / 0.

   `UnkDngS18TileSP' is a new typedef and is NOT `UnkDng74334SP': the
   RotTransPers input SVECTOR sits at +0x04 and the cursor snapshot at +0x1C,
   both inside UnkDng74334SP's padding.  Everything from +0x24 up agrees
   field for field with it.

   This function owns NO jump table: its listing has no `.section .rodata'.
*/
s32 func_s18_800249F4(u8 *p, UnkVec *v, UnkSlot *s)
{
    Unk80083160 *g = &D_80083160;
    UnkDngS18TileSP *sp = (UnkDngS18TileSP *)0x1F800000;
    MATRIX m;
    u8 *q;
    u8 *r;
    UnkNode *n;
    s32 i;

    for (;;)
    {
        sp->unk_74.t[0] = *(s16 *)((u8 *)v + 0x2);
        sp->unk_74.t[1] = *(s16 *)((u8 *)v + 0x6);
        sp->unk_74.t[2] = *(s16 *)((u8 *)v + 0xA);
        PushMatrix();
        sp->unk_34.vx = sp->unk_34.vy = sp->unk_34.vz = 0x1000;
        sp->unk_A4.vx = s->unk_16;
        sp->unk_A4.vz = s->unk_1A;
        sp->unk_A4.vy = s->unk_18;
        RotMatrix(&sp->unk_A4, &sp->unk_74);
        ReadRotMatrix(&m);
        CompMatrix(&m, &sp->unk_74, &sp->unk_54);
        ScaleMatrix(&sp->unk_54, &sp->unk_34);
        SetTransMatrix(&sp->unk_54);
        SetRotMatrix(&sp->unk_54);
        sp->unk_1C = *(u8 **)(g->unk_00 + 0x8D0);
        sp->unk_24 = g->unk_00 + 0xB0;
        for (i = 0; i < *(s16 *)(p + 0x14); i++)
        {
            sp->unk_04 = *(u16 *)(p + 0x16 + i * 2);
            sp->unk_06 = *(u16 *)(p + 0x44 + i * 2);
            sp->unk_08 = *(u16 *)(p + 0x72 + i * 2);
            q = *(u8 **)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)q + 0xC;
            sp->unk_100 = RotTransPers((SVECTOR *)&sp->unk_04, (s32 *)(q + 0x8), &sp->unk_D0, &sp->unk_D4);
            if ((u32)sp->unk_100 < 0x1E0)
            {
                *(s32 *)(q + 0x4) = *(s32 *)(p + 0x0);
                q[0x4] = q[0x4] * *(s16 *)(p + 0x10) / *(s16 *)(p + 0x12);
                q[0x5] = q[0x5] * *(s16 *)(p + 0x10) / *(s16 *)(p + 0x12);
                q[0x6] = q[0x6] * *(s16 *)(p + 0x10) / *(s16 *)(p + 0x12);
                setTile1(q);
                setSemiTrans(q, 1);
                addPrim(sp->unk_24 + sp->unk_100 * 4, q);
                r = *(u8 **)(g->unk_00 + 0x8D0);
                *(u32 *)(g->unk_00 + 0x8D0) = (u32)r + 0xC;
                func_80067F20((u32 *)r, 0, 0, GetTPage(0, 1, 0, 0), 0);
                addPrim(sp->unk_24 + sp->unk_100 * 4, r);
            }
        }
        PopMatrix();
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

/* Add the payload's +0xA0 halfword into the slot's +0x1A halfword, spend 0x20
   of the payload's +0x10 life counter, raise the engine's D_8002694C tick,
   and when the counter runs out set the 0x8000 kill bit in the node's -0x2
   flags and in D_800814A0.
   24 words, 0 differing, FIRST COMPILE.

   The twin is the LANDED func_s58_80024374 in csrc/ovl_dng_s58.c (20 words,
   dngtwin 0.909; func_s51_800249A0, func_s41_80025294 and func_s39_80025348
   are the same body).  The four extra words are the slot's +0x1A accumulate,
   which is written FIRST; the relink is counter +0x32 -> +0x10, step 8 ->
   0x20 and flag D_800269B4 -> D_8002694C.

   BOTH of the twin's measured facts HOLD, at this body's own prices:
     - the decrement is read into an `s16' local BEFORE the D_8002694C store.
       Folded as `if ((*(s16 *)(p + 0x10) -= 0x20) <= 0)' with the flag store
       first it is 25 words / 12 aligned differences (the twin recorded 20
       words / 11 for the same shape).
     - the local is `s16', not `s32'.  As `s32' the `sll 16'/`bgtz' pair goes
       and it is 22 words / 5 aligned differences (the twin recorded 19 / 19).
   The accumulate reads BOTH halfwords unsigned -- `*(u16 *)((u8 *)s + 0x1A)
   += *(u16 *)(p + 0xA0)' -- which is what gives the target's two `lhu's
   rather than an `lh'.

   This function owns NO jump table: its listing has no `.section .rodata'.
*/
void func_s18_80024D40(u8 *p, UnkVec *v, UnkSlot *s)
{
    s16 t;

    *(u16 *)((u8 *)s + 0x1A) += *(u16 *)(p + 0xA0);
    t = *(s16 *)(p + 0x10) - 0x20;
    D_8002694C = 1;
    *(s16 *)(p + 0x10) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* One semi-transparent 1x1 tile and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad, with the tile's colour word
   copied off the payload at +0x8 and then faded: each of the three bytes is
   scaled by the payload's s16 counter at +0x32 over its s16 limit at +0x34.

   The twin is the LANDED func_s16_8002416C in csrc/ovl_dng_s16.c, also 148
   words -- same scratchpad slots, same addPrim pair, same
   setTile1/setSemiTrans fold -- differing in ONE token: the fade's divisor is
   the loaded `*(s16 *)(p + 0x34)' here where s16 divides by the constant
   0x100. That is why both come out at 148: the variable divide's
   lh/div/mflo triple costs exactly what the constant divide's
   bgez/addiu-0xFF/sra correction does.

   Landed on the FIRST COMPILE, so the sibling's three measured orderings were
   taken as given and are NOT re-measured here -- the colour word and its
   three scalings before the tag setup, `sp->unk_18' assigned before
   `sp->unk_20', and `p = (u8 *)n + 0x20;' before `v = n->unk_08;'.
   148 words, 0 differing.
   THIS COPY'S OWN CONSTANT: 0x8 -> 0x0 at 1 of 2 sites, 0x32 -> 0x10 at all 3 sites, 0x34 -> 0x12 at all 3 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
s32 func_s18_80024DA0(u8 *p, UnkVec *v)
{
    UnkDng73E94SP *sp = (UnkDng73E94SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    TILE_1 *t;
    DR_MODE *dm;

    sp->unk_18 = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    while (1)
    {
        sp->unk_00 = *(u16 *)((u8 *)v + 0x2);
        sp->unk_02 = *(u16 *)((u8 *)v + 0x6);
        sp->unk_04 = *(u16 *)((u8 *)v + 0xA);
        t = (TILE_1 *)sp->unk_18;
        sp->unk_18 = (u8 *)(t + 1);
        sp->unk_C0 = RotTransPers((SVECTOR *)sp, (s32 *)&t->x0, &sp->unk_90, &sp->unk_94);
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            *(u32 *)&t->r0 = *(u32 *)(p + 0x0);
            t->r0 = t->r0 * *(s16 *)(p + 0x10) / *(s16 *)(p + 0x12);
            t->g0 = t->g0 * *(s16 *)(p + 0x10) / *(s16 *)(p + 0x12);
            t->b0 = t->b0 * *(s16 *)(p + 0x10) / *(s16 *)(p + 0x12);
            setTile1(t);
            setSemiTrans(t, 1);
            addPrim(sp->unk_20 + sp->unk_C0 * 4, t);
            dm = (DR_MODE *)sp->unk_18;
            sp->unk_18 = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            addPrim(sp->unk_20 + sp->unk_C0 * 4, dm);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_18;
    return 0;
}

/* Turn the mover towards func_800BCB04's bearing -- gliding one frame by the
   payload's +0x48 step, but only while the mover is still below that bearing
   -- and run the +0x32 countdown down by 4 a frame, raising the node's redraw
   bit and D_800814A0's at zero. csrc/ovl_dungeon_800CABA8.c's func_800CAED0
   is the twin: the same bearing test over 67 words, with a full three-axis
   glide where this has one axis, no `else' arm and a 4-step countdown instead
   of 8.

   THE THREE ARGUMENT LOADS ARE THREE LOCALS, ASSIGNED BEFORE THE D_8002694C
   STORE, IN THE ORDER +0xA, +0x2, +0x6. Written the twin's way, with the three
   loads inside the call, it is 6 differing at the right 48 words, every row
   the `li 1'/`sh' pair and the three `lhu's trading places. The cause is a
   true dependence, read out of `tools/decomp/sched.py --rtl' and not
   inferred: gcc 2.7.2 cannot disambiguate the `mem:HI (lo_sum D_8002694C)'
   store from `mem:HI (s0 + 10)', so every argument load carries insn 17 --
   the store -- in its dependency list and can never be scheduled above it.
   calls.c emits a call's argument register loads at the very END of its
   expansion, so the only way to get them above the store is to evaluate them
   in an earlier statement.

   Measured and rejected, all at 48 words: the twin's spelling 6; the call
   inlined into the comparison 6; `*(s16 *)&D_8002694C = 1' 6; hoisting only
   the +0xA load 4; hoisting all three in source order +0x2, +0x6, +0xA 3 --
   the three `lhu's then come out in that order against the target's +0xA
   first, the tie breaking on LUID, which is why the +0xA load is written
   first. `do { D_8002694C = 1; } while (0)' is 49 words and 44 differing.
   The `+ 2' stays inside the argument: its `addiu' sits AFTER the store in
   the target.

   Macro residue rather than a device -- three loads named and then handed to
   one call is what a bearing-test macro leaves behind. 48 words,
   0 differing.
   THIS COPY'S OWN CONSTANT: 0x48 -> 0xb4, 0x32 -> 0x10.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s18_80024FF0(u8 *p, UnkVec *v)
{
    s32 x;
    s32 y;
    s32 z;
    s16 r;

    z = *(u16 *)((u8 *)v + 0xA);
    x = *(u16 *)((u8 *)v + 0x2);
    y = *(u16 *)((u8 *)v + 0x6);
    D_8002694C = 1;
    r = func_800BCB04(x, y, z + 2);
    if (*(s16 *)((u8 *)v + 0xA) < r)
    {
        v->unk_08 += *(s32 *)(p + 0xb4);
    }
    if ((*(s16 *)(p + 0x10) -= 4) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* A 0x211 spawner: seed the new node's mover from the parent `a''s, jitter each
   axis by a fresh rand() & 0x1F and the caller's own offset less 0x10, then
   park func_s18_80024FF0 as the step and func_s18_80024DA0 as the draw, with
   `b' at the payload's +0x4, `d' at BOTH +0x10 and +0x12, a
   rand() + 0x10000 step at +0xB4 and `c' at +0x0.
   78 words, 0 differing, FIRST COMPILE.

   The twin is the LANDED func_s59_800248A8 in csrc/ovl_dng_s59.c (78 words,
   dngtwin 0.987); func_s17_80025338 is the same body at the same ratio.  This
   bank's relink is the whole delta: the payload offsets 0x14, 0x32, 0x34,
   0x48 and 0x8 become 0x4, 0x10, 0x12, 0xB4 and 0x0, which is the same
   mapping func_s18_80024FF0 and func_s18_80024DA0 above already carry, and
   `c' lands at q+0x0 so the store reads `sw $s7, 0x20($s1)' off the node
   rather than off `q'.  Both installed handlers are defined ABOVE this point
   in the file, so no forward declaration was needed.

   The twin landed first-compile too and took its own twin's two facts as
   given, so BOTH were re-measured here and both HOLD -- at this body's own
   prices, not the recorded ones:
     - the three stack parameters are `s16'.  As `s32' this is 74 words /
       43 aligned differences / 75 differing, the three save-restore pairs
       missing (the sibling recorded 66 words / 71 differing).
     - the subtraction wears its own `(s16)' cast, which stops fold
       associating the `addiu -0x10' onto the running sum.  Without it this is
       80 words / 23 aligned differences (the sibling recorded 74 / 20).
   ONE MORE, MEASURED HERE AND NOT STATED BY EITHER TWIN: the second `sh' of
   `d' into +0x12 must be written BEFORE the func_8004491C call, because it
   rides in that call's delay slot.  Moved after the call it is 77 words /
   4 aligned differences.

   This function owns NO jump table: its listing has no `.section .rodata'.
*/
void func_s18_800250B0(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *q;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s18_80024FF0;
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x1F) + (s16)(dx - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x1F) + (s16)(dy - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x1F) + (s16)(dz - 0x10);
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x4) = b;
        *(s16 *)(q + 0x10) = d;
        *(s16 *)(q + 0x12) = d;
        func_8004491C(n, (u32)func_s18_80024DA0);
        *(s32 *)(q + 0xB4) = rand() + 0x10000;
        *(u32 *)(q + 0x0) = c;
    }
}

/* A 0x212 spawner for the 0x17-point sphere shell: allocate the node, then
   fill the payload's three parallel u16 arrays -- x at +0x16, y at +0x44,
   z at +0x72, all stride 2 -- with 0x17 points at radius `a' on a random
   great circle, two `rand() & 0xFFF' angles per point through
   func_80064584 and func_800644B8 with a `>> 12' after each multiply.  Then
   seed the mover from (dx, dy, dz), set the slot's +0x1C/+0x1E scale to
   0x1000, write the point count 0x17 at q+0x14, park func_s18_80024D40 as
   the step and func_s18_800249F4 as the draw, put a
   `(rand() & 0x3F) + 0x3C' life at q+0xA0, `c' at BOTH q+0x10 and q+0x12 and
   `b' at q+0x0.
   102 words, 0 differing.

   NO TWIN: the draw it installs is func_s18_800249F4 above, which reads
   exactly these three arrays at exactly these offsets, so no offset needed
   guessing -- the spawner/step pair in this unit interprets itself.  The
   frame is -0x40 with all nine callee-saved registers live, which is why dx,
   dy and dz are SPILLED to 0x10/0x12/0x14($sp): that is allocation, not
   source, and the home slots at 0x50/0x54 are where they arrive.

   ONE FACT, MEASURED ON THIS BODY AFTER IT REACHED 0, with two symptoms:
     - HOLDS: `n->unk_08' goes through the local `m'.  Spelled inline at all
       three mover stores it is 104 words / 21 aligned differences.  The two
       extra words are two extra `lw 0x8($s1)': each
       `*(s16 *)((u8 *)n->unk_08 + N)' store is a store through a loaded
       pointer and gcc 2.7.2 cannot disambiguate it from the `n->unk_08' load
       off `n', so cse invalidates the load and every later use reloads.  The
       other 19 rows are the knock-on -- the extra pressure costs `c' its
       callee-saved register and dx takes $fp instead, which exchanges the
       two for the rest of the body.  It is ONE edit and ONE fact, so do not
       read it as two.

   This function owns NO jump table: its listing has no `.section .rodata'.
*/
void func_s18_800251E8(s32 a, u32 b, s16 c, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    UnkSlot *s;
    UnkVec *m;
    u8 *q;
    s32 i;
    s32 r;
    s32 u;
    s32 w;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        for (i = 0; i < 0x17; i++)
        {
            u = rand() & 0xFFF;
            w = rand() & 0xFFF;
            r = a * func_80064584(u) >> 12;
            *(s16 *)(q + 0x16 + i * 2) = r * func_800644B8(w) >> 12;
            *(s16 *)(q + 0x44 + i * 2) = r * func_80064584(w) >> 12;
            *(s16 *)(q + 0x72 + i * 2) = a * func_800644B8(u) >> 12;
        }
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = dx;
        *(s16 *)((u8 *)m + 0x6) = dy;
        *(s16 *)((u8 *)m + 0xA) = dz;
        s = (UnkSlot *)n->unk_0C;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
        *(s16 *)(q + 0x14) = 0x17;
        n->unk_10 = (u32)func_s18_80024D40;
        *(s16 *)(q + 0xA0) = (rand() & 0x3F) + 0x3C;
        *(s16 *)(q + 0x10) = c;
        *(s16 *)(q + 0x12) = c;
        func_8004491C(n, (u32)func_s18_800249F4);
        *(u32 *)(q + 0x0) = b;
    }
}

/* The +0x42 phase driver for the same node: raise D_8002694C, spend one
   frame of the +0x2 life counter, then run three INDEPENDENT gates on the
   s32 at +0x40 -- 0 averages +0x1E/+0x58 and +0x2A/+0x60 into +0x20/+0x2C
   and publishes them to +0x5A/+0x5E and +0x62/+0x66; 1 does the same but
   averages a SECOND time against the value it has just stored; 0x10000
   copies +0x1E and +0x2A straight across -- and when the LOW halfword of
   +0x40 is 1, clears it and dispatches the nine-case +0x42 phase machine
   through jtbl_s18_80024038, otherwise bumps +0x40 by one.  Finally it fades
   the slot's three colour bytes by the remaining life over 30 and sets the
   0x8000 kill bits at zero.
   199 words, 0 differing.

   OWNS jtbl_s18_80024038.  9 cases, dispatch bound 9, all entries distinct,
   and the standalone compile emits 9 words with NO pad -- the ROM's tenth
   word is cc1psx' `.align 3' pad before the next rodata object, not a case.
   Entry-address order is ascending case value 0..8, confirmed by standalone
   compile: the emitted entry sequence resolves to the ROM's exactly.
   TWO THINGS THE DISPATCH ARITHMETIC SETTLES, both needed:
     - `case 8:' is an EMPTY `break' arm.  The bound of 9 is the case COUNT
       with no minval subtraction, so maxval is 8 and the case exists; its
       label collapses onto the function exit, which is why entry 8 and the
       range-check target are the same address.
     - there is NO `default:'.  The range-check `beqz' goes to the common
       tail, which is not a table entry.

   TWO FACTS, MEASURED ON THIS BODY AFTER IT REACHED 0:
     - HOLDS: in the +0x40 == 1 gate the +0x5A/+0x5E publish is its own
       statement, reading `*(s16 *)(p + 0x20)' back AFTER both +0x2C passes.
       Folded into the second +0x20 assignment as a three-deep chain it is
       197 words / 25 aligned differences.
     - HOLDS: case 7's byte pairs are CHAINS, `s->unk_0C[0] = s->unk_0C[1] =
       0xFF' and `p[0x36] = p[0x37] = 0xFF'.  As four separate ascending
       stores it is 199 words / 4 aligned differences, every row the 0xC/0xD
       and 0x36/0x37 stores transposed: a chain assigns the RIGHTMOST lvalue
       first, so it emits the HIGHER offset first, which is the order the
       target has.

   READ OFF THE LISTING, not priced, because the alternative does not
   compile to anything comparable: the read-backs' SIGNEDNESS decides whether
   cse forwards or reloads.  `sh' then `*(u16 *)' cannot be forwarded -- it
   would need a zero-extension of a value only known as truncated -- so it
   reloads, which is the `lhu 0x20($a1)' in gate 0; `sh' then `*(s16 *)' IS
   forwarded, with the `sll 16'/`sra 16' pair that gate 1 shows twice.  Match
   the cast to which of the two the listing has at each site.

   The nine arms' shared tails -- the `sb' on +0x4C that cases 1/2/3/5/6
   reach and the exit that case 8 collapses onto -- are all jump2 cross-joins
   and need no shared locals.
*/
void func_s18_80025380(u8 *p, UnkVec *v, UnkSlot *s)
{
    D_8002694C = 1;
    *(u16 *)(p + 0x2) -= 1;
    if (*(s32 *)(p + 0x40) == 0)
    {
        *(s16 *)(p + 0x20) = (*(s16 *)(p + 0x1E) + *(s16 *)(p + 0x58)) / 2;
        *(s16 *)(p + 0x62) = *(s16 *)(p + 0x66) = *(s16 *)(p + 0x2C) =
            (*(s16 *)(p + 0x2A) + *(s16 *)(p + 0x60)) / 2;
        *(u16 *)(p + 0x5A) = *(u16 *)(p + 0x5E) = *(u16 *)(p + 0x20);
    }
    if (*(s32 *)(p + 0x40) == 1)
    {
        *(s16 *)(p + 0x20) = (*(s16 *)(p + 0x1E) + *(s16 *)(p + 0x58)) / 2;
        *(s16 *)(p + 0x20) = (*(s16 *)(p + 0x1E) + *(s16 *)(p + 0x20)) / 2;
        *(s16 *)(p + 0x2C) = (*(s16 *)(p + 0x2A) + *(s16 *)(p + 0x60)) / 2;
        *(s16 *)(p + 0x2C) = (*(s16 *)(p + 0x2A) + *(s16 *)(p + 0x2C)) / 2;
        *(s16 *)(p + 0x5A) = *(s16 *)(p + 0x5E) = *(s16 *)(p + 0x20);
        *(u16 *)(p + 0x62) = *(u16 *)(p + 0x66) = *(u16 *)(p + 0x2C);
    }
    if (*(s32 *)(p + 0x40) == 0x10000)
    {
        *(u16 *)(p + 0x5A) = *(u16 *)(p + 0x5E) = *(u16 *)(p + 0x1E);
        *(u16 *)(p + 0x62) = *(u16 *)(p + 0x66) = *(u16 *)(p + 0x2A);
    }
    if (*(s16 *)(p + 0x40) == 1)
    {
        *(s16 *)(p + 0x40) = 0;
        switch (*(s16 *)(p + 0x42))
        {
        case 0:
            *(s16 *)(p + 0x42) = 1;
            break;
        case 1:
            *(s16 *)(p + 0x42) = 2;
            p[0x4C] += 0x20;
            break;
        case 2:
            *(s16 *)(p + 0x42) = 3;
            p[0x4C] += 0x20;
            break;
        case 3:
            *(s16 *)(p + 0x42) = 4;
            p[0x4C] += 0x20;
            break;
        case 4:
            *(s16 *)(p + 0x42) = 5;
            p[0x4C] = 0x80;
            p[0x4D] += 0x20;
            break;
        case 5:
            *(s16 *)(p + 0x42) = 6;
            p[0x4C] += 0x20;
            break;
        case 6:
            *(s16 *)(p + 0x42) = 7;
            p[0x4C] += 0x20;
            break;
        case 7:
            p[0x4E] = 0;
            p[0x4F] = 0;
            s->unk_0C[0] = s->unk_0C[1] = 0xFF;
            s->unk_0C[2] = 0;
            p[0x36] = p[0x37] = 0xFF;
            p[0x38] = 0;
            s->unk_14 |= 0xC;
            break;
        case 8:
            break;
        }
    }
    else
    {
        *(s16 *)(p + 0x40) = *(u16 *)(p + 0x40) + 1;
    }
    if (*(s16 *)(p + 0x2) < 0x1E)
    {
        s->unk_0C[0] = p[0x36] * *(s16 *)(p + 0x2) / 0x1E;
        s->unk_0C[1] = p[0x37] * *(s16 *)(p + 0x2) / 0x1E;
        s->unk_0C[2] = p[0x38] * *(s16 *)(p + 0x2) / 0x1E;
    }
    if (*(s16 *)(p + 0x2) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* One frame of the bank's 0x18-step colour pulse: raise the engine's
   D_8002694C tick, spend one frame of the payload's +0x2 life counter, flip
   the +0x42 phase between 0 and 1 -- bumping the payload's +0x4C byte by
   +0x18 or -0x18 as it goes -- fade the slot's three colour bytes at +0xC by
   the remaining life over the payload's +0x4 span, and set the 0x8000 kill
   bit in the node's -0x2 flags and in D_800814A0 both when the life runs out
   and when the slot already carries 0x8000.
   81 words, 0 differing.

   NO TWIN BY EITHER TOOL: dngtwin reports no carved sibling at 0.70 for any
   stub in this unit, and dngcallee's only clean row is for the 822-word
   driver below.  The shape came from this unit's own landed bodies instead --
   the fade is func_s18_800249F4's `q[0x4] = q[0x4] * ... / ...' idiom and the
   kill-bit tail is func_s18_80024D40's.

   ONE FACT, MEASURED ON THIS BODY AFTER IT REACHED 0:
     - HOLDS: the +0x42 phase flip is a `switch' with `case 0:' and `case 1:',
       not `if (k == 0) ... else if (k == 1)'.  As the if-chain it is 79 words
       / 9 aligned differences.  Two cases are under gcc 2.7.2's
       case_values_threshold so neither form emits a jump table; what differs
       is the DISPATCH SHAPE.  expand_end_case builds a balanced tree and
       emits the arms AFTER it, giving `beqz' to arm 0, then `li 1'/`beq' to
       arm 1, then `j' to the default -- six words.  The if-chain tests in
       place with the arms inline and is two words shorter.  The diagnostic is
       the branch SENSE: the target's first branch is `beqz' FORWARD to the
       k == 0 arm, which an if-chain can never produce because its then-arm
       is the fall-through.
   The two arms' shared `sb' on +0x4C is jump2's cross-join and needs no
   shared local.

   This function owns NO jump table: its listing has no `.section .rodata'.
*/
void func_s18_8002569C(u8 *p, UnkVec *v, UnkSlot *s)
{
    s16 k;

    D_8002694C = 1;
    *(u16 *)(p + 0x2) -= 1;
    k = *(s16 *)(p + 0x42);
    switch (k)
    {
    case 0:
        *(s16 *)(p + 0x42) = 1;
        p[0x4C] += 0x18;
        break;
    case 1:
        *(s16 *)(p + 0x42) = 0;
        p[0x4C] -= 0x18;
        break;
    }
    s->unk_0C[0] = p[0x36] * *(s16 *)(p + 0x2) / *(s16 *)(p + 0x4);
    s->unk_0C[1] = p[0x37] * *(s16 *)(p + 0x2) / *(s16 *)(p + 0x4);
    s->unk_0C[2] = p[0x38] * *(s16 *)(p + 0x2) / *(s16 *)(p + 0x4);
    if (*(s16 *)(p + 0x2) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    if ((s->unk_14 & 0x8000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The +0x0 state machine for the five-point ring.  State 0 lays out five
   points at radius p+0xA around the bearing at p+0xC, one fifth of 0x1000
   apart (`(i << 12) / 5'), into the payload's two u16 arrays at +0x1E and
   +0x2A, zeroes the ring index at +0x1C and the frame counter at +0x2, bumps
   the state and FALLS THROUGH into state 1.  State 1, each time the +0x2
   counter reads 0, spawns a 0x212 node with func_s18_80025380 as its step
   and func_s18_800249A0 as its draw, copies the mover across, picks the
   quad's four corners out of the ring by the +0x1C index through
   jtbl_s18_80024060, averages +0x58/+0x62 twice into +0x20/+0x2C, paints the
   slot 0xE0/0xE0/0x20 at scale 0x1000 and parks the 0xC-byte D_80026940
   template at q+0x44 as its element list; otherwise it runs the +0x2 counter
   to 4, advances the ring index and kills the node at index 5.
   287 words, 0 differing.

   OWNS jtbl_s18_80024060.  5 cases, dispatch bound 5, all entries distinct,
   no pad in either the ROM or the standalone compile.  Entry-address order
   is ascending case value 0..4, confirmed by standalone compile: the emitted
   entry sequence resolves to the ROM's exactly.  There is no `default:' --
   the range-check `beqz' goes to the common averaging tail, which is not a
   table entry.

   FOUR FACTS, MEASURED ON THIS BODY AFTER IT REACHED 0.  The last two are
   the ones worth carrying to another body.
     - HOLDS: the dispatch value is read into the local `k' BEFORE the
       `D_8002694C = 1' store.  With the store first it is 288 words / 12
       aligned differences: gcc 2.7.2 cannot disambiguate
       `mem:HI (lo_sum D_8002694C)' from `mem:HI (s3 + 0)', so the
       `lh 0x0($s3)' can never be scheduled above the store, and the `sh'
       the target hides in the `beqz' delay slot becomes a `nop' after the
       load instead.  func_s18_80024FF0 in this unit prices the same
       dependence at 6 on three argument loads.
     - HOLDS: `k' is `s32'.  As `s16' it is 289 words / 8 differences -- a
       HImode pseudo makes the load `lhu' and adds the `sll 16'/`sra 16' the
       switch then needs to widen it.
     - HOLDS: `n->unk_0C' is ONE local assigned TWICE, not two locals.  Two
       separate locals is 287 words / 34 aligned differences, a uniform
       whole-block shift in which the slot pointer takes $v1 instead of $a3
       and every pseudo below it moves one register.  The TELL is in the
       listing before any compile: the target loads it twice into the SAME
       register, and only a multi-SET pseudo does that -- a single-set pseudo
       gets `adjust_priority's birthing boost and wins a different register.
     - HOLDS: each switch arm's own corner halfword goes through a local
       declared INSIDE THAT ARM, which is why the arms carry braces.  One
       function-scope local shared by all five arms is 287 words / 7
       differences, and NO local at all -- the read inline in each arm's last
       store -- is 303 words / 67, because the differing `lhu' then sits
       between the arms' otherwise identical six-store tails and jump2 cannot
       cross-join them: 16 words of duplicated tail across four arms.  Five
       per-arm locals keep the pseudo BLOCK-LOCAL so local-alloc hands each
       one $v1; one shared local is a global allocno and gets $a0.
   MEASURED AND FREE, so neither is the reason this lands: the per-arm local
   declared `s32' rather than `u16' (287 / 0 either way), and moving its
   declaration to the head of the block.  Both are FREE and not merely
   unmeasurable -- the source trees differ and the output does not.
   The 0x18 constant in those tails stays a literal inside each arm; it is
   NOT cross-joined, and each arm keeps its own `li'.

   DECLARATION this body added: `UnkDngS18Elem', a 0xC-byte 1-aligned block
   type.  The lwl/lwr plus swl/swr triple at q+0x44 is an ordinary struct
   assignment of a byte-aligned type, the same idiom `Unk3E39C' records in
   structs.h; it is not a hand-rolled copy.
*/
void func_s18_800257E0(u8 *p, UnkVec *v)
{
    UnkNode *n;
    UnkSlot *s;
    UnkVec *m;
    u8 *q;
    s32 i;
    s32 t;
    s32 k;

    k = *(s16 *)(p + 0x0);
    D_8002694C = 1;
    switch (k)
    {
    case 0:
        for (i = 0; i < 5; i++)
        {
            t = (i << 12) / 5;
            *(s16 *)(p + 0x1E + i * 2) = *(s16 *)(p + 0xA) * func_80064584(*(s16 *)(p + 0xC) + t) >> 12;
            *(s16 *)(p + 0x2A + i * 2) = *(s16 *)(p + 0xA) * func_800644B8(*(s16 *)(p + 0xC) + t) >> 12;
        }
        *(s16 *)(p + 0x1C) = 0;
        *(s16 *)(p + 0x2) = 0;
        *(u16 *)(p + 0x0) += 1;
        /* fall through */
    case 1:
        if (*(s16 *)(p + 0x2) == 0)
        {
            *(s16 *)(p + 0x2) = *(u16 *)(p + 0x2) + 1;
            n = func_8003FC64(0x212);
            if (n != NULL)
            {
                q = (u8 *)n + 0x20;
                *(s16 *)(q + 0x2) = 0x37 - *(s16 *)(p + 0x1C) * 4;
                n->unk_10 = (u32)func_s18_80025380;
                func_8004491C(n, (u32)func_s18_800249A0);
                s = (UnkSlot *)n->unk_0C;
                s->unk_14 &= 0xFFF3;
                m = n->unk_08;
                *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)v + 0x2);
                *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)v + 0x6);
                *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)v + 0xA);
                switch (*(s16 *)(p + 0x1C))
                {
                case 0:
                {
                    u16 w;

                    *(u16 *)(q + 0x58) = *(u16 *)(q + 0x5C) = *(u16 *)(p + 0x1E);
                    *(u16 *)(q + 0x5A) = *(u16 *)(q + 0x5E) = *(u16 *)(p + 0x22);
                    *(u16 *)(q + 0x60) = *(u16 *)(q + 0x64) = *(u16 *)(p + 0x2A);
                    w = *(u16 *)(p + 0x2E);
                    *(u16 *)(q + 0x68) = *(u16 *)(q + 0x6A) = 0;
                    *(u16 *)(q + 0x6C) = *(u16 *)(q + 0x6E) = 0x18;
                    *(u16 *)(q + 0x62) = *(u16 *)(q + 0x66) = w;
                    break;
                }
                case 1:
                {
                    u16 w;

                    *(u16 *)(q + 0x58) = *(u16 *)(q + 0x5C) = *(u16 *)(p + 0x22);
                    *(u16 *)(q + 0x5A) = *(u16 *)(q + 0x5E) = *(u16 *)(p + 0x26);
                    *(u16 *)(q + 0x60) = *(u16 *)(q + 0x64) = *(u16 *)(p + 0x2E);
                    w = *(u16 *)(p + 0x32);
                    *(u16 *)(q + 0x68) = *(u16 *)(q + 0x6A) = 0;
                    *(u16 *)(q + 0x6C) = *(u16 *)(q + 0x6E) = 0x18;
                    *(u16 *)(q + 0x62) = *(u16 *)(q + 0x66) = w;
                    break;
                }
                case 2:
                {
                    u16 w;

                    *(u16 *)(q + 0x58) = *(u16 *)(q + 0x5C) = *(u16 *)(p + 0x26);
                    *(u16 *)(q + 0x5A) = *(u16 *)(q + 0x5E) = *(u16 *)(p + 0x20);
                    *(u16 *)(q + 0x60) = *(u16 *)(q + 0x64) = *(u16 *)(p + 0x32);
                    w = *(u16 *)(p + 0x2C);
                    *(u16 *)(q + 0x68) = *(u16 *)(q + 0x6A) = 0;
                    *(u16 *)(q + 0x6C) = *(u16 *)(q + 0x6E) = 0x18;
                    *(u16 *)(q + 0x62) = *(u16 *)(q + 0x66) = w;
                    break;
                }
                case 3:
                {
                    u16 w;

                    *(u16 *)(q + 0x58) = *(u16 *)(q + 0x5C) = *(u16 *)(p + 0x20);
                    *(u16 *)(q + 0x5A) = *(u16 *)(q + 0x5E) = *(u16 *)(p + 0x24);
                    *(u16 *)(q + 0x60) = *(u16 *)(q + 0x64) = *(u16 *)(p + 0x2C);
                    w = *(u16 *)(p + 0x30);
                    *(u16 *)(q + 0x68) = *(u16 *)(q + 0x6A) = 0;
                    *(u16 *)(q + 0x6C) = *(u16 *)(q + 0x6E) = 0x18;
                    *(u16 *)(q + 0x62) = *(u16 *)(q + 0x66) = w;
                    break;
                }
                case 4:
                {
                    u16 w;

                    *(u16 *)(q + 0x58) = *(u16 *)(q + 0x5C) = *(u16 *)(p + 0x24);
                    *(u16 *)(q + 0x5A) = *(u16 *)(q + 0x5E) = *(u16 *)(p + 0x1E);
                    *(u16 *)(q + 0x60) = *(u16 *)(q + 0x64) = *(u16 *)(p + 0x30);
                    w = *(u16 *)(p + 0x2A);
                    *(u16 *)(q + 0x68) = *(u16 *)(q + 0x6A) = 0;
                    *(u16 *)(q + 0x6C) = *(u16 *)(q + 0x6E) = 0x18;
                    *(u16 *)(q + 0x62) = *(u16 *)(q + 0x66) = w;
                    break;
                }
                }
                *(u16 *)(q + 0x1E) = *(u16 *)(q + 0x5A);
                *(u16 *)(q + 0x2A) = *(u16 *)(q + 0x62);
                *(s16 *)(q + 0x20) = (*(s16 *)(q + 0x1E) + *(s16 *)(q + 0x58)) / 2;
                *(s16 *)(q + 0x20) = (*(s16 *)(q + 0x20) + *(s16 *)(q + 0x58)) / 2;
                *(s16 *)(q + 0x2C) = (*(s16 *)(q + 0x2A) + *(s16 *)(q + 0x60)) / 2;
                *(s16 *)(q + 0x2C) = (*(s16 *)(q + 0x2C) + *(s16 *)(q + 0x60)) / 2;
                *(u16 *)(q + 0x5A) = *(u16 *)(q + 0x5E) = *(u16 *)(q + 0x20);
                *(u16 *)(q + 0x62) = *(u16 *)(q + 0x66) = *(u16 *)(q + 0x2C);
                s = (UnkSlot *)n->unk_0C;
                s->unk_0C[0] = s->unk_0C[1] = 0xE0;
                s->unk_0C[2] = 0x20;
                q[0x36] = q[0x37] = 0xE0;
                q[0x38] = 0x20;
                s->unk_1C = s->unk_1E = 0x1000;
                *(UnkDngS18Elem *)(q + 0x44) = D_80026940;
                s->unk_08 = (u32)(q + 0x44);
            }
        }
        else
        {
            *(s16 *)(p + 0x2) = *(u16 *)(p + 0x2) + 1;
            if (*(s16 *)(p + 0x2) >= 4)
            {
                *(s16 *)(p + 0x2) = 0;
                *(u16 *)(p + 0x1C) += 1;
                if (*(s16 *)(p + 0x1C) >= 5)
                {
                    *(u16 *)(p - 0x2) |= 0x8000;
                    D_800814A0 |= 0x8000;
                }
            }
        }
        break;
    }
}

const u8 D_s18_80024074[] = {
    0x20,
    0x00,
    0x00,
    0x00,
    0x20,
    0x00,
    0x20,
    0x00,
    0x00,
    0x00,
    0x20,
    0x00,
    0xE0,
    0xFF,
    0x20,
    0x00,
    0xE0,
    0xFF,
    0x00,
    0x00,
    0xE0,
    0xFF,
    0xE0,
    0xFF,
    0x00,
    0x00,
    0xE0,
    0xFF,
    0x20,
    0x00,
    0xE0,
    0xFF,
};


/* The eight-state driver for the caster's thrown node.  `p' is this node's
   payload, `o = *(u8 **)p' the caster's payload, `n = o - 0x20' its node and
   `m = n->unk_08' its mover; `tbl' is the eight compass offsets at +-0x20,
   copied off this bank's own D_s18_80024074 rodata.  State 0 paints the
   renderable 0x808080 at scale 0x1000, parks the 0xC-byte D_s18_80026934
   template at p+0x98 as the primitive, picks the facing out of the caster's
   +0x2A and FALLS THROUGH into 1; state 1 reads the caster's hand through
   func_8003DF74, mirrors its position into the mover (less 0x40 when the slot
   is flagged), tags the node with D_80045340, and takes the flight either
   from the caster's +0x60 target (height off the target's mover, step count
   from the two tile corners through D_8006CCD8/D_8006CCE8) or, with no
   target, from the facing's row of `tbl'; state 2 sprays a 0x12 spark a
   frame, integrates the mover and on arrival goes to 3, 7 or the
   func_800A56E0 landing; 3 flips the +0x96 blink, spawns the 0x12 ring and
   goes to 4; 4 fades the renderable to black over 0x14 frames while spraying
   four func_s18_800250B0 motes a frame; 5 and 6 drive the two-per-frame
   func_s18_800251E8 sphere bursts over 0x28 and 0x46 frames, 6 also pushing
   the caster's own +0x1C flag and tinting its renderable; 7 waits 0x15 frames
   and then either consumes the bank's D_8002694C tick or kills the node.
   822 words, 0 differing.

   DONOR: csrc/ovl_dng_s38.c's LANDED func_s38_80024FA4, 627 words, which
   dngtwin cannot see at any floor (it reports no carved sibling at 0.70 for
   every stub in this unit).  dngcallee named it ALONE at 12 of 16 symbols
   with a clean gap to the 11/16 tier, and it is the same family: same
   `(u8 *p, UnkVec *v, u8 *c)' signature, same `o'/`n'/`m' head with the
   0x20-byte table copy BEFORE them, same case-0 renderable opening, the same
   func_8003DF74 hand read with the same `u16 out[3]' stack local, the same
   D_80045340 tag block, the same `D_8006CCD8[facing]'/`D_8006CCE8[facing]'
   corner pair and the same `__builtin_abs(... ? ... : ...) * 2 - 1' step
   count.  Its frame is -0x70 with six callee-saved registers where this is
   -0x68 with seven, so its BUDGET facts were not expected to carry and none
   was needed.  First compile here was 822 words / 49 aligned differences.

   OWNS jtbl_s18_80024098.  8 cases, dispatch bound 8, all entries distinct,
   no pad in either the ROM or the standalone compile.  Entry-address order is
   ascending case value 0..7, confirmed by standalone compile: the emitted
   entry sequence resolves to the ROM's exactly.  There is no `default:' --
   the range-check `beqz' goes to the function exit.

   FOUR FACTS, MEASURED ON THIS BODY AFTER IT REACHED 0:
     - HOLDS, and it is the largest: the pointer locals are SPLIT per block,
       not shared.  Case 1's `e' and case 6's `y' are different locals even
       though both are `*(u8 **)(X - 0x14)'; merging them into one is 822
       words / 65 aligned differences.  The listing says so before any
       compile: the target gives case 1's pointer $a0 and case 6's $a1, and
       ONE pseudo cannot hold two hard registers -- local-alloc only sees a
       pseudo referenced in a single basic block, so a pointer used in two
       cases becomes a global allocno and gets one register for both.  The
       converse holds in the same body: `u' IS one local across cases 1, 2, 5
       and 6 and the target gives it $s0 at all four sites, which is what a
       global allocno looks like.  Read the register, not the role.
     - HOLDS: case 7's write-back of the pre-increment counter is the ARRAY
       REF `((s16 *)p)[0x41] = j', not `*(s16 *)(p + 0x82) = j'.  As the plain
       cast it is 823 words / 16 differences.  The array ref sets
       MEM_IN_STRUCT_P, so `true_dependence' excuses the varying store against
       the FIXED-address `lh %lo(D_8002694C)' beside it and the flag load
       hoists ABOVE it into the branch delay slot; as a plain cast neither
       memref is in-struct, the hoist is illegal and a `nop' comes back.  This
       is the donor's own mechanism (it prices the same sentence at 7) with
       the polarity reversed -- there the array ref let a LOAD hoist over the
       flag store, here it lets the flag load hoist over the array STORE.
     - HOLDS: case 6's three colour bumps are written ASCENDING, 0xC then 0xD
       then 0xE.  Written 0xC/0xE/0xD -- the order the listing appears to
       show -- it is 823 words / 18 differences, because sched transposes the
       last two independent stores and the cross-joined arm tail then ends on
       the wrong one.  A store order read off a cross-joined switch arm is
       the SCHEDULER's, not the source's.
     - DEVICE, see the marker at the site: `*(volatile s8 *)(p + 0x7B)' in
       case 1's no-target arm.  Without it the body is 821 words / 6.

   DECLARATIONS this body added: `UnkDngS18Blk32' (the 0x20-byte 1-aligned
   block for the D_s18_80024074 copy) and `UnkDngS18Rec' (`s16 a; u16 b'),
   both the donor's `UnkDngS58Blk32'/`UnkDngS58Rec' under this unit's name and
   with the same layout.  `UnkDngS18Rec's SIGNEDNESS is witnessed twice and
   the two witnesses disagree only because of convert.c: the common tail
   stores `tbl[k].a << 16' into an s32 and reads it `lh' (so `a' is signed),
   while case 1's no-target arm reads the same field `lhu' because the sum it
   joins narrows to `unsigned short' and convert.c takes its unsignedness
   from the first operand.  Do not change the field's type to match the
   narrowing site.
   Also added: `void func_s18_800240B8_3(u8 *, u8, u8 *)
   __asm__("func_s18_800240B8")'.  Case 6 calls that function with THREE
   arguments where its landed definition in this unit takes two; the third
   is `o' in $a2, dead after the call and computed only for it.  The
   unit-local alias leaves the landed definition and its own 60-word body
   alone -- NO prototype anywhere is changed by this function.
*/
void func_s18_80025C5C(u8 *p, UnkVec *v, u8 *c)
{
    u16 out[3];
    UnkDngS18Rec tbl[8];
    UnkNode *n;
    UnkNode *n2;
    UnkSlot *r;
    UnkVec *m2;
    u8 *o;
    u8 *m;
    u8 *e;
    u8 *t;
    u8 *h;
    u8 *y;
    u8 *u;
    u8 *q;
    s32 i;
    s32 z;
    s16 j;

    o = *(u8 **)p;
    *(UnkDngS18Blk32 *)tbl = *(const UnkDngS18Blk32 *)D_s18_80024074;
    n = (UnkNode *)(o - 0x20);
    m = (u8 *)n->unk_08;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)(c + 0xC) = 0x808080;
        *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = 0x1000;
        *(UnkDngS18Elem *)(p + 0x98) = D_s18_80026934;
        *(u32 *)(c + 0x8) = (u32)(p + 0x98);
        D_8002694C = 1;
        *(s16 *)(p + 0x7E) = (((u16 *)o)[0x15] >> 9) & 0x7;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fall through */
    case 1:
        if (func_8003DF74((u8 *)((UnkSlot *)n->unk_0C)->unk_08, (UnkSlot *)n->unk_0C, (s16 *)out, 0) ==
            NULL)
        {
            if ((((UnkSlot *)n->unk_0C)->unk_14 & 0x8000) == 0)
            {
                break;
            }
        }
        *(s16 *)((u8 *)v + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)v + 0x6) = *(u16 *)(m + 0x6);
        z = *(u16 *)(m + 0xA);
        *(s16 *)((u8 *)v + 0xA) = z;
        if ((((UnkSlot *)n->unk_0C)->unk_14 & 0x8000) == 0)
        {
            *(s16 *)((u8 *)v + 0x2) = *(u16 *)((u8 *)v + 0x2) + out[0];
            *(s16 *)((u8 *)v + 0x6) = *(u16 *)((u8 *)v + 0x6) + out[1];
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)v + 0xA) + out[2];
        }
        else
        {
            *(s16 *)((u8 *)v + 0xA) = z - 0x40;
        }
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        if ((p[0x7A] & 0x4) == 0)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
            *(s16 *)(c + 0x10) = 0x20;
            c[0xC] = c[0xD] = 0x80;
            c[0xE] = 0x20;
            *(u16 *)(c + 0x14) |= 0xC;
            p[0x7A] |= 0x4;
        }
        if (*(u8 **)(o + 0x60) != NULL)
        {
            u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
            *(s16 *)(p + 0x74) = *(u16 *)(u + 0x2);
            *(s16 *)(p + 0x76) = *(u16 *)(u + 0x6);
            *(s16 *)(p + 0x78) = *(u16 *)(u + 0xA) - (D_800DDC40[(*(u8 **)(o + 0x60))[0x13]] + 0x20);
            e = *(u8 **)(o - 0x14);
            p[0xA4] = e[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x7E)];
            p[0xA5] = e[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x7E)];
            p[0x7B] = __builtin_abs(*(s8 *)(o + 0x72) == e[0x24] ? *(s8 *)(o + 0x73) - e[0x25]
                                                                 : *(s8 *)(o + 0x72) - e[0x24]) *
                          2 -
                      1;
        }
        else
        {
            p[0x7B] = 0x10;
            *(s16 *)(p + 0x74) = *(u16 *)((u8 *)v + 0x2) + (tbl[*(s16 *)(p + 0x7E)].a << 4);
            /* DEVICE: the `volatile' is the only spelling found that keeps the target's
               THREE-insn byte sign-extension here -- `lbu 0x7B($s2)' plus `sll 24'/`sra 24'
               on the register -- instead of a single `lb'.  Worth 1 word and 6 aligned
               differences: without it this body is 821 words / 6 rows, and the 6 are the
               missing `lbu' plus the three branches whose exit label the missing word moves.
               MECHANISM, read out of the compiler and not inferred: mips.md's `extendqisi2'
               and `extendqihi2' expanders both do `if (optimize && GET_CODE (operands[1]) ==
               MEM) operands[1] = force_not_mem (operands[1]);' (config/mips/mips.md:2621 and
               :2589 in scratch/gcc-src/cdk-gcc-b18), so EVERY sign-extending byte load is
               first expanded as a plain `lbu' into a register plus the two shifts -- which is
               exactly what the ROM has.  cse then rewrites the shifted subreg back to the
               memref and combine recognises `(sign_extend:SI (mem:QI))' as `lb', collapsing
               three insns into one.  A volatile MEM is the one thing found that stops the
               second half: combine calls `init_recog_no_volatile' (combine.c:502) so
               `volatile_ok' is 0 and it cannot re-recognise the memref.
               SPELLINGS RULED OUT, all 821 words / 6 aligned differences, i.e. all `lb':
               `*(s8 *)(p + 0x7B)'; `(s8)p[0x7B]'; `((p[0x7B] << 24) >> 24)'; an `s32' local
               assigned the byte and then cast; the same local assigned TWICE so that
               combine's `REG_N_SETS == 1' guard would fail (cse folds it before combine is
               reached); an `s16' local, i.e. the `extendqihi2' path.  Also ruled out, worse:
               the multiply's operands exchanged, 821 / 10; the byte loaded into a local
               before an interposed `p + 0x78' store so cse loses the equivalence, 821 / 25.
               It does NOT warn under -Wall.  Call it a device, not macro residue: a payload
               step count is not a volatile object and no macro expands to this. */
            *(s16 *)(p + 0x76) =
                *(u16 *)((u8 *)v + 0x6) + tbl[*(s16 *)(p + 0x7E)].b * *(volatile s8 *)(p + 0x7B);
            *(s16 *)(p + 0x78) = *(u16 *)(o + 0x88) - 0x50;
        }
        v->unk_0C = tbl[*(s16 *)(p + 0x7E)].a << 16;
        v->unk_10 = tbl[*(s16 *)(p + 0x7E)].b << 16;
        v->unk_14 = ((*(s16 *)(p + 0x78) << 16) - v->unk_08) / *(s8 *)(p + 0x7B);
        *(s16 *)(p + 0x82) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 2:
        for (;;)
        {
            n2 = func_8003FC64(0x12);
            if (n2 != NULL)
            {
                q = (u8 *)n2 + 0x20;
                *(s16 *)(q + 0x2) = 8;
                *(s16 *)(q + 0x4) = 8;
                n2->unk_10 = (u32)func_s18_8002569C;
                func_8004491C(n2, (u32)D_80045340);
                r = (UnkSlot *)n2->unk_0C;
                r->unk_10 = 0x20;
                r->unk_14 |= 0xC;
                m2 = n2->unk_08;
                *(s16 *)((u8 *)m2 + 0x2) = *(u16 *)((u8 *)v + 0x2);
                *(s16 *)((u8 *)m2 + 0x6) = *(u16 *)((u8 *)v + 0x6);
                *(s16 *)((u8 *)m2 + 0xA) = *(u16 *)((u8 *)v + 0xA);
                r = (UnkSlot *)n2->unk_0C;
                r->unk_1C = r->unk_1E = 0x1000;
                r->unk_0C[0] = r->unk_0C[1] = 0x60;
                r->unk_0C[2] = 0x10;
                q[0x38] = 0x10;
                q[0x36] = q[0x37] = 0x60;
                *(UnkDngS18Elem *)((u8 *)n2 + 0x64) = D_s18_80026934;
                r->unk_08 = (u32)((u8 *)n2 + 0x64);
            }
            if ((s16)func_800A4778(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                                   *(s16 *)((u8 *)v + 0xA), *(s32 *)(o + 0x60)) != 0)
            {
                *(s16 *)(p + 0xA) = 7;
                *(s16 *)(p + 0x82) = 0;
                *(u16 *)(c + 0x14) |= 0x80;
                return;
            }
            p[0x7B] = p[0x7B] - 1;
            if (*(s8 *)(p + 0x7B) <= 0)
            {
                if (*(u8 **)(o + 0x60) != NULL)
                {
                    *(s16 *)(p + 0xA) = 3;
                    *(s16 *)(p + 0x82) = 0;
                    u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                    *(s16 *)((u8 *)v + 0x2) = *(u16 *)(u + 0x2);
                    *(s16 *)((u8 *)v + 0x6) = *(u16 *)(u + 0x6);
                    *(s16 *)((u8 *)v + 0xA) = *(u16 *)(p + 0x78);
                    func_800A56E0(0x300);
                    return;
                }
                *(s16 *)(p + 0xA) = 7;
                *(s16 *)(p + 0x82) = 0;
                c[0xE] = 0;
                c[0xD] = 0;
                c[0xC] = 0;
                *(u16 *)(c + 0x14) |= 0x80;
                return;
            }
            v->unk_0C = ((*(s16 *)(p + 0x74) << 16) - v->unk_00) / *(s8 *)(p + 0x7B);
            v->unk_10 = ((*(s16 *)(p + 0x76) << 16) - v->unk_04) / *(s8 *)(p + 0x7B);
            v->unk_14 = ((*(s16 *)(p + 0x78) << 16) - v->unk_08) / *(s8 *)(p + 0x7B);
            v->unk_00 += v->unk_0C;
            v->unk_04 += v->unk_10;
            v->unk_08 += v->unk_14;
        }
    case 3:
        if (*(s16 *)(p + 0x96) == 0)
        {
            *(s16 *)(p + 0x96) = 1;
            p[0xA0] += 0x18;
        }
        else
        {
            *(s16 *)(p + 0x96) = 0;
            p[0xA0] -= 0x18;
        }
        n2 = func_8003FC64(0x12);
        if (n2 != NULL)
        {
            q = (u8 *)n2 + 0x20;
            *(s16 *)(q + 0x2) = 8;
            *(s16 *)(q + 0x4) = 8;
            *(s16 *)(q + 0xA) = 0x30;
            *(s16 *)(q + 0xC) = -0xDE;
            n2->unk_10 = (u32)func_s18_800257E0;
            r = (UnkSlot *)n2->unk_0C;
            r->unk_10 = 0x20;
            r->unk_14 |= 0xC;
            m2 = n2->unk_08;
            *(s16 *)((u8 *)m2 + 0x2) = *(u16 *)((u8 *)v + 0x2);
            *(s16 *)((u8 *)m2 + 0x6) = *(u16 *)((u8 *)v + 0x6);
            *(s16 *)((u8 *)m2 + 0xA) = *(u16 *)((u8 *)v + 0xA);
            *(UnkDngS18Elem *)((u8 *)n2 + 0x64) = D_80026940;
            r->unk_08 = (u32)((u8 *)n2 + 0x64);
        }
        p[0x92] = 0;
        p[0x91] = 0;
        p[0x90] = 0;
        *(s16 *)(p + 0xA) = 4;
        *(s16 *)(p + 0x82) = 0;
        break;
    case 4:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) < 0x14)
        {
            c[0xC] = (0x14 - *(s16 *)(p + 0x82)) * 0xE0 / 0x14;
            c[0xD] = (0x14 - *(s16 *)(p + 0x82)) * 0xE0 / 0x14;
            c[0xE] = (0x14 - *(s16 *)(p + 0x82)) * 0x20 / 0x14;
            if (*(s16 *)(p + 0x96) == 0)
            {
                *(s16 *)(p + 0x96) = 1;
                p[0xA0] += 0x18;
            }
            else
            {
                *(s16 *)(p + 0x96) = 0;
                p[0xA0] -= 0x18;
            }
            for (i = 0; i < 4; i++)
            {
                func_s18_800250B0((UnkNode *)(p - 0x20), *(s16 *)(p + 0x7E), 0x20E0E0,
                                  (rand() & 0xFF) | 0x80, 0, 0, 0);
            }
        }
        else
        {
            *(s16 *)(p + 0x82) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        p[0x92] = *(s16 *)(p + 0x82) * 0x20 / 0x28;
        p[0x91] = *(s16 *)(p + 0x82) * 0xE0 / 0x28;
        p[0x90] = *(s16 *)(p + 0x82) * 0xE0 / 0x28;
        u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
        for (i = 0; i < 2; i++)
        {
            func_s18_800251E8((D_800DDC40[(*(u8 **)(o + 0x60))[0x13]] >> 1) + 4,
                              (p[0x92] << 16) + (p[0x91] << 8) | p[0x90], 0x80, *(s16 *)(u + 0x2),
                              *(s16 *)(u + 0x6),
                              *(u16 *)(u + 0xA) - (D_800DDC40[(*(u8 **)(o + 0x60))[0x13]] >> 1));
        }
        if (*(s16 *)(p + 0x82) >= 0x28)
        {
            *(s16 *)(p + 0x82) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 6:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) >= 0x24)
        {
            p[0x92] = (0x46 - *(s16 *)(p + 0x82)) * 0x20 / 0x23;
            p[0x91] = (0x46 - *(s16 *)(p + 0x82)) * 0xE0 / 0x23;
            p[0x90] = (0x46 - *(s16 *)(p + 0x82)) * 0xE0 / 0x23;
        }
        u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
        for (i = 0; i < 2; i++)
        {
            func_s18_800251E8((D_800DDC40[(*(u8 **)(o + 0x60))[0x13]] >> 1) + 4,
                              (p[0x92] << 16) + (p[0x91] << 8) | p[0x90], 0x80, *(s16 *)(u + 0x2),
                              *(s16 *)(u + 0x6),
                              *(u16 *)(u + 0xA) - (D_800DDC40[(*(u8 **)(o + 0x60))[0x13]] >> 1));
        }
        t = *(u8 **)(o + 0x60);
        *(u32 *)(t + 0x1C) |= 0x10000000;
        y = *(u8 **)(t - 0x14);
        if (*(s16 *)(p + 0x82) >= 0x24)
        {
            y[0xC] -= 3;
            y[0xD] -= 3;
            y[0xE] += 2;
        }
        else
        {
            y[0xC] += 3;
            y[0xD] += 3;
            y[0xE] -= 2;
        }
        if (*(s16 *)(p + 0x82) >= 0x46)
        {
            h = *(u8 **)(o + 0x60);
            *(u32 *)(h + 0x1C) &= 0xEFFFFFFF;
            y = *(u8 **)(h - 0x14);
            y[0xE] = 0x80;
            y[0xD] = 0x80;
            y[0xC] = 0x80;
            func_s18_800240B8_3(*(u8 **)(o + 0x60), p[0x9], o);
            *(s16 *)(p + 0x82) = 0x14;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 7:
        j = *(u16 *)(p + 0x82);
        *(s16 *)(p + 0x82) = j + 1;
        if (j >= 0x15)
        {
            ((s16 *)p)[0x41] = j;
            if (D_8002694C == 0)
            {
                D_8008346C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            else
            {
                D_8002694C = 0;
            }
        }
        break;
    }
}
