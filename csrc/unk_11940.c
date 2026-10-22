#include "common.h"

s32 func_8003F270(void);
void func_8003E758(void);

void func_8003E140(s8 *arg0) {
    *arg0 = 1;
}

s32 func_8003E14C(void) {
    s32 v;

    /* The mode byte is a field of the Unk80082E60 struct, not a standalone
     * global: going through the struct is what makes the compiler split the
     * address into a shared lui plus a lbu with a %lo displacement. */
    if (D_80082E60.unk_0A != 2) {
        return 0;
    }

    v = 0x28;
    if (D_8008146C < 0x29) {
        v = D_8008146C;
    }
    return v;
}

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003E188);

void func_8003E1C4(s16 arg0) {
    func_80041284(D_8006CDC8[arg0]);
}

void func_8003E1FC(void) {
    u8 *p = &D_80080AC0;

    if (D_80082E60.unk_0A == 2) {
        p = &D_80080AC8;
    }
    func_8003E4FC(6, p, 0);
}

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003E240);

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003E2D8);

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003E34C);

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003E39C);

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003E4FC);

void func_8003E70C(void) {
    D_80083958.unk_08 = D_80083958.unk_08 + 1;
    D_80083958.unk_06 = D_80083958.unk_06 + 1;
    if (D_80083958.unk_06 > 0x400) {
        D_80083958.unk_06 = 0;
        CdInit();
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003E758);

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003F240);

s32 func_8003F270(void) {
    /* The target materialises the address of element 1 and reaches element 0
     * through it with a -1 displacement, and reloads element 0 for the
     * subtraction rather than keeping it live; the separate `t` is what stops
     * the second read being held in a register. */
    u8 *p = &D_800814D0[1];
    u32 v = D_800814D0[1];
    u8 t = p[-1];

    if (v < t) {
        v += 0x20;
    }
    return v - p[-1];
}

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003F2A4);

void func_8003F320(void) {
    while (func_8003F270() != 0) {
        func_8003CB08();
        func_80044618(2);
        func_8003E758();
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003F368);

s32 func_8003F52C(s32 arg0) {
    return arg0;
}

Unk8006CE34 *func_8003F534(void) {
    return &D_8006CE34;
}

void func_8003F540(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    D_8006CE34.unk_00 = arg0;
    D_8006CE34.unk_04 = arg1;
    D_8006CE34.unk_08 = arg2;
    D_8006CE34.unk_0C = arg3;
}

s32 func_8003F55C(void) {
    if (CdSync(1, NULL) == 2) {
        CdControl(0x10, NULL, D_800814B8);
    }
    return CdPosToInt(D_800814B8);
}

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003F5AC);
