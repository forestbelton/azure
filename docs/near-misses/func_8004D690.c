/* func_8004D690 (unk_1C540) -- 29 of 31 words, 18 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep row on 2026-10-27 so there is a source to
 * sweep; the row had the measurements and no candidate.
 *
 * The retry pump: while the handler slot at `a` is empty, run func_8004D7E8
 * on `c` as long as `b`'s word is nonzero; once the slot is filled, call
 * through it with `a + 4` and stop.
 *
 * EVERY INSTRUCTION IS RIGHT and the function is TWO WORDS SHORT, which is
 * the whole problem. The target's layout is
 *
 *     L6B0:  lw $v0, 0x0($s0) ; beqz $v0 -> L6D0
 *            jalr $v0 (a0 = s0 + 4) ; j L6F0 ; nop      <- falls THROUGH into
 *     L6D0:  lw $v0, 0x0($s1) ; beqz $v0 -> L6F0           the call block, and
 *            jal func_8004D7E8 (a0 = s2) ; j L6B0 ; nop     spends j + nop
 *     L6F0:  epilogue
 *
 * while every spelling of this source inverts the first test (`bnez $v0` to a
 * block at the END) and lets the call block fall into the epilogue, which
 * saves exactly the `j` and its `nop`. gcc is moving the arm that ends the
 * function to the end of the function, and the arm here ends it.
 *
 * MEASURED, all 29 words and 18 DIFF unless noted: `return` and `break` and
 * `goto out` with the label at the end (identical); the two arms as
 * `if (*a == 0) { ... } else { call; return; }`; the second test inverted with
 * `continue` (`if (*b != 0) { func_8004D7E8(c); continue; } return;`); and
 * peeling the first test out of the loop, which is 32 words because
 * cross-jumping does not merge the two call sites back. `while (*a == 0)`
 * with the call AFTER the loop is 30 words and 26 DIFF -- the only spelling
 * that changes the count, and it changes the shape too.
 *
 * So the next thing to look for is NOT another arrangement of these three
 * blocks. It is whatever makes gcc 2.7.2 keep a function-ending arm in place:
 * read `jump_optimize`'s block-moving transformations in
 * scratch/gcc-src/gcc-2.7.2.3/jump.c and find the condition that blocks it.
 * matching-levers has the converse lever (a shared tail that cross-jumping
 * does NOT reach wants a `goto`); this is the same family from the other side.
 */

#include "common.h"

void func_8004D7E8(u8 *);

void func_8004D690(u8 *a, u8 *b, u8 *c) {
    while (1) {
        if (*(u32 *) a != 0) {
            (*(void (**)(u8 *)) a)(a + 4);
            return;
        }
        if (*(u32 *) b == 0) {
            return;
        }
        func_8004D7E8(c);
    }
}
