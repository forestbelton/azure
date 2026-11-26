/* ovl_dng_s25, the child-sprite spawner of the 0x7E07 effect. 117 words.
   PARKED at 117 words / 3 differing (ovlverify) / 2 ALIGNED DIFFERENCES
   (aligndiff).

   THE WORD COUNT IS EXACT, EVERY REGISTER AGREES, AND THE RESIDUE IS ONE
   INSTRUCTION'S POSITION. The two aligned rows are a single transposition in
   the first block of the gate:

       target   15 lui   $v0, %hi(func_s25_80024340)   <- beqz delay slot
                16 addiu $v0, $v0, %lo(func_s25_80024340)
                17 sw    $s1, 0x20($s2)
                18 addiu $s1, $s2, 0x20
                19 sw    $v0, 0x10($s2)

       here     15 lui   $v0, %hi(func_s25_80024340)   <- beqz delay slot
                16 sw    $s1, 0x20($s2)
                17 addiu $s1, $s2, 0x20
                18 addiu $v0, $v0, %lo(func_s25_80024340)
                19 sw    $v0, 0x10($s2)

   Words 0-14 and 19-116 are byte-identical. Naming it by the five insns:
   HIGH, LO, SW20 (the owner store), CURSOR (p = n + 0x20) and SW10 (the step
   pointer store). The target emits HIGH LO SW20 CURSOR SW10; this body emits
   HIGH SW20 CURSOR LO SW10.

   WHAT THIS BODY IS. func_s25_800247D4's state 2 calls it ten times as
   func_s25_800244A0(p, b, i * -25, i) and keeps each result in the actor's
   +0x24 slot array. It allocates a 0x212 node, parks the caller's record
   pointer at n + 0x20 so the node's own record starts there, fills that
   record with the phase, a -6 - rand()%5 drift and the parent's x/y/z+0x20,
   dresses the renderable from D_800DECF8 at frame i % 5, scales it 0x1000
   both ways, and seeds the mover on a fresh rand()%4096 unit-circle angle.
   func_s25_80024340 is the per-frame step it installs, and that body is
   LANDED in this unit at 88 words / 0 differing -- the two interpret each
   other, which is how the record offsets +0x4/+0x6/+0x8/+0xA/+0xC were read
   rather than guessed.

   DONOR. dngcallee put nine functions at 7 of 8 symbols; none has this
   frame. The body was written from the listing plus the two spawners already
   landed in this unit, func_s25_80024148 and func_s25_80024264.

   THE RETURN SHAPE IS SETTLED AND IT IS THE IN-UNIT SIBLINGS' FACT INVERTED.
   Both landed spawners use `else { return NULL; } return n;' and the
   comments price the early return at 38 differing. Here the SAME shape is
   66 words / 13 differing and `return (s32)n;' as the last statement INSIDE
   the gate is 117 / 0 for that block. The sibling func_s25_800246D0, landed
   this run at 65 / 0, needed the same inversion. Do not restore the
   siblings' spelling.

   NINE SPELLINGS MEASURED, ALL LEAVING THE BIT-IDENTICAL TWO ROWS:

   | spelling                                              | words | aligned |
   | as written below                                       |  117 |    2    |
   | SW10 / SW20 / CURSOR in source order SW20 CURSOR SW10  |  117 |    2    |
   | ... SW20 SW10 CURSOR                                   |  117 |    2    |
   | ... SW20 CURSOR ph-store SW10                          |  117 |    5    |
   | SW10 as a plain cast, step store first                 |  117 |    2    |
   | SW10 as a plain cast, step store second                |  117 |    2    |
   | the ph store as the array ref ((s16 *)p)[2]            |  117 |    2    |
   | ... crossed with SW10 as n->unk_10                     |  117 |    2    |
   | a separate u32 local holding the step address          |  117 |    2    |

   The positional sweep of the step store across its four legal slots is
   2 / 2 / 2 / 5 -- a FLAT minimum, so the statement order is NOT MEASURABLE
   here rather than free, and the cause is not an order fault.

   WHY NO SOURCE PERMUTATION CAN REACH IT -- the one real result of this
   park, from -da on a single-function probe. In the sched2 dump every insn
   in this block carries priority = 1, so rank_for_schedule never separates
   two candidates on priority. Its second test classifies a candidate by its
   dependence on last_scheduled_insn, and insn_cost returns
   result_ready_cost, which is 1 for an addiu -- so the lo_sum is class 3,
   the same class as an independent insn, and the class test cannot separate
   them either. Everything therefore falls to the final
   `return INSN_LUID (tmp) - INSN_LUID (tmp2);', and the dump confirms the
   LOWER-LUID candidate is the one taken at every slot:

       ready list at T-70: 35 (1) 41 (1), now 35 41      -> 35 chosen
       ready list at T-71: 41 (1) 33 (1), now 33 41      -> 33 chosen
       ready list at T-72: 41 (1) 31 (1), now 31 41      -> 31 chosen

   sched2 fills the block BACKWARD, so T-70 is the LAST emitted slot of the
   five. Reproducing the target needs SW10 to win T-70 against CURSOR and
   CURSOR to win T-71 against LO, which is
   LUID(SW10) < LUID(CURSOR) < LUID(LO). But SW10 consumes LO's result, so
   LUID(LO) < LUID(SW10) holds in every insn chain. The three inequalities
   are inconsistent, so NO ORDER OF THESE STATEMENTS PRODUCES THE TARGET and
   the nine flat measurements above are exactly what that predicts.

   WHAT THAT MEANS FOR THE NEXT AGENT: stop sweeping spellings of these three
   statements. The remaining fault is STRUCTURAL -- the target's block does
   not contain this insn set, so one statement here is not the statement the
   original wrote. The LUID argument says the thing to doubt is the
   assumption that the %lo and the +0x10 store belong to ONE source
   statement; look for a form in which the step pointer reaches n + 0x10
   through a value whose address load is a separate insn chain that cse does
   NOT collapse. The plain `u32 st' local does collapse -- measured above,
   bit-identical.

   Line numbers are deliberately absent from the sched.c citations: the b18
   tree and FSF 2.7.2.3 drift. rank_for_schedule and insn_cost are both in
   scratch/gcc-src/cdk-gcc-b18/sched.c, findable by name.

   DECLARATIONS
   s32 rand(void);
   s32 func_8004491C(UnkNode *, u32);
   void func_8003DB94(void *, void *, s32);
   UnkNode *func_8003FC64(s32);
   s32 func_800644B8(s32);
   s32 func_80064584(s32);
   extern u8 D_80045340[];
   extern u8 D_800DECF8[];
   void func_s25_80024340(u8 *, UnkVec *, u8 *, u8 *);
   END DECLARATIONS
*/
s32 func_s25_800244A0(u8 *p, UnkVec *b, s16 ph, s16 i)
{
    UnkNode *n;
    u8 *m;
    u8 *m2;
    s16 r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s25_80024340;
        *(u8 **)((u8 *)n + 0x20) = p;
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x4) = ph;
        *(s16 *)(p + 0x6) = -6 - rand() % 5;
        *(s16 *)(p + 0x8) = *(u16 *)((u8 *)b + 0x2);
        *(s16 *)(p + 0xA) = *(u16 *)((u8 *)b + 0x6);
        *(s16 *)(p + 0xC) = *(u16 *)((u8 *)b + 0xA) + 0x20;
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x60;
        m[0xD] = 0x60;
        m[0xC] = 0x60;
        *(s16 *)(m + 0x12) = 0x7E07;
        *(u16 *)(m + 0x10) |= 0x60;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8003DB94(m, D_800DECF8, i % 5);
        *(s16 *)(m + 0x1E) = 0x1000;
        *(s16 *)(m + 0x1C) = 0x1000;
        func_8004491C(n, (u32)D_80045340);
        m2 = (u8 *)n->unk_08;
        r = rand() % 4096;
        *(s16 *)(m2 + 0x2) = *(u16 *)(p + 0x8) + ((func_800644B8(r) << 1) >> 8);
        *(s16 *)(m2 + 0x6) = *(u16 *)(p + 0xA) + ((func_80064584(r) << 1) >> 8);
        *(s16 *)(m2 + 0xA) = *(u16 *)(p + 0xC);
        return (s32)n;
    }
    return 0;
}
