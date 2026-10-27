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
 * Both insns are dependency-free and both birth a single-set pseudo, so they
 * tie on sched1 priority and the order is decided by LUID, i.e. RTL order.
 * That means the target's parameter-5 load is LATE in its RTL -- sched1 then
 * groups it with its own prologue save, which is a late insn -- while this
 * source's comes from assign_parms at the very top. Why the original's is late
 * when the value still ends up in a callee-saved register is the open
 * question; nothing tried moves it, and the committed prototype
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
