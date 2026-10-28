/* func_80033D54 (gprel_72A8) -- 537 of 537 words, 49 DIFF. COMPILES.
 *
 * The sprite/billboard draw of the object tree: for each node on the
 * `UnkNode.unk_18` chain it builds the object's world matrix in the
 * scratchpad, projects the four corners of every element record through
 * RotTransPers4, and emits a POLY_FT4 per element. It is the fourth member of
 * the `(u8 *p, UnkVec *v, UnkSlot *s)` family -- func_80044BB0, func_80044C54,
 * func_80045C34, func_80046398, func_800463EC -- and its inner loop is
 * func_80044D24's, so READ csrc/unk_1811C.c's func_80044D24 FIRST: the element
 * record (Unk44D24E), the +0x100 UV wrap clamps, the sign arms, `setlen`,
 * the `s->unk_0C[3]` flag byte and the `AddPrim` tail are all the same code
 * with the scratchpad offsets shifted by +4.
 *
 * It was the last function BLOCKED on addressing (it needs
 * %gp_rel(D_8008099C) while two landed functions in the old unk_72A8 reach
 * that word absolutely); the unit carve of 2026-10-27 unblocked it.
 *
 * WHAT IS LEFT: 49 words, and they are ALL ALLOCATION, in two groups.
 *
 *  (1) TWO ADJACENT PAIRS OF CALLEE-SAVED REGISTERS ARE SWAPPED. The target
 *      has m1=$s4, e=$s5, m2=$s6, pl=$s7; this build has e=$s4, m1=$s5,
 *      pl=$s6, m2=$s7. `advise.py --greg` prints the allocator's own numbers:
 *      e is 24 refs / 385 live / priority 0.249 and m1 is 25 / 425 / 0.235, a
 *      5.6% gap, and pl (11/429/0.077) over m2 (9/421/0.064) is 16.6%. To
 *      flip the first pair e has to reach 408 live or m1 has to drop to 401,
 *      or m1 has to cross 32 refs (floor_log2 is a step). Everything tried is
 *      recorded at the bottom. Fixing it is worth all 49 words: there is no
 *      instruction difference left anywhere else except (2).
 *
 *  (2) THE TWO EXTRA `lbu` OF THE WRAP CLAMPS sit six slots late (sched1
 *      puts them after `lbu 0x9($s2)`, the target after the first
 *      `lbu 0x7($s2)`, where they fill its load-delay slot). Statement
 *      placement is already swept: a and b between the +0x10 and +0x14
 *      stores is the best of the six legal positions (65 DIFF at the time,
 *      the others 73/75 or a word short because cse merges the reload).
 *
 * THE SHAPES THAT ARE SETTLED. Each was measured; the cost of reverting is in
 * brackets.
 *
 *  - THE +1 POINTER IS NOT IN THE SOURCE. Every element field is read as a
 *    STRUCT FIELD off `e` (`e->unk_08`, `e->unk_0A`, ...) and loop.c
 *    strength-reduces the address givs into one register that happens to be
 *    e+1, which is why the listing reads `0x7($s2)` for `unk_08`: $s2 is the
 *    REDUCED GIV, initialised `addiu $s2, $s5, 0x1` in the inner loop's
 *    PREHEADER and bumped `addiu $s2, $s2, 0xC` in the back-branch's delay
 *    slot. Writing the obvious `q = (u8 *)&e->unk_01;` and indexing off it
 *    instead costs ONE WORD and the whole allocation: loop.c then records a
 *    DEST_REG giv for q as well, the DEST_ADDR givs all combine into the LAST
 *    one recorded, and the combined giv is rejected --
 *    `giv of insn N not worth while, 0 vs 311` in the .loop dump -- so the
 *    `addiu` stays INSIDE the loop and `e` keeps four more references.
 *    See matching-levers section 8. An explicit second pointer
 *    (`q = ...; ... q += 0xC;`) is worse again (+5): loop.c then reduces the
 *    high-offset uses into a THIRD register at e+4, and the extra pressure
 *    spills the callback pointer out of $fp.
 *
 *  - BUT THE TWO SIGN BYTES MUST GO THROUGH A POINTER LOCAL, because they are
 *    VOLATILE reads and a volatile MEM is not replaced by the reduced giv.
 *    `q = (u8 *)&e->unk_01;` with `(s8)((volatile u8 *)q)[1]` lands them on
 *    $s2 as the listing has them; `*(volatile u8 *)&e->unk_02` reads them off
 *    $s5 instead (4 register-only words, and it pushes `e` from 24 refs to
 *    36, which is what makes the (1) gap unreachable).
 *
 *  - THE VOLATILE CAST ITSELF IS LOAD-BEARING. `(s8)x * 2` on a plain `u8 *`
 *    read folds the sign-extension into the load: `lb` + `sll 1`, two words
 *    where the target has three (`lbu` + `sll 24` + `sra 23`). combine
 *    cannot fold a volatile MEM, so the shifts survive. The same cast on the
 *    four UV reads would cost an `andi 0xff` each (a volatile QImode read
 *    cannot be widened in place), so exactly two reads are volatile.
 *
 *  - THE MATRIX COPY IS EIGHT SEPARATE WORD ASSIGNMENTS, not a struct copy.
 *    `o->unk_40 = p->unk_54;` with MATRIX on both sides is a 4-register block
 *    move (16 words); the target reuses ONE register with a nop between each
 *    load and store (23 words), which is what eight statements give. So both
 *    structs model that range as eight `s32`s.
 *
 *  - ARM B'S LAST CALL IS WRITTEN `TransMatrix(m2, &p->unk_44)` AND ARM A'S
 *    `TransMatrix(m2, tv)`. Both arms end in the same 30-word tail; with the
 *    same spelling in both, jump2's cross-jumping merges all 30 and the
 *    function is 28 WORDS SHORT (509/537, 512 DIFF). The target merged only
 *    the `jal` and its delay slot, because the argument setup differs --
 *    `lw $a1, 0x28($sp)` (tv is spilled) against `addiu $a1, $s1, 0x44`.
 *
 *  - EACH SIGN ARM HAS ITS OWN SINGLE-SET LOCALS (x0/t0 .. x3/t3) and its own
 *    copy of the two `sh`. Sharing one `x` and one `t` across the four arms
 *    costs FOUR WORDS (533/537): a local SET EXACTLY ONCE is a birthing insn
 *    for sched1, which schedules it LAST in its block, and that is what keeps
 *    `subu`/`addu` after the two `sh` -- with a shared local the subtract
 *    moves ahead of them, the arms' tails become identical and cross-jumping
 *    lifts both stores into the join. matching-levers section 8.
 *
 *  - `a` AND `b` HOLD THE FIRST OPERAND OF EACH WRAP CLAMP (a second read of
 *    unk_08/unk_09, which the stores in between make a real reload). Reading
 *    the fields inline in the conditions instead is a word over and 389 DIFF.
 *
 *  - THE CLIP TEST IS ONE EXPRESSION: four `&&` pairs joined by `|`, not four
 *    0/1 locals OR'd afterwards. The ORs have to interleave with the
 *    short-circuit branches (two of them sit in branch delay slots).
 *
 *  - THE CLUT IS A TERNARY INSIDE THE u0/v0 STORE, so that the
 *    `p->unk_10.w + p->unk_0C.w` sum is computed BEFORE the branch.
 *
 *  - THE PROLOGUE READS THE OT POINTER INTO ITS OWN SHORT-LIVED LOCAL (`ot`),
 *    separate from the loop's `prim`. Without it the value lands in $s0
 *    (12 words); with the loop's `prim` reused it lands in a callee-saved
 *    register instead of the target's $v1. The three matrix pointers and
 *    `base` are assigned AFTER the D_8008099C guard -- initialising them at
 *    their declarations materialises them above the guard (9 words).
 *
 *  - The guard is `if (D_8008099C != 0) { return 0; }` with a separate
 *    `return 0;` at the end; that is what gives `beqz`+`j` rather than one
 *    `bnez`, because the then-arm carries its own `$v0 = 0`.
 *
 * MEASURED AND REJECTED for the allocation swap: reordering the three matrix
 * pointer assignments (all six orders, 119-122 DIFF at the time); assigning
 * them just before PushMatrix (no change); assigning them inside the outer
 * loop (509 words -- the constants fold into the call sites); moving
 * `e = s->unk_08` to the top of the outer loop body (538 words); reusing a
 * multi-set local for `a` or `b` (68/73 DIFF). The two margins above are the
 * place to attack, with `advise.py --greg`.
 */
