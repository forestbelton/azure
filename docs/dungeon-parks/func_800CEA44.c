/* ***** LANDED 2026-11-22: 242 of 242 words, 0 differing. *****

   This file is now a LANDED body awaiting merge, not a park.  Merge it with
   /tmp/claude-1000/agent-cea44b/merge_func_800CEA44.py (anchor-based and
   idempotent) or by hand into csrc/ovl_dungeon_800CABA8.c in place of
   `INCLUDE_ASM("asm/nonmatchings/ovl_dungeon_800CABA8", func_800CEA44);',
   then DELETE this file.  Declaration needed: `extern u8 D_800E1C3A[];' in
   the unit's own top block.  `extern u32 D_800835E4[];' is ALREADY in
   csrc/ovl_dungeon_decls.h:1073 and must NOT be added again.  No prototype
   change: csrc/ovl_dungeon_decls.h:1421 already carries
   `void func_800CEA44();', the only in-unit reference is the address-of in
   func_800CEE0C (defined after it, so no forward declaration is needed), and
   every other function in the unit compiles byte-identically modulo label
   numbering.

   PARK HISTORY BELOW.

   THE COMMENT BLOCK TO PUT ABOVE THE BODY IN csrc IS REPRODUCED BELOW, one
   `   | ' prefix per line, so that it survives even if the merge script in
   /tmp does not.  Strip the prefix and wrap it in a C comment.

   |  242 words, 0 differing.  A three-state machine on the halfword at `p + 0x6`.
   | State 0 spends the D_800E3648 slot for room `*(s16 *)(p + 0xC)`: for each
   | of its `*(s8 *)(e + 2)` charges it rolls a random pair out of D_80083460's
   | +0x18 table, asks func_800A1618 for a template, then tries up to sixteen
   | random tiles inside the D_800E2970 room record for one that func_8009A350
   | reports with no 0xB700 flag, and spawns there through the function pointer
   | func_800A0B94 RETURNS -- the `jalr $v0`, called with (0, (u8)a, (u8)c,
   | -0x400).  It then drains the spawn's +0x18 budget through func_800A1D4C and
   | stamps +0x9A/+0x8C/+0x1C/+0x92.  State 1 counts down +0x4 and plays the
   | message; the default state counts down, clears the slot and sets the two
   | 0x8000 request bits.
   | 
   | THE LAST TWO LEVERS, both register names with every instruction already in
   | place, and both derived from the compiler rather than swept for -- all five
   | repo sweeps (part/width/stmt/dev/ref) come back empty on the 3-row body AND
   | on the 2-row body.
   | 
   | (A) `c0 = func_800A6DA4(...); ... c = t[2] + c0;` -- THE SUM'S DESTINATION
   | MUST NOT BE ONE OF ITS OPERANDS.  optabs.c:412-434 swaps a commutative
   | optab's operands when
   |   ((target == 0 || REG_P (target))
   |      ? ((REG_P (op1) && ! REG_P (op0)) || target == op1)
   |      : rtx_equal_p (op1, target)) || GET_CODE (op0) == CONST_INT
   | -- "try to make the first operand a register; even better, try to make it
   | the same as the target".  With one accumulator `c`, `c = t[2] + c;` hits
   | `target == op1` and expand emits `(plus c t[2])`, which is exactly what
   | `c = c + t[2];` gives with no swap: so BOTH orders of one variable produce
   | `addu $s0, $v0, $a0` and eight spellings all measured the same 3 rows.
   | Giving the second func_800A6DA4 result its own local makes `target` a third
   | pseudo, the swap does not fire, and the target's `addu $s0, $a0, $v0`
   | appears.  3 rows -> 2.  (`c = c0 + t[2];` puts the operands back the wrong
   | way round; the device must stay on `c`, not move to `c0` -- 22 rows.)
   | 
   | (B) `j = *(u32 *)(o + 0x18) < tb[o[0x11]]; if (j == 0)` -- THE GUARD'S
   | `sltu` DESTINATION IS A GLOBAL-ALLOC DECISION, AND A BLOCK-LOCAL CANNOT
   | REACH $a0.  Written as a plain `if (tb[o[0x11]] <= *(u32 *)(o + 0x18))`
   | the comparison result is a fresh block-local pseudo; block_alloc
   | (local-alloc.c:1404) walks the operands 1..n calling
   | combine_regs(operand[i], operand[0]) and takes the first that wins, and
   | combine_regs (:1791) refuses operand 1 at its very first test because
   | `j` is a global allocno (`reg_qty < 0`; local-alloc.c:469 needs
   | reg_basic_block >= 0 and `j` is also read at the loop top).  The tie
   | therefore falls to operand 2, the limit -- a block-local -- and the result
   | takes ITS register: `sltu $v0, $a0, $v0`.  Untying it does not help, and
   | neither does moving the value into a block-local of its own: MIPS defines
   | no REG_ALLOC_ORDER, so find_free_reg hands an unsuggested block-local
   | quantity $v0, then $v1, then $a0, and the store temp just swaps registers
   | with `tb` (10 rows, `sltu $v1, $v1, $v0`).  docs/matching-levers.md,
   | "BREAKING combine_regs' TIE IS NOT THE SAME AS MOVING THE REGISTER": to
   | reach an $aN a value needs BOTH halves -- multi-block, so it is a
   | global-alloc allocno at all, and conflicting with $v0.  Storing the
   | comparison into `j` gives it both for free, because `j` already has them
   | (`.greg`: `98 conflicts: ... 95 ... 2 29`, and `98 in 4`), and the one
   | `sltu` then writes $a0 because its destination IS `j`.  2 rows -> 0.
   | `j = (u32)j < tb[o[0x11]];` with `if (j == 0)` or `if (!j)` is the same
   | object; `j = tb[o[0x11]] <= *(u32 *)(o + 0x18); if (j)` costs a word and 11
   | rows (the `<=` needs an extra `xori`), and `a`/`c`/`x`/`n` in place of `j`
   | measure 15/26/8/13 because only `j`'s allocno has the $v0 conflict.
   | 
   | `j` IS DEAD AT BOTH REUSES: it is `func_800A6D30() & 0x1E` from the top of
   | the same iteration and is reassigned every iteration, so using it first as
   | the o+0x18 store temp and then as the guard's boolean is safe, and the
   | stored value round-trips bit-exactly through s32.
   | 
   | THE FRAME IS 0x48 BECAUSE `k` IS A SPILLED HImode PSEUDO.  `w` sits at 0x10
   | and `k` at 0x18 with six bytes between them: `(s16)k` is a PARADOXICAL
   | subreg (mips.md's extendhisi2 is `gen_lowpart (SImode, ...)`), so
   | scan_paradoxical_subregs (reload1.c:3772) sets reg_max_ref_width[k] = 4 and
   | alter_reg passes align == -1 to assign_stack_local, which rounds the slot
   | up to 8 at BIGGEST_ALIGNMENT -- vars 16, frame 0x48.  That is also why the
   | reload is `lhu` + `sll 16` + `sra 16` and both ends use $t0.  `(void)&k`
   | makes `k` an ordinary addressable 2-byte local and loses all of it (~23
   | rows of prologue/epilogue offsets).  AND IT ONLY SURVIVES WHILE
   | `g = (u8 *)&D_80083460;` IS WRITTEN BEFORE THE func_800A6D30 CALL: with `g`
   | written after, the symbol+0x18 folds to two words instead of three, the
   | `while (n >= 0)` body gets small enough for loop.c:1730's
   | `threshold * savings * m->lifetime >= insn_count` to flip, and
   | `(s16)k * 0x14` is hoisted out of the loop, undoing the spill.  6 words.
   | 
   | ONE LOCAL PER SITE for D_800E3648 (b/e, b2/e2, b3/e3) and D_80083460
   | (g, g2): one variable is one pseudo spanning three basic blocks, which
   | global_alloc gives a callee-saved register so the symbol's `addiu` writes
   | $s0 at every site; the target materialises it into a dead temp each time.
   | Shared: 81 rows.  Same finding as func_800CB170 above.
   | 
   | `rm = D_800E2970; t = rm + k * 0x14;` -- the symbol is its own local for the
   | same reason, 8 rows.  `tb` + `tb1` are two pseudos because the target has
   | two: the `tb1 = tb` copy survives cse only inside the `if`, where the `do`
   | body's label has two predecessors and resets cse's table.
   | 
   | BOTH func_800A6DA4 CALLS BEFORE EITHER `t[...]` LOAD: a load cannot be
   | scheduled across a call, so `a = call() + t[0];` reads t[0] between the two
   | calls.  17 rows.  `(s16)x` at the three post-spawn calls folds to a plain
   | move (num_sign_bit_copies on a `lbu` is 24, so combine's
   | simplify_shift_const kills the sll/sra pair) and nothing after combine
   | deletes copies; plain `x` is 1 row worse, `(u8)x` leaves an `andi`.
   | 
   | THE ONE DEVICE: `c++; c--;` after `c = t[2] + c0;`, worth 7 rows at the
   | same word count -- four more references to `c`, no instructions.  Without
   | it the two `lbu`s and `addiu $a3, $sp, 0x10` schedule in the wrong order
   | (0x800CEB68..0x800CEB78).  No honest statement was found that supplies
   | those references. 


   PARK: func_800CEA44, 242 of 242 words, 2 aligned differences.

   ====================================== 2026-11-22 (session 3): 3 rows -> 2

   (13) THE `addu' OPERAND ORDER IS expand_binop's `target == op1' SWAP, and
       no spelling of ONE variable can change it.  optabs.c:412-434 says, for
       a commutative optab, "try to make the first operand a register; even
       better, try to make it the same as the target", and swaps when
         ((target == 0 || REG_P (target))
            ? ((REG_P (op1) && ! REG_P (op0)) || target == op1)
            : rtx_equal_p (op1, target))
          || GET_CODE (op0) == CONST_INT
       For `c = t[2] + c;' op0 is t[2] (a reg), op1 is `c' AND `c' is the
       store target, so `target == op1' fires and expand emits
       `(plus c t[2])' -- the same RTL `c = c + t[2];' gives with no swap at
       all.  That is why all eight spellings in the list below measure 3:
       both orders of ONE accumulator variable collapse to the same insn.
       The fix is to make the add's DESTINATION a different pseudo from both
       operands, so `target == op1' is false:
         `c0 = func_800A6DA4(...); ... c = t[2] + c0;'       242 w, 2 rows
       `c = c0 + t[2];' would swap back (op0 == ... no: no swap, but then the
       order is c0,t[2] again), so the sum must be written t[2] FIRST.
       Measured on the 3-row body: `s32 c0' and `u32 c0' are identical at 2;
       putting the sum in a fresh `c2' and using `c2' at the two later casts
       is also 2; `a' through a fresh local as well is 20; and the `c++; c--;'
       device must stay ON `c' (moving it to `c0', or dropping it, is 22).

   ====================================== 2026-11-21 (session 2): 112 rows -> 3

   The body was REWRITTEN.  WORD COUNT IS NOW EXACT and all nineteen remaining
   rows are REGISTER NAMES -- no instruction is missing, spurious or in the
   wrong order except one `addu` operand pair.  Every device the park carried
   is gone except `c++; c--;`.  Each finding below was measured on the body the
   one before it produced, on a tools/decomp/onefunc.py standalone that
   reproduces the real unit exactly (re-checked against the unit at 112, 240
   and 242 words).

   (1) THE FRAME IS 0x48 BECAUSE THE LOCAL AREA IS 16 BYTES, NOT 4.  The park
       had `w` and `k` as two adjacent 2-byte locals -> vars=8 -> frame 0x40,
       which cost ~23 rows of prologue/epilogue offsets on its own.  The
       target puts `w` at 0x10 and `k` at 0x18 with six bytes between them.
       That gap is reload's: `k` is an HImode PSEUDO that gets SPILLED, and
       its only use is `(s16)k`, which mips.md's `extendhisi2`
       (config/mips/mips.md:2559-2577) expands as
       `gen_lowpart (SImode, operands[1])` -- a PARADOXICAL subreg.
       scan_paradoxical_subregs (reload1.c:3772) sets reg_max_ref_width[k] = 4,
       so alter_reg (reload1.c:2495) calls `assign_stack_local (GET_MODE (k),
       total_size, inherent_size == total_size ? 0 : -1)` with align == -1,
       and assign_stack_local (function.c) turns align == -1 into
       BIGGEST_ALIGNMENT (mips.h:1252 = 64 bits) AND rounds the SIZE up to 8:
         w at 0x10, 2 bytes -> frame_offset 2 -> CEIL_ROUND(2,8) = 8
         -> k at 0x18, size 8 -> frame_offset 16 -> vars 16 -> frame 0x48.
       `(void)&k` -- what the park had -- makes k an ordinary addressable
       2-byte local at align 2 and the gap never opens.

   (2) `(void)&k` ALSO HID THE SPILL FORM.  A spilled HImode pseudo under a
       paradoxical subreg reloads as `lhu` + `sll 16` + `sra 16`; an
       addressable `u16 k` read as `(s16)k` is one `lh`, because combine
       re-forms extendhisi2_internal.  The target has the three-insn form.
       Same at the store: `sra $t0,$v0,24` + `sh $t0,0x18($sp)` is reload
       writing the spill, which is why BOTH ends use $t0.

   (3) THE `if (q)` DEVICES WERE THE BUG, NOT THE FIX.  `q` is never read by
       the target -- $a2 appears nowhere in the listing -- and the park's two
       duplicated `if (q)` arms cost `sw $a2` in the prologue plus two `lw`
       reloads.  Removing the one around `t = D_800E2970 + k * 0x14` is also
       what lets the k spill happen at all.

   (4) ONE LOCAL PER SITE FOR D_800E3648 AND D_80083460.  The park reused one
       `b`/`e` pair at all three D_800E3648 sites and one `g` at both
       D_80083460 sites.  One variable is ONE pseudo spanning three basic
       blocks, so global_alloc gives it a callee-saved register and the
       symbol's `addiu` writes $s0 at every site; the target materialises the
       symbol into a dead temp at each site.  Separate locals (b/e, b2/e2,
       b3/e3, g/g2): 81 -> 69 rows, and the word count back to 242.  Same
       finding csrc/ovl_dungeon_800CABA8.c:211 records for func_800CB170.

   (5) BOTH func_800A6DA4 CALLS BEFORE EITHER `t[...]` LOAD.  The target
       loads t[0] and t[2] AFTER the second call; `a = call() + t[0];
       c = call() + t[2];` loads t[0] between the two calls, because a load
       cannot be scheduled across a call.  Splitting into
       `a = call(); c = call(); a = a + t[0]; c = t[2] + c;` is 69 -> 52.

   (6) `b3 = D_800E3648;` BELONGS AFTER THE TWO CALLS in the default state.
       The park had moved it above them (worth a row on the old body); with a
       per-site local it costs a callee-saved register.  52 -> 45.

   (7) THE do/while GUARD: REVERSE THE COMPARISON, DO NOT RE-READ.  The park
       re-read `tb[y]` in the guard to stop gcc unifying the guard with the
       loop test.  The real reason they do not unify in the target is that
       the loop test loads `*(u32 *)(o + 0x18)` AFTER the limit: written as
       `tb[o[0x11]] <= *(u32 *)(o + 0x18)` the limit is evaluated first, so
       the reload of o+0x18 lands between the limit load and the `sltu`, and
       jump2's cross-jumping has no common tail to merge.  Written the other
       way round gcc emits `j` into the loop's own test and the target's
       seven-word duplicated exit test never appears.  45 -> 35.

   (8) `g = (u8 *)&D_80083460;` BEFORE `j = func_800A6D30() & 0x1E;`.  Worth
       SIX WORDS and 10 rows (234/44 -> 240/34), and it REPLACED a `g++; g--;`
       device that had been doing the same job.  With `g` written after the
       call the symbol+0x18 folds to `lui %hi(D_80083460+0x18)` /
       `lw %lo(...)`, two words where the target has three
       (`lui`/`addiu`/`lw 0x18($t0)`) -- and because that is two fewer insns
       inside the `while (n >= 0)` loop, loop.c's desirability test
       `threshold * savings * m->lifetime >= insn_count` (loop.c:1730) flips
       and `(s16)k * 0x14` gets HOISTED out of the loop, which undoes (1) and
       (2) as well.  THE TWO ARE ONE LEVER: the k spill only survives while
       the loop body is big enough that loop.c declines to hoist.

   (9) TWO SECOND PSEUDOS, which were the last two words.  A value computed
       in one block and used inside the following loop is TWO pseudos in the
       target and one here, and a plain `t2 = t;` copy is deleted by cse.
       * `tb`: `if (...) { tb1 = tb; do { } while (tb1[...] <= ...); }`.  The
         copy survives because the `do` body's label has two predecessors, so
         cse's table is reset there and `tb1` is never replaced by `tb`.
         35 -> 20 rows, and 242/242 words.
       * `x`: `(s16)x` at the three calls after the spawn.  `x` holds
         `r[0]`, a `lbu`, so num_sign_bit_copies is 24 and combine's
         simplify_shift_const folds `(ashiftrt (ashift x 16) 16)` to `x`
         itself -- turning the sll/sra pair into a PLAIN MOVE, which survives
         because NOTHING AFTER COMBINE DELETES COPIES.  `(u8)x` leaves an
         `andi` (1 row worse), `x3 = x;` is deleted outright (no effect).
         20 -> 19.
       The honest reading of the second is probably that func_800A0B94,
       func_800A152C and func_80042640 take `s16`/`short` in that position
       and the conversion is implicit at the call; none of the three is
       landed in this unit, so the cast is written out rather than changing a
       prototype other functions share.

   (10) `j = tb[y]; *(u32 *)(o + 0x18) = j;` -- SPLIT THE STORE THROUGH `j`,
       THE ROLL VARIABLE, not through a fresh local.  19 -> 13 rows.  Found by
       decomp-permuter (output-105-1 of a 25-minute 4-job run, the only real
       change in that find; everything else it had was reparenthesisation).
       `j` is dead at that point -- it is `func_800A6D30() & 0x1E` from the
       top of the same iteration and is reassigned every iteration -- so this
       is safe, and the value round-trips bit-exactly through s32.  IT HAS TO
       BE `j`: a fresh `u32 u` or `s32 n2` measures 19, and reusing `a` 25,
       `n` 24, `i` 19, `x` 29/243, `c` 47, `y` 46.  What it buys is `j`'s
       extra references and longer live range, which is what finally gets the
       guard's `tb` off $a0.

   (11) `rm = D_800E2970; t = rm + k * 0x14;` -- THE SYMBOL IS ITS OWN LOCAL,
       exactly like (4) and (7).  13 -> 5 rows.  It closes cluster (A) whole:
       with the address in its own variable the chain comes out k in $v1, the
       *20 in $v0 and the symbol in a third register, which is what the target
       has.  Found by decomp-permuter (run 2, output-30-1) and SPLIT OUT of
       that find: the find also carried `new_var = (s16 *)(p + 0xC)` hoisted
       above the `if (k >= 0)` and an `if (1)` wrapper, and each of those
       measures 13 on its own -- i.e. worth nothing.  The whole find and this
       one statement both measure 5.

   (12) `j = func_800A6D30(); ... + (j & 0x1E)` -- MASK AT THE USE, not at the
       assignment.  5 -> 3 rows.  `j = func_800A6D30() & 0x1E;` puts the
       masked value in $a0 where the target reuses $v0 (`andi $v0, $v0, 0x1E`
       straight on the return register).  `j = func_800A6D30(); j = j & 0x1E;`
       as two statements measures 5, and dropping `j` entirely --
       `r = *(u8 **)(g + 0x18) + (func_800A6D30() & 0x1E);` -- measures 11,
       because `j` is also the store temp from (10).

   ====================================== 2026-11-22 (session 3): 2 rows -> 0

   (14) `j = *(u32 *)(o + 0x18) < tb[o[0x11]]; if (j == 0)' -- THE GUARD'S
       `sltu' DESTINATION IS A GLOBAL-ALLOC DECISION AND A BLOCK-LOCAL CANNOT
       REACH $a0.  The diagnosis in the residue note below is right about the
       tie and wrong about the cure.  Making the compared value block-local
       DOES move the tie to operand 1 -- but MIPS defines no REG_ALLOC_ORDER,
       so find_free_reg hands an unsuggested block-local quantity $v0, then
       $v1, then $a0, and the store temp merely SWAPS REGISTERS WITH `tb':
       `sltu $v1, $v1, $v0' with tb in $a0, 10 rows.  Measured that way for a
       fresh `u32 u'/`s32 u', for `j' made block-local by renaming its
       loop-top use, and with `tb' split into a base local + an incremented
       one (243-244 words).  docs/matching-levers.md, "BREAKING combine_regs'
       TIE IS NOT THE SAME AS MOVING THE REGISTER", states the rule: to reach
       an $aN a value needs BOTH halves -- multi-block, so it is a
       global-alloc allocno at all, AND conflicting with $v0.
       `j' ALREADY HAS BOTH (`.greg': `98 conflicts: 80 91 93 94 95 97 98 101
       2 29', and `98 in 4'), so the cure is to make the comparison's
       DESTINATION be `j': one `sltu' whose dest and operand 1 are the same
       global pseudo.  2 -> 0.
         `j = (u32)j < tb[o[0x11]];' with `if (j == 0)' or with `if (!j)'
           produce the same object                                         0
         `j = tb[o[0x11]] <= *(u32 *)(o + 0x18); if (j)'          243 w, 11
           (the `<=' needs an extra `xori')
         `a'/`c'/`c0'/`i'/`x'/`n' as the boolean instead of `j'
                                              15/26/2/2/8/13 (c0 and i are 2,
           i.e. no effect at all -- their allocnos have no $v0 conflict)
       `j' is dead at both reuses, so this is safe: it is
       `func_800A6D30() & 0x1E' from the top of the same iteration and is
       reassigned every iteration.

   ALSO RE-RUN AT 2 ROWS AND EMPTY, all five of them, on the body (13)
   produced: partsweep (5 + 2 partitions), widthsweep, stmtsweep (8 movable
   assignments), devsweep (1740 candidates/round) and refsweep (28 names,
   2923 insertion points).  A 506-candidate private sweep of the guard block
   -- store-temp name x value-split shape x call/`o[0x12]'/`tb++' position --
   also bottomed out at 2.  Nothing in any of those spaces was ever going to
   find it, because every one of them keeps the comparison result in a
   compiler temp.

   -------------------------------------------- residue AS IT WAS AT 2 ROWS

   TWO ROWS, 242/242 words, and both are ONE DECISION -- the guard's `sltu`
   DESTINATION:

     0x800CEC58  sltu $v0, $a0, $v0   vs  sltu $a0, $a0, $v0
     0x800CEC5C  bnez $v0, ...        vs  bnez $a0, ...

   Both source registers already match ($a0 = the value stored at o+0x18,
   $v0 = the limit).  The mechanism is local-alloc.c:1404/1791: block_alloc
   walks the operands 1..n and calls combine_regs(operand[i], operand[0]),
   taking the FIRST that wins, so the sltu's result is tied to operand 1 if
   it can be and to operand 2 otherwise.  Operand 1 here is `j`, and
   combine_regs refuses at its first test, `reg_qty[ureg] < 0`:
   local-alloc.c:472 sets reg_qty to -1 for any pseudo with
   reg_basic_block < 0, and `j` is referenced in TWO basic blocks (the
   `func_800A6D30() & 0x1E` at the loop top and the store here), so it is a
   global allocno.  The tie therefore falls through to operand 2, the limit
   -- a block-12 local -- and the result takes ITS register, $v0.  The .lreg
   dump shows this directly: insn 363 `(set (reg 196) (ltu (reg/v 98)
   (reg 194)))` with `;; Register 194 in 2.` and `;; Register 196 in 2.`,
   and `Register 98 used 10 times across 8 insns` with NO `in block N`.
   `98 in 4` -- `j` IS in $a0, as the target wants; it is only the tie that
   goes the wrong way.

   ALL FOUR SWEEPS ARE CLEAN ON THE 3-ROW BODY, re-run after every change
   above with a private fast harness on the onefunc standalone (~0.15 s a
   candidate -- this unit is 5745 lines, so the repo tools compile the whole
   unit at ~25 s a candidate and a full refsweep does not finish):
     cancelled pairs, every local x every statement      1403 cands, none < 3
     do-while(0) + duplicated arm, every local x stmt    1334 cands, none < 3
     declared widths, every local and the parameter        46 cands, none < 3
     adjacent-statement swaps, every same-indent pair      37 cands, none < 3
     declaration ORDER, every local to every position     506 cands (run on
       the 19-row body; pseudo number is the LAST tie-break in both
       allocno_compare and qty_compare_1, and it changed nothing anywhere)

   MEASURED AND REJECTED (rows / words, on the body named):
     on the 112-row park body:
       plain `while (*(u32 *)(o + 0x18) >= tb1[o[0x11]])`   235 rows / 230 w
       `tb1` computed before the store                      235 / 230
       `tb++` with the guard on `u`                         224 / 232
       `s32 kk` intermediate for the byte test              229 / 237
       `(u8 *)&D_80083460` inlined at the use               220 / 240
     on the 81-row body (w[4] + addressable k):
       `s16 k; k = D_80082EA6; if (k >= 0)`                  82 / 242
       `u16 k; if ((s16)k >= 0)` (the park's)                82 / 242
       `kk = D_80082EA6; k = kk; if (kk >= 0)`               83 / 239
       `k = D_80082EA6; kk = k; if (kk >= 0)`                85 / 244
       no `(void)&k`, `s16 k`, `if (k >= 0)`                 90 / 241
     on the 52-row body:
       `a = c1; c = c2; a = a + t[0]; c = c + t[2];`         63 / 233
       `a++; a--;` instead of `c++; c--;`                    62 / 233
       `x2`/`y2` separate locals for the two call results    59 / 233
     on the 34-row body:
       deleting `g++; g--;` WITHOUT moving `g` up            44 / 234
       `r = *(u8 **)(g + 0x18); r = r + j;`                  44 / 234
       `g = *(u8 **)((u8 *)&D_80083460 + 0x18); r = g + j;`  44 / 234
       `y = r[1];` before `x = r[0];`                        44 / 234
       `x3 = x;` before the func_800A0B94 call               34 / 240 (the
         copy is deleted by cse -- byte-identical to no copy)
       `if (x) { x3 = x; } else { x3 = x; }`                 34 / 240
       `x3 = x; x3++; x3--;`                                 34 / 240
       `s = func_800A1618(r[0], 1); x = r[0];`               40 / 240
       `s = func_800A1618(r[0], 1);` after `y = r[1];`       37 / 240
       `s++; s--;` after func_800A1618 (x DOES get $s5, but
         `s` overshoots $s4 and takes $s3 off `i`)           37 / 240
     on the 3-row body:
       `c = c + t[2]`, `c += t[2]`, `(s32)t[2] + c`, the two adds
         swapped, `d2 = t[2]` and `d0`/`d2` into their own locals   all 3
       `if (*(u32 *)(o + 0x18) >= tb[o[0x11]])`, `if (j >= ...)`,
         `if (tb[o[0x11]] <= j)` for the guard                      all 3
       `lim = tb[o[0x11]];` into its own local                          3
     on the 5-row body:
       `j = func_800A6D30(); j = j & 0x1E;` as two statements            5
       `r = j + *(u8 **)(g + 0x18);`                                     5
       `r = *(u8 **)(g + 0x18) + (func_800A6D30() & 0x1E);` (no `j`)     11
       `a`/`n`/`i`/`x`/`c`/`y`/a fresh local as the store temp
         instead of `j`                            18/16/11/21/39/38/11
       no store temp at all                                             11
       deleting `c++; c--;`                                             10
       `x` instead of `(s16)x`                                     20 / 241
       the permuter's `new_var = (s16 *)(p + 0xC)` hoist alone            13
       the permuter's `if (1)` wrapper alone                             13
     on the 13-row body:
       deleting `c++; c--;`                                  18 / 242
       `x` instead of `(s16)x` at the three calls            28 / 241
       one `tb` instead of `tb` + `tb1`                      27 / 241
       `g = (u8 *)&D_80083460;` after the func_800A6D30 call 59 / 236
       `u = tb[y];` / `n2 = tb[y];` with a FRESH local       19 / 242
       `a`/`n`/`i`/`x`/`c`/`y` as the store temp instead of
         `j`                                      25/24/19/29/47/46
     on the 20/19-row body:
       `x3 = (u8)x` / `x3 = (u16)x`                          20 / 242 (`andi`)
       `tb = &D_800835E4_s;` with a scalar extern alias      20 / 242
       `tb1 = tb + 1;` without the `if` guard                32 / 243
       `tb1 = tb;` outside the guard (plain `while`)         22 / 241
       `o[0x12] = 0;` before `tb = D_800835E4;`              20 / 242
       `u = tb[y]; tb++; *(u32 *)(o + 0x18) = u;`            20 / 242
       `tb = D_800835E4;` before `o[0x43] = func_800A6D30()` 31 / 240
       `o[0x12] = 0;` first in the block                     29 / 241
       `o[0x12] = 0;` last in the block                      31 / 243
       `tb = D_800835E4 + 1; ... tb[y - 1]`                  22 / 242
       `t = D_800E2970;` then `t = t + k * 0x14;`            22 / 241
       `t = &D_800E2970[k * 0x14]`, `k * 0x14 + D_800E2970`,
         and `t = ...` before `i = 0xF;`                     19 / 242 (same)
       deleting `c++; c--;`                                  24 / 242

   THE ONE DEVICE LEFT: `c++; c--;` after `c = t[2] + c;`, worth 7 rows
   (3 -> 10) at the same word count.  It is four more references to `c` and compiles to
   nothing; without it the two `lbu`s and `addiu $a3, $sp, 0x10` schedule in
   the wrong order (0x800CEB68..0x800CEB78).  I did not find the honest
   statement that supplies those references.

   `u16 w` WITH `&w` IS THE SPELLING, and it is the unit's established one:
   the landed func_8009A350 (csrc/ovl_dungeon.c:9896) takes `u16 *out` and
   writes exactly one halfword through it.  An intermediate body in this
   session used `u16 w[4]` to force eight bytes of frame, and on the 81-row
   body the scalar measured 90 rows against the array's 81 -- but that was a
   stale negative the moment (8) landed: on THIS body the scalar and the array
   produce a byte-identical object (same aligndiff md5, 242 words / 19 rows),
   because the eight bytes are k's spill slot, not w.  The scalar is what is
   written below.

   DECLARATIONS THIS NEEDS -- EXACTLY ONE AT MERGE TIME:
     extern u8 D_800E1C3A[];
   `extern u32 D_800835E4[]' is ALREADY at csrc/ovl_dungeon_decls.h:1073 and
   must not be added twice.  The line below is kept only so parksplice still
   harvests a working splice from this file; it is a duplicate, not a change.
     extern u32 D_800835E4[];
   NO PROTOTYPE CHANGE.

   WHAT IT DOES: a three-state machine on the halfword at `p + 0x6`.
   State 0 spends the D_800E3648 slot for room `*(s16 *)(p + 0xC)`: for each
   of its `*(s8 *)(e + 2)` charges it rolls a random pair out of
   D_80083460's +0x18 table, asks func_800A1618 for a template, then tries up
   to sixteen random tiles inside the D_800E2970 room record for one that
   func_8009A350 reports with no 0xB700 flag, and spawns there through the
   function pointer func_800A0B94 RETURNS -- the `jalr $v0` at 0x800CEBF0,
   called with (0, (u8)a, (u8)c, -0x400).  It then drains the spawn's +0x18
   budget through func_800A1D4C and stamps +0x9A/+0x8C/+0x1C/+0x92.  State 1
   counts down +0x4 and plays the message; the default state counts down,
   clears the slot and sets the two 0x8000 request bits. */

