/* PARK -- func_b31_801745B4, ovl_dng_b31, 143 words.
   143 words built against 143 target words, 4 ALIGNED DIFFERENCES.
   dngfam: a one-member family, so this is one body, not a representative.

   WHAT THE FOUR ROWS ARE.  Every opcode and every register is right.  The
   only residue is the POSITION of one `lui/addiu %hi/%lo(D_80073414)' pair:
   the target emits it at 801745D8/801745DC, interleaved with the prologue
   saves between `addiu $s3, $zero, 0xF' and `sw $ra, 0x30($sp)'; this body
   emits it five insns later, after all five remaining `sw's.  aligndiff
   counts that as two target-only rows and two built-only rows.

   WHY, AND WHAT IT COSTS TO REMOVE.  The `do { } while (0)' around the head
   is a REFERENCE BUMP on `base' and is what gets `base' into $s5 and `tb'
   into $s6 (the target's assignment).  Without it the body is still 143
   words but `base' and `tb' hold each other's register and the residue is 7
   rows.  The bump is also what moves the `lui': loop.c treats the `do'-loop
   as a real loop, hoists the loop-invariant address into the fake loop's
   PREHEADER, and the preheader is a basic block of its own after the
   prologue -- so sched1 can no longer interleave the pair with the saves.
   So the two halves of the residue are in tension and no single change
   reaches 0 from here.  That is the brief's "A FIX CAN BE WORSE ALONE AND
   NECESSARY IN COMBINATION" shape with the signs reversed: what is needed is
   a DIFFERENT WAY TO GIVE `base' A FOURTH REFERENCE, one that is not a loop.

   THE ALLOCATION ARITHMETIC, read off `tools/cdkgcc -da' rather than swept.
   global.c ranks on floor_log2(n_refs) * n_refs / live_length.  Without any
   device the .lreg dump for this body gives

       82  tb    4 refs / 144 insns   2*4/144 = 0.0556
       84  base  3 refs /  71 insns   1*3/71  = 0.0423
       86  r     4 refs /  70 insns   2*4/70  = 0.1143
       88  i     5 refs /  87 insns   2*5/87  = 0.1149
       85  ok    5 refs /  88 insns   2*5/88  = 0.1136

   and the allocation order 88, 86, 85, 82, 84 puts i in $s2, r in $s3, ok in
   $s4, tb in $s5, base in $s6.  The target wants r, i, ok, base, tb.  So TWO
   pairs are inverted and BOTH margins are tiny: i beats r by 0.0006 (1149
   against 1143 after global.c's x10000), and tb beats base because
   144/71 = 2.03 where base needs the ratio above 8/3 = 2.667.
   -- r needs a FIFTH reference (2*5/70 = 0.1429 clears i); that is the
      `do { } while (0)' around the accept block, and it must enclose the
      `if ((u8)r == 4)' gate as well, because wrapping only the four stores
      leaves the `addiu $v0, $zero, 0x4' outside their block and sched1 then
      emits it after the three `sb's instead of before them (2 extra rows).
   -- base needs a FOURTH reference (2*4/71 = 0.1127 clears tb at 0.0556).
      Only the head`s `do { } while (0)' achieved that, at the cost above.
   NEITHER a declaration reorder nor a statement reorder moves either pair:
   the body is FLAT under both axes (all 143 words / 17 rows), which is why
   the numbers above were read rather than swept.

   WHAT IS ALREADY SETTLED, and is worth more than the residue:
   1. THE LOOP MUST BE `again:' + `goto', NOT a `for'.  As a `for` loop the
      constant 1 of the `e->unk_02 == 1' test is hoisted into a seventh
      callee-saved register (`li s7,1') and the body is 146 words / 47 rows.
      A goto back edge emits no NOTE_INSN_LOOP_BEG, so loop.c hoists nothing
      out of the body -- which is also why the three copies of the class
      expression stay inside their arms.
   2. THE HEAD MUST NAME `D_80073414' AND `tb' MUST BE ASSIGNED AFTER IT.
      `tb = D_80073414; base = *(u16 *)(tb[p[1]]...)' is 142 words / 17 rows:
      one word short, because the target`s `addu $s6, $a1, $zero' only exists
      when cse finds the symbol already in a pseudo and makes `tb' a COPY of
      it.  This is the single most load-bearing line in the body.
   3. THE ACCEPT TEST READS A COPY OF THE LATCH, NOT THE LATCH.  `v = ok;' at
      the end of EACH arm is 143 words; `if (ok == 0)' straight off the latch
      is 142 / 40, and one `v = ok;' after the if/else is also 142 / 40
      because regmove coalesces it.  The target`s three
      `addu $v0, $s4, $zero' insns are ONE source copy at .Lb31_8017474C
      that reorg then duplicated into the two branch delay slots.
   4. The `ok' latch is declared and zeroed OUTSIDE the loop and is never
      reset, so a draw that fails its own class test still accepts once an
      earlier draw has set it.  That is what the listing says; it reads like
      a bug in the original.
   5. Arm A is `base == CLASS', not `CLASS == base' -- the target`s
      `bne $s5, $v0' has the latch first (1 row).  Arm B is
      `CLASS <= base' (`slt $v0, $s5, $v0'), the other way round.
   6. The descriptor pointer is `(Unk80073414 *)((u8 *)0 + (u8)r * 20 +
      (s32)tb)', which puts the INDEX as the `addu's first operand as the
      target has (`addu $s0, $v0, $s6'); plain `&tb[(u8)r]' puts `tb' first
      (1 row).  Same lever as func_b31f_8016EB14's +0x49 table byte.
   7. The stride on D_80073414[k].unk_0C is 20 HERE, where
      func_lo16_8001D0F4 has 12 on the same table.
   8. `(q[0x25] + 0xF) / 16' -- the `addiu 0xF' / `bgez' / `addiu 0x1E' /
      `sra 4' sequence at 80174790 is the signed-division correction, so the
      dividend is a plain `int', not an unsigned.

   MEASURED AND REJECTED, every figure from this body:
   | spelling                                            | words | aligned |
   | as written below                                    |  143  |    4    |
   | no head `do { } while (0)' (base/tb exchanged)      |  143  |    7    |
   | the loop as `for (i = 0xF; i >= 0; i--)'            |  146  |   47    |
   | `tb' assigned before the head expression            |  142  |   17    |
   | `if (ok == 0)' with no `v' copy                     |  142  |   40    |
   | one `v = ok;' after the if/else                     |  142  |   40    |
   | `ok = cond ? 1 : ok' in each arm                    |  141  |   40    |
   | `v = cond ? (ok = 1) : ok' in each arm              |  143  |   25    |
   | the `ok == 0' test duplicated into each arm         |  141  |   43    |
   | accept-block wrapper around the four stores only    |  143  |    6    |
   | no accept-block wrapper at all                      |  143  |   17    |
   | `do { } while (0)' around both arm gates            |  143  |   21    |
   | `do { } while (0)' around arm A's gate only         |  143  |   17    |
   | `do { } while (0)' around arm B's gate only         |  143  |   17    |
   | `do { } while (0)' around the whole `i' gate        |  143  |   24    |
   | `do { } while (0)' around `tb' only                 |  143  |   11    |
   | `do { } while (0)' around `base' only               |  144  |    8    |
   | head wrapper moved ahead of `ok = 0; i = 0xF;'      |  143  |   12    |
   | head wrapper after `ok = 0', before `i = 0xF'       |  143  |    8    |
   | `ok = 0; i = 0xF;' inside the head wrapper          |  143  |   14    |
   | `for (;;) { ...; break; }' in place of the wrapper  |  143  |    4    |
   | `base = <expr>; base &= 0x3000;' (combine folds it) |  143  |   17    |
   | `i = 0xF' moved after `tb = D_80073414'             |  143  |   33    |
   | `tb' before `base' inside the wrapper               |  143  |   16    |
   | `v = base;' after the head (dead, no bump)          |  143  |    7    |
   | the class into a local `c' in each arm              |  143  |    9    |
   | `base = 0;' dead store before the real assignment   |  143  |    9    |
   | `base' as `u16'                                     |  143  |   17    |
   | arm A as `CLASS == base'                            |  143  |    5    |
   | `&tb[(u8)r]' for the descriptor pointer             |  143  |    5    |
   | three separate `goto next' rejects, not one `||'    |  143  |   17    |
   | `r & 0xFF' in place of `(u8)r'                      |  143  |   17    |
   | `i > 0xB' in place of `i >= 0xC'                    |  143  |   17    |
   | declaration order: base before tb, and/or i before r|  143  |   17    |
   MEASURED AND FREE, so NOT the reason it is at 4: `for (;;) { break; }' in
   place of the head `do { } while (0)' (143/4, identical); `base' as `u16'
   is NOT free and is in the table above.

   THE NEXT THING TO TRY is a fourth reference on `base' that is not a loop.
   Everything in this project's device vocabulary that bumps a reference
   count goes through flow.c:2122's loop_depth, and that is exactly what
   creates the preheader.  So the route is probably a SOURCE spelling in
   which `base' genuinely appears three times in the arms rather than twice
   -- or a reading of the two class comparisons that I have not found.

DECLARATIONS
s32 func_b31_801745B4(u8 *, u8 *);
s32 func_800A6DA4(u32, u32);
END DECLARATIONS

   Those two are the only declarations this body needs that ovl_dng_b31 did
   not already have.  `s32 func_b31_801745B4(u8 *, u8 *);' is in the unit
   already -- func_b31_80174D98 calls it -- so only func_800A6DA4 has to be
   added back when this lands.  Unk80073414 and D_80073414 come from
   include/structs.h and need nothing.
   No jump table and no rodata of its own -- the listing opens at `glabel'. */
