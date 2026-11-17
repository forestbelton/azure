/* PARKED 2026-11-16, j50; j69 took it 15 aligned rows -> 2; j74 settled WHY the
   last two will not move, and the answer closes a question that was open
   across TWO parks (this one and func_8008EAC8, whose residue is the same two
   instructions of the same shared source snippet).
   target 397 words, candidate 397 words, **2 aligned differences**.

   WHAT IS LEFT, AND IT IS ONLY THIS:
     136  sll v0,a0,0x10   vs   sll $v1, $a0, 16
     137  sra a1,v0,0x10   vs   sra $a1, $v1, 16
   The SImode temp holding `want << 16` (pseudo 181) gets $v0 where the target
   has $v1.  Everything else is byte-identical and the length is right.

   ========== j74: THE RESIDUE IS A MISSING CONFLICT, NOT A QTY INVERSION ======

   j69 diagnosed a `local-alloc` qty-priority inversion and derived inequalities
   (an insn strictly between the `sll` and the `lh`; the `subu` immediately
   after the `sra`).  THE ARITHMETIC IS RIGHT AND THE DIAGNOSIS IS WRONG, and
   the reason is one line of `local-alloc.c` nobody had checked:

     `find_free_reg` builds its `used` set as
     `for (ins = born_index; ins < dead_index; ins++) used |= regs_live_at[ins]`
     (local-alloc.c:2178), and `post_mark_life` marks exactly `[birth, death)`
     (local-alloc.c:2300ish).  Births are `2*insn_number`, deaths
     `2*insn_number + output_p` (`reg_is_set` :2038, `wipe_dead_reg` :2126),
     and deaths are processed BEFORE births inside one insn (:1477 vs :1487).

   In the TARGET's own instruction order -- `join, sll, sra, lh, lhu, subu` --
   that makes the two quantities' ranges

       181 = want << 16   [2*n(sll),  2*n(sra))   = two slots
       177 = the `lh`     [2*n(lh),   2*n(subu))

   **DISJOINT.**  So whichever quantity `qty_compare` sorts first takes $v0, and
   the other finds $v0 free over its own range and takes $v0 TOO.  That is not a
   prediction: it is what our build does -- the `.lreg` dump prints
   `;; Register 177 in 2.` and `;; Register 181 in 2.`, both $v0, and the
   emitted code reuses $v0 for the `lh` one insn after the `sra` kills it.

   **Exchanging the allocation order therefore cannot change anything.**  There
   is no inversion to fix.  What the target needs is a CONFLICT: 181 must still
   be live when 177 is born.

   =============== WHY NO C SPELLING CAN BUY THAT CONFLICT =================

   181 is born at the `sll` and dies at its LAST REFERENCE.  For a conflict its
   last reference must be scheduled at or after the `lh`.  And `cur`'s own
   register pins the other end: the target's `lhu $v1` at word 139 REUSES 181's
   $v1, so 181 must be dead before the `lhu` -- `cur` is a global allocno and
   `global_conflicts` would see the overlap.  So 181's last reference must land
   STRICTLY BETWEEN the `lh` and the `lhu`, and it must emit no word.

   Every zero-word insn fails that, for a reason read off sched.c:

   (1) A zero-word insn is scheduled AFTER the `lh` only if the `lh` depends on
       it -- sched runs BACKWARD, so a dependent is scheduled first and placed
       later.  Anything the `lh` depends on is therefore scheduled before the
       `lh` in the backward pass.
   (2) Such an insn carries priority 1 unless `adjust_priority` boosts it, and
       the boost needs `birthing_insn_p` to see a SET of a REG with
       `reg_n_sets == 1` that is live (sched.c:2513, :2551).  A `use`, a bare
       `__asm__("")`, a non-volatile `__asm__("" ::"r"(x))` and an
       `__asm__("" : "+r"(x))` (two sets!) are ALL priority 1.
   (3) A priority-1 insn loses every tie to the boosted `sra`/`sll`/`lh`/`lhu`,
       so it is scheduled only once nothing boosted is ready -- which is after
       the `sra`.  The `lh`, blocked behind it, then lands after the `sra` too,
       and THE EMITTED ORDER CHANGES: `sll, lh, sra, ...`, which is not the
       target's.  MEASURED, five ways, all 6-9 rows (below).
   (4) A BOOSTED zero-word insn (an asm with an `"=r"` output whose pseudo is
       live out of the block) is the opposite failure: it has no dependent in
       the block, so `sched_block` gives it an anti-dependence on the jump
       (sched.c:3409), it is ready at T-2 and its boost wins immediately, and it
       is placed at the END of the block -- past the `lhu`, which is the one
       place 181 must not reach.

   So the last reference can land between the `sll` and the `sra` (no overlap),
   or after the `lh` with the order destroyed, or at the block end with `cur`
   broken.  **There is no fourth slot.  The local-alloc route is closed.**

   ============ WHAT THAT MEANS, AND WHERE THE FIX MUST COME FROM ===========

   If local-alloc cannot do it, 181 is NOT a local quantity in the target: it
   is a GLOBAL allocno, and `global_alloc` gave it $v1.  That is j71's reading
   of func_8008EAC8 and it is the correct one -- but its stated reason ("you
   would need a basic-block boundary between the `sll` and the `sra`, and the
   sign-extension expander emits them as one unit") is only ONE of flow's two
   routes to globalness, and the cheaper one was missed:

     * flow.c:1480 -- a pseudo LIVE AT A BASIC BLOCK END is marked
       REG_BLOCK_GLOBAL.  (This is the boundary route j71 named.)
     * flow.c:2559-2562 -- a pseudo REFERENCED IN TWO BLOCKS is marked
       REG_BLOCK_GLOBAL.

   and, decisively, **`flow` runs ONCE, BEFORE `combine`** (toplev.c:3463 vs
   :3488), while `reg_basic_block` is never recomputed.  So a second-block
   reference that exists at flow time and that COMBINE later deletes still
   makes the temp global at local-alloc time, for zero words.  This unit
   already shows combine doing exactly that kind of deletion in this very
   block: pre-combine the `lh` was `179 = cur << 16; 177 = 179 >> 16` (cse had
   made `*(s16 *)(d+0x2A)` a sign-extension of `cur`), combine folded it to one
   `extendhisi2` from memory, and the leftover `(use (reg:SI 179))` is still
   sitting at the head of the block in the `.sched`/`.lreg` dumps.

   **THE HANDLE FOR ANY SUCH ATTEMPT IS NEW AND FREE, AND IT IS MEASURED:**

       sh = ((s32)want) << 16;
       delta = (*((s16 *)(d + 0x2A))) - (sh >> 16);

   with `s32 sh;` is an EXACT TIE -- 397 words, the same 2 rows (j74).  It
   names the shift temp as a C variable without changing one instruction, which
   is the thing the next attempt needs: the two halves of the sign extension
   become separate C expressions that control flow can be put between, and the
   temp becomes something a second reference can name.  What did NOT work from
   there: a second reference to `sh` from a later block costs a word (398, the
   value gets rematerialised in the `bgez` delay slot).

   =============== WHAT IS NEVERTHELESS PROVEN TO MOVE IT ==================

   The allocation CAN be flipped -- j74 got `sll $v1, $a0, 16` ALIGNED WITH THE
   TARGET for the first time -- by raising 177's loop-depth-weighted reference
   count, because `QTY_CMP_PRI` is
   `floor_log2(n_refs) * n_refs * size / (death - birth)` and `flow` adds
   `loop_depth` PER REFERENCE (flow.c:2566, :2780):

       do { ... cur = *((u16 *)(d + 0x2A));
            do { h = *((s16 *)(d + 0x2A)); } while (0);      <-- depth 3
       } while (0);
       delta = h - want;                                  <-- depth 1

   gives 177 four weighted refs against 181's two, 177 sorts first, and 181 is
   pushed to $v1.  It is 8 rows, not 0, and for a reason that is itself a lever
   worth recording: **an inner `do { } while (0)`'s LOOP notes are a FULL
   SCHEDULING BARRIER on the insn that carries them.**  `sched_analyze_insn`,
   when `loop_notes` is non-zero, makes that insn depend on the last set of
   EVERY register (kind 0, i.e. DATA, so loads cost 2) and flushes the memory
   lists (sched.c:2086-2104).  So the note before the `delta` statement pins the
   `sll` behind both loads and the emitted order becomes `lhu, lh, sll, sra,
   subu`.  Every placement of the inner loop that buys the weight also buys
   that barrier; every placement that avoids the barrier loses the weight.

   The same note mechanism explains the baseline order, which nothing had: the
   `bgez` carries this body's NOTE_INSN_LOOP_CONT/LOOP_END, so it has a DATA
   dependence on all seven insns of the block, which is why both loads are
   launched at a cost of 2 and the `lhu` reaches the ready list at T-3, beats
   the `sra` on `schedule_select`'s potential-hazard rule, and pushes the `sra`
   ahead of the `lh`.  That is also why `__asm__("" : "+r"(delta))` works the
   way j69 saw it: it occupies T-2, the `subu` slips to T-4, and the `lh` is
   then launched where only the `sll` competes.

   =================== HOW THE 15 ROWS WENT (j69, ONE COMPILE) ==============

   `allocno_compare` scores `floor_log2(refs) * refs / live_length`, and with
   j50's permuted `do { } while (0)` the real numbers (tools/decomp/dump.sh,
   -G0) were:

     98  = want                      12 refs / 28 insns -> 12857   got $a1
     154 = the raw *(u16 *)(d + 0x2A) 6 refs /  9 insns -> 13333   got $a0
     155 = the `& 0xFFF` result       5 refs /  7 insns -> 14285   got $v1

   so `want` had to land strictly between 154 and 155.  The cure was NOT to buy
   `want` a reference: the `do { } while (0)`'s LOOP notes were DOUBLE-COUNTING
   the two references to the raw halfword inside them, so moving

       *(s16 *)(d + 0x2A) |= 0xF800;   /   *(s16 *)(d + 0x2A) &= 0x7FF;

   OUT of the `do { } while (0)` took 154 from 6 refs to 4 (8888) and left 98 at
   12/28 (12857).  The pair flipped and thirteen rows went with it.

   **THE `do { } while (0)` IS STILL LOAD-BEARING**: it is what keeps the three
   references to `want` inside it weighted at depth 2, which holds 98 at 12 refs
   rather than 7.  Do not delete it, and do not put the halfword if/else back
   inside it.

   ====================== DECLARATIONS THIS BODY NEEDS ======================
       extern u8 D_800245A8[];
       extern void (*D_800DD830[])(void);
   NO prototype change: `void func_80096384();` in csrc/ovl_dungeon.c is
   unprototyped and compatible with this definition.

   ============== SPELLINGS THAT ARE ALREADY LOAD-BEARING HERE ==============
   (all measured by j50; do not "simplify" any of them)
   (1) `s16 want;` -- the catalogue's `:25519` entry.  As `s32` the byte folds
       and the function is 396 words; the narrow destination keeps the
       surviving `andi 0xFF`, buys the `addu $v0, $a0, $zero` copy, and raises
       nonzero_bits(want) past 0xFFFF so that `want & ~0xFFF` comes out as the
       target's `addiu $v0, $zero, -0x1000` + `and` instead of one `andi`.
   (2) THE SWITCH ARMS IN VALUE ORDER 8, 0x10, 0x18, 0xD8 -- `:27053`, worth
       10 differing.
   (3) THE `want` SIGN EXTENSION IS A TERNARY, NOT AN IF/ELSE (398 words as an
       if/else).  The SAME construct on `*(s16 *)(d + 0x2A)` must stay an
       if/else, because there the common store is the join.
   (4) `*(s16 *)(d + 0x2A) &= 0xFFF;` as a COMPOUND ASSIGNMENT, not through an
       `n` local: 26 -> 23 differing.
   (5) ONE POINTER LOCAL PER D_800DD274 ARM (`t` at the head, `t2` in the
       tail): 6 differing.
   (6) `cur = *(u16 *)(d + 0x2A);` read BEFORE the `delta` subtraction: 3
       differing.

   ==================== MEASURED AND REJECTED (j50) =========================
   (all 397 words / 17 differing at the time unless stated)
     `want = m[1] & 7; want <<= 9;`                       396 words
     `(s16)((m[0x1] & 7) << 9)`                           no change
     `want` and the compare operands swapped              18
     `(*(u16 *)(d + 0x2A) & 0xFFF) != want`               no change
     `want < *(s16 *)(d + 0x2A)` for the `>` test         no change
     a `wext` local for the sign-extended value           no change
     an explicit `cur0` local for the raw halfword        no change
     `cur0` reused for the `& 0xFFF` in the `> 0x800` arm 396 words
     `cur` placed after `delta`, or after the abs         17 / 20
     `delta >= 0x801` instead of `> 0x800`                no change
     `u16 cur`, `s16 delta`, six declaration orders       no change

   ============ MEASURED AND REJECTED (j69, against the 2-row body) =========
     cur read after `delta`, both still inside the do-while      2  (a tie)
     `delta = -(want - *(s16 *)(d + 0x2A))`                      2  (a tie)
     `delta = *(s16 *)(d + 0x2A) - (s32)want`                    2  (a tie)
     `u16 cur`                                                   2  (a tie)
     `__asm__("" : "+r"(want))` after the ternary                2  (a tie)
     `__asm__("" : "+r"(want))` before the subtraction           2  (a tie)
     `__asm__("" : "+r"(delta))` after the subtraction           6
     ... with either `+r(want)` or `"r"(want)` as well           6
     `__asm__("" : "+r"(cur))` after the cur read                10
     `__asm__("" : "+r"(cur))` + `+r(delta)`                     10
     `"r"(cur)` before the subtraction + `+r(delta)`             13, 398 words
     cur read OUTSIDE (before) the do-while                      8
     `delta` computed OUTSIDE (after) the do-while               10
     do-while around the `want` ternary ONLY                     10
     do-while nested round `cur`+`delta`                         10
     do-while nested round `delta` only                          10
     the `if (delta < 0)` abs pulled INSIDE the do-while         20, 403 words
     `delta = *(s16 *)(d + 0x2A); delta -= want;`                20
     `s16 delta`                                                 41, 400 words

   ============== MEASURED AND REJECTED (j74, 397 words unless noted) =======
   Every one of these is a whole-function aligndiff against the body below.
   The ones that reached `sll $v1` are marked -- they are the ones that prove
   the mechanism, and they fail only on the emitted ORDER of the two loads.
     `sh = ((s32)want) << 16;` + `(sh >> 16)`                     2  (a tie)
         -- the free handle on the shift temp; see above
     `h = *(s16 *)(d+0x2A)` inside, `delta = h - want` outside    9
     ... with an inner do-while round the `h` statement           8   $v1 REACHED
     ... that, plus `__asm__("" : "+r"(delta))`                   9
     inner do-while round the `delta` statement only              8
     inner do-while round the `delta` statement, no `h` local    10
     inner do-while round `h`, plus `+r(delta)`                   8
     `__asm__("" ::"r"(h))` before the subtraction                8
     `__asm__("" ::"r"(h))` after the subtraction                 8
     `+r(delta)` plus `"r"(h)` after                              7
     `+r(delta)` plus `"r"(h)` before                             7
     `sh` + `__asm__("" ::"r"(sh))` after the subtraction         8, 398 words
     `sh` + `__asm__("" ::"r"(sh), "r"(h), "r"(h))`               8, 398 words
     `sh` + `__asm__("" ::"r"(sh))` in the `if (delta < 0)` block 19
   In EVERY one of the 8/9-row rows the two loads are hoisted above the `sll`
   (`lhu, lh, sll, sra, subu`); in the 6/7-row rows the `lh` sits between the
   `sll` and the `sra` (`sll, lh, sra, subu, lhu`).  Both are the same thing in
   two directions: whatever gives 181 a long enough life to conflict also moves
   the `lh` relative to the `sra`.

   =============== WHAT THE NEXT ATTEMPT SHOULD AND SHOULD NOT DO ==========
   DO NOT re-run any spelling above, and DO NOT look for a zero-word insn
   between the `sll` and the `lh`: j69's inequalities describe an order that is
   not the target's, and sched2 does not restore the target's order -- measured
   on nine variants, the post-sched1 (`.lreg`) order equals the emitted order
   EVERY time, so the order local-alloc prices is the order that ships.

   DO attack globalness, which is where the target's $v1 has to come from, and
   remember it is decided PRE-COMBINE.  The target's own
   `slt $v0, $a1, $v0` at word 153 shows the companion temp 180 (the `sra`
   result) IS global -- cse unified the later `*(s16 *)(d+0x2A) > want` test
   onto it across a block boundary, which is exactly the kind of second-block
   reference 181 needs and does not have.  A spelling that makes cse stop one
   level short -- unifying `want << 16` across the two tests but NOT the full
   sign extension -- would give 181 a second-block reference at flow time; with
   `sh` now available as a named C variable that is a reachable experiment
   rather than a wish.  The permuter is still the wrong tool here (a pure
   register residue at the correct word count), and this park now says exactly
   which pass has to move, so a search that cannot change `reg_basic_block`
   cannot win.
 */

