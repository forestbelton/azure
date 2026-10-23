/* func_8003F5AC alone, carved out of the tail of unk_12A70: it reaches
 * D_80080AD4 and D_80083958 $gp-relatively and nothing absolutely at all, so
 * it was its own original translation unit. Built with MASPSXFLAGS_G8; see
 * docs/addressing-and-translation-units.md.
 */
#include "common.h"

s32 func_8003F270(void);
void func_8003E758(void);
u32 func_8003E240(s32);

/* Returns the status byte at D_8008395C, with bit 2 set while the D_80080AD4
 * counter is nonzero.
 *
 * Two shapes are load-bearing in ten words:
 *  - the else arm must contain an EXPLICIT QImode truncation of a value whose
 *    type is SImode -- `r = (u8) flags;` with `flags` declared `u32` -- and the
 *    two arms must JOIN rather than return early. The `andi $v0,$v0,0xFF` the
 *    target has in that arm is that cast surviving: combine folds `(and x 255)`
 *    away only when it can prove x's high bits, and `nonzero_bits`'s record of
 *    the `lbu` is valid only inside the block containing it
 *    (reg_last_set_label == label_tick), so a cast in a DIFFERENT basic block
 *    from the load is not folded. Declaring `flags` as `u8`, returning it with
 *    no cast, or an early `return` instead of the join all fold it away: 8
 *    words against 10, 6 differing. Two other spellings also match -- a `u32`
 *    return with `v |= 4;` / `v &= 0xFF;` on one variable, and the same with a
 *    separate result variable.
 *  - the byte is reached as `D_80083958.unk_04`, a member of the existing
 *    object, NOT as a new scalar symbol at 0x8008395C. As its own `extern u8`
 *    it is small data in this -G8 unit and the `lui %hi` / `lbu %lo` pair
 *    collapses to a single `lbu %gp_rel($gp)`: 7 words against 10, 10 DIFF.
 *    verify.py therefore marks the `lbu` word `?`, because the reloc is
 *    `%lo(D_80083958+4)` where the listing prints `%lo(D_8008395C)`; the two
 *    are the same address and `make check` is the authority. */
u8 func_8003F5AC(void) {
    u32 flags = D_80083958.unk_04;
    u32 r;

    if (D_80080AD4 != 0) {
        r = flags | 4;
    } else {
        r = (u8) flags;
    }
    return r;
}
