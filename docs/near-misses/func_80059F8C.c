/* func_80059F8C (unk_2CDC0) -- 146 of 145 words, 121 DIFF. NOT compiled.
 *
 * FIRST ATTEMPT, 2026-10-27, and this is a DECODING park rather than a
 * near-miss: the control flow below is read off the listing and compiles to
 * within a word, but the loop's shape is not yet right. It was one of the ten
 * stubs with neither a park file nor a carved-sweep row (see
 * docs/where-we-are.md); the other nine are done or further along.
 *
 * WHAT IT DOES. The counterpart to func_80059E94 (landed, just above it in
 * csrc/unk_2CDC0.c): where that one CLAIMS a D_800869C0 slot, this one puts a
 * run {p, n} back, keeping the table ordered. D_800869C0 is sixteen
 * {offset, length} pairs; `D_80073830[D_8007382B[0]]` is the reserve at the
 * top of the 0x80000 window, exactly as in the sibling.
 *
 *   * Two range guards: `(u32) (p - 0x1010) > 0x7EFEF` -- i.e. p outside
 *     [0x1010, 0x7FFFF] -- and `p + n > 0x7FFFF`. Both answer -1.
 *   * Table empty (slot 0's offset is zero): store {p, n} in slot 0 if
 *     `p + n` clears the reserve, call func_80059DAC and answer p.
 *   * Otherwise walk the sixteen slots. Slot 0 is its own case (`if (i == 0)`
 *     in the draft, the listing's `bnez $a2` at the loop top). The arms
 *     answer -1 on any overlap, replace a slot whose offset already IS p
 *     (and answer p WITHOUT calling func_80059DAC -- the listing's
 *     `addu $v0, $s0, $zero` before those two stores), or find the first slot
 *     with a zero length and store there.
 *   * The tail is `if (i != 0x10) { func_80059DAC(); return p; } return -1;`
 *     -- the listing's `addiu $v0, $zero, 0x10` / `bne $a2, $v0`.
 *
 * WHAT IS ALREADY RIGHT. Every instruction of the two guards, the whole
 * empty-table arm and the reserve computation match position for position,
 * and so does the loop's preheader except for the copies below. `end = p + n`
 * in a local is worth one word (147 -> 146) and one differing word.
 *
 * WHAT IS WRONG, and it is one shape. THE TWO FREE-SLOT SCANS ARE WALKED
 * POINTERS, not indexed:
 *
 *     L080:  v1 = a0 ; v0 = v1[1] ; beqz v0 -> store ; a2++ ;
 *            if (a2 < 0x10) { a0 = v1 + 8 ; goto L080 }
 *     L16C:  v0 = v1[1] ; beqz v0 -> store ; a2++ ;
 *            if (a2 < 0x10) { v1 += 8 ; goto L16C }
 *
 * -- a cursor advanced by 8 with the counter incremented beside it, and the
 * test at the TOP (the first iteration is entered unguarded, because i < 0x10
 * is already known). The draft below writes both as
 * `D_800869C0[i * 2 + 1]`, which is this unit's recipe for the OUTER loops
 * and the wrong one here: it leaves eight extra copies in the preheader
 * ($t4/$t3/$t6/$t5/$t7/$t0/$t2/$t1 against the listing's four) because each
 * indexed scan gets its own giv. Note the asymmetry -- the first scan copies
 * the cursor into `v1` and stores through the ORIGINAL, the second stores
 * through the cursor itself -- which is the same "one local per
 * materialisation" shape as elsewhere in this unit.
 *
 * The other thing to carry over from the sibling: `D_80073830[...]`'s ADDRESS
 * is hoisted into the preheader but its LOAD stays in the loop (the listing's
 * `sll`/`addu` at L034 and `lw $v1, 0x0($v1)` inside), because the stores
 * through the table may alias it. The draft's inline spelling gets that right
 * already.
 */
#include "common.h"

void func_80059DAC(void);

s32 func_80059F8C(u32 p, s32 n)
{
    s32 i;
    u32 b;
    u32 end;

    if ((u32) (p - 0x1010) > 0x7EFEF) {
        return -1;
    }
    end = p + n;
    if (end > 0x7FFFF) {
        return -1;
    }
    if (D_800869C0[0] == 0) {
        if (end < 0x80000 - D_80073830[D_8007382B[0]]) {
            D_800869C0[0] = p;
            D_800869C0[1] = n;
            func_80059DAC();
            return p;
        }
        return -1;
    }
    for (i = 0; i < 0x10; i++) {
        if (i == 0) {
            if (p < D_800869C0[0]) {
                if (D_800869C0[0] < end) {
                    return -1;
                }
                for (; i < 0x10; i++) {
                    if (D_800869C0[i * 2 + 1] == 0) {
                        D_800869C0[i * 2] = p;
                        D_800869C0[i * 2 + 1] = n;
                        break;
                    }
                }
                break;
            }
        } else {
            if (p < D_800869C0[i * 2]) {
                continue;
            }
        }
        b = D_800869C0[i * 2];
        if (b == p) {
            D_800869C0[i * 2] = p;
            D_800869C0[i * 2 + 1] = n;
            return p;
        }
        if (D_800869C0[(i + 1) * 2 + 1] == 0) {
            if (end < 0x80000 - D_80073830[D_8007382B[0]]) {
                D_800869C0[(i + 1) * 2] = p;
                D_800869C0[(i + 1) * 2 + 1] = n;
                func_80059DAC();
                return p;
            }
            return -1;
        }
        if (b < p && p < D_800869C0[(i + 1) * 2]) {
            if (p < b + D_800869C0[i * 2 + 1]) {
                return -1;
            }
            if ((s32) D_800869C0[(i + 1) * 2] < (s32) end) {
                return -1;
            }
            for (; i < 0x10; i++) {
                if (D_800869C0[i * 2 + 1] == 0) {
                    D_800869C0[i * 2] = p;
                    D_800869C0[i * 2 + 1] = n;
                    break;
                }
            }
            break;
        }
    }
    if (i != 0x10) {
        func_80059DAC();
        return p;
    }
    return -1;
}
