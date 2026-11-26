/* PARK: func_s60_80024D84, csrc/ovl_dng_s60.c, agent-h278, 2026-11-26.

   TARGET 281 words. BEST BUILT 281 words, 11 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py); ovlverify reports the same 11 masked, which
   means nothing is shifted. 1 member, 281 words (tools/decomp/dngfam.py) --
   no replication value. OWNS NO JUMP TABLE: the dispatch on
   *(s16 *)(p + 0x4) is a balanced compare tree over {0,1,2,3}, four arms,
   under cc1psx's five-case threshold. relcand: ok, 0 data symbols, 13 calls.
   declcheck s60 --src: clean.

   VERDICT: ALLOC, and it is ONE PAIR OF REGISTERS. Every opcode, immediate
   and displacement matches and nothing is moved. The entire residue is that
   the loop counter `i' and the top-vertex colour `c1' hold each other's
   callee-saved register: the target has i in $s6 and c1 in $s5, this body has
   i in $s5 and c1 in $s6. The 11 rows are words 105, 117, 141, 207, 211, 214,
   216, 228, 235, 250 and 253 -- which is EVERY site either variable is named
   at, i.e. a clean whole-variable exchange with no partial agreement, so
   there is no naming evidence pointing at a mis-assigned site.

   WHAT THE FUNCTION IS. It builds a 23-segment translucent ribbon in the PSX
   scratchpad: `sp' is the UnkDngS60PolySP at the constant 0x1F800000 that the
   unit's landed func_s60_80024770 (POLY_GT3) and func_s60_80024A30 (POLY_GT4)
   read their six vertices out of. A brightness ramp `r' is picked from the
   actor's state at +0x4, divided by 29, and scaled by 23 / 24 / 29 to greyscale
   the opening triangle; the loop then walks 23 pairs of angles out of the
   actor record with two-byte stride, projects them through func_800644B8 and
   func_80064584 (sin / cos), and emits one GT4 per step with the previous and
   current greyscales as its four vertex colours. The closing block re-publishes
   the last ring and emits one more GT3.

   THE MAGIC DIVISOR IS 29, computed numerically, not by hand. The listing has
   lui 0x8D3D / ori 0xCB09 / mult / mfhi / addu / sra 4 / subu(sra 31), which is
   gcc's add-variant signed divide; brute-forcing the quotient function against
   x / d over ten probe values gives d = 29 uniquely. My own hand arithmetic on
   the same constant produced 29 and then 30 and then 10 on three tries -- this
   is the `never regex padded output' failure in another costume, so compute it.

   THE DONOR is the landed func_s28_80024A64 (csrc/ovl_dng_s28.c, 348 words,
   3 params, s32 return). dngcallee was NOISE here -- only two symbols to
   search (func_800644B8, func_80064584) and 140 functions tie at 2/2, which is
   exactly the brief's documented noise condition -- and dngtwin found no carved
   sibling at 0.70 for either of this unit's stubs. What found the donor was
   GREPPING THE LISTING'S OWN SHAPE: the constant 0x1F800048 appears in exactly
   THREE listings in the whole corpus, and two of them are landed bodies in
   ovl_dng_s28. Of those two, func_s28_80024A64 is the frame match (both save
   s0..s7 plus ra, 8 callee-saved registers) and the arity match. Its
   `UnkDngS28PolySP *sp = (UnkDngS28PolySP *)0x1F800000;' idiom, its
   `(f(angle) >> 4) * radius << 8' vertex form and its `sp->unk_A = sp->unk_B'
   ring-shuffle are this body's verbatim.

   AND THE UNUSED TYPEDEF IN THIS UNIT NAMED THE SHAPE FIRST. `UnkDngS60PolySP'
   sits in ovl_dng_s60.c's declaration block with no user among the 13 landed
   bodies that have it in scope -- the brief's `an unused typedef can name your
   twin' lever, fired cleanly: it is precisely the scratchpad struct this body
   needs, with unk_30..unk_68 already laid out as the six-vertex matrix.

   MEASUREMENT TABLE. Every row is a single ablation off the 281 / 11 body
   below unless the row says otherwise. These are MID-FIGHT figures: the body
   never reached 0, so every one of them prices its spelling AND the live
   register exchange together, and the brief's rule applies -- re-price any row
   you intend to keep once the exchange is solved.

   | spelling                                                | words | rows |
   |---------------------------------------------------------|-------|------|
   | the body as parked                                      |  281  |  11  |
   | THE SIX LOOP READS AS SUBSCRIPTS OFF p, p + i * 2 + K    |  281  |  11  |
   |   ... a walked cursor, q = p; ... ; q += 2;              |  281  |  22  |
   | case 1 and case 2 as TWO SEPARATE ARMS, same body        |  281  |  11  |
   |   ... written case 1: case 2: sharing one arm            |  282  |  66  |
   |   ... the same, measured before the cursor fix           |  281  | 123  |
   | the OLD c1 saved to a temp, c1 and c2 assigned direct    |  281  |  11  |
   |   ... new values in t1/t2 then c1 = t1; c2 = t2;         |  282  |  22  |
   | c2 = c3 AFTER the opening func_s60_80024770 call         |  281  |  11  |
   |   ... before it                                          |  281  |  37  |
   | default arm as r = r & 0xFFFF                            |  281  |  11  |
   | NOT MEASURABLE, identical 11 rows and identical rows:    |       |      |
   |   declaring i first among the s32 locals, or last        |  281  |  11  |
   |   t1 = c1 and c3 = c2 in either order                    |  281  |  11  |
   |   a local for the opening r * 24 colour, or inline       |  281  |  11  |
   |   q += 2 before or after the GT4 call (cursor form)      |  281  |  22  |
   | ALLOCATION ATTEMPTS, all worse:                          |       |      |
   |   do { t1 = c1; } while (0)                              |  282  |  58  |
   |   do { } while (0) round the whole colour-and-emit block |  283  | 118  |

   WHY THE CURSOR FOLDS, read off the compiler and off a landed comment.
   With `q = p; ... q += 2;' the ten address uses of q are all DEST_ADDR givs
   on one biv; loop.c's combine_givs merges all ten onto giv_array[0], which is
   the LAST giv in the loop because record_giv PREPENDS (`v->next_iv = bl->giv;
   bl->giv = v'), so the base becomes q + 0x84 -- the offset of the last read.
   The accumulated lifetime and benefit then clear the
   `v->lifetime * threshold * benefit < insn_count' test and `biv 84 was
   eliminated' is printed verbatim in the -da .loop dump of a single-function
   probe. The displacements come out -0x30, -0x6C, -0x3C, -0x78, 0xC and 0.
   The landed func_800D6C18 in csrc/ovl_dungeon_800D50FC.c already prices this
   exact mechanism in its own header -- `a q = p; q += 2; cursor folds the
   +0x4C into the biv and the displacements come out -8 and 0' -- and its cure,
   writing the reads as subscripts off p, carried here for 11 rows.

   WHAT I RULED OUT ON THE EXCHANGE, so the next agent starts after it.
   The .greg dump of a single-function probe prints the allocation order as
   86 84 85 87, that is c2, i, c1, c3, and the target needs c2, c1, i, c3 --
   one adjacent transposition. The conflict rows for 84 and 85 are IDENTICAL
   sets, so it is not a conflict asymmetry. Permuting the declaration order is
   FLAT (two permutations, 11 rows and the same 11 rows), which per the brief's
   correction rules out declaration-order RANKING only for locals not tied to
   something else -- neither of these is a copy of a parameter, so the screen is
   live and the tie-break in allocno_compare (`return v1 - v2') is therefore
   never reached, i.e. the two priorities are NOT equal. .flow for this body
   gives i 11 refs over 122 insns and c1 9 refs over 169, and
   floor_log2(n) * n / length makes i strictly better at every reference count
   below 16; but those are PRE-regmove, PRE-sched1, PRE-local-alloc numbers and
   the brief says plainly they are the wrong numbers to compute priority from --
   the proof is in the same dump, where c3 scores highest of the four on them
   and is allocated LAST. So the arithmetic that would say how big a nudge is
   needed is not available from any dump this body produces, and both reference
   bumps I tried overshoot in the same direction (c1 jumps past c2 rather than
   past i). THE CAUSE IS STATED AS ARITHMETIC ONLY SO FAR AS THIS: it is one
   adjacent transposition in global_alloc's priority order between two
   correctly identified locals, with the shape, the word count, every opcode
   and every displacement already exact.

   DECLARATIONS
   s32 func_800644B8(s32);
   s32 func_80064584(s32);
   s32 func_s60_80024770(s32 a, s32 b, s32 c);
   s32 func_s60_80024A30(s32 a, s32 b, s32 c, s32 d);
   typedef struct UnkDngS60PolySP
   {
       u8 *unk_00;
       s32 unk_04;
       s32 unk_08;
       s32 unk_0C;
       s16 unk_10;
       s16 unk_12;
       s16 unk_14;
       u8 pad_16[0x2];
       s16 unk_18;
       s16 unk_1A;
       s16 unk_1C;
       u8 pad_1E[0x2];
       s16 unk_20;
       s16 unk_22;
       s16 unk_24;
       u8 pad_26[0x2];
       s16 unk_28;
       s16 unk_2A;
       s16 unk_2C;
       u8 pad_2E[0x2];
       s32 unk_30;
       s32 unk_34;
       s32 unk_38;
       u8 pad_3C[0x4];
       s32 unk_40;
       s32 unk_44;
       s32 unk_48;
       u8 pad_4C[0x4];
       s32 unk_50;
       s32 unk_54;
       s32 unk_58;
       u8 pad_5C[0x4];
       s32 unk_60;
       s32 unk_64;
       s32 unk_68;
       u8 pad_6C[0x4];
       u16 unk_70;
       u16 unk_72;
       u16 unk_74;
       u16 unk_76;
       u16 unk_78;
       u16 unk_7A;
       u16 unk_7C;
       u16 unk_7E;
   } UnkDngS60PolySP;
   END DECLARATIONS
*/
s32 func_s60_80024D84(u8 *p, UnkVec *b, u8 *s)
{
    UnkDngS60PolySP *sp = (UnkDngS60PolySP *)0x1F800000;
    s32 i;
    s32 c1;
    s32 c2;
    s32 c3;
    s32 t1;
    s32 r;

    r = 0;
    switch (*(s16 *)(p + 0x4))
    {
    case 0:
        r = *(s16 *)(p + 0x6) * 0x140000;
        break;
    case 1:
        r = 0xA00000;
        break;
    case 2:
        r = 0xA00000;
        break;
    case 3:
        r = (0xA0 - *(s16 *)(p + 0x6) * 20) << 16;
        break;
    default:
        r = r & 0xFFFF;
        break;
    }
    r = r / 29;
    sp->unk_30 =
        (func_800644B8(*(s16 *)(p + 0x50) + *(s16 *)(p + 0x8)) >> 4) * *(s16 *)(p + 0x14) << 8;
    sp->unk_40 =
        (func_800644B8(*(s16 *)(p + 0x52) + *(s16 *)(p + 0x8)) >> 4) * *(s16 *)(p + 0x16) << 8;
    sp->unk_50 =
        (func_800644B8(*(s16 *)(p + 0x46) + *(s16 *)(p + 0x8)) >> 4) * *(s16 *)(p + 0xA) << 8;
    sp->unk_34 =
        (func_80064584(*(s16 *)(p + 0x50) + *(s16 *)(p + 0x8)) >> 4) * *(s16 *)(p + 0x14) << 8;
    sp->unk_44 =
        (func_80064584(*(s16 *)(p + 0x52) + *(s16 *)(p + 0x8)) >> 4) * *(s16 *)(p + 0x16) << 8;
    sp->unk_54 =
        (func_80064584(*(s16 *)(p + 0x46) + *(s16 *)(p + 0x8)) >> 4) * *(s16 *)(p + 0xA) << 8;
    c1 = (r * 23 >> 16 & 0xFF) * 0x10101;
    c3 = (r * 29 >> 16 & 0xFF) * 0x10101;
    sp->unk_38 = *(s16 *)(p + 0x8C) << 16;
    sp->unk_48 = *(s16 *)(p + 0x8E) << 16;
    sp->unk_58 = *(s16 *)(p + 0x82) << 16;
    func_s60_80024770((r * 24 >> 16 & 0xFF) * 0x10101, c1, c3);
    c2 = c3;
    sp->unk_60 = sp->unk_50;
    sp->unk_64 = sp->unk_54;
    sp->unk_68 = sp->unk_58;
    for (i = 0; i < 0x17; i++)
    {
        sp->unk_30 = sp->unk_40;
        sp->unk_40 = (func_800644B8(*(s16 *)(p + i * 2 + 0x54) + *(s16 *)(p + 0x8)) >> 4) *
                     *(s16 *)(p + i * 2 + 0x18) << 8;
        sp->unk_50 = sp->unk_60;
        sp->unk_60 = (func_800644B8(*(s16 *)(p + i * 2 + 0x48) + *(s16 *)(p + 0x8)) >> 4) *
                     *(s16 *)(p + i * 2 + 0xC) << 8;
        sp->unk_34 = sp->unk_44;
        sp->unk_44 = (func_80064584(*(s16 *)(p + i * 2 + 0x54) + *(s16 *)(p + 0x8)) >> 4) *
                     *(s16 *)(p + i * 2 + 0x18) << 8;
        sp->unk_54 = sp->unk_64;
        sp->unk_64 = (func_80064584(*(s16 *)(p + i * 2 + 0x48) + *(s16 *)(p + 0x8)) >> 4) *
                     *(s16 *)(p + i * 2 + 0xC) << 8;
        sp->unk_38 = sp->unk_48;
        sp->unk_48 = *(s16 *)(p + i * 2 + 0x90) << 16;
        sp->unk_58 = sp->unk_68;
        sp->unk_68 = *(s16 *)(p + i * 2 + 0x84) << 16;
        c3 = c2;
        t1 = c1;
        c1 = ((0x16 - i) * r >> 16 & 0xFF) * 0x10101;
        c2 = ((0x1C - i) * r >> 16 & 0xFF) * 0x10101;
        func_s60_80024A30(t1, c1, c3, c2);
    }
    sp->unk_30 = sp->unk_40;
    sp->unk_40 = sp->unk_60;
    sp->unk_34 = sp->unk_44;
    sp->unk_44 = sp->unk_64;
    sp->unk_38 = sp->unk_48;
    sp->unk_48 = sp->unk_68;
    func_s60_80024770(c1, c2, c3);
    return 0;
}
