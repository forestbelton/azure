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
 * THAT INSTRUCTION IS WRONG -- corrected 2026-10-27 from the dumps and the
 * listing. It is not a priority question and no reference count touches it.
 *
 * The `.greg` dump says `r` (pseudo 83) conflicts with only 2 and 29 -- $v0
 * and $sp -- so BOTH $a0 and $a1 are legal for it, and `find_reg`'s pass 1
 * walks hard registers in ascending number, so it takes $a0. The count
 * (pseudo 81) prefers 5 and gets $a1. Neither one's priority changes that:
 * $a1 is in `regs_someone_prefers[83]` during pass 0 only, and pass 1 drops
 * that and hands out the lowest LEGAL register.
 *
 * So the listing can only have `r` in $a1 if $a0 is ILLEGAL for it, i.e. if
 * `r` CONFLICTS with $a0 -- and the listing shows exactly why it does:
 *
 *     addu $t2, $a1, $zero     <- the count copied out
 *     addu $a1, $zero, $zero   <- r = 0, into $a1
 *     sll  $v1, $t2, 1
 *     bltz $v1, .L8004EA68
 *      addu $a3, $a1, $zero    <- i = 0, a COPY of r's zero register
 *     lui  $t1, %hi(D_80080B70)
 *     addu $a2, $a0, $zero     <- q's PARAMETER COPY, at insn 7
 *
 * `r` is live across q's parameter copy, where $a0 is still live, so it
 * conflicts with $a0 and $a1 is the lowest register left. In this build the
 * parameter copies are all at the top, `r = 0` comes after them, and the
 * conflict never forms.
 *
 * So the open question is: what emits q's parameter copy LATE, after `r = 0`?
 * It is pinned out of sched1 (section 8's prefix entry) but sched2 can move
 * it, and in sched2 every insn in that block is priority 1 -- so this is a
 * sched2 PLACEMENT question, LUID order and hazards, not an allocation one.
 * Note also that the listing's `i = 0` is a copy of `r`'s register, so cse has
 * merged the two zeros; any shape that separates them loses that copy.
 *
 * Measured and NOT working, all 32 of 33 words and 26 DIFF: `i = 0` before
 * `r = 0`; both initialised in their declarations with `r` first; the same
 * with `i` first; the two declarations swapped with the assignments unchanged;
 * and `i = r` in place of `i = 0`. Statement order does not reach it, which is
 * what the sched2 reading predicts.
 *
 * AUDIT 2026-10-27 -- pseudo 81 has a DOUBLED live length, and
 * it reports roughly twice its real range in this 33-word function.
 * `update_equiv_regs` (local-alloc.c:1018-1064) does `reg_live_length *= 2` for
 * any pseudo with `reg_n_sets == 1` whose insn carries a REG_EQUIV note, and
 * global-alloc reads the doubled value. The note comes from a CONSTANT_P
 * REG_EQUAL, from a MEM init confined to one block, or -- most often -- from
 * being a PARAMETER, which assign_parms equates to its incoming stack slot.
 *
 * READ THAT CORRECTLY: global-alloc really does use the doubled number, so the
 * ORDER `--greg` prints is the order gcc chose -- it is not wrong about the
 * assignment. What is wrong is treating those priorities as a measure of the real
 * live range, and the MARGINS table's `live <= N` instruction, which is in DOUBLED
 * units: shortening the real range by k moves the stored value by 2k, so the
 * instruction costs half as many real insns as it reads.
 *
 * AND IT IS A LEVER. A pseudo that is NOT once-set escapes the doubling entirely
 * (a large-constant pointer does it by accident: its pre-sched lui/ori split
 * leaves reg_n_sets == 2 before local-alloc runs). So giving 81 a second
 * set that survives cse DOUBLES its priority at zero instruction cost, and
 * then 81 would pass 96 (0.296 against 0.227).
 * Worth trying before any reference arithmetic. See matching-levers section 3.
 *
 * 2026-10-28: THE SECOND-SET LEVER IS NOT REACHABLE FOR A PARAMETER, measured
 * on func_80041E70 and recorded in all five files that carry this paragraph.
 * Three zero-cost spellings of a second set -- `p = p;` before the first
 * statement, after it, and `p = (Unk42710 *) (u8 *) p;` -- are each 270 of 270
 * words and the SAME 1 DIFF as the base, and the dumps say why rather than
 * leaving it to inference: the `flow` dump still carries exactly one
 * REG_EQUIV, and `.greg` prints the identical conflict list and the identical
 * allocation order. A no-op move is deleted before flow runs, so `reg_n_sets`
 * never reaches 2 and the doubling is never escaped.
 *
 * Which is the general statement, and it is why this paragraph should stop
 * being read as an open suggestion: a second set only survives if it computes
 * a DIFFERENT value, and a different value costs an instruction. The
 * large-constant pointer in the paragraph above escapes the doubling precisely
 * because its `lui`/`ori` pair is work that has to happen anyway -- that is a
 * property of the CONSTANT, not something a parameter or an ordinary local can
 * be given. Checked separately and worth knowing: the parameter pseudo really
 * does carry the note on this target, `REG_EQUIV (mem:SI (reg:SI 0 $0))` on
 * the `(set (reg/v:SI 80) (reg:SI 4 a0))`, even though the argument arrives in
 * a register -- so the doubling diagnosis itself is right, it is only the cure
 * that is unavailable.
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
