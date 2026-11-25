/* func_s44_80024C0C -- ovl_dng_s44 -- PARKED at 157 words / 157 words,
   10 ALIGNED DIFFERENCES (9 by ovlverify's word-against-word count).
   dngfam: 1 member, 157 words, so this park is worth exactly 157 words.

   THE HOMING MISSILE'S PER-FRAME STEP. Bumps the +0x14 counter on the node at
   the actor's +0x0, then dispatches on +0x4. State 0 spins the slot's +0x1A by
   0x200, advances the mover on all three axes from the two angles at +0xE and
   +0x10 exactly as the landed func_s44_80024710 in this unit does (the same
   `>> 4' trig idiom and the same 0x1C / 0x1C00 scales), then recomputes the
   first of those three terms a FOURTH time as a trial step and asks whether
   taking it would move the mover AWAY from the target at the mover's +0xC: if
   the target is still ahead (sense chosen by the +0x12 flag) and the trial
   step overshoots, the state is left alone; otherwise func_s44_80024A34 fires
   the burst and the state advances. State 1 dims the slot's three tint bytes
   by 0x20 while +0xC is non-zero, grows it by 0x400 both ways, hands the
   renderable to func_800478B8 TWICE, and on 0x6000 bumps the node's +0x16 and
   retires the actor through the usual D_800814A0 pair.

   THE RESIDUE IS ONE PAIR OF LOADS IN THE WRONG ORDER, and every one of the
   ten rows is that swap or a register rename downstream of it. At the word the
   state-0 trial step is set up:

        target                          this body
     77 lw   $a1, 0x0($s3)   (cur)   77 lh   $a1, 0x12($s1)  (the +0x12 flag)
     78 lh   $v1, 0x12($s1)  (flag)  78 lw   $v1, 0x0($s3)   (cur)
     79 mflo $s0                     79 mflo $s0
     ...                             ...
     83 beqz $v1, ...                83 beqz $a1, ...
     84 addu $a2, $a1, $v0  (nxt)    84 addu $v0, $v1, $v0   (nxt)
     87 slt  $v0, $a0, $a1           87 slt  $v1, $a0, $v1
     90 slt  $v0, $a1, $a0           90 slt  $v1, $v1, $a0
     91 beqz $v0, ...                91 beqz $v1, ...
     92 subu $v1, $a1, $a0           92 subu $v1, $v1, $a0
     96 subu $v0, $a2, $a0           96 subu $v0, $v0, $a0

   So: cur lands in $a1 / flag in $v1 / nxt in $a2 / over in $v0 in the target,
   and flag in $a1 / cur in $v1 / nxt in $v0 / over in $a1 here. Every row
   after 77 follows from which of the two loads sched1 issues first. Nothing
   else in the 157 words differs -- words 0..76 and 97..156 are byte-identical
   once branch targets are masked.

   THE MECHANISM, as far as it goes. The two loads are both ready at slot 77
   (the `mult' at 76 blocks `mflo' for two cycles, so sched1 has two filler
   slots and exactly these two loads to fill them with). `rank_for_schedule'
   (sched.c) decides between them on INSN_PRIORITY, then on class against
   `last_scheduled_insn', then on INSN_LUID -- and `adjust_priority' raises a
   SINGLE-SET pseudo's birthing insn to `max_priority' (`birthing_insn_p'
   requires `REG_N_SETS == 1'). `cur' is single-set and so its `lw' IS boosted;
   the flag shares the `over' pseudo with both arms' comparison results, so its
   `lh' is three-set and is NOT boosted -- and the unboosted `lh' still wins.
   I could not close that gap from the C side. What I can state is which
   knobs do NOT move it (below), and that the fault is in the sched1 ready-list
   order and not in the statement order, the arm order, the local widths or the
   declaration order, all of which were swept.

   MEASURED AND REJECTED. All at 157 words unless stated; "10" means ten
   aligned differences, i.e. no change from the parked body.

   | spelling                                                   | words | rows |
   |------------------------------------------------------------|-------|------|
   | PARKED BODY (below)                                        |  157  |  10  |
   | `tgt' loaded ONCE before the gate, not in both arms        |  154  |  12  |
   | ... same, with the abs test as `nxt < cur'                 |  154  |  12  |
   | flag read inline in the `if', no `over' pre-read           |  158  |  22  |
   | flag in its OWN `s32 f' local (cur read first)             |  158  |  22  |
   | ... `s32 f', flag read before cur                          |  158  |  22  |
   | ... `u16 f' / `s16 f'                                      |  158  |  22  |
   | gate duplicated per arm, no `over' local at all            |  158  |  19  |
   | condition as a ternary `flag ? tgt<cur : cur<tgt'          |  158  |  15  |
   | `over = *(s16 *)(p+0x12) != 0' instead of the raw value    |  158  |  19  |
   | `nxt' inline as `cur + t - tgt' inside the abs             |  157  |   9  |
   | `nxt = t + cur' instead of `cur + t'                       |  157  |  10  |
   | flag read AFTER `cur', before `nxt'                        |  157  |  10  |
   | flag read AFTER `nxt'                                      |  157  |  10  |
   | `cur' read after `nxt' (`nxt = b->unk_00 + t')             |  157  |  10  |
   | `nxt = <product> + b->unk_00', cur cse-shared              |  159  |  57  |
   | `over' as `s16' / `u8'                                     |  158  |  19  |
   | `cur' via `*(s32 *)((u8 *)b + 0x0)' not `b->unk_00'        |  157  |  10  |
   | `tgt' via `*(s32 *)((u8 *)b + 0xC)' not `b->unk_0C'        |  157  |  10  |
   | arms written as `cur > tgt' / `tgt > cur'                  |  157  |  10  |
   | arms swapped with the test inverted to `over == 0'         |  157  |  10  |
   | nested `if (over) { if (abs..) break; }' instead of `&&'   |  157  |  10  |
   | six declaration orders of the five locals                  |  157  |  10  |
   | `tgt' declared last                                        |  157  |  10  |

   WHAT IS SETTLED, and should not be re-measured:
   * The abs test is `abs(cur - tgt) > abs(nxt - tgt)', NOT
     `abs(nxt - tgt) < abs(cur - tgt)'. The operand EVALUATION order decides
     which `subu' comes first, and the wrong way round is 12 rows.
   * `tgt' is loaded SEPARATELY IN EACH ARM of the +0x12 gate. One load before
     the gate is 154 words / 12 rows -- three words short, because the target
     really has two `lw 0xC($s3)'.
   * `nxt' is a LOCAL computed before the gate. Inline inside the abs it is
     157 / 9, but the `addu' then sinks past the gate into the join block,
     which is a second fault, not a fix: take the 10-row body as the base.
   * The fourth trig product IS recomputed; `func_800644B8' is not pure to
     gcc, so the four calls in state 0 are five `jal's in the target and five
     here.
   * State 1's two repeated values each need a LOCAL (the dimmed tint byte
     and the `+0x1E' scale), because the first store is to the SLOT and the
     re-read would be from the ACTOR -- two base registers cse cannot prove
     disjoint. Same mechanism func_s44_8002441C's comment prices at 56 rows.
   * It IS a `switch'. Two cases plus a default, and words 0..19 are
     byte-identical, so the dispatch is not in question.

   NEXT STEP. This is a permuter job and the statement to permute is narrow:
   the four statements `t = <product>;', `cur = b->unk_00;',
   `over = *(s16 *)(p + 0x12);' and `nxt = cur + t;' plus the two arms of the
   +0x12 gate, with everything outside state 0 frozen. Twenty-four hand
   spellings sit flat at 10, which is batch 34's "a one-at-a-time sweep cannot
   reach it" signature; the thing to vary ON TOP of a held change is the
   number of SETS on the `over' pseudo, since that is what decides the
   birthing-insn boost and it cannot be varied without also changing the
   word count by hand.

   DECLARATIONS
   void func_800478B8(u8 *);
   s32 func_80064584(s32);
   s32 func_800644B8(s32);
   void func_s44_80024A34(u8 *p, UnkVec *q, u8 *c);
   END DECLARATIONS
*/
void func_s44_80024C0C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 cur;
    s32 nxt;
    s32 tgt;
    s32 over;
    s32 t;

    *(s16 *)(*(u8 **)(p + 0x0) + 0x14) = *(u16 *)(*(u8 **)(p + 0x0) + 0x14) + 1;
    switch (*(s16 *)(p + 0x4))
    {
    case 0:
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x200;
        b->unk_00 += (func_800644B8(*(s16 *)(p + 0xE)) >> 4) * (func_800644B8(*(s16 *)(p + 0x10)) >> 4) * 0x1C;
        b->unk_04 += (func_800644B8(*(s16 *)(p + 0xE)) >> 4) * (func_80064584(*(s16 *)(p + 0x10)) >> 4) * 0x1C;
        b->unk_08 += (func_80064584(*(s16 *)(p + 0xE)) >> 4) * 0x1C00;
        t = (func_800644B8(*(s16 *)(p + 0xE)) >> 4) * (func_800644B8(*(s16 *)(p + 0x10)) >> 4) * 0x1C;
        cur = b->unk_00;
        over = *(s16 *)(p + 0x12);
        nxt = cur + t;
        if (over != 0)
        {
            tgt = b->unk_0C;
            over = tgt < cur;
        }
        else
        {
            tgt = b->unk_0C;
            over = cur < tgt;
        }
        if (over && abs(cur - tgt) > abs(nxt - tgt))
        {
            break;
        }
        func_s44_80024A34(p, b, c);
        *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
        break;
    case 1:
        if (c[0xC] != 0)
        {
            t = c[0xE] - 0x20;
            c[0xE] = t;
            c[0xD] = t;
            c[0xC] = t;
        }
        t = *(u16 *)(c + 0x1E) + 0x400;
        *(s16 *)(c + 0x1E) = t;
        *(s16 *)(c + 0x1C) = t;
        func_800478B8(c);
        func_800478B8(c);
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(s16 *)(*(u8 **)(p + 0x0) + 0x16) = *(u16 *)(*(u8 **)(p + 0x0) + 0x16) + 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}
