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
void func_s67_8002408C(u8 *p, UnkVec *v, UnkSlot *s, s16 d);

typedef struct UnkDngS67Dirs
{
    u8 unk_00[0x20];
} UnkDngS67Dirs;
void func_s67_800246F0(u8 *p, UnkVec *v, UnkSlot *s);
extern s16 D_800261B0;
extern const u8 D_s67_80024034[];
extern u8 D_80045340[];
extern u8 D_800DEC00[];
UnkNode *func_8003FC64(s32);
void func_8003DB94(void *, void *, s32);
void func_800478B8(u8 *);

/* One semi-transparent gouraud quad per slot element, with its own DR_MODE:
   compose the slot's rotation with the camera matrix in the 0x1F800000
   scratchpad, publish a 0xC-byte DR_MODE on the resident OT root, then
   RotAverage4 the four SVECTORs taken off the payload's +0x54..+0x6B into a
   0x24-byte POLY_G4 at the +0x8D0 cursor and -- when the OT index is in range
   and at least one corner is on screen -- give it the payload's four colour
   words, the element's +0x1 flag byte and a second DR_MODE.
   310 words, 0 differing, FIRST COMPILE.

   The twin is the LANDED func_s17_80024188 in csrc/ovl_dng_s17.c (351 words,
   the POLY_FT4 version) and through it func_b14_80173B4C: same signature,
   same UnkDng74334SP scratchpad, same PushMatrix/RotMatrix/ReadRotMatrix/
   CompMatrix/ScaleMatrix/SetTransMatrix/SetRotMatrix chain, same `- d - 6',
   same four on-screen tests and the same s->unk_0C[3] flag byte.  All four of
   the twin's measured facts were taken as given and are NOT re-measured here:
   the scale vector is ONE CHAINED assignment, the four on-screen tests are
   `|' and not `||', `&m' does NOT go through a pointer local, and
   `e = (u8 *)s->unk_08;' comes BEFORE `sp->unk_28 = s->unk_14;'.

   WHAT IS THIS COPY'S OWN, read off this listing: there is no e[0] & 0x20
   gate and no handler call, the primitive is a POLY_G4 so the cursor steps
   0x24 and the four vertices are at stride 8 (+0x8/+0x10/+0x18/+0x20), the
   four colour words come straight off the payload at +0xC/+0x10/+0x14/+0x18,
   the tag is the `setPolyG4' MACRO plus `setSemiTrans(q, 1)' -- the pair
   folds to one `sb 8, 0x3' / `sb 0x3A, 0x7', no SetPolyG4 call -- and the
   draw is bracketed by TWO 0xC-byte DR_MODEs, the first hung on
   `g->unk_00 + 0xB0' with GetTPage(0, 0, 0, 0) before the quad and the
   second on `sp->unk_24 + sp->unk_100 * 4' with GetTPage(0, 1, 0, 0) after
   it.  The four vertex halfword runs are read as three contiguous runs of
   four (all vx, all vy, all vz) off INTERLEAVED triples at stride 6. */
void func_s67_8002408C(u8 *p, UnkVec *v, UnkSlot *s, s16 d)
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
    sp->unk_B0[0].vx = *(u16 *)(p + 0x54);
    sp->unk_B0[1].vx = *(u16 *)(p + 0x5A);
    sp->unk_B0[2].vx = *(u16 *)(p + 0x60);
    sp->unk_B0[3].vx = *(u16 *)(p + 0x66);
    sp->unk_B0[0].vy = *(u16 *)(p + 0x56);
    sp->unk_B0[1].vy = *(u16 *)(p + 0x5C);
    sp->unk_B0[2].vy = *(u16 *)(p + 0x62);
    sp->unk_B0[3].vy = *(u16 *)(p + 0x68);
    sp->unk_B0[0].vz = *(u16 *)(p + 0x58);
    sp->unk_B0[1].vz = *(u16 *)(p + 0x5E);
    sp->unk_B0[2].vz = *(u16 *)(p + 0x64);
    sp->unk_B0[3].vz = *(u16 *)(p + 0x6A);
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
            *(s32 *)(q + 0x4) = *(s32 *)(p + 0xC);
            *(s32 *)(q + 0xC) = *(s32 *)(p + 0x10);
            *(s32 *)(q + 0x14) = *(s32 *)(p + 0x14);
            *(s32 *)(q + 0x1C) = *(s32 *)(p + 0x18);
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

