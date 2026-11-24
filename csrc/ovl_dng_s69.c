#include "common.h"

s32 func_8004491C(UnkNode *, u32);
UnkNode *func_s69_800249DC(UnkVec *b);
UnkNode *func_8003FC64(s32);
s32 func_800CEEFC();
extern u8 D_80027460[];
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
void func_s69_800245A0(u8 *, UnkVec *, u8 *, u8 *);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80024020);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80024328);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_800244C4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_800245A0);

/* The nine-node burst func_s56_80024388 fires: allocate nine 0x2 nodes, give
   each func_s69_800245A0 as its step and D_80027460 as its slot's +0x8
   record, copy the caller's mover position into both the new mover's position
   and velocity and into the payload's +0x38 triple, scale the last node 0x800
   and the rest 0x200, ramp the grey down by 4 per node and return the last
   node allocated. No xsibs twin; the spawner skeleton is
   csrc/ovl_dng_s21.c's func_s21_8002495C (same func_8003FC64 /
   `n->unk_10 = step' / func_8004491C(n, (u32)func_800CEEFC) order).

   Three spellings were measured here, and each is worth real rows:

     * `q = (u8 *)n + 0x20' must be computed AFTER both calls. Before them it
       is live across a call, takes a SEVENTH callee-saved register and the
       body is 74 words / 63 differing.
     * the 0x200-vs-0x800 scale is an `if'/`else' with the `sh' DUPLICATED PER
       ARM, which cross-jumping merges back. A value in a temp collapses to a
       pre-loaded 0x800 plus a conditional overwrite: 72 words / 45 differing.
     * each position group is ONE CHAINED assignment,
       `*(q + 0x38) = *(m + 0xE) = *(m + 0x2) = *(u16 *)(b + 0x2)', emitted
       innermost-first. A `u16' local holding the loaded halfword gives the
       same one `lhu' but swaps $v0 and $v1 between `m' and the value:
       13 differing with every opcode right.

   73 words, 0 differing.
   THIS COPY'S OWN CONSTANT: 0x2 -> 0x12 at 1 of 4 sites, 0x38 -> 0x1c, 0x3a -> 0x1e, 0x3c -> 0x20, 0xc -> 0xd, 0x54 -> 0x38 at all 2 sites, 0x4c -> 0x30.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
UnkNode *func_s69_800249DC(UnkVec *b)
{
    UnkNode *n;
    u8 *m;
    u8 *r;
    u8 *q;
    s32 i;

    for (i = 0; i < 9; i++)
    {
        n = func_8003FC64(18);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s69_800245A0;
            func_8004491C(n, (u32)func_800CEEFC);
            m = (u8 *)n->unk_08;
            q = (u8 *)n + 0x20;
            *(s16 *)(q + 0x1c) = *(s16 *)(m + 0xE) = *(s16 *)(m + 0x2) =
                *(u16 *)((u8 *)b + 0x2);
            *(s16 *)(q + 0x1e) = *(s16 *)(m + 0x12) = *(s16 *)(m + 0x6) =
                *(u16 *)((u8 *)b + 0x6);
            *(s16 *)(q + 0x20) = *(s16 *)(m + 0x16) = *(s16 *)(m + 0xA) =
                *(u16 *)((u8 *)b + 0xA);
            r = (u8 *)n->unk_0C;
            *(u32 *)(r + 0x8) = (u32)D_80027460;
            if (i != 8)
            {
                *(s16 *)(r + 0x1E) = 0x200;
            }
            else
            {
                *(s16 *)(r + 0x1E) = 0x800;
            }
            r[0xd] = -0x80 - ((8 - i) << 2);
            *(s16 *)(r + 0x1C) = *(u16 *)(r + 0x1E);
            *(s16 *)(q + 0x38) = i;
            if (i == 8)
            {
                *(s16 *)(q + 0x38) = 7;
            }
            *(s16 *)(q + 0x30) = 8;
        }
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80024B00);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80024C38);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80024E60);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80024EF8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_8002501C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_800255B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_800257D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80025840);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_8002590C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_8002593C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80025A34);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80025AAC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80025B78);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80025D28);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_80025FCC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_800260D4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s69", func_s69_8002614C);
