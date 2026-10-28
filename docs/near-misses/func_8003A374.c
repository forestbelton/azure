/* func_8003A374 (unk_72A8) -- 29 of 29 words, 12 DIFF. NOT compiled.
 *
 * Written out of the `func_8003A3E8 + func_8003A374` row on 2026-10-27.
 * The twin func_8003A3E8 is the same opcode WITH a table lookup
 * (`m = *(u32 *) (s + **cp * 4 + 0x48) * 4` in place of `m = **cp * 4`), and
 * the two sit at the same 12 DIFF.
 *
 * The func_8003A2E0 family: reads a byte, a table index and a little-endian
 * 32-bit operand from the bytecode cursor, and stores a word THROUGH A BYTE
 * it loads from the composed address -- `*(u32 *) (u32) *(u8 *) d = ...`,
 * which is what the `lbu`/`sw` pair at the end is (the game's own bug or a
 * deliberate low-address write; either way that is the C).
 *
 * The statement order is pinned and is the family's: the first byte with its
 * increment, the state pointer at +0x80, the table lookup through `**cp`
 * WITHOUT incrementing, the second increment, the four operand bytes off
 * `r = *cp`, then `*cp += 4`. The lookup after the operand bytes is 26 DIFF.
 *
 * WHAT IS LEFT: TWELVE WORDS, which is three better than the row this came
 * from and the same for both twins -- the target accumulates the operand in
 * $v0, starting from the first byte, where this source uses $a1, and the
 * `sll` of the table value interleaves differently. Measured: the operand
 * composition as a step-by-step accumulation (`d = r[0]; d += r[1] << 8;
 * ...`), which is func_8003A1A4's shape for ITS second composition, is 15 --
 * so the one-expression form below is right and the registers are an
 * allocation question, not a spelling one.
 */

/* 2026-10-28: THE TWELVE WORDS ARE ONE $v1/$a1 SWAP, AND THE QUANTITIES ARE
 * NOW IDENTIFIED. From the `.lreg` dump and `.greg`'s dispositions, the two
 * rivals are
 *
 *   the CURSOR   pseudo 94 = `88 + 1` (the second increment): 5 refs over 6
 *                insns -- its own set, the `sw` to +0x1C, and the bases of
 *                r[1], r[2], r[3]. Priority floor_log2(5)*5*4/6 = 6.67. It
 *                takes $v1 here; the listing gives it $a1.
 *   the ACCUM.   a FIVE-pseudo chain 95 -> 98 -> 101 -> 104 -> 85 (r[0]'s
 *                byte, then each `addu`, then `d`), combined into ONE quantity
 *                because each `addu`'s destination ties to its dying source,
 *                so it carries ~10 refs -- but over a span that reaches from
 *                r[0]'s load to the final `lbu`, which is long enough to put
 *                it UNDER the cursor. It takes $a1 here; the listing gives it
 *                $v1.
 *
 * So the accumulator has to outrank the cursor. MEASURED 2026-10-28, all 29
 * words, none of it reaching:
 *
 *   * splitting the accumulation to buy free references -- `+ m` as its own
 *     statement (15 DIFF), `+ (r[3] << 24)` and `+ m` split off (15), and
 *     `d = r[0];` then one `+=` with the rest (13). The references do arrive,
 *     but the split also moves `m`'s `sll` later and costs three words
 *     elsewhere; +2 refs is not enough to flip 6.67 over a span this long,
 *     and the arithmetic says it needs about +4.
 *   * reading r[0] BEFORE the increment, so the accumulator is born first and
 *     would win a tie -- `d = (*cp)[1];` above `(*cp)++` is 16 DIFF: it also
 *     reorders the table byte's load.
 *   * computing the cursor into `r` explicitly to lengthen its range --
 *     `r = *cp + 1; *cp = r;` in place of `(*cp)++; r = *cp;`, with the
 *     lookup before or after it, is 12 DIFF either way. gcc folds the two
 *     forms together, so there is no range to lengthen there.
 *
 * WHY THE SPLITS FAIL, which is the useful part. The chain is one quantity
 * only because each `addu`'s destination ties to a source that DIES in it; the
 * tie is what sums the references. A split (`d += x;`) makes `d` multi-set and
 * BREAKS that tie: in the three-split variant `d` does reach 8 refs over 11
 * insns, exactly as intended, but `.greg`'s dispositions then show the chain
 * scattered across three registers instead of one, each piece pricing on its
 * own 2 refs and losing by more than the whole did. So the usual free-
 * reference lever is unavailable here: it cannot add a reference to this
 * quantity without dissolving it.
 *
 * `advise.py --greg` prints the chain as separate 2-ref pseudos at 4.000 and
 * 2.667 against the cursor's 6.667, because it reads the per-pseudo .lreg
 * lines and does not model `combine_regs`' merging. The merged quantity is
 * what competes, and the margin is thin: at 10 refs it needs about +1
 * reference or -2 live to pass 6.667. Both have to arrive WITH the tie intact.
 *
 * The twin func_8003A3E8 sits at the same 12 and should take whatever lands
 * here.
 */

#include "common.h"

void func_8003A374(u8 *p) {
    u8 **cp = (u8 **) (p + 0x1C);
    u32 a;
    u32 m;
    u8 *r;
    u32 d;
    u8 *s;
    u8 *t;

    a = *(*cp)++;
    m = **cp * 4;
    (*cp)++;
    r = *cp;
    d = r[0] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24) + m;
    *cp += 4;
    s = *(u8 **) (p + 0x80);
    t = (u8 *) (a * 4 + (u32) s);
    *(u32 *) (u32) *(u8 *) d = *(u32 *) (t + 0x48);
}
