/* func_8004437C (gprel_17B7C) -- 100 of 100 words, 8 DIFF. NOT compiled.
 *
 * Switches the active buffer pair: if the requested entry is not already
 * bound, flip the bank index, point the slot at the right scratch buffer, hand
 * it to func_8003E4FC, then switch the display pair and tell the sound driver.
 *
 * Every opcode, immediate, displacement and delay slot matches. The 8 are two
 * spots, both the register holding %hi(D_80080AF4):
 *   prologue: this emits `lui $v1 / lh $v0 / addiu $a1,$v1`, the target has
 *             `lh $v0 / lui $a1 / addiu $a1,$a1`                   (3 words)
 *   after func_8003E4FC: this has the address in $v1 and the stored pointer in
 *             $a0; the target has them swapped                     (5 words)
 *
 * THE LOCAL-ALLOC SUGGESTION LEVER DOES NOT REACH THIS FUNCTION, checked
 * 2026-10-23. `block_alloc` does have a preference system this header did not
 * know about -- every quantity carrying a SUGGESTION gets its suggested
 * register in a first phase that jumps the queue, ahead of the unsuggested
 * quantities sorted by life length -- and I recorded this function as a
 * candidate because it has 6 calls. That was the wrong test.
 *
 * Suggestions come from `combine_regs` on a copy relating a pseudo to a HARD
 * register, so what matters is whether THE QUANTITY YOU NEED TO MOVE is on a
 * hard-register copy path, not whether the function makes calls. The quantity
 * here is the `%hi` half of `D_80080AF4[D_80080AFC]`'s address, and that value
 * is never an argument and never a return value: the only hard-register copies
 * in this function are its two parameters and the arguments of its calls
 * (`6`, the `D_8006E704` pointer, `0`, `arg1`, and the `0x22`/`0x21`
 * constant). Nothing can attach a suggestion to the address without changing
 * what the function does.
 *
 * So no new idea, and the analysis below stands unchanged.
 *
 * MECHANISM. Indexing a symbol (`D_80080AF4[i]`) goes through
 * legitimize_address, which puts the HIGH in one pseudo and force_regs the
 * LO_SUM into another. Here the LO_SUM pseudo is a global allocno ($a1) while
 * the HIGH is block-local ($v1); the target uses one register for both. So
 * the question is how to make cc1psx materialise the address once, into one
 * pseudo, without hoisting it out of the block.
 *
 * Measured and ruled out, so do not retry: a pointer local
 * (`u32 **af4 = D_80080AF4;`) does give the one-pseudo form but hoists the
 * materialisation to the top of the function (14 DIFF); the same assignment
 * written inside the comparison (cse folds it back, unchanged at 8); a
 * `(u32)`-cast pointer arithmetic spelling (101 words, 79 DIFF);
 * `*(u32 **) &D_80080AF0[4 + i * 4]` (101 words, 79 DIFF); complete array
 * sizes [4] and [16] (unchanged); hoisting the stored value into a local
 * (unchanged); hoisting the index into a local (unchanged); swapping the first
 * comparison's operands (18 DIFF); and a full `search.py --g8` sweep, 11
 * candidates across 8 transform families, which found nothing.
 *
 * LOAD-BEARING, each measured by reverting it:
 *  - D_80080AFC reached as a SCALAR, not `D_80080AFC[0]`: 1 word and 79 DIFF
 *    otherwise. As an array element the `sh` is an in-struct reference and
 *    true_dependence pins it ahead of the two `lw`s, leaving a load-delay nop;
 *    as a scalar the exception applies and sched1 drops the `sh` into the slot.
 *    See matching-levers section 5.
 *  - `extern s16 D_80080AFC;` with a `(u16)` cast at the xor site ONLY. Declared
 *    u16 with `(s16)` casts at the index sites costs 10 DIFF, because a (s16)
 *    cast whose value only feeds a zero test is folded away and the load stays
 *    `lhu`. Section 7 now records that limit.
 *  - the mask inside each ternary arm, `? (A & 0x7FFFFF) : (B & 0x7FFFFF)`:
 *    jump2 cross-jumps the identical and/or/sw tails into the join, reorg
 *    lifts the common `lui 0x7f` into the branch delay slot, and the `ori`
 *    stays duplicated per arm -- which is the listing.
 *  - `|=` with the ternary on the right. expand_assignment evaluates the LHS
 *    first, so the pointer re-read and `lw $a0,0($a1)` sit above the branch
 *    while the arithmetic is below the join.
 *  - `D_8006E704[arg0]` re-dereferenced at all four uses, never cached.
 *  - `func_80053DA8(cond ? 0x22 : 0x21)` as a ternary argument; reorg steals
 *    the 0x21 into the beqz's delay slot.
 *  - s16 parameters, s32 return. include/structs.h still declares
 *    `void func_8004437C(s32, s32);` and wants `s32 func_8004437C(s16, s16);`
 *    when this lands -- its one caller, csrc/unk_102A8.c's
 *    `func_8004437C(0x2A, 0)`, passes constants so no codegen change is
 *    expected there, but re-verify that function after the change.
 *
 * SECOND PASS (2026-10-22). Still 100 of 100 words and 8 DIFF, and the
 * diagnosis is now at pseudo level rather than at the level of shapes tried.
 * **Treat this as a wall and do not spend another session on it without a new
 * idea from outside the function.**
 *
 * WHY THE TWO HALVES CANNOT BE MADE TO COINCIDE. Region 1's address is
 * `(set (reg 95) (high (symbol_ref "D_80080AF4")))` then
 * `(set (reg 94) (lo_sum (reg 95) (symbol_ref "D_80080AF4")))`. Pseudo 94 is
 * used by the `addu` in block 0 AND the `addu` in block 2, so it is a global
 * allocno and global-alloc gives it $a1 -- which is correct. Pseudo 95 is
 * block-local, so LOCAL-alloc gets it first and hands it $v1, the first
 * register free after the `lh` result in $v0. Region 2's pair has the same
 * shape but there BOTH pseudos are block-local, local-alloc owns both and
 * reuses the dying source's register -- which is why region 2 already prints
 * `lui $v1 / addiu $v1,$v1` with the right structure and only a $v1/$a0 swap.
 * So the target's `lui $a1 / addiu $a1,$a1` is ONE pseudo, i.e.
 * emit_move_insn(reg, symbol): the symbol materialised as a VALUE, not inside
 * an address computation. The general rule to carry away: **an indexed symbol
 * whose base is reused across blocks splits its address between local-alloc
 * and global-alloc, and no source spelling measured makes the two halves
 * coincide.** (That is a result about spellings, not a proof: it assumes this
 * unit's carve and D_80080AF4's declared size, both inferred.)
 *
 * THE 3-WORD SPOT IS ALSO A SCHED1 TIE. Pre-sched order is
 * [lhu][lh][high][lo_sum], which is the target's. The `sched` dump gives
 * `insn (lh): priority = 1` and `insn (high): priority = 1` -- a tie -- and
 * sched1 hoists the `high` above the `lh` on the LUID tie-break in
 * rank_for_schedule. Both chains to the block end are the same length
 * (`lh -> sll -> addu -> lw -> bne` against `high -> lo_sum -> addu -> lw ->
 * bne`), so no source change breaks the tie without adding an insn.
 *
 * TWELVE MORE SHAPES MEASURED, baseline 8 DIFF, 100/100 words unless noted:
 * a two-set pointer local (region 1 + region 2), first set inside the
 * comparison, 15 DIFF -- and the word count and the emitted `lui $v0 /
 * addiu $a1,$v0` show cse folded the local into the addresses anyway, so the
 * two-set trick does NOT survive here; the same with the first set as a plain
 * statement, 15 DIFF; a region-2 slot local
 * (`u32 **slot = &D_80080AF4[D_80080AFC];`), 10 DIFF; two-set
 * `cur = D_8006E704[arg0];` before each comparison, 29 DIFF; the second
 * comparison's operands swapped, 10 DIFF; both comparisons swapped, 20 DIFF;
 * the first `if` inverted with `return 1` out of line, 99 words and 79 DIFF.
 * Four `MEM_IN_STRUCT_P` spellings (`*(u32 **)((u8 *) D_80080AF4 + i * 4)` on
 * the region-2 store LHS, on its RHS, on both, and on the first comparison
 * alone) are **bit-identical**, not merely the same count: inert.
 *
 * TWO CORRECTIONS TO THE NOTES ABOVE, both from re-measuring:
 *  - the in-struct requirement belongs to BLOCK 2's two `lw`s, not block 0's.
 *    Block 0 has no `sh` to hoist past -- the `sh` is in block 2 -- so
 *    spelling only the first comparison non-struct is free and buys nothing,
 *    while spelling BOTH comparisons non-struct costs a word (101 words,
 *    79 DIFF).
 *  - the operand swap and the inverted `if` are evidence, not failures. The
 *    swap moves the D_80080AF4 materialisation to the TOP of block 0, which
 *    proves the operand evaluation order is source-controlled and that the
 *    baseline's order -- materialised mid-block, after the D_8006E704 address
 *    chain -- is the target's. The inverted `if` losing a word confirms the
 *    polarity and block layout are right too. Between them, every structural
 *    degree of freedom in that block is pinned to what this source has.
 *
 * THE ONE UNTRIED ESCAPE IS NOT CONSTRUCTIBLE, checked 2026-10-23, so this
 * function has no open idea left AT THE SOURCE LEVEL -- which is the point at
 * which the context questions in docs/where-we-are.md become the cheaper thing
 * to doubt. The escape was a cse run boundary
 * inside region 1 -- identical `if`/`else` arms between a single-set
 * `u32 **af4 = D_80080AF4;` and its uses, which section 1 makes free in words
 * and which is exactly what lets func_8003CCB0's two-set pointer locals survive
 * cse. But section 1's lever duplicates a statement into the arms of an
 * **existing** `if`, and region 1 is the function's FIRST comparison: there is
 * no antecedent condition before it to duplicate into, and introducing one
 * costs the test. (The stated worry -- that the construct is also a
 * sched1/sched2 block split in a block whose 17 words are exact -- would have
 * applied on top of that.)
 *
 * Also checked against the newer section 6 finding that a ONE-set
 * pointer-to-symbol local survives unfolded when its uses are
 * negative-displacement: these uses are `D_80080AF4[D_80080AFC]` with a
 * non-constant index of 0 or 1, and biasing the base to `&D_80080AF4[1]` to get
 * a negative displacement adds a subtraction, so it is not free either.
 *
 * The unit is built at -G8 and D_80080AF4 must stay an INCOMPLETE array; see
 * include/structs.h and the unit header for why.
 */

#define func_8004437C func_8004437C_hidden
#include "common.h"
#undef func_8004437C



void func_80044698(void);
s32  func_8004450C(s16);

s32 func_8004437C(s16 arg0, s16 arg1) {
    if (D_8006E704[arg0] == D_80080AF4[D_80080AFC]) {
        return 1;
    }

    D_80080AFC = (u16) D_80080AFC ^ 1;

    if (D_8006E704[arg0] != D_80080AF4[D_80080AFC]) {
        func_800542BC();
        if ((s8) D_80080AF3 != arg1) {
            func_80044698();
        }
        *D_8006E704[arg0] &= 0xFF800000;
        *D_8006E704[arg0] |= (D_80080AFC != 0) ? ((u32) D_801C4640 & 0x7FFFFF)
                                                     : ((u32) D_801BEE40 & 0x7FFFFF);
        func_8003E4FC(6, (u8 *) D_8006E704[arg0], 0);
        D_80080AF4[D_80080AFC] = D_8006E704[arg0];
        func_8003F320();
    }

    func_8004450C(arg1);
    func_80053DA8(D_80080AFC != 0 ? 0x22 : 0x21);
    return 0;
}
