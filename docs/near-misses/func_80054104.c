/* func_80054104 (unk_1C540) -- 61 of 57 words, 60 DIFF. NOT compiled.
 * Four words OVER, which is the unusual direction.
 *
 * Copies the 8 bytes at D_80032E0C into a local `s16 t[4]` and then, for each
 * of 0x30 outer steps and the first 3 entries of t, builds
 * `((t[j] << 8) & 0xFF00) | ((i & 0xFF) >> 4)` -- plus 0x10 when t[j] is zero
 * -- and calls func_80055730 with it and `((i & 0xF) << 10) + 0x1C00`, but
 * only when func_8005405C(t[j]) is non-zero.
 *
 * The arithmetic and both loop bounds are right. `(i & 0xFF) >> 4` has to be
 * written `((u32) i & 0xFF) >> 4` or it comes out `sra` where the target has
 * `srl` -- advise.py reports that as SIGNEDNESS.
 *
 * THE FOUR WORDS ARE TWO CALLEE-SAVED REGISTERS this source spends and the
 * target does not: loop.c strength-reduces the access to the LOCAL array into
 * a walking cursor and hoists `$sp + 0x10` into a register of its own, where
 * the target recomputes `sll $v0,$s1,1` / `addu $v0,$sp,$v0` / `lh 0x10($v0)`
 * on every inner iteration. The inner loop has two calls, so `loop_has_call`
 * is true and the reduction threshold is halved, which is presumably why it
 * reduces here; why it did not in the original is the open question.
 *
 * MEASURED AND NOT WORKING, all still 61 words: reading `t[j]` once into a
 * local so the address has a single use; `u8 t[8]` with `*(s16 *)(t + j * 2)`;
 * `*(s16 *) ((u8 *) t + j * 2)`; `s32 t[2]` with `((s16 *) t)[j]`; and an
 * explicit `s32 off = j * 2;` with the address formed either way round. The
 * reduction is not reachable from how the subscript is spelled.
 *
 * It also needs func_80055730 and func_8005BAB0 widened to the two arguments
 * their call sites prove they take -- func_80055730 is a thin wrapper that
 * passes them straight on, so its landed one-argument form compiles to the
 * same bytes, and func_8005BAB0's listing reads $a0 and $a1 at entry.
 */

#include "common.h"
extern u8 D_80032E0C[];

void func_80055730(s32, s32);

void func_80054104(void) {
    s16 t[4];
    s32 i;
    s32 j;

    memcpy(t, D_80032E0C, 8);
    for (i = 0; i < 0x30; i++) {
        s32 hi = ((u32) i & 0xFF) >> 4;
        s32 base = ((i & 0xF) << 10) + 0x1C00;

        for (j = 0; j < 3; j++) {
            s16 v = t[j];
            s32 k = ((v << 8) & 0xFF00) | hi;

            if (v == 0) {
                k += 0x10;
            }
            if (func_8005405C(v)) {
                func_80055730(k, base);
            }
        }
    }
}
