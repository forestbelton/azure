/* func_lo16_80018E80 -- ovl_dng_lo16, 401 target words.
   ADVANCED by agent h197, batch 36, 2026-11-25, from 402 words / 17 ALIGNED
   differences to 401 WORDS -- EXACT -- and ONE ALIGNED DIFFERENCE.

       built 401 words, target 401 words, 1 aligned difference
       func_lo16_80018E80: target 401 words, candidate 401 words, 1 differing

   THE WHOLE RESIDUE IS NOW ONE INSTRUCTION, and it is a cse canonicalisation:

       R   83   83 80018FCC  bne v1,v0   <->   bne $a1, $v0

   i.e. the `k == 2 test reads q`s register where the target reads k`s.  See
   WHAT IS LEFT below.

   THREE THINGS THE PREVIOUS BODY HAD WRONG.  All three were found by reading
   the residue, not by sweeping, and each is a separate mechanism:

   1. `s32 k`, NOT `s16 k`, and the read is `(s16)(u16)D_8008146C`.
      D_8008146C is `extern s16` in structs.h, but the target reads it with
      `lhu` and then `sll 16`/`sra 16` (80018F2C..80018F38), so the source
      cast is `(s16)(u16)` -- and the SHIFTED value is reused by the signed
      divide-by-5 (`sra $v1, $v1, 31` at 80018F40 takes the sign bit off
      `k << 16`).  With `s16 k` every later use of k re-extends it and the
      `k == 2 compare costs an extra sra`: 402 words / 17.  With `s32 k`:
      400 words / 16.  This is the same verdict func_lo16_8001E660`s comment
      in this unit records ("`s32 k`, NOT `s16`"), on a third body.

   2. `q = k` IS ONE STATEMENT AFTER THE GATE, NOT ONE IN EACH ARM, and that
      is what produces the TARGET`S TWO `addu $v1, $a1, $zero` COPIES.  This
      inverts what a residue reading suggests.  The target has the copy twice
      (80018F60 in the branch delay slot and 80018F7C at the end of the if
      arm) because `q` is in $v1 and the `D_800E296C |= 0x10000000` arm
      clobbers $v1 between them; reorg steals the else arm`s copy into the
      delay slot and keeps the if arm`s.  Written in BOTH ARMS the copy
      survives as a source construct but `q` is then live across the or
      sequence, so it cannot have $v1, gets $a1, and the second copy is
      deleted as redundant: 400 words / 16.  Written ONCE after the gate, cse
      cannot forward it (q is also live into the q != 0 branch), q gets $v1,
      and the duplication falls out of reorg for free.  Measured, all with
      `s32 k` and `c` a local:
          q = k in both arms, copy last in the if arm      400w / 16
          q = k in both arms, copy first in the if arm     400w / 16
          q = k in both arms, gate inverted               400w / 16
          q = k before the gate AND in the if arm         401w / 11
          q = k ONCE after the gate                       401w /  1   <- kept
      A SEPARATE SECOND LOCAL for the k-derived value (`w = k`, q kept for
      the loaded value only) is strictly worse in every arm shape -- 400w/14
      in both arms, 400w/5 once after the gate -- because cse then forwards
      the copy into the compares and it disappears.  Do not re-sweep it.

   3. BOTH 0x80010000 BASES ARE `u8 *` POINTER LOCALS AND BOTH ARE NEEDED,
      and these addresses are CONSTANTS, not the structs.h symbols.  The
      target materialises the base twice (`$a0` at 80018E90 for the 0x22C/
      0x2090 group and `$a2` at 80018F08/80018F14 for the second group), so
      two locals is the right count.  Measured:
          b and c both `u8 *` locals                      401w /  1   <- kept
          c dropped, `*(s32 *)0x80012090` written direct   402w /  8
          b dropped, c kept                                402w / 17
          both dropped                                     403w / 27
          b and c MERGED into one local                    399w / 25
                     (one lui only, and it lands in $s1)
          the symbols D_8001022C / D_80012090 / D_80010234
            in place of the constant addresses             405w / 45
      The last row is the important one: splat renders this body`s listing
      with `%hi(D_80012090)`/`%lo(D_80012090)`, which LOOKS like a relocation
      and is not -- the symbol spelling re-materialises the address at every
      use and costs four words.  A `lui`+`lw` pair cannot be told from a
      relocation at the byte level, so the listing`s symbol names are splat`s
      INFERENCE here, and the unit`s own comments on func_lo16_8001784C and
      func_lo16_8001E660 ("D_80012090 IS THE CONSTANT ADDRESS ... as it is
      everywhere else in this unit") are the authority.  It is the constant
      address in this body too.

   WHAT IS LEFT -- ONE INSN, AND IT IS A cse REPRESENTATIVE CHOICE, NOT A
   STATEMENT POSITION.  After `q = k`, cse puts q and k in one equivalence
   class.  `make_regs_eqv` makes the NEW register canonical when it outlives
   the old one beyond the current cse block, and q (used again in the q != 0
   branch at 80019138/80019190) outlives k (last used at the `== 2 compare`),
   so every later reference to k is rewritten to q.  `flag_cse_skip_blocks`
   is what carries the class as far as the `== 2` test: cse follows the
   forward conditional branches around the 0x28 and `< 2` arms, so the test
   is still inside one cse path.  The target did NOT make that substitution.

   RULED OUT for that one insn, all at 401 words / 1 (i.e. perfectly flat):
       `2 == k`, `k - 2 == 0`                                       1
       `q == 0x28` / `q < 2` rewritten as `k == 0x28` / `k < 2`
         (either or both)                                           1
       `q = (s32)k` instead of `q = k`                              1
       declaration order of q and k, both ways                      1
       `if (1)` round the two tail arms                             1
       `m = (s16)(k / 5)` moved after `q = k`                 402w / 33
       `do { } while (0)` round the two tail arms             399w / 24
       the `&&` operands of the `== 2` gate swapped           402w / 25
   NEXT AGENT: the lever is whatever makes k outlive q, or whatever ends the
   cse path before 80018FCC.  A `-da` `.cse1`/`.cse2` pair on this body is
   the cheap next step; the statement-order sweep above is closed.

   DECLARATIONS
   void func_lo16_80019648(void);
   void func_lo16_80019A74(void);
   s32 func_lo16_80019684(s16, s16, s16);
   void func_lo16_800194C4(s16);
   void func_80048088(s16);
   END DECLARATIONS 
   THREE MORE SPELLINGS CLOSED BY THE DISPATCHER, 2026-11-25, all measured from
   the 401/1 body.  The residue is the word-83 comparison naming `$v1' (q)
   where the target names `$a1' (k), and the target's own allocation is now
   fully read off its listing: k lives in $a1 (`sra $a1, $v1, 16' at word 58,
   then the /5 `mult', the `>= 0x28' `slti', and the word-83 test), while
   `q = k' is copied into $v1 TWICE.  The `k == 2' test at word 83 is reached
   WITHOUT passing through the 0x28 arm's two calls, which is why a
   caller-saved $a1 can still hold k there.

       the two LATE tests (`q == 1', `q == 2') rewritten on `k'
                                                       400 words /  26 rows
         -- `q = k' goes dead and gcc deletes it, losing the word the two
            copies are made of.  So the late tests MUST read q, which is what
            forces q to outlive k and makes q canonical.  The tension is
            structural, not a spelling.
       the whole q/k pair's roles swapped (q holds the read, k is the copy)
                                                       400       /  21
       the word-83 test given its own re-read of the global,
         `(s16)(u16)D_8008146C == 2'                    401       /   1  FREE
       `q' assigned by re-reading the global instead of `q = k'
                                                       401       /   1  FREE

   The last two are the informative ones: cse folds a second read of
   D_8008146C back into a register copy from k, so re-reading cannot break the
   q/k equivalence class and cannot change which member is canonical.  That
   closes the source-level space for this residue.  The remaining route is the
   one h197 named -- a `-da' `.cse1'/`.cse2' pair to see why the target's
   compiler did not canonicalise at all -- and NOT another spelling.

   AND THE ONE LEVER THAT LOOKED LIKE IT SHOULD BREAK THAT IS NOW MEASURED AND
   FLAT (dispatcher, 2026-11-26).  A park in b07 had the same shape as this one
   -- two pseudos in one equivalence class, the wrong member canonical, every
   spelling flat -- and it landed on the SECOND disjunct of `cse.c:838's
   `make_regs_eqv': a pseudo whose `regno_first_uid' is BEFORE
   `cse_basic_block_start' becomes `qty_first_reg'.  `regno_first_uid' is
   filled by `reg_scan' before cse runs, so a DEAD earlier set is enough and
   `flow' deletes it at no cost in words.  On that body it was 455/1 -> 455/0.

   It does nothing here:

       `k = 0;' as the first statement after the declarations   401 w / 1 row
       `q = 0;' instead                                         401     / 1
       both                                                     401     / 1

   So the lever has a precondition this body does not meet.  In b07 the pair
   was a CALL RESULT and a copy of it, with the consuming store sitting
   BETWEEN the two assignments -- the store is processed before the pair
   becomes equivalent and so keeps the non-canonical member.  Here the pair is
   a local-to-local copy (`q = k') with no consumer between them, so there is
   no insn that cse processes while the two are still distinct, and which
   member is canonical does not change what any insn reads.

   That is the whole source-level space closed twice over, by two different
   mechanisms, and it is worth more than another flat spelling: the
   `-da' `.cse1'/`.cse2' route is now the ONLY one left.
*/

