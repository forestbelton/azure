/* func_800CE4E8 (ovl_dungeon) -- PARK, best so far.

   target 109 words, candidate 109 words, 3 differing (masked);
   aligndiff: 109 built / 109 target, 4 ALIGNED DIFFERENCES.
   Was 14/12 before j48, 11/9 before j56, 109 / 8 aligned before j62.

   WHAT IT IS.  The 0x202-style grid spawner for func_800CE028's node: a 5x5
   sweep that fills the view's +0x04 row of negated func_800BCA68 heights and
   the +0x36 row of 0..3 random jitters, then the usual
   func_800419EC(0x20, 8) / D_800E296C flag / D_80083460+0xA counter close.
   Parameters 3 and 4 (`ang`, the record pointer) are never read; the fifth is
   the 0/1 the two call sites at :34604 and :34620 pass.


   -------------------------------------------------- WHAT j56 ADDED: 9 -> 8

   `t = f;' IS NOT INSIDE THE `if (n != NULL)' AT ALL.  It goes between
   `n = func_8003FC64(2);' and the `if', so the `s16' local's spill
   `sh $s0, 0x10($sp)' is emitted in the ENTRY block, before the `beqz'.
   reorg's `fill_simple_delay_slots' then takes it straight out of the block
   it is already in, which is what the target does -- the old header read the
   target as reorg reaching into the fall-through, and that reading is what
   made residue (a) look unreachable.  Worth 4 of the 9 rows, at the same 109
   words.  Before the CALL instead (`t = f; n = func_8003FC64(2);') is
   109 / 12: the spill is then above the `jal' and $s0 has to be set up
   earlier.

   j56 also re-ran the head-statement sweep on top of that hoist -- the five
   remaining head statements in the orders p,q,H,X,Y / P,H,X,Y,Q /
   P,H,Q,X,Y and the `j = 0' hoist out of the outer `for' header are ALL
   109 / 8, byte-identical; P,X,Y,H,Q / P,Q,X,Y,H / P,X,Y,Q,H are 109 / 14.
   The 8-plateau is as wide as the 9-plateau was.

   Three devices on `t', all WORSE and all for the same reason -- an "m"
   constraint forces the local into memory and deletes the `$s7'
   sign-extended copy the loop test needs:
     `__asm__("" ::"m"(t));' after `t = f'      107 words / 44 aligned
     `__asm__("" : "=m"(t) : "m"(t));'          107 / 44
     the same before `func_800419EC'            108 / 47
     the same before `*(s16 *)(p + 0x6E) = t'   107 / 47
     the same before the outer `for'            107 / 54
     `__asm__("" ::"r"(t));' after `t = f'      110 / 10
   And a bare `__asm__("");' after `t = f' is 110 / 10: it DOES put the spill
   first in the block, but reorg then has nothing it may use before the
   branch and `fill_eager_delay_slots' duplicates the branch target's
   `addu $v0, $fp, $zero' into the delay slot instead -- one word over.

   Four more cell-address spellings, against lever 1 below:
     `4 + (s32)r + i * 2'        110 words / 35 aligned
     `i * 2 + ((s32)r + 4)'      110 / 35
     `(s32)r + (4 + i * 2)'      109 / 29
     `4 + i * 2 + (s32)r'        109 / 8  -- on the plateau, byte-identical
   So the rule holds exactly as written: the giv fold fires iff the `+ 4'
   term is associated onto `r' before the index is added.

   THE RESIDUE IS NOW 8 ROWS, and only two facts are left:
     a) `p' and `q' (`addiu $s3, $fp, 0x20' / `addu $s5, $s3, $zero') come
        FIRST in this block and should come after the `t' sign-extension and
        the `j = 0' -- the target's order is spill, `addu $s4, $zero, $zero',
        `sll'/`sra', `p', `q'.  4 rows.  Nothing in the head-order sweep
        moves them; they are the two insns with the smallest LUIDs that have
        dependents, and sched2 picks them last in its backward pass.
     b) residue (b) below, unchanged, 3 rows.

   ------------------------------------------------- WHAT j48 ADDED: 12 -> 9

   THE INNER LOOP'S `i = 0' IS HOISTED OUT OF THE `for' HEADER, ABOVE
   `r = q;'.  Worth 3 of the 12 rows, and the lever is sched1's tie-break.

       for (j = 0; j < 5; j++)
       {
           i = 0;                     <- here, NOT in the for header
           r = q;
           u = q + 0x36;
           for (; i < 5; i++)

   The target's outer-loop head is `addu $s2, $zero, $zero' (i = 0) /
   `addu $s6, $s5, $zero' (r = q) / `addiu $s0, $s5, 0x36' (u).  The three are
   independent, so sched.c's `rank_for_schedule' finds equal INSN_PRIORITY and
   falls through to `INSN_LUID (tmp) - INSN_LUID (tmp2)' -- ORIGINAL SOURCE
   ORDER.  Written as `for (i = 0; ...)' the initialisation is emitted after
   `r = q' and `u = q + 0x36' and the three come out in the other order.

   This is the same class of lever as the opening block's statement order but
   it is NOT reachable from it: the 90-order sweep below is over the SIX
   opening statements and never touches a `for' header's own init.  Hoisting
   `j = 0' the same way is WORSE (13 rows alone, 10 combined with the `i'
   hoist), so it is the inner counter specifically.

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
      association orders are 106 or 110.  j48 added four more, all 110 words:
      `(s32)r + 4 + i * 2' (37 aligned), `(s32)(r + 4) + i * 2',
      `i * 2 + (s32)(r + 4)' and `(s32)(r + 4) + (i << 1)' (35 each).  Every
      spelling that puts the `+ 4' term FIRST re-triggers the giv fold, which
      is exactly what residue (b) below would need.

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
       dependences measure 12, 15 or 16, and 42 of them measure 12.  j48
       RE-RAN that whole 90-order sweep on top of the `i = 0' hoist: they now
       measure 9 (42 orders), 11 (24) or 15 (24), and the 9-plateau is the
       same 42 orders.  Head order is exhausted at 9.

   THE RESIDUE, 9 aligned rows, is two placements and the renames they drag:
     a) the `t` spill `sh $s0, 0x10($sp)` is the FIRST insn of the block after
        the null check in the target -- so reorg takes it for the `beqz`'s
        delay slot.  Here it is scheduled seventh and `addiu $s3, $fp, 0x20`
        (the `p` local) fills the slot instead.  5 rows.  No source order
        reaches it: the sign-extension `sll $v0,$s0,16 / sra $s7,$v0,16` reads
        the PARAMETER `$s0` and not the copy, so the copy has no dependent in
        the block, INSN_PRIORITY 1, and nothing in the 90-order sweep moves it.
     b) the target computes `addiu $a2, $s6, 0x4` BEFORE `sll $v1, $s2, 1`;
        this candidate computes them the other way round, so the two scratch
        registers are swapped and the `addu` reads them in the other order.
        4 rows.  The sum's operand order is already right (index first) in
        BOTH -- `(plus (mult i 2) (plus r 4))' -- so only the emission order of
        the two independent operands differs, which is again LUID.  expand_expr
        always evaluates op0 first, so the fix would be a source spelling whose
        op0 is `(s32)r + 4'; every such spelling re-triggers lever 1's giv
        fold and costs a word (see the four 110s above).
   Everything else in the function is word- and register-exact.

   Permuter note: the word counts match and every difference is a register
   number or a two-insn order, which docs/dungeon-sweep.md's triage rule calls
   NOT permuter food.  .greg/.sched work is the route.

   -------------------------------------------------- WHAT j62 ADDED: 8 -> 4

   RESIDUE (a) IS CLOSED.  TWO statements at the TOP of the `if' body, IN
   THIS ORDER, put the block's first five insns in the target's order:

       if (n != NULL)
       {
           j = 0;             <- hoisted out of the outer `for' header
           s = t;             <- NEW `s32 s'; the loop test becomes `s != 0'
           p = (u8 *)n + 0x20;
           q = p;
           ...

   Measured, all 109 words:
     j = 0 and s = t, in that order      109 /  4   <- this candidate
     s = t and j = 0, in that order      109 /  6
     s = t alone, `for (j = 0; ...)'     109 /  7
     j = 0 alone, no `s', `if (t != 0)'  109 /  8   (the j56 candidate's score
                                                     with different rows: p/q
                                                     land right, sll/sra move
                                                     down to where p/q were)

   WHY NEITHER HALF ALONE IS ENOUGH, and why no statement order could ever
   have reached this.  The `sll $v0,$s0,16 / sra $s7,$v0,16' in the old
   candidate was NOT an ordinary source insn: it was loop.c's INVARIANT HOIST
   of `(s16)t' for the inner `if (t != 0)'.  `move_movables' emits a hoisted
   insn immediately before `loop_start' -- i.e. AFTER everything already in
   the preheader block -- so the sign extension was pinned to the END of that
   block no matter how the six head statements were permuted.  That is why
   j48's and j56's 90-order sweeps both bottomed out: the insn they needed to
   move was not one of the statements they were moving.  Writing `s32 s = t;'
   as a real statement makes the sll/sra an ordinary pair with an ordinary
   LUID, and sched2 (every insn in this block is priority 1, so
   `rank_for_schedule' falls straight through to `INSN_LUID') then leaves the
   block in source order.  `j = 0' has to be hoisted as well because the
   target's first insn in the block is `addu $s4, $zero, $zero'; inside the
   `for' header it is emitted after the two stores.

   NOTE FOR THE MERGE: `s' is a new `s32' local and the inner test is now
   `if (s != 0)', not `if (t != 0)'.  `t' is still the `s16' that is spilled
   to 0x10($sp) and stored to +0x6E -- do not collapse the two.

   -------------------------------------- RESIDUE (b), 4 rows, UNCHANGED

   candidate                         target
     sll   $a2, $s2, 1                 addiu $a2, $s6, 0x4
     addiu $v1, $s6, 4                 sll   $v1, $s2, 1
     addu  $s1, $a2, $v1               addu  $s1, $v1, $a2

   j62 read the dumps for this one and the diagnosis is now exact, which is
   worth more than the four rows:

   * THE RTL IS IDENTICAL IN BOTH.  The `addu' is `(plus <sll result>
     <addiu result>)' either way -- candidate `addu $s1,$a2,$v1' has
     $a2 = sll, target `addu $s1,$v1,$a2' has $v1 = sll.  So the tree is the
     same and `fold' is not the lever.
   * ONLY THE EMISSION ORDER OF THE TWO PRODUCERS DIFFERS, and the hard
     registers follow it: in BOTH streams the first-emitted producer gets
     $a2 and the second gets $v1.  One source fact has to flip the order of
     two insns whose RTL must stay as it is.
   * IT IS NOT A sched TIE THAT SOURCE ORDER CAN REACH.  In the sched1 dump
     (tools/decomp/sched.py <src> func_800CE4E8 --sched1, basic block 3) the
     two insns are 130 (`sll') and 132 (`addiu'), BOTH bumped to priority
     0x7f000001 by `adjust_priority'/`birthing_insn_p' when the `addu'
     releases them, so `rank_for_schedule' ties on priority, ties on class
     (both ALU, `insn_cost' 1), and decides on INSN_LUID -- i.e. on the order
     `expand_expr' emitted them.  Un-bumping either one with a second set
     does NOT help: the other three insns in that ready group are the call's
     `li $a0,1' / `li $a1,3' at priority 2 with HIGHER LUIDs, so an un-bumped
     producer sorts below them and comes out before the `li' pair, which the
     target does not do.  Worked through in both directions; neither reaches
     the target order.
   * AND `expand_expr' ALWAYS EMITS op0 FIRST (expr.c, the `binop:' label:
     two separate statements, op0 then op1), so the only way to emit the
     `addiu' first is to make `(s32)r + 4' op0 -- and then the `addu' comes
     out `(plus <addiu> <sll>)', which is the wrong operand order AND
     re-triggers lever 1's giv fold.  `expand_binop''s commutative swap
     (optabs.c:412) cannot rescue it: it fires only when op1 is a REG and op0
     is NOT, or op0 is a CONST_INT, or `target == op1'; both operands here
     are pseudos.  (The `target == op1' clause IS reachable -- it is what
     makes `c = (u8 *)(i * 2 + (s32)c);' come out `(plus c <sll>)' -- but it
     needs op1 to be a bare decl that is also the destination, which cannot
     be arranged for the operand we need swapped.)

   SO: either a pass between expand and reload moves the `addiu' up, or the
   original is not this expression.  Everything source-level that j62 could
   see has been tried.

   New spellings j62 measured and rejected (all on top of the new 4-row
   candidate, so these supersede nothing in lever 1 -- they confirm it):
     `(u8 *)((s32)r + 4) + i * 2'          110 words / 36 aligned  (giv fold)
     `(u8 *)&((s16 *)r)[i + 2]'            110 / 36  (giv fold)
     the address inlined at BOTH uses, no
       `c' variable at all                 106 / 59  (cse+loop reduce the
                                                      whole address to a giv
                                                      and a register is freed,
                                                      so the frame shrinks too)
     `c = (u8 *)((s32)r + 4);' then
       `c = (u8 *)(i * 2 + (s32)c);'       109 / 16  -- the split DOES get the
       `addiu' emitted first, but loop.c hoists it into the inner preheader
       and the `addu' comes out `(plus c <sll>)' via the `target == op1'
       swap, so the operand order is wrong in the other direction.

   Permuter note: unchanged in kind but much smaller -- 109/109 words with
   exactly three differing words, all of them one transposed producer pair
   and the two register numbers that follow it.  docs/dungeon-sweep.md's
   triage rule calls that NOT permuter food, but at three words it is cheap
   to try, and no hand spelling j48, j56 or j62 found moves it.
*/
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
    s32 s;
    s32 w;
    s32 a;
    s32 b;
    s16 v;
    s32 i;
    s32 j;

    n = func_8003FC64(2);
    t = f;
    if (n != NULL)
    {
        j = 0;
        s = t;
        p = (u8 *)n + 0x20;
        q = p;
        n->unk_10 = (u32)func_800CE028;
        *(s16 *)p = x - 2;
        *(s16 *)(p + 0x2) = y - 2;
        for (; j < 5; j++)
        {
            i = 0;
            r = q;
            u = q + 0x36;
            for (; i < 5; i++)
            {
                a = (i + *(s16 *)p) * 64;
                b = (j + *(s16 *)(p + 0x2)) * 64;
                v = func_800BCA68(a, b);
                c = (u8 *)(i * 2 + 4 + (s32)r);
                *(s16 *)c = -v;
                w = (u16)func_800A6DA4(1, 3) << 5;
                *(s16 *)u = w;
                if (s != 0)
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
