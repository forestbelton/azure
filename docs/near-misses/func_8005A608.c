/* func_8005A608 (unk_1C540) -- 88 of 92 words, 85 DIFF. NOT compiled.
 *
 * Claims a D_80086A40 slot for `p` -- the caller's own index when it gives
 * one, otherwise the first free slot of sixteen -- fills it, and asks
 * func_80059E94 for a handle. Returns the slot index, or -1 if there was no
 * free slot or no handle.
 *
 * include/structs.h gained Unk80086A40.unk_08, .unk_14 and .unk_18, split out
 * of pad_08 and pad_14.
 *
 * WHAT IS PAID FOR: the whole structure. Every branch target, every `sltiu`
 * bound, the 28-byte stride, the `i` declared `s16` (the listing sign-extends
 * it at each use and stores it back with `sh`), `i = 0` BEFORE the `id == -1`
 * test (the listing has `addu $s1, $zero, $zero` in the entry block) and the
 * `do`/`while` with no entry guard.
 *
 * WHAT IS LEFT: 4 words, and all four are the ELSE block sharing values with
 * the entry block that the listing recomputes:
 *
 *   1. `addu $v1, $a1, $zero` -- the listing saves the RAW `id` before
 *      sign-extending it IN PLACE into $a1. Here the extension goes to a
 *      different register and no copy is needed.
 *   2. `sll`/`sra` -- the listing re-extends `(s16) i` inside the else block.
 *      Here the entry block's `(s16) id` is still live and gcc reuses it.
 *   3. `addiu $v0, $zero, -1` -- the listing materialises the -1 a second
 *      time in the else block; here loop.c hoists one into the preheader and
 *      the else copies it.
 *
 * All three are one question: cse carries the entry block's table into the
 * else block here and does not in the target. `-fcse-follow-jumps` is on at
 * -O2 and `cse_end_of_basic_block` will follow a conditional jump whose label
 * has exactly one use, which is this one -- so the lead is what makes the
 * target's cse stop there. Measured and not working, all at 88 words: the
 * else indexing through `id` instead of `i` (92 DIFF), an `s32` parameter with
 * the cast on the comparison (85), and `i = id` hoisted above the test with
 * `i = 0` moved into the then-arm (84).
 */

#include "common.h"

extern s32 D_8007382C[];

void func_8005A1D0(u32);
s32 func_80059E94(u32);

s32 func_8005A608(u8 *p, s16 id)
{
    s16 i;
    s32 k;
    Unk80086A40 *e;
    s32 h;

    D_8007382C[0] = 0;
    i = 0;
    if (id == -1)
    {
        do
        {
            if (D_80086A40[i].unk_00 == -1)
            {
                goto found;
            }
            i++;
        } while (i < 0x10);
        return -1;
    }
    i = id;
    if (D_80086A40[i].unk_00 != -1)
    {
        func_8005A1D0(D_80086A40[i].unk_10);
    }
found:
    k = i;
    e = &D_80086A40[k];
    e->unk_00 = i;
    e->unk_04 = p;
    e->unk_08 = (*(u16 *)(p + 0x12) << 9) + 0xA20;
    e->unk_14 = *(u32 *)(p + 0xC) - e->unk_08;
    e->unk_18 = p[0x18];
    e->unk_1B = p[0x19];
    h = func_80059E94(e->unk_14);
    e->unk_10 = h;
    if (h == -1)
    {
        return -1;
    }
    return k;
}
