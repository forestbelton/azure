/* func_8004D880 (unk_1C540) -- 28 of 28 words, 12 DIFF. A Shift-JIS to ASCII
 * classifier:
 * three `(u16)(c + K) < N` range tests, each returning the same sum plus an
 * ASCII base, falling through to func_8004D828.
 *
 * The word count and the structure are right and what is left is allocation.
 * The listing's three long-lived values sit as
 *
 *   target:  parameter -> $v1   call argument -> $a0   the sums -> $a1
 *
 * and the 2026-10-28 section below has where this source puts them, why the
 * two-variable form cannot reach it, and what the current four-allocno source
 * has left.
 *
 * Two shapes are paid for and should not be re-derived:
 *
 * * The SUM is its own `u16` local, assigned per test and reused by the
 *   return. Writing the sum inline in both places (`if ((u16)(c + 0x7DB1) <
 *   0xA) return c + 0x7DB1 + 0x30;`) lets `fold` merge the two constants, and
 *   because the return type is `u8` it merges them MOD 256: the listing's
 *   `addiu $v0, $a1, 0x30` comes out as `addiu $v0, $a0, 0xE1`. As a `u16`
 *   local the truncation is free (a HImode move is an `addu` on MIPS), the
 *   `< 0xA` test supplies the `andi 0xFFFF` the listing has, and the return's
 *   `+ 0x30` lands on the sum's own register.
 *
 * * The third test REASSIGNS the parameter (`c = c + 0x7D7F`), which is what
 *   makes the listing's third `addiu` write $v1 in place rather than taking a
 *   fourth register.
 *
 * 2026-10-28: 22 DIFF -> 12, and the shape of what is left is now different
 * from what this header used to claim.
 *
 *  - (*) THE THIRD TEST IS INVERTED: `if ((u16) c >= 0x1A) { return
 *    func_8004D828(d); } return c + 0x61;` rather than the in-range return in
 *    the `if`. The listing falls THROUGH to `addiu $v0, $v1, 0x61` and
 *    branches away to the call, which fills its `j`'s delay slot; written the
 *    other way round this source branches to the return instead and spends a
 *    `nop`. ON ITS OWN THAT IS 27 WORDS, 26 DIFF -- A WORD SHORT -- which is
 *    the real news: the 28 words this file used to have were 28 for the WRONG
 *    REASON, a `nop` standing in for the listing's entry copy.
 *  - (*) THE PARAMETER IS COPIED INTO ITS OWN LOCAL: `c = x;` with the call's
 *    argument then taken as `d = c`. That is the listing's
 *    `addu $v1, $a0, $zero`, and it restores the 28th word. 26 -> 12.
 *
 * WHY THE TWO-VARIABLE FORM CANNOT GET THERE, which is what the old header was
 * really asking. The listing holds the parameter in $v1 and the call argument
 * in $a0; this source's parameter takes $a0 because it PREFERS it (it arrives
 * there) and it is allocated first. Both routes out are now closed by reading:
 *
 *   * `regs_someone_prefers` CANNOT bar it -- and note the reason is the
 *     SUBTRACTION below, not the pass structure. (A reservation on a
 *     caller-saved register like $a0 IS effective in general: pass 0 can hand
 *     those out, because global.c:352 seeds `regs_used_so_far` with every
 *     caller-saved register. It is only callee-saved registers that pass 0
 *     cannot give out for the first time.) `prune_preferences` (global.c:875)
 *     subtracts the higher-priority allocno's own preferences from the set it
 *     inherits from a conflicting lower-priority one -- "if we also prefer
 *     some of those registers, don't exclude them" -- and skips the
 *     subtraction only when the lower allocno is LARGER. `allocno_size` is
 *     PSEUDO_REGNO_SIZE, which is in WORDS (regs.h:64), so every integer value
 *     in this function is size 1, the subtraction always applies, and a
 *     register the allocno prefers is never excluded. Measured in passing:
 *     the parameter as `u16`/`s16` with `d` as `u32`/`s32`, in all four
 *     combinations, is still 22 DIFF (and `s16` is 29 words).
 *   * PRIORITY CANNOT BAR IT EITHER, because `d` holds the SAME VALUE as the
 *     parameter and cse canonicalises every read to one pseudo. Moving the
 *     three sums' reads onto `d` to raise it does nothing at all -- one sum,
 *     two sums, both 28 words and 22 DIFF -- and moving the third sum too
 *     costs a word (29). So references cannot be moved between them.
 *
 * WHAT IS LEFT: 12 words, and they are an allocation puzzle with FOUR allocnos
 * where the listing has three live values. `advise.py --greg` on this source:
 *
 *     the sums (82)   6 refs / 11 live / 1.091  -> $v1, target $a1
 *     x        (80)   6 refs / 15 live / 0.800  -> $a0, target $a0 (dies early)
 *     c        (83)   4 refs / 12 live / 0.667  -> $a1, target $v1
 *     d        (81)   2 refs / 13 live / 0.154  -> $a2, target $a0
 *
 * so `d` has to end up sharing $a0 with `x` -- which it may, since x dies at
 * `c = x` -- and `c` and the sums have to swap.
 *
 * WHAT STOPS `x` BEING SHORT is cse, and it is the same obstacle as above seen
 * from the other side: `c = x` makes the two pseudos one value, cse
 * canonicalises every later read to the OLDER of them -- the parameter -- so
 * the sums read `x` and its range runs to the third test (6 refs / 15 live,
 * the longest of the four) instead of ending at the copy. Measured 2026-10-28
 * and NOT working, both of them by removing a copy this function needs:
 *
 *   * `u16 c` or `s16 c` instead of `u32 c`, to make the values differ so cse
 *     keeps them apart: 27 words, 26 DIFF. The narrowing is free, which is
 *     exactly the problem -- a HImode move coalesces and the entry copy goes
 *     with it.
 *   * dropping `d` and passing the parameter itself (`func_8004D828(x)`, with
 *     or without a `(u16)` cast), so that x legitimately holds $a0 to the
 *     call: 26 words, 27 DIFF -- BOTH copies disappear.
 *
 * So the entry copy survives only while `c` is word-wide AND reassigned, and
 * in that shape cse hands the sums to `x`. Something has to make `c` the
 * canonical pseudo for those three reads without changing its value or its
 * width.
 */

#include "common.h"

u8 func_8004D828(u16);

u8 func_8004D880(u32 x)
{
    u16 d;
    u16 e;
    u32 c;

    c = x;
    d = c;
    e = c + 0x7DB1;
    if (e < 0xA)
    {
        return e + 0x30;
    }
    e = c + 0x7DA0;
    if (e < 0x1A)
    {
        return e + 0x41;
    }
    c = c + 0x7D7F;
    if ((u16) c >= 0x1A)
    {
        return func_8004D828(d);
    }
    return c + 0x61;
}
