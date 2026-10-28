/* func_800563B0 (unk_2879C) -- 22 of 22 words, 14 DIFF. NOT compiled.
 *
 * Written out of the `func_80056DB4 + func_800563B0 + func_80056D44` row on
 * 2026-10-27. Same recipe as func_80056DB4 (read that file's header for the
 * addressing) with four halfword stores instead: flags 0xF at +4, the two
 * s16 arguments at +8 and +0xA, zeros at +0xC and +0xE, the D_80073740
 * handle at +0.
 *
 * The listing puts the +0 store in the `jal`'s DELAY SLOT
 * (`jal func_8005F134 ; sw $v0, %lo(D_80084918)($a3)`), so it is the last of
 * the six and reorg moves it.
 *
 * THE PERMUTATION RUN IS DONE, 2026-10-28 (perm.py, 200 of the 720 orderings).
 * Two results, and the better NUMBER is not the better SOURCE:
 *
 *     1 2 3 4 5 6   22 words, 14 DIFF   <- the order below, and the LISTING's
 *     1 5 6 2 3 4   22 words, 11 DIFF   <- three words closer, wrong order
 *
 * The listing's own store order is the one in this file: flags at +4, then
 * +8, +0xA, +0xC, +0xE, with the +0 handle LAST (reorg then lifts it into the
 * `jal`'s delay slot). The 11-DIFF arrangement gets there by moving the +0xE
 * zero and the +0 handle up between the flags and the arguments, which is not
 * what the listing does, so this file keeps the 14 and records the 11.
 *
 * WHAT IS ACTUALLY LEFT IS A SCHEDULE, and it is the same wall as the sibling
 * func_80056D44's. Side by side:
 *
 *     target  addiu q ; li 0xF ; sw ra ; sw +4 ; lui/addiu table ; sll ;
 *             addu ; lw ; addu $a0,$v1 <- the call arg fills the load slot
 *     built   addiu q ; lui/addiu table ; sll ; addu ; sw ra ; lw ; li 0xF ;
 *             sw +4 ; ... ; sw +0 ; jal ; move $a0,$v1 <- the arg in the jal slot
 *
 * The listing does the flags store FIRST and leaves the table lookup until
 * after it; this build runs the lookup first, because sched1 ranks by the
 * longest chain and the lookup's (lui/addiu, sll, addu, lw, sw) beats the
 * flags store's (li, sw). No store order changes that -- it is the same
 * normalisation that holds func_80056D44 two words short. Measured
 * 2026-10-28, both 22 words and 14 DIFF: the lookup assigned BELOW the flags
 * store (`u32 h;` with no initialiser), and the lookup written inline in the
 * +0 store with no `h` at all. Priority decides it, not the source order.
 *
 * AND THE 2-DIFF SIBLING SAYS WHY IT WORKS THERE: func_80056DB4's early store
 * is 0x60100, which needs `lui`+`ori` -- a TWO-insn materialisation, long
 * enough to outrank the lookup, which is exactly what its header describes as
 * "+0x24 stores a REGISTER value and its `li 5` has to be pulled ahead of it,
 * and the load rides along". Here the flags value is 0xF, one `li`, and it
 * loses. So the three siblings differ in this one respect and the recipe does
 * not transfer: what would fix this function is a longer dependence chain
 * ahead of the lookup, not another arrangement of the stores.
 *
 * ALSO MEASURED 2026-10-28: un-bumping the lookup by making its destination
 * multi-set -- `i = D_80073740[i];`, reusing the parameter, which is the only
 * second set a straight-line function has -- is 22 words and 16 DIFF, two
 * worse. Same result as the sibling func_80056DB4 got from the same change
 * (10 DIFF there). The `birthing_insn_p` mechanism behind "bumped" is now
 * written up in matching-levers section 8, including why this lever wants a
 * value whose live range is already short: here it lengthens `i`'s.
 */

#include "common.h"

void func_8005F134(u8 *);

void func_800563B0(s32 i, s16 a, s16 b) {
    u8 *q = D_80084918;
    u32 h = D_80073740[i];

    *(u32 *) (q + 4) = 0xF;
    *(s16 *) (q + 8) = a;
    *(s16 *) (q + 0xA) = b;
    *(s16 *) (q + 0xC) = 0;
    *(s16 *) (q + 0xE) = 0;
    *(u32 *) D_80084918 = h;
    func_8005F134(q);
}
