#include "common.h"

void func_8003D7DC(void);

INCLUDE_ASM("asm/nonmatchings/unk_102A8", func_8003CAA8);

void func_8003CAC4(void) {
    s32 i;

    for (i = 0x3F; i >= 0; i--) {
        D_80083858[i] = 0x500;
        D_800838D8[i] = 0x380;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_102A8", func_8003CB08);

INCLUDE_ASM("asm/nonmatchings/unk_102A8", func_8003CCB0);

INCLUDE_ASM("asm/nonmatchings/unk_102A8", func_8003D0F0);

void func_8003D468(void) {
    u8 *base = (u8 *) 0x80010000;
    u8  a = base[0x3186];
    u32 b = *(u32 *) (base + 0x3180);
    u8  c = base[0x3184];
    u8  d = base[0x2D6E];
    u16 e = *(u16 *) (base + 0x2094);
    u16 f = *(u16 *) (base + 0x2098);

    bzero(base, 0x5C84);
    /* Both assignments are byte-aligned block moves: 6 bytes comes out as one
     * lwl/lwr pair plus two lb/sb, 10 bytes as two pairs plus two lb/sb. The
     * cast supplies the size and the alignment; D_80080AB4's own declaration
     * is an incomplete array so that its address is compiler-split. */
    *(Unk80080AB4 *) 0x8001020C = *(Unk80080AB4 *) D_80080AB4;
    *(Unk8002D5B4 *) 0x80010238 = D_8002D5B4;
    base[0x3186] = a;
    *(u32 *) (base + 0x3180) = b;
    base[0x3184] = c;
    base[0x2D6E] = d;
    *(u16 *) (base + 0x2094) = e;
    *(u16 *) (base + 0x2098) = f;
}

void func_8003D56C(void) {
    func_80069C90();
    func_80069CE8();
    _bu_init();
    ChangeClearPAD(0);
}

INCLUDE_ASM("asm/nonmatchings/unk_102A8", func_8003D5A4);

void func_8003D760(void) {
    /* The rect goes out as two words: four halfword field assignments would
       give four `sh`s, and an aggregate initializer lands in .sdata, which
       this unit's linker script discards. */
    u32 r[2];

    func_8005FFFC();
    func_8004EB30();
    func_8003D7DC();
    r[0] = 0;
    r[1] = 0x2000400;
    ClearImage(r, 0, 0, 0);
    D_8008148C = (u32) &D_80126804;
    D_80081480 = (u32) &D_80126804;
    D_80080A7C = 0x38000;
}

void func_8003D7DC(void) {
    /* The base has to come out as a lo_sum in a register: a constant address
     * or a scalar's `&` folds the +0x5C into a second `la` instead of the
     * target's single `addiu` off the base. */
    u8 *p = D_801C9E40;

    SetDefDrawEnv(p, 0, 0, 0x140, 0xE0);
    SetDefDrawEnv(&D_801DA714, 0, 0xE0, 0x140, 0xE0);
    SetDefDispEnv(p + 0x5C, 0, 0xE0, 0x140, 0xF0);
    SetDefDispEnv(&D_801DA770, 0, 0, 0x140, 0xF0);
    *(s16 *) (p + 0x1093A) = 0xC;
    *(s16 *) (p + 0x66) = 0xC;
    *(s16 *) (p + 0x1093E) = 0xE0;
    *(s16 *) (p + 0x6A) = 0xE0;
    D_801DA72C[0] = 1;
    *(u32 *) (p + 0x18) = 1;
}

void func_8003D8B0(void) {
    s32 i;

    func_8003DB4C((int *) &D_80083120, 0x10);
    func_8003DB4C((int *) &D_80082EC0, 0x80);

    /* Must be an indexed store into a real array: only an array reference
     * forms the address as (lo_sum base) + i*4, which is what leaves the
     * strength-reduced +0xC as its own addiu. A pointer form folds it. */
    for (i = 3; i >= 0; i--) {
        D_80083110[i] = 0;
    }

    D_80013714 = 0;
    func_800499BC();
    func_8004437C(0x2A, 0);
    func_8004425C(0);
}

INCLUDE_ASM("asm/nonmatchings/unk_102A8", func_8003D92C);
