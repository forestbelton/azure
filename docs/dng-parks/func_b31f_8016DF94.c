/* PARK: func_b31f_8016DF94 (ovl_dng_b31f) -- 99 words / 99 words /
   15 ALIGNED DIFFERENCES.  Agent h173, batch 35.  1 member (one-member
   family, no replication).  OWNS NO JUMP TABLE (jtcheck: the unit's three
   tables belong to 8016B778, 8016E528 and 801730AC).

   WHAT IT DOES: the bank's re-entry/arming routine.  Put 1 in the parked
   record's +0xD2, zero the three halfwords at 0x8001371A/18/16, spend one
   of D_80083460's +0xA counter, set bits 0 and 3 and clear bit 4 of the
   0x80013714 word, publish a D_b31f_801748C8 row at 0x8001371C, raise
   D_800DCF4D, take the parked actor's cell through func_800A0818 into the
   payload's +0x2A, publish a D_b31f_801749E0 row into D_b31f_80174CCC with
   D_b31f_80174CD0 cleared, then walk the engine's two +0xAC actor slots,
   handing each live one to func_b31f_8016A908 and func_8009A028 and raising
   0x80000000 in its node's +0x10.

   WORD COUNT IS EXACT AND THE STATEMENT SET IS SETTLED.  Rows 1..6 and
   rows 35..99 of the stream MATCH EXACTLY, including the whole loop (its
   three reloads of D_800E3D7C, the `addu' operand order, the `sra 14'
   index and the 0x80000000 `or').  All 15 rows live between stream
   positions 7 and 34, and they are TWO INSNS SCHEDULED LATE plus their
   knock-on register renaming:

     target 8:  lui $s2, %hi(D_800E3D7C)      candidate has it at 31
     target 19: lw $s0, %lo(D_b31f_80174704)  candidate has it at 33

   Everything else in those rows is the $v0/$v1 exchange in the
   D_b31f_801748C8 index chain that follows from the two displacements.
   The target puts both of these address/load insns at the TOP of the
   pre-loop block; the candidate puts them twelve insns from its end.
   gcc 2.7.2's sched1 is BOTTOM-UP and `rank_for_schedule' ties on
   INSN_LUID with the LOWEST luid picked FIRST, so an insn with no in-block
   dependent and a HIGH luid is placed at the TOP of the block.  The
   target's `lui %hi(D_800E3D7C)' therefore has a luid near the END of the
   pre-sched stream -- i.e. it is loop.c's hoist out of the walk, emitted
   just before the loop -- while the candidate's sits mid-stream.  I could
   not find the source shape that gives it that luid.

   MEASURED AND REJECTED -- 24 spellings, ALL of them 15 or worse, and the
   sweep is FLAT at 15 across four independent axes, which is the signature
   the brief describes for a fault that none of the axes contains.

   A. STATEMENT ORDER (the g-block, the record store, the slot read, the
      D_800DCF4D raise).  The kept order is the floor:
      | order                                                   | aligned |
      | D2, Z, H5, RMW, TBL, RQ, CF  (as below)                 |   15    |
      | ... TBL, CF, RQ   (the D_800DCF4D raise before the slot) |   22    |
      | ... RMW, RQ, TBL, CF                                     |   22    |
      | ... H5, RQ, RMW, TBL, CF                                 |   20    |
      | ... Z, RQ, H5, RMW, TBL, CF                              |   27    |
      | ... D2, RQ, Z, ...                                       |   35    |
      | RQ first, before the +0xD2 store                         |   42 (98 words) |
      | TBL before the 0x3714 read-modify-write                  |   38    |
      | D_80083460h[5] first                                     |   51    |
      | D_80083460h[5] after the 0x3714 RMW                      |   35    |
      | D_80083460h[5] last                                      |   40    |
      | the +0xD2 store after the three zero stores              |   27    |
      | the +0xD2 store after the table publish                  |   25    |
      | the +0xD2 store between the zero stores and the RMW      |   50    |
      `r' and `q' SPLIT to different positions (they share one `lw' of the
      symbol, so they need not be adjacent):
      | r after H5, q after TBL                                  |   20    |
      | r after Z,  q after TBL                                  |   27    |
      | r after RMW, q after TBL                                 |   22    |
      | r after TBL, q last                                      |   15    |
      | q second, r after TBL                                    |   15    |
      | r after H5, q last                                       |   20    |

   B. THE TABLE ROW'S ADDRESSING.  `t = &D_b31f_801748C8[...]' then `*t'
      (the pointer-value hoist) is 15; the MEM form
      `*(s32 *)(g + 0x371C) = D_b31f_801748C8[...]' is 40.  This REPRODUCES
      func_b31f_8016DC8C's fact 2 in this file (18 rows there) at a
      different price, so it is settled: the pointer-value hoist is right.

   C. LOOP SHAPE AND LOCALS.  `for (i = 0; i < 2; i++)' 15;
      `i = 0; do { ... i++; } while (i < 2)' 15; `i != 2' as the test 17;
      the gate's pointer through a local `e' reused for the first call 17.
      `i' MUST BE `s16' -- the loop's `sll 16' / `sra 14' index and the
      `sll 16'/`sra 16'/`slti 2' test are the narrow form.

   D. ALLOCATION PRIORITY IS RULED OUT.  Three permutations of the local
      DECLARATION order (`i' first, `i' last, `e' last) all measure 15
      exactly, which is the brief's test for this.  Also free at 15: the
      index shifted as `(u16)m >> 9' instead of `m >> 9'.  Worse: the
      0x3714 read-modify-write as `(x & ~0x10) | 9' instead of
      `(x | 9) & ~0x10' is 17 (the listing's `ori' precedes its `andi').
      The D_80083460 access through a POINTER LOCAL instead of the
      `D_80083460h[5]' array reference is 15 either way, at the top of the
      body or in place -- NOT MEASURABLE here, so this body is no evidence
      about that lever.

   WHAT I WOULD TRY NEXT: the candidate's own `-da' `.loop' and `.sched'
   dumps, to see whether its `%hi(D_800E3D7C)' is loop.c's hoist at all --
   if it is not, the question is what stops `scan_loop' moving it, and that
   is a question about the loop body's insn set rather than about any
   statement order out here.

   THE CONSTANT BASE.  This body addresses the 0x8001xxxx block through
   `g = (u8 *)0x80010000' with five displacements off one `lui', which is
   the spelling func_b31f_8016DC8C measured in this unit (a struct alias is
   16/5, a declared array 16/5, five declared symbols 19/13, five raw
   constants 21/21).  THAT MEANS A MASKED `0 differing' WOULD NOT BE
   SUFFICIENT FOR THIS BODY: ovlverify's mask ignores the displacement
   field, so the four store displacements (0x371A, 0x3718, 0x3716, 0x3714)
   and the 0x371C publish must be read against the listing by eye.  They
   are, below, and they are written DESCENDING as 8016DC8C requires.

   DECLARATIONS
   extern u16 D_800834E2;
   extern s32 D_b31f_801748C8[];
   extern u8 *D_b31f_801749E0[];
   extern u16 D_80083460h[] __asm__("D_80083460");
   extern u8 *D_b31f_80174704;
   extern u8 *D_b31f_80174CCC;
   extern s16 D_b31f_80174CD0;
   extern u8 *D_800E3D7C;
   extern u8 D_80082E80[];
   s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
   void func_8009A028(u8 *);
   void func_b31f_8016A908(u8 *p);
   END DECLARATIONS */
