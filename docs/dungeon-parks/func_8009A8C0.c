/* func_8009A8C0 (ovl_dungeon) -- PARK, best candidate so far.
   target 184 words, candidate 184 words, 12 ALIGNED DIFFERENCES.
   (ovlverify masked: 12 differing.)  Was 184 / 43 before 2026-11-16 (j87).

   Template: the landed func_8009A66C (csrc/ovl_dungeon.c), the same probe
   one layer in.  EVERY INSTRUCTION IS NOW IN THE RIGHT SLOT: the 12 rows are
   two opcodes and one register rotation, nothing structural, and both are
   diagnosed to the compiler pass and the line of compiler source below.

   THE RESIDUE, IN FULL (this is the entire diff)

      row  candidate                 target
        5  sw   $s6, 0x38($sp)       sw   $s5, 0x34($sp)      <- rotation
        6  andi $s6, $a0, 0xFF       addu $s5, $a0, $zero     <- (A)
       10  sll  $s3, $s6, 1          sll  $s3, $s5, 1         <- rotation
       15  sw   $s5, 0x34($sp)       sw   $s6, 0x38($sp)      <- rotation
       22  andi $a0, $a0, 0xFFFF     addu $a0, $s5, $zero     <- (A)
       61  lhu  $s4, 0x88($t0)       lhu  $s6, 0x88($t0)      <- rotation
       62  addiu $s5, $v0, 0x20      addiu $s4, $v0, 0x20     <- rotation
       63  subu $a3, $s4, $a3        subu $a3, $s6, $a3       <- rotation
       69  addu $a2, $s6, $zero      addu $a2, $s5, $zero     <- rotation
       83  addu $s0, $v0, $s5        addu $s0, $v0, $s4       <- rotation
      137  subu $a2, $s4, $t0        subu $a2, $s6, $t0       <- rotation
      160  subu $a2, $s4, $t0        subu $a2, $s6, $t0       <- rotation

   (A) THE TWO `andi`s THAT SHOULD BE MOVES.  The target computes
   `(f >> 9) & 7` into $a0 (a block-local pseudo, call it kx), then copies it
   to $s5 (k) at word 6, and copies THAT to $a0 again at word 22, where it
   waits twenty-odd words and four conditional branches for the
   `jal func_8009A540`.  Three pseudos, two surviving MOVES.
   A plain `k = kx;` / `k2 = k;` can never survive: cse.c make_regs_eqv puts
   the destination of a reg-to-reg copy into the SOURCE's quantity, and
   canon_reg then rewrites every use of the destination back to
   qty_first_reg, so the copy goes dead and flow deletes it.  The one escape
   in make_regs_eqv is that NEW becomes canonical when it "will live longer
   than any other reg of the same qty, and that is beyond the current basic
   block" -- true for `k = kx` but NOT for `k2 = k`, because k (last used at
   func_8009A350's argument, word 69) outlives k2 (last used at word 65).
   So at most one of the two can be a plain copy, and measuring every
   combination confirms it (see the kx table below).
   What DOES survive is a widened recomputation: cse cannot hash
   `(and kx 0xFF)` into kx's class, so it gets its own quantity.  The cost is
   that the surviving insn is an `andi` rather than an `addu`.  combine would
   fold the mask away -- nonzero_bits(kx) = 7 -- but its LOG_LINKS do not
   cross a basic block and kx has two uses, so neither fold is reachable.
   A HImode copy (`s16 k = kx;`) IS emitted as a bare `move`, but then every
   use of k needs a sign/zero extension the target does not have.
   SO: the thing to find is a third SImode-valued expression of kx that
   cse will not hash into kx's class and that costs one `addu`.  Nothing in
   C obviously spells that; this is the better half of the permuter's job.

   (B) THE ROTATION IS ARITHMETIC, AND HERE IS THE EXACT CONDITION.
   global.c:587 allocno_compare:
       pri = floor_log2(n_refs) * n_refs / live_length * 10000
   and `.lreg` prints both numbers ("Register N used R times across L insns").
   find_reg (global.c:907) then scans hard regs 0..31 -- MIPS defines no
   REG_ALLOC_ORDER -- and "never allocates a register for the first time in
   pass 0", so pass 0 only REUSES a register already in use (that is how wx
   gets $s0: its range is disjoint from az's second incarnation) and pass 1
   takes the lowest free one.  For this body:
       az  5/43 = 2325   ax  5/45 = 2222   c 10/150 = 2000   k<<1 5/56 = 1785
       h   4/67 = 1194   wx  2/18 = 1111   wz  2/18 = 1111   k    3/50 =  600
       &D_8006CCE8[k] 3/52 = 576   &D_8006CCD8[k] 3/67 = 447
       d   5/238 = 420   y   3/98 = 306            (d and y both spill)
   which allocates az=$s0 ax=$s1 c=$s2 k<<1=$s3 h=$s4 wx=$s0(reused)
   wz=$s5 k=$s6 &E8=$s7 &D8=$fp.
   THE TARGET'S ORDER MUST BE ... k<<1, wz, k, h, so that wz=$s4, k=$s5,
   h=$s6.  With h at 1194 that needs
       pri(wz) >= pri(k) > 1194
   and the only cell that fits without disturbing k<<1 (1785) is
       wz at 3 refs / length 18 = 1666, with k back at 4 refs / 50 = 1600.
   Equivalently wz at 2 refs with length <= 12.  k is at 4 refs whenever the
   func_8009A540 argument is `k` rather than a second copy of kx (see the kx
   table), so the ONE missing fact is a third reference to wz.
   flow.c:2122 ("this counts a register twice if it is modified, but that is
   correct") says an IN-PLACE modify counts twice, and flow runs BEFORE
   combine, so a reference combine later deletes is free.  flow.c:2122 also
   weights each reference by `loop_depth`.  Everything tried for that third
   reference is in the rejected list.

   THE FIVE LEVERS THAT TOOK THIS FROM 43 ROWS TO 12.  Each was measured on
   top of the previous; all five are needed, and the first three were each
   worth more than every spelling sweep in the old park put together.

   1. `u8 *d` PLAIN -- NO `volatile`, NO `(void)&d;`.  The target's
      `lw $t0, 0x50($sp)` at words 55, 118, 148 and `lw $a0, 0x50($sp)` at
      8009AB1C are RELOAD reloads of a SPILLED PSEUDO, not reads of an
      addressable variable: reload picks $t0 for them, and local-alloc would
      have picked $v0/$v1.  `d` spills because this body finally puts nine
      pseudos in $s0..$s7/$fp, which is every callee-saved register, leaving
      d and y as the two lowest-priority allocnos with nowhere to go -- and
      y spills too, to 0x18($sp), exactly as the target does.
      Measured, on top of levers 2-4:  plain d, no (void)&d  183w/14 rows;
      volatile d, no (void)&d 183w/25;  volatile d + (void)&d 184w/26;
      plain d + (void)&d 182w/33.  The old park's `(void)&d;` was holding
      `d` in a register and letting cse forward the incoming $a2 into the
      first `*(u16 *)(d + 0x88)`, which is what kept $a2 away from the
      `(s16 *)D_8008333C` base and suppressed all four reloads.

   2. `kx` / `k` / `k2` -- THREE pseudos for the facing index, with the
      func_8009A540 argument taken from its OWN copy in the FIRST basic
      block.  This is what puts a copy of k in $a0 at word 22, which the old
      park called a hoist nothing could reproduce.  It is not a hoist: the
      insn is a source-level copy in block 1, and global_alloc gives its
      pseudo $a0 (free, no call in its range), after which the call's own
      `(set (reg a0) k2)` is a no-op move that flow deletes.
      The kx table, all with levers 1, 3, 4, 5:
        k = (u8)kx;  k2 = (u16)kx;   184 words, 12 rows   <- THIS BODY
        k = (u16)kx; k2 = kx;        183 words, 12 rows
        k = kx;      k2 = (u16)kx;   183 words, 14 rows
        no kx, k2 = (u16)k;          183 words, 14 rows
        no kx, k2 = (u8)k;           183 words, 14 rows
        no kx, k2 = (f >> 9) & 0xF;  183 words, 14 rows
        no kx, u16 k2 = k;           183 words, 14 rows
        k = (u16)kx; k2 = (u16)kx;   183 words, 25 rows
        no kx, k2 = k & 0xF;         183 words, 25 rows
        no kx, k2 = k (plain);       183 words, 36 rows   (copy dies)
        k2 assigned after `wz =`;    184 words, 36 rows
      `s16/s8/s32/u32 k2 = k` all die the same way.

   3. `if (ax == 0 || (1 << sh[10]) - 1 < ax) return -1;` -- THE FIRST TWO
      TESTS JOINED WITH `||`, AND THE SAME FOR THE az PAIR.  Worth 10 rows
      and 2 words.  With separate `if`s, reorg's fill_slots_from_thread
      owns the first test's `return -1` block, steals `addiu $v0, -1` into
      the branch's delay slot and redirects the branch to the epilogue; the
      `%lo(D_8008333C)` then has to be materialised a word early and the
      SECOND test comes out inverted as `beqz` + `j` + `addiu -1`.  Joining
      them makes the -1 block shared, so reorg cannot own it, and
      fill_simple_delay_slots puts `addiu $a2, %lo(D_8008333C)` in the slot
      instead -- which is the target's word 28.
      Grid (24 cells, rows/words), with volatile d at the time:
        or + or  + k2   26/184     or + sep + k2   28/182
        or + or  + no k2 36/183    sep + * + k2    38/185
        sep + * + no k2  43/184  <- the old park
   4. The ternary IN func_8009B25C's fourth argument, and the inverted tail
      (`(s16)t >= 0x201 -> return -1` first).  Both inherited from the old
      park and re-confirmed: the tail written the other way round is
      183 words / 43 rows here.
   5. x/ax and z/az SHARING one local each (inherited, not re-measured:
      separate locals were 180 words / 102 rows).

   ALSO MEASURED IN THIS BASIN AND REJECTED
   * all 24 declaration orders of wx/wz/k/h: every order with wx before wz
     is identical bytes; every order with wz before wx is 183 words / 92.
     It is NOT a priority tie-break -- the priorities above are equal for
     wx and wz and the tie goes to the lower pseudo number either way.
   * all 30 legal statement orders of the {h, px, pz, wx, wz} group
     (px before wx, pz before wz): every one is identical bytes.
   * all four placements of the kx/k2 declarations: identical bytes.
   * types: u16/u32 wx and wz, u16/u32 h, u16/u32 k, u32 kx, u32 k2, u32 t,
     s32 px/pz, `h = *(s16 *)(d + 0x88)`, `wz = (u16)(pz + 0x20)` -- all
     identical bytes.  ONLY ax/az matter: `s32`/`u32` for them is
     178 words / 70 rows, so they must stay 16-bit.
   * everything aimed at wz's third reference:
       `wz = pz; wz += 0x20;`                 183w/92
       `wz = pz + 0x10; wz += 0x10;`          identical bytes -- cse's
          fold_rtx associates the two constants, and `.lreg` confirms wz is
          still 2 refs / 18.
       `wz &= 0xFFFF;` after the set          182w/74 -- this DOES work
          (`.lreg`: wz becomes 4 refs / 19 = 4210) but overshoots: wz is then
          allocated before az/ax/c and takes $s0.  3 refs is the target,
          4 is too many.
       `wz = (u16)wz;`, `wx &= 0xFFFF` too    182w/74 (same cause)
       `do { wz = pz + 0x20; } while (0);`    178w/75; around both
          assignments 184w/45; around the ax/az pair 183w/31; around the
          whole {h,px,pz,wx,wz} group 178w/65.  The loop NOTE does give wx
          and wz 3 refs (`.lreg`: 3 refs each) -- flow weights a reference by
          loop_depth -- but it also grows their live_length from 18 to 25,
          so 3/25 = 1200 is still under k's 1600.  A loop note that does not
          lengthen the range would be the whole answer.
       `__asm__ __volatile__("" : "+r"(wz));` 177w/59 -- the asm is a
          scheduling barrier and wrecks the pre-call block.
   * swapping the `ax =` / `az =` statements: 182 words / 74 rows.  It is
     the right LEVER (`.lreg`: wz's length moves 18 -> 17 and wx's 18 -> 19)
     but the wrong direction, and the target needs the ax-then-az order
     because `az` sits in `jal func_8009A350`'s delay slot.
   * precomputing func_8009A540's arguments into locals (`hy = h - y`,
     `qx = px >> 6`, `qz = pz >> 6`) to shorten wz's range: identical bytes,
     cse removes the locals and wz stays at 18.
   * `sh` moved after the `ax` assignment: identical bytes.
     `((s16 *)D_8008333C)[10]` inline: 41 rows.
   * `t = 0;` before the `(w & 0x40)` test: identical bytes.
   * cc1psx's SN-only `shorten_lifetimes` pass (between sched1 and
     local_alloc) is NOT the mechanism behind the word-22 copy: the
     `.shorten` dump has the same 126 insns in the same order as `.sched`.

   FROM THE OLD PARK, STILL TRUE AND NOT RE-MEASURED IN THIS BASIN
   * the 4th argument of func_8009B25C as an `s16 hh` local assigned in an
     `if`: 182 words, 48 -- the load comes out `lhu` + `sll`/`sra` instead
     of the target's bare `lh`.
   * `k = f >> 9; k &= 7;` 60 rows; `k = (f & 0xE00) >> 9` 45 rows;
     `u16 f`, `(f >> 9) & 7` inline at the call sites: identical bytes.
   * PERMUTED 2026-11-16 from the OLD 43-row body, 30 min, 6 jobs:
     REJECTED -- its best output was 186 words / 88 rows AND semantically
     wrong (it dropped a `return -1` from the `ax == 0` arm).  THIS IS A
     DIFFERENT BASIN and is worth permuting again; the two things to permute
     for are named above, and both are small and local:
       (A) a third SImode expression of kx that costs one `addu` and that
           cse will not hash into kx's class;
       (B) a third reference to wz that costs no instruction and does not
           lengthen its live range past 18.

   TOOL NOTE, worth the five minutes it costs: a ONE-FUNCTION FILE -- this
   body plus the eleven externs and the one typedef it needs, in a file
   NAMED ovl_dungeon.c in its own directory -- reproduces the unit's
   measurement for this function EXACTLY (184 words, 12 rows, same stream)
   in 0.1 SECONDS instead of 30, so ovlverify/aligndiff become a sweep loop
   rather than a decision.  Every table above came out of one.  And
   `tools/decomp/dump.sh` on that file gives the `.lreg` refs/length numbers
   the allocation arithmetic is read off, and `greg`'s first line
   (";; N regs to allocate: ...") is literally allocno_order -- the order
   find_reg will be called in.
 */
