/* func_8004D880 -- 28/28 words, 22 DIFF. A Shift-JIS to ASCII classifier:
 * three `(u16)(c + K) < N` range tests, each returning the same sum plus an
 * ASCII base, falling through to func_8004D828.
 *
 * The word count and the whole structure are right, and what is left is a
 * three-way REGISTER PERMUTATION of the function's three long-lived values.
 *
 *   target:  parameter -> $v1   call argument -> $a0   the sums -> $a1
 *   built:   parameter -> $a0   call argument -> $a1   the sums -> $v1
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
 * The permutation is the open part. The parameter and the call's argument both
 * PREFER $a0 -- the parameter arrives there, the argument has to end there --
 * and they conflict, so whichever `find_reg` reaches first takes it. Here the
 * parameter wins because it has the references (three sums plus the copy
 * against the argument's two) over a live range of the same length, and
 * `prune_preferences`' `regs_someone_prefers` does not fire the other way.
 * Measured, all 28 words and 22 DIFF or worse: `d` as `u16`/`u32`/`s32`, the
 * two declarations in both orders, `d = c` as the first statement, as the last
 * statement before the call, and inside the call's own block (both 27 words,
 * because then the parameter simply stays in $a0 and the listing's SECOND copy
 * has nowhere to come from), and the parameter as `u32`/`s32`/`u16`.
 *
 * Note the listing spends TWO copies on one value -- in at entry and back out
 * for the call -- so a source that keeps it in $a0 throughout is a word short,
 * not a word better. Whatever bars the parameter from $a0 is the whole
 * finding; see the entry on `regs_someone_prefers` in docs/matching-levers.md
 * for the pass arithmetic, and func_80058A04 for the one cure that worked on
 * a non-parameter (give the value a second live range so a block-local
 * quantity's hard register conflicts with it -- a parameter has no second
 * range to give it). */

#include "common.h"

u8 func_8004D828(u16);

u8 func_8004D880(u32 c)
{
    u16 d;
    u16 e;

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
    if ((u16) c < 0x1A)
    {
        return c + 0x61;
    }
    return func_8004D828(d);
}
