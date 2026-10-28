/* func_800537D0 (unk_1C540) -- 34 of 34 words, 3 DIFF. NOT compiled.
 *
 * Written out of the shared coalesced-copy row on 2026-10-27; 34 of 34 words
 * and 3 DIFF on the first draft, which is the figure that row quotes.
 *
 * The right-aligned decimal formatter: walks the buffer from the last column
 * down, writing `v % 10 + 0x30` until the quotient runs out and then spaces
 * (0x20) for the remaining columns. Division by 10 through the 0x66666667
 * magic (tools/decomp/magic.py; do not estimate it).
 *
 * WHAT IS PAID FOR
 *
 *   * `u8 *d = buf + i;` inside the body, which stops loop.c
 *     strength-reducing `buf[i]` into a walked pointer -- the listing's
 *     `addu $a3, $a2, $a1` per iteration.
 *   * The DIGIT ARM COMES FIRST (`if (blank == 0) { digit } else { space }`),
 *     so the blank store is the branch target. jump2 then cross-joins the two
 *     arms' `mult` into the shared tail, which is why the listing shows the
 *     multiply twice.
 *   * The quotient in its OWN local, tested before the assignment:
 *     `q = v / 10; if (q == 0) { blank = 1; } v = q;`.
 *
 * WHAT IS LEFT: THREE WORDS, and they are the row's shared coalesced copy --
 * `addu $a0, $v1, $zero`, the quotient moved into `v`'s register. The target
 * computes the quotient into its own register, tests THAT, and copies it in
 * the branch's delay slot; here `q` and `v` coalesce into one pseudo and the
 * copy disappears.
 *
 * They cannot be made to conflict from the source: `v = q` is `q`'s last use,
 * so nothing keeps it live across the assignment, and the row has already
 * measured that statement in every order, a local copy of the parameter, and
 * `if (v / 10 == 0)` with the division repeated. TODAY'S RETURN-TYPE LEVER
 * (matching-levers section 1) does not reach it either: the copy is between
 * $v1 and $a0, and making $v0 live out at the epilogue does not touch either.
 * Read the row for the five known cures and why a parameter defeats all of
 * them.
 *
 * AUDIT 2026-10-27 -- THIS PARK'S LIVE LENGTHS ARE DOUBLED, so any priority
 * or margin number above that involves pseudo 82 is wrong, and any
 * "live <= N" instruction from the MARGINS table for it is unreachable.
 * `update_equiv_regs` (local-alloc.c) does `reg_live_length *= 2` for a once-set
 * pseudo whose init carries a constant REG_EQUAL note, and global-alloc reads the
 * doubled value -- so this pseudo reports roughly twice its real range in a
 * 34-word function. See matching-levers section 3. Re-derive the arithmetic over
 * halved lengths before concluding anything about that pair.
 */

#include "common.h"

void func_800537D0(s32 v, s32 n, u8 *buf) {
    s32 blank = 0;
    s32 i;
    s32 q;

    for (i = n - 1; i >= 0; i--) {
        u8 *d = buf + i;

        if (blank == 0) {
            *d = v % 10 + 0x30;
        } else {
            *d = 0x20;
        }
        q = v / 10;
        if (q == 0) {
            blank = 1;
        }
        v = q;
    }
}
