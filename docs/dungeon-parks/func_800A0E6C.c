/* PARK -- func_800A0E6C, batch j52, 2026-11-16.

   ====================================== 2026-11-20: 93 rows -> 57

   The body below carries one device: a duplicated-arm `if (e)` around `c[0x73] = 0;`.

   A `do { } while (0);` wrapper and a duplicated-arm `if (v) { s } else { s }`
   are the same free-reference lever from two directions -- flow.c:2128 weights
   each reference by `loop_depth`, jump2's cross-jumping merges identical arms
   -- so both cost nothing and both are ordinary C.  Found by sweeping every
   statement position times both devices over every local on a
   `tools/decomp/onefunc.py` file, where a candidate is 0.09 s.  Nothing in
   this park's own diagnosis points at the winning statement.

   RESIDUE: 432 candidate words against 432 target words, 93 aligned
   differences (tools/decomp/aligndiff.py).  ovlverify's word-by-word count
   reads 171 because the two streams shift by a word twice; aligndiff is the
   honest figure.

   ALL BUT 8 OF THOSE ROWS ARE ONE ALLOCATION INVERSION.  The parameter `c`
   comes out in $s5 and the probe block's `w` (the masked func_800A0818
   result, the target's $s5) in $s4; the target has them the other way round,
   so every instruction that addresses `c` differs by one register bit.
   Measured out of the .greg/.lreg dumps:

       c (pseudo 83)   26 refs / 744 insns -> floor_log2(26)*26/744 = 0.1398
       w (pseudo 94)    5 refs /  70 insns -> floor_log2(5) * 5/ 70 = 0.1429

   so global.c's allocno_compare puts `w` first by 2.2% and it takes $s4.
   The order flips if `w`'s live_length reaches 72 (two more insns between its
   set and its last use in the probe loop) or `c`'s drops to 727.  `w`'s five
   references are one set plus two uses weighted by loop_depth 2.

   THE ONE SPELLING THAT FLIPS IT COSTS MORE THAN IT BUYS: writing the
   8-direction probe as a `goto` loop drops loop_depth to 1, `w`'s references
   to 3, and `c` lands in $s4 -- but a goto loop carries no loop notes, so
   loop.c stops hoisting `&D_8006CD00` and `&D_800DCEAC` into $s6/$s7 and the
   function comes out 430 words / 114 rows.  A `for` loop with those two
   hoists written as explicit locals instead is 436 / 121.

   THE OTHER 8 ROWS, all scheduling:
   * loop B's entry: the target is `lui $v0 / lb $v1, 0x26($s3) / addiu $a0`,
     with `addu $a2, $a0, $zero` (a second pseudo for the loop's copy of the
     D_800E2970 base) filling the `blez`'s delay slot; the candidate
     materialises the base straight into $a2 and leaves a `nop`.  Writing the
     loop body's base as the bare symbol does buy the second pseudo but
     loop.c then hoists `g + 0xE` instead of `g`: 433 / 93.
   * the same two rows again at loop C's entry.
   * the probe prologue's `lhu 0x2A($s4)` / `addiu $s1, 1` pair is scheduled
     one slot later than the target's.
   * `andi $a1, $s0, 0xff` where the target has `addu $a1, $s0, $zero`: the
     probe's direction index is a `u8` here, which is what buys the copy at
     all (as `s32` the copy is deleted and the function is a word short), but
     gcc re-zero-extends it where the target does not.

   MEASURED AND REJECTED (words / aligned rows):
   * the `*(s8 *)(b + 0x26) < 0` arm as an early-return block at the top
     instead of the `else` of the outer `if`: 434 / 459 -- it lands inline.
   * `if (r == NULL) { return 0; }` instead of nesting the rest in
     `if (r != NULL)` and falling out to the single final `return 0`: three
     extra `move $v0, $zero` blocks, 435 / 110.
   * `break` out of the first search loop instead of `goto hit;` with the hit
     block after loop B: gcc moves the block, but in front of the loop
     instead of behind it, 435 / 114.
   * `t` (the switch's result flag) as `s32`: the target's `addu $a0, $v0` /
     `addu $v0, $a0` pair around the switch is deleted, 431 / 110.  `s16` and
     `u8` both keep it.
   * sharing one local between the first loop's `abs` temporary and the probe
     block's `w`: the merged allocno outranks both parameters and `b`, `c`
     and `w` rotate through $s3/$s4/$s5, 435 / 171.
   * the probe's direction index as `s32`: 431 / 100; as `s16`: `sll`/`sra`
     pairs at every use.
   * `k` as `s32`: 432 / 120.  `v` as `s16`: 437 / 112.  `j` as `s32`:
     425 / 92 (seven words short).  A separate counter for the probe loop:
     432 / 111.  `w` as `s16` and `d` as `u16 *`: both identical at 432 / 93.

   WHAT THE DEVICES BUY, for whoever permutes this:
   * `*(volatile u8 *)e` on the SECOND read of the record's byte 0 is worth
     the target's reload and its `nop` (431 -> 432 words, 97 -> 93 rows).
     docs/matching-levers.md's "-fcse-skip-blocks COMMONS TWO READS ACROSS A
     BRANCH" entry is the mechanism; no plain spelling reaches it.
   * `goto hit;` / `goto again;` is what puts the first loop's hit block
     behind loop B, where the target has it.

   The second parameter is `s16`: the callee narrows it with `sll`/`sra 16`
   before comparing it with `*(s8 *)(b + 0x26)`.  The only call site,
   func_800CA444:34859, is on an implicit declaration today; it re-measures
   at 209 words / 0 differing against this prototype.

   DECLARATIONS THIS BODY NEEDS (none of them are in the file yet):

       extern s16 D_8006CD02;

   Everything else it calls or reads is already declared. 
   DECLARATIONS
   extern s16 D_8006CD02;
   END DECLARATIONS
   PROTOTYPE NOTE ON LANDING: the second parameter is s16. Its only call
   site, in the landed func_800CA444, re-measures at 209/209, 0 differing.
*/

