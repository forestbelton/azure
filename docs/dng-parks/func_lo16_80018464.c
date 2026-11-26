/* func_lo16_80018464 -- ovl_dng_lo16, 387 target words.

   NOW AT 6 ALIGNED ROWS, 387/387 WORDS, and the body below IS the 6-row
   version -- start from it, not from the 13-row one this header used to
   describe.  Dispatcher, 2026-11-25.

   This park was UNMEASURABLE for most of its life: parkresidue printed SKIP
   because the body uses `D_800E36C8[i].unk_02' and the unit's
   UnkDungeonSpotA had `u8 pad_02[0x2]' there.  The park's own prose said so
   -- "UnkDungeonSpotA's `u8 pad_02[0x2]' must become `u16 unk_02'" -- and the
   change had never been made.  Naming a pad at the same offset and size is
   layout-neutral and the five containers stay byte-identical, so it is done
   now, IN csrc/ovl_dng_lo16.c.  Note the unit defines its OWN
   UnkDungeonSpotA, incompatible with the one in csrc/ovl_dungeon_decls.h
   (same 12 bytes, different breakdown) and does not include that header, so
   editing the header does nothing -- patch the unit.

   WHAT TOOK IT FROM 13 TO 6, and it is ONE SHAPE APPLIED TWICE.  The target
   materialises the `q = p + 8' cursor EARLY and keeps it in $s0, reading
   `0x0($s0)', `0x4($s0)' and `$s0 + 1'; the 13-row body let gcc fold the
   cursor into its first use, read `8($a2)' instead, and then needed a
   `move a2,s0'.  Writing the cursor assignment TWICE forces it out:

       as parked                                          387 w / 13 rows
       `q = p;' before the FIRST `q = p + 8;'              387   /  8
       ... and before the SECOND one as well               387   /  6  <- kept
       first cursor assigned LAST, after the room stores   387   / 13
       the room stores read through `q - 8' etc            387   / 15
       `room = ...' before the cursor                      387   / 13
       second cursor AFTER the `D_8008146E = n' store      387   /  8
       second cursor two-set AND moved after that store    387   /  6  (same)

   I DO NOT KNOW THE MECHANISM AND THE OBVIOUS ONE IS WRONG.  My first
   reading was `birthing_insn_p' -- a single-set pseudo boosted to
   max_priority -- but `q' already has six sets in this function, so
   `reg_n_sets' was never 1 and that explanation cannot hold.  The 13-to-6 is
   measured and reproducible; the cause is open.  Do not write the
   birthing_insn_p story into anything.

   THE REMAINING 6 ROWS ARE PURE STATEMENT ORDER in one loop, at target words
   105..111.  The target emits `addiu $a1, $a1, 0x14' then `addiu $s3, $s3,
   0x1' then `lhu $v0, 0x4($s0)' then `addu $a2, $s0, $zero'; this body emits
   `move a2,s0', `addiu a1,a1,20', `lhu v0,4(a2)', `addiu s3,s3,1'.  So the
   target increments the counter BEFORE the read and copies the cursor AFTER,
   and this body does the reverse.  That is a counter-increment position
   question inside the `while (*q != 0)' scan, and it is the one axis left.

   PARKED by its own sub-agent, batch 35, 2026-11-25, at 25 aligned rows.
   THAT AGENT THEN GOT IT TO 13 AND WAS KILLED BY A WEEKLY API LIMIT BEFORE IT
   COULD REWRITE THIS HEADER.  Agent h181 has recovered the 13-row body from
   its working copy, re-measured it, and REPLACED the stale head -- the
   original head described the `p' in $v1 instead of $a2 problem AS THE WHOLE
   RESIDUE, and that problem is SOLVED.  Do not re-fight it.

   MEASURED BY h181, in sub-8464, just now:

       built 387 words, target 387 words, 13 aligned differences

   THE REMAINING RESIDUE, all thirteen rows:

       -        67 80018570                                           addiu $s0, $a2, 0x8
       X   69   70 8001857C  lbu v0,8(a2)                             lbu $v0, 0x0($s0)
       R   71   72 80018584  beqz v0,258c <func_lo16_80018464+0x1a4>  beqz $v0, .Llo16_80018608
       X   72   73 80018588  addiu s0,a2,8                            sll $a2, $s3, 6
       +   74                sll a2,s3,0x6
       -       105 80018608                                           addiu $a1, $a1, 0x14
       -       106 8001860C                                           addiu $s3, $s3, 0x1
       +  106                move a2,s0
       +  107                addiu a1,a1,20
       R  108  108 80018614  lhu v0,4(a2)                             lhu $v0, 0x4($s0)
       X  111  111 80018620  addiu s3,s3,1                            addu $a2, $s0, $zero
       X  115  115 80018630  lbu v0,8(a2)                             lbu $v0, 0x0($s0)
       X  119  119 80018640  addiu s2,a2,9                            addiu $s2, $s0, 0x1

   ONE CAUSE, and it is the FOLDING of `q = p + 8', not a schedule.  The
   target materialises `addiu $s0, $a2, 0x8' at word 67 and then reads every
   mark-cursor use through $s0 -- `lbu 0x0($s0)', `lhu 0x4($s0)',
   `addiu $s2, $s0, 0x1', `addu $a2, $s0, $zero'.  The candidate emits the
   same `addiu' but five words later, in the `beqz's DELAY SLOT, and expresses
   the uses as displacements off `p' instead: `lbu 0x8($a2)',
   `addiu $s2, $a2, 0x9'.  cse has substituted `q's known value `(plus p 8)'
   into the addresses, which costs nothing on MIPS (reg and reg+disp cost the
   same), and because the uses then keep `p' live the `addiu' has no consumer
   to be scheduled ahead of and sinks into the delay slot.  Fix the folding
   and the schedule follows; the word count is already exact either way.

   WHAT h181 MEASURED AND REJECTED on top of the agent's own list below, all
   387 words / 13 rows unless noted -- i.e. ALL FLAT, which by the batch rule
   means statement order and allocation priority are both ruled out and the
   lever is the local SET or a type:
       `q = p + 8' moved after all five room-record stores              13
       `q = p + 8' moved between the last store and `unk_0E = 0'        13
       `q = &p[8]' instead of `q = p + 8'                               13
       an extra `q2 = p + 8; q = q2' indirection                        13
       ONE cursor -- `p += 8', the inner loop on `p', no `q' at all     28
   The one-cursor spelling is the informative failure: it is much worse, so
   the target really does hold TWO pointers with a copy each way (`q = p + 8'
   at the head, `addu $a2, $s0, $zero' at the latch), and `p' is DEAD between
   them in the target -- $a2 is reused for `n << 6' at word 73.  Keeping `p'
   live is exactly what the folding does, so the two symptoms are one.

   NEXT AGENT: the question is how to stop cse substituting `(plus p 8)' into
   the five `q' addresses while keeping both pointers. Not a statement
   position and not an allocation priority, so not a permuter job either.
   A `-da' `.cse'/`.cse2' pair on both sides is what would say it.

   WHAT WAS MEASURED AND REJECTED, every figure from aligndiff on this body:

   THE THREE FIXES THAT GOT IT TO 387 WORDS, each necessary:

   1. `r2 = &D_800E2970_s[n]' AS A SECOND ROOM POINTER WRITTEN INSIDE THE
      INNER LOOP.  This is func_lo16_80017BEC's rule and it is worth 1 word
      plus the whole inner loop's register assignment: loop.c hoists the
      invariant address into the inner loop's preheader and cse2 rewrites it
      as the target's `addu $a0, $a1, $zero' at word 74.  Using `room->' for
      the inner loop too is 386 words / 56 rows with the inner loop's
      $a0/$a1/$a3/$t0 all rotated against the target's $v0/$v1/$a2/$a3.
      Bare `D_800E2970_s[n].unk_0E' subscripts inside the inner loop instead
      of a pointer local: 388 / 67.

   2. `n++' AFTER the func_8009A21C call in the D_800E3CD8 loop, not before.
      With `n++' before the call the `bne' gate at word 148 gets a nop and
      `addiu $a2, $zero, 0x20' is emitted after the join (two words where the
      target has one, reorg filling the gate's delay slot with the constant):
      388 words / 67 rows.  `n++' before the `if' is 393 / 103.  Advancing
      `q += 4' before the call and reading `q[-4]' is 388 / 83.  An inverted
      two-arm gate (`if (x != 2) {} else {...}') is 388 / 67.

   3. A SEPARATE `b' FOR THE GRID-HEADER READS, distinct from the walker `q'.
      One variable for both is 387 words / 38 rows; splitting them is 25.

   REJECTED SPELLINGS FOR THE ROOM-MARK ROW BASE, and the mechanism:
   - `room->unk_10 = (void *)((n << 6) + (s32)D_800E2C40_s)' with the three
      mark addresses written out is right ONLY once fix 1 is in place.  On its
      own it is 390 words / 56: that expression is a one-use, single-set
      DEST_REG giv of the outer biv `n' with benefit 4, loop.c hoists it out
      of the inner loop (`move $t0, $a2') and strength-reduces it into a
      third cursor (`addiu $a2, $a2, 64'), two words long.  `D_800E2C40_s[n]'
      as a pointer is the same 390 / 56.
   - routing all four addresses through ONE multiply-set `UnkDngMark2C40 *m'
      kills the giv (n_times_set > 1 is not a movable) and is exactly 387
      words -- but it merges the unk_10 value and the mark address into one
      allocno, where the target has two ($v1 and $v0): 387 / 56 before fix 1,
      387 / 63 with it.  With fix 1 present the inner loop is large enough
      (move_movables' `threshold * savings * lifetime >= insn_count' no longer
      holds at insn_count 38) that the plain single-set spelling is safe, so
      `m' is NOT needed and must not be used.
   - `&D_800E2C40_s[r][n]' and the flat `&D_800E2C40_s[r * 0x10 + n]' were
      already priced on func_lo16_800177D8; the integer form is kept.

   REJECTED SPELLINGS FOR THE CELL WRITE IN THE D_800E3CD8 LOOP:
   - writing the address expression out again for `unk_06' costs 9 words: the
      store through the cell invalidates cse's record of q[0], q[1] and
      g->unk_14, so all three are reloaded.  A local `UnkDungeonCell *c' for
      the decrement and the read-back, with the expression written out ONLY
      for the `unk_00 = 1' arm (which the target does recompute), is correct.

   REJECTED SPELLINGS FOR THE PROLOGUE AND THE CURSORS:
   - `D_80081480 = D_8008148C' BEFORE `b = (u8 *)D_8008148C' costs a word
      (`move $s0, $v0'): 388 / 37.  After the grid setup: 385 / 44.
   - `p = (u8 *)D_8008148C + 4' as a second read: 388 / 30.
   - `p = b + 4' before the calls: 388 / 28 (it is then live across them).
   - the grid setup before the calls: 387 / 68.
   - `p = b + 4' moved to five different source positions (first after the
      calls, between the two halves of the grid setup, last before `n = 0',
      after `n = 0'): 387 / 25 EVERY TIME, byte-identical output.
   - `b' declared `u16 *' with `b[0]'/`b[1]' and `p = (u8 *)(b + 2)': 25.
   - `p' declared as a 4-halfword struct pointer so the record reads carry
      MEM_IN_STRUCT_P: 25.
   - the two shift amounts held in `s16 sx'/`s16 sy' locals: 25.
   - the room loop written as an explicit `if (...) do { } while (...)' with
      the bottom test on `q' instead of `p': 387 / 25, byte-identical -- gcc
      normalises it to the same RTL as the `while'.
   - `p = q + 2; q = p;' at the latch instead of `q += 2; p = q;': 25.

   FACTS WORTH KEEPING THAT ARE NOT LEVERS HERE:
   - the stream is a byte walk with THREE cursors: `b' (the 4-byte header),
      `p' (the 8-byte room record, read as four u16 at 0/2/4/6 with the
      +4 halfword as the terminator) and `q' (everything else).  Each room is
      8 bytes + 4-byte marks + a 2-byte terminator; after the room loop the
      next section starts at `p + 8'.
   - D_8001F604 IS A REAL RELOCATED SYMBOL, not a constant address, even
      though it is a 0x8001xxxx address out of dungeon.bin_undefined_syms_auto:
      the target pairs `lui 0x8002' with `addiu -0x9FC', and gas' li macro for
      a CONST_INT emits lui/ori.  Same for D_lo16_8001F586, whose store is
      `lui $v0' + `sh $v1, %lo($v0)' with no `lui $at' macro.  Declared as
      externs the candidate's words are byte-identical after relocation.
   - `D_8001F604[(s16)(r - 1)]' needs the inner `(s16)' cast: that is the
      target's `sll 16'/`sra 14' pair.  Without it the index is a plain
      `sll 2'.
   - func_800A7A38 is `u32 func_800A7A38(u8 *)' from csrc/ovl_dng_b30.c,
      func_8004068C is `u8 *func_8004068C(u8 *, u8 *)' from csrc/unk_12DEC.c,
      func_800BCA68 is `s16 func_800BCA68(u16, u16)' from
      csrc/ovl_dungeon_800B6AC4.c.  func_8003E4FC and D_8008148C/D_80081480/
      D_8008333C are already in include/structs.h.
   - jtcheck reports 0 tables for this function, so the rodata hazard in the
      brief does not apply.

   THIS BODY ALSO NEEDS ONE TYPEDEF FIELD CHANGE, layout-identical:
   UnkDungeonSpotA's `u8 pad_02[0x2]' must become `u16 unk_02' (the final
   loop stores func_800BCA68's result to +0x04 and then +0x02).  Nothing else
   in the unit reads pad_02, and all 49 landed bodies still verify at
   0 differing with the change in place.

   DECLARATIONS
   extern u8 *D_8001F604[];
   extern s16 D_lo16_8001F586;
   extern u32 D_800E3D6C;
   extern void *D_800DF258[];
   u32 func_800A7A38(u8 *);
   u8 *func_8004068C(u8 *, u8 *);
   s16 func_800BCA68(u16, u16);
   s32 func_lo16_800199C8(s16);
   END DECLARATIONS
*/

