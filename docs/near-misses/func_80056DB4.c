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
 * THE PARAGRAPH THAT USED TO BE HERE WAS STALE -- re-read the diff before
 * trusting a WHAT IS LEFT section. It said the lookup's `lw` and the
 * `addiu $v0, $zero, 0x5` were still after the two `sh`s. They are not: this
 * source already emits `lw` at word 12 and `li 5` at word 13, exactly as the
 * listing does. Re-measured 2026-10-27.
 *
 * WHAT IS ACTUALLY LEFT is the ORDER OF THREE STORES, words 14 and 16:
 *
 *     target  ... lw ; li 5 ; sh 0x3A ; sh 0x3C ; sw 0x24 ; sw +0
 *     built   ... lw ; li 5 ; sw 0x24 ; sh 0x3C ; sh 0x3A ; sw +0
 *
 * and the two sides are COUPLED, which is why 120 orderings of the stores
 * cannot get both. Measured:
 *
 *   * STORES FOLLOW SOURCE ORDER. A store's destination is a MEM, so
 *     `birthing_insn_p` never fires for it (it requires a REG dest) and the
 *     store keeps its LUID position. The three come out in exactly the order
 *     they are written.
 *   * THE LOOKUP'S LOAD IS BUMPED, so it floats as late as its consumer
 *     allows -- and where it lands depends on which store comes FIRST among
 *     the three. Written +0x24 first (this file) the load lands ahead of all
 *     three, because +0x24 stores a REGISTER value and its `li 5` has to be
 *     pulled ahead of it, and the load rides along. Written in the listing's
 *     own store order -- 0x3A, 0x3C, 0x24, which is what the output needs --
 *     the first two stores need only $zero, nothing is pulled ahead, and the
 *     `lw`/`li` pair lands AFTER them: 4 DIFF, the stores right and the pair
 *     two slots late.
 *   * THE LOOKUP'S POSITION IS IRRELEVANT in that order. Seven placements
 *     measured -- declared before `q`, declared after `q`, and as a bare
 *     statement before each of the five stores -- all 23 words, all 4 DIFF.
 *     sched1 normalises it.
 *   * UN-BUMPING THE LOAD by making its destination multi-set does not work
 *     here. The only second set available to a straight-line function is the
 *     PARAMETER's incoming copy (`i = D_80073740[i];`), and reusing `i`
 *     lengthens its live range enough to move the base pointer out of $v1
 *     into $a1/$a2: 10 DIFF in both store orders.
 *
 * So the next idea has to un-bump that load, or give one of the two halfword
 * stores a register-valued source, WITHOUT lengthening any live range -- not
 * reorder anything. SYMPTOM: nothing in this file is implausible as original
 * source; the +0x24-first store order is the only oddity and it is worth 2
 * words against the listing's own order.
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
