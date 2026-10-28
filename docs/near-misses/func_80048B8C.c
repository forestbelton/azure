/* func_80048B8C (unk_1AF38) -- 44 of 44 words, 14 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep row on 2026-10-27 so there is a source to
 * sweep, and the row's first finding is sharper than it was stated.
 *
 * Walks 8-byte entries from `p[0] + p[1] * 4` to `p[2]`; for each whose +2
 * halfword is 2, chases the pointer at +4 and, unless its 8 bit is already
 * set, sets that bit and runs an inner loop over 0xC-byte records clearing
 * the low 6 bits of the halfword at +4 and then setting them to 0xE.
 *
 * WHAT IS PAID FOR
 *
 * * THE MASK NEEDS ITS OWN STATEMENT, not just a `u32` local for the
 *   halfword. The row says "read the halfword into a u32 local first", and
 *   that is NOT sufficient: `u32 v = *w; ... v & ~0x3F` is still narrowed by
 *   fold to `andi $v0, $v0, 0xffc0`, because `v` provably fits in sixteen
 *   bits, and the function then runs a word SHORT (43) with nothing for
 *   loop.c to hoist. What works is the mask IN PLACE in a statement of its
 *   own -- `v = *w; v &= ~0x3F;` -- or equivalently `(u32) *w & 0xFFFFFFC0`
 *   with the cast written out: then the constant stays 32-bit, comes out as
 *   `addiu $t1, $zero, -0x40` hoisted into the prologue, and the loop has the
 *   listing's `and $v1, $v0, $t1`. Both spellings measure 44 words and
 *   14 DIFF; the inline one is 43 and 36.
 * * The two stores to the SAME halfword need the first one `volatile`, or
 *   gcc 2.7.2's flow deletes it as a dead store. An intervening read of `*q`
 *   keeps it too, but then sched fills the load-delay slot with it and the
 *   function is a word short.
 * * Both stores go through ONE `u16 *` local advanced by 6, or the volatile
 *   store is not strength-reduced and uses `4($v1)` where the target has
 *   `0($a1)`.
 * * The inner loop's exit reads the flag byte into a local and advances the
 *   record cursor AFTER it (`f = *q; q += 0xC;` then
 *   `while ((f & 0x80) == 0)`), which is func_80047054's shape in the same
 *   unit: that is what puts the `lbu` before the `addiu` with the increment
 *   in the branch's delay slot.
 *
 * WHAT IS LEFT: fourteen words, and the row's reading of them stands -- the
 * target has the entry pointer in $a2, the halfword cursor in $a1 and the
 * flag byte in $v1, this source $a1/$v1/$a2, and the `addiu` of the cursor's
 * base lands before the flag `sb` instead of after. perm.py over the six
 * inner-loop statements (base order is the best) and a search.py sweep both
 * found nothing better.
 *
 * 2026-10-28: WHAT THE FOURTEEN WORDS ARE, read off the aligned diff rather
 * than summarised as "registers". Every differing word is the same
 * instruction in the same place with a different register, except one swap,
 * and the registers map cleanly:
 *
 *     the record pointer q   built $a1   target $a2
 *     the halfword cursor w  built $v1   target $a1
 *     the flag byte / temps  built $a2   target $v1
 *
 * plus the swap this file already names (`addiu` of the cursor's base before
 * the flag `sb` instead of after). But the one difference that is NOT just a
 * renaming is in the mask:
 *
 *     target  lhu $v0,0($a1) ; and $v1,$v0,$t1 ; sh $v1,0($a1) ;
 *             addiu $v0,$v1,0xE ; sh $v0,0($a1)
 *     built   lhu $v0,0($v1) ; and $v0,$v0,$t1 ; sh $v0,0($v1) ;
 *             addiu $v0,$v0,0xE ; sh $v0,0($v1)
 *
 * The listing masks into a DIFFERENT register from the load and then reuses
 * the load's register for the `+ 0xE`; this build masks in place. So the
 * target has two pseudos where this source has one, and the registers follow
 * from that rather than the other way round.
 *
 * SIX SPELLINGS MEASURED FOR THAT, all 44 of 44 words and the same 14 DIFF:
 * a separate `u32 m` local for the masked value, declared after `v` and
 * before it; the same with the mask written `0xFFFFFFC0`; and `m`, `v`, or
 * both moved out to FUNCTION scope. local-alloc ties them every time, which is
 * `combine_regs` doing what it does -- `v` dies at the `and`, so the dest is
 * tied to the dying source and the two pseudos share a register.
 *
 * AND THE func_800565D8 CURE DOES NOT REACH IT. That function refused the same
 * tie by making the destination a CROSS-BLOCK pseudo that local-alloc is not
 * allocating, which is why moving the declaration to function scope was worth
 * trying here. It does nothing, and the reason is worth keeping: `reg_basic_block`
 * is computed from where a pseudo is actually USED, not from where it is
 * declared, and every use here is inside the inner do-while. Giving `m` a use
 * in another block would cost an instruction. So the lever needs a second USE
 * elsewhere, not a wider declaration.
 */

#include "common.h"

void func_80048B8C(u32 *p) {
    u8 *e;

    e = (u8 *) (p[0] + p[1] * 4);
    while ((u32) e < p[2]) {
        if (*(s16 *) (e + 2) == 2) {
            u8 *q = *(u8 **) (e + 4);

            if ((*q & 8) == 0) {
                u16 *w = (u16 *) (q + 4);
                u8 f;

                *q |= 8;
                do {
                    u32 v;

                    v = *w;
                    v &= ~0x3F;
                    *(volatile u16 *) w = v;
                    *w = v + 0xE;
                    w += 6;
                    f = *q;
                    q += 0xC;
                } while ((f & 0x80) == 0);
            }
        }
        e += 8;
    }
}
