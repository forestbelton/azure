#include "common.h"
void func_8003E188(s16 arg0, s16 arg1) {
    u8 *p = (u8 *) 0x80010000;
    s32 sh;
    u8 v;
    s32 bits;

    p += arg0 >> 2;
    sh = (arg0 & 3) * 2;
    v = p[0x3614];
    bits = 1;
    if (arg1 != 0) {
        bits = 3;
    }
    p[0x3614] = v | (bits << sh);
}
