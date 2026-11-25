/* func_lo16_80017A9C -- ovl_dng_lo16, 84 target words.
   PARKED by agent h172, batch 35, 2026-11-25.

   BEST: 84 candidate words, 16 ALIGNED differences, and the word count,
   the instruction sequence and every opcode agree.  The whole residue is a
   REGISTER PERMUTATION caused by ONE HOIST ORDERING in the first loop's
   preheader:

       target     $t0 = 3 / $a3 = 0x400 / $a1 = 1 / lui / addiu base /
                  addiu base-6
       candidate  $a3 = 3 / $a1 = 0x400 / lui / addiu base / addiu base-6 /
                  $t1 = 1

   i.e. the constant 1 is hoisted BEFORE the D_800EA000 base in the target and
   AFTER it in the candidate.  Everything downstream is the same three
   registers renamed.

   THE MECHANISM.  loop.c runs `move_movables' (loop-invariant CONSTANTS)
   before `strength_reduce' (address givs), so a constant hoisted as a movable
   always precedes the base-register insns strength reduction emits.  The
   target's `1' is therefore a MOVABLE -- the literal in `(1 + i)' and in the
   bound `1 << g->unk_16' sharing one register.  In the candidate the `1' lands
   after the bases, which means it is not being hoisted as a movable but is
   coming out of the giv machinery instead.

   WHAT THE FUNCTION DOES.  It paints the map border.  Loop 1 walks rows
   y = 0 .. (1 << g->unk_16) - 1 and writes the cell at `y << g->unk_14'
   (column 0) and the cell at `((1 + y) << g->unk_14) - 1' (the last column);
   loop 2 walks the same count and writes `D_800EA000_s[i]' (row 0) and
   `(((1 << g->unk_16) - 1) << g->unk_14) + i - 1' (the last row).  BOTH loop
   bounds are `1 << g->unk_16' -- the listing reads +0x16 for the x loop too,
   which is only correct because the shipped grid is square.  Each cell gets
   unk_00 = 3, unk_02 = 0x400, unk_04 |= 0x8001, in that source order (the
   `lhu' of unk_04 is scheduled to the head of each group).

   The `- 1' on the two far edges folds into the symbol: the preheader carries
   `addiu $t2, $t1, -0x6' and the address add is `addu $v1, $v0, $t2'.

   SPELLINGS MEASURED (candidate words / aligned rows):

     `(1 + i)' in the far-column index                     84w / 16  <- kept
     `(i + 1)'                                             84w / 16  FREE,
       and NOT MEASURABLE which of the two the target wrote, except that the
       target's `addu $v1, $a1, $a0' puts the CONSTANT register first, which
       `1 + i' is the natural spelling of

   WHAT TO TRY NEXT: get the `1' hoisted as a movable ahead of the bases.
   Untried ideas, cheapest first: write the loop bound through a local
   (`s32 w = 1 << g->unk_16;' -- but check it does not get hoisted out of the
   second loop as well, the target recomputes `1 << g->unk_16' at both loop
   bottoms); write the far-edge index with the `1' appearing EARLIER in the
   body than the near edge's base, e.g. by putting the far-edge cell first and
   the near-edge second (this was not measured and it also changes the store
   order, so price both halves); or give the body a second reference to the
   literal 1 so `move_movables' sees a bigger saving.

   DECLARATIONS
   typedef struct UnkDungeonCell
   {
       u16 unk_00;
       u16 unk_02;
       u16 unk_04;
   } UnkDungeonCell;
   extern UnkDungeonCell D_800EA000_s[] __asm__("D_800EA000");
   typedef struct UnkDngLo16Grid
   {
       UnkDungeonCell *unk_00;
       u8 pad_04[0x10];
       s16 unk_14;
       s16 unk_16;
       s16 unk_18;
       s16 unk_1A;
       s16 unk_1C;
       s16 unk_1E;
   } UnkDngLo16Grid;
   END DECLARATIONS

   D_8008333C is `extern u32 D_8008333C[]' in include/structs.h.  The grid
   descriptor must be reached as a STRUCT pointer, not a `u8 *': the shift
   reads stay inside both loops here, which is what the struct form gives.
*/
void func_lo16_80017A9C(void)
{
    UnkDngLo16Grid *g;
    UnkDungeonCell *c;
    s32 i;

    g = (UnkDngLo16Grid *)D_8008333C;
    for (i = 0; i < (1 << g->unk_16); i++)
    {
        c = &D_800EA000_s[i << g->unk_14];
        c->unk_00 = 3;
        c->unk_02 = 0x400;
        c->unk_04 |= 0x8001;
        c = &D_800EA000_s[((i + 1) << g->unk_14) - 1];
        c->unk_00 = 3;
        c->unk_02 = 0x400;
        c->unk_04 |= 0x8001;
    }
    for (i = 0; i < (1 << g->unk_16); i++)
    {
        c = &D_800EA000_s[i];
        c->unk_00 = 3;
        c->unk_02 = 0x400;
        c->unk_04 |= 0x8001;
        c = &D_800EA000_s[(((1 << g->unk_16) - 1) << g->unk_14) + i - 1];
        c->unk_00 = 3;
        c->unk_02 = 0x400;
        c->unk_04 |= 0x8001;
    }
}
