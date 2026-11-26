#include "common.h"

s32 rand(void);

typedef union UnkDng2429CW
{
    s32 w;
    u16 h[2];
} UnkDng2429CW;
typedef struct UnkDng2429CSP
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
    u8 pad_72[0x2];
    u16 unk_74;
    u16 unk_76;
    u16 unk_78;
    u8 pad_7A[0x2];
    u16 unk_7C;
    u16 unk_7E;
    u16 unk_80;
    u8 pad_82[0x2];
    s32 unk_84;
    s32 unk_88;
    u8 pad_8C[0x28];
    s32 unk_B4;
    u8 pad_B8[0x20];
    u16 unk_D8;
    u16 unk_DA;
    u16 unk_DC;
    u16 unk_DE;
    u16 unk_E0;
    u16 unk_E2;
    u16 unk_E4;
    u16 unk_E6;
    u8 pad_E8[0xC];
    s32 unk_F4;
    s32 unk_F8;
    u8 pad_FC[0xC];
    UnkDng2429CW unk_108;
    UnkDng2429CW unk_10C;
    s32 unk_110;
} UnkDng2429CSP;
UnkNode *func_8003FC64(s32);
s32 func_800A44E0(u16, u16, s16, u32);
s32 func_800BCB04(s32, s32, s16);
s32 func_800A56E0(s32);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_800DDC40[];
extern s32 D_8008346C;
UnkNode *func_s43_80024170(u8 *p, UnkVec *q);
UnkNode *func_s43_80025874(u8 *p, UnkVec *q, s16 d);
extern u8 D_80045340[];
extern u8 D_800DE720[];
void func_s43_80024044(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_s43_800253F4(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_s43_8002429C(u8 *p, UnkVec *b, s16 e, s16 f);
void func_s43_800246C0(u8 *p, UnkVec *b, s16 e, s16 f);
void func_s43_80024ACC(u8 *p, UnkVec *b, s16 e, s16 f);
void func_s43_8002539C(u8 *a);
void func_s43_8002528C(u8 *p, UnkVec *b, u8 *c, u8 *d);

/* The per-frame step func_s43_80024170 installs: bump the +0x14 counter on the
   node at the payload's +0x0, run the payload's +0x6 frame counter up, advance
   the UnkVec's x/y by their velocities, accumulate the +0xC angle by the z
   velocity and set the z position to that angle less func_800644B8's sine of
   0x800 / p+0x8 * p+0x6 scaled by 0x4000, ease the renderable's +0x1C/+0x1E
   scale down by 0x200 / p+0x8 a frame and spin its +0x1A by 0x300, then retire
   when +0x6 passes +0x8.
   75 words, 0 differing.
   NO TWIN: dngtwin finds nothing at 0.70 and xsibs nothing at all. It came
   off the listing against csrc/ovl_dng_s22.c's landed func_s22_80024028 (the
   "bump +0x14, advance the UnkVec, spin +0x1A, retire on the counter"
   skeleton) plus this bank's three extra statements.
   TWO FACTS, both priced from the 0 state with aligndiff:
     * THE +0x1C/+0x1E SCALE STATEMENT COMES BEFORE THE +0x1A SPIN.  The other
       order is 77 words / 14 aligned differences: the +0x1A read-modify-write
       cannot be scheduled into the 0x200 / p+0x8 division's shadow, so the
       whole block lands four slots late and two `nop' appear.
     * `if (*(s16 *)(p + 0x6) > *(s16 *)(p + 0x8))' and NOT the same test
       written `p + 0x8 < p + 0x6': 2 aligned differences, the two `lh' in the
       other order, with nothing else moving (the brief's comparison-operand
       lever).
   MEASURED AND FREE, so none of these is why this lands: the +0x1C/+0x1E pair
   as a chained `c+0x1C = c+0x1E = <expr>' (kept), as two statements through an
   `s32' local, or written out twice (all 75 / 0 -- cse forwards the +0x1E
   store to the +0x1C read either way); and `((...) >> 4) * 0x4000' for
   `((...) >> 4) << 14'. */
void func_s43_80024044(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)(*(u8 **)(p + 0x0) + 0x14) = *(u16 *)(*(u8 **)(p + 0x0) + 0x14) + 1;
    *(s16 *)(p + 0x6) = *(u16 *)(p + 0x6) + 1;
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    *(s32 *)(p + 0xC) += b->unk_14;
    b->unk_08 = *(s32 *)(p + 0xC) -
                ((func_800644B8(0x800 / *(s16 *)(p + 0x8) * *(s16 *)(p + 0x6)) >> 4) << 14);
    *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - 0x200 / *(s16 *)(p + 0x8);
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x300;
    if (*(s16 *)(p + 0x6) > *(s16 *)(p + 0x8))
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212 node whose step is func_s43_80024044: payload +0x0 the
   caller's record, +0x6 the frame counter at 0, +0x8 the lifetime copied from
   the record's +0x12, +0xC the mover's z; dress the slot with a 0x808080 tint,
   D_800DE720's model, 0x7E00 at +0x12, bit 8 set in +0x14, a random
   `rand() % 0x1000' spin at +0x1A and 0x400 scale, register it with
   func_8004491C against D_80045340, copy the caller's UnkVec into the node's
   own and seed the payload's +0xC from its z.
   75 words, 0 differing, FIRST COMPILE.
   TWIN: the landed func_s22_800240F8 in csrc/ovl_dng_s22.c (dngtwin 0.861,
   69 words), whose two facts both HOLD here: the `r = (u8 *)n + 0x20' payload
   pointer local, and the jitter as a SIGNED `rand() % 0x1000' (its bgez /
   addiu 0xFFF / sra 12 / sll 12 / subu quintet is 5 of these 75 words; the
   twin prices `rand() & 0xFFF' at 57 differing).  The deltas off the listing:
   the payload fields move from +0xC/+0x10/+0x12 to +0x0/+0x6/+0x8, the model
   symbol is the resident D_800DE720 rather than a unit-local table, the twin's
   `k->unk_14 = 0xFFFF0000' becomes `*(u32 *)(r + 0xC) = k->unk_08', and two
   dress stores (+0x12 and the +0x14 or) are new.
   NOTHING WAS FOUGHT FOR, so every spelling here is ASSERTED, not measured. */
UnkNode *func_s43_80024170(u8 *p, UnkVec *q)
{
    UnkNode *n;
    UnkVec *k;
    u8 *m;
    u8 *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s43_80024044;
        r = (u8 *)n + 0x20;
        *(u8 **)(r + 0x0) = p;
        *(s16 *)(r + 0x6) = 0;
        *(s16 *)(r + 0x8) = *(u16 *)(p + 0x12);
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        *(u32 *)(m + 0x8) = (u32)D_800DE720;
        *(s16 *)(m + 0x12) = 0x7E00;
        *(s16 *)(m + 0x14) = *(u16 *)(m + 0x14) | 0x100;
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1E) = 0x400;
        *(s16 *)(m + 0x1C) = 0x400;
        func_8004491C(n, (u32)D_80045340);
        k = n->unk_08;
        *k = *q;
        *(u32 *)(r + 0xC) = k->unk_08;
        return n;
    }
    return NULL;
}

