/* func_80048FBC (unk_1C540) -- 18 of 18 words, 7 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep row on 2026-10-27 so there is a source to
 * sweep. Eighteen words, one statement, and every one of them right except
 * the two callee-saved registers.
 *
 * `p[func_80048F8C(p, n)] = func_8004B404(0x104);` -- both arguments of the
 * first call pass through untouched, so neither move is emitted, and the
 * second call's constant is its only setup.
 *
 * WHAT IS LEFT is ONLY which of $s0/$s1 holds the pointer and which the index
 * chain: the target keeps the pointer in $s0 and builds the chain in $s1,
 * this source the other way round. local-alloc's own arithmetic says why --
 * the chain (the result copy, the `sll` and the `addu`, all one quantity) has
 * six references over four insns and the pointer two over nine, so the chain
 * outranks the pointer and takes $s0 first.
 *
 * MEASURED, all 18 words and all 7 DIFF: the statement as one expression; an
 * `s32 i` local for the index; a `u32 v` local for the value as well, so both
 * calls are their own statements; `*(u32 *) ((u8 *) p + i * 4)` as the
 * destination; `*(p + i)`; and a `(u32)` cast on the call's result. The row
 * also measured `p += f(...)` and a separate `q = p + i`, which move the
 * arithmetic ahead of the second call instead (16 DIFF).
 *
 * THOSE PRIORITIES WERE THIS HEADER'S OWN ESTIMATE AND THEY ARE WRONG --
 * corrected 2026-10-28 from `advise.py --greg`, which reads the dumps. There
 * is no 6-reference chain quantity. The function's four quantities are
 *
 *     85   2 refs /  2 insns / 4.000   -- crosses no call
 *     86   2 refs /  2 insns / 4.000   -- crosses no call
 *     82   2 refs /  3 insns / 2.667   -- crosses 1 call: THE INDEX
 *     80   3 refs / 16 insns / 0.750   -- crosses 2 calls: THE POINTER
 *
 * and 85/86 never compete for $s0 or $s1 at all, because they cross no call
 * and `find_free_reg` hands them caller-saved registers. The whole contest is
 * 82 against 80, at 2.667 against 0.750 -- a factor of 3.6, not the factor of
 * 13 this header used to claim. What it would take, in the same arithmetic:
 *
 *   * THE POINTER NEEDS SIX REFERENCES (it has three): floor_log2(6)*6*4/16 =
 *     3.000 clears 2.667, while four refs give 2.000 and five give 2.500 --
 *     both still short. So this is not one free reference, it is three.
 *   * OR THE INDEX NEEDS AN ELEVEN-INSN RANGE (it has three):
 *     floor_log2(2)*2*4/11 = 0.727 falls below 0.750. Its range runs from the
 *     first call's result to the `sll`, and there is nothing to put between
 *     them that does not cost a word.
 *
 * Measured 2026-10-28 and NOT working, all THREE WORDS SHORT at 15 of 18 and
 * 16 DIFF: reassigning the pointer to buy references -- `p += func_80048F8C(p,
 * n);`, `p = p + func_80048F8C(p, n);` and `p = &p[func_80048F8C(p, n)];`,
 * with the store then through `*p`. They do add the references, but a pointer
 * that is dead after the store no longer needs to survive the second call, so
 * gcc keeps the whole function in caller-saved registers and the two
 * callee-saved saves and restores go with it.
 *
 * So the lever is not a spelling of the statement, and the two targets are the
 * ones in the arithmetic above: three free references on the pointer, or eight
 * more insns of range on the index. The pointer's range is already the whole
 * function (it is the parameter, live from entry to the store) so there is
 * nothing to shorten, and every reference bought so far has come with an
 * instruction or has removed the call-crossing that makes the register
 * callee-saved in the first place.
 */

#include "common.h"

s32 func_80048F8C(u32 *, s32);
u32 func_8004B404(s32);

void func_80048FBC(u32 *p, s32 n) {
    p[func_80048F8C(p, n)] = func_8004B404(0x104);
}
