#include "common.h"

/* 15 words, 12 DIFF; see csrc/ovl_main_80020000.c for the measurement table.

   RE-MEASURED 2026-11-01: 12, not the 11 this note used to say, and four more
   spellings all give the same 12 -- an explicit `u8` or `s32` temp for p[0] as
   the FIRST statement (the lever that cracked the func_80022B48 family), that
   temp combined with the return value in a local, and `u32 *` for the word
   store; putting the word store after the p[0xA] byte store is 13. The target
   reads p[0] at word 4, between the two halves of `(n << 3) + 4`, and gcc
   will not move the read there from any source order; it also keeps the
   0xA04010 constant in $a0 where every spelling here puts it in $v1. */

u8 *func_80022300(s32 a, u8 *p, s32 n)
{
    p[1] = 0x28;
    p[0xB] = 0x13;
    *(s32 *)(p + 4) = 0xA04010;
    p[0xA] = (n << 3) + 4;
    p[0] |= 0x80;
    return p + 0xC;
}
