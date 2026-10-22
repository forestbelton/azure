#include "common.h"
void func_800439F8(void) {
    u8 *base;
    if (D_80080A8A != 0) {
        base = (u8 *) 0x80010000;
        D_80080A8A = 0;
        *(u32 *) (base + 0x3180) = D_80081494;
        base[0x3184] = D_800814AC;
        base[0x3186] = D_800814A4;
    } else {
        base = (u8 *) 0x80010000;
        D_80081494 = *(u32 *) (base + 0x3180);
        D_800814AC = base[0x3184];
        D_800814A4 = base[0x3186];
    }
}
