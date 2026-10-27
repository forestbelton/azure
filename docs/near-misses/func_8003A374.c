/* func_8003A374 (unk_72A8) -- 29 of 29 words, 12 DIFF. NOT compiled.
 *
 * Written out of the `func_8003A3E8 + func_8003A374` row on 2026-10-27.
 * The twin func_8003A3E8 is the same opcode WITH a table lookup
 * (`m = *(u32 *) (s + **cp * 4 + 0x48) * 4` in place of `m = **cp * 4`), and
 * the two sit at the same 12 DIFF.
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
 */

#include "common.h"

void func_8003A374(u8 *p) {
    u8 **cp = (u8 **) (p + 0x1C);
    u32 a;
    u32 m;
    u8 *r;
    u32 d;
    u8 *s;
    u8 *t;

    a = *(*cp)++;
    m = **cp * 4;
    (*cp)++;
    r = *cp;
    d = r[0] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24) + m;
    *cp += 4;
    s = *(u8 **) (p + 0x80);
    t = (u8 *) (a * 4 + (u32) s);
    *(u32 *) (u32) *(u8 *) d = *(u32 *) (t + 0x48);
}
