/* func_80056DB4 (unk_1C540) -- 23 of 23 words, 2 DIFF. NOT compiled.
 *
 * Written out of the `func_80056DB4 + func_800563B0 + func_80056D44` row on
 * 2026-10-27 so all three have sources (see the sibling files).
 *
 * Fills the parameter block at D_80084918 -- flags 0x60100 at +4, zeros at
 * +0x3A and +0x3C, 5 at +0x24, and the D_80073740 handle at +0 -- and hands
 * its address to func_8005F134.
 *
 * THE ADDRESSING RECIPE, shared by all three siblings: the base is a POINTER
 * LOCAL initialised in its declaration (`u8 *q = D_80084918;`), which
 * materialises it first, while the +0 store goes through the SYMBOL
 * (`*(u32 *) D_80084918 = h;`) and so keeps the listing's symbolic
 * `sw $a0, %lo(D_80084918)($a1)` against the shared `(high)` pseudo. The
 * D_80073740 lookup goes into its own local, also in a declaration. A struct
 * type for the block is NOT the answer: as COMPONENT_REFs the +0 store folds
 * onto the register base and the function is a word short (21 DIFF).
 *
 * WHAT IS LEFT: TWO WORDS, and the store ORDER that gets there is not the
 * listing's. perm.py over all 120 orderings of the five stores (run
 * 2026-10-27) ranks them
 *
 *     1 4 3 2 5   2 DIFF   <- the order below: +4, +0x24, +0x3C, +0x3A, +0
 *     1 4 2 3 5   3 DIFF
 *     1 2 3 4 5   4 DIFF   <- the listing's own order
 *
 * so the +0x24 store comes BEFORE the two halfword stores and the two
 * halfwords are written in the REVERSE of the order they are emitted in. Both
 * are stores of zero to independent fields, so the order is free in the
 * source and only the score is evidence either way.
 *
 * THE PARAGRAPH THAT USED TO BE HERE WAS STALE -- re-read the diff before
 * trusting a WHAT IS LEFT section. It said the lookup's `lw` and the
 * `addiu $v0, $zero, 0x5` were still after the two `sh`s. They are not: this
 * source already emits `lw` at word 12 and `li 5` at word 13, exactly as the
 * listing does. Re-measured 2026-10-27.
 *
 * WHAT IS ACTUALLY LEFT is the ORDER OF THREE STORES, words 14 and 16:
 *
 *     target  ... lw ; li 5 ; sh 0x3A ; sh 0x3C ; sw 0x24 ; sw +0
 *     built   ... lw ; li 5 ; sw 0x24 ; sh 0x3C ; sh 0x3A ; sw +0
 *
 * and the two sides are COUPLED, which is why 120 orderings of the stores
 * cannot get both. Measured:
 *
 *   * STORES FOLLOW SOURCE ORDER. A store's destination is a MEM, so
 *     `birthing_insn_p` never fires for it (it requires a REG dest) and the
 *     store keeps its LUID position. The three come out in exactly the order
 *     they are written.
 *   * THE LOOKUP'S LOAD IS BUMPED, so it floats as late as its consumer
 *     allows -- and where it lands depends on which store comes FIRST among
 *     the three. Written +0x24 first (this file) the load lands ahead of all
 *     three, because +0x24 stores a REGISTER value and its `li 5` has to be
 *     pulled ahead of it, and the load rides along. Written in the listing's
 *     own store order -- 0x3A, 0x3C, 0x24, which is what the output needs --
 *     the first two stores need only $zero, nothing is pulled ahead, and the
 *     `lw`/`li` pair lands AFTER them: 4 DIFF, the stores right and the pair
 *     two slots late.
 *   * THE LOOKUP'S POSITION IS IRRELEVANT in that order. Seven placements
 *     measured -- declared before `q`, declared after `q`, and as a bare
 *     statement before each of the five stores -- all 23 words, all 4 DIFF.
 *     sched1 normalises it.
 *   * UN-BUMPING THE LOAD by making its destination multi-set does not work
 *     here. The only second set available to a straight-line function is the
 *     PARAMETER's incoming copy (`i = D_80073740[i];`), and reusing `i`
 *     lengthens its live range enough to move the base pointer out of $v1
 *     into $a1/$a2: 10 DIFF in both store orders.
 *
 * So the next idea has to un-bump that load, or give one of the two halfword
 * stores a register-valued source, WITHOUT lengthening any live range -- not
 * reorder anything. SYMPTOM: nothing in this file is implausible as original
 * source; the +0x24-first store order is the only oddity and it is worth 2
 * words against the listing's own order.
 *
 * 2026-10-28: THE REQUIREMENT IS NOW EXACT, and it is TWO un-bumps, not one.
 * Read out of this function's own sched1 ready lists in the LISTING's store
 * order (0x3A, 0x3C, 0x24, which scores 4 DIFF with everything else right):
 *
 *     ;; ready list at T-5: 31 (1) 28 (1) 25 (1) 34 (7f000001) 20 (7f000001)
 *
 * where 20 is the lookup's `lw`, 34 is the `li 5`, and 25/28/31 are the three
 * stores. The two bumped insns win every round and are placed LATE; the
 * stores, which can never be bumped (a MEM destination fails
 * `birthing_insn_p`), lose until T-11..T-13 and so are placed FIRST -- which
 * is exactly the 4 words wrong. For the listing's order the pick sequence has
 * to be 36, 31, 28, 34, 20, 18, 16, 13, 12, 25, so BOTH 34 and 20 must come
 * down to priority 1 and the `li 5` must also be generated BEFORE the two
 * `sh`s (its LUID has to be below theirs while its store stays above them,
 * which needs the 5 in a named local).
 *
 * ALL THREE WAYS OF BUYING A SECOND SET COST MORE THAN THE TWO WORDS:
 *
 *   * `u32 v = 0x60100;` reused for the 5 (`v = 5;`) does un-bump the `li`
 *     and the three stores DO move to the right side of it -- the order
 *     requirement is met -- but the shared quantity takes $a2 where the
 *     listing has $v0 and everything downstream shifts: 15 DIFF, in all three
 *     positions of `v = 5` and with the lookup in a declaration or a
 *     statement. Note the listing's own `lui $v0`/`ori $v0` and
 *     `addiu $v0, $zero, 0x5` share $v0 anyway, so that register reuse is the
 *     allocator's and NOT evidence of a shared variable.
 *   * `i = D_80073740[i];` un-bumps the `lw` and is the only second set a
 *     straight-line function has for it, but it moves `h` out of $a0 (the
 *     listing loads into the parameter's own register, `lw $a0, 0x0($a0)`):
 *     10 DIFF alone, and 15 combined with the shared `v` above, where the
 *     emitted ORDER is right and five registers are wrong.
 *   * A DEAD INITIALISER BUYS NOTHING. `u32 h = 0;` / `u32 n = 0;` before the
 *     real assignments is 23 words and the same 4 DIFF, and the dumps say why
 *     rather than leaving it inferred: `.rtl` and `.jump` carry four
 *     `(const_int 0)`s, `.flow` and `.combine` carry two, so FLOW ITSELF
 *     deletes the dead sets, and the sched1 ready list still reads
 *     `34 (7f000001) 31 (7f000001)`. See the refinement this produced in
 *     matching-levers section 3: the free-reference lever needs an insn
 *     removed AFTER flow, not before it.
 *
 * AND 432 VARIANTS SAY THERE IS NOTHING ELSE ON THESE AXES. Swept 2026-10-28:
 * the 5 inline / in a fresh local / reusing `i` / sharing the flags local, the
 * flags inline or in a local, `h` in its own local or reusing `i`, the lookup
 * in a declaration or a statement, all six orderings of the three stores, and
 * every position of the 5's assignment among them. The best is the 2 DIFF this
 * file already has (reached by several of them, including `i = 5`), and nothing
 * beat it.
 *
 * SO WHAT A SOLUTION LOOKS LIKE, stated tightly enough to recognise: a second
 * set on the `li 5`'s pseudo and on the lookup's, both surviving into flow's
 * `reg_n_sets`, neither lengthening a quantity's live range. This function has
 * no branch, so it has no `if` arms and no loop-carried variable -- the two
 * constructs that produce such a set for free -- and the two it does have
 * (the parameter's incoming copy, and one local doing double duty) are both
 * priced above. That is the whole of it; do not re-run store orders.
 */

#include "common.h"

void func_8005F134(u8 *);

void func_80056DB4(s32 i) {
    u8 *q = D_80084918;
    u32 h = D_80073740[i];

    *(u32 *) (q + 4) = 0x60100;
    *(u32 *) (q + 0x24) = 5;
    *(s16 *) (q + 0x3C) = 0;
    *(s16 *) (q + 0x3A) = 0;
    *(u32 *) D_80084918 = h;
    func_8005F134(q);
}
