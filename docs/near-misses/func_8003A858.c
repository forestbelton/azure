/* func_8003A858 (unk_72A8) -- 43 of 43 words, 2 DIFF. NOT compiled.
 *
 * Written out of the `func_8003A858 + func_8003A060` carved-sweep row on
 * 2026-10-27 so both have a source. THE TWO LISTINGS ARE BYTE-IDENTICAL, so
 * this file and its twin differ only in the function name, and anything found
 * for one settles both.
 *
 * The func_8003A2E0 family: reads the destination index and the table index
 * with `*(*cp)++`, then the four operand bytes off `r = *cp`, advances the
 * cursor by 4, and writes the little-endian word they address into
 * `state[a].unk_48` as FOUR ACCUMULATING STORES -- cse forwards each re-read
 * of the slot, so `*(u32 *) (t + 0x48) += q[k] << n` is the shape, and the
 * listing shows `sw` after every step.
 *
 * WHAT IS PAID FOR, from the landed func_8003A1A4 and this row:
 *
 *   * the table index times 4 in ITS OWN LOCAL right after the read
 *     (`m = **cp * 4;`), inline in the operand sum it is 17 DIFF;
 *   * the destination as `a * 4 + (u32) s`, not `s + a * 4` -- the `addu`
 *     operand order, 8 DIFF the other way;
 *   * `m` written at the END of the operand sum, 11 DIFF at the front;
 *   * the cursor read through `u8 **cp = (u8 **) (p + 0x1C);` with
 *     `*(*cp)++`, which is the family-wide idiom.
 *
 * WHAT IS LEFT: TWO WORDS, one adjacent pair. The target emits
 * `sll $t0, $t0, 2` -- the DESTINATION index -- before the operand chain's
 * last `addu`, this source after it:
 *
 *     target  ... ; sll $t0, $t0, 2 ; addu $a1, $a1, $t1 ; lbu $v1, 0x0($a1)
 *     built   ... ; addu $a1, $a1, $t1 ; sll $t0, $t0, 2 ; lbu ...
 *
 * All 120 permutations of the five statements between the reads were run with
 * perm.py (best 2 DIFF, the same pair), and three more placements of the
 * destination computation were measured here -- before the operand sum, with
 * the state read hoisted beside it, and both after the cursor advance: 7, 7
 * and 2. So the pair is not statement order. Both insns are ALU, both in one
 * priority group, so this is sched1 within priority 1 -- the same family as
 * func_80035E58's last two words and probably as unreachable.
 *
 * NOT REACHABLE BY STATEMENT ORDER -- and that is ALL this shows. Measured
 * 2026-10-27 against gcc's own sched.c. Read the limits at the end of this
 * note before concluding anything stronger: the target IS output of this
 * compiler from some source, so a source that produces it almost certainly
 * exists and the question is only which shape it has. The two insns are 54 (the
 * operand chain's last `addu`, which adds `m`) and 68 (the destination index's
 * `sll`). In sched1 BOTH are bumped to LAUNCH_PRIORITY -- the ready list reads
 * `68 (7f000001) 54 (7f000001) 61 (2)` at T-20 -- and the 2026-10-28 note
 * below says exactly what that bump is -- so what decides them is the
 * key BELOW priority, and it is not LUID. `rank_for_schedule` (sched.c:2399)
 * classifies each candidate against the LAST-SCHEDULED insn: class 1 if that
 * insn is DATA dependent on the candidate, 2 if anti/output, 3 if independent,
 * and it takes the HIGHEST class. The insn after this pair is `lbu $v1, 0($a1)`,
 * which depends on the `addu` for $a1 -- so the `addu` is permanently class 1
 * and the `sll` class 3, and the `sll` wins every time, whatever their LUIDs.
 *
 * WHAT THE BUMP IS, 2026-10-28, read out of sched.c rather than inferred from
 * the dump. Both insns are BIRTHING INSNS. `schedule_block`'s launch logic
 * adjusts every insn's priority as it becomes ready by how many registers DIE
 * in it (>>= 3 at four or more, >>= 2 at three, >>= 1 at one or two), and at
 * ZERO deaths, if `birthing_insn_p` holds, it assigns `max_priority`
 * (sched.c:2539). `birthing_insn_p` (sched.c:2469) is a SET whose destination
 * is an already-live REG with **`reg_n_sets` == 1**. And `max_priority` is set
 * to `MAX (INSN_PRIORITY (ready[0]), INSN_PRIORITY (insn))` (sched.c:2574)
 * where `insn` is the one being scheduled, whose priority was just set to
 * LAUNCH_PRIORITY -- which is why a birthing insn prints as exactly 7f000001
 * and why TWO of them can, so the dump reading above is right.
 *
 * THAT GIVES THE LEVER A NAME: `reg_n_sets`. Un-bump either insn and the other
 * wins on priority before the class key is ever consulted -- and un-bumping
 * the `sll` is what this function wants, since the bumped insn is taken first
 * in the backward pass and so placed LAST. Both attempts at it are recorded
 * above and both fail for reasons that are now explicable rather than
 * mysterious: `a = a * 4;` in place is expanded as a temp plus a copy, and cse
 * propagates the temp into `a`'s single later use, so `reg_n_sets` returns to
 * 1; and splitting the index into its own statement does un-bump it but gives
 * it a much smaller LUID, which moves it seven slots instead of one. So the
 * shape wanted is a second set on the shift's destination that cse CANNOT
 * propagate away -- which needs the destination to have another use, and in
 * this source it has exactly one.
 *
 * The only key above class is PRIORITY, and it cannot be moved here. Priority
 * is the longest dependence path from the block start, and `insn_cost` is 1
 * between ALU insns against the `- 1` in `priority()` (sched.c:1488) -- so a
 * chain of adds and shifts does not accumulate priority AT ALL. It only grows
 * across a LOAD delay (cost 2). Both insns sit at 3 for that reason: the
 * `sll` is lw(1) -> lbu(2) -> sll(3), the `addu` is lbu(2) -> sll(3) ->
 * addu(3) -> addu(3) -> addu(3). Lengthening either side by one needs an extra
 * LOAD on that path, which is an extra instruction.
 *
 * Also measured, and the reason the obvious reading of section 8's new
 * birthing lever does NOT apply: splitting the index into its own statement
 * (`a4 = a * 4;` with `t = (u8 *) (a4 + (u32) s);`) to give the `sll` a
 * smaller LUID moves it 7 slots, not 1 -- 8 DIFF after statement 1, 7 after
 * 2, 3 and 4, and 2 (unchanged) after 5, 6 and 7. Once the `sll` is not the
 * last of its group it loses every subsequent tie and drifts to the front of
 * the whole operand block. And `a = a * 4;` in place does NOT un-bump it:
 * cse propagates the copy back out, so `reg_n_sets` returns to 1 and the
 * ready list still shows `7f000001`. See the note on that in
 * docs/matching-levers.md section 8.
 *
 * WHAT THIS ARGUMENT DOES NOT COVER, written down because the first draft of
 * it said "unreachable" and meant only this:
 *
 *   * Priority IS sensitive to LOAD delays -- that is the whole reason it is
 *     insensitive to ALU chains. An extra load on the `addu`'s path was waved
 *     off above as an extra instruction, but the target and this build are
 *     both 43 words, so a shape that changes WHICH values arrive from memory
 *     rather than from a register could move priority at constant instruction
 *     count. Not explored. This is the first thing to try.
 *   * The whole argument assumes THIS dependence graph. Every item under WHAT
 *     IS PAID FOR is pinned by measurement, not by proof; narrower field
 *     types (a `lw` becoming `lbu`/`lhu`), a different cursor idiom or a
 *     different declaration for the state struct rewrite the graph, and the
 *     class argument says nothing about those.
 *   * The decision traced here is SCHED1's. The emitted order is sched2's,
 *     which was inferred to follow from sched1's output through the LUID
 *     tie-break rather than traced in its own ready lists.
 *
 * So: no permutation or local respelling of THIS source reaches it, the key
 * that decides it is not one statement order can touch, and the escape is a
 * different dependence graph -- not a different ordering of this one. *
 * This note's first draft said "unreachable". docs/where-we-are.md already
 * warns against exactly that, in the paragraph headed "Do not read BOUNDED as
 * unreachable" -- these bytes came out of cc1psx from a C file, so a source
 * that reproduces them EXISTS, and every "unreachable" in this repo is really
 * "unreachable given everything else this project assumes": the unit boundary,
 * each symbol's declared type and size, the -G value, the struct layouts. The
 * same page records that "argument preferences are unreachable" once parked
 * two functions on a reason that turned out to be half true, and
 * func_80037E58's carved-sweep row is a sched2 choice written up as
 * unreachable that then LANDED on a declaration change. Read the limits above
 * as the live list they are.
 * 2026-10-28: "THE FIRST THING TO TRY" HAS NOW BEEN TRIED, and it does not
 * move. The note above leaves one opening -- that priority is sensitive to
 * LOAD delays and insensitive to ALU chains, so a shape changing WHICH values
 * arrive from memory could move priority at constant instruction count. Seven
 * shapes, every one of them 43 of 43 words so the instruction count really is
 * constant:
 *
 *     `s` and `t` both computed right after `a` is read      8 DIFF
 *     `s` alone moved up, `t` left in place                  2 DIFF (base)
 *     no `t` local -- the stores index `*(u32 *)(s + a * 4 + 0x48)`   2 DIFF
 *     `t` written `(u32) s + a * 4` instead of `a * 4 + (u32) s`     11 DIFF
 *     the `* 4` moved onto the read, `a = *(*cp)++ * 4;`      8 DIFF
 *     `m`'s `* 4` deferred to its use in `q` instead         9 DIFF
 *     BOTH multiplies moved that way                         2 DIFF
 *
 * The last row is the informative one. Moving either multiply alone is 8 or 9,
 * moving both is 2 -- the same as the base -- so what the schedule is
 * sensitive to is the two multiplies' position RELATIVE TO EACH OTHER, not
 * where either sits in the block. That is consistent with the account above
 * (the swap is between `sll $t0, $t0, 2` and `addu $a1, $a1, $t1`, and $t1 is
 * the other multiply's result) and it says the load-delay opening is not a way
 * in: none of these changes which values come from memory, because every one
 * of them leaves the same four `lbu`s and the same `lw`s in place. To use the
 * opening properly something would have to change a FIELD'S WIDTH or where a
 * value is stored, which is the second bullet of the "what this argument does
 * not cover" list -- a different dependence graph, not a different arrangement
 * of this one. Still 2 DIFF, still the floor.
 *
 * 2026-10-28: AND IT WAS CONFIRMED ON THIS FILE, not just assumed from the
 * identical listings. Four of the seven shapes were re-measured against
 * func_8003A858's own target and score the same to the word: the two 2-DIFF
 * ones (no `t` local; both multiplies moved) and the two 8-DIFF ones (`s` and
 * `t` hoisted; the `* 4` moved onto the read). So a measurement on either
 * file is a measurement on both, and whichever lands first lands the other --
 * 86 words, not 43.
 *
 * 2026-10-28, THE TIE IS NOW NAMED AT INSN LEVEL, and that closes the class
 * route. The two swapped words are, from the `sched2` dump:
 *
 *     insn 54  (set (reg a1) (plus (reg a1) (reg t1)))   the `+ m` addu
 *              priority = 3, ref_count = 4
 *     insn 68  (set (reg t0) (ashift (reg t0) 2))        the `a * 4` sll
 *              priority = 3, ref_count = 1
 *
 * `rank_for_schedule` (sched.c) has exactly three keys, in order:
 *
 *   1. PRIORITY, higher first. Both are 3. CORRECTED: `priority()` iterates
 *      an insn's LOG_LINKS -- its PRODUCERS -- so priority is the depth from
 *      the block START, not the distance to the end, and the dump bears that
 *      out (insn 13 is priority 1, insn 90 is 6). The `- 1` in the comment
 *      "when all instructions have a latency of 1 ... all instructions will
 *      end up with a priority of one" means only LOAD delays add depth. So
 *      priority(68) = priority(insn 17, the `lbu` of `a`) + 1 = 3, and
 *      priority(54) = max(insn 26, insn 52) + 0 = 3, both ALU-fed. Raising 68
 *      needs a SECOND load in `a`'s derivation, and `a = *(*cp)++;` has one.
 *      Lowering 54 needs a shallower producer chain, and it sits at the end of
 *      the sum. (The scheduler also runs BACKWARD, which is why key 3 reads
 *      `INSN_LUID(y) - INSN_LUID(x)`: the higher LUID is taken first and
 *      placed LATER.)
 *   2. The DEPENDENCE CLASS relative to `last_scheduled_insn`: class 3 is
 *      "independent of it, OR has latency of one", class 1 is data dependent,
 *      class 2 anti/output dependent, and the HIGHEST class wins.
 *   3. INSN_LUID, lower first -- i.e. source order.
 *
 * KEY 2 CAN NEVER BITE HERE, which is the new result. For insn 54 to fall to
 * class 1 it must be data dependent on the last scheduled insn with a latency
 * above one -- that means being fed by a LOAD. Its two LOG_LINKS are
 *
 *     insn 26  (set (reg t1) (ashift (reg t1) 2))    `m * 4`
 *     insn 52  (set (reg a1) (plus (reg a1) (reg v1)))  the r[3]<<24 term
 *
 * both ALU, both cost 1, so insn 54 is class 3 against either of them forever.
 * Measured, trying to put a byte LOAD last in the sum so the final addu would
 * consume it: `(r[1]<<8) + (r[2]<<16) + (r[3]<<24) + m + r[0]` is 20 DIFF,
 * `m + ... + r[0]` is 8, and `... + m + (r[3]<<24)` is 4. All 43 words. The
 * operand order also fixes the emitted SHIFT order, which is already right, so
 * every reordering that would demote the addu breaks something worth more.
 *
 * SO IT IS KEY 3, AND KEY 3 IS ALL-OR-NOTHING. The addu wins today only
 * because `q = ...` precedes `t = ...` in the source, giving it the lower
 * LUID. Giving the `sll` the lower LUID instead does not move it one slot --
 * it makes the sll win EVERY tie in its priority group, so it drifts to the
 * front of the whole operand block, which is the 7-and-8-DIFF column the
 * `a4 = a * 4;` split already measured at positions 1-4. The listing's
 * position for it -- second to last, immediately before the addu -- is
 * reachable from neither key. That is a complete account of the two words and
 * it says the remaining escape is a different dependence GRAPH, not a
 * different order within this one.
 */

#include "common.h"

void func_8003A858(u8 *p) {
    u8 **cp = (u8 **) (p + 0x1C);
    u32 a;
    u32 m;
    u8 *r;
    u8 *q;
    u8 *s;
    u8 *t;

    a = *(*cp)++;
    m = **cp * 4;
    (*cp)++;
    r = *cp;
    q = (u8 *) (r[0] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24) + m);
    *cp += 4;
    s = *(u8 **) (p + 0x80);
    t = (u8 *) (a * 4 + (u32) s);
    *(u32 *) (t + 0x48) = q[0];
    *(u32 *) (t + 0x48) += q[1] << 8;
    *(u32 *) (t + 0x48) += q[2] << 16;
    *(u32 *) (t + 0x48) += q[3] << 24;
}
