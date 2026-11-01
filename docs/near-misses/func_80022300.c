#include "common.h"

/* 15 words, 11 DIFF; see csrc/ovl_main_80020000.c for the measurement table. */

u8 *func_80022300(s32 a, u8 *p, s32 n)
{
    p[1] = 0x28;
    p[0xB] = 0x13;
    *(s32 *)(p + 4) = 0xA04010;
    p[0xA] = (n << 3) + 4;
    p[0] |= 0x80;
    return p + 0xC;
}
