#include "common.h"

/* 57 words against 60: the target keeps the parameter p ITSELF in a fourth callee-saved register ($s3) as well as p+4 in $s0, and RECOMPUTES p+4 once after the if/else join -- which is where the three words go. Five spellings measured: p+4 in one local (57w/58), p+4 written inline at all eleven call sites (61w/31), the local reassigned after the join (61w/32), and the D_80028294 base hoisted into a local for indices 1..3 or for all four (both 57w/58). The body is right: strcpy from D_80028294[0], then func_8003AD08(n+1) into a stack buffer and a chain of strcats with D_80028294[3] twice, the 0x8001020C literal, D_80028294[1] or [2] on the 0x80010208 halfword, D_800282A0, and finally func_8003AD08 of the word at 0x8001022C.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

/* Declarations this park needs; they live in csrc/ovl_main_80020000.c.
   Repeated here so the park compiles STANDALONE, which is what
   tools/decomp/permimport.py needs -- without them a permrun reports a
   silent `base=?` and searches nothing. */
extern u8 *D_80028294[];
extern u8 *D_800282A0;

void func_80025F0C(u8 *p, s32 n)
{
    u8 buf[0x10];
    u8 *s = p + 4;

    func_80069E78(s, D_80028294[0]);
    func_8003AD08(n + 1, buf);
    func_80069E38(s, buf);
    func_80069E38(s, D_80028294[3]);
    func_80069E38(s, (u8 *)0x8001020C);
    func_80069E38(s, D_80028294[3]);
    if (*(s16 *)0x80010208 != 0)
    {
        func_80069E38(s, D_80028294[1]);
    }
    else
    {
        func_80069E38(s, D_80028294[2]);
    }
    func_80069E38(s, D_800282A0);
    func_8003AD08(*(s32 *)0x8001022C, buf);
    func_80069E38(s, buf);
}
