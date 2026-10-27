/* func_8004E4C0 (unk_1C540) -- 46 of 47 words, 45 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep row on 2026-10-27 so there is a source to
 * sweep.
 *
 * The fixed-width decimal formatter: writes `n`'s digits
 * least-significant-first into the buffer, pads the rest with the fourth
 * argument, NUL-terminates and answers `out - w`. Division by 10 through the
 * 0xCCCCCCCD magic (tools/decomp/magic.py; do not estimate it).
 *
 * THE SHAPE IS SETTLED and every instruction is right. `w` is a `u16`
 * PARAMETER, which is what puts an `andi $..., $a1, 0xFFFF` at each of its
 * three uses -- the first loop's guard, the pad loop's guard and the final
 * subtraction -- rather than one shared mask. The first loop is
 * `for (i = 1; i < w; i++) { n = n / 10; if (n == 0) break; *out++ = ...; }`
 * and the pad loop continues on the same `i` and bound.
 *
 * THE ONE MISSING WORD IS `addu $t2, $v1, $zero`, and it is a REGISTER
 * artifact rather than a statement. Both streams mask `w` three times. The
 * target's first mask lands in `$v1`, which the loop BODY clobbers (`sll $v1,
 * $v0, 2` building the x10), so the bound has to be copied to `$t2` before
 * the loop; here the mask lands in `$t1`, which nothing in the body touches,
 * so there is no copy to make. The difference is which register the mask
 * gets, not whether the source names the bound: an `m = w` local is
 * coalesced away, because nothing makes `m` conflict with the mask.
 *
 * So the lever to look for is whatever forces the mask into a
 * body-clobbered register -- read `advise.py --greg` and see which quantity
 * holds `$v1` across the loop in each build, and consider the dead-reference
 * trick from matching-levers section 3 on the loop's own scratch values
 * rather than on the bound. Measured and not working (the row's list plus
 * this file's): an explicit guard plus do-while, `w > 1` as the guard, an
 * `m = w` local for the first bound, for the pad bound, and before the guard
 * -- all 46 words -- and a rotated loop at 48.
 */

#include "common.h"

u8 *func_8004E4C0(u32 n, u16 w, u8 *out, u8 pad) {
    s32 i;

    *out++ = n % 10 + 0x30;
    for (i = 1; i < w; i++) {
        n = n / 10;
        if (n == 0) {
            break;
        }
        *out++ = n % 10 + 0x30;
    }
    for (; i < w; i++) {
        *out++ = pad;
    }
    *out = 0;
    return out - w;
}
