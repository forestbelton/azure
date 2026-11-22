/* LANDED func_80089588 -- 151 of 151 words, 0 differing (2026-11-22).
   Park kept only until this is merged into csrc/ovl_dungeon.c; the comment
   block to put above the body in csrc/ is the "THE THREE CHANGES" section.
   Was 151 words / 9 aligned rows at the start of this run; before that 72, and
   before that 148/151 and 80.

   ============================== THE THREE CHANGES THAT CLOSED IT (9 -> 1 -> 2 -> 0)

   1. BLOCK 2's STORE INSIDE BOTH ARMS, `y' DELETED.         9 rows -> 13 (152w)
      The park already knew this cure and had priced it at 152w/13 and
      rejected it.  It is right; what was missing was (2).  cse2's extended
      basic block walks AROUND a one-armed `if' (cse.c:8198, skip_blocks)
      and canon_reg then rewrites the second `q[0x3] = 5;' to the first's
      pseudo, so the two 5s become ONE callee-saved pseudo live across the
      AddPrim call -- one word short of the target's two `addiu $v1,$zero,5'.
      Putting the store inside both arms makes the `if' two-armed, cse's
      block ends at the join (cse.c:8166, follow_jumps past the BARRIER),
      and both 5s are materialised.  jump2 cross-jumps the identical `sh'
      tails, so the branch is still the target's three words.

   2. `h' DECLARED INSIDE EACH TAIL BLOCK.                   13 rows -> 1 (152w)
      THIS IS THE ONE THE PARK WAS MISSING, and it is the whole of change 1's
      bad score.  `h' at function scope is assigned in TWO basic blocks, so
      combine_regs refuses it (local-alloc.c:1834 -- a local assigned in more
      than one block has reg_qty == -1 and cannot be tied to its own sum), and
      `h + 0xE' takes a different register from `h'.  The target's
      `addiu $v0, $v0, 0xE' IS that tie.  With `{ s32 h; ... }' in each tail
      block there are two block-local pseudos, each tied to its own sum, and
      both tails come out exactly: h in $v0 and the 5 in $v1, and the
      `addu $a1, $s2, $zero' moves back ahead of the `lhu'.
      All three placements measure the same (whole tail in the scope /
      the 5 left outside / `s32 h = *(u16 *)(q + 0xE);' as an initialiser):
      152w/1 each.  Undoing it on the final body costs 12 rows.
      After 1+2 the ONLY remaining difference is the `andi' of `u8 lean'.

   3. `g' MATERIALISED LATE, BLOCK 0's ACCESSES SPELLED DIRECT.        -> 0
      `u8 lean' bought lean's register with a word (the `andi'); the real
      lever is the SPAN side of QTY_CMP_PRI, not the ref count.  Declaring
      `Unk80083160 *g;' WITHOUT an initialiser, spelling all SEVEN block-0
      `g->unk_00' accesses as `D_80083160.unk_00', and assigning
      `g = &D_80083160;' inside block 1's five-store header moves the
      `addiu $s1, $s1, %lo(D_80083160)' lo_sum three insns later -- to the
      target's 80089634 -- and leaves `s32 lean' (no `andi') free to win $s0.
      Spelling the accesses direct is free on its own (n = 4, 5 and 7 all
      measure 2 rows with `g' initialised at the top), and moving the
      assignment is worth nothing on its own; the pair is one change.
      POSITION: inside the header only.  Measured, with the rest final --
        g assigned before the 1st header store   22 rows
        ... after the 1st store                  22
        ... before the `tmp' statement           22
        ... after the `tmp' statement             2
        ... before `*(s16 *)(q + 0x14) = 0x100;'  2
        ... after it / before or after
            `*(s16 *)(q + 0xC) = 0x100;' /
            before `lean2 = (s16)tmp;'            0  <- any of these four
        ... after `lean2 = (s16)tmp;'             2
        ... just before the block-1 tail AddPrim 23 (and 152 words)

   ============================== WHY THE do-while IS STILL THERE

   `lean' must be allocated BEFORE the `%hi(D_80083160)' pseudo, and both are
   block-0-local and cross calls, so local-alloc hands whichever it ranks
   first the lowest free callee-saved register.  Getting it the wrong way
   round is 21 rows on the final body (every `$s0'/`$s1' in the function).

   The quantity that competes is NOT just `lean': local-alloc ties the xor
   temp into it (lean dies nowhere, but the xor temp dies in the `sltu', and
   combine_regs adds its refs -- local-alloc.c:1939
   `qty_n_refs[sqty] += reg_n_refs[sreg]').  Read off .lreg:
       %hi pseudo      7 refs across 82-84 insns in block 0, crosses 6 calls
       xor temp        2 refs across 2 insns
       lean (s32)      3 refs across 35 insns, crosses 6 calls
   so lean's QUANTITY has 5 refs naturally and loses; it needs 7.
   floor_log2(n)*n is 10 at n=5, 12 at n=6, 14 at n=7, and the %hi quantity
   is 14/H against lean's at 1/L, with 1 < H/L < 1.167 bracketed by
   measurement (6 refs loses, 7 wins) -- so NOTHING below 7 is enough.

   `do { do { tmp = (s16)lean; } while (0); } while (0);' with `lean2 = tmp'
   taken from `tmp' is what supplies them, for zero words:
   flow.c:2122 is `reg_n_refs[regno] += loop_depth;', loop_depth is 3 inside
   two nested do-whiles, so lean reads 1 (set) + 3 (the tmp read) + 1 (the
   `if') = 5, and the quantity reaches 7.  The SPLIT through `tmp' is
   required: the same wrapper on `lean2 = (s16)lean;' directly gives lean 5
   refs too but also gives `lean2' FOUR, and `lean2' then outranks `g' in
   allocno_compare (global.c:587, 8/28 = 0.286 against g's 27/198 = 0.136)
   and takes $s1, pushing `g' to $s3 -- 14 rows at 151 words, or 23 on the
   final body.  `tmp' keeps lean2 at two refs.

   DEPTH 2 IS REQUIRED AND BOTH CASTS MATTER:
     dw^1 on `tmp = (s16)lean;'                  151w / 21   (6 refs, loses)
     dw^2                                        151w /  0   <- taken
     dw^3, dw^4                                  151w /  0   (same stream)
     `tmp = lean;' (no cast, dw^2)               151w / 21   plain copy dies
                                                 in cse, before flow counts
     `lean2 = tmp;' instead of `lean2 = (s16)tmp;'       151w / 0  (equal)
     `u8 tmp;' instead of `s32 tmp;'                     151w / 0  (equal)

   EVERY DEVICE-FREE ROUTE TO THOSE TWO REFS WAS MEASURED AND FAILS:
     * `u8 lean' (the park's own lever) reaches 7 refs via the zero_extend
       temp, but that temp IS the `andi $s0, $s0, 0xff' -- 152w / 1 row on
       the final body.  One word long, and no spelling of the first `if'
       avoids it (the park's own list).
     * `s32 lean' with no device: 151w / 21, at EVERY ONE of the 36
       combinations of the `g' assignment position x the `lean2' assignment
       position in block 1's header.
     * refsweep on that no-device body, 386 insertion points over 7 names:
       no cancelled pair helps.  widthsweep and stmtsweep likewise clean.
       (`lean++; lean--;' cannot work here: cse folds `(lean+1)-1' back
       before flow counts references.)
     * a copy `c = lean; if (c)' inside dw^2 does nothing (151w/23): cse1
       propagates a plain copy into the branch before flow, so the extra
       reference never exists.  With a cast, `c = (s16)lean; ... if (c)'
       compiles three words SHORT (148w / 8..13 for s16/s8/u16/u8).
     * splitting the flag as `d = (u32)D_80083160.unk_00 ^ (u32)D_801C9E40;
       lean = d != 0;' and wrapping either statement, or both, at depth
       1..3: 151w / 21 before change 3 and 70-76 rows after it.  loop.c
       hoists an invariant out of a do-while that sits at the top of the
       function, so a wrapper there buys no references at all -- which is
       also why wrapping `lean = ...' in place measures 151w/76.
     * a single do-while around {`lean2' assignment + the first `if'}
       reaches the right allocation but costs a `j': 152w / 8 (and 152w/27,
       153w/17 for the other depths).  Wrapping the `if' ALWAYS costs that
       word.
     * `s8 lean' 148w/28, `u16 lean' 150w/32, `s16 lean' 148w/28.

   ============================== WHAT THE PARK ALREADY HAD RIGHT

   Change 1 of the previous run -- `lean2 = (s16)lean;' before the first `if'
   and the second block testing a SECOND copy `lean3 = (s16)lean2;' -- is
   still exactly right and is still what buys the target's two reg-reg
   copies, `addu $s3, $s0, $zero' at 80089638 and `addu $v0, $s3, $zero' at
   800896A8.  combine folds a redundant extension of a value whose
   nonzero_bits is 1 into a plain movsi (combine.c:8303) and nothing
   afterwards deletes a movsi between two live pseudos.  Do not "simplify"
   the chain; `lean2 = lean;' coalesces and emits nothing.
   The park's TWO DEVICES (the duplicated-arm `if (q)' and the do-while on
   block 2's header) stayed gone and were never needed again.

   NEW CATALOGUE ENTRIES EARNED HERE (not in docs/matching-levers.md):
     * A LOCAL ASSIGNED IN TWO ARMS CANNOT BE TIED TO ITS OWN SUM applies to
       a local assigned in two SEPARATE TAIL BLOCKS as well, and the fix is
       two inner-scope declarations of the same name.
     * MATERIALISE A `&global' BASE POINTER LATE TO MOVE ITS lo_sum.  The
       `addiu $rX, $rX, %lo(sym)' is scheduled from the C assignment's
       position; spelling the early accesses absolutely is free, and moving
       the assignment moves the lo_sum and with it the %hi quantity's
       QTY_CMP_PRI span.
     * A do-while(0) BUYS REFERENCES ONLY WHERE loop.c CANNOT HOIST THE
       STATEMENT OUT -- at the top of a function it buys none.
     * SPLIT A CAST COPY THROUGH A TEMP so the do-while's reference
       multiplier lands on the source and not on the long-lived destination.
*/
void func_80089588(void)
{
    Unk80083160 *g;
    u32 w[2];
    u8 *q;
    s32 lean;
    s32 tmp;
    s32 lean2;
    s32 lean3;

    lean = D_80083160.unk_00 != D_801C9E40;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    SetDrawArea(q, D_80083160.unk_00);
    AddPrim(D_80083160.unk_00 + 0x7B0, q);
    q += 0xC;
    SetDrawArea(q, D_80083160.unk_00);
    AddPrim(D_80083160.unk_00 + 0x830, q);
    q += 0xC;
    func_80067EF4(q, 0, 0);
    AddPrim(D_80083160.unk_00 + 0x830, q);
    q += 0xC;
    *(u32 *)(q + 0x4) = 0x2A000000;
    *(s16 *)(q + 0x10) = 1;
    *(s16 *)(q + 0x8) = 1;
    do
    {
        do
        {
            tmp = (s16)lean;
        } while (0);
    } while (0);
    *(s16 *)(q + 0x14) = 0x100;
    g = &D_80083160;
    *(s16 *)(q + 0xC) = 0x100;
    lean2 = (s16)tmp;
    if (lean)
    {
        *(s16 *)(q + 0xE) = 0xE0;
    }
    else
    {
        *(s16 *)(q + 0xE) = 0x1C0;
    }
    q[0x3] = 5;
    {
        s32 h;
        h = *(u16 *)(q + 0xE);
        *(s16 *)(q + 0xA) = h;
        *(s16 *)(q + 0x16) = h + 0xE;
        *(s16 *)(q + 0x12) = h + 0xE;
    }
    AddPrim(g->unk_00 + 0x830, q);
    q += 0x18;
    *(u32 *)(q + 0x4) = 0x2A000000;
    *(s16 *)(q + 0x10) = 1;
    *(s16 *)(q + 0x8) = 1;
    *(s16 *)(q + 0x14) = 0x100;
    *(s16 *)(q + 0xC) = 0x100;
    lean3 = (s16)lean2;
    if (lean3 == 0)
    {
        *(s16 *)(q + 0xE) = 0x1C0;
    }
    else
    {
        *(s16 *)(q + 0xE) = 0xE0;
    }
    q[0x3] = 5;
    {
        s32 h;
        h = *(u16 *)(q + 0xE);
        *(s16 *)(q + 0xA) = h;
        *(s16 *)(q + 0x16) = h + 0xE;
        *(s16 *)(q + 0x12) = h + 0xE;
    }
    AddPrim(g->unk_00 + 0x7B0, q);
    q += 0x18;
    func_80067EF4(q, 0, 1);
    AddPrim(g->unk_00 + 0x830, q);
    q += 0xC;
    func_80067F20((u32 *)q, 0, 0, 0x20, 0);
    AddPrim(g->unk_00 + 0x7B0, q);
    q += 0xC;
    func_80067F20((u32 *)q, 0, 0, 0x20, 0);
    AddPrim(g->unk_00 + 0x830, q);
    q += 0xC;
    w[0] = 0x01C00000;
    w[1] = 0x00100100;
    SetDrawArea(q, w);
    AddPrim(g->unk_00 + 0x7B0, q);
    q += 0xC;
    SetDrawArea(q, w);
    AddPrim(g->unk_00 + 0x830, q);
    q += 0xC;
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
}
