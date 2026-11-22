/* RECONSTRUCTION of func_80089AA0 (1165 words), batch j76, 2026-11-16.

   ===================================== 2026-11-21 (evening): 62 rows -> 3
   1165 target words, 1165 candidate words, 3 aligned differences, and
   ovlverify agrees (3 differing, masked). THE FRAME IS IDENTICAL: 0x40,
   the same nine callee-saved saves, the same 0x10 local area.

   WHAT IS LEFT -- ONE REGISTER, THREE ROWS, 80089DD4..80089DE0
       ours    sra $s6, $v0, 16 / bgtz $s6 / bgez $s6
       target  sra $v1, $v0, 16 / bgtz $v1 / bgez $v1
   `r1' (the `(s16)func_80042900(e, 7)' result) wants $v1 and gets $s6.
   `n1' (the 0x80/0x100/0x200 fork) is correct, in $v0.
   DIAGNOSIS, read off `.greg' and verified against global.c -- see
   "THE LAST THREE ROWS" below. It is a `hard_reg_preferences' question,
   not a shape question, and both halves of it are understood.

   WHAT THIS SESSION CHANGED, each measured on the body before it
     1.  t4: `t4 = *(u16 *)(fA + 2);' split out ABOVE `f13 = fA;', with
         the store still `*(u16 *)(f13 + 2) = t4 | one;'            62 -> 58
         The target loads through fA ($v1) and stores through f13 ($s3);
         with the read-modify-write on f13 the load has to wait for the
         copy. A FRESH local is required: with `t1' (a retry-loop local)
         it is 66, and `*(u16 *)(fA + 2) |= one;' alone is byte-identical
         to the park's `f13' spelling (cse coalesces fA and f13).
     2.  `if (((s32 *)s1)[4] > 0)' instead of `*(s32 *)(s1 + 0x10) > 0'
         in the nested callback loop                                58 -> 52
     3.  the same in the second callback loop                       52 -> 47
         MECHANISM, and the single biggest lever in this park, verified
         line by line against scratch/gcc-src/cdk-gcc-b18: expand_expr's
         INDIRECT_REF arm sets MEM_IN_STRUCT_P when
         `TREE_CODE (TREE_OPERAND (exp, 0)) == PLUS_EXPR'
         (expr.c:5343, the flag at :5351). In `*(s32 *)(s1 + 0x10)' the
         top of the address tree is the CAST -- a NOP_EXPR -- so the flag
         is NOT set; in `((s32 *)s1)[4]' the cast is on the pointer and
         the PLUS_EXPR is on top, so it IS. The two spell the same byte
         address and the same mode; only the tree shape differs.
         true_dependence (sched.c:835) then DROPS the dependence between
         the `D_800E3DE8 = ...' store and this load, because its last
         clause asks for exactly this: the load in-struct with a varying
         address and non-QImode, the store neither -- and the store's
         `(lo_sum (reg) (symbol_ref))' address does NOT vary, since
         rtx_varies_p returns `rtx_varies_p (XEXP (x, 1))' for a LO_SUM
         (rtlanal.c:104) and operand 1 is the SYMBOL_REF. sched1 floats
         the `lw 0x10($s0)' above the store, the store lands in the
         `blez' delay slot -- which is where the target has it -- and the
         load-delay `nop' our version needed disappears. In the second
         loop it ALSO moves `s1 = q3 - 0x20' out of the loop top into the
         preheader and the closing branch's delay slot, which the park's
         old diagnosis had listed as a separate 4-row problem.
         Hoisting the load into a local instead (`t4 = *(s32 *)(s1+0x10);')
         buys the same delay slot and costs 8 rows, because the `or' that
         builds fn1 then writes a different register.
     4.  `r5' DELETED. The 0x8A store and the func_800BCB04 argument both
         read `*(u16 *)(e + 0x88)' inline, AND `e++; e--;' removed. Both
         halves together, never one alone                           47 -> 42
         This is also what closed the WORD GAP: steps 2 and 3 each delete
         a `nop', and the pair restores a word by giving `e' one more
         reference. `r5' kept with the recompute is 72 rows (the extra
         `e' reference moves `c' from $s5 to $s7); `e++; e--;' kept with
         the recompute is 44.
     5.  `if (ta) { fn1(q3,...) } else { ... }' removed -- inert (42)
     6.  `ta++; ta--;' after `D_800E3DE8 = *(u8 **)(q3 + 0x5C);' in the
         nested callback loop                                       42 -> 40
         (REMOVED AGAIN at the end: inert on the 3-row body, so the
         body carries three devices, not four)
     7.  the retry loop's walker `q8' DELETED and `e' reused for it,
         alone this is 40 -> 46, but it is what puts the walker in $s4
     8.  `w2++; w2--;' immediately after `w2 = D_800E3D7C;'         46 -> 18
         7 and 8 are ONE change: `q8' has 55 references over 89 insns
         against `w2's 9 over 54, so allocno_compare (global.c:597)
         prices q8 at 30898 and w2 at 5000 and q8 takes $s2 whatever you
         do to w2; reusing `e' (dead by the retry loop) pins the walker
         to $s4 by making it the same allocno, and the cancelled pair
         then buys w2 the $s2 that q8 vacated.
     9.  ALL FOUR walk sentinels as ONE local `ta', the nested loop's
         included -- all 15 partitions re-measured on this body, every
         other one is 14-18                                         18 -> 12
    10.  the nested callback loop written ROTATED, guard on `e' and back
         edge on `ta': `if (q3 != e) { do {...} while (q3 != ta); }',
         which is the target's `beq $s1,$s4' / `bne $s1,$s2' pair  12 -> 11
    11.  the nested loop's `cnt' and the retry loop's `i' are ONE local,
         which puts the counter in $s6 and the hoisted %hi(D_800E3DE8)
         in $s3                                                     11 -> 5
    12.  `s32 flagB[1]', not `flagB[2]'                              5 -> 3
         The retry flag's array is ONE word. [2] leaves the loop's
         `lw 0x10($sp)' in $v1 where the target has $t1; [3] and [4] are
         36 rows. The 0x10($sp) offset and the 0x40 frame are unchanged.

   THE LAST THREE ROWS -- WHY `r1' IS IN $s6, AND WHY EVERY OTHER SHAPE
   IS WORSE. Two independent things have to be true at once and no
   spelling found so far does both.

   (a) sched1 HOISTS `n1 = 0x80' above the `sll $v0, $v0, 16'. With the
       plain `r1 = (s16)func_80042900((s8 *)e, 7);' the `.greg' line reads
       `;; 116 conflicts: ... 2 ...' -- n1 conflicts with hard $v0 -- so
       n1 cannot have $v0 and takes $v1, and r1 takes $v0. Seven rows.
       reorg later moves the `addiu' back after the `sra', so the emitted
       POSITIONS are right and only the registers are wrong; the conflict
       is an artifact of the sched1 order, which is what global_conflicts
       prices.
   (b) The duplicated arm `if (r1) { r1 = call; } else { r1 = call; }'
       blocks that hoist (the join CODE_LABEL sits between the `sra' and
       `n1 = 0x80', and sched1 is a basic-block scheduler), so n1 keeps
       $v0. But the condition reads r1 BEFORE its only assignment, so
       flow makes r1 live from function entry -- `.lreg' says `used 5
       times across 144 insns; crosses 8 calls' -- and find_reg's
       `used1 = call_used_reg_set' then forbids every caller-saved
       register. Hence $s6.
       `if (i)', `if (k)' and `if (f2)' do NOT help, and the reason is
       worth writing down: their conditions FOLD (a `&SYM' pointer and a
       counter the compiler can resolve), the branch disappears, and the
       block is never split -- they measure exactly what no device at
       all measures. `if (v2)' / `if (x3)' / `if (n3)' really do split
       it, and then r1 is an ordinary global allocno that PREFERS $v0:
       set_preference (global.c:1545) takes the SET_SRC's FIRST OPERAND
       when the source is an expression (`if (GET_RTX_FORMAT
       (GET_CODE (src))[0] == 'e') src = XEXP (src, 0)', :1555), that
       operand is the `sll' temp, local-alloc has renumbered it to $v0,
       and the dump says `;; 115 preferences: 2'. r1 is lower priority
       than n1 and they conflict, so prune_preferences' merge loop
       (global.c:873-884) copies $v0 into `regs_someone_prefers[n1]',
       and find_reg's pass 0 ORs that into `used' (:956). n1 -> $v1,
       r1 -> $v0. Seven rows again.
       `if (r1)' is the ONLY condition that wins, because making r1
       call-crossing is also what makes prune_preferences DELETE its
       $v0 preference (`IOR_HARD_REG_SET (temp, call_used_reg_set)'
       at :855 then `AND_COMPL_HARD_REG_SET (hard_reg_preferences
       [allocno], temp)' at :862) -- which is what lets n1 keep $v0.
       The escape hatch the same loop offers is at :879: a register the
       allocno ITSELF prefers is removed from the merged set when the
       sizes are equal. So giving `n1' a $v0 preference of its own would
       also do it -- but all three of n1's sets are `(const_int ...)',
       set_preference wants a REG, and expand_preferences (:783) only
       merges across an insn whose SET_DEST is a REG, which the `sh'
       of n1 is not.

       SO THE FIX IS ONE OF: make r1 conflict with hard $v0 without
       crossing a call (then its preference is pruned the same way), or
       give n1 its own $v0 preference (prune_preferences does NOT exclude
       a register the allocno itself prefers, when the sizes are equal),
       or stop the `sll' temp being renumbered to $v0. None of the
       spellings below does any of them.

   MEASURED AND REJECTED ON THE 3-ROW BODY (all 1165 words unless noted)
     `if (X)' for the duplicated arm, all 50 locals: r1 is 3, the next
       best are i/k/f2 at 7 (they fold, so they do not even split the
       block), then v2/x3/n3 at 10, and it degrades to 170 for `if (n2)'
     no duplicated arm at all                                        7
     `t6 = call << 16; r1 = t6 >> 16;' (split shift, no arm)          7
       -- this blocks the hoist by itself, but combine folds the two
       shifts back into one sign_extend so the temp is recreated and
       still prefers $v0; adding `t6++; t6--;' in another arm to make
       it a global allocno does not change the preference              7
     `r1 = call; r1 = (s16)r1;'                                       7
     the fork as `goto n1done;' early exits (with the arm)            3 (inert)
     `one = 1;' moved after the `sh'                                  3 (inert)
     `do { n1 = 0x80; } while (0);' with the arm                      3 (inert)
     `n1++; n1--;' before or after the fork, with the arm             3 (inert)
     a duplicated arm on `n1 = 0x80' with any condition            9-33
     merging `n1' with any other s32 local                        3 (best) or worse
     merging `r1' with any other s32 local                        3 (best) or worse
     ALL FOUR SWEEPS ARE CLEAN ON THIS BODY, each run to completion with
     its own AZURE_SCRATCH on 2026-11-21:
       widthsweep.py (every width, every local AND parameter)
                                                    "no width change helps"
       stmtsweep.py  (18 movable assignments)       "no safe move helps"
       refsweep.py   (51 names x 23232 points)      "no cancelled pair helps"
       devsweep.py   (51 names x 228 statements)    "no wrapper helps"
     All four were re-run from scratch after EVERY change above, and the
     ORDER mattered: `tc++; tc--;' (change 6) is only findable once the
     ARRAY_REF changes are in, and `ta++; ta--;' went inert once the
     sentinels were merged.

   MEASURED AND REJECTED EARLIER IN THE SESSION, each on the body named
     tc merged into ta, on the 62-row body                           69
     `tc = e;' moved inside the outer do-loop, 62-row body          107/119
     `tc = e;' moved inside, 47-row body                            107
     w2 merged into ta / into tc, 40-row body                       86 / 107
     the first retry do-loop's walker split off, 40-row body         40 (inert)
     cnt2 merged into cnt, 40-row body                               77
     cnt merged into i, 40-row body                                 104
       -- the SAME merge is worth 6 rows on the 11-row body. The park's
       old "{i, cnt} 185" is the same claim about a third body again.
     all 15 sentinel partitions, 47-row body                         47 (all inert)
     a duplicated arm on `s1 = D_800E3DE8;' in the nested loop,
       every condition, 12-row body                                  12-69
     `(s16)((u16)r5 - 0x20)', `(s16)((r5 & 0xFFFF) - 0x20)', a `u16'
       copy local, a chained `t = r5 = *(u16 *)(e + 0x88);', a read
       back of `*(s16 *)(e + 0x8A)' and an ARRAY_REF read of it: all
       byte-identical on the 47-row body -- none of them produces the
       target's `addu $a2, $v1, $zero'. Deleting `r5' (change 4) is
       what produces that word, as a second `lhu'.

   WHAT IS SETTLED, AND WHAT EACH STEP WAS WORTH (543 -> 87 -> 3)
   The numbered list below is the 2026-11-20 reconstruction's and is
   still correct except where this session's list above supersedes it
   (items 1, 5 and the walker/sentinel grouping).
   1. `s32 flagB[N]' used as `flagB[0]' IS the retry flag of the final
      loop, and it gives the target's 0x40 frame with the 8-byte local
      area at sp+0x10. All twenty sw/lw offsets match. (:28057.)
      N IS 1, NOT 2 -- see change 12 above.
   2. ONE LOCAL PER REGION, NOT ONE REUSED LOCAL PER ROLE: 543 -> 298.
      Reusing five pointer locals (f/q/s/w/k) across the whole function
      made each ONE pseudo with a 1200-1300-insn live range crossing
      36-43 calls, and the whole callee-saved assignment came out
      permuted. Thirteen separate &D_80083460 locals, one per point where
      the target rematerialises the base, is what fixes it.
   3. EACH `while (q != e)' WALK NEEDS ITS OWN SENTINEL LOCAL `t = e;':
      200 -> 169 and +4 words. (But the FOUR walk sentinels are now one
      SHARED local -- see change 9. The rule that survives is "a sentinel
      local, not a bare `e'", not "one per walk".)
   4. THE FINAL RETRY LOOP MUST BE A REAL `for (;;)', NOT A BACKWARD
      `goto': 200 -> 169, and it is what puts a `high' in a register for
      the whole loop. gcc only emits NOTE_INSN_LOOP_BEG for a loop
      STATEMENT. The two inner back edges are `continue'.
   5. WALKER GROUPING -- superseded. The current grouping is: the four
      walk sentinels one local `ta' (change 9), the retry loop's walker
      the parameter copy `e' (change 7), the three function-pointer temps
      one local `fn1', the two func_800BCB04 results one local `r6',
      {nz, i, cnt} one counter `i' (change 11).
   6. THE TWO BIT TESTS AT 8008A79C AND THE TWO AT 8008ABA4 MUST BE
      NESTED `if's, NOT `&&'. `(x & 0x400) == 0 && (x & 0x2000) != 0'
      folds to one `andi 0x2400' and is three words short.
   7. The 0x80/0x100/0x200 fork is NOT an if/else-if chain -- that gives
      `bgtz' then `bltz'. The target's `bgtz'/`bgez' pair is the
      early-exit form:
          n1 = 0x80; if (r1 <= 0) { n1 = 0x100; if (r1 < 0) n1 = 0x200; }
   8. THE RETRY LOOP'S BASE IS TWO LOCALS, `fA' before the loop and
      `f13 = fA' for the loop body: 94 -> 92 AND it is the last two words.
   9. THE 0xFBFF VALUE IN THE TAIL NEEDS ITS OWN LOCAL (92 -> 87).
  10. `s32 func_8009FB34_i(u16, u16) __asm__("func_8009FB34");' -- the
      unit's declaration returns s16, which costs a `sll 16'/`sra 16'
      pair the target does not have. Two words.
  11. `*(u16 *)0x80013714', the raw constant address, at all six sites.
  12. The func_8003DB6C destination is `(u8 *)&D_80083460 + 8', NOT the
      D_80083468 symbol: the relocation has to be against D_80083460
      with +8 in the addend for cse's related_value to give
      `addiu $s0, $s3, -0x8' and `sh $v0, -0x8($s3)' off the same base.
  13. D_800E4948 is the 4-word SNAPSHOT that call writes, written
      symbolically as `D_800E4948[0..2]' plus `*(s16 *)&D_800E4948[3]'.
  14. `addiu $s3, $a0, 0x18' at 80089AD8 is a base local plus 0x18:
      `g = (u8 *)&D_80083160;' and `h = g + 0x18;'.
  15. `if ((*(s32 *)(e + 0x1C) & 0x20) == 0) { ... } else { |= 0x6000; }'
      -- the 0x6000 arm is the BRANCH TARGET, so it is the else.

   SHAPES I AM NOT HAPPY ABOUT, WITH THEIR DEVICE-FREE COST
     * `if (r1) { r1 = call; } else { r1 = call; }' reads r1 before its
       only assignment. Device-free: 7 rows (and see "(a)" above).
     * the retry loop reuses the view pointer `e' as its list walker.
       `e' is provably dead there (nothing after the loop reads it, and
       every path into the loop body assigns it first), and the target's
       $s4 says the original source did the same -- but a separate `q8'
       local is 110 rows, so this is not a free choice.
     * `w2++; w2--;' is a cancelled pair and compiles to nothing.
       Device-free: 111 rows.
     * `r7++; r7--;' (device-free: 10 rows) and
       `if (f13) { s3 = D_800E3DE8; } else { s3 = D_800E3DE8; }'
       (device-free: 67 rows and one word short) are the 2026-11-20/21
       devices and both still earn their keep. EVERY device in the body
       was re-ablated on EVERY intermediate body; `if (ta)' went inert at
       47 rows and `ta++; ta--;' went inert at 3, and both were removed.
     * the rotated nested loop (change 10) is worth 1 row;
       `while (q3 != ta)' is the honest alternative at 4 rows.

   DECLARATIONS
   extern s32 D_800E4948[];
   extern s32 D_800DD160;
   extern s32 D_800E3D70;
   extern u16 D_800DD258;
   s32 func_80096100_4(u8 *, UnkVec *, u8 *, u8 *) __asm__("func_80096100");
   s32 func_8009FB34_i(u16, u16) __asm__("func_8009FB34");
   END DECLARATIONS

   AND ONE MORE, WHICH CANNOT GO IN THE BLOCK ABOVE BECAUSE
   tools/decomp/parkresidue.py CORRUPTS THE UNIT WHEN IT SEES IT. It is
   written below this comment instead, behind an empty comment:

       extern u16 D_800DD25A __asm__("D_800DD258 + 2");

   `adopt()'s PROTO regex reads `__asm__' as the declared NAME on an
   `extern ... __asm__("...")' line; it then searches the unit for that
   "name", matches
   `extern UnkDungeon82EB0 D_80082EB0_m __asm__("D_80082EB0");'
   and REPLACES it, and the compile fails with `D_80082EB0_m undeclared'
   7000 lines away. A leading comment on the line stops PROTO matching and
   the declaration still reaches the compiler. Worth fixing in the tool:
   PROTO should not match a line that starts with `extern'.

   PROTOTYPE CHANGE ON LANDING: none. func_80096100 is declared in the
   unit with ONE parameter and func_8009FB34 with an s16 return; both
   call sites go through the `__asm__' aliases above, so neither landed
   definition is touched.
*/
extern u16 D_800DD25A __asm__("D_800DD258 + 2");
void func_80089AA0(u8 *p, UnkVec *c, u8 *d)
{
    s32 one;
    u8 *e;
    u8 *g;
    u8 *h;
    u8 *v1;
    u8 *f1;
    u8 *k;
    u8 *f2;
    u8 *f3;
    u8 *f4;
    u8 *f5;
    u8 *f6;
    u8 *f7;
    u8 *f8;
    u8 *f9;
    u8 *f10;
    u8 *f11;
    u8 *f12;
    u8 *fA;
    u8 *f13;
    u8 *v2;
    u8 *q1;
    u8 *q2;
    u8 *q3;
    u8 *s1;
    u8 *s3;
    u8 *w2;
    void (*fn1)(u8 *, s32, s32);
    s32 x0;
    s32 x1;
    s32 x2;
    s32 x3;
    s32 r1;
    s32 n1;
    s32 n2;
    s32 r2;
    s32 n3;
    s32 fi;
    s32 r3;
    s32 mm;
    s32 r6;
    s32 r7;
    s32 i;
    s32 cnt2;
    s32 t1;
    s32 t2;
    s32 t3;
    u8 *ta;
    s32 t4;
    s32 flagB[1];

    g = (u8 *)&D_80083160;
    e = p;
    h = g + 0x18;
    if ((*(u16 *)0x80013714 & 4) != 0 && D_800E3CD0 == 0 && *(s32 *)(g + 0x10) != 0)
    {
        D_800E3CD0 = 1;
        func_80040AA0(3);
        return;
    }
    if (D_80080AA0 != 0 && D_800E3CD0 == 0 && func_80053EF0(1) == 0)
    {
        func_80053DA8(0x200);
        D_80080AA0 = 0;
        D_80080A88 = 1;
    }
    if ((*(u16 *)0x80013714 & 4) != 0)
    {
        v1 = D_800DCE60;
        *(s16 *)(v1 + 4) = *(s16 *)(v1 + 4) + (((D_80081468[0] & 1) != 0) ? 8 : -8);
        D_800832B4 = 0;
        func_8004D7A8(one);
        func_8004D294(NULL, D_800DCE60, 2);
    }
    *(s16 *)(p + 0x9E) = (*(u16 *)(h + 0xB0) >> 9) & 7;
    f1 = (u8 *)&D_80083460;
    *(u16 *)(f1 + 2) &= 0xFFCF;
    x0 = *(s32 *)(f1 + 8);
    if (x0 == D_800E4948[0] && (x1 = *(s32 *)(f1 + 0xC)) == D_800E4948[1] &&
        (x2 = *(s32 *)(f1 + 0x10)) == D_800E4948[2] &&
        (x3 = *(s16 *)(f1 + 0x14)) == *(s16 *)&D_800E4948[3] &&
        (*(u16 *)0x80013714 & 8) == 0 && p[0x9A] != 0x17 && p[0x9A] != 0x27 &&
        p[0x9A] != 0x29 && D_800DCF4F == 0 &&
        (x1 != 0 || x2 != 0 || x3 != 0 || x0 != 0))
    {
        D_800DD160++;
    }
    else
    {
        D_800DD160 = 0;
    }
    k = (u8 *)&D_80083460 + 8;
    func_8003DB6C(D_800E4948, (int *)k, 4);
    f2 = (u8 *)&D_80083460;
    if (*(s32 *)(f2 + 0xC) == 0 && *(s32 *)(f2 + 0x10) == 0 && (*(u16 *)(f2 + 2) & 8) == 0 &&
        *(s16 *)(f2 + 8) == 0 && *(s16 *)(f2 + 0x14) == 0 && (D_800E296C & 0x100000) == 0 &&
        *(s16 *)(f2 + 0xA) == 0 && (*(u16 *)(f2 + 2) & 0x2E01) == 0)
    {
        if ((s16)func_8009F5CC() != 0)
        {
            func_800C6AB0();
        }
        if ((*(u16 *)(f2 + 2) & 2) != 0)
        {
            *(u16 *)(f2 + 2) = (*(u16 *)(f2 + 2) & 0xFFFD) | 0x20;
            func_800A0DD8();
            if (r1)
            {
                r1 = (s16)func_80042900((s8 *)e, 7);
            }
            else
            {
                r1 = (s16)func_80042900((s8 *)e, 7);
            }
            n1 = 0x80;
            if (r1 <= 0)
            {
                n1 = 0x100;
                if (r1 < 0)
                {
                    n1 = 0x200;
                }
            }
            one = 1;
            *(s16 *)(k - 8) = n1;
            if ((*(u16 *)0x80013714 & 8) == 0)
            {
                f3 = (u8 *)&D_80083460;
                *(u16 *)(f3 + 0x1E) = *(u16 *)(f3 + 0x1E) + 1;
                D_800E3D70 += e[0x29];
                n2 = e[0x28];
                while ((u32)D_800E3D70 > 0x100)
                {
                    D_800E3D70 -= 0x100;
                    n2++;
                }
                if (e[0x29] < n2)
                {
                    n2 = e[0x29];
                }
                e[0x28] = n2;
                for (i = 1; i >= 0; i--)
                {
                    q1 = *(u8 **)(p + 0xAC + i * 4);
                    if (q1 != NULL && q1[0x25] != 0)
                    {
                        D_800E3D74[i] += q1[0x29];
                        n2 = q1[0x28];
                        while ((u32)D_800E3D74[i] > 0x100)
                        {
                            D_800E3D74[i] -= 0x100;
                            n2++;
                        }
                        if (q1[0x29] < n2)
                        {
                            n2 = q1[0x29];
                        }
                        q1[0x28] = n2;
                    }
                }
            }
            if ((*(s32 *)(e + 0x1C) & 0x800) != 0)
            {
                r2 = *(u16 *)(p + 0xF8);
                *(s16 *)(p + 0xF8) = r2 + one;
                n3 = (func_800644B8((s16)r2) >> 8) + 0x30;
            }
            else
            {
                r2 = *(u16 *)(p + 0xF8);
                *(s16 *)(p + 0xF8) = r2 + one;
                n3 = (func_800644B8((s16)r2) >> 7) + 0x40;
            }
            e[3] = n3;
            *(s32 *)(p + 0x128) = func_800A6D30() & 0xFFFF;
            if ((*(u16 *)0x80013714 & 8) == 0)
            {
                *(s16 *)(p + 0x118) = *(u16 *)(p + 0x118) + one;
            }
            func_800A4ACC(e);
            if (*(s32 *)(p + 0x8C) == (s32)func_80096384)
            {
                if ((s16)func_80042900((s8 *)e, 0xA) == 0)
                {
                    *(s32 *)(p + 0x8C) = (s32)func_8008ACDC;
                    f4 = (u8 *)&D_80083460;
                    *(u16 *)(f4 + 2) |= 4;
                }
            }
            else if (*(s32 *)(p + 0x8C) == (s32)func_8008ACDC)
            {
                if ((s16)func_80042900((s8 *)e, 0xA) != 0)
                {
                    *(s32 *)(p + 0x8C) = (s32)func_80096384;
                    f5 = (u8 *)&D_80083460;
                    *(u16 *)(f5 + 2) |= 4;
                }
            }
        }
        f6 = (u8 *)&D_80083460;
        *(u16 *)(f6 + 2) &= 0xBFFB;
        *(s32 *)(e + 0x14) &= 0x7FFE7FFF;
    }
    else
    {
        f7 = (u8 *)&D_80083460;
        *(u16 *)(f7 + 2) |= 4;
    }
    if ((*(u16 *)0x80013714 & 8) == 0)
    {
        if (p[0x9A] != 0x17 && D_800DCF4F == 0 && (u32)D_800DD160 >= 0x385)
        {
            q2 = D_800E3D7C;
            D_800DD160 = 0;
            do
            {
                q2[0x71] &= 0x7F;
                *(s16 *)(q2 + 0x6C) = 0;
                q2 = *(u8 **)(q2 + 0x5C) + 0x20;
            } while (q2 != D_800E3D7C);
            f8 = (u8 *)&D_80083460;
            *(s16 *)(f8 + 8) = 0;
            *(s16 *)(f8 + 0xA) = 0;
            *(s16 *)(f8 + 0x14) = 0;
            *(s32 *)(f8 + 0xC) = 0;
            *(s32 *)(f8 + 0x10) = 0;
            *(u16 *)(f8 + 2) = 0;
        }
        if ((*(u16 *)0x80013714 & 8) == 0)
        {
            if ((D_80083462 & 4) == 0 || (D_800E296C & 0x40) != 0 || p[0x9A] == 0x17)
            {
                D_800E296C &= ~0x40;
                v2 = D_800DCE60;
                if ((s16)func_800C77D0(e - 0x20, c, 8, *(s16 *)(v2 + 6)) != 0)
                {
                    D_800832B4 = 0;
                    func_8004D7A8(1);
                    func_8004D294(NULL, v2, 2);
                }
                else
                {
                    D_800E3D20 = 0;
                }
            }
        }
    }
    if ((*(s32 *)(e + 0x1C) & 0x20) == 0)
    {
        if ((s16)func_80042900((s8 *)e, 0xA) != 0)
        {
            func_800478B8(d);
        }
        else
        {
            func_80048AC8(d, one);
        }
    }
    else
    {
        *(u16 *)(d + 0x14) |= 0x6000;
    }
    if (*(s32 *)(p + 0x8C) != 0)
    {
        (*(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C))(p, c, d, e);
    }
    D_800DD168[p[0x9A]](p, c, d, e);
    fi = (((D_80083228 + *(s16 *)(e + 0x2A)) + 0x100) >> 9) & 7;
    if (*(s16 *)(p + 0x94) != fi)
    {
        if ((s16)func_80042900((s8 *)e, 0xA) != 0)
        {
            func_8003DB94(d, *(void **)(*(s32 *)(d + 0x2C) + fi * 4), *(s8 *)(d + 4));
        }
        else
        {
            func_800489F4(d, *(*(u8 **)(d + 0x2C) + fi), *(s8 *)(d + 4), one);
        }
        *(s16 *)(p + 0x94) = fi;
        if (p[0x9A] != 0x22 && D_8006CCF8[fi] != 0)
        {
            *(u16 *)(d + 0x14) |= one;
        }
        else
        {
            *(u16 *)(d + 0x14) &= 0xFFFE;
        }
    }
    r3 = func_8009FB34_i(d[0x24], d[0x25]);
    d[0x26] = r3;
    mm = (s8)r3;
    if (mm >= 0)
    {
        D_800E2970_s[mm].unk_0C |= one;
    }
    func_800A020C(*(s32 *)(e + 0x1C), d + 0xC);
    if ((*(s32 *)(e + 0x14) & 0x100000) == 0)
    {
        c->unk_00 += c->unk_0C;
        c->unk_04 += c->unk_10;
        if ((*(u16 *)(p + 0x98) & 8) == 0)
        {
            c->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
            p[0x9D] = p[0x9D] + one;
        }
        *(s32 *)(p + 0x90) += c->unk_14;
        r6 = (s16)func_800BCB04(*(u16 *)((u8 *)c + 2), *(u16 *)((u8 *)c + 6),
                                (s16)(*(u16 *)(e + 0x88) - 0x20));
        if (r6 < 0x200)
        {
            if (*(s16 *)(p + 0x92) + *(s16 *)(e + 0x88) >= r6)
            {
                if ((*(u16 *)(p + 0x98) & 4) != 0 || c->unk_14 < 0)
                {
                    *(u16 *)(p + 0xA2) &= 0xFFEF;
                }
                else
                {
                    *(s32 *)(p + 0x90) = 0;
                    c->unk_14 = 0;
                    p[0x9D] = 0;
                    if ((*(u16 *)(p + 0xA2) & 0x10) == 0)
                    {
                        if ((D_800E296C & 0x80000) == 0)
                        {
                            func_800A56E0(0x508);
                        }
                        func_800B66C8((u8 *)c);
                    }
                    *(u16 *)(p + 0xA2) |= 0x10;
                }
                *(u16 *)(p + 0xA2) &= 0xF7FF;
            }
            else
            {
                if ((*(u16 *)(p + 0xA2) & 0x800) != 0)
                {
                    *(u16 *)(p + 0xA2) &= 0xFFEF;
                }
                *(u16 *)(p + 0xA2) |= 0x800;
            }
            if ((*(s32 *)(e + 0x1C) & 0x40000000) != 0)
            {
                *(s32 *)(e + 0x1C) &= 0xBFFFFFFF;
                *(s16 *)(e + 0x8A) = *(u16 *)(e + 0x88);
                r6 = (s16)func_800BCB04((d[0x24] << 6) | 0x20, (d[0x25] << 6) | 0x20,
                                        (s16)(*(u16 *)(e + 0x88) - 0x20));
                *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + (*(u16 *)(e + 0x88) - r6);
                *(s16 *)(e + 0x88) = r6;
                if ((D_80083462 & 0x80) != 0)
                {
                    *(s32 *)(p + 0x90) = 0;
                    c->unk_14 = 0;
                    p[0x9D] = 0;
                    *(u16 *)(p + 0xA2) |= 0x10;
                }
            }
        }
        else
        {
            if ((*(u16 *)(p + 0xA2) & 0x800) != 0)
            {
                *(u16 *)(p + 0xA2) &= 0xFFEF;
            }
            *(u16 *)(p + 0xA2) |= 0x800;
        }
        *(s16 *)((u8 *)c + 0xA) = *(u16 *)(e + 0x88) + *(u16 *)(p + 0x92);
    }
    func_80096100_4(p, c, d, e);
    if ((*(s32 *)(e + 0x14) & 0x100000) == 0)
    {
        if (D_800DD258 != d[0x24] || D_800DD25A != d[0x25])
        {
            func_8009D3B0();
            if ((s16)func_800A2C34(NULL) == 0)
            {
                if ((s16)func_800A4474(d[0x24], d[0x25]) != 0)
                {
                    *(u16 *)(p + 0xA2) |= 0x200;
                }
                else
                {
                    *(u16 *)(p + 0xA2) &= 0xFDFF;
                }
            }
        }
        q3 = func_8009B25C(e, d[0x24], d[0x25], *(s16 *)(e + 0x88));
        if (q3 != NULL)
        {
            func_8009A180(q3, (u8 *)D_800814A8);
            *(s32 *)(q3 + 0x1C) &= 0x7FFFFFFF;
        }
        D_800DD258 = d[0x24];
        D_800DD25A = d[0x25];
    }
    q3 = *(u8 **)(e + 0x5C) + 0x20;
    ta = e;
    while (q3 != ta)
    {
        *(u16 *)(*(u8 **)(q3 - 0x14) + 0x14) &= 0xFFBF;
        q3 = *(u8 **)(q3 + 0x5C) + 0x20;
    }
    f9 = (u8 *)&D_80083460;
    if ((*(u16 *)(f9 + 2) & 0x800) != 0 && (s16)func_800A2C34(NULL) == 0 &&
        *(s16 *)(f9 + 8) == 0 && *(s16 *)(f9 + 0x14) == 0)
    {
        *(u16 *)(f9 + 2) = (*(u16 *)(f9 + 2) & 0xF7FF) | 0x2000;
    }
    f10 = (u8 *)&D_80083460;
    if ((*(u16 *)(f10 + 2) & 0x400) == 0)
    {
        if ((*(u16 *)(f10 + 2) & 0x2000) != 0)
        {
            if ((s16)func_800A2C34(NULL) == 0)
            {
                q3 = *(u8 **)(e + 0x5C) + 0x20;
                ta = e;
                while (q3 != ta)
                {
                    *(s16 *)(q3 + 0x8A) = *(u16 *)(q3 + 0x88);
                    *(s16 *)(q3 + 0x6A) = *(u16 *)(q3 + 0x2A);
                    q3 = *(u8 **)(q3 + 0x5C) + 0x20;
                }
                ta = e;
                do
                {
                    s1 = *(u8 **)(e + 0x5C);
                    q3 = s1 + 0x20;
                    i = 0;
                    if (q3 != e)
                    {
                        do
                        {
                            D_800E3DE8 = *(u8 **)(q3 + 0x5C);
                            if (((s32 *)s1)[4] > 0)
                            {
                                fn1 = (void (*)(u8 *, s32, s32))(*(u32 *)(s1 + 0x10) | 0x80000000);
                                fn1(s1 + 0x20, *(s32 *)(s1 + 8), *(s32 *)(s1 + 0xC));
                                if ((*(u16 *)(s1 + 0x1E) & 0x8000) == 0 && *(s8 *)(q3 + 0x71) < 0 &&
                                    *(s8 *)(q3 + 0x6D) != 0)
                                {
                                    i++;
                                }
                            }
                            s1 = D_800E3DE8;
                            q3 = s1 + 0x20;
                        } while (q3 != ta);
                    }
                } while (i != 0);
                q3 = *(u8 **)(e + 0x5C) + 0x20;
                ta = e;
                while (q3 != ta)
                {
                    func_800A9AFC(*(u8 **)(q3 - 0x14), q3);
                    q3 = *(u8 **)(q3 + 0x5C) + 0x20;
                }
                f11 = (u8 *)&D_80083460;
                *(u16 *)(f11 + 2) = (*(u16 *)(f11 + 2) & 0xDFFF) | 0x1000;
            }
            else
            {
                *(u16 *)(f10 + 2) &= 0xDFFF;
                q3 = *(u8 **)(e + 0x5C) + 0x20;
                while (q3 != e)
                {
                    s1 = q3 - 0x20;
                    D_800E3DE8 = *(u8 **)(q3 + 0x5C);
                    if (((s32 *)s1)[4] > 0)
                    {
                        fn1 = (void (*)(u8 *, s32, s32))(*(u32 *)(s1 + 0x10) | 0x80000000);
                        fn1(q3, *(s32 *)(s1 + 8), *(s32 *)(s1 + 0xC));
                    }
                    q3 = D_800E3DE8 + 0x20;
                }
                f12 = (u8 *)&D_80083460;
                *(u16 *)(f12 + 2) |= 0x2000;
            }
        }
    }
    fA = (u8 *)&D_80083460;
    i = 0;
    if (*(s32 *)(fA + 0x10) > 0)
    {
        *(s32 *)(fA + 0x10) = 0;
    }
    flagB[0] = 0;
    t4 = *(u16 *)(fA + 2);
    f13 = fA;
    *(u16 *)(f13 + 2) = t4 | one;
    for (;;)
    {
        if ((D_800E296C & 0x100000) != 0 && (s16)func_800A2C34(NULL) == 0)
        {
            e = D_800E3D7C;
            do
            {
                n2 = (e[0x41] << 8) | e[0x42];
                e[0x42] = 0;
                e[0x41] = 0;
                *(s32 *)(e + 0x18) += n2;
                if ((s16)func_800A1C58(e) != 0)
                {
                    *(u8 **)(f13 + 0xC) = e;
                    goto walk;
                }
                e = *(u8 **)(e + 0x5C) + 0x20;
            } while (e != D_800E3D7C);
            D_800E296C &= 0xFFEFFFFF;
        }
    walk:
        w2 = D_800E3D7C;
        w2++;
        w2--;
        s3 = *(u8 **)(w2 + 0x5C);
        e = s3 + 0x20;
        cnt2 = 0;
        while (e != w2)
        {
            D_800E3DE8 = *(u8 **)(e + 0x5C);
            if (flagB[0] != 0 && *(s8 *)(e + 0x6D) == 0 && *(s16 *)(f13 + 0x14) == 0)
            {
                goto next;
            }
            if (*(s32 *)(s3 + 0x10) <= 0)
            {
                goto next;
            }
            r7 = (s16)func_800A2C34(NULL);
            r7++;
            r7--;
            if (*(s16 *)(f13 + 0x14) != 0)
            {
                if (r7 == 0)
                {
                    if ((*(u32 *)(s3 + 0xB8) & 0xFFFF0000) != 0x150000)
                    {
                        goto next;
                    }
                    *(u8 **)(f13 + 0xC) = s3 + 0x20;
                }
            }
            fn1 = (void (*)(u8 *, s32, s32))(*(u32 *)(s3 + 0x10) | 0x80000000);
            fn1(s3 + 0x20, *(s32 *)(s3 + 8), *(s32 *)(s3 + 0xC));
            if ((*(u16 *)(s3 + 0x1E) & 0x8000) != 0)
            {
                break;
            }
            if (*(s8 *)(e + 0x6D) != 0)
            {
                cnt2++;
            }
            if (r7 != 0 && (s16)func_800A2C34(NULL) == 0)
            {
                break;
            }
        next:
            if (f13)
            {
                s3 = D_800E3DE8;
            }
            else
            {
                s3 = D_800E3DE8;
            }
            e = s3 + 0x20;
        }
        t1 = *(u16 *)(f13 + 2);
        if ((t1 & 0x1000) != 0)
        {
            if (*(s16 *)(f13 + 8) == 0)
            {
                if (*(s16 *)(f13 + 4) != 0)
                {
                    return;
                }
                *(u16 *)(f13 + 2) = (t1 & 0xEFF7) | 0x4000;
                if (cnt2 != 0)
                {
                    flagB[0] = one;
                    continue;
                }
            }
        }
        else if ((t1 & 0x400) != 0)
        {
            if ((t1 & 0x800) == 0 && (s16)func_800A2C34(NULL) == 0 &&
                (s16)func_800A2B28() == 0)
            {
                if (i == 0)
                {
                    i = one;
                    continue;
                }
                t2 = *(u16 *)(f13 + 2);
                t3 = t2 & 0xFBFF;
                *(u16 *)(f13 + 2) = t3;
                if ((t2 & 0x4000) != 0)
                {
                    return;
                }
                *(u16 *)(f13 + 2) = t3 | 0x800;
                if (func_800A5C70() != 0)
                {
                    *(u16 *)(f13 + 2) |= 0x80;
                }
                return;
            }
        }
        if (e == D_800E3D7C && cnt2 == 0)
        {
            *(u16 *)(f13 + 2) &= 0xFFFE;
        }
        t1 = *(u16 *)(f13 + 2);
        if ((t1 & 0x80) == 0 || *(s32 *)(f13 + 0xC) != 0 || *(s32 *)(f13 + 0x10) != 0 ||
            *(s16 *)(f13 + 0xA) != 0 || *(s16 *)(f13 + 8) == 0)
        {
            break;
        }
        *(u16 *)(f13 + 2) = t1 & 0xFFF7;
    }
}