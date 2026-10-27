/* func_8004E6F4 (unk_1C540) -- 102 of 100 words, TWO words OVER. NOT
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
            s32 wt = w[i - 1];
            s32 u = 4 - wt;

            r = c[0] * wt + b[0] * u;
            g = c[1] * wt + b[1] * u;
            bl = c[2] * wt + b[2] * u;
        }
        else
        {
            s32 wt = w[i - 1];
            s32 u = 4 - wt;

            r = d[0] * wt + c[0] * u;
            g = d[1] * wt + c[1] * u;
            bl = d[2] * wt + c[2] * u;
        }
        acc += r >> 5;
        acc += g & 0x3E0;
        acc += (bl & 0x3E0) << 5;
        *(u16 *) (i * 2 + (u32) buf) = acc;
    }
    func_8003F80C((u32) buf, (a & 0xF) | 0x7D00, 1, 2);
    DrawSync(0);
}
