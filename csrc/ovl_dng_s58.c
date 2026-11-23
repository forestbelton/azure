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
s32 func_80069EF8();
void func_s58_80024374();
s32 func_s58_80024124(u8 *p, UnkVec *v);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s58", func_s58_8002407C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s58", func_s58_80024124);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s58", func_s58_80024374);

/* A 0x211 spawner: seed the new node's mover from the parent `a`'s, jitter
   each axis by a fresh func_80069EF8() & 0x1F and the caller's own offset
   less 0x10, then park func_s58_80024374 as the step and
   func_s58_80024124 as the draw, with `b` at the payload's +0x14, `d` at
   +0x32 and `c` at +0x8.  72 words, 0 differing.  The twin is
   csrc/ovl_dungeon_800D50FC.c's func_800D6AD4 -- the same 0x211 spawner
   without the jitter.

   THE THREE STACK OFFSETS ARE `s16` AND THE SUBTRACTION WEARS AN `(s16)`
   CAST, and both halves of that are load-bearing:

   * `s32 dx/dy/dz` is 66 words and 71 differing.  An `s32` stack parameter
     keeps its incoming MEM as its rtl and is rematerialised with a
     `lw 0x48($sp)` at its single use; the promoted `s16` goes through a
     pseudo, which register allocation then parks in $s2/$s3/$s4.  The six
     missing words are those three registers' save/restore pairs.  (Same
     mechanism, opposite sign, as func_800D6AD4's own note.)
   * `s16` with a bare `(dx - 0x10)` is 74 words and 20 aligned differences:
     fold's associate step splits `(X + jitter) + (dx + -0x10)` into
     `(X + jitter + dx) + -0x10` and the `addiu` lands on the running sum
     instead of on the parameter.  The NOP_EXPR the cast leaves blocks that
     split, which is the whole of the remaining 20 rows -- with it the
     `andi` falls into the `lhu`'s load-delay slot and the destination's
     `lw 0x8($s0)` moves ahead of the first `addu` on its own.
   THIS COPY'S OWN CONSTANT: 0x1f -> 0xf at 2 of 3 sites, -0x10 -> -0x8 at 2 of 3 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s58_800243C4(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s58_80024374;
        *(s16 *)((u8 *)n->unk_08 + 0x2) =
            *(u16 *)((u8 *)a->unk_08 + 0x2) + (func_80069EF8() & 0xf) + (s16)(dx - 0x8);
        *(s16 *)((u8 *)n->unk_08 + 0x6) =
            *(u16 *)((u8 *)a->unk_08 + 0x6) + (func_80069EF8() & 0xf) + (s16)(dy - 0x8);
        *(s16 *)((u8 *)n->unk_08 + 0xA) =
            *(u16 *)((u8 *)a->unk_08 + 0xA) + (func_80069EF8() & 0x1F) + (s16)(dz - 0x10);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = d;
        func_8004491C(n, (u32)func_s58_80024124);
        *(u32 *)(p + 0x8) = c;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s58", func_s58_800244E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s58", func_s58_80024874);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s58", func_s58_80024A5C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s58", func_s58_80024AF8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s58", func_s58_80024CA4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s58", func_s58_80024F80);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s58", func_s58_8002525C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s58", func_s58_8002553C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s58", func_s58_80025648);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s58", func_s58_80025724);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s58", func_s58_800257B0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s58", func_s58_80025A80);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s58", func_s58_80025D68);
