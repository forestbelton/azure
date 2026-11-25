/* func_s56_80025AD8 -- ovl_dng_s56, 109 words.
   PARKED at 109 words / 5 aligned differences (agent h179, batch 35).
   dngfam: one-member family, so this body lands once.

   WHAT IT DOES. The 7x7 scan that FILLS the two bank-local tables
   func_s56_80025C8C later stamps into the cell grid. It publishes the caller's
   (x, y) cell into D_8002744C / D_8002744D as bytes, then for each of the 49
   cells around (x - 3, y - 3):
     * D_800273CC[j][i] = -func_800BCA68((u16)(ix * 0x40), (u16)(iz * 0x40))
     * func_8009B4B0(D_800E3D7C, (u16)ix, (u16)iz), and if the result is
       neither NULL nor D_s56_8002732C, raise 0x100000 in its +0x14 word
     * D_800274DC[j][i] = func_s56_80025D30(...) when (ix, iz) is inside the
       grid, NULL otherwise.
   No constant-base (0x800xxxxx) addressing anywhere in this body, so
   ovlverify's masked mode is trustworthy here; relcand is clean.

   THE RESIDUE -- 5 rows, all in the ONE basic block that computes the
   D_800274DC store address, and all one cause:

       target                             candidate
       lui   $v0, %hi(D_800274DC)         sll   $v0, $s1, 2      (i * 4)
       addiu $v0, $v0, %lo(D_800274DC)    sll   $v1, $s4, 5      (j * 0x20)
       sll   $v1, $s4, 5                  lui   $a3, %hi(...)
       addu  $v1, $v1, $v0                addiu $a3, $a3, %lo(...)
       sll   $v0, $s1, 2                  addu  $v1, $v1, $a3
       addu  $v0, $v0, $v1                addu  $v0, $v0, $v1
       sw    $a0, 0x0($v0)                sw    $a0, 0x0($v0)

   Same seven instructions, same two `addu' operand orders (index first in
   both), same grouping. The ONLY differences are (a) `sll $v0, $s1, 2' is
   scheduled before the pair instead of after the first `addu', and (b) the
   table base therefore needs a THIRD register and reload picks $a3 where the
   target reuses $v0. (b) is a consequence of (a): in the target the base dies
   at the first `addu', so $v0 is free again for `i * 4'.

   DIAGNOSIS. The base is a rematerialised constant in every spelling tried --
   local-alloc's `update_equiv_regs' turns a single-set pseudo whose source is
   a SYMBOL_REF into a `reg_equiv_constant', so it never gets allocated and
   reload re-emits `lui'/`addiu' immediately before the using insn. That part
   reproduces the target exactly. What does not reproduce is the position of
   the independent `sll $v0, $s1, 2' relative to it, and nothing in the source
   moved it across 14 spellings. This is the sched tie-break between two
   independent `sll's, which is exactly the class the brief says no
   source-order sweep reaches.

   WHAT I DID NOT TRY, and would try next: `-da' on this body to read the
   two slls' INSN_PRIORITY and LUIDs directly (the brief's "use -da to confirm
   a mechanism you already have a source change for" -- here I do not have
   one); and the permuter, which is the dispatcher's to spend.

   MEASURED TABLE (all at 109 candidate words unless stated; ovlverify masked):

     SHAPE FACTS -- all four needed, all four reproduce:
     * `u8 *m = (u8 *)D_8008333C;' AT THE FUNCTION TOP, not inside the inner
       loop.  At the top it is live across the two `jal's, there is no
       callee-saved register left (the body saves s0-s7, fp and ra), so it
       becomes a reg_equiv_constant and reload re-emits `lui'/`addiu' in EACH
       gate block -- which is what the target does, twice, into $a3, with the
       `1' of `1 << n' rematerialised beside it.  Assigned inside the inner
       loop instead it gets $v1 for the whole gate chain and the two gate
       blocks come out with three unfillable `nop's: 22 differing.
       `((s16 *)D_8008333C)[0xA]' / `[0xB]' inline is 108 words / 56 -- the
       array ref folds the displacement into the relocation (`lui'/`lh %lo',
       two words) and cse then reaches the second halfword as `+0x2' off the
       first, so the body is a word SHORT.  `*(s16 *)((u8 *)D_8008333C + 0x14)'
       inline is the same 108 / 56.
     * THE GRID GATE IS ONE `||' CHAIN, NOT AN `&&' WITH THE ARMS SWAPPED:
       `if (ix < 0 || ix >= (1 << *(s16 *)(m + 0x14)) || iz < 0 ||
            iz >= (1 << *(s16 *)(m + 0x16))) { e = NULL; } else { e = f(); }'
       The `||' puts the e = NULL block FIRST, which is where the target has
       it (.Ls56_80025BF4 precedes .Ls56_80025BFC) and makes the last test
       `bnez -> the call'.  Written as the positive `&&' with the arms the
       other way round the two blocks swap: 2 extra rows on top of whatever
       else is live (24 -> 22 when corrected alongside the ix++ fact).
     * `ix++' GOES IN THE INNER `for' INCREMENT, not the body: in the body it
       is 2 rows, the row cursor's giv increment and `ix++' exchanging the
       delay slot with each other (identical mechanism to
       func_s56_80025C8C's `x++', priced there at 5).
     * `row' MUST BE INITIALISED IN THE OUTER `for' COMMA INIT
       (`for (j = 0, row = D_800273CC; j < 7; j++)').  As a statement before
       the loop it is born one insn earlier and the prologue's
       [$s4 = 0] / [$s7 = &D_800273CC] pair comes out in the other order:
       2 rows.  Same fact as func_s56_80025C8C's.

     THE STORE ADDRESS -- 14 spellings, three distinct outcomes:
     * `D_800274DC[j][i] = e;' with the array declared `u8 *[][8]':
       103-110 differing and 110 WORDS.  loop.c hoists the base AND
       strength-reduces the row address into a seventh callee-saved giv
       (`addiu $s5, $s5, 0x20'), which the target does not have.
     * any spelling whose base is the SYMBOL written inline as an integer --
       `*(u8 **)(i * 4 + (j * 0x20 + (s32)D_800274DC))',
       `((u8 **)(j * 0x20 + (s32)D_800274DC))[i]',
       `*(u8 **)(i * 4 + (s32)&D_800274DC[j][0])',
       `*(u8 **)(i * 4 + (s32)D_800274DC[j])',
       `*(u8 **)(i * 4 + (s32)(u8 *)(j * 0x20 + (s32)D_800274DC))',
       the same with a `volatile' cast, and the (s32)j mixed-type variant:
       ALL 6 differing, ALL with the WRONG GROUPING
       `(j * 0x20 + i * 4) + base'.  fold's `split_tree' classifies an address
       constant as the constant part and rebuilds the sum with it LAST, and
       every intervening cast between two 32-bit types is stripped before fold
       sees it, so the grouping cannot be reached with the symbol inline.
     * the base through a POINTER LOCAL is 5 differing and is the best known:
       `u8 *tbl = (u8 *)D_800274DC;' at the function top with
       `*(u8 **)(i * 4 + (j * 0x20 + (s32)tbl)) = e;'.  `tbl' is a variable so
       split_tree leaves the grouping alone.  Measured FREE against that
       (identical 5 rows, bit for bit): `((j * 0x20 + (s32)tbl) + i * 4)',
       `((s32)tbl + j * 0x20 + i * 4)', `((u8 **)(j * 0x20 + (s32)tbl))[i]',
       `((u8 **)((s32)tbl + j * 0x20))[i]',
       `(u8 *)(j * 0x20 + (s32)tbl) + i * 4', declaring `tbl' before `m',
       assigning `tbl' inside the inner loop, and declaring it in a nested
       block.  So the outer sum's textual form is NOT MEASURABLE here -- the
       grouping is all that the tools can see.
       `u8 **tbl' with `tbl[j * 8 + i] = e;' is 12 differing (one `sll 5' /
       `sll 2' pair becomes an `addu' and a single `sll 2').

     OTHER TYPES, measured:
     * the two loop counters must be `u32': the loop tests are `sltiu', and
       `s32' gives `slti' (3 rows) -- and that is the only thing it changes.
     * `x' is `u16' and NOT `s16': it is spilled to 0x10($sp) by reload and
       read back `lbu' for the byte publish and `lhu' for `x - 3'; `s16' makes
       the second one an `lh'.
     * `z' is `s16' (incremented by `addiu', sign-extended into $s3 once per
       outer iteration by loop.c's invariant motion).

   DECLARATIONS
   s16 func_800BCA68(u16, u16);
   u8 *func_8009B4B0(u8 *, u16, u16);
   u8 *func_s56_80025D30(s32, s32, s16);
   extern u8 *D_800E3D7C;
   extern u8 *D_s56_8002732C;
   extern u8 *D_800274DC[][8];
   extern u16 D_800273CC[];
   extern u8 D_8002744C;
   extern u8 D_8002744D;
   extern u32 D_8008333C[];
   END DECLARATIONS
*/
void func_s56_80025AD8(u16 x, u16 y)
{
    u8 *m = (u8 *)D_8008333C;
    u8 *tbl = (u8 *)D_800274DC;
    u8 *e;
    u8 *o;
    u16 *row;
    s16 ix;
    s16 z;
    u32 i;
    u32 j;

    D_8002744C = x;
    D_8002744D = y;
    z = y - 3;
    for (j = 0, row = D_800273CC; j < 7; j++)
    {
        ix = x - 3;
        for (i = 0; i < 7; i++, ix++)
        {
            row[i] = -func_800BCA68((u16)(ix * 0x40), (u16)(z * 0x40));
            o = func_8009B4B0(D_800E3D7C, (u16)ix, (u16)z);
            if (o != NULL && o != D_s56_8002732C)
            {
                *(u32 *)(o + 0x14) |= 0x100000;
            }
            if (ix < 0 || ix >= (1 << *(s16 *)(m + 0x14)) || z < 0 ||
                z >= (1 << *(s16 *)(m + 0x16)))
            {
                e = NULL;
            }
            else
            {
                e = func_s56_80025D30((u16)(ix * 0x40 + 0x20),
                                      (u16)(z * 0x40 + 0x20), -0x400);
            }
            *(u8 **)(i * 4 + (j * 0x20 + (s32)tbl)) = e;
        }
        row += 8;
        z++;
    }
}
