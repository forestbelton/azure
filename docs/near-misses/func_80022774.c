#include "common.h"

/* NEAR MISS: 102 words -- the right count -- but 84 differing, nearly all of it register names cascading from one hoisting decision. This is func_80022A30's bigger sibling (same unaligned D_800200A0 copy, same four iterations, same 'p[-0x18] |= 0x80' and return) with THREE primitives per iteration instead of two: 0xC, 0xC and then 0x18 bytes, the last one a D_80027F48 template copied through a 24-byte alignment-1 struct and then patched with four 0x90D0E0 words. The width and height come from D_800280B4[q[2]].unk_16 * 4 and .unk_17 * 17, stored to q[6]/q[7] with +8 and halved for the sprite bytes. Declaring w and h as u32 is right -- it turns three of the four halvings from 'sra' into the 'srl' the target has, and the fourth, (w + 0xA) >> 1, really is 'sra' there. What is left: loop.c hoists all FOUR halvings out of the loop here, where the target hoists only three and leaves 'h + 0xA' unshifted in $t4, doing the 'srl' inside the loop each iteration -- a move_movables threshold decision, so the register pressure differs by one quantity. Seven spellings measured: >>1 and /2 on u32 and on s32 with casts (all 102w/84), and three shapes that hoist a, b and c into explicit locals and shift d inline (102w/88, and 111w/102 when the UnkStrSet row is re-indexed per value instead of cached in w/h).

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

/* Types and declarations this needs; they live in csrc/ovl_main_80020000.c,
   which is also where the live comment is. Repeated here so the park compiles
   standalone, which is what tools/decomp/permimport.py needs. */
typedef struct
{
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
} UnkFour;

typedef struct
{
    u8 b[24];
} UnkBlock24;

typedef struct
{
    u8 *strs[5];
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
} UnkStrSet;

extern u8 D_800200A0[];
extern u8 D_80027F48[];
extern UnkStrSet D_800280B4[];

u8 *func_80022774(u8 *p, s32 *q)
{
    u8 buf[4];
    u32 w;
    u32 h;
    s32 i;

    *(UnkFour *)buf = *(UnkFour *)D_800200A0;
    w = D_800280B4[q[2]].unk_16 * 4;
    h = D_800280B4[q[2]].unk_17 * 17;
    q[6] = w + 8;
    q[7] = h + 8;
    for (i = 0; i < 4; i++)
    {
        p[0] = buf[i];
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x104058;
        p[0xA] = (w + 8) >> 1;
        p[0xB] = (h + 8) >> 1;
        p += 0xC;
        p[0] = buf[i];
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x104058;
        p[0xA] = (w + 0xA) >> 1;
        p[0xB] = (h + 0xA) >> 1;
        p += 0xC;
        *(UnkBlock24 *)p = *(UnkBlock24 *)D_80027F48;
        *(s32 *)(p + 4) = 0x90D0E0;
        *(s32 *)(p + 0xC) = 0x90D0E0;
        *(s32 *)(p + 0x10) = 0x90D0E0;
        *(s32 *)(p + 0x14) = 0x90D0E0;
        p[0xA] = (w + 8) >> 1;
        p[0xB] = (h + 8) >> 1;
        p[0] = buf[i];
        p += 0x18;
    }
    p[-0x18] |= 0x80;
    return p;
}
