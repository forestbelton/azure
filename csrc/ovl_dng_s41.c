#include "common.h"
extern u8 D_s41_80024004[];
extern u8 D_s41_8002400C[];
extern u16 D_80026820[];

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
void func_s41_80024FB4(u8 *p, UnkVec *v);

void func_s41_80025294(u8 *p);
typedef struct UnkDngS58Blk32
{
    u8 b[0x20];
} UnkDngS58Blk32;
typedef struct UnkDngS58Rec
{
    s16 a;
    u16 b;
} UnkDngS58Rec;
typedef struct UnkDngS58Blk64
{
    u8 b[0x40];
} UnkDngS58Blk64;
typedef struct UnkDngS58Name12
{
    u8 b[0xC];
} UnkDngS58Name12;
extern s16 D_800267B8;

UnkNode *func_8003FD64(s32, void *);

s32 func_800BCB04(s32, s32, s16);
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

void func_s41_80024190(u8 *a, u8 *pp, u8 *ot, s16 d);

void func_80067EF4(void *, s32, s32);
extern Unk6CD30 D_8006CD10;
s32 func_800644B8(s32);
s32 func_8009D218(u8 *, s32, u8 *);
u16 func_800A6D30(void);
void *func_800990FC(void);
u8 *func_8009929C(s32, u8 *);
void *func_80099194(u8 *, u8 *);
void *func_80099734(u8 *, u8 *);
u8 *func_80099290(u8 *);
s32 func_800A5720(void *);
void func_80099844(u8 *, u8 *);
UnkNode *func_800B4C7C(u16, u8 *, s16, s16);
extern u8 D_800E20D6[];
extern u8 D_800E20FC[];

/* The slot's "use the item on me" responder, reached from
   func_s41_800257D0's state 5: refuse if func_8009D218 does, then on a
   1-in-p[0x3] roll (or with the caller's strength at 0xFF) spend half the
   remaining 0x27 charges and print the three-part D_800E20D6 message,
   otherwise print D_800E20FC, and tell func_800B4C7C 0x53 either way.
   77 words, 0 differing, 0 aligned differences.

   func_8009D218 TAKES THE THIRD PARAMETER STRAIGHT THROUGH. Nothing in
   this body sets $a2 and nothing reads it either; the caller at
   0x8002661C passes `g' there and the call inherits it, so the arity is
   three and `q' is otherwise unused. A two-argument alias is 1 word short.

   THE DIVIDEND IS AN `s32' LOCAL AND THE TRUNCATION IS WRITTEN AT THE
   DIVISION: `x = (u16)r % p[0x3]'. With `u16 r' and a plain `r % p[0x3]'
   it is 76 words and 5 aligned differences -- the divisor's extra `addu'
   never appears, because the HImode pseudo dies at the zero-extend and the
   remainder reuses its register. The compound form
   `x = func_800A6D30(); x %= p[0x3];' is the right 77 words and 3 rows:
   expand_divmod drops a target mentioned in op0, so the remainder lands in
   the dividend's own register where the target gives it a third. The
   divisor guard is a real source-level `if (p[0x3] != 0)', not the
   assembler's div-by-zero expansion: the zero arm sets the remainder to 0
   and jumps, where a `break 7' would trap. Reading p[0x3] once into a
   local measures the same 0; the two reads are not what buys the copy.

   The message-assembly chain is csrc/ovl_dng_s18.c's func_s18_800240B8's,
   with func_8009929C(8, h) in place of its first func_80099194 -- two
   locals, `h' chained and `m' kept for func_800A5720. */
void func_s41_8002405C(u8 *p, u8 b, u8 *q)
{
    void *m;
    void *h;
    s32 r;
    s32 x;
    s16 n;

    if (func_8009D218(p, 2, q) != 0)
    {
        return;
    }
    r = func_800A6D30();
    if (p[0x3] != 0)
    {
        x = (u16)r % p[0x3];
    }
    else
    {
        x = 0;
    }
    if (x < (b << 5) || b == 0xFF)
    {
        if (p[0x27] >= 2)
        {
            n = (p[0x27] + 1) >> 1;
            p[0x27] -= n;
            h = func_800990FC();
            m = h;
            h = func_8009929C(8, h);
            h = func_80099734(p, h);
            h = func_80099194(D_800E20D6, h);
            func_80099290(h);
            func_800A5720(m);
            func_800B4C7C(0x53, p, n, 1);
            return;
        }
    }
    func_80099844(p, D_800E20FC);
    func_800B4C7C(0x53, p, -1, 1);
}

