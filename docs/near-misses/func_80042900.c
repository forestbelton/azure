/* func_80042900 (unk_14934) -- 1 instruction off, 34 words against 33.
 *
 * The loop, both early returns, the re-sign-extension of arg1 after the loop
 * and the 0xE arm are all byte-exact. What is left is the 0xD arm.
 *
 * The target's 0xD arm is a single `lw $v1, 0x14($a0)` whose result is never
 * used: the function returns 0 on that path, from the `addu $v0, $zero, $zero`
 * that reorg hoisted into the `bne`'s delay slot. So the original performed a
 * load for its own sake and discarded it.
 *
 * A non-volatile dead load is deleted outright by flow.c's insn_dead_p (tested:
 * the whole arm vanishes), so the read has to be volatile. With volatile, gcc
 * keeps the load but gives it $v0 -- the first register in REG_ALLOC_ORDER,
 * free because the return value's `$v0 = 0` is only set afterwards -- and then
 * re-emits `move $v0, zero` after it. That extra move is the one instruction.
 *
 * To match, the load's pseudo has to conflict with $v0, i.e. the return value
 * has to be live across it. Ruled out, all still giving `lw $v0` + the extra
 * move: `s32 r = 0;` at the top of the function with `return r;` at the bottom;
 * the same with `r = 0;` moved to just before the 0xD test; a block-local
 * `s32 r = 0;` set before the volatile read inside the arm; assigning the read
 * to a dead local; `volatile s32 *` instead of `volatile u32 *`.
 *
 * Reusing one `r` for the loop's loaded byte and the 0xD arm does put the load
 * in $v1 -- one allocno, one hard register -- but writing the loop that way
 * (`r = arg0[i * 2 + 0x2D]; if (r != 0) return r;`) flips the inner branch arms
 * and costs more than it saves (9 diffs). Something that gives the dead load
 * $v1 while leaving the loop's two array references in place is what is
 * missing.
 */

#include "common.h"

s32 func_80042900(s8 *arg0, s8 arg1) {
    s32 i;

    if (arg1 != 0) {
        for (i = 3; i >= 0; i--) {
            if (arg0[i * 2 + 0x2C] == arg1) {
                if (arg0[i * 2 + 0x2D] != 0) {
                    return arg0[i * 2 + 0x2D];
                }
                return 0x100;
            }
        }
    }
    if (arg1 == 0xE) {
        return ((u8 *) arg0)[0x28] == 0;
    }
    if (arg1 == 0xD) {
        *(volatile u32 *) (arg0 + 0x14);
    }
    return 0;
}
