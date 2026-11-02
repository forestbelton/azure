#include "common.h"

/* 3 DIFF, and the SHAPE is settled -- only an ordering remains.

   One of five members of a family (func_800AEF5C, func_800B0198,
   func_800B1434, func_800B38A8, func_800B6990; 31 words each, 155 together)
   that differ only in which offsets of their argument they touch, so landing
   this lands all five. Each member works at a different offset group of its
   argument -- {0xDC,0xD8,0xBC}, {0xAC,0xA8,0x8C}, {0xD0,0xCC,0xB0},
   {0x9C,0x98,0x7C} -- which is why the argument is a `u8 *` with casts rather
   than a struct: one struct would have to name twenty fields and claim all
   five members are handed the same object, which nothing here witnesses.

   WHAT IS SETTLED. Writing no local for the allocation and letting cse supply
   the register from the store is what keeps the pointer in $v0 across the
   test, which took the park from 7 DIFF to 3. With a `u8 *p` local gcc
   assigns it $a0 -- the register it will need for the call -- and emits the
   copy ahead of the `beqz`, where the target copies after it. The store to
   +0xD8 is unconditional: it sits in the `beqz` delay slot, and a filled MIPS
   delay slot executes on both paths, so it cannot be inside the test.
   func_80049004 and func_800491CC are resident engine functions and their
   prototypes are copied from csrc/unk_1C540.c, not guessed.

   WHAT IS LEFT is three instructions rotated among the same three slots:
   the target emits `addiu $a1, $s0, 0xBC` at w13, `addiu $a2, $zero, 0x1A` at
   w18 and `addiu $s1, $zero, 0x1` at w21; this source emits them w13/w18/w21
   as s1, a1, a2. So `ok = 1` is scheduled FIRST of the three here and LAST
   there. It belongs before the call in source order either way -- the target
   has it at w21 and the `jal` at w22, and moving it after the call would put
   it past the delay slot. That makes this a sched1/local-alloc tie-break
   rather than a source shape, which is the permuter's job and not a sweep's:
   work out the shape by hand, hand the ordering to the permuter. */

u8 *func_80049004(u8 **p, s32 n, s32 m);
void func_800491CC(u32 **p, u32 v, s32 n);

s32 func_800AEF5C(u8 *s, s32 n)
{
    s32 ok;

    ok = 0;
    *(u8 **)(s + 0xD8) = func_80049004((u8 **)(s + 0xDC), n, 0x8);
    if (*(u8 **)(s + 0xD8) != 0)
    {
        *(u32 *)(s + 0xBC) = D_8002E5E8[0];
        *(u32 *)(s + 0xC0) = D_8002E5E8[1];
        *(u32 *)(s + 0xC4) = D_8002E5E8[2];
        ok = 1;
        func_800491CC((u32 **)*(u8 **)(s + 0xD8), (u32)(s + 0xBC), 0x1A);
    }
    return ok;
}
