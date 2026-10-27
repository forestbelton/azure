/* func_8004A330 (unk_1C540) -- 74 of 77 words, 74 DIFF. NOT compiled.
 *
 * Eight parameters. Allocates a node, allocates a buffer with func_8004B404,
 * and on success fills the +0x20 sub-object and hands six of its arguments to
 * func_8004A24C (landed, so its `s16` arguments 4-6 are what produce the
 * sll/sra pairs at the call site); on failure it sets bit 15 of the node's
 * +0x1E, ors 0x8000 into D_800814A0 and returns 0.
 *
 * NOW 74 OF 77. Four of the seven words came from declaring arguments 5 and 6
 * `s16` rather than `s32`, which is advise.py's CALLEE-SAVED row applied
 * literally and the same lever that landed func_8004A24C: as `s32` the
 * parameter's pseudo is a plain copy of its incoming MEM and
 * `update_equiv_regs` substitutes it back, so the `lw` sinks to the single
 * use and no register is needed; as `s16` the entry insn is a PROMOTION, the
 * substitution cannot fire, and both values live in callee-saved registers
 * across the two calls with the sign-extension reappearing at the use. The
 * listing's `lw $s3, 0x50($sp)` and `lw $s5, 0x54($sp)` at ENTRY are the
 * signature, against this source's loads at the use. `d` stays `s32`: it is a
 * register argument and has to be saved either way.
 *
 * The original note, kept because its reasoning about the substitution is
 * what pointed at the fix:
 *
 * SIX OF THE SEVEN WORDS ARE THREE CALLEE-SAVED REGISTERS -- the target keeps
 * eight values in s0..s7 and this source five:
 *
 *   kept by both   arguments 1-4 (register arguments, so they must be saved)
 *                  and the node
 *   kept only by   argument 5 and argument 6, the two STACK parameters, and
 *   the target     `m`, the node + 0x20
 *
 * The two stack parameters are the update_equiv_regs substitution, and the
 * condition is now read rather than guessed (see matching-levers section 7):
 * `reg_n_refs == 2 && reg_basic_block < 0`, i.e. the set plus exactly one use,
 * with the pseudo spanning more than one basic block. Both parameters are
 * loaded at entry in block 0 and used once inside the `if`, which is precisely
 * that shape, so both loads sink to the use and no register is needed.
 * MEASURED: copying them into locals at the top does NOT block it -- the copy
 * is tied to the parameter and the combined quantity still spans blocks.
 *
 * What WOULD block it is a second reference, or confining the references to
 * one block -- but hoisting `d - 0xA0` and `e - 0x80` out of the `if` puts
 * their `addiu`s before the branch where the target has them after it, so that
 * trade needs checking rather than assuming.
 *
 * THE REMAINING THREE WORDS are all `m`, and the earlier reading of them was
 * wrong: the 0x20 does NOT fold into the displacements here -- `m` is a
 * VAR_DECL, so the tree is `m + 8` and `fold` has nothing to combine, and
 * this source does materialise it and store through it. What it does not do
 * is spend a CALLEE-SAVED register on it. The listing has
 * `addiu $s1, $s0, 0x20` and then `addu $a0, $s1, $zero` for the call, eight
 * values in s0..s7; this source puts `m` straight into $a0 -- which it
 * prefers, being func_8004A24C's first argument -- and spends one insn
 * instead of two, so the save/restore pair and the copy are the three words.
 *
 * `m` crosses no call (it is computed after func_8004B404 returns and dies at
 * the call's argument setup), so nothing forces a callee-saved register on
 * it; what the listing needs is for $a0 to be unavailable, i.e. a
 * lower-priority conflicting allocno that prefers it -- matching-levers'
 * `regs_someone_prefers` entry -- and no such allocno exists in this
 * source's RTL. MEASURED and NOT working: hoisting `d - 0xA0` and
 * `e - 0x80` into locals before the `if` (70 words, no change -- so the
 * substitution was never about where the subtraction sits).
 *
 * SINCE PARKED, 2026-10-27: 74 of 77 words and 74 DIFF to 76 of 77 and 43,
 * by declaring the local `r` `s16` rather than `s32` -- the same lever as
 * arguments 5 and 6 above, applied to the result accumulator. Found by
 * search.py's declaration-width family in the sweep of all forty parks.
 * ONE word and 43 differing words left, where it was three and 74.
 *
 * THE FRAME, read with advise.py's census 2026-10-27: the listing's frame is
 * 0x40 and this source's 0x38, and the whole 8 bytes is TWO MORE SAVED
 * CALLEE-SAVED REGISTERS. The incoming arguments move with it, which is why
 * the two builds reload DIFFERENT ones: with eight parameters, arguments 5
 * to 8 are all on the stack, and the listing keeps 5 and 6 in registers and
 * reloads 7 and 8 while this source does the opposite. So the one word left
 * is not a missing instruction anywhere -- it is the register pressure being
 * two registers lower here than in the listing, and the thing to look for is
 * whatever makes two more values live across the calls.
 *
 * A SWEEP RESULT WAS REVERTED HERE, 2026-10-27: search.py's
 * declaration-width family proposed `s16 r` and it scores better, but it is
 * SEMANTICALLY WRONG -- r takes func_8004B404's return value, and that function is declared `u32 func_8004B404(s32)` and used elsewhere as `(u8 *) func_8004B404(0x104)` -- it is an allocator returning a pointer, which an s16 truncates before `*(u32 *) (m + 8) = r` stores it.
 * The score is an oracle for SHAPE, not for correctness, and the width
 * family is the one that can change VALUES rather than representations.
 * See docs/matching-levers.md on reviewing a narrowing against the
 * listing's own arithmetic before keeping it.
 */

#include "common.h"
void func_8004A24C(u8 *, u32, u8 *, s16, s16, s16);
s32 func_8004491C(UnkNode *, u32);
void func_8004CAA0(void);
void func_80049F2C(u8 *);

s32 func_8004A330(u32 a, s32 b, s32 c, s32 d, s16 e, s16 f, s32 g, u8 *h) {
    u8 *n;
    u8 *m;
    s32 r;

    n = (u8 *) func_8003FC64(0);
    if (n == 0) {
        return 0;
    }
    r = func_8004B404(0);
    m = n + 0x20;
    *(u32 *) (m + 0x8) = r;
    if (r != 0) {
        *(s16 *) (n + 0x20) = g;
        *(s16 *) (m + 0x4) = d - 0xA0;
        *(s16 *) (m + 0x6) = e - 0x80;
        func_8004A24C(m, a, h, b, c, f);
        *(u32 *) (n + 0xC) = (u32) (n + 0x50);
        func_8004491C((UnkNode *) n, (u32) func_8004CAA0);
        *(u32 *) (n + 0x10) = (u32) func_80049F2C;
    } else {
        *(s16 *) (n + 0x1E) = *(u16 *) (n + 0x1E) | 0x8000;
        D_800814A0 |= 0x8000;
        n = 0;
    }
    return (s32) n;
}
