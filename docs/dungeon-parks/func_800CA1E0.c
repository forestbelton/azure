/* PARK func_800CA1E0 -- 152 of 153 words, 2 aligned differences (j82, 2026-11-16).
   Previous bests: 151/17 (j40), 153/6 word-exact (permuter + j60), 152/5.
   THIS BODY IS 2 ROWS.  Everything in the function now matches except ONE
   instruction, and the reason it cannot be had is a named pass with a named
   test -- see "THE WALL" below before spending a single compile.

   Shape: func_8009A66C's twin (csrc/ovl_dungeon.c, 149 words, landed) with a
   map-bounds pre-check in front of it and a 1/-1 tail instead of 1/2/3.
   Returns -1 blocked, 0 cannot, 1 ok.

   RESIDUE -- one instruction, in the entry block:

       target     3 srl   $a0, $a0, 9      candidate  3 srl   $a0, $a0, 9
       target     4 andi  $a0, $a0, 0x7    candidate  -  (missing)
       target     6 addu  $s5, $a0, $zero  candidate  5 andi  $s5, $a0, 0x7
       target    10 sll   $s3, $s5, 1      candidate 10 sll   $s3, $s5, 1   OK
       target    22 addu  $a0, $s5, $zero  candidate 22 addu  $a0, $s5, 0   OK

   The target computes `(f >> 9) & 7` into a SHORT-LIVED pseudo in $a0, copies
   it into the long-lived $s5, and copies it BACK into $a0 at word 22 to feed
   func_8009A540 forty instructions later.  This body gets the $s5 live range
   and the word-22 copy-back EXACTLY right -- both were missing from every
   earlier candidate -- and writes the `andi` straight into $s5, so the one
   missing word is the A->B copy at word 6.

   -------------------------------------------------------------- WHAT IS NEW

   (1) THE LEVER THAT BOUGHT WORD 22, AND IT IS WORTH COPYING ELSEWHERE.
       A SECOND, SEPARATELY-MASKED COMPUTATION OF THE SAME VALUE FROM THE SAME
       PARAMETER, WRAPPED IN A ZERO-EXTENDING WIDENING AND SPLIT OVER TWO
       STATEMENTS, IS A REGISTER COPY THAT SURVIVES:

           k  = (f >> 9) & 7;           -- the index, $s5, all real uses
           kz = (u16)(f >> 9);          -- cse-opaque: NOT folded at hash level
           ka = kz & 7;              -- cse substitutes (reg k); combine keeps it:
                                        a real `move $a0,$s5` at word 22
           ... func_8009A540(ka, ...)

       cse cannot hash `(and (zero_extend (subreg:HI (lshiftrt f 9))) 7)` to
       the class of `(and (lshiftrt f 9) 7)`, so it does not call
       make_regs_eqv and does NOT rewrite ka's use back onto k -- which is
       exactly what kills the plain `k3 = k;` third local that j60 measured as
       byte-identical.  It still substitutes the equivalent register into the
       SET_SRC, so the insn becomes a bare copy and costs exactly one word.
       This refines the catalogue entry "A SOURCE-LEVEL COPY CANNOT REPRODUCE
       AN ALLOCATOR COPY": a copy written as a REDUNDANT RECOMPUTATION does.

       IT MUST BE TWO STATEMENTS.  `ka = (u16)(f >> 9) & 7;` as one statement
       is 152 words / 10 rows -- the whole index triple collapses and the
       register file renumbers.  `(u8)` in place of `(u16)` measures the same
       152/2.  This is the same two-statement rule as the widening lever that
       landed func_8008EAC8 / func_80096384 / func_8008ACDC.

   (2) THE SAME DEVICE OFF A LOCAL, `kz = (u16)X; Y = kz & 7;`, ALSO YIELDS A
       MOVE -- combine folds `(and (zero_extend (subreg:HI X)) 7)` to
       `(reg X)` via significant_bits(X) == 7 -- BUT ONLY IF X HAS TWO OR MORE
       USES.  With one use combine substitutes X's own `andi` into the
       widening first and the whole chain collapses back to a single `andi`.
       That is why `kk = (u16)k; ...` (one use of k) is byte-identical to no
       device at all, and why variants E/F/H/V1/V2/V3/Z4 all measure 152/10.

   (3) THE WALL, AND IT IS NOT AN ALLOCATION TIE.  The word-6 copy and the
       word-22 copy CANNOT BOTH EXIST in anything compiled from a source where
       the dying temp lands in $a0.  jump2 -- `jump_optimize (insns, 1, 1, 0)`
       at toplev.c:3690, so noop_moves == 1 -- runs the redundant-copy block
       at jump.c:476-525.  For every `(set (reg hard) (reg hard))` it calls

           find_equiv_reg (NULL_RTX, insn, 0, sreg, NULL_PTR, dreg,
                           GET_MODE (SET_SRC (body)))        [reload.c:5554]

       which scans BACKWARD over the linear insn stream, stopping only at a
       CODE_LABEL, for any insn that copies into or out of $a0 whose other end
       is $s5.  The word-6 copy is exactly that insn, in the same block, with
       nothing writing $a0 or $s5 in between, and the mode check
       `GET_MODE (value) != mode` passes because both are SImode -- so word 22
       is deleted as redundant.  Verified in the dumps: for the variant with
       both copies, `.combine`, `.regmove`, `.sched`, `.lreg`, `.greg` and
       `.sched2` ALL contain

           (insn 32 43 12 (set (reg/v:SI 4 a0) (reg/v:SI 19 s3)) 168)

       sitting at precisely the target's word-22 slot (between the `lhu` from
       $fp and the `sh $a3,0x18($sp)`), and `.jump2` does not.  The register
       dispositions were `92 in 4` (the temp, $a0), `93 in 19`, `94 in 4` with
       `;; 94 preferences: 4` -- the allocator did everything right.

       MEASURED ESCAPE ROUTES FROM find_equiv_reg, ALL OF THEM:
         * put the word-22 device in a later basic block -- NO EFFECT, 150 and
           152 words.  The fall-through successor of block 0 carries no
           CODE_LABEL, so the backward scan walks straight into block 0.
         * `goto L1; L1:` between the two copies -- NO EFFECT, 152/13.  Both
           the jump and the now-unreferenced label are gone by jump2.
         * make the word-6 copy HImode (so `GET_MODE (value)` != SImode, which
           DOES defeat the check): `u16` holder spellings are 152/10 -- combine
           folds the truncating move away, and when it does not, every one of
           the holder's three uses needs a zero_extend the target has no room
           for.  A narrow PROTOTYPE cannot help either: mips.h:1325 defines
           PROMOTE_PROTOTYPES, so a declared `s16` parameter is promoted at
           the call site and costs `sll`/`sra`.
         * TAKE THE TEMP OUT OF $a0 -- THE ONLY ROUTE THAT WORKS.  Keeping `f`
           live past the copy forces the srl/andi off $a0 and BOTH copies then
           survive: 153 words, every index instruction present, 10 rows, all
           of them register names:

               k = (f >> 9) & 7;
               __asm__("" ::"r"(k));          -- 2nd use: blocks (2) above
               m = (u8 *)D_8008333C;
               kz = (u16)k;   kk = kz & 7;    -- move: word 6
               kz2 = (u16)kk; ka = kz2 & 7;   -- move: word 22
               __asm__("" ::"r"(f));          -- keeps $a0 busy

           gives `srl $v0,$a0,9 / andi $v0,$v0,7 / move $s4,$v0 / sll $s3,$s4
           / move $a0,$s4`.  Word-exact and structurally complete, but the
           temp is $v0 where the target has $a0 and the index is $s4 where the
           target has $s5, and the $s4/$s5 swap costs five more rows.  Moving
           the second asm earlier is the same 153/10; folding both asms into
           one with two operands is 153/15.

       So EITHER the temp is in $a0 and word 22 is deleted, OR the temp is not
       in $a0 and word 4/6 name the wrong register.  Nothing in between was
       found.  Two things would explain the target and are worth checking
       before more spellings: a word-6 copy that is not an SImode reg-reg SET
       (an `addsi3` whose second operand is the literal `$zero` register
       assembles to the same word as `move`, and find_equiv_reg's
       `true_regnum (SET_SRC (pat))` is -1 for a PLUS), or something that
       writes $a0 between words 6 and 22 in the RTL and is gone from the final
       output.

   (4) THE INDEX LANDS IN $s5 ONLY WITH TWO GLOBAL INDEX RANGES.  With one
       global range it is $s4 and `wx` takes $s5, which renumbers five rows
       (every 152/8 and 152/10 variant above).  The body below has exactly two
       (k and ka), which is why rows 10, 71 and 85 are clean here and were not
       in any earlier candidate.

   (5) `one = 1;` IS NO LONGER USEFUL.  The park's old word-count device,
       which bought 153/6, costs 22 rows on this body (29 if assigned before
       `k`): the extra live range renumbers the callee-saved file.  The one
       missing word has to come from the copy, not from a device.

   -------------------------------------------------- PRIOR MEASUREMENTS, KEPT
   (j40 / permuter / j60 -- do not re-run these)

     * THE BOUNDS CHECK AND THE STEP TARGET ARE THE SAME TWO LOCALS.  Separate
       `tx`/`tz` for the pre-check is 81 rows; reusing `ax`/`az` is 26.  That
       is what puts the two sums in $s1/$s0 and forces `s16 y` out to the
       `sh $a3, 0x18($sp)` spill slot.  DO NOT DISTURB.
     * THE TAIL IS `if (t >= 0x201) return -1; return 1;`.  With `return -1`
       last, cross-jumping merges all five `return -1` bodies and the target's
       shared `.L800CA2B4` block disappears: 150/26.  `r = -1; if (...) r = 1;`
       and `return t < 0x201 ? 1 : -1;` are both 152/30.
     * the two bounds tests as FOUR separate `if`s: 28 rows.  As ONE `if` with
       four `||` terms: 83 rows.
     * `m` must be a LOCAL `u8 *m = (u8 *)D_8008333C;`.  Folding the
       displacement into the relocation costs the shared base: 152/24.
     * `h = *(u16 *)(d + 0x88)` hoisted above the bounds checks: 150/34.
     * `s32 kk = k;` with EVERY index use moved to kk was the permuter's win
       at 152/5; moving only some uses is 151/17.  On this body it is 152/2
       with or without the kk rename, so the rename is now redundant.
     * BYTE-IDENTICAL to the no-device body (j40 + j60, 21+ spellings): a third
       index local as `k3 = kk;`/`k3 = k;`; `kq = kk & 7`; the argument inline;
       `f = kk;`/`f = k;` as host; `register s32 kk`; `D_8006CCD8[kq = kk]`,
       `func_8009A540(kq = kk, ...)`; `kk = k;` twice; `k` as `u32`; the rename
       taken to completion; early-on-k/late-on-kk and the reverse; swapping the
       k/m/ax statement order; `kk = (u16)k` with one use of k.
     * SCORED WORSE: `k = f >> 9; k &= 7;` on top of the rename 153/16;
       `volatile s32 k3` 154/113; `kk` as s16/u16/u8/s8 152/105; `k` as
       s16/u16/u8 151/148; `f = (f >> 9) & 7;` with `f` everywhere 45 rows;
       `one = 1;` ahead of `k` or `m` 153/11.
     * NO PASS INSERTS A REGISTER COPY THE SOURCE DID NOT ASK FOR: this tree's
       regmove.c has no `copy_src_to_dest`, and shorten-life.c runs only under
       `-fshorten-lifetimes`, which is 0 at toplev.c:482 and gated at :3567.
     * cse's `make_regs_eqv` over its extended-basic-block traversals, not the
       source, decides which member of a copy's class gets which use: in the
       short traversal the later pseudo is canonical for block-0 uses, in the
       long ones the earlier one keeps the late uses.  The `.regmove` dumps of
       the swapped and unswapped sources are identical insn for insn.

   FOR THE PERMUTER: the body below is 152/2 and its content is the closest
   any candidate has been.  The word-exact 153/6 body the permuter produced in
   October is kept under `#if 0` at the bottom in case word count is wanted
   instead; it is 6 rows and four of them are the index.  The 153/10 asm body
   in (3) is word-exact AND structurally complete and is the third thing worth
   feeding, but it needs the $s4 -> $s5 renumbering to pay off. */

