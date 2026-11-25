/* func_s56_8002616C -- ovl_dng_s56, 218 words.
   PARKED at 216 words (TWO SHORT) / 21 aligned differences (agent h179,
   batch 35).  dngfam: one-member family.

   *** USES CONSTANT-BASE ADDRESSING: the 0x1F800000 scratchpad throughout,
   and `*(u32 *)0x1F800020 = (u32)ot' is an ABSOLUTE store that gas expands
   through $at.  ovlverify's masked mode cannot see a displacement off a
   constant base, so every figure below is from aligndiff and a landing must
   be eyeballed or checked with --raw. ***

   WHAT IT DOES.  The quad emitter func_s56_80026064 calls twice.  It takes
   (yoff, w, b, ot): parks `ot' in the scratchpad at 0x1F800020, grabs the
   +0x8D0 prim cursor, builds an IDENTITY rotation (RotMatrix of a zero
   SVECTOR at 0x1F800028 into the scratchpad matrix at 0x1F800050, with the
   translation zeroed t[2], t[1], t[0] DESCENDING -- func_s56_800264D4's
   fact), seeds all four scratchpad corner vz's to 0x800, then either
     * b != NULL: fills the four corner words from D_80083160.unk_1E4 indexed
       by b's four halfwords (`*(u32 *)(*(u16 *)(b + K) * 8 + verts)'), and
       the prim's UV words from b + 0x8/0xC/0x12/0x14 with r0 = 0x808080; or
     * b == NULL: a unit quad (0, 0x40, 0x400000, 0x400040) and the UV/tpage
       from D_80027374, with the grey
       `t[0x5] = t[0x6] = t[0x4] = *(u8 *)&D_800273A8 * 0x80 / 0xF0'
       (magic 0x88888889 with `sra 7' -- read the SHIFT: 2^39/(2^32+M) = 240,
       so the divisor is 0xF0, not anything the magic names),
   then SetPolyFT4, RotTransPers4 into the prim's own x0..x3, biases every x
   by `w[0] - 0xA0' and every y by `w[1] + yoff - 0x78', AddPrim, PopMatrix
   and writes the cursor back.

   THE RESIDUE -- 21 rows and 2 words, in four places, all scheduling:
   1. THE PROLOGUE PARAMETER-COPY ORDER.  The target copies $a1 (row 4) then
      $a2 (row 6) and sinks the $a0 copy into PushMatrix's delay slot (row
      19); the candidate copies $a0 first and sinks $a2.  4 rows.
   2. ARM 1: the 0x808080 constant and `addu $a0, $s0, $zero' (SetPolyFT4's
      argument) are two slots earlier in the target.  3 rows.
   3. ARM 2: the target issues its four scratchpad word stores BEFORE the
      first `lhu 0x4($a0)' of D_80027374 and leaves a `nop' after that load;
      the candidate hoists the load above the stores and fills the slot.
      One of the two missing words is that nop.  ~8 rows.
   4. The tail has one more nop the candidate fills.
   Every arm's body, both UV blocks, the magic divide, all eight coordinate
   fixups and the epilogue are otherwise byte-identical.

   MEASURED TABLE (aligndiff rows; ovlverify's masked count is useless here
   because the word count differs):

   FACTS THAT HOLD:
   * `verts = *(u32 *)((u8 *)g + 0x1E4)' AS A PLAIN CAST, not `g->unk_1E4'.
     As a COMPONENT_REF the load is MEM_IN_STRUCT_P and
     `fixed_scalar_and_varying_struct_p' excuses it against the fixed-address
     `*(u32 *)0x1F800020' store, so it hoists above it; the plain cast keeps
     the dependence and the load stays where the target has it.  15 rows.
   * THE MATRIX TRANSLATION ZEROS COME FIRST, THE ROTATION VECTOR SECOND AND
     DESCENDING: `sp->unk_6C = 0; sp->unk_68 = 0; sp->unk_64 = 0;' then
     `sp->unk_28.vz = 0; .vy = 0; .vx = 0;'.  With the vector first, reorg
     fills RotMatrix's delay slot with the 0x64 store where the target has the
     0x28 store.  Part of the same 15.
   * THE PRIM IS A `u8 *t' WITH PLAIN CASTS AND BYTE ARRAY REFS, NOT A
     `POLY_FT4 *'.  Through POLY_FT4 members the arm-2 byte stores are
     MEM_IN_STRUCT_P and the body comes out 215 words / 65 rows; `u8 *' is
     217 / 55 at the same point in the sweep.
   * `*(u32 *)(t + 0x4) = 0x808080' IS THE LAST STORE OF ARM 1, after the
     +0x24 halfword.  Before it: 103 vs 144 masked at equal word counts, so
     roughly 10 rows.
   * EACH COORDINATE FIXUP'S SUBTRACTION NEEDS ITS OWN BLOCK-SCOPE `s32 k'.
     Written as one expression (`*(u16 *)w + (*(u16 *)(t + 0x8) - 0xA0)')
     convert.c narrows the inner subtraction through UNSIGNED short -- its
     first operand is a `u16' load -- so -0xA0 becomes 0xFF60, which is no
     `addiu' immediate, and cc1psx materialises `li $a0, 0xFF60' once for all
     four x's and once for all four y's: 8 rows AND 2 extra words.  A
     FUNCTION-SCOPE `s32 k' reused eight times fixes the `addiu' but gives the
     eight live ranges one quantity spanning the whole tail, so `k' loses $v0
     to `w' and all sixteen rows come out with $v0/$v1 exchanged.  Declaring
     `k' inside a brace block per fixup is what lands those rows.
     `*(s16 *)(t + 0x8)' as the inner read (signed, so the subtraction stays
     signed) measures the same 21 rows, so THAT spelling is an equally good
     alternative and the two are not distinguishable here.
   * `t = t + 0x28;' MUST BE ITS OWN STATEMENT after PopMatrix.  As
     `(u32)(t + 0x28)' inside the cursor write-back the add lands after the
     `lw $v0, 0($s6)' instead of in PopMatrix's delay slot: 5 rows.
   * the four corner vz seeds are written DESCENDING (unk_70[3] first).

   NOT RESOLVED: the three scheduling clusters above.  In particular I could
   not find a spelling that stops the scheduler hoisting arm 2's
   `lhu 0x4($a0)' above the scratchpad stores -- `((u16 *)d)[0x2]' (array ref,
   MEM_IN_STRUCT_P) measures the identical 21 rows, so `true_dependence' is
   not the handle.  `-da' on arm 2, or the permuter.

   DECLARATIONS
   typedef struct UnkDng2616CSP
   {
       u8 pad_00[0x20];
       u8 *unk_20;
       u8 pad_24[0x4];
       SVECTOR unk_28;
       u8 pad_30[0x34];
       s32 unk_64;
       s32 unk_68;
       s32 unk_6C;
       SVECTOR unk_70[4];
       s32 unk_90;
       s32 unk_94;
   } UnkDng2616CSP;
   extern u8 D_80027374[];
   extern u32 D_800273A8;
   END DECLARATIONS
*/
void func_s56_8002616C(s32 yoff, u8 *w, u8 *b, u8 *ot)
{
    UnkDng2616CSP *sp = (UnkDng2616CSP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    u8 *t;
    u8 *d;
    u32 verts;

    t = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u32 *)0x1F800020 = (u32)ot;
    verts = *(u32 *)((u8 *)g + 0x1E4);
    PushMatrix();
    sp->unk_6C = 0;
    sp->unk_68 = 0;
    sp->unk_64 = 0;
    sp->unk_28.vz = 0;
    sp->unk_28.vy = 0;
    sp->unk_28.vx = 0;
    RotMatrix(&sp->unk_28, (MATRIX *)0x1F800050);
    SetTransMatrix((MATRIX *)0x1F800050);
    SetRotMatrix((MATRIX *)0x1F800050);
    sp->unk_70[3].vz = 0x800;
    sp->unk_70[2].vz = 0x800;
    sp->unk_70[1].vz = 0x800;
    sp->unk_70[0].vz = 0x800;
    if (b != NULL)
    {
        *(u32 *)&sp->unk_70[0] = *(u32 *)(*(u16 *)(b + 0x0) * 8 + verts);
        *(u32 *)&sp->unk_70[1] = *(u32 *)(*(u16 *)(b + 0x2) * 8 + verts);
        *(u32 *)&sp->unk_70[2] = *(u32 *)(*(u16 *)(b + 0x4) * 8 + verts);
        *(u32 *)&sp->unk_70[3] = *(u32 *)(*(u16 *)(b + 0x6) * 8 + verts);
        *(u32 *)(t + 0xC) = *(u32 *)(b + 0x8);
        *(u32 *)(t + 0x14) = *(u32 *)(b + 0xC);
        *(s16 *)(t + 0x1C) = *(u16 *)(b + 0x12);
        *(s16 *)(t + 0x24) = *(u16 *)(b + 0x14);
        *(u32 *)(t + 0x4) = 0x808080;
    }
    else
    {
        *(u32 *)&sp->unk_70[0] = 0;
        *(u32 *)&sp->unk_70[1] = 0x40;
        *(u32 *)&sp->unk_70[2] = 0x400000;
        *(u32 *)&sp->unk_70[3] = 0x400040;
        d = D_80027374;
        *(s16 *)(t + 0x16) = *(u16 *)(d + 0x4);
        *(s16 *)(t + 0xE) = *(u16 *)(d + 0x6);
        t[0xC] = t[0x1C] = d[0x8];
        t[0x14] = t[0x24] = d[0x8] + d[0xA];
        t[0xD] = t[0x15] = d[0x9];
        t[0x1D] = t[0x25] = d[0x9] + d[0xB];
        t[0x5] = t[0x6] = t[0x4] = *(u8 *)&D_800273A8 * 0x80 / 0xF0;
    }
    SetPolyFT4(t);
    RotTransPers4(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2], &sp->unk_70[3],
                  (s32 *)(t + 0x8), (s32 *)(t + 0x10), (s32 *)(t + 0x18),
                  (s32 *)(t + 0x20), &sp->unk_90, &sp->unk_94);
    {
        s32 k;

        k = *(u16 *)(t + 0x8) - 0xA0;
        *(s16 *)(t + 0x8) = *(u16 *)w + k;
    }
    {
        s32 k;

        k = *(u16 *)(t + 0x18) - 0xA0;
        *(s16 *)(t + 0x18) = *(u16 *)w + k;
    }
    {
        s32 k;

        k = *(u16 *)(t + 0x10) - 0xA0;
        *(s16 *)(t + 0x10) = *(u16 *)w + k;
    }
    {
        s32 k;

        k = *(u16 *)(t + 0x20) - 0xA0;
        *(s16 *)(t + 0x20) = *(u16 *)w + k;
    }
    {
        s32 k;

        k = *(u16 *)(t + 0xA) - 0x78;
        *(s16 *)(t + 0xA) = k + (*(u16 *)(w + 0x2) + yoff);
    }
    {
        s32 k;

        k = *(u16 *)(t + 0x12) - 0x78;
        *(s16 *)(t + 0x12) = k + (*(u16 *)(w + 0x2) + yoff);
    }
    {
        s32 k;

        k = *(u16 *)(t + 0x1A) - 0x78;
        *(s16 *)(t + 0x1A) = k + (*(u16 *)(w + 0x2) + yoff);
    }
    {
        s32 k;

        k = *(u16 *)(t + 0x22) - 0x78;
        *(s16 *)(t + 0x22) = k + (*(u16 *)(w + 0x2) + yoff);
    }
    AddPrim(sp->unk_20, t);
    PopMatrix();
    t = t + 0x28;
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)t;
}
