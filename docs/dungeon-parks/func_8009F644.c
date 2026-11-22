/* PARK: func_8009F644 -- 137/137 words, 16 ALIGNED DIFFERENCES (was 41)

   Measured with
     python3 tools/decomp/parkresidue.py func_8009F644
   and reproduced byte for byte on a tools/decomp/onefunc.py standalone.

   STILL BLOCKED ON THE THIRD RODATA CARVE.  It owns `jtbl_80088D7C' at rodata
   offset 0x61c, which is 4 mod 8, so landing it inside the HEAD unit pads the
   table and the container comes out eight bytes long.  That is a splat change,
   not a body problem; `parkresidue'/`aligndiff'/`ovlverify' all measure against
   the listing and are unaffected.

   WHAT IT IS.  The message/damage-number queue push.  Bails out if
   `D_800E296C & 0x10000000', if `*(u16 *)0x80013714 & 1', or if the queue is
   full at 0xF01 entries; otherwise it indexes the queue by the count at +0x6,
   decides whether to overwrite the last entry or append, and fills one of six
   shapes chosen by a jump table on `f - 8'.

   THE SIGNATURE IS FIXED BY THE UNIT, NOT BY THE LISTING.
   csrc/ovl_dungeon_decls.h declares `void func_8009F644(u8 *, s32, s32, s32);'
   and four landed call sites rely on it, so the definition must be
   `(u8 *p, s32 f, s32 a, s32 e)'.  The narrowing is AT THE USE:
   `(q[1] & 0xF8) != (s16)f' buys the target's `sll'/`sra' pair.  Defining it
   with `s16 f' under an `__asm__("func_8009F644")' alias WAS measured (it is
   the only legal way to narrow it): 137 words but 36 rows, because every
   `f | i' then pays its own `sll 16'/`sra 16'.  Do not take that route.

   DECLARATIONS
   NONE -- func_8009F9E8 rides an implicit declaration, and D_800E296C and the
   existing func_8009F644 prototype are already in the header.
   END DECLARATIONS

   ========================================================================
   HOW IT GOT FROM 41 TO 16 (2026-11-22).  Each step was measured on the body
   that preceded it; several of them are stale-negative reversals, so RE-ABLATE
   EVERYTHING after any change rather than trusting the list below.

   1. 41 -> 34 (137w -> 136w).  THE SHARED TAIL BELONGS AT THE END OF THE
      SWITCH, NOT IN THE FIRST ARM.  `.L8009F82C' (`q[0] = m') is target index
      122 and `.L8009F830' (`q[3] = 0; q[2] = 0') is 123 -- after every case
      body -- and the LAST arm (`.L8009F80C') FALLS THROUGH into them.  The
      park had `store:'/`clear:' inside the FIRST arm with every other arm
      jumping back, which is the same word count for the arms but costs an
      extra `j' in the last one and 9 rows of block order.  Read off the
      listing's ADDRESSES, not from a word count.

   2. 34 -> 32 (136w -> 135w).  `qo = 0xC;' / `*(u8 **)(g + qo)' WAS BACKWARDS.
      The target's insn is `lw $a0, 0xC($s1)' -- the FOLDED displacement off
      `g'.  Through an `s16 qo' local gcc constant-folds `g + qo' to the
      absolute 0x8001371C and emits `lui'/`lw'.  The park's header claimed the
      opposite and called it "THE MISSING WORD"; it was an extra one.

   3. 32 -> 31.  The 0x68 arm's `q[0] = m | ~0x7F | (e & 0x1F)' reordered to
      `(e & 0x1F) | m | ~0x7F' -- superseded by step 6, which removes the
      cluster entirely.

   4. 31 -> 24 (135w -> 136w).  THE MISSING `lbu $v1, 0x1($a0)' IS A cse2
      COMMONING.  `tools/decomp/dump.sh' on the standalone shows insn 89 -- the
      third read of `q[1]', the one under `& 0xF8' -- still a
      `(mem/s:QI (plus (reg 84) (const_int 1)))' in `.cse' and `.loop' and
      `(set (reg 116) (reg/v:QI 90))' in `.cse2'.  cse1 does NOT common it,
      because the park's `do { v = q[1]; } while (0);' puts a
      NOTE_INSN_LOOP_END between the two and `cse_end_of_basic_block' breaks on
      that note while `after_loop' is 0 (cse.c:8102); cse2 runs with
      `after_loop' 1 (toplev.c:3307 passes 0, toplev.c:3376 passes 1),
      ignores the note, and commons it there.  cse2's scan
      stops only at a CODE_LABEL (cse.c:8087,
      `while (p && GET_CODE (p) != CODE_LABEL)'), and a label put there by a
      duplicated fail-body is followed anyway: `cse_end_of_basic_block' takes
      the branch whenever `LABEL_NUSES == 1' and the label is preceded by a
      BARRIER (cse.c:8154), which a fail-body ending in `goto setsw' always is.

      THE CURE IS TO INVALIDATE THE POINTER, NOT TO MAKE A LABEL.  Split the
      first `||' operand out (byte-identical on its own) and put a CANCELLED
      PAIR on `q' between the two tests.  `q++; q--;' sets `reg 84' twice, so
      cse's entry for `(mem (plus (reg 84) 1))' is invalidated and the third
      read is a real `lbu' again.  It compiles to nothing.

   5. 24 -> 20 (136w -> 137w).  A SECOND, NARROW LOCAL FOR `i'.  Keep
      `s32 i;' and add `u8 t;' with `i = <expr>; t = i;', testing `t':
      the QImode truncation is a copy cse cannot see and jump2 keeps, so the
      `beqz' delay slot gets a real `move' and the word count is right.

   6. 20 -> 16.  THE 0x68 ARM'S `(a & 1) << 5' NEEDS ITS OWN `s32' LOCAL, NOT
      `m'.  `m' is `u8' (the earlier permuter find, worth 39 rows), so
      `q[0] = m | ~0x7F | ...' is a QImode `ior' and `~0x7F' collapses into
      `ori $v0, $v0, 0x80'.  The target does the whole thing in SImode --
      `andi $v1, $s7, 0x1F' as the block's FIRST insn (reorg then steals it for
      the `beq' delay slot), `addiu $v0, $zero, -0x80', `or $v0, $s4, $v0',
      `or $v0, $v0, $v1'.  `s32 n;' / `n = (a & 1) << 5;' /
      `q[0] = n | ~0x7F | (e & 0x1F);' is the whole 4-row cluster.  THE WIDTH
      OF THE FIRST OPERAND IS WHAT DECIDES IT, not the destination: assigning
      through a `u8' variable first is still QImode (21).

   DEVICE ABLATION, POWER SET, ON THE CURRENT BODY (2x3 = 6 measurements):
     do-while on `v = q[1]'  x  {split+`q++;q--;', split only, joined `||'}
       dw   split+pair  16/137     --   split+pair  16/137   <- shipped
       dw   split       27/136     --   split       27/136
       dw   joined      27/136     --   joined      27/136
   So the `q' pair is worth 11 rows and THE `do { } while (0)' IS NOW WORTH
   NOTHING -- it was worth 1 row on the 42-row body and is removed here.
   `v++; v--;' at the same point as the `q' pair measures the same;
   `i++; i--;' there is 46.  `g++; g--;' before `if (v != 0)' was worth 2 rows
   on the 24-row body and is a LOSS now: it makes `g' a two-set pseudo so
   `update_equiv_regs' rematerialises the 0x80013710 constant a second time
   inside the body, which is the right word count for the wrong reason.

   BLOCK ORDER vs THE LISTING: checked by pairing branches by ordinal and
   comparing drift at each branch with drift at its destination.  After step 1
   every block is at its target ordinal; the three `bnez <epilogue>' rows that
   appeared at 24 rows were pure word-count drift and closed by themselves.

   ALL FOUR SWEEPS COME BACK EMPTY ON THIS BODY, cycled after every landing:
     widthsweep  no width change helps
     refsweep    1205 insertion points, no cancelled pair helps
     devsweep    540 candidates/round, no wrapper helps
     stmtsweep   5 movable assignments, no safe move helps

   ========================================================================
   WHAT IS LEFT: 16 ROWS, 0 WORDS, IN FOUR CLUSTERS.  All sixteen are register
   names or prologue order; the instruction stream is otherwise the target's.

   A. 6 rows -- THE PROLOGUE EMITS `sw $s1'/`la $s1' AFTER THE `$s5/$s6/$s7'
      PARAMETER COPIES, the target before them.  Derived, not swept:
      sched2's block 0 has every one of these insns at priority 1
      (`.sched2' prints it), `rank_for_schedule' (sched.c) breaks a priority
      tie first on class-relative-to-last_scheduled_insn and then on
      INSN_LUID, and the emission order we get is exactly LUID-ascending:
      insn 4 (`$t0 = $a0'), 6, 8, 10, then 382/383 (the `la', split into
      `lui'/`ior' -- greg shows them as two insns with 383 depending on 382).
      For the target's order the `la' would need a LUID between the FIRST and
      SECOND parameter copy.  It cannot get one: `assign_parms' emits all four
      copies before any user insn, and sched.c:3251 ("At the start of a
      function, before reload has run, don't delay getting parameters from hard
      registers into pseudo registers") walks the LEADING RUN of
      `(set pseudo hard_reg)' insns and sets `INSN_REF_COUNT = 1' on each so
      sched1 can never move anything across them (sched.c:3250) -- sched1's
      own dump confirms
      block 0 starts at insn 382.  Measured and rejected, all 16 rows: `g'
      declared first/second/last, `u8 *g = (u8 *)0x80013710;' as an
      initialiser, `u16 *g', `(u8 *)(0x80013700 + 0x10)', a two-local
      `h = ...; g = h;', and stmtsweep's own moves.  Assigning `g' inside the
      `&&' chain is 19.  I believe this cluster is unreachable from the source
      and is the one thing a permuter run should be pointed at.

   B. 2 rows -- THE `i' COPY IS THE RIGHT WORD IN THE WRONG DIRECTION.  We emit
      `andi $s2, $v1, 0x7' / `move $v1, $s2'; the target has
      `andi $v1, $v1, 0x7' / `addu $s2, $v1, $zero'.  Every `t = <expr>;
      i = t;' ordering measures 24/136 -- five widths (u8/u16/s8/s16/s32) x
      three positions for the copy -- because the WIDENING is always folded and
      the two pseudos merge; only the truncating direction keeps a copy
      (cse cannot see a QImode destination, docs/matching-levers.md:2337).
      `s16 i' alone also produces the target's two insns exactly (22 rows) but
      costs 6 rows of `or' operand order: a narrow `i' makes every `f | i' an
      `(ior (reg f) (sign_extend (reg:HI i)))' and combine.c:9048's `gen_binary'
      swaps a commutative pair when op0 is class 'o' and op1 is not, so the
      extend goes first and we get `or $v0, $s2, $s5' at five arm sites.
      `i | f', `f | (s32)i', `(s32)i | f', `f | (u16)i' are all identical --
      the swap is combine's, not the source's.

   C. 4 rows -- `(s16)f' READS `$s5' WHERE THE TARGET READS `$a1', and `s'
      (the `f - 8' temp) is in `$v1' where the target has `$v0'
      (`addiu $v0, $s5, -0x8' / `sll $v0, $v0, 16' in place).  The `$a1' read
      means the target has a SECOND pseudo for `f', live from entry to that one
      insn, crossing no call and therefore taking the lowest free register --
      `$v0', `$v1' and `$a0' are all busy in that block, so `$a1'.  Rejected:
      `s32 ff = f;' used at the narrow site, at the `f - 8' site, and at the
      `f | i' sites (16, 16, 32); `u16 s' with `switch ((s16)s)' (16);
      `s32 s' with `switch ((s16)s)' (16); hoisting `s = f - 8;' out of the
      comma into its own statement before the second test (18).

   D. 4 rows -- THE FIRST SWITCH ARM'S TWO REGISTERS ARE SWAPPED.  Target:
      `lbu $v0, 0x0($a0)' / `or $v1, $s5, $s2' / `sb $v1, 0x1($a0)' /
      `addiu $v0, $v0, 0x1' -- q[0] and `m' are ONE quantity and the increment
      is in place.  Ours has `f | i' in `$v0' and q[0] in `$v1'.  Rejected:
      `m = q[0]; m++;' and `m += 1;' (19), the two statement orders (16, no
      change), a `c = f | i;' temp (33), a `u8 t'/`s32 n' temp for q[0] (16,
      37), `s32 m' (43), and `m' declared first/last (16).

   THINGS MEASURED AND REJECTED THAT ARE WORTH NOT REPEATING
     * `*(volatile u8 *)(q + 1)' for the third read, and
       `__asm__ __volatile__("" ::: "memory")' between the two tests: both 23
       rows / 137 words on the 29-row body -- one row better than the cancelled
       pair there, but each leaves a spurious `nop', and both are devices this
       project avoids.  The cancelled pair reaches 16 and they do not.
     * A duplicated fail-body per test (two copies or three): 29/137 with the
       `g' device, 40/134 without -- cross-jumping merges them back and cse
       follows the branch anyway (see cluster A's citation).
     * `s32/u8/u16/s8 v' for the first read: all identical to `u8 v'.
     * Reading `v' versus `q[1]' at each of the three test sites: all eight
       combinations are the same stream.
     * `(s16)f != (q[1] & 0xF8)' (operand order): 27 on the 23-row body.
     * Swapping `i = ...' and `v = q[1]': 51-56 everywhere it was tried.
*/
void func_8009F644(u8 *p, s32 f, s32 a, s32 e)
{
    u8 *q;
    u8 *r;
    s32 i;
    u8 t;
    s32 c;
    u8 m;
    s32 n;
    s16 s;
    u8 v;
    u8 *g;

    g = (u8 *)0x80013710;
    if ((D_800E296C & 0x10000000) == 0 && (*(u16 *)(g + 0x4) & 1) == 0 &&
        *(u16 *)(g + 0x6) < 0xF01)
    {
        q = *(u8 **)(g + 0xC) + *(u16 *)(g + 0x6) * 2;
        i = ((u16)*(u16 *)(p + 0x2A) >> 9) & 7;
        t = i;
        v = q[1];
        if (v != 0)
        {
            if ((v & 7) != t)
                goto fresh;
            q++;
            q--;
            if ((q[1] & 0xF8) != (s16)f || (s = f - 8, q[0] >= 0x7F))
            {
            fresh:
                q += 2;
                *(u16 *)(g + 0x6) += 1;
                q[1] = 0;
                q[0] = 0;
                goto setsw;
            }
        }
        else
        {
        setsw:
            s = f - 8;
        }
        switch (s)
        {
        case 0x0:
        case 0x8:
        case 0x10:
        case 0x18:
        case 0x20:
        case 0x58:
            q[1] = f | i;
            m = q[0] + 1;
            goto store;
        default:
            goto clear;
        case 0x28:
        case 0x38:
        case 0xA0:
            q[1] = f | i;
            m = a | 0x80;
            goto store;
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
            m = ((a & 1) << 5) | ~0x7F | (e & 0x1F);
        store:
            q[0] = m;
        clear:
            q[3] = 0;
            q[2] = 0;
            break;
        }
    }
}
