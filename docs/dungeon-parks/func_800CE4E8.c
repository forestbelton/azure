/* func_800CE4E8 (ovl_dungeon) -- PARK, best so far.

   target 109 words, candidate 109 words, 14 differing (masked);
   aligndiff: 109 built / 109 target, 12 ALIGNED DIFFERENCES.

   WHAT IT IS.  The 0x202-style grid spawner for func_800CE028's node: a 5x5
   sweep that fills the view's +0x04 row of negated func_800BCA68 heights and
   the +0x36 row of 0..3 random jitters, then the usual
   func_800419EC(0x20, 8) / D_800E296C flag / D_80083460+0xA counter close.
   Parameters 3 and 4 (`ang`, the record pointer) are never read; the fifth is
   the 0/1 the two call sites at :34604 and :34620 pass.

   THREE MEASURED LEVERS, worth 1, 2 and 3 words:

   1. THE CELL ADDRESS IS INTEGER ARITHMETIC WITH THE CONSTANT ON THE INDEX:
      `(u8 *)(i * 2 + 4 + (s32)r)`.  Any pointer spelling (`r + 4 + i * 2`,
      `r + (i * 2 + 4)`, `&r[4 + i * 2]`) lets loop.c fold the `4` into a giv
      on `i * 2` -- `li $s3, 4` in the inner preheader plus `addiu $s3, $s3, 2`
      in the body -- which is 110 words and ALSO costs the register that keeps
      the node in `$fp`, so the node is spilled to the stack as well
      (110 words, 37 aligned).  With the `4` written on the INDEX side
      fold swaps it onto `r`, loop.c sees no profit in reducing a bare
      `i * 2`, and the target's three separate `addiu`/`sll`/`addu` appear.
      `(i << 1) + 4 + (s32)r` measures identically (109/12); the four other
      association orders are 106 or 110.

   2. THE CALL RESULT MUST BE ITS OWN `s16` LOCAL, NOT INLINE IN THE STORE.
      Inline -- `*(s16 *)c = -func_800BCA68(a, b);` -- the address block is
      scheduled to the TOP of the inner loop instead of between the two calls
      (109 words, 21 aligned).  Split into `v = func_800BCA68(a, b); c = ...;
      *(s16 *)c = -v;` it lands where the target has it.  `s32 v` costs the
      two words of a sign extension the `s16` return does not need
      (111 words, 16 aligned); `s16 v` is 109 and 12.

   3. THE TWO HEIGHT ARGUMENTS ARE `* 64` ON A SIGNED LOAD.  Written inline in
      the call, combine narrows `lh $a0, 0x0($s3)` to `lhu`; with `a` and `b`
      as their own `s32` statements the loads stay `lh` and the `andi 0xFFC0`
      (the u16 promotion of func_800BCA68's parameters) is unchanged.

   ALSO SETTLED, each by measurement:
     * `s16 t = f;` -- a genuine local, spilled to 0x10($sp) exactly as the
       target spills it, with the sign-extended copy in $s7 for the loop test.
       `u16 t` is 106 words / 59 aligned.
     * the final test is written `== 0` first: `!= 0` first inverts the
       `bnez` and costs the arm order.
     * `*(s16 *)p = x - 2;` really does emit `sh $v0, 0x20($fp)` off the node.
     * `r = q;` inside the outer loop is real: using `q` directly at the cell
       address is 107 words / 57 aligned.
     * head statement order is a wash -- all 90 orders of
       {t, p, q, handler install, x store, y store} that respect the
       dependences measure 12, 15 or 16, and 42 of them measure 12.

   THE RESIDUE, 12 aligned rows, is two placements and the renames they drag:
     a) the `t` spill `sh $s0, 0x10($sp)` is the FIRST insn of the block after
        the null check in the target -- so reorg takes it for the `beqz`'s
        delay slot.  Here it is scheduled seventh and `addiu $s3, $fp, 0x20`
        (the `p` local) fills the slot instead.  No source order reaches it.
     b) the target computes `addiu $a2, $s6, 0x4` BEFORE `sll $v1, $s2, 1`;
        this candidate computes them the other way round, so the two scratch
        registers are swapped and the `addu` reads them in the other order.
        The sum's operand order is already right (index first).
   Everything else in the function is word- and register-exact.

   Permuter note: the word counts match and every difference is a register
   number or a two-insn order, which docs/dungeon-sweep.md's triage rule calls
   NOT permuter food.  .greg/.sched work is the route. */
s32 func_800CE4E8(s32 x, s32 y, s32 ang, u8 *e, s32 f)
{
    UnkNode *n;
    u8 *p;
    u8 *q;
    u8 *r;
    u8 *u;
    u8 *c;
    u8 *g;
    s16 t;
    s32 w;
    s32 a;
    s32 b;
    s16 v;
    s32 i;
    s32 j;

    n = func_8003FC64(2);
    if (n != NULL)
    {
        t = f;
        p = (u8 *)n + 0x20;
        q = p;
        n->unk_10 = (u32)func_800CE028;
        *(s16 *)p = x - 2;
        *(s16 *)(p + 0x2) = y - 2;
        for (j = 0; j < 5; j++)
        {
            r = q;
            u = q + 0x36;
            for (i = 0; i < 5; i++)
            {
                a = (i + *(s16 *)p) * 64;
                b = (j + *(s16 *)(p + 0x2)) * 64;
                v = func_800BCA68(a, b);
                c = (u8 *)(i * 2 + 4 + (s32)r);
                *(s16 *)c = -v;
                w = (u16)func_800A6DA4(1, 3) << 5;
                *(s16 *)u = w;
                if (t != 0)
                {
                    *(s16 *)u = -w;
                }
                *(s16 *)u = *(u16 *)u + *(u16 *)c;
                u += 2;
            }
            q += 0xA;
        }
        *(s16 *)(p + 0x6C) = 0x10;
        *(s16 *)(p + 0x68) = 0x20;
        *(s16 *)(p + 0x6E) = t;
        func_800419EC(0x20, 8);
        if (*(s16 *)(p + 0x6E) == 0)
        {
            D_800E296C |= 0x80080000;
        }
        else
        {
            D_800E296C |= 0x40080000;
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
    return (s32)n;
}
