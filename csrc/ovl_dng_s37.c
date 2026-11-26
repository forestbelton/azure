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
void func_s37_80024ED4(u8 *p, UnkVec *v);

UnkNode *func_8003FD64(s32, void *);

s32 func_800BCB04(s32, s32, s16);
extern s16 D_80026428;
s32 func_800644B8(s32);
void func_80067EF4(void *, s32, s32);
extern Unk6CD30 D_8006CD10;
typedef struct UnkDng24094SP
{
    u8 pad_00[0x8];
    Unk3DBD0W unk_08;
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    u8 pad_18[0x8];
    u8 *unk_20;
    u16 unk_24;
    u8 pad_26[0xA];
    Unk3DBD0W unk_30;
    Unk3DBD0W unk_34;
    Unk3DBD0W unk_38;
    u8 pad_3C[0x34];
    s16 unk_70;
    s16 unk_72;
    s16 unk_74;
    u8 pad_76[0x2];
    s16 unk_78;
    s16 unk_7A;
    s16 unk_7C;
    u8 pad_7E[0x2];
    s16 unk_80;
    s16 unk_82;
    s16 unk_84;
    u8 pad_86[0x2];
    s16 unk_88;
    s16 unk_8A;
    s16 unk_8C;
    u8 pad_8E[0x2];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x20];
    u16 unk_B8;
    u16 unk_BA;
    u8 pad_BC[0x4];
    s32 unk_C0;
    u8 pad_C4[0x20];
    s32 unk_E4;
    s32 unk_E8;
    s32 unk_EC;
    u16 unk_F0;
    u16 unk_F2;
    u16 unk_F4;
    u16 unk_F6;
    u16 unk_F8;
    u16 unk_FA;
    u16 unk_FC;
    u16 unk_FE;
    u16 unk_100;
    u16 unk_102;
    u16 unk_104;
    u16 unk_106;
    u16 unk_108;
    u16 unk_10A;
} UnkDng24094SP;

/* The bank's room renderer: one POLY_FT4 per cell of the slot's +0x8 cell
   list, projected through the 0x1F800000 scratchpad with RotTransPers4 and
   biased by the view's +0xB8/+0xBA screen offset, then re-emitted once per
   row with the four x corners jittered by
   func_800644B8(ang) * the payload's +0x50 amplitude >> 16 and the row's y
   and v walked up one at a time until the row reaches the cell's own y2.
   A cell with 0x20 in its first byte is a handler instead: its +0x8 word is
   called as (p, v, s, e, q) and returns the new primitive cursor.
   626 words, 0 differing, 0 aligned differences.

   THE BODY IS A BLEND OF TWO RESIDENT TWINS, both in
   csrc/ovl_dungeon_800B6AC4.c. func_800B9144 gives the signature, the
   `if (d)' mask-primitive bracket at both ends, the two +0x100 carry
   corrections, the +0xE clut and +0x16 tpage arms, the two
   `q[0x14] = q[0x24]--' pairs, the +0xF shade byte, the handler tail and the
   0x8D0 writeback; func_800BC4D4 gives the e[0] ^ unk_24 corner arms,
   RotTransPers4 and the screen bias. The per-row ripple loop is this
   object's own and has no resident twin.

   FOUR MEASURED SPELLINGS, each a single ablation off the landed body.

   * `q += 0x28' COMES BEFORE THE SECOND `ang' WRAP, NOT AFTER IT. Written
     last in the loop body it is 626 words and 5 aligned differences: loop.c
     emits the q + 0x20 giv's update immediately before the biv's own, so
     putting the biv update last drags the giv update past the wrap into the
     merge block -- and then the wrap branch's delay slot has nothing but
     `addiu $s4, $s4, 0x28' to take, where the target fills it with the
     loop-invariant `li 0x9' and keeps both updates as plain insns.

   * THE `sp->unk_102' SUM IS TWO NAMED LOCALS. As one expression,
     `((sp->unk_38.h + 0x100) & 0x1FF) + (s->unk_18 - 0x100)' is 20 aligned
     differences: fold's associate step moves the -0x100 off the parameter
     and onto the masked sum. An `(s16)' cast on the right operand gets that
     to 10 and does NOT block the split -- the `addiu -0x100' still lands on
     the mask. Only naming both halves blocks it, and naming the masked half
     FIRST is also what lifts its `lhu'/`addiu 0x100' pair above the +0x104
     store, where the target has them.

   * +0x20 AND +0x22 ARE EACH READ ONCE, INTO A LOCAL. Read twice -- once for
     `sp->unk_E4' and once for `sp->unk_108' -- it is 631 words and 15
     aligned differences, five words of re-loads: the `sw' to the scratchpad
     in between is a store gcc cannot disambiguate from `s->unk_20'. This is
     func_800B9144's own `t20'/`t22' device, re-measured here.

   * THE AMPLITUDE INDEX IS `*(u8 *)(pp + i + 0x50)', NOT `pp[0x50 + i]'. The
     subscript form is 1 aligned difference: fold associates the constant
     next to the pointer and the `addu' comes out `$s6, $t1' against the
     target's `$t1, $s6'.

   Measured and NOT needed: func_800B9144's `volatile u8 *e'. It is 0 either
   way -- the scratchpad stores between the three `e[0]' reads already defeat
   cse -- so the plain `u8 *' is kept. With the volatile in place the
   e + 0x2 / e + 0x3 byte reads must then be spelled
   `(s8) * (u8 *)(e + 0x2)', because a volatile MEM is one loop.c will not
   strength-reduce onto the e + 0x8 cursor: 41 aligned differences spelled
   `(s8)e[0x2]' with the volatile on.

   The two 0x28-byte locals are POLY_FT4, which is what makes their four
   assignments compile to the target's word-aligned four-register copy loops
   instead of memcpy's runtime alignment test. `D_80083160.unk_00' is read
   DIRECTLY in the head (two words, `%lo' in the load) and through `g' in the
   two later groups, which is where the target's three `lui'/`addiu' pairs
   come from. raw 31 == relocdump's 31: 8 on D_80083160, 2 on D_8006CD10, 14
   R_MIPS_26 on the library calls and 7 on .text for the body's own `j's. */
