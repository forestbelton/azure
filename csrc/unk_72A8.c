#include "common.h"

/* Bare file-scope size records: each symbol lands in maspsx's sdata_entries
   while nothing is DEFINED, so maspsx rewrites every load and store of it in
   this unit to %gp_rel while leaving address materialisation absolute. See the
   $gp section of docs/carved-sweep.md; adding a symbol changes every access to
   it in this whole unit, so grep this file first.
   D_8008099C is deliberately NOT here, and cannot be: func_80034EF8 and
   func_8003C0C8 in this same unit reach it ABSOLUTELY and match that way,
   while func_80033D44 and func_80033D54 need it $gp-relatively. One carved
   unit cannot give one symbol both forms, so those two stay stubs until the
   unit is split. */
__asm__(".sdata\n\t.size D_80080A6C,4\n\t.size D_80081440,4\n"
        "\t.size D_80081448,4\n\t.size D_80081460,4\n"
        "\t.size D_800809B8,4\n\t.size D_800809BC,4\n"
        "\t.text");

/* The bitmap set, clear and test on the out-of-image words at 0x80012D70,
   whose base 0x80010000 has no low half, so 0x2D70 stays in the load and store
   displacements. The base is assigned AFTER the two guards so its `lui` is the
   first insn of the following block and reorg lifts it into the second guard's
   delay slot; at the top of the function the index chain is emitted first and
   the `lui` lands after the `sll` (3 DIFF). Adding the index to the base in a
   separate statement (`p += ...`) also hands the `addu`'s destination to the
   pointer's quantity instead of the index chain's, which permutes $a0/$v0
   through the whole function (7 DIFF). */
void func_80033AA8(s32 id) {
    u32 k = id & 0xFFFF;
    u8 *p;

    if (k == 0) {
        return;
    }
    if (k == 1) {
        return;
    }
    p = (u8 *) 0x80010000;
    *(u32 *) (p + (k >> 5) * 4 + 0x2D70) |= 1 << (id & 0x1F);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80033AE8);

s32 func_80033B2C(s32 id) {
    u32 k = id & 0xFFFF;
    u8 *p;

    if (k == 0) {
        return 0;
    }
    if (k == 1) {
        return 1;
    }
    p = (u8 *) 0x80010000;
    return (*(u32 *) (p + (k >> 5) * 4 + 0x2D70) & (1 << (id & 0x1F))) != 0;
}

void func_80033AE8(s32);

void func_80033B78(s16 a) {
    func_80033AE8(a);
}

void func_80033AA8(s32);

void func_80033B9C(s16 a) {
    func_80033AA8(a);
}

s32 func_80033B2C(s32);

void func_80033BC0(s16 a) {
    func_80033B2C(a);
}

u8 *func_80033BE4(void) {
    u8 *p = D_800816C0;
    s32 i;

    for (i = 0; i < 0x14; i++) {
        if (p[0] == 0) {
            return p;
        }
        p += 0x64;
    }
    return 0;
}

void func_80033C1C(u8 *p, u32 a) {
    *(u32 *) (p + 0x00) = 1;
    *(u32 *) (p + 0x04) = a;
    *(u16 *) (p + 0x08) = 0;
    *(u16 *) (p + 0x0A) = 0;
    *(u16 *) (p + 0x0C) = 0;
    *(u32 *) (p + 0x10) = 0;
    *(u32 *) (p + 0x14) = 0;
    *(u32 *) (p + 0x18) = 0;
    *(u32 *) (p + 0x20) = 0x1000;
    *(u32 *) (p + 0x24) = 0x1000;
    *(u32 *) (p + 0x28) = 0x1000;
    *(u32 *) (p + 0x30) = 0;
    *(u32 *) (p + 0x34) = 0;
    *(u32 *) (p + 0x38) = 0;
    *(u32 *) (p + 0x40) = 0;
    *(u32 *) (p + 0x44) = 0;
    *(u32 *) (p + 0x48) = 0;
    *(u32 *) (p + 0x4C) = 0;
    *(u32 *) (p + 0x50) = 0;
    *(u32 *) (p + 0x54) = 0;
    *(u32 *) (p + 0x58) = 0;
    *(u32 *) (p + 0x5C) = 0;
    *(u32 *) (p + 0x60) = 0;
}

void func_80033C84(u32 *p) {
    *p = 0;
}

void func_80033C8C(void) {
    u8 *p = D_800816C0;
    s32 i;

    for (i = 0; i < 0x14; i++) {
        func_80033C84((u32 *) p);
        p += 0x64;
    }
}

void func_80033CD8(u8 *p, u32 tag) {
    if (*(u32 *) (p - 0xC) == 0) {
        func_8004491C((UnkNode *) (p - 0x20), tag);
    }
}

void func_80033D08(u8 *p) {
    if (*(u32 *) (p - 0xC) != 0) {
        func_80044A50((UnkNode *) (p - 0x20));
    }
}

s32 func_80033D38(u32 *p) {
    return p[-3] != 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80033D44);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80033D54);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800345B8);

void func_80034E40(void) {
}

void func_8003C920(void);
void func_80034EC4(u8 *);

void func_80034E48(void) {
    u8 *p;

    func_8003C920();
    p = (u8 *) D_80082A38;
    func_80034EB4(p);
    func_80034EC4(p);
    ClearImage(D_800809A8, 0, 0, 0);
    ClearImage(D_800809B0, 0, 0, 0);
}

void func_80034EB4(u8 *p) {
    p[0] = 0;
    p[1] = 0;
    *(u32 *) (p + 0x34) = 0;
}

void func_80034EC4(u8 *p) {
    u8 *q = p + 8;
    /* A local for the table base, so the symbol is materialised once and both
       words stay displacements off it. */
    u8 *b = D_8006ADBC;

    p[2] = 2;
    *(u8 **) (p + 0x10) = q;
    *(u8 **) (p + 0xC) = q;
    *(u32 *) (p + 0x44) = *(u32 *) (b + 0x20);
    *(u32 *) (p + 0x40) = *(u32 *) (b + 0x1C);
}

void func_80037D50(u8 *, u8);

void func_80034EF8(u8 *p, u8 v) {
    /* The table byte is read into a local BEFORE the last two stores, which is
       what fills the load's delay slot; as the last statement's RHS the load
       lands next to its own use and costs a nop. */
    u8 c;

    D_8008099C = 0;
    func_80037D50(p, v);
    p[2] = v;
    p[0] = 0;
    p[1] = 0;
    c = D_8007216C[0];
    p[0x3D] = 0;
    *(u32 *) (p + 0x34) = 0;
    p[0x3C] = c;
}

void func_80035484(u8 *, u8 *, s32);

void func_80034F58(s32 a) {
    func_80035484(D_800820A8, D_80081EB0, a);
}

void func_80034F88(u32 *p, u32 v) {
    p[13] = v;
    p[14] = v;
    p[66] = (u32) &p[50];
}

void func_80034F9C(u8 *p) {
    if (func_80034FD0((u32 *) p) != 0) {
        p[0] = 1;
    }
}

s32 func_80034FD0(u32 *p) {
    return p[4] != (u32) &p[2];
}

s32 func_80034FE4(u32 *p) {
    return p[39] != (u32) func_80038A00;
}

/* The split spelling: the scalar D_80082E6A's load is one insn before maspsx
   and comes out in one register, where the target has the pair across two. */
extern u8 D_80082E6Aa[] __asm__("D_80082E6A");

s32 func_80034FFC(u32 *p) {
    s32 r;

    /* The `!= 2` arm is written FIRST so the bare `return func_80034FE4();`
       ends up last in memory, which is where the target has it. */
    if (D_80082E6Aa[0] != 2) {
        r = 0;
        if (func_80034FE4(p) != 0 || D_8006ADE8[0] != 0) {
            r = 1;
        }
        return r;
    }
    return func_80034FE4(p);
}

void func_80035060(u8 *p, u32 v) {
    p[3] = v;
}

void func_80035068(u8 *p, u8 *n) {
    *(u8 **) (n + 0x4) = p + 0x8;
    *(u8 **) (n + 0x8) = *(u8 **) (p + 0x10);
    *(u8 **) (*(u8 **) (p + 0x10) + 0x4) = n;
    *(u8 **) (p + 0x10) = n;
}

void func_80035090(u32 *p) {
    ((u32 *) p[1])[2] = p[2];
    ((u32 *) p[2])[1] = p[1];
}

void func_800350B0(u8 *a, u8 *p) {
    *(u8 **) (p + 0x54) = p;
    func_80035068(a, p + 0x54);
}

void func_80033C84(u32 *);
void func_80035090(u32 *);
void func_8003FFF0(u8 *);

/* Drains the list at p+0x10 (the head is p+8, the forward link +8 off it):
   each link stores the node that owns it, so `e` is the node and `e - 0x20` is
   its allocation header. The head is re-read by the loop condition, which is
   what the target does -- the three calls can move it. */
void func_800350D4(u8 *p) {
    u8 *h = p + 8;
    u8 *n;
    u8 *e;
    u8 *d;

    while ((n = *(u8 **) (h + 8)) != h) {
        e = *(u8 **) n;
        if (*(u32 **) e != 0) {
            func_80033C84(*(u32 **) e);
        }
        func_80035090((u32 *) n);
        d = e - 0x20;
        func_80044A50((UnkNode *) d);
        func_8003FFF0(d);
    }
}

void func_80037D9C(u8 *, u8);

/* Both calls written out in all three arms: an `&&` or a nested `if` inverts
   both branch senses (28 DIFF either way), and a shared `u8 *q = (u8 *) p;`
   local costs a fourth callee-saved register. gcc's cross-jumping merges the
   tails back to the target's single func_80037D9C and func_80034F88 sites. */
void func_80035164(u8 *p, s32 b, u8 c) {
    if (func_80034FD0((u32 *) p) != 0) {
        if (p[0] != 0) {
            func_800350D4(p);
            func_80037D9C(p, c);
        } else if (p[2] != c) {
            func_800350D4(p);
            func_80037D9C(p, c);
        } else {
            func_80034F88((u32 *) p, b);
            return;
        }
    } else {
        func_80037D9C(p, c);
    }
    func_80034F88((u32 *) p, b);
}

void func_80035208(s32 a) {
    func_80035164((u8 *) D_80082A38, a, 2);
}

void func_80035234(s32 a) {
    func_80035164((u8 *) D_80082A38, a, 1);
}

void func_80035260(s32 a) {
    func_80035164((u8 *) D_80082A38, a, 0);
}

void func_8003528C(u32 v) {
    func_80034F88((u32 *) D_80082A38, v);
}

void func_800352B4(void) {
    func_80034F9C((u8 *) D_80082A38);
}

void func_800352D8(void) {
    func_80034FD0((u32 *) D_80082A38);
}

s32 func_80034FFC(u32 *);

void func_800352FC(void) {
    func_80034FFC((u32 *) D_80082A38);
}

void func_80035320(u32 v) {
    func_80035060((u8 *) D_80082A38, v);
}

void func_80035348(void) {
    u32 *b = (u32 *) D_80082A38;
    s32 i;

    b[0xE] = 0;
    for (i = 0xA; i < 0xF; i++) {
        b[0x12 + i] = 0;
    }
}

/* The five-slot queue at D_80082A38+0x48, indexed 10..14. The 0x48 is added to
   `&D_80082A38w[i]` rather than written inside the index: as `[i + 0x12]`
   loop.c folds 0x48 into the giv's initial value and the function is four
   words longer. D_80082AB8 is the last slot reached as its own symbol, through
   the incomplete-array spelling so the store shares the `lui` instead of going
   through the assembler's $at. */
void func_80035378(u32 v) {
    s32 i;

    for (i = 10; i < 15; i++) {
        u8 *q = (u8 *) &D_80082A38w[i];

        if (*(u32 *) (q + 0x48) == 0) {
            *(u32 *) (q + 0x48) = v;
            if (i != 14) {
                *(u32 *) (q + 0x4C) = 0;
            }
            return;
        }
    }
    for (i = 11; i < 14; i++) {
        u8 *q = (u8 *) &D_80082A38w[i];

        *(u32 *) (q + 0x48) = *(u32 *) (q + 0x4C);
    }
    D_80082AB8[0] = v;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800353F4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035484);

void func_800355EC(u8 *p) {
    (*(void (**)(u8 *)) (p + 0x68))(p);
}

void func_80035888(u8 *, u8 *, u8 *, s32, u32);
void func_8003566C(u8 *);

void func_80035614(u8 *p) {
    func_80035888(D_800821CC, D_80081F14, p, 0, *(u32 *) (p + 0x74));
    p[0x4C] = 0;
    *(u32 *) (p + 0x68) = (u32) func_8003566C;
}

extern u8 D_800822F0[];
extern u8 D_80081F78[];
extern u8 D_8006A86C[];
void func_80035E58(u8 *, u8 *, u8 *, u8 *, s32, u32);
void func_8003571C(u8 *);

/* THREE separate locals for the three reads of `*(u8 **) p`, not one `q`
   reassigned: as one variable it is one allocno and the pointer and the
   halfword swap registers in the first statement (8 DIFF). */
