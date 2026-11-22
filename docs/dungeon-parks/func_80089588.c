/* PARK func_80089588 -- 151 of 151 words, 9 aligned differences (2026-11-21).
   WAS 151 words / 72 rows at the start of this run; before that 148/151 and 80.

   ============================== WHAT MOVED IT, AND WHY  (72 -> 28 -> 9)

   THE WHOLE RESIDUE WAS ONE LOCAL-ALLOC ORDERING, AND THE PARK'S TWO DEVICES
   WERE PAYING FOR THE SYMPTOM.  Both devices are GONE from the body below:
   the duplicated-arm `if (q) SetDrawArea(..) else SetDrawArea(..)' and the
   `do { } while (0);' around the second POLY_F4 header.  They are not merely
   unnecessary now, the first of them was ACTIVELY WRONG -- see "ablation".

   Three changes, each measured on the body that preceded it:

   1. `lean2 = (s16)lean;' before the first `if', and the second block testing
      `lean2' instead of `lean'.                      72 rows -> 28 (149 words)

      The target's `addu $s3, $s0, $zero' at 80089638 is a REG-REG COPY that
      survives because combine folds a redundant extension of a value whose
      nonzero_bits is 1 into a plain movsi, and nothing afterwards deletes a
      movsi between two pseudos that are both live (docs/matching-levers.md,
      "combine can manufacture a surviving copy from a redundant sign
      extension", combine.c:8303).  In the .combine dump it is literally
          (insn 118 (set (reg/v:SI 86) (subreg:SI (reg/v:QI 83) 0)))
      `lean2 = lean;' is NOT the same lever: the plain assignment coalesces
      into lean's own quantity and emits nothing (or, on a u8 lean, emits the
      `andi' in place) -- 149 words / 8 rows, two words short.
      MEASURED ON THE 72-ROW BODY: (s16) 70, (s8) 70, (u8) 72, (u16) 72,
      plain 72 -- i.e. only the SIGNED narrow casts make a copy, and only
      with the `if (q)' device REMOVED does the copy pay (28 instead of 70).

   2. The SECOND copy, `lean3 = (s16)lean2;' immediately before the second
      block's `if', with that `if' testing `lean3'.   28 rows -> 29 (150 words)

      Worth minus one row on its own -- it buys the target's
      `addu $v0, $s3, $zero' at 800896A8 (2 rows) and costs 3 by pushing `y'
      out of $v0 in the second block.  It is kept because it is a word the
      target has and because change 3 pays for it.  `lean3 = lean2;' (no
      cast) is byte-identical to no copy at all.  POSITION MATTERS: before
      the five-store header (`pre_blk2') the copy lands in $v1 and scores
      150w/29 with the copy in the WRONG PLACE; immediately before the `if'
      it lands in $v0 at 800896A8 exactly as the target does.

   3. `u8 lean' instead of `s32 lean'.             29 rows -> 9 (151 words)

      THIS IS THE WHOLE FUNCTION.  See the mechanism below.

   ============================== THE MECHANISM: A STEP FUNCTION IN local-alloc

   `lean', `%hi(D_80083160)', `q' and `lean2' are the four callee-saved
   quantities.  The target's assignment is lean=$s0, g=$s1, q=$s2, lean2=$s3;
   every "register name only" row in the old 72-row residue came from getting
   the first two the wrong way round.

   `lean' and the `%hi(D_80083160)' pseudo are BOTH LOCAL TO BLOCK 0 (the
   function's first basic block runs all the way to the `beqz' at 80089640,
   because the first CODE_LABEL is .L80089650), and both cross calls, so
   LOCAL-ALLOC assigns them, in order of QTY_CMP_PRI, each taking the lowest
   free callee-saved register.  global_alloc then gets $s2/$s3 for q/lean2
   whichever way round the first two went -- which is why this is a two-way
   switch worth nineteen rows and nothing in between.

   local-alloc.c:1693 --
       #define QTY_CMP_PRI(q)  ((int) (((double) (floor_log2 (qty_n_refs[q])
            * qty_n_refs[q] * qty_size[q]) / (qty_death[q] - qty_birth[q]))
            * 10000))
   and qty_n_refs is FLOW's count, not local-alloc's own:
       local-alloc.c:297   qty_n_refs[qty]  = reg_n_refs[regno];
       local-alloc.c:1939  qty_n_refs[sqty] += reg_n_refs[sreg];

   `floor_log2(n) * n' is a STEP FUNCTION.  Measured on this function, with
   the %hi pseudo fixed at `7 times across 82 insns' (floor_log2(7)*7 = 14):

       lean 3 refs across 35 insns  (3*1 =  3)  -> %hi first, 28/29 rows
       lean 4 refs across 35 insns  (4*2 =  8)  -> %hi first, 29 rows
       lean 5 refs across 35 insns  (5*2 = 10)  -> lean first, 12 rows

   so the crossing on THIS function is between four and five references, and
   nothing below five buys anything at all.  (Note the ratio is NOT
   reg_live_length's: 8/35 > 14/82 arithmetically, yet 4 refs loses, because
   QTY_CMP_PRI divides by local-alloc's own qty_death-qty_birth span and not
   by the `across N insns' that .lreg prints.  Use .lreg for n_refs and treat
   the length as an unknown you bracket by measurement.)

   `u8 lean' is what supplies the two extra references for free-ish.  A u8
   local tested with `if (lean)' needs an SImode value for MIPS's branch, so
   gcc emits
       (insn 120 (set (reg:SI 107) (zero_extend:SI (reg/v:QI 83))))
   and that insn's set+use take the quantity from 3 references to 5.  Combine
   will NOT fold it away (it folds the extension in the `lean2' copy, which
   has the only other use, but the compare's operand must be a real SImode
   register), so it costs exactly one word: the `andi $s0, $s0, 0xff' that is
   this park's remaining extra instruction.

   ============================== RESIDUE -- 9 ROWS, TWO CLUSTERS

   +   45            andi s0,s0,0xff            (candidate only)
   R   55   54 80089660  li s0,5       vs  addiu $v1, $zero, 0x5
   R   56   55 80089664  sb s0,3(s2)   vs  sb $v1, 0x3($s2)
   R   75   74 800896B0  li v1,224     vs  addiu $v0, $zero, 0xE0
   R   76   75 800896B4  li v1,448     vs  addiu $v0, $zero, 0x1C0
   R   77   76 800896B8  sh v1,14(s2)  vs  sh $v0, 0xE($s2)
   -        77 800896BC                vs  addu $a1, $s2, $zero
   X   79   79 800896C4  move a1,s2    vs  addiu $v1, $zero, 0x5
   R   80   80 800896C8  sb s0,3(s2)   vs  sb $v1, 0x3($s2)

   CLUSTER A (1 row): the `andi'.  The price of `u8 lean' above.  No spelling
   of the first `if' avoids it -- `lean != 0', `(s8)lean', `lean == 1',
   `lean & 1' all measured 9 or worse, and `if (lean2)' (which does avoid it)
   drops lean to two references and loses the allocation again, 149w/10.

   CLUSTER B (8 rows): THE TWO `q[0x3] = 5;' ARE COMMONED BY cse2 INTO ONE
   CALLEE-SAVED PSEUDO.  The target materialises `addiu $v1, $zero, 0x5' once
   per block (80089660 and 800896C4); the candidate emits one `li' and keeps
   the 5 in $s0 across the AddPrim call (reusing lean's register after lean
   dies), which is one word short and pushes `y' out of $v0 in the second
   block.

   WHY, read off cse.c and confirmed in the dumps: in the .cse dump the two
   are still two pseudos (108 and 121); in .cse2 insn 243 already stores
   reg 108, and flow then deletes the dead `(set (reg:QI 121) (const_int 5))'.
   `cse_end_of_basic_block' (cse.c:8056) walks
       while (p && GET_CODE (p) != CODE_LABEL)        cse.c:8087
   and continues PAST a conditional jump's label in two cases:
       cse.c:8166  follow_jumps, when a BARRIER precedes the target
                   -- the two-armed shape, which ENDS the block at the join;
       cse.c:8198  skip_blocks "branch around a block", when the insn before
                   the target is not a CODE_LABEL -- the ONE-ARMED shape,
                   which does NOT end the block.
   The first block's `if' is two-armed (the store is inside both arms), so
   cse's block ends at its join label -- that is exactly the 65-row lever
   already in docs/matching-levers.md, and it is why 0x2A000000/1/0x100 are
   rematerialised in the second block.  The second block's `if' is one-armed
   (jump.c:789 fires), so cse walks AROUND it and both `q[0x3] = 5;' land in
   the same extended block, where canon_reg (cse.c:2583) rewrites the second
   to the first's qty_first_reg.

   AND THE CURE IS KNOWN AND MEASURED, IT JUST COSTS MORE THAN IT PAYS:
   putting the second block's store inside both arms
       if (lean3 == 0) { *(s16 *)(q + 0xE) = 0x1C0; }
       else            { *(s16 *)(q + 0xE) = 0xE0; }
   keeps the `if' two-armed through cse2, ends the block at its join, and the
   target's SECOND `addiu $v1, $zero, 0x5' appears -- confirmed in the diff.
   jump2 cross-jumps the identical `sh' tails so the emitted branch is still
   the target's three words.  But it measures 152 words / 13 rows: the two
   now-block-local 5s outrank `h' for $v0 in BOTH blocks and the `sh 0xA'
   swaps with the `addiu 0xE'.  Sweeping the tail did not recover it:
     tail `h += 0xE;' (in-place rmw)              152w / 15
     tail mixed (one rmw, one expression)         152w / 13
     `q[0x3] = 5;' before/after the `lhu', x4     152w / 13 in every case
   So the next agent's job on this function is: with blk2's store inside both
   arms, get `h' allocated $v0 ahead of the 5 in each tail block.

   ============================== DEVICE ABLATION (the POWER SET)

   Run first, on the 72-row body, over all 8 subsets of {dup-arm `if (q)',
   do-while on block 1's header, do-while on block 2's header}:
       q=0 dw1=0 dw2=0  148w / 80      q=1 dw1=0 dw2=0  151w / 75
       q=0 dw1=0 dw2=1  148w / 77      q=1 dw1=0 dw2=1  151w / 72  <- the park
       q=0 dw1=1 dw2=0  148w / 81      q=1 dw1=1 dw2=0  151w / 75
       q=0 dw1=1 dw2=1  148w / 78      q=1 dw1=1 dw2=1  151w / 72
   -- so on the PARK's body the ablation is a clean negative and both devices
   look load-bearing.  THAT IS THE TRAP.  The `if (q)' device puts a CODE_LABEL
   at 0x44, which ends basic block 0 and makes `lean' a GLOBAL allocno; the
   entire local-alloc mechanism above is unreachable while it is there.  With
   change 1 applied the same ablation reads
       q=1 (device kept)   151w / 70
       q=0 (device gone)   149w / 28
   The do-while on block 2's header was worth 5 rows at 28 and is worth
   NOTHING at 9 (measured both ways, 9 and 9), so it is gone too.
   FINAL BODY HAS ZERO DEVICES.

   ============================== BLOCK-ORDER CHECK

   Done against the listing's addresses: every branch and every call in the
   candidate sits at the target's ordinal, and the drift at each branch equals
   the drift at its destination.  The only non-register rows are the ones
   listed in the residue above.  Block order is NOT the defect here.

   ============================== MEASURED AND REJECTED IN THIS RUN
   (rows / words, and the body each was measured on)

   On the 72-row park body:
     * spelling the first N `g->unk_00' accesses as `D_80083160.unk_00' and
       the rest through `g', for every N from 0 to 15: N<=4 byte-identical
       (72), N=5 73, N=6..8 84, N=9 85, N>=10 93-94.  The target's switch
       from `lw %lo(D_80083160)($s1)' to `lw 0x0($s1)' after the fifth access
       is NOT a source split -- it is cse's %hi anchor dying at the lo_sum,
       and the park's `Unk80083160 *g' spelling already produces it.
     * lean2 = (u8)lean / (u16)lean: 72 (nothing).  lean2 = lean: 72.
     * lean2 placed before block 2's header or before block 2's `if':
       72/73 -- it must be before the FIRST `if'.

   On the 28-row body (s32 lean, lean2 only):
     * refsweep  (491 insertion points, 6 names): no cancelled pair helps.
       `lean++; lean--;' specifically is a no-op here at five positions --
       cse folds `(lean+1)-1' back to lean BEFORE flow counts references, so
       it buys nothing on a plain integer local in straight-line code.
     * devsweep  (413 candidates/round): no wrapper helps.
     * stmtsweep (2 movable assignments): no safe move helps.
     * widthsweep: `s8 y' 32/151, `u16 lean' 32/150, `u8 lean' 8/150  <- taken.
     * a duplicated-arm `if (v) { s } else { s }' around each of six
       statements between the two `q[0x3] = 5;', for v in q/lean2/lean3/h/y
       (30 candidates): best 36, worst 89.  All worse.

   On the 9-row body (final):
     * refsweep, devsweep, stmtsweep, widthsweep: ALL FOUR CLEAN.
     * declaration order: all 5040 permutations of the seven locals after `g'
       -- 9 rows in every single one.
     * MEM_IN_STRUCT_P: `((s16 *)q)[K]' instead of `*(s16 *)(q + K)' for the
       five-store header, the 0xE stores and the h tail, each group alone,
       each pair, all three, and each applied to block 1 only / block 2 only /
       both (63 candidates): 9 rows in every single one.  `*(u8 *)(q + 3) = 5'
       instead of `q[0x3] = 5': 9.
     * two-step and renormalising spellings of the flag --
       `lean = (s32)g->unk_00; lean = lean != (s32)D_801C9E40;' 151w/30,
       `!!(...)' 9, `(...) ? 1 : 0' 9, `lean = lean != 0;' 9,
       `(g->unk_00 - D_801C9E40) != 0' 9, operands swapped 9.
     * duplicated-arm wrappers at seven positions x six conditions: best 12.
     * `q[0x3] = 5;' moved before the `lhu' in either or both blocks, x3 tail
       shapes: 9 everywhere.

   THE DEVICE-FREE ALTERNATIVE TO `u8 lean', AND ITS COST.  `u8' is an honest
   declaration for a flag, but it is doing a register-allocator's job here and
   it costs the `andi'.  The device version that avoids the `andi' is
   `s32 lean' with `do { } while (0);' around BOTH the `lean2 = (s16)lean;'
   statement and the first `if' -- the wrappers double those two references
   (flow.c:2122 `reg_n_refs[regno] += loop_depth;') and reach the same five.
   It measures 150 words / 12 rows, two of which are the `addu $s3' landing in
   the `beqz' delay slot instead of two insns earlier; moving the wrapped
   assignment between the fourth and fifth header store gives 150w/10.  EITHER
   WRAPPER ALONE IS WORTHLESS (29 and 31 against a 29-row base) -- this is a
   power-set find, not a single-device one.  I kept `u8 lean' because it is
   one row better, word-exact, and not a device.

   NOT TRIED: anything that would make `%hi(D_80083160)' cheaper rather than
   `lean' dearer.  Its seven references are the five `lw %lo' loads, the `lui'
   and the `addiu' lo_sum, and all seven are in the target. */
