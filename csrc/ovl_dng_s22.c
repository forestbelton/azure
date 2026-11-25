#include "common.h"

typedef struct UnkDngS22LineSP
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
    u8 pad_7A[0xA];
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
} UnkDngS22LineSP;
void func_800478B8(u8 *);
void func_8003DB94(void *, void *, s32);
s32 func_800644B8(s32);
s32 func_80064584(s32);
UnkNode *func_8003FC64(s32);
s32 func_8004491C(UnkNode *, u32);
s32 func_s22_8002445C(u8 *p, u8 *b);
UnkNode *func_s22_80024824(u8 *v, UnkVec *q, s16 d, s16 e);
extern u8 D_80045340[];
extern u8 D_800E2178[];
extern u8 D_s22_80025308[];
extern u8 D_800253BC[];

/* 52 words, 0 differing, first compile. The step of the node
   func_s22_800240F8 installs: bump the +0x14 counter on the node at the
   actor's +0x0C, advance the UnkVec position by its velocity on x and y,
   double the z velocity while the +0x10 frame counter is under 8, advance z,
   spin the renderable's +0x1A by 0x300, and retire when +0x10 passes +0x12.
   MEASURED AND FREE, so neither is why this lands: `b->unk_14 <<= 1' in place
   of `*= 2' (52 w, 0) and the tail compare written `p + 0x10 > p + 0x12'
   instead of `p + 0x12 < p + 0x10' (52 w, 0). */
void func_s22_80024028(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(*(u8 **)(p + 0xC) + 0x14) = *(u16 *)(*(u8 **)(p + 0xC) + 0x14) + 1;
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    if (*(s16 *)(p + 0x10) < 8)
    {
        b->unk_14 *= 2;
    }
    b->unk_08 += b->unk_14;
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x300;
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    if (*(s16 *)(p + 0x12) < *(s16 *)(p + 0x10))
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* 69 words, 0 differing, first compile, and it landed off func_s22_80024824
   which was already in this file: the `r = (u8 *)n + 0x20' pointer local is
   the same device, here with the stores at r+0xC/+0x10/+0x12.
   ONE PRICED FACT: the jitter is `rand() % 0x1000', a SIGNED modulo, and its
   bgez/addiu 0xFFF/sra 12/sll 12/subu quintet is 5 of the 69 words --
   `rand() & 0xFFF' is 63 words / 57 differing. */
UnkNode *func_s22_800240F8(u8 *v, UnkVec *q, s16 d)
{
    UnkNode *n;
    UnkVec *k;
    u8 *m;
    u8 *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s22_80024028;
        r = (u8 *)n + 0x20;
        *(u8 **)(r + 0xC) = v;
        *(s16 *)(r + 0x10) = 0;
        *(s16 *)(r + 0x12) = d;
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        *(u32 *)(m + 0x8) = (u32)D_s22_80025308;
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1E) = 0x800;
        *(s16 *)(m + 0x1C) = 0x800;
        func_8004491C(n, (u32)D_80045340);
        k = n->unk_08;
        *k = *q;
        k->unk_14 = 0xFFFF0000;
        return n;
    }
    return NULL;
}

/* 57 words, 0 differing, first compile. TWO PRICED COMPARISON-OPERAND FACTS,
   both the brief's lever and both worth 2 rows with nothing moved:
     `*(s16 *)(p + 0x4) == *(s16 *)(p + 0x6)'   57 w, 0   kept
       ... operands the other way round         57 w, 2
     `*(s16 *)(p + 0x4) > *(s16 *)(p + 0x6)'    57 w, 0   kept
       ... as `p + 0x6 < p + 0x4'               48 w, 2 on func_s22_800248F8
   The `b->unk_08 += b->unk_14' BEFORE the doubling is what makes the shift
   come out `addu $v0, $v1, $zero' / `sll $v0, $v0, 1' rather than a fresh
   load; func_s22_80024028 has the two statements the other way round. */
void func_s22_8002420C(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
    b->unk_08 += b->unk_14;
    if (*(s16 *)(p + 0x4) < 8)
    {
        b->unk_14 *= 2;
    }
    if (*(s16 *)(p + 0x4) == *(s16 *)(p + 0x6))
    {
        *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) >> 1;
    }
    func_800478B8(c);
    if (*(s16 *)(p + 0x4) > *(s16 *)(p + 0x6))
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* 66 words, 0 differing, first compile, built on func_s22_80024824's
   `r = (u8 *)n + 0x20' local.
   ONE PRICED FACT: the two 0x1800 scale stores go +0x1E FIRST, then +0x1C --
   reversed it is 66 words / 2 differing, because the one `addiu $v0, $zero,
   0x1800' has to be live across both and the second store sits in the
   func_8004491C delay slot.
   MEASURED AND FREE: `k->unk_08 += 0xFD010000' in place of `-= 0x2FF0000'
   (66 w, 0) -- both give the same `lui $a0, 0xFD01' / `addu', so the kept
   form is chosen only because it stays signed. */
UnkNode *func_s22_800242F0(u8 *v, UnkVec *q, s16 d)
{
    UnkNode *n;
    UnkVec *k;
    u8 *m;
    u8 *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s22_8002420C;
        r = (u8 *)n + 0x20;
        *(u8 **)r = v;
        *(s16 *)(r + 0x4) = 0;
        *(s16 *)(r + 0x6) = d;
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        func_8003DB94(m, D_800E2178, 0);
        *(s16 *)(m + 0x1E) = 0x1800;
        *(s16 *)(m + 0x1C) = 0x1800;
        func_8004491C(n, (u32)D_80045340);
        k = n->unk_08;
        *k = *q;
        k->unk_14 = 0x10000;
        k->unk_08 -= 0x2FF0000;
        return n;
    }
    return NULL;
}