void func_8003566C(u8 *p) {
    u8 *q;
    u8 *r;
    u8 *s;

    q = *(u8 **) p;
    *(u16 *) (q + 8) = *(u16 *) (q + 8) - 0xA0;
    r = *(u8 **) p;
    if (*(s16 *) (r + 8) < 0) {
        *(s16 *) (r + 8) = 0;
    }
    s = *(u8 **) p;
    if (*(s16 *) (s + 8) < 0x100) {
        func_80035E58(D_800822F0, D_80081F78, D_8006A86C, p, 1,
                      *(u32 *) (p + 0x74));
        p[0x4D] = 0;
        *(u32 *) (p + 0x68) = (u32) func_8003571C;
    }
}

void func_80035760(u8 *);

void func_8003571C(u8 *p) {
    *(u16 *) (*(u8 **) p + 8) -= 0xA0;
    if (*(s16 *) (*(u8 **) p + 8) < 0) {
        *(u16 *) (*(u8 **) p + 8) = 0;
        *(u32 *) (p + 0x68) = (u32) func_80035760;
    }
}

void func_800357B0(u8 *);

void func_80035760(u8 *p) {
    u8 *q = *(u8 **) (p + 0x74);

    if (*q == 1 || p[0x51] == 0xFF) {
        *q = 1;
        p[0x4C] = 0xFE;
        p[0x4D] = 0xFE;
        p[0x51] = 0;
        *(u32 *) (p + 0x68) = (u32) func_800357B0;
    }
}

void func_800357D0(u8 *);

void func_800357B0(u8 *p) {
    if (*(u16 *) (p + 0x50) == 0x101) {
        *(u32 *) (p + 0x68) = (u32) func_800357D0;
    }
}

void func_80035820(u8 *);

void func_800357D0(u8 *p) {
    *(u16 *) (*(u8 **) p + 8) += 0x40;
    if (*(s16 *) (*(u8 **) p + 8) > 0x400) {
        p[0x4C] = 0xFF;
        p[0x4D] = 0xFF;
        *(u32 *) (p + 0x68) = (u32) func_80035820;
    }
}

void func_8003FFF0(u8 *);

