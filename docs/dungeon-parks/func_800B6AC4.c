/* ==================== LANDED 2026-11-22.  812/812 words, 0 aligned
   ==================== differences, and 0 differing (masked) in the real unit.

   THE BODY BELOW IS THE LANDED BODY.  Merge it with the comment block and the
   DECLARATIONS reproduced further down.  Everything under the second ruler is
   the pre-landing log, kept for the lever catalogue.

   THE LAST TWO ROWS WERE A const ALIAS ON D_800814A0, AND IT IS A NEW LEVER.
   Case 10 reads and writes the word through

       extern const u32 D_800814A0_c __asm__("D_800814A0");
       ...
       z = D_800814A0_c;
       *(u32 *)&D_800814A0_c = z | 0x8000;

   and that one change takes the arm from 2 aligned rows to 0 at 812 words.

   WHY, AND IT IS THE STEP THE REST OF THIS HEADER WAS MISSING.  The residue
   was a transposition of the two sched2 fillers at aligned 763 and 769, and
   this header had proved the basin closed WHILE 1841 IS BIRTHING.  That proof
   is sound.  The conclusion drawn from it was not, because de-birthing 1841
   was never the only way out.  The question that mattered is whether 1846,
   the `lw %lo(D_800814A0)', is BLOCKED in the cycle after the c+0x2C store.
   Off the sched1 trace for the park body:

       ;; ready list at T-19: 1849 (8) 1881 (1) 1877 (1), now 1849 1881 1877
       ;; ready list at T-20: 1881 (1) 1877 (1) 1846 (7f000001) 1841 (7f000001)
       ;; blocking insn 1846 for 1 cycles, now 1841 1881 1877
       ;; ready list at T-21: ... 1840 (7f000001) 1846 (7f000001), now 1846 1840 ...
       ;; ready list at T-22: ... 1838 (8) 1843 (7f000001), now 1843 1840 1838 ...

   1846 and 1841 are released TOGETHER by the store at T-19, both carry
   LAUNCH_PRIORITY, 1846 has the higher LUID and sorts first, and ONLY the
   memory-unit hazard hands T-20 to 1841.  Unblock 1846 and the order falls
   out with no change to 1841 at all.  1846 takes T-20, 1843 is released and
   takes T-21, 1841 takes T-22, 1840 takes T-23, and the post-sched1 chain is
   `1838 < 1840 < 1841 < 1843 < 1846', which contains the target's filler
   order `a2 < 1840 < 1841 < 1843'.

   AND 1846 IS BLOCKED ONLY BECAUSE THE STORE RELEASES IT.  The store is
   1846's ANTI successor, so 1846 can never be ready before the cycle after
   the store, and that cycle is always the blocked one.  The link has to go.
   `anti_dependence' (sched.c:866) has two escapes and the first is
   `if (RTX_UNCHANGING_P (mem)) return 0' on the READ.  varasm.c:773 sets that
   flag from `TREE_READONLY' on the DECL, so a const-qualified DECLARATION of
   the symbol is the entire mechanism.  expr.c:5359 is why no CAST reaches it,
   because an INDIRECT_REF takes the flag from `TREE_READONLY & TREE_STATIC'
   and a cast to `const u32 *' never sets TREE_STATIC.  That is why the nine
   const and volatile spellings recorded below all measure 2.

   WHY THE STORE MUST CAST THE const AWAY.  `output_dependence' (sched.c:893)
   has NO unchanging escape, so the link pinning the c+0x2C store ahead of the
   D_800814A0 store survives either way.  The cast keeps the store an ordinary
   changing, non-in-struct reference, which is what keeps `true_dependence''s
   in-struct exception able to sink it past the `((s16 *)d)[0x15]' load.  An
   INDIRECT_REF through `(u32 *)' is not in-struct here, since expr.c:5344
   wants a PLUS_EXPR or an ADDR_EXPR of an aggregate at the top of the
   address.

   THE ALIAS IS THIS REPO'S OWN SPELLING FOR THIS VERY SYMBOL.  csrc/main.h
   already carries `extern u32 D_800814A0_a[1] __asm__("D_800814A0")' with a
   long comment about needing two spellings per site, so this is the same
   device one qualifier over.  Putting `const' on include/structs.h's own
   `extern u32 D_800814A0' is NOT an option.  csrc/unk_24FCC.c,
   csrc/unk_154BC.c and six landed functions in THIS unit assign to the bare
   name and all of them would stop compiling.

   MEASURED AROUND THE LEVER, on the 2-row body, with a baseline canary
   interleaved every six candidates:
     * `extern const u32 D_800814A0_c', read and store both through it, 0 rows
       at 812 words.  THE LANDING.
     * the same lever spelled as `const' added to include/structs.h and
       csrc/ovl_dungeon_decls.h with the store as
       `*(u32 *)&D_800814A0 = z | 0x8000', also 0 at 812, measured against
       shadow copies of both headers.  Same mechanism, not usable, see above.
     * the alias WITHOUT `const', read and store both through it, 2 at 812 --
       so the qualifier is the lever and the indirect-ref store is not.
     * the const alias on the READ ONLY with the bare name on the store, 18
       rows at 813 words.  Two DECLs for one symbol give two SYMBOL_REFs, cse
       cannot share the `%hi' and the arm costs an extra word.
     * the c+0x2C store as `((u8 **)c)[0xB]', which removes the SAME anti
       dependence by MEM_IN_STRUCT_P, 9 rows at 812 words -- AND ITS FILLER
       ORDER IS THE TARGET'S.  It is 9 and not 0 because the in-struct flag
       also kills the OUTPUT dependence on the D_800814A0 store.  That
       asymmetry is exactly what the const buys and the array form cannot.
     * making the D_800814A0 STORE in-struct to keep that output dependence,
       `((u32 *)((u8 *)&D_800814A0 - 4))[1]', 43 rows at 805 words with and
       without the array form.  An in-struct store can no longer be sunk past
       the d+0x2A load, so it collides head-on with the lever that landed that
       load.
     * the same bias on the LOAD with the array-form store, 8 rows at 812.

   THE COMMENT BLOCK FOR csrc GOES IMMEDIATELY ABOVE THE BODY.  Its opening
   and closing comment markers are dropped here so that this header stays a
   single comment, and the ready-to-paste form is in the merge script:

     The 12-case +0x9B state machine over jtbl_80089310.  812 words, 0 differing.
     FIVE SPELLINGS CARRIED IT, each measured, and the last of them is the one
     that had kept the park at 2 aligned rows for a day:

     (1) EVERY D_80083460 BASE AND EVERY SCRATCH POINTER NEEDS ITS OWN LOCAL --
         h/h2/h3/h4, r, e2/e7/e9/e10, n5, base9/base10.  One `h' shared by cases
         2, 3 and 7 crosses a call in case 3, so global alloc gives the single
         pseudo a callee-saved register everywhere and case 2's
         `lui/addiu D_80083460' comes out in $s0 where the target has $v1.
         Splitting them took the function from 280 aligned rows to 96 in one
         step and, with it, fixed the whole parameter register map.
     (2) CASE 2's FIRST D_80013714 READ IS THE CONSTANT ADDRESS
         `*(u16 *)0x80013714' and the other four are structs.h's SYMBOL.  As the
         symbol the `lui' is two insns and reorg steals one into the delay slot
         of the preceding `bnez $v0, .L800B7750', where the target has a `nop'.
         In case 7 the `%hi' is CSE'd into $s3 and serves three reads, which the
         assembler macro cannot do, so that site must NOT be the constant.
     (3) CASE 10's d+0x2A LOAD IS `((s16 *)d)[0x15]', NOT `*(s16 *)(d + 0x2A)'.
         expr.c:5344 sets MEM_IN_STRUCT_P when the TOP NODE of the INDIRECT_REF's
         address is a PLUS_EXPR, and that is what lets true_dependence's
         in-struct exception sink the D_800814A0 store past the load.  The
         flagless cast is 818 words and 46 rows on this body.
     (4) CASE 10's TABLE STORE IS `*(u8 **)(c + 0x2C)', NOT `((u8 **)c)[0xB]'.
         Here the SAME flag is wanted ABSENT.  The array form sets it, the
         store's dependences on the D_800814A0 pair vanish and the store floats.
         152 bodies with the array form measure 9 rows and the same bodies with
         the pointer cast measure 2.
     (5) CASE 10 READS D_800814A0 THROUGH THE const ALIAS `D_800814A0_c' AND
         WRITES IT THROUGH `*(u32 *)&D_800814A0_c'.  The alias carries its own
         comment at the top of this file.  With the bare name on the same two
         statements the arm is 2 aligned rows, with the alias it is 0.

     WHAT DOES NOT WORK, so that it is not tried again.  On the 2-row body all
     five sweeps come back empty -- widthsweep, stmtsweep, refsweep over 28
     names and 7680 insertion points, devsweep over 4959 wrappers a round, and
     partsweep.  So do 420 legal statement orders of case 10 and a 150-minute
     permuter run of 219965 candidates whose best find measured 19 rows.  The
     residue was a transposition of the two sched2 fillers at aligned 763 and
     769, and every way of de-birthing the table pseudo to fix it costs more
     than it buys -- a cancelled `t10++; t10--;' 13 rows, a real second set
     sharing another arm's table 19 to 22, a read-modify-write 27 rows at 814
     words (case 8 cross-jumps into case 10's tail at .L800B770C, which pins the
     call's index to `t10[idx]'), and a `register u8 *t10 __asm__("$5")' pin 6.
     Lever (5) fixes the transposition without touching that pseudo at all.

   DECLARATIONS
     extern u32 D_800835E4[];
     extern u8 D_800DD108[];
     extern u8 D_800DD110[];
     extern u8 D_800DD118[];
     extern u8 D_800E0A2A[];
     extern u8 D_800E0A33[];
     extern u8 D_800E2968;
     extern const u32 D_800814A0_c __asm__("D_800814A0");
     typedef u8 *(*UnkDungeonSpawner)(s32, u8, u8, s16);
   END DECLARATIONS
   The alias wants the explanatory comment that sits above it in the landed
   file, because parksplice carries the LINE and not the comment.

   ==================== everything below is the pre-landing log.

   PARK LOG -- func_800B6AC4, the 12-case +0x9B state machine over jtbl_80089310.
   LANDED -- see the top of this file.  STATE BEFORE IT, 2026-11-22 (the dated sections below are a
   log, newest first; this block is the only one that describes the body as
   it stands):

       812 words, target 812 words, 2 aligned differences
       (tools/decomp/aligndiff.py); ovlverify's positional count is
       2 differing (masked).

   EVERY ARM IS BYTE-EXACT EXCEPT FOR TWO INSNS IN CASE 10, AND THE RESIDUE
   IS ONE TRANSPOSITION OF TWO ADJACENT FILLERS:

       aligned 763   candidate `lui $t0, %hi(D_800814A0)'
                     target    `addiu $a1, $a1, %lo(D_800DD140)'
       aligned 769   candidate `addiu $a1, $a1, %lo(D_800DD140)'
                     target    `lui $t0, %hi(D_800814A0)'

   Everything else in case 10 -- the register map, `sw $a1, 0x2C($a0)' at
   776 through the arg copy, the `lh $a3, 0x2A($s3)' at 778, the a2 setup at
   750, `lui $a1, %hi(D_800DD140)' at 752 -- is byte-exact.

   IT IS A sched2 FILLER TIE BROKEN ON LUID, AND THE LUID IS SET IN sched1.
   `rank_for_schedule' (sched.c:2429) compares INSN_PRIORITY, then a CLASS
   against the just-scheduled insn, then `INSN_LUID (tmp) - INSN_LUID (tmp2)'
   with `tmp = *y' and `tmp2 = *x', so the HIGHER LUID sorts FIRST.  The
   scheduler runs BACKWARD and takes ready[0], so of a tied ready set the
   highest LUID is placed at the LATEST position.  LUID is the POST-SCHED1
   chain position.  For this candidate that chain is

       ... 1838(sh -0x2)   1840(%hi D_800DD140)   1843(%hi D_800814A0)
           1846(lw %lo D_800814A0)   1841(lo_sum D_800DD140)
           1849(sw c+0x2C)   1857(%hi D_80083228) ...

   At sched2 both 1841 and 1843 have INSN_PRIORITY 1 (sched2 priorities here
   are NOT binary -- the trace shows 8, 7, 6, 5, 4, 3, 2, 1), so they are
   FILLERS and get placed only when nothing better is ready.  The sched2
   trace reads

       ;; ready list at T-25: 1841 (1) 1843 (1) 1881 (1), now 1841 1843 1881

   -- 1841 sorts first because it has the higher LUID, so it takes T-25 (769)
   and 1843 waits until T-31 (763).  Position is `794 - T' in this block.

   ============== CORRECTED 2026-11-22.  IT IS NOT A CLASS TIE-BREAK, AND
   ============== CLASS 2 IS UNREACHABLE ON MIPS.

   This header said for a day that sched1's T-20 pick was decided by
   `rank_for_schedule''s CLASS test -- 1841 a DATA predecessor at
   `insn_cost == 1' giving class 3 against 1846 an ANTI predecessor at cost 0
   giving class 2.  BOTH HALVES ARE WRONG, and the second one is wrong for
   every function in this project, so it is worth stating plainly.

   `ADJUST_COST' (config/mips/mips.h:3187) is

       if (REG_NOTE_KIND (LINK) != 0) (COST) = 0

   for ANY anti or output dependence, and `insn_cost' (sched.c:1394) then does
   `if (ncost <= 1) LINK_COST_FREE (link) = ncost = 1'.  So an anti or output
   dependence ALWAYS returns cost 1, and `rank_for_schedule''s first class
   test -- `link == 0 || insn_cost (...) == 1' -> class 3 -- fires before the
   `REG_NOTE_KIND (link) == 0' test can ever be reached.  **On MIPS a
   non-data dependence is always class 3 and tmp_class is never 2.**  Both
   1841 and 1846 are class 3 here, so the class test TIES and decides nothing.

   WHAT ACTUALLY DECIDES T-20 IS A FUNCTION-UNIT HAZARD.  The sched1 trace,
   read in full rather than one line at a time, says so outright:

       ;; ready list at T-20: 1881 (1) 1877 (1) 1846 (7f000001) 1841 (7f000001)
       ;; blocking insn 1846 for 1 cycles, now 1841 1881 1877
       ;; launching 1846 before 1841 with no stalls at T-21

   The sort puts 1846 first (higher LUID, same class, same priority).  Then
   `schedule_select' (sched.c:2660) calls `actual_hazard' on each insn of the
   top priority group and QUEUES 1846, because the c+0x2C store 1849 issued at
   T-19 and mips.md:148-158 gives the "memory" unit multiplicity 1 with a
   store ready-delay of 1 -- a load cannot issue the cycle after a store.  The
   same blockage is visible twice more in this one block, at T-15 (the two
   `lh' loads behind the D_800814A0 store) and at sched2 T-19, where it is
   CORRECT and produces the target.  With 1846 queued, 1841 is the only insn
   left in the top group, so `j - i - q == 1' and 1841 takes T-20.

   ============== AND THAT MAKES THE BASIN PROVABLY CLOSED WHILE 1841 IS
   ============== BIRTHING.  THIS IS A PROOF, NOT A SWEEP.

   Three facts, each read off the dumps for this exact body:

     (a) 1846 is an ANTI predecessor of 1849 (`REG_DEP_ANTI 1846' appears in
         1849's LOG_LINKS), so 1846 must precede 1849 in the chain, and 1843
         is 1846's DATA predecessor, so 1843 precedes 1846.
     (b) 1841 is birthing -- `birthing_insn_p' (sched.c:2512), SET_DEST is
         reg/v:SI 100 which is `t10' with `reg_n_sets == 1' -- so
         `adjust_priority' gives it LAUNCH_PRIORITY and it is taken the
         INSTANT it is ready.  It becomes ready the cycle after 1849, its
         only late successor.
     (c) that cycle is, BY CONSTRUCTION, the one where 1846 is blocked,
         because 1849 is a store and 1846 is a load.

   So 1841 is always placed in the slot immediately after the store, and
   1840/1843/1846 are all before the store, hence before 1841.  1841 is
   therefore ALWAYS the last of the three in the post-sched1 chain,
   LUID(1841) > LUID(1843) always, and sched2 always puts 1841 at 769.
   NO STATEMENT ORDER OR CAST CAN CHANGE THIS.  That is why 420 statement
   orders, 93 spellings in the sibling basin and all five sweeps come back
   empty -- they are all searching inside a closed basin.

   THE TARGET'S FILLER LUID ORDER IS `a2 < 1840 < 1841 < 1843'.  This body
   gives `a2 < 1840 < 1843 < 1841'.  It is ONE ADJACENT SWAP, and the swap
   requires 1841 to be NON-BIRTHING, i.e. `reg_n_sets (t10) != 1'.

   ============== THE NON-BIRTHING ROUTE IS BLOCKED BY THE ARGUMENT COPY,
   ============== WHICH calls.c PUTS OUT OF REACH OF THE SOURCE.

   Make 1841 non-birthing and the top priority group at T-20 is `{1846}'
   alone.  It is entirely queued, so `j - i - q == 0' and `schedule_select'
   CONTINUES to the next group -- the priority-1 one, which is
   `{1881 (a2 = 0), 1877 (a0 = c), 1841}'.  The a2 argument copy has the
   highest LUID of the three and takes the freed slot.  `calls.c:1851' emits
   the argument copies LAST in the call's expansion, so no statement order can
   lower their LUID -- see docs/matching-levers.md:18378, which proves exactly
   this for two town functions.  The a2 copy's LUID then rises above 1843's,
   it wins the FIRST sched2 filler slot, and it lands at 769 instead.
   Verified in the dumps for the cancelled-pair body: insn 1847 (the lo_sum)
   reads `priority = 1' at sched1, `;; insn 1887 has a greater potential
   hazard' names the a2 copy as the winner of T-20, and the post-sched1 chain
   becomes `1840 1847 1835 1883 1836 1838 1849 1852 1887 1855' -- the flip
   HAPPENED and cost 11 rows elsewhere.

   SO THE SHAPE OF ANY WINNING CANDIDATE IS NOW KNOWN AND IS THREE-PART:

     1. `reg_n_sets (t10) != 1', so the lo_sum stops being birthing.
     2. the freed sched1 slot taken by a NON-FILLER, so the a2 copy keeps its
        low LUID.  THE ONE INSN THAT CAN DO THIS IS FOUND AND IT IS FREE --
        see the d+0x60-2 MEM_IN_STRUCT_P entry below.  It is worth 0 rows on
        its own and it is a PREREQUISITE, not a win.
     3. t10 still block-local with `reg_n_deaths == 1', so local-alloc keeps
        it and still hands the pair $a1.

   Every mechanism measured satisfies 1 but breaks 3, and that is where this
   park now stands.  Part 3 is the open question.

   `tools/decomp/sched.py --sched1', `tools/decomp/dump.sh' and the
   `;; ready list at T-NN' trace are the tools for all of this -- every
   number in this header came from them.

   AN EQUAL-SCORING ALTERNATIVE EXISTS AND IS A DEAD END -- DO NOT SPEND ON
   IT.  With the c+0x2C store BEFORE the D_800814A0 read-modify-write and no
   `t10'/`z' at all (case 10 then needs no local: `D_800814A0 |= 0x8000;'),
   the body also measures 2 rows at 812 words, and its single residue is
   `lw $v1, %lo(D_800814A0)($t0)' at candidate 776 against target 774.  THAT
   BASIN CANNOT CLOSE: `*(u8 **)(c + 0x2C)' is not MEM_IN_STRUCT_P and the
   D_800814A0 refs have non-varying `lo_sum' addresses, so the sched.c:835
   exception cannot fire in either direction and the two conflict; a
   dependence is in PROGRAM order, and the target's listing has the LOAD at
   774 and the STORE at 776, so the target's source has the load first.  93
   spellings in that basin all give the same 2-row residue.

   ====================================== 2026-11-22 (second session): still 2
                                          rows at 812/812.  189 candidates, a
                                          corrected mechanism and a proof.

   THE RESIDUE IS CONFIRMED IN RAW BYTES, so it is a real transposition of two
   relocated operands and not a masking artefact.  `ovlverify.py --raw':
   candidate 763 is `3C080000' against target `24A5D140', candidate 769 is
   `24A50000' against target `3C088008'.

   partsweep.py -- THE ONE SWEEP THAT HAD NEVER BEEN RUN HERE -- IS CLEAN.
   Two groups found, `(u8 *)&D_80083460' over 4 names with 15 partitions and
   `(u8 *)0x80010000' over 2 names with 2 partitions.  `no partition helps',
   base and final both 2 rows at 812/812.  All five sweeps are now empty on
   this body.

   THE d+0x60-0x2 HALFWORD STORE AS AN ARRAY_REF IS WORTH 0 ROWS AND IS THE
   PREREQUISITE FOR PART 2 ABOVE.  Written `((u16 *)*(u8 **)(d + 0x60))[-1]'
   the top node of the INDIRECT_REF's address is a PLUS_EXPR, so
   MEM_IN_STRUCT_P is set (expr.c:5344), and `true_dependence''s exception
   fires against the D_800814A0 load -- HImode, address varies, in-struct,
   against a `lo_sum' that is neither.  VERIFIED IN THE DUMP, not inferred:
   insn 1846's LOG_LINKS go from `(1828, ANTI 1789, 1838, 1843)' to
   `(1828, ANTI 1789, 1843)', and the sched1 trace changes from

       ;; ready list at T-20: 1881 (1) 1877 (1) 1846 (7f000001) 1841 (7f000001)

   to

       ;; ready list at T-20: 1881 (1) 1877 (1) 1838 (8) 1846 (7f000001) 1841 (7f000001)

   -- the `sh' (1838, priority 8, a NON-filler) is now ready in the contested
   slot, which is exactly what keeps the a2 copy's LUID low.  It changes
   nothing on its own because 1841 is still birthing and still wins the slot.
   The output dependence that pins the `sh' ahead of the c+0x2C store SURVIVES
   (the store's address varies, so the exception cannot fire there), so the
   park's program-order proof is not disturbed.  Both spellings measure 2 at
   812 words -- `((u16 *)*(u8 **)(d + 0x60))[-1]' and
   `((u16 *)((u8 **)d)[0x18])[-1]'.  KEEP THIS IN MIND rather than
   re-deriving it, and PAIR IT with any part-1 change.

   EVERY WAY OF MAKING THE lo_sum NON-BIRTHING, PRICED.  All satisfy part 1
   and all break part 3.

     * a cancelled `t10++; t10--;' anywhere between the table assignment and
       the call: 13 rows at 812 words, at all six positions and in both
       orders.  AFTER the call it is dead and measures 2.  BEFORE the
       assignment it is also dead -- 14 candidates over seven positions, all
       2 -- because flow deletes the dead store and `reg_n_sets' is 1 again.
       The 13 is combine's `i2dest_in_i2src' path (combine.c:2310) taking
       `reg_n_deaths' to 2, which fails local-alloc.c:472, so t10 leaves
       local-alloc and the pair comes out in $v0/$t0 where the target has $a1.
       With the d+0x60 array_ref as well: 22.
     * a REAL second set sharing t10 with another arm's table.  Twelve
       candidates over cases 0, 1, 8 and 9, all at 812 words: store and index
       both through t10 19-22, store only 19-22, index only 13.  t10 is then
       live in two blocks, so flow.c:2561 makes it a GLOBAL allocno and
       `find_reg' prices its whole range.  With the d+0x60 array_ref as well:
       13 to 49.
     * a REAL second set reusing a local whose live range is provably
       disjoint: `e10' (which already has two real sets) 22, `base10' 14,
       t10 holding e10's first value 15, t10 holding 0x80010000 14.
     * a REAL second set from the d+0x60 pointer, whose last use precedes the
       table assignment: for the `func_8009A028' argument only 2 at 812 --
       AND THAT 2 IS A FALSE NEGATIVE OF THE MECHANISM, because cse folds the
       set into the `if' condition's existing pseudo and flow deletes it.
       Confirmed by counting `set (reg/v:SI 100)' in the dump -- ONE set, so
       reg_n_sets is still 1 and nothing was tested.  For the call plus the
       halfword store 32 at 811, for the halfword store only 16, set before
       the `if' with the condition through it too 42 at 811.
     * a READ-MODIFY-WRITE second set, which is the func_800D92C0 lever and
       the one that costs no deaths: `t10 += idx' with the call as `*t10'
       is 27 rows at 814 words, and the two extra words are NOT in this arm.
       THE REASON IS CROSS-JUMPING AND IT PINS THE CALL'S SPELLING FOR GOOD:
       case 8 ends `j .L800B770C' with `addu $v0, $v0, $a1' in the delay slot
       at aligned 656/657, and `.L800B770C' IS CASE 10's `lbu $a1, 0x0($v0)'
       at 786.  Case 8 cross-jumps INTO case 10's tail, so case 10 must emit
       `addu $v0, $v0, $a1' then `lbu $a1, 0x0($v0)' -- which is `t10[idx]'
       and not `*t10'.  Compensated forms are worse still: `t10 += K' with
       `t10[idx - K]' is 27 rows at 815 words for K in 1, 2, 4, 8 and 0x10,
       `t10 -= 8' with `t10[idx + 8]' 27 at 815, and the += moved ahead of the
       store with the store compensated 32 at 816.  With the d+0x60 array_ref
       as well: 40 at 814.
     * A REGISTER PIN, which makes SET_DEST a HARD reg so `reg_n_sets' is the
       count for that hard register and birthing_insn_p fails with no second
       set at all.  THIS IS THE BEST NON-PARK NUMBER FOUND AND IT IS 6 ROWS at
       812 words, with `register u8 *t10 __asm__("$5")' -- $a1, the target's
       own register.  Its residue is the PREDICTED failure, which is the
       value in it: the pair goes non-birthing and floats to the top of the
       block at 749/752 against the target's 752/763, `lui $t0' lands at 763
       against 769, and `addu $a2, $zero, $zero' is driven from 750 to 769.
       Other pins, all one-line changes: $t0 9, $t1 9, $t8 9, $t9 9, $a3 10,
       $v1 11, $a0 30 at 815, $a2 32 at 815, $v0 40 at 814.  With the d+0x60
       array_ref, $a1 is 36 at 808 words (three spellings of the array_ref,
       all 36/808) and $t0 is 9.
     * ON THE PINNED ($a1) BODY, ALL 20 LEGAL STATEMENT ORDERS RE-SWEPT, with
       and without the d+0x60 array_ref -- 40 candidates.  Best 6 (DTYCS and
       TDYCS), then 7 (DTCYS, TDCYS), then 36 at 808 and 42-50 at 803.  So the
       order space is empty on that body too.

   PAIRS OF THE TWO KNOWN LEVERS, 40 candidates: the c+0x2C store in pointer
   cast or array form, x four statement orders, x a cancelled pair at four
   positions or none.  Pointer cast with no pair 2 everywhere.  Array form
   with no pair 9 everywhere.  Pointer cast with a pair 7 to 13, array form
   with a pair 10 to 18.  Nothing under 2.

   THE PARK'S RECORDED NEGATIVES WERE RE-MEASURED AND ARE ALL SOUND.  Per the
   brief's lens -- re-price every recorded negative whose word count is not
   812 -- all of these reproduce exactly on this body, so there is no stale
   number hiding a prerequisite here: the constant address on the load 18 at
   813, on the store 24 at 813, on both 21 at 813, `k' for the rmw 26 at 813,
   the e10 load through `| 0x29C' 20 at 813, `(lh + D_80083228 + 0x100)' 11,
   `(D_80083228 + (lh + 0x100))' 37 at 809, `((D_80083228 + 0x100) + lh)' 40
   at 809, the flagless `*(s16 *)(d + 0x2A)' 43 at 805, `((u16 *)d)[0x15]' 3,
   and the D_800814A0 store ahead of the c store 42 at 803.

   ====================================== 2026-11-22: 11 -> 2 rows at 812/812

   FOUR CHANGES IN CASE 10.  Each is listed with what it is worth and the
   mechanism; none of the first three is worth anything without the one
   before it, which is why the park sat at 11 for a day.

   (1) THE d+0x60 STORE MOVES AHEAD OF THE c+0x2C TABLE STORE, and
   (2) CASE 10's D_800814A0 TEMP STOPS SHARING `x' WITH CASE 9's p+0xC0 BYTE.
       Together 11 -> 9.  (1) alone is 19 rows; (2) alone is 11, no change.
   (3) THE TABLE STORE IS `*(u8 **)(c + 0x2C)', NOT `((u8 **)c)[0xB]'.
       9 -> 2.
   (4) THE TABLE GETS ITS OWN `u8 *t10' LOCAL, ASSIGNED BEFORE THE
       D_800814A0 LOAD, used by both the store and the call's index.
       3 -> 2 in the load-before-store basin.  THIS REVERSES the park's
       "THE SIX FACING TABLES ARE NAMED TWICE, NOT BOUND TO A u8 *t" for
       CASE 10 ONLY; the other five sites still have no local and are
       byte-exact.

   WHY (1), AND IT IS A GENERAL WAY TO READ A SOURCE ORDER OFF A LISTING.
   The target's case 10 has `sw $a1,0x2C($a0)' AFTER `sh $v0,-0x2($v1)' and
   after `lw $v1,0x60($s3)'.  Those pairs are dependences no scheduler may
   reorder: `(plus (reg c) 44)' against `(plus (reg d) 96)' and against
   `(plus (reg629) -2)'.  BOTH addresses vary, so the MEM_IN_STRUCT_P
   exception cannot fire, and `memrefs_conflict_p' (sched.c) bottoms out on
   two distinct REGs and returns 1 -- `reg_known_value' cannot separate two
   parameters either, since their single sets are the incoming arg hard regs.
   A DEPENDENCE IS IN PROGRAM ORDER, so if the target's asm has A after B and
   A/B cannot be disambiguated, the TARGET'S SOURCE HAS B FIRST.  That is a
   proof, not a guess, and it is what pointed at a basin the park's own row
   count had rejected at 19.

   It also explains the target's `sw ...,0x2C($a0)', which the park had
   recorded as unexplained: with the store after the arg copy in the
   post-sched1 chain and `c' not dead at the copy, `optimize_reg_copy_1'
   (local-alloc.c, called from update_equiv_regs) rewrites the later use of
   `c' to the hard reg $a0.  CAUTION, measured here: reorg.c:3700 UNDOES
   that substitution -- "if this insn is a register-register copy and the
   next insn has a use of our destination, change it to use our source" --
   whenever the store is the IMMEDIATE `next_nonnote_insn' after the copy.
   On the 9-row body local-alloc produced `0x2C($a0)' and reorg turned it
   back into `0x2C($s1)'; the dumps show it flipping between `jump2' and
   `dbr'.  So the $a0 spelling needs the store NOT adjacent to the copy.

   WHY (2).  In the target $v1 is busy CONTINUOUSLY from aligned 749 to 785,
   which is what forces the D_800DD140 quantity out of $v1 and into $a1.
   With `x' shared between case 9 and case 10 it is a GLOBAL allocno, so at
   local_alloc time $v1 is not yet reserved, the block-local table quantity
   grabs $v1, and global_alloc then has to put `x' in $a1 -- which also moved
   case 9's byte to $a1 and cost two rows there.  THIS REVERSES the
   2026-11-21 finding below ("sharing the case 9 byte with the case 10 rmw
   measures 23, splitting 26"): that was measured on the C-before-D body and
   is a STALE POSITIVE.  A shared-local decision is only valid inside the
   statement-order basin it was measured in.

   WHY (3).  `((u8 **)c)[0xB]' sets MEM_IN_STRUCT_P (expr.c:5344 -- the top
   node of the INDIRECT_REF's address is a PLUS_EXPR), and the D_800814A0
   load and store have `(lo_sum (reg) (symbol_ref))' addresses, which
   `rtx_varies_p' (rtlanal.c:103) reports as NON-varying.  That is exactly
   the shape `true_dependence'/`output_dependence' (sched.c:835) make an
   exception for, so with the array form the table store has NO dependence on
   the D_800814A0 pair and floats; with the NOP_EXPR cast the flag is clear,
   the exception cannot fire in either direction, and the store is PINNED
   between them -- which is where the target has it.  So the lever is the
   same MEM_IN_STRUCT_P question as the 2026-11-21 d+0x2A entry, used in the
   OPPOSITE direction: there we wanted a dependence gone, here we want it
   kept.  152 bodies with the array form all measure 9; the same bodies with
   the pointer cast measure 2.

   THE ALLOCATOR ARITHMETIC, confirmed against the compiler's own dumps.
   local-alloc.c:1693

       QTY_CMP_PRI = floor_log2(n_refs) * n_refs * size / (death - birth) * 10000

   where (a) `n_refs' is the SUM of reg_n_refs over every pseudo combine_regs
   tied into the quantity (`qty_n_refs[sqty] += reg_n_refs[sreg]',
   local-alloc.c:1939) -- the D_800DD140 quantity is the `%hi' pseudo AND the
   `lo_sum' pseudo, tied at the lo_sum because the %hi dies there, so it is
   2+3 = 5 and NOT the 3 that `.lreg' prints for the lo_sum alone; (b) birth
   and death are `2*insn_number (+1 for an output)' over the POST-SCHED1
   order, so death-birth = 2*(span in insns) - 1, and this is NOT
   reg_live_length; (c) `size' is in WORDS, so 1 here.  On the 11-row body
   that was TBL 2*5/39 = 2564 against the d+0x2A load's 1*2/5 = 4000, so the
   load went first and took the lower register.

   A SMALL INSTRUMENT IS WORTH MORE THAN A SWEEP HERE.  Compile the
   `onefunc.py' standalone with `-dl', parse the `.lreg' dump for
   `Register N used M times', `;; Register N in R.' and the insn order, and
   print each quantity's n_refs / span / PRI / hard reg.  That is 0.05 s a
   candidate -- CHEAPER than aligndiff -- and it says whether a candidate
   moved the allocator at all before you spend a measurement on it.  It is
   also what proved the do-while(0) family below was hitting the target's
   register pair and losing on everything else.

   MEASURED AND REJECTED.  ON THE CURRENT (2-ROW) BODY:
     * all 20 legal orders of {D, T(t10=), Y(z=load), C(store), S} after the
       e10 chain, with D<C, T<C, Y<S: DTCYS / DTYCS / TDCYS / TDYCS all 2;
       DYTCS 3; TYDCS / YDTCS / YTDCS 16; the eight orders that put the
       D_800814A0 STORE before C are 803 words / 42 rows.
     * THE FOUR SWEEPS WERE RE-RUN ON EVERY BODY THIS PARK PASSED THROUGH.
       On the 9-ROW body widthsweep ("no width change helps"), stmtsweep
       ("no safe move helps") and devsweep ("no wrapper helps", 4760
       candidates per round) all came back empty; the park's older entries
       record all four empty on the 11-row and 23-row bodies.  ON THIS
       2-ROW BODY: widthsweep "no width change helps", stmtsweep "no safe
       move helps" (6 movable assignments), devsweep "no wrapper helps"
       (4959 candidates per round) and refsweep "no cancelled pair helps"
       (28 names, 7680 insertion points) ALL CAME BACK EMPTY, each having
       confirmed the 2-row baseline on its own standalone first.  That is
       the brief's stopping condition for the sweep cycle; the next spend on
       this park is the sched1 class/LUID question above, or the permuter on
       THIS body (the 2026-11-21 permuter run below is spent on a body that
       no longer exists -- it was 11 rows, four changes ago).
     * THE ARM IS INSENSITIVE TO EVERY REMAINING MINOR SPELLING, all 2:
       case 10's own `if' condition in four forms (both `(u8 **)d)[0x18]'
       and `((u32 *)...)[7]', and with an explicit `!= 0'); `e10[3]' as
       `*(u8 *)(e10 + 3)'; `~0x4000' written `0xFFFFBFFF'; `(u32)0x8000';
       the d+0x60-2 `|=' split into an explicit load/or/store;
       `t10 = &D_800DD140[0]'; and the call's index as `*(t10 + i)'.
     * THE `RTX_UNCHANGING_P' ESCAPE IN `anti_dependence' IS NOT REACHABLE
       FROM A CAST.  sched.c's anti_dependence returns 0 outright when the
       READ is unchanging, which is exactly the link we want gone, but
       `z = *(const u32 *)&D_800814A0;', the same through `const s32 *', the
       array form `((const u32 *)&D_800814A0)[0]', and a `const u32 *' local
       all measure 2 -- identical to the unqualified load, so cc1psx is not
       setting the flag from the cast's TREE_READONLY.  `volatile' is also 2.
     * D_800814A0 AS A CONSTANT ADDRESS (the trick case 2 needs for
       D_80013714) COSTS A WORD HERE: `*(u32 *)0x800814A0' on the load 18
       rows / 813 words, on the store 24 / 813, on both 21 / 813.  A `u8 *'
       local holding 0x80080000 with `+ 0x14A0' inside the index is 2 at 812
       -- identical to the symbol, and no better.
     * `u32 *a10 = &D_800814A0;' with the load and store through `*a10' is
       also 2 at 812 words (the `addiu %lo' folds back into the load and the
       store, so the address local is free) -- it does not move the residue.
     * MOVING THE `t10 = D_800DD140;' ASSIGNMENT EARLIER THAN THE e10 CHAIN
       IS A REGRESSION, NOT A FREE CHOICE: ahead of the func_8009A028 call
       it is 31 rows at 810 words, and outside the `if' 29 rows at 811.  It
       is free anywhere from just after that call to just before
       `z = D_800814A0;' (six positions, all 2).
     * all 64 combinations of where `u8 *t10' and `s32 z' are DECLARED, over
       eight positions each in the declaration block: 2 everywhere, so the
       declaration order is free and does not need preserving.
     * which local holds the D_800814A0 value: a fresh `s32 z' 2, `x' 3,
       `y' 7, `v' 17, `k' 26 at 813 words.  Widths of `z': s32 / u32 2,
       u16 / u8 3, s16 26 at 814 words.  `t10' as `u8 *' 2, as `u8 **' 33
       at 815 words.
     * t10's assignment position (six places in the arm) and whether t10 is
       used by the store, by the call's index, or by both: all 2.
     * eight spellings of the D_800814A0 read-modify-write (`|=' as one
       statement, `D_800814A0 = D_800814A0 | 0x8000', a local for the loaded
       value, a local for the or'd value, `z |= 0x8000' in the middle,
       `*(u32 *)&D_800814A0', `(&D_800814A0)[0]'): with the c store BEFORE
       them all 2 (the dead basin), with the c store BETWEEN the load and the
       store all 2-3, with the c store AFTER the store 803 words / 42 rows.
     * the d+0x8A load as `((s16 *)d)[0x45]' or with `| 0x29C' folded into
       the index, and the three `*(u8 **)(d + 0x60)' reads as
       `((u8 **)d)[0x18]': 2 in every combination (measured on the 9-row
       body, which shares case 10's structure), so none of those four casts
       matters.
     * THE d+0x2A CAST AND THE INDEX SUM ARE BOTH STILL LOAD-BEARING -- 20
       candidates, four casts x five sums, re-measured ON THIS BODY:
       `((s16 *)d)[0x15]' with the sum as written is the unique 2.  Same
       cast, other sums: `(lh + D_80083228 + 0x100)' 11,
       `(D_80083228 + (lh + 0x100))' 37 at 809 words,
       `((D_80083228 + 0x100) + lh)' and `(0x100 + D_80083228 + lh)' 40 at
       809 words.  `((u16 *)d)[0x15]' with the sum as written is 3 (NOT
       identical, which the 2026-11-21 entry below claimed), and the two
       flagless pointer casts are 43 rows at 805 and 808 words.
     * `((u32 *)e10)[5]' for the +0x14 mask, `((s16 *)d)[0x45]' for the
       d+0x8A load, `((u8 **)(base10 + 0x29C))[...]' for the e10 load, and
       all three `*(u8 **)(d + 0x60)' reads (including func_8009A028's
       argument) as `((u8 **)d)[0x18]': 2 in every case, so none of those
       casts matters.  `(u32)base10 | 0x29C | (idx * 4)' is 20 rows at 813
       words, so the `+ 0x29C' inside the index is load-bearing.
     * A SEPARATE LOCAL `q10' FOR THE D_800E3DF0 ENTRY IS NOW WORTH 2 ROWS
       AGAINST IT (4 rows vs 2), so the 2026-11-21 "e10 AGAIN, NOT A NEW
       PSEUDO" entry still holds on this body -- it was neutral on the 9-row
       body, which is one more reason to re-price a park's claims after every
       structural change.
   ON THE 11-ROW BODY (kept because each has a mechanism attached):
     * A LOCAL COPY OF `c' FOR THE STORE AND THE CALL -- `cc = c;' with the
       store, the call, or both through `cc', at ten statement positions:
       all 11.  WITH A CANCELLED PAIR as well (`cc++; cc--;' at every
       position after the assignment, one pair and two, 330 candidates): 290
       of them 11 and 40 of them 17.  cse folds the copy away at every use,
       because `cc' is provably `c'.  The $a0 spelling is NOT reachable this
       way -- it comes from optimize_reg_copy_1 and therefore from the
       STATEMENT ORDER, see above.
     * HOISTING THE d+0x2A LOAD INTO A LOCAL to lengthen its quantity's life
       (which does flip the priority: a fresh `s32'/`u32' local takes the lh
       span from 3 insns to 18 and the PRI from 4000 to 571): `s32 jj' 13 at
       best, `u32 jj' 14, `s16 jj' 43, `v' 25, `y' 16, `k' 51 -- over nine
       insertion positions each.  The hoisted pseudo takes $t0 and collides
       with D_800814A0's base.
     * `do { } while (0);' OVER 138 WRAPPER PLACEMENTS -- every contiguous
       run of case 10's eleven statements plus 72 two-wrapper combinations.
       This is the n_refs lever: flow.c:441 starts `depth' at 1 and
       NOTE_INSN_LOOP_BEG bumps it, and flow.c:2122 does
       `reg_n_refs[regno] += loop_depth', so a wrapper DOUBLES the refs of
       every pseudo inside it.  Wrapping statements i..6 takes the table
       quantity to n_refs 9, PRI 6923, and gives EXACTLY the target's
       register pair -- and still measures 26 rows at 814 words, because the
       same doubling re-prices every other quantity in the arm.  Best
       wrapper 26; most 31-64; eleven exceed 812 words.
     * 192 assignments of the four scalar temp roles (case 2's amount,
       case 9's func_80099090 result, case 9's p+0xC0 byte, case 10's rmw)
       over {x, y, v, z}: nothing beat the split described above.
     * 420 legal orders of the eight case-10 statements with the ARRAY-form
       store: best 9 (this body's order), then 13, 16, 19-45.

   LEVERS THAT WERE WORTH A LOT AND ARE ALREADY IN THE BODY:

   * EVERY D_80083460 BASE AND EVERY SCRATCH POINTER NEEDS ITS OWN LOCAL.
     One `h` shared by cases 2, 3 and 7 crosses a call in case 3, so global
     alloc gives the single pseudo a CALLEE-SAVED register everywhere and
     case 2's `lui/addiu D_80083460` comes out in $s0 where the target has
     $v1.  Splitting h/h2/h3/h4 (and r/e2/e7/e9/e10/n5/base9/base10) took
     the function from 280 aligned differences to 96 IN ONE STEP and, with
     it, fixed the whole parameter register map: before the split the
     allocation order was b, g, c, p, d and after it g, c, p, d, b, which
     is the target's.  Nothing else moved that map.

   * THE SIX FACING TABLES ARE NAMED TWICE, NOT BOUND TO A `u8 *t`.
     func_8008E264's rule transfers verbatim: with the local, the
     `addu $a0, $s1, $zero` the target emits BEFORE the table store is
     pushed past it at all six sites.  325 aligned differences with the
     local, 280 without.

   * CASE 2's FIRST D_80013714 READ MUST BE THE CONSTANT ADDRESS
     `*(u16 *)0x80013714`; the other four must be structs.h's SYMBOL.
     As the symbol, the `lui` is two insns and reorg steals it into the
     delay slot of the preceding `bnez $v0, .L800B7750`, where the target
     has a `nop` -- func_8008E0C4 records exactly this.  As the symbol in
     case 7 the `%hi` is CSE'd into $s3 and serves three reads, which the
     assembler macro cannot do, so that site must NOT be the constant.

   * CASE 1's THREE STORES ARE 0xC0, D_80081484, 0x8C -- in that order,
     with the +0x8C one a COMPONENT_REF (`((UnkDungeonActor *)p)->unk_8C`).
     The component ref is what lets sched sink the D_80081484 store past it
     (true_dependence's in-struct/varying exception); written
     `*(u32 *)(p + 0x8C)`, or with the 0x8C store ahead of the D_80081484
     one, the `lui %hi(func_800B69DC)` is stolen into the delay slot of the
     `bnez $v0, .L800B6C80` above and the block comes out in the wrong
     order.  Worth 10 aligned differences (75 -> 65).

   * CASE 4's +0x8C STORE IS ALSO A COMPONENT_REF, for func_800969B8's
     reason: through `*(u32 *)(p + 0x8C)` the store may alias D_800E296C
     and sched1 cannot lift that global's `lw` above it.

   * EVERY p+0xC0 ACCESS IS `((u8 **)p)[0x30]`, NOT A CAST.  The array form
     sets MEM_IN_STRUCT_P, which is what lets case 7's `lw $a0, 0xC0($s2)`
     move above the `sb` to D_800E2968.

   * CASE 9's e9[0] SOURCE BYTE IS READ INTO A TEMP BEFORE THE e9[3]/e9[2]
     STORES.  The target's `lw`/`nop`/`lbu` pair sits between `sb $v1, 0x1`
     and `sb $v0, 0x3`, and a byte store is QImode, which true_dependence
     never disambiguates -- so no spelling moves the load across it.  Only
     writing it out in that order puts it there.  Worth 10 (85 -> 75).

   * CASE 2's FIRST TEST IS WRITTEN `== 0` SO THE D_80083460 ARM IS THE
     FALL-THROUGH; written the other way round the branch is inverted.

   * D_80083228 IS D_80083160 + 0xC8, AND BOTH SPELLINGS APPEAR.  Case 0
     reads it as `q->unk_C8` off its own `Unk80083160 *q` (func_800927E4's
     measured shape, which this arm is a line-for-line copy of); every other
     arm reads the absolute `D_80083228`.

   * D_8001029C IS REACHED AS func_80098FF8 DOES IT: a `u8 *` local holding
     0x80010000 with 0x29C inside the index.

   Target 812 words = 0xCB0 bytes.  jtbl_80089310 is at rodata 0x89310,
   0 mod 8, so this function is safe to land (func_80095538 is the
   precedent). 
   DECLARATIONS (added 2026-11-16: this park did not compile from the repo
   alone -- seven symbols it uses were declared only in its author's
   transcript, so parkresidue could not measure it at all.)
     extern u32 D_800835E4[];
     extern u8 D_800DD108[];
     extern u8 D_800DD110[];
     extern u8 D_800DD118[];
     extern u8 D_800E0A2A[];
     extern u8 D_800E0A33[];
     extern u8 D_800E2968;
     typedef u8 *(*UnkDungeonSpawner)(s32, u8, u8, s16);
   END DECLARATIONS
   UnkDungeonSpawner was used by the body and defined nowhere -- reconstructed
   2026-11-16 from the `jalr $v0` at 800B7140: four arguments in $a0..$a3 as
   int / lbu c[0x24] / lbu c[0x25] / a halfword sign-extended by the sll+sra
   pair at 800B713C, and $v0 used as a pointer immediately after
   (`lbu 0x13($s0)`, `lw -0x14($s0)`).
   Shapes read off asm/nonmatchings/ovl_dungeon/func_800B6AC4.s: the six
   arrays are lui/addiu address pairs, D_800E2968 is lui + `sb %lo(..)` and
   so is a scalar byte, not an array.
   ====================================== 2026-11-21: 13 -> 11 rows at 812/812

   CASE 10's D_800E3DF0 ENTRY POINTER IS `e10' AGAIN, NOT A NEW PSEUDO:

       e10 = (u8 *)D_800E3DF0[e10[3] & 0x1F];
       *(u32 *)(e10 + 0x14) &= ~0x4000;

   The target holds both in $a0 (`lw $a0, %lo(D_8001029C)($v0)' at aligned
   755 and `lw $a0, 0x0($v0)' at 762); written as one expression the first
   one takes $v0 and costs aligned 755 and 757.  Measured on the 13-row body:
   this reuse 11, a SEPARATE local `q10' for the entry 13, a local for
   `e10[3] & 0x1F' 13, dropping `e10' entirely 13, `u8 **e10' 13.
   Re-sweeping the statement order afterwards keeps B1,B2,D,C,Y,S best at 11
   (B1 = the e10 reassignment): 1D2CYS and D12CYS are 13, anything with C
   ahead of D is 19 or worse.

   ====================================== 2026-11-21: 23 -> 13 rows at 812/812

   CASE 10's `*(s16 *)(d + 0x2A)' MUST BE AN IN-STRUCT REFERENCE, AND WITH IT
   THE ORDER BECOMES B, D, C, Y, S.  Two changes, measured together because
   neither is worth anything alone (the array cast alone is 46 rows / 807
   words; B,D,C,Y,S on the plain cast is 36).

   WHY.  The residue was one cascade, and `tools/decomp/sched.py' names its
   root: in sched1 the `D_800814A0' store (insn 1867) is a PREDECESSOR of the
   `lh 0x2A($s3)' load (insn 1878) -- sched.c's `true_dependence' cannot
   disambiguate a store whose address is `(lo_sum (reg) (symbol_ref))' from a
   load whose address is `(plus (reg) 42)'.  The store is therefore pinned
   ahead of that load, which pins the D_800814A0 base pseudo's live range
   short, which lets local-alloc give the base $a2 -- and then the arg-setup
   `addu $a2, $zero, $zero' has to follow the store (WAR), so it lands at the
   bottom of the block instead of the top, and the whole caller-saved file
   below it rotates.  The target has the store as the LAST insn of the arm.

   `true_dependence' (scratch/gcc-src/cdk-gcc-b18/sched.c) has exactly one
   exception that applies here: the load is MEM_IN_STRUCT_P and its address
   varies, the load is not QImode, and the STORE is not in-struct and its
   address does not vary.  `rtx_varies_p' (rtlanal.c:103) returns the
   varies-ness of a LO_SUM's *symbol*, so the D_800814A0 store's address
   already does not vary -- the only missing half is MEM_IN_STRUCT_P on the
   load, which `*(s16 *)(d + 0x2A)' does not have and an ARRAY_REF through an
   ARRAY-TYPED pointer does.  With the dependence gone the store schedules
   last, the base takes $t0, `addu $a2,$zero,$zero' moves to the top of the
   arm, `addu $a0,$s1,$zero' lands at its target position 771, and the
   D_800814A0 store itself is byte-exact.

   THE SPELLING IS ORDINARY C AND NEEDS NO WITNESS.  CORRECTED 2026-11-21:
   this paragraph used to say `((s16 *)d)[0x15]' was a placeholder for a
   COMPONENT_REF on an unwitnessed struct type.  That was wrong.
   MEM_IN_STRUCT_P is set when the TOP NODE of the INDIRECT_REF's address
   tree is a PLUS_EXPR (expr.c:5344) -- a question about where the cast sits,
   not about struct types -- so a pointer cast and an index is already the
   honest form and a struct would buy nothing.
   Measured equivalents, all 13 rows ON THE BODY OF THAT DATE: `u16' instead
   of `s16', and the same shape applied to d+0x8A, d+0x60, c+0x2C or the
   D_800E3DF0 entry (none of which changes anything -- the d+0x2A load is the
   only one whose dependence matters).  Two spellings that do NOT work:
   `(*(s16 (*)[1])(d + 0x2A))[0]' is 23, because a one-element array folds
   back to an INDIRECT_REF before expand; and the plain `*(s16 *)(d + 0x2A)'
   is 818 words / 46 rows on the current body, because the cast on top is a
   NOP_EXPR and the flag is never set.
   Writing the sum the other way round, `((s16 *)d)[0x15] + D_80083228',
   is 24.

   STATEMENT ORDER, re-measured on the in-struct spelling, all 60 legal orders of
   {B, D, C} x (Y before S): B,D,C,Y,S 13; D,B,C,Y,S 15; B,C,D,Y,S 21;
   B,C,Y,D,S 21; B,C,Y,S,D 21; Y,B,C,D,S 23.  On the flagless cast the same
   sweep makes B,Y,D,C,S and B,D,Y,C,S the joint best at 23, so the two
   changes have to be made together.

   WHAT IS LEFT -- 13 rows, all in case 10, one cluster:
     * the D_800DD140 pseudo takes $a3 and the `lh 0x2A' takes $a1; the
       target has them the other way round.  That is 3 rows directly
       (aligned 778, 780, 784) ...
     * ... and it drags the table's `addiu %lo' to aligned 752 (the target
       has it at 763, filling the load-delay after `lw $a0, 0x0($v0)'),
       which moves the `((u8 **)c)[0xB]' store to 768 where it is spelled
       `sw $a3, 0x2C($s1)' through `c' itself instead of the target's
       `sw $a1, 0x2C($a0)' at 776, and pushes `addu $a2,$zero,$zero' from
       750 to 763.
     * e10 takes $v0 where the target takes $a0 (aligned 755, 757).
   MEASURED AND REJECTED ON THIS BODY: every do-while(0) over every
   contiguous run of case 10's ten statements (best 13 = no change; 15 for
   the whole arm), a cancelled `v++; v--;' for all 26 names at all 11
   statement positions in case 10 (best 13 = no change), `*(u8 **)(c + 0x2C)'
   and the array form for the c+0x2C store (both 13), a separate local for
   the D_800E3DF0 entry or for the table (13), and widthsweep and stmtsweep
   over the whole body (no change).

   ====================================== 2026-11-21: 26 -> 23 rows at 812/812

   CASE 10's D_800814A0 TEMP IS `x', THE SAME LOCAL CASE 9 READS p+0xC0 INTO.
   Case 2's residue (the `addiu $v0, $v0, 0x1' / `subu' pair at aligned 224)
   is gone with it, so cases 0-9 and 11 are now byte-exact and the whole
   residue is case 10.  The sweep that found it enumerated which NAME each of
   four temp roles uses -- case 2's amount, case 9's func_80099090 result,
   case 9's p+0xC0 byte, case 10's read-modify-write -- over {k,x,y,v}, 96
   legal assignments.  Every assignment that gives the three roles
   (case2) / (case9 result) / (case9 byte + case10 rmw) three DISTINCT names
   with the last two SHARED measures 23; every one that instead shares
   case 2's with case 10's measures 26; sharing case 2 with case 9 is 37.
   The widths of the three names do not matter -- all six renamings of
   {x,y,v} onto those roles measure 23 exactly.

   ====================================== 2026-11-21: 37 -> 26 rows at 812/812

   CASE 2's `v' IS `y' -- the SAME LOCAL case 10 uses for its D_800814A0
   read-modify-write.  Residue items (1) and (3) are both gone: case 9 is now
   byte-exact.  The mechanism is which pseudo ends up a GLOBAL allocno.  With
   `v' shared by case 2 and case 9, `v' is global, conflicts with hard $v0,
   $v1 and $a0 (greg: ";; 105 conflicts: ... 2 3 4"), and global-alloc gives
   it $a1 -- so case 9 needs `move $a1, $v0' after the func_80099090 call and
   the freed $v0 lets the `lui %hi(D_800E3DF0)' float up into the load-delay
   stall the target leaves as a `nop'.  Moving case 2's temp onto `y' makes
   `v' BLOCK-LOCAL to case 9, local-alloc gives it $v0, and the whole of case
   9 falls out.  Measured on the 37-row body, case 2's temp as:
   `y' 26, `x' 31, `k' 30 (811 words), `v' (the park's own) 37.
   DO NOT re-split `v' into two locals: a separate `v9' declaration measured
   84 rows / 819 words (the frame grows 8 bytes) at every declaration
   position and every width tried.

   ====================================== 2026-11-21: 39 -> 37 rows at 812/812

   CASE 7's `D_800DCF4F = 1;' MOVES TO THE TOP OF ITS ARM, ahead of the two
   `h3' statements.  That is residue item (2) below closed: with the store
   written last the `lui %hi(D_800DCF4F)' is emitted after the whole
   `lui/addiu D_80083460' pair; written first it lands between them, which is
   where the target has it.  39 -> 37.  Measured on the same body:
   `D_800DCF4F = 1;' in the MIDDLE is 39 (no change), splitting the `+= 1'
   into an explicit load/store is 39, and dropping the `h3' local is 52/811.

   ====================================== 2026-11-20: 46 rows at 811/812 words
                                          -> 39 at 812/812

   ONE DECLARATION: `s32 k' became `s16 k'.  That is the whole change -- it
   supplies the missing word AND seven rows.  A permuter find, and the kind
   that is worth running the permuter for: nothing in the park's diagnosis
   pointed at `k''s width, and no amount of reading the residue would have.

   The park could not be measured AT ALL until today, for two reasons that had
   nothing to do with the body: csrc/ovl_dungeon_decls.h declared this
   function `void' while it returns s32 (so no parksplice placement compiled),
   and func_800B69DC -- defined in the PREVIOUS carved unit and called only
   from here -- had no prototype in this unit, because the carve generated
   cross-unit prototypes only for callees that LANDED code mentions.

   ====================================== 2026-11-21: the cast is no longer a
                                          device.  Still 11 rows, 812/812.

   The body said `(*(s16 (*)[0x16])d)[0x15]' and called it a device in
   disguise -- a two-dimensional array cast whose only purpose was to make the
   load an ARRAY_REF so MEM_IN_STRUCT_P would be set.  It is now
   `((s16 *)d)[0x15]', which measures IDENTICALLY (11 rows, 812/812) and is an
   ordinary pointer cast and index.

   WHY THE FANCY CAST WAS NEVER NEEDED.  expr.c:5344 sets MEM_IN_STRUCT_P when
   the TOP NODE of the INDIRECT_REF's address tree is a PLUS_EXPR.  That is a
   question about where the cast sits, not about struct types:

       *(s16 *)(d + 0x2A)     top node is the CAST (NOP_EXPR)  -> flag NOT set
       ((s16 *)d)[0x15]       top node is the PLUS_EXPR        -> flag SET

   No struct, no typedef and no usage witness is required.  The park's long
   note about not being able to witness a struct for `d' was answering a
   question that did not need to be asked.  See the CORRECTION entry in
   docs/matching-levers.md.

   For the record, on THIS body (with the B,D,C,Y,S statement order that the
   flag makes possible) the flagless form `*(s16 *)(d + 0x2A)' is 818 words
   and 46 rows -- the two halves are still only worth anything together.

   ====================================== 2026-11-21: PERMUTER EXHAUSTED, 11 rows

   150 minutes, 4 jobs, 219965 candidates, base score 645 -> best 485, eight
   finds.  MEASURED AGAINST THE PARK, every one is worse or equal:

       output-485-1 (best score)   19 rows, 812/812
       output-505-2                21
       output-505-1 / 515-1        22
       output-580-1                33 rows, 813 words
       output-550-1                64 rows, 814 words
       output-585-1                65 rows, 813 words
       output-630-1                11 rows, 812/812   (ties, no gain)

   `no find beats the park'.  This is the documented divergence: the permuter
   scores the whole object with its own diff, aligndiff counts rows in this
   function's words, and below roughly ten rows the two point in different
   directions -- the BEST-SCORING find is nearly the WORST on rows here.

   So all four sweeps are clean AND the permuter is spent on this body.  The
   remaining 11 rows are the single local-alloc choice described above
   ($a3 vs $a1 for the D_800DD140 table pseudo, both quantities block-local).
   The next spend should be hand analysis of QTY_CMP_PRI on a CHANGED body,
   not another search on this one.
 */
