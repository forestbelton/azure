/* func_80056DB4 (unk_1C540) -- 23 of 23 words, 3 DIFF. NOT compiled.
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
 * WHAT IS LEFT: THREE WORDS, the order of the +0x24 store against the two
 * halfword stores and the lookup's own load. The listing emits
 *
 *     lw $a0, 0x0($a0)   (the lookup) ; addiu $v0, $zero, 0x5 ;
 *     sh $zero, 0x3A ; sh $zero, 0x3C ; sw $v0, 0x24 ; sw $a0, %lo(...)
 *
 * i.e. the lookup's load and the constant 5 BEFORE the two halfword stores.
 * With the +0x24 store written after them (the listing's own store order)
 * this source emits the two `sh`s first and the load after: 4 DIFF. Writing
 * the +0x24 store BEFORE them -- the order below -- is 3. The lookup declared
 * before `q`, and read in a statement of its own after the +4 store, are both
 * 4.
 *
 * None of the 120 permutations of the five stores reaches it (perm.py; the
 * row's run and a second one here agree, best 2-3 DIFF), so the remaining
 * words are sched1 ordering the load against the stores rather than statement
 * order.
 */

#include "common.h"

void func_8005F134(u8 *);

void func_80056DB4(s32 i) {
    u8 *q = D_80084918;
    u32 h = D_80073740[i];

    *(u32 *) (q + 4) = 0x60100;
    *(u32 *) (q + 0x24) = 5;
    *(s16 *) (q + 0x3A) = 0;
    *(s16 *) (q + 0x3C) = 0;
    *(u32 *) D_80084918 = h;
    func_8005F134(q);
}
