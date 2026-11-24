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
UnkNode *func_8003FC64(s32);
void func_8003DB94(void *, void *, s32);
void func_s14_8002418C(u8 *, UnkVec *, u8 *);
extern u8 D_800DEDB0[];
extern u8 D_80045340[];
extern u8 D_800E3D68;
extern u8 *D_800E3D7C;

/* 75 words, 0 differing. Walks the resident actor ring at D_800E3D7C, and
   for each actor the cursor admits whose grid cell is within 2 of (x, y)
   calls the 0x800C87C4 report with a 0x10 chance -- 0xFF when D_800E3D68
   is saturated -- and a rank of (d >> 2) + 0x10 plus two random bits. */
void func_s14_80024060(s16 x, s16 y, u8 *c, u8 d)
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s14", func_s14_8002418C);

/* Spawn the 0x212 node that animates the hit flash at q: white renderable,
   the 0x7E0B sprite, the +0x10/+0x14 flag bits and a position jittered down
   by rand() % 0x10 under q's own. 70 words, 0 differing.

   THE ONE SPELLING FACT: `*(u8 **)((u8 *)n + 0x20) = v;' must be the FIRST
   statement inside the gate. Written third, after the +0x10 step pointer and
   the `m = n->unk_0C' load -- which is the order every landed sibling of this
   shape uses -- the two parameters exchange $s2 and $s3 and it is 9 aligned
   differences; nothing else in the body moves.

   The FIRST rand() call has its result discarded: it is a real call that the
   target makes and gcc cannot delete, so it is written as a bare `rand();'.
   Read as macro residue -- the x and y jitters collapsed to nothing and only
   the z one survived. */
void func_s14_8002425C(u8 *v, u8 *q)
{
    UnkNode *n;
    u8 *m;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        *(u8 **)((u8 *)n + 0x20) = v;
        n->unk_10 = (u32)func_s14_8002418C;
        m = (u8 *)n->unk_0C;
        *((u8 *)n + 0x24) = 0;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        func_8003DB94(m, D_800DEDB0, 0);
        *(s16 *)(m + 0x1E) = 0x1000;
        *(s16 *)(m + 0x1C) = 0x1000;
        *(s16 *)(m + 0x12) = 0x7E0B;
        *(u16 *)(m + 0x10) |= 0x60;
        *(u16 *)(m + 0x14) |= 0x10C;
        func_8004491C(n, (u32)D_80045340);
        m = (u8 *)n->unk_08;
        rand();
        *(s16 *)(m + 0x2) = *(u16 *)(q + 0x2);
        *(s16 *)(m + 0x6) = *(u16 *)(q + 0x6);
        *(s16 *)(m + 0xA) = *(u16 *)(q + 0xA) - 0x40 - rand() % 0x10;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s14", func_s14_80024374);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s14", func_s14_80024428);

/* The beam line: one semi-transparent LINE_G2 from the payload's +0x22/+0x26
   /+0x2A origin to its +0x3A/+0x3E/+0x42 endpoint, interpolated by the +0x10
   phase and coloured from a 15-entry table, projected through the 0x1F800000
   scratchpad and chained into D_80083160's +0x8D0 primitive ring with the
   addPrim macro. 259 words, 0 differing.

   THIS FUNCTION OWNS jtbl_s14_80024008, 15 entries: cases 0..6 share the
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
   come from. */
s32 func_s14_800246D8(u8 *p)
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
    sp->unk_64 = *(u16 *)(p + 0x22);
    sp->unk_66 = *(u16 *)(p + 0x26);
    sp->unk_68 = *(u16 *)(p + 0x2A);
    sp->unk_6C = *(u16 *)(p + 0x3A);
    sp->unk_6E = *(u16 *)(p + 0x3E);
    sp->unk_70 = *(u16 *)(p + 0x42);
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s14", func_s14_80024AE4);
