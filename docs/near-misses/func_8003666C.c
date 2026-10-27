/* func_8003666C (unk_72A8) -- 34 of 34 words, 7 DIFF. NOT compiled.
 *
 * func_800364EC's twin with the half-width offset. This file is the sweep
 * row's source, written out 2026-10-27 so that perm.py and search.py have
 * something to chew on -- the row had the measurements but no candidate.
 *
 * WHAT IS PAID FOR
 *
 * * `s32 n = ((s[0x26] + 1) >> 1) - 1;` AS ITS OWN LOCAL. Inline in the
 *   subtraction, fold moves the `- 1` onto the other operand (13 DIFF), and
 *   with only the `>> 1` in the local and `(n - 1)` written in the
 *   subtraction it is 14.
 * * `(w - n) * 2` for the `sll 17` / `sra 16` pair: the product is an `s16`
 *   local, so the widening happens in the caller, which is what the stack
 *   argument at 0x10($sp) wants.
 * * The second `*(u8 **) (p + 0x74)` in the call's last argument is a REAL
 *   second read -- the listing has two `lw 0x74($s0)` -- which is the twin's
 *   shape too.
 *
 * WHAT IS LEFT: SEVEN WORDS, AND THEY ARE ONE DECISION. The entry region
 * emits the same seven insns in a different order, and the one that matters
 * is WHICH OF THE THREE LOADS OFF `s` COMES LAST, because the last one gets
 * to overwrite the base register:
 *
 *   target  sw $ra ; lw 0x74 ; addu $a2,$s0 ; lbu 0x27($v0) ; lh 0x22($v0) ;
 *           lbu 0x26($v0) ; addiu $a3,1        <- base is $v0, dies into 0x26
 *   built   addu $a2,$s0 ; sw $ra ; lw 0x74 ; addiu $a3,1 ; lbu 0x27($v1) ;
 *           lbu 0x26($v1) ; lh 0x22($v1)       <- base is $v1, dies into 0x22
 *
 * It is sched1's choice among three equal loads, and SOURCE ORDER DOES NOT
 * REACH IT. Measured 2026-10-27, all at 34 words and all with the identical
 * seven differing words -- i.e. the load order did not budge:
 *
 *   * the halfword read into its own `s32` local before `n`          7 DIFF
 *   * the same with an `s16` local                                   7
 *   * one `s16 t` reused: read the halfword into it, then
 *     `t = (t - n) * 2;` as a separate statement                     7
 *   * `s32 b = s[0x26];` with the arithmetic in `t`'s statement      8
 *   * all 24 permutations of the four statements, with perm.py  best 7
 *   * a local for the shift alone, and the argument passed inline
 *     instead of through `t`                                      14-17
 *
 * So the next thing to try is NOT another spelling of these statements. The
 * three loads are all priority 1 with the same function unit, so neither
 * PRIORITY nor `potential_hazard` separates them and the tie-break is LUID --
 * yet changing the LUID order (the four measurements above do exactly that)
 * changes nothing, which means the decision is being made before the ready
 * list, in the dependence graph. Read the sched1 dump (tools/decomp/sched.py,
 * block 0) and find which edge the target lacks: the base pointer's death is
 * the only asymmetry between the three.
 */

#include "common.h"

void func_8003719C(u8 *, u8 *, u8 *, s16, s32, u32);
void func_800366F4(u8 *, s32, s32);

void func_8003666C(u8 *p) {
    u8 *s = *(u8 **) (p + 0x74);
    s32 n = ((s[0x26] + 1) >> 1) - 1;
    s16 t = (*(s16 *) (s + 0x22) - n) * 2;

    p[0x4D] = s[0x27];
    func_8003719C(D_80082538, D_80082040, p, 1, t,
                  *(u32 *) (*(u8 **) (p + 0x74) + 0x80));
    *(u32 *) (p + 0x68) = (u32) func_800366F4;
}
