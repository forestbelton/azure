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

INCLUDE_ASM("asm/nonmatchings/unk_102A8", func_8003D468);

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

INCLUDE_ASM("asm/nonmatchings/unk_102A8", func_8003D7DC);

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