void func_s37_80024094(u8 *a, u8 *pp, u8 *ot, s16 d)
{
    Unk80083160 *g = &D_80083160;
    UnkDng24094SP *sp = (UnkDng24094SP *)0x1F800000;
    UnkSlot *s;
    UnkVec *v;
    u8 *e;
    u8 *q;
    POLY_FT4 base;
    POLY_FT4 row;
    s32 i;
    s32 ang;
    s32 t;
    s32 t2;
    s32 u1;
    s32 u2;
    s32 c;
    s32 t20;
    s32 t22;
    s32 x18;
    s32 x38;
    u8 *(*h)(u8 *, UnkVec *, UnkSlot *, u8 *, u8 *);
    s16 a1;
    s16 a2;
    s16 b1;
    s16 b2;

    s = (UnkSlot *)*(u32 *)(pp - 0x14);
    v = *(UnkVec **)(pp - 0x18);
    e = (u8 *)*(u32 *)((u8 *)s + 0x8);
    sp->unk_EC = 0;
    sp->unk_8C = 0;
    sp->unk_84 = 0;
    sp->unk_7C = 0;
    sp->unk_74 = 0;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = ot;
    sp->unk_C0 = 0;
    sp->unk_B8 = *(u16 *)a - 0xA0;
    sp->unk_BA = *(u16 *)(a + 0x2) - 0x78;
    if (d != 0)
    {
        func_80067EF4(q, 0, 0);
        addPrim(sp->unk_20, q);
        q += 0xC;
    }
    D_8006CD10.unk_1C = *(s32 *)((u8 *)g + 0xA0);
    PushMatrix();
    sp->unk_30.w = g->unk_C4;
    sp->unk_34.w = g->unk_C6;
    sp->unk_38.w = g->unk_C8;
    sp->unk_100 = s->unk_16;
    x38 = (sp->unk_38.h + 0x100) & 0x1FF;
    sp->unk_104 = s->unk_1A - sp->unk_34.h;
    x18 = s->unk_18 - 0x100;
    sp->unk_102 = x38 + x18;
    t20 = s->unk_20;
    sp->unk_E4 = t20;
    sp->unk_108 = t20;
    t22 = s->unk_22;
    sp->unk_E8 = t22;
    sp->unk_10A = t22;
    RotMatrix((SVECTOR *)0x1F800100, (MATRIX *)0x1F8000D0);
    sp->unk_30.w = s->unk_1C;
    sp->unk_34.w = s->unk_1E;
    sp->unk_38.w = 0x1000;
    ScaleMatrix((MATRIX *)0x1F8000D0, (VECTOR *)0x1F800030);
    CompMatrix((MATRIX *)&D_8006CD10, (MATRIX *)0x1F8000D0, (MATRIX *)0x1F800050);
    SetTransMatrix((MATRIX *)0x1F800050);
    SetRotMatrix((MATRIX *)0x1F800050);
    sp->unk_24 = s->unk_14;
    do
    {
        if ((e[0x0] & 0x20) == 0)
        {
            sp->unk_08.w = *(u8 *)(e + 0x8);
            sp->unk_0C.w = *(u8 *)(e + 0x9);
            sp->unk_10.w = *(u8 *)(e + 0xA);
            sp->unk_14.w = *(u8 *)(e + 0xB);
            if ((e[0x0] ^ sp->unk_24) & 1)
            {
                a1 = -(s8)e[0x2] - sp->unk_108;
                sp->unk_80 = a1;
                sp->unk_70 = a1;
                a1 = a1 - sp->unk_10.h;
                sp->unk_88 = a1;
                sp->unk_78 = a1;
            }
            else
            {
                a2 = (s8)e[0x2] - sp->unk_108;
                sp->unk_80 = a2;
                sp->unk_70 = a2;
                a2 = a2 + sp->unk_10.h;
                sp->unk_88 = a2;
                sp->unk_78 = a2;
            }
            if ((e[0x0] ^ sp->unk_24) & 2)
            {
                b1 = -(s8)e[0x3] - sp->unk_10A;
                sp->unk_7A = b1;
                sp->unk_72 = b1;
                b1 = b1 - sp->unk_14.h;
                sp->unk_8A = b1;
                sp->unk_82 = b1;
            }
            else
            {
                b2 = (s8)e[0x3] - sp->unk_10A;
                sp->unk_7A = b2;
                sp->unk_72 = b2;
                b2 = b2 + sp->unk_14.h;
                sp->unk_8A = b2;
                sp->unk_82 = b2;
            }
            RotTransPers4((SVECTOR *)&sp->unk_70, (SVECTOR *)&sp->unk_78, (SVECTOR *)&sp->unk_80,
                          (SVECTOR *)&sp->unk_88, (s32 *)&sp->unk_F0, (s32 *)&sp->unk_F4,
                          (s32 *)&sp->unk_F8, (s32 *)&sp->unk_FC, &sp->unk_90, &sp->unk_94);
            *(s16 *)(q + 0x8) = sp->unk_F0 + sp->unk_B8;
            *(s16 *)(q + 0xA) = sp->unk_F2 + sp->unk_BA;
            *(s16 *)(q + 0x10) = sp->unk_F4 + sp->unk_B8;
            *(s16 *)(q + 0x12) = sp->unk_F6 + sp->unk_BA;
            *(s16 *)(q + 0x18) = sp->unk_F8 + sp->unk_B8;
            *(s16 *)(q + 0x1A) = sp->unk_FA + sp->unk_BA;
            *(s16 *)(q + 0x20) = sp->unk_FC + sp->unk_B8;
            *(s16 *)(q + 0x22) = sp->unk_FE + sp->unk_BA;
            q[0x3] = 9;
            u1 = sp->unk_10.w - 1;
            t = u1 + sp->unk_08.w;
            sp->unk_10.w = t;
            if ((t & 0x100) != 0)
            {
                sp->unk_10.w = t - 1;
            }
            u2 = sp->unk_14.w - 1;
            t2 = u2 + sp->unk_0C.w;
            sp->unk_14.w = t2;
            if ((t2 & 0x100) != 0)
            {
                sp->unk_14.w = t2 - 1;
            }
            sp->unk_14.w = sp->unk_14.w << 8;
            sp->unk_0C.w = sp->unk_0C.w << 8;
            if (s->unk_12 != 0)
            {
                if ((sp->unk_24 & 0x100) != 0)
                {
                    *(s16 *)(q + 0xE) = s->unk_12;
                }
                else
                {
                    *(s16 *)(q + 0xE) = s->unk_12 + *(u16 *)(e + 0x6);
                }
            }
            else
            {
                *(s16 *)(q + 0xE) = *(u16 *)(e + 0x6);
            }
            *(s16 *)(q + 0xC) = sp->unk_0C.h + sp->unk_08.h;
            *(s16 *)(q + 0x14) = sp->unk_0C.h + sp->unk_10.h;
            if (s->unk_10 != 0)
            {
                *(s16 *)(q + 0x16) = s->unk_10 + (*(u16 *)(e + 0x4) & 0xFF9F);
            }
            else
            {
                *(s16 *)(q + 0x16) = *(u16 *)(e + 0x4);
            }
            *(s16 *)(q + 0x1C) = sp->unk_14.h | sp->unk_08.h;
            *(s16 *)(q + 0x24) = sp->unk_14.h | sp->unk_10.h;
            if (*(s16 *)(q + 0x8) > *(s16 *)(q + 0x20))
            {
                q[0x14] = q[0x24]--;
            }
            if (*(s16 *)(q + 0xA) > *(s16 *)(q + 0x22))
            {
                q[0x1D] = q[0x25]--;
            }
            c = *(u8 *)(e + 0x1);
            *((u8 *)s + 0xF) = c;
            if ((sp->unk_24 & 8) != 0)
            {
                *((u8 *)s + 0xF) = (sp->unk_24 & 4) ? (c | 2) : (c & 0xFD);
            }
            *(u32 *)(q + 0x4) = *(u32 *)((u8 *)s + 0xC);
            base = *(POLY_FT4 *)q;
            row = base;
            i = 0;
            ang = *(s16 *)(pp + 0x8C);
            do
            {
                q[0x3] = 9;
                *(POLY_FT4 *)q = base;
                *(s16 *)(q + 0x8) = *(u16 *)(q + 0x8) + (func_800644B8(ang) * *(u8 *)(pp + i + 0x50) >> 16);
                *(s16 *)(q + 0x10) = *(u16 *)(q + 0x10) + (func_800644B8(ang) * *(u8 *)(pp + i + 0x50) >> 16);
                ang += *(s16 *)(pp + 0x8E);
                if (ang > 0x1000)
                {
                    ang -= 0x1000;
                }
                *(s16 *)(q + 0x18) = *(u16 *)(q + 0x18) + (func_800644B8(ang) * *(u8 *)(pp + i + 0x50) >> 16);
                *(s16 *)(q + 0x20) = *(u16 *)(q + 0x20) + (func_800644B8(ang) * *(u8 *)(pp + i + 0x50) >> 16);
                *(s16 *)(q + 0xA) = row.y0;
                *(s16 *)(q + 0x12) = row.y0;
                *(s16 *)(q + 0x1A) = row.y0 + 1;
                *(s16 *)(q + 0x22) = row.y0 + 1;
                q[0xD] = row.v0;
                q[0x15] = row.v0;
                q[0x1D] = row.v0 + 1;
                q[0x25] = row.v0 + 1;
                i++;
                addPrim(sp->unk_20, q);
                row = *(POLY_FT4 *)q;
                row.y0++;
                row.v0++;
                q += 0x28;
                ang += *(s16 *)(pp + 0x8E);
                if (ang > 0x1000)
                {
                    ang -= 0x1000;
                }
            } while (row.y0 < base.y2);
        }
        else
        {
            h = *(u8 * (**)(u8 *, UnkVec *, UnkSlot *, u8 *, u8 *))(e + 0x8);
            if (h != NULL)
            {
                q = h(pp, v, s, e, q);
            }
        }
        if (*(s8 *)e < 0)
        {
            break;
        }
        e += 0xC;
    } while (1);
    if (d != 0)
    {
        func_80067EF4(q, 0, 1);
        addPrim(sp->unk_20, q);
        q += 0xC;
    }
    PopMatrix();
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
}

