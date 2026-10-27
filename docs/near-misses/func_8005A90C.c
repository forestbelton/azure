/* func_8005A90C (unk_2CDC0) -- 100 of 103 words, 92 DIFF. NOT compiled.
 *
 * The third member of the func_8005A608 / func_8005A778 family, and the one
 * with the longest tail: it claims a D_80086A40 slot the same way the other
 * two do, fills in six fields, and then runs the handle through
 * func_80059F8C, func_8005ECA0 and func_8005EC40, bailing out with -1 at each
 * step. The tail is func_8005AAA8's (landed) word for word, so the +0x0C
 * store is spelled its way, through a `u8 *` cast.
 *
 * WHAT IS PAID FOR. The dispatch, the search loop and every instruction of
 * the body are in place: `j = id; id = id << 16; id = id >> 16;` for the
 * in-place sign-extension (matching-levers section 7, the family's shared
 * lever), `s16 i` so the counter's increments and its `< 0x10` test come out
 * as sll/sra pairs, `k = i` once at the join so there is a single
 * `sra $s2, $v0, 16` with two predecessors, and the six field stores in the
 * listing's order with `e->unk_10 = b` left for the `jal`'s delay slot.
 *
 * WHAT IS LEFT: FOUR words, and they are two callee-saved registers. The
 * listing uses six (p, b, c, i, k and the entry pointer all live across the
 * calls); this uses four, merging c with the entry pointer and i with
 * func_8005ECA0's result, which costs two save/restore pairs and cascades
 * into 92 differing register names. The merge is reload's, not the source's:
 * `c` is still live at `e->unk_08 = c`, which is a store THROUGH the entry
 * pointer, so the two overlap and cannot legally share -- the copy to `$a1`
 * that frees the register is func_80059F8C's own argument load, moved ahead
 * of the stores. Measured and not working: all six orders of the three
 * prologue statements (the committed one is best); `e` as `Unk80086A40 *`
 * with field accesses rather than a `u8 *` with casts; `s16 k`; and the
 * `(s16) id` cast form in place of the two shifts.
 *
 * The family's other two parks (func_8005A608 at 11 DIFF, func_8005A778 at
 * 21) are stuck on a different fact -- which register the entry's `-1`
 * comparison constant lands in, and whether cc1psx materialises it once or
 * three times -- so this one is NOT blocked on them.
 *
 * SINCE PARKED, 2026-10-27: 99 of 103 words to 102 of 103, by declaring both
 * `r` and `t` `s16` rather than `s32` and `u32`. Three words, found by
 * search.py's declaration-width family in the sweep of all forty parks -- it
 * needed BOTH, which is why a single-change sweep had to compose them over
 * two passes. The differing count stays at 92 because one word is still
 * missing and everything after it reads as wrong; advise.py's branch spans
 * are the way to localise it, and the notes below predate these numbers.
 *
 * A SWEEP RESULT WAS REVERTED HERE, 2026-10-27: search.py's
 * declaration-width family proposed `s16 t` and it scores better, but it is
 * SEMANTICALLY WRONG -- t takes func_8005ECA0's return value, declared `u32 func_8005ECA0(u32)`, and is stored with `*(u32 *) (e + 0x10) = t`.
 *
 * The companion narrowing `s16 r` IS kept -- it is worth one word -- but it
 * is NOT proven: func_80059F8C is declared `s32 func_80059F8C(s32, u32)`,
 * `r` is compared against -1 and stored through `*(u32 *) (e + 0x10)`, so
 * s16 is faithful only if that function cannot return a value outside s16
 * range. func_80059F8C is itself still an asm stub, so nothing settles it
 * here. Check that before trusting this park's shape.
 * The score is an oracle for SHAPE, not for correctness, and the width
 * family is the one that can change VALUES rather than representations.
 * See docs/matching-levers.md on reviewing a narrowing against the
 * listing's own arithmetic before keeping it.
 */

#include "common.h"

extern s32 D_8007382C[];

void func_8005A1D0(u32);
s32 func_80059F8C(s32, u32);
u32 func_8005ECA0(u32);
u32 func_8005EC40(u8 *, u32);

s32 func_8005A90C(u8 *p, s32 b, u32 c, s32 id)
{
    s16 i;
    s16 j;
    s32 k;
    u8 *e;
    s16 r;
    u32 t;

    D_8007382C[0] = 0;
    i = 0;
    j = id;
    id = id << 16;
    id = id >> 16;
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
    e = (u8 *) &D_80086A40[k];
    *(u32 *) (e + 0x14) = c;
    e[0x18] = 0x7F;
    *(s16 *) e = i;
    *(u8 **) (e + 4) = p;
    *(u32 *) (e + 8) = c;
    e[0x1B] = 0x40;
    *(u32 *) (e + 0x10) = b;
    r = func_80059F8C(b, c);
    *(u32 *) (e + 0x10) = r;
    if (r == -1)
    {
        return -1;
    }
    t = func_8005ECA0(r);
    if (func_8005EC40(p, *(u32 *) (e + 0x14)) != *(u32 *) (e + 0x14))
    {
        return -1;
    }
    *(u32 *) (e + 0xC) = (u32) p;
    *(u32 *) (e + 0x10) = t;
    return k;
}
