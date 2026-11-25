/* func_lo20_80027A20 -- NOT PARKED FOR A RESIDUE.  THE BODY IS COMPLETE AND
   AT 0 DIFFERING; what blocks it is this unit's BROKEN SYMBOL CARVE.

   THREE FUNCTIONS UNDER ONE SYMBOL -- the `two or more jr $ra in a small
   listing' case, with the second AND third symbols missing.

   func_lo20_80027A20's listing is 44 words with THREE `jr $ra', and splat
   has already named the two inner entry points as DATA -- `alabel
   D_lo20_80027A68' and `alabel D_lo20_80027A88' -- because each is stored
   as a handler pointer rather than called.  The three real functions are

       0x80027A20..0x80027A64   18 words   the dispatcher
       0x80027A68..0x80027A84    8 words   a bare forwarder to it
       0x80027A88..0x80027ACC   18 words   the guarded entry

   and 18 + 8 + 18 = 44.  Measured 44 words / 0 differing, FIRST COMPILE,
   with PAIRVERIFY_SPLIT=1 (the candidate defines each part as its own C
   function, so the measured extent has to run past all three).

   The body below names the second and third after splat's own symbols, so
   that the third one's `*(void **)(p - 0x10) = D_lo20_80027A68' keeps the
   name the listing shows.  Rename them `func_lo20_80027A68' /
   `func_lo20_80027A88' if you prefer, but then the stored pointer names a
   symbol splat does not have and relcand will say so.

   ovlverify reads `func_lo20_80027A20: target 44 words, candidate 18 words,
   26 differing' and that is the CARVE's figure, not the body's: it
   compares the head's TRUNCATED listing against the whole body.
   Joined, the measurement is 44 words / 44 words / 0 differing.

   HOW TO MEASURE IT: ovlverify CANNOT, because it reads the HEAD symbol's
   listing (which stops at the bogus symbol) and compares it word-for-word
   against the whole body.  The figure above comes from
   scratch/azure-dng/agent-h171/pairverify.py, which is ovlverify with one
   change: the target is the CONCATENATION of the listings, in address
   order, and the candidate's extent runs to the symbol after the LAST
   part.  It reuses ovlverify's own masking and reporting, so the numbers
   are comparable.

       python3 pairverify.py <copy>/ovl_dng_lo20.c func_lo20_80027A20 D_lo20_80027A68 D_lo20_80027A88

   parkcheck --splice will FAIL on this file and that is expected: splicing
   the body in replaces the HEAD's INCLUDE_ASM and leaves the tail's in
   place, so the unit would get the tail's words twice.  Every part's
   INCLUDE_ASM has to go together.

   THE UNIT-WIDE INDEX of this defect is docs/dng-parks/func_lo20_80026EC0.c
   (agent-h161, amended by h171): it lists every spurious symbol in
   ovl_dng_lo20, the one rule that finds them, and the three joined bodies
   it carries itself.  Read it before landing any of these.

   DECLARATIONS
   extern void (*D_80029514[])(void);
   s32 func_8004CB2C(u8 *);
   void D_lo20_80027A68(u8 *);
   END DECLARATIONS

   Agent h171, batch 35.  Every INCLUDE_ASM line for the parts named
   above has to be removed together when this lands. */
void func_lo20_80027A20(u8 *p)
{
    if (*(s32 *)(p + 0x20) != 0)
    {
        D_80029514[*(s32 *)(p + 0x14)]();
    }
}

void D_lo20_80027A68(u8 *p)
{
    func_lo20_80027A20(p);
}

void D_lo20_80027A88(u8 *p)
{
    if (func_8004CB2C(p + 0x2C) != 0)
    {
        *(void **)(p - 0x10) = D_lo20_80027A68;
        *(s32 *)p = 0;
    }
    func_lo20_80027A20(p);
}
