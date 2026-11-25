/* func_lo16_8001E824 -- ovl_dng_lo16, 82 target words.
   PARKED by agent h176, batch 35, 2026-11-25.  81 words / 16 ALIGNED
   DIFFERENCES.  Supersedes func_lo16_8001E824.NOTES.txt, which was parked
   UNATTEMPTED (no body) on the false ground that this unit cannot hold a
   jump table.  IT CAN: its sibling table owner func_lo16_8001D4AC LANDED
   this batch at 75/75, 0 differing, with the switch written as a switch.

   THIS FUNCTION OWNS jtbl_lo16_80016034, AND THE TABLE IS SETTLED.
   18 entries, minval 1 (`lbu $v0,0x1($s3)' / `addiu $v1,$v0,-0x1' /
   `sltiu $v0,$v1,0x12'), so table index k is case value k+1.  Entry list,
   read off asm/data/2A3000.data.s:

       idx  case  target        meaning
        0    1    8001E944      = the beqz target = DEFAULT/epilogue
        1    2    8001E944      default fill
        2    3    8001E944      default fill
        3    4    8001E87C      real arm
        4-13 5-14 8001E944      default fill
       14   15    8001E8A4      real arm
       15   16    8001E8E8      INTERIOR label inside case 15's block
       16   17    8001E8B8      INTERIOR label inside case 15's block
       17   18    8001E910      real arm

   So the shape is the brief's "default at an INTERIOR index" crossed with
   two FALL-THROUGH cases: 15 falls into 17 falls into 16.  Source arm order
   is 1, 4, 15, 17, 16, 18 -- strictly ascending ADDRESSES (E87C, E8A4,
   E8B8, E8E8, E910), which is the source order for a table owner.
   `case 1: break;' is REQUIRED and is not optional: it is what makes
   minval 1.  Cases 2, 3 and 5..14 are absent from the source and gcc pads
   their entries with the default label -- writing them out as empty arms is
   not distinguishable from the table and was NOT measured.

   THE ONE REMAINING FAULT, and it is a named compiler mechanism:

   The target keeps FOUR callee-saved registers -- $s0 = f (the p[3] byte),
   $s1 = h (a temp live only inside case 16), $s2 = g (the p[2] byte),
   $s3 = p -- and a -0x28 frame.  The NATURAL source for case 16,

       h = f | -0x80;
       f = h;
       if ((func_800A6D30() & 7) == 0) { g = -1; f = h | 0x40; }

   compiles to THREE callee-saved registers and a -0x20 frame, because
   cse.c:7486 ("Special handling for (set REG0 REG1)") rewrites the pair:
   when the insn immediately BEFORE a `(set f h)' is the `(set h ...)' that
   computed h, cse changes that insn's dest to f and the copy dies.  h then
   never exists, f and h cannot conflict, and global.c has nothing to put in
   $s1.  Verified in the -da dumps: reg 83 (h) is present in probe.c.rtl and
   probe.c.jump and GONE in probe.c.cse, with insn 102's dest changed from
   83 to 81.

   The preconditions of that transform are the lever.  Breaking condition 5
   -- "prev must be the insn that set h" -- is the ONLY thing that recovers
   the fourth register; any insn between the two statements does it.  The
   body below uses the call as that separator and reaches 81/16 with the
   right register COUNT.  It is still 16 rows out because the separator is
   in the wrong place: with `f = h' after the call, reorg fills the jal's
   delay slot with the `or' (backward search) instead of with the copy, and
   the -0x80 constant ends up tied to h's callee-saved register instead of
   staying in $v0.  The target's stream at reorg time is
   [const][or][copy][call], i.e. the copy IS adjacent to the or there -- so
   the real source must contain an insn between them that a LATER pass
   removes or moves (combine, flow, or sched1 across the call).  Finding
   that statement is the whole remaining job.

   MEASURED, all on top of the case-4 and case-17 forms kept below:

     case 16 spelling                                            words/rows
     h = f|-0x80; f = h; if(..){g=-1; f=h|0x40;}      natural      79 / 23
     h = f|-0x80; if(..){g=-1; f=h|0x40;} else {f=h;}              79 / 23
     both of the above together                                    79 / 23
     f |= -0x80; if(..){g=-1; f |= 0x40;}   (no h at all)          79 / 23
     f |= -0x80; h = f; if(..){g=-1; f=h|0x40;}                    79 / 23
     f = ((rand()&7)==0) ? (g=-1, h|0x40) : h;                     79 / 23
     h = f|-0x80; f = h; t = rand(); if((t&7)==0){..}              79 / 23
     h = f|-0x80; t = rand(); if((t&7)==0){..} else {f=h;}         79 / 23
     h = f|-0x80; t = rand(); f = h; if((t&7)==0){..}   KEPT       81 / 16
     h = f|-0x80; t = rand()&7; f = h; if(t==0){..}                81 / 16
     ... with `u32 r' as the separator temp instead of `s32 t'     81 / 25
     if-arm written `f = h|0x40; g = -1;' (order swapped)          81 / 19
   SEVEN of those nine spellings give the IDENTICAL 23 rows, which is the
   brief's signature for a cause no one-at-a-time sweep reaches.

   DECLARATION ORDER IS NOT MEASURABLE HERE: six permutations of
   `f g h t r' all give 79 / 23, so allocation PRIORITY is ruled out and the
   cause is the statement order, exactly as the brief predicts.

   TWO FACTS THAT ARE MEASURED AND LOAD-BEARING, worth carrying out:

   1. `case 18' is `u32 r = func_800A6D30(); g = r % 0x28 + 0x3C;' and the
      u32 LOCAL is required.  Written inline as
      `g = (u32)func_800A6D30() % 0x28 + 0x3C' fold narrows the whole
      TRUNC_MOD_EXPR to HImode -- both operands fit in `unsigned short' --
      and emits a SECOND `andi $v0,$v0,0xFFFF' after the `subu', which the
      target has not got.  82 words / 37 rows with the inline cast against
      81 / 35 with the local, measured 40 words away from the fault.  The
      0xCCCCCCCD/`srl 5' magic multiply is an UNSIGNED divide by 40, so the
      dividend must be unsigned; a plain `func_800A6D30() % 0x28' promotes
      the u16 to signed int and would give 0x66666667.
   2. `case 17' must read `t = (rand()&3)-1; if (t < 0) { f |= 0x40; } g = t;'
      with `g = t' AFTER the if -- 2 rows cheaper than `g = t' before it.
      Same cse.c:7486 mechanism with the sign flipped: before the if, the
      copy's prev IS the `addiu $v0,$v0,-0x1' that set t, so cse folds the
      subtraction's dest into g and the target's `addu $s2,$v0,$zero' (in
      the bgez's delay slot, placed there by reorg from the fall-through
      thread) disappears.  After the if, prev is a CODE_LABEL and the copy
      survives.  THIS IS THE SAME LEVER AS THE OPEN FAULT ABOVE, measured
      working -- so the open fault's answer is very likely a statement
      order that puts a label or an insn before case 16's copy too.
   3. `case 4' is `g = 1; if (p[0] != 0x11) { g = (rand()&7)|4; } f = -0x80;'
      Written as a two-arm if/else with `g = 1' in the else it is 81 / 35:
      reorg then duplicates `f = -0x80' into the jal's delay slot and jump2
      builds a second exit that does `p[2] = g' inline, +2 words in the arm.
      `f = -0x80' first is 80 / 34, and the if/else with `f = -0x80'
      duplicated into both arms is 80 / 27.  The gate is `!=' with the rand
      block as the FALL-THROUGH, which is what gives the target's `beq'.

   DECLARATIONS
   u16 func_800A6D30();
   END DECLARATIONS
*/
void func_lo16_8001E824(u8 *p)
{
    s32 f;
    s32 g;
    s32 h;
    s32 t;
    u32 r;

    g = f = 0;
    switch (p[1])
    {
    case 1:
        break;
    case 4:
        g = 1;
        if (p[0] != 0x11)
        {
            g = (func_800A6D30() & 7) | 4;
        }
        f = -0x80;
        break;
    case 15:
        if (p[0] == 9)
        {
            f |= 0x40;
        }
    case 17:
        if ((func_800A6D30() & 3) == 0)
        {
            t = (func_800A6D30() & 3) - 1;
            if (t < 0)
            {
                f |= 0x40;
            }
            g = t;
        }
    case 16:
        h = f | -0x80;
        t = func_800A6D30();
        f = h;
        if ((t & 7) == 0)
        {
            g = -1;
            f = h | 0x40;
        }
        break;
    case 18:
        r = func_800A6D30();
        g = r % 0x28 + 0x3C;
        break;
    }
    p[2] = g;
    p[3] = f;
}
