/* func_800565D8 (unk_2879C) -- 31 of 31 words, 3 DIFF. NOT compiled.
 *
 * The two-sided scale: a byte at +0x21 scaled by how far `n` is below 0x40
 * (negated), a byte at +0x20 scaled by how far it is above (positive), zero at
 * the boundary and zero whenever the byte is zero.
 *
 * WHAT IS PAID FOR
 *
 * Both bytes are VOLATILE. The target loads each one TWICE -- once for the
 * zero test, once for the multiply -- and cse forwards the tested byte to the
 * multiply in every non-volatile spelling, which is where this function's
 * earlier 30/31 came from. `*(volatile u8 *)` on the second read restores both
 * loads and fixes the word count. That lever is matching-levers section 5; it
 * landed func_80036F24, func_80037090 and func_80037394 the same day.
 *
 * One result variable, so both arms share the final `sll`/`sra` pair, and
 * `return 0` written as its own statement in all three zero cases, so none of
 * them reaches that pair.
 *
 * WHAT IS LEFT: TWO SITES, SIX WORDS
 *
 * 1. Arm one, four words. The target emits
 *
 *      beqz $v0, .L80056618
 *       addiu $v0, $zero, 0x3F     <- the constant, in the branch delay slot
 *      lbu   $v1, 0x21($a0)
 *      subu  $v0, $v0, $a1
 *      sll   $v1, $v1, 1           <- the doubling is on the BYTE
 *      mult  $v1, $v0              <- byte chain is mult operand 0
 *
 *    and no spelling measured gets all three of those at once. Writing the
 *    doubling as `<< 1` keeps it on the byte and gives the target's exact
 *    instruction ORDER, but then the two chains hold each other's registers
 *    and the mult's operand roles swap (4 DIFF, RE-MEASURED 2026-10-27 -- this
 *    header said 7, from before the volatile read was added):
 *
 *      r = -((0x3F - n) * (*(volatile u8 *) (p + 0x21) << 1));
 *
 *    Writing it as `* 2` or `2 *` lets fold move the doubling onto the
 *    subtraction -- wrong operand, and the `subu` then precedes the `lbu` --
 *    but the registers come out right and the constant reaches the delay slot,
 *    which is the 6 DIFF this file holds. The tension is that the delay slot
 *    wants the constant's subtree expanded FIRST (lower INSN_LUID, which is
 *    sched1's tie-break for the two equal-priority chains) while the mult's
 *    operand order and the register assignment want the byte's subtree first.
 *
 *    Measured, all at 31 words and 6-7 DIFF unless noted: `<< 1` and `* 2` on
 *    either side of the product, `2 * byte`, the product cast to `s32` and to
 *    `u32`, `* -1` instead of the unary minus, and the subtraction hoisted
 *    into its own local before the zero test (32 words). `-(0x3F - n) * byte`
 *    and `(n - 0x3F) * byte` collapse the `li`/`subu` pair into one `addiu`
 *    (25 words).
 *
 *    RE-MEASURED 2026-10-27, and the tension is now exactly located: it is
 *    WHICH CHAIN GETS $v0, and the source can have either the order or the
 *    registers but not both.
 *
 *      `-(byte << 1) * (0x3F - n)` (byte first, inline)   32 words: registers
 *          right (byte in $v1, the constant chain in $v0, `mult $v1,$v0`
 *          byte-first) but the `li 0x3F` no longer reaches the branch delay
 *          slot, because the body now opens with the `lbu`.
 *      `d = 0x3F - n;` then `-(byte << 1) * d`            31 words, 5 DIFF:
 *          order right AND operand roles right (`mult $v0,$v1`, byte first),
 *          registers swapped -- the byte takes $v0 because after scheduling
 *          its quantity is the shorter of the two (`advise.py --greg`'s
 *          local-alloc table, block b3: three quantities at 2 refs each, and
 *          qty_compare_1 orders them by live length).
 *      the same with `d * byte`, with both operands in locals either way, and
 *          with `* 2` instead of `<< 1`: 4, 4, 5, 5 DIFF.
 *
 *    So the remaining lever is whatever lengthens the BYTE's quantity or
 *    shortens the constant chain's, not the expression shape.
 *
 * 2. Arm two, two words plus the branch. The target's zero-byte test branches
 *    BACKWARD to the shared `j .L8005664C` / `addu $v0, $zero, $zero` block
 *    that arm one's test and the `n == 0x40` case also reach, which leaves its
 *    delay slot free for `addiu $v1, $a1, -0x40`. Here jump threading
 *    redirects that branch past the `j` to the epilogue, so reorg has to copy
 *    `addu $v0, $zero, $zero` into the delay slot and the subtraction lands
 *    after the `lbu` instead. Measured against: the guard inverted with the
 *    body as the `then` arm, and a single trailing `return 0` with both arms
 *    returning their product (33-34 words -- cross-jumping does not merge the
 *    two tails back).
 *
 * So the open question at both sites is the same one: which of two
 * equal-priority chains cc1psx scheduled first. Section 8 of matching-levers
 * has the statement-order levers that bear on it; none of the ones listed
 * there applies to a single expression's two operand subtrees.
 *
 * SINCE PARKED, 6 DIFF -> 3: `d = n - 0x40;` is computed into a local BEFORE
 * the `p[0x20] == 0` test. reorg fills a branch's delay slot from the insn
 * BEFORE the branch first, so with the subtraction inside the arm the slot
 * takes the shared `return 0`'s `addu $v0, $zero, $zero` from the branch
 * TARGET instead, which is what the listing does not have.
 *
 * What is left is three words: the listing's `* 2` lands on the BYTE
 * (`sll $v1, $v1, 1`, with the `lbu` ahead of the `subu`) and here fold moves
 * it onto the other factor (`sll $v0, $v0, 1` on `0x3F - n`). A temporary
 * pins it, as matching-levers says, but costs a word (32 words, 27 DIFF),
 * and so does `<< 1` in place of `* 2` on the first operand; `<< 1` as the
 * second operand is 31 words and 4 DIFF. So the constant's binding here is
 * reachable only at the cost of the word count, which usually means one more
 * reference somewhere else is what the original had.
 *
 * START FROM THE 4-DIFF VARIANT, NOT THIS ONE -- measured 2026-10-27. That
 * line above undersells it. `r = -((0x3F - n) * (*(volatile u8 *) (p + 0x21)
 * << 1));` is 31 words / 4 DIFF, and its four words are ALL REGISTER NAMES
 * with the structure EXACT:
 *
 *     target  li $v0,0x3F ; lbu $v1,0x21 ; subu $v0,$v0,$a1 ; sll $v1,$v1,1
 *     << 1    li $v1,0x3F ; lbu $v0,0x21 ; subu $v1,$v1,$a1 ; sll $v0,$v0,1
 *
 * -- the `lbu` is ahead of the `subu` and the shift is on the BYTE, which is
 * exactly what this file's 3-DIFF form gets wrong structurally (it shifts
 * `0x3F - n` instead, because fold moves the 2 across the multiply). The
 * `mult $v1, $v0` word is identical in both, which is why the diff count is
 * larger while the function is closer. A 4-word register swap on an exact
 * structure beats 3 words with the constant bound to the wrong factor.
 *
 * THAT PRICING WAS ON THE WRONG PSEUDOS -- corrected 2026-10-27, from the
 * dumps rather than from advise.py's tables. It said the byte was pseudo 82
 * (3 refs / 4 live / 0.750, $v0) and the subtraction pseudo 83 (2 / 5 / 0.400,
 * $v1), and concluded the subtraction needed +2 references. Both rows are
 * GLOBAL allocnos that live elsewhere in this function. The registers that
 * differ belong to two BLOCK-LOCAL quantities, and `lreg` names them:
 *
 *     Register 89 used 2 times across 6 insns in block 3
 *     Register 91 used 2 times across 3 insns in block 3; 1 bytes
 *
 * 89 is the `0x3F - n` chain, 91 is the volatile byte -- and 91 is ONE BYTE,
 * where advise.py's local table takes every quantity as 4. Priced properly
 * with `qty_size` in bytes (local-alloc.c:380): 91 is 1 * 2 * 1 / 3 = 0.667
 * and 89 is 1 * 2 * 4 / 6 = 1.333. So the SUBTRACTION ALREADY OUTRANKS THE
 * BYTE two to one, and on priority alone it would take the lower register
 * first -- which is exactly what the listing has.
 *
 * This build gives the byte $v0 regardless, so the byte is being allocated in
 * local-alloc's EARLIER phase: `qty_sugg_compare` orders the quantities that
 * carry a hard-register suggestion and they are all assigned before the
 * unsuggested ones, whatever their priority. The lever is therefore that
 * SUGGESTION -- find what copies the byte's quantity to or from $v0 and break
 * it -- and no reference count on either side will do anything.
 *
 * Measured against that variant and NOT working: the subtraction through the
 * function-scope `d` before the byte test (32 words, 26 DIFF) or after it
 * (31 / 4); through a block-local `e` in its own braces (31 / 4); the byte
 * factor written first in the product, with `* 2`, `<< 1` or a `(u32)` cast
 * (all 32 words / 27 DIFF); `(u32)` and `(s32)` casts on the second operand
 * (31 / 3, i.e. back to the structural problem); `byte + byte` instead of
 * `byte * 2` (33 / 31); and the subtraction written `(s32) 0x3F - (s32) n`
 * (31 / 4).
 *
 * 2026-10-28: THE TWO PRIORITY LEVERS ARE NOW MEASURED INERT, which retires
 * the "whatever lengthens the BYTE's quantity or shortens the constant chain's"
 * line above and leaves the SUGGESTION reading as the only one standing.
 *
 * Against the 4-DIFF variant, the subtraction chain was given the free-
 * reference split that landed func_8004D294 -- `c = 0x3F; c -= n;` into a
 * block-local, which compiles to the same `li`/`subu` pair. It works as
 * intended on the numbers: `.lreg` then reads `Register 84 used 4 times across
 * 5 insns in block 3`, i.e. 4 refs over 5, priority 6.400 -- the HIGHEST of any
 * quantity in that block, against the byte's 0.667 and its shift's 4.000. And
 * `.greg`'s dispositions are unchanged: `84 in 3`, `90 in 2`, `91 in 2`. The
 * byte keeps $v0, the chain keeps $v1, 31 words, 4 DIFF.
 *
 * So qty_compare_1 DOES NOT DECIDE THIS PAIR, and no reference count or live
 * length on either side will move it -- which is what the suggestion phase
 * predicts, since it is assigned before priority is consulted at all. Three
 * more probes agree:
 *
 *   * reading the byte into a block-local BEFORE the `li` (so its range is the
 *     longer of the two as well as the split being in place): 31 words, 4.
 *   * the byte through a local shared by BOTH arms, which makes it a global
 *     allocno: `.greg` gives it neither a $v0 conflict nor a preference and it
 *     still takes $v0 -- 4 DIFF with the `li` first, and 32 words / 27 DIFF if
 *     the volatile load opens the block, because then the `li` cannot reach
 *     the branch delay slot (the volatile MEM is not a legal filler).
 *   * routing the subtraction through `r`, the returned variable -- whose
 *     allocno really does carry the $v0 copy preference from the return -- is
 *     7 DIFF: the preference does not win $v0 either, and the mult result, the
 *     negate and the `mflo` all follow the chain into $v1.
 *
 * EXHAUSTION: a full `tools/decomp/search.py` sweep over every transform
 * family, 17 candidates across 15 families, run 2026-10-28 ON THE 4-DIFF
 * VARIANT rather than on this file, improved on nothing.
 *
 * No phantom test was tried. It would fit the shape -- see the section 3
 * entry -- but on a 31-word function that is a poor trade for two
 * references, and the SYMPTOM note there applies: if the subtraction really
 * is two references short, something else about this function's context is
 * still wrong.
 */

#include "common.h"

s16 func_800565D8(u8 *p, u32 n) {
    s32 r;
    s32 d;

    if (n < 0x40) {
        if (p[0x21] == 0) {
            return 0;
        }
        r = -((0x3F - n) * (*(volatile u8 *) (p + 0x21) * 2));
    } else if (n == 0x40) {
        return 0;
    } else {
        d = n - 0x40;
        if (p[0x20] == 0) {
            return 0;
        }
        r = (*(volatile u8 *) (p + 0x20) << 1) * d;
    }
    return r;
}
