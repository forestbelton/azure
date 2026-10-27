/* func_8004AC3C (unk_1C540) -- 129 of 128 words, 94 DIFF. NOT compiled.
 *
 * Resolves an item/monster record pointer from the two bytes at p[0]/p[1],
 * with a different table and a different field depending on D_80082E6A's mode
 * byte, func_80042A80's flags, and whether the D_80073414 entry's +0x00 is
 * set; `*out` reports which kind was resolved.
 *
 * THE STRUCTURE IS VERIFIED against the listing and is the value of this file;
 * what is left is one allocation fact. Worth keeping because re-deriving 128
 * words of control flow is the expensive part.
 *
 * WHAT IS ESTABLISHED
 *
 * * Labels and gotos, as in func_8003B470: the `D_80073414[p[1]].unk_00`
 *   test appears TWICE (once on the `func_80042A80() == 0` path and once in
 *   the mode-1 path) and both share the `p[0] * 2` block and the final
 *   `(n + p[0]) * 4 + 4` load. `odd:` and `join:` are those two shared blocks.
 *
 * * The index arithmetic is the func_80049374 family's: `n = p[0] * 4` or
 *   `p[0] * 2` with ONE shared `(n + p[0]) * 4`, which is the `sll 2`/`sll 1`
 *   in the two delay slots plus the shared `addu`/`sll 2`. The two
 *   `func_80042A80() != 0` arms instead spell `p[0] * 20` out in full, because
 *   their final offsets differ (+4 against +0xC) so there is no shared tail.
 *
 * * Mode 1's 0x13 case is a RAW CONSTANT base, `(u8 *) 0x80010324`, times
 *   0x54 -- `lui`/`ori` rather than `lui`/`addiu`, which is the tell from
 *   docs/addressing-and-translation-units.md.
 *
 * * `D_80082E6A` through the incomplete-array alias, as unk_72A8.c declares
 *   it, for the compiler-split form the listing has.
 *
 * * One `return r` at the end: every path writes `r` and jumps to the single
 *   exit, which is the listing's `.L8004AE1C` with its `addu $v0, $v1, $zero`.
 *
 * WHAT IS LEFT
 *
 * The target RE-READS `p[1]` at the head of each of the four blocks that index
 * D_80073414 -- `lbu $a0, 0x1($s0)` with the `addiu %lo(D_80073414)` filling
 * its load-delay slot -- while this source reads it once for the test chain
 * and cse forwards that value into every block. That accounts for both the
 * four extra `nop`s advise.py reports and the register swap that follows
 * ($a0 holds `f` and $v1 holds `p[1]` in the target; reversed here).
 *
 * Only ONE of the four re-reads has an explanation: the mode-1 block is
 * preceded by `*out = 7`, a store through a pointer, which invalidates
 * everything in cse. The other three have no store and no call between the
 * test chain and the block, so why cse did not forward there is the open
 * question -- and it is the whole remaining diff.
 *
 * Measured and NOT working: `u32 f` rather than `s32` (94 DIFF); flattening
 * the `func_80042A80` test so the zero case comes first (101); and
 * `*(volatile u8 *) (p + 1)` in all four blocks, which DOES produce the four
 * re-reads and drops the diff to 62 -- but volatile pins each load so its
 * delay slot stays empty, and the function comes out 131 words. So the
 * re-reads are ordinary loads that cse declined to fold, not volatile ones.
 *
 * SINCE PARKED, 2026-10-27: 129 of 128 words and 94 DIFF to 128 of 128 and
 * 90, by declaring `r` `u16` rather than `s32` -- the word count is now
 * exact. Found by search.py's declaration-width family in the sweep of all
 * forty parks. advise.py's verdict is SHAPE, so what is left is still source
 * shape rather than allocation, and the notes above were written against the
 * old numbers.
 *
 * A SWEEP RESULT WAS REVERTED HERE, 2026-10-27: search.py's
 * declaration-width family proposed `u16 r` and it scores better, but it is
 * SEMANTICALLY WRONG -- r is assigned `(s32) ((u8 *) 0x80010324 + ...)` and several `*(s32 *)` reads -- it holds ADDRESSES, which a u16 truncates.
 * The score is an oracle for SHAPE, not for correctness, and the width
 * family is the one that can change VALUES rather than representations.
 * See docs/matching-levers.md on reviewing a narrowing against the
 * listing's own arithmetic before keeping it.
 */

#include "common.h"

extern u8 D_80082E6Aa[] __asm__("D_80082E6A");

s32 func_80042A80(u8 *);
s32 func_80049330(u8 *);

s32 func_8004AC3C(u8 *p, s32 *out)
{
    Unk80073414 *e;
    s32 r;
    s32 n;
    s32 f;

    *out = 0;
    if (D_80082E6Aa[0] != 1)
    {
        if (p[1] == 0x13)
        {
            r = func_80049330(p) + 0x34;
        }
        else
        {
            f = func_80042A80(p);
            if (f != 0)
            {
                if ((u32)(p[1] - 0xF) < 3 || (p[1] == 4 && (f & 0x400) == 0))
                {
                    r = *(s32 *)(D_80073414[p[1]].unk_0C + p[0] * 20 + 4);
                }
                else
                {
                    r = *(s32 *)(D_80073414[p[1]].unk_0C + p[0] * 20 + 0xC);
                }
                *out = 4;
            }
            else
            {
                e = &D_80073414[p[1]];
                if (e->unk_00 != 0)
                {
                    goto odd;
                }
                n = p[0] * 4;
                goto join;
            }
        }
    }
    else
    {
        if (p[1] == 0x13)
        {
            r = (s32)((u8 *)0x80010324 + (p[3] & 0x1F) * 0x54);
        }
        else
        {
            if (p[1] == 0x16)
            {
                *out = 7;
            }
            e = &D_80073414[p[1]];
            if (e->unk_00 != 0)
            {
            odd:
                n = p[0] * 2;
            }
            else
            {
                n = p[0] * 4;
            }
        join:
            r = *(s32 *)(e->unk_0C + (n + p[0]) * 4 + 4);
        }
    }
    return r;
}
