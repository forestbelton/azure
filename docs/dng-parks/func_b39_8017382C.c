/* PARK: func_b39_8017382C, csrc/ovl_dng_b39.c, agent-h198, 2026-11-25.

   TARGET 442 words.  BEST BUILT 442 words, 22 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py).  1 member, 442 words (dngfam.py) -- no
   replication value beyond its own body.
   VERDICT: ALLOC.  Every opcode, immediate and displacement matches except
   for one 2-row schedule shift; all 22 rows are register numbers at three
   sites, and all three want $a1 where this body uses $v0 or $a0.

   DONOR.  dngcallee.py puts this in the 42-member `p[0x9B]' dress/step
   family whose representative text is csrc/ovl_dng_b43.c's
   func_b43_80173228 (231 words, landed).  Frames agree exactly: -0x28 with
   s0,s1,s2,s3,s4,ra, so b43's allocation facts are in scope.  This body is
   that family's 442-word variant: it has FOUR states instead of three, and
   two of them (1 and 3) are new -- a GetClut fade that b43 does not have.
   b43's whole case 1 is this body's case 2 from the func_80042900 gate
   onward, statement for statement, and all of b43's recorded facts held:
   two different `&D_80083460' locals per site, the tail written out at each
   of the three dress sites rather than reached by goto, and the
   `d + 0x64' / func_800AA6B4 chain nested rather than inverted.

   RESOLVED NAMES.  func_8006649C is GetClut (splat/common_syms.txt), so
   case 1 and case 3 are
     *(s16 *)(c + 0x12) = GetClut(((x % 8 >> 2) << 6) + 0x340,
                                  (x >> 3) + 0x100 or 0x102)
                          - *(u16 *)(p + 0xAC);
   with `s16 x = *(u16 *)(p + 0xAE)' -- the u16 read plus the s16 local is
   what emits `lhu' + `sll 16'/`sra 22' and `sra 19'; a `*(s16 *)' read
   emits `lh' and loses the sll pair.
   The spawned node's step function is func_b39_80174424 (landed this run,
   145 words): its RECT is the +0xA0..+0xA6 the dress block stores and its
   duration is +0xA8, so no offset in the dress block is guessed.

   THE THREE RESIDUE SITES, all $a1 in the target:
     1. the i*2 temp of each scan loop ($v0 here, $a1 there).  In the target
        that temp is live across the loop body -- reorg puts the `sll' in the
        exit branch's delay slot -- so it conflicts with $v0 (the loop's byte
        temp) and $a0 (the cursor) and is forced to $a1.  Here it is born
        after the loop, $v0 is free, and local-alloc takes it.
     2. `m' at dress site 1 ($a0 here, $a1 there).  Mechanism read out of
        -da: global.c's find_reg pass 0 only considers registers already in
        `regs_used_so_far' and excludes `regs_someone_prefers' -- which for
        `m' contains $a0, because the parameter `p' is copied from $a0 and
        conflicts with `m'.  When this body allocates `m' the only register
        used so far is $v1 (allocnos 159, 342, 84, 85, 86 all take it), so
        pass 0 finds nothing and pass 1 takes the first free register in
        reg_alloc_order, $a0.  In the target something earlier has already
        taken $a1, so pass 0 succeeds there.  Sites 2 and 3 are already
        correct ($a1), because in case 2 the `a0 = d' argument conflicts.
     THEREFORE the two are ONE problem: fixing (1) puts $a1 into
     regs_used_so_far and (2) should follow.  That is a prediction, not a
     measurement.

   MEASURED SPELLINGS -- words / aligned rows.  The base is the body below.

     | spelling                                                | words | rows |
     |---------------------------------------------------------|-------|------|
     | THE BODY BELOW                                          |  442  |  22  |
     | the two scan loops as `while ((q[0] & 0x20) != 0)'      |  452  |  98  |
     | ... as for(;;) + break, or for(;;i++), or an index-only |  452  |  98  |
     |     `while ((base[i * 0xC] & 0x20) != 0)', or the body  |       |      |
     |     statements swapped -- SIX forms, all identical      |       |      |
     | the scan loops as `goto' back edges (adopted)           |  445  |  89  |
     | `m = (u8 *)n + 0x20' inside `if (n != NULL)'            |  445  |  89  |
     | ... hoisted ABOVE the gate (adopted)                    |  445  |  77  |
     | ... with `n->unk_10 = ' written before it               |  445  |  89  |
     | the p+0xAC read inline at the +0xA2 store               |  444  |  75  |
     | ... as `s16 y' read BEFORE the dress block              |  441  |  49  |
     | ... as `s16 y' read after the +0xA0 store (adopted)     |  441  |  28  |
     | ... as `s16 y' read after the +0xA4 store               |  441  |  33  |
     | loop 1 `i = 0' first / loop 2 `i2 = 0' first            |  441  |  28  |
     | loop 1 `q = ' first / loop 2 `i2 = 0' first (adopted)   |  442  |  26  |
     | loop 1 `q = ' first / loop 2 `q2 = ' first              |  441  |  28  |
     | both loops `i' first                                    |  442  |  26  |
     | `do { } while (0)' around a scan loop                   |  442  |  26  |
     | ... around the loop AND its tail store                  |  438  |  42  |
     | ... around the TAIL STORE ONLY (adopted)                |  443  |  23  |
     | ... plus `q = ' moved above the +0xAE store (adopted)   |  442  |  22  |
     | ... plus `do { } while (0)' around dress site 1         |  442  |  22  |
     | the dress stores as array refs `((s16 *)m)[0x50]'       |  442  |  26  |
     | the p+0xAC read as an array ref `((u16 *)p)[0x56]'      |  442  |  26  |
     | the tail index as `((u16 *)base)[i * 6 + 3]'            |  442  |  26  |
     | the tail index as `i * 0xC + base + 0x6'                |  442  |  26  |
     | one shared (q, i) pair for both scan loops              |  442  |  26  |
     | `s32 i' declared before `u8 *q'                         |  442  |  26  |
     | `s16 i' instead of `s32 i'                              |  447  |  27  |
     | the scan cursor stepped by 0xC on the INDEX             |  437  |  39  |
     | `q' re-read after the loop for the tail base            |  442  |  26  |
     | `*q & 0x20' instead of `q[0] & 0x20'                    |  442  |  26  |

   WHY `goto' AND NOT `while'.  This is the load-bearing fact and it is a
   compiler reading, not a guess.  stmt.c's expand_end_loop rotates any loop
   that begins with a conditional exit, and jump.c:661 then calls
   duplicate_loop_exit_test on the leading `simplejump', which copies the
   test above the loop: that is 4 words per scan loop and SIX different C
   loop spellings all reach it (measured identical at 452/98).  A `goto' back
   edge emits no NOTE_INSN_LOOP_BEG, so neither pass fires and the emitted
   shape -- test at the top, body, `j' back -- is the target's exactly.  The
   price of the `goto' is that flow.c:2122 stops loop-weighting the
   references inside it, which is what the `do { } while (0)' around the tail
   store buys back: it restores depth 1 over the two references `i' has in
   the tail, taking i from 5 refs to 7 so that
   allocno_compare's floor_log2(7)*7/11 = 1.27 beats `q''s
   floor_log2(4)*4/7 = 1.14 and `i' is allocated FIRST, which is what puts
   the cursor in $a0 and the index in $v1 as the target has them.  Without
   it the loops' two registers are exchanged (26 rows).  DEVICE, or macro
   residue -- the wrapped statement is one coherent publish.

   UNMEASURED, and the places to look next:
     * anything that puts $a1 into regs_used_so_far before `m' is allocated
       -- the prediction above.  No spelling for this was tried.
     * the 2-row schedule shift at 801738E8 (`sll $v0, $v0, 2' one slot
       earlier in the target).  Only the two `q = ' positions were tried.
     * writing case 1 and case 3 with ONE shared (x, u) pair.  Both cases
       have their own locals here, on b43's two-locals rule, and the shared
       form was never priced.
     * the case-2 scan loop's `q2 = ' hoisted above the +0xAC gate measured
       439/31, i.e. worse, but only in combination with loop 1 unhoisted.

   DECLARATIONS
   s32 func_80042900(s8 *, s8);
   void func_80042B68(s8 *, s8);
   s16 func_8009FD40(u8 *, u8 *);
   s32 func_800AA79C(u8 *, void *, u8 *, u8 *);
   UnkNode *func_8003FD64(s32, void *);
   extern u8 D_80083498[];
   void func_b39_80174424(u8 *p, UnkVec *b, u8 *c, u8 *d);
   END DECLARATIONS
*/
void func_b39_8017382C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;
    UnkNode *n2;
    UnkNode *n3;
    u8 *q;
    u8 *q2;
    u8 *e;
    u8 *g;
    u8 *h;
    u8 *k;
    u8 *k2;
    u8 *k3;
    u8 *m;
    u8 *m2;
    u8 *m3;
    s32 i;
    s32 i2;
    s32 v;
    s16 y;
    s16 y2;
    s16 y3;
    s16 x;
    s16 u;
    s16 x2;
    s16 u2;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        q = *(u8 **)(c + 0x8);
        *(s16 *)(p + 0xAE) = *(u16 *)(c + 0x12);
        i = 0;
    scan1:
        if ((q[0] & 0x20) != 0)
        {
            q += 0xC;
            i++;
            goto scan1;
        }
        do
        {
            *(s16 *)(p + 0xAC) = *(u16 *)(*(u8 **)(c + 0x8) + i * 0xC + 0x6) & 0xFFC0;
        } while (0);
        *(u8 **)(c + 0x2C) = D_80175298;
        func_80047784(c, D_80175298[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        n = func_8003FD64(0x10, D_80083498);
        m = (u8 *)n + 0x20;
        if (n != NULL)
        {
            n->unk_10 = (u32)func_b39_80174424;
            *(s16 *)(m + 0xA0) = 0;
            y = *(u16 *)(p + 0xAC);
            *(s16 *)(m + 0xA4) = 0x100;
            *(s16 *)(m + 0xA6) = 1;
            *(s16 *)(m + 0xA8) = 8;
            *(s16 *)(m + 0xA2) = y >> 6;
            *(s16 *)(p + 0x96) = 8;
            *(s16 *)(m + 0x96) = 0;
            *(s16 *)(m + 0x9E) = 0;
        }
        p[0x9B] += 1;
        return;
    case 1:
        x = *(u16 *)(p + 0xAE);
        u = x % 8;
        *(s16 *)(c + 0x12) = GetClut(((u >> 2) << 6) + 0x340, (x >> 3) + 0x100) - *(u16 *)(p + 0xAC);
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        if (*(s16 *)(p + 0x96) > 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            return;
        }
        *(s16 *)(c + 0x12) = *(u16 *)(p + 0xAE) - 0x40;
        p[0x9B] += 1;
        return;
    case 2:
        if (*(s16 *)(p + 0xAC) == 0)
        {
            i2 = 0;
            q2 = *(u8 **)(c + 0x8);
        scan2:
            if ((q2[0] & 0x20) != 0)
            {
                q2 += 0xC;
                i2++;
                goto scan2;
            }
            do
            {
                *(s16 *)(p + 0xAC) = *(u16 *)(*(u8 **)(c + 0x8) + i2 * 0xC + 0x6) & 0xFFC0;
            } while (0);
        }
        if ((s16)func_80042900((s8 *)d, 1) == 0)
        {
            *(u8 **)(c + 0x2C) = D_80175298;
            func_80047784(c, D_80175298[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            n2 = func_8003FD64(0x10, D_80083498);
            m2 = (u8 *)n2 + 0x20;
            if (n2 != NULL)
            {
                n2->unk_10 = (u32)func_b39_80174424;
                *(s16 *)(m2 + 0xA0) = 0;
                y2 = *(u16 *)(p + 0xAC);
                *(s16 *)(m2 + 0xA4) = 0x100;
                *(s16 *)(m2 + 0xA6) = 1;
                *(s16 *)(m2 + 0xA8) = 8;
                *(s16 *)(m2 + 0xA2) = y2 >> 6;
                *(s16 *)(p + 0x96) = 8;
                *(s16 *)(m2 + 0x96) = 0;
                *(s16 *)(m2 + 0x9E) = 1;
            }
            *(s16 *)(p + 0xAE) = *(u16 *)(c + 0x12) + 0x40;
            k = (u8 *)&D_80083460;
            *(u16 *)(k + 0xA) += 1;
            p[0x9B] += 1;
            return;
        }
        g = (u8 *)&D_80083460;
        if ((*(u16 *)(g + 0x2) & 0x1000) != 0)
        {
            return;
        }
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, NULL) != 0)
        {
            return;
        }
        if (d[0x25] == 0)
        {
            if ((*(u16 *)(g + 0x2) & 0x2008) != 0)
            {
                return;
            }
            func_800AA79C(p, b, c, d);
            return;
        }
        if ((s16)func_800A2C34(d) != 0)
        {
            return;
        }
        v = *(s32 *)(d + 0x1C);
        if ((v & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if ((v & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_b39_80174250(p, (u8 *)b, c, d);
            return;
        }
        if (*(s8 *)(d + 0x6D) == 0)
        {
            return;
        }
        if ((s16)func_800A2C34(d) != 0 &&
            (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
        {
            return;
        }
        func_800A9A0C(d);
        func_800A9A04(d);
        if ((s16)func_80042900((s8 *)d, 1) != 0)
        {
            e = D_80082E80;
            if (*(s8 *)(c + 0x26) != *(s8 *)(e + 0x26) || *(s8 *)(c + 0x26) < 0)
            {
                if ((s16)func_8009FD40(e, c) >= 2)
                {
                    goto again;
                }
            }
            if ((func_800A6D30() & 7) == 0)
            {
                func_80042B68((s8 *)d, 1);
            }
        }
    again:
        if ((s16)func_80042900((s8 *)d, 1) != 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_80175298;
        func_80047784(c, D_80175298[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        n3 = func_8003FD64(0x10, D_80083498);
        m3 = (u8 *)n3 + 0x20;
        if (n3 != NULL)
        {
            n3->unk_10 = (u32)func_b39_80174424;
            *(s16 *)(m3 + 0xA0) = 0;
            y3 = *(u16 *)(p + 0xAC);
            *(s16 *)(m3 + 0xA4) = 0x100;
            *(s16 *)(m3 + 0xA6) = 1;
            *(s16 *)(m3 + 0xA8) = 8;
            *(s16 *)(m3 + 0xA2) = y3 >> 6;
            *(s16 *)(p + 0x96) = 8;
            *(s16 *)(m3 + 0x96) = 0;
            *(s16 *)(m3 + 0x9E) = 1;
        }
        k2 = (u8 *)&D_80083460;
        *(u16 *)(k2 + 0xA) += 1;
        p[0x9B] += 1;
        *(s16 *)(p + 0xAE) = *(u16 *)(c + 0x12) + 0x40;
        return;
    case 3:
        x2 = *(u16 *)(p + 0xAE);
        u2 = x2 % 8;
        *(s16 *)(c + 0x12) = GetClut(((u2 >> 2) << 6) + 0x340, (x2 >> 3) + 0x102) - *(u16 *)(p + 0xAC);
        if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
            if (*(s16 *)(p + 0x96) > 0)
            {
                return;
            }
        }
        k3 = (u8 *)&D_80083460;
        *(u16 *)(k3 + 0xA) -= 1;
        *(s32 *)(d + 0x1C) &= ~0x200;
        *(u32 *)(p + 0x8C) = (u32)func_b39_80170F6C;
        *(s16 *)(c + 0x12) = *(u16 *)(p + 0xAE);
        return;
    }
}
