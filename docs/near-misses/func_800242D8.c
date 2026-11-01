#include "common.h"

/* 46 words, 10 DIFF: the halved width and height must be computed INSIDE the loop (they are loop-invariant, so loop.c hoists them, and hoisting puts them AFTER the three constant movables where the target has them) -- that took it from 24 DIFF to 10. What is left is register naming around those two divisions. The shape is right: a 0xC-byte header, four 0x18-byte records (byte 1 = 0x38, three words of 0x9CBCBC then one of 0x8C9C9C, bytes 0xA/0xB the halved extents, byte 0 from D_80027FD0[i]), then a 0xC-byte trailer, returning p + 0xC.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

u8 *func_800242D8(u8 *p, s32 w, s32 h)
{
    s32 i;
    s32 hw;
    s32 hh;

    p[1] = 0x81;
    *(s16 *)(p + 4) = 0x20;
    *(s16 *)(p + 6) = 0;
    p += 0xC;
    hw = w / 2;
    hh = h / 2;
    for (i = 0; i < 4; i++)
    {
        p[1] = 0x38;
        *(s32 *)(p + 4) = 0x9CBCBC;
        *(s32 *)(p + 0xC) = 0x9CBCBC;
        *(s32 *)(p + 0x10) = 0x9CBCBC;
        *(s32 *)(p + 0x14) = 0x8C9C9C;
        p[0xA] = hw;
        p[0xB] = hh;
        p[0] = D_80027FD0[i];
        p += 0x18;
    }
    p[1] = 0x81;
    *(s16 *)(p + 4) = 0x20;
    *(s16 *)(p + 6) = 1;
    p[0] = 0x80;
    return p + 0xC;
}
