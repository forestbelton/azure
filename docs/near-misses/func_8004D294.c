/* func_8004D294 (unk_1C540) -- 114 of 114 words, 27 DIFF. NOT compiled.
 *
 * Wraps a caller's three Euler angles into 12-bit signed range, wraps
 * D_80083178's own three the same way, and snaps any axis that ended up more
 * than half a turn away from it; then hands both vectors to func_8004D1EC.
 * Three identical nine-word blocks, one per axis.
 *
 * WHAT IS PAID FOR
 *
 * * There are TWO func_8004D1EC calls, not one. m2c's output was truncated at
 *   the first; the second is the last statement of the `q != NULL` block,
 *   `func_8004D1EC(b + 0x140, q, a & 0xFFFF, (u16 *) (b + 0xAC))`. Without it
 *   the function is eight words short -- five for the call and three for the
 *   $s2 save, copy and restore, which only exist because `a` is used on both
 *   sides of the first call and therefore has to survive it.
 *
 * * D_80083178 is reached through a `u8 *` base with byte displacements, NOT
 *   through the struct. The listing materialises `%lo(D_80083178)` once into
 *   $s1 and uses 0xAC/0xAE/0xB0/0xB8/0xA4/0x140 off it -- and 0x140 is past
 *   the end of Unk80083178 anyway. The angles are read `lhu` and stored `sh`,
 *   so they are `*(u16 *)` and not the struct's `s16` SVECTOR fields.
 *
 * * The caller's vector is read BOTH ways per axis: `lhu` for the two masks
 *   and `lh` for the difference, so it is a `u8 *` with `*(u16 *)` and
 *   `*(s16 *)` at the same offset.
 *
 * WHAT IS LEFT: 27 words, nine in each of the three blocks, and it is ONE
 * register choice replicated three times. The target holds the caller's angle
 * in $v0 and the sign-extended wrapped reference in $v1:
 *
 *     sll  $v1, $a0, 16      <- this source: sll $v0, $a0, 16
 *     sh   $a0, 0xAC($s1)
 *     lh   $v0, 0x0($s0)     <- this source: lh $v1, 0x0($s0)
 *     sra  $v1, $v1, 16
 *     subu $v0, $v0, $v1     <- this source: subu $v0, $v1, $v0
 *
 * and the `lhu` for the `& 0xFFF` mask lands between the `slti` and the
 * `bnez` where this source puts it right after the `subu`, which shifts four
 * more rows per block. The two facts are one: whichever quantity gets $v0
 * decides where the spare load goes.
 *
 * Measured, all 114 words and all 27 DIFF: `w` as `s16` with no cast on the
 * subtraction; a separate `s16` local holding the sign-extended copy; the
 * subtraction written `-((s16) w - q)`; and re-reading the field
 * (`*(s16 *) (b + 0xAC)`) instead of using `w`. Reading the caller's angle
 * into its own local first is WORSE, 33 DIFF. So no spelling of the
 * subtraction moves it, and the next step is `tools/decomp/sched.py` on one
 * block -- section 8 of docs/matching-levers.md for what its output means.
 */

#include "common.h"

void func_8004D1EC(u8 *, u16 *, s32, u16 *);

void func_8004D294(u16 *p, u8 *q, s32 a)
{
    u8 *b = (u8 *)&D_80083178;
    u16 t;
    u16 v;
    u16 w;
    s32 d;

    if (p != NULL)
    {
        func_8004D1EC(b + 0xB8, p, a & 0xFFFF, (u16 *)(b + 0xA4));
    }
    if (q != NULL)
    {
        t = *(u16 *)(q + 0);
        if ((t & 0x800) != 0)
        {
            v = t | 0xF800;
        }
        else
        {
            v = t & 0x7FF;
        }
        *(u16 *)(q + 0) = v;
        t = *(u16 *)(b + 0xAC);
        if ((t & 0x800) != 0)
        {
            w = t | 0xF800;
        }
        else
        {
            w = t & 0x7FF;
        }
        *(u16 *)(b + 0xAC) = w;
        d = *(s16 *)(q + 0) - (s16)w;
        if (d < 0)
        {
            d = -d;
        }
        if (d >= 0x801)
        {
            *(u16 *)(q + 0) = (w & 0xF000) | (*(u16 *)(q + 0) & 0xFFF);
        }

        t = *(u16 *)(q + 2);
        if ((t & 0x800) != 0)
        {
            v = t | 0xF800;
        }
        else
        {
            v = t & 0x7FF;
        }
        *(u16 *)(q + 2) = v;
        t = *(u16 *)(b + 0xAE);
        if ((t & 0x800) != 0)
        {
            w = t | 0xF800;
        }
        else
        {
            w = t & 0x7FF;
        }
        *(u16 *)(b + 0xAE) = w;
        d = *(s16 *)(q + 2) - (s16)w;
        if (d < 0)
        {
            d = -d;
        }
        if (d >= 0x801)
        {
            *(u16 *)(q + 2) = (w & 0xF000) | (*(u16 *)(q + 2) & 0xFFF);
        }

        t = *(u16 *)(q + 4);
        if ((t & 0x800) != 0)
        {
            v = t | 0xF800;
        }
        else
        {
            v = t & 0x7FF;
        }
        *(u16 *)(q + 4) = v;
        t = *(u16 *)(b + 0xB0);
        if ((t & 0x800) != 0)
        {
            w = t | 0xF800;
        }
        else
        {
            w = t & 0x7FF;
        }
        *(u16 *)(b + 0xB0) = w;
        d = *(s16 *)(q + 4) - (s16)w;
        if (d < 0)
        {
            d = -d;
        }
        if (d >= 0x801)
        {
            *(u16 *)(q + 4) = (w & 0xF000) | (*(u16 *)(q + 4) & 0xFFF);
        }
        func_8004D1EC(b + 0x140, (u16 *)q, a & 0xFFFF, (u16 *)(b + 0xAC));
    }
}
