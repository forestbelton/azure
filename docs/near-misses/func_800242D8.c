#include "common.h"

/* 46 words, 10 DIFF: the halved width and height must be computed INSIDE the loop (they are loop-invariant, so loop.c hoists them, and hoisting puts them AFTER the three constant movables where the target has them) -- that took it from 24 DIFF to 10. BODY CORRECTED 2026-11-01: the file had been saved with the halvings hoisted
   into locals BEFORE the loop, which is the 24-DIFF shape the note was
   describing as wrong -- it now carries the 10-DIFF body it describes. Written
   inline at the two stores is 10; in locals assigned inside the loop, 22; in
   locals before the loop, 24.

   What is left is the SCHEDULE around the D_80027FD0 index. The target
   computes `addu $v0, $a3, $a2` (&D_80027FD0[i]) and `addiu $a3, $a3, 1`
   at the TOP of the loop body, before the five stores; here they come after
   it, so the body is the target shifted by two. Reading the byte into a local
   at the top of the loop -- the lever that cracked the func_80022B48 family
   -- makes it WORSE here, 17 either as `u8` or as `s32`. The shape is right: a 0xC-byte header, four 0x18-byte records (byte 1 = 0x38, three words of 0x9CBCBC then one of 0x8C9C9C, bytes 0xA/0xB the halved extents, byte 0 from D_80027FD0[i]), then a 0xC-byte trailer, returning p + 0xC.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

/* Declarations this park needs; they live in csrc/ovl_main_80020000.c.
   Repeated here so the park compiles STANDALONE, which is what
   tools/decomp/permimport.py needs -- without them a permrun reports a
   silent `base=?` and searches nothing. */
extern u8 D_80027FD0[];

u8 *func_800242D8(u8 *p, s32 w, s32 h)
{
    s32 i;

    p[1] = 0x81;
    *(s16 *)(p + 4) = 0x20;
    *(s16 *)(p + 6) = 0;
    p += 0xC;
    for (i = 0; i < 4; i++)
    {
        p[1] = 0x38;
        *(s32 *)(p + 4) = 0x9CBCBC;
        *(s32 *)(p + 0xC) = 0x9CBCBC;
        *(s32 *)(p + 0x10) = 0x9CBCBC;
        *(s32 *)(p + 0x14) = 0x8C9C9C;
        p[0xA] = w / 2;
        p[0xB] = h / 2;
        p[0] = D_80027FD0[i];
        p += 0x18;
    }
    p[1] = 0x81;
    *(s16 *)(p + 4) = 0x20;
    *(s16 *)(p + 6) = 1;
    p[0] = 0x80;
    return p + 0xC;
}
