/* func_s10_80024930 -- PARKED at 243 words / 243 words / 14 aligned
   differences (ovlverify reports 14 differing; aligndiff agrees, so no word
   is displaced).  Single-member family (dngfam.py: 1 member, 243 words), so
   landing it lands only itself.  relcand.py: ok, 1 data symbol, 6 calls.

   WHAT IT IS.  The sixteen-segment semi-transparent Gouraud-textured ring
   this effect is drawn as: one 0x34-byte POLY_GT4 per 1/16 of a turn taken
   off D_80083160.unk_00 + 0x8D0, its four corners staged as SVECTORs in the
   0x1F800000 scratchpad at +0x64 (stride 8), projected with RotAverage4 into
   the scratchpad pairs at +0xD8..+0xE7, and hung on the OT row RotAverage4
   returns when that row is below 0x1E0.  Eight parameters; the first and the
   third are never read.

   NO TWIN BY EITHER TOOL.  dngtwin.py --unit ovl_dng_s10 reports no carved
   sibling at ratio 0.70 for this stub, and xsibs.py is useless on an s-unit
   (the fifth consecutive one).  The route that supplied every spelling was
   the callee-set grep: GetTPage / func_80065590 / D_1F800018 / D_1F8000D8
   intersected to csrc/ovl_dng_s17.c, csrc/ovl_dng_s46.c and
   csrc/ovl_dungeon_800D50FC.c, and the two bodies actually read for idiom
   are csrc/ovl_dng_s17.c:func_s17_80024188 (the scratchpad struct at
   0x1F800000, RotAverage4 out-parameters, the `(u32)x < 0x1E0' OT gate) and
   csrc/ovl_dungeon_800A4B88.c:func_800AFBFC (the chained
   `sp->unk_70[0].vx = sp->unk_70[2].vx = ...' corner assignment).
   csrc/ovl_dng_b02.c:func_b02_801750E4 is the nearest shape -- the same
   POLY_GT4 ribbon off the same +0x8D0 ring -- but it calls SetPolyGT4 and
   AddPrim as functions where this one has setPolyGT4 and addPrim inlined
   from psyq/include/libgpu.h.

   THE RESIDUE, exactly.  All 14 rows are two instances of ONE fault: in each
   of the two signed `% 0x1000' expansions the SUM pseudo and the CORRECTION
   pseudo hold each other's register.

     rows 84-98   target  addu $a0,$v0,$t1 / addu $a1,$a0,$zero /
                          bgez $a0 / addiu $a1,$a0,0xFFF /
                          sra $v0,$a1,12 / sll $v0,$v0,12 / subu $s3,$a0,$v0
                  mine    addu $a1,$v0,$t1 / addu $a0,$a1,$zero /
                          bgez $a1 / addiu $a0,$a1,0xFFF /
                          sra $s3,$a0,12 / sll $v0,$s3,12 / subu $s3,$a1,$v0
     rows 118-124 target  addu $v0,$v0,$t1 / bgez $v0 /
                          addu $s0,$v0,$zero / addiu $s0,$v0,0xFFF /
                          sra $s0,$s0,12 / sll $s0,$s0,12 / subu $s0,$v0,$s0
                  mine    addu $s0,$v0,$t1 / bgez $s0 /
                          addu $v0,$s0,$zero / addiu $v0,$s0,4095 /
                          sra $v0,$v0,12 / sll $v0,$v0,12 / subu $s0,$s0,$v0

   Nothing else differs: the frame is -0x58 with the same nine callee-saved
   saves in the same order and the same slots, the whole prologue, the
   prim-dressing block, the RotAverage4 argument build, the eight screen-pair
   copies, the inlined addPrim and the epilogue are word-identical, and q and
   the loop counter now hold the registers the target gives them.

   TWO FACTS WERE MEASURED AND BOTH ARE LOAD-BEARING.  They were found in
   this order and each reproduces when the other is removed.

   1. THE +0xA SCRATCHPAD READ-BACK ORDER -- the byte-dressing block must be
      clut, then the u/v pairs in setUV4 order, then the twelve colour bytes.
      This is the setUV4 macro's own order from psyq/include/libgpu.h
      (u0,v0,u1,v1,u2,v2,u3,v3) with setRGB0..3 after it, not u-bytes
      followed by v-bytes.  Measured, all other spellings at 243 words:

      | byte-dressing order                              | aligned |
      | clut, setUV4 pairs, colours   (kept)             |   14    |
      | setUV4 pairs, colours, clut                      |   19    |
      | setUV4 pairs, clut, colours                      |   19    |
      | u0..u3, colours, clut, v0..v3                    |   21    |
      | u0..u3, colours, v0..v3, clut                    |   24    |
      | u0..u3, v0..v3, colours, clut                    |   24    |
      | u0..u3, v0..v3, clut, colours                    |   24    |
      | colours, setUV4 pairs, clut                      |   27    |
      | colours, u0..u3, v0..v3, clut                    |   28    |

      The mechanism is sched1, not allocation.  With the u-bytes grouped and
      the v-bytes separate, the `li 0x3F' feeding u1 and u3 is scheduled
      AFTER the whole colour run and after the first modulo sum, so it has to
      share $v0 with the 0x7E00/0x80/0xBF chain; the target has it in $v1 at
      word 67 with its two stores at 69 and 71.

   2. `t2 = <sum>; t2 = t2 % 0x1000;' AS TWO STATEMENTS, and `t' as ONE.
      This is the asymmetric one and it is worth 41 rows:

      | t spelling              | t2 spelling             | aligned |
      | one expression (kept)   | two statements (kept)   |   14    |
      | one expression          | one expression          |   58    |
      | two statements          | two statements          |   98    |
      | two statements          | one expression          |   54    |
      | one expression          | three statements        |   22    |

      With t2 as one expression the exchange is not just inside the modulo:
      the cursor pointer q and the sign-extended loop counter swap $s0 and
      $s1 across the WHOLE body, which is 62 rows at 243 words.  q has 78
      references over 171 insns against the counter's 8 over 23, so
      allocno_compare ranks q first and it takes $s0; splitting t2 is what
      changes the counter allocno enough to reverse that.

   SPELLINGS MEASURED AND REJECTED, all at 243 words unless noted
   (aligned differences):

   | spelling                                                   | rows |
   | `Unk80083160 *gg = &D_80083160' and `gg->unk_00' INSIDE the
     loop, with `D_80083160.unk_00' inline outside it (kept)    |  14  |
   |   ... `D_80083160.unk_00' inline inside the loop too       | 242 words, 69 |
   | `q->tpage' etc. as POLY_GT4 FIELDS (kept)                  |  14  |
   |   ... `u8 *q' with plain casts at every site               |  14  |
   | `i << 8' for the first angle                               |  14  |
   |   ... `i * 0x100'                                          |  14  |
   | `((i + 1) % 0x10) * 0x100 + d' with d second               |  14  |
   |   ... `d + ((i + 1) % 0x10) * 0x100'                       |  14  |
   | `(i << 8) + d' with d second                               |  14  |
   |   ... `d + (i << 8)'                                       |  14  |
   | `t2 = t2 % 0x1000;'                                        |  14  |
   |   ... `t2 %= 0x1000;'                                      |  14  |
   | `s16 i' with the loop `for (i = 0; i < 0x10; i++)'         |  14  |
   |   ... `s32 i' with `(s16)i' at all three uses              | 241 words, 31 |
   | `s16 d' and `s16 f' PARAMETERS                             |  14  |
   |   ... `s32 d' parameter with an `s16 d' local              |  14  |
   |   ... `s32 f' parameter with an `s16 f' local              | 241 words, 34 |
   |   ... both as s32 parameters with s16 locals               | 241 words, 34 |
   | no device                                                  |  14  |
   |   ... `if (1)' around the `t' statement                    |  14  |
   |   ... `do { } while (0)' around the `t' statement          | 242 words, 104 |
   |   ... `do { } while (0)' around the two `t2' statements    | 242 words, 59 |
   |   ... `do { } while (0)' around `t2 = t2 % 0x1000;' alone  |  14  |
   | declaration order sp, gg, q, i, t, t2                      |  14  |
   |   ... q after t2                                           |  14  |
   |   ... gg last                                              |  14  |
   |   ... t2 before t                                          |  14  |
   |   ... i last                                               |  14  |
   | separate scratch local for the t2 sum                      |  62  |
   | separate scratch local for the t sum                       |  14  |
   | `t2' reused as the scratch for `t' as well                 | 245 words, 91 |
   | the t2 block written BEFORE the t block                    |  82  |

   The eleven rows that measure 14 unchanged are FREE, not evidence: the
   residue is bit-identical in every one of them, so none of those spellings
   is why the body is as close as it is.

   WHAT TO TRY NEXT.  The fault is `allocno_compare' inside two
   compiler-generated `%' expansions, so it is not reachable by respelling
   the modulo -- every form of it was priced above.  Run the permuter; this
   is exactly the residue it is for.  Failing that, `-da' on the two sums'
   allocnos: in this build the first modulo's sum is live 84..98 and its
   correction 85..95, and the target gives the LONGER-lived sum the lower
   register, which is the reverse of local-alloc's shorter-first rule, so
   something is adding a conflict on $a0 in the real source.

   DECLARATIONS
   typedef struct UnkDngS10QuadSP
   {
       u8 pad_00[0x18];
       u8 *unk_18;
       u8 pad_1C[0x48];
       SVECTOR unk_64[4];
       s32 unk_84;
       s32 unk_88;
       u8 pad_8C[0x28];
       s32 unk_B4;
       u8 pad_B8[0x20];
       u16 unk_D8;
       u16 unk_DA;
       u16 unk_DC;
       u16 unk_DE;
       u16 unk_E0;
       u16 unk_E2;
       u16 unk_E4;
       u16 unk_E6;
   } UnkDngS10QuadSP;
   s32 func_800644B8(s32);
   s32 func_80064584(s32);
   END DECLARATIONS */
