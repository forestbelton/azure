/* func_8005A608 (unk_1C540) -- 92 of 92 words, 11 DIFF. NOT compiled.
 * WAS 88 of 92, 85 DIFF. The four missing words are now paid for and what is
 * left is one register chain.
 *
 * Claims a D_80086A40 slot for `p` -- the caller's own index when it gives
 * one, otherwise the first free slot of sixteen -- fills it, and asks
 * func_80059E94 for a handle. Returns the slot index, or -1 if there was no
 * free slot or no handle.
 *
 * THE LEVER, and it is new: the index parameter is `s32` and the function
 * RE-ASSIGNS IT ITS OWN TRUNCATION,
 *
 *     j = id;              // s16 -- the raw value, a plain copy
 *     id = (s16) id;       // the sign-extension, IN PLACE
 *     if (id == -1) { ...scan... }
 *     i = j;               // the else arm uses the raw
 *
 * which buys three of the four words at once:
 *
 *   1. `addu $v1, $a1, $zero`. `j = id` is a real copy because the next
 *      statement overwrites `id`, so the raw has to be saved first. Walking
 *      the parameter straight through (`s16 id` with `i = id`) lets the raw
 *      stay in $a1 and no copy is emitted.
 *   2+3. `sll $v0, $s1, 16` / `sra $a0, $v0, 16` in the ELSE arm. Assigning
 *      to `id` INVALIDATES cse's equivalences for that pseudo, so the else's
 *      `(s16) i` -- with `i` copied from the raw `j` -- is no longer the same
 *      expression as the entry's `(s16) id` and has to be recomputed. With a
 *      plain `s16 id`, cse forwards the entry's extension into the else (it
 *      follows the conditional jump: the label has one use and is preceded by
 *      a barrier) and those two words go missing.
 *
 * Three variables do NOT work: `j = id; t = (s16) id; if (t == -1)` is 88
 * words again, because the extension then lands in its own pseudo and the
 * parameter keeps $a1 with no copy. The assignment has to be back to `id`.
 *
 * WHAT IS LEFT: 11 words, and they are ONE chain, traced from the dumps:
 *
 *   the extension's intermediate pseudo takes $v0 here, so the entry's -1
 *   is pushed to $a2; the listing's extension is IN PLACE in $a1, which
 *   leaves $v0 for the -1 -- and then the then-arm's own
 *   `lui $v0, %hi(D_80086A40)` CLOBBERS it, so reload's `find_equiv_reg`
 *   cannot find the value and materialises `addiu $a0, $zero, -1` fresh,
 *   which is what the listing has. With the -1 parked in $a2 instead,
 *   find_equiv_reg finds it and emits `addu $a0, $a2, $zero`, and the knock-on
 *   runs through the else's index registers as well.
 *
 *   find_equiv_reg stops at a CODE_LABEL, which is why only the then-arm --
 *   the fall-through, with no label between it and the branch -- is affected.
 *
 *   So the whole residue is: get the extension's intermediate into $a1. It
 *   cannot share with `id`'s own pseudo, because `id = (s16) id` makes that
 *   pseudo's live range continuous across the shift, and local-alloc
 *   therefore sees a conflict. All 12 orders of the four entry statements
 *   were measured: 11 DIFF is the floor, 14 and 18 the alternatives.
 *
 * func_8005A778 is the same function with a third argument and a bound check
 * and is at 101/101 words with the same residue; func_8005A90C is a fourth
 * member. One register chain unlocks three functions.
 */

#include "common.h"

extern s32 D_8007382C[];

void func_8005A1D0(u32);
s32 func_80059E94(u32);

s32 func_8005A608(u8 *p, s32 id)
{
    s16 i;
    s16 j;
    s32 k;
    Unk80086A40 *e;
    s32 h;

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
    if (D_80086A40[i].unk_00 != -1)
    {
        func_8005A1D0(D_80086A40[i].unk_10);
    }
found:
    k = i;
    e = &D_80086A40[k];
    e->unk_00 = i;
    e->unk_04 = p;
    e->unk_08 = (*(u16 *) (p + 0x12) << 9) + 0xA20;
    e->unk_14 = *(u32 *) (p + 0xC) - e->unk_08;
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
