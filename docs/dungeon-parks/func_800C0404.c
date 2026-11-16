/* PARK: func_800C0404, 234 of 234 words, 2 aligned differences / 3 masked.
   ovl_dungeon.  Re-measured 2026-11-16.  THIS BODY WAS RECOVERED OUT OF THE
   GITIGNORED `nonmatchings/func_800C0404/base.c` -- the 2026-11-15 permuter
   run's scaffold -- which was the only copy of it anywhere.  It is written
   here so that a `nonmatchings/` clean does not take it.

   It is the kind-? node's +0x10 handler (`n->unk_10 = (u32)func_800C0404`),
   so the three parameters are the engine's usual (node + 0x20, n->unk_08,
   n->unk_0C).  It draws the flash quads out of D_80083160.unk_00's +0x8D0
   cursor, then runs a three-state tail on +0x4A.

   NO NEW DECLARATIONS and NO PROTOTYPE CHANGE: every symbol it names is
   already declared in csrc/ovl_dungeon.c, and `void func_800C0404();` at
   :1046 fits the definition below.

   ------------------------------------------------------------------ residue

   WORD COUNTS MATCH.  `aligndiff` reports TWO rows and they are the SAME
   INSTRUCTION in two places -- a pure transposition inside one basic block:

     target    132 lw   $v1, %lo(D_800E3D7C)($a2)
               133 lui  $v0, %hi(D_80083228)
               134 sw   $a1, 0x2C($a0)
     candidate 132 lui  $v0, %hi(D_80083228)
               133 sw   $a1, 0x2C($a0)
               134 lw   $v1, %lo(D_800E3D7C)($a2)

   Everything before 132 and from 135 on is byte-identical, registers
   included.  The three insns are mutually independent; both schedules
   satisfy the load delay of the two `lh`s at 135/136.  So the residue is
   sched1 choosing a different member of the ready list, which is what
   docs/matching-levers.md's "THE PERMUTER DOES NOT CRACK A SMALL
   PURE-ALLOCATION RESIDUE" table already records for this function:
   **base 60 -> best 60 over 132,425 candidates, 30 minutes, found=none.**
   Do not queue it again.

   THE `do { if (lean) { ... } } while (0)` IN THE LOOP IS LOAD-BEARING AND
   ALREADY PAID FOR.  It is worth 28 of 31 differing -- `flow.c:2122` adds
   `loop_depth` to `reg_n_refs` at every reference, so the zero-trip loop
   multiplies the reference counts inside it and flips `allocno_compare`.
   The `if` MUST be nested inside the loop: `while (x) { ...; break; }` and
   `do { if (!x) break; ... } while (0)` are both 40 differing.  See
   docs/matching-levers.md, "A ZERO-TRIP `do { } while (0)` AROUND AN `if` IS
   A REGISTER-ALLOCATION DEVICE".

   MEASURED AND REJECTED on 2026-11-16, all against this body, all at 234
   words (so none of them is a length regression -- they only move the
   transposition around or break something downstream of it):

     this body                                                3 masked
     `((u8 **)D_80082E80)[0xB] = t;` (ARRAY_REF store)         7   -- it frees
         the store to float ABOVE the `sb $v0, 0x9B($v1)` as well, 10 aligned
     `((s16 *)D_800E3D7C)[0x15]` for the 0x2A read            11
     both of the above                                         7
     `u8 *d = D_800E3D7C;` before the store, 0x2A read off d   3   -- the `lw`
         is STILL scheduled after the `sw`, which is what rules the source
         ORDER out as the cause and leaves the ready-list tie as the cause
     the same with `d` assigned before `t`                     3
     `*(u8 **)(D_80082E80 + 0x2C) = D_800DD0F8;` (no `t`)      3
     the store written before `t = D_800DD0F8;`                3
     no `t` local at all, D_800DD0F8 named twice               3
     `*(u8 *)(D_800E3D7C + 0x9B) = 0x11;` (INDIRECT_REF store) 3
     `*(s16 *)&D_800E3D7C[0x2A]`                               3
     the store moved ABOVE `D_800E3D7C[0x9B] = 0x11;`          8
     `(*(s16 *)(D_800E3D7C + 0x2A) + D_80083228 + 0x100)`      5   -- swaps
         which of $v0/$v1 holds which half; the ORDER is unchanged
     `((D_80083228 + 0x100) + *(s16 *)(...))`                 15
     the index hoisted into an `s32` local before the call   235 words / 181

   WHAT THIS CANDIDATE HOLDS FIXED: the `u32 w[2]` initialised aggregate and
   the `UnkDungeonRect q` with its two `.v[]` stores, the `lean` boolean
   computed once before the loop, `s16 h` for func_8003F794's result, and the
   three-way `m` chain written as `if (m == 0) ... else if (m == 1)`.  None of
   those was re-measured this session.

   ------------------------------------- THE SCHEDULER, MEASURED (agent j54)

   2026-11-16.  Read out of cc1's own `-da' dumps for a standalone file
   (unit declaration block + this body, compiled with tools/cdkgcc), via
   `tools/decomp/sched.py <file> func_800C0404' and `--sched1'.  THE
   STANDALONE FILE REPRODUCES THE RESIDUE EXACTLY, so the whole loop below
   is one 0.5s compile and needs no splice and no verifier.

   THE TRANSPOSITION IS DECIDED AT sched1, NOT sched2, AND sched2 CANNOT
   UNDO IT.  Name the four insns by their sched dump numbers:

       329  sb  $v0, 0x9B($v1)                (word 131, matches)
       337  sw  $a1, 0x2C($a0)                the store
       341  lui $v0, %hi(D_80083228)
       350  lw  $v1, %lo(D_800E3D7C)($a2)     the SECOND pointer load
       354  lh  $v1, 0x2A($v1)

   * BEFORE sched1 the RTL order is 337, 341, 346(`lh %lo'), 350, 354 --
     the store precedes the second load because the store statement
     precedes the call whose argument reads D_800E3D7C.
   * AT sched1 the store and the load have NO dependence: alias.c's
     `canon_rtx' resolves the store's base pseudo through its REG_EQUIV
     note to `symbol_ref D_80082E80', the load's address is
     `lo_sum(reg, symbol_ref D_800E3D7C)', and two different symbols do
     not conflict.  sched1 is therefore free to order them either way.
   * AT sched2 the same two MEMs are `(plus (reg a0) 44)' and
     `(lo_sum (reg a2) D_800E3D7C)'.  There are no REG_EQUIV notes on hard
     registers, so `canon_rtx' resolves nothing and the dependence IS
     created.  **Whichever of the two sched1 emits first is frozen.**  That
     is why every spelling in the table above leaves the load after the
     store: they all let sched1 emit the store first.

   WHY sched1 ALWAYS EMITS THE STORE FIRST.  sched.c schedules backward and
   the insn selected at the LARGEST T is emitted first.  In block 9:

       ;; ready list at T-11: 346 (7f000001) 365 (1) 337 (2), now 346 337 365
       ;; ready list at T-12: 337 (2) 365 (1) 341 (7f000001) 350 (7f000001),
       ;;                     now 350 341 337 365
       ;; ready list at T-13: 341 (7f000001) 337 (2) 365 (1), now 341 337 365
       ;; ready list at T-14: 337 (2) 365 (1), now 337 365

   0x7f000001 is sched.c:189 `LAUNCH_PRIORITY'.  `adjust_priority'
   (sched.c:2551) gives it to every insn that becomes ready while
   `reload_completed == 0' and `birthing_insn_p' (sched.c:2512) holds --
   that is, the insn SETs a pseudo that is live and `reg_n_sets == 1'.
   `schedule_select' only ever chooses inside the HIGHEST-priority group,
   so the store at priority 2 cannot win a cycle in which any boosted insn
   is ready; it is selected last (T-14) and therefore EMITTED FIRST.  Both
   341 and 350 are boosted, they tie, `rank_for_schedule' falls through to
   INSN_LUID and the later one (350) wins T-12.  Changing which of 341/350
   has the higher LUID only swaps those two; the store still comes out
   first.  So no reordering of the addends and no placement of the store
   relative to `t =' or to the 0x9B store can reach the target, which is
   exactly what the sixteen spellings in the table above measured.

   ----------------------------- THE SPELLING THAT DOES FIX THE SCHEDULE

   Give the second load a destination with `reg_n_sets > 1' and
   `birthing_insn_p' returns 0, the boost disappears, the store wins T-12,
   and THE ORDER COMES OUT EXACTLY RIGHT.  Routing BOTH reads of
   D_800E3D7C through one local does it:

       z = D_800E3D7C;
       z[0x9B] = 0x11;
       z = D_800E3D7C;
       t = D_800DD0F8;
       *(u8 **)(D_80082E80 + 0x2C) = t;
       func_80048A44(D_80082E80,
                     t[((D_80083228 + *(s16 *)(z + 0x2A) + 0x100) >> 9) & 7], 1, 1);

   (`u8 *z;' added to the locals; the 0x9B store through `z' is the same
   lw+sb, and it cannot be CSEd with the second read because the sb
   invalidates D_800E3D7C.)  Measured: **234 of 234 words, 5 aligned
   differences, and the two transposed rows are GONE** -- words 125..145
   are in the target's order instruction for instruction.  Saved as the
   alternative body at the bottom of this file.

   WHY IT IS STILL 5 AND NOT 0 -- AND WHY THAT LOOKS UNREACHABLE.  All five
   rows are ONE REGISTER PAIR SWAPPED: the shared `%hi(D_800E3D7C)' base is
   $v1 here and $a2 in the target, and the loaded pointer is $a2 here and
   $v1 there.  Cause, read off `.lreg'/`.greg':

     * local-alloc.c:472 only marks a pseudo locally allocatable when
       `reg_basic_block >= 0 && reg_n_deaths == 1'.  Two sets in one block
       means two deaths, two sets in two blocks means no single block, so
       **any pseudo with reg_n_sets > 1 is handed to global_alloc** -- and
       in the target both the base and the pointer are plainly local-alloc
       temps ($v1 is reused by both loads).
     * global_alloc then prints `;; 88 conflicts: 80 88 2 3 4 5 29' for
       `z': $v1 is excluded because `z's FIRST range (the 0x9B store)
       overlaps the `%hi' base, which local-alloc has already given $v1.
       $a2 is the first register left.

   So the two requirements are contradictory under this compiler:
   killing the boost needs reg_n_sets >= 2, and reg_n_sets >= 2 forces
   global allocation, which cannot give back the local-alloc register.
   Also measured, all at 234 words unless noted:

     `c' (2 sets, the m==1 arm reuses it) as the second load's home . 235 / 17
     `e' (3 sets, the loop's cursor)   .............................. 234 / 9
     `z' split across the two arms (second set replaces the m==1
         arm's `c = *(u8 **)(p - 0x18)') ............................ 234 / 11
         -- `z' still takes $a2, and the m==1 arm's `lw $a0, -0x18($s4)'
            becomes `lw $a2', so that arm regresses too
     `u8 *d2 = D_800E3D7C;' between the sb and the store, used once
         (reg_n_sets == 1) ......................................... 234 / 2,
         byte-identical to this body: it is still birthing, still boosted,
         and sched1 still moves it back below the store.

   THE ONLY ROUTE LEFT is a spelling that makes the second load's pseudo
   have two sets whose FIRST range does not overlap the `%hi' base -- i.e.
   a second set of the same local, in block 9, before the
   `lui %hi(D_800E3D7C)', that survives cse (a plain `z = <other pseudo>'
   copy does not: cse propagates it and the set is deleted, putting
   reg_n_sets back to 1).  Nothing in the block's prologue qualifies: every
   candidate value there is a call argument and crosses func_80091934.

   NEXT MOVE, if anyone spends more on it: `tools/decomp/sched.py <file>
   func_800C0404 --sched1` prints cc1psx's own ready lists.  The block is
   number 9 (insns 307..373 in that dump) and the insn that keeps losing the
   tie shows as priority 1 / ref_count 1, sitting in the ready list from T-2
   to T-21.  Solve for the reference count rather than sweeping spellings --
   that is the route docs/matching-levers.md prescribes for exactly this
   class. */

