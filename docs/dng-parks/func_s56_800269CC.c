/* func_s56_800269CC -- ovl_dng_s56, 119 words.
   PARKED at 119 words / 12 differing (ovlverify) / 10 aligned (agent h179,
   batch 35).  dngfam: one-member family.

   *** THIS BODY USES CONSTANT-BASE ADDRESSING (the 0x1F800000 scratchpad), so
   ovlverify's masked mode cannot see a displacement off it.  Every figure
   below was cross-checked with aligndiff, and the residue rows are all
   REGISTER-NAME-FREE scheduling moves, but a future landing must be checked
   by eye or with --raw before it goes in the container. ***

   WHAT IT DOES.  The tag chain's POLY_FT4 renderer: for each node of the walk
   starting at `p', copy the payload's four 6-byte corner records at
   +0x10/+0x18/+0x20/+0x28 into the scratchpad quad at 0x1F800070, project all
   four through RotTransPers4 straight into the prim's x0..x3, and when the
   returned z - 8 is under 0x1E0 fill the prim's UV quad from the slot's
   record pointer (`c = *(u8 **)(s + 0x8)': u0 = u2 = c[8],
   u1 = u3 = c[8] + c[0xA], v0 = v1 = c[9], v2 = v3 = c[9] + c[0xB]), its
   tpage from c[4], its colour word from the slot's +0xC, mark it
   semi-transparent and hang it on OT row z.  The +0x8D0 cursor is written
   back once at the end.

   THE BASE REGISTER $s0 = q + 7 IS NOT A SOURCE LOCAL: it is loop.c's
   combine_givs representative for the fifteen `q + K' addresses, with the
   `code' byte at +7 chosen as the base, so every byte store appears at a
   displacement 7 less than its POLY_FT4 offset.  Writing the body through a
   `POLY_FT4 *t' reproduces it exactly.

   THE RESIDUE -- 10 aligned rows, ALL in the scratchpad copy block, and all
   one cause.  The target leaves the first two load-delay slots as `nop' and
   issues `lw $s1, 0x8($s5)' (c) in the FOURTH slot, then keeps
   `lw $v1, 0x28($s3)' live across eight instructions before storing it to
   0x1F800088.  Every spelling tried lets the scheduler pull c's load into the
   FIRST free slot instead, which pushes the four `addiu $aN, $s2, 0x7x'
   argument addresses three slots later and makes the 0x88 load/store
   adjacent:

       target (rows 19..39)            candidate
       lw   $v0, 0x10($s3)             lw   $v0, 0x10($s3)
       nop                             lw   $s1, 0x8($s5)      <- hoisted
       sw   $v0, 0x70($s2)             sw   $v0, 0x70($s2)
       lw   $v0, 0x18($s3)             lw   $v0, 0x18($s3)
       nop                             nop
       sw   $v0, 0x78($s2)             sw   $v0, 0x78($s2)
       lw   $v0, 0x20($s3)             lw   $v0, 0x20($s3)
       addiu $a0, $s2, 0x70            nop
       sw   $v0, 0x80($s2)             sw   $v0, 0x80($s2)
       lhu  $v0, 0x14($s3)             lhu  $v0, 0x14($s3)
       lw   $s1, 0x8($s5)              addiu $a0, $s2, 0x70
       lw   $v1, 0x28($s3)             sh   $v0, 0x74($s2)
       ...                             ...

   Everything outside that block is byte-identical, including all four
   chained UV pairs, the semi-transparent `ori', the AddPrim argument and the
   +0x8D0 write-back.

   MEASURED TABLE (candidate words / ovlverify differing):

   * THE SCRATCHPAD STORE ORDER IS w0, w1, w2, vz0, vz1, w3, vz2, vz3 --
     the emitted store order, and the stores are NOT transposable here because
     each pair shares the quad.  Priced: that order 119/12; all four words
     then all four vz (the func_s21_80024A68 shape) 119/13; fully interleaved
     (w0, vz0, w1, vz1, ...) 119/18.
   * `c = *(u8 **)(s + 0x8)' MUST BE BEFORE THE RotTransPers4 CALL (it is
     live across it in $s1).  Its POSITION among the quad stores is NOT
     MEASURABLE in the useful direction: at the top of the loop body it is
     119/12, and in ANY slot between the stores the body comes out 120 WORDS
     and 91-93 differing, because an extra unfillable delay slot appears.
   * THE QUAD MUST BE PASSED AS `&sp->unk_70[i]', NOT AS THE INTEGER CONSTANT
     `(SVECTOR *)0x1F800070'.  The constants are 123 words / 112 differing
     (121/109 with only the two output pointers left as members).  This is the
     INVERSE of func_s20_80025A58's recorded fact, which
     csrc/ovl_dng_s56.c's own func_s56_800264D4 comment carries ("a scratchpad
     ADDRESS PASSED AS AN ARGUMENT is the integer constant off the same
     hoisted lui") -- on THIS body the member address is what lands the word
     count, and the s21 spelling (`&sp->unk_70[0]') is the one to keep.
   * MEM_IN_STRUCT_P ON EITHER SIDE DOES NOT MOVE THE RESIDUE, measured both
     polarities: the word store as `((u32 *)sp)[0x1C]' (array ref, in_struct)
     is 119/12, bit-identical to `*(u32 *)&sp->unk_70[0]'; the load as
     `((u8 **)s)[0x2]' is 120/91 (worse).  The store as a plain constant cast
     `*(u32 *)0x1F800070' is 121/109.  So `true_dependence' is not what is
     letting c's load hoist, and the lever named in the brief for exactly this
     presentation ("a load hoists above a store") does not apply.
   * The declaration order of `c' among the locals is free (119/12 either
     way), and `s32 func_s56_800269CC(u8 *p, UnkVec *v, u8 *s)' with `v'
     unused is required -- the slot arrives in $a2.

   WHAT TO TRY NEXT: `-da' to read the block's INSN_PRIORITY/LUID table (the
   two unfilled slots mean the scheduler had nothing READY, so c's load must
   be held back by a dependence I have not reproduced), or the permuter, which
   is the dispatcher's to spend.

   DECLARATIONS
   typedef struct UnkDng24EF4SP
   {
       u8 pad_00[0x18];
       u8 *unk_18;
       u8 pad_1C[0x4];
       u8 *unk_20;
       u8 pad_24[0x4C];
       SVECTOR unk_70[4];
       s32 unk_90;
       s32 unk_94;
       u8 pad_98[0x28];
       s32 unk_C0;
   } UnkDng24EF4SP;
   END DECLARATIONS
*/
s32 func_s56_800269CC(u8 *p, UnkVec *v, u8 *s)
{
    UnkDng24EF4SP *sp = (UnkDng24EF4SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    POLY_FT4 *t;
    UnkNode *n;
    u8 *c;

    t = (POLY_FT4 *)*(u32 *)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    for (;;)
    {
        c = *(u8 **)(s + 0x8);
        *(u32 *)&sp->unk_70[0] = *(u32 *)(p + 0x10);
        *(u32 *)&sp->unk_70[1] = *(u32 *)(p + 0x18);
        *(u32 *)&sp->unk_70[2] = *(u32 *)(p + 0x20);
        sp->unk_70[0].vz = *(u16 *)(p + 0x14);
        sp->unk_70[1].vz = *(u16 *)(p + 0x1C);
        *(u32 *)&sp->unk_70[3] = *(u32 *)(p + 0x28);
        sp->unk_70[2].vz = *(u16 *)(p + 0x24);
        sp->unk_70[3].vz = *(u16 *)(p + 0x2C);
        sp->unk_C0 = RotTransPers4(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2],
                                   &sp->unk_70[3], (s32 *)&t->x0, (s32 *)&t->x1,
                                   (s32 *)&t->x2, (s32 *)&t->x3, &sp->unk_90,
                                   &sp->unk_94) -
                     0x8;
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            t->u0 = t->u2 = c[0x8];
            t->u1 = t->u3 = c[0x8] + c[0xA];
            t->v0 = t->v1 = c[0x9];
            t->v2 = t->v3 = c[0x9] + c[0xB];
            t->tpage = *(u16 *)(c + 0x4);
            *(u32 *)&t->r0 = *(u32 *)(s + 0xC);
            SetPolyFT4(t);
            setSemiTrans(t, 1);
            AddPrim(sp->unk_20 + sp->unk_C0 * 4, t);
            t++;
        }
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        s = (u8 *)n->unk_0C;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)t;
    return 0;
}
