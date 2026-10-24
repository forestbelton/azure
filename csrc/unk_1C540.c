#include "common.h"

/* D_800814A0 is declared `extern u32` in structs.h and csrc/unk_12DEC.c passes
   its address to a function taking `u32 *`, so that declaration cannot become
   an incomplete array -- and the scalar spelling gets the assembler-expanded
   symbolic form, one `lui` per access. func_8004EE90 and func_8004F52C need
   the compiler-split form (one `lui` shared by the load and the store), which
   only the incomplete array gives, while func_8004B500 in this same unit needs
   the symbolic one. One carved unit standing in for several original
   translation units is why both appear against one symbol: an `__asm__` alias
   gives those two functions the array spelling under a different C name,
   without touching the declaration unk_12DEC depends on. */
extern u32 D_800814A0a[] __asm__("D_800814A0");

/* Bare file-scope size records: they put each symbol in maspsx's sdata_entries
   while DEFINING nothing, so .sdata stays 0 bytes, the symbols stay undefined
   and the linker script's own assignments resolve them -- and maspsx rewrites
   every load and store of them in this unit to %gp_rel, which is what these
   functions' listings have. Address materialisation stays absolute, so
   D_80080B18 is deliberately NOT here: func_80048DA0 indexes it through a
   materialised base. Adding a symbol here changes every access to it in this
   whole unit, so check `grep` before adding one. */
__asm__(".sdata\n\t.size D_80081550,4\n\t.size D_80081554,4\n"
        "\t.size D_80081558,4\n\t.size D_80080B94,4\n"
        "\t.size D_80080B98,4\n\t.size D_80081540,4\n"
        "\t.size D_80081568,4\n\t.size D_8008156C,4\n"
        "\t.size D_80080B58,4\n\t.size D_80080B5C,4\n"
        "\t.size D_80080B60,4\n\t.size D_80080B64,4\n"
        "\t.size D_80080B1B,1\n\t.size D_80080B1C,1\n\t.text");

void func_80048C3C(s32);

void func_80048D40(void) {
    func_80048C3C(2);
}

void func_8004B568(u8 *);

void func_80048D60(u8 *p) {
    u16 f = *(u16 *) (p - 2);

    if ((f & 0x2000) != 0) {
        *(u16 *) (p - 2) = f & 0xDFFF;
        *(u32 *) (p - 0x10) = (u32) D_800212DC;
        func_8004B568(p);
    }
}

u8 func_80048DA0(s32 i, s32 n) {
    D_80080B1B = n + 5;
    D_80080B1C = n * 2 + 5;
    return D_80080B18[i];
}

u32 func_80048DCC(s32 i) {
    return D_80071250[i];
}

u8 func_80048DE8(s32 i) {
    return D_80071298[i];
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80048E00);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80048EE4);

s32 func_80048F8C(u32 *p, s32 n) {
    s32 i = 0;

    while (i < n) {
        if (*p == 0) {
            break;
        }
        i++;
        p++;
    }
    return i;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80048FBC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049004);

s32 func_80048F8C(u32 *, s32);
u32 *func_8004B42C(u32 *, s32);

u8 *func_80049150(u8 *p, u32 n, s32 c) {
    s32 i = func_80048F8C((u32 *) p, c);
    u8 *q = p + i * 4;

    if (func_8004B42C((u32 *) q, n / 21 + 1) == 0) {
        q = 0;
    }
    return q;
}

void func_800491CC(u32 **p, u32 v, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        (*p++)[2] = v;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800491F4);

void func_80049250(u8 *p, u8 *n) {
    while (*(u8 **) (p + 0xC) != 0) {
        p = *(u8 **) (p + 0xC);
    }
    *(u8 **) (p + 0xC) = n;
}

s32 func_80049280(s32 a) {
    return a == 0x14;
}

s32 func_80049280(s32);

s32 func_8004928C(u8 *p) {
    return func_80049280(p[3] & 0x1F);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800492B0);

u32 func_800492B0(u32);

u32 func_80049330(u8 *p) {
    if (p[0] != 0) {
        return func_800492B0(p[3] & 0x1F);
    } else {
        return D_800814A8;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049374);

void func_80049374(s32, u8 *);

void func_80049490(s32 a) {
    func_80049374(a, D_800713FC);
}

s32 func_800B8C90(void);

void func_800494B4(s32 p) {
    u8 *q;

    /* One call after the join, not a call in each arm: in each arm the
       argument setup for p is duplicated and the jal's delay slot goes empty
       (14 DIFF). */
    if (func_800B8C90() != 0) {
        q = D_80080B34;
    } else {
        q = D_80080B40;
    }
    func_80049374(p, q);
}

s32 func_800494FC(u8 *p) {
    return func_80042A80(p) & 0x400;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004951C);

s32 func_8004951C(u8 *, s32);

u8 *func_80049790(s32 a) {
    u8 *p = (u8 *) func_8004B404(0x104);

    if (p != 0) {
        if (func_8004951C(p, a) == 0) {
            func_8004B1A4((s32) p);
            p = 0;
        }
    }
    return p;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800497F4);

typedef struct Unk498A0 {
    u8 unk_00[8];
} Unk498A0;

u8 func_800498A0(u8 *p) {
    u8 t[8];

    /* A byte-aligned 8-byte struct assignment: two lwl/lwr and two swl/swr
       pairs, not four word moves. */
    *(Unk498A0 *) t = *(Unk498A0 *) D_80080B20;
    return t[*(u32 *) (p + 0x14) & 7];
}

u32 func_800498EC(s32 i) {
    return D_8007361C[D_800712AC[i]];
}

u32 func_80049918(s32 i) {
    return D_8007361C[D_800712B0[i]];
}

s32 func_80049944(u32 a) {
    s32 r = 3;

    if (a & 1) {
        r = 0;
    } else if (a & 2) {
        r = 1;
    } else if (a & 4) {
        r = 2;
    }
    return r;
}

s32 func_80049944(u32);

s32 func_80049984(u8 *p) {
    s32 r = 3;

    if (p[0x13] != 0) {
        r = func_80049944(*(u32 *) (p + 0x14));
    }
    return r;
}

void func_800499BC(void) {
    D_80083D78[0] = -1;
    D_80083D78[2] = -1;
    D_80083D78[4] = -1;
    D_80083D78[6] = -1;
}

void func_800499DC(s32 *p) {
    *p = 1;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800499E8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049CF4);

void func_80049CF4(s32, s32);
void func_800499E8(u8 *, s32, u8 *);

void func_80049D80(u8 *p, s32 b) {
    func_80049CF4((s32) p, b);
    func_800499E8(p + 0x18, b, D_80080B54);
}

void func_80049CF4(s32, s32);

/* Two arguments that pass through untouched, which is invisible here and shows
   up at func_80049D80, which passes both. */
void func_80049DC8(s32 a, s32 b) {
    func_80049CF4(a, b);
}

/* The negative arm is a CONDITIONAL EXPRESSION, not an `if`: the target spends
   a copy into $v0 on each of its two paths and a copy back into $a0 at the
   join, which is what `?:` produces and what a plain `if (a < 0) a = m + a;`
   optimises away (12 words against 13, 6 DIFF). The positive arm is a plain
   `if`, in place. */
s32 func_80049DE8(s32 a, s32 d, s32 m) {
    a += d;
    if (d > 0) {
        if (a >= m) {
            a -= m;
        }
    } else {
        a = (a < 0) ? m + a : a;
    }
    return a;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049E1C);

u32 func_80049E6C(s32 i) {
    return D_800712B4[i];
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049E88);

void func_80049F2C(u8 *p) {
    *(u16 *) (p + 0x28) += (*(s16 *) (p + 0x4) - *(s16 *) (p + 0x28)) >> 1;
    *(u16 *) (p + 0x2A) += (*(s16 *) (p + 0x6) - *(s16 *) (p + 0x2A)) >> 1;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049F68);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A030);

u32 func_8004A0C0(u32 m) {
    /* The result, the counter and the pointer initialise in source order, and
       the counter's `addu $a1, $a2, $zero` is cse sharing the result's zero. */
    u32 r = 0;
    s32 i = 0;
    u8 *p = D_800712FC;

    for (; i < 0xD; i++) {
        if ((*(u32 *) (p + 4) & m) != 0) {
            r = *(u32 *) p;
            break;
        }
        p += 8;
    }
    return r;
}

s16 func_8004A10C(u8 *p) {
    s32 n;

    if (p == 0) {
        return 0;
    }
    n = func_8004B4DC(p);
    return *(s8 *) (p + n * 0xC - 0xA) + 0x80;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A170);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A24C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A330);

/* Both load-delay slots are filled in the target, and cc1psx's fill pass only
   ever swaps a load with the insn IMMEDIATELY after it: so the `addiu` for the
   +0x20 base has to stand right behind the `lhu`, and the first argument's
   load right behind the global's load. That is this statement order -- the
   flag update, then the base, then the argument read into a local, then the
   global. Any other order leaves cc1psx emitting its own `#nop` after the
   `lhu` and the function is a word long (14 DIFF over six orderings). */
void func_8004A464(u8 *p) {
    u8 *q;
    u8 *v;

    if (p != 0) {
        *(u16 *) (p + 0x1E) |= 0x8000;
        q = p + 0x20;
        v = *(u8 **) (q + 8);
        D_800814A0 |= 0x8000;
        func_8004B530(v);
        func_8004B530(*(u8 **) (q + 0x18));
    }
}

s32 func_8004A4C4(s32 arg0, u8 *p) {
    return func_800438E4(p);
}

s32 func_8004A4E4(s32 a, u8 *p) {
    a += ((u32) a / 10) * *(s8 *) (p + 2);
    if ((p[3] & 0x40) != 0) {
        a += a * 4;
    }
    if (a <= 0) {
        a = 1;
    }
    return a;
}

typedef struct Unk800102F0 {
    u8 pad_00[0x11];
    u8 unk_11;
    u8 pad_12[0x42];
} Unk800102F0;

u32 func_8004A534(u32 a, u8 *p) {
    /* A struct at the constant base, so the +0x11 stays in the load's
       displacement instead of folding onto the constant. */
    return a + (a >> 2) * ((Unk800102F0 *) 0x800102F0)[p[3] & 0x1F].unk_11;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A574);

void func_8004A574(s32, s32);

void func_8004A618(s32 a) {
    func_8004A574(a, 1);
}

void func_8004A574(s32, s32);

void func_8004A638(s32 a) {
    func_8004A574(a, 0);
}