void func_800B6AC4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk80083160 *q;
    u8 *g;
    u8 *h;
    u8 *h2;
    u8 *h3;
    u8 *h4;
    u8 *n;
    u8 *m;
    u8 *r;
    u8 *e2;
    u8 *e7;
    u8 *e9;
    u8 *e10;
    u8 *n5;
    u8 *base9;
    u8 *base10;
    u8 *t10;
    u8 *w;
    void *s;
    s16 k;
    s32 x;
    u32 y;
    s32 v;
    s32 z;
    s16 out[4];

    g = (u8 *)&D_80083160;
    switch (p[0x9B])
    {
    case 0:
        q = &D_80083160;
        if ((((q->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7) == 2)
        {
            *(u8 **)(c + 0x2C) = D_800DD108;
            func_80048A44(c, D_800DD108[((q->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            *(s16 *)(p + 0x96) = 0x12;
            p[0x9B]++;
        }
        else
        {
            *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
        }
        break;
    case 1:
        if (--*(s16 *)(p + 0x96) == 0)
        {
            func_800A56E0(0x701);
        }
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            *(u8 **)(c + 0x2C) = D_800DD110;
            func_80048A44(c, D_800DD110[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
            if ((D_80013714 & 1) == 0)
            {
                func_800B1768(0x22, 0xF2, 0xBA, 0x105, 0, 8);
                func_800B1768(0x23, 0xE0, 0xCE, 0x104, 1, 8);
            }
            ((u8 **)p)[0x30] = *(u8 **)(p + 0xBC);
            *(u32 *)D_80081484 = **(u32 **)(p + 0xBC);
            ((UnkDungeonActor *)p)->unk_8C = func_800B69DC;
        }
        break;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            h = (u8 *)&D_80083460;
            *(u16 *)(h + 0x2) |= 4;
        }
        else
        {
            *(u16 *)(c + 0x14) |= 0x6800;
        }
        if (D_80083462 & 4)
        {
            break;
        }
        k = 0;
        if (*(u16 *)0x80013714 & 1)
        {
            r = func_8009F868();
            if (r == NULL)
            {
                func_800B1768(0x22, 0xF2, 0xBA, 0x105, 0, 8);
                func_800B1768(0x23, 0xE0, 0xCE, 0x104, 1, 8);
                break;
            }
            D_800E3544 = r[1] & 0xF8;
            if (D_800E3544 == 0x60)
            {
                k = 1;
            }
            else
            {
                func_8009F988();
                *(u8 **)(c + 0x2C) = D_800DD118;
                func_80048A44(c, D_800DD118[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
                *(u16 *)(c + 0x14) &= 0xF7FF;
                *(s32 *)(p + 0x8C) = 0;
                p[0x9B] = 4;
                break;
            }
        }
        else
        {
            if (*(s32 *)(g + 0x8) & 0x40)
            {
                k = 1;
            }
        }
        if (k)
        {
            func_8009F644(d, 0x60, 0, 0);
            func_800A56E0(0x500);
            *(u16 *)(c + 0x14) &= 0x97FF;
            p[0x9B] = 3;
            y = (func_800A6D30() & 3) + 1;
            (*(u8 **)(p + 0xBC))[2] -= y;
            e2 = *(u8 **)(p + 0xBC);
            if (*(s8 *)(e2 + 0x2) <= 0)
            {
                e2[2] = 0;
                *(u8 **)(c + 0x2C) = D_800DD118;
                func_80048A44(c, D_800DD118[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
                *(u16 *)(c + 0x14) &= 0xF7FF;
                p[0x9B] = 5;
                *(s16 *)(d + 0x8A) = func_8009904C(*(u8 **)(p + 0xBC));
            }
            func_800B4C7C(0xA4, d, (s16)(0x64 - *(s8 *)(*(u8 **)(p + 0xBC) + 0x2)), 0);
        }
        else
        {
            if (D_80013714 & 1)
            {
                break;
            }
            if ((*(s32 *)(g + 0x10) & 0x20) == 0)
            {
                break;
            }
            func_8009F644(d, 0xA8, 0, 0);
            *(u8 **)(c + 0x2C) = D_800DD118;
            func_80048A44(c, D_800DD118[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            *(u16 *)(c + 0x14) &= 0xF7FF;
            func_800A56E0(0x515);
            *(s32 *)(p + 0x8C) = 0;
            p[0x9B] = 4;
        }
        break;
    case 3:
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            h2 = (u8 *)&D_80083460;
            if ((*(u16 *)(h2 + 0x2) & 2) == 0)
            {
                func_80099F70(*(u8 **)(d + 0x5C));
                func_80099F04(*(u8 **)(d + 0x5C));
                *(u16 *)(h2 + 0x2) |= 0x812;
            }
            p[0x9B] = 2;
        }
        break;
    case 4:
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            func_80098B38(D_80081484);
            ((UnkDungeonActor *)p)->unk_8C = func_8008ACDC;
            D_800E296C &= 0xFFDFFFFF;
        }
        break;
    case 5:
        n5 = (u8 *)func_80098CF8(p, b, c, D_80081484);
        if (n5 != NULL)
        {
            *(s16 *)(n5 + 0xCC) = 2;
            p[0x9A] = 0x22;
            p[0x9B] = 6;
        }
        break;
    case 6:
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            if (func_8003DE58(*(u8 **)(c + 0x8), c, out, 0) == 0)
            {
                out[1] = 0;
                out[0] = 0;
                out[2] = -D_800DDC40[0];
            }
            func_800B8024(*(s16 *)((u8 *)b + 0x2) + out[0], *(s16 *)((u8 *)b + 0x6) + out[1],
                          *(s16 *)((u8 *)b + 0xA) + out[2]);
            p[0x9B]++;
        }
        break;
    case 7:
        D_800E2968 = 1;
        n = ((UnkDungeonSpawner)func_800A0B94((((u8 **)p)[0x30])[0],
                                              func_800A1618((((u8 **)p)[0x30])[0], 2), 1))(
            2, c[0x24], c[0x25], (s16)(*(u16 *)(d + 0x88) - 0x20));
        if (n == NULL)
        {
            break;
        }
        func_8003E188(n[0x13], 0);
        func_800A90E8(((u8 **)p)[0x30]);
        e7 = *(u8 **)(n - 0x14);
        func_8009A3D0(e7[0x24], e7[0x25], 0x300);
        if (func_8003DE58(*(u8 **)(c + 0x8), c, out, 0) == 0)
        {
            out[1] = 0;
            out[0] = 0;
            out[2] = -D_800DDC40[0];
        }
        m = *(u8 **)(n - 0x18);
        *(s16 *)(m + 0x2) = *(u16 *)((u8 *)b + 0x2) + (u16)out[0];
        *(s16 *)(m + 0x6) = *(u16 *)((u8 *)b + 0x6) + (u16)out[1];
        *(s16 *)(m + 0xA) = *(u16 *)((u8 *)b + 0xA) + (u16)out[2];
        *(s16 *)(n + 0x88) = *(s16 *)(m + 0xA);
        *(u8 **)(d + 0x60) = n;
        func_80042640((Unk42710 *)n, n[0x13]);
        func_800424E0(n, n[0x13], NULL);
        n[0x43] = 0xFF;
        *(u8 **)(n + 0x60) = d;
        *(u32 *)(n + 0x1C) |= 0x80000;
        *(u32 *)(n + 0x14) |= 0x104000;
        *(u16 *)(*(u8 **)(d + 0x60) + 0x98) |= 0xC;
        func_80069E78(n + 0x34, func_8003C06C(n[0x13]));
        *(u32 *)(n + 0x18) = D_800835E4[D_800E3D7C[0x11]];
        while (D_800835E4[n[0x11] + 1] <= *(u32 *)(n + 0x18))
        {
            func_800A1D4C(n, 0);
        }
        n[0x28] = n[0x29];
        n[0x26] = n[0x68];
        n[0x27] = n[0x69];
        if ((D_80013714 & 1) == 0)
        {
            D_800DCF4F = 1;
            h3 = (u8 *)&D_80083460;
            *(u16 *)(h3 + 0xA) += 1;
        }
        s = func_800990FC();
        w = func_8009929C(8, s);
        w = func_80099194((u8 *)D_8007359C[0][n[0x13]].unk_04, w);
        w = func_80099194(D_800E0A2A, w);
        w = func_80099734(n, w);
        w = func_80099194(D_800E0A33, w);
        if ((D_80013714 & 1) == 0)
        {
            w = func_8009929C(0x11, w);
            w = func_8009929C(0x4C, w);
            w = func_80099254(D_800E0458, w);
        }
        func_80099290(w);
        func_800A5720(s);
        *(s16 *)(p + 0x96) = 0x20;
        p[0x9B]++;
        if ((D_80013714 & 1) == 0)
        {
            h4 = (u8 *)&D_80083460;
            if (*(s16 *)(h4 + 0xA) >= 2)
            {
                *(u16 *)(h4 + 0xA) -= 1;
            }
        }
        else
        {
            h4 = (u8 *)&D_80083460;
            if (*(s16 *)(h4 + 0xA) > 0)
            {
                *(u16 *)(h4 + 0xA) -= 1;
            }
        }
        break;
    case 8:
        if (D_8008346A == 0)
        {
            if (--*(s16 *)(p + 0x96) > 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0 &&
                *(s32 *)(g + 0x10) == 0)
            {
                break;
            }
            *(u8 **)(c + 0x2C) = D_800DD130;
            func_80048A44(c, D_800DD130[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        break;
    case 9:
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            *(u8 **)(c + 0x2C) = D_800DD138;
            func_80048A44(c, D_800DD138[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            *(u16 *)(c + 0x14) |= 0x200;
            func_80093C70(p, (u8 *)b, c);
            func_80093D8C(p, (u8 *)b, c);
            v = func_80099090();
            base9 = (u8 *)0x80010000;
            e9 = *(u8 **)(base9 + *(s16 *)(d + 0x8A) * 4 + 0x29C);
            e9[1] = 0x13;
            x = (((u8 **)p)[0x30])[0];
            e9[3] = v;
            e9[2] = 0;
            e9[0] = x;
            D_800E3DF0[v] = (s32)&D_800E3E48[v];
            D_800E3E48[v] = *(Unk800E3E48 *)*(u8 **)(d + 0x60);
            *(u32 *)(*(u8 **)(d + 0x60) + 0x1C) |= 0x400000;
            *(u32 *)(*(u8 **)(d + 0x60) + 0x14) &= 0xFFEFFFFF;
            func_800A56E0(0x704);
            p[0x9B]++;
        }
        break;
    case 10:
        if (*(u32 *)(*(u8 **)(d + 0x60) + 0x1C) & 0x800000)
        {
            func_8009A028(*(u8 **)(d + 0x60));
            base10 = (u8 *)0x80010000;
            e10 = *(u8 **)(base10 + *(s16 *)(d + 0x8A) * 4 + 0x29C);
            e10 = (u8 *)D_800E3DF0[e10[3] & 0x1F];
            *(u32 *)(e10 + 0x14) &= ~0x4000;
            *(u16 *)(*(u8 **)(d + 0x60) - 0x2) |= 0x8000;
            t10 = D_800DD140;
            z = D_800814A0_c;
            *(u8 **)(c + 0x2C) = t10;
            *(u32 *)&D_800814A0_c = z | 0x8000;
            func_80048A44(c, t10[((D_80083228 + ((s16 *)d)[0x15] + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        break;
    case 11:
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            *(u16 *)(c + 0x14) &= 0xFDFF;
            *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
        }
        break;
    }
}
