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