/* The tail is written TWICE, once per arm, and jump2's cross-jumping merges
   it back into the single tail the listing shows -- so the duplication is
   invisible in the output but decides the registers. Written with a join
   block, the index is a global allocno that conflicts with the $v0 that
   local-alloc gives the `lw` in the join; written per arm, the whole tail is
   block-local, the index chain keeps $v0 and the load falls to $v1 -- which is
   also where `p` ends up, dying into its own load. 29 DIFF with every opcode
   already right.

   The base pointers are read into locals first (`b`, `t`) so that each symbol
   is materialised before the index that uses it: `&D_80073414[i]` alone folds
   to index+ADDR_EXPR, and the ADDR_EXPR is TREE_CONSTANT, so fold puts it
   second and the lui/addiu come out after the shifts. */
u32 func_8004A658(s32 i, s32 j) {
    u32 *t = D_8007361C;
    Unk80073414 *b = D_80073414;
    Unk80073414 *p = &b[i];

    if (p->unk_00 != 0) {
        return t[*(s16 *) (p->unk_0C + (j * 2 + j) * 4 + 2)];
    }
    return t[*(s16 *) (p->unk_0C + (j * 4 + j) * 4 + 2)];
}

/* The scan is INDEXED by the counter off a loop-invariant constant base, not a
   pointer walked with `p += 4`. Both spellings give the same 16 words, but a
   walked pointer whose only uses are the two loads is eliminated by loop.c,
   which biases the replacement giv's initial value to the second field
   (`ori 0x3566`, `lh -2`, `lh 0`) -- one word long and the whole reason this
   family sat parked. Indexed, the giv is `base + i * 4` and the two field
   offsets stay as the loads' displacements, which is what the listing shows:
   a bare `lui 0x8001` (0x80010000 has no low half) with 0x3564/0x3566 in the
   loads and `addiu 4` in the branch delay slot.

   s32 parameters, not s16: the target compares the `lh` results against $a0
   and $a1 as they arrive, with no sign-extension of its own. */
s32 func_8004A6C0(s32 x, s32 y) {
    u8 *b = (u8 *) 0x80010000;
    s32 i = 0;

    for (; i < 0x14; i++) {
        if (*(s16 *) (b + i * 4 + 0x3564) == x) {
            if (*(s16 *) (b + i * 4 + 0x3566) == y) {
                break;
            }
        }
    }
    return i;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A700);

s32 func_8004A6C0(s32, s32);

/* 20 entries of 19 bytes from 0x800133E8, the other half of the table
   func_8004A6C0 scans. */
u8 *func_8004A784(s32 x, s32 y) {
    u8 *r = 0;
    s32 i = func_8004A6C0(x, y);

    if (i < 0x14) {
        r = (u8 *) 0x800133E8 + i * 19;
    }
    return r;
}

s32 func_8004A6C0(s32, s32);

/* The two halfwords are at the CONSTANT base 0x80010000 with their offsets
   written inside the index expression -- 0x80010000's low half is zero, so the
   base is a bare `lui`, and 0x3564/0x3566 stay as the stores' displacements
   (splat prints them as %lo(D_80013564) and %lo(D_80013566) because both
   addresses are in its symbol list). Declaring the table as a symbol instead,
   in any of five spellings including a struct array and a one-set pointer
   local, always materialises the whole address with an `addiu %lo` and is a
   word long: cse folds a %lo into a MEM only for a CONSTANT offset off the
   `(high)` pseudo, never past a register index. 13 DIFF. */