s32 func_800CA1E0(u32 f, u8 *c, u8 *d, s16 y)
{
    u16 w;
    u16 ax;
    u16 az;
    u32 px;
    u32 pz;
    s32 wx;
    s32 wz;
    s32 k;
    s32 ka;
    s32 kz;
    s32 h;
    s32 t;
    s16 n;
    u8 *m;

    k = (f >> 9) & 7;
    m = (u8 *)D_8008333C;
    kz = (u16)(f >> 9);
    ka = kz & 7;
    ax = c[0x24] + (u16)D_8006CCD8[k];
    if (ax == 0 || (1 << *(s16 *)(m + 0x14)) - 1 < ax)
    {
        return -1;
    }
    az = c[0x25] + (u16)D_8006CCE8[k];
    if (az == 0 || (1 << *(s16 *)(m + 0x16)) - 1 < az)
    {
        return -1;
    }
    h = *(u16 *)(d + 0x88);
    px = c[0x24] << 6;
    pz = c[0x25] << 6;
    wx = px + 0x20;
    wz = pz + 0x20;
    if (func_8009A540(ka, px >> 6, pz >> 6, h - y) == 0)
    {
        return 0;
    }
    ax = D_800DCEAC[k] + wx;
    az = D_800DCEBC[k] + wz;
    func_8009A350(c[0x24], c[0x25], k, &w);
    if ((w & 0x8002) != 0)
    {
        return 0;
    }
    if ((s8)c[0x26] < 0)
    {
        n = func_8009FB34(c[0x24] + (u16)D_8006CCD8[k], c[0x25] + (u16)D_8006CCE8[k]);
        if (n >= 0 && (D_800E2970_s[n].unk_0C & 2) != 0 && (*(s32 *)(d + 0x1C) & 0x2000) == 0)
        {
            return 0;
        }
    }
    t = func_800BCB04(ax, az, h - y);
    if ((s16)t >= 0x201)
    {
        return -1;
    }
    return 1;
}