/* This object's room renderer: one POLY_FT4 per cell of the slot's +0x8
   cell list, projected through the 0x1F800000 scratchpad with
   RotTransPers4 and biased by the view's +0xB8/+0xBA screen offset, then
   re-emitted as EIGHT ROWS of TWO COLUMNS -- the left column copied from
   the base primitive with its four y corners taken from the wave table the
   payload keeps at +0x62 and its four v bytes bumped by i * 16 (and by
   (i - 4) * 16 past the fourth row), the right column copied from the
   primitive just emitted with its four x corners moved on by 0x40. A cell
   with 0x20 in its first byte is a handler instead. 631 words, 0
   differing, 0 aligned differences.

   TWIN: csrc/ovl_dng_s37.c's func_s37_80024094 (626 words, dngtwin 0.807).
   Its head is carried VERBATIM -- the two corner arms off e[0] ^ unk_24,
   RotTransPers4, the eight +0xB8/+0xBA stores, the two +0x100 carry
   corrections, the +0xE clut and +0x16 tpage arms, the two
   `q[0x14] = q[0x24]--' pairs, the +0xF shade byte and the 0x8D0
   writeback -- and so is its tail. All four of its measured spellings that
   still apply carried, including `t20'/`t22' and the two named halves of
   the +0x102 sum. WHAT DOES NOT CARRY IS ITS TWO POLY_FT4 STACK LOCALS:
   this frame is -0x60 where the twin's is -0xB8, so the row copies are
   memory to memory off two pointer locals and the ripple loop -- the whole
   func_800644B8 jitter -- is absent from this object. Its `-da' prologue
   test said as much before the first compile: ten saves in both, 0x58
   fewer bytes of locals here.

   TWO SPELLING FACTS OF ITS OWN, both measured by ablation off the landed
   body:

   * THE BASE PRIMITIVE'S y2/y3 SUM IS ONE NAMED LOCAL. Written twice --
     `q + 0x22' and `q + 0x1A' each from
     `*(u16 *)(q + 0xA) + *(u16 *)(pp + 0x64)' -- it is 633 words and 4
     aligned differences: the first store invalidates the `q + 0xA' load
     for cse, so the pair is re-loaded and re-added.
   * THE RIGHT COLUMN'S COPY SOURCE IS ITS OWN POINTER LOCAL, assigned
     from `q' BEFORE the addPrim that advances it. Spelled
     `*(POLY_FT4 *)q = *(POLY_FT4 *)(q - 0x28)' it is 3 words short and 56
     aligned differences, and the knock-on is that $s6 and $s7 exchange --
     pp against the 0xFF000000 mask -- across the whole body.

   raw 28 == relocdump's 28: 4 HI16 + 4 LO16 on D_80083160, 1 + 1 on
   D_8006CD10, 10 R_MIPS_26 on the library calls and 8 on .text. */
