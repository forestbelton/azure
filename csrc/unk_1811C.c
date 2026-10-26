/* Another piece of the original unk_14934 chain, carved off when the
 * gprel_17B7C $gp cluster was taken out of the middle of unk_176B8. Named for
 * its start offset. See docs/addressing-and-translation-units.md.
 */
/* The tail of the original unk_14934 split, carved off when gprel_17504 was
 * taken out of the middle of it: a $gp cluster cannot be compiled with the
 * same -G as its neighbours, so the unit had to be cut in three. Named for its
 * start offset, like the other provisional units. See
 * docs/addressing-and-translation-units.md.
 */
#include "common.h"

void func_800423C0(u8 *, s32, u8 *);
void func_80041E70(void *);
void func_80047338(Unk800777AC *, u32, u32);
void func_80047468(Unk800777AC *, u32, u32);
void func_800463EC(u8 *, UnkVec *, UnkSlot *, s16);
void func_80045CC4(u8 *, UnkVec *, UnkSlot *, s16);
void func_80044D24(u8 *, UnkSlot *, s16);
void func_80041344(u32, u32);
void func_8003F6D4(s32, s32, s32 *, s32);
void func_80040B88(void);
void func_80042984(u8 *);
void func_80041AB0(void);
void func_80041AE4(void);
void func_80043CD0(void);
void func_80043D04(void);
s32 func_80042518(s8 *, s8);
UnkNode *func_8003FC64(s32);
void func_80043FD0(Unk44144 *);
s32 func_8003F794(s16, s16);
void func_80040454(void);

/* Links the node into one of the 0x20 tag slots at D_80083360/D_800833E0,
 * pushing any existing head down, or claims a free slot. Returns 0 only when
 * every slot is taken.
 *
 * Three shapes are load-bearing:
 *  - `D_800833E0[i] = (u32) n;` is duplicated into both arms of the null
 *    check. That gives the store its own basic block instead of putting it in
 *    the join block, so sched1 never has it and the `return 1` constant in one
 *    ready list, where the hazard heuristic always hoists the constant past
 *    it. jump2's cross-jumping then merges the two copies back and plants the
 *    join label in front of the store, which is why .L80044964 sits before it
 *    and the word count is unchanged.
 *  - the free-slot bookkeeping is `if (first == -1) { first = i; } last = i;`
 *    in that order. With `last = i` first, cse folds the second assignment to
 *    a copy of last's register; in this order the `last = i` insn is instead
 *    stolen into the bne's delay slot. The test has to be against -1, not
 *    `< 0`: the target compares against a loop-hoisted -1 in a register.
 *  - `return 0` is the last statement, so its block lands just before the
 *    epilogue and the test is a single bltz. An early return costs a bgez, a
 *    `j` and a move.
 *
 * The two tails of the D_8006E7B4 walk are genuinely duplicated in the target
 * -- they differ only in which index they use -- so they are written out
 * twice here as well. */
s32 func_8004491C(UnkNode *n, u32 tag)
{
    s32 first = -1;
    s32 last = 0;
    s32 i;
    s32 j;

    for (i = 0; i < 0x20; i++)
    {
        if (D_80083360[i] == tag)
        {
            UnkNode *h = (UnkNode *)D_800833E0[i];

            n->unk_18 = h;
            if (h != NULL)
            {
                h->unk_14 = n;
                D_800833E0[i] = (u32)n;
            }
            else
            {
                D_800833E0[i] = (u32)n;
            }
            n->unk_14 = (UnkNode *)&D_800833E0[i];
            n->unk_1C = i;
            return 1;
        }
        if (D_80083360[i] == 0)
        {
            if (first == -1)
            {
                first = i;
            }
            last = i;
        }
    }
    if (first >= 0)
    {
        n->unk_18 = NULL;
        j = 0;
        while (D_8006E7B4[j] != 0)
        {
            if (D_8006E7B4[j] == tag)
            {
                D_80083360[first] = tag;
                D_800833E0[first] = (u32)n;
                n->unk_14 = (UnkNode *)&D_800833E0[first];
                n->unk_1C = first;
                return 1;
            }
            j++;
        }
        D_80083360[last] = tag;
        D_800833E0[last] = (u32)n;
        n->unk_14 = (UnkNode *)&D_800833E0[last];
        n->unk_1C = last;
        return 1;
    }
    return 0;
}

