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
 *
 * TWO ADDITIONS, 2026-10-27, both from reading the two streams side by side
 * rather than the margins.
 *
 * (1) THE MAGIC CONSTANT'S REGISTER DIFFERS TOO, which the note above does
 * not mention, and it is the other half of the same fact:
 *
 *     target  lui $t1,0xcccc ; ori $t1 ; multu $a0,$t1 ... andi $v1,$a1,0xffff
 *     built   lui $t2,0xcccc ; ori $t2 ; multu $a0,$t2 ... andi $t1,$a1,0xffff
 *
 * and the POSITIONS differ with them: this build computes the first mask at
 * word 6, before the first digit's divide, while the listing computes it at
 * word 14, immediately before the loop guard that uses it. So in the listing
 * the magic is allocated first and takes $t1, leaving the mask $v1; here the
 * mask is allocated first, takes $t1, and pushes the magic to $t2. Any lever
 * has to move that order, and the mask's EARLY POSITION is the thing to
 * attack -- a single-set pseudo is bumped by sched1 and placed just before
 * its use (section 8), which is what the listing shows, so ask why this
 * build's mask is NOT being bumped.
 *
 * (2) THE LISTING HAS TWO QUANTITIES WHERE THIS HAS ONE. Its mask is SHORT --
 * from the `andi` to the `addu $t2, $v1, $zero` -- and the bound then rides
 * in $t2 through the loop; this build has one long-lived mask in $t1 and
 * therefore nothing to copy. That is why `m = w` coalesces away: two pseudos
 * holding the same value merge unless they CONFLICT or one of them is
 * MULTI-SET. So the shape to look for is not another placement of `m = w`
 * (four were measured) but a second SET of the bound that survives cse --
 * section 3's note on which second sets survive lists the three kinds, and
 * none of them is available before a loop with no preceding branch, which is
 * the real obstacle here and worth stating plainly.
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