/* 25 words, 0 differing, first compile. A leaf: bump the +0x14 counter on
   the node at the actor's +0x0, bump the +0x4 frame, retire when it reaches
   +0x6. The re-read of +0x4 after the store is what gives the `sll 16' /
   `sra 16' pair -- cse keeps the stored u16 and the s16 read sign-extends it.
   MEASURED AND FREE, so it is NOT why this lands: the gate written
   `*(s16 *)(p + 0x6) <= *(s16 *)(p + 0x4)' instead of
   `*(s16 *)(p + 0x4) >= *(s16 *)(p + 0x6)' (25 w, 0). */
void func_s22_800243F8(u8 *p)
{
    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
    if (*(s16 *)(p + 0x4) >= *(s16 *)(p + 0x6))
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* 242 words, 0 differing (aligndiff 0 aligned differences). Eight
   semi-transparent LINE_F3 spokes per node, projected through the 0x1F800000
   scratchpad with RotAverage3 and chained into D_80083160's +0x8D0 primitive
   ring with the addPrim macro, then the same again for every node on the
   -0x8 list. OWNS NO TABLE.
   TWIN: the landed func_s14_800246D8 in csrc/ovl_dng_s14.c. dngtwin scored it
   BELOW 0.60 and the lead files said "no carved sibling"; what found it was
   its typedef name, UnkDngS14LineSP, whose scratchpad layout is this one with
   two vertices where this has three. Its `(u32)sp | 0x64' argument form is
   INVERTED here -- all eight RotAverage3 addresses are `addiu $aN, $s4, imm'
   off the pointer local, i.e. `&sp->unk_XX', where s14 needed the `ori'.
   FOUR PRICED FACTS, each re-measured on this body:
   (1) THE ANGLE STEP MUST BE A LOCAL ASSIGNED OUTSIDE THE INNER LOOP.
       `st = 0x100' before the loop, `stx = st * 2' inside it, keeps the
       target's `addiu $t0, $zero, 0x100' / `sll $s2, $t0, 1' / `mult'.
       Assigned INSIDE the loop it is 231 words / 230 differing: cse folds the
       constant, loop.c's move_movables hoists the set (which makes the pseudo
       invariant) and strength_reduce then turns `(i * 2 + 1) * st' into an
       `addu $17, $17, 512' induction variable -- `tools/cdkgcc -da' prints it
       as "giv at 163 reduced". Assigned outside, cse's basic block (which
       ends at the loop's CODE_LABEL) cannot see the value, the giv's mult_val
       stays a REG so loop.c rejects it for needing a multiply to increment,
       and reload rematerialises the constant at each use from its REG_EQUIV.
       Three separate literal 0x100 locals inside the loop measure the same
       231/230, and so does one `st' inside it. `stx' must be its own local:
       `i * (st * 2)' is reassociated by fold to `(i * 2) * st' and shifts the
       wrong register.
   (2) `g = &D_80083160' IS A POINTER LOCAL, and the two +0x8D0 reads go
       through it: spelled `D_80083160.unk_00' directly it is 241 words / 212
       differing, because reload then folds the symbol into the MEM instead of
       emitting the target's `lui' / `addiu' / `lw 0($t0)' triple twice.
       The ONE use outside the loops (`sp->unk_18 = ... + 0xB0') is spelled
       DIRECT and does get the folded `lw %lo(D_80083160)($v0)'.
   (3) `sc' IS COMPUTED BEFORE THE THREE vz STORES. After them it is 244 words
       / 220 differing.
   (4) The vz chain is `sp->unk_68 = sp->unk_70 = sp->unk_78 = ...', which
       emits 0x78/0x70/0x68 (right to left). Written the other way round it is
       242 words / 2 differing. */
s32 func_s22_8002445C(u8 *p, u8 *b)
{
    UnkDngS22LineSP *sp = (UnkDngS22LineSP *)0x1F800000;
    UnkNode *n;
    LINE_F3 *l;
    DR_MODE *dm;
    s32 i;
    s32 st;
    s32 stx;
    Unk80083160 *g;
    s32 g0;
    s32 g1;
    s32 g2;
    s16 sc;

    st = 0x100;
    g = &D_80083160;
    sp->unk_18 = D_80083160.unk_00 + 0xB0;
    while (1)
    {
        sc = (*(s16 *)(p + 0x4) + 1) * 32;
        sp->unk_68 = sp->unk_70 = sp->unk_78 = *(u16 *)(b + 0xA);
        for (i = 0; i < 8; i++)
        {
            l = *(LINE_F3 **)(g->unk_00 + 0x8D0);
            *(LINE_F3 **)(g->unk_00 + 0x8D0) = l + 1;
            setLineF3(l);
            setSemiTrans(l, 1);
            l->r0 = -0x80 - *(s16 *)(p + 0x4) * 16;
            l->g0 = 0;
            l->b0 = 0;
            stx = st * 2;
            g0 = i * stx;
            sp->unk_64 = *(u16 *)(b + 0x2) + ((func_800644B8(g0) >> 4) * sc >> 8);
            g1 = (i * 2 + 1) * st;
            sp->unk_6C = *(u16 *)(b + 0x2) + ((func_800644B8(g1) >> 4) * sc >> 8);
            g2 = (i * 2 + 2) * st;
            sp->unk_74 = *(u16 *)(b + 0x2) + ((func_800644B8(g2) >> 4) * sc >> 8);
            sp->unk_66 = *(u16 *)(b + 0x6) + ((func_80064584(g0) >> 4) * sc >> 8);
            sp->unk_6E = *(u16 *)(b + 0x6) + ((func_80064584(g1) >> 4) * sc >> 8);
            sp->unk_76 = *(u16 *)(b + 0x6) + ((func_80064584(g2) >> 4) * sc >> 8);
            sp->unk_B4 = RotAverage3((SVECTOR *)&sp->unk_64, (SVECTOR *)&sp->unk_6C, (SVECTOR *)&sp->unk_74, (long *)&sp->unk_D8,
                                     (long *)&sp->unk_DC, (long *)&sp->unk_E0, (long *)&sp->unk_84, (long *)&sp->unk_88);
            l->x0 = sp->unk_D8;
            l->y0 = sp->unk_DA;
            l->x1 = sp->unk_DC;
            l->y1 = sp->unk_DE;
            l->x2 = sp->unk_E0;
            l->y2 = sp->unk_E2;
            if ((u32)sp->unk_B4 < 0x1E0)
            {
                addPrim(sp->unk_18 + sp->unk_B4 * 4, l);
                dm = *(DR_MODE **)(g->unk_00 + 0x8D0);
                *(DR_MODE **)(g->unk_00 + 0x8D0) = dm + 1;
                func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
                addPrim(sp->unk_18 + sp->unk_B4 * 4, dm);
            }
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        b = (u8 *)n->unk_08;
    }
}

/* 53 words, 0 differing. Spawns the 0x212 node that runs
   func_s22_800243F8 as its step and func_s22_8002445C as its renderer, with
   the caller's UnkVec copied whole into the payload and e added to its z.
   ONE PRICED FACT, and it is the whole residue: `r = (u8 *)n + 0x20' MUST BE
   A POINTER LOCAL. Without it -- the three stores written
   `*(u8 **)((u8 *)n + 0x20)', `+ 0x24', `+ 0x26' -- the body is 52 words and
   41 aligned differences: the missing word is the target's
   `addiu $v0, $s0, 0x20', and the SAME omission also exchanges $s2 and $s3
   across the whole prologue, so the "wrong register within a class" residue
   and the short word count have one cause. combine folds the first store back
   onto `0x20($s0)' by itself, which is why the local is invisible at r+0. */
UnkNode *func_s22_80024824(u8 *v, UnkVec *q, s16 d, s16 e)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s22_800243F8;
        r = (u8 *)n + 0x20;
        *(u8 **)r = v;
        *(s16 *)(r + 0x4) = 0;
        *(s16 *)(r + 0x6) = d;
        m = n->unk_08;
        *m = *q;
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)m + 0xA) + e;
        func_8004491C(n, (u32)func_s22_8002445C);
        return n;
    }
    return NULL;
}