/* The permuter's October body: 153 words (word-exact), 6 aligned rows.
   Kept only as a second thing to feed the permuter; the live body above is
   152 words but 2 rows. */
#if 0
s32 func_800CA1E0(u32 f, u8 *c, u8 *d, s16 y)
{
    u16 w;
    u16 ax;
    u16 az;
    u32 px;
    u32 pz;
    s32 wx;
    s32 wz;
    s32 k;
    s32 kk;
    s32 one;
    s32 h;
    s32 t;
    s16 n;
    u8 *m;

    k = (f >> 9) & 7;
    m = (u8 *)D_8008333C;
    kk = k;
    one = 1;
    ax = c[0x24] + (u16)D_8006CCD8[kk];
    if (ax == 0 || (1 << *(s16 *)(m + 0x14)) - 1 < ax)
    {
        return -1;
    }
    az = c[0x25] + (u16)D_8006CCE8[kk];
    if (az == 0 || (1 << *(s16 *)(m + 0x16)) - 1 < az)
    {
        return -one;
    }
    h = *(u16 *)(d + 0x88);
    px = c[0x24] << 6;
    pz = c[0x25] << 6;
    wx = px + 0x20;
    wz = pz + 0x20;
    if (func_8009A540(kk, px >> 6, pz >> 6, h - y) == 0)
    {
        return 0;
    }
    ax = D_800DCEAC[k] + wx;
    az = D_800DCEBC[k] + wz;
    func_8009A350(c[0x24], c[0x25], k, &w);
    if ((w & 0x8002) != 0)
    {
        return 0;
    }
    if ((s8)c[0x26] < 0)
    {
        n = func_8009FB34(c[0x24] + (u16)D_8006CCD8[kk], c[0x25] + (u16)D_8006CCE8[kk]);
        if (n >= 0 && (D_800E2970_s[n].unk_0C & 2) != 0 && (*(s32 *)(d + 0x1C) & 0x2000) == 0)
        {
            return 0;
        }
    }
    t = func_800BCB04(ax, az, h - y);
    if ((s16)t >= 0x201)
    {
        return -1;
    }
    return 1;
}
#endif
