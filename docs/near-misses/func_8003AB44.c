/* func_8003AB44 (unk_72A8) -- 54 of 54 words, 4 DIFF. NOT compiled.
 *
 * Moves a VRAM rectangle: x = arg5 + arg2 * D_8006A8C8[arg4], and if that is
 * in [0x140, 0x400) it fills a RECT from func_8004D880/func_8004D91C and
 * MoveImages it to (x, arg6 + arg3 * D_8006A8DC[arg4]).
 *
 * It was 52 of 54 until 2026-10-26, two words short of the target's extra
 * callee-saved register. **That was argument 6 declared `s32` with an
 * `(s16)` cast at the use** -- see the narrow-parameter entry in
 * matching-levers section 7, which this function and func_8004A24C are the
 * two measurements behind. Declaring it `s16` restores the `lw $s3` at entry
 * and the `sll`/`sra` at the use, and the word count is now exact.
 *
 * NOTE THE COMMITTED PROTOTYPE IS STILL WRONG. csrc/unk_72A8.c declares
 * `void func_8003AB44(u8 *, s16, s32, u8, s16, s32, s32);` and func_8003AAC4
 * (landed, matching) calls it. Landing this source means narrowing argument 6
 * to `s16` there, which changes the CALLER's argument setup -- check
 * func_8003AAC4 still matches, or give this function its own declaration
 * through an `__asm__` alias (carved-sweep has the idiom for data; it works
 * on a function declarator too).
 *
 * THE 3-CYCLE IS SOLVED, 2026-10-28, 9 DIFF -> 4. The rotation was
 *
 *     built  s1 = arg4*4  s2 = the 0x44 stack arg  s3 = arg3  (s0 = x)
 *     target s2 = arg4*4  s3 = the 0x44 stack arg  s1 = arg3  (s0 = x)
 *
 * i.e. `arg3` had to move from LAST in global-alloc's order to SECOND, with
 * `x` staying first. `advise.py --greg` priced the four at 0.136 (x), 0.125
 * (arg4*4), 0.100 (the stack arg) and 0.067 (arg3), so arg3 had to jump two
 * places without passing x. Two free references did it:
 *
 *   1. ARG3 GETS A SECOND SET: `c = (s16) c * D_8006A8DC[d];` with the call
 *      then reading `f + c`. Same instructions -- the multiply happens either
 *      way -- but `reg_n_sets` goes to 2, which ESCAPES `update_equiv_regs`'
 *      live-length DOUBLING (local-alloc.c:1018-1064, the audit note in
 *      section 3) as well as adding references. That alone is 9 -> 7, but it
 *      OVERSHOOTS: arg3 goes to 0.381, first, and takes $s0 from x.
 *   2. SO X GETS TWO FREE REFERENCES TOO, by the split that costs no insn:
 *      `x = b * D_8006A8C8[d]; x += e;`. 7 -> 4, and the four callee-saved
 *      registers are now exactly the listing's.
 *
 * ORDER MATTERS IN BOTH SPLITS. `x = e; x += b * D_8006A8C8[d];` is 5 DIFF
 * rather than 4, because then `e` shares x's register instead of staying in a
 * temp; and `c = (s16) c;` as its own statement, with the multiply left in the
 * call's argument, is 8 (the cast's result lands in arg3's callee-saved
 * register where the listing keeps it in $v1).
 *
 * WHAT IS LEFT: FOUR WORDS, two pairs, and both are the same artefact of the
 * levers above -- each product is computed INTO a callee-saved register where
 * the listing computes it into a temp:
 *
 *     built  mflo $s0 ; addu $s0, $s0, $v0      target  mflo $t0 ; addu $s0, $v0, $t0
 *     built  mflo $s1 ; addu $a2, $a2, $s1      target  mflo $t0 ; addu $a2, $a2, $t0
 *
 * So the listing's own source assigns NEITHER product to the variable that
 * holds the register -- which means its arg3 has two references and one set,
 * is doubled like this file's was, and still outranks arg4*4. Something other
 * than the printed priority is deciding that pair there, and `regs_someone_
 * prefers` is the first thing to check: if arg3 prefers $s1, the two
 * higher-priority allocnos avoid it and leave it free, which produces exactly
 * the listing's assignment without moving any priority at all. The next step
 * is therefore to find the two references WITHOUT writing a product into
 * either variable.
 */

#include "common.h"
extern s32 D_8006A8C8[];
extern s32 D_8006A8DC[];

s32 func_8004D880(s32);
void func_8004D91C(u8, s16 *);

void func_8003AB44(u8 *p, s16 b, s32 c, u8 d, s16 e, s16 f, s32 g) {
    RECT r;
    s32 x;

    x = b * D_8006A8C8[d];
    x += e;
    if (x >= 0x140 && x < 0x400) {
        func_8004D91C(func_8004D880(p[1] | (p[0] << 8)), (s16 *) &r);
        c = (s16) c * D_8006A8DC[d];
        MoveImage(&r, x, f + c);
    }
}