void func_800C0404(u8 *p, UnkVec *v, UnkSlot *r)
{
    u32 w[2];
    UnkDungeonRect q;
    Unk80083160 *g;
    u8 *tag;
    u8 *e;
    u8 *c;
    u8 *t;
    s32 lean;
    s32 i;
    s32 n;
    s32 k;
    s32 m;
    s16 h;

    w[0] = 0x1800340;
    w[1] = 0x400040;
    g = &D_80083160;
    lean = g->unk_00 != D_801C9E40;
    e = *(u8 **)(g->unk_00 + 0x8D0);
    tag = g->unk_00 + 0x8B0;
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(e + 0xC);
    func_80067E2C(e, g->unk_00);
    func_8006658C(tag, e);
    for (i = 0; i < *(s16 *)(p + 0x52); i++)
    {
        e = *(u8 **)(g->unk_00 + 0x8D0);
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)(e + 0x10);
        n = (func_80069EF8() & 0x3F) + 0x340;
        *(s16 *)(e + 0xC) = n;
        *(s16 *)(e + 0x8) = n;
        k = func_80069EF8() & 0x3F;
        *(s16 *)(e + 0xA) = 0x180 - k;
        *(s16 *)(e + 0xE) = 0x1BF - k;
        do
        {
            if (lean)
            {
                *(u16 *)(e + 0xA) -= 0xE0;
                *(u16 *)(e + 0xE) -= 0xE0;
            }
        } while (0);
        *(u32 *)(e + 0x4) = 0;
        func_800667BC(e);
        func_8006658C(tag, e);
    }
    if (*(s16 *)(p + 0x52) < 0x60)
    {
        *(s16 *)(p + 0x52) = *(u16 *)(p + 0x52) + 1;
    }
    e = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(e + 0xC);
    func_80067E2C(e, w);
    func_8006658C(tag, e);
    q.v[0] = 0x360;
    q.v[1] = 0x1B8;
    func_800B8FC8(*(u8 **)(p + 0x68), w, q.v, 1, 0);
    *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
    m = *(s16 *)(p + 0x4A);
    if (m == 0)
    {
        if (*(s16 *)(p + 0x4C) <= 0x20)
        {
            return;
        }
        c = *(u8 **)(p + 0x68);
        if (c[0x13] == 0)
        {
            func_80091934((UnkDungeonActor *)c, D_80083780, D_80082E80, NULL);
            D_800E3D7C[0x9B] = 0x11;
            t = D_800DD0F8;
            *(u8 **)(D_80082E80 + 0x2C) = t;
            func_80048A44(D_80082E80, t[((D_80083228 + *(s16 *)(D_800E3D7C + 0x2A) + 0x100) >> 9) & 7],
                          1, 1);
        }
        *(s16 *)(p + 0x4A) = *(u16 *)(p + 0x4A) + 1;
    }
    else if (m == 1)
    {
        r->unk_0C[0] -= r->unk_0C[0] >> 3;
        r->unk_0C[1] -= r->unk_0C[1] >> 3;
        r->unk_0C[2] -= r->unk_0C[2] >> 3;
        r->unk_1C += (0x400 - r->unk_1C) >> 2;
        c = *(u8 **)(p - 0x18);
        v->unk_08 += v->unk_14;
        *(s32 *)(c + 0x8) = v->unk_08;
        v->unk_14 -= 0x40000;
        if ((*(u8 **)(p + 0x68))[0x13] != 0)
        {
            return;
        }
        if (!(*(u16 *)(*(u8 **)(D_800E3D7C - 0x14) + 0x14) & 0x6000))
        {
            return;
        }
        h = func_8003F794(6, 0x20);
        D_800814E8 = h;
        D_80083120[h].unk_06 = 1;
        func_800945E8(D_800E3D7C);
        func_800948BC();
        D_80082E76 = 0x8000;
        func_80041094(6, 0, 0, 0, 0x8000);
        *(s16 *)(p + 0x4A) = *(u16 *)(p + 0x4A) + 1;
    }
}

