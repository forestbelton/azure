/* func_8004A574 (unk_1C540) -- 41 of 41 words, 5 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep row on 2026-10-27 so there is a source to
 * sweep.
 *
 * Scales a halfword of `D_80073414[p[1]].unk_0C + p[0] * 20` -- +0x10 or
 * +0x12 by the second argument -- by `D_800713CC[e[3]]` and passes it to the
 * function pointer at `e + 8`, if that is non-null. Both index chains are
 * `x * 20` written `(x * 4 + x) * 4`, which is what gcc emits for the
 * multiply; the struct is 20 bytes.
 *
 * THE ARM ORDER IS `w == 0` FIRST, worth three words: the listing's
 * `bnez $a1, .L8004A5E0` branches to the +0x12 arm and falls through into the
 * +0x10 one, so the `w == 0` body is the fall-through and must be written
 * first. As `if (w != 0) { +0x12 } else { +0x10 }` the test comes out `beqz`
 * and the two arms swap: 8 DIFF.
 *
 * WHAT IS LEFT: FIVE WORDS, and they are the `mflo`'s position with the copy
 * that follows from it. The target has
 *
 *     mflo  $v0                      <- in the JOIN block
 *     lw    $v1, 0x8($a2)
 *     beqz  $v1, .L8004A608
 *      addu $a0, $v0, $zero          <- the copy, in the delay slot
 *     jalr  $v1
 *
 * where this source emits the function-pointer load first and sinks the
 * `mflo` past the branch into the call block, straight into $a0, so there is
 * no copy to make:
 *
 *     lw    $v1, 0x8($a2) ; nop ; beqz $v1 ; nop ; mflo $a0 ; jalr
 *
 * Measured and not working: the function pointer read into a local AFTER the
 * if/else (identical), a second `u = v` local for the argument (identical),
 * and the row's own list -- both arms as if/else and as a ternary, the
 * operands of the multiply swapped (7), the halfword into its own local with
 * one multiply after the `if` (17), the multiply split from the load (12),
 * the pointer hoisted into a local before the test, an early `return` instead
 * of a guarded block, an `s32` product and a temporary for the argument (all
 * 5), plus a full search.py sweep of 18 variants.
 *
 * So the question is only what pins the `mflo` above the function-pointer
 * load. Both are in the join block; nothing in the source orders them,
 * because the multiply's result and the load are independent. Read the sched1
 * dump (tools/decomp/sched.py) for the join block and see which of the two
 * the ready list prefers -- this is the same "memory over ALU inside one
 * priority group" family as func_80035E58's last two words, and if that is
 * what it is, the answer is that it is not reachable from the source.
 */

#include "common.h"

extern s32 D_800713CC[];

void func_8004A574(u8 *p, s32 w) {
    u8 *e;
    u8 *r;
    s32 s;
    s32 v;

    e = (u8 *) D_80073414 + p[1] * 20;
    r = (u8 *) *(u32 *) (e + 0xC) + p[0] * 20;
    s = D_800713CC[e[3]];
    if (w == 0) {
        v = *(u16 *) (r + 0x10) * s;
    } else {
        v = *(u16 *) (r + 0x12) * s;
    }
    if (*(u32 *) (e + 8) != 0) {
        (*(void (**)(s32, u8 *)) (e + 8))(v, p);
    }
}
