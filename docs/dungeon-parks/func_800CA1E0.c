/* PARK func_800CA1E0 -- 151 of 153 words, 17 aligned differences (j40, 2026-11-16).

   Shape: func_8009A66C's twin (csrc/ovl_dungeon.c, 149 words, landed) with a
   map-bounds pre-check in front of it and a 1/-1 tail instead of 1/2/3.
   Probe the tile one step along the facing in `f`: reject the step if the
   target cell is 0 or past the map's 1<<unk_1F0 / 1<<unk_1F2 extent, ask
   func_8009A540 whether the step is legal, func_8009A350 for the tile's flag
   word, func_8009FB34/D_800E2970 for the room gate, and func_800BCB04 for
   the floor height.  Returns -1 blocked, 0 cannot, 1 ok.

   RESIDUE -- exactly two words, both in the prologue's handling of `k`:
     * 0x800CA1F8  addu $s5, $a0, $zero   (the target computes `(f >> 9) & 7`
                   into a SHORT-LIVED pseudo in $a0 and copies it into the
                   long-lived $s5; the candidate's `andi` writes the
                   long-lived register directly)
     * 0x800CA238  addu $a0, $s5, $zero   (that value copied BACK into $a0 in
                   the ENTRY basic block, 40 instructions before the
                   func_8009A540 call it feeds -- so the insn is in block 1 in
                   the RTL, not hoisted; the candidate emits its one
                   `move $a0, k` in the call's own block, where it also fills
                   the delay slot the target leaves as a `nop` at 0x800CA2B0)
   Every other row aligns and 15 of the 17 are register-number-only.

   MEASURED AND REJECTED (aligndiff rows at 150-152 words):
     * THE BOUNDS CHECK AND THE STEP TARGET ARE THE SAME TWO LOCALS.  Writing
       `tx`/`tz` for the pre-check and `ax`/`az` for `D_800DCEAC[k] + wx` is
       81 rows; reusing `ax`/`az` for both is 26.  That is the single biggest
       lever here: it is what puts the two sums in CALLEE-SAVED registers
       ($s1/$s0) and forces the `s16 y` parameter out to the `sh $a3,
       0x18($sp)` spill slot the target has, which is 2 of the 4 words the
       first draft was missing.
     * THE TAIL IS `if (t >= 0x201) return -1; return 1;`, NOT
       `if (t < 0x201) return 1; return -1;`.  With the `return -1` last,
       cross-jumping merges all five `return -1` bodies into one block at the
       end of the function, test 4's branch is inverted, and the target's
       shared `.L800CA2B4: j .L800CA410 / addiu $v0, $zero, -1` block
       disappears: 150 words / 26 rows.  With `return -1` first it is
       151 / 17.  `r = -1; if (t < 0x201) r = 1; return r;` is 152 / 30 and
       `return t < 0x201 ? 1 : -1;` is 152 / 30 (both leave a `move $v0,$v1`).
     * the two bounds tests as FOUR separate `if`s: 28 rows (vs 17 for two
       `if`s with `||`).  As ONE `if` with four `||` terms and the `az`
       assignment inside the condition: 83 rows.
     * `m` must be a LOCAL `u8 *m = (u8 *)D_8008333C;`.  Spelling the two
       reads `*(s16 *)((u8 *)D_8008333C + 0x14)` or `((s16 *)D_8008333C)[0xA]`
       folds the displacement into the relocation and costs the shared base
       register: 152 words / 24 rows.
     * `h = *(u16 *)(d + 0x88)` hoisted above the bounds checks: 150 / 34.
     * for the two missing words, all of these were measured and are
       BYTE-IDENTICAL to the 17-row body (cse collapses every one):
       `kk = (f >> 9) & 7; k = kk;`;  `dir = k;` with `dir` passed to
       func_8009A540 and/or func_8009A350;  `k = f >> 9; k &= 7;` split;
       `k` declared `u32` / `s16`;  `f = (f >> 9) & 7; k = f;`;
       `k = (f >> 9) & 7; f = k;`;  no `k` at all with `(f >> 9) & 7`
       written out at all five use sites;  and swapping the order of the
       `k` / `m` / `ax` statements.  Reusing the parameter outright
       (`f = (f >> 9) & 7;` and `f` everywhere) is 45 rows.

   Two words of allocator noise inside one 153-word function whose every
   other word aligns -- good permuter food.    ---------------------------------------------- 2026-11-16, permuted and improved

   PERMUTED, 25 minutes, 6 jobs: 660 -> 315 on its own metric, which
   re-measures with aligndiff as **152 words of 153 and 5 aligned rows**,
   from this park's 151 / 17.  Its two ideas were legal C this time
   (`D_8006CCD8[new_var2 = k]` and `return -(new_var = 1)`), and BOTH have
   legitimate equivalents that reproduce them exactly.  The body below is the
   legitimate form.

   (1) `s32 kk = k;` with **every** use of the index moved to `kk`.
       151/17 -> 152/5.  Moving only the first three uses and leaving the
       later `D_800DCEAC[k]` on the original is 151/17 -- no effect at all.
       See the catalogue entry "A SOURCE-LEVEL COPY CANNOT REPRODUCE AN
       ALLOCATOR COPY", which this refines: the copy pays when the original
       is left with no uses, and does nothing when it keeps some.

   (2) `s32 one = 1;` assigned at the top and the second bounds test written
       `return -one;`.  152/5 -> **153/6 -- the exact target word count.**
       Assigned later instead it is 151/22 or 152/5, so the position is the
       lever, not the local.

   WHAT IS LEFT, 6 rows: the target computes `(f >> 9) & 7` into `$a0` and
   copies it to `$s5`, then copies it BACK into `$a0` at word 22; this body
   computes straight into `$s5` and copies once.  And the target's
   `addiu $a1, $zero, 0x1` sits at word 30 where this has a `nop`, with the
   `li 1` at 22 instead -- the two instructions are in swapped slots.

   A THIRD index local for the copy-back does not buy it: assigned after
   `kk`, before the call, after the first bounds test, or also used for the
   later table reads, all four measure identically to not having it.  The
   copy is coalesced every time, which is the same entry's rule working
   correctly -- `kk` is still live, but the copy's own source is dead.

   Word-exact with every remaining difference a register name or a two-insn
   swap: `.greg`/`.sched` work by the triage rule, not another search.

   ---------------------------------------------- 2026-11-16, j60: RTL read,
   residue reduced to ONE instruction, and the lever ruled out by mechanism

   Reproduced 153/6 exactly before touching anything.  The body below is
   unchanged; what follows is why it does not move.

   THE WHOLE RESIDUE IS ONE INSTRUCTION.  Delete `one` (and write `return -1;`
   twice) and the body is 152 words / 5 rows, and the only row that goes away
   is the li/nop pair.  So `one`'s entire contribution is to move
   `addiu $a1, $zero, 0x1` out of block 1 -- where it naturally lands at word
   30 and fills the `lh`/`sllv` load-use slot -- into block 0, which costs a
   `nop` at 30.  It buys the word COUNT and nothing else.  The one honest
   missing instruction is the target's

       0x800CA238  addu $a0, $s5, $zero          (entry block, word 22)

   WHY IT IS MISSING, FROM THE DUMPS (tools/decomp/dump.sh, .cse2/.combine/
   .regmove/.lreg/.greg).  The body compiles to exactly TWO pseudos for the
   index and they survive to allocation:

       (insn 19  (set (reg/v:SI 92) (and:SI (reg:SI 99) (const_int 7))))
       (insn 25  (set (reg/v:SI 93) (reg/v:SI 92)))                  <- kk = k
       (insn 34  (set (reg:SI 105) (ashift:SI (reg/v:SI 93) 1)))     <- k*2 base
       (insn 140 (set (reg:SI 4 a0) (reg/v:SI 93)))                  <- A540 arg
       (insn 208 (set (reg:SI 6 a2) (reg/v:SI 92)))                  <- A350 arg

   92 crosses the func_8009A540 call, so global alloc must give it a callee-
   saved register (`92 in 21` = $s5).  93 crosses nothing and carries
   `;; 93 preferences: 4`, so it gets $a0 -- and insn 140 becomes
   `(set a0 a0)` and is deleted.  THAT deleted insn is the missing word.

   THE TARGET NEEDS THREE LIVE RANGES, NOT TWO, and no assignment of two
   pseudos can produce it:  A = the `andi` destination, in $a0, dead at word
   6;  B = the copy, in $s5, used by the `sll` at word 10 and by the $a2 arg
   at word 71;  C = a third value in $a0, DEFINED IN THE ENTRY BLOCK at word
   22 and dying at the call 43 instructions later.  If the `andi` writes $a0
   then the `sll` reads $s5, so A is not B; and word 22 rewrites $a0 after A
   is already dead, so C is neither.

   WHICH OF THE TWO PSEUDOS GETS WHICH USE IS NOT A SOURCE DECISION.  It is
   cse's `make_regs_eqv` in-block/out-of-block test applied over its extended
   basic-block traversals: in the short traversal (`;; Processing block from
   2 to 72`) 93's range leaves the traversal and 92's does not, so 93 becomes
   canonical and every block-0 use is rewritten to 93; in the long ones
   (`from 2 to 312`) 93's whole range is inside, the test fails, 92 stays
   canonical and the late uses are rewritten to 92.  Swapping every index use
   between `k` and `kk` in the source therefore changes nothing -- confirmed
   not just by the byte count but by diffing the `.regmove` dumps of the
   swapped and unswapped sources, which are identical insn for insn.

   MEASURED AND REJECTED by j60 (all BYTE-IDENTICAL to the 152/5 body unless
   a score is given -- that is 21 spellings on top of the list above):

     * a third index local used only as func_8009A540's argument, as
       `k3 = kk;` and as `k3 = k;`, each placed right after `kk = k;`
     * `kq = kk & 7;` in the entry block with `func_8009A540(kq, ...)`.  cse
       folds the AND outright: there is no kq pseudo left in `.cse2`.
     * the argument written inline as `kk & 7`
     * the parameter as host -- `f = kk;` and `f = k;` in the entry block with
       `func_8009A540(f, ...)`, with and without the early `D_8006CCD8` read
       also moved onto `f`
     * `register s32 kk`
     * embedded assignments (the shape the permuter liked):
       `D_8006CCD8[kq = kk]`, `D_8006CCD8[kk = k]`, `func_8009A540(kq = kk, ...)`
     * `kk = k;` written twice
     * `k` declared `u32`
     * ALL uses moved onto `kk` (the rename taken to completion, including
       `D_800DCEAC[kk]`, `D_800DCEBC[kk]` and func_8009A350's third argument)
     * early uses on `k` with late uses on `kk`, and the reverse
     * `k = f >> 9; k &= 7;` ON TOP OF the rename: 153 words / **16 rows** --
       the two sets renumber $s3/$s4/$s5 through the whole body.  Worse.
     * `volatile s32 k3` as the third local: 154 / 113
     * `kk` declared `s16`/`u16`/`u8`/`s8`: 152 / 105 (the frame shrinks by 8)
     * `k` declared `s16`/`u16`/`u8`: 151 / 148
     * `one = 1;` moved ahead of `k` or ahead of `m`: 153 words / **11 rows**
       (the constant lands in $a2 and D_8008333C's base in $a1).  Moved to
       just before `kk = k;` it is 153/6, the same as below -- so what matters
       is only that it is in the entry block, not where in it.

   SO THE LEVER IS NOT A SPELLING.  This is the catalogue's "A SOURCE-LEVEL
   COPY CANNOT REPRODUCE AN ALLOCATOR COPY" with one more turn of the screw:
   the RENAME form (original left with no uses) buys the SECOND pseudo and
   has a ceiling there -- it has never bought a third, and the third is what
   is missing.  gcc 2.7.2 has no pass that would insert it either: this tree's
   regmove.c has no `copy_src_to_dest`, and shorten-life.c runs only under
   `-fshorten-lifetimes`, which is off.

   FOR THE PERMUTER, TWO BODIES ARE WORTH FEEDING: the one below (153/6,
   word-exact) and the same body with `one`, `s32 one;` and `return -one;`
   removed, which is 152 words / 5 rows and whose CONTENT is strictly closer
   -- its only fault is the one deleted `(set a0 a0)`. */

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
