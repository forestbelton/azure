/* PARK: func_lo20_80026EC0 (joined, 0 differing) -- and with it the whole of
   ovl_dng_lo20's unlandable function symbols, agent-h161, 2026-11-25,
   AMENDED BY AGENT h171 the same day (see WHAT h171 ADDED at the end: the
   third body here now measures 0, SIX more joined functions are at 0 in
   their own park files, and one statement in section A is wrong).
   Filed under this name so parkcheck and the landing tools see it; it is not
   a park about a hard body but a park about the UNIT'S SYMBOL TABLE, and it
   carries THREE bodies (func_lo20_80026EC0, func_lo20_80027328 and
   func_lo20_80026F04), each of which spans two of the unit's symbols.  Nothing here can be landed until splat's symbol list for
   this object is corrected, and that is the dispatcher's call, not mine.

   Thirteen of the forty-six names on this agent's list are NOT functions.
   They are fragments of larger functions, or two functions sharing one
   symbol.  Each was diagnosed off the listing, not inferred: the
   discriminator is the number of `jr $ra' in the listing.

   ONE SIMPLE RULE FINDS THEM ALL, and it is worth running over the other
   115 objects:

     * a listing with NO `jr $ra' is a fragment-HEAD -- its body runs on
       into the next symbol (unless its last instruction is a call to a
       noreturn routine, which is why func_lo20_800250B4, ending in
       `jal func_80069E88', still needs reading);
     * therefore the symbol AFTER a no-`jr' listing is a fragment-TAIL;
     * a TINY listing with TWO `jr $ra' where the instruction after the
       first one's delay slot carries no label is TWO FUNCTIONS under one
       symbol -- the second function's symbol is missing entirely.

   Across ovl_dng_lo20's 106 listings that rule names thirteen interior
   symbols as spurious and four addresses as missing symbols.  Two of the
   unit's three already-"landed" bodies, func_lo20_80025FF4 and
   func_lo20_80028208, are themselves fragment-TAILS: they are the epilogues
   of func_lo20_80025E3C and func_lo20_800280FC, and they read 2 words /
   0 differing only because `void f(void) { }' happens to compile to
   `jr $ra' + `nop'.  They are not functions either.

   ---------------------------------------------------------------- the list

   A. func_lo20_8001E814 -- 0 words, NOT CODE AT ALL.
      0x8001E814 is the start of lo20's `c' subsegment and holds a 0x682C
      DATA blob (the Shift-JIS string island splat/dungeon.bin.yaml
      documents).  splat emits no listing for it and
      splat/dungeon.bin_extra_syms.txt declares it as `D_lo20_8001E814';
      the `INCLUDE_ASM(..., func_lo20_8001E814)' in csrc/ovl_dng_lo20.c is
      stale and names a data symbol as a function.
      SEPARATE, LOUDER PROBLEM: the listing was MISSING when this batch
      started -- `tools/decomp/lstcheck.py' reported it, and ovl_dng_b31f's
      func_b31f_80174674 as well.  Nothing in ovl_dng_lo20 could be
      compiled or measured at all until
      docs/recovered-listings/func_lo20_8001E814.s was copied back into
      asm/nonmatchings/ovl_dng_lo20/.  docs/recovered-listings/README.md
      predicted exactly this and says the permanent fix is still a judgement
      call; it is now overdue, because `make splat' deletes the file and the
      next agent handed either unit loses its whole run to it.

   B. FRAGMENT-TAILS whose head is NOT on this agent's list.  Each is the
      back half of a larger body and has no prologue of its own:
        func_lo20_80025170  10w  -- the epilogue of func_lo20_800250B4
                                    (47w, no `jr', ends `jal func_80069E88'
                                    whose delay slot is 80025170's first
                                    instruction; it also restores $ra/$s1/$s0
                                    from a frame it never built)
        func_lo20_80026C88  17w  -- the loop tail of func_lo20_80026C20
                                    (26w, no `jr'); it reads $t4/$t3/$t2/$t0
                                    it never sets and its back branch targets
                                    .Llo20_80026C6C, which is BEFORE its own
                                    first word
      and one fragment-HEAD in the same position:
        func_lo20_80026A64   8w  -- the prologue of the 56-word body that
                                    continues as func_lo20_80026A84 (48w)
        func_lo20_800280F4   2w  -- `jr $ra' + `sb $v0, 0xF($v1)', the tail
                                    of func_lo20_80028050 (41w, no `jr');
                                    both registers are live-in

   C. TWO FUNCTIONS UNDER ONE SYMBOL.  A function symbol is MISSING at each
      address named:
        func_lo20_80025514   7w  -- a 4-word setter at 0x80025514 that
                                    installs &D_lo20_80025480 into +0x10,
                                    then a 3-word getter at 0x80025524
                                    (`lw $v0, 0x40($a0)') with no symbol
        func_lo20_800255AC  20w  -- a 9-word wrapper at 0x800255AC, then an
                                    11-word body at 0x800255D0 with its own
                                    `addiu $sp, $sp, -0x18' prologue
      and, for completeness, one that is NOT on this agent's list but is the
      same defect and is referenced BY one of its functions:
        func_lo20_8002553C  23w  -- a 3-word getter, then a 20-word body at
                                    0x80025548.  func_lo20_80025598 (LANDED)
                                    installs the address of that second body,
                                    which is why its declaration block entry
                                    has to be
                                    `extern u8 D_lo20_80025548[]
                                         __asm__("func_lo20_8002553C + 0xC");'

   D. THREE PAIRS WHERE BOTH HALVES WERE ON THIS AGENT'S LIST.  Removing the
      spurious interior symbol turns each pair into one landable function,
      and the bodies are below.  They were measured against the TWO
      LISTINGS CONCATENATED -- ovlverify takes its target from the
      per-function listing and so stops at the spurious symbol, which is why
      it cannot see these.  The harness that did it is 60 lines and reuses
      ovlverify.report; recreate it or ask for it.

        spurious symbol        joined function      words   result
        func_lo20_80026ED0  -> func_lo20_80026EC0   4+13=17  0 DIFFERING
        func_lo20_80027368  -> func_lo20_80027328  16+ 7=23  0 DIFFERING
        func_lo20_80026F1C  -> func_lo20_80026F04   6+14=20  0 DIFFERING
                                                              (h171; it read
                                                              11 when this
                                                              park was written)

      So ALL THREE are word-exact and finished; land them by deleting the
      interior symbol and splicing the bodies below.  Note that
      func_lo20_80026ED0 is only the LOAD-DELAY `nop' after the loop head
      `.Llo20_80026ECC', and func_lo20_80027368 is only an epilogue -- neither
      address is a branch target of anything.

   WHAT THE THIRD ONE WAS MISSING -- SOLVED BY h171, and the answer is the
   SUBSCRIPT FORM, not the parameter copy this paragraph blames.  The four
   spellings in the table below all keep the base LOOP-INVARIANT or
   pre-offset; what lands is the base assigned INSIDE THE LOOP BODY with the
   stride AND the 0x57D2 BOTH in the subscript, which is the landed
   func_lo20_80026FA8's recorded fact applied here:

       for (i = c; i < 0x40; i++)
       {
           q = (u8 *)0x80010000;
           if (q[i * 0x13 + 0x57D2] == a) { return i; }
       }

   20 words / 0 differing, and 0 under --raw as well, so the displacement
   and the stride are byte-confirmed.  The reasoning below is kept because
   its PRICES are real and its reading of the listing is right; only its
   conclusion (that the remaining job is to keep a parameter copy alive) is
   wrong -- the copy appears by itself once the base is rebuilt in the body.
   The original paragraph follows.

   The target opens with
   `addu $a2, $a1, $zero', a copy of the `start' parameter into the loop
   cursor, and then computes the giv init `start * 0x13' off `$a1' -- the
   ORIGINAL parameter -- while counting in `$a2'.  Writing `for (i = start;
   ...)' with the subscript on `i' lets regmove fold the copy away (19 words,
   one short: the brief's "A MISSING register-to-register COPY IS USUALLY A
   PARAMETER THAT DIES AT ITS COPY", in the direction where the copy must
   SURVIVE).  Priced, joined-target 20 words:

     | spelling                                                   | w  | rows |
     |------------------------------------------------------------|----|------|
     | pointer local advanced in the body, `q[0x57D2]'  KEPT      | 20 |  11  |
     | plain subscript `b[i * 0x13 + 0x57D2]', no pointer local    | 19 |  20  |
     | base pre-offset by `start * 0x13', subscript `(i - start)'  | 27 |  26  |
     | the same with the base rebuilt inside the loop              | 26 |  23  |

   The 11 rows are two causes: the counter register ($a1 against $a2) and
   the ORDER of the rotation guard against the address arithmetic.  The
   target tests `i < 0x40' FIRST and computes the address after, which is
   where loop.c puts a GIV INIT -- so the address must come from the
   SUBSCRIPT, not from a pointer local assigned before the loop, and the
   remaining job is to keep the parameter copy alive in that form.

   NOTE ON THE TABLE ALL THREE WALK.  It is at 0x800157D2, stride 0x13, 64
   entries, and the spelling that matches is a CONSTANT pointer local holding
   0x80010000 with the stride and 0x57D2 inside the subscript -- see
   func_lo20_80026E30's comment in csrc/ovl_dng_lo20.c for the full price
   table and why no symbol spelling can reach it.  splat prints those words
   as `%hi(D_800157D2)'/`%lo(D_800157D2)' in one listing and as the plain
   constant `(0x80010000 >> 16)'/`0x57D2' in another; the instruction words
   are identical and the rendering is not evidence.

   ---------------------------------------------- WHAT h171 ADDED, 2026-11-25

   Batch 35 handed h171 the 22 remaining names in this unit and, by the rule
   at the top of this file, 11 of them are not functions.  Eleven of the
   eleven REAL ones are landed in csrc/ovl_dng_lo20.c.  For the fragments:

   1. THE THIRD BODY HERE NOW MEASURES 0 -- see the amended section above.
      The missing ingredient was the subscript form, not the parameter copy.

   2. SIX MORE JOINED FUNCTIONS ARE AT 0 DIFFERING, each in its own park
      file named after its HEAD, because a landing tool looks up a park by
      the name it is going to define:

        docs/dng-parks/func_lo20_800250B4.c   + 80025170   57w  0 differing
        docs/dng-parks/func_lo20_80026A64.c   + 80026A84   56w  0 differing
        docs/dng-parks/func_lo20_80026C20.c   + 80026C88   43w  0 differing
        docs/dng-parks/func_lo20_80027008.c   + 80027070   66w  0 differing
        docs/dng-parks/func_lo20_80028050.c   + 800280F4   43w  0 differing
        docs/dng-parks/func_lo20_80027A20.c   THREE functions, 44w, 0

      func_lo20_80027A20 is a case this park's rule does not cover: THREE
      `jr $ra' in one 44-word listing, i.e. three functions under one
      symbol, with splat already naming the two inner entries as DATA
      (`alabel D_lo20_80027A68' and `alabel D_lo20_80027A88') because each
      is stored as a handler pointer rather than called.  So the rule's
      third clause should read "two OR MORE `jr $ra'", and an `alabel'
      inside a listing is a strong hint that the missing symbol is a
      FUNCTION entry rather than data.

      That leaves ONE group in this unit unwritten, and it is the big one:
      func_lo20_800274A8 (35w) + func_lo20_80027534 (130w) +
      func_lo20_8002773C (46w) + func_lo20_800277F4 (139w) is ONE function
      of 350 words at 0x800274A8..0x80027A1C.  The chain is forced by the
      branch targets: 800274A8 branches to .Llo20_80027548 (inside 80027534)
      and .Llo20_80027790 (inside 8002773C), 80027534 has no `jr $ra' and
      also branches to .Llo20_80027790, and 8002773C branches to
      .Llo20_80027820 and .Llo20_80027A08 (both inside 800277F4).

   3. SECTION A IS WRONG ON ONE POINT and right on everything else.  It says
      the `INCLUDE_ASM(..., func_lo20_8001E814)' names a data symbol, and
      that is correct -- the recovered listing's own header says "symbol type
      O, not F, 26668 bytes".  But the batch-35 dispatch brief describes
      that same stub as "the unit's text collapsed into one `.word' blob by
      the same defect", which is a different claim and is not what the
      listing says.  The two descriptions should not both be acted on.

   4. A MEASUREMENT TOOL, so the next agent does not rebuild it.  Section D
      says "the harness that did it is 60 lines and reuses ovlverify.report;
      recreate it or ask for it."  It is now
      scratch/azure-dng/agent-h171/pairverify.py -- same invocation as
      ovlverify plus the extra part names, PAIRVERIFY_FLAGS=--raw for raw
      mode and PAIRVERIFY_SPLIT=1 when the candidate defines each part as
      its own C function (which is what func_lo20_80027A20 needs).

   5. TWO PROTOTYPE PROBLEMS THIS PARK'S OWN BODY WALKS INTO.  The joined
      func_lo20_80027328 below stores func_lo20_80026DE0's RETURN VALUE
      (`sw $v0, 0x8($s0)' sits after that `jal' and its delay slot), and
      func_lo20_80026DE0 is already LANDED in csrc/ovl_dng_lo20.c as
      `void'.  This park's DECLARATIONS block declares it `s32', which
      conflicts with the landed definition in the same translation unit.
      Use the project's unit-local alias instead --
      `s32 func_lo20_80026DE0_r(s32, s32) __asm__("func_lo20_80026DE0");'
      -- which is the device this unit already uses for
      func_lo20_80025040_4, and leaves the landed body alone.  Likewise
      func_lo20_80027328 returns `u8 *' where the unit's existing
      declaration says `void'; changing it is safe (its only caller,
      the landed func_lo20_80027454, drops the result and still measures 0
      differing, re-checked across all 70 defined bodies in the unit).

   DECLARATIONS
   void func_8004B1A4(s32);
   s32 func_lo20_80025080(s32);
   s32 func_lo20_80026DE0_r(s32, s32) __asm__("func_lo20_80026DE0");
   u8 *func_lo20_80027328(u8 *, s32);
   END DECLARATIONS */

