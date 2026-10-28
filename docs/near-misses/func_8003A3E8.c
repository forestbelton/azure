/* func_8003A3E8 (unk_72A8) -- 32 of 32 words, 12 DIFF. NOT compiled.
 *
 * Written out of the `func_8003A3E8 + func_8003A374` row on 2026-10-27.
 * Its twin func_8003A374 is the same opcode without the table lookup (the
 * index is used directly), and the two sit at the same 12 DIFF.
 *
 * The func_8003A2E0 family: reads a byte, a table index and a little-endian
 * 32-bit operand from the bytecode cursor, and stores a word THROUGH A BYTE
 * it loads from the composed address -- `*(u32 *) (u32) *(u8 *) d = ...`,
 * which is what the `lbu`/`sw` pair at the end is (the game's own bug or a
 * deliberate low-address write; either way that is the C).
 *
 * The statement order is pinned and is the family's: the first byte with its
 * increment, the state pointer at +0x80, the table lookup through `**cp`
 * WITHOUT incrementing, the second increment, the four operand bytes off
 * `r = *cp`, then `*cp += 4`. The lookup after the operand bytes is 26 DIFF.
 *
 * WHAT IS LEFT: TWELVE WORDS, which is three better than the row this came
 * from and the same for both twins -- the target accumulates the operand in
 * $v0, starting from the first byte, where this source uses $a1, and the
 * `sll` of the table value interleaves differently. Measured: the operand
 * composition as a step-by-step accumulation (`d = r[0]; d += r[1] << 8;
 * ...`), which is func_8003A1A4's shape for ITS second composition, is 15 --
 * so the one-expression form below is right and the registers are an
 * allocation question, not a spelling one.
 *
 * 2026-10-28: see func_8003A374's park for the two measurement runs that close
 * BOTH terms of the margin this pair is waiting on -- the free-reference
 * splits (which dissolve the `combine_regs` tie that sums the chain's
 * references, so each piece then prices on its own 2 refs and loses by more
 * than the whole did) and the live-range shortenings by statement order (`s`
 * and `t` moved above the accumulation: 16 DIFF, and moving `*cp += 4` with
 * them costs a word, because gcc folds it into the preceding increment). The
 * accumulator still has to outrank the cursor and neither term of the
 * arithmetic is reachable from the source.
 */

#include "common.h"

void func_8003A3E8(u8 *p) {
    u8 **cp = (u8 **) (p + 0x1C);
    u32 a;
    u32 m;
    u8 *s;
    u8 *r;
    u32 d;
    u8 *t;

    a = *(*cp)++;
    s = *(u8 **) (p + 0x80);
    m = *(u32 *) (s + **cp * 4 + 0x48) * 4;
    (*cp)++;
    r = *cp;
    d = r[0] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24) + m;
    *cp += 4;
    t = (u8 *) (a * 4 + (u32) s);
    *(u32 *) (u32) *(u8 *) d = *(u32 *) (t + 0x48);
}
