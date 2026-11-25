/* func_lo20_80026A64 -- NOT PARKED FOR A RESIDUE.  THE BODY IS COMPLETE AND
   AT 0 DIFFERING; what blocks it is this unit's BROKEN SYMBOL CARVE.

   THE OBJECT'S CONSTRUCTOR, and the carve cut it four words into the body.

   func_lo20_80026A64 is 8 words, has NO `jr $ra', and sets up the 0x28-byte
   frame that func_lo20_80026A84 then saves $ra into (`sw $ra, 0x24($sp)'
   with no `addiu $sp' anywhere in its own listing).  The real function is
   0x80026A64..0x80026B40, 56 words; `func_lo20_80026A84' is its TAIL.

   First compile, so nothing here is priced on this body.  It is
   func_lo20_80026B94's teardown run forwards: allocate the node, build the
   +0x20 record, and on failure release it and set the two 0x8000 flags.
   The +0x1E halfword uses the ARRAY-REF form `((u16 *)p)[0xF] |= 0x8000'
   because the target hoists the D_800814A0 load ABOVE the `sh' --
   func_lo20_80026294's measured fact, 1 word / 15 rows the other way.

   ovlverify reads `func_lo20_80026A64: target 8 words, candidate 56 words,
   48 differing' and that is the CARVE's figure, not the body's: it
   compares the head's TRUNCATED listing against the whole body.
   Concatenated, the measurement is 56 words / 56 words / 0 differing.

   HOW TO MEASURE IT: ovlverify CANNOT, because it reads the HEAD symbol's
   listing (which stops at the bogus symbol) and compares it word-for-word
   against the whole body.  The figure above comes from
   scratch/azure-dng/agent-h171/pairverify.py, which is ovlverify with one
   change: the target is the CONCATENATION of the listings, in address
   order, and the candidate's extent runs to the symbol after the LAST
   part.  It reuses ovlverify's own masking and reporting, so the numbers
   are comparable.

       python3 pairverify.py <copy>/ovl_dng_lo20.c func_lo20_80026A64 func_lo20_80026A84

   parkcheck --splice will FAIL on this file and that is expected: splicing
   the body in replaces the HEAD's INCLUDE_ASM and leaves the tail's in
   place, so the unit would get the tail's words twice.  Every part's
   INCLUDE_ASM has to go together.

   THE UNIT-WIDE INDEX of this defect is docs/dng-parks/func_lo20_80026EC0.c
   (agent-h161, amended by h171): it lists every spurious symbol in
   ovl_dng_lo20, the one rule that finds them, and the three joined bodies
   it carries itself.  Read it before landing any of these.

   DECLARATIONS
   UnkNode *func_8003FC64(s32);
   s32 func_8004491C(UnkNode *, u32);
   extern u8 D_8004CAA0[];
   END DECLARATIONS

   Agent h171, batch 35.  Every INCLUDE_ASM line for the parts named
   above has to be removed together when this lands. */
u8 *func_lo20_80026A64(s32 b, s32 c, s32 d)
{
    u8 *p;
    u8 *q;

    p = (u8 *)func_8003FC64(0);
    if (p != NULL)
    {
        q = p + 0x20;
        if (func_lo20_800269E8(q) != 0)
        {
            *(u8 **)(p + 0xC) = p + 0x80;
            *(s32 *)(q + 0x6C) = func_lo20_800267B0(*(s32 *)(q + 0x70));
            func_lo20_800269A0(q, b, c, d);
            func_8004491C((UnkNode *)p, (u32)D_8004CAA0);
            return p;
        }
        func_8004B1A4(*(s32 *)(q + 0x74));
        ((u16 *)p)[0xF] |= 0x8000;
        D_800814A0 |= 0x8000;
        p = NULL;
    }
    return p;
}
