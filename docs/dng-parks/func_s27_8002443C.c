/* PARK: func_s27_8002443C, csrc/ovl_dng_s27.c, agent-h196, 2026-11-25.

   TARGET 117 words. BEST BUILT 117 words, 2 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py), 3 differing by ovlverify. 1 member, 117 words
   (dngfam.py), so no replication value.

   ==== AXIS CLOSED WITH THE MECHANISM, agent-h229, 2026-11-26.  THE RESIDUE
   IS A REGISTER-BUDGET WALL, NOT A SCHEDULE TIE-BREAK, AND THE EXISTING
   LEVER'S PROOF FOR THE IDENTICAL SIBLING RESIDUE IS WRONG ABOUT WHY.
   See THE h229 RESULT at the end of this header.  Nothing above it changed.

   THE RESIDUE IS A THREE-INSTRUCTION ROTATION IN ONE BASIC BLOCK AND NOTHING
   ELSE. Every other word of the 117 is exact, including the %hi/%lo pair, the
   register assignment and the frame. The block is the head of the non-NULL
   arm:

     target     15 lui   $v0, %hi(func_s27_800242E0)   (beqz delay slot)
                16 addiu $v0, $v0, %lo(func_s27_800242E0)
                17 sw    $s1, 0x20($s3)
                18 addiu $s1, $s3, 0x20
                19 sw    $v0, 0x10($s3)

     built      15 lui   $v0, %hi(...)                 (beqz delay slot)
                16 sw    $s1, 0x20($s3)
                17 addiu $s1, $s3, 0x20
                18 addiu $v0, $v0, %lo(...)
                19 sw    $v0, 0x10($s3)

   So the `addiu' that completes the symbol address is scheduled LAST here and
   FIRST in the target; the set of instructions is identical. This is a sched
   priority/tie-break question, not a shape question.

   WHAT MADE THIS BODY, measured on this body:

   * THE PAYLOAD CURSOR MUST REUSE THE PARAMETER `v'. A separate `u8 *w'
     local is 120 words / 31 aligned rows -- it takes a SIXTH callee-saved
     register (frame 0x30 against the target's 0x28) because with two
     pseudos there is no anti-dependence forcing `sw $s1, 0x20($s3)' ahead of
     `addiu $s1', sched interleaves them and their live ranges overlap. With
     `v' reused the anti-dependence exists and global.c shares $s1.
     Variants measured: `w' with the step store first 118 w / 103; `w' third
     118 w / 103.

   * `return n' INSIDE THE then-ARM AND `return NULL' AFTER THE GATE.
     This is the shape; the other two cost:
       `if (n != NULL) { ...; return n; } return NULL;'  117 w,  2 rows  <-- kept
       `if (n != NULL) { ... } else { return NULL; } return n;'  118 w,  9 rows
       `if (n == NULL) { return NULL; } ...; return n;'   117 w,  6 rows
     The else-arm form is the landed func_s27_800241F8's shape in this same
     unit, so the fact INVERTS between two bodies 0x244 apart.

   * `func_8003DB94(m, D_800DEC70, (s16)(a % 16))' -- the third argument is
     sign-extended at the call (`sll 16' / `sra 16' after the `subu'), which
     with the unit's `s32' third parameter has to be an explicit cast. The
     same code comes from declaring that parameter `s16'; this body cannot
     tell them apart and the cast was preferred because it leaves the two
     landed callers of func_8003DB94 in this unit untouched.

   SPELLINGS MEASURED AND REJECTED, all at 117 words unless stated:
     statement order unk_10 / +0x20 / cursor, all 6 permutations      3
     `*(u32 *)((u8 *)n + 0x10) = (u32)func_s27_800242E0'              3
     `u32 fptr = (u32)func_s27_800242E0; n->unk_10 = fptr;'           3
     `if (1) { ... }' round the +0x20 store                           3
     `do { } while (0)' round the +0x20 store / the cursor / both    19
     `do { } while (0)' round the unk_10 store                       18/19
     cursor store before the +0x4 store, unk_10 last                  5
     the rand()%5 store before the k store                           17
     `k' as s32                                                      11
     `k' as u16                                                       3
     `a' as s32 with `(s16)((s16)a % 16)'                             3
     `v' as UnkNode * throughout                                      3
   (those figures are ovlverify counts; the baseline reads 3 there and 2 by
   aligndiff, so every row above is FLAT.)

   ==== THE h229 RESULT, 2026-11-26.  TWELVE MORE CELLS, AND THE CAUSE.

   The identical residue on the identical body shape is recorded in
   docs/matching-levers.md under "A `%lo` TRANSPOSED AGAINST INDEPENDENT INSNS
   CAN BE PROVED UNREACHABLE BY STATEMENT ORDER" (func_s25_800244A0, 117
   words, 2 rows, same five words).  That entry proves no STATEMENT ORDER
   reaches it from sched2's LUID tie-break and says the fault must be the
   statement SET.  The statement-SET axis has now been swept and the real
   cause is one pass earlier.

   THE BLOCK, read off the listing rather than inferred.  The block runs from
   the beqz's fall-through to the first `jal', seven insns, and `tools/decomp/
   sched.py --insn' names them:

     32  sw $s1, 0x20($s3)       the old payload pointer  (C)
     35  addiu $s1, $s3, 0x20    the cursor, REG_DEP_ANTI on 32  (D)
     37  lui %hi(func_s27_800242E0)   (A)
     39  lo_sum  -> $v0          (B, depends on A)
     41  sw $v0, 0x10($s3)       (E, depends on B)
     44  sh $s0, 0x4($s1)        (F, depends on D; ends in the jal's slot)
     47  jal rand

   sched2 emits in descending-LUID order here -- every insn prints priority 1,
   so it is the identity on the chain -- and the chain entering sched2 is
   sched1's output.  SCHED1 IS WHERE THE ROTATION IS DECIDED, and the decider
   is `adjust_priority' -> `birthing_insn_p' in sched.c (scratch/gcc-src/
   cdk-gcc-b18, cite by name; LAUNCH_PRIORITY is 0x7f000001 at sched.c:189).
   An insn with no REG_DEAD notes -- and the file says they are all stripped
   before sched, so that is every insn -- whose pattern is `(set (reg) ...)'
   with the dest live and `reg_n_sets[dest] == 1' is given max_priority, which
   `schedule_insn' has just set to LAUNCH_PRIORITY.  The dumps show it:

     base body,  sched1  T-58: 35 (1) 39 (7f000001), now 39 35
     base body,  sched2  T-56: 35 (1) 39 (1),        now 39 35

   So the `lo_sum' is LAUNCHED down to the slot immediately before its
   consuming store, whatever the statement order, and the cursor escapes the
   boost ONLY because `v' is a reused parameter and so has reg_n_sets == 2.
   The target's order needs the cursor to win that slot against the boosted
   `lo_sum', which needs the cursor boosted too -- i.e. a single-set local.

   CONFIRMED ON A STANDALONE PROBE (scratch probe, cdkgcc -O2 -G0), which is
   what makes this a mechanism rather than a dump reading.  Same five words,
   four cells:

     address inline, cursor = reused parameter    sw / addu / lo_sum / sw
     address in its own first statement, same     sw / addu / lo_sum / sw
     address inline, cursor a single-set local    sw / addu / lo_sum / sw
     BOTH: own statement AND single-set cursor    lo_sum / sw / addu / sw
                                                  = THE TARGET'S ORDER

   AND ON THIS BODY THE PAIR IS A REGISTER WALL.  Both conditions together DO
   fix the rotation -- and cost exactly one callee-saved register:

   | address statement | cursor      | words | aligned | note                |
   |-------------------|-------------|-------|---------|---------------------|
   | inline (park)     | reuse `v'   |  117  |    2    | the rotation        |
   | own, 1st in arm   | reuse `v'   |  117  |    2    | BIT-IDENTICAL rows  |
   | own, via u32      | reuse `v'   |  117  |    2    | BIT-IDENTICAL       |
   | own, fn-ptr type  | reuse `v'   |  117  |    2    | BIT-IDENTICAL       |
   | own, plain-cast store          |  117  |    2    | BIT-IDENTICAL       |
   | own + dead `fptr = 0' first    |  117  |    2    | boost still fires   |
   | own + dead `fptr = 0' last     |  117  |    2    | boost still fires   |
   | own + `fptr' reused for D_80045340 | 117 |   2    | boost still fires   |
   | own, 1st in arm   | `u8 *w'     |  119  |   25    | ROTATION FIXED, 6th |
   | own, decls reordered | `u8 *w'  |  119  |   25    | ROTATION FIXED, 6th |
   | via `m' (2 sets)  | reuse `v'   |  119  |   25    | 6th register        |
   | via `r' (2 sets)  | reuse `v'   |  119  |   25    | 6th register        |

   WHY THE DEAD SET DOES NOT WORK, and it is worth knowing wherever
   `reg_n_sets' is the lever: `reg_n_sets' is counted by `life_analysis' in
   flow.c (flow.c:2102/2120/2378/2781), and flow DELETES the dead store in the
   same pass, so it is never counted.  This is the opposite timing from the
   `regno_first_uid' lever in docs/matching-levers.md ("A DEAD INITIALISER
   MAKES cse KEEP A REG-REG COPY"), where `reg_scan' runs early and a dead set
   DOES count.  A DEAD SET IS FREE FOR cse AND INVISIBLE TO sched1.

   AND THE BUDGET IS EXACTLY FULL.  The target's five callee-saved registers
   are n ($s3), the cursor sharing $s1 with the parameter `v', q ($s2),
   a ($s4) and m ($s0); `a' is live across the first rand(), `q' across it
   too, and `m' across func_8003DB94, so none of them can be given up without
   moving a word that already matches.  Every cell that supplies either
   precondition adds a sixth, and that is +2 words in the prologue/epilogue.

   > SO THE AXIS IS CLOSED: the rotation is reachable, the price is one
   > callee-saved register, and this body has none to spend.  Do not re-run
   > any 117-word spelling above -- all eight are bit-identical -- and do not
   > re-run the separate-cursor family; it is 119/25 in four independent
   > forms.  What is NOT closed is the possibility that one of the five
   > callee-saved values can be freed by a shape nobody has found, which is a
   > question about the OTHER 112 words, not about these five.

   DECLARATIONS
   void func_s27_800242E0(u8 *p, UnkVec *b, u8 *c, u8 *d);
   s32 rand(void);
   s32 func_800644B8(s32);
   s32 func_80064584(s32);
   END DECLARATIONS
*/
UnkNode *func_s27_8002443C(u8 *v, UnkVec *q, s16 k, s16 a)
{
    UnkNode *n;
    u8 *m;
    u8 *m2;
    s32 r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        *(u8 **)((u8 *)n + 0x20) = v;
        v = (u8 *)n + 0x20;
        n->unk_10 = (u32)func_s27_800242E0;
        *(s16 *)(v + 0x4) = k;
        *(s16 *)(v + 0x6) = -6 - rand() % 5;
        *(s16 *)(v + 0x8) = *(u16 *)((u8 *)q + 0x2);
        *(s16 *)(v + 0xA) = *(u16 *)((u8 *)q + 0x6);
        *(s16 *)(v + 0xC) = *(u16 *)((u8 *)q + 0xA);
        m = (u8 *)n->unk_0C;
        m[0xE] = 0;
        m[0xD] = 0;
        m[0xC] = 0;
        *(s16 *)(m + 0x12) = 0x7E09;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x20;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8003DB94(m, D_800DEC70, (s16)(a % 16));
        *(s16 *)(m + 0x1E) = 0x1000;
        *(s16 *)(m + 0x1C) = 0x1000;
        func_8004491C(n, (u32)D_80045340);
        m2 = (u8 *)n->unk_08;
        r = rand() % 4096;
        *(s16 *)(m2 + 0x2) = *(u16 *)(v + 0x8) + ((func_800644B8(r) << 1) >> 8);
        *(s16 *)(m2 + 0x6) = *(u16 *)(v + 0xA) + ((func_80064584(r) << 1) >> 8);
        *(s16 *)(m2 + 0xA) = *(u16 *)(v + 0xC);
        return n;
    }
    return NULL;
}
