/* PARK CLOSED: func_800C2824 LANDS -- 278/278 words, 0 aligned differences,
   0 differing under ovlverify, confirmed in a real unit copy and with every
   other definition in csrc/ovl_dungeon_800C2824.c still at 0 differing.
   Opened at 285/84.  Sessions: 84 -> 73 -> 43 -> 27 -> 10 -> 0.

   TWO CHANGES CLOSED IT, and BOTH of them are a verdict this header had
   recorded as CLOSED IN THE COMPILER SOURCE.  Neither compiler-source
   argument was wrong.  Both were answers to the wrong question, and the
   superseded header is kept below in full because the shape of that mistake
   is the most useful thing in this file.

   ====================================== CHANGE 1 -- CASE 16, 10 -> 6 ROWS
                                          THEN 6 -> 5

   Case 16 was two residue clusters at once, and the header called them
   "MUTUALLY EXCLUSIVE in the source": the target issues both loads BEFORE
   the `D_800DF55C' store (which needs the store written last) and also has
   an `addu $t0, $v0, $zero' copy (which the header said needs the read-back,
   and the read-back puts the store FIRST).  Both halves are reachable.

   1a. THE STORE-LAST ORDER COSTS NOTHING ONCE THE 0x88 LOAD IS HOISTED INTO
       A 32-BIT LOCAL READ THROUGH `*(s16 *)'.  The header had this as "form
       B2: 12 rows but 280 words, i.e. two words long, and worse than form A
       on both counts".  THE TWO WORDS WERE THE HOISTED LOCAL'S TYPE, NOT THE
       SHAPE.  On the 10-row body:
           `s16 h16 = *(s16 *)(e3 + 0x88);'  ...... 12 rows at 280 words
           `s32 h32 = *(s16 *)(e3 + 0x88);'  ......  6 rows at 278 words
           the existing multi-block `a' as the target   6 rows at 278 words
       PROMOTE_MODE (config/mips/mips.h) promotes an HImode local to SImode,
       so a 32-bit local loads with `lh' and needs no conversion at the call,
       while an `s16' local is HImode in the tree, loads with `lhu' and pays
       a `sll'/`sra' pair to reach func_800BBA40's `s16' parameter.  The
       FORM was right in session 5 and the TYPE threw it away.
       `a' is case 2's and case 17's 0x3000/0x300 flag and is already
       multi-block, so reusing it needs no new declaration and measures the
       same as a fresh `s32'.  Nothing else in the function reads `a' across
       an arm boundary, so this is safe.

   1b. THE COPY IS A PRE-COPY USE OF AN ANONYMOUS TEMP, NOT THE READ-BACK.
       The copy survives only when the pseudo that the `lw' writes is NOT a
       user variable and has a use BEFORE the copy insn.  Both conditions are
       mechanical:
         * cse's `insert' makes a REG_USERVAR_P pseudo the `qty_first_reg' of
           its quantity, so `canon_reg' rewrites later uses of the ANONYMOUS
           pseudo to the named one.  That is why `n3 = *(u8 **)p; e3 = n3;'
           collapses -- both are user variables, `n3' is first, and `e3'
           disappears (measured: every two-named-local shape is 6 rows, i.e.
           exactly the no-copy body, and the two that store from `n3' instead
           are 15).
         * combine deletes the copy whenever the temp has exactly one use, so
           a use before the copy is what keeps it.  In form A that use is the
           store, which is why form A's store is first.
       A use before the copy that is NOT the store:
           r = *(u8 **)(*(u8 **)p - 0x14);   <-- the temp's first use
           e3 = *(u8 **)p;                   <-- cse makes this the copy
           a = *(s16 *)(e3 + 0x88);
           D_800DF55C = e3;
       5 rows at 279 words, and CASE 16 IS THEN PERFECT -- the whole residue
       is case 2's one word.  The same thing written with a named temp whose
       first use precedes the copy is identical:
           n3 = *(u8 **)p;
           r = *(u8 **)(n3 - 0x14);
           e3 = n3;                          <-- the surviving `move'
           a = *(s16 *)(e3 + 0x88);
           D_800DF55C = e3;
       also 5 at 279, and that is the live body because it reads `*(u8 **)p'
       once, as the target's single `lw $v0, 0x0($s1)' does.
       THE REGISTERS FALL OUT OF IT: `n3' is block-local so `find_free_reg'
       gives it $v0, `e3' is multi-block (it carries case 3's D_800E3D7C
       value) so global_alloc gives it $t0, and `combine_regs'
       (local-alloc.c:1791) refuses to tie them because `reg_qty[e3] < 0'.
       That is the brief's lever 1 and it is what makes the `move' an insn
       instead of a coalesced nothing.
       MEASURED, same session, same body: the 0x88 load hoisted but the temp's
       first use being the 0x88 load instead of the `-0x14' load is 7 at 279
       (right shape, wrong insn before the copy), and three shapes that put
       the copy before any use are 6, 15 and 18.

   ====================================== CHANGE 2 -- CASE 2, THE LAST WORD
                                          AND THE LAST FOUR ROWS

   THE HEADER'S "CASE 2's WORD IS UNREACHABLE -- CONFIRMED FROM A SECOND
   DIRECTION" IS A CORRECT PROOF OF SOMETHING ELSE.  There is genuinely no
   way to make this compiler fold a symbol's `%lo' into a store whose address
   carries a REGISTER index -- LEGITIMIZE_ADDRESS has no `(plus SYMBOL_REF
   REG)' arm, GO_IF_LEGITIMATE_ADDRESS's `(plus REG CONSTANT_ADDRESS)' arm is
   gated on `! mips_split_addresses', and `find_split_point's MEM arm
   (combine.c:2493) takes only a bare CONST or SYMBOL_REF.  All of that was
   re-read this session and all of it holds.

   WHAT IT MEANS IS THAT THE TARGET'S STORE IS NOT A SYMBOL REFERENCE.
       800C2A10 lui  $a0, %hi(D_80010A93)
       800C2A28 addu $v0, $v0, $a0
       800C2A30 sb   $zero, %lo(D_80010A93)($v0)
   is a CONSTANT BASE 0x80010000 -- whose low half is zero, so it is a bare
   `lui' -- with 0xA93 written inside the INDEX EXPRESSION, where it becomes
   the store's displacement.  splat prints that displacement as
   `%lo(D_80010A93)' because 0x80010A93 is in its symbol list, which is the
   whole reason the listing looks symbolic.  So:

       if (j < 0x40)
       {
           u8 *bb = (u8 *)0x80010000;

           *(u8 *)(bb + j * 0x54 + 0xA93) = 0;
           *(u32 *)(0x80010980 + j * 4) = 0;
       }

   278/278 words, 0 differing.  `lui $a0, 0x8001' and `sb $zero, 0xA93($v0)'
   are byte-identical to the target's relocated pair, confirmed with
   `ovlverify --raw' word for word (words 123 and 131).

   THIS IS docs/matching-levers.md's "A %lo IN THE DISPLACEMENT WITH A
   REGISTER INDEX MEANS A CONSTANT BASE" (func_8004A7D8, func_8004A6C0),
   WHOSE WORKED EXAMPLE IS THIS VERY ADDRESS REGION -- `u8 *b = (u8 *)
   0x80010000;' with `+ i * 4 + 0x3564'.  Twenty-three symbol spellings over
   two sessions all spent the `addiu %lo' because every one of them was a
   symbol spelling.  THE CATALOGUE ENTRY NAMES FIVE OF THEM AS NEGATIVES
   ALREADY -- "incomplete array, sized array, struct array with an `__asm__'
   alias, an element pointer, and a one-set pointer local".  The park re-ran
   that same closed set and read the compiler source to explain why it could
   not work, instead of reading the entry that says what to write instead.

   THE CONSTANT MUST STAY SPLIT IN THE SOURCE.  Written as one integer the C
   front end folds it and the base becomes a full 32-bit materialisation:
       `*(u8 *)(0x80010000 + (j * 0x54 + 0xA93)) = 0;'  .... 8 rows at 279
       `*(u8 *)(j * 0x54 + 0x80010A93) = 0;'  ............... 8 rows at 279
       `*(u8 *)(0x80010A93 + j * 0x54) = 0;'  ............... 8 rows at 279
       `((u8 *)0x80010A93)[j * 0x54] = 0;'  ................. 8 rows at 279
   Only a POINTER VARIABLE holding 0x80010000 keeps `lui' alone.  All four of
   these spellings reach 0 and are interchangeable:
       base declared inside the `if', pointer arithmetic   <-- live
       base declared inside the `if', `bb[j * 0x54 + 0xA93]'
       base a function-scope local assigned in the `if'
       the same with the subscript spelling
   Declaring the base AFTER the 0x80010980 store, so its `lui' comes second,
   is 17 rows -- the order of the two bases is load-bearing.
   `extern u8 D_80010A93;' IS NO LONGER REFERENCED and has been dropped from
   the declaration block.

   ====================================== THE SCREEN THAT WOULD HAVE CAUGHT
                                          BOTH OF THESE

   Session 5 wrote one down and it is still the right one -- "for each residue
   row, read the TARGET's register and ask which allocator produced it".  Add
   two more, because each cost this park a session:

   * A HOISTED LOAD'S PENALTY MAY BE ITS LOCAL'S DECLARED TYPE RATHER THAN
     ITS POSITION.  Price a hoist at EVERY width before concluding the hoist
     itself is long.  The park's `widthsweep' cannot see this because the
     local did not exist until the hoist was written, and the shape was
     rejected on the one type it was first written with.
   * A `%hi'/`%lo' PAIR IN A LISTING IS A CLAIM ABOUT splat's SYMBOL LIST,
     NOT ABOUT THE SOURCE.  Before proving that a relocation is unreachable,
     check whether the target's bytes are a relocation at all.  With a
     register index in the address they are not, and the catalogue has said
     so since 2026-11-11.
   * AND THE GENERAL ONE: A COMPILER-SOURCE CLOSURE BOUNDS A MECHANISM, NOT A
     RESIDUE.  Both of this park's closures were sound proofs that a
     particular RTL shape is unreachable.  Neither was a proof that the
     target has that RTL shape.  A closure needs the second half before it
     licenses parking.

   ====================================== WHAT THE SUPERSEDED HEADER IS STILL
                                          GOOD FOR

   Everything measured in it is still a measurement, and the five exhaustive
   spaces it closed on the 10-row body are what made the last two steps
   cheap -- the 203 pointer-site partitions, the 552 declaration moves, the
   24 stack-pair store orders, the six `default:' positions and the 16-way
   `w' power set are all still closed, and the landed body keeps every
   structural claim it re-priced.  The four sweeps were clean on the 10-row
   body and neither change above is one a sweep varies: change 1 is a
   statement-order plus a type and change 2 is a new local with constant
   arithmetic moved across it.
   The uninitialised-read rejections stand as well.  The landed body reads
   nothing undefined -- `n3', `e3', `r' and `a' are each assigned above their
   use inside case 16 -- so the two rows the permuter could only reach
   through a wild load are reached legally here and the remaining four came
   from case 2.

   DECLARATIONS
   void func_800C27F0();
   void func_800C0180();
   void func_800CEF54();
   extern u8 D_800E1640[];
   END DECLARATIONS
   (func_800C27F0 is in the HEAD unit and func_800CEF54 in
   ovl_dungeon_800CABA8, so both are cross-unit references now and need a
   declaration where a single-unit build needed none.  `extern u8
   D_80010A93;' was in this list until this session and is now unused.
   Everything else -- D_800DF45C, D_800DF4A4, D_800DF55C, D_800DF560,
   D_800E3D7C, D_80082E80, D_800814A0 -- is already in
   csrc/ovl_dungeon_decls.h.)
   The park declares `void func_800C27F0();' and `void func_800C0180();'
   where the definitions are `s32 f(void)'.  THOSE SPELLINGS ARE WRONG AND
   COST NOTHING, and adding the correct ones alongside is a hard
   `conflicting types' error, so they are kept as they are.

   ====================================== SUPERSEDED HEADER, SESSIONS 1-5,
                                          KEPT VERBATIM

   TWO OF ITS VERDICTS ARE NOW FALSE and both are named above -- "CASE 2's
   WORD IS UNREACHABLE" and case 16's "MUTUALLY EXCLUSIVE".  Every number in
   it is still a real measurement of the body it names.

   |  PARK: func_800C2824 -- 278/278 words, 10 ALIGNED ROWS.
   | Opened at 285/84.  Sessions: 84 -> 73 -> 43 -> 27 -> 10.
   |
   | Measured with
   |     python3 tools/decomp/parkresidue.py func_800C2824
   | and, for every sweep and grid below, on an onefunc.py standalone that
   | reproduces the unit's numbers byte for byte (0.09 s a candidate; confirmed
   | against a real unit copy at 73, 49, 43, 35, 32, 25, 13 and 10 rows).
   | EVERY NUMBER IN THIS HEADER IS AN ALIGNDIFF ROW COUNT, AND THE WORD COUNT
   | IS GIVEN WHENEVER IT IS NOT 278.
   |
   | **DO NOT PANIC AT `ovlverify'.  IT REPORTS 81 DIFFERING ON THIS BODY** --
   | that is the word-against-word count the brief warns about, inflated by the
   | case-2 word shifting everything after it by one and by case 16's
   | transposition.  The honest aligned figure is 10.  (It said 145 on the
   | 32-row body, so the ratio moves too; use `aligndiff.py', always.)
   |
   | ====================================== IF YOU HAVE ONE HOUR, SPEND IT HERE
   |
   | 1. DO NOT re-run the four sweeps (clean on this body), the 203-partition
   |    space, declaration order, the `w' set, the array lengths, the store
   |    order, the `default:' positions, or any case-2 address spelling.  All
   |    closed below with numbers.
   | 2. THE ONLY TWO THINGS LEFT ARE (A) case 2's four-word symbol address, 4
   |    rows, which is closed in the compiler source two different ways, and
   |    (B) case 16's store/load transposition, 6 rows, which is closed in
   |    `memrefs_conflict_p' and whose 2 cheapest rows are reachable only by
   |    an undefined read.
   | 3. SO THE HONEST ANSWER IS THAT THIS PARK IS PROBABLY FINISHED AT 10
   |    unless someone finds the pass that folds `(plus (lo_sum (high s) s) i)'
   |    into `(lo_sum (plus (high s) i) s)'.  If you want to try, read "CASE
   |    2's WORD IS UNREACHABLE" first -- it names every route already ruled
   |    out and the exact arm in `find_split_point' that would have to fire.
   | 4. If you only want the better DIAGNOSIS rather than the better score,
   |    work on form A (9 rows at 279 words, three lines from the live body):
   |    its residue is case 2's word and nothing else but a 4-row transposition.
   |
   | ====================================== THE WHOLE RESIDUE, AND WHY THIS
   |                                        PARK IS NOW A ONE-WORD PROBLEM
   |
   | 10 rows, and they are TWO causes only:
   |
   |   R  114  114 800C29EC  beqz v0,218      beqz $v0, .L800C2A38
   |   R  120  120 800C2A04  beqz v0,218      beqz $v0, .L800C2A38
   |   +  124                addiu a0,a0,0
   |   R  182  181 800C2AF8  beqz v0,438      beqz $v0, .L800C2C5C
   |   -       197 800C2B38                   lw $v0, 0x0($s1)
   |   X  199  199 800C2B40  lw t0,0(s1)      addu $t0, $v0, $zero
   |   X       200 800C2B44                   lw $a1, -0x14($t0)
   |   X       201 800C2B48                   lh $a2, 0x88($t0)
   |   +  202                lw a1,-20(t0)
   |   +  203                lh a2,136(t0)
   |
   | (A) CASE 2's FOUR-WORD SYMBOL ADDRESS, built 124, plus the three branch
   |     drifts it causes (114, 120, 182).  4 rows.  **CLOSED -- see "CASE 2's
   |     WORD IS UNREACHABLE" below, where this session confirmed it from a
   |     second direction in the compiler source and measured ten more
   |     spellings.**
   | (B) CASE 16's STORE/LOAD TRANSPOSITION AND ITS MISSING COPY, 6 rows.
   |
   | AND THE TWO ARE LOCKED TOGETHER BY ONE WORD.  There are exactly two
   | frontier bodies, and they differ only in case 16's form:
   |
   |     case 16 form C (live park body) ....... 10 rows at 278 words
   |       e3 = *(u8 **)p;  D_800DF55C = e3;  r = *(u8 **)(e3 - 0x14);
   |     case 16 form A (the read-back) ........  9 rows at 279 words
   |       (both confirmed in a real unit copy, not just the standalone)
   |       D_800DF55C = *(u8 **)p;  e3 = D_800DF55C;  r = *(u8 **)(e3 - 0x14);
   |
   | **FORM A IS THE STRUCTURALLY CORRECT ONE AND FORM C IS NOT.**  Form A
   | keeps the `addu $t0, $v0, $zero' copy at 800C2B40 that the target has and
   | form C deletes; form C is word-exact ONLY because losing that copy cancels
   | case 2's extra word exactly.  The park body is form C because the
   | objective is word-exactness first (`parkresidue' ranks on
   | `(abs(ww - tw), rows)'), but **form A at 9/279 is the body that says what
   | is actually left**, and its residue is nothing but (A) and a four-row pure
   | transposition in case 16:
   |
   |     built 279 words, target 278 words, 9 aligned differences
   |     R   11   11 800C2850  beqz v0,43c      beqz $v0, .L800C2C5C
   |     R  106  106 800C29CC  bgtz v0,43c      bgtz $v0, .L800C2C5C
   |     R  114  114 800C29EC  beqz v0,218      beqz $v0, .L800C2A38
   |     R  120  120 800C2A04  beqz v0,218      beqz $v0, .L800C2A38
   |     +  124                addiu a0,a0,0
   |     -       200 800C2B44                   lw $a1, -0x14($t0)
   |     -       201 800C2B48                   lh $a2, 0x88($t0)
   |     +  203                lw a1,-20(t0)
   |     +  204                lh a2,136(t0)
   |
   | So: if case 2's word is ever solved, form A becomes 278 words and rows 11,
   | 106, 114, 120 and 124 all go with it -- **4 rows, all of them the case-16
   | transposition** -- while form C would become 277.  Whoever takes this next
   | should hold both bodies in mind: the live one is the better score, the
   | other one is the better diagnosis.  Form A is THREE LINES away from the
   | live body -- replace case 16's first three statements with
   |     D_800DF55C = *(u8 **)p;
   |     e3 = D_800DF55C;
   |     r = *(u8 **)(e3 - 0x14);
   | and nothing else changes.
   |
   | ====================================== 2026-11-22 (session 5): 27 -> 10
   |                                        IN THREE STEPS, ALL BY HAND
   |
   | Nothing in this session came from a sweep or the permuter.  All four
   | sweeps were clean on 27, clean again on 13, and clean again on 10 (see
   | "SWEEPS" below) -- the three changes are a REGISTER-CLASS MERGE, a
   | CONTROL-FLOW change no sweep varies, and a REGISTER-CLASS SPLIT.
   |
   | --------------------------------------- STEP 1, 27 -> 25: `r16' DELETED,
   |                                         MERGED INTO THE SHARED `r'
   |
   | Case 16's `u8 *r16;' is gone; case 16 now uses `r', the local case 2 and
   | case 17 already share:
   |
   |     case 16:  ...  r = *(u8 **)(e3 - 0x14);
   |                    if (func_800BBA40(r[0x24], r[0x25], ...
   |
   | 25 rows at 278/278, confirmed in a real unit copy.
   |
   | WHY.  `r16' was block-local, and `find_free_reg' takes the LOWEST free
   | register, so it got $v0 (built 200 `lw v0,-20(t0)' / 202 `lbu a0,36(v0)' /
   | 203 `lbu a1,37(v0)').  The target has it in $a1 -- 800C2B44
   | `lw $a1,-0x14($t0)' / 800C2B54 `lbu $a0,0x24($a1)' / 800C2B58
   | `lbu $a1,0x25($a1)' -- which is exactly what case 2 (800C2BE0) and case 17
   | already get, because their shared `r' is referenced in two blocks and is
   | therefore a GLOBAL allocno (flow.c:2561) that `find_reg' prices over its
   | whole live range.  Adding case 16 to that allocno gives case 16 the same
   | register and the three rows go.
   |
   | **THE PARK HAD THIS RECORDED AS A NEGATIVE AND THE NEGATIVE WAS STALE.**
   | "`r' is split for case 0 (`r0') and for case 16 (`r16') but SHARED between
   | case 2 and case 17: case 16 alone 43, case 2 alone 51, case 17 alone 51,
   | case 16 + case 17 57" was measured ON THE 44-ROW BODY, four sessions ago.
   | This is the brief's rule biting for the seventh time on this park: a
   | partition negative is scoped to the body it was measured on.
   |
   | EQUAL-SCORING ALTERNATIVES, ALL 25/278 -- so the lever is "make case 16's
   | `r' a multi-block allocno", NOT the particular partner:
   |     `r16' -> `r' ............................................. 25
   |     `r16' -> `ba' (case 3's D_80082E80 local) ................. 25
   |     `r16' -> `r' plus `r++; r--;' after the `-0x14' load ...... 25
   |     `r16' -> `r' plus case 16's two `lbu' args hoisted ........ 25
   | LOSERS:
   |     `r16' -> `q17' 28, -> `q17b' 28, -> `q2' 28, -> `q0' 29,
   |     -> `r0' 33 at 279 words;
   |     the shared `r' SPLIT into `r2'/`r17' ..................... 41.
   |
   | --------------------------------------- STEP 2, 25 -> 13: THE SHARED TAIL
   |                                         WRITTEN TWICE WITH ITS OWN `f',
   |                                         AND THE D_800DF560 READ-MODIFY-
   |                                         WRITE SPLIT
   |
   | TWO changes, and **NEITHER IS WORD-EXACT ON ITS OWN -- only the
   | composition is**, which is why three sessions of one-change-at-a-time
   | hand work never saw either of them:
   |     the duplicated tail with own `f' + D2 off, alone .. 20 rows at 280
   |     the DF560 read-modify-write split, alone ........... 22 rows at 277
   |     both (case 16 still form A) ........................ 12 rows at 279
   |     both, plus case 16 form A -> form C ................ 13 rows at 278
   | A tool that rejects any candidate off the target word count -- which is
   | what all four sweeps and `measure_finds' do, correctly -- cannot reach
   | this pair at all.  It took pricing the full 9 x 9 grid.
   |
   | 2a. `goto shared' IS GONE.  The tail
   |         *(u16 *)(p - 0x2) |= 0x8000;  D_800814A0 = f;
   |     is written TWICE -- once at the end of case 3's `if', once at the end
   |     of case 17's -- and **each copy has its OWN `f': `u32 f3;' in case 3
   |     and `u32 f17;' in case 17.**  jump2 cross-jumps them and the join's
   |     rematerialised `lui %hi(D_800814A0)' disappears.
   |
   |     THIS IS docs/matching-levers.md's "A SHARED TAIL IN THE TARGET MAY BE
   |     TWO BLOCKS CROSS-JUMPED AFTER RELOAD -- WRITE IT TWICE, WITH ITS OWN
   |     LOCALS" (func_800B45E0, 2026-11-21), and **the park had measured the
   |     duplicated tail FOUR TIMES across THREE SESSIONS and recorded it as a
   |     negative every time, because every one of those measurements shared
   |     one `f' between the copies** -- which, as that entry says in so many
   |     words, "is the same position as the `goto'": a name used in both
   |     copies is multi-block and therefore global again.  On the 25-row body:
   |         duplicated tail, OWN `f' per copy, D2 off ..... 22 at 280
   |         duplicated tail, SHARED `f',      D2 off ..... 34 at 285
   |         duplicated tail, OWN `f' per copy, D2 on ..... 27 at 277
   |         duplicated tail, SHARED `f',      D2 on ..... 27 at 277
   |     Re-measured on the 10-row body, one shared `f' is 25 at 284.
   |     **The lever's failure mode is recorded in this file as a negative for
   |     the lever itself.  That is the transferable lesson.**
   |
   | 2b. DEVICE D2 IS GONE, because 2a pays for it.  Case 3's `if' body is now
   |     the plain order the target plainly has:
   |         n2 = *(u8 **)p;
   |         *(u16 *)(n2 - 0x2) |= 0x8000;
   |         f3 = D_800814A0 | 0x8000;
   |         D_800814A0 = f3;
   |         *(u16 *)(p - 0x2) |= 0x8000;
   |         D_800814A0 = f3;
   |     which is instruction for instruction 800C2B00-800C2B28.  Every
   |     previous session had D2 ON (`f = ...' written ABOVE the `*(u8 **)p'
   |     store) because the three words D2-off cost in the tail -- the join's
   |     `lui', a load-delay `nop', and `f' landing in $a2 so the tails could
   |     not cross-jump -- were worth more than case 3's structure.  2a and 2c
   |     remove all three.  Re-priced on the 10-row body: D2 back ON is 20 at
   |     275 words, and `goto shared' back is 23 at 280.
   |
   | 2c. THE D_800DF560 READ-MODIFY-WRITE IS SPLIT SO ITS LOAD COMES FIRST.
   |     Case 17's tail was `*(u16 *)(D_800DF560 + 0x1E) |= 0x8000;' written
   |     AFTER the `f' read-modify-write.  It is now
   |
   |         hv = *(u16 *)(D_800DF560 + 0x1E);     <-- the LOAD, before `f'
   |         f17 = D_800814A0 | 0x8000;
   |         D_800814A0 = f17;
   |         *(u16 *)(D_800DF560 + 0x1E) = hv | 0x8000;    <-- the STORE, after
   |
   |     with `u16 hv;' among the locals.  On the `goto shared' 25-row body
   |     alone this is 25 -> 22 at 277 words; composed with 2a/2b it is worth
   |     8 more rows.
   |
   |     THIS IS TWO CATALOGUE ENTRIES AT ONCE: "A READ-MODIFY-WRITE OF A
   |     SCALAR GLOBAL BLOCKS ITS OWN LOAD DELAY" (func_800A504C, 2026-11-15 --
   |     and on D_800814A0, the very same global) and "READING A GLOBAL INTO A
   |     LOCAL BEFORE AN UNRELATED READ-MODIFY-WRITE LETS sched1 INTERLEAVE"
   |     (func_800B348C).  It produces the target's order exactly: 800C2C2C
   |     `lw $a1, %lo(D_800DF560)($v0)' first, 800C2C30
   |     `lw $v1, %lo(D_800814A0)($a0)' second, 800C2C34 `lhu $v0, 0x1E($a1)'
   |     two insns behind its base so it needs no stall, and
   |     `ori $v1, $v1, 0x8000' filling the `lhu's delay slot.
   |
   |     **IT IS THE SPLIT, NOT A HOIST.**  Priced separately on the 25-row
   |     body:
   |         split the RMW (load first, store last) ............ 22 at 277
   |         `df = D_800DF560;' pointer local as well .......... 22 at 277
   |               (IDENTICAL in every cell of the 81-body grid and again on
   |                the 10-row body -- the pointer local buys NOTHING)
   |         `df = D_800DF560;' first but the RMW NOT split .... 30 at 278
   |         the whole DF560 statement moved first, not split .. 28 at 278
   |         the whole DF560 RMW moved BEFORE the `f' chain .... 35 at 281
   |     On the 10-row body, un-splitting the RMW is 18 at 279.
   |     Variations that change nothing (all 10 on the 10-row body): `hv'
   |     declared `u32' or `s32'; `hv |= 0x8000;' as its own statement; the
   |     `hv' load moved after `f17 ='.
   |
   |     **AND A CORRECTION TO THE PARK.**  Session 4 wrote that the two `lw's
   |     here were "swapped in both order and register against the target's
   |     800C2C2C/800C2C30".  THE REGISTERS WERE ALREADY RIGHT -- $a1 for the
   |     D_800DF560 value and $v1 for `f', in both streams.  Only the ORDER was
   |     wrong, which is why a statement-order lever reached it and why the
   |     "sched1 decision source order has no purchase on" verdict was wrong.
   |
   | 2d. CASE 16 THEN WANTS FORM C.  Composed with 2a-2c, the 9 case-16 forms
   |     were crossed with 9 tail shapes -- 81 bodies, every cell measured.
   |     See "THE 81-BODY GRID" below for all of it.  The frontier is form A at
   |     12/279 and form C at 13/278 on that body, which became 9/279 and
   |     10/278 after step 3.
   |
   | --------------------------------------- STEP 3, 13 -> 10: CASE 3's
   |                                         `*(u8 **)p' SPLIT OUT OF `e3'
   |
   | `e3' had THREE roles after session 4's permuter merge.  Name them:
   |     R1  case 3's `*(u8 **)&D_800E3D7C' value, with the cancelled pair
   |     R2  case 3's `*(u8 **)p' inside the `if' body
   |     R3  case 16's `*(u8 **)p'
   | Session 4's find merged all three.  **R2 wants to be ON ITS OWN**:
   |
   |     case 3:  ... n2 = *(u8 **)p;  *(u16 *)(n2 - 0x2) |= 0x8000;  ...
   |
   | with `u8 *n2;' among the locals.  10 rows at 278/278, confirmed in a real
   | unit copy, and it kills case 3's last three rows (built 184/186/189
   | `lw t0,0(s1)' / `lhu v0,-2(t0)' / `sh v0,-2(t0)' against 800C2B00/B08/B14
   | `lw $v1,0x0($s1)' / `lhu $v0,-0x2($v1)' / `sh $v0,-0x2($v1)').
   |
   | WHY.  Merged, `e3' is multi-block, so global_alloc gives it ONE register
   | for the whole function -- $t0.  That is RIGHT for case 16 (the target has
   | $t0 at 800C2B40) and WRONG for case 3 (the target has $v1, a lowest-free
   | block-local choice).  **The target therefore has TWO variables here, not
   | one**, and R1+R3 is the pair that must stay merged.
   |
   | THE WHOLE PARTITION, measured on the 13-row body and re-measured on the
   | 10-row one.  R2's name (R1+R3 stay merged in `e3'):
   |     its own local `n2' ....... 10   <-- live
   |     `q2' ..................... 10
   |     `q17b' ................... 10
   |     `ba' ..................... 13
   |     `r' ...................... 13
   |     `e3' (i.e. the 13-row body) 13
   |     `q0' ..................... 15
   |     `q17' .................... 16
   | R3's name (R1+R2 merged in `e3', i.e. the 13-row body's partition):
   |     `q0' 19, `ba' 19, `q17' 22, `q17b' 22, `q2' 22
   | R3's name on the 10-row body (R1 in `e3', R2 in `n2'):
   |     its own local `n3' 18, `q0' 16, `q2' 19, `q17' 19, `q17b' 19,
   |     `n2' 19, `r0' 24 at 279
   | The five partitions of {R1,R2,R3}, on the 13-row body:
   |     {R1 R2 R3} 13 (that body) | {R1 R2}{R3} 21 | {R1 R3}{R2} 10  <-- live
   |     {R1}{R2 R3} 19 | {R1}{R2}{R3} 18
   | Renaming does not matter, only the PARTITION does: all three roles renamed
   | together is 13, exactly like the body they came from, and giving R1 a fresh
   | name while R2/R3 share is 19, exactly like {R1}{R2 R3} above.  Four
   | fresh-name variants measured as a control: 19, 18, 18, 19.
   |
   | **SO SESSION 4's PERMUTER FIND WAS TWO THIRDS RIGHT.**  It merged `q' into
   | `e3' at both of its sites and measured 27; the correct move was to merge
   | only CASE 16's site and leave case 3's alone, which is 10 on the composed
   | body.  The permuter could not see that because it searched the 32-row body
   | with D2 on and a `goto shared' tail, where case 3's register was not yet
   | the only thing wrong there -- which is exactly why its gate-3 split read
   | the other way round at the time: case-16's site alone 29, case-3's site
   | alone 40 at 277, both 27.  **A gate-3 split is scoped to its own body too.**
   |
   | ====================================== CASE 2's WORD IS UNREACHABLE --
   |                                        CONFIRMED FROM A SECOND DIRECTION
   |
   | The target folds the symbol into the store:
   |     800C2A10 lui  $a0, %hi(D_80010A93)
   |     800C2A28 addu $v0, $v0, $a0
   |     800C2A30 sb   $zero, %lo(D_80010A93)($v0)
   | i.e. `(set (mem:QI (lo_sum R SYM)) 0)' with `R = (plus (high SYM) idx)' --
   | three words.  We materialise the whole address and add the index after:
   | `lui'/`addiu %lo'/`addu'/`sb 0($v0)' -- four.
   |
   | Session 4 closed the LEGITIMIZE_ADDRESS route (config/mips/mips.h has
   | exactly three arms -- `mips_check_split' on the WHOLE address, a PIC/CONST
   | arm, and `(plus REG CONST_INT)' -- and NO arm for `(plus SYMBOL_REF REG)'
   | in any operand order).  **That is confirmed, and this session closed the
   | only other route that could build that RTL.**
   |
   | `combine' is the only pass that creates `(lo_sum R SYM)' out of nothing,
   | and it does it in exactly one place: `find_split_point's MEM case
   | (combine.c:2493),
   |     if (GET_CODE (XEXP (x, 0)) == CONST || GET_CODE (XEXP (x, 0)) == SYMBOL_REF)
   |       SUBST (XEXP (x, 0), gen_rtx_combine (LO_SUM, Pmode,
   |                 gen_rtx_combine (HIGH, Pmode, XEXP (x, 0)), XEXP (x, 0)));
   | whose partner is `combine_simplify_rtx's LO_SUM case (combine.c:3711),
   | `(lo_sum (high FOO) FOO)' -> `FOO', commented "This is necessary so we can
   | add in an offset.  find_split_point will split this address up again if it
   | doesn't match."  **That pair is what folds a CONSTANT offset into the
   | relocation, and the MEM arm accepts ONLY a bare CONST or SYMBOL_REF** --
   | `(plus (symbol_ref) (reg))' takes neither arm, the next arm wants a
   | CONST_INT second operand, and the combination is rejected.  Traced the
   | three candidate combinations by hand and all three fail:
   |   * `addu' into `sb': `(mem (plus r2 r_idx))' is not a `memory_operand'
   |     under `mips_split_addresses', and find_split_point cannot split it;
   |   * `low' into `addu': `(plus (lo_sum r1 S) r_idx)' is not `arith_operand'
   |     and LO_SUM is rtx class `o', so find_split_point returns 0 for it;
   |   * `high' into `low': simplifies to `(set r2 S)', which `move_operand'
   |     (mips.c:712-719) rejects while `mips_split_addresses' is on, and
   |     find_split_point has nothing to split in a bare SYMBOL_REF.
   | And the four-word form is FORCED upstream of all of it:
   | `memory_address' (explow.c:395) calls `break_out_memory_refs' BEFORE
   | `GO_IF_LEGITIMATE_ADDRESS', and that force_reg's the SYMBOL_REF operand of
   | the PLUS -- which goes through `movsi' and therefore through the
   | `high'/`low' pair.
   | Note also that `GO_IF_LEGITIMATE_ADDRESS' DOES have a `(plus REG
   | CONSTANT_ADDRESS)' arm, with a comment describing the target's three-word
   | sequence word for word -- but it is gated on `! mips_split_addresses', and
   | it would be an ASSEMBLER macro expansion using `$at' in three CONSECUTIVE
   | words, where the target's `lui' sits SIX insns before its `addu'.  So the
   | target's form is compiler-scheduled and that arm is not the answer either.
   |
   | TEN MORE SPELLINGS MEASURED THIS SESSION, on the 9-row form-A body where
   | the word is the only thing in the way.  **EVERY ONE IS 279 WORDS:**
   |     `((u8 *)&D_80010A93)[j * 0x54]' .............. 9 at 279
   |     `*((u8 *)&D_80010A93 + j * 0x54)' ............ 9 at 279
   |     `j * 84' written decimal ..................... 9 at 279
   |     `(s32)&D_80010A93 + j * 0x54' (symbol first) . 9 at 279
   |     `(u32)&D_80010A93 + j * 0x54' ................ 9 at 279
   |     `(u8 *)&D_80010A93 + j * 0x54' ............... 9 at 279
   |     `da = (u8 *)&D_80010A93;' then `da[j*0x54]' .. 9 at 279
   |     `k = j * 0x54;' then `k + (s32)&D_80010A93' .. 19 at 279
   |     `da = (u8 *)&D_80010A93 + j*0x54;' `*da = 0;'  19 at 279
   |     the two stores swapped ....................... 26 at 279
   | Together with session 4's thirteen -- `(&D_80010A93)[j * 0x54]',
   | `*(&D_80010A93 + j * 0x54)', `*(u8 *)((u8 *)&D_80010A93 + j * 0x54)',
   | `((u8 *)&D_80010A93)[j * 0x54]', `(u32)j' for the index, and all of those
   | with the declaration changed to `extern u8 D_80010A93[];',
   | `extern u8 D_80010A93[0x54 * 0x40];', `extern u8 D_80010A93[][0x54];'
   | (with `D_80010A93[j][0]') and a 0x54-byte struct array (with
   | `D_80010A93r[j].u0'); hoisting `j * 0x54' into a local was 59 and swapping
   | the two stores inside the `if' was 66 -- **twenty-three spellings now give
   | the same four words.**  TREAT THIS WORD AS A PROPERTY OF THE COMPILER, NOT
   | OF THE SOURCE, AND DO NOT SPEND ANOTHER BUDGET ON IT.
   |
   | A NEGATIVE WORTH KEEPING FROM SESSION 4, because it looked like a lever
   | for ten minutes: `mips_check_split' DOES accept
   | `CONST(PLUS(SYMBOL_REF, CONST_INT))', so writing a CONSTANT-offset extern
   | access as `*(T *)((s32)&SYM + K)' -- an integer cast that does not fold
   | away, unlike `(u8 *)((u8 *)SYM + K)', which does -- really does change the
   | RTL: the address becomes a bare CONST, LEGITIMIZE_ADDRESS's LO_SUM arm
   | fires, and `.rtl' shows `(mem:QI (reg))' with MEM_IN_STRUCT_P cleared
   | instead of `(mem/s:QI (plus (reg) (const_int 36)))'.  IT IS WORTH NOTHING.
   | A two-function micro-test compiled with tools/cdkgcc -O2 -G0 gives
   | `lui $2, %hi(SYM+36)' / `lbu $3, %lo(SYM+36)($2)' for BOTH spellings --
   | gcc already folds a lone constant offset into the relocation, which is the
   | `find_split_point'/LO_SUM pair above doing its job.  The two spellings only
   | diverge when SEVERAL constant offsets of one symbol are live at once, and
   | then the ARRAY spelling is the cheaper one, because cse shares a single
   | `lui'/`addiu %lo' base and addresses the rest by displacement while the
   | cast spelling makes each offset its own relocation.
   |
   | ====================================== CASE 16's 6 ROWS: WHAT IS LEFT,
   |                                        AND WHAT CANNOT REACH IT
   |
   | The target's case 16 is
   |     800C2B38 lw   $v0, 0x0($s1)
   |     800C2B40 addu $t0, $v0, $zero          <-- the COPY
   |     800C2B44 lw   $a1, -0x14($t0)          <-- LOADS FIRST
   |     800C2B48 lh   $a2, 0x88($t0)
   |     800C2B4C lui  $v0, %hi(D_800DF55C)     <-- STORE SECOND
   |     800C2B50 sw   $t0, %lo(D_800DF55C)($v0)
   |     800C2B54 lbu  $a0, 0x24($a1)
   |     800C2B58 lbu  $a1, 0x25($a1)
   | and it wants two things at once that are MUTUALLY EXCLUSIVE in the source:
   |
   |   * THE COPY needs the read-back (`D_800DF55C = *(u8 **)p; e3 =
   |     D_800DF55C;'), because cse turns the reload into a copy of the value
   |     just stored and `update_equiv_regs' (local-alloc.c:1081-1093) gives it
   |     a REG_EQUIV that reload satisfies from the register already holding
   |     the value.  That is form A, and it puts the STORE FIRST in the RTL.
   |   * THE LOADS BEFORE THE STORE needs the store written after them, and
   |     then there is no read-back and no copy.
   |
   | And sched1 cannot repair either direction, and that is now DERIVED from
   | `memrefs_conflict_p' (sched.c:627-798) rather than assumed.  The store's
   | address is `(lo_sum R SYM)', and the function's first act is
   | `if (GET_CODE (x) == LO_SUM) x = XEXP (x, 1);' -- so it canonicalises to
   | the bare SYMBOL_REF.  The load's is `(plus (reg e3) (const_int -0x14))',
   | which the PLUS arm reduces to just `(reg e3)' with the offset folded into
   | `c'.  That leaves x = SYMBOL_REF, y = REG, and the tail is
   | `if (CONSTANT_P (x)) { ... return 1; }' -- **an unconditional conflict.**
   | The ONLY escape is `canon_rtx' turning the REG into a constant, which
   | happens only through `reg_known_value', which `init_alias_analysis'
   | (sched.c:440) fills only for a pseudo set EXACTLY ONCE and carrying a
   | REG_EQUIV -- the `ba' lever.  Case 16's pointer is a LOADED value, so its
   | known value can never be a constant and no number of sets changes that.
   | **A load therefore may not float above that store, nor the store sink
   | below the loads, in any body.  This direction is closed for good.**  **The `ba' lever does not apply** -- case 16's pointer is a LOADED
   | value, not a symbol address, so there is no REG_EQUIV for a second set to
   | invalidate (contrast case 3's R1, where it is the whole lever).
   |
   | **AND A CHAINED ASSIGNMENT PICKS WHICH OF THE TWO FRONTIER BODIES YOU
   | GET, BY ITS DIRECTION.**  This is the one genuinely new spelling this
   | session found, and the permuter found it (output-225-1, its best-scoring
   | find at score 225 against a base of ~475):
   |     e3 = D_800DF55C = *(u8 **)p;   ..... 9 rows at 279 -- FORM A's class
   |     D_800DF55C = e3 = *(u8 **)p;   ..... 10 rows at 278 -- FORM C's class
   | and `r = *(u8 **)((e3 = D_800DF55C = *(u8 **)p) - 0x14);' is 9/279 while
   | the same with the assignments the other way round is 10/278.  So
   | `e3 = (D_800DF55C = v)' keeps the `addu $t0, $v0, $zero' copy -- e3 takes
   | the VALUE OF THE ASSIGNMENT EXPRESSION, which is a second pseudo -- and
   | `D_800DF55C = (e3 = v)' does not, because there e3 IS the stored value.
   | **Two spellings that differ only in which `=' is the outer one land in
   | different frontier bodies.**  Neither beats the park.
   | The rest of the chained-assignment family is worth nothing (all 10/278):
   | case 3's `D_800DF55C = e3 = *(u8 **)&D_800E3D7C;'; case 0's three sprite
   | pairs written `*(u16 *)(b + 0xE) = *(u16 *)(b + 0x2) = w0 = *(u16 *)(r0 +
   | 0x2);' individually and all three at once; the tail's
   | `D_800814A0 = f17 = D_800814A0 | 0x8000;'; and a `bb = b;' alias local for
   | case 0 (the other half of the permuter's second-best find, which measures
   | 13 at 279 as a whole).
   |
   | TWELVE SHAPES WERE MEASURED ON THE 10-ROW BODY, mostly ways of getting the
   | copy WITHOUT the read-back, and every one that puts the loads first costs
   | 8 rows:
   |     e3; n3 = e3; store n3; loads off n3 ............... 10
   |     e3; D_800DF55C = e3; n3 = D_800DF55C; loads off n3  10
   |     e3; n3 = e3; loads off n3; store n3 ............... 18
   |     n3; e3 = n3; loads; store ......................... 18
   |     e3; loads; n3 = e3; store n3 ...................... 18
   |     e3; loads; store; read back into e3 ............... 18
   |     e3; loads; D_800DF55C = *(u8 **)p; ................ 18
   |     store; loads off a fresh `*(u8 **)p' deref ........ 21 at 279
   |     n3; store n3; e3 = D_800DF55C; loads off e3 .......  9 at 279
   |     form A, form A with the 0x88 load hoisted, form D
   |       (both loads re-read off D_800DF55C), form F
   |       (0x88 load before the `-0x14' load) ............. 9 at 279 (all four)
   |     form C with the 0x88 load hoisted ................. 10 (nothing)
   |     forms B2 and E2 (B/E with the 0x88 load hoisted) .. 12 at 280 both
   | Form B (`e3 = *(u8**)p; r = ...; D_800DF55C = e3;') is 18 at 278, and its
   | residue is worth reading because it says exactly why the loads-first order
   | does not pay.  **The `-0x14' load DOES come out before the store -- and
   | `e3' lands in $a2, with no copy, and case 3's R1 is dragged to $a2 with
   | it** (built 169/171/174 `lw a2,0(v0)' / `sw a2,0(v0)' / `lh a2,136(a2)'
   | against 800C2AC4/2ACC/2AD8, which are $t0 in the target).
   | THE MECHANISM: the 0x88 load's result is an outgoing ARGUMENT, so sched1
   | schedules it last -- AFTER the store -- which makes it `e3's last use, so
   | `e3' DIES at the `lh' and `combine_regs' ties the two into one register:
   | `lh a2,136(a2)', base and destination the same.  In the TARGET the store
   | sits at 800C2B50 AFTER the `lh' at 800C2B48, so $t0 is still live when the
   | `lh' writes $a2 and no tie is possible.  **So the target's shape needs the
   | store after BOTH loads, and in every source order that puts the store last
   | sched1 still sinks the argument load past it.**  Hoisting the 0x88 load
   | into a local to stop it being an argument is form B2: 12 rows but 280
   | words, i.e. two words long, and worse than form A on both counts.
   |
   | SESSION 4 ALSO MEASURED 48 CASE-16 BODIES ON THE 32-ROW BODY AND 24 MORE
   | ON THE 27-ROW ONE -- forms A/B/C/D crossed with {`q' shared, `q16' split}
   | x {no pair, a pair after the copy, a pair after the `-0x14' load} x {0x88
   | load inline, hoisted} -- and form A won both times.  Those scores are
   | stale (the body's tail has changed twice since) but the SHAPE conclusion
   | has now held four times: the read-back is right, splitting case 16's
   | pointer into its own local is not (every `q16'-split body was 35 or worse,
   | 277 words, because mechanism (f) only produces the copy while the pointer
   | is a multi-block allocno), and a cancelled pair on it buys nothing (14
   | bodies, all 277/53).
   |
   | The four rows that remain in form A are therefore a pure transposition --
   | our `lui'/`sw' pair sits at built 201/202 where the target has it at
   | 202/203 -- and nothing in the 20-odd shapes above moves it.  **This is
   | where a permuter or a dump-driven derivation should go next, and it is
   | worth 4 rows at most.**
   |
   | ====================================== SWEEPS: ALL FOUR, CLEAN, ON ALL
   |                                        THREE BODIES THIS SESSION -- AND
   |                                        NOTHING IS OWED
   |
   | **90f26de's THRESHOLD FIX WORKS AND IT IS THE REASON THIS SESSION GOT
   | ANYWHERE.**  Session 4 killed refsweep twice (45 minutes, unfinished)
   | because all four tools built an onefunc.py standalone only above 8000
   | lines and csrc/ovl_dungeon_800C2824.c is 4815.  With the threshold at 800
   | every one of the twelve runs below printed `standalone agrees (N rows, 278
   | words) -- sweeping there' and **all four sweeps on one body take 8
   | minutes.**
   |
   | ON THE 27-ROW BODY (session 4's, which owed all four):
   |     refsweep ... clean.  23 names, 3195 insertion points, "no cancelled
   |                  pair helps".
   |     devsweep ... clean.  0 devices already in the body; 93 statements x
   |                  23 names = 2232 candidates per round; "no wrapper helps".
   |     stmtsweep .. clean.  8 movable assignments, "no safe move helps".
   |     widthsweep . clean.  "no width change helps".
   | ON THE 13-ROW BODY: all four clean again (refsweep 24 names / 3367 points;
   |     devsweep 96 statements / 24 names = 2400 per round; stmtsweep 7
   |     movable assignments).
   | ON THE 10-ROW BODY (the live body): all four clean a third time --
   |     refsweep 25 names / 3519 insertion points, "no cancelled pair helps";
   |     devsweep 0 devices in the body, 96 statements x 25 names = 2496
   |     candidates per round, "no wrapper helps"; stmtsweep 7 movable
   |     assignments, "no safe move helps"; widthsweep "no width change helps".
   | **SO ALL FOUR SWEEPS ARE CLEAN ON THE LIVE BODY AND THERE IS NOTHING OWED.
   | Twelve runs, three bodies, every one on a standalone that agreed with the
   | unit.**  Do not re-run them without changing the body first.
   |
   | **AND THE `if (q)' DEVICE IS NOW MOOT.**  devsweep reports NOTHING on any
   | of these bodies -- not the 2-row duplicated-arm find session 4 declined,
   | not anything.  `q' no longer exists (session 4's permuter deleted it), and
   | the exhaustive 2026-11-22 result that only `q' as the condition ever paid
   | means there is nothing left for the device to read.  The standing decision
   | to decline it no longer has anything to decline; do not reintroduce `q'
   | to get it back.
   | THE EVIDENCE BEHIND THAT DECISION, KEPT BECAUSE IT IS WHAT MAKES IT A
   | DECISION RATHER THAN A GUESS.  On the 43-row body the duplicated arm was
   | tried around EVERY one of the 89 statements with EVERY one of the 19
   | locals and the three parameters as the condition (about 1900 bodies), plus
   | `do { } while (0);' around every statement.  EXACTLY FOUR bodies came in
   | under 43, all of them 41, and **all four had `q' as the condition** -- and
   | `q' was assigned only in case 3 and case 16, so every one of the four read
   | it in a block where it was not defined.  `if (p)', `if (c)', `if (*p)' and
   | `if (p[0])' around the same statement were all 43.  The same duplicated
   | arm placed in CASE 16, where `q' IS assigned above it, was 42 at 277 --
   | **so the device only ever paid from a block that does not define the
   | variable, which is precisely what makes it an uninitialised read.**  Giving
   | `q' a real definition in case 0 instead: `q = *(u8 **)p;' at the 0x4000
   | test 45, `q0 = q - 0x20;' with that definition 43, defining `q' inside the
   | early-exit block then using the arm 45, the arm with `q' defined at the
   | test 45.
   |
   | ====================================== THE PERMUTER RUN: 8 AND 9 ROWS AT
   |                                        278 WORDS, BOTH REJECTED, AND THEY
   |                                        PRICE THE DEFECT EXACTLY
   |
   | `permlog.tsv': 40m, 124,352 iterations, base=480, best=225, rate=51.8/s,
   | jobs=5, 14 finds.  Seeded on the 10-row body (`permimport.py' FIRST, and
   | `nonmatchings/func_800C2824/base.c' confirmed to carry `n2'/`hv'/`f17'
   | before the run started).  **ALL FOURTEEN FINDS NAMED IN THE permlog ROW
   | WERE MEASURED**, each against a baseline canary:
   |     output-465-1 ...  8 at 278  REJECTED (undefined read -- see below)
   |     output-470-1 ...  9 at 278  REJECTED (undefined read + unreachable)
   |     output-225-1 ...  9 at 279  the best LEGAL find (chained assignment)
   |     output-330-1, -385-1, -475-1 ... 10 at 278  (no change)
   |     output-410-1 ... 10 at 276 | output-425-3 ... 10 at 279
   |     output-330-2 ... 11 at 278 | output-425-1 ... 12 at 277
   |     output-235-1, -430-1 ... 13 at 279 | output-435-1 ... 14 at 277
   |     output-425-2 ... would not compile
   | **Nothing usable.  The park stays at 10.**
   | **GATE 1, MEASURED.  The two best word-exact finds beat the park -- and
   | BOTH are the uninitialised-read defect this park has now rejected FOUR
   | TIMES.**
   |
   |   output-465-1 ... **8 rows at 278 words** -- REJECTED
   |      case 16:  r = *(u8 **)(e3 - 0x14);      <-- READS e3 BEFORE IT IS SET
   |                e3 = *(u8 **)p;
   |                D_800DF55C = e3;
   |                if (func_800BBA40(r[0x24], r[0x25], ...
   |      `e3' is assigned in exactly two places in the whole body: case 3's
   |      `*(u8 **)&D_800E3D7C' and the line BELOW this one.  Reaching case 16
   |      means `*(s16 *)(p + 0x4) == 16', so case 3 never ran and `e3' is an
   |      indeterminate automatic.  **And this is not the sanctioned
   |      free-reference shape: the statement DEREFERENCES the garbage pointer
   |      and `r' is never recomputed, so `r[0x24]' and `r[0x25]' -- a wild
   |      load through an indeterminate address -- are passed to
   |      func_800BBA40.**  On hardware this reads garbage or faults.
   |   output-470-1 ... **9 rows at 278 words** -- REJECTED TWICE OVER: the
   |      same case-16 defect, PLUS `new_var = &(*((u8 **) p));' placed after
   |      case 16's `break;' and before `case 17:' -- unreachable -- and then
   |      read as `func_800A32A4(*new_var);' in case 17.  Both of this park's
   |      historical defect shapes in one find.
   |   GATE 3, SPLIT: there is nothing salvageable.  465-1's single change IS
   |      the defect; each of 470-1's two halves is independently defective.
   |
   | **THE BEST LEGAL FIND IS output-225-1 AT 9 ROWS / 279 WORDS**, and it is
   | the chained assignment written up under case 16 below -- form A's class,
   | so not word-exact.  Gate 2 on it is clean (assigned before use, nothing
   | unreachable) and gate 3 has nothing to split: it is one statement.
   | (`measure_finds.py' was NOT used: 447 output-* directories survive from
   | session 4's run, so its "N best scoring" would mix two runs against
   | different bases.  The finds named in THIS run's `permlog.tsv' row were
   | measured directly instead, each against a baseline canary.)
   |
   | ====================================== AND THE 2 ROWS BETWEEN 10 AND 8 ARE
   |                                        *EXACTLY* THE UNDEFINED READ
   |
   | This is the most useful thing the run produced, so do not lose it.  The
   | target's case 16 issues the `-0x14' load BEFORE the `D_800DF55C' store;
   | we cannot, because the store must precede the load for the read-back to
   | make the copy (above).  The rejected find gets the target's RTL order for
   | nothing -- it leaves the park's allocation alone and merely lets the load
   | be scheduled first -- and it can only do that by reading `e3' one line
   | before it is written.  **EVERY LEGAL WAY OF PUTTING THAT LOAD FIRST WAS
   | THEN MEASURED AND ALL OF THEM COST ROWS:**
   |     `r = *(u8 **)(*(u8 **)p - 0x14); e3 = D_800DF55C = *(u8 **)p;' .. 21
   |     `n3 = *(u8 **)p; r = ...; e3 = D_800DF55C = n3;' ............... 21
   |     `n3 = *(u8 **)p; r = ...; D_800DF55C = n3; e3 = D_800DF55C;' ... 21
   |     `n3 = ...; r = ...; D_800DF55C = e3 = n3;' .............. 13 at 279
   |     both loads first through `n3', then the chain ........... 20 at 280
   |     both loads first through fresh derefs, then the chain ... 20 at 280
   |     the chain first, then `r' through a FRESH deref of p .... 21 at 279
   | plus the twelve shapes under case 16 below.  **So this park's last two
   | rows have the same shape as the `if (q)' device's two: a real score that
   | only an undefined read reaches.  That is twice on one function, and it is
   | the reason 10 is the floor rather than a plateau.**
   |
   | ====================================== EXHAUSTIVE SPACES CLOSED ON THE
   |                                        10-ROW BODY (1127 BODIES)
   |
   | All measured on the live body, every candidate interleaved with a baseline
   | canary, ZERO canary drift in 1127 measurements.
   |
   | (i) THE FULL PARTITION OF THE SIX `*(u8 **)p'-DERIVED POINTER SITES IS
   |     CLOSED AT 10 -- **ALL 203 PARTITIONS MEASURED.**  The sites are
   |       s0    case 0's `q0 = *(u8 **)p - 0x20'
   |       s2    case 2's `q2'
   |       s3    case 3's `n2'
   |       s16   case 16's `e3' (carrying R1 with it)
   |       s17a  case 17's `q17'
   |       s17b  case 17's `q17b'
   |     and one partition of the set was generated for each of the 203, with
   |     each group named after the first of its members' original identifiers
   |     in declaration order so declaration order is perturbed as little as
   |     possible.  The all-singleton partition came out byte-identical to the
   |     live body, which is the batch's own control.
   |     FIVE PARTITIONS TIE AT 10, and every one of them keeps s0, s16 and
   |     s17a apart and merges only inside {s17b, s2, s3}:
   |       {s0}{s2}{s3}{s16}{s17a}{s17b}  <-- live
   |       {s0}{s16}{s17a}{s17b,s2,s3}
   |       {s0}{s16}{s17a}{s17b,s3}{s2}
   |       {s0}{s16}{s17a}{s17b,s2}{s3}
   |       {s0}{s16}{s17a}{s17b}{s2,s3}
   |     THE WHOLE DISTRIBUTION, so nobody has to re-run it: 5 bodies at 10,
   |     11 at 13, 11 at 15, 20 at 16, 27 at 18, 24 at 19, 32 at 21, 19 at 22,
   |     23 at 24, 8 at 25, 3 at 27, 7 at 28, 2 at 30, 5 at 31, 1 at 33, 3 at
   |     34, 2 at 37 -- and 190 of the 203 are 278 words exactly, the other 13
   |     being 279 or 280.  The eleven at 13 all merge s0 with s16.
   |     **NOTHING BEATS 10 and nothing ties it that is not a relabelling
   |     of the tail-local group.**  This supersedes session 4's 52-body `q'
   |     partition sweep, which covered four sites and was scoped to the 43-row
   |     body.
   | (ii) R1 MUST STAY MERGED WITH s16.  Giving case 16's site a fresh name and
   |     attaching case 3's D_800E3D7C value (R1) to each other local instead:
   |     `q2' 18, `q17' 18, `q0' 20, `n2' 21, `q17b' 21, and R1+s16 together
   |     (the live partition, renamed as a control) 10.
   | (iii) DECLARATION ORDER IS CLOSED.  **All 552 single moves of each of the
   |     24 declarations to each other position measured.**  528 are 10, i.e.
   |     no effect, and the other 24 are all the SAME declaration: moving
   |     `s32 rc[2];' ANYWHERE costs 7 rows (17/278), at every one of its 24
   |     destinations.  `s16 wh[2];' is free to move.  **So `rc[2]'s position
   |     -- immediately after `s32 j;' -- is uniquely optimal, and it is the
   |     only declaration in this function whose position matters at all.**
   |     AND THAT IS TWO CATALOGUE ENTRIES CONFIRMED IN ONE BATCH, which is
   |     worth stating as one rule: "DECLARATION ORDER IS NOT A LEVER -- 446
   |     orders, no change" holds for every SCALAR AND POINTER (528 of 528
   |     moves, 0 rows), because DECL_RTL is created lazily by expand_expr so a
   |     pseudo's number follows its FIRST USE; and "LOCAL DECLARATION ORDER IS
   |     FRAME LAYOUT, AND AN ARRAY BESIDE A `MATRIX' IS WHERE IT BITES" holds
   |     for every AGGREGATE (24 of 24 moves, 7 rows each).  **The tell is that
   |     the word count does not move at all -- 278 in all 552 bodies -- which
   |     is exactly func_800D6170's "eleven displacements turn over with no
   |     change in word count".  So sweep declaration order for the AGGREGATES
   |     and never for the scalars.**
   | (iiia) THE DECLARED TYPE OF `e3' AND `r' IS IRRELEVANT, re-confirmed on
   |     this body (the park said so for the 32-row body; widthsweep cannot see
   |     it, because its WIDTHS list is the six INTEGER widths and these are
   |     pointers).  `e3' as `u16 *', `u32 *', `s32' or `u32' -- with the
   |     `(u8 *)' casts the body then needs -- is 10/278, every one; `r' as
   |     `u16 *', `u32 *' or `s32' is 10/278, every one.
   | (iiib) EVERY `return'/`break' TERMINATOR VARIANT IS 10/278.  No tool
   |     varies these and they are what jump2 cross-jumps, so all eight
   |     semantically identical rewrites were priced: an explicit `return;' at
   |     the end of case 3's `if', of case 17's `if', of both; case 16's inner
   |     `return;' as `break;'; case 2's trailing `return;' as `break;';
   |     case 1's; `default: break;'; and all four `break' conversions at once.
   |     **Not one moves a row or a word** -- so the existing cross-jumps are
   |     already the ones the target has, and the arm terminators are not a
   |     lever on this function.
   | (iiic) EVERY OTHER READ-MODIFY-WRITE IN THE FUNCTION WAS SPLIT TOO, AND
   |     NONE OF THEM PAYS.  Because step 2c's split was worth 3 rows alone,
   |     the same transform was applied to all five other RMWs: `*(u16 *)(c +
   |     0x14) |= 0xC;', `*(u16 *)(n2 - 0x2) |= 0x8000;',
   |     `*(u16 *)(q17b - 0x2) |= 0x8000;' (each in two forms -- a temp with
   |     `t |= K;' as its own statement, and `t = load; store = t | K;') and
   |     `*(u16 *)(p + 0x4) += 1;' at all four of its sites, plus all three
   |     `|=' splits at once.  **ELEVEN BODIES, EVERY ONE 10/278.**
   |     WHY, AND IT IS THE POINT OF THE LEVER: a split pays only where the
   |     LOAD has to cross a store the compiler cannot disambiguate from it.
   |     D_800DF560's load has to cross `D_800814A0 = f;' -- an opaque-pointer
   |     dereference against a SYMBOL_REF store, which
   |     `memrefs_conflict_p' calls a conflict -- so only the split can express
   |     the middle position.  The other five RMWs have nothing to cross, so
   |     splitting them is a pure no-op.  **That is exactly what
   |     docs/matching-levers.md's "A READ-MODIFY-WRITE ON A GLOBAL MUST BE
   |     SPLIT AROUND A STORE THE COMPILER CANNOT DISAMBIGUATE FROM IT" says,
   |     and the five negatives are the control that makes it a mechanism
   |     rather than a coincidence.**
   | (iiie) THE `(s32)(w << 16) <= 0' TEST CANNOT BE RESPELT USEFULLY.  A late
   |     permuter find (output-330-2, 11 rows at 278) carried `w << 11 << 5'
   |     alongside a case-16 statement move, so GATE 3 SPLIT IT: the double
   |     shift ALONE, in case 1, in case 17, in both, and as 8/8, 15/1, 1/15,
   |     4/12, 12/4 and 5/11, is 10/278 in all nine bodies -- gcc folds it.
   |     The find itself is legal (it moves `D_800DF55C = e3;' after the
   |     `-0x14' load, which is form B) but it is a row worse than the park.
   | (iiid) THE `default:' ARM'S POSITION NO LONGER MATTERS AT ALL.  All six
   |     placements -- before case 0, before case 2 (where it is), before case
   |     3, before case 16, before case 17, and last -- measure 10/278.
   |     Session 4 recorded "43 or worse, 52 after case 0" on the 43-row body,
   |     so this is a verdict that CHANGED rather than merely went stale: the
   |     arm order the park guards is still right, but `default:' is now free.
   |     (All six are semantically identical: it is a bare `return;' and no
   |     arm falls into it.)
   | (iv) THE 16-WAY `w' POWER SET IS 10 EVERYWHERE.  All sixteen subsets of
   |     {case-0 sprite pair 2, case-0 sprite pair 3, case 1, case 17} given
   |     their own `u16' local instead of sharing `w': every one 10/278.  Clean,
   |     and re-confirmed on the current body (session 4 closed it at 43).
   | (v) ARRAY LENGTHS ARE CLOSED.  `rc[2..5]' x `wh[2,3,4,6]', all sixteen:
   |     rc[2]/wh[2] (the body), rc[2]/wh[3] and rc[2]/wh[4] tie at 10;
   |     rc[2]/wh[6] 29; every rc[3] or rc[4] body 31-32; rc[5] 25-31.
   |
   | ====================================== ALL THREE LEVERS WERE ALREADY IN
   |                                        THE CATALOGUE, AND THE PARK HELD A
   |                                        NEGATIVE FOR EACH OF THEM
   |
   | **NOT ONE of this session's three changes is a new mechanism.  Every one of
   | them is an existing docs/matching-levers.md entry that this park had
   | already recorded as a negative, because the negative was measured with the
   | wrong variant of the lever.**  That is the single most transferable thing
   | in this header.
   |
   |   lever                                      the park's negative, and why
   |                                              it was the wrong variant
   |   ---------------------------------------------------------------------
   |   "A SHARED TAIL IN THE TARGET MAY BE TWO     "duplicated tail 43/277,
   |   BLOCKS CROSS-JUMPED AFTER RELOAD --         50/285, 32/277, 39/285" --
   |   WRITE IT TWICE, WITH ITS OWN LOCALS"        all four measured with ONE
   |   (func_800B45E0)                             `f' SHARED between the
   |                                               copies, which the entry
   |                                               itself says "is the same
   |                                               position as the `goto'".
   |   "A READ-MODIFY-WRITE ON A GLOBAL MUST BE    "a `void *df = D_800DF560;'
   |   SPLIT AROUND A STORE THE COMPILER CANNOT    local is 32 with and without
   |   DISAMBIGUATE FROM IT" (func_800A6194), and  a cancelled pair" and "the
   |   "A READ-MODIFY-WRITE OF A SCALAR GLOBAL     legal permutations of case
   |   BLOCKS ITS OWN LOAD DELAY" (func_800A504C,  17's four tail statements are
   |   on D_800814A0 itself)                       36..52" -- the park moved and
   |                                               hoisted the statement but
   |                                               never SPLIT it.
   |   "one pointer local per arm cuts both        "case 16 + case 17 57" on the
   |   ways" / "TWO SWITCH ARMS CROSS-JUMP ONLY    44-row body -- a partition
   |   IF THEIR INDEX LANDS IN THE SAME            negative four sessions stale.
   |   REGISTER"
   |
   | The screen that would have caught all three in one pass: **for each
   | residue row, read the TARGET's register and ask which allocator produced
   | it.**  An ARGUMENT register ($a0-$a3) that the function's other arms
   | already get for the same quantity says "make this a multi-block allocno"
   | (step 1).  A LOWEST-FREE register ($v0/$v1) where ours has a callee-saved
   | or high temp says "make it block-local" (step 3).  Both are a partition
   | question and the partition is the whole lever; the park had measured both
   | partitions and both measurements were scoped to dead bodies.
   |
   | ====================================== THE 81-BODY GRID (step 2's
   |                                        COMPOSITION, EVERY CELL)
   |
   | 9 tail shapes x 9 case-16 forms, on the 25-row body.  Tail shapes are
   | {goto shared, duplicated tail} x {D2 on, D2 off} x {DF560 std, split,
   | split2}, where `split' adds a `df = D_800DF560;' pointer local and
   | `split2' does not -- **the two are identical in all 27 cells, so the three
   | `split2' rows are folded into their `split' rows below and only six are
   | listed.**  A duplicated tail always has its own `f' per copy here.
   | Case-16 forms: A = read-back; Ah = A with the 0x88 load hoisted;
   | B = loads then store; B2 = B with the 0x88 load hoisted; C = store then
   | loads (no read-back); D = A with both loads re-read off D_800DF55C;
   | E = B with a trailing read-back; E2 = B2 with one; F = A with the 0x88
   | load before the `-0x14' load.
   |
   |   tail shape              A     Ah    B     B2    C     D     E     E2    F
   |   goto  D2on std        25    25   37/277 28/279 29/277 25    37/277 28/279 25
   |   goto  D2on split      22/277 22/277 32/276 22    24/276 22/277 32/276 22   22/277
   |   dup   D2on std        25/277 25/277 35/276 25    27/276 25/277 35/276 25   25/277
   |   dup   D2on split      18/276 18/276 28/275 20/277 20/275 18/276 28/275 20/277 18/276
   |   dup   D2off std       20/280 20/280 29/279 22/281 21/279 20/280 29/279 22/281 20/280
   |   dup   D2off split     12/279 12/279 21/278 15/280 13/278 12/279 21/278 15/280 12/279
   | `goto shared' with D2 OFF and the DF560 statement unsplit is 30 at 281 and
   | does not improve.  A SHARED `f' with a duplicated tail: 32/285 with the
   | DF560 statement unsplit, 25/284 with it split, 32/279 with it merely moved
   | first.  "Moved first but NOT split" with OWN `f' per copy: 28/278 D2 off,
   | 31/278 D2 on -- so moving it is worth 3 rows LESS than splitting it even
   | where it helps at all.
   | The 13/278 cell is the body step 3 then took to 10.
   |
   | ====================================== STILL-VALID STRUCTURE AND
   |                                        MECHANISM FROM SESSIONS 3-4
   |
   | Everything in this block has been re-confirmed on a session-5 body or is a
   | compiler fact that does not depend on the body.
   |
   | * CASE 3's TWO CANCELLED PAIRS ARE STILL LOAD-BEARING, and `ba's is the
   |   bigger one.  On the 10-row body: no `ba' pair 17, neither pair 17 (and
   |   on the 13-row body, no `ba' LOCAL at all with `D_80082E80[0x24]' inline
   |   is 20, equal to no-pair there).  **But `e3's pair is now worth NOTHING:
   |   no `e3' pair is 10.**  It was worth 5 rows on the
   |   43-row body (where it lifted `QTY_CMP_PRI' out of the `floor_log2(1)==0'
   |   floor for a one-reference quantity); after step 3, R1+R3 is already a
   |   multi-block allocno with four references, so the floor no longer binds.
   |   **IT IS KEPT IN THE BODY ANYWAY, because it costs no words and no rows
   |   and because it is the one thing standing between R1 and a REG_EQUIV
   |   (symbol_ref) that `init_alias_analysis' would trust.**  Whoever edits
   |   case 16's form should re-price it: it may come back.
   |   Doubling either pair is 10, i.e. one is enough.  `n2++; n2--;' is 10,
   |   `r++; r--;' in case 16 is 10.
   | * `ba's PAIR IS THE sched.c:440 LEVER, unchanged and fully diagnosed in
   |   session 4: `init_alias_analysis' fills `reg_known_value' only for a
   |   pseudo set exactly ONCE, so with one set `ba' carries
   |   `REG_EQUIV (symbol_ref D_80082E80)', `canon_rtx' replaces the base with
   |   the bare symbol, and `memrefs_conflict_p' disambiguates both `lbu's
   |   against every stack slot and against `%lo(D_800DF55C)' on the symbols
   |   alone -- so they float to the front of the block.  `ba++; ba--;' makes
   |   `reg_n_sets' 2, the note is no longer trusted, and the real dependences
   |   appear.  The pair compiles to nothing (combine folds the two addiu).
   |   **If a sched1 residue involves a constant symbol at either end of a
   |   memory pair, check the REG_EQUIV guard before the MEM flags** -- session
   |   3 spent its budget on MEM_IN_STRUCT_P, which is set in the winning body
   |   too.
   | * THE "NEW LOCAL + CANCELLED PAIR" LEVER IS EXHAUSTED EVERYWHERE ELSE on
   |   this function (session 4, on the 32-row body, all 32/278 i.e. nothing):
   |   `da = (u8 *)&D_80010A93;' (32 without the pair, 38 with),
   |   `pa = (u32 *)0x80010980;', the two together (38), `r16++/--' and `q++/--'
   |   in case 16, `h16 = *(u16 *)(q + 0x88);', `df = D_800DF560;',
   |   `dp = &D_800DF45C;', `f++/--' at five positions, `fa = &D_800814A0;'.
   |   What makes case 3's two sites special is that their REG_EQUIV is READ by
   |   a later pass; elsewhere nothing consults it.
   | * `a' SPLIT PER ARM (`a2'/`a17') is 10, i.e. nothing.  Case 16's two `lbu'
   |   arguments hoisted into `x16'/`y16' is 10; into `x2'/`y2' 10; into
   |   `x17'/`y17' 10.  But case 2's and case 17's four argument locals must
   |   stay FOUR: sharing `x' is 28/280, sharing `y' 38/280, sharing both
   |   28/280.
   | * CLOSED AS EXHAUSTIVE AND NEGATIVE, from session 4, on the 43-row body:
   |   the full 16-way `w' power set (43 everywhere); the 52-body `q' partition
   |   space (best 43); `q16' splitting (277/53 in all sixteen combinations,
   |   with a second set failing to buy it back across 14 more bodies); all 306
   |   single-declaration moves (at 49 rows); `rc[2..5]' x `wh[2,3,4,6]' array
   |   lengths; the 89-statement x 22-name duplicated-arm/do-while space (about
   |   1900 bodies, only four under 43 and all four reading `q' uninitialised);
   |   every position of `default:' (43 or worse, 52 after case 0).
   |   **ALL OF THESE ARE SCOPED TO BODIES THAT NO LONGER EXIST** -- and FIVE
   |   OF THEM WERE RE-RUN ON THE 10-ROW BODY THIS SESSION, in bigger forms:
   |   the `w' power set, declaration order (552 moves against session 4's
   |   306), the array lengths, the `q' partition space (all 203 partitions of
   |   the six sites against session 4's 52 over four), the `q16'-split family
   |   (the R3_* row under step 3) and the `default:' positions.  See
   |   "EXHAUSTIVE SPACES CLOSED ON THE 10-ROW BODY".  **Only ONE of the six
   |   changed verdict, and it is `default:', which is now neutral everywhere.**
   |   What is NOT re-run: the 1900-body duplicated-arm/do-while space, and it
   |   does not need to be -- devsweep covers it and is clean on this body.
   | * BLOCK ORDER IS RIGHT.  Arm order (0, 1, default, 2, 3, 16, 17) confirmed
   |   against the listing's addresses in session 4.
   | * **THE FOUR STRUCTURAL CLAIMS THE PARK HAD ONLY MEASURED ON THE 44-, 49-
   |   AND 57-ROW BODIES ARE NOW RE-PRICED ON THE LIVE ONE, AND ALL FOUR
   |   HOLD** (every ablation is worse, so do not "simplify" any of them):
   |       `w0' merged back into `w' .............. 20   (worth 10 rows)
   |       `r0' merged back into `r' .............. 18 at 279
   |       both ................................... 18 at 279
   |       `x2'/`y2' inlined into case 2's call ... 14   (worth 4 rows)
   |       `x17'/`y17' inlined into case 17's ..... 14   (worth 4 rows)
   |       both pairs inlined ..................... 18   (worth 8 rows)
   |   So the four distinct argument locals and the two case-0 splits are each
   |   load-bearing on this body, not just on the bodies they were found on.
   |   **AND THE FOUR STACK-PAIR STORES' ORDER IS RE-CONFIRMED UNIQUELY
   |   OPTIMAL, all 24 permutations re-measured on the live body:**
   |   `rc[0]; rc[1]; wh[0]; wh[1];' is 10 and NOTHING ELSE IS UNDER 14.  The
   |   distribution is 10, 14, 14, 15, 15, 16, 16, 17, 17, 18 x4, 19 x3, 20,
   |   20, 21, 21, 22 x3, 23 -- all at 278 words, so what the order decides is
   |   which 32-bit constant is MATERIALISED first and hence which `lui' reorg
   |   steals into the `bnez' delay slot, exactly as session 3 recorded.
   | * STRUCTURE THAT IS ALREADY RIGHT, re-confirmed: case 0 falls through into
   |   case 1 and case 1 into the default `return'; cases 1, 2 and 16 share
   |   `*(u16 *)(p + 0x4) += 1; return;' as a jump2 cross-join;
   |   `(s16)func_80042900(...) == 0' for the 0x1B arm; the 0x3000/0x300 pick
   |   as a plain `if'; the DOUBLED `D_800814A0 = f;' is real (the target
   |   stores the same $v1 at 800C2C3C and again at 800C2C58); the four
   |   stack-pair stores in the order `rc[0]; rc[1]; wh[0]; wh[1];';
   |   THE STACK PAIRS MUST BE ARRAYS (as four scalars nothing reads sp24 or
   |   sp2A, gcc deletes both stores as dead: 280 words / 105 rows with a 0x40
   |   prologue where the target has 0x48); `r' split for case 0 (`r0') and
   |   `w0' split for the FIRST of case 0's three sprite pairs, which are a
   |   PAIR and must not be ablated alone.
   | * func_800BBA40's third parameter is declared `s16' in
   |   csrc/ovl_dungeon_decls.h, which is why `*(u16 *)(e3 + 0x88)' comes out
   |   as `lh' and not `lhu' in both case 3 and case 16.  Do not "fix" the
   |   cast.
   |
   | ====================================== THREE TOOLS THIS PARK WANTED AND
   |                                        DOES NOT HAVE
   |
   | Recorded here because all three would have found this session's changes
   | mechanically, and two of the three would help the whole block.
   |
   | 1. **A PARTITION SWEEP.**  Two of the three changes were partition moves
   |    -- merge case 16's `r' into the shared one, split case 3's pointer out
   |    of `e3' -- and the park held a stale negative for each.  Enumerating
   |    the partitions of the locals that hold the SAME expression is a
   |    40-line generator (`partitions()' over the sites, each group named
   |    after the first of its members in declaration order) and it measured
   |    all 203 partitions of this function's six `*(u8 **)p' sites in about
   |    40 seconds on an onefunc standalone.  Neither refsweep, devsweep,
   |    stmtsweep nor widthsweep varies a partition; the permuter reaches them
   |    only by luck, and session 4's find was a partition move it got
   |    two-thirds right.
   | 2. **A READ-MODIFY-WRITE SPLIT SWEEP.**  `*(T *)(E) op= V;' rewritten as
   |    `t = *(T *)(E); ... *(T *)(E) = t op V;', with the store at each later
   |    legal position in the block.  That is step 2c, worth 3 rows alone and
   |    8 in composition, and it is already a hand lever in the catalogue
   |    twice over (func_800A6194, func_800A504C).  stmtsweep MOVES simple
   |    assignments and cannot rewrite one statement into two; devsweep only
   |    wraps.  The five negatives under (iiic) are the control that says when
   |    it pays, so the sweep can even be ordered: try the RMWs whose load
   |    would have to cross another global's store first.
   | 3. **AN INTERMEDIATE-WORD-EXCURSION MODE for the four sweeps.**  Step 2's
   |    two halves measure 280 and 277 words; only together are they 278.
   |    Every sweep and `measure_finds' orders on `(abs(ww - tw), rows)' and
   |    so rejects each half on its own -- correctly, as a final answer, but
   |    it makes the pair unreachable one change at a time.  A beam that
   |    allowed, say, +-3 words at intermediate depth and required
   |    word-exactness only at the leaves would have found it; the repo has a
   |    beam driver (`tools/decomp/search.py') that could carry the flag.
   |
   | ====================================== EARLIER SESSIONS (kept, SCORES
   |                                        STALE)
   |
   | 2026-11-20: case 3 inlining the shared tail cost four words; `goto shared'
   |   took 285 -> 281.
   | 2026-11-21 (earlier): refsweep's `r++; r--;' took 78 -> 73 and stmtsweep's
   |   D2 move took 281/80 -> 278/78.  A 75-row permuter find at 279 words was
   |   REJECTED for hoisting `D_80082E80' into a local whose assignment sits
   |   after a `return;'.
   | 2026-11-21 (session 3), 73 -> 43: "one local per symbol lifetime" applied
   |   six times -- `q' split at four of six sites 73 -> 62; the four
   |   stack-pair stores reordered 62 -> 57; the two `lbu' arguments hoisted
   |   through four locals 57 -> 49; `r' split for case 0; `w0' split 53 -> 43;
   |   the case-16 read-back 43/277 -> 44/278; `r' split for case 16 44 -> 43.
   | 2026-11-22 (session 4), 43 -> 27: `ba'/`e3' cancelled pairs 43 -> 32 by
   |   hand, then a 40-minute 5-job permuter run (131,762 iterations) deleted
   |   `q' and reused `e3' at all three of its sites, 32 -> 27.  Its best
   |   SCORING find measured 41 rows at 279 words while the 27-row body was a
   |   lower-scoring find -- the score and the row count are not monotonic on
   |   this function, which is why every find must be MEASURED.
   | 2026-11-22 (session 5), 27 -> 10: this header.
   |
   | **FIVE PERMUTER FINDS AND ONE SWEEP FIND HAVE NOW BEEN REJECTED ON THIS
   | PARK FOR READING A LOCAL UNINITIALISED**, two of them with the assignment
   | sitting after a `return;' or a `break;', and the case-16 statement-order
   | defect has now been produced INDEPENDENTLY BY TWO DIFFERENT TOOLS (a
   | statement sweep in session 3, the permuter in session 5).
   | **ANY SUSPICIOUSLY GOOD SCORE REPORTED FOR THIS FUNCTION SHOULD BE READ
   | FOR THAT SHAPE BEFORE IT IS BELIEVED.**
   |
   | DECLARATIONS
   | void func_800C27F0();
   | void func_800C0180();
   | void func_800CEF54();
   | extern u8 D_80010A93;
   | extern u8 D_800E1640[];
   | END DECLARATIONS
   | (func_800C27F0 is in the HEAD unit and func_800CEF54 in
   | ovl_dungeon_800CABA8, so both are cross-unit references now and need a
   | declaration where a single-unit build needed none.  Everything else --
   | D_800DF45C, D_800DF4A4, D_800DF55C, D_800DF560, D_800E3D7C, D_80082E80,
   | D_800814A0 -- is already in csrc/ovl_dungeon_decls.h.)
*/
void func_800C2824(u8 *p, u8 *b, u8 *c)
{
    u8 *q0;
    u8 *q2;
    u8 *q17;
    u8 *q17b;
    u8 *r;
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
    u32 f3;
    u32 f17;
    u8 v;
    u16 hv;
    u16 w;
    u16 w0;
    u8 *ba;
    u8 *e3;
    u8 *n2;
    u8 *n3;

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
                u8 *bb = (u8 *)0x80010000;

                *(u8 *)(bb + j * 0x54 + 0xA93) = 0;
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
            n2 = *(u8 **)p;
            *(u16 *)(n2 - 0x2) |= 0x8000;
            f3 = D_800814A0 | 0x8000;
            D_800814A0 = f3;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 = f3;
        }
        break;
    case 16:
        n3 = *(u8 **)p;
        r = *(u8 **)(n3 - 0x14);
        e3 = n3;
        a = *(s16 *)(e3 + 0x88);
        D_800DF55C = e3;
        if (func_800BBA40(r[0x24], r[0x25], a, &D_800DF45C, 0x2800, 0x208020,
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
            hv = *(u16 *)(D_800DF560 + 0x1E);
            f17 = D_800814A0 | 0x8000;
            D_800814A0 = f17;
            *(u16 *)(D_800DF560 + 0x1E) = hv | 0x8000;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 = f17;
        }
        break;
    }
}
