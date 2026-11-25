/* PARK: func_b31f_801712C4 (ovl_dng_b31f) -- 82 words / 82 words /
   16 ALIGNED DIFFERENCES.  Agent h173, batch 35.  One-member family.
   OWNS NO JUMP TABLE.

   WHAT IT DOES: the bank's arming routine for the D_b31f_80174CD8 actor.
   Publish D_b31f_80174B38 into D_b31f_80174AB4, clear D_b31f_80174AB8, put
   1 in the record's +0xB2 and +0xB0, copy four bytes of D_b31f_80174C58 to
   the raw address 0x80013720, raise 0x8000000 in D_800E296C, set bits 0 and
   3 and clear bit 4 of the 0x80013714 word, zero 0x8001371A/18/16, publish
   D_b31f_80174C58's ADDRESS at 0x8001371C, then walk the engine's two +0xAC
   actor slots through func_b31f_8016F140 and func_8009A028, raising
   0x80000000 in each live one's node +0x10.

   WORD COUNT IS EXACT.  The whole walk loop matches, and so does the tail
   from the 0x3714 `ori' on.  All 16 rows are in stream positions 12..24 and
   30..36 and they are ONE GROUP SCHEDULED EARLY: the candidate's
   `D_800E296C' read-modify-write (its `lui'/`lw'/`or'/`sw') lands at
   positions 15..22 where the target has it at 30..35, displacing the
   `lw %lo(D_b31f_80174CD8)', the `li 1' and the `lui %hi(D_b31f_80174C58)'
   by two slots each.

   TWO THINGS THIS BODY SETTLES FOR THE UNIT, both from the listing:
   1. THE 0x8001xxxx BLOCK IS THE CONSTANT BASE AGAIN -- one `lui` with no
      `addiu` serving 0x3714/0x3716/0x3718/0x371A/0x371C, exactly as
      func_b31f_8016DC8C measured (struct alias 16/5, declared array 16/5,
      five symbols 19/13, raw constants 21/21).  So THIS BODY ADDRESSES A
      CONSTANT BASE and a masked `0 differing` would not be sufficient for
      it: the four displacements must be read off the listing by eye, and
      the three zero stores are written DESCENDING (0x371A, 0x3718, 0x3716)
      as 8016DC8C requires.
   2. 0x80013720 IS A SEPARATE RAW CONSTANT ADDRESS, NOT `g + 0x3720`.  The
      target builds it with `lui $a2, 0x8001' / `ori $a2, 0x3720' -- the
      lui/ori pair is how cc1psx materialises an integer CONSTANT, where a
      symbol gives lui/addiu %lo and the 0x80010000 base gives a bare `lui'
      because its low half is zero.  A displacement off `g' would have been
      legal (0x3723 fits an `swl') and is not what the target does.
   3. The four-byte copy is UNALIGNED (`lwl'/`lwr' + `swl'/`swr'), so it
      goes through a 4-byte `u8 bb[4]' name struct of alignment 1 -- the
      same device as func_b31f_8016E138's UnkDngB31fName12 record copy.
   4. `i' IS `s32' HERE, not `s16': this loop's index is `sll $s0, $s1, 2'
      and its test a bare `slti $v0, $s1, 0x2'.  (func_b31f_8016DF94's
      otherwise identical walk has `sll 16'/`sra 14' and `sll 16'/`sra 16',
      which is the `s16' form.  The two walks in one unit want different
      widths, so neither is evidence about the other.)

   MEASURED AND REJECTED -- 30 orders, and the sweep is FLAT AT 16 on every
   axis.  Statement labels: A = the D_b31f_80174AB4 publish,
   B = `q = D_b31f_80174CD8 + 0x20', C = `D_b31f_80174AB8 = 0',
   D = the +0xB2/+0xB0 pair, E = the four-byte copy,
   F = `D_800E296C |= 0x8000000', G = `g = (u8 *)0x80010000',
   H = the 0x3714 read-modify-write, I = the three zero stores,
   J = the 0x371C publish.

   | order                                         | aligned |
   | G A B C D E F H I J   (plain source reading)  |   28    |
   | G F A B C D E H I J   (as written below)      |   16    |
   F's POSITION SWEPT ACROSS ALL NINE SLOTS: 16, 16, 16, 16, 28, 28, 23,
   23, 23 -- so F must come before the +0xB2/+0xB0 pair and anywhere there
   is equivalent.  Not convex, and 16 is the floor.
   E's POSITION SWEPT ACROSS ALL EIGHT SLOTS with F first: 37, 37, 37, 37,
   16, 16, 16, 16 -- E must come after the +0xB2/+0xB0 pair.
   Also measured, all with F first: I before H 16; E last 16; `g' assigned
   just before its own block instead of at the top 16 (NOT MEASURABLE here,
   so this body is no evidence for func_b31f_8016DDB8's position fact);
   `q' computed before the D_b31f_80174AB4 publish 16.
   WORSE: the g block written first 56; A moved after D 33; C after D 26;
   J before I 27; the +0xB0 store before the +0xB2 store 17; the
   D_b31f_80174AB8 clear before the D_b31f_80174AB4 publish 22.

   WHAT I WOULD TRY NEXT: the D_800E296C read-modify-write is the thing out
   of place and no statement position moves it, so the next question is
   whether it is a read-modify-write at all in the source -- something that
   gives its `lw' a longer dependence chain (a value read into a local and
   used later, or the raise written through a pointer local shared with
   something else) would drop its sched1 priority and let it fall to the end
   of the block.  gcc 2.7.2's sched1 is bottom-up, so an insn emitted LATE
   is one picked EARLY, i.e. one with a LOW priority and a LOW luid.

   DECLARATIONS
   extern s32 D_b31f_80174AB4;
   extern s16 D_b31f_80174AB8;
   extern u8 D_b31f_80174B38[];
   extern u8 D_b31f_80174C58[];
   extern u8 *D_b31f_80174CD8;
   extern u32 D_800E296C;
   extern u8 *D_800E3D7C;
   void func_8009A028(u8 *);
   void func_b31f_8016F140(u8 *p);
   typedef struct UnkDngB31fName4
   {
       u8 bb[0x4];
   } UnkDngB31fName4;
   END DECLARATIONS */
