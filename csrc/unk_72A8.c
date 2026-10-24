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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800350D4);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035164);

void func_80035164(s16 *, s32, s32);

void func_80035208(s32 a) {
    func_80035164(D_80082A38, a, 2);
}

void func_80035164(s16 *, s32, s32);

void func_80035234(s32 a) {
    func_80035164(D_80082A38, a, 1);
}

void func_80035164(s16 *, s32, s32);

void func_80035260(s32 a) {
    func_80035164(D_80082A38, a, 0);
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
void func_8003566C(void);

void func_80035614(u8 *p) {
    func_80035888(D_800821CC, D_80081F14, p, 0, *(u32 *) (p + 0x74));
    p[0x4C] = 0;
    *(u32 *) (p + 0x68) = (u32) func_8003566C;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003566C);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800359FC);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035AF8);

void func_8003C7D8(u8, s8);
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

void func_80035C80(void);

void func_80035C44(u8 *p, u8 *q) {
    u16 *v = (u16 *) (q + 2);

    *v += 0x10;
    if ((s16) *v > 0x40) {
        *v = 0x40;
        *(u32 *) (p + 0x68) = (u32) func_80035C80;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035C80);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035CE4);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035DBC);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035E58);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003605C);

void func_80036154(void);

void func_80036110(u8 *p) {
    *(u16 *) (*(u8 **) p + 0xA) -= 0x140;
    if (*(s16 *) (*(u8 **) p + 0xA) < 0) {
        *(u16 *) (*(u8 **) p + 0xA) = 0;
        *(u32 *) (p + 0x68) = (u32) func_80036154;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036154);

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

void func_80036154(void);

void func_800362A0(u8 *p) {
    if (p[0x50] != 0) {
        (*(u8 **) (p + 0x74))[0x84] = 1;
        *(u32 *) (p + 0x68) = (u32) func_80036154;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800362D0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036350);

void func_80036484(u8 *);

void func_80036434(u8 *p) {
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

void func_80036154(void);

void func_800364BC(u8 *p) {
    if (p[0x51] != 0) {
        (*(u8 **) (p + 0x74))[0x84] = 1;
        *(u32 *) (p + 0x68) = (u32) func_80036154;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800364EC);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036574);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800366F4);

void func_80036484(u8 *);

void func_80036880(u8 *p) {
    u8 *q = *(u8 **) (p + 0x74);

    q[0x27] = p[0x4D];
    *(u16 *) (p + 0x64) = 0x11;
    *(u32 *) (p + 0x68) = (u32) func_80036484;
    p[0x4D] |= 0x80;
    func_80053DA8(0x503);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800368D0);

void func_800368F8(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036900);

void func_80036980(void) {
}

void func_80036988(u8 *d, u8 *s) {
    *(u16 *) (d + 0x8) = *(u16 *) (s + 0x0) + *(u16 *) (s + 0x8);
    *(u16 *) (d + 0xA) = *(u16 *) (s + 0x2) + *(u16 *) (s + 0xA);
    *(u16 *) (d + 0xC) = *(u16 *) (s + 0xC);
    *(u16 *) (d + 0xE) = *(u16 *) (s + 0xE);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800369C8);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036A70);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036B10);

s32 func_80037534(s32, s32, s32);
void func_800478B8(s32);

void func_80036C44(s32 a, s32 b, s32 c) {
    if (func_80037534(a, b, c) == 0) {
        func_800478B8(c);
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036C7C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036D4C);

void func_80036D4C(u8 *, s32, s32, s32, s32, u32);
void func_80036F24(u8 *, u32, u32);

void func_80036ED4(u8 *p, s32 b, s32 c, s16 d, s16 e, u32 f) {
    func_80036D4C(p, b, c, d, e, f);
    func_80036F24(p + 0x20, *(u32 *) (p + 0x8), *(u32 *) (p + 0xC));
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80036F24);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037090);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003719C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037394);

u32 func_800374F4(u16 n) {
    /* The product must be unsigned -- `(u32)` -- or the high half comes out
       with `sra` where the target has `srl`. */
    return (u32) (n * (u16) (rand() << 1)) >> 16;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037534);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800375C0);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037714);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037C7C);

/* The second parameter is unused here; func_80034EF8 masks a byte into it, so
   it is real. An unused parameter costs no instruction. */
void func_80037D50(u8 *p, u8 b) {
    u8 *q = p + 0x8C;

    q[0x25] = D_8006A908[p[2]];
    *(u16 *) (q + 0xE) = D_8006A8B4[q[0x2A]] * q[0x25];
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037D9C);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037E58);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80037F70);