/* One semi-transparent gouraud LINE_G2 per ring segment: five 0x14-byte
   primitives off D_80083160.unk_00's +0x8D0 cursor, each tinted from the
   payload's +0x40..+0x42, with both endpoints taken from the UnkVec's
   x/y/z and pushed out by func_800644B8/func_80064584's sine and cosine of
   i * 0x333 + p+0xA scaled by p+0xE << 8 -- the near endpoint by (e - 1)/f of
   it and the far one by e/f -- and the z spread likewise from
   (p+0x14 << 16) - b->unk_08.  RotTransPers each endpoint, average the two
   otz, and when that is under 0x1E0 addPrim the line and a 0xC-byte DR_MODE
   carrying GetTPage(0, p+0x12, 0, 0).
   265 words, 0 differing, FIRST COMPILE.
   NO TWIN AT ANY RATIO.  The callee-set intersection of RotAverage4/GetTPage/
   func_80067F20 across csrc/ gave thirteen files and none is this shape; what
   found it was the IN-UNIT SIBLING func_s43_800246C0 forty words below, which
   is the same emitter for a POLY_G4 -- the scratchpad typedef, the `g'
   pointer local, the chained vector stores, the `m' DR_MODE local and the
   addPrim pair all came from there with no measurement.
   FOUR FACTS PRICED FROM THE 0 STATE with aligndiff, all of them things the
   sibling does not settle:
     * THE TWO `/ f' DIVISIONS ARE WRITTEN TWICE, once per endpoint.  Hoisted
       into a local it is 262 words / 11 aligned differences -- cse does not
       unify across a `div' (the brief's barrier rule), so the target really
       divides twice and the second `div' in each pair is source.
     * THE Z SPREAD IS SHIFTS, NOT THE SAME ARITHMETIC AS THE X/Y: the x/y use
       `sp->unk_108.w / f * (e - 1)' and the z uses
       `sp->unk_110 >> f << (e - 1)'.  Writing the z as `/ f * (e - 1)' too is
       274 words / 38 aligned differences (`div'/`mult' where the target has
       `srav'/`sllv').
     * `(e - 1)' IS INLINE AT BOTH SITES.  As a local assigned before the loop
       it is 267 words / 163 -- the pseudo takes a callee-saved register the
       whole body wants, where the target recomputes the `addiu $t1, $fp, -1'
       once in the prologue and SPILLS it.
     * the averaged otz is `(unk_F4 + unk_F8) / 2', a SIGNED divide; `>> 1' is
       263 words / 3 differences (the srl 31 / addu rounding pair absent). */
