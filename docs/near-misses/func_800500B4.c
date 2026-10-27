/* func_800500B4 (unk_1C540) -- 139 of 139 words, 96 DIFF. NOT compiled.
 * No calls; all arithmetic, pointer chasing and one six-iteration loop.
 *
 * Positions a dialogue window: the +0x0A of p[8]'s and p[9]'s sub-object from
 * the caller's +0x10..+0x1C fixed-point span, then six passes that decay a
 * per-column counter at q[0xA4 + i] toward 8 and write it into p[1 + i] and
 * p[10 + i]'s sub-objects as a halfword and three bytes of `c * 5 + 0x58`.
 *
 * WHAT IS PAID FOR -- the word count is exact and the structure is verified:
 *
 *   * The loop is a `goto` loop, which is what keeps the two walking pointers
 *     UNBIASED. As a `do`/`while`, loop.c combines the +0x4 and +0x28 address
 *     givs off `pp` into one biased base -- `addiu $a3, $a0, 0x4` with
 *     displacements 0x0 and 0x24 -- and does the same to `qq` with +0xA4,
 *     where the listing keeps `addu $a3, $a0, $zero` / `addu $a2, $t2, $zero`
 *     and the offsets as displacements. See the goto-loop lever in
 *     matching-levers section 9.
 *   * The -0x28 belongs to `b`, not to the sum: `b = ... / ... - 0x28` then
 *     `(a << 4) + b`. Written `(a << 4) + b - 0x28` fold moves the constant
 *     onto the sum and the `addiu` comes out after the `addu` instead of
 *     before it.
 *   * `pp[8]` for the -(a << 9) store, not `p[8]`: the listing reads it as
 *     `0x20($a3)`, through the walking pointer, which is also what stops the
 *     bias from settling on +0x4.
 *   * The counter store goes through a POINTER -- `u8 *r = qq + 0xA4;` and
 *     `*r = c;` -- which is the listing's `addiu $t0, $a2, 0xA4` in the
 *     branch's delay slot and `sb $v0, 0x0($t0)` at the join, where
 *     `qq[0xA4] = c` is one insn and the function comes out a word short.
 *   * Each of the six `c * 5 + 0x58` stores re-reads `qq[0xA4]`, which the
 *     listing does too.
 *
 * WHAT IS LEFT: 96 words, and they are a single register cascade with two
 * shape symptoms. `a` takes $a2 here and $a1 in the listing, where it is then
 * shifted IN PLACE -- `sll $a1, $a1, 4` -- which lets sched2 lift the shift
 * into the `lw 0x20($a0)` load-delay slot; with the shift writing a different
 * register that slot stays a nop and everything downstream is renumbered.
 * Assigning the shifted value back to `a` gets the in-place form but loses a
 * word elsewhere (138). The root fact to find is why `a` does not take $a1:
 * $a1 is free from the `q` copy onwards and ascending allocation order should
 * reach it before $a2.
 *
 * MEASURED: the counter reload written as `c = *r` rather than `c = qq[0xA4]`
 * is 87 DIFF but two nops out; dropping the reload entirely is also 87 with a
 * missing `qq` copy; `qq[0xA4] = c` direct is 138 words. Four declaration
 * orders all give 96.
 */

#include "common.h"

void func_800500B4(u8 **p, u8 *q)
{
    s32 a;
    s32 b;
    s32 i;
    u8 **pp;
    u8 *qq;

    a = *(s32 *) (q + 0x18);
    b = ((*(s32 *) (q + 0x1C) - a) * (*(s32 *) (q + 0x14) << 4)) / *(s32 *) (q + 0x10) - 0x28;
    *(s16 *) (*(u8 **) (p[8] + 4) + 0xA) = (a << 4) + b;
    *(s16 *) (*(u8 **) (p[9] + 4) + 0xA) = *(u16 *) (*(u8 **) (p[8] + 4) + 0xA);
    *(s16 *) (*(u8 **) (p[8] + 8) + 8) = *(u16 *) (*(u8 **) (p[8] + 4) + 0xA) - 2;
    a = *(s32 *) (q + 0x1C);
    b = ((a - *(s32 *) (q + 0x18)) * -(*(s32 *) (q + 0x14) << 9)) / *(s32 *) (q + 0x10);
    i = 0;
    pp = p;
    qq = q;
    *(s16 *) (*(u8 **) (pp[8] + 8) + 4) = -(a << 9) - b;
top:
    {
        u8 *r = qq + 0xA4;
        s32 c = qq[0xA4];

        if (i == *(s32 *) (q + 0x1C))
        {
            c = c + (8 - c) / (*(s32 *) (q + 0x10) - *(s32 *) (q + 0x14) + 1);
        }
        else if (c != 0)
        {
            c = c - 1;
        }
        *r = c;
        c = qq[0xA4];
        *(s16 *) (*(u8 **) (pp[1] + 4) + 8) = c;
        *(s16 *) (*(u8 **) (pp[10] + 4) + 8) = c + 0x1E;
        *(u8 *) (*(u8 **) (pp[10] + 4) + 0) = qq[0xA4] * 5 + 0x58;
        *(u8 *) (*(u8 **) (pp[10] + 4) + 1) = qq[0xA4] * 5 + 0x58;
        *(u8 *) (*(u8 **) (pp[10] + 4) + 2) = qq[0xA4] * 5 + 0x58;
        *(u8 *) (*(u8 **) (pp[1] + 4) + 0) = qq[0xA4] * 5 + 0x58;
        *(u8 *) (*(u8 **) (pp[1] + 4) + 1) = qq[0xA4] * 5 + 0x58;
        *(u8 *) (*(u8 **) (pp[1] + 4) + 2) = qq[0xA4] * 5 + 0x58;
        i++;
        pp++;
        qq++;
    }
    if (i < 6)
    {
        goto top;
    }
    if (*(s32 *) (q + 0x14) < *(s32 *) (q + 0x10))
    {
        *(s32 *) (q + 0x14) = *(s32 *) (q + 0x14) + 1;
    }
}
