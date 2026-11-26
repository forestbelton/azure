/*
   THIS PARK WAS UNMEASURABLE UNTIL 2026-11-25 and its older notes were written
   against a splice that no longer compiled. `parkresidue' printed SKIP with
   `redefinition of struct Unk...' because the unit has since REFINED two
   typedefs this park also declares, and `drop_known' matched declarations by
   exact text, so the park's copies survived. Fixed in `parksplice.py' by
   matching a struct by TAG and an `extern' by SYMBOL NAME. Current measured
   baseline, which is what any new work must beat:

       264 words built / 264 target, 4 ALIGNED ROWS -- word-exact.

   THE FOUR ROWS, AND THREE READINGS OF THEM THAT ARE ALL WRONG. I read the
   rows as three independent faults and priced each one; every cell is worse
   than leaving the park alone, so the park's existing spelling is right on all
   three counts and the rows mean something else.

       baseline (as parked)                          264 w /  4 rows
       `a = (s8)...unk_02' for the `(u8)' cast       266   / 10
       `s32 kr' for `u16 kr'                         263   /  9
       `extern u8 D_800735C4[]' for `u8 *'           264   /  9
       all three together                            265   / 19

   What each row looked like, and why the obvious reading failed:

     * word 45: target `lb $v0, 0x2($s2)', built `lbu v0,2(s2)'. Reads as a
       signedness fault on `unk_02'. It is not -- making the read signed costs
       2 words and 6 rows. The sign must come from somewhere other than this
       field's cast.
     * word 155: target `addu $s0, $a1, $zero' (a plain move), built
       `andi s0,a1,0xffff'. Reads as `u16 kr' narrowing a value the target
       keeps wide. Widening it loses a word and gains 5 rows, so the narrowing
       is wanted and the MOVE is what differs.
     * words 156/157: target `lui $a0, %hi(D_800735C4)', built `lui a0,0x0'.
       Reads as the declared-shape rule -- a pointer where the target wants the
       symbol's address. It is not: the array spelling keeps 264 words and
       still gains 5 rows, so `extern u8 *D_800735C4' is correct here and the
       zero high half has another cause.

   So all three rows are symptoms of ONE thing this park has not found, not
   three local faults, and the next agent should look for a single cause that
   puts a signed byte load, a register move and a symbol's high half in the
   target where this body has an unsigned load, a mask and a zero. Do not
   re-price the three cells above.
 func_lo16_8001EF0C -- ovl_dng_lo16, 264 target words.
   PARKED by agent h181, batch 35, 2026-11-25.  264 WORDS -- EXACT -- and
   THREE ALIGNED DIFFERENCES, all in one four-instruction window.  Words
   0..154 and 158..263 are identical, registers included.

   WHAT IT IS: the monster repopulate, run once per floor.  Loop 1 walks the
   0x20 D_800E3648 slots, and for every slot tagged 0x13 it instantiates the
   species through func_800A1618 / func_800A0B94's returned constructor, wires
   it with func_80042640, seeds its +0x18 countdown from D_800835E4 and ticks
   it forward with func_800A1D4C until the countdown passes the D_800835E8
   threshold, then clears the slot.  Loop 2 then spends a budget -- half
   D_8008146C plus a random 0..0x3F -- re-tagging up to `cap` slots with fresh
   species out of func_800A6DA4, skipping any whose 0x3000 flags are both set
   or that costs more budget than is left, placing each with func_800A4E2C and
   publishing height, draw flags and the func_800DF258 template word.
   No twin: dngtwin and xsibs were both run over the whole unit and neither
   finds a sibling at any ratio.

   THE RESIDUE, exactly:

       target                            candidate
       154 addu  $a1, $v0, $zero         154 move  a1,v0          (same)
       155 addu  $s0, $a1, $zero         155 andi  s0,a1,0xffff
       156 lui   $a0, %hi(D_800735C4)    156 andi  v1,s0,0xff
       157 andi  $v1, $s0, 0xFF          157 lui   a0,0x0

   The species id is held in TWO pseudos: a short-lived one ($a1, used only by
   the `sb $a1, 0x0($s2)' thirty insns later) and a long-lived callee-saved one
   ($s0, used by both `& 0xFF' masks, one of them past four calls).  The target
   copies between them with a PLAIN `addu'; the candidate needs an `andi
   0xFFFF', and the `lui'/`andi' pair behind it then swaps.  ONE instruction's
   FORM, with two rows of scheduling knock-on.

   WHY, AND WHY NO SOURCE SPELLING REACHES IT.  Two SImode locals with
   `k = kr' do not survive: cse's copy propagation rewrites `k's first use --
   which is in the SAME basic block, since the `jal func_800A6DA4' ends the
   previous one -- and the copy is then deleted, giving ONE pseudo spanning to
   the tail use and 263 words / 8 rows.  The only way to keep two pseudos from
   the source is to make the copy a CONVERSION, and every conversion on MIPS
   costs an instruction that is not `addu':
       `u16 kr' -> `s32 k'      zero_extend   `andi 0xFFFF'   264 / 3  (kept)
       `u16 kr' -> `u16 k'      same                          264 / 3
       `s32 kr' -> `s32 k' with an explicit `& 0xFFFF'        264 / 7
       `s32 kr' -> `s32 k' with a `(u16)' cast                264 / 3
       `s16 kr'                 sign_extend, two insns        265 / 22
       `s8  kr'                                               264 / 13
       `u8  kr'                                               261 / 27
       `u32 kr'                 propagated away               263 / 10
       no second local at all                                 263 / 10
       `k = call; kr = k' (roles swapped), both s32           263 / 8
       the same with the masks moved onto `kr'                263 / 8
   So the residue is in the dispatcher's "insn the compiler EMITS" class -- a
   conversion/copy's form driven by a register lifetime, not a statement
   position -- and it is NOT a permuter job.  What would settle it is why cse
   propagates the copy here and not in the original; a `-da' `.cse' dump on
   both sides is the only thing left.

   MEASURED AND LOAD-BEARING.  The body went 274 -> 270 -> 264 words and
   149 -> 62 -> 41 -> 28 -> 15 -> 9 -> 5 -> 3 rows on these, in this order:

   1. **THE TWO POSITION BYTES ARE TWO SEPARATE `u8' SCALARS, NOT `u8 buf[2]'.**
      This is worth 6 rows and it is my sub-agent's mechanism from
      func_lo16_8001E18C, confirmed here independently: gcc 2.7.2's
      `true_dependence' varying-struct / fixed-scalar clause is guarded by
      `GET_MODE (mem) != QImode', so the HImode `D_800E39C8[i].unk_12 = h'
      store loses its dependence on the fixed-address stack loads ONLY when
      those loads are not MEM_IN_STRUCT_P -- and an ARRAY REF sets
      MEM_IN_STRUCT_P.  As `u8 buf[2]' the `lbu $a0, 0x10($sp)' /
      `lbu $a1, 0x11($sp)' / `addiu $a2, 0x20' argument trio for
      func_8009A21C CANNOT be hoisted above the two `sh's and the body is
      264 / 15; as two scalars it hoists exactly as the target has it.  The
      two scalars land at 0x10 and 0x11 as the target wants; do NOT use two
      `u8 x[2]' arrays, because assign_stack_local gives a BLKmode local
      BIGGEST_ALIGNMENT and they would be eight bytes apart.
   2. **AND ONLY THEN DOES THE STORE ORDER MATTER.**  With the scalars in
      place the three publish stores must be `unk_12`, `unk_10`, `unk_14` --
      6 rows, because `unk_14 = 0' then sinks into the `jal's delay slot and
      the two `sh $v0's precede it.  All six permutations measured:
      unk_12/unk_10/unk_14 3 (with the rest of the facts), unk_10/unk_12/unk_14
      11, unk_12/unk_14/unk_10 13, unk_10/unk_14/unk_12 13,
      unk_14/unk_10/unk_12 11, unk_14/unk_12/unk_10 9.  Before fact 1 the SAME
      sweep was flat-to-worse at 15..17, which is the dispatcher's rule exactly:
      a flat order sweep means the lever is the local SET, not the sequence.
   3. **`UnkDungeonSlot.unk_02' IS `s8' IN THIS UNIT, NOT `u8'** -- see the
      LOUD note below -- and that is worth 6 words and 21 rows.  Loop 1 reads
      it with `lb $v0, 0x2($s2)' off the SAME loop-reduced cursor as unk_00 and
      unk_01; spelled `*(s8 *)&D_800E3648_s[i].unk_02' the cast builds a SECOND
      giv biased +2, costs a ninth callee-saved register, grows the frame from
      -0x38 to -0x40 and is 270 words / 62 rows.  Loop 2 reads the same field
      UNSIGNED (`lbu'), so it carries an explicit `(u8)' cast.
   4. **`i = 0' IS A STATEMENT BEFORE THE POINTER SETUP, not the `for' init.**
      The target's `sw $s4, 0x28($sp)' / `addu $s4, $zero, $zero' are words 1
      and 2, ahead of the three address materialisations.  As `for (i = 0; ...)'
      it is 4 rows.
   5. **`k1' FOR LOOP 1 IS A SEPARATE `s16' LOCAL** from loop 2's `k'.  Sharing
      one `s32' gives `lbu $s0' + `move $s1,$s0' where the target loads
      straight into $s1: 265 / 28.  As a separate `s32' it is still 265 / 28
      and as `u8' 265 / 28; `s16' is 263 / 22 and is the only width that drops
      it.  This is the "second local where one was doing the work of two"
      result, with the width as the second axis.
   6. **`cost' IS A LOCAL** -- `cost = (fl >> 10) + 4' once, then the compare
      and the subtract.  Written twice inline the target's
      `addiu $v1, $v0, 0x4' / `slt' / `subu $s3, $s3, $v1' chain comes out as
      an `addiu $v0, $s3, -0x4' instead.
   7. **`s16 fl' AND A PLAIN `fl >> 10'.**  `fl' holds `*(u16 *)(...) & 0x3000'
      and the shift must be ARITHMETIC on the sign-extended narrow local, which
      is what gives the target's `sll $a0, $v1, 16' / `srl $v0, $a0, 26' pair.
      Measured: plain `fl >> 10' 3; `(u16)fl >> 10' 5; `(u16)(fl >> 10)' 3;
      `((u32)((u16)fl << 16) >> 26)' 5; `(u16)fl / 0x400' 5; `u16 fl' with the
      same shift 5; `fl' unmasked with the mask at both uses 265 / 33.
   8. **THE INNER THRESHOLD LOOP READS THE TABLE FIRST**:
      `while (D_800835E8[m[0x11]] <= *(u32 *)(m + 0x18))'.  Written with the
      countdown on the left the `lw 0x18($s0)' fills the load-delay slot ahead
      of the table load and it is 8 rows.  The guard above it is a SEPARATE
      expression off `d8' (= `d4 + 1') while the loop test is off the
      `D_800835E8' SYMBOL -- the target uses two different base registers for
      the two, so both spellings are real and must not be unified.
   9. `w = d4[...]; *(u32 *)(m + 0x18) = w; if (w >= d8[m[0x11]])' -- the guard
      compares the STORED VALUE, not a reload; the target's `sltu' operands are
      the just-computed `$v1' and the table word.  And the comparison is
      UNSIGNED, which `u32 w' gives.
  10. Settled and uncontested: `func_800A1618(k1, 1)' is called ONCE into a
      local and the pointer is passed on to func_800A0B94, whose result is
      called immediately as `func_800A0B94(...)(0, e06, e07, -0x400)' with no
      intermediate local; `(D_800E3D6C & 8) != 0 || i >= cap' is one `||' and
      gives the target's `slt' in the first branch's delay slot;
      `(u8)(k - 0xF) < 2' is the `addiu -0xF' / `andi 0xFF' / `sltiu 2' trio;
      the 0x840/0x940 pair reads `unk_14' ONCE into `t' and assigns `t | 0x840'
      then `t | 0x940', because the target's second `ori' is off the ORIGINAL
      value; and the `|= 0x800' in the other arm DOES reload (`lhu'), because
      the `unk_08' store between invalidates cse.

   LOUD -- A TYPEDEF IN THIS UNIT CHANGED: `UnkDungeonSlot.unk_02' IS NOW
   `s8' WHERE IT WAS `u8'.  The unit carries its own copy of that typedef, so
   nothing outside ovl_dng_lo16 is affected, and csrc/ovl_dungeon.c's copy of
   the name is untouched.  Inside the unit the only other reference is
   func_lo16_8001D0F4's `D_800E3648_s[i].unk_02 = 0;`, a store, which is
   unaffected: all 54 defined bodies were re-verified at 0 differing after the
   change.  A landing tool that cannot widen a typedef in place must make this
   edit by hand.

   NEXT AGENT: do not re-sweep the store order or the local set; both are
   priced above and the body is word-exact.  The whole job is the form of one
   copy at word 155.

   ===================================================================
   agent-h231, 2026-11-26.  THE `-da' DUMPS WERE READ, AND ONE OF THE FOUR
   ROWS WAS NEVER THIS BODY'S FAULT.

   1. ROW 45 (`lbu' vs `lb') IS PARK ROT, NOT A RESIDUE.  `parksplice' now
      matches a struct by TAG, so it DROPS this park's own
      `UnkDungeonSlot' -- the refined one with `s8 unk_02' that fact 3 above
      is about -- and the splice compiles against the UNIT's copy at
      `csrc/ovl_dng_lo16.c:34-40', which still says `u8 unk_02'.  Widen that
      one field in a copy of the spliced unit and the row disappears:
          park as spliced (unit's `u8 unk_02')      264 w / 4 rows
          same, unit's typedef widened to `s8'      264 w / 3 rows
      SO THE PARK'S REAL BASELINE IS 264/3, AND THE 264/4 FIGURE AT THE TOP
      OF THIS HEADER IS AN ARTEFACT OF THE SPLICER.  This park cannot be made
      self-measuring: the unit also declares `D_800E3648_s' itself, so the
      park's narrower typedef can never be the one in scope.  It is a
      per-park repair of the kind docs/dng-endgame.md describes, and a
      landing MUST edit `csrc/ovl_dng_lo16.c:38' by hand.
      RE-VERIFIED, so the edit is gated: with `s8 unk_02' in place,
      `ovlverify' over ALL 57 currently-defined bodies of the unit gives 56
      at `0 differing' and only this park non-zero.  (h181's note says 54
      bodies; it is 57 now and still clean.)

   2. WHAT THE `andi 0xffff' ACTUALLY IS.  In `.jump', `kr' is `reg/v:HI 91'
      and `k' is `reg/v:SI 89', and the copy is
          (insn 478 (set (reg/v:SI 89) (zero_extend:SI (reg/v:HI 91))))
      i.e. word 155 is the HImode->SImode ZERO_EXTEND, not a copy whose form
      went wrong.  THE TWO PSEUDOS SURVIVE PRECISELY BECAUSE THE MODES
      DIFFER: `insert_regs' (cse.c:1029-1046) calls `make_regs_eqv' only for
      a class member with `GET_MODE (classp->exp) == GET_MODE (x)', so a
      HI/SI pair is never put in one equivalence class and neither pseudo can
      be canonicalised onto the other.

   3. AND THAT IS WHY EVERY SAME-MODE SPELLING LOSES THE WORD.  With
      `s32 kr' the copy is a reg-reg copy, the class forms with `k'
      canonical, EVERY use of `kr' is rewritten to `k' -- including the
      `sb $a1, 0x0($s2)', which the target keeps on `kr' -- and by the `.cse'
      dump insn 477 is gone and insn 474 sets `k' straight from `$v0':
          `s32 kr'                    263 w /  8 rows, `sb s0,0(s2)'
          `u16 kr' + `u16 k'          263 w / 17 rows, BOTH moves gone and
                                      `andi a0,v0,0xff' reads `$v0' direct
      (that second cell CORRECTS the "`u16 kr' -> `u16 k' same 264 / 3" row
      in the table above, which no longer reproduces.)
      The target reads `$a1' at the `sb' and `$s0' at both masks, so the
      target's two pseudos were never in one class either -- the same
      structural tension as func_lo16_80018E80's park, and the mode
      difference is the only thing in the source language that produces it.

   4. THE cse-BLOCK AXIS IS CLOSED, AND THIS IS THE NEW RESULT, because a
      cse dump was the next step this header named.  It is NOT a cse
      block-extension decision:
          baseline + `-fno-cse-follow-jumps'     264 w /  3 rows  IDENTICAL
          baseline + `-fno-cse-skip-blocks'      264   / 16
          baseline + `-fno-rerun-cse-after-loop' 267   / 56
          `s32 kr' + `-fno-cse-follow-jumps'     263   /  8  IDENTICAL
      (On func_lo16_80018E80 the same probe gave 401/0, so this is a real
      discrimination between the two parks and not a flat sweep.)
      cse1's blocks here end at LABELS, not at branches: the three
      `continue' guards between the copy and the `sb' do not split the block,
      which is why turning jump-following off changes nothing.

   SO THE REMAINING AXIS IS: a source construct whose RTL is a plain SImode
   reg-reg move that cse cannot put in one class -- and `insert_regs' offers
   only the mode escape, which on MIPS costs the extension that IS word 155.
   Unless a pass after cse can be made to drop the `andi', this park's three
   rows are the price of the only two-pseudo shape the language gives.
   Measurement harness note: `onefunc.py' drops the park's spliced
   DECLARATIONS (they sit after the unit's preamble), so add the 11 lines to
   the standalone preamble by hand or it will not compile; with them it
   reproduces 264/3 exactly at 0.09 s a candidate.

   DECLARATIONS
   extern u32 D_800835E4[];
   extern u32 D_800835E8[];
   extern s32 D_800DF258[];
   extern u8 *D_800735C4;
   extern u16 D_8008146Cu __asm__("D_8008146C");
   extern s16 D_80081468h[] __asm__("D_80081468");
   extern s32 D_800E3D6C;
   extern u32 D_800E296C;
   typedef struct UnkDungeonSlot
   {
       u8 unk_00;
       u8 unk_01;
       s8 unk_02;
       u8 unk_03;
   } UnkDungeonSlot;
   extern UnkDungeonSlot D_800E3648_s[] __asm__("D_800E3648");
   typedef struct UnkDungeonSpotB
   {
       u8 pad_00[0x6];
       u8 unk_06;
       u8 unk_07;
       s32 unk_08;
       void *unk_0C;
       s16 unk_10;
       s16 unk_12;
       u16 unk_14;
       u8 pad_16[0x2];
   } UnkDungeonSpotB;
   extern UnkDungeonSpotB D_800E39C8[];
   typedef u8 *(*FnDngLo16Spawn)(s32, s32, s32, s16);
   u8 *func_800A1618(s16, s16);
   FnDngLo16Spawn func_800A0B94(s32, u8 *, s32);
   s32 func_800A152C(s16, s16);
   void func_800A1D4C(u8 *, s16);
   void func_8003DB94(u8 *, s32, s32);
   s32 func_800A6DA4(u32, u32);
   s16 func_800A4E2C(u8 *, u8 *);
   u16 func_800A6D30();
   void func_8009A21C(s32, s32, s32);
   s32 func_800BCB04(s32, s32, s16);
   END DECLARATIONS */

