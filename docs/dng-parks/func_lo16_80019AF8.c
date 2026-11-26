/* func_lo16_80019AF8 -- ovl_dng_lo16, 774 target words.  THE LARGEST SINGLE
   BODY LEFT IN THIS UNIT.
   PARKED by agent h191, batch 35, 2026-11-25.

   BEST: 738 candidate words, 423 ALIGNED differences
   (aligndiff: 51 X, 71 +, 128 -, 173 R).  ovlverify's masked count is 719 and
   is useless here -- read aligndiff only.

   An earlier agent was assigned this body and was killed by a usage limit
   before writing any C at all, so nothing was inherited: this is a fresh
   reconstruction.  dngtwin finds no carved sibling at ratio >= 0.70 and every
   callee but func_800A6D30/func_800A6DA4 is in this unit, so there is no twin
   and there was none to find.

   WHAT IT IS: the dungeon GENERATOR.  It picks a room grid of
   cols x rows (each independently 2, 3 or 4 from one rand draw), marks
   roughly three quarters of the cells as wanted, tries to place a rectangular
   room in each wanted cell, then runs a 0x400-iteration randomised
   depth-first carve that joins rooms through the four directions, recording
   each join as a byte pair in D_8001F6A8 and counting the joins in
   D_8001F668.  It returns 1 if func_lo16_8001AC4C refuses a corridor and 0
   otherwise.  cols is `*(s32 *)D_8001F660', rows is `D_8001F664' and EVERY
   product in the body is `D_8001F664 * *(s32 *)D_8001F660' -- rows first,
   written INLINE at all eight sites, never through a local: a local keeps the
   product live and the target recomputes the two `lw's and the `mult' after
   every loop exit, which is cse's extended-basic-block boundary.

   THE STRUCTURE IS SETTLED.  Every opcode, immediate, displacement, label and
   delay slot of the 738 words agrees with the target somewhere; 173 of the 423
   rows are register NAMES only and the 128 `-' rows are dominated by ONE
   missing block, described next.  Nothing in the residue is a missing or
   wrong statement.

   THE SINGLE BIGGEST FAULT IS 45 WORDS AND IT IS NOT A SOURCE PROBLEM.

   Words 270..320 and 321..372 of the target are TWO COPIES of the same
   "scatter a decorative marker in this cell" block -- the `else' of the
   `fl[idx] > 0' gate and the `else' of the func_lo16_80017584 gate.  I
   normalised both and diffed them: they are instruction-for-instruction,
   immediate-for-immediate IDENTICAL for 49 of 51 insns, and the ONLY
   difference is that $t1 and $t2 are EXCHANGED at every single reload point:

       copy 1   lui $t1 F660 / lw 0x90($sp)->$t2 / mflo $t1 / lui $t2 F664 /
                lw 0x8C->$t1 / lui $t2 E2970 / lui $t2 333C / lui $t1 EA000
       copy 2   lui $t2 F660 / lw 0x90($sp)->$t1 / mflo $t2 / lui $t1 F664 /
                lw 0x8C->$t2 / lui $t1 E2970 / lui $t1 333C / lui $t2 EA000

   cross-jumping is `jump_optimize (insns, 1, 1, 0)' at toplev.c:3142, which
   runs AFTER reload and sched2 and compares with `rtx_renumbered_equal_p',
   i.e. on HARD registers.  The exchange therefore limits the match to the one
   final `sh $v0, 0x2($v1)' -- which IS merged, at .Llo16_8001A0CC -- and the
   other 50 insns of copy 2 survive.  The exchange exists because reload
   rotates its spill registers (`last_spill_reg') and copy 1 contains exactly
   ONE MORE RELOAD than copy 2: the `lw $t2, 0x94($sp)' that reloads the `pc'
   pointer for copy 1's extra `*pc = 0;'.  One extra reload shifts the phase by
   one and the whole block comes out with the two names swapped.

   MY BUILD HAS ONLY ONE SPILL REGISTER.  Every reload in my common block is
   $t1 ($9) -- there is no rotation, so the two copies come out
   byte-identical, cross_jump merges all 50 insns, and the body is 45 words
   short.  The 51-word block is written twice in my source and that is
   certainly right (nothing else produces two copies: gcc never duplicates a
   block, and jump_optimize's earlier passes run with cross_jump = 0).

   relcand CORROBORATES THIS AND IS THE CHEAPEST WAY TO SEE IT: on my body it
   reports exactly `CALLS the listing makes, yours does not: func_800a6d30,
   func_800a6d30, func_800a6d30' and nothing else, which is the three rand
   draws of the merged-away copy.  No other call, symbol or relocation in the
   774 words is missing, so relcand is clean the moment the merge is broken.

   SO THE WHOLE 45 WORDS HANG ON ONE QUESTION: what makes reload allocate TWO
   spill registers?  `reload()' sizes its spill set from the maximum number of
   reload registers any ONE insn needs.  In the target there is such an insn
   and I found it: 0x8001A4A8 `sh $t2, 0xC($t1)' -- `room->unk_0C = 1' in
   phase 2, where BOTH the address (the spilled `room' pointer at 0xA0($sp))
   AND the stored constant 1 need a reload register.  In my build `room' lives
   in $s2, so that insn needs one reload and the function never needs two.
   THE NEXT AGENT'S JOB IS TO MAKE SOME INSN NEED TWO RELOADS -- most
   naturally by getting phase 2's room pointer spilled, which in the target it
   is because it has only TWO references there (one set, one use).

   I tried the obvious form of that and it is WRONG, measured: giving phase 2
   its own room pointer variable (2 refs) instead of sharing the generation
   phase's `room' is 725 words / 426 rows -- it loses 13 more words.  So the
   spill has to come from pressure, not from reference counting.

   THE OTHER RESIDUE, all of it allocation, with the exact swap in each case:

   1. GENERATION PHASE, 2 values on the wrong side.  The target holds `nr'
      in $s7 and `rw' in $fp and SPILLS `(s32)x' to 0x90($sp).  I hold
      `(s32)x' in $s0 and `room' in $s5 and spill `nr' (0x8C) and `rw'
      (0x98).  That costs 9 words of `lw'/`nop' around `nr++',
      `D_80081468h[3] = nr' and the three `rw' reads, and about 40 rows.
      My ten callee-saved demands are {(s32)x, idx, 4 args, room, rh, 0x40}
      against the target's nine {nr, 0x40, rw, rh, idx, 4 args}: the target
      REUSES the two dead argument registers $s1 and $s0 for `idx * 4' and
      `room' immediately after copying the four arguments into $a0..$a3,
      while my build computes `room' BEFORE the argument copies and so needs
      a tenth register.  sched1 orders those; I could not move it from C.
      Note the arithmetic says the target's choice is the SURPRISING one:
      `(s32)x' has 3 refs at loop depth 3 over ~210 insns and `nr' has 7 refs
      over ~380, so allocno_compare ranks `(s32)x' six times higher and ought
      to keep it.  Do not assume the ranking is the whole story here.
   2. PHASE 2, one wasted register.  My build hoists `%hi(D_8001F668)' out of
      the 0x400 loop into $s7; the target does not hoist it at all (it
      materialises `lui $a0, %hi(D_8001F668)' inside the join block and shares
      it across all four accesses) and spends $s7 on `cnt + idx * 4' instead,
      which my build spills to 0xA0($sp).  Stopping that one loop hoist is
      probably the cheapest thing left in the whole body.
   3. The A-init loop's `y--' is read late in my build (`lhu' + `nop' where
      the target fills the load delay with `idx++'), 4 words.
   4. The `nr < 2' single-room fallback is ~6 words short, all scheduling of
      `g->unk_16' and the shared constant 1.

   FACTS MEASURED HERE (all against the 738 / 423 body unless stated):

   - THE TWO 2/3/4 DIMENSION DRAWS MUST STORE INSIDE EACH ARM.  Written
     `if ((r & 0x7F) == 0) { *(s32 *)D_8001F660 = 2; } else if ...' the three
     arms each end `lui'/`li'/`sw', cross-jumping merges only the `sw', and
     the target's shape falls out exactly -- a `lui' in arm 1, a second `lui'
     in arm 2's `bnez' DELAY SLOT serving arms 2 and 3, one `sw' at the join.
     Hoisting the store out (`i = 2/3/4;' then one `*(s32 *)D_8001F660 = i;')
     is 732 words / 449 rows: 6 WORDS AND 26 ROWS.  This is the cheapest
     measured fact in the body and it generalises to any n-way constant store.
   - `s8 fl[0x10];' and `u8 cnt[0x10][4];' as TWO SEPARATE LOCALS, declared in
     that order, NOT as one struct.  As a single
     `struct { s8 unk_00[0x10]; u8 unk_10[0x10][4]; }' reached through its
     members the body is 736 words / 473 rows (76 +, 118 -, 218 R, 61 X): the
     struct folds phase 2's `&cnt[0]' into `&gen + 0x10' and loses the
     target's separate `addiu $s6, $sp, 0x30' base entirely.  Two locals is
     738 / 423 with 51 X.  BOTH FORMS still give the generation phase's
     `addu $s1, $s1, $t1' + `lw 0x10($s1)' -- i.e. the +0x10 displacement off
     the hoisted `&fl' base -- so that shape does NOT prove a struct, which is
     what I first concluded from it.
   - `UnkDngRoom2970 *base = D_800E2970_s;' assigned once, after the
     `d = rand() & 6' draw and before the 0x400 loop, and used for every
     D_800E2970 access INSIDE that loop and in the following
     func_lo16_8001DC34 sweep -- but NOT in the func_lo16_8001CE44 double loop
     before it and NOT in the final count loop, both of which take the bare
     symbol.  Worth 4 words and 77 rows (742 / 521 -> 738 / 444) and it is
     what brings the FRAME to the target's 0xE0: the pointer is the pseudo
     the target puts in $t0 with a caller-save slot at 0xB0($sp), and without
     it the frame is 0xD8.
   - A SEPARATE POINTER FOR THE MARKER BLOCK'S ROOM.  The two copies of the
     common block must use a DIFFERENT variable (`rc') from the place-a-room
     arm's `room'; sharing one variable makes the allocno cross a call and
     forces it callee-saved.  21 rows (444 -> 423), 0 words.
   - `u16 r' for the two rand draws, not `s32 r': 2 rows (423 -> 425).  Nearly
     free -- the `andi 0xFFFF' combine removes anyway.
   - `u16 d' and `s32 d' with explicit `(u16)d >> 1' / `(s16)d' casts are
     IDENTICAL, 738 / 423 both ways.  So the target's `sll 16'/`sra 16' before
     the func_lo16_8001AC4C call is NOT reachable by the cast; gcc proves the
     range either way.  That missing sign-extension is one of the target's ten
     phase-2 register demands and I could not create it.
   - `D_800E2970_s[idx].unk_0A = 1;' through the bare subscript with
     `room = &D_800E2970_s[idx];' moved AFTER the func_lo16_80017668 call --
     an attempt at the argument-register reuse in item 1 -- is 735 words /
     411 rows but 66 X against 51, so it is worse where it counts.  REJECTED.

   FACTS THAT ARE NOT LEVERS BUT COST HOURS TO ESTABLISH:

   - EVERY 0x8001xxxx ADDRESS IN THIS BODY IS A REAL RELOCATED SYMBOL, not a
     constant address.  D_8001F660, D_8001F664, D_8001F668 and D_8001F6A8 all
     pair `lui 0x8002' with an `addiu'/`%lo' displacement, which is what a
     reloc looks like; gas' `li' macro for a CONST_INT would emit lui/ori.
     The batch brief's blanket "every 0x8001xxxx in this unit is a plain
     CONSTANT" is about the OUT-OF-IMAGE ones and must not be applied to this
     unit's own 0x8001F5B4..0x8001F6E8 data, which 53 landed bodies already
     reach as symbols.
   - D_8001F664 IS ITS OWN SYMBOL and is read with `lw': `extern s32
     D_8001F664'.  D_8001F660 is the unit's existing `extern u8
     D_8001F660[]' reached as `*(s32 *)D_8001F660', the idiom
     func_lo16_8001A710 already uses -- a second extern under the same
     assembler name is two symbols and a second `lui'.
   - THE SECOND HALFWORD AT D_8001F66A IS `(&D_8001F668)[1]'.  splat renders
     it `%lo(D_8001F668 + 0x2)', so it is an addend on D_8001F668 and not a
     symbol; the subscript off `&D_8001F668' gives exactly that.
   - THE FRAME'S THREE DECLARED LOCALS ARE `fl' (0x20), `cnt' (0x30) and
     `dd' (0x70), in that declaration order; everything from 0x78 up is a
     reload spill slot.  0x78 and 0x80 are HImode spills at EIGHT-byte
     alignment, which is reload1.c `alter_reg' passing align = -1 when
     total_size != inherent_size, so do not try to pack them.
   - THE FIRST `fl'/`cnt' INIT LOOP GENUINELY COUNTS DOWN IN THE SOURCE.  It
     has a down-counter in the 0x80($sp) slot (the same `s16 y' the generation
     loops use) and a separate up-counter `i' for the two subscripts.
     check_dbra_loop cannot be the cause: `i' is used in both addresses, so
     the comparison biv is not dead, and a reversed loop would run the
     addresses DOWN.  Written `for (y = rows * cols - 1; y >= 0; y--)' with
     `i++' in the body it reproduces the target's entry guard, both bivs and
     the giv.
   - THE 0x40-DIVISION CONSTANT IS A HOISTED LOOP INVARIANT, `0x40 / cols'
     and `0x40 / rows' written inline at all six sites off a `0x40' that
     loop.c puts in a callee-saved register ($s6 in the target).
   - THE SEARCH LOOPS' GATES ARE NOT SYMMETRIC, which is easy to get backwards
     off the listing: the FIRST search breaks on
     `fl[idx] > 0 && base[idx].unk_0A != 0' and the SECOND (the
     all-four-directions-failed fallback) breaks on
     `base[j].unk_0C != 0 && fl[j] > 0'.
   - func_lo16_8001A768 TAKES SEVEN ARGUMENTS and func_lo16_8001AC4C FIVE;
     both are still INCLUDE_ASM in this unit so the declarations below are new
     and nothing had to be reconciled.  No alias was needed anywhere in this
     body and no already-landed prototype was touched.
   - jtcheck: this function owns NO jump table, so ovl_dng_lo16's missing
     rodata support does not apply to it.

   ONE SHARED TYPEDEF FIELD CHANGE IS REQUIRED, layout-identical:
   UnkDngRoom2970's `u8 pad_08[0x2]' must become `s16 unk_08' (the room
   record's 0x8 halfword takes `rand() & 0x60').  Nothing else in the unit
   reads pad_08, and I re-verified ALL 53 LANDED BODIES in ovl_dng_lo16 with
   the change in place: every one is still 0 differing.

   DECLARATIONS
   extern s32 D_8001F664;
   void func_lo16_8001A710(void);
   void func_lo16_8001CE44(s16 *, s16);
   s16 func_lo16_8001A768(s16 *, s16, s16, s8 *, u8 *, s32, s16);
   s16 func_lo16_8001AC4C(s16, s16, s16, s32, s8 *);
   void func_lo16_8001DC34(s16 *);
   void func_lo16_8001DE2C(void);
   END DECLARATIONS
*/

