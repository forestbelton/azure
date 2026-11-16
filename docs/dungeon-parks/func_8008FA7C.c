/* PARK: func_8008FA7C, 182 words against a target of 181, 46 differing
   (masked), 24 aligned differences.  ovl_dungeon, 2026-11-16, agent j41.
   The reading is complete and every arm but state 1 is byte-exact; the
   residue is one word and the scheduling of state 1's tail.

   TWO NEW DECLARATIONS, plus one alias:

       extern u8 D_800DCFD0[];
       extern u8 D_800DD060[];

   and a prototype for func_8009AF18, which is DEFINED LOWER IN THE FILE with
   a `u16` first parameter.  The call here passes a signed angle and the
   target's load is `lh $a0, 0x2A($s1)`; the real prototype makes it `lhu`.
   So the call needs the unit's alias spelling:

       s16 func_8009AF18_s(s16, UnkVec *, u8 *, s16) __asm__("func_8009AF18");

   ------------------------------------------------------ WHAT IT IS

   The same +0x9B state machine as the landed func_8008E504 (:3816), four
   states, each ending in `*(u8 **)(c + 0x2C) = <table>` plus
   `func_80048A44(c, table[dir], n, 1)` and `p[0x9B]++`, where
   `dir = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7`.  State 0
   parks 0xFFEA0000 in the vector's +0x14; state 1 runs the +0x96 countdown,
   probes with func_8009AF18 and, on a nonzero answer, raises the parked
   +0x124 record's 0x200000, writes the answer to its +0x8A, sets its
   +0x84/+0x85 to 0x78/2, drops d's 0x100000, moves D_80081484 into
   D_800E3540 and bumps D_80083460+0xA; state 2 gates on +0xA2's 0x10 and
   d's 0x100000; state 3 gates on D_80083460+0xA, sets its +0x2 0x412, tears
   down d's +0x5C and installs func_8008EAC8 or func_8008ACDC.

   ------------------------------------------- THE THREE LEVERS THAT MATTERED

   1. **STATE 3 MUST STORE THE TABLE IN BOTH ARMS, NOT ASSIGN A LOCAL AND
      STORE ONCE.**  This is the whole shape of the function.  Writing
      `t = A; ... t = B; ... *(u8 **)(c + 0x2C) = t; f(c, t[dir], ...)` gives
      ONE store and no reload, and then cross-jumping folds state 0's and
      state 2's entire thirteen-instruction index blocks into state 3's:
      157-167 words of 181.  Writing the store inside each arm makes
      cross-jumping merge the two stores (that is what `.L8008FCE8` is) and
      leaves the later `(*(u8 **)(c + 0x2C))[dir]` as a REAL reload --
      `lw $v1, 0x2C($a0)` -- because cse cannot forward a value that two
      predecessors store differently.  That reload is the one instruction
      that makes state 3's last insn `lbu $a1, 0x0($v1)` instead of
      `lbu $a1, 0x0($v0)`, and per docs/matching-levers.md's "A SHORT
      CROSS-JUMP MATCH FOUND FIRST BLOCKS A LONGER ONE" that mismatch caps
      find_cross_jump's first call at the `jal`, so the loop that would
      compare state 0 against state 2 never runs.  165 -> 182 words,
      59 -> 30 rows.

   2. **EACH STATE NEEDS ITS OWN TABLE POINTER LOCAL** (t0/t1/t2 here).  One
      shared local is one global-alloc quantity in one hard register, and two
      arms whose index lands in the same register cross-jump: 148 words.

   3. **`((u32 *)d)[7] &= ~0x100000` MUST BE AN ARRAY REF**, the rule
      func_8008E504's own comment records.  `*(u32 *)(d + 0x1C) &=` is
      184 words / 57 rows.  `((u32 *)q)[7] |= 0x200000` is free either way.

   -------------------------------------------------- WHAT IS LEFT: ONE WORD

   All 24 rows are in state 1, and they are one scheduling decision.  The
   target fills the load-delay slot after `lh $a3, 0x2A($s1)` with
   `lui $v1, %hi(D_800E3540)`, which leaves `addiu $a3, $zero, 0x1` as the
   last insn before the shared `jal func_80048A44` -- so cross-jumping takes
   BOTH into the shared tail at `.L8008FD1C` and reorg puts the `addiu` in
   the jal's delay slot.  We schedule `li $a3, 1` into that load-delay slot
   instead, the shared tail is the `jal` alone, and its delay slot is a `nop`:
   that nop is the extra word.  `D_800E3540 = s;` is already the last
   statement before the call; the scheduler still issues its `lui` ahead of
   the D_80083228 load.

   MEASURED AND REJECTED (all on top of the body below):
     * a named index local `v` in state 1:            180 words / 31 rows
     * `D_800E3540 = s;` before the +0x2C store:      182 / 31
     * `D_800E3540 = s;` before `t1 = D_800DCFD0;`:   182 / 31
     * table store before the +0xA bump:              182 / 29
     * a separate `g1` local for state 1:             182 / 35
     * `&D_80083460` written inline in state 1:       181 / 44
     * `*(u32 *)(d + 0x1C) &=` instead of the array:  184 / 57
     * state 3 with its own `t3` local and one store: 157 / 65
     * state 3 storing through `((u8 **)c)[0xB]`:     165 / 59 */

