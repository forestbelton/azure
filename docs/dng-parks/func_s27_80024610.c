/* PARK: func_s27_80024610, csrc/ovl_dng_s27.c, agent-h196, 2026-11-25.

   TARGET 233 words. BEST BUILT 235 words, 44 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py; ovlverify reports 209 unaligned on the same
   body). 1 member, 233 words (dngfam.py). The three bodies that CALL this
   one -- func_s27_800249B4, func_s27_80024B34, func_s27_80024C84 -- all
   LANDED at 0 differing against the signature below, so the prototype is
   settled and only this body is open.

   WHAT IT DOES. Sixteen semi-transparent LINE_G2 primitives, i from 0xF down
   to 0. Each takes two consecutive entries of the caller's 34-word sin/cos
   table (t[i], t[i+1] for x; t[i+17], t[i+18] for y), scales them by the s16
   `w', offsets them by the UnkVec's +0x2/+0x6, projects both endpoints
   through the 0x1F800000 scratchpad with two RotTransPers calls, colours
   both ends from D_s27_8002588C's byte ramp indexed `(i + h) % 16' and
   `(i + h + 1) % 16' scaled by `((g << 16) / 255 * ramp) >> 18', and -- when
   the averaged z is under 0x1E0 -- addPrims the line and a fresh DR_MODE
   (GetTPage(0, 1, 0, 0)) into D_80083160.unk_00 + 0xB0.

   THE SIGNATURE, which the three landed callers are built against:
     s32 func_s27_80024610(u8 *p, UnkVec *b, u8 *c, s16 w, s32 e,
                           s32 *t, u8 g, s16 h);
   Parameters 1 (`p') and 3 (`c') ARE NEVER READ -- $a0 is overwritten by
   %hi(D_80083160) in the second word and $a2 is never saved -- but all three
   callers pass their own first three parameters through unchanged, so they
   are in the list. Parameter 7 is `u8' (`lbu 0x68($sp)', then `g << 16' as a
   bare `sll'); parameter 8 is `s16' (a plain HImode load is `lhu' on this
   target whatever the sign, so the callee cannot tell you -- the USE at
   `i + h' is `sll 16' / `sra 16', and `u16' there gives `andi 0xFFFF' and
   leaves all three callers 2 words short, which is how they landed).

   ===================================================================
   THE RESIDUE IS ONE loop.c THRESHOLD DECISION, AND IT IS ARITHMETIC
   ===================================================================

   `tools/cdkgcc -da' on this unit prints, for this loop, with insn numbers
   that match `probe.c.cse':

     Loop from 53 to 491: 175 real insns.
     Insn 263: regno 190 (life 13), savings 1  moved to 514
     Insn 265: regno 191 (life 13), savings 1  moved to 515

   and insns 263 and 265 are exactly `(plus (reg sp) 132)' and
   `(plus (reg sp) 136)' -- &sp->unk_84 and &sp->unk_88, the third and fourth
   RotTransPers arguments. THE TARGET MOVES 263 AND NOT 265:

     target  arg3 = the hoisted-and-spilled constant 0x1F800084 in 0x20($sp),
                    reloaded `lw $a2, 0x20($sp)' at both calls
             arg4 = `addiu $s0, $s2, 0x88' INSIDE the loop, a block-local
                    pseudo that LOCAL-ALLOC gives $s0 -- which takes $s0 out
                    of global.c's pool, so `b' is the tenth global value and
                    is SPILLED to 0x54($sp)

   Everything else in the 131-difference version of this body was a register
   rename downstream of that one decision.

   move_movables' gate (2.7.2 loop.c:1730) is

       (threshold * savings * m->lifetime) >= insn_count

   with `threshold -= 3' after every successful move (loop.c:1818, :2003) and
   a documented starting value of 29 on this target
   (docs/matching-levers.md). Both movables have savings 1 and life 13, so
   with N moves already done when insn 263 is reached:

       arg3 moves  iff  (29 - 3N) * 13 >= insn_count
       arg4 moves  iff  (29 - 3N - 3) * 13 >= insn_count

   At insn_count 175 that needs N = 5 EXACTLY: arg3 is then tested at 14
   (14*13 = 182 >= 175, moves) and arg4 at 11 (143 < 175, stays). The first
   body of this park had N = 4 -- the D_s27_8002588C %hi/%lo pair and the
   `(s16)w' extension pair -- so both moved and `b' kept a register.

   THE FIFTH MOVE IS THE `D_80083160' ADDRESS, AND YOU GET IT BY DROPPING THE
   POINTER LOCAL. With `Unk80083160 *gg = &D_80083160;' outside the loop the
   address is not a loop invariant at all; reading the global directly inside
   the loop makes it one, loop.c hoists it, and the dump goes to 8 moves with
   arg4 no longer among them. Measured:

     `gg = &D_80083160' outside the loop (N=4)        232 w, 131 rows
     `D_80083160.unk_00' in the loop     (N=5)        235 w,  61 rows
     `gg = &D_80083160' assigned INSIDE the loop      235 w,  61 rows
     the same through the array alias D_80083160_s    235 w,  44 rows  <-- kept

   The array alias is the project's `extern T X_s[] __asm__("X")' form (see
   csrc/ovl_dng_b09.c:27 for the same thing on D_800E2970); per the
   addressing note it makes the address its own quantity instead of letting
   the `high' die into the `lo_sum', and it is worth 17 rows here.

   THE ONE OTHER SPELLING THAT CHANGED THE WORD COUNT:
     `u = g << 16; u = u / 255;'  (two statements)    the kept form
     `u = (g << 16) / 255;'       (one statement)     233 w, 181 rows
   The two-statement form sets the user variable `u' TWICE in the loop, so
   `n_times_set[u] == 2' and loop.c's `consec_sets_invariant_p' fails on
   non-consecutive sets: nothing in the /255 magic sequence is a movable and
   it all stays in the loop, where the target has it. As one statement the
   dump gains THREE more moves (insns 84, 85, 92 -- life 6, 6 and 31), which
   pushes the threshold at arg3 down to 8 and BOTH argument addresses then
   fail, which is why the one-statement form is 181 rows and not fewer.

   ===================================================================
   WHAT IS LEFT, AND IT IS THREE ITEMS
   ===================================================================

   1. `b' IS RELOADED FOUR TIMES INSTEAD OF ONCE (+4 words, ~12 rows).
      The target reloads the spilled `b' once into $t0 and lets $t0 serve all
      four `lhu' because its multiply result pseudo took $v1
      (`mflo $v1' / `sra $v1, $v1, 8'). Here the multiply pseudo takes $t0
      (`mflo $t0' / `sra $v1, $t0, 8'), so $t0 is clobbered and `b' has to
      come back off the stack before each group. Spellings tried, all 44
      rows and no change: a `u8 *bb = (u8 *)b' local inside the loop, the
      same local before the loop, and `u16 bx/by' locals for the two fields
      (that last one is 228 words / 69 rows -- it removes two of the four
      loads outright).
   2. TWO WORDS SHORT AT THE `D_80083160' ADDRESS. The target materialises
      the full address (`lui %hi' / `addiu %lo' / `lw 0x0($t0)', 3 words) at
      BOTH in-loop sites; the direct access here folds the `%lo' into the
      load (2 words). The 3-word form is exactly what the pointer local
      `gg' gives -- and the pointer local is also what costs the fifth move,
      so THE TWO REQUIREMENTS ARE IN CONFLICT FROM C and that conflict is
      the whole remaining problem. The fifth move has to come from somewhere
      other than this address, or the address has to keep its register
      through reload with the global read directly.
   3. ONE MISSING `addu $a0, $a3, $zero' (word 75 of the target), the
      quotient copied back for the second multiply.

   THINGS TRIED FOR THE FIFTH MOVE AND MEASURED NOT TO BE ONE:
     an `s32 ww = w;' local inside the loop                 IC 175, 8 moves
     an `s32 hh = h;' local inside the loop                 IC 175, 8 moves
     `UnkDngS14LineSP *sp' declared inside the loop         IC 170, 7 moves
       (the 0x1F800000 hoist is lost instead)   235 w, 167 rows
     `sp' re-assigned at the loop top                       234 w, 168 rows
     four redundant `& 0xFFFF' masks on the x/y stores, to raise insn_count
       instead -- cse1 folds them before loop.c and the dump still says 175
   The insn_count route is the other half of the answer and is UNMEASURED
   beyond that one attempt: with N = 4 the target is equally explained by
   insn_count in [183, 221], i.e. eight to forty-six more RTL insns in the
   loop at loop.c time that combine removes again. A pointer local folded
   into a memory reference is the family to look in.

   OTHER SPELLINGS MEASURED, all against the 232-word/131-row version:
     `q->r0 = q->b0 = m0'  (chained)                        232 w, 135
     `q->b0 = q->r0 = m0'  (chained, other way)             232 w, 131
     `q->r0 = m0; q->g0 = g; q->b0 = m0;' (setRGB0 order)   232 w, 131
     `q->g0 = g; u = (q->g0 << 16) / 255;' (colour read
       back instead of the parameter)                       234 w, 149
       with both greens stored first                        233 w, 146
       -- cse1 does NOT forward the read-back, so an `lbu 0x5($s1)' survives
          in the loop and the word count is bought the wrong way
     arg3 `(s32 *)0x1F800084' / arg4 `&sp->unk_88'          231 w, 126
     arg3 `&sp->unk_84' / arg4 `(s32 *)0x1F800088'          231 w, 123
     both as integer constants                              230 w, 164
     arg4 `&sp->unk_84 + 1'                                 232 w, 131
     arg3 and arg4 each through an `s32 *' local in the
       loop                                                 232 w, 125
     arg4 alone through an `s32 *' local in the loop         237 w,  45
       -- that one DOES reach the 45-row class and the roles come out
          EXACTLY SWAPPED from the target (the block-local $s0 holds
          &sp->unk_84 and the spilled slot 0x1F800088), which is what
          prompted the threshold arithmetic above; on top of the array-alias
          body it is 53 rows and no better.
   LOAD-BEARING, with the price of the alternative: the two colour locals
   m0/m1 -- writing `(u * ramp) >> 18' twice duplicates the `lbu' and the
   `mult' and is 247 words / 115 rows; and `sp->unk_68 = sp->unk_70 = e'
   chained, which is the order the target stores 0x70 before 0x68.

   NO TWIN. dngtwin.py --unit ovl_dng_s27 reports nothing above 0.62 for
   this body and xsibs.py at --min-words 18 --ratio 0.70 finds nothing. The
   tail came from a GREP, and it is why this body is 44 rows and not 400:
   csrc/ovl_dng_s15.c's landed func_s15_800248C8 is the same
   LINE_G2-through-the-scratchpad renderer, and the UnkDngS14LineSP struct,
   the `sp->unk_18 = D_80083160.unk_00 + 0xB0' prologue, the
   `sp->unk_B4 = (sp->unk_F4 + sp->unk_F8) / 2' average and the
   `(u32)sp->unk_B4 < 0x1E0' gate are taken from it verbatim. Its own
   documented fact -- that the two INPUT addresses want `(u32)sp | 0x64' and
   not the integer constant -- is INVERTED here: this body's inputs are
   `&sp->unk_64' / `&sp->unk_D8' (plain `addiu') and the `|' form appears
   nowhere in this listing.

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
   extern Unk80083160 D_80083160_s[] __asm__("D_80083160");
   void func_80067F20(u32 *, s32, s32, s32, s32);
   extern u8 D_s27_8002588C[];
   END DECLARATIONS
*/
s32 func_s27_80024610(u8 *p, UnkVec *b, u8 *c, s16 w, s32 e, s32 *t, u8 g, s16 h)
{
    UnkDngS14LineSP *sp = (UnkDngS14LineSP *)0x1F800000;
    LINE_G2 *q;
    u8 *dm;
    s32 i;
    s32 j;
    s32 u;
    s32 m0;
    s32 m1;

    sp->unk_18 = D_80083160.unk_00 + 0xB0;
    sp->unk_68 = sp->unk_70 = e;
    for (i = 0xF; i >= 0; i--)
    {
        q = *(LINE_G2 **)(D_80083160_s->unk_00 + 0x8D0);
        *(LINE_G2 **)(D_80083160_s->unk_00 + 0x8D0) = q + 1;
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
        sp->unk_F4 = RotTransPers((SVECTOR *)&sp->unk_64, (s32 *)&sp->unk_D8, &sp->unk_84,
                                  &sp->unk_88);
        sp->unk_F8 = RotTransPers((SVECTOR *)&sp->unk_6C, (s32 *)&sp->unk_DC, &sp->unk_84,
                                  &sp->unk_88);
        q->x0 = sp->unk_D8;
        q->y0 = sp->unk_DA;
        q->x1 = sp->unk_DC;
        q->y1 = sp->unk_DE;
        sp->unk_B4 = (sp->unk_F4 + sp->unk_F8) / 2;
        if ((u32)sp->unk_B4 < 0x1E0)
        {
            addPrim(sp->unk_18 + sp->unk_B4 * 4, q);
            dm = *(u8 **)(D_80083160_s->unk_00 + 0x8D0);
            *(u32 *)(D_80083160_s->unk_00 + 0x8D0) = (u32)dm + 0xC;
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            addPrim(sp->unk_18 + sp->unk_B4 * 4, dm);
        }
    }
    return 0;
}