/* The bank's own copy of csrc/ovl_dungeon_800B6AC4.c's func_800B8FC8 (95
   words): the draw-area / 0x60000000 tint bracket around one call of this
   object's renderer, line for line, with func_800B9144 replaced by
   func_s37_80024094 and the resident's `AddPrim' call replaced by libgpu's
   inline `addPrim' macro. 126 words, 0 differing, 0 aligned differences.

   TWO OF THE TWIN'S DEVICES HAVE TO BE REMOVED HERE, and that is most of the
   delta. Spelled the twin's way:
   * `do { ... } while (0)' around the func_s37_80024094 call is 126 words and
     18 aligned differences. flow.c:2122's `reg_n_refs[regno] += loop_depth'
     gives the fifth parameter's pseudo 3 references instead of 2 inside the
     wrapper, lifting its allocno_compare priority from 1*2/24 to 1*3/24 --
     above the hoisted `%hi(D_80083160)' and the 0xFF000000 constant -- so it
     takes $s5 instead of the $s7 the target gives it. The twin needs the
     wrapper for `r'/`g'; this body does not.
   * `g++; g--;' inside that wrapper is 130 words: four over.

   AND THE FIFTH PARAMETER IS `s16', NOT THE TWIN'S `s32'. As `s32' with
   `(s16)e' at the call it is 125 words -- a word SHORT -- and 49 aligned
   differences: assign_parms hangs a REG_EQUIV on a stack parameter's
   prologue load only when nominal_mode == passed_mode, update_equiv_regs
   then DOUBLES that pseudo's live length, its allocno loses the last
   callee-saved register and the same pass deletes the prologue load
   outright, so the value is read from 0x58($sp) at its use. Declaring the
   parameter `s16' sets did_conversion, the note is never attached, the
   pseudo keeps $s7 and the FIRST parameter spills to its own 0x48($sp) home
   instead -- which is the target's `sw $a0, 0x48($sp)' / `lw $a1, 0x48($sp)'
   pair.

   BOTH of the twin's own facts hold here and were re-measured. The 0x8D0
   store has to be the ARRAY REF `((u32 *)X)[0x234]': as
   `*(u32 *)(X + 0x8D0)' it is 13 aligned differences. And `u16 cw = (u16)c;'
   has to be hoisted out of the `if' -- the 0x18($sp) slot behind the
   target's `sh $a3' / `lhu' pair is that HImode pseudo SPILLED, not an array
   element: `u16 w[6]' with `w[4] = c;' in its place reproduces the slot and
   the 0xB8-byte frame exactly and is still 14 aligned differences.
   raw 10 == relocdump's 10: 5 on D_80083160, 2 on D_801C9E40, 3 R_MIPS_26. */