void func_8004A7D8(s32 x, s32 y) {
    s32 i = func_8004A6C0(x, y);
    u8 *b = (u8 *) 0x80010000;

    if (i < 0x14) {
        bzero((u8 *) 0x800133E8 + i * 19, 0x13);
        *(s16 *) (b + i * 4 + 0x3564) = 0;
        *(s16 *) (b + i * 4 + 0x3566) = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A83C);

s32 func_8004A6C0(s32, s32);

/* The constant base is declared INSIDE the if, so its `lui` is the body's
   first insn and reorg lifts it into the `beqz`'s delay slot. At function
   scope the index comes out first instead (fold puts the constant second) and
   the two insns are in the other order -- 4 DIFF, with the call's result in
   $v1 rather than $a0 as a knock-on. */
void func_8004A8D8(s32 x, s32 y) {
    s32 i = func_8004A6C0(x, y);

    if (i < 0x14) {
        u8 *b = (u8 *) 0x80010000;

        *(s16 *) (b + i * 4 + 0x3564) = 0;
        *(s16 *) (b + i * 4 + 0x3566) = 0;
    }
}

void func_8004A918(void) {
    bzero((void *) 0x80013564, 0x50);
}

void func_8004A940(void) {
    s16 *k = (s16 *) 0x80013564;
    u8 *e = (u8 *) 0x800133E8;
    s32 i = 0;

    for (; i < 0x14; i++) {
        if (k[0] != 0) {
            *(u32 *) (D_80073414[k[0]].unk_0C + k[1] * 20 + 0xC) = (u32) e;
        }
        k += 2;
        e += 0x13;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A9B0);

s32 func_8004AA34(s32 *a, s32 *b) {
    return *a - *b;
}

s32 func_8004AA34(s32 *, s32 *);
void func_8004AFC8(s32, s32, s32, void *);

void func_8004AA44(s32 a, s32 b) {
    func_8004AFC8(b, a, 4, func_8004AA34);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004AA78);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004AB7C);

u32 func_8004AC18(s32 i) {
    return D_8007359C[0][i].unk_04;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004AC3C);

s32 func_8004AE3C(u32 *p) {
    s32 n = 0;

    while (*p != 0) {
        p++;
        n++;
    }
    return n;
}

s32 func_8004AE68(u8 **a, u8 **b) {
    return D_800713E4[(*a)[1]] - D_800713E4[(*b)[1]];
}

s32 func_8004AE98(u8 **a, u8 **b) {
    return D_80081540[(*a)[0]] - D_80081540[(*b)[0]];
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004AEC4);

/* A forwarding wrapper: its four arguments pass through untouched, which is
   invisible in the listing (no register setup before the jal) and only shows
   up at a caller -- func_8004AA44 passes four. */
void func_8004AEC4(s32, s32, s32, void *);

void func_8004AFC8(s32 a, s32 b, s32 c, void *d) {
    func_8004AEC4(a, b, c, d);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004AFE8);

void func_8004B028(u8 *m, u8 **list, s32 k) {
    s32 i = 0;
    u8 *e;
    u8 *q;

    while (*list != 0) {
        e = *list;
        if (e[1] == k) {
            q = m + e[0];
            if (*q == 0) {
                *q = i + 1;
            }
        }
        list++;
        i++;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B08C);

/* s32 rather than u8 *, because the two callers 280 lines below already
   declare it that way and pass their own s32 parameter straight through. */
void func_8004B1A4(s32 a) {
    if (a != 0) {
        *(u16 *) (a - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Indexed, so the walked pointer is a giv loop.c creates in the PREHEADER --
   after the `blez` -- where `p++` on the parameter puts the copy in the
   prologue instead (8 DIFF, the register saves in the other order). */
void func_8004B1DC(s32 *p, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        if (p[i] != 0) {
            func_8004B1A4(p[i]);
            p[i] = 0;
        }
    }
}

void func_8004B248(u32 *p) {
    while (*p != 0) {
        func_8004B1A4(*p);
        p++;
    }
}

void func_8004B298(s32 a, s32 b) {
    u32 *q = *(u32 **) (b + 4);

    if (q[0] == 0) {
        *(u16 *) (a - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

void func_8004B2E0(s32 a, s32 b) {
    s32 *n = (s32 *) b;

    /* `!= 0`: the flag goes in while the count is still nonzero. */
    *n -= 1;
    if (*n != 0) {
        *(u16 *) (a - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

void func_8004B298(s32, s32);
void func_8004B2E0(s32, s32);

void func_8004B324(s32 a, s32 b) {
    func_8004B298(a, b);
    func_8004B2E0(a, b);
}

void func_8004B324(s32, s32);

/* Three parameters, as both callers declare it, though the third only passes
   through to func_8003FC64 (whose own $a0 this zeroes). One return of the node
   pointer, advanced in place -- `return (u8 *)(n + 1)` from two places
   computes `$v1 + 0x20` twice, once into each guard's delay slot, and is a
   word short. */
u32 func_8004B364(s32 a, s32 b, s32 c) {
    UnkNode *n = func_8003FC64(0);

    if (n != NULL) {
        if (a != 0) {
            if (b != 0) {
                u8 *e = (u8 *) n->unk_08;

                *(s32 *) e = b;
                *(s32 *) (e + 4) = a;
                n->unk_10 = (u32) func_8004B324;
            }
        }
        n++;
    }
    return (u32) n;
}

u32 func_8004B364(s32, s32, s32);

void func_8004B3DC(s32 a) {
    func_8004B364(0, 1, a);
}

u32 func_8004B364(s32, s32, s32);

/* Returns what func_8004B364 returns: func_80048FBC stores the result. */
u32 func_8004B404(s32 a) {
    return func_8004B364(0, 0, a);
}

u32 *func_8004B42C(u32 *p, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        u32 v = func_8004B404(0x82);

        p[i] = v;
        if (v == 0) {
            func_8004B248(p);
            p = 0;
            break;
        }
    }
    return p;
}

UnkNode *func_8004B4A8(UnkNode *target) {
    UnkNode *p = D_80081498;

    while (p != 0) {
        if (p == target) {
            break;
        }
        p = *(UnkNode **) p;
    }
    return p;
}

s32 func_8004B4DC(u8 *p) {
    s32 n = 0;
    u8 v;

    /* The advance is inside the loop body and BEFORE the test, which is how it
       ends up in the backward branch's delay slot and so runs on the exiting
       iteration too. Latching the byte in `v` is what lets it. */
    do {
        n++;
        v = *p;
        p += 0xC;
    } while ((v & 0x80) == 0);
    return n;
}

void func_8004B500(u8 *p) {
    u32 g;

    if (p != 0) {
        g = D_800814A0;
        *(u16 *) (p + 0x1E) |= 0x8000;
        D_800814A0 = g | 0x8000;
    }
}

void func_8004B530(u8 *p) {
    if (p != 0) {
        *(u16 *) (p - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B568);

/* The sized alias, not the incomplete `D_80080B48[]`: at -G8 an object of 8
   bytes or fewer is small data, so its address is the ONE-insn `la` macro that
   maspsx splits afterwards -- and a pair maspsx made cannot be scheduled
   apart. Through the incomplete array the compiler splits it itself and sched1
   puts the `addiu $a0, 6` between the `lui` and the `addiu`, which is the 2
   DIFF this sat at. (The symbol is not in this unit's sdata_entries, so the
   expansion is absolute, not $gp-relative.) */
extern u8 D_80080B48s[8] __asm__("D_80080B48");

void func_8004B634(void) {
    func_8003E4FC(6, D_80080B48s, 0);
}

void func_8004B634(void);
void func_8003F320(void);
void func_800253A0(s32, s32, s32, s32);

void func_8004B660(s32 a, s32 b, s32 c) {
    func_8004B634();
    func_8003F320();
    func_800253A0(a, b, 1, c);
}

void func_8004B634(void);
void func_8003F320(void);
void func_800253A0(s32, s32, s32, s32);

void func_8004B6BC(s32 a, s32 b, s32 c) {
    func_8004B634();
    func_8003F320();
    func_800253A0(a, b, 2, c);
}

void func_8004B634(void);
void func_8003F320(void);
void func_800253A0(s32, s32, s32, s32);

void func_8004B718(s32 a, s32 b, s32 c) {
    func_8004B634();
    func_8003F320();
    func_800253A0(a, b, 0, c);
}

void func_8004B634(void);
void func_8003F320(void);
void func_800253A0(s32, s32, s32, s32);

void func_8004B774(s32 a, s32 b, s32 c) {
    func_8004B634();
    func_8003F320();
    func_800253A0(a, b, 3, c);
}

typedef struct Unk4B7D0 {
    u8 unk_00[4];
} Unk4B7D0;

void func_8004B634(void);
void func_8003F320(void);
void func_800253A0(s32, s32, s32, s32);

void func_8004B7D0(s32 a) {
    u8 t[4];

    *(Unk4B7D0 *) t = *(Unk4B7D0 *) D_80080B50;
    func_8004B634();
    func_8003F320();
    func_800253A0(a, 0, 0, (s32) t);
}

void func_8004B634(void);

void func_8004B834(void) {
    func_8004B634();
}

void func_8004B834(void);
void func_8003F320(void);
void func_8002537C(s32, s32, s32, s32, s32);

/* func_8004B834 takes no arguments. Passing a0/a1/a2 through to it costs no
   instructions either way -- the moves are deleted as no-ops -- but while they
   exist they are REFERENCES that lengthen each parameter's live range, and
   global-alloc's priority is refs*log(refs)/live_length. With the three extra
   refs `a` outranked `b` and took $s1; with the real (void) call the two
   parameters' ranges end in source order and they come out $s2/$s1. 6 DIFF. */
void func_8004B854(s32 a, s32 b, s32 c) {
    u8 t[2];

    func_8004B834();
    t[0] = c;
    t[1] = 0x13;
    func_8003F320();
    func_8002537C(a, 0, 0, (s32) t, b);
}

void func_8004B854(s32, s32, s32);

void func_8004B8BC(s32 a, s32 b) {
    func_8004B854(a, b, 0);
}

void func_8004B834(void);
void func_8003F320(void);
void func_800B6A0C(s32, s32, s32, s32, s32);

void func_8004B8DC(s32 a, s32 b, s32 c, s32 d, s32 e) {
    func_8004B834();
    func_8003F320();
    func_800B6A0C(a, b, c, d, e);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B954);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004BDDC);

u8 *func_8004C010(u8 *d, u8 *s) {
    u8 *p;
    s32 i;
    s32 v;

    if ((*(u32 *) s << 8) != 0x80808000) {
        p = d;
        /* All three increments in the `for`, in the target's order: with
           `p++`/`s++` in the body the counter is bumped after the pointer. */
        for (i = 0; i < 3; i++, p++, s++) {
            v = (*p * *s) >> 7;
            if (v < 0x100) {
                *p = v;
            } else {
                *p = 0xFF;
            }
        }
    }
    return d;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004C080);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004C36C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004CAA0);

u8 *func_8004CAE8(u8 *p, s32 k) {
    /* A do-while with the byte latched and the advance BEFORE the test, so
       the `addiu` lands in the back edge's delay slot and runs on the exiting
       iteration too -- the same shape as func_8004B4DC. The second read of
       p[0] is a second read in the source; cse does not share it across the
       0x20 mask. */
    u8 v;

    do {
        if ((p[0] & 0x20) != 0) {
            if (*(u16 *) (p + 6) == k) {
                return p;
            }
        }
        v = p[0];
        p += 0xC;
    } while ((v & 0x80) == 0);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004CB2C);

void func_8004CBFC(u32 *p, u8 *q, u32 *r) {
    p[2] = (u32) q;
    p[0] = *(u16 *) q / D_80080A84;
    p[1] = (u32) r;
    if (r != 0) {
        r[0] = *(u32 *) (q + 4);
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004CC38);

void func_8004CCBC(u8 **p, s32 n) {
    s32 i = 0;

    for (; i < n; i++) {
        /* `s` is assigned INSIDE the body so loop-invariant motion puts its
           materialisation in the preheader, after the `blez`; as an
           initialiser before the loop it lands ahead of the guard. And the
           first element is read through the symbol while the other two go
           through `s`, which is what the target's `%lo($t0)` against
           `0x4($a3)` is showing. */
        u32 *s = D_8002E5E8;

        (*(u32 **) (*p + 8))[0] = D_8002E5E8[0];
        (*(u32 **) (*p + 8))[1] = s[1];
        (*(u32 **) (*p + 8))[2] = s[2];
        p++;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004CD28);

void func_8004CE68(u8 *p, s32 f) {
    /* A do-while with the `p = q` at the TOP of the body and an explicit
       guard, not `while (p != 0)`: the loop condition is tested once, so the
       listing has no `p != 0` at the bottom, and the copy lands in the first
       branch's delay slot. */
    u8 *q = p;

    if (p != 0) {
        do {
            p = q;
            if (f != 0) {
                p[1] |= 2;
            } else {
                p[1] &= 0xFD;
            }
            if ((p[1] & 0xFC) == 0x38) {
                q = p + 0x18;
            } else {
                q = p + 0xC;
            }
        } while ((p[0] & 0x80) == 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004CECC);

u8 *func_8004D064(u8 *p) {
    /* The parameter is advanced in place and returned, which is the `addu
       $v0, $a0, $zero` at the end; computing `p + 0xC` into the return value
       instead fills both delay slots and loses a word. */
    switch (p[1] & 0xF0) {
    case 0x20:
        p += 0xC;
        break;
    case 0x30:
        p += 0x18;
        break;
    default:
        p += 0xC;
        break;
    }
    return p;
}

void func_8004D09C(u8 *p) {
    u16 *q = *(u16 **) (p + 0x20);

    D_80083160.unk_BC = q[1];
    D_80083160.unk_BE = q[3];
    D_80083160.unk_C0 = q[5];
}

void func_8004D0C8(u32 a) {
    D_80083178.unk_D8 = a;
    D_80083178.unk_B4 = (u32) func_8004D09C;
}

void func_8004D0E4(u8 *p) {
    u16 *q = *(u16 **) p;

    D_80083160.unk_BC = q[0];
    D_80083160.unk_BE = q[1];
    D_80083160.unk_C0 = q[2];
}

void func_8004D110(u32 a) {
    D_80083178.unk_B8 = a;
    D_80083178.unk_B4 = (u32) func_8004D0E4;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D12C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D1EC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D294);

void func_8004D294(s16 *, s32, u16);

void func_8004D45C(u8 *p, s32 b, u16 c) {
    s16 t[3];
    s16 *q = 0;

    if (p != 0) {
        t[0] = *(u16 *) (p + 0x2);
        t[1] = *(u16 *) (p + 0x6);
        t[2] = *(u16 *) (p + 0xA);
        q = t;
    }
    func_8004D294(q, b, c);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D4AC);

void func_8004D4AC(void);

void func_8004D5D0(void) {
    u8 *d = (u8 *) &D_80083178;
    u8 *s = D_80083780;

    *(u16 *) (d + 0xA4) = *(u16 *) (s + 0x2);
    *(u16 *) (d + 0xA6) = *(u16 *) (s + 0x6);
    *(u16 *) (d + 0xA8) = *(u16 *) (s + 0xA);
    func_8004D4AC();
}

/* $gp group: D_80080B58..D_80080B64 are in this unit's sdata_entries block
   (added for this function -- nothing else in the unit touches them), so their
   four loads are %gp_rel. Verified in a scratch file carrying the block, since
   verify.py compiles standalone. */
void func_8004D614(void) {
    u8 *p = (u8 *) &D_80083178;

    bzero(p + 0x9C, 8);
    *(u32 *) (p + 0x94) = D_80080B60;
    *(u32 *) (p + 0x98) = D_80080B64;
    *(u32 *) (p + 0xAC) = D_80080B58;
    *(u32 *) (p + 0xB0) = D_80080B5C;
    bzero(p + 0xA4, 8);
    *(u32 *) (p + 0x13C) = 0;
    *(u32 *) (p + 0x180) = 0;
    *(u32 *) (p + 0xB4) = 0;
    *(u32 *) (p + 0xF8) = 0;
    func_8004D0C8((u32) D_80083780);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D690);

void func_8004D690(u8 *, u8 *, s32);
void func_8004D4AC(void);

void func_8004D70C(void) {
    u8 *p = (u8 *) &D_80083160;

    func_8004D690(p + 0xCC, p + 0x110, 0);
    func_8004D690(p + 0x154, p + 0x198, 1);
    func_8004D4AC();
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D75C);

void func_8004D75C(u8 *, u8 *);

void func_8004D7A8(s32 a) {
    u8 *p = (u8 *) &D_80083178;

    if (a == 0) {
        func_8004D75C(p + 0xF8, p + 0xB4);
    } else {
        func_8004D75C(p + 0x180, p + 0x13C);
    }
}

void func_8004D7E8(s32 a) {
    u8 *p = (u8 *) &D_80083178;

    if (a == 0) {
        func_8004D75C(p + 0xB4, p + 0xF8);
    } else {
        func_8004D75C(p + 0x13C, p + 0x180);
    }
}

u8 func_8004D828(u16 k) {
    /* Indexed, not a walked pointer: through `u16 *p` and `p++` the base is
       materialised before the guard branch instead of in the loop's
       preheader (7 DIFF). */
    s32 i = 0;

    while (D_8007142C[i] != 0) {
        if (D_8007142C[i] == k) {
            break;
        }
        i++;
    }
    return D_8007147C[i];
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D880);

void func_8004D8F0(u32 a, u8 *p) {
    p[0] = (a & 0xF) << 3;
    p[1] = (a & 0xF0) - 0x20;
    p[2] = 8;
    p[3] = 0x10;
}

/* The `andi $a0, 0xFF` is the u8 parameter widening to func_8004D8F0's u32,
   not a mask in the source. */
void func_8004D91C(u8 a, s16 *out) {
    u8 t[4];

    func_8004D8F0(a, t);
    out[0] = (t[0] >> 2) + 0x3C0;
    out[1] = t[1] + 0x100;
    out[2] = t[2] >> 2;
    out[3] = t[3];
}

void func_8004D98C(u8 a, s32 b, u8 *p) {
    s32 t = b & 0xF;

    p[1] = 0x2C;
    *(u16 *) (p + 4) = 0x1F;
    p[0] = 0;
    *(u16 *) (p + 6) = t + 0x7C80;
    if ((b & 0x10) != 0) {
        *(u16 *) (p + 6) = t + 0x7C88;
    }
    func_8004D8F0(a, p + 8);
}

void func_8004D9E0(u8 *p, s32 b, s32 c, s32 d, s32 e) {
    /* `b * 4` is the LEFT operand of the outer add: the target's
       `addu $a1, $a1, $a3` puts the sum in the register holding b * 4. */
    p[2] = b * 4 + (b * d / 2 - 0x80);
    p[3] = c * (e + 0x10) - 0x80;
}

void func_8004DA24(u8 *p, u8 *q) {
    if (q != 0) {
        p[0x8] += q[0];
        p[0x9] += q[1];
        p[0xA] = q[2];
        p[0xB] = q[3];
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004DA74);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004DC14);

void func_8004DCA8(u8 *p) {
    if (p == 0) {
        p = D_80080B68;
    }
    /* The post-increment, not a separate read-and-store pair: it is what puts
       the `addiu` before the index's `addu` (2 DIFF the other way, 13 with the
       increment written as its own statement first). */
    D_80083D98[D_80081554++] = (u32) p;
}

void func_8004DCE0(u32 a) {
    D_80081550 = a;
}

void func_8004DCEC(void) {
    ((void (*)(u8 *)) D_800714A4[D_80081550])(D_80080B6C);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004DD2C);

void func_8004DDB4(void) {
    D_80081554 = 0;
    D_80081558 = 0;
}

void func_80033AE8(s32);

void func_8004DDC4(void) {
    func_80033AE8(2);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004DDE4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004DE88);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004DF8C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E0AC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E130);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E188);

typedef struct Unk4E21C {
    u8 unk_00[4];
} Unk4E21C;

void func_8004E188(u8 *, u8);

u8 *func_8004E21C(u8 *p, u8 b) {
    u8 t[16];

    func_8004E188(t, b);
    /* A byte-aligned 4-byte struct assignment: that is the lwl/lwr and
       swl/swr pair rather than a word move. */
    *(Unk4E21C *) p = *(Unk4E21C *) (t + 8);
    return p;
}

void func_8004E264(u16 *p, u32 v) {
    u32 t = (v & 0xF) + 0x7C80;

    p[3] = t + ((v & 0x10) << 3);
}

u8 *func_8004E280(u8 *p, s32 *out) {
    *out = *p - 0x30;
    return p + 1;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E298);

u8 *func_8004E478(u8 *p, u16 n) {
    /* The PARAMETER is the moving pointer and the saved copy is returned. */
    u8 *r = p;
    u8 *b = r + (n - 1);
    u8 x;
    u8 y;

    while (p < b) {
        x = *p;
        y = *b;
        *b-- = x;
        *p++ = y;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E4C0);

void func_8004E4C0(s32, u16, s32, s32);

void func_8004E57C(s32 a, u16 b, s32 c) {
    func_8004E4C0(a, b, c, 0x20);
}

u8 *func_8004E478(u8 *, u16);

u8 *func_8004E5A0(s32 a, u16 n, u8 *dst) {
    func_8004E57C(a, n, (s32) dst);
    func_8004E478(dst, n);
    return dst;
}

void func_8004E4C0(s32, u16, s32, s32);
u8 *func_8004E478(u8 *, u16);

u8 *func_8004E5E8(s32 a, u16 n, u8 *dst) {
    func_8004E4C0(a, n, (s32) dst, 0x30);
    func_8004E478(dst, n);
    return dst;
}

u8 *func_8004E478(u8 *, u16);

void func_8004E634(u32 n, u8 *s) {
    s32 len = 0;
    u32 q;

    do {
        q = n / 10;
        *s = n - q * 10 + 0x30;
        len++;
        s++;
        n = q;
    } while (n != 0);
    *s = 0;
    func_8004E478(s - len, len);
}

u8 *func_8004E69C(u8 *s) {
    u8 *p = s;
    u8 c;

    while (*p != 0) {
        c = *p;
        if ((D_800714B0[c] & 0x10) != 0) {
            *p = c + 0x31;
        }
        p++;
    }
    return s;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E6F4);

void func_8004E884(u8 *p, u32 b) {
    u8 v;

    if (p != 0) {
        do {
            func_8004E264((u16 *) p, b);
            v = p[0];
            p += 0xC;
        } while ((v & 0x80) == 0);
    }
}

s32 func_8004E8D8(u8 *p, s32 b) {
    s32 r = 0;
    u8 c = *p;

    while (c != 0) {
        if (c != 0x20 && c != 0xA) {
            r = 1;
            break;
        }
        p++;
        c = *p;
    }
    return r;
}

void func_8004E298(u8 *, s32, s32);

u8 *func_8004E928(s32 a, s32 b) {
    u8 *r = 0;

    if (func_8004E8D8((u8 *) a, b) != 0) {
        r = (u8 *) func_8004B404(0x100);
        if (r != 0) {
            func_8004E298(r, a, b);
        }
    }
    return r;
}

void func_8004E994(void) {
}

void func_8004B1A4(s32);

void func_8004E99C(s32 a) {
    if (a != 0) {
        func_8004B1A4(a);
    }
}

void func_8004B1A4(s32);

void func_8004E9C0(s32 a) {
    if (a != 0) {
        func_8004B1A4(a);
    }
}

void func_8004E9E4(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E9EC);

s32 func_8004E9EC(u8 *, s32);

u8 *func_8004EA70(u8 *p, s32 i) {
    func_8004E9EC(p, i);
    p += i * 2;
    strncpy(p, D_80080B70, 2);
    return p;
}

s32 func_8004E9EC(u8 *, s32);

/* The offset pointer is its OWN variable, not `dst += i * 2`. In-place, the
   parameter and the walked pointer are one quantity that spans both calls and
   the branch, so it is the function's only global allocno and takes the last
   callee-saved register, dragging the other two parameters round with it. As a
   separate local, `dst` dies at the `addu` and `q` is what lives, which is the
   $s1/$s2/$s0 assignment the target has. 13 DIFF of pure register naming. */
u8 *func_8004EAC4(u8 *dst, u8 *src, s32 i) {
    s32 r = func_8004E9EC(dst, i);
    u8 *q = dst + i * 2;

    strncpy(q, src, 2);
    if (r != 0) {
        q[2] = 0;
    }
    return q;
}

void func_8004EB30(void) {
    D_80084130[0] = 0;
}

void func_8004EB3C(u8 *p) {
    p[0] = 0;
    func_8003E4FC(6, D_8007164C, 0);
    func_8003E4FC(0xFF, (u8 *) func_8003E140, (s32) p);
}

void func_8004EB8C(u8 *p) {
    p[0] = 0;
    func_8003E4FC(6, D_80071644, 0);
    func_8003E4FC(0xFF, (u8 *) func_8003E140, (s32) p);
}

s32 func_8004EBDC(void) {
    s32 r = 0;

    if (func_80053EF0(1) == 0) {
        r = func_80053EF0(4) == 0;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004EC20);

void func_8004EE90(u8 *);

void func_8004ECAC(u8 *q) {
    u8 *p = q - 0x20;
    u16 f = *(u16 *) (p + 0x1E);
    u8 *r;

    if ((f & 0x2000) != 0) {
        *(u16 *) (p + 0x1E) = f & 0xDFFF;
        r = *(u8 **) q;
        *(u16 *) (r + 0x1E) |= 0x2000;
        func_8004EE90(p);
    }
}

void func_8004EB8C(u8 *);
void func_8004ECAC(u8 *);
/* The split spelling of D_80081480, whose own declaration is a scalar with
   users elsewhere: the symbolic form is one insn before maspsx, so its `lui`
   cannot go in the `beqz`'s delay slot where the target has it. */
extern u32 D_80081480a[] __asm__("D_80081480");

void func_8004ED00(u8 *p) {
    if (p[8] != 0) {
        func_80041344(0x80020000, D_80081480a[0]);
        DrawSync(0);
        func_8004EB8C(p + 9);
        *(u32 *) (p - 0x10) = (u32) func_8004ECAC;
    }
}

void func_8004EC20(void);
void func_8004ED00(u8 *);

void func_8004ED5C(u8 *p) {
    if (func_80044144(1, 0x1E, (u32) func_8004EC20, (u32) p) != 0) {
        *(u32 *) (p - 0x10) = (u32) func_8004ED00;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004EDA8);

void func_8004EDA8(s32, s32);

void func_8004EE30(s32 a) {
    func_8004EDA8(a, 0);
}

void func_8004EDA8(s32, s32);

void func_8004EE50(s32 a) {
    func_8004EDA8(a, 1);
}

void func_8004EDA8(s32, s32);

void func_8004EE70(s32 a) {
    func_8004EDA8(a, 2);
}

void func_8004EE90(u8 *p) {
    /* The global is read BEFORE the halfword store and written after, and that
       is load-bearing: in the split addressing form its MEM is register-based,
       so sched will not hoist the load above a store through a pointer it
       cannot disambiguate. Written as a plain `D_800814A0a[0] |= 0x8000;`
       after the halfword, the two chains stay serialised and the function is
       12 words against 11 (12 DIFF). The target interleaves them, which also
       costs the copy of the argument into $a1: with all four quantities live
       at once the global's value takes $a0. */
    u32 g;

    if (p != 0) {
        g = D_800814A0a[0];
        *(u16 *) (p + 0x1E) |= 0x8000;
        D_800814A0a[0] = g | 0x8000;
    }
}

void func_8004EEBC(u8 *p) {
    u32 *q = *(u32 **) (p + 4);

    if (q != 0) {
        if (q[0] != 0) {
            *(u32 *) (p + 0x28C) = 0;
            *(u32 *) (p + 0x29C) = (u32) (p + 0xF8);
        } else {
            *(u32 *) (p + 0x28C) = (u32) (p + 0x80);
            *(u32 *) (p + 0x29C) = 0;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004EEFC);

/* func_8004EFF4's shape: the two template words read into locals before the
   byte stores, through the incomplete-array spelling so their lui/lw pairs are
   the compiler's and can be scheduled apart. The `|= 2` then re-reads p[1]
   because the intervening store through p invalidated cse's entry for it --
   the parked note had this backwards, blaming the store ORDER. 24 DIFF. */
u8 *func_8004EF90(u8 *p) {
    u32 a;
    u32 b;

    p[1] = 0xB0;
    *(u32 *) (p + 8) = D_80071660;
    a = D_80071668[0];
    b = D_80071664[0];
    p[1] |= 2;
    *(u32 *) (p + 4) = a;
    *(u32 *) (p + 0xC) = b;
    p += 0x18;
    p[1] = 0x81;
    *(u16 *) (p + 4) = 0x20;
    *(u16 *) (p + 6) = 0;
    p[0] |= 0x80;
    return p + 0xC;
}

/* The two template words are read into locals BEFORE the byte stores, because
   sched cannot lift a load above a store through a pointer it cannot
   disambiguate -- and they are read through the incomplete-array spelling, so
   the lui/lw pair is the compiler's and can be split apart, where the scalar
   spelling's pair is maspsx's and stays glued (10 DIFF with the loads in
   place, 26 with them left in the stores). The first word stays a scalar. */
u8 *func_8004EFF4(u8 *p) {
    u32 a;
    u32 b;

    p[1] = 0xB0;
    *(u32 *) (p + 8) = D_8007166C;
    a = D_80071674[0];
    b = D_80071670[0];
    p[1] |= 2;
    p[0] |= 0x60;
    *(u32 *) (p + 4) = a;
    *(u32 *) (p + 0xC) = b;
    p += 0x18;
    p[1] = 0x81;
    *(u16 *) (p + 4) = 0x20;
    *(u16 *) (p + 6) = 1;
    p[0] |= 0x80;
    return p + 0xC;
}

/* func_8004EFF4's twin; see the note there.
   The two template words are read into locals BEFORE the byte stores, because
   sched cannot lift a load above a store through a pointer it cannot
   disambiguate -- and they are read through the incomplete-array spelling, so
   the lui/lw pair is the compiler's and can be split apart, where the scalar
   spelling's pair is maspsx's and stays glued (10 DIFF with the loads in
   place, 26 with them left in the stores). The first word stays a scalar. */
u8 *func_8004F068(u8 *p) {
    u32 a;
    u32 b;

    p[1] = 0xB0;
    *(u32 *) (p + 8) = D_80071678;
    a = D_80071680[0];
    b = D_8007167C[0];
    p[1] |= 2;
    p[0] |= 0x61;
    *(u32 *) (p + 4) = a;
    *(u32 *) (p + 0xC) = b;
    p += 0x18;
    p[1] = 0x81;
    *(u16 *) (p + 4) = 0x20;
    *(u16 *) (p + 6) = 1;
    p[0] |= 0x80;
    return p + 0xC;
}

/* Seven entries of the same shape. The inner pointer at +4 is re-read for the
   second store because the first one goes through it, which is what every
   repeated `lw 0x4($a3)` in the listing is. */
void func_8004F0DC(u8 *p) {
    u8 *q;

    q = *(u8 **) (p + 0x220);
    *(s16 *) (*(u8 **) (q + 4) + 8) = 6;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0x10;
    *(u8 **) q = p + 8;
    q = *(u8 **) (p + 0x234);
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0;
    *(u8 **) q = p + 0x80;
    q = *(u8 **) (p + 0x238);
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0x68;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0;
    *(u8 **) q = p + 0xF8;
    q = *(u8 **) (p + 0x224);
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0xA0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0x70;
    *(u8 **) q = p + 0x170;
    q = *(u8 **) (p + 0x228);
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0x70;
    *(u8 **) q = p + 0x170;
    q = *(u8 **) (p + 0x22C);
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0xA0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0;
    *(u8 **) q = p + 0x170;
    q = *(u8 **) (p + 0x230);
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0;
    *(u8 **) q = p + 0x170;
}

u8 *func_8004F1E8(u8 *p) {
    p[1] = 0x80;
    p[8] = 0;
    p[9] = 0;
    p[0xA] = 0xFF;
    p[0xB] = 0xFF;
    p += 0xC;
    p[1] = 0x2C;
    *(s16 *) (p + 4) = GetTPage(1, 0, 0x380, 0x100);
    *(s16 *) (p + 6) = GetClut(0, 0x1F5);
    p[0xA] = 0xA0;
    p[8] = 0;
    p[9] = 0;
    p[0xB] = 0x70;
    p += 0xC;
    p[8] = 0x70;
    p[9] = 0xE0;
    p[1] = 0x80;
    p[0xA] = 8;
    p[0xB] = 8;
    p += 0xC;
    p[1] = 0x81;
    *(u16 *) (p + 4) = 0x20;
    *(u16 *) (p + 6) = 0;
    p[0] = 0x80;
    return p + 0xC;
}

u8 *func_8004EF90(u8 *);
u8 *func_8004EFF4(u8 *);
u8 *func_8004F068(u8 *);
u8 *func_8004F1E8(u8 *);
void func_8004F0DC(u8 *);

void func_8004F2BC(u8 *p) {
    func_8004EF90(p + 0x8);
    func_8004EFF4(p + 0x80);
    func_8004F068(p + 0xF8);
    func_8004F1E8(p + 0x170);
    func_8004F0DC(p);
}

void func_800491F4(s32, u8 *, s32);

void func_8004F308(s32 a) {
    func_800491F4(a, D_80071684, 7);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F330);

void func_8004F3D0(u32 *p, u32 v) {
    *p = v;
}

void func_8004EEFC(u8 *, u8 *, u8 *);
void func_8004F2BC(u8 *);

void func_8004F3D8(u8 *p, u32 v) {
    func_8004F3D0((u32 *) p, v);
    func_8004EEFC(p + 0x210, p + 0x1E8, p + 0x1F8);
    func_8004F2BC(p);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F418);

void func_8004F52C(u8 *p) {
    u32 g;

    if (p != 0) {
        g = D_800814A0a[0];
        *(u16 *) (p + 0x1E) |= 0x8000;
        D_800814A0a[0] = g | 0x8000;
    }
}

void func_8004FE78(u32);

void func_8004F558(u8 *p) {
    func_8004FE78(*(u32 *) (p + 4));
    *(u16 *) (p - 2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

void func_8004F558(u8 *);

void func_8004F5B0(u8 *p) {
    if ((D_80083170[0] & 0x20) != 0) {
        func_80053DA8(0x515);
        func_8004F558(p);
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F5F4);

void func_8004F67C(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F684);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F780);

s32 func_8004F85C(u32 *p, u32 *q) {
    u32 *a = (u32 *) ((u32 *) ((u32 *) p[0])[3])[1];
    u32 *b = (u32 *) ((u32 *) ((u32 *) q[0])[3])[1];

    return *(s16 *) ((u8 *) a + 0xC) - *(s16 *) ((u8 *) b + 0xC);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F884);

void func_8004F884(u8 *);

void func_8004F95C(u8 *p) {
    func_8004F884(p);
    /* The post-increment INSIDE the condition: the old value is compared and
       the incremented one stored, which is the `addu $v1, $v0, $zero` copy.
       Read, compare, then store as three statements is a word shorter. */
    if ((*(s32 *) (p + 0x2C))++ >= *(s32 *) (p + 0x28)) {
        *(u32 *) p = (u32) func_8004F67C;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F9AC);

void func_8004F9AC(u8 *);
void func_8004F95C(u8 *);
s32 func_80049DE8(s32, s32, s32);

void func_8004FA2C(u8 *p, s32 b) {
    *(u32 *) (p + 0x2C) = 0;
    if (*(u32 *) p == (u32) func_8004F9AC) {
        *(u32 *) (p + 0x20) = 0x20;
        func_8004F9AC(p);
    }
    *(u32 *) p = (u32) func_8004F95C;
    *(u32 *) (p + 0x34) = *(u32 *) (p + 0x30);
    *(u32 *) (p + 0x30) = func_80049DE8(*(u32 *) (p + 0x30), b, *(u32 *) (p + 0x24));
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004FAA4);

void func_8004FAA4(u32 *);

void func_8004FC2C(u32 *p) {
    ((void (*)(u32 *)) p[0])(p);
    func_8004FAA4(p);
}

u32 func_80021448(u8 *);

void func_8004FC68(u8 *p) {
    *(u32 *) (p + 0x24) = func_80021448(p + 0x10);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004FC98);

u32 func_8002137C(s32);

/* The call is the FIRST declaration, so the counter is initialised after it
   and never crosses it: it stays in $v1 and the function spends one
   callee-saved register. Declared before the call, `i` crosses it, takes $s0
   and pushes the pointer to $s1 -- 28 DIFF and two words longer. */
s32 func_8004FD78(u8 *p, s32 b) {
    u32 v = func_8002137C(b);
    s32 i = 0;
    s32 n = *(s32 *) (p + 0x24);

    while (i < n) {
        if (*(u32 *) (p + i * 4 + 0x10) == v) {
            break;
        }
        i++;
    }
    return i;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004FDE0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004FE78);

u32 func_8004FF00(u8 *p) {
    u32 *q = (u32 *) (p + 0x20);

    return q[q[12] + 4];
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004FF20);

s32 func_8004FF20(u8 *);
void func_80050EA8(u8 *);

s32 func_8004FFA8(u8 *p) {
    s32 r = func_8004FF20(p);

    if (r != 0) {
        *(u32 *) (p - 0x10) = *(u32 *) (p + 4);
        func_80050EA8(p - 0x20);
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004FFF4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800500B4);

void func_80021300(u32);

void func_800502E0(u8 *p) {
    func_80021300(func_8004FF00(p));
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050308);

void func_80050308(u8 *);

/* The argument passes through untouched, which is invisible here and shows up
   at func_80050640. */
void func_80050494(u8 *p) {
    func_80050308(p);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800504B4);

/* Three quotients first, then the stores. The three bytes go through one local
   copy of the +0x4 pointer and the halfword at +0xC re-reads it, which is what
   the two `lw 0x4($a0)` in the listing are: a byte store through a loaded
   pointer invalidates cse's entry for the pointer itself.

   s32 for the second parameter, not the u32 the caller's prototype said: the
   three divisions are signed. */
void func_80050550(u8 *p, s32 b, s32 d) {
    s32 x = b * 80 / d;
    s32 y = -(b << 7) / d;
    s32 z = (b << 10) / d;
    u8 *q = *(u8 **) (p + 4);

    q[2] = x + 0x30;
    q[1] = x + 0x30;
    q[0] = x + 0x30;
    *(s16 *) (*(u8 **) (p + 4) + 0xC) = 0x180 - y;
    *(s16 *) (*(u8 **) (p + 8) + 2) = 0x400 - z;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800505BC);

void func_80050550(u8 *, s32, s32);
void func_800505BC(void);

void func_80050640(u8 *p) {
    /* The post-increment is the ARGUMENT: the old value goes in $a1 and the
       incremented one is stored, which is the copy in the listing. */
    func_80050550(p + 0x58, (*(u32 *) (p + 8))++, 3);
    *(u32 *) p = (u32) func_800505BC;
    func_80050308(p);
}

void func_80050694(u8 *p) {
    (*(void (**)(u8 *)) p)(p);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800506BC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005084C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800508F0);

void func_800508F0(u8 *, u8 *);

void func_8005097C(u8 *p) {
    func_800508F0(p, D_80080B88);
    func_800499E8(p + 0x18, (s32) D_80080B88, D_80080B90);
}

void func_8005097C(u8 *);
void func_80050640(u8 *);

void func_800509C4(u8 *p, s32 a) {
    *(s32 *) (p + 0x1C) = a;
    func_8005097C(*(u8 **) (p + 0x2C));
    *(u32 *) p = (u32) func_80050640;
}

void func_800509C4(u8 *, s32);
void func_8005084C(u8 *, u8 *, u8 *, u8 *);
void func_800506BC(u32, u8 *, s32);

void func_80050A00(u8 *p, s32 b) {
    func_800509C4(p, b);
    func_8005084C(p, p + 0x58, p + 0x30, p + 0x40);
    func_800506BC(*(u32 *) (p + 0x68), p, b);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050A58);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050AF0);

void func_800491F4(s32, u8 *, s32);

void func_80050B7C(s32 a) {
    func_800491F4(a, D_800717BC, 0x11);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050BA4);

void func_80050BA4(s32);

void func_80050CAC(void) {
    s32 a = 0;

    if (D_80080B94 != 0) {
        a = D_80080B98;
    }
    func_80050BA4(a);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050CDC);

void func_80050CDC(void);
void func_8004FE78(u32);

/* $gp group: the store to D_80080B98 is %gp_rel because the symbol is in this
   unit's sdata_entries block at the top of the file -- so this one has to be
   verified in a scratch file carrying that block, not by sweep.py (which
   compiles standalone and would report it wrong).

   Both loads are read into locals before the two stores through `q`: sched
   cannot lift a load above a store it cannot disambiguate, so left inside the
   call and the assignment they come out after both stores (4 DIFF). */
void func_80050DA8(u8 *p) {
    u8 *q = p + 0x20;

    if (p != 0) {
        u32 v;
        u32 arg;
        u8 *e;
        u32 g;
        u16 f;

        *(u32 *) (q + 8) = 2;
        v = *(u32 *) (q + 0x1C);
        arg = *(u32 *) (q + 0x28);
        *(u32 *) (q - 0x10) = (u32) func_80050CDC;
        D_80080B98 = v;
        func_8004FE78(arg);
        e = *(u8 **) (q + 0xA0);
        g = D_800814A0;
        f = *(u16 *) (e + 0x1E);
        D_800814A0 = g | 0x8000;
        *(u16 *) (e + 0x1E) = f | 0x8000;
        *(u32 *) (q + 0x28) = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050E20);

void func_80050EA8(u8 *p) {
    u8 *q = p + 0x20;
    u32 v = *(u32 *) (p + 0x10);

    if (v != 0) {
        *(u32 *) (q + 0x24) = v;
        *(u32 *) (p + 0x10) = 0;
    }
    func_80044A50((UnkNode *) p);
    func_8004FE78(*(u32 *) (q + 0x28));
    *(u32 *) (q + 0x28) = 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050EF8);

void func_80050F74(u8 *p, u32 v) {
    u32 *q = (u32 *) (p + 0x20);

    q[6] = q[7] = v;
}

void func_8003AEF8(u8 *);

void func_80050F84(void) {
    func_8003AEF8(D_800717D0);
}

void func_8003AEF8(u8 *);

void func_80050FA8(void) {
    func_8003AEF8(D_800717E8);
}

void func_80050FCC(void) {
    func_80041284(&D_80080B9C);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050FF4);

void func_80050FF4(u8 *);
u32 func_80126890(u32);

void func_800510A0(u8 *p) {
    func_80050FF4(p + 0xC);
    *(u32 *) (p + 0x8) = func_80126890(*(u32 *) (p + 0x4));
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800510DC);

void func_800510DC(void);
void func_800510A0(u8 *);

void func_80051164(u8 *p) {
    if (func_80044144(1, 0x1E, (u32) func_800510A0, (u32) p) != 0) {
        *(u32 *) (*(u8 **) (p + 4) + 0x10) = (u32) func_800510DC;
    }
}

void func_80051164(u8 *);

UnkNode *func_800511B4(UnkNode *p) {
    UnkNode *n = func_8003FC64(0);
    u8 *e;

    if (n != NULL) {
        *(UnkNode **) (n + 1) = p;
        n->unk_1E &= 0xDFFF;
        p->unk_1E &= 0xDFFF;
        e = (u8 *) (n + 1);
        *(UnkNode **) (e + 4) = n;
        e[0xC] = 0;
        n->unk_10 = (u32) func_80051164;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80051228);

void func_80051500(void) {
    func_80041284(&D_80080C1C);
}

void func_80051548(void);

void func_80051528(u8 *p) {
    if (*(u32 *) (p + 0x10) == (u32) func_80051548) {
        *(u16 *) (p + 0x2C) = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80051548);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80051708);

void func_800517CC(void);

void func_800517AC(u8 *p) {
    if (*(u32 *) (p + 0x10) == (u32) func_800517CC) {
        *(u16 *) (p + 0x2C) = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800517CC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80051BFC);

void func_80051CC4(void);

void func_80051CA4(u8 *p) {
    if (*(u32 *) (p + 0x10) == (u32) func_80051CC4) {
        *(u16 *) (p + 0x2C) = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80051CC4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80051E94);

void func_80051F58(void);

void func_80051F38(u8 *p) {
    if (*(u32 *) (p + 0x10) == (u32) func_80051F58) {
        *(u16 *) (p + 0x2C) = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80051F58);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800520B4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80052144);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800525D4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80052774);

/* The counter is consumed and incremented in one expression at the TOP of the
   body (`i++ << 4`), which is where the target's `addiu $a0, $a0, 1` sits --
   as a `for`'s increment it comes out last (12 DIFF). The ten stores go
   through a walked pointer, which loop.c splits into the two induction
   variables the listing has: one at +0 and one biased to +0xB. */
void func_80052A20(void) {
    u8 *p = D_80084478;
    s32 i = 0;

    do {
        u8 v = i++ << 4;

        p[0] = 0xC0;
        p[1] = 0x2C;
        p[2] = 0;
        p[3] = 0;
        *(s16 *) (p + 4) = 0xE;
        *(s16 *) (p + 6) = 0x7080;
        p[8] = 0;
        p[9] = v;
        p[0xA] = 0xFC;
        p[0xB] = 0x10;
        p += 0xC;
    } while (i < 0x10);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80052A90);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80052C10);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80052CE0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80052FE8);

void func_8005313C(void);

void func_800530A4(u8 *p) {
    if (*(u32 *) (p + 0x10) == (u32) func_8005313C) {
        *(u16 *) (p + 0x2A) = 1;
    }
}

/* Both read-modify-writes in the loop are split into a read, a read, then the
   two writes: cc1psx fills a load's delay slot with the insn that follows it
   in the source and nothing else, so the global's `lw` needs the entry's `lhu`
   next (17 DIFF with the two `|=` written whole, one nop each).

   The last `D_800814A0 |= 0x8000` re-uses the value the loop left in the
   register -- section 5's exception, since the global's address does not
   vary. */
void func_800530C4(u8 *p) {
    u8 **b = (u8 **) (p + 0x20);
    s32 i;

    *(u32 *) (*b + 0x1C) = 0;
    *(u16 *) (*b + 6) += 1;
    for (i = 0; i < 0x10; i++) {
        u8 *e = b[i + 4];
        u32 g = D_800814A0;
        u16 f = *(u16 *) (e + 0x1E);

        D_800814A0 = g | 0x8000;
        *(u16 *) (e + 0x1E) = f | 0x8000;
    }
    *(u16 *) ((u8 *) b - 2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005313C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80053374);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80053428);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80053604);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800537D0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80053858);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80053A88);

void func_80053C3C(u8 *p) {
    func_8003E4FC(6, p, 0);
    func_8003F320();
}

void func_80053C6C(void) {
    u32 *b = (u32 *) D_8006ADBC;
    u32 v = b[1];

    *(u32 *) D_8006ADBC = v;
    if (v != 0) {
        func_80053C3C((u8 *) v);
    }
}

void func_80053C6C(void);

void func_80053CA4(void) {
    D_8006ADC0[0] = (u32) D_800812C8;
    func_80053C6C();
}

void func_80053C6C(void);

void func_80053CD0(void) {
    D_8006ADC0[0] = (u32) D_800812D0;
    func_80053C6C();
}

/* $gp group (D_80081568 and D_8008156C added to the unit's sdata_entries
   block; nothing else in the unit touches them). The pair is filled with an
   8-BYTE STRUCT COPY, which is what makes the following `D_80081568` read a
   real `lw` again: as two word stores cse keeps the value it just stored and
   the function is a word short. Verified in a scratch file carrying the block,
   since verify.py compiles standalone. */
void func_80053CFC(u32 *p, u32 b) {
    *(Unk80081568 *) &D_80081568 = *(Unk80081568 *) p;
    D_80081568 = (((D_80081568 + 0x7FF) >> 11) << 23) | (b & 0x7FFFFF);
    func_8003E4FC(6, (u8 *) &D_80081568, b);
    func_8003F320();
}

s32 func_80053D64(void) {
    u8 loc[8];

    if (CdSync(1, 0) == 2) {
        CdControl(0x10, 0, loc);
    }
    return CdPosToInt(loc);
}

s32 func_80055778(s32);

s16 func_80053DA8(u16 a) {
    return func_80055778(a);
}

void func_80054D64(s32);

void func_80053DCC(s32 a) {
    D_8008480C[0] = a;
    func_80054D64(a);
}

void func_800552C8(s32);

void func_80053DF0(s32 a) {
    D_8008480A[0] = a;
    func_800552C8(a);
}

void func_80053E14(s16 v) {
    D_80084808[0] = v;
}

void func_800552C8(s32);
void func_80054D64(s32);

/* The case bodies are emitted in SOURCE order, which the compare tree (built
   from the sorted values) does not show: case 4 first, then 1, then 2, which
   is why the `bne` for 4 falls straight into its own body. Written 2, 1, 4 the
   three blocks come out in the other order -- 18 DIFF with every instruction
   right. Both callees take the first parameter, which arrives in $a0 and is
   never moved. */
void func_80053E20(s32 a, s32 k) {
    switch (k) {
    case 4:
        D_8008480C[0] = a;
        func_80054D64(a);
        break;
    case 1:
        D_8008480A[0] = a;
        func_800552C8(a);
        break;
    case 2:
        D_80084808[0] = a;
        break;
    }
}

/* func_80053E20's getter, same case order. */
s16 func_80053E90(s32 k) {
    switch (k) {
    case 4:
        return D_8008480C[0];
    case 1:
        return D_8008480A[0];
    case 2:
        return D_80084808[0];
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80053EF0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80053F98);

/* Nested ifs with ONE return 0 at the end, and the 1 returned from the inner
   arm. Every shape that assigns a variable in the arms puts the result in a
   callee-chosen register with a `move $v0` at the end: a variable live to the
   single return is a global allocno, local-alloc has already given the second
   mask chain $v0, and the result is pushed to $a1. Returning directly makes
   hard reg $v0 live from each arm, so local-alloc routes the second chain
   around it ($v1/$a0) exactly as the target does. 13 DIFF.

   `r = 1` before the second test and `r = 0` in the arm is also what jump.c
   store-flags into one `sltu`, which three earlier spellings all hit. */
s16 func_8005405C(s16 n) {
    if ((D_800847D0[1] & (0x1000000 << n)) == 0) {
        if ((D_800847D0[0] & (0x10000 << n)) != 0) {
            return 1;
        }
    }
    return 0;
}

void func_800540A8(void) {
    u8 *p;

    func_8005A4E8(0, 0, 0);
    func_80055990(D_80084858);
    /* Assigned AFTER the calls: as an initialiser it lives across them in
       $s0, which costs the save, the restore and the frame slot (3 words). */
    p = (u8 *) D_800847D0;
    *(u32 *) (p + 0x8) = 0;
    *(u32 *) (p + 0xC) = 0;
    p[0x30] = 0;
    p[0x32] = 0;
    *(u32 *) (p + 0x10) = 0;
    *(u32 *) (p + 0x14) = 0;
    p[0x31] = 0;
    p[0x33] = 0;
    *(u32 *) (p + 0x18) = 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054104);

void func_8005E7E0(s32);

void func_800541E8(void) {
    func_8005B300();
    func_8005B418(0);
    func_8005B348();
    func_8005E7E0(4);
    func_8005B320();
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054228);

s32 func_800542A4(void) {
    return 0;
}

void func_800542AC(void) {
    D_800847F6[0] = -1;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800542BC);

void func_80055CF4(void);
void func_800544A4(void);
void func_8005440C(void);

void func_800543C8(void) {
    if ((D_800847D4 & 0xFF000000) != 0) {
        func_80055CF4();
    }
    func_800544A4();
    func_8005440C();
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005440C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800544A4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054538);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800545F4);

s32 func_800545F4(u8 *);
void func_80054E00(s32);

void func_800546B0(void) {
    if ((u32) func_80053D64() >= D_800847DC ||
        func_800545F4(D_80084858) == -1) {
        func_80054E00(0x74);
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054704);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054788);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005497C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800549FC);

void func_80055B44(u16);
void func_800540A8(void);
void func_800541E8(void);

/* Cases 1 and 2 share a body and there is no default: {1,2} is a RANGE node,
   and after `!= 3`, `< 4` and `!= 0` nothing is left to test, so
   node_is_bounded holds for it and emit_case_nodes branches on the LEFT with
   `blt` instead of the usual `bgt` on the right -- which is the branch sense
   the listing shows. Written as a default, the test comes out the other way
   round and the trailing `j` goes to the default body instead of the
   epilogue. */
void func_80054A7C(u16 k) {
    switch (k & 0xFF) {
    case 1:
    case 2:
        func_80055B44(k);
        break;
    case 3:
        func_800540A8();
        break;
    case 4:
        func_800541E8();
        break;
    }
}

s32 func_80054AF0(s32 a) {
    if (a == 1) {
        return 0x5F;
    }
    return 0x7F;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054B08);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054C58);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054CD4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054D64);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054E00);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054F9C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005500C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800550E8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800552C8);

void func_8005537C(s16 a) {
    s16 v;

    D_800848F8[5] = a;
    if ((D_800847D0[0] & 0x20) != 0) {
        D_800848F8[5] = a - 0x18;
    }
    v = func_80055750(D_800848F8[5]);
    D_800848F8[5] = v;
    D_800848F8[4] = v;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800553D4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005552C);

void func_800555FC(void) {
}

void func_80055604(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005560C);

void func_8005BAB0(void);

void func_80055730(void) {
    func_8005BAB0();
}

s32 func_80055750(s16 a) {
    if (a < 0) {
        return 0;
    }
    if (a >= 0x80) {
        return 0x7F;
    }
    return a;
}

/* Push onto the D_80084778 ring. The guard is an EARLY RETURN and the success
   path returns its own 0: one variable assigned in both arms gave the target's
   instructions with $v1 and a copy, because a variable live through the join is
   a global allocno. With a return in each arm the whole tail is block-local,
   the 0 goes straight into $v0, and jump2's cross-jumping merges the two `jr`s
   back into the one the listing shows. 7 DIFF.

   The count is re-read after `++` because the store invalidated cse's entry for
   it -- matching-levers section 4 -- so the element written is [count + 1]. */
s32 func_80055778(s32 v) {
    if (D_80084778[0] >= 0x20) {
        return -1;
    }
    D_80084778[0]++;
    D_80084778[D_80084778[0]] = v;
    return 0;
}

void func_800557BC(void) {
    D_80084778[0] = 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800557C8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80055864);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800558FC);

void func_80055990(u8 *p) {
    *(u16 *) (p + 0x10) = 0x80;
    *(u32 *) (p + 0xC) = 3;
    *(u32 *) (p + 0x4) = 0;
    *(u16 *) (p + 0x16) = 0xA;
    *(u16 *) (p + 0x18) = 0xA;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800559B4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80055ADC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80055B44);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80055BD8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80055C50);

void func_80055CEC(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80055CF4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80055D84);

void func_80055E6C(void) {
}

void func_80055E74(void) {
}

void func_80055E7C(s32 a, s32 b, s32 c) {
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80055E84);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80056098);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800561D8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800563B0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80056408);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800564A8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80056558);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800565D8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80056654);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80056800);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800568B4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80056948);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80056A08);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80056C30);

void func_80056D3C(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80056D44);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80056DB4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80056E10);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80057948);

