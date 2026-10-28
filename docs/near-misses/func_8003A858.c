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
 * `68 (7f000001) 54 (7f000001) 61 (2)` at T-20 -- so what decides them is the
 * key BELOW priority, and it is not LUID. `rank_for_schedule` (sched.c:2399)
 * classifies each candidate against the LAST-SCHEDULED insn: class 1 if that
 * insn is DATA dependent on the candidate, 2 if anti/output, 3 if independent,
 * and it takes the HIGHEST class. The insn after this pair is `lbu $v1, 0($a1)`,
 * which depends on the `addu` for $a1 -- so the `addu` is permanently class 1
 * and the `sll` class 3, and the `sll` wins every time, whatever their LUIDs.
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