void func_s37_80024A5C(u8 *p, s32 *b, s32 *b2, s32 c, s16 e)
{
    Unk80083160 *g;
    u8 *q;
    u8 *r;
    u8 *u;
    s16 w[2];
    s16 lean;
    s32 t;
    u16 cw;

    cw = (u16)c;
    g = &D_80083160;
    lean = D_80083160.unk_00 != D_801C9E40;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    r = D_80083160.unk_00 + 0x8B0;
    ((u32 *)D_80083160.unk_00)[0x234] = (u32)(q + 0xC);
    SetDrawArea(q, D_80083160.unk_00);
    addPrim(r, q);
    w[0] = *(u16 *)b2;
    t = *((s16 *)b2 + 1);
    if (lean)
    {
        t -= 0xE0;
    }
    w[1] = t;
    func_s37_80024094((u8 *)w, p, r, e);
    if (cw != 0)
    {
        u = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(u + 0x10);
        *(u32 *)(u + 0x4) = 0x60000000;
        u[0x3] = 3;
        *(u16 *)(u + 0x8) = *(u16 *)b;
        t = *((s16 *)b + 1);
        if (lean)
        {
            t -= 0xE0;
        }
        *(s16 *)(u + 0xA) = t;
        *(u32 *)(u + 0xC) = b[1];
        addPrim(r, u);
    }
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    SetDrawArea(q, b);
    addPrim(r, q);
}

/* One semi-transparent 1x1 tile and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad, with the tile's colour word
   copied off the payload at +0x8 and then faded: each of the three bytes is
   scaled by the payload's s16 life counter at +0x32 over 0x100. The twin is
   csrc/ovl_dungeon_800D50FC.c's func_800D5AC0 (120 words, resident), which is
   this function without the fade; the relink moves every scratchpad slot down
   by 4 -- the input SVECTOR to +0x00, the OT cursor to +0x18, the OT base to
   +0x20, RotTransPers' two out-parameters to +0x90/+0x94 and the OT index to
   +0xC0 -- so the struct is this unit's own, and the input vector is the
   scratchpad base itself, which is why $a0 is a bare register copy.

   THE FADE REPLACES THE TWIN'S DEAD setRGB0 ECHO. func_800D5AC0 needs
   setRGB0(t, t->r0, t->g0, t->b0) to keep three loads whose stores cse
   strips; here the three lbu/mult/sb runs are the real work and the echo must
   NOT be written -- added in front of the tag setup it is 151 words and 76
   differing.

   `/ 0x100' AND NOT `>> 8': the bgez/addiu 0xFF pair ahead of each sra is the
   signed division's correction, and `>> 8' drops it -- 139 words, 102
   differing.

   THE SCALE FACTOR IS RE-READ AT ALL THREE CHANNELS. A local for
   *(s16 *)(p + 0x32) is 145 words and 109 differing; the target has three
   separate `lh 0x32($s3)'.

   Three orderings, every one re-measured here rather than taken from the
   twin: the colour word and its three scalings come BEFORE the tag setup
   (setTile1 first is 147 words and 111 differing); `sp->unk_18' is assigned
   BEFORE `sp->unk_20' even though the target STORES them the other way round
   (swapped, 11 differing, against the twin's 20 for the same pair); and
   `p = (u8 *)n + 0x20;' comes before `v = n->unk_08;' (swapped, 17
   differing, against the twin's 4).

   setTile1(t) plus setSemiTrans(t, 1) is the whole `sb 2, 0x3' /
   `sb 0x6A, 0x7' pair: cse folds the getcode back to the 0x68 setTile1 just
   stored and drops the dead store. `(SVECTOR *)&sp->unk_00' written for
   `(SVECTOR *)sp' is a tie at 0.

   148 words, 0 differing. raw 7 == relocdump's 7, none on a function symbol:
   1 HI16 + 2 LO16 on D_80083160, R_MIPS_26 on RotTransPers, GetTPage and
   func_80067F20, and one on .text for the loop's own backward `j'. The
   +0x90/+0x94 addresses that splat auto-names D_1F800090/D_1F800094 carry no
   relocation on this side and come out byte-identical, so seqcheck counts one
   extra entry in the target and reports seq=FAIL on that bookkeeping alone. */