void func_800CEA44(u8 *p, UnkVec *v, u32 q)
{
    u8 *b;
    u8 *b2;
    u8 *b3;
    u8 *e2;
    u8 *e3;
    u8 *g2;
    u8 *e;
    u8 *t;
    u8 *rm;
    u8 *r;
    u8 *o;
    u8 *g;
    u32 *tb;
    u32 *tb1;
    s32 n;
    s32 j;
    s16 i;
    u16 w;
    s16 k;
    s32 a;
    s32 c;
    s32 c0;
    s32 x;
    s32 y;
    u8 *s;

    if (*(s16 *)(p + 0x6) == 0)
    {
        k = D_80082EA6;
        if (k >= 0)
        {
            b = D_800E3648;
            e = b + *(s16 *)(p + 0xC) * 4;
            n = *(s8 *)(e + 2);
            while (n >= 0)
            {
                if (func_8003FA44(3) == 0)
                {
                    b2 = D_800E3648;
                    e2 = b2 + *(s16 *)(p + 0xC) * 4;
                    e2[2] = e2[2] - n;
                    return;
                }
                g = (u8 *)&D_80083460;
                j = func_800A6D30();
                r = *(u8 **)(g + 0x18) + (j & 0x1E);
                x = r[0];
                y = r[1];
                s = func_800A1618(x, 1);
                if (s != NULL)
                {
                    i = 0xF;
                    rm = D_800E2970;
                    t = rm + k * 0x14;
                    while (1)
                    {
                        a = func_800A6DA4(0, (u16)(*(u16 *)(t + 4) - 1));
                        c0 = func_800A6DA4(0, (u16)(*(u16 *)(t + 6) - 1));
                        a = a + t[0];
                        c = t[2] + c0;
                        c++;
                        c--;
                        if (func_8009A350((u8)a - 1, (u8)c, 0, &w) != 0 && (w & 0xB700) == 0)
                        {
                            break;
                        }
                        i--;
                        if (i < 0)
                        {
                            break;
                        }
                    }
                    if (i >= 0)
                    {
                        D_800E2968 = 1;
                        o = ((u8 * (*)(s32, s32, s32, s32)) func_800A0B94((s16)x, s, 1))(
                            0, (u8)a, (u8)c, -0x400);
                        if (o != NULL)
                        {
                            func_800A152C((s16)x, 1);
                            func_80042640((Unk42710 *)o, (s16)x);
                            o[0x43] = func_800A6D30();
                            tb = D_800835E4;
                            o[0x12] = 0;
                            j = tb[y];
                            *(u32 *)(o + 0x18) = j;
                            tb++;
                            j = *(u32 *)(o + 0x18) < tb[o[0x11]];
                            if (j == 0)
                            {
                                tb1 = tb;
                                do
                                {
                                    func_800A1D4C(o, 0);
                                } while (tb1[o[0x11]] <= *(u32 *)(o + 0x18));
                            }
                            o[0x9A] = 0x16;
                            *(u32 *)(o + 0x1C) &= 0xF7FBFFFF;
                            *(u32 *)(o + 0x8C) = 0;
                            *(u16 *)(o + 0x92) -= 0x200;
                        }
                    }
                }
                n--;
            }
        }
        *(s16 *)(p + 0x4) = 0xA;
        *(u16 *)(p + 0x6) += 1;
    }
    else if (*(s16 *)(p + 0x6) == 1)
    {
        *(u16 *)(p + 0x4) -= 1;
        if (*(s16 *)(p + 0x4) < 0)
        {
            func_800997FC(D_800E1C3A);
            func_800419EC(0xC, 0x18);
            func_800A56E0(0x602);
            *(s16 *)(p + 0x4) = 0x10;
            *(u16 *)(p + 0x6) += 1;
        }
    }
    else
    {
        *(u16 *)(p + 0x4) -= 1;
        if (*(s16 *)(p + 0x4) < 0)
        {
            func_8004437C(0, 1);
            func_80053DA8(0x200);
            b3 = D_800E3648;
            e3 = b3 + *(s16 *)(p + 0xC) * 4;
            g2 = (u8 *)&D_80083460;
            *(u32 *)e3 = 0;
            *(u16 *)(g2 + 0xA) -= 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

