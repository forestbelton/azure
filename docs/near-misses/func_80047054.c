/* func_80047054 (unk_19434) -- 58 of 58 words, 24 DIFF. NOT compiled.
 *
 * Every word count, and for a long time every opcode, immediate, displacement
 * and nop was identical too, with the only difference a permutation of five
 * registers that followed from one decision -- see below. advise.py now also
 * reads EXTRA, IMMEDIATE and MISSING rows, so re-read it before trusting the
 * register-permutation framing.
 *
 * The target keeps the first parameter in $a0 for all four of its loads and
 * puts the outer walker in $v1:
 *
 *     p = $a0   e = $v1   outer giv = $t2   n = $t1   n+6 giv = $t0
 *
 * This source copies the parameter out (`move $t2, $a0` in the prologue) and
 * gives the walker $a0:
 *
 *     p = $t2   e = $a0   outer giv = $t1   n = $t0   n+6 giv = $v1
 *
 * Everything else is a consequence: with $a0 taken by `e`, $v1 is free and the
 * highest-priority allocno (the n+6 giv) takes it instead of $t0, and the rest
 * shift down.
 *
 * From the .greg dump the allocation order is `122 95 96 124 84 80 81 82 83 94`
 * -- the n+6 giv (26 refs / 22 insns), n (23/28), the outer giv (9/36), then
 * `e` (84, 9 refs / 40 insns) and only then `p` (80, 6 refs / 88 insns). global.c
 * ranks by roughly log2(refs) * refs / live_length, so `e` at ~0.68 beats `p` at
 * ~0.14 and is allocated first; it also carries `;; 84 preferences: 4`, a
 * preference for $a0, which `p` does not have. For the target's assignment `e`
 * would have to be allocated after the n+6 giv (a gap of 0.68 against 4.7, far
 * too wide to close by tweaking ref counts) or else conflict with $v1.
 *
 * REASSESSED 2026-10-22: this is the shared-preference case, the same class as
 * func_800407C0, and no source shape measured reaches it. `p` and `e` BOTH
 * prefer $a0 -- `p` because it is the incoming parameter, `e` because of its
 * own recorded preference -- and their live ranges overlap, because the loop
 * condition reads p[2] every iteration. So they conflict, both want $a0, and
 * whichever is allocated first simply takes it. `e` wins at 0.675 against
 * 0.136, a five-fold gap, and the numbers behind it are fixed by the target's
 * own code: `e`'s nine references are its initialisation, the condition, the
 * increment and its two loads, all inside the loop and so weighted by
 * loop_depth, while `p`'s are two reads outside it and one inside. Making the
 * loop a `goto` loop drops both weights but not their ratio (0.25 against
 * 0.09). Closing a five-fold gap would need `e` down to about four references
 * or `p` up to eight, and neither is available.
 *
 * So treat the remaining 30-register difference as a property of the
 * allocator, not of the source, unless someone finds a way to remove `e`'s
 * $a0 preference specifically. See docs/matching-levers.md section 10.
 *
 * Ruled out, all giving the same 30-register-difference output: `u32 *p` with
 * `(u32) e < p[2]`; a struct pointer with `u8 *` fields and a pointer compare;
 * `while` instead of `for`; the walker initialised in two statements (worse --
 * 58 diffs, it loses the combined giv).
 *
 * Worth keeping even so: the inner `u8 f;` snapshot of the flags before the
 * `n += 0xC` is what produces the target's `lbu` / `addiu` / `beqz` tail, and
 * writing the halfword updates as `*(u16 *) (n + 4)` and `*(u16 *) (n + 6)` off
 * a walking byte pointer is what makes gcc's combine_givs fold them into one
 * induction variable at +6 with the other access as `-0x2` off it.
 *
 * UNIT: pass `unk_19434` as verify.py's UNIT argument. This function started in
 * unk_14934; successive $gp clusters have been carved out of that chain, and
 * the piece holding it is now unk_19434. (An earlier version of this line said
 * unk_1811C, which was true for one carve and is now stale -- the chain is
 * unk_14934, gprel_152E4, unk_154BC, gprel_17504, unk_176B8, gprel_17B7C,
 * unk_1811C, gprel_18B40, unk_19434, each named for its start offset. Derive it
 * from the splat spec by address rather than from a comment:
 * `tools/decomp/nmcheck.py --fix-units` does exactly that.)
 *
 * BOUNDED, confirmed 2026-10-23 with a sharper diagnosis than the above: the
 * two facts the target needs have ONE common cause, and no shape measured
 * splits it. Note what that bound does and does not cover -- it is a statement
 * about conflicts and allocation order WITHIN this translation unit as carved,
 * with p[0] local-allocated to $a0. If the unit boundary or a declared size is
 * wrong, the conflict set changes and the bound is void. See the context audit
 * in docs/where-we-are.md before sweeping source shapes here again.
 *
 * `p` must keep $a0 and `e` must avoid it. Both follow from the block-local
 * `p[0]` load being local-allocated to $a0: that makes `p`, which is live
 * across it, conflict with hard reg 4 -- `.greg` shows
 * `80 conflicts: ... 2 3 4 5 6 7 29` and consequently NO `preferences:` line
 * for 80 at all, because `prune_preferences` strips the parameter's own $a0
 * preference as a conflict -- and it propagates `;; 84 preferences: 4` onto
 * `e` along the dataflow chain through `(set (reg 84) (plus (reg 88) (reg 87)))`.
 *
 * And nothing measured splits it, because **`p`'s live range strictly contains
 * `e`'s**
 * (88 insns against 40; the loop condition reads `p[2]` every iteration). Any
 * hard register that conflicts with `e` therefore conflicts with `p` too, so
 * there is no conflict-based assignment in which `e` avoids $a0 and `p` keeps
 * it. That specifically closes off the conflict-import lever that closed
 * func_8003CCB0 (matching-levers section 3), which is the newest thing anyone
 * would reach for here. The only route left is allocation order, and the gaps
 * are the ones recorded above: the n+6 giv at 4.7, `n` at 3.3, `e` at 0.675,
 * `p` at 0.136 -- `p` would need 16 references to overtake `e` against the 6 it
 * has, or `e` would need to drop to 3 against the 9 it has, all inside the loop
 * and so weighted by loop_depth.
 *
 * The entry block's SCHEDULE differs too and is coupled to the same decision:
 * the target leaves the `p[2]` load after `e`'s `addu` and pays a nop in its
 * load-delay slot, where this build hoists the load and spends the word on
 * `move $t2,$a0` instead -- which is how both come to 58 words. Every spelling
 * tried left sched1 in source order.
 *
 * TEN MORE VARIANTS MEASURED, all exactly 58 words / 30 DIFF: a non-struct
 * spelling of the bound, of `p[1]`, and of all three accesses; a struct pointer
 * with three u32 fields and `p->unk_08` (all three in-struct); `break` instead
 * of a `for` condition; `p[1] << 2` for `* 4`; `p[1] * 4 + p[0]` reversed;
 * `(u8 *) p[0] + p[1] * 4`; and `&((u8 *) p[0])[p[1] * 4]`.
 *
 * FOUR MIXED MEM_IN_STRUCT_P SPELLINGS, added 2026-10-23, also 58 / 30: the
 * previous ten covered all-in-struct and all-non-struct but no combination, and
 * section 5's lever is specifically about a dependence between references of
 * DIFFERING kinds, so the mixtures were the live gap. `*(p+0) + *(p+1)*4` with
 * the bound left as `p[2]`; the same with only `p[0]` non-struct; with only
 * `p[1]` non-struct; and all three non-struct via `*(p+N)`. None of them moves
 * sched1's hoist of the `p[2]` load.
 *
 * WHICH MATTERS, because the hoist -- not the priority arithmetic -- is the
 * tighter statement of what is wrong. The entry block's schedule and the
 * register assignment are ONE decision: hoisting the `p[2]` load above `e`'s
 * `addu` makes a third value live at once, and that is what puts the `p[0]`
 * load on $a0, which is what makes `p` conflict with $a0 and propagates the
 * $a0 preference onto `e`. Everything in the paragraphs above follows from it.
 * So the question to attack is not "how do I reprice `e` against `p`" (a
 * five-fold gap, and the ten spellings above all failed at it) but **"what
 * stops sched1 moving that one load"** -- and the answer is not operand order,
 * not loop form, not struct-ness in any of the 14 combinations tried.
 *
 * CONTEXT AUDIT RUN, 2026-10-23, and it CLOSES rather than opens -- recorded so
 * nobody spends a session there on my suggestion, which is what the previous
 * version of this paragraph invited. Four of the five questions in
 * docs/where-we-are.md's audit are not inferences for this function at all:
 *
 *  - Unit boundary, declared symbol sizes, -G value, function order within the
 *    unit: ALL IRRELEVANT HERE. The target listing contains zero `lui`, `%hi`,
 *    `%lo` and `%gp_rel` -- this function references no global whatsoever, and
 *    addressing modes plus the .sdata table are the only things a unit carve
 *    controls. So unk_19434's boundary cannot be what is wrong, and that is
 *    direct evidence from the bytes rather than an argument.
 *  - Struct layout: not inferred either. The displacements are IN the target --
 *    0x0/0x4/0x8 off $a0, and -0x2/0x0/0xC off the inner walkers.
 *
 * What is genuinely unverified is only the signature: there are no callers of
 * func_80047054 anywhere in the resident executable (it is reached from an
 * overlay), so nothing cross-checks the parameter count or types. The target's
 * own use of $a1/$a2/$a3 as full words added into u16 fields is the only
 * evidence, and it is consistent with what this source declares.
 *
 * Worth knowing while working it: the function has NO STACK FRAME -- no
 * `addiu $sp`, nothing saved -- so the allocator has only caller-saved
 * registers to work with. Any lever that wants a callee-saved register is
 * unavailable here by construction, which is a second reason the section 3
 * escape from local-alloc does not apply (the first being the contained live
 * range above).
 *
 * So this bound is tighter than the others, not looser: it is genuinely
 * function-local, the surrounding context is evidence rather than assumption,
 * and the one open question was sched1's hoist of the `p[2]` load.
 *
 * BOTH HALVES NOW HAVE A SOURCED MECHANISM, read out of gcc 2.7.2 on
 * 2026-10-23. Neither opens a lever here, and both say why.
 *
 *  THE HOIST is not a tie-break. `rank_for_schedule` (sched.c) compares
 *  INSN_PRIORITY, then classifies each insn against the LAST SCHEDULED insn --
 *  3 for independent or latency-1, 2 for anti/output dependent, 1 for data
 *  dependent -- and prefers the higher class, falling back to INSN_LUID only
 *  to keep the sort stable. The list is built in reverse, so a dependency-free
 *  insn floats to the FRONT of its block, outranking a dependent neighbour
 *  wherever the source put it. The `p[2]` load depends on nothing in that
 *  block, so it is class 3 and goes first. The lever would be to make it
 *  data-dependent on its neighbour -- and nothing in the entry block exists
 *  for `p[2]` to depend on, which is why all 14 spellings failed.
 *
 *  THE $a0 ASSIGNMENT is local-alloc phase 2, and phase 1 cannot be reached
 *  from here. `block_alloc` first gives every quantity carrying a SUGGESTION
 *  (qty_phys_copy_sugg, else qty_phys_sugg) its suggested register, jumping
 *  the queue; only the unsuggested remainder is sorted by decreasing life and
 *  handed the first register find_free_reg finds. Suggestions come from
 *  `combine_regs` on a copy relating a pseudo to a HARD register -- i.e. from
 *  a call's arguments or return value. **This function contains zero `jal`.**
 *  No calls means no hard-register copies beyond the incoming parameters, so
 *  no suggestion can be created for the block-local `p[0]` load, and it must
 *  go through phase 2 and take the first free register, which is $a0. That is
 *  also consistent with the function having no stack frame.
 *
 * Note for anyone carrying the lever elsewhere: func_8004437C (6 calls) and
 * func_8003F368 (12 calls) DO have the suggestion lever available, and it is
 * unexplored on both. It is unavailable here specifically, not generally.
 *
 * SINCE PARKED, 2026-10-27: 30 DIFF to 24, by making the `+= arg2` at +0x06
 * a `volatile` read-modify-write. A volatile load is never forwarded and the
 * store is never deleted, which is what keeps the listing's separate load
 * and store of that field; found by search.py's new `t_volatile` family, on
 * the first park where advise.py's MISSING row named a load. The rest of the
 * header below predates that and is written against 30 DIFF.
 */

#include "common.h"

void func_80047054(u32 *p, s32 arg1, s32 arg2, s32 arg3) {
    u8 *e;

    for (e = (u8 *) (p[0] + p[1] * 4); (u32) e < p[2]; e += 8) {
        if (*(s16 *) (e + 2) == 2) {
            u8 *n = *(u8 **) (e + 4);

            if (!(*n & 8)) {
                u8 f;

                *n |= 8;
                do {
                    if (*n & 0x40) {
                        *(u16 *) (n + 4) += arg1;
                    } else {
                        *(u16 *) (n + 4) += arg3;
                    }
                    if (!(*n & 0x20)) {
                        *(volatile u16 *) (n + 6) += arg2;
                    }
                    f = *n;
                    n += 0xC;
                } while (!(f & 0x80));
            }
        }
    }
}
