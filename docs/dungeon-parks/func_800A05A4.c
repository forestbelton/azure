/* PARK: func_800A05A4, 139 of 139 words, 59 aligned differences (`ovlverify'

   ============================== 2026-11-21: 47 rows -> 45

   `r = NULL;` moved from the middle of the head block to the FIRST statement
   of the function, ahead of `g = ((u32)dir >> 9) & 7;`.  Nothing else; the
   permuter's find carried no other change.  r's pseudo is created by its
   first use (DECL_RTL is lazy, see docs/matching-levers.md), so moving the
   initialisation moves r's number ahead of g's and the two swap places in
   local-alloc's queue.  Statement ORDER inside a straight-line block is a
   lever even where declaration order is not.

   AND IT CONTRADICTS THIS PARK'S OWN "the entry block is exhausted".  The
   210-order sweep below measured every interleaving of the entry block and
   concluded order "cannot get below 59".  It was swept on the body AS IT THEN
   WAS -- before the `do { } while (0);` device.  The device changed which
   pseudo wins the contested register, and that changed which order is best:
   one of the orders the sweep had scored 59 is now 45.  A SWEEP'S NEGATIVE IS
   SCOPED TO THE BODY IT WAS SWEPT ON.  After landing any device, the earlier
   sweeps are stale and worth re-running -- which is cheap, and is why the
   permuter found this in minutes.
   target 139 words, candidate 139 words, 45 aligned differences

   ============================== 2026-11-20: 59 rows -> 47

   The body below carries one device: a `do { } while (0);` around `r = func_8009B4B0(p, xx, y...);`.

   A `do { } while (0);` wrapper and a duplicated-arm `if (v) { s } else { s }`
   are the same lever from two directions -- flow.c:2128 weights each
   reference by `loop_depth`, and jump2's cross-jumping merges identical arms
   -- so both cost nothing and both are ordinary C.  SWEPT, NOT REASONED:
   every statement position times both devices over every local, on a
   `tools/decomp/onefunc.py` file where a candidate is 0.09 s.  Nothing in
   this park's diagnosis points at the winning statement, and that is the
   normal case rather than the exception.
   reports 64 differing masked).  ovl_dungeon.  Measured 2026-11-16, batch j48,
   over the 2026-11-16 138-word park.

   THE WORD COUNT IS NOW EXACT.  The missing word -- the `lhu' reload of `i'
   that loop.c was hoisting into the preheader -- is back, together with the
   preheader `addu $s5, $s6, $zero' copy the target carries and this body used
   to lack.  What is left is ONE INSTRUCTION IN THE WRONG BASIC BLOCK and the
   nine-register permutation that follows from it.

   THE PREVIOUS PARK IS ONE EDIT AWAY: delete `s32 ii;', delete the `ii = i;'
   statement and write `i << 9' back into the func_800A44E0 call.  That body
   is 138 of 139 words, 42 aligned / 64 masked.  Keep it if the permuter wants
   a one-word-short residue; this one if it wants a word-exact one.  Both score
   64 masked; they differ only in how aligndiff charges the residue.

   NO NEW DECLARATIONS.  The prototype `s32 func_800A05A4(u8 *, s32, s32,
   s16, s32);' at csrc/ovl_dungeon.c:1483 is pinned by the landed caller
   func_800C0E88 and this body matches it.  Writing the fourth parameter `s32'
   is a hard `conflicting types', which is how its type was settled.

   ------------------------------------------------- THE LEVER THAT PAID (new)

   A USER VARIABLE WHOSE SET AND WHOSE USE ARE IN DIFFERENT BASIC BLOCKS OF
   THE LOOP CANNOT BE HOISTED.  Not in docs/matching-levers.md as of j48.

   `i << 9' written inline in the func_800A44E0 call expands to two insns, a
   `zero_extend' of the HImode `i' into a COMPILER TEMP and an `ashift' of
   that temp.  cdk-gcc-b18 loop.c:794-800 admits a loop-invariant set as a
   movable if ANY of three arms holds, and arm (2) is
   `! REG_USERVAR_P (SET_DEST (set)) && ! REG_LOOP_TEST_P (SET_DEST (set))' --
   which a compiler temp always satisfies.  So the zero_extend is hoisted into
   the preheader whatever the surrounding source says, the two insns collapse
   to one, and the function is one word short.

   Give the zero_extend a DECLARED local as its destination and arm (2) fails.
   Arm (1) (`! maybe_never && ! loop_reg_used_before_p') fails as long as the
   assignment sits after a conditional jump inside the loop -- here, after the
   `if (r != NULL)' block.  Arm (3) is `reg_in_basic_block_p', which fails as
   long as the USE is in a later basic block than the set -- here, the
   `if (func_800A0548(...))' between them ends the block.  All three fail, the
   insn is not a movable, and the `lhu' stays in the loop:

       s32 ii;                 (* declared 
       ...
       ii = i;                  after the `if (r != NULL)' block 
       if (func_800A0548(...) != 0) { ... return 0; }      ends the block 
       ... func_800A44E0(..., ii << 9) ...                 the use 

   139 words.  138 -> 139 and 42 -> 59 aligned rows, and the preheader copy
   appears at the same word the target has it.

   Note the arms are an OR: two of the three must be broken as well as the
   third, and the placement is what breaks them.  Every placement that puts
   the set and the use in the SAME block measures 138 again (arm 3 passes).

   -------------------------------------- what the residue is now, 59 rows

   ONE `lhu' ONE BLOCK TOO EARLY, AND NINE REGISTERS PERMUTED.

       target   .L800A06FC: lhu $a0,0($s5) / lw $t0,0x20($sp) / ... /
                            jal func_800A0548 / bnez $v0,.L800A065C
                            ---- block boundary ----
                            andi $a1,$a1,0xFFC0 / lhu $a2,0x88($fp) /
                            lhu $t0,0x10($sp)      <- word 104
                            ... / jal func_800A44E0 / sll $a3,$t0,9

       this     the same `lhu' at word 89, inside the EARLIER block, because
                that is where `ii = i;' has to sit for arm (3) to fail.

   Every other opcode, immediate, displacement and branch target matches.  The
   remaining ~50 rows are one permutation of the nine callee-saved registers:

       quantity                 this        target
       xx                       $s1         $s0
       yy                       $s2         $s1
       lim (then the copy)      $s7         $s2
       thr                      $s4         $s3
       r                        $s5         $s4
       i*2 (then the copy)      $s0         $s5
       &D_8006CCD8[g]           $s3         $s6
       &D_8006CCE8[g], then k   $s6         $s7
       p                        $fp         $fp

   so the frame, the spill slots (0x10 `i', 0x18 `lim', 0x20 `i*2') and the
   save/restore set are all the target's; only which pseudo sits in which
   callee-saved register differs.  That is `.greg' work: global.c's
   `allocno_compare' orders by floor_log2(n_refs)*n_refs/live_length, and the
   `lhu' being one block early is what shortens `i' and lengthens the block-A
   pseudos enough to reorder the list.

   ------------------------------- the 210-order sweep, all at 139 words

   Every interleaving of {r = NULL; thr = 0x20; lim = n;} with the dependent
   chain {g; i = g; xx; yy} in the entry block -- 210 orders -- was measured on
   this body.  ALL 210 are 139 words.  They score 59 (42 orders), 60 (63),
   64 (42) or 65 (63) aligned.  Statement order in the entry block is
   therefore exhausted: it cannot move the `lhu' and it cannot get below 59.
   This body uses one of the 59s: g, thr, lim, i, r, xx, yy.

   ------------------------------ placements measured for `ii', with scores

     ii = i; after the `if (r != NULL)' block, use in the later block
                                                           139 / 59-65  <- this
     ii = i; after the `if (func_800A0548())' block (same block as the use)
                                                           138 / 42  (hoisted)
     ii = i; both places (cse deletes the second)           139 / 65
     ii = i; before the loop AND after func_800A0548        137 / 67
     ii = 0; before func_800A0548, ii = i; after it         137 / 67
     ii = g; before func_800A0548, ii = i; after it         137 / 67
     `d' reused as the destination instead of a new `ii'    137 / 67
     u32 ii / ii declared first in the block / dropping the
       loop-top `i = g;' entirely                           139 / 65 (plateau)
   and without `ii' at all:
     `i = g;' moved from the loop top to the loop bottom    141 / 51
     `i = g;' at BOTH the loop top and the loop bottom      138 / 42

   The 137s are all the same regime: once the zero_extend lands in the block
   with its use, `i' stops being spilled as a halfword, the entry `sh' and the
   loop `lhu' both go, the `i*2' spill and the preheader copy go with them,
   and the frame drops to 0x48.

   ------------------------------------- spellings measured on the 138-word
   ------------------------------------- body by the 2026-11-16 park, kept
   ------------------------------------- because they still apply here

   `u16 i' MUST HAVE TWO SETS.  With a single `i = ((u32)dir >> 9) & 7;' the
   HImode pseudo never survives: combine's `set_nonzero_bits_and_sign_copies'
   records nonzero_bits 7 for the `andi' result (it records only for pseudos
   with REG_N_SETS == 1), `zero_extend (subreg:HI g)' folds back to `g', and
   `i' is spilled as a WORD.  Splitting the value into an `s32 g' that carries
   every array index and a `u16 i' assigned both before the loop and again as
   the loop body's first statement gives `i' two sets and the fold cannot be
   proved.  (On THIS body, dropping the loop-top set measures the same 139/65
   plateau, so the second set is no longer load-bearing -- but it costs
   nothing and it is what the measurement above was taken on.)

   On the old 139-word plateau (all BYTE-IDENTICAL to each other), each 139/80:
     s16 i instead of u16 i; ((u16)dir >> 9) & 7; ((u32)dir >> 9) % 8;
     j = (u32)dir >> 9; i = j & 7; `dir' reassigned in place; the whole
     `((u32)dir >> 9) & 7' inlined at all 7 uses; i * 512; (s32)i << 9;
     (u16)D_8006CCD8[i]; operand order flipped on the x + ... sum;
     `i' declared first / last; xx/yy declaration order swapped; s32 r with
     casts; s16 *dp = &D_8006CCD8[i] inside the loop; for (k = 0; k < lim; k++);
     k = 0; if (k < lim) { do { } while (k < lim); }.

   and everything that moved the word count, all worse:
     ((u32)dir & 0xE00) >> 9  139/82;  (i << 9) & 0xE00  139/94;
     (i & 7) << 9  137/105;  u32 i  137/105;  s32 w9 = i << 9 hoisted 137/113;
     s32 g with a single u16 i = g  137/101;  u16 i[1] / volatile u16 i /
     (void)&i  137/101-106;  s16 k 140/91;  s16 d 141/103;  s16 *ep 140/108;
     both dp and ep 140/111;  u16 xx, u16 yy 145/136;  k = 0 above the first
     call 142/124;  (void)&i on the two-set body 142/135;  volatile u16 i on
     it 142/114;  __asm__("" : "+r"(i)) at the loop top 144/136, before the
     func_800A44E0 call 148/142;  __asm__("" ::"r"(i)) there 143/132;
     dp and ep loop-top pointers 139/76;  ep-only 139/68;  `i = g' just before
     the func_800A44E0 call 137/105.

   THREE SPELLINGS ARE LOAD-BEARING AND MUST NOT BE VARIED BY A SEARCH:

   * `d = d >= 0 ? d : -d;' -- the abs fold.  `if (d < 0) { d = -d; }' fills
     the `bgez's delay slot and is one word short; the target's bgez/nop/negu
     trio is gcc's `abs' pattern.
   * `s32 thr' with `(s16)thr' in the `d <= thr' compare AND an explicit
     `(s16)(*(u16 *)(p + 0x88) - thr)' on func_800A44E0's third argument.
     func_800A44E0 is DEFINED BELOW this function, so the call rides an
     implicit declaration and gets no prototype conversion -- with the cast
     omitted the sign extension disappears entirely and it is 137 / 97.  With
     `s16 thr' and no cast the extension lands on `thr' instead of on the
     difference: 139 words, 73 aligned.
   * ONE loop guard on `k', not on the constant.  `k = 0;' before a plain
     `while (k < lim)' is what makes cse canonicalise the duplicated guard's
     `k' onto `r' (both are 0 there) and gives the target's
     `slt $v0, $s4, $v0'; writing the guard as `if (0 < lim)' gives a `blez'
     and is one word short. *)
 */

