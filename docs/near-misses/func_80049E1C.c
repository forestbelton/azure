/* func_80049E1C (unk_1C540) -- 20 of 20 words, 8 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep row on 2026-10-27, and ten words better
 * than the row's 18: it had this at 16-18 words of 20, three or four short,
 * and "needs three or four more copies than any of six shapes produces".
 *
 * The wrapping clamp, func_80049DE8's family: adds the delta, then clamps at
 * 0 going down and at `m - 1` going up, and answers 0 when `m` is 0.
 *
 * THREE THINGS BUY THE MISSING WORDS, and all three are block LAYOUT or the
 * conditional-expression form rather than arithmetic:
 *
 *   * BOTH arms are conditional expressions, not `if`s. `a = (a < 0) ? 0 : a;`
 *     is what landed func_80049DE8, and the same form on the OTHER arm
 *     (`r = (a >= m - 1) ? m - 1 : a;`) is what pays for the rest of the
 *     copies -- a plain `if` optimises them away. With the positive arm as an
 *     `if` the function is 17 of 20 words; with both as expressions it is 20.
 *   * THE `m == 0` ANSWER GOES LAST: `if (m != 0) { ... return r; } return 0;`
 *     and not `if (m == 0) return 0;` first. The listing's `beqz $a2` jumps
 *     to the function's LAST block, so the zero answer is the one placed at
 *     the end -- worth 8 differing words (16 against 8).
 *   * THE POSITIVE ARM IS THE FALL-THROUGH: `if (d >= 0) { clamp high }
 *     else { clamp low }`, because the listing's `bltz $a1` branches to the
 *     low-clamp arm. Written `if (d < 0)` first it is 12.
 *
 * A separate `r` for the result is needed in any case: returning from each
 * arm directly is 17 of 20 words (10 DIFF), because the two returns then
 * share one copy.
 *
 * WHAT IS LEFT: eight words, and they are which register each arm's value
 * lands in. The target puts the high clamp's value in $a0 and the low
 * clamp's in $v1, with $v0 as the common result; this source uses $a0 for
 * both. Writing the high clamp as a plain `if` on `r` (variant i in the
 * measurements) is also 8 and lands the same way, and a `q = m - 1` local is
 * 18 words. So the two arms want DIFFERENT result registers, which is an
 * allocation question -- read `advise.py --greg`.
 *
 * THE `--greg` READING, done 2026-10-27, and it closes the obvious direction.
 * From the `.greg` dump:
 *
 *     ;; 5 regs to allocate: 80 83 85 81 82
 *     ;; 80 preferences: 2 4      -> gets $v0   (the result `r`)
 *     ;; 83 preferences: 4        -> gets $a0   (7 refs, the busiest)
 *     ;; 83 conflicts: 82 83 85 2 29
 *     ;; 85 preferences: 6        -> gets $a2
 *     ;; 82 preferences: 6        -> also $a2 (82 and 85 do not conflict)
 *
 * Two things follow. 83 CONFLICTS with $v0 (2), so $v0 is illegal for it and
 * its $a0 is not a tie-break that more references would win -- and `r` (80)
 * already has the $v0 the listing gives it. The eight words are therefore
 * entirely about the LOW arm's value, which the listing puts in $v1 and this
 * build folds into 83's $a0.
 *
 * And that fold cannot be broken by adding a variable, which is the thing to
 * stop trying: the two arms' values live in DIFFERENT BLOCKS, so their live
 * ranges never overlap, they never conflict, and local-alloc ties them into
 * one quantity however many locals the source spells. Measured, all 20 of 20
 * words and 8 DIFF: a `s32 t` local in the low arm assigned from the ternary;
 * the same split into `t = a; if (t < 0) t = 0;`; and `r = a;` hoisted ahead
 * of the `d >= 0` test with each arm narrowing it. A local in the HIGH arm is
 * worse (17 words, 9 DIFF), and section 3's seventh cure does not apply --
 * there is no copy whose source could stay live past it, because the two
 * values are never simultaneously live.
 *
 * So the question is what gives ONE arm's value a preference or a conflict
 * the other lacks. On the evidence that is not reachable by naming variables.
 *
 * 2026-10-28, THREE MORE SHAPES, all of them WORSE, and they say the same
 * thing from the other direction: clamping `a` IN PLACE and copying it to `r`
 * removes the very copies the target spends.
 *
 *     high arm in place, low arm as now      17 words,  9 DIFF
 *     low arm in place, high arm as now      19 words, 14 DIFF
 *     both arms in place                     16 words, 20 DIFF
 *
 * So each arm needs its own temp that is NOT the parameter, which is what this
 * file already has -- and the open question stands. For the record of what the
 * target's shape actually is: it spends TWO copies in the high arm (one per
 * ternary branch, `addu $v0, $a0, $zero`) and ONE in the low arm
 * (`addu $v0, $v1, $zero`), with the high temp in $a0 and the low in $v1. For
 * the low temp to be $v1 rather than $a0 it has to be excluded from $a0 in its
 * own block, and the only thing that can do that is a conflict -- so the shape
 * to look for is one where the low arm's value is live at a point where $a0 is
 * (the incoming parameter at the top), without paying for the copy that gets
 * it there. Note that making it a cross-block allocno is the lever that landed
 * func_800565D8, but every placement tried so far keeps it inside one block.
 */

#include "common.h"

s32 func_80049E1C(s32 a, s32 d, s32 m) {
    s32 r;

    if (m != 0) {
        a += d;
        if (d >= 0) {
            r = (a >= m - 1) ? m - 1 : a;
        } else {
            r = (a < 0) ? 0 : a;
        }
        return r;
    }
    return 0;
}
