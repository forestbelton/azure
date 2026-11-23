/* ===========================================================================
   BEST MEASURED CANDIDATE for func_800ADE74,
   csrc/ovl_dungeon_800A4B88.c (1817 words).

   2026-11-22, EIGHTH session.  1817/1817 WORDS, 2 aligned rows.
                                CLUSTER I IS CLOSED -- ALL FOURTEEN ROWS --
                                by two free-reference chains, see change (O).
                                What is left is rows 15 and 20 only.
   was 2026-11-22, seventh:     1817/1817 words, 16 aligned rows
                                (residue (1) CLOSED, cluster II CLOSED.
                                 WHAT IS LEFT IS cluster I's 14 rows plus
                                 TWO NEW rows, 15 and 20, which are the price
                                 of change (N) -- see below.)
   was 2026-11-22, seventh (b): 1817/1817 words, 17 aligned rows
   was 2026-11-22, seventh (a): 1817/1817 words, 20 aligned rows
   was 2026-11-22, sixth:       1817/1817 words, 23 aligned rows
                                (27 before the prototype block below; see
                                 "THE CARVE REGRESSION")
   was 2026-11-22, fifth:       1817/1817 words, 30 aligned rows
   was 2026-11-22, fourth:      1817/1817 words, 30 aligned rows
   was 2026-11-22, third:       1817/1817 words, 35 aligned rows
   was 2026-11-21, later:       1821/1817 words, 170 aligned rows
   was 2026-11-21, morning:     1817/1817 words, 255 rows -- but that word
                                count was TWO CANCELLING ERRORS, see below
   was 2026-11-16 (j36):        1826/1817 words, 256 aligned rows

   Measured with, and all of them agree at every step below:
       python3 tools/decomp/parksplice.py func_800ADE74 <copy>    (real unit)
       python3 tools/decomp/ovlverify.py  <copy> func_800ADE74
       python3 tools/decomp/onefunc.py func_800ADE74 --src <copy> --out one
       python3 tools/decomp/aligndiff.py one/ovl_dungeon.c func_800ADE74

   ON THIS BODY THE TWO COUNTS AGREE: ovlverify's positional count and
   aligndiff's aligned count are BOTH 2, because the two differing words are
   a straight swap in place and nothing is transposed.  (They disagreed on
   the sixth session's body -- 27 against 23 -- so keep using aligndiff:
   ovlverify compares word N against word N and a transposed pair misses
   twice.)

   THE WORD COUNT IS HONEST: every word is accounted for, and everything
   except words 15 and 20 is byte-identical to the target.

   ===========================================================================
   WHAT MOVED ON 2026-11-22 (EIGHTH SESSION), 16 -> 2 ROWS: CLUSTER I FELL.

   (O) TWO FREE-REFERENCE CHAINS GIVE THE ALLOCATOR THE ORDER THE SEVENTH
       SESSION PROVED IT NEEDED.                16 -> 2 rows, 0 words.

           +   r7 = r7 + r7 + r7 + r7 + r7 - r7 - r7 - r7 - r7;
               ang = *(u16 *)(c + 0x88);
           +   ang = ang + ang - ang;

       The requirement was `pri(r7) > pri(ang) > pri(i2A)' and it is now MET:
           r7  119   13 -> 15 refs / 85 insns   5294
           ang 129    9 -> 12 refs / 73 insns   4931
           i2A 111    9 refs / 61 insns         4426   (untouched)
           ptr 1331   5 refs / 44 insns         2272   (untouched)
       greg allocates r7 first, so r7 takes $s0, ang then takes $s1, and i2A
       and the pointer go to $s2 by themselves -- EXACTLY what the seventh
       session's pinning probe predicted ("an allocator that produced r7 in
       $s0 and ang in $s1 leaves this function at 2 rows").  All fourteen
       cluster-I rows match and NONE of the pinning's eleven rows of damage
       appears, because nothing is pinned.

       THE DEVICE IS docs/matching-levers.md's "A TIED PHANTOM COPY" (:11434)
       and "flow RUNS ONCE, BEFORE combine -- SO A REFERENCE combine DELETES
       IS STILL FREE" (:31950).  cse leaves the chain alone, flow counts every
       reference in it, combine folds the whole chain back to `(set rN rN)'
       and that no-op move is deleted before any code is emitted: 1817 words,
       and with the chains alone at the wrong sizes the body is BYTE-IDENTICAL
       to the park.  Each `+ V' term past the first is worth 2 references,
       which is how the sizes were chosen rather than swept.

       WHY `do { } while (0)' CANNOT DO THIS, which is the finding that cost
       the session its first hour.  Wrapping `r7 = func_8009FD40w(D_80082E80,
       b);' in k nested do-whiles raises loop_depth (flow.c:440, :2122) for
       every reference in the statement -- and that statement also mentions
       D_80082E80, so the POINTER gains the same k.  The pointer's live length
       (44, already doubled) is just over half r7's 83, so the pointer wins at
       every level and takes $s0 ahead of r7:
           k   ptr refs -> pri      r7 refs -> pri
           0    5         2272       6         1445
           1    6         2727       7         1686
           2    7         3181       8         2891
           3    8         5454       9         3253
           5   10         6818      11         3975
       Measured: k = 1 and k = 2 are byte-identical at 16 rows, k = 3 and 4
       are 151 rows, k = 5 is 156; and with i2A also lowered, `.greg' shows
       the pointer at order 27 holding $s0 with r7 at order 41.  SO THE
       REFERENCES HAVE TO LAND ON ONE VARIABLE, which is what the chain does
       and what a loop note cannot.

       THE SIZES, measured (chain of n `+' terms = n uses = +n refs, +1 insn
       of live length, 1817 words throughout):
           r7 chain alone   n=1  9/84  3214   16 rows (ang still $s1)
                            n=2 11/84  3928   21 rows (r7 $s1, ang $s2)
                            n=3 13/84  4642   16 rows, r7 $s0 -- i2A $s1
           + ang chain n=1      ang 12/73 4931, r7 n=4 15/85 5294   2 ROWS
           + ang chain n=2      ang 14/73 5753, r7 n=5 17/85 8000   2 rows
           + ang chain n=2      ang 14/73 5753, r7 n=6 19/85 8941   2 rows
           + ang chain n=3      ang 16/73 8767, r7 n=7 21/85 9882   2 rows
       The n=1/n=4 pair is kept as the smallest that satisfies the ordering.

       BOTH CHAINS ARE REQUIRED AT EXACTLY THESE SIZES, and the priority
       arithmetic predicts every one of these measurements before it is run
       (1817 words throughout -- the chains never cost a word):
           both, as kept        r7 15/85 5294 > ang 12/73 4931 > i2A 4426
                                $s0 / $s1 / $s2                      2 rows
           drop the ang chain   r7 5357 > i2A 4426 > ang 3802
                                r7 $s0, i2A $s1, ang $s2            16 rows
           drop the r7 chain    ang 5000 > i2A 4426 > r7 1428
                                ang $s0                             21 rows
           drop both            the 16-row park, BYTE-IDENTICAL     16 rows
           r7 chain one shorter r7 13/85 4588 < ang 4931
                                ang $s0, r7 $s1                     12 rows
           ang chain one longer ang 14/73 5753 > r7 5294
                                ang $s0, r7 $s1                     12 rows
       -- so the requirement really is the ORDERING and not the sizes, and
       the sizes are the smallest pair that produces it.

       THE OTHER HALF OF THE ARITHMETIC, ALSO MEASURED AND NOT TAKEN: lower
       i2A instead of raising ang.  i2A's live length only grows when its
       BIRTH MOVES ACROSS A BRANCH -- `i2A = 0;' moved inside its own block
       (above `ang = ') is byte-identical and does not change 61 at all,
       because the SN `shorten' pass re-derives the lengths from the SCHEDULED
       order.  Moved across a branch it works and costs words:
           i2A = 0 at the arm top         9/102 2647   1819w  83 rows
           i2A = 0 before the 365C guard  9/96  2812   1820w  83 rows
           i2A = 0 before the r7 call     9/90  3000   1820w  82 rows
       -- every one of those reaches r7 $s0 / ang $s1 with an r7 chain, and
       every one loses words, so raising ang is strictly better.

   ===========================================================================
   THE OTHER BASIN, WRITTEN OUT IN FULL BECAUSE THE NEXT SESSION SHOULD
   PERMUTE IT RATHER THAN THIS ONE IF THIS ONE COMES BACK EMPTY.

   The park body is the 2-row branch (the maximum in `e').  The 3-row branch
   has rows 15 and 20 CLOSED and only cluster II's three rows open, which is
   a completely different residue and a completely different basin.  It is
   four lines against this file:

       s32 dy;
   +   s16 mxp;
       ...
       ret = 2;
   +   mxp = (s16)e;
       ...
   -       func_800A19E4((s32)b, c, (s16)d, (s16)e, f);
   +       func_800A19E4((s32)b, c, (s16)d, mxp, f);

   -- leaving case 0x24's three `e' statements exactly as they are.  It
   measures 1817/1817 words, 3 aligned rows, and the three are
       R 1235  move a2,v1   ->  addu $s0, $v1, $zero
       R 1247  move a2,a0   ->  addu $s0, $a0, $zero
       R 1248  sll v0,a2,0x10 -> sll $v0, $s0, 16
   i.e. the maximum in $a2 where the target has $s0, and NOTHING ELSE.
   stmtsweep, widthsweep and partsweep were all run on THIS body too and all
   three are clean on it ("no safe move helps", "no width change helps", "no
   partition helps", each re-deriving `base 3 rows, 1817/1817 words' first).
   Its one question is the one the residue section proves is shut from the
   source side -- so if a permuter run on the 2-row body comes back empty,
   run one on this body instead: its object diff sees three words of a
   REGISTER choice rather than two words of a SCHEDULING choice, and the two
   are not the same search.

   ===========================================================================
   CLUSTER I, 2026-11-22 (SEVENTH SESSION): THE SIXTH SESSION'S PROOF WAS
   SOLVING A TWO-WAY PROBLEM AND IT IS A **FOUR-WAY** ONE.  THE REQUIREMENT IS
   NOW AN EXACT PRIORITY ORDERING, AND THREE OF ITS FOUR LINKS ARE REFUTED.

   The sixth session's proof compared `r7' (allocno 119) only against the
   D_80082E80 pointer (1338) and concluded that r7 must be allocated first.
   THAT IS NECESSARY AND NOT SUFFICIENT, and this session demonstrated it by
   ACHIEVING IT: merging `mx' into `r7' took 119 to `used 11 times across 98
   insns' -- priority 3367 against the pointer's 2272 -- and greg's order
   duly became `... 111 155 128 891 119 295 432 1208 81 570 1338 ...', with
   119 BEFORE 1338.  r7 still came out $s2 and cluster I did not move one row.

   WHY: a FOURTH allocno holds $s0 in our build and the sixth session never
   named it.  `i2A' is allocno 111, `Register 111 used 9 times across 61
   insns; crosses 2 calls', and it is allocated FIRST of the four and takes
   $s0; the pointer then shares $s0 with it because 111 and 1338 do NOT
   conflict (111's conflict list is `81 82 88 111 119 125 126 128 1375 1386
   1426' and has no 1338 in it -- the pointer dies at 800AF384 and i2A is born
   at 800AF38C).  The target's assignment is r7 $s0, `ang' $s1, pointer $s2,
   i2A $s2; ours is i2A $s0, pointer $s0, `ang' $s1, r7 $s2.

   THE FOUR PRIORITIES, off `.lreg' and `.greg' on this body, with
   global.c:588-610's `pri = floor_log2(n_refs) * n_refs / live_length
   * 10000':
       i2A      111   9 refs / 61 insns / 2 calls      4426
       ang      128   9 refs / 70 insns / 3 calls      3857
       pointer 1338   5 refs / 44 insns / 2 calls      2272  (44 is ALREADY
                                                              doubled -- see
                                                              the sixth
                                                              session's note)
       r7       119   6 refs / 83 insns / 3 calls      1446
   find_reg takes the lowest free register in a 0..31 scan, and all four cross
   a call so all four are confined to the callee-saved registers.  Enumerating
   the orders gives exactly ONE that produces the target's assignment:

       r7 -> 16, then ang -> 17 (conflicts r7), then i2A -> 18 (conflicts r7
       and ang), then pointer -> 18 (conflicts r7 and ang, does NOT conflict
       i2A, so it shares 18).

   Every other order fails, and the two near misses are worth recording
   because both have been measured:
       r7, i2A, ang, pointer  ->  r7 16, i2A 17, ang 18, pointer 17.  ang and
           the pointer are then BOTH wrong, which is precisely what the sixth
           session saw when it pinned r7: `register s32 r7 __asm__("$16")' is
           18 rows on this body (16 without it), four of r7's rows match and
           six others break, and the park's note that `the pointer and ang
           swap into $s1/$s2' is this order.
       i2A, ang, pointer, r7  ->  the body we have.

   SO THE REQUIREMENT IS   pri(r7) > pri(ang) > pri(i2A),   i.e. TWO
   inversions, not one.  Numerically, with ang left alone at 3857:
       r7   needs floor_log2(R)*R > 32.0 at live_length 83, so R >= 11 refs
            (3*11 = 33); 8 is not enough (3*8 = 24), which retires the sixth
            session's "r7 n_refs >= 8" route even on its own terms.
       i2A  needs live_length > 70, or n_refs <= 7 (floor_log2(7)*7/61 = 2295).
   AND A BLOCKING ALLOCNO CANNOT SUBSTITUTE FOR EITHER.  The sixth session's
   "create a quantity Q" route is closed by the ORDER rather than by the
   conflict sets: any Q that takes $s0 before i2A pushes i2A to 17 and `ang'
   to 18, and `ang' MUST be 17.  Two blockers taking 16 and 17 push `ang' to
   19.  No blocking scheme can produce the one good order, because in that
   order r7 is allocated FIRST and nothing may hold $s0 at all.

   AND THE ENUMERATION IS CONFIRMED BY PINNING, WHICH IS THE SHARPEST
   STATEMENT THIS RESIDUE HAS EVER HAD.  On the 16-row body:
       nothing pinned                                   16
       register s32 r7  __asm__("$16")                   18
       register s32 ang __asm__("$17")                   24
       r7 $16 AND ang $17                                13
       r7 $16, ang $17 AND i2A __asm__("$18")            13  (no change -- so
            once r7 is $s0 and ang is $s1, i2A and the pointer go to $s2 BY
            THEMSELVES, exactly as the good order predicts)
   With r7 $16 and ang $17 ALL FOURTEEN cluster-I rows match -- 1330, 1332,
   1334, 1337, 1338, 1347, 1348, 1350, 1360, 1410, 1411, 1413, 1416, 1418 --
   and the thirteen that remain are rows 15 and 20 plus ELEVEN NEW ONES at
   1400..1418 which are the PINNING's own damage: gcc 2.7.2 uses an
   `__asm__'-declared local AS the destination of the sign extensions that
   read it (`sll s1,s1,0x10 / sra s1,s1,0x10' for `aa = (s16)ang', and
   `sll s0,s0,0x10 / sra s0,s0,0x10' for the final `(s16)r7') instead of a
   scratch pseudo.  SO THE WHOLE OF THIS FUNCTION'S REMAINING RESIDUE IS TWO
   FACTS: `r7' in $s0 with `ang' in $s1, and the parameter load's delay slot.
   An allocator that produced the first would leave the function at 2 rows.
   (The pinning is NOT landable -- it is an asm device, and gcc's mishandling
   of it above is its own reason not to trust it as anything but a probe.)

   MEASURED THIS SESSION AND REJECTED:
       mx merged into r7 (11 refs / 98 insns, pri 3367)      cluster I
                                                             unchanged, 33
       `aa' deleted and both its uses written `(s16)ang' -- two more
            loop-depth references on `ang', intended to lift 3857 past 4426
                                                   1818/97, LOSES A WORD
       `aa' kept and only ONE of its uses written `(s16)ang'   1818/97
       `register s32 r7 __asm__("$16")' on the 16-row body     18
   The `aa' route is the only honest way found to add references to `ang', and
   it deletes a word the target has (the target's `sra $v1,$v0,16' at
   800AF464 IS `aa = (s16)ang' and it must be a variable), so `ang' is as
   fixed as `r7'.  THE CLUSTER REMAINS CLOSED, now with a complete
   enumeration rather than a two-way comparison.

   ===========================================================================
   WHAT MOVED ON 2026-11-22 (SEVENTH SESSION), 17 -> 16 ROWS: CLUSTER II IS
   CLOSED.  `mx' IS THE PARAMETER `e'.

   (N) case 0x24's Chebyshev maximum lives in the PARAMETER `e', not in a
       variable of its own.                 17 -> 16 rows, 0 words, and all
                                            THREE of cluster II's rows go.

           -   s16 mx;                 (declaration deleted)
           -   mx = dx;                     +   e = dx;
           -   mx = sp1A;                   +   e = sp1A;
           -   if (mx == 1)                 +   if ((s16)e == 1)

       Rows 1235, 1247 and 1248 -- `move a2,v1' / `move a2,a0' /
       `sll v0,a2,0x10' against the target's `addu $s0,$v1,$zero' /
       `addu $s0,$a0,$zero' / `sll $v0,$s0,16' -- ALL MATCH.  `e' is dead from
       the `func_800A19E4' call onwards, so case 0x24 may use it as a scratch;
       it is a parameter, so it is definitely assigned, and nothing in this is
       undefined behaviour.

       THE PROOF THAT CAME FIRST, AND IT IS WHY THE PARTNER HAD TO BE `e'.
       `mx' reaching $s0 needs EVERY hard register from 2 to 15 excluded at
       find_reg's 0..31 scan (global.c:955-990; MIPS defines no
       REG_ALLOC_ORDER).  Three routes exist and only one is reachable:
         * hard_reg_conflicts -- impossible for $t0..$t9.  The function's
           `Hard regs used' line is `2 3 4 5 6 7 16..23 29 30 31', so nothing
           is ever live in $8..$15 and no conflict with them can be created.
         * regs_someone_prefers -- EMPTY for `mx'.  prune_preferences
           (global.c:869-884) unions the preferences of LOWER-priority
           CONFLICTING allocnos only, and `mx' (allocno 131) conflicts with
           81, 82 and 130 alone, all three of which are HIGHER priority.
         * allocno_calls_crossed != 0, which switches `used1' from
           `fixed_reg_set' to `call_used_reg_set' (global.c:925-929) and
           excludes 2..15 in one step.  THIS IS THE ONLY ROUTE, so `mx' MUST
           CROSS A CALL.  The fifth session's corrected reading of
           global.c:924 was right about the code -- crossing no call is the
           LEAST restrictive case -- and that is exactly why a short-lived
           non-call-crossing quantity can never reach $s0.
       AND THE PARTNER MUST ALSO BE **HImode**.  Merging `mx' into an SImode
       local that crosses a call does put the value in $s0 and then loses more
       than it wins, because `mx = dx' becomes a plain SImode copy and gcc
       folds the abs straight into it:
           ours    bgez $v0 / move $s0,$v0 / negu $s0,$s0 / sh $s0,0x18($sp)
           target  bgez $v1 / nop / negu $v1,$v1 / addu $s0,$v1,$zero
                                                 / sh $v1,0x18($sp)
       With an s16/u16 partner `mx = dx' is a TRUNCATION, not a copy, so the
       two pseudos cannot be folded together and the separate `addu' survives.
       Measured on the 17-row body, with the 16-bit compare restored as
       `if ((s16)X == 1)' so the candidate does not simply lose the `sll/sra':
           e   (u16 parameter, crosses 2 calls)      16   <-- taken
           r6  31   i2A 31   r9  32   r7  33   r8  33   i   33   j   33
           nn  33   r4  35   r1  36   ang 36   r5  37   t1  40   t3  48
           d   (u16 parameter, arrives in $a3)  1816/29
           t4  1818/90   n 1818/110   r2 1813/47   r3 1816/50
       WITHOUT the cast -- which is how the fifth and sixth sessions swept it
       -- every one of these loses the two words of the 16-bit compare, and
       that is why that whole table read as a refutation: `mx_as_r7' is
       1815/98 uncast and 1817/33 cast.  RE-PRICE A MERGE SWEEP WITH THE
       CONVERSION THE NARROWER VARIABLE WAS DOING, or the words hide the rows.
       HOW `e' WAS FOUND, from the dumps rather than by sweeping: `.lreg'
       lists exactly three HImode allocnos that cross a call --
           Register 83  used 2 times across 24 insns; crosses 2 calls; 2 bytes
           Register 85  used 2 times across 23 insns; crosses 2 calls; 2 bytes
           Register 88  used 7 times across 1189 insns; crosses 110 calls
       -- 85 is `e', 83 is `d' and 88 is `ret'.  `ret' is $fp and `d' arrives
       in $a3, so `e' is the only one of the three already sitting in $s0, and
       the only HImode call-crossing allocno in the function that is dead
       inside the switch.  THE RULE WORTH KEEPING: when a residue needs a
       specific callee-saved register for a short-lived value, read `.lreg'
       for the allocnos that ALREADY have the mode and the calls-crossed count
       you need and merge into one of those.  Do not sweep all the locals:
       the answer here was a PARAMETER, which no merge sweep in six sessions
       had included.

       THE PRICE, AND IT IS THE WHOLE OF WHAT IS NEW IN THE RESIDUE: rows 15
       and 20.  `e' is the fifth parameter and arrives on the stack, so the
       function loads it once at entry; extending its live range into case
       0x24 lengthens its dependence chain and sched1 moves that load from the
       SECOND load-delay slot at the top of the function into the FIRST:
           ours    800ADEB0 lhu s0,96(sp)          target  nop
           ours    800ADEC4 nop                    target  lhu $s0, 0x60($sp)
       Both are load-delay slots and the word count does not move.  Nothing
       else in the first 1228 words differs now.

   ===========================================================================
   WHAT MOVED ON 2026-11-22 (SEVENTH SESSION), 20 -> 17 ROWS: RESIDUE (1) IS
   CLOSED, AND THE SECOND SET IS **PROVED NECESSARY** RATHER THAN SWEPT.

   (M) THE CALL RESULT GOES INTO A FRESH SCRATCH `u2', sp20 AND rN ARE BOTH
       COPIED FROM IT, AND `u2' IS GIVEN A SECOND SET AFTERWARDS.
                                  20 -> 17 rows, 0 words.  ONE ROW PER SITE,
                                  fully additive, at all three sites.

           -   sp20 = func_800A384C(c, q2, (u8 *)&sp1E, 0);
           -   *(s16 *)(c + 0x2A) = sp1E;
           -   r2 = (u16)sp20;
           +   u2 = func_800A384C(c, q2, (u8 *)&sp1E, 0);
           +   sp20 = u2;
           +   *(s16 *)(c + 0x2A) = sp1E;
           +   r2 = u2;
           +   u2++;
           +   u2--;

       plus one new declaration `s32 u2;'.  The same shape at case 1 (r3) and
       at case 2 (r4, which has no `c + 0x2A' store between).  Measured:
           site 0 alone 19   site 1 alone 19   site 2 alone 19
           0+1 18   0+2 18   1+2 18   all three 17
       -- so each site is worth exactly one row and they are independent, the
       same additivity change (I) had.

       WHAT IT FIXES.  Rows 277, 393 and 556, which were
           ours  andi $v1, $v0, 0xFFFF      target  addu $v1, $v0, $zero
       and which change (I) had left as the price of getting the sp20 store to
       read the raw return register.  Now BOTH facts hold at once: the store
       reads $v0 and the second pseudo is a plain SImode copy.

       THE MECHANISM, AND WHY THE SECOND SET IS NOT A GUESS.  The target is
           jal func_800A384C / addu $a3,$zero,$zero
           lhu  $v1, 0x1E($sp)
           sh   $v0, 0x20($sp)      <- sp20, from the RAW return register
           sh   $v1, 0x2A($s5)
           addu $v1, $v0, $zero     <- r2, a plain copy, and $v0 is DEAD after
           sll  $v0, $v1, 16 / bltz $v0 / nop / j / ori $v0,$v1,0x8000
       $v0 is dead after the copy, so the copy is NOT forced by liveness.  It
       is forced by WHO ALLOCATES WHAT: the call-value pseudo is block-local,
       so local_alloc ties its quantity to hard reg 2 through
       `qty_phys_copy_sugg' and it IS $v0; `r2' is referenced in three basic
       blocks, so flow.c:2561 makes it a global allocno, `reg_qty' is -1 and
       local-alloc.c's `combine_regs' refuses to tie the copy; global_alloc
       then has to give it something other than $v0, because the sign-extend
       temp that local_alloc already placed there conflicts.  So
       `addu $v1,$v0,$zero' is what gcc emits for `r2 = <a block-local pseudo
       holding the call value>' -- provided the copy survives cse.

       AND IT DOES NOT SURVIVE cse UNLESS THE SOURCE PSEUDO IS SET AGAIN.
       `cse_insn' records `(set (reg r2) (reg u2))' by inserting r2 into u2's
       quantity, and `canon_reg' then rewrites every later reference to r2 as
       `qty_first_reg' of that quantity, which is `u2' -- the register set
       first.  r2 becomes dead and flow deletes the copy.  Within one basic
       block the ONLY thing that invalidates u2's quantity is a SET of u2, so
       a second set after `r2 = u2;' is not a device chosen by sweeping: it is
       the single necessary condition, and that is why every honest spelling
       below fails.  The pair `u2++; u2--;' supplies it and combine cancels
       the two `addiu' (1817 words, unchanged), which is the catalogue's
       cancelled-pair device used for the one thing it is actually for.
       EVERY BREAKER MEASURES THE SAME 19 at site 0: `u2++; u2--;', `u2++;'
       alone, `u2 = 0;', `u2 = 1;', `u2 = -1;', `u2 = r2;', `u2 = sp1E;', and
       `u2 = 0;' placed before the `c + 0x2A' store.  The pair is kept because
       it is the catalogued spelling and provably emits nothing.
       A `do { r2 = u2; } while (0);' wrapper instead of a second set is
       1815/54 -- the loop-depth device is NOT a substitute for the set.

       MEASURED AND REJECTED, all on the 20-row body, all at site 0, and all
       of them are the honest spellings (no second set):
           u2 = f(); sp20 = u2; store; r2 = u2;        1809/57  <- the park's
                                     own "fresh temp" note, re-measured
           u2 = f(); store; sp20 = u2; r2 = u2;        21
           u2 = f(); sp20 = u2; r2 = u2; store;        1809/57
           r1 = f(); sp20 = r1; store; r2 = r1;        1809/57
           r1 = f(); store; sp20 = r1; r2 = r1;        1815/56
           r1 = f(); sp20 = r1; r2 = r1; store;        1809/57
           r1 = f(); sp20 = r1; store; r2 = r1 | 0;    1809/57
           sp20 = f(); store; r2 = sp20;               1816/35
           sp20 = f(); store; r2 = (s32)sp20;          1816/35
           r2 = sp20 = f(); store;                     1816/35
           r2 = f(); sp20 = r2; store;                 1809/57
           r2 = f(); store; sp20 = r2;                 21
           r2 = 0; r2 = f(); sp20 = r2; store;         1809/57
           r2 = 0; sp20 = f(); store; r2 = sp20;       1816/35
           sp20 = f(); store; r2 = sp20; with r2 at s16    1809/57
                                         with r2 at u16    1809/57
           r2 = f(); sp20 = r2; store; with r2 at s16  1809/57
           the temp spelled as an EXISTING local instead of a fresh `u2',
              WITH the second set: i 1818/30, j 1818/30, r1 1818/30
              -- so the temp must be a variable of its own, and that is why
              refsweep could never find this: it only inserts pairs on names
              that already exist.
           `u2 = 0x800B;' as a REAL second set, then used in the `< 0' arm
                                                       1817/109
       inverting case 0's test (`if ((s16)r2 >= 0) { ... return 1; }' first)
       is 20, byte-identical, on the 20-row body.

   ===========================================================================
   WHAT MOVED ON 2026-11-22 (SEVENTH SESSION), 23 -> 20 ROWS, IN ONE CHANGE
   AND IT CLOSES CLUSTER II DOWN TO THREE PURE-REGISTER ROWS.

   (L) case 0x24's dy SUBTRACTION GETS ITS OWN VARIABLE `t4', AND THEN THE
       FOLDED (`>= 0 ? x : -x') dy abs IS THE ONE THAT WINS.
                                               23 -> 20 rows, 0 words.

           -   t3 = b[0x25] - sp1A;
           -   dy = (t3 < 0) ? -t3 : t3;
           +   t4 = b[0x25] - sp1A;
           +   dy = (t4 >= 0) ? t4 : -t4;
       plus one new declaration `s32 t4;'.

       BOTH HALVES ARE REQUIRED AND NEITHER PAYS ALONE.  The folded abs on
       `t3' is 48 rows (the sixth session's number) and the non-folded abs on
       a fresh `t4' is 23 (byte-identical to the park).  Together: 20.

       WHY THE 48 WAS NEVER A CLUSTER-II RESIDUE AT ALL.  The sixth session
       read the 48 as "the right schedule with every register wrong" and spent
       the session trying to fix `mx' and `dx'.  Reading the 48-row aligndiff
       ROW BY ROW instead shows that only 16 of those 48 rows are in case 0x24.
       FIFTEEN of them are at rows 785..789, 907..911 and 1442..1446 -- the
       OTHER THREE `t3' sites, in cases 0x2C, 0x29 and 0x1B, which MATCH on the
       park body and break when case 0x24's use of `t3' changes:
           ours   sra v1,v0,0x10 / bgez v1 / slti v0,v1,33 / negu v1,v1 / slti
           target sra $v0,$v0,16 / bgez $v0 / nop         / negu $v0,$v0 / slti
       Change (H) had made `t3', `t5' and `t7' ONE variable across four switch
       arms, so `t3' is a single global allocno with a single hard register;
       the folded form in case 0x24 pushes that register from $v0 to $v1 and
       all three other sites go with it.  Giving case 0x24 its own `t4' makes
       case 0x24's temp a BLOCK-LOCAL quantity again, leaves `t3' in $v0 at
       the three arms that already matched, and lets the folded schedule stand
       on its own.
       RULE WORTH KEEPING: when a change inside one switch arm regresses a
       body by much more than that arm can hold, COUNT THE DIFFERING ROWS BY
       ADDRESS before diagnosing the arm.  A shared scratch local is one
       allocno, so a spelling change in one arm is a register change in every
       other arm that shares the name.  (H)'s unification is still right for
       the three `(s16)func_800A0134' sites -- it is worth 2 rows there -- but
       case 0x24 was never part of that family.

       WHAT THE FOLDED FORM BUYS, and it is the sixth session's reading,
       confirmed: the folded `x >= 0 ? x : -x' is a single `abs' RTL insn, so
       there is NO basic-block split at the dy abs, block B runs through the
       Chebyshev comparison, and sched1 puts the sp18 widening in the `lbu's
       load-delay slot.  On the 20-row body rows 1229..1248 are
       BYTE-IDENTICAL to the target EXCEPT the three rows that read or write
       `mx' (1235, 1247, 1248), and `dx' is in $v1 and `t4' in $v0 exactly as
       the target has them -- no pinning, no device.

       THE WHOLE SWEEP, on the folded body, of WHICH VARIABLE holds the dy
       subtraction (each spelled `V = b[0x25] - sp1A; dy = (V >= 0) ? V : -V;'):
           t4 (FRESH s32)                20   <-- taken
           aa                            25
           r9, t1, r1, r4, r5, r6, r7,
              r8, i, i2A, j, dy          33
           nn                            37
           n, ang                        40
           r2                            1813/47
           r3                            1816/50
           t3 (the park's own name)      48
           ret                           1813/99
       and the non-folded abs on `t4' is 23, byte-identical to the park.
       So a FRESH variable is not merely as good as borrowing one here -- it is
       five rows better than the best borrowed one, which is the opposite of
       what cluster II's own history (changes (G) and (H)) would predict.

   ===========================================================================
   WHAT MOVED ON 2026-11-22 (SIXTH SESSION), 30 -> 23 ROWS, IN THREE CHANGES
   Both came from reading the two streams side by side rather than from a
   sweep.  All five sweeps were clean on the 30-row body and three of them are
   clean again on this one.

   (I) THE sp20 STORE MUST READ THE RAW CALL RESULT, AND THE WAY TO MAKE IT
       DO THAT IS TO ASSIGN sp20 FROM THE CALL AND rN FROM sp20 THROUGH A
       REDUNDANT ZERO-EXTENSION.              30 -> 26 rows, 0 words.

           -   rN   = func(...);
           -   sp20 = rN;
           +   sp20 = func(...);
           +   rN   = (u16)sp20;

       applied at THREE sites and only three: case 0 (r2), case 1 (r3) and
       case 2 (r4).  Each is worth exactly one row net, and all three are
       independent -- measured singly and in every combination:
           case 2 alone                 28      case 0 alone            29
           case 1 alone                 29      case 2 + case 0         27
           case 2 + case 1              27      all three               26
       THE MECHANISM, read off the two streams and not guessed.  The target's
       case-2 site is
           jal  func_800A40AC / andi $a1,$v0,0xFFFF
           sh   $v0, 0x20($sp)        <- the store reads the RETURN register
           addu $v1, $v0, $zero       <- and the live copy is made AFTER it
           sll  $v0, $v1, 16 / bltz $v0, .L800AE76C
       and the old body's was
           move $3,$2 / sll $2,$3,16 / bltz $2,$L58 / sh $3,32($sp)
       -- the store reads the COPY and therefore cannot be lifted above it, so
       cc1psx's delay-slot pass puts it in the BRANCH's slot instead.  It can
       do that precisely BECAUSE the store reads $v1: the target's store reads
       $v0, which the following `sll' overwrites, so the same move is illegal
       there and the slot stays empty.  THE RESIDUE WAS A DELAY-SLOT
       CONSEQUENCE OF WHICH PSEUDO THE STORE READS, not a sched1 priority.
       `sp20 = func(...)' makes the store's source the call-result pseudo, and
       the `(u16)' makes `rN' a SECOND pseudo -- a redundant recomputation is a
       copy that survives where a plain copy does not, because cse cannot hash
       the widened form into the original's class.  Both halves are required.
       THE COST: the `(u16)' emits `andi $v1,$v0,0xFFFF' where the target has
       `addu $v1,$v0,$zero', which is the one row each site keeps.  That
       `andi' is the whole of the remaining residue (1) below.
       MEASURED AND REJECTED for this slot, all on the 30- or 25-row body:
         rN = sp20 = func(...)              31 (store right, copy replaced by
                                               sll/sra -- no second pseudo)
         sp20 = func(...); rN = sp20;       31 (same)
         sp20 = func(...); rN = (s16)sp20;  31
         drop rN, use sp20 everywhere       30 (byte-identical: the store is
                                               NOT emitted from the valreg)
         a fresh temp: u = f(); sp20 = u; rN = u;   byte-identical in BOTH
                                               orders -- gcc coalesces u and rN
         rN = (u16)sp20 with rN at u16 or s16      30 (plain HImode copy, so
                                               the pseudos coalesce again)
         rN = (u16)sp20 with rN at u32      28 (= the s32 spelling)
         rN = sp20 & 0xFFFF, (s32)(u16)sp20        byte-identical to (u16)
         the second variable spelled as an EXISTING 16-bit local instead of
            rN -- aa, mx, h1, h2, h3 all 27, and k, k2, sp1C, sp1E 1818/45
         r1 29, r9 30 as the second variable
         test on sp20 with the `|' on rN    29
         `|' on sp20 with the test on rN    30
         the SAME change at the q7 tail     1816/68 and 1816/66 -- the q7
            tail already matches and MUST BE LEFT ALONE
         the SAME change at case 4 (r5)     n/a: case 4 has no sp20 residue
       AND THE MEM_IN_STRUCT_P ROUTE IS REFUTED.  The obvious reading of the
       target's `lhu / sh sp20 / sh 0x2A' order is that the 0x2A store's
       dependence on the stack store was dropped, i.e. the catalogue's
       "MEM_IN_STRUCT_P NEEDS A PLUS_EXPR ON TOP" with the flag on the STORE.
       Writing `*(s16 *)(c + 0x2A) = sp1E' as `((s16 *)c)[0x15] = sp1E' is
       1809/66 at case 0, 1816/48 at case 1, 1808/74 at both, 1817/30
       (byte-identical) at the q7 tail and at case 4.  Note that case 0's
       1809/66 and case 1's 1816/48 are EXACTLY the scores the fifth session
       recorded for DROPPING r2 and r3 -- two different changes, the same
       output -- and both delete words the target has.  Array form is wrong
       here.

   (J) case 2's `0x20000 / func_800A404C / 0x9A == 0x11' BLOCK BELONGS INSIDE
       THE `func_80042900((s8 *)c, 6) == 0' BLOCK.      26 -> 25 rows, 0 words.

           -       } while (i < 2);
           -       }
           -   }
           -   if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 && ...)
           +       } while (i < 2);
           +       }
           +       if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 && ...)
           ... and the matching brace moved down past the block.

       THIS IS A PROGRAM CHANGE, NOT A RESPELLING, AND THE LISTING IS WHAT
       PROVES IT.  The row was the destination of ONE `bnez':
           ours    row 405  bnez $v0, +0x834   (= word 525)
           target  row 405  bnez $v0, .L800AE76C (= word 574)
       Word 525 is case 2's `lw $v0,0x1C($s5)' -- the 0x20000 test -- and word
       574 is its `jal func_800A2C34'.  So when func_80042900 returns nonzero
       the TARGET skips the 0x20000 block entirely, while the old body ran it.
       The other exit confirms the nesting from the inside: `sp1C < 0' branches
       to .L800AE6A8 = word 525, i.e. INTO the 0x20000 test, which is only
       reachable as a fall-through if that test is inside the same block.
       RULE WORTH KEEPING: a differing BRANCH DESTINATION is not a register or
       schedule residue -- it is a claim about which block the code belongs to,
       and it is the cheapest row on the board.  Read every such row before
       spending a compile on anything else.

   ===========================================================================
   WHEN THIS LANDS, func_800ADE74 MUST **NOT** BE FORWARD-DECLARED IN THE UNIT.
   (2026-11-22, sixth session.  A LANDED FUNCTION IS BROKEN BY THE SPLICE AND
   HAS BEEN IN EVERY SESSION'S MEASUREMENT OF THIS PARK.)

   parksplice.py synthesises a forward declaration of the function it splices
   and puts it at the end of the unit's top declaration block.  For this park
   that line is
       (the definition's own signature, with u16 on the 4th and 5th parameters)
   and csrc/ovl_dungeon_800A4B88.c does NOT declare func_800ADE74 at all -- its
   only in-unit caller, the LANDED func_800ADDA0 at csrc line 6039, calls it
   through an implicit `int f()'.  With the synthesised declaration present:

       func_800ADDA0   target 53 words, candidate 52 words, 50 differing
       func_800ADE74   1817/1817 words, 25 aligned rows

   and with that one line deleted:

       func_800ADDA0   target 53 words, candidate 53 words, 0 differing
       func_800ADE74   1817/1817 words, 25 aligned rows   (IDENTICAL)

   -- so it costs func_800ADE74 nothing and costs its caller everything.  The
   mechanism is the default argument promotions.  func_800ADDA0's 4th and 5th
   arguments are `s16 d, s16 e'; through an implicit `int f()' they are
   promoted with SIGN extension, which is what the target does
       sll $a3,$a3,16 / lw $v1,0x3C($sp) / lh $v0,0x38($sp) / sra $a3,$a3,16
   and through the `u16' prototype they are converted with ZERO extension
       lw $v1,0x3C($sp) / lhu $v0,0x38($sp) / andi $a3,$a3,0xFFFF
   which is the 50 differing rows.  The definition keeps `u16 d, u16 e' -- that
   is what gives func_800ADE74 its own 1817 words -- so the landed file wants a
   definition with no declaration, exactly like the unit's note on
   func_800A0E6C ("CALLED WITHOUT A PROTOTYPE, as the unit's later call site
   does") one section down.  gcc 2.7.2 only warns about the mismatch.

   Verified with the whole unit: all 160 defined functions in the unit copy
   are `0 differing' with that line deleted, and func_800ADDA0 is the only one
   that moves either way.

   ===========================================================================
   THE CARVE REGRESSION: 30 -> 34 ROWS, AND IT IS ONE MISSING PROTOTYPE
   (2026-11-22, fifth session.  DIAGNOSED, FIXED, AND IT IS A carve_dungeon.py
   BUG THAT AFFECTS FOUR OTHER PARKS.)

   The third rodata carve (commit fb2d885) moved this function out of
   csrc/ovl_dungeon.c into csrc/ovl_dungeon_800A4B88.c.  The park body did not
   change by one character, and it went from 30 aligned rows to 34.

   THE CAUSE, measured with one variable:
       the park body in a PRE-CARVE csrc/ovl_dungeon.c   1817 words, 30 rows
       the SAME body in csrc/ovl_dungeon_800A4B88.c      1817 words, 34 rows
   Fourteen of this function's callees are defined in an EARLIER unit now and
   are declared NOWHERE in csrc/ovl_dungeon_800A4B88.c nor in
   csrc/ovl_dungeon_decls.h, so they became implicit `int f()'.  Each of the
   fourteen was added to the 34-row body ALONE:

       s32 func_8009CD58(u8 *a, s32 b, s16 c);                   34
       s16 func_800A0134(u8 *a, u8 *b);                          34
       u8 *func_800A02AC(u8 *p, u16 x, u16 y);                   34
       s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);    34
       s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);            34
       s32 func_800A2CAC(u8 *p);                                 34
       s32 func_800A3518(u8 *p);                                 34
       s16 func_800A3544(u8 *p, u8 k);                           34
       s32 func_800A365C(u8 *a, u8 *b);                          34
       s32 func_800A36B4(u8 *a, u8 *b);                          34
       s32 func_800A384C(u8 *a, u8 *b, u8 *c, s32 s);            34
       u8 *func_800A3D18(u8 *a, u8 *b, s16 c);                   34
       s32 func_800A404C(void);                                  34
       s16 func_800A45D8(u16 x, u16 y, s32 c);                   30  <-- THIS
       all fourteen together                                     30
       the other thirteen, without func_800A45D8                 34

   So it is EXACTLY ONE prototype, `s16 func_800A45D8(u16 x, u16 y, s32 c)',
   and it is necessary and sufficient.  Its single call site is case 0x2A's

       if ((s16)func_800A45D8(sp18 << 6, sp1A << 6, (s16)(ang + 0x20)) != 0)

   -- inside residue cluster (I), which is why the four rows land there.
   With the prototypes restored the generated text is IDENTICAL to the
   pre-carve build: `aligndiff' output diffs clean against the pre-carve
   standalone except for the disassembler's absolute branch addresses
   (`bnez v0,834' against `bnez v0,9b1c', same `+0x834' offset), which is the
   function sitting at a different file offset.  **Therefore every sweep
   result recorded below, measured pre-carve, transfers unchanged.**

   THE GENERATOR BUG, and it is not defs_in().  carve_dungeon.py:58-62 is

       named = set(re.findall(r'\b(func_[0-9A-Fa-f]{8})\b', ch))
       for f in sorted(named & set(earlier)):

   -- the callee set is read out of THE UNIT'S OWN C TEXT.  A PARKED function
   contributes nothing but its `INCLUDE_ASM(...)' line, so its callees are
   invisible and no prototype is synthesised for them.  Proof it is this and
   not a parse failure: `s16 func_800A45D8(u16 x, u16 y, s32 c);' IS present
   in csrc/ovl_dungeon_800D50FC.c:21, because that unit has a LANDED caller at
   line 623 whose text names it.  The generator parsed the definition line
   fine; it just never asked for it here.

   THE FIX (safe, and narrow): union in the callee names of each park the unit
   stubs, read from the park's LIVE BODY (parkresidue.body_of), not its header
   -- a header mentions functions in prose.  It cannot change a landed
   function: if a landed function in the unit called the same callee, the name
   would already be in `ch' and the prototype would already exist.

   FOUR OTHER PARKS ARE MISSING PROTOTYPES FOR THE SAME REASON (computed by
   replaying the generator's own `earlier' map over the nine units in order;
   each line is the generated spelling that is absent):
   (THE SPELLINGS BELOW DELIBERATELY HAVE NO TRAILING SEMICOLON.  They are
   prose about OTHER units, and parkresidue.decls_of harvests every
   declaration-SHAPED line out of a park header -- with semicolons it spliced
   all fourteen of them into THIS unit, where func_800B1768, func_800BC318,
   func_800C0180 and func_800C27F0 could reach a landed function.  Caught by
   reading `parksplice' reporting 43 declaration lines where 17 were meant.)
       ovl_dungeon_800B6AC4  park func_800B6AC4  6 missing
           void func_80093C70(u8 *a, u8 *b, u8 *c)
           void func_80093D8C(u8 *a, u8 *b, u8 *c)
           s16  func_8009904C(void *x)
           s32  func_80099090(void)
           u8  *func_80099254(u8 *s, u8 *d)
           void func_800B1768(s16 k, s16 x, s16 y, s16 d, s16 e, s16 f)
       ovl_dungeon_800C2824  park func_800C2824  4 missing
           s32  func_800A6620(u8 *p, s32 f)
           void func_800BC318(u8 *key)
           s32  func_800C0180(void)
           s32  func_800C27F0(void)
       ovl_dungeon_800CABA8  park func_800CEA44  3 missing
           s32  func_800A0B94(s16 a, u8 *s, s16 c)
           s32  func_800A152C(s16 a, s16 b)
           void func_800A1D4C(u8 *p, s16 f)
       ovl_dungeon_800D50FC  park func_800D92C0  1 missing
           s32  func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d)
   ALL FOUR WERE MEASURED, and none of them is affected -- the exposure is
   real but it only bit this park:
       func_800B6AC4  812/2  rows with and without its 6        -> unaffected
       func_800C2824  278/10 rows with and without its 4        -> unaffected
       func_800CEA44  242/3  rows with and without its 3        -> unaffected
       func_800D92C0  344/5  rows with and without its 1        -> unaffected
   (func_800C2824 needed care: its park already DECLARES func_800C27F0 and
   func_800C0180 as `void f()' where the definitions are `s32 f(void)', so
   adding the generated spellings is a `conflicting types' hard error.
   Replacing the park's two with the definitions' and adding the other two is
   the measurement above, and it is 10 rows either way.)

   AND THE PROTOTYPE SPELLING IS ITSELF A LEVER, swept here for the first time
   because before the carve it was not a free parameter -- the definition line
   supplied it.  Fourteen spellings of func_800A45D8, on the 30-row body:
       s16 f(u16 x, u16 y, s32 c)   30   <-- the definition's own, KEPT
       s32 f(u16 x, u16 y, s32 c)   30        (return type does not matter)
       u32 f(u16 x, u16 y, s32 c)   30
       s16 f(u16 x, u16 y, s16 c)   30        (3rd parameter does not matter)
       s16 f(u16 x, u16 y, u32 c)   30
       s8  f(u16 x, u16 y, s32 c)   31
       s16 f(u16 x, u16 y, u16 c)   43
       s16 f(s16 x, s16 y, s32 c)   34
       s16 f(s32 x, s32 y, s32 c)   34
       s16 f(s32 x, s32 y, s16 c)   34
       s16 f(u32 x, u32 y, s32 c)   34
       s16 f()                      34        (= the implicit `int f()')
       u16 f(u16 x, u16 y, s32 c)   1818/112  (truncates the result)
       u8  f(u16 x, u16 y, s32 c)   1818/112
       s16 f(u8 x, u8 y, s32 c)     1819/124
   SO THE WHOLE FOUR ROWS ARE THE **u16 ON THE FIRST TWO PARAMETERS**, not the
   `s16' return: every spelling with `u16 x, u16 y' and a 32-bit-or-wider
   return is 30, and every spelling that widens or signs them is 34, exactly
   like having no prototype at all.  The call passes `sp18 << 6' and
   `sp1A << 6' from `s16' stack locals, so a `u16' parameter is a narrowing
   conversion the target plainly performs and a wider one is not.  Nothing
   beats the definition's own spelling, which is what the generator emits.

   func_800AFBFC, which moved into THIS unit in the same commit, is NOT
   affected: it is still 238/238 words and 2 aligned rows after the carve.
   Its only two callees are func_800644B8 (another unit entirely) and
   func_800AFFB4 (defined LATER in this same unit), so both were implicit
   before the carve as well and nothing changed for it.

   ===========================================================================
   WHAT MOVED ON 2026-11-22 (FOURTH SESSION), 35 -> 30 ROWS, IN TWO CHANGES

   Both came from the same question: decomp-permuter, run on the 35-row body,
   found 32 rows by storing case 0x24's Chebyshev comparison into `r5' -- a
   DEAD function-scope local whose real use is twenty switch arms away.  That
   is an allocation artifact, not something an author wrote.  Chasing the
   honest spelling of it produced both changes below, and a precise statement
   of the lever.

   (G) case 0x24's func_800A6E8C result goes into the arm's OWN scratch, and
       that same scratch then holds the comparison.      35 -> 32 rows

           -   if ((s16)func_800A6E8C(b, 0x12, &sp18, &sp1A) == 0)
           -   {
           -       break;
           -   }
           +   r9 = func_800A6E8C(b, 0x12, &sp18, &sp1A);
           +   if ((s16)r9 == 0)
           +   {
           +       break;
           +   }
               ...
           -   if (sp18 < sp1A)
           +   r9 = sp18 < sp1A;
           +   if (r9)
               {
                   mx = sp1A;
               }

       -3 rows, 0 words.  `r9' is a NEW local in case 0x24 only, in the same
       idiom as r1..r8 ("a call result in a scratch, then test it"), so no
       other arm is touched.  The three rows are a pure $v0/$v1 swap on the
       two sign-extend temps of the comparison: the target has
       `sll $v1,$v1,16 / sll $v0,$a0,16 / slt $v1,$v1,$v0 / beqz $v1' and the
       35-row body had the two registers the other way round.

       THE LEVER, STATED EXACTLY -- and this is the part worth keeping:
       the comparison's value must land in a pseudo that flow makes a GLOBAL
       allocno, i.e. one REFERENCED IN MORE THAN ONE BASIC BLOCK
       (flow.c:2561).  A pseudo referenced only in the block that sets it is a
       local_alloc quantity and gets $v0; a global allocno goes through
       global_alloc and can come out $v1.  BOTH HALVES OF (G) ARE REQUIRED
       AND NEITHER PAYS ALONE -- the call-into-`r9' alone is 35 and
       `r9 = cond; if (r9)' alone (with the call left inline) is 35.

       WHAT IS **NOT** THE LEVER, all measured on the 35-row body:
         the FORM of the condition          -- every ternary and every
            `if' shape is 35, byte-identical: `mx = (sp18 < sp1A) ? sp1A : mx',
            `mx = (sp18 >= sp1A) ? sp18 : sp1A', `(sp18 > sp1A) ? ...',
            `(sp18 < sp1A) ? sp1A : sp18', `(sp18 <= sp1A) ? sp1A : sp18',
            `if (mx < sp1A) { mx = sp1A; }', `if (!(sp18 >= sp1A))',
            `if (sp18 >= sp1A) {} else {...}', `(s16)'/`(s32)' casts on both
            operands, and `if (sp18 < sp1A) { mx = dy; }'.
         a FRESH local holding only the condition -- 35 at s32, s16, u8, u16,
            s8 and u32, and 35 with `if (cc != 0)' as well.  A fresh local
            used once is block-local, which is exactly why it cannot work.
         the SET COUNT on the variable        -- a dead store `r5 = cond;' with
            the plain `if' is 35; `r5 = 0;' is 35; `r5 = sp18;' is 35;
            `r5++; r5--;' is 35; and `if ((r5 = sp18 < sp1A) != 0)' -- one
            statement instead of two -- is 35.  So it is not reg_n_sets, not
            reg_n_refs and not a cancelled pair.
         a fresh local holding the MAX instead of the condition -- 1816/98,
            1816/115, 1818/111, 1815/101.  All lose words.
       Which variable you borrow decides the register, and only some win:
       r4, r5, t1 and dx reach 32; r6, r7, t3, t5, t7, i, i2A, j reach 34;
       aa, k, k2, h1 are 35; nn 36, r1 37, r8 38, n and ang 39, h2/h3 41,
       ret 1816/47, r2 1816/49, r3 1816/52, and sp1C/sp1E/sp20 1818/98
       (address-taken, so they are real memory traffic).  `r9' is preferred
       over all of them because it is the arm's own variable and borrows
       nothing.

   (H) `t3', `t5' and `t7' are ONE scratch, and case 0x24's dy abs uses it.
                                                        32 -> 30 rows

           -   s32 t3;  s32 t5;  s32 t7;
           +   s32 t3;
           (t5 in case 0x29 and t7 in case 0x1B renamed to t3)

           -   dy = b[0x25] - sp1A;
           -   if (dy < 0)
           -   {
           -       dy = -dy;
           -   }
           +   t3 = b[0x25] - sp1A;
           +   dy = (t3 < 0) ? -t3 : t3;
               sp1A = dy;

       -2 rows, 0 words, and two fewer declarations.  t3 (case 0x2C), t5
       (case 0x29) and t7 (case 0x1B) were three names for one idiom in three
       DISJOINT switch arms -- `t = (s16)func_800A0134(c, X); if (t < 0)
       { t = -t; } if (t <op> 0x21 ...)' -- so unifying them is safe and is
       the honest reading; case 0x24's dy abs then joins the same family.

       WHY IT PAYS, read off the target's own asm at 800AF1C8..800AF1DC:

           lbu  $v0, 0x25($s7)
           sll  $v1, $v1, 16
           subu $v0, $v0, $a1        <- the subtraction, in its own register
           bgez $v0, .L800AF1E0
           addu $a0, $v0, $zero      <- dy = that value, in the delay slot
           negu $a0, $a0             <- and negated there

       The target has TWO pseudos: the subtraction's result is what gets
       TESTED, and `dy' is a separate register copied from it in the branch's
       delay slot.  The 32-row body negated in place, one pseudo, and the
       delay slot went to `mx = dx' instead.
       THE TERNARY DIRECTION IS THE WHOLE THING, and it is the mirror image
       of change (A) below:
           dy = (t3 < 0) ? -t3 : t3;     30   <-- taken, does NOT fold
           dy = (t3 >= 0) ? t3 : -t3;    53       folds to one `abs' insn
       BUT READ CLUSTER (II)(b) BEFORE BELIEVING THAT SECOND NUMBER.  The
       sixth session showed that the FOLDING form reproduces the target's
       SCHEDULE for this whole arm instruction for instruction, and scores
       badly only because it then gets every register in the block wrong.  The
       53 (now 48) is a register residue wearing a schedule residue's clothes.
       `x >= 0 ? x : -x' folds to a single `abs' RTL insn and `x < 0 ? -x : x'
       does not, so on dx (change (A)) you want the FOLDING form and on dy you
       want the NON-folding one.  THAT EXPLAINS THE dx/dy ASYMMETRY the
       earlier headers recorded as an unexplained measurement.
       Equally good at 30, same words: `t3 = sub; dy = t3; if (t3 < 0) dy =
       -dy;', `if (t3 < 0) dy = -t3; else dy = t3;', and
       `dy = t3 = sub; if (t3 < 0) dy = -dy;'.  The ternary is preferred as
       the shortest.
       MEASURED AND REJECTED for this slot, all on the 32-row body:
         a FRESH s32 temp in the same two-pseudo shape    32 (no change)
         fresh s16 36, fresh u16 1813/103
         the same shape through r9, t1, t3, t5, r2..r6, j   32 (no change)
         i/i2A 33, r8 35, nn 35, aa 36, ang 38, n 41
         dropping `dy' and doing the abs in place on t3    47
         `dy = (dy >= 0) ? dy : -dy;' (re-priced here)     38
         `dy = 0 - dy;' inside the `if'                    32
         the same two-pseudo treatment applied to dx       42
         dx's abs rewritten as a symmetric `if'            42
       So ONLY t3-the-shared-scratch pays, and only in the non-folding form.

   ===========================================================================
   WHAT MOVED ON 2026-11-21, 170 -> 35 ROWS, IN SIX CHANGES

   FOUR OF THE FIVE ARE THE SAME LEVER: a `return CONST' site that the
   original did NOT have.  This function's residue was dominated by
   jump2's cross-jump pairing, and the pairing is decided by HOW MANY sites
   share the epilogue label and in what order -- not by any spelling inside
   the arms that failed to merge.  Every one of the four is a pure
   restructuring with identical semantics, and each one is ALSO the simpler
   C.  The fifth is a conditional negate written as a ternary.

   (A) case 0x24's `dx' abs written as a ternary.          1821/170 -> 1823/160

           -   dx = b[0x24] - sp18;
           -   if (dx < 0)
           -   {
           -       dx = -dx;
           -   }
           +   dx = b[0x24] - sp18;
           +   dx = (dx >= 0) ? dx : -dx;

       -10 rows, +2 words -- and those two words were MISSING here, so this
       CLOSES case 0x24's word gap rather than opening one.
       `x >= 0 ? x : -x' folds to ONE `abs' RTL insn (`x < 0 ? -x : x' does
       not; the catalogue already has that half).  WHAT IS NEW is that the
       fold is a sched1 lever on the operands AROUND it: with the whole
       conditional negate collapsed into one insn, sched1 has different
       material in the block and the two `sll 16'/`sra 16' pairs that
       sign-extend the two address-taken `s16' stack locals come out beside
       their own `lhu's instead of being spread into the branch's delay
       slot.  Nine insns at 800AF190..800AF1B0 become byte-identical.
       THE ASYMMETRY IS THE FINDING -- the same rewrite on the OTHER operand
       of the same arm is twenty rows WORSE:
           dx tern_ge, dy as-is       1823/160   <-- taken
           dx tern_ge, dy tern_lt     1823/160   (identical)
           dx tern_ge, dy tern_ge     1823/163
           dx as-is,   dy tern_lt     1821/170   (byte-identical to the `if')
           dx tern_lt, dy as-is       1821/170   (byte-identical)
           dx as-is,   dy tern_ge     1823/180
           dx tern_lt, dy tern_ge     1823/180
           dx abs into a separate local  1823/169
           dy abs into a separate local  1821/175 and 1823/176
       So sweep each conditional negate on its own; they are not a family.

   (B) The trailing k2 == 0x1E block had TWO identical
       `c[0x46] = 0x8009; return 5;' bodies.  It has ONE.  1823/160 -> 1815/130

           -   if (c[0xAC] < 2)
           -   {
           -       if (g != NULL)
           -       {
           -           *(u16 *)(c + 0x46) = 0x8009;
           -           return 5;
           -       }
           -       g = (u8 *)((s16)func_800A70E4(sp18, sp1A, *(s16 *)(c + 0x88)) + 1);
           -   }
           -   if (g != NULL) { ... return 5; }
           +   if (c[0xAC] < 2 && g == NULL)
           +   {
           +       g = (u8 *)((s16)func_800A70E4(sp18, sp1A, *(s16 *)(c + 0x88)) + 1);
           +   }
           +   if (g != NULL) { ... return 5; }

       -30 rows, -8 words, AND IT CLOSES RESIDUE (1) -- the case 0x1D /
       case 0x26 cross-jump, +8 words and about 105 rows, which two previous
       sessions had written off as unreachable from any source form.
       SAME PROGRAM, PATH BY PATH, INCLUDING SIDE EFFECTS:
         c[0xAC] <  2, g != NULL -- both skip func_800A70E4 and return 5;
         c[0xAC] <  2, g == NULL -- both call it, then test g again;
         c[0xAC] >= 2            -- both skip the block and test g.
       AND IT REPRODUCES THE TARGET'S OWN TRAILING BLOCK: `&&' compiles its
       second test to `bnez $s0, <join>', thread_jumps threads that edge past
       the following `beqz $s0' into the return-5 path, and the result is the
       target's `800AF9DC bnez $s0, .L800AFA10'.  Not one differing row is
       left anywhere in 800AF974..800AFA18.

   (C) case 0x20's THREE `c[0x46] = 0x800B; return 2;' tails become ONE.
                                                      1815/130 -> 1814/123
           -   if (*(s8 *)(b + 0x26) >= 0)
           -   { func_800A0E6C(...); *(u16 *)(c+0x46) = 0x800B; return 2; }
           -   if ((s16)func_8009FD40(b, D_80082E80) != 1)
           -   { *(u16 *)(c+0x46) = 0x800B; return 2; }
           -   *(s16 *)(c + 0x2A) = func_800A0818(...) + 0x800;
           -   *(u16 *)(c + 0x46) = 0x800B;
           -   return 2;
           +   if (*(s8 *)(b + 0x26) >= 0)
           +   { func_800A0E6C(...); }
           +   else if ((s16)func_8009FD40(b, D_80082E80) == 1)
           +   { *(s16 *)(c + 0x2A) = func_800A0818(...) + 0x800; }
           +   *(u16 *)(c + 0x46) = 0x800B;
           +   return 2;
       -7 rows.  It closes the whole of case 0x20 (the old residues (8) and
       (9), `lb $a1,0x9C($s5)' one slot early and the `+0x800' result in the
       wrong register) AND the `+1' word case 0x20 was costing.  The word
       count goes 1815 -> 1814 because that +1 was masking a -1 elsewhere.
       The call order and call count are identical on every path.

   (D) case 0x18's TWO `c[0x46] = 0x800B; return 2;' tails become ONE.
                                                      1814/123 -> 1816/46
           -   if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
           -   { *(u16 *)(c + 0x46) = 0x800B; return 2; }
           -   *(s16 *)(c + 0x2A) = func_800A0818(...);
           -   *(u16 *)(c + 0x46) = 0x800B;
           -   return 2;
           +   if ((*(s32 *)(c + 0x1C) & 0x20000) != 0)
           +   { *(s16 *)(c + 0x2A) = func_800A0818(...); }
           +   *(u16 *)(c + 0x46) = 0x800B;
           +   return 2;
       -77 ROWS AND +2 WORDS, the single largest change ever measured on this
       park.  It closes the ENTIRE 0x800B triangle (the old residue (4)):
       case 0x18's tail stops being a second copy in the `$v1' schedule, so
       .L800AE010 is written out in full the way the target writes it
       (800AE010..800AE03C), and every other 0x800B site cross-jumps into the
       canonical .L800AF8F0/.L800AF8F4 block.

   (E) The `c[0x25] == 0' early exit shares the function's `return' with the
       tail, so cse can no longer fold `0 << 16 >> 16'.   1816/46 -> 1817/36

           -   if (c[0x25] == 0)
           -   {
           -       ret = 0;
           -       return ret << 16 >> 16;
           -   }
           -   ... whole body ...
           -   if (*(s8 *)(b + 0x26) < 0) { return ret << 16 >> 16; }
           -   return 2;
           +   if (c[0x25] == 0)
           +   {
           +       ret = 0;
           +   }
           +   else
           +   {
           +       ... whole body ...
           +       if (*(s8 *)(b + 0x26) >= 0) { return 2; }
           +   }
           +   return ret << 16 >> 16;

       -10 rows AND THE LAST MISSING WORD.  The previous header had this
       diagnosed exactly ("for the target not to fold it, that `return' must
       have had two predecessors at cse time") and had not tried it.  The
       target's `j .L800AFA9C / addu $fp,$zero,$zero' goes THROUGH the shared
       `sll $v0,$fp,16 / srl $v0,$v0,16'; with the early `return' spelled out
       the two sites are one cse region and `0 << 16 >> 16' folds to `0'.
       It ALSO supplies the empty delay slot at 800ADFE4 that residue (7) was
       about, and six displacement rows at the top of the function go with it.
       (A `goto out;' to a label before the shared return measures exactly the
       same, 1817/36.  The `else' is preferred: this body contains no gotos.)

   (K) SPLIT case 0x24's dx SUBTRACTION INTO TWO STATEMENTS, REUSING `dx' AS
       ITS OWN TEMPORARY.                      25 -> 23 rows, 0 words.

           -   dx = b[0x24] - sp18;
           +   dx = b[0x24];
           +   dx = dx - sp18;

       THIS IS decomp-permuter's, AND IT ONLY SURVIVED BECAUSE THE FIND WAS
       SPLIT.  It came out of the 40-minute run below as the body with the
       WORST score of the ten finds (output-1070-1, score 1070 against a best
       of 980) and it was the only one worth anything -- see "THE PERMUTER RUN"
       below for the full table.  The find carried exactly this one change and
       nothing else, which is why it transferred onto a body seven rows better
       than the one it was found on.
       WHAT IT FIXES: rows 1229 and 1231.  The target ties the subtraction's
       destination to the LOAD'S OWN register --
           lbu  $v1, 0x24($s7) / subu $v1, $v1, $a0
       -- and the single-statement form gave `lbu $v0 / subu $v1,$v0,$a0', two
       quantities where the target has one.  Writing the load and the
       subtraction as separate statements, with the SAME variable on the left
       of both, gives local-alloc's block_alloc the tie it takes
       (local-alloc.c:1349 breaks at the first tying win, and addsi3_internal
       prints `addu %0,%1,%2' so operand 1 is the one the dest ties to).
       AND THIS IS A SEARCH SPACE NO SWEEP VARIES.  stmtsweep MOVES statements,
       refsweep inserts cancelled pairs, devsweep wraps, widthsweep retypes and
       partsweep repartitions; none of them SPLITS a compound right-hand side.
       A 9-candidate hand sweep of the whole body for the same mutation
       (`LHS = A <op> B;' -> `LHS = A; LHS = LHS <op> B;') found this one and
       nothing else: `t3 = b[0x25] - sp1A' and both of case 0x2A's
       `spXX = spXX + (u16)D_8006CCxx[sp1E]' are byte-identical, and
       `dx = dx - sp18' re-split is byte-identical.  Worth adding to the sweep
       family -- the space is small and this instance was worth 2 rows.
       REJECTED NEARBY, on the 23-row body: `nn = (sp18 << 6) + 0x20' hoisted
       out of the func_800BCB04 call 26, `nn = r7 - 2' hoisted out of the
       guard 28, `sp1E = sp1E << 9' split at the arm's tail 1823/101,
       `ang = *(u16 *)(c + 0x88)' split through a temp byte-identical,
       `sp18 << 6' hoisted out of the func_800A45D8 call byte-identical,
       the func_800A0818 result hoisted into r9/t3/t1/r1/i or sp1E itself
       byte-identical (nn 27, n 29), `ang' moved after that call 1816/99,
       and `i2A = 0' moved after the two `spXX = b[0x2X]' byte-identical.

   ===========================================================================
   THE PERMUTER RUN, 2026-11-22 SIXTH SESSION -- AND GATE 3 IS WHAT PAID
   (base=1080 on the 30-row body, 40 minutes, 5 jobs, 43854 iterations,
    best=980, ten finds.  Every one measured against the park with
    measure_finds.py; `SPD' must be exported or the tool dies on a KeyError.)

       output-980-1    1818/41   <- the BEST-SCORING find, a word long
       output-995-1    1816/45
       output-1040-1   1818/40
       output-1040-2   1818/44
       output-1040-3   1818/94
       output-1040-4   1818/81
       output-1040-5   1818/101
       output-1060-1   1818/53
       output-1065-1   1816/48
       output-1070-1   1817/28   <- the WORST-SCORING find, and the only one
                                    with anything in it -- see change (K)

   SO GATE 1 HELD AGAIN, HARDER THAN BEFORE: the score ordering and the row
   ordering are very nearly REVERSED here, and `best=980' names the find that
   measures second-worst.  Read every find.
   GATE 2 CAUGHT TWO, and both are the same defect -- an ADDRESS-TAKEN local
   reused as a scratch while it is still live:
     output-1040-1  `sp1C = (s16) func_800A2C34(c); if (sp1C != 0)' inside
        case 2's sp1C >= 0 block.  sp1C is read AFTER that test, by
        `func_800A35A4h(c, sp1C)' and `((u16 *)c)[0x23] = (sp1C + 1) | 0x8000'.
     output-1065-1  case 0x2A's `n' replaced by `sp1E'.  sp1E is read after
        the loop, by `*(s16 *)(c + 0x2A) = sp1E << 9'.
   A third, output-1060-1, writes the func_800A70E4 result into `sp1A' in the
   trailing k2 == 0x1E block; sp1A happens to be dead there, so it is legal,
   but it measures 1818/53.
   GATE 3 IS THE ONE THAT PAID.  output-1070-1 was ONE change; output-980-1
   was two (a `nd = c + 0x60' base local for case 0's `*(u32 *)(c + 0x60)'
   store, and a temp for case 1's func_800A384C result).  Split and measured
   on the 23-row body: the base local alone is byte-identical, and with the dx
   split 23 (no interaction).  Reading a permuter find means diffing it against
   the PYCPARSER ROUND-TRIP of base.c, not against base.c -- the output is
   regenerated C and a raw diff is 985 lines of reformatting:
       import sys
       sys.path.insert(0, "vendor/decomp-permuter")
       from src import ast_util
       ast = ast_util.parse_c(open("nonmatchings/F/base.c").read(), from_import=True)
       fn, _ = ast_util.extract_fn(ast, "F")
       print(ast_util.to_c(fn))
   Against that, every find's real change is one or two lines.

   ===========================================================================
   THE LEVER BEHIND (B), (C) AND (D), AND WHY NO SPELLING REACHED IT

   `find_cross_jump' walks its two streams ASYMMETRICALLY
   (scratch/gcc-src/cdk-gcc-b18/jump.c:2596-2620):

       i1 = prev_nonnote_insn (i1);
       i2 = PREV_INSN (i2);
       while (i2 && (GET_CODE (i2) == NOTE || GET_CODE (i2) == CODE_LABEL))
         i2 = PREV_INSN (i2);
       ...
       if (GET_CODE (i1) == CODE_LABEL)
         { --minimum; break; }

   -- stream 1 (the jump being processed) GIVES UP AT A LABEL, stream 2 (the
   candidate partner) skips labels -- and the simplejump arm
   (jump.c:2178-2211) tries the code before its own target label FIRST and
   enters the `jump_chain' walk only while nothing has been found:

       find_cross_jump (insn, JUMP_LABEL (insn), 1, &newjpos, &newlpos);
       if (INSN_UID (JUMP_LABEL (insn)) < max_uid)
         for (target = jump_chain[INSN_UID (JUMP_LABEL (insn))];
              target != 0 && newjpos == 0;
              target = jump_chain[INSN_UID (target)])
           if (target != insn && JUMP_LABEL (target) == JUMP_LABEL (insn)
               && ! INSN_DELETED_P (target))
             find_cross_jump (insn, target, 2, &newjpos, &newlpos);

   `jump_chain' is built ONCE by PREPENDING (jump.c:249-269), so it lists the
   jumps to a label in REVERSE SOURCE ORDER and the first partner matching
   two insns wins; `do_cross_jump' then deletes INSN's OWN copy and keeps the
   partner's (jump.c:2800-2805); and `rtx_renumbered_equal_p' compares HARD
   registers, so two tails in different register schedules cannot pair at all.

   So the pairing is a function of HOW MANY `j <epilogue>' sites exist and
   WHERE THEY SIT.  Nothing inside either of two arms that failed to merge
   can reach it -- which is why the previous sessions' exhaustive spelling
   sweeps (all 19 `c[0x46] = 0x8009' sites singly, in pairs and in triples;
   all 10 `0x800B' sites the same; 12 tail-swap variants; a 39-candidate grid
   over case 0x18 x .L800AE010) all came back BYTE-IDENTICAL.  They were
   searching the wrong object.

   RULE FOR THE NEXT PARK: when the residue is "two arms did not cross-jump"
   and every spelling in both arms compiles byte-identically, stop
   respelling the arms.  Go and COUNT the other sites that share the epilogue
   label, and look for one you can REMOVE by writing the guard as a
   short-circuit or an `else if' instead of a duplicated early return.  The
   catalogue entry this corroborates is "HOW MANY IDENTICAL `return CONST;'
   BODIES A FUNCTION HAS DECIDES WHICH COPY SURVIVES CROSS-JUMPING".
   Corroboration from the other side: decomp-permuter, run on the 1821/170
   body for 45 minutes, converged on exactly this region -- its three best
   finds all wrap the trailing block's last `return 5' in an always-true
   `if (x || y) { ... } else { ... }' so as to DUPLICATE it.  Same object,
   opposite direction, and a device where the source change is ordinary C.

   ===========================================================================
   RESIDUE THAT IS LEFT (2026-11-22, EIGHTH SESSION): 2 ROWS, 0 WORDS, AND IT
   IS ONE FACT WITH A COMPLETE IMPOSSIBILITY PROOF ATTACHED.

     rows 15 and 20   X   15   15 800ADEB0  lhu s0,96(sp)   ->   nop
                      X   20   20 800ADEC4  nop             ->   lhu $s0,0x60($sp)
   Nothing else in 1817 words differs.  Of the unit's 170 defined functions,
   167 are `0 differing' with this body spliced in, func_800ADE74 is these 2,
   and the remaining two -- func_800A9A04 and func_800A9C10 -- have NO
   LISTING in asm/nonmatchings/ovl_dungeon_800A4B88/ at all, so ovlverify
   prints `NO TARGET' for them and they cannot be checked either way.  They
   are untouched by anything here.  func_800ADDA0 is 53/53, 0 differing,
   with NO forward declaration of func_800ADE74 (see below).

   WHAT THE TWO ROWS ARE, AND WHY 0 IS AVAILABLE IN PRINCIPLE.
   The residue is a single either/or between two facts that cannot both hold
   in any spelling found:

     (i)  the parameter load `lhu $s0,0x60($sp)' is emitted in the SECOND
          load-delay slot only when ITS DESTINATION PSEUDO HAS ONE SET
          (`birthing_insn_p', sched.c:2512, raises a set to `max_priority'
          only at `reg_n_sets[dest] == 1', and sched1 is a BACKWARD list
          scheduler so the bump moves the insn LATER).  Change (N) puts case
          0x24's maximum in `e', giving it three sets.
          THIS IS NOT A HYPOTHESIS ANY MORE -- `tools/decomp/sched.py <body>
          func_800ADE74 --sched1' prints basic block 0's ready list and the
          two bodies differ in exactly one number.  `insn 16' is
              (insn 16 ... (set (reg/v:HI 85) (mem:HI (plus:SI (reg:SI 0 $0) ...
          i.e. the load of `e' (pseudo 85).  On THIS body:
              ready list at T-2: 18 (7f000001) 12 (7f000001) 16 (1)
                                 25 (1) 31 (2) 38 (7f000001)
          and on the 3-row body with the maximum moved out of `e':
              ready list at T-2: 18 (7f000001) 12 (7f000001) 16 (7f000001)
                                 25 (1) 31 (2) 38 (7f000001)
          -- `insn 12' is `d's prologue copy `(set (reg/v:HI 83) (reg:HI 7
          a3))' and `insn 18' is `f's load, and both keep the bump in both
          bodies because they have one set each.  0x7f000001 IS
          `max_priority'.  So the whole of rows 15 and 20 is that one ready
          list entry reading 1 instead of 0x7f000001.
     (ii) case 0x24's maximum is in $s0 only when it lives in an allocno that
          CROSSES A CALL, and `e' is the only HImode call-crossing allocno
          that is dead inside the switch.

   THE PRIZE IS PROVED: `register s16 mx __asm__("$16")' on this body, with
   `e' left alone, measures 1817/5 and rows 1235 and 1247 -- cluster II's
   core -- MATCH, while rows 15 and 20 are GONE.  The five rows left are the
   pinning's own damage (gcc 2.7.2 uses an `__asm__'-declared local as the
   DESTINATION of the `sh' and of the sign extension that read it: rows 1236,
   1248..1251).  SO AN ORDINARY `s16 mx' IN $s0 LANDS THIS FUNCTION.

   WHY (ii) REALLY NEEDS A CALL CROSSING -- the seventh session's claim, now
   read off find_reg rather than inferred.  For `mx' (3 refs / 14 insns, no
   calls crossed) pass 0 computes
       used = used1 | hard_reg_conflicts | ~regs_used_so_far
              | regs_someone_prefers
   with `used1 = fixed_reg_set' (global.c:926).  Its conflicts cover $v0, $v1
   ($dx and the slt temp, as hard conflicts 2 and 3), $a0 (`dy') and $a1, so
   the 0..31 scan stops at $a2 -- which is where we hold it.  $s0 needs every
   register from 2 to 15 excluded, and the two candidate routes are both shut:
     * `~regs_used_so_far' CANNOT exclude $t0..$t9.  global.c:332-349 is
           for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
             if (regs_ever_live[i] || call_used_regs[i])
               SET_HARD_REG_BIT (regs_used_so_far, i);
       -- EVERY call-used register counts as already used, unconditionally,
       so the fact that this function never touches $t0..$t9 buys nothing.
       (The seventh session's note that pass 0 excludes them is wrong.)
     * `regs_someone_prefers' would need lower-priority CONFLICTING allocnos
       preferring all of $a2, $a3 and $t0..$t9.  Preferences come from copies
       to hard registers, i.e. outgoing arguments, and case 0x24 HAS NO CALL
       AFTER `mx = dx' -- the target's own 800AF1C0..800AF208 contains no
       `jal'.  mx conflicts with exactly `b', `c' and `dy'; none of the three
       has a preference line in `.greg'.
   So `allocno_calls_crossed[mx] != 0' (global.c:928, `used1 =
   call_used_reg_set') is the ONLY route, and it excludes 2..15 in one step.

   AND NOTHING HONEST CROSSES A CALL THERE.  A live range crosses a call only
   if a value is computed before it and READ after it.  Measured and rejected
   this session, all on the 2-row body:
     THE MAXIMUM IN EVERY OTHER VARIABLE, AT ITS OWN DECLARED WIDTH (the
     seventh session's table was on the 17-row body, so all of it was stale;
     re-swept with the `(s16)' on the compare kept in every candidate, which
     is what the fifth and sixth sessions left out):
         e (the park)  2     aa 3     fresh s16 3    fresh u16 3
         ret 4    h1 7    h2 9    h3 9
         g 14    r6 14    r7 14    dx 14    r9 15
         g22/i/j/r8/nn/a 16    fresh s32 16    fresh u32 16
         r1 19    r4 19    r5 20    t1 23    ang 24    t3 31    i2A 33
         r2 1817/66    d 1816/15    f 1816/22    r3 1819/50
         t4 1818/73    dy 1818/89    n 1818/93    k 1818/76    k2 1818/73
         u2 1820/47    fresh s8 1819/72    fresh u8 1818/73
     -- so `e' is the unique best and every alternative is 3 or worse.
     AND THE SAME SWEEP AGAIN WITH THE PARTNER NARROWED TO s16 FIRST, which
     is the only way an SImode local can hold the maximum without the abs
     folding into the copy:
         r5 3    r8 3    nn 3    r6 3    r1 3    r9 4    r4 6    t4 9
         t1 1810/19    r7 1816/84    n 1816/79    t3 1823/110
     -- still nothing at 2, and the narrowings `r6 -> s16/u16' and
     `k2 -> s16' are BYTE-IDENTICAL on their own, so the cost is the
     maximum's register and not the width.
     NARROWING AN SImode CALL-CROSSER TO GIVE IT THE RIGHT MODE: `r6' (which
     the target really does hold in $s0 across two calls, 800AEA80) narrowed
     to s16 or u16 is BYTE-IDENTICAL on its own, and with the maximum in it
     measures 3 -- `.lreg' then shows the case-0x24 range as a SEPARATE
     3-refs/14-insns allocno with calls=0, and the other arm's references are
     nowhere in the global allocno list at all.  Same for r1, r4, r5, r8, nn,
     aa (3..6), t1/t3/t4/n (9..110 or words lost).  A SHARED NAME DOES NOT
     SHARE A CALL CROSSING -- that is the MEASUREMENT; which pass splits the
     two ranges is NOT established, so do not cite a mechanism for it.
     WIDENING THE TWO `u8' LOCALS so they could be HImode partners -- `k' is
     the one other value the target really does hold in $s0 across a call
     (800AE084, across func_80042900) -- costs more than it buys: `u16 k'
     alone is 5 rows and with the maximum 8, `s16 k' alone 5 and with the
     maximum 1818/10, `u16 k2' alone 1819/27, `s16 k2' alone BYTE-IDENTICAL
     and with the maximum 3.
     `aa' WITH ITS LIVE RANGE DELIBERATELY EXTENDED across the loop's two
     calls (`aa = (s16)ang;' hoisted to the TOP of case 0x2A's loop body) is
     BYTE-IDENTICAL on its own -- so the hoist does nothing, gcc recomputes
     the `sra' at the use -- and 3 with the maximum in it.
     CARRYING A VALUE ACROSS case 0x24's func_800A0818 CALL in `mx':
         mx = sp18 before the call, `c[0x54] = mx' after    1816/75
         mx = sp1A before the call, `c[0x55] = mx' after    1816/86
         both                                               1816/75
         mx = b[0x24] before the call, `dx = mx' after      1817/37
     -- the target reloads sp18/sp1A from the stack AFTER the call
     (800AF17C/800AF180), so carrying them in a register deletes those loads.
     THE LAST ONE IS A POSITIVE RESULT AND WORTH KEEPING: its row 1212 is
     `lbu $s0, 0x24($s7)', so the maximum DOES reach $s0 the moment its
     allocno crosses a call, exactly as the proof says.  The 37 rows are the
     price of hoisting the `b[0x24]' load above func_800A0818 -- the target
     loads it at 800AF1A8, after the call -- not a failure of the register.
     So the route is real and every honest instance of it is too expensive.
     NAMING THE TEMP THE TARGET ALREADY HOLDS IN $s0 ACROSS A CALL.  At six
     places in cases 0, 1 and 2 the target has
         jal func_8009FD40 / ... / jal func_800A35A4
         addu $s0,$v0,$zero / sll $s0,$s0,16 / sll $v0,$v0,16 / slt $s0,$s0,$v0
     i.e. an anonymous 16-bit temp, saved across the second call in $s0 and
     then OVERWRITTEN BY THE COMPARISON RESULT.  Naming it at the three
     in-loop sites (the safe hoist -- the comparison is the first operand of
     its `&&', so both calls already happen unconditionally) does put it in
     $s0, and costs 5 rows per site: the copy lands before the argument setup
     instead of in the jal's delay slot, and the `slt' then uses $v1.
         named, one statement, s16, 3 sites              17   (15 with the
                                                              maximum shared)
         named, TWO statements (`mx = call; mx = mx < other;'), s16   17
         the same at u16 or s32                      1820/43..1820/49
     ROUTING THE PARAMETER THROUGH A LOCAL SO `e' KEEPS ONE SET.  This is the
     permuter's find (see below) and it works -- for (i):
         mxp = (s16)e; at the top, passed to func_800A19E4,
         maximum left in `e'                             3  <- ROWS 15/20 GO,
                                                            cluster II's 3
                                                            rows come back
     `.lreg' explains it exactly: the load folds into the copy, so the load's
     DEST is `mxp' (2 refs / 23 insns, crosses 2 calls, ONE SET) -> $s0 with
     the bump, and `e's pseudo is left holding only the maximum (3 refs / 14
     insns, calls=0) -> $a2.  Put the maximum in `mxp' instead and it is 2
     rows again: `mxp' becomes 5 refs / 44 insns with THREE sets, so (ii)
     holds and (i) fails.  The two facts swap and never hold together.
     A SECOND COPY IN A LATER BLOCK, to keep the once-set load and still have
     a call-crossing holder (`mxp = (s16)e;' at the top, `mx = mxp;' later,
     `mx' passed to the call, maximum in `mx'):
         copy just before the call      3   (mx 5 refs/14, calls=0)
         copy at the top of the `else'  2   (mxp merged away entirely)
         with every cse breaker on `mxp' after the copy -- `mxp++; mxp--;',
         `mxp = 0;', `mxp = 1;', `mxp++;', `mxp = (s16)e;'      all 3
     THE REASON IT CANNOT WORK, and it is worth keeping: `mx' is CONSUMED BY
     THE ARGUMENT SETUP, which is BEFORE the call, so its live range ENDS at
     the call and never crosses it.  A value passed to a call does not cross
     that call.  Only a value read AFTER one does, and between
     func_800A19E4 and case 0x24 there is no value computed before a call and
     read after it.
     AND THE SImode PARTNERS CANNOT BE RESCUED BY A SECOND SET.  The reason
     an SImode holder fails is that `mx = dx' becomes a plain copy and
     combine folds the abs straight into it (`move s0,v0 / negu s0,s0' for
     the target's `bgez / nop / negu' plus a separate `addu $s0,$v1,$zero');
     change (M) shows a second set is exactly what keeps a copy alive, so it
     was tried on every SImode call-crosser:
         r7 with `r7++; r7--;'  1818/69     with `r7 += 0;'  1817/14
         r6 with the pair       1818/69     with `+= 0'      1817/14
         r5 with the pair       1818/75     with `+= 0'      1817/20
         i2A with the pair      1818/88     with `+= 0'      1817/33
         a fresh s32 with the pair  1817/16, with a chain    1818/71
     -- nothing near 2, so the HImode requirement is not an artefact of how
     the copy was spelled.
     THE SET COUNT ON `e' CANNOT BE REDUCED BELOW 2 while it holds the
     maximum: `e = r9 ? sp1A : dx;', `if (r9) { e = sp1A; } else { e = dx; }'
     and `e = (sp18 < sp1A) ? sp1A : dx;' are all ONE source set, hence two
     with the parameter load, and all three measure 2.  `reg_n_sets == 1' is
     exact.
     AND THE MECHANISM IS CONFIRMED FROM THE OTHER SIDE: on the 3-row
     reverted body, where rows 15 and 20 are CLOSED, adding `e++; e--;'
     before the func_800A19E4 call leaves it BYTE-IDENTICAL at 3 rows -- the
     pair is folded before flow counts sets, so it is not a second set and
     the rows do not reopen.  `d++; d--;' there is byte-identical too, and
     `e = 0;' is 1815/13.  (Note the asymmetry with change (O)'s chains: a
     cancelled pair on `r7' DOES survive to flow and is worth +4 references,
     so "the pair is folded" is a fact about THIS variable at THIS site, not
     a general rule.)
     THE STATEMENT-SPLIT SPACE (`LHS = A <op> B;' -> `LHS = A; LHS = LHS <op>
     B;', the space the sixth session found change (K) in and which no sweep
     varies) re-run on THIS body:
         `t4 = b[0x25] - sp1A' split            16   (also as `t4 -= sp1A')
         `j = D_8008347E & 1' split, all three   8
         the k2 == 0x1E tail's `sp18 = b[0x24] + ...' split    3
         the same for `sp1A = b[0x25] + ...'                   3
         change (K)'s own `dx = b[0x24]; dx = dx - sp18;' PUT BACK
             TOGETHER as `dx = b[0x24] - sp18;'    BYTE-IDENTICAL, 2 rows
     -- so CHANGE (K) IS NOW WORTH NOTHING.  It bought 2 rows on the 23-row
     body and the one-statement form is byte-identical here.  The two
     statements are kept only because this body is the one measured against
     the whole unit; a future session may collapse them freely.
     A DEPENDENCE ON THE `c[0x46] |= 0x4000' STORE does not pin the load
     either: `*(u16 *)(c + 0x46) |= 0x4000;', the long `= .. | ..' forms, the
     `s16' spellings, and the store routed through a fresh pointer local
     (`o2 = c + 0x46; *(u16 *)o2 |= 0x4000;', which keeps 1817 words because
     combine folds the `addiu' back into the displacement) are ALL
     BYTE-IDENTICAL at 2 rows.
     MOVING THE ENTRY BLOCK'S OWN CONTENTS, since what sched1 is choosing the
     load's slot against is the rest of that block:
         `ret = 2;' past the D_80013714 test    1816/6
         `ret = 2;' into the `else' branch (same program, the if-branch sets
             0, and case 4 sets 0 again)        1815/9
         the `c[0x46] |= 0x4000' store past the test   1814/22
         both past the test                     1814/24
         the D_80013714 test inverted into an empty-then `else'
                                                BYTE-IDENTICAL
     THE DEVICE SPACE AT THE ENTRY BLOCK, swept by hand because refsweep and
     devsweep take 160 and 75 minutes on a body this size and the two rows
     are in the first twenty words: a cancelled pair `v++; v--;' for each of
     19 register locals at each of the entry block's four statement
     positions (76 candidates) is BYTE-IDENTICAL at 2 rows in EVERY case --
     and all 76 would read an uninitialised local anyway, so none is
     landable.  `do { } while (0)' and a duplicated arm on the entry block's
     own two statements are no better: around `ret = 2;' both nestings are
     1816/16 and a duplicated arm on any of the five parameters is
     byte-identical at 2; around the `c[0x46] |= 0x4000' store every one of
     them is 1818/6 or worse.  And on the 3-row basin, `do { } while (0)'
     around each of
     case 0x24's 17 statements in turn: the best is 3 (no change, around the
     func_800A6E8C call), then 5, 5, 5, 8, 8, 10, 10, and two at 1812 words.
     A FREE READ OF `e' IN THE ENTRY BLOCK, to raise the load's own sched1
     priority instead of relying on the bump: `ret = 2 + e - e;' is
     byte-identical (fold collapses it), and `ret = 2 + e + e - e - e;',
     the three-term version, the `d' version and both together are 11.

     rows 1330..1418  CLOSED by change (O).
     rows 15 and 20   the `lhu $s0,0x60($sp)' parameter load in the wrong
                      load-delay slot -- the price of change (N), 2 rows.
                      SPELLING-PROOF: byte-identical under every spelling of
                      the two statements above it (`*(u16 *)(c + 0x46) |=',
                      the long `= .. | ..' form either way round, `if
                      (D_80013714 & 8)' bare, an empty-then `else', `ret = 2'
                      moved after the `|= 0x4000', and a cancelled pair or a
                      dead set on `e' or `d' after the func_800A19E4 call).
                      AND THE DIAGNOSIS: sched.c's `adjust_priority' /
                      `birthing_insn_p' (sched.c:2513-2592) raises a set's
                      priority to `max_priority' only when
                      `reg_n_sets[dest] == 1'; sched1 is a BACKWARD list
                      scheduler, so the bump moves the insn LATER in the
                      emitted stream, which is where the target has it.  With
                      `e' holding the maximum it has three sets and loses the
                      bump.  IF THAT IS THE WHOLE STORY IT ALSO PROVES THE
                      TARGET'S `e' HAS EXACTLY ONE SET -- i.e. the original's
                      maximum lived in some other HImode call-crossing
                      variable -- but the experiment that would settle it
                      cannot be run: every second set of `e' that does not
                      lengthen its live range (`e++; e--;', `e = 0;') is gone
                      before flow counts sets, and measures 17, byte-identical.
     rows 1330..1418  cluster I, 14 rows, the $s0/$s2 swap.  CLOSED with the
                      four-way priority enumeration above.
   Residue (1) and clusters II, III and IV are CLOSED.  Every other word of
   the function is byte-identical to the target.

   ===========================================================================
   THE OLDER RESIDUE SECTION: 25 ROWS, 0 WORDS
   (rows 277/393/556 = residue (1), 3 rows -- see change (I) above;
    rows 1229..1248 = cluster II, 8 rows; rows 1330..1418 = cluster I, 14.)
   Clusters III and IV ARE CLOSED by changes (I) and (J).  Their paragraphs
   are kept below because every measurement in them is still a valid negative
   for the shape it names.

   (rows 277/393/556 = residue (1), 3 rows;
    rows 1235..1248 = cluster II, 6 rows; rows 1330..1418 = cluster I, 14.
    Rows 1229 and 1231 are CLOSED by change (K).)

   (1) Three identical rows, one per site of change (I):
           ours  andi $v1, $v0, 0xFFFF      target  addu $v1, $v0, $zero
       at rows 277 (case 0), 393 (case 1) and 556 (case 2).  The target's
       second pseudo is defined by a PLAIN SImode copy of the return register
       and ours by a zero-extension.  Everything priced for this slot is in
       change (I)'s rejected list: every plain-copy spelling collapses the two
       pseudos back into one and costs the 2 rows that (I) bought, and every
       other real recomputation (sign-extend, mask) is the same or worse.  The
       honest statement of the residue is: gcc 2.7.2 will not keep a
       pseudo-to-pseudo copy that computes nothing, so the target's copy was
       not written as a copy in C -- it is the live-range copy gcc makes for
       ITS OWN second pseudo, and we have not found the source form that makes
       gcc create that pseudo without an instruction.

   (I) case 0x2A: a clean two-register swap.          14 rows, 0 words.
       Ours                      target
         &D_80082E80 -> $s0        $s2
         i2A         -> $s0        $s2   (reusing the pointer's register)
         r7          -> $s2        $s0
       `ang' is $s1, `&D_8006CCE8' $s3, `&D_8006CCD8' $s4 in BOTH, and every
       instruction in the arm is right.  The dumps make it arithmetic:

         .lreg   Register 119  used 6 times across 83 insns; crosses 3 calls;
                 Register 1333 used 5 times across 44 insns; crosses 2 calls;
                 Register 128  used 9 times across 70 insns; crosses 3 calls;
         .greg   ;; 71 regs to allocate: ... 128 ... 1333 ... 119 ...
                 119 in 18   128 in 17   1333 in 16

       119 is `r7', 1333 is this arm's `&D_80082E80' (its lo_sum carries
       REG_EQUAL (symbol_ref D_80082E80)), 128 is `ang'.
         flow.c:2957   the dump prints reg_n_refs and reg_live_length, which
                       ARE allocno_compare's two inputs for a GLOBAL allocno
         global.c:420  allocno_n_refs      += reg_n_refs[i];
         global.c:422  allocno_live_length  = max over the pseudos
         global.c:596  pri = floor_log2 (n_refs) * n_refs / live_length
         global.c:606  tie-break: LOWER pseudo first
       so 1333 is 2*5/44 = 0.227 and 119 is 2*6/83 = 0.145, 1333 is allocated
       first and find_reg's 0..31 scan hands it the lowest free callee-saved
       register.  THE TARGET NEEDS 119 FIRST, and floor_log2 is a step
       function, so there are exactly three ways in:
         r7 at n_refs >= 8          3*8/83 = 0.289 > 0.227   (7 is NOT enough:
                                    2*7/83 = 0.169)
         the pointer at n_refs <= 3 1*3/44 = 0.068 < 0.145   (4 is not enough:
                                    2*4/44 = 0.182, still wins)
         the pointer's live_length > 69
       MEASURED AND REJECTED, on the 1815/130 body unless noted:
         the pointer routed through a named local `dp' at every non-empty
                      subset of its three use sites, with the assignment both
                      immediately before the first use and at the top of the
                      arm (14 candidates): 130 (no change) at best, then 137,
                      139, 150, 157, 158, 159, 166, 170.  cse unifies the two
                      base pseudos -- see the catalogue's "TWO BASE LOCALS IN
                      ONE EXTENDED BASIC BLOCK ARE UNIFIED BY cse INTO ONE
                      `lui'".
         loop spellings: `for (i2A = 0; ...)', `while (...) { ...; i2A++; }',
                      `while (++i2A < (s16)r7)', the three init statements
                      reordered -- all FIVE byte-identical.  Comparing `r7'
                      raw instead of `(s16)r7' is 1810/143 and deletes code.
       All of the above RE-RUN on the final 1817/35 body: the 14 `dp' splits
       are 35 (best, no change), 43, 45, 45, 56 and worse; the three loop
       spellings are byte-identical.  The cluster is unmoved by any source
       lever found so far.
         `r7++; r7--;' anywhere in the arm INCLUDING inside the loop:
                      byte-identical (cse2 runs before flow -- cse_main at
                      toplev.c:3376, flow_analysis at :3463 -- and folds the
                      pair back, so flow never counts it).  The "+4
                      references" route to n_refs >= 8 does not exist here.
       `i2A' must stay its own variable.

       2026-11-22 (fourth session).  THE ARITHMETIC ABOVE IS CONFIRMED on the
       30-row body, read off a fresh .lreg: the pointer is now pseudo 1332,
       `Register 1332 used 5 times across 44 insns', and r7 is still
       `Register 119 used 6 times across 83 insns'.  greg's allocation order
       lists `... 1202 81 568 1332 1551 130 800 119 118 ...', so 1332 is
       indeed allocated before 119.  And allocno_compare's formula was read
       off the compiler rather than remembered (global.c:588-610):

           pri = (double)(floor_log2 (n_refs) * n_refs) / live_length
                 * 10000 * allocno_size;        -- HIGHER goes first

       -- the `* 10000' is what keeps the division from truncating to zero, so
       the earlier header's float arithmetic was right.  pointer = 2272,
       r7 = 1445.  Exact thresholds: r7 n_refs >= 8, OR r7 live_length <= 52,
       OR pointer n_refs <= 3, OR pointer live_length >= 70.

       NEWLY MEASURED AND REJECTED on the 30-row body:
         ROUTE (b), shorten r7's live range by giving the loop bound its own
         name -- `lim = (s16)r7;' once, then `i2A < lim' and `i2A != lim'.
         THIS IS A DISASTER AT EVERY SPELLING and it is the clearest refutation
         the cluster has: fresh s32 lim 1815/108, fresh s16 1818/104,
         fresh u16 1815/107, fresh u8 1815/107, lim set at the guard
         1814/111 (s32) and 1818/104 (s16), and through an existing local
         1813/131..1815/107 across r1,r2,r3,r4,r5,r6,r8,t1,n,i,j,h1,h2,h3,k,k2.
         Renaming ONLY the post-loop site is 1816/118..1817/44.
         WHY: the target recomputes the sign extension at BOTH sites -- rows
         1410 and 1416 are two separate `sll $v0,$s0,16' -- so `(s16)r7'
         written twice is what the target has, and one `lim' deletes one of
         them.  THE LOOP BOUND MUST STAY `(s16)r7' AT BOTH SITES.
         ROUTE (a) via the guard: `if (r7 < 2 || r7 >= 9)' 1816/100,
         `if (r7 - 2 < 0 || r7 - 2 >= 7)' 1816/100, `(u16)(r7 - 2) > 6'
         byte-identical.  No honest spelling adds references to r7.
         A SIZED DECLARATION FOR THE POINTER: `extern u8 D_80082E80[N];'
         completing the incomplete array type, to try to make the address a
         constant and delete the competing allocno outright -- BYTE-IDENTICAL
         at N = 0x26, 0x27, 0x28, 0x40, 0x60, 0x100 and 0x400.  D_80082E80 is
         out of the image, so -G0 materialises it symbolically whatever its
         declared size.  (Measured in the onefunc standalone, where the
         declaration block is local; csrc/ovl_dungeon_decls.h was not touched.)

       2026-11-22 (SIXTH session).  A DIAGNOSTIC THAT PINS THE RESIDUE DOWN,
       and it is not landable (asm register pinning is a device) but it says
       what the fourteen rows are: `register s32 r7 __asm__("$16")' measures
       1817/27 and the four rows that are purely r7's -- 1337, 1338, 1360 and
       1410 -- ALL MATCH.  What is left over then moves onto the pointer and
       `ang', which swap into $s1/$s2 instead of the target's $s2/$s1, so the
       cluster is a THREE-way contention (target: r7 $s0, ang $s1, pointer
       $s2) in which only the r7/pointer pair is wrong today.  Other pinnings
       for the record: r7 at $18 1817/68, i2A at $18 1817/70, r7 $16 + i2A
       $18 1817/56, mx at $16 1817/29.

       2026-11-22 (SIXTH session).  THE CLUSTER IS NOW CLOSED WITH A PROOF
       RATHER THAN A LIST OF FAILURES, and the proof says what a candidate
       would have to look like.  Re-read on the 25-row body: the pointer is
       pseudo 1338 (`used 5 times across 44 insns; crosses 2 calls') -> $s0,
       r7 is 119 (`used 6 times across 83 insns; crosses 3 calls') -> $s2, and
       greg's order is `... 1208 81 570 1338 1557 130 806 119 118 ...'.
       THE 44 IS ALREADY DOUBLED.  insn 4194 is
           (set (reg 1338) (lo_sum (reg 1339) (symbol_ref "D_80082E80")))
       with a REG_EQUIV note, so update_equiv_regs (local-alloc.c:1094,
       `reg_live_length[regno] *= 2') has already run on it: the true length is
       22.  That kills the only route left in the fifth session's list from the
       other end -- giving the pointer a SECOND SET to lengthen its range
       instead REMOVES the doubling (the gate at local-alloc.c:1043 is
       `reg_n_sets[regno] != 1'), taking it from 44 to 22 and raising its
       priority from 2272 to 4545.  Exactly the wrong direction.
       AND THE "BLOCK $s0 WITH A THIRD ALLOCNO" ROUTE IS CLOSED BY THE
       CONFLICT SETS THEMSELVES:
           1338 conflicts: 81 82 88 119 127 + hard 2 4 5 6 29
            119 conflicts: 81 82 88 111 119 124 125 127 1338 1376 1387 1427
                           + hard 2 3 4 5 6 7 29
       EVERY pseudo that conflicts with the pointer also conflicts with r7.
       $s1 is already blocked for both (127 holds it), so the only thing
       missing for the target's assignment is a conflicting allocno holding
       $s0 at the pointer's turn -- and any EXISTING one that does that blocks
       $s0 for r7 as well.  So a candidate has to CREATE a quantity Q with all
       four of: pri(Q) > 2272, Q conflicts with 1338, Q does NOT conflict with
       119, and Q lands in $s0 (which needs allocno_calls_crossed[Q] != 0, or
       regs 2..15 otherwise excluded).  Q must therefore be born before the
       pointer's materialisation and dead before r7's birth, i.e. live across
       func_800A365C and dead by func_8009FD40w's return, and it must cross a
       call.  The arm offers no honest value with that live range: the only
       things live there are `b', `c' and the pointer itself, and the two
       guards between them produce nothing that outlives its own test.
       SO THE FOURTEEN ROWS ARE EITHER (a) one of the four priority routes --
       all four of which are now measured negatives at every spelling tried --
       or (b) a Q of that shape, which would not be C anyone wrote.

   (II) case 0x24's Chebyshev test.         WAS 13 rows; STILL 8, 0 words.

       2026-11-22 (SIXTH session).  THE EIGHT ROWS ARE **TWO INDEPENDENT
       FACTS**, and separating them is the session's contribution here.
       Side by side, rows 1229..1248:
           ours                      target
           lbu  v0,0x24(s7)          lbu  $v1,0x24($s7)
           subu v1,v0,a0             subu $v1,$v1,$a0
           sh   v1,0x18(sp)          addu $s0,$v1,$zero
           lbu  v0,0x25(s7)          sh   $v1,0x18($sp)
           move a2,v1                lbu  $v0,0x25($s7)
           subu v0,v0,a1             sll  $v1,$v1,16
           bgez v0,..                subu $v0,$v0,$a1
           move a0,v0                bgez $v0,..
           negu a0,v0                addu $a0,$v0,$zero
           sll  v1,v1,0x10           negu $a0,$a0
           ... (the slt block matches) ...
           move a2,a0                addu $s0,$a0,$zero
           sll  v0,a2,0x10           sll  $v0,$s0,16
       (a) `mx' IS $a2 AND THE TARGET'S IS $s0 -- 3 rows.  greg says mx is
           pseudo 130, `used 3 times across 16 insns, 2 bytes', with
           `130 conflicts: 81 82 121 123 128 129 130 2 3 5 29' and no
           calls-crossed line, so used1 = fixed_reg_set and find_reg's 0..31
           scan hands it the first register not held by a conflicting allocno:
           $v0 is 123's, $v1 is 121/128's, $a0 is 129's, $a1 is a hard
           conflict, and $a2 is free.  For $s0 (16) every register from 2 to 15
           must be excluded, which on this body means mx would have to CROSS A
           CALL -- and the fifth session's corrected reading of global.c:924
           is right about the code and wrong about the conclusion it drew:
           crossing no call is the least restrictive case, which is exactly why
           a short-lived non-call-crossing quantity can never reach $s0 by
           priority.  mx's live range is entirely between the dx abs and
           `if (mx == 1)', with no call in it, and there is no honest way to
           extend it across one: `dx' does not exist before the func_800A0818
           call.
       (b) THE `sll $v1,$v1,16' IS IN A DIFFERENT BASIC BLOCK -- 5 rows.
           This one is NOT a register fact and NOT a sched1 priority.  The dx
           abs is ONE rtl insn (`x >= 0 ? x : -x' folds to `abs', and the
           emitted text is the template's own `bgez %0,1f / negu %0,%0 / 1:'
           with no `.set noreorder'), so it does not split the block, but the
           dy abs IS a real branch in both streams.  The target's block
           therefore ENDS at the dy branch and already contains the comparison's
           `sll $v1,$v1,16'; ours contains four insns and emits that `sll' as
           the FIRST insn of the join block.  sched1 cannot move an insn
           between blocks, so the target's source must emit the sp18 widening
           BEFORE the dy branch.
           MEASURED AND REJECTED, all on the 25-row body, all BYTE-IDENTICAL
           unless noted -- the hypothesis was that gcc expands a comparison's
           LEFT operand (and its integral promotion) before its right, so
           burying the dy conditional in the right operand would strand the
           `sll' in the earlier block.  IT DOES NOT:
             r9 = sp18 < (sp1A = dy = (t3 < 0) ? -t3 : t3)   25
             r9 = sp18 < (sp1A = dy)                         25
             sp1A = dy = (t3 < 0) ? -t3 : t3                 25
             r9 = sp18 < (s16)dy                             25
             r9 = mx < sp1A                                  25
             r9 = (s16)dx < sp1A                             25
             mx = r9 ? sp1A : mx                             25
             dy = t3; if (t3 < 0) { dy = -dy; }              25
             if (t3 < 0) { dy = -t3; } else { dy = t3; }      25
             dy = t3 = sub; if (t3 < 0) { dy = -dy; }         25
             mx = dx moved after sp18 = dx                   25
             mx = (s16)dx / mx = sp18 / mx = dy              25
             r9 = sp18 < (dy = (t3 < 0) ? -t3 : t3)          30
             if ((r9 = sp18 < sp1A) != 0)                    28
             r9 = sp18 < dy                                  33
             r9 = dx < dy                                    1815/94
             sp1A = (t3 < 0) ? -t3 : t3 with no `dy'         1820/101
             r9 = sp18 < (sp1A = (t3 < 0) ? -t3 : t3)        1819/96
             sp1A = t3 = sub before the abs                  1818/93
             sp1A = dy moved after the comparison            1815/94
           AND THEN THE REASON WAS FOUND, AND IT IS A BASIN THE FOURTH
           SESSION DECLARED REFUTED.  **`dy = (t3 >= 0) ? t3 : -t3;' -- the
           FOLDING direction, recorded at 53 rows and "do not clean up either
           side" -- PRODUCES THE TARGET'S SCHEDULE EXACTLY.**  Read the two
           streams with it in place (1817 words, 48 aligned rows):
               ours                  target
               move a2,v0            addu $s0,$v1,$zero
               sh   v0,0x18(sp)      sh   $v1,0x18($sp)
               lbu  v1,0x25(s7)      lbu  $v0,0x25($s7)
               sll  v0,v0,16         sll  $v1,$v1,16     <- IN BLOCK B NOW
               subu v1,v1,a1         subu $v0,$v0,$a1
               bgez v1,..            bgez $v0,..
               move a0,v1            addu $a0,$v0,$zero
               negu a0,a0            negu $a0,$a0
               sll  v1,a0,16         sll  $v0,$a0,16
               slt  v0,v0,v1         slt  $v1,$v1,$v0
           -- instruction for instruction identical, opcode for opcode, and
           ONLY THE REGISTER ASSIGNMENT DIFFERS (ours dx $v0 / b[0x25] $v1 /
           mx $a2, target dx $v1 / b[0x25] $v0 / mx $s0).  The reason the
           earlier sessions scored it 53 and now 48 is that fixing the
           schedule makes EVERY row of the block differ by register, where the
           wrong schedule left some of them coinciding.  WHY IT WORKS: the
           folded form is a single `abs' rtl insn, so there is NO basic-block
           split at the dy abs, block B runs through the comparison, and sched1
           is free to put the sp18 widening in the `lbu's load-delay slot.  The
           emitted `bgez $v0,1f / addu $a0,$v0,$zero / negu $a0,$a0 / 1:' is
           the abs TEMPLATE with dest != src, whose delay slot GAS fills with
           the template's own `move' -- which is why it looks like a
           compiler-scheduled branch and is not one.  (The dx abs gets a `nop'
           instead because its negate writes the register the branch reads.)
           PROOF THAT ONLY REGISTERS ARE LEFT, with asm pinning as a
           diagnostic (not landable):
               folded dy alone                              1817/48
               + mx pinned $16                              1817/50
               + dx pinned $3                               1817/37
               + mx $16 AND dx $3                           1817/31
           and in that last one rows 1232..1247 -- the whole Chebyshev core --
           ARE BYTE-IDENTICAL TO THE TARGET.  What is left over is damage the
           pinning itself does to the preceding block (rows 1218..1228) plus
           the `sh' of sp18 reading mx's register.
           SO THE 6 REMAINING ROWS OF CLUSTER II ARE ONE DECISION: get `mx'
           into $s0 and `dx' into $v1 while keeping the folded dy.  Measured
           and rejected on the folded body: mx merged with aa 48, mx at s32
           1815/98, u16 1816/112, dx at s16 1817/59, u32 1814/110, dy at s16
           48, t3 at s16 1823/127, the comparison respelled four ways 48,
           `mx = dx' moved after `sp18 = dx' 48.
           AND NOTE THE INTERACTION: change (K)'s dx split, which is worth 2
           rows on the NON-folded body, is worth nothing on the folded one
           (dx split + folded dy = 48, as is t3 split + folded dy).  The two
           halves of this cluster are not additive, and the folded basin has
           to be entered with the register fix already in hand.
           WHICH IS WHY THE PARK STAYS NON-FOLDED AT 23 ROWS.  `do not clean
           up either side' is still the right instruction for the body in this
           file -- but the sentence "on dy you want the NON-folding one" is a
           statement about a local optimum, not about the original, and the
           original plainly used the folding form.  The next session's best
           single shot at cluster II is the folded body plus an honest route to
           mx-in-$s0.
       (c) The `lbu' register ($v0 for us, $v1 for the target) is the
           remaining 2 rows and follows (b): the target ties the subtraction's
           destination to the load's own register, which it can because the
           block's contents differ.

       RE-SWEPT ON THE 25-ROW BODY (the fifth session's table was on the
       30-row one, and the body has moved three times since): `mx' merged with
       each of the 28 other scalar locals -- `aa' 25 (byte-identical, the only
       other s16), k2 30, k 34, then ret 1816/92, sp1C 1816/93, sp1E 1816/93,
       h1 1816/95, h2/h3 1816/96, i/i2A/j 1815/109, r1 1815/112, r2 1814/120,
       r3 1814/122, r4 1815/111, r5 1815/113, r6/r7 1815/109, r8 1815/113,
       r9 1815/108, t1 1815/113, t3 1816/123, n 1816/124, nn 1815/108,
       ang 1815/114, dx 1815/98, dy 1816/110.  Unmoved.
       THE PARAGRAPHS BELOW ARE THE OLDER DIAGNOSIS and still read true.
       Changes (G) and (H) closed five of these.  What is left is `mx': the
       target holds it in $s0 and we hold it in $a2, and rows 1229, 1231,
       -1235, 1237, 1241, +1242, 1247 and 1248 all follow from that one fact.
       RE-PRICED ON THE 30-ROW BODY and still refuted:
         mx merged with each of the other 27 scalar locals (the park's old
             52-candidate sweep was on the 36-row body): `aa' is 30 (no
             change, both s16), k2 34, k 38, then ret 1816/96, sp1C 1816/97,
             h1/h2/h3 1816/99..100, dy 1816/114, t3 1816/127, n 1816/128,
             dx 1815/102, r9 1815/112 and everything else 1815/112..1814/127.
         `register s16 mx __asm__("$16")' 34, $17 36, $18 78, $19 87, $20 95.
         mx declared inside case 0x24's own block: 30, byte-identical.
         mx at s32 1815/113, u32 1815/113, u16 1816/95, u8 34, s8 1818/94.
       The paragraphs below are the old 13-row diagnosis and still read true
       for the `mx' half of it.
       Instruction for instruction correct and the word count is right; the
       target has a real `addu $s0,$v1,$zero' for `mx = dx' at .L800AF1C0 and
       puts `sll $v1,$v1,16' in the following load-delay slot, where we keep
       mx in dx's register, leave a nop there, and copy later.  mx is $a2
       where the target has $s0; dx is $v1 and dy's abs $a0 in both.
       THIS NEXT CLAIM, CARRIED BY THREE HEADERS, IS BACKWARDS -- corrected
       2026-11-22 by reading the code instead of the note.  It said "`mx' is
       in a callee-saved register in the target, which by global.c:925-932
       needs allocno_calls_crossed != 0".  What find_reg actually does
       (global.c:924-929) is

           if (accept_call_clobbered)      used1 = call_fixed_reg_set;
           else if (allocno_calls_crossed[allocno] == 0)
                                           used1 = fixed_reg_set;
           else                            used1 = call_used_reg_set;

       -- crossing NO call is the LEAST restrictive case: used1 is only the
       fixed registers, so every callee-saved register is available.  Crossing
       a call is what EXCLUDES the caller-saved ones.  So `mx' crossing no
       call does not bar it from $s0 at all, and "give mx a call to cross" is
       not the lever; do not spend compiles on it.
       What actually decides it is the 0..31 scan (MIPS defines no
       REG_ALLOC_ORDER, so lowest-numbered free register wins) together with
       pass 0's extra exclusions:

           used = used1 | ~regs_used_so_far | regs_someone_prefers[allocno]

       For `mx' to reach $s0 (16) rather than $a2 (6), every register from 2
       to 15 has to be conflicting or someone-preferred at that moment.  That
       makes this a `regs_someone_prefers' residue -- the catalogue's "it only
       helps when something actually PREFERS the register you want vacated".
       Two further things the earlier headers asserted are wrong:
         * FORCING the registers does not reproduce the target's code.
           `register s16 mx __asm__("$16")' is 1816/100 on this body (and was
           1822/171 on the 1821/170 one); adding `dx' in $v1 and `dy' in $a0
           was 1822/173 and 1822/172 there.  Every variant is worse and the
           target's `bgez/nop/negu' + separate `addu $s0,$v1,$zero' shape
           still does not appear.  This is not a residue a register settles.
         * `mx' AND `aa' CANNOT BE ONE VARIABLE.  The target holds `aa' in
           $v1 at 800AF464 (`sra $v1,$v0,16' for `aa = (s16)ang') and `mx' in
           $s0 at 800AF1C0.  A C local referenced in two basic blocks is ONE
           global allocno and gets ONE hard register (flow.c:2561,
           global.c:420-422), so two registers means two variables.
       Measured and rejected on THIS body: `mx' merged with every other
       declared scalar local, at s16 and at s32 (52 candidates) -- aa, k2, nn,
       r1..r6, r8 are all exactly 36 rows (no change at all), `ret' 37, `dx'
       51, `t3' 70, `t5' 86, `dy' 104, `ang' 117, everything else worse.
       Also rejected on the 1823/160 body: all twelve statement orders and
       store spellings inside the arm (`sp18 = dx' before `mx = dx', the
       `|= 0x8000' store moved to four other positions, the 0x54/0x55/0x4C/
       0x15 stores in pointer-cast form) -- nothing beats it.

   (III) CLOSED 2026-11-22 (sixth session) by change (I), which left one
       `andi' row at each of the three sites -- residue (1) above.  The
       paragraphs below are the fourth and fifth sessions' diagnosis; every
       measurement in them is still a valid negative for the shape it names,
       and the last one -- "any fix has to change which pseudo the store
       reads" -- was right.
       `sp20 = r;' WAS stored from the COPY, not from the call result.
                                                      6 rows, 0 words.
       Target, cases 0 and 1 and the case-2 site at 800AE720:
           lhu $v1,0x1E($sp) / sh $v0,0x20($sp) / sh $v1,0x2A($s5) /
           addu $v1,$v0,$zero / sll $v0,$v1,16 / bltz $v0,.L800AF8F0 / nop
       -- the sp20 store reads the RAW call result $v0 and fills the `lhu's
       load-delay slot.  Ours:
           lhu $3,30($sp) / nop / sh $3,42($21) / move $3,$2 /
           sll $2,$3,16 / bltz $2,$L220 / sh $3,32($sp)
       -- it reads the COPY and lands in the branch's delay slot instead.
       Both have the copy; only its position relative to the store differs.
       Measured and rejected ON THIS BODY:
         `sp20 = r2;' before `*(s16 *)(c + 0x2A) = sp1E;'
                      case 0 only 1809/72, case 1 only 1816/54, both 1808/80
         `sp20 = (s16)rN;' at all five sites: byte-identical
         the CHAINED form `sp20 = rN = func_800A384C(...);' -- which stores
                      sp20 straight from the call result, exactly what the
                      target's `sh $v0,0x20($sp)' does -- at every subset of
                      the four call sites (15 candidates): r5 and r8 are
                      byte-identical, r3 is 1816/53, r2 is 1809/71 and
                      deletes code.  The chain does not move the store.
       The trailing q7 block ALREADY has the target's order (`sp20 = r8;'
       then `*(s16 *)(c + 0x2A) = sp1E;') and matches there, so the two
       orders are not interchangeable and the current one is right.

       2026-11-22 (fourth session).  THE BEST HYPOTHESIS THIS CLUSTER HAS HAD,
       AND IT IS REFUTED.  Everything the target emits here is 16-bit --
       `sh $v0,0x20($sp)' from the raw result, `sll $v1,$v1,16 / bltz' for the
       test, `ori $v0,$v1,0x8000' for the or -- which is exactly what you get
       if there is NO `rN' VARIABLE AT ALL and the value simply IS `sp20', an
       s16 stack local, with cse keeping the stored value in $v1 instead of
       reloading it.  That also explains the store reading $v0: with no `rN'
       to copy into first, the store has no dependence to wait for and sched1
       can lift it into the `lhu's load-delay slot.  All five sites have the
       identical shape, so it was swept at all 31 non-empty subsets of

           rN = call;  sp20 = rN;  if ((s16)rN <op> K) { ... rN | 0x8000 ... }
        -> sp20 = call;             if (sp20 <op> K) { ... sp20 | 0x8000 ... }

       (safe on value: sp20 is s16, and every use of rN is 16-bit or stored
       through a u16, so the truncation is unobservable).  RESULT:
         r4/case2a, r5/case2b, r8/q7tail -- BYTE-IDENTICAL, singly, in every
             pair and all three together (30 rows, 1817 words).  Those three
             `rN' variables are therefore unnecessary; they are kept only so
             the five sites stay spelled alike.
         r3/case1 -- 1816/48 at every combination.  LOSES A WORD.
         r2/case0 -- 1809/66..1810/58 at every combination.  LOSES EIGHT.
         both -- 1808/74.
       So the hypothesis fails at EXACTLY the two sites the residue is about,
       and `r2' and `r3' are load-bearing.  Cases 0 and 1 really do have a
       register variable distinct from sp20; the residue is the scheduler's
       choice of which pseudo the store reads, not the variable's existence.

       ALSO NEWLY PRICED, the two assignment shapes that make sp20's store
       read the raw call result while KEEPING rN (the header had only priced
       the `sp20 = rN = f()' nesting, where the store still reads rN):
             rN = sp20 = f();            r4 31, r5 37, r8 38, pairs 38..45,
                                         r2 and r3 1816/44..1809/72
             sp20 = f(); rN = sp20;      r4 31, r5 37, r8 38, pairs 38..53,
                                         r2 1816/44, r3 1816/45
       Every one is worse than the 30-row baseline at every subset of the five
       sites.  `sp20 = rN;' as its own statement after the call is right.

       CLASSIFY IT BEFORE SPENDING COMPILES: this is NOT a sched1 priority
       tie, so sched.py will not settle it.  Our store cannot be hoisted into
       the `lhu's load-delay slot because it has a real DEPENDENCE on the
       `move $3,$2' that materialises r2, and the target's has none (it reads
       $v0).  Any fix has to change which pseudo the store reads, and the two
       source-level ways to do that -- drop rN, or chain the assignment in
       either nesting -- are both priced above and both fail at cases 0 and 1.

   (IV) CLOSED 2026-11-22 (sixth session) by change (J) -- it was not a
       `break' reaching the tail early, it was case 2's 0x20000 block sitting
       one brace too far out.  See (J).
       (The `beq v0,s5' operand-order row at target 505 is CLOSED -- see the
       sixth change below.)

   (F) `if (c != *(u8 **)(D_800814A8 + j * 4 + 0xAC))' instead of
       `if (*(u8 **)(...) != c)' in cases 0, 1 and 2.          1817/36 -> 1817/35
       Only ONE of the three sites emits a comparison at all (the target has
       exactly one `beq' against $s5 in the whole function, at 800AE658, and
       so do we), so only the third site is observable: flipping it alone is
       35 rows, flipping either of the other two alone is 36 (byte-identical),
       and flipping all three is 35.  All three are flipped, because a
       uniform spelling is the honest one when two of the three are
       unobservable.  The catalogue entry is "COMPARISON OPERAND ORDER IS
       WORTH A WORD WHEN IT DECIDES A BLOCK'S FIRST INSN".

   ===========================================================================
   THE WORD COUNT WAS A LIE ONCE: 1817/1817 WAS TWO CANCELLING ERRORS

   The 2026-11-21 morning session reached 1817/1817 with `s32 t1 -> s16'.
   That narrowing DELETES A BASIC BLOCK THE TARGET HAS, at
   800ADF90..800ADFAC:

       lui  $v0, (0x10000 >> 16) / lhu $v1, 0x24($s5) / nop /
       slt  $v0, $v0, $v1 / beqz $v0, .L800ADFAC /
       ori  $v1, $zero, 0xFFFF / sh $v1, 0x24($s5)

   -- the `if (t1 > 0x10000)' guard, emitted in full.  At `s16'
   `shorten_compare' folds it at TREE level:

       c-common.c:2049  if (!real1 && !real2
                            && TREE_CODE (primop1) == INTEGER_CST
                            && TYPE_PRECISION (TREE_TYPE (primop0))
                               < TYPE_PRECISION (*restype_ptr))
       c-common.c:2108  else if (code == GT_EXPR)
                          { if (min_gt)  val = boolean_true_node;
                            if (!max_gt) val = boolean_false_node; }

   TYPE_MAX_VALUE(s16) is not > 0x10000, so `!max_gt' holds, the comparison
   becomes boolean_false_node, and guard and body both vanish.  At `s32' the
   precision test fails and the compare is emitted exactly as the target has
   it.  SO A COMPARISON THAT APPEARS IN THE TARGET BUT CAN NEVER BE TRUE IS
   POSITIVE EVIDENCE THAT ITS OPERAND IS 32-BIT.  `s32 t1' is not negotiable.

   Three more narrowings have the same shape, and the same trap caught a
   candidate THIS session too (see "swap" below):
     s32 t1 -> u8    1817 words but ALSO wrong on value (t1 is read from and
                     stored back to *(u16 *)(c + 0x24))
     u8  k  -> u32   1817 words, deletes FIVE words at 800AE0B8..800AE0C8
     s32 dx -> u32   deletes the `bgez/nop/negu' abs at 800AF1B4..800AF1BC
   and inverting the function's FINAL test deletes the canonical .L800AF8F0
   block.

   RULE: before believing any candidate that moves the word count toward
   1817, READ THE `+' AND `-' ROWS.  A contiguous run of four or more `-'
   rows is a basic block the target has and you deleted.  A single `-' paired
   with a single `+' is two errors cancelling.  Measured example from this
   session, on the 1816/46 body: swapping the two stores inside
   `if (c[0x66] < c[0x25])' scores 1817 words and 44 rows -- BETTER on both
   counts -- and it is wrong twice over: it emits `sb ..,0x24' before
   `sb ..,0x25' where the target has 0x25 first, and it reaches 1817 by
   losing the empty delay slot at 800ADFE4 and gaining a redundant
   `lbu $v1,0x66($s5)' reload.  Change (E) reaches 1817 honestly instead.

   RE-SWEPT ON THIS BODY: every declared width of every one of the 33
   non-pointer locals, all six integer types (165 candidates).  Nothing
   improves rows at any word count.  The widths are settled.

   ===========================================================================
   SWEEPS RUN, AND WHAT THEY RETURNED

   2026-11-22, EIGHTH SESSION.  All three quick sweeps were re-run on the
   2-ROW body and ALL THREE ARE CLEAN, each re-deriving `base 2 rows,
   1817/1817 words' first:
       stmtsweep   23 movable assignment(s), "no safe move helps"
       widthsweep  "no width change helps"
       partsweep   "no partition helps", four groups now -- `0' {i,i2A,ret},
                   `*(u8 **)(D_800E3D7C + i * 4 + 0xAC)' {g,g22},
                   `*(u16 *)(c + 0x98)' {h1,h2,h3},
                   `func_8009FD40w(D_80082E80, b)' {r6,r7}, `b[0x24]' {dx,sp18}
   refsweep (50 names, 38060 insertion points) and devsweep (356 statements x
   50 names = 18156 per round) were launched on the 2-row body and were still
   running when the session ended; logs are
       /tmp/claude-1000/agent-ade74i/logs/refsweep.log
       /tmp/claude-1000/agent-ade74i/logs/devsweep.log
   and both re-derived `base 2 rows, 1817/1817 words' before starting, so
   their baseline is this body.

   THE PERMUTER ON THE 2-ROW BODY IS A CLEAN NEGATIVE.  Imported fresh from
   this park and run 50 minutes at 10 jobs:
       2026-11-22  func_800ADE74  50m  88323  base=120  best=120
                   rate=29.4/s  found=none  jobs=10
   -- 88,323 candidates and it never beat the park's own score once.  Taken
   with the three quick sweeps clean on this body AND on the 3-row basin, and
   with the find_reg proof in the residue section, the two rows are as closed
   as this park can make them from the source side.

   THE PERMUTER RUN BEFORE IT, same session, on the 16-ROW body (55 minutes,
   10 jobs, 91653 iterations, base=195, best=100).  ONE find, `output-100-1'
   -- moved aside to /tmp/claude-1000/agent-ade74i/old-outputs/ so a later
   measure_finds cannot mix the runs -- and GATE 3 IS AGAIN WHAT
   MATTERED -- it carries TWO changes and they are worth completely different
   things:
       `mxp = (s16)e;' in the entry block, passed to func_800A19E4 in place
           of `(s16)e'                      THE DIAGNOSIS OF ROWS 15 AND 20.
           On the 16-row body it is part of the find; on the 2-row body it
           CLOSES rows 15 and 20 and reopens cluster II's three, i.e. 3 rows.
           It is the measurement that turned the seventh session's
           `reg_n_sets' hypothesis into a proof -- see the residue section.
       `nv = (s32 *)(c + 0x1C);' as a base local shared by case 0x20's two
           tests and case 0x1C's                      not separately measured
           (case 0x1C's test text occurs eight times, so the split needs a
           longer anchor than a one-line replacement).
   `best=100' against a base this run never printed; measure, do not read the
   score.

   2026-11-22, SEVENTH SESSION.  The three quick sweeps were run on the 17-row
   body (i.e. after changes (L) and (M), before (N)) and ALL THREE ARE CLEAN,
   each re-deriving `base 17 rows, 1817/1817 words' first:
       stmtsweep   22 movable assignment(s), "no safe move helps"
       widthsweep  "no width change helps"
       partsweep   "no partition helps".  Note the two groups it now reports:
                   `b[0x24]' -> {dx, sp18} and `dx' -> {mx, sp18}; after
                   change (N) the second is {e, sp18}.
   refsweep and devsweep were launched on the 20-row body and were still
   running when the session ended -- their logs are
       /tmp/claude-1000/agent-ade74h/logs/refsweep20.log
       /tmp/claude-1000/agent-ade74h/logs/devsweep20.log
   and their baseline is TWO ROWS WORSE than this park, so re-launch them on
   this body rather than reading those logs as negatives for it.
   NEXT SESSION'S BEST SHOT IS THE PERMUTER ON THIS BODY.  The residue is now
   two facts and fourteen of the sixteen rows are ONE of them (`r7' in $s0 and
   `ang' in $s1), which is a program-shape question about case 0x2A that no
   sweep varies -- and the permuter's object diff will see every one of those
   fourteen rows, so a find there is immediately legible.

   2026-11-22, SIXTH SESSION.  stmtsweep, widthsweep and partsweep were run
   on the 26-row body and again on the 25-row body, and ALL SIX RUNS ARE
   CLEAN:
       stmtsweep   base 26 / base 25, 19 movable, "no safe move helps"
       widthsweep  base 26 / base 25, "no width change helps"
       partsweep   base 26 / base 25, same six groups, "no partition helps"
   All five were relaunched on the 23-row body in this file.  THE THREE QUICK
   ONES CAME BACK CLEAN ON IT TOO -- stmtsweep "no safe move helps" (19
   movable), widthsweep "no width change helps", partsweep "no partition
   helps" (same six groups), all three re-deriving `base 23 rows, 1817/1817'
   first.  refsweep (36735 insertion points, ~160 min) and devsweep (17250 per
   round, ~75 min) were still running when the session ended; logs are
       /tmp/claude-1000/agent-ade74g/logs/refsweep23.log
       /tmp/claude-1000/agent-ade74g/logs/devsweep23.log
   and a SECOND permuter run was imported on this 23-row body and launched
   (40 min, 5 jobs) -- /tmp/claude-1000/agent-ade74g/logs/permrun23.log, with
   the previous run's ten output dirs moved aside to
   /tmp/claude-1000/agent-ade74g/old-outputs2/ so measure_finds cannot mix the
   two runs.  REMEMBER `export SPD=<a scratch dir>' before measure_finds.py.
   NEW SWEEP SPACE, AND IT PAID 2 ROWS: the STATEMENT SPLIT
   (`LHS = A <op> B;' -> `LHS = A; LHS = LHS <op> B;').  No existing sweep
   varies it.  Nine candidates in this body, one of them is change (K), the
   other eight byte-identical or worse.  Script at
       /tmp/claude-1000/agent-ade74g/splitsweep.py
   and it belongs in tools/decomp next to the other five.

   2026-11-22, FIFTH SESSION.  partsweep.py (new that day, and the ONE space
   no other sweep varies) run on the 30-row body IN THE CARVED UNIT, with the
   prototype block in place:
       partsweep   base 30 rows, 1817/1817 words      CLEAN
                   -- "no partition helps".  It enumerated six
                      same-expression groups and 21 partitions:
                        `0'                                  i, i2A, ret
                        `*(u8 **)(D_800814A8 + j * 4 + 0xAC)' q1, q3, q5
                        `*(u8 **)(D_800E3D7C + i * 4 + 0xAC)' g, g22
                        `*(u16 *)(c + 0x98)'                  h1, h2, h3
                        `func_8009FD40w(D_80082E80, b)'       r6, r7
                        `dx'                                  mx, sp18
                      -- so `g'/`g22' (change (g)) and `mx'/`sp18' and
                      `r6'/`r7' are all confirmed as drawn, and the three
                      `h' and three `q' locals must stay separate.
   THE OTHER FOUR SWEEPS' RESULTS BELOW TRANSFER TO THE CARVED UNIT UNCHANGED,
   and that is proved rather than assumed: with the prototype block restored
   the generated text is IDENTICAL to the pre-carve build (aligndiff output
   diffs clean except for the disassembler's absolute branch addresses), and
   the declaration set the compiler sees is the same, so a candidate measured
   pre-carve measures the same now.  gcc 2.7.2 has no interprocedural
   analysis, so the rest of the unit cannot matter.

   ON THE CURRENT 1817/30 BODY (2026-11-22, fourth session), with each sweep
   given its OWN AZURE_SCRATCH -- all four under one scratch produced
   `symbol func_800ADE74 is already defined' from concurrent compiles, which
   looks exactly like a broken park and is not:
       stmtsweep   base 30, "no safe move helps"           CLEAN (19 movable)
       widthsweep  base 30, "no width change helps"        CLEAN
       refsweep    49 names, 36686 insertion points        CLEAN
                   -- "final 30 rows, 1817/1817 words / no cancelled pair
                      helps".  That is the THIRD full cancelled-pair sweep of
                      this function to come back empty.
       devsweep    344 statements x 49 names = 17200/round
                   -- reached 23 rows at 1817 words in three rounds AND IT IS
                      NOT LANDABLE.  See the next block.  All four tools
                      independently re-derived `base 30 rows, 1817/1817 words'
                      on this body first, so the baseline is confirmed four
                      times over.

   ===========================================================================
   DEVSWEEP'S 23-ROW FIND IS BUILT ON UNDEFINED BEHAVIOUR.  DO NOT LAND IT.

   devsweep (2026-11-22, on the 30-row body) reported 30 -> 23 rows at 1817
   words, "verified against the real unit", with three duplicated-arm devices:

       if (d)  { func_800A9A0C(c); }        else { func_800A9A0C(c); }
       if (g)  { ang = n; }                 else { ang = n; }
       if (r7) { q7 = func_800A3D18(b, c, 2); }
                                            else { q7 = func_800A3D18(b, c, 2); }

   TWO OF THE THREE TESTS READ AN UNINITIALISED LOCAL:
     * `ang = n;' is inside CASE 0x2A's loop, and case 0x2A never assigns `g'
       -- every assignment to `g' is in a different switch arm, so reaching
       this statement means `g' was never written.
     * the `q7 = func_800A3D18' site is in the trailing code AFTER the switch,
       reachable by `break' from about twenty arms, and `r7' is assigned ONLY
       in case 0x2A.
   `if (d)' is the only safe one -- `d' is a parameter.
   devsweep cannot see any of this: its objective is an object diff, and a
   duplicated arm is identity in its model whatever the condition reads.

   SPLIT, each device alone and in every combination, on the 30-row body:
       if (d)   alone                    34   <-- the ONLY SAFE one, and it is
                                               a FOUR-ROW REGRESSION
       if (g)   alone                    29   (UB)
       if (r7)  alone                    34   (UB)
       if (g) + if (r7)                  25   (UB x2)
       if (d) + if (g)                   27   (UB)
       if (d) + if (r7)                  38   (UB)
       all three                         23   (UB x2)
   So EVERY row of the improvement depends on at least one uninitialised read,
   and the safe part of the find is worse than doing nothing.  Substituting a
   variable that IS live at the `ang = n;' site, on top of `if (d)': b 39,
   c 39, sp1E 39, e 43, f 43, k2 43, i 82, j 298, a 1818/42.  Nothing honest
   is anywhere near 30.

   THE PARK BODY THEREFORE STAYS AT THE HONEST 30 ROWS, and "NO DEVICES"
   below stays true.  Recorded here so the next session does not re-find this
   and land it; devsweep will report it again on any body close to this one.
   RULE: a duplicated-arm find is only valid if its CONDITION is a variable
   that is definitely assigned on every path reaching that statement.  Check
   that before measuring anything, not after.

   Logs for the two long sweeps, both run with `python3 -u':
       /var/tmp/ade74e/logs/refsweep.log   /var/tmp/ade74e/logs/devsweep.log
   devsweep's device body: /var/tmp/ade74e/s_devsweep/devsweep/func_800ADE74/best.c
   (deliberately NOT copied into this park -- it is UB).

   BUDGET THESE TWO PROPERLY ON A FUNCTION THIS SIZE.  A candidate costs
   ~0.26 s in the onefunc standalone here, not the ~0.09 s the procedure
   quotes, so refsweep is ~160 MINUTES and devsweep ~75 minutes PER ROUND --
   not "minutes".  A first attempt at 90 minutes under `timeout' was killed
   mid-sweep and produced NOTHING AT ALL, because both tools buffer stdout to
   a file in 8 KB blocks and their whole verdict is a few hundred bytes, which
   SIGTERM then discards.  RUN THEM AS `python3 -u tools/decomp/refsweep.py'
   so progress and findings survive a kill, and give them hours.
   AND GIVE EACH SWEEP ITS OWN AZURE_SCRATCH: all four launched under one
   scratch directory failed instantly with
       {standard input}: Error: symbol `func_800ADE74' is already defined
   from concurrent compiles sharing one `ovl_dungeon.s'.  That looks exactly
   like a broken park -- parkcheck said 0 bad throughout -- and is not.
   Note for the next session: the four-sweep objective change in commit
   17f1b0d does NOT invalidate a clean result recorded on a WORD-EXACT park.
   The old gate was `ww == tw and (rr, ww) < best' with best = (rows, w); when
   w == tw that is exactly equivalent to the new `(abs(ww - tw), rr)' key.
   The bug only bit a park whose words differed from the target -- which this
   one was on 2026-11-21 (1826/1817) and is not now.

   On the 1823/160 body: 38010 CANCELLED-PAIR insertions -- `v++; v--;' for
       all 50 incrementable names at every legal statement position, the full
       refsweep space no previous session had finished.  Fifty-five scored
       under 160 rows and NOT ONE IS USABLE: every one is on `sp18', `sp1A',
       `sp1C', `sp1E' or `sp20', which are ADDRESS-TAKEN and so live in
       memory, making `sp18++; sp18--;' a real lhu/addiu/sh pair costing 1 to
       6 words (1824..1829) for 1 to 3 rows.  A cancelled pair is only free
       on a register local.
   On the 1815/130 body: the same 37810-candidate sweep -- one hit at 128
       rows, and it is `sp20++/--', the same defect.  Nothing free helps.
   On the 1817/36 body: every adjacent same-indent statement swap in the
       whole body (94) -- nothing.
   On the 1817/36 body: stmtsweep's safe single-assignment moves (28)
       -- nothing.
   On the 1817/36 body: 165 declared-width candidates -- nothing.
   On the 1817/36 body: 52 `mx'-merged-with-another-local candidates and four
       `register s16 mx __asm__' pinnings -- nothing (see residue (II)).
   On the 1815/130 body: all ten `c[0x46] = 0x800B' sites in array form,
       singly, in every pair and in every triple (175) -- nothing.
   Earlier sessions, still valid for the shapes they cover: devsweep at 17550
       candidates per round ("no wrapper helps"); every `*(T *)(V + K)'
       converted to `((T *)V)[K/sizeof T]' (142 singly, 448 pairs, 30 whole
       groups); every arm's `g'/`nd'/`i'/`j' split out; every same-width pair
       of locals merged (~350); 80 random declaration-order reorderings.
       All neutral or worse.  NOTE THAT ALL OF THOSE PREDATE CHANGES (A)-(E)
       and the body has moved a very long way; re-run them before trusting a
       negative.

   ===========================================================================
   WHAT EARLIER SESSIONS CHANGED AND WHAT EACH WAS WORTH
   (still in this body; do not undo any of them)

   (a) s16 t1 -> s32                                 -11 rows, +8 words that
       were already there and were being hidden.  See above.
   (b) The function's LAST test written the other way round:
           if ((s16)r8 >= 0) { c[0x46] = r8 | 0x8000; return 1; }
           c[0x46] = 0x800B; return 2;
       -9 rows.  A BLOCK ORDER fix: the target's `bltz $v0, .L800AF8F0' at
       800AF8D8 makes the r8<0 arm the out-of-line one.  It is what creates
       the canonical .L800AF8F0/.L800AF8F4 pair that changes (C) and (D) then
       let every other 0x800B site cross-jump into.  The other two tests of
       this family must NOT be inverted (case 0's `if ((s16)r2 < 0)' and case
       4's `if ((s16)r5 >= 0)' are already right; case 1's is `< 4').
   (c) ((s16 *)c)[0x15] = func_800A0818(...) in cases 0, 1 and 2's first
       return-4 block, all three together                        -15 rows.
       docs/matching-levers.md's "MEM_IN_STRUCT_P NEEDS A PLUS_EXPR ON TOP"
       (expr.c:5332-5344; then sched.c:854 / 895 / 904 drop the dependence
       between the store through `c' and a later -G0 global load or stack
       store).  THE RELEVANT FLAG IS THE ONE ON THE **STORE**.
   (d) ((u16 *)c)[0x23] |= 0x4000;  (first statement)             -7 rows.
   (e) ((u16 *)c)[0x23] = r5 | 0x8000; in case 4                  -5 rows.
   (f) case 0x24's four `c'-relative stores in array form, together -9 rows.
       ALL-OR-NOTHING: the minimal paying subset is the PAIR
       {((s16 *)c)[0x15], ((u16 *)c)[0x54]}; either alone is worth nothing.
   (g) case 0x22's `g' given its own variable `g22'               -5 rows.
       `g' dies at the `func_800A2CAC(g)' call, so an arm-local variable
       crosses no call, global.c:927 leaves only the fixed registers out of
       `used1', and find_reg's 0..31 scan hands it $a0 -- the argument
       register, so the copy disappears.  A `g' shared with the arms that
       keep it across a call can only ever get $s0.  Splitting any OTHER
       arm's `g' is worth nothing or hurts (all fourteen at once: +30).
   (h) ((u16 *)c)[0x23] = 0x8009; at case 0x2A's and case 0x1C's tails,
       BOTH OR NEITHER                                            -7 rows.
   (i) ((u16 *)c)[0x23] = (sp1C + 1) | 0x8000; at ALL SEVEN sites
                                                     -15 rows, -2 words.
       The minimal paying subset is the three inside the cases 0/1/2
       do-while loops.  ANY ONE ALONE IS A DISASTER (1832/204, 1843/211,
       1832/216).  It buys the target's interleave at 800AE63C..800AE650.
   (j) In case 4, `ret = 0;' AFTER the func_800A03C4 call         -2 rows.
       The target computes it late (`addu $fp,$zero,$zero' at 800AE7AC, in
       the jal's shadow).

   ===========================================================================
   BLOCK ORDER: CHECKED AND CORRECT.

   `aligndiff.py -v' finds ONE monotone alignment and there is no transposed
   chunk anywhere: the 22 switch arms are emitted in the listing's address
   order (0x19, 0x16, 0x2C, 0x2B, 0x22, 0x29, 0x28, 0x1A, 0x23, 0x26, 0x27,
   0x20, 0x24, 0x25, 0x18, 0x2A, 0x1B, 9/0xA, 0x17, 0x1C, 0x1D, 0x15), and so
   are the four arms of the first switch and the trailing code.  THE SOURCE
   ORDER OF THE ARMS IS A FIXED PARAMETER.

   There is no frame-size cluster.  0x50 frame, $s0-$s7/$fp/$ra at
   0x28..0x4C, sp18/sp1A/sp1C/sp1E/sp20 at 0x18/0x1A/0x1C/0x1E/0x20,
   outgoing arg 5 at 0x10($sp).

   ===========================================================================
   SHAPES I AM NOT HAPPY ABOUT, WITH THE HONEST ALTERNATIVE AND ITS COST

   * THE `((T *)c)[K]' SPELLINGS.  Nineteen of ~160 `c'-relative accesses are
     array references and the rest are not, and no plausible original mixes
     them like that.  The honest reading is that `c' is a struct and these
     nineteen are field accesses (a COMPONENT_REF also sets
     MEM_IN_STRUCT_P) -- but then ALL of them would be, and that is measured:
     a uniform struct does NOT reproduce the target and a mixed spelling
     does.  The alternative -- the pointer-cast form at all nineteen --
     measured 1825/228 against 1823/160 when it was last priced.  Kept, but
     this is the one shape in the body that is still not explained.
   * (SETTLED BY CHANGE (L): BOTH CONDITIONAL NEGATES IN case 0x24 ARE NOW
     WRITTEN THE SAME, FOLDING WAY, `(x >= 0) ? x : -x'.  The paragraph below
     is the sixth session's and its suspicion was right.)
   * `dx = (dx >= 0) ? dx : -dx;' next to
     `dy = (t3 < 0) ? -t3 : t3;' in the SAME arm, AND I AM NOW FAIRLY SURE IT
     IS WRONG -- the original almost certainly wrote both the same way.  See
     cluster (II)(b): the SYMMETRIC, folded spelling
     `dy = (t3 >= 0) ? t3 : -t3;' reproduces the target's schedule for the
     whole Chebyshev block instruction for instruction and loses only on
     register assignment, and with `mx' and `dx' pinned to the target's
     registers its core 16 words are byte-identical.  It is kept unfolded only
     because 23 < 48 on today's body.  Do not "clean it up" without the
     register half in hand, and do not treat the asymmetry as explained.
   * `r9 = sp18 < sp1A;' followed by `if (r9)' in case 0x24.  A boolean in an
     s32 scratch is not the prettiest C, but `r9' is the arm's own variable
     and holds the arm's own call result two statements earlier, which is the
     same idiom as r1..r8 everywhere else in this body.  It replaced a
     permuter find that reused the DEAD `r5' from twenty arms away, and it is
     the only shape found that reaches 32 without borrowing another arm's
     variable.  The plain `if (sp18 < sp1A)' costs 3 rows -- see (G) for the
     full list of what does not work.
   * `g22'.  A second pointer variable for one arm is what the register
     allocation demands (see (g)) and is an ordinary local, but the name is
     mine.  One shared `g' costs 5 rows.
   * `r4', `r5' and `r8' are UNNECESSARY: dropping each of them and assigning
     the call result straight to `sp20' is byte-identical (see residue (III)).
     They are kept only so all five `sp20 = rN' sites read alike.  `r2' and
     `r3' cannot go -- they cost 1 and 8 words.
   * THREE CANCELLED PAIRS, AND THE `NO DEVICES' CLAIM IS NO LONGER TRUE.
     `u2++; u2--;' at each of the three sites of change (M).  It is the
     catalogue's cancelled pair, it compiles to nothing (combine folds the two
     `addiu'; 1817 words either way), and it is there for ONE reason which is
     PROVED rather than swept: within a basic block the only thing that stops
     cse rewriting `r2' back to `u2' and deleting the copy gcc emitted for
     `r2 = u2' is a SET of `u2'.  See change (M).  Every other breaker --
     `u2++;' alone, `u2 = 0;', `u2 = 1;', `u2 = -1;', `u2 = r2;',
     `u2 = sp1E;' -- measures identically, so the pair is chosen only because
     it is the spelling the catalogue already names.  RE-PRICED ON THE 2-ROW
     BODY, and all three are still load-bearing: dropping all three is
     1808/56, dropping the two in cases 0 and 1 is 1809/60, and dropping the
     one in case 2 is 1819/26.  They are not residue from an older body.
   * `e', THE FIFTH PARAMETER, IS REUSED AS case 0x24's MAXIMUM.  See change
     (N).  It is legal and defined (`e' is a parameter, and it is dead from
     the func_800A19E4 call onwards), it closes all three of cluster II's
     rows, and it is almost certainly NOT what the original said -- it costs
     rows 15 and 20 and the residue note above argues the target's `e' has one
     set.  IT IS KEPT BECAUSE 16 < 17 AND NOTHING ELSE REACHES $s0.  To get
     the honest 17-row body back: re-add `    s16 mx;' to the declarations and
     change `e = dx;', `e = sp1A;' and `if ((s16)e == 1)' back to `mx = dx;',
     `mx = sp1A;' and `if (mx == 1)'.  Four lines.
   * TWO FREE-REFERENCE CHAINS, `r7 = r7 + r7 + r7 + r7 + r7 - r7 - r7 - r7
     - r7;' and `ang = ang + ang - ang;'.  See change (O).  They are the
     catalogue's combine-folded free reference (matching-levers.md:11434 and
     :31950), they are mathematically the identity, and they emit NOTHING --
     1817 words with them and 1817 without, and with the chains at sizes that
     do not move the allocator the body is BYTE-IDENTICAL to the 16-row park.
     They exist for ONE reason, which is arithmetic rather than swept: the
     allocation order of case 0x2A's four contenders must be r7, ang, then
     i2A and the pointer, and the only other way to add references to r7 --
     loop notes -- adds them to the D_80082E80 pointer in the same statement
     and the pointer wins at every nesting level.  THE SIZES ARE MINIMAL:
     with one term fewer on either chain the ordering breaks (r7 at 13 refs
     is 4642 against ang's 4931, and ang with no chain is 3802 against i2A's
     4426).  They are the one shape in this body that no plausible original
     wrote; everything else here is ordinary C.
     NEITHER CHAIN CAN OVERFLOW, which matters because signed overflow would
     be undefined and the gate-2 reading has to hold: the r7 chain sits AFTER
     the guard `if ((u16)(r7 - 2) >= 7) break;', so r7 is in [2, 8] there and
     the largest intermediate is 5 * 8 = 40; `ang' is `*(u16 *)(c + 0x88)',
     so it is in [0, 0xFFFF] and the largest intermediate is 3 * 0xFFFF.
     Both are s32.  And the chains READ ONLY a variable that is definitely
     assigned on the path that reaches them -- `r7' two statements above and
     `ang' one -- so neither is the devsweep class of find this park already
     rejected once.
   * NO OTHER DEVICES.  No `do { } while (0)', no duplicated arm, no
     `__asm__', no redundant store and no `goto'.  THREE full cancelled-pair
     sweeps said none helps on the bodies they were run on -- and note that
     none of them could ever have found change (M), because refsweep only
     inserts pairs on names that ALREADY EXIST and `u2' had to be created
     first.  Confirmed
     again this session: `r5++; r5--;' at the one place a cancelled pair
     would have been worth 3 rows is byte-identical, because cse2 folds the
     pair before flow ever counts it.  And the one devsweep find that DOES
     beat this body -- 23 rows -- is undefined behaviour twice over and its
     safe part is a 4-row regression; see "DEVSWEEP'S 23-ROW FIND" above.

   ===========================================================================
   STILL TRUE FROM THE EARLIER HEADERS, DO NOT RE-DERIVE

   * The empty switch cases must NOT be written out; gcc sizes the table
     from the lowest and highest case labels only (stmt.c:5052-5071, and the
     gap-filling loop at stmt.c:5304-5307).
   * The six shared tails are NOT a goto ladder: .L800AF89C/.L800AF8A0/
     .L800AF8A4 are the delay-slot scheduler stealing one or two
     instructions from the head of the code after the switch.
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
   * `*(s16 *)(c + 0x2A) = sp1E;' BEFORE `sp20 = r;' in cases 0 and 1.
   * `D_800E2970_s[*(s8 *)(b + 0x26)].unk_0C & 2' is csrc/ovl_dungeon.c's
     own spelling.
   * func_800A0E6C IS CALLED WITHOUT A PROTOTYPE, as the unit's later call
     site does.  Adding one would change that landed call.
   * CASE 0x2A'S LOOP COUNTER MUST BE ITS OWN VARIABLE: `i2A' must stay.
   * declaration-order search, 80 random reorderings: no effect at all.
   =========================================================================== */

