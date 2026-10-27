/* func_80055864 (unk_2879C) -- 38 of 38 words, 3 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep row on 2026-10-27 at 19 DIFF, and closed to
 * 3 the same day.
 *
 * Fills the three parallel tables: D_80084538 with four pointers into
 * D_801B9000 (+0, +0x3000, +0x4020, +0x4E40), then D_800847C0 and
 * D_80084768 with four constants each. Each table's FIRST store is symbolic
 * and the other three go through a rebuilt base (`sw $v0, %lo(SYM)($a0)` then
 * `addiu $a0, $a0, %lo(SYM)`), which the array spelling gives for free.
 *
 * THE SIX BIG CONSTANTS GO IN LOCALS, DECLARED IN THE ORDER THE LISTING
 * MATERIALISES THEM, and the stores stay GROUPED. That is worth sixteen of
 * the nineteen words this started at, and it is the whole finding:
 *
 *   * the target materialises them in an ALTERNATION between the last two
 *     tables -- 0x3A980 ($t2), 0x3B990 ($a3), 0x1BD50 ($t1), 0x576E0 ($a2),
 *     0x11940 ($t0), 0x69020 ($a1), i.e. D_80084768[0], D_800847C0[1],
 *     D_80084768[1], D_800847C0[2], D_80084768[2], D_800847C0[3];
 *   * but it gives D_80084768's three constants $t2, $t1, $t0 and
 *     D_800847C0's three $a3, $a2, $a1 -- two descending runs from different
 *     banks, which is what GROUPED stores give, because each group's
 *     constants are then live over one contiguous range;
 *   * so the order and the grouping disagree, and writing the STORES in the
 *     alternation gets the order right and the registers wrong (29 DIFF),
 *     while writing them grouped gets the registers right and the order wrong
 *     (19). A local per constant, declared in the alternating order, gives
 *     BOTH: the declarations fix the materialisation order (an initialiser
 *     list is a statement sequence -- matching-levers section 6) and the
 *     grouped stores fix the allocation.
 *
 * WHAT IS LEFT: THREE WORDS, and they are the order of two `lui`s. The target
 * materialises `%hi(D_80084538)` -- the first store's DESTINATION -- before
 * `%hi(D_801B9000)` and its `addiu`, where this source does the value first:
 *
 *     target  lui $a0, %hi(D_80084538) ; lui $v0, %hi(D_801B9000) ;
 *             addiu $v0, $v0, %lo ; sw $v0, %lo(D_80084538)($a0)
 *     built   lui $v0 ; addiu $v0 ; lui $a0 ; sw
 *
 * Measured and not working: `b` assigned as a statement after the constant
 * locals, `b` declared last among them, and the first store written
 * `D_80084538[0] = (u32) D_801B9000;` with no `b` at all -- all 3 DIFF. A
 * pointer local for D_80084538 would materialise its `lui` first but costs the
 * `addiu` as a word, since the listing wants only the HIGH there. Also
 * measured and worse: the row's per-index interleave (29) and swapping the
 * two blocks (26).
 */

#include "common.h"

void func_80055864(void) {
    u8 *b = D_801B9000;
    u32 c0 = 0x3A980;
    u32 c1 = 0x3B990;
    u32 c2 = 0x1BD50;
    u32 c3 = 0x576E0;
    u32 c4 = 0x11940;
    u32 c5 = 0x69020;

    D_80084538[0] = (u32) b;
    D_80084538[1] = (u32) (b + 0x3000);
    D_80084538[2] = (u32) (b + 0x4020);
    D_80084538[3] = (u32) (b + 0x4E40);
    D_800847C0[0] = 0x1010;
    D_800847C0[1] = c1;
    D_800847C0[2] = c3;
    D_800847C0[3] = c5;
    D_80084768[0] = c0;
    D_80084768[1] = c2;
    D_80084768[2] = c4;
    D_80084768[3] = 0xFA00;
}
