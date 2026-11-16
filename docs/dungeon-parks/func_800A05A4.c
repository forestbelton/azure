/* PARK: func_800A05A4, 139 of 139 words, 68 aligned differences
   (`ovlverify` reports 80 differing masked).  ovl_dungeon.  Measured
   2026-11-16 by splicing this body over the stub in a private copy of
   csrc/ovl_dungeon.c and running aligndiff.py; the numbers reproduce exactly.

   NO NEW DECLARATIONS.  The prototype `s32 func_800A05A4(u8 *, s32, s32, s16,
   s32);` at csrc/ovl_dungeon.c:1425 is pinned by the landed caller
   func_800C0E88 (:27757, `func_800A05A4(p, e[0x24], e[0x25], *(s16 *)(p +
   0x2A), w)`), and the body here matches it.  Writing the fourth parameter
   `s32` is a hard `conflicting types`, which is how the shape was settled.

   ----------------------------------------------------------------- residue

   SHAPE IS RIGHT, ALLOCATION IS NOT.  Of the 68 aligned rows, 36 are
   register-only, 17 are the FRAME SIZE and the spill-slot displacements, and
   the remaining 15 are two insns' scheduling in the loop preheader.  Every
   opcode, immediate and branch target is already the target's.

   THE WHOLE RESIDUE REDUCES TO ONE FACT: the spill of `i`.

     target   sh $a3, 0x10($sp)  ...  lhu $t0, 0x10($sp) ; sll $a3, $t0, 9
              sw $s5, 0x20($sp)  ...  lw  $t0, 0x20($sp) ; addu $s2, $t0, $v0
              -> two HImode slots (0x10, 0x18) + one SImode slot (0x20),
                 frame 0x50

     this     sw $7,  24($sp)    ...  lw  $8, 28($sp)    ; sll $7, $8, 9
              lw $8,  24($sp) ; move $fp, $18 ; sw $8, 28($sp)   (preheader)
              -> one HImode slot (0x10) + two SImode slots (0x18, 0x1C),
                 frame 0x48, and i*2 stays in $fp instead of being spilled

   In the target `i` is an HImode pseudo allocated to memory, so the loop's
   `(int)i` is a real `zero_extend` that reload turns into `lhu` AT THE USE.
   Here the HImode pseudo does not survive: cse folds `zero_extend(subreg:HI
   t)` back to `t` (combine's nonzero_bits for the `andi $a3,$a3,7` result is
   7), the value becomes SImode, and loop.c then hoists the resulting
   invariant copy into the preheader as a TENTH quantity.  That extra quantity
   is what permutes all nine callee-saved registers: the target gives `p` $fp
   (last) and `n`/the loop's D_8006CCE8 pointer $s2 (third); this body gives
   `p` $s7 and the hoisted copy $fp.

   The two-word arithmetic behind that claim: `u32 i` (no HImode at all, so no
   zero_extend and no hoist) is 137 words -- two SHORT -- which is the pair
   `lhu`+`sll` the target carries in the loop.

   ----------------------------------- spellings measured, all 139/139 unless
   ----------------------------------- noted; `differing` is ovlverify masked

   The 139/80 plateau is BROAD.  Every one of these produced a byte-identical
   candidate (same md5 over the emitted word column), so they are not merely
   equal-scoring, they are the same program:

     s16 i (instead of u16 i)                              139 / 80, identical
     ((u16)dir >> 9) & 7   /  ((u32)dir >> 9) % 8          139 / 80, identical
     j = (u32)dir >> 9; i = j & 7;                         139 / 80, identical
     dir reassigned in place, no `i` at all                139 / 80, identical
     the whole `((u32)dir >> 9) & 7` inlined at all 7 uses 139 / 80, identical
     i * 512 / (s32)i << 9  for  i << 9                    139 / 80, identical
     (u16)D_8006CCD8[i] for (D_8006CCD8[i] & 0xFFFF)       139 / 80, identical
     x + (D_8006CCD8[i] & 0xFFFF)  (operand order flipped) 139 / 80, identical
     `i` declared first / last in the declaration block    139 / 80, identical
     `i = ...` moved after `lim = n`                       139 / 80, identical
     xx/yy declaration order swapped                       139 / 80, identical
     for (k = 0; k < lim; k++)                             139 / 80
     k = 0; if (k < lim) { do { } while (k < lim); }       139 / 80
     `thr = 0x20` after `lim = n`                          139 / 80
     s32 r with casts instead of u8 *r                     139 / 80, identical
     s16 *dp = &D_8006CCD8[i] hoisted inside the loop      139 / 80, identical

   and the ones that moved the word count, all WORSE:

     ((u32)dir & 0xE00) >> 9                               139 / 82
     (i << 9) & 0xE00                                      139 / 94
     (i & 7) << 9                                          137 / 105
     u32 i                                                 137 / 105
     s32 w9 = i << 9 hoisted before the loop               137 / 113
     s16 k                                                 140 / 91
     s16 d                                                 141 / 103
     s16 *ep for the D_8006CCE8 side                       140 / 108
     both dp and ep as loop-local pointers                 140 / 111
     u16 xx, u16 yy                                        145 / 136
     s32 g for the table index plus a separate u16 i       146 / 138
     k = 0 moved above the first call                      142 / 124
     (void)&i;  (force `i` addressable)                    146 / 135
     __asm__("" : "+r"(i)) at the loop top                 144 / 136
     __asm__("" : "+r"(i)) before the func_800A44E0 call   148 / 142
     __asm__("" ::"r"(i)) before the func_800A44E0 call    143 / 132

   s32 thr with the explicit (s16) casts in both places is 137/97; s16 thr
   with `*(u16 *)(p + 0x88) - thr` (no explicit cast) is 139/73 but puts the
   sign extension on `thr` instead of on the difference.  The combination
   kept below -- s16 thr for the `d <= thr` compare, explicit `(s16)(...)` on
   the third argument because func_800A44E0 is defined BELOW this function and
   the call rides an implicit declaration with no prototype conversion -- is
   the only one that gets both.

   WHAT THIS CANDIDATE HOLDS FIXED: the statement order of the entry block,
   the duplicated `p[0x72]/p[0x73]` tail in both the entry and the loop arm
   (cross-jumping merges the last two words, which the target also does), the
   `d >= 0 ? d : -d` abs spelling (`if (d < 0) d = -d;` is one word short --
   the bgez/nop/negu trio), and the `goto`-free `break` out of the while.
   None of the sixteen plateau spellings above varies the SPILL DECISION,
   which is the thing that is wrong.
*/
s32 func_800A05A4(u8 *p, s32 x, s32 y, s16 dir, s32 n)
{
    u8 *r;
    s16 xx;
    s16 yy;
    u16 i;
    s16 lim;
    s32 thr;
    s32 k;
    s32 d;

    r = NULL;
    i = ((u32)dir >> 9) & 7;
    thr = 0x20;
    lim = n;
    xx = (D_8006CCD8[i] & 0xFFFF) + x;
    yy = (D_8006CCE8[i] & 0xFFFF) + y;
    if (func_800A0548(xx, yy) != 0)
    {
        p[0x72] = *(u8 *)&D_8006CCD8[i] - xx;
        p[0x73] = *(u8 *)&D_8006CCE8[i] - yy;
        return 0;
    }
    k = 0;
    while (k < lim)
    {
        r = func_8009B4B0(p, xx, yy);
        if (r != NULL)
        {
            d = *(s16 *)(r + 0x88) - *(s16 *)(p + 0x88);
            d = d >= 0 ? d : -d;
            if (d <= (s16)thr)
            {
                break;
            }
        }
        if (func_800A0548(xx + (D_8006CCD8[i] & 0xFFFF), yy + (D_8006CCE8[i] & 0xFFFF)) != 0)
        {
            p[0x72] = *(u8 *)&D_8006CCD8[i] - xx;
            p[0x73] = *(u8 *)&D_8006CCE8[i] - yy;
            return 0;
        }
        if ((s16)func_800A44E0((xx << 6) & 0xFFC0, (yy << 6) & 0xFFC0,
                               (s16)(*(u16 *)(p + 0x88) - thr), i << 9) != 0)
        {
            p[0x72] = -xx;
            p[0x73] = -yy;
            return 0;
        }
        thr += 0x20;
        k++;
        xx += D_8006CCD8[i] & 0xFFFF;
        yy += D_8006CCE8[i] & 0xFFFF;
    }
    p[0x72] = xx;
    p[0x73] = yy;
    return (s32)r;
}
