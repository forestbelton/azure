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
void func_s72_80024BF4(u8 *p, UnkVec *v, UnkSlot *s, s16 d);

s32 func_80069EF8(void);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_80024064);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_80024398);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_800244BC);

/* The bank's "random value below n": six masked draws from func_80069EF8,
   picked by the size of n, each retried until it falls under n. The mask and
   shift pairs are read straight off the listing -- 0xC >> 2, 0xF0 >> 4,
   0xFC >> 2, 0xFF0 >> 4, then 0xFFF and 0xFFFF unshifted -- and the six
   bounds are the `slti' constants 5, 0x11, 0x41, 0x101, 0x1001 and the
   fall-through. xsibs found no twin at ratio 0.70 and nothing in the project
   reads func_80069EF8 this way, so this is plain decompilation.

   Two shape facts read off the listing rather than guessed. The n == 0 test
   shares its `addu $v0, $v1, $zero' with the LAST arm's fall-through at
   .Lb25_80171768, so `r = 0' is initialised before the test and the last
   `do' loop has NO `return' of its own -- the five earlier arms do, which is
   what puts the return-value copy in each of their `beqz' delay slots and a
   bare `nop' in the following `j'. The retry test is a signed `slt', so n and
   r are both s32.

   73 words, 0 differing, on the first compile; no alternative spelling was
   measured, so nothing here is claimed to be load-bearing. No jump table and
   no rodata of its own -- the listing opens at `glabel'. */
s32 func_s72_8002458C(s32 n)
{
    s32 r;

    r = 0;
    if (n != 0)
    {
        if (n < 5)
        {
            do
            {
                r = (func_80069EF8() & 0xC) >> 2;
            } while (r >= n);
            return r;
        }
        if (n < 0x11)
        {
            do
            {
                r = (func_80069EF8() & 0xF0) >> 4;
            } while (r >= n);
            return r;
        }
        if (n < 0x41)
        {
            do
            {
                r = (func_80069EF8() & 0xFC) >> 2;
            } while (r >= n);
            return r;
        }
        if (n < 0x101)
        {
            do
            {
                r = (func_80069EF8() & 0xFF0) >> 4;
            } while (r >= n);
            return r;
        }
        if (n < 0x1001)
        {
            do
            {
                r = func_80069EF8() & 0xFFF;
            } while (r >= n);
            return r;
        }
        do
        {
            r = func_80069EF8() & 0xFFFF;
        } while (r >= n);
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_800246B0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_800247C0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_80024AE8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_80024BA0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_80024BF4);

/* Walk the tag chain from `p` and hand every node to func_s72_80024BF4 with a
   zero fourth argument.  21 words, 0 differing, first compile.

   The twin is func_b06_80173E08 two listings up in THIS unit -- the same
   walk with a different per-node call -- and csrc/ovl_dungeon_800D50FC.c's
   func_800D67B0 is the resident original, identical down to the
   `s = (UnkSlot *)n->unk_0C;` cast.  Nothing was fought for; the loop is a
   `for (;;)` with the NULL test in the middle, which is what puts the
   `addiu $s0, $a2, 0x20` in the `beqz`'s delay slot. */
s32 func_s72_80025034(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_s72_80024BF4(p, v, s, 0);
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_80025088);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_800251F4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_80025648);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_80025710);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_80025800);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_80025CE8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_80025FB0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_80026060);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_8002612C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_8002615C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_8002626C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_80026418);