#include "common.h"

__asm__(".sdata\n\t.size D_8008099C,4\n\t.text");

extern SVECTOR D_80080994;

typedef union Unk33D54XY
{
    s32 w;
    struct
    {
        u16 x;
        u16 y;
    } h;
} Unk33D54XY;

typedef struct Unk33D54O
{
    u8 pad_00[0x4];
    struct Unk33D54O *unk_04;
    SVECTOR unk_08;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
    u8 pad_1C[0x4];
    s32 unk_20;
    s32 unk_24;
    s32 unk_28;
    u8 pad_2C[0x4];
    VECTOR unk_30;
    s32 unk_40;
    s32 unk_44;
    s32 unk_48;
    s32 unk_4C;
    s32 unk_50;
    s32 unk_54;
    s32 unk_58;
    s32 unk_5C;
    s32 unk_60;
} Unk33D54O;

typedef struct Unk33D54SP
{
    u8 pad_00[0xC];
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    Unk3DBD0W unk_18;
    POLY_FT4 *unk_1C;
    u8 pad_20[0x4];
    s32 unk_24;
    u16 unk_28;
    u8 pad_2A[0xA];
    VECTOR unk_34;
    VECTOR unk_44;
    s32 unk_54;
    s32 unk_58;
    s32 unk_5C;
    s32 unk_60;
    s32 unk_64;
    s32 unk_68;
    s32 unk_6C;
    s32 unk_70;
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
    Unk33D54XY unk_9C;
    u8 pad_A0[0x4];
    Unk33D54XY unk_A4;
    u8 pad_A8[0x4];
    Unk33D54XY unk_AC;
    u8 pad_B0[0x4];
    Unk33D54XY unk_B4;
    u8 pad_B8[0xC];
    s32 unk_C4;
    MATRIX unk_C8;
} Unk33D54SP;

