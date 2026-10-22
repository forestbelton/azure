#include "common.h"
void func_8003E188(s16 arg0, s16 arg1) {
    s32 sh = (arg0 & 3) * 2;
    u8 v = *((u8 *) 0x80013614 + (arg0 >> 2));
    s32 bits = 1;

    if (arg1 != 0) {
        bits = 3;
    }
    *((u8 *) 0x80013614 + (arg0 >> 2)) = v | (bits << sh);
}
