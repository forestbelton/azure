/* func_8004E9EC (unk_1C540) -- 32 of 33 words, 26 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep row on 2026-10-27. THE ROW'S 33/33 AND 27
 * DIFF COULD NOT BE REPRODUCED: the shape it describes (`n * 2` re-evaluated
 * in both conditions, the three bytes read into locals before the stores,
 * D_80080B70 read signed through an alias) measures 34 words and 32 DIFF
 * here, because loop.c gives the walked `q` TWO givs in the inner loop -- one
 * at offset 0 and one biased +2, with `sb $v1, -0x1($a2)` off the second --
 * where the listening has one pointer and displacements 0, 1, 2. The row's
 * "what is left is registers" was therefore missing a word.
 *
 * The template fill: while the cursor's byte is zero, copy the three
 * D_80080B70 bytes over it two at a time until the counter reaches n * 2, and
 * answer 1 if anything was copied.
 *
 * WHAT IS PAID FOR
 *
 * * THE INNER LOOP IS A BACKWARD `goto`, not a `while`. That is what stops
 *   loop.c strength-reducing the three stores into a second biased pointer:
 *   as a `while` it is 34 words with two `addiu ..., 0x2`s per iteration, as a
 *   guarded `goto` it is 32 with one, which is the listing's shape. Section 2
 *   of matching-levers (the same lever that answered func_8003C67C's row).
 * * THE INNER BOUND IS A LOCAL (`m = n * 2;` before the guard) while the
 *   OUTER condition recomputes `n * 2` -- the listing's `addu $t0, $v1, $zero`
 *   before the inner loop against its `sll $v1, $t2, 1` each outer iteration.
 *   Worth 3 differing words.
 * * The three bytes go through `s32` locals, which is what makes them `lb`:
 *   as `s8` locals the value needs no sign extension and gcc emits `lbu`.
 *   D_80080B70 needs the `s8` __asm__ alias, since the header's is `u8`.
 *
 * WHAT IS LEFT: ONE WORD AND THE REGISTERS, and the word is named. The target
 * COPIES THE COUNT OUT OF `$a1` (`addu $t2, $a1, $zero`) so that `r`, the
 * result, can live in `$a1` and be copied to `$v0` in the `jr`'s delay slot;
 * this source leaves the count in `$a1` and puts `r` in `$a0`, so there is no
 * copy to make. `s32 n = count;` from a renamed parameter does NOT keep the
 * copy -- gcc coalesces it, since nothing makes `n` conflict with `$a1`. What
 * would is `r` being allocated before `n`: read `advise.py --greg` on this
 * function and compare the two quantities' priorities, and look for a
 * zero-cost extra reference to `r` (matching-levers section 3's dead-reference
 * lever) rather than another spelling of the count.
 *
 * AUDIT 2026-10-27 -- THIS PARK'S LIVE LENGTHS ARE DOUBLED, so any priority
 * or margin number above that involves pseudo 81 is wrong, and any
 * "live <= N" instruction from the MARGINS table for it is unreachable.
 * `update_equiv_regs` (local-alloc.c) does `reg_live_length *= 2` for a once-set
 * pseudo whose init carries a constant REG_EQUAL note, and global-alloc reads the
 * doubled value -- so this pseudo reports roughly twice its real range in a
 * 33-word function. See matching-levers section 3. Re-derive the arithmetic over
 * halved lengths before concluding anything about that pair.
 */

#include "common.h"

extern s8 D_80080B70s[] __asm__("D_80080B70");

s32 func_8004E9EC(u8 *q, s32 n) {
    s32 i;
    s32 r;
    s32 a;
    s32 b;
    s32 c;
    s32 m;

    r = 0;
    i = 0;
    while (i <= n * 2) {
        if (*q == 0) {
            m = n * 2;
            if (i < m) {
            inner:
                a = D_80080B70s[0];
                b = D_80080B70s[1];
                c = D_80080B70s[2];
                q[0] = a;
                q[1] = b;
                q[2] = c;
                i += 2;
                q += 2;
                if (i < m) {
                    goto inner;
                }
            }
            r = 1;
        }
        i++;
        q++;
    }
    return r;
}