void func_80035820(u8 *p) {
    u8 *q;

    if (*(u16 *) (p + 0x50) == 0x101) {
        func_80034EB4(*(u8 **) (p + 0x74));
        func_80033C84(*(u32 **) p);
        func_80035090((u32 *) (p + 0x54));
        q = p - 0x20;
        func_80044A50((UnkNode *) q);
        func_8003FFF0(q);
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035888);

void func_80035CE4(u8 *, u8 *, s32);
void func_80037C7C(u8 *, s16);

/* The +0x44 pointer and the +0x4A index in SEPARATE statements, pointer
   first: that is the func_800368D0 family's load-order lever (sched1's
   tie-break is RTL order), and here the registers come out right as well, so
   it lands. Both calls take their arguments straight from the parameter
   registers -- nothing sets $a0 before either `jal`. */
void func_800359FC(u8 *p, u8 *q, s32 r) {
    u8 *b;
    s16 i;
    u8 *e;

    if (func_80037534(p, (s32) q, r) != 0) {
        return;
    }
    if (p[0x48] != 0) {
        b = *(u8 **) (p + 0x44);
        i = *(s16 *) (p + 0x4A);
        e = b + i;
        if (e[0x4C] == 0xFE) {
            func_8003C24C();
            *(u32 *) (p + 0x68) = (u32) func_80035CE4;
        }
    }
    (*(void (**)(u8 *, u8 *, s32)) (p + 0x68))(p, q, r);
    func_80037C7C(p, *(s16 *) (q + 2));
}

void func_80035AAC(u32 *p) {
    p[0x1A] = (u32) func_80035AF8;
}

void func_80035ABC(u8 *p, u8 *q) {
    u16 *v = (u16 *) (q + 2);

    *v -= 0x10;
    if ((s16) *v < 2) {
        *v = 2;
        *(u32 *) (p + 0x68) = (u32) func_80035AF8;
    }
}

void func_8003AE60(s16, s16, s16, s16, u8, u8, u8);
u32 func_80035DBC(s32);
void func_8003AC1C(s16, s16, u8, u8 *);
u32 *func_8003C8E4(s32);
s32 func_8003C890(s32, s32);
void func_8003C2B8(void);
void func_80035BE0(u8 *);
void func_80035C44(u8 *, u8 *);

void func_80035AF8(u8 *p)
{
    u8 *e;
    u8 n;

    n = *(u8 *)(*(u8 **)(p + 0x74) + 1);
    if (n == 0)
    {
        return;
    }
    p[0x78] = n;
    func_8003AE60(*(s16 *)(p + 0x14), *(s16 *)(p + 0x16), 0x30, 0x10, 0, 0, 0);
    func_8003AC1C(*(s16 *)(p + 0x14), *(s16 *)(p + 0x16), 1, (u8 *)func_80035DBC(p[0x78]));
    e = (u8 *)func_8003C8E4(p[0x78]);
    if (e != 0)
    {
        u8 c = e[2];

        p[0x79] = c;
        if (func_8003C890(p[0x78], (s8)c) != 0)
        {
            e[2] = e[3];
            func_8003C2B8();
            *(s16 *)(p + 0x64) = 1;
            *(u32 *)(p + 0x68) = (u32)func_80035BE0;
        }
        else
        {
            *(u32 *)(p + 0x68) = (u32)func_80035C44;
        }
    }
    else
    {
        *(u32 *)(p + 0x68) = (u32)func_80035C44;
    }
}

s32 func_8003C7D8(s32, s32);
void func_8003C160(void);
void func_80035C44(u8 *, u8 *);

void func_80035BE0(u8 *p) {
    u16 *c = (u16 *) (p + 0x64);

    *c = *c - 1;
    if ((s16) *c < 0) {
        func_8003C7D8(p[0x78], *(s8 *) (p + 0x79));
        func_8003C160();
        *(u32 *) (p + 0x68) = (u32) func_80035C44;
    }
}

void func_80035C80(u8 *);

void func_80035C44(u8 *p, u8 *q) {
    u16 *v = (u16 *) (q + 2);

    *v += 0x10;
    if ((s16) *v > 0x40) {
        *v = 0x40;
        *(u32 *) (p + 0x68) = (u32) func_80035C80;
    }
}

void func_80035D38(void);

/* The func_800368D0 family: four functions that all end by storing through
   `*(u8 **)(p + 0x44) + *(s16 *)(p + 0x4A)`, and all four were two registers
   wrong at the exact word count until func_80036F24 showed the spelling. The
   +0x44 pointer takes a local -- that is what puts its `lw` ahead of the `lh`,
   sched1's tie-break being RTL order -- and the index stays INSIDE the
   subscript. Given its own local the index is a second quantity with the
   shorter live range, so local-alloc hands it $v0 and the pointer $v1, the
   reverse of every one of these listings, and the `addu`'s operands swap with
   them. func_800359FC above is the one member that wanted both as locals. */
void func_80035C80(u8 *p)
{
    u8 *t;

    if (p[0x78] != (*(u8 **)(p + 0x74))[1])
    {
        t = *(u8 **)(p + 0x44);
        (t + *(s16 *)(p + 0x4A))[0x50] = 1;
        func_8003C24C();
        *(u32 *)(p + 0x68) = (u32)func_80035ABC;
    }
}

void func_80035CE4(u8 *p, u8 *q, s32 r)
{
    u8 *t;

    *(u16 *)(q + 2) = *(u16 *)(q + 2) - 0x20;
    if (*(s16 *)(q + 2) < 2)
    {
        *(u16 *)(q + 2) = 2;
        *(u32 *)(p + 0x68) = (u32)func_80035D38;
        t = *(u8 **)(p + 0x44);
        (t + *(s16 *)(p + 0x4A))[0x50] = 1;
    }
}

void func_80035D38(void) {
}

void func_80035D40(u32 v) {
    D_8006ADB8[0] = v;
}

/* `k == p[0]`, not `p[0] == k`: the operand order survives into the `bne`. */
void func_80035D4C(s16 k) {
    s16 *p = D_8006A94C;

    while (p[0] != 0) {
        if (k == p[0]) {
            func_80033AA8(p[1]);
        }
        p += 2;
    }
}

void func_80035D4C(s16);

/* The parameter is s32 -- only the call takes an s16, which is where the
   sll/sra comes from; as an s16 parameter the three comparisons each want the
   sign-extended copy and the function is five words longer. */
u32 func_80035DBC(s32 k) {
    func_80035D4C((s16) k);
    if (k == 1) {
        return D_8006A944;
    }
    if (k == 2) {
        return D_8006A948;
    }
    if (k == 0xA && func_80033B2C(0x1473) == 0) {
        return D_80080A50;
    }
    return D_8006AC5C[k];
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035E58);

void func_80036900(u8 *, u8 *, s32);

/* A (p, q, r) HANDLER, like func_800359FC: the last two parameters are unused
   except as the pass-through to the indirect call, and their allocnos are what
   keep the +0x4C byte off $a1 -- declared `(u8 *p)` alone it lands in $a1
   where the target has $a3, 3 DIFF that no cast, cache or branch shape moves.
   And the +0x44 pointer and the +0x4A index in separate statements, pointer
   first, as that family always wants. */
void func_8003605C(u8 *p, u8 *q, s32 r) {
    u8 *b;
    s16 i;
    u8 *e;
    u8 c;
    u8 *d;

    if (p[0x48] != 0) {
        b = *(u8 **) (p + 0x44);
        i = *(s16 *) (p + 0x4A);
        e = b + i;
        c = e[0x4C];
        if (c == 0xFF) {
            e[0x50] = 1;
            func_80033C84(*(u32 **) p);
            func_80035090((u32 *) (p + 0x54));
            d = p - 0x20;
            func_80044A50((UnkNode *) d);
            func_8003FFF0(d);
            return;
        }
        if (c == 0xFE) {
            p[0x4C] = 0xFF;
            p[0x4D] = 0xFF;
            *(u32 *) (p + 0x68) = (u32) func_80036900;
        }
    }
    (*(void (**)(u8 *, u8 *, s32)) (p + 0x68))(p, q, r);
}

void func_80036154(u8 *);

void func_80036110(u8 *p) {
    *(u16 *) (*(u8 **) p + 0xA) -= 0x140;
    if (*(s16 *) (*(u8 **) p + 0xA) < 0) {
        *(u16 *) (*(u8 **) p + 0xA) = 0;
        *(u32 *) (p + 0x68) = (u32) func_80036154;
    }
}

void func_80036210(u8 *);
void func_800362D0(u8 *);
void func_800364EC(u8 *);
void func_8003666C(u8 *);
void func_800368D0(u8 *);
void func_80038478(u8 *);
void func_80038588(u8 *);
void func_800385E8(u8 *);
void func_80038648(u8 *);
void func_80038A00(void);
void func_80036988(u8 *, u8 *);

void func_80036154(u8 *p) {
    u32 h = *(u32 *) (*(u8 **) (p + 0x74) + 0x10);

    if (h == (u32) func_80038478) {
        *(u32 *) (p + 0x68) = (u32) func_80036210;
    } else if (h == (u32) func_80038588) {
        *(u32 *) (p + 0x68) = (u32) func_800362D0;
    } else if (h == (u32) func_800385E8) {
        *(u32 *) (p + 0x68) = (u32) func_800364EC;
    } else if (h == (u32) func_80038648) {
        *(u32 *) (p + 0x68) = (u32) func_8003666C;
    } else if (h == (u32) func_80038A00) {
        *(u32 *) (p + 0x68) = (u32) func_800368D0;
    }
    func_80036988(p + 0xC, *(u8 **) (p + 0x74));
    (*(u8 **) (p + 0x74))[0x85] = 1;
}

void func_80036B10(u8 *, u8 *, u8 *, s32, u32);
void func_8003626C(u8 *);

void func_80036210(u8 *p) {
    func_80036B10(D_80082414, D_80081FDC, p, 0,
                  *(u32 *) (*(u8 **) (p + 0x74) + 0x80));
    p[0x4C] = 0;
    *(u32 *) (p + 0x68) = (u32) func_8003626C;
}

void func_800362A0(u8 *);

void func_8003626C(u8 *p) {
    if ((D_80083170[0] & 0x60) != 0) {
        p[0x4C] = 0xFF;
        p[0x50] = 0;
        *(u32 *) (p + 0x68) = (u32) func_800362A0;
    }
}

void func_80036154(u8 *);

void func_800362A0(u8 *p) {
    if (p[0x50] != 0) {
        (*(u8 **) (p + 0x74))[0x84] = 1;
        *(u32 *) (p + 0x68) = (u32) func_80036154;
    }
}

void func_80036ED4(u8 *, s32, s32, s16, s16, u32);
void func_80036350(u8 *, u8 *, s32);

void func_800362D0(u8 *p) {
    u8 *s = *(u8 **) (p + 0x74);
    s16 t = *(u16 *) (s + 0x22) - (s[0x26] - 1);

    p[0x4D] = s[0x27];
    func_80036ED4(D_80082538, (s32) D_80082040, (s32) p, 1, t, *(u32 *) (*(u8 **) (p + 0x74) + 0x80));
    *(u32 *) (p + 0x68) = (u32) func_80036350;
}

void func_80036434(u8 *, u8 *, s32);

void func_80036350(u8 *p, u8 *q, s32 r)
{
    u8 *b = (u8 *)&D_80083160;
    u32 f = *(u32 *)(b + 0x10);

    if (f & 0x4000)
    {
        func_80053DA8(0x502);
        p[0x4D] = p[0x4D] + 1;
        p[0x4D] = p[0x4D] % *(u8 *)(*(u8 **)(p + 0x74) + 0x26);
    }
    else if (f & 0x1000)
    {
        func_80053DA8(0x502);
        p[0x4D] = p[0x4D] + *(u8 *)(*(u8 **)(p + 0x74) + 0x26) - 1;
        p[0x4D] = p[0x4D] % *(u8 *)(*(u8 **)(p + 0x74) + 0x26);
    }
    if (*(u32 *)(b + 0x10) & 0x40)
    {
        func_80036434(p, q, r);
    }
}

void func_80036484(u8 *);

void func_80036434(u8 *p, u8 *q, s32 r) {
    u8 *q = *(u8 **) (p + 0x74);

    q[0x27] = p[0x4D];
    *(u16 *) (p + 0x64) = 0x11;
    *(u32 *) (p + 0x68) = (u32) func_80036484;
    p[0x4D] |= 0x80;
    func_80053DA8(0x503);
}

void func_800364BC(u8 *);

void func_80036484(u8 *p) {
    u16 *c = (u16 *) (p + 0x64);

    *c = *c - 1;
    if ((s16) *c <= 0) {
        p[0x4D] = 0xFF;
        p[0x51] = 0;
        *(u32 *) (p + 0x68) = (u32) func_800364BC;
    }
}

void func_80036154(u8 *);

void func_800364BC(u8 *p) {
    if (p[0x51] != 0) {
        (*(u8 **) (p + 0x74))[0x84] = 1;
        *(u32 *) (p + 0x68) = (u32) func_80036154;
    }
}

void func_80037030(u8 *, s32, s32, s16, s16, u32);
void func_80036574(u8 *);

void func_800364EC(u8 *p) {
    u8 *s = *(u8 **) (p + 0x74);
    s16 t = *(u16 *) (s + 0x22) - (s[0x26] - 1);

    p[0x4D] = s[0x27];
    func_80037030(D_80082538, (s32) D_80082040, (s32) p, 1, t,
                  *(u32 *) (*(u8 **) (p + 0x74) + 0x80));
    *(u16 *) (p + 0x64) = 0x5A;
    *(u32 *) (p + 0x68) = (u32) func_80036574;
}

void func_8003661C(u8 *);
void func_800364BC(u8 *);

/* The expired arm is written FIRST, so the countdown test branches INTO the
   still-running block as the target's `bgtz` does (41 DIFF the other way
   round), and D_80083160 goes through a one-set POINTER LOCAL -- the struct
   field read directly is the two-insn symbolic form where the target
   materialises the base and loads `0x10($v1)`. */
void func_80036574(u8 *p) {
    Unk80083160 *g = &D_80083160;
    s16 n;
    u32 f;
    u8 *e;

    n = *(u16 *) (p + 0x64) - 1;
    *(s16 *) (p + 0x64) = n;
    if (n <= 0) {
        e = *(u8 **) (p + 0x74);
        p[0x4D] = 0xFF;
        p[0x51] = 0;
        e[0x27] = e[0x26];
        *(u32 *) (p + 0x68) = (u32) func_800364BC;
    } else {
        f = g->unk_10;
        if ((f & 0x20) != 0) {
            p[0x4D] = *(u8 *) (*(u8 **) (p + 0x74) + 0x26) - 1;
        } else if ((f & 0x40) != 0) {
            p[0x4D] = 0;
        } else {
            return;
        }
        func_8003661C(p);
    }
}

void func_80036484(u8 *);

void func_8003661C(u8 *p) {
    u8 *q = *(u8 **) (p + 0x74);

    q[0x27] = p[0x4D];
    *(u16 *) (p + 0x64) = 0x11;
    *(u32 *) (p + 0x68) = (u32) func_80036484;
    p[0x4D] |= 0x80;
    func_80053DA8(0x503);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003666C);

void func_80036880(u8 *, s32, s32);

/* The first test's two bits are written through TWO DIFFERENT SPELLINGS of the
   same lvalue: `*(u32 *) (g + 0x10)` for 0x2000 and `D_80083160.unk_10` for
   0x8000. With both written the same way `fold` merges
   `(x & 0x2000) || (x & 0x8000)` into one `andi 0xA000` and the function is
   three words short; with two spellings fold cannot see one expression, so the
   two `andi`s and the two branches survive -- and cse still unifies the loads,
   which is why the listing has only one `lw 0x10($s2)` feeding both.

   The flag word is re-read before the 0x4000 test and again before the 0x40
   test, and that comes for free: both labels have more than one predecessor,
   so cse's extended basic block ends there. Reading it into a local would give
   one load and lose the two re-reads. */
void func_800366F4(u8 *p, s32 b, s32 c)
{
    u8 *g = (u8 *)&D_80083160;
    s32 s;
    s32 n;

    s = p[0x4D];
    if ((*(u32 *)(g + 0x10) & 0x2000) != 0 || (D_80083160.unk_10 & 0x8000) != 0)
    {
        if ((s & 1) != 0)
        {
            s = s - 1;
        }
        else if (s + 1 < (*(u8 **)(p + 0x74))[0x26])
        {
            s = s + 1;
        }
    }
    if ((*(u32 *)(g + 0x10) & 0x4000) != 0)
    {
        if (s + 2 < (*(u8 **)(p + 0x74))[0x26])
        {
            s = s + 2;
        }
        else
        {
            s = s % 2;
        }
    }
    else if ((*(u32 *)(g + 0x10) & 0x1000) != 0)
    {
        if (s - 2 >= 0)
        {
            s = s - 2;
        }
        else
        {
            n = (*(u8 **)(p + 0x74))[0x26];
            if ((n & 1) == 0)
            {
                s = s % 2 + n - 2;
            }
            else if ((s & 1) != 0)
            {
                s = n - 2;
            }
            else
            {
                s = n - 1;
            }
        }
    }
    if (p[0x4D] != s)
    {
        func_80053DA8(0x502);
        p[0x4D] = s;
    }
    if ((*(u32 *)(g + 0x10) & 0x40) != 0)
    {
        func_80036880(p, b, c);
    }
}

void func_80036484(u8 *);

void func_80036880(u8 *p, s32 b, s32 c) {
    u8 *q = *(u8 **) (p + 0x74);

    q[0x27] = p[0x4D];
    *(u16 *) (p + 0x64) = 0x11;
    *(u32 *) (p + 0x68) = (u32) func_80036484;
    p[0x4D] |= 0x80;
    func_80053DA8(0x503);
}

void func_800368F8(void);

void func_800368D0(u8 *p)
{
    u8 *t;

    t = *(u8 **)(p + 0x44);
    (t + *(s16 *)(p + 0x4A))[0x50] = 0xFF;
    *(u32 *)(p + 0x68) = (u32)func_800368F8;
}

void func_800368F8(void) {
}

void func_80036980(void);

void func_80036900(u8 *p, u8 *q, s32 r)
{
    u8 *t;

    *(u16 *)(*(u8 **)p + 0xA) = *(u16 *)(*(u8 **)p + 0xA) + 0x100;
    if (*(s16 *)(*(u8 **)p + 0xA) >= 0x401)
    {
        func_80044A50((UnkNode *)(p - 0x20));
        *(u32 *)(p + 0x68) = (u32)func_80036980;
        t = *(u8 **)(p + 0x44);
        (t + *(s16 *)(p + 0x4A))[0x50] = 1;
    }
}

void func_80036980(void) {
}

void func_80036988(u8 *d, u8 *s) {
    *(u16 *) (d + 0x8) = *(u16 *) (s + 0x0) + *(u16 *) (s + 0x8);
    *(u16 *) (d + 0xA) = *(u16 *) (s + 0x2) + *(u16 *) (s + 0xA);
    *(u16 *) (d + 0xC) = *(u16 *) (s + 0xC);
    *(u16 *) (d + 0xE) = *(u16 *) (s + 0xE);
}

void func_80067F20(u32 *, s32, s32, s32, s32);

void func_800369C8(s32 n) {
    u8 *b = D_80083160.unk_00;
    u32 *p = *(u32 **) (b + 0x8D0);
    s16 t[4];
    u16 tp;
    s32 o;

    *(u32 **) (b + 0x8D0) = p + 3;
    t[0] = 0;
    t[1] = 0;
    t[2] = 0x40;
    t[3] = 0x20;
    tp = GetTPage(0, 0, 0x1C0, 0x100);
    func_80067F20(p, 0, 0, tp, (s32) t);
    o = n * 4 + 0x70;
    AddPrim(D_80083160.unk_00 + o, p);
}

void func_80067F20(u32 *, s32, s32, s32, s32);

/* The 0x70 binds to the term the source groups it with: as
   `D_80083160.unk_00 + n * 4 + 0x70` it folds onto the loaded base and costs
   4 words, so the whole index lives in its own local. */
void func_80036A70(s32 n) {
    u8 *b = D_80083160.unk_00;
    u32 *p = *(u32 **) (b + 0x8D0);
    s16 t[4];
    u16 tp;
    s32 o;

    *(u32 **) (b + 0x8D0) = p + 3;
    t[0] = 0;
    t[1] = 0;
    t[2] = 0;
    t[3] = 0;
    tp = GetTPage(0, 0, 0x1C0, 0x100);
    func_80067F20(p, 0, 0, tp, (s32) t);
    o = n * 4 + 0x70;
    AddPrim(D_80083160.unk_00 + o, p);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036B10);

s32 func_80037534(u8 *, s32, s32);
void func_800478B8(s32);

void func_80036C44(s32 a, s32 b, s32 c) {
    if (func_80037534((u8 *) a, b, c) == 0) {
        func_800478B8(c);
    }
}

void func_80036C7C(u8 *p, u32 *s, u8 *d) {
    *(u32 *) (d + 0) = s[0];
    *(u32 *) (d + 4) = s[1];
    *(u32 *) (d + 8) = s[2];
    *(s16 *) (d + 4) = GetTPage(0, 1, *(s16 *) (p + 8), *(s16 *) (p + 0xA));
    d[8] = d[8] + (p[0x18] + (s16) (*(s16 *) (p + 8) % 64) * 4);
    d[9] = d[9] + (*(s16 *) (p + 0xA) % 256 + p[0x1A]);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036D4C);

void func_80036D4C(u8 *, s32, s32, s32, s32, u32);
void func_80036F24(u8 *, u32, u32);

void func_80036ED4(u8 *p, s32 b, s32 c, s16 d, s16 e, u32 f) {
    func_80036D4C(p, b, c, d, e, f);
    func_80036F24(p + 0x20, *(u32 *) (p + 0x8), *(u32 *) (p + 0xC));
}

extern u32 D_8006A958[];
extern u32 D_8006A964[];
extern u32 D_8006A970[];
extern u32 D_8006A97C[];
extern u8 D_80082B50[];
extern u8 D_80082B60[];
extern u8 D_80082B70[];

/* The counter at +0x64 is VOLATILE. The target increments it and then LOADS
   IT AGAIN to test bit 0 -- `lhu`, `nop`, `addiu`, `sh`, `lhu`, `nop`,
   `andi 1` -- and nothing about how the increment or the test is spelled stops
   cse forwarding the stored value instead: a `u16 *` local, `(*p)++`, reading
   it back as `s16`, and the increment through a `u16` temporary were all two
   words short, and MEM_IN_STRUCT_P is refuted as the cause (matching-levers
   section 10). A volatile load is never forwarded and never deleted, which is
   exactly the behaviour the listing has, so the field was volatile in the
   original struct. Only the RE-READ needs the qualifier; marking both is
   equivalent.

   The base at +0x44 goes into its own local so its `lw` is emitted before the
   `lh` of +0x4A -- sched's tie-break for equal-priority insns is INSN_LUID,
   i.e. RTL order. The index must NOT also be a local: as a local it is a
   second quantity whose live range is shorter, so local-alloc gives it $v0 and
   the pointer $v1, where the target has them the other way round, and the
   `addu`'s operands come out swapped with them. Left inside the subscript it
   is one operand of the pointer's own address expression. */
void func_80036F24(u8 *p, u32 b, u32 r)
{
    u8 n;
    u8 *t;

    if (func_80037534(p, b, r) != 0)
    {
        return;
    }
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A958, D_80082B50);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A964, D_80082B60);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A970, D_80082B70);
    t = *(u8 **)(p + 0x44);
    n = (t + *(s16 *)(p + 0x4A))[0x4C];
    if ((n & 0x80) != 0)
    {
        *(volatile u16 *)(p + 0x64) = *(u16 *)(p + 0x64) + 1;
        if ((*(volatile u16 *)(p + 0x64) & 1) == 0)
        {
            *(u32 *)(r + 0xC) = 0;
        }
        else
        {
            *(u32 *)(r + 0xC) = 0xC0C0C0;
        }
    }
    else
    {
        *(u32 *)(r + 0xC) = 0xC0C0C0;
    }
    *(u32 *)(r + 0x8) = D_8006A97C[(n & 0x7F) + *(s16 *)(p + 0x66)];
}

