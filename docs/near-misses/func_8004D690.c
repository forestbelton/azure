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
 * THE TRANSFORMATION IS FOUND, 2026-10-28: `jump.c:1800`, the one whose comment
 * reads "Look for   if (foo) bar; else break;". It matches exactly this
 * function's layout --
 *
 *     insn = condjump label1;      <- `if (*(u32 *) a != 0)`
 *     ...range1...                 <- the call block
 *     jump label2;                 <- its `return`
 *     label1:
 *     ...range2...                 <- the pump
 *     jump somewhere unconditionally   <- the `while (1)` back edge
 *     label2:
 *
 * -- and it INVERTS the condition and SWAPS the two ranges, which is precisely
 * the difference: the arm that ends the function moves to the end and its `j`
 * and `nop` are no longer needed. The five conditions it needs, from the
 * source, are the list of things that would block it:
 *
 *   1. `! first` -- it is skipped on jump_optimize's FIRST round, and
 *      `(reload_completed ? ! flag_delayed_branch : 1)` means it is skipped
 *      after reload too. So it fires in a middle round and nothing about
 *      -O or delay slots turns it off.
 *   2. `condjump_p (insn)` and `JUMP_LABEL (insn) == label1`.
 *   3. **`LABEL_NUSES (label1) == 1`** -- the cheapest-looking one to break: a
 *      SECOND reference to the pump's own label stops it. Nothing in this
 *      source gives it one (the back edge targets the loop top, not the pump),
 *      and buying one needs a branch, which costs the words it saves.
 *   4. `simplejump_p (range1end)` -- the call arm's `return` has to be a `j`
 *      to the shared epilogue. It is, in both builds.
 *   5. **`GET_CODE (range2end) == JUMP_INSN && GET_CODE (NEXT_INSN (range2end))
 *      == BARRIER`** -- the pump has to end in an UNCONDITIONAL jump. A
 *      conditional back edge is a JUMP_INSN but is NOT followed by a barrier,
 *      so a loop whose back edge is conditional does not match. `while (1)`
 *      and `for (;;)` both give the unconditional form; a real condition costs
 *      the re-read.
 *
 * So the two openings are a second use of the pump's label or a conditional
 * back edge, and both cost a word in this source. That is a sharper statement
 * than "find the condition", and it says the answer is probably not in the
 * loop's spelling at all.
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
