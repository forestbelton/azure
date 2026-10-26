/* func_80055D84 (unk_1C540) -- 58 of 58 words, 5 DIFF. NOT compiled.
 *
 * Tries func_8005A778 on slot k, then func_8005AAA8; on the first failure it
 * reports 0x01010000, on the second 0x01020000 (both or-ed with
 * `(k << 8) & 0xFF00`) through func_8003F52C, and on success sets bit
 * `0x10000 << k` in D_800847D0[0].
 *
 * THE LOAD-BEARING SHAPE IS WHERE THE CONSTANT IS ASSIGNED, and it is worth
 * two words. The target computes the `(k << 8) & 0xFF00` mask ONCE, in a block
 * both failure paths jump to, with each path contributing only its own `lui`.
 * That is jump2 cross-jumping two duplicated `func_8003F52C(...)` calls, and
 * how far back the merge reaches depends on INSTRUCTION ORDER inside each arm:
 * do_cross_jump walks backwards from the two jumps while the insns are
 * equivalent and stops at the first difference, which is the `lui` of the
 * differing constant. So
 *
 *     f = 0x1010000;                 (lui)
 *     m = (k << 8) & 0xFF00;         (sll, andi)
 *     func_8003F52C(f | m);
 *
 * merges the sll and the andi, while writing the call as one expression --
 * `func_8003F52C(((k << 8) & 0xFF00) | 0x1010000)` -- puts the lui LAST in
 * each arm, the backwards walk stops immediately, and the mask is duplicated
 * into both arms for +2 words. Writing the constant first in the expression
 * does not help: fold reorders it. Two statements, constant first, is the only
 * form found that works.
 *
 * WHAT IS LEFT is a two-register swap around the final `or`: the target has
 * the mask in $a0 and the constant in $v0, so its `or $a0,$a0,$v0` reuses the
 * mask's own register as the destination, where this source has the mask in
 * $v0, the constant in $v1 and `or $a0,$v0,$v1`. From --greg, the mask is
 * pseudo 84 (4 refs / 4 insns, priority 2.0) and the constant pseudo 83 (4 /
 * 8, priority 1.0), so the mask is allocated FIRST and takes the lowest free
 * register -- the target's mask instead carries a preference for the argument
 * register it is or-ed into. `m |= f; func_8003F52C(m);` and
 * `func_8003F52C(m | f)` both compile to exactly the same thing as `f | m`,
 * so the preference is not reachable by making the mask the or's destination
 * in the source.
 */

#include "common.h"
s32 func_8005A778(u8 *, s16, u32);
s32 func_8003F52C(s32);

void func_80055D84(s16 k) {
    s16 r;
    s32 f;
    s32 m;

    r = func_8005A778((u8 *) D_80084538[k], k, D_800847C0[k]);
    if (r == -1) {
        f = 0x1010000;
        m = (k << 8) & 0xFF00;
        func_8003F52C(f | m);
    } else {
        r = func_8005AAA8((u8 *) D_80084758[k], k);
        if (r == -1) {
            func_80055C50(k);
            f = 0x1020000;
            m = (k << 8) & 0xFF00;
            func_8003F52C(f | m);
        } else {
            func_8005AC30(1);
            D_800847D0[0] |= 0x10000 << k;
        }
    }
}