void func_8008FA7C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *q;
    u8 *t0;
    u8 *t1;
    u8 *t2;
    void *s;
    s16 r;

    switch (p[0x9B])
    {
    case 0:
        b->unk_14 = (s32)0xFFEA0000;
        t0 = D_800DD0C8;
        *(u8 **)(c + 0x2C) = t0;
        func_80048A44(c, t0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        p[0x9B]++;
        break;
    case 1:
        if (--*(s16 *)(p + 0x96) > 0)
        {
            break;
        }
        r = func_8009AF18_s(*(s16 *)(d + 0x2A), b, c, 8);
        if (r != 0)
        {
            q = *(u8 **)(p + 0x124);
            ((u32 *)q)[7] |= 0x200000;
            *(s16 *)(*(u8 **)(p + 0x124) + 0x8A) = r;
            (*(u8 **)(p + 0x124))[0x84] = 0x78;
            (*(u8 **)(p + 0x124))[0x85] = 2;
            ((u32 *)d)[7] &= ~0x100000;
            s = *(void **)D_80081484;
            *(void **)D_80081484 = NULL;
            g = (u8 *)&D_80083460;
            *(s16 *)(g + 0xA) += 1;
            t1 = D_800DCFD0;
            *(u8 **)(c + 0x2C) = t1;
            D_800E3540 = s;
            func_80048A44(c, t1[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 5, 1);
        }
        p[0x9B]++;
        break;
    case 2:
        if ((*(u16 *)(p + 0xA2) & 0x10) == 0)
        {
            break;
        }
        b->unk_14 = 0;
        if ((*(u32 *)(d + 0x1C) & 0x100000) == 0)
        {
            t2 = D_800DD060;
            *(u8 **)(c + 0x2C) = t2;
            func_80048A44(c, t2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        }
        p[0x9B]++;
        break;
    case 3:
        g = (u8 *)&D_80083460;
        if (*(s16 *)(g + 0xA) != 0)
        {
            break;
        }
        *(u16 *)(g + 0x2) |= 0x412;
        func_80099F70(*(u8 **)(d + 0x5C));
        func_80099F04(*(u8 **)(d + 0x5C));
        if ((*(u32 *)(d + 0x1C) & 0x100000) != 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_8008EAC8;
            *(u8 **)(c + 0x2C) = D_800DD0B8;
        }
        else
        {
            *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
            *(u8 **)(c + 0x2C) = D_800DCFB0;
        }
        func_80048A44(c, (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        p[0x9B]++;
        break;
    }
}
