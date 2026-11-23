#include "common.h"

typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
typedef struct UnkDng73E94SP
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x12];
    u8 *unk_18;
    u8 pad_1C[0x4];
    u8 *unk_20;
    u8 pad_24[0x6C];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x28];
    s32 unk_C0;
} UnkDng73E94SP;
typedef struct UnkDng74334SP
{
    u8 pad_00[0xC];
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    Unk3DBD0W unk_18;
    u8 pad_1C[0x8];
    u8 *unk_24;
    u16 unk_28;
    u8 pad_2A[0xA];
    VECTOR unk_34;
    u8 pad_44[0x10];
    MATRIX unk_54;
    MATRIX unk_74;
    u8 pad_94[0x10];
    SVECTOR unk_A4;
    u8 pad_AC[0x4];
    SVECTOR unk_B0[4];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDng74334SP;
void func_s17_80024188(u8 *p, UnkVec *v, UnkSlot *s, s16 d);

void func_s17_80024758(u8 *p, UnkVec *v, UnkSlot *s, s16 d);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80024098);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80024188);

/* Walk the tag chain from `p` and hand every node to func_s17_80024188 with a
   zero fourth argument.  21 words, 0 differing, first compile.

   The twin is func_b06_80173E08 two listings up in THIS unit -- the same
   walk with a different per-node call -- and csrc/ovl_dungeon_800D50FC.c's
   func_800D67B0 is the resident original, identical down to the
   `s = (UnkSlot *)n->unk_0C;` cast.  Nothing was fought for; the loop is a
   `for (;;)` with the NULL test in the middle, which is what puts the
   `addiu $s0, $a2, 0x20` in the `beqz`'s delay slot. */
s32 func_s17_80024704(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_s17_80024188(p, v, s, 0);
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80024758);

/* Walk the tag chain from `p` and hand every node to func_s17_80024758 with a
   zero fourth argument.  21 words, 0 differing, first compile.

   The twin is func_b06_80173E08 two listings up in THIS unit -- the same
   walk with a different per-node call -- and csrc/ovl_dungeon_800D50FC.c's
   func_800D67B0 is the resident original, identical down to the
   `s = (UnkSlot *)n->unk_0C;` cast.  Nothing was fought for; the loop is a
   `for (;;)` with the NULL test in the middle, which is what puts the
   `addiu $s0, $a2, 0x20` in the `beqz`'s delay slot. */
s32 func_s17_80024FD4(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_s17_80024758(p, v, s, 0);
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80025028);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80025278);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80025338);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80025470);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80025814);

/* 0x20 bytes of this bank's own rodata, between its jump tables,
   read by func_s17_800259D8. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_s17_80024058[] = {
    0x08,
    0x00,
    0x00,
    0x00,
    0x08,
    0x00,
    0x08,
    0x00,
    0x00,
    0x00,
    0x08,
    0x00,
    0xF8,
    0xFF,
    0x08,
    0x00,
    0xF8,
    0xFF,
    0x00,
    0x00,
    0xF8,
    0xFF,
    0xF8,
    0xFF,
    0x00,
    0x00,
    0xF8,
    0xFF,
    0x08,
    0x00,
    0xF8,
    0xFF,
};

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_800259D8);
