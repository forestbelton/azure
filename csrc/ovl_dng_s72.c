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

s32 func_800BCE7C(u8 *);
typedef struct UnkDng264D4Vec
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 pad_06[0x2];
} UnkDng264D4Vec;
typedef struct UnkDng264D4SP
{
    u8 pad_00[0x30];
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    u8 pad_3C[0x28];
    s32 unk_64;
    s32 unk_68;
    s32 unk_6C;
    SVECTOR unk_70[4];
    u8 pad_90[0x8];
    UnkDng264D4Vec unk_98[4];
} UnkDng264D4SP;

/* The dungeon's height cast turned into a PICKER: clamp (x, z) onto the
   0x40-square cell grid, read the 6-byte row at D_8008333C[0] and walk that
   cell's 0x18-byte polygon records, keeping the lowest plane above the
   caller's floor -- but RETURN THE WINNING RECORD rather than its height.
   csrc/ovl_dungeon_800B6AC4.c's func_800BCB04 is the line-for-line template
   and all of its measured facts hold here: the record advance is
   `n * 0x18 + 0x18' and not `(n + 1) * 0x18', the bases are INTEGER so every
   `addu' is index-first, and the row address is written out at each of its
   three uses because the `sh' to +0x152 invalidates cse's load of +0x150.
   Two deltas off that twin, both read off this listing: the four grid clamps
   on ix and iy are GONE (the scratchpad's +0x148 is seeded with 0x400 as the
   function's FIRST statement instead of after them), and the result is the
   record pointer `r', seeded NULL and bumped alongside +0x148.

   THE BODY IS NESTED IN `if (cell != 0)' SO THERE IS EXACTLY ONE `return r'.
   Spelled with an early `return r' instead, the body is still 205 words but
   `r' and `norms' hold each other's callee-saved register: 16 aligned
   differences, every row one of that pair. It is global.c's
   `allocno_compare' -- both are 6 refs, `r' over 123 insns and `norms' over
   131, so `r' scores 975 against 916 and takes $s3. Merging the two returns
   drops `r' to 5 refs and 813, and `norms' takes $s3. 205 words,
   0 differing. */
