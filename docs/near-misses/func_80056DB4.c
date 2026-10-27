/* func_80056DB4 (unk_1C540) -- 23 of 23 words, 2 DIFF. NOT compiled.
 *
 * Written out of the `func_80056DB4 + func_800563B0 + func_80056D44` row on
 * 2026-10-27 so all three have sources (see the sibling files).
 *
 * Fills the parameter block at D_80084918 -- flags 0x60100 at +4, zeros at
 * +0x3A and +0x3C, 5 at +0x24, and the D_80073740 handle at +0 -- and hands
 * its address to func_8005F134.
 *
 * THE ADDRESSING RECIPE, shared by all three siblings: the base is a POINTER
 * LOCAL initialised in its declaration (`u8 *q = D_80084918;`), which
 * materialises it first, while the +0 store goes through the SYMBOL
 * (`*(u32 *) D_80084918 = h;`) and so keeps the listing's symbolic
 * `sw $a0, %lo(D_80084918)($a1)` against the shared `(high)` pseudo. The
 * D_80073740 lookup goes into its own local, also in a declaration. A struct
 * type for the block is NOT the answer: as COMPONENT_REFs the +0 store folds
 * onto the register base and the function is a word short (21 DIFF).
 *
 * WHAT IS LEFT: TWO WORDS, and the store ORDER that gets there is not the
 * listing's. perm.py over all 120 orderings of the five stores (run
 * 2026-10-27) ranks them
 *
 *     1 4 3 2 5   2 DIFF   <- the order below: +4, +0x24, +0x3C, +0x3A, +0
 *     1 4 2 3 5   3 DIFF
 *     1 2 3 4 5   4 DIFF   <- the listing's own order
 *
 * so the +0x24 store comes BEFORE the two halfword stores and the two
 * halfwords are written in the REVERSE of the order they are emitted in. Both
 * are stores of zero to independent fields, so the order is free in the
 * source and only the score is evidence either way.
 *
 * The two that remain are the lookup's load and the constant 5 against the
 * two halfword stores: the listing emits `lw` then `addiu $v0, $zero, 0x5`
 * then the two `sh`s, and this source still has the load after them. The
 * lookup declared before `q`, and read in a statement of its own after the
 * +4 store, are both worse. Since 120 orderings cannot reach it, that pair is
 * sched1 ordering a load against stores rather than statement order.
 */

#include "common.h"

void func_8005F134(u8 *);

void func_80056DB4(s32 i) {
    u8 *q = D_80084918;
    u32 h = D_80073740[i];

    *(u32 *) (q + 4) = 0x60100;
    *(u32 *) (q + 0x24) = 5;
    *(s16 *) (q + 0x3C) = 0;
    *(s16 *) (q + 0x3A) = 0;
    *(u32 *) D_80084918 = h;
    func_8005F134(q);
}
