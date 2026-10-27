/* func_8003719C (unk_72A8) -- 126 of 126 words, 6 DIFF. NOT compiled.
 *
 * The six-parameter member of the func_80036B10 family: allocates with
 * func_8003FF2C, installs func_80037394 at +0x10 and func_80033D54 through
 * func_8004491C, fills the +0x20 sub-object, copies six sprite templates with
 * func_80036C7C and tail-calls func_80037394. Built from func_80036B10's
 * template, which got it to 126/126 with 15 DIFF on the first compile.
 *
 * WHAT IS PAID FOR
 *
 * `t = D_80082B90;` goes AFTER the first func_80036C7C call, not before it.
 * The listing materialises that address between the first and second calls --
 * it is the second call's third argument AND the value stored to `q + 8` at
 * the end, so it lives in $s0 across all six -- and placing the local above
 * the first call puts its `lui`/`addiu` one call earlier. Nine words, 15 DIFF
 * to 6.
 *
 * WHAT IS LEFT: THE STACK-PARAMETER PROLOGUE INTERLEAVE, third instance
 *
 * All six remaining words are in the entry block, and they are three insns
 * placed differently among the two parameter save/load pairs:
 *
 *     target               this
 *     addu  $a1, $v0       addu  $a1, $v0
 *     addiu $a2, 0x49      sw    $fp, 0x30($sp)
 *     sw    $fp, 0x30($sp) lw    $fp, 0x48($sp)
 *     lw    $fp, 0x48($sp) addiu $a2, 0x49
 *     lui   $a3, %hi(...)  sw    $s5, 0x24($sp)
 *     sw    $s5, 0x24($sp) lw    $s5, 0x4C($sp)
 *     lw    $s5, 0x4C($sp) lui   $a3, %hi(...)
 *     addiu $a3, %lo(...)  addiu $a3, %lo(...)
 *
 * The target alternates strictly -- setup, pair, setup, pair, setup -- and
 * this one emits both pairs before catching up on the setup. Same difference
 * as func_80036B10 (3 DIFF, one stack parameter) and func_80036D4C (6 DIFF,
 * three), so it is one phenomenon in three functions.
 *
 * THE func_80036B10 HEADER'S HYPOTHESIS IS REFUTED. It reads: "the target's
 * parameter-5 load is LATE in its RTL ... while this source's comes from
 * assign_parms at the very top", with the order decided by sched's LUID
 * tie-break. The RTL order is not free:
 *
 *   * `thread_prologue_and_epilogue_insns` (function.c:5522) emits the
 *     prologue with `emit_insn_after (seq, f)` -- immediately after the
 *     function's first NOTE -- so every `sw $sX, N($sp)` is ahead of the
 *     whole body in the insn chain, whatever the source said.
 *   * It runs at toplev.c:3103, after reload and BEFORE sched2 at 3105, and
 *     -O2 turns both schedulers on (toplev.c:3397). So the pass that
 *     interleaves saves with body insns is sched2, reading LUIDs assigned
 *     over a chain that already has the prologue at the top.
 *   * assign_parms' copies, including the two `lw`s from the incoming slots,
 *     are emitted at function start, so they precede the rest of the body.
 *
 * So in BOTH builds the LUID order is fixed: every prologue save, then the
 * parameter copies, then the call's argument setup. The target's
 * `addiu $a2, 0x49` ahead of `sw $fp` therefore cannot be an LUID tie-break
 * won the other way -- it has to be a priority or ready-list difference, and
 * priority here is a path length through the whole function, since this
 * function has no branches at all and is a single basic block from `addiu $sp`
 * to the final `jal`. That is the thing to compute next, not another source
 * permutation.
 *
 * Measured and NOT working, all 126/126 and 6 DIFF: parameter 6 as `u8 *`
 * rather than `u32`; func_8003FF2C's count parameter as `s32` rather than
 * `s16`; parameter 1 as `UnkNode *` with no cast at the call; and the fourth
 * argument hoisted into a `UnkNode **` local assigned before the call, which
 * was the direct test of "give the `lui` a lower LUID" and moved nothing.
 */

#include "common.h"

extern UnkNode *D_80082BC0;
extern u32 D_8006A988[];
extern u32 D_8006A994[];
extern u32 D_8006A9A0[];
extern u32 D_8006A9AC[];
extern u32 D_8006A9B8[];
extern u32 D_8006A9C4[];
extern u8 D_80082B80[];
extern u8 D_80082B90[];
extern u8 D_80082BA0[];
extern u8 D_80082BB0[];
extern u8 D_80081E90[];
extern u8 D_80081EA0[];

UnkNode *func_8003FF2C(s32, UnkNode *, s16, UnkNode **);
s32 func_8004491C(UnkNode *, u32);
void func_80033C1C(u8 *, u32);
void func_800350B0(u8 *, u8 *);
void func_80033D54(void);
void func_80036C7C(u8 *, u32 *, u8 *);
void func_80037394(u8 *, u32, u32);

void func_8003719C(u8 *a, u8 *b, u8 *c, s16 d, s32 e, u32 f)
{
    u8 *n;
    u8 *m;
    u8 *q;
    u8 *v;
    u8 *t;

    n = (u8 *)func_8003FF2C(0x11, (UnkNode *)a, 0x49, &D_80082BC0);
    *(u32 *)(n + 0x10) = (u32)func_80037394;
    func_8004491C((UnkNode *)n, (u32)func_80033D54);
    m = n + 0x20;
    v = *(u8 **)(n + 0x8);
    q = *(u8 **)(n + 0xC);
    *(u32 *)(m + 0x44) = (u32)c;
    m[0x48] = 1;
    *(s16 *)(m + 0x4A) = d;
    func_80033C1C(b, *(u32 *)c);
    *(u32 *)(n + 0x20) = (u32)b;
    *(u32 *)(b + 0x60) = 6;
    *(u32 *)(*(u8 **)(n + 0x20) + 0x10) = 0x47;
    *(u32 *)(*(u8 **)(n + 0x20) + 0x14) = 2;
    *(u32 *)(m + 0x4) = 0;
    *(u32 *)(m + 0x8) = 0;
    func_800350B0((u8 *)f, m);
    *(u32 *)(q + 0xC) = 0xC0C0C0;
    func_80036C7C(*(u8 **)(m + 0x44) + 0xC, D_8006A988, D_80082B80);
    t = D_80082B90;
    func_80036C7C(*(u8 **)(m + 0x44) + 0xC, D_8006A994, t);
    func_80036C7C(*(u8 **)(m + 0x44) + 0xC, D_8006A9A0, D_80082BA0);
    func_80036C7C(*(u8 **)(m + 0x44) + 0xC, D_8006A9AC, D_80082BB0);
    func_80036C7C(*(u8 **)(m + 0x44) + 0xC, D_8006A9B8, D_80081E90);
    func_80036C7C(*(u8 **)(m + 0x44) + 0xC, D_8006A9C4, D_80081EA0);
    *(u32 *)(q + 0x8) = (u32)t;
    *(u32 *)(q + 0x0) = 0;
    q[0x4] = 0;
    q[0x5] = 0;
    *(u16 *)(q + 0x14) |= 0x1C;
    *(s16 *)(m + 0x66) = e;
    func_80037394(m, (u32)v, (u32)q);
}
