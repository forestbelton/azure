/* func_8009C93C (ovl_dungeon) -- 261 of 263 words, 33 ALIGNED DIFFERENCES.
   Agent j49, 2026-11-16.  Cold start; no park existed.

   SIGNATURE, settled from the three landed callers and the listing:
     u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, s32 dist, u8 *e);
   The fifth argument arrives at 0x40($sp).  The three landed call sites
   (csrc/ovl_dungeon.c:2909, :8026, :8826, :33379) ride an IMPLICIT
   declaration today, so landing this needs the forward declaration above
   added to the unit's declaration block AND those four callers re-measured.

   FOUR of the 33 are the PROVED-UNREACHABLE `|= 0x80000000` gate
   (docs/dungeon-sweep.md, "A PROVED-UNREACHABLE RESIDUE EXISTS IN THIS
   BLOCK"), at candidate words 18/20/21/22:

       mine    lw $v1, 0x14($s3) / bltz $v1 / lui $v0, 0x8000 / or $v0,$v1,$v0
       target  lw $v0, 0x14($s3) / bltz $v0 / lui $v1, 0x8000 / or $v0,$v0,$v1

   Nothing was spent on those four.

   THE TWO MISSING WORDS ARE ONE THING: the target keeps the re-aimed angle
   in a SECOND pseudo and the candidate coalesces it into the parameter.
   Target words 9 and 29 are `addu $a1, $s0, $zero` (w = ang, before the
   `bnez $s2` gate) and `addu $a1, $v0, $zero` (w = the new angle, before the
   `sh`); `$s0` stays the parameter and is read at word 28 as the addend,
   `$a1` is read at word 32 by `srl $v0, $a1, 8`.  Every spelling measured
   lets the two share one register: `addu $s0, $s0, $v0` then
   `srl $v0, $s0, 8`, and both copies disappear.  Measured, all against the
   261-word base below unless noted:

     w = ang; ... w = ang + ((f() & 7) << 9);        261 / 33  (the body)
     the same with a temp: v = ang + ...; w = v;     263 / 47  (the two words
                                                     come back as a tail nop
                                                     and a stray `move`, NOT
                                                     as the $a1 copies)
     w = ((f() & 7) << 9) + ang;  (operand order)    261 / 33, identical md5
     a duplicated `if (b != NULL)` arm around the
       `|= 0x80000000` store, as a cse-region
       boundary (func_8009704C's device)             262 / 37  -- it DOES buy
                                                     one of the two words, so
                                                     the cause really is cse
                                                     carrying `w == ang` into
                                                     the inner block, but it
                                                     costs four elsewhere

   THE REST, in order, each already reduced as far as hand spellings reached:

   * words 135-145, the +0x84 draw.  Target: `lbu $s1, 0x84($s3)`,
     `addu $s0, $s1, $zero`, `andi $v1, $v0, 0xFFFF`, `beqz`,
     `addu $s1, $s0, $zero` (delay), `div $zero, $v1, $v0`.  The candidate
     gets the two copies the right way round but leaves a `nop` in the beqz
     delay slot where the target has the second copy.  Adding a redundant
     `k = t;` before the `if` DOES fill that slot but moves the
     `andi 0xFFFF` one word later: 263 / 47 as a whole, no net gain.
     `s16 k` + `s32 t` reused across the function is what fixed this block
     from 11 differences to 5 -- see below.

   * words 173-181, the second draw.  Target copies the u16 result into a
     callee-saved register with a plain `addu $s1, $v0, $zero` and then
     `andi $s0,$s1,1` / `andi $v0,$s1,2` / `sltu $v0,$zero,$v0`.  With an
     `s32` local the assignment costs the `andi $v0, $v0, 0xFFFF` widening
     and combine then rewrites `(r & 2) != 0` as `(r >> 1) & 1`: same word
     count, 6 differing.  Measured: `u16` local 262 / 57 (the copy vanishes
     entirely), `s16` local 262 / 57, reusing `k` 263 / 57.  The `s32` form
     below is the best of the four.

   * words 196-203, the +0x03 clamp: register numbers only.

   * the prologue, words 1-12: the candidate loads the stack argument FIRST
     and the target loads it fourth, so the five `sw $sN` saves interleave
     with the parameter copies in a different order.  sched1 output; no
     source spelling reached it.

   WHAT GOT IT FROM 117 TO 33, and these all generalise:

   * TWO SCRATCH LOCALS, `s16 k` and `s32 t`, REUSED FOR ALL FOUR BLOCKS.
     The target's $s1/$s0 pair carries (a[0x84], its copy), then
     (a[3]>>1|a[3]>>6, its widening), then the remainder.  The s16->s32
     direction is what emits `sll $s0,$s1,16` / `sra $s0,$s0,16` at word
     198: with `s32 n` holding the shift and `s16 m = n` the extension is
     folded away and the function is a word short there.  Worth 3 words.
   * `k = t;` BEFORE the `if`, not an `else` arm: an `if`/`else` emits a `j`
     the target does not have.  Worth 2 words.
   * `g = D_800E3D7C;` READ INTO A LOCAL BETWEEN the two `&=` stores of the
     tail.  Read inline in the compare, the `lui`/`lw` pair sits after both
     read-modify-writes, the load-delay `nop` survives and reorg fills the
     `bne`'s delay slot with a `move` instead of the `sw $v0, 0x14($s2)`
     the target puts there: 14 aligned differences across words 232-249.
     Read before BOTH stores instead, it is 261 / 47.  This is
     docs/matching-levers.md's "WHERE A GLOBAL IS READ DECIDES A DELAY-SLOT
     FILL" with the read site one statement later than that entry's.

   TRIAGE: the counts differ by two inside ONE identified block and the
   structure either side of it is proved, so this is permuter food by
   docs/dungeon-sweep.md's rule, not a .greg job.  Queue it. */
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, s32 dist, u8 *e)
{
    s16 k;
    s32 i;
    s32 r;
    s32 t;
    u8 *g;
    u32 w;

    w = ang;
    if (e == NULL)
    {
        if ((*(s32 *)(a + 0x1C) & 0x400) != 0 && *(s32 *)(a + 0x14) >= 0)
        {
            *(s32 *)(a + 0x14) |= 0x80000000;
            w = ang + ((func_800A6D30() & 7) << 9);
            *(s16 *)(a + 0x2A) = w;
        }
    }
    i = (w >> 8) & 0xE;
    func_800C78A0(a - 0x20, b[0x24] * 64 + (*(s16 *)((u8 *)D_800DCEAC + i) >> 1) + 0x20,
                  b[0x25] * 64 + (*(s16 *)((u8 *)D_800DCEBC + i) >> 1) + 0x20,
                  *(s16 *)(a + 0x88), 8, 0x300);
    *(u8 **)(a + 0x60) = e;
    *(s32 *)(a + 0x14) &= ~0x4000000;
    if (e == NULL)
    {
        if (func_8009A540((*(u16 *)(a + 0x2A) >> 9) & 7, b[0x24], b[0x25],
                          *(u16 *)(a + 0x88) - 0x20) == 0)
        {
            return NULL;
        }
        e = func_8009B25C(a, b[0x24] + *(u16 *)((u8 *)D_8006CCD8 + i) * dist,
                          b[0x25] + *(u16 *)((u8 *)D_8006CCE8 + i) * dist,
                          *(s16 *)(a + 0x88));
        *(u8 **)(a + 0x60) = e;
        if (e == NULL)
        {
            return NULL;
        }
    }
    *(u16 *)(e + 0x6A) = *(u16 *)(a + 0x2A);
    if (e[0x13] != 0)
    {
        if ((*(s32 *)(e + 0x1C) & 0x238) == 0 && (*(u16 *)(e + 0x46) & 0x8000) == 0)
        {
            *(u16 *)(e + 0x2A) = *(u16 *)(a + 0x2A) + 0x800;
        }
    }
    *(s32 *)(a + 0x14) |= 0x4000000;
    *(s32 *)(e + 0x14) = (*(s32 *)(e + 0x14) | 0x20000) & ~0x1000000;
    t = a[0x84];
    k = t;
    r = func_800A6D30();
    if (a[3] != 0)
    {
        k = t + (r % a[3]) * 4;
    }
    if (e[0x13] == 0 && *(u8 **)(e + 0x50) != NULL && **(u8 **)(e + 0x50) == 2)
    {
        k -= k >> 2;
    }
    if ((*(s32 *)(e + 0x1C) & 0x238) == 0 && k < 0x80)
    {
        return e;
    }
    r = func_800A6D30();
    t = (r & 1) + ((r & 2) != 0);
    t += (a[0x26] + func_800981F8(a)) * 2;
    *(s16 *)(a + 0x20) = t;
    if ((*(s32 *)(a + 0x1C) & 0x1000) != 0)
    {
        k = a[3] >> 1;
    }
    else
    {
        k = a[3] >> 6;
    }
    t = k;
    if (t >= (s32)(func_800A6D30() & 0x7F))
    {
        *(s32 *)(a + 0x1C) |= 0x1000000;
    }
    else
    {
        *(s32 *)(a + 0x1C) &= ~0x1000000;
    }
    if ((*(s32 *)(e + 0x1C) & 0x238) != 0)
    {
        r = func_800A6D30();
        if (e[3] != 0)
        {
            k = r % e[3];
        }
        else
        {
            k = 0;
        }
        if ((s16)(k + e[0x70]) >> 7 > 0)
        {
            return e;
        }
    }
    *(s32 *)(a + 0x14) &= ~0x4000000;
    g = D_800E3D7C;
    *(s32 *)(e + 0x14) &= ~0x20000;
    if (a == g)
    {
        func_8003E188(e[0x13], 0);
    }
    return e;
}
