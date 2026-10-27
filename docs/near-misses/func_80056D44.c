/* func_80056D44 (unk_1C540) -- 26 of 28 words, 27 DIFF. NOT compiled.
 *
 * Written out of the `func_80056DB4 + func_800563B0 + func_80056D44` row on
 * 2026-10-27. Same recipe as func_80056DB4 (read that file's header for the
 * addressing), but the three field values are COPIED from the second
 * argument: +0x60 to +0x3A, +0x64 to +0x3C, +0x68 to +0x24.
 *
 * WHAT IS LEFT: TWO WORDS, and they are NOPS -- which is the useful thing to
 * know about it. The listing is
 *
 *     lw  $v0, 0x0($a0) ; nop ; sw $v0, %lo(D_80084918)($a2)
 *     lhu $v0, 0x60($a1) ; nop ; sh $v0, 0x3A($v1)
 *     lhu $v0, 0x64($a1) ; nop ; sh $v0, 0x3C($v1)
 *     lw  $v0, 0x68($a1) ; addu $a0, $v1, $zero ; jal ; sw $v0, 0x24($v1)
 *
 * -- three load-delay slots, of which the target fills exactly ONE, with the
 * call's argument. So the target had nothing else schedulable between the
 * copies, where this source has two more insns available and fills all three
 * slots. The instruction count excluding nops is already right.
 *
 * Measured and not working: each copy through a `u32 v` local, the lookup
 * inline in the +0 store rather than a local, and the field reads as
 * ARRAY_REFs on `(u16 *)`/`(u32 *)` casts (24 words -- worse, the index
 * folding costs two more). The thing to look for is what our build has spare:
 * read the sched1 dump for the block and see which two insns it moves into
 * the slots, then find the spelling that pins them. Note that both sides'
 * loads and stores are bare pointer derefs, so MEM_IN_STRUCT_P is 0 for both
 * and `true_dependence`'s differing-flags exception does NOT apply -- the
 * reordering is allowed because `memrefs_conflict_p` sees two different
 * pseudo bases, not because of the flags.
 */

#include "common.h"

void func_8005F134(u8 *);

void func_80056D44(s32 i, u8 *e) {
    u8 *q = D_80084918;
    u32 h = D_80073740[i];

    *(u32 *) (q + 4) = 0x60100;
    *(u32 *) D_80084918 = h;
    *(s16 *) (q + 0x3A) = *(u16 *) (e + 0x60);
    *(s16 *) (q + 0x3C) = *(u16 *) (e + 0x64);
    *(u32 *) (q + 0x24) = *(u32 *) (e + 0x68);
    func_8005F134(q);
}