#if 0
/* ALTERNATIVE (agent j54, 2026-11-16): 234 of 234 words, 5 aligned
   differences, ALL of them the $v1/$a2 swap described above.  The
   instruction ORDER of words 125..145 matches the target exactly, so if
   anyone finds a way to keep `z' in a local-alloc register this is the
   body to finish.  Do NOT permute it: the residue is allocation. */
void func_800C0404(u8 *p, UnkVec *v, UnkSlot *r)
{
    u32 w[2];
    UnkDungeonRect q;
    Unk80083160 *g;
    u8 *tag;
    u8 *e;
    u8 *c;
    u8 *t;
    u8 *z;
    s32 lean;
    s32 i;
    s32 n;
    s32 k;
    s32 m;
    s16 h;

    w[0] = 0x1800340;
    w[1] = 0x400040;
    g = &D_80083160;
    lean = g->unk_00 != D_801C9E40;
    e = *(u8 **)(g->unk_00 + 0x8D0);
    tag = g->unk_00 + 0x8B0;
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(e + 0xC);
    func_80067E2C(e, g->unk_00);
    func_8006658C(tag, e);
    for (i = 0; i < *(s16 *)(p + 0x52); i++)
    {
        e = *(u8 **)(g->unk_00 + 0x8D0);
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)(e + 0x10);
        n = (func_80069EF8() & 0x3F) + 0x340;
        *(s16 *)(e + 0xC) = n;
        *(s16 *)(e + 0x8) = n;
        k = func_80069EF8() & 0x3F;
        *(s16 *)(e + 0xA) = 0x180 - k;
        *(s16 *)(e + 0xE) = 0x1BF - k;
        do
        {
            if (lean)
            {
                *(u16 *)(e + 0xA) -= 0xE0;
                *(u16 *)(e + 0xE) -= 0xE0;
            }
        } while (0);
        *(u32 *)(e + 0x4) = 0;
        func_800667BC(e);
        func_8006658C(tag, e);
    }
    if (*(s16 *)(p + 0x52) < 0x60)
    {
        *(s16 *)(p + 0x52) = *(u16 *)(p + 0x52) + 1;
    }
    e = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(e + 0xC);
    func_80067E2C(e, w);
    func_8006658C(tag, e);
    q.v[0] = 0x360;
    q.v[1] = 0x1B8;
    func_800B8FC8(*(u8 **)(p + 0x68), w, q.v, 1, 0);
    *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
    m = *(s16 *)(p + 0x4A);
    if (m == 0)
    {
        if (*(s16 *)(p + 0x4C) <= 0x20)
        {
            return;
        }
        c = *(u8 **)(p + 0x68);
        if (c[0x13] == 0)
        {
            func_80091934((UnkDungeonActor *)c, D_80083780, D_80082E80, NULL);
            z = D_800E3D7C;
            z[0x9B] = 0x11;
            z = D_800E3D7C;
            t = D_800DD0F8;
            *(u8 **)(D_80082E80 + 0x2C) = t;
            func_80048A44(D_80082E80, t[((D_80083228 + *(s16 *)(z + 0x2A) + 0x100) >> 9) & 7], 1, 1);
        }
        *(s16 *)(p + 0x4A) = *(u16 *)(p + 0x4A) + 1;
    }
    else if (m == 1)
    {
        r->unk_0C[0] -= r->unk_0C[0] >> 3;
        r->unk_0C[1] -= r->unk_0C[1] >> 3;
        r->unk_0C[2] -= r->unk_0C[2] >> 3;
        r->unk_1C += (0x400 - r->unk_1C) >> 2;
        c = *(u8 **)(p - 0x18);
        v->unk_08 += v->unk_14;
        *(s32 *)(c + 0x8) = v->unk_08;
        v->unk_14 -= 0x40000;
        if ((*(u8 **)(p + 0x68))[0x13] != 0)
        {
            return;
        }
        if (!(*(u16 *)(*(u8 **)(D_800E3D7C - 0x14) + 0x14) & 0x6000))
        {
            return;
        }
        h = func_8003F794(6, 0x20);
        D_800814E8 = h;
        D_80083120[h].unk_06 = 1;
        func_800945E8(D_800E3D7C);
        func_800948BC();
        D_80082E76 = 0x8000;
        func_80041094(6, 0, 0, 0, 0x8000);
        *(s16 *)(p + 0x4A) = *(u16 *)(p + 0x4A) + 1;
    }
}
#endif