/* THE FOURTEEN PROTOTYPES THE THIRD CARVE TOOK AWAY.  Every one of these
   callees is DEFINED IN AN EARLIER UNIT now, so this unit sees an implicit
   `int f()' where the single-unit compile saw the definition line.  They are
   copied VERBATIM from those definition lines, i.e. they say exactly what
   csrc/ovl_dungeon.c said before commit fb2d885.  ONLY func_800A45D8 IS
   LOAD-BEARING (it is worth 4 rows); the other thirteen are byte-identical
   singly and are kept so the park reproduces the pre-carve compile exactly,
   which is what makes every sweep result recorded below transfer.  None of
   the fourteen is referenced anywhere else in csrc/ovl_dungeon_800A4B88.c
   (0 textual references, and none is in csrc/ovl_dungeon_decls.h), so
   declaring them cannot touch a landed function.  carve_dungeon.py should
   generate them -- see "THE CARVE REGRESSION" at the top. */
s32 func_8009CD58(u8 *a, s32 b, s16 c);
s16 func_800A0134(u8 *a, u8 *b);
u8 *func_800A02AC(u8 *p, u16 x, u16 y);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
s32 func_800A2CAC(u8 *p);
s32 func_800A3518(u8 *p);
s16 func_800A3544(u8 *p, u8 k);
s32 func_800A365C(u8 *a, u8 *b);
s32 func_800A36B4(u8 *a, u8 *b);
s32 func_800A384C(u8 *a, u8 *b, u8 *c, s32 s);
u8 *func_800A3D18(u8 *a, u8 *b, s16 c);
s32 func_800A404C(void);
s16 func_800A45D8(u16 x, u16 y, s32 c);

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
    s32 r9;
    s32 t1;
    s32 t3;
    s32 t4;
    s32 u2;
    s32 n;
    s32 nn;
    s16 aa;
    s32 ang;
    s32 dx;
    s32 dy;
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
    }
    else
    {
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
                                    if (c != *(u8 **)(D_800814A8 + j * 4 + 0xAC))
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
                u2 = func_800A384C(c, q2, (u8 *)&sp1E, 0);
                sp20 = u2;
                *(s16 *)(c + 0x2A) = sp1E;
                r2 = u2;
                u2++;
                u2--;
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
                                    if (c != *(u8 **)(D_800814A8 + j * 4 + 0xAC))
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
                u2 = func_800A384C(c, q4, (u8 *)&sp1E, 0);
                sp20 = u2;
                *(s16 *)(c + 0x2A) = sp1E;
                r3 = u2;
                u2++;
                u2--;
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
                                    if (c != *(u8 **)(D_800814A8 + j * 4 + 0xAC))
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
                    if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 && (s16)func_800A404C() == 0 &&
                        *(u8 *)(D_800814A8 + 0x9A) == 0x11)
                    {
                        o = *(u8 **)(D_800814A8 + 0x60);
                        if (o != NULL && (*(s32 *)(o + 0x14) & 0x2000) == 0)
                        {
                            u2 = func_800A40ACw(c, func_8009CD58(o, 7, 0));
                            sp20 = u2;
                            r4 = u2;
                            u2++;
                            u2--;
                            if ((s16)r4 >= 0)
                            {
                                *(u16 *)(c + 0x46) = r4 | 0x8000;
                                *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25],
                                                                   D_80082E80[0x24],
                                                                   D_80082E80[0x25], (u16 *)&sp20);
                                return 3;
                            }
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
                    t3 = (s16)func_800A0134(c, g);
                    if (t3 < 0)
                    {
                        t3 = -t3;
                    }
                    if (t3 < 0x21 && (s16)func_800A41F0(g) != 0)
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
            }
            else if ((s16)func_8009FD40(b, D_80082E80) == 1)
            {
                *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                                   D_80082E80[0x25], (u16 *)(c + 0x98)) +
                                     0x800;
            }
            *(u16 *)(c + 0x46) = 0x800B;
            return 2;

        case 0x24:
            ((u16 *)c)[0x4C] &= 0x7FFF;
            if (c[0x49] != 0)
            {
                break;
            }
            r9 = func_800A6E8C(b, 0x12, &sp18, &sp1A);
            if ((s16)r9 == 0)
            {
                break;
            }
            ((s16 *)c)[0x15] = func_800A0818(b[0x24], b[0x25], sp18, sp1A, (u16 *)(c + 0x98));
            ((u16 *)c)[0x54] = sp18;
            ((u16 *)c)[0x55] = sp1A;
            ((u16 *)c)[0x4C] |= 0x8000;
            dx = b[0x24];
            dx = dx - sp18;
            dx = (dx >= 0) ? dx : -dx;
            e = dx;
            sp18 = dx;
            t4 = b[0x25] - sp1A;
            dy = (t4 >= 0) ? t4 : -t4;
            sp1A = dy;
            r9 = sp18 < sp1A;
            if (r9)
            {
                e = sp1A;
            }
            if ((s16)e == 1)
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
            if ((*(s32 *)(c + 0x1C) & 0x20000) != 0)
            {
                *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                                   D_80082E80[0x25], (u16 *)(c + 0x98));
            }
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
            r7 = r7 + r7 + r7 + r7 + r7 - r7 - r7 - r7 - r7;
            ang = *(u16 *)(c + 0x88);
            ang = ang + ang - ang;
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
                    t3 = (s16)func_800A0134(c, g);
                    if (t3 < 0)
                    {
                        t3 = -t3;
                    }
                    if (t3 < 0x21 && (s16)func_800A41F0(g) != 0 &&
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
            if (c[0xAC] < 2 && g == NULL)
            {
                g = (u8 *)((s16)func_800A70E4(sp18, sp1A, *(s16 *)(c + 0x88)) + 1);
            }
            if (g != NULL)
            {
                *(u16 *)(c + 0x46) = 0x8009;
                return 5;
            }
        }
        if (*(s8 *)(b + 0x26) >= 0)
        {
            return 2;
        }
    }
    return ret << 16 >> 16;
}
