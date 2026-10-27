/* func_80048C3C (unk_1AF38) -- 48 of 49 words, 15 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep row on 2026-10-27 so there is a source to
 * sweep; the row had the measurements and no candidate.
 *
 * Reloads one of the D_80071210 pairs: func_8003E4FC on the first word, then
 * through the second a func_8003F80C / DrawSync / func_80047200 /
 * func_8003F80C / func_80046F88 / func_80048B8C sequence, clears D_80080A89
 * and answers the pair's second half.
 *
 * EVERY INSTRUCTION IS RIGHT and the function is ONE WORD SHORT. The tail is
 *
 *     jal  func_80048B8C
 *     addiu $s1, $s1, 0x4        <- the pointer advanced IN PLACE
 *     addu  $v0, $s1, $zero      <- and then copied to the return register
 *
 * where combine folds the pair into a single `addiu $v0, $s1, 0x4` in every
 * spelling tried. For the in-place add to survive, `e` has to be live past
 * the return-value move, and nothing in the tail keeps it so.
 *
 * MEASURED, all 48 words and 15 DIFF: `e++`, `e = e + 1`, `e += 1`,
 * `return &e[0]`, `return (u32 *) e`, and the D_80080A89 store before or
 * after the increment. (The row's own list -- `return e + 1`,
 * `e = &e[1]; return e;` and a `u8 *` base with `e += 4` -- is the same wall
 * from the other side.)
 *
 * WHAT TO TRY NEXT. This is the mirror of section 4's "a re-read that cse
 * turns into a register copy": there the copy survives because the statement
 * writes the SAME pseudo in place and so has two sets. Here the in-place add
 * gives `e` two sets already and combine still folds the following move, so
 * the two-sets rule is about substituting INTO an insn, not out of one. The
 * thing that blocks this fold is a second USE of `e` after the add -- so look
 * for one the listing can afford, or read combine.c's `try_combine` for the
 * condition on a single-use destination (it is `REG_N_SETS == 1` plus the
 * REG_DEAD note on the link, and the dead note is what this tail has).
 */

#include "common.h"

extern u32 D_80071210[];

void func_8003F320(void);
void func_8003F80C(u32, s32, s32, s16);
void func_80047200(u32 *, s32, s32);
void func_80046F88(u8 *);
void func_80048B8C(u8 *);

u32 *func_80048C3C(s32 i) {
    u32 *e;
    u8 *q;
    u8 *r;

    e = (u32 *) ((u8 *) D_80071210 + i * 8);
    func_8003E4FC(6, (u8 *) e[0], 0);
    func_8003F320();
    q = (u8 *) e[1];
    r = q + *(u32 *) (q + 0x1C);
    func_8003F80C((u32) r, 0x7A00, *(u32 *) (q + 0x20), 2);
    DrawSync(0);
    func_80047200((u32 *) r, 1, 1);
    func_8003F80C((u32) r, 0x7980, *(u32 *) (q + 0x20), 2);
    func_80046F88(q);
    func_80048B8C(q);
    e++;
    D_80080A89 = 0;
    return e;
}
