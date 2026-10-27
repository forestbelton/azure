/* func_80037E58 (unk_72A8) -- 70 of 70 words, 2 DIFF. NOT compiled.
 *
 * Spawns the node with func_8003FE78, hangs func_80037F70 on it, points its
 * +0x20/+0x24 pair at D_80082A38 and D_80082A38+0x8C, zeroes 0x6B eight-byte
 * D_80082660 entries, clears sixteen words of D_80082A38 from the top down,
 * plants D_800809C0 and a zero, and copies a byte out of D_80012D6E.
 *
 * WHAT IS PAID FOR -- everything structural, and three of the shapes took
 * finding:
 *
 *   1. The DESCENDING loop keeps 0x48 as the store's DISPLACEMENT, with the
 *      giv based at D_80082A38 + 0x3C. Reaching the words by name --
 *      `D_80082A38w[i + 0x12] = 0`, or any of the five byte-cast regroupings
 *      measured -- lets loop.c fold the 0x48 into the giv's add_val, because
 *      symbol plus constant IS a constant; the store then comes out as
 *      `sw $zero, 0x0($v0)` off a base of +0x84. Going through a loop-
 *      invariant REGISTER instead (`r = D_80082A38w;` then `r[i + 0x12]`)
 *      leaves the add_val as a pseudo, the 0x48 cannot fold into it, and the
 *      displacement survives. Worth one word and the whole loop.
 *   2. `q = (u8 **) (n + 0x20)` as its own local, with the +0x24 store and
 *      every later read through it but the +0x20 store spelled `n + 0x20`.
 *      That asymmetry is what the listing has ($s2 for 0x4, $v0 for 0x20).
 *   3. The three tail reads of q[0] each reload it, because the preceding
 *      `sb` invalidates cse's memory table -- but the last pair,
 *      `q[0][0x86] = q[0][0x85]`, shares ONE load, which is only true when
 *      both subscripts are in the same statement.
 *
 * SIX OF THE EIGHT WENT ON 2026-10-27, and the lever was the DECLARATION:
 * `extern u8 D_800809C0;` with `(u32) &D_800809C0`, not `extern u8
 * D_800809C0[];`. The park's own arithmetic below had asked for exactly this
 * -- "its live length down to 3, which would need D_800809C0's address to be
 * one insn rather than two" -- and the scalar spelling is what gives it: a
 * scalar sets SYMBOL_REF_FLAG, so cc1psx emits a one-insn `la` that gas
 * expands to `lui`/`addiu` in the SAME register, where the incomplete array
 * is a two-insn compiler-split pair from the start. Pseudo 106 then costs
 * local-alloc one insn of live length instead of two, 103 is allocated
 * first, and the pair comes out the listing's way round. Found by
 * `search.py --only=global_decl`, the family added that day; the park had
 * measured `&D_800809C0[0]` but not the declaration itself.
 *
 * WHAT IS LEFT: TWO WORDS, and they are one sched2 choice inside priority 1.
 * The listing has `lw $v1, 0x4($s2)` then `lui $v0, %hi(D_80012D6E)`; this
 * has them the other way round. From the sched2 dump (block 4): insns 133
 * (the `lw`) and 134 (the `lui`) are both priority 1, both ready at T-18, and
 * `potential_hazard` takes the MEMORY insn first in the backward pass --
 * which puts it LATER in the stream. That is matching-levers' "sched2's
 * tie-break inside a priority group is memory over ALU", and the same class
 * as func_80035E58's last two words: the whole top-of-block region is
 * priority 1, so the dependence graph never gets a say, and no statement
 * order reaches it. For the listing's order the `lui` would have to be
 * selected at T-18, which needs it to OUTRANK the load on priority -- i.e. a
 * longer path from the block start, which a bare `high` of a symbol cannot
 * have.
 *
 * (The eight-word analysis that follows is kept because it is what named the
 * fix. The register pair it describes is now right.)
 *
 * WHAT WAS LEFT AT 8 WORDS: all 8 were one register pair. The block after
 * the descending loop materialises two addresses:
 *   lui/addiu $v0 = D_80082A38      (the listing)   vs   $v1   (this source)
 *   lui/addiu $v1 = D_800809C0                           $v0
 *   sw $v1, 0x88($v0) ; sw $zero, 0x38($v0)
 *
 * and because D_800809C0's register dies at the first store here, sched2 is
 * free to pull the next group's `lui %hi(D_80012D6E)` up between the two
 * stores, which is the 8th word. Fix the pair and the schedule follows.
 *
 * The .lreg dump prices it exactly. The pseudos are
 *
 *   103 = lo_sum(D_80082A38): 3 refs over 4 insns -> 1 * 3 * 4 / 4 = 3.000
 *   106 = lo_sum(D_800809C0): 2 refs over 2 insns -> 1 * 2 * 4 / 2 = 4.000
 *
 * so local-alloc takes 106 first and gives it $v0, and 103 -- which conflicts
 * with it -- falls to $v1. The listing needs 103 allocated first, which wants
 * either a FOURTH reference to the D_80082A38 address in this block (there is
 * no third store to spend) or its live length down to 3, which would need
 * D_800809C0's address to be one insn rather than two.
 *
 * Measured and not working, all 70 words and all 8 DIFF: the two stores in
 * either source order (sched2 reorders them regardless, so the source order
 * does not reach the allocator at all), `&D_800809C0[0]`, a duplicate
 * `D_80082A38w[0xE] = 0` to buy a reference (flow counts refs after the dead
 * store is deleted), a `u32 *` pointer local assigned after the loop and used
 * for both stores, and `q` as `u32 *` with the casts moved to the uses.
 * Also measured and worse: `*(u8 **) &D_80082A38w[0x22] = D_800809C0` and
 * `*(u32 *) &D_80082A38[0x1C] = 0` both come out at 69 words, the second
 * materialisation collapsing into the first; reaching the words through `r`
 * after the loop is 68 words, because the register then survives the loop.
 */

#include "common.h"

extern UnkNode *D_80082BC0[];
extern u8 D_800809C0;
extern u8 D_80012D6E[];

UnkNode *func_8003FE78(s32, UnkNode *, s16);
void func_80037F70(u8 **);
void func_80037D9C(u8 *, u8);

void func_80037E58(void)
{
    u8 *n;
    u8 **q;
    s32 i;
    u32 *r;

    n = (u8 *) func_8003FE78(0, D_80082BC0, 0x49);
    q = (u8 **) (n + 0x20);
    *(u32 *) (n + 0x10) = (u32) func_80037F70;
    q[1] = (u8 *) D_80082A38w;
    *(u32 *) (n + 0x20) = (u32) ((u8 *) D_80082A38w + 0x8C);
    for (i = 0; i < 0x6B; i++)
    {
        memset(&D_80082660[i], 0, 8);
    }
    r = D_80082A38w;
    for (i = 0xF; i >= 0; i--)
    {
        r[i + 0x12] = 0;
    }
    D_80082A38w[0x22] = (u32) &D_800809C0;
    D_80082A38w[0xE] = 0;
    q[1][3] = D_80012D6E[0];
    func_80037D9C(q[1], 3);
    q[0][0x87] = 0;
    q[0][0x85] = 0;
    q[0][0x86] = q[0][0x85];
}