void func_80044A50(UnkNode *n)
{
    UnkNode *a = n->unk_14;
    UnkNode *b;

    if (a == NULL)
    {
        return;
    }
    b = n->unk_18;
    if (b != NULL)
    {
        b->unk_14 = a;
    }
    if ((u8 *)a >= (u8 *)&D_800833E0 &&
        (u8 *)a < (u8 *)&D_800833E0 + 0x80)
    {
        a->unk_00 = b;
    }
    else
    {
        a->unk_18 = b;
    }
    n->unk_18 = NULL;
    n->unk_14 = NULL;
}

void func_80044AAC(UnkNode *n, UnkNode *m)
{
    /* One function-scope local shared by both arms, not a block-local in
     * each: set in two blocks it becomes a global allocno and lands in $a1,
     * which is the register the target uses in both arms. */
    UnkNode *p;

    func_80044A50(m);
    if (n != NULL)
    {
        p = n->unk_18;
        n->unk_18 = m;
        m->unk_14 = n;
        m->unk_18 = p;
        if (p != NULL)
        {
            p->unk_14 = m;
        }
    }
    else
    {
        p = (UnkNode *)D_800833E0[m->unk_1C];
        /* The store between the first two reads of unk_1C is what stops gcc
         * sharing the index: it cannot prove the store does not alias. */
        m->unk_18 = p;
        m->unk_14 = (UnkNode *)&D_800833E0[m->unk_1C];
        D_800833E0[m->unk_1C] = (u32)m;
        p->unk_14 = m;
    }
}

void func_80044B48(void)
{
    u32 *p = *(u32 **)(D_80083160.unk_00 + 0x8D0);

    func_80067F20(p, 0, 0, 0, 0);
    AddPrim(D_80083160.unk_00 + 0x8AC, p);
    *(u32 **)(D_80083160.unk_00 + 0x8D0) = p + 3;
}

