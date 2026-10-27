/* func_8003BD84 (unk_72A8) -- 37 of 38 words, 24 DIFF. NOT compiled.
 *
 * Written out of the shared coalesced-copy row on 2026-10-27 so there is a
 * source to sweep; the row listed it without a figure of its own.
 *
 * An atan2-style ratio: takes the absolute values of both arguments, and
 * depending on which is larger divides the matching argument by
 * func_800644B8 or func_80064584 of func_80065F90's angle, then shifts left
 * by 12. The two `jal func_80065F90` sites are not cross-joined, so the call
 * is inside each arm.
 *
 * WHAT IS PAID FOR: the two arms as an `if`/`else` assigning ONE result local
 * rather than two `return`s. With a `return` in each, gcc moves the first arm
 * to the end of the function (the func_8004D690 transformation) and the test
 * comes out `bnez` where the listing has `beqz`: 25 DIFF against 24.
 *
 * WHAT IS LEFT: ONE WORD and the registers with it. The missing word is in
 * the absolute values: the target copies and negates IN PLACE,
 *
 *     bgez  $s1, .L8003BDA4
 *      addu $v0, $s1, $zero     <- the copy, in the delay slot
 *     negu  $v0, $v0            <- negating the COPY
 *
 * while this source emits the copy and then `negu $v0, $s1`, negating the
 * ORIGINAL. Section 5's conditional-negate lever is about the opposite case
 * (two quantities rather than one), and what it says here is that these ARE
 * one quantity, so the in-place negate should fall out -- but it does not:
 * `ax = x; if (ax < 0) { ax = -ax; }` measures identically to
 * `ax = (x < 0) ? -x : x`, because cse folds the test and the negation's
 * operand back to `x`'s register either way.
 *
 * So what is wanted is a spelling where cse CANNOT see that `ax` and `x` are
 * the same register at the negation -- which is the shared coalesced-copy
 * problem of this row in yet another form, and the row's five cures are all
 * elsewhere in the source. One thing not yet tried: a narrow declaration for
 * `ax`/`ay` (section 5's lever uses `s8` for exactly this separation),
 * remembering that a width change here must be checked against the `slt` --
 * the comparison is signed and full width in the listing.
 */

#include "common.h"

s32 func_80065F90(s32, s32);
s32 func_800644B8(s32);
s32 func_80064584(s32);

s32 func_8003BD84(s32 x, s32 y) {
    s32 ax;
    s32 ay;
    s32 r;

    ax = x;
    if (ax < 0) {
        ax = -ax;
    }
    ay = y;
    if (ay < 0) {
        ay = -ay;
    }
    if (ax < ay) {
        r = (y / func_800644B8(func_80065F90(y, x))) << 12;
    } else {
        r = (x / func_80064584(func_80065F90(y, x))) << 12;
    }
    return r;
}