void func_b31f_8016DF94(void)
{
    u8 *g;
    u8 *q;
    u8 *r;
    u8 *e;
    s32 *t;
    u16 m;
    u16 x;
    s16 i;

    g = (u8 *)0x80010000;
    D_b31f_80174704[0xD2] = 1;
    *(s16 *)(g + 0x371A) = 0;
    *(s16 *)(g + 0x3718) = 0;
    *(s16 *)(g + 0x3716) = 0;
    D_80083460h[5] -= 1;
    *(u16 *)(g + 0x3714) = (*(u16 *)(g + 0x3714) | 9) & ~0x10;
    t = &D_b31f_801748C8[(D_800834E2 >> 9) & 7];
    *(s32 *)(g + 0x371C) = *t;
    r = *(u8 **)(D_b31f_80174704 + 0xC);
    q = D_b31f_80174704 + 0x20;
    D_800DCF4D = -1;
    m = func_800A0818(r[0x24], r[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
    *(s16 *)(q + 0x2A) = m;
    D_b31f_80174CD0 = 0;
    D_b31f_80174CCC = D_b31f_801749E0[(m >> 9) & 7];
    for (i = 0; i < 2; i++)
    {
        if (*(u8 **)(D_800E3D7C + i * 4 + 0xAC) != NULL)
        {
            func_b31f_8016A908(*(u8 **)(D_800E3D7C + i * 4 + 0xAC));
            func_8009A028(*(u8 **)(D_800E3D7C + i * 4 + 0xAC));
            e = *(u8 **)(D_800E3D7C + i * 4 + 0xAC) - 0x20;
            *(s32 *)(e + 0x10) |= 0x80000000;
        }
    }
}