void func_80089588(void)
{
    Unk80083160 *g = &D_80083160;
    u32 w[2];
    u8 *q;
    u8 lean;
    s32 y;
    s32 h;
    s32 lean2;
    s32 lean3;

    lean = g->unk_00 != D_801C9E40;
    q = *(u8 **)(g->unk_00 + 0x8D0);
    SetDrawArea(q, g->unk_00);
    AddPrim(g->unk_00 + 0x7B0, q);
    q += 0xC;
    SetDrawArea(q, g->unk_00);
    AddPrim(g->unk_00 + 0x830, q);
    q += 0xC;
    func_80067EF4(q, 0, 0);
    AddPrim(g->unk_00 + 0x830, q);
    q += 0xC;
    *(u32 *)(q + 0x4) = 0x2A000000;
    *(s16 *)(q + 0x10) = 1;
    *(s16 *)(q + 0x8) = 1;
    *(s16 *)(q + 0x14) = 0x100;
    *(s16 *)(q + 0xC) = 0x100;
    lean2 = (s16)lean;
    if (lean)
    {
        *(s16 *)(q + 0xE) = 0xE0;
    }
    else
    {
        *(s16 *)(q + 0xE) = 0x1C0;
    }
    q[0x3] = 5;
    h = *(u16 *)(q + 0xE);
    *(s16 *)(q + 0xA) = h;
    *(s16 *)(q + 0x16) = h + 0xE;
    *(s16 *)(q + 0x12) = h + 0xE;
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
        y = 0x1C0;
    }
    else
    {
        y = 0xE0;
    }
    *(s16 *)(q + 0xE) = y;
    q[0x3] = 5;
    h = *(u16 *)(q + 0xE);
    *(s16 *)(q + 0xA) = h;
    *(s16 *)(q + 0x16) = h + 0xE;
    *(s16 *)(q + 0x12) = h + 0xE;
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
