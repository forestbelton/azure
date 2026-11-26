/* PARK: func_s27_80024610, csrc/ovl_dng_s27.c, agent-h196, 2026-11-25.

   TARGET 233 words. BEST BUILT 237 words, 45 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py; ovlverify reports 176 unaligned on the same
   body). A SECOND BODY, identical but with the fourth RotTransPers argument
   written `&sp->unk_88' inline instead of through the `fp4' local, builds
   232 words / 131 aligned rows -- every one of those 131 is a register
   rename. 1 member, 233 words (dngfam.py).

   WHAT IT DOES. The per-sprite renderer the three dressers
   func_s27_800249B4 / func_s27_80024B34 / func_s27_80024C84 call 8, 8 and
   up to 8 times each. Sixteen semi-transparent LINE_G2 primitives, i from
   0xF down to 0: each takes two consecutive entries of the caller's 34-word
   sin/cos table (t[i], t[i+1] for x; t[i+17], t[i+18] for y), scales them by
   the s16 `w', offsets them by the UnkVec's +0x2/+0x6, projects the two
   endpoints through the 0x1F800000 scratchpad with two RotTransPers calls,
   colours both ends from D_s27_8002588C's byte ramp indexed `(i + h) % 16'
   and `(i + h + 1) % 16' scaled by `(g << 16) / 255 >> 18', and -- when the
   averaged z is under 0x1E0 -- addPrims the line and a fresh DR_MODE
   (GetTPage(0, 1, 0, 0)) into D_80083160.unk_00 + 0xB0.

   THE SIGNATURE IS SETTLED AND THE THREE CALLERS ARE LANDED AGAINST IT:
     s32 func_s27_80024610(u8 *p, UnkVec *b, u8 *c, s16 w, s32 e,
                           s32 *t, u8 g, s16 h);
   Parameters 1 (`p') and 3 (`c') ARE NEVER READ -- $a0 is overwritten by
   %hi(D_80083160) in the second word and $a2 is never saved. They are in the
   signature because all three callers pass their own first three parameters
   through unchanged. Parameter 7 is `u8' (`lbu 0x68($sp)' with no
   sign-extension and `g << 16' as a bare `sll'), parameter 8 is `s16' (a
   plain HImode load is `lhu' on this target either way, but the USE at
   `i + h' is `sll 16' / `sra 16' -- a `u16' there gives `andi 0xFFFF' and
   costs the three callers 2 words each, which is how the three of them
   landed).

   THE ONE SPELLING THAT CHANGED THE WORD COUNT, measured here:
     `u = g << 16; u = u / 255;'   (two statements)      231 w, 126 rows
     `u = (g << 16) / 255;'        (one statement)       236 w, 115 rows
   The two-statement form sets the user variable `u' TWICE in the loop, so
   `n_times_set[u] == 2' and loop.c's `consec_sets_invariant_p' fails on
   non-consecutive sets: the shift and the whole /255 magic sequence are then
   not movables at all and stay in the loop, which is where the target has
   them. Written as one statement loop.c hoists `(g << 16)' and `(g << 16)
   >> 31' out, spills both, and the body gains 5 words.

   THE RESIDUE IS ONE loop.c DECISION AND IT IS MEASURED IN THE DUMP.
   `tools/cdkgcc -da' on this unit prints, for this loop:

     Loop from 53 to 491: 175 real insns.
     ...
     Insn 263: regno 190 (life 13), savings 1  moved to 514
     Insn 265: regno 191 (life 13), savings 1  moved to 515

   and insns 263/265 are exactly `(plus (reg sp) 132)' and
   `(plus (reg sp) 136)' -- the third and fourth RotTransPers arguments,
   &sp->unk_84 and &sp->unk_88. THE TARGET MOVES 263 AND NOT 265. Its
   &sp->unk_84 is the hoisted-and-spilled 0x1F800084 in 0x20($sp) that the
   loop reloads twice; its &sp->unk_88 is `addiu $s0, $s2, 0x88' INSIDE the
   loop, a block-local pseudo that local-alloc gives $s0 -- which is what
   takes $s0 out of global.c's pool and SPILLS the `b' parameter to
   0x54($sp). Here both move, nothing is block-local, `b' keeps $s8, and the
   131 register renames follow from that one difference.

   move_movables' test is `(threshold * savings * m->lifetime) >= insn_count'
   with `threshold -= 3' after every move (loop.c:1730, :1818, :2003). With
   savings 1 and life 13 for both, and four moves already done when 263 is
   reached, the target needs

       (T - 15) * 13  <  insn_count  <=  (T - 12) * 13

   while this body has insn_count = 175 and clears both. Bounding T from the
   same dump's other verdicts (insn 99, life 2 savings 2, `not desirable';
   insn 265, life 13, moved) gives 28.46 <= T < 43.75, so the target's loop
   has ROUGHLY 183 TO 221 REAL INSNS AT loop.c TIME -- at least eight more
   than this body -- all of which combine must remove again, because the two
   final instruction streams are otherwise identical. FINDING THOSE EIGHT
   INSNS IS THE WHOLE REMAINING JOB.

   SPELLINGS MEASURED FOR THE TWO ARGUMENT ADDRESSES (aligned rows):
     arg3 `&sp->unk_84'       arg4 `&sp->unk_88'           232 w, 131
     arg3 `&sp->unk_84'       arg4 `fp4' local in loop     237 w,  45  <-- kept
     arg3 `fp3' local in loop arg4 `&sp->unk_88'           232 w, 131
     arg3 `fp3' local in loop arg4 `fp4' local in loop     232 w, 125
     arg3 `fp3' local before the loop, arg4 inline         232 w, 130
     arg3 `(s32 *)0x1F800084' arg4 `&sp->unk_88'           231 w, 126
     arg3 `&sp->unk_84'       arg4 `(s32 *)0x1F800088'     231 w, 123
     arg3 `(s32 *)0x1F800084' arg4 `(s32 *)0x1F800088'     230 w, 164
     arg3 `&sp->unk_84'       arg4 `&sp->unk_84 + 1'       232 w, 131
     arg3 `&sp->unk_84'       arg4 `(u8 *)' local cast     237 w,  45
   Only the `fp4'-local shape reaches the 45-row class, and in it the roles
   are EXACTLY SWAPPED from the target: the block-local $s0 holds
   &sp->unk_84 and the spilled slot holds 0x1F800088. The mirror spelling
   does not mirror the result, which is why the threshold arithmetic above
   was worked out rather than swept further.

   OTHER SPELLINGS MEASURED AND FREE OR WORSE, all against 232 w / 131:
     `q->r0 = q->b0 = m0'  (chained)                       232 w, 135
     `q->b0 = q->r0 = m0'  (chained, other way)            232 w, 131
     `q->r0 = m0; q->g0 = g; q->b0 = m0;' (setRGB0 order)  232 w, 131
     `q->r0; q->b0; q->g0' (colour before green)           232 w, 131
     `u8 *bb = (u8 *)b' local for the four b reads         232 w, 131
     `q->g0 = g; u = (q->g0 << 16) / 255;' (read-back)     234 w, 149
       and with both greens stored first                   233 w, 146
       -- the read-back is NOT forwarded by cse1, so an `lbu 0x5($s1)'
          survives in the loop and the word count is bought the wrong way.
     `q->x0 = sp->unk_D8 & 0xFFFF' x4 (insn padding)       232 w, 131
       -- cse1 folds the masks before loop.c, so the dump still says 175.
   LOAD-BEARING AND NOT RE-MEASURED AS ALTERNATIVES: the two colour locals
   m0/m1 (writing the `(u * tbl) >> 18' expression twice duplicates the
   `lbu' and the `mult' and is 247 w / 115 rows); `sp->unk_68 = sp->unk_70 =
   e' chained, which is the order the target stores 0x70 before 0x68.

   UNMEASURED: any spelling that raises the loop's RTL insn count at loop.c
   time by eight or more and is removed again by combine -- a pointer local
   folded into a memory reference is the obvious family and none was found
   in this body; and `-da' on .sched1/.greg, which would say which of the
   nine callee-saved values the target's global.c dropped first.

   NO TWIN. dngtwin.py --unit ovl_dng_s27 reports nothing above 0.62 for
   this body and xsibs.py at --min-words 18 --ratio 0.70 finds nothing. The
   tail WAS found by grep, and it is the whole reason this body is 45 rows
   and not 400: csrc/ovl_dng_s15.c's landed func_s15_800248C8 is the same
   LINE_G2-through-the-scratchpad renderer, and the UnkDngS14LineSP struct,
   the `sp->unk_18 = D_80083160.unk_00 + 0xB0' prologue, the
   `sp->unk_B4 = (sp->unk_F4 + sp->unk_F8) / 2' average and the
   `(u32)sp->unk_B4 < 0x1E0' gate are taken from it verbatim. Its own
   documented fact -- that the two INPUT addresses want `(u32)sp | 0x64' and
   not the integer constant -- is INVERTED here: this body's inputs are
   `&sp->unk_64' / `&sp->unk_D8' (plain `addiu'), and the `|' form does not
   appear anywhere in this listing.

   DECLARATIONS
   typedef struct UnkDngS14LineSP
   {
       u8 pad_00[0x18];
       u8 *unk_18;
       u8 pad_1C[0x48];
       u16 unk_64;
       u16 unk_66;
       u16 unk_68;
       u8 pad_6A[0x2];
       u16 unk_6C;
       u16 unk_6E;
       u16 unk_70;
       u8 pad_72[0x12];
       s32 unk_84;
       s32 unk_88;
       u8 pad_8C[0x28];
       s32 unk_B4;
       u8 pad_B8[0x20];
       u16 unk_D8;
       u16 unk_DA;
       u16 unk_DC;
       u16 unk_DE;
       u8 pad_E0[0x14];
       s32 unk_F4;
       s32 unk_F8;
   } UnkDngS14LineSP;
   void func_80067F20(u32 *, s32, s32, s32, s32);
   extern u8 D_s27_8002588C[];
   END DECLARATIONS
*/
s32 func_s27_80024610(u8 *p, UnkVec *b, u8 *c, s16 w, s32 e, s32 *t, u8 g, s16 h)
{
    UnkDngS14LineSP *sp = (UnkDngS14LineSP *)0x1F800000;
    Unk80083160 *gg = &D_80083160;
    LINE_G2 *q;
    u8 *dm;
    s32 i;
    s32 j;
    s32 u;
    s32 m0;
    s32 m1;
    s32 *fp4;

    sp->unk_18 = D_80083160.unk_00 + 0xB0;
    sp->unk_68 = sp->unk_70 = e;
    for (i = 0xF; i >= 0; i--)
    {
        q = *(LINE_G2 **)(gg->unk_00 + 0x8D0);
        *(LINE_G2 **)(gg->unk_00 + 0x8D0) = q + 1;
        setlen(q, 4);
        setcode(q, 0x52);
        u = g << 16;
        u = u / 255;
        j = i + h;
        m0 = (u * D_s27_8002588C[j % 16]) >> 18;
        q->g0 = g;
        q->r0 = m0;
        q->b0 = m0;
        m1 = (u * D_s27_8002588C[(j + 1) % 16]) >> 18;
        q->g1 = g;
        q->r1 = m1;
        q->b1 = m1;
        sp->unk_64 = *(u16 *)((u8 *)b + 0x2) + ((t[i] * w) >> 8);
        sp->unk_6C = *(u16 *)((u8 *)b + 0x2) + ((t[i + 1] * w) >> 8);
        sp->unk_66 = *(u16 *)((u8 *)b + 0x6) + ((t[i + 17] * w) >> 8);
        sp->unk_6E = *(u16 *)((u8 *)b + 0x6) + ((t[i + 18] * w) >> 8);
        fp4 = &sp->unk_88;
        sp->unk_F4 = RotTransPers((SVECTOR *)&sp->unk_64, (s32 *)&sp->unk_D8, &sp->unk_84,
                                  fp4);
        sp->unk_F8 = RotTransPers((SVECTOR *)&sp->unk_6C, (s32 *)&sp->unk_DC, &sp->unk_84,
                                  fp4);
        q->x0 = sp->unk_D8;
        q->y0 = sp->unk_DA;
        q->x1 = sp->unk_DC;
        q->y1 = sp->unk_DE;
        sp->unk_B4 = (sp->unk_F4 + sp->unk_F8) / 2;
        if ((u32)sp->unk_B4 < 0x1E0)
        {
            addPrim(sp->unk_18 + sp->unk_B4 * 4, q);
            dm = *(u8 **)(gg->unk_00 + 0x8D0);
            *(u32 *)(gg->unk_00 + 0x8D0) = (u32)dm + 0xC;
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            addPrim(sp->unk_18 + sp->unk_B4 * 4, dm);
        }
    }
    return 0;
}
