/* func_80054B08 (unk_1C540) -- 82 of 84 words, two words short. NOT compiled.
 *
 * Dispatches on `arg & 0xF000`: 0x1000/0x2000/0x4000 each call func_80054E00
 * with 0x74/0xE4/0xF4 (jump2 cross-joins the three into one call site), and 0
 * runs the big block -- func_8003F534's record copied into D_800847D0 with the
 * top byte of each of two words split off into +0x31/+0x33 and the low 24 bits
 * rebased on the record's +0x04, then either setting 0x4000 and D_80084864, or
 * running func_80054AF0/func_80054C58/func_80054CD4.
 *
 * The case bodies are in the right ORDER (case 0 first, then 0x1000, 0x2000,
 * 0x4000 -- that is the target's layout) and the three repeated stores to each
 * of +0x10 and +0x14 are right.
 *
 * TWO WORDS SHORT, and advise.py localises it: `IMMEDIATE` at 80054C14, built
 * `sh` offset 6 where the target has 8. So gcc has biased the D_80084858 base
 * by 2 and is reaching +0x4/+0x8/+0xA as +0x2/+0x6/+0x8 -- the "constant
 * indices on an array name bias the base" entry in carved-sweep, seen from the
 * losing side. MEASURED: the fix that entry prescribes, a local `u8 *c =
 * D_80084858;` with `c + 4` etc., does NOT work here -- still 82 words and 61
 * DIFF instead of the original 2-word gap. So the bias is coming from
 * somewhere else and the base wants a different spelling again.
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
        u16 n = *(u16 *) q;
        u32 base = q->unk_04;
        u32 w = q->unk_08;
        u32 x = q->unk_0C;
        u32 f;

        *(s16 *) (b + 0x1C) = n;
        *(u32 *) (b + 0x10) = w;
        *(u32 *) (b + 0x10) = w & 0xFFFFFF;
        b[0x31] = w >> 24;
        *(u32 *) (b + 0x10) = (w & 0xFFFFFF) - 0x20 + base;
        *(u32 *) (b + 0x18) = 0;
        *(u32 *) (b + 0x14) = x;
        *(u32 *) (b + 0x14) = x & 0xFFFFFF;
        b[0x33] = x >> 24;
        *(u32 *) (b + 0x14) = (x & 0xFFFFFF) + 0x20 + base;
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
