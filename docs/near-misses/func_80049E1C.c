/* func_80049E1C (unk_1C540) -- 20 of 20 words, 3 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep row on 2026-10-27, and ten words better
 * than the row's 18: it had this at 16-18 words of 20, three or four short,
 * and "needs three or four more copies than any of six shapes produces".
 *
 * The wrapping clamp, func_80049DE8's family: adds the delta, then clamps at
 * 0 going down and at `m - 1` going up, and answers 0 when `m` is 0.
 *
 * THREE THINGS BUY THE MISSING WORDS, and all three are block LAYOUT or the
 * conditional-expression form rather than arithmetic:
 *
 *   * BOTH arms are conditional expressions, not `if`s. `a = (a < 0) ? 0 : a;`
 *     is what landed func_80049DE8, and the same form on the OTHER arm
 *     (`r = (a >= m - 1) ? m - 1 : a;`) is what pays for the rest of the
 *     copies -- a plain `if` optimises them away. With the positive arm as an
 *     `if` the function is 17 of 20 words; with both as expressions it is 20.
 *   * THE `m == 0` ANSWER GOES LAST: `if (m != 0) { ... return r; } return 0;`
 *     and not `if (m == 0) return 0;` first. The listing's `beqz $a2` jumps
 *     to the function's LAST block, so the zero answer is the one placed at
 *     the end -- worth 8 differing words (16 against 8).
 *   * THE POSITIVE ARM IS THE FALL-THROUGH: `if (d >= 0) { clamp high }
 *     else { clamp low }`, because the listing's `bltz $a1` branches to the
 *     low-clamp arm. Written `if (d < 0)` first it is 12.
 *
 * A separate `r` for the result is needed in any case: returning from each
 * arm directly is 17 of 20 words (10 DIFF), because the two returns then
 * share one copy.
 *
 * WHAT IS LEFT: eight words, and they are which register each arm's value
 * lands in. The target puts the high clamp's value in $a0 and the low
 * clamp's in $v1, with $v0 as the common result; this source uses $a0 for
 * both. Writing the high clamp as a plain `if` on `r` (variant i in the
 * measurements) is also 8 and lands the same way, and a `q = m - 1` local is
 * 18 words. So the two arms want DIFFERENT result registers, which is an
 * allocation question -- read `advise.py --greg`.
 *
 * THE `--greg` READING, done 2026-10-27, and it closes the obvious direction.
 * From the `.greg` dump:
 *
 *     ;; 5 regs to allocate: 80 83 85 81 82
 *     ;; 80 preferences: 2 4      -> gets $v0   (the result `r`)
 *     ;; 83 preferences: 4        -> gets $a0   (7 refs, the busiest)
 *     ;; 83 conflicts: 82 83 85 2 29
 *     ;; 85 preferences: 6        -> gets $a2
 *     ;; 82 preferences: 6        -> also $a2 (82 and 85 do not conflict)
 *
 * Two things follow. 83 CONFLICTS with $v0 (2), so $v0 is illegal for it and
 * its $a0 is not a tie-break that more references would win -- and `r` (80)
 * already has the $v0 the listing gives it. The eight words are therefore
 * entirely about the LOW arm's value, which the listing puts in $v1 and this
 * build folds into 83's $a0.
 *
 * And that fold cannot be broken by adding a variable, which is the thing to
 * stop trying: the two arms' values live in DIFFERENT BLOCKS, so their live
 * ranges never overlap, they never conflict, and local-alloc ties them into
 * one quantity however many locals the source spells. Measured, all 20 of 20
 * words and 8 DIFF: a `s32 t` local in the low arm assigned from the ternary;
 * the same split into `t = a; if (t < 0) t = 0;`; and `r = a;` hoisted ahead
 * of the `d >= 0` test with each arm narrowing it. A local in the HIGH arm is
 * worse (17 words, 9 DIFF), and section 3's seventh cure does not apply --
 * there is no copy whose source could stay live past it, because the two
 * values are never simultaneously live.
 *
 * So the question is what gives ONE arm's value a preference or a conflict
 * the other lacks. On the evidence that is not reachable by naming variables.
 *
 * 2026-10-28, THREE MORE SHAPES, all of them WORSE, and they say the same
 * thing from the other direction: clamping `a` IN PLACE and copying it to `r`
 * removes the very copies the target spends.
 *
 *     high arm in place, low arm as now      17 words,  9 DIFF
 *     low arm in place, high arm as now      19 words, 14 DIFF
 *     both arms in place                     16 words, 20 DIFF
 *
 * So each arm needs its own temp that is NOT the parameter, which is what this
 * file already has -- and the open question stands. For the record of what the
 * target's shape actually is: it spends TWO copies in the high arm (one per
 * ternary branch, `addu $v0, $a0, $zero`) and ONE in the low arm
 * (`addu $v0, $v1, $zero`), with the high temp in $a0 and the low in $v1. For
 * the low temp to be $v1 rather than $a0 it has to be excluded from $a0 in its
 * own block, and the only thing that can do that is a conflict -- so the shape
 * to look for is one where the low arm's value is live at a point where $a0 is
 * (the incoming parameter at the top), without paying for the copy that gets
 * it there. Note that making it a cross-block allocno is the lever that landed
 * func_800565D8, but every placement tried so far keeps it inside one block.
 */

