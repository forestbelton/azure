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
void func_s17_80024188(u8 *p, UnkVec *v, UnkSlot *s, s16 d);

void func_s17_80024758(u8 *p, UnkVec *v, UnkSlot *s, s16 d);

UnkNode *func_8003FD64(s32, void *);
extern s16 D_80026664;
s32 func_800BCB04(s32, s32, s16);
extern u8 D_800E1C8A[];
extern u8 D_s17_80024004[];
extern u8 D_s17_80024034[];
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
   print the resident D_800E1C8A message if the status took, or assemble the
   three-part bank-local message D_s17_80024004 / name / D_s17_80024034 if it
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

   D_s17_80024004 and D_s17_80024034 ARE EXTERNS, NOT DEFINITIONS: they sit in
   this object's `data' subsegment, 0x1894800..0x1894838, below the `.rodata'
   one that owns D_s17_80024058. */
s32 rand(void);
extern u8 D_80026658[];

void func_s17_80024098(u8 *p, u8 d)
{
    void *m;
    void *h;

    if (func_8009D218_2(p, 2) == 0)
    {
        if ((s16)func_800A48F0_i(p, 0x13, (s8)(func_800A6870(d) + 4)) != 0)
        {
            if ((*(s32 *)(p + 0x14) & 0x4000) != 0)
            {
                func_80099844(p, D_800E1C8A);
            }
        }
        else if ((*(s32 *)(p + 0x14) & 0x4000) != 0)
        {
            h = func_800990FC();
            m = h;
            h = func_80099194(D_s17_80024004, h);
            h = func_80099734(p, h);
            h = func_80099194(D_s17_80024034, h);
            func_80099290(h);
            func_800A5720(m);
        }
    }
}

/* One semi-transparent textured billboard quad for the slot's element record:
   compose the slot's rotation with the camera matrix in the 0x1F800000
   scratchpad, RotAverage4 the four SVECTORs taken off the payload's
   +0x50..+0x67 into the POLY_FT4 at the +0x8D0 cursor, and -- when the OT
   index is in range and at least one corner is on screen -- build its
   UV/CLUT/TPAGE words out of the slot's own +0x10/+0x12 overrides and hang
   the quad on the OT row. 0x28 bytes of cursor, which is what a POLY_FT4
   costs. When the element's +0x00 carries 0x20 instead, the whole draw is
   skipped and the handler word at e+0x8 is called with (p, v, s, e); either
   way the matrix stack is popped on the way out.

   The twin is the LANDED func_b14_80173B4C in csrc/ovl_dng_b14.c (346 words,
   the POLY_GT4 version) -- same signature, same UnkDng74334SP scratchpad,
   same PushMatrix/RotMatrix/ReadRotMatrix/CompMatrix/ScaleMatrix/
   SetTransMatrix/SetRotMatrix chain, same `- d - 6', same four on-screen
   tests, same clut/tpage conditionals and the same s->unk_0C[3] flag byte.
   Landed on the FIRST COMPILE, so all four of the twin's measured facts were
   taken as given and are NOT re-measured here: the scale vector is ONE
   CHAINED assignment (which is what emits +0x3C, +0x38, +0x34 in that
   order), the four on-screen tests are `|' and not `||', `&m' does NOT go
   through a pointer local, and `e = (u8 *)s->unk_08;' comes BEFORE
   `sp->unk_28 = s->unk_14;'.

   WHAT IS THIS COPY'S OWN, read off this listing: the primitive is a POLY_FT4,
   so the cursor steps 0x28, the four vertices are at stride 8 rather than 0xC
   (+0x8/+0x10/+0x18/+0x20), the UV pairs are +0xC/+0x14/+0x1C/+0x24 with clut
   at +0xE and tpage at +0x16, the x correction decrements +0x14/+0x24 and the
   y correction +0x1D/+0x25, and the tag is `setPolyFT4' -- the `sb 9, 0x3' /
   `sb 0x2C, 0x7' pair, written at the TOP of the on-screen block where the
   twin calls setPolyGT4 near its end.

   The colour is ONE WORD, not the twin's four: `*(s32 *)(q + 0x4) =
   *(s32 *)s->unk_0C' writes r0/g0/b0 AND code together, which is why the
   0x2C setPolyFT4 just stored is overwritten by s->unk_0C[3] and why the
   twin's separate `q[0x7] |= s->unk_0C[3]' has no counterpart. Reading the
   payload's +0x50..+0x67 as three contiguous runs of four halfwords -- all
   four vx, then all four vy, then all four vz -- is this copy's layout too;
   the twin reads interleaved triples at stride 6.

   The cursor read and its 0x28 bump sit BEFORE `s->unk_14 |= 0x8000' and
   before PushMatrix here, where the twin takes them after the e[8..0xB]
   stores. 351 words, 0 differing. */
void func_s17_80024188(u8 *p, UnkVec *v, UnkSlot *s, s16 d)
{
    UnkDng74334SP *sp = (UnkDng74334SP *)0x1F800000;
    Unk80083160 *g;
    MATRIX m;
    u8 *q;
    u8 *e;
    u8 f;
    void (*h)(u8 *, UnkVec *, UnkSlot *, u8 *);

    g = &D_80083160;
    sp->unk_24 = g->unk_00 + 0xB0;
    sp->unk_74.t[0] = *(s16 *)((u8 *)v + 0x2);
    sp->unk_74.t[1] = *(s16 *)((u8 *)v + 0x6);
    sp->unk_74.t[2] = *(s16 *)((u8 *)v + 0xA);
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q + 0x28;
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
    if ((e[0] & 0x20) == 0)
    {
        sp->unk_0C.w = e[8];
        sp->unk_10.w = e[9];
        sp->unk_14.w = e[10];
        sp->unk_18.w = e[11];
        sp->unk_B0[0].vx = *(u16 *)(p + 0x50);
        sp->unk_B0[1].vx = *(u16 *)(p + 0x52);
        sp->unk_B0[2].vx = *(u16 *)(p + 0x54);
        sp->unk_B0[3].vx = *(u16 *)(p + 0x56);
        sp->unk_B0[0].vy = *(u16 *)(p + 0x58);
        sp->unk_B0[1].vy = *(u16 *)(p + 0x5A);
        sp->unk_B0[2].vy = *(u16 *)(p + 0x5C);
        sp->unk_B0[3].vy = *(u16 *)(p + 0x5E);
        sp->unk_B0[0].vz = *(u16 *)(p + 0x60);
        sp->unk_B0[1].vz = *(u16 *)(p + 0x62);
        sp->unk_B0[2].vz = *(u16 *)(p + 0x64);
        sp->unk_B0[3].vz = *(u16 *)(p + 0x66);
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
    else
    {
        h = *(void (**)(u8 *, UnkVec *, UnkSlot *, u8 *))(e + 0x8);
        if (h != NULL)
        {
            h(p, v, s, e);
        }
    }
    PopMatrix();
}

/* Walk the tag chain from `p` and hand every node to func_s17_80024188 with a
   zero fourth argument.  21 words, 0 differing, first compile.

   The twin is func_b06_80173E08 two listings up in THIS unit -- the same
   walk with a different per-node call -- and csrc/ovl_dungeon_800D50FC.c's
   func_800D67B0 is the resident original, identical down to the
   `s = (UnkSlot *)n->unk_0C;` cast.  Nothing was fought for; the loop is a
   `for (;;)` with the NULL test in the middle, which is what puts the
   `addiu $s0, $a2, 0x20` in the `beqz`'s delay slot. */
s32 func_s17_80024704(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_s17_80024188(p, v, s, 0);
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

/* The sphere-patch draw: for every 0xC-byte element record in the chain at
   s->unk_08, emit one semi-transparent textured quad per cell of a
   p+0xC by p+0xA lat/long grid. Each cell's four corners are a spherical
   patch at radius p+0x68: the longitudes are `i << 12' and `(i + 1) << 12'
   over p+0xA plus the base angle at p+0x10, both wrapped into 0..0x1000, and
   the latitudes are `-p[0xE] * j' and `-p[0xE] * (j + 1)' over p+0xC plus
   0x400. The corners go through the same 0x1F800000 scratchpad as
   func_s17_80024188, into the POLY_FT4 at the +0x8D0 cursor, and the whole
   per-quad tail -- the four on-screen tests, the 0x100 carry corrections, the
   clut/tpage overrides, the +0x14/+0x24 and +0x1D/+0x25 decrements, the
   e[1] semi-transparency flag and the inline addPrim -- is
   func_s17_80024188's verbatim. The record loop ends on a negative e[0].

   THE TWIN IS func_s17_80024188, FIVE LISTINGS UP IN THIS UNIT (351 words,
   landed): same UnkDng74334SP scratchpad, same
   PushMatrix/RotMatrix/ReadRotMatrix/CompMatrix/ScaleMatrix/SetTransMatrix/
   SetRotMatrix chain, same `- d - 6', same four on-screen tests, same
   s->unk_0C[3] flag byte, same `*(s32 *)(q + 0x4) = *(s32 *)s->unk_0C'. Its
   element record is read at the same offsets (0, 1, 4, 6, 8, 9, 10, 11) --
   this body just iterates over an ARRAY of them. Everything from
   `if ((u32)sp->unk_100 < 0x1E0)' down was taken from it unchanged and is NOT
   re-measured here.

   FOUR FACTS MEASURED ON THIS BODY, all from the landed 543/0 state:

   (1) THE RECORD IS ONE POINTER, NOT TWO -- loop.c MAKES THE SECOND ONE.
   The target carries `e' spilled at 0x50($sp), read only for the `lb 0($t0)'
   terminator and its `+0xC', and a SECOND cursor in $fp at `e + 1' serving
   displacements 0, 3, 5, 7, 8, 9, 0xA. That second cursor is loop.c's
   strength reduction of the DEST_ADDR givs, not source: written as one `e'
   with displacements 1, 4, 6, 8..0xB it appears by itself, at e+1, with the
   right displacements. Writing `f = e + 1' in the source as well is
   **550 words / 49 aligned differences** -- gcc then reduces `f + 3' instead
   and the register-resident cursor comes out at e+4 with displacements
   0, 2, 4..7, while `f' is spilled for its single `f[0]' use.

   (2) THE `+ p[0x10]' IS ITS OWN STATEMENT, AFTER ALL FOUR DIVIDES. The two
   longitudes must be `a = (i << 12) / A;' ... `b = ((i + 1) << 12) / A;'
   with `a += C;' and `b += C;' following, because the target emits `b += C'
   in the block AFTER `a''s two wrap tests, which expand cannot do if the add
   is part of `b''s initialiser. Fused into the two initialisers it is
   **544 words / 135 aligned differences**, and the four `div's come out in
   the order a, b, la, lb against the target's a, la, lb, b. This reads as a
   wrap-macro over (value, base): the add and the two wraps are one unit
   applied to `a' and then to `b'.

   (3) `s->unk_14 &= 0x7FFF' COMES BEFORE `SetPolyFT4(q)', WHICH IS THE
   TWIN'S ORDER INVERTED. The twin's `setPolyFT4' is the psyq MACRO -- two
   `sb's, no call -- so its order there is unobservable; here the primitive is
   the REAL function at 0x800666F4 and the `sh $v0, 0x14($t0)' is what fills
   its `jal' delay slot. The twin's order is **544 words / 9 aligned
   differences**: one `nop' in the delay slot plus the eight branches whose
   offsets the extra word moves. reorg cannot steal the store from after a
   call, because the call clobbers the `$t0' the store addresses through.

   (4) THE RADIUS IS RE-READ AT EVERY USE. `*(s32 *)(p + 0x68) >> 8' appears
   six times and the target has six `lw 0x68' / `sra 8' pairs. Hoisted into
   one local it is **540 words / 222 aligned differences** -- THREE WORDS
   SHORT, which is the cheap test for this class: a hoist that shortens the
   body means the target reloads and the repetition is real.

   Two spellings carried from the twin without re-measuring, so do not read
   them as levers here: `D_80083160.unk_00 + 0xB0' direct at the top against
   `g->unk_00' through the pointer local inside the loop (the same mixed
   idiom func_s17_80025028 uses), and the chained
   `sp->unk_34.vx = sp->unk_34.vy = sp->unk_34.vz = 0x2000'. The fourteen
   sin/cos calls are in source order, one per store, with no common
   subexpression held in a local. 543 words, 0 differing, fifth compile. */
void func_s17_80024758(u8 *p, UnkVec *v, UnkSlot *s, s16 d)
{
    UnkDng74334SP *sp = (UnkDng74334SP *)0x1F800000;
    Unk80083160 *g;
    MATRIX m;
    u8 *q;
    u8 *e;
    u8 k;
    s32 i;
    s32 j;
    s32 a;
    s32 b;
    s32 la;
    s32 lb;
    s32 u;
    s32 w;

    g = &D_80083160;
    sp->unk_24 = D_80083160.unk_00 + 0xB0;
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
        for (j = 0; j < *(s16 *)(p + 0xC); j++)
        {
            for (i = 0; i < *(s16 *)(p + 0xA); i++)
            {
                sp->unk_0C.w = e[8];
                sp->unk_10.w = e[9];
                sp->unk_14.w = e[10];
                sp->unk_18.w = e[11];
                q = *(u8 **)(g->unk_00 + 0x8D0);
                *(u32 *)(g->unk_00 + 0x8D0) = (u32)q + 0x28;
                a = (i << 12) / *(s16 *)(p + 0xA);
                la = -*(s16 *)(p + 0xE) * j / *(s16 *)(p + 0xC) + 0x400;
                lb = -*(s16 *)(p + 0xE) * (j + 1) / *(s16 *)(p + 0xC) + 0x400;
                b = ((i + 1) << 12) / *(s16 *)(p + 0xA);
                a += *(s16 *)(p + 0x10);
                if (a > 0x1000)
                {
                    a -= 0x1000;
                }
                if (a < 0)
                {
                    a += 0x1000;
                }
                b += *(s16 *)(p + 0x10);
                if (b > 0x1000)
                {
                    b -= 0x1000;
                }
                if (b < 0)
                {
                    b += 0x1000;
                }
                u = (*(s32 *)(p + 0x68) >> 8) * func_80064584(la) >> 20;
                w = (*(s32 *)(p + 0x68) >> 8) * func_80064584(lb) >> 20;
                sp->unk_B0[0].vx = u * func_800644B8(a) >> 12;
                sp->unk_B0[0].vy = u * func_80064584(a) >> 12;
                sp->unk_B0[0].vz = -((*(s32 *)(p + 0x68) >> 8) * func_800644B8(la) >> 20);
                sp->unk_B0[1].vx = u * func_800644B8(b) >> 12;
                sp->unk_B0[1].vy = u * func_80064584(b) >> 12;
                sp->unk_B0[1].vz = -((*(s32 *)(p + 0x68) >> 8) * func_800644B8(la) >> 20);
                sp->unk_B0[2].vx = w * func_800644B8(a) >> 12;
                sp->unk_B0[2].vy = w * func_80064584(a) >> 12;
                sp->unk_B0[2].vz = -((*(s32 *)(p + 0x68) >> 8) * func_800644B8(lb) >> 20);
                sp->unk_B0[3].vx = w * func_800644B8(b) >> 12;
                sp->unk_B0[3].vy = w * func_80064584(b) >> 12;
                sp->unk_B0[3].vz = -((*(s32 *)(p + 0x68) >> 8) * func_800644B8(lb) >> 20);
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
                        SetPolyFT4((POLY_FT4 *)q);
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
                        k = e[1];
                        s->unk_0C[3] = k;
                        if ((sp->unk_28 & 0x8) != 0)
                        {
                            if ((sp->unk_28 & 0x4) != 0)
                            {
                                s->unk_0C[3] = k | 0x2;
                            }
                            else
                            {
                                s->unk_0C[3] = k & 0xFD;
                            }
                        }
                        *(s32 *)(q + 0x4) = *(s32 *)s->unk_0C;
                        addPrim(sp->unk_24 + sp->unk_100 * 4, q);
                    }
                }
            }
        }
        if (*(s8 *)e < 0)
        {
            break;
        }
        e += 0xC;
    }
    PopMatrix();
}

/* Walk the tag chain from `p` and hand every node to func_s17_80024758 with a
   zero fourth argument.  21 words, 0 differing, first compile.

   The twin is func_b06_80173E08 two listings up in THIS unit -- the same
   walk with a different per-node call -- and csrc/ovl_dungeon_800D50FC.c's
   func_800D67B0 is the resident original, identical down to the
   `s = (UnkSlot *)n->unk_0C;` cast.  Nothing was fought for; the loop is a
   `for (;;)` with the NULL test in the middle, which is what puts the
   `addiu $s0, $a2, 0x20` in the `beqz`'s delay slot. */
s32 func_s17_80024FD4(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_s17_80024758(p, v, s, 0);
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
   148 words, 0 differing. */
s32 func_s17_80025028(u8 *p, UnkVec *v)
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
        sp->unk_C0 = RotTransPers((SVECTOR *)sp, (long *)&t->x0, (long *)&sp->unk_90, (long *)&sp->unk_94);
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            *(u32 *)&t->r0 = *(u32 *)(p + 0x8);
            t->r0 = t->r0 * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
            t->g0 = t->g0 * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
            t->b0 = t->b0 * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
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

   THE THREE ARGUMENT LOADS ARE THREE LOCALS, ASSIGNED BEFORE THE D_80026664
   STORE, IN THE ORDER +0xA, +0x2, +0x6. Written the twin's way, with the three
   loads inside the call, it is 6 differing at the right 48 words, every row
   the `li 1'/`sh' pair and the three `lhu's trading places. The cause is a
   true dependence, read out of `tools/decomp/sched.py --rtl' and not
   inferred: gcc 2.7.2 cannot disambiguate the `mem:HI (lo_sum D_80026664)'
   store from `mem:HI (s0 + 10)', so every argument load carries insn 17 --
   the store -- in its dependency list and can never be scheduled above it.
   calls.c emits a call's argument register loads at the very END of its
   expansion, so the only way to get them above the store is to evaluate them
   in an earlier statement.

   Measured and rejected, all at 48 words: the twin's spelling 6; the call
   inlined into the comparison 6; `*(s16 *)&D_80026664 = 1' 6; hoisting only
   the +0xA load 4; hoisting all three in source order +0x2, +0x6, +0xA 3 --
   the three `lhu's then come out in that order against the target's +0xA
   first, the tie breaking on LUID, which is why the +0xA load is written
   first. `do { D_80026664 = 1; } while (0)' is 49 words and 44 differing.
   The `+ 2' stays inside the argument: its `addiu' sits AFTER the store in
   the target.

   Macro residue rather than a device -- three loads named and then handed to
   one call is what a bearing-test macro leaves behind. 48 words,
   0 differing. */
