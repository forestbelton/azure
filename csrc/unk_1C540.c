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

void func_80048C3C(s32);

void func_80048D40(void) {
    func_80048C3C(2);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80048D60);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80048DA0);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049150);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049330);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049374);

void func_80049374(s32, u8 *);

void func_80049490(s32 a) {
    func_80049374(a, D_800713FC);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800494B4);

s32 func_800494FC(u8 *p) {
    return func_80042A80(p) & 0x400;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004951C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049790);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800497F4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800498A0);

u32 func_800498EC(s32 i) {
    return D_8007361C[D_800712AC[i]];
}

u32 func_80049918(s32 i) {
    return D_8007361C[D_800712B0[i]];
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049944);

s32 func_80049944(u32 *);

s32 func_80049984(u8 *p) {
    s32 r = 3;

    if (p[0x13] != 0) {
        r = func_80049944(*(u32 **) (p + 0x14));
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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049D80);

void func_80049CF4(void);

void func_80049DC8(void) {
    func_80049CF4();
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049DE8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049E1C);

u32 func_80049E6C(s32 i) {
    return D_800712B4[i];
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049E88);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049F2C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049F68);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A030);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A0C0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A10C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A170);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A24C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A330);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A464);

s32 func_8004A4C4(s32 arg0, u8 *p) {
    return func_800438E4(p);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A4E4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A534);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A574);

void func_8004A574(s32, s32);

void func_8004A618(s32 a) {
    func_8004A574(a, 1);
}

void func_8004A574(s32, s32);

void func_8004A638(s32 a) {
    func_8004A574(a, 0);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A658);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A6C0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A700);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A784);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A7D8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A83C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A8D8);

void func_8004A918(void) {
    bzero((void *) 0x80013564, 0x50);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A940);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004AE98);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004AEC4);

/* A forwarding wrapper: its four arguments pass through untouched, which is
   invisible in the listing (no register setup before the jal) and only shows
   up at a caller -- func_8004AA44 passes four. */
void func_8004AEC4(s32, s32, s32, void *);

void func_8004AFC8(s32 a, s32 b, s32 c, void *d) {
    func_8004AEC4(a, b, c, d);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004AFE8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B028);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B08C);

/* s32 rather than u8 *, because the two callers 280 lines below already
   declare it that way and pass their own s32 parameter straight through. */
void func_8004B1A4(s32 a) {
    if (a != 0) {
        *(u16 *) (a - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B1DC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B248);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B298);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B2E0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B324);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B364);

void func_8004B364(s32, s32, s32);

void func_8004B3DC(s32 a) {
    func_8004B364(0, 1, a);
}

void func_8004B364(s32, s32, s32);

void func_8004B404(s32 a) {
    func_8004B364(0, 0, a);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B42C);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B634);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B660);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B6BC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B718);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B774);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B7D0);

void func_8004B634(void);

void func_8004B834(void) {
    func_8004B634();
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B854);

void func_8004B854(s32, s32, s32);

void func_8004B8BC(s32 a, s32 b) {
    func_8004B854(a, b, 0);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B8DC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B954);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004BDDC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004C010);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004C080);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004C36C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004CAA0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004CAE8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004CB2C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004CBFC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004CC38);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004CCBC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004CD28);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004CE68);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D45C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D4AC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D5D0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D614);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D690);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D70C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D75C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D7A8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D7E8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D828);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D880);

void func_8004D8F0(u32 a, u8 *p) {
    p[0] = (a & 0xF) << 3;
    p[1] = (a & 0xF0) - 0x20;
    p[2] = 8;
    p[3] = 0x10;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D91C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D98C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D9E0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004DA24);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004DA74);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004DC14);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004DCA8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004DCE0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004DCEC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004DD2C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004DDB4);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E21C);

void func_8004E264(u16 *p, u32 v) {
    u32 t = (v & 0xF) + 0x7C80;

    p[3] = t + ((v & 0x10) << 3);
}

