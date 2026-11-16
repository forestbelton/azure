/* PARK: func_800A05A4, 138 of 139 words, 42 aligned differences
   (`ovlverify` reports 64 differing masked).  ovl_dungeon.  Measured
   2026-11-16 by splicing this body over the stub in a private copy of
   csrc/ovl_dungeon.c and running `aligndiff.py`; the numbers reproduce.

   ONE WORD SHORT INSIDE ONE BLOCK, with every opcode, immediate and branch
   target already the target's -- which is the shape the dungeon-sweep triage
   rule says to QUEUE for the permuter.

   NO NEW DECLARATIONS.  The prototype `s32 func_800A05A4(u8 *, s32, s32,
   s16, s32);` at csrc/ovl_dungeon.c:1425 is pinned by the landed caller
   func_800C0E88 (`func_800A05A4(p, e[0x24], e[0x25], *(s16 *)(p + 0x2A), w)`),
   and this body matches it.  Writing the fourth parameter `s32` is a hard
   `conflicting types`, which is how its type was settled.

   --------------------------------------------------------------- the residue

   THE MISSING WORD IS THE `lhu` RELOAD OF `i`, AND IT IS A loop.c HOIST.

       target    sh  $a3, 0x10($sp)        (entry block)
                 ...
                 lhu $t0, 0x10($sp)        INSIDE the loop
                 sll $a3, $t0, 9           in the jal's delay slot

       this      sw  $a3, 16($sp)          (entry block)
                 lhu $s8, 16($sp)          in the PREHEADER  <- hoisted
                 sll $a3, $s8, 9           inside the loop

   `(int)i` is loop-invariant, its set and its single use sit in one basic
   block, so `scan_loop` admits it under arm (1) (`reg_in_basic_block_p`,
   loop.c:794-800) and `move_movables` lifts it into the preheader, where the
   two insns collapse to one.  The target does not hoist it.  What the target
   DOES hoist out of the same loop is exactly two movables -- the `i * 2`
   copy, which lands in memory at 0x20($sp), and the `&D_8006CCD8[i]` copy,
   which lands in $s5 -- so the question is why the zero_extend is not the
   third.  Not established; the threshold arithmetic (60, decaying 3 per
   hoist, against an ~50-insn loop) is close enough that it may simply be
   spent, but that was not measured.

   The other 41 rows are a straight permutation of the nine callee-saved
   registers plus the frame size (0x50 here against the target's 0x50 is now
   equal; it was 0x48 before the `i` fix), and they move together with the
   hoist: the hoisted pseudo is a tenth quantity and every allocno after it
   shifts one register down.

   ---------------------------------------------- WHAT PAID: 139/68 -> 138/42

   `u16 i` MUST HAVE TWO SETS.  With a single `i = ((u32)dir >> 9) & 7;` the
   HImode pseudo never survives: combine's `set_nonzero_bits_and_sign_copies`
   records nonzero_bits 7 for the `andi` result (it records only for pseudos
   with REG_N_SETS == 1), `zero_extend (subreg:HI g)` folds back to `g`, and
   `i` is spilled as a WORD -- one HImode slot and two SImode slots, frame
   0x48 against the target's 0x50.  Splitting the value into an `s32 g` that
   carries every array index and a `u16 i` that is assigned BOTH before the
   loop and again as the loop body's first statement gives `i` two sets, the
   fold cannot be proved, and the `sh`/`lhu` pair appears: 139 words / 68
   aligned -> 138 / 42.

   The two-word arithmetic behind the diagnosis: `u32 i` -- no HImode pseudo
   at all, so no zero_extend and no hoist -- is 137 words, exactly two short,
   which is the `lhu` + `sll` the target carries in the loop.

   ------------------------------------- spellings measured, with their scores
   ------------------------------------- `differing` is ovlverify masked

   On the 139-word plateau (all BYTE-IDENTICAL to each other -- same md5 over
   the emitted word column -- so these are not merely equal-scoring, they are
   the same program):

     s16 i instead of u16 i                                139 / 80
     ((u16)dir >> 9) & 7   /   ((u32)dir >> 9) % 8         139 / 80
     j = (u32)dir >> 9; i = j & 7;                         139 / 80
     `dir` reassigned in place, no `i` at all              139 / 80
     the whole `((u32)dir >> 9) & 7` inlined at all 7 uses 139 / 80
     i * 512   /   (s32)i << 9   for   i << 9              139 / 80
     (u16)D_8006CCD8[i] for (D_8006CCD8[i] & 0xFFFF)       139 / 80
     x + (D_8006CCD8[i] & 0xFFFF) (operand order flipped)  139 / 80
     `i` declared first / last in the declaration block    139 / 80
     `i = ...` moved after `lim = n`; `thr` after `lim`    139 / 80
     xx/yy declaration order swapped                       139 / 80
     s32 r with casts instead of u8 *r                     139 / 80
     s16 *dp = &D_8006CCD8[i] assigned inside the loop     139 / 80
     for (k = 0; k < lim; k++)                             139 / 80
     k = 0; if (k < lim) { do { } while (k < lim); }       139 / 80
     u16 i0 copied into `i` at the loop top                139 / 80 (new md5)

   and everything that moved the word count, all worse than 138/42:

     ((u32)dir & 0xE00) >> 9                               139 / 82
     (i << 9) & 0xE00                                      139 / 94
     (i & 7) << 9                                          137 / 105
     u32 i                                                 137 / 105
     s32 w9 = i << 9 hoisted before the loop                137 / 113
     s32 g for the index with a single u16 i = g            137 / 101
     the same with u16 i[1] / volatile u16 i / (void)&i    137 / 101-106
     s16 k                                                 140 / 91
     s16 d                                                 141 / 103
     s16 *ep for the D_8006CCE8 side                       140 / 108
     both dp and ep as loop-local pointers                 140 / 111
     u16 xx, u16 yy                                        145 / 136
     k = 0 moved above the first call                      142 / 124
     (void)&i on the two-set body                          142 / 135
     volatile u16 i on the two-set body                    142 / 114
     __asm__("" : "+r"(i)) at the loop top                 144 / 136
     __asm__("" : "+r"(i)) before the func_800A44E0 call   148 / 142
     __asm__("" ::"r"(i)) before the func_800A44E0 call    143 / 132
     dp and ep loop-top pointers on the two-set body       139 / 76  (67 aligned)
     ep-only loop-top pointer on the two-set body          139 / 68
     `i = g` placed just before the func_800A44E0 call     137 / 105
     `i = g` a second time inside the loop                 138 / 64  (identical)

   THREE SPELLINGS ARE LOAD-BEARING AND MUST NOT BE VARIED BY A SEARCH:

   * `d = d >= 0 ? d : -d;` -- the abs fold.  `if (d < 0) { d = -d; }` fills
     the `bgez`'s delay slot and is one word short; the target's bgez/nop/negu
     trio is gcc's `abs` pattern.
   * `s32 thr` with `(s16)thr` in the `d <= thr` compare AND an explicit
     `(s16)(*(u16 *)(p + 0x88) - thr)` on func_800A44E0's third argument.
     func_800A44E0 is DEFINED BELOW this function, so the call rides an
     implicit declaration and gets no prototype conversion -- with the cast
     omitted the sign extension disappears entirely and it is 137 / 97.  With
     `s16 thr` and no cast the extension lands on `thr` instead of on the
     difference: 139 words, 73 aligned.
   * ONE loop guard on `k`, not on the constant.  `k = 0;` before a plain
     `while (k < lim)` is what makes cse canonicalise the duplicated guard's
     `k` onto `r` (both are 0 there) and gives the target's
     `slt $v0, $s4, $v0`; writing the guard as `if (0 < lim)` gives a `blez`
     and is one word short.

   WHAT THIS CANDIDATE HOLDS FIXED: the statement order of the entry block,
   the duplicated `p[0x72]/p[0x73]` tail in both the entry arm and the loop
   arm (cross-jumping merges the last two words, as the target does), and the
   `break` out of the while.  Nothing in the table above varies the HOIST,
   which is the one thing that is wrong. */

