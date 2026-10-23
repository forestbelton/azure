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
