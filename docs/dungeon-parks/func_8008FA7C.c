/* PARK: func_8008FA7C, 180 words against a target of 181, 31 aligned
   differences.  ovl_dungeon, 2026-11-16, agent j80 (after j41).

   j80's result in one line: THE BODY BELOW PRODUCES THE TARGET'S EXACT
   INSTRUCTION STREAM, with a single two-insn pair in the wrong place and
   one load-delay nop missing because of it.  There is no longer a wrong
   instruction, a wrong operand or a wrong register anywhere else in the
   function.  Candidate words 0..56 and 91..179 are byte-identical; the
   whole residue lives in words 57..90.

   WHAT CHANGED FROM j41's PARK (182 words / 24 rows):
   only the spelling of state 1's angle load, from
       *(s16 *)(d + 0x2A)        (inside the call argument)
   to
       ((s16 *)d)[0x15]
   That one array ref is worth EIGHT instructions' worth of ordering and
   the register of `s`.  Mechanism, which is the main thing this run
   established:

     `sched.c`'s `anti_dependence`/`true_dependence` (:860, :880) have an
     escape clause BEFORE `memrefs_conflict_p` ever decides:

        a MEM_IN_STRUCT reference at a VARYING, non-QImode, non-AND address
        never conflicts with a non-MEM_IN_STRUCT reference at a FIXED address

     `D_800E3540 = s;` is a MEM at a SYMBOL_REF -- fixed, not in a struct.
     `*(s16 *)(d + 0x2A)` is a plain INDIRECT_REF -- varying, NOT in a
     struct -- so the clause does NOT apply and the two CONFLICT.  The
     store is the last statement before the call, so source order pins it
     BEFORE the angle load, and the target has it AFTER
     (`lh $a3, 0x2A($s1)` at 88, `sw $t0, %lo(D_800E3540)` at 90).
     `((s16 *)d)[0x15]` is an ARRAY_REF, so MEM_IN_STRUCT_P is set, the
     clause applies, the edge disappears and sched1 is free to float the
     load above the store -- which it does, exactly as the target does.
     This is the same mechanism as j41's lever 3 (`((u32 *)d)[7] &=` must
     be an array ref), read in the other direction.

   That one change fixes, all at once:
     * `s` moves from $a2 to $t0           (`lw $t0, %lo(D_80081484)`)
     * the angle moves into $a3            (`lh $a3, 0x2A($s1)`)
     * `lui/sw %hi/%lo(D_800E3540)` land after the angle load
     * `addiu $a3, $zero, 0x1` returns to the SHARED tail, so
       `.L8008FD1C` is `jal` + `addiu $a3,1` and not `jal` + `nop`
   i.e. it removes j41's extra word -- and then re-introduces a different
   one-word error, below.

   ------------------------------------------- WHAT IS LEFT: TWO INSNS

   Candidate 62..90 against target 62..91, instruction for instruction:

       cand                      target
       62 lui  v0,%hi(83460)     62 lw   v0,0x124(s0)
       63 addiu a3,v0,%lo        63 addu a0,s2,zero
       64 lw   v1,0x124(s0)      64 sh   a2,0x8A(v0)
       65 li   v0,0x78           65 li   v0,0x78
       66 sh   a2,0x8A(v1)       66 lw   v1,0x124(s0)
       67 lw   v1,0x124(s0)      67 nop
       68 addu a0,s2,zero        68 sb   v0,0x84(v1)
       ... identical from here except  73 lui  v1,%hi(83460)
       ...                             77 addiu v1,v1,%lo
       (everything else in 69..90 matches 69..91 exactly)

   So `&D_80083460`'s `lui`/`addiu` sit at the TOP of state 1's block in
   $a3; the target has them at words 73/77 in $v1.  Because they sit at the
   top they are not needed as delay-slot filler, `addu $a0,$s2,$zero`
   becomes the filler for word 67 instead, and the target's nop there is
   the missing 181st word.

   THE CAUSAL CHAIN, measured, not guessed (dumps in `sched.py --sched1`,
   `.lreg`, `.greg`):

     1. `g` is a pseudo with TWO sets (state 1 and state 3), so
        `birthing_insn_p` (sched.c, `reg_n_sets == 1`) never bumps its
        `lo_sum` insn and it keeps priority 1 -- the minimum.
     2. In sched1's backward scan the priority-1 group at T-24 is
        { `a0 = c`, g's `lo_sum`, the 0xFFEFFFFF `ori` }.  `schedule_select`
        breaks a priority tie on `potential_hazard` and then on ready-list
        order, which `rank_for_schedule` sorts by DESCENDING `INSN_LUID`.
        `a0 = c` is the call's argument setup, so its LUID is larger than
        the `lo_sum`'s -- it wins, and the `lo_sum` is pushed all the way
        down to T-35, i.e. to the top of the block.
        With j41's body the group at T-25 was only { `lo_sum`, `ori` } --
        `a0 = c` had already drained at T-20 -- so the `lo_sum` won there
        and landed at the right place.  The ONE cycle of difference is
        traced below.
     3. Scheduled at the top of the block, `g` is now live across the three
        `lw $v1, 0x124($s0)` reloads, which local-alloc has put in $v1.
        `.greg` prints `84 conflicts: 80 82 83 84 90 2 3 4 5 6 8 29` --
        the conflict with hard reg 3 is exactly those reloads -- so
        `find_free_reg` takes the first free register, $a3 (7).  In the
        target `g` is born at word 77, AFTER the last reload dies at 71,
        so it does not conflict with 3 and gets $v1.
     4. sched2 then can't help: with `g` in $a3 the `lui` has no
        predecessor at all in the block, priority 1, so it goes to the
        block head.  With `g` in $v1 the `lui` carries an anti-dependence
        on `sb $v0, 0x85($v1)` and cannot precede it.

   WHY THE ONE CYCLE (this is the thing to attack):
   in j41's body the E3540 store depends on nothing after it except the
   call, but the ANGLE LOAD depends on the store, so sched1's chain is
   `sw 0x2C(c)` -> `sw D_800E3540` -> `lh 0x2A` and the store is not ready
   until T-11.  T-9 therefore has nothing of priority > 1 ready and
   `a2 = 5` drains there; `a0 = c` drains at T-20.  Remove the edge (any
   spelling that puts the load first) and the store becomes ready at T-4
   with priority 8, wins T-9, blocks the following `lh` for one cycle on
   the memory unit, and `sw 0x2C(c)` lands at T-14 instead of T-15 -- every
   later insn shifts one cycle and `a0 = c` is still ready at T-24.
   The fix wanted is ONE more occupied cycle in sched1's T-4..T-20 window,
   or anything that drains `a0 = c` before the g `lo_sum` becomes ready at
   T-21.

   MEASURED AND REJECTED BY j80 (all on top of this body unless noted; the
   number is built words / aligned rows, target 181):

   -- ways of getting the angle load above the E3540 store --
     * `a = *(s16 *)(d + 0x2A);` hoisted to a local, index uses `a`:
                                                      180 / 33
     * `a = D_80083228 + *(s16 *)(d + 0x2A);`          180 / 31 (same
       instruction stream as this body, same g residue)
     * `a = (... + 0x100) >> 9;`                       180 / 31
     * `a = D_80083228 + ... + 0x100;`                 180 / 31
     * `a` declared `s16` instead of `s32`:            182 / 41
     * `a` assigned TWICE (also feeding func_8009AF18) 180 / 36
     * hoisting the whole `t1[...]` byte into a local: 182 / 41
     * `*(u8 *)(t1 + i)` instead of `t1[i]`:           184 / 26..28
   -- `&D_80083460` spellings --
     * separate local for state 1 (`g1`) + array ref:  182 / 32
       (g1's pair DOES land at 69/76, close to the target's 73/77, but it
        is then a LOCAL quantity, takes $a1, pushes the 0xFFEFFFFF mask to
        $a3 and `r` to $a1, and the mask's `lui`/`ori` become adjacent so
        the two `lw`s at 58/60 get nops: that is the 182.)
     * separate local, four declaration orders:        182 / 32..35
       (declaration order does not move anything)
     * `g = (u8 *)&D_80083460 + 0xA; *(s16 *)g += 1;`  181 / 29
       RIGHT LENGTH, but it is the same shape as "inline" below: `r` in
       $a1, the mask in $a3, nops at 59/61, words 73..85 reordered.  ~12
       genuinely different instructions, so WORSE than this body even
       though the count matches.
     * `&D_80083460` inline in state 1 (no local):     181 / 31  (same
       shape as the row above)
     * `((s16 *)g)[5] += 1;`                           180 / 31
     * `*(u16 *)(g + 0xA) += 1;`                       180 / 31
     * `*(s16 *)(g+0xA) = *(s16 *)(g+0xA) + 1;`        180 / 31
   -- dependence-structure probes, all 180 / 31..35, none moved the g pair --
     * `*(u32 *)(q + 0x1C) |= 0x200000;` (non-array)   180 / 31
     * `((void **)D_80081484)[0]` for both accesses    180 / 31
     * D_80081484 pair before `((u32 *)d)[7] &=`       180 / 32
     * `*(void **)&D_800E3540 = s;`                    180 / 31
     * `((u8 **)c)[0xB] = t1;`                         180 / 35
     * `D_800E3540 = s;` before the +0x2C store        182 / 31
     * `((u32 *)d)[7] &= 0xFFEFFFFF;`                  180 / 31
     * `t1 = D_800DCFD0;` moved to five earlier points, each  180 / 31
       (sched1 normalises all five to the same schedule)

   STILL TRUE FROM j41's PARK, DO NOT DISTURB:
     * state 3 must store the table in BOTH arms, not assign a local and
       store once (157..167 words if you don't);
     * each state needs its OWN table-pointer local (t0/t1/t2);
     * `((u32 *)d)[7] &= ~0x100000` must be the array ref
       (`*(u32 *)(d + 0x1C) &=` is 184 / 57);
     * `s16 func_8009AF18_s(s16, UnkVec *, u8 *, s16) __asm__("func_8009AF18");`
       is required -- the real prototype's `u16` first parameter gives
       `lhu` where the target has `lh`.

DECLARATIONS
extern u8 D_800DCFD0[];
extern u8 D_800DD060[];
s16 func_8009AF18_s(s16, UnkVec *, u8 *, s16) __asm__("func_8009AF18");
END DECLARATIONS
*/

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
            func_80048A44(c, t1[((D_80083228 + ((s16 *)d)[0x15] + 0x100) >> 9) & 7], 5, 1);
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