void func_80057A48(void) {
}

extern s16 D_80086D50a[] __asm__("D_80086D50");

/* The two address materialisations interleave -- `lui`, `lui`, `addiu`, `lh`
   -- which they can only do if BOTH are compiler-split pairs. As the scalar
   `D_80086D50`, cc1psx emits `lh $2,D_80086D50` as one macro insn and maspsx
   expands it into an adjacent lui/lh pair that nothing can schedule apart, so
   the pair lands after the other symbol's `addiu`: 2 DIFF that six statement
   orderings could not move. The array alias gives the split form.

   The 0x820 binds to the index term and not to the sum, so the index is its
   own local: written as one expression the constant folds onto the pointer
   instead and the final `addu`/`addiu` come out in the other order. */
u8 *func_80057A50(s32 *p, u16 j) {
    u8 *q = D_80086A40[D_80086D50a[0]].unk_04;
    s32 k = (((*p << 4) + j) << 5) + 0x820;

    return q + k;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80057A94);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80057D20);

void func_8005845C(u8 i, u8 v) {
    D_80084960[i].unk_00 = v;
}

void func_8005848C(void) {
}

void func_80058494(u8 i, s32 b, u8 v) {
    D_80084960[i].unk_1C = v & 0x7F;
    func_80055E7C(1, i, v);
}

