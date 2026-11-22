/* PARK: func_800C2824 -- 278/278 words, 43 ALIGNED ROWS.
   Opened at 285/84.  Handed to this session at 278/73.

   Measured with
       python3 tools/decomp/parkresidue.py func_800C2824
   and, for every sweep below, on an onefunc.py standalone that reproduces
   the unit's numbers byte for byte (0.09 s a candidate; confirmed against
   a real unit copy at 73, 49 and 43 rows).  EVERY NUMBER IN THIS HEADER IS
   AN ALIGNDIFF ROW COUNT, AND THE WORD COUNT IS GIVEN WHENEVER IT IS NOT
   278.

   ====================================== 2026-11-21 (session 3): 73 -> 43,
                                          AND ONE OF THE TWO DEVICES IS GONE

   THE WHOLE RESULT IS ONE LEVER APPLIED SIX TIMES: **ONE LOCAL PER SYMBOL
   LIFETIME.**  The park had one `u8 *q', one `u8 *r' and one `u16 w'
   standing for fourteen independent quantities spread over six basic
   blocks.  A C variable is one pseudo; a pseudo live in more than one block
   is a GLOBAL allocno (flow.c:2561), so `find_reg' allocates it, starting
   from `call_used_reg_set' or `fixed_reg_set' by
   `allocno_calls_crossed[allocno] == 0' (global.c:925-932) and then
   excluding `hard_reg_conflicts[allocno]' (global.c:940) -- the conflicts
   of its WHOLE live range across every block it appears in.  A quantity
   confined to one block goes to local-alloc's `find_free_reg' instead and,
   because MIPS defines no REG_ALLOC_ORDER, takes the LOWEST free register.
   That is the whole difference between the park's $s0/$a2 and the target's
   $v0/$v1.  Every one of the splits below was found by measuring the FULL
   POWER SET of its sites, and every one was re-measured after each later
   change.

   (a) `q' SPLIT, four of six sites .............................. 73 -> 62
       Own locals for case 0, case 2 and both case-17 sites; case 3 and
       case 16 KEEP SHARING `q' -- splitting either is worse.  Full 64-way
       power set, re-run on the 49-row and 43-row bodies, same winner each
       time.  Singles on the 73-row body: case 0 71 (at 277 words), case 2
       70, case 17a 70, case 17b 70, case 3 75, case 16 76.

   (b) THE FOUR STACK-PAIR STORES IN SOURCE ORDER ................ 62 -> 57
       `rc[0]; rc[1]; wh[0]; wh[1];' instead of the park's `rc[1]; wh[0];
       wh[1]; rc[0];'.  ALL 24 PERMUTATIONS measured: 43 for this one, 47
       for the next two, 49 for the park's old order on the current body.
       The park's header had read the source order off the OUTPUT's store
       order and got it backwards -- the stores are scheduled, and what the
       source order decides is which 32-bit constant is MATERIALISED first,
       hence which `lui' reorg steals into the `bnez' delay slot at word 27.

   (c) THE TWO `lbu' ARGUMENTS HOISTED OUT OF THE CALL ........... 57 -> 49
       In case 2 and case 17, `func_8009A3D0(r[0x24], r[0x25], a)' becomes
       `x = r[0x24]; y = r[0x25]; ... func_8009A3D0(x, y, a);' so the byte
       loads issue BEFORE the 0x2000 test, which is where the target has
       them (800C2BE8/800C2BEC sit above the `beqz' at 800C2BF0).
       docs/matching-levers.md, "READ A CALL'S SMALL ARGUMENTS BEFORE THE
       `if' THAT COMPUTES ITS LAST ONE" -- written against func_800AA888 and
       against a call to this same callee.
       **FOUR DISTINCT LOCALS, NOT TWO SHARED ONES.**  case 2 alone 53,
       case 17 alone 53, both through ONE shared pair 62 at 280 WORDS, both
       through `x2/y2' and `x17/y17' 49.  Declared type is irrelevant: s32,
       u8, u16 and s16 all measure the same.

   (d) `r' SPLIT FOR CASE 0 ...................................... 49 -> 53
       (at 277 words) -- WORSE ON ITS OWN AND RIGHT ANYWAY.  The target's
       case-0 `r' is $v1 and its case-2 `r' is $a1, so they cannot be one
       variable.  With the split, case 0's residue stops being structural --
       the `lui %hi(func_800CEF54)'/`addiu %lo' pair splits across two
       load-delay slots the way the target's does (800C2918 and 800C2928),
       the unfilled `nop' at word 57 goes away and a word with it -- and
       what is left is ONE register-name row per instruction.  The row count
       goes UP because eleven name rows replace six structural ones.  Keep
       it: (e) pays it back five times over.

   (e) `w' SPLIT FOR THE FIRST OF CASE 0's THREE SPRITE PAIRS .... 53 -> 43
       (at 277 words).  `w0 = *(u16 *)(r0 + 0x2);' as its own local flips
       `r0' and `w' into the target's registers ($v1 for the pointer, $v0
       for the halfword) and case 0 then matches instruction for
       instruction.  The other two pairs' `w' may be split as well with no
       further change (43 either way); one local is the smaller source, so
       only the first is split.  Splitting ONLY the second pair is 53 --
       i.e. nothing -- and only the third is 51.
       **(d) AND (e) ARE A PAIR, AND NEITHER IS VISIBLE ALONE.**  On the
       49-row body the `w0' split alone measures 49 -- nothing -- and the
       `r0' split alone measures 53, four rows WORSE.  Together they are
       43.  A single-site sweep reports one as a negative and the other as
       a harmful change, and the pair is worth six rows.  The mechanism is
       that the two quantities only exchange registers ($v1 for the
       pointer, $v0 for the halfword, as the target has them) once BOTH are
       block-local; while either is still a multi-block pseudo the
       allocator prices it over its whole-function conflict set and the
       swap cannot happen.  This is the `r'/`w' analogue of the device
       power-set lesson -- sweep the SUBSETS, not the singles.

   (f) CASE 16 READS `D_800DF55C' BACK ........................... 43 at 277
                                                             ->  44 at 278
       `D_800DF55C = *(u8 **)p; q = D_800DF55C;' instead of
       `q = *(u8 **)p; D_800DF55C = q;'.  THIS IS THE WORD EVERYTHING ELSE
       WAS SHORT.  The target spends a word on `addu $t0, $v0, $zero' at
       800C2B40 -- a copy of `*(u8 **)p' that survives into the allocator --
       and reading the global back is what produces it: cse turns the reload
       into a copy of the value just stored, and update_equiv_regs
       (local-alloc.c:1081-1093) gives it a REG_EQUIV that reload satisfies
       from the register that already holds the value, so the copy survives
       as `move $a1, $v0'.  This is docs/matching-levers.md's "read back the
       FIELD you just stored, not the source", and it is the ONLY one of
       TWELVE shapes tried that produces the copy -- see the rejected list.

   (g) `r' SPLIT FOR CASE 16 ..................................... 44 -> 43
       Once (f) exists, case 16's `r' wants its own local too.  Re-measured
       power set of the three remaining `r' sites: case 16 alone 43, case 2
       alone 51, case 17 alone 51, case 16 + case 17 57.

   (h) THE CANCELLED PAIR `r++; r--;' IS NOW DEAD -- DELETED.
       It was refsweep's find in the previous session and it was worth 5
       rows on the 78-row body.  On this body it is worth ZERO: the device
       power set measures 43/278 with it and 43/278 without it, in all four
       combinations with the other device.  **A device that was load-bearing
       at the start of the day was lifting a tie that (a), (f) and (g) now
       resolve honestly.**  That is the clearest argument in this park for
       re-ablating applied changes after every landing.

   ====================================== DEVICE ABLATION (THE POWER SET)

   Run at 73 rows (2 devices, 4 bodies), at 62, at 49, and again on this
   body crossed with {`goto shared', duplicated tail}:

       body 73 rows:  D1+D2 73 | D1 75/281 | D2 78 | neither 80/281
       body 62 rows:  D1+D2 62 | D1 64/281 | D2 67 | neither 69/281
       body 49 rows:  D1+D2 49 | D1 51/281 | D2 54 | neither 56/281
       body 43 rows:  D1+D2 43 | D1 46/281 | D2 43 | neither 46/281
                                                 ^^^^ D1 free -> deleted

   ONE DEVICE REMAINS.  D2 is `f = D_800814A0 | 0x8000;' written ABOVE
   `q = *(u8 **)p;' and the `*(u16 *)(q - 0x2) |= 0x8000;' store in case 3.
   Its device-free alternative is the plain order
         q = *(u8 **)p;
         *(u16 *)(q - 0x2) |= 0x8000;
         f = D_800814A0 | 0x8000;
         D_800814A0 = f;
   and ITS COST TODAY IS 46 ROWS AT 281 WORDS.

   **AND THE DEVICE-FREE BODY IS THE STRUCTURALLY CORRECT ONE.**  With D2
   OFF, case 3 comes out instruction for instruction right -- `lui
   %hi(D_800814A0)' in the `beqz' delay slot at 800C2AFC, the two load-delay
   `nop's at 800C2B04/800C2B0C, and the `lw %lo' / `nop' / `ori' trio after
   the `sh' -- and every row there is a register NAME.  The three extra
   words it costs are identified and are all in the TAIL, not in case 3:
     * `lui %hi(D_800814A0)' re-materialised inside the shared block
       (built word 271) because `goto shared' makes a real RTL join;
     * a load-delay `nop' after `lhu $v0, 0x1E($a1)' that the target fills
       with `ori $v1, $v1, 0x8000';
     * `f' landing in $a2 instead of $v1, so case 3's tail and case 17's
       tail differ and jump2 cannot cross-jump them.
   So D2 is propping up the cross-jump as well as case 3's `lui', which is a
   second reason to distrust it.  The aliasing question the park raised
   before (can `*(u8 **)p - 2' be `&D_800814A0'?) is still open; `p's +0 is
   the actor's primitive-buffer pointer and D_800814A0 is a resident-engine
   word, so it is implausible, but it is still not proven.

   ====================================== `goto shared' vs. WRITING IT TWICE

   The target has NO join at .L800C2C48.  It has TWO copies of
       *(u16 *)(p - 0x2) |= 0x8000;  D_800814A0 = f;
   that **jump2 cross-jumped** -- jump_optimize runs at toplev.c:3690, after
   global_alloc (:3617) and sched2 (:3661), so the two tails only have to
   agree in HARD REGISTERS, and the target's do ($v1 for `f', $a0 for the
   `%hi').  Writing the tail out twice and deleting the label:
       duplicated tail, D2 on ... 43 at 277   (one word SHORT)
       goto shared,    D2 on ... 43 at 278   <-- live body
       duplicated tail, D2 off .. 50 at 285   (no cross-jump at all)
       goto shared,    D2 off ... 46 at 281
   The duplicated tail buys exactly the shared block's `lui' and nothing
   else.  The full {D2} x {goto,dup} x {64 q-splits} grid (256 bodies) was
   measured at the 49-row stage and nothing in it beat the live shape.

   ====================================== BLOCK ORDER IS RIGHT

   Checked against the listing's addresses, pairing branches by ordinal:
   every `R' row is a same-index or within-2 row, the four `.L800C2C5C'
   branches drift by at most 2 words, and the drift at each branch tracks
   the drift at its destination.  The arm order (0, 1, default, 2, 3, 16,
   17) is confirmed; moving `default:' to six other positions measures 43 or
   worse (52 after case 0).  This is NOT a func_800C4F20-style mis-ordered
   park, and the two block-order moves that function needed have no
   counterpart here.

   ====================================== WHAT IS LEFT -- 43 ROWS, 4 CLUSTERS

   1. CASE 3's ARGUMENT BLOCK, 9 rows (built 162-179).  THE SAME TWENTY
      INSTRUCTIONS AS THE TARGET IN A DIFFERENT ORDER.  The target computes
      `&D_80082E80' FIRST (800C2AA8/800C2AAC, and note `addiu $a1, $v0,
      %lo' -- the lo_sum's output register differs from its input) and
      issues the two `lbu's LAST (800C2AD0/800C2AD4), after the
      `sw %lo(D_800DF55C)'; we compute the address in the middle and put the
      `lbu's immediately behind it.  Consequently 0x208020 and the
      `D_800E3D7C' value have their registers SWAPPED ($t0/$v1 against the
      target's $v1/$t0).
      THIS IS sched1, not aliasing and not spelling.  The `.sched' trace for
      the block shows both `lbu's entering the ready list at LAUNCH_PRIORITY
      (0x7f000001, sched.c:189, assigned at sched.c:3998) at T-18 and T-17
      of a 24-cycle BACKWARD schedule, so they are emitted as soon as they
      become ready, which is early in the stream.  Nothing in the source
      moves them; all of the following measure 43/278 unchanged:
        - seven spellings of the first two arguments: `D_80082E80[0x24]',
          `*(D_80082E80 + 0x24)', `*(u8 *)((u8 *)D_80082E80 + 0x24)',
          `*(u8 *)((u8 *)&D_80082E80 + 0x24)', and the same through a
          `u8 *ba = D_80082E80;' local placed before or after the
          `D_800DF55C' store;
        - `D_800DF55C = D_800E3D7C;' (unifying the two spellings of the same
          global -- levers item 7) and a `u8 *e3' local for it;
        - hoisting the first two arguments into `x3'/`y3' locals, the 0x88
          load into `h3', and both;
        - a `s32 k = 0x208020;' local for the sixth argument;
        - `(void *)' and bare spellings of the seventh argument, `(s32)' on
          the sixth, `(s16 *)' on the 0x88 load;
        - lifting the call's result into `i' with a separate `if (i != 0)';
        - the case-16 READ-BACK trick applied here too -- reading
          `*(u16 *)(D_800DF55C + 0x88)' after the store instead of
          `*(u16 *)(D_800E3D7C + 0x88)', with and without a `q3' local for
          it and with and without a `ba' local for D_80082E80;
        - **THE WHOLE ARGUMENT LIST HOISTED INTO LOCALS IN THE TARGET'S OWN
          EMITTED ORDER** -- `ba = D_80082E80; k = 0x208020;
          dp = &D_800DF45C;' then the `D_800DF55C' store then the call --
          and each pair of that separately.  43/278, every one.  Source
          order has no purchase on this block at all.
      `h3' hoisted ABOVE the `D_800DF55C' store is 53 at 280 -- the only
      spelling in the family that moves anything, and the wrong way.
      **AND IT IS NOT MEM_IN_STRUCT_P**, which was the obvious lever and is
      now a measured negative.  `D_80082E80[0x24]' builds `(mem/s:QI ...)'
      because expr.c:5344's INDIRECT_REF arm sets MEM_IN_STRUCT_P whenever
      `TREE_OPERAND (exp, 0)' is a PLUS_EXPR.  Writing the two loads
      `*(u8 *)((s32)&D_80082E80 + 0x24)' puts a NOP_EXPR on top of the
      addition -- a cast that does NOT fold away, unlike `(u8 *)((u8 *)X +
      K)', which does -- and the `.rtl' dump then shows a plain `(mem:QI
      (reg:SI 170))' with the flag CLEARED.  The schedule does not change:
      43/278 for the int cast, the u32 cast, the reversed operand order, the
      `(s32)D_80082E80' form and either load alone.  The reason is that the
      dependence was never there to break -- at sched1 the lbu's address is
      a `lo_sum' on D_80082E80 and the store's is a `lo_sum' on
      D_800DF55C, so `memrefs_conflict_p' answers "no conflict" on the
      symbols alone and the flag is never consulted.
      BEST THEORY: this needs a priority argument of the sched.c:2583
      (`adjust_priority'/`birthing_insn_p') kind, or the permuter.

   2. CASE 3's `D_800814A0' BLOCK, 7 rows (built 182-189 plus the three
      target-only words at 800C2B18-800C2B20).  This is device D2's doing
      and it is discussed above: with the device off the block is right and
      three words appear in the tail instead.  The row count prefers the
      device by 3 and the word count by 3; the STRUCTURE prefers no device.

   3. CASE 16's INTERNAL ORDER, 6 rows (built 197-206).  The copy is now
      there (`move $a1, $v0' against `addu $t0, $v0, $zero'), but the target
      then does `lw $a1, -0x14($t0)' / `lh $a2, 0x88($t0)' / `lui %hi' /
      `sw %lo(D_800DF55C)' and we do `sw %lo(D_800DF55C)' / `lw -0x14' /
      `lh 0x88'.  Our copy lands in $a1 and the `-0x14' load then overwrites
      it, which is what forces our order; the target's copy is in $t0 and
      survives both loads.
      Measured and rejected on this body, all 43/278: hoisting the 0x88 load
      into a local, hoisting the two `lbu's, hoisting both, and dropping the
      cancelled pair (which is how (h) was found).

   4. THE SHARED TAIL, 5 rows (built 256-269) AND THE ONE WORD IN CASE 2.
      The tail's `lui v0,0x0' at built 268 is the `goto shared' join; see
      above.  Reordering the three tail statements is 47/277 at best
      (`*(u16 *)(D_800DF560 + 0x1E) |= 0x8000;' in the middle), 53/281
      first; a `void *df = D_800DF560;' local is 48.
      CASE 2's EXTRA WORD (built 124): the target folds the symbol into the
      store -- `lui $a0, %hi(D_80010A93)' ... `addu $v0, $v0, $a0' ...
      `sb $zero, %lo(D_80010A93)($v0)' -- and we materialise the whole
      address with `lui'+`addiu %lo' and then add the index.  Measured and
      rejected, every one 43/278: `(&D_80010A93)[j * 0x54]',
      `*(&D_80010A93 + j * 0x54)', `*(u8 *)((u8 *)&D_80010A93 + j * 0x54)',
      `((u8 *)&D_80010A93)[j * 0x54]', `j * 84' written decimal, `(u32)j'
      for the index, and all of those with the declaration changed to
      `extern u8 D_80010A93[];', `extern u8 D_80010A93[0x54 * 0x40];',
      `extern u8 D_80010A93[][0x54];' (with `D_80010A93[j][0]') and a 0x54
      byte struct array (with `D_80010A93r[j].u0').  Hoisting `j * 0x54'
      into a local is 59; swapping the two stores inside the `if' is 66.
      WHY NONE OF THEM REACH IT: `move_operand' (mips.c:712-719) rejects a bare
      SYMBOL_REF as a move source whenever `mips_split_addresses' is on, so
      the address is always expanded as `high' + `lo_sum' INTO A REGISTER
      and the index is added afterwards (visible in the `.rtl' dump,
      insns 263/264 before the index chain).  LEGITIMIZE_ADDRESS
      (mips.h:2652) can only put the `lo_sum' inside the MEM through
      `mips_check_split' (mips.c:884), which accepts a bare SYMBOL_REF, a
      CONST(PLUS(SYMBOL_REF, CONST_INT)) or a LABEL_REF and NOTHING ELSE --
      a `symbol + register' address can never take that arm.  The target's
      form therefore arrives by some other route, and finding which pass
      does it is the open question for this word.
      A NEGATIVE WORTH KEEPING, because it looked like a lever for ten
      minutes: `mips_check_split' DOES accept
      `CONST(PLUS(SYMBOL_REF, CONST_INT))', so writing a CONSTANT-offset
      extern access as `*(T *)((s32)&SYM + K)' -- an integer cast that does
      not fold away, unlike `(u8 *)((u8 *)SYM + K)' -- really does change
      the RTL: the address becomes a bare CONST, LEGITIMIZE_ADDRESS's LO_SUM
      arm fires, and `.rtl' shows `(mem:QI (reg))' with MEM_IN_STRUCT_P
      cleared instead of `(mem/s:QI (plus (reg) (const_int 36)))'.
      IT IS WORTH NOTHING.  A two-function micro-test compiled with
      tools/cdkgcc -O2 -G0 gives `lui $2, %hi(SYM+36)' / `lbu $3,
      %lo(SYM+36)($2)' for BOTH spellings -- gcc already folds a lone
      constant offset into the relocation.  The two spellings only diverge
      when SEVERAL constant offsets of one symbol are live at once, and then
      the ARRAY spelling is the cheaper one, because cse shares a single
      `lui'/`addiu %lo' base and addresses the rest by displacement while
      the cast spelling makes each offset its own relocation.

   ====================================== SWEEPS -- ALL FOUR CLEAN ON *THIS*
                                          BODY (devsweep's only find is the
                                          `if (q)' device two sections down)

   refsweep.py ... CLEAN ON THIS BODY.  Run twice: at 49 rows (19 names,
     2527 insertion points) and again on this body (22 names, 2960
     insertion points) -- "final 43 rows, 278/278 words, no cancelled pair
     helps" both times.  Consistent with (h): the one cancelled pair this
     park used to carry is now worth zero.
   widthsweep.py . clean at 49; and by hand on this body every one of `i',
     `j', `a', `f', `v', `w', `w0', `x2', `y2', `x17', `y17' through
     s8/u8/s16/u16/s32/u32 -- nothing under 43.  The tool's own re-run on
     THIS body agrees: "final 43 rows, 278/278 words, no width change
     helps".
   stmtsweep.py .. clean at 49 ("no safe move helps"), and a gated
     hand-rolled version is clean on this body over six rounds.
   devsweep.py ... run twice, on the 49-row body (47, `if (q)') and again
     on THIS body (41, the same `if (q)' around the same statement).  Not
     taken -- see the section above for why, and for the exhaustive scan
     that shows no defined condition reaches it.
   DECLARATION ORDER ... clean.  All 306 single-declaration moves measured
     at the 49-row stage; best was the order in the body.
   ARRAY LENGTHS ....... clean.  rc[2..5] x wh[2,3,4,6]: rc[2]/wh[2] is best;
     rc[2]/wh[3] and rc[2]/wh[4] tie, everything with rc[3] or more is 64+.
   `a' AND THE REMAINING `w' LIFETIMES ... clean.  The 32-way power set of
     {case-2 `a', case-17 `a', case-0 pair 2 `w', case-0 pair 3 `w',
     case-17 `w'} is 43 everywhere -- no further split pays.

   ====================================== THE ONE DEVICE THIS PARK DOES NOT
                                          TAKE: `if (q)' FOR 2 ROWS

   devsweep.py's duplicated-arm device around case 0's early exit --

       if ((*(u32 *)(*(u8 **)p + 0x14) & 0x4000) == 0)
       {
           if (q)  { *(s16 *)(p + 0x4) = 0x10; }
           else    { *(s16 *)(p + 0x4) = 0x10; }
           return;
       }

   -- measures **41 rows at 278 words on this exact body**, i.e. two rows
   better than the body below.  IT IS NOT IN THE LIVE BODY because `q' is
   not assigned on that path (it is only set in case 3 and case 16), so the
   condition reads an indeterminate pointer.  Both arms are identical, so
   this cannot change what the function computes -- it is the sanctioned
   free-reference device, not the uninitialised-read DEFECT the three
   rejected finds above are -- but this park has now lost three candidates
   to that shape and it did not seem right to add a fourth that looks like
   one.  **Take the two rows if you want them; the body is otherwise
   unchanged.**

   NO DEFINED CONDITION REACHES IT, and that was measured EXHAUSTIVELY: the
   duplicated arm was tried around EVERY one of the 89 statements with EVERY
   one of the 19 locals and the three parameters as the condition (about
   1900 bodies), plus `do { } while (0);' around every statement.  EXACTLY
   FOUR bodies come in under 43, all of them 41, and all four have `q' as
   the condition -- around `*(s16 *)(p + 0x4) = 0x10;' (case 0),
   `i = func_800A32A4(*(u8 **)p);' (case 2), `func_800A32A4(*(u8 **)p);'
   (case 17) and `w = *(u16 *)(p + 0x6) - 1;' (case 1/17).  `q' is assigned
   only in case 3 and case 16, so every one of the four reads it in a block
   where it is not defined.  The two rows ARE the extra reference to `q'
   from a block that does not define it, and nothing else buys them.  `if (p)',
   `if (c)', `if (*p)' and `if (p[0])' around that same statement are all
   43.  Giving `q' a real definition in case 0 instead does not work either:
   `q = *(u8 **)p;' at the 0x4000 test is 45, `q0 = q - 0x20;' with that
   definition is 43, defining `q' inside the early-exit block and then using
   the arm is 45, and the arm with `q' defined at the test is 45.  The two
   rows are specifically the cost of `q' being referenced in a block it is
   not defined in.

   ====================================== A SECOND UNINITIALISED-READ FIND,
                                          THIS TIME FROM A SWEEP

   An UNGATED statement-move sweep reports 47 rows at 278 words on the
   49-row body for case 16 written

       case 16:
           r = *(u8 **)(q - 0x14);     <-- READS `q' BEFORE IT IS SET
           q = *(u8 **)p;

   which is the same defect as the permuter find this park rejected in the
   previous session, found by a different tool.  `q' is only assigned in
   case 3, which case 16 cannot reach.  REJECTED.  With a def-use gate (a
   statement may not cross one that writes or reads a local it writes, or
   that writes a local it reads) the same sweep is clean.
   **ANY 47-OR-BETTER REPORTED FOR THIS FUNCTION SHOULD BE READ FOR THIS
   SHAPE BEFORE IT IS BELIEVED** -- that is now three finds on this one
   park, from three different tools.

   ====================================== STRUCTURE THAT IS ALREADY RIGHT
                                          (read off the listing, re-confirmed)

     * case 0 FALLS THROUGH into case 1, and case 1 falls through to the
       default `return';
     * cases 3 and 17 share the `|= 0x8000' tail and cases 1, 2 and 16 share
       `*(u16 *)(p + 0x4) += 1; return;' -- both are jump2 cross-jumps;
     * `(s16)func_80042900(...) == 0' for the 0x1B arm, which is what gives
       the `sll 16' before the test;
     * the 0x3000/0x300 pick for func_8009A3D0's third argument is a plain
       `if' on `*(u32 *)(q + 0x1C) & 0x2000', in both case 2 and case 17;
     * the DOUBLED `D_800814A0 = f;' is real -- the target stores the same
       $v1 at 800C2C3C and again at 800C2C58;
     * THE STACK PAIRS MUST BE ARRAYS.  Written as four scalars nothing in
       this function ever reads sp24 or sp2A, so gcc allocates only the two
       addressable scalars and DELETES both stores as dead: 280 words / 105
       rows, with a 0x40 prologue where the target has 0x48.  Four scalars
       plus an unreferenced `s32 unused[2];' buys the frame and leaves the
       stores deleted (280 / 88, and ALL NINE declaration positions measure
       88 -- the tell that the aggregate is wrong).  As `s32 rc[2];
       s16 wh[2];' the four stores are live and the frame comes out 0x48 by
       itself.

   ====================================== EARLIER SESSIONS (kept)

   2026-11-20: case 3 inlining the shared tail cost four words; `goto
     shared' took 285 -> 281.
   2026-11-21 (earlier): refsweep's `r++; r--;' took 78 -> 73 and
     stmtsweep's D2 move took 281/80 -> 278/78.  A 75-row permuter find at
     279 words was REJECTED for hoisting `D_80082E80' into a local whose
     assignment sits after a `return;' -- unreachable, so the local is read
     uninitialised.  The same hoist in the reachable position measured 80,
     i.e. nothing.  A `u32 *fa = &D_800814A0;' pointer local measured
     283/87, 284/83 and 285/87 at three positions, all worse.
   THOSE NUMBERS ARE SCOPED TO BODIES THAT NO LONGER EXIST.  Everything
   above the "EARLIER SESSIONS" line was measured on the body below.

   DECLARATIONS
   void func_800C27F0();
   void func_800C0180();
   void func_800CEF54();
   extern u8 D_80010A93;
   extern u8 D_800E1640[];
   END DECLARATIONS
   (func_800C27F0 is in the HEAD unit and func_800CEF54 in
   ovl_dungeon_800CABA8, so both are cross-unit references now and need a
   declaration where a single-unit build needed none.  Everything else --
   D_800DF45C, D_800DF4A4, D_800DF55C, D_800DF560, D_800E3D7C, D_80082E80,
   D_800814A0 -- is already in csrc/ovl_dungeon_decls.h.)
*/
void func_800C2824(u8 *p, u8 *b, u8 *c)
{
    u8 *q;
    u8 *q0;
    u8 *q2;
    u8 *q17;
    u8 *q17b;
    u8 *r;
    u8 *r16;
    u8 *r0;
    s32 i;
    s32 j;
    s32 rc[2];
    s16 wh[2];
    s32 a;
    s32 x2;
    s32 y2;
    s32 x17;
    s32 y17;
    u32 f;
    u8 v;
    u16 w;
    u16 w0;

    switch (*(s16 *)(p + 0x4))
    {
    case 0:
        if ((*(u32 *)(*(u8 **)p + 0x14) & 0x4000) == 0)
        {
            *(s16 *)(p + 0x4) = 0x10;
            return;
        }
        rc[0] = 0x1800340;
        rc[1] = 0x400040;
        wh[0] = 0x360;
        wh[1] = 0x1B8;
        func_800B8FC8(*(u8 **)p, rc, (s32 *)wh, 1, 2);
        q0 = *(u8 **)p - 0x20;
        func_80044A50(q0);
        func_800BC318(q0);
        r0 = *(u8 **)(q0 + 0x8);
        w0 = *(u16 *)(r0 + 0x2);
        *(u16 *)(b + 0x2) = w0;
        *(u16 *)(b + 0xE) = w0;
        w = *(u16 *)(r0 + 0x6);
        *(u16 *)(b + 0x6) = w;
        *(u16 *)(b + 0x12) = w;
        w = *(u16 *)(r0 + 0xA);
        *(u16 *)(b + 0xA) = w;
        *(u16 *)(b + 0x16) = w;
        func_8004491C(p - 0x20, (u32)func_800CEF54);
        *(u16 *)(c + 0x10) = 0x20;
        *(u8 **)(c + 0x8) = (u8 *)&D_800DF4A4;
        *(s32 *)(c + 0xC) = 0x808080;
        *(u16 *)(c + 0x1E) = 0x1000;
        *(u16 *)(c + 0x1C) = 0x1000;
        *(u16 *)(c + 0x14) |= 0xC;
        *(u16 *)(p + 0x6) = 0x10;
        func_800A56E0(1);
        *(u16 *)(p + 0x4) += 1;
    case 1:
        v = c[0xC];
        v = v - (s32)v / *(s16 *)(p + 0x6);
        c[0xC] = v;
        c[0xE] = v;
        c[0xD] = v;
        w = *(u16 *)(p + 0x6) - 1;
        *(u16 *)(p + 0x6) = w;
        if ((s32)(w << 16) <= 0)
        {
            *(u16 *)(p + 0x4) += 1;
        }
        return;
    default:
        return;
    case 2:
        i = func_800A32A4(*(u8 **)p);
        if (i != 0)
        {
            j = func_800A6620(i, 0);
            if (j < 0x40)
            {
                *(u8 *)(j * 0x54 + (s32)&D_80010A93) = 0;
                *(u32 *)(0x80010980 + j * 4) = 0;
            }
        }
        if ((s16)func_80042900(*(u8 **)p, 0x1B) == 0)
        {
            q2 = *(u8 **)p;
            r = *(u8 **)(q2 - 0x14);
            x2 = r[0x24];
            y2 = r[0x25];
            a = 0x3000;
            if ((*(u32 *)(q2 + 0x1C) & 0x2000) != 0)
            {
                a = 0x300;
            }
            func_8009A3D0(x2, y2, a);
        }
        func_8009A028(*(u8 **)p);
        func_80099844(*(u8 **)p, &D_800E1640);
        *(u16 *)(p + 0x4) += 1;
        return;
    case 3:
        D_800DF55C = *(u8 **)&D_800E3D7C;
        if (func_800BBA40(D_80082E80[0x24], D_80082E80[0x25], *(u16 *)(D_800E3D7C + 0x88),
                          &D_800DF45C, 0x2800, 0x208020, (u32)func_800C0180) != 0)
        {
            f = D_800814A0 | 0x8000;
            q = *(u8 **)p;
            *(u16 *)(q - 0x2) |= 0x8000;
            D_800814A0 = f;
            goto shared;
        }
        break;
    case 16:
        D_800DF55C = *(u8 **)p;
        q = D_800DF55C;
        r16 = *(u8 **)(q - 0x14);
        if (func_800BBA40(r16[0x24], r16[0x25], *(u16 *)(q + 0x88), &D_800DF45C, 0x2800, 0x208020,
                          (u32)func_800C27F0) != 0)
        {
            *(u16 *)(p + 0x6) = 0x3C;
            *(u16 *)(p + 0x4) += 1;
            return;
        }
        break;
    case 17:
        w = *(u16 *)(p + 0x6) - 1;
        *(u16 *)(p + 0x6) = w;
        if ((s32)(w << 16) <= 0)
        {
            func_800A32A4(*(u8 **)p);
            if ((s16)func_80042900(*(u8 **)p, 0x1B) == 0)
            {
                q17 = *(u8 **)p;
                r = *(u8 **)(q17 - 0x14);
                x17 = r[0x24];
                y17 = r[0x25];
                a = 0x3000;
                if ((*(u32 *)(q17 + 0x1C) & 0x2000) != 0)
                {
                    a = 0x300;
                }
                func_8009A3D0(x17, y17, a);
            }
            func_8009A028(*(u8 **)p);
            q17b = *(u8 **)p;
            *(u16 *)(q17b - 0x2) |= 0x8000;
            f = D_800814A0 | 0x8000;
            D_800814A0 = f;
            *(u16 *)(D_800DF560 + 0x1E) |= 0x8000;
        shared:
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 = f;
        }
        break;
    }
}