s32 func_800A05A4(u8 *p, s32 x, s32 y, s16 dir, s32 n)
{
    u8 *r;
    s16 xx;
    s16 yy;
    u16 i;
    s32 g;
    s16 lim;
    s32 thr;
    s32 k;
    s32 d;

    r = NULL;
    g = ((u32)dir >> 9) & 7;
    i = g;
    thr = 0x20;
    lim = n;
    xx = (D_8006CCD8[g] & 0xFFFF) + x;
    yy = (D_8006CCE8[g] & 0xFFFF) + y;
    if (func_800A0548(xx, yy) != 0)
    {
        p[0x72] = *(u8 *)&D_8006CCD8[g] - xx;
        p[0x73] = *(u8 *)&D_8006CCE8[g] - yy;
        return 0;
    }
    k = 0;
    while (k < lim)
    {
        i = g;
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
        if (func_800A0548(xx + (D_8006CCD8[g] & 0xFFFF), yy + (D_8006CCE8[g] & 0xFFFF)) != 0)
        {
            p[0x72] = *(u8 *)&D_8006CCD8[g] - xx;
            p[0x73] = *(u8 *)&D_8006CCE8[g] - yy;
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
        xx += D_8006CCD8[g] & 0xFFFF;
        yy += D_8006CCE8[g] & 0xFFFF;
    }
    p[0x72] = xx;
    p[0x73] = yy;
    return (s32)r;
}