s32 func_s37_80024C54(u8 *p, UnkVec *v)
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
            *(u32 *)&t->r0 = *(u32 *)(p + 0x8);
            t->r0 = t->r0 * *(s16 *)(p + 0x32) / 0x100;
            t->g0 = t->g0 * *(s16 *)(p + 0x32) / 0x100;
            t->b0 = t->b0 * *(s16 *)(p + 0x32) / 0x100;
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

/* A random sign: rand() & 1, with the zero case turned into -1. 12 words,
   0 differing. ONE LOCAL CARRIES BOTH VALUES, not two `return's: the target
   reuses the `andi' result on the taken path and only materialises
   `addiu $v0, $zero, -0x1' on the fall-through. As
   `if ((rand() & 1) == 0) return -1; return 1;' it is 12 words and 4
   differing -- the taken path gets its own `li 1'. 0x80069EF8 is `rand' in
   splat/common_syms.txt, so the call needs no declaration.
   raw 1 == relocdump's 1, the R_MIPS_26 on rand. */
s32 func_s37_80024EA4(void)
{
    s32 r;

    r = rand() & 1;
    if (r == 0)
    {
        r = -1;
    }
    return r;
}

/* One frame of the 0x211 spark func_s37_80024FA0 spawns: pin D_80026428 to 1,
   and while the mover's +0xA is still below the ground height func_800BCB04
   reports two units above it, climb +0x8 by 0x20000 plus a fresh
   rand() & 0xFFF; the payload's +0x32 countdown falls by 8 a frame and raises
   the node's redraw bit and D_800814A0's at zero. 51 words, 0 differing.

   THE THREE ARGUMENT LOADS ARE THREE LOCALS, ASSIGNED BEFORE THE D_80026428
   STORE, IN THE ORDER +0xA, +0x2, +0x6. Written with the loads inside the
   call it is 51 words and 6 differing, every row the `li 1' / `sh' pair and
   the three `lhu's trading places: gcc 2.7.2 cannot disambiguate the
   `mem:HI (lo_sum D_80026428)' store from `mem:HI ($s0 + 10)', so each
   argument load carries the store in its dependency list, and calls.c emits
   a call's argument register loads at the very END of its expansion -- the
   only way to get them above the store is to evaluate them in an earlier
   statement. The shape is csrc/ovl_dng_s18.c's func_s18_80024FF0, which
   documents it for the 48-word relative of this family; the bearing test
   itself is csrc/ovl_dungeon_800CABA8.c's func_800CAED0 (67 words) with a
   one-axis rand() climb in place of its three-axis glide and 8 for its 4.
   raw 7 == relocdump's 7: 2 on D_80026428, 3 on D_800814A0, 2 R_MIPS_26. */
void func_s37_80024ED4(u8 *p, UnkVec *v)
{
    s32 x;
    s32 y;
    s32 z;
    s16 r;

    z = *(u16 *)((u8 *)v + 0xA);
    x = *(u16 *)((u8 *)v + 0x2);
    y = *(u16 *)((u8 *)v + 0x6);
    D_80026428 = 1;
    r = func_800BCB04(x, y, z + 2);
    if (*(s16 *)((u8 *)v + 0xA) < r)
    {
        v->unk_08 += 0x20000 + (rand() & 0xFFF);
    }
    if ((*(s16 *)(p + 0x32) -= 8) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* A 0x211 spawner: seed the new node's mover from the parent `a`'s, jitter
   each axis by a fresh rand() & 0x1F and the caller's own offset
   less 0x10, then park func_s37_80024ED4 as the step and
   func_s37_80024C54 as the draw, with `b` at the payload's +0x14, `d` at
   +0x32 and `c` at +0x8.  72 words, 0 differing.  The twin is
   csrc/ovl_dungeon_800D50FC.c's func_800D6AD4 -- the same 0x211 spawner
   without the jitter.

   THE THREE STACK OFFSETS ARE `s16` AND THE SUBTRACTION WEARS AN `(s16)`
   CAST, and both halves of that are load-bearing:

   * `s32 dx/dy/dz` is 66 words and 71 differing.  An `s32` stack parameter
     keeps its incoming MEM as its rtl and is rematerialised with a
     `lw 0x48($sp)` at its single use; the promoted `s16` goes through a
     pseudo, which register allocation then parks in $s2/$s3/$s4.  The six
     missing words are those three registers' save/restore pairs.  (Same
     mechanism, opposite sign, as func_800D6AD4's own note.)
   * `s16` with a bare `(dx - 0x10)` is 74 words and 20 aligned differences:
     fold's associate step splits `(X + jitter) + (dx + -0x10)` into
     `(X + jitter + dx) + -0x10` and the `addiu` lands on the running sum
     instead of on the parameter.  The NOP_EXPR the cast leaves blocks that
     split, which is the whole of the remaining 20 rows -- with it the
     `andi` falls into the `lhu`'s load-delay slot and the destination's
     `lw 0x8($s0)` moves ahead of the first `addu` on its own. */
void func_s37_80024FA0(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s37_80024ED4;
        *(s16 *)((u8 *)n->unk_08 + 0x2) =
            *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x1F) + (s16)(dx - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0x6) =
            *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x1F) + (s16)(dy - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0xA) =
            *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x1F) + (s16)(dz - 0x10);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = d;
        func_8004491C(n, (u32)func_s37_80024C54);
        *(u32 *)(p + 0x8) = c;
    }
}

