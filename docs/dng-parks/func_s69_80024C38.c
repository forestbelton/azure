/* PARK: func_s69_80024C38, csrc/ovl_dng_s69.c, agent-h37, 2026-11-23.

   DISPATCHER DECISION on this park's two proposed next steps: NEITHER, for
   now. Both are mine to spend and I am declining both, with reasons, so the
   next reader does not re-raise them.

   NO PERMUTER RUN. The park asks for one on the grounds that this is a
   sched2 INSN_LUID tie and that is what the permuter is good at. True in
   general, and the park's own analysis is the argument against it here: the
   requirement is that `q' be a ONE-SET pseudo so `birthing_insn_p' promotes
   it to LAUNCH_PRIORITY in sched1, and the park then shows that two separate
   `q' locals cost a callee-saved register -- 85 differing, frame 0x30 against
   the target's 0x38 -- while the target's eight callee-saved quantities leave
   no room for a ninth. It also already swept the direct axis: SIX positions
   for the `q' assignment, all 2 differing. A permuter permutes and respells
   statements; it does not change a variable's set count, so it would be
   searching for something this park has argued is not in the search space.
   If that argument is wrong, the way to show it is to find a spelling that
   makes `q' one-set WITHOUT a second local -- then the permuter has something
   to find.

   NO `__asm__' MARKER. The park is right that matching-levers.md:14498 gives
   the sched1 priority arithmetic and that a zero-word marker could occupy a
   slot between T-7 and T-17. It is still an asm device on a 2-word
   scheduling residue in a body that is otherwise exact, which is the case
   the project's own rule reserves for last. The park was right not to spend
   one without asking.

   WHAT THIS IS WORTH, stated so the trade is visible: 138 words x 2 members
   = 276 words, against roughly 213,000 still in stubs. A batch lands about
   5,000. So this is a fifth of a percent of one batch, and the units the
   twin sweep prices cheapest (b43 at 68% of its stub words within a few
   instructions of landed C, b06 53%, b41 48%) have never been picked. The
   diagnosis here is the asset, not the two words: it is the first park in
   this sweep to name `birthing_insn_p' and `reg_n_sets' as the gate, and
   that mechanism will recognise the next function that hits it faster than
   this one would land.

   Revisit when a lever arrives that changes a pseudo's SET COUNT without
   costing a register. Until then this park is a reference, not a queue item.


   TARGET 138 words. BEST BUILT 138 words, 2 differing (ovlverify) and
   4 ALIGNED DIFFERENCES (tools/decomp/aligndiff.py -- the 4 is one
   transposed PAIR counted as two deletions plus two insertions).
   2 members, 138 words each, 276 words total (tools/decomp/dngfam.py:
   func_s55_8002589C in ovl_dng_s55 is the other, still an INCLUDE_ASM, and
   no park file exists for it).

   THE WHOLE RESIDUE IS ONE PAIR OF ADJACENT INSTRUCTIONS, in loop 1's
   third position store:

       target     lw $v1,0x8($s3) / andi $v0,$v0,0x3F / lhu $v1,0xA($v1) /
                  nop             / addu $v1,$v1,$v0
       built      lw $v1,0x8($s3) / nop                / lhu $v1,0xA($v1) /
                  andi $v0,$v0,0x3F / addu $v1,$v1,$v0

   Every other word of the 138, every register and every relocation agrees,
   including the identical statement in loop 2, where the target ALSO emits
   the built order (lw / nop / lhu / andi). The two nops are maspsx
   load-delay fill, so the compiler question is only whether sched2 emits
   the `andi' before or after the `lhu'.

   WHAT IT DOES. Two bursts of two 0x212 nodes each off the caller's node.
   Loop 1: each node gets func_s69_80024B00 as its +0x10 step, D_80045340
   through func_8004491C, a position jittered (rand() & 0x3F) - 0x20 on each
   of the three axes from the CALLER's mover, this bank's D_80027484 at the
   slot's +0x8, the caller's colour word at +0xC, 0x1000/0x1000 scales,
   0x8 at +0x6, `b' at the payload's +0x36 and a (rand() & 0xFF) | 0x80
   life at +0x30. Loop 2: the same step and the same jitter, but
   func_800C95C0 as the func_8004491C argument and no slot dressing at all
   -- instead the payload's first four halfwords get 0x20, 0x20, 1, 0.
   `for (i = 2; i > 0; i--)' twice, which is the `addiu $s2,-1' / `bgtz'
   with no entry test and the `addiu $s2,$zero,2' reload between them.

   THE TEMPLATE is csrc/ovl_dungeon_800CABA8.c's landed func_800CAFDC
   (xsibs 0.686): same func_8003FD64(0x212, a) / +0x10 install /
   func_8004491C / three `*(u16 *)((u8 *)a->unk_08 + N) + (rand() & 0x3F)
   - 0x20' jitters re-reading a->unk_08 each time / `r = (u8 *)n->unk_0C'
   with 0x1E before 0x1C / `q = (u8 *)n + 0x20' tail. Its own offsets are
   +0xB0 and +0x96 where this copy has +0x36 and +0x30, and it has a
   velocity from func_80064584/func_800644B8 that this copy does not.
   dngtwin.py finds NO carved sibling above 0.62 other than the s55 clone.

   THE MECHANISM, read out of tools/decomp/sched.py on the built body and
   gcc 2.7.2.3's sched.c. It is a tie, and the tie is decided two passes
   earlier.

     * In SCHED2 the ready list at T-18 is exactly
       `131 (7) 113 (7), now 113 131': insn 113 is `andi $v0,$v0,0x3F' and
       insn 131 is `addiu $s1,$s0,0x20' (the `q' assignment). Equal
       priority, and both are ALU, so `potential_hazard' (which promotes
       memory over ALU) does not separate them -- `rank_for_schedule'
       falls through to its third key, INSN_LUID, and picks 113. The
       target needs 131 picked here: 131 then takes T-18, 115 (the `lhu')
       takes T-19 and 113 takes T-20, which emits lw / andi / lhu, and
       reorg afterwards steals the `addiu $s1' into the FIRST rand call's
       delay slot in both versions, leaving the hole the target's nop
       fills.

     * INSN_LUID is SCHED1's output order. In sched1 the same two insns meet
       at the same T-18 with priorities `131 (1) 113 (7f000001)'.
       0x7f000001 is LAUNCH_PRIORITY (sched.c:187), handed out by
       `adjust_priority' -> `birthing_insn_p' (sched.c), which promotes a
       newly-ready insn only when `reg_n_sets[dest] == 1'. 113's
       destination is a one-set compiler temp, so it is promoted; 131's
       destination is `q', which is ASSIGNED TWICE (once per loop), so
       reg_n_sets is 2 and it sits at priority 1. Priority 1 loses every
       contested slot: 131 is ready from sched1's T-6 and is finally placed
       at T-20, the first slot whose ready list holds nothing else. That
       puts it BEFORE 113 in sched1's output, which is the LUID order
       sched2 then loses the tie on.

     So reaching 0 needs either `q' to be a one-set pseudo in loop 1 (and
     see the measurement below -- two locals costs a callee-saved register
     and 85 differences), or an extra uncontested sched1 hole between T-7
     and T-17. Thirty-odd spellings below move neither.

   MEASURED AND REJECTED. All at 138 built words unless stated; the number
   is ovlverify's `differing'.

     The `q' assignment's POSITION in loop 1 -- six places, all 2:
     immediately after func_8004491C, before the third jitter, before
     `r = ', after `*(s16 *)(r + 0x6) = 8', before `n->unk_10 = ', and
     after `r = ' (the body below). Seventh place, between the 0x1E and
     0x1C stores: 140 words / 133.

     The third jitter's EXPRESSION -- all 2:
     `(rand() & 0x3F) + field - 0x20', `t = rand() & 0x3F' with an s32 or
     an s16 `t', `t = rand()' then `(t & 0x3F)', and the same two temps
     applied to all three jitters in loop 1 or in both loops.
     `field - 0x20 + (rand() & 0x3F)': 6.

     MEM_IN_STRUCT_P conversions (docs/matching-levers.md:34602), which is
     what landed func_s69_80024B00 two listings away -- all 2:
     the three jitter LOADS as `((u16 *)a->unk_08)[1]/[3]/[5]', the three
     jitter STORES as `((s16 *)n->unk_08)[1]/[3]/[5]', both, each of those
     in loop 1 only or in both loops, the five slot stores as
     `((s16 *)r)[0xF]' etc., and the two tail stores as `((s16 *)q)[0x1B]'
     and `[0x18]' singly or together. It cannot help here: every memref in
     the block has a REGISTER base, so neither side of `true_dependence'
     earns the `MEM_IN_STRUCT_P && rtx_addr_varies_p' escape
     (docs/matching-levers.md:20973).

     `do { } while (0)' wrappers: around the third jitter 2, around all
     three jitters 2, around the two tail stores 2, around `q' plus the two
     tail stores 2; around the `r' block 8, around the `r' block plus the
     tail 8.

     Declaration and type sweeps -- 2 each: `q' declared before `r',
     `col' as u32 with a u32 store, `a' as `u8 *' with
     `*(u8 **)(a + 0x8)', `q = (u8 *)(n + 1)', both loops as
     `i = 2; do { } while (--i > 0)', and `q' assigned after the whole `r'
     block. `b' as s32: 6. `i' as s16: 144 words / 127.

     Statement order: the `+0x36' store moved ahead of the `r' block 7;
     the two tail stores swapped 8; the whole `r' block ahead of the three
     jitters 25; `r' read inline at every use 148 words / 123; `r' read
     inline for the 0x1E store only 142 words / 134.

     `*(s16 *)((u8 *)n + 0x56) = b' in place of the `q + 0x36' store
     (so `q' has one use left): 3 -- the same transposition plus that one
     word. Both tail stores off `n' (`+0x56' and `+0x50', no `q' at all):
     137 words / 131, which is how we know the target's `addiu' is a source
     `q' and not a gcc artefact.

     TWO SEPARATE `q' LOCALS, one per loop -- the spelling the mechanism
     above asks for -- is 85 differing in ALL FIVE declaration orders tried
     (q then qb, qb then q, after `i', before `n', and with the loops
     swapped). The reason is visible in word 0: the frame comes out 0x30
     instead of the target's 0x38 and the body uses $s0..$s6 where the
     target uses $s0..$s7. Two non-overlapping live ranges pack into one
     register and free a callee-saved one, and every register name below
     $s7 shifts. The target's eight callee-saved quantities are n, a, b,
     col, i, the step address, D_80027484 and q -- so the target's `q' IS
     one shared two-set variable, and `birthing_insn_p' cannot be reached
     from the source.

   NOT TRIED, and what I would try next: the permuter, which is a sched2
   LUID tie and exactly what it is good at; and a zero-word
   `__asm__("" ::"r"(...))' marker, which per
   docs/matching-levers.md:14498 has sched1 priority
   `priority(pred) + cost - 1' and could occupy a sched1 slot between T-7
   and T-17 so that `q' lands below T-18. I did not spend an asm device on
   a 2-word scheduling residue.

   DECLARATIONS
   s32 func_8004491C(UnkNode *, u32);
   UnkNode *func_8003FD64(s32, void *);
   void func_s69_80024B00(u8 *, UnkVec *);
   s32 func_800C95C0(u8 *, UnkVec *, UnkSlot *);
   extern u8 D_80045340[];
   extern u8 D_80027484[];
   END DECLARATIONS
*/
void func_s69_80024C38(UnkNode *a, s16 b, s32 col)
{
    UnkNode *n;
    u8 *r;
    u8 *q;
    s32 i;

    for (i = 2; i > 0; i--)
    {
        n = func_8003FD64(0x212, a);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s69_80024B00;
            func_8004491C(n, (u32)D_80045340);
            *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x3F) - 0x20;
            *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x3F) - 0x20;
            *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x3F) - 0x20;
            r = (u8 *)n->unk_0C;
            q = (u8 *)n + 0x20;
            *(s16 *)(r + 0x1E) = 0x1000;
            *(s16 *)(r + 0x1C) = 0x1000;
            *(u8 **)(r + 0x8) = D_80027484;
            *(s32 *)(r + 0xC) = col;
            *(s16 *)(r + 0x6) = 8;
            *(s16 *)(q + 0x36) = b;
            *(s16 *)(q + 0x30) = (rand() & 0xFF) | 0x80;
        }
    }
    for (i = 2; i > 0; i--)
    {
        n = func_8003FD64(0x212, a);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s69_80024B00;
            func_8004491C(n, (u32)func_800C95C0);
            q = (u8 *)n + 0x20;
            *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x3F) - 0x20;
            *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x3F) - 0x20;
            *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x3F) - 0x20;
            *(s16 *)(q + 0x0) = 0x20;
            *(s16 *)(q + 0x2) = 0x20;
            *(s16 *)(q + 0x4) = 1;
            *(s16 *)(q + 0x6) = 0;
            *(s16 *)(q + 0x36) = b;
            *(s16 *)(q + 0x30) = (rand() & 0xFF) | 0x80;
        }
    }
}