s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d)
{
    u8 seen[8];
    u8 *g;
    u8 *r;
    u8 *e;
    s16 i;
    s16 j;
    s16 k;
    s16 t;
    s32 v;
    s32 q;
    s32 w;
    u8 m;

    if (*(s8 *)(b + 0x26) >= 0)
    {
        if ((c[0x72] | c[0x73]) == 0 || *(s8 *)(b + 0x26) != f)
        {
            if (e)
            {
                c[0x73] = 0;
            }
            else
            {
                c[0x73] = 0;
            }
            c[0x72] = 0;
            c[0x67] = 0;
            b[0x27] = 0xFF;
            r = (u8 *)func_8009FCAC(*(s8 *)(b + 0x26));
            if (r != NULL)
            {
                g = D_800E2970;
                for (i = 0; i < *(s16 *)(g + *(s8 *)(b + 0x26) * 0x14 + 0xE); i++)
                {
                    e = r + i * 4;
                    if (e[0] != 0)
                    {
                        v = b[0x24] - *(volatile u8 *)e;
                        if (v < 0)
                        {
                            v = -v;
                        }
                        if (v < 2)
                        {
                            v = b[0x25] - e[1];
                            if (v < 0)
                            {
                                v = -v;
                            }
                            if (v < 2)
                            {
                                goto hit;
                            }
                        }
                    }
                }
            again:
                j = func_800A6D30();
                g = D_800E2970;
                for (i = 0; i < *(s16 *)(g + *(s8 *)(b + 0x26) * 0x14 + 0xE); i++)
                {
                    j = j % *(s16 *)(g + *(s8 *)(b + 0x26) * 0x14 + 0xE);
                    e = r + j * 4;
                    if (e[0] != 0 && b[0x27] != j)
                    {
                        t = 0;
                        switch (c[0x67])
                        {
                        case 0:
                            t = *(s16 *)(e + 2) < 2;
                            break;
                        case 1:
                            t = *(s16 *)(e + 2) == 2;
                            break;
                        case 2:
                            t = *(s16 *)(e + 2) == 0;
                            break;
                        }
                        if (t != 0)
                        {
                            goto found;
                        }
                    }
                    j++;
                }
                return 0;
            hit:
                b[0x27] = i;
                c[0x67] = e[2];
                goto again;
            }
        }
        else if (b[0x24] == c[0x72] && b[0x25] == c[0x73])
        {
            r = (u8 *)func_8009FCAC(*(s8 *)(b + 0x26));
            if (r != NULL)
            {
                j = D_8008347E;
                g = D_800E2970;
                for (i = 0; i < *(s16 *)(g + *(s8 *)(b + 0x26) * 0x14 + 0xE); i++)
                {
                    j = j % *(s16 *)(g + *(s8 *)(b + 0x26) * 0x14 + 0xE);
                    e = r + j * 4;
                    if (e[0] != 0 && b[0x27] != j)
                    {
                        t = 0;
                        switch (c[0x67])
                        {
                        case 0:
                            t = *(s16 *)(e + 2) < 2;
                            break;
                        case 1:
                            t = *(s16 *)(e + 2) == 2;
                            break;
                        case 2:
                            t = *(s16 *)(e + 2) == 0;
                            break;
                        }
                        if (t != 0)
                        {
                            goto found;
                        }
                    }
                    j++;
                }
                return 0;
            }
        }
        else
        {
            *(s32 *)&seen[0] = 0;
            *(s32 *)&seen[4] = 0;
            k = (((s16) * (u16 *)(c + 0x2A) >> 9) - 4) & 7;
            seen[k] = 1;
            seen[(k + 1) & 7] = 1;
            seen[(k - 1) & 7] = 1;
            w = (s16)(func_800A0818(b[0x24], b[0x25], *(s8 *)(c + 0x72), *(s8 *)(c + 0x73),
                                    (u16 *)d) &
                      0xFFF);
            k = ((w >> 9) - 4) & 7;
            seen[k] = 1;
            seen[(k + 1) & 7] = 1;
            seen[(k - 1) & 7] = 1;
            for (i = 0; i < 8; i++)
            {
                if ((*(u16 *)d & 2) != 0)
                {
                    m = ((w - D_8006CD00[i]) >> 9) & 7;
                }
                else
                {
                    m = ((w + D_8006CD00[i]) >> 9) & 7;
                }
                if (seen[m] == 0)
                {
                    if ((s16)func_800BCB04((u16)(D_800DCEAC[m] + (b[0x24] << 6)),
                                           (u16)(D_800DCEBC[m] + (b[0x25] << 6)),
                                           (s16)(*(u16 *)(c + 0x88) - 0x20)) < 0x200)
                    {
                        *(s16 *)(c + 0x2A) = m << 9;
                        return 0;
                    }
                }
            }
            return 0;
        }
        goto out;
    found:
        e = r + j * 4;
        c[0x72] = e[0];
        c[0x73] = e[1];
        c[0x67] = e[2];
        *(s16 *)(c + 0x2A) =
            func_800A0818(b[0x24], b[0x25], *(s8 *)(c + 0x72), *(s8 *)(c + 0x73), (u16 *)d);
        b[0x27] = j;
    }
    else
    {
        b[0x27] = 0xFF;
        q = ((*(s16 *)(c + 0x2A) - D_8006CD02) >> 9) & 7;
        if ((s16)func_800BCB04((u16)(D_800DCEAC[q] + (b[0x24] << 6)),
                               (u16)(D_800DCEBC[q] + (b[0x25] << 6)),
                               (s16)(*(u16 *)(c + 0x88) - 0x20)) < 0x200)
        {
            *(s16 *)(c + 0x2A) = q << 9;
        }
    }
out:
    return 0;
}
