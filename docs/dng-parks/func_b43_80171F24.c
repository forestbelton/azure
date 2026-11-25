/* PARK: func_b43_80171F24, csrc/ovl_dng_b43.c, agent-h49, 2026-11-23.

   TARGET 123 words. BEST BUILT 118 words, 11 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py; ovlverify reports 71 differing unaligned,
   which is the layout inflation the brief warns about).
   1 member, 123 words, 123 words total (tools/decomp/dngfam.py), so there
   is no replication value beyond its own body. This function owns NO jump
   table.

   TWIN: the landed carved sibling func_b44_80172050 in csrc/ovl_dng_b44.c
   (114 words), which is itself a transcription of the landed
   func_b45_80171EBC in csrc/ovl_dng_b45.c (113 words), which is the
   resident overlay's func_800CA788. Both of those are the SAME BODY as this
   one plus/minus the deltas below, and the three-way comparison is what the
   measurements here are built on.

   WHAT IT DOES. The banked room script's "same slot" branch: with the
   D_80083462 gates clear, ask func_800A04F0 what sits under the cursor, and
   if the two headings are within 0x40 of each other put the actor into state
   0x11, swap in this bank's D_80173FB8 facing table and re-aim through
   func_8009C93C.

   THE FIVE MISSING WORDS ARE TWO THINGS AND I COULD NOT SPELL EITHER.

   (1) THREE WORDS: the target evaluates `*(u16 *)(p + 0x98) & 0x8000' a
   SECOND time in the tail and throws the answer away. At 8017207C the
   listing has

       lhu   $v0, 0x98($s2)
       nop
       andi  $v0, $v0, 0x8000
       lui   $a1, %hi(D_80173FB8)
       addiu $a1, $a1, %lo(D_80173FB8)
       lui   $v0, %hi(D_80083228)

   and that last `lui' kills $v0, so the `andi' is dead in the shipped ROM.
   The same expression is the function's new entry gate at 80171F84, so this
   is a genuine second read, not a cse artefact. A dead insn can only survive
   if its consumer was deleted AFTER flow, and every plain-C shape I tried
   has its consumer deleted BEFORE flow, which deletes the compute with it.
   Nine shapes measured, all 118 words:

       duplicated arm around `d[0x85] = 0'            11   <- THE PARKED BODY
       duplicated arm around `e = D_80173FB8'         14
       duplicated arm around the two store statements 16
       duplicated arm around the whole publish        24   registers churn
       duplicated arm around the whole tail           26   two `jal's
       `d[0x85] = *(u16 *)(p + 0x98) & 0x8000'        17   folded in `fold'
       the same with an explicit `(u8)' cast          17   folded in `fold'
       `d[0x85] = (... & 0x8000) >> 16'               40   folded in `fold'
       `d[0x85] = (... & 0x8000) != 0 ? 0 : 0'        17   folded in `fold'

   An unused `s32 f = *(u16 *)(p + 0x98) & 0x8000;' in a `do { } while (0)'
   also reaches 11 and is the same residue as the parked body, so the device
   is worth the REFERENCE and not the insn -- consistent with
   docs/matching-levers.md's "A DUPLICATED ARM WITH IDENTICAL ARMS IS DELETED
   BY jump2; ITS ONLY EFFECT IS THE REFERENCE". Two shapes DID emit the three
   words and both put them in the wrong place:

       `__asm__("" ::"r"(f));' after the assignment    121 words, 19
       `do { e = ...; } while (f == 0x4000);'          124 words, 17

   the first because the asm pins the value in $a3 and pins the schedule, the
   second because the compare is not folded and costs a live constant and a
   `beq'. I think this is macro residue rather than a device: the natural
   reading is a two-table select on that flag that collapsed because THIS
   bank's two facings are the same symbol, which is exactly the kind of thing
   a relink does. If so the C is unrecoverable in principle and the right
   fix is a permuter search over the tail, which is the dispatcher's budget.

   (2) TWO WORDS: the target has a SECOND shared `j' / `addu $v0, $zero'
   return-0 pair. In the target the `t == NULL' return 0 keeps its own pair
   at index 31/32 and the func_800A2CB8 return 0 branches FORWARD to a pair
   at index 61/62 that the heading-window test falls into, so that test is
   `bnez $v0, .Lb43_80172020' + `nop'. Every build of mine cross-jumps the
   other way: the func_800A2CB8 return 0 goes BACKWARD to the `t == NULL'
   pair and the heading window branches straight to the epilogue with the
   zero in its delay slot, which is two words cheaper. An explicit `nope:'
   label on the heading-window `return 0' with a `goto nope' from the
   func_800A2CB8 arm measures the same 11, so jump2 rebuilds its own choice
   and the source cannot ask for the target's. Note this is NOT what the
   landed func_b45_80171EBC does either -- b45 has mine, both sites branching
   direct with the zero in the delay slot -- so the extra pair appears only
   once this body gains the third return 0.

   THE TWIN'S THREE FACTS, PRICED BOTH WAYS ON THIS BODY. b44's header names
   three; b45's names two. Outcomes:

     HELD  THE 0x40 FRAME NEEDS b45's UNREFERENCED 8-BYTE AGGREGATE LOCAL.
           Without `s32 unused[2];' the frame is 0x30 against the target's
           0x38 and it is 36 aligned differences -- every $sp offset and
           both ends of the frame. With it, 11. Only the size is witnessed,
           never the type.

     HELD  THE HEADING WINDOW'S UNARY MINUS IS NOT COSMETIC.
           `(u16)(0x40 + -func_800A0134(t, d)) > 0x80' is 11;
           `(u16)(0x40 - func_800A0134(t, d)) > 0x80' is 16 -- the
           subtraction reassociates and the `negu' goes. `>= 0x81' for
           `> 0x80' is the same word either way and measures 11, so b44's
           "free" on that one holds too.

     INVERTED  THE HEADING WINDOW'S CONSTANTS ARE b45's 0x40/0x80, NOT b44's
           0x3F/0x7E. b44 records 0x3F/0x7E as ITS copy's constants and the
           `negu'/`addiu 0x3F'/`andi 0xFFFF'/`sltiu 0x7F' that go with them;
           here the listing is `addiu 0x40'/`sltiu 0x81' and b44's pair
           measures 13 against 0x40/0x80's 11.

     INVERTED  d[0x85] IS b45's 0, NOT b44's 4. b44's `d[0x85] = 4;' is
           119 words and 13 aligned differences here.

     INVERTED  THE D_80083460 POINTER LOCAL IS WRONG HERE. Both b44 and b45
           hold the three gate reads through `g = (u8 *)&D_80083460;' and
           read `*(u16 *)(g + 0x2)'. This body's listing has
           `lui/lhu %lo(D_80083462)' at all three sites and SIX callee-saved
           saves where b45 has seven -- the seventh is b45's `g'. Spelled
           b45's way this body is 121 words and 33 aligned differences; with
           the direct `D_80083462' symbol it is 118 / 11. This is the
           cheapest single fact in the file and it is the one a transcription
           of the twin gets wrong.

     FREE  THE SHARED `-1' EXIT. b44 prices four plain `return -1;' with no
           label at 36 differing on its body. Here the `fail:' label and four
           plain `return -1;' statements BOTH measure 118 / 11, so reorg
           rebuilds the shared block by itself and the label is not what this
           body needs.

     FREE  THE FACING TABLE INLINE AT BOTH SITES WITH NO `e' LOCAL measures
           118 / 11, the same as the `e' local. b44 records the same.

   THE NEW GATE IS b43's OWN AND ITS ARM ORDER IS MEASURED. Neither b44 nor
   b45 has it. After the func_800A04F0 call this body tests
   `*(u16 *)(p + 0x98) & 0x8000' and, when the bit is SET, bails on a NULL
   cursor instead of calling func_800A2CB8 at all:

       the parked order, bit-set arm first         118 words, 11
       the arms the other way round, `== 0' first  119 words, 22
       two nested ifs instead of `else if'         118 words, 11

   so the `else if' and the explicit nesting are interchangeable and the arm
   ORDER is worth 11 rows and a word.

   WHAT DIFFERS, EXACTLY, IN THE PARKED BUILD (aligndiff rows):

       17, 37, 42, 50, 64   branch offsets only -- consequences of the
                            missing five words, not independent faults
       59, 60, 61           the heading window: target `bnez' + `nop' + `j',
                            built `beqz' with the zero in the delay slot
       86, 87, 88           the dead `lhu' / `nop' / `andi 0x8000'

   There is nothing else. Both measurable causes are named above and both
   are compiler-layout questions rather than missing statements, so a
   permuter run over the tail is the next step and not more reading.

   ===================================================================
   UPDATE, agent-h126, 2026-11-24, batch 30.  STILL PARKED at 118 words
   and 11 ALIGNED DIFFERENCES -- the body below is unchanged and is still
   the best on rows.  Nothing above is retracted; the twin
   func_b44_80172050 and the four call-graph donors in NEAR.txt were read
   and neither lends anything here.  What is new is that THE THREE WORDS
   ARE NOW REACHABLE IN PLAIN C, and a pass-order fact that explains the
   whole of this park's first table.

   WHY EVERY DUPLICATED ARM LOSES THE COMPUTE, from toplev.c.  In this
   compiler jump_optimize is called with cross_jump = 1 ONLY ONCE, at
   toplev.c:3690, AFTER reload and sched2; every earlier call
   (toplev.c:3266, 3312, 3373, 3379) passes cross_jump = 0 and the first
   and third pass after_regscan = 1.  So (a) identical arms cannot be
   cross-jumped before flow, yet they still collapse, and (b) a then-arm
   whose only insn is a DEAD SET is killed by the after_regscan dead-store
   removal in jump.c BEFORE flow, which turns the branch into a jump to the
   next insn, which the same pass deletes, after which flow removes the
   compute.  That is the mechanism behind every 118-word row above, and it
   is why no arm-shaped device can keep the three words.  Measured, all
   118 words unless stated:

       no device at all, plain `d[0x85] = 0;'              17   also moves
                                                                the 0x85 sb
       then-arm is `zz = 1;' (fresh dead s32 local)        17
       then-arm is `t = NULL;' (dead, t already dead)      11   identical
                                                                residue to
                                                                the parked
                                                                body, and one
                                                                line shorter
       then-arm is `e = e;'                                17   deleted
       then-arm is `c = c;'                                17   deleted
       then-arm is `d = d;'                                17   deleted
       `e = D_80173FB8;' dominating, then-arm repeats it   17   cse -> copy,
                                                                deleted
       the two arms are the SAME TWO INDEPENDENT STORES IN
         THE OPPOSITE ORDER (0x84 / 0x85)       125 words, 27
       the same with three stores               126 words, 17
   The last two are the interesting negative: sched2 does NOT normalise the
   order of two independent adjacent stores across two blocks, so the
   "arms identical only after scheduling" route is closed, and the arms
   survive as two real blocks plus the branch.

   THE THREE WORDS, IN PLAIN C, AT 121 WORDS AND 13 ROWS.  Give the ANDed
   value a consumer that flow cannot delete -- a store into the frame's
   unreferenced 8-byte local.  Replacing the duplicated arm with

       d[0x85] = 0;
       unused[0] = *(u16 *)(p + 0x98) & 0x8000;

   measures 121 words / 13 aligned differences and EMITS the second read:
   `lhu $v0, 0x98($s2)' and `andi $v0, $v0, 0x8000' land where the target
   has them.  `unused[1]' instead of `unused[0]' is the same 121 / 13.
   The remaining tail delta is one word in the wrong place, not a missing
   one: built `lhu / andi / sw $v0, 0x18($sp)' against target
   `lhu / nop / andi', and the `lui $a1, %hi(D_80173FB8)' schedules six
   words early.  So the carrier store is itself the last fault in the
   tail, and this is the build to scaffold a permuter run from -- NOT the
   body below -- because it is 2 words from the target where the body
   below is 5, and both of its remaining faults are named.
   For the record a volatile read keeps only the load:
       (void)(*(volatile u16 *)(p + 0x98) & 0x8000);   119 words, 14
   which is `lhu' alone -- the andi is dead and flow takes it.

   THE TWO WORDS (the second shared return-0 pair) ARE STILL UNSOLVED and
   two more shapes are now ruled out:
       the gate rewritten with no `else' at all -- `if (flag) { if (t !=
         NULL) goto cont; return 0; } if (func_800A2CB8(...) == 0)
         return 0; cont:'                                    11   FREE,
                                                                  identical
                                                                  to the
                                                                  `else if'
       `return (s32)t;' in the t == NULL arm                  11   FREE
       the arms the other way round in the goto form  119 words, 22
   The first of those kills the natural hypothesis -- that the t == NULL
   return 0 and the func_800A2CB8 return 0 merge BECAUSE they are sibling
   arms of one if/else.  They are not: separate them completely and jump2
   still merges them.  From jump.c, the merge is find_cross_jump over
   jump_chain[epilogue label], which is pushed at the head while insns are
   scanned forward (jump.c:259) and so is walked LATEST-FIRST, with
   minimum = 2 matching insns; all three `addu $v0, $zero, $zero' + jump
   blocks match at 2, so the choice is made inside the chain walk and no
   source ordering reached it.  Note the parked build already reproduces
   the t == NULL pair EXACTLY, rows 31..36 inclusive -- the only fault is
   where the func_800A2CB8 arm's branch points.

   ===================================================================
   UPDATE, agent-h147, 2026-11-25, batch 34.  NEW BEST: 121 WORDS /
   10 ALIGNED DIFFERENCES, and THE TWO WORDS ARE SOLVED.  The body below
   is the new one; nothing above is retracted.  One statement was added to
   the heading-window's return arm.

   THE MECHANISM, from jump.c, and it explains BOTH of the park's measured
   negatives on this question.  Call the three `return 0' sites S1 (t ==
   NULL), S2 (func_800A2CB8) and S3 (the heading window).  All three compile
   to a one-insn block `set v0 0' + `j epilogue'.  jump2 (toplev.c:3690,
   the only call with cross_jump = 1) cross-jumps them in `find_cross_jump'
   with minimum = 2, and a one-insn block can only reach minimum <= 0 by
   collecting ONE of the two bonuses in that function:

       jump.c:2409   if (GET_CODE (i1) == CODE_LABEL) { --minimum; break; }
       jump.c:2513   if (GET_CODE (i1) == JUMP_INSN && JUMP_LABEL (i1)
                         && prev_real_insn (JUMP_LABEL (i1)) == e1) --minimum;

   where i1 walks back from the jump being PROCESSED.  The second bonus is
   the jump-around-jump rule: it fires when the block sits immediately
   before the label its own guard branches to.  S1 never qualifies, because
   S2's block lies between S1's block and the if/else's join label, so
   prev_real_insn of that label is S2's jump and not S1's -- which is why
   S1's pair survives in every build and in the ROM.  S2 and S3 both
   qualify.  The chain `jump_chain[epilogue]' is pushed head-first during a
   forward scan (jump.c:217), so it is walked LATEST-FIRST: processing S2
   finds S3 and merges S2 into S3, which is the ROM's arrangement and puts a
   CODE_LABEL on S3's block.  Processing S3 then finds S1, collects the
   CODE_LABEL bonus from the label the first merge just created, and merges
   S3 into S1 as well.  That second merge is the whole fault.  It also
   explains the park's `nope:' measurement exactly: an explicit label in
   front of that block HANDS jump2 the bonus, so writing the goto by hand
   is 11 for the same reason doing nothing is.

   WHY LOSING S3's BLOCK COSTS TWO WORDS AND NOT ONE.  Once S3's block has
   no label, reorg's relax_delay_slots inverts the window's guard around it
   -- the transformation is gated on `no_labels_between_p (insn, next)' --
   and takes over the `j's delay slot, so `bnez' + `nop' + `j' + `addu'
   becomes `beqz' + `addu'.  In the ROM the label from the S2 merge blocks
   that inversion, which is also why the ROM carries a NOP at word 60 that
   no build of mine had.

   THE FIX IS TO DENY S3 BOTH BONUSES, and the only reachable way is to put
   ONE REAL NON-JUMP INSN between the window's guard and its `set v0 0':
   then i1 is an INSN where i2 is S1's JUMP_INSN, the codes differ, the loop
   breaks with no bonus, minimum stays 1 and the merge fails.  A frame store
   into the park's own unreferenced 8-byte local does it:

       if ((u16)(0x40 + -func_800A0134(t, d)) > 0x80)
       {
           unused[1] = 0;
           return 0;
       }

   Measured, all against the 123-word target:

       the park's 118-word body + `unused[1] = 0'   121 words, 10  KEPT
       the 121-word carrier scaffold + the same     124 words, 12
       the carrier scaffold alone (h126's)          121 words, 13
       the 118-word body alone (h49's)              118 words, 11

   In the 121/10 build the pair is EMITTED AT THE RIGHT ADDRESSES and the
   ROM's `nop' at word 60 is reproduced; rows 17, 37, 42, 50, 59 and 66 are
   branch offsets only.  The ENTIRE residue is now two things:

       61            the device's own `sw zero, 0x1C(sp)' -- one SURPLUS word
       86, 87, 88    the dead `lhu' / `nop' / `andi 0x8000', still missing

   and 121 - 1 + 3 = 123.  So the park's two open questions are now ONE
   question each, both worth one word.

   THINGS THAT DO NOT WORK, measured here:
       `unused[1] = 0' with a then-arm or `nope:' label as well     no change
       the `nope:' label on the scaffold                121 words, 13  (as h49)
       the same plus `return (s32)t' in the t == NULL arm 121 words, 13
       a nested `if (t != NULL) return 0; return 0;' in the window arm
                                                        121 words, 13
         -- the inner block is cross-jumped away, so it is NOT a real insn
       `*(u8 *)unused = 0' instead of `unused[1] = 0'   124 words, 12
       the carrier moved INTO the window arm,
         `unused[0] = *(u16 *)(p + 0x98) & 0x8000', tail plain
                                                        123 words, 20
         -- WORD-EXACT, and instructive: reorg sinks that store into the
            `j's own delay slot, so the carrier costs NOTHING there.  The
            dead `lhu'/`nop'/`andi' then land at words 61/62/63 instead of
            86/87/88, which is why it is 20 rows and not a landing.  It does
            prove the tail carrier can be free if its store can reach a
            delay slot.
       the same with `unused[1] = 0' in the window arm and a plain tail
                                                        121 words, 16

   THE NEW NEXT STEP, and it is now narrow enough to name a statement.
   Two one-word questions remain and either one alone leaves 123 words:

   (1) THE WINDOW ARM'S DEVICE MUST COST NO WORD.  It has to be a real
       non-jump insn at jump2 time -- a dead one is removed by flow's
       dead-store pass before jump2 and the block reverts (that is h126's
       `t = NULL' row, re-confirmed here) -- yet must not survive to the
       assembler.  reorg will not move it: it sits on the guard's
       FALL-THROUGH, and fill_slots_from_thread only steals from the TAKEN
       thread, so the `nop' at word 60 stays a `nop' and our insn stays at
       word 61.  I could not find such a statement.  The alternative is to
       pay the word somewhere the ROM also pays it.
   (2) THE TAIL CARRIER MUST REACH A DELAY SLOT, as it does in the window
       arm.  The ROM's tail between `sb zero, 0x85' and `lui %hi(D_80173FB8)'
       has no jump to hide a store in; the nearest is the
       `jal func_80047784' whose slot the ROM fills with its own
       `addu $a2, $zero, $zero' third argument.  Passing the ANDed value as
       that third argument instead is the one shape I did not get to measure
       and is the first thing I would try.

   For the dispatcher: a permuter run over the HEADING-WINDOW ARM and the
   `d[0x85]' tail of the 121/10 body below is now a two-word search with
   both faults named and localised, where h126 could only offer a five-word
   one.  The statement to search is the arm
   `if ((u16)(0x40 + -func_800A0134(t, d)) > 0x80) { <stmt>; return 0; }'
   together with the two `d[0x85]' / p+0x98 statements in the tail.

   ===================================================================
   DECLARATIONS
   u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
   s16 func_800A0134(u8 *a, u8 *b);
   s16 func_800A2CB8(u8 *a, u8 *b);
   s32 func_800A2B5C(void *d);
   s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
   u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);
   void func_80047784(u8 *p, s32 b, s32 c);
   extern u16 D_80083462;
   extern s16 D_80083228;
   extern u8 D_80173FB8[];
   END DECLARATIONS */