void func_s17_80025278(u8 *p, UnkVec *v)
{
    s32 x;
    s32 y;
    s32 z;
    s16 r;

    z = *(u16 *)((u8 *)v + 0xA);
    x = *(u16 *)((u8 *)v + 0x2);
    y = *(u16 *)((u8 *)v + 0x6);
    D_80026664 = 1;
    r = func_800BCB04(x, y, z + 2);
    if (*(s16 *)((u8 *)v + 0xA) < r)
    {
        v->unk_08 += *(s32 *)(p + 0x48);
    }
    if ((*(s16 *)(p + 0x32) -= 4) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* A 0x211 spawner: seed the new node's mover from the parent `a''s, jitter each
   axis by a fresh rand() & 0x1F and the caller's own offset less 0x10, then
   park func_s17_80025278 as the step and func_s17_80025028 as the draw, with
   `b' at the payload's +0x14, `d' at BOTH +0x32 and +0x34, a
   rand() + 0x10000 step at +0x48 and `c' at +0x8.

   The twin is the landed func_s16_80024488 in csrc/ovl_dng_s16.c, 72 words to
   this 78: the six extra are the second `sh' of `d' into +0x34 -- which rides
   in func_8004491C's delay slot, so it is written BEFORE that call -- and the
   five-word rand()/+0x10000 store that follows it. Landed on the FIRST
   COMPILE, so the twin's two measured facts were taken as given and are not
   re-measured here: the three stack offsets are `s16' (s32 is 66 words and 71
   differing in the sibling, the three $s2/$s3/$s5 save-restore pairs missing)
   and the subtraction wears its own `(s16)' cast to stop fold associating the
   `addiu -0x10' onto the running sum (74 words, 20 differing there).

   Both installed handlers are this unit's own and both are DEFINED ABOVE this
   point in the file, so no forward declaration was needed. 78 words,
   0 differing. */
void func_s17_80025338(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *q;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s17_80025278;
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x1F) + (s16)(dx - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x1F) + (s16)(dy - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x1F) + (s16)(dz - 0x10);
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x14) = b;
        *(s16 *)(q + 0x32) = d;
        *(s16 *)(q + 0x34) = d;
        func_8004491C(n, (u32)func_s17_80025028);
        *(s32 *)(q + 0x48) = rand() + 0x10000;
        *(u32 *)(q + 0x8) = c;
    }
}

/* The 8-state step of the burst actor: state 0 raises the 0x68/0x6A
   fixed-point height to its 0x800 ceiling in 0x28 steps, 1 arms the fall
   (+0x74 velocity 0, +0x80 gravity -0x900) and runs an 11-frame wait, 2
   integrates the fall while the integer height at +0x6A is still at least
   0x1F and copies it back to +0x1A, 3 waits 0x1F frames, 4 copies the
   12-byte D_80026658 record onto the payload at +0x2C, publishes its address
   at c+0x8, raises the slot flag at +0x88 of the record at +0x40 and spawns
   0x20 jittered 0x211 particles through this unit's own
   func_s17_80025338, 5 waits 0x10 frames, 6 fades the caller's colour triple
   at c[0xC..0xE] by 5 a frame until it is under 6, and 7 runs the +0x2
   countdown down and retires the node. States 0-3 also step the +0x34 angle
   byte by 2 with a 0xE0 wrap. Out-of-range states fall straight to the
   epilogue: the range-check `beqz' targets the function exit, so there is NO
   `default:' arm.

   OWNS jtbl_s17_80024038 -- 8 words, 8 cases + 0 pad, minval 0 (the dispatch
   is a bare `sltiu $a0, 0x8' with no subtraction), 8 distinct targets. The
   ROM's eight arm labels are at STRICTLY ASCENDING addresses
   (800254CC < 80025528 < 8002559C < 800255FC < 8002564C < 80025730 <
   8002575C < 800257B4), so entry-address order IS ascending case order and
   the arms are written 0..7. VERIFIED by standalone compile
   (`tools/cdkgcc -O2 -G0 -S'), because ovlverify cannot see a table at all:
   the emitted table's eight entries sit at STRICTLY ASCENDING block
   addresses in the .s, so the substitution against the ROM's eight strictly
   ascending arm labels is 1:1 and the resolved entry sequence is the ROM's.
   8 words standalone, 8 in the ROM, no pad either side. (gcc's own labels
   are [$L103, $L106, $L109, $L112, $L115, $L121, $L123, $L126] at .s lines
   1936 < 1976 < 2017 < 2052 < 2077 < 2159 < 2169 < 2204 in this file state;
   the numbers move with the rest of the unit, the ORDER does not.)

   TWO MEASURED FACTS, the second being the whole fight:

   (1) THE STATE READ IS ITS OWN STATEMENT, AHEAD OF `D_80026664 = 1'. This is
   the third body in this unit to pay for that dependence (func_s17_80025278
   and func_s17_80025814 are the others): gcc 2.7.2 cannot disambiguate the
   `mem:HI (lo_sum D_80026664)' store from `mem:HI (s3 + 0)', so the switch's
   own load can never be scheduled above the store. Spelled
   `D_80026664 = 1; switch (*(s16 *)p)' it is 234 words / 7 aligned
   differences -- the `lh' sinks below the `sh' and a `nop' fills its
   load-delay slot.

   (2) THE STATE LOCAL IS `s32', NOT `s16'. `s16 st' keeps the value in
   HImode, loads it with `lhu' and sign-extends at the switch:
   235 words / 7 aligned differences, every row the extra `sll 16'/`sra 16'
   pair. `s32 st' takes the sign extension in the load itself, which is the
   target's `lh'.

   The rest came out on the first two compiles and is NOT measured, so do not
   read it as a lever: the 0xC-byte record copy as `memcpy(p + 0x2C,
   D_80026658, 0xC)' (three unaligned word pairs off one source base, the
   same size that csrc/ovl_dng_lo20.c's func_lo20_80028774 prices), the four
   per-state counters as in-place `(*(s16 *)(p + 0x2) += 1) >= K' flag forms,
   the `>= 0xE0' / `< 6' byte tests left unsigned by c-typeck's
   shorten_compare, and the four `rand()' jitters written INLINE in the
   spawn call rather than as locals. 233 words, 0 differing, second
   compile. */
void func_s17_80025470(u8 *p, UnkVec *v, u8 *c)
{
    s32 st;
    s32 i;

    st = *(s16 *)p;
    D_80026664 = 1;
    switch (st)
    {
    case 0:
        *(s16 *)(p + 0x68) = 0;
        *(s16 *)(p + 0x6A) = *(u16 *)(p + 0x1A);
        if ((*(u8 *)(p + 0x34) += 2) >= 0xE0)
        {
            *(u8 *)(p + 0x34) -= 0x20;
        }
        if ((*(s16 *)(p + 0xE) += 0x28) >= 0x800)
        {
            *(s16 *)(p + 0xE) = 0x800;
            *(s16 *)p += 1;
        }
        break;
    case 1:
        if ((*(u8 *)(p + 0x34) += 2) >= 0xE0)
        {
            *(u8 *)(p + 0x34) -= 0x20;
        }
        if ((*(s16 *)(p + 0x2) += 1) >= 0xB)
        {
            *(s16 *)(p + 0x2) = 0;
            *(s16 *)p += 1;
        }
        *(s16 *)(p + 0x68) = 0;
        *(s32 *)(p + 0x74) = 0;
        *(s32 *)(p + 0x80) = -0x900;
        *(s16 *)(p + 0x6A) = *(u16 *)(p + 0x1A);
        break;
    case 2:
        if ((*(u8 *)(p + 0x34) += 2) >= 0xE0)
        {
            *(u8 *)(p + 0x34) -= 0x20;
        }
        if (*(s16 *)(p + 0x6A) >= 0x1F)
        {
            *(s32 *)(p + 0x74) += *(s32 *)(p + 0x80);
            *(s32 *)(p + 0x68) += *(s32 *)(p + 0x74);
            *(s16 *)(p + 0x1A) = *(u16 *)(p + 0x6A);
            break;
        }
        *(s16 *)(p + 0x2) = 0;
        *(s16 *)p += 1;
        break;
    case 3:
        if ((*(u8 *)(p + 0x34) += 2) >= 0xE0)
        {
            *(u8 *)(p + 0x34) -= 0x20;
        }
        if ((*(s16 *)(p + 0x2) += 1) >= 0x1F)
        {
            *(s16 *)(p + 0x2) = 0;
            *(s16 *)p += 1;
        }
        break;
    case 4:
        memcpy(p + 0x2C, D_80026658, 0xC);
        *(u32 *)(c + 0x8) = (u32)(p + 0x2C);
        *(s16 *)(p + 0x2) = 0;
        *(s16 *)p += 1;
        *(s16 *)(*(u8 **)(p + 0x40) + 0x88) = 1;
        for (i = 0; i < 0x20; i++)
        {
            func_s17_80025338((UnkNode *)(p - 0x20), 0, 0xE04040, (rand() & 0xFF) | 0x80,
                              (rand() & 0x7F) - 0x40, (rand() & 0x7F) - 0x40, (rand() & 0x3F) - 0x60);
        }
        break;
    case 5:
        if ((*(s16 *)(p + 0x2) += 1) >= 0x10)
        {
            *(s16 *)(p + 0x2) = 0;
            *(s16 *)p += 1;
        }
        break;
    case 6:
        if (c[0xC] >= 5)
        {
            c[0xC] -= 5;
            c[0xD] -= 5;
            c[0xE] -= 5;
        }
        if (c[0xC] < 6)
        {
            *(s16 *)(p + 0x2) = 0;
            *(s16 *)p += 1;
        }
        break;
    case 7:
        if ((*(s16 *)(p + 0x2) -= 1) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* One frame of the 0x37-state spinner that feeds func_s17_80024188: rebuild
   the payload's four quad corners at +0x50..+0x67 from a 0x10-wide square
   swung round by the +0xC angle at the +0xA radius, re-seed the mover from
   the record at +0x44 and the three colour bytes from the one at +0x48, step
   the angle back by 0x64 a frame with a 0x1000 wrap, and raise the node's
   redraw bit -- which the wrong state, or an exhausted +0xA counter on the
   record at +0x4C, reaches directly.

   No twin: xsibs found none at ratio 0.70 and the callee-set grep over the
   landed side found nothing either. The corner slots are the ones
   func_s17_80024188 reads as sp->unk_B0[0..3], which is how the three groups
   were identified as vx[4], vy[4], vz[4] rather than four corners.

   THE TWELVE CORNER STORES ARE AXIS-MAJOR -- all four vx, then all four vy,
   then all four vz -- AND THAT IS THE WHOLE FIGHT. Every grouping gives the
   same instructions; only the order and the number of surviving `lhu'
   reloads change, because cse folds a reload of a slot whose stored value it
   still knows and loses it once an intervening store invalidates the entry.
   Measured ladder, aligned differences from `tools/decomp/aligndiff.py':

       axis-major vx, vy, vz                  113 words,  0   <- landed
       corner-major vx, vy, vz                113 words,  7
       vy axis first, then corner vx/vz       113 words,  7
       corner-major vy, vx, vz                113 words,  7
       corner-major vx, vz, vy                113 words, 18
       vx axis first, then corner vz/vy       113 words, 18
       corner-major vy, vz, vx                113 words, 20
       axis-major vz, vy, vx                  112 words, 20
       corner-major vz, vy, vx                112 words, 21
       axis-major vz, vx, vy                  112 words, 28
       corner-major vz, vx, vy                112 words, 33

   The four 112-word orders are the ones that put a +0x60 read directly after
   the `*(s16 *)(p + 0x60) = 0' store: cse folds it to the constant, the
   `lhu' disappears and `sh $zero, 0x62' replaces it. The target has TWO
   `lhu 0x60' -- one serving +0x62 and +0x64, one serving +0x66 -- and one
   `lhu 0x50' serving all three of its dependents, which only the axis-major
   order reproduces.

   `d = *(u8 **)(p + 0x4C);' COMES BEFORE `D_80026664 = 1;', worth one word
   and 6 aligned differences the other way round (114 words). This is the same
   dependence func_s17_80025278 in this unit pays for: gcc 2.7.2 cannot
   disambiguate the `mem:HI (lo_sum D_80026664)' store from
   `mem:SI (s0 + 0x4C)', so the load cannot be scheduled above the store and
   the `lw $s3' leaves the `lui's load-delay slot empty.

   THE RECORD POINTERS AT +0x44 AND +0x48 ARE RE-READ AT EVERY USE, not held
   in a local: the target has three `lw 0x44($s0)' and three `lw 0x48($s0)',
   which is what writing the double dereference out each time gives once the
   store through the other pointer invalidates cse's entry.
   `(*(s16 *)(p + 0xC) -= 0x64) < 0' is the in-place flag form, which is what
   lets combine fold the wrap arm's `addiu 0x1000' onto the pre-decrement
   value as a single `addiu 0xF9C'. 113 words, 0 differing. */
void func_s17_80025814(u8 *p, UnkVec *v, u8 *c)
{
    u8 *d;

    d = *(u8 **)(p + 0x4C);
    D_80026664 = 1;
    if (*(s16 *)(d + 0x9C) == 0x37)
    {
        *(s16 *)(p + 0x50) = *(s16 *)(p + 0xA) * func_80064584(*(s16 *)(p + 0xC)) >> 12;
        *(s16 *)(p + 0x58) = *(s16 *)(p + 0xA) * func_800644B8(*(s16 *)(p + 0xC)) >> 12;
        *(s16 *)(p + 0x60) = 0;
        *(s16 *)(p + 0x52) = *(u16 *)(p + 0x50) + 0x10;
        *(s16 *)(p + 0x54) = *(u16 *)(p + 0x50);
        *(s16 *)(p + 0x56) = *(u16 *)(p + 0x50) + 0x10;
        *(s16 *)(p + 0x5A) = *(u16 *)(p + 0x58);
        *(s16 *)(p + 0x5C) = *(u16 *)(p + 0x58);
        *(s16 *)(p + 0x5E) = *(u16 *)(p + 0x58);
        *(s16 *)(p + 0x62) = *(u16 *)(p + 0x60);
        *(s16 *)(p + 0x64) = *(u16 *)(p + 0x60) + 0x10;
        *(s16 *)(p + 0x66) = *(u16 *)(p + 0x60) + 0x10;
        v->unk_00 = *(s32 *)(*(u8 **)(p + 0x44) + 0x0);
        v->unk_04 = *(s32 *)(*(u8 **)(p + 0x44) + 0x4);
        v->unk_08 = *(s32 *)(*(u8 **)(p + 0x44) + 0x8);
        c[0xC] = (*(u8 **)(p + 0x48))[0xC];
        c[0xD] = (*(u8 **)(p + 0x48))[0xD];
        c[0xE] = (*(u8 **)(p + 0x48))[0xE];
        if ((*(s16 *)(p + 0xC) -= 0x64) < 0)
        {
            *(s16 *)(p + 0xC) += 0x1000;
        }
        if (*(s16 *)(d + 0xA) < 5)
        {
            return;
        }
    }
    *(u16 *)(p - 0x2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

/* 0x20 bytes of this bank's own rodata, between its jump tables,
   read by func_s17_800259D8. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_s17_80024058[] = {
    0x08,
    0x00,
    0x00,
    0x00,
    0x08,
    0x00,
    0x08,
    0x00,
    0x00,
    0x00,
    0x08,
    0x00,
    0xF8,
    0xFF,
    0x08,
    0x00,
    0xF8,
    0xFF,
    0x00,
    0x00,
    0xF8,
    0xFF,
    0xF8,
    0xFF,
    0x00,
    0x00,
    0xF8,
    0xFF,
    0x08,
    0x00,
    0xF8,
    0xFF,
};

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_800259D8);
