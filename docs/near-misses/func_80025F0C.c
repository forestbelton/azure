#include "common.h"

/* 61 words against 60, 30 DIFF. UPDATED 2026-11-01 -- three structural
   corrections, which is why the body here no longer matches the 57-word
   version the older note described.

   1. 0x80010208 and 0x8001022C are SYMBOLS, not constant addresses. The
      target reads them as `lui` + a load at `%lo(...)`, which is the
      symbolic form; a constant base would give `lui $v0, 0x8001` with the
      0x208 in the load's displacement and no relocation. ovlverify's mask
      cannot see the difference (the immediate is the masked field) but the
      LINK can, so this matters for `make check` even though the DIFF count
      does not move.
   2. The 0x80010208 test does NOT guard two calls. The target's arms set only
      $a1 -- `lw $a1, 4($s2)` against `lw $a1, 8($s2)` -- and join on ONE
      `jal func_80069E38` at word 39, so the source selects the string into a
      local and makes a single call.
   3. `p + 4` is recomputed after that join (`addiu $s0, $s3, 0x4` in the
      shared call's delay slot) and the parameter `p` is kept in a fourth
      callee-saved register, $s3, purely so it can be. That is where three of
      the words the 57-word version was missing come from.

   What is left is one word and a register swap: the target holds `p + 4` in
   $s0 and `n` in $s1, every spelling here has them the other way round, and
   mine carries one extra `nop` where the target fills the `beqz`'s delay slot
   with the shared call's `$a0` setup. Measured for the swap: `s` declared
   before `buf`, `n` copied to a local, `n + 1` computed into a local early
   (61w/30, the best), and `t` declared before `s` -- 61w/32 otherwise. The
   older measurements still stand for the shapes they describe: `p + 4`
   written inline at all eleven call sites, and the D_80028294 base hoisted
   for indices 1..3 or for all four.

   The body: strcpy from D_80028294[0], then func_8003AD08(n + 1) into a stack
   buffer and a chain of strcats with D_80028294[3] twice, the 0x8001020C
   literal (that one IS a constant -- `lui` + `ori`), D_80028294[1] or [2] on
   the D_80010208 halfword, D_800282A0, and finally func_8003AD08 of
   D_8001022C. */

/* Declarations this park needs; they live in csrc/ovl_main_80020000.c.
   Repeated here so the park compiles STANDALONE, which is what
   tools/decomp/permimport.py needs -- without them a permrun reports a
   silent `base=?` and searches nothing. */
extern u8 *D_80028294[];
extern u8 *D_800282A0;
extern s16 D_80010208;
extern s32 D_8001022C;

void func_80025F0C(u8 *p, s32 n)
{
    u8 buf[0x10];
    s32 m = n + 1;
    u8 *s = p + 4;
    u8 *t;

    func_80069E78(s, D_80028294[0]);
    func_8003AD08(m, buf);
    func_80069E38(s, buf);
    func_80069E38(s, D_80028294[3]);
    func_80069E38(s, (u8 *)0x8001020C);
    func_80069E38(s, D_80028294[3]);
    if (D_80010208 != 0)
    {
        t = D_80028294[1];
    }
    else
    {
        t = D_80028294[2];
    }
    func_80069E38(s, t);
    s = p + 4;
    func_80069E38(s, D_800282A0);
    func_8003AD08(D_8001022C, buf);
    func_80069E38(s, buf);
}
