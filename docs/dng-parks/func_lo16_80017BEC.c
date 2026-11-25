/* func_lo16_80017BEC -- ovl_dng_lo16, 180 target words.
   PARKED by agent h181, batch 35, 2026-11-25.  180 words / 51 ALIGNED
   DIFFERENCES.  Every instruction, immediate and displacement matches except
   ONE instruction position; the other 50 rows are CALLER-SAVED REGISTER NAMES
   in two blocks.  The word count has been exact since the first structural
   fix, so read the figures below as a register-allocation residue, not a
   shape problem.

   WHAT IT IS: the auto-walk that drags the party marker from the spot
   func_lo16_80017F88 picks to a reachable neighbour, step by step, until
   func_lo16_80017EBC's open-cell count runs out.  It is NOT a twin of
   anything; its callees are all in this unit.

   THE FOUR-ARGUMENT CALL IS REAL AND IT MATTERS.  At 0x80017E24 the listing
   is `jal func_lo16_8001816C' with `addiu $a3, $zero, 0x1' computed
   immediately before it and dead after.  func_lo16_8001816C is LANDED in this
   unit at 102/102 with THREE parameters and does not read $a3.  So the
   original declaration had four and the definition ignores the fourth; the
   project fix is the unit-local alias

       s32 func_lo16_8001816C_4(s16, s16 *, s16 *, s16)
           __asm__("func_lo16_8001816C");

   which leaves the landed definition alone.  Do not add a fourth parameter to
   the definition.  The sibling func_lo16_80017F88 (parked, 121 words) is the
   same routine WITH the fourth parameter live, which is why the pair exists.

   THE ONE REMAINING INSTRUCTION-POSITION FAULT:

       -        94 80017D64      sll $v0, $v0, 1       in the bltz delay slot
       +   96                    sll a0,v0,0x1         two insns later

   `best * 2' -- the shared index for the two step tables -- belongs in the
   `bltz' delay slot.  In the candidate sched1 ranks the `la D_8006CCD8' ahead
   of it, so reorg fills the slot with the `lui' instead.  At sched1 both are
   one insn (`la' is still unexpanded), so the priorities TIE and
   rank_for_schedule breaks the tie on LUID; the target wants the shift to
   have the later LUID.  Every source reordering of the two table statements
   was measured and none moves it:
       ox then oy, both `(u16)' casts (as below)                 51
       oy then ox                                                79
       table read as the FIRST operand of the `+'                77
       `ox += ...' instead of `ox = ox + ...'                    70 variant
       `&D_8006CCD8[best]' into a pointer local first            83
       `*(s16 *)((u8 *)D_8006CCD8 + best * 2)'                   87

   THE 50 REGISTER ROWS ARE LOCAL-ALLOC, AND THE DISCRIMINATOR IS VISIBLE IN
   THE TARGET.  In the best-step block the target holds seven live
   caller-saved values -- $v0 the index then the dy pointer then the cell
   pointer, $v1 the dx pointer then the cell flags, $a0, $a1 the oy sum, $a2
   the ox sum, $a3 the sign-extended ox, $t0 the sign-extended oy -- so
   registers 2..8 are all taken and the `la' temp lands in $t1.  The candidate
   assigns the same block differently and $v1 is free for the `la', so every
   name in the block rotates.  The two sched1 orderings differ by the one
   instruction above, and local-alloc PRICES THE SCHED1 ORDER, so the single
   remaining position fault and the fifty names are very likely ONE fault.
   Fix the delay slot first.

   FACTS MEASURED AND LOAD-BEARING, with the cost of the alternative:

   1. THE THREE SINGLE-USE CELL POINTERS MUST BE THREE SEPARATE LOCALS
      (`c1', `c2', `c3'), with `c' kept only for the inner loop.  As ONE
      function-scope `c' the pointer becomes a GLOBAL allocno -- 24 refs
      across 25 insns, priority 3.84, allocated second -- and takes $a3 for
      the whole body where the target uses a different caller-saved register
      in each block: 180 words / 70 differing.  Measured splits:
          all three separate, `c' for the inner loop only          51
          the two tail sites separate, site 1 on `c'               58
          the two tail sites sharing one extra local               70
          site 1 separate only                                     66
          site 1 separate plus the tails sharing                   63
      Inlining the three sites as `((UnkDungeonCell *)...)->unk_04 |= 0x100'
      measures the same 51, so the three locals are a readability choice and
      not a measured fact; the SPLIT is the fact.
   2. TWO `do { } while (0)' REFERENCE BUMPS ON `h', AND THEY ARE WORTH
      NOTHING APART.  `h' and `best' are both 2-byte allocnos competing for
      $s4: as written, `h' is 8 refs over 85 insns (3 * 8 / 85 = 0.282) and
      `best' 7 over 42 (2 * 7 / 42 = 0.333), so `best' is allocated first and
      takes $s4 where the target has `h'.  Wrapping BOTH `h ='  calls in the
      loop takes `h' to 10 refs (3 * 10 / 85 = 0.353) and the allocation order
      in the .greg dump flips from `... 194 92 93 94 ...' to
      `... 194 93 92 94 ...'.  Measured: both wrappers 51, either one alone
      58, neither 58.  Nine refs is not enough -- 3 * 9 / 85 = 0.318 -- which
      is why one wrapper is free.  This is the brief's "worse or neutral
      alone, necessary in combination" exactly.  Probably macro residue: both
      sites are a height probe.
   3. THE SHARED `return 0' BLOCK IS REACHED BY `goto' FROM INSIDE THE LOOP,
      and the label sits INSIDE the gate's then-arm.  The target's layout is
      `bnez $v0, body' / `nop' / L17C60: `j epilogue' / `v0 = 0' / `body:',
      so the block is between the gate and the body and L17C60 is a BACKWARD
      branch target.  Written as three ordinary `return 0' statements
      jump_optimize cross-jumps them all onto the LAST one and then inverts
      the gate: the block moves to the end, the `return 1' block needs a `j'
      of its own, and it is 77 differing.  A `break' out of the `for (;;)'
      with a trailing `return 0' is the same 77 shape.  BOTH the
      `found == 0' exit AND the else-arm's `h >= 0x200' exit must be
      `goto fail' -- with only one of them converted it is still 84 -- because
      a second `move v0,0' / `j' block anywhere gives cross_jump something to
      merge.  `fail:' placed after the gate instead of inside it measures the
      same 51.
   4. `i' and `found' are `s32'; `h', `cnt', `best' and `ox'/`oy' are `s16';
      `sr' is `u16'.  As `s16 i' the latch needs a sign extension and a copy
      (`addiu $v0,$s0,-1' / `move $s0,$v0' / `sll') where the target has a
      bare `addiu $s0,$s0,-1' / `bgez $s0'; as `s16 found' the `beqz $s3'
      needs its own `sll'.  `u16 sr' is what gives `sh $v1, 0x18($sp)' of the
      RAW parameter and `lhu $t1, 0x18($sp)' + a sign extension at the use --
      it is spilled in both target and candidate.
   5. THE CELL BASE IS AN `s32' `cb', not a `UnkDungeonCell *', and the
      subscript is `(ox + (oy << g->unk_14)) * 6 + cb' -- INDEX FIRST.  Same
      rule as func_lo16_80017F88's park fact 1 and func_lo16_80019A84.
   6. `g' IS A FUNCTION-SCOPE LOCAL here (nine refs across 282 insns, $fp in
      both), which INVERTS func_lo16_80017F88's park fact 2 in the same unit.
      The frame is the discriminator: this body saves TEN callee-saved
      registers plus $fp and has one to spare.
   7. The inner loop walks the two step tables with DECREMENTING POINTERS
      initialised in TWO steps -- `bd = D_8006CCE8; p2 = bd + 7;' -- which is
      the brief's two-pointer-locals rule.  `p2 = &D_8006CCE8[7]' folds the
      +0xE into the R_MIPS_LO16 addend and loses the target's third insn.
      The two bases must be TWO locals, not one reused: merged into one
      pseudo it is 80 differing.

   NEXT AGENT: get `sll $v0, $v0, 1' into the `bltz' delay slot.  Nothing else
   in this body is unexplained.

   DECLARATIONS
   extern u32 D_8008333C[];
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   typedef struct UnkDungeonCell
   {
       u16 unk_00;
       u16 unk_02;
       u16 unk_04;
   } UnkDungeonCell;
   typedef struct UnkDngLo16Grid
   {
       UnkDungeonCell *unk_00;
       u8 pad_04[0x10];
       s16 unk_14;
       s16 unk_16;
       s16 unk_18;
       s16 unk_1A;
       s16 unk_1C;
       s16 unk_1E;
   } UnkDngLo16Grid;
   s32 func_lo16_80017F88(s16, s16 *, s16 *, s16);
   s32 func_lo16_8001816C_4(s16, s16 *, s16 *, s16) __asm__("func_lo16_8001816C");
   s16 func_lo16_80017EBC(s16);
   s32 func_lo16_80018304(s16, s16, s16, s16);
   s32 func_800BCB04(s32, s32, s16);
   END DECLARATIONS */