void func_s41_80024190(u8 *a, u8 *pp, u8 *ot, s16 d)
{
    Unk80083160 *g = &D_80083160;
    UnkDng24094SP *sp = (UnkDng24094SP *)0x1F800000;
    UnkSlot *s;
    UnkVec *v;
    u8 *e;
    u8 *q;
    u8 *b0;
    u8 *pv;
    s32 y0;
    s32 i;
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
            b0 = q;
            *(s16 *)(pp + 0x62) = *(u16 *)(q + 0xA);
            y0 = *(u16 *)(q + 0xA) + *(u16 *)(pp + 0x64);
            *(s16 *)(q + 0x22) = y0;
            *(s16 *)(q + 0x1A) = y0;
            addPrim(sp->unk_20, q);
            q += 0x28;
            q[0x3] = 9;
            *(POLY_FT4 *)q = *(POLY_FT4 *)b0;
            *(s16 *)(q + 0x8) = *(u16 *)(q + 0x8) + 0x40;
            *(s16 *)(q + 0x10) = *(u16 *)(q + 0x10) + 0x40;
            *(s16 *)(q + 0x18) = *(u16 *)(q + 0x18) + 0x40;
            *(s16 *)(q + 0x20) = *(u16 *)(q + 0x20) + 0x40;
            addPrim(sp->unk_20, q);
            q += 0x28;
            for (i = 1; i < 8; i++)
            {
                q[0x3] = 9;
                *(POLY_FT4 *)q = *(POLY_FT4 *)b0;
                *(s16 *)(q + 0xA) = *(u16 *)(pp + 0x62) + *(u16 *)(pp + i * 2 + 0x62);
                *(s16 *)(q + 0x12) = *(u16 *)(pp + 0x62) + *(u16 *)(pp + i * 2 + 0x62);
                *(s16 *)(q + 0x1A) = *(u16 *)(pp + 0x62) + *(u16 *)(pp + i * 2 + 0x64);
                *(s16 *)(q + 0x22) = *(u16 *)(pp + 0x62) + *(u16 *)(pp + i * 2 + 0x64);
                if (i < 4)
                {
                    q[0xD] += i * 16;
                    q[0x15] += i * 16;
                    q[0x1D] += i * 16;
                    q[0x25] += i * 16;
                }
                else
                {
                    q[0xD] += (i - 4) * 16;
                    q[0x15] += (i - 4) * 16;
                    q[0x1D] += (i - 4) * 16;
                    q[0x25] += (i - 4) * 16;
                }
                pv = q;
                addPrim(sp->unk_20, q);
                q += 0x28;
                q[0x3] = 9;
                *(POLY_FT4 *)q = *(POLY_FT4 *)pv;
                *(s16 *)(q + 0x8) = *(u16 *)(q + 0x8) + 0x40;
                *(s16 *)(q + 0x10) = *(u16 *)(q + 0x10) + 0x40;
                *(s16 *)(q + 0x18) = *(u16 *)(q + 0x18) + 0x40;
                *(s16 *)(q + 0x20) = *(u16 *)(q + 0x20) + 0x40;
                addPrim(sp->unk_20, q);
                q += 0x28;
            }
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
   func_s41_80024190 and the resident's `AddPrim' call replaced by libgpu's
   inline `addPrim' macro. 126 words, 0 differing, 0 aligned differences.

   TWO OF THE TWIN'S DEVICES HAVE TO BE REMOVED HERE, and that is most of the
   delta. Spelled the twin's way:
   * `do { ... } while (0)' around the func_s41_80024190 call is 126 words and
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
void func_s41_80024B6C(u8 *p, s32 *b, s32 *b2, s32 c, s16 e)
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
    func_s41_80024190((u8 *)w, p, r, e);
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
s32 func_s41_80024D64(u8 *p, UnkVec *v)
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

/* One frame of the 0x211 spark func_s37_80024FA0 spawns: pin D_800267B8 to 1,
   and while the mover's +0xA is still below the ground height func_800BCB04
   reports two units above it, climb +0x8 by 0x20000 plus a fresh
   rand() & 0xFFF; the payload's +0x32 countdown falls by 8 a frame and raises
   the node's redraw bit and D_800814A0's at zero. 51 words, 0 differing.

   THE THREE ARGUMENT LOADS ARE THREE LOCALS, ASSIGNED BEFORE THE D_800267B8
   STORE, IN THE ORDER +0xA, +0x2, +0x6. Written with the loads inside the
   call it is 51 words and 6 differing, every row the `li 1' / `sh' pair and
   the three `lhu's trading places: gcc 2.7.2 cannot disambiguate the
   `mem:HI (lo_sum D_800267B8)' store from `mem:HI ($s0 + 10)', so each
   argument load carries the store in its dependency list, and calls.c emits
   a call's argument register loads at the very END of its expansion -- the
   only way to get them above the store is to evaluate them in an earlier
   statement. The shape is csrc/ovl_dng_s18.c's func_s18_80024FF0, which
   documents it for the 48-word relative of this family; the bearing test
   itself is csrc/ovl_dungeon_800CABA8.c's func_800CAED0 (67 words) with a
   one-axis rand() climb in place of its three-axis glide and 8 for its 4.
   raw 7 == relocdump's 7: 2 on D_800267B8, 3 on D_800814A0, 2 R_MIPS_26. */
void func_s41_80024FB4(u8 *p, UnkVec *v)
{
    s32 x;
    s32 y;
    s32 z;
    s16 r;

    z = *(u16 *)((u8 *)v + 0xA);
    x = *(u16 *)((u8 *)v + 0x2);
    y = *(u16 *)((u8 *)v + 0x6);
    D_800267B8 = 1;
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
   less 0x10, then park func_s41_80024FB4 as the step and
   func_s41_80024D64 as the draw, with `b` at the payload's +0x14, `d` at
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
void func_s41_80025080(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s41_80024FB4;
        *(s16 *)((u8 *)n->unk_08 + 0x2) =
            *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x1F) + (s16)(dx - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0x6) =
            *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x1F) + (s16)(dy - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0xA) =
            *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x1F) + (s16)(dz - 0x10);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = d;
        func_8004491C(n, (u32)func_s41_80024D64);
        *(u32 *)(p + 0x8) = c;
    }
}

