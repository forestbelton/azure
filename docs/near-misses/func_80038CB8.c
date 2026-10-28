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
 *
 * 2026-10-28: IT IS NOT AN ALLOCATION-ORDER QUESTION, it is WHERE THE `sh`
 * GOES. Read the two streams side by side at the second loop's tail:
 *
 *   target  lhu 0x20 ; lw 0x1C ; addiu +1 ; sh 0x20 ; lb 0x28 ; addiu q+2 ;
 *           slt ; bnez ; sw 0x1C
 *   built   lw 0x1C ; lhu 0x20 ; lb 0x28 ; addiu q+2 ; addiu +1 ; slt ;
 *           sh 0x20 ; bnez ; sw 0x1C
 *
 * The listening completes the +0x20 read-modify-write FIRST, which frees $v0,
 * and the +0x28 temp then REUSES $v0 -- `lb $v0, 0x28($s0)`. This build leaves
 * the `sh` until after the `slt`, so $v0 is still holding the incremented
 * halfword when the temp is loaded and the temp has to take $v1; the carried
 * pointer is then pushed off $v1 to $a1, and the four words that name it
 * follow. So the register assignment is downstream of the schedule, and the
 * source question is what puts the `sh` ahead of the `lb`.
 *
 * Note the two MEMs do NOT conflict -- same base register, offsets 0x20 and
 * 0x28, sizes 2 and 1, so `memrefs_conflict_p` separates them before the
 * QImode clause of `true_dependence` is ever reached -- which is why sched is
 * free to order them either way and why the source order (the +0x20 update is
 * already written first) does not pin it. Measured 2026-10-28, all 164 words
 * and all 12 DIFF: the +0x20 update moved above `n--`, the +0x28 read taken
 * into a local immediately after that store, and both together. sched
 * normalises every one of them.
 *
 * WHY THE `sh` SINKS, and what would lift it: a store has no dependents inside
 * the block, so its sched priority is the floor and nothing in the source
 * changes that. The one thing that WOULD give it a dependent is a later read
 * of +0x20 in the same block -- the loop's own next-iteration read is across
 * the back edge, which sched never sees. So the shape to look for is one where
 * something after the store reads +0x20 (or any address sched cannot separate
 * from it) at no instruction cost. The prototype for func_8003AB44 in
 * this file was also stale: its arguments 3 and 6 are `s16`, which is how that
 * function landed on 2026-10-28, and correcting it changes nothing here (164
 * words, 12 DIFF either way) because every argument at these three call sites
 * is already an `s16` expression.
 */

#include "common.h"

void func_80038128(u8 *);
void func_80038F48(u8 *);
void func_80039148(u8 *);
void func_8003AB44(u8 *, s16, s16, u8, s16, s16, s32);

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
