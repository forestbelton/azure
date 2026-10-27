/* func_8003C67C (unk_72A8) -- 31 of 31 words, 28 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep row on 2026-10-27, with the row's open
 * question ANSWERED: a backward `goto` does stop the strength reduction.
 *
 * Moves D_8006B200s[i] to the front, shifting the rest up through an 8-byte
 * stack temp. The word count is right as STRUCT assignments on the
 * `Unk8006B200 D_8006B200s[]` alias -- the block moves then cost exactly what
 * the target spends.
 *
 * WHAT IS LEFT: loop.c strength-reduces both addresses into pointers walked
 * by -8, where the target recomputes two `sll ..., 3`s every iteration with
 * the base in $a2:
 *
 *     L6AC:  sll   $a0, $a1, 3        <- the DESTINATION index, old i
 *            addiu $a1, $a1, -0x1     <- i--
 *            sll   $v0, $a1, 3        <- the SOURCE index, i.e. the NEW i
 *            addu  $v0, $v0, $a2
 *            lw/sw ... ; lw/sw ...
 *            bgtz  $a1, .L8003C6AC
 *
 * MEASURED 2026-10-27, and the shape is now bracketed rather than open:
 *
 *   * `while (i > 0) { D_8006B200s[i] = D_8006B200s[i - 1]; i--; }`
 *     -- the source below -- 31 words, 28 DIFF, fully strength-reduced.
 *   * THE SAME LOOP AS A BACKWARD `goto` (`if (i > 0) { loop: ...; if (i > 0)
 *     goto loop; }`): 32 words, 32 DIFF, and NO STRENGTH REDUCTION -- one
 *     `sll` per iteration, which is half of what the listing wants. This is
 *     section 2's "a backward goto is not a loop to loop.c at all", and it
 *     answers the row's guess about `loop_has_call`: the threshold is not the
 *     reason, the loop notes are. The cost is that the symbol is then
 *     materialised TWICE, once per address.
 *   * goto PLUS a `Unk8006B200 *p = D_8006B200s;` base local: 30 words, 31
 *     DIFF -- ONE materialisation copied to a register, which is the
 *     listing's `addu $a2, $a0, $zero`, and still no strength reduction. But
 *     gcc then shares ONE address between source and destination and reaches
 *     the source with `-8`/`-4` displacements, where the listing computes two.
 *   * goto + base local + `j = i; i--; p[j] = p[i];` (the decrement BETWEEN
 *     the two index uses, which is what the listing's two `sll`s say): 32
 *     words, 32 DIFF.
 *   * the same `j` form as a `while` loop, with and without the base local:
 *     32 words, 32 DIFF -- strength-reduced again, so the base local
 *     re-enables it whenever the loop is a real loop.
 *
 * So the target sits between the 30-word goto-plus-base form and the 32-word
 * two-index form, and the question is now narrow: with the goto loop and ONE
 * base register, what spelling makes gcc compute the two addresses separately
 * instead of sharing one and offsetting by -8? Try the source index through a
 * second base local, or the two struct halves as explicit word copies.
 *
 * Also measured by the row, all worse: `while` with `i--` in the condition, a
 * `for`, `p[i] = p[--i]`, a separate `j = i - 1`, field-by-field copies, and a
 * `(u32)`-cast index.
 */

#include "common.h"

void func_8003C67C(s32 i) {
    Unk8006B200 t;

    t = D_8006B200s[i];
    while (i > 0) {
        D_8006B200s[i] = D_8006B200s[i - 1];
        i--;
    }
    D_8006B200s[0] = t;
}
