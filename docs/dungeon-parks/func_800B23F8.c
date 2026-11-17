/* PARK: func_800B23F8, 81 of 81 words, 13 aligned differences.
   ovl_dungeon.  Measured 2026-11-16 (j42, 21 rows), re-measured and improved
   to 13 by j57 the same day.  A 0x112 spawner: allocates a node, installs
   func_800B274C as its handler and func_800B2A60 as the payload's +0x8C
   step, and returns the payload or NULL.

   NEW DECLARATION NEEDED (func_800B274C is defined BELOW this function and
   its address is taken, so an implicit declaration will not do):

       void func_800B274C(u8 *, UnkVec *, u8 *);

   Everything else it touches is already declared: func_8003FD64,
   func_8004491C, func_800B22F0, func_800B2A60, D_80083498, D_80045340.

   -------------------------------------- residue: 13 rows, ALL IN THE PROLOGUE

   EVERY REGISTER IS NOW RIGHT and the whole body matches instruction for
   instruction.  What is left is a PERMUTATION of the thirteen block-0
   instructions -- the five (save, parameter-copy) pairs and the three call
   setup words:

       candidate   s4/a, s5/b, s3/c, s2/d, s1/p=NULL, li a0, la a1
       target      s4/a, s1/p=NULL, li a0, s5/b, la a1, s2/d, s3/c

   Same thirteen instructions, same registers, different order, so aligndiff
   scores it 13.  It is a sched2 decision in basic block 0: `sched.py`
   (--keep, block 0) shows every one of them at `priority = 1`, so the order
   is `rank_for_schedule`'s dependence-class tie-break over INSN_LUID, and the
   candidate's order is exactly ascending LUID (declaration order a,b,c,d then
   the first statement).  The target needs `p = NULL` and `li a0` BETWEEN a's
   copy and b's copy, and d's copy BEFORE c's -- i.e. LUIDs that no statement
   order can produce, because `assign_parms` emits the four parameter copies
   ahead of NOTE_INSN_FUNCTION_BEG and sched1's region starts after them
   (confirmed in the .sched dump: block 0's region is insns 18..39 and the
   copies are not in it).  So this is a sched2 ready-list problem and the
   lever, if there is one, is the block's insn SET, not its source order.

   Measured and rejected at 13 (no change): `s32 d`; the local declaration
   order with `p` first; `u8 *c` with the stores cast (byte-identical to the
   `s32 c` body below).  Measured and worse: `s32 b` 17; `s32 b, s32 d` 17;
   `p = NULL;` moved after the func_8003FD64 call 33.

   --------------------- j67, 2026-11-16: THE RESIDUE IS PROVABLY UNREACHABLE

   Re-measured unchanged: 81 of 81 words, 13 aligndiff rows, and `ovlverify`
   says **11 differing** -- the honest positional figure, because the residue
   is one permutation of block 0's words 4..14 and aligndiff's LCS scores the
   same permutation as 13.  Quote 11 to the next reader.

   WHAT sched2 ACTUALLY DOES HERE (read out of the compiler, not guessed:
   scratch/gcc-src/cdk-gcc-b18/sched.c, which IS cc1psx's scheduler):

   * `rank_for_schedule` (sched.c:2429) is priority, then dependence class
     against `last_scheduled_insn`, then `INSN_LUID (tmp) - INSN_LUID (tmp2)`
     with `tmp = *y` -- i.e. DESCENDING LUID, best first.
   * `schedule_select` (sched.c:2663) then scans the WHOLE first priority
     group -- and all of block 0 is priority 1, so that is every ready insn --
     and promotes the one with the largest `potential_hazard` (sched.c:1349).
     That is nonzero only for a unit with `max_blockage > 1`, and mips.md has
     exactly two units, "memory" and "imuldiv".  So a ready LOAD or STORE
     beats every ALU insn and nothing else ever ties.
   * Scheduling runs BACKWARD, so "take the highest-LUID ready insn" picks
     the last unscheduled insn of the chain every time.

   THEREFORE, for a block whose dependences agree with its chain order,
   **sched2's output IS the chain order, with every memory insn sunk to the
   latest legal slot**.  That is the whole of block 0: each `sw $sN` sits
   immediately before the insn that first writes $sN (its anti-dependence
   partner), and the three saves with no partner in this block -- $ra, $s6,
   $s0 -- sink to the end as $ra, $s6, $s0.  Both streams do that; the target
   does it too.  The candidate's whole block-0 trace was re-derived from the
   `sched.py` dump insn by insn and comes out exactly as emitted, so the
   model is not a hypothesis.

   SO BLOCK 0 IS NOT A TIE-BREAK TO BE NUDGED -- IT IS A PRINTOUT OF THE INSN
   CHAIN, and the target's order reads back as the chain

       mv_a  <  mv_p  <  li $a0  <  lui $a1  <  addiu $a1  <  mv_d  <  mv_c
       (and mv_b somewhere after li $a0)

   against the candidate's a, b, c, d, p=NULL, li $a0, lui/addiu $a1.  Two
   inequalities have to reverse: the copies of parameters 2, 3 and 4 must come
   AFTER the call's own argument setup, and parameter 3's copy after
   parameter 4's.

   NEITHER IS REACHABLE FROM C, and docs/matching-levers.md already says so
   under "A NARROWED FOURTH PARAMETER IS A sched2 ENTRY-BLOCK LEVER":
   `conversion_insns` is flushed by `emit_insns (conversion_insns)` at
   function.c:4093, INSIDE `assign_parms`, so every parameter insn precedes
   every body insn unconditionally -- a parameter copy's LUID cannot be beaten
   by a body statement.  `li $a0` and the `%hi/%lo` pair are body insns (the
   call's own argument setup, `load_register_parameters`, a0 then a1), and the
   call ENDS basic block 0 (the sched2 dump's block 0 runs 179..39, 39 being
   the jal), so there is no source position between the argument setup and the
   call to put anything in.  Nothing later moves a parameter copy either:
   local-alloc's `update_equiv_regs` is the only pass that relocates an
   initialization down to its use (local-alloc.c:1199), and it fires only for
   a pseudo carrying a REG_EQUIV, which is minted here only for a CONSTANT or
   a MEM source (local-alloc.c:1085-1092) -- never for a copy out of a hard
   register.  And the declaration order is nailed down by the ABI plus the
   target's own stores: the 4th parameter is the `sh` at m+0xA, the 3rd is the
   `sb` at r+0x25.

   The one freedom that lever does grant -- make the copy you want scheduled
   LAST into a conversion insn -- is already spent on this function.
   PROMOTE_PROTOTYPES is defined (config/mips/mips.h:1325), so a prototyped
   `s16`/`u8` parameter has DECL_ARG_TYPE `int` and takes the conversion path
   at function.c:3813 (`nominal_mode != passed_mode`); `a`, `b` and `d` are
   ALL already conversion parameters -- `a`'s surviving pair is exactly the
   two pseudos $s4/$s6 this park already calls load-bearing.  Only `c` is
   plain, and `c` cannot be narrowed: it is the long-lived host of the
   `c = (s32)p;` merge that is worth most of the 21 -> 13.  Even if it could
   be, the model says the order would become
   a, b, d, c, p, li, lui, addiu -- still 11 differing, a c/d swap inside the
   same residue.  (Model prediction, NOT measured; the spelling does not
   exist.)

   CONCLUSION: leave it at 81/81 and 11.  **Do not spend the permuter on it**
   -- the permuter rewrites C, and no C reaches this order.  Closing it needs
   a different insn SET for block 0, i.e. a different function, and the body
   below is already correct instruction for instruction and register for
   register everywhere else.

   ------------------------------------------ what took it from 21 to 13

   TWO THINGS, and both were read out of `-da` dumps rather than guessed.
   Method: concatenate csrc/ovl_dungeon.c's declaration block with the body
   and compile it alone with `tools/cdkgcc -O2 -G8 -da -S`; `.lreg` gives
   every pseudo's refs/live length and `.greg`'s `;; N regs to allocate:` IS
   `allocno_compare`'s sort.  The target's map is
   n=$s0 p=$s1 r/d=$s2 c=$s3 a=$s4 b=$s5 a_hi=$s6.

   1. `c` AND THE SECOND PAYLOAD POINTER ARE THE SAME LOCAL.  The target's
      `addu $s3, $s1, $zero` at word 37 re-uses the register the `u8 c`
      parameter has just died in, and that is not first-fit luck: as a
      separate `u8 *w` the copy is a 4-reference allocno that crosses one
      call, scores `floor_log2(4)*4/L` with L about 20, outranks `r` (0.278)
      and takes $s2 -- which is why j42's sweep of all ten placements of
      `w = p;` could only reach 34.  Declaring the parameter `s32 c` and
      writing `c = (s32)p;` after the +0x25 store merges the two into ONE
      six-reference allocno, 0.194, which sorts below `r` and above `a`, and
      $s3 falls out.  That alone fixes the a/c swap, the b and d registers
      and the four `w` rows: 21 -> 34 -> (with 2.) 13.
      Measured: `s32 b, c` without the merge 25; `w = (u8 *)n + 0x20;` in the
      late block (the 21-row park) leaves w in $v1.

   2. THE MERGE COSTS `n` ITS NINTH REFERENCE, AND n MUST OUTRANK p.
      With `w = (u8 *)n + 0x20;` in the late block, `n` has 9 refs over 37
      insns (0.730) against `p`'s 12 over 58 (0.621).  Merging replaces that
      statement with `c = (s32)p;`, which moves the reference to `p`: n falls
      to 8 (0.649) and p rises to 13 (0.672), they swap, and $s0/$s1 swap
      with them -- 34 differing, every `n` and `p` row wrong.
      `c = (s32)n + 0x20;` written before the `if` does NOT help: cse folds it
      straight back to `c = p` (byte-identical output, same 34).  Written in
      the join block cse cannot reach it, n gets its ninth reference and
      $s0/$s1 come out right -- but `c`'s live range then starts after the
      arms, 6 refs over 37 is 0.324, it outranks `r` and takes $s2: 24.
      THE FIX IS THE ZERO-INSTRUCTION DEVICE func_800A17E8 already uses
      (csrc/ovl_dungeon.c, "a zero-instruction reference"):

          __asm__("" ::"r"(n));

      one extra reference, no instruction, n back to 9 refs over the same 37
      insns.  Placed before the func_800B22F0 call or straight after the null
      test it gives the identical 13-row result; `;; Register dispositions`
      is then the target's map exactly.
      NOTE the device is NOT needed if an honest ninth reference to `n` can
      be found.  Everything tried cost a row: `*((u8 *)n + 0x33) = 0x1E;` for
      `p[0x13] = 0x1E;` does give n 9 and p 12, but it also drops p's live
      length to 46 so p still wins, and the store comes out `0x33($s0)` -- 35.
      The arithmetic to aim at, from `.lreg`: n is 8 refs/37 insns and p is
      13/58; n wins at 9 refs, or at <= 35 insns, or if p reaches 61 insns or
      drops to 12 refs WITHOUT shortening.  The same `__asm__("" ::"r"(c));`
      trick on `c` costs 3 words (84), because `c` is a `u8` there.

   The two 2026-11-16 catalogue entries this lands on are
   "`floor_log2` IN `allocno_compare` IS A CLIFF AT EVERY POWER OF TWO" and
   "A SHORT-LIVED VALUE REACHES A CALLEE-SAVED REGISTER ONLY BY SHARING A
   LOCAL WITH SOMETHING THAT CROSSES CALLS" -- this is that second entry's
   mechanism with a PARAMETER as the long-lived local.

   -------------------------------------------------------- also still true

   * `*(s8 *)(p + 0x9C) = -1;` and NOT `p[0x9C] = -1;`.  Through a `u8 *`
     lvalue gcc folds the -1 to 0xFF, cse shares it with the neighbouring
     `p[0x9A] = 0xFF` and the `addiu $v0, $zero, -0x1` disappears.  1 word.
   * `s32 a` with an explicit `(s16)a` at the func_800B22F0 call is 78 words
     and 30 -- the `s16` parameter's two pseudos are load-bearing.

   Measured with
     python3 tools/decomp/aligndiff.py <unit copy> func_800B23F8
     python3 tools/decomp/ovlverify.py <unit copy> func_800B23F8
   -> 81 of 81 words, 13 aligned differences. */

u8 *func_800B23F8(s16 a, u8 b, s32 c, s16 d, s32 e)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *p;
    s32 k;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        n->unk_10 = (u32)func_800B274C;
        p[0x13] = 0x1E;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x25) = c;
        *((u8 *)r + 0x24) = b;
        c = (s32)p;
        k = a & 3;
        if (k == 1)
        {
            *(u32 *)(p + 0x14) |= 0x6000;
            *(u32 *)(p + 0x1C) |= 0x6000;
        }
        else if (k >= 2)
        {
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
        }
        __asm__("" ::"r"(n));
        func_800B22F0(n, m, r, a);
        ((u8 *)c)[0x9A] = 0xFF;
        *(s8 *)((u8 *)c + 0x9C) = -1;
        *(u32 *)((u8 *)c + 0x8C) = (u32)func_800B2A60;
        *(s32 *)((u8 *)r + 0x8) = e;
    }
    return p;
}