s32 func_b31_801745B4(u8 *p, u8 *q)
{
    Unk80073414 *tb;
    Unk80073414 *e;
    s32 base;
    s32 ok;
    s32 r;
    s32 r2;
    s32 i;
    s32 v;

    ok = 0;
    i = 0xF;
    do
    {
        base = *(u16 *)(D_80073414[p[1]].unk_0C + p[0] * 20) & 0x3000;
        tb = D_80073414;
    } while (0);
again:
    r = func_800A6DA4(1, 0x13);
    e = (Unk80073414 *)((u8 *)0 + (u8)r * 20 + (s32)tb);
    if (e->unk_02 == 1 || (u8)r == 0x12 || (u8)r == 0xE)
    {
        goto next;
    }
    r2 = func_800A6DA4(1, (u16)(e->unk_02 - 1));
    if ((*(u16 *)(e->unk_0C + (u8)r2 * 20) & 0x10) != 0)
    {
        goto next;
    }
    if (i >= 0xC)
    {
        if (base == (*(u16 *)(tb[p[1]].unk_0C + p[0] * 20) & 0x3000))
        {
            ok = 1;
        }
        v = ok;
    }
    else
    {
        if ((*(u16 *)(tb[p[1]].unk_0C + p[0] * 20) & 0x3000) <= base)
        {
            ok = 1;
        }
        v = ok;
    }
    if (v == 0)
    {
        goto next;
    }
    do
    {
        p[1] = r;
        p[0] = r2;
        p[2] = 0;
        p[3] = 0;
        if ((u8)r == 4)
        {
            p[3] = 0x80;
            if ((u8)r2 == 0x11)
            {
                p[2] = 1;
            }
            else
            {
                p[2] = (q[0x25] + 0xF) / 16;
            }
        }
    } while (0);
    return 1;
next:
    if (--i >= 0)
    {
        goto again;
    }
    return 0;
}