/* 48 words, 0 differing. ONE PRICED FACT: the tail gate is
   `*(s16 *)(p + 0x4) > *(s16 *)(p + 0x6)' and NOT `p + 0x6 < p + 0x4'. The
   wrong order is 48 words / 2 differing, and the two differing rows are the
   two `lh' loads swapped with `slt' and everything else identical -- the
   brief's comparison-operand lever, exactly as recorded.
   MEASURED AND FREE: an explicit `(s16)' cast on the fourth argument
   (48 w, 0), because func_s22_80024824's prototype already truncates. */
void func_s22_800248F8(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
    func_800478B8(c);
    if (*(s16 *)(p + 0x4) < 5)
    {
        func_s22_80024824(*(u8 **)p, b, 8, *(s16 *)(p + 0x4) * 0x10 - 0x40);
    }
    if (*(s16 *)(p + 0x4) > *(s16 *)(p + 0x6))
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* 35 words, 0 differing, first compile.
   TWIN: the landed func_s13_8002418C in csrc/ovl_dng_s13.c (LEADS.txt 0.958,
   36 words). Its statements carry verbatim; the one word of delta is that the
   twin's two stores on c+0x1E/+0x1C become this copy's single
   `*(s16 *)(b + 0xA) -= 4', on the UnkVec rather than the renderable. No
   device, and the twin names none. */
void func_s22_800249B8(u8 *p, u8 *b, u8 *c)
{
    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(b + 0xA) = *(u16 *)(b + 0xA) - 0x4;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* 140 words, 0 differing, first compile (aligndiff agrees). The burst
   spawner: one func_s22_800248F8 node carrying a copy of the caller's UnkVec
   and a func_s22_80024824 child, then eight func_s22_800249B8 nodes placed on
   a random ring. It landed first compile only because the eight smaller
   bodies were already in this file -- the `r = (u8 *)n + 0x20' local, the
   `*m = *q' UnkVec struct assignment and the 0x80 whitening all came from
   them.
   ONE PRICED FACT, and the save count is what found it: nine callee-saved
   registers against three parameters leaves five long-lived values, two of
   which are gcc's own hoists (the func_s22_800249B8 address and the 0x80), so
   the body has room for exactly ONE pointer local besides n and i. Writing
   n->unk_0C and n->unk_08 through the SAME `u8 *m' is 140 words / 0; a
   separate `UnkVec *k' for n->unk_08 is 139 words / 93 differing. */
void func_s22_80024A44(u8 *v, UnkVec *q, s16 d)
{
    UnkNode *n;
    UnkVec *k;
    u8 *m;
    u8 *r;
    s32 i;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s22_800248F8;
        r = (u8 *)n + 0x20;
        *(u8 **)r = v;
        *(s16 *)(r + 0x4) = 0;
        *(s16 *)(r + 0x6) = d;
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        func_8003DB94(m, D_800E2178, 0);
        *(s16 *)(m + 0x1E) = 0x1800;
        *(s16 *)(m + 0x1C) = 0x1800;
        func_8004491C(n, (u32)D_80045340);
        m = (u8 *)n->unk_08;
        *(UnkVec *)m = *q;
        func_s22_80024824(v, (UnkVec *)m, 8, -0x40);
    }
    for (i = 0; i < 8; i++)
    {
        n = func_8003FC64(0x212);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s22_800249B8;
            r = (u8 *)n + 0x20;
            *(u8 **)r = v;
            *(s16 *)(r + 0x4) = 0;
            *(s16 *)(r + 0x6) = d;
            m = (u8 *)n->unk_0C;
            m[0xE] = 0x80;
            m[0xD] = 0x80;
            m[0xC] = 0x80;
            func_8003DB94(m, D_800253BC, 0);
            *(s16 *)(m + 0x1E) = 0x800;
            *(s16 *)(m + 0x1C) = 0x800;
            *(u16 *)(m + 0x14) |= 0xC;
            *(u16 *)(m + 0x10) |= 0x60;
            func_8004491C(n, (u32)D_80045340);
            m = (u8 *)n->unk_08;
            *(UnkVec *)m = *q;
            *(s16 *)(m + 0x2) = *(u16 *)(m + 0x2) + ((func_800644B8(rand()) * 2) >> 8);
            *(s16 *)(m + 0x6) = *(u16 *)(m + 0x6) + ((func_80064584(rand()) * 2) >> 8);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s22", func_s22_80024C74);
