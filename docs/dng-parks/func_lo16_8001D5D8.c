/* func_lo16_8001D5D8 -- ovl_dng_lo16, 253 target words.
   PARKED by agent h181, batch 35, 2026-11-25.  247 words / 197 ALIGNED
   DIFFERENCES.  THE CONTROL FLOW AND EVERY STATEMENT ARE DECODED -- what is
   left is SIX WORDS of reload traffic and the register rotation behind it.
   Read the residue section before touching the source; the shape below has
   been checked instruction by instruction against the listing.

   WHAT IT IS: the room filler.  Part 1 paints every cell of the room
   rectangle with kind 0x19 / height v - 0x80 / flags 6.  Part 2 scatters
   (w + h) / 4 blobs -- a quarter of that when D_8008146E < 5 -- each a
   bw x bh patch anchored at a random cell inside the room, pushing each cell
   through func_lo16_8001DD08 or func_lo16_8001DD48.  Part 3 draws the walls
   with func_lo16_8001D9CC.  Callees all live in this unit.

   THE SIX MISSING WORDS ARE ALL ONE CAUSE: THE TARGET SPILLS `n' AND KEEPS
   `bh' IN A CALLEE-SAVED REGISTER; THE CANDIDATE DOES THE OPPOSITE.
   Target: `sw $a0, 0x38($sp)' in the entry guard's delay slot, then
   `lw $t5, 0x38($sp)' / `nop' / `addiu $t5,-1' / `bgtz' / `sw $t5, 0x38($sp)'
   at the latch -- five words -- with `bh' living in $s6 throughout.
   Candidate: `n' takes a register, the latch is three words, and `bh' is
   spilled instead (`sh $s1, 0x58($sp)', which the target does not have).
   That single exchange also costs the target's
       lw $t5, 0x68($sp) / nop / sra $t5,$t5,16 / sw $t5, 0x64($sp)
   block at words 154-157, where loop.c has SPLIT the `(s16)xe' sign-extension
   pair -- the `sll' hoisted to the n-loop preheader and spilled as a word at
   0x68, the `sra' left inside the loop and its result spilled at 0x64.  The
   candidate computes `(s16)xe' once in the preheader, so it has neither the
   split nor the two extra reloads.  Same mechanism as func_lo16_80017F88's
   park, where the split is the fault rather than the goal.
   So: the whole job is to make `n' lose the allocation to `bh'.  Spellings
   measured on `n', all 197: the `if (D_8008146E < 5) n >>= 2' form, the
   ternary `(D_8008146E < 5) ? ((ww + hh) / 4 >> 2) : ((ww + hh) / 4)'.  An
   explicit `if/else' with the whole division in both arms is 256 words / 212
   -- it loses the combine fold described below.

   ONE DEAD INSTRUCTION IN THE TARGET AND IT IS DIAGNOSTIC.  Word 1 is
   `lui $v0, 0x8008' and $v0 is redefined at word 18 without being read, so it
   is dead.  0x8008 is %hi(D_8008333C), and it is what is left of a
   `g = (UnkDngLo16Grid *)D_8008333C;' in the ENTRY block after cse2 pushed
   the address into each use and flow deleted the `addiu' but not the `high'.
   The candidate below inlines `g' at both use sites and therefore has no such
   word; adding the local back gives the in-loop materialisation but costs 10
   rows elsewhere.  Measured: both sites inlined 197, `g' a local at both 207,
   local in loop 1 only 198, local in loop 2 only 209.  So the dead `lui' is
   worth one word and the next agent should expect to pay for it somewhere
   else; it is NOT free.

   MEASURED AND LOAD-BEARING:

   1. `n = (ww + hh) / 4;' THEN `if (D_8008146E < 5) n >>= 2;' -- TWO
      STATEMENTS, with the second an explicit SHIFT and not a second divide.
      The target has ONE +3 bias (`bgez $v1' / `addiu $v1,$v1,3') and then
      `sra $a0,$v1,2' in one arm and `sra $a0,$v1,4' in the other: combine
      folded `(v1 >> 2) >> 2' into `v1 >> 4' in the second block and left the
      single `>> 2' in the first.  A second `/ 4' or a `/ 16' would emit its
      own bias.
   2. THE DISPATCH IS A DUPLICATED FOUR-WAY GATE, written out in full:
          if (t != 0) { if (kind != 0) DD08 else DD48 }
          else        { if (kind != 0) DD48 else DD08 }
      which is what the target's `beqz $v0' / `bnez $s3' / `j' / `beqz $s3'
      shape is after jump2 cross-joins the two DD48 arms onto one `jal'.
      func_lo16_8001DD08 gets `v - 0x40' and func_lo16_8001DD48 gets
      `v - 0xC0'.  The `(v - 0x40)' conversion is hoisted as a SPLIT pair --
      `sll $s2, $v0, 16' in the inner preheader and `sra $a1, $s2, 16' at the
      call -- and that falls out of the `s16' second parameter by itself.
   3. `v' is read FIRST (`lhu $fp, 0x8($a0)' at word 12) and `z = v - 0x80' is
      a SEPARATE LOCAL in the entry block, not a loop-hoisted expression: the
      target's `addiu $t0, $fp, -0x80' is at word 14, BEFORE the outer loop's
      entry guard at word 27, and loop.c hoists only as far as the preheader
      (words 29-35).  Inlining it in the store is 248 words / 212.
   4. `x0' and `y0' are read ZERO-extended and sign-extended only where the
      source casts: `xe = x0 + w' is `addu $a1, $t5, $s0' off the raw `lhu'
      while `bw = (ww - (xs - (s16)x0)) / 2 + ...' has the `sll 16'/`sra 16'
      pair.  `u16 x0' with `(s16)x0' at the subtraction and `s16 x0' with
      `(u16)' at the sums both measure 197, so the DECLARATION is not
      measurable here; the CASTS are, and they are as written.
   5. `ww' and `hh' are `s32' locals holding `(s16)w' and `(s16)h', spilled as
      WORDS at 0x58/0x5C and reloaded as the `div' divisors and in the
      `(ww - ...)' subtractions.  `w' and `h' are `u16' and feed `xe'/`ye'
      zero-extended -- one pseudo chain each, which is why the target's
      `sll $v0,$s0,16' / `sra $s0,$v0,16' overwrites the register that held
      `w'.
   6. THERE IS NO SEPARATE `ys'.  `y' is both the bh-expression operand and
      the row cursor the bh loop increments, so the target's
      `addu $s5, $s1, $zero' is regmove's copy keeping it alive across the
      subtraction chain.  Writing a separate `ys' adds a second copy and is
      246 words / 221.  `xs' IS separate, because the x cursor is re-set from
      it on every bh iteration (`addu $s1, $s4, $zero').
   7. `func_800A6D30() % ww' with NO explicit mask: the `andi $v0,$v0,0xFFFF'
      is the `u16' return value widening and comes free from the existing
      declaration, and the `div' is SIGNED because the other operand is
      `s32'.  Same as func_lo16_8001CCEC in this unit.
   8. The three counters decrement at the END of their bodies
      (`while (m > 0) { if (...) {...} m--; x++; }'), which is what the
      target's `addiu $v0,$s0,-1' after the gate plus `bgtz' on the new value
      gives.  This is the OPPOSITE of func_lo16_8001744C and
      func_lo16_8001DC34 in this unit, where the decrement is the first
      statement -- the tell is whether the bottom test reads the decremented
      value.  `for (; bh > 0; bh--)' measures the same 197.
   9. `t', `kind' and `bw' are `s16' (halfword spills at 0x48, 0x50, 0x40);
      `n' is `s32' (a word spill at 0x38).  `s32 t' is 245 words / 204.

   NEXT AGENT: make `n' spill and `bh' take $s6.  Everything else in this
   body has been checked against the listing word by word.

   DECLARATIONS
   extern u32 D_8008333C[];
   extern s16 D_8008146E;
   typedef struct UnkDungeonCell
   {
       u16 unk_00;
       u16 unk_02;
       u16 unk_04;
   } UnkDungeonCell;
   extern UnkDungeonCell D_800EA000_s[] __asm__("D_800EA000");
   typedef struct UnkDngLo16Grid
   {
       UnkDungeonCell *unk_00;
       u8 pad_04[0x10];
       s16 unk_14;
       s16 unk_16;
       s16 unk_18;
       s16 unk_1A;
       s16 unk_1C;
       s16 unk_1E;
   } UnkDngLo16Grid;
   u16 func_800A6D30();
   void func_lo16_8001D9CC(s16, s16, s16, s16, s32);
   void func_lo16_8001DD08(s16 *, s16);
   void func_lo16_8001DD48(s16 *, s16);
   END DECLARATIONS */