void func_80036D4C(u8 *, s32, s32, s32, s32, u32);
void func_80037090(u8 *, u32, u32);

/* func_80036ED4's shape plus the two stores, and both argument loads read into
   locals BEFORE the store through `q`. Left in the call, they are evaluated
   after it, and `sw $zero, 0xC($a2)` has a base sched cannot disambiguate from
   `$s0`, so the `lw $a1, 0x8($s0)` can never move back above it: 7 DIFF of
   pure order. The pointer itself has to be a local too, or the argument load
   and the store's load are two separate `lw`s and the function is a word long
   (14 DIFF). */
void func_80037030(u8 *p, s32 b, s32 c, s16 d, s16 e, u32 f) {
    u32 *q;
    u32 v;

    func_80036D4C(p, b, c, d, e, f);
    q = *(u32 **) (p + 0xC);
    v = *(u32 *) (p + 0x8);
    *(u32 *) (p + 0x10) = (u32) func_80037090;
    q[3] = 0;
    func_80037090(p + 0x20, v, (u32) q);
}

/* func_80036F24 with the two arms of the inner test the other way round: the
   zero store is the shared block both branches reach, so it is the `else` of
   the inner test AND the `else` of the outer one. See func_80036F24 above for
   the volatile counter and the pointer local. */
void func_80037090(u8 *p, u32 b, u32 r)
{
    u8 n;
    u8 *t;

    if (func_80037534(p, b, r) != 0)
    {
        return;
    }
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A958, D_80082B50);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A964, D_80082B60);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A970, D_80082B70);
    t = *(u8 **)(p + 0x44);
    n = (t + *(s16 *)(p + 0x4A))[0x4C];
    if ((n & 0x80) != 0)
    {
        *(volatile u16 *)(p + 0x64) = *(u16 *)(p + 0x64) + 1;
        if ((*(volatile u16 *)(p + 0x64) & 1) != 0)
        {
            *(u32 *)(r + 0xC) = 0xC0C0C0;
        }
        else
        {
            *(u32 *)(r + 0xC) = 0;
        }
    }
    else
    {
        *(u32 *)(r + 0xC) = 0;
    }
    *(u32 *)(r + 0x8) = D_8006A97C[(n & 0x7F) + *(s16 *)(p + 0x66)];
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003719C);

extern u32 D_8006A988[];
extern u32 D_8006A994[];
extern u32 D_8006A9A0[];
extern u32 D_8006A9AC[];
extern u32 D_8006A9B8[];
extern u32 D_8006A9C4[];
extern u32 D_8006A9D0[];
extern u8 D_80082B80[];
extern u8 D_80082B90[];
extern u8 D_80082BA0[];
extern u8 D_80082BB0[];
extern u8 D_80081E90[];
extern u8 D_80081EA0[];

/* func_80036F24's body with six of the copies instead of three. */
void func_80037394(u8 *p, u32 b, u32 r)
{
    u8 n;
    u8 *t;

    if (func_80037534(p, b, r) != 0)
    {
        return;
    }
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A988, D_80082B80);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A994, D_80082B90);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A9A0, D_80082BA0);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A9AC, D_80082BB0);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A9B8, D_80081E90);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A9C4, D_80081EA0);
    t = *(u8 **)(p + 0x44);
    n = (t + *(s16 *)(p + 0x4A))[0x4C];
    if ((n & 0x80) != 0)
    {
        *(volatile u16 *)(p + 0x64) = *(u16 *)(p + 0x64) + 1;
        if ((*(volatile u16 *)(p + 0x64) & 1) == 0)
        {
            *(u32 *)(r + 0xC) = 0;
        }
        else
        {
            *(u32 *)(r + 0xC) = 0xC0C0C0;
        }
    }
    else
    {
        *(u32 *)(r + 0xC) = 0xC0C0C0;
    }
    *(u32 *)(r + 0x8) = D_8006A9D0[(n & 0x7F) + *(s16 *)(p + 0x66)];
}

u32 func_800374F4(u16 n) {
    /* The product must be unsigned -- `(u32)` -- or the high half comes out
       with `sra` where the target has `srl`. */
    return (u32) (n * (u16) (rand() << 1)) >> 16;
}

void func_80033C84(u32 *);
void func_80035090(u32 *);
void func_8003FFF0(u8 *);

/* Two of the three parameters are unused -- func_80036C44 passes all three and
   they cost nothing either way -- and the base goes into its own local: as one expression the `lh 0x4A` is
   emitted before the `lw 0x44` whichever way round the operands are (3 DIFF). */
s32 func_80037534(u8 *p, s32 b2, s32 c2) {
    u8 *b;
    u8 *q;
    u8 *d;

    if (p[0x48] == 0) {
        return 0;
    }
    b = *(u8 **) (p + 0x44);
    q = b + *(s16 *) (p + 0x4A);
    if (q[0x4C] != 0xFF) {
        return 0;
    }
    q[0x50] = 1;
    func_80033C84(*(u32 **) p);
    func_80035090((u32 *) (p + 0x54));
    d = p - 0x20;
    func_80044A50((UnkNode *) d);
    func_8003FFF0(d);
    return 1;
}

void func_800375C0(u8 *p, u8 *q)
{
    *(s32 *)(*(u8 **)p + 0X60) = *(s16 *)(q + 0X0);
    *(s32 *)(*(u8 **)p + 0X10) = *(s16 *)(q + 0X4);
    *(s32 *)(*(u8 **)p + 0X14) = *(s16 *)(q + 0X6);
    *(s32 *)(*(u8 **)p + 0X18) = *(s16 *)(q + 0X8);
    *(s32 *)(*(u8 **)p + 0X30) = *(s16 *)(q + 0XC);
    *(s32 *)(*(u8 **)p + 0X34) = *(s16 *)(q + 0XE);
    *(s32 *)(*(u8 **)p + 0X38) = *(s16 *)(q + 0X10);
    *(u16 *)(*(u8 **)p + 0X8) = *(u16 *)(q + 0X14);
    *(u16 *)(*(u8 **)p + 0XA) = *(u16 *)(q + 0X16);
    *(u16 *)(*(u8 **)p + 0XC) = *(u16 *)(q + 0X18);
    *(u32 *)(p + 0XC) = *(u32 *)(q + 0X1C);
    *(u32 *)(p + 0X10) = *(u32 *)(q + 0X20);
    *(u32 *)(p + 0X14) = *(u32 *)(q + 0X24);
    *(u32 *)(p + 0X18) = *(u32 *)(q + 0X28);
    *(u32 *)(p + 0X1C) = *(u32 *)(q + 0X2C);
    *(u16 *)(p + 0X20) = *(u16 *)(q + 0X30);
    *(u16 *)(p + 0X22) = *(u16 *)(q + 0X32);
    *(u16 *)(p + 0x24) = *(u16 *)(p + 0x26) = 0;
    *(u32 *)(p + 0X28) = *(u32 *)(q + 0X38);
    *(u32 *)(p + 0X2C) = *(u32 *)(q + 0X3C);
    *(u32 *)(p + 0X30) = *(u32 *)(q + 0X40);
    *(u32 *)(p + 0X34) = *(u32 *)(q + 0X44);
    *(u32 *)(p + 0X38) = *(u32 *)(q + 0X48);
    *(u16 *)(p + 0X3C) = *(u16 *)(q + 0X4C);
    *(u16 *)(p + 0X3E) = *(u16 *)(q + 0X4E);
    *(u16 *)(p + 0x40) = *(u16 *)(p + 0x42) = 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037714);

void func_80037C7C(u8 *p, s16 n)
{
    s16 m;
    s16 t;

    *(s32 *)(*(u8 **)p + 0x10) = n;
    if (n >= 0x37)
    {
        *(s16 *)(p + 0xC) = -0x38;
        *(s16 *)(p + 0x10) = 0x30;
    }
    else
    {
        *(s16 *)(p + 0xC) = -n - 2;
        *(s16 *)(p + 0x10) = n - 6;
        if (*(s16 *)(p + 0x10) <= 0)
        {
            *(s16 *)(p + 0x10) = 1;
        }
    }
    if (n < 0x36)
    {
        s16 k = -n;

        *(s16 *)(p + 0x24) = k + 0x36;
    }
    else
    {
        *(s16 *)(p + 0x24) = 0;
    }
    m = -n;
    *(s16 *)(p + 0x28) = m - 3;
    t = n + 6;
    *(s16 *)(p + 0x2C) = t;
    *(s16 *)(p + 0x18) = *(u16 *)(p + 0x10);
    if (t <= 0)
    {
        *(s16 *)(p + 0x2C) = 1;
    }
    if (n < 0x40)
    {
        *(s16 *)(p + 0x40) = m + 0x40;
    }
    else
    {
        *(s16 *)(p + 0x40) = 0;
    }
    *(s16 *)(p + 0x34) = *(u16 *)(p + 0x2C);
}

/* The second parameter is unused here; func_80034EF8 masks a byte into it, so
   it is real. An unused parameter costs no instruction. */
void func_80037D50(u8 *p, u8 b) {
    u8 *q = p + 0x8C;

    q[0x25] = D_8006A908[p[2]];
    *(u16 *) (q + 0xE) = D_8006A8B4[q[0x2A]] * q[0x25];
}

extern u8 D_800809C0[];
extern u8 D_8006A86C[];
void func_8003AB14(u16 *, u16 *);
void func_80037D50(u8 *, u8);
void func_80038A10(u8 *, u8 *, s32);

void func_80037D9C(u8 *p, u8 c) {
    u8 *q = p + 0x8C;
    u8 v;

    *(u32 *) (q + 0x80) = (u32) p;
    func_8003AB14((u16 *) q, (u16 *) D_8006A86C);
    q[0x2A] = 1;
    q[0x24] = 0x20;
    *(s16 *) (q + 0xC) = 0xFC;
    *(s16 *) (q + 8) = 0;
    *(s16 *) (q + 0xA) = 0;
    *(u32 *) (q + 0x1C) = (u32) D_800809C0;
    *(s16 *) (q + 0x22) = 0;
    *(s16 *) (q + 0x20) = 0;
    q[0x2E] = 0;
    v = p[3];
    p[4] = v;
    q[0x28] = v;
    *(u32 *) (q + 0x7C) = (u32) (p + 0xC8);
    *(u32 *) (q + 0x10) = (u32) func_80038A10;
    p[2] = c;
    func_80037D50(p, c);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037E58);

s32 func_80043FB8(s32 arg0, ...);
void func_8003800C(u8 *);

void func_80037F70(u8 **p) {
    u8 *b = (u8 *) D_80082A38;

    func_80043FB8((s32) D_8002D000, (s32) b);
    func_80043FB8((s32) D_8002D010, (s32) (b + 0xA8));
    func_80043FB8((s32) D_8002D020, (s32) (b + 0x9C));
    func_8003800C(*p);
    (*p)[0x86] = (*p)[0x85];
    (*p)[0x85] = 0;
}

void func_80038000(u8 v) {
    D_80082B4B[0] = v;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003800C);

void func_80038A10(u8 *, u8 *, s32);

/* The struct through a pointer local declared at function scope, so its
   lui/addiu pair lands in the entry block (the `addiu` ends up in the first
   guard's delay slot) and the field load keeps a plain +8 displacement.
   Written as `D_80083160.unk_08` the pair is emitted inside the second block
   and cse folds the %lo into the load, which is a word short. */
void func_80038128(u8 *p) {
    Unk80083160 *s = &D_80083160;

    if (*(s8 *) (p + 0x87) == 0 && (s->unk_08 & 0x20) != 0) {
        if (p[0x2E] == 0) {
            p[0x2E] = 1;
        }
        *(u16 *) (p + 0x18) = 0;
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    } else {
        u16 *c = (u16 *) (p + 0x18);

        *c = *c - 1;
        if ((s16) *c <= 0) {
            *(u32 *) (p + 0x10) = (u32) func_80038A10;
        }
    }
}

void func_80038A10(u8 *, u8 *, s32);

