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
 * 2026-10-27: THE -8 OFFSET IS A GIV, so the question is really "what stops
 * loop.c strength-reducing this loop", and the answer has to cost no words.
 * The listing does NOT reduce it -- it recomputes the index each iteration,
 * TWICE, with the decrement between the two shifts:
 *
 *     addu $a2,$a0,$zero   <- the base copied out, because $a0 is reused
 *     sll  $a0,$a1,3       <- dest offset, from i
 *     addiu $a1,$a1,-1     <- i--
 *     sll  $v0,$a1,3       <- source offset, from the NEW i
 *     addu $v0,$v0,$a2 ; lw $v1,0($v0) ; addu $a0,$a0,$a2 ; sw $v1,0($a0)
 *
 * while this build has `addiu $t0,$t0,-8` -- a walked pointer, i.e. exactly
 * the giv the question above describes as "sharing one base and offsetting by
 * -8". So the two addresses are not computed separately because the INDEX is
 * no longer being multiplied at all.
 *
 * Measured 2026-10-27, and the word count is the wall:
 *   * `s32 j = i; i--; D_8006B200s[j] = D_8006B200s[i];` -- the decrement
 *     between the two subscripts, which is the listing's own order: 32 words
 *     (one too many), 32 DIFF.
 *   * the same through a pointer local (`Unk8006B200 *d = &D_8006B200s[i];
 *     i--; *d = D_8006B200s[i];`): 31 of 31 words and 28 DIFF -- the only
 *     variant that holds the count, but loop.c still reduces it to a walked
 *     pointer, so it buys nothing.
 *   * three GOTO-LOOP forms, which is the repo's standing way to hide a loop
 *     from loop.c (section 8): plain, with `--i` in the test, and with the
 *     pointer local. All three are 32 words and 32 DIFF -- the extra guard
 *     costs the word the giv saves.
 *
 * So: the loop must not be a loop to loop.c AND must not spend a word on the
 * guard. Those two are in tension in every form tried, and that tension is
 * the real content of this park.
 *
 * THE "EXPLICIT WORD COPIES" SUGGESTION IS REFUTED, 2026-10-28. The paragraph
 * above offers it as one of two ideas; it costs six or seven words:
 *
 *     two `u32` locals for the saved element, fields copied in and out   24/31
 *     the same plus field-by-field copies in the loop                    25/31
 *
 * against the struct assignment's 31 of 31. The struct assignment is what
 * holds the word count -- with explicit fields gcc shares and folds more of
 * the addressing -- so it is load-bearing and that idea is closed.
 *
 * ON THE SAVE/RESTORE ITSELF, worth recording because it looks like the
 * standing struct-copy lever and is not: the listing INTERLEAVES the two words
 * while this build batches them --
 *
 *     target  lw $v1,0($v0) ; addiu $sp,-8 ; sw $v1,0($sp) ; lw $v0,4($v0) ;
 *             blez ; sw $v0,4($sp)
 *     built   lw $v0,0($a2) ; lw $v1,4($a2) ; sw $v0,0($sp) ; blez ;
 *             sw $v1,4($sp)
 *
 * -- load, store, load, store against load, load, store, store. Both are the
 * same four insns from the same struct assignment, so this is sched2 ordering
 * two independent lw/sw pairs, not the block-move-versus-word-pairs choice in
 * section 8, and no spelling of the copy reaches it.
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
