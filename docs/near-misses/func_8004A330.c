/* func_8004A330 (unk_1C540) -- 70 of 77 words, seven words short. NOT compiled.
 *
 * Eight parameters. Allocates a node, allocates a buffer with func_8004B404,
 * and on success fills the +0x20 sub-object and hands six of its arguments to
 * func_8004A24C (landed, so its `s16` arguments 4-6 are what produce the
 * sll/sra pairs at the call site); on failure it sets bit 15 of the node's
 * +0x1E, ors 0x8000 into D_800814A0 and returns 0.
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
 * THE SEVENTH WORD is `m`: the target materialises `addiu $s1,$s0,0x20` and
 * uses `0x8($s1)`, `0x4($s1)`, `0x6($s1)`; this source writes `u8 *m = n +
 * 0x20;` and gcc folds the 0x20 into each displacement off the node instead,
 * which is the "a rebased pointer must not fold" entry in carved-sweep seen
 * from the losing side.
 */

#include "common.h"
void func_8004A24C(u8 *, u32, u8 *, s16, s16, s16);
s32 func_8004491C(UnkNode *, u32);
void func_8004CAA0(void);
void func_80049F2C(u8 *);

s32 func_8004A330(u32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, u8 *h) {
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
