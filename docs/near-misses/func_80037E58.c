/* func_80037E58 (unk_72A8) -- 70 of 70 words, 8 DIFF. NOT compiled.
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
 * WHAT IS LEFT: 8 words, and all 8 are one register pair. The block after the
 * descending loop materialises two addresses:
 *
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
extern u8 D_800809C0[];
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
    D_80082A38w[0x22] = (u32) D_800809C0;
    D_80082A38w[0xE] = 0;
    q[1][3] = D_80012D6E[0];
    func_80037D9C(q[1], 3);
    q[0][0x87] = 0;
    q[0][0x85] = 0;
    q[0][0x86] = q[0][0x85];
}