#include "common.h"

/* 2026-10-28: 8 DIFF -> 3, AND THE TWO THINGS THAT DID IT.
 *
 * Everything this file used to say about the two arms wanting different result
 * registers was right; what it had wrong was that the fix is not naming more
 * locals. Two changes, each measured on its own:
 *
 * 1. THE LOW ARM'S TEMP IS DECLARED INSIDE THE ELSE BLOCK, not at function
 *    scope. That alone makes the ENTIRE HIGH ARM match -- words 5..12, the
 *    `addu $a0,$v0,$zero` / `addiu $a2,-1` / `slt` / `bnez` / `addu $v0,$a0`
 *    / `addu $a0,$a2` / `j` / `addu $v0,$a0` run -- and fixes the `bltz`
 *    offset too, because the high arm is then 8 words as the listing has it
 *    rather than 7. With one function-scope `r` shared by both arms the m-1
 *    exit gets cross-jumped into the low arm's tail, which is what forced both
 *    temps into $a0. See "WHERE A VARIABLE IS DECLARED DECIDES WHICH ALLOCATOR
 *    SEES IT" in matching-levers section 3.
 *
 * 2. THE LOW ARM IS WRITTEN `s = 0;` THEN `if (a >= 0) { s = a; }` -- the
 *    zero unconditional and the copy conditional. Written the other way round
 *    (`s = a; if (a < 0) { s = 0; }`) the function is SEVENTEEN words, three
 *    short, because cse rewrites the test's operand from `a` to `s`, `a` then
 *    dies at the copy, `s` and `a` no longer conflict, global-alloc gives both
 *    $v0 and all three moves vanish. With `s = 0` first, cse cannot equate `s`
 *    with `a` at all, so the copy survives and `s` takes $v1 -- the listing's
 *    register.
 *
 * WHAT IS LEFT IS THREE WORDS, and they are one mirror image. The listing's
 * low arm is
 *
 *     bgez $v0, .L80049E58 ; addu $v1,$v0,$zero ; addu $v1,$zero,$zero
 *
 * i.e. `s = a` unconditional with `s = 0` under the branch; this source emits
 *
 *     bltz $v0, L          ; addu $v1,$zero,$zero ; addu $v1,$v0,$zero
 *
 * the same three insns with the branch sense and the two assignments swapped.
 * Words 1-12 and 16-20 are identical. So the original wrote the clamp the
 * other way round and did NOT lose the copy to cse, which is exactly what
 * point 2 says this source cannot do.
 *
 * AND THE cse RULE IS NOW READ OUT RATHER THAN INFERRED, which is where the
 * next attempt starts. `merge_equiv_classes` (cse.c:837-862) makes the NEW
 * register of an equivalence class the canonical one when
 *
 *     uid_cuid[regno_last_uid[new]]  > cse_basic_block_end
 *       || uid_cuid[regno_first_uid[new]] < cse_basic_block_start
 *     AND uid_cuid[regno_last_uid[new]] > uid_cuid[regno_last_uid[firstr]]
 *
 * -- "if NEW will live longer than any other reg of the same qty, and that is
 * beyond the current basic block, make it the new canonical replacement". `s`
 * qualifies on both counts: it is used after the join, so it outlives `a` and
 * its last use is past the block end. That is why the test is rewritten to `s`
 * and why `a` dies at the copy. So the shape to look for gives `a` a last use
 * no earlier than `s`'s -- a use of `a` after the join -- or keeps `s`'s live
 * range inside the block. Note this is a cse question, not an allocation one,
 * which is a correction to everything above the 2026-10-28 line.
 *
 * 2026-10-28, SECOND SESSION ON THE LAST THREE WORDS, and they are now a
 * single question with a single answer that this source cannot give.
 *
 * THE REGISTERS ARE ALREADY RIGHT. In the committed source the low arm emits
 * $v1 for the temp and `addu $v0, $v1, $zero` for the return, exactly as the
 * listing does, and words 1-12 and 16-20 are byte-identical. The whole
 * remaining difference is WHICH OF THE TWO ASSIGNMENTS IS UNCONDITIONAL:
 *
 *     listing  bgez $v0, L ; addu $v1,$v0,$zero ; addu $v1,$zero,$zero
 *     built    bltz $v0, L ; addu $v1,$zero,$zero ; addu $v1,$v0,$zero
 *
 * The delay-slot insn is the one reorg's backward scan took from before the
 * branch, so pre-reorg the listing is `s = a ; bgez a ; s = 0` and this source
 * is `s = 0 ; bltz a ; s = a`. A one-armed `if` is the only shape that gives
 * one unconditional assignment and one branch, so the listing's source is
 * `s = a; if (a < 0) { s = 0; }` and this one is `s = 0; if (a >= 0) { s = a; }`.
 *
 * AND `s = a` UNCONDITIONAL CANNOT SURVIVE. Every spelling of it is SEVENTEEN
 * words, three short, by the route in point 2 above. Tried and all 17 words:
 * the test on `a` and on `s`; `s` block-scoped, function-scoped, and with the
 * high arm's local in the other scope; both arms block-scoped; the high arm
 * returning its expression with no local at all; the high arm in-place on a
 * function-scope `r`; `s = a` hoisted ABOVE the `d >= 0` test (so that the
 * copy and the test fall in different cse blocks -- 17 words, 11 DIFF, gcc
 * sinks it); a `goto` form; `if (a < 0) { s = 0; return s; } return s;` and the
 * same with `return a;` (16 words, 20 DIFF); and
 * `if (a >= 0) { return a/return s; } s = 0; return s;`.
 *
 * Conversely every spelling whose UNCONDITIONAL assignment is the zero is 20
 * words and the same 3 DIFF -- `s = 0; if (a >= 0) s = a;`, `if (a > 0)`,
 * `if (!(a < 0))`, `if (a >= 0) { s = a; } else { s = 0; }`, and `u32 s`.
 * So the mirror is stable and the cost is exactly the three words.
 *
 * WHERE THAT LEAVES IT. The cse rule quoted above explains why `s = a` cannot
 * survive in THIS function: `s`'s last use is the return, which is past the
 * join label and therefore past `cse_basic_block_end`, and it outlives `a`
 * whose last use is the test. Both of `merge_equiv_classes`' conditions hold,
 * so `s` becomes the canonical register, the test is rewritten to `s`, `a` dies
 * at the copy, and the copy coalesces away. For the listing's shape, one of
 * those conditions has to fail -- `a` must have a last use no earlier than
 * `s`'s, or `s` must not be live past the join -- and neither is available: the
 * only insns after the low arm's return are the `m == 0` block and the shared
 * `jr`, and the clamp needs a join. So the next attempt should question an
 * assumption OUTSIDE the arms (the parameter types, the `m == 0` placement, or
 * whether `a += d` is really a reassignment of the parameter) rather than
 * respell the low arm again. That space is now swept twice over.
 *
 * MEASURED on the way, all with the high arm as above: both arms as ternaries,
 * as if/else, and as in-place clamps; per-arm returns and a shared `return r`;
 * one variable and two; the low arm's test on `a` and on `s`; `q = m - 1`
 * hoisted; `s = a` hoisted ahead of the `d >= 0` test; a goto form; and a
 * 139-variant cross product of all of those. Everything with a function-scope
 * low temp is 8 DIFF and everything with `s = a` first is 17 words.
 */

s32 func_80049E1C(s32 a, s32 d, s32 m) {
    s32 r;

    if (m != 0) {
        a += d;
        if (d >= 0) {
            r = (a >= m - 1) ? m - 1 : a;
            return r;
        } else {
            s32 s = 0;

            if (a >= 0) {
                s = a;
            }
            return s;
        }
    }
    return 0;
}
