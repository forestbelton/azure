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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s18", func_s18_800251E8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s18", func_s18_80025380);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s18", func_s18_8002569C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s18", func_s18_800257E0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s18", func_s18_80025C5C);