/* JOINED func_lo20_80026EC0 + func_lo20_80026ED0 -- 17 words, 0 differing
   against the two listings concatenated.  Returns the index of the
   (m + 1)-th table entry whose byte equals `a', or -1. */
s32 func_lo20_80026EC0(s32 a, s32 m)
{
    s32 n;
    s32 i;
    u8 *b;

    n = 0;
    b = (u8 *)0x80010000;
    for (i = 0; i < 0x40; i++)
    {
        if (b[i * 0x13 + 0x57D2] == a)
        {
            if (n == m)
            {
                return i;
            }
            n++;
        }
    }
    return -1;
}

/* JOINED func_lo20_80027328 + func_lo20_80027368 -- 23 words, 0 differing
   against the two listings concatenated. */
u8 *func_lo20_80027328(u8 *p, s32 q)
{
    func_8004B1A4(*(s32 *)(p + 0x8));
    *(s32 *)(p + 0x4) = func_lo20_80025080(*(s32 *)p);
    *(s32 *)(p + 0x8) = func_lo20_80026DE0_r(*(s32 *)p, q);
    return p;
}

/* JOINED func_lo20_80026F04 + func_lo20_80026F1C -- 20 words, 0 DIFFERING
   against the two listings concatenated, and 0 under --raw too (h171).
   Returns the index of the first entry at or after `start' whose byte
   equals `a', or -1.  The base is assigned INSIDE the loop body on purpose:
   loop.c hoists it into the preheader, which is after the entry guard that
   a runtime start index forces.  See the amended analysis above. */
s32 func_lo20_80026F04(s32 a, s32 start)
{
    s32 i;
    u8 *q;

    for (i = start; i < 0x40; i++)
    {
        q = (u8 *)0x80010000;
        if (q[i * 0x13 + 0x57D2] == a)
        {
            return i;
        }
    }
    return -1;
}
