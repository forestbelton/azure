/* func_8005552C (unk_2879C) -- 50 of 52 words, 37 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep row on 2026-10-27, two words closer, with
 * the store-flag transform HALF BLOCKED and the reason it fires read out of
 * jump.c.
 *
 * Maps `k & 0xF00` to a mode (0x600 -> 2, 0x700 -> 3, 0x800 -> 0, else 1),
 * calls func_8005405C with it, and then func_80055730 or func_8005560C with
 * `(m << 8) | ((k & 0xFF) >> 4)` and `((k & 0xF) << 10) + 0x1C00`, the first
 * argument plus 0x10 when the mode is 0 and which callee chosen by k's
 * 0x1000 bit.
 *
 * WHAT IS PAID FOR
 *
 * * The parameter is 32 bits wide, so the three later masks read it raw.
 * * `x` is SIGNED: the low-bound test is `slti $v0, $a0, 0x601`, and a `u32`
 *   local gives `sltiu` (one differing word).
 * * `m` is `s16`, which is what makes the arms set $v0 and a shared
 *   `addu $s0, $v0, $zero` copy it.
 * * The dispatch is a NESTED-IF chain written with `!=` tests, which is the
 *   only shape that branches INTO each arm and emits the `slti 0x601`
 *   low-bound test; a two- or three-case `switch` gives a decision tree
 *   rooted at 0x700 instead (44-46 DIFF).
 * * THE TWO `m = 1` SITES ARE ONE STATEMENT, reached by a `goto` from the
 *   innermost else. That is worth two of the four missing words, and the
 *   reason is jump.c's store-flag transform: with the innermost pair written
 *   `if (x != 0x800) m = 1; else m = 0;` jump_optimize rewrites it as
 *   `xori $v0, $v1, 0x800` + `sltu $v0, $zero, $v0`, where the listing keeps
 *   the two branches and an `addu $v0, $zero, $zero`.
 *
 *   WHY IT ALWAYS FIRES on a 0/1 pair (jump.c:1139 in
 *   scratch/gcc-src/gcc-2.7.2.3): the gate is
 *
 *       ((reversep = 0, temp2 == const0_rtx)
 *        || (temp3 == const0_rtx && (reversep = can_reverse_comparison_p ...))
 *        || (BRANCH_COST >= 2 && both CONST_INT && bit-subset)
 *        || BRANCH_COST >= 3)
 *
 *   on the pattern `x = a; if (...) x = b;`. With {0, 1} one of the two
 *   values IS const0_rtx whichever way round gcc normalises the if/else, so
 *   the first or second disjunct holds and no spelling of the PAIR escapes
 *   it. Sharing the `m = 1` site removes the pair -- the innermost `if` then
 *   has one arm and a `goto` -- and the transform has nothing to match.
 *
 * WHAT IS LEFT: TWO WORDS AND THE ARM ORDER. The listing lays the bodies out
 * default(1), 0x600(2), 0x700(3), 0x800(0) and branches INTO the m=0 arm
 * (`beq $a0, $v0, .L80055584`) with m=1 as the fall-through; this source
 * emits 1, 0, 3, 2 and branches AWAY to the join (`bne`). Per
 * matching-levers' "case bodies are emitted in SOURCE order", that layout is
 * evidence about the nesting, and the nesting that produces it has not been
 * found: the plain `m = 1;` pre-initialisation with three `else if`s gives the
 * right body order but no `slti` low-bound test at all, and the nested-`!=`
 * chain gives the `slti` but the wrong order. Something between the two.
 *
 * Also measured and not working: `x` masked inline in each test (identical),
 * the parameter masked IN PLACE with a saved copy (identical -- gcc still
 * reads the copy, so `andi $v1, $s1` rather than the listing's
 * `andi $a0, $a0`), and the innermost pair as `m = 1; if (x == 0x800) m = 0;`
 * (identical: that IS the form the transform matches).
 *
 * 2026-10-28: A 26-DIFF VARIANT, MEASURED AND REJECTED, with the listing's own
 * prologue as the reason. `search.py`'s `copy_param` family reports that
 * copying the parameter (`k_c = k;`, with the three masks reading `k_c`) is
 * 53 words / 26 DIFF against this file's 50 / 37 -- one over instead of two
 * short, and eleven fewer differing words.
 *
 * The three words it adds are `sw $s2, 0x18($sp)`, `move $s2, $s1` and the
 * frame growth that follows, i.e. a THIRD callee-saved register holding a copy
 * of the second. The listing has neither: it saves exactly $s0, $s1 and $ra in
 * a 0x20 frame, and contains ZERO `addu $sN, $sM, $zero` copies between
 * callee-saved registers.
 *
 * WHAT THE LISTING'S PROLOGUE DOES SAY, and it is worth more than the
 * rejection:
 *
 *     addu  $s1, $a0, $zero      <- `k` copied to its callee-saved home, ONCE
 *     andi  $a0, $a0, 0xF00      <- the first mask reads the ARGUMENT register,
 *                                   in place
 *     addiu $v0, $zero, 0x600
 *     beq   $a0, $v0, ...
 *
 * so the raw/copy split this variant tries to spell by hand is one gcc makes
 * for itself: the first test masks `$a0` while the later uses read `$s1`. The
 * source therefore wants `k` left exactly as the parameter, which is what this
 * file has. Checked under the word-count rule in unname.py's header.
 */

#include "common.h"

s16 func_8005405C(s16);
void func_80055730(s32, s32);
void func_8005560C(s32, s32);

void func_8005552C(u32 k) {
    s16 m;
    s32 a;
    s32 b;
    s32 x;

    x = k & 0xF00;
    if (x != 0x600) {
        if (x > 0x600) {
            if (x != 0x700) {
                if (x == 0x800) {
                    m = 0;
                } else {
                    goto one;
                }
            } else {
                m = 3;
            }
        } else {
        one:
            m = 1;
        }
    } else {
        m = 2;
    }
    if (func_8005405C(m) != 0) {
        a = (m << 8) | ((k & 0xFF) >> 4);
        b = ((k & 0xF) << 10) + 0x1C00;
        if (m == 0) {
            a += 0x10;
        }
        if ((k & 0x1000) != 0) {
            func_80055730(a, b);
        } else {
            func_8005560C(a, b);
        }
    }
}
