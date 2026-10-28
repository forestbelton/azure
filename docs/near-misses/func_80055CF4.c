/* func_80055CF4 (unk_2879C) -- 35 of 36 words, 31 DIFF. NOT compiled.
 *
 * Written out of the shared coalesced-copy row on 2026-10-27, with the CAUSE
 * of its missing word identified and the row's "same word on a SYMBOL's
 * address" claim made concrete (that sentence is truncated in the row).
 *
 * Scans the four top bits of D_800847D0's +0x04 flag word from the bottom,
 * and on the first one set calls func_80055D84 with its index and clears it.
 *
 * THE MISSING WORD IS `addu $s1, $v0, $zero`, and the reason is `b`'s LIVE
 * RANGE, not a spelling. The target materialises the symbol's address once
 * and COPIES it:
 *
 *     lui   $v0, %hi(D_800847D0)
 *     addiu $v0, $v0, %lo(D_800847D0)
 *     addu  $s1, $v0, $zero          <- the word this source does not have
 *     ...
 *     lw    $v1, 0x4($s1)
 *
 * `b` is live across `jal func_80055D84`, so it must end up callee-saved;
 * the target produces the address into a caller-saved temp and copies it in,
 * while this source has local-alloc produce the lui/addiu pair straight into
 * $s1 and needs no copy. That is the row's shared problem in its clearest
 * form, and none of its five cures applies: there is nothing to
 * re-dereference, no loop bound to guard, no statement available between the
 * producer and the copy, no second value to unify, and TODAY'S RETURN-TYPE
 * LEVER DOES NOT HELP -- `s32` with no return measures 35 words and 31 DIFF,
 * identical, because the copy wants to come OUT of $v0 and making $v0
 * live-out does not stop the address being produced in $s1.
 *
 * ONE VARIANT SCORES MUCH BETTER AND IS NOT THE SHAPE, which is worth
 * recording rather than adopting. Reading the flag word through the SYMBOL
 * inline and assigning `b` AFTER it --
 *
 *     f = *(u32 *) ((u8 *) D_800847D0 + 4) & 0xFF000000;
 *     b = (u8 *) D_800847D0;
 *
 * -- gives 36 words and 12 DIFF, because the second materialisation supplies
 * a third address word and the whole body then aligns. But its three address
 * insns are `lui $v1, %hi` / `addiu $v0, $v1, 4` / `addiu $s1, $v0, -4`,
 * where the target has `lui` / `addiu` / `addu` -- so the count is right for
 * the wrong reason and 12 of the 12 are still in that region. Resume from the
 * shape below, not from that.
 *
 * Also measured, all 35 words and 31 DIFF: everything through the symbol with
 * no `b` at all, and the loop as an explicit `i = 0; do { ... } while (i < 4);`. *
 * 2026-10-27: THE MISSING COPY IS DOWNSTREAM OF THE SAVE'S POSITION. The park
 * above reads it as local-alloc producing the `lui`/`addiu` pair straight into
 * $s1 because `b` must be callee-saved. That is true but it is the second
 * fact; the words say which order causes it:
 *
 *     target  lui $v0,%hi ; addiu $v0,$v0,%lo ; sw $s1,0x14($sp) ;
 *             addu $s1,$v0,$zero
 *     built   lui $v0,%hi ; sw $s1,0x14($sp) ; addiu $s1,$v0,%lo
 *
 * The listing COMPLETES the address in $v0 and only then saves $s1 and copies
 * into it. This build interleaves: the `sw $s1` lands between the `lui` and
 * the `addiu`, so by the time the `addiu` runs $s1 is already free and it
 * writes there directly -- and a copy that would have gone from $v0 to $s1 has
 * nothing to do. So the question is not how to force a second quantity for the
 * address; it is why the save is scheduled INTO the pair here and after it in
 * the listing. sched2 interleaves the prologue saves with the body (section 8),
 * and both the save and the `addiu` are in that interleave.
 *
 * Measured and NOT working, all 35 of 36 words and 31 DIFF: `b` declared last
 * among the locals, declared in the middle, declared without an initialiser
 * and assigned as the first statement, and an explicit
 * `u8 *t = (u8 *) D_800847D0; u8 *b = t;` temp pair -- so neither declaration
 * ORDER (section 8's prologue tie-break) nor an explicit second variable
 * reaches it. Section 3's seventh cure does not apply either: the value whose
 * live range would have to extend past the copy is the address itself, and its
 * only use IS the copy.
 */

#include "common.h"

void func_80055D84(s16);

void func_80055CF4(void) {
    u8 *b = (u8 *) D_800847D0;
    s32 i;
    u32 f;
    u32 m;

    f = *(u32 *) (b + 4) & 0xFF000000;
    for (i = 0; i < 4; i++) {
        m = (0x1000000 << i) & 0xF000000;
        if ((f & m) != 0) {
            func_80055D84((s16) i);
            *(u32 *) (b + 4) = *(u32 *) (b + 4) & ~m;
            break;
        }
    }
}
