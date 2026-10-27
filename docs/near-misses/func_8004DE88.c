/* func_8004DE88 (unk_1C540) -- 64 of 65 words, 59 DIFF. NOT compiled.
 *
 * Builds a command buffer in D_80083E18: a leading 0x08, the argument string
 * via strcpy, then 0x19, 0x02, 0x1A, then the ADDRESS of D_80083D88 and that
 * address plus four, each written out as four separate bytes (an unaligned
 * 32-bit store done by hand). It then dispatches through
 * D_800714A4[D_80081550] -- the idiom func_8004DCF4 above it already uses --
 * and initialises D_80083D88[0] and [1].
 *
 * `strrchr(buf, 0)` is how it finds the end of the string; strcpy and strrchr
 * are not declared anywhere in include/, so they need hand prototypes beside
 * the function (structs.h declares memcpy the same way).
 *
 * THE MISSING WORD is `addu $a0,$v0,$zero`, a copy of strrchr's return value.
 * The target keeps the cursor in $a0 and the three byte constants plus the
 * D_80083D88 address value in $v0; this source keeps the cursor in $v0 (the
 * return register, so the copy is deleted) and the constants in $v1. So the
 * question is only which quantity wins $v0, and the candidate is local-alloc's
 * SUGGESTION pass: a quantity born from a copy of a hard register is assigned
 * that register before the priority loop runs, and the cursor is born from a
 * copy of $v0. For the target's allocation, that suggestion has to have been
 * blocked -- i.e. the cursor conflicts with $v0 -- which would be the case if
 * the address value in $v0 is live across the cursor's whole range. Reading
 * THE MECHANISM IS NOW READ, NOT GUESSED (local-alloc.c, 2026-10-26; the tree
 * is at scratch/gcc-src). `block_alloc` allocates in two phases and the first
 * ignores priority entirely: every quantity that has a SUGGESTED hard register
 * -- recorded by `combine_regs` for either side of a move whose other side is
 * a hard register -- is allocated first, with
 * `find_free_reg (..., just_try_suggested=1)`. Only what is left is sorted by
 * the priority formula.
 *
 * So the cursor, born from `(set (reg cursor) (reg $v0))` as a plain move of
 * strrchr's return value, carries a `qty_phys_copy_sugg` of $v0 and is
 * allocated in phase 1. The address value is born from an `addiu` and has no
 * suggestion at all, so it waits for phase 2 and cannot compete. **The
 * 0.714-against-0.688 priority margin recorded here before was the wrong
 * story**: the two quantities are not in the same phase, and nothing done to
 * their reference counts will change that.
 *
 * PHASE 1 HAS ONE ESCAPE, AND IT IS NOT THIS FUNCTION'S ANSWER -- recorded
 * because it was briefly written up as if it were. `find_free_reg` is called
 * in the suggestion phase with `accept_call_clobbered = 0`, and it starts its
 * `used` set from `call_used_reg_set` whenever `qty_n_calls_crossed[qty] > 0`.
 * $v0 is call-clobbered, so a quantity with a $v0 suggestion that CROSSES A
 * CALL is refused it in phase 1, falls through to phase 2, and its copy
 * survives. That is a real lever and `advise.py --greg` prints `crosses N
 * calls` for it. But CHECKED AGAINST THE LISTING: the target's cursor is born
 * at `addu $a0,$v0,$zero` right after strrchr and dies at `sb $v0,0x3($a0)`
 * (8004DF34), with no `jal` in between -- the dispatching `jalr` is after it.
 * So the target's cursor crosses nothing either, and the escape cannot be why
 * phase 1 declined to give it $v0.
 *
 * SO THE QUESTION IS NARROWER THAN IT LOOKS, and worth stating precisely for
 * whoever picks it up: the target's cursor is a block-local quantity born from
 * a plain move of a hard register, which gives it a `qty_phys_copy_sugg` of
 * $v0; nothing in its live range holds $v0 (combine_regs deliberately
 * terminates the hard register's lifespan at the copy, for exactly this
 * reason); it crosses no call; and yet it came out in $a0 with the copy
 * intact, which is what phase 2 allocation looks like. Something kept it out
 * of phase 1 or out of $v0, and none of the three mechanisms read so far
 * accounts for it. The remaining candidates are `reg_qty < 0` -- the pseudo
 * not being local to one basic block, which would hand it to global-alloc and
 * suppress the suggestion entirely -- and something in combine_regs' earlier
 * tying logic refusing the pair.
 *
 * MEASURED 2026-10-26 with
 * `advise.py --greg`, which now prints every local-alloc quantity because this
 * function is a single basic block and global-alloc therefore reports nothing:
 * the two quantities competing for $v0 are pseudo 81, the cursor, at 20 refs
 * over 28 insns (0.714), and pseudo 82, the address value, at 11 over 16
 * (0.688). The cursor wins by that margin and keeps the $v0 it is born in as a
 * copy of strrchr's return, so the copy is deleted; the target's allocation is
 * the other way round.
 *
 * SPLITTING THE CURSOR to lower its reference count does NOT do it: writing
 * the second four-byte group through a separate `q = p + 4` takes the cursor
 * to 16 refs over 22 insns, which is 0.727 -- HIGHER, because the live range
 * shrank with it -- gcc coalesces q back into p anyway, and the word count is
 * unchanged at 64. To flip the contest the cursor has to lose references
 * without losing live range, or the address value has to gain them, and no
 * spelling found does either at zero instruction cost.
 *
 * MEASURED AND NOT WORKING: a separate `e` variable with `p = e;` (62 words --
 * gcc coalesces both and loses two more); the three stores as `p[0] = c; p++;`
 * instead of `*p++ = c;` (64, unchanged); and computing the end pointer as
 * `&D_80083E18[1] + strlen(s)` instead of with strrchr (68).
 *
 * SINCE PARKED, 2026-10-27: 64 of 65 words to 65 of 65 and 47 DIFF, by
 * declaring `a` `s16` rather than `u32`. Found by search.py's
 * declaration-width family in the sweep of all forty parks. advise.py now
 * reads SHAPE with a SIGNEDNESS row, so check the signedness of every local
 * in the expression it names before anything else -- the width change may
 * have moved the problem rather than fixed it.
 *
 * A SWEEP RESULT WAS REVERTED HERE, 2026-10-27: search.py's
 * declaration-width family proposed `s16 a` and it scores better, but it is
 * SEMANTICALLY WRONG -- a holds the 32-bit address of D_80083D88 and is split into four bytes with >> 8, >> 16 and >> 24; at s16 the top two shifts are dead.
 * The score is an oracle for SHAPE, not for correctness, and the width
 * family is the one that can change VALUES rather than representations.
 * See docs/matching-levers.md on reviewing a narrowing against the
 * listing's own arithmetic before keeping it.
 */

#include "common.h"
extern u8 D_80083E18[];
extern u32 D_80083D88[];

char *strcpy(char *, const char *);
char *strrchr(const char *, s32);

void func_8004DE88(u8 *s) {
    u8 *p;
    u32 a;

    D_80083E18[0] = 8;
    strcpy((char *) &D_80083E18[1], (char *) s);
    p = (u8 *) strrchr((char *) D_80083E18, 0);
    *p++ = 0x19;
    *p++ = 2;
    *p++ = 0x1A;
    a = (u32) D_80083D88;
    p[0] = a;
    p[1] = a >> 8;
    p[2] = a >> 16;
    p[3] = a >> 24;
    p += 4;
    a += 4;
    p[0] = a;
    p[1] = a >> 8;
    p[2] = a >> 16;
    p[3] = a >> 24;
    ((void (*)(u8 *)) D_800714A4[D_80081550])(D_80083E18);
    D_80083D88[0] = 0x2300030C;
    D_80083D88[1] = 0x2300040C;
}
