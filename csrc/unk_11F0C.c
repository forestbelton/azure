/* The remainder of unk_11940 after the gprel_11CFC carve-out. Named for its
 * start offset. */
#include "common.h"

s32 func_8003F270(void);
void func_8003E758(void);
u32 func_8003E240(s32);

void func_8003E70C(void) {
    D_80083958.unk_08 = D_80083958.unk_08 + 1;
    D_80083958.unk_06 = D_80083958.unk_06 + 1;
    if (D_80083958.unk_06 > 0x400) {
        D_80083958.unk_06 = 0;
        CdInit();
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_11F0C", func_8003E758);
