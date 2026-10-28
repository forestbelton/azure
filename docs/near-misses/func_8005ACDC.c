/* func_8005ACDC (unk_2CDC0) -- 75 of 75 words, 3 DIFF. NOT compiled.
 *
 * FIRST ATTEMPT, 2026-10-27: this function had never been read (it was one of
 * the nine stubs with neither a park file nor a carved-sweep row of its own --
 * see docs/where-we-are.md). Four edits took it from 35 DIFF to 3.
 *
 * The release sweep: unless D_80073828 says otherwise, walks every live
 * D_80085458 entry, and for each whose +0x1A countdown is nonzero and whose
 * D_80073740 handle func_8005EB78 reports in state 3, either decrements the
 * countdown or -- once it has reached 2 -- collects the handle into a mask,
 * spins func_8005E97C/func_8005EB78 until the voice reports 2 or 0, and
 * clears the countdown. The collected mask is keyed off in one final call.
 *
 * WHAT IS PAID FOR
 *
 * * D_80073828 is read SIGNED (`lh`), through an `s16` __asm__ alias: the
 *   header's `extern u16 D_80073828[];` gives `lhu`. One word.
 *
 * * The GUARD TESTS `m`, not `i`. The listing's `slt $v0, $s3, $v0` names the
 *   mask's register, which holds 0 at that point, and `i = 0` is the
 *   `addu $s4, $s3, $zero` in the `beqz`'s own delay slot -- so `i = 0` is
 *   INSIDE the guarded block and the bound test compares `m`. With `i < bound`
 *   as the guard and `i = 0` above it, the copy is emitted before the branch
 *   and five prologue words shuffle with it.
 *
 * * The `v >= 2` body FALLS THROUGH and the decrement is the `else`. gcc
 *   computes `v < 2` and branches on it being true, so writing
 *   `if (v < 2) { decrement } else { big }` puts the big body first and the
 *   two arms and their shared tail come out in the wrong order: 15 words.
 *
 * * A SECOND ROW POINTER at offset zero for the trailing `+0x1A` store,
 *   declared after the inner do/while -- matching-levers section 2's giv
 *   lever, the one that landed func_80057D20. Without it the D_80073740 giv
 *   is initialised before the D_80085458 one and they take each other's
 *   registers ($s1/$s2 swapped), because the inner loop's hoisted copy of
 *   `h[0]` is the last giv created and so represents its group.
 *
 * WHAT IS LEFT: THREE WORDS, and they are the ORDER OF THREE ALU INSNS in the
 * loop's preheader. The listing has `addu $s4, $s3, $zero` (i = 0), then
 * `addiu $s5, $zero, 0x2` (the inner loop's hoisted constant), then
 * `lui %hi(D_80085458)`; this source emits the `lui` first and the other two
 * after. All three are priority 1 and all three are ALU, so `potential_hazard`
 * does not separate them and the choice is LUID -- and the `lui` here is
 * hoisted by move_movables (the symbol is referenced twice in the body, once
 * for `e` and once for `e2`) rather than being part of the giv's own
 * initialisation, which is what gives it the earlier LUID.
 *
 * MEASURED 2026-10-27, which prices the trade exactly. Dropping `e2` and
 * writing the trailing store through `e` is SEVEN words: the three preheader
 * words above become right in kind -- there is no movable left to hoist, so
 * each `lui` sits next to its own `addiu` -- but the two giv initialisations
 * then come out in the other order (D_80073740's first, D_80085458's second)
 * and so do the two increments at the loop bottom, which is four words plus
 * the one this trade was meant to save. That is the mechanism from both
 * sides: `e2` exists to make the D_80085458 giv the LAST one created, which
 * puts it FIRST in the preheader (giv initialisations are emitted in reverse
 * body order), and the price of the second reference is that the symbol's
 * high half becomes a loop movable and is hoisted ahead of everything.
 *
 * Also measured and not working: the second row through a base local assigned
 * inside the loop body (`base = D_80085458;` with both rows derived from it,
 * either declared in the body or at function scope) -- 3 DIFF, the movable is
 * still hoisted first; the trailing store written inline with the symbol
 * (80 words); and `e2` through an `__asm__` alias of the same symbol, which
 * does defeat cse but costs the second materialisation (82 words).
 *
 * So the thing to try next is a spelling that keeps the second row reference
 * WITHOUT giving the symbol a second materialisation to hoist: `e2 = e;`
 * loses the giv ordering and goes back to 18 DIFF, and a walked `e` biases
 * the giv to +0x1A (`lhu 0x0($s1)` where the listing has `lhu 0x1A($s1)`).
 * Measured on the way: the two arms in both orders (3 and 15), the guard on
 * `i` (18), `lhu` for D_80073828 (one more word differing), and no second row
 * pointer at all (30). A FULL `search.py` sweep over all twelve transform
 * families found nothing better than the base, so the remaining three words
 * are not a local source rewrite.
 *
 * WHAT DECIDES THE HOIST ORDER, 2026-10-28 -- and it explains why none of
 * those families reaches it. `scan_loop` APPENDS each movable as it discovers
 * it (loop.c:799-802, `last_movable->next = m`) and `move_movables` emits the
 * list in that order ahead of the loop, so THE PREHEADER'S ORDER IS THE ORDER
 * THE INVARIANTS ARE FIRST USED in the body -- the one used first ends up
 * furthest from the loop. Here the symbol's first use is the body's FIRST
 * statement (`e = D_80085458 + i * 0x78`) and the hoisted constant 2 is used
 * far deeper (`v >= 2`), so the symbol is discovered first and hoisted first.
 * That is exactly the `lui` the listing puts LAST:
 *
 *     target  move $s4,$s3 ; li $s5,2 ; lui $v0 ; addiu $s1,$v0,%lo
 *     built   lui $v0 ; move $s4,$s3 ; li $s5,2 ; addiu $s1,$v0,%lo
 *
 * So the shape to look for is one whose body touches the CONSTANT before the
 * SYMBOL. None of the twelve transform families does that -- they move
 * declarations, spellings and statement order among the existing uses, while
 * what decides this is which invariant is reached FIRST. That is a
 * restructuring of the body's opening statements, and it is not an allocation
 * question, so section 3's procedure does not apply (its step 1 already says
 * SHAPE here). See the section 9 entry.
 *
 * AND THAT REORDERING IS NOT AVAILABLE IN THIS SOURCE -- 2026-10-28, with the
 * constant identified. The hoisted 2 is loop.c's insn 220 in the `.loop` dump,
 * and its only use is insn 74, `(ltu (reg 83) (const_int 2))`, which is the
 * `v >= 2` test. `v` is `*(u16 *) (e + 0x1A)` -- read THROUGH the symbol. So
 * the data dependence forces D_80085458 to be touched before the 2 can be
 * used at all, and no permutation of these statements puts the constant first.
 *
 * Which makes the listing's order evidence about the ORIGINAL: it hoists the 2
 * ahead of the symbol, so the original must use the constant 2 somewhere that
 * does NOT depend on D_80085458 and that is reached earlier in the body. This
 * source has no such use -- its only 2s are `v >= 2` (dependent) and the inner
 * `k != 2` (deeper still). That is a statement about the function's shape
 * rather than its spelling, and it is the thing to look for: an early
 * comparison against 2 on something other than the row halfword.
 *
 * 2026-10-28: "A SPELLING THAT DOES NOT GIVE THE SYMBOL A MOVABLE TO HOIST"
 * DOES NOT EXIST, and the reason is structural rather than another thing to
 * measure. `move_movables` will hoist a loop invariant when any of three
 * conditions holds (loop.c:690-700), and the second -- "it is not a user
 * variable and it is not used in the exit test" -- is the one a source can
 * work, by giving the value a NAME so the pseudo carries REG_USERVAR_P. That
 * is a real lever and it landed nothing here, because the THIRD condition
 * already holds: the set is guaranteed to run once the loop starts. The shared
 * `%hi(D_80085458)` pseudo is set at `e`'s computation, which is the FIRST
 * statement of the do-body, before any branch -- and `maybe_never` only
 * becomes true at the first label or jump scan_loop walks past (loop.c:921).
 * So the movable is hoisted however it is spelt, which is exactly what the
 * `base = D_80085458;` measurements above found from the other end.
 *
 * The movable exists at all only because the symbol is referenced TWICE, and
 * the file already prices dropping the second reference at seven words. So
 * both ends of this trade are now closed.
 *
 * 2026-10-28: AND IT IS NOT A SCHEDULING QUESTION AT ALL -- THE PASS IS REORG.
 * Everything above, and every earlier note in this file, reasons about where
 * the three preheader insns are PLACED. They are already placed correctly.
 * Read off the dumps rather than the diff:
 *
 *     sched1 places the preheader as  29, 220, 213, 226, 217, 250
 *     sched2's chain reads            25(beqz), 29, 220, 213, 226, 217, 250
 *     the TARGET emits                25(beqz), [29], 220, 213, 226, 217, 250
 *     this build emits                25(beqz), [213], 29, 220, 226, 217, 250
 *
 * where 29 is `addu $s4, $s3, $zero` (`i = 0`), 220 is `addiu $s5, $zero, 2`
 * (the hoisted inner-loop constant), 213/226 are the `lui`/`addiu` pair for
 * D_80085458 and [brackets] mark the insn lifted into the `beqz`'s DELAY SLOT.
 *
 * So sched1 and sched2 BOTH already produce the listing's order, and the three
 * words are entirely the delay-slot filler's choice: the target lifts the
 * FIRST insn of the fall-through thread, this build reaches past it and lifts
 * the THIRD. Nothing about move_movables, LUID or priority is involved -- the
 * notes above are answering the wrong question, and they are kept only because
 * their measurements are still true of the source.
 *
 * WHICH REORG PATH, narrowed in reorg.c. It is not
 * `fill_simple_delay_slots`: its forward scan can never fill a conditional
 * jump, because the win is gated on `target == 0` and for a JUMP_INSN
 * `target = JUMP_LABEL (insn)` is set just above the loop (reorg.c:3004-3009,
 * 3056). Its BACKWARD scan has no liveness-at-target test at all, which is why
 * the other slots in this function hold whatever preceded their branch --
 * including callee-saved $s2 after jump 181. So this slot was filled by
 * `fill_eager_delay_slots` -> `fill_slots_from_thread`, whose candidate gate is
 *
 *     condition == const_true_rtx
 *     || (! insn_sets_resource_p (trial, &opposite_needed, 1)
 *         && ! may_trap_p (pat))                     -- reorg.c:3374-3376
 *
 * with `opposite_needed = mark_target_live_regs (insn_at_target)`. The loop
 * keeps scanning past a rejected insn whenever `own_thread` holds
 * (`(! lose || own_thread)`, reorg.c:3315), which is exactly the "reach past
 * the first two" behaviour seen here.
 *
 * IT IS A REJECTION, NOT A POSITION, and the same function proves it: at jump
 * 193 reorg takes insn 197, the FIRST insn of that fall-through. So it does
 * take the first when it can, and insns 29 and 220 were refused.
 *
 * WHICH GATE REFUSED THEM IS STILL OPEN, and the attractive answer is
 * probably the wrong one. The suggestive pattern is that every fall-through
 * fill in this function sets a CALLER-saved register ($v0 at jumps 25 and 75,
 * $a0 at jump 193) while the two refused insns set $s4 and $s5, and reorg does
 * add the epilogue's restored registers to `end_of_function_needs`
 * (reorg.c:4291-4299, whose comment says those registers "won't have been
 * processed by flow"). That is the same liveness mechanism as the
 * `$v0`-live-at-the-epilogue lever in matching-levers section 1, so it is the
 * natural guess. TWO THINGS ARGUE AGAINST IT:
 *
 *   * `end_of_function_needs` is only used when `mark_target_live_regs` is
 *     called with `target == 0` (reorg.c:2458). This target is a real insn in
 *     a real block, so the answer comes from
 *     `basic_block_live_at_start[b]` mapped through `reg_renumber` -- and flow
 *     ran before allocation, where the only pseudo live at the start of the
 *     post-loop block is `m`. `i` has no use after the exit test. So $s4
 *     should NOT be in `opposite_needed`.
 *   * the listing itself lifts an insn setting $s4 out of this very thread, so
 *     callee-saved cannot be a blanket bar.
 *
 * AND THE OTHER TWO GATES ARE NOW RULED OUT, so it IS the liveness test after
 * all. `eligible_for_delay` is MIPS's
 * `(define_delay (eq_attr "type" "branch") [(and (eq_attr "dslot" "no")
 * (eq_attr "length" "1")) ...])` (mips.md:125). `dslot` is "yes" only for
 * `type` in `branch,jump,call,load,xfer,hilo,fcmp` (mips.md:79-82), and
 * `movsi_internal2`'s attributes are
 * `type "move,load,arith,arith,..."` with `length "1,2,1,2,..."`
 * (mips.md:3298-3300) -- so a reg-to-reg move is type `move`, length 1, dslot
 * `no`, and `addiu $s5, $zero, 2` is type `arith`, length 1, dslot `no`. Both
 * are eligible. And `redundant_insn` is ruled out by arithmetic: when it fires
 * on an owned thread reorg DELETES the insn (reorg.c:3350-3367) and this build
 * is 75 of 75 words, so nothing was deleted.
 *
 * SO $s4 AND $s5 REALLY ARE IN `opposite_needed`, and the likely reason is
 * structural: THERE IS NO BARRIER BEFORE THE POST-LOOP LABEL, because the loop
 * falls through into it. `find_basic_block` (reorg.c) locates a block by
 * scanning BACKWARD to the previous BARRIER, so for this target it walks back
 * past the whole loop and the preheader, and `mark_target_live_regs` then
 * simulates FORWARD from that far-back block head through the entire loop body
 * to the target. `i` and the hoisted 2 are set inside that stretch. Whether
 * they survive to the target depends on the REG_DEAD notes the walk passes,
 * which is the thing to trace -- and it is also the lever, because a loop whose
 * last block ends in an unconditional jump WOULD put a barrier there.
 *
 * What is NOT the answer: putting `i = 0` where the backward scan
 * could reach it. Measured 2026-10-28, all 75 of 75 words:
 *
 *     `i = 0;` above the guard, guard on `m`      15 DIFF
 *     `i = 0;` above the guard, guard on `i`      15 DIFF
 *     `i = 0;` above `m = 0;`                     17 DIFF
 *
 * In the first of those the copy lands in the ENTRY block and the prologue's
 * register saves reorder around it; the `beqz`'s slot still takes the `lui`.
 * The target also has a `nop` in the guard's own block (the `lw` of
 * D_80073734's load delay), which is evidence that `i = 0` was NOT in that
 * block in the original either -- sched2 would have used it to fill that nop.
 *
 * SO THE NEXT STEP IS A REORG QUESTION, not a source sweep. In order of cost:
 * read `movsi_internal2`'s `dslot`/`length` attributes in config/mips/mips.md
 * (free); then, if those clear it, instrument the liveness by finding another
 * function in this repo whose conditional-branch fall-through slot our build
 * fills with an insn setting $s0-$s7 -- if none exists anywhere, the register
 * class IS the gate whatever reorg.c appears to say, and the question becomes
 * which register `i` gets rather than where the insns sit.
 *
 * AND THE GENERAL POINT, which is why this is worth the space: three earlier
 * notes in this file each concluded with a different scheduling mechanism, and
 * all three were about the placement of insns that were already placed
 * correctly. Before arguing about sched1 or sched2, diff sched2's CHAIN against
 * the emitted words. If they agree, the pass is reorg and no amount of
 * statement order will move it.
 */
