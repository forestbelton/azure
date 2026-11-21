/* PARK: func_800D81D4 -- **239 of 239 words, 0 DIFFERING**, 2026-11-20.
   NOT LANDED, AND NOT BECAUSE OF THE BODY.

   The fourth of the seven 4-mod-8 jump-table functions: it owns
   `jtbl_80089474` at rodata offset 0xD14.  cc1psx writes `.align 3` before
   every table it compiles, so inside one big ovl_dungeon.c this table gains
   four bytes of padding the original does not have and the container comes
   out eight bytes long.  **Do not merge it until the unit is carved in the
   same change** -- see docs/dungeon-parks/func_800D50FC.c and
   docs/dungeon-sweep.md, "SEVEN of the 28 jump tables are at 4 mod 8".

   Seven cases, a sine/cosine position update off `p + 0x18`, and a shared
   tail.  m2c gave a clean draft at 238 words / 22 rows; FOUR facts took it
   to 0, and all four are about operand WIDTH or ASSOCIATION rather than
   structure:

   1. `p + 0x4A` IS AN `lh`, NOT AN `lhu` -- `*(s16 *)` for every read of it.
      3 rows.
   2. `p + 0x1E` IS ALSO AN `lh`.  3 rows (two reads in the tail).
   3. `c + 0x10` IS A HALFWORD STORE: `*(u16 *)(c + 0x10) = 0x60;`, not
      `c[0x10] = 0x60;`.  1 row.
   4. THE TWO ASSOCIATION FACTS, which are the interesting ones:

        ((0x20 - *(s16 *)(p + 0x48)) << 3) * *(s16 *)(p + 0x4A)

      with the `<< 3` WRITTEN AS A SHIFT AND PARENTHESISED, because
      `(0x20 - x) * 8 * y` reassociates: the target emits `subu` / `lh 0x4A`
      / `sll 3` / `mult`, and the `* 8` spelling folds the 8 into the
      multiply instead, which is 238 words / 19 rows.  Measured, all on the
      same body: `x * 8 * y` 238/19, `(x * 8) * y` 238/19, `8 * x * y`
      238/19, `x * y * 8` 239/16, `y * (x << 3)` 239/8, and
      `(x << 3) * y` **239 words and 7 rows**, which is the one below.

        ((-*(s16 *)(p + 0x48) << 4) + 0x1E0)

      NEGATE THEN SHIFT.  The target is `negu $v0, $v0` then
      `sll $v0, $v0, 4`; `x * -0x10` and `-(x << 4)` both emit the shift
      first and are 239/4.  `(-x) * 0x10` measures identically to
      `(-x) << 4`, so either spelling of the shift is fine once the negation
      is on the inside.  Two places, 4 rows.

   Also worth keeping: case 0 FALLS THROUGH into case 1, and cases 1, 2, 3
   and 4 all end by sharing the `*(u16 *)(p + 0x4C) += 1;` increment, which
   jump2 cross-jumps -- writing each one out is correct and costs nothing.
   `func_80069EF8() % 96` is a signed modulo and comes out as the target's
   `div`/`mfhi` pair.

   DECLARATIONS
   NONE -- func_800478B8, func_80064710, func_80069EF8, func_800644B8 and
   func_80064584 all ride implicit declarations exactly as the surrounding
   landed code does, and D_800814A0 is already declared.
   END DECLARATIONS

   Measured with
     python3 tools/decomp/parkresidue.py func_800D81D4
*/
void func_800D81D4(u8 *p, u8 *b, u8 *c)
{
    s32 i;
    s32 n;

    *(u16 *)(p + 0x48) -= 1;
    func_800478B8(c);
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        *(u16 *)(p + 0x48) = 0x10;
        i = func_80069EF8() % 96 - 0x30;
        *(s32 *)(b + 0x14) = (i << 0x10) / *(s16 *)(p + 0x48);
        *(s32 *)(p + 0x28) = (func_80064710(0x900 - i * i) << 0x10) / *(s16 *)(p + 0x48);
        *(u16 *)(p + 0x18) = func_80069EF8();
        *(u16 *)(p + 0x4C) += 1;
    case 1:
        *(s32 *)(b + 0x8) += *(s32 *)(b + 0x14);
        *(s32 *)(p + 0x1C) += *(s32 *)(p + 0x28);
        *(s32 *)(c + 0xC) += 0x40404;
        *(u16 *)(p + 0x18) += ((0x20 - *(s16 *)(p + 0x48)) << 3) * *(s16 *)(p + 0x4A);
        if (*(s16 *)(p + 0x48) < 0)
        {
            *(u16 *)(p + 0x48) = 0x14;
            *(u16 *)(p + 0x4C) += 1;
        }
        break;
    case 2:
        *(u16 *)(p + 0x18) += *(s16 *)(p + 0x4A) * 0x140;
        if (*(s16 *)(p + 0x48) <= 0)
        {
            *(u16 *)(p + 0x48) = 0x1C;
            *(u16 *)(p + 0x4C) += 1;
        }
        break;
    case 3:
        *(s32 *)(b + 0x8) -= *(s32 *)(b + 0x14) >> 1;
        *(s32 *)(p + 0x1C) -= *(s32 *)(p + 0x28) >> 1;
        *(s32 *)(c + 0xC) += 0x30303;
        *(u16 *)(p + 0x18) += ((0x1C - *(s16 *)(p + 0x48)) * 8 + 0x140) * *(s16 *)(p + 0x4A);
        if (*(s16 *)(p + 0x48) <= 0)
        {
            *(u16 *)(p + 0x48) = 0;
            *(u16 *)(p + 0x4C) += 1;
        }
        break;
    case 4:
        *(u16 *)(p + 0x18) += ((-*(s16 *)(p + 0x48)) * 0x10 + 0x1E0) * *(s16 *)(p + 0x4A);
        *(s32 *)(c + 0xC) += 0x40404;
        if (c[0xC] >= 0xF1)
        {
            *(u16 *)(c + 0x10) = 0x60;
            *(s16 *)(b + 0x16) = -(func_80069EF8() & 3);
            *(u16 *)(p + 0x4C) += 1;
        }
        break;
    case 5:
        *(s32 *)(b + 0x8) += *(s32 *)(b + 0x14);
        *(s32 *)(b + 0x14) -= (func_80069EF8() & 0xFFF) << 6;
        *(s32 *)(p + 0x1C) -= 0x8000;
        *(u16 *)(p + 0x18) += ((-*(s16 *)(p + 0x48)) * 0x10 + 0x1E0) * *(s16 *)(p + 0x4A);
        *(s32 *)(c + 0xC) += 0xFFE7E7E8;
        if (c[0xC] < 0x18)
        {
            *(u16 *)(p + 0x4C) += 1;
        }
        break;
    case 6:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
    *(s16 *)(b + 0x2) = *(s16 *)(p + 0xC) + (func_800644B8(*(s16 *)(p + 0x18)) * *(s16 *)(p + 0x1E) >> 0xC);
    *(s16 *)(b + 0x6) = *(s16 *)(p + 0xE) + (func_80064584(*(s16 *)(p + 0x18)) * *(s16 *)(p + 0x1E) >> 0xC);
}