s32 func_800A05A4(u8 *p, s32 x, s32 y, s16 dir, s32 n)
{
    u8 *r;
    s16 xx;
    s16 yy;
    u16 i;
    s32 g;
    s16 lim;
    s32 thr;
    s32 k;
    s32 d;
    s32 ii;

    r = NULL;
    g = ((u32)dir >> 9) & 7;
    thr = 0x20;
    lim = n;
    i = g;
    xx = (D_8006CCD8[g] & 0xFFFF) + x;
    yy = (D_8006CCE8[g] & 0xFFFF) + y;
    if (func_800A0548(xx, yy) != 0)
    {
        p[0x72] = *(u8 *)&D_8006CCD8[g] - xx;
        p[0x73] = *(u8 *)&D_8006CCE8[g] - yy;
        return 0;
    }
    k = 0;
    while (k < lim)
    {
        i = g;
        do
        {
            r = func_8009B4B0(p, xx, yy);
        } while (0);
        if (r != NULL)
        {
            d = *(s16 *)(r + 0x88) - *(s16 *)(p + 0x88);
            d = d >= 0 ? d : -d;
            if (d <= (s16)thr)
            {
                break;
            }
        }
        ii = i;
        if (func_800A0548(xx + (D_8006CCD8[g] & 0xFFFF), yy + (D_8006CCE8[g] & 0xFFFF)) != 0)
        {
            p[0x72] = *(u8 *)&D_8006CCD8[g] - xx;
            p[0x73] = *(u8 *)&D_8006CCE8[g] - yy;
            return 0;
        }
        if ((s16)func_800A44E0((xx << 6) & 0xFFC0, (yy << 6) & 0xFFC0,
                               (s16)(*(u16 *)(p + 0x88) - thr), ii << 9) != 0)
        {
            p[0x72] = -xx;
            p[0x73] = -yy;
            return 0;
        }
        thr += 0x20;
        k++;
        xx += D_8006CCD8[g] & 0xFFFF;
        yy += D_8006CCE8[g] & 0xFFFF;
    }
    p[0x72] = xx;
    p[0x73] = yy;
    return (s32)r;
}
