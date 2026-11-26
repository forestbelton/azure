/* PARK: func_s17_800259D8, csrc/ovl_dng_s17.c, agent-h217, 2026-11-26.

   TARGET 791 words. BEST BUILT 791 WORDS, 25 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py), and ovlverify agrees at 25 differing -- the
   word count is EXACT and every differing word lies inside ONE 24-word
   region, the two arms of case 1's distance/step divide at
   .Ls17_80025D40 and .Ls17_80025D70. The other 767 words are byte-exact.
   1 member (tools/decomp/dngfam.py) -- no replication value.

   VERDICT: a sched1 ORDERING fault inside two sibling basic blocks. Not
   ALLOC: permuting the declaration order five ways gives 791/25 every time,
   which by the levers file rules allocation priority out. Not SHAPE: the
   word count is exact and no opcode, immediate or displacement differs
   outside those two blocks.

   OWNS jtbl_s17_80024078 -- 8 words, 8 cases + 0 pad, minval 0 (the dispatch
   is a bare sltiu $v1, 0x8 off `lh 0xA($s4)' with no subtraction), 8
   distinct targets, and the range-check beqz targets the function exit so
   there is NO default: arm. The ROM arm labels are at strictly ascending
   addresses (80025AD0 < 80025B50 < 80026018 < 800261D8 < 8002634C <
   80026498 < 800264B4 < 8002659C), so entry-address order IS ascending case
   order and the arms are written 0..7, with case 0 FALLING THROUGH into
   case 1. VERIFIED by standalone compile: the emitted table is
   [$L138, $L139, $L156, $L164, $L168, $L178, $L180, $L184] at strictly
   ascending .s line numbers (2497 < 2530 < 2913 < 3065 < 3192 < 3314 <
   3326 < 3407), so the substitution is 1:1 and the resolved sequence is the
   ROM's. 8 words standalone, 8 in the ROM.

   THE TWIN IS func_s20_80024064 in csrc/ovl_dng_s20.c, LANDED at 488 words
   with a nine-row priced fact table. It is the same actor in another bank:
   same `o = *(u8 **)p; n = (UnkNode *)(o - 0x20); m = (u8 *)n->unk_08;'
   head, the same switch on *(s16 *)(p + 0xA), the same case 0 fall-through
   into case 1, the same func_8003DF74 gate with the nested 0x8000 retest,
   the same `(**(u16 **)(p + 0x4) & 0x80)' guard, the same func_8004491C /
   D_80045340 dress block, the same `*(u8 **)(o + 0x60)' pair of arms with
   D_800DDC40 and the D_8006CCD8/D_8006CCE8 byte pair, and the same
   D_8008346C / p-0x2 / D_800814A0 retirement. EIGHT of its facts carried
   here unchanged and are not re-measured; they are listed under CARRIED.

   WHAT IT DOES. The eight-state driver of a summoned actor. State 0 paints
   the slot grey, sets its 0x1000 scale pair, copies the 12-byte
   D_s17_80026634 record onto the payload at +0xA2 and publishes it at
   s->unk_08, then latches the owner's facing (bits 9..11 of r[0x2A]) at
   +0x80 and falls through. State 1 asks func_8003DF74 for the spawn offset,
   seeds the mover from the owner, and then -- if the owner has a target at
   r+0x60 -- aims at it, reading the per-facing step from the 0x20-byte
   D_s17_80024058 table copied onto the stack, or else aims straight ahead;
   it ends by spawning eight 0x12 nodes driven by func_s17_80025814 and
   drawn by func_s17_80024704. State 2 integrates the aim over the remaining
   +0x7C frames, retiring to state 7 when the target goes away or
   func_800A4778 says the tile is blocked and to state 3 when the count runs
   out. State 3 spawns the single 0x12 node that func_s17_80025470 steps and
   func_s17_80024FD4 draws. State 4 fades the slot over 30 frames by
   (0x1E - n) * 0x80 / 0x1E and (0x1E - n) * 0xE0 / 0x1E while spawning four
   0xE02020 particles a frame. State 5 waits on +0x88. State 6 raises
   0x10000000 on the target's +0x1C, pulses its colour for 0x47 frames and
   then calls func_s17_80024098. State 7 counts 0xF frames and clears
   D_8008346C, the node redraw bit and D_800814A0.

   THE REMAINING FAULT, named and priced but not fixed. Case 1 computes
   `*(s16 *)(p + 0x7C) = abs(dv) / abs(step)' in two arms selected by
   `*(s8 *)(r + 0x72) == e[0x24]'. In both arms the target schedules the
   STEP TABLE READ (lh/lhu off the stack copy) BEFORE the __builtin_abs
   insn; this body schedules the abs first in the FALL-THROUGH arm and the
   table read after it. The two arms then differ by one instruction each in
   a way that cancels in the word count: the fall-through arm gains the
   load-delay nop after its second `lh' that the target fills with the
   index load, and the branch-target arm loses its `addiu $sp, 0x28' to the
   `beq' delay slot, which reorg steals because that addiu is the first insn
   of the thread and a MEM is not speculatively movable where an addiu is.
   The mechanism is sched1's backward tie-break: the abs insn and the table
   load both have zero dependents and equal priority, so rank_for_schedule
   falls through to INSN_LUID, and the target wants the abs to carry the
   HIGHER luid. It does not: 11 source spellings of the table read and 3
   statement orders in each arm all leave the RTL order unchanged after
   combine. Reading .combine with -da confirms the RTL order does change
   with the source order while the .sched output does not, so the decision
   is not luid-driven after all and the real discriminator is still unknown.
   That is the next thing to attack, and it is ONE scheduling decision.

   MEASURED AND REJECTED, all against this 791/25 body (aligndiff):

   the branch-target arm, table read FIRST, then abs    791 words, 25  KEPT
     ... abs first, then the table read                 792 words, 23
     ... the difference split out and abs applied later 792 words, 24
   the fall-through arm's table read spelled
     ((s16 *)t)[k * 2]                                  791 words, 25  KEPT
     ... (s16)t[k].unk_00                               791 words, 25
     ... *(s16 *)&t[k].unk_00                           791 words, 25
     ... *(s16 *)((u8 *)t + k * 4)                      791 words, 25
     ... *(s16 *)(t + k)                                791 words, 25
     all four NOT MEASURABLE -- identical residue, same rows
   the fall-through arm's statement order, 3 ways       791 words, 25
     NOT MEASURABLE -- identical residue
   declaration order of dd/ee/i/f, 5 permutations       791 words, 25
     NOT MEASURABLE -- allocation priority is therefore OUT
   the divide and the store duplicated into both arms   790 words, 31
   abs(dv) and abs(step) both hoisted to the divide     787 words, 34
   the step read as a plain u16 with no sign extension  792 words, 36

   MEASURED AND LOAD-BEARING, the facts that took this body from 779/206 to
   791/25. Each is priced against the body as it stood when it was found, so
   re-price before building on one:

   1. THE 0x20-BYTE STACK COPY NEEDS A u8 * SOURCE LOCAL. memcpy from the
      `const u8 D_s17_80024058[]' symbol directly emits an ALIGNED lw/sw
      run; `sr = (u8 *)D_s17_80024058' forces get_pointer_alignment to 1 and
      restores the target's lwl/lwr/swl/swr. This is csrc/ovl_dng_b05.c's
      func_b05_801733BC fact (1) at 0x20 bytes instead of 8. Worth 12 rows.
   2. THE STEP TABLE IS AN ARRAY OF 4-BYTE STRUCTS, NOT A FLAT ARRAY. All
      four read sites want the base address &t[0] MATERIALISED with the
      field offset as the MEM displacement; a flat `u8 t[0x20]' with
      `*(u16 *)(t + k * 4 + 2)' reassociates the constant and gives the
      frame base plus a 0xA displacement instead, and a flat
      `u16 t[0x10]' with array refs folds the whole base into the MEM. The
      COMPONENT_REF path runs through get_inner_reference, whose base
      address cannot stay a three-term sum, so memory_address forces it into
      a register. Worth 39 rows across the four sites.
   3. CASE 2 IS FOUR GATES AND THE ARM ORDER OF EVERY ONE IS OBSERVABLE.
      The 7-state retirement block is written TWICE -- once for the
      func_800A4778 arm placed FIRST, once at the end of the
      count-exhausted arm -- and jump.c cross-joins them from the
      `sh $v0, 0xA($s4)' onward, keeping the EARLIER copy and leaving the
      first arm's `addiu $v0, 7' in the branch delay slot. Written once at
      the end of the case the block lands after the integration block
      instead of before it, 7 rows. The count gate must be
      `if ((*(s16 *)(p + 0x7C) -= 1) <= 0) { ... } integration;' so the
      integration is the fall-out and the bgtz jumps forward to it.
   4. CASE 7 STORES THE OLD COUNTER BACK THROUGH AN ARRAY REF.
      `((s16 *)p)[0x42] = f;' ahead of the D_80026664 test lets sched1 hoist
      the fixed-address `lh %lo(D_80026664)' ABOVE the p-relative store --
      MEM_IN_STRUCT_P at a varying address against a plain cast at a symbol
      address, true_dependence (sched.c:817), the same lever the twin
      prices at 4 rows for its +0x16 facing read. With the store duplicated
      into both arms as a plain cast it is 792 words / 42: the store never
      reaches the branch delay slot. Worth 1 word and 11 rows.
   5. CASE 6 READS *(u8 **)(r + 0x60) ONCE PER GROUP, AND THE -0x14 READ
      MUST COME FIRST. A store through one pointer kills cse's entry for a
      load through another, so `e = *(u8 **)(*(u8 **)(r + 0x60) - 0x14);'
      has to be written BEFORE the |= and the &= on +0x1C or the r+0x60
      load is repeated. Worth 4 words and 18 rows over the two groups.
   6. CASE 3'S +0x3C STORE IS WRITTEN THIRD, not last. The value is
      `*(u8 **)(r + 0x60)' and the load cannot be scheduled above the ten
      stores through q, so it has to be loaded before them; sched then
      sinks the store itself to the end. Worth 3 rows.
   7. THE CASE 1 and CASE 2 GATE POLARITIES: the out[] offsets are the
      fall-through and the -0x40 arm the branch target; the
      `*(s8 *)(r + 0x72) != e[0x24]' arm is the fall-through; case 6's
      `>= 0x24' arm is the fall-through. Written the other way round the
      three cost 206, 110 and 23 rows respectively, every row a moved block.

   CARRIED FROM THE TWIN WITHOUT RE-MEASURING, so do not read these as
   levers for this body: the `o/n/m' head triple, `(((u16 *)r)[0x15] >> 9) &
   0x7' as an ARRAY REF so the lhu hoists above the `D_80026664 = 1' store,
   the nested-if form of the func_8003DF74 gate, the three descending
   `c[0xE]/c[0xD]/c[0xC]' byte stores as separate statements, the
   `e[0x24] + *(u8 *)&D_8006CCD8[k]' byte lookup off the s16 array, the
   second inline `*(u8 **)(o + 0x60)' for the D_800DDC40 index, the
   `*(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1' bump spelling, and the
   three-argument alias for func_s17_80024098.

   PROTOTYPE NOTE, and it is the twin's as well: func_s17_80024098 is
   called with THREE arguments here while its landed definition higher in
   this unit takes two, so the unit needs a three-argument alias for that
   one site exactly as csrc/ovl_dng_s20.c carries func_s20_800262B8_3.
   The landed func_s17_80024098 is NOT changed.

   relcand ok (12 data symbols, 8 calls); declcheck s17 clean.

   DECLARATIONS
   s32 rand(void);
   extern u8 D_s17_80026634[];
   extern u8 D_80026640[];
   extern u8 D_8002664C[];
   extern u8 D_80045340[];
   extern u8 D_800DDC40[];
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern s32 D_8008346C;
   UnkNode *func_8003FC64(s32);
   u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
   s32 func_800A4778(u16, u16, s16, s32);
   s32 func_800A56E0(s32);
   void func_s17_80024098_3(u8 *, u8, u8 *) __asm__("func_s17_80024098");
   typedef struct UnkDngS17Step
   {
       u16 unk_00;
       u16 unk_02;
   } UnkDngS17Step;
   END DECLARATIONS
*/
void func_s17_800259D8(u8 *p, UnkVec *v, u8 *c)
{
    u16 out[3];
    UnkDngS17Step t[8];
    UnkNode *n;
    UnkNode *n2;
    u8 *r;
    u8 *b;
    u8 *m;
    u8 *e;
    u8 *q;
    u8 *u;
    u8 *mv;
    s32 i;
    s32 dd;
    s32 ee;
    s32 f;
    u8 *sr;

    r = *(u8 **)p;
    sr = (u8 *)D_s17_80024058;
    memcpy(t, sr, 0x20);
    n = (UnkNode *)(r - 0x20);
    b = (u8 *)n->unk_08;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)(c + 0xC) = 0x808080;
        *(s16 *)(c + 0x1E) = 0x1000;
        *(s16 *)(c + 0x1C) = 0x1000;
        memcpy(p + 0xA2, D_s17_80026634, 0xC);
        *(u32 *)(c + 0x8) = (u32)(p + 0xA2);
        D_80026664 = 1;
        *(s16 *)(p + 0x80) = (((u16 *)r)[0x15] >> 9) & 0x7;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fallthrough */
    case 1:
        if (func_8003DF74((u8 *)((UnkSlot *)n->unk_0C)->unk_08, (UnkSlot *)n->unk_0C, (s16 *)out,
                          0) == NULL)
        {
            if ((((UnkSlot *)n->unk_0C)->unk_14 & 0x8000) == 0)
            {
                break;
            }
        }
        *(s16 *)((u8 *)v + 0x2) = *(u16 *)(b + 0x2);
        *(s16 *)((u8 *)v + 0x6) = *(u16 *)(b + 0x6);
        *(s16 *)((u8 *)v + 0xA) = *(u16 *)(b + 0xA);
        if ((((UnkSlot *)n->unk_0C)->unk_14 & 0x8000) == 0)
        {
            *(s16 *)((u8 *)v + 0x2) += out[0];
            *(s16 *)((u8 *)v + 0x6) += out[1];
            *(s16 *)((u8 *)v + 0xA) += out[2];
        }
        else
        {
            *(s16 *)((u8 *)v + 0xA) -= 0x40;
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
            *(u16 *)(c + 0x14) |= 0xC;
            p[0x7A] |= 0x4;
        }
        if (*(u8 **)(r + 0x60) != NULL)
        {
            m = *(u8 **)(*(u8 **)(r + 0x60) - 0x18);
            *(s16 *)(p + 0x74) = *(u16 *)(m + 0x2);
            *(s16 *)(p + 0x76) = *(u16 *)(m + 0x6);
            *(s16 *)(p + 0x78) = *(u16 *)(m + 0xA) - D_800DDC40[(*(u8 **)(r + 0x60))[0x13]] - 0x60;
            e = *(u8 **)(r - 0x14);
            p[0xAE] = e[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x80)];
            p[0xAF] = e[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x80)];
            m = *(u8 **)(*(u8 **)(r + 0x60) - 0x18);
            if (*(s8 *)(r + 0x72) != e[0x24])
            {
                ee = ((s16 *)t)[*(s16 *)(p + 0x80) * 2];
                dd = __builtin_abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)(m + 0x2));
            }
            else
            {
                ee = (s16)t[*(s16 *)(p + 0x80)].unk_02;
                dd = __builtin_abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)(m + 0x6));
            }
            *(s16 *)(p + 0x7C) = dd / __builtin_abs(ee);
        }
        else
        {
            *(s16 *)(p + 0x7C) = 0x80;
            *(s16 *)(p + 0x74) =
                *(u16 *)((u8 *)v + 0x2) + (t[*(s16 *)(p + 0x80)].unk_00 << 7);
            *(s16 *)(p + 0x76) = *(u16 *)((u8 *)v + 0x6) +
                                 t[*(s16 *)(p + 0x80)].unk_02 * *(u16 *)(p + 0x7C);
            *(s16 *)(p + 0x78) = *(u16 *)(r + 0x88) - 0x50;
        }
        *(s32 *)((u8 *)v + 0xC) = (s16)t[*(s16 *)(p + 0x80)].unk_00 << 16;
        *(s32 *)((u8 *)v + 0x10) = t[*(s16 *)(p + 0x80)].unk_02 << 16;
        *(s32 *)((u8 *)v + 0x14) = ((*(s16 *)(p + 0x78) << 16) - v->unk_08) / *(s16 *)(p + 0x7C);
        *(s16 *)(p + 0x84) = 0;
        *(s16 *)(p + 0x9C) = 0x37;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        for (i = 0; i < 8; i++)
        {
            n2 = func_8003FC64(0x12);
            if (n2 != NULL)
            {
                q = (u8 *)n2 + 0x20;
                *(u32 *)(q + 0x38) = (u32)r;
                *(u32 *)(q + 0x44) = (u32)v;
                *(u32 *)(q + 0x48) = (u32)c;
                *(u32 *)(q + 0x4C) = (u32)p;
                n2->unk_10 = (u32)func_s17_80025814;
                func_8004491C(n2, (u32)func_s17_80024704);
                u = (u8 *)n2->unk_0C;
                *(s16 *)(u + 0x10) = 0x20;
                *(u16 *)(u + 0x14) |= 0xC;
                mv = (u8 *)n2->unk_08;
                *(s16 *)(q + 0xA) = (rand() & 0x7) + 0xC;
                *(s16 *)(q + 0xC) = rand() & 0xFFF;
                *(s16 *)(u + 0x16) = (rand() & 0xFF) << 4;
                *(s16 *)(u + 0x18) = (rand() & 0xFF) << 4;
                *(s16 *)(u + 0x1A) = (rand() & 0xFF) << 4;
                *(s32 *)(mv + 0x0) = *(s16 *)(q + 0xA) * func_80064584(*(s16 *)(q + 0xC)) << 4;
                *(s32 *)(mv + 0x4) = *(s16 *)(q + 0xA) * func_800644B8(*(s16 *)(q + 0xC)) << 4;
                *(s16 *)(mv + 0xA) = *(u16 *)((u8 *)v + 0xA);
                u = (u8 *)n2->unk_0C;
                *(s16 *)(u + 0x1E) = 0x1000;
                *(s16 *)(u + 0x1C) = 0x1000;
                u[0xE] = 0x80;
                u[0xD] = 0x80;
                u[0xC] = 0x80;
                memcpy((u8 *)n2 + 0x4C, D_80026640, 0xC);
                *(u32 *)(u + 0x8) = (u32)((u8 *)n2 + 0x4C);
            }
        }
        break;
    case 2:
        if (*(s16 *)(p + 0xA0) == 0)
        {
            *(s16 *)(p + 0xA0) = 1;
            p[0xAA] += 0x18;
        }
        else
        {
            *(s16 *)(p + 0xA0) = 0;
            p[0xAA] -= 0x18;
        }
        if ((s16)func_800A4778(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                               *(s16 *)((u8 *)v + 0xA), (s32)*(u8 **)(r + 0x60)) != 0)
        {
        *(s16 *)(p + 0xA) = 7;
        *(s16 *)(p + 0x84) = 0;
        c[0xE] = 0;
        c[0xD] = 0;
        c[0xC] = 0;
        break;
        }
        {
            if (*(u8 **)(r + 0x60) == NULL)
            {
                c[0xC] -= 4;
                c[0xD] -= 4;
                c[0xE] -= 4;
                if (c[0xC] == 0)
                {
                    *(s16 *)(p + 0x7C) = 1;
                }
            }
            if ((*(s16 *)(p + 0x7C) -= 1) <= 0)
            {
                if (*(u8 **)(r + 0x60) != NULL)
                {
                    *(s16 *)(p + 0xA) = 3;
                    *(s16 *)(p + 0x84) = 0;
                    m = *(u8 **)(*(u8 **)(r + 0x60) - 0x18);
                    *(s16 *)((u8 *)v + 0x2) = *(u16 *)(m + 0x2);
                    *(s16 *)((u8 *)v + 0x6) = *(u16 *)(m + 0x6);
                    *(s16 *)((u8 *)v + 0xA) = *(u16 *)(p + 0x78);
                    func_800A56E0(0x300);
                    break;
                }
                *(s16 *)(p + 0xA) = 7;
                *(s16 *)(p + 0x84) = 0;
                c[0xE] = 0;
                c[0xD] = 0;
                c[0xC] = 0;
                break;
            }
            *(s32 *)((u8 *)v + 0xC) = ((*(s16 *)(p + 0x74) << 16) - v->unk_00) / *(s16 *)(p + 0x7C);
            *(s32 *)((u8 *)v + 0x10) =
                ((*(s16 *)(p + 0x76) << 16) - v->unk_04) / *(s16 *)(p + 0x7C);
            *(s32 *)((u8 *)v + 0x14) =
                ((*(s16 *)(p + 0x78) << 16) - v->unk_08) / *(s16 *)(p + 0x7C);
            v->unk_00 += *(s32 *)((u8 *)v + 0xC);
            v->unk_04 += *(s32 *)((u8 *)v + 0x10);
            v->unk_08 += *(s32 *)((u8 *)v + 0x14);
            break;
        }
    case 3:
        if (*(s16 *)(p + 0xA0) == 0)
        {
            *(s16 *)(p + 0xA0) = 1;
            p[0xAA] += 0x18;
        }
        else
        {
            *(s16 *)(p + 0xA0) = 0;
            p[0xAA] -= 0x18;
        }
        n2 = func_8003FC64(0x12);
        if (n2 != NULL)
        {
            q = (u8 *)n2 + 0x20;
            *(s16 *)(q + 0x2) = 0;
            *(u32 *)(q + 0x38) = (u32)r;
            *(u32 *)(q + 0x3C) = (u32)*(u8 **)(r + 0x60);
            *(s16 *)(q + 0xA) = 0xC;
            *(s16 *)(q + 0xC) = 0x8;
            *(u32 *)(q + 0x40) = (u32)p;
            *(s16 *)(q + 0xE) = 0;
            *(s16 *)(q + 0x10) = 0;
            *(s16 *)(q + 0x12) = 0;
            *(s16 *)(q + 0x1C) = 0;
            *(s16 *)(q + 0x1A) = 0x40;
            m = *(u8 **)(*(u8 **)(r + 0x60) - 0x18);
            n2->unk_10 = (u32)func_s17_80025470;
            func_8004491C(n2, (u32)func_s17_80024FD4);
            u = (u8 *)n2->unk_0C;
            *(s16 *)(u + 0x10) = 0x20;
            *(u16 *)(u + 0x14) |= 0xC;
            mv = (u8 *)n2->unk_08;
            *(s32 *)(mv + 0x0) = *(s32 *)(m + 0x0);
            *(s32 *)(mv + 0x4) = *(s32 *)(m + 0x4);
            *(s32 *)(mv + 0x8) =
                *(s32 *)(m + 0x8) - (D_800DDC40[(*(u8 **)(r + 0x60))[0x13]] << 15);
            u = (u8 *)n2->unk_0C;
            *(s16 *)(u + 0x1E) = 0x1000;
            *(s16 *)(u + 0x1C) = 0x1000;
            u[0xE] = 0x80;
            u[0xD] = 0x80;
            u[0xC] = 0x80;
            memcpy((u8 *)n2 + 0x4C, D_8002664C, 0xC);
            *(u32 *)(u + 0x8) = (u32)((u8 *)n2 + 0x4C);
        }
        *(s16 *)(p + 0xA) = 4;
        *(s16 *)(p + 0x84) = 0;
        break;
    case 4:
        *(s16 *)(p + 0x84) = *(u16 *)(p + 0x84) + 1;
        if (*(s16 *)(p + 0x84) < 0x1E)
        {
            c[0xC] = (0x1E - *(s16 *)(p + 0x84)) * 0x80 / 0x1E;
            c[0xD] = (0x1E - *(s16 *)(p + 0x84)) * 0x80 / 0x1E;
            c[0xE] = (0x1E - *(s16 *)(p + 0x84)) * 0xE0 / 0x1E;
            if (*(s16 *)(p + 0xA0) == 0)
            {
                *(s16 *)(p + 0xA0) = 1;
                p[0xAA] += 0x18;
            }
            else
            {
                *(s16 *)(p + 0xA0) = 0;
                p[0xAA] -= 0x18;
            }
            for (i = 0; i < 4; i++)
            {
                func_s17_80025338((UnkNode *)(p - 0x20), *(s16 *)(p + 0x80), 0xE02020,
                                  (rand() & 0xFF) | 0x80, 0, 0, 0);
            }
        }
        else
        {
            *(s16 *)(p + 0x84) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            *(u16 *)(c + 0x14) |= 0x80;
        }
        break;
    case 5:
        if (*(s16 *)(p + 0x88) != 0)
        {
            *(s16 *)(p + 0x84) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 6:
        *(s16 *)(p + 0x84) = *(u16 *)(p + 0x84) + 1;
        e = *(u8 **)(*(u8 **)(r + 0x60) - 0x14);
        *(s32 *)(*(u8 **)(r + 0x60) + 0x1C) |= 0x10000000;
        if (*(s16 *)(p + 0x84) >= 0x24)
        {
            e[0xC] += 2;
            e[0xD] += 2;
            e[0xE] -= 3;
        }
        else
        {
            e[0xC] -= 2;
            e[0xD] -= 2;
            e[0xE] += 3;
        }
        if (*(s16 *)(p + 0x84) >= 0x47)
        {
            e = *(u8 **)(*(u8 **)(r + 0x60) - 0x14);
            *(s32 *)(*(u8 **)(r + 0x60) + 0x1C) &= 0xEFFFFFFF;
            e[0xE] = 0x80;
            e[0xD] = 0x80;
            e[0xC] = 0x80;
            func_s17_80024098_3(*(u8 **)(r + 0x60), p[0x9], r);
            *(s16 *)(p + 0x84) = 0xD;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 7:
        f = *(u16 *)(p + 0x84);
        *(s16 *)(p + 0x84) = f + 1;
        if ((s16)f >= 0xF)
        {
            ((s16 *)p)[0x42] = f;
            if (D_80026664 == 0)
            {
                D_8008346C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            else
            {
                D_80026664 = 0;
            }
        }
        break;
    }
}
