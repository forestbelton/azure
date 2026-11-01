#include "common.h"

/* 53 words and exactly ONE differing word: the target fills func_80025358's jal delay slot with a DEAD 'lui $a0, 0x8001' (overwritten by the next instruction's 'lw $a0, 0x28($s2)'), where every spelling here fills it with a second copy of 'addiu $a0, $s2, -0x20'. Both are reorg picking the next instruction in the stream; the target's stream had the 0x80010000 materialisation there and this one's had the fail-path argument. Four measured originally: the constant in a local against inline at all three uses, and 'p - 8' hoisted into a local (54w/50, much worse).

   RE-MEASURED 2026-11-01, seven more spellings, every one 53w/1: the constant
   re-assigned to the same local after the func_80025358 call, a second local
   assigned there, inline at all three uses, and four spellings of the 'p - 8'
   sites ('(s32)p - 0x20' in the fail path, in the call, in both, and
   '(u8 *)p - 0x20'). Note that word 19's 'addiu $a0, $s2, -0x20' is dead in
   BOTH versions -- 'p - 8' is hoisted and discarded there either way -- so the
   difference really is only WHICH dead insn sched1 leaves next to the call for
   reorg to take. That is the scheduler's priority order, which the source does
   not reach; see the single-set-pseudo note in docs/compiler-version.md. Everything else matches, including the goto structure -- the two failure exits really do converge on one block and then on the shared tail.

   Parked by tools/decomp/park.py; the live comment is in csrc/ovl_main_80020000.c, which also carries the
   declarations this needs. */

void func_80025398(s32 *p)
{
    u8 *b = (u8 *)0x80010000;
    s32 i;
    s32 off;

    if (func_80021FF0(p[11], b, 0) == 0)
    {
        goto fail;
    }
    func_80025358(p - 8, p[10]);
    if (func_80021F18(p[10], b) == 0)
    {
        goto fail;
    }
    func_8002519C(b);
    func_800220DC();
    off = 0;
    for (i = 0; i < 5; i++)
    {
        func_80024298((s32 *)p[off + 1]);
        off++;
    }
    func_80024FFC(p);
    goto done;
fail:
    p[13] = (s32)func_80024FFC;
    func_800230F4((s32)(p - 8));
    p[-4] = (s32)func_80024F7C;
done:
    func_80020984();
    p[16] = 0;
}
