/* func_lo16_8001EAA4 -- ovl_dng_lo16, 108 target words.
   PARKED by agent h176, batch 35, 2026-11-25.  108 words / 54 ALIGNED
   DIFFERENCES, and FIFTY OF THE FIFTY-FOUR ARE `R' ROWS -- a single uniform
   rotation of the caller-saved allocation.  The word count, the control
   flow, every branch target and every delay slot agree.

   WHAT IT IS: a weighted random pick over D_80073414.  It draws
   `rand() % D_8001F6F8[0x13]', walks D_8001F6F8[1..0x12] to find the band
   the draw falls in, then walks that band's D_80073414[i] entry list
   accumulating a per-entry weight (0x80 / 0x55 / 0x20 / 1, selected by bits
   12-13 of each entry's first halfword) until the running total passes the
   draw.  It writes the band in a[0] and the entry index in b[0] and returns
   the band, or zeroes both and returns 0.  func_lo16_8001E660 calls it as
   its fall-out path.

   THE RESIDUE IS A WHOLE-BODY REGISTER ROTATION, and the diagnosis is a
   PSEUDO COUNT, not a priority:

     target   $a0 r   $a1 w   $a2 i   $a3 off  $t0 j  $t1 acc  $t2 const 2
              $t3 D_80073414 base   $t4 (s16)i   $t5 q   $v1 k (block temp)
     mine     $a0 r   $a1 k   $a2 w   $a3 i    $t0 j  $t1 off  $t2 acc
              $t4 const 2   $t5 (s16)i   $t3 q   $v1 D_80073414 base

   i.e. the target gives `k' (the 0..3 selector) the EARLY, block-local
   register $v1 and pushes the D_80073414 base out to $t3, while mine gives
   the base $v1 and `k' a global register, displacing everything after it by
   one.  MIPS's REG_ALLOC_ORDER puts $v1 (reg 3) ahead of $a1 (reg 5), so
   mine is allocating one allocno too many before `w'.
   ELEVEN candidate allocnos here against the target's ten.

   TWO FACTS ARE SETTLED AND BOTH ARE WORTH CARRYING:

   1. THE SECOND HALFWORD READ MUST GO THROUGH A POINTER LOCAL
      `q = &D_80073414[i]' ASSIGNED BEFORE THE LOOP, while the first read
      and the loop bound use the array subscript `D_80073414[i]'.  With all
      three spelled as the subscript, cse unifies them and the body is 99
      words / 37 -- five words short, missing the target's second
      `lw 0xC($t5)' / `addu' / `lhu' entirely.  That asymmetry is visible in
      the listing: the in-loop reads RECOMPUTE `&D_80073414[i]' from the
      `lui'/`addiu %lo' every iteration while the `lhu' at .Llo16_8001EBA0
      goes through the invariant `$t5', which is a COPY of the address the
      loop guard computed (`addu $t5, $a3, $zero').  Measured 108 words with
      `q', 99 without -- the only spelling that reaches the right length.
   2. `(x & 0x3000) / 0x1000' MUST GO THROUGH AN `s32' LOCAL.  Written
      inline, fold narrows the whole TRUNC_DIV_EXPR to HImode -- both
      operands fit in `unsigned short' -- and emits a single
      `srl $v1, $v0, 0xC' where the target has the SIGNED power-of-two
      sequence `bgez' / `addiu $v0, $v0, 0xFFF' / `sra $v0, $v0, 12'.
      Four words and the wrong signedness.  This is the SAME convert.c/fold
      narrowing that func_lo16_8001E824's `% 0x28' hits, now on a DIVISION
      as well as a modulo, and the cure is the same: assign to a wide local
      first.  Assigning to the SAME local twice (`k = x & 0x3000;
      k = k / 0x1000 & 3;') costs a `move` in the `bgez's delay slot where
      the target has a `nop'; a separate `s32 m' for the masked value is
      what the kept body uses.

   OTHER SPELLINGS MEASURED (all against the facts above unless noted):
     r as `u16'                                      KEPT      108 / 54
     r as `s32'                                                107 / 59
     r as `u32'                                                107 / 58
     r without the `(u32)' on the rand() result                108 / 54
     acc as `s32'                                              106 / 60
     the first read through `q', the second through the symbol 101 / 45
     the three reads all inline, no `v' local                  108 / 54
     the 0x3000 mask and the divide as three statements        108 / 54
     `off' replaced by `j * 0x14' in all three subscripts      108 / 54
   NOTE the cluster at 54: eight different spellings give the IDENTICAL
   figure, which is the brief's signature for a cause none of them touches.

   READ THE DISPATCH SHAPES OFF THE LISTING, they are settled:
   - the band walk is `for (i = 1; i < 0x13; i++) if (D_8001F6F8[i] >= r)
     break;' -- an UNSIGNED compare (`sltu'), and `acc = D_8001F6F8[i - 1]'
     afterwards reuses the loop's giv (`lhu $t1, -0x2($v1)').
   - the weight selector is an if/else-if CHAIN, not a switch: 0 -> 0x80,
     1 -> 0x55, 2 -> 0x20, else -> 1, and the ELSE's `addiu $a1, $zero, 0x1'
     sits in the `bne $v1, $t2' delay slot with the `k == 2' arm falling
     through, exactly as func_lo16_8001E824's case 4 does.
   - `j = 1' is a statement AFTER the band loop, which is why reorg
     duplicates it into the `beqz' delay slot (word 21) and the loop's
     fall-out (word 26).  In this body it is the `for' init and only one
     copy appears -- one of the four non-`R' rows.
   - THIS BODY ADDRESSES A CONSTANT BASE: `*(s32 *)0x80012090', the same
     verdict func_lo16_8001E660 measures 3 rows for and
     csrc/ovl_dungeon_800A4B88.c records.  The masked verifier cannot see
     that displacement.
   - the return is `(s16)i' and the sign extension is LOOP-INVARIANT: the
     target computes `sll $v0, $a2, 16' / `sra $t4, $v0, 16' once in the
     preheader and the return is `addu $v0, $t4, $zero'.

   PROTOTYPE NOTE: the unit's existing declaration for this function has to
   become `s32 func_lo16_8001EAA4(u8 *, u8 *);' -- it returns the band.  Its
   only caller, func_lo16_8001E660, discards the result, so widening the
   declaration moves nothing.

   DECLARATIONS
   extern u16 D_8001F6F8[];
   s32 func_lo16_8001EAA4(u8 *, u8 *);
   END DECLARATIONS
*/
s32 func_lo16_8001EAA4(u8 *a, u8 *b)
{
    u16 r;
    u16 acc;
    Unk80073414 *q;
    u16 v;
    s32 i;
    s32 j;
    s32 off;
    s32 k;
    s32 w;

    r = (u32)func_800A6D30() % D_8001F6F8[0x13];
    for (i = 1; i < 0x13; i++)
    {
        if (D_8001F6F8[i] >= r)
        {
            break;
        }
    }
    q = &D_80073414[i];
    acc = D_8001F6F8[i - 1];
    off = 0x14;
    for (j = 1; j < D_80073414[i].unk_02; j++)
    {
        v = *(u16 *)(D_80073414[i].unk_0C + off);
        if ((v & 0x10) == 0)
        {
            if ((v & 0x40) == 0 || *(s32 *)0x80012090 == 2)
            {
                k = *(u16 *)(q->unk_0C + off) & 0x3000;
                k = k / 0x1000 & 3;
                if (k == 0)
                {
                    w = 0x80;
                }
                else if (k == 1)
                {
                    w = 0x55;
                }
                else if (k == 2)
                {
                    w = 0x20;
                }
                else
                {
                    w = 1;
                }
                acc += w;
                if (r < acc)
                {
                    a[0] = i;
                    b[0] = j;
                    return (s16)i;
                }
            }
        }
        off += 0x14;
    }
    a[0] = 0;
    b[0] = 0;
    return 0;
}
