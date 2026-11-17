/* ===========================================================================
   BEST MEASURED CANDIDATE for func_800ADE74, ovl_dungeon (1817 words).
   2026-11-16, agent j36.  Supersedes j34's uncompiled decode: the structural
   reading in that file was right and is what this was built from, but almost
   every per-instruction claim in it needed a compile to settle, and three of
   its "load-bearing" notes were wrong (see below).

   MEASURED, with the three alias declarations below in place:

       python3 tools/decomp/aligndiff.py <unit copy> func_800ADE74
       built 1826 words, target 1817 words, 256 aligned differences

   256 is aligndiff's raw count and it OVERSTATES this residue badly: my
   build is 9 words long, so EVERY branch in the function has a different
   displacement and aligndiff cannot mask that.  Counting only rows that are
   not a pure branch-offset difference gives

       non-branch R  54      (register assignment, almost all of it)
       X             20      (one instruction against a different one)
       + / -         56      (words one side has and the other does not)
       -------------------
       130 rows of 1817 words -- 93% of the function is byte-identical.

   ovlverify's "1685 differing (masked)" is meaningless at a 9-word length
   difference; use aligndiff.

   THIS IS A PERMUTER CANDIDATE.  The residue is overwhelmingly register
   allocation inside three arms plus two cross-jump decisions, which is
   exactly what decomp-permuter is good at and what hand spelling is not: I
   measured ~60 variants by hand and the last 17 rows of progress cost most
   of them.
   ===========================================================================

   WHAT THE COMPILER SETTLED (none of this was in j34's decode)

   * THE EMPTY CASES MUST NOT BE WRITTEN OUT.  j34's decode says cases
     11..20, 0x1E, 0x1F and 0x21 have to appear or the table comes out short.
     They do not: gcc emits a dense table from the lowest case label to the
     highest, so `case 9` and `case 0x2C` alone produce all 36 entries with
     every gap pointing at the default label.  The table is byte-identical
     with the gaps left out.  Checked against the compiler rather than taken
     on the measurement: `stmt.c:5052-5071` takes `minval`/`maxval` from the
     case list and sizes the vector from their difference, and
     `stmt.c:5304-5307` is a loop headed `(* Fill in the gaps with the
     default.  `.So only the LOWEST and HIGHEST case labels matter to the
    table's extent; everything between is free.
        *THE SIX SHARED TAILS ARE NOT A GOTO LADDER,
    and they are not written
        out per arm either.  `.L800AF89C` is simply THE CODE AFTER THE SWITCH;
    every arm reaches it with a plain `break`, and the three entry points
                                                           .L800AF89C /
                                                           .L800AF8A0 /.L800AF8A4 are the delay -
                                                   slot scheduler
                                                       stealing one or two instructions from the head of that
                                                       block(`addu $a0, $s7, $zero`, then `addu $a1, $s5, $zero`) and bumping the
                                                   branch target by one word each time.Written this way the whole tail
                                                   structure falls out with NO gotos anywhere in the function-- the
                                                   finished body below contains none.
                                                       *THE FACING PREAMBLE HAS NO SEPARATE INDEX VARIABLE.It is
     `sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;` and then `D_8006CCD8[sp1E]` directly.j34 's `s32 u` intermediate puts the index in a second register and defers the `sh $v1, 0x1E($sp)` past the `sll`, costing a word of shuffling in each of the NINE copies.(The nine copies themselves are real-- that part of the decode is correct.) * `ret` IS `u32` AND IS RETURNED AS `ret << 16 >> 16`.Every other spelling loses the target' s `sll $v0, $fp, 16` / `srl $v0, $v0, 16` pair and compiles to a bare `move v0, s8`:
measured `u16 ret;
return ret;
`,
     `s32 ret;
return (u16)ret;
`, `s32 ret;
return ret & 0xFFFF;
` and
     `s16 ret;
return (u16)ret;
` all give the move(150 rows), and
     `return (u16)(ret << 16) >> 16;
` gives 175. The shift pair is 147. * THREE LANDED CALLEES NEED AN ALIAS WITH A DIFFERENT RETURN TYPE : -func_800A35A4 is landed returning `u8`.The target compares its
                                                                                                            result with func_8009FD40 's as `sll`/`sll`/`slt`, which is C' s
                                                                                                                shorten_compare narrowing a comparison of two SHORT -
    typed rvalues.Through the `u8` return the cast leaves an `andi $v0,
    $v0, 0xFF` and the compare degrades to `sra`/`slt`: +5 words over the three copies.The casts must come OFF and the callee must be `s16`.- func_8009FD40 is landed returning `s16`.Cases 0x2C and 0x2A need its result UNEXTENDED(`addu $s0, $v0, $zero`) so that
         `(u16)(r - 2) < 7` keeps its `andi $v0, $v0, 0xFFFF`; through the s16
         return gcc sign-extends on assignment and then drops the andi.
       - func_800A40AC is landed returning `s16` and costs a `sra` in case
         2's throw path for the same reason.
     The unit already uses this mechanism (`func_800BCB04h` at line 329), so
     these are three more of the same and they leave the landed definitions
     alone.
   * `sp1E = ((u32)func_800A0818(...) >> 9) & 7;` in cases 0x2A and 0x1C.
     func_800A0818 returns s32, so without the `(u32)` the shift is `sra`
     where the target has `srl` -- 2 words, and the same two arms also need
     it to reach their `.L800AF720` tail.
   * `*(s16 *)(c + 0x2A) = sp1E;` BEFORE `sp20 = r;` in cases 0 and 1.  The
     other order puts the func_800A384C result in $a0 instead of $v1 and
     costs 14 rows across the two arms -- the single largest hand win in the
     run (147 -> 133).
   * CASE 0x2A'S LOOP COUNTER MUST BE ITS OWN VARIABLE.  Sharing the `i` of
     the other six loops stops gcc reusing $s2 for both `&D_80082E80` and the
     counter, which forces a seventh callee-saved register ($s6) and shifts
     every register in the arm (133 -> 130).  Giving cases 0x22, 0x29 or 0x1B
     their own counters does nothing or hurts (0x22: 130 -> 178).
   * `D_800E2970_s[*(s8 *)(b + 0x26)].unk_0C & 2` is csrc/ovl_dungeon.c's own
     spelling (around line 32172) and matched on the first compile, 0x14
     stride and all.
   * func_800A0E6C IS CALLED WITHOUT A PROTOTYPE, exactly as the unit's later
     call site does.  Adding one would change that landed call.
   * The dead `*(u16 *)(c + 0x24) > 0x10000` test is real and is written out
     as j34 says;
with `s32 t` it gives the target's `lui`/`slt` exactly.

    RESIDUE,
    by region(+9 words net)

            1. case 0x1D's tail does not cross
        - jump into case 0x26's at.L800AEEFC:
      +8 words, 9 rows.  The nine instructions are TEXTUALLY IDENTICAL in
      both arms in my build -- same opcodes, same registers, same branch
      targets -- and gcc still emits both copies.  I read jump.c's
      find_cross_jump in scratch/gcc-src: for case 0x1D's `j .L800AFA10` the
      FIRST attempt, `find_cross_jump (insn, JUMP_LABEL (insn), 1, ...)`,
      matches the single `ori $v0,$zero,0x8009` that sits before that label
      in t6, succeeds at minimum 1, and the `for (target = jump_chain[...])`
      loop that would have found case 0x26's nine-instruction tail is guarded
      by `newjpos == 0` and never runs.  Giving either arm its own `g` does
      not change it (0x1D: 130 -> 137, 0x26: no change).

      ANSWERED 2026-11-16 by the dispatcher, and the answer says to stop
      looking at the two arms.  The first attempt compares the insns before
      the jump with the insns before **its own target label** -- E2 in
      `find_cross_jump (e1, e2, ...)` is the LABEL, not the sibling jump
      (`jump.c:2564`'s own comment).  So the original build gets past it
      because in the original the last instruction before `.L800AFA10`
      DIFFERS from case 0x1D's last instruction, the minimum-1 match fails,
      `newjpos` stays 0 and the `jump_chain` loop runs and finds case 0x26's
      nine-instruction tail.

      **The lever is in whatever falls into `.L800AFA10`, not in either
      arm.**

      CONFIRMED BY THE PERMUTER, 2026-11-16, 35 minutes, 6 jobs.  It found
      the +8 words and it found them exactly where the analysis above says
      they are: its one change is to DUPLICATE t6's `*(u16 *)(c + 0x46) =
      0x8009; return 5;` body into two arms of a test, which changes the last
      instruction before `.L800AFA10` and unblocks the longer merge.
      Re-measured with aligndiff rather than taken from its own score
      (6080 -> 5220): **1818 words against 1817 and 229 aligned rows**, from
      this body's 1826 / 256.  So the direction and the magnitude are proved.

      **Its spelling is not usable.**  The test it duplicated under is
      `if (D_80082E80)` -- an `extern u8 []`, so the condition is the array's
      address and is always true.  That cannot be the original source; it is
      the permuter finding the shape by the only route its mutation set
      offers.  The remaining job is a legitimate source form that makes gcc
      emit that block twice, or that otherwise changes the instruction
      before the label.

      One legitimate partial, measured here: inverting the function's FINAL
      test -- `if (*(s8 *)(b + 0x26) >= 0) return 2; return ret << 16 >> 16;`
      rather than the other way round -- is **1823 words**, three better than
      this body, though the aligned count rises to 271 because a shorter
      build shifts more branch displacements.  Writing the second 0x8009
      store through a temp is 1829 / 273.  Neither reaches the permuter's
      1818, and whether they stack with it is untested.  Change which constant that block leaves in a register, or
      reorder its last statements, so that its final instruction stops
      matching the arm's.  See docs/matching-levers.md, "A SHORT CROSS-JUMP
      MATCH FOUND FIRST BLOCKS A LONGER ONE".  Nothing in cases 0x1D or 0x26
      can reach this, which is why ~60 variants on them did not.
   2. case 0x18's last `c[0x46] = 0x800B; return 2;` does not cross-jump into
      .L800AF8F0:
      + 3 words.gcc holds the 0x800B in $v1 there;
      the target
          holds it in $v0,
          which is what lets it merge.3. .L800AE010's `func_800A0818(...); c[0x2A]=; c[0x46]=0x800B; return 2;` cross - jumps in MY build(into case 0x18's identical tail) and does NOT
                                                                                                                        in the target : -5 words.(2)and(3) are one question with opposite
                                                                                                                                        signs : in the target.L800AE010 holds 0x800B in $v1 and case 0x18 holds
                                                                                                                                                it in $v0,
          so neither merges with the other and case 0x18 merges with
                  .L800AF8F0 instead.Both of mine hold it in $v1.4. The claim block reloads D_800814A8 with two load
              - delay nops where the
              target interleaves the `lw` with the `lhu $v0,
          0x1C($sp)` chain:
      +2 words, 3 rows.  I tried the array-reference spelling the unit's
      func_80097F84 comment records (`((u8 **)(D_800814A8 + 0xAC))[j]`, which
      IS MEM_IN_STRUCT_P where `*(u8 **)(D_800814A8 + j*4 + 0xAC)` is not):
      byte-for-byte no change here, so that lever does not reach this store.
   5. case 0x24's Chebyshev test:
      ~15 rows, net - 2 words.The dx / dy / mx shape below reproduces the target's instruction sequence but gcc puts `mx` in $a2 where the target has $s0, and sp18 / sp1A in $v1 / $a0 where the target has $a0 / $a1.Six orderings measured, all 130 or worse.6. case 0x2A: ~20 rows, a clean permutation of the callee-saved registers
      ($s0<->$s3 for r/ang, $s1<->$s2 for &D_80082E80).  Instruction-for-
      instruction correct otherwise.
   7. case 0x22: 3 rows, `lui $s1` where the target has `lui $s0` and the
      probed record kept in $s0 instead of $a0.
   8. Parameters `d` and `e` are swapped between $s0 and $s1 against the
      target: 4 rows, no word cost, and nothing I tried moved them.

   SPELLINGS MEASURED AND REJECTED (count = non-branch rows, lower is better)
     one variable per use site for q/nd/i/j/r/t/g/h   1829 words, 224
     one variable per use site for i                  1823 words, 417
     one variable per use site for nd                 1829 words, 224
     one variable per use site for g                  1817 words, 182
     all of q/nd/i/j/r/t/g/h shared (the start point)  1820 words, 192
     `s32 u` for the facing index (j34's decode)      +1 word x 9 arms
     `(s16)func_800A35A4(...)` with the landed u8     +5 words
     `u16 ret; return ret;`                           1823 words, 150
     `s32 ret; return ret & 0xFFFF;`                  1823 words, 151
     `s16 mx` -> `s32 mx` in case 0x24                1825 words, 159
     case 0x24 without the dx/dy temporaries          1828 words, 160
     case 0x24 max as if/else                         1827 words, 138
     `sp20 = r` before `c[0x2A] = sp1E` (cases 0,1)    1824 words, 147
     the same swap in the fallback                    1827 words, 144
     declaration-order search, 80 random reorderings   no effect at all
   ===========================================================================

   THE THREE ALIAS DECLARATIONS BELOW ARE REQUIRED and are not yet in
   csrc/ovl_dungeon.c's declaration block.  Everything else this body needs
   is already declared or defined above line 20092 of the unit.
*)

s16 func_800A35A4h(u8 *, s32) __asm__("func_800A35A4");
s32 func_8009FD40w(u8 *, u8 *) __asm__("func_8009FD40");
s32 func_800A40ACw(u8 *, u16) __asm__("func_800A40AC");
 */

