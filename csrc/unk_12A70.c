/* The remainder of unk_11940 after the gprel_12A40 carve-out. Named for its
 * start offset. */
#include "common.h"

s32 func_8003F270(void);
void func_8003E758(void);
u32 func_8003E240(s32);

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

INCLUDE_ASM("asm/nonmatchings/unk_12A70", func_8003F2A4);

void func_8003F320(void) {
    while (func_8003F270() != 0) {
        func_8003CB08();
        func_80044618(2);
        func_8003E758();
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_12A70", func_8003F368);

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
