#include "common.h"

/* 49 words, 7 DIFF: the function is an 0x80-byte block copy of one D_80083E98 row from src+0x200, through a byte-aligned struct so gcc emits its unknown-alignment expansion (the (src|dst)&3 test plus both a lwl/lwr and a lw loop), then sets row[0] = 1. What differs is only the ORDER of the SECOND row-address computation: the target emits 'sll' then 'lui'/'addiu' then 'addu' (and reorg pulls the sll into the preceding j's delay slot), while every spelling here emits the base first. Three measured: the plain 2D index, a byte offset for the second access, and byte offsets for both. The FIRST computation's order (base first) already matches, so the two differ in the target and no single spelling gives both.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

/* Declarations this park needs; they live in csrc/ovl_main_80020000.c.
   Repeated here so the park compiles STANDALONE, which is what
   tools/decomp/permimport.py needs -- without them a permrun reports a
   silent `base=?` and searches nothing. */
extern s32 D_80083E98[][32];
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

/* Eight and 0x20 bytes, alignment 1, for the same reason as UnkFour:
   func_80025FFC copies D_800200B8 onto its stack and a D_80027E78 row into
   its argument with lwl/lwr + swl/swr. */
typedef struct
{
    u8 b[8];
} UnkEight;

typedef struct
{
    u8 b[0x20];
} UnkBlock20;

/* The 0x80-byte save-slot row, declared as a STRUCT alias of D_80083E98 (the
   unit already has it as `s32 [][32]`, which the other functions want).
   func_80024390 needs the struct spelling: its first row access is the name
   at +0xC, so gcc relocates against D_80083E98+0xC -- which splat names
   D_80083EA4 -- keeps that address in a callee-saved register, and derives
   the row base as `that - 0xC` in $fp for the other seven fields. With two
   separate symbols each access materialises its own %hi. */
typedef struct
{
    s32 unk_00;
    s32 unk_04;
    s16 unk_08;
    s16 unk_0A;
    u8 name[0x10];
    u32 unk_1C;
    s32 unk_20;
    s32 unk_24;
    s32 unk_28;
    u32 unk_2C;
    s32 unk_30;
    s32 unk_34;
    u8 pad_38[0x48];
} UnkSaveRow;

extern UnkSaveRow D_80083E98_s[] __asm__("D_80083E98");

/* 24 bytes, alignment 1: func_80022774 copies a D_80027F48 template into its
   third primitive with lwl/lwr + swl/swr. */
typedef struct
{
    u8 b[24];
} UnkBlock24;

/* Two words, alignment 4: func_8002789C copies the pair func_800217D4
   returns into p[6]/p[7] with gcc's aligned 8-byte block move (lw/lw/sw/sw);
   as two field assignments it is one word longer and interleaved. */
typedef struct
{
    s32 a;
    s32 b;
} UnkTwoWords;

/* The u16 pair at the front of func_80025FFC's stack copy: the loads are
   `lhu`, not `lh`, so the fields are unsigned. */
typedef struct
{
    u16 x;
    u16 y;
    u16 w;
    u16 h;
} UnkURect;

/* 0x80 bytes, alignment 1: func_80021DF4 copies a whole D_80083E98 row with
   gcc's unknown-alignment block move -- the `(src | dst) & 3` test and both a
   lwl/lwr and a lw loop. Through the s32[][32] type gcc would know both sides
   are word-aligned and emit only the second. */
typedef struct
{
    u8 b[0x80];
} UnkRow;

void func_80021DF4(s32 idx, u8 *src)
{
    *(UnkRow *)D_80083E98[idx] = *(UnkRow *)(src + 0x200);
    D_80083E98[idx][0] = 1;
}
