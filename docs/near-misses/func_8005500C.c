/* func_8005500C (unk_1C540) -- 55 of 55 words, 33 DIFF. NOT compiled.
 *
 * Dispatches on `arg & 0xF000`: 0x1000/0x2000/0x4000 each call
 * func_800553D4 with 0x71/0xE1/0xF1 (jump2 cross-jumps the three calls into
 * one site, which is why the listing has a single `jal` reached from three
 * delay slots), while 0, 0x8000 and 0x9000 share a body that stores the low
 * byte at D_800847D0+0x26 -- twice, the second time or-ed with the nibble --
 * and then branches on D_800847D0[0] & 0x100.
 *
 * Every opcode and immediate matches and the case bodies are in the right
 * order. TWO things are left and they are probably one thing:
 *
 *   - `advise.py` reports BRANCH-ARMS at 8005506C: built `beq`, target `bne`.
 *     The target's last compare in the tree (against 0x9000) FALLS THROUGH
 *     into the shared body; this source branches to it, and reorg then steals
 *     the body's first `lui` into the delay slot and emits a second copy.
 *     No arrangement of the case labels found so far inverts it -- gcc builds
 *     the tree from the sorted values, not from source order.
 *   - a 3-way register rotation: built {k=$v1, arg=$a0, base=$a1} against the
 *     target's {k=$a0, arg=$a1, base=$v1}. The target spends a parameter copy
 *     `addu $a1,$a0,$zero` that this source does not need, because its `k`
 *     took $a0.
 *
 * MEASURED AND NOT WORKING: `switch (a & 0xF000)` with the mask recomputed
 * inside the case body (56 words -- it buys the copy but adds a word);
 * declaring `b` before `v`; `v |= k` as a separate statement; and the whole
 * of `search.py`'s greedy sweep (13 variants, 8 families, nothing improved on
 * the base).
 */

#include "common.h"
extern u32 D_800847D0[];
extern s16 D_800848F8[];

void func_800553D4(s32);
void func_800550E8(void);
void func_80054F9C(s32, s16 *);

void func_8005500C(s32 a) {
    s32 k = a & 0xF000;

    switch (k) {
    case 0x1000:
        func_800553D4(0x71);
        break;
    case 0x4000:
        func_800553D4(0xF1);
        break;
    case 0x0:
    case 0x8000:
    case 0x9000: {
        u8 *b = (u8 *) D_800847D0;
        s32 v = a & 0xFF;

        *(s16 *) (b + 0x26) = v;
        if (k != 0) {
            *(s16 *) (b + 0x26) = v | k;
        }
        if (D_800847D0[0] & 0x100) {
            func_80054F9C(0xB1, D_800848F8);
        } else {
            func_800550E8();
        }
        break;
    }
    case 0x2000:
        func_800553D4(0xE1);
        break;
    }
}
