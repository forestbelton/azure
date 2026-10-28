/* func_800345B8 (unk_72A8) -- 546 of 546 words, 128 DIFF. COMPILES.
 *
 * First attempt, 2026-10-27, written straight out of func_80033D54's park the
 * same day: this is the SAME FAMILY and most of the shapes transferred
 * unchanged. It is the model/billboard draw with a wobble: for each node on
 * the `UnkNode.unk_18` chain it projects the slot's origin with
 * RotTransPers, rejects it on depth, then repeats the element loop `nrep`
 * times (1, or 5 when the projected origin is within 0x330 of the screen
 * centre), offsetting the screen origin on each repeat by a table entry
 * scaled by `func_800644B8(func_80065F90(angle, 0x200))` and compositing a
 * different base matrix per repeat.
 *
 * WHAT IS LEFT is TWO ADJACENT PAIRS in global-alloc's priority order, and
 * the register names are nearly all of the 128 words. `advise.py --greg`:
 *
 *     the +1 giv (449): 63 refs / 313 live / 1.006   -> $s3, target $s2
 *     s          (82):  86 refs / 456 live / 1.132   -> $s2, target $s3
 *     m2         (87):   7 refs / 459 live / 0.031   -> SPILLED, target $s6
 *     y          (99):   8 refs / 405 live / 0.059   -> $fp, target SPILLED
 *
 * The giv needs ONE more reference to cross `floor_log2`'s step at 64 (6*64/313
 * = 1.227 beats s's 1.132) or a live range under 278; m2 needs 11 references
 * or 351 live. The target's frame is 0x58 against this build's 0x60 for
 * exactly that reason: it spills ONE value (`y`, as a HImode `sh`/`lhu` pair
 * at 0x28($sp)) where this spills two. Everything else in the diff --
 * the wobble's `sll`/`sra` placement, the prologue's save interleave -- is
 * downstream of those two swaps, so fix them first and re-read the diff.
 *
 * THE SHAPES THAT ARE PAID FOR. Each was measured on the way from 550 words
 * and 516 DIFF; the four marked (*) are the ones that moved it most.
 *
 *  - (*) THE +1 POINTER IS NOT IN THE SOURCE, exactly as in func_80033D54:
 *    the element fields are STRUCT FIELDS off `e` and loop.c reduces the
 *    address givs into one register that happens to be e+1. The flag byte at
 *    the bottom of the body must be `e->unk_01` and NOT `q[0]`: as `q[0]` the
 *    LAST giv recorded is the one at +4 instead, the group's representative
 *    becomes e+4, a THIRD pointer register appears, and the function is 512
 *    DIFF. The two sign bytes still go through `q` because they are volatile
 *    reads and a volatile MEM is not replaced by the reduced giv.
 *  - (*) D_8006CD4C, D_8006CD10 and D_8006A84C are INCOMPLETE ARRAYS indexed
 *    `[0]`. As scalars D_8006CD4C's store comes out as gas expanding a bare
 *    symbol through $at and the other two fold the %lo into the store's
 *    displacement; the listing has the compiler-split `lui`/`%lo` pair and a
 *    materialised address with a +0x1C displacement, which is what the
 *    incomplete array gives. See docs/addressing-and-translation-units.md.
 *  - (*) s->unk_20 and s->unk_22 go through LOCALS, because each is stored
 *    twice (the matrix translation at +0xDC/+0xE0 and the halfwords at
 *    +0x100/+0x102) and the first store invalidates cse's entry for the
 *    second read. Written twice from the field it is four words over.
 *  - (*) The flag byte's plain store is UNCONDITIONAL and the `i != 0` arm
 *    only ORs in the 2 bit, so the shared `sb` is what cross-jumping merges;
 *    with a store in each arm the whole block is a word out.
 *  - `nrep` is a `u16` and `i` an `s32`. That is what gives the listing's
 *    `beqz $fp` entry guard with a SIGNED `slt` at the bottom: a u16 promotes
 *    to a non-negative int, so fold rewrites `0 < nrep` to `nrep != 0`, while
 *    the comparison itself stays signed. As `u32` both the guard and the test
 *    are unsigned (`sltu`); as `s32` the guard is a `blez`.
 *  - `x` and `y` are `u16` locals too -- the listing spills `y` with `sh` and
 *    reloads it with `lhu`, which only a HImode pseudo does.
 *  - The table entry is INDEXED INLINE (`D_8006A83E[i] * func_800644B8(...)`)
 *    rather than hoisted into a pointer: gcc evaluates the call before the
 *    other operand, so the address materialises BETWEEN the two calls where
 *    the listing has it, and cse shares it with the second statement.
 *  - `abs()` from structs.h, not `if (v < 0) v = -v;` -- the MIPS `abssi2`
 *    pattern is one insn whose template carries its own `bgez`/`negu` and the
 *    `nop` in the middle (matching-levers section 1).
 *  - The wobble's divide is `/ 0x1000` on a signed value, which is where the
 *    `bgez`/`addiu 0xFFF`/`sra 12` triple comes from.
 *  - The final D_80083160 read is through the `g` pointer local (three words:
 *    `lui`/`addiu`/`lw 0`) while the prologue's is the symbolic
 *    `D_80083160.unk_00` (two words). Both forms appear in this one function,
 *    as they do in func_80044D24 and func_80033D54.
 *  - The prologue's OT pointer has its own short-lived local (`ot`), and
 *    arg0 is re-read from its stack home in the node-walk tail, which is what
 *    the listing's `lw $t0, 0x58($sp)` is -- the parameter is spilled, like
 *    `pl` in func_80033D54.
 *  - The matrix-source selection writes the `i != 0` body FIRST so the i==0
 *    arm is the branch target, which is the layout the listing has.
 *
 * NOT YET TRIED: anything that moves the two allocno pairs. The reference
 * trick that worked on func_80033D54 -- reading an existing store through the
 * pointer whose priority has to rise -- is available here too (`m2->t[0]`,
 * `m2->t[1]`), and it does give m2 the references, but it also moves those
 * two stores onto $s6 where the listing has them off $s1. Measured: 126 DIFF.
 *
 * AUDIT 2026-10-27 -- THIS PARK'S LIVE LENGTHS ARE DOUBLED, so any priority
 * or margin number above that involves pseudo 83, 84 is wrong, and any
 * "live <= N" instruction from the MARGINS table for them is unreachable.
 * `update_equiv_regs` (local-alloc.c) does `reg_live_length *= 2` for a once-set
 * pseudo whose init carries a constant REG_EQUAL note, and global-alloc reads the
 * doubled value -- so these pseudos report roughly twice their real range in a
 * 546-word function. See matching-levers section 3. Re-derive the arithmetic over
 * halved lengths before concluding anything about those pairs.
 */
