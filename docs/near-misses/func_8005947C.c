/* func_8005947C (unk_2879C) -- 81 of 81 words, 6 DIFF. NOT compiled.
 *
 * The sequencer's event dispatch: reads a byte, decides whether it is a
 * running-status continuation or a new status byte, looks the status up in
 * D_800737DC and either calls the three-argument handler or one of three
 * special cases. Worked from an m2c draft; the word count was right on the
 * first compile and every shape finding below was paid for after that.
 *
 * WHAT IS PAID FOR
 *
 * * TWO variables hold the byte: `r`, the full s32 the read returns (the
 *   target's $a1, used for the `& 0x80` test, the store to +0x4A and the copy
 *   into `i`), and `b`, a `u8` copy of it (the target's $a0, used for every
 *   `== 0xFF` / `== 0xF0` comparison and for `c`'s initial value). One
 *   variable gives one register and loses the `addu $a0, $a1, $zero` the
 *   target puts in the first `bnez`'s delay slot.
 *
 * * `i` is `u32`, NOT `u8`. As a `u8` it is masked to 0xFF before the shift,
 *   which makes `& 0xF` redundant and emits `srl 4` / `sll 2` where the target
 *   has the folded `srl 2` / `andi 0x3C`.
 *
 * * `i = r` is the LAST statement of the status-byte arm. Written first, cse
 *   uses `i`'s register for the +0x4A store (`sb $s1` where the target has
 *   `sb $a1`) and the `beq`'s delay slot goes empty: 82 words.
 *
 * * The three special cases are a `switch` on `b`, not an if-chain. The chain
 *   gets the same 81 words but lays the three bodies out in source order,
 *   where the switch puts the 0xFF body first and the 0xF0 body after it,
 *   which is the order the listing has. 14 DIFF as a nested if-else with the
 *   arms already permuted to match; 12 as the switch.
 *
 * WHAT IS LEFT: TWO COUPLED ALLOCATION FACTS
 *
 * 1. `b` is in $a2 here and $a0 in the target. Six of the twelve words are
 *    that, including its knock-on: with `b` in its own register the tail's
 *    mask is `andi $v1, $a2, 0xFF`, which reorg hoists into the `beqz $s3`
 *    delay slot, while the target's is the IN-PLACE `andi $a0, $a0, 0xFF` --
 *    which cannot be hoisted, because $a0 is live down the other arm -- and
 *    the slot goes to `addiu $v0, $zero, 0xF0` instead. So this is one fact,
 *    not two: get `b` into $a0 and the mask, the slot and the two comparisons
 *    all follow. `b` crosses no call on either path, so every caller-saved
 *    register is legal for it and the choice is global-alloc's.
 *
 * 2. `t` and `c` hold each other's callee-saved registers: $s2/$s3 here,
 *    $s3/$s2 in the target. Both have three references and live ranges within
 *    an insn or two of each other, so they are a near-tie in global-alloc's
 *    priority and the tie breaks on allocno number, i.e. which appears first
 *    in the RTL. `t` does, which is why it takes $s2 here; for the target to
 *    have given $s2 to `c`, `c` must have outranked it -- four references
 *    rather than three would do it (floor_log2(4)*4 = 8 against
 *    floor_log2(3)*3 = 3), which would mean the original reads `c` once more
 *    than this does.
 *
 * Measured and NOT working: `u32 t`; `t` declared before `c`; `b` declared
 * before `r`; `c = r` instead of `c = b` (15 DIFF); `d = 0` after the read
 * rather than before (19); the parameter as `s32` with a `u8 *` local (82);
 * `s32 b` with `(b & 0xFF)` at every comparison and `b &= 0xFF` in the tail
 * (80 words -- the in-place mask DOES appear, and costs the +0x4A arm's
 * `andi`); and the whole tail nested inside `if (t == 0)` instead of after an
 * early return (37). *
 * 2026-10-27: 12 DIFF -> 6, AND FACT 2 IS SOLVED.
 *
 * `c` is assigned in the two arms of an INVERTED if/else --
 * `if (p[0x49] != 0) { c = b; } else { c = func_800589B8(...); }` -- instead
 * of `c = b;` followed by a one-armed `if`. That starts `c`'s live range
 * inside the arms rather than before the test, which is the one insn the
 * MARGINS table was asking for: `t` and `c` were 0.308 against 0.300, a 2.5%
 * gap with "live <= 9 (-1)" on `c`, and they now sit at 3 refs / 11 live /
 * 0.273 EACH -- an exact tie -- in the listing's own $s2 and $s3. Same word
 * count, six words gone.
 *
 * Measured alternatives for that block: the if/else with the condition the
 * natural way round is 7 DIFF (one worse, and it is the inversion that does
 * it); moving the `t == 2` block ahead of the `c` assignment is 75 or 60 DIFF
 * depending on the form, which is a different function.
 *
 * WHAT IS LEFT is only fact 1 -- `b` in $a2 here and $a0 in the listing, six
 * register-only words (advise.py now says VERDICT: ALLOC). Two things are now
 * known about it that the older note did not have:
 *
 *   * `b` DOES NOT CONFLICT WITH $a0. `--greg`'s conflicts column for its
 *     pseudo (82) reads `$v0 $v1 $sp` only, so $a0 is legal for it and this is
 *     not a live-range problem -- the range does not need shortening.
 *   * IT IS A PREFERENCE, and the `.greg` dump names it outright:
 *
 *         ;; 82 conflicts: 80 81 82 83 85 86 2 3 29
 *         ;; 82 preferences: 6
 *
 *     `b` PREFERS $a2 (6). It did not earn that itself -- global.c:809
 *     propagates copy preferences across copies, so `b` inherited it from `c`
 *     through `c = b`, and `c` prefers $a2 because it is the THIRD argument of
 *     func_8005914C. With no preference at all `b` would take $a0, because
 *     `find_reg`'s pass 1 walks hard registers in ascending number (mips.h
 *     defines no REG_ALLOC_ORDER) and $v0/$v1 are b's only conflicts below it.
 *
 *     So the lever is the COPY, not b's references or range. Measured and NOT
 *     working: assigning `c` from `r` instead of `b` -- `c = r`,
 *     `c = r & 0xFF`, `c = (u8) r` -- all three move the preference and all
 *     three cost three words elsewhere (9 DIFF against 6). What is wanted is a
 *     shape where `c` still prefers $a2 but `b` is not copied into it, or
 *     where `b` acquires a $a0 preference of its own.
 *
 * 2026-10-28: WHAT IT WOULD TAKE TO BLOCK THE PROPAGATION, read out of
 * `expand_preferences` (2.7.2 global.c:780-824) rather than inferred. The
 * propagation across `c = b` is NOT unconditional. It fires only when
 *
 *   1. the copy insn carries a REG_DEAD note FOR ITS SOURCE -- the loop walks
 *      `REG_NOTES` looking for `REG_DEAD` and does nothing otherwise; and
 *   2. the two allocnos DO NOT CONFLICT (both directions are tested).
 *
 * Both conditions say the same thing here: `b` has to be LIVE past the copy.
 * It is not, because every path through the `t != 0` block returns, so the
 * `switch (b)` below is unreachable from the copy and flow marks `b` dead on
 * it. Note the copy is in the listing too (`addu $s2, $a0, $zero`), so the
 * original has the same copy and must be blocking the propagation some other
 * way -- or `b` there is live past it.
 *
 * MEASURED, and both cost the word that makes them useless: hoisting the copy
 * OUT of the `t != 0` block so that `b` is still live at it -- `c = b;` above
 * the `if (t != 0)` with the arm reduced to the one-armed
 * `if (p[0x49] == 0) { c = func_800589B8(...); }` -- is 82 words / 36 DIFF,
 * and the same copy put immediately after `b = r;` is 82 / 70. The extra word
 * is the copy itself: inside the arm it is the arm's only insn and reorg
 * fills with it, hoisted it is a separate insn on both paths.
 *
 * So the remaining shapes are: a use of `b` after the copy ON THAT PATH that
 * costs no instruction, or a conflict between `b` and `c` without one. Note
 * also that `c` crosses a call and `b` does not, which is why pruning removes
 * $a2 from `c` (and gives it $s2) but leaves it on `b`.
 *
 * EXHAUSTION: a full `tools/decomp/search.py` sweep over every transform
 * family, run 2026-10-28 on this 6-DIFF base, improved on nothing. The
 * measured lists above predate the 12 -> 6 inverted-if/else fix, so treat
 * them as evidence about THAT source, not this one.
 *
 * `u8 b` is load-bearing and was re-measured: `s32 b` and `u32 b` are 78
 * words / 62 DIFF, `s32 b = r & 0xFF` is 79 / 66, dropping `b` and masking
 * `r` at each site is 80 / 65. `b = r & 0xFF` with `u8 b` is identical to
 * plain `b = r` (6 DIFF), so the mask in the source is free but buys nothing.
 */

