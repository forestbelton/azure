/* func_80047054 (unk_14934) -- 58 words against 58, every opcode, immediate,
 * displacement and nop identical. The only difference is a permutation of five
 * registers, and it follows from one decision.
 *
 * The target keeps the first parameter in $a0 for all four of its loads and
 * puts the outer walker in $v1:
 *
 *     p = $a0   e = $v1   outer giv = $t2   n = $t1   n+6 giv = $t0
 *
 * This source copies the parameter out (`move $t2, $a0` in the prologue) and
 * gives the walker $a0:
 *
 *     p = $t2   e = $a0   outer giv = $t1   n = $t0   n+6 giv = $v1
 *
 * Everything else is a consequence: with $a0 taken by `e`, $v1 is free and the
 * highest-priority allocno (the n+6 giv) takes it instead of $t0, and the rest
 * shift down.
 *
 * From the .greg dump the allocation order is `122 95 96 124 84 80 81 82 83 94`
 * -- the n+6 giv (26 refs / 22 insns), n (23/28), the outer giv (9/36), then
 * `e` (84, 9 refs / 40 insns) and only then `p` (80, 6 refs / 88 insns). global.c
 * ranks by roughly log2(refs) * refs / live_length, so `e` at ~0.68 beats `p` at
 * ~0.14 and is allocated first; it also carries `;; 84 preferences: 4`, a
 * preference for $a0, which `p` does not have. For the target's assignment `e`
 * would have to be allocated after the n+6 giv (a gap of 0.68 against 4.7, far
 * too wide to close by tweaking ref counts) or else conflict with $v1.
 *
 * Ruled out, all giving the same 30-register-difference output: `u32 *p` with
 * `(u32) e < p[2]`; a struct pointer with `u8 *` fields and a pointer compare;
 * `while` instead of `for`; the walker initialised in two statements (worse --
 * 58 diffs, it loses the combined giv).
 *
 * Worth keeping even so: the inner `u8 f;` snapshot of the flags before the
 * `n += 0xC` is what produces the target's `lbu` / `addiu` / `beqz` tail, and
 * writing the halfword updates as `*(u16 *) (n + 4)` and `*(u16 *) (n + 6)` off
 * a walking byte pointer is what makes gcc's combine_givs fold them into one
 * induction variable at +6 with the other access as `-0x2` off it.
 */

#include "common.h"

void func_80047054(u32 *p, s32 arg1, s32 arg2, s32 arg3) {
    u8 *e;

    for (e = (u8 *) (p[0] + p[1] * 4); (u32) e < p[2]; e += 8) {
        if (*(s16 *) (e + 2) == 2) {
            u8 *n = *(u8 **) (e + 4);

            if (!(*n & 8)) {
                u8 f;

                *n |= 8;
                do {
                    if (*n & 0x40) {
                        *(u16 *) (n + 4) += arg1;
                    } else {
                        *(u16 *) (n + 4) += arg3;
                    }
                    if (!(*n & 0x20)) {
                        *(u16 *) (n + 6) += arg2;
                    }
                    f = *n;
                    n += 0xC;
                } while (!(f & 0x80));
            }
        }
    }
}
