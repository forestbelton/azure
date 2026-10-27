/* func_80035E58 (unk_72A8) -- 129 of 129 words, 2 DIFF. NOT compiled.
 *
 * The six-argument spawn: allocates with func_8003FF2C, installs
 * func_8003605C at the node's +0x10 and func_80033D54 through func_8004491C,
 * fills the +0x20 sub-object, clears the caller's RECT, and sets five fields
 * from the D_8006A8FC..D_8006A92C tables indexed by the actor type byte.
 *
 * `e` is the actor at `node + 0x20` and `g` is the node's +0x0C record, both
 * named by func_8003605C's own body (`d = p - 0x20`). `q = f + 0x8C` is the
 * sub-struct func_80037D9C builds, and `*(u8 **)(q + 0x80)` is the owner
 * pointer it stores there -- which is why each of the five table lookups
 * re-reads BOTH the +0x80 pointer and its +2 byte: the stores between them
 * flush cse.
 *
 * WHAT IS PAID FOR -- 8 DIFF on the first compile, 2 now
 *
 * * **D_80082BC0 is an incomplete ARRAY passed by name, not `&` on a scalar
 *   pointer.** Six of the eight words. Both spellings emit the same
 *   `lui`/`addiu` for func_8003FF2C's fourth argument, but as `&D_80082BC0`
 *   the pair lands after both parameter save/load pairs in the entry block
 *   and as `D_80082BC0[]` it lands between them, where the listing has it;
 *   it also pulls `addiu $a2, $zero, 0x49` one slot earlier. This is the
 *   whole "stack-parameter prologue interleave" cluster -- func_80036B10,
 *   func_80036D4C and func_8003719C all MATCH on the same change -- and the
 *   rule is now matching-levers section 6.
 *
 * * `*(u32 *)((u8 *)n + 0x20) = (u32)b;` and the final
 *   `*(u32 *)(*(u32 *)((u8 *)n + 0x20) + 0x34)` go through the NODE, while
 *   everything else in that object goes through `e`. The listing's
 *   `sw $s6, 0x20($s2)` and `lw $v1, 0x20($s2)` name $s2, not $s0, so the
 *   two spellings of one address are both in the source.
 *
 * WHAT IS LEFT: 2 words, and they are ONE swap.
 *
 * func_80036988's two argument setups land in each other's slots: the target
 * puts `addu $a1, $s1, $zero` in the D_8006A924 group's load-delay slot and
 * `addiu $a0, $s2, 0x2C` in the D_8006A92C group's, this build the reverse.
 * They are insns 197 and 199 in the sched2 dump, both at priority 3, and
 * `rank_for_schedule`'s last key is `INSN_LUID(y) - INSN_LUID(x)` -- a
 * DESCENDING sort, so the higher LUID is taken first in the backward pass and
 * ends up LATER in the stream. Since expand_call's register-load loop
 * (calls.c:1850) runs forward over the arguments, $a0's setup always has the
 * lower LUID, and a0-later is unreachable through argument order.
 *
 * Two keys are left above LUID: priority, and the dependence CLASS relative
 * to the last-scheduled insn. Raising $a0's priority needs one more edge on
 * its path from the block start, which means one more computation feeding it;
 * `e + 0xC` does NOT provide one, because fold rewrites it straight back to
 * `n + 0x2C` (measured -- same instruction, same 2 DIFF).
 *
 * Also measured and not working: `np = (u8 *)n` for the +0x20/+0x2C bases;
 * all three declaration orders of e/g/q; `f += 0x8C` in place of a `q` local
 * (12 DIFF); the +0x74 store moved after the lookups (27); `f + 0x8C` inline
 * at the call; dropping func_80036988's prototype; a `(u32)` cast on the
 * table index; and the call moved ahead of the third `sh` (11).
 */

#include "common.h"

extern u16 D_8006A91C[];
extern u16 D_8006A924[];
extern u16 D_8006A92C[];
extern u32 D_8006A8FC[];
extern s16 D_8006A914[];
extern u8 D_80073AAC[];
extern u8 D_8006AA3C[];
extern UnkNode *D_80082BC0[];

UnkNode *func_8003FF2C(s32, UnkNode *, s16, UnkNode **);
s32 func_8004491C(UnkNode *, u32);
void func_80033C1C(u8 *, u32);
void func_800375C0(u8 *, u8 *);
void func_800350B0(u8 *, u8 *);
void func_80036988(u8 *, u8 *);
void func_80033D54(void);
void func_8003605C(u8 *, u8 *, s32);
void func_80037714(void);
void func_80036110(u8 *);

void func_80035E58(UnkNode *a, u8 *b, u8 *c, u32 *d, s16 i, u8 *f)
{
    UnkNode *n;
    u8 *e;
    u8 *g;
    u8 *q;

    n = func_8003FF2C(0x11, a, 0x49, D_80082BC0);
    n->unk_10 = (u32)func_8003605C;
    func_8004491C(n, (u32)func_80033D54);
    e = (u8 *)n + 0x20;
    g = (u8 *)n->unk_0C;
    *(u32 *)(e + 0x44) = (u32)d;
    e[0x48] = 1;
    *(s16 *)(e + 0x4A) = i;
    ClearImage((RECT *)(c + 4), 0, 0, 0);
    func_80033C1C(b, *d);
    *(u32 *)((u8 *)n + 0x20) = (u32)b;
    *(u32 *)(e + 4) = (u32)c;
    *(u32 *)(e + 8) = (u32)func_80037714;
    func_800375C0(e, D_8006AA3C);
    func_800350B0(f, e);
    *(u32 *)(e + 0x68) = (u32)func_80036110;
    *(u32 *)(g + 0xC) = 0x808080;
    *(u32 *)(g + 8) = (u32)D_80073AAC;
    *(u32 *)g = 0;
    g[4] = 0;
    g[5] = 0;
    q = f + 0x8C;
    *(u32 *)(e + 0x74) = (u32)q;
    *(s16 *)(e + 0x12) = D_8006A91C[(*(u8 **)(q + 0x80))[2]];
    *(s16 *)(e + 0x2E) = D_8006A924[(*(u8 **)(q + 0x80))[2]];
    *(s16 *)(e + 0x36) = D_8006A92C[(*(u8 **)(q + 0x80))[2]];
    func_80036988((u8 *)n + 0x2C, q);
    *(u32 *)(g + 8) = D_8006A8FC[(*(u8 **)(q + 0x80))[2]];
    *(u32 *)(*(u32 *)((u8 *)n + 0x20) + 0x34) = D_8006A914[(*(u8 **)(q + 0x80))[2]];
}