void func_s43_8002429C(u8 *p, UnkVec *b, s16 e, s16 f)
{
    UnkDng2429CSP *sp = (UnkDng2429CSP *)0x1F800000;
    Unk80083160 *g;
    u8 *q;
    u8 *m;
    s32 i;

    g = &D_80083160;
    sp->unk_18 = D_80083160.unk_00 + 0xB0;
    for (i = 0; i < 5; i++)
    {
        q = *(u8 **)(g->unk_00 + 0x8D0);
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0x14);
        q[0x3] = 4;
        q[0x7] = 0x52;
        q[0x4] = p[0x40];
        q[0x5] = p[0x41];
        q[0x6] = p[0x42];
        q[0xC] = p[0x40];
        q[0xD] = p[0x41];
        q[0xE] = p[0x42];
        sp->unk_64 = sp->unk_6C = *(u16 *)((u8 *)b + 0x2);
        sp->unk_108.w =
            (func_800644B8(i * 0x333 + *(s16 *)(p + 0xA)) >> 4) * *(s16 *)(p + 0xE) << 8;
        sp->unk_64 = sp->unk_64 + (sp->unk_108.w / f * (e - 1) >> 16);
        sp->unk_6C = sp->unk_6C + (sp->unk_108.w / f * e >> 16);
        sp->unk_66 = sp->unk_6E = *(u16 *)((u8 *)b + 0x6);
        sp->unk_10C.w =
            (func_80064584(i * 0x333 + *(s16 *)(p + 0xA)) >> 4) * *(s16 *)(p + 0xE) << 8;
        sp->unk_66 = sp->unk_66 + (sp->unk_10C.w / f * (e - 1) >> 16);
        sp->unk_6E = sp->unk_6E + (sp->unk_10C.w / f * e >> 16);
        sp->unk_68 = sp->unk_70 = *(u16 *)((u8 *)b + 0xA);
        sp->unk_110 = (*(s16 *)(p + 0x14) << 16) - b->unk_08;
        sp->unk_68 = sp->unk_68 + (sp->unk_110 >> f << (e - 1) >> 16);
        sp->unk_70 = sp->unk_70 + (sp->unk_110 >> f << e >> 16);
        sp->unk_F4 =
            RotTransPers((SVECTOR *)&sp->unk_64, (long *)&sp->unk_D8, (long *)&sp->unk_84, (long *)&sp->unk_88);
        sp->unk_F8 =
            RotTransPers((SVECTOR *)&sp->unk_6C, (long *)&sp->unk_DC, (long *)&sp->unk_84, (long *)&sp->unk_88);
        *(s16 *)(q + 0x8) = sp->unk_D8;
        *(s16 *)(q + 0xA) = sp->unk_DA;
        *(s16 *)(q + 0x10) = sp->unk_DC;
        *(s16 *)(q + 0x12) = sp->unk_DE;
        sp->unk_B4 = (sp->unk_F4 + sp->unk_F8) / 2;
        if ((u32)sp->unk_B4 < 0x1E0)
        {
            addPrim(sp->unk_18 + sp->unk_B4 * 4, q);
            m = *(u8 **)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)(m + 0xC);
            func_80067F20((u32 *)m, 0, 0, GetTPage(0, *(s16 *)(p + 0x12), 0, 0) & 0xFFFF, 0);
            addPrim(sp->unk_18 + sp->unk_B4 * 4, m);
        }
    }
}

/* One semi-transparent gouraud POLY_G4 per ring segment: five 0x24-byte
   primitives off D_80083160.unk_00's +0x8D0 cursor, each tinted from the
   payload's +0x40..+0x42 on all four corners, spanning ring entry i and
   (i + 2) % 5 of the payload's +0x18/+0x2C 16.16 x/y rings -- the far pair
   offset by the high halfword of the delta, scaled by e/f while e < f -- with
   the lower edge at b+0xA - p+0x10 and the upper at b+0xA.  RotAverage4 the
   four corners, and when the otz is under 0x1E0 addPrim the quad and a
   0xC-byte DR_MODE carrying GetTPage(0, p+0x12, 0, 0).
   259 words, 0 differing.
   NO TWIN AT ANY RATIO; the shape came from csrc/ovl_dng_b06.c's landed
   func_b06_80174334 (the scratchpad-pointer-local / RotAverage4 / addPrim
   idiom and the `addPrim(sp->unk_XX + otz * 4, q)' spelling) and the rest off
   the listing.  THE SCRATCHPAD TYPEDEF IS THIS UNIT'S OWN and is shared with
   func_s43_8002429C and the parked func_s43_80024ACC: its vector slots are
   u16 FIELDS, not `SVECTOR'/`DVECTOR' members, because func_s43_8002429C
   read-modify-writes them and the target's loads are `lhu'.  The four
   arguments are therefore cast: `(SVECTOR *)&sp->unk_64'.
   FOUR FACTS PRICED FROM THE 0 STATE with aligndiff:
     * `Unk80083160 *g = &D_80083160' ABOVE THE LOOP, with `g->unk_00' at both
       in-loop cursor sites and the DIRECT `D_80083160.unk_00' at the one
       pre-loop `sp->unk_18' store.  `g' assigned INSIDE the loop is 257 words
       / 11 aligned differences and no `g' at all is also 257 / 11 -- two words
       short each time, because the target wants the `lui'/`addiu'/`lw 0x0'
       triple in the loop and the `lui'/`lw %lo' pair outside it.
     * A SECOND POINTER LOCAL `m' FOR THE DR_MODE.  Reusing `q' is 259 words /
       7 aligned differences: with two sets `q' is no longer local to the
       block, so `combine_regs' (local-alloc.c:1759) will not tie the last
       addPrim's `and' output to the dying input and the target's
       `and $s0, $s0, $s4' comes out `and $v1, $s0, $s4' with the ot address in
       the wrong register.
     * THE TOP EDGE IS CHAINED `sp->unk_80 = sp->unk_78 = <expr>', i.e. the
       0x7C vector's z first in SOURCE and so SECOND in emission.  The other
       way round is 4 aligned differences.
     * the (i + 2) % 5 row cursor is a LOCAL: inline at both sites it is 1
       aligned difference. */