void func_lo16_8001D5D8(s16 *p, s16 b)
{
    UnkDungeonCell *c;
    u16 v;
    s16 z;
    u16 x0;
    u16 y0;
    s16 xe;
    s16 ye;
    u16 w;
    u16 h;
    s32 ww;
    s32 hh;
    s32 n;
    s16 t;
    s16 kind;
    s16 bw;
    s16 bh;
    s16 xs;
    s16 x;
    s16 y;
    s16 i;
    s16 j;
    s16 m;

    v = p[4];
    z = v - 0x80;
    y0 = p[1];
    x0 = p[0];
    w = p[2];
    h = p[3];
    xe = x0 + w;
    ye = y0 + h;
    for (i = y0; i < ye; i++)
    {
        for (j = x0; j < xe; j++)
        {
            c = (UnkDungeonCell *)(((i << ((UnkDngLo16Grid *)D_8008333C)->unk_14) + j) * 6 +
                                   (s32)D_800EA000_s);
            c->unk_00 = 0x19;
            c->unk_02 = z;
            c->unk_04 = 6;
        }
    }
    t = func_800A6D30() & 1;
    ww = (s16)w;
    hh = (s16)h;
    n = (ww + hh) / 4;
    if (D_8008146E < 5)
    {
        n >>= 2;
    }
    while (n > 0)
    {
        xs = x0 + func_800A6D30() % ww;
        y = y0 + func_800A6D30() % hh;
        bw = (ww - (xs - (s16)x0)) / 2 + (func_800A6D30() & 7) - 4;
        bh = (hh - (y - (s16)y0)) / 2 + (func_800A6D30() & 7) - 4;
        kind = (func_800A6D30() & 7) != 0;
        while (bh > 0)
        {
            if (y < ye)
            {
                x = xs;
                m = bw;
                while (m > 0)
                {
                    if (x < xe)
                    {
                        c = (UnkDungeonCell *)(((y << ((UnkDngLo16Grid *)D_8008333C)->unk_14) +
                                                x) *
                                                   6 +
                                               (s32)D_800EA000_s);
                        if (t != 0)
                        {
                            if (kind != 0)
                            {
                                func_lo16_8001DD08((s16 *)c, v - 0x40);
                            }
                            else
                            {
                                func_lo16_8001DD48((s16 *)c, v - 0xC0);
                            }
                        }
                        else
                        {
                            if (kind != 0)
                            {
                                func_lo16_8001DD48((s16 *)c, v - 0xC0);
                            }
                            else
                            {
                                func_lo16_8001DD08((s16 *)c, v - 0x40);
                            }
                        }
                    }
                    m--;
                    x++;
                }
            }
            bh--;
            y++;
        }
        n--;
    }
    func_lo16_8001D9CC((s16)x0, (s16)y0, xe, ye, (s16)v);
}