u8 *func_8004E280(u8 *p, s32 *out) {
    *out = *p - 0x30;
    return p + 1;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E298);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E478);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E4C0);

void func_8004E4C0(s32, u16, s32, s32);

void func_8004E57C(s32 a, u16 b, s32 c) {
    func_8004E4C0(a, b, c, 0x20);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E5A0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E5E8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E634);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E69C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E6F4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E884);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E8D8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E928);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004EA70);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004EAC4);

void func_8004EB30(void) {
    D_80084130[0] = 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004EB3C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004EB8C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004EBDC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004EC20);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004ECAC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004ED00);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004ED5C);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004EEBC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004EEFC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004EF90);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004EFF4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F068);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F0DC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F1E8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F2BC);

void func_800491F4(s32, u8 *, s32);

void func_8004F308(s32 a) {
    func_800491F4(a, D_80071684, 7);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F330);

void func_8004F3D0(u32 *p, u32 v) {
    *p = v;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F3D8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F418);

void func_8004F52C(u8 *p) {
    u32 g;

    if (p != 0) {
        g = D_800814A0a[0];
        *(u16 *) (p + 0x1E) |= 0x8000;
        D_800814A0a[0] = g | 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F558);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F5B0);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F95C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F9AC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004FA2C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004FAA4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004FC2C);

u32 func_80021448(u8 *);

void func_8004FC68(u8 *p) {
    *(u32 *) (p + 0x24) = func_80021448(p + 0x10);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004FC98);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004FD78);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004FDE0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004FE78);

u32 func_8004FF00(u8 *p) {
    u32 *q = (u32 *) (p + 0x20);

    return q[q[12] + 4];
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004FF20);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004FFA8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004FFF4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800500B4);

void func_80021300(u32);

void func_800502E0(u8 *p) {
    func_80021300(func_8004FF00(p));
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050308);

void func_80050308(void);

void func_80050494(void) {
    func_80050308();
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800504B4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050550);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800505BC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050640);

void func_80050694(u8 *p) {
    (*(void (**)(u8 *)) p)(p);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800506BC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005084C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800508F0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005097C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800509C4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050A00);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050A58);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050AF0);

void func_800491F4(s32, u8 *, s32);

void func_80050B7C(s32 a) {
    func_800491F4(a, D_800717BC, 0x11);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050BA4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050CAC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050CDC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050DA8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050E20);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050EA8);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800510A0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800510DC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80051164);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800511B4);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80052A20);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800530C4);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80053CFC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80053D64);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80053E20);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80053E90);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80053EF0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80053F98);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005405C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800540A8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054104);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800541E8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054228);

s32 func_800542A4(void) {
    return 0;
}

void func_800542AC(void) {
    D_800847F6[0] = -1;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800542BC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800543C8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005440C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800544A4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054538);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800545F4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800546B0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054704);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054788);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005497C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800549FC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054A7C);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005537C);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80055778);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80057A50);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005863C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800586B0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058700);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800587B4);

void func_800587F8(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058800);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058850);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800588C8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058940);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800589B8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058A04);

u32 func_80058A7C(u32 a, u32 b, u32 c, u32 d) {
    return ((((((a & 0xFF) << 8) + (b & 0xFF)) << 8) + (c & 0xFF)) << 8) | (d & 0xFF);
}

u32 func_80058AA8(u32 a, u32 b) {
    return ((a & 0xFF) << 8) | (b & 0xFF);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058ABC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058B2C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058B70);

void func_80058E50(s32 *p, s32 v) {
    *p += v;
}

void func_80058E64(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058E6C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800590E0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005914C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005947C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800595C0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800597A8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80059814);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800599B0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80059BC4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80059D1C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80059D60);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80059DAC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80059E94);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80059F8C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005A1D0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005A26C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005A2FC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005A33C);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005A5C8);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005AC68);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005B27C);

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

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005B378);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005B3D8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005B418);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005B470);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005B4D0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005BAB0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005BBFC);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005C130);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005C2C0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005C4D0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005C5D0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005C668);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005C6B4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005C710);

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
