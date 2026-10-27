/* func_8004827C (unk_1AF38) -- 18 of 19 words, 12 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep rows on 2026-10-27 so there is a source to
 * sweep. Calls func_80047BC0 with the s16 second argument and stores
 * `D_80083D08 + result * 8` into the +0x28 field of the record at `p + 0xC`.
 *
 * WHAT IS LEFT is ONE WORD, and it is the shared `addu $rX, $rY, $zero` of
 * the `func_8004827C + func_8004D880 + func_800537D0 + func_80055CF4 +
 * func_8003BD84 + func_80058A04` row -- a copy local-alloc coalesces away and
 * the target keeps. Here it is the PARAMETER, and the target spends TWO
 * registers on it:
 *
 *     addu $s0, $a0, $zero      <- the entry save, because $a0 is the call's
 *     ...                          own argument register
 *     jal  func_80047BC0
 *     addu $v0, $s0, $zero      <- a SECOND copy, just to read through
 *     lw   $a1, 0xC($v0)
 *
 * where this source reads `0xC($s0)` directly off the saved copy. Measured:
 * the read through an explicit `b = p;` local after the call coalesces the
 * same way (identical, 18 words).
 *
 * The row lists the five known CURES for this copy -- re-dereference instead
 * of caching, an explicit guard plus do-while on a loop bound, a statement
 * between the producer and the copy, unifying two values so a loop's
 * conflict is imported, and making the register unavailable -- and notes that
 * none of them applies to a PARAMETER, because a parameter has no second live
 * range to give it. That is still true here: `p` is read exactly once, so
 * there is nothing to split.
 *
 * The one thing NOT yet tried on this family is today's return-type lever
 * (matching-levers section 1): declaring a `void` function non-void makes
 * $v0 live out at the epilogue and so unavailable, which is the "make the
 * register unavailable" cure in a form a parameter CAN have. It cannot help
 * this function -- the copy it wants is INTO $v0, not away from it -- but
 * func_80055CF4 and func_8003BD84 should be checked against it before
 * anything else.
 */

#include "common.h"

s32 func_80047BC0(s16);

void func_8004827C(u8 *p, s16 i) {
    s32 k = func_80047BC0(i);
    u8 *q = *(u8 **) (p + 0xC);

    *(u32 *) (q + 0x28) = (u32) ((u8 *) D_80083D08 + k * 8);
}
