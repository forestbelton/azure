/* func_80059814 (unk_2CDC0) -- 94 of 103 words, 69 DIFF. NOT compiled.
 *
 * Advances the +0x10 accumulator by the +0x40 step at a rate set by the
 * current video mode in D_800869A8, then splits the result into a remainder
 * stored back at +0x40 and a quotient stored at +0x10. Ten case values in
 * six bodies.
 *
 * WHAT IS PAID FOR -- the whole structure, verified instruction by
 * instruction against the listing:
 *
 * * the switch's TEN labels in SIX bodies, in the listing's block order:
 *   0x30 (x10), 0x60 (x5), 0xC0/0xF0 (&1, >>1), 0x120/0x168 (/3 and return),
 *   0x180/0x1E0 (&3, >>2), 0x300/0x3C0 (&7, >>3), default return. The
 *   dispatch tree, every `sltiu` bound and all the `beq` constants match.
 * * the subject is UNSIGNED: `D_800869A8` is an incomplete `u32` array read
 *   as `D_800869A8[0]`, which is what makes the bounds `sltiu` rather than
 *   `slti`.
 * * the two tail stores are shared -- `p->unk_40 = m; p->unk_10 = t;` once
 *   after the switch, with the bodies setting `m` and `t` and `goto`ing /
 *   breaking to it. The 0x120 body and the default bypass it.
 * * each body's accumulate is TWO statements, `*= k` then `+= p->unk_40`, so
 *   the listing's two `sw 0x10` per body are both real. **flow does not
 *   delete the first of them, and the reason is the block boundary**: the
 *   overwrite is in the shared tail, and flow's `mem_set_list` is reset per
 *   basic block in `propagate_block`, so a store at the end of one block is
 *   never matched against an overwrite in another. All nine stores to +0x10
 *   survive to `lreg` in this source (checked in the dumps) -- the loss is
 *   later.
 *
 * WHAT IS LEFT: 9 words, and they are ONE sched1 decision.
 *
 * `jump2` -- the cross-jumping pass, which runs after reload and before
 * sched2 -- moves each body's trailing `sw 0x10` into the shared tail,
 * because sched1 has put it LAST in the body. That saves five words, and the
 * five `addu $v1, $v0, $zero` copies the listing spends go with them: with
 * the store last, `m` and `t` do not conflict with the accumulated value, so
 * global-alloc gives them $a2 and $a0 instead of $v0 and $v1 and the
 * destructive `andi` needs no copy.
 *
 * sched.py --sched1 prices it exactly. In the 0x30 body (basic block 17) the
 * insns are 15 `lw 0x10`, 18 `sll 2`, 20 `addu`, 28 `lw 0x40`, 21 `sll 1`,
 * 23 `sw 0x10` (the x10 store), 30 `addu` (the +=), 32 `sw 0x10` (the +=
 * store), 36 `andi` (m), 40 `srl` (t). Insns 23, 32, 36 and 40 all have
 * priority 2, and the trace reads:
 *
 *     ready list at T-2: 32 (2) 36 (2) 40 (2), now 40 36 32
 *     insn 32 has a greater potential hazard, now 32 40 36
 *     ready list at T-3: 40 (2) 36 (2) 23 (2), now 40 36 23
 *     insn 23 has a greater potential hazard, now 23 40 36
 *
 * -- so `schedule_select` takes both STORES first in the backward pass,
 * which puts them last in the forward one. The listing has them interleaved
 * with the arithmetic (23 right after the `sll 1`, 32 right after its
 * `addu`), i.e. the two `and`/`lshiftrt` insns taken first instead. That is a
 * DEFINE_FUNCTION_UNIT decision, not a dependence one, so no statement order
 * reaches it: measured at 94 words for all of an explicit `x` local for the
 * sum, the two tail stores reversed, `t` computed before `m`, and the tail
 * stores written out per body instead of shared (the last merges even more).
 * Writing the two stores per body and the `& `/`>>` inline, with no locals at
 * all, is 105 words and 73 DIFF -- the opposite error.
 *
 * The lead to take next is the other half of the same cluster as
 * func_80035E58: what changes `potential_hazard`'s choice inside one
 * priority group. Both functions now have the trace lines that show it.
 */

#include "common.h"

extern u32 D_800869A8[];

void func_80059814(u8 *p)
{
    u32 m;
    u32 t;

    switch (D_800869A8[0])
    {
    case 0x30:
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) * 10;
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) + *(u32 *)(p + 0x40);
        m = *(u32 *)(p + 0x10) & 3;
        t = *(u32 *)(p + 0x10) >> 2;
        goto store;
    case 0x60:
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) * 5;
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) + *(u32 *)(p + 0x40);
        m = *(u32 *)(p + 0x10) & 3;
        t = *(u32 *)(p + 0x10) >> 2;
        goto store;
    case 0xC0:
    case 0xF0:
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) + *(u32 *)(p + 0x40);
        m = *(u32 *)(p + 0x10) & 1;
        t = *(u32 *)(p + 0x10) >> 1;
        goto store;
    case 0x120:
    case 0x168:
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) / 3;
        return;
    case 0x180:
    case 0x1E0:
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) + *(u32 *)(p + 0x40);
        m = *(u32 *)(p + 0x10) & 3;
        t = *(u32 *)(p + 0x10) >> 2;
        goto store;
    case 0x300:
    case 0x3C0:
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) + *(u32 *)(p + 0x40);
        m = *(u32 *)(p + 0x10) & 7;
        t = *(u32 *)(p + 0x10) >> 3;
        goto store;
    default:
        return;
    }
store:
    *(u32 *)(p + 0x40) = m;
    *(u32 *)(p + 0x10) = t;
}