s32 func_80033D54(u8 *pl, UnkVec *v, UnkSlot *s)
{
    Unk33D54SP *p = (Unk33D54SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    MATRIX *m1;
    MATRIX *m2;
    VECTOR *tv;
    u8 *base;
    Unk33D54O *o;
    Unk44D24E *e;
    POLY_FT4 *prim;
    POLY_FT4 *ot;
    u8 *q;
    UnkNode *n;
    void (*f)(u8 *);
    s32 x0;
    s32 x1;
    s32 x2;
    s32 x3;
    s32 t0;
    s32 t1;
    s32 t2;
    s32 t3;
    s32 t;
    s32 a;
    s32 b;
    u8 c;

    if (D_8008099C != 0)
    {
        return 0;
    }
    m1 = (MATRIX *)&p->unk_54;
    tv = &p->unk_44;
    m2 = &p->unk_C8;
    base = D_80083160.unk_00;
    ot = *(POLY_FT4 **)(base + 0x8D0);
    p->unk_24 = (s32)(base + 0x70);
    p->unk_34.vz = 0x1000;
    p->unk_44.vz = 0;
    p->unk_90 = 0;
    p->unk_88 = 0;
    p->unk_80 = 0;
    p->unk_78 = 0;
    p->unk_1C = ot;
    PushMatrix();
    for (;;)
    {
        o = *(Unk33D54O **)pl;
        f = *(void (**)(u8 *))(pl + 8);
        p->unk_C4 = o->unk_60;
        p->unk_28 = s->unk_14;
        e = (Unk44D24E *)s->unk_08;
        p->unk_34.vx = o->unk_20;
        p->unk_34.vy = o->unk_24;
        p->unk_34.vz = o->unk_28;
        if (o->unk_04 == NULL)
        {
            p->unk_44.vx = ((s16 *)v)[1];
            p->unk_44.vy = ((s16 *)v)[3];
            p->unk_44.vz = ((s16 *)v)[5];
            TransMatrix(m1, tv);
            RotMatrix(&o->unk_08, m1);
            ScaleMatrix(m1, &p->unk_34);
            o->unk_40 = p->unk_54;
            o->unk_44 = p->unk_58;
            o->unk_48 = p->unk_5C;
            o->unk_4C = p->unk_60;
            o->unk_50 = p->unk_64;
            o->unk_54 = p->unk_68;
            o->unk_58 = p->unk_6C;
            o->unk_5C = p->unk_70;
            p->unk_44.vx = o->unk_10;
            p->unk_44.vy = o->unk_14;
            p->unk_44.vz = o->unk_18;
            TransMatrix(m2, tv);
        }
        else
        {
            TransMatrix(m1, &o->unk_30);
            RotMatrix(&o->unk_08, m1);
            ScaleMatrix(m1, &p->unk_34);
            CompMatrix((MATRIX *)&o->unk_04->unk_40, m1, m1);
            o->unk_40 = p->unk_54;
            o->unk_44 = p->unk_58;
            o->unk_48 = p->unk_5C;
            o->unk_4C = p->unk_60;
            o->unk_50 = p->unk_64;
            o->unk_54 = p->unk_68;
            o->unk_58 = p->unk_6C;
            o->unk_5C = p->unk_70;
            p->unk_44.vx = o->unk_10;
            p->unk_44.vy = o->unk_14;
            p->unk_44.vz = o->unk_18;
            TransMatrix(m2, &p->unk_44);
        }
        RotMatrix(&D_80080994, m2);
        CompMatrix(m1, m2, m1);
        SetRotMatrix(m1);
        SetTransMatrix(m1);
        while (e != NULL)
        {
            prim = p->unk_1C;
            p->unk_1C = prim + 1;
            q = (u8 *)&e->unk_01;
            p->unk_0C.w = e->unk_08;
            p->unk_10.w = e->unk_09;
            a = e->unk_08;
            b = e->unk_09;
            p->unk_14.w = e->unk_0A;
            p->unk_18.w = e->unk_0B;
            if (a + e->unk_0A >= 0x100)
            {
                p->unk_14.w = p->unk_14.w - 1;
            }
            if (b + e->unk_0B >= 0x100)
            {
                p->unk_18.w = p->unk_18.w - 1;
            }
            if (((e->unk_00 ^ p->unk_28) & 1) != 0)
            {
                x0 = -((s8)((volatile u8 *)q)[1] * 2);
                p->unk_84 = x0;
                p->unk_74 = x0;
                t0 = x0 - p->unk_14.h;
                p->unk_8C = t0;
                p->unk_7C = t0;
            }
            else
            {
                x1 = (s8)((volatile u8 *)q)[1] * 2;
                p->unk_84 = x1;
                p->unk_74 = x1;
                t1 = x1 + p->unk_14.h;
                p->unk_8C = t1;
                p->unk_7C = t1;
            }
            if (((e->unk_00 ^ p->unk_28) & 2) != 0)
            {
                x2 = -((s8)((volatile u8 *)q)[2] * 2);
                p->unk_7E = x2;
                p->unk_76 = x2;
                t2 = x2 - p->unk_18.h;
                p->unk_8E = t2;
                p->unk_86 = t2;
            }
            else
            {
                x3 = (s8)((volatile u8 *)q)[2] * 2;
                p->unk_7E = x3;
                p->unk_76 = x3;
                t3 = x3 + p->unk_18.h;
                p->unk_8E = t3;
                p->unk_86 = t3;
            }
            RotTransPers4((SVECTOR *)&p->unk_74, (SVECTOR *)&p->unk_7C,
                          (SVECTOR *)&p->unk_84, (SVECTOR *)&p->unk_8C,
                          &p->unk_9C.w, &p->unk_A4.w, &p->unk_AC.w,
                          &p->unk_B4.w, &p->unk_94, &p->unk_98);
            if ((((u16)(p->unk_9C.h.x + 0x20) < 0x181 &&
                  (u16)(p->unk_9C.h.y + 0x20) < 0x121) |
                 ((u16)(p->unk_A4.h.x + 0x20) < 0x181 &&
                  (u16)(p->unk_A4.h.y + 0x20) < 0x121) |
                 ((u16)(p->unk_AC.h.x + 0x20) < 0x181 &&
                  (u16)(p->unk_AC.h.y + 0x20) < 0x121) |
                 ((u16)(p->unk_B4.h.x + 0x20) < 0x181 &&
                  (u16)(p->unk_B4.h.y + 0x20) < 0x121)) != 0)
            {
                *(s32 *)&prim->x0 = p->unk_9C.w;
                *(s32 *)&prim->x1 = p->unk_A4.w;
                *(s32 *)&prim->x2 = p->unk_AC.w;
                *(s32 *)&prim->x3 = p->unk_B4.w;
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
                *(s32 *)&prim->u0 =
                    p->unk_10.w + p->unk_0C.w +
                    ((s->unk_12 != 0 ? s->unk_12 : e->unk_06) << 16);
                *(u16 *)&prim->u1 = p->unk_10.h + p->unk_14.h;
                if (s->unk_10 != 0)
                {
                    prim->tpage = s->unk_10 + (e->unk_04 & 0xFF9F);
                }
                else
                {
                    prim->tpage = e->unk_04;
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
                if (p->unk_34.vx >= 0x1800)
                {
                    prim->u1 = prim->u1 - 1;
                    prim->u3 = prim->u3 - 1;
                }
                if (p->unk_34.vy >= 0x1800)
                {
                    prim->v2 = prim->v2 - 1;
                    prim->v3 = prim->v3 - 1;
                }
                c = e->unk_01;
                s->unk_0C[3] = c;
                if (p->unk_28 & 8)
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
                AddPrim((void *)(p->unk_24 + p->unk_C4 * 4), prim);
            }
            else
            {
                s->unk_14 |= 0x8000;
            }
            if ((s8)e->unk_00 < 0)
            {
                break;
            }
            e++;
        }
        *(POLY_FT4 **)((u8 *)g->unk_00 + 0x8D0) = p->unk_1C;
        if (f != NULL)
        {
            f(pl);
        }
        p->unk_1C = *(POLY_FT4 **)((u8 *)g->unk_00 + 0x8D0);
        n = *(UnkNode **)(pl - 8);
        if (n == NULL)
        {
            break;
        }
        pl = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
    PopMatrix();
    *(POLY_FT4 **)((u8 *)g->unk_00 + 0x8D0) = p->unk_1C;
    return 0;
}
