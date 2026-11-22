/* PARK: func_800A05A4, 139 of 139 words, 26 aligned differences
   (`ovlverify' reports 57 differing masked).  ovl_dungeon.
   Measured on the REAL UNIT, not only on an onefunc.py standalone.

   ============================== 2026-11-21 (evening): 45 rows -> 26

   The 45-row park carried a `do { } while (0);' around
   `r = func_8009B4B0(p, xx, yy);' and an `s32 ii' whose only job was to stop
   loop.c hoisting the HImode->SImode conversion of `i'.  BOTH ARE GONE.
   The body below has no `ii' at all (`i << 9' is written inline again) and
   no do-while, and it carries two cheaper devices instead.

   ------------------------------------------------------- 1. THE DO-WHILE
   ------------------------------------------- WAS COSTING A floor_log2 STEP

   Deleting it is worth 27 rows and a word on this body (ablation: put it
   back and the same body measures 138 words / 53 rows).  The mechanism is
   an arithmetic cliff that nothing in this block had priced:

     flow.c:2122 counts a reference as `reg_n_refs[regno] += loop_depth', so
     a `do { } while (0);' adds exactly +1 to EVERY operand of the statement
     it wraps -- here r, p, xx AND yy, not just the one you wanted.

     global.c:597 (cdk-gcc-b18) prices an allocno at
     `floor_log2(n_refs) * n_refs / live_length * 10000 * size', and
     floor_log2 STEPS at every power of two.  `p' is a pointer parameter
     live across the whole function (live_length 210 after
     update_equiv_regs' REG_EQUIV doubling), and the wrapper is the
     difference between

         15 refs -> 3*15/210 = 0.214   LAST of thirteen allocnos
         16 refs -> 4*16/210 = 0.305   SEVENTH

     MIPS defines no REG_ALLOC_ORDER, so global.c hands out callee-saved
     registers in ascending order -- $s0..$s7 then $fp ($30) -- to the
     allocnos in `allocno_compare' order that need a NEW register.  One
     reference too many moved `p' five places up that queue and rotated five
     registers.  The target has `p' in $fp, i.e. DEAD LAST, and the only way
     to get there was to take a reference away.  Devices only ADD.

   ------------------------------------------- 2. `i++; i--;' AT THE LOOP TOP

   Worth the missing word.  Ablation: delete it and the same body measures
   138 words / 23 rows, one word short because scan_loop hoists the
   zero_extend of `i' into the preheader and the preheader copy
   `addu $s5,$s6,$zero' that the target has disappears with it.

   The pair is DEAD -- the very next statement is `i = g;' -- and combine
   folds the two addiu into each other, so it costs no instructions.  What
   matters is that it survives until FLOW, which is what counts references
   (toplev runs cse2, bp, flow, THEN combine).  A duplicated `i = g;' does
   not: cse2 deletes it before flow ever sees it.  Measured, all 138/23,
   i.e. all failing to buy the word:

       `i = g;' added at the loop bottom                     138/23
       `i = g;' added just before the func_800A44E0 call     138/23
       `i = g;' written twice at the loop top                138/23
       `i++; i--;' after the `if (r != NULL)' block          139/59
       `i++; i--;' moved below `i = g;'                      139/26 (same)

   ------------------------------- 3. `if (lim) { return 0; } else { ... }'

   A duplicated arm on the FIRST early return (the one before the loop),
   whose free reference lands on the CONDITION's variable -- `lim', the
   HImode copy of `n' that ends up spilled at 0x18.  Worth 6 rows
   (ablation: write a plain `return 0;' and the body measures 139/32).
   jump2 cross-jumps the two arms, so it costs nothing.  `if (n)' measures
   identically; `if (thr)' is 32, `if (k)' is 142 words / 39.

   THE DEVICE-FREE ALTERNATIVE, for the record: drop both devices and the
   body is 138 words / 41 rows; drop them and put the do-while back and it
   is 138 / 47.  The 45-row park was 139 / 45.  Neither device is pretty and
   both are ordinary C that jump2/combine erase.

   -------------------------------------- what the residue is now, 26 rows

   TWO THINGS, and they are coupled.

   (a) ONE INSTRUCTION IN THE WRONG BLOCK, as before, but the other way
       round.  loop.c hoists the zero_extend of `i' into the preheader:

           this     lhu $s5, 0x10($sp)   at word 72 (the preheader)
                    sll $a3, $s5, 9      at word 105
           target   lhu $t0, 0x10($sp)   at word 104 (with its use)
                    sll $a3, $t0, 9      at word 109

       and `i' is spilled with `sw $a3,0x10($sp)' where the target writes
       `sh $a3,0x10($sp)' -- same slot, same reload, SImode instead of
       HImode.  Worth about seven of the 26 rows.

       THE FIX IS KNOWN AND IT COSTS MORE THAN IT PAYS.  scan_loop
       (loop.c:789-800) admits an invariant as a movable if ANY of
         (1) `! maybe_never && ! loop_reg_used_before_p'
         (2) `! REG_USERVAR_P (SET_DEST) && ! REG_LOOP_TEST_P (SET_DEST)'
         (3) `reg_in_basic_block_p (p, SET_DEST (set))'
       holds, and `invariant_p' is only consulted after that.  Giving `i' a
       set that is NOT itself a movable -- put `i = g;' AFTER the
       `if (r != NULL)' block, so maybe_never is already 1 and the use is in
       a later block -- makes n_times_set[i] nonzero, the conversion is no
       longer invariant, and the `lhu' lands exactly where the target has
       it.  That body is 139 words and 59 ALIGNED ROWS: the instruction
       order is right and the register assignment is much worse.  Four
       spellings of it (`i = g;' alone after the block; the same plus the
       loop-top set; the cancelled pair after the block; both) all measure
       139/59, and a 1289-candidate single-device sweep ON that 59-row body
       bottoms out at 44 rows at 139 words (37 at 141).  So the regime with
       the `lhu' in the right block is, as of today, worse everywhere than
       the regime with it in the preheader.  That is the one thing in this
       park most worth attacking next: the two defects are not independent,
       and 26 is the better half of a choice, not a floor.

   (b) A THREE-CYCLE IN THE CALLEE-SAVED FILE.  Six of the nine are now
       correct:

           quantity                     this     target
           xx                           $s0      $s0    correct
           yy                           $s1      $s1    correct
           thr                          $s3      $s3    correct
           r                            $s4      $s4    correct
           &D_8006CCE8[g] (entry), k    $s7      $s7    correct
           p                            $fp      $fp    correct
           g*2 (early)                  $s2      $s5    <- the cycle
           &D_8006CCD8[g]               $s5      $s6    <-
           `n' temp, loop &D_8006CCE8   $s6      $s2    <-

       and the frame (0x50), the spill slots (0x10 `i', 0x18 `lim',
       0x20 `g*2') and the save/restore set are the target's.

   HOW TO READ THIS, FOR THE NEXT AGENT.  `.greg''s FIRST line,
   `;; N regs to allocate: ...', IS allocno_compare's output in order, and
   the Nth allocno in it that needs a NEW register takes the Nth
   callee-saved register.  Dividing floor_log2(n_refs)*n_refs by
   live_length, both printed by `.lreg''s dump_flow_info
   (`Register N used R times across L insns'), reproduces that list exactly.
   On the OLD 45-row body it was

       87 xx   16/ 77 = 0.831  -> $s0     105 g*2 early  4/27 = 0.296 -> $s2
       88 yy   16/ 86 = 0.744  -> $s1     106 loop CCE8  4/28 = 0.286 -> $s3
       95 ii    4/ 12 = 0.667  -> $s2      93 k         7/57 = 0.246 -> $s7
      176 CCD8  7/ 29 = 0.483  -> $s3     164 copy      6/52 = 0.231 -> $fp
       86 r    10/ 78 = 0.385  -> $s4      85 n temp    3/16 = 0.188 -> $fp
       92 thr   9/ 75 = 0.360  -> $s5     117 CCE8 entry3/18 = 0.167 -> $s7
       80 p    16/210 = 0.305  -> $s6     163,89,91 spilled

   and that table is what the whole 45 -> 26 came out of: `p' at 0.305 has
   to be below `117' at 0.167, and only losing a reference does it.

   NOTE ALSO: allocno 95 WAS `ii' (the dump marks it `reg/v'), the
   third-highest priority in the function.  The target has no such quantity
   at all -- its zero_extend destination is $t0, a caller-saved reload
   register, so that pseudo is BLOCK-LOCAL there and never reaches
   global_alloc.  Deleting `ii' was therefore right on the merits as well as
   on the score.

   ------------------------------------------------ measured and rejected
   ------------------------------------- ON THE 26-ROW BODY BELOW (current)

     refsweep    15 names x 830 insertion points, 2 rounds   nothing
     devsweep    33 statements x 15 names, 2 rounds          nothing
     widthsweep  every declared width, every local           nothing
     stmtsweep   8 movable assignments                       nothing
     1413 single devices scored on ROWS (own sweep)          nothing under 26
     1260 interleavings of the seven entry-block statements  all 26 or worse
     a 1413-candidate single-device sweep scored on the
       REGISTER ASSIGNMENT instead of rows                   no single change
                                                             reaches the target
     `u16 ii' instead of `s32 ii'                            26 (identical)
     `s16 ii'                                                137/53
     keeping `ii' at all (byte-identical streams)            26

   ------------------------------------- ON THE 45-ROW PARK BODY (previous)

     all four sweeps clean, 2 rounds each
     decomp-permuter, 40 min x 5 jobs, 22 finds, best score 575.  Measured
       with measure_finds.py: 138/52, 141/50, 137/69, 141/65, 139/45,
       141/66, 141/77, 141/52.  NOT ONE BEAT THE PARK.  The permuter's
       object diff and the aligned row count disagree badly on this
       function; do not read `best=NNN' as progress.

   ------------------------------------- ON THE NO-DO-WHILE BODY (139/64)

     all 210 dependency-legal entry orders      best 59 (g,thr,r,lim,i,xx,yy)
     1230 single devices                        best 29, but at 142 words
     `if (ii) { ii = i; } else { ii = i; }'     142/29  -- AND IT READS `ii'
                                                UNINITIALISED on iteration 1
     `if (lim) { i = g; } else { i = g; }' plus
       `if (ii) { d = ...; } else { d = ...; }' 141/20  -- same defect, and
                                                two words long, because `ii'
                                                spills to 0x20 as a word and
                                                pays `sw' + `lw' for it
   The 141/20 is the lowest residue seen anywhere in this search and it is
   rejected on both counts.  It is recorded because it proves the remaining
   permutation is reachable, and that an extra reference to the variable
   that carries the conversion is what reaches it.

   ------------------------------------------------ STILL TRUE, CARRIED OVER

   NO NEW DECLARATIONS.  The prototype `s32 func_800A05A4(u8 *, s32, s32,
   s16, s32);' at csrc/ovl_dungeon.c:1483 is pinned by the landed caller
   func_800C0E88 and this body matches it.  Writing the fourth parameter
   `s32' is a hard `conflicting types', which is how its type was settled.

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
     and is one word short.

   `u16 i' MUST HAVE TWO SETS -- one before the loop and one as the loop
   body's first statement.  With a single `i = ((u32)dir >> 9) & 7;' the
   HImode pseudo never survives: combine's `set_nonzero_bits_and_sign_copies'
   records nonzero_bits 7 for the `andi' result (it records only for pseudos
   with REG_N_SETS == 1), `zero_extend (subreg:HI g)' folds back to `g', and
   `i' is spilled as a WORD.  Dropping the loop-top `i = g;' from THIS body
   (leaving only the cancelled pair there) is 138 words / 65 rows; dropping
   it from the device-free 138/23 body is 137 words / 50 rows.

   SPELLINGS MEASURED ON EARLIER BODIES (2026-11-16 / 2026-11-20), kept
   because the shapes they rule out have not changed:

     on the old 139-word plateau (all BYTE-IDENTICAL), each 139/80:
       s16 i instead of u16 i; ((u16)dir >> 9) & 7; ((u32)dir >> 9) % 8;
       j = (u32)dir >> 9; i = j & 7; `dir' reassigned in place; the whole
       `((u32)dir >> 9) & 7' inlined at all 7 uses; i * 512; (s32)i << 9;
       (u16)D_8006CCD8[i]; operand order flipped on the x + ... sum;
       `i' declared first / last; xx/yy declaration order swapped; s32 r with
       casts; s16 *dp = &D_8006CCD8[i] inside the loop; for (k = 0; k < lim;
       k++); k = 0; if (k < lim) { do { } while (k < lim); }.
     and everything that moved the word count, all worse:
       ((u32)dir & 0xE00) >> 9  139/82;  (i << 9) & 0xE00  139/94;
       (i & 7) << 9  137/105;  u32 i  137/105;  s32 w9 = i << 9 hoisted
       137/113;  s32 g with a single u16 i = g  137/101;  u16 i[1] /
       volatile u16 i / (void)&i  137/101-106;  s16 k 140/91;  s16 d 141/103;
       s16 *ep 140/108;  both dp and ep 140/111;  u16 xx, u16 yy 145/136;
       k = 0 above the first call 142/124;  (void)&i on the two-set body
       142/135;  volatile u16 i on it 142/114;  __asm__("" : "+r"(i)) at the
       loop top 144/136, before the func_800A44E0 call 148/142;
       __asm__("" ::"r"(i)) there 143/132;  dp and ep loop-top pointers
       139/76;  ep-only 139/68.

     DECLARATION ORDER IS NOT A LEVER: 446 orders, 47 rows and 139 words for
     every one of them (DECL_RTL is lazy, so a local's pseudo number follows
     its first USE).  See docs/matching-levers.md.
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
        if (lim)
        {
            return 0;
        }
        else
        {
            return 0;
        }
    }
    k = 0;
    while (k < lim)
    {
        i++;
        i--;
        i = g;
        r = func_8009B4B0(p, xx, yy);
        if (r != NULL)
        {
            d = *(s16 *)(r + 0x88) - *(s16 *)(p + 0x88);
            d = d >= 0 ? d : -d;
            if (d <= (s16)thr)
            {
                break;
            }
        }
        if (func_800A0548(xx + (D_8006CCD8[g] & 0xFFFF), yy + (D_8006CCE8[g] & 0xFFFF)) != 0)
        {
            p[0x72] = *(u8 *)&D_8006CCD8[g] - xx;
            p[0x73] = *(u8 *)&D_8006CCE8[g] - yy;
            return 0;
        }
        if ((s16)func_800A44E0((xx << 6) & 0xFFC0, (yy << 6) & 0xFFC0,
                               (s16)(*(u16 *)(p + 0x88) - thr), i << 9) != 0)
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