s32 func_lo16_80018E80(void)
{
    u8 *b;
    u8 *c;
    s16 *rp;
    s32 q;
    s32 k;
    s32 m;
    s32 ret;
    s32 flag;
    s32 v;
    s32 t;
    s32 i;
    s32 h;

    ret = 0;
    flag = 0;
    b = (u8 *)0x80010000;
    if (*(s32 *)(b + 0x22C) == 1)
    {
        if (D_8008146C < 2)
        {
            *(s32 *)(b + 0x2090) = 1;
            func_lo16_80019648();
        }
        else if (*(s32 *)(b + 0x2090) == 1)
        {
            *(s32 *)(b + 0x2090) = 0;
        }
    }
    t = D_80082E6B;
    if (t < 0x16)
    {
        if (t >= 0x12)
        {
            ret = 0;
            goto done;
        }
    }
    {
        c = (u8 *)0x80010000;
        q = *(s32 *)(c + 0x2090);
        if (q == 0)
        {
            k = (s16)(u16)D_8008146C;
            m = (s16)(k / 5);
            if (k >= 0x28)
            {
                flag = 1;
                D_800E296C |= 0x10000000;
            }
            q = k;
            if (q == 0x28)
            {
                ret = 1;
                if (func_80033BC0(0xA2) != 0)
                {
                    m = 7;
                }
                else
                {
                    m = -1;
                    func_lo16_80019A74();
                }
                flag = 1;
            }
            else if (q < 2)
            {
                ret = (func_800A6D30() & 3) + 3;
            }
            else if (k == 2 && *(s32 *)(c + 0x22C) != 1)
            {
                ret = 2;
                D_800E296C |= 0x10000000;
            }
            else if (D_8008146C == 0x1F && func_80033BC0(0x1389) == 0)
            {
                D_800E39C8[0].unk_06 = 1;
                D_800E39C8[0].unk_07 = 1;
                D_800E3648_s[0].unk_00 = 0x37;
                D_800E3648_s[0].unk_01 = 0x13;
                D_800E3648_s[0].unk_03 = 0;
                D_800E3648_s[0].unk_02 = 0x63;
                D_800E296C |= 0x10000000;
            }
            if (m >= 0)
            {
                m = func_lo16_80019684(m + 1, 1, 1);
                if (m == 0)
                {
                    D_80080A88 = 0;
                }
                if (D_80080A88 == 0)
                {
                    func_80044698();
                    D_80080A88 = 1;
                    func_80053DA8(0x200);
                    func_800542BC();
                }
                if (m == 0)
                {
                    func_lo16_80019684(0, 1, 0);
                }
            }
            else
            {
                func_lo16_80019684(0, 1, 0);
                func_lo16_80019684(0x29, 1, 1);
                func_80053DA8(0x200);
                func_800542BC();
                func_80048088(0x38);
            }
            if (flag == 0)
            {
                func_lo16_800194C4(D_8008146C);
            }
        }
        else
        {
            flag = 1;
            if (q == 1)
            {
                ret = 9;
                D_800E296C |= 0x10000000;
                func_lo16_80019684(1, 1, 1);
                func_80053DA8(0x200);
                func_800542BC();
                func_lo16_80019684(0, 1, 0);
            }
            else if (q == 2)
            {
                v = func_800A6D30() & 7;
                if (func_lo16_80019684(v + 1, 1, 1) == 0)
                {
                    func_80044698();
                    func_80053DA8(0x200);
                    func_800542BC();
                    func_lo16_80019684(0, 1, 0);
                }
                rp = D_80081468h;
                if (rp[2] == 1)
                {
                    {
                        u8 *bb;
                        u8 *pa;

                        v = func_800A6DA4(3, 0x2D) & 0xFFFF;
                        h = rp[2] + (func_800A6D30() & 1);
                        if (h >= 0x64)
                        {
                            h = 0x63;
                        }
                        i = 0;
                        bb = (u8 *)D_80083460h;
                        for (; i < 4; i++)
                        {
                            pa = &D_800DDC9C[i * 2 + 0x0];
                            *(u8 *)(i * 2 + *(s32 *)(bb + 0x18)) = v;
                            *(u8 *)(i * 2 + *(s32 *)(bb + 0x18) + 1) = h;
                            D_800DDC9C[i * 2 + 1] = v;
                            *pa = v;
                        }
                    }
                    {
                        u8 *bb;
                        u8 *pa;

                        v = func_800A6DA4(3, 0x2D) & 0xFFFF;
                        h = D_8008146C + (func_800A6D30() & 1);
                        if (h >= 0x64)
                        {
                            h = 0x63;
                        }
                        i = 0;
                        bb = (u8 *)D_80083460h;
                        for (; i < 4; i++)
                        {
                            pa = &D_800DDC9C[i * 2 + 0x8];
                            *(u8 *)(i * 2 + *(s32 *)(bb + 0x18) + 0x8) = v;
                            *(u8 *)(i * 2 + *(s32 *)(bb + 0x18) + 0x9) = h;
                            D_800DDC9C[i * 2 + 0x9] = v;
                            *pa = v;
                        }
                    }
                }
                else
                {
                    memcpy(D_80083478, D_80083478 + 0x10, 0x10);
                    memcpy(D_800DDC9C, D_800DDC9C + 0x10, 0x10);
                }
                {
                    u8 *bb;
                    u8 *pa;

                    v = func_800A6DA4(3, 0x2D) & 0xFFFF;
                    h = D_8008146C + (func_800A6D30() & 1);
                    i = 0;
                    bb = (u8 *)D_80083460h;
                    for (; i < 4; i++)
                    {
                        pa = &D_800DDC9C[i * 2 + 0x10];
                        *(u8 *)(i * 2 + *(s32 *)(bb + 0x18) + 0x10) = v;
                        *(u8 *)(i * 2 + *(s32 *)(bb + 0x18) + 0x11) = h;
                        D_800DDC9C[i * 2 + 0x11] = v;
                        *pa = v;
                    }
                }
                {
                    u8 *bb;
                    u8 *pa;

                    v = func_800A6DA4(3, 0x2D) & 0xFFFF;
                    h = D_8008146C + (func_800A6D30() & 1);
                    i = 0;
                    bb = (u8 *)D_80083460h;
                    for (; i < 4; i++)
                    {
                        pa = &D_800DDC9C[i * 2 + 0x18];
                        *(u8 *)(i * 2 + *(s32 *)(bb + 0x18) + 0x18) = v;
                        *(u8 *)(i * 2 + *(s32 *)(bb + 0x18) + 0x19) = h;
                        D_800DDC9C[i * 2 + 0x19] = v;
                        *pa = v;
                    }
                }
            }
        }
    }
done:
    *(s32 *)0x80010234 = D_8008146C;
    return ret;
}
