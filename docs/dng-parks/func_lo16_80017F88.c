/* func_lo16_80017F88 -- ovl_dng_lo16, 121 target words.
   PARKED by agent h176, batch 35, 2026-11-25.  124 words / 17 ALIGNED
   DIFFERENCES, and the seventeen are ONE fault plus its rename wake.

   WHAT IT IS: func_lo16_8001816C (LANDED this batch at 102/102, 0
   differing) with a BEST-SO-FAR tracker instead of a first-hit return, and
   a fourth parameter that gates the 0x300 cell test.  Read that body's
   comment first; every shape fact carries.

   THE ONE REMAINING FAULT, exactly:

       +   12       sll a3,a3,0x10              (hoisted to depth 0)
       X   31       sw a3,20(sp)   vs  sra $fp, $v0, 16
       +   42       lw t0,20(sp)
       +   43       nop
       +   44       sra s6,t0,0x10              (sunk to depth 1)

   The `(s16)f' sign extension is ONE PAIR in the target, `sll $v0,$a3,16' /
   `sra $fp,$v0,16' at words 30-31, kept in $fp for the whole body.  In the
   candidate loop.c's move_movables splits the pair -- the `sll' to depth 0
   and the `sra' into the outer loop's body -- and the `a3 << 16'
   intermediate is SPILLED between them.  Three extra words and a nine-value
   register rotation behind them.

   THE ALLOCATOR IS CHOOSING BETWEEN `cb' AND `(s16)f' FOR THE LAST
   CALLEE-SAVED REGISTER, and it picks the wrong one:
     - as written below (no `fl' local) `cb' is SPILLED to 0x10($sp), which
       is what the target does, and `(s16)f' loses: 124 words / 17.
     - hoisting the extension into an explicit `s32 fl = f;' swaps them --
       `cb' takes $s8 and `fl' spills: 122 words / 22.  Closer on length,
       worse on rows, and it is the same fault with the sign flipped.
   Both orderings of the two assignments, and `fl' declared first, last and
   in between, measure 22.  So this is NOT a declaration-order lever.
   `-da' on the real unit shows 23 allocnos with FOURTEEN crossing the one
   call against nine callee-saved registers, and a reload spill of $t0 at
   insn 26 -- so the body is genuinely at the edge and one source statement
   either way decides it.

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

   NEXT AGENT: the whole job is to get `(s16)f' into a callee-saved register
   with `cb' still spilled.  Everything else in this body is at 0.

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

    best = -0x200;
    cb = (s32)D_8008333C[0];
    y = D_800E2970_s[r].unk_02;
    for (n = D_800E2970_s[r].unk_06; n > 0; n--)
    {
        x = D_800E2970_s[r].unk_00;
        for (w = D_800E2970_s[r].unk_04; w > 0; w--)
        {
            c = (UnkDungeonCell *)((x + (y << ((UnkDngLo16Grid *)D_8008333C)->unk_14)) * 6 + cb);
            v = c->unk_04;
            if ((v & 0x8400) == 0)
            {
                if (f == 0 || (v & 0x300) == 0x200)
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
