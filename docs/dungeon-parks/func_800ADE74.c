/* ===========================================================================
   BEST MEASURED CANDIDATE for func_800ADE74, ovl_dungeon (1817 words).

   2026-11-21, later session.   1821/1817 words, 170 aligned rows
   was 2026-11-21 morning:      1817/1817 words, 255 aligned rows
   was 2026-11-16 (j36):        1826/1817 words, 256 aligned rows

   Measured with, and both agree:
       python3 tools/decomp/parkresidue.py func_800ADE74        (real unit)
       python3 tools/decomp/onefunc.py func_800ADE74 --src <copy> --out one
       python3 tools/decomp/aligndiff.py one/ovl_dungeon.c func_800ADE74

   READ THIS FIRST: THE +4 WORDS ARE NOT A REGRESSION.  The morning's
   1817/1817 was TWO CANCELLING ERRORS -- see "THE WORD COUNT WAS A LIE".
   The honest residue is much smaller than either row count suggests:

     95 rows   pure BRANCH-DISPLACEMENT noise.  The built and target words
               are the same opcode and the same registers and differ only in
               the displacement, because our build is 4 words long overall
               and 6 words short at target index 108, so every branch to the
               shared tails (.L800AF89C / .L800AF8A0 / .L800AF8A4 /
               .L800AFAA4) and every branch across a hole misses.  Each of
               these rows is (dest_index - src_index) off by exactly the net
               word error between the branch and its target, so every one of
               them becomes equal as soon as the +/- clusters below are
               closed -- they are consequences, not independent defects.
               Nearly all of them are gated on residue (1) alone, which is
               +8 words sitting between almost every branch and almost every
               tail.  See "HOW TO COUNT THEM" below.
     35 rows   real register differences (R with different register names)
     10 rows   X, one instruction against another
     17 rows   words we emit that the target does not
     13 rows   words the target has that we do not

   ===========================================================================
   BLOCK ORDER: CHECKED AND CORRECT.

   Mapping the emitted stream against the listing's addresses with
   `aligndiff.py -v', difflib finds ONE monotone alignment that covers
   ~1790 of 1817 words.  There is no transposed chunk left anywhere: the
   22 switch arms are emitted in the listing's address order (0x19, 0x16,
   0x2C, 0x2B, 0x22, 0x29, 0x28, 0x1A, 0x23, 0x26, 0x27, 0x20, 0x24, 0x25,
   0x18, 0x2A, 0x1B, 9/0xA, 0x17, 0x1C, 0x1D, 0x15), and so are the four
   arms of the first switch and the trailing code.  So the source order of
   the arms is a FIXED parameter -- do not permute it looking for the
   cross-jump in residue (1) below.

   ONE block order defect WAS found and fixed, worth 9 rows: see change (2).

   There is no frame-size cluster.  The prologue, the epilogue and every
   stack displacement match: 0x50 frame, $s0-$s7/$fp/$ra saved at
   0x28..0x4C, sp18/sp1A/sp1C/sp1E/sp20 at 0x18/0x1A/0x1C/0x1E/0x20,
   outgoing arg 5 at 0x10($sp).  Every `lhu/sh ..($sp)' row in the diff is a
   register-name row, not a displacement row.

   ===========================================================================
   THE WORD COUNT WAS A LIE: 1817/1817 WAS TWO CANCELLING ERRORS

   The morning session reached 1817/1817 with `s32 t1 -> s16'.  That
   narrowing DELETES A BASIC BLOCK THE TARGET HAS.  The target's listing
   contains, at 800ADF90..800ADFAC:

       lui  $v0, (0x10000 >> 16)
       lhu  $v1, 0x24($s5)
       nop
       slt  $v0, $v0, $v1
       beqz $v0, .L800ADFAC
       nop
       ori  $v1, $zero, 0xFFFF
       sh   $v1, 0x24($s5)

   -- the `if (t1 > 0x10000) t1 = 0xFFFF;' guard, emitted in full.  With
   `s16 t1' the guard is NOT emitted at all, because `shorten_compare'
   folds it at TREE level:

       c-common.c:1960  shorten_compare ()
       c-common.c:2031  "If comparing an integer against a constant more
                         bits wide, maybe we can deduce a value of 1 or 0
                         independent of the data."
       c-common.c:2049  if (!real1 && !real2
                            && TREE_CODE (primop1) == INTEGER_CST
                            && TYPE_PRECISION (TREE_TYPE (primop0))
                               < TYPE_PRECISION (*restype_ptr))
       c-common.c:2108  else if (code == GT_EXPR)
                          { if (min_gt)  val = boolean_true_node;
                            if (!max_gt) val = boolean_false_node; }

   For `s16 t1', TYPE_PRECISION(s16)=16 < 32, TYPE_MAX_VALUE(s16)=32767 is
   not > 0x10000, so `!max_gt' holds and the comparison becomes
   boolean_false_node: guard and body both vanish, 6 words gone.  For
   `s32 t1' the precision test fails (32 < 32 is false), nothing folds, and
   the compare is emitted exactly as the target has it.

   The morning header's claim -- "its guard is DEAD IN BOTH WIDTHS ... so the
   bits are identical either way" -- is true about the RUNTIME VALUE and
   false about the CODE, which is the only thing being matched.

   SO THE DEAD GUARD IS POSITIVE EVIDENCE ABOUT THE DECLARED WIDTH.  A
   comparison that appears in the target's listing but can never be true
   proves its operand is 32-bit in the original, because at any narrower
   width cc1psx would not have emitted it.  `s32 t1' is restored and is not
   negotiable.

   TWO MORE WIDTH NARROWINGS HAVE THE SAME SHAPE and widthsweep will keep
   offering them, because its objective orders by (word gap, rows) and each
   one reaches 1817 by deleting code:

     s32 t1 -> u8    1817/1817, 209 rows.  Also WRONG ON VALUE: t1 is read
                     from and stored back to *(u16 *)(c + 0x24), so u8
                     would zero the high byte.
     u8 k   -> u32   1817/1817, 214 rows.  Deletes FIVE words the target has
                     at 800AE0B8..800AE0C8 (`andi $v1,$s0,0xFF' twice, the
                     `beqz $s0' and the two `addiu ..,1') -- the u8
                     truncation of `k' on both paths into the switch.
     s32 dx -> u32   1822/1817, 192 rows.  Deletes the `bgez/nop/negu' abs
                     at 800AF1B4..800AF1BC plus the `addu $s0,$v1,$zero'
                     that follows it: `dx < 0' is constant-false for an
                     unsigned.  Same for `dy -> u16/u32', `t3/t5/t7 -> u*',
                     `n/nn -> u*', `r1 -> u32' (whose `r1 >= 0' guard is the
                     target's `bltz $a0, .L800AE008').

   Inverting the function's FINAL test the other way --
       if (*(s8 *)(b + 0x26) >= 0) return 2; return ret << 16 >> 16;
   -- is 1818/1817 and 201 rows, and it is the SAME TRAP: it deletes the
   canonical `.L800AF8F0: ori $v0,0x800B / sh / j / addiu $v0,2' block
   (target 1695..1698) that the target plainly has.  The morning header
   floated this as a "legitimate partial".  It is not.

   RULE FOR THE NEXT AGENT: before believing any candidate that moves the
   word count toward 1817, look at the `-' rows.  If a run of `-' rows is a
   whole basic block (a compare plus its branch plus its body), the
   candidate deleted code the target has and the word count is a
   coincidence.

   ===========================================================================
   WHAT CHANGED THIS SESSION, AND WHAT EACH WAS WORTH

   All measured on the standalone (which agrees with the real unit at every
   step) and quoted as (words, aligned rows).  Start: 1817/255.

   (1) s16 t1 -> s32                              1825 / 244   (-11 rows)
       Restores the dead guard, above.  Costs +8 words, which were ALREADY
       there and were being hidden; it also realigns the whole top of the
       function, so ten displacement rows go with it.

   (2) The function's LAST test, written the other way round:
           if ((s16)r8 >= 0) { c[0x46] = r8 | 0x8000; return 1; }
           c[0x46] = 0x800B; return 2;
       instead of
           if ((s16)r8 < 0)  { c[0x46] = 0x800B; return 2; }
           c[0x46] = r8 | 0x8000; return 1;
                                                  1825 / 235   (-9 rows)
       A BLOCK ORDER fix, not a spelling: the target's listing has
       `bltz $v0, .L800AF8F0' at 800AF8D8, i.e. the r8<0 arm is the
       OUT-OF-LINE one and `return 1' falls through.  Written as above the
       two four-word blocks swap into the target's order and the whole tail
       at 800AF8D8..800AF8FC becomes byte-identical.  It also creates the
       `.L800AF8F0 / .L800AF8F4' entry pair the target has.
       NOTE the other two tests of this family must NOT be inverted: case 0's
       `if ((s16)r2 < 0)' and case 4's `if ((s16)r5 >= 0)' are already right
       (both measured: no change), and case 1's is `< 4', a different test.

   (3) ((s16 *)c)[0x15] = func_800A0818(...) in cases 0, 1 and 2's FIRST
       return-4 block (the D_80082E80 one), all three together
                                                  1823 / 220   (-15 rows)
       This is docs/matching-levers.md's "MEM_IN_STRUCT_P NEEDS A PLUS_EXPR
       ON TOP", and it is the lever that did nearly all the work this
       session.  Verified again here:
         expr.c:5332  case INDIRECT_REF:
         expr.c:5344  if (TREE_CODE (TREE_OPERAND (exp, 0)) == PLUS_EXPR
                          || ... ) MEM_IN_STRUCT_P (temp) = 1;
       `*(T *)(c + K)' puts a NOP_EXPR (the cast) on top of the PLUS_EXPR,
       so MEM_IN_STRUCT_P stays 0; `((T *)c)[K/sizeof(T)]' puts the
       PLUS_EXPR on top, so it is set, at the same address and with no
       struct or typedef invented.  Then
         sched.c:835  true_dependence ()
         sched.c:854      && ! (MEM_IN_STRUCT_P (mem) && rtx_addr_varies_p (mem)
                                && GET_MODE (mem) != QImode
                                && GET_CODE (XEXP (mem, 0)) != AND
                                && ! MEM_IN_STRUCT_P (x) && ! rtx_addr_varies_p (x))
       drops the dependence between the STORE through `c' (varying address)
       and a later -G0 global LOAD (a LO_SUM, non-varying by rtlanal.c:104),
       so sched1 may hoist the load above the store.  output_dependence
       (sched.c:895/904) has the IDENTICAL clause, so the same spelling also
       frees a STORE TO A STACK LOCAL to move across it -- a stack slot is
       (plus virtual_frame_pointer K) at sched1 time and rtlanal.c:101 makes
       the frame pointer non-varying.  Here that lets `lw %lo(D_800814A8)'
       and `addiu $v0,4' float up and the `sh $v0,0x2A($s5)' fall into a
       delay slot, which is exactly the target's schedule.
       THE RELEVANT MEM_IN_STRUCT_P IS THE ONE ON THE **STORE**, not on the
       load -- which is why the morning header's note that the array-ref
       spelling of the LOAD (`((u8 **)(D_800814A8 + 0xAC))[j]') "does not
       reach this store" was looking at the wrong operand.

   (4) ((u16 *)c)[0x23] |= 0x4000;  (the function's FIRST statement)
                                                  1823 / 213   (-7 rows)

   (5) ((u16 *)c)[0x23] = r5 | 0x8000; in case 4
                                                  1823 / 208   (-5 rows)

   (6) The four `c'-relative stores of case 0x24 in array form, together:
           ((u16 *)c)[0x4C] &= 0x7FFF;      (and the |= 0x8000 below it)
           ((s16 *)c)[0x15] = func_800A0818(...);
           ((u16 *)c)[0x54] = sp18;
           ((u16 *)c)[0x55] = sp1A;
                                                  1823 / 199   (-9 rows)
       ALL-OR-NOTHING: the minimal subset that pays is the PAIR
       {((s16 *)c)[0x15], ((u16 *)c)[0x54]} -- 1823/199 on its own.  EITHER
       ONE ALONE is worth nothing (0x15 alone 1824/208, 0x54 alone
       1823/208), and so is every other pair in the arm.  The other two
       stores are free; they are included so the arm reads consistently.

   (7) case 0x22's `g' given its own variable, `g22'
                                                  1823 / 187   (-5 rows)
       MECHANISM, and it is a general one: the target holds the probed
       record in **$a0** here (`lw $a0,0xAC($v0)' / `beqz $a0, .L800AEC38'
       with an EMPTY delay slot), where a shared `g' gets $s0 and needs an
       extra `move a0,s0'.  A callee-saved register is only reachable for an
       allocno that CROSSES A CALL:
         global.c:925  if (accept_call_clobbered)
                         COPY_HARD_REG_SET (used1, call_fixed_reg_set);
         global.c:927  else if (allocno_calls_crossed[allocno] == 0)
                         COPY_HARD_REG_SET (used1, fixed_reg_set);
         global.c:929  else
                         COPY_HARD_REG_SET (used1, call_used_reg_set);
       In case 0x22 `g' dies at the `func_800A2CAC(g)' call, so a variable
       local to that arm crosses no call, `used1' excludes only the fixed
       registers, and find_reg's 0..31 scan hands it $a0 -- the argument
       register, so the copy disappears.  A `g' SHARED with the arms that do
       keep it across a call has allocno_calls_crossed != 0 for the whole
       function and can only ever get $s0.
       Splitting any OTHER arm's `g' is worth nothing or hurts: 0x19, 0x29,
       0x1A, 0x26, 0x27, 0x18, 0x1B all 187; 0x2C 189; 0x17 189; 9/0xA 191;
       0x15 190; 0x1D 194; the trailing k2==0x1E block 199.  Splitting all
       fourteen at once is 217.  Same for `nd' (cases 0/1/2 are 238/238/234,
       0x29 187, 0x1B 204, 9/0xA 193) and for `i'/`j' (every split 188 or
       much worse; cases 0/1/2 are 1846-1869 words).

   (8) ((u16 *)c)[0x23] = 0x8009; at case 0x2A's tail AND case 0x1C's tail,
       the two textually identical tails, both or neither
                                                  1823 / 180   (-7 rows)
       ALL-OR-NOTHING AGAIN: each alone is 187, the pair is 180, and no
       other pair or triple among the nineteen `c[0x46] = 0x8009' sites does
       anything.  (It lands as 192 -> 187 -> 180 depending on the order the
       changes are applied; on the final body it is worth 7.)

   (9) ((u16 *)c)[0x23] = (sp1C + 1) | 0x8000;  at ALL SEVEN sites
                                                  1821 / 172   (-15 rows, -2 words)
       THE BIGGEST SINGLE FIND, and the clearest all-or-nothing group in the
       function.  The minimal paying subset is the THREE sites inside the
       cases 0/1/2 do-while loops (lines with `q1/q3/q5'); adding the three
       straight-line copies and the one in the trailing block is free.
       ANY ONE OF THEM ALONE IS A DISASTER: 1832/204, 1843/211, 1832/216.
       What it buys is the target's interleave at 800AE63C..800AE650,
           lhu $v0, 0x1C($sp) / lw $v1, %lo(D_800814A8)($s6) /
           addiu $v0,$v0,1 / ori $v0,$v0,0x8000 / addu $v1,$s4,$v1 /
           sh $v0,0x46($s5) / lw $v0,0xAC($v1)
       where ours had the global load after the store with two load-delay
       nops.  Same true_dependence mechanism as (3), on the store to
       c + 0x46 this time.

  (10) In case 4, `ret = 0;' AFTER the func_800A03C4 call rather than before
                                                  1821 / 170   (-2 rows)
       The target computes it late: at 800AE79C it has `addu $a0,$s5,$zero'
       and only at 800AE7AC, in the jal's shadow, `addu $fp,$zero,$zero'.
       Found by swapping every pair of ADJACENT same-indent simple
       assignments in the body (42 legal candidates, one minute); it is the
       only one of the 42 that is both legal and an improvement.  The other
       five that score better are all wrong: swapping
       `h2 = *(u16 *)(c + 0x98);' with the store that consumes h2 (two
       sites, 1819/175), swapping `q7 = func_800A3D18(...)' with the store
       of its own result (1815/227), and two that move a `c + 0x2A' store
       across the call that produces its value (1819/223, 1820/174).

   NET: 255 -> 170 rows; 1817 -> 1821 words, of which the +8 at residue (1)
   was present and hidden from the start.

   ===========================================================================
   HOW TO COUNT THEM (the 95 displacement rows)

   aligndiff's `R' rows are two words with the same key but different bytes.
   For a branch that is almost always the DISPLACEMENT only, because our
   build is a different length.  To separate them: for each `R' row, strip
   the branch destination (the last operand, and the `<func+0x..>' comment)
   from both sides and compare what is left.  Equal means the row is pure
   displacement noise.  On this body that is 95 of the 130 `R' rows; the
   other 35 are real register differences and are listed below.

   DO NOT report aligndiff's raw count as the residue of this function while
   the word count is off; it is dominated by displacement noise and moves by
   tens for changes that alter nothing but the length.  Order candidates by
   (word gap, real rows), and check every `-' run for a deleted basic block.

   ===========================================================================
   RESIDUE THAT IS LEFT, BY CLUSTER, LARGEST FIRST

   (1) case 0x1D's nine-instruction tail does not cross-jump into case
       0x26's at .L800AEEFC.      +8 words, 10 rows, and it is what holds
       ~95 displacement rows hostage.

       The target has, at 800AF7CC:
           lbu $v0, 0x13($s0)
           j   .L800AEEFC
           nop
       and .L800AEEFC is a label do_cross_jump planted INSIDE case 0x26, at
       800AEEFC, in front of the nine insns
           beqz $v0, .L800AF89C / addu $a0,$s0,$zero / jal func_80042900 /
           addiu $a1,$zero,0x4 / sll $v0,$v0,16 / bnez $v0,.L800AF8A0 /
           addu $a0,$s7,$zero / j .L800AFA10 / ori $v0,$zero,0x8009
       We emit both copies.  OUR TWO COPIES ARE BYTE-IDENTICAL, including
       the branch destinations: grep our ovl_dungeon.s for the two
       occurrences of `jal func_80042900' followed by `li $5,4' and the two
       blocks are character-for-character the same, so
       rtx_renumbered_equal_p cannot be what fails.

       WHAT IS ACTUALLY GOING ON (read off jump.c, and this corrects the
       morning header's diagnosis).  Both tails originally end
           (set v0 0x8009) (set mem(s5+0x46) v0) (set v0 5) (jump return_label)
       and jump2 processes unconditional jumps at jump.c:2178-2211:
           find_cross_jump (insn, JUMP_LABEL (insn), 1, &newjpos, &newlpos);
           if (INSN_UID (JUMP_LABEL (insn)) < max_uid)
             for (target = jump_chain[INSN_UID (JUMP_LABEL (insn))];
                  target != 0 && newjpos == 0;
                  target = jump_chain[INSN_UID (target)])
               if (target != insn && JUMP_LABEL (target) == JUMP_LABEL (insn) ...)
                 find_cross_jump (insn, target, 2, &newjpos, &newlpos);
       Three gates, and all three are GLOBAL properties of the function, not
       properties of either arm:
         * jump_chain is built ONCE, before the `while (changed)' loop
           (jump.c:249-269), by PREPENDING, so the chain for a label lists
           the jumps to it in REVERSE source order; the first partner that
           matches two insns wins, so the pairing is decided by how many
           `j return_label' there are and in what order.
         * `JUMP_LABEL (target) == JUMP_LABEL (insn)' -- do_cross_jump
           retargets a jump with redirect_jump and does NOT update
           jump_chain for a simplejump (only the RETURN arm at
           jump.c:2776-2781 does).  So once case 0x26's jump has been
           retargeted by its own earlier merge, it is INVISIBLE to case
           0x1D, which is processed later because case 0x26 is earlier in
           the function.
         * `INSN_UID (JUMP_LABEL (insn)) < max_uid' -- a label do_cross_jump
           created during this pass has a UID past max_uid, so once a jump
           points at one the jump_chain loop is skipped entirely and only
           the 1-insn "code before my own label" attempt remains.
       In our build both tails end up as `j <label before the sh>' with
       their own `ori' in the delay slot, i.e. each merged three insns into
       the trailing k2==0x1E block's copy and neither ever saw the other.
       In the target case 0x1D's jump found case 0x26's jump first and
       merged nine.

       SO THE LEVER IS NEITHER ARM, AND IT IS NOT A SPELLING IN EITHER ARM.
       It is the number/order/content of the `return 5' sites that share the
       epilogue label.  The 2026-11-16 permuter run proved the direction by
       DUPLICATING a `c[0x46] = 0x8009; return 5;' body into two arms of a
       test, re-measured at 1818/229 against that body's 1826/256; its own
       spelling (`if (D_80082E80)', an always-true array address) is not
       usable.  I could not find a legitimate source form, and I measured:
         - the array form of each of the 19 `c[0x46] = 0x8009' sites alone,
           and every pair and triple of them: nothing changes the merge;
         - the array form of each of the 10 `c[0x46] = 0x800B' sites alone
           and every pair and triple: nothing;
         - cancelled `r7++; r7--;' pairs and statement moves throughout
           case 0x2A and case 0x24: nothing;
         - rewriting the trailing k2==0x1E block as
               if (c[0xAC] < 2) { if (g == NULL) g = ...; if (g) return 5; }
               else if (g != NULL) { ... return 5; }
           (equivalent, and it does duplicate the block in SOURCE) -- NOT
           measured, because the target's listing at 800AF974..800AFA08 is
           instruction-for-instruction what we already emit, so this would
           have to break that to help.  Someone should still try it.
       THIS IS THE PERMUTER'S JOB.  Re-import before running it: the body
       has moved a long way since the 2026-11-16 import.

   (2) case 0x24's Chebyshev test: a register permutation.   22 rows, -2 words.
       Instruction-for-instruction correct, and the delay slots agree, but
         mx    is $a2 where the target has $s0
         dx    is $a1 where the target has $v1
         (s16)sp1A is $a0/$a3 where the target has $a1
       and the target has a real `addu $s0,$v1,$zero' for `mx = dx;' at the
       join (.L800AF1C0) where we keep mx in dx's register and copy later,
       plus a different split of the two `sll/sra' pairs.

       BEST THEORY, and the dump settles the first half of it.  `mx' must
       be in a CALLEE-SAVED register, and global.c:925-930 (quoted under
       change (7)) says that happens ONLY for an allocno with
       allocno_calls_crossed != 0.  `.lreg' for this body says

           Register 131 used 3 times across 15 insns; 2 bytes;

       -- pseudo 131 is `mx' (HImode, `.greg' says `131 in 6' = $a2, and
       its neighbours 129/130 are `dx' in $a1 and `dy' in $a0) -- and the
       line carries NO `crosses N calls' clause, i.e. zero.  So our `mx'
       cannot reach $s0 at all, whatever its priority, and no allocation
       tie-break or device can change that: nothing in case 0x24 is live
       across a call after `mx = dx'.
       THEREFORE THE ORIGINAL'S `mx' IS A VARIABLE THAT CROSSES A CALL
       SOMEWHERE ELSE IN THE FUNCTION -- one name reused across arms, which
       is already the catalogue's "`reg_n_calls_crossed' IS A PROPERTY OF
       THE C VARIABLE" and "A CALL-CROSSING SHARED LOCAL IN **ONE** ARM SETS
       THE REGISTER CLASS FOR **EVERY** ARM".  I could not find which name.
       Measured and rejected (all on the 1823/187 body):
         mx <-> aa                     no change at all (so `aa' crosses no
                                       call either; they may well be one
                                       variable in the original, and saying
                                       so costs nothing)
         mx -> nn, r1, r6, r8 as s16   no change
         mx -> n as s16  193   mx -> r7 as s16  191   mx -> k2 as u8  192
         mx -> i as s16  421   mx -> j as s16   203   mx -> i2A as s16 203
         mx -> i/j/i2A as s32          200/200/200
         mx -> ang/t3/t5/t7 as s16     198/196/198/198
         s16 mx -> u16                 1822/201
         sp1A <-> mx                   1820/198 and WRONG (mx = sp1A becomes
                                       a no-op and the max is lost)
         all six legal reorderings of {mx = dx; sp18 = dx;} against the dy
                                       block: 187 or 188-189
       stmtsweep-style moves inside case 0x24 report 1817/195, 1818/186 and
       1819/186 -- every one of them moves `sp1A = dy;' or `mx = sp1A;'
       to a point where its right-hand side is not yet computed or is
       immediately overwritten.  They are wrong, not better.

   (3) case 0x2A: a clean two-register swap.          13 rows, 0 words.
       Ours                      target
         &D_80082E80 -> $s0        $s2
         i2A         -> $s0        $s2   (reusing the pointer's register)
         r7          -> $s2        $s0
       and `ang' is $s1, `&D_8006CCE8' $s3, `&D_8006CCD8' $s4 in BOTH, and
       every instruction in the arm is right.  So it is exactly the priority
       order of TWO allocnos, and the dumps make it arithmetic.  From
       tools/decomp/dump.sh on the standalone of THIS body:

         .lreg
           Register 119 used 6 times across 83 insns; crosses 3 calls;
           Register 1342 used 5 times across 44 insns; crosses 2 calls;
           Register 128 used 9 times across 70 insns; crosses 3 calls;
         .greg
           ;; 70 regs to allocate: ... 1342 ... 119 ...
           119 in 18     1342 in 16     128 in 17

       119 is `r7' (the insn setting it follows the `*func_8009FD40' call in
       case 0x2A), 1342 is that arm's `&D_80082E80' (its lo_sum carries
       REG_EQUIV (symbol_ref D_80082E80)), 128 is `ang'.

       THOSE TWO NUMBERS IN THE DUMP LINE ARE EXACTLY allocno_compare'S
       INPUTS, which is what makes this arithmetic and not a guess:
         flow.c:2957  fprintf (file, "\nRegister %d used %d times across %d
                      insns", i, reg_n_refs[i], reg_live_length[i]);
         flow.c:2963  if (reg_n_calls_crossed[i] == 1) ... else if
                      (reg_n_calls_crossed[i]) fprintf (... "%d calls" ...)
                      -- so NO `crosses' clause means ZERO calls crossed
         global.c:420 allocno_n_refs[allocno]      += reg_n_refs[i];
         global.c:422 allocno_live_length[allocno]  = max over the pseudos
       and allocno_compare is

         global.c:585  allocno_compare ()
         global.c:596  pri1 = (((double) (floor_log2 (allocno_n_refs[v1])
                                          * allocno_n_refs[v1])
                                / allocno_live_length[v1])
                               * 10000 * allocno_size[v1]);
         global.c:606  return v1 - v2;      (tie-break: LOWER pseudo first)

       so, dropping the common factor,
         1342:  floor_log2(5) * 5 / 44 = 2*5/44 = 0.227
         119:   floor_log2(6) * 6 / 83 = 2*6/83 = 0.145
       1342 is allocated first and find_reg's 0..31 scan hands it the lowest
       free callee-saved register, $s0; `r7' then gets $s2 because $s1 is
       held by `ang'.  (Both cross calls, so global.c:925-930 restricts both
       to callee-saved.)

       THE TARGET NEEDS 119 FIRST, AND floor_log2 IS A STEP FUNCTION, so
       there are exactly three ways in and the thresholds are known:
         * r7 at n_refs >= 8 : floor_log2 goes 2 -> 3 and 3*8/83 = 0.289 >
           0.227.  n_refs is loop-weighted (flow.c:2122,
           `reg_n_refs[regno] += loop_depth'), so ONE more reference to r7
           INSIDE the loop is worth 2 and is enough.  r7 at 7 refs is
           2*7/83 = 0.169 and is NOT enough.
         * the pointer at n_refs <= 3 : 1*3/44 = 0.068 < 0.145.  At 4 refs
           it is 2*4/44 = 0.182 and still wins, so dropping ONE of its four
           uses is not enough -- two must go, or they must be served by a
           SECOND address pseudo.
         * the pointer's live_length > 69 : 2*5/x < 0.145 needs x > 69,
           i.e. its range would have to span the loop.
       Measured and rejected on the way (1823/199 body unless noted):
         r7++; r7--;  at every statement boundary in case 0x2A INCLUDING
                      inside the loop body, both orders: EVERY ONE compiles
                      BYTE-IDENTICALLY, so the "+4 references" route does
                      not exist for this quantity.  THEORY (not verified in
                      a dump): cse2 runs before flow -- cse_main at
                      toplev.c:3376, flow_analysis at toplev.c:3463 -- and
                      folds r7+1-1 back to r7, so flow never counts it.  Two,
                      three and four stacked pairs: also byte-identical.
                      The one position that changes anything, after
                      `if (i2A != (s16)r7)': 1819/206, worse.
         a named local for the pointer (`u8 *dp; dp = D_80082E80;' with dp
                      at all four uses), declared first and last in the
                      block: 175 either way (1821 words).
         every statement move inside case 0x2A: 187 or worse, and the
                      1818/222 entries all move a loop-body statement out
                      of the loop (wrong).
       Note also: the morning header's "CASE 0x2A'S LOOP COUNTER MUST BE ITS
       OWN VARIABLE" is still true and `i2A' must stay.

   (4) The 0x800B triangle -- ONE defect with three symptoms, 0 words net,
       ~13 rows.
       The target has the canonical block ONCE as
           .L800AF8F0: ori $v0,$zero,0x800B
           .L800AF8F4: sh  $v0,0x46($s5)
                       j   .L800AFAA4
                       addiu $v0,$zero,0x2
       (0x800B first, in $v0), and writes .L800AE010's copy out in full
       with the OTHER schedule (`addiu $v0,2' first, so 0x800B lands in $v1).
       Case 0x18's tail and case 0x20's `+0x800' tail both cross-jump into
       the canonical block: `j .L800AF8F0 / sh $v0,0x2A($s5)' and
       `addiu $v0,$v0,0x800 / j .L800AF8F0 / sh $v0,0x2A($s5)'.
       We get the canonical block right (change (2) created it) but
       CASE 0x18's tail becomes a SECOND copy in the $v1 schedule, and then
         - .L800AE010 cross-jumps into case 0x18's copy instead of being
           written out:            -5 words at target 108..112
         - case 0x18 keeps 4 words:                      +4
         - case 0x20 keeps its own `addiu $v0,$zero,2':   +1
       Measured and rejected (1823/199 body), as a 6x6 grid over
       .L800AE010's block x case 0x18's block:
         array form on either or both stores of either block   199 (no change)
         `h1 = 0x800B; *(u16 *)(c + 0x46) = h1;'  in .L800AE010  1828/198
                                                  in case 0x18   1828/195
                                                  in both        1825/201
         storing c[0x46] BEFORE the func_800A0818 call
                                   in .L800AE010  1826/201
                                   in case 0x18   1825/203
                                   in both        1824/207
       None reaches 199, and the two 195/198 entries cost five words.  This
       is the same jump2-ordering question as residue (1) and should go to
       the permuter with it.

   (5) `sp20 = r;' lands after the branch, not before.  6 rows, 0 words.
       Cases 0, 1 and 2: the target has `sh $v0,0x20($sp)' BEFORE the
       `addu $v1,$v0,$zero' copy and an empty delay slot; we put
       `sh $v1,0x20($sp)' in the delay slot, from the copy.  Writing
       `sp20 = r2;' before `*(s16 *)(c + 0x2A) = sp1E;' -- which is what
       the target's order suggests -- is WORSE and loses words:
         case 0 only   1813/189      case 1 only  1820/186
         both          1812/198      (and 1813/1812 delete real code)
       Left as it is.

   (6) `ret' is read as a constant at the c[0x25]==0 exit.  1 row.
       Target: `j .L800AFA9C / addu $fp,$zero,$zero' -- it sets $fp and goes
       THROUGH `sll $v0,$fp,16 / srl $v0,$v0,16'.  Ours: `j <epilogue> /
       move v0,zero', because `ret = 0; return ret << 16 >> 16;' sits in one
       extended basic block and cse folds 0<<16>>16 to 0.  For the target
       not to fold it, that `return' must have had two predecessors at cse
       time, which means a shared exit (a goto, or `ret = 0;' with the rest
       of the function in an else).  Everything the morning header measured
       about `ret''s type still holds -- `u16 ret' with
       `return ret << 16 >> 16;' is right and all five alternatives lose the
       shift pair -- so do not touch the type; this is about control flow.
       The OTHER two `ret' rows (target 586/590) are just a swap of
       `addu $fp,$zero,$zero' and `addu $a0,$s5,$zero' in case 4, 0 words.

   (7) One empty delay slot at 800ADFE4.  1 row, -1 word.
       The target leaves the `beqz $v0, .L800ADFF4' slot empty; cc1psx fills
       ours with the `addiu $v0,$zero,0xFF' that heads the then-arm
       (`.set noreorder / beq $2,$0,$L10 / li $2,255' in our ovl_dungeon.s).
       Not chased.

   (8) `lb $a1,0x9C($s5)' one slot earlier in the target, case 0x20.  2 rows.
   (9) case 0x20's `+0x800' result is in $v1 where the target has $v0.  2 rows.
       Both fall out of (4).

   ===========================================================================
   SWEEPS RUN, AND WHAT THEY RETURNED

   On the 1823/208 body:  devsweep --rounds 3   "0 device(s) already in the
       body ... 351 statements, 49 names -> 17550 candidates per round ...
       no wrapper helps".  THE BODY CONTAINS NO DEVICES AND WANTS NONE.
   On the 1823/208 body:  widthsweep --rounds 4  offers `s32 t1 -> u8' at
       1817/218.  REJECT -- see "THE WORD COUNT WAS A LIE".
   On the 1821/172 body:  every declared width of every non-pointer local
       (240 candidates, by hand): every candidate that improves the word gap
       does it by deleting a block the target has; nothing improves rows.
   On the 1821/172 body:  every `*(T *)(V + K)' in the body converted to
       `((T *)V)[K/sizeof T]' -- 142 singly, 448 pairs within 20 lines, and
       every (type,base,offset) group and every identical-statement group
       converted wholesale (30 groups).  All neutral or worse.  THE
       MEM_IN_STRUCT_P LEVER IS EXHAUSTED on this body.
   On the 1821/172 body:  every arm's `g' split out singly (13), `nd' (6),
       `i' (6), `j' (3).  All neutral or worse.
   On the 1823/187 body:  every same-width pair of the 40 locals merged
       (about 350 pairs).  The only hits truncate a width and delete code.
   On the 1823/187 body:  group merges {q1..q7}, {r1..r8}, {t1,t3,t5,t7},
       {h1,h2,h3}: 210, 221, 202, 197.  {t3,t5,t7} alone: NO CHANGE, so
       those three may be one variable in the original at no cost.
   stmtsweep-style statement moves over lines 100-150, 200-300 and the whole
       of case 0x24 and case 0x2A: every candidate that scores better is
       semantically wrong (reads a variable before its assignment, or moves
       a store past the assignment it feeds, or moves a loop-body statement
       out of the loop).  READ WHAT THEY MEAN BEFORE APPLYING ANY OF THEM.

   On the 1821/170 body:  every ADJACENT pair of same-indent simple
       assignments swapped (42 candidates).  One win -- change (10) -- and
       five wrong-but-better-scoring candidates, listed under (10).
   On the 1821/170 body:  the last two statements of every `return 5' tail
       swapped, single-line (4) and multi-line (8): all neutral or worse
       (1819/225, 1824/179 x2, 1824/181, 1824/183, 1829/184, 1830/187 x2).
       This was a deliberate attempt to move jump2's cross-jump pairing for
       residue (1) and it does not.
   On the 1821/170 body:  case 0x2A's `&D_80082E80' given a named local
       (`u8 *dp; dp = D_80082E80;' and dp used for all four references),
       declared both first and last in the block: 175 either way.
   On the 1821/170 body:  s32 dy -> s16 is 1822/168 -- two rows better and
       one word WORSE, and asymmetric with dx (s16 dx is 1822/180, both s16
       is 1823/174), so it is not taken.  If someone closes residue (1) and
       the word count comes out at 1816, this is the first thing to try.

   STILL NOT RUN to completion on the final body: tools/decomp/refsweep.py
   and tools/decomp/stmtsweep.py with the real tools (the hand versions
   above covered the regions with residue, not the whole function; a
   refsweep over 49 names x 351 statements is about 90 minutes here).

   ===========================================================================
   SHAPES I AM NOT HAPPY ABOUT, WITH THE HONEST ALTERNATIVE AND ITS COST

   * THE `((T *)c)[K]' SPELLINGS.  NINETEEN of the ~160 `c'-relative
     accesses are written as array references and the rest are not, and
     there is no story in which the original source mixed them like that.
     The honest reading is that `c' is a STRUCT in the original and these
     nineteen sites are field accesses -- a COMPONENT_REF also sets
     MEM_IN_STRUCT_P -- but then ALL of them would be, and that is measured:
     every `c + 0x2A' site in array form is 1812/198, every `c + 0x46' site
     is 1821/172 (neutral), every `c + 0x98' site neutral, and all 22
     `*(s16 *)(c + 0x2A) =' at once is 1815/242 on the 1823/220 body.  So a
     uniform struct does NOT reproduce the target and a mixed spelling does.
     Something about this function's `c' is still wrong; I do not know what.
     The array form is ordinary C and not a device, and the alternative --
     the pointer-cast form at all nineteen, i.e. this park's morning body
     with everything else of this session kept -- measures 1825/228, so it
     costs 58 rows and four words.  Kept.
   * `g22'.  A second pointer variable for one arm is the shape the register
     allocation demands (see change (7)) and is a perfectly ordinary local,
     but the name is mine.  The alternative, one shared `g', costs 5 rows.
   * NO DEVICES.  There is not a single `do { } while (0)', cancelled pair,
     duplicated arm or redundant store in this body, and devsweep confirms
     none helps.  That is worth keeping true.

   ===========================================================================
   STILL TRUE FROM THE 2026-11-16 HEADER, DO NOT RE-DERIVE

   * The empty switch cases must NOT be written out; gcc sizes the table
     from the lowest and highest case labels only (stmt.c:5052-5071, and the
     gap-filling loop at stmt.c:5304-5307).
   * The six shared tails are NOT a goto ladder: .L800AF89C/.L800AF8A0/
     .L800AF8A4 are the delay-slot scheduler stealing one or two
     instructions from the head of the code after the switch.  The body
     contains no gotos.
   * The facing preamble has NO separate index variable:
     `sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;' then `D_8006CCD8[sp1E]'.
     A `s32 u' intermediate costs a word in each of the nine copies.
   * `u16 ret' returned as `ret << 16 >> 16'.  Every other spelling loses
     the `sll $v0,$fp,16 / srl $v0,$v0,16' pair.
   * THE THREE ALIAS DECLARATIONS BELOW ARE REQUIRED.  func_800A35A4 must be
     seen as `s16' (through the landed `u8' the cast leaves an `andi' and
     the compare degrades: +5 words over three copies); func_8009FD40 must
     be seen as `s32' at cases 0x2C and 0x2A so `(u16)(r - 2) < 7' keeps its
     `andi'; func_800A40AC as `s32' in case 2.  They leave the landed
     definitions alone, the same way the unit's own `func_800BCB04h' does.
   * `sp1E = ((u32)func_800A0818(...) >> 9) & 7;' in cases 0x2A and 0x1C --
     without the (u32) the shift is `sra' where the target has `srl'.
   * `*(s16 *)(c + 0x2A) = sp1E;' BEFORE `sp20 = r;' in cases 0 and 1
     (re-measured this session: the other order is 1813/189 and 1820/186).
   * `D_800E2970_s[*(s8 *)(b + 0x26)].unk_0C & 2' is csrc/ovl_dungeon.c's
     own spelling.
   * func_800A0E6C IS CALLED WITHOUT A PROTOTYPE, as the unit's later call
     site does.  Adding one would change that landed call.
   * declaration-order search, 80 random reorderings: no effect at all.
   =========================================================================== */

