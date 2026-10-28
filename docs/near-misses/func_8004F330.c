/* func_8004F330 (unk_1C540) -- 40 of 40 words, 9 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep row on 2026-10-27 so there is a source to
 * sweep. Fills n 0x10-byte slots from the four-word D_8002E5D8 template,
 * cross-links each slot's +0x240/+0x244 pair, and copies three words from
 * D_8002E5E8 into +0x204 after the loop.
 *
 * WHAT IS PAID FOR
 *
 * * `u32 *s = D_8002E5D8;` assigned INSIDE the loop body, so its
 *   materialisation lands in the preheader AFTER the `blez` (func_8004CCBC's
 *   lever), with the FIRST word read through the symbol
 *   (`lw %lo(D_8002E5D8)($t5)`) and the other three through `s`.
 * * `u32 *t = D_8002E5E8;` assigned AFTER the loop, or its base is hoisted
 *   ahead of the guard.
 * * The 0x2B8 store written BEFORE the 0x240/0x244 pair, even though the
 *   listing emits it after: four other orderings measured at 30-41 DIFF.
 * * THE p/q SPLIT IS THE REVERSE OF THE OBVIOUS ONE: `u8 *q = p;` as a
 *   declaration initialiser, the LOOP's stores through `p`, and the three
 *   TRAILING stores through `q`. The other way round -- loop through `q`,
 *   trailing through `p` -- is 40 words and 15 DIFF, and it gets the
 *   registers RIGHT (the loop addresses off $t3, as the listing has them)
 *   while losing the insn order inside the body; with everything through one
 *   of them the copy coalesces and the function is a word short (39).
 *
 * WHAT IS LEFT: NINE WORDS, and they are one allocation decision with its
 * knock-ons. The target copies the parameter to $t3 BEFORE the guard and uses
 * $t3 for everything, which frees $a0 as the scratch for the in-loop address
 * value `p + (0x2AC + i * 0x10)`:
 *
 *     addu $t3, $a0, $zero ; blez $a1 ; addu $a3, $zero, $zero
 *     ...
 *     addu $a0, $t3, $t2   <- the 0x2AC address, in $a0
 *     addu $v0, $t3, $t1   <- the 0x23C address, in $v0
 *
 * This source keeps `p` in $a0 and puts the copy in $t3, so both address
 * values go to $v0/$v1 and the copy is emitted after the guard. For the
 * target's assignment the 0x2AC address value has to be allocated BEFORE the
 * parameter -- read `advise.py --greg` and compare the two quantities, and
 * note that the 15-DIFF variant above already has the registers, so the
 * answer may be that variant plus whatever fixes its ORDER rather than this
 * one plus whatever fixes its registers.
 *
 * THE p/q SPLIT IS A DEAD END IN BOTH DIRECTIONS, and the reason is worth
 * having -- measured 2026-10-27.
 *
 * The listing uses $t3 for EVERYTHING: the preheader values (`addiu $t6, $t3,
 * 0x204`, `addu $a2, $t3`, `addu $t0, $t3`), the two in-loop addresses, AND
 * the three trailing stores (`sw $v1, 0x204($t3)`). There is no second
 * pointer in it at all. So `addu $t3, $a0, $zero` is not a `q = p` copy -- it
 * is the PARAMETER's own assign_parms copy, surviving because local-alloc gave
 * the parameter's pseudo $t3 instead of $a0. That is why neither split can
 * reach it: loop through `q` and trail through `p` leaves the preheader and
 * the tail on $a0 (15 DIFF, words 6/8/10 and 34/37/40), and one pointer
 * everywhere coalesces the copy away (39 words, a word short).
 *
 * 2026-10-28, THE PREFERENCE READING IS CONFIRMED and the class rule behind it
 * is now in matching-levers section 3: `regs_someone_prefers` is honoured in
 * `find_reg`'s PASS 0 only, and pass 0 can hand out exactly the registers in
 * `regs_used_so_far`, which global.c:352 seeds with every CALLER-SAVED
 * register. $a0 and $t0-$t2 are all caller-saved, so this function's contest
 * is settled in pass 0 and the reservation really does bite -- which is why
 * the three address values skip $a0. (The contrast is func_80048FBC, whose
 * contest is over $s0/$s1: pass 0 cannot give out the first callee-saved
 * register, so that one falls to pass 1 where the reservation is dropped and
 * only priority decides.)
 *
 * MEASURED 2026-10-28 and NOT working, both 39 of 40 words -- a word SHORT:
 * one pointer for everything with the copy made INSIDE the loop (`r = p;` as
 * the body's first statement, which is loop-invariant and gets hoisted to the
 * preheader for free), with and without a pre-loop initialiser. The point of
 * the shape was to keep the PARAMETER live past the copy so that
 * `expand_preferences` could not propagate its $a0 preference onto `r`; the
 * copy coalesces away regardless, which is the same wall as "one pointer
 * everywhere" below. And the conflict route to pruning the preference is not
 * available either: this function contains NO CALL, so $a0 is live only at
 * entry and nothing can conflict with it inside the parameter's range.
 *
 * WHY THE PARAMETER TAKES $a0 HERE, from `--greg` on the single-pointer form:
 * pseudo 80 (`p`) is 11 refs / 74 live / 0.446 and the three in-loop address
 * values are 7 refs each at 0.667, 0.636 and 0.583 -- so the address values
 * are allocated FIRST and still do not take $a0; they take $t0, $t1, $t2.
 * `p` takes $a0 afterwards because global-alloc records a COPY PREFERENCE on
 * it from the incoming parameter copy (`set_preference` in global.c), and a
 * copy preference is tried before the register order. CONFIRMED FROM THE DUMP
 * 2026-10-27 rather than inferred -- `/tmp/azure-decomp/dumps/<base>/greg`:
 *
 *     ;; 10 regs to allocate: 138 82 140 139 137 80 85 86 81 129
 *     ;; 80 preferences: 4
 *     ;; 81 preferences: 5
 *     ;; 137 conflicts: 80 81 82 85 86 129 137 138 139 140 2 3 29
 *
 * and the second half matters more than the first: the three address values
 * (140, 139, 137) are allocated BEFORE `p`, $a0 is LEGAL for them -- it is not
 * in their conflict sets -- and they still skip it, because
 * `prune_preferences` (global.c:833) puts every register preferred by a
 * conflicting LOWER-priority allocno into `regs_someone_prefers` and find_reg
 * avoids it. `p`'s preference RESERVES $a0. So the listing's assignment
 * requires the parameter not to hold that preference at all; no amount of
 * priority on the address values can take a reserved register. So this is a PREFERENCE
 * question, not a priority one, and the MARGINS table cannot price it -- every
 * "to flip: +N refs" row in it is about `allocno_compare`, which has already
 * been decided by the time the preference applies.
 *
 * WORDS 1 AND 3 ARE A DELAY-SLOT CHOICE, not an allocation one. Both builds
 * put one insn in the guard's delay slot; the listing puts `i = 0` there and
 * keeps the copy first, this build puts the copy there and keeps `i = 0`
 * first. dbr fills the slot from the insn immediately preceding the branch,
 * so what is needed is the copy emitted BEFORE `i = 0` in the pre-dbr stream.
 * Declaration order does NOT do it -- four variants measured, all 40 words /
 * 9 DIFF: `q` declared before `i`, `i` initialised in its declaration with a
 * bare `for (; i < n; i++)`, both together, and `i` declared last. `i = 0`
 * comes from the for-init, so loop.c fixes its position relative to the
 * guard, not the declaration list.
 *
 * So the next attempt is: deny the parameter its $a0 copy preference (read
 * `set_preference` and `find_reg` in global.c for what can), or move `i = 0`
 * after the copy without disturbing the guard shape. Both splits, all four
 * declaration orders, and the whole MARGINS table are now closed.
 *
 * 2026-10-28: `set_preference` AND `prune_preferences` ARE NOW READ, and there
 * is nothing in C that denies the preference. Four routes, all closed, with
 * the code for each:
 *
 *   * WHERE IT COMES FROM. `set_preference`'s second branch (global.c:1601)
 *     fires on `(set pseudo (reg a0))` whenever `reg_allocno[pseudo] >= 0`,
 *     and records BOTH a copy preference and a full preference. The incoming
 *     parameter copy is exactly that insn, so the preference exists for any
 *     parameter that global-alloc allocates.
 *   * DOWNGRADING IT DOES NOT HELP. `set_preference` records a plain rather
 *     than a copy preference when the SET's source is an expression rather
 *     than a bare register (`copy = 0` at the top of the function). It makes
 *     no difference here, because `regs_someone_prefers` -- the set `find_reg`
 *     avoids, global.c:953 -- is built from `hard_reg_full_preferences`
 *     (global.c:880), which carries both kinds.
 *   * MAKING $a0 ILLEGAL FOR `p` would prune the preference, and needs $a0
 *     live inside `p`'s range. In a function with NO CALL, $a0 is live only at
 *     entry; nothing in the source can extend it.
 *   * THE SIZE ESCAPE NEEDS A CALL TOO. `prune_preferences` drops a rival's
 *     preferences from `regs_someone_prefers` when the rival is not larger AND
 *     this allocno prefers the same registers (global.c:876). So an address
 *     value could take $a0 after all -- if it PREFERRED $a0 itself. A hard-reg
 *     preference comes only from a copy to or from that hard register, i.e.
 *     from an argument setup or a return value, and this function has neither.
 *
 * So for a PARAMETER used in more than one basic block, in a function with no
 * call, the incoming register is reserved and no choice of which pointer is
 * used where can change it. The one shape that removes the preference is a
 * parameter confined to ONE block -- `reg_allocno < 0`, so local-alloc owns it
 * and `set_preference` never fires -- and that is the same shape whose copy
 * coalesces away, measured above at 39 words. That is the fork, stated
 * exactly: the preference and the surviving copy cannot both be had by
 * changing pointers, and what would break it is something that gives an
 * address value its own $a0 preference.
 */

#include "common.h"

void func_8004F330(u8 *p, s32 n) {
    s32 i;
    u32 *t;
    u8 *q = p;

    for (i = 0; i < n; i++) {
        u32 *s = D_8002E5D8;

        *(u32 *) (p + 0x220 + i * 4) = (u32) (p + 0x23C + i * 0x10);
        *(u32 *) (p + 0x2AC + i * 0x10) = D_8002E5D8[0];
        *(u32 *) (p + 0x2B0 + i * 0x10) = s[1];
        *(u32 *) (p + 0x2B4 + i * 0x10) = s[2];
        *(u32 *) (p + 0x2B8 + i * 0x10) = s[3];
        *(u32 *) (p + 0x240 + i * 0x10) = (u32) (p + 0x2AC + i * 0x10);
        *(u32 *) (p + 0x244 + i * 0x10) = (u32) (p + 0x204);
    }
    t = D_8002E5E8;
    *(u32 *) (q + 0x204) = t[0];
    *(u32 *) (q + 0x208) = t[1];
    *(u32 *) (q + 0x20C) = t[2];
}