void func_lo16_80018464(s16 r)
{
    UnkDngLo16Grid *g;
    UnkDngRoom2970 *room;
    UnkDngRoom2970 *r2;
    UnkDungeonCell *c;
    u8 *p;
    u8 *q;
    u8 *b;
    s32 cb;
    s32 n;
    s32 i;

    b = (u8 *)D_8008148C;
    D_80081480 = D_8008148C;
    cb = (s32)D_8008333C[0];
    g = (UnkDngLo16Grid *)D_8008333C;
    func_8003E4FC(6, D_8001F604[(s16)(r - 1)], 0);
    func_8003F320();
    p = b + 4;
    g->unk_14 = *(u16 *)b;
    g->unk_18 = (1 << g->unk_14) - 1;
    g->unk_16 = *(u16 *)(b + 2);
    g->unk_1C = 0x40 << g->unk_14;
    g->unk_1A = (1 << g->unk_16) - 1;
    g->unk_1E = 0x40 << g->unk_16;
    n = 0;
    while (*(u16 *)(p + 4) != 0)
    {
        q = p;
        q = p + 8;
        room = &D_800E2970_s[n];
        room->unk_0A = 1;
        room->unk_00 = *(u16 *)p;
        room->unk_02 = *(u16 *)(p + 2);
        room->unk_04 = *(u16 *)(p + 4);
        room->unk_06 = *(u16 *)(p + 6);
        room->unk_0E = 0;
        while (*q != 0)
        {
            r2 = &D_800E2970_s[n];
            r2->unk_10 = (void *)((n << 6) + (s32)D_800E2C40_s);
            ((UnkDngMark2C40 *)(r2->unk_0E * 4 + (n << 6) + (s32)D_800E2C40_s))->unk_00 = *q++;
            ((UnkDngMark2C40 *)(r2->unk_0E * 4 + (n << 6) + (s32)D_800E2C40_s))->unk_01 = *q++;
            ((UnkDngMark2C40 *)(r2->unk_0E * 4 + (n << 6) + (s32)D_800E2C40_s))->unk_02 = *q;
            q += 2;
            r2->unk_0E++;
        }
        q += 2;
        p = q;
        n++;
    }
    q = p;
    q = p + 8;
    D_8008146E = n;
    n = 0;
    while (*q != 0)
    {
        D_800E3CD8[n].unk_00 = q[2] + 2;
        D_800E3CD8[n].unk_02 = q[0];
        D_800E3CD8[n].unk_04 = q[1];
        c = (UnkDungeonCell *)((q[0] + (q[1] << g->unk_14)) * 6 + cb);
        c->unk_02 -= 0x20;
        D_800E3CD8[n].unk_06 = c->unk_02;
        if (D_800E3CD8[n].unk_00 == 2)
        {
            ((UnkDungeonCell *)((q[0] + (q[1] << g->unk_14)) * 6 + cb))->unk_00 = 1;
        }
        func_8009A21C(q[0], q[1], 0x20);
        n++;
        q += 4;
    }
    if (func_lo16_800199C8((s16)n) != 0)
    {
        n++;
    }
    D_lo16_8001F586 = 1;
    n++;
    for (; n < 4; n++)
    {
        D_800E3CD8[n].unk_00 = 0;
    }
    q += 2;
    n = 0;
    while (*q != 0)
    {
        if (*q == 0x63)
        {
            D_800E3D6C |= 2;
            q += 6;
            break;
        }
        D_800E36C8[n].unk_00 = *q++;
        D_800E36C8[n].unk_01 = *q++;
        D_800E3548_s[n].unk_00 = *q++;
        D_800E3548_s[n].unk_01 = *q++;
        D_800E3548_s[n].unk_03 = *q++;
        D_800E3548_s[n].unk_02 = *q++;
        D_800E36C8[n].unk_08 = func_800A7A38((u8 *)&D_800E3548_s[n]);
        n++;
    }
    for (; n < 0x40; n++)
    {
        D_800E3548_s[n].unk_01 = 0;
        D_800E3548_s[n].unk_00 = 0;
    }
    q += 2;
    n = 0;
    while (*q != 0)
    {
        if (*q == 0x63)
        {
            D_800E3D6C |= 8;
            q += 6;
            break;
        }
        D_800E39C8[n].unk_06 = *q++;
        D_800E39C8[n].unk_07 = *q++;
        D_800E3648_s[n].unk_00 = *q++;
        D_800E3648_s[n].unk_01 = *q++;
        D_800E3648_s[n].unk_03 = *q++;
        D_800E3648_s[n].unk_02 = *q++;
        D_800E39C8[n].unk_08 = (s32)D_800DF258[D_800E3648_s[n].unk_00];
        func_8009A21C(D_800E39C8[n].unk_06, D_800E39C8[n].unk_07, 0x20);
        n++;
    }
    q += 2;
    while (*q != 0)
    {
        D_800E296C |= 0x10000000;
        D_800E39C8[n].unk_06 = *q++;
        D_800E39C8[n].unk_07 = *q++;
        D_800E3648_s[n].unk_00 = *q++;
        D_800E3648_s[n].unk_01 = *q++;
        D_800E3648_s[n].unk_03 = *q++;
        D_800E3648_s[n].unk_02 = *q++;
        n++;
    }
    q += 2;
    for (; n < 0x20; n++)
    {
        D_800E3648_s[n].unk_01 = 0;
        D_800E3648_s[n].unk_00 = 0;
    }
    func_8004068C(q, (u8 *)D_800EA000_s);
    for (i = 0; D_800E3548_s[i].unk_01 != 0; i++)
    {
        D_800E36C8[i].unk_04 = func_800BCA68(D_800E36C8[i].unk_00 << 6, D_800E36C8[i].unk_01 << 6);
        D_800E36C8[i].unk_02 = D_800E36C8[i].unk_04;
    }
}