void func_80059BC4(void);

void func_800584E4(void) {
    if (D_800737A4[0] != 0) {
        func_80059BC4();
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058510);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800585A0);

void func_8006A27C(u32);

void func_8005863C(void) {
    D_800737C0[0] = 1;
    EnterCriticalSection();
    func_8006A27C(0xF2000002);
    DisableEvent(D_80085F9C[0]);
    CloseEvent(D_80085F9C[0]);
    ExitCriticalSection();
    D_80085F98[0] = 0;
    D_800737C0[0] = 0;
}

void func_8006A27C(u32);

void func_800586B0(void) {
    D_800737C0[0] = 1;
    EnterCriticalSection();
    func_8006A27C(0xF2000002);
    ExitCriticalSection();
    D_80085F98[0] = 0;
    D_800737C0[0] = 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058700);

void func_800599B0(void);

void func_800587B4(void) {
    D_80085FA4[0] = 0x80080000;
    D_800869B0[0] = 0xC000;
    func_80056C30();
    func_800599B0();
    D_800737A4[0] = 1;
}

void func_800587F8(void) {
}

s32 func_80058800(u8 *a, u8 *b, s32 n) {
    if (n == 0) {
        return -1;
    }
    while (--n != 0) {
        if (*a != *b) {
            return *a - *b;
        }
        a++;
        b++;
    }
    return *a - *b;
}

