/* func_s56_8002401C -- ovl_dng_s56, 219 words.  THE UNIT'S JUMP-TABLE OWNER.
   PARKED at 219 words / 6 differing (ovlverify) / 2 ALIGNED (agent h179,
   batch 35).  dngfam: one-member family.

   *** THE TABLE, RESOLVED.  jtbl_s56_80024008, 5 words = 5 cases + 0 pad,
   dispatch bound 5, 5 distinct targets.  MINVAL IS 0: the dispatch is
   `lh $v1, 0xA($s1)' / `sltiu $v0, $v1, 0x5' with NO subtraction, so entry k
   is case k.  THERE IS NO DEFAULT AMONG THE ENTRIES -- the range check's
   `beqz $v0, .Ls56_80024290' goes to the function's COMMON TAIL, which is not
   any entry's address.  So all five arms are real and written out:

       entry 0  .Ls56_8002407C   case 0   seeds the bank state, zeroes
                                          D_800274DC and calls
                                          func_s56_80025AD8 -- FALLS THROUGH
       entry 1  .Ls56_80024184   case 1   spawns func_s56_8002453C, breaks
       entry 2  .Ls56_800241E0   case 2   counts +0x1C down, calls
                                          func_800A56E0(0x300) -- FALLS THROUGH
       entry 3  .Ls56_80024214   case 3   waits on D_800273BC, calls
                                          func_s56_80025C8C, breaks
       entry 4  .Ls56_80024244   case 4   waits on D_80027330, retires the
                                          actor, breaks

   Case 0 falls through into case 1 and case 2 falls through into case 3 (case
   2's fall-through is CONDITIONAL -- `if (n >= 0) break;' first), so the arms
   are written in ascending source order 0,1,2,3,4 and NO `default:' label.
   Arms 1 and 3 both end with the `*(s16 *)(p + 0xA) += 1' bump and jump2
   cross-joins them into the target's shared .Ls56_80024230; writing it out in
   both places is what produces that.  `.Ls56_80024290' and `.Ls56_80024294'
   are ONE tail -- the difference is only that reorg duplicated the
   `lui $a0, %hi(D_80027398)' into the delay slots of the paths that reach the
   second label.  Measured with the arms as above; the arms are NOT
   cross-joinable with each other, so ovlverify's clean rows ARE evidence
   about their order here. ***

   THE RESIDUE -- 2 aligned rows, one load's slot:
       target row 32   lw $a0, 0x0($s1)     (the value for D_s56_8002732C)
       candidate       the same load at row 37
   Everything else is byte-identical, including both nested loops, all five
   arms, the tail's 49 func_s56_80026064 calls and the epilogue.  The target
   hoists that load five slots earlier, holding $a0 from row 32 to the store
   at row 40; the candidate holds it from 37.  Both blocks are dense (no nops
   either side), so this is a pure `rank_for_schedule' choice on a load that
   is ready from the top of the block.  Twelve spellings left it at exactly
   these 2 rows -- see the table.

   MEASURED TABLE (all at 219 candidate words; aligndiff rows):

   FACTS THAT HOLD (each one priced by removing it from the kept body):
   * `D_800274DC' IS AN ARRAY REF IN THE ZEROING LOOP (`D_800274DC[j][i] =
     NULL') AND A ROW POINTER IN NEITHER.  With a `row' pointer local bumped
     by 8, `j' has no use left but the exit test, `check_dbra_loop' REVERSES
     the outer loop (`li 6' / `addiu -1' / `bgez' where the target has
     `addiu +1' / `slti 7' / `bnez'), and that is 36 rows.  Keeping `j' in the
     address blocks the reversal.
   * THE TAIL LOOP IS ALSO AN ARRAY REF (`func_s56_80026064((u8 *)w,
     D_800274DC[j][i])').  With a row pointer the loop-invariant `0x10' that
     feeds `w[2] = w[3] = 0x10' is hoisted one slot LATER than the giv's init,
     because a for-init is emitted before loop.c's invariant hoist while a
     giv's init is emitted after it: 2 rows.  (Same mechanism as
     func_s56_80025C8C's `row[i]' fact, with the sign of the conclusion
     reversed because there the cursor was the giv.)
   * THE func_s56_80025AD8 ARGUMENTS MUST GO THROUGH `s32' LOCALS.  Written
     inline the two `D_8006CCD8'/`D_8006CCE8' halfword reads come out `lhu'
     where the target has `lh' -- the call narrows to the callee's `s16'
     parameter, convert.c distributes the truncation into the sum and the
     high half stops mattering, so combine drops the sign extension.  An
     `s32' local keeps the sum in `int': 28 rows.
   * AND THE `(x >> 8) & 0xE' INDEX MUST BE INLINED IN BOTH ARGUMENTS, not
     hoisted into a local.  As a local `k' the D_800E3D7C load is the first
     thing in its statement and the scheduler lifts it above the
     `D_800273A8 = 0' store and ahead of `D_80082E80[0x24]': 6 rows.  Inlined,
     the first operand of each argument is the `lbu' and the order is the
     target's.
   * `func_s56_80025AD8' TAKES TWO `s16' PARAMETERS, not `u16': the call sites
     here are `sll 16'/`sra 16' sign extensions, where a `u16' parameter would
     be `andi 0xFFFF'.  **This contradicts the signature in
     docs/dng-parks/func_s56_80025AD8.c, which was derived from the callee's
     own spill reloads; the callee's `lhu' reload is explicable with an `s16'
     parameter (only the low half of `x - 3' matters) but the caller's sign
     extension is not explicable with `u16', so s16 wins and that park's
     prototype should be changed.**
   * CASE 4's D_80027330 TEST READS IT SIGNED, AS `*(s16 *)&D_80027330'.
     The unit declares `extern u16 D_80027330;' (three landed bodies do
     `D_80027330 += 1' off it and must keep it), and `D_80027330 == 0' or
     `(s16)D_80027330 == 0' both give `lhu' where the target has `lh': 1 row.
     The plain-cast read of the symbol's address is the only spelling that
     reaches `lh' without changing the declaration.
   * CASE 1 READS D_800814A8 THROUGH A POINTER LOCAL (`w8 = (u8 *)D_800814A8;'
     ahead of the two stores) WHILE CASE 0 READS IT DIRECTLY THREE TIMES.
     Direct in case 1, the `lui'/`lw' pair has a higher LUID than the
     `*(s16 *)(p + 0x1C) = 0x10' store, nothing is left to cover the `lw's
     delay slot and the body is 220 WORDS with an extra `nop': 7 rows and a
     word.  Splitting the +0xA6 read-modify-write around the 0x1C store as
     well is 25 differing, so the pointer local is the whole fact.
   * `D_800273BE = p[0x9]' MUST BE THE SECOND STATEMENT OF CASE 0, between
     `D_800273BC = 1' and `D_s56_80027328 = 0'.  Third (after the 80027328
     store) its `lbu' lands three slots late: 2 rows.
   * CASE 4's D_80083460 COUNTER BUMP IS SPLIT around the +0xC store
     (`c = *(u16 *)(g2 + 0xA); *(u32 *)(g2 + 0xC) = 0;
     *(s16 *)(g2 + 0xA) = c - 1;') and reached through ITS OWN pointer local,
     separate from the one used for D_80027374 in the tail.  Sharing one local
     between the two splits the `lui'/`addiu' pair across two registers where
     the target ties them.
   * `g = &D_80083160' is a pointer local used only by the last two stores
     (`x[0x8] = (g->unk_04 & 0x1F) + 0x80; x[0x9] = g->unk_04 & 0x1F;' off a
     second local `x = D_80027374').

   NOT MEASURABLE / FREE, measured:
   * the position of `D_s56_8002732C = *(u8 **)p' among case 0's five opening
     stores: slots 3 and 4 are both 2 rows, slots 0, 1 and 2 are 5-6 rows, so
     the curve has a flat minimum and the statement order is not the cause.
   * `*(u32 *)&D_s56_8002732C = *(u32 *)p', `((u8 **)p)[0]', and
     `((u8 *)D_800814A8)[0xA8]' are each bit-identical to the kept spelling
     (2 rows), so neither the store width nor MEM_IN_STRUCT_P on those
     references moves the residue.  `((u32 *)D_800814A8)[0x3D]' for the +0xF4
     store is 16 rows -- worse.
   * reordering case 0's two `*(u32 *)p' reads: free (2 rows either way).
   * a local holding `*(u8 **)p' for the D_s56_8002732C store: 15-17 rows,
     worse -- the target genuinely loads p + 0 twice.

   WHAT TO TRY NEXT: `-da' on the opening block of case 0 to read that load's
   INSN_PRIORITY against the five `lui'/`sw' pairs around it, or the permuter.

   DECLARATIONS
   extern u32 D_s56_80027328;
   extern u8 D_800273BE;
   extern u32 D_800273C0;
   extern u32 D_800273A8;
   extern u8 D_80027398[];
   extern u8 D_80027374[];
   extern u8 D_80082E80[];
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern u8 *D_800E3D7C;
   extern u8 *D_s56_8002732C;
   extern u8 *D_800274DC[][8];
   extern s16 D_800273BC;
   extern u16 D_80027330;
   s32 func_800A56E0(s32);
   void func_800B835C(u8 *, s32 *, s32, s32);
   void func_s56_80025AD8(s16, s16);
   void func_s56_80025C8C(void);
   u8 *func_s56_8002453C(u8 *, u8 *);
   void func_s56_80026064(u8 *, u8 *);
   END DECLARATIONS
*/
void func_s56_8002401C(u8 *p)
{
    Unk80083160 *g = &D_80083160;
    u8 *q;
    u8 *t;
    u8 *x;
    u8 *w8;
    u8 *g2;
    s32 i;
    s32 j;
    s32 ax;
    s32 az;
    s16 n;
    u16 c;
    s16 w[4];

    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        D_800273BC = 1;
        D_800273BE = p[0x9];
        D_s56_80027328 = 0;
        D_800273C0 = *(u32 *)p;
        D_s56_8002732C = *(u8 **)p;
        *(u32 *)(D_800814A8 + 0xF4) = 0;
        *(u8 *)(D_800814A8 + 0x102) = 1;
        *(u8 *)(D_800814A8 + 0xA8) = p[0x8];
        for (j = 0; j < 7; j++)
        {
            for (i = 6; i >= 0; i--)
            {
                D_800274DC[j][i] = NULL;
            }
        }
        D_800273A8 = 0;
        ax = D_80082E80[0x24] +
             (*(s16 *)((u8 *)D_8006CCD8 + ((*(u16 *)(D_800E3D7C + 0x2A) >> 8) & 0xE))
              << 2);
        az = D_80082E80[0x25] +
             (*(s16 *)((u8 *)D_8006CCE8 + ((*(u16 *)(D_800E3D7C + 0x2A) >> 8) & 0xE))
              << 2);
        func_s56_80025AD8(ax, az);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
    case 1:
        if ((**(u16 **)(p + 0x4) & 0x80) != 0)
        {
            q = *(u8 **)p;
            t = func_s56_8002453C(*(u8 **)(q - 0x18), q);
            *(u32 *)(p + 0xC) = (u32)t;
            if (t != NULL)
            {
                w8 = (u8 *)D_800814A8;
                *(s16 *)(p + 0x1C) = 0x10;
                *(u16 *)(w8 + 0xA6) -= 1;
                *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            }
        }
        break;
    case 2:
        n = *(u16 *)(p + 0x1C) - 1;
        *(s16 *)(p + 0x1C) = n;
        if (n >= 0)
        {
            break;
        }
        func_800A56E0(0x300);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
    case 3:
        if (D_800273BC == 0)
        {
            func_s56_80025C8C();
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        if (*(s16 *)&D_80027330 == 0)
        {
            g2 = (u8 *)&D_80083460;
            c = *(u16 *)(g2 + 0xA);
            *(u32 *)(g2 + 0xC) = 0;
            *(s16 *)(g2 + 0xA) = c - 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    w[0] = 0x340;
    w[1] = 0x180;
    w[2] = 0x70;
    w[3] = 0x70;
    func_800B835C(D_80027398, (s32 *)w, 0, 2);
    for (j = 0; j < 7; j++)
    {
        for (i = 0; i < 7; i++)
        {
            w[0] = 0x340 + i * 0x10;
            w[1] = 0x180 + j * 0x10;
            w[2] = 0x10;
            w[3] = 0x10;
            func_s56_80026064((u8 *)w, D_800274DC[j][i]);
        }
    }
    D_80027330 = 0;
    x = D_80027374;
    x[0x8] = (g->unk_04 & 0x1F) + 0x80;
    x[0x9] = g->unk_04 & 0x1F;
}