u8 *func_s72_80024064(s32 x, s32 z, s16 c)
{
    u8 *sp = (u8 *)0x1F800000;
    u8 *m = (u8 *)D_8008333C;
    u32 verts;
    u32 cells;
    u32 norms;
    u8 *p;
    u8 *r;
    SVECTOR *n;
    SVECTOR *v;
    u16 px;
    u16 py;

    r = 0;
    *(s32 *)(sp + 0x148) = 0x400;
    *(u16 *)(sp + 0x140) = x & 0x3F;
    *(u16 *)(sp + 0x142) = z & 0x3F;
    *(s32 *)(sp + 0x14C) = c;
    *(u16 *)(sp + 0x150) = (u16)x >> 6;
    verts = *(u32 *)(m + 0x8);
    cells = D_8008333C[0];
    norms = *(u32 *)(m + 0xC);
    *(u16 *)(sp + 0x150) =
        *(u16 *)(sp + 0x150) + (((u16)z >> 6) << *(s16 *)(m + 0x14));
    if (*(u16 *)((s16) * (u16 *)(sp + 0x150) * 6 + cells) != 0)
    {
        *(u16 *)(sp + 0x152) = -*(u16 *)((s16) * (u16 *)(sp + 0x150) * 6 + cells + 2);
        r = *(u8 **)(*(u16 *)((s16) * (u16 *)(sp + 0x150) * 6 + cells) * 4 +
                     *(u32 *)(m + 0x4));
        p = r;
        while (*(s16 *)(*(u16 *)(p + 0x10) * 8 + norms + 4) < -0x7FF)
        {
            if (!(*(u8 *)(p + 0x17) & 1))
            {
                px = *(u16 *)(sp + 0x140);
                py = *(u16 *)(sp + 0x142);
                *(s32 *)(sp + 0x154) = *(s32 *)(*(u16 *)(p + 0x0) * 8 + verts);
                *(u16 *)(sp + 0x154) = *(u16 *)(sp + 0x154) - px;
                *(u16 *)(sp + 0x156) = *(u16 *)(sp + 0x156) - py;
                *(s32 *)(sp + 0x158) = *(s32 *)(*(u16 *)(p + 0x2) * 8 + verts);
                *(u16 *)(sp + 0x158) = *(u16 *)(sp + 0x158) - px;
                *(u16 *)(sp + 0x15A) = *(u16 *)(sp + 0x15A) - py;
                *(s32 *)(sp + 0x15C) = *(s32 *)(*(u16 *)(p + 0x6) * 8 + verts);
                *(u16 *)(sp + 0x15C) = *(u16 *)(sp + 0x15C) - px;
                *(u16 *)(sp + 0x15E) = *(u16 *)(sp + 0x15E) - py;
                *(s32 *)(sp + 0x160) = *(s32 *)(*(u16 *)(p + 0x4) * 8 + verts);
                *(u16 *)(sp + 0x160) = *(u16 *)(sp + 0x160) - px;
                *(u16 *)(sp + 0x162) = *(u16 *)(sp + 0x162) - py;
                if (func_800BCE7C(sp) != 0)
                {
                    n = (SVECTOR *)(*(u16 *)(p + 0x10) * 8 + norms);
                    v = (SVECTOR *)(*(u16 *)(p + 0x0) * 8 + verts);
                    *(s32 *)(sp + 0x144) =
                        (n->vx * (v->vx - (s16) * (u16 *)(sp + 0x140)) +
                         n->vy * (v->vy - (s16) * (u16 *)(sp + 0x142)) + n->vz * v->vz) /
                        n->vz;
                    *(s32 *)(sp + 0x144) =
                        *(s32 *)(sp + 0x144) + (s16) * (u16 *)(sp + 0x152);
                    if (*(s32 *)(sp + 0x144) >= *(s32 *)(sp + 0x14C))
                    {
                        if (*(s32 *)(sp + 0x144) < *(s32 *)(sp + 0x148))
                        {
                            r = p;
                            *(s32 *)(sp + 0x148) = *(s32 *)(sp + 0x144);
                        }
                    }
                }
            }
            if ((*(u8 *)(p + 0x16) & 0xF) == 1)
            {
                if (*(s8 *)(p + 0x17) < 0)
                {
                    break;
                }
                if ((*(u8 *)(p + 0x16) & 0xF0) != 0)
                {
                    p = p + ((*(u8 *)(p + 0x16) >> 4) * 0x18 + 0x18);
                    continue;
                }
            }
            p = p + 0x18;
        }
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_80024398);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_800244BC);

/* The bank's "random value below n": six masked draws from rand,
   picked by the size of n, each retried until it falls under n. The mask and
   shift pairs are read straight off the listing -- 0xC >> 2, 0xF0 >> 4,
   0xFC >> 2, 0xFF0 >> 4, then 0xFFF and 0xFFFF unshifted -- and the six
   bounds are the `slti' constants 5, 0x11, 0x41, 0x101, 0x1001 and the
   fall-through. xsibs found no twin at ratio 0.70 and nothing in the project
   reads rand this way, so this is plain decompilation.

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
                r = (rand() & 0xC) >> 2;
            } while (r >= n);
            return r;
        }
        if (n < 0x11)
        {
            do
            {
                r = (rand() & 0xF0) >> 4;
            } while (r >= n);
            return r;
        }
        if (n < 0x41)
        {
            do
            {
                r = (rand() & 0xFC) >> 2;
            } while (r >= n);
            return r;
        }
        if (n < 0x101)
        {
            do
            {
                r = (rand() & 0xFF0) >> 4;
            } while (r >= n);
            return r;
        }
        if (n < 0x1001)
        {
            do
            {
                r = rand() & 0xFFF;
            } while (r >= n);
            return r;
        }
        do
        {
            r = rand() & 0xFFFF;
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
