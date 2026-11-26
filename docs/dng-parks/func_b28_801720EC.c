/* PARK: func_b28_801720EC, csrc/ovl_dng_b28.c, agent-h132, 2026-11-24.

   TARGET 160 words. BEST BUILT 160 words, 24 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py); ovlverify reports the same 24. 1 member, 160
   words (tools/decomp/dngfam.py), so there is no replication value beyond its
   own body. This function owns NO jump table: its listing has no
   `.section .rodata' block and the three failure shapes are a compare chain.
   tools/decomp/relcand.py: ok, 5 data symbols and 10 calls all agreeing with
   the listing.

   THE WHOLE RESIDUE IS ONE MISSING LONG-LIVED PSEUDO, and I could not build
   it from C. The target allocates NINE callee-saved values and the ninth is a
   DEAD ZERO:

     80172144  bnez  $v0, .Lb28_801721AC
     80172148   addu $s7, $zero, $zero      <- set, saved, restored, NEVER READ

   $s7 is written exactly once with 0, is saved at 0x44($sp) and restored, and
   nothing in the 160 words reads it. The mechanism is almost certainly
   reload_cse_regs (reload1.c; it IS present in this compiler): a long-lived
   local holding 0 whose every use reload_cse replaced with $zero, leaving the
   set dead with no dead-code pass after it to remove it.

   WHY IT MATTERS MORE THAN ONE WORD. With that ninth value present the target
   has no register left for `t' (= d - 0x20), so `t' goes to the caller-saved
   $t0 and is read back from the frame; `y2' takes $s6 instead of $s7; and
   reorg has one spare insn, so the `lbu $v0, 0x71($s1)' LOAD-DELAY slot at
   word 15 is filled by the `c' parameter copy instead of a `nop'. Those three
   consequences are 22 of the 24 rows. The body below gets the word count, the
   frame, the save list, the save ORDER, every save/restore offset, every
   relocation and every call exactly right; the residue is entirely the
   register map downstream of the absent pseudo.

   EIGHT SHAPES TRIED FOR THE ZERO, ALL 160 words / 24 rows -- i.e. the
   pseudo was folded away in every one, so none of these is the answer:
     `s32 r = 0;' with the five `return 0' written `return r'
     the same plus `r = 1' before the final `return r' (two definitions)
     `s32 z = 0;' used as func_80047784's third argument
     `s32 z = 0;' used at `p[0x9B] = z' and `*(s32 *)(p + 0x8C) = z'
     the same, plus the five returns and the func_80047784 argument
     `u8 *w = NULL;' carried to `p[0x9B] = (s32)w' with func_8009B4B0's
       result assigned into `w' first
     a THIRD copy of each c byte (`x3 = x2; y3 = y2') to raise pressure
     `k = 0' before the gates and reassigned after them
   ROUTE (a) IS NOW RUN AND IT IS FLAT (dispatcher, 2026-11-26).  The idea was
   a value cse proves zero only through `record_jump_equiv', so that the
   pseudo holds a CALL RESULT rather than a constant and cse has nothing to
   fold.  The body has the ideal site for it: the `func_800A44E0' gate at
   word ~38 returns on non-zero, so on the FALL-THROUGH edge its result is
   provably 0, and the zero uses at `p[0x9B]' and `p + 0x8C' are downstream of
   it.  Measured:

       the result in `s32 r0', both zero uses written `r0'   160 w / 24 rows
       the same with only `p[0x9B] = r0'                      160     / 24
       the same plus the five `return 0' written `return r0'  160     / 26

   So cse folds the uses to `$zero' before allocation even when the value's
   definition is a call, the pseudo never reaches the allocator, and the
   register pressure that costs the 22 downstream rows is still absent.  That
   is NINE shapes now, and the source-level space for creating this pseudo
   looks genuinely closed.

   ROUTE (b) IS NOW RUN: 75 minutes, 532,033 candidates at 118/s,
   base 1405 -> best 740, roughly 250 saved improvements spread across the
   whole range.  IT DID NOT REACH 0 and 740 is not a landing.  The best
   candidate is kept at nonmatchings/func_b28_801720EC/output-740-1/ (the
   other 270 were superseded and deleted; they were 32M).

   The run's SHAPE is the informative part, and it is what the triage rule
   predicted.  A register-residue park on a different body went base 65 ->
   best 30 in its first TWO minutes and then found nothing in 88 more, with 2
   saved candidates out of 376,116.  This one improved steadily for the whole
   75 minutes with 250 saved.  So the permuter's moves CAN express what this
   park needs -- it simply did not arrive.  *** THE LONGER RUN IS DONE AND 740 IS THE FLOOR.
   DO NOT RUN IT AGAIN. An independent run at a 300-minute budget produced 178
   saved candidates across 64 distinct scores and NEVER WENT BELOW 740 -- it
   found a second candidate AT 740 and nothing under it. Two independent
   searches converging on the same number closes the permuter axis on this
   body: its move set cannot express what is missing, and a third run is waste.
   The two 740 candidates are kept at nonmatchings/func_b28_801720EC/.

   What that leaves is source structure, which is where the record below
   already points. Nine hand-written spellings and that whole record are
   already spent, so the next move is a different statement, not another
   search.

   Note the output is preprocessed C, so cleaning a 740 candidate up into
   source is real work and only worth doing from a 0.

   ROUTE (b), THE PERMUTER, IS STILL WORTH IT -- and that is not the default
   answer.  A separate finding on 2026-11-26 is that the permuter cannot crack
   a residue which is WHICH REGISTER an existing pseudo got, because a register
   follows the live ranges and the permuter only permutes spellings; a
   376,000-candidate run on such a park moved nothing.  **This park is the other
   kind.** Its residue needs a pseudo to EXIST that nine hand-written spellings
   do not create, and whether a pseudo exists IS a property of the source
   structure -- which is exactly what the permuter explores.  So the triage rule
   points the opposite way here, and a run is justified on a body whose word
   count, frame, save list, save order, every save offset, every relocation and
   every call already match.

   THE FRAME NEEDED 0x28 BYTES BELOW THE SAVES AND THE SPLIT IS A GUESS. The
   target is `addiu $sp, $sp, -0x50' with ten saves at 0x28..0x4C, so
   args_size + var_size = 0x28. The body below reaches that as a FIVE-ARGUMENT
   func_800C7930 (0x18 of outgoing args) plus a 16-byte local (`zz'), and that
   combination reproduces the frame, every save offset AND the
   `sw $t0, 0x10($sp)' at exactly word 42. It is not proven:
     * func_800C7930's real arity is FOUR -- csrc/ovl_dungeon_800C2824.c:3015
       defines `s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 d)' -- so the fifth
       argument below is a device, declared through a unit-local `__asm__'
       alias so the landed definition is untouched.
     * Written with FOUR arguments the body is 160 - 5 = 155 words and 87
       rows: the five missing words are `sw $a0, 0x50($sp)',
       `sw $a1, 0x54($sp)', the two `x2'/`y2' copies and `sw $t0, 0x10($sp)'.
       Adding a 24-byte local instead gets the frame right but NOT the
       `sw $t0', because a stack local adds no register pressure and `t' then
       stays in $s6.
     * So the honest reading of 0x10($sp) is either the fifth outgoing
       argument (args 0x18, local 0x10) or `t's SPILL SLOT with a 4-argument
       call (args 0x10, locals 0x18) -- and the second needs `t' to spill,
       which needs the missing ninth pseudo. FINDING THE ZERO MAY DELETE THE
       FIVE-ARGUMENT DEVICE AND THE `zz' LOCAL TOGETHER. Do not inherit
       either of them as facts.

   WHAT IT IS. The actor's "can I step onto this tile" test, the relinked and
   extended cousin of the resident func_800CA788
   (csrc/ovl_dungeon_800C2824.c:4893, 109 words, 0 differing -- the pair the
   brief already records as func_b03_80171DA0 <- func_800CA788). It clears
   d[0x71]'s 0x80, refuses on D_80083462's 0x2000, on D_80083462's 8 unless
   d + 0x46 has 0x8000, and on func_800A2B5C either side of
   func_800C7930 -- all four with -1. Then it asks func_800A44E0 whether the
   cell at the facing index (d + 0x2A bits 9..11) is passable, func_8009B4B0
   whether anything occupies it, func_800A5690 whether the move is allowed at
   all, and func_800BCB04 for the floor height there, refusing with 0 unless
   the height is under 0x201 and within 0x3F of d + 0x88. On success it puts
   the actor into state 0x17 with a null +0x8C, points the slot's +0x2C at
   this bank's D_80174558 facing table, re-dresses through func_80047784,
   runs func_800A4ACC and func_800C77D0 and drops d[0x6D].

   MEASURED ON THIS BODY. Every figure is aligndiff on the body below unless a
   word count is given. NONE of these is a price from a clean body -- the
   24-row fault is live in all of them, so re-price anything you keep.

   | spelling                                              | words | rows |
   | as written below                                       |  160  |  24  |
   | `s16 *t1' DECLARED BEFORE `s16 *t2'                    |  160  |  29  |
   |   (t1/t2 then hold each other's $s0/$s2)               |       |      |
   | x and y declared before their copies, in the other     |  160  |  32  |
   |   order (y, x, y2, x2)                                 |       |      |
   | ONE local per c byte -- no `x2'/`y2' at all            |  160  |  68  |
   |   (with the 5-arg call and `zz' still in place)        |       |      |
   | `s32 x2'/`s32 y2' instead of `u8'                      |  155  |  87  |
   |   -- a WIDE copy is propagated away entirely, which    |       |      |
   |   is the whole reason the copies are `u8': cse's       |       |      |
   |   make_regs_eqv never equates a subreg set, so only    |       |      |
   |   a NARROWING copy survives to the allocator           |       |      |
   | `s16 x2'/`s16 y2'                                      |  155  |  87  |
   | `u16 x2'/`u16 y2'                                      |  159  |  93  |
   | `u8 x'/`u8 y' with `s32 x2'/`s32 y2' (copy the other   |  158  |  77  |
   |   way round -- the copy comes out `andi 0xFF', not     |       |      |
   |   the target's `addu')                                 |       |      |
   | `zz' as 24 bytes instead of 16                         |  160  |  60  |
   | `zz' as 8 bytes                                        |  160  |  64  |
   | `d[0x6D] -= 1' moved BEFORE func_800C77D0              |  161  |  26  |
   |   -- it costs a WORD, so the decrement really is after |       |      |
   |   the call and reorg lifts its `sb' into the delay slot|       |      |
   | NOT MEASURABLE: `t' dropped and `d - 0x20' written     |  160  |  24  |
   |   inline at both calls, or inline at the second only   |       |      |
   |   -- cse keeps one pseudo either way                   |       |      |
   | NOT MEASURABLE: func_8009B4B0's addends swapped to     |  160  |  24  |
   |   `*t1 + x' -- the emitted `addu' operand order does   |       |      |
   |   not follow the source here, and the target's         |       |      |
   |   `addu $a1, $s3, $a1' is still 2 of the 24 rows       |       |      |

   ===================================================================
   UPDATE, agent-h147, 2026-11-25, batch 34.  STILL PARKED at 160 words /
   24 ALIGNED DIFFERENCES -- the body below is unchanged and is still the
   best on rows.  But the park's STATED MECHANISM IS WRONG, and the ninth
   pseudo is now BUILDABLE, which it was not before.  Read both before
   spending anything here.

   (1) THE reload_cse THEORY IS REFUTED.  reload1.c's `reload_cse_regs'
   (called once, reload1.c:2140) walks the insns with a flat
   `reg_values[FIRST_PSEUDO_REGISTER]' table and, at EVERY CODE_LABEL,

       -- its own comment there reads "Forget all the register values at a
       -- code label.  We don't try to do anything clever around jumps." --
       for (i = 0; i < FIRST_PSEUDO_REGISTER; i++) reg_values[i] = 0;

   so it cannot carry `$s7 == 0' past the first label.  The dead set sits in
   the FIRST basic block (word 23, in the first gate's branch delay slot)
   and anything far enough away to need a callee-saved register is past at
   least the `&&' gate's drop-through label.  So reload_cse CANNOT have
   rewritten this pseudo's uses to $zero.  Whatever removed them ran
   earlier.  Also measured, same file: reload_cse DELETES no-op sets
   (`reload_cse_noop_set_p'), so it never leaves a dead word behind either.

   (2) A ZERO PSEUDO CAN BE MADE TO SURVIVE cse.  All eight of the park's
   shapes fold because they spell the zero as a LITERAL, and cse propagates
   a literal into every use -- verified in the RTL dumps: with
   `s32 rz = 0;' and the five returns written `return rz', the .jump dump
   has `(insn 17 (set (reg/v:SI 93) (const_int 0)))' with five
   `(set (reg/i:SI 2 v0) (reg/v:SI 93))' uses, and by the .cse dump the set
   is gone and all five are `(const_int 0)'.  A second definition of rz does
   not stop it either.  The shape that DOES survive is a zero cse cannot
   prove and combine can:

       rz = c[0x24] >> 8;

   cse's `simplify_binary_operation' will not fold a shift whose operand is
   not constant, so the insn survives cse, loop, cse2 AND FLOW -- which is
   the point, because flow is what counts `reg_n_refs' and
   `reg_live_length'.  combine then folds it with `nonzero_bits' (the
   zero_extend of a QImode load has only 8 significant bits) to
   `(set rz (const_int 0))' and deletes the load, so it costs exactly ONE
   word.  Equivalent and also measured: `*(u16 *)(d + 0x2A) >> 16' and
   `d[0x71] >> 8'.  This is the first shape in nine that produces a ninth
   long-lived pseudo at all, and it does land the dead
   `addu $sX, $zero, $zero' AT WORD 23, where the ROM has it.

   MEASURED WITH THAT ZERO.  Every row is aligndiff against the 160-word
   target.  `zz' is the park's 16-byte local and `5-arg' is the park's
   func_800C7930 device.

   | shape                                               | words | rows |
   | 5-arg + zz[4] + rz at the five returns              |  163  |  55  |
   |   -- too much pressure: `c' spills as well          |       |      |
   | 5-arg + zz[0..3] + rz at the five returns           |  163  | 91+  |
   | 4-ARG + zz[4] + rz at the five returns              |  160  |  30  |
   | 4-arg + zz[3] + rz at the five returns              |  160  |  30  |
   | 4-arg + zz[0]/zz[2]/zz[5]/zz[6], five returns       |  160  | 55+  |
   | 4-arg + zz[4] + rz used ONCE, at `*(p+0x8C) = rz'   |  159  |  31  |
   | 4-arg + zz[4] + rz used TWICE, at p+0x9B and p+0x8C |  160  |  39  |
   | 5-arg + zz[4] + rz used twice                       |  162  |  70  |

   THE 160 / 30 ROW IS THE ONE TO READ.  It reproduces, with NO
   func_800C7930 device: the dead zero at word 23; `t' in $t0 spilled and
   reloaded; and the exact word count.  Its whole residue is
       the dead set is `move s3,zero' where the ROM has `$s7'
       `sw t0,0x20(sp)' where the ROM has `sw t0,0x10(sp)'
       five `move v0,s3' where the ROM has `addu $v0,$zero,$zero'
       x/y/x2/y2 rotated one register up, and the word 15 load-delay nop
   so the park's three named consequences are all PRESENT; what is wrong is
   only WHICH register the zero takes and that its reads survive.

   THE TWO CONSTRAINTS THIS PINS DOWN, and they are in tension.
   * REFERENCE COUNT.  global.c ranks on floor_log2(n_refs)*n_refs /
     live_length.  With 6 references (set + five returns) rz outranks `x'
     and takes $s3.  With 2 or 3 it is outranked by everything and SPILLS
     (`sw zero,0x24(sp)', and the uses reload from the stack).  For the ROM's
     $s7 -- allocated LAST of the five -- rz needs about 3 references, which
     is exactly where it spills in this body.  So the ROM's ninth value has
     FEW references and still gets a register, which this body cannot yet
     reproduce.
   * A 2-REFERENCE ZERO CAN NEVER LEAVE A DEAD WORD.  local-alloc.c:1080's
     `update_equiv_regs' replaces the single use of a register referenced
     exactly twice that has a constant REG_EQUIV, and then does
     `PUT_CODE (equiv_insn, NOTE)' -- it DELETES the initialising insn.
     That is the 159-word row above, one word short, and it rules the
     2-reference shape out by construction.

   AND THE READS ARE THE REAL WALL.  The ROM reads $s7 nowhere, so every
   read must be gone by the assembler, yet flow must have seen one to give
   the pseudo a long live range.  Nothing between flow and the assembler can
   delete a CROSS-BLOCK read and keep the set: combine's LOG_LINKS are
   intra-block, so it cannot reach a use in another block; update_equiv_regs
   deletes the set when it replaces the use; reload_cse forgets everything
   at each label (above).  So EITHER the ninth value's read is in the SAME
   basic block as its set -- in which case it is not long-lived and cannot
   be the thing that spills `t' -- OR the ninth value is not a zero local at
   all.  I now think the second is more likely than the park does.

   WHAT I WOULD TRY NEXT, in this order.
   1. KEEP `rz = c[0x24] >> 8' AND THE 4-ARGUMENT CALL -- drop the 5-arg
      device, which this update shows is not needed once the zero exists --
      and attack only the reference count: find a use of rz that global.c
      prices at about 3 references but that reload does not spill.  The
      frame local is the obvious carrier (`zz[0] = rz') and I did not
      measure it.
   2. RE-READ THE NINTH VALUE.  `sw $t0, 0x10($sp)' is reloaded at
      `lw $a0, 0x10($sp)' before func_800C77D0, so 0x10($sp) is a SPILL
      SLOT and not an outgoing argument, which means args_size is 0x10, the
      call really is 4-argument, and var_size is 0x18 with the spill at the
      FIRST local offset.  Declared locals are assigned before reload's
      spill slots, so a declared local cannot sit below 0x10 -- which says
      there are NO declared locals in this function and 0x14 more bytes of
      SPILL.  That is a second spilled pseudo, not a 16-byte array.  Chase
      that: the ninth value may be a pseudo that spills, and the dead
      `addu $s7, $zero, $zero' may belong to a different value entirely.
   3. A permuter run, which is now cheaper than it was: the 4-arg + zz[4] +
      five-returns body is 160 words with the frame, the spill and the dead
      word all in place, so the search is over the zero's USES only.

   DECLARATIONS
   s32 func_800A44E0(u16, u16, s16, u32);
   u8 *func_8009B4B0(u8 *, u16, u16);
   s32 func_800A5690(void);
   s32 func_800C77D0(void *, u8 *, s16, s32);
   s32 func_800C7930_5(u8 *, u8 *, s16, s32, u8 *) __asm__("func_800C7930");
   END DECLARATIONS

   Everything else it names (func_800A2B5C, func_800BCB04, func_80047784,
   func_800A4ACC, D_80083462, D_80083228, D_80174558, D_8006CCD8, D_8006CCE8)
   is already declared in ovl_dng_b28.c or comes from common.h.
*/
s32 func_b28_801720EC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;
    s16 *t2;
    s16 *t1;
    s32 x;
    s32 y;
    u8 x2;
    u8 y2;
    s32 k;
    s32 h;
    s32 zz[4];

    d[0x71] &= 0x7F;
    if ((D_80083462 & 0x2000) != 0)
    {
        goto fail;
    }
    if ((*(u16 *)(d + 0x46) & 0x8000) == 0 && (D_80083462 & 8) != 0)
    {
        goto fail;
    }
    if ((s16)func_800A2B5C(d) != 0)
    {
        goto fail;
    }
    t = d - 0x20;
    func_800C7930_5(t, (u8 *)b, 8, 0x300, t);
    if ((s16)func_800A2B5C(d) != 0)
    {
    fail:
        return -1;
    }
    x = c[0x24];
    y = c[0x25];
    x2 = x;
    y2 = y;
    k = (*(u16 *)(d + 0x2A) >> 9) & 7;
    if ((s16)func_800A44E0(x2 << 6, y2 << 6, *(s16 *)(d + 0x88), k << 9) != 0)
    {
        return 0;
    }
    t1 = &D_8006CCD8[k];
    t2 = &D_8006CCE8[k];
    if (func_8009B4B0(d, x + *t1, y + *t2) != NULL)
    {
        return 0;
    }
    if ((s16)func_800A5690() == 0)
    {
        return 0;
    }
    h = func_800BCB04((((x2 + *t1) << 6) + 0x20) & 0xFFE0, (((y2 + *t2) << 6) + 0x20) & 0xFFE0,
                      *(u16 *)(d + 0x88) - 0x20);
    if ((s16)h >= 0x201)
    {
        return 0;
    }
    if ((s16)(h - *(u16 *)(d + 0x88)) < -0x3F)
    {
        return 0;
    }
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    p[0x9A] = 0x17;
    *(u8 **)(c + 0x2C) = D_80174558;
    func_80047784(c, D_80174558[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    func_800A4ACC(d);
    func_800C77D0(t, (u8 *)b, 8, 0x300);
    d[0x6D] -= 1;
    return 1;
}