#include "common.h"

extern s32 D_8006CD4C[];
extern Unk6CD30 D_8006CD10[];
extern Unk6CD30 D_8006A84C[];
extern s16 D_8006A83E[];
extern u8 D_8006A847[];

s32 func_80065F90(s32, s32);

typedef struct Unk345B8SP
{
    u8 pad_00[0x4];
    SVECTOR unk_04;
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    Unk3DBD0W unk_18;
    POLY_FT4 *unk_1C;
    u8 pad_20[0x4];
    s32 unk_24;
    u16 unk_28;
    u8 pad_2A[0x2A];
    MATRIX unk_54;
    s16 unk_74;
    s16 unk_76;
    s16 unk_78;
    u8 pad_7A[0x2];
    s16 unk_7C;
    s16 unk_7E;
    s16 unk_80;
    u8 pad_82[0x2];
    s16 unk_84;
    s16 unk_86;
    s16 unk_88;
    u8 pad_8A[0x2];
    s16 unk_8C;
    s16 unk_8E;
    s16 unk_90;
    u8 pad_92[0x2];
    s32 unk_94;
    s32 unk_98;
    u8 pad_9C[0x20];
    u16 unk_BC;
    u16 unk_BE;
    u8 pad_C0[0x4];
    s32 unk_C4;
    MATRIX unk_C8;
    u16 unk_E8;
    u16 unk_EA;
    u16 unk_EC;
    u16 unk_EE;
    u16 unk_F0;
    u16 unk_F2;
    u16 unk_F4;
    u16 unk_F6;
    SVECTOR unk_F8;
    u16 unk_100;
    u16 unk_102;
} Unk345B8SP;