s16 func_800A35A4h(u8 *, s32) __asm__("func_800A35A4");
s32 func_8009FD40w(u8 *, u8 *) __asm__("func_8009FD40");
s32 func_800A40ACw(u8 *, u16) __asm__("func_800A40AC");

s32 func_800ADE74(u8 *a, u8 *b, u8 *c, u16 d, u16 e, u8 *f)
{
    u16 ret;
    s16 sp18;
    s16 sp1A;
    s16 sp1C;
    s16 sp1E;
    s16 sp20;
    u8 *g;
    u8 *g22;
    u8 *q1;
    u8 *q2;
    u8 *q3;
    u8 *q4;
    u8 *q5;
    u8 *q6;
    u8 *q7;
    u8 *nd;
    u8 *o;
    u8 k;
    u8 k2;
    u16 h1;
    u16 h2;
    u16 h3;
    s32 i;
    s32 i2A;
    s32 j;
    s32 r1;
    s32 r2;
    s32 r3;
    s32 r4;
    s32 r5;
    s32 r6;
    s32 r7;
    s32 r8;
    s32 t1;
    s32 t3;
    s32 t5;
    s32 t7;
    s32 n;
    s32 nn;
    s16 aa;
    s32 ang;
    s32 dx;
    s32 dy;
    s16 mx;
    ret = 2;
    ((u16 *)c)[0x23] |= 0x4000;
    if ((D_80013714 & 8) != 0)
    {
        func_800A9A0C(c);
        return -1;
    }
    if (c[0x25] == 0)
    {
        ret = 0;
        return ret << 16 >> 16;
    }
    if ((s16)func_800A2CAC(c) != 0 && (s16)func_800A6E8C(b, 0x60C, &sp18, &sp1A) != 0 &&
        c[0x13] != 0x22)
    {
        if ((s16)func_800A2C34(c) != 0)
        {
            return -1;
        }
        if (sp18 == b[0x24] && sp1A == b[0x25])
        {
            r1 = (s16)func_800A70E4(sp18, sp1A, *(s16 *)(c + 0x88));
            if (r1 >= 0)
            {
                t1 = *(u16 *)(c + 0x24);
                if (t1 > 0x10000)
                {
                    t1 = 0xFFFF;
                }
                *(u16 *)(c + 0x24) = t1;
                if (c[0x25] != 0)
                {
                    *(s32 *)(c + 0x1C) &= ~0x8;
                }
                if (c[0x66] < c[0x25])
                {
                    c[0x25] = c[0x66];
                    c[0x24] = 0xFF;
                }
                func_80098B38(D_800E3548 + r1 * 4);
            }
            return 0;
        }
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], sp18, sp1A, (u16 *)&sp1C);
        *(u16 *)(c + 0x46) = 0x800B;
        return 2;
    }
    func_800A19E4((s32)b, c, (s16)d, (s16)e, f);
    if ((*(s32 *)(c + 0x1C) & 0x2410) == 0x2000)
    {
        k = c[0x12];
        if ((*(s32 *)(D_800E3D7C + 0x1C) & 0x220) != 0 ||
            (s16)func_80042900((s8 *)D_800E3D7C, 0xA) != 0)
        {
            if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 && k != 0)
            {
                k = 1;
            }
        }
        switch (k)
        {
        case 0:
            if ((s16)func_800A2C34(c) != 0)
            {
                return -1;
            }
            if ((s16)func_80042900((s8 *)c, 6) == 0)
            {
                sp1C = func_800A3544(c, 0x1E);
                if (sp1C >= 0)
                {
                    if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 &&
                        func_8009FD40(b, D_80082E80) < func_800A35A4h(c, sp1C) &&
                        (s16)func_800A3518(D_800E3D7C) != 0)
                    {
                        ((u16 *)c)[0x23] = (sp1C + 1) | 0x8000;
                        ((s16 *)c)[0x15] = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                                         D_80082E80[0x25], (u16 *)&sp1C);
                        *(u32 *)(c + 0x60) = D_800814A8;
                        return 4;
                    }
                    i = 0;
                    j = D_8008347E & 1;
                    do
                    {
                        q1 = *(u8 **)(D_800814A8 + j * 4 + 0xAC);
                        if (q1 != NULL)
                        {
                            nd = *(u8 **)(q1 - 0x14);
                            if (func_8009FD40(b, nd) < func_800A35A4h(c, sp1C) &&
                                (s16)func_800A3518(*(u8 **)(D_800814A8 + j * 4 + 0xAC)) != 0)
                            {
                                ((u16 *)c)[0x23] = (sp1C + 1) | 0x8000;
                                if (*(u8 **)(D_800814A8 + j * 4 + 0xAC) != c)
                                {
                                    *(s16 *)(c + 0x2A) =
                                        func_800A0818(b[0x24], b[0x25], nd[0x24], nd[0x25],
                                                      (u16 *)&sp1C);
                                }
                                *(u32 *)(c + 0x60) = *(u32 *)(D_800814A8 + j * 4 + 0xAC);
                                return 4;
                            }
                        }
                        i++;
                        j ^= 1;
                    } while (i < 2);
                }
            }
            q2 = func_800A3D18(b, c, 4);
            *(u8 **)(c + 0x60) = q2;
            if (q2 == NULL)
            {
                break;
            }
            r2 = func_800A384C(c, q2, (u8 *)&sp1E, 0);
            *(s16 *)(c + 0x2A) = sp1E;
            sp20 = r2;
            if ((s16)r2 < 0)
            {
                *(u16 *)(c + 0x46) = 0x800B;
                return 2;
            }
            *(u16 *)(c + 0x46) = r2 | 0x8000;
            return 1;

        case 1:
            if ((s16)func_800A2C34(c) != 0)
            {
                return -1;
            }
            if ((s16)func_80042900((s8 *)c, 6) == 0)
            {
                sp1C = func_800A3544(c, 0x1E);
                if (sp1C >= 0)
                {
                    if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 &&
                        func_8009FD40(b, D_80082E80) < func_800A35A4h(c, sp1C) &&
                        (s16)func_800A3518((u8 *)D_800814A8) != 0)
                    {
                        ((u16 *)c)[0x23] = (sp1C + 1) | 0x8000;
                        ((s16 *)c)[0x15] = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                                         D_80082E80[0x25], (u16 *)&sp1C);
                        *(u32 *)(c + 0x60) = D_800814A8;
                        return 4;
                    }
                    i = 0;
                    j = D_8008347E & 1;
                    do
                    {
                        q3 = *(u8 **)(D_800814A8 + j * 4 + 0xAC);
                        if (q3 != NULL)
                        {
                            nd = *(u8 **)(q3 - 0x14);
                            if (func_8009FD40(b, nd) < func_800A35A4h(c, sp1C) &&
                                (s16)func_800A3518(*(u8 **)(D_800814A8 + j * 4 + 0xAC)) != 0)
                            {
                                ((u16 *)c)[0x23] = (sp1C + 1) | 0x8000;
                                if (*(u8 **)(D_800814A8 + j * 4 + 0xAC) != c)
                                {
                                    *(s16 *)(c + 0x2A) =
                                        func_800A0818(b[0x24], b[0x25], nd[0x24], nd[0x25],
                                                      (u16 *)&sp1C);
                                }
                                *(u32 *)(c + 0x60) = *(u32 *)(D_800814A8 + j * 4 + 0xAC);
                                return 4;
                            }
                        }
                        i++;
                        j ^= 1;
                    } while (i < 2);
                }
            }
            q4 = func_800A3D18(b, c, -3);
            *(u8 **)(c + 0x60) = q4;
            if (q4 == NULL)
            {
                break;
            }
            r3 = func_800A384C(c, q4, (u8 *)&sp1E, 0);
            *(s16 *)(c + 0x2A) = sp1E;
            sp20 = r3;
            if ((s16)r3 < 4)
            {
                *(u16 *)(c + 0x46) = 0x800B;
                return 2;
            }
            *(u16 *)(c + 0x46) = r3 | 0x8000;
            return 1;

        case 2:
            if ((s16)func_80042900((s8 *)c, 6) == 0)
            {
                sp1C = func_800A3544(c, 0x1E);
                if (sp1C >= 0)
                {
                    if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 &&
                        func_8009FD40(b, D_80082E80) < func_800A35A4h(c, sp1C) &&
                        (s16)func_800A3518((u8 *)D_800814A8) != 0)
                    {
                        if ((s16)func_800A2C34(c) != 0)
                        {
                            return -1;
                        }
                        ((u16 *)c)[0x23] = (sp1C + 1) | 0x8000;
                        ((s16 *)c)[0x15] = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                                         D_80082E80[0x25], (u16 *)&sp1C);
                        *(u32 *)(c + 0x60) = D_800814A8;
                        return 4;
                    }
                    i = 0;
                    j = D_8008347E & 1;
                    do
                    {
                        q5 = *(u8 **)(D_800814A8 + j * 4 + 0xAC);
                        if (q5 != NULL)
                        {
                            nd = *(u8 **)(q5 - 0x14);
                            if (func_8009FD40(b, nd) < func_800A35A4h(c, sp1C) &&
                                (s16)func_800A3518(*(u8 **)(D_800814A8 + j * 4 + 0xAC)) != 0)
                            {
                                if ((s16)func_800A2C34(c) != 0)
                                {
                                    return -1;
                                }
                                ((u16 *)c)[0x23] = (sp1C + 1) | 0x8000;
                                if (*(u8 **)(D_800814A8 + j * 4 + 0xAC) != c)
                                {
                                    *(s16 *)(c + 0x2A) =
                                        func_800A0818(b[0x24], b[0x25], nd[0x24], nd[0x25],
                                                      (u16 *)&sp1C);
                                }
                                *(u32 *)(c + 0x60) = *(u32 *)(D_800814A8 + j * 4 + 0xAC);
                                return 4;
                            }
                        }
                        i++;
                        j ^= 1;
                    } while (i < 2);
                }
            }
            if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 && (s16)func_800A404C() == 0 &&
                *(u8 *)(D_800814A8 + 0x9A) == 0x11)
            {
                o = *(u8 **)(D_800814A8 + 0x60);
                if (o != NULL && (*(s32 *)(o + 0x14) & 0x2000) == 0)
                {
                    r4 = func_800A40ACw(c, func_8009CD58(o, 7, 0));
                    sp20 = r4;
                    if ((s16)r4 >= 0)
                    {
                        *(u16 *)(c + 0x46) = r4 | 0x8000;
                        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                                           D_80082E80[0x25], (u16 *)&sp20);
                        return 3;
                    }
                }
            }
            if ((s16)func_800A2C34(c) != 0)
            {
                return -1;
            }
            break;

        case 4:
            if ((s16)func_800A2C34(c) != 0)
            {
                return -1;
            }
            q6 = func_800A03C4(c, b[0x24], b[0x25]);
            ret = 0;
            *(u8 **)(c + 0x60) = q6;
            if (q6 != NULL)
            {
                r5 = func_800A384C(c, q6, (u8 *)&sp1E, 0);
                sp20 = r5;
                if ((s16)r5 >= 0)
                {
                    ((u16 *)c)[0x23] = r5 | 0x8000;
                    *(s16 *)(c + 0x2A) = sp1E;
                    return 1;
                }
            }
            break;

        default:
            break;
        }
        if ((s16)func_800A2C34(c) != 0)
        {
            return -1;
        }
        return ret << 16 >> 16;
    }

    if ((s16)func_800A2C34(c) != 0)
    {
        return -1;
    }
    if ((*(s32 *)(c + 0x1C) & 0x10) != 0)
    {
        return ret << 16 >> 16;
    }
    if (*(s8 *)(b + 0x26) >= 0 && (D_800E2970_s[*(s8 *)(b + 0x26)].unk_0C & 2) != 0)
    {
        return ret << 16 >> 16;
    }
    if ((s16)func_80042900((s8 *)c, 6) == 0)
    {
        sp1C = func_800A3544(c, 0x1E);
        if (sp1C >= 0 && (s16)func_800A3518(c) != 0)
        {
            ((u16 *)c)[0x23] = (sp1C + 1) | 0x8000;
            *(u8 **)(c + 0x60) = c;
            return 4;
        }
    }
    switch (c[0x13])
    {
    case 0x19:
        func_800ADD20(c, 0x10);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                          *(s16 *)(c + 0x88));
        if (g[0x13] != 0)
        {
            break;
        }
        if ((s16)func_800A3518(c) == 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x800A;
        return 6;

    case 0x16:
        if (c[0x48] != 0xF)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        if ((s16)func_8009FD40(D_80082E80, b) >= 0xB)
        {
            break;
        }
        if (func_800A365C(b, D_80082E80) == 0)
        {
            break;
        }
        if (func_800A36B4(c, D_800E3D7C) == 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8008;
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)&sp20);
        return 1;

    case 0x2C:
        func_800ADD20(c, 4);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        r6 = func_8009FD40w(D_80082E80, b);
        if ((u16)(r6 - 2) >= 7)
        {
            break;
        }
        if (func_800A365C(b, D_80082E80) == 0)
        {
            break;
        }
        t3 = (s16)func_800A0134(c, D_800E3D7C);
        if (t3 < 0)
        {
            t3 = -t3;
        }
        if (t3 >= 0x21)
        {
            break;
        }
        g = (u8 *)func_800A05A4(c, b[0x24], b[0x25], *(s16 *)(c + 0x2A), (s16)r6);
        if ((s16)func_800A2CB8(c, g) == 0)
        {
            break;
        }
        if (func_800A36B4(c, g) == 0)
        {
            break;
        }
        if (c[0xAE] != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)&sp20);
        return 5;

    case 0x2B:
        func_800ADD20(c, 0x20);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        if ((*(u16 *)(c + 0x98) & 0x8000) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)&sp20);
        return 5;

    case 0x22:
        func_800ADD20(c, 0x20);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        if (*(s8 *)(b + 0x26) < 0)
        {
            break;
        }
        for (i = 0; i < 2; i++)
        {
            g22 = *(u8 **)(D_800E3D7C + i * 4 + 0xAC);
            if (g22 != NULL && (s16)func_800A2CAC(g22) != 0)
            {
                *(u16 *)(c + 0x46) = 0x8009;
                *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                                   D_80082E80[0x25], (u16 *)&sp20);
                return 5;
            }
        }
        break;

    case 0x29:
        func_800ADD20(c, 0x10);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        for (i = 0; i < 2; i++)
        {
            g = *(u8 **)(D_800E3D7C + i * 4 + 0xAC);
            if (g != NULL)
            {
                t5 = (s16)func_800A0134(c, g);
                if (t5 < 0)
                {
                    t5 = -t5;
                }
                if (t5 < 0x21 && (s16)func_800A41F0(g) != 0)
                {
                    nd = *(u8 **)(g - 0x14);
                    if ((s16)func_8009FD40(nd, b) == 1 && func_800C7F68(g) == 0)
                    {
                        *(u16 *)(c + 0x46) = 0x8009;
                        *(u8 **)(c + 0xA8) = g;
                        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], nd[0x24],
                                                           nd[0x25], (u16 *)&sp20);
                        return 5;
                    }
                }
            }
        }
        break;

    case 0x28:
        func_800ADD20(c, 0x40);
        h1 = *(u16 *)(c + 0x98);
        if ((h1 & 0x100) != 0)
        {
            *(u16 *)(c + 0x98) = h1 | 0x8000;
            break;
        }
        *(u16 *)(c + 0x98) = h1 & 0x7FFF;
        break;

    case 0x1A:
        func_800ADD20(c, 2);
        h2 = *(u16 *)(c + 0x98);
        *(u16 *)(c + 0x98) = h2 & 0x7FFF;
        if ((h2 & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                          *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((s16)func_800A2CB8(c, g) == 0)
        {
            break;
        }
        if ((s16)func_80042900((s8 *)g, 1) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x98) |= 0x8000;
        break;

    case 0x23:
        func_800ADD20(c, 4);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        if (*(s16 *)(c + 0xA6) != 0)
        {
            break;
        }
        if ((D_800E296C & 4) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        return 5;

    case 0x26:
        func_800ADD20(c, 2);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                          *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((s16)func_800A2CB8(c, g) == 0)
        {
            break;
        }
        if ((s16)func_80042900((s8 *)g, 4) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        return 5;

    case 0x27:
        func_800ADD20(c, 0x10);
        h3 = *(u16 *)(c + 0x98);
        *(u16 *)(c + 0x98) = h3 & 0x7FFF;
        if ((h3 & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                          *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((s16)func_800A2CB8(c, g) == 0)
        {
            break;
        }
        if ((s16)func_80042900((s8 *)g, 2) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        *(u16 *)(c + 0x98) |= 0x8000;
        return 5;

    case 0x20:
        if (c[0x49] == 0)
        {
            func_800ADD20(c, 2);
            if ((*(u16 *)(c + 0x98) & 0x100) == 0)
            {
                break;
            }
            sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
            if (func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                              *(s16 *)(c + 0x88)) != D_800E3D7C)
            {
                break;
            }
            *(u16 *)(c + 0x46) = 0x8009;
            return 5;
        }
        if ((*(s32 *)(c + 0x1C) & 0x410) != 0)
        {
            break;
        }
        func_800AA53C(c);
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            return 2;
        }
        if (*(s8 *)(b + 0x26) >= 0)
        {
            func_800A0E6C(b, *(s8 *)(c + 0x9C), c, c + 0x98);
            *(u16 *)(c + 0x46) = 0x800B;
            return 2;
        }
        if ((s16)func_8009FD40(b, D_80082E80) != 1)
        {
            *(u16 *)(c + 0x46) = 0x800B;
            return 2;
        }
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)(c + 0x98)) +
                             0x800;
        *(u16 *)(c + 0x46) = 0x800B;
        return 2;

    case 0x24:
        ((u16 *)c)[0x4C] &= 0x7FFF;
        if (c[0x49] != 0)
        {
            break;
        }
        if ((s16)func_800A6E8C(b, 0x12, &sp18, &sp1A) == 0)
        {
            break;
        }
        ((s16 *)c)[0x15] = func_800A0818(b[0x24], b[0x25], sp18, sp1A, (u16 *)(c + 0x98));
        ((u16 *)c)[0x54] = sp18;
        ((u16 *)c)[0x55] = sp1A;
        ((u16 *)c)[0x4C] |= 0x8000;
        dx = b[0x24] - sp18;
        if (dx < 0)
        {
            dx = -dx;
        }
        mx = dx;
        sp18 = dx;
        dy = b[0x25] - sp1A;
        if (dy < 0)
        {
            dy = -dy;
        }
        sp1A = dy;
        if (sp18 < sp1A)
        {
            mx = sp1A;
        }
        if (mx == 1)
        {
            *(u16 *)(c + 0x46) = 0x8009;
            return 5;
        }
        *(u16 *)(c + 0x46) = 0x800B;
        return 2;

    case 0x25:
        func_800ADD20(c, 0x20);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        return 5;

    case 0x18:
        if ((s16)func_8009FD40(b, D_80082E80) >= 2 && c[0xB5] == 0)
        {
            func_800ADD20(c, 8);
            if ((*(u16 *)(c + 0x98) & 0x100) != 0)
            {
                *(u16 *)(c + 0x46) = 0x800A;
                return 6;
            }
        }
        if (c[0xB5] == 0)
        {
            break;
        }
        g = func_8009B25C(c, b[0x24], b[0x25], *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            *(u16 *)(c + 0x46) = 0x800B;
            return 2;
        }
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)(c + 0x98));
        *(u16 *)(c + 0x46) = 0x800B;
        return 2;

    case 0x2A:
        func_800ADD20(c, 4);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        if (func_800A365C(b, D_80082E80) == 0)
        {
            break;
        }
        r7 = func_8009FD40w(D_80082E80, b);
        if ((u16)(r7 - 2) >= 7)
        {
            break;
        }
        ang = *(u16 *)(c + 0x88);
        sp1E = ((u32)func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                   (u16 *)&sp20) >>
                9) &
               7;
        i2A = 0;
        sp18 = b[0x24];
        sp1A = b[0x25];
        for (; i2A < (s16)r7; i2A++)
        {
            sp18 = sp18 + (u16)D_8006CCD8[sp1E];
            sp1A = sp1A + (u16)D_8006CCE8[sp1E];
            if ((s16)func_800A45D8(sp18 << 6, sp1A << 6, (s16)(ang + 0x20)) != 0)
            {
                break;
            }
            n = func_800BCB04(((sp18 << 6) + 0x20) & 0xFFE0, ((sp1A << 6) + 0x20) & 0xFFE0,
                              (s16)(ang - 0x20));
            nn = (s16)n;
            aa = (s16)ang;
            if (nn < aa)
            {
                break;
            }
            if (nn - aa >= 0x21)
            {
                break;
            }
            ang = n;
        }
        if (i2A != (s16)r7)
        {
            break;
        }
        ((u16 *)c)[0x23] = 0x8009;
        *(s16 *)(c + 0x2A) = sp1E << 9;
        return 5;

    case 0x1B:
        func_800ADD20(c, 4);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        for (i = 0; i < 2; i++)
        {
            g = *(u8 **)(D_800E3D7C + i * 4 + 0xAC);
            if (g != NULL)
            {
                t7 = (s16)func_800A0134(c, g);
                if (t7 < 0)
                {
                    t7 = -t7;
                }
                if (t7 < 0x21 && (s16)func_800A41F0(g) != 0 &&
                    (s16)func_80042900((s8 *)g, 0xC) == 0)
                {
                    nd = *(u8 **)(g - 0x14);
                    if ((s16)func_8009FD40(nd, b) == 1)
                    {
                        *(u16 *)(c + 0x46) = 0x8009;
                        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], nd[0x24],
                                                           nd[0x25], (u16 *)&sp20);
                        return 5;
                    }
                }
            }
        }
        break;

    case 9:
    case 0xA:
        g = func_800A02AC(c, b[0x24], b[0x25]);
        if (g == NULL)
        {
            break;
        }
        if ((u32)(g[0x13] - 0x33) >= 4)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        nd = *(u8 **)(g - 0x14);
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], nd[0x24], nd[0x25], (u16 *)&sp20);
        return 5;

    case 0x17:
        func_800ADD20(c, 0x10);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                          *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((*(s32 *)(g + 0x14) & 0x4000) == 0)
        {
            break;
        }
        if (g == D_800E3D7C)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        return 5;

    case 0x1C:
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        func_800ADD20(c, 0x20);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        sp1E = ((u32)func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                   (u16 *)&sp20) >>
                9) &
               7;
        if ((s16)func_8009A350(b[0x24], b[0x25], sp1E, (u16 *)&sp20) == 0)
        {
            break;
        }
        if ((sp20 & 0xB712) != 0)
        {
            break;
        }
        if ((s16)func_8009FD40(D_80082E80, b) != 2)
        {
            break;
        }
        ((u16 *)c)[0x23] = 0x8009;
        *(s16 *)(c + 0x2A) = sp1E << 9;
        return 5;

    case 0x1D:
        func_800ADD20(c, 8);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                          *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((*(s32 *)(g + 0x14) & 0x4000) == 0)
        {
            break;
        }
        if (g[0x13] == 0)
        {
            break;
        }
        if ((s16)func_80042900((s8 *)g, 4) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        return 5;

    case 0x15:
        if (c[0xA7] == 0)
        {
            break;
        }
        if (c[0xA8] != 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                          *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if (g != D_800E3D7C)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)&sp20);
        return 5;

    default:
        break;
    }

    q7 = func_800A3D18(b, c, 2);
    *(u8 **)(c + 0x60) = q7;
    if (q7 != NULL)
    {
        r8 = func_800A384C(c, q7, (u8 *)&sp1E, 1);
        sp20 = r8;
        *(s16 *)(c + 0x2A) = sp1E;
        if ((s16)r8 >= 0)
        {
            *(u16 *)(c + 0x46) = r8 | 0x8000;
            return 1;
        }
        *(u16 *)(c + 0x46) = 0x800B;
        return 2;
    }
    k2 = c[0x13];
    if (k2 == 0x1E && (*(s32 *)(c + 0x14) & 0x20000000) == 0)
    {
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        sp18 = b[0x24] + (u16)D_8006CCD8[sp1E];
        sp1A = b[0x25] + (u16)D_8006CCE8[sp1E];
        g = NULL;
        if (c[0xAC] == 0)
        {
            g = func_8009B25C(c, sp18, sp1A, *(s16 *)(c + 0x88));
            if (g != NULL && ((u32)(g[0x13] - 1) >= 0x2D || g[0x13] == k2))
            {
                g = NULL;
            }
        }
        if (c[0xAC] < 2)
        {
            if (g != NULL)
            {
                *(u16 *)(c + 0x46) = 0x8009;
                return 5;
            }
            g = (u8 *)((s16)func_800A70E4(sp18, sp1A, *(s16 *)(c + 0x88)) + 1);
        }
        if (g != NULL)
        {
            *(u16 *)(c + 0x46) = 0x8009;
            return 5;
        }
    }
    if (*(s8 *)(b + 0x26) < 0)
    {
        return ret << 16 >> 16;
    }
    return 2;
}