void func_80038000(u8 v) {
    D_80082B4B[0] = v;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003800C);

void func_80038A10(void);

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

void func_80038A10(void);

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

void func_80038A10(void);

void func_80038368(u8 *p) {
    u16 *c = (u16 *) (p + 0x18);

    *c = *c - 1;
    if ((s16) *c <= 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_80038A10(void);

void func_80038394(u8 *p) {
    if (func_80033B2C(*(s16 *) (p + 0x30)) != 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_80038A10(void);

void func_800383D4(u8 *p) {
    if (D_80082660[p[0x2F]].unk_01 == 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

u8 *func_800392A4(u8);
void func_80038A10(void);

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

void func_80038A10(void);

void func_80038538(u8 *p) {
    func_80053DA8(0xB4);
    *(u32 *) (p + 0x10) = (u32) func_80038A10;
}

void func_80038570(u8 *p) {
    p[0x27] = 0;
    p[0x84] = 0;
    *(u32 *) (p + 0x10) = (u32) func_80038588;
}

void func_80038A10(void);

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

void func_80038A10(void);

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

void func_80038A10(void);

void func_80038648(u8 *p) {
    if (*(s8 *) (p + 0x84) != 0) {
        func_80053DA8(0xB4);
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_80038A10(void);

void func_80038690(u8 *p) {
    u8 *s = *(u8 **) (p + 0x80);
    u32 r = (*(u32 (**)(u32, u32, u32, u32)) (*(u8 **) (s + 0x40) + *(s8 *) (p + 0x32) * 4))(*(u32 *) (s + 0x48), *(u32 *) (s + 0x4C), *(u32 *) (s + 0x50), *(u32 *) (s + 0x54));

    *(u32 *) (s + 0x84) = r;
    if (r != 0) {
        *(u32 *) (p + 0x34) = r;
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_80038A10(void);

void func_80038708(u8 *p) {
    ((void (*)(void)) D_800DCF54[*(s8 *) (p + 0x32)])();
    *(u32 *) (p + 0x10) = (u32) func_80038A10;
}

/* D_80110E98 is an ARRAY of pointers, not a pointer to one: the target takes
   its address (`addiu %lo`) where `extern u32 **` loads through it. */
extern u32 *D_80110E98[];
void func_80038A10(void);

void func_8003875C(u8 *p) {
    u32 r = (*(u32 (**)(u32)) (D_80110E98[*(s8 *) (p + 0x32)] + *(s8 *) (p + 0x33)))(*(u32 *) (p + 0x34));

    if (r != 0) {
        *(u32 *) (p + 0x34) = r;
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800387D0);

void func_80038A10(void);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038A10);

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
    ClearImage(p, 0, 0, 0);
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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039470);

void func_80039560(void) {
}

void func_80039568(u32 *p) {
    p[4] = (u32) func_80038464;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039578);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800395B0);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039694);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039730);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039A08);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039C44);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039C74);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039D10);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039DBC);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80039E1C);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A1A4);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003AC1C);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003AD08);

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
    s16 r[4];

    r[0] = x;
    r[1] = y;
    r[2] = w;
    r[3] = h;
    LoadImage(r, data);
}

void func_8003AE28(s16 x, s16 y, s16 w, s16 h, s32 a, s32 b) {
    s16 r[4];

    r[0] = x;
    r[1] = y;
    r[2] = w;
    r[3] = h;
    MoveImage(r, a, b);
}

void func_8003AE60(s16 x, s16 y, s16 w, s16 h, u8 r, u8 g, u8 b) {
    s16 rect[4];

    rect[0] = x;
    rect[1] = y;
    rect[2] = w;
    rect[3] = h;
    ClearImage(rect, r, g, b);
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

void func_8003C758(u8 *);

void func_8003B06C(void) {
    func_8003C758(D_80080F40);
    func_8003C758(D_80081010);
    func_8003C758(D_80080F60);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B0B4);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B35C);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B64C);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003B820);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003BFE4);

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
void func_8003C3B4(void);

void func_8003C128(void) {
    /* u8 * rather than UnkNode *: the node's +0x20 is past the end of the
       struct, so the offsets stay explicit until something says what is
       there. */
    u8 *n = (u8 *) func_8003FE78(0x15, &D_80082D38, 0x49);

    *(u32 *) (n + 0x10) = (u32) func_8003C3B4;
    *(u32 *) (n + 0x20) = 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C160);

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

void func_8003C520(void);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C3B4);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C520);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C634);

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

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C758);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C7D8);

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
