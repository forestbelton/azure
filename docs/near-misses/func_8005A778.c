/* func_8005A778 (unk_2CDC0) -- 101 of 101 words, 21 DIFF. NOT compiled.
 *
 * func_8005A608 with a third argument and a bound check: claims a D_80086A40
 * slot for `p`, stores `h` at +0x10 twice (once as given, once as
 * func_80059F8C's answer), and returns -1 when the caller's index is out of
 * range as well as when there is no free slot or no handle.
 *
 * Everything structural is paid for, including:
 *
 *   * `j = id; id = (s16) id;` -- the lever written up in
 *     docs/near-misses/func_8005A608.c and in matching-levers section 7. It
 *     buys the raw parameter's copy AND stops cse forwarding the entry's
 *     sign-extension into the else arm, which is three words.
 *   * TWO variables hold `p`: the parameter for the `e->unk_04 = p` store and
 *     a second local `q` for the four field reads. That is the listing's
 *     `addu $s3, $s4, $zero`, a second callee-saved register for the same
 *     pointer.
 *   * `e->unk_10 = h` comes AFTER `e->unk_08 = ...` in the source, so sched2
 *     can lift the store into the `lhu 0x12($s3)` load-delay slot. Written
 *     before it, the slot takes a nop.
 *   * The bound check is on `(s16) i` AFTER `i = j`, which is what gives the
 *     else arm its own `sll`/`sra` pair ahead of the `slti 0x10`.
 *
 * SINCE PARKED: writing the sign-extension as two statements --
 * `id = id << 16; id = id >> 16;` instead of `id = (s16) id` -- reproduces
 * the listing's IN-PLACE `sll $a1, $a1, 16` / `sra $a1, $a1, 16` here too
 * (23 differing words rather than 21, but structurally closer; see the lever
 * in matching-levers section 7). What is then left is exactly
 * func_8005A608's single scheduling fact: the listing emits
 * `sw $zero, %lo(D_8007382C)($v0)` AFTER the shifts and this source emits it
 * before, which decides whether $v0 is still holding the symbol's high half
 * when `j = id` is allocated.
 *
 * THE OLDER ANALYSIS: 21 words, all of them the one register chain func_8005A608's
 * park describes -- the extension's intermediate takes $v0 here instead of
 * being in place in $a1, which pushes the entry's -1 to $a2, which lets
 * reload's find_equiv_reg reuse it in the then-arm and in the else instead of
 * materialising fresh, which in turn moves the else's index registers. Fix
 * that one allocation and this function, func_8005A608 and func_8005A90C all
 * fall.
 */

#include "common.h"

extern s32 D_8007382C[];

void func_8005A1D0(u32);
s32 func_80059F8C(s32, u32);

s32 func_8005A778(u8 *p, s32 id, s32 h)
{
    u8 *q = p;
    s16 i;
    s16 j;
    s32 k;
    Unk80086A40 *e;
    s32 r;

    D_8007382C[0] = 0;
    i = 0;
    j = id;
    id = (s16) id;
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
    i = j;
    if (i >= 0x10)
    {
        return -1;
    }
    if (D_80086A40[i].unk_00 != -1)
    {
        func_8005A1D0(D_80086A40[i].unk_10);
    }
found:
    k = i;
    e = &D_80086A40[k];
    e->unk_00 = i;
    e->unk_04 = p;
    e->unk_08 = (*(u16 *) (q + 0x12) << 9) + 0xA20;
    e->unk_10 = h;
    e->unk_14 = *(u32 *) (q + 0xC) - e->unk_08;
    e->unk_18 = q[0x18];
    e->unk_1B = q[0x19];
    r = func_80059F8C(h, e->unk_14);
    e->unk_10 = r;
    if (r == -1)
    {
        return -1;
    }
    return k;
}
