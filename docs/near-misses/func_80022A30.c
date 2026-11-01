#include "common.h"

/* 44 words against 46, and the two missing words are COPIES of hoisted constants: the target has 'addiu $t4, $zero, 5' then 'addu $t3, $t4, $zero', and the same pair for 6, where every spelling here CSEs the two 5s into one pseudo before loop.c sees them. Per docs/matching-levers.md:567, combine_movables matches two movables of equal value and mode and hoists one while the other becomes a copy -- so the target's two 5s survived cse1 as separate pseudos and this one's did not. The rest is right: an unaligned 4-byte copy of D_800200A0 to a stack buffer, then four iterations writing two 0xC-byte records each (byte 0 from the buffer, byte 1 = 0x48, word at 4 = 0x104058, bytes 0xA/0xB = 5 then 6), then 'p[-0x18] |= 0x80' and return p.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

u8 *func_80022A30(u8 *p, s32 *q)
{
    u8 buf[4];
    s32 i;

    *(UnkFour *)buf = *(UnkFour *)D_800200A0;
    for (i = 0; i < 4; i++)
    {
        p[0] = buf[i];
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x104058;
        p[0xA] = 5;
        p[0xB] = 5;
        p += 0xC;
        p[0] = buf[i];
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x104058;
        p[0xA] = 6;
        p[0xB] = 6;
        p += 0xC;
    }
    p[-0x18] |= 0x80;
    return p;
}
