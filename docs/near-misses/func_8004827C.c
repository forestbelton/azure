/* func_8004827C (unk_1AF38) -- 18 of 19 words, 12 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep rows on 2026-10-27 so there is a source to
 * sweep. Calls func_80047BC0 with the s16 second argument and stores
 * `D_80083D08 + result * 8` into the +0x28 field of the record at `p + 0xC`.
 *
 * WHAT IS LEFT is ONE WORD, and it is the shared `addu $rX, $rY, $zero` of
 * the `func_8004827C + func_8004D880 + func_800537D0 + func_80055CF4 +
 * func_8003BD84 + func_80058A04` row -- a copy local-alloc coalesces away and
 * the target keeps. Here it is the PARAMETER, and the target spends TWO
 * registers on it:
 *
 *     addu $s0, $a0, $zero      <- the entry save, because $a0 is the call's
 *     ...                          own argument register
 *     jal  func_80047BC0
 *     addu $v0, $s0, $zero      <- a SECOND copy, just to read through
 *     lw   $a1, 0xC($v0)
 *
 * where this source reads `0xC($s0)` directly off the saved copy. Measured:
 * the read through an explicit `b = p;` local after the call coalesces the
 * same way (identical, 18 words).
 *
 * The row lists the five known CURES for this copy -- re-dereference instead
 * of caching, an explicit guard plus do-while on a loop bound, a statement
 * between the producer and the copy, unifying two values so a loop's
 * conflict is imported, and making the register unavailable -- and notes that
 * none of them applies to a PARAMETER, because a parameter has no second live
 * range to give it. That is still true here: `p` is read exactly once, so
 * there is nothing to split.
 *
 * The one thing NOT yet tried on this family is today's return-type lever
 * (matching-levers section 1): declaring a `void` function non-void makes
 * $v0 live out at the epilogue and so unavailable, which is the "make the
 * register unavailable" cure in a form a parameter CAN have. It cannot help
 * this function -- the copy it wants is INTO $v0, not away from it -- but
 * func_80055CF4 and func_8003BD84 should be checked against it before
 * anything else. *
 * 2026-10-27: THE FIRST DIVERGENCE IS THE SHIFT'S DESTINATION, not the copy,
 * and that reorders the whole investigation. Side by side from word 9:
 *
 *     target  sll $a0,$v0,3 ; addu $v0,$s0,$zero ; lui $v1 ; addiu $v1 ;
 *             lw $a1,0xC($v0) ; addu $a0,$a0,$v1 ; sw $a0,0x28($a1)
 *     built   sll $v0,$v0,3 ; lui $v1 ; addiu $v1 ; lw $a0,0xC($s0) ;
 *             addu $v0,$v0,$v1 ; sw $v0,0x28($a0)
 *
 * The listing shifts `k` OUT of $v0 into $a0 and carries the address sum in
 * $a0; this build shifts in place and carries it in $v0. Only once $v0 is
 * free does the listing's copy of `p` into it make any sense -- so the copy is
 * DOWNSTREAM of the shift's register, and attacking the copy directly (which
 * is what this row's note says to do) is attacking the second fact.
 *
 * Both builds have the shift and the sum as ONE quantity, so the question is
 * just which register that quantity gets: the call's result arrives in $v0,
 * the shift ties to it here, and the listing does not. Per section 3's "two
 * ways a copy disappears", the tie is the ALLOCATION kind, so what is needed
 * is for the shift's destination to CONFLICT with $v0 -- i.e. for the call's
 * result to still be live after the shift -- and `k` has exactly one use, so
 * nothing in this source does that.
 *
 * Measured and NOT working, 2026-10-28, also all 18 of 19 and 12 DIFF: the
 * address as an ARRAY REFERENCE off the typed alias, `&D_80083D08s[k]` (the
 * byte-array form `&D_80083D08[k]` is 17 words, since the element size is 1
 * and the `* 8` disappears); and the multiply SPLIT into its own statement,
 * `k = k * 8;` with the sum then `D_80083D08 + k`, in both the pointer and the
 * integer spelling. The split is the free-reference lever and it does nothing
 * here, which is the expected result rather than a surprise: `k` and the call
 * result are ONE quantity either way, so the extra reference lands on the
 * quantity that already owns $v0 instead of giving the call result a use that
 * outlives the shift.
 *
 * Measured and NOT working, all 18 of 19 words and 12 DIFF: a `u8 *pp = p;`
 * local for the dereference (it coalesces, as the row predicts); the address
 * computed into a local before the dereference; the dereference written inline
 * inside the store; and `k` holding the already-multiplied value
 * (`func_80047BC0(i) * 8`). Section 3's seventh cure -- extend the source
 * value past the copy, which landed func_800537D0 -- does not apply, because
 * the value whose live range would have to grow is the CALL RESULT and its
 * only use is the shift.
 *
 * 2026-10-28: THE "DIE MORE THAN ONCE" ROUTE IS CLOSED, which is worth having
 * because it is the one `combine_regs` refusal a single-basic-block function
 * can reach. `combine_regs` (local-alloc.c:1762) refuses the tie when the
 * SOURCE pseudo has `reg_qty < 0` -- "not local to this block OR DIES MORE
 * THAN ONCE" -- and this function has no branch, so multi-block is not
 * available but dying twice is: give a variable two live ranges and
 * local-alloc hands it to global-alloc instead, where no tie forms. Five
 * shapes measured, ALL 18 of 19 words and 12 DIFF, i.e. bit-identical to the
 * base:
 *
 *     `p` reused to hold the address sum
 *     one pointer local reused for `p`'s copy and then for `q`
 *     `k` reused as the copy's destination (`k = (s32) p;`)
 *     `k` reused to hold the address sum
 *     `q` reused for the copy, the load and then `q + 0x28`
 *
 * and six spellings of the dereference and the sum are the same 18/12 as well:
 * a `pp = p` local after the call, a `u32` base, the sum into a local before
 * or after the read, and `q` as a `u32`. So the copy is not being coalesced by
 * `combine_regs` at all -- a source-level `pp = p` never reaches local-alloc,
 * because cse substitutes `p` for `pp` and flow deletes the dead set. THE COPY
 * IN THE LISTING IS THEREFORE NOT A SOURCE-LEVEL COPY, and the next thing to
 * establish is which pass creates it. Note what it is NOT: it has no
 * displacement (`addu $v0, $s0, $zero`, then `lw $a1, 0xC($v0)`), so it is not
 * an address computation; `$s0` has no later use, so it is not a liveness
 * conflict; and gcc 2.7's caller-save spills to stack slots rather than
 * registers, so it is not that either.
 */

#include "common.h"

s32 func_80047BC0(s16);

void func_8004827C(u8 *p, s16 i) {
    s32 k = func_80047BC0(i);
    u8 *q = *(u8 **) (p + 0xC);

    *(u32 *) (q + 0x28) = (u32) ((u8 *) D_80083D08 + k * 8);
}
