/* PARK: func_800B8FC8, 95 of 95 words, 19 aligned differences.

   ====================================== 2026-11-21: 12 rows -> 5

   TWO DEVICES ON THE SAME STATEMENT, found by two different sweeps:

       do
       {
           g++;
           g--;
           func_800B9144((u8 *)w, p, r, (s16)ee);
       } while (0);

   tools/decomp/devsweep.py found the `do { } while (0);` (12 -> 7) and
   tools/decomp/refsweep.py, re-run on THAT body, found the cancelled pair
   inside it (7 -> 5).  On the park's own body the pair alone is worth 2
   (12 -> 10) at a different statement entirely, and the two sweeps had to be
   run in that order to compose: the wrapper changes which name is contested
   at that point, and the pair then pays for it.  Neither sweep finds the pair
   it wants without the other's result in hand.
   ovl_dungeon.  **95 of 95 words, 16 ALIGNED DIFFERENCES** (2026-11-20).
   Measured 2026-11-16 and called PROVED UNREACHABLE at 19 by j70; that
   verdict is corrected below and the body is now at 16.  The
   five-argument sibling of func_800B835C -- `nearsibs.py` scores the pair at
   0.891 and that is an understatement: the two are the same function with a
   different middle callee and one more parameter.  **Read
   docs/dungeon-parks/func_800B835C.c first**; every lever in its header
   applies here and was carried over rather than re-derived.

   THE SIGNATURE IS NOW PINNED.  The park used to say "no declaration needed
   and no call site in this unit".  Both are stale: csrc/ovl_dungeon.c now
   declares it at line ~386 and calls it from func_800B9A1C's family at
   line ~27952, `func_800B8FC8((u8 *)e, c, (s32 *)w, 0, 0)`, which agrees
   with

       void func_800B8FC8(u8 *p, s32 *b, s32 *b2, s32 c, s32 e)

   so the parameter list below must not be narrowed or reordered.

   It calls func_800B9144, which landed on 2026-11-16 and is declared in the
   unit; the first argument is the `s16 w[2]` pair cast to `u8 *`.

   Note the two SEPARATE record pointers, which is what makes this function
   longer than its sibling: `b2` feeds the `w[2]` pair handed to
   func_800B9144, and `b` feeds the 0x60000000 rectangle and the tail's
   SetDrawArea.  func_800B835C uses one record for both.

   ------------------------------------------------------------------ residue

   A THREE-WAY REGISTER ROTATION AND NOTHING ELSE.  Every instruction,
   immediate and displacement matches; `ovlverify` says 95/95.  The
   `%hi(D_80083160)` pseudo is in $s5 here and $s4 in the target; `b` is in
   $s4 here and $s6 there; `e` is in $s6 here and $s5 there.  The 19 rows are
   that rotation plus the prologue save/copy interleaving that follows from
   it.

   ------------------------------------ the allocator numbers, read not guessed

   `tools/decomp/dump.sh` on a one-function file (the unit's declaration
   block, lines 1..2284, plus this body).  The twelve globals and their
   `allocno_compare` priorities, floor_log2(refs)*refs/live_length*10000:

     93 4/5  =16000 v0   90 4/6 =13333 v0   86 8/25=9600 s2   88 7/16=8750 a1
     87 5/48= 2083 s3   92 5/51= 1960 s4   94 5/74=1351 s5   117 3/24=1250 s0
     91 2/27=  740 s6   83 2/50=  400 s7   80 2/60= 333 fp    85 2/66= 303 s1

   and `;; 12 regs to allocate: 93 90 86 88 87 92 94 117 91 83 80 85`
   reproduces that order exactly, so the model is confirmed, not assumed.
   92 is `b`, 94 is the %hi pseudo, 91 is `e`, 83 is `c`, 80 is `p`, 117 is
   `lean`, 85 is the `&D_80083160` pointer.  Every one of the twelve conflicts
   with hard 16 and 17 except 85, so the pool is handed out in the fixed order
   $s2 $s3 $s4 $s5 $s6 $s7 $fp, with $s0 taken out of turn by 117's
   `;; 117 preferences: 16` and $s1 left for 85.

   THE TARGET NEEDS THE ORDER %hi > e > b > c > p.  We have b > %hi > e, and
   c > p is already right.

   ------------------------------------------- why no source spelling reaches it

   Three measured facts, and together they close the door.

   (1) %hi > b needs live_length(%hi) < live_length(b), both at 5 refs.
       live_length(%hi) is 68..74 in every spelling tried.  live_length(b) is
       110 when `b` is used directly, and 51 the moment a surviving body copy
       `bb = b;` exists -- because combine folds the parameter insn into the
       copy, so b's def moves INSIDE sched1's region and sinks.  So %hi > b
       requires NO surviving copy of b.

   (2) c > p needs live_length(c) < live_length(p), both at 2 refs.  With the
       parameter copies left alone that is 64 against 54 and it never
       inverts: p's only use is the `$a1` move of the func_800B9144 call and
       c's is the `andi` AFTER that call, while p's def precedes c's.  The
       only measured way to invert it is to let sched1 SINK c's parameter
       copy, which gives 50 against 60.

   (3) sched.c:3250-3276 decides whether a parameter copy is sinkable, and it
       is a RUN, not a per-insn test:

           if (reload_completed == 0 && b == 0)
             { ...skip notes up to NOTE_INSN_FUNCTION_BEG...
               while (head != tail && GET_CODE (head) == INSN
                      && GET_CODE (PATTERN (head)) == SET)
                 { src = SET_SRC (PATTERN (head));
                   ...strip SUBREG, SIGN_EXTEND, ZERO_EXTEND,
                      SIGN_EXTRACT and ZERO_EXTRACT...
                   if (GET_CODE (src) != REG
                       || REGNO (src) >= FIRST_PSEUDO_REGISTER)
                     break;
                   INSN_REF_COUNT (head) = 1;   -- never scheduled
                   head = NEXT_INSN (head); } }

       The run is p, b, b2, c in source order and it stops at the first insn
       that is not a plain set from a hard register.  `sched.py --sched1`
       confirms it: with `b` copied the block prints priorities for insns 8
       and 10 (b2 and c) and base measures c=50 < p=60; with `b` used
       directly it prints none for 4, 6, 8, 10 and c=64 > p=54.

   So c sinks only if an EARLIER member of the run disappears, and the only
   one that can is b's -- which is exactly what (1) forbids.  Measured, the
   other two cannot be removed:

     * b2's copy cannot be folded into a body copy: `cb = b2;` with both
       reads through `cb` is byte-for-byte the no-copy compilation (same
       pseudo numbers, same lengths), because both b2 uses are in block 0 and
       cse canonicalises them straight back onto the parameter.  A copy only
       survives when its uses are out of cse's reach -- `bb = b;` survives
       because b's surviving use is in the TAIL join block, and `ee = e;`
       survives because its source is a MEM that a call invalidates.
       `pp = p;` and `cc = c;` do not survive, and neither creates an allocno.
     * b2's copy cannot be folded into its first use either: two calls
       (SetDrawArea, AddPrim) clobber $a2 between the copy and the first
       `lhu`.  Proved by making b2 single-use (`t = 7;`): the copy stays and
       c is still 64.
     * a parameter copy is NOT deleted merely by being dead.  With b2 unused
       entirely the copy survives to sched1 and c is still 64 (`sched.py
       --sched1` lists no priority for insn 10).

   (1) and (2) are therefore mutually exclusive on this 95-instruction shape
   FOR THE SPELLINGS ABOVE.

   ============================== THE "PROVED" VERDICT WAS WRONG -- 2026-11-20

   **This park said "19 rows is the floor; this is PROVED, not exhausted".
   The live body below is at 16.**  The argument above is sound about what
   source spellings can do to the two live lengths; it says nothing about a
   FREE REFERENCE, and `grep -c __asm__` on this park was 0.

   One reference on `b2` -- `__asm__("" ::"r"(b2));` immediately after
   `ee = e;` -- is 95 words / 16 rows.  flow_analysis runs ONCE
   (toplev.c:3463) and BEFORE combine (:3488), and flow.c:2128 weights each
   reference by loop_depth, so a reference that costs no instruction still
   moves `allocno_compare`.  Seven positions in the first block all reach 16
   (`ee = e;`, `bb = b;`, `g = ...`, `lean = ...`, `q = ...`, `r = ...`, and
   the `*(u32 *)(... + 0x8D0) = ...` store); from `SetDrawArea` onward it is
   19 again.  Swept: 10 variables x 32 insertion points = 320 measurements,
   then a greedy second round with both the input-only and the
   matched-operand asm forms over 10 variables -- no second device improves
   on 16.

   The residue at 16 is still the same family -- the `%hi(D_80083160)`
   pseudo, `b` and `e` rotating through $s4/$s5/$s6 -- but three fewer rows
   of it, and the allocno table moves accordingly: 94 (the %hi) goes
   5/76 = 1315 and 92 (`b`) 5/50 = 2000, with `;; 12 regs to allocate` still
   printing the same order.

   **THE ASM IS A LAST RESORT AND THE PLAIN-C ALTERNATIVES DO NOT REACH 16
   HERE.**  Swept on the same file: `do { stmt } while (0);` at every
   statement, best 32; a duplicated-arm `if (v) { s } else { s }` for each of
   p/b/bb/c/ee/g/lean/b2/q/r at every statement, best 19 (i.e. no
   improvement on the base); and a greedy second round on the best of those,
   no improvement.  So 16 currently costs one asm device, and **a plain-C
   route to 16 or below is the thing to look for before this lands.**
   docs/dungeon-parks/README.md records the preference and why.

   If a run is spent here, the permuter is a route, and
   docs/matching-levers.md's "THE PERMUTER DOES NOT CRACK A SMALL PURE-
   ALLOCATION RESIDUE" says what to expect.  Quote `ovlverify`'s 95/95 rather
   than aligndiff's 19: this is a pure permutation, and the LCS charges every
   moved instruction twice.

   -------------------------------------------------------------- what paid

   93/23 -> 95/19, all carried forward unchanged:

   * `ee = e;` AS THE FIRST STATEMENT IN THE FUNCTION, and nothing else
     before it.  Worth the 2 missing words.  Without it the fifth argument is
     re-loaded from the caller's frame at the call site and never occupies a
     register, so the function is two words short and the target's $fp
     save/restore pair has nothing to hold.  The position is not negotiable:
     all six orderings of `{ee = e; g = &D_80083160; lean = ...}` were
     measured and the three with `ee` first are 95 words while the three with
     anything ahead of it are 93.  j70 re-measured `bb = b; ee = e;` (bb
     first) and it is 93/29, so `bb` may not be hoisted over it either.
   * `bb = b;` right after it.  22 -> 19.
   * `s16 lean;` and not `s32` -- 19 against 26 (j70 re-measured 26).
   * the `g = &D_80083160;` pointer local for the tail block, and a separate
     local for the middle block's halfword temp, both carried over from the
     sibling.

   ------------------------------------------- what j70 measured and rejected

   Base here is 95 words / 19 differing.  The "profile" column is
   %hi/b/e/c/p live lengths, which is what decides the rotation.

   | spelling                                        | words | diff | profile
   | base (ee, bb, g, lean)                          |  95   |  19  | 74/51/27/50/60
   | `bb = b;` before `ee = e;`                      |  93   |  29  |
   | no `bb` at all (`b` used directly)              |  95   |  22  | 68/110/24/64/54
   | no bb + `cc = c;`                               |  95   |  22  | identical to no-bb
   | no bb + `pp = p;`                               |  95   |  22  | identical to no-bb
   | no bb + `cb = b2;` (both b2 reads via cb)       |  95   |  22  | identical to no-bb
   | no bb + head pointer local `sp0 = ...unk_00`    |  94   |  32  |
   | no bb + tail pointer temp                       |  95   |  33  |
   | no bb + `s32 lean`                              |  95   |  26  |
   | no bb + `ee` after `g` / after `lean`           |  93   |  23  |
   | no bb + `s16 ee`                                |  93   |  23  |
   | no bb + `t` and `t2` shared                     |  95   |  22  |
   | no bb + `q` and `u` shared                      |  95   |  30  |
   | no bb + `r` computed before `q`                 |  95   |  22  |
   | no bb + gate as `(c & 0xFFFF) != 0`             |  95   |  22  |
   | base + `lean` before `g`                        |  95   |  19  | unchanged
   | base + `bb` after `g` / after `lean`            |  95   |  19  | unchanged
   | base + `s16 ee`                                 |  93   |  29  |
   | base + middle block via `g->unk_00`             |  95   |  19  | unchanged
   | base + head block via `g->unk_00`               |  95   |  19  | unchanged
   | base + both via `g->unk_00`                     |  95   |  19  | unchanged
   | base + `bb` as `u8 *` with byte offsets         |  95   |  19  | unchanged
   | base + `bb` used only in the tail (`b` middle)  |  95   |  19  | unchanged
   | base + `bb` used only in the middle (`b` tail)  |  95   |  22  | copy dies

   The three `g->unk_00` rows matter: cse rewrites `(mem (reg g))` back onto
   `(mem (lo_sum %hi sym))`, so no spelling of the tail or middle block can
   shorten the %hi pseudo's range by taking it off that pseudo.

   Measured and byte-identical, so free to spell either way: `lean` and `g`
   in either order after `ee`/`bb`, and `bb` as a `u8 *` with byte offsets
   instead of an `s32 *`.    ====================================== 2026-11-20: 16 rows -> 12 at 95/95

   A PERMUTER FIND, two changes:

     * `(u16)c' HOISTED INTO A LOCAL immediately after the `b2' barrier --
       `cw = (u16)c;' -- and the later test written `if (cw != 0)' instead of
       `if ((u16)c != 0)'.  The narrowing then happens once, on the near side
       of the barrier, which is where the target has it.
     * the second `SetDrawArea' takes `b' DIRECTLY rather than the `bb' copy.
       `bb' is still load-bearing for the three `*(u16 *)bb' / `bb[1]' uses
       further down -- it is only this one argument that wants the parameter
       itself.

   Found by decomp-permuter in half an hour and measured with
   tools/decomp/measure_finds.py, which is necessary because the permuter's
   score is not the row count: of its eight best finds here, one measured 12
   and the rest measured 19 to 41.
*/
void func_800B8FC8(u8 *p, s32 *b, s32 *b2, s32 c, s32 e)
{
    Unk80083160 *g;
    u8 *q;
    u8 *r;
    u8 *u;
    s16 w[2];
    s16 lean;
    s32 t;
    s32 ee;
    s32 *bb;
    s32 t2;
    u16 cw;

    ee = e;
    __asm__("" ::"r"(b2));
    cw = (u16)c;
    bb = b;
    g = &D_80083160;
    lean = D_80083160.unk_00 != D_801C9E40;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    r = D_80083160.unk_00 + 0x8B0;
    *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
    SetDrawArea(q, D_80083160.unk_00);
    AddPrim(r, q);
    w[0] = *(u16 *)b2;
    t = *((s16 *)b2 + 1);
    if (lean)
    {
        t -= 0xE0;
    }
    w[1] = t;
    do
    {
        g++;
        g--;
        func_800B9144((u8 *)w, p, r, (s16)ee);
    } while (0);
    if (cw != 0)
    {
        u = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(u + 0x10);
        *(u32 *)(u + 0x4) = 0x60000000;
        u[0x3] = 3;
        *(u16 *)(u + 0x8) = *(u16 *)bb;
        t2 = *((s16 *)bb + 1);
        if (lean)
        {
            t2 -= 0xE0;
        }
        *(s16 *)(u + 0xA) = t2;
        *(u32 *)(u + 0xC) = bb[1];
        AddPrim(r, u);
    }
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    SetDrawArea(q, b);
    AddPrim(r, q);
}
