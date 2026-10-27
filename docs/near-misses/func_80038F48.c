/* func_80038F48 (unk_72A8) -- 98 of 98 words, 12 DIFF. NOT compiled.
 *
 * The script VM's control-flow dispatcher: while the opcode at +0x1C is 0xFD,
 * 0xFE, 0xFF or 0x00, it pushes a return address onto the +0x7C stack and
 * jumps -- 0xFD through the +0x80 table's +0x48 handler via func_8003AD08,
 * 0xFE through D_8006A944 indexed by the next byte, 0xFF through a 32-bit
 * address built from the next four bytes -- and 0x00 goes to func_80039640.
 *
 * WHAT IS PAID FOR -- everything structural, and the word count was exact on
 * the first compile:
 *
 *   * The loop condition is `(u8) (op + 3) < 3 || op == 0` with `op` a `u8`
 *     local, which is the listing's `addiu $v0, $v1, 0x3` / `andi 0xFF` /
 *     `sltiu 0x3` / `bnez` followed by `beqz $v1`. The `j` over the first
 *     test into the bottom is gcc's loop rotation.
 *   * The three arms test `*(s8 *) pc` against -3, -2 and -1 as an if/else-if
 *     CHAIN, not a switch -- the constants are not in sorted order, so a
 *     switch lays them out differently.
 *   * `t = *(u8 **) (p + 0x80)` is read into a local BEFORE the loop and
 *     indexed `q[1] * 4 + (u32) t + 0x48`, which keeps 0x48 as the load's
 *     displacement (matching-levers section 9); the +0x80 field is then read
 *     AGAIN inside the arm for func_8003AD08's second argument and a third
 *     time after the call, because the call invalidates cse.
 *   * The 0xFE and 0xFF arms share their tail -- `addu` / `lw` / `sw 0x1C` --
 *     by cross-jumping, which is why the 0xFE arm arrives at it with the
 *     D_8006A944 base in $v1 and the index in $v0.
 *   * D_8006A944 takes an incomplete-array `__asm__` alias: the listing
 *     materialises its address with `lui`/`addiu`, which the committed
 *     `extern u32 D_8006A944` scalar cannot do.
 *   * `q` is re-read from +0x1C inside each arm rather than reusing `pc`: the
 *     store through the +0x7C stack pointer invalidates cse, so the listing
 *     reloads.
 *
 * WHAT IS LEFT: 12 words, all inside the 0xFF arm and all schedule and
 * registers. The listing interleaves the four byte loads with the shifts and
 * adds -- `lbu r[1]`, `lbu q[1]`, `lbu r[2]`, `sll`, `addu`, `sll`,
 * `lbu r[3]`, `addu`, `sll`, `addu` -- and reuses $v1 for r[1] and r[3],
 * because r[1] is dead by the time r[3] is loaded. This source issues all
 * four loads first, so all four values are live at once and take four
 * registers, which also puts `r` in $v0 where the listing has $a1 and
 * serialises it behind the +0x7C increment's own $v0 temp.
 *
 * MEASURED and not working, all 98 words and all 12 DIFF: `r` declared before
 * `q`; `(u32) q[1]` to change the promotion; `*(u32 *) (p + 0x7C) += 4` for
 * the increment; and `r[1] << 8` hoisted into its own local to force that
 * load first. Also measured: the four bytes all off `q` (q[1]..q[4], 12
 * DIFF -- the listing genuinely mixes one read off `q` with three off `r`);
 * `pc[1]` in place of `q[1]` (16); the sum accumulated into a `u32` with four
 * `+=` statements (99 words); and the +0x7C store moved after the `q`/`r`
 * computation (97 words).
 */

#include "common.h"

extern u32 D_8006A944w[] __asm__("D_8006A944");

u8 *func_8003AD08(u32, u8 *);
void func_80039640(u8 *);

void func_80038F48(u8 *p)
{
    u8 *t;
    u8 op;

    op = **(u8 **) (p + 0x1C);
    t = *(u8 **) (p + 0x80);
    while ((u8) (op + 3) < 3 || op == 0)
    {
        u8 *pc = *(u8 **) (p + 0x1C);

        if (*(s8 *) pc == -3)
        {
            u8 *q;

            **(u8 ***) (p + 0x7C) = pc + 2;
            *(u8 **) (p + 0x7C) = *(u8 **) (p + 0x7C) + 4;
            q = *(u8 **) (p + 0x1C);
            *(u8 **) (p + 0x1C) = q + 1;
            func_8003AD08(*(u32 *) (q[1] * 4 + (u32) t + 0x48), *(u8 **) (p + 0x80) + 0x14);
            *(u8 **) (p + 0x1C) = *(u8 **) (p + 0x80) + 0x14;
        }
        else if (*(s8 *) pc == -2)
        {
            u8 *q;

            **(u8 ***) (p + 0x7C) = pc + 2;
            *(u8 **) (p + 0x7C) = *(u8 **) (p + 0x7C) + 4;
            q = *(u8 **) (p + 0x1C);
            *(u8 **) (p + 0x1C) = q + 1;
            *(u8 **) (p + 0x1C) = *(u8 **) (q[1] * 4 + (u32) D_8006A944w);
        }
        else if (*(s8 *) pc == -1)
        {
            u8 *q;
            u8 *r;

            **(u8 ***) (p + 0x7C) = pc + 5;
            *(u8 **) (p + 0x7C) = *(u8 **) (p + 0x7C) + 4;
            q = *(u8 **) (p + 0x1C);
            r = q + 1;
            *(u8 **) (p + 0x1C) = r;
            *(u8 **) (p + 0x1C) = *(u8 **) (q[1] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24));
        }
        else
        {
            func_80039640(p);
        }
        op = **(u8 **) (p + 0x1C);
    }
}
