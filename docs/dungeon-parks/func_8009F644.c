/* LANDED 2026-11-22 -- func_8009F644: 137/137 words, 0 differing.
   Verified `python3 tools/decomp/ovlverify.py csrc/ovl_dungeon_8009F644.c
   func_8009F644' -> "target 137 words, candidate 137 words, 0 differing", and
   all 82 landed functions of the unit re-verified at 0 differing in the same
   copy.  THIS FILE IS THE PRE-MERGE SAFE COPY; delete it in the merge commit.

   WHAT IT IS.  The message/damage-number queue push.  Bails out if
   `D_800E296C & 0x10000000', if `*(u16 *)0x80013714 & 1', or if the queue is
   full at 0xF01 entries; otherwise it indexes the queue by the count at +0x6,
   decides whether to overwrite the last entry or append, and fills one of six
   shapes chosen by a jump table on `f - 8'.

   It owns `jtbl_80088D7C' at rodata offset 0x61c, 4 mod 8, which is why it
   needed its own translation unit -- csrc/ovl_dungeon_8009F644.c, whose rodata
   STARTS at that table so cc1psx's `.align 3' is a no-op.  That carve landed
   in fb2d885; this park was blocked on it and is not blocked any more.

   THE SIGNATURE IS FIXED BY THE UNIT, NOT BY THE LISTING.
   csrc/ovl_dungeon_decls.h declares `void func_8009F644(u8 *, s32, s32, s32);'
   and four landed call sites rely on it.  The parameters are
   `(u8 *p, s32 f0, s32 a0, s32 e0)' with `s32 f, a, e;' user copies -- names
   only; types and order are unchanged, so no header edit is needed.  Do NOT
   narrow a parameter in the definition: `s16 f' under an
   `__asm__("func_8009F644")' alias measured 137 words / 36 rows, because every
   `f | i' then pays its own `sll 16'/`sra 16'.  The narrowing belongs AT THE
   USE -- `(q[1] & 0xF8) != (s16)f' is what buys the target's `sll'/`sra' pair.

   DECLARATIONS
   NONE -- func_8009F9E8 rides an implicit declaration, and D_800E296C and the
   func_8009F644 prototype are already in csrc/ovl_dungeon_decls.h.
   END DECLARATIONS

   ========================================================================
   THE LAST TWO ROWS, AND THE ONE-LINE FIX (2026-11-22)

     R 37  8009F6D8  andi s2,v1,0x7   vs  andi $v1, $v1, 0x7
     R 41  8009F6E8  move v1,s2       vs  addu $s2, $v1, $zero

   THE COPY WAS THE RIGHT WORD IN THE WRONG DIRECTION, and the direction the
   target wants is `t = <expr>; i = t;'.  Writing it that way had always cost
   a WORD -- 136 words / 6 rows, of which 3 are only branch-offset collateral
   from being one word short -- because the copy disappeared entirely.  The fix
   is ONE EXTRA MENTION OF `t', ANYWHERE AFTER `i''S LAST MENTION:

       t = (*(u16 *)(p + 0x2A) >> 9) & 7;      -- `andi $v1, $v1, 0x7', in place
       i = t;                                  -- `addu $s2, $v1, $zero'
       ...
       clear:
           t++;                                -- the late mention
           t--;

   WHY, READ OFF THE DUMPS (tools/decomp/dump.sh on an onefunc.py standalone;
   compared the no-pair body and this one pass by pass).  `.rtl' and `.jump'
   are IDENTICAL in both:

       (insn 69 ... (set (reg/v:SI 87) (zero_extend (subreg:HI (reg:SI 116)))))
       (insn 72 ... (set (reg/v:SI 86) (reg/v:SI 87)))      ; i = t
       (jump_insn 85 ... (ne:SI (reg:SI 119) (reg/v:SI 87)))  ; (v&7) != t

   By `.cse', WITHOUT the late mention, insn 69 writes **86**, insn 72 is GONE
   from the chain, and jump 85 reads **86**.  WITH it, all three are unchanged.
   So it is cse1 that destroys the copy, not combine (the previous park header
   blamed combine; that was wrong, and the dumps say so).  Two cse mechanisms
   in series:

     * `make_regs_eqv' (cse.c:838) gives `qty_first_reg' to the register whose
       LAST use is later -- cse.c:866-869,
       `uid_cuid[regno_last_uid[new]] > uid_cuid[regno_last_uid[firstr]]'.
       With no late mention of `t', `i' wins that test (its last use is a
       `f | i' in the last switch arm) and heads the quantity.
     * then cse.c:7499, "Special handling for (set REG0 REG1) where REG0 is the
       cheapest", gated EXACTLY on
       `qty_first_reg[reg_qty[REGNO (SET_SRC)]] == REGNO (SET_DEST)':
       it rewrites the PREVIOUS insn's SET_DEST to `i', turns the copy into a
       dead `t = i' and drops it.  That is the missing word.

   A later mention of `t' makes the first test fail, cse.c:7499 never fires,
   `t' keeps two uses and the copy is emitted.  THE PREDICTION IS A CLIFF AT
   `i''S LAST MENTION (the last arm's `q[1] = f | i;'), AND IT MEASURES AS ONE:

       t++; t-- at `clear:'                         137w,  0 rows   <-- shipped
       t++; t-- after the switch, end of the `if'    137w,  0 rows
       t++; t-- in the last arm, AFTER `q[1]=f|i'    137w,  0 rows
       t--; t++ at `clear:' (reversed)               137w,  0 rows
       t++; t-- after `q[2] = 0;'                    137w,  0 rows
       t++; t-- in the last arm, BEFORE `q[1]=f|i'   136w,  6 rows
       t++; t-- after the 0x68 arm's `c = f | i;'    136w,  6 rows
       t++; t-- after the 0x40 arm's `q[1]=f|i'      136w,  6 rows
       t++; t-- immediately after `i = t;'           136w,  6 rows
       no mention at all                             136w,  6 rows

   `t' MUST BE `s32'.  With the late mention present and `t' declared u8, u16,
   s8 or s16 it is 136 words / 6 rows in every case: a narrow `t' makes `i = t'
   an extension rather than a reg-reg copy, cse.c:7499 wants
   `GET_CODE (SET_SRC) == REG', and what kills it instead is the extension
   being combined away.  (This is also why the OLD park's `u8 i' variant --
   137w/5, see below -- closed these two rows: a QImode destination is a
   different escape from the same place.)

   The test may read either end: `(v & 7) != t' and `(v & 7) != i' both measure
   137w / 0 rows, because once `t' heads the quantity cse canonicalises the
   test's operand to `t' either way.  `t' is shipped, since that is the value
   the statement computes.

   RELATED CATALOGUE ENTRY, already there: "cse CANONICALISES A QUANTITY TO
   WHICHEVER REGISTER IS USED LATER" (docs/matching-levers.md) states the
   `make_regs_eqv' half and the cure ("give the other variable any later
   reference at all").  What is new here is the SECOND half -- cse.c:7499 is
   what actually deletes the copy and rewrites the producing insn's destination,
   so the symptom is a MISSING WORD and a transposed `and', not just a wrong
   register name -- and the measured positional cliff above.

   ========================================================================
   THE TWO DEVICES, AND WHAT THE HONEST ALTERNATIVES COST

   Three cancelled pairs are present.  Each one was priced against every
   device-free spelling of the same job ON THIS BODY, not on an older one.

   1. `q++; q--;' between the two `q[1]' tests -- LOAD-BEARING, DO NOT REMOVE.
      It invalidates cse2's entry for `(mem (plus (reg 84) 1))' so the third
      read of `q[1]' is a real `lbu $v1, 0x1($a0)' again.  cse1 does not common
      it; cse2 runs with `after_loop' 1 (toplev.c:3307 vs :3376), ignores the
      NOTE_INSN_LOOP_END a `do { } while (0)' would plant, and commons it.
      `q++; q--;' sets `reg 84' twice, which invalidates the entry, and
      compiles to nothing.  Removing it was 27 rows / 136 words.

   2. `t++; t--;' at `clear:' -- the cse1 canonicality lever above.
      Device-free attempts, all measured on this body:
        `q[1] = f | t;' in the last arm (a real late use of `t')  137w, 18 rows
        `q[1] = f0 | t;' there (one statement doing BOTH jobs)    138w, 32 rows
      so there is no free late reference to `t'; the pair is worth 6 rows (and
      a word) over no device.

   3. `f0++; f0--;' in the last switch arm -- the allocation lever the 19->2
      step turned on (see the history below).  ALL 128 SUBSETS of the seven
      `f' use sites reading `f0' instead of `f' were re-measured on THIS body
      with the pair removed; the best is 2 ROWS:
        any ONE of the four `q[1] = f | i' / `c = f | i' sites 1-4   2 rows
        that plus the narrow test                                   2 rows
        `s = f0 - 8;' alone                                         3 rows
        any two of sites 1-4                                        3 rows
        the narrow test alone                                      18 rows
        the LAST arm's `q[1] = f0 | i;'                            22 rows
        no substitution at all                                     18 rows
      So the pair is worth 2 rows over the best device-free spelling, same as
      the old park measured on the old body.  IF YOU WOULD RATHER NOT SHIP IT,
      `q[1] = f0 | i;' in the FIRST switch arm (case 0x0..0x58) with the pair
      removed is 137 words / 2 rows -- but it does not land.

   ONE DEVICE INSTEAD OF TWO, if that is preferred: a single cancelled pair
   that mentions both names does both jobs and also lands --
        `t += f0; t -= f0;' at `clear:', both pairs removed        137w, 0 rows
        `f0 += t; f0 -= t;' at `clear:', both pairs removed        137w, 0 rows
   Shipped is the two-pair version, because `v++; v--;' is the project's
   standard zero-cost reference lever and each pair's mechanism is separately
   derived and separately checked; the combined form makes `t' look as if it
   depended on `f0'.

   NOTHING HERE READS AN UNINITIALISED LOCAL.  `t' is assigned unconditionally
   before the switch, in the same `if' body as `clear:'; `f0' is a parameter;
   `q' is assigned at the top of the body.  No duplicated-arm device is used,
   so the gate-2 condition that bit func_800ADE74 and func_800C2824 does not
   arise.

   ========================================================================
   HOW IT GOT FROM 16 TO 2 (2026-11-22, previous agent) -- KEEP ALL OF THIS,
   every step is still load-bearing in the body above.

   1. 16 -> 12.  `m' MUST NOT EXIST: EACH ARM STORES `q[0]' ITSELF AND ONLY
      `q[3] = 0; q[2] = 0;' IS SHARED.  Sharing a `store:' block too makes `m'
      live in three blocks, so `reg_qty[m] == -1' and `combine_regs'
      (local-alloc.c:1834) refuses to tie the first arm's `q[0] + 1' to the
      `lbu' that feeds it.  The target plainly ties them -- `lbu $v0, 0x0($a0)'
      ... `addiu $v0, $v0, 0x1', one register, in place, with `f | i' pushed
      out to `$v1' -- and the SECOND arm has `f | i' in `$v0', so the arms do
      NOT agree on registers, which is itself the evidence that each arm is
      allocated on its own.  With `m' gone, jump2's `cross_jump' merges the
      three now-identical `sb $v0, 0x0($a0)' + `clear:' tails back into ONE
      copy at `.L8009F82C', which is what the target's `j .L8009F82C' from arms
      1 and 2 actually is.  4 rows, 0 words.

      **DO NOT DUPLICATE `q[3] = 0; q[2] = 0;' INTO THE ARMS AS WELL.**  140
      words / 24 rows.  Inside the arm, sched1 hoists the two zero stores
      between the `lbu' and the `addiu' (independent -- different constant
      offsets off one base), stretching the tied quantity's live range from 8
      half-slots to 12 and dropping its `QTY_CMP_PRI' (local-alloc.c:1693)
      from 10000 to 6666.  `f | i' (2 refs over 2 slots = 10000) then outranks
      it and takes `$v0', the tie lands in `$v1', the three tails are no longer
      identical and `cross_jump' cannot merge them -- 3 extra words.  With
      `clear:' SHARED the pair's range ends at its own `sb', the two quantities
      TIE at 10000 exactly, `qty_compare_1' breaks on quantity NUMBER = birth
      order, and the `lbu' is born first.  The whole 4 rows is that tie-break.

   2. 12 -> 9.  THE TARGET'S SECOND `addiu $v0, $s5, -0x8' IS dbr's, NOT THE
      SOURCE'S -- `s = f - 8;' IS ASSIGNED ONCE, AFTER THE `if'.  Two source
      assignments (a comma in the condition plus a `setsw:' label in the
      `else') make `s' live in two blocks, so `combine_regs' refuses to tie the
      switch's sign-extension to it: `addiu $v1, $s5, -8' / `sll $v0, $v1, 16'
      against the target's in-place pair.  As ONE statement between the `if'
      and the `switch', `s' is block-local to the switch's block, the extension
      ties in place, and the word count is STILL 137 because `dbr_schedule'
      COPIES that insn into the `bnez $v0, .L8009F73C' delay slot and advances
      the branch past `.L8009F738' (`fill_slots_from_thread', `own_thread'
      false, because `.L8009F738' is also the target of the `beqz' at
      8009F6E4, so the insn is not deleted).  Safe to copy because `$v0' is
      dead on the fall-through path -- `.L8009F720' opens `lhu $v0, 0x6($s1)'.
      3 rows, 0 words.  `goto setsw' and the whole `else' block disappear.

   3. 9 -> 19 (DELIBERATELY WORSE, and step 4 is what pays it back).
      THE PROLOGUE CLUSTER IS NOT UNREACHABLE: USER COPIES OF `f', `a' AND `e'
      CLOSE ALL SIX ROWS.  `assign_parms' does emit every parameter copy before
      any user insn, but sched.c:3251's head-skip protects only the LEADING RUN
      of `(set pseudo hard_reg)' insns (`INSN_REF_COUNT = 1'), and combine
      merges a parameter's own hard-register copy into a user copy and DELETES
      the parameter copy (see "A USER COPY OF A PARAMETER IS THE ONLY WAY TO
      GIVE ITS HARD-REG COPY A LATER LUID" in docs/matching-levers.md).  With
      `f = f0; a = a0; e = e0;' AFTER `g = (u8 *)0x80013710;', `$t0 = $a0' is
      the whole protected run and the `la' -- priority 1, which the BACKWARD
      scheduler places first in the block -- lands immediately after it.  The
      `sw $sN' saves are placed immediately before the first insn that SETS
      $sN (this compiler saves lazily), so the only thing that moves is the
      `la'.  Rows 2/3/4 match from that moment on.
      Permutations that do NOT fix the global-alloc fallout: copy order
      `e,a,f' (21); declaration order `e,a,f' (19); `f = f0' before `g' with
      a/e after (19); all three copies before `g' (19); only `f' a user copy
      (22).

   4. 19 -> 2.  `f0++; f0--;' IN THE LAST SWITCH ARM, found by
      `refsweep.py --body' in step 3's basin.  It does two things at once:
        * `f0' gains references AFTER the user copy, so combine can no longer
          delete `f0''s own pseudo.  It survives as its own allocno and
          `combine_regs' ties it to the incoming hard register -- `.greg' gains
          `81 in 5' (`$a1') -- and combine propagates it into the narrow test,
          which is the `sll $v0, $a1, 16' the 16-row park had filed as a second
          pseudo for `f' that it could not create.
        * with `f0' covering the range from entry, `f''s own allocno is priced
          over a shorter span and `allocno_compare' sorts it BEHIND `i', `c'
          and `n': `;; 13 regs to allocate: 84 91 87 179 92 86 88 89 93 94 95
          80 81', so i -> $s2, c -> $s3, n -> $s4, f -> $s5, a -> $s6,
          e -> $s7.  Thirteen renaming rows close at once.

   THE SHAPE TO CARRY AWAY, three times over now: a repeated insn in a listing
   is not necessarily a repeated statement (dbr COPIES, cross_jump MERGES);
   before pricing a register residue as unreachable, ask of every local in it
   HOW MANY BLOCKS assign or read it; and when a copy you wrote is simply NOT
   THERE, the question is not "which pass deleted it" in the abstract but
   WHICH REGISTER HEADS ITS cse QUANTITY, which is decided by whose last
   mention is later and is therefore fixable from the source.

   DEVICES AND SPELLINGS MEASURED AND REJECTED, WORTH NOT REPEATING
     * On the OLD 2-row body (`i = <expr>; t = i;'): `t = <expr>; i = t;' at
       t = s32/u8/u16/s8/s16, test on `t' or on `i' -- all 136w/6; `i = t;'
       hoisted before the switch 137w/24; `w = <expr>; i = w; t = w;' 137w/2
       (the same stream).  Every one of those was a step away from the lever
       above, which is a late mention of `t' and nothing else.
     * `u8 i; s32 t; t = <expr>; i = t;' is 137w / 5 ROWS and it CLOSES these
       two rows -- a QImode destination is invisible to cse.c:7499 -- but the
       five rows it leaves are all `or $v1, $s2, $s5' against the target's
       `or $v1, $s5, $s2' at the five `f | i' sites.  That one is combine's:
       a narrow `i' makes `f | i' an `(ior (reg f) (zero_extend (reg:QI i)))'
       and `gen_binary' (combine.c:9048) swaps a commutative pair when
       `GET_RTX_CLASS (op0) == 'o'' and op1's is not.  ZERO_EXTEND is class
       '1', SUBREG is 'x', so only a full SImode `i' makes op1 class 'o':
       `i | f', `(s32)i | f', `f | (s32)i', `f | (i | 0)' are all 137w/5, a
       separate `s32 j; j = i;' before the switch is 138w/25, `u16 i' is
       137w/5.  THE SImode `t' ROUTE ABOVE IS THE ONE THAT ESCAPES BOTH.
     * `g++; g--;' before `if (v != 0)': a LOSS -- it makes `g' a two-set
       pseudo so `update_equiv_regs' rematerialises the 0x80013710 constant a
       second time inside the body.
     * `do { } while (0)' around `v = q[1]': worth NOTHING.
     * `*(volatile u8 *)(q + 1)' for the third read, and
       `__asm__ __volatile__("" ::: "memory")' between the two tests: one row
       better than `q++; q--;' on the 29-row body, but each leaves a spurious
       `nop', and both are devices this project avoids.
     * A duplicated fail-body per test (two copies or three): 29/137 with a `g'
       device, 40/134 without -- cross-jumping merges them back and cse follows
       the branch anyway (`cse_end_of_basic_block' takes the branch whenever
       `LABEL_NUSES == 1' and the label is preceded by a BARRIER, cse.c:8154,
       which a fail-body ending in a `goto' always is).
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
     * The redundant `(u16)' cast in `(u16)*(u16 *)(p + 0x2A)' is a no-op and
       is dropped, because clang-format rewrites it to `(u16) * (u16 *)' and
       format-on-save would then churn the file.

   SWEEPS.  All four were clean on the 2-row body (widthsweep: no width change;
   refsweep: 16 names, 1378 insertion points; devsweep: 40 statements, 16
   names, 680 candidates/round; stmtsweep: 7 movable assignments) -- AND NONE
   OF THEM COULD HAVE FOUND THIS, because the fix needs the copy REVERSED
   first, which is a direction change no sweep varies, and the reversed body is
   a WORD SHORT so a row-count sweep reads it as a regression.  The lead was
   the old park's own note that `t = <expr>; i = t;' is 136w/6 with 3 of the 6
   rows pure branch-offset collateral: ONE missing word, one cause.
*/
void func_8009F644(u8 *p, s32 f0, s32 a0, s32 e0)
{
    u8 *q;
    u8 *r;
    s32 i;
    s32 t;
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
        t = (*(u16 *)(p + 0x2A) >> 9) & 7;
        i = t;
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
            t++;
            t--;
            q[3] = 0;
            q[2] = 0;
            break;
        }
    }
}
