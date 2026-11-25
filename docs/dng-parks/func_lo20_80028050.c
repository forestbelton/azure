/* func_lo20_80028050 -- NOT PARKED FOR A RESIDUE.  THE BODY IS COMPLETE AND
   AT 0 DIFFERING; what blocks it is this unit's BROKEN SYMBOL CARVE.

   THE THIRD TEMPLATE-COPY CONSTRUCTOR, and the carve cut off its last two
   words.

   func_lo20_80028050 has NO `jr $ra'; func_lo20_800280F4 is TWO words,
   `jr $ra' with `sb $v0, 0xF($v1)' in the delay slot, and both $v0 and $v1
   are set in func_lo20_80028050's listing.  The real function is
   0x80028050..0x800280F8, 43 words; `func_lo20_800280F4' is its TAIL.  It
   is already DECLARED and CALLED by the landed func_lo20_8002823C.

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

   HOW TO MEASURE IT: ovlverify CANNOT, because it reads the HEAD symbol's
   listing (which stops at the bogus symbol) and compares it word-for-word
   against the whole body.  The figure above comes from
   scratch/azure-dng/agent-h171/pairverify.py, which is ovlverify with one
   change: the target is the CONCATENATION of the listings, in address
   order, and the candidate's extent runs to the symbol after the LAST
   part.  It reuses ovlverify's own masking and reporting, so the numbers
   are comparable.

       python3 pairverify.py <copy>/ovl_dng_lo20.c func_lo20_80028050 func_lo20_800280F4

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

   Agent h171, batch 35.  Every INCLUDE_ASM line for the parts named
   above has to be removed together when this lands. */
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