/* Walk the tag chain from `p` and hand every node to func_s67_8002408C with a
   zero fourth argument.  21 words, 0 differing, first compile.

   The twin is func_b06_80173E08 two listings up in THIS unit -- the same
   walk with a different per-node call -- and csrc/ovl_dungeon_800D50FC.c's
   func_800D67B0 is the resident original, identical down to the
   `s = (UnkSlot *)n->unk_0C;` cast.  Nothing was fought for; the loop is a
   `for (;;)` with the NULL test in the middle, which is what puts the
   `addiu $s0, $a2, 0x20` in the `beqz`'s delay slot. */
s32 func_s67_80024564(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_s67_8002408C(p, v, s, 0);
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

/* The tick for the record whose +0x3A counter is also its ANIMATION PHASE:
   raise D_800261B0, run the phase-3..13 arm that steps the two 0x92/0x9E UV
   bytes (and on phases 6 and 10 rewinds them by 0x60 and advances the
   0x93/0x9F pair, on 7/9/11/12 also steps the 0x8C/0x98 pair, and on 3 and 5
   plants a height and a -8 or -0xC offset), then spend the frame and kill the
   node when the counter runs out.  78 words, 0 differing.

   THIS FUNCTION OWNS jtbl_s67_80024008, 11 entries for cases 3..13 at
   0x80024008 (0 mod 8, so no padding question), ahead of this unit's
   D_s67_80024034.

   Landed off func_s67_80024834's fact, already in this unit: the switch
   index is read into a local BEFORE the D_800261B0 store, which is what puts
   the `lhu 0x3A' ahead of the `sh %lo(D_800261B0)' and buys the
   `addu $a1, $a0, $zero' parameter copy.  With `D_800261B0 = 1;' first the
   anti-dependence pins the load after the store: 78 words, 37 differing.

   `switch (t)' with `s16 t' is what makes the range reduction HImode --
   `addiu -3' then `sll 16'/`sra 16' rather than a sign-extending load and a
   word subtract: c_expand_start_case's get_unwidened keeps the short index
   because the signedness matches.

   THIS BODY'S OWN FACT: the two negative phase offsets are written through
   `*(s8 *)' and not as `p[0x8D]'.  Through the `u8 *' the constant is
   truncated first and materialises as `li 0xF4'/`li 0xF8' where the target
   has `addiu $v0, $zero, -0xC'/`-0x8': 78 words, 2 differing. */
void func_s67_800245B8(u8 *p)
{
    s16 t;

    t = *(s16 *)(p + 0x3A);
    D_800261B0 = 1;
    switch (t)
    {
    case 6:
    case 10:
        p[0x92] -= 0x60;
        p[0x9E] -= 0x60;
        p[0x93] += 0x20;
        p[0x9F] += 0x20;
        break;
    case 7:
    case 9:
    case 11:
    case 12:
        p[0x92] += 0x20;
        p[0x9E] += 0x20;
        p[0x8C] -= 1;
        p[0x98] += 1;
        break;
    case 5:
        p[0x95] = 0x18;
        p[0xA1] = 0x18;
        *(s8 *)(p + 0x8D) = -0xC;
        *(s8 *)(p + 0x99) = -0xC;
        p[0x92] += 0x20;
        p[0x9E] += 0x20;
        break;
    case 3:
        p[0x95] = 0x10;
        p[0xA1] = 0x10;
        *(s8 *)(p + 0x8D) = -0x8;
        *(s8 *)(p + 0x99) = -0x8;
        p[0x92] += 0x20;
        p[0x9E] += 0x20;
        break;
    case 4:
    case 8:
    case 13:
        p[0x92] += 0x20;
        p[0x9E] += 0x20;
        break;
    }
    if (--*(s16 *)(p + 0x3A) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The +0x10 step func_s67_80024C50 installs: raise D_800261B0, spend one
   frame of the +0x3A counter, hand the slot to func_800478B8, fade the slot's
   3-byte colour as `counter * 0x80 / limit', integrate the mover's two
   velocity words at +0xC/+0x10 into its +0x0/+0x4 position, and kill the node
   when the counter runs out.  52 words, 0 differing, FIRST COMPILE.

   Landed off func_s67_80024834's and func_s67_80024B3C's facts, already in
   this unit: the decrement is split around the D_800261B0 store, and the
   three colour bytes are one chained `[0] = [1] = [2]' assignment.  Unlike
   func_s67_80024B3C the fade reads the counter back out of MEMORY with `lh'
   -- the func_800478B8 call in between clobbers it, so there is nothing to
   reuse and no `sll 16'/`sra 16' pair. */
void func_s67_800246F0(u8 *p, UnkVec *v, UnkSlot *s)
{
    u16 t;

    t = *(u16 *)(p + 0x3A) - 1;
    D_800261B0 = 1;
    *(u16 *)(p + 0x3A) = t;
    func_800478B8((u8 *)s);
    s->unk_0C[0] = s->unk_0C[1] = s->unk_0C[2] = (*(s16 *)(p + 0x3A) << 7) / *(s16 *)(p + 0x3C);
    *(s32 *)((u8 *)v + 0x0) += *(s32 *)((u8 *)v + 0xC);
    *(s32 *)((u8 *)v + 0x4) += *(s32 *)((u8 *)v + 0x10);
    if (*(s16 *)(p + 0x3A) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s67", func_s67_800247C0);

/* Spend one frame of the payload's +0x3A life counter, raise the bank's
   D_800261B0 busy flag, fade all four of the record's 3-byte colours at
   +0xC..+0x1A in proportion to the counter over its +0x3C limit, and kill the
   node when the counter runs out.  57 words, 0 differing.

   The twin is csrc/ovl_dungeon_800D50FC.c's func_800D6804, which is the same
   fade and the same `D_800814A0 |= 0x8000' kill tail over ONE colour at a
   +0x32/+0x34 counter pair; the relink moves the counter to +0x3A/+0x3C and
   turns the three scalings into a four-element loop.

   THE TWO SPELLING FACTS, both measured on this body:

     The counter decrement is SPLIT AROUND the D_800261B0 store -- `t = load
     - 1; D_800261B0 = 1; store t;' -- so the load is live across the global
     store and block 0 needs a THIRD scratch register, which takes $a0 away
     from `p' and is exactly the target's `addu $a2, $a0, $zero' parameter
     copy.  Written as one statement (`*(s16 *)(p + 0x3A) -= 1;' either side
     of the store) it is 56 words / 57 differing -- one word SHORT, with
     every register shifted down one.  Probably macro residue: a tick macro
     whose store the flag set interleaves.

     The loop is indexed, not a walking pointer: `p[i * 4 + 0xC]' keeps the
     giv's base at `p' with displacements 0xC..0x1E, where `c = p; ...;
     c += 4' lets gcc bias the base to `p + 0xE' and store at -2/-1/0
     (56 words / 57 differing). */
void func_s67_80024834(u8 *p)
{
    s32 i;
    u16 t;

    t = *(u16 *)(p + 0x3A) - 1;
    D_800261B0 = 1;
    *(u16 *)(p + 0x3A) = t;
    for (i = 0; i < 4; i++)
    {
        p[i * 4 + 0xC] = p[i * 4 + 0x1C] * *(s16 *)(p + 0x3A) / *(s16 *)(p + 0x3C);
        p[i * 4 + 0xD] = p[i * 4 + 0x1D] * *(s16 *)(p + 0x3A) / *(s16 *)(p + 0x3C);
        p[i * 4 + 0xE] = p[i * 4 + 0x1E] * *(s16 *)(p + 0x3A) / *(s16 *)(p + 0x3C);
    }
    if (*(s16 *)(p + 0x3A) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The same tick again, for a record that also interpolates six halfwords:
   one frame off the +0x3A counter with D_800261B0 raised, then -- while the
   elapsed count +0x3C - counter is still within the +0x3E span -- the three
   pairs at +0x54..+0x5E are set to the pairs at +0x60..+0x6A plus the six
   per-axis deltas at +0x6C..+0x76 scaled by elapsed/span, and finally the
   four 3-byte colours at +0xC..+0x1A are faded exactly as in
   func_s67_80024834.  137 words, 0 differing, FIRST COMPILE.

   Written off func_s67_80024834's two measured facts, already in this unit
   (the decrement split around the D_800261B0 store, and the indexed
   `p[i * 4 + 0xC]' fade loop); nothing here had to be fought for.

   Note that only the FIRST of the six scalings reuses the gate's `elapsed'
   subtraction -- it is spelled with the pre-store register `(s16)t', which is
   the same expression the gate tested -- while the other five read
   `*(s16 *)(p + 0x3C) - *(s16 *)(p + 0x3A)' out of memory and get their own
   `lh'/`lh'/`subu'.  That is cse reproducing the target word for word; it was
   not arranged. */
void func_s67_80024918(u8 *p)
{
    u16 t;
    s32 i;

    t = *(u16 *)(p + 0x3A) - 1;
    D_800261B0 = 1;
    *(u16 *)(p + 0x3A) = t;
    if (*(s16 *)(p + 0x3C) - (s16)t <= *(s16 *)(p + 0x3E))
    {
        *(s16 *)(p + 0x54) =
            *(u16 *)(p + 0x60) + *(s16 *)(p + 0x6C) * (*(s16 *)(p + 0x3C) - (s16)t) / *(s16 *)(p + 0x3E);
        *(s16 *)(p + 0x56) = *(u16 *)(p + 0x62) +
                             *(s16 *)(p + 0x6E) * (*(s16 *)(p + 0x3C) - *(s16 *)(p + 0x3A)) / *(s16 *)(p + 0x3E);
        *(s16 *)(p + 0x58) = *(u16 *)(p + 0x64) +
                             *(s16 *)(p + 0x70) * (*(s16 *)(p + 0x3C) - *(s16 *)(p + 0x3A)) / *(s16 *)(p + 0x3E);
        *(s16 *)(p + 0x5A) = *(u16 *)(p + 0x66) +
                             *(s16 *)(p + 0x72) * (*(s16 *)(p + 0x3C) - *(s16 *)(p + 0x3A)) / *(s16 *)(p + 0x3E);
        *(s16 *)(p + 0x5C) = *(u16 *)(p + 0x68) +
                             *(s16 *)(p + 0x74) * (*(s16 *)(p + 0x3C) - *(s16 *)(p + 0x3A)) / *(s16 *)(p + 0x3E);
        *(s16 *)(p + 0x5E) = *(u16 *)(p + 0x6A) +
                             *(s16 *)(p + 0x76) * (*(s16 *)(p + 0x3C) - *(s16 *)(p + 0x3A)) / *(s16 *)(p + 0x3E);
    }
    for (i = 0; i < 4; i++)
    {
        p[i * 4 + 0xC] = p[i * 4 + 0x1C] * *(s16 *)(p + 0x3A) / *(s16 *)(p + 0x3C);
        p[i * 4 + 0xD] = p[i * 4 + 0x1D] * *(s16 *)(p + 0x3A) / *(s16 *)(p + 0x3C);
        p[i * 4 + 0xE] = p[i * 4 + 0x1E] * *(s16 *)(p + 0x3A) / *(s16 *)(p + 0x3C);
    }
    if (*(s16 *)(p + 0x3A) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The same tick as func_s67_80024834 for a record with ONE 3-byte colour: one
   frame off the +0x3A counter, D_800261B0 raised, the slot's +0xC..+0xE faded
   as `counter * 0x80 / limit', the +0x8C byte counter stepped down, the
   slot's +0x1A spun by the payload's +0x40, and the node killed when the
   counter runs out.  37 words, 0 differing.

   Landed off func_s67_80024834's two facts, already in this unit: the
   decrement is split around the D_800261B0 store for the same reason, and the
   three colour bytes are ONE CHAINED assignment written `[0] = [1] = [2]',
   which is what emits 0xE, 0xD, 0xC in that order.

   THIS BODY'S OWN FACT: the quotient also goes through a local, so the whole
   `sll 16 / sra 9 / div / mflo' chain lands BEFORE the D_800261B0 store.
   With the division written inline at the three-byte assignment the store
   sinks no further than the second insn: 37 words, 18 differing. */
void func_s67_80024B3C(u8 *p, UnkVec *v, UnkSlot *s)
{
    u16 t;
    s32 c;

    t = *(u16 *)(p + 0x3A) - 1;
    c = ((s16)t << 7) / *(s16 *)(p + 0x3C);
    D_800261B0 = 1;
    *(u16 *)(p + 0x3A) = t;
    s->unk_0C[0] = s->unk_0C[1] = s->unk_0C[2] = c;
    p[0x8C] -= 1;
    s->unk_1A += *(u16 *)(p + 0x40);
    if (*(s16 *)(p + 0x3A) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s67", func_s67_80024BD0);

/* Spawn one of this bank's 0x212 particle nodes at the caller's +0x84..+0x88
   position offset by (dx, dy, dz), with func_s67_800246F0 as its +0x10 step,
   a 5-frame life in the payload's +0x3A/+0x3C pair, a random 3-bit facing,
   and an initial velocity taken from this bank's own 8-entry direction table
   D_s67_80024034 indexed by the caller's +0x36 heading.  129 words,
   0 differing.

   The structural template is csrc/ovl_dng_b10.c's landed func_b10_80170CF8:
   the same func_8003FC64(0x212) / +0x10 install / func_8004491C(n,
   D_80045340) / func_8003DB94(r, D_800DEC00, 0) skeleton with the two
   `n->unk_0C' loads, the 0x1E-before-0x1C pair and the [2]/[1]/[0] colour
   bytes in that order.

   THE TWO SPELLING FACTS, both measured here:

     The direction table is copied into the frame by a STRUCT ASSIGNMENT
     (`dd = *(UnkDngS67Dirs *)D_s67_80024034'), not by `memcpy' -- both give
     the identical alignment-1 lwl/lwr + swl/swr block move, but only the
     struct assignment's MEM_IN_STRUCT_P lets the scheduler sink the two
     stack-parameter `lw's past it.  With `memcpy(d, D_s67_80024034, 0x20)'
     the block move anti-depends on them, they stay in the prologue, and dy
     and dz lose $s3/$s4 to dx: 129 words, 18 aligned differences.

     The table index is `<< 2', not `* 4'.  `d + (*(s16 *)(p + 0x36) << 2)'
     keeps pointer_int_sum's pointer-first order, `addu $v0, $s6, $v0';
     `d + *(s16 *)(p + 0x36) * 4' reassociates the sum and comes out
     `addu $v0, $v0, $s6' -- 2 aligned differences, and the array-member and
     `(u32)d' spellings measure the same 2.

   The three deltas must be `s16' PARAMETERS: it is the promoted-subreg
   conversion that makes each `+=' a plain `addu $v0, $sN, $zero' ahead of
   the add.  As `s32' they reload from the frame at every use and from
   `*(s16 *)(m + N)' at every `+=': 129 words, 28 aligned differences. */
void func_s67_80024C50(u8 *p, s32 a, s32 b, s16 dx, s16 dy, s16 dz)
{
    UnkDngS67Dirs dd;
    u8 *d = dd.unk_00;
    UnkNode *n;
    UnkSlot *r;
    u8 *m;
    u8 *q;

    dd = *(UnkDngS67Dirs *)D_s67_80024034;
    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x3A) = 5;
        *(s16 *)(q + 0x3C) = 5;
        n->unk_10 = (u32)func_s67_800246F0;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_14 |= 0xC;
        r->unk_1A = (rand() & 7) << 9;
        r->unk_06 = 0;
        m = (u8 *)n->unk_08;
        *(s16 *)(m + 0x2) = dx;
        *(s16 *)(m + 0x6) = dy;
        *(s16 *)(m + 0xA) = dz;
        *(s16 *)(m + 0x2) += *(u16 *)(p + 0x84);
        *(s16 *)(m + 0x6) += *(u16 *)(p + 0x86);
        *(s16 *)(m + 0xA) += *(u16 *)(p + 0x88);
        *(s32 *)(m + 0xC) = *(s16 *)(d + (*(s16 *)(p + 0x36) << 2)) << 20;
        *(s32 *)(m + 0x10) = *(u16 *)(d + (*(s16 *)(p + 0x36) << 2) + 2) << 20;
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0x800;
        r->unk_1C = 0x800;
        r->unk_0C[2] = 0x80;
        r->unk_0C[1] = 0x80;
        r->unk_0C[0] = 0x80;
        func_8003DB94(r, D_800DEC00, 0);
    }
}

/* 0x20 bytes of this bank's own rodata, between its jump tables,
   read by func_s67_80024C50. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_s67_80024034[] = {
    0x01,
    0x00,
    0x00,
    0x00,
    0x01,
    0x00,
    0x01,
    0x00,
    0x00,
    0x00,
    0x01,
    0x00,
    0xFF,
    0xFF,
    0x01,
    0x00,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0x01,
    0x00,
    0xFF,
    0xFF,
};

/* 0x24 bytes of this bank's own rodata, between its jump tables,
   read by func_s67_80024E54. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_s67_80024054[] = {
    0x05,
    0x00,
    0x00,
    0x00,
    0x05,
    0x00,
    0x05,
    0x00,
    0x00,
    0x00,
    0x05,
    0x00,
    0xFB,
    0xFF,
    0x05,
    0x00,
    0xFB,
    0xFF,
    0x00,
    0x00,
    0xFB,
    0xFF,
    0xFB,
    0xFF,
    0x00,
    0x00,
    0xFB,
    0xFF,
    0x05,
    0x00,
    0xFB,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
};

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s67", func_s67_80024E54);
