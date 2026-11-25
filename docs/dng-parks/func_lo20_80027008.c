/* func_lo20_80027008 -- NOT PARKED FOR A RESIDUE.  THE BODY IS COMPLETE AND
   AT 0 DIFFERING; what blocks it is this unit's BROKEN SYMBOL CARVE.

   THE PAGE BUILDER, and the carve cut it inside the clamp.

   func_lo20_80027008 has NO `jr $ra' and branches to .Llo20_800270E4 inside
   func_lo20_80027070's listing; func_lo20_80027070 opens with
   `slti $v0, $s3, 0x9' against a register nothing in its own listing sets.
   The real function is 0x80027008..0x8002710C, 66 words;
   `func_lo20_80027070' is its TAIL.  It is already DECLARED and CALLED by
   the landed func_lo20_80027384.

   FOUR facts, all measured on this body (the first draft was 30 rows):

   1. THE DIVISOR IS 72, NOT 9.  Magic 0x38E38E39 is the SAME constant for
      both; what distinguishes them is the shift after `mfhi'.
      2^(32+s)/d: s=1 gives /9, s=4 gives /72, and the listing has
      `sra $v1, $a3, 4'.  Written `b / 9' the body is the right LENGTH and
      30 rows out, with `sra $v0, $a3, 0x1' the only tell.  Read the shift,
      never the magic.
   2. The quotient is assigned BACK INTO THE PARAMETER -- `b = b / 72', not
      a separate `s32 e'.  One pseudo means the parameter copy
      `addu $s0, $a1, $zero' is needed (the pseudo now lives across the
      func_8004B404 call) and the prologue opens with `sw $s0, 0x10($sp)'.
      With a separate local gcc leaves `b' in $a1, multiplies $a1 directly,
      and the body comes out ONE WORD SHORT.
   3. The destination cursor must be the INLINE `t + i * 0x12', not a `cur'
      local advanced by `cur += 0x12'.  As a giv, loop.c puts its init in
      the PREHEADER -- after the `blez' guard and after the hoisted constant
      base, which is the target's
      `blez / addu $s1,0 / lui $s5 / ori $s5 / addu $s2, $s4' order.  As a
      plain local the init lands BEFORE the guard.
   4. The source address is spelled INT FIRST,
      `(char *)(k * 0x13 + (s32)base)'.  pointer_int_sum always builds
      PLUS(ptr, intop), so `base + k * 0x13' can only emit
      `addu $a1, $s5, $a1'; the target has `addu $a1, $a1, $s5'.

   The early exit is `if (t != NULL) { ... } return (s32)t;' and NOT
   `if (t == NULL) return (s32)t;' -- inside the null test gcc
   constant-folds that return to `move $v0, zero' where the target has
   `addu $v0, $s4, $zero' in the branch delay slot (reorg duplicating the
   one real return).
   func_80069E88 is `strncpy'.  The 0x800157C0 base is a CONSTANT ADDRESS,
   not a symbol: the listing materialises it with `lui' + `ori', which is
   the tell -- a symbol would be `lui' + `addiu %lo'.

   ovlverify reads `func_lo20_80027008: target 26 words, candidate 66 words,
   40 differing' and that is the CARVE's figure, not the body's: it
   compares the head's TRUNCATED listing against the whole body.
   Concatenated, the measurement is 66 words / 66 words / 0 differing.

   HOW TO MEASURE IT: ovlverify CANNOT, because it reads the HEAD symbol's
   listing (which stops at the bogus symbol) and compares it word-for-word
   against the whole body.  The figure above comes from
   scratch/azure-dng/agent-h171/pairverify.py, which is ovlverify with one
   change: the target is the CONCATENATION of the listings, in address
   order, and the candidate's extent runs to the symbol after the LAST
   part.  It reuses ovlverify's own masking and reporting, so the numbers
   are comparable.

       python3 pairverify.py <copy>/ovl_dng_lo20.c func_lo20_80027008 func_lo20_80027070

   parkcheck --splice will FAIL on this file and that is expected: splicing
   the body in replaces the HEAD's INCLUDE_ASM and leaves the tail's in
   place, so the unit would get the tail's words twice.  Every part's
   INCLUDE_ASM has to go together.

   THE UNIT-WIDE INDEX of this defect is docs/dng-parks/func_lo20_80026EC0.c
   (agent-h161, amended by h171): it lists every spurious symbol in
   ovl_dng_lo20, the one rule that finds them, and the three joined bodies
   it carries itself.  Read it before landing any of these.

   DECLARATIONS
   s32 func_lo20_80026EC0(s32, s32);
   s32 func_lo20_80026F04(s32, s32);
   END DECLARATIONS

   Agent h171, batch 35.  Every INCLUDE_ASM line for the parts named
   above has to be removed together when this lands. */
s32 func_lo20_80027008(s32 a, s32 b, s32 c)
{
    s32 k;
    s32 i;
    u8 *t;
    u8 *base;

    b = b / 72;
    t = (u8 *)func_8004B404(0x91);
    if (t != NULL)
    {
        c -= b * 8;
        if (c >= 9)
        {
            c = 8;
        }
        k = func_lo20_80026EC0(a, b * 8);
        for (i = 0; i < c; i++)
        {
            base = (u8 *)0x800157C0;
            strncpy((char *)(t + i * 0x12), (char *)(k * 0x13 + (s32)base), 0x12);
            k = func_lo20_80026F04(a, k + 1);
        }
    }
    return (s32)t;
}
