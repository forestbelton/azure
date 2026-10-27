/* func_8003A060 (unk_72A8) -- 43 of 43 words, 2 DIFF. NOT compiled.
 *
 * Written out of the `func_8003A858 + func_8003A060` carved-sweep row on
 * 2026-10-27 so both have a source. THE TWO LISTINGS ARE BYTE-IDENTICAL, so
 * this file and its twin differ only in the function name, and anything found
 * for one settles both.
 *
 * The func_8003A2E0 family: reads the destination index and the table index
 * with `*(*cp)++`, then the four operand bytes off `r = *cp`, advances the
 * cursor by 4, and writes the little-endian word they address into
 * `state[a].unk_48` as FOUR ACCUMULATING STORES -- cse forwards each re-read
 * of the slot, so `*(u32 *) (t + 0x48) += q[k] << n` is the shape, and the
 * listing shows `sw` after every step.
 *
 * WHAT IS PAID FOR, from the landed func_8003A1A4 and this row:
 *
 *   * the table index times 4 in ITS OWN LOCAL right after the read
 *     (`m = **cp * 4;`), inline in the operand sum it is 17 DIFF;
 *   * the destination as `a * 4 + (u32) s`, not `s + a * 4` -- the `addu`
 *     operand order, 8 DIFF the other way;
 *   * `m` written at the END of the operand sum, 11 DIFF at the front;
 *   * the cursor read through `u8 **cp = (u8 **) (p + 0x1C);` with
 *     `*(*cp)++`, which is the family-wide idiom.
 *
 * WHAT IS LEFT: TWO WORDS, one adjacent pair. The target emits
 * `sll $t0, $t0, 2` -- the DESTINATION index -- before the operand chain's
 * last `addu`, this source after it:
 *
 *     target  ... ; sll $t0, $t0, 2 ; addu $a1, $a1, $t1 ; lbu $v1, 0x0($a1)
 *     built   ... ; addu $a1, $a1, $t1 ; sll $t0, $t0, 2 ; lbu ...
 *
 * All 120 permutations of the five statements between the reads were run with
 * perm.py (best 2 DIFF, the same pair), and three more placements of the
 * destination computation were measured here -- before the operand sum, with
 * the state read hoisted beside it, and both after the cursor advance: 7, 7
 * and 2. So the pair is not statement order. Both insns are ALU, both in one
 * priority group, so this is sched1 within priority 1 -- the same family as
 * func_80035E58's last two words and probably as unreachable.
 */

#include "common.h"

void func_8003A060(u8 *p) {
    u8 **cp = (u8 **) (p + 0x1C);
    u32 a;
    u32 m;
    u8 *r;
    u8 *q;
    u8 *s;
    u8 *t;

    a = *(*cp)++;
    m = **cp * 4;
    (*cp)++;
    r = *cp;
    q = (u8 *) (r[0] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24) + m);
    *cp += 4;
    s = *(u8 **) (p + 0x80);
    t = (u8 *) (a * 4 + (u32) s);
    *(u32 *) (t + 0x48) = q[0];
    *(u32 *) (t + 0x48) += q[1] << 8;
    *(u32 *) (t + 0x48) += q[2] << 16;
    *(u32 *) (t + 0x48) += q[3] << 24;
}