void func_800381A4(u8 *p) {
    u16 *c = (u16 *) (p + 0x18);

    *c = *c - 1;
    if ((s16) *c <= 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

s32 func_80038240(s32);
void func_80038288(u8 *);
void func_800382F0(u8 *);
void func_8003832C(u8 *);

u32 func_800381D0(u8 *p) {
    /* The func_80038288 arm is written LAST because the target places it last,
       reached by `beq`; as an early `if (... == 0x100) return ...;` it comes
       out first and everything after it shifts (18 DIFF). */
    if (func_80053EF0(4) != 0x100) {
        if (func_80038240((*(u8 **) (p + 0x1C))[0]) != 0) {
            return (u32) func_800382F0;
        }
        return (u32) func_8003832C;
    }
    return (u32) func_80038288;
}

s32 func_80038240(s32 a) {
    if (a == 0) {
        return 0;
    }
    if (a == 1) {
        func_80053DA8(0xB4);
        return 0;
    }
    func_80053DA8(0x74);
    return 1;
}

s32 func_80038240(s32);
void func_800382F0(u8 *);
void func_8003832C(u8 *);

void func_80038288(u8 *p) {
    if (func_80053EF0(4) != 0x100) {
        if (func_80038240((*(u8 **) (p + 0x1C))[0]) != 0) {
            *(u32 *) (p + 0x10) = (u32) func_800382F0;
        } else {
            *(u32 *) (p + 0x10) = (u32) func_8003832C;
        }
    }
}

void func_8003832C(u8 *);

void func_800382F0(u8 *p) {
    if (func_80053EF0(4) == 0) {
        func_8003832C(p);
    }
}

void func_80039AF0(u8 *);
void func_80038368(u8 *);

void func_8003832C(u8 *p) {
    func_80039AF0(p);
    *(u16 *) (p + 0x18) = 5;
    *(u32 *) (p + 0x10) = (u32) func_80038368;
}

void func_80038A10(u8 *, u8 *, s32);

void func_80038368(u8 *p) {
    u16 *c = (u16 *) (p + 0x18);

    *c = *c - 1;
    if ((s16) *c <= 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_80038A10(u8 *, u8 *, s32);

void func_80038394(u8 *p) {
    if (func_80033B2C(*(s16 *) (p + 0x30)) != 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_80038A10(u8 *, u8 *, s32);

void func_800383D4(u8 *p) {
    if (D_80082660[p[0x2F]].unk_01 == 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

u8 *func_800392A4(u8);
void func_80038A10(u8 *, u8 *, s32);

void func_80038408(u8 *p) {
    if (func_800392A4(p[0x2F]) != 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    } else {
        p[0x89] -= 1;
        if ((s8) p[0x89] < 0) {
            *(u32 *) (p + 0x10) = (u32) func_80038A10;
        }
    }
}

void func_80038464(u8 *p) {
    p[0x84] = 0;
    *(u32 *) (p + 0x10) = (u32) func_80038478;
}

void func_800384A8(u8 *);

void func_80038478(u8 *p) {
    if (*(s8 *) (p + 0x84) != 0) {
        func_800384A8(p);
    }
}

void func_800384F8(u8 *);
void func_80038538(u8 *);

void func_800384A8(u8 *p) {
    if (func_80053EF0(4) != 0x100) {
        func_80038538(p);
    } else {
        *(u32 *) (p + 0x10) = (u32) func_800384F8;
    }
}

void func_80038538(u8 *);

void func_800384F8(u8 *p) {
    if (func_80053EF0(4) != 0x100) {
        func_80038538(p);
    }
}

void func_80038A10(u8 *, u8 *, s32);

void func_80038538(u8 *p) {
    func_80053DA8(0xB4);
    *(u32 *) (p + 0x10) = (u32) func_80038A10;
}

void func_80038570(u8 *p) {
    p[0x27] = 0;
    p[0x84] = 0;
    *(u32 *) (p + 0x10) = (u32) func_80038588;
}

void func_80038A10(u8 *, u8 *, s32);

void func_80038588(u8 *p) {
    if (*(s8 *) (p + 0x84) != 0) {
        func_80053DA8(0xB4);
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_800385D0(u8 *p) {
    p[0x27] = 0;
    p[0x84] = 0;
    *(u32 *) (p + 0x10) = (u32) func_800385E8;
}

void func_80038A10(u8 *, u8 *, s32);

void func_800385E8(u8 *p) {
    if (*(s8 *) (p + 0x84) != 0) {
        func_80053DA8(0xB4);
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_80038630(u8 *p) {
    p[0x27] = 0;
    p[0x84] = 0;
    *(u32 *) (p + 0x10) = (u32) func_80038648;
}

void func_80038A10(u8 *, u8 *, s32);

void func_80038648(u8 *p) {
    if (*(s8 *) (p + 0x84) != 0) {
        func_80053DA8(0xB4);
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_80038A10(u8 *, u8 *, s32);

void func_80038690(u8 *p) {
    u8 *s = *(u8 **) (p + 0x80);
    u32 r = (*(u32 (**)(u32, u32, u32, u32)) (*(u8 **) (s + 0x40) + *(s8 *) (p + 0x32) * 4))(*(u32 *) (s + 0x48), *(u32 *) (s + 0x4C), *(u32 *) (s + 0x50), *(u32 *) (s + 0x54));

    *(u32 *) (s + 0x84) = r;
    if (r != 0) {
        *(u32 *) (p + 0x34) = r;
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_80038A10(u8 *, u8 *, s32);

void func_80038708(u8 *p) {
    ((void (*)(void)) D_800DCF54[*(s8 *) (p + 0x32)])();
    *(u32 *) (p + 0x10) = (u32) func_80038A10;
}

/* D_80110E98 is an ARRAY of pointers, not a pointer to one: the target takes
   its address (`addiu %lo`) where `extern u32 **` loads through it. */
extern u32 *D_80110E98[];
void func_80038A10(u8 *, u8 *, s32);

void func_8003875C(u8 *p) {
    u32 r = (*(u32 (**)(u32)) (D_80110E98[*(s8 *) (p + 0x32)] + *(s8 *) (p + 0x33)))(*(u32 *) (p + 0x34));

    if (r != 0) {
        *(u32 *) (p + 0x34) = r;
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800387D0);

void func_80038A10(u8 *, u8 *, s32);

void func_800389B4(u8 *p) {
    if (func_80053EF0(4) != 0x100) {
        func_80053DA8(0xB4);
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_80038A00(void) {
}

void func_80038A08(void) {
}

extern u32 D_80081448;
s32 func_80038AB8(u8 *, u8 *);

/* $gp group (D_80081448), so this was verified in a scratch file carrying the
   .sdata block at the top of this file. The three guards are `&&`-joined, so
   each branches past the body as the target does, and reorg fills all three
   delay slots with the `lui %hi(func_80038A10)` the loop then compares
   against. */
void func_80038A10(u8 *p, u8 *q, s32 r) {
    Unk80083160 *g = &D_80083160;
    u8 *s;

    D_80081448 = (u32) p;
    s = *(u8 **) (p + 0x80);
    if (*(s8 *) (p + 0x87) == 0 && (g->unk_08 & 0x20) != 0 && p[0x2E] == 0) {
        p[0x2E] = 1;
    }
    do {
        if (func_80038AB8(p, s) == 0) {
            break;
        }
    } while (*(u32 *) (p + 0x10) == (u32) func_80038A10);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038AB8);

typedef struct Unk8002D03C {
    s8 unk_00[9];
} Unk8002D03C;

/* The 9-byte table is COPIED to the stack -- a byte-aligned struct, so the
   copy is lwl/lwr pairs plus an `lb` -- and the copy comes before the
   counter's initialisation (as a `for`, not `i = 0` ahead of the copy, which
   puts `move $t0, $zero` first and spends a register: 22 DIFF). The `for`
   guard folds away because 0 < 9. */
s32 func_80038C40(s8 k) {
    Unk8002D03C t;
    s32 i;

    t = *(Unk8002D03C *) D_8002D03C;
    for (i = 0; i < 9; i++) {
        if (k == t.unk_00[i]) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038CB8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038F48);

extern u8 D_80082E6Aa[] __asm__("D_80082E6A");
void func_80038A00(void);
s16 func_80053DA8(u16);

/* `!= 0x100` with the early return in the ELSE: the branch is TAKEN to the
   returning arm, which is therefore placed after the other one. Written
   `== 0x100` with the return in the then-arm, the two blocks swap and the
   whole tail follows them (19 DIFF). */
void func_800390D0(u8 *p) {
    if (D_80082E6Aa[0] != 2) {
        if (func_80053EF0(4) != 0x100) {
            func_80053DA8(0xC4);
        } else {
            *(u32 *) (p + 0x10) = 0;
            *(u32 *) (p + 0x1C) -= 1;
            return;
        }
    }
    *(u32 *) (p + 0x10) = (u32) func_80038A00;
}

void func_800387D0(void);
void func_800381A4(u8 *);

void func_80039148(u8 *p) {
    *(u16 *) (p + 0x20) = 0;
    if (*(s16 *) (p + 0x22) < p[0x25] - 1) {
        *(u16 *) (p + 0x22) += 1;
        if (p[0x2E] == 1) {
            *(u16 *) (p + 0x18) = 0;
            *(u32 *) (p + 0x10) = (u32) func_800381A4;
        }
    } else {
        *(u32 *) (p + 0x10) = (u32) func_800387D0;
    }
}

void func_8003919C(void) {
}

void func_800391A4(void) {
}

void func_800391AC(void) {
}

void func_800391B4(void) {
}

void func_800391BC(u8 *p) {
    *(u16 *) (p + 0x20) = 0;
    *(u16 *) (p + 0x22) = 0;
    ClearImage((RECT *)p, 0, 0, 0);
}

void func_800391EC(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    p[0x28] = *(*q)++;
}

void func_80039208(u8 *p) {
    p[0x2E] = 2;
}

void func_80039214(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *c = *q;
    u16 v = c[0] + (c[1] << 8);

    *q = c + 2;
    func_80033AA8((s16) v);
}

void func_80039258(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *c = *q;
    u16 v = c[0] + (c[1] << 8);

    *q = c + 2;
    func_80033AE8((s16) v);
}

void func_8003929C(void) {
}

u8 *func_800392A4(u8 i) {
    u8 *q = (u8 *) D_80082660[i].unk_04;
    u8 *r;

    if (q != 0) {
        r = q + 0x20;
        if (i == 1) {
            return r;
        }
        if (*(u32 *) (q + 0x10) == (u32) D_800C3174 || *(u32 *) (q + 0x10) == (u32) D_800C321C) {
            if (*(u32 *) (r + 0x50) == (u32) D_800C3960) {
                return r;
            }
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003931C);

s32 func_80039470(s32 a, s32 b, s32 c, s32 d)
{
    Unk80082660 *t = D_80082660;
    s32 k = d & 0xFF;
    u8 *e = (u8 *)&t[k];

    if (*(s8 *)(e + 1) == 0)
    {
        u8 *r;
        u8 *f;

        if (k == 0)
        {
            return 1;
        }
        r = func_800392A4(k);
        if (r == 0)
        {
            return 1;
        }
        e[0] = d >> 8;
        e[1] = 1;
        e[2] = a;
        f = r - 0x20;
        if (k == 1 && *(u32 **)(f + 0x10) != D_800C3174 && *(u32 **)(f + 0x10) != D_800C321C)
        {
            *(s16 *)(r + 0x36) = b;
            *(s16 *)(r + 0x38) = c;
        }
        else
        {
            *(s16 *)(r + 0x88) = b;
            *(s16 *)(r + 0x8A) = c;
        }
        return 1;
    }
    return 0;
}

void func_80039560(void) {
}

void func_80039568(u32 *p) {
    p[4] = (u32) func_80038464;
}

void func_80038128(u8 *);

void func_80039578(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    *(u16 *) (p + 0x18) = (*q)[0] + ((*q)[1] << 8);
    *q += 2;
    *(u32 *) (p + 0x10) = (u32) func_80038128;
}

void func_80038394(u8 *);

void func_800395B0(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    *(u16 *) (p + 0x30) = (*q)[0] + ((*q)[1] << 8);
    *q += 2;
    *(u32 *) (p + 0x10) = (u32) func_80038394;
}

void func_800395E8(void) {
}

void func_800395F0(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 **s = (u32 **) (p + 0x7C);
    u8 *c;

    **s = (u32) (*q + 4);
    *s = *s + 1;
    c = *q;
    *(u32 *) (p + 0x1C) = c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24);
}

void func_80039640(u8 *p) {
    u32 **q = (u32 **) (p + 0x7C);

    *(u32 *) (p + 0x1C) = *--(*q);
}

void func_8003965C(u8 *p) {
    u8 *c = *(u8 **) (p + 0x1C);

    *(u32 *) (p + 0x1C) = c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24);
}

/* The 32-bit jump target is composed from the four stream bytes; both arms
   end in the same store to the cursor, which cross-jumping merges. */
s32 func_80033B2C(s32);

void func_80039694(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *c = *q;
    u16 v = c[0] + (c[1] << 8);
    u8 *r;

    *q = c + 2;
    if (func_80033B2C((s16) v) != 0) {
        r = *q;
        *q = (u8 *) (r[0] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24));
    } else {
        *q = *q + 4;
    }
}

void func_80038570(u8 *);

void func_80039730(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    p[0x26] = **q;
    *q += 1;
    p[0x27] = 0;
    *(u32 *) (p + 0x10) = (u32) func_80038570;
}

void func_80039760(u8 *p) {
    u32 i = p[0x27];
    u8 *c = *(u8 **) (p + 0x1C);

    /* The byte offsets are written INSIDE each index, not folded into a
       `c = base + i * 4` pointer: through a derived pointer the four loads
       interleave with the shifts and the registers shift with them (12 DIFF).
       Same rule as the field offsets in docs/matching-levers.md section 6. */
    *(u32 *) (p + 0x1C) = c[i * 4] + (c[i * 4 + 1] << 8) +
                          (c[i * 4 + 2] << 16) + (c[i * 4 + 3] << 24);
}

void func_800397A0(void) {
}

u32 *func_8003C8E4(s32);

void func_800397A8(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *r = (u8 *) func_8003C8E4(*(*q)++);

    if (r != 0) {
        r[2] = **q;
    }
    *q = *q + 1;
}

void func_80039814(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *r = (u8 *) func_8003C8E4(*(*q)++);
    u8 b;

    if (r != 0) {
        b = **q;
        r[2] = b;
        r[3] = b;
    }
    *q = *q + 1;
}

s32 func_80039884(u8 *p) {
    /* Through a pointer local, so the struct's address is materialised
       (lui+addiu) rather than split with %lo in the load's displacement --
       the target spends the addiu and fills the branch's delay slot with it. */
    Unk80083160 *s = &D_80083160;
    s32 r = 0;

    if (*(s8 *) (p + 0x87) != 0) {
        r = 1;
    } else if ((s->unk_08 & 0x60) == 0) {
        r = 1;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800398BC);

void func_800399EC(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);

    t[0x3D] = *(*q)++;
}

s32 func_80039884(u8 *);
void func_800389B4(u8 *);
s16 func_80053DA8(u16);

void func_80039A08(u8 *p) {
    s32 r = func_80053EF0(4);

    if (func_80039884(p) != 0) {
        if (r != 0) {
            *(u32 *) (p + 0x10) = 0;
            *(u32 *) (p + 0x1C) -= 1;
        }
    } else if (r == 0x100) {
        *(u32 *) (p + 0x10) = (u32) func_800389B4;
    } else {
        func_80053DA8(0xB4);
    }
}

void func_80039A88(u8 *p) {
    if (func_80053EF0(4) != 0) {
        *(u32 *) (p + 0x10) = 0;
        *(s32 *) (p + 0x1C) -= 1;
    }
}

void func_80039ACC(u8 *p) {
    *(u32 *) (p + 0x10) = (u32) func_80039AE8;
    *(s32 *) (p + 0x1C) -= 1;
}

void func_80039AE8(void) {
}

void func_80039AF0(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 **r = (u8 **) (p + 0x80);
    u8 *t;

    (*r)[1] = *(*q)++;
    t = *r;
    t[0x3C] = D_8007216C[t[1]];
}

void func_80039B28(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *c = *q;
    u16 v = c[0] | (c[1] << 8);

    *q = c + 2;
    func_80053DA8(v);
}

void func_80039B64(void) {
}

void func_80039B6C(void) {
}

void func_80039B74(void) {
}

void func_80039B7C(void) {
}

void func_80039B84(u32 *p) {
    p[4] = (u32) func_80038A08;
}

void func_800383D4(u8 *);

void func_80039B94(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 b = *(*q)++;

    *(u32 *) (p + 0x10) = (u32) func_800383D4;
    p[0x2F] = b;
}

void func_80038690(u8 *);

void func_80039BBC(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 b = *(*q)++;

    *(u32 *) (p + 0x10) = (u32) func_80038690;
    p[0x32] = b;
}

void func_80038708(u8 *);

void func_80039BE4(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 b = *(*q)++;

    *(u32 *) (p + 0x10) = (u32) func_80038708;
    p[0x32] = b;
}

void func_8003875C(u8 *);

void func_80039C0C(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    p[0x32] = *(*q)++;
    p[0x33] = *(*q)++;
    *(u32 *) (p + 0x10) = (u32) func_8003875C;
}

void func_800385D0(u8 *);

void func_80039C44(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    p[0x26] = **q;
    *q += 1;
    p[0x27] = 0;
    *(u32 *) (p + 0x10) = (u32) func_800385D0;
}

s32 func_80033B2C(s32);

void func_80039C74(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *c = *q;
    u16 v = c[0] + (c[1] << 8);
    u8 *r;

    *q = c + 2;
    if (func_80033B2C((s16) v) == 0) {
        r = *q;
        *q = (u8 *) (r[0] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24));
    } else {
        *q = *q + 4;
    }
}

void func_80038630(u8 *);

void func_80039D10(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    p[0x26] = **q;
    *q += 1;
    p[0x27] = 0;
    *(u32 *) (p + 0x10) = (u32) func_80038630;
}

void func_80039D40(void) {
}

/* A bytecode opcode: the four bytes at the cursor are a little-endian function
   address, called with four words out of the block at +0x80 and its result
   stored back there. */
void func_80039D48(u8 *p) {
    u8 *c = *(u8 **) (p + 0x1C);
    u8 *s = *(u8 **) (p + 0x80);
    u32 f = c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24);

    *(u8 **) (p + 0x1C) = c + 4;
    *(u32 *) (s + 0x84) = ((u32 (*)(u32, u32, u32, u32)) f)(*(u32 *) (s + 0x48), *(u32 *) (s + 0x4C), *(u32 *) (s + 0x50), *(u32 *) (s + 0x54));
}

void func_80039DBC(u8 *p) {
    u8 *c = *(u8 **) (p + 0x1C);
    u8 *s = *(u8 **) (p + 0x80);
    u32 n = *c;
    u32 *t = *(u32 **) (s + 0x40);

    *(u8 **) (p + 0x1C) = c + 1;
    *(u32 *) (s + 0x84) = ((u32 (*)(u32, u32, u32, u32)) t[n])(
        *(u32 *) (s + 0x48), *(u32 *) (s + 0x4C), *(u32 *) (s + 0x50),
        *(u32 *) (s + 0x54));
}

/* `*cp += 4` comes AFTER the second byte chain. The store is to `p + 0x1C`
   and the chain loads through an arbitrary pointer, so gcc cannot disambiguate
   them: written before the chain the store pins all four loads behind it and
   the function is a word long (42 DIFF). */
void func_80039E1C(u8 *p) {
    u8 **cp = (u8 **) (p + 0x1C);
    u8 *s = *(u8 **) (p + 0x80);
    u32 a;
    u32 w;
    u8 *r;
    u8 *q;
    u8 *t;

    a = *(*cp)++;
    r = *cp;
    q = (u8 *) (r[0] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24));
    w = q[0] + (q[1] << 8) + (q[2] << 16) + (q[3] << 24);
    *cp += 4;
    t = (u8 *) (a * 4 + w);
    *(u32 *) (s + 0x84) = (*(u32 (**)(u32, u32, u32, u32)) t)(
        *(u32 *) (s + 0x48), *(u32 *) (s + 0x4C),
        *(u32 *) (s + 0x50), *(u32 *) (s + 0x54));
}

void func_80039ED0(u32 *p) {
    p[4] = 0;
}

void func_80039ED8(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    p[0x88] = *(*q)++;
}

void func_80039EF4(u8 *p) {
    p[0x88] -= 1;
    if ((s8) p[0x88] >= 0) {
        *(u32 *) (p + 0x10) = 0;
        *(s32 *) (p + 0x1C) -= 1;
    }
}

void func_80039F28(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + *(*q)++ * 4 + 0x48) = 0;
}

void func_80039F4C(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 v = *(*q)++;
    s32 i;

    for (i = 0xF; i >= 0; i--) {
        *(u32 *) (t + i * 4 + 0x48) = v;
    }
}

void func_80039F80(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + a * 4 + 0x48) = *(u32 *) (t + b * 4 + 0x48);
}

void func_80039FC0(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;
    u8 *c = *q;

    *(u32 *) (t + i * 4 + 0x48) = c[0] + (c[1] << 8) + (c[2] << 16) +
                                  (c[3] << 24);
    *q += 4;
}

void func_8003A01C(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u32 **tab = *(u32 ***) (t + 0x44);

    *(u32 *) (t + a * 4 + 0x48) = *tab[b];
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A060);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A10C);

/* The func_8003A2E0 family. The first chain is one expression with the
   `* 4` of the table value at the END; the SECOND chain has to be written as
   an explicit accumulation (`w = q[0]; w += q[1] << 8; ...`) or the two
   low bytes swap registers and the accumulator takes $v0 where the target has
   $v1 (9 DIFF as one expression, in any term order). */
void func_8003A1A4(u8 *p) {
    u8 **cp = (u8 **) (p + 0x1C);
    u32 a;
    u32 m;
    u32 w;
    u8 *s;
    u8 *r;
    u8 *q;
    u8 *d;
    u8 *t;

    a = *(*cp)++;
    s = *(u8 **) (p + 0x80);
    m = *(u32 *) (s + **cp * 4 + 0x48) * 4;
    (*cp)++;
    r = *cp;
    q = (u8 *) (r[0] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24) + m);
    *cp += 4;
    w = q[0];
    w += q[1] << 8;
    w += q[2] << 16;
    w += q[3] << 24;
    d = (u8 *) w;
    t = (u8 *) (a * 4 + (u32) s);
    *(u32 *) (t + 0x48) = *(u32 *) d;
}

void func_8003A248(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + b * 4 + 0x48) = *(u32 *) (t + a * 4 + 0x48);
}

void func_8003A288(u32 *p) {
    p[7]++;
}

void func_8003A29C(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u32 **tab = *(u32 ***) (t + 0x44);

    *tab[b] = *(u32 *) (t + a * 4 + 0x48);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A2E0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A374);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A3E8);

void func_8003A468(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + a * 4 + 0x48) += *(u32 *) (t + b * 4 + 0x48);
}

void func_8003A4B4(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + a * 4 + 0x48) -= *(u32 *) (t + b * 4 + 0x48);
}

void func_8003A500(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + 0x84) = *(u32 *) (t + a * 4 + 0x48) -
                          *(u32 *) (t + b * 4 + 0x48);
}

void func_8003A54C(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;
    u8 *c = *q;

    if (*(u32 *) (t + i * 4 + 0x48) == 0) {
        *q = (u8 *) (c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24));
    } else {
        *q = c + 4;
    }
}

void func_8003A5B4(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;
    u8 *c = *q;

    if (*(u32 *) (t + i * 4 + 0x48) != 0) {
        *q = (u8 *) (c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24));
    } else {
        *q = c + 4;
    }
}

void func_8003A61C(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;
    u8 *c = *q;

    /* s32, not u32: the table entry is compared `<= 0`, which is `bgtz`. */
    if (*(s32 *) (t + i * 4 + 0x48) <= 0) {
        *q = (u8 *) (c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24));
    } else {
        *q = c + 4;
    }
}

void func_8003A684(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;
    u8 *c = *q;

    if (*(s32 *) (t + i * 4 + 0x48) < 0) {
        *q = (u8 *) (c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24));
    } else {
        *q = c + 4;
    }
}

void func_8003A6EC(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;
    u8 *c = *q;

    if (*(s32 *) (t + i * 4 + 0x48) >= 0) {
        *q = (u8 *) (c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24));
    } else {
        *q = c + 4;
    }
}

void func_8003A754(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;
    u8 *c = *q;

    if (*(s32 *) (t + i * 4 + 0x48) > 0) {
        *q = (u8 *) (c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24));
    } else {
        *q = c + 4;
    }
}

void func_8003A7BC(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + a * 4 + 0x48) = *(u32 *) (t + b * 4 + 0x48);
}

