/* func_8004A574 (unk_1C540) -- 41 of 41 words, 5 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep row on 2026-10-27 so there is a source to
 * sweep.
 *
 * Scales a halfword of `D_80073414[p[1]].unk_0C + p[0] * 20` -- +0x10 or
 * +0x12 by the second argument -- by `D_800713CC[e[3]]` and passes it to the
 * function pointer at `e + 8`, if that is non-null. Both index chains are
 * `x * 20` written `(x * 4 + x) * 4`, which is what gcc emits for the
 * multiply; the struct is 20 bytes.
 *
 * THE ARM ORDER IS `w == 0` FIRST, worth three words: the listing's
 * `bnez $a1, .L8004A5E0` branches to the +0x12 arm and falls through into the
 * +0x10 one, so the `w == 0` body is the fall-through and must be written
 * first. As `if (w != 0) { +0x12 } else { +0x10 }` the test comes out `beqz`
 * and the two arms swap: 8 DIFF.
 *
 * WHAT IS LEFT: FIVE WORDS, and they are the `mflo`'s position with the copy
 * that follows from it. The target has
 *
 *     mflo  $v0                      <- in the JOIN block
 *     lw    $v1, 0x8($a2)
 *     beqz  $v1, .L8004A608
 *      addu $a0, $v0, $zero          <- the copy, in the delay slot
 *     jalr  $v1
 *
 * where this source emits the function-pointer load first and sinks the
 * `mflo` past the branch into the call block, straight into $a0, so there is
 * no copy to make:
 *
 *     lw    $v1, 0x8($a2) ; nop ; beqz $v1 ; nop ; mflo $a0 ; jalr
 *
 * Measured and not working: the function pointer read into a local AFTER the
 * if/else (identical), a second `u = v` local for the argument (identical),
 * and the row's own list -- both arms as if/else and as a ternary, the
 * operands of the multiply swapped (7), the halfword into its own local with
 * one multiply after the `if` (17), the multiply split from the load (12),
 * the pointer hoisted into a local before the test, an early `return` instead
 * of a guarded block, an `s32` product and a temporary for the argument (all
 * 5), plus a full search.py sweep of 18 variants.
 *
 * 2026-10-28: IT IS NOT A SCHED QUESTION, and `--greg` says so. The six global
 * allocnos are
 *
 *     107  3 refs /  5 live / 0.600  -> $v1   the function pointer
 *     84   3 refs /  6 live / 0.500  -> $a0   THE PRODUCT `v`
 *     85   3 refs /  6 live / 0.500  -> lo
 *     82   4 refs / 19 live / 0.421  -> $a2   `e`
 *     83   3 refs / 10 live / 0.300  -> $a3   `r`
 *     80   4 refs / 58 live / 0.138  -> $t0   `p`, THE PARAMETER
 *
 * and `v` holds $a0 because it is the call's FIRST ARGUMENT: the copy into
 * $a0 gives it a copy preference, and `find_reg` applies that preference over
 * its lowest-legal pick. Once `v` owns $a0 there is nothing for the `mflo` to
 * write but $a0, so it goes where the argument is set up -- after the branch.
 * The listing has `v` in $v0 and spends the copy, which is only possible if
 * $a0 is NOT available to it. So the mflo's position is DOWNSTREAM of that
 * register, exactly as func_8004827C's shift turned out to be, and reading the
 * sched1 ready list would be reading the consequence.
 *
 * WHY $a0 IS AVAILABLE HERE AND NOT IN THE LISTING. $a0 is caller-saved, so
 * this is settled in `find_reg`'s pass 0 where a reservation does bite
 * (matching-levers section 3, the class rule) -- but `v` prefers $a0 itself,
 * and `prune_preferences` subtracts an allocno's own preferences from what it
 * inherits from a conflicting rival, so no reservation can exclude it. The
 * other route is a CONFLICT: $a0 would have to be live inside `v`'s range,
 * which means the PARAMETER's pseudo holding $a0 -- and `p` is allocated LAST
 * here, at 0.138, because its 58 live is `update_equiv_regs`' doubling of a
 * once-set parameter (its real range is 29). Even with the doubling escaped it
 * is 0.276 against `v`'s 0.500, and it would need eight references to pass it.
 *
 * So the open question is narrower than this header had it: get the PARAMETER
 * allocated before the product, or give the product's copy into $a0 a reason
 * to survive. Measured already and not working: a second `u = v` local for the
 * argument (the copy coalesces, since `v` dies at it).
 *
 * So the question is only what pins the `mflo` above the function-pointer
 * load. Both are in the join block; nothing in the source orders them,
 * because the multiply's result and the load are independent. Read the sched1
 * dump (tools/decomp/sched.py) for the join block and see which of the two
 * the ready list prefers -- this is the same "memory over ALU inside one
 * priority group" family as func_80035E58's last two words, and if that is
 * what it is, the answer is that it is not reachable from the source.
 *
 * THAT IS THE WRONG QUESTION -- 2026-10-27, from the `.greg` dump. This is an
 * ALLOCATION fact, and the scheduling follows from it.
 *
 * The five words are not a reordering, they are a MISSING COPY. Put the two
 * streams side by side:
 *
 *     target  mult ; mflo $v0 ; lw $v1,8($a2) ; nop ; beqz $v1 ;
 *             move $a0,$v0 (DELAY SLOT) ; jalr $v1
 *     built   mult ; lw $v1,8($a2) ; nop ; beqz $v1 ; nop (DELAY SLOT) ;
 *             mflo $a0 ; jalr $v1
 *
 * The listing keeps the product in $v0 and copies it into the argument
 * register in the branch's delay slot; this build writes `mflo` straight into
 * $a0 and has nothing left to fill the slot with, so it emits a `nop`. Same
 * five words either way.
 *
 * And `--greg` says exactly why:
 *
 *     ;; 84 preferences: 4
 *     ;; 84 conflicts: 80 82 83 84 2 5 29 64 66
 *
 * pseudo 84 -- the product -- PREFERS $a0, inherited from the copy into the
 * call's argument register (global.c:809 propagates copy preferences across
 * copies), and $a0 (4) is NOT in its conflict set, so it takes it. With the
 * product already in $a0 the copy has nowhere to exist, and the empty delay
 * slot is a CONSEQUENCE of that, not an independent scheduler choice.
 *
 * So this is carved-sweep's missing-copy family, not the "memory over ALU"
 * family: the product and the argument are ONE quantity here and TWO in the
 * listing. And the reason `v` already having two sets (one per arm) does not
 * help is now settled: a COPY CAN DISAPPEAR TWO WAYS. combine/cse can
 * substitute the value away, and a second set prevents that; or the ALLOCATOR
 * can give both pseudos the same register, so the copy reaches reload as
 * `$a0 = $a0` and is deleted there -- and `reg_n_sets` is never consulted when
 * `find_reg` honours a preference. This is the second kind, so set counts are
 * irrelevant to it (matching-levers section 3 now carries the distinction).
 *
 * THE REQUIREMENT, stated exactly: $a0 must be LIVE during the product's live
 * range, so that the product CONFLICTS with it. $a0 holds the multiplier `s`
 * at the `mult` and dies there, and the product's range begins at the `mflo`
 * just after -- so the two never overlap. Either `s` has to still be live
 * after the multiply, or the product has to be live from before it. Neither
 * is available in this source as written, and that is the shape to hunt,
 * not another statement order.
 */

#include "common.h"

extern s32 D_800713CC[];

void func_8004A574(u8 *p, s32 w) {
    u8 *e;
    u8 *r;
    s32 s;
    s32 v;

    e = (u8 *) D_80073414 + p[1] * 20;
    r = (u8 *) *(u32 *) (e + 0xC) + p[0] * 20;
    s = D_800713CC[e[3]];
    if (w == 0) {
        v = *(u16 *) (r + 0x10) * s;
    } else {
        v = *(u16 *) (r + 0x12) * s;
    }
    if (*(u32 *) (e + 8) != 0) {
        (*(void (**)(s32, u8 *)) (e + 8))(v, p);
    }
}
