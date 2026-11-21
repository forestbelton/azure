/* PARK: func_8008E700, 208 of 208 words, **2 ALIGNED DIFFERENCES** -- ONE
   INSTRUCTION, ONE SLOT.  2026-11-20; was 6 rows (j84, 2026-11-16).

   The residue is the whole of it:

       target 137  addu $a0, $s2, $zero
       cand   138  move a0,s2

   Words 0..136 and 139..207 are BYTE-EXACT and every register in all 208
   words is correct.  The `addu $a0, $s2, $zero` is func_80048A44's first
   argument; the target puts it between the mask's `ori` (136) and
   `lui $a1, %hi(D_80081484)` (138), and every spelling measured puts it
   either at 135 (before the mask pair) or at 138.  Slot 137 has not been
   reached.  **THIS IS A PERMUTER TARGET, and a small one.**

   ======================================================= HOW IT GOT HERE

   j84 proved that no body keeping `*(u32 *)(d + 0x1C) &= ~0x100000;` as ONE
   statement can ever match, and that proof still stands -- it is reproduced
   below.  The live body is now the SPLIT it predicted, which j84 could only
   reach at 13 rows.  Two things closed the gap from 13 to 2:

   1. **THE SPLIT ITSELF** (j84's ALTERNATIVE 1, now the live body):

          m = *(u32 *)(d + 0x1C);          -- H, before the NULL store
          s = *(void **)D_80081484;
          *(void **)D_80081484 = NULL;     -- G
          k = ~0x100000;
          *(u32 *)(d + 0x1C) = m & k;      -- X and I, after G

      This is the only dependence set that is the target's: H before G
      (anti), I after G (output), nothing else.

   2. **THE MASK IS A LOCAL PINNED TO `$v1`** --
      `register u32 k __asm__("$3");`.  That is worth ELEVEN rows and it is
      the whole of j84's open question.  j84 had priced the residue off
      `.lreg` and could not move it:

          Register 182 (%hi(D_80081484))  used 3 times across 10 insns -> $v1
          Register 186 (the 0xFFEFFFFF mask) used 2 times across 8 insns -> $a1

      local_alloc sorts by `QTY_CMP_PRI = floor_log2(n_refs) * n_refs * size
      / (death - birth)` (local-alloc.c:1693) and hands each quantity the
      lowest-numbered free register, so 3/10 = 0.3 beats 2/8 = 0.25, the
      `%hi` is priced first and takes $v1, and the mask gets $a1.  The target
      has **mask $v1, `%hi` $a1**.  Winning that on priority needs the mask
      at 3 references (0.375), or its live range at 6 insns (0.333), or the
      `%hi`'s at 13 (0.231) -- and j84 measured every source-level route to
      all three and found none, because cse2 merges the `(high (symbol_ref))`
      however the pair is written and rematerialises a bare `~0x100000`
      whatever local you bind it to.

      **So do not fight the priority: name the register.**  A `register u32 k
      __asm__("$3")` local is immune to the rematerialisation that made
      `u32 k = ~0x100000;` a no-op, and it is the one lever that does not go
      through QTY_CMP_PRI at all.

      Three further spellings reach the same 2 rows and are interchangeable:
      the mask made opaque with `__asm__("" : "+r"(k));` after `k =
      ~0x100000;` instead of the pin; the same with `k` assigned first; and
      the pin plus `m` pinned to `$v0`.  **ALL 540 legal source orders of the
      split basin were re-swept with the pin in place** (L before or after P,
      `K`+`M` in each slot after Q, times the 30 legal permutations of
      XGYWVE with G before Y and X before W): **the floor is 2** and eight
      orders reach it --

          LPQGKMXYWVE  LPQGKMYXWVE  LPQGXKMYWVE  LPQKMGXYWVE
          LPQKMGYXWVE  LPQKMXGYWVE  LPQXGKMYWVE  LPQXKMGYWVE

      then 4, and up to 36 at the worst.  Sweeping the ARGUMENT COPY as well
      -- a `register u8 *cc __asm__("$4"); cc = c;` with the call rewritten
      to `func_80048A44(cc, ...)`, placed at all twelve positions in four of
      those orders -- is 2 everywhere except at the very front of the window,
      which puts the move at 135 instead of 138: 4 rows.  So the pin DOES
      control that insn's slot, and 137 is not among the slots it offers.

   ======================= WHY NO SINGLE-STATEMENT ORDER CAN EVER MATCH (j84)

   Name the window's insns by their final slots:

       H = lw  $v0, 0x1C($s0)                  (the +0x1C read)
       I = sw  $v0, 0x1C($s0)                  (the +0x1C write-back)
       E = lw  $t0, %lo(D_80081484)($a1)       (`s = *D_80081484`)
       G = sw  $zero, %lo(D_80081484)($a1)     (`*D_80081484 = NULL`)

   The target emits H(139) ... G(142) ... I(147): the +0x1C READ is before
   the D_80081484 STORE and the +0x1C WRITE is after it.

   `*(u32 *)(d + 0x1C)` is `(mem (plus (reg) 28))` -- non-struct, varying --
   and `*(void **)D_80081484` is `(mem (lo_sum (reg) (symbol_ref)))`, which
   `rtx_varies_p` (rtlanal.c, case LO_SUM) calls NON-varying and which is
   non-struct.  Neither side gets the `MEM_IN_STRUCT_P && rtx_addr_varies_p`
   escape in `true_dependence`/`anti_dependence`/`output_dependence`
   (sched.c:846-911), and `memrefs_conflict_p` cannot prove two non-constant
   addresses apart, so BOTH edges exist:

     * A after Q  (`P Q A`) -> true_dependence pins H AFTER G.  Measured: H
       lands at 142 and G at 141.  H can never reach 139.
     * A before Q (`P A Q`) -> output_dependence pins I BEFORE G.  Measured:
       I lands at 144 and G at 147.  G can never reach 142.

   A single `&=` statement puts H and I on the same side of Q, so one of the
   two edges always points the wrong way.

   `((u32 *)d)[7] &= ~0x100000;` DOES set `MEM_IN_STRUCT_P` and so deletes
   BOTH edges (expr.c:5343-5351: `TREE_CODE (TREE_OPERAND (exp, 0)) ==
   PLUS_EXPR`, which `((u32 *)d)[7]` has and `*(u32 *)(d + 0x1C)` does not).
   It does not help -- with no edge every placement collapses to the same bad
   schedule, mask in $a3, and the +0x1C load still AFTER the store, 19-23
   rows for all nine placements.  And the registers say the target does not
   use the escape here: the landed sibling `func_8008E504`, which the
   array-ref rule was written against, puts its two masks in $a2/$a3, and
   every array-ref spelling of THIS function puts the mask in $a3, while
   func_8008E700's target has $v1 -- which is what the cast spelling gives.

   The comma trick that would split the RMW without a temp does not work:
   `*(u32 *)(d + 0x1C) &= (*(void **)D_80081484 = NULL, ~0x100000);` is 6 and
   byte-identical to the `P Q A` body, because expand_expr evaluates the
   side-effecting operand of a commutative binop FIRST.

   ====================================== j84's OTHER MEASUREMENTS, STILL VALID

   Mixed forms, which put the MEM_IN_STRUCT escape on ONE HALF of the clear
   only -- a mechanism worth keeping because it is the only way to break one
   edge without the other.  `S` = `((u32 *)d)[7] = *(u32 *)(d + 0x1C) &
   ~0x100000;`, `T` = the reverse: SPQXGYWVE 19, PQTXGYWVE 15, and all 180
   legal orders of that family best 14.  `SPQXGYWVE` is the only no-temp body
   with the target's SHAPE, which is what confirms the dependence functions
   behave as described.

   Temp variations on the split, WITHOUT the pin: reusing `r` instead of a
   fresh `m` (so the temp has two sets and loses the `birthing_insn_p` boost)
   13-20; reusing `v` 17; `g2` as a `u8 *` 18; the temp's TYPE makes no
   difference at all (`u32`, `s32`, `int`, `unsigned long` all 15 on
   PLQXMGYWVE); splitting the `and` off the write-back so the temp gets a
   second set changes nothing (13, same as not splitting); `m & 0xFFEFFFFF`
   written out instead of `m & ~0x100000` no change.  Without the pin `L`
   must precede `P` and the floor is 13.

   Three spellings of the D_80081484 pair that might have split the `%hi`
   into two pseudos (`*(u32 *)D_80081484 = 0;`, `(void *)*(u32 *)D_80081484`,
   `*(u8 **)D_80081484 = NULL;`) are every one bit-identical to the baseline.

   And j42's park claimed `PAQXDYVC` was "6 and byte-identical" to
   `PQAXDYVC`.  IT IS NOT -- two different streams that score the same, and
   that claim is what kept the split basin unexplored for a day.
   **Re-measure a "byte-identical" claim before you believe it.**

   DECLARATIONS
   extern u8 D_800DD048[];
   extern u8 D_800DD060[];
   extern u8 D_800DCFD0[];
   END DECLARATIONS
   (D_800DD0C8, D_80081484, D_800E3540, D_80083228, D_80083460, func_80048A44,
   func_8009F644, func_800A56E0, func_80099F04, func_80099F70, func_8008ACDC
   and func_8008EAC8 are all already declared.)

   Measured with
     python3 tools/decomp/parkresidue.py func_8008E700
     python3 tools/decomp/parksplice.py func_8008E700 <unit copy>
*/
void func_8008E700(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *g2;
    u8 *t;
    u8 *t2;
    void *s;
    s16 n;
    s32 r;
    s32 v;
    u32 m;
    register u32 k __asm__("$3");

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            b->unk_14 = -0x160000;
            if ((*(u32 *)(d + 0x1C) & 0x100000) != 0)
            {
                *(u8 **)(c + 0x2C) = D_800DD0C8;
            }
            else
            {
                *(u8 **)(c + 0x2C) = D_800DD048;
            }
            func_80048A44(
                c, (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        break;
    case 1:
        n = ++*(u16 *)(p + 0x96);
        if ((*(u16 *)(p + 0xA2) & 0x10) != 0 && n >= 4)
        {
            b->unk_14 = 0;
            if ((*(u32 *)(d + 0x1C) & 0x100000) != 0)
            {
                *(u32 *)(p + 0x8C) = (u32)func_8008EAC8;
            }
            else
            {
                t = D_800DD060;
                *(u8 **)(c + 0x2C) = t;
                func_80048A44(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            }
            p[0x9B]++;
        }
        else if ((*(u32 *)(d + 0x1C) & 0x300000) == 0x100000 && (*(u16 *)0x80013714 & 1) == 0)
        {
            r = func_8009AF18(*(s16 *)(d + 0x2A), b, c, 8);
            if ((s16)r != 0)
            {
                ((u32 *)*(u8 **)(p + 0x124))[7] |= 0x200000;
                ((u32 *)d)[7] |= 0x200000;
                *(s16 *)(*(u8 **)(p + 0x124) + 0x8A) = r;
                (*(u8 **)(p + 0x124))[0x84] = 0x78;
                (*(u8 **)(p + 0x124))[0x85] = 2;
                func_8009F644(d, 0x30, *(s16 *)(p + 0x96), 0);
                m = *(u32 *)(d + 0x1C);
                s = *(void **)D_80081484;
                *(void **)D_80081484 = NULL;
                k = ~0x100000;
                *(u32 *)(d + 0x1C) = m & k;
                t2 = D_800DCFD0;
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) += 1;
                *(u8 **)(c + 0x2C) = t2;
                v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
                D_800E3540 = s;
                func_80048A44(c, t2[v], 5, 1);
                func_800A56E0(0x512);
            }
        }
        break;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            g2 = (u8 *)&D_80083460;
            if (*(s16 *)(g2 + 0xA) == 0)
            {
                if ((*(u32 *)(d + 0x1C) & 0x200000) != 0)
                {
                    *(u32 *)(d + 0x1C) &= 0xFFDFFFFF;
                    *(u16 *)(g2 + 0x2) |= 0x412;
                    func_80099F70(*(void **)(d + 0x5C));
                    func_80099F04(*(void **)(d + 0x5C));
                }
                *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
            }
        }
        break;
    }
}
