/* Was unk_102A8, renamed when its start moved to 0x102c4.
 *
 * func_8003CAA8 used to sit here as an INCLUDE_ASM stub and is now gone,
 * because it is not a function: `main` ends at 0x8003CAA4 with
 * `j .L8003CA2C`, an infinite loop, and the seven instructions above
 * 0x8003CAA8 are the epilogue that loop can never reach. splat had split them
 * off as their own function only because the dead code carries a label. They
 * belong to `main`, so the `asm, main` subsegment was extended from 0x102a8 to
 * 0x102c4 and this unit starts after them.
 *
 * Earlier comments elsewhere that say "carved out of unk_102A8" are historical
 * and still correct: this is the same unit under its new name.
 */
#include "common.h"

void func_8003D7DC(void);

void func_8003CAC4(void) {
    s32 i;

    for (i = 0x3F; i >= 0; i--) {
        D_80083858[i] = 0x500;
        D_800838D8[i] = 0x380;
    }
}