/* A 0x212 spark's step handler: ramps the renderable's three colour bytes
   from the spark's +0x4 age against its +0xA lifetime, ticks the age, runs
   the +0x1E four-phase wobble on the +0x28/+0x29 screen offsets, and retires
   the node once the age reaches the +0x2 limit.
   76 words, 0 differing, 0 aligned differences.

   THE THIRD PARAMETER IS THE RENDERABLE AND THE SECOND IS UNUSED. The
   listing clobbers $a1 before reading it and writes through $a2, so the
   arity is three: this is the (spawn, vec, renderable) handler shape that
   csrc/ovl_town.c:19042 calls through `n->unk_10'. func_s37_800255B4
   installs this body at +0x10 of a 0x212 node.

   IT OWNS A JUMP TABLE AND THE TABLE IS VERIFIED POSITIONALLY.
   jtbl_s37_80024008, 8 `.word's = 8 cases + 0 pad (tools/decomp/jtcheck.py),
   dispatch `sltiu $v0, $v1, 0x8' with NO minval subtraction, so minval is 0
   and the case set is 0..7. Three distinct targets, and -- unlike the usual
   shape -- NONE of them is the range check's `beqz' target: that goes to
   .Ls37_800251B0, the shared tail, which appears in no entry. So all EIGHT
   entries are real arms and NOT ONE IS PAD; the three targets are genuine
   arm sharing, with cases 0,1,2,4,5,6 on one arm. There is no `default:'.
   Written as one six-label arm, then case 3, then case 7 -- which is entry
   ADDRESS order (80025150 < 8002516C < 80025194). Confirmed by a standalone
   `tools/cdkgcc -O2 -G0 -S' compile and substitution rather than by the
   verifier, which cannot see .rodata:
     emitted [$L76,$L76,$L76,$L77,$L76,$L76,$L76,$L78] with block addresses
     $L76 < $L77 < $L78, so it resolves to the ROM's entry sequence exactly.

   FOUR MEASURED SPELLINGS, every one priced by aligndiff against the LANDED
   body (not mid-fight), each a single ablation off the body below.

   * THE NUMERATOR AND THE QUOTIENT MUST BE TWO DIFFERENT LOCALS. This is
     the whole fight and it is worth 4 rows. `n = a * 48' in each arm and
     then `t = n / h' once lands at 0; writing `t = a * 48 / h' in both arms
     is 76 words / 4 rows, and `t = a * 48' then `t = t / h' is 76 / 6.
     The mechanism, read out of -da and not guessed: `divmodsi4' is a
     PARALLEL that also sets an unused `mod' output. When the quotient is a
     user variable assigned in BOTH arms it is a cross-block pseudo, so
     local-alloc's `combine_regs' will not tie it to the dying dividend
     ("not local to this block"), and ties the throwaway `mod' output to it
     instead; that steals $v0 and the quotient takes $v1. With a separate
     numerator the quotient has ONE set in the merge block, the tie goes to
     it, and it gets the dividend's $v0 -- which is what the ROM has at
     `div $zero, $v0, $a3' / `mflo $v0'. .greg confirms it on the 4-row
     body: the quotient is in the global set with hard reg 2 in its conflict
     list, because the mod output had already taken $v0 locally.

   * THE TWO EARLY READS ARE `s32' LOCALS, NOT `s16'. `s16 a' / `s16 h' is
     82 words / 45 rows -- six words of `sll 16'/`sra 16' re-extension plus
     an extra parameter copy. The target loads both with `lh' straight into
     $a1/$a3 and uses them unextended.

   * BOTH READS MUST COME BEFORE THE `D_80026428 = 1' STORE. Written after
     it the body is 77 words / 5 rows: the two `lh's cannot be scheduled up
     past the store, because the store is a plain cast at a fixed address
     and so are the loads, so `true_dependence' refuses and the extra word
     is the `nop' the target does not have. (This is the same dependence
     this unit's func_s37_800251F0 solves with an ARRAY REF to set
     MEM_IN_STRUCT_P; here plain source ORDER does it, and the two are
     alternatives for one mechanism.)

   * THE THREE COLOUR STORES ARE DESCENDING, 0xE then 0xD then 0xC. Written
     ascending they emit ascending: 76 words / 4 rows. sched1 did NOT
     transpose them here, so for this triple the listing's store order IS
     the source's -- the opposite of the usual caution, which is why it is
     recorded rather than assumed. */
