/* PARK: func_800C2824 -- 278/278 words, 27 ALIGNED ROWS.
   Opened at 285/84.  Sessions: 84 -> 73 -> 43 -> 27.

   Measured with
       python3 tools/decomp/parkresidue.py func_800C2824
   and, for every sweep below, on an onefunc.py standalone that reproduces
   the unit's numbers byte for byte (0.09 s a candidate; confirmed against a
   real unit copy at 73, 49, 43, 35 and 32 rows).  EVERY NUMBER IN THIS
   HEADER IS AN ALIGNDIFF ROW COUNT, AND THE WORD COUNT IS GIVEN WHENEVER IT
   IS NOT 278.  (`ovlverify' reports 145 differing on this body; that is the
   word-against-word count the brief warns about, inflated by transposition.
   The honest aligned figure is 32.)

   ====================================== 2026-11-22 (session 4): 43 -> 27
                                          IN TWO STEPS, 43 -> 32 BY HAND AND
                                          32 -> 27 FROM THE PERMUTER

   The hand result is the `ba'/`e3' cancelled pairs below (43 -> 32).  The
   permuter, run for 40 minutes on the 32-row body with 5 jobs, then found
   one further change worth 5 rows.  BOTH SECTIONS MATTER; read the pairs
   first, because the permuter's find only works on top of them.

   ====================================== THE PERMUTER's FIND: 32 -> 27,
                                          `q' DELETED AND `e3' REUSED

   `u8 *q;' is GONE.  Its three sites now use `e3', the local that case 3
   already introduced for the `*(u8 **)&D_800E3D7C' value:

       case 3:  ... e3 = *(u8 **)&D_800E3D7C; e3++; e3--;
                    D_800DF55C = e3;
                    if (func_800BBA40(..., *(u16 *)(e3 + 0x88), ...) != 0)
                    {
                        f = D_800814A0 | 0x8000;
                        e3 = *(u8 **)p;             <-- WAS `q'
                        *(u16 *)(e3 - 0x2) |= 0x8000;
       case 16: D_800DF55C = *(u8 **)p;
                e3 = D_800DF55C;                    <-- WAS `q'
                r16 = *(u8 **)(e3 - 0x14);
                if (func_800BBA40(r16[0x24], r16[0x25], *(u16 *)(e3 + 0x88),

   GATE 1, MEASURED: 27 rows at 278/278 words.  And the permuter's own score
   is MISLEADING here exactly as the brief warns -- base=1130, best=910, but
   output-910-1 measures 41 rows at 279 WORDS while output-965-1 is the
   27-row body.  The eight best-scoring finds measured 41/279, 27, 30, 31,
   32, 32, 36 and 42; the score and the row count are not monotonic.

   GATE 2, READ FOR THE UNINITIALISED-READ SHAPE THIS PARK HAS REJECTED
   THREE TIMES -- AND IT IS CLEAN.  `e3' now has two unrelated lifetimes in
   case 3, so this needed checking carefully:
     * case 3 assigns `e3' the D_800E3D7C pointer, and the only read of that
       value, `*(u16 *)(e3 + 0x88)', is an ARGUMENT of func_800BBA40 and is
       therefore fully evaluated before the if-body runs.  The reassignment
       `e3 = *(u8 **)p;' inside the if-body cannot be seen by it.
     * case 16 assigns `e3' before both of its reads.
     * the `shared:' tail is reached from case 3 (after the reassignment) and
       by fall-through from case 17 (where `e3' is never assigned) -- but the
       tail does NOT READ `e3' at all (`*(u16 *)(p - 0x2) |= 0x8000;
       D_800814A0 = f;'), so the case-17 path never observes it.
     * `e3++; e3--;' stays a no-op.
   Nothing the function computes changes, and no path reads `e3' before it is
   written.  THIS FIND IS CORRECT -- unlike the three this park rejected.

   GATE 3, SPLIT.  The find is one textual idea but two independent sites,
   and they are NOT independently good:
       case 16's `q' -> `e3' alone .......... 29 at 278
       case 3's `q' -> `e3' alone ........... 40 at 277   (harmful, and short)
       both (the find) ...................... 27 at 278   <-- live body
   So the case-3 half alone is four rows worse than doing nothing and loses a
   word, and it is still needed for the best score.  Another instance of the
   park's standing lesson: sweep the SUBSETS, not the singles.

   WHY IT WORKS, AND HOW IT SQUARES WITH THE `q' PARTITION NEGATIVE BELOW.
   Merging `q' into `e3' gives case 16's pointer a pseudo that is referenced
   in several blocks AND set four times (the pair, the case-3 reassignment,
   the case-16 assignment), so local-alloc hands it to `find_reg', which
   prices it over its whole-function conflict set and gives it **$t0 -- the
   target's own register for it** (800C2B40).  Case 16's `move $a1, $v0' /
   `sw $a1' rows and the copy row all vanish; the arm goes from 8 rows to 4.
   The cost is case 3, where the target has this quantity in $v1 (800C2B00)
   and we now have $t0 -- which is exactly why the case-3 half alone is
   worse, and it is a real residue, not an artefact.
   THIS DOES NOT CONTRADICT the 52-body `q' partition negative recorded
   below: that sweep partitioned the SIX `*(u8 **)p' SITES among themselves,
   and merging one of them with a local holding a COMPLETELY DIFFERENT VALUE
   (the D_800E3D7C pointer) was outside its space entirely.  The lesson for
   the next park is that a "one local per symbol lifetime" power set does not
   cover MERGES ACROSS unrelated values, and the permuter is good at exactly
   that move.
   RE-SWEPT AFTERWARDS: the 24-body case-16 form sweep (forms A/B/C/D x
   {no pair, pair after the copy, pair after the `-0x14' load} x {0x88 load
   inline, hoisted}) run again on THIS body still puts form A -- the body as
   it stands -- first at 27; form C is 31 at 277 and forms B and D are worse.

   ====================================== THE HAND RESULT: 43 -> 32,
                                          CASE 3's ARGUMENT BLOCK IS NOW
                                          INSTRUCTION-FOR-INSTRUCTION RIGHT

   TWO CHANGES, BOTH THE SAME LEVER, WORTH 8 AND 5 ROWS (11 TOGETHER):

       case 3:
           ba = D_80082E80;
           ba++; ba--;                      <-- THE CANCELLED PAIR IS THE LEVER
           e3 = *(u8 **)&D_800E3D7C;
           e3++; e3--;                      <-- AND AGAIN, FOR 3 MORE ON TOP
           D_800DF55C = e3;
           if (func_800BBA40(ba[0x24], ba[0x25], *(u16 *)(e3 + 0x88), ...

   with `u8 *ba;' and `u8 *e3;' declared among the locals.

   THE 2x2 GRID, EVERY CELL AT 278 WORDS:
       both locals, NEITHER pair ............................... 43
       `ba' pair only .......................................... 35
       `e3' pair only -- with the `ba' local present, and also with
            no `ba' local at all and `D_80082E80[0x24]' inline .. 38
       both pairs .............................................. 32  <-- live
   So the two are largely independent, 8 rows and 5 rows, 11 together, and
   neither needs the other to be visible.  WHAT IS WORTH NOTHING IS THE LOCAL
   WITHOUT THE PAIR: `ba' and `e3' as plain locals with no second set measure
   43, exactly the park's old score, which is why the previous session's
   `u8 *ba = D_80082E80;' test came back a negative.  The whole payment is
   the SECOND SET.  `e3' with a DOUBLED pair (`e3++; e3--; e3++; e3--;') is
   also 32, so one pair is enough.

   WHY, AND WHY NEITHER SWEEP COULD FIND IT.  The previous session diagnosed
   this block correctly as sched1 and then measured about twenty-five source
   spellings against it, every one 43/278, INCLUDING a plain
   `u8 *ba = D_80082E80;' local placed before and after the `D_800DF55C'
   store.  The missing ingredient is the SECOND SET.

   `priority()' (sched.c:1455-1529) is the longest WEIGHTED path from the
   block START: `priority(insn) = max over LOG_LINKS of priority(pred) +
   insn_cost(pred) - 1', floored at 1.  Because `insn_cost' is 1 for
   everything but a load, a chain of ordinary insns stays at priority 1 and
   only a LOAD in the chain bumps it.  On the 43-row body that gave:

       382/387 (the two `lbu's)      priority 1   (pred is a plain lo_sum)
       412/414 (moves into $a0/$a1)  priority 2   (pred is a lbu, cost 2)
       376 (sw %lo(D_800DF55C))      priority 2
       401/405/410 (the stack args)  priority 2
       416 (move of the `lh' into $a2) priority 3

   and `schedule_select' resolves a priority tie by `potential_hazard', which
   a MEMORY insn always wins against a register move.  So 412 and 414 lost to
   401, 405, 410 and 376 at T-4, T-7, T-8 and T-11 of a 24-cycle BACKWARD
   schedule, were finally chosen at T-16/T-15, and the two `lbu's were then
   dragged immediately in front of them by the load-delay LAUNCH mechanism --
   which is why they came out at stream position 7 and 8 instead of the
   target's 11 and 12.  NOTHING IN THE SOURCE ORDER TOUCHES THAT, which is
   exactly what the twenty-five negatives were saying.

   What the cancelled pair changes is ALIAS ANALYSIS, not order directly.
   `init_alias_analysis' (sched.c:440) builds `reg_known_value' from the
   REG_EQUIV/REG_EQUAL note of a pseudo set EXACTLY ONCE.  With one set `ba'
   carries `REG_EQUIV (symbol_ref D_80082E80)', `canon_rtx' replaces the base
   with the bare symbol, and `memrefs_conflict_p' then disambiguates both
   `lbu's against every stack slot and against `%lo(D_800DF55C)' on the
   symbols alone -- so the `lbu's have NO dependence on the store or on the
   three stack stores and float to the front.  `ba++; ba--;' makes
   `reg_n_sets' 2, the note is no longer trusted, the base is an opaque
   pseudo, and the real dependences appear.  The pair itself compiles to
   nothing (combine folds the two addiu), so it costs no words.

   CONFIRMED IN THE DUMPS, both ways round (`tools/decomp/sched.py <file>
   func_800C2824 --sched1', then read the `sched' dump's RTL):
     * NO PAIRS.  Case 3's three stack stores to sp+16/sp+20/sp+24 have
       LOG_LINKS `(REG_DEP_ANTI <the lh>)' and their own value producer, AND
       NOTHING ELSE -- the two `lbu's are absent from all three lists, so
       sched1 is free to float them to the front of the block.  (Contrast
       case 16's two stack stores in the same dump, which carry FIVE
       anti-dependences each, because case 16's loads go through `q', an
       opaque pointer that cannot be disambiguated.)
     * BOTH PAIRS.  The same three stores now carry
       `(REG_DEP_ANTI <lbu 0x24>) (REG_DEP_ANTI <lbu 0x25>)
        (REG_DEP_ANTI <lh>)', and each `lbu' itself gains a LOG_LINK on the
       `sw %lo(D_800DF55C)'.  That is the dependence that pins the `lbu's
       behind the store, which is where the target has them.
     * AND THE `lbu's ARE STILL `mem/s' IN BOTH DUMPS, with their base now a
       `(reg/v:SI ...)' user-variable pseudo.  MEM_IN_STRUCT_P is unchanged
       by the lever, which closes the previous session's MEM_IN_STRUCT_P
       hypothesis from the other direction: the flag is set in the WINNING
       body too, so it was never what suppressed the dependence.

   THE MECHANISM WAS ALREADY IN THE CATALOGUE, AND BOTH EARLIER SESSIONS HAD
   IT IN FRONT OF THEM.  docs/matching-levers.md, "A SECOND SET ON A POINTER
   LOCAL TURNS ITS STORES INTO ALIASING STORES" (func_800C0404, 2026-11-16),
   quotes the `sched.c:440' guard verbatim.  What hid it is the FRAMING: that
   entry is about a pointer local's own STORES floating across `%lo' loads,
   and this residue is the mirror image -- a pointer local's LOADS floating
   across an unrelated symbol's store.  The park's previous session reached
   for the neighbouring entry instead ("MEM_IN_STRUCT_P decides sched1's
   alias calls") and spent its budget on a flag that is set in the winning
   body too.  **If a sched1 residue involves a constant symbol at either end
   of a memory pair, check the REG_EQUIV guard before the MEM flags.**

   **AND THIS IS A COMPOSITION NEITHER SWEEP CAN REACH.**  refsweep only inserts
   `v++; v--;' for locals that ALREADY EXIST, and the park body had no `ba';
   the statement sweeps only move statements that are already there.  The
   lever is "introduce a base-pointer local AND give it a cancelled pair",
   and it has to be tried as ONE change.  Worth generalising: any sched1
   residue where a load through a CONSTANT SYMBOL floats above a store is a
   candidate, and the park's own `MEM_IN_STRUCT_P' negative was looking at
   the wrong flag -- the dependence is suppressed by the REG_EQUIV path in
   sched.c's own alias analysis, not by `mem/s'.

   THE GRID.  {ba absent / before the store / after the store} x {pair, no
   pair} x {hp absent / before / after} x {pair, no pair}, where `hp' is the
   same device applied to the `*(u16 *)(D_800E3D7C + 0x88)' base:
       ba + pair, no hp pair ........ 35 at 278   (four bodies, all equal)
       ba + pair AND hp + pair ...... 40 at 279
       hp + pair alone .............. 46 at 279
       everything without ba's pair . 43 at 278
   So only the `lbu' base wants the device, and `ba's position does not
   matter.

   WHY e3's PAIR PAYS, AND WHAT IS STILL WRONG THERE.  With `ba's pair alone
   the block is instruction for instruction identical to 800C2AA8-800C2AD8
   and the whole residue is a TWO-QUANTITY REGISTER SWAP (6 rows): 0x208020
   in $t0 where the target has $v1, and the `*(u8 **)&D_800E3D7C' value in
   $v1 where the target has $t0.  Their live ranges are NESTED -- 0x208020 is
   born at built word 164 and dies at the `sw $v1, 0x14($sp)' at 179, the
   D_800E3D7C value lives 169..174 -- so they conflict and local-alloc hands
   the LOWEST FREE register to whichever quantity it allocates first.  The
   sort key is
       QTY_CMP_PRI(q) = floor_log2(n_refs) * n_refs * size
                        / (qty_death - qty_birth)          (local-alloc.c:1693)
   and NOTE THAT floor_log2(1) IS 0, so a one-reference quantity sorts last.
   With the park's spelling the D_800E3D7C value has 3 references over a
   5-insn range and 0x208020 has 4 over a 15-insn range, so the value sorts
   first and takes $v1.  `e3's second set raises its reference count and its
   birth, 0x208020 wins the sort and takes $v1, and three of the six rows go.
   THE THREE THAT REMAIN: e3 is now in $a2 where the target has $t0 (built
   169/171/174 -- `lw a2' / `sw a2' / `lh a2,136(a2)').  e3 dies AT the `lh'
   that sets $a2, and `find_free_reg' scans a HALF-OPEN [birth, death), so
   ours shares $a2 with the `lh' result; the target's does not, which means
   the target's e3 is still live when $a2 is written.  Finding what keeps it
   alive one insn longer is the open question for these 3 rows, and it is an
   ALLOCATION question, not a scheduling one.

   ====================================== ALLOCATION: THE LIFETIME WORK IS
                                          NOW EXHAUSTIVE (new negatives)

   Three gaps in the previous session's power sets are closed, all negative.

   (i) THE FULL 16-WAY `w' POWER SET IS 43 EVERYWHERE (on the 43-row body).
       The previous session's 32-way set covered {case-2 `a', case-17 `a',
       case-0 pair 2 `w', case-0 pair 3 `w', case-17 `w'} and NEVER INCLUDED
       CASE 1's `w' (`w = *(u16 *)(p + 0x6) - 1;' in the case-1 arm), so `w'
       could never be made block-local to CASE 0 by anything it measured.
       All sixteen subsets of {case-0 pair2, case-0 pair3, case-1, case-17}
       measure 43/278.  Clean.

   (ii) THE `q' PARTITION SPACE IS 43 AT BEST -- 52 BODIES MEASURED.  The
       previous session measured the 64-way POWER SET of the six `q' sites,
       which only explores "one shared group plus singletons".  The target's
       registers say more is worth asking: `*(u8 **)p' is $v1 at case 2
       (800C2A50), $v1 at case 3 (800C2B00), $v1 at case 17a (800C2BD4) and
       $v1 at case 17b (800C2C10), while the case-16 copy is $t0 (800C2B40)
       -- so case 16 cannot share an allocno with the other four, and the
       other four might share one with each other.  Every PARTITION of
       {case 2, case 3, case 17a, case 17b}, crossed with every way of
       attaching case 16 to one of the groups or leaving it alone, was
       measured: 52 distinct partitions.  Best is 43, reached by TWO of them
       -- the park's own {c3,c16}{c2}{c17a}{c17b} and also
       {c3,c16}{c2,c17b}{c17a}.  Then 46 (four partitions), 48 (three),
       49 (nine), 51, 53.  Nothing beats 43.

   (iii) SPLITTING CASE 16's `q' COSTS THE COPY.  `q16' as its own local is
       277 words / 53 rows in ALL SIXTEEN combinations with {hoist the 0x88
       load into a local} x {read the `-0x14' load back off D_800DF55C} x
       {read the `0x88' load back off D_800DF55C}.  The word it loses is the
       `addu $t0, $v0, $zero' copy at 800C2B40: mechanism (f) below only
       produces that copy BECAUSE `q' is a multi-block (global) allocno, so
       cse's reload-as-a-copy survives local-alloc's tie.  Making `q16'
       block-local lets local-alloc tie it to the `lw' result and the copy is
       deleted.
       AND A SECOND SET DOES NOT BUY IT BACK: `q16++; q16--;' at each of the
       three positions in case 16, and a literal duplicated
       `q16 = D_800DF55C;', each crossed with the 0x88 hoist -- 14 bodies,
       every one 277 words / 53 rows.  So the "a cancelled pair buys a second
       set" lever that pays for `ba' above does NOT apply here, and the two
       results together say the pair is worth something only where a
       REG_EQUIV is being USED by a later pass.

   ====================================== THE "NEW LOCAL + CANCELLED PAIR"
                                          LEVER IS EXHAUSTED EVERYWHERE ELSE

   Because the lever paid twice in case 3, it was then tried at EVERY other
   address or value in the function that is computed from a constant symbol
   or an immediate.  All of these measure 32/278 on the live body -- i.e.
   nothing, with or without the pair:
     * `da = (u8 *)&D_80010A93;' for case 2's indexed store (the 3-row
       cluster 5) -- 32 without the pair, and 38 WITH it;
     * `pa = (u32 *)0x80010980;' for the `pa[j] = 0;' store, and `da'+`pa'
       together (38);
     * `r16++; r16--;' and `q++; q--;' in case 16;
     * `h16 = *(u16 *)(q + 0x88);' in case 16, with and without the pair;
     * `df = D_800DF560;' for the tail's `|= 0x8000' store, with and without;
     * `dp = &D_800DF45C;' for case 3's fourth argument, with and without;
     * `f++; f--;' after case 3's `f =' and before the shared tail's
       `D_800814A0 = f;';
     * `fa = &D_800814A0;' with and without the pair, in the D2-off body.
   So the two case-3 sites are special, and what makes them special is that
   their REG_EQUIV is READ by a later pass (sched.c's own alias analysis for
   `ba', local-alloc's quantity sort for `e3').  Elsewhere nothing consults
   it and the extra set buys nothing.

   ====================================== e3's REMAINING 3 ROWS: 14 MORE
                                          NEGATIVES

   All measured on the live body.  The pair must sit BETWEEN `e3's assignment
   and the `D_800DF55C' store; everything else is worthless or worse:
     * `e3++; e3--;' AFTER the store ......................... 36 at 278
     * the pair both before and after the store, and three pairs
       before it ............................................ 32 (nothing)
     * the store, then `h3 = *(u16 *)(e3 + 0x88);' .......... 32 (nothing)
     * `h3 = ...' BEFORE the store (in three arrangements) .. 43/280, 43/280,
                                                              46/280
     * store, pair, then `h3' ............................... 36 at 278
     * `e3' declared `u16 *', `u32 *', `void *' or `s32' (with the casts the
       body then needs) ..................................... 32, every one
   So the declared type is irrelevant here exactly as it was for x2/y2 --
   what matters is the second set and its POSITION.

   ====================================== D2 CROSSED WITH `f' PAIRS: 9 MORE
                                          NEGATIVES

   The one combination the previous session could not have tried, because
   `ba'/`e3' did not exist: the D2-OFF body with a cancelled pair on `f'.
   `f++; f--;' at each of five positions -- after case 3's `f =', after its
   `D_800814A0 = f;', before the shared tail's `D_800814A0 = f;', and after
   case 17's `f =' -- is 35 at 281 words in EVERY ONE, i.e. identical to
   D2-off with no pair.  D2-off plus a duplicated tail with and without the
   pair is 39 at 285.  And on the LIVE (D2-on) body, `f++; f--;' after case
   17's `f =' and before the shared tail's first statement are both 32.
   So the three words D2-off costs in the tail are NOT a register-placement
   problem that a second set can fix, and D2 stays.

   ====================================== WHAT IS LEFT -- 27 ROWS, 4 CLUSTERS

   NOTE: case 3's ARGUMENT block is now entirely clean -- the three `e3'-in-
   $a2 rows that the 32-row body still had went with the permuter's merge,
   because `e3' is no longer a block-local quantity competing for $a2.  The
   14 e3-lifetime negatives recorded below were measured on the 32-row body
   and are superseded; keep them for the mechanism, not the scores.

   1. CASE 3's `D_800814A0' BLOCK AND ITS POINTER REGISTER, 9 rows
      (built 182-189 plus the three target-only words at
      800C2B18-800C2B20).  Two things are tangled here: device D2 (see
      "DEVICE ABLATION", re-confirmed on the 32-row body) and the merged
      `e3' landing in $t0 where case 3 alone would want $v1 (built 184
      `lw t0,0(s1)' against 800C2B00 `lw $v1, 0x0($s1)').  The second is the
      price of the permuter's find and is discussed in that section.
      With D2 OFF the block is structurally right and three words move into
      the tail instead -- see "DEVICE ABLATION", re-measured on the 32-row
      body, where D2 still wins by 3 rows and 3 words.
      **EVERY SPELLING NEGATIVE THE 43-ROW SESSION RECORDED FOR CASE 3's
      ARGUMENT BLOCK IS NOW STALE** -- all of it was measured on a body whose
      instruction ORDER there was wrong, so the thing those spellings were
      failing to move is no longer the thing that is wrong.  The list is kept
      below for its reasoning, not for its scores.

   2. CASE 16's `r16' REGISTER, 8 rows: 6 at built 200-203 plus the two
      branch-drift rows at built 211 and 223, which cluster 1's three missing
      words cause rather than this arm.  (3 + 9 + 8 + 7 = 27.)
      The permuter's merge fixed `e3' here --
      it is in $t0 as the target has it, and the copy and `sw' rows are gone
      -- and what is left is `r16' in $v0 against the target's $a1 (built
      200/202/203 `lw v0,-20(t0)' / `lbu a0,36(v0)' / `lbu a1,37(v0)' against
      800C2B44/800C2B54/800C2B58 `lw $a1,-0x14($t0)' / `lbu $a0,0x24($a1)' /
      `lbu $a1,0x25($a1)'), plus the `sw %lo(D_800DF55C)' still sitting
      before the two loads instead of after them.
      THE MECHANISM, WHICH IS NOW CLEAR: `r16' is block-local, and
      `find_free_reg' takes the LOWEST free register, so it gets $v0.  In the
      TARGET $v0 is not free across `r16's range, because the target's
      `lui $v0, %hi(D_800DF55C)' sits at 800C2B4C -- AFTER the `-0x14' load
      -- so the %hi pseudo is live inside `r16's range and conflicts with it.
      Ours issues the store, and therefore its %hi, BEFORE the loads, so $v0
      is free.  GETTING THE STORE AFTER THE LOADS IS THEREFORE THE WHOLE
      REMAINING LEVER FOR THIS ARM, and the reason we cannot is a dependence:
      our loads go through `e3', an opaque pointer, while the store's address
      is a symbol, and `memrefs_conflict_p' cannot disambiguate an opaque
      pseudo against a SYMBOL_REF, so sched1 sees a TRUE dependence
      store -> load and may not move the loads up.  The target's compile
      evidently had no such dependence.
      48 BODIES MEASURED ON THE 32-ROW BODY AND 24 MORE ON THIS ONE -- four
      forms
        A  D_800DF55C = *(u8 **)p;  e3 = D_800DF55C;  r16 = *(u8 **)(e3-0x14);
        B  e3 = *(u8 **)p;  r16 = ...;  [h16 = ...];  D_800DF55C = e3;
        C  e3 = *(u8 **)p;  D_800DF55C = e3;  r16 = ...;
        D  as A but both loads re-read off D_800DF55C
      crossed with {`q' shared, `q16' split} x {no pair, `q++; q--;' after the
      copy, after the `-0x14' load} x {0x88 load inline, hoisted into `h16'}.
      BEST IS FORM A -- the body as it stands -- BOTH TIMES: 32 on the
      32-row body (nine of the forty-eight bodies tie) and 27 on this one.
      Form C is 35/277 then 31/277 (it loses the copy), form B is 38/279 and
      worse, form D ties A, and every `q16'-split body is 35 or worse.  So
      the read-back shape is confirmed optimal a fourth time.

   3. THE SHARED TAIL, 7 rows (built 256-269).  The
      two `lw's at built 256/257 are swapped in both order and register
      against the target's 800C2C2C/800C2C30, and built 268's `lui v0,0x0' is
      the `goto shared' join.  RE-MEASURED ON THE 32-ROW BODY: a
      `void *df = D_800DF560;' local is 32 with and without a cancelled pair,
      and the legal permutations of case 17's four tail statements are
      36/277, 42/277, 42/281, 43/277, 44/276, 45/278, 46/279, 47/271, 49/278,
      50/277 and 52/279 -- the body's own order is best by four rows.
      (THOSE TWO ARE 32-ROW-BODY NUMBERS; the cluster is unchanged at 27, but
      re-measure before relying on them.)
      AND A `u32 *fa = &D_800814A0;' LOCAL IS 41 AT 279 WORDS.  Set in BOTH
      case 3 and case 17 (so the shared tail's `*fa = f;' is defined on either
      path) with the cancelled pair in neither block, either block or both --
      all four measure 41/279.  The park's old `fa' negative from the 285-word
      era therefore still holds with the pair added.
      WHAT THE TARGET ACTUALLY DOES HERE, for whoever takes this next: it
      loads D_800DF560 FIRST (800C2C2C) and D_800814A0 second (800C2C30), so
      the `lhu $v0, 0x1E($a1)' at 800C2C34 is two insns behind the load that
      produces its base and needs no stall, and `ori $v1, $v1, 0x8000' fills
      the `lhu's delay slot.  We issue the two loads the other way round, so
      our `lhu' is immediately behind its base load and we pay a `nop' (built
      261).  Reordering the SOURCE does not fix it -- "DF560 statement first"
      is one of the permutations above and scores 36 -- so this is the same
      shape as case 3's argument block was: a sched1 decision that source
      order has no purchase on.  The `ba' lever does not apply because
      D_800DF560's value is a LOADED pointer, not a symbol address, so there
      is no REG_EQUIV for a second set to invalidate.

   4. CASE 2's EXTRA WORD (built 124) AND THE TWO BRANCH DRIFTS IT CAUSES
      (built 114 and 120), 3 rows.  Fully diagnosed in the previous session:
      the target folds the symbol into the store -- `lui $a0,
      %hi(D_80010A93)' ... `addu $v0, $v0, $a0' ... `sb $zero,
      %lo(D_80010A93)($v0)' -- and we materialise the whole address with
      `lui'+`addiu %lo' and add the index afterwards.  `move_operand'
      (mips.c:712-719) rejects a bare SYMBOL_REF as a move source whenever
      `mips_split_addresses' is on, so the address is always expanded as
      `high' + `lo_sum' INTO A REGISTER; LEGITIMIZE_ADDRESS (mips.h:2652) can
      only put the `lo_sum' inside the MEM through `mips_check_split'
      (mips.c:884), which accepts a bare SYMBOL_REF, a
      CONST(PLUS(SYMBOL_REF, CONST_INT)) or a LABEL_REF and NOTHING ELSE, so
      a `symbol + register' address can never take that arm.
      READ THE WHOLE MACRO THIS SESSION AND IT IS WORSE THAN THAT: besides
      the `mips_check_split' arm, LEGITIMIZE_ADDRESS (config/mips/mips.h) has
      exactly two others -- a PIC/CONST arm, and a `(plus REG CONST_INT)' arm
      for a non-SMALL_INT constant -- and NO arm for `(plus SYMBOL_REF REG)'
      in any operand order.  So after `GO_IF_LEGITIMATE_ADDRESS' rejects
      `symbol + reg', LEGITIMIZE_ADDRESS prints "could not fix" and
      `memory_address' falls back to `copy_to_mode_reg' of the WHOLE address,
      which goes through `movsi' and therefore through the high/lo_sum
      expansion -- the extra `addiu %lo' word.  THAT ROUTE IS CLOSED; no C
      spelling of a symbol-plus-register address can reach the target's form
      through this macro, which is why all thirteen spellings below score the
      same.  The target's `lui %hi' / `addu' / `sb %lo(...)($v0)' must be
      built by a later pass folding `(plus (lo_sum (high s) s) i)' into
      `(lo_sum (plus (high s) i) s)', and finding that pass is the whole
      remaining question for this word.
      Measured and rejected on the 43-row body, every one 43/278:
      `(&D_80010A93)[j * 0x54]', `*(&D_80010A93 + j * 0x54)',
      `*(u8 *)((u8 *)&D_80010A93 + j * 0x54)',
      `((u8 *)&D_80010A93)[j * 0x54]', `j * 84' written decimal, `(u32)j' for
      the index, and all of those with the declaration changed to
      `extern u8 D_80010A93[];', `extern u8 D_80010A93[0x54 * 0x40];',
      `extern u8 D_80010A93[][0x54];' (with `D_80010A93[j][0]') and a 0x54
      byte struct array (with `D_80010A93r[j].u0').  Hoisting `j * 0x54' into
      a local was 59; swapping the two stores inside the `if' was 66.  The
      target's form arrives by some other route and finding which pass does
      it is the open question for this word.
      A NEGATIVE WORTH KEEPING, because it looked like a lever for ten
      minutes: `mips_check_split' DOES accept
      `CONST(PLUS(SYMBOL_REF, CONST_INT))', so writing a CONSTANT-offset
      extern access as `*(T *)((s32)&SYM + K)' -- an integer cast that does
      not fold away, unlike `(u8 *)((u8 *)SYM + K)', which does -- really
      does change the RTL: the address becomes a bare CONST,
      LEGITIMIZE_ADDRESS's LO_SUM arm fires, and `.rtl' shows `(mem:QI (reg))'
      with MEM_IN_STRUCT_P cleared instead of
      `(mem/s:QI (plus (reg) (const_int 36)))'.  IT IS WORTH NOTHING.  A
      two-function micro-test compiled with tools/cdkgcc -O2 -G0 gives
      `lui $2, %hi(SYM+36)' / `lbu $3, %lo(SYM+36)($2)' for BOTH spellings --
      gcc already folds a lone constant offset into the relocation.  The two
      spellings only diverge when SEVERAL constant offsets of one symbol are
      live at once, and then the ARRAY spelling is the cheaper one, because
      cse shares a single `lui'/`addiu %lo' base and addresses the rest by
      displacement while the cast spelling makes each offset its own
      relocation.

   ====================================== THE 43-ROW BODY's CASE-3 ARGUMENT
                                          NEGATIVES (REASONING KEPT, SCORES
                                          STALE)

   All of the following measured 43/278 on the 43-row body, i.e. none of them
   moved the `lbu' pair, which is what `ba++; ba--;' now does:
     - seven spellings of the first two arguments: `D_80082E80[0x24]',
       `*(D_80082E80 + 0x24)', `*(u8 *)((u8 *)D_80082E80 + 0x24)',
       `*(u8 *)((u8 *)&D_80082E80 + 0x24)', and the same through a
       `u8 *ba = D_80082E80;' local placed before or after the `D_800DF55C'
       store;
     - `D_800DF55C = D_800E3D7C;' (unifying the two spellings of the same
       global -- levers item 7) and a `u8 *e3' local for it;
     - hoisting the first two arguments into `x3'/`y3' locals, the 0x88 load
       into `h3', and both;
     - a `s32 k = 0x208020;' local for the sixth argument;
     - `(void *)' and bare spellings of the seventh argument, `(s32)' on the
       sixth, `(s16 *)' on the 0x88 load;
     - lifting the call's result into `i' with a separate `if (i != 0)';
     - the case-16 READ-BACK trick applied here too -- reading
       `*(u16 *)(D_800DF55C + 0x88)' after the store instead of
       `*(u16 *)(D_800E3D7C + 0x88)', with and without a `q3' local for it
       and with and without a `ba' local for D_80082E80;
     - THE WHOLE ARGUMENT LIST HOISTED INTO LOCALS IN THE TARGET'S OWN
       EMITTED ORDER -- `ba = D_80082E80; k = 0x208020; dp = &D_800DF45C;'
       then the `D_800DF55C' store then the call -- and each pair of that
       separately.
   `h3' hoisted ABOVE the `D_800DF55C' store was 53 at 280.
   AND IT WAS NOT MEM_IN_STRUCT_P.  `D_80082E80[0x24]' builds
   `(mem/s:QI ...)' because expr.c:5344's INDIRECT_REF arm sets
   MEM_IN_STRUCT_P whenever `TREE_OPERAND (exp, 0)' is a PLUS_EXPR.  Writing
   the two loads `*(u8 *)((s32)&D_80082E80 + 0x24)' puts a NOP_EXPR on top of
   the addition and the `.rtl' dump then shows a plain `(mem:QI (reg:SI 170))'
   with the flag CLEARED.  The schedule did not change: 43/278 for the int
   cast, the u32 cast, the reversed operand order, the `(s32)D_80082E80' form
   and either load alone.  The reason, now confirmed, is that `mem/s' was
   never what suppressed the dependence -- the REG_EQUIV on the base pointer
   was, which is why the second set is the lever and the flag is not.

   ====================================== DEVICE ABLATION (THE POWER SET)

   Run at 73 rows (2 devices, 4 bodies), at 62, at 49, and again on the
   43-row body crossed with {`goto shared', duplicated tail}:

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
   and ITS COST ON THE 43-ROW BODY WAS 46 ROWS AT 281 WORDS.

   **AND THE DEVICE-FREE BODY IS THE STRUCTURALLY CORRECT ONE.**  With D2
   OFF, case 3 comes out instruction for instruction right -- `lui
   %hi(D_800814A0)' in the `beqz' delay slot at 800C2AFC, the two load-delay
   `nop's at 800C2B04/800C2B0C, and the `lw %lo' / `nop' / `ori' trio after
   the `sh' -- and every row there is a register NAME.  The three extra words
   it costs are identified and are all in the TAIL, not in case 3:
     * `lui %hi(D_800814A0)' re-materialised inside the shared block
       (built word 271) because `goto shared' makes a real RTL join;
     * a load-delay `nop' after `lhu $v0, 0x1E($a1)' that the target fills
       with `ori $v1, $v1, 0x8000';
     * `f' landing in $a2 instead of $v1, so case 3's tail and case 17's tail
       differ and jump2 cannot cross-jump them.
   So D2 is propping up the cross-jump as well as case 3's `lui', which is a
   second reason to distrust it.  The aliasing question the park raised before
   (can `*(u8 **)p - 2' be `&D_800814A0'?) is still open; `p's +0 is the
   actor's primitive-buffer pointer and D_800814A0 is a resident-engine word,
   so it is implausible, but it is still not proven.
   RE-MEASURED ON THE 32-ROW BODY, and D2 still wins:
       D2 on,  goto shared ....... 32 at 278   <-- live body
       D2 on,  duplicated tail ... 32 at 277   (one word SHORT, as before)
       D2 off, goto shared ....... 35 at 281
       D2 off, duplicated tail ... 39 at 285
       D2 off + `fa = &D_800814A0;', pair and no pair ... 35 at 281 both
   The `ba'/`e3' changes did NOT move this balance.

   ====================================== `goto shared' vs. WRITING IT TWICE

   The target has NO join at .L800C2C48.  It has TWO copies of
       *(u16 *)(p - 0x2) |= 0x8000;  D_800814A0 = f;
   that **jump2 cross-jumped** -- jump_optimize runs at toplev.c:3690, after
   global_alloc (:3617) and sched2 (:3661), so the two tails only have to
   agree in HARD REGISTERS, and the target's do ($v1 for `f', $a0 for the
   `%hi').  Writing the tail out twice and deleting the label, on the 43-row
   body:
       duplicated tail, D2 on ... 43 at 277   (one word SHORT)
       goto shared,    D2 on ... 43 at 278   <-- live body
       duplicated tail, D2 off .. 50 at 285  (no cross-jump at all)
       goto shared,    D2 off ... 46 at 281
   The duplicated tail buys exactly the shared block's `lui' and nothing else.
   The full {D2} x {goto,dup} x {64 q-splits} grid (256 bodies) was measured
   at the 49-row stage and nothing in it beat the live shape.

   ====================================== BLOCK ORDER IS RIGHT

   Checked against the listing's addresses, pairing branches by ordinal:
   every `R' row is a same-index or within-2 row, the four `.L800C2C5C'
   branches drift by at most 2 words, and the drift at each branch tracks the
   drift at its destination.  The arm order (0, 1, default, 2, 3, 16, 17) is
   confirmed; moving `default:' to six other positions measures 43 or worse
   (52 after case 0).  This is NOT a func_800C4F20-style mis-ordered park, and
   the two block-order moves that function needed have no counterpart here.

   ====================================== SWEEPS

   WHAT THIS SESSION ACTUALLY RAN, STATED EXACTLY, BECAUSE THE BODY MOVED
   TWICE UNDER THE SWEEPS:
     * stmtsweep and widthsweep are CLEAN -- "no safe move helps", "no width
       change helps" -- ON THE 35-ROW BODY, which is NEITHER the 32-row body
       the permuter ran on nor the live 27-row body.
     * refsweep and devsweep are UNMEASURED on every body after 43.  They
       were started on the 35-row body, killed when it was superseded,
       restarted on the 32-row body, and **killed again after 45 minutes
       without finishing.**
   SO ALL FOUR SWEEPS ARE OWED A RUN ON THE LIVE 27-ROW BODY, and that is the
   single cheapest thing left to do on this park.  The reason refsweep and
   devsweep time out is worth fixing rather than retrying blind: both tools
   build an onefunc.py standalone only when the unit is OVER 8000 LINES
   (refsweep.py:195, devsweep.py:250), and since the 2026-11-21 carve
   csrc/ovl_dungeon_800C2824.c is 4987 lines, so they fall back to measuring
   the whole unit -- 0.23 s a candidate against the standalone's 0.09 s, over
   ~2960 insertion points and three rounds.  That threshold was tuned when
   ovl_dungeon.c was 37k lines and every sweep took the fast path; ALL SIX
   CARVED UNITS ARE NOW UNDER IT.  Either lower the threshold or give these
   two a ~90 minute budget on this park.
   The `if (q)' device and the cancelled pairs on every existing local at the
   positions that matter were measured BY HAND on this body instead -- see the
   sections above and below -- so the gap is the positions a full sweep would
   cover that a hand pass did not.

   ALL FOUR WERE CLEAN ON THE 43-ROW BODY and that evidence is valid -- the
   `ww == tw' objective bug fixed in 17f1b0d does not bite on a WORD-EXACT
   park, where the old gate and the new `(abs(ww - tw), rows)' key are
   equivalent.  But they are SCOPED TO THAT BODY, and the body has changed.

   refsweep.py ... clean at 49 rows (19 names, 2527 insertion points) and
     again on the 43-row body (22 names, 2960 insertion points).  NOTE WHAT
     IT CANNOT SEE: it inserts a pair only for a local that is ALREADY
     DECLARED, so it could never find this session's `ba' result.
   widthsweep.py . clean at 49; and by hand on the 43-row body every one of
     `i', `j', `a', `f', `v', `w', `w0', `x2', `y2', `x17', `y17' through
     s8/u8/s16/u16/s32/u32 -- nothing under 43.  The tool's own re-run agreed.
   stmtsweep.py .. clean at 49 ("no safe move helps"), and a gated
     hand-rolled version clean on the 43-row body over six rounds.
   devsweep.py ... run twice, on the 49-row body (47, `if (q)') and again on
     the 43-row body (41, the same `if (q)' around the same statement).  Not
     taken -- see the section below.
   DECLARATION ORDER ... clean.  All 306 single-declaration moves measured at
     the 49-row stage; best was the order in the body.
   ARRAY LENGTHS ....... clean.  rc[2..5] x wh[2,3,4,6]: rc[2]/wh[2] is best;
     rc[2]/wh[3] and rc[2]/wh[4] tie, everything with rc[3] or more is 64+.
   `a' AND THE `w' LIFETIMES ... clean, and now exhaustively so -- see (i)
     above for the sixteen-way `w' set this session added.

   ====================================== THE ONE DEVICE THIS PARK DOES NOT
                                          TAKE: `if (q)' FOR 2 ROWS

   devsweep.py's duplicated-arm device around case 0's early exit --

       if ((*(u32 *)(*(u8 **)p + 0x14) & 0x4000) == 0)
       {
           if (q)  { *(s16 *)(p + 0x4) = 0x10; }
           else    { *(s16 *)(p + 0x4) = 0x10; }
           return;
       }

   -- measured 41 rows at 278 words on the 43-row body and **30 rows at 278
   words on the LIVE 32-row body**: still exactly two rows, re-confirmed after
   the `ba'/`e3' changes.  STILL NOT TAKEN, for the reason below.
   AND THERE IS STILL NO DEFINED EQUIVALENT.  The same duplicated arm with
   `q' as the condition placed in CASE 16, where `q' IS assigned above it, is
   42 rows at 277 words -- i.e. the device only pays from a block that does
   not define `q', which is exactly the thing that makes it an uninitialised
   read.  `ba' and `e3' do not help either: they are assigned only in case 3,
   so a reference to them from case 0 has the same defect.
   IT IS NOT IN THE LIVE BODY because `q' is not assigned on that path (it is
   only set in case 3 and case 16), so the condition reads an indeterminate
   pointer.  Both arms are identical, so this cannot change what the function
   computes -- it is the sanctioned free-reference device, not the
   uninitialised-read DEFECT the rejected permuter finds are -- but this park
   has lost three candidates to that shape and a fourth that LOOKS like one
   was not worth adding.  Not re-measured on the 35-row body.

   NO DEFINED CONDITION REACHES IT, and that was measured EXHAUSTIVELY on the
   43-row body: the duplicated arm was tried around EVERY one of the 89
   statements with EVERY one of the 19 locals and the three parameters as the
   condition (about 1900 bodies), plus `do { } while (0);' around every
   statement.  EXACTLY FOUR bodies came in under 43, all of them 41, and all
   four have `q' as the condition -- around `*(s16 *)(p + 0x4) = 0x10;'
   (case 0), `i = func_800A32A4(*(u8 **)p);' (case 2),
   `func_800A32A4(*(u8 **)p);' (case 17) and `w = *(u16 *)(p + 0x6) - 1;'
   (case 1/17).  `q' is assigned only in case 3 and case 16, so every one of
   the four reads it in a block where it is not defined.  The two rows ARE the
   extra reference to `q' from a block that does not define it, and nothing
   else buys them.  `if (p)', `if (c)', `if (*p)' and `if (p[0])' around that
   same statement are all 43.  Giving `q' a real definition in case 0 instead
   does not work either: `q = *(u8 **)p;' at the 0x4000 test is 45,
   `q0 = q - 0x20;' with that definition is 43, defining `q' inside the
   early-exit block and then using the arm is 45, and the arm with `q' defined
   at the test is 45.

   ====================================== A SECOND UNINITIALISED-READ FIND,
                                          THIS TIME FROM A SWEEP

   An UNGATED statement-move sweep reported 47 rows at 278 words on the
   49-row body for case 16 written

       case 16:
           r = *(u8 **)(q - 0x14);     <-- READS `q' BEFORE IT IS SET
           q = *(u8 **)p;

   which is the same defect as the permuter find this park rejected in an
   earlier session, found by a different tool.  `q' is only assigned in case 3,
   which case 16 cannot reach.  REJECTED.  With a def-use gate (a statement may
   not cross one that writes or reads a local it writes, or that writes a local
   it reads) the same sweep is clean.
   **ANY SUSPICIOUSLY GOOD SCORE REPORTED FOR THIS FUNCTION SHOULD BE READ FOR
   THIS SHAPE BEFORE IT IS BELIEVED** -- that is three finds on this one park,
   from three different tools.

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
     * the two `lbu' call arguments in case 2 and case 17 are hoisted out of
       the call through FOUR DISTINCT LOCALS (x2/y2 and x17/y17), not two
       shared ones: case 2 alone 53, case 17 alone 53, one shared pair 62 at
       280 words, four locals 49, on the 57-row body;
     * the four stack-pair stores are in the source order
       `rc[0]; rc[1]; wh[0]; wh[1];' -- all 24 permutations were measured and
       what the source order decides is which 32-bit constant is MATERIALISED
       first, hence which `lui' reorg steals into the `bnez' delay slot at
       word 27;
     * THE STACK PAIRS MUST BE ARRAYS.  Written as four scalars nothing in
       this function ever reads sp24 or sp2A, so gcc allocates only the two
       addressable scalars and DELETES both stores as dead: 280 words / 105
       rows, with a 0x40 prologue where the target has 0x48.  Four scalars
       plus an unreferenced `s32 unused[2];' buys the frame and leaves the
       stores deleted (280 / 88, and ALL NINE declaration positions measure
       88 -- the tell that the aggregate is wrong).  As `s32 rc[2];
       s16 wh[2];' the four stores are live and the frame comes out 0x48 by
       itself;
     * `r' is split for case 0 (`r0') and for case 16 (`r16') but SHARED
       between case 2 and case 17: case 16 alone 43, case 2 alone 51, case 17
       alone 51, case 16 + case 17 57, on the 44-row body;
     * `w0' is split for the FIRST of case 0's three sprite pairs only, and
       `r0'/`w0' are a PAIR -- on the 49-row body `w0' alone measured 49 and
       `r0' alone 53, and together 43.  Do not ablate either alone;
     * CASE 16 READS `D_800DF55C' BACK: `D_800DF55C = *(u8 **)p;
       q = D_800DF55C;' and NOT `q = *(u8 **)p; D_800DF55C = q;'.  That is
       what produces the `addu $t0, $v0, $zero' copy at 800C2B40 -- cse turns
       the reload into a copy of the value just stored and update_equiv_regs
       (local-alloc.c:1081-1093) gives it a REG_EQUIV that reload satisfies
       from the register already holding the value.  It is the only one of
       twelve shapes tried that produces the copy, and this session's (iii)
       shows the copy also needs `q' to stay a multi-block allocno.

   ====================================== EARLIER SESSIONS (kept)

   2026-11-20: case 3 inlining the shared tail cost four words; `goto shared'
     took 285 -> 281.
   2026-11-21 (earlier): refsweep's `r++; r--;' took 78 -> 73 and stmtsweep's
     D2 move took 281/80 -> 278/78.  A 75-row permuter find at 279 words was
     REJECTED for hoisting `D_80082E80' into a local whose assignment sits
     after a `return;' -- unreachable, so the local is read uninitialised.
     The same hoist in the reachable position measured 80, i.e. nothing.  A
     `u32 *fa = &D_800814A0;' pointer local measured 283/87, 284/83 and
     285/87 at three positions, all worse.
   2026-11-21 (session 3), 73 -> 43: ONE LEVER APPLIED SIX TIMES, "one local
     per symbol lifetime".  A C variable is one pseudo; a pseudo live in more
     than one block is a GLOBAL allocno (flow.c:2561) that `find_reg'
     allocates from its WHOLE live range's conflicts (global.c:925-940), while
     a quantity confined to one block goes to local-alloc's `find_free_reg'
     and, because MIPS defines no REG_ALLOC_ORDER, takes the LOWEST free
     register.  Stages: `q' split at four of six sites 73 -> 62; the four
     stack-pair stores reordered 62 -> 57; the two `lbu' arguments hoisted out
     of the call through four locals 57 -> 49; `r' split for case 0 49 -> 53
     (worse alone, right anyway); `w0' split 53 -> 43; the case-16 read-back
     43/277 -> 44/278; `r' split for case 16 44 -> 43; and the old `r++; r--;'
     device deleted as worth zero.
   THOSE NUMBERS ARE SCOPED TO BODIES THAT NO LONGER EXIST.

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
    u8 *ba;
    u8 *e3;

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
        ba = D_80082E80;
        ba++;
        ba--;
        e3 = *(u8 **)&D_800E3D7C;
        e3++;
        e3--;
        D_800DF55C = e3;
        if (func_800BBA40(ba[0x24], ba[0x25], *(u16 *)(e3 + 0x88), &D_800DF45C, 0x2800, 0x208020,
                          (u32)func_800C0180) != 0)
        {
            f = D_800814A0 | 0x8000;
            e3 = *(u8 **)p;
            *(u16 *)(e3 - 0x2) |= 0x8000;
            D_800814A0 = f;
            goto shared;
        }
        break;
    case 16:
        D_800DF55C = *(u8 **)p;
        e3 = D_800DF55C;
        r16 = *(u8 **)(e3 - 0x14);
        if (func_800BBA40(r16[0x24], r16[0x25], *(u16 *)(e3 + 0x88), &D_800DF45C, 0x2800, 0x208020,
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
