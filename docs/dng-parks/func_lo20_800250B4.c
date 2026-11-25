/* func_lo20_800250B4 -- NOT PARKED FOR A RESIDUE.  THE BODY IS COMPLETE AND
   AT 0 DIFFERING; what blocks it is this unit's BROKEN SYMBOL CARVE.

   A FOUR-WAY NAME DISPATCHER, and the carve split it in two.

   func_lo20_800250B4 has NO `jr $ra' in its listing and branches forward to
   .Llo20_8002517C and .Llo20_80025180, both defined inside
   func_lo20_80025170's listing.  The real function is
   0x800250B4..0x80025194, 57 words; `func_lo20_80025170' is its TAIL and is
   not a function at all.

   THE ONE FACT, worth 37 aligned differences: the `*(s32 *)(p + 0x4) = v'
   store for the 0x13 arm and for the func_8004A784 arm must be ONE SHARED
   statement after the if/else, not one store per arm.  The target reaches
   it by `j .Llo20_8002517C' from both arms; written out per arm the body is
   still 57 words but everything from the 0x13 arm on is displaced, because
   jump2 does not cross-join two `store + return' tails whose stored
   REGISTER differs.  An `s32 v' with one store is 0.  The LAST arm keeps
   its own store, in the strncpy delay slot.
   func_80069E88 is `strncpy' (splat/common_syms.txt).

   ovlverify reads `func_lo20_800250B4: target 47 words, candidate 57 words,
   10 differing' and that is the CARVE's figure, not the body's: it
   compares the head's TRUNCATED listing against the whole body.
   Concatenated, the measurement is 57 words / 57 words / 0 differing.

   HOW TO MEASURE IT: ovlverify CANNOT, because it reads the HEAD symbol's
   listing (which stops at the bogus symbol) and compares it word-for-word
   against the whole body.  The figure above comes from
   scratch/azure-dng/agent-h171/pairverify.py, which is ovlverify with one
   change: the target is the CONCATENATION of the listings, in address
   order, and the candidate's extent runs to the symbol after the LAST
   part.  It reuses ovlverify's own masking and reporting, so the numbers
   are comparable.

       python3 pairverify.py <copy>/ovl_dng_lo20.c func_lo20_800250B4 func_lo20_80025170

   parkcheck --splice will FAIL on this file and that is expected: splicing
   the body in replaces the HEAD's INCLUDE_ASM and leaves the tail's in
   place, so the unit would get the tail's words twice.  Every part's
   INCLUDE_ASM has to go together.

   THE UNIT-WIDE INDEX of this defect is docs/dng-parks/func_lo20_80026EC0.c
   (agent-h161, amended by h171): it lists every spurious symbol in
   ovl_dng_lo20, the one rule that finds them, and the three joined bodies
   it carries itself.  Read it before landing any of these.

   DECLARATIONS
   u32 func_80049330(u8 *);
   u8 *func_8004A784(s32, s32);
   u8 *func_8004A83C(s32, s32);
   END DECLARATIONS

   Agent h171, batch 35.  Every INCLUDE_ASM line for the parts named
   above has to be removed together when this lands. */
void func_lo20_800250B4(u8 *p, u8 *q, s32 c)
{
    s32 v;
    u8 *d;

    if (c != 0)
    {
        *(s32 *)(p + 0x4) = c;
        return;
    }
    if (q[1] == 0x13)
    {
        v = func_80049330(q) + 0x34;
    }
    else
    {
        v = (s32)func_8004A784(q[1], q[0]);
        if (v == 0)
        {
            *(s32 *)(p + 0x8) = *(s32 *)(D_80073414[q[1]].unk_0C + q[0] * 0x14 + 0xC);
            d = func_8004A83C(q[1], q[0]);
            *(s32 *)(p + 0x4) = (s32)d;
            strncpy((char *)d, *(char **)(p + 0x8), 0xC);
            return;
        }
    }
    *(s32 *)(p + 0x4) = v;
}