/* One frame of the shutter's wave: spend the payload's +0x2 counter, raise
   D_800267B8, seed the eight +0x62 wave slots with i * 16, then for each of
   the eight run its +0x50 angle through func_800644B8 and add the sine
   (>> 9) into the slot, advance the angle by 0x50 and wrap it at 0x1000,
   and when the counter runs out raise the node's 0x8000 kill bit and
   D_800814A0's. 61 words, 0 differing, 0 aligned differences.
   func_s41_800257D0 parks this as the step of the third node it spawns.

   THE +0x62 STORE COMES BEFORE THE +0x50 READ-MODIFY-WRITE, not after it.
   Written last in the loop body -- which is where the listing shows it,
   in the `bnez' delay slot -- it is 61 words and 11 aligned differences:
   the scheduler then emits the whole load/add pair for +0x50 ahead of the
   one for +0x62, and the three quantities rotate registers behind it
   ($a1/$v1/$a0 for the target's $v1/$a0/$v0). reorg sinks the store into
   the delay slot by itself.

   Both loops are written over `i' with the byte offsets inside the
   subscript -- `*(s16 *)(p + i * 2 + 0x62)' -- so loop.c shares one
   induction variable at p + i * 2 between the +0x50 and +0x62 arrays and
   keeps both displacements in the instructions. The counter read before
   the D_800267B8 store is func_s41_80025294's documented idiom, re-measured
   here: it is also what lets the constant 1 serve both the store and `i'.
   raw 6 of 7 relocations: 1 HI16 + 1 LO16 on D_800267B8, 1 + 2 on
   D_800814A0, 1 R_MIPS_26 on func_800644B8. */
