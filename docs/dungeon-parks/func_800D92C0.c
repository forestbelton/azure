/* func_800D92C0 (ovl_dungeon_800D50FC) -- 344 of 344 words, 5 ALIGNED DIFFERENCES.

   ====================== 2026-11-21 (later): 48 -> 44 -> 34 -> 31 -> 30 -> 17 -> 12 -> 5

   THREE CLUSTERS at 48 rows: 4 at words 42-46 (the `|= 0x80000000' gate),
   23 at words 77-109 (the 0x20000 arm's two table lookups), 21 at words
   228-246 (the loop's two table lookups).  At 5 the gate and the 0x20000 arm
   are both CLOSED and all five remaining rows are in the loop block.

   (1) THE `|= 0x80000000' GATE IS NOT UNREACHABLE -- 4 rows, 48 -> 44.
   The old header below cites docs/matching-levers.md:27186 as "PROVED
   UNREACHABLE".  That entry was REWRITTEN on 2026-11-16 ("A LOCALITY
   PROBLEM, NOT UNREACHABLE -- SOLVED") and its cure applies verbatim: split
   the `&&', assign the constant to a local IN THE BLOCK THAT HOLDS THE
   `>= 0' TEST, and give it a second reference.  Measured on the 48-row body:
   do-while around the store 44; `__asm__("" ::"r"(cc2));' after the store 44;
   do-while around `cc2 = 0x80000000;' 44; duplicated-arm `if (d)' around the
   store 44; the constant moved up with NO second reference 48 (unchanged).

   (2) THE LOOP'S TABLE INDEX MUST NOT SHARE `k' WITH THE 0x20000 ARM'S --
   10 rows, 44 -> 34.  One `k' served both `k = ((d[0x45] + ...) & 7) * 2' in
   the 0x20000 arm and `k = (*(u16 *)(d + 0x2A) >> 8) & 0xE' in the loop.  Two
   blocks referencing one pseudo makes it GLOBAL (flow.c:2561), local_alloc
   skips it, and the whole lhu/srl/andi chain in the loop block then misses
   the register the target gives it.  A separate `kk' at function scope, used
   only in the loop, is the fix; width does not matter (s32/u32/s16/u16 all 34).

   CONFIRMED IN THE `-dg' DUMP, not inferred.  With `k' shared, the `.greg'
   header reads `;; 27 regs to allocate: 96 113 92 310 311 334 335 95 ...'
   and 95 IS the index pseudo (`(insn 740 ... (set (reg/v:SI 95)
   (and:SI (reg:SI 319) (const_int 14))))').  With `kk' split out the header
   reads `;; 25 regs to allocate: 96 114 92 296 297 320 321 94 ...' and the
   index pseudo (98) is NOT in it -- local_alloc kept it, two global allocnos
   fewer.  None of 308-313 (the two table-lookup chains) is in either list, so
   the whole loop-block residue is a LOCAL allocation decision; `allocno_compare'
   and global.c do not come into it, and neither does a cancelled pair, which
   is why refsweep is clean on every body this park has had.

   DERIVED, NOT SWEPT.  In the loop block local-alloc builds two tied
   quantities: P = {high, lo_sum, addu} for D_8006CCD8's address and
   Q = {the c[0x24] load, the sum}.  QTY_CMP_PRI (local-alloc.c:1693) is
   floor_log2(n_refs)*n_refs*size/(death-birth)*10000, so P = 2*6/7 = 17142
   and Q = 2*4/5 = 16000: P is allocated first, takes $v0, and Q takes $v1 --
   the target has them the other way round.  The $v0 choice then propagates
   into sched2: an address pseudo in $v0 has an OUTPUT DEPENDENCE on the call
   that sets $v0, so `lui %hi(D_8006CCD8)' gets sched2 priority 2 instead of 1
   and cannot float to the top of the block; and the store `sb $v0, 0x24($s3)'
   lands in the same priority group as the second table's address insns, where
   schedule_select's potential_hazard rule promotes the STORE because it uses
   the memory unit (`;; insn 753 has a greater potential hazard' in the sched2
   dump).  All 21 rows of that block are downstream of one register choice.

   CAVEAT ON THE ARITHMETIC: the mechanism (QTY_CMP_PRI deciding which of two
   quantities takes $v0, and the sched2 consequences of that choice) is solid
   and is what the measurements track.  The exact numbers above assume
   combine_regs ties {high, lo_sum, addu} and {load, sum} into one quantity
   each; that was not confirmed against a dump, and a hand model built on
   per-pseudo quantities predicts the OPPOSITE assignment for the last two
   registers.  Note also that qty_n_refs is reg_n_refs (local-alloc.c:297),
   which flow.c:2128 has already WEIGHTED BY LOOP DEPTH -- every pseudo in
   this block reads "used 4 times" in dump_flow_info for two references,
   because the block is inside the do-while.  Anyone finishing this should
   read the quantities out of a dump rather than trust the figures here.

   (3) THE SUMS ARE WRITTEN TABLE-FIRST -- 4 rows, 34 -> 30.
     `c[0x24] = *(u8 *)(kk + (s32)D_8006CCD8) + c[0x24];'   34 -> 31
     `x = *(u16 *)(k + (s32)D_8006CCD8) + t80[0x24];'       31 -> 30
   The operand order decides which input block_alloc's combine_regs ties the
   sum's destination to, hence which quantity is the short one.  Measured:
   on the 34-row body -- both c[0x24] and c[0x25] table-first 34, only c[0x25]
   36, only c[0x24] 31.  On the 31-row body -- both x and y 30, only y 32,
   only x 30; a second `t81 = D_80082E80;' for the y read 39/345;
   `x = t80[0x24]; x += ...;' split 35/345; both reads split from both adds
   33/345; `t80' dropped and D_80082E80[0x24] inline 33; a `kb = k;' copy 31;
   `(s32)' casts on both sums 31.

   (4) THE PARK'S OWN `if (c)' DUPLICATED ARM AROUND THE `k =' LINE IS NOW
   WORTH -13 ROWS -- 30 -> 17, by DELETING it.  The header below records it as
   the device that took 62 to 56 and calls it "also the missing word".  That
   was true of the body it was measured on.  With `kk' split out (2) the arm
   is what still makes `k' multi-block, and once `k' is block-local the whole
   0x20000 arm re-allocates.  This is the stale-negative rule in its strongest
   form: the device that paid most in this park is the one to delete first.

   (5) `y++; y--;' IS NOW WORTH -5 ROWS -- 17 -> 12, by DELETING it.  Same
   story: recorded below as worth one row on the 52-row body.  With the arm
   gone, the four extra references to `y' cost the register the target gives
   it.  Re-measured at that same position on the FINAL 5-row body:
     `y++; y--;'  11      `f++; f--;'  7
     `x++; x--;'   5      `t80++; t80--;'  5   `k++; k--;'  5   `m++; m--;'  5
   so nothing wants a cancelled pair there any more.

   THE TWO DEVICES THAT STILL PAY, re-measured on the 12-row body:
     `if (n) {r = f(...);} else {r = f(...);}' deleted            15 (+3)
     `do { return; } while (0);' unwrapped                        16 (+4)
     both deleted                                                 19 (+7)
   so both stay, and the park's note about WHICH `return;' matters stands.

   WHAT WAS LEFT AT 12 ROWS -- the diagnosis that led to (6):

   * words 80-95 (7 rows), the 0x20000 arm.  The target materialises
     D_8006CCD8's address at word 80 and spends the `lw %lo(D_800814A8)'
     delay on `lbu 0x45($s2)' / `lbu 0x24($a1)' / `lbu 0x25($a1)', reading
     BOTH t80 bytes into x's and y's own registers ($a2 and $a1) before the
     index multiply.  The candidate emits `addiu $a0' there and reads the two
     t80 bytes at words 94/95, after the second table address.
   * words 228-235 (5 rows), the loop.  The same P-vs-Q inversion as (2), one
     step short: D_8006CCD8's address is still $v0 where the target has $v1,
     and the c[0x24] load still $v1 where the target has $v0.  P = 17142
     against Q = 26666 does not decide it because P and Q are DISJOINT
     ([15,22) and [23,26) in block_alloc's half-open units), so both can take
     $v0 and the quantity that conflicts with neither still wins it.  P's
     priced live length has to grow past 8 half-units, or the c[0x24] load has
     to be allocated first; nothing measured here does either.

   (6) AND THEN `x' GOES BACK TO BEING WRITTEN t80-FIRST -- 7 rows, 12 -> 5,
   which CLOSES the 0x20000 arm entirely.  Change (3) above is reversed for
   `x' once (4) and (5) are in: `x = t80[0x24] + *(u16 *)(k + (s32)D_8006CCD8);'
   is the honest shape and the matching one.  Measured on the 12-row body:
     `x = t80[0x24] + ...' (t80 first)                       5   <- the body
     `x = *(u16 *)(...) + t80[0x24]' (table first)           12
     `t80 = ...' moved below the `k =' line                  12
     `y' line before the `x' line                            14
     `y' written table-first as well                         18
     the two t80 reads hoisted into `xa'/`ya' s32 locals     11
     `x = t80[0x24]; y = t80[0x25];' then two `+=' lines     23
     both reads moved AFTER the two table reads              35 / 345
   The c[0x24] line stays TABLE-first (change 3); reverting it is 8.

   (7) THE `__asm__("" : "+r"(i));' AFTER THE func_800BCB04 CALL IS NO LONGER
   NEEDED, and this park now carries NO asm.  The old header calls that line
   "the single most valuable line in the function"; it still is -- without a
   second set for `i' there the body is 95 rows -- but a DUPLICATED ARM around
   the call buys the same second set in ordinary C:

       if (d)
       {
           i = func_800BCB04(...);
       }
       else
       {
           i = func_800BCB04(...);
       }

   Measured on the 5-row body:
     duplicated arm, `if (d)' / `if (c)' / `if (p)' / `if (f)'     5 (all four)
     duplicated arm, `if (w)' / `if (g2)' / `if (g)' / `if (tbl)'  19/16/17/17
     `__asm__("" : "+r"(i));'   (the old device)                   5
     `__asm__("" : "=r"(i) : "0"(i));'                             5
     `__asm__("" ::"r"(i));'    (an input -- and a barrier)        8
     NOTHING there                                                95
     `i++; i--;' / `i += 0;' / `i = (s16)i;' / `m = i; i = m;'    95 (all four)
     `do { i = func_800BCB04(...); } while (0);'                  95
   So the cancelled pair is NOT a substitute for a second set here, and the
   do-while is not either: only a real second SET of `i' is.

   (8) `if ((s16)r > 0)' IS BYTE-IDENTICAL TO `if ((r << 16) > 0)' -- both 5.
   The cast form is in the body because it says what it means.

   DEVICES STILL IN THE BODY, each re-measured on the 5-row body by deleting it:
     `cc2' one block up + `do { ... |= cc2; } while (0);'   delete -> 9
       (`__asm__("" ::"r"(cc2));' in place of the do-while is also 5)
     `if (n) {r = f(...);} else {r = f(...);}'              delete -> 8
     `do { return; } while (0);' around ONE return          delete -> 9
     the duplicated arm around func_800BCB04 (7)            delete -> 95

   WHAT IS LEFT -- 5 rows, words 228/229/231/234/235, all in the loop block.
   D_8006CCD8's address is $v0 where the target has $v1, and the c[0x24] load
   is $v1 where the target has $v0; the instruction ORDER is already the
   target's.  It is the P-vs-Q quantity inversion described in (2), one step
   short: P = {high, lo_sum, addu} and Q = {the loaded byte, the sum} are
   DISJOINT in block_alloc's half-open [birth, death) units, so both can take
   $v0 and the one sorted first by QTY_CMP_PRI wins it.

   THE ARITHMETIC, WORKED THROUGH ON THE FINAL BODY'S OWN sched1 ORDER (block
   37 of the `-ds' dump; local_alloc sees sched1's order, and qty_n_refs is
   flow's loop-weighted reg_n_refs, so every pseudo here counts 4 for two
   textual references).  Let p be the position of the table load.
     TABLE-FIRST (the body below): the sum ties to the table byte.
       P = {high, lo_sum, addu}   refs 12, len 5   -> 3*12/5 = 72000
       T = {table byte, sum}      refs  8, len 5   -> 3* 8/5 = 48000
       V = {c[0x24] load}         refs  4, len 3   -> 2* 4/3 = 26666
       P and T are disjoint, so P takes $v0, T takes $v0, V is left $v1.
     VALUE-FIRST (`c[0x24] += ...', 8 rows): the sum ties to the c[0x24] load.
       P  refs 12, len 5  -> 72000
       R = {table byte}    refs 4, len 1 -> 2*4/1 = 80000
       V' = {c[0x24] load, sum}  refs 8, len 5 -> 48000
       R takes $v0, P takes $v0 (disjoint), V' is left $v1.
   The target wants V' = $v0 with P and the table byte BOTH in $v1, which
   needs value-first AND V' sorted first -- i.e. R's priced length at 2 or
   more AND P's past 7.  Both are sched1 PLACEMENT facts (a gap either side of
   the table load), not source-expression facts, which is why thirty-odd
   spellings of these three statements all land on 5 or 8 and never between.

   MEASURED AND REJECTED on the 5-row body (the loop's lookups):
     `c[0x24] += ...' instead of table-first                       8
     `c[0x25]' written table-first too                            8 (and both: 10)
     `c[0x24] = c[0x24] + ...' written out                         8
     `*(u8 *)(c + 0x24) = ... + *(u8 *)(c + 0x24);'                5 (identical)
     `kk++; kk--;' before the c[0x24] line / after it             15 / 5
     `kk = *(u16 *)(d + 0x2A); kk = (kk >> 8) & 0xE;' split       14
     do-while around the c[0x24] / c[0x25] / `kk =' line       15 / 14 / 15 (345)
     a `q' temp for the table byte                                12

   MEASURED AND REJECTED on the 44-row body (cluster C, the loop's lookups):
     `*(u8 *)(c + 0x24) += ...' instead of `c[0x24] += ...'        44
     a `q' temp for the table byte                                 47 / 345
     do-while around the c[0x24] line / the c[0x25] line / `k ='    49 / 50 / 55
     `k++; k--;' before or after the c[0x24] line                   44
     `t2 = (u8 *)D_8006CCE8;' local for the second table            44
     `c[0x24] = c[0x24] + ...' written out                          44
     duplicated arm `if (c)' around either line                     58 / 63
     `(s32)D_8006CCD8 + k' (base first)                             44 (identical)
     a `u8 *t1' local for the first table, assigned in the SAME
       block, in the block above, or before the loop                52-56 / 345-347
     a `u8 *t1' + do-while or `__asm__("" ::"r"(t1))'               53-55 / 345-347
     both lookups written inline without an index local             45 / 347
     `kk = *(u16 *)(d + 0x2A); kk = (kk >> 8) & 0xE;' split         43
   The `t1' family is the interesting negative: a pointer local for a `-G0'
   global costs two to three words here, which is the opposite of what the
   pointer-local hoist does elsewhere in this unit.

   WHICH LOCALS MUST AND MUST NOT BE SHARED -- the (2) question asked of every
   other name, on the 5-row body:
     `i' split into a loop counter and a separate `i2' for the
        func_800BCB04 result                        95 (s16 i2) / 101 (s32 i2, 342)
     `v' split into `v' and `v2' for the second `*(s32 *)(d + 0x1C)' read   5
     `t80[0x24]'/`t80[0x25]' used in place of the two later INLINE
        `D_80082E80[0x24]'/`[0x25]' reads       21/343, 22/342, 26/341
   So `i' MUST stay one name -- the loop counter and the func_800BCB04 result
   are the same pseudo in the target, and the duplicated arm of (7) only works
   because of it -- `v' is free either way, and `t80' must stay block-local
   with the later reads spelled inline.  Declaration POSITION of `kk'/`cc2' in
   the block does not matter: seven positions, all 5.

   ALSO MEASURED AND REJECTED on the 5-row body (all in the loop block):
     `kk = (*(u16 *)(d + 0x2A) & 0xE00) >> 8;'                     7
     `kk = ((*(u16 *)(d + 0x2A) >> 9) & 7) * 2;'                   5 (identical)
     `kk = *(u16 *)(d + 0x2A) >> 8;  kk &= 0xE;'                   5 (identical)
     `*(u8 *)(c + 0x24) = ... + *(u8 *)(c + 0x24);'                5 (identical)
     `*(u8 *)(c + 0x25) += ...'                                    5 (identical)
     a dead `kk = 0;' or `kk = c[0x24];' after the pair            5 (deleted)
     `((u8 *)D_8006CCD8)[kk]' / `((u8 *)D_8006CCE8)[kk]'          5 (identical)
     `*(u8 *)((s32)D_8006CCD8 + kk)' (base first)                 5 (identical)
     a `c2' local for the c[0x24] read, three spellings         8-11
     the func_8009A3D0 ternary written as a full if/else       18 / 347
     a duplicated arm around either lookup                     38-41 / 353-356
     do-while around {both lookups} / {kk + both} / {both + the
       func_8009A21C call} / {kk + both + the call}        17/74/73/65, 344-346
   The do-while numbers are worth keeping: flow.c:2128 weights a reference by
   loop_depth and `reg_n_refs' is what QTY_CMP_PRI reads, so an extra loop
   level around these statements SHOULD raise the c[0x24] quantity -- and it
   is catastrophic every time, because the loop NOTEs are also a full
   scheduling barrier in the one block whose schedule is already right.

   THE FOUR SWEEPS ON THE 5-ROW BODY (the one below):
     widthsweep  -- no width change helps
     stmtsweep   -- 7 movable assignments, no safe move helps
     devsweep    -- 3 devices already in the body re-spelled, 59 statements,
                    22 names, 1357 candidates a round: no wrapper helps
     refsweep    -- 22 names, 2865 candidate insertion points:
                    no cancelled pair helps
   Earlier: all four were clean on the 44-row body as well, and widthsweep
   found `s32 x' -> `u8 x' worth 4 rows on the intermediate 30-row body.  That
   is NOT adopted and should not be: `x' is a tile coordinate sum the target
   narrows with `sll 16 / sra 16', so `u8' is semantically wrong, and the body
   has moved three changes past the measurement anyway.

   NEXT STEP: this is permuter food.  One block, five rows, the instruction
   order already correct, and the only question is which of two local
   quantities block_alloc hands $v0.  Nothing in the C that was measured here
   moves it, and the four sweeps are clean on this body.

   ====== EVERYTHING BELOW IS THE PARK HEADER AS IT STOOD AT 48 ROWS ======
   ====================================== 2026-11-21 (later): 48 rows -> 44

   THE `|= 0x80000000` GATE IS NOT UNREACHABLE.  The header below says "FOUR of
   the 62 are the PROVED-UNREACHABLE `|= 0x80000000` gate
   (docs/matching-levers.md:27186)".  That lever entry was REWRITTEN on
   2026-11-16 -- its new title is "THE `|= 0x80000000` GATE IS A LOCALITY
   PROBLEM, NOT UNREACHABLE -- SOLVED 2026-11-16" -- and the cure applies here
   verbatim.  Split the `&&`, assign the constant to a local IN THE BLOCK THAT
   HOLDS THE `>= 0` TEST (one block above the store), and give it a second
   reference:

       if ((v & 0x400) != 0)
       {
           cc2 = 0x80000000;
           if (*(s32 *)(d + 0x14) >= 0)
           {
               do { *(s32 *)(d + 0x14) |= cc2; } while (0);

   Worth exactly the 4 rows at words 42/44/45/46 (the target's
   `lw $v0` / `lui $v1` / `or $v0, $v0, $v1`; the candidate had the value in
   $v1 and the constant in $v0).  Measured on the 48-row body:
     do-while around the store                             44   <- the body below
     `__asm__("" ::"r"(cc2));` after the store              44
     do-while around `cc2 = 0x80000000;`                    44
     duplicated-arm `if (d)` around the store               44
     cc2 one block up with NO second reference              48 (unchanged)

   ====== EVERYTHING BELOW IS THE PARK HEADER AS IT STOOD AT 48 ROWS ======
   ====================================== 2026-11-21: 51 rows -> 48

   A duplicated-arm `if (n)` around `r = func_8009A66C((s16)s, c, d...)`, from
   tools/decomp/devsweep.py, on top of this morning's cancelled pair.  Three
   rows, and the third device this park carries.

   ====================================== 2026-11-21: 52 rows -> 51

   `y++; y--;` before `if (c[0x24] == (u16)x && c[0x25] == (u16)y)`, from
   tools/decomp/refsweep.py.  One row.

   ============================== 2026-11-20: 62 rows at 343/344 words -> 56 at 344/344

   The body below carries one device: a duplicated-arm `if (c)` around the `k = ((d[0x45] + ...` line -- which is also the missing word.

   A `do { } while (0);` wrapper and a duplicated-arm `if (v) { s } else { s }`
   are the same lever from two directions -- flow.c:2128 weights each
   reference by `loop_depth`, and jump2's cross-jumping merges identical arms
   -- so both cost nothing and both are ordinary C.  SWEPT, NOT REASONED:
   every statement position times both devices over every local, on a
   `tools/decomp/onefunc.py` file where a candidate is 0.09 s.  Nothing in
   this park's diagnosis points at the winning statement, and that is the
   normal case rather than the exception.
   Agent j49, 2026-11-16.  Written cold; the 2026-11-15 candidate that reached
   15 was lost with /tmp and nothing of it survived but the three
   docs/matching-levers.md entries, all three of which are applied below.

   SIGNATURE: void func_800D92C0(u8 *p, void *b, u8 *c, u8 *d);  `b` is dead.
   The one landed caller (csrc/ovl_dungeon.c:41344, func_800D8C64) rides an
   IMPLICIT declaration today, so landing this needs
       void func_800D92C0(u8 *, void *, u8 *, u8 *);
   in the declaration block.  func_800D8C64 re-measures at 250/250, 0
   differing with it.  func_800A0E6C is left on its implicit declaration on
   purpose -- csrc/ovl_dungeon.c:33865 already calls it that way.

   FOUR of the 62 are the PROVED-UNREACHABLE `|= 0x80000000` gate
   (docs/matching-levers.md:27186), at words 42/44/45/46.  Nothing was spent
   on them.

   THE DEVICE FROM docs/matching-levers.md:27214 IS IN THE BODY AND IS WORTH
   79 ALIGNED DIFFERENCES HERE, not the 57 that entry records: without
   `__asm__("" : "+r"(i));` after the func_800BCB04 assignment the function is
   344 words / 142 differing, with EVERY callee-saved register one lower than
   the target's ($s1 holds `d` where the target holds `g`/`i`, $s2 holds `c`
   where the target holds `d`, and so on down).  With it the whole register
   assignment -- $s0 w/s, $s1 g then i, $s2 d, $s3 c, $s4 tbl, $s5 p, $s6 f --
   is the target's.  This is the single most valuable line in the function and
   it is the first thing to restore if the body is ever re-derived.

   THE ONE MISSING WORD IS A `nop` THE TARGET KEEPS AND THE CANDIDATE FILLS.
   At 0x800D9458..0x800D9460 the target spends the `bne`'s delay slot on
   `andi $v0, $a1, 0xFFFF` (the second comparison's 16-bit window) and then
   cannot fill the `lbu $v1, 0x25($s3)` load delay, so word 104 is a `nop`;
   reorg here steals `sll $a2, $a2, 16` -- the (s16) conversion that belongs to
   the func_800A0818 call BELOW the `beq` -- into the delay slot instead and
   fills word 104 with the `andi`.  Measured and rejected, all 343 words /
   62 differing and byte-identical to each other:
     the compare nested (`if (c[0x24]==x) { if (c[0x25]==y) {...} }`)
     the two (s16) conversions hoisted into `s16 x2`/`s16 y2` locals
     `t80` dropped and `D_80082E80[0x24]` written inline at both reads
     `t80 = D_80082E80;` moved AFTER the `k =` statement
     `u8 *t1 = (u8 *)D_8006CCD8;` added as a third local before `k`
     `x = t80[0x24]; y = t80[0x25];` split from the two table adds   345 / 64

   ONE SPELLING SCORES 344 / 61 AND IS WRONG, so it is NOT the body here.
   The loop's second test is `i == 0`, not `r == 0`: at 0x800D95D4 the
   `blez`'s delay slot holds `sll $v0, $s1, 16`, which CLOBBERS $v0 before
   .L800D96C4's `bnez $v0` -- so that `bnez` can only be reading `i << 16`,
   and reorg placed the `sll` there because it is the first insn of BOTH
   threads.  Writing the test as `(r << 16) == 0` (with `s32 r` holding
   func_8009A66C's result) makes the candidate keep its own `sll` AND a `nop`
   in the `blez` slot, which pays for the missing word above: 344 / 61.  It
   tests the wrong value.  `(i << 16) == 0` and `i == 0` are byte-identical
   to each other at 343 / 62.

   WHAT GOT IT FROM 184 TO 62 BESIDES THE DEVICE, in the order it paid:

   * THE LOOP'S `*(s16 *)(d + 0x2A)` IS READ ONCE INTO AN `s32` LOCAL before
     the `p + 0x98` test, not inside both arms: read inline it is duplicated
     into each arm (2 words) and comes out `lhu` where the target has `lh`.
     Worth 6 differences and the `lh`/`lhu` pair.
   * THE TWO ROW STORES ARE BASE-FIRST: `*(d + (d[0x71] & 0x7F) + 0x74)`, not
     `d[(d[0x71] & 0x7F) + 0x74]`.  The subscript form is `addu $v0, $v0, $s2`
     where the target has `addu $v0, $s2, $v0` -- 2 differences, and it is
     func_800D9820's rule (csrc/ovl_dungeon.c, "The row address is
     BASE-FIRST") applied to a different row pair.
   * ALL FOUR D_8006CCD8/D_8006CCE8 LOOKUPS ARE INTEGER ARITHMETIC WITH THE
     INDEX FIRST: `*(u16 *)(k + (s32)D_8006CCD8)`.  `(u8 *)D_8006CCD8 + k`
     gives the transposed `addu`.
   * `if (i >= 8) {...} else {...}`, NOT the other way round: the target's
     `bnez $v0, .L800D975C` puts the i<8 arm at the branch target and the
     i>=8 arm on the fall-through, and swapping the arms is 37 differences.
   * func_8009FD7C's and func_8009A66C's results are tested through an
     explicit `(s16)` cast; without it on the func_8009A66C inside the
     0x20000 arm the `sll $v0, $v0, 16` at 0x800D94AC disappears.

   THE TWO BLOCKS THAT STILL CARRY MOST OF THE RESIDUE are both pure sched1
   placement of a symbol's `lui`/`addiu`, and no source spelling moved either:

   * 0x800D93F4..0x800D9474 (14 rows).  The target materialises BOTH
     D_80082E80 and D_8006CCD8 before the D_800814A8 load; the candidate
     materialises D_80082E80 there and D_8006CCD8 after the index multiply.
     Six placements of the base locals were measured (see the rejected list
     above) and every one is byte-identical.
   * 0x800D9650..0x800D9698 (12 rows).  The target materialises D_8006CCD8
     before `lhu 0x2A($s2)` and D_8006CCE8 after the first `sb`; the
     candidate materialises each at its use.  A `u8 *` local for the first
     table ADDS three words (347 / 67), which is the opposite of what the
     pointer-local hoist does elsewhere in this unit.

   TRIAGE: one word short, in one identified block, with the structure either
   side of it proved -- permuter food by docs/dungeon-sweep.md's rule.    ====================================== 2026-11-21: 56 rows -> 52 at 344/344

   A `do { return; } while (0);' around ONE of this function's eight `return;'
   statements -- the one inside

       if (i == 0 && *(u16 *)(D_80082E80 + 0x24) != *(u16 *)(c + 0x24))
           if ((s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)

   WHICH return MATTERS.  All eight were measured: seven of them are 57 rows
   (one row WORSE than the park) or 63, and this one is 52.  A device on the
   wrong statement here costs a row; there is no "harmless place to try one".

   The permuter proposed this together with `*(&D_800E2970_s[j].unk_0C) & 2'
   -- an address-of/dereference round trip -- and that half is REJECTED: on
   its own it is 57 rows, and on top of the do-while it turns 52 into 53. The
   permuter's score could not tell the two apart because it scores the pair.
   Measuring the halves separately is the only way this came out clean.

   ====================================== 2026-11-21: PERMUTER SPENT AT 5 ROWS

   150 minutes, 4 jobs, 314251 candidates, base score 50 -> best 50,
   `found=none'.  Not one candidate in 314k improved even the permuter's OWN
   metric, let alone the row count; the output-* dirs measured by
   measure_finds are an older run's and are 45-59 rows at 345-346 words.

   That is the THIRD clean permuter negative at the floor today
   (func_800B6AC4 at 11 rows, func_8009E798 at 1, this at 5).  All four sweeps
   were already clean here too, so both automatic routes are spent and the
   last 5 rows need hand analysis -- the agent's own theory is that the target
   wants value-first with the table byte's priced length >= 2 and the pointer
   chain's > 7, which are sched1 PLACEMENT facts rather than source-expression
   facts, and that is why ~40 spellings all land on 5 or 8 and never between.
 */
