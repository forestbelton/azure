/* func_80036B10 (unk_72A8) -- 77 of 77 words, 3 DIFF. NOT compiled.
 *
 * Allocates a node with func_8003FF2C, installs func_80036C44 at +0x10 and
 * func_80033D54 through func_8004491C, fills the +0x20 sub-object, and ends
 * with func_800350B0 and func_8003DB94.
 *
 * ONE INSTRUCTION IS ONE SLOT OUT and that is the whole difference: the target
 * emits `lui $a3, %hi(D_80082BC0)` BEFORE the pair that saves $s5 and loads
 * parameter 5 from 0x40($sp); this source emits it after them. Everything
 * else, including the triple indirection through c+0x74 and the D_8006A934
 * halfword lookup, matches.
 *
 * THIS IS A CLUSTER, not a one-off: func_80036D4C is the same difference with
 * three stack parameters instead of one (6 DIFF). See the
 * STACK-PARAMETER PROLOGUE INTERLEAVE section of docs/carved-sweep.md.
 *
 * THE RTL-ORDER EXPLANATION BELOW IS REFUTED. It read: "the target's
 * parameter-5 load is LATE in its RTL ... while this source's comes from
 * assign_parms at the very top", with the order decided by sched's LUID
 * tie-break. The RTL order is not free. `thread_prologue_and_epilogue_insns`
 * (function.c:5522) emits the prologue with `emit_insn_after (seq, f)`, right
 * after the function's first NOTE, so every `sw $sX, N($sp)` precedes the
 * whole body whatever the source said; it runs at toplev.c:3103, after reload
 * and before sched2 at 3105, and -O2 enables both schedulers
 * (toplev.c:3397). assign_parms' copies are emitted at function start, ahead
 * of the rest of the body. So in both builds the LUID order is fixed --
 * prologue saves, then parameter copies, then the call's argument setup -- and
 * the difference has to be a priority or ready-list one. See
 * docs/near-misses/func_8003719C.c, the third instance, for the full
 * derivation and what to compute next.
 *
 * The old wording follows for the record: both insns are dependency-free and
 * both birth a single-set pseudo, so they tie on sched1 priority and the order
 * is decided by LUID. Nothing tried moves it, and the committed prototype
 * `void func_80036B10(u8 *, u8 *, u8 *, s32, u32)` plus its landed caller
 * func_80036210 pin parameter 5's type, so the narrow-parameter lever from
 * matching-levers section 7 is not available here without touching both.
 */

#include "common.h"
extern UnkNode *D_80082BC0;
extern s16 D_8006A934[];
extern u8 D_80073DC4[];

UnkNode *func_8003FF2C(s32, UnkNode *, s16, UnkNode **);
s32 func_8004491C(UnkNode *, u32);
void func_80033C1C(u8 *, u32);
void func_800350B0(u8 *, u8 *);
void func_8003DB94(u8 *, u32, s32);
void func_80033D54(void);
void func_80036C44(s32, s32, s32);

void func_80036B10(u8 *a, u8 *b, u8 *c, s32 d, u32 e) {
    u8 *n;
    u8 *m;
    u8 *q;

    n = (u8 *) func_8003FF2C(0x11, (UnkNode *) a, 0x49, &D_80082BC0);
    *(u32 *) (n + 0x10) = (u32) func_80036C44;
    func_8004491C((UnkNode *) n, (u32) func_80033D54);
    m = n + 0x20;
    q = *(u8 **) (n + 0xC);
    *(u32 *) (m + 0x44) = (u32) c;
    m[0x48] = 1;
    *(s16 *) (m + 0x4A) = d;
    func_80033C1C(b, *(u32 *) c);
    *(u32 *) (n + 0x20) = (u32) b;
    *(u32 *) (b + 0x60) = 6;
    *(u32 *) (*(u8 **) (n + 0x20) + 0x10) = 0x10C;
    *(u32 *) (*(u8 **) (n + 0x20) + 0x14) = D_8006A934[*(u8 *) (*(u8 **) (*(u8 **) (c + 0x74) + 0x80) + 2)];
    *(u32 *) (m + 0x4) = 0;
    *(u32 *) (m + 0x8) = 0;
    func_800350B0((u8 *) e, m);
    *(u32 *) (q + 0xC) = 0x808080;
    func_8003DB94(q, (u32) D_80073DC4, 0);
}
