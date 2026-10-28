/* func_8003A904 (unk_72A8) -- 46 of 45 words, 44 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep row on 2026-10-27 so there is a source to
 * sweep, with one new observation that narrows the gap a lot (see the end).
 *
 * The func_8003A2E0 family again: func_8003A1A4's state-table lookup
 * (`m = state[B].unk_48 * 4`) with func_8003A858's four accumulating stores
 * as the tail. Every instruction of the tail matches position for position,
 * and the whole diff is ONE WORD AT THE TOP -- a `nop` this source has and
 * the target does not, which shifts everything after it.
 *
 *     target   lw $v0, 0x1C($a0) ; lw $t2, 0x80($a0) ; lbu $t0, 0x0($v0)
 *     built    lw $v1, 0x1C($a0) ; nop               ; lbu $t0, 0x0($v1)
 *
 * i.e. sched1 fills the cursor load's delay slot with the STATE load, where
 * this source leaves it empty and the state load lands seventh.
 *
 * MEASURED, all 46 words and 43-44 DIFF: the state read first, second, and
 * through an explicit cursor local with `*cp = c + 1` instead of `(*cp)++`;
 * the state read as an ARRAY_REF (`((u8 **) p)[0x20]`, which sets
 * MEM_IN_STRUCT_P and so changes what `true_dependence` says about it against
 * the cursor store); and the row's own list.
 *
 * THE NEW OBSERVATION, and it is the thing to work from: ITS LANDED SIBLING
 * func_8003A1A4 HAS THE SAME NOP. Decoded out of the ROM at 0xD9A4, that
 * function begins `lw $v1, 0x1C($a0) ; nop ; lbu $t1, 0x0($v1)` -- so the
 * shape this source uses is RIGHT, and matched there. The two functions differ
 * only in the tail: func_8003A1A4 computes the little-endian word into a `w`
 * local and does ONE store, while this one has four accumulating stores. So
 * the state load's hoist is not something the top of the function asks for --
 * it is the extra insns in the tail changing sched1's ready list, and the
 * thing to read is the sched1 dump of the whole block in both functions
 * (tools/decomp/sched.py), not another spelling of the two loads.
 *
 * 2026-10-28: A SWEEP HIT THAT IS WRONG, recorded so it is not adopted if it
 * turns up again. `tools/decomp/unname.py`'s first version reported that
 * inlining `m` -- deleting
 *
 *     m = *(u32 *) (s + **cp * 4 + 0x48) * 4;
 *
 * and writing that expression at its one use inside `q` -- is 44 words and
 * 42 DIFF against this file's 46 and 44, i.e. better on BOTH axes (one word
 * under instead of one over). It is a semantic change and not a candidate:
 * `(*cp)++` runs between that line and the use, so the inlined form reads the
 * byte through the INCREMENTED pointer. The right number off the wrong program.
 *
 * The tool now rejects it -- it checks that every operand of a value it moves
 * is unwritten between the assignment and the last use -- so this is a note
 * about the function rather than a standing caution. What it does say about
 * the function is that 44 words is reachable here and the current shape is one
 * over, which is worth remembering when the real 45-word shape is looked for:
 * the one-over is not forced by the arithmetic.
 */

#include "common.h"

void func_8003A904(u8 *p) {
    u8 **cp = (u8 **) (p + 0x1C);
    u32 a;
    u32 m;
    u8 *s;
    u8 *r;
    u8 *q;
    u8 *t;

    a = *(*cp)++;
    s = *(u8 **) (p + 0x80);
    m = *(u32 *) (s + **cp * 4 + 0x48) * 4;
    (*cp)++;
    r = *cp;
    q = (u8 *) (r[0] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24) + m);
    *cp += 4;
    t = (u8 *) (a * 4 + (u32) s);
    *(u32 *) (t + 0x48) = q[0];
    *(u32 *) (t + 0x48) += q[1] << 8;
    *(u32 *) (t + 0x48) += q[2] << 16;
    *(u32 *) (t + 0x48) += q[3] << 24;
}
