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

   TWO OF h172's THREE "WHAT TO TRY NEXT" IDEAS ARE NOW PRICED -- agent h181,
   batch 35, same day -- and both are dead ends:

     far-edge cell FIRST and near-edge second in loop 1       84w / 40
     a single `s32 w = 1' local in the entry block, used for
       every literal 1 (both bounds and the `(i + 1)')        82w / 15

   The far-edge-first order is much worse, so the store order in loop 1 is
   settled as it stands.  The `w = 1' local is the more informative failure:
   it LOSES TWO WORDS, because the target materialises the constant 1 FOUR
   times -- once for each loop's entry guard (`addiu $v0, $zero, 0x1' at words
   4 and 47) and once as each loop's movable (`$a1' at word 10, `$a3' at word
   53) -- and one function-scope pseudo collapses two of the four.  So the
   fix must leave all four materialisations standing and only change the
   ORDER within loop 1's preheader; anything that unifies the 1 across the
   loops is moving away from the target.

   WHY THE ORDER IS WHAT IT IS, now read off the listing.  loop.c's
   `move_movables' emits the loop-invariant CONSTANTS in the order they appear
   in the loop BODY, and `strength_reduce' emits the address bases afterwards,
   so the target's `3 / 0x400 / 1 / lui / addiu base / addiu base-6' is
   exactly the body order 3 (`unk_00 = 3'), 0x400 (`unk_02 = 0x400'), 1
   (`addu $v1, $a1, $a0' at word 28, the `1 + i') followed by the two bases.
   In the candidate the 1 lands AFTER the bases, which means it is not in the
   movables list at all -- it is being created later, by cse2 sharing the
   bottom test's `sllv $v0, $a1, $v0' operand with the body's add.  So the
   question is sharper than h172 could state it: WHAT MAKES THE `1' INVARIANT
   INSIDE THE BODY RATHER THAN AT THE BOTTOM TEST?  h172's third idea -- a
   second body-level reference to the literal 1, early, so the movables scan
   sees it before the address computations -- is the one still untried, and it
   must not be a reference that also serves either entry guard.

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