s32 func_800345B8(u8 *pl, UnkVec *v, UnkSlot *s)
{
    Unk345B8SP *p = (Unk345B8SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    MATRIX *m1;
    MATRIX *m2;
    Unk6CD30 *m;
    u8 *base;
    POLY_FT4 *ot;
    POLY_FT4 *prim;
    Unk44D24E *e;
    UnkNode *n;
    u8 *q;
    s32 u;
    s32 w;
    u16 nrep;
    s32 i;
    u16 x;
    u16 y;
    s32 z;
    s32 x0;
    s32 x1;
    s32 x2;
    s32 x3;
    s32 t0;
    s32 t1;
    s32 t2;
    s32 t3;
    s32 t;
    u8 c;

    m2 = &p->unk_C8;
    base = D_80083160.unk_00;
    ot = *(POLY_FT4 **)(base + 0x8D0);
    p->unk_24 = (s32)(base + 0xB0);
    p->unk_C8.t[2] = 0;
    p->unk_1C = ot;
    for (;;)
    {
        p->unk_04.vx = ((u16 *)v)[1];
        p->unk_04.vy = ((u16 *)v)[3];
        p->unk_04.vz = ((u16 *)v)[5];
        p->unk_C4 = RotTransPers(&p->unk_04, (s32 *)&p->unk_BC, &p->unk_94, &p->unk_98);
        s->unk_14 |= 0x8000;
        z = p->unk_C4 << 2;
        D_8006CD4C[0] = z;
        D_8006CD10[0].unk_1C = z;
        D_8006A84C[0].unk_1C = z;
        p->unk_C4 = p->unk_C4 - (s16)s->unk_06;
        if ((u32)p->unk_C4 < 0x1E0)
        {
            PushMatrix();
            nrep = 1;
            x = p->unk_BC - 0xA0;
            p->unk_BC = x;
            y = p->unk_BE - 0x78;
            p->unk_BE = y;
            if (abs((s16)x) < 0x330 && abs((s16)y) < 0x330)
            {
                nrep = 5;
            }
            for (i = 0; i < nrep; i++)
            {
                if (i != 0)
                {
                    p->unk_C4 = 0xA;
                    p->unk_BC = x - D_8006A83E[i] *
                                        func_800644B8(func_80065F90((s16)x, 0x200)) /
                                        0x1000;
                    p->unk_BE = y - D_8006A83E[i] *
                                        func_800644B8(func_80065F90((s16)y, 0x200)) /
                                        0x1000;
                }
                p->unk_F8.vx = s->unk_16;
                p->unk_F8.vy = s->unk_18;
                p->unk_F8.vz = s->unk_1A;
                u = s->unk_20;
                p->unk_C8.t[0] = u;
                p->unk_100 = u;
                w = s->unk_22;
                p->unk_C8.t[1] = w;
                p->unk_102 = w;
                RotMatrix(&p->unk_F8, m2);
                if (i != 0)
                {
                    if (D_8006A847[i] != 0)
                    {
                        m = &D_8006A84C[0];
                    }
                    else
                    {
                        m = &D_8006CD10[0];
                    }
                }
                else
                {
                    m = &D_8006CD30;
                }
                CompMatrix((MATRIX *)m, m2, &p->unk_54);
                m1 = &p->unk_54;
                SetTransMatrix(m1);
                SetRotMatrix(m1);
                e = (Unk44D24E *)s->unk_08;
                p->unk_28 = s->unk_14;
                for (;;)
                {
                    if ((e->unk_00 & 0x20) == 0)
                    {
                        prim = p->unk_1C;
                        q = (u8 *)&e->unk_01;
                        p->unk_1C = prim + 1;
                        p->unk_0C.w = e->unk_08;
                        p->unk_10.w = e->unk_09;
                        p->unk_14.w = e->unk_0A;
                        p->unk_18.w = e->unk_0B;
                        if (((e->unk_00 ^ p->unk_28) & 1) != 0)
                        {
                            x0 = -(s8)((volatile u8 *)q)[1] - p->unk_100;
                            p->unk_84 = x0;
                            p->unk_74 = x0;
                            t0 = x0 - p->unk_14.h;
                            p->unk_8C = t0;
                            p->unk_7C = t0;
                        }
                        else
                        {
                            x1 = (s8)((volatile u8 *)q)[1] - p->unk_100;
                            p->unk_84 = x1;
                            p->unk_74 = x1;
                            t1 = x1 + p->unk_14.h;
                            p->unk_8C = t1;
                            p->unk_7C = t1;
                        }
                        if (((e->unk_00 ^ p->unk_28) & 2) != 0)
                        {
                            x2 = -(s8)((volatile u8 *)q)[2] - p->unk_102;
                            p->unk_7E = x2;
                            p->unk_76 = x2;
                            t2 = x2 - p->unk_18.h;
                            p->unk_8E = t2;
                            p->unk_86 = t2;
                        }
                        else
                        {
                            x3 = (s8)((volatile u8 *)q)[2] - p->unk_102;
                            p->unk_7E = x3;
                            p->unk_76 = x3;
                            t3 = x3 + p->unk_18.h;
                            p->unk_8E = t3;
                            p->unk_86 = t3;
                        }
                        p->unk_90 = 0;
                        p->unk_88 = 0;
                        p->unk_80 = 0;
                        p->unk_78 = 0;
                        RotTransPers4((SVECTOR *)&p->unk_74, (SVECTOR *)&p->unk_7C,
                                      (SVECTOR *)&p->unk_84, (SVECTOR *)&p->unk_8C,
                                      (s32 *)&p->unk_E8, (s32 *)&p->unk_EC,
                                      (s32 *)&p->unk_F0, (s32 *)&p->unk_F4,
                                      &p->unk_94, &p->unk_98);
                        prim->x0 = p->unk_E8 + p->unk_BC;
                        prim->y0 = p->unk_EA + p->unk_BE;
                        prim->x1 = p->unk_EC + p->unk_BC;
                        prim->y1 = p->unk_EE + p->unk_BE;
                        prim->x2 = p->unk_F0 + p->unk_BC;
                        prim->y2 = p->unk_F2 + p->unk_BE;
                        prim->x3 = p->unk_F4 + p->unk_BC;
                        prim->y3 = p->unk_F6 + p->unk_BE;
                        if (((u16)(prim->x0 + 0x20) < 0x181 &&
                             (u16)(prim->y0 + 0x20) < 0x121) |
                            ((u16)(prim->x1 + 0x20) < 0x181 &&
                             (u16)(prim->y1 + 0x20) < 0x121) |
                            ((u16)(prim->x2 + 0x20) < 0x181 &&
                             (u16)(prim->y2 + 0x20) < 0x121) |
                            ((u16)(prim->x3 + 0x20) < 0x181 &&
                             (u16)(prim->y3 + 0x20) < 0x121))
                        {
                            setlen(prim, 9);
                            s->unk_14 &= 0x7FFF;
                            t = p->unk_14.w + p->unk_0C.w;
                            p->unk_14.w = t;
                            if (t & 0x100)
                            {
                                p->unk_14.w = t - 1;
                            }
                            t = p->unk_18.w + p->unk_10.w;
                            p->unk_18.w = t;
                            if (t & 0x100)
                            {
                                p->unk_18.w = t - 1;
                            }
                            p->unk_18.w = p->unk_18.w << 8;
                            p->unk_10.w = p->unk_10.w << 8;
                            *(s32 *)&prim->u0 = p->unk_10.w + p->unk_0C.w +
                                                ((s->unk_12 + e->unk_06) << 16);
                            if (i != 0)
                            {
                                *(u16 *)&prim->u1 = p->unk_10.h + p->unk_14.h;
                                if (s->unk_10 != 0)
                                {
                                    prim->tpage =
                                        (s->unk_10 + (e->unk_04 & 0xFF9F)) | 0x20;
                                }
                                else
                                {
                                    prim->tpage = e->unk_04 | 0x20;
                                }
                            }
                            else
                            {
                                *(u16 *)&prim->u1 = p->unk_10.h + p->unk_14.h;
                                if (s->unk_10 != 0)
                                {
                                    prim->tpage = s->unk_10 + (e->unk_04 & 0xFF9F);
                                }
                                else
                                {
                                    prim->tpage = e->unk_04;
                                }
                            }
                            *(u16 *)&prim->u2 = p->unk_18.h + p->unk_0C.h;
                            *(u16 *)&prim->u3 = p->unk_18.h + p->unk_14.h;
                            if (prim->x0 > prim->x3)
                            {
                                prim->u1 = prim->u1 - 1;
                                prim->u3 = prim->u3 - 1;
                            }
                            if (prim->y0 > prim->y3)
                            {
                                prim->v2 = prim->v2 - 1;
                                prim->v3 = prim->v3 - 1;
                            }
                            c = e->unk_01;
                            s->unk_0C[3] = c;
                            if (i != 0)
                            {
                                s->unk_0C[3] = c | 2;
                            }
                            else if (p->unk_28 & 8)
                            {
                                if (p->unk_28 & 4)
                                {
                                    s->unk_0C[3] = c | 2;
                                }
                                else
                                {
                                    s->unk_0C[3] = c & 0xFD;
                                }
                            }
                            if (p->unk_28 & 0x10)
                            {
                                s->unk_0C[3] = s->unk_0C[3] & 0xFE;
                            }
                            else
                            {
                                s->unk_0C[3] = s->unk_0C[3] | 1;
                            }
                            *(u32 *)&prim->r0 = *(u32 *)s->unk_0C;
                            if (i != 0)
                            {
                                prim->r0 = prim->r0 >> 2;
                                prim->b0 = prim->b0 >> 2;
                                prim->g0 = prim->g0 >> 2;
                            }
                            AddPrim((void *)(p->unk_24 + p->unk_C4 * 4), prim);
                        }
                    }
                    if ((s8)e->unk_00 < 0)
                    {
                        break;
                    }
                    e++;
                }
            }
            PopMatrix();
        }
        n = *(UnkNode **)(pl - 8);
        if (n == NULL)
        {
            break;
        }
        pl = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
    *(POLY_FT4 **)((u8 *)g->unk_00 + 0x8D0) = p->unk_1C;
    return 0;
}
