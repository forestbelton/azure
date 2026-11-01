#include "common.h"

/* 45 words, 28 DIFF, all register NAMES: the open/lseek/read/close sequence is right (func_8006A0A4/B4/C4/E4 = open/lseek/read/close on "bu00:"), but local-alloc assigns the five callee-saved locals differently -- the target has buf=$s5, n=$s2, off=$s4, r=$s1, -1=$s3, fd=$s0 and keeps the -1 in a register across both comparisons. Three spellings measured (an explicit -1 local, fd declared first with a len local, the plain form); the plain form is this one. Allocation only.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

s32 func_80021B98(u8 *name, u8 *buf, s32 n, s32 off)
{
    s32 r = 0;
    s32 fd;

    fd = func_8006A0A4(name, 1);
    if (fd != -1)
    {
        if (off == 0 || func_8006A0B4(fd, off << 7, 0) != -1)
        {
            r = func_8006A0C4(fd, buf, n << 7) == (n << 7);
        }
        func_8006A0E4(fd);
    }
    return r;
}
