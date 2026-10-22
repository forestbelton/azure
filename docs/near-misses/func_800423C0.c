/* func_800423C0 (unk_14934) -- 4 instructions off out of 72. Everything up to
 * and including the func_800A1BD0 call is exact, as are all four accesses
 * through the indexed base and the tail. What is left is how the three
 * 0x8001xxxx constants get into registers:
 *
 *   target                         this source
 *   lui $a1, 0x8001                lui $a1, 0x8001          (matches)
 *   ori $a1, $a1, 0x2094           ori $a1, $a1, 0x2094     (matches)
 *   lui $v1, 0x8001                sll $v1, $a0, 1
 *   sll $v0, $a0, 1                lui $v0, 0x8001
 *   bnez $a0                       bnez $a0                 (matches)
 *   addu $v1, $v0, $v1             addu $v1, $v1, $v0
 *   lui $a1, 0x8001                move $a1, $v0
 *   ori $a1, $a1, 0x2096           ori $a1, $a1, 0x2096     (matches)
 *
 * So: the two `addu` operands are swapped, and where the target materialises
 * a third independent `lui 0x8001` for 0x80012096, gcc here reuses the base
 * pointer's register (`move $a1, $v0`). gcc splits a large constant into
 * lui + ori as two RTL insns, so cse shares the `lui 0x8001` across the whole
 * extended basic block -- and the 0x80012096 arm is the fall-through of the
 * `bnez`, hence inside it. The target has all three unshared, which means
 * either cse could not reach the third one or the constants were not RTL
 * lui/ori pairs there.
 *
 * Settled, and not to be re-derived:
 *
 * - The indexed base is the plain constant 0x80010000 with 0x2094/0x2098 as
 *   load displacements, NOT the symbol D_80012094. splat renders a bare
 *   `lui reg, 0x8001` as `%hi(D_80012094)` whenever it can pair the register
 *   with a later `%lo`, which hides the difference. Measured: a symbol array
 *   with a variable index (`extern u16 A[]; A[i]`) always materialises the
 *   whole address with `lui %hi` + `addiu %lo` and then indexes off that, with
 *   a plain `0(reg)` on the load -- for every spelling tried, including a
 *   complete array `u16 B[0x100]`, two accesses at different indices, and a
 *   `union { u16 h; u8 b; }` element type. Only the constant base pointer
 *   gives the target's form, so declaring D_80012094 is a dead end; that
 *   declaration was added and then reverted.
 * - The branch sense is `if (t == *cmp)`, not `!=`: the target's
 *   `bne $a0, $v0, .L800424A0` jumps *past* the store.
 * - The counter has to be read into `t` and reused for `t + 1`; the target
 *   has `addiu $v0, $a0, 0x1` off the already-loaded value.
 * - arg1 is genuinely unused -- $a1 is overwritten with the -8 mask before
 *   any use -- and that mask constant is what ends up in $a1 at the
 *   func_800A1BD0 call, which takes one argument.
 */

#include "common.h"

void func_80042710(u8 *, Unk800E3E48 *);

void func_800423C0(u8 *arg0, s32 arg1, u8 *arg2) {
    if (arg2 != NULL) {
        Unk800E3E48 *e = &D_800E3E48[arg2[3] & 0x1F];
        s32 r;

        func_80042710(arg0, e);
        *(u32 *) (arg0 + 0x14) &= ~7;
        *(u32 *) (arg0 + 0x1C) &= ~7;
        *(u32 *) (arg0 + 0x14) |= e->unk_14 & 7;
        *(u32 *) (arg0 + 0x1C) |= e->unk_1C & 7;
        r = (s16) func_800A1BD0(arg0);
        if (r >= 0) {
            u8 *p = (u8 *) 0x80010000 + r * 2;
            u16 *cmp = (u16 *) 0x80012094;
            u16 t;

            if (r == 0) {
                cmp = (u16 *) 0x80012096;
            }
            t = *(u16 *) (p + 0x2094);
            if (t == *cmp) {
                *(u16 *) (p + 0x2094) = (t + 1) & 7;
            }
            arg0[0x45] = p[0x2094];
            arg0[0x12] = p[0x2098];
        }
    }
    *(u32 *) (arg0 + 0x1C) |= 0x20000;
}
