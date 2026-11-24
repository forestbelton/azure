#include "common.h"

UnkNode *func_8003FC64(s32);
extern u8 D_800269FC[];
extern u8 D_800269D4[];
extern u8 D_800DF31C[];
void func_800B835C(u8 *, s32 *, s32, s32);
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
void func_s62_8002432C(u8 *p, UnkVec *b, u8 *c, u8 *d);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_80024064);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_8002432C);

/* The 0x202 tint node's spawner: run func_800B835C over {0x1000340,
   0x200020} on D_800DF31C and then over {0x1000360, 0x200020} on
   D_800269D4, park func_s62_8002432C as the new node's step, seed its mover
   from the caller's, point its slot at D_800269FC with a 0xC00 scale, stash
   the owning node at the payload's +0xC and return the payload.
   csrc/ovl_dungeon_800C2824.c's func_800C4AFC is the template: same stack
   `s32 arr[2]', same `func_800B835C(..., arr, 1, 0)', same mover copy. Three
   deltas, all off this listing: func_8003FC64(2) in place of
   func_8003FD64(0x202, D_80083498), TWO func_800B835C calls (the second
   rewrites arr[0] and arr[1], which is the second `sw 0x14($sp)'), and no
   payload pointer -- the node is reached at +0x10 and +0x2C directly and the
   return is `(n != NULL) ? (u8 *)n + 0x20 : NULL'.
   67 words, 0 differing, first compile.
   THIS COPY'S OWN CONSTANT: 0x1000360 -> 0x1200340, 0x2c -> 0x28.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
u8 *func_s62_800244EC(u8 *m, u8 *act)
{
    UnkNode *n;
    UnkVec *v;
    u8 *r;
    s32 arr[2];

    n = func_8003FC64(2);
    if (n != NULL)
    {
        arr[0] = 0x1000340;
        arr[1] = 0x200020;
        func_800B835C(D_800DF31C, arr, 1, 0);
        arr[0] = 0x1200340;
        arr[1] = 0x200020;
        func_800B835C(D_800269D4, arr, 1, 0);
        v = n->unk_08;
        n->unk_10 = (u32)func_s62_8002432C;
        *(s16 *)((u8 *)v + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)v + 0x6) = *(u16 *)(m + 0x6);
        *(s16 *)((u8 *)v + 0xA) = *(u16 *)(m + 0xA);
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0x8) = (u32)D_800269FC;
        *(s16 *)(r + 0x1E) = 0xC00;
        *(s16 *)(r + 0x1C) = 0xC00;
        *(u8 **)((u8 *)n + 0x28) = act - 0x20;
    }
    return (n != NULL) ? (u8 *)n + 0x20 : NULL;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_800245F8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_8002470C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_80024888);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_80024938);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_80024AB4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_80024B2C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_80024DAC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_80025F54);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_80026240);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_800262B0);

/* A renderable's facing step: point the slot's +0x00 element at its sheet's
   +0xC plus 22 bytes per frame of the signed +0x04 index, step that index by
   one towards the sign of *b, then clamp it into [lo, hi] -- raising the
   slot's 0x4000 flag when it was clamped and clearing it when it was not.
   The three clamp arms share the one `sh 0x14($t0)' because jump2
   cross-jumps them. No twin anywhere: dngtwin finds nothing above 0.62
   landed, and the resident units have nothing at this shape.
   51 words, 0 differing, FIRST COMPILE.

   LOAD-BEARING, measured: the `lo' test must be the FIRST arm. With the
   `hi' test first it is 51 words / 11 differing. Measured and FREE at 51
   words / 0 differing, so neither is why this lands: the 0x4000 flag store
   written before the clamp store in each arm rather than after, and the
   whole body written through `UnkSlot *s' field accesses (unk_00/unk_04/
   unk_08/unk_14) instead of `u8 *s' with casts -- the slot type is
   witnessed here by +0x04 as a signed byte, +0x08 as a pointer and +0x14 as
   the flag halfword, and it measures identically either way. */
void func_s62_800262F4(u8 *s, s16 *b, s16 lo, s16 hi)
{
    *(s32 *)s = *(s32 *)(*(u8 **)(s + 0x8) + 0xC) + *(s8 *)(s + 0x4) * 22;
    if (*b > 0)
    {
        *(s8 *)(s + 0x4) = *(s8 *)(s + 0x4) + 1;
    }
    else if (*b < 0)
    {
        *(s8 *)(s + 0x4) = *(s8 *)(s + 0x4) - 1;
    }
    if (*(s8 *)(s + 0x4) < lo)
    {
        *(s8 *)(s + 0x4) = lo;
        *(u16 *)(s + 0x14) |= 0x4000;
    }
    else if (hi < *(s8 *)(s + 0x4))
    {
        *(s8 *)(s + 0x4) = hi;
        *(u16 *)(s + 0x14) |= 0x4000;
    }
    else
    {
        *(u16 *)(s + 0x14) &= 0xBFFF;
    }
}

/* Point the slot's +0x00 element at its sheet's +0xC plus 22 bytes per frame
   of `d' -- func_s69_80025840's head without the step or the clamp.
   12 words, 0 differing, FIRST COMPILE.

   LOAD-BEARING, measured: `d' must be an `s16' PARAMETER. As `s32' the
   `sll'/`sra' promotion pair disappears and the body is 10 words /
   12 differing. Measured and FREE at 12 words / 0 differing: `d * 11 * 2'
   in place of `d * 22' -- fold reassociates it to the same
   sll/addu/sll/subu/sll chain. */
void func_s62_800263C0(u8 *s, s16 d)
{
    *(s32 *)s = *(s32 *)(*(u8 **)(s + 0x8) + 0xC) + d * 22;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_800263F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_80026444);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_8002654C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_80026694);
