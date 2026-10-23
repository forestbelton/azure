/* func_80042900 (unk_14934) -- 1 instruction off, 34 words against 33.
 *
 * The loop, both early returns, the re-sign-extension of arg1 after the loop,
 * the 0xE arm AND (new, 2026-10-22) the register the dead load lands in are all
 * byte-exact. What is left is a single duplicated `move $v0, zero`.
 *
 * The target's 0xD arm is a single `lw $v1, 0x14($a0)` whose result is never
 * used: the function returns 0 on that path, from the `addu $v0, $zero, $zero`
 * that reorg hoisted into the `bne`'s delay slot. So the original performed a
 * load for its own sake and discarded it.
 *
 * A non-volatile dead load is deleted outright by flow.c's insn_dead_p (tested:
 * the whole arm vanishes), so the read has to be volatile.
 *
 * SOLVED: why the load got $v0 and how to give it $v1. Written as a bare
 * statement, the load's destination is referenced exactly once, in one block,
 * so it is a block-local QUANTITY and local-alloc assigns it -- before
 * global-alloc, looking only inside that block, where $v0 is free because the
 * return value's `$v0 = 0` is set in a different block. It takes $v0, the first
 * register in REG_ALLOC_ORDER, and the return value then has to be
 * re-established afterwards.
 *
 * Assigning the load to a variable that lives across blocks makes it a global
 * ALLOCNO instead, and every global allocno in this function conflicts with
 * hard register 2 ($v0), so find_reg skips straight to $v1. The variable to
 * reuse is the one holding the sign-extended arg1 that both tests compare
 * against -- which is $v1 in the target, and costs nothing, because the
 * `t = arg1;` sign extension is the `sll`/`sra` pair the target already has:
 *
 *     s32 t = arg1;
 *     if (t == 0xE) { ... }
 *     if (t == 0xD) { t = *(volatile u32 *) (arg0 + 0x14); }
 *
 * That took it from 6 DIFF to 5. The general lever is now in
 * docs/matching-levers.md section 3: a one-block value is only out of reach
 * while it has its own pseudo.
 *
 * WHAT IS LEFT is one `move $v0, zero` too many. Mine:
 *
 *     bne $v1,$v0,.L97C / move $v0,zero / lw $v1,0x14($a0) / move $v0,zero / jr
 *
 * against the target's
 *
 *     bne $v1,$v0,.L97C / addu $v0,$zero,$zero / lw $v1,0x14($a0) / jr
 *
 * The join label has several other predecessors (the 0xE arm's `j`, and two
 * exits from the loop), so it is not an `own_thread_p` for reorg, which
 * therefore COPIES its first insn into the delay slot and must leave the
 * original behind. In the target only the slot's copy exists, so pre-reorg
 * there was exactly one `$v0 = 0` insn serving both sides of that branch, and
 * it cannot have been before the `bne`, because $v0 holds the 0xD comparison
 * constant there. The likely shape is therefore an arm that is its own thread,
 * ending in its own `return 0`, with nothing at the join at all -- which is not
 * what any `return 0` spelling produces, because jump2 cross-jumps the arm's
 * `move`/`jr` tail into the shared one before reorg ever runs.
 *
 * Ruled out, all still 34 words with the duplicate (and all with the `t` reuse
 * above, which is kept):
 *   - `s32 r = 0;` as the arm's first statement with `return r;` as its last --
 *     the insn does reach the delay slot, but the join's copy stays.
 *   - the same using the loop counter instead of a fresh local.
 *   - `if (t != 0xD) { return 0; }` with the read after it, so both returns are
 *     written separately (5 DIFF, identical output).
 *   - an explicit `return 0;` inside the arm (5 DIFF, identical output).
 *   - no final return at all, falling off the end of the 0xD arm: 11 DIFF, gcc
 *     emits a second `jr` and reorders the tail.
 *   - the arm owning the ONLY `return 0` and the not-0xD path falling off the
 *     end of the function -- the shape the paragraph above argues for -- in
 *     both spellings: `s32 r = 0;` first with `return r;` last, and a plain
 *     `return 0;` after the read. Both 8 DIFF. The duplicate does go away, but
 *     gcc pays for the undefined path elsewhere in the tail, so this is worse
 *     than the 5 DIFF source below, not better. If the shape is right, the
 *     cost has to come off somewhere else.
 *   - `volatile s32 *` instead of `volatile u32 *`; assigning the read to a
 *     dead local of its own (both pre-date the `t` reuse and are now moot).
 *
 * Reusing one `r` for the loop's loaded byte and the 0xD arm also puts the load
 * in $v1, but writing the loop that way (`r = arg0[i * 2 + 0x2D]; if (r != 0)
 * return r;`) flips the inner branch arms and costs more than it saves (9
 * diffs). The `t` reuse above is the cheap version of the same idea.
 *
 * SEARCHER SWEEP (2026-10-22): tools/decomp/search.py found 11 candidate
 * changes inside this function's body and compiled 12 variants; none improved
 * on 5 DIFF. Read that as "the eight transform families in search.py found
 * nothing at the sites they recognise" -- NOT as "every source shape has been
 * tried", which is a much stronger claim than 12 variants can support. The
 * families are declared widths, adding and dropping an (s16) cast, splitting
 * and joining a statement, `<` against `>`, the `& 1` mask, and duplicating
 * the statement after an if/else into both arms.
 */

#include "common.h"

s32 func_80042900(s8 *arg0, s8 arg1) {
    s32 i;
    s32 t;

    if (arg1 != 0) {
        for (i = 3; i >= 0; i--) {
            if (arg0[i * 2 + 0x2C] == arg1) {
                if (arg0[i * 2 + 0x2D] != 0) {
                    return arg0[i * 2 + 0x2D];
                }
                return 0x100;
            }
        }
    }
    t = arg1;
    if (t == 0xE) {
        return ((u8 *) arg0)[0x28] == 0;
    }
    if (t == 0xD) {
        t = *(volatile u32 *) (arg0 + 0x14);
    }
    return 0;
}