s32 func_80058850(u32 i) {
    do {
        if (func_80058800(D_800330FC, (u8 *) (D_80085FA4[0] + i), 4) == 0) {
            return i + 4;
        }
        i++;
    } while (D_800869B0[0] >= i);
    return -1;
}

s32 func_800588C8(u32 i) {
    do {
        if (func_80058800(D_80033104, (u8 *) (D_80085FA4[0] + i), 4) == 0) {
            return i + 4;
        }
        i++;
    } while (D_800869B0[0] >= i);
    return -1;
}

s32 func_80058940(u32 i) {
    do {
        if (func_80058800(D_800737D0, (u8 *) (D_80085FA4[0] + i), 3) == 0) {
            return i + 3;
        }
        i++;
    } while (D_800869B0[0] >= i);
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800589B8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058A04);

u32 func_80058A7C(u32 a, u32 b, u32 c, u32 d) {
    return ((((((a & 0xFF) << 8) + (b & 0xFF)) << 8) + (c & 0xFF)) << 8) | (d & 0xFF);
}

u32 func_80058AA8(u32 a, u32 b) {
    return ((a & 0xFF) << 8) | (b & 0xFF);
}

u32 func_800589B8(s32);
u32 func_80058A7C(u32, u32, u32, u32);

/* Four u8 locals, so each result is masked again at the call. The first call
   needs no argument move -- $a0 still holds the parameter. */
