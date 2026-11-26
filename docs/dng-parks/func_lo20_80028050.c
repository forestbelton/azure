/* func_lo20_80028050 -- NOT PARKED FOR A RESIDUE.  THE BODY IS COMPLETE AND
   AT 0 DIFFERING; what blocks it is this unit's BROKEN SYMBOL CARVE.

   RE-CONFIRMED 2026-11-26 by agent-h232, independently of this header:
   43 words / 43 words / 0 DIFFERING.  Three checks, all first-hand:
     - asm/nonmatchings/ovl_dng_lo20/func_lo20_80028050.s is 41 words and
       contains NO `jr $ra' at all (grep -c "jr .*ra" is 0).  Its last three
       words are `sh $v0, 0x6($v1)', `lw $v1, 0x4($a1)', `addiu $v0, $zero, 0x4'.
     - func_lo20_800280F4.s is exactly two words, `jr $ra' with
       `sb $v0, 0xF($v1)' in the delay slot, and both $v0 and $v1 are the
       registers the predecessor's last two instructions set.
     - with the body spliced and the tail's INCLUDE_ASM removed, ALL 70 other
       defined bodies in ovl_dng_lo20 still measure 0 differing.

   TWO CORRECTIONS TO THIS HEADER, both measured.

   (1) `pairverify.py' IS NOW A PROJECT TOOL, at tools/decomp/pairverify.py.
   The paragraph below points at agent-h171's scratch copy; use the committed
   one.  It refuses to run while the tail is still assembly, printing
   `func_lo20_800280F4 is still an INCLUDE_ASM -- remove it first', so the
   order is: parksplice the body, THEN delete the tail's INCLUDE_ASM line from
   the working copy, THEN

       uv run python tools/decomp/pairverify.py <copy>/ovl_dng_lo20.c \
           func_lo20_80028050 func_lo20_800280F4

   which prints `target 43 words, candidate 43 words, 0 differing (masked)'.

   (2) func_lo20_800280F4 IS NOT DECLARED OR CALLED ANYWHERE.  This header
   said it `is already DECLARED and CALLED by the landed func_lo20_8002823C';
   it is not.  Its only occurrence in csrc/ovl_dng_lo20.c is its own
   INCLUDE_ASM at line 1365, and no listing under
   asm/nonmatchings/ovl_dng_lo20/ names it except its own.  So deleting that
   stub leaves no undefined symbol and needs no compensating declaration --
   which makes the landing a two-line edit rather than a three-way one.

   A NOTE FOR WHOEVER WRITES THE NEXT BRIEF FOR THIS PARK.  A brief handed to
   agent-h232 described this as "a SMALL body that is two words OVER, so the
   word count is itself the signal", and offered the two usual causes (an
   extra callee-saved register, or a re-materialised `lui').  NEITHER APPLIES:
   the 43-against-41 is the carve's truncated target, not an over-long body,
   and no spelling change can or should move it.  The figure to quote for this
   park is 43/43/0, not 43/41.

   THE THIRD TEMPLATE-COPY CONSTRUCTOR, and the carve cut off its last two
   words.

   func_lo20_80028050 has NO `jr $ra'; func_lo20_800280F4 is TWO words,
   `jr $ra' with `sb $v0, 0xF($v1)' in the delay slot, and both $v0 and $v1
   are set in func_lo20_80028050's listing.  The real function is
   0x80028050..0x800280F8, 43 words; `func_lo20_800280F4' is its TAIL.

   A 2-WORD BODY AT 0 DIFFERING WOULD HAVE PROVED NOTHING HERE, which is
   exactly the trap the dispatch brief names: these two words are a real
   function's epilogue and are indistinguishable at that size from the
   smallest real function.  What settles it is that the PREDECESSOR has no
   `jr $ra' at all and that the two words read registers it sets.

   First compile off func_lo20_80025D9C, which lands in this batch: the same
   body with -0x58/-0x40 for 0x2C/-0x8 and two stores through b + 0x8 in
   place of the single `sh $zero, 0x6'.  Both of that body's facts are
   needed here too -- ARRAY-REF destinations `((u32 *)c)[1]' and element 0
   read through the pointer local `s1[0]' -- each worth 2 words / 32 aligned
   differences there.

   ovlverify reads `func_lo20_80028050: target 41 words, candidate 43 words,
   2 differing' and that is the CARVE's figure, not the body's: it
   compares the head's TRUNCATED listing against the whole body.
   Concatenated, the measurement is 43 words / 43 words / 0 differing.

   parkcheck --splice will FAIL on this file and that is expected: splicing
   the body in replaces the HEAD's INCLUDE_ASM and leaves the tail's in
   place, so the unit would get the tail's words twice.  Every part's
   INCLUDE_ASM has to go together.

   THE UNIT-WIDE INDEX of this defect is docs/dng-parks/func_lo20_80026EC0.c
   (agent-h161, amended by h171): it lists every spurious symbol in
   ovl_dng_lo20, the one rule that finds them, and the three joined bodies
   it carries itself.  Read it before landing any of these.

   DECLARATIONS
   (none)
   END DECLARATIONS

   Agent h171, batch 35; re-confirmed and corrected by agent-h232, 2026-11-26.
   Every INCLUDE_ASM line for the parts named above has to be removed
   together when this lands:
       csrc/ovl_dng_lo20.c  func_lo20_80028050   (replaced by the body)
       csrc/ovl_dng_lo20.c  func_lo20_800280F4   (deleted outright) */
void func_lo20_80028050(u8 *p, u8 *b, u8 *c, u8 *d)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

    ((u32 *)c)[0] = s1[0];
    ((u32 *)c)[1] = s1[1];
    ((u32 *)c)[2] = s1[2];
    ((u32 *)c)[3] = s1[3];
    ((u32 *)d)[0] = s2[0];
    ((u32 *)d)[1] = s2[1];
    ((u32 *)d)[2] = s2[2];
    *(u8 **)(b + 0x4) = c;
    *(u8 **)(b + 0x8) = d;
    *(s32 *)b = 0;
    *(s16 *)(*(u8 **)(b + 0x4) + 0x8) = -0x58;
    *(s16 *)(*(u8 **)(b + 0x4) + 0xA) = -0x40;
    *(s16 *)(*(u8 **)(b + 0x4) + 0xC) = 0x200;
    *(s16 *)(*(u8 **)(b + 0x8) + 0x2) = 0x400;
    *(s16 *)(*(u8 **)(b + 0x8) + 0x6) = -0x50;
    (*(u8 **)(b + 0x4))[0xF] = 4;
}
