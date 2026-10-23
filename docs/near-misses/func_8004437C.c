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