s32 func_80058ABC(u8 *p) {
    u8 a = func_800589B8((s32) p);
    u8 b = func_800589B8((s32) p);
    u8 c = func_800589B8((s32) p);
    u8 d = func_800589B8((s32) p);

    return func_80058A7C(a, b, c, d);
}

u32 func_800589B8(s32);

u16 func_80058B2C(s32 a) {
    u8 x = func_800589B8(a);
    u8 y = func_800589B8(a);

    return func_80058AA8(x, y);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058B70);

void func_80058E50(s32 *p, s32 v) {
    *p += v;
}

void func_80058E64(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058E6C);

u32 func_80058A04(s32);
u32 func_800589B8(s32);

/* `func_80058A04(p)` emits nothing -- $a0 already holds p and cse deletes the
   no-op move -- but it is the FOURTH reference to p, which crosses the
   floor_log2 boundary in global-alloc's priority (2*4/11 against 1*3/9) and
   wins p the $s1 the target has. With three references n outranks it and the
   two swap. The increment is written FIRST in the body, which is what puts a
   copy of it in the preheader and the other in the bottom branch's delay
   slot. */
void func_800590E0(s32 p) {
    u32 i = 0;
    u32 n = func_80058A04(p);

    do {
        i++;
        if ((u8) func_800589B8(p) == 0xF7) {
            break;
        }
    } while (i < n);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005914C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005947C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800595C0);