void func_800D92C0(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *g2;
    u8 *w;
    s16 *tbl;
    s16 f;
    s16 i;
    s16 m;
    s32 s;
    s32 n;
    u8 *t80;
    s32 j;
    s32 k;
    s32 r;
    s32 v;
    s32 kk;
    s32 cc2;
    s32 x;
    s32 y;

    g = (u8 *)&D_80083460;
    f = 0;
    if ((*(u16 *)(g + 0x2) & 0x4000) != 0 || *(s8 *)(d + 0x71) >= 0)
    {
        func_800A9A0C(d);
        return;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
    {
        return;
    }
    w = p + 0x9C;
    func_800A19E4((s32)c, d, 3, 6, w);
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x410) != 0)
    {
        if ((v & 0x400) != 0)
        {
            cc2 = 0x80000000;
            if (*(s32 *)(d + 0x14) >= 0)
            {
                do
                {
                    *(s32 *)(d + 0x14) |= cc2;
                } while (0);
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + ((func_800A6D30() & 7) << 9);
            }
        }
    }
    else
    {
        if (*(s8 *)(d + 0x71) >= 0)
        {
            return;
        }
        if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
        {
            return;
        }
        func_800A19E4((s32)c, d, 3, 6, w);
        v = *(s32 *)(d + 0x1C);
        if ((v & 0x2000) != 0)
        {
            if ((v & 0x20000) != 0)
            {
                t80 = D_80082E80;
                k = ((d[0x45] + ((s16)(*(u16 *)(D_800814A8 + 0x2A)) >> 9)) & 7) * 2;
                x = t80[0x24] + *(u16 *)(k + (s32)D_8006CCD8);
                y = t80[0x25] + *(u16 *)(k + (s32)D_8006CCE8);
                if (c[0x24] == (u16)x && c[0x25] == (u16)y)
                {
                    d[0x71] &= 0x7F;
                    return;
                }
                m = func_800A0818(c[0x24], c[0x25], x, y, (u16 *)(p + 0x98));
                *(s16 *)(d + 0x2A) = m;
                if ((s16)func_8009A66C(m, c, d, 0x20) <= 0)
                {
                    *(s16 *)(d + 0x2A) = func_800A0818(c[0x24], c[0x25], D_80082E80[0x24],
                                                       D_80082E80[0x25], (u16 *)(p + 0x98));
                }
                if (func_8009FD7C(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25]) != 0)
                {
                    f = 1;
                }
            }
            else
            {
                func_800A0E6C(c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
        else
        {
            j = *(s8 *)(c + 0x26);
            if (j >= 0 && (D_800E2970_s[j].unk_0C & 2) != 0)
            {
                func_800A0E6C(c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
            else if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_800A0E6C(c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
    }
    i = 0;
    tbl = D_8006CD00;
    do
    {
        n = *(s16 *)(d + 0x2A);
        if ((*(u16 *)(p + 0x98) & 2) != 0)
        {
            s = n - tbl[i];
        }
        else
        {
            s = n + tbl[i];
        }
        if (n)
        {
            r = func_8009A66C((s16)s, c, d, 0x20);
        }
        else
        {
            r = func_8009A66C((s16)s, c, d, 0x20);
        }
        if ((s16)r > 0)
        {
            if (i >= 3 && f != 0)
            {
                d[0x71] &= 0x7F;
                return;
            }
            *(s16 *)(d + 0x2A) = s;
            *(d + (d[0x71] & 0x7F) + 0x74) = c[0x24];
            *(d + (d[0x71] & 0x7F) + 0x7C) = c[0x25];
            d[0x71]++;
            func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            kk = (*(u16 *)(d + 0x2A) >> 8) & 0xE;
            c[0x24] = *(u8 *)(kk + (s32)D_8006CCD8) + c[0x24];
            c[0x25] += *(u8 *)(kk + (s32)D_8006CCE8);
            func_8009A21C(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            break;
        }
        if (i == 0 && *(u16 *)(D_80082E80 + 0x24) != *(u16 *)(c + 0x24))
        {
            if ((s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                do
                {
                    return;
                } while (0);
            }
        }
        i++;
    } while (i < 8);
    if (i >= 8)
    {
        d[0x71] &= 0x7F;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A9A0C(d);
    }
    else
    {
        p[0x9C] = c[0x26];
        d[0x6D]--;
        g2 = (u8 *)&D_80083460;
        *(u16 *)(g2 + 0x8) = *(u16 *)(g2 + 0x8) + 1;
        if (*(s8 *)(d + 0x6D) == 0)
        {
            d[0x71] &= 0x7F;
            return;
        }
        if (d)
        {
            i = func_800BCB04(c[0x24] * 64 | 0x20, c[0x25] * 64 | 0x20,
                              *(u16 *)(d + 0x88) - 0x20);
        }
        else
        {
            i = func_800BCB04(c[0x24] * 64 | 0x20, c[0x25] * 64 | 0x20,
                              *(u16 *)(d + 0x88) - 0x20);
        }
        if (i < 0x200)
        {
            *(s16 *)(d + 0x88) = i;
        }
    }
}