#include "common.h"

extern s32 D_80073734a[] __asm__("D_80073734");
extern s16 D_80073828s[] __asm__("D_80073828");

void func_8005E97C(s32, s32);
s32 func_8005EB78(u32);

void func_8005ACDC(void) {
    s32 m;
    s32 i;
    s32 k;
    u32 v;

    m = 0;
    if (D_80073828s[0] == 0 && m < D_80073734a[0]) {
        i = 0;
        do {
            u8 *e = D_80085458 + i * 0x78;

            if (*(u16 *) (e + 0x1A) != 0) {
                if (func_8005EB78(D_80073740[i]) == 3) {
                    v = *(u16 *) (e + 0x1A);
                    if (v >= 2) {
                        u32 *h = &D_80073740[i];

                        m |= D_80073740[i];
                        do {
                            func_8005E97C(0, h[0]);
                            k = func_8005EB78(h[0]);
                        } while (k != 2 && k != 0);
                        {
                            u8 *e2 = D_80085458 + i * 0x78;

                            *(u16 *) (e2 + 0x1A) = 0;
                        }
                    } else {
                        *(u16 *) (e + 0x1A) = v + 1;
                    }
                }
            }
            i++;
        } while (i < D_80073734a[0]);
    }
    if (m != 0) {
        func_8005E97C(0, m);
    }
}
