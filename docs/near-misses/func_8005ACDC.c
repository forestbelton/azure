/* func_8005ACDC (unk_2CDC0) -- 75 of 75 words, 3 DIFF. NOT compiled.
 *
 * FIRST ATTEMPT, 2026-10-27: this function had never been read (it was one of
 * the nine stubs with neither a park file nor a carved-sweep row of its own --
 * see docs/where-we-are.md). Four edits took it from 35 DIFF to 3.
 *
 * The release sweep: unless D_80073828 says otherwise, walks every live
 * D_80085458 entry, and for each whose +0x1A countdown is nonzero and whose
 * D_80073740 handle func_8005EB78 reports in state 3, either decrements the
 * countdown or -- once it has reached 2 -- collects the handle into a mask,
 * spins func_8005E97C/func_8005EB78 until the voice reports 2 or 0, and
 * clears the countdown. The collected mask is keyed off in one final call.
 *
 * WHAT IS PAID FOR
 *
 * * D_80073828 is read SIGNED (`lh`), through an `s16` __asm__ alias: the
 *   header's `extern u16 D_80073828[];` gives `lhu`. One word.
 *
 * * The GUARD TESTS `m`, not `i`. The listing's `slt $v0, $s3, $v0` names the
 *   mask's register, which holds 0 at that point, and `i = 0` is the
 *   `addu $s4, $s3, $zero` in the `beqz`'s own delay slot -- so `i = 0` is
 *   INSIDE the guarded block and the bound test compares `m`. With `i < bound`
 *   as the guard and `i = 0` above it, the copy is emitted before the branch
 *   and five prologue words shuffle with it.
 *
 * * The `v >= 2` body FALLS THROUGH and the decrement is the `else`. gcc
 *   computes `v < 2` and branches on it being true, so writing
 *   `if (v < 2) { decrement } else { big }` puts the big body first and the
 *   two arms and their shared tail come out in the wrong order: 15 words.
 *
 * * A SECOND ROW POINTER at offset zero for the trailing `+0x1A` store,
 *   declared after the inner do/while -- matching-levers section 2's giv
 *   lever, the one that landed func_80057D20. Without it the D_80073740 giv
 *   is initialised before the D_80085458 one and they take each other's
 *   registers ($s1/$s2 swapped), because the inner loop's hoisted copy of
 *   `h[0]` is the last giv created and so represents its group.
 *
 * WHAT IS LEFT: THREE WORDS, and they are the ORDER OF THREE ALU INSNS in the
 * loop's preheader. The listing has `addu $s4, $s3, $zero` (i = 0), then
 * `addiu $s5, $zero, 0x2` (the inner loop's hoisted constant), then
 * `lui %hi(D_80085458)`; this source emits the `lui` first and the other two
 * after. All three are priority 1 and all three are ALU, so `potential_hazard`
 * does not separate them and the choice is LUID -- and the `lui` here is
 * hoisted by move_movables (the symbol is referenced twice in the body, once
 * for `e` and once for `e2`) rather than being part of the giv's own
 * initialisation, which is what gives it the earlier LUID.
 *
 * So the thing to try next is a spelling that keeps the second row reference
 * WITHOUT giving the symbol a second materialisation to hoist: `e2 = e;`
 * loses the giv ordering and goes back to 18 DIFF, and a walked `e` biases
 * the giv to +0x1A (`lhu 0x0($s1)` where the listing has `lhu 0x1A($s1)`).
 * Measured on the way: the two arms in both orders (3 and 15), the guard on
 * `i` (18), `lhu` for D_80073828 (one more word differing), and no second row
 * pointer at all (30). A FULL `search.py` sweep over all twelve transform
 * families found nothing better than the base, so the remaining three words
 * are not a local source rewrite.
 */
#include "common.h"

extern s32 D_80073734a[] __asm__("D_80073734");
extern s16 D_80073828s[] __asm__("D_80073828");

void func_8005E97C(s32, s32);
s32 func_8005EB78(u32);

void func_8005ACDC(void) {
    s32 m;
    s32 i;
    s32 k;
    u32 v;

    m = 0;
    if (D_80073828s[0] == 0 && m < D_80073734a[0]) {
        i = 0;
        do {
            u8 *e = D_80085458 + i * 0x78;

            if (*(u16 *) (e + 0x1A) != 0) {
                if (func_8005EB78(D_80073740[i]) == 3) {
                    v = *(u16 *) (e + 0x1A);
                    if (v >= 2) {
                        u32 *h = &D_80073740[i];

                        m |= D_80073740[i];
                        do {
                            func_8005E97C(0, h[0]);
                            k = func_8005EB78(h[0]);
                        } while (k != 2 && k != 0);
                        {
                            u8 *e2 = D_80085458 + i * 0x78;

                            *(u16 *) (e2 + 0x1A) = 0;
                        }
                    } else {
                        *(u16 *) (e + 0x1A) = v + 1;
                    }
                }
            }
            i++;
        } while (i < D_80073734a[0]);
    }
    if (m != 0) {
        func_8005E97C(0, m);
    }
}
