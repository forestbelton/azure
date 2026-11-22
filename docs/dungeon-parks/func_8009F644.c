/* PARK: func_8009F644 -- 137/137 words, 2 ALIGNED DIFFERENCES (was 9, 16, 41)

   Measured with
     python3 tools/decomp/parkresidue.py func_8009F644
   reproduced byte for byte on a tools/decomp/onefunc.py standalone, and
   re-measured in a real unit copy: ovlverify says 137 target words, 137
   candidate words, 2 differing (masked) -- so there are no transposed pairs
   and the aligned row count and the word count agree.

   STILL BLOCKED ON THE THIRD RODATA CARVE.  It owns `jtbl_80088D7C' at rodata
   offset 0x61c, which is 4 mod 8, so landing it inside the HEAD unit pads the
   table and the container comes out eight bytes long.  That is a splat change,
   not a body problem; `parkresidue'/`aligndiff'/`ovlverify' all measure
   against the listing and are unaffected.

   WHAT IT IS.  The message/damage-number queue push.  Bails out if
   `D_800E296C & 0x10000000', if `*(u16 *)0x80013714 & 1', or if the queue is
   full at 0xF01 entries; otherwise it indexes the queue by the count at +0x6,
   decides whether to overwrite the last entry or append, and fills one of six
   shapes chosen by a jump table on `f - 8'.

   THE SIGNATURE IS FIXED BY THE UNIT, NOT BY THE LISTING.
   csrc/ovl_dungeon_decls.h declares `void func_8009F644(u8 *, s32, s32, s32);'
   and four landed call sites rely on it.  The PARAMETER NAMES below are
   `(u8 *p, s32 f0, s32 a0, s32 e0)' with `s32 f, a, e;' user copies -- names
   only, the types and the order are unchanged, so the existing prototype
   still matches and no header edit is needed.  Do NOT narrow a parameter in
   the definition: `s16 f' under an `__asm__("func_8009F644")' alias was
   measured at 137 words / 36 rows, because every `f | i' then pays its own
   `sll 16'/`sra 16'.  The narrowing belongs AT THE USE --
   `(q[1] & 0xF8) != (s16)f' is what buys the target's `sll'/`sra' pair.

   DECLARATIONS
   NONE -- func_8009F9E8 rides an implicit declaration, and D_800E296C and the
   existing func_8009F644 prototype are already in csrc/ovl_dungeon_decls.h.
   END DECLARATIONS

   ========================================================================
   WHAT IS LEFT: 2 ROWS, 0 WORDS, ONE CLUSTER.

     R 37  8009F6D8  andi s2,v1,0x7   vs  andi $v1, $v1, 0x7
     R 41  8009F6E8  move v1,s2       vs  addu $s2, $v1, $zero

   THE `i' COPY IS THE RIGHT WORD IN THE WRONG DIRECTION.  The target's `andi'
   ties IN PLACE into the `lhu'/`srl' chain, and `i' -- which has to be
   callee-saved because it crosses the `jal func_8009F9E8' -- is set by a
   surviving plain COPY out of it.  Ours writes `i' with the `andi' itself and
   copies BACK into a narrow local for the `(v & 7) != t' test.  So the roles
   are right in `t = <expr>; i = t;' AND THAT SPELLING LOSES A WORD.

   WHY IT LOSES THE WORD, read off the dumps rather than guessed.  With
   `s32 t; t = (*(u16 *)(p + 0x2A) >> 9) & 7; i = t;' the `.rtl' dump has
   insn 59 `(set (reg 113) (and (subreg:SI (reg:HI 112)) 7))', insn 60
   `(set (reg/v:SI 87) (zero_extend ...))' = `t', insn 63
   `(set (reg/v:SI 86) (reg/v:SI 87))' = `i = t'.  By `.combine', insn 60 is
   `(set (reg/v:SI 86) (and (reg 111) 7))' and insn 63 is GONE, and jump_insn
   76 (the `bne') already reads reg 86.  The `and:SI' count by pass is 10
   through rtl/jump/cse/loop/cse2/bp/flow and 9 from combine on, so CSE (with
   `-fcse-follow-jumps', which this compiler enables) rewrote the test's
   operand from `t' to `i' first, which left `t' with a single remaining use
   and let combine write the `and' straight into `i'.

   MEASURED AND REJECTED ON THIS 2-ROW BODY (baseline canary interleaved
   between every candidate; the canary read 2/137 every time):
     `t = <expr>; i = t;', test `t', at t = s32/u8/u16/s8/s16   136w, 6 rows
     the same with the test reading `i'                          136w, 6 rows
     `i = t;' moved out of the block, before the switch          137w, 24 rows
     `w = <expr>; i = w; t = w;' (three locals)                  137w, 2 rows
                                                                 -- same stream
     `u8 i; s32 t; t = <expr>; i = t;', test `t'                 137w, 5 rows
     `u16 i' likewise                                            137w, 5 rows
   THE NARROW-`i' SPELLING IS THE INTERESTING NEGATIVE: it CLOSES both rows 37
   and 41 -- the truncating copy survives, cse cannot see a QImode destination
   -- and the five rows it leaves are ALL the same thing,
   `or $v1, $s2, $s5' against the target's `or $v1, $s5, $s2' at the five
   `f | i' sites.  THE REGISTERS ARE RIGHT; ONLY THE OPERAND ORDER IS WRONG,
   and it is combine's, not the source's: a narrow `i' makes `f | i' an
   `(ior (reg f) (zero_extend (reg:QI i)))', and `gen_binary'
   (combine.c:9048, "Put complex operands first and constants second") swaps a
   commutative pair when `GET_RTX_CLASS (op0) == 'o'' and op1's class is not
   'o'.  A ZERO_EXTEND is class '1' and a SUBREG is class 'x', so NOTHING
   short of a full SImode `i' makes op1 class 'o'.  Measured, all 137w/5 rows:
   `i | f', `(s32)i | f', `f | (s32)i', `f | (i | 0)'.  A separate
   `s32 j; j = i;' before the switch with the arms reading `j' is 138w/25.
   So the two residues are mutually exclusive through this lever, and the
   `andi'/`move' pair is the cheaper of the two.

   **IF YOU POINT A PERMUTER AT THIS FUNCTION, POINT IT AT THESE TWO ROWS**,
   and give it the narrow-`i' body as a second seed -- it is 5 rows away in a
   completely different place, and all four sweeps are clean on both.

   ALL FOUR SWEEPS ARE CLEAN ON THIS BODY, cycled after every change:
     widthsweep  no width change helps
     refsweep    16 names, 1378 insertion points, no cancelled pair helps
     devsweep    40 statements, 16 names, 680 candidates/round, no wrapper
     stmtsweep   7 movable assignments, no safe move helps

   ========================================================================
   HOW IT GOT FROM 16 TO 2 (2026-11-22).  The 16-row park filed its residue as
   four clusters of "register names we cannot reach" and nominated the largest
   for the permuter as unreachable from the source.  THREE of the four were one
   mechanism -- local-alloc's `combine_regs' refusing at local-alloc.c:1834 to
   tie a quantity whose pseudo is not block-local -- and the fourth was a
   sched1 head-skip that a user copy of a parameter walks straight past.  None
   of the four was reachable by any of the sweeps, which were clean at 16 rows
   and are clean again now: all four steps below are CONTROL-FLOW or
   DECLARATION changes, not spellings.

   1. 16 -> 12.  `m' MUST NOT EXIST: EACH ARM STORES `q[0]' ITSELF AND ONLY
      `q[3] = 0; q[2] = 0;' IS SHARED.  The park shared BOTH tail blocks,
      through `store:' (`q[0] = m') and `clear:', which makes `m' live in three
      blocks, so `reg_qty[m] == -1' and `combine_regs' refuses to tie the first
      arm's `q[0] + 1' to the `lbu' that feeds it.  The target plainly ties
      them -- `lbu $v0, 0x0($a0)' ... `addiu $v0, $v0, 0x1', one register, in
      place, with `f | i' pushed out to `$v1' -- and note that the SECOND arm
      has `f | i' in `$v0', so the arms do NOT agree on registers, which is
      itself the evidence that each arm is allocated on its own.
      Deleting `m' and writing `q[0] = q[0] + 1;' / `q[0] = a | 0x80;' /
      `q[0] = ((a & 1) << 5) | ~0x7F | (e & 0x1F);' in the three arms gives
      each arm its own block-local quantity, the tie succeeds, and jump2's
      `cross_jump' merges the three now-identical `sb $v0, 0x0($a0)' +
      `clear:' tails back into ONE copy at `.L8009F82C' -- which is what the
      target's `j .L8009F82C' from arms 1 and 2 actually is.  4 rows, 0 words.

      **DO NOT DUPLICATE `q[3] = 0; q[2] = 0;' INTO THE ARMS AS WELL.**  That
      was measured first and it is 140 words / 24 rows.  With the two zero
      stores inside the arm, sched1 hoists them between the `lbu' and the
      `addiu' (independent -- different constant offsets off one base), which
      stretches the tied quantity's live range from 8 half-slots to 12 and
      drops its `QTY_CMP_PRI' (local-alloc.c:1693,
      `floor_log2(n_refs)*n_refs*size/(death-birth)*10000') from 10000 to 6666.
      `f | i' (2 refs over 2 slots = 10000) then outranks it and takes `$v0',
      the tie lands in `$v1', the three tails are no longer identical and
      `cross_jump' cannot merge them -- 3 extra words.  With `clear:' SHARED
      the pair's range ends at its own `sb' and the two quantities TIE at
      10000 exactly, which `qty_compare_1' breaks on quantity NUMBER = birth
      order, and the `lbu' is born first.  The whole 4 rows is that tie-break.

   2. 12 -> 9.  THE TARGET'S SECOND `addiu $v0, $s5, -0x8' IS dbr's, NOT THE
      SOURCE'S -- `s = f - 8;' IS ASSIGNED ONCE, AFTER THE `if'.  The park read
      the two `addiu $v0, $s5, -0x8' insns (8009F71C, in the `bnez' delay slot,
      and 8009F738 at `.L8009F738') as two source assignments and spelled them
      `(s = f - 8, q[0] >= 0x7F)' inside the comma plus `setsw: s = f - 8;' in
      the `else'.  That makes `s' live in two blocks, so `combine_regs' refuses
      to tie the switch's sign-extension to it, and we got
      `addiu $v1, $s5, -8' / `sll $v0, $v1, 16' against the target's
      `addiu $v0, $s5, -0x8' / `sll $v0, $v0, 16' in place.
      Written as ONE statement between the `if' and the `switch', `s' is
      block-local to the switch's own block, the extension ties in place, and
      the word count is STILL 137, because `dbr_schedule' COPIES that insn into
      the `bnez $v0, .L8009F73C' delay slot and advances the branch past
      `.L8009F738' (reorg's `fill_slots_from_thread' with `own_thread' false,
      because `.L8009F738' is also the target of the `beqz' at 8009F6E4, so the
      insn is not deleted).  It is safe to copy because `$v0' is dead on the
      fall-through path -- `.L8009F720' opens `lhu $v0, 0x6($s1)'.  3 rows,
      0 words.  `goto setsw' and the whole `else' block disappear.

   3. 9 -> 19 (DELIBERATELY WORSE).  THE PROLOGUE CLUSTER IS NOT UNREACHABLE:
      USER COPIES OF `f', `a' AND `e' CLOSE ALL SIX ROWS.  The park's residue
      had `sw $s1'/`lui $s1'/`ori $s1' AFTER the `$s5/$s6/$s7' parameter copies
      and the target has them before, and the park derived that as impossible
      because `assign_parms' emits every parameter copy before any user insn.
      (The `sw $sN' saves are placed immediately before the first insn that
      SETS $sN -- this compiler saves lazily -- so the only thing that actually
      moves is the `la'.)  What the derivation missed is the OTHER half of
      sched.c:3251: the head-skip protects the LEADING RUN of
      `(set pseudo hard_reg)' insns by setting `INSN_REF_COUNT = 1', and
      combine merges a parameter's own hard-register copy into a user copy and
      DELETES the parameter copy (see "A USER COPY OF A PARAMETER IS THE ONLY
      WAY TO GIVE ITS HARD-REG COPY A LATER LUID" in docs/matching-levers.md).
      Renaming the parameters to `f0/a0/e0' and writing `f = f0; a = a0;
      e = e0;' AFTER `g = (u8 *)0x80013710;' therefore leaves `$t0 = $a0' as
      the whole protected run, and the `la' -- priority 1, which the BACKWARD
      scheduler places first in the block -- lands immediately after it.
      Rows 2/3/4 match from that moment on.
      It cost 10 rows at first, because it is a global-alloc permutation:
        9-row body  `;; 12 regs to allocate: 84 91 87 176 92 86 88 89 81 82 80 83'
                    f/a/e = 81/82/83 in $s5/$s6/$s7, i = 86 in $s2.  CORRECT.
        user copies `;; 12 regs to allocate: 84 91 87 179 92 93 86 88 89 94 95 80'
                    f = 93 allocated SIXTH, takes $s2; i -> $s3, c -> $s4,
                    n -> $s5.  13 of the 19 rows are that one renaming.
      Measured permutations that do NOT fix it: copy order `e,a,f' (21);
      declaration order `e,a,f' (19); `f = f0' before `g' with a/e after (19);
      all three copies before `g' (19); only `f' a user copy (22).

   4. 19 -> 2.  ONE CANCELLED PAIR ON THE PARAMETER `f0' IN THE LAST SWITCH
      ARM, found by `refsweep.py --body' re-run in step 3's basin:

          f0++;
          f0--;      -- immediately before
                     q[0] = ((a & 1) << 5) | ~0x7F | (e & 0x1F);

      and it does TWO things at once, which is why it is worth 17 rows:
        * it gives `f0' references AFTER the user copy, so combine can no
          longer delete `f0''s own pseudo.  It survives as its own allocno and
          `combine_regs' ties it to the incoming hard register: `.greg' gains
          `81 in 5' -- `$a1' -- and combine propagates it into the narrow test,
          which is exactly the `sll $v0, $a1, 16' the old park's cluster C had
          filed as a second pseudo for `f' that it could not create.  So the
          pair closes CLUSTER C as well.
        * with `f0' covering the range from entry, `f''s own allocno is priced
          over a shorter span and `allocno_compare' sorts it back BEHIND `i',
          `c' and `n': `;; 13 regs to allocate: 84 91 87 179 92 86 88 89 93 94
          95 80 81', so i -> $s2, c -> $s3, n -> $s4, f -> $s5, a -> $s6,
          e -> $s7.  Every one of the 13 renaming rows closes at once.

      **THE HONEST ALTERNATIVES WERE PRICED FIRST AND THEY DO NOT REACH 2.**
      The pair's only job is to reference `f0' late, and a real use of `f0' at
      one of the seven `f' sites does the same thing for free.  All 128 subsets
      of {narrow test, `f - 8', the five `f | i' sites} reading `f0' instead of
      `f' were measured: the best is 4 ROWS (any single one of the four
      `q[1] = f | i' / `c = f | i' sites alone, or that plus the narrow test),
      `s = f0 - 8;' alone is 5, and no substitution is 19.  Adding any
      substitution ON TOP of the pair is 2 or 3, never better.  So the pair is
      worth 2 rows over the best device-free spelling, and it is a cancelled
      increment pair -- the project's standard zero-cost reference lever, which
      combine folds to nothing -- not an `asm' device.  If you would rather not
      ship it, `q[1] = f0 | i;' in the last arm with the pair removed is
      137 words / 4 rows.

   THE SHAPE TO CARRY AWAY, twice over: a repeated insn in a listing is not
   necessarily a repeated statement -- dbr COPIES and cross_jump MERGES -- and
   before pricing a register residue as unreachable, ask of every local in it
   HOW MANY BLOCKS assign or read it and whether the target needs it to be one.
   And a cluster whose derivation says "the source cannot reach this" is worth
   one measurement anyway: this park's six-row cluster A had a correct
   derivation about `assign_parms' and a wrong conclusion.

   DEVICES PRESENT, AND WHY
   The `q++; q--;' between the two tests is load-bearing and must not be
   removed: it invalidates cse2's entry for `(mem (plus (reg 84) 1))' so the
   third read of `q[1]' is a real `lbu $v1, 0x1($a0)' again.  cse1 does not
   common it (the old park's `do { v = q[1]; } while (0);' put a
   NOTE_INSN_LOOP_END between the two reads and `cse_end_of_basic_block' breaks
   on that note while `after_loop' is 0, cse.c:8102); cse2 runs with
   `after_loop' 1 (toplev.c:3307 vs :3376), ignores the note, and commons it.
   `q++; q--;' sets `reg 84' twice, which invalidates the entry, and compiles
   to nothing.  Removing it was 27 rows / 136 words on the 16-row body.
   The `do { } while (0)' around `v = q[1]' is worth NOTHING and is gone.
   `g++; g--;' before `if (v != 0)' was worth 2 rows on the 24-row body and is
   a LOSS now: it makes `g' a two-set pseudo so `update_equiv_regs'
   rematerialises the 0x80013710 constant a second time inside the body.

   THINGS MEASURED AND REJECTED THAT ARE WORTH NOT REPEATING
     * `*(volatile u8 *)(q + 1)' for the third read, and
       `__asm__ __volatile__("" ::: "memory")' between the two tests: one row
       better than the cancelled pair on the 29-row body, but each leaves a
       spurious `nop', and both are devices this project avoids.
     * A duplicated fail-body per test (two copies or three): 29/137 with the
       `g' device, 40/134 without -- cross-jumping merges them back and cse
       follows the branch anyway (`cse_end_of_basic_block' takes the branch
       whenever `LABEL_NUSES == 1' and the label is preceded by a BARRIER,
       cse.c:8154, which a fail-body ending in a `goto' always is).
     * `s32/u8/u16/s8 v' for the first read: all identical to `u8 v'.
     * Reading `v' versus `q[1]' at each of the three test sites: all eight
       combinations are the same stream.
     * `(s16)f != (q[1] & 0xF8)' (operand order): 27 on the 23-row body.
     * Swapping `i = ...' and `v = q[1]': 51-56 everywhere it was tried.
     * On the user-copy body, raising `i'/`c'/`n' instead of lengthening `f':
       `i++; i--;' after `t = i;' 138w/41; doubled 138w/41;
       `c++; c--; n++; n--;' after `n = (a & 1) << 5;' 137w/35; both 138w/38.
       `floor_log2(6)*6 = 12' goes to `floor_log2(8)*8 = 24', so a pair on a
       six-reference quantity overshoots -- the lever is LENGTHENING `f'.
     * `q[0] = m | ~0x7F | (e & 0x1F)' through a `u8 m': the QImode `ior'
       collapses `~0x7F' into `ori $v0, $v0, 0x80'.  The target does it in
       SImode (`addiu $v0, $zero, -0x80'), which is what the arms' own
       `((a & 1) << 5) | ~0x7F | (e & 0x1F)' gives for free now that `m' is
       gone.  THE WIDTH OF THE FIRST OPERAND decides it, not the destination.
     * The redundant `(u16)' cast in `i = ((u16)*(u16 *)(p + 0x2A) >> 9) & 7;'
       is a no-op (2 rows either way) and is dropped, because clang-format
       rewrites `(u16)*(u16 *)' to `(u16) * (u16 *)' and format-on-save would
       then churn the file.
*/
void func_8009F644(u8 *p, s32 f0, s32 a0, s32 e0)
{
    u8 *q;
    u8 *r;
    s32 i;
    u8 t;
    s32 c;
    s32 n;
    s16 s;
    u8 v;
    u8 *g;
    s32 f;
    s32 a;
    s32 e;

    g = (u8 *)0x80013710;
    f = f0;
    a = a0;
    e = e0;
    if ((D_800E296C & 0x10000000) == 0 && (*(u16 *)(g + 0x4) & 1) == 0 &&
        *(u16 *)(g + 0x6) < 0xF01)
    {
        q = *(u8 **)(g + 0xC) + *(u16 *)(g + 0x6) * 2;
        i = (*(u16 *)(p + 0x2A) >> 9) & 7;
        t = i;
        v = q[1];
        if (v != 0)
        {
            if ((v & 7) != t)
                goto fresh;
            q++;
            q--;
            if ((q[1] & 0xF8) != (s16)f || q[0] >= 0x7F)
            {
            fresh:
                q += 2;
                *(u16 *)(g + 0x6) += 1;
                q[1] = 0;
                q[0] = 0;
            }
        }
        s = f - 8;
        switch (s)
        {
        case 0x0:
        case 0x8:
        case 0x10:
        case 0x18:
        case 0x20:
        case 0x58:
            q[1] = f | i;
            q[0] = q[0] + 1;
            goto clear;
        default:
            goto clear;
        case 0x28:
        case 0x38:
        case 0xA0:
            q[1] = f | i;
            q[0] = a | 0x80;
            goto clear;
        case 0x40:
            q[1] = f | i;
            q[0] = a | 0x80;
            q[2] = e;
            q[3] = 0;
            q += 2;
            *(u16 *)(g + 0x6) += 2;
            goto clear;
        case 0x68:
        case 0x70:
        case 0x78:
        case 0x88:
            r = q;
            c = f | i;
            n = (a & 1) << 5;
            q = func_8009F9E8(c & 0xFF, n);
            if (r != q)
            {
                *(u16 *)(g + 0x6) -= 1;
                q[1] = c;
                q[0] = n | ~0x7F | (e & 0x1F);
                return;
            }
        case 0x48:
        case 0x50:
        case 0x60:
        case 0x80:
        case 0x90:
        case 0x98:
            q[1] = f | i;
            f0++;
            f0--;
            q[0] = ((a & 1) << 5) | ~0x7F | (e & 0x1F);
        clear:
            q[3] = 0;
            q[2] = 0;
            break;
        }
    }
}