void func_s37_800250C0(u8 *p, UnkVec *v, u8 *q)
{
    s32 a;
    s32 h;
    s32 n;
    s32 t;

    a = *(s16 *)(p + 0x4);
    h = *(s16 *)(p + 0xA);
    D_80026428 = 1;
    if (a < h)
    {
        n = a * 48;
    }
    else
    {
        n = (*(s16 *)(p + 0x2) - a) * 48;
    }
    t = n / h;
    *(u8 *)(q + 0xE) = t;
    *(u8 *)(q + 0xD) = t;
    *(u8 *)(q + 0xC) = t;
    (*(u16 *)(p + 0x4))++;
    switch (*(s16 *)(p + 0x1E))
    {
    case 0:
    case 1:
    case 2:
    case 4:
    case 5:
    case 6:
        (*(u16 *)(p + 0x1E))++;
        *(u8 *)(p + 0x28) += 0x10;
        break;
    case 3:
        (*(u16 *)(p + 0x1E))++;
        *(u8 *)(p + 0x28) -= 0x30;
        *(u8 *)(p + 0x29) += 0x20;
        break;
    case 7:
        *(s16 *)(p + 0x1E) = 0;
        *(u8 *)(p + 0x28) -= 0x30;
        *(u8 *)(p + 0x29) -= 0x20;
        break;
    }
    if (*(s16 *)(p + 0x4) >= *(s16 *)(p + 0x2))
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x212 shockwave's step handler: counts the +0x2 life down and the
   +0x8C spin up by 0xC8, advances the +0xA/+0xC two-stage ring radius
   (clamped at 0x20), fattens every byte of the +0x50 radius profile by 2,
   runs the same +0x1E/+0x28/+0x29 wobble as func_s37_800250C0, and retires
   the node when the life reaches 0. func_s37_800255B4's case 3 installs it.
   92 words, 0 differing, 0 aligned differences.

   IT OWNS A JUMP TABLE, THE SAME SHAPE AS func_s37_800250C0'S AND VERIFIED
   THE SAME WAY. jtbl_s37_80024028, 8 `.word's = 8 cases + 0 pad, dispatch
   `sltiu $v0, $v1, 0x8' with no minval subtraction so the set is 0..7, three
   distinct targets, and again NONE of them is the range check's `beqz'
   target (.Ls37_80025328, the shared tail, is in no entry). All EIGHT
   entries are real arms, no pad, no `default:', cases 0,1,2,4,5,6 sharing
   one arm, written in entry-ADDRESS order (800252C8 < 800252E4 < 8002530C).
   Standalone `tools/cdkgcc -O2 -G0 -S' substitution:
     emitted [$L97,$L97,$L97,$L98,$L97,$L97,$L97,$L99], block addresses
     $L97 < $L98 < $L99, resolving to the ROM's entry sequence exactly.
   The switch arms and the tail are byte-identical to func_s37_800250C0's,
   so landing that one first gave this body its whole second half.

   TWO MEASURED SPELLINGS, both priced by aligndiff off the landed body.

   * THE +0x2 DECREMENT IS AN ARRAY REF, `((u16 *)p)[1]--'. THIS IS THE
     WHOLE FIGHT and it is worth a word and 42 rows. As the plain cast
     `(*(u16 *)(p + 0x2))--' the body is 91 words / 42 aligned differences,
     and the residue is a WHOLE-BODY register exchange -- `p' in $a0 and the
     loop counter in $a1, where the target has them the other way round and
     keeps an `addu $a1, $a0, $zero' entry copy this body does not emit.
     The mechanism, measured by compiling both and confirmed in global.c:
     an ARRAY_REF sets MEM_IN_STRUCT_P, so gcc's fixed-scalar-versus-varying
     -struct rule lets the `lhu 0x2' hoist ABOVE the `sh %lo(D_80026428)'
     store beside it. That hoist is what puts a third value live at the top
     of the function, so local-alloc spends $a0 on the `%hi(D_80026428)'
     address, which makes $a0 conflict with `p' -- and then
     `prune_preferences' (global.c) cannot give `p' its preferred $a0, the
     entry copy survives, and the counter takes $a0. As a plain cast neither
     memref is in-struct, the hoist is illegal, `p' keeps $a0, and every
     later row shifts. This is the same dependence func_s37_800250C0 settles
     by plain statement ORDER rather than by an array ref; the two spellings
     are alternatives for one mechanism and the choice is per body.

   * THE PROFILE INDEX IS `*(u8 *)(p + i + 0x50)', NOT `p[0x50 + i]'. The
     subscript form is 92 words / 2 rows, both of them the two
     `addu $v0, $a1, $a0' sites coming out `$a0, $a1': `pointer_int_sum'
     always builds PLUS(ptr, intop), so the subscript spelling can only emit
     base-first. This re-measures the sibling func_s37_80024094's note on
     the SAME +0x50 array (it prices its single site at 1 row); the fact
     carries, the magnitude is per site count.

   Note the loop reloads `*(s16 *)(p + 0xC)' every iteration and the target
   does too, so the bound is NOT hoisted into a local -- a local there would
   make the body shorter than the target. */
void func_s37_800251F0(u8 *p, UnkVec *v, u8 *q)
{
    s32 i;

    D_80026428 = 1;
    ((u16 *)p)[1]--;
    *(u16 *)(p + 0x8C) += 0xC8;
    (*(u16 *)(p + 0xA))++;
    if (*(s16 *)(p + 0xA) >= 2)
    {
        *(s16 *)(p + 0xA) = 0;
        (*(u16 *)(p + 0xC))++;
    }
    if (*(s16 *)(p + 0xC) > 0x20)
    {
        *(s16 *)(p + 0xC) = 0x20;
    }
    for (i = 0; i < *(s16 *)(p + 0xC); i++)
    {
        *(u8 *)(p + i + 0x50) += 2;
    }
    switch (*(s16 *)(p + 0x1E))
    {
    case 0:
    case 1:
    case 2:
    case 4:
    case 5:
    case 6:
        (*(u16 *)(p + 0x1E))++;
        *(u8 *)(p + 0x28) += 0x10;
        break;
    case 3:
        (*(u16 *)(p + 0x1E))++;
        *(u8 *)(p + 0x28) -= 0x30;
        *(u8 *)(p + 0x29) += 0x20;
        break;
    case 7:
        *(s16 *)(p + 0x1E) = 0;
        *(u8 *)(p + 0x28) -= 0x30;
        *(u8 *)(p + 0x29) -= 0x20;
        break;
    }
    if (*(s16 *)(p + 0x2) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x212 flash's step handler, two states deep: state 0 clears the
   renderable's 0x80 bit and falls in with state 1, both then re-arm the two
   screen rects and hand them to func_s37_80024A5C, ramp the renderable's
   three colour bytes up over the first 0x15 frames and back down over the
   last 0x14 of 0x64, and on the 0x65th frame bump the state, poke +0x9C on
   the +0x40 partner and retire the node. func_s37_800255B4's case 3
   installs it, passing itself at +0x40 and the shockwave node at +0x48.
   149 words, 0 differing, 0 aligned differences.

   THIS BODY DELETED A COMMITTED FILE-SCOPE OBJECT -- SAY SO LOUDLY.
   `const u8 D_s37_80024048[]' used to sit between the stubs here as a
   hand-written stand-in. It is NOT bank data: it is cc1psx's own anonymous
   initialiser blob for this body's `s16 a[4] = { 0x340, 0x100, 0x60, 0x54 }'
   local. Those four halfwords ARE its eight bytes, 40 03 00 01 60 00 54 00,
   and a standalone `tools/cdkgcc -O2 -G0 -S' of the unit emits them as
   `$LC0: .align 2 / .half 832, 256, 96, 84' immediately before this
   function -- i.e. after jtbl_s37_80024028 and before D_s37_80024050, which
   is exactly 0x80024048. So the array had to go: keeping both would put
   sixteen bytes where the ROM has eight and shift every later rodata
   object. With it gone the unit's emitted .rodata is, in file order,
   jtbl_80024008 / jtbl_80024028 / $LC0 / D_s37_80024050 / jtbl_80024070 --
   the ROM's layout exactly, with the ROM's alignments (3/3/2/2/3).
   CONSEQUENCE FOR relcand: it reports "DATA in the listing, not in your
   code: D_s37_80024048" on this body, because splat named the blob and an
   anonymous compiler label has no name to match. That mismatch is expected
   and benign; `--raw' shows the site is still a `lui'/`addiu' RELOCATION
   pair and not a `lui'/`ori' constant. It is the one thing here that only
   `make containers' can finally confirm.

   THE 8-BYTE COPY IS A LOCAL AGGREGATE INITIALISER, NOT A STRUCT ASSIGNMENT.
   The `lwl'/`lwr' + `swl'/`swr' pair at the top is gcc's unaligned block
   move for `s16 a[4] = {...}' -- 2-byte alignment is under a word, so it
   cannot use `lw'/`sw'. Note the initialiser is then fully OVERWRITTEN by
   the four `a[...] =' stores below and the ROM still performs the copy: the
   array's address escapes into func_s37_80024A5C, so gcc may not delete it.
   Do not "clean that up"; the dead copy is four of the 149 words.

   TWO MEASURED SPELLINGS, both priced by aligndiff off the landed body.

   * THE TWO-STATE DISPATCH IS A `switch', NOT AN `if'/`else if'. This is
     the whole fight. Written as
     `if (s == 0) { ... } else if (s != 1) { return; }' the body is 148 words
     / 10 aligned differences: gcc emits `bnez' first and puts the state-0
     arm inline, where the ROM branches `beqz' PAST the range tests and puts
     that arm after them. With two cases and a `default: return', gcc 2.7.2
     is under `case_values_threshold' so `expand_end_case' emits a
     COMPARISON TREE rather than a table -- and a comparison tree emits its
     arms AFTER the tests, which is the ROM's block order and supplies the
     missing word. `case 1:' is an empty `break' and `default:' is the
     `return'.

   * BOTH COLOUR RAMPS STORE DESCENDING, 0xE then 0xD then 0xC. Flipping the
     SECOND ramp to ascending is 149 words / 2 rows -- the first and last
     `sb' of that ramp, with nothing else moving. I expected this to be free
     (each store has its own re-read and division between it and the next,
     so there is nothing adjacent for sched1 to transpose) and it is not, so
     it is recorded as measured rather than reasoned. The same descending
     order is worth 4 rows on func_s37_800250C0's single triple.

   The `* 128 / 20' in both ramps is a literal divisor: `expand_divmod'
   strength-reduces it at expand time, which is where the 0x66666667 magic
   multiply and `sra 3' come from, and each of the three stores re-reads
   `*(s16 *)(p + 0x2)' because the `sb' between them invalidates cse's
   entry. */
void func_s37_80025360(u8 *p, UnkVec *v, u8 *q)
{
    s16 a[4] = {0x340, 0x100, 0x60, 0x54};
    s16 b[2];

    D_80026428 = 1;
    switch (*(s16 *)p)
    {
    case 0:
        (*(u16 *)p)++;
        *(u16 *)(q + 0x14) &= 0xFF7F;
        break;
    case 1:
        break;
    default:
        return;
    }
    a[0] = 0x340;
    a[1] = 0x100;
    a[2] = 0x60;
    a[3] = 0x40;
    b[0] = 0x370;
    b[1] = 0x120;
    func_s37_80024A5C(*(u8 **)(p + 0x48), (s32 *)a, (s32 *)b, 1, 1);
    if (*(s16 *)(p + 0x2) < 0x15)
    {
        *(u8 *)(q + 0xE) = *(s16 *)(p + 0x2) * 128 / 20;
        *(u8 *)(q + 0xD) = *(s16 *)(p + 0x2) * 128 / 20;
        *(u8 *)(q + 0xC) = *(s16 *)(p + 0x2) * 128 / 20;
    }
    if (*(s16 *)(p + 0x2) > 0x50)
    {
        *(u8 *)(q + 0xE) = (0x64 - *(s16 *)(p + 0x2)) * 128 / 20;
        *(u8 *)(q + 0xD) = (0x64 - *(s16 *)(p + 0x2)) * 128 / 20;
        *(u8 *)(q + 0xC) = (0x64 - *(s16 *)(p + 0x2)) * 128 / 20;
    }
    (*(u16 *)(p + 0x2))++;
    if (*(s16 *)(p + 0x2) >= 0x65)
    {
        *(s16 *)(p + 0x2) = 0;
        (*(u16 *)p)++;
        *(s16 *)(*(u8 **)(p + 0x40) + 0x9C) = 1;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* 0x8 bytes of this bank's own rodata, between its jump tables,
   read by func_s37_80025360. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s37", func_s37_800255B4);