s32 func_lo16_80019AF8(void)
{
    s8 fl[0x10];
    u8 cnt[0x10][4];
    s16 dd;
    UnkDngLo16Grid *g;
    UnkDngRoom2970 *room;
    UnkDngRoom2970 *base;
    UnkDngRoom2970 *rc;
    UnkDngRoom2970 *q;
    s8 *pc;
    u8 *qq;
    u8 *cc;
    u8 *cn;
    s16 x;
    s16 y;
    s16 cx;
    s16 cy;
    s32 d;
    s32 lim;
    s32 nr;
    s32 i;
    s32 j;
    s32 idx;
    s32 nxt;
    s32 rx;
    s32 ry;
    s32 rw;
    s32 rh;
    u16 r;

    nr = 0;
    lim = 0x400;
    g = (UnkDngLo16Grid *)D_8008333C;
    r = func_800A6D30();
    if ((r & 0x7F) == 0)
    {
        *(s32 *)D_8001F660 = 2;
    }
    else if ((r & 0x1F) == 0)
    {
        *(s32 *)D_8001F660 = 3;
    }
    else
    {
        *(s32 *)D_8001F660 = 4;
    }
    r = func_800A6D30();
    if ((r & 0x7F) == 0)
    {
        D_8001F664 = 2;
    }
    else if ((r & 0x1F) == 0)
    {
        D_8001F664 = 3;
    }
    else
    {
        D_8001F664 = 4;
    }
    for (i = 0x20; i > 0; i--)
    {
        qq = &D_8001F6A8[i * 2];
        qq[1] = 0;
        qq[0] = 0;
    }
    D_8001F668 = 0;
    if ((s16)func_800A6DA4(0, 0x200) < 2)
    {
        nr = 1;
    }
    else
    {
        i = 0;
        for (y = D_8001F664 * *(s32 *)D_8001F660 - 1; y >= 0; y--)
        {
            fl[i] = (func_800A6D30() & 3) != 0;
            *(s32 *)cnt[i] = 0;
            i++;
        }
        for (y = 0; y < D_8001F664; y++)
        {
            for (x = 0; x < *(s32 *)D_8001F660; x++)
            {
                idx = y * *(s32 *)D_8001F660 + x;
                pc = &fl[idx];
                if (*pc > 0)
                {
                    rx = func_800A6DA4(0, (u16)(0x40 / *(s32 *)D_8001F660 / 2 - 1)) +
                         x * (0x40 / *(s32 *)D_8001F660);
                    ry = func_800A6DA4(0, (u16)(0x40 / D_8001F664 / 2 - 1)) + y * (0x40 / D_8001F664);
                    rw = func_800A6DA4(0, (u16)(0x40 / *(s32 *)D_8001F660 / 2 - 1));
                    rh = func_800A6DA4(0, (u16)(0x40 / D_8001F664 / 2 - 1)) + 5;
                    if ((s16)rh < 5)
                    {
                        rh = 5;
                    }
                    if (func_lo16_80017584(rx - 2, ry - 2, rw + 9, rh + 4) == 0)
                    {
                        room = &D_800E2970_s[idx];
                        room->unk_0A = 1;
                        func_lo16_80017668(rx - 2, ry - 2, rw + 9, rh + 4, (u16)(idx + 1));
                        room->unk_00 = rx + 1;
                        room->unk_02 = ry + 1;
                        room->unk_04 = rw + 3;
                        room->unk_06 = rh - 2;
                        nr++;
                        room->unk_08 = func_800A6D30() & 0x60;
                        *pc = 2;
                        *(s32 *)cnt[idx] += 0x1010101;
                    }
                    else
                    {
                        *pc = 0;
                        cx = x * (0x40 / *(s32 *)D_8001F660) + (func_800A6D30() & 7) + 6;
                        cy = y * (0x40 / D_8001F664) + (func_800A6D30() & 7) + 6;
                        rc = &D_800E2970_s[idx];
                        rc->unk_00 = cx;
                        rc->unk_02 = cy;
                        D_800EA000_s[(cy << g->unk_14) + cx].unk_02 = func_800A6D30() & 0x60;
                    }
                }
                else
                {
                    cx = x * (0x40 / *(s32 *)D_8001F660) + (func_800A6D30() & 7) + 6;
                    cy = y * (0x40 / D_8001F664) + (func_800A6D30() & 7) + 6;
                    rc = &D_800E2970_s[idx];
                    rc->unk_00 = cx;
                    rc->unk_02 = cy;
                    D_800EA000_s[(cy << g->unk_14) + cx].unk_02 = func_800A6D30() & 0x60;
                }
            }
        }
    }
    func_lo16_80017560();
    D_80081468h[3] = nr;
    if ((s16)nr < 2)
    {
        D_80081468h[3] = 1;
        q = D_800E2970_s;
        q->unk_00 = 2;
        q->unk_02 = 2;
        q->unk_0A = 1;
        q->unk_04 = (1 << g->unk_14) - 4;
        q->unk_06 = (1 << g->unk_16) - 4;
        q->unk_08 = func_800A6D30() & 0x60;
        func_lo16_8001CE44((s16 *)q, 0);
    }
    else
    {
        for (y = 0; y < D_8001F664; y++)
        {
            for (x = 0; x < *(s32 *)D_8001F660; x++)
            {
                idx = y * *(s32 *)D_8001F660 + x;
                q = &D_800E2970_s[idx];
                if (q->unk_0A != 0)
                {
                    func_lo16_8001CE44((s16 *)q, idx);
                }
            }
        }
        func_lo16_80019A84(0x4000);
        idx = (func_800A6D30() & 0xFFFF) % (D_8001F664 * *(s32 *)D_8001F660);
        d = func_800A6D30() & 6;
        base = D_800E2970_s;
        do
        {
            for (i = 0; i < D_8001F664 * *(s32 *)D_8001F660; i++)
            {
                if (fl[idx] > 0 && base[idx].unk_0A != 0)
                {
                    break;
                }
                idx = (idx + 1) % (D_8001F664 * *(s32 *)D_8001F660);
            }
            if (i >= D_8001F664 * *(s32 *)D_8001F660)
            {
                break;
            }
            y = idx / *(s32 *)D_8001F660;
            x = idx % *(s32 *)D_8001F660;
            nxt = idx;
            room = &base[idx];
            i = 4;
            do
            {
                cc = &cnt[idx][(u16)d >> 1];
                if (*cc != 0)
                {
                    dd = d;
                    func_lo16_8001A710();
                    nxt = func_lo16_8001A768(&dd, x, y, fl, cnt[0], 1, idx);
                    if (nxt >= 0)
                    {
                        if (func_lo16_8001AC4C((s16)d, dd, idx, nxt, fl) != 0)
                        {
                            return 1;
                        }
                        cn = &cnt[nxt][((dd + 4) & 7) >> 1];
                        *cc = *cc - 1;
                        *cn = *cn - 1;
                        room->unk_0C = 1;
                        base[nxt].unk_0C = 1;
                        D_8001F6A8[D_8001F668 * 2] = idx;
                        D_8001F6A8[D_8001F668 * 2 + 1] = nxt;
                        D_8001F668 = D_8001F668 + 1;
                        break;
                    }
                }
                i--;
                d = (d + 2) & 6;
            } while (i > 0);
            if (i > 0)
            {
                d = (dd + 2) & 7;
                fl[idx] = fl[idx] - 1;
                if (fl[idx] <= 0)
                {
                    fl[idx] = -1;
                }
            }
            else
            {
                j = (idx + 1) % (D_8001F664 * *(s32 *)D_8001F660);
                for (i = 0; i < D_8001F664 * *(s32 *)D_8001F660; i++)
                {
                    if (base[j].unk_0C != 0 && fl[j] > 0)
                    {
                        break;
                    }
                    j = (j + 1) % (D_8001F664 * *(s32 *)D_8001F660);
                }
                nxt = j;
            }
            idx = nxt;
            lim--;
        } while (lim >= 0);
        for (i = D_8001F664 * *(s32 *)D_8001F660 - 1; i >= 0; i--)
        {
            q = &base[i];
            if (q->unk_0C == 0 && q->unk_0A != 0)
            {
                func_lo16_8001DC34((s16 *)q);
            }
        }
        (&D_8001F668)[1] = 0;
        for (i = 0; i < D_8001F664 * *(s32 *)D_8001F660; i++)
        {
            q = &D_800E2970_s[i];
            if (q->unk_0C == 0 && q->unk_0A != 0)
            {
                (&D_8001F668)[1] = (&D_8001F668)[1] + 1;
            }
        }
    }
    func_lo16_800176EC();
    func_lo16_80017530();
    func_lo16_8001DE2C();
    return 0;
}
