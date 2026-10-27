/* func_80059DAC (unk_2CDC0) -- 58 of 58 words, 8 DIFF. NOT compiled.
 *
 * Sorts D_800869C0 ascending by each slot's first word. Counts the live slots
 * first (stop at a zero first word, cap 16), then a bubble sort comparing
 * slot i against slot j for j from n-1 down to i+1, swapping the whole 8-byte
 * pair through a stack temp -- which is the entire 8-byte frame.
 *
 * FOUR THINGS WERE NEEDED TO GET THE WORD COUNT EXACT, all of them structural
 * rather than arithmetic, and each worth recording because they pull against
 * each other:
 *
 *   1. The base goes through a LOCAL with the index written first,
 *      `(u32 *) (i * 8 + (u32) c)` and not `c + i * 8`, which is the idiom
 *      func_8005A1D0 above it already documents.
 *   2. The two loops need SEPARATE base locals. With one, the second loop
 *      reuses the first's register and the target's second lui/addiu pair
 *      goes missing (three words); with two, cse's run has ended at the first
 *      loop's multi-use exit label and the address is re-materialised.
 *   3. The second loop's base must be materialised INSIDE the loop body, so
 *      loop.c hoists it to the preheader -- which is AFTER the `blez` guard.
 *      Declared between the loops it lands before the guard, and reorg then
 *      steals `i = 0` into the first loop's `beq` delay slot where the target
 *      has a nop.
 *   4. The comparison is `*(s32 *) d > *(s32 *) s`, not `s < d`. Same test,
 *      but the operand named first is loaded first (advise.py now reports
 *      that pair as EVAL-ORDER rather than REASSOCIATED).
 *
 * WHAT IS LEFT, one word's worth of structure and two registers with it: the
 * target makes `i * 8 + base` an outer-loop GIV in $t1, copies it into $a1 for
 * the inner loop (`addu $a1,$t1,$zero`), keeps a second copy of the base in
 * $t2 for the inner `j * 8 + base`, and increments the giv in the outer
 * branch's delay slot. This source recomputes `sll`/`addu` for both pointers
 * instead, so loop.c declined the reduction.
 *
 * THE THRESHOLD GUESS IN THIS NOTE WAS WRONG and is corrected here. loop.c's
 * cut is `v->lifetime * threshold * benefit < insn_count` (loop.c:3823) with
 * `threshold = (loop_has_call ? 1 : 2) * (3 + n_non_fixed_regs)` -- and
 * `n_non_fixed_regs` counts non-fixed HARD registers, so it is a constant for
 * this target and the product is in the hundreds either way. `loop_has_call`
 * cannot decide anything here. What decides a giv is only whether `benefit`
 * survives above zero: the address's rtx cost, minus `add_cost * biv_count`,
 * minus `copy_cost` when the giv is NOT replaceable.
 *
 * So the real question is `v->replaceable`. `d` is assigned inside the INNER
 * loop here, which the outer loop's analysis sees as not-every-iteration, so
 * `replaceable` is 0, `benefit` goes 4 - 2 - 2 = 0 and the giv is ignored.
 * Hoisting `d` to the outer body makes it replaceable and the giv appears --
 * which is the 57-word measurement below, one word short because the giv's
 * register then IS `d` and the listing's `addu $a1, $t1, $zero` copy has
 * nothing to copy. The open question is therefore narrower than it was: what
 * makes the listing keep the walked value and `d` in SEPARATE registers.
 *
 * MEASURED, all worse: `d` hoisted to the outer body (57 words -- it gets the
 * giv but loses the copy), an explicit `r += 8` cursor (59), and an explicit
 * `u8 *e` giv with `d` copied from it inside the inner loop (57 words, 41
 * DIFF).
 */

#include "common.h"
void func_80059DAC(void) {
    u8 *b = (u8 *) D_800869C0;
    s32 n;
    s32 i;

    for (n = 0; n < 0x10; n++) {
        if (*(u32 *) (n * 8 + (u32) b) == 0) {
            break;
        }
    }
    for (i = 0; i < n; i++) {
        u8 *c = (u8 *) D_800869C0;
        s32 j;

        for (j = n - 1; i < j; j--) {
            u32 *d = (u32 *) (i * 8 + (u32) c);
            u32 *s = (u32 *) (j * 8 + (u32) c);
            u32 t[2];

            if (*(s32 *) d > *(s32 *) s) {
                t[1] = d[1];
                t[0] = d[0];
                d[1] = s[1];
                d[0] = s[0];
                s[1] = t[1];
                s[0] = t[0];
            }
        }
    }
}
