/* func_8005B070 (unk_1C540) -- 63 of 63 words, 4 DIFF. NOT compiled.
 *
 * Walks the D_80085458 entry table and submits one primitive per live entry
 * whose +0x06 index is below 0x10 and whose +0x1A is set, then marks
 * D_80086C00[n] and clears two flags. Called from func_800553D4's 0xE1 case.
 *
 * WHAT IS PAID FOR -- 67 words on the first compile, 63 now
 *
 * All three are the D_80085458 loop recipe that csrc/unk_1C540.c's
 * func_80056558 already documents, and getting any of them wrong costs TWO
 * words each because loop.c hoists the invariant into a callee-saved register
 * and the save/restore pair comes with it:
 *
 * * the entry pointer is computed INSIDE the body (`u8 *e = D_80085458 +
 *   i * 0x78;`). Walked from a pointer declared outside, loop.c biases the
 *   giv to +0x1A -- the first field used -- and the two loads come out as
 *   `lhu -20($s0)` and `lhu 0($s0)`.
 *
 * * the bound is read in the loop CONDITION, not into a local.
 *
 * * the bound uses the INCOMPLETE ARRAY spelling, `(s32) D_80073734[0]`, not
 *   the `__asm__` scalar alias. That is the opposite of func_80056558, where
 *   the scalar is what matches: here the listing has `lui $v0` / `lw $v1,
 *   %lo(...)($v0)` twice, the compiler-split form with a separate address
 *   temp, where the scalar gives the assembler macro's one-register form. Six
 *   of the ten words that were left came from this one choice, including the
 *   $a0/$v0 swap at the top of the function. So the spelling is per-site
 *   evidence, not a project-wide rule -- read the `lui`'s destination.
 *
 * The 0x40-byte stack buffer is only four fields wide (+0x00, +0x04, +0x08,
 * +0x0A) but the frame is 0x68 with five saved slots, which leaves 0x40.
 *
 * WHAT IS LEFT: one scheduling slot, three insns and a register.
 *
 *     target               this
 *     addiu $v0, 3         addiu $v0, 3
 *     sw    $v0, 0x14(sp)  lw    $v1, 0x0($s2)
 *     lw    $v0, 0x0($s2)  addiu $a0, $sp, 0x10
 *     addiu $a0, $sp, 0x10 sw    $v0, 0x14(sp)
 *
 * The target stores the 3 and then loads D_80073740[i] into the register the 3
 * vacated; this source issues the load two slots earlier and so needs a second
 * register for it. Measured and NOT moving it: the load in its own statement
 * right after the 3's store; the four buf stores in three other orders; the
 * buffer as a STRUCT so its stores carry MEM_IN_STRUCT_P (section 5's alias
 * rule would make the varying-address load dependent on them); and
 * `*(volatile s32 *)` on the 3's store, which nothing should be able to cross.
 * That last one not working says the reorder is NOT sched1 -- it is the fill
 * pass cc1psx runs afterwards, the one carved-sweep notes "swaps a load with
 * the next insn". If that is right, no source order can reach it and the
 * question is what makes the fill pass leave the pair alone.
 */

#include "common.h"

void func_8005F134(void *);

void func_8005B070(s16 n)
{
    u8 buf[0x40];
    s32 i = 0;

    D_80085F98[0] = 1;
    while (i < (s32)D_80073734[0])
    {
        u8 *e = D_80085458 + i * 0x78;

        if (*(u16 *)(e + 6) < 0x10 && *(u16 *)(e + 0x1A) != 0)
        {
            *(s32 *)(buf + 4) = 3;
            *(s16 *)(buf + 8) = 0;
            *(s16 *)(buf + 0xA) = 0;
            *(u32 *)buf = D_80073740[i];
            func_8005F134(buf);
        }
        i++;
    }
    D_80086C00[n].unk_02 = 4;
    D_80085F98[0] = 0;
    D_800737A4[0] = 0;
}
