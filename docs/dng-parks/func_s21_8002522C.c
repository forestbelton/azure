/* func_s21_8002522C -- PARKED at 115/115 words, 1 differing.  2 copies in its
   family (dngfam.py: func_s20_8002569C in ovl_dng_s20, still an INCLUDE_ASM,
   so there was no landed sibling).

   WHAT IT IS: the 0x202 spawner for func_s21_80024CD4's step and
   func_s21_80024ED0's renderer.  It seeds the new mover's position from the
   caller's own position offset by rcos/rsin of `ry' scaled by `rx' (>> 15)
   and by `rz >> 3' in z, clamps the z component at D_800269C8, copies the
   unoffset position into the mover's +0xE/+0x12/+0x16 interpolation target,
   writes 0x40E020 into the renderable's colour word, records the three
   angles and the current ring cursor in the payload, and -- while the cursor
   is under 0x71 -- writes the interpolation target into BOTH trail-ring banks
   at D_800265C8[cursor] and D_800265C8[cursor + 0x10] and bumps the cursor.

   THE ONE DIFFERING WORD IS word 82, the `sh' in the `beqz' delay slot:

       candidate  A4660022   sh $a2, 0x22($v1)
       target     A4670022   sh $a3, 0x22($v1)

   $a3 is the raw `lhu' of D_80026476 and $a2 is its sign extension, and the
   two are the same 16 bits, so this is a register-choice difference and not a
   value one.  It is a CATCH-22 between the schedule and the allocation, and
   both halves are measured:

   * Written `*(s16 *)(q + 0x22) = c;' -- the RAW value, which is what the
     target stores -- the store has no dependence on the compare chain, sched1
     leaves it in source position (before the `sll'), `q' therefore dies
     before the `sll' temp is born, local-alloc gives BOTH $v0, and reorg can
     no longer sink the store into the delay slot because `sll $v0' clobbers
     its base.  That costs the four `q' stores' base register and the delay
     slot: 115 words, 9 differing.
   * Written `*(s16 *)(q + 0x22) = e;' with `e = (s16)c;' hoisted above it,
     the store depends on the `sra', sched1 defers it past the compare, `q'
     overlaps the `sll' temp, local-alloc gives the temp $v0 and `q' $v1, and
     reorg sinks the store into the delay slot.  Everything matches -- except
     that the stored register is now $a2.

   So the residue is one word either way and the missing spelling is one that
   makes the store ORDER after the sign extension while still storing the raw
   value.  `(u16)e' and `e' with a second `(s16)c' for the ring both measure 1
   (combine does not fold the subreg back through a pseudo), and nothing tried
   reaches 0.

   EVERYTHING ELSE MEASURED AND REJECTED:

     *(s16 *)(q + 0x22) = c, compare on (s16)c                     9
     ... compare on *(s16 *)(q + 0x22) (read back)                 9
     ... (s16)c hoisted into e above the store, compare on e       9
     ... c declared u16 / s16 / s16 from *(u16 *)&D_80026476       9 each
     ... the 0x22 store first, second or third among the four      9 each
     ... q as s16 * with q[0x11]                                   9
     ... q and c assigned before the three m stores     113-115,  22-55
     ... the 0x40E020 store moved after the q stores     114-115,  13-49
     *(s16 *)((u8 *)n + 0x42) = c (no q for that store)            5
     e and f both hoisted above the store                         29
     the 0x22 store duplicated into both arms           117 words, 22

   FOUR SPELLING FACTS THAT ARE MEASURED AND SHOULD BE KEPT:

   * THE TWO RING BANKS ARE CHAINED ASSIGNMENTS.
     `D_800265C8[f].unk_00 = D_800265C8[e].unk_00 = *(u16 *)(m + 0xE);'
     loads `m + 0xE' ONCE and stores the inner target first, which is the
     target's order.  Written as two separate statements the `sh' to the first
     bank invalidates cse's memory table and each of the three values is
     RELOADED for the second bank: 121 words, 44 aligned differences.
   * `f = e + 0x10' MUST BE AN INDEX LOCAL.  `D_800265C8[e + 0x10]' folds the
     0x80 into the base symbol and deletes the target's `addiu/sll/addu'.
   * `c = (u16)D_80026476;' -- the cast is what turns the unit's `extern s16
     D_80026476' into the target's `lhu', and the `%lo' stays folded into the
     load.  Without it the load is `lh' and the `sll/sra' pair disappears.
   * THE CLAMP READS D_800269C8 TWICE, `lh' for the compare and `lhu' for the
     store, so it is `u16' in the declarations and the compare carries an
     explicit `(s16)'.  One `s16' declaration would let cse share one load.

   func_s21_80024CD4 and func_s21_80024ED0 need NO declaration: both are
   defined above this function in ovl_dng_s21.c (func_s21_80024CD4 landed in
   this batch).  func_s21_80024CD4 CALLS this function, with exactly the
   signature above -- five arguments, the last on the stack -- so the two must
   land together or the call site has to be re-measured.

   raw 13 == the 13 relocations: R_MIPS_26 on func_8003FD64, func_8004491C,
   func_80064584 and func_800644B8, and HI16/LO16 pairs on
   func_s21_80024CD4, func_s21_80024ED0, D_800269C8 (one HI, two LO),
   D_80026476 (one HI, two LO) and D_800265C8.

DECLARATIONS
typedef struct UnkDng26478
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x2];
} UnkDng26478;
extern UnkDng26478 D_800265C8[];
extern s16 D_80026476;
extern u16 D_800269C8;
UnkNode *func_8003FD64(s32, void *);
END DECLARATIONS */
UnkNode *func_s21_8002522C(u8 *p, s16 ry, s16 rx, s16 rz, void *tmpl)
{
    UnkNode *n;
    u8 *m;
    u8 *r;
    u8 *q;
    u32 c;
    s32 e;
    s32 f;

    n = func_8003FD64(0x202, tmpl);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s21_80024CD4;
        func_8004491C(n, (u32)func_s21_80024ED0);
        m = (u8 *)n->unk_08;
        *(s16 *)(m + 0x2) = *(u16 *)(p + 0x2) + ((func_80064584(ry) * rx) >> 15);
        *(s16 *)(m + 0x6) = *(u16 *)(p + 0x6) + ((func_800644B8(ry) * rx) >> 15);
        *(s16 *)(m + 0xA) = *(u16 *)(p + 0xA) + (rz >> 3);
        if ((s16)D_800269C8 < *(s16 *)(m + 0xA))
        {
            *(s16 *)(m + 0xA) = D_800269C8;
        }
        *(s16 *)(m + 0xE) = *(u16 *)(p + 0x2);
        *(s16 *)(m + 0x12) = *(u16 *)(p + 0x6);
        *(s16 *)(m + 0x16) = *(u16 *)(p + 0xA);
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0xC) = 0x40E020;
        q = (u8 *)n + 0x20;
        c = (u16)D_80026476;
        *(s16 *)(q + 0x16) = ry;
        *(s16 *)(q + 0x1C) = rz;
        *(s16 *)(q + 0x18) = rx;
        e = (s16)c;
        *(s16 *)(q + 0x22) = e;
        if (e < 0x71)
        {
            f = e + 0x10;
            D_800265C8[f].unk_00 = D_800265C8[e].unk_00 = *(u16 *)(m + 0xE);
            D_800265C8[f].unk_02 = D_800265C8[e].unk_02 = *(u16 *)(m + 0x12);
            D_800265C8[f].unk_04 = D_800265C8[e].unk_04 = *(u16 *)(m + 0x16);
            D_80026476 = c + 1;
        }
    }
    return n;
}

   DISPATCHER NOTE, 2026-11-23: WHAT GUARDS THIS SIGNATURE. The hand-back
   warns that the landed func_s21_80024CD4 calls this function with FIVE
   arguments (the fifth on the stack at 0x10($sp)), that this park's body
   uses that signature, and that "neither the build nor ovlverify will catch
   a mismatch" if it is ever landed with a different arity.

   Half right, and the half that matters is covered. A bare `ovlverify' on
   ONE function would indeed not catch it -- but nothing lands that way.
   tools/decomp/dngland.py re-verifies EVERY other defined function in the
   unit in the same compile and REFUSES to write on any regression; the
   integration of this very batch printed "8 other function(s) in the unit
   re-verified, 0 now failing". A changed arity here alters func_s21_80024CD4's
   argument setup, so it would come back as a regression and the landing would
   be refused.

   So the real risk is narrower: an agent working in a PRIVATE COPY can
   change the arity, measure only its own function, and hand back something
   that will be rejected at integration. Keep the five-argument signature, and
   if a measurement genuinely demands otherwise, say so in the hand-back so
   func_s21_80024CD4 is re-measured deliberately rather than by refusal.

