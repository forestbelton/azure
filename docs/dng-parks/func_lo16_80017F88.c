/* func_lo16_80017F88 -- ovl_dng_lo16, 121 target words.
   ADVANCED by agent h197, batch 36, 2026-11-25, from 124 words / 17 ALIGNED
   differences to 121 WORDS -- EXACT -- and 11 ALIGNED differences.

       built 121 words, target 121 words, 11 aligned differences
       func_lo16_80017F88: target 121, candidate 121, 21 differing (masked)

   h176 NAMED THE JOB EXACTLY -- "get `(s16)f` into a callee-saved register
   with `cb` still spilled" -- AND THE ANSWER IS ONE STATEMENT`S POSITION.
   The extension must be an explicit local assigned AT THE TOP OF THE OUTER
   LOOP BODY, not at function scope:

       for (n = ...; n > 0; n--)
       {
           fl = f;                       <<< here, and nowhere else
           x = D_800E2970_s[r].unk_00;

   loop.c then hoists `fl = f` into the outer preheader -- which is where the
   target`s `sll $v0, $a3, 16` / `sra $fp, $v0, 16` pair sits, words 30-31 --
   and because `flow.c:2122` counts a reference once per ENCLOSING LOOP the
   hoisted definition is worth TWO references instead of one.  `fl` then
   scores 5 weighted references against `cb``s 4 in `allocno_compare`, takes
   $fp, and `cb` spills to 0x10($sp) exactly as the target does.  At function
   scope the two allocnos TIE at 4 references each and the tie breaks on
   allocno number, which `cb` wins because it is declared first -- and that
   is why h176 measured every declaration order at the same 22 rows.  The
   position sweep, all with `s32 fl`:

       fl = f at the top of the OUTER loop body        121w / 11   <- kept
       fl = f just before the outer `for`              122w / 22
       fl = f after `y = ...`                          122w / 22
       fl = f as the first statement                   122w / 22
       fl = f at the top of the INNER loop body        124w / 17
       no `fl` at all, `f == 0` inlined                124w / 17
       `s16 fl` (any position)                         124w / 17
       `s32 fl = (s16)f`                               122w / 22

   WHY THE REFERENCE DEVICES DO NOT WORK HERE, measured, because they are the
   first thing the brief suggests for an `allocno_compare` residue:
       `do { } while (0)` round the `f` gate (with fl)  122w / 65
       `do { } while (0)` round the 0x8400 gate         122w / 65
       `do { } while (0)` round the `f` gate (bare f)   125w / 73
   A wrapper inside the inner loop raises BOTH allocnos` counts and adds a
   sched barrier in the hot block; the hoist raises only one and adds none.

   WHAT IS LEFT -- 11 ROWS, AND THEY ARE ONE REGISTER CHOICE IN THE ENTRY
   BLOCK.  The target puts `%lo(D_800E2970)` in $a1 and `%hi(D_8008333C)` in
   $v0; the candidate puts them in $a2 and $a1.  Because the candidate needs
   an argument register for the SECOND address it spills `ox` ($a1) early,
   at word 12, where the target`s parameter home store for $a1 is at word 4
   and `oy` ($a2) at word 21 -- so the two parameter home stores come out in
   the opposite order and the `lui %hi(D_8008333C)` is scheduled eleven words
   early.  RULED OUT for it, all at 121 words / 11:
       `cb = ...` after `y = ...`, before the `for`, or first             11
       `cb = ...` at the top of the inner loop                     121w / 21
       `cb = ...` at the top of the outer loop                     122w / 25
       `if (1)` round the `cb` load, after `y` or first                  11
       `do { } while (0)` round the `cb` load                      123w / 25
       `cb` read through a `u8 *` cast instead of `D_8008333C[0]`        11
       `*ox`/`*oy`/`best` in the orders ox,best,oy and best,ox,oy        11
       `*oy` before `*ox`                                          121w / 15
   So the cb load`s source position does not move its schedule, and the
   residue is below the C level.  NEXT AGENT: a `-da` `.sched`/`.lreg` pair
   is the only thing left; do not re-sweep the fl position (closed above),
   the cb position, or the store order.

   FACTS MEASURED AND LOAD-BEARING:

   1. THE CELL BASE IS AN `s32', NOT A `UnkDungeonCell *'.  The subscript is
      `(UnkDungeonCell *)((x + (y << g->unk_14)) * 6 + cb)' and the target's
      final add is `addu $v0, $v0, $t0' -- INDEX FIRST.  Off a
      `UnkDungeonCell *cb' it is `addu $v0, $t0, $v0', base first, because
      pointer_int_sum always builds PLUS(ptr, intop): 124 words / 18 with
      the pointer form against 124 / 17 with the integer one, and the
      operand exchange is visible in the diff.  Same rule as
      func_lo16_80019A84 and func_lo16_80017480 in this unit.
   2. `g' MUST BE INLINED AT ITS USE SITE as
      `((UnkDngLo16Grid *)D_8008333C)->unk_14' and must NOT be a local.  As
      a local assigned inside the inner loop, loop.c hoists it to the outer
      preheader, it needs a register the body has not got and gcc spills it
      across the func_800BCB04 call: 123 words / 46 differing with
      `sw a3,24(sp)' in the dress block.  Inlined, the `lui'/`addiu %lo'
      pair is re-materialised per inner iteration, which is what the target
      has.  NOTE THAT THIS INVERTS func_lo16_8001816C, 500 words away in the
      same unit, where `g' as a function-scope local is what lands -- the
      brief's "TWO BODIES IN ONE UNIT CAN WANT OPPOSITE ANSWERS", and the
      discriminator is the frame: 8001816C saves nine and has a register to
      spare, this one saves nine and does not.
   3. The subscript operand order is `x + (y << g->unk_14)' -- the INDEX
      first and the shift second, the opposite of func_lo16_80017668's
      `(y << g->unk_14) + j'.  Read `addu $v1, $a0, $v1' in the listing.
   4. `best' is `s16' and takes the RAW call result (`addu $s3, $a0, $zero'),
      while both comparisons go through its sign extension.  `h' is `s16'
      for the same reason and keeps TWO pseudos, the raw $a0 for `best = h'
      and the extended $v1 for `h < 0x200' and `best < h'.
   5. The two cell-flag tests are NESTED `if's and the `f' gate is an `||'
      INSIDE the second one, in that order: `(v & 0x8400) == 0' then
      `f == 0 || (v & 0x300) == 0x200'.  Swapping the `||' operands is
      125 words / 22.
   6. `u16 t = y * 64 + 0x20' as a local, hoisted by loop.c into the inner
      loop's preheader; inlining it at the call measures the same 124 / 17
      (NOT MEASURABLE here) and writing it as an explicit outer-loop
      statement is 124 / 25.

   (h176's closing note -- "the whole job is to get (s16)f into a callee-saved
   register with cb still spilled" -- IS DONE; see the top of this header.)

   DECLARATIONS
   extern u32 D_8008333C[];
   END DECLARATIONS
*/
s32 func_lo16_80017F88(s16 r, s16 *ox, s16 *oy, s16 f)
{
    UnkDungeonCell *c;
    s32 cb;
    s16 y;
    s16 n;
    s16 x;
    s16 w;
    s16 h;
    s16 best;
    u16 v;
    u16 t;
    s32 fl;

    best = -0x200;
    cb = (s32)D_8008333C[0];
    y = D_800E2970_s[r].unk_02;
    for (n = D_800E2970_s[r].unk_06; n > 0; n--)
    {
        fl = f;
        x = D_800E2970_s[r].unk_00;
        for (w = D_800E2970_s[r].unk_04; w > 0; w--)
        {
            c = (UnkDungeonCell *)((x + (y << ((UnkDngLo16Grid *)D_8008333C)->unk_14)) * 6 + cb);
            v = c->unk_04;
            if ((v & 0x8400) == 0)
            {
                if (fl == 0 || (v & 0x300) == 0x200)
                {
                    t = y * 64 + 0x20;
                    h = func_800BCB04((x * 64 + 0x20) & 0xFFE0, t, -0x400);
                    if (h < 0x200 && best < h)
                    {
                        *ox = x;
                        *oy = y;
                        best = h;
                    }
                }
            }
            x++;
        }
        y++;
    }
    if (best == -0x200)
    {
        best = 0x200;
    }
    return best;
}