#include "common.h"

extern u32 D_800737DC[];

u32 func_800589B8(s32);
void func_8005914C(s32, u8, u8, u8);
void func_80058E64(void);
void func_80058E6C(s32, u8);
void func_800590E0(s32);

s32 func_8005947C(u8 *p) {
    s32 r;
    u8 b;
    u32 i;
    u8 c;
    u8 d;
    s32 t;

    d = 0;
    r = func_800589B8((s32) p);
    b = r;
    if ((r & 0x80) == 0) {
        i = p[0x4A];
        p[0x49] = 1;
    } else {
        if (b != 0xFF) {
            p[0x4A] = r;
        }
        i = r;
        p[0x49] = 0;
    }
    t = D_800737DC[(i >> 4) & 0xF];
    if (t != 0) {
        if (p[0x49] != 0) {
            c = b;
        } else {
            c = func_800589B8((s32) p);
        }
        if (t == 2) {
            d = func_800589B8((s32) p);
        }
        func_8005914C((s32) p, i, c, d);
        return 0;
    }
    switch (b) {
    case 0xFF:
        func_80058E6C((s32) p, func_800589B8((s32) p));
        break;
    case 0xF0:
        func_800590E0((s32) p);
        break;
    default:
        func_80058E64();
        break;
    }
    return 0;
}