void func_b31f_801712C4(void)
{
    u8 *g;
    u8 *q;
    u8 *e;
    s32 i;

    g = (u8 *)0x80010000;
    D_800E296C |= 0x8000000;
    D_b31f_80174AB4 = (s32)D_b31f_80174B38;
    q = D_b31f_80174CD8 + 0x20;
    D_b31f_80174AB8 = 0;
    *(s16 *)(q + 0xB2) = 1;
    *(s16 *)(q + 0xB0) = 1;
    *(UnkDngB31fName4 *)0x80013720 = *(UnkDngB31fName4 *)D_b31f_80174C58;
    *(u16 *)(g + 0x3714) = (*(u16 *)(g + 0x3714) | 9) & ~0x10;
    *(s16 *)(g + 0x371A) = 0;
    *(s16 *)(g + 0x3718) = 0;
    *(s16 *)(g + 0x3716) = 0;
    *(s32 *)(g + 0x371C) = (s32)D_b31f_80174C58;
    for (i = 0; i < 2; i++)
    {
        if (*(u8 **)(D_800E3D7C + i * 4 + 0xAC) != NULL)
        {
            func_b31f_8016F140(*(u8 **)(D_800E3D7C + i * 4 + 0xAC));
            func_8009A028(*(u8 **)(D_800E3D7C + i * 4 + 0xAC));
            e = *(u8 **)(D_800E3D7C + i * 4 + 0xAC) - 0x20;
            *(s32 *)(e + 0x10) |= 0x80000000;
        }
    }
}
