/* func_80052A90 (unk_1C540) -- 96 of 96 words, 1 DIFF. NOT compiled.
 *
 * Blits a Shift-JIS string one 3x16 cell at a time: maps each two-byte code
 * to a font index, pulls the cell out of VRAM at 0x340 with StoreImage and
 * puts it back at (x, y) with LoadImage.
 *
 * WHAT IS PAID FOR -- everything but one word:
 *
 * * the last three arms of the code chain are a SWITCH, not two more
 *   `else if`s, with `default:` written FIRST. As an if/else chain each
 *   body follows its own test and the `beq` comes out as a `bne`; as a
 *   two-case switch gcc emits both equality tests and then the bodies in
 *   source order, which is the listing's default, 0xE, 0xD layout.
 * * the first and fourth arms produce the SAME value (`c + 0x7DC1`), which
 *   is why the listing has one `j .L80052B70` serving both -- cross-jumping
 *   merged them, so both arms have to be written out.
 * * `c` is a `u16`, so each use as an int is a zero-extension; the three
 *   equality tests share one, and the rect computation gets its own through
 *   `d = c;` -- without that local gcc folds the first mask into `andi 7`
 *   and re-extends for the shift (4 words' worth of difference).
 * * the two `RECT` fills are written out in full, w and h included, even
 *   though both are 3 and 0x10: the listing re-stores `$s5` and `$s4` the
 *   second time, and loop.c hoists both constants into the preheader.
 *
 * WHAT IS LEFT: ONE word. The listing tests the flag as
 * `addu $v0, $s3, $zero` / `beqz $v0` -- a register COPY and then a zero
 * test. `s32 ok` gives `beqz $s3` alone (95 words), `u8 ok` gives
 * `andi $v0, $s3, 0xFF` / `beqz $v0` (the right count, wrong opcode, and the
 * version here), `ok == 1` gives `addiu $v0, $zero, 1` / `bne` (96/2).
 *
 * A plain reg-to-reg move before a branch means combine turned a
 * `(zero_extend (subreg:QI x))` into a copy -- it knows the flag's nonzero
 * bits -- and the copy survived because the two pseudos conflict.
 * `set_nonzero_bits_and_sign_copies` gives up on a pseudo with more than one
 * SET, and this flag has two (the `1` before the loop and the `0` in the
 * default arm), so the question is what spelling leaves it with one set and
 * still a live copy. `t = ok; if (t != 0)` does not: copy propagation folds
 * it away (95 words).
 *
 * SINCE PARKED (advise.py): the one word is `andi $v0, $s3, 0xFF` where the
 * listing has `addu $v0, $s3, $zero` -- a plain copy of the flag into the
 * branch's register rather than a zero-extension of a QImode pseudo. So the
 * listing's `ok` is 32 bits wide AND there is still a real copy before the
 * test, which is the part no spelling found reaches: `u32`/`s32 ok` drops the
 * word entirely (95 words, 41 DIFF, the register assignment cascading), and
 * an explicit `t = ok; if (t != 0)` is propagated away. Also measured: `if
 * (ok)` without the comparison (identical).
 */

#include "common.h"

void func_80052A90(u32 s, s32 x, s16 y)
{
    RECT r;
    u8 buf[0x80];
    u8 ok = 1;
    u16 c;
    u32 d;

    while (*(u8 *)s != 0)
    {
        c = (*(u8 *)s << 8) | *(u8 *)(s + 1);
        s += 2;
        if (c == 0x8140)
        {
            c = 0;
        }
        else if ((u16)(c + 0x7DA0) < 0x1A)
        {
            c = c + 0x7DC1;
        }
        else if ((u16)(c + 0x7D7F) < 0x1A)
        {
            c = c + 0x7DC0;
        }
        else if ((u16)(c + 0x7DB1) < 0xA)
        {
            c = c + 0x7DC1;
        }
        else
        {
            switch (c)
            {
            default:
                ok = 0;
                break;
            case 0x8144:
                c = 0xE;
                break;
            case 0x817C:
                c = 0xD;
                break;
            }
        }
        if (ok != 0)
        {
            d = c;
            r.x = (d & 7) * 3 + 0x340;
            r.y = (d >> 3) << 4;
            r.w = 3;
            r.h = 0x10;
            StoreImage(&r, (u_long *)buf);
            r.x = x;
            r.y = y;
            r.w = 3;
            r.h = 0x10;
            LoadImage(&r, (u_long *)buf);
            DrawSync(0);
        }
        x += 3;
    }
}
