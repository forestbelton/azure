/* PARK func_800CA1E0 -- 153 of 153 words, 1 aligned difference (j82, 2026-11-16).
   Previous bests: 151/17 (j40), 153/6 (permuter + j60), 152/5.
   ONE WORD IS WRONG AND IT IS ONE OPCODE: the candidate has
   `andi $a0, $s5, 0x7` at word 22 where the target has `addu $a0, $s5, $zero`.
   Same slot, same registers, same destination.  Every other word of the 153
   is byte-identical.

   Shape: func_8009A66C's twin (csrc/ovl_dungeon.c, 149 words, landed) with a
   map-bounds pre-check in front of it and a 1/-1 tail instead of 1/2/3.
   Returns -1 blocked, 0 cannot, 1 ok.

   HOW THE INDEX IS BUILT, AND WHY EACH PIECE IS LOAD-BEARING.  The target
   spends four words on it:

       3 srl  $a0, $a0, 9      the parameter, dying
       4 andi $a0, $a0, 0x7    a SHORT-LIVED pseudo, dead after word 6
       6 addu $s5, $a0, $zero  the index: the sll at 10 and the $a2 arg at 71
      22 addu $a0, $s5, $zero  a THIRD pseudo, born in the entry block and
                               dying at the func_8009A540 call 43 insns later

   The body below reproduces words 3, 4, 6 and 10 exactly.  Three devices do
   it and none of them can be dropped:

     (a) `kz = (u16)k; kk = kz & 7;` is the word-6 copy.  combine folds
         `(and (zero_extend (subreg:HI k)) 7)` to `(reg k)` via
         significant_bits(k) == 7, so the insn is a bare move.  TWO STATEMENTS
         ARE REQUIRED -- `kk = (u16)k & 7;` as one statement is 152/10.
     (b) `__asm__("" ::"r"(k));` is k's SECOND USE.  With one use combine
         substitutes k's own `andi` into the widening and (a) collapses back to
         a single `andi` writing $s5 -- that is the whole family of
         byte-identical results j60 recorded, and it is what 152/10 looks like.
         No outputs, so the asm is implicitly volatile and survives flow; it
         emits nothing.  `__asm__("" : "=r"(k) : "0"(k))` in its place also
         blocks the absorption but poisons (a): 153/2, word 6 becomes an andi.
     (c) `__asm__("" : "=r"(kg) : "0"(kk));` with **kg pinned to $21** is what
         keeps the register file right, and the PIN is the load-bearing half.
         Without the pin the fence's extra allocno pushes the index to $s0 and
         `c` to $s3: 153 words / **36 rows**.  With it: 1 row.  `"+r"(kg)`
         measures identically to `"=r"/"0"`; the fence must NOT be volatile.

   -------------------------------------------- WHERE THE LAST WORD COMES FROM
   AND WHY IT IS AN `andi` AND NOT AN `addu`

   THE TARGET'S WORD-22 COPY IS REDUNDANT.  $a0 already holds the index from
   word 4 and nothing writes $a0 between words 4 and the call.  The original
   compiler emitted it only because its argument pseudo was a third, distinct
   pseudo that no pass connected to the first.  Every spelling in which cse can
   see that the value is already in $a0 therefore deletes it:

     * plain `ka = kk;`                            152 words / 8 rows
     * `func_8009A540(kk, ...)` with no `ka`       152 / 8
     * `kz2 = (u16)kk; ka = kz2 & 7;`  (reads the UNFENCED kk, so the mask does
       fold and the insn IS a `move $a0,$s5`)      152 / **1** -- see the
       alternative body under `#if 0`: 152 words, word 22 absent, EVERY OTHER
       WORD BYTE-IDENTICAL.  `(u8)kk`, `kk & 0xFFFF`, masks `& 0xF`, and a
       second fence before the device all measure the same 152/1.

   So the live body reads the FENCED, PINNED `kg` instead.  combine cannot
   prove significant_bits of a hard register whose last set is an asm, so the
   `& 7` survives as a real `andi` -- and an `andi` is not a reg-reg SET, so
   jump2's redundant-copy block never looks at it.  That is where the 153rd
   word comes from.  `& 0xF` in place of `& 7` is the same 153/1 with
   `andi $a0,$s5,0xf`.

   THE WALL, STATED EXACTLY.  jump2 is `jump_optimize (insns, 1, 1, 0)`
   (toplev.c:3690, so noop_moves == 1) and runs the block at jump.c:476-525.
   For every `(set (reg hard) (reg hard))` whose two `true_regnum`s are both
   >= 0 it calls

       find_equiv_reg (NULL_RTX, insn, 0, sreg, NULL_PTR, dreg,
                       GET_MODE (SET_SRC (body)))            [reload.c:5554]

   which scans BACKWARD over the linear insn stream -- stopping only at a
   CODE_LABEL -- for any insn copying into or out of $a0 whose other end is
   $s5, then rejects it only on `find_reg_note (where, REG_UNUSED, value)`,
   on `GET_MODE (value) != mode`, or on a set of $a0 or $s5 in between.  The
   word-6 copy is exactly such an insn, same block, both SImode.  Verified in
   the dumps: a variant with both copies carries
   `(insn 32 (set (reg/v:SI 4 a0) (reg/v:SI 19 s3)))` at precisely the
   target's word-22 slot through `.combine .regmove .sched .lreg .greg
   .sched2`, with dispositions `92 in 4`, `93 in 19`, `94 in 4` and
   `;; 94 preferences: 4`, and `.jump2` does not.

   A PREVIOUS VERSION OF THIS HEADER CLAIMED THE `"=r"/"0"` FENCE DEFEATS
   THAT TEST.  IT DOES NOT -- that was a misreading of a 153-word result whose
   extra word was the `andi` above, not a rescued move.  The fence's insn IS a
   SET of $s5 sitting between words 6 and 22 in `.greg` (insn 30, immediately
   after the copy and before the `sll`), and the move is still deleted.  Do
   not spend compiles on fence placement; three placements and two constraint
   spellings were measured and all are 152/1.

   BUT A CORRECT `addu $a0, $s5, $zero` AT WORD 22 IS REACHABLE -- JUST NOT
   WITH THE TEMP IN $a0.  Replace the argument device with a second,
   separately-masked RECOMPUTATION from the parameter:

       kz2 = (u16)(f >> 9);   ka = kz2 & 7;    -- or (u8)

   and word 22 comes out as the target's move (cse substitutes the register
   already holding `(and (lshiftrt f 9) 7)` into the SET_SRC without calling
   make_regs_eqv, so the use is never rewritten back).  That measures
   **153 words / 5 rows**: keeping the `srl` result live pushes the temp off
   $a0, so words 4, 6 and 20 take register rows instead.  On its own, with no
   front device at all, that recomputation lever is 152 words / 2 rows.

   **THAT IS THE PERMUTER'S TARGET: this body's front with that body's back.**
   The two halves are each exactly right and they fight over $a0.  The escape
   has to be either a word-6 copy that is not an SImode reg-reg SET -- note
   that an `addsi3` whose second operand is the literal `$zero` register
   assembles to the same word as `move`, and find_equiv_reg's
   `true_regnum (SET_SRC (pat))` is -1 for a PLUS -- or something that writes
   $a0 between words 6 and 22 and leaves no instruction.

   OTHER NEW MEASUREMENTS
     * `register s32 k __asm__("$3")` / `("$2")` / `("$5")` on the temp with
       the index pinned to $21 and a plain `kk = k;`: 152 / 23.  The temp pin
       is simply ignored -- cse merges the copy and the `andi` writes $s5.
     * pinning `ka` to `$4`: 152 / 5, and it breaks the front (the srl/andi
       leave $a0).  Pinning both temp and index: 153 / 2, both words become
       `andi`.
     * `one = 1;`, the old word-count device, is now actively harmful:
       153 / 22, or 153 / 29 if assigned before `k`.  The 153rd word comes
       from the argument device, not from a device elsewhere.
     * MIPS defines no REG_ALLOC_ORDER, so a lone block-local quantity takes
       $v0 in plain regno order; the temp here lands in $a0 only because $v0
       and $v1 are busy in the entry block.  That is why every attempt to
       move it costs words 3 and 4.

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

   FOR THE PERMUTER, THREE BODIES ARE WORTH FEEDING: the one below (153 words,
   1 row, one opcode wrong), the `#if 0` body at the bottom (152 words, 1 row,
   one word absent and everything else byte-identical), and the 153/5 body
   described above whose word 22 is already correct.  The October 153/6 body
   is superseded by all three. */

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
    register s32 kg __asm__("$21");
    s32 ka;
    s32 kz;
    s32 kz2;
    s32 h;
    s32 t;
    s16 n;
    u8 *m;

    k = (f >> 9) & 7;
    __asm__("" ::"r"(k));
    m = (u8 *)D_8008333C;
    kz = (u16)k;
    kk = kz & 7;
    __asm__("" : "=r"(kg) : "0"(kk));
    kz2 = (u16)kg;
    ka = kz2 & 7;
    ax = c[0x24] + (u16)D_8006CCD8[kg];
    if (ax == 0 || (1 << *(s16 *)(m + 0x14)) - 1 < ax)
    {
        return -1;
    }
    az = c[0x25] + (u16)D_8006CCE8[kg];
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
    ax = D_800DCEAC[kg] + wx;
    az = D_800DCEBC[kg] + wz;
    func_8009A350(c[0x24], c[0x25], kg, &w);
    if ((w & 0x8002) != 0)
    {
        return 0;
    }
    if ((s8)c[0x26] < 0)
    {
        n = func_8009FB34(c[0x24] + (u16)D_8006CCD8[kg], c[0x25] + (u16)D_8006CCE8[kg]);
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

/* ALTERNATIVE -- 152 words, 1 aligned difference.  Word 22 is ABSENT; every
   other word is byte-identical to the target, including words 3, 4, 6 and 10.
   Identical to the live body except that the argument device reads the
   UNFENCED `kk`, so its mask folds and the insn becomes a real
   `move $a0,$s5` -- which jump2 then deletes as redundant.  Strictly closer
   in content than the live body; one word short. */
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
    register s32 kg __asm__("$21");
    s32 ka;
    s32 kz;
    s32 kz2;
    s32 h;
    s32 t;
    s16 n;
    u8 *m;

    k = (f >> 9) & 7;
    __asm__("" ::"r"(k));
    m = (u8 *)D_8008333C;
    kz = (u16)k;
    kk = kz & 7;
    __asm__("" : "=r"(kg) : "0"(kk));
    kz2 = (u16)kk;
    ka = kz2 & 7;
    ax = c[0x24] + (u16)D_8006CCD8[kg];
    if (ax == 0 || (1 << *(s16 *)(m + 0x14)) - 1 < ax)
    {
        return -1;
    }
    az = c[0x25] + (u16)D_8006CCE8[kg];
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
    ax = D_800DCEAC[kg] + wx;
    az = D_800DCEBC[kg] + wz;
    func_8009A350(c[0x24], c[0x25], kg, &w);
    if ((w & 0x8002) != 0)
    {
        return 0;
    }
    if ((s8)c[0x26] < 0)
    {
        n = func_8009FB34(c[0x24] + (u16)D_8006CCD8[kg], c[0x25] + (u16)D_8006CCE8[kg]);
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
