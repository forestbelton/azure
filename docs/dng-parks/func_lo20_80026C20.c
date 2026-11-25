/* func_lo20_80026C20 -- NOT PARKED FOR A RESIDUE.  THE BODY IS COMPLETE AND
   AT 0 DIFFERING; what blocks it is this unit's BROKEN SYMBOL CARVE.

   A NAME-VALIDITY SCAN, and the carve cut it mid-gate.

   func_lo20_80026C20 has NO `jr $ra' and branches to .Llo20_80026C98 and
   .Llo20_80026CC4, both inside func_lo20_80026C88's listing; conversely
   func_lo20_80026C88 branches BACKWARDS to .Llo20_80026C6C, inside
   func_lo20_80026C20's.  The real function is 0x80026C20..0x80026CC8, 43
   words; `func_lo20_80026C88' is its TAIL.

   This one matters beyond its own words: `s32 func_lo20_80026C20(u8 *)' is
   already DECLARED and CALLED by two landed bodies in this unit
   (func_lo20_80025964 and func_lo20_800259CC), so the carve has been hiding
   a function the unit already depends on.

   First compile.  Two readable points, neither priced: the `n = 6 / n = 9'
   gate keeps its branch because jump.c's store-flag transform needs one arm
   to be 0 (func_lo20_80026708 prices the same shape), and the `x == 0' exit
   is a `break' -- jump2 merges the loop exit with the function exit because
   nothing follows the loop.

   ovlverify reads `func_lo20_80026C20: target 26 words, candidate 43 words,
   17 differing' and that is the CARVE's figure, not the body's: it
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

       python3 pairverify.py <copy>/ovl_dng_lo20.c func_lo20_80026C20 func_lo20_80026C88

   parkcheck --splice will FAIL on this file and that is expected: splicing
   the body in replaces the HEAD's INCLUDE_ASM and leaves the tail's in
   place, so the unit would get the tail's words twice.  Every part's
   INCLUDE_ASM has to go together.

   THE UNIT-WIDE INDEX of this defect is docs/dng-parks/func_lo20_80026EC0.c
   (agent-h161, amended by h171): it lists every spurious symbol in
   ovl_dng_lo20, the one rule that finds them, and the three joined bodies
   it carries itself.  Read it before landing any of these.

   DECLARATIONS
   extern u8 D_80029510[];
   END DECLARATIONS

   Agent h171, batch 35.  Every INCLUDE_ASM line for the parts named
   above has to be removed together when this lands. */
s32 func_lo20_80026C20(u8 *p)
{
    s32 r;
    s32 n;
    s32 j;
    u8 *c;
    s32 x;

    r = 0;
    if (p != NULL)
    {
        p += 0x20;
        if (*(s32 *)(p + 0xC) == 0x13)
        {
            n = 6;
        }
        else
        {
            n = 9;
        }
        for (j = 0; j < n * 2; j += 2)
        {
            c = *(u8 **)(p + 0x14) + j;
            x = c[0];
            if (x == 0)
            {
                break;
            }
            if (!((x == D_80029510[0] && c[1] == D_80029510[1]) ||
                  (x == 0x20 && c[1] == 0x20)))
            {
                r = 1;
            }
        }
    }
    return r;
}