s32 func_80044BB0(u8 *p, UnkVec *v, UnkSlot *s)
{
    u16 *scratch = (u16 *)0x1F800000;
    /* One temporary for both halves: two separate single-set pseudos get
     * maximum scheduler priority and both loads float above both stores,
     * where the target pairs each load with its store. */
    u16 t;

    for (;;)
    {
        UnkNode *n;

        if (!(s->unk_14 & 0x80))
        {
            t = ((u16 *)v)[1];
            scratch[0] = t;
            t = ((u16 *)v)[3];
            scratch[1] = t;
            func_80044D24(p, s, ((u16 *)v)[5] + s->unk_06);
        }
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

s32 func_80044C54(u8 *p, UnkVec *v, UnkSlot *s)
{
    u8 *scratch = (u8 *)0x1F800000;
    u16 t;

    for (;;)
    {
        UnkNode *n;

        if (!(s->unk_14 & 0x80))
        {
            t = ((u16 *)v)[1];
            *(u16 *)(scratch + 0x70) = t;
            t = ((u16 *)v)[3];
            *(u16 *)(scratch + 0x72) = t;
            t = ((u16 *)v)[5];
            *(u16 *)(scratch + 0x74) = t;
            RotTransPers(scratch + 0x70, scratch, scratch + 0x90,
                         scratch + 0x94);
            func_80044D24(p, s, s->unk_06);
            if (func_80045310(*(u32 *)(D_80083160.unk_00 + 0x8D0)) != 0)
            {
                return 0;
            }
        }
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

/* Emits one sprite's pieces: builds the matrix, then per piece clamps the UV
 * pairs, converts the two signed byte offsets into four corner vectors,
 * projects each through the GTE and adds the primitive.
 *
 * The seven words this was parked on are the three byte-to-signed
 * conversions, and `volatile` is the answer -- which is worth stating plainly
 * because docs/matching-levers.md section 10 had `volatile` down as a dead
 * end. It is a dead end as a *nudge*; against this particular fold it is the
 * mechanism. extendqisi2's expander does force_not_mem under -O, so
 * `lbu` / `sll 24` / `sra 24` is the NATURAL expansion of the conversion and
 * combine is what refolds it to `lb`, via a 3-insn combine into
 * extendqisi2_internal, which wants a memory_operand.
 * gen_lowpart_for_combine returns a CLOBBER for a volatile MEM, so the refold
 * cannot happen.
 *
 * Two caveats make it awkward, and both are load-bearing here:
 *  - loop.c's find_mem_givs skips volatile MEMs, so a volatile FIELD is
 *    addressed off the biv where the target reads off the combined giv. The
 *    address therefore goes through a `volatile u8 *` local instead, which is
 *    an ordinary DEST_REG giv that combine_givs merges into the
 *    representative. Pointing the pointer at the field and dereferencing it
 *    directly costs 53 words, because cse eliminates the pointer.
 *  - the pointer must be assigned in the ENCLOSING block, and there is one per
 *    block (vp and wp hold the same address on purpose). Assigned in the same
 *    block as its use, cse folds it into the address before loop runs and no
 *    giv is formed. Shared across both blocks it is not `replaceable`
 *    (loop.c's last_use_this_basic_block), so loop.c leaves a `move` copy and
 *    cse2 propagates it away only within its path-length window -- which came
 *    out as one block's loads correct and the other's off the copy's register.
 *
 * Four more shapes, each measured by reverting it (all stay at 379 words):
 *  - the negation happens inside each arm, not at the join (8 words). That
 *    keeps the sra's destination a block-local temp, and jump2 cross-jumps the
 *    two `negu`s back into one at the join -- the duplicate-store trick of
 *    matching-levers section 1, applied to an arithmetic insn.
 *  - the arm intermediates are separate locals, `n` in the then-arm and `w` in
 *    the else-arm, not one `t` (15 words). The target has four live ranges
 *    there where one variable gives one allocno.
 *  - the e->unk_01 byte goes through its own `f`, not the clamps' `c`
 *    (7 words), which otherwise spans both as one allocno.
 *  - the clamp comparisons are written `prim->x0 > prim->x3`, not reversed
 *    (4 words): operand order decides which `lh` sched1 issues first.
 *  - the three UV pairs and the +0x04 colour word go in through casts
 *    (`*(u16 *)&prim->u1`, `*(u32 *)&prim->r0`). psyq's POLY_FT4 splits each
 *    pair into `u_char u, v` and +0x04 into r0/g0/b0/code, but the target
 *    stores a pair with one `sh` and the colour word with one `sw`, then
 *    byte-patches `u`/`v` individually in the clamps -- so both widths are
 *    needed at the same address. Measured identical to the old hand-written
 *    struct's union fields.
 *
 * And from the earlier pass: `e = (Unk44D24E *) s->unk_08;` must sit between
 * the unk_14 update and the unk_24 store or the whole prologue's register
 * assignment changes; one scratchpad pointer local but literal constants for
 * the libgte arguments; D_80083160 reached both ways, pointer local declared
 * first so its lui is shared; store-then-overwrite clamps; arg0 unused and
 * arg2 s16; and one `Unk44D24E *e` with `e++`, which makes it the biv. */
void func_80044D24(u8 *arg0, UnkSlot *s, s16 arg2)
{
    Unk80083160 *g = &D_80083160;
    Unk44D24SP *p = (Unk44D24SP *)0x1F800000;
    u8 *base = D_80083160.unk_00;
    POLY_FT4 *prim = *(POLY_FT4 **)(base + 0x8D0);
    Unk44D24E *e;
    volatile u8 *vp;
    volatile u8 *wp;
    s32 t;
    s32 w;
    s32 n;
    s32 x;
    s32 y;
    u8 c;
    u8 f;

    p->unk_20 = (s32)(base + 0x70);
    p->unk_38 = 0x1000;
    p->unk_48 = 0;
    p->unk_8C = 0;
    p->unk_84 = 0;
    p->unk_7C = 0;
    p->unk_74 = 0;
    PushMatrix();
    s->unk_14 |= 0x8000;
    e = (Unk44D24E *)s->unk_08;
    p->unk_24 = s->unk_14;
    p->unk_30 = s->unk_1C;
    p->unk_34 = s->unk_1E;
    p->unk_40 = p->unk_00 + s->unk_20;
    p->unk_44 = (s16)p->unk_02 + s->unk_22;
    TransMatrix((void *)0x1F800050, (void *)0x1F800040);
    RotMatrix(&s->unk_16, (void *)0x1F800050);
    ScaleMatrix((void *)0x1F800050, (void *)0x1F800030);
    SetRotMatrix((void *)0x1F800050);
    SetTransMatrix((void *)0x1F800050);
    for (;;)
    {
        if ((e->unk_00 & 0x20) == 0)
        {
            x = e->unk_08;
            p->unk_08.w = x;
            y = e->unk_0A;
            p->unk_10.w = y;
            if (x + y >= 0x100)
            {
                p->unk_10.w = y - 1;
            }
            x = e->unk_09;
            p->unk_0C.w = x;
            y = e->unk_0B;
            p->unk_14.w = y;
            if (x + y >= 0x100)
            {
                p->unk_14.w = y - 1;
            }
            vp = (volatile u8 *)&e->unk_01;
            if (((e->unk_00 ^ p->unk_24) & 1) != 0)
            {
                if (s->unk_14 & 0x400)
                {
                    n = -((s8)vp[1] * 2);
                }
                else
                {
                    n = -((s8)vp[1]);
                }
                p->unk_80 = n;
                p->unk_70 = n;
                t = n - p->unk_10.h;
            }
            else
            {
                w = (s8)vp[1];
                if (s->unk_14 & 0x400)
                {
                    w = w * 2;
                }
                p->unk_80 = w;
                p->unk_70 = w;
                t = w + p->unk_10.h;
            }
            p->unk_88 = t;
            p->unk_78 = t;
            wp = (volatile u8 *)&e->unk_01;
            if (((e->unk_00 ^ p->unk_24) & 2) != 0)
            {
                if (s->unk_14 & 0x400)
                {
                    n = -((s8)wp[2] * 2);
                }
                else
                {
                    n = -((s8)wp[2]);
                }
                p->unk_7A = n;
                p->unk_72 = n;
                t = n - p->unk_14.h;
            }
            else
            {
                w = (s8)wp[2];
                if (s->unk_14 & 0x400)
                {
                    w = w * 2;
                }
                p->unk_7A = w;
                p->unk_72 = w;
                t = w + p->unk_14.h;
            }
            p->unk_8A = t;
            p->unk_82 = t;
            gte_ldv0(&p->unk_70);
            gte_rtv0();
            gte_stsv(&prim->x0);
            gte_ldv0(&p->unk_78);
            gte_rtv0();
            gte_stsv(&prim->x1);
            gte_ldv0(&p->unk_80);
            gte_rtv0();
            gte_stsv(&prim->x2);
            gte_ldv0(&p->unk_88);
            gte_rtv0();
            gte_stsv(&prim->x3);
            setlen(prim, 9);
            s->unk_14 &= 0x7FFF;
            p->unk_10.w = p->unk_10.w + p->unk_08.w;
            p->unk_14.w = (p->unk_14.w + p->unk_0C.w) << 8;
            p->unk_0C.w = p->unk_0C.w << 8;
            if (p->unk_24 & 0x100)
            {
                prim->clut = s->unk_12;
            }
            else
            {
                prim->clut = s->unk_12 + e->unk_06;
            }
            *(u16 *)&prim->u0 = p->unk_0C.h + p->unk_08.h;
            *(u16 *)&prim->u1 = p->unk_0C.h + p->unk_10.h;
            if (s->unk_10 != 0)
            {
                prim->tpage = s->unk_10 + (e->unk_04 & 0xFF9F);
            }
            else
            {
                prim->tpage = e->unk_04;
            }
            *(u16 *)&prim->u2 = p->unk_14.h + p->unk_08.h;
            *(u16 *)&prim->u3 = p->unk_14.h + p->unk_10.h;
            if ((s16)p->unk_50 >= 0x1800)
            {
                c = prim->u3;
                prim->u3 = c + 0xFF;
                prim->u1 = c;
            }
            if ((s16)p->unk_58 >= 0x1800)
            {
                c = prim->v3;
                prim->v3 = c + 0xFF;
                prim->v2 = c;
            }
            if (prim->x0 > prim->x3)
            {
                c = prim->u3;
                prim->u3 = c + 0xFF;
                prim->u1 = c;
            }
            if (prim->y0 > prim->y3)
            {
                c = prim->v3;
                prim->v3 = c + 0xFF;
                prim->v2 = c;
            }
            f = e->unk_01;
            s->unk_0C[3] = f;
            if (p->unk_24 & 8)
            {
                if (p->unk_24 & 4)
                {
                    s->unk_0C[3] = f | 2;
                }
                else
                {
                    s->unk_0C[3] = f & 0xFD;
                }
            }
            *(u32 *)&prim->r0 = *(u32 *)s->unk_0C;
            AddPrim((void *)(p->unk_20 + arg2 * 4), prim);
            prim++;
        }
        if ((s8)e->unk_00 < 0)
        {
            break;
        }
        e++;
    }
    PopMatrix();
    *(POLY_FT4 **)((u8 *)g->unk_00 + 0x8D0) = prim;
}

s32 func_80045310(u32 arg0)
{
    /* D_80081478 must be declared with an incomplete array type: a complete
     * one of 8 bytes is small data, which makes the base an `la` constant and
     * folds the +4 into it, where the target bumps a register. */
    u32 *p = D_80081478;

    if (D_80083160.unk_00 != D_801C9E40)
    {
        p++;
    }
    return *p < arg0;
}
