/* func_80054B08 (unk_1C540) -- 84 of 84 words, 32 DIFF. NOT compiled.
 *
 * Dispatches on `arg & 0xF000`: 0x1000/0x2000/0x4000 each call func_80054E00
 * with 0x74/0xE4/0xF4 (jump2 cross-joins the three into one call site), and 0
 * runs the big block -- func_8003F534's record copied into D_800847D0 with the
 * top byte of each of two words split off into +0x31/+0x33 and the low 24 bits
 * rebased on the record's +0x04.
 *
 * WAS 82/84, two words short. Both of them are now paid for:
 *
 * * THE THREE STORES TO +0x10 AND THE THREE TO +0x14 ARE VOLATILE. The
 *   listing writes each field three times with a progressively refined value
 *   and never reads it back, so gcc's flow pass deletes the first two of each
 *   as dead -- that was the two-word gap. In-place field updates
 *   (`*(u32 *) (b + 0x10) &= 0xFFFFFF;`) do not help: cse forwards the stored
 *   value, the loads vanish, and the stores are dead again (measured, still
 *   82). `*(volatile u32 *)` on the six stores is the only spelling that keeps
 *   them, and it brings the count to 84. Marking the whole base `volatile`,
 *   including +0x1C, +0x18 and +0x04, is WORSE (36 DIFF): only the two
 *   repeatedly-written fields want it.
 *
 * * The -0x20 and +0x20 go in their OWN statements (`w -= 0x20; w += base;`).
 *   As `(w & 0xFFFFFF) - 0x20 + base` fold reassociates the constant onto
 *   `base`, so gcc computes `base - 0x20` once and shares it with the `+ 0x20`
 *   side, where the listing spends `addiu` then `addu` on each.
 *
 * WHAT IS LEFT: 32 words of schedule and allocation, no shape findings. The
 * target emits the block in source order -- `sh` to +0x1C, then the three
 * +0x10 stores with `sb` to +0x31 between the second and third, then
 * +0x18 = 0, then the three +0x14 stores with `sb` to +0x33 -- while this
 * hoists the two `lw`s of the record's +0x08/+0x0C to the top, sinks the `sh`
 * and both `sb`s to the end, and puts `sw $zero, 0x18` third. The volatile
 * stores are pinned and the non-volatile ones move around them.
 *
 * Reading the reads at their point of use rather than in the declarations
 * (measured) does not change that, so the next thing to look at is
 * `tools/decomp/sched.py` on this function: which priority group the four
 * non-volatile stores land in, and whether `potential_hazard` is what orders
 * them. See section 8 of docs/matching-levers.md for what the keys are.
 */

#include "common.h"
extern u32 D_80084864[];

void func_80054E00(s32);
s32 func_80054AF0(s32);
void func_80054C58(void);
void func_80054CD4(void);

void func_80054B08(s32 a) {
    switch (a & 0xF000) {
    case 0x0: {
        Unk8006CE34 *q = func_8003F534();
        u8 *b = (u8 *) D_800847D0;
        u16 n;
        u32 base;
        u32 w;
        u32 x;
        u32 f;

        n = *(u16 *) q;
        *(s16 *) (b + 0x1C) = n;
        base = q->unk_04;
        w = q->unk_08;
        *(volatile u32 *) (b + 0x10) = w;
        x = q->unk_0C;
        b[0x31] = w >> 24;
        w &= 0xFFFFFF;
        *(volatile u32 *) (b + 0x10) = w;
        w -= 0x20;
        w += base;
        *(volatile u32 *) (b + 0x10) = w;
        *(u32 *) (b + 0x18) = 0;
        *(volatile u32 *) (b + 0x14) = x;
        b[0x33] = x >> 24;
        x &= 0xFFFFFF;
        *(volatile u32 *) (b + 0x14) = x;
        x += 0x20;
        x += base;
        *(volatile u32 *) (b + 0x14) = x;
        f = D_800847D0[0];
        if (f & 0x400) {
            D_800847D0[0] = f | 0x4000;
            D_80084864[0] = 2;
        } else if ((*(u32 *) (b + 4) & 0x200) == 0) {
            s32 r;

            *(u32 *) (D_80084858 + 4) = 0;
            r = func_80054AF0((s16) n);
            *(s16 *) (D_80084858 + 8) = r;
            *(s16 *) (D_80084858 + 0xA) = r;
            func_80054C58();
            func_80054CD4();
        }
        break;
    }
    case 0x1000:
        func_80054E00(0x74);
        break;
    case 0x2000:
        func_80054E00(0xE4);
        break;
    case 0x4000:
        func_80054E00(0xF4);
        break;
    }
}
