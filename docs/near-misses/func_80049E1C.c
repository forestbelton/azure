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
