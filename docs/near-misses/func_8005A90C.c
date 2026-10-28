/* func_8005A90C (unk_2CDC0) -- 100 of 103 words, 92 DIFF. NOT compiled.
 *
 * The third member of the func_8005A608 / func_8005A778 family, and the one
 * with the longest tail: it claims a D_80086A40 slot the same way the other
 * two do, fills in six fields, and then runs the handle through
 * func_80059F8C, func_8005ECA0 and func_8005EC40, bailing out with -1 at each
 * step. The tail is func_8005AAA8's (landed) word for word, so the +0x0C
 * store is spelled its way, through a `u8 *` cast.
 *
 * WHAT IS PAID FOR. The dispatch, the search loop and every instruction of
 * the body are in place: `j = id; id = id << 16; id = id >> 16;` for the
 * in-place sign-extension (matching-levers section 7, the family's shared
 * lever), `s16 i` so the counter's increments and its `< 0x10` test come out
 * as sll/sra pairs, `k = i` once at the join so there is a single
 * `sra $s2, $v0, 16` with two predecessors, and the six field stores in the
 * listing's order with `e->unk_10 = b` left for the `jal`'s delay slot.
 *
 * WHAT IS LEFT: FOUR words, and they are two callee-saved registers. The
 * listing uses six (p, b, c, i, k and the entry pointer all live across the
 * calls); this uses four, merging c with the entry pointer and i with
 * func_8005ECA0's result, which costs two save/restore pairs and cascades
 * into 92 differing register names. The merge is reload's, not the source's:
 * `c` is still live at `e->unk_08 = c`, which is a store THROUGH the entry
 * pointer, so the two overlap and cannot legally share -- the copy to `$a1`
 * that frees the register is func_80059F8C's own argument load, moved ahead
 * of the stores. Measured and not working: all six orders of the three
 * prologue statements (the committed one is best); `e` as `Unk80086A40 *`
 * with field accesses rather than a `u8 *` with casts; `s16 k`; and the
 * `(s16) id` cast form in place of the two shifts.
 *
 * ADVISE.PY'S CALLEE-SAVED HINT IS REFUTED HERE, 2026-10-28, and the refutation
 * is worth having because the hint scores better. It reports that the target
 * saves $s4 and $s5 where this build does not -- four words, two sw/lw pairs --
 * and says THE USUAL CAUSE IS A NARROW PARAMETER DECLARED WIDE. Measured:
 *
 *     s16 b                      101 words, 91 DIFF   <- one word closer
 *     s16 b + u16 c              101 words, 91 DIFF
 *     u16 b                      100 words, 95 DIFF
 *     u16 c                      100 words, 95 DIFF
 *     s16 id, shifts removed      96 words, 99 DIFF
 *     s16 id, shifts kept         73 words, 102 DIFF
 *
 * so `s16 b` looks like a gain. IT IS NOT THE LISTING'S SHAPE: the target's
 * prologue copies `b` with a PLAIN MOVE, `addu $s3, $a1, $zero`, with no
 * sign-extension anywhere -- compare `id` three insns later, which DOES get
 * the `sll 16`/`sra 16` pair. So `b` is genuinely `s32` there, and narrowing
 * it is the same class of error as the `s16 t` reverted below: it changes the
 * VALUE stored by `*(u32 *) (e + 0x10) = b` and passed to
 * `func_80059F8C(s32, u32)`, and it scores better for an unrelated reason.
 *
 * THE THREE MERGES, from `--greg` (this park had no allocator numbers). Seven
 * allocnos cross a call and they land in only FOUR registers, because three
 * pairs share:
 *
 *     87  13 refs /  27 live / 1.444 / xcall 3  -> $s0
 *     82   2 refs /  94 live / 0.021 / xcall 1  -> $s0   <- shares
 *     84  11 refs /  50 live / 0.660 / xcall 1  -> $s1
 *     89   2 refs /   7 live / 0.286 / xcall 1  -> $s1   <- shares
 *     86   4 refs /  31 live / 0.258 / xcall 3  -> $s2
 *     81   2 refs /  96 live / 0.021 / xcall 1  -> $s2   <- shares
 *     80   4 refs / 146 live / 0.055 / xcall 4  -> $s3
 *
 * Two of those three have to come apart to reach the listing's six. Note what
 * the three low rows have in common: 2 references and a live length near 95 or
 * 146, which is `update_equiv_regs`' DOUBLING of a once-set parameter -- so
 * 80, 81 and 82 are parameters and their real ranges are half what is printed.
 * A pair sharing a register means global-alloc found NO CONFLICT between them,
 * so each merge is a claim that the two values are never live at the same
 * time; the fix is to find which of those claims the listing contradicts and
 * make the two overlap, not to move a priority.
 *
 * WHY THERE IS NO CONFLICT, from the `.greg` conflict lists and the `.lreg`
 * RTL, 2026-10-28 -- and this is the thread to pull. The lists say the merges
 * are legal, not accidental:
 *
 *     ;; 81 conflicts: 80 81 82 84 85 92 93 105 119 ...
 *     ;; 82 conflicts: 80 81 82 84 85 92 93 105 119 ...
 *     ;; 86 conflicts: 80 84 86 87 89 143 ...
 *     ;; 87 conflicts: 80 84 86 87 89 143 ...
 *
 * so {81, 82} and {86, 87} are two disjoint groups: the parameters never
 * overlap `k` or the entry pointer at all. That contradicts the source, where
 * `*(u32 *) (e + 0x14) = c` plainly uses a parameter after `e` exists -- and
 * the RTL says why:
 *
 *     (insn 153 (set (mem:SI (plus:SI (reg/v:SI 87) (const_int 20)))
 *                    (reg:SI 5 a1)))
 *
 * the stored value is the INCOMING HARD REGISTER $a1, not a pseudo. gcc is
 * keeping that parameter in its argument register across the block rather than
 * copying it to a pseudo, so there is no allocno to conflict with 87 and
 * nothing for global-alloc to separate. The listing cannot be doing that: it
 * copies three parameters to callee-saved registers in the prologue
 * ($s5 = p, $s3 = b, $s4 = c), which is what its six registers are FOR.
 *
 * So the question is no longer "which priority moves" but "why does this build
 * get to use $a1 directly there". The likely answer is the path: insn 153 sits
 * on the `id == -1` arm, which reaches `found:` by `goto` WITHOUT passing
 * func_8005A1D0's call, so on that path $a1 is still intact. The listing's
 * prologue copies happen before the dispatch, i.e. unconditionally. Something
 * about this source lets gcc prove the argument register survives, and the
 * first thing to check is whether the two paths into `found:` really are
 * equivalent in the listing.
 *
 * Which leaves the callee-saved finding itself standing and its cause open:
 * the listing keeps SIX values across calls and this build four. The target's
 * prologue names them -- $s5 = p, $s3 = b, $s4 = c, $s1 = i -- so the two
 * extra are among the rest, and the question is which value this build lets
 * die that the listing keeps.
 *
 * The family's other two parks (func_8005A608 at 11 DIFF, func_8005A778 at
 * 21) are stuck on a different fact -- which register the entry's `-1`
 * comparison constant lands in, and whether cc1psx materialises it once or
 * three times -- so this one is NOT blocked on them.
 *
 * SINCE PARKED, 2026-10-27: 99 of 103 words to 102 of 103, by declaring both
 * `r` and `t` `s16` rather than `s32` and `u32`. Three words, found by
 * search.py's declaration-width family in the sweep of all forty parks -- it
 * needed BOTH, which is why a single-change sweep had to compose them over
 * two passes. The differing count stays at 92 because one word is still
 * missing and everything after it reads as wrong; advise.py's branch spans
 * are the way to localise it, and the notes below predate these numbers.
 *
 * A SWEEP RESULT WAS REVERTED HERE, 2026-10-27: search.py's
 * declaration-width family proposed `s16 t` and it scores better, but it is
 * SEMANTICALLY WRONG -- t takes func_8005ECA0's return value, declared `u32 func_8005ECA0(u32)`, and is stored with `*(u32 *) (e + 0x10) = t`.
 *
 * The companion narrowing `s16 r` IS kept -- it is worth one word -- but it
 * is NOT proven: func_80059F8C is declared `s32 func_80059F8C(s32, u32)`,
 * `r` is compared against -1 and stored through `*(u32 *) (e + 0x10)`, so
 * s16 is faithful only if that function cannot return a value outside s16
 * range. func_80059F8C is itself still an asm stub, so nothing settles it
 * here. Check that before trusting this park's shape.
 * The score is an oracle for SHAPE, not for correctness, and the width
 * family is the one that can change VALUES rather than representations.
 * See docs/matching-levers.md on reviewing a narrowing against the
 * listing's own arithmetic before keeping it.
 */