void func_80096384(u8 *p, void *b, u8 *c, u8 *d)
{
    Unk80083160 *g = &D_80083160;
    u8 *q;
    void **t;
    void **t2;
    s32 cur;
    u8 *m;
    u16 k;
    s32 e;
    s32 f;
    s32 w;
    s32 n;
    s32 r;
    s32 delta;
    s16 want;
    if (p[0x9A] != 0x32)
    {
        t = D_800DD274;
        *((void ***)(c + 0x2C)) = t;
        func_8003DB94(c, t[(((g->unk_C8 + (*((s16 *)(d + 0x2A)))) + 0x100) >> 9) & 7], 0);
        p[0x9A] = 0x32;
        *((u16 *)(p + 0xA2)) &= 0xFEFF;
        *((u16 *)(c + 0x14)) &= 0xF7FF;
        p[0x9B] = 0;
        *((s16 *)(p + 0x96)) = 0;
        func_800A4300(c, d);
    }
    q = (u8 *)(&D_80083460);
    *((u16 *)(q + 0x2)) &= 0xFF7F;
    r = *((s16 *)(d + 0x64));
    if (r < 0)
    {
        func_80097898(p, b, c, d);
        return;
    }
    if (r > 0)
    {
        func_8008CBA0(p, b, c, d);
    }
    if (((*((s32 *)(d + 0x14))) & 0x20000) != 0)
    {
        *((s32 *)(d + 0x14)) &= 0xFFFDFFFF;
    }
    if (((*((u16 *)(p + 0xA2))) & 0x10) == 0)
    {
        func_80096AC0(p, b, c, d);
        return;
    }
    e = *((s32 *)(d + 0x1C));
    if ((e & 0x200) != 0)
    {
        func_80097C50((UnkDungeonActor *)p, b, c, d);
        return;
    }
    if (((*((u16 *)(q + 0x2))) & 0x4) == 0)
    {
        if ((e & 0x20) != 0)
        {
            func_800969B8(p, b, c, d);
            return;
        }
        k = *((u16 *)0x80013714);
        if ((k & 1) != 0)
        {
            *((u16 *)(p + 0xA2)) &= 0xFFFE;
            if (((*((u16 *)(p + 0xA2))) & 0x200) != 0)
            {
                *((u16 *)(p + 0xA2)) &= 0xFDFF;
            }
            m = func_8009F868();
            if (m == 0)
            {
                return;
            }
            want = (m[0x1] & 7) << 9;
            *((s16 *)(d + 0x2A)) &= 0xFFF;
            if ((*((s16 *)(d + 0x2A))) != want)
            {
                if (((*((u16 *)(d + 0x2A))) & 0x800) != 0)
                {
                    *((s16 *)(d + 0x2A)) |= 0xF800;
                }
                else
                {
                    *((s16 *)(d + 0x2A)) &= 0x7FF;
                }
                do
                {
                    want = (want & 0x800) ? (want | 0xF800) : (want & 0x7FF);
                    cur = *((u16 *)(d + 0x2A));
                    delta = (*((s16 *)(d + 0x2A))) - want;
                } while (0);
                if (delta < 0)
                {
                    delta = -delta;
                }
                if (delta > 0x800)
                {
                    *((s16 *)(d + 0x2A)) = (want & (~0xFFF)) | (cur & 0xFFF);
                }
                if ((*((s16 *)(d + 0x2A))) > want)
                {
                    *((s16 *)(d + 0x2A)) = (*((u16 *)(d + 0x2A))) - 0x200;
                }
                else
                {
                    *((s16 *)(d + 0x2A)) = (*((u16 *)(d + 0x2A))) + 0x200;
                }
                func_8009F988();
                goto tail;
            }
            D_800E3544 = m[0x1] & 0xF8;
            switch (D_800E3544)
            {
            case 8:
                func_80096C24(p, b, c, d);
                return;

            case 0x10:
                func_800969B8(p, b, c, d);
                return;

            case 0x18:
                func_800973F0(p, b, c, d);
                goto tail;

            case 0xD8:
                D_800DD830[m[0x0] & 0x7F]();
                goto tail;
            }

            goto tail;
        }
        D_800E3544 = 0;
        if ((k & 4) != 0)
        {
            func_80040AA0(3);
            return;
        }
        if (((s16)func_80094EA4()) != 0)
        {
            *((s16 *)(p + 0xA4)) = 0;
        }
        if ((((((*((u16 *)(p + 0xA2))) & 0x200) != 0) && (((s16)func_800A2C34(d)) == 0)) && (((*((u16 *)(q + 0x2))) & 0x4) == 0)) && (((s16)func_800A4474(c[0x24], c[0x25])) != 0))
        {
            *((u16 *)(p + 0xA2)) &= 0xFDFF;
            func_8008CF6C((UnkDungeonActor *)p, b, c, D_800245A8);
            return;
        }
        f = g->unk_08;
        if ((f & 0x80) != 0)
        {
            *((s16 *)(d + 0x8A)) = 2;
            D_800E4940 = 2;
            func_8008CF6C((UnkDungeonActor *)p, b, c, D_8004F5F4);
            D_80082EB0 = 0;
            *((s32 *)(p + 0xC8)) = 0;
            *((s32 *)(p + 0x104)) = 0;
            return;
        }
        if ((f & 3) != 0)
        {
            func_800A56E0(0x506);
            return;
        }
        n = func_8009074C(*((s16 *)(p + 0x9E)), (u16 *)(p + 0xA2), (u16 *)(d + 0x2A));
        if (((s16)n) != 0xFFF)
        {
            *((s16 *)(d + 0x2A)) = n;
            if (((g->unk_08 & 0x10) == 0) && (((*((u16 *)(p + 0xA2))) & 0x400) == 0))
            {
                *((u16 *)(p + 0xA2)) &= 0xFFFE;
                if (((*((s32 *)(d + 0x1C))) & 0x400) != 0)
                {
                    *((u16 *)(d + 0x2A)) += func_800A6D30() & 0xE00;
                }
                func_80096C24(p, b, c, d);
                if (p[0x9A] != 0x32)
                {
                    return;
                }
            }
            else
            {
                *((s16 *)(p + 0x96)) = 0;
            }
        }
        w = g->unk_08;
        if ((w & 0x30) == 0x30)
        {
            *((u16 *)(p + 0xA2)) &= 0xFFFE;
            func_800969B8(p, b, c, d);
            return;
        }
        if (((*((u16 *)(p + 0xA2))) & 0x40) != 0)
        {
            if ((w & 0x40) == 0)
            {
                *((u16 *)(p + 0xA2)) &= 0xFFBF;
            }
        }
        else if ((w & 0x40) != 0)
        {
            *((u16 *)(p + 0xA2)) &= 0xFFFE;
            if (((s16)func_800973F0(p, b, c, d)) != 0)
            {
                return;
            }
        }
    }
tail:
    if (((*((u16 *)(p + 0xA2))) & 0x1) != 0)
    {
        *((u16 *)(p + 0xA2)) &= 0xFFFE;
        t2 = D_800DD274;
        *((void ***)(c + 0x2C)) = t2;
        func_8003DB94(c, t2[(((D_80083228 + (*((s16 *)(d + 0x2A)))) + 0x100) >> 9) & 7], 0);
    }
}