void func_8003A7FC(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;
    u8 *c = *q;

    *(u32 *) (t + i * 4 + 0x48) = c[0] + (c[1] << 8) + (c[2] << 16) +
                                  (c[3] << 24);
    *q += 4;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A858);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A904);

void func_8003A9B8(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;

    *q = *(u8 **) (t + i * 4 + 0x48);
}

void func_8003A9E4(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + a * 4 + 0x48) *= *(u32 *) (t + b * 4 + 0x48);
}

void func_8003AA34(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(s32 *) (t + a * 4 + 0x48) /= *(s32 *) (t + b * 4 + 0x48);
}

void func_8003AA84(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 **s = (u32 **) (p + 0x7C);
    u32 i = *(*q)++;

    **s = (u32) *q;
    *s = *s + 1;
    *q = *(u8 **) (t + i * 4 + 0x48);
}

u32 func_8003AAC4(void) {
    return D_80083168[0];
}

u32 func_8003AAD4(void) {
    return D_80083170[0];
}

void func_8003AAE4(u16 a, u16 b) {
    D_80082A38[86] = a;
    D_80082A38[87] = b;
}

void func_8003AAF8(void) {
    /* A local pointer, not `D_80082A38[36]` directly: with constant indices on
       the array name the base comes out biased to the first one
       (`addiu %lo(D_80082A38)+0x48`) and a second addiu undoes it, 8 words
       against 7. Through a pointer the symbol is materialised once and all
       four accesses are displacements, as the target has them. */
    s16 *b = D_80082A38;

    *(s32 *) &b[36] = b[86];
    *(s32 *) &b[38] = b[87];
}

void func_8003AB14(u16 *d, u16 *s) {
    d[0] = s[2];
    d[1] = s[3];
    d[2] = s[4];
    d[3] = s[5];
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003AB44);

void func_8003AB44(u8 *, s16, s32, u8, s16, s32, s32);

void func_8003AC1C(s16 x, s16 y, u8 c, u8 *p) {
    s16 i = 0;

    while (*p != 0) {
        func_8003AB44(p, i, 0, c, x, y, 1);
        p += 2;
        i++;
    }
}