s32 func_b43_80171F24(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;
    u8 *e;
    s32 unused[2];

    d[0x71] &= 0x7F;
    if ((D_80083462 & 0x2000) != 0)
    {
        goto fail;
    }
    t = func_800A04F0_r(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A));
    if ((*(u16 *)(p + 0x98) & 0x8000) != 0)
    {
        if (t == NULL)
        {
            return 0;
        }
    }
    else if ((s16)func_800A2CB8(d, t) == 0)
    {
        return 0;
    }
    if ((D_80083462 & 0x2000) != 0)
    {
        goto fail;
    }
    if ((*(u16 *)(d + 0x46) & 0x8000) == 0 && (D_80083462 & 0x8) != 0)
    {
        goto fail;
    }
    if ((u16)(0x40 + -func_800A0134(t, d)) > 0x80)
    {
        unused[1] = 0;
        return 0;
    }
    if ((s16)func_800A2B5C(d) != 0)
    {
        goto fail;
    }
    func_800C7930(d - 0x20, (u8 *)b, 8, 0x300);
    if ((s16)func_800A2B5C(d) != 0)
    {
    fail:
        return -1;
    }
    p[0x9A] = 0x11;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    d[0x84] = 0x7C;
    if ((*(u16 *)(p + 0x98) & 0x8000) != 0)
    {
        d[0x85] = 0;
    }
    else
    {
        d[0x85] = 0;
    }
    e = D_80173FB8;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    return 1;
}
