/* func_800398BC (unk_72A8) -- 75 of 76 words, one word short. NOT compiled.
 *
 * FIRST FUNCTION WORKED FROM AN m2c DRAFT, and worth recording as that: the
 * draft had the control flow, the two little-endian 4-byte reads off the
 * script cursor, the 0xFF07FFFF mask and the double table lookup right, and
 * listed exactly the three symbols that needed declaring. It reached 75 of 76
 * words on the FIRST compile. Two fixes after that, both already documented
 * levers, and every branch span matches.
 *
 * The two fixes: the inner `t != 0` test has to be written so the shared
 * "clear +0x10, back the cursor up one" block is the FALL-THROUGH -- as nested
 * ifs with that block duplicated, which jump2 cross-joins -- rather than as a
 * ternary condition, which inverts the branch; and D_800721C0 is a `u32`
 * array indexed by `(u >> 0x13) & 0x1F`, not a byte array indexed by that
 * times four.
 *
 * THE ONE MISSING WORD is the scaling of that index. The target spends three
 * insns -- `srl`, `andi 0x1F`, `sll 2` -- where gcc here folds the mask and
 * the scale together into `srl 0x11` + `andi 0x7C` and saves one. Note it is
 * NOT the D_8006CD50 lookup: that one does emit its `sll 2` correctly
 * (verified in the built asm, `lw` / `sll 2` / `addu` / `lw`).
 *
 * For gcc to decline that fold the masked value normally needs a second use,
 * or to be used in another basic block. MEASURED AND NOT WORKING, all still 75
 * words: an explicit `s32 i` index variable; splitting it as `i = u >> 0x13;
 * i &= 0x1F;` so the pseudo is multi-set; reaching the table as
 * `*(u32 *) ((u8 *) D_800721C0 + i * 4)` off a byte-array declaration; a
 * COMPLETE `extern u32 D_800721C0[32]` declaration; the index typed `u8` and
 * `u32`; `(u >> 0x13) % 32` instead of the mask; and `(u << 9) >> 27`. Eight
 * spellings, all 75 words -- the fold is not reachable from how the index is
 * written, so the second use (or whatever else denies combine the
 * substitution) has to come from somewhere else in the original.
 */

#include "common.h"
extern u32 D_800721C0[];

s32 func_80039884(u8 *);
s32 func_80053EF0(s32);

void func_800398BC(u8 *p) {
    s32 t = func_80053EF0(4);

    if (func_80039884(p) != 0) {
        if (t != 0) {
            *(u32 *) (p + 0x10) = 0;
            *(u8 **) (p + 0x1C) = *(u8 **) (p + 0x1C) - 1;
            return;
        }
    } else if (t == 0x100 || t == 3) {
        *(u32 *) (p + 0x10) = 0;
        *(u8 **) (p + 0x1C) = *(u8 **) (p + 0x1C) - 1;
        return;
    }
    {
        u8 *q = *(u8 **) (p + 0x1C);
        u32 u;
        u32 v;
        s32 i;

        *(u8 **) (p + 0x1C) = q + 4;
        u = q[0] + (q[1] << 8) + (q[2] << 16) + (q[3] << 24);
        *(u8 **) (p + 0x1C) = q + 8;
        i = (u >> 0x13) & 0x1F;
        v = q[4] + (q[5] << 8) + (q[6] << 16) + (q[7] << 24);
        func_8003F540(*(u32 *) (*(u8 **) (p + 0x80) + 0x3C),
                      D_8006CD50[D_800721C0[i]],
                      u & 0xFF07FFFF, v);
        func_80053DA8(0x300);
        *(u32 *) (p + 0x10) = 0;
    }
}