u8 *func_8003ACC0(u8 *p, u16 n) {
    /* The PARAMETER is the moving pointer and the saved copy is what gets
       returned, which is the `addu $a2, $a0, $zero` at the top; the other way
       round swaps those two registers through the whole function. */
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

u8 *func_8003ACC0(u8 *, u16);

/* `d` is REUSED for func_8003ACC0's result rather than a second pointer
   local: as its own variable it takes $v1 and the hoisted 0x82 takes $a0,
   where the target has them the other way round (5 DIFF, and no declaration
   order, loop shape or explicit constant local moves it). The prototype has to
   be in scope too -- without it the `u16` conversion of the digit count is
   not emitted. */
u8 *func_8003AD08(u32 n, u8 *out) {
    u8 buf[0x10];
    u8 *d = buf;
    s32 c = 0;

    do {
        c++;
        *d = n % 10;
        n = n / 10;
        d++;
    } while (n != 0);
    d = func_8003ACC0(buf, c);
    do {
        *out++ = 0x82;
        *out++ = *d++ + 0x4F;
        c--;
    } while (c > 0);
    *out = 0;
    return out;
}

/* The array spelling, not the scalar `D_80082E6A`: a 1-byte scalar is small
   data at -G8 and maspsx expands its load into one register, where the target
   has the compiler-split pair across two. */
extern u8 D_80082E6Aa[] __asm__("D_80082E6A");

s16 func_8003ADB4(u16 a) {
    if (D_80082E6Aa[0] != 2) {
        return func_80053DA8(a);
    } else {
        return 0;
    }
}

void func_8003ADF4(s16 x, s16 y, s16 w, s16 h, u32 *data) {
    RECT r;

    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    LoadImage(&r, data);
}

void func_8003AE28(s16 x, s16 y, s16 w, s16 h, s32 a, s32 b) {
    RECT r;

    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    MoveImage(&r, a, b);
}

void func_8003AE60(s16 x, s16 y, s16 w, s16 h, u8 r, u8 g, u8 b) {
    RECT rect;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    ClearImage(&rect, r, g, b);
}

void func_8003AEA8(u32 *p, s32 n) {
    for (; n > 0; n--) {
        *p++ = 0;
    }
}

void func_800A4928(void);

void func_8003AEC8(void) {
    func_800A4928();
}

void func_8003AEE8(void) {
}

void func_8003AEF0(void) {
}

void func_8003AEF8(u8 *p) {
    u8 *next;
    u32 f;

    while (p != 0) {
        next = *(u8 **) (p + 0x14);
        f = *(u32 *) p;
        *(u32 *) (p + 0x14) = 0;
        if (f != 0) {
            ((void (*)(u32, u32, u32, u32)) f)(*(u32 *) (p + 0x4),
                                               *(u32 *) (p + 0x8),
                                               *(u32 *) (p + 0xC),
                                               *(u32 *) (p + 0x10));
        }
        p = next;
    }
}

void func_8003AF58(u8 *p, u8 *q) {
    while (*(u8 **) (p + 0x14) != 0) {
        p = *(u8 **) (p + 0x14);
    }
    *(u8 **) (p + 0x14) = q;
    *(u8 **) (q + 0x14) = 0;
}

void func_8003AF8C(u32 *p) {
    p[5] = 0;
}

void func_8003AF94(s32 i, u32 v) {
    D_80189394 = v;
    D_801B8EB8 = D_8006CD50[i];
    func_801768AC();
    func_800411FC(0);
}

void func_8003AFE0(s32 i, u32 v) {
    /* The table read is the FIRST statement: with the `| 0x40000000` first
       its `lui` comes out ahead of the index chain instead of in the load's
       shadow, 8 DIFF. */
    u32 t = D_8006CD50[i];

    D_80189394 = v | 0x40000000;
    D_801B8EB8 = t;
    func_801768AC();
    func_800411FC(0);
}

u32 func_80048D20(void);
void func_800483AC(s32);

void func_8003B034(void) {
    D_80080A80 = func_80048D20();
    func_800483AC(1);
    func_800483AC(0);
}

s32 func_8003C758(u8 *);

void func_8003B06C(void) {
    func_8003C758(D_80080F40);
    func_8003C758(D_80081010);
    func_8003C758(D_80080F60);
}

extern Unk41284 D_80080EA0;
extern Unk41284 D_80080E28;
extern Unk41284 D_800D1D54;
extern Unk41284 D_800D1E7C;
extern u8 D_80081018;
void func_8008B408(s32);
void func_8008B550(s32);
void func_800903A8(void);

/* D_80081018 is declared as a SCALAR and passed as `&D_80081018`: as an
   incomplete array the decayed pointer comes out as a split lui/addiu pair
   and reorg puts the addiu in func_8003C758's delay slot, a word short. As an
   address-of it is one two-instruction `la`, which reorg cannot use for a
   delay slot -- which is why all five of these calls have an empty one. */
void func_8003B0B4(void) {
    VSync(0);
    VSync(0);
    DrawSync(0);
    func_80053DA8(0x7F);
    func_800542BC();
    VSync(0);
    func_8003AF94(0x16, 0x5DB);
    SetDispMask(1);
    func_80041284(&D_80080EA0);
    func_80041284(&D_80080E28);
    func_80041284(&D_800D1D54);
    func_80041284(&D_800D1E7C);
    func_8003B034();
    func_8003B06C();
    func_8003C758(&D_80081018);
    func_8008B408(1);
    func_8008B550(1);
    func_800903A8();
}

void func_8003AF58(u8 *, u8 *);

void func_8003B188(void) {
    func_8003AF58(D_800717D0, D_8006AE1C);
}

void func_8003B034(void);
void func_8003B06C(void);
void func_800903A8(void);

void func_8003B1B4(void) {
    func_8003B034();
    func_8003B06C();
    func_800903A8();
}

void func_8003AF58(u8 *, u8 *);

void func_8003B1E4(void) {
    func_8003AF58(D_800717D0, D_8006AE34);
}

void func_8003B210(void) {
    func_8003B034();
    func_8003B06C();
}

void func_8003AF58(u8 *, u8 *);

void func_8003B238(void) {
    func_8003AF58(D_800717D0, D_8006AE4C);
}

void func_800C17A0(s32, s32);

void func_8003B264(void) {
    func_8003B034();
    func_8003B06C();
    func_800C17A0(0x2B, 0x200);
}

void func_8003AF58(u8 *, u8 *);

void func_8003B298(void) {
    func_8003AF58(D_800717D0, D_8006AE64);
}

void func_800C178C(s32, s32);

void func_8003B2C4(void) {
    func_800C178C(0xD, -1);
    func_8003B034();
    func_8003B06C();
}

void func_8003AF58(u8 *, u8 *);

void func_8003B2F8(void) {
    func_8003AF58(D_800717D0, D_8006AE7C);
}

void func_8003AE28(s16, s16, s16, s16, s32, s32);

void func_8003B324(void) {
    func_8003AE28(0x90, 0x1F2, 0x10, 1, 0, 0x1F2);
}

extern u8 D_8006AE94[];
extern Unk41284 D_80080ED0;
extern Unk41284 D_80080ED8;
void func_8003AF58(u8 *, u8 *);
void func_8003AE28(s16, s16, s16, s16, s32, s32);
void func_8004B834(void);
void func_800418B4(void);

void func_8003B35C(void) {
    func_8003AF58(D_800717D0, D_8006AE94);
    func_8003AE28(0, 0x1F2, 0x10, 1, 0x90, 0x1F2);
    func_8003AE28(0x80, 0x1F2, 0x10, 1, 0, 0x1F2);
    func_80041284(&D_80080ED0);
    func_80041284(&D_80080ED8);
    func_8004B834();
    func_800418B4();
}

void func_8003AF58(u8 *, u8 *);

void func_8003B400(void) {
    func_8003AF58(D_800717D0, D_8006AEAC);
}

void func_8003B42C(s32 a) {
    if (a != 0) {
        func_80033AA8(0x3E8);
        func_80033AE8(0xA);
    } else {
        func_80033AE8(0x3E8);
        func_80033AA8(0xA);
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B470);

void func_8003B644(void) {
}

void func_800A0150(s32, u8 *, u8 *, s32);
void func_800B2244(u32);

/* func_8003B820's constant-address recipe: the 0x80010000 half in its own
   pointer local and the list pointer built from it with `| 0x29C`. Unlike
   func_8003B714 -- same walk, one call fewer -- a single `s32 v` local for
   the table byte is right here; the extra func_800B2244 call keeps the loop
   long enough that loop.c leaves the comparison constant inside it. */
void func_8003B64C(void) {
    u32 *e;
    u8 *base;

    if (*(u32 *) 0x8001029C == 0) {
        return;
    }
    base = (u8 *) 0x80010000;
    e = (u32 *) ((u32) base | 0x29C);
    do {
        u8 *a = (u8 *) *e;

        if (a[1] == 0x13) {
            s32 k = (a[3] & 0x1F) * 84;
            s32 v = *(s8 *) (base + 0x333 + k);

            if (v >= 0) {
                func_800A0150(v & 0x3F, a, (u8 *) (0x800102F0 + k), 0);
                func_800B2244(*e);
                continue;
            }
        }
        e++;
    } while (*e != 0);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B714);

s32 func_800B28A0(void);

/* Indexed by the counter off the constant base, which is what puts the giv's
   initialisation in the loop's PREHEADER -- after the `blez` -- and leaves
   0xAC4 as the store's displacement and 0x54 as the increment. A pointer
   walked with `p += 0x54` hoists its `lui` above the table lookup instead
   (9 DIFF); see the unbiased-giv note in docs/matching-levers.md. */
void func_8003B7C8(void) {
    u8 *b = (u8 *) 0x80010000;
    s32 n = D_800D0728[func_800B28A0()];
    s32 i;

    for (i = 0; i < n; i++) {
        *(b + i * 0x54 + 0xAC4) = 0;
    }
}

void func_800B2244(u32);

/* Two constant-address levers here. The 0x80010000 half goes into its OWN
   pointer local and the list pointer is built from it with `| 0x29C`: that is
   what gives the target's `lui $s1` / `addu $s0, $s1, $zero` / `ori` -- one
   hoisted %hi pseudo serving both constants -- where `(u32 *) 0x8001029C`
   written whole is a separate `li` and the table base a second one (41 DIFF).
   And the comparison constant goes into a local assigned BEFORE the base, so
   loop.c hoists it first and it takes $s2 while the base takes $s1; the
   literal 0x13 is hoisted second and the two registers swap (6 DIFF). */
void func_8003B820(void) {
    u32 *e;
    u8 *base;
    s32 key;

    if (*(u32 *) 0x8001029C == 0) {
        return;
    }
    key = 0x13;
    base = (u8 *) 0x80010000;
    e = (u32 *) ((u32) base | 0x29C);
    do {
        u8 *a = (u8 *) *e;

        if (a[1] == key &&
            *(s8 *) (base + 0x333 + (a[3] & 0x1F) * 84) < 0) {
            func_800B2244((u32) a);
        } else {
            e++;
        }
    } while (*e != 0);
}

void func_800B2244(u32);

void func_8003B8DC(void) {
    while (*(u32 *) 0x8001029C != 0) {
        func_800B2244(*(u32 *) 0x8001029C);
    }
}

void func_80043458(void);
void func_8003B820(void);
void func_8003B714(void);

void func_8003B92C(void) {
    u8 *b;

    func_80043458();
    func_8003B820();
    func_8003B714();
    /* The constant base is assigned AFTER the three calls, which is where the
       target materialises it; as an initialiser its `lui` is hoisted above
       them. */
    b = (u8 *) 0x80010000;
    *(u32 *) (b + 0x2D5C) += *(u32 *) (b + 0x2D58);
    if (*(u32 *) (b + 0x2D5C) > 0x3B9ACA00) {
        *(u32 *) (b + 0x2D5C) = 0x3B9ACA00;
    }
}

void func_8003B64C(void);
void func_8003B8DC(void);

void func_8003B988(void) {
    func_80043458();
    func_8003B64C();
    func_8003B8DC();
}

void func_8003B8DC(void);
void func_8003B7C8(void);

void func_8003B9B8(void) {
    func_80043458();
    func_8003B8DC();
    func_8003B7C8();
}

s32 func_800B2834(void);
void func_8003BAF8(u32);

void func_8003B9E8(void) {
    func_8003BAF8(D_8006AFDC[func_800B2834()]);
}

void func_8003BA24(void) {
    func_8003BAF8(D_8006AFE8[func_800B2834()]);
}

void func_8003BA60(void) {
    func_8003BAF8(D_8006AFF4[func_800B2834()]);
}

void func_8003BA9C(void) {
    func_8003BAF8(D_8006B000[func_800B2834()]);
}

void func_8003B9E8(void);

void func_8003BAD8(void) {
    func_8003B9E8();
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003BAF8);

typedef struct Unk8006ADBC {
    u32 unk_00[12];
} Unk8006ADBC;

/* A 0x30-byte struct assignment: cc1psx's block move does four words per
   iteration, which is the loop in the listing -- nothing in the source is
   unrolled. */
void func_8003BD34(void) {
    *(Unk8006ADBC *) D_8006ADBC = *(Unk8006ADBC *) D_8006ADEC;
}

void func_8003BD7C(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003BD84);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003BE1C);

void func_8003BE1C(u8 *, s32, s32);

/* Three halfwords per entry, six entries, through two walked pointers that
   loop.c splits into four induction variables (+0 and +4 on each side). The
   source base is `base + 0x20` off its OWN local, not `&D_80083160 + 0x20`:
   written as one address expression the 0x20 folds into the `addiu %lo` and
   the function is a word short. `d += 8` before `s += 8` is what gives the two
   secondary givs the target's $a0/$v1 rather than the other way round. */
void func_8003BFE4(void) {
    u8 *d = D_80082D08;
    u8 *base = (u8 *) &D_80083160;
    u8 *s = base + 0x20;
    s32 i;

    for (i = 0; i < 6; i++) {
        *(u16 *) d = *(u16 *) s;
        *(u16 *) (d + 2) = *(u16 *) (s + 2);
        *(u16 *) (d + 4) = *(u16 *) (s + 4);
        d += 8;
        s += 8;
    }
    func_8003BE1C(D_80082D08, 6, 4);
}

void func_8003C064(void) {
}

extern u8 D_80082E6Aa[] __asm__("D_80082E6A");

/* A return in each arm, not one `t` assigned in both: with a join the table
   pointer is a global allocno in $v1 and each arm's `(high)` is a block-local
   that takes $v0, where the target has the `lui` and the `addiu` on the same
   register. Per arm, cross-jumping merges the index and the load back into the
   one tail the listing shows and the pair can share a register. 4 DIFF.

   `== 2` with the fall-through arm second: written `!= 2` gcc branches TAKEN to
   the then-block and lays the arms out the other way round (1 DIFF). */
u32 func_8003C06C(s32 i) {
    if (D_80082E6Aa[0] == 2) {
        return D_8006B008[i];
    }
    return D_800D0858[i];
}

void func_8003C0A4(u8 *p, u32 *q) {
    if (q[2] != 0) {
        p[4] = 1;
    }
}

/* The second parameter is unused here -- func_8003C2B8 loads a word and passes
   it, so it is real. An unused parameter costs no instruction, so the body
   stays byte-identical. */
void func_8003C0C0(u8 *p, u32 a) {
    p[4] = 0;
}

void func_80044BB0(void);

void func_8003C0C8(u8 *p, s32 q) {
    if (D_8008099C == 0 && p[4] != 0) {
        func_80033CD8(p, (u32) func_80044BB0);
        func_800478B8(q);
    } else {
        func_80033D08(p);
    }
}

UnkNode *func_8003FE78(s32, UnkNode *, s16);
void func_8003C3B4(u8 *, u8 *, s32);

void func_8003C128(void) {
    /* u8 * rather than UnkNode *: the node's +0x20 is past the end of the
       struct, so the offsets stay explicit until something says what is
       there. */
    u8 *n = (u8 *) func_8003FE78(0x15, &D_80082D38, 0x49);

    *(u32 *) (n + 0x10) = (u32) func_8003C3B4;
    *(u32 *) (n + 0x20) = 0;
}

extern u8 D_800F26F8[];
extern u8 D_80077784[];
extern u8 D_800776FC[];

void func_8003DB94(u8 *, u32, s32);
void func_8003C0A4(u8 *, u32 *);
void func_8003C450(u32 *, s32, u8 *);

void func_8003C160(void)
{
    u32 *b = D_80082D58;
    u8 *e = (u8 *)b - 0x20;
    u8 *p = *(u8 **)(e + 0xC);
    u8 *q = *(u8 **)(e + 0x8);
    u8 *t;

    *(s16 *)(p + 0x1E) = 0x200;
    *(s16 *)(p + 0x1C) = 0x200;
    *(u32 *)(p + 0xC) = 0x808080;
    *(u32 *)(p + 0x8) = (u32)D_800F26F8;
    *(u32 *)(p + 0x0) = 0;
    p[4] = 0;
    p[5] = 0;
    func_8003DB94(p, b[2], 0);
    *(s16 *)(p + 0x14) = 0;
    *(s16 *)(p + 0x10) = 0;
    t = (u8 *)b[2];
    if (t == D_80077784 || t == D_800776FC)
    {
        *(s16 *)(p + 0x14) |= 0x400;
    }
    *(s16 *)(q + 0x2) = 0x54;
    *(s16 *)(q + 0x6) = 0x8D;
    *(s16 *)(q + 0xA) = 0x10;
    b[0] = (u32)func_8003C450;
    func_8003C0A4((u8 *)b, (u32 *)p);
}

void func_8003C4C8(u8 *, s32, u8 *);

void func_8003C24C(void) {
    u32 *b = D_80082D58;
    u8 *e = *(u8 **) ((u8 *) b - 0x14);

    if (e[0xC] == 0x80) {
        e[0xE] = 0x60;
        e[0xD] = 0x60;
        e[0xC] = 0x60;
        *(u16 *) (e + 0x10) = 0x20;
        *(u16 *) (e + 0x14) |= 0x1C;
    }
    b[0] = (u32) func_8003C4C8;
    func_8003C0A4((u8 *) b, (u32 *) e);
}

void func_8003C2B8(void) {
    u8 *p = (u8 *) D_80082D58;
    u32 a = *(u32 *) (p - 0x14);

    D_80082D58[0] = 0;
    func_8003C0C0(p, a);
    func_80033D08(p);
}

void func_8003C520(u8 *, s32, u8 *);

void func_8003C2F8(void) {
    u32 *b = D_80082D58;
    u8 *e = *(u8 **) ((u8 *) b - 0x14);

    if (e[0xC] == 0x80) {
        e[0xE] = 0x60;
        e[0xD] = 0x60;
        e[0xC] = 0x60;
        *(u16 *) (e + 0x10) = 0x20;
        *(u16 *) (e + 0x14) |= 0x1C;
    }
    b[0] = (u32) func_8003C520;
    func_8003C0A4((u8 *) b, (u32 *) e);
}

void func_8003C364(void) {
    u8 *p = (u8 *) D_80082D58;
    u32 g;

    func_80033D08(p);
    g = D_800814A0;
    *(u16 *) (p - 2) |= 0x8000;
    D_800814A0 = g | 0x8000;
}

/* The last store as an if/else, not a `?:`: as a conditional expression the
   value lands in $v1 instead of reusing the flag's $v0 (3 DIFF). The indirect
   call's argument is the pass-through $a0. */
void func_8003C3B4(u8 *p, u8 *q, s32 c) {
    u8 *b = (u8 *) &D_80083160;
    u32 f = *(u32 *) p;

    if (f != 0) {
        ((void (*)(u8 *)) f)(p);
        if ((*(u16 *) (p - 2) & 0x8000) != 0) {
            return;
        }
    }
    func_8003C0C8(p, c);
    if ((*(u32 *) (b + 8) & 0x10) != 0) {
        *(s16 *) (q + 0xA) = 5;
    } else {
        *(s16 *) (q + 0xA) = 0x10;
    }
}

void func_8003C488(u32 *, s32, u8 *);

void func_8003C450(u32 *cb, s32 b, u8 *p) {
    u16 *v = (u16 *) (p + 0x1E);

    *v += 0x400;
    if (*v >= 0x1000) {
        *v = 0x1000;
        *cb = (u32) func_8003C488;
    }
}

void func_8003C4C0(void);

void func_8003C488(u32 *cb, s32 b, u8 *p) {
    u16 *v = (u16 *) (p + 0x1C);

    *v += 0x200;
    if (*v >= 0x1000) {
        *v = 0x1000;
        *cb = (u32) func_8003C4C0;
    }
}

void func_8003C4C0(void) {
}

void func_8003C4C8(u8 *p, s32 b, u8 *q) {
    s32 v = q[0xE] - 0x10;

    *(u16 *) (q + 0x1C) -= 0x200;
    if (v < 0) {
        v = 0;
    }
    q[0xE] = v;
    q[0xD] = v;
    q[0xC] = v;
    if (*(s16 *) (q + 0x1C) <= 0) {
        *(u16 *) (q + 0x1E) = 0;
        *(u32 *) p = 0;
        func_8003C0C0(p, (u32) q);
    }
}

void func_8003C520(u8 *p, s32 b, u8 *q) {
    s32 n = q[0xE] - 0x10;

    *(u16 *) (q + 0x1C) -= 0x200;
    if (n < 0) {
        n = 0;
    }
    q[0xE] = n;
    q[0xD] = n;
    q[0xC] = n;
    if (*(s16 *) (q + 0x1C) <= 0) {
        func_80033D08(p);
        *(u16 *) (p - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* A `while` with the test on the INDEXED table, not a walked pointer: loop
   duplication gives the guard's `lb %lo(D_8006B1EC)` with the index folded
   away, and strength reduction then walks the pointer in the body. One
   `return -1` serves both the guard and the loop exit, which is why the target
   can copy the guard's comparison constant into $a3 and return it in $v0 --
   written as two returns the -1 is rematerialised and the index return needs
   its own block (11 DIFF, one word long). The pointer spelling instead swaps
   the pointer and the counter registers (13 DIFF). */
s32 func_8003C5A4(s32 key) {
    s32 i = 0;

    while (D_8006B1EC[i] != -1) {
        if (D_8006B1EC[i] == key) {
            return i;
        }
        i++;
    }
    return -1;
}

s32 func_8003C5A4(s32);

/* The key passes straight through from this function's own parameter, so no
   argument move is emitted -- see the pass-through note in
   docs/matching-levers.md section 3. */
u32 func_8003C5F0(s32 key) {
    s32 i = func_8003C5A4(key);

    if (i < 0) {
        return 0;
    } else {
        return D_8006B1A8[i];
    }
}

/* $gp group: D_80080A6C is in this unit's sdata_entries block, so this was
   verified in a scratch file carrying that block. The comparison is written
   with the parameter on the LEFT: the other way round the `beq` operands swap.
   */
s32 func_8003C634(u8 *a) {
    s32 i;

    for (i = 0; i < D_80080A6C; i++) {
        if ((u32) a == D_8006B200s[i].unk_00) {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C67C);

extern u8 D_80082E6Aa[] __asm__("D_80082E6A");

void func_8003C6F8(u32 a, s32 i) {
    /* Through the struct array, which brings the base materialisation out
       ahead of the index shift as the target has it (3 DIFF the other way). */
    u32 v = D_8006B200s[i].unk_04;

    D_8006B200s[i].unk_00 = a;
    D_80081460 = a;
    D_80080A8C = v;
    func_80041284((Unk41284 *) a);
    if (D_80082E6Aa[0] == 2) {
        D_800DCF4D = -2;
    }
}

extern u8 D_80082E6Aa[] __asm__("D_80082E6A");
s32 func_8003C634(u8 *);

/* $gp group: D_80080A6C is in this unit's sdata_entries block, so it was
   verified in a scratch file carrying that block. It is re-read after the call
   because the call invalidates it. */
s32 func_8003C758(u8 *p) {
    s32 r;

    if (D_80082E6Aa[0] == 2 && D_800DCF4D != -2) {
        r = -1;
    } else {
        r = func_8003C634(p);
    }
    if (r == -1) {
        func_8003C6F8((u32) p, D_80080A6C - 1);
        r = D_80080A6C - 1;
    }
    return r;
}

extern RECT D_8006B220[];
void func_8003C67C(s32);

/* NESTED guards with one `return -1` at the end, not three early returns:
   with three branches to a shared `-1` block reorg fills the second one's
   delay slot from that block and the `li -1` is duplicated, a word over
   (36 DIFF). Both parameters are `s32`, not the `(u8, s8)` the callers'
   prototype said: the listing has no sign extension of the index and no
   `andi` of the key before func_8003C5F0's pass-through. And the element
   address is written index-first, `(u8 *) (n * 8 + (u32) t)`. */
s32 func_8003C7D8(s32 k, s32 n) {
    u32 *d = D_80082D58;
    u8 *t;
    u8 *e;
    u8 *f;
    s32 r;
    u8 *b;

    if (n >= 0) {
        t = (u8 *) func_8003C5F0(k);
        if (t != 0) {
            e = (u8 *) (n * 8 + (u32) t);
            f = *(u8 **) e;
            if (f != 0) {
                d[2] = *(u32 *) (e + 4);
                r = func_8003C758(f);
                func_8003C67C(r);
                b = D_8006B200;
                LoadImage(D_8006B220, *(u32 **) (b + 4));
                LoadImage(&D_8006B220[1],
                          (u32 *) (*(u32 *) (b + 4) + 0x2000));
                return r;
            }
        }
    }
    return -1;
}

u32 func_8003C5F0(s32);

/* `||` is the shape, not two nested ifs. The short-circuit layout puts the if
   BODY between the two tests and the continuation last -- `bltz` to the body,
   `bnez` past it -- which is what the listing has. Every nesting of two tests
   with a `return 0` in each arm instead ends with gcc keeping the TRAILING
   copy of `return 0` and inverting the second branch, because cross-jumping
   merges the two identical return blocks and redirects the earlier one. 13 DIFF
   over four spellings. */
s32 func_8003C890(s32 a, s32 b) {
    u32 t;

    if (b < 0 || (t = func_8003C5F0(a)) == 0) {
        return 0;
    }
    return *(u32 *) (t + b * 8) != 0;
}

s32 func_8003C5A4(s32);

/* The argument passes straight through to func_8003C5A4, which is invisible
   here and shows up at func_800397A8, which passes a stream byte. */
u32 *func_8003C8E4(s32 k) {
    s32 i = func_8003C5A4(k);

    /* The arm written FIRST ends up SECOND in memory, so the `>= 0` arm first
       is what gives the target's `bgez`; written as `if (i < 0) return 0;`
       the branch comes out `bltz` and the two arms swap (6 DIFF). */
    if (i >= 0) {
        return &((u32 *) 0x800135C4)[i];
    } else {
        return 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C920);
