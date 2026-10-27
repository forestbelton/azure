/* func_80036D4C (unk_72A8) -- 98 of 98 words, 6 DIFF. NOT compiled.
 *
 * The six-argument sibling of func_80036B10: allocates through func_8003FF2C,
 * installs func_80036F24 at +0x10 and func_80033D54 via func_8004491C, fills
 * the +0x20 sub-object, runs func_80036C7C three times over the
 * D_8006A958/964/970 and D_80082B50/B60/B70 pairs, and finishes by or-ing 0x1C
 * into the +0x14 halfword of the +0xC object.
 *
 * THE WHOLE BODY MATCHES -- 80 consecutive words -- and every differing word
 * is in the prologue. See the STACK-PARAMETER PROLOGUE INTERLEAVE cluster in
 * docs/carved-sweep.md: the target emits each call-argument setup insn BEFORE
 * the save/load pair for the stack parameter that follows it, and this source
 * emits it after. Both insns birth a single-set pseudo so they tie on sched1
 * priority and LUID decides, which puts the question on where the parameter
 * load sits in RTL. func_80036B10 is the same thing with one argument instead
 * of three (3 DIFF against this one's 6).
 *
 * Nothing else is outstanding: `(u32) D_80082B60` being stored at +0x8 of the
 * +0xC object reuses the address the second func_80036C7C call materialised,
 * which cse does on its own.
 */

#include "common.h"
extern UnkNode *D_80082BC0;
extern u32 D_8006A958[];
extern u32 D_8006A964[];
extern u32 D_8006A970[];
extern u8 D_80082B50[];
extern u8 D_80082B60[];
extern u8 D_80082B70[];

UnkNode *func_8003FF2C(s32, UnkNode *, s16, UnkNode **);
s32 func_8004491C(UnkNode *, u32);
void func_80033C1C(u8 *, u32);
void func_800350B0(u8 *, u8 *);
void func_80036C7C(u8 *, u32 *, u8 *);
void func_80033D54(void);
void func_80036F24(u8 *, u32, u32);

void func_80036D4C(u8 *p, s32 b, s32 c, s32 d, s32 e, u32 f) {
    u8 *n;
    u8 *m;
    u8 *q;

    n = (u8 *) func_8003FF2C(0x11, (UnkNode *) p, 0x49, &D_80082BC0);
    *(u32 *) (n + 0x10) = (u32) func_80036F24;
    func_8004491C((UnkNode *) n, (u32) func_80033D54);
    m = n + 0x20;
    q = *(u8 **) (n + 0xC);
    *(u32 *) (m + 0x44) = c;
    m[0x48] = 1;
    *(s16 *) (m + 0x4A) = d;
    func_80033C1C((u8 *) b, *(u32 *) c);
    *(u32 *) (n + 0x20) = b;
    *(u32 *) (b + 0x60) = 6;
    *(u32 *) (*(u8 **) (n + 0x20) + 0x10) = 0x47;
    *(u32 *) (*(u8 **) (n + 0x20) + 0x14) = 2;
    *(u32 *) (m + 0x4) = 0;
    *(u32 *) (m + 0x8) = 0;
    func_800350B0((u8 *) f, m);
    *(u32 *) (q + 0xC) = 0xC0C0C0;
    func_80036C7C(*(u8 **) (m + 0x44) + 0xC, D_8006A958, D_80082B50);
    func_80036C7C(*(u8 **) (m + 0x44) + 0xC, D_8006A964, D_80082B60);
    func_80036C7C(*(u8 **) (m + 0x44) + 0xC, D_8006A970, D_80082B70);
    *(u32 *) (q + 0x8) = (u32) D_80082B60;
    *(u32 *) (q + 0x0) = 0;
    q[4] = 0;
    q[5] = 0;
    *(s16 *) (q + 0x14) = *(u16 *) (q + 0x14) | 0x1C;
    *(s16 *) (m + 0x66) = e;
}
