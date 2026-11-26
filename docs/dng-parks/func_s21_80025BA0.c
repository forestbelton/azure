/* PARK: func_s21_80025BA0, csrc/ovl_dng_s21.c.

   THIS PARK IS THE WHOLE FAMILY'S, AND IT ABSORBED A SECOND ONE. Until batch
   12 this body was parked TWICE -- here and as func_s20_80026010 (agent-h13,
   23 aligned differences) -- by two agents who each recorded that the other
   member was "still an INCLUDE_ASM, so there was no landed sibling to read"
   and neither of whom looked for a park on it. The two files were kept
   because they reached their figures by DIFFERENT C shapes, and the ladder
   below is why that mattered: h13's integer address form plus this park's
   index locals is the hybrid neither agent tried, and it is what took the
   residue from 16 to 5. h13's file is deleted now that its content is here;
   `git log` has it in full. Landing either member lands both (dngfam: 2
   members, 170 words each).
   agent-h38, 2026-11-23, batch 12.  WAS 16 ALIGNED DIFFERENCES; NOW 5.

   TARGET 170 words.  BEST BUILT 170 words, 5 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py; ovlverify also reports 5 masked, 37 raw).
   2 members, 170 words each, 340 words total (tools/decomp/dngfam.py:
   func_s20_80026010 in ovl_dng_s20, func_s21_80025BA0).  Landing either
   member lands both via dngclone.py.

   THE BODY IN THIS FILE IS A NEW SHAPE AND IT SUPERSEDES BOTH EARLIER ONES.
   It is the HYBRID that neither of the two earlier parks tried:

     * POINTER locals for the three corner addresses, in the INTEGER form
       `((index) << 3) + (u32)D_800265C8', which is h13's shape from
       the now-merged func_s20_80026010 park (agent-h13) -- that is what
       gives the target's
       strictly serial index block and the `addu <index>, <base>' operand
       order, and it is what the first s21 park (index locals plus array refs
       off a pointer local `t') could not reach: its own note records three
       pointer locals measuring 172 words / 153, because it tried them with
       ARRAY REFS rather than the integer form.
     * INDEX locals `ia', `ib', `ic' for the index arithmetic, which is the
       FIRST s21 park's shape.  h13's body wrote the indices inline and `fold'
       then reassociated `(i & -0x10) + (((i + 1) & 0xF) + 0x10)' into
       `((i & -0x10) + 0x10) + ((i + 1) & 0xF)' -- putting the `+ 0x10' on the
       WRONG term.  That is the whole of h13's 23-difference residue.

   WHAT IT DOES.  The 0x202 spawner for the trail ring: for every live entry
   of D_800265C8 take a node, park func_s21_800255E8 as its step and
   func_s21_800259DC as its draw, seed the mover at the average of four ring
   entries (entry i, the next entry in its 0x10-wide ring, and the two
   matching entries of the second bank at D_80026648 = D_800265C8 + 0x80),
   jitter the three velocities by `(rand() & 0xF) - 8' in the high halfword,
   jitter the slot's +0x16/+0x18 by `(rand() & 0x1FF) - 0x100', set the
   payload's +0x1A life to 0x20, copy the four ring entries into the corner
   records at +0x34..+0x53, pick the colour pair by `i %% 0x20 < 0x10', and
   clear D_80026476 on the way out.

   NO TWIN outside the family.  tools/decomp/dngtwin.py --unit ovl_dng_s21
   scores func_s20_80026010 at 1.000 (the clone) and nothing else above 0.45.

   THE RESIDUE IS FIVE ROWS AND ALL FIVE ARE ONE REGISTER.  `ib'/`ic' -- the
   `(i + 1) & 0xF' chain -- lands in $v0 where the target has $v1.  Nothing
   else differs: word count, instruction selection, instruction order, the
   $s0/$s1 assignment that was h13's whole residue, and every other register
   agree exactly.  The five rows are target words 32, 33, 34, 40 and 41:

     addiu v0,s5,1     / addiu $v1, $s5, 0x1
     andi  v0,v0,0xf   / andi  $v1, $v1, 0xF
     addu  s3,s1,v0    / addu  $s3, $s1, $v1
     addiu v0,v0,16    / addiu $v1, $v1, 0x10
     addu  s1,s1,v0    / addu  $s1, $s1, $v1

   THE MECHANISM, read out of `-da'.  `tools/cdkgcc -da -O2 -G0 -Iinclude
   -Ipsyq/include -Icsrc -S' on this body gives, for func_s21_80025BA0:

     ;; 7 regs to allocate: 87 209 88 80 237 109 196
     ;; Register dispositions: ... 81 in 16  83 in 20  84 in 18  85 in 17
                                   86 in 16  92 in 17  93 in 2  94 in 2
                                   105 in 17  106 in 2  111 in 18  112 in 18
                                   115 in 17  116 in 17

   81 = m -> $s0, 86 = e -> $s0, 85 = c -> $s1, 83 = a -> $s4, 84 = b -> $s2:
   ALL CORRECT, which is the part this file fixes.  93 = `(i + 1) & 0xF',
   94 = `ib + 0x10' and 106 = `i + 1' are one local-alloc quantity (each
   link's source dies at the next, so `combine_regs' ties them) and it is
   BLOCK-LOCAL with `qty_n_calls_crossed == 0'.  local-alloc.c's
   `find_free_reg' then scans plain ascending hard-register order -- cdk-gcc's
   mips.h defines NO `REG_ALLOC_ORDER', confirmed in
   scratch/gcc-src/cdk-gcc-b18/config/mips/mips.h -- and takes the first free
   call-used register, which is $v0.  For the target to use $v1 instead,
   SOMETHING ELSE MUST HOLD $v0 LIVE ACROSS TARGET WORDS 30..43, and no value
   in the emitted target touches $v0 between the `sw $v0, 0x10($s6)' in the
   func_8004491C delay slot (word 29) and the `lh $v0, 0x0($s4)' ring load
   (word 45).  Whatever it is produces no instruction in that window.

   AGENT-h130, 2026-11-24, BATCH 30.  STILL 5 ALIGNED DIFFERENCES.  The body
   below is unchanged: I reproduced agent-h38's 170 words / 5 aligned rows
   exactly, byte for byte, with the same five rows at the same target words,
   and 13 further spellings are priced below.  (My dispatch brief said this
   park stood at 23 rows; that is the figure of the DELETED h13 park.  The
   file is right and the brief was wrong -- 5 is what this shape measures.)

   THE DIAGNOSIS IS NARROWED FROM `find the missing $v0 consumer' TO A PROOF
   THAT THERE IS NO SUCH CONSUMER REACHABLE AT 170 WORDS.  I read sched1's
   output (`probe.c.sched') rather than only `.lreg'/`.greg', which names the
   pseudos and, more usefully, the TIES.  In sched1 order the window is

     insn 53   pseudo 105 = -0x10                  (li)
     insn 54   pseudo  92 = ia = i & -0x10         (and)   <- chain's rival
     insn 57   pseudo 106 = i + 1
     insn 58   pseudo  93 = ib = (i + 1) & 0xF
     insn 61   pseudo 107 = ia + ib
     insn 62   pseudo 108 = 107 << 3
     insn 66   pseudo  83 = a = 108 + base
     insn 69   pseudo 111 = i + 0x10
     insn 70   pseudo 112 = 111 << 3
     insn 74   pseudo  84 = b = 112 + base
     insn 77   pseudo  94 = ic = ib + 0x10         (REG_DEAD 93 here)
     insn 80   pseudo 115 = ia + ic                (REG_DEAD 94 here)

   so the chain 106 -> 93 -> 94 is one `combine_regs' quantity whose range is
   insns 57..80, and that range CONTAINS six other values: 107, 108, 83, 111,
   112, 84.  Those are the only candidates to hold $v0 across the chain, and
   every one of them is unavailable, for a reason that is a fact about the
   TARGET and not about this body:

     * 107/108 are tied into one quantity with each other (each dies at the
       next) and 108 IS USED AGAIN AFTER THE rand() CALLS -- it is the scaled
       `a' index the target re-adds D_80026648 to with `addu $s3, $s3, $t3'.
       So that quantity crosses five calls, `find_free_reg' starts from
       `call_used_reg_set', and it can only have a callee-saved register.  It
       gets $s3 in both bodies.
     * 111/112/84 are one quantity ending in `b', which crosses the same five
       calls: $s2 in both bodies.
     * 83 is `a' and 92 is `ia' (tied to `c'): $s4 and $s1, both call-crossing.

   I CONFIRMED 108's call-crossing IS FORCED, not a choice of this body: the
   second-bank copy written off the pointer instead, `*(u32 *)((u8 *)a +
   0x80)', comes out 168 words / 62 -- TWO WORDS SHORT, which is the
   `lui'/`addiu D_80026648' pair the target really emits.  So there is no
   spelling that frees 108 from the calls while keeping 170 words, and with
   all six rivals callee-saved nothing marks $v0 in `regs_live_at' over insns
   57..80.  `find_free_reg' then scans ascending (cdk-gcc's mips.h has no
   `REG_ALLOC_ORDER') and hands the chain $v0.

   WHAT THIS MEANS FOR THE NEXT AGENT.  Do not look for another statement
   order and do not look for a reference bump: the quantity's priority is
   irrelevant here, because NO rival wants $v0 at all, so the chain takes it
   whatever its rank.  The only two things that can move it are
     (a) a value that is born before insn 57, dies after insn 80, does NOT
         cross a call, and costs no instruction -- which I believe is
         impossible, since a quantity needs a set and a use; or
     (b) the chain itself crossing a call, which I MEASURED for the first time
         (it was listed as untried): the whole index block moved above
         func_8004491C is 170 words / 24 rows, and it does hold the word
         count, so the register IS reachable from the quantity's own
         properties -- just in the wrong direction (callee-saved, not $v1).
   My own reading is that these five rows are not a C-level fact at all and
   that the next real move is the permuter on the index block, or accepting
   the park.  They are 5 rows out of 170 words and the whole rest of the body,
   including the $s0..$s7/$fp assignment, is exact.

   EVERY SPELLING agent-h130 MEASURED, all 170 words unless stated, all
   against the 5-row body:

     `ic = ((i + 1) & 0xF) + 0x10' recomputed instead of `ib + 0x10'      5
       (cse unifies it back to `ib + 0x10'; identical code)
     `a's index as `(ib + ia)' rather than `(ia + ib)'                    5
     `c's index as `(ic + ia)' rather than `(ia + ic)'                   23
     a separate local `j' for `i + 1', then `ib = j & 0xF'                5
     `(ia + ib) * 8' for the `a' index instead of `<< 3'                  5
     `(ia + ic) * 8' for the `c' index instead of `<< 3'                  5
     `a = &D_800265C8[ia + ib]' (array ref) with b and c left integer     5
     `b's scaled index hoisted into its own local used twice             27
     the second-bank `a' copy as `*(u32 *)((u8 *)a + 0x80)'
         -- TWO WORDS SHORT, so the target really emits the
            lui/addiu D_80026648 pair                     168 words,     62
     the second-bank `a' copy indexed `(ic - 0x10 + ia)'    176 words,    68
     the second-bank `a' copy indexed `(ia + ic - 0x10)'                 32
     the whole index block moved ABOVE func_8004491C
         (the chain then crosses the call)                               24
     `n->unk_10 = step' moved BELOW the index block                      13

   The last two are the park's own two "what I would try next" shapes, now
   priced.  Neither helps, and the 13-row one is the closest miss in this
   file after the kept body.

   WHAT I WOULD TRY NEXT.  Find the missing $v0 consumer, not another
   statement order -- order is exhausted below.  Two concrete shapes:
     * a value computed before the index block and used after it that
       `combine' folds away to nothing, so it costs no word but still holds a
       register at local-alloc time;
     * getting the `ib' quantity to CROSS A CALL, which makes
       `find_free_reg' exclude every call-used register -- that is the wrong
       direction for $v1, but it proves whether the register choice is
       reachable at all from the quantity's own properties.
   The one thing I did NOT measure is a device that spends a word: every row
   below holds the word count at 170, and a 171-word body is not a landing.

   EVERY SPELLING MEASURED IN THIS RUN, all 170 words unless stated.
   The first block is the shape search; the second is this shape's details.

     POINTER locals, integer form, indices inline (h13's body
         translated: the untested combination the dispatcher flagged)      23
     ... with `c's index written `(((i + 1) & 0xF) + 0x10 + (i & -0x10))'
         so fold cannot move the constant onto the `i & -0x10' term         7
     ... with `ia'/`ib'/`ic' index locals and `ic = ib + 0x10' AFTER the
         `a' and `b' statements                      (THIS BODY)            5
     ... `ic = ib + 0x10' before `a'                                       17
     ... `ic = ib + 0x10' between `a' and `b'                               7
     ... `ib += 0x10' in place of a separate `ic'                          28
     ... `c's index as `(ia + (ib + 0x10))' with no `ic' local             23
     ... `ib' assigned before `ia'                                          9
     ... the `b' statement before the `a' statement        174 words,      36

     STATEMENT ORDER OF THE EIGHT HEAD STATEMENTS (ia, ib, a, b, ic, c, w, m)
     -- ten orders, every one that keeps `ic' after `a' measures 5 and the
     two that move it measure 9 and 11, so NOTHING ELSE IN THE HEAD IS
     ORDER-SENSITIVE:
       ia ib a b ic c w m   (this body)                                     5
       ia ib w a b ic c m / ia ib m a b ic c w / m ia ib a b ic c w
         / w m ia ib a b ic c / ia ib a b w ic c m / ia ib a b ic c m w
         / ia ib a b m ic c w                                         5 each
       ia ib a ic c b w m                                                  11
       ib ia a b ic c w m                                                   9

     MEASURED AND FREE on this shape, so NOT reasons it reaches 5:
       the `>> 2' inside the three sums instead of in the three stores       5
       `x'/`y'/`z' declared in the order z, y, x                            5
       `m' as a `u8 *' with `*(u32 *)(m + 0xC)' casts                       5
       `ia'/`ib'/`ic' declared as `u32'                                     5
       `ic' declared `s16'                                                  5
       `ia'/`ib'/`ic' declared before `i'/`x'/`y'/`z', and `ib'/`ic'
           before `ia'                                                      5
       a local for the `a' sum index, for `i + 0x10', or for `ia + ic'       5
       `((s16 *)m)[1]' array refs for the three mover stores                5

     RE-MEASURED FROM h13's PARK ON THIS SHAPE and still negative, so its
     conclusion carries:
       `do { } while (0)' around the `m->unk_14' store      175 words,     42
       `do { } while (0)' around the `m->unk_0C' store      175 words,     42
       `m = n->unk_08' moved after the three sums                         24

   WHY THE EARLIER s21 BODY'S FOUR "LOAD-BEARING" FACTS NO LONGER APPLY.
   That body addressed the ring through a pointer local `t = D_800265C8'
   assigned inside the loop and indexed it `t[i]', `t[j]', `t[k]', `t[l]'.
   This body has no `t': the first corner is `&D_800265C8[i]' (which loop.c
   strength-reduces into the target's `$s7' giv, incremented by 8 in the
   loop's branch delay slot) and the other three are integer addresses off
   the hoisted `$fp' base.  Its three other facts survive in a changed form
   and are visible in the body: the three averages still go into `x'/`y'/`z'
   locals and are stored afterwards, the second bank D_80026648 is still its
   own symbol for the two word copies (`(i << 3) + (u32)D_80026648' and
   `((ia + ib) << 3) + (u32)D_80026648', which is where the target's two
   `lui/addiu D_80026648' pairs and its `addu $s3, $s3, $t3' reuse of the
   scaled `a' index come from), and `i + 0x10' is still a separate index
   rather than a displacement off the biv.

   `rand' IS func_80069EF8 (splat/common_syms.txt), so the five `jal
   func_80069EF8' in the listing are the five `rand()' calls here; the real
   name needs no declaration.

   AGENT-h136, 2026-11-24, BATCH 32.  STILL 5 ALIGNED DIFFERENCES, AND THIS
   TIME MEASURED ON THE OTHER MEMBER.  I was dispatched on func_s20_80026010
   (the s20 relink) and translated the body below into it symbol for symbol.
   It builds 170 words with THE SAME FIVE ROWS AT THE SAME TARGET WORDS --
   32, 33, 34, 40, 41, all `$v0' where the target has `$v1' -- so the shape in
   this file is confirmed independently on both members and the residue is a
   property of the family, not of one unit's allocation.  Everything else,
   including the whole $s0..$s7/$fp assignment and the two
   `lui/addiu D_800264F8' pairs, is exact.

   THE s20 RELINK MAP, so the next agent does not re-derive it.  The body is
   otherwise character for character the one below:

     D_80026476  (loop bound, cleared on exit)  ->  D_s20_8002632A
     D_800265C8  (ring bank 1)                  ->  D_80026478
     D_80026648  (ring bank 2, bank1 + 0x80)    ->  D_800264F8
     D_800265C0 / D_800265C4  (colour pair)     ->  D_80026470 / D_80026474
     func_s21_800255E8  (the +0x10 step)        ->  func_s20_80025A58
     func_s21_800259DC  (the func_8004491C draw)->  func_s20_80025E4C
     every payload offset from +0x34 down is FOUR LOWER: the corner records
     run +0x30..+0x4C and the colour pair is +0x50/+0x54, against +0x34..+0x50
     and +0x54/+0x58 here.  +0x1A, the life, is unchanged.

   In ovl_dng_s20 that needs `extern UnkDng26478 D_800264F8[];' (the unit
   already declares the `UnkDng26478' typedef, `D_80026478', `D_s20_8002632A'
   and `D_80026470'/`D_80026474' as `u32', and `func_8003FC64'), plus forward
   declarations for the two handlers, which are DEFINED LOWER in that file:
   `void func_s20_80025A58(u8 *p, UnkVec *v, UnkSlot *s);' and
   `s32 func_s20_80025E4C(u8 *p, UnkVec *v, UnkSlot *s);'.  Note also that
   ovl_dng_s20's declaration block already carries an UNUSED
   `typedef struct UnkDng265C8 { s16 unk_00; s16 unk_02; s16 unk_04;
   u8 pad_06[0x2]; } UnkDng265C8;' -- that is residue of THIS shape's s21
   type name and is a witness that the two units' ring banks are one layout.

   FOUR OF THIS FILE'S ORDER AND ARITHMETIC ROWS REPRODUCE TO THE DIGIT on
   the s20 member, which is the strongest transfer I have seen in this tree:

     `ic = ib + 0x10' moved BEFORE the `a' statement       17  (file: 17)
     `ic = ib + 0x10' between `a' and `b'                   7  (file:  7)
     `c's index as `(ic + ia)' rather than `(ia + ic)'     23  (file: 23)
     `ib' assigned before `ia'                              9  (file:  9)

   THREE NEW SPELLINGS, all against the 5-row body:

     `ia = i - (i & 0xF)' in place of `i & -0x10'        170 words,       7
       -- a third arithmetic form of the mask, untried here; it keeps the
          word count and costs 7, so `& -0x10' stays.
     `w' in the INTEGER form `(i << 3) + (u32)D_800265C8' rather than the
       array ref `&D_800265C8[i]'                        170 words,       5
       -- FREE, and worth knowing because this file's prose explains the
          kept array ref as what lets loop.c strength-reduce `w' into the
          `$s7' giv.  It does that from the integer form too, so the array
          ref is NOT a reason this shape reaches 5.
     a fourth inline spelling of the indices,
       `(i & -0x10) + (((i + 1) & 0xF) + 0x10)' for `c' with no locals
                                                        177 words,     101
       -- distinct from both inline rows above (23 and 7); recorded so the
          next agent does not count it as a contradiction of either.

   AND I TESTED agent-h130's SHAPE (a) -- "a value born before insn 57, dying
   after insn 80, not crossing a call" -- in the one form C can express it:
   a separate local for `i + 0x10' ASSIGNED BEFORE `ia', so its quantity is
   born ahead of the chain and dies inside it.  170 words, 35 ROWS.  That
   kills the shape from the other end: a rival that OVERLAPS the chain does
   take a call-used register, but it takes it at the cost of displacing the
   `b' block, and nothing in C can make a rival whose range CONTAINS the
   chain without also costing an instruction.  I agree with h130's reading
   and I want to state the negative result plainly: a pseudo needs a set and
   a use, flow deletes a dead set before local-alloc ever sees it, so there
   is no zero-word $v0 holder.  The next move on this body is the permuter on
   the index block or accepting the park -- not another respelling.  (I was
   explicitly barred from the permuter.)

   MY MEASUREMENTS AGREE WITH THIS FILE EVERYWHERE THEY OVERLAP; nothing here
   contradicts it.  Two of the file's own claims I did NOT re-price, because
   they describe the kept body and the kept body is what I reproduced: the
   `>> 2' placement and the `((s16 *)m)[1]' store spelling.

   raw 37 == the 5 masked rows plus 32 relocation and local-label words:
   R_MIPS_26 on func_8003FC64, func_8004491C and rand five times, HI16/LO16
   pairs on D_80026476 three times, D_800265C8, func_s21_800255E8,
   func_s21_800259DC, D_80026648 twice, D_800265C0 twice and D_800265C4
   twice, and the body's own `j'/`beqz'/`bnez' targets.

   func_s21_800255E8 and func_s21_800259DC both need NO declaration: both are
   defined above this function in csrc/ovl_dng_s21.c.  Of the block below,
   only D_80026648 is new to that unit as of this run.

   AGENT-h146, 2026-11-25, BATCH 34.  STILL PARKED AT 5 ALIGNED DIFFERENCES,
   BUT THE FIVE ROWS ARE NO LONGER A MYSTERY: I CAN NOW MAKE THEM MATCH, AND
   THE REASON NO RESPELLING FOUND IT IS THAT THIS FILE'S MODEL OF
   `find_free_reg' IS INCOMPLETE.  I reproduced 170 words / 5 rows at the same
   five target words first, so everything below is measured against that.

   WHAT THIS FILE GETS WRONG, and it is the one load-bearing claim in it:
   `local-alloc.c's find_free_reg then scans plain ascending hard-register
   order ... the quantity's priority is irrelevant here, because NO rival
   wants $v0 at all, so the chain takes it whatever its rank.'  find_free_reg
   is called in TWO ROUNDS, not one (local-alloc.c:1576-1586, then
   :1627-1648):

     ROUND 1 walks the quantities in `qty_sugg_compare_1' order and allocates
       EVERY quantity that has a hard-register SUGGESTION -- before any
       un-suggested quantity is considered at all.  A successful allocation
       calls `post_mark_life (regno, mode, 1, born_index, dead_index)'
       (local-alloc.c:2235), which marks that hard register live in
       `regs_live_at' over the quantity's WHOLE [birth, death) range.
     ROUND 2 then walks the rest in `qty_compare_1' order -- DECREASING LIFE
       LENGTH, not source order -- and find_free_reg excludes, for each,
       `for (ins = born_index; ins < dead_index; ins++) used |=
       regs_live_at[ins]' (local-alloc.c:2179).

   So a rival does NOT have to `want $v0'.  It only has to carry a
   SUGGESTION of $v0 and have a live range that covers the chain's, and round
   1 will hand it $v0 and reserve it over the window before the chain is ever
   looked at.  Suggestions are recorded in `combine_regs' (local-alloc.c:
   1867-1907) whenever a pseudo is tied to a HARD register, and on MIPS the
   only hard registers that tie to a pseudo are the argument registers
   $a0..$a3 and the RETURN REGISTER $v0.  So the question this file asks --
   `find a value born before insn 57, dying after insn 80, not crossing a
   call' -- was the wrong question.  The right one is: WHICH CALL'S RETURN
   VALUE IS LIVE ACROSS THE INDEX BLOCK IN THE TARGET.

   THREE POSITIVE CONTROLS.  Target words 32, 33, 34, 40 and 41 MATCH EXACTLY
   in all three, which is the first time in three agent-runs that this block
   has been reproduced:

     x = rand() inserted immediately before `ia' and folded into the x sum
       (`x = x + w->unk_00 + ...')                        172 words,     28
       -- the rand result is copy-suggested $v0, is call-free, and its range
          covers the chain.  Index block EXACT.  Two words over, so not a
          landing: this is the mechanism's positive control, nothing more.
     qq = (UnkNode *)func_8004491C(n, (u32)func_s21_800259DC) with
       `m = qq->unk_08' in place of `m = n->unk_08'        170 words,      9
       -- HOLDS THE WORD COUNT AND THE INDEX BLOCK IS EXACT.  Of the 9 rows
          exactly one is the real residue, `lw $s0, 0x8($v0)' at word 47
          where the target has `lw $s0, 0x8($s6)'; the other 8 are knock-on
          register rotation through the sums block.  THIS BODY IS IN FULL AT
          THE END OF THIS SECTION and it is the better permuter seed, because
          its residue is one base register rather than a whole allocator
          decision.
     n = (UnkNode *)func_8004491C(n, ...), reassigning n   171 words,     10
       -- one word over (the return copy survives) and the index block is
          BACK TO 5 rows, because the reassigned `n' crosses the five rand()
          calls and so takes a callee-saved register instead of $v0.

   THE OTHER CONSUMERS OF THE RETURN VALUE, all measured, all worse:
     consumed as `e's base (`e = (u8 *)qq->unk_0C')        176 words,     71
     consumed as `q's base (`q = (u8 *)qq + 0x20')         175 words,     85
     consumed as BOTH `m's and `e's base                   176 words,     45
   `e' and `q' are both computed AFTER the five rand() calls, so a return
   value kept alive to reach them crosses the calls and goes callee-saved --
   which is also why the `m' spelling is the only one that works: `m's load
   is the one use of a node pointer that sits between the index block and the
   first rand().

   TWO NEGATIVE RESULTS THAT CLOSE THE SOURCE-REORDERING SEARCH FOR GOOD, AND
   THE REASON, WHICH THIS FILE DOES NOT STATE: sched1 RUNS BEFORE LOCAL-ALLOC
   AND NORMALISES THIS WINDOW, so source order cannot place a value's birth
   above the index block or its death below it.

     `x = w->unk_00' hoisted above `ia', with the rest of the sum left
       where it was (`x = x + a->unk_00 + b->unk_00 + c->unk_00')
                                                           170 words,     33
       -- this is agent-h130's shape (a) in its most natural form, and it
          FAILS FOR A NEW REASON: sched1 SINKS the load back to target word
          44, BELOW the index block, so the quantity never overlaps the chain
          at all.  It is emitted in $t3 and the chain still takes $v0.  The
          park's reading that `nothing in C can make a rival whose range
          CONTAINS the chain' is right, but the obstruction is sched1, not
          the impossibility of a zero-word pseudo.
     `n->unk_10 = step' moved below the index block -- THE 13-ROW ROW IN THIS
       FILE IS POSITION-INVARIANT.  I re-priced it at SEVEN source positions
       (immediately before `ia', before `ib', before `ic', before `c',
       before `w', before `m', and before the x sum) and every one is
       170 words / 13 aligned differences, identical residue, because sched1
       re-emits `sw $v0, 0x10($s6)' at target word 31 -- above the index
       block again -- in all seven.  Three further positions are worse and
       are new figures: before the y sum 25, before the first mover store 50,
       before `m->unk_0C' 14, before `e' 169 words / 16, before `q' 15.
       So this file's single 13 is not one spelling among many: it is what
       the whole family of `store the step later' shapes measures.

   WHY IT STILL DOES NOT LAND, STATED EXACTLY, because this is the part that
   is actionable.  The $v0 holder must be live across target words 32..41 and
   its LAST USE must be an instruction the target already emits.  Walk the
   target forward from word 42: 42 `addu $s1, $s1, $v1', 43 `sll $s1, $s1, 3',
   44 `addu $s1, $s1, $fp', 45 `lh $a0, 0x0($s7)', 46 `lh $v0, 0x0($s4)'.
   Words 42..44 are the `c' chain off $s1 and $fp; words 45 and 46 are the
   `w' and `a' ring loads off $s7 and $s4.  NOT ONE of them can be the use of
   a node pointer returned by func_8004491C, and word 47 -- `lw $s0,
   0x8($s6)' -- names $s6, which is `n' itself and not the return value.  So
   the target's $v0 holder dies at a point where the target emits NOTHING
   that could be its use, and a C pseudo needs a use or flow deletes its set
   before local-alloc sees it.  That is the same wall agent-h136 hit, but it
   is now a wall at a NAMED location with a NAMED register and a NAMED call,
   rather than an open search.

   MY READING, and I agree with h130 and h136 on the verdict while
   disagreeing with the file's reason for it: THIS IS PERMUTER WORK, AND THE
   EXACT STATEMENT IS `func_8004491C(n, (u32)func_s21_800259DC);' -- its
   DISCARDED RETURN VALUE.  The permuter should be given the index block
   TOGETHER WITH that call statement, and what it is looking for is a
   consumer of that return value that costs no word.  Seed it with the 9-row
   body below rather than the 5-row body above: the 9-row body already has
   the index block exact, holds 170 words, and its residue is a single base
   register plus rotation, which is the kind of thing a permuter moves.  If
   no such consumer exists, accept the park -- 5 rows out of 170 words, with
   the whole $s0..$s7/$fp assignment exact, and 340 words across the family.

   NOT RE-PRICED BY ME, so this file's figures stand: the whole spelling
   ladder above (the shape search, the ten head orders, the eight
   measured-and-free rows and the three h136 rows).  I re-priced only the two
   rows that bear on the mechanism -- the 13-row step store and h130's shape
   (a) -- and both got WORSE or stayed equal, never better.

   THE 9-ROW BODY, for the permuter.  It is the body below with exactly two
   changes: one added local, and the call's return value consumed as `m's
   base.  170 words, 9 aligned differences, index block EXACT.

       UnkNode *qq;                     -- added after `UnkNode *n;'

       qq = (UnkNode *)func_8004491C(n, (u32)func_s21_800259DC);
                                        -- replaces the bare call
       m = qq->unk_08;                  -- replaces `m = n->unk_08;'

   Its nine rows, for comparison with the five above:

       R  45  lh v1,0(s4)      / lh $v0, 0x0($s4)
       R  46  lh a1,0(s1)      / lh $v1, 0x0($s1)
       R  47  lw s0,8(v0)      / lw $s0, 0x8($s6)     <- the one real row
       R  48  lh v0,2(s7)      / lh $a1, 0x2($s4)
       R  53  addu a0,a0,v1    / addu $a0, $a0, $v0
       R  54  lh v1,0(s2)      / lh $v0, 0x0($s2)
       R  56  addu a0,a0,v1    / addu $a0, $a0, $v0
       R  57  addu a0,a0,a1    / addu $a0, $a0, $v1
       R  58  lh a1,2(s4)      / lh $v0, 0x2($s7)

   AGENT-h282, 2026-11-26, BATCH 35.  STILL 5 ALIGNED DIFFERENCES -- FOURTH
   REPRODUCTION, AND THE SECOND ON THE s20 MEMBER.  I was dispatched on
   func_s20_80026010, translated the body below through the s20 relink map
   above and measured 170 words / 5 aligned differences with THE SAME FIVE
   ROWS AT THE SAME TARGET WORDS (32, 33, 34, 40, 41, every one `$v0' where
   the target has `$v1').  I agree with the ACCEPT verdict.  I add one
   CORRECTION to this file's compiler model, one sharpening of its central
   negative into arithmetic, one new figure and one widening of the search
   object.  I re-priced nothing else, and I contradict nothing in this file.

   THE RELINK MAP ABOVE IS NOW ONE ITEM SHORTER.  It says ovl_dng_s20 needs
   forward declarations for func_s20_80025A58 and func_s20_80025E4C "which
   are DEFINED LOWER in that file".  They are no longer: both are landed and
   both sit ABOVE the func_s20_80026010 stub in csrc/ovl_dng_s20.c as of
   batch 35, so the only declaration the s20 translation needs is
   `extern UnkDng26478 D_800264F8[];'.  Everything else in that map held.

   CORRECTION -- ROUND 2 IS ORDERED BY PRIORITY, NOT BY DECREASING LIFE
   LENGTH.  agent-h146's round-1/round-2 reading is otherwise right and is
   the best thing in this file, but its round-2 sentence is wrong:
   `qty_compare' and `qty_compare_1' (local-alloc.c:1697-1716) both rank on

     QTY_CMP_PRI(q) = (int) (((double) (floor_log2 (qty_n_refs[q])
                              * qty_n_refs[q] * qty_size[q])
                             / (qty_death[q] - qty_birth[q])) * 10000)

   which is the same shape as global.c's `allocno_compare' -- references
   weighted by their own log and DIVIDED by life length, so a longer life
   makes a quantity rank WORSE, not better.  The phrase "decreasing length of
   life" is gcc's OWN comment at local-alloc.c:1586-1588, immediately above
   the sort that contradicts it, so this file inherited a stale compiler
   comment rather than misreading the code.  NOT A NEW FINDING, and I am
   saying so rather than claiming one: docs/matching-levers.md already records
   that gcc's "decreasing length of life" comment is stale and that the sort
   is the refs-weighted priority (two places, in the pass-order section and in
   the allocation-priority section).  Only THIS FILE was still wrong, which is
   why the correction belongs here and not there.

   AND WHY THAT OPENS NO DOOR -- THE CENTRAL NEGATIVE IS ARITHMETIC, NOT AN
   EXHAUSTED SEARCH.  `find_free_reg' seeds its exclusion set at
   local-alloc.c:2170-2175:

     if (accept_call_clobbered)      used = call_fixed_reg_set;
     else if (qty_n_calls_crossed[qty] == 0)  used = fixed_reg_set;
     else                            used = call_used_reg_set;

   For a quantity that crosses a call that is a HARD EXCLUSION of every
   call-used register -- $v0 and $v1 alike -- before any ranking happens, not
   a preference.  agent-h130 enumerated every quantity born inside the index
   window (107/108, 111/112/84, 83, 92) and showed each one feeds a value the
   target reads AFTER the five rand() calls, so every one of them crosses
   those calls.  Therefore no round-2 rival for $v0 can exist in that window
   AT ANY PRIORITY, and the correction above cannot be turned into a lever.
   h130's conclusion stands and its reason is right for the wrong reason:
   priority is not irrelevant here, it is unreachable.

   NEW FIGURE, and it corrects what h136's 35-row row is evidence OF.  A
   separate local `id' for `i + 0x10' assigned BEFORE `ia', with `b' written
   `(id << 3) + (u32)D_80026478' -- h136's shape, on the s20 member:

     170 words, 7 ALIGNED DIFFERENCES

   and the five index rows are UNCHANGED.  The whole of the 7 is those five
   plus two rows for the hoisted `addiu $s2, $s5, 0x10' moving from target
   word 37 up to word 30.  h136 records 35 for what it describes as this
   shape on the s21 member; I am recording mine rather than reconciling them,
   since the two placements may not be identical.  What matters is the same
   either way and is now explicit: the rival was emitted in $s2, a
   CALLEE-SAVED register, because `b' is read after the calls -- so it was
   never a candidate for $v0 at all.  h136's note says "a rival that OVERLAPS
   the chain does take a call-used register, but it takes it at the cost of
   displacing the `b' block"; the first half of that is not what the rival
   does, and the 35 rows are not evidence for it.

   ONE WIDENING OF THE SEARCH OBJECT, nobody has stated it.  This file hunts
   for a PSEUDO holding $v0 across the window.  It need not be a pseudo:
   local-alloc.c:1525-1526 ORs `regs_live' -- the RTL's own HARD-register
   liveness, recomputed per insn -- into `regs_live_at' at every insn, and
   `find_free_reg' reads that same array.  So a hard $v0 live range running
   from a call's return to a later copy blocks the chain just as an allocated
   quantity would.  That does not change h146's verdict -- such a copy still
   needs a use the target emits, and h146's forward walk shows words 42..47
   supply none -- but it means the thing to look for is "$v0 live", not "a
   pseudo assigned $v0", and the two are not the same object.

DECLARATIONS
==== THE PERMUTER HAS NOW BEEN SPENT ON THIS PARK, 2026-11-25, dispatcher.

Seeded from the 9-ROW body (the return value captured through `qq', with the
index block EXACT) rather than from the 5-row body below, on the reasoning that
a seed whose disputed block is already correct is the better base -- which is
what the earlier scaffold-versus-park result on another park had shown.

    45 minutes, 317,225 iterations, 8 jobs, base = 80 -> best = 60

The winning candidate is SEMANTICALLY VALID and is exactly the shape the
previous agent asked for -- a consumer that gives the discarded return value a
death point:

    qq = (UnkNode *)func_8004491C(n, (u32)func_s21_800259DC);
    ia = i & -0x10;
    qq = n;                          <- added
    ...
    q = (u8 *)qq + 0x20;             <- was (u8 *)n + 0x20

Measured by hand: **170 words / 5 aligned differences, and they are THE SAME
FIVE ROWS** -- words 32, 33, 34, 40, 41, every one `$v0` where the target has
`$v1`. So the search did not beat this body; it found a different route back to
it, trading the 9-row seed's four knock-on rows for the index block it had
fixed.

WHAT THAT SETTLES. The park's two stated options were "accept 5 or permute".
Permuting is now done and returned the floor, from the best seed available,
over three hundred thousand iterations. Together with the two facts already in
this file -- that the five rows REPRODUCE AT THE SAME TARGET WORDS on the
exact-clone sibling in another unit, so the fault is in the shape and not the
bank, and that the one untried rival shape is closed at 170/35 -- the evidence
for 5 being this shape's floor is as strong as this project gets.

MY RECOMMENDATION IS NOW ACCEPT, not continue. Three agents have respelled it,
a relink has proved the residue structural, and a permuter run from an informed
seed has returned the same five rows. Anyone picking this up should read that
as a closed question unless they have a NEW mechanism -- not a new spelling --
and the two mechanisms nobody has produced are a way to give the return value a
use the target already emits (walking forward from word 42, none of words
42..47 can be it), and a way to make the index chain's quantity lose `$v0` in
round 1 of find_free_reg.

The stale `output-25-1' in nonmatchings/func_s21_80025BA0/ is from the FIRST
run against this body and is not comparable; permimport now archives such
directories, because `ls' sorted it ahead of this run's genuine best.

(UnkDng26478 is now defined in ovl_dng_s21 itself, by a body landed after
this park was written.  Declaring it here again is a redefinition and is why
the park stopped splicing; take it from the unit.)
extern UnkDng26478 D_800265C8[];
extern UnkDng26478 D_80026648[];
extern s16 D_80026476;
extern u32 D_800265C0;
extern u32 D_800265C4;
UnkNode *func_8003FC64(s32);
END DECLARATIONS */
void func_s21_80025BA0(void)
{
    UnkNode *n;
    UnkVec *m;
    UnkDng26478 *w;
    UnkDng26478 *a;
    UnkDng26478 *b;
    UnkDng26478 *c;
    u8 *e;
    u8 *q;
    s32 i;
    s32 x;
    s32 y;
    s32 z;
    s32 ia;
    s32 ib;
    s32 ic;

    for (i = 0; i < D_80026476; i++)
    {
        n = func_8003FC64(0x202);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s21_800255E8;
            func_8004491C(n, (u32)func_s21_800259DC);
            ia = i & -0x10;
            ib = (i + 1) & 0xF;
            a = (UnkDng26478 *)(((ia + ib) << 3) + (u32)D_800265C8);
            b = (UnkDng26478 *)(((i + 0x10) << 3) + (u32)D_800265C8);
            ic = ib + 0x10;
            c = (UnkDng26478 *)(((ia + ic) << 3) + (u32)D_800265C8);
            w = &D_800265C8[i];
            m = n->unk_08;
            x = w->unk_00 + a->unk_00 + b->unk_00 + c->unk_00;
            y = w->unk_02 + a->unk_02 + b->unk_02 + c->unk_02;
            z = w->unk_04 + a->unk_04 + b->unk_04 + c->unk_04;
            *(s16 *)((u8 *)m + 0x2) = x >> 2;
            *(s16 *)((u8 *)m + 0x6) = y >> 2;
            *(s16 *)((u8 *)m + 0xA) = z >> 2;
            m->unk_0C = ((rand() & 0xF) - 0x8) << 16;
            m->unk_10 = ((rand() & 0xF) - 0x8) << 16;
            m->unk_14 = ((rand() & 0xF) - 0x8) << 16;
            e = (u8 *)n->unk_0C;
            *(s16 *)(e + 0x16) = (rand() & 0x1FF) - 0x100;
            *(s16 *)(e + 0x18) = (rand() & 0x1FF) - 0x100;
            q = (u8 *)n + 0x20;
            *(s16 *)(q + 0x1A) = 0x20;
            *(u32 *)(q + 0x34) = *(u32 *)w;
            *(s16 *)(q + 0x38) = w->unk_04;
            *(u32 *)(q + 0x3C) = *(u32 *)a;
            *(s16 *)(q + 0x40) = a->unk_04;
            *(u32 *)(q + 0x44) = *(u32 *)((i << 3) + (u32)D_80026648);
            *(s16 *)(q + 0x48) = b->unk_04;
            *(u32 *)(q + 0x4C) = *(u32 *)(((ia + ib) << 3) + (u32)D_80026648);
            *(s16 *)(q + 0x50) = c->unk_04;
            if (i % 0x20 < 0x10)
            {
                *(u32 *)(q + 0x54) = D_800265C0;
                *(u32 *)(q + 0x58) = D_800265C4;
            }
            else
            {
                *(u32 *)(q + 0x58) = D_800265C0;
                *(u32 *)(q + 0x54) = D_800265C4;
            }
        }
    }
    D_80026476 = 0;
}
