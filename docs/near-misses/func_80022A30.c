#include "common.h"

/* 46 words -- the right count now -- and 8 DIFF. UPDATED 2026-11-01 with two
   corrections the old 44-word body had wrong.

   1. The byte-0 store comes LAST in each primitive, not first. The target's
      order is p[0xA], p[0xB], p[1], *(p+4), then p[0] = buf[i], and buf[i] is
      re-read once per primitive (words 24 and 32). With p[0] first the
      function is 44 words.
   2. The two bytes at 0xA and 0xB come from a register and a COPY of it --
      `addiu $t4, $zero, 5` then `addu $t3, $t4, $zero`, and the same pair for
      6. That is the narrower-destination lever from azure-matching-tips: a
      copy survives as a second register only if cse cannot see it as a
      same-mode register copy, and `u8 h = w` is a QImode move, which MIPS
      emits as a plain `addu`. As literals the two 5s coalesce.

   Together those took it from 36 differing words to 8, and the 8 are the
   ORDER of the loop preheader. The target emits `i = 0`, then the 5 pair,
   then the 0x48 and 0x104058 hoists, then the 6 pair -- which is loop-body
   order, so in the original the pairs were hoisted OUT of the loop by loop.c
   rather than assigned before it. Declaring them inside the loop body gets
   that order but loses the copies (gcc coalesces them again, 44w/36), and
   assigning them before the loop keeps the copies but puts both pairs ahead
   of the for-init. Also measured: the pairs initialised at their declarations
   rather than after the buffer copy (46w/15), a local for w with a literal
   for h (46w/17), the reverse (46w/19), and only the first pair as locals
   (45w/42). One for the permuter -- it is combine_movables deciding which of
   two equal movables to hoist and which to make a copy.

   The ORIGINAL note, still accurate about the mechanism: per
   docs/matching-levers.md:567, combine_movables matches two movables of equal
   value and mode and hoists one while the other becomes a copy, so the
   target's two 5s survived cse1 as separate pseudos and a literal pair does
   not. */

/* Declarations this park needs; they live in csrc/ovl_main_80020000.c.
   Repeated here so the park compiles STANDALONE, which is what
   tools/decomp/permimport.py needs -- without them a permrun reports a
   silent `base=?` and searches nothing. */
typedef struct
{
    s32 unk_00;
    s32 unk_04;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkCardPrim;

typedef struct
{
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
} UnkCardPrim12;

/* Four bytes, alignment 1: func_80026128 copies this out of D_80028064 with
   lwl/lwr + swl/swr, which is what gcc emits for a struct it cannot assume is
   word-aligned. */
typedef struct
{
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
} UnkFour;
extern u8 D_800200A0[];

u8 *func_80022A30(u8 *p, s32 *q)
{
    u8 buf[4];
    u8 w1;
    u8 h1;
    u8 w2;
    u8 h2;
    s32 i;

    *(UnkFour *)buf = *(UnkFour *)D_800200A0;
    w1 = 5;
    h1 = w1;
    w2 = 6;
    h2 = w2;
    for (i = 0; i < 4; i++)
    {
        p[0xA] = w1;
        p[0xB] = h1;
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x104058;
        p[0] = buf[i];
        p += 0xC;
        p[0xA] = w2;
        p[0xB] = h2;
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x104058;
        p[0] = buf[i];
        p += 0xC;
    }
    p[-0x18] |= 0x80;
    return p;
}
