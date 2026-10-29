/* func_8003C67C (unk_72A8) -- 31 of 31 words, 5 DIFF. NOT compiled.
 *
 * 2026-10-28: 28 DIFF -> 5, AND THE PARK'S CENTRAL QUESTION IS ANSWERED. Every
 * note below this one was written against a 28-DIFF source whose loop was
 * STRENGTH-REDUCED, and they circle one question -- "the loop must not be a
 * loop to loop.c AND must not spend a word on the guard", which they call "the
 * real content of this park". That is now done, the loop is unreduced at the
 * right word count, and the remaining 5 are a single register swap.
 *
 * FIRST, THE BENEFIT ARITHMETIC THE OLD NOTES WERE WORKING IS HOPELESS, and
 * knowing that saves the next session the whole `copy_cost` thread. loop.c's
 * gate is one line (loop.c:3822):
 *
 *     if (v->lifetime * threshold * benefit < insn_count && ! bl->reversed)
 *         v->ignore = 1;                     -- i.e. do NOT reduce
 *
 * and it PRINTS the comparison. This function's `.loop` dump says
 * `giv of insn 36 not worth while, 0 vs 9.`, so `insn_count` is 9, while
 * `threshold = (loop_has_call ? 1 : 2) * (3 + n_non_fixed_regs)` is about 58
 * on this target. The givs that get reduced carry benefit 13 after combining
 * at lifetime 1, i.e. **754 against 9**. No `copy_cost` charge (worth 8) can
 * bridge that, and the only giv that IS skipped is skipped because its benefit
 * reached exactly 0. loop.c cannot be PERSUADED not to reduce this loop; it
 * can only be prevented from seeing it.
 *
 * FOUR THINGS GET IT TO 5, and each is worth words on its own:
 *
 *  1. THE GOTO LOOP, which is why loop.c never sees a loop. The old notes
 *     priced goto forms at 32 words, one too many -- but they were all paired
 *     with a shared-address body. Combined with the three below it is free.
 *  2. THE BASE LOCAL IS DECLARED INSIDE THE GUARDED BLOCK, not at function
 *     scope and not used by the save. That is what produces the listing's
 *     `addu $a2, $a0, $zero` -- the base copied into the loop's own register --
 *     and it is the difference between 30 words and 31. With the base also
 *     feeding the save it is 32.
 *  3. THE DESTINATION GOES THROUGH A POINTER LOCAL THAT CAPTURES THE OLD
 *     INDEX, with `i--` between it and the source subscript:
 *     `d = &p[i]; i--; d->unk_00 = p[i].unk_00;`. That is what makes gcc
 *     compute TWO addresses -- the listing's two `sll ..., 3` with the
 *     decrement between them -- instead of sharing one and reaching the source
 *     with a -8 displacement. Written `p[i] = p[i - 1]; i--;` the two
 *     subscripts are linearly related and gcc shares: 29 or 30 words.
 *  4. THE SAVE, THE RESTORE AND THE LOOP BODY ARE ALL FIELD BY FIELD, not
 *     struct assignments. This REVERSES the old note's "the struct assignment
 *     is what holds the word count ... and that idea is closed" -- that was
 *     measured against the reduced loop, where it was true. Here each one is
 *     worth several words, and for two separate reasons:
 *       * the RESTORE field by field gives the listing's symbolic first store,
 *         `sw $v0, %lo(D_8006B200)($v1)` followed by `addiu $v1, $v1, %lo`
 *         for the second word, where the struct assignment materialises the
 *         whole address first: 28 -> 22.
 *       * the SAVE field by field gives the listing's INTERLEAVED
 *         `lw ; sw ; lw ; sw` where the struct assignment batches
 *         `lw ; lw ; sw ; sw`: 22 -> 14. The old note called this
 *         "sched2 ordering two independent lw/sw pairs ... no spelling of the
 *         copy reaches it"; field by field reaches it.
 *       * the LOOP BODY field by field, on top of the pointer local: 14 -> 5.
 *
 * WHAT IS LEFT: FIVE WORDS AND ONE SWAP, with the margin measured. Every
 * opcode, immediate, displacement and branch offset matches (advise.py says
 * ALLOC). $v1 and $a0 are exchanged between two quantities:
 *
 *     ours    sll $v1,$a1,3 ... lw $a0,0($v0) ; addu $v1,$a2,$v1 ; sw $a0,0($v1)
 *     target  sll $a0,$a1,3 ... lw $v1,0($v0) ; addu $a0,$a0,$a2 ; sw $v1,0($a0)
 *
 * From `.lreg`: pseudo 107 is the destination address (3 refs, live 8,
 * priority 1.500) and pseudo 101 is the first loaded word (2 refs, live 6,
 * 1.333). 107 is allocated first and takes $v1, the lowest free register, and
 * 101 falls to $a0. **To flip them pseudo 101 needs ONE more reference or ONE
 * less of live length** -- `advise.py --greg` puts the gap at 20%, needing
 * 3 refs or live <= 5. Measured and not reaching it: the two loaded words
 * through named locals, one shared local for both (30 words), splitting the
 * destination sum (`k = i * 8` then the add, or `d = p; d = &d[i];`), and five
 * spellings of the destination address -- `&p[i]`, `p + i`, `i + p`,
 * byte-cast arithmetic in both operand orders, and an integer base -- all
 * 5 DIFF, because gcc canonicalises the sum's operand order whatever the
 * source says.
 *
 * ALSO MEASURED AND WORSE, so the four above are each load-bearing: the
 * destination as an array reference with the old index in a variable
 * (`j = i; i--; p[j]...` -- 32 words, the `j` copy), both subscripts as array
 * references with `j = i - 1` (22), the source through the symbol while the
 * destination goes through `p` (33), two base locals (32), the base local at
 * function scope or feeding the save (30, 23), and `--i` inside the source
 * subscript (30 -- it does give two addresses and an unreduced loop, but
 * without the base copy).
 */

#include "common.h"

void func_8003C67C(s32 i) {
    Unk8006B200 t;

    t.unk_00 = D_8006B200s[i].unk_00;
    t.unk_04 = D_8006B200s[i].unk_04;
    if (i > 0) {
        Unk8006B200 *p = D_8006B200s;
        Unk8006B200 *d;

    loop:
        d = &p[i];
        i--;
        d->unk_00 = p[i].unk_00;
        d->unk_04 = p[i].unk_04;
        if (i > 0) {
            goto loop;
        }
    }
    D_8006B200s[0].unk_00 = t.unk_00;
    D_8006B200s[0].unk_04 = t.unk_04;
}
