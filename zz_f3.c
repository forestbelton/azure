#include "common.h"
void func_800439F8(void) {
    u8 *base = (u8 *) 0x80010000;
    u32 a;
    u8 b, c;
    if (D_80080A8A != 0) {
        D_80080A8A = 0;
        a = D_80081494;
        b = D_800814AC;
        c = D_800814A4;
        *(u32 *) (base + 0x3180) = a;
        base[0x3184] = b;
        base[0x3186] = c;
    } else {
        a = *(u32 *) (base + 0x3180);
        b = base[0x3184];
        c = base[0x3186];
        D_80081494 = a;
        D_800814AC = b;
        D_800814A4 = c;
    }
}