s32 func_800588C8(u32);
s32 func_80058ABC(u8 *);

/* No local for the second result: the target stores it straight out of $v0 and
   only then copies it to $a1, because the `return 0` needs $v0. Reading
   `*(u32 *) (p + 0x18)` back for the sum is free -- cse disambiguates it from
   the +0x38 store -- while `*(u32 *) p` is re-read for real, the call having
   invalidated it. */
s32 func_800597A8(u8 *p) {
    s32 v = func_800588C8(*(u32 *) p);

    *(u32 *) p = v;
    if (v == -1) {
        return 1;
    }
    *(u32 *) (p + 0x18) = func_80058ABC(p);
    *(u32 *) (p + 0x38) = *(u32 *) p;
    *(u32 *) (p + 0x1C) = *(u32 *) p + *(u32 *) (p + 0x18);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80059814);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800599B0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80059BC4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80059D1C);

s32 func_80059D60(void) {
    u32 i;

    for (i = 0; i < D_800869B4; i++) {
        if (D_80085FA8s[i].unk_2C == 0) {
            return 1;
        }
    }
    return 3;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80059DAC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80059E94);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80059F8C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005A1D0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005A26C);

void func_8005D838(s32, u8 *);

void func_8005A2FC(void) {
    func_8005CA70();
    func_80056C30();
    func_8005A26C();
    func_8005D838(0x10, D_80086CC0);
}

void func_800585A0(void);

void func_8005A33C(void) {
    /* Nested ifs, not `> 0 && < 4`: written with && the two bounds combine
       into one `addiu -1` / `sltiu 3` range check (13 DIFF). */
    if ((s32) D_80086D4C[0] > 0) {
        if ((s32) D_80086D4C[0] < 4) {
            func_800585A0();
        }
    }
    D_80073824[0] = 1;
}

void func_8005A33C(void);

void func_8005A37C(void) {
    func_8005A33C();
}

void func_8005A39C(u32 v) {
    D_80086D4C[0] = v;
}

void func_80058700(void);

void func_8005A3A8(void) {
    if (D_80073824[0] != 0) {
        func_80058700();
    }
}

void func_8005A3D4(void) {
    D_8007382A[0] = 0;
}

void func_8005A3E0(void) {
    D_8007382A[0] = 1;
}

s32 func_8005A3F0(u32 a) {
    a &= 0xFF;
    if (a >= 0x19) {
        return 0xFF;
    }
    if (a == 0) {
        return 0xFF;
    }
    D_80073734[0] = a;
    return a;
}

void func_8005A420(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005A428);

void func_80056C30(void);
void func_8005A26C(void);
void func_8005D7BC(void);

void func_8005A4B8(void) {
    func_80056C30();
    func_8005A26C();
    func_8005D7BC();
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005A4E8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005A56C);

void func_8005EDA0(s16 *);

void func_8005A5C8(s16 x, s16 y) {
    s16 t[20];

    *(s32 *) &t[0] = 0xF;
    t[2] = x << 7;
    t[3] = y << 7;
    t[4] = 0;
    t[5] = 0;
    func_8005EDA0(t);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005A608);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005A778);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005A90C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005AAA8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005AB50);

s32 func_8005ECF8(s32);

s16 func_8005AC30(s16 a) {
    /* Two returns of the same call, not `func_8005ECF8(a != 0)`, which
       store-flags into one `sltu`. And the arm written FIRST ends up SECOND in
       memory: crossjumping merges the two tails and leaves the second arm as
       the fall-through, so `a == 0` first is what gives the target's `beqz`. */
    if (a == 0) {
        return func_8005ECF8(0);
    } else {
        return func_8005ECF8(1);
    }
}

void func_8005A1D0(u32);

/* The base read into a local first, so its lui/addiu come out ahead of the
   28-stride index chain (8 DIFF the other way round). The -1 is one constant
   shared by the compare and the store, which is why it sits in a callee-saved
   register across the call. */
void func_8005AC68(s16 i) {
    Unk80086A40 *b = D_80086A40;
    Unk80086A40 *p = &b[i];

    if (p->unk_00 != -1) {
        func_8005A1D0(p->unk_10);
        p->unk_00 = -1;
    }
}

void func_8005ACD0(u16 v) {
    D_80073828[0] = v;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005ACDC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005AE08);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005AE90);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005AF74);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005AFF4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005B070);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005B16C);

void func_800564A8(s32);

void func_8005B27C(s16 i, u8 a, u8 b) {
    D_80086C00[i].unk_08 = a & 0x7F;
    D_80086C00[i].unk_0A = b & 0x7F;
    func_800564A8(i);
}

void func_8005B2C8(s16 i, s32 b, u16 *x, u16 *y) {
    *x = D_80086C00[i].unk_08;
    *y = D_80086C00[i].unk_0A;
}

void func_8005ACDC(void);

void func_8005B300(void) {
    func_8005ACDC();
}

void func_8005E450(s32);
void func_8005D88C(s32);

void func_8005B320(void) {
    func_8005E450(1);
    func_8005D88C(1);
}

void func_8005D88C(s32);
void func_8005E450(s32);

void func_8005B348(void) {
    D_8007382B[0] = 0;
    func_8005D88C(0);
    func_8005E450(0);
}

s32 func_8005DA88(void *);

s16 func_8005B378(s16 a) {
    s32 t[6];

    t[0] = 1;
    t[1] = a;
    if (func_8005DA88(t) != 0) {
        return -1;
    }
    D_8007382B[0] = a;
    return a;
}

s32 func_8005DA88(void *);

void func_8005B3D8(s32 x, s32 y) {
    s16 t[12];

    *(s32 *) &t[0] = 6;
    /* `(x << 16) >> 8`, not `(s16) x << 8`: the two shifts cannot be combined
       (the arithmetic right shift fills from bit 15, so the result is not
       x << 8), which is why the target spends both. Any spelling that means
       "sign-extend then shift" collapses to one `sll` because the value is
       stored as a halfword. */
    t[4] = (x << 16) >> 8;
    t[5] = (y << 16) >> 8;
    func_8005DA88(t);
}

void func_8005E97C(s32, s32);

void func_8005B418(s32 a) {
    s32 i = 0;
    s32 r = 0;

    /* The bound is read from the global IN the loop condition, not hoisted
       into a local: loop-invariant motion then lifts that read into the
       preheader as a COPY of the guard's read, which is the target's
       `addu $a2, $v0, $zero`. With a local it is one read and one register,
       a word short. The `(s32)` is what makes the tests `blez`/`slt`. */
    for (; i < (s32) D_80073734[0]; i++) {
        r |= D_80073740[i];
    }
    func_8005E97C(0, r);
}

s32 func_8005B470(s16 i, u8 *dst) {
    u8 *src;
    u32 j;

    if (D_80086A40[i].unk_00 == -1) {
        return -1;
    }
    src = D_80086A40[i].unk_04;
    for (j = 0; j < 0x20; j++) {
        *dst = *src;
        src++;
        dst++;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005B4D0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005BAB0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005BBFC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005C130);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005C2C0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005C4D0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005C5D0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005C668);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005C6B4);

void func_8005FA34(u32 *);

/* A u32 block, not a u8 one: as `u8 t[0x40]` with `*(u32 *)` casts the two
   stores and the table load come out in the other order (3 DIFF). */
s32 func_8005C710(s16 i, s16 *a, s16 *b) {
    u32 t[0x10];

    t[1] = 0;
    t[0] = D_80073740[i];
    func_8005FA34(t);
    *a = *(u16 *) ((u8 *) t + 8);
    *b = *(u16 *) ((u8 *) t + 0xA);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005C784);

u32 func_8005C808(void) {
    return D_80085FCC[0];
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005C818);

s32 func_8005C858(s32 a, s16 i) {
    return D_80084960[i].unk_0C;
}

void func_80057D20(s16, s32, u32);

void func_8005C88C(s32 arg0, s16 b, u32 c) {
    func_80057D20(b, 7, c & 0x7F);
}

s32 func_8005C8B8(s32 a, s16 i) {
    return D_80084960[i].unk_04;
}

void func_80057D20(s16, s32, u32);

void func_8005C8EC(s32 arg0, s16 b, u32 c) {
    func_80057D20(b, 0xA, c & 0x7F);
}

s32 func_8005C918(s32 a, s16 i) {
    return D_80084960[i].unk_1C;
}

void func_8005C94C(s32 a, s16 i, u8 v) {
    D_80084960[i].unk_1C = v & 0x7F;
}

void func_8005C980(void) {
}

void func_8005C988(void) {
}

void func_8005C990(void) {
}

void func_8005C998(void) {
}

void func_8005C9A0(void) {
}