s32 func_8009A8C0(u32 f, u8 *c, u8 *d, s16 y)
{
    s16 *sh;
    u16 w;
    u16 ax;
    u16 az;
    u32 px;
    u32 pz;
    s32 wx;
    s32 wz;
    s32 k;
    s32 h;
    s32 t;
    s16 n;
    s32 k2;
    s32 kx;

    kx = (f >> 9) & 7;
    k = (u8)kx;
    k2 = (u16)kx;
    sh = (s16 *)D_8008333C;
    ax = c[0x24] + (u16)D_8006CCD8[k];
    if (ax == 0 || (1 << sh[10]) - 1 < ax)
    {
        return -1;
    }
    az = c[0x25] + (u16)D_8006CCE8[k];
    if (az == 0 || (1 << sh[11]) - 1 < az)
    {
        return -1;
    }
    h = *(u16 *)(d + 0x88);
    px = c[0x24] << 6;
    pz = c[0x25] << 6;
    wx = px + 0x20;
    wz = pz + 0x20;
    if ((s16)func_8009A540(k2, px >> 6, pz >> 6, h - y) == 0)
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
    if ((w & 0x3300) != 0)
    {
        if ((w & 0x40) == 0)
        {
            return -1;
        }
        t = func_800BCB04(ax, az, h - y);
        if (func_8009B25C(d, ax >> 6, az >> 6,
                          (s16)t < 0x201 ? (s16)t : *(s16 *)(d + 0x88)) != NULL)
        {
            return -1;
        }
    }
    else
    {
        t = func_800BCB04(ax, az, h - y);
    }
    if ((s16)t >= 0x201)
    {
        return -1;
    }
    return 1;
}