s16 func_lo16_80017BEC(s16 r)
{
    UnkDngLo16Grid *g;
    UnkDungeonCell *c;
    UnkDungeonCell *c1;
    UnkDungeonCell *c2;
    UnkDungeonCell *c3;
    s16 *bd;
    s16 *be;
    s16 *p1;
    s16 *p2;
    s32 cb;
    s16 ox;
    s16 oy;
    u16 sr;
    s16 best;
    s16 h;
    s16 cnt;
    s32 found;
    s32 i;
    u16 v;

    cb = (s32)D_8008333C[0];
    g = (UnkDngLo16Grid *)D_8008333C;
    sr = r;
    h = (s16)func_lo16_80017F88(r, &ox, &oy, 0);
    if (h >= 0x200)
    {
    fail:
        return 0;
    }
    {
        c1 = (UnkDungeonCell *)((ox + (oy << g->unk_14)) * 6 + cb);
        c1->unk_04 |= 0x100;
        cnt = func_lo16_80017EBC(r);
        for (;;)
        {
            best = -1;
            found = 0;
            i = 7;
            bd = D_8006CCE8;
            p2 = bd + 7;
            be = D_8006CCD8;
            p1 = be + 7;
            for (; i >= 0; i--)
            {
                if ((s16)func_lo16_80018304(ox, oy, h, i) != 0)
                {
                    c = (UnkDungeonCell *)(((ox + *p1) + ((oy + *p2) << g->unk_14)) * 6 + cb);
                    v = c->unk_04;
                    if ((v & 0x100) == 0)
                    {
                        c->unk_04 = v | 0x200;
                        best = i;
                    }
                    found++;
                }
                p2--;
                p1--;
            }
            if (found == 0)
            {
                goto fail;
            }
            if (best >= 0)
            {
                ox = (u16)ox + (u16)D_8006CCD8[best];
                oy = (u16)oy + (u16)D_8006CCE8[best];
                c2 = (UnkDungeonCell *)((ox + (oy << g->unk_14)) * 6 + cb);
                cnt--;
                c2->unk_04 |= 0x100;
                if (cnt <= 0)
                {
                    return 1;
                }
                do
                {
                    h = (s16)func_800BCB04((ox * 64 + 0x20) & 0xFFE0,
                                           (oy * 64 + 0x20) & 0xFFE0, -0x400);
                } while (0);
            }
            else
            {
                do
                {
                    h = (s16)func_lo16_8001816C_4((s16)sr, &ox, &oy, 1);
                } while (0);
                if (h >= 0x200)
                {
                    goto fail;
                }
                c3 = (UnkDungeonCell *)((ox + (oy << g->unk_14)) * 6 + cb);
                cnt--;
                c3->unk_04 |= 0x100;
                if (cnt <= 0)
                {
                    return 1;
                }
            }
        }
    }
}
