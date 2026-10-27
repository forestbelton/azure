/* func_8004E6F4 (unk_1C540) -- 100 of 100 words, 13 DIFF. NOT
 * compiled.
 *
 * Builds a 16-entry 15-bit colour ramp in a stack buffer and hands it to
 * func_8003F80C: entry i blends two of the three palettes by the weight
 * D_8007162C[k * 8 + i - 1] out of 4, with bit 15 set while i < n. Entries
 * 1..4 blend c over b, 5..8 blend d over c.
 *
 * WHAT IS PAID FOR -- every opcode, immediate and displacement in the two
 * arms and the shared tail:
 *
 *   * `w[i - 1]` with `w` a pointer LOCAL. The address is
 *     `(plus (plus w i) -1)` -- two adds, so the giv's benefit is 4 and
 *     loop.c reduces it: the listing's `addiu $t6, $v1, 0x1` init and
 *     `lbu -0x1($t6)` displacement. Walking a pointer with `wp[-1]` instead
 *     lets fold collapse the +1 and the -1 and the reduction stops (one add,
 *     benefit 2, which does not survive `- add_cost`).
 *   * THREE combined values, not six products: `r = c[0] * wt + b[0] * u`
 *     and so on. gcc splits them back into the six `mflo`s the listing has,
 *     and cross-jumping then shares the adds; written as six product temps
 *     the six are live at once from the source's point of view and reload
 *     spills five of them -- 116 words.
 *   * `acc` IS a variable, assigned `(i < n) << 15` and then `+=` three
 *     times. Inlined into the store expression, gcc turns the `<< 15` into a
 *     conditional `ori 0x8000` with its own branch -- 101 words and the wrong
 *     shape.
 *   * The store is `*(u16 *) (i * 2 + (u32) buf)`, index-first, which keeps
 *     the listing's `sll` / `addiu $a0, $sp, 0x10` / `addu` per iteration;
 *     `buf[i]` is reduced to a walking pointer.
 *   * func_8003F80C's first argument is `buf`, and the listing does not set
 *     $a0 before the `jal` -- the loop's own last `addiu $a0, $sp, 0x10`
 *     already holds it.
 *
 * WHAT IS LEFT: two words, and they are ONE register. The listing spends
 * twelve long-lived values -- `acc`, the six products, `i`, the weight giv,
 * the constant 4, `n` and `a` -- across $t0-$t9, $v1 and $s1, with $v1 doing
 * double duty: it holds the weight inside each arm and is then reused for the
 * fifth product, because the weight dies at the last `mult`. That leaves $t9
 * for `a`, so only $s1 is callee-saved (the listing also saves $s0 without
 * using it, which is a gcc artifact).
 *
 * This source does not get that reuse: the weight and the fifth product take
 * separate registers, so thirteen values need homes, `a` falls to $s0 and the
 * extra save/restore pair is the two words. Measured and not better: `acc`
 * inlined (101 words, the conditional shape), the masked func_8003F80C
 * argument computed before the loop (102), and six explicit product temps
 * (116).
 *
 * SINCE PARKED, 2026-10-27: 102 of 100 words and 101 DIFF to 100 of 100 and
 * 13, in two steps.
 *
 * * `acc += r >> 5;` DUPLICATED into both arms of the `i < 5` test rather
 *   than written once after it. Two words: jump2 cross-joins the copies at no
 *   cost, and the duplicate buys the pseudos it mentions an extra reference
 *   at allocation time. search.py's dup-into-arms family found it in the
 *   sweep of all forty parks.
 *
 * * The weight and its complement written INLINE at all six products --
 *   `c[0] * w[i - 1] + b[0] * (4 - w[i - 1])` -- instead of through `wt` and
 *   `u` locals. 17 DIFF to 13. The locals made `w[i - 1]` load FIRST, and the
 *   listing loads the first-named operand of the first product first
 *   (`lbu $v0, 0x0($a2)` before `lbu $v1, -0x1($t6)`), with the complement
 *   computed lazily between the first and second multiply. cse still shares
 *   both subexpressions, so the inline form costs nothing. Keeping `u` as a
 *   local and inlining only the weight is 17 again.
 *
 * WHAT IS LEFT, 13 words: nine of them register-only, and the other four a
 * $t2/$t3 swap with its consequences. The six products land in $t4, $t2, $t3,
 * $t1, $v1, $s1 in the listing and in $t4, $t3, $t2, $t1, $v1, $s1 here, so
 * the two sums pair the wrong halves (`addu $v0, $t4, $t3` against the
 * listing's `addu $v0, $t4, $t2`) and the shared tail then computes both sums
 * before processing either, where the listing computes r's sum, folds it into
 * acc, and only then computes g's.
 */

#include "common.h"

extern u8 D_8007162C[];

void func_8004E6F4(s32 a, u8 *b, u8 *c, u8 *d, s32 k, s32 n)
{
    u16 buf[0x10];
    s32 i;
    u8 *w;
    s32 acc;
    s32 r;
    s32 g;
    s32 bl;

    buf[0] = 0;
    w = &D_8007162C[k * 8];
    for (i = 1; i < 9; i++)
    {
        acc = (i < n) << 15;
        if (i < 5)
        {
            r = c[0] * w[i - 1] + b[0] * (4 - w[i - 1]);
            g = c[1] * w[i - 1] + b[1] * (4 - w[i - 1]);
            bl = c[2] * w[i - 1] + b[2] * (4 - w[i - 1]);
            acc += r >> 5;
        }
        else
        {
            r = d[0] * w[i - 1] + c[0] * (4 - w[i - 1]);
            g = d[1] * w[i - 1] + c[1] * (4 - w[i - 1]);
            bl = d[2] * w[i - 1] + c[2] * (4 - w[i - 1]);

            acc += r >> 5;
        }
        acc += g & 0x3E0;
        acc += (bl & 0x3E0) << 5;
        *(u16 *) (i * 2 + (u32) buf) = acc;
    }
    func_8003F80C((u32) buf, (a & 0xF) | 0x7D00, 1, 2);
    DrawSync(0);
}
