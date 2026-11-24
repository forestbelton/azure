/* PARK: func_b12_80173FDC, csrc/ovl_dng_b12.c, agent-h21, 2026-11-23.

   TARGET 364 words. BEST BUILT 359 words, 97 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py); ovlverify reports 344 differing unaligned,
   which is the transposition inflation the brief warns about.
   1 member, 364 words, 364 words total (tools/decomp/dngfam.py:
   func_b12_80173FDC alone -- no replication value beyond its own body).

   NO TWIN ANYWHERE. xsibs.py at --min-words 18 --ratio 0.40 --top 400 does
   not list this stub at all, so its best landed match is under 0.40. The
   callee-set grep is empty too: func_80065770 appears in NO C file in the
   tree, func_800DBA90 has exactly two callers and both are in
   csrc/ovl_dungeon_800D50FC.c (func_800DAB50 and func_800DB660) and neither
   is this shape, and the only landed function that pairs func_800667D0 with
   func_80067F20 and the 0x8D0 allocator is csrc/ovl_town_800BF3F0.c's
   func_800BFFF4 -- 122 words, the ONE-LINE_G2-per-node version of the inner
   block here, and its header is where the addPrim/LINE_G2/DR_MODE spellings
   below come from. This body is plain decompilation.

   WHAT IT DOES. For every node of the tag chain off p - 8, draw a wireframe
   8x8 lat/long sphere (or dome) in LINE_G2 segments. Per node:

     * build func_800DBA90's 0x20-byte matrix record on the stack -- source
       and destination both the 64-entry SVECTOR grid, +0xA zero,
       +0xC the node's +0x10 halfword less 0x400, +0x8 the
       func_80065F90 (ratan2) of the node's +0x12 against 0x40, +0x10..+0x17
       the node's first eight bytes copied as ONE 8-byte alignment-1 object
       (the target moves it with lwl/lwr + swl/swr, which is what forces the
       UnkDngB12Name8 type below), +0x18 the point count 0x40 and +0x1A
       mode 1;
     * fill the grid: for i in 0..7 and j in 7..0,
       vy = ((8 - q[0xE]) << 4) * i / 8, vx = rsin(j << 9) * rsin(vy*16 +
       0x800) >> 18, vz = rcos(j << 9) * rsin(vy*16 + 0x800) >> 18 -- i.e.
       a vertical ramp times a circle of 8 longitudes;
     * func_800DBA90(rec) to stack the matrix, then func_80065770, which is
       libgte RotTransPersN(SVECTOR *v0, DVECTOR *v1, u_short *sz,
       u_short *p, u_short *flag, long n) -- witnessed by the scratchpad
       layout, which is the whole key to this function: v1 is the 64-entry
       DVECTOR (packed xy) array at 0x1F800000..0x1F8000FF, sz the 64-entry
       halfword depth array at 0x1F800100..0x1F80017F, and p and flag BOTH
       0x1F800180, the first word past sz. n is re-READ out of rec + 0x18
       (`lh 0x30($sp)'), not passed from a register;
     * loop A, the 7 MERIDIAN segments: for i = 56 stepping -8 while > 0 and
       j = 7..0, depth = (sz[i+j] + sz[i+j-8]) >> 3 and the two endpoints
       dv[i+j] and dv[i+j-8];
     * loop B, the 6 PARALLEL rings: for i = 48 stepping -8 while > 0 and
       j = 7..0, jj = (j+1) & 7, depth = (sz[i+j] + sz[i+jj]) >> 3 and the
       endpoints dv[i+j] and dv[i+jj]. Rows 0 and 7 get no ring, which is
       what a pole looks like.

     Both loops' bodies are the same block, written out twice (the target
     has two copies; jump2 did not cross-jump them because the two endpoint
     operands differ): allocate a 0x14-byte LINE_G2 and then a 0xC-byte
     DR_MODE off D_80083160.unk_00 + 0x8D0, SetDrawMode(dm, 0, 0,
     (u16)GetTPage(0, 1, 0, 0), 0), both gouraud words from the node's +0x8,
     SetLineG2, SetSemiTrans(ln, 1), the two endpoints, then addPrim of ln
     and of dm at (u32 *)(g->unk_00 + 0xB0) + z. Gate is (u32)z < 0x1E0.

   SPELLINGS THAT ARE ALREADY MEASURED AND LOAD-BEARING. Each number is the
   aligned difference count with everything else as in the body below.

   (1) THE SCRATCHPAD ADDRESSES ARE POINTER LOCALS HOLDING PLAIN INTEGER
   CONSTANTS, NOT THE D_1F8000xx SPLAT SYMBOLS. spimdisasm renders the
   loop-A and loop-B row-base inits as %hi/%lo(D_1F8000E0) and
   %hi/%lo(D_1F8000C0), but they are NOT relocations: a CONST_INT load on
   this compiler is `li' = lui/ori (which is exactly how the target spells
   0x1F800100 and 0x1F800180, `lui $t0, 0x1F80' + `ori $t0, 0x100'), while
   these two are lui + addiu, i.e. `0x1F800000 + 0xE0' kept unfolded because
   cse's rtx_cost prefers (plus (reg) (const_int 224)) over a two-insn
   constant. `dv + 56' and `dv + 48' off `dv = (s32 *)0x1F800000' reproduce
   both words, and `lui $a1, 0x1F80' is shared between the func_80065770
   argument and the loop-A base.

   (2) THE ROW POINTERS MUST BE EXPLICIT AND THE [jj] SIDE MUST GO THROUGH
   ITS OWN COPY. Measured:
     row pointers zr/xr walked by -8, with zw/xw copies in loop B
       (the body below)                                    359 words,  97
     1D indexing everywhere, sz[i+j] / dv[i+j]             325 words, 290
     1D indexing in loop B only                            335 words, 360
     row pointers, NO zw/xw copies                         356 words, 130
     row pointers, zw/xw copies in loop A as well          359 words,  97
     loop B [j] off zr/xr and [jj] off zw/xw               359 words,  97
     loop B [jj] off sz[i+jj] / dv[i+jj]                   366 words, 116
     zw copy only (xw dropped)                             357 words,  99
     xw copy only (zw dropped)                             358 words, 103
   and the STATEMENT ORDER of the two inits is worth 3: `zr = ...; xr = ...'
   before both loops is 361 words / 106, `xr' first is 359 / 97.

   (3) THE GRID'S +0x2 READ-BACK IS A u16 LOAD SIGN-EXTENDED BY SHIFTS, NOT
   A SIGNED LOAD. The target is `lhu'/`sll 16'/`sra 12' (three words) per
   use, twice, because the intervening `sh' to +0x0 wipes cse's memory
   table. `(s16)*(u16 *)(v + 2) * 0x10' and `*(s16 *)(v + 2) * 0x10' both
   FOLD to `lh'/`sll 4' (two words) -- that folding alone was 8 words and
   ~30 differences. `((*(u16 *)(v + 0x2)) << 16) >> 12' is the form that
   keeps all three. Probably macro residue: a sign-extend-a-halfword macro
   applied to a u16 field, scaled by 16.

   (4) THE vy PRODUCT MUST KEEP THE 16 ON THE (8 - e) SIDE.
   `i * ((8 - *(s16 *)(q + 0xE)) * 0x10)' reassociates to `(i * 16) * (8-e)'
   and gcc then HOISTS `i * 16' out of the inner loop, which also splits the
   inner loop's head into a preheader and moves the outer back edge to a
   second label -- the target's two back edges share .Lb12_80174070.
   Writing the 16 as a SHIFT, `((8 - *(s16 *)(q + 0xE)) << 4) * i', stops
   the reassociation. `i * ((8 - ...) << 4)' is identical (97 either way);
   fold normalises the operand order.

   (5) `q = p' IS A SEPARATE SPILLED LOCAL. The target keeps p in its own
   home slot 0x280($sp) and q in 0x238($sp), reloads p AFTER storing q, and
   reads the node's +0x8, +0xE and +0x0..+0x7 off q while rec + 0xC and
   rec + 0x8 read p. Same as csrc/ovl_dungeon_800D50FC.c's func_800DB660,
   whose header records the same `q' idiom for the same reason.

   (6) `Unk80083160 *g = &D_80083160' AS A POINTER LOCAL, so reload
   rematerialises `lui'/`addiu' + `lw 0x0' at each of the five uses in the
   draw block. Writing `D_80083160.unk_00' directly would fold the %lo into
   the `lw' and give a two-word load instead of three.

   (7) `u16 func_80066460(...)', NOT `s32'. The target has
   `andi $a3, $v0, 0xFFFF' between GetTPage and SetDrawMode; an s32 return
   gives a bare `move' and is 2 differences.

   WHAT IS STILL WRONG, exactly, at 359 / 97. Of the 97 rows: 33 are PURE
   REGISTER RENAMES, 28 are substitutions or transpositions, 20 are
   target-only words and 16 are body-only words. Grouped:

     * 8 rows are the 0xFFFFFF addPrim mask: the target hoists it into $s7
       and the outer counter `i' into $s6, this body has them the other way
       round. The mask is loop-invariant across the whole function in both.

     * ~5 words and most of the remaining renames are ONE allocation
       decision in loop B. The target's frame is 0x280; this body's is
       0x278, two spill slots fewer. The target spills BOTH row bivs (zr at
       0x250, xr at 0x24C) AND both row copies (zw at 0x244, xw at 0x248),
       and puts the dv giv `&xw[j]' in $fp; this body gives $fp to the zr
       biv and spills the dv giv instead. From the -da dumps the two
       candidates are pseudos 92 and 93, 25 refs each over 176 and 178
       insns, so floor_log2(25)*25/live_length is 0.568 against 0.562 --
       a margin of one part in a hundred, exactly the shape
       docs/matching-levers.md records for allocno_compare. 93 needs one
       more reference or one fewer insn of live range. None of the ten
       spellings in (2) moved it.

     * 1 target-only word is a DEAD LOAD this compiler will not keep:
       `lw $a1, 0x8($a1)' in the walk tail, i.e. `<something> = n->unk_08'
       after `p = (u8 *)n + 0x20'. Spelled as an unused second parameter
       (`UnkVec *b', the body below) flow deletes it; the same is true of an
       unused local. Nothing in the 364 words reads $a1 again before
       `addiu $a1, $zero, 0x40' at the next iteration's head, so whatever
       the real source assigns it to must be read somewhere I have not
       identified. This is the one fact in the function I could not explain.

     * 2 rows are a transposition of `addiu $v0, 0x40' / `sh $v0, 0x30($sp)'
       (the rec + 0x18 store) around the 8-byte record copy. All three
       source orders -- before the copy, after it, and up beside the
       rec + 0x8 store -- give 97; sched1 places it.

     * 1 row is `addu $a0, $s4, $zero' against `addiu $a0, $sp, 0x38': the
       target still has the grid base in $s4 from the grid loop and this
       body rematerialises it. Same word count.

   NOT TRIED, and the two things I would try next: the permuter (the brief
   forbids it to agents), and a -da-guided reference bump on pseudo 93 of
   the kind docs/matching-levers.md prices for allocno_compare -- the
   coherent unit of work around the dv row pointer is the whole
   two-endpoint publish, so a wrapper there rather than around the depth
   gate is where the macro boundary would be.

   NO DEVICES ARE USED BELOW. Nothing in the body is an asm device or a
   phantom-zero; it is all plain C.

   DECLARATIONS
   typedef struct UnkDngB12Name8
   {
       u8 b[0x8];
   } UnkDngB12Name8;
   s32 func_800644B8(s32);
   s32 func_80064584(s32);
   s16 func_80065F90(s16, s32);
   void func_80065770(SVECTOR *v, s32 *dv, s16 *sz, s16 *pp, s16 *fl, s32 n);
   u16 func_80066460(s32, s32, s32, s32);
   void func_80066640(void *, s32);
   void func_800667D0(void *);
   void func_80067F20(u32 *, s32, s32, s32, s32);
   void func_800DBA90(u8 *);
   END DECLARATIONS
*/
s32 func_b12_80173FDC(u8 *p, UnkVec *b)
{
    u8 rec[0x20];
    SVECTOR grid[64];
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    u8 *ln;
    u8 *dm;
    u8 *q;
    u8 *v;
    s32 *dv;
    s16 *sz;
    s16 *fl;
    s16 *zr;
    s32 *xr;
    s16 *zw;
    s32 *xw;
    s32 i;
    s32 j;
    s32 jj;
    s32 z;

    dv = (s32 *)0x1F800000;
    sz = (s16 *)0x1F800100;
    fl = (s16 *)0x1F800180;
    while (1)
    {
        *(u8 **)rec = (u8 *)grid;
        *(u8 **)(rec + 0x4) = (u8 *)grid;
        *(s16 *)(rec + 0xA) = 0;
        *(s16 *)(rec + 0xC) = *(u16 *)(p + 0x10) - 0x400;
        q = p;
        *(s16 *)(rec + 0x8) = func_80065F90(*(s16 *)(p + 0x12), 0x40);
        for (i = 0; i < 8; i++)
        {
            for (j = 7; j >= 0; j--)
            {
                v = (u8 *)grid + ((j << 3) + (i << 6));
                *(s16 *)(v + 0x2) = ((8 - *(s16 *)(q + 0xE)) << 4) * i / 8;
                *(s16 *)v = (func_800644B8(j << 9) *
                             func_800644B8((((*(u16 *)(v + 0x2)) << 16) >> 12) + 0x800)) >>
                            18;
                *(s16 *)(v + 0x4) = (func_80064584(j << 9) *
                                     func_800644B8((((*(u16 *)(v + 0x2)) << 16) >> 12) + 0x800)) >>
                                    18;
            }
        }
        *(s16 *)(rec + 0x18) = 0x40;
        *(UnkDngB12Name8 *)(rec + 0x10) = *(UnkDngB12Name8 *)q;
        *(s16 *)(rec + 0x1A) = 1;
        func_800DBA90(rec);
        func_80065770(grid, dv, sz, fl, fl, *(s16 *)(rec + 0x18));
        xr = dv + 56;
        zr = sz + 56;
        for (i = 56; i > 0; i -= 8)
        {
            for (j = 7; j >= 0; j--)
            {
                z = (zr[j] + zr[j - 8]) >> 3;
                if ((u32)z < 0x1E0)
                {
                    ln = *(u8 **)(g->unk_00 + 0x8D0);
                    *(u32 *)(g->unk_00 + 0x8D0) = (u32)ln + 0x14;
                    dm = *(u8 **)(g->unk_00 + 0x8D0);
                    *(u32 *)(g->unk_00 + 0x8D0) = (u32)dm + 0xC;
                    func_80067F20((u32 *)dm, 0, 0, func_80066460(0, 1, 0, 0), 0);
                    *(s32 *)(ln + 0x4) = *(s32 *)(q + 0x8);
                    *(s32 *)(ln + 0xC) = *(s32 *)(q + 0x8);
                    func_800667D0(ln);
                    func_80066640(ln, 1);
                    *(s32 *)(ln + 0x8) = xr[j];
                    *(s32 *)(ln + 0x10) = xr[j - 8];
                    addPrim((u32 *)(g->unk_00 + 0xB0) + z, ln);
                    addPrim((u32 *)(g->unk_00 + 0xB0) + z, dm);
                }
            }
            zr -= 8;
            xr -= 8;
        }
        xr = dv + 48;
        zr = sz + 48;
        for (i = 48; i > 0; i -= 8)
        {
            xw = xr;
            zw = zr;
            for (j = 7; j >= 0; j--)
            {
                jj = (j + 1) & 7;
                z = (zw[j] + zw[jj]) >> 3;
                if ((u32)z < 0x1E0)
                {
                    ln = *(u8 **)(g->unk_00 + 0x8D0);
                    *(u32 *)(g->unk_00 + 0x8D0) = (u32)ln + 0x14;
                    dm = *(u8 **)(g->unk_00 + 0x8D0);
                    *(u32 *)(g->unk_00 + 0x8D0) = (u32)dm + 0xC;
                    func_80067F20((u32 *)dm, 0, 0, func_80066460(0, 1, 0, 0), 0);
                    *(s32 *)(ln + 0x4) = *(s32 *)(q + 0x8);
                    *(s32 *)(ln + 0xC) = *(s32 *)(q + 0x8);
                    func_800667D0(ln);
                    func_80066640(ln, 1);
                    *(s32 *)(ln + 0x8) = xw[j];
                    *(s32 *)(ln + 0x10) = xw[jj];
                    addPrim((u32 *)(g->unk_00 + 0xB0) + z, ln);
                    addPrim((u32 *)(g->unk_00 + 0xB0) + z, dm);
                }
            }
            zr -= 8;
            xr -= 8;
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        b = n->unk_08;
    }
    return 0;
}
