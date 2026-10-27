/* func_800563B0 (unk_1C540) -- 22 of 22 words, 14 DIFF. NOT compiled.
 *
 * Written out of the `func_80056DB4 + func_800563B0 + func_80056D44` row on
 * 2026-10-27. Same recipe as func_80056DB4 (read that file's header for the
 * addressing) with four halfword stores instead: flags 0xF at +4, the two
 * s16 arguments at +8 and +0xA, zeros at +0xC and +0xE, the D_80073740
 * handle at +0.
 *
 * The listing puts the +0 store in the `jal`'s DELAY SLOT
 * (`jal func_8005F134 ; sw $v0, %lo(D_80084918)($a3)`), so it is the last of
 * the six and reorg moves it.
 *
 * WHAT IS LEFT: fourteen words. The row expected twelve and did not say which
 * arrangement gave them, so that is worth re-measuring before anything else;
 * this file is the plain shape with the stores in the listing's own order and
 * has had no permutation run of its own. Start with
 * `tools/decomp/perm.py` over the six stores -- 720 orderings, so pass
 * `--max 200` -- and then compare against func_80056DB4's 3 DIFF, since the
 * two differ only in the stores they make.
 */

#include "common.h"

void func_8005F134(u8 *);

void func_800563B0(s32 i, s16 a, s16 b) {
    u8 *q = D_80084918;
    u32 h = D_80073740[i];

    *(u32 *) (q + 4) = 0xF;
    *(s16 *) (q + 8) = a;
    *(s16 *) (q + 0xA) = b;
    *(s16 *) (q + 0xC) = 0;
    *(s16 *) (q + 0xE) = 0;
    *(u32 *) D_80084918 = h;
    func_8005F134(q);
}
