/* func_80054F9C (unk_1C540) -- 28 of 28 words, MATCH. NOT COMPILED, and the
 * reason is placement, not codegen.
 *
 * Sets a controller's step size from the command byte: 0xB1/0xB4 -> 0xF,
 * 0xC1/0xC4 -> 4, 0xD1/0xD4 -> 2, anything else -> 0xA, into +0x18; and if
 * +0x04 is not already 2 it latches +0x08 into +0x0A first.
 *
 * MATCHED ON THE FIRST COMPILE once the table was read. The shapes, such as
 * they are:
 *
 * * The dispatch is a 36-entry JUMP TABLE over 0xB1..0xD4 -- six case labels
 *   in three bodies plus the default -- so the case VALUES and the body order
 *   come out of jtbl_80032E14 (file 0x6614) and nothing else. Read, the table
 *   pairs 0xB1 with 0xB4, 0xC1 with 0xC4 and 0xD1 with 0xD4, and the bodies
 *   are laid out in that ascending order with `default:` last.
 *
 * * The first parameter is `s32` and the mask is INSIDE (`switch (k & 0xFF)`).
 *   Declared `u8`, the two callers in func_80054788 would mask at the call
 *   instead -- they pass `n & 0xFFF1` and `n & 0xFFF4` -- and those two
 *   functions already match.
 *
 * * `k & 0xFF` is non-negative, so the range check is `sltiu`; the listing
 *   agrees. The two `andi $v0, $a0, 0xFF` are ONE source expression, reorg
 *   having copied it into the `beq`'s delay slot.
 *
 * WHY IT IS NOT LANDED. cc1psx emits jtbl_80032E14 into unk_1C540's
 * `.rodata`, and that section is already spoken for: jtbl_8003310C is placed
 * at 0x8003310C as the unit's only run (see the comment on func_800595C0).
 * jtbl_80032E14 belongs 0x2F8 bytes earlier, in the three-table run at
 * 0x80032E14 -- jtbl_80032E14 (36 entries), jtbl_80032EA4 (23) and, after the
 * `.align 3` pad, jtbl_80032F04 (126) -- which is 744 bytes and needs ALL
 * THREE of its functions in C before its length is right.
 *
 * So this is the first of three, and the other two are func_80057A94 (163
 * words, 23 entries) and func_80057D20 (463 words, 126 entries). With all
 * three matching, unk_1C540 splits in TWO at func_800595C0: the first piece's
 * `.rodata` is the 0x80032E14 run and the second's is jtbl_8003310C, and the
 * functions whose tables sit in the three EARLIER runs are all still
 * `INCLUDE_ASM` so they contribute nothing to either. Three functions land at
 * once, and the split is the staged form in docs/open-matching-problems.md.
 */

#include "common.h"

void func_80054F9C(s32 k, u8 *q)
{
    s32 r;

    if (*(u32 *) (q + 4) != 2)
    {
        *(u32 *) (q + 4) = 2;
        *(s16 *) (q + 0xA) = *(u16 *) (q + 8);
    }
    switch (k & 0xFF)
    {
    case 0xB1:
    case 0xB4:
        r = 0xF;
        break;
    case 0xC1:
    case 0xC4:
        r = 4;
        break;
    case 0xD1:
    case 0xD4:
        r = 2;
        break;
    default:
        r = 0xA;
        break;
    }
    *(s16 *) (q + 0x18) = r;
}
