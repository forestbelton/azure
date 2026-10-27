/* func_80038CB8 (unk_72A8) -- 164 of 164 words, 12 DIFF. NOT compiled.
 *
 * The tile-row emitter: walks the 2-byte records at +0x1C, handing each one to
 * func_8003AB44 with the row position, and stops at a record whose first byte
 * is positive. Three near-identical blocks -- the +0x2E flag picks the first,
 * and a negative +0x28 picks the second (which also counts down and then
 * reloads +0x28 from the +0x80 object) with the third being the single-record
 * case that installs func_80038128.
 *
 * EVERY instruction matches. What is left is one register in the second
 * block; see the last section.
 *
 * Four shapes paid for, and the first two are the same lever read twice:
 *
 *   * The loop's record pointer is TWO locals, not one walked pointer. The
 *     listing's `lb $v1, 0x2($v1)` is the condition reading the NEXT record
 *     through the OLD pointer as a displacement, while the loop top reads the
 *     NEW one -- so `r` holds `*(u8 **) (p + 0x1C)` and the condition is
 *     `*(s8 *) (r + 2) <= 0`. Advanced as `q += 2` the condition is
 *     `lb 0x0($v0)` and the back edge's delay slot goes empty, which is ten
 *     words.
 *
 *   * The FIRST loop's carried pointer is a READ-BACK of the field it just
 *     stored (`q = *(u8 **) (p + 0x1C)` after the store), which is cse's
 *     reg-to-reg move and the listing's `addu $a0, $v0, $zero` in that delay
 *     slot. The SECOND loop's is the plain `q = r + 2`, where the listing has
 *     no copy and a nop. The two loops really do differ here.
 *
 *   * The two loops use SEPARATE locals for the carried pointer. Sharing one
 *     puts both in the same register, and the listing gives the first `$a0`
 *     and the second `$v1`, which one pseudo cannot do (same reason
 *     func_80037714 needed two uv accumulators). The OLD pointer, on the
 *     other hand, is the same local in both -- sharing it is worth one word
 *     over a second fresh one.
 *
 *   * `p[0x28]` is read with `lb` throughout, so it is `*(s8 *) (p + 0x28)`
 *     and the second block's guard is `< 0`, not `!= 0`.
 *
 * WHAT IS LEFT: 12 words, all of the second loop, and one allocation decision
 * behind them. The carried pointer wants `$v1` and gets `$a1`, because
 * local-alloc hands `$v1` to the `lb 0x28($s0)` temp first; the listing has
 * that temp in `$v0` and the pointer in `$v1`. Both are in the same situation
 * -- a loop-carried value whose range ends at the loop-top read, against a
 * local temp -- so this is the allocation-order question of matching-levers
 * section 3 and not a source shape. Measured and not working: every
 * assignment of the two loops' four pointer locals (q/r/u/w in all
 * combinations, 12 to 19 DIFF); the declaration order of all four; reading
 * +0x28 into a local; `*(s8 *) (p + 0x28) > n` instead of `n < ...`; a
 * read-back for the second loop's carried pointer (165 words); and three
 * orders of the second loop's tail.
 */

#include "common.h"

void func_80038128(u8 *);
void func_80038F48(u8 *);
void func_80039148(u8 *);
void func_8003AB44(u8 *, s16, s32, u8, s16, s32, s32);

void func_80038CB8(u8 *p)
{
    u8 *q;
    s32 n;
    u8 *r;
    u8 *u;
    u8 *w;

    if (p[0x2E] != 0)
    {
        q = *(u8 **) (p + 0x1C);
        if (*(s8 *) q <= 0)
        {
            do
            {
                if (*q != 0 && *(s16 *) (p + 0x20) >= p[0x24])
                {
                    func_80039148(p);
                    return;
                }
                func_80038F48(p);
                if (*(s8 *) *(u8 **) (p + 0x1C) > 0)
                {
                    return;
                }
                func_8003AB44(*(u8 **) (p + 0x1C), *(s16 *) (p + 0x20), *(s16 *) (p + 0x22),
                              p[0x2A], *(s16 *) p, (s16) (*(u16 *) (p + 2) + 2), 1);
                *(u16 *) (p + 0x20) = *(u16 *) (p + 0x20) + 1;
                r = *(u8 **) (p + 0x1C);
                *(u8 **) (p + 0x1C) = r + 2;
                q = *(u8 **) (p + 0x1C);
            } while (*(s8 *) (r + 2) <= 0);
        }
    }
    else if (*(s8 *) (p + 0x28) < 0)
    {
        n = 0;
        u = *(u8 **) (p + 0x1C);
        if (*(s8 *) u <= 0)
        {
            do
            {
                if (*u != 0 && *(s16 *) (p + 0x20) >= p[0x24])
                {
                    func_80039148(p);
                    return;
                }
                func_80038F48(p);
                if (*(s8 *) *(u8 **) (p + 0x1C) > 0)
                {
                    return;
                }
                func_8003AB44(*(u8 **) (p + 0x1C), *(s16 *) (p + 0x20), *(s16 *) (p + 0x22),
                              p[0x2A], *(s16 *) p, (s16) (*(u16 *) (p + 2) + 2), 1);
                n--;
                *(u16 *) (p + 0x20) = *(u16 *) (p + 0x20) + 1;
                q = *(u8 **) (p + 0x1C);
                u = q + 2;
                *(u8 **) (p + 0x1C) = u;
                if (n < *(s8 *) (p + 0x28))
                {
                    break;
                }
            } while (*(s8 *) (q + 2) <= 0);
        }
        p[0x28] = (*(u8 **) (p + 0x80))[3];
    }
    else
    {
        func_80038F48(p);
        if (*(s8 *) *(u8 **) (p + 0x1C) > 0)
        {
            return;
        }
        func_8003AB44(*(u8 **) (p + 0x1C), *(s16 *) (p + 0x20), *(s16 *) (p + 0x22),
                      p[0x2A], *(s16 *) p, (s16) (*(u16 *) (p + 2) + 2), 1);
        *(u16 *) (p + 0x20) = *(u16 *) (p + 0x20) + 1;
        *(u8 **) (p + 0x1C) = *(u8 **) (p + 0x1C) + 2;
        if (*(s8 *) (p + 0x28) != 0)
        {
            *(s16 *) (p + 0x18) = *(s8 *) (p + 0x28);
            *(u32 *) (p + 0x10) = (u32) func_80038128;
        }
    }
}