void func_s41_800251A0(u8 *p)
{
    s32 i;
    s16 t;
    s16 w;
    s16 v;

    t = *(u16 *)(p + 0x2) - 1;
    D_800267B8 = 1;
    *(s16 *)(p + 0x2) = t;
    for (i = 1; i < 9; i++)
    {
        *(s16 *)(p + i * 2 + 0x62) = i * 16;
    }
    for (i = 1; i < 9; i++)
    {
        w = *(u16 *)(p + i * 2 + 0x62) + (func_800644B8(*(s16 *)(p + i * 2 + 0x50)) >> 9);
        *(s16 *)(p + i * 2 + 0x62) = w;
        v = *(u16 *)(p + i * 2 + 0x50) + 0x50;
        *(s16 *)(p + i * 2 + 0x50) = v;
        if (v > 0x1000)
        {
            *(s16 *)(p + i * 2 + 0x50) = v - 0x1000;
        }
    }
    if (*(s16 *)(p + 0x2) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spend 8 of the payload's +0x32 life counter, raise the engine's D_800267B8
   tick, and when the counter runs out set the 0x8000 kill bit in the node's
   -0x2 flags and in D_800814A0.  20 words, 0 differing.  The shape is
   func_800999D4 (csrc/ovl_dungeon.c) plus the D_800267B8 store.

   THE DECREMENT MUST BE READ INTO AN `s16` LOCAL *BEFORE* THE D_800267B8
   STORE.  Every folded spelling -- `if ((*(s16 *)(p + 0x32) -= 8) <= 0)`
   with the store first, the early-return form, the counter behind a
   `s16 *` local, a `do { } while (0)` around the store, and the
   decrement/test split into two statements -- is 20 words and 11 differing,
   and the same local written AFTER the store is 11 too.  The eleven rows are
   one allocation: with the load after the store, the constant 1 is the only
   block-local quantity alive over $v0, so local-alloc hands it $v0 and
   global-alloc then gives `p` its incoming $a0; reading the counter first
   keeps $v0/$v1 busy across the constant's range, so the constant takes $a0
   and `p` is copied to $a1 by the target's entry `addu`.  That also buys the
   schedule: `addiu $a0, $zero, 1` fills the `lhu`'s load-delay slot, where
   the store-first forms spend a `nop` there.  Hoisting the result into an
   `s32` instead loses the `sll 16`/`bgtz` pair: 19 words, 19 differing.
   THIS COPY'S OWN CONSTANT: 0x32 -> 0x2 at all 2 sites, -0x8 -> -0x1.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s41_80025294(u8 *p)
{
    s16 t;

    t = *(s16 *)(p + 0x2) - 1;
    D_800267B8 = 1;
    *(s16 *)(p + 0x2) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The shutter's second node, stepped once a frame: raise D_800267B8, and
   if the +0x3C object's +0x13 byte has reached 0x33..0x36 give up at once
   with both 0x8000 kill bits; otherwise spend the +0x2 counter, hand the
   renderable's 0x80 flag to the object at -0x14 of the +0x3C object's
   -0x14, and drive the renderable's three colour bytes off the counter --
   up from 0x20 over the first 0x14 frames and down again from 0x28 -- then
   raise both kill bits at zero and take the 0x80 flag back. 73 words,
   0 differing, 0 aligned differences. Its first gate is the same test
   func_s41_80025408's tail makes, and it is a RANGE TEST in the source:
   `n[0x13] >= 0x33 && n[0x13] <= 0x36' is what fold's make_range turns
   into the listing's `addiu -0x33' / `sltiu 4'.

   TWO FACTS, both priced:

   * THE TWO +0x14 FLAG WORDS ON THE SIBLING POINTER ARE ARRAY REFS,
     `((u16 *)m)[0xA]'. As `*(u16 *)(m + 0x14)' they are 6 words longer and
     16 aligned differences against this body's 6: expr.c sets
     MEM_IN_STRUCT_P only when the top node of the address is a PLUS_EXPR,
     and true_dependence's in-struct exception (sched.c) is what lets the
     D_800814A0 read-modify-write interleave with the flag clear in the
     tail. Through the cast the two chains are dependent and emit strictly
     one after the other, with a `nop' in each load delay.
   * THE TWO POINTER LOADS STRADDLE THE DECREMENT. `n2 = *(u8 **)(p + 0x3C)'
     goes BEFORE it and `m = *(u8 **)(n2 - 0x14)' AFTER it, with the
     renderable's flag clear last. The three other arrangements of those
     four statements are 4, 6 and 14 aligned differences -- the decrement's
     store blocks any hoist of a p-based load past it, so the first load
     has to be written above it and the second below.

   The fade is `0x20 + x * 0x60 / 0x14' at both ends, which is where the
   0x66666667 magic and its `sra 3' come from -- divisor 0x14, not the 0xA
   the same magic means with `sra 2'. Written as one arm with a shared
   value it loses the cross-jump; both arms spell out the three stores and
   jump2 merges their last two. */
void func_s41_800252E4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *n;
    u8 *n2;
    u8 *m;
    s16 t;
    s32 v;

    n = *(u8 **)(p + 0x3C);
    D_800267B8 = 1;
    if (n[0x13] >= 0x33 && n[0x13] <= 0x36)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return;
    }
    n2 = *(u8 **)(p + 0x3C);
    *(s16 *)(p + 0x2) = *(u16 *)(p + 0x2) - 1;
    m = *(u8 **)(n2 - 0x14);
    *(u16 *)(c + 0x14) &= 0xFF7F;
    ((u16 *)m)[0xA] |= 0x80;
    t = *(s16 *)(p + 0x2);
    if (t < 0x14)
    {
        v = (0x14 - t) * 0x60 / 0x14 + 0x20;
        c[0xC] = v;
        c[0xD] = v;
        c[0xE] = v;
    }
    else if (t < 0x28)
    {
        v = (t - 0x14) * 0x60 / 0x14 + 0x20;
        c[0xC] = v;
        c[0xD] = v;
        c[0xE] = v;
    }
    if (*(s16 *)(p + 0x2) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        ((u16 *)m)[0xA] &= 0xFF7F;
    }
}

void func_s41_80025408(u8 *p, UnkVec *b, u8 *c)
{
    RECT r1;
    RECT r2;
    DVECTOR w;
    u8 *n;
    u16 *q;
    s32 i;
    s32 j;
    s32 k;
    s32 t;
    int new_var;
    s16 u;

    r1 = *(RECT *)D_s41_80024004;
    new_var = 0x60;
    r2 = *(RECT *)D_s41_8002400C;
    t = *(s16 *)p;
    D_800267B8 = 1;
    switch (t)
    {
    case 0:
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                r2.x = i * 0x18 + 0x340;
                r2.y = j * 0x15 + 0x154;
                r2.w = 0x18;
                r2.h = 0x15;
                StoreImage(&r2, (u_long *)D_80026820);
                while (DrawSync(1) != 0)
                {
                    ;
                }
                q = D_80026820;
                for (k = 0; k < 0x1F8; k++)
                {
                    if (*q == 0)
                    {
                        *q = 0xFFFF;
                    }
                    else
                    {
                        *q = 0;
                    }
                    q++;
                }
                LoadImage(&r2, (u_long *)D_80026820);
            }
        }
        *(s16 *)p = *(u16 *)p + 1;
        *(u16 *)(c + 0x14) &= 0xFF7F;
    case 1:
        r2.x = 0x340;
        r2.y = 0x100;
        r2.w = 0x60;
        r2.h = 0x54;
        w.vx = 0x370;
        w.vy = 0x148;
        func_800B8FC8(*(u8 **)(p + 0x44), (s32 *)&r2, (s32 *)&w, 0, 1);
        r1.x = 0x340;
        r1.y = 0x100;
        k = new_var;
        r1.w = k;
        r1.h = 0x54;
        w.vx = 0x370;
        w.vy = 0x12A;
        func_s41_80024B6C(*(u8 **)(p + 0x48), (s32 *)&r1, (s32 *)&w, 1, 1);
        if (*(s16 *)(p + 0x2) < 0x15)
        {
            c[0xE] = *(s16 *)(p + 0x2) * 0x80 / 0x14;
            c[0xD] = *(s16 *)(p + 0x2) * 0x80 / 0x14;
            c[0xC] = *(s16 *)(p + 0x2) * 0x80 / 0x14;
        }
        if (*(s16 *)(p + 0x2) >= 0x51)
        {
            c[0xE] = (0x64 - *(s16 *)(p + 0x2)) * 0x80 / 0x14;
            c[0xD] = (0x64 - *(s16 *)(p + 0x2)) * 0x80 / 0x14;
            c[0xC] = (0x64 - *(s16 *)(p + 0x2)) * 0x80 / 0x14;
        }
        u = *(u16 *)(p + 0x2) + 1;
        *(s16 *)(p + 0x2) = u;
        if (u > 0x64)
        {
            *(s16 *)(p + 0x2) = 0;
            *(s16 *)p = *(u16 *)p + 1;
            *(s16 *)(*(u8 **)(p + 0x40) + 0x9C) = 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    n = *(u8 **)(p + 0x3C);
    if (n[0x13] >= 0x33 && n[0x13] <= 0x36)
    {
        *(u16 *)(c + 0x14) |= 0x80;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s41", func_s41_800257D0);