void func_s43_800246C0(u8 *p, UnkVec *b, s16 e, s16 f)
{
    UnkDng2429CSP *sp = (UnkDng2429CSP *)0x1F800000;
    Unk80083160 *g;
    u8 *q;
    u8 *m;
    u8 *r;
    s32 i;

    g = &D_80083160;
    sp->unk_18 = D_80083160.unk_00 + 0xB0;
    for (i = 0; i < 5; i++)
    {
        q = *(u8 **)(g->unk_00 + 0x8D0);
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0x24);
        q[0x3] = 8;
        q[0x7] = 0x3A;
        q[0x4] = p[0x40];
        q[0x5] = p[0x41];
        q[0x6] = p[0x42];
        q[0xC] = p[0x40];
        q[0xD] = p[0x41];
        q[0xE] = p[0x42];
        q[0x14] = p[0x40];
        q[0x15] = p[0x41];
        q[0x16] = p[0x42];
        q[0x1C] = p[0x40];
        q[0x1D] = p[0x41];
        q[0x1E] = p[0x42];
        sp->unk_64 = sp->unk_74 = *(u16 *)(p + 0x1A + i * 4);
        r = p + (i + 2) % 5 * 4;
        sp->unk_108.w = *(s32 *)(r + 0x18) - *(s32 *)(p + 0x18 + i * 4);
        if (e < f)
        {
            sp->unk_108.w = sp->unk_108.w / f * e;
        }
        sp->unk_6C = sp->unk_7C = *(u16 *)(p + 0x1A + i * 4) + sp->unk_108.h[1];
        sp->unk_66 = sp->unk_76 = *(u16 *)(p + 0x2E + i * 4);
        sp->unk_10C.w = *(s32 *)(r + 0x2C) - *(s32 *)(p + 0x2C + i * 4);
        if (e < f)
        {
            sp->unk_10C.w = sp->unk_10C.w / f * e;
        }
        sp->unk_6E = sp->unk_7E = *(u16 *)(p + 0x2E + i * 4) + sp->unk_10C.h[1];
        sp->unk_68 = sp->unk_70 = *(u16 *)((u8 *)b + 0xA) - *(u16 *)(p + 0x10);
        sp->unk_80 = sp->unk_78 = *(u16 *)((u8 *)b + 0xA);
        sp->unk_B4 = RotAverage4((SVECTOR *)&sp->unk_64, (SVECTOR *)&sp->unk_6C,
                                 (SVECTOR *)&sp->unk_74, (SVECTOR *)&sp->unk_7C,
                                 (long *)&sp->unk_D8, (long *)&sp->unk_DC, (long *)&sp->unk_E0,
                                 (long *)&sp->unk_E4, (long *)&sp->unk_84, (long *)&sp->unk_88);
        *(s16 *)(q + 0x8) = sp->unk_D8;
        *(s16 *)(q + 0xA) = sp->unk_DA;
        *(s16 *)(q + 0x10) = sp->unk_DC;
        *(s16 *)(q + 0x12) = sp->unk_DE;
        *(s16 *)(q + 0x18) = sp->unk_E0;
        *(s16 *)(q + 0x1A) = sp->unk_E2;
        *(s16 *)(q + 0x20) = sp->unk_E4;
        *(s16 *)(q + 0x22) = sp->unk_E6;
        if ((u32)sp->unk_B4 < 0x1E0)
        {
            addPrim(sp->unk_18 + sp->unk_B4 * 4, q);
            m = *(u8 **)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)(m + 0xC);
            func_80067F20((u32 *)m, 0, 0, GetTPage(0, *(s16 *)(p + 0x12), 0, 0) & 0xFFFF, 0);
            addPrim(sp->unk_18 + sp->unk_B4 * 4, m);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s43", func_s43_80024ACC);

/* The damage-number step func_s43_8002539C installs: bump the +0x14 counter on
   the node at the actor's +0x0, take a 0x10-byte TILE off D_80083160.unk_00's
   +0x8D0 primitive cursor, lay it out 0x140 x 0xE0 at (0, 0), tint it red
   0xC0 with green and blue falling -0x40 - 0xC per frame remaining at the
   record's +0x5, hang it on the +0xB0 OT row semi-transparent, and retire the
   actor when that counter decrements to zero.
   68 words, 0 differing.
   TWIN: the landed func_s44_8002441C in csrc/ovl_dng_s44.c (dngtwin 0.926, 68
   words), the same body with the tint on the other two channels and a 0x30
   per-frame step instead of 0xC.  Its stated facts carry: the tint is ONE
   `s32' local written to both bytes, and the retire counter is `s8'.
   ONE FACT INVERTS, and it is the whole 16-difference first compile: THE TINT
   IS COMPUTED AFTER THE CONSTANT CHANNEL'S STORE here, where the twin prices
   "before" at 0 and "after" at 11.  With `v = -0x40 - *(s8 *)(c + 0x5) * 0xC'
   ahead of `q[0x4] = 0xC0' this is 68 words / 11 aligned differences; after
   it, 68 / 0.  The twin's reason still applies one level down -- it is the
   `lb' of `c + 0x5' being unable to cross a store to `q' that cse cannot
   prove disjoint -- but the sign follows which byte holds the constant.
   MEASURED AND NOT MEASURABLE: the four geometry stores in the twin's source
   order (+0xC, +0x8, +0xA, +0xE) and in the listing's emitted order (+0xC,
   +0xE, +0x8, +0xA) are both 68 words / 0 once the tint is in the right
   place, so the target does not constrain them. */
void func_s43_8002528C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *q;
    s32 v;

    *(s16 *)(*(u8 **)(p + 0x0) + 0x14) = *(u16 *)(*(u8 **)(p + 0x0) + 0x14) + 1;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0x10);
    *(s16 *)(q + 0xC) = 0x140;
    *(s16 *)(q + 0x8) = 0;
    *(s16 *)(q + 0xA) = 0;
    *(s16 *)(q + 0xE) = 0xE0;
    q[0x4] = 0xC0;
    v = -0x40 - *(s8 *)(c + 0x5) * 0xC;
    q[0x6] = v;
    q[0x5] = v;
    SetTile((TILE *)q);
    q[0x7] |= 2;
    AddPrim(D_80083160.unk_00 + 0xB0, q);
    if (--*(s8 *)(c + 0x5) == 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn this bank's 0x212 node for the effect func_s43_8002528C draws: install
   that as the node's +0x10 per-frame step, set the dresser slot's +0x5 mode
   byte to 0x10, and copy the caller's first word into the payload.
   22 words, 0 differing, FIRST COMPILE.
   TWIN: the landed func_s44_8002452C in csrc/ovl_dng_s44.c (dngtwin 0.905, 20
   words), identical but for the mode byte (4 there) and the payload store,
   which copies `*(u32 *)a' here instead of the pointer itself -- that
   dereference is the two extra words and the reason `a' needs $s0 across the
   func_8003FC64 call.  The twin's one stated fact carries: the `lw 0xC($a0)'
   appearing before the `sw 0x10($a0)' is sched1 filling the beqz delay, not
   source order, and the three stores are in source order. */
void func_s43_8002539C(u8 *a)
{
    UnkNode *n;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s43_8002528C;
        ((UnkSlot *)n->unk_0C)->unk_05 = 0x10;
        *(u32 *)n->payload = *(u32 *)a;
    }
}

/* The seven-state step func_s43_80025874 installs, and the owner of
   jtbl_s43_80024008.  Every frame: bump the +0x14 counter on the node at the
   payload's +0x0, run the +0x6 frame counter up, and rebuild the five-point
   ring at +0x18/+0x2C from the UnkVec's x/y plus func_800644B8/func_80064584
   of i * 0x333 + p+0xA scaled by p+0xE << 8.  Then switch on +0x4: state 0
   draws the LINE_G2 fan and seeds the mover's z from +0x14; state 1 draws the
   quad ring and arms a 4-frame state 2; state 2 adds the solid pass and arms
   0x10 frames with +0x12 = 3; states 3, 4 and 5 spin the ring by a +0x10
   accelerating step (wrapping +0xA at 0x1000) while fading the tint, state 4
   also spawning the number and state 5 doubling the radius for the five-point
   rebuild at << 9 before halving it again; state 6 retires the actor.
   288 words, 0 differing.
   THIS FUNCTION OWNS A JUMP TABLE: jtbl_s43_80024008, 8 `.word' = 7 cases +
   1 pad word (0x00000000), dispatch bound `sltiu $v0, $v1, 0x7' with NO minval
   subtraction, so the case set is 0..6 and the arms are written ascending.
   All 7 entries are distinct and their addresses ascend: 0 .Ls43_800254DC,
   1 .Ls43_8002551C, 2 .Ls43_80025558, 3 .Ls43_800255B0, 4 .Ls43_80025640,
   5 .Ls43_800256F4, 6 .Ls43_80025830.  THE RANGE-CHECK `beqz $v0,
   .Ls43_80025850' LANDS ON THE BARE EPILOGUE, which is no table entry, so
   there is NO DEFAULT LABEL AMONG THE ENTRIES and nothing to hand-settle; an
   explicit `default: break;' measures 288 / 0, i.e. free.
   NO TWIN AT ANY RATIO.  The payload layout came from func_s43_80025874,
   which this unit's own listing shows installing this function, and the three
   handlers it calls are the three siblings above it.
   THREE FACTS PRICED FROM THE 0 STATE with aligndiff:
     * `s16 k' HOLDING THE WRAPPED ANGLE, assigned and stored and then taken
       modulo, in states 3, 4 and 5.  Re-reading `*(s16 *)(p + 0xA)' for the
       modulo is 285 words / 75 aligned differences -- three words short,
       because the intervening store to another payload field invalidates
       cse's entry and the target's `sll 16'/`sra 16' of the live value becomes
       a one-word `lh' reload.  And the WIDTH is the fact, not the local:
       `s32 k' is 285 words / 80.
     * THE STATEMENT ORDER INSIDE STATES 4 AND 5 IS +0xE, THE TINT BYTES,
       +0xC, THEN THE ANGLE.  All twelve legal permutations were swept and
       only that one reaches 0: the next best is 8 (+0xE, +0xC, angle, tint),
       then 22, 26, 38, 39, 40, 48, 53, 57, 59 and 60.  SIX of the twelve also
       come out 290 words, so this is one of the cases where a position must be
       swept rather than read.
     * `*(s16 *)(p + 0x6) >= *(s16 *)(p + 0x8)' at all five gates with the
       counter FIRST; the two `lh' come out in that order.
   MEASURED AND FREE: `*(u16 *)(p + 0xE) * 2' for `<< 1' in state 5 (288 / 0
   both ways, so the sub-word doubling lever does not bite at this
   destination), and the explicit `default: break;' above. */
void func_s43_800253F4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 i;
    s16 k;

    *(s16 *)(*(u8 **)(p + 0x0) + 0x14) = *(u16 *)(*(u8 **)(p + 0x0) + 0x14) + 1;
    *(s16 *)(p + 0x6) = *(u16 *)(p + 0x6) + 1;
    for (i = 0; i < 5; i++)
    {
        *(s32 *)(p + 0x18 + i * 4) =
            b->unk_00 +
            ((func_800644B8(i * 0x333 + *(s16 *)(p + 0xA)) >> 4) * *(s16 *)(p + 0xE) << 8);
        *(s32 *)(p + 0x2C + i * 4) =
            b->unk_04 +
            ((func_80064584(i * 0x333 + *(s16 *)(p + 0xA)) >> 4) * *(s16 *)(p + 0xE) << 8);
    }
    switch (*(s16 *)(p + 0x4))
    {
    case 0:
        func_s43_8002429C(p, b, *(s16 *)(p + 0x6), *(s16 *)(p + 0x8));
        if (*(s16 *)(p + 0x6) >= *(s16 *)(p + 0x8))
        {
            b->unk_08 = *(s16 *)(p + 0x14) << 16;
            *(s16 *)(p + 0x6) = 0;
            *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
        }
        break;
    case 1:
        func_s43_800246C0(p, b, *(s16 *)(p + 0x6), *(s16 *)(p + 0x8));
        if (*(s16 *)(p + 0x6) >= *(s16 *)(p + 0x8))
        {
            *(s16 *)(p + 0x6) = 0;
            *(s16 *)(p + 0x8) = 4;
            *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
        }
        break;
    case 2:
        func_s43_800246C0(p, b, 0, 0);
        func_s43_80024ACC(p, b, *(s16 *)(p + 0x6), *(s16 *)(p + 0x8));
        if (*(s16 *)(p + 0x6) >= *(s16 *)(p + 0x8))
        {
            *(s16 *)(p + 0x12) = 3;
            *(s16 *)(p + 0x6) = 0;
            *(s16 *)(p + 0x8) = 0x10;
            *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
        }
        break;
    case 3:
        *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) + 0x10;
        k = *(u16 *)(p + 0xA) + *(u16 *)(p + 0xC);
        *(s16 *)(p + 0xA) = k;
        *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 0x10;
        *(s16 *)(p + 0xA) = k % 0x1000;
        func_s43_800246C0(p, b, 0, 0);
        func_s43_80024ACC(p, b, 0, 0);
        if (*(s16 *)(p + 0x6) >= *(s16 *)(p + 0x8))
        {
            *(s16 *)(p + 0x6) = 0;
            *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
        }
        break;
    case 4:
        *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) - 0x4;
        p[0x41] = p[0x42] = p[0x42] + 0xC;
        *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) + 0x10;
        k = *(u16 *)(p + 0xA) + *(u16 *)(p + 0xC);
        *(s16 *)(p + 0xA) = k;
        *(s16 *)(p + 0xA) = k % 0x1000;
        func_s43_800246C0(p, b, 0, 0);
        func_s43_80024ACC(p, b, 0, 0);
        if (*(s16 *)(p + 0x6) >= *(s16 *)(p + 0x8))
        {
            func_s43_8002539C(p);
            *(s16 *)(p + 0x6) = 0;
            *(s16 *)(p + 0x8) = 0x20;
            *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
        }
        break;
    case 5:
        *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 0xC;
        p[0x40] = p[0x41] = p[0x42] = p[0x42] - 0x6;
        *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) + 0x10;
        k = *(u16 *)(p + 0xA) + *(u16 *)(p + 0xC);
        *(s16 *)(p + 0xA) = k;
        *(s16 *)(p + 0xA) = k % 0x1000;
        func_s43_800246C0(p, b, 0, 0);
        *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) << 1;
        for (i = 0; i < 5; i++)
        {
            *(s32 *)(p + 0x18 + i * 4) =
                b->unk_00 +
                ((func_800644B8(i * 0x333 + *(s16 *)(p + 0xA)) >> 4) * *(s16 *)(p + 0xE) << 9);
            *(s32 *)(p + 0x2C + i * 4) =
                b->unk_04 +
                ((func_80064584(i * 0x333 + *(s16 *)(p + 0xA)) >> 4) * *(s16 *)(p + 0xE) << 9);
        }
        func_s43_80024ACC(p, b, 0, 0);
        *(s16 *)(p + 0xE) = (s16) * (u16 *)(p + 0xE) >> 1;
        if (*(s16 *)(p + 0x6) >= *(s16 *)(p + 0x8))
        {
            *(s16 *)(p + 0x6) = 0;
            *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
        }
        break;
    case 6:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}

/* Spawn the 0x212 node whose step is func_s43_800253F4: payload +0x0 the
   caller's record, +0x8 the 8-frame first-state lifetime, +0xE 0x48, +0x10 2,
   +0x12 1, +0x14 the caller's third argument, +0x40 0xC0 and +0x41/+0x42 0 as
   the starting tint, +0x4/+0x6/+0xA/+0xC cleared, and the caller's UnkVec
   copied into the node's own.
   56 words, 0 differing, FIRST COMPILE.
   NO TWIN.  It is func_s43_80024170 twenty listings up -- the same
   func_8003FC64(0x212) / `+0x10 install' / `r = (u8 *)n + 0x20' skeleton with
   a different payload and no dress block -- and that is where the shape came
   from.  The `addu $v0, $zero, $zero' in the `beqz' delay slot is the trailing
   `return NULL;' hoisted, not a separate arm.
   NOT MEASURABLE: `*(u8 **)(r + 0x0) = p' and `*(u8 **)n->payload = p' are
   both 56 words / 0 -- structs.h records that they compile to the same
   `addu $2,$4,32', so this is not a free choice, it is an invisible one.
   Everything else here is ASSERTED: it landed first compile. */
UnkNode *func_s43_80025874(u8 *p, UnkVec *q, s16 d)
{
    UnkNode *n;
    UnkVec *k;
    u8 *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s43_800253F4;
        r = (u8 *)n + 0x20;
        *(u8 **)(r + 0x0) = p;
        *(s16 *)(r + 0x8) = 8;
        *(s16 *)(r + 0x10) = 2;
        *(s16 *)(r + 0xE) = 0x48;
        *(s16 *)(r + 0x12) = 1;
        *(s16 *)(r + 0x4) = 0;
        *(s16 *)(r + 0x6) = 0;
        *(s16 *)(r + 0xA) = 0;
        *(s16 *)(r + 0xC) = 0;
        r[0x40] = 0xC0;
        r[0x42] = 0;
        r[0x41] = 0;
        *(s16 *)(r + 0x14) = d;
        k = n->unk_08;
        *k = *q;
        return n;
    }
    return NULL;
}

/* The seven-state homing step, and the owner of jtbl_s43_80024028.  State 0
   seeds the facing from the actor's +0x2A and whitens the tint; state 1 reads
   the target through func_8003DE58, copies its position into the mover, then
   either locks on to the party member at `*(u8 **)(s + 0x60) - 0x18' (three
   abs deltas, the largest >> 4 as the frame count, the three velocities
   divided by it, with the z corrected by D_800DDC40's per-kind height and a
   0x300000 lift) or walks up to eight tiles with func_800A44E0/func_800BCB04
   and aims at the far end; states 2 and 6 integrate the velocity and retire
   the leg; states 3, 4 and 5 are the dwell, the func_8009CE1C hit and the
   teardown.
   454 words, 0 differing.
   THIS FUNCTION OWNS A JUMP TABLE: jtbl_s43_80024028, 7 `.word' = 7 cases +
   0 pad (odd case count, and it is the last object in .rodata), dispatch bound
   `sltiu $v0, $v1, 0x7' with no minval subtraction, so the case set is 0..6.
   All 7 entries are distinct and ascend, the arms are written ascending, and
   CASE 0 FALLS THROUGH INTO CASE 1.  The range-check `beqz $v0,
   .Ls43_80026034' lands on the function's COMMON TAIL (the
   `*(s16 *)(p + 0x14) = 0' after the switch), which is not a table entry, so
   there is no default label among the entries; an explicit `default: break;'
   measures 454 / 0, free.
   DONOR: the landed func_s28_80025738 in csrc/ovl_dng_s28.c (dngtwin 0.873,
   449 words), itself a relink of func_s25_800247D4.  ALL THIRTEEN of its
   priced facts carry unchanged -- the shared `UnkVec *v' across the lock-on
   and walk arms, `(cx << 6) & 0xFFFFFFC0', `ox * 64' / `oy * 64',
   `(D_8006CCD8[dir] + 1) * 32', `u16 z' for the target's +0xA across the flag
   gate, `p+0x10 = 0' ONCE after the if/else, the `i = 1' hoisted ahead of the
   vv stores, and case 6's `p+0xA = 5' before `p+0x10 = 0' -- and its device is
   absent here as it is there.  THE DELTAS off this listing: s28's
   `*(s32 *)(p + 0x18) = (s32)*(u8 **)(s + 0x60)' is gone, the lock-on z uses
   D_800DDC40 and a 0x300000 lift where s28 reads the party member's own +0x88,
   the walk arm's third abs delta is gone with it (so its max is a single
   comparison against out[1], not a loop), case 2 spawns through
   func_s43_80025874, case 4's func_8009CE1C takes 0x18 and 1, and both arms
   call func_s43_80024170.
   THE ONE FACT FOUGHT FOR, and it is new: **`A - B - K - C' AND
   `A - B - (C + K)' ARE DIFFERENT CODE, AND IT IS THE FIRST FORM THAT KEEPS
   THE GROUPING.**  The target computes the lift as a value --
   `lh $v1, 0xA($s6)' / `lui $a0, 0x30' / `addu $v1, $v1, $a0' / `subu' -- and
   writing that the obvious way, `... - (*(s16 *)((u8 *)b + 0xA) + 0x300000)',
   is 453 words / 25 aligned differences, because fold's `associate:' block
   splits ARG1 as well as ARG0 (fold-const.c:4257, `split_tree (arg1, ...)')
   and rewrites `X - (C + K)' as `(X - K) - C', giving `lui $v1, 0xffd0' and an
   `addu' on the wrong side.  Written `X - K - C' the SAME block re-associates
   it BACK: split_tree takes K off arg0, the varsign == 1 arm builds
   `fold (MINUS, arg1, con)' = `C + K' and returns `X - (C + K)' -- exactly the
   target.  Both the lock-on `out[2]' and the `b->unk_14' divide need it, and
   it is 454 / 0 with the constant moved.  Checked in the compiler source and
   confirmed on a standalone probe; `(s32)' and `(u32)' casts around the sum do
   NOT help, because split_tree strips same-mode conversions first
   (fold-const.c:1002), and a shared `s32' local for the subtrahend is 453 /
   42. */
void func_s43_80025954(u8 *p, UnkVec *b, u8 *c)
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
            v = (UnkVec *)*(u8 **)(*(u8 **)(s + 0x60) - 0x18);
            out[0] = abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)b + 0x6));
            out[2] = abs(*(s16 *)((u8 *)v + 0xA) -
                         (D_800DDC40[(*(u8 **)(s + 0x60))[0x13]] << 16) - 0x300000 -
                         *(s16 *)((u8 *)b + 0xA));
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
            b->unk_14 = (v->unk_08 - (D_800DDC40[(*(u8 **)(s + 0x60))[0x13]] << 16) -
                         0x300000 - b->unk_08) /
                        *(s16 *)(p + 0x12);
            func_s43_80024170(p, b);
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
            *(s16 *)((u8 *)v + 0x2) = ox * 64 + (D_8006CCD8[*(s16 *)(p + 0xE)] + 1) * 32;
            *(s16 *)((u8 *)v + 0x6) = oy * 64 + (D_8006CCE8[*(s16 *)(p + 0xE)] + 1) * 32;
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)b + 0xA);
            out[0] = abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)b + 0x6));
            *(s16 *)(p + 0x12) = out[0];
            if ((s16)out[1] > *(s16 *)(p + 0x12))
            {
                *(s16 *)(p + 0x12) = out[1];
            }
            *(s16 *)(p + 0x12) = (s16) * (u16 *)(p + 0x12) >> 4;
            if (*(s16 *)(p + 0x12) == 0)
            {
                *(s16 *)(p + 0x12) = 1;
            }
            b->unk_0C = (v->unk_00 - b->unk_00) / *(s16 *)(p + 0x12);
            b->unk_10 = (v->unk_04 - b->unk_04) / *(s16 *)(p + 0x12);
            b->unk_14 = 0;
            func_s43_80024170(p, b);
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
            func_s43_80025874(p, b, *(s16 *)(*(u8 **)(s + 0x60) + 0x88));
            func_800A56E0(0x300);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x10) >= 0x8)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        if (*(s16 *)(p + 0x10) >= 0x44)
        {
            func_8009CE1C(*(u8 **)(s + 0x60), 0x18, p[0x9], 1,
                          (s16)(*(u16 *)(p + 0xE) << 9), (u32)s, 1);
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