s32 func_800ADE74(u8 *a, u8 *b, u8 *c, u16 d, u16 e, u8 *f)
{
    u32 ret;
    s16 sp18;
    s16 sp1A;
    s16 sp1C;
    s16 sp1E;
    s16 sp20;
    u8 *g;
    u8 *q1;
    u8 *q2;
    u8 *q3;
    u8 *q4;
    u8 *q5;
    u8 *q6;
    u8 *q7;
    u8 *nd;
    u8 *o;
    u8 k;
    u8 k2;
    u16 h1;
    u16 h2;
    u16 h3;
    s32 i;
    s32 i2A;
    s32 j;
    s32 r1;
    s32 r2;
    s32 r3;
    s32 r4;
    s32 r5;
    s32 r6;
    s32 r7;
    s32 r8;
    s32 t1;
    s32 t3;
    s32 t5;
    s32 t7;
    s32 n;
    s32 nn;
    s32 aa;
    s32 ang;
    s32 dx;
    s32 dy;
    s16 mx;
    ret = 2;
    *(u16 *)(c + 0x46) |= 0x4000;
    if ((D_80013714 & 8) != 0)
    {
        func_800A9A0C(c);
        return -1;
    }
    if (c[0x25] == 0)
    {
        ret = 0;
        return ret << 16 >> 16;
    }
    if ((s16)func_800A2CAC(c) != 0 && (s16)func_800A6E8C(b, 0x60C, &sp18, &sp1A) != 0 &&
        c[0x13] != 0x22)
    {
        if ((s16)func_800A2C34(c) != 0)
        {
            return -1;
        }
        if (sp18 == b[0x24] && sp1A == b[0x25])
        {
            r1 = (s16)func_800A70E4(sp18, sp1A, *(s16 *)(c + 0x88));
            if (r1 >= 0)
            {
                t1 = *(u16 *)(c + 0x24);
                if (t1 > 0x10000)
                {
                    t1 = 0xFFFF;
                }
                *(u16 *)(c + 0x24) = t1;
                if (c[0x25] != 0)
                {
                    *(s32 *)(c + 0x1C) &= ~0x8;
                }
                if (c[0x66] < c[0x25])
                {
                    c[0x25] = c[0x66];
                    c[0x24] = 0xFF;
                }
                func_80098B38(D_800E3548 + r1 * 4);
            }
            return 0;
        }
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], sp18, sp1A, (u16 *)&sp1C);
        *(u16 *)(c + 0x46) = 0x800B;
        return 2;
    }
    func_800A19E4((s32)b, c, (s16)d, (s16)e, f);
    if ((*(s32 *)(c + 0x1C) & 0x2410) == 0x2000)
    {
        k = c[0x12];
        if ((*(s32 *)(D_800E3D7C + 0x1C) & 0x220) != 0 ||
            (s16)func_80042900((s8 *)D_800E3D7C, 0xA) != 0)
        {
            if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 && k != 0)
            {
                k = 1;
            }
        }
        switch (k)
        {
        case 0:
            if ((s16)func_800A2C34(c) != 0)
            {
                return -1;
            }
            if ((s16)func_80042900((s8 *)c, 6) == 0)
            {
                sp1C = func_800A3544(c, 0x1E);
                if (sp1C >= 0)
                {
                    if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 &&
                        func_8009FD40(b, D_80082E80) < func_800A35A4h(c, sp1C) &&
                        (s16)func_800A3518(D_800E3D7C) != 0)
                    {
                        *(u16 *)(c + 0x46) = (sp1C + 1) | 0x8000;
                        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                                           D_80082E80[0x25], (u16 *)&sp1C);
                        *(u32 *)(c + 0x60) = D_800814A8;
                        return 4;
                    }
                    i = 0;
                    j = D_8008347E & 1;
                    do
                    {
                        q1 = *(u8 **)(D_800814A8 + j * 4 + 0xAC);
                        if (q1 != NULL)
                        {
                            nd = *(u8 **)(q1 - 0x14);
                            if (func_8009FD40(b, nd) < func_800A35A4h(c, sp1C) &&
                                (s16)func_800A3518(*(u8 **)(D_800814A8 + j * 4 + 0xAC)) != 0)
                            {
                                *(u16 *)(c + 0x46) = (sp1C + 1) | 0x8000;
                                if (*(u8 **)(D_800814A8 + j * 4 + 0xAC) != c)
                                {
                                    *(s16 *)(c + 0x2A) =
                                        func_800A0818(b[0x24], b[0x25], nd[0x24], nd[0x25],
                                                      (u16 *)&sp1C);
                                }
                                *(u32 *)(c + 0x60) = *(u32 *)(D_800814A8 + j * 4 + 0xAC);
                                return 4;
                            }
                        }
                        i++;
                        j ^= 1;
                    } while (i < 2);
                }
            }
            q2 = func_800A3D18(b, c, 4);
            *(u8 **)(c + 0x60) = q2;
            if (q2 == NULL)
            {
                break;
            }
            r2 = func_800A384C(c, q2, (u8 *)&sp1E, 0);
            *(s16 *)(c + 0x2A) = sp1E;
            sp20 = r2;
            if ((s16)r2 < 0)
            {
                *(u16 *)(c + 0x46) = 0x800B;
                return 2;
            }
            *(u16 *)(c + 0x46) = r2 | 0x8000;
            return 1;

        case 1:
            if ((s16)func_800A2C34(c) != 0)
            {
                return -1;
            }
            if ((s16)func_80042900((s8 *)c, 6) == 0)
            {
                sp1C = func_800A3544(c, 0x1E);
                if (sp1C >= 0)
                {
                    if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 &&
                        func_8009FD40(b, D_80082E80) < func_800A35A4h(c, sp1C) &&
                        (s16)func_800A3518((u8 *)D_800814A8) != 0)
                    {
                        *(u16 *)(c + 0x46) = (sp1C + 1) | 0x8000;
                        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                                           D_80082E80[0x25], (u16 *)&sp1C);
                        *(u32 *)(c + 0x60) = D_800814A8;
                        return 4;
                    }
                    i = 0;
                    j = D_8008347E & 1;
                    do
                    {
                        q3 = *(u8 **)(D_800814A8 + j * 4 + 0xAC);
                        if (q3 != NULL)
                        {
                            nd = *(u8 **)(q3 - 0x14);
                            if (func_8009FD40(b, nd) < func_800A35A4h(c, sp1C) &&
                                (s16)func_800A3518(*(u8 **)(D_800814A8 + j * 4 + 0xAC)) != 0)
                            {
                                *(u16 *)(c + 0x46) = (sp1C + 1) | 0x8000;
                                if (*(u8 **)(D_800814A8 + j * 4 + 0xAC) != c)
                                {
                                    *(s16 *)(c + 0x2A) =
                                        func_800A0818(b[0x24], b[0x25], nd[0x24], nd[0x25],
                                                      (u16 *)&sp1C);
                                }
                                *(u32 *)(c + 0x60) = *(u32 *)(D_800814A8 + j * 4 + 0xAC);
                                return 4;
                            }
                        }
                        i++;
                        j ^= 1;
                    } while (i < 2);
                }
            }
            q4 = func_800A3D18(b, c, -3);
            *(u8 **)(c + 0x60) = q4;
            if (q4 == NULL)
            {
                break;
            }
            r3 = func_800A384C(c, q4, (u8 *)&sp1E, 0);
            *(s16 *)(c + 0x2A) = sp1E;
            sp20 = r3;
            if ((s16)r3 < 4)
            {
                *(u16 *)(c + 0x46) = 0x800B;
                return 2;
            }
            *(u16 *)(c + 0x46) = r3 | 0x8000;
            return 1;

        case 2:
            if ((s16)func_80042900((s8 *)c, 6) == 0)
            {
                sp1C = func_800A3544(c, 0x1E);
                if (sp1C >= 0)
                {
                    if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 &&
                        func_8009FD40(b, D_80082E80) < func_800A35A4h(c, sp1C) &&
                        (s16)func_800A3518((u8 *)D_800814A8) != 0)
                    {
                        if ((s16)func_800A2C34(c) != 0)
                        {
                            return -1;
                        }
                        *(u16 *)(c + 0x46) = (sp1C + 1) | 0x8000;
                        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                                           D_80082E80[0x25], (u16 *)&sp1C);
                        *(u32 *)(c + 0x60) = D_800814A8;
                        return 4;
                    }
                    i = 0;
                    j = D_8008347E & 1;
                    do
                    {
                        q5 = *(u8 **)(D_800814A8 + j * 4 + 0xAC);
                        if (q5 != NULL)
                        {
                            nd = *(u8 **)(q5 - 0x14);
                            if (func_8009FD40(b, nd) < func_800A35A4h(c, sp1C) &&
                                (s16)func_800A3518(*(u8 **)(D_800814A8 + j * 4 + 0xAC)) != 0)
                            {
                                if ((s16)func_800A2C34(c) != 0)
                                {
                                    return -1;
                                }
                                *(u16 *)(c + 0x46) = (sp1C + 1) | 0x8000;
                                if (*(u8 **)(D_800814A8 + j * 4 + 0xAC) != c)
                                {
                                    *(s16 *)(c + 0x2A) =
                                        func_800A0818(b[0x24], b[0x25], nd[0x24], nd[0x25],
                                                      (u16 *)&sp1C);
                                }
                                *(u32 *)(c + 0x60) = *(u32 *)(D_800814A8 + j * 4 + 0xAC);
                                return 4;
                            }
                        }
                        i++;
                        j ^= 1;
                    } while (i < 2);
                }
            }
            if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 && (s16)func_800A404C() == 0 &&
                *(u8 *)(D_800814A8 + 0x9A) == 0x11)
            {
                o = *(u8 **)(D_800814A8 + 0x60);
                if (o != NULL && (*(s32 *)(o + 0x14) & 0x2000) == 0)
                {
                    r4 = func_800A40ACw(c, func_8009CD58(o, 7, 0));
                    sp20 = r4;
                    if ((s16)r4 >= 0)
                    {
                        *(u16 *)(c + 0x46) = r4 | 0x8000;
                        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                                           D_80082E80[0x25], (u16 *)&sp20);
                        return 3;
                    }
                }
            }
            if ((s16)func_800A2C34(c) != 0)
            {
                return -1;
            }
            break;

        case 4:
            if ((s16)func_800A2C34(c) != 0)
            {
                return -1;
            }
            ret = 0;
            q6 = func_800A03C4(c, b[0x24], b[0x25]);
            *(u8 **)(c + 0x60) = q6;
            if (q6 != NULL)
            {
                r5 = func_800A384C(c, q6, (u8 *)&sp1E, 0);
                sp20 = r5;
                if ((s16)r5 >= 0)
                {
                    *(u16 *)(c + 0x46) = r5 | 0x8000;
                    *(s16 *)(c + 0x2A) = sp1E;
                    return 1;
                }
            }
            break;

        default:
            break;
        }
        if ((s16)func_800A2C34(c) != 0)
        {
            return -1;
        }
        return ret << 16 >> 16;
    }

    if ((s16)func_800A2C34(c) != 0)
    {
        return -1;
    }
    if ((*(s32 *)(c + 0x1C) & 0x10) != 0)
    {
        return ret << 16 >> 16;
    }
    if (*(s8 *)(b + 0x26) >= 0 && (D_800E2970_s[*(s8 *)(b + 0x26)].unk_0C & 2) != 0)
    {
        return ret << 16 >> 16;
    }
    if ((s16)func_80042900((s8 *)c, 6) == 0)
    {
        sp1C = func_800A3544(c, 0x1E);
        if (sp1C >= 0 && (s16)func_800A3518(c) != 0)
        {
            *(u16 *)(c + 0x46) = (sp1C + 1) | 0x8000;
            *(u8 **)(c + 0x60) = c;
            return 4;
        }
    }
    switch (c[0x13])
    {
    case 0x19:
        func_800ADD20(c, 0x10);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                          *(s16 *)(c + 0x88));
        if (g[0x13] != 0)
        {
            break;
        }
        if ((s16)func_800A3518(c) == 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x800A;
        return 6;

    case 0x16:
        if (c[0x48] != 0xF)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        if ((s16)func_8009FD40(D_80082E80, b) >= 0xB)
        {
            break;
        }
        if (func_800A365C(b, D_80082E80) == 0)
        {
            break;
        }
        if (func_800A36B4(c, D_800E3D7C) == 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8008;
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)&sp20);
        return 1;

    case 0x2C:
        func_800ADD20(c, 4);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        r6 = func_8009FD40w(D_80082E80, b);
        if ((u16)(r6 - 2) >= 7)
        {
            break;
        }
        if (func_800A365C(b, D_80082E80) == 0)
        {
            break;
        }
        t3 = (s16)func_800A0134(c, D_800E3D7C);
        if (t3 < 0)
        {
            t3 = -t3;
        }
        if (t3 >= 0x21)
        {
            break;
        }
        g = (u8 *)func_800A05A4(c, b[0x24], b[0x25], *(s16 *)(c + 0x2A), (s16)r6);
        if ((s16)func_800A2CB8(c, g) == 0)
        {
            break;
        }
        if (func_800A36B4(c, g) == 0)
        {
            break;
        }
        if (c[0xAE] != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)&sp20);
        return 5;

    case 0x2B:
        func_800ADD20(c, 0x20);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        if ((*(u16 *)(c + 0x98) & 0x8000) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)&sp20);
        return 5;

    case 0x22:
        func_800ADD20(c, 0x20);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        if (*(s8 *)(b + 0x26) < 0)
        {
            break;
        }
        for (i = 0; i < 2; i++)
        {
            g = *(u8 **)(D_800E3D7C + i * 4 + 0xAC);
            if (g != NULL && (s16)func_800A2CAC(g) != 0)
            {
                *(u16 *)(c + 0x46) = 0x8009;
                *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                                   D_80082E80[0x25], (u16 *)&sp20);
                return 5;
            }
        }
        break;

    case 0x29:
        func_800ADD20(c, 0x10);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        for (i = 0; i < 2; i++)
        {
            g = *(u8 **)(D_800E3D7C + i * 4 + 0xAC);
            if (g != NULL)
            {
                t5 = (s16)func_800A0134(c, g);
                if (t5 < 0)
                {
                    t5 = -t5;
                }
                if (t5 < 0x21 && (s16)func_800A41F0(g) != 0)
                {
                    nd = *(u8 **)(g - 0x14);
                    if ((s16)func_8009FD40(nd, b) == 1 && func_800C7F68(g) == 0)
                    {
                        *(u16 *)(c + 0x46) = 0x8009;
                        *(u8 **)(c + 0xA8) = g;
                        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], nd[0x24],
                                                           nd[0x25], (u16 *)&sp20);
                        return 5;
                    }
                }
            }
        }
        break;

    case 0x28:
        func_800ADD20(c, 0x40);
        h1 = *(u16 *)(c + 0x98);
        if ((h1 & 0x100) != 0)
        {
            *(u16 *)(c + 0x98) = h1 | 0x8000;
            break;
        }
        *(u16 *)(c + 0x98) = h1 & 0x7FFF;
        break;

    case 0x1A:
        func_800ADD20(c, 2);
        h2 = *(u16 *)(c + 0x98);
        *(u16 *)(c + 0x98) = h2 & 0x7FFF;
        if ((h2 & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                          *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((s16)func_800A2CB8(c, g) == 0)
        {
            break;
        }
        if ((s16)func_80042900((s8 *)g, 1) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x98) |= 0x8000;
        break;

    case 0x23:
        func_800ADD20(c, 4);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        if (*(s16 *)(c + 0xA6) != 0)
        {
            break;
        }
        if ((D_800E296C & 4) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        return 5;

    case 0x26:
        func_800ADD20(c, 2);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                          *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((s16)func_800A2CB8(c, g) == 0)
        {
            break;
        }
        if ((s16)func_80042900((s8 *)g, 4) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        return 5;

    case 0x27:
        func_800ADD20(c, 0x10);
        h3 = *(u16 *)(c + 0x98);
        *(u16 *)(c + 0x98) = h3 & 0x7FFF;
        if ((h3 & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                          *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((s16)func_800A2CB8(c, g) == 0)
        {
            break;
        }
        if ((s16)func_80042900((s8 *)g, 2) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        *(u16 *)(c + 0x98) |= 0x8000;
        return 5;

    case 0x20:
        if (c[0x49] == 0)
        {
            func_800ADD20(c, 2);
            if ((*(u16 *)(c + 0x98) & 0x100) == 0)
            {
                break;
            }
            sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
            if (func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                              *(s16 *)(c + 0x88)) != D_800E3D7C)
            {
                break;
            }
            *(u16 *)(c + 0x46) = 0x8009;
            return 5;
        }
        if ((*(s32 *)(c + 0x1C) & 0x410) != 0)
        {
            break;
        }
        func_800AA53C(c);
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            return 2;
        }
        if (*(s8 *)(b + 0x26) >= 0)
        {
            func_800A0E6C(b, *(s8 *)(c + 0x9C), c, c + 0x98);
            *(u16 *)(c + 0x46) = 0x800B;
            return 2;
        }
        if ((s16)func_8009FD40(b, D_80082E80) != 1)
        {
            *(u16 *)(c + 0x46) = 0x800B;
            return 2;
        }
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)(c + 0x98)) +
                             0x800;
        *(u16 *)(c + 0x46) = 0x800B;
        return 2;

    case 0x24:
        *(u16 *)(c + 0x98) &= 0x7FFF;
        if (c[0x49] != 0)
        {
            break;
        }
        if ((s16)func_800A6E8C(b, 0x12, &sp18, &sp1A) == 0)
        {
            break;
        }
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], sp18, sp1A, (u16 *)(c + 0x98));
        *(u16 *)(c + 0xA8) = sp18;
        *(u16 *)(c + 0xAA) = sp1A;
        *(u16 *)(c + 0x98) |= 0x8000;
        dx = b[0x24] - sp18;
        if (dx < 0)
        {
            dx = -dx;
        }
        mx = dx;
        sp18 = dx;
        dy = b[0x25] - sp1A;
        if (dy < 0)
        {
            dy = -dy;
        }
        sp1A = dy;
        if (sp18 < sp1A)
        {
            mx = sp1A;
        }
        if (mx == 1)
        {
            *(u16 *)(c + 0x46) = 0x8009;
            return 5;
        }
        *(u16 *)(c + 0x46) = 0x800B;
        return 2;

    case 0x25:
        func_800ADD20(c, 0x20);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        return 5;

    case 0x18:
        if ((s16)func_8009FD40(b, D_80082E80) >= 2 && c[0xB5] == 0)
        {
            func_800ADD20(c, 8);
            if ((*(u16 *)(c + 0x98) & 0x100) != 0)
            {
                *(u16 *)(c + 0x46) = 0x800A;
                return 6;
            }
        }
        if (c[0xB5] == 0)
        {
            break;
        }
        g = func_8009B25C(c, b[0x24], b[0x25], *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            *(u16 *)(c + 0x46) = 0x800B;
            return 2;
        }
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)(c + 0x98));
        *(u16 *)(c + 0x46) = 0x800B;
        return 2;

    case 0x2A:
        func_800ADD20(c, 4);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        if (func_800A365C(b, D_80082E80) == 0)
        {
            break;
        }
        r7 = func_8009FD40w(D_80082E80, b);
        if ((u16)(r7 - 2) >= 7)
        {
            break;
        }
        ang = *(u16 *)(c + 0x88);
        sp1E = ((u32)func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                   (u16 *)&sp20) >>
                9) &
               7;
        i2A = 0;
        sp18 = b[0x24];
        sp1A = b[0x25];
        for (; i2A < (s16)r7; i2A++)
        {
            sp18 = sp18 + (u16)D_8006CCD8[sp1E];
            sp1A = sp1A + (u16)D_8006CCE8[sp1E];
            if ((s16)func_800A45D8(sp18 << 6, sp1A << 6, (s16)(ang + 0x20)) != 0)
            {
                break;
            }
            n = func_800BCB04(((sp18 << 6) + 0x20) & 0xFFE0, ((sp1A << 6) + 0x20) & 0xFFE0,
                              (s16)(ang - 0x20));
            nn = (s16)n;
            aa = (s16)ang;
            if (nn < aa)
            {
                break;
            }
            if (nn - aa >= 0x21)
            {
                break;
            }
            ang = n;
        }
        if (i2A != (s16)r7)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        *(s16 *)(c + 0x2A) = sp1E << 9;
        return 5;

    case 0x1B:
        func_800ADD20(c, 4);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        for (i = 0; i < 2; i++)
        {
            g = *(u8 **)(D_800E3D7C + i * 4 + 0xAC);
            if (g != NULL)
            {
                t7 = (s16)func_800A0134(c, g);
                if (t7 < 0)
                {
                    t7 = -t7;
                }
                if (t7 < 0x21 && (s16)func_800A41F0(g) != 0 &&
                    (s16)func_80042900((s8 *)g, 0xC) == 0)
                {
                    nd = *(u8 **)(g - 0x14);
                    if ((s16)func_8009FD40(nd, b) == 1)
                    {
                        *(u16 *)(c + 0x46) = 0x8009;
                        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], nd[0x24],
                                                           nd[0x25], (u16 *)&sp20);
                        return 5;
                    }
                }
            }
        }
        break;

    case 9:
    case 0xA:
        g = func_800A02AC(c, b[0x24], b[0x25]);
        if (g == NULL)
        {
            break;
        }
        if ((u32)(g[0x13] - 0x33) >= 4)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        nd = *(u8 **)(g - 0x14);
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], nd[0x24], nd[0x25], (u16 *)&sp20);
        return 5;

    case 0x17:
        func_800ADD20(c, 0x10);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                          *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((*(s32 *)(g + 0x14) & 0x4000) == 0)
        {
            break;
        }
        if (g == D_800E3D7C)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        return 5;

    case 0x1C:
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        func_800ADD20(c, 0x20);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        sp1E = ((u32)func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                   (u16 *)&sp20) >>
                9) &
               7;
        if ((s16)func_8009A350(b[0x24], b[0x25], sp1E, (u16 *)&sp20) == 0)
        {
            break;
        }
        if ((sp20 & 0xB712) != 0)
        {
            break;
        }
        if ((s16)func_8009FD40(D_80082E80, b) != 2)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        *(s16 *)(c + 0x2A) = sp1E << 9;
        return 5;

    case 0x1D:
        func_800ADD20(c, 8);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                          *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((*(s32 *)(g + 0x14) & 0x4000) == 0)
        {
            break;
        }
        if (g[0x13] == 0)
        {
            break;
        }
        if ((s16)func_80042900((s8 *)g, 4) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        return 5;

    case 0x15:
        if (c[0xA7] == 0)
        {
            break;
        }
        if (c[0xA8] != 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                          *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if (g != D_800E3D7C)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)&sp20);
        return 5;

    default:
        break;
    }

    q7 = func_800A3D18(b, c, 2);
    *(u8 **)(c + 0x60) = q7;
    if (q7 != NULL)
    {
        r8 = func_800A384C(c, q7, (u8 *)&sp1E, 1);
        sp20 = r8;
        *(s16 *)(c + 0x2A) = sp1E;
        if ((s16)r8 < 0)
        {
            *(u16 *)(c + 0x46) = 0x800B;
            return 2;
        }
        *(u16 *)(c + 0x46) = r8 | 0x8000;
        return 1;
    }
    k2 = c[0x13];
    if (k2 == 0x1E && (*(s32 *)(c + 0x14) & 0x20000000) == 0)
    {
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        sp18 = b[0x24] + (u16)D_8006CCD8[sp1E];
        sp1A = b[0x25] + (u16)D_8006CCE8[sp1E];
        g = NULL;
        if (c[0xAC] == 0)
        {
            g = func_8009B25C(c, sp18, sp1A, *(s16 *)(c + 0x88));
            if (g != NULL && ((u32)(g[0x13] - 1) >= 0x2D || g[0x13] == k2))
            {
                g = NULL;
            }
        }
        if (c[0xAC] < 2)
        {
            if (g != NULL)
            {
                *(u16 *)(c + 0x46) = 0x8009;
                return 5;
            }
            g = (u8 *)((s16)func_800A70E4(sp18, sp1A, *(s16 *)(c + 0x88)) + 1);
        }
        if (g != NULL)
        {
            *(u16 *)(c + 0x46) = 0x8009;
            return 5;
        }
    }
    if (*(s8 *)(b + 0x26) < 0)
    {
        return ret << 16 >> 16;
    }
    return 2;
}
