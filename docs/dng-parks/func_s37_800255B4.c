/* PARK: func_s37_800255B4, csrc/ovl_dng_s37.c, agent-h222, 2026-11-26.

   TARGET 907 words. BEST BUILT 906 words, 172 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py). ovlverify on the same body reports
   907/906/907 masked -- that count is noise once a word moves, so every
   figure in this file is aligndiff's.
   1 member, 907 words (tools/decomp/dngfam.py) -- no replication value.

   VERDICT: ALLOC, and it is ONE 3-CYCLE of callee-saved registers.
   170 of the 172 rows are that cycle and nothing else:

     value                     this body   target
     p (parameter 1)             $s5         $s3
     m, and n3 sharing with it   $s3         $s4
     w                           $s4         $s5

   Counted off the target side of the rows: 114 are $s3, 34 are $s4, 12 are
   $s5, and only 8 are anything else. NO row is an opcode, an immediate or a
   displacement; the frame (-0x78), the save LIST (s0-s7 + ra, nine saves)
   and the save SLOTS (0x50..0x70) are all already exact. The remaining 2
   rows and the 1 missing word are one register copy, below.

   WHY THE CYCLE HAPPENS, read out of -da and global.c rather than guessed.
   global.c's find_reg runs two passes and `IOR_COMPL_HARD_REG_SET (used,
   regs_used_so_far)' in pass 0 means it will NOT allocate a callee-saved
   register for the FIRST time in pass 0 -- so an allocno either takes one
   of the s-registers already in use, or falls to pass 1 and takes the
   lowest free one. In this body's .greg the sorted allocation order
   (`;; 39 regs to allocate:' prints allocno_order) reaches the loop
   counter/`n' ($s0), `u' ($s1) and `q' ($s2) first -- all three of which
   match the target -- and then reaches n3 and w BEFORE p. p conflicts with
   every one of them, so pass 0 fails for it and pass 1 hands it $s5.
   For the target's assignment p must be ranked ABOVE n3 and w by
   allocno_compare, which is
   `floor_log2 (n_refs) * n_refs / live_length' (global.c, allocno_compare,
   verified in scratch/gcc-src/cdk-gcc-b18). p is 115 refs over 782 insns in
   the .flow dump; the next floor_log2 step needs 128 refs, so ~13 more p
   references would promote it. I could not find 13 p references that this
   body is missing, and the two obvious ways to DEMOTE the competition both
   cost a save -- see the rejected table.

   CAUTION ON THOSE REF FIGURES: 115/782 is from the `.flow' dump, which is
   the FIRST flow pass. global_alloc runs after regmove, sched1 and
   local_alloc and uses recomputed reg_n_refs/reg_live_length, and the
   numbers it actually sorted on are not dumped -- hand-computing priorities
   from .flow puts p above n3 and w, which is NOT what the sorted order
   shows. So treat the arithmetic above as the MECHANISM (confirmed) and the
   individual numbers as STALE (assumed). That mismatch is why the promote/
   demote attempts were made empirically rather than from the figures.

   IT OWNS A JUMP TABLE AND THE TABLE IS SETTLED AND VERIFIED.
   jtbl_s37_80024070, 9 `.word's = 9 cases + 0 pad (tools/decomp/jtcheck.py),
   dispatch bound `sltiu $v0, $v1, 0x9' with NO minval subtraction, so
   minval is 0 and the case set is 0..8. 9 distinct targets.
   ENTRY 7 IS THE EPILOGUE: .Ls37_800263B0 is both the table's case-7 entry
   and the range-check `beqz $v0' target, so case 7 is the implicit default
   -- there is NO `case 7:' label and NO `default:' statement, and the body
   below writes EIGHT arms (0,1,2,3,4,5,6,8) in ascending order, which is
   also entry-address order. Case 0 FALLS THROUGH into case 1.
   VERIFIED POSITIONALLY by a standalone `tools/cdkgcc -O2 -G0 -S' compile
   and substitution, not by a word count:

     emitted entries  [$L117, $L118, $L129, $L143, $L162, $L180, $L182,
                       $L116, $L184]
     emitted arm-block line numbers (within the function)
                      $L117=83  < $L118=138  < $L129=359  < $L143=526
                      < $L162=798 < $L180=1112 < $L182=1127 < $L184=1151
                      < $L116=1193
     ROM arm addresses
                      800256A8 < 8002576C < 80025A34 < 80025C10 < 80025F64
                      < 800262F4 < 80026318 < 8002634C < 800263B0
     substitution     $L117=800256A8  $L118=8002576C  $L129=80025A34
                      $L143=80025C10  $L162=80025F64  $L180=800262F4
                      $L182=80026318  $L184=8002634C  $L116=800263B0
     resolved         [800256A8, 8002576C, 80025A34, 80025C10, 80025F64,
                       800262F4, 80026318, 800263B0, 8002634C] == the ROM's

   $L116 is also this build's own range-check target (`beq $2,$0,$L116'),
   which is the same discriminator the ROM shows. The arm order is therefore
   MEASURED, not assumed -- and note it is measurable here only because all
   nine targets are distinct; it would not be if the arms cross-joined.

   THE RODATA IS ALSO SETTLED, and it changes a committed file-scope object.
   This unit's `.rodata' subsegment (splat/dungeon.bin.yaml:1345,
   0x190c808..0x190c894 -> 0x80024008..0x80024094) must come out of the C
   file, unlike ovl_dng_s38's, whose equivalent table is `extern' because it
   lives in that unit's `data' subsegment. A standalone compile of the unit
   with this body in place emits, in file order:

     $L79            .align 3   jtbl_s37_80024008  (func_s37_800250C0)
     $L100           .align 3   jtbl_s37_80024028  (func_s37_800251F0)
     $LC0            .align 2   = ROM 0x80024048, 8 bytes
     D_s37_80024050  .align 2   = ROM 0x80024050, 0x20 bytes
     $L188           .align 3   jtbl_s37_80024070  (this body)

   which is the ROM's layout exactly. Two consequences, both reported to the
   dispatcher:
   (1) `const u8 D_s37_80024048[]' MUST BE DELETED. It is not bank data: it
       is cc1psx's own anonymous initialiser blob `$LC0' for
       func_s37_80025360's `s16 a[4] = { 0x340, 0x100, 0x60, 0x54 }', whose
       four halfwords ARE its eight bytes (40 03 00 01 60 00 54 00).
       Keeping both puts sixteen bytes where the ROM has eight.
   (2) `const u8 D_s37_80024050[]' is a REAL bank object and is defined in
       the unit between func_s37_80025360 and this body -- the slot the old
       array occupied. It is eight { s16 a; u16 b; } records, the eight-way
       velocity table this body copies to its stack and indexes by the
       facing at p+0x7E. It is spelled as BYTES because the block copy is
       1-aligned (lwl/lwr), which is what a u8 array's alignment gives.

   THE DONOR is the landed func_s38_80024FA4 (csrc/ovl_dng_s38.c, 627 words).
   tools/decomp/dngcallee.py named it alone at 17 of 23 symbols with a clean
   gap to the 12/23 tier. Same `(u8 *p, UnkVec *v, u8 *c)' signature, same
   `o = *(u8 **)p' / 0x20-byte table copy / `n = o - 0x20' / `m = n->unk_08'
   head in that order, same 9-entry table with case 7 absent, same case-0
   fallthrough, and its case 0/1/2/6/8 are this body's verbatim modulo
   constants. Its FRAME DOES NOT MATCH (-0x70 and seven saves against this
   body's -0x78 and nine), so by the three-kinds taxonomy its budget facts
   cannot transfer; its SHAPE facts all did. Deltas this body has: case 2's
   spark loop is 4 iterations of func_s37_80024FA0 with 0xE02020 and
   `(rand() & 0xFF) | 0x80' (the donor's is 0x10 of func_s38_80024BBC); case
   3 spawns TWO 0x212 nodes rather than one, chaining the first into the
   second's +0x48; case 4 is almost entirely new (an empty delay loop, the
   `* 128 / 20' colour ramp, a third 0x212 spawn whose three velocities are
   `rand()'/func_s37_80024EA4 products); and the ring-spin + blink block is
   written out THREE times, in cases 2, 3 and 4.

   FACTS MEASURED ON THIS BODY. Everything below was measured from the
   906/172 state unless the row says otherwise, so by this project's own
   rule they are improvements rather than facts priced from 0 -- read them
   as directions, not magnitudes.

   | spelling                                              | words | rows |
   |-------------------------------------------------------|-------|------|
   | case 4's empty delay loop as                          |       |      |
   |   `i = 4; do { } while (--i >= 0);'            KEPT   |  906  |  172 |
   |   ... `for (i = 0; i < 4; i++) { }'                   |  906  |  173 |
   |   ... `i = 4; while (--i >= 0) { }'                   |  906  |  173 |
   | case 3's n3 block: the two `|=' on r+0x14 SEPARATED   |       |      |
   |   by the r+0x10 and r+0x6 stores                KEPT  |  906  |  172 |
   |   ... the two `|=' adjacent                          |  903  |  181 |
   | case 4 reusing the SECOND node variable (n3)    KEPT  |  906  |  172 |
   |   ... its own fourth variable n4                     |  904  |  206 |
   |   ... reusing the FIRST node variable (n2)           |  904  |  206 |
   | no `g = D_800DDC40' local, inlined at the one site    |       |      |
   |                                                 KEPT  |  906  |  172 |
   |   ... the donor's `g' local                          |  906  |  172 |

   THE TWO `|=' ROW IS THE REAL ONE and it is the levers file's "gcc merges
   two adjacent `|=' on one lvalue" entry firing: written adjacent,
   `(x|0xC)|0x80' folds to a single `ori 0x8C' and one `sh', which is 2
   words short and 9 extra rows. The ROM has both `ori'/`sh' pairs, so two
   independent stores sit between them in the source. Target emission has
   those stores scheduled ABOVE the first `ori', which is why this is not
   readable off the listing.

   BOTH NODE-VARIABLE ROWS COST A SAVE, and that is the finding that closes
   the obvious route. Giving case 4's spawn its own variable -- the
   `reg_n_sets' / "one local shared between two arms 300 words apart" lever,
   which is exactly what the .greg order says to do to demote n3 below p --
   shortens n3's live range enough that gcc needs only EIGHT callee-saved
   registers. The frame drops to -0x70, the save list loses one entry, and
   the body goes to 904 words / 206 rows. So the demotion that would fix the
   cycle is unavailable: this body's nine saves REQUIRE n3 to span case 3
   and case 4. Same figure, same cause, for reusing n2 instead.

   WHAT I DID NOT TRY, for the next agent: promoting p across the 128-ref
   floor_log2 step by finding p references this body caches and the ROM
   repeats (the .greg order says that is the other half of the fix, and it
   is the half that does not cost a save); and a positional sweep of the
   three ring-spin copies, which are the only blocks whose statement order I
   took from the donor rather than measuring.

   THE 1 MISSING WORD is in case 3's FIRST node block, at ROM 80025D54: the
   target has `addu $v1, $v0, $zero' / `addiu $v0, $zero, 0x20' /
   `sh $v0, 0x10($a3)' where this body has `li $v1, 32' / `sh $v1, 0x10($a3)'
   -- a register copy that survives in the ROM because the `&= 0xFFF3'
   result must stay live while $v0 takes the 0x20 constant. It is 2 of the
   172 rows and is most likely downstream of the cycle rather than its own
   fault, since the cycle moves what is live across that block.

   DECLARATIONS
   typedef struct UnkDngS58Blk32 { u8 b[0x20]; } UnkDngS58Blk32;
   typedef struct UnkDngS58Rec { s16 a; u16 b; } UnkDngS58Rec;
   typedef struct UnkDngS58Name12 { u8 b[0xC]; } UnkDngS58Name12;
   extern s16 D_80026428;
   extern u8 D_800263E0[];
   extern u8 D_800263EC[];
   extern u8 D_800263F8[];
   extern u8 D_80026410[];
   extern u8 D_80045340[];
   extern u8 D_800DDC40[];
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern u8 D_800E3D68;
   extern s32 D_8008346C;
   extern const u8 D_s37_80024050[];
   s32 rand(void);
   s32 func_8004491C(UnkNode *, u32);
   void func_800B835C(u8 *, s32 *, s32, s32);
   u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
   UnkNode *func_8003FC64(s32);
   s32 func_8009D218(u8 *, s32, u8 *);
   s32 func_800A4778(u16, u16, s16, s32);
   s32 func_800A56E0(s32);
   s32 func_800A6D30(void);
   s32 func_800C8A3C(u8 *, s16, s16);
   void func_s37_800250C0(u8 *p, UnkVec *v, u8 *q);
   void func_s37_800251F0(u8 *p, UnkVec *v, u8 *q);
   void func_s37_80025360(u8 *p, UnkVec *v, u8 *q);
   s32 func_s37_80024EA4(void);
   void func_s37_80024FA0(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz);
   END DECLARATIONS
*/
void func_s37_800255B4(u8 *p, UnkVec *v, u8 *c)
{
    u16 out[3];
    s32 arr[2];
    UnkDngS58Rec tbl[8];
    UnkNode *n;
    UnkNode *n2;
    UnkNode *n3;
    u8 *o;
    u8 *m;
    u8 *e;
    u8 *t;
    u8 *u;
    u8 *q;
    u8 *r;
    u8 *w;
    s32 i;
    s32 k;
    s32 z;
    s32 j;
    s32 h;

    o = *(u8 **)p;
    *(UnkDngS58Blk32 *)tbl = *(UnkDngS58Blk32 *)D_s37_80024050;
    n = (UnkNode *)(o - 0x20);
    m = (u8 *)n->unk_08;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)(c + 0xC) = 0x808080;
        *(UnkDngS58Name12 *)(p + 0xA2) = *(UnkDngS58Name12 *)D_800263F8;
        *(u32 *)(c + 0x8) = (u32)(p + 0xA2);
        *(s16 *)(p + 0x7E) = (((u16 *)o)[0x15] >> 9) & 0x7;
        D_80026410[0x10] = 0x80;
        D_80026410[0x11] = 0x80;
        D_80026410[0x12] = 0x80;
        D_80026410[0x14] = 4;
        D_80026410[0x15] = 4;
        D_80026410[0x16] = 4;
        arr[0] = 0x1400340;
        arr[1] = 0x400040;
        func_800B835C(D_80026410, arr, 1, 0);
        D_80026428 = 1;
        ((s16 *)p)[0x5] = ((u16 *)p)[0x5] + 1;
        /* fallthrough */
    case 1:
        if (func_8003DF74((u8 *)((UnkSlot *)n->unk_0C)->unk_08, (UnkSlot *)n->unk_0C, (s16 *)out, 0) ==
            NULL)
        {
            if ((((UnkSlot *)n->unk_0C)->unk_14 & 0x8000) == 0)
            {
                break;
            }
        }
        *(s16 *)((u8 *)v + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)v + 0x6) = *(u16 *)(m + 0x6);
        z = *(u16 *)(m + 0xA);
        *(s16 *)((u8 *)v + 0xA) = z;
        if ((((UnkSlot *)n->unk_0C)->unk_14 & 0x8000) == 0)
        {
            *(s16 *)((u8 *)v + 0x2) = *(u16 *)((u8 *)v + 0x2) + out[0];
            *(s16 *)((u8 *)v + 0x6) = *(u16 *)((u8 *)v + 0x6) + out[1];
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)v + 0xA) + out[2];
        }
        else
        {
            *(s16 *)((u8 *)v + 0xA) = z - 0x40;
        }
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        if ((p[0x7A] & 0x4) == 0)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
            *(s16 *)(c + 0x10) = 0x20;
            c[0xE] = 0x80;
            c[0xD] = 0x80;
            c[0xC] = 0x80;
            *(s16 *)(c + 0x1E) = 0x7D0;
            *(s16 *)(c + 0x1C) = 0x7D0;
            *(u16 *)(c + 0x14) |= 0xC;
            p[0x7A] |= 0x4;
        }
        t = *(u8 **)(o + 0x60);
        if (t != NULL)
        {
            u = *(u8 **)(t - 0x18);
            *(s16 *)(p + 0x78) = *(u16 *)(u + 0xA) - (D_800DDC40[t[0x13]] + 0x40);
            e = *(u8 **)(o - 0x14);
            p[0xBA] = e[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x7E)];
            p[0xBB] = e[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x7E)];
            p[0x7B] = __builtin_abs(*(s8 *)(o + 0x72) == e[0x24] ? *(s8 *)(o + 0x73) - e[0x25]
                                                                 : *(s8 *)(o + 0x72) - e[0x24]) *
                          2 -
                      1;
        }
        else
        {
            k = *(u16 *)(o + 0x88) - 0x50;
            p[0x7B] = 0x20;
            *(s16 *)(p + 0x78) = k;
        }
        v->unk_0C = tbl[*(s16 *)(p + 0x7E)].a << 16;
        v->unk_10 = tbl[*(s16 *)(p + 0x7E)].b << 16;
        v->unk_14 = ((*(s16 *)(p + 0x78) << 16) - v->unk_08) / *(s8 *)(p + 0x7B);
        *(s16 *)(p + 0x8A) = *(s8 *)(p + 0x7B) * *(s8 *)(p + 0x7B);
        *(s16 *)(p + 0x82) = 0;
        *(s16 *)(p + 0x90) = 0;
        *(s16 *)(p + 0x92) = 0;
        *(s16 *)(p + 0x94) = 5;
        *(s16 *)(p + 0x96) = 0xC8;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0x88) = *(s8 *)(p + 0x7B);
        break;
    case 2:
        for (i = 0; i < 4; i++)
        {
            func_s37_80024FA0((UnkNode *)(p - 0x20), *(s16 *)(p + 0x7E), 0xE02020,
                              (rand() & 0xFF) | 0x80, 0, 0, 0);
        }
        if ((s16)func_800A4778(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                               *(s16 *)((u8 *)v + 0xA), *(s32 *)(o + 0x60)) != 0)
        {
            *(s16 *)(p + 0xA) = 8;
            *(s16 *)(p + 0x82) = 0;
            c[0xE] = 0;
            c[0xD] = 0;
            c[0xC] = 0;
            break;
        }
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x190;
        if (*(u16 *)(c + 0x1A) > 0x1000)
        {
            *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) - 0x1000;
        }
        if (*(s16 *)(p + 0x90) == 0)
        {
            *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + *(u16 *)(p + 0x96);
            *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + *(u16 *)(p + 0x96);
        }
        else
        {
            *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) - *(u16 *)(p + 0x96);
            *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - *(u16 *)(p + 0x96);
        }
        *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + 1;
        if (*(s16 *)(p + 0x92) >= *(s16 *)(p + 0x94))
        {
            *(s16 *)(p + 0x92) = 0;
            *(s16 *)(p + 0x90) = *(u16 *)(p + 0x90) ^ 1;
        }
        p[0x7B] = p[0x7B] - 1;
        if (*(s8 *)(p + 0x7B) <= 0)
        {
            if (*(u8 **)(o + 0x60) != NULL)
            {
                *(s16 *)(p + 0xA) = 3;
                *(s16 *)(p + 0x82) = 0;
                u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                *(s16 *)((u8 *)v + 0x2) = *(u16 *)(u + 0x2);
                *(s16 *)((u8 *)v + 0x6) = *(u16 *)(u + 0x6);
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)(p + 0x78);
                func_800A56E0(0x300);
                break;
            }
            *(s16 *)(p + 0xA) = 8;
            *(s16 *)(p + 0x82) = 0;
            c[0xE] = 0;
            c[0xD] = 0;
            c[0xC] = 0;
        }
        else
        {
            v->unk_00 += v->unk_0C;
            v->unk_04 += v->unk_10;
            v->unk_08 += v->unk_14;
        }
        break;
    case 3:
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x190;
        if (*(u16 *)(c + 0x1A) > 0x1000)
        {
            *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) - 0x1000;
        }
        if (*(s16 *)(p + 0x90) == 0)
        {
            *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + *(u16 *)(p + 0x96);
            *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + *(u16 *)(p + 0x96);
        }
        else
        {
            *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) - *(u16 *)(p + 0x96);
            *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - *(u16 *)(p + 0x96);
        }
        *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + 1;
        if (*(s16 *)(p + 0x92) >= *(s16 *)(p + 0x94))
        {
            *(s16 *)(p + 0x92) = 0;
            *(s16 *)(p + 0x90) = *(u16 *)(p + 0x90) ^ 1;
        }
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) == 4)
        {
            n2 = func_8003FC64(0x212);
            if (n2 != NULL)
            {
                q = (u8 *)n2 + 0x20;
                *(s16 *)(q + 0x2) = 0x78;
                for (i = 0; i < 0x3C; i++)
                {
                    *(u8 *)(q + 0x50 + i) = 0;
                }
                *(s16 *)(q + 0x8E) = 0xC8;
                n2->unk_10 = (u32)func_s37_800251F0;
                func_8004491C(n2, (u32)D_80045340);
                r = (u8 *)n2->unk_0C;
                *(s16 *)(r + 0x6) = 0;
                *(u16 *)(r + 0x14) &= 0xFFF3;
                *(s16 *)(r + 0x10) = 0x20;
                *(u16 *)(r + 0x14) |= 0x80;
                w = (u8 *)n2->unk_08;
                *(s16 *)(q + 0xA) = 0;
                *(s16 *)(q + 0xC) = 0;
                u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                *(s32 *)(w + 0x0) = *(s32 *)(u + 0x0);
                *(s32 *)(w + 0x4) = *(s32 *)(u + 0x4);
                *(s32 *)(w + 0x8) = *(s32 *)(u + 0x8);
                r = (u8 *)n2->unk_0C;
                *(s16 *)(r + 0x1E) = 0x1000;
                *(s16 *)(r + 0x1C) = 0x1000;
                r[0xE] = 0x80;
                r[0xD] = 0x80;
                r[0xC] = 0x80;
                for (i = 0; i < 1; i++)
                {
                    ((UnkDngS58Name12 *)(q + 0x20))[i] = ((UnkDngS58Name12 *)D_800263EC)[i];
                }
                *(u32 *)(r + 0x8) = (u32)(q + 0x20);
            }
            n3 = func_8003FC64(0x212);
            if (n3 != NULL)
            {
                q = (u8 *)n3 + 0x20;
                *(u32 *)(q + 0x38) = (u32)o;
                *(u32 *)(q + 0x3C) = *(u32 *)(o + 0x60);
                *(u32 *)(q + 0x40) = (u32)p;
                *(u32 *)(q + 0x48) = (u32)((u8 *)n2 + 0x20);
                n3->unk_10 = (u32)func_s37_80025360;
                func_8004491C(n3, (u32)D_80045340);
                r = (u8 *)n3->unk_0C;
                *(u16 *)(r + 0x14) |= 0xC;
                *(s16 *)(r + 0x10) = 0x20;
                *(s16 *)(r + 0x6) = 6;
                *(u16 *)(r + 0x14) |= 0x80;
                w = (u8 *)n3->unk_08;
                *(s16 *)(q + 0xA) = 0;
                *(s16 *)(q + 0xC) = 0;
                u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                *(s32 *)(w + 0x0) = *(s32 *)(u + 0x0);
                *(s32 *)(w + 0x4) = *(s32 *)(u + 0x4);
                *(s32 *)(w + 0x8) =
                    *(s32 *)(u + 0x8) - (D_800DDC40[(*(u8 **)(o + 0x60))[0x13]] >> 1);
                r = (u8 *)n3->unk_0C;
                *(s16 *)(r + 0x1E) = 0x1400;
                *(s16 *)(r + 0x1C) = 0x1400;
                r[0xE] = 0x80;
                r[0xD] = 0x80;
                r[0xC] = 0x80;
                *(UnkDngS58Name12 *)((u8 *)n3 + 0x40) = *(UnkDngS58Name12 *)D_800263E0;
                *(u32 *)(r + 0x8) = (u32)((u8 *)n3 + 0x40);
            }
            if (*(s16 *)(p + 0x82) == 4)
            {
                *(s16 *)(p + 0xA) = 4;
                *(s16 *)(p + 0x82) = 0;
                *(s16 *)(p + 0x9C) = 0;
            }
        }
        break;
    case 4:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        i = 4;
        do
        {
        } while (--i >= 0);
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x190;
        if (*(u16 *)(c + 0x1A) > 0x1000)
        {
            *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) - 0x1000;
        }
        if (*(s16 *)(p + 0x90) == 0)
        {
            *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + *(u16 *)(p + 0x96);
            *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + *(u16 *)(p + 0x96);
        }
        else
        {
            *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) - *(u16 *)(p + 0x96);
            *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - *(u16 *)(p + 0x96);
        }
        *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + 1;
        if (*(s16 *)(p + 0x92) >= *(s16 *)(p + 0x94))
        {
            *(s16 *)(p + 0x92) = 0;
            *(s16 *)(p + 0x90) = *(u16 *)(p + 0x90) ^ 1;
        }
        if (*(u16 *)(p + 0x82) >= 0xB && *(u16 *)(p + 0x82) < 0x1C)
        {
            c[0xC] = (0x1E - *(s16 *)(p + 0x82)) * 128 / 20;
            c[0xD] = (0x1E - *(s16 *)(p + 0x82)) * 128 / 20;
            c[0xE] = (0x1E - *(s16 *)(p + 0x82)) * 128 / 20;
        }
        if (*(s16 *)(p + 0x82) >= 0x28)
        {
            *(u16 *)(c + 0x14) |= 0x80;
        }
        if ((*(u16 *)(p + 0x82) & 0x3) == 0 && *(s16 *)(p + 0x82) < 0x50)
        {
            n3 = func_8003FC64(0x212);
            if (n3 != NULL)
            {
                q = (u8 *)n3 + 0x20;
                *(s16 *)(q + 0x2) = 0x14;
                *(s16 *)(q + 0x4) = 0;
                *(s16 *)(q + 0xA) = 0xA;
                n3->unk_10 = (u32)func_s37_800250C0;
                func_8004491C(n3, (u32)D_80045340);
                r = (u8 *)n3->unk_0C;
                *(s16 *)(r + 0x10) = 0x20;
                *(u16 *)(r + 0x14) |= 0xC;
                w = (u8 *)n3->unk_08;
                u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                *(s32 *)(w + 0x0) =
                    *(s32 *)(u + 0x0) + ((((rand() & 0x1F) + 0x10) * func_s37_80024EA4()) << 16);
                *(s32 *)(w + 0x4) =
                    *(s32 *)(u + 0x4) + ((((rand() & 0x1F) + 0x10) * func_s37_80024EA4()) << 16);
                *(s32 *)(w + 0x8) = *(s32 *)(u + 0x8) -
                                    (D_800DDC40[(*(u8 **)(o + 0x60))[0x13]] << 15) -
                                    ((rand() & 0x1F) << 16);
                r = (u8 *)n3->unk_0C;
                *(s16 *)(r + 0x1E) = 0x800;
                *(s16 *)(r + 0x1C) = 0x800;
                r[0xE] = 0;
                r[0xD] = 0;
                r[0xC] = 0;
                *(UnkDngS58Name12 *)((u8 *)n3 + 0x40) = *(UnkDngS58Name12 *)D_800263EC;
                *(u32 *)(r + 0x8) = (u32)((u8 *)n3 + 0x40);
            }
        }
        if (*(s16 *)(p + 0x82) == 0x50)
        {
            if (func_8009D218(*(u8 **)(o + 0x60), 1, o) == 0)
            {
                j = (func_800A6D30() & 3) + 2;
                h = (p[0x9] >> 2) + j;
                func_800C8A3C(*(u8 **)(o + 0x60), D_800E3D68 == 0xFF ? 0xFF : 0x10, h);
            }
        }
        if (*(s16 *)(p + 0x82) >= 0x78)
        {
            *(s16 *)(p + 0x82) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if (*(s16 *)(p + 0x9C) != 0)
        {
            *(s16 *)(p + 0x82) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 6:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) >= 0xB)
        {
            *(s16 *)(p + 0xA) = 8;
            *(s16 *)(p + 0x82) = 0x1E;
        }
        break;
    case 8:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) >= 0x1F)
        {
            ((s16 *)p)[0x41] = ((u16 *)p)[0x41] - 1;
            if (D_80026428 == 0)
            {
                D_8008346C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            else
            {
                D_80026428 = 0;
            }
        }
        break;
    }
}
