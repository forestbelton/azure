/* func_lo16_8001CBD4 -- ovl_dng_lo16, 70 target words.
   PARKED by agent h172, batch 35, 2026-11-25.

   BEST: 70 candidate words, 13 ALIGNED differences (aligndiff), 22 under
   ovlverify.  Every differing row is a REGISTER NAME.  Nothing is moved,
   nothing is missing, the word count agrees, and the three preheader
   register shuffles are the right COUNT -- they shuffle the wrong VALUES.

   WHAT THE FUNCTION DOES.  Given (x, y, d) it looks at the cell one step in
   direction d; if that cell's unk_04 is nonzero it tries d+2 and d+6 (mod 8,
   masked to even) and returns the first direction whose cell is empty,
   otherwise it returns d.  The step tables are D_8006CCD8 (dx) and
   D_8006CCE8 (dy), both read with `lhu'.

   THE RESIDUE, exactly.  The target's preheader is

       33 addiu $a1, $zero, 0x2        k = 2
       34 addu  $t0, $t1, $zero        <<< a copy of (s16)d
       35 addu  $t1, $a3, $zero        a copy of the D_800EA000 base
       36 addu  $a3, $v1, $zero        a copy of the D_80083350 shift
       37 addu  $v0, $t0, $a1          d + k

   and the candidate's is the same four insns with the FIRST one copying the
   dx TABLE BASE instead of (s16)d.  The tables then live in different
   registers from the target's for the rest of the body, which is the other
   twelve rows.

   THE MECHANISM, as far as it is understood.  Each of those copies is loop.c
   hoisting an invariant into the preheader where cse2 then finds the value
   already live in the entry block (the preheader is the gate's fall-through,
   so it is the SAME cse block, cse.c:8086) and rewrites the hoisted insn as a
   register copy.  Which values get a copy therefore depends on which
   expressions loop.c has to hoist, i.e. on which of them are SOURCE LOCALS
   (already one pseudo, nothing to hoist) and which are written out inside the
   loop (hoisted, new pseudo, copy).

   The target copies (s16)d and NOT the table bases.  So in the target the two
   tables are source-level POINTER LOCALS and `(s16)d' is hoisted.  No
   spelling tried reproduces the second half: `(d + k) & 6' never leaves a
   sign-extension for loop.c to hoist, because combine strips it (the mask
   makes the high bits dead), and cse1 cannot substitute into the loop body
   across the loop label, so the loop's d reference ends up using the entry
   block's register with no copy at all.

   SPELLINGS MEASURED (aligndiff, candidate words / aligned rows):

     both tables bare array refs, D_800EA000 bare         71w / 12
     both tables pointer locals assigned at the top       69w / 22
     both tables pointer locals, assigned interleaved
       with their first use                               69w / 17
     dx bare, dy a pointer local assigned before its use  70w / 13   <- kept
     (dx local / dy bare not tried)

   The word count moves with the number of preheader shuffles: 2 shuffles =
   69, 3 = 70, 4 = 71.  Only the kept spelling has the target's three.

   WHAT TO TRY NEXT, in order:
   1. find a spelling of the loop's direction index that keeps a
      sign-extension of `d' INSIDE the loop for loop.c to hoist, while still
      emitting `andi 0x6' and `addu $v0, $t0, $a1'.  A local holding
      `(d + k) & 6' and used for both table subscripts AND the return value
      was NOT tried and is the obvious candidate -- but note the target
      recomputes the return value from the RAW $a2 (`addu $v0, $a2, $a1'),
      so the local cannot also be the returned expression.
   2. permute the local declaration order; this body was never swept that way
      and the brief's rule is to do it before anything else on a clean
      two-register exchange.

   The `(u16)' casts on both table reads are load-bearing in the sense that
   the listing reads them with `lhu'; the project declares both tables
   `extern s16 ...[]' in twenty units and the cast is how you get `lhu' from
   that.  `s16 nx, ny' locals for the stepped coordinates are what give the
   `sll 16'/`sra 16' pair before each shift, which the target has.

   DECLARATIONS
   extern s16 D_80083350;
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   typedef struct UnkDungeonCell
   {
       u16 unk_00;
       u16 unk_02;
       u16 unk_04;
   } UnkDungeonCell;
   extern UnkDungeonCell D_800EA000_s[] __asm__("D_800EA000");
   END DECLARATIONS
*/
s16 func_lo16_8001CBD4(s16 x, s16 y, s16 d)
{

    s16 *dy;
    s16 nx;
    s16 ny;
    s32 k;

    nx = x + (u16)D_8006CCD8[d];
    dy = D_8006CCE8;
    ny = y + (u16)dy[d];
    if (D_800EA000_s[(ny << D_80083350) + nx].unk_04 != 0)
    {
        for (k = 2; k < 7; k += 4)
        {
            nx = x + (u16)D_8006CCD8[(d + k) & 6];
            ny = y + (u16)dy[(d + k) & 6];
            if (D_800EA000_s[(ny << D_80083350) + nx].unk_04 == 0)
            {
                return (d + k) & 6;
            }
        }
    }
    return d;
}