void func_lo16_8001EF0C(void)
{
    u8 px;
    u8 py;
    u32 *d4;
    u32 *d8;
    u8 *p;
    u8 *m;
    s32 i;
    s32 cap;
    s32 budget;
    s32 k;
    s32 cost;
    u16 kr;
    s16 k1;
    s16 a;
    s16 bb;
    s16 h;
    s16 fl;
    u16 t;
    u32 w;
    s32 v;

    i = 0;
    d4 = D_800835E4;
    d8 = d4 + 1;
    for (; i < 0x20; i++)
    {
        if (D_800E3648_s[i].unk_01 == 0x13)
        {
            k1 = D_800E3648_s[i].unk_00;
            p = func_800A1618(k1, 1);
            if (p != NULL)
            {
                m = func_800A0B94(k1, p, 1)(0, D_800E39C8[i].unk_06, D_800E39C8[i].unk_07,
                                           -0x400);
                if (m != NULL)
                {
                    func_800A152C(k1, 1);
                    func_80042640((Unk42710 *)m, k1);
                    m[0x43] = 0xFF;
                    m[0x12] = 0;
                    w = d4[D_800E3648_s[i].unk_02];
                    *(u32 *)(m + 0x18) = w;
                    if (w >= d8[m[0x11]])
                    {
                        do
                        {
                            func_800A1D4C(m, 0);
                        } while (D_800835E8[m[0x11]] <= *(u32 *)(m + 0x18));
                    }
                    m[0x43] = func_800A6D30();
                }
            }
            *(s32 *)&D_800E3648_s[i] = 0;
        }
    }
    if (D_80081468h[3] == 1)
    {
        cap = (func_800A6D30() & 3) + 4;
    }
    else
    {
        cap = (s16)(u16)D_80081468h[3] / 2 + (func_800A6D30() & 3);
    }
    if (cap >= 0x21)
    {
        cap = 0x20;
    }
    budget = (s16)D_8008146Cu / 2 + (func_800A6D30() & 0x3F);
    for (i = 0; i < 0x20; i++)
    {
        if (D_800E3648_s[i].unk_01 == 0x15)
        {
            px = D_800E39C8[i].unk_06;
            py = D_800E39C8[i].unk_07;
            k = D_800E3648_s[i].unk_00;
            a = (u8)D_800E3648_s[i].unk_02;
            bb = D_800E3648_s[i].unk_03;
        }
        else
        {
            if ((D_800E3D6C & 8) != 0 || i >= cap)
            {
                continue;
            }
            D_800E3648_s[i].unk_01 = 0;
            D_800E3648_s[i].unk_00 = 0;
            budget += func_800A6D30() & 0xF;
            kr = func_800A6DA4(1, 0x13);
            k = kr;
            fl = *(u16 *)(D_800735C4 + (k & 0xFF) * 12) & 0x3000;
            if (fl == 0x3000)
            {
                continue;
            }
            cost = (fl >> 10) + 4;
            if (budget < cost)
            {
                continue;
            }
            budget -= cost;
            if ((D_800E296C & 0x20000000) == 0 && (u8)(k - 0xF) < 2)
            {
                continue;
            }
            D_800E3648_s[i].unk_01 = 0x15;
            D_800E3648_s[i].unk_00 = kr;
            while ((s16)func_800A4E2C(&px, &py) < 0)
            {
            }
            D_800E39C8[i].unk_06 = px;
            D_800E39C8[i].unk_07 = py;
            a = (func_800A6D30() & 7) | 4;
            bb = 0x80;
        }
        D_800E3648_s[i].unk_02 = a;
        D_800E3648_s[i].unk_03 = bb;
        h = func_800BCB04((px << 6) | 0x20, (py << 6) | 0x20, -0x400);
        D_800E39C8[i].unk_12 = h;
        D_800E39C8[i].unk_10 = h;
        D_800E39C8[i].unk_14 = 0;
        func_8009A21C(px, py, 0x20);
        v = D_800DF258[k & 0xFF];
        if (v < 0)
        {
            func_8003DB94((u8 *)&D_800E39C8[i], v, 0);
            t = D_800E39C8[i].unk_14;
            D_800E39C8[i].unk_14 = t | 0x840;
            if ((v & 0x20000000) != 0)
            {
                D_800E39C8[i].unk_14 = t | 0x940;
            }
        }
        else
        {
            if (v != 0)
            {
                D_800E39C8[i].unk_08 = v | 0x80000000;
            }
            D_800E39C8[i].unk_14 |= 0x800;
        }
        D_800E39C8[i].unk_0C = NULL;
    }
}
