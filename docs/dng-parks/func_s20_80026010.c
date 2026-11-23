/* PARK: func_s20_80026010, csrc/ovl_dng_s20.c, agent-h13, 2026-11-23.

   TARGET 170 words. BEST BUILT 170 words, 23 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py; ovlverify also reports 23). 2 members, 170
   words each, 340 words total (tools/decomp/dngfam.py: func_s20_80026010,
   func_s21_80025BA0).

   WHAT IT DOES. The spawner for the trail ring: for every live entry of
   D_80026478 take a 0x202 node, park func_s20_80025A58 as its step and
   func_s20_80025E4C as its draw, seed the node's mover at the average of four
   ring entries (this one, the next one in its 0x10-wide ring, and the two
   matching entries of the second bank at &D_80026478[0x10]), jitter its three
   velocities by a fresh func_80069EF8() & 0xF less 8 scaled into the high
   halfword, jitter the slot's +0x16/+0x18 by & 0x1FF less 0x100, set the
   payload's +0x1A life to 0x20, copy the four ring entries into the payload's
   four corner records at +0x30..+0x4F, pick the colour pair by
   `i % 0x20 < 0x10', and finally clear D_s20_8002632A.

   NO TWIN: the only sibling is func_s21_80025BA0, the identical body; the
   best non-family neighbour in an opcode-sequence sweep of asm/nonmatchings
   is 0.443 (func_s20_80024EF4, which I landed in this unit and which reads
   the same ring).

   THE RESIDUE IS 23 ROWS AND EVERY ONE OF THEM IS `c' AND `m'/`e' EXCHANGING
   $s0 AND $s1. The target has `m' (the mover, n->unk_08) and `e' (the slot,
   n->unk_0C) in $s0 and the fourth ring pointer `c' in $s1; this body has
   them the other way round. Word count, instruction selection, instruction
   order and every other register agree exactly.

   I READ THE SORT OUT OF `-da' (the recipe in matching-levers.md). On the
   standalone probe `;; 7 regs to allocate:' lists only 87 216 88 80 244 108
   199, so `c', `m' and `e' are all LOCAL quantities, not global allocnos, and
   the formula that decides them is local-alloc.c:1693's

       floor_log2 (qty_n_refs) * qty_n_refs * qty_size / (qty_death - qty_birth)

   `bb.c.lreg' gives, with references already weighted x2 by flow.c's
   loop_depth:

       81 = m    14 refs / 36 insns   crosses 3 calls   -> $s1 here
       85 = c    10 refs / 72 insns   crosses 5 calls   -> $s0 here
       86 = e     6 refs /  9 insns   crosses 2 calls   -> $s1 here
       83 = a    12 refs / 70 insns                     -> $s4 (correct)
       84 = b    10 refs / 73 insns                     -> $s2 (correct)

   and `;; Register dispositions' puts 85 102 103 116 119 120 all in reg 16,
   which is the tell: `combine_regs' has tied the whole address chain for `c'
   -- the `addiu $zero, -0x10' constant, `i & -0x10', `((i + 1) & 0xF) + 0x10',
   the sum, the `sll 3' and the final `addu $fp' -- into ONE quantity, because
   each link's source dies at the next. That quantity therefore carries about
   32 references over the chain's whole span of about 94 insns, for a priority
   near floor_log2(32) * 32 / 94 = 1.70, against `m's 3 * 14 / 36 = 1.167 and
   `e's 2 * 6 / 9 = 1.333. `c' sorts FIRST and takes $s0.

   SO THE REQUIREMENT IS ARITHMETIC: either `m' reaches 16 references at the
   same live length -- 4 * 16 / 36 = 1.78, which clears `c' -- or the `c'
   chain drops to 27 references or fewer (4 * 27 / 94 = 1.149, below `m').
   ONE extra real reference on `m' is enough, because flow weights it twice.

   AND NEITHER FREE-REFERENCE DEVICE IS FREE IN THIS BODY, which is the
   measured result worth having:

     `do { <stmt>; } while (0)' around each of the six statements that
         mention `m', in order +0x2, +0x6, +0xA, +0xC, +0x10, +0x14:
         175 words / 63, 55, 53, 42, 42, 42 differing.
     `__asm__("" ::"r"(m));' after four different statements:
         174 words / 72 differing at every one.

   Both are inside the loop, and both cost words here rather than nothing --
   the do-while's loop NOTEs and the asm's implicit volatile are full
   scheduling barriers, and this loop body is scheduled tightly enough that a
   barrier anywhere in `m's range moves the giv at $s7 as well.

   MEASURED AND REJECTED, all 170 words and all 23 differing unless noted:

     `m = n->unk_08' at each of four positions among the a/b/c/w
         assignments                                             23 each
     `m' as a `u8 *' with `*(s32 *)(m + 0xC)' casts                    23
     `a', `b', `c', `w' as `u8 *' with casts                           23
     `c' assigned before `b' /  before `a'                         27 / 23
     `w' as the plain array ref `&D_80026478[i]'       23 (KEPT: cleanest)
     two separate mover locals, one per store group        172 words, 61
     `e' reusing `m's local (a second set, so global alloc)            40
     `x', `y', `z' as `s16' instead of `s32'               173 words, 58
     `w' as a pre-loop pointer local with `w++' at the loop end
                                                           176 words, 51
     `a', `b', `c' as plain array refs `&D_80026478[...]'   167 words, 68

   TWO SPELLINGS ARE LOAD-BEARING AND ARE ALREADY IN THE BODY:

     THE THREE SUMS GO INTO LOCALS and are stored afterwards. Written inline
       in the three store statements it is 172 words and 59 differing: the
       target loads all twelve ring halfwords BEFORE its first store to the
       mover, and with the stores interleaved the loads cannot cross them
       (`*(s16 *)((u8 *)m + 0x2)' is a cast, so MEM_IN_STRUCT_P is clear and
       true_dependence's escape does not apply).
     THE RING INDEX IS `(u32)D_80026478' WITH AN EXPLICIT `<< 3'. The plain
       array ref `&D_80026478[i + 0x10]' folds (i + 0x10) * 8 into
       i * 8 + 0x80 and loses the target's three `addiu ... 0x10' -- 167
       words. The integer form also keeps the `addu $s4, $s3, $fp' operand
       order, index first, which `pointer_int_sum' will not give.
       (func_s20_80024EF4 in the same unit needs the OPPOSITE spelling: there
       the plain array ref off the loop-invariant symbol is what matches. The
       difference is whether the +0x10 belongs in the base.)

   THE SAME ARITHMETIC ON `e' INSTEAD, AND WHY IT ALSO FAILS. `e' only needs
   8 references at a live length of 13 or less (3 * 8 / 13 = 1.85) to clear
   the `c' chain, and it starts at 6 / 9 -- the smallest window in the
   function. Every device that adds the reference also adds the barrier that
   destroys the length:

     `__asm__("" ::"r"(e));' after the def, after the +0x16 store and after
         the +0x18 store                    169 words / 27 differing, each
     `do { } while (0)' around the def, the +0x16 store, the +0x18 store
                                            175 words / 42, 42, 44
     the same around BOTH stores            175 words / 27
     nested twice around the +0x18 store    175 words / 27

   The `-da' probe says exactly why: with the asm in place pseudo 86 reads
   `8 times across 29 insns' instead of `6 times across 9 insns', so its
   priority FALLS from 2 * 6 / 9 = 1.333 to 3 * 8 / 29 = 0.828. The barrier
   stops sched1 packing the slot's load and its two stores together, and the
   range it spreads costs more than the reference buys. (The 169-word rows
   are one word SHORT, not matching: the asm eats the `nop' in the +0x16
   store's load-delay slot.)

   AND THE DUPLICATED-TAIL ROUTE, measured because it is the recipe that
   settled func_b03_80170E54, func_b04_80173ED8 and func_b14_80170894: this
   function's only branch pair inside `c's live range is the
   `i % 0x20 < 0x10' if/else, and the only `c' reference available to
   duplicate into it is the `+0x4C' corner store:

     the store moved to the HEAD of both arms      175 words / 80
     the store moved to the TAIL of both arms      170 words / 72
     the store kept in place AND added to both     178 words / 77

   The tail form does confirm the coordinator's zero-word claim -- jump2
   cross-jumps the two copies back into one and the body is still 170 words.
   It is no use here because cross-jumping merges a tail AFTER the join, and
   the target stores +0x4C in the `bgez $s5' DELAY SLOT, ahead of the branch.

   WHAT I WOULD TRY NEXT. Break the `combine_regs' tie chain on `c' rather
   than pay for a reference on `m': the chain is tied only because each link's
   source dies at the next, so one extra live use of `i & -0x10' or of the
   scaled index -- or getting the `c' pseudo into a second basic block, which
   local-alloc.c:472 turns into a global allocno and takes out of this sort
   entirely -- drops the quantity to the few references of its tail. The
   coordinator's duplicated-tail recipe is the shape to reach for; this
   function has no `goto' tail to duplicate, but its `i % 0x20 < 0x10'
   if/else is a candidate site for moving the `+0x4C' corner store into both
   arms, which would put `c' in two blocks. I did not measure that because
   the target stores +0x4C in the `bgez's delay slot, ahead of the branch,
   and jump2's cross-jumping merges tails after the join, not before it.

   DECLARATIONS
   typedef struct UnkDng26478
   {
       s16 unk_00;
       s16 unk_02;
       s16 unk_04;
       u8 pad_06[0x2];
   } UnkDng26478;
   extern UnkDng26478 D_80026478[];
   extern UnkDng26478 D_800264F8[];
   extern u32 D_80026470;
   extern u32 D_80026474;
   extern s16 D_s20_8002632A;
   void func_s20_80025A58(u8 *, UnkVec *, UnkSlot *);
   s32 func_s20_80025E4C();
   UnkNode *func_8003FC64(s32);
   s32 func_80069EF8();
   END DECLARATIONS

   UnkDng26478, D_80026478, D_80026470, D_80026474 and D_s20_8002632A are
   already in csrc/ovl_dng_s20.c's block for the two landed functions; only
   D_800264F8, the two func_s20_ forward declarations, func_8003FC64 and
   func_80069EF8 are new. func_8004491C is already `extern s32' in
   include/structs.h -- do NOT redeclare it, it conflicts. */