/* ==== SECOND PASS, agent h148, batch 34. The author's section above is
   intact; this appends to it and CORRECTS ITS DIAGNOSIS AND TWO OF ITS
   TABLE ROWS. The body is unchanged -- nothing measured better than 14.

   THE FIGURE REPRODUCES EXACTLY. parksplice + aligndiff on the unit as it
   stands today: 243 words built, 243 target, 14 aligned differences, and the
   14 rows are the two blocks the author lists, instruction for instruction.
   The park splices and compiles; no rot.

   THE DIAGNOSIS IS NOT `allocno_compare'. It is `expand_divmod's' TARGET
   THREADING, decided at RTL expansion, long before any allocator runs.
   expmed.c:2815 reads

       if (target && GET_MODE (target) == compute_mode)
         tquotient = target;
       else
         tquotient = gen_reg_rtx (compute_mode);

   and `tquotient' is what the `sra' of the signed-%-by-power-of-2 expansion
   writes (expmed.c:3029). In THIS body `target' is the destination variable's
   own pseudo, so the quotient lands in it -- the `.lreg' dump has

       (insn 246 ... (set (reg/v:SI 97) (ashiftrt:SI (reg:SI 138) (const_int 12))))

   with reg 97 = `t', reg 137 = the sum and reg 138 = the +0xFFF correction.
   The target's listing puts that `sra' in a register that is NOT `t's'
   ($v0 against t's $s3), so in the real source `tquotient != target'.
   expmed.c:2758-2768 kills `target' when `rem_flag' and
   `reg_mentioned_p (target, op0)' -- i.e. when the DIVIDEND READS THE
   DESTINATION -- and the mode test above kills it when the destination is
   narrower than the computation.

   PROOF, and it is the most useful measurement in this pass: `s16 t' alone
   (nothing else changed) makes `GET_MODE (target) != compute_mode' and
   BLOCK 1 GOES WORD-EXACT. 245 words / 11 rows, and the surviving rows are
   only the two sign-extension words the narrow declaration costs, the
   `subu' destination, and block 2 (untouched):

       R 98   subu v0,a0,v0            subu $s3, $a0, $v0
       + 99   sll  v0,v0,0x10
       + 100  sra  s3,v0,0x10
       (rows 84-97 -- addu $a0, addu $a1, bgez $a0, addiu $a1, sra $v0,
        sll $v0 -- all leave the residue)

   So the residue is reachable, and it is reachable at the DECLARATION, not
   at the statement split. `s16 t' is not the answer because the two SImode
   call arguments cost the extension and `func_800644B8' / `func_80064584'
   are prototyped `s32' in include/structs.h, which is not this unit's to
   change.

   TWO OF THE AUTHOR'S ROWS ARE TRANSPOSED. Re-measured on today's unit:

     | t spelling    | t2 spelling   | park says | measures |
     | two statements| two statements|    98     |    54    |
     | two statements| one expression|    54     |    98    |
     | one expression| one expression|    58     |    58    |
     | one expression| two statements|    14     |    14    |

   And the 54-row row is the interesting one, because its rows are a
   DIFFERENT fault from the kept body's. With `t' split, block 1's quotient
   moves to $v0 as the target has it -- `sra $v0, $a0, 0xc' -- and what is
   left is that `t's' pseudo now also holds the SUM (op0 == reg(t), which is
   what makes `reg_mentioned_p' fire), so `t' and `sp' exchange $s2/$s3 for
   the whole body. In the 98-row variant (t split, t2 one expression) BOTH
   blocks' quotient placement matches the target and the residue is a
   four-way callee-saved ROTATION: q $s0 where the target has $s1, the
   counter $s1 for $s0, sp $s3 for $s2, t $s2 for $s3.

   SO THE 14 IS A LOCAL MINIMUM with the wrong fault in it, and the two
   layers are now separable:
     layer 1, which pseudo the `%' expansion's intermediates get -- a
       declaration/dividend question, settled by expmed.c's two guards;
     layer 2, a callee-saved rotation among q, the counter, sp and t -- which
       is the allocation question the author diagnosed, but it only becomes
       the WHOLE residue once layer 1 is spelled the target's way.

   MEASURED AND REJECTED IN THIS PASS, all 243 words:

     | spelling                                                   | rows |
     | `t = ((i << 8) + d + ((t + t) - t - t)) % 0x1000' -- the
       PHANTOM ZERO of matching-levers:12070, to make the dividend
       mention `t' without a statement split                      |  14  |
     | the same with `+ (t - t)'                                  |  14  |
     | `t2 = (t2 + ((t2 + t2) - t2 - t2)) % 0x1000'               |  14  |
     | `s16 t2'                                             | 249 words, 89 |
     | `s16 t' and `s16 t2'                                 | 251 words, 86 |

   The phantom zero is a DEAD END HERE and the reason is structural, so do
   not retry it: `reg_mentioned_p (target, op0)' tests the rtx `op0', which
   after expansion is a bare `(reg N)' holding the sum. Extra references to
   `t' inside the dividend do not put `t's' REGISTER into that rtx, so the
   guard cannot fire. Only reading `t' as the dividend itself does -- and
   that makes `t' hold the sum, which the target does not.

   NEW NEXT STEP, in order.

   1. FIND THE SPELLING THAT GIVES `target == 0' WITHOUT PUTTING THE SUM IN
      `t's' REGISTER AND WITHOUT A NARROW DECLARATION. That is the whole
      remaining question for layer 1 and it is a question about expmed.c's
      two guards, not about allocation. The three ways `target' dies are
      `REG_FUNCTION_VALUE_P (target)', `reg_mentioned_p (target, op0)' and
      the mode test; the first is the one nobody has tried to arrange, and
      note that this function's `return 0' makes its value register free.
   2. IF THAT FAILS, TAKE THE 98-ROW VARIANT (t two statements, t2 one
      expression) AS THE BODY AND FIGHT LAYER 2. Its residue is one
      four-allocno rotation with the `%' expansions already correct, which is
      a far better-understood class than what the 14-row body has -- and the
      phantom zero, useless for layer 1, is exactly the tool for it, because
      it adds references to one value and to nothing else.
   3. THE PERMUTER IS STILL REASONABLE on the kept 14-row body, but name the
      statement for it: `t = ((i << 8) + d) % 0x1000;' and
      `t2 = t2 % 0x1000;' are the only two that matter, and the search space
      it needs to cover is the DECLARATION of `t' and `t2' and the dividend's
      shape, not the modulo's.

   NOT RE-PRICED: the author's byte-dressing order table and the eleven rows
   that measure 14 unchanged. The body below is the author's, byte for byte.
*/
/* ==== THIRD PASS, agent h187, batch 35.  The two sections above are intact.
   THE BODY IS UNCHANGED AND THE FIGURE IS UNCHANGED -- 243 words / 243 words /
   14 aligned differences, reproduced today after the eight other stubs in
   ovl_dng_s10 landed, so nothing in the unit's new context moved it.  What this
   pass adds is a PROOF that one of the two layers is unreachable from C, a
   THIRD layer nobody had named, and two alternative bases that are better
   starting points than the 14-row body even though their figures are worse.

   LAYER 1 IS TWO FACTS WITH OPPOSITE REQUIREMENTS.  h148 had this half right.
   Read the three expansions separately:

     block 1, the `t' modulo (rows 84-98):  the target's `sra' writes $v0, a
       pseudo that is NOT t ($s3), so expand_divmod ran with `target == 0'.
     block 2, the `t2' modulo (rows 118-124): the target's `sra', `sll' and
       `subu' ALL write $s0, which IS t2's register, so expand_divmod ran with
       `target' ALIVE -- the exact opposite.

   So the park's kept body has BOTH SIGNS WRONG, and no single change can fix
   both.  Block 2 is fixed by writing t2 as ONE EXPRESSION (the dividend is
   then a fresh pseudo, `reg_mentioned_p' cannot fire, target survives).

   BLOCK 1 IS NOT REACHABLE WITH AN SImode `t', AND THIS IS A PROOF, NOT A
   SWEEP.  There are exactly four ways `target' dies (expmed.c:2760-2771):
   `REG_FUNCTION_VALUE_P (target)', `reg_mentioned_p (target, op0)',
   `reg_mentioned_p (target, op1)', and a MEM/MEM pair; plus the mode test at
   expmed.c:2815, `if (target && GET_MODE (target) == compute_mode) tquotient
   = target'.  Taken in turn:

     - op1 is `(const_int 4096)', so no register is mentioned in it.
     - `REG_FUNCTION_VALUE_P' cannot be arranged from source.
     - MEM/MEM needs BOTH a memory `t' and a memory dividend; the dividend is
       a sum and is always forced into a register.
     - `reg_mentioned_p (target, op0)' fires only when op0's rtx IS t's
       pseudo, and the ONLY source form that does that is the statement split
       `t = sum; t = t % 0x1000;'.  But then op0 and t are ONE PSEUDO and get
       ONE hard register -- measured: with the split the sum lands in t's own
       register ($s2 in that build) -- while the target has the sum in $a0 and
       t in $s3.  So the split is excluded by the listing, not by its score.
     - AND THE DIVIDEND CAN NEVER LAND IN t's PSEUDO BY ACCIDENT, which is the
       fact that closes the argument: expr.c:6325 expands the dividend with
       `subtarget', and expr.c:4820 sets `subtarget = 0' whenever
       `preserve_subexpressions_p ()' is true, which at -O2 is unconditional
       because `flag_expensive_optimizations' is 1.  So for a plain
       `var = X % K;' at -O2 the dividend is ALWAYS a fresh pseudo and
       `target' ALWAYS survives.

   That leaves the mode test, and the mode test is exactly a narrow
   declaration: store_expr's `GET_CODE (target) == SUBREG &&
   SUBREG_PROMOTED_VAR_P (target)' arm (expr.c:3169) calls
   `expand_expr (exp, NULL_RTX, VOIDmode, 0)' -- the one place a plain
   assignment hands expand_divmod a zero target -- and then pays for it with
   the conversion at expr.c:3197/3216.  THE CONVERSION'S PRICE IS THE WHOLE
   REMAINING PROBLEM AND IT IS NOW MEASURED PER SIGNEDNESS:

     | `t' spelling, t2 kept SPLIT                      | words | rows |
     | `s32 t' (the park's body)                        |  243  |  14  |
     | `s16 t'                                          |  245  |  11  |
     | `u16 t'                                          |  244  |  10  |
     | `s32 t', `t = (s16)(sum % 0x1000);'              |  245  |  11  |
     | `s32 t', `t = (u16)(sum % 0x1000);'              |  244  |  10  |
     | `s32 t', `t = (s16)(sum) % 0x1000;'              |  249  | 102  |

   `s16' costs TWO words (`sll 16' + `sra 16'), `u16' costs ONE (`andi
   0xFFFF'), and a cast costs exactly what the matching declaration costs --
   so casts are not a way round it, which is worth knowing because h148's
   note could be read as leaving them open.

   AND THE `u16 t' ROW IS THE MOST USEFUL MEASUREMENT IN THIS PASS: AT 244
   WORDS BLOCK 1 IS WORD-EXACT.  Its entire residue is

     R  98  subu v0,a0,v0           subu $s3, $a0, $v0
     +  99  andi s3,v0,0xffff
     R 118-124                      block 2, the seven known rows
     R 228  bnez (branch target, a consequence of the one extra word)

   Rows 84-97 -- `addu $a0,$v0,$t1', `addu $a1,$a0,$zero', `bgez $a0',
   `addiu $a1,$a0,0xFFF', `sra $v0,$a1,12', `sll $v0,$v0,12' -- are
   word-identical to the target for the first time in this park's history.
   The `subu' destination differs only because the `andi' stands between it
   and t, so IF THAT ONE CONVERSION WORD CAN BE SUPPRESSED the body is 243
   words with block 2 as its only fault.  That is a much smaller question than
   the one this park has been carrying.

   LAYER 3, WHICH IS NEW: SPELLING t2 AS ONE EXPRESSION ROTATES `q' OFF $s1,
   AND `-da' NAMES THE MECHANISM EXACTLY.  `;; N regs to allocate' in `.greg'
   is global.c's `allocno_order', i.e. the priority order, and three builds of
   this body give:

     t2 SPLIT   (14 rows)   22 regs: 98 164 95 153 ...   98=t2 is #1 -> $s0,
                                                         95=q  is #3 -> $s1
     t2 ONE     (58 rows)   23 regs: 163 164 95 153 171 181 98 ...
                                                         95=q  is #3 -> $s0,
                                                         98=t2 is #7 -> $s1
     t2 TWO-STMT (22 rows)  23 regs: 161 98 162 95 ...   98=t2 is #2 -> $s0,
                                                         95=q  is #4 -> $s1

   So q (reg 95, 78 references over 171 insns, `floor_log2 (78) * 78 / 171 =
   2.74') always ranks third or fourth, and whether it gets $s0 depends
   entirely on whether a CALL-CROSSING allocno outranks it and takes $s0
   first.  t2 is that allocno when -- and only when -- an earlier statement
   has already set it; as one expression its allocno falls to #7 and q takes
   $s0, which is 44 of the 58 rows.  `i' sign-extended (the `(s16)i' pseudo)
   then follows t2 into $s0, so the whole q/i exchange h148 described is one
   decision, not two.

   NOTE ALSO that the loop counter itself is $s7, not $s0: $s0 holds the
   `(s16)i' temporary and then t2, two pseudos sharing one register.  h148's
   phrase "the counter" means the sign-extended temporary.

   THE FULL GRID, aligndiff, built words / aligned rows.  t2 forms:
   SPLIT = `t2 = sum; t2 = t2 % 0x1000;' (the park's);
   ONE   = `t2 = (((i + 1) % 0x10) * 0x100 + d) % 0x1000;';
   TWO   = `t2 = (i + 1) % 0x10; t2 = (t2 * 0x100 + d) % 0x1000;'.

     | t            | t2    | words | rows |
     | s32 one-expr | SPLIT |  243  |  14  |  the kept body
     | s32 one-expr | ONE   |  243  |  58  |
     | s32 one-expr | TWO   |  243  |  22  |
     | s32 split    | ONE   |  243  |  98  |  h148's row, reproduced
     | s32 split    | TWO   |  243  |  62  |
     | s16          | SPLIT |  245  |  11  |  h148's row, reproduced
     | s16          | ONE   |  245  |  55  |
     | s16          | TWO   |  245  |  19  |
     | u16          | SPLIT |  244  |  10  |  block 1 WORD-EXACT
     | u16          | ONE   |  244  |  54  |
     | u16          | TWO   |  244  |  18  |
     | u16 split    | ONE   |  240  | 145  |

   MEASURED AND REJECTED THIS PASS, t kept as the park's `s32' one expression
   unless stated:

     | t2 spelling or device                                  | words | rows |
     | a scratch local for the WHOLE t2 sum, `u = sum;
       t2 = u % 0x1000;'                                      |  243  |  58  |
     | `u = (i + 1) % 0x10; t2 = (u * 0x100 + d) % 0x1000;'
       with `s32 u'                                           |  243  |  58  |
     |   ... with `s16 u'                                     |  244  |  61  |
     | `t2 = ((i + 1) % 0x10) * 0x100; t2 = (t2 + d) % 0x1000' |  243  |  59  |
     | `t2 = i + 1; t2 = ((t2 % 0x10) * 0x100 + d) % 0x1000;'  |  245  |  95  |
     | `t2 = i;' then ONE  (the dead set is deleted)           |  243  |  58  |
     | `t2 = (u16)((i + 1) % 0x10);' then `(t2*0x100+d)%0x1000' |  244  |  61  |
     | `t2 = sum; t2 = (t2 + 0x1000) % 0x1000;'                |  244  |  16  |
     | ONE wrapped in `do { } while (0)'                       |  242  |  95  |
     | ONE plus `do { } while (0)' round t2's vx statement      |  243  |  62  |
     | ONE plus `do { } while (0)' round t2's vy statement      |  243  |  93  |
     | the `t' assignment EMBEDDED in the vx statement as
       `func_800644B8(t = sum % 0x1000)', t2 SPLIT              |  243  |  14  |
     |   ... the same with t2 ONE                              |  243  |  58  |

   THE EMBEDDED-ASSIGNMENT ROW IS A REAL NEGATIVE AND WORTH THE LINE: it is
   FREE, not an improvement and not a regression, because store_expr passes
   `target' through on the want_value path too (expr.c:3221).  Reaching
   `target == 0' by nesting the assignment inside an expression therefore does
   not work, and nobody needs to try it again.

   THE DECLARATION-ORDER SWEEP IS FLAT AT THE NEW SET TOO.  Six permutations
   of {sp, gg, q, i, t, t2} with t2 as ONE expression all measure 243 / 58 --
   bit-identical residues -- so order is not reachable at this set either, and
   an unused extra `s32' local is free (gcc drops it) rather than a set
   change.  `s32 i' with no casts is a different program (237 / 91) and is not
   a spelling of this body.

   WHAT TO TRY NEXT, in order, and the question is now SINGULAR.

   1. THE ONLY OPEN QUESTION IS AN SImode SPELLING FOR WHICH expand_divmod
      RECEIVES `target == 0'.  Every source route has been enumerated and
      eliminated above from the compiler's own text, so this is a permuter
      job and the statement to name for it is just
      `t = ((i << 8) + d) % 0x1000;'.  Do NOT spend measurements on the
      dividend's shape or on casts -- both are closed.
   2. IF THAT FAILS, START FROM THE 244-WORD `u16 t' BODY, NOT FROM THE
      14-ROW ONE.  Block 1 is word-exact there and the whole deficit is one
      `andi $s3, $v0, 0xFFFF' that `convert_move' emits at the store.  A
      device that suppresses one zero-extension is a far better-posed target
      than a four-allocno rotation.
   3. THE 243-WORD / 22-ROW TWO-STATEMENT BODY IS THE OTHER BASE WORTH
      HAVING, because it is the only 243-word body in which block 2's
      expmed threading is CORRECT and `q' keeps $s1 at the same time.  Its
      faults are block 1 (7 rows, the unreachable one) and the `% 0x10'
      expansion (8 rows) -- and the `% 0x10' fault is the SAME fault one
      level down, since writing `t2 = (i + 1) % 0x10;' makes t2 that
      modulo's target where the target has a fresh pseudo.
   4. DO NOT REACH FOR A REFERENCE BUMP ON q OR t2.  Bumps on t2's two use
      sites measure 62 and 93, a bump on the t2 statement itself costs a word
      (242 / 95), and q's 78 references over 171 insns cannot be moved by
      anything that does not change the code.

   NOT RE-PRICED: the first author's byte-dressing order table.  The body
   below is unchanged from h148's pass, which is unchanged from the author's.
*/
s32 func_s10_80024930(u8 *a, UnkVec *b, u8 *c, s16 d, s32 e, s16 f, u8 g, u8 h)
{
    UnkDngS10QuadSP *sp = (UnkDngS10QuadSP *)0x1F800000;
    Unk80083160 *gg = &D_80083160;
    POLY_GT4 *q;
    s16 i;
    s32 t;
    s32 t2;

    sp->unk_18 = D_80083160.unk_00 + 0xB0;
    sp->unk_64[0].vz = sp->unk_64[1].vz = *(u16 *)((u8 *)b + 0xA) - e + 1;
    sp->unk_64[3].vz = sp->unk_64[2].vz = *(u16 *)((u8 *)b + 0xA);
    for (i = 0; i < 0x10; i++)
    {
        q = *(POLY_GT4 **)(gg->unk_00 + 0x8D0);
        *(u32 *)(gg->unk_00 + 0x8D0) = (u32)q + 0x34;
        setPolyGT4(q);
        if (g == 4)
        {
            q->tpage = GetTPage(0, 0, 0x280, 0x100);
        }
        else
        {
            setSemiTrans(q, 1);
            q->tpage = GetTPage(0, g, 0x280, 0x100);
        }
        q->clut = 0x7E00;
        q->u0 = 0;
        q->v0 = 0x80;
        q->u1 = 0x3F;
        q->v1 = 0x80;
        q->u2 = 0;
        q->v2 = 0xBF;
        q->u3 = 0x3F;
        q->v3 = 0xBF;
        q->r0 = 0;
        q->g0 = 0;
        q->b0 = 0;
        q->r1 = 0;
        q->g1 = 0;
        q->b1 = 0;
        q->r2 = h;
        q->g2 = h;
        q->b2 = h;
        q->r3 = h;
        q->g3 = h;
        q->b3 = h;
        t = ((i << 8) + d) % 0x1000;
        sp->unk_64[0].vx = sp->unk_64[2].vx =
            *(u16 *)((u8 *)b + 0x2) + ((func_800644B8(t) >> 4) * f >> 8);
        t2 = ((i + 1) % 0x10) * 0x100 + d;
        t2 = t2 % 0x1000;
        sp->unk_64[1].vx = sp->unk_64[3].vx =
            *(u16 *)((u8 *)b + 0x2) + ((func_800644B8(t2) >> 4) * f >> 8);
        sp->unk_64[0].vy = sp->unk_64[2].vy =
            *(u16 *)((u8 *)b + 0x6) + ((func_80064584(t) >> 4) * f >> 8);
        sp->unk_64[1].vy = sp->unk_64[3].vy =
            *(u16 *)((u8 *)b + 0x6) + ((func_80064584(t2) >> 4) * f >> 8);
        sp->unk_B4 = RotAverage4(&sp->unk_64[0], &sp->unk_64[1], &sp->unk_64[2], &sp->unk_64[3],
                                 (long *)&sp->unk_D8, (long *)&sp->unk_DC, (long *)&sp->unk_E0,
                                 (long *)&sp->unk_E4, (long *)&sp->unk_84, (long *)&sp->unk_88);
        q->x0 = sp->unk_D8;
        q->y0 = sp->unk_DA;
        q->x1 = sp->unk_DC;
        q->y1 = sp->unk_DE;
        q->x2 = sp->unk_E0;
        q->y2 = sp->unk_E2;
        q->x3 = sp->unk_E4;
        q->y3 = sp->unk_E6;
        if ((u32)sp->unk_B4 < 0x1E0)
        {
            addPrim(sp->unk_18 + sp->unk_B4 * 4, q);
        }
    }
    return 0;
}
