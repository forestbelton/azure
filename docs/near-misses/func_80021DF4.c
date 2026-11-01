#include "common.h"

/* 49 words, 7 DIFF: the function is an 0x80-byte block copy of one D_80083E98 row from src+0x200, through a byte-aligned struct so gcc emits its unknown-alignment expansion (the (src|dst)&3 test plus both a lwl/lwr and a lw loop), then sets row[0] = 1. What differs is only the ORDER of the SECOND row-address computation: the target emits 'sll' then 'lui'/'addiu' then 'addu' (and reorg pulls the sll into the preceding j's delay slot), while every spelling here emits the base first. Three measured: the plain 2D index, a byte offset for the second access, and byte offsets for both. The FIRST computation's order (base first) already matches, so the two differ in the target and no single spelling gives both.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

void func_80021DF4(s32 idx, u8 *src)
{
    *(UnkRow *)D_80083E98[idx] = *(UnkRow *)(src + 0x200);
    D_80083E98[idx][0] = 1;
}
