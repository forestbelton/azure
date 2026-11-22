/* ===========================================================================
   BEST MEASURED CANDIDATE for func_800ADE74, ovl_dungeon (1817 words).

   2026-11-22, fourth session.  1817/1817 WORDS, 30 aligned rows
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

   NOTE ovlverify's POSITIONAL count on this body says `35 differing' where
   aligndiff says 30.  aligndiff is the honest figure (ovlverify compares word
   N against word N, so a transposed pair misses twice).  Use aligndiff.

   THE WORD COUNT IS HONEST: every word is accounted for, and the first 274
   words and the last 398 are byte-identical.  What is left is 30 rows in
   THREE register/schedule clusters, listed at the bottom.

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
   RESIDUE THAT IS LEFT: 30 ROWS, 0 WORDS, THREE CLUSTERS
   (rows 275/280/391/398/405/555/557/558 = cluster III+IV, 8 rows;
    rows 1229..1248 = cluster II, 8 rows; rows 1330..1418 = cluster I, 14.)

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

   (II) case 0x24's Chebyshev test.         WAS 13 rows; NOW 8, 0 words.
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

   (III) `sp20 = r;' is stored from the COPY, not from the call result.
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

   (IV) One single row: a `bnez' at target 405 whose destination is word 525
       where the target's is 574.  Both are entries into the same shared
       tail, so this is a `break' that reaches the tail one label earlier.
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
   * `dx = (dx >= 0) ? dx : -dx;' next to
     `dy = (t3 < 0) ? -t3 : t3;' in the SAME arm.  THIS ASYMMETRY IS NOW
     EXPLAINED rather than merely measured -- see change (H).  `x >= 0 ? x :
     -x' folds to one `abs' RTL insn and `x < 0 ? -x : x' does not; dx wants
     the folded form and dy wants the unfolded one, because the target tests
     the subtraction's own register and copies into dy in the delay slot.
     Making the two match either way costs 10 to 23 rows.
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
   * NO DEVICES.  Not one `do { } while (0)', cancelled pair, duplicated arm,
     `__asm__' or redundant store, and no `goto'.  THREE full cancelled-pair
     sweeps now say none helps, the third on this very body ("no cancelled
     pair helps", 36686 insertion points).  Worth keeping true.  Confirmed
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
            dx = b[0x24] - sp18;
            dx = (dx >= 0) ? dx : -dx;
            mx = dx;
            sp18 = dx;
            t3 = b[0x25] - sp1A;
            dy = (t3 < 0) ? -t3 : t3;
            sp1A = dy;
            r9 = sp18 < sp1A;
            if (r9)
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