void func_s20_80026010(void)
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

    for (i = 0; i < D_s20_8002632A; i++)
    {
        n = func_8003FC64(0x202);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s20_80025A58;
            func_8004491C(n, (u32)func_s20_80025E4C);
            a = (UnkDng26478 *)((((i & -0x10) + ((i + 1) & 0xF)) << 3) + (u32)D_80026478);
            b = (UnkDng26478 *)(((i + 0x10) << 3) + (u32)D_80026478);
            c = (UnkDng26478 *)((((i & -0x10) + (((i + 1) & 0xF) + 0x10)) << 3) +
                                (u32)D_80026478);
            w = &D_80026478[i];
            m = n->unk_08;
            x = w->unk_00 + a->unk_00 + b->unk_00 + c->unk_00;
            y = w->unk_02 + a->unk_02 + b->unk_02 + c->unk_02;
            z = w->unk_04 + a->unk_04 + b->unk_04 + c->unk_04;
            *(s16 *)((u8 *)m + 0x2) = x >> 2;
            *(s16 *)((u8 *)m + 0x6) = y >> 2;
            *(s16 *)((u8 *)m + 0xA) = z >> 2;
            m->unk_0C = ((func_80069EF8() & 0xF) - 0x8) << 16;
            m->unk_10 = ((func_80069EF8() & 0xF) - 0x8) << 16;
            m->unk_14 = ((func_80069EF8() & 0xF) - 0x8) << 16;
            e = (u8 *)n->unk_0C;
            *(s16 *)(e + 0x16) = (func_80069EF8() & 0x1FF) - 0x100;
            *(s16 *)(e + 0x18) = (func_80069EF8() & 0x1FF) - 0x100;
            q = (u8 *)n + 0x20;
            *(s16 *)(q + 0x1A) = 0x20;
            *(u32 *)(q + 0x30) = *(u32 *)w;
            *(s16 *)(q + 0x34) = w->unk_04;
            *(u32 *)(q + 0x38) = *(u32 *)a;
            *(s16 *)(q + 0x3C) = a->unk_04;
            *(u32 *)(q + 0x40) = *(u32 *)((i << 3) + (u32)D_800264F8);
            *(s16 *)(q + 0x44) = b->unk_04;
            *(u32 *)(q + 0x48) =
                *(u32 *)((((i & -0x10) + ((i + 1) & 0xF)) << 3) + (u32)D_800264F8);
            *(s16 *)(q + 0x4C) = c->unk_04;
            if (i % 0x20 < 0x10)
            {
                *(u32 *)(q + 0x50) = D_80026470;
                *(u32 *)(q + 0x54) = D_80026474;
            }
            else
            {
                *(u32 *)(q + 0x54) = D_80026470;
                *(u32 *)(q + 0x50) = D_80026474;
            }
        }
    }
    D_s20_8002632A = 0;
}
