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
 *
 * 2026-10-28: A 103-WORD / 59-DIFF VARIANT THAT IS NOT ADOPTED -- the better
 * number off a shape the listing does not have, the same call func_80056D44
 * and func_8005914C get. The change is one statement:
 *
 *     cc = c;                      <- a copy of the third parameter, made
 *                                     BEFORE the id dispatch
 *     ...
 *     *(u32 *) (e + 0x14) = cc;    <- the two stores read the COPY
 *     *(u32 *) (e + 8)    = cc;
 *     r = func_80059F8C(b, c);     <- the call still reads the RAW parameter
 *
 * This is the thread the paragraph above says to pull, and it closes it. The
 * build was storing the INCOMING HARD REGISTER (`(set (mem (plus (reg 87) 20))
 * (reg:SI 5 a1))`) because the `id == -1` arm reaches `found:` by `goto`
 * without passing func_8005A1D0's call, so on that path $a1 is untouched and
 * there was no allocno for global-alloc to conflict with pseudo 87. Copying
 * the parameter ABOVE the dispatch gives it a pseudo that is live on both
 * paths.
 *
 * WHAT THE THREE WORDS ACTUALLY ARE, corrected after checking rather than
 * assumed: NOT the prologue parameter copies. The 100-word base already had
 * all three of those (`move $s3, $a0`, `move $s2, $a1`, `move $s0, $a2`), so
 * the first reading of this was wrong. Diffing the two builds, the copy adds a
 * FOURTH callee-saved register -- `sw $s3, 0x1C($sp)`, `move $s3, $s0`, and
 * the matching `lw` in the epilogue -- which is the cross-path home for `c`
 * that the $a1 store made unnecessary before. The frame does not grow for it
 * (0x28 had the slack; `sw $ra` just moves from 0x20 to 0x24).
 *
 * THE SPLIT BETWEEN COPY AND RAW IS LOAD-BEARING, which is the same shape as
 * func_8005BBFC's `vel` fix: cse cannot share a raw parameter read with a copy,
 * so each keeps its own reference. Measured:
 *
 *     stores read `cc`, call reads `c`     103 words, 59 DIFF   <- this file
 *     stores AND call read `cc`            100 words, 95 DIFF
 *     the same for `b` as well             106 words, 102 DIFF
 *     the same for `p` as well             106 words, 102 DIFF
 *
 * and the copy's POSITION does not matter: before `D_8007382C[0] = 0`, after
 * it, or after the two shifts are all 103 words and the same 59.
 *
 * AND IT IS NOT THE ONLY PARAMETER THAT WORKS, which bounds the explanation
 * above. `search.py`'s new `copy_param` family, run on the 100-word base,
 * finds `copy param p (stores only)` at the same 103 words / 59 DIFF. The two
 * builds are NOT the same: they differ in five words and so miss the target in
 * different places. So what buys the word count is a cross-path pseudo for
 * ANY of the parameters the `found:` block stores, and the `$a1` account above
 * explains why the `c` form works rather than why it is the only one. Whether
 * `c` or `p` is the listing's own shape is undecided -- both are 59 -- and
 * picking between them is the next question, not finding a third.
 *
 * WHY IT IS NOT ADOPTED, read off the listing's own prologue:
 *
 *     addiu $sp, $sp, -0x30
 *     sw $s5, 0x24($sp) ; addu $s5, $a0, $zero     <- p
 *     sw $s3, 0x1C($sp) ; addu $s3, $a1, $zero     <- b
 *     sw $s4, 0x20($sp) ; addu $s4, $a2, $zero     <- c
 *     sw $s1, 0x14($sp) ; addu $s1, $zero, $zero   <- i
 *     ... sw $s2, 0x18($sp) ... sw $s0, 0x10($sp)
 *
 * SIX callee-saved registers ($s0-$s5) and EXACTLY THREE parameter copies.
 * The 100-word base already has those three; both 59-DIFF variants add a
 * FOURTH copy -- a duplicate of a parameter in a fifth callee-saved register
 * (`sw $s3`, `move $s3, $s0`, `lw $s3`), which is where their three words come
 * from. The listing has no such duplicate. So the word count goes exact for
 * the wrong reason, and a park carrying that source would tell the next
 * attempt the shape is right when it is not.
 *
 * WHAT THE LISTING ACTUALLY WANTS -- stated from both register assignments
 * rather than guessed. The first version of this note said "a sixth
 * callee-saved register holding something that is NOT a parameter", and that
 * is wrong: the target's two extra registers hold `c`, which IS a parameter,
 * and `k`. The two sides are
 *
 *     target   $s5 = p   $s4 = c   $s3 = b   $s2 = k   $s1 = i   $s0 = e
 *              six registers, each with ONE value
 *     built    $s3 = p   $s2 = b then k   $s1 = i then a later value
 *              $s0 = c then e
 *              four registers, three of them REUSED
 *
 * so the difference is not a missing register, it is that the listing's six
 * values are ALL LIVE AT ONCE while this build's ranges are disjoint enough to
 * share. `move $s0, $a2` followed later by `addu $s0, $v1, $v0` is gcc deciding
 * `c` is dead before `e` exists; `move $s2, $a1` then `sra $s2, $v0, 0x10` is
 * the same for `b` against `k`. That is why adding a duplicate copy scores the
 * three words and is still the wrong shape: it adds a fifth register instead
 * of making two existing ranges overlap.
 *
 * So the question for the next attempt is which of those overlaps the source
 * can create: `c` still live when `e` is formed, or `b` still live when `k`
 * is. Note `c` IS read through `e` in this source
 * (`*(u32 *) (e + 0x14) = c`), so gcc is reaching those stores with the
 * parameter still in $a1 -- the insn-153 fact above -- and that is the same
 * finding seen from the register side rather than the RTL side.
 *
 * 2026-10-28, WHICH PASS PUTS $a1 IN THAT STORE -- it is LOCAL-ALLOC, not
 * combine, and that relocates the whole question. Counting `(reg:SI 5 a1)` in
 * each dump of this source:
 *
 *     rtl 5    cse 5    loop 5    combine 8    flow 8    lreg 10
 *
 * and the +0x14 store itself still reads `(reg/v:SI 82)` in the COMBINE dump:
 *
 *     (set (mem:SI (plus:SI (reg/v:SI 87) (const_int 20))) (reg/v:SI 82))
 *
 * So the park's insn-153 reading -- taken from `.lreg` -- is of RTL that
 * local-alloc had already rewritten. Nothing before local-alloc puts the
 * incoming register in that store.
 *
 * TWO THINGS THAT FOLLOW. First, `optimize_reg_copy_1`'s refusal to touch hard
 * registers is `#ifdef SMALL_REGISTER_CLASSES` (local-alloc.c:712-716, comment
 * "We don't want to mess with hard regs if register classes are small"), and
 * config/mips never defines it -- so on this target local-alloc DOES rewrite
 * hard-register copies, and that is the third place today where that macro is
 * the deciding fact (the others are combine's REG_USERVAR_P tests and
 * `can_combine_p`'s reg-to-reg escape). Second, pseudo 82 is listed at TWO
 * references in `--greg`, which is `update_equiv_regs`' substitution rule
 * exactly -- set once, used once, multi-block -- the same rule that sinks
 * func_8005BBFC's parameter loads.
 *
 * AND THE SUBSTITUTION IS NOW TRACED, which rules out the obvious cure.
 * Pseudo 82 appears FOUR times in the `rtl` dump -- its set at insn 8 plus
 * exactly the three uses `c` has in this source -- so `c` is ONE pseudo and is
 * not split across several, and the `update_equiv_regs` two-reference rule
 * (set once, used once, multi-block) does NOT apply to it. Comparing the
 * `flow` and `lreg` dumps use by use:
 *
 *     flow uses 82 in insns   8, 153, 167, 180
 *     lreg uses 82 in insns   8,           180
 *     insn 153  (set (mem (plus (reg 87) 20)) (reg:SI 5 a1))   <- +0x14 store
 *     insn 167  (set (mem (plus (reg 87) 8))  (reg:SI 5 a1))   <- +8 store
 *
 * so local-alloc replaced the TWO STORES with the incoming register and left
 * the call argument (insn 180) on the pseudo. THAT IS WHERE `--greg`'S "2
 * refs" FOR PSEUDO 82 COMES FROM: it is the count AFTER the substitution, not
 * the source's. Any allocator arithmetic in this file built on that 2 is
 * arithmetic about a derived number.
 *
 * WHAT IS STILL NOT ESTABLISHED, said plainly: WHICH routine in local-alloc
 * does it. `optimize_reg_copy_1` is permitted to -- its "don't mess with hard
 * regs" guard is `#ifdef SMALL_REGISTER_CLASSES` (local-alloc.c:712) and
 * config/mips never defines it -- but the replacement it performs runs the
 * other way, SRC into DEST, so it is not obviously the one. Do not attribute
 * this to a named function without checking; two readings of this park have
 * already been wrong, one blaming combine and one calling the two extra
 * registers "not a parameter".
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
