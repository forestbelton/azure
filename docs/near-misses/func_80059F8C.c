/* func_80059F8C (unk_2CDC0) -- 145 of 145 words, 103 DIFF. NOT compiled.
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
 * THE WORD CAME FROM THE TWO FREE-SLOT SCANS AS BACKWARD GOTOS, 2026-10-27:
 * 146 of 145 words and 121 DIFF to 145 of 145 and 103, the exact count. Each
 * scan is now
 *
 *     if (i < 0x10) {
 *     scan1:
 *         if (D_800869C0[i * 2 + 1] == 0) { store; break; }
 *         i++;
 *         if (i < 0x10) { goto scan1; }
 *     }
 *     break;
 *
 * which is not a loop to loop.c, so it gets no biv, no giv and no hoisting --
 * matching-levers section 2, the same lever that answered func_8003C67C's row
 * and took func_8004E9EC from 34 words to 32. Found by `search.py
 * --only=goto_loop`, and then WITHDRAWN FROM THAT FAMILY: a `break` inside a
 * rewritten loop belongs to the loop being rewritten, and the goto form hands
 * it to the enclosing one. Here that is harmless, because each scan's `break`
 * is immediately followed by the outer loop's own `break` and both spellings
 * land in the same place -- but it is luck rather than a rule, so the family
 * now skips any body containing `break` or `continue` and this one has to be
 * written by hand.
 *
 * Note what this did NOT need: the walked-pointer spelling the listing's
 * `addiu ..., 0x8` suggests. Measured before the goto: both scans walked with
 * the outer loop walked too, 148 words and 125 DIFF; both as a `for (;;)`
 * with the outer loop indexed, 156 and 127. The `addiu` is strength reduction
 * on an INDEXED reference, as everywhere else in this unit.
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
                if (i < 0x10) {
                scan1:
                    if (D_800869C0[i * 2 + 1] == 0) {
                        D_800869C0[i * 2] = p;
                        D_800869C0[i * 2 + 1] = n;
                        break;
                    }
                    i++;
                    if (i < 0x10) {
                        goto scan1;
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
            if (i < 0x10) {
            scan2:
                if (D_800869C0[i * 2 + 1] == 0) {
                    D_800869C0[i * 2] = p;
                    D_800869C0[i * 2 + 1] = n;
                    break;
                }
                i++;
                if (i < 0x10) {
                    goto scan2;
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