#include "common.h"

extern s32 D_8007382C[];

void func_8005A1D0(u32);
s32 func_80059F8C(s32, u32);
u32 func_8005ECA0(u32);
u32 func_8005EC40(u8 *, u32);

s32 func_8005A90C(u8 *p, s32 b, u32 c, s32 id)
{
    s16 i;
    s16 j;
    s32 k;
    u8 *e;
    s16 r;
    u32 t;

    D_8007382C[0] = 0;
    i = 0;
    j = id;
    id = id << 16;
    id = id >> 16;
    if (id == -1)
    {
        do
        {
            if (D_80086A40[i].unk_00 == -1)
            {
                goto found;
            }
            i++;
        } while (i < 0x10);
        return -1;
    }
    i = j;
    if (D_80086A40[i].unk_00 != -1)
    {
        func_8005A1D0(D_80086A40[i].unk_10);
    }
found:
    k = i;
    e = (u8 *) &D_80086A40[k];
    *(u32 *) (e + 0x14) = c;
    e[0x18] = 0x7F;
    *(s16 *) e = i;
    *(u8 **) (e + 4) = p;
    *(u32 *) (e + 8) = c;
    e[0x1B] = 0x40;
    *(u32 *) (e + 0x10) = b;
    r = func_80059F8C(b, c);
    *(u32 *) (e + 0x10) = r;
    if (r == -1)
    {
        return -1;
    }
    t = func_8005ECA0(r);
    if (func_8005EC40(p, *(u32 *) (e + 0x14)) != *(u32 *) (e + 0x14))
    {
        return -1;
    }
    *(u32 *) (e + 0xC) = (u32) p;
    *(u32 *) (e + 0x10) = t;
    return k;
}
