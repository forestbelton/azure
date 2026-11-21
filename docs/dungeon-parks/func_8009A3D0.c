/* func_8009A3D0 (ovl_dungeon) -- PARK, best so far.

   target 92 words, candidate 92 words, **2 ALIGNED DIFFERENCES**
   (2026-11-20).  Previous states: 91/45 (j41-era), 92/15 (j89).

   THE RESIDUE IS LEVER 5's TWO WORDS AND NOTHING ELSE:

       81  or  $s0, $v0, $a2     target  or  $v0, $v0, $a2
       82  sh  $s0, 0x4($v1)             sh  $v0, 0x4($v1)

   i.e. the tail's `or`/`sh` write `f` ($s0) where the target writes a
   caller-saved temp.  Every other word of the 92, and every other register,
   is correct.

   ============ WHAT TOOK IT FROM 15 TO 2: THE CLAMP IS ONE-ARMED, AND ITS
   ============ COPY IS AN addsi3 WITH `$zero`

   The park had the diagnosis right and the shape wrong.  It is

       m = n + z0;                 (* with register s32 z0 __asm__("$0") *)
       if ((s16)n < 0)
       {
           m = 0;
       }

   and NOT the two-armed `if ((s16)n < 0) m = 0; else m = n;` this park
   carried.  Both halves are needed and each is visible in the target.

   (a) ONE-ARMED, WITH THE COPY FIRST.  The target's word 64 is
   `addu $a2, $a0, $zero` -- the copy -- at a slot BEFORE the sign test at
   67/68, and word 68 is `bgez $a0, .L8009A4EC`, an INVERTED branch whose
   delay slot holds `sh $a1, 0x4($v1)` and whose taken arm is
   `addu $a2, $zero, $zero` at 70.  That is `m = n;` unconditionally followed
   by a one-armed `if (n < 0) m = 0;`.  The park's rejected list has the
   OTHER one-arming -- `m = 0;` first with `if ((s16)n >= 0) m = n;`, 22 rows
   -- and never measured this one.  On its own it is 91 words / 18 rows.

   (b) THE COPY IS `n + z0`, NOT `n`.  This is the whole of the park's
   identified root cause.  local-alloc's `optimize_reg_copy_1` eats a
   `m = n` copy: sched1 has already placed it ahead of the sign-test `sll`,
   so the forward scan from the copy reaches that `sll`, finds `REG_DEAD n`
   on it and rewrites it to read `m` -- which shortens n's quantity, lifts
   its QTY_CMP_PRI and cascades through six caller-saved registers.  The
   park's cure was to BLOCK the scan with a third set of `m`, which works but
   drags a `nor` to word 54 (19 differing).  The better cure is never to
   enter the pass: local-alloc.c:1025-1028 gates it on

       flag_expensive_optimizations && GET_CODE (dest) == REG
       && GET_CODE (SET_SRC (set)) == REG
       && ! find_reg_note (insn, REG_DEAD, SET_SRC (set))

   and a PLUS is not a REG, so `m = n + z0;` is not considered at all -- while
   `addu $a2, $a0, $zero` is the same word the target has.  See
   docs/matching-levers.md, "A REDUNDANT REGISTER COPY SURVIVES jump2 IF YOU
   SPELL IT `x + z0`": that spelling has now beaten jump2's noop-move
   deletion (func_800CA1E0), cse's copy propagation (func_8009A8C0) and
   local-alloc's optimize_reg_copy_1 (here).  Three passes, one spelling.

   With the two-armed clamp, `m = n + z0` alone is 22 rows; with the
   one-armed clamp it is 2.  `m = z0;` in place of `m = 0;` measures
   identically, so the zero arm is free either way.

   ======================================= WHAT IS LEFT, AND WHAT IT IS NOT

   LEVER 5's two words.  `f` needs a second set or update_equiv_regs doubles
   its live length and it cannot win $s0 (the arithmetic is below), and the
   cheapest second set is the tail's `f = cell | m; cell = f;` -- which puts
   the `or`/`sh` in $s0 where the target has $v0.  Measured on THIS body,
   all 92 words unless noted:

     * the tail storing direct, with no second set of `f` ...... 20 rows
     * the tail through a fresh `s32 z` ........................ 20 rows
     * the tail through `o` .................................... 20 rows
     * `f = ~f;` ahead of the tail, storing direct ............. 20 rows
     * `f = ~f;` moved into the THEN-arm instead, so the else-arm
       tail can use a temp ....................................... 7 rows
       -- it does give `f` its second set without the tail, but it
          breaks the then-arm, which was byte-identical
     * the same with the temp tail as well ............ 91 words / 11 rows

   So the second set has to be in the else-arm and has to be the tail, unless
   something gives `f` a second set in the then-arm WITHOUT moving the `nor`.
   **That is the one thing left here, and it is two words.**

   WHAT THE FUNCTION IS.  (x, y, flags) over the 6-byte map cells at
   `*(s32 *)(&D_80083160 + 0x1DC)`, row shift at +0x1F0.  `flags & 0x8832`
   non-zero CLEARS flags out of the cell's +0x04 halfword -- and when
   `flags & 0x800` only if func_800A6E10(x, y) < 2.  Otherwise it clears them
   AND ors back the saturated `(cell & flags) - (flags & 0x1100)`.

   ================================================================
   LEVER 1, worth 13 words: THE ROW SHIFT MUST BE A STRUCT MEMBER
   ================================================================
   `cse.c`'s invalidate_memory drops an entry only on
   `all || (nonscalar && p->in_struct) || cse_rtx_addr_varies_p (p->exp)`.
   The halfword store `*(u16 *)(c + 0x4) = ...` has a PLUS address, so
   note_mem_written sets only `nonscalar`; and the shift's own address folds
   to a constant, so `cse_rtx_addr_varies_p` is 0.  Read through a `u8 *` its
   in_struct is clear, the entry SURVIVES the store, cse keeps the whole cell
   index in a register and the SECOND cell access disappears entirely:
     *(s16 *)(w + 0x14), u8 *w ............ 78 words, 88 differing
     ((UnkDungeonMapHdr *)w)->unk_14 ...... 88 words, 86 differing
   `*(volatile s16 *)(w + 0x14)` also reaches 88, which is what proves the
   mechanism is the reload and not the struct type.
   (The same lever landed func_8008DBE8.)

   ===========================================================
   LEVER 2, worth 2 words: THE CALL RESULT NEEDS AN s32 + CAST
   ===========================================================
   func_800A6E10 is landed as `s16`, so `if (func_800A6E10(a, b) >= 2)` emits
   NO sign extension at all -- gcc trusts the SImode-promoted return.
   `s32 r = func_800A6E10(a, b); if ((s16)r >= 2)` is what puts the target's
   `sll 16`/`sra 16` back in front of the `slti`.

   =====================================================
   LEVER 3, worth 1 word: THE CLAMP IS A TWO-ARM if/else
   =====================================================
   `if ((s16)n < 0) m = 0; else m = n;` gives the target's
   `addu $a2, $a0, $zero` copy; the one-armed `if ((s16)n < 0) n = 0;` does
   not and is 90 words.  `m = n; if ((s16)n < 0) m = 0;` is 91 words on the
   current body as well, so the two-arm form is still the only one that holds
   92.

   =========================================================================
   LEVER 4 (NEW, 2026-11-16, j89), worth THE MISSING WORD AND 12 DIFFERENCES:
   THE THEN-ARM'S JOIN VARIABLE IS `index * 6`, AND THE ADDRESS IS A
   SINGLE-SET POINTER LOCAL IN THE JOINED BLOCK
   =========================================================================
   The old body bound `c = (u8 *)(... * 6 + t)` inside BOTH inner arms and
   shared only the store.  `c` was then set in two blocks, so flow made it
   global, global_alloc gave it ONE hard register for every site, and both
   arms' stores came out `sh $v0, 0x4($a1)` -- identical insns, so jump2
   cross-jumped them and the function was ONE WORD SHORT.  The target's two
   stores use opposite pairs (`sh $v1, 0x4($v0)` in the then-arm against
   `sh $v0, 0x4($v1)` in the else-arm) and cannot merge.

   What the target has is an `index * 6` joined between the two inner arms
   and the address computed ONCE after the join, as a pseudo local to that
   block, which local-alloc then folds onto the dying multiply temp ($v0):

        if (f & 0x800)  { ...; i = (a + (b << w->unk_14)) * 6; }
        else            { i = ((s16)x + ((s16)y << w->unk_14)) * 6; }
        c = (u8 *)(i + t);
        *(u16 *)(c + 0x4) &= ~f;

   Three nearby spellings and what each costs, all measured on this body:
     i = index * 6, `c = (u8 *)(i + t)` ...... 92 words  (this park)
     i = index,     `c = (u8 *)(i * 6 + t)` .. 91 words, and the loss is
       a DIFFERENT word: both inner arms then start with the same
       `sll $v0, $a0, 16` into the same hard register, and reorg.c fills the
       `beq`'s delay slot from BOTH threads at once and deletes the copy at
       the branch target.  The target keeps two slls (`sll $v0, $a0, 16` in
       the delay slot for the fall-through, `sll $v1, $a0, 16` at
       .L8009A454) precisely because their destinations differ.
     i = index, store written inline `*(u16 *)(i * 6 + t + 4) &= ~f` .. 91
       words, same cause.
   So the `* 6` must sit INSIDE the join and the `+ t` outside it: that both
   keeps the two slls distinct and makes the address block-local.  The
   else-arm must write all three cell accesses inline (no pointer local) for
   the same reason.

   ==========================================================================
   LEVER 5 (NEW, 2026-11-16, j89), worth the whole callee-saved rotation (16
   differences): `f` NEEDS A SECOND SET, OR update_equiv_regs DOUBLES ITS
   LIVE LENGTH AND IT CANNOT WIN A CALLEE-SAVED REGISTER
   ==========================================================================
   This is the park's old "SIX extra references are needed to take $s0" note,
   and that note was measuring the wrong knob.  The numbers are exact and
   come out of `.lreg`'s `dump_flow_info` header plus global.c's
   allocno_compare, whose priority is

        floor_log2 (n_refs) * n_refs * size / live_length

   and whose order decides $s0, $s1, ... among the five call-crossing
   allocnos.  On the pre-lever body:

     | pseudo | what          | refs | live_length | priority |
     |--------|---------------|------|-------------|----------|
     |  92    | (s16)y        |   3  |     10      |  0.300   |
     |  91    | (s16)x        |   3  |     13      |  0.231   |
     |  85    | t (cell base) |   4  |     63      |  0.127   |
     |  82    | f (flags)     |   7  |    112      |  0.125   |
     |  84    | &hdr          |   5  |     90      |  0.111   |

   -> y, x, t, f, hdr get $s0..$s4; the target wants f, y, x, t, hdr.
   `.greg` prints that order verbatim (`;; 11 regs to allocate: 88 136 80 92
   81 91 133 85 82 132 84`), so the table is checkable, not inferred.

   f's 112 is 56 DOUBLED.  local-alloc.c:1094 does
   `reg_live_length[regno] *= 2;` for any pseudo carrying a REG_EQUIV note,
   and a parameter copy carries one -- function.c:4014 attaches the incoming
   stack slot to `(set (reg 82) (reg $a2))`.  Undoubled f is 14/56 = 0.250,
   still under y's 0.300, so removing the doubling alone is NOT enough; but
   the gate at local-alloc.c:1043 is

        || reg_n_sets[regno] != 1

   so a SECOND SET skips the doubling AND adds a reference, and 8 refs crosses
   the floor_log2 step: 3*8/56 = 0.429, first of the five.  `.lreg` then
   prints `Register 82 used 8 times across 56 insns` and f takes $s0, y $s1,
   x $s2, t $s3 -- the target's exact assignment.  A dead second set does NOT
   work: flow deletes it before it is counted (`f = 0;` at the end of the
   else-arm measures identically to no lever at all, 32 differences).

   Placements measured, all at 92 words:
     f = *(u16 *)(cell) | m; *(u16 *)(cell) = f;  (this park) .. 15 differing
       -- costs exactly 2 words, `or $s0,...` / `sh $s0,...` for the target's
       `or $v0,...` / `sh $v0,...`
     the clamp assigns f instead of m (no `m` at all), with the else-arm's
       store moved ahead of `n = ...` ........... 19 masked / 13 aligned
       -- fewer aligned rows but 19 differing words, and it DISPLACES two
       insns (the `nor` is hoisted to word 54), so it is the worse residue
     f = ~f; *(..) = o & f;  in the else-arm .... 19 differing
     f = ~f;  in the then-arm .................. 91 words (the then-arm's
       store cross-jumps again and the delay slot duplicates)
     f = *(u16 *)(c+4) & ~f; in the then-arm .... 19 differing
     f = m; after the clamp .................... 24 differing (the copy is
       real: f and m conflict, so it is not coalesced away)
     a plain body copy `q = f;` with every use on q ... 30 differing; it
       removes the doubling (no REG_EQUIV on q) and lifts t to $s3, but cse
       rewrites the first test back to $a2, q keeps only 5 refs and
       2*5/57 = 0.175 is still under y's 0.300, so q lands in $s2.
     extra PRE-COMBINE references are NOT a usable substitute:
       `(f & 0x1000) | (f & 0x100)` for `f & 0x1100` is folded by cse, which
       runs BEFORE flow, so reg_n_refs stays 7 and nothing moves (32
       differing).  combine.c:56 says reg_n_refs is not re-adjusted, so a
       reference combine (not cse) removes WOULD count -- no spelling found
       that survives cse and dies in combine.

   ==========================================================================
   WHAT IS LEFT: 15 WORDS, SIX CALLER-SAVED QUANTITIES IN THE ELSE-ARM
   ==========================================================================
     here   target
     $v0    $a0     n   = (o & f) - (f & 0x1100)
     $a0    $v0     the `f & 0x1100` temp
     $a0    $a2     m   (it reuses the mask temp's register here)
     $a3    $t0     px
     $a2    $a3     py
     $s0    $v0     the final `or`/`sh` pair (LEVER 5's two words)
   `o` is $a1 and the cell address is $v1 in both, and the `nor`/`and` pair
   at words 65/66 is already byte-identical.

   The root cause is ONE pass and it is identified: local-alloc's
   `optimize_reg_copy_1` (local-alloc.c:704) eats the `m = n` copy.  At
   local_alloc time sched1 has already placed the copy ahead of the `nor`
   (`.sched` shows the emitted order 163 subu, 257 m=n, 184 nor, 187 and, 191
   sll), so the forward scan from the copy reaches the sign-test `sll`, finds
   `REG_DEAD n` on it, and rewrites it to read `m`.  `.lreg` proves it: the
   shift's source changes from `(reg/v 135)` = n in `.regmove` to `(reg/v
   136)` = m in `.lreg`.  That shortens n's quantity from four insns to two,
   lifts its QTY_CMP_PRI to 8/3 = 2.67 against the mask temp's 2/2 = 1.00, so
   local-alloc allocates n FIRST and it takes $v0; the mask gets $a0; m is
   then free to reuse $a0; and with $a0 and $a2 spoken for differently, px/py
   slide down to $a3/$a2 instead of $t0/$a3.  In the target the shift still
   reads n (`sll $a0, $a0, 16`, overwriting it), which is only possible if
   the scan was blocked.

   The scan breaks on `reg_set_p (src, p) || reg_set_p (dest, p)`, so a third
   set of `m` between the copy and the shift blocks it -- and it DOES work:
   `m = ~f; *(..) = o & m;` ahead of the clamp puts m in $a2 and px/py in
   $t0/$a3, exactly the target, but sched1 then hoists that `nor` to word 54
   and the net is 19 differing / 14 aligned.  Every other way of giving m a
   third set was measured and is worse: `m = o & ~f; *(..) = m;` is 93 words;
   `m = o; *(..) = m & ~f;` is 15; `m = 0;` first with a one-armed
   `if ((s16)n >= 0) m = n;` is 22.

   OTHER SPELLINGS MEASURED AND REJECTED on the current body (all 92 words
   unless noted), none of which moves the six:
     n = o & f; n -= f & 0x1100; .................... 15
     s32 k = f & 0x1100; n = (o & f) - k; ........... 15 (k before the store,
       to lengthen the mask's range: 18)
     n = -(f & 0x1100) + (o & f); ................... 18
     s32 sv = (s16)n; if (sv < 0) ................... 15
     if ((n << 16) < 0) ............................. 15
     if ((s16)n >= 0) m = n; else m = 0; ............ 22
     o declared s32 instead of u16 .................. 15
     m declared before n / px,py declared after o,n . 15 / 17
     the else-arm's store moved ahead of `n = ...` .. fixes px/py AND m by
       itself (the `nor` temp then occupies $a2 early) but hoists the `nor`
       to word 54; 19 differing
     f declared u16/u32/s16 ......................... conflicting types; the
       unit declares `void func_8009A3D0(s32, s32, s32);` and an s16/u16 f
       would need a mask the target does not have
     x/y declared s16 instead of (s16) casts ........ identical allocation

   PERMUTER TRIAGE: WORTH A SEARCH NOW, where the old 91-word residue was
   not.  The word count is right, the stream aligns 1:1 with no displaced
   insn, and all fifteen differences are register names produced by one
   named pass (`optimize_reg_copy_1`) whose trigger is a statement position.
   That is the shape docs/dungeon-sweep.md says the permuter wins.

   NEEDS the typedef below in the declaration block.    ====================================== 2026-11-20: THE 2 ROWS ARE THE PRICE
                                          OF LEVER 5, AND EIGHT WAYS OUT FAIL

   The two rows are `or s0,v0,a2' / `sh s0,4(v1)' against the target's
   `or $v0,$v0,$a2' / `sh $v0,0x4($v1)' -- so the final OR lands in `f''s
   callee-saved register here and in a temp there.  The target's shape for
   that statement is plainly a read-modify-write, `*(u16 *)(...) |= m;', with
   no variable on the left.

   WRITING IT THAT WAY COSTS 18 ROWS, and the 20 that result are ONE CYCLIC
   ROTATION of four callee-saved registers:

       clean   py=$s0  px=$s1  t=$s2  f=$s3
       target  f =$s0  py=$s1  px=$s2  t=$s3

   so `f' goes from first allocated to last.  Everything else matches.  Eight
   ways to keep f first WITHOUT making it the OR's destination, all measured
   at 92/92 words:

       f = *(...) | m; *(...) = f;      2   <- this park
       f carries m, then *(...) |= f;  10
       f = m; *(...) |= f;             12
       f = o & ~f; *(...) = f; |= m;   13
       f = ~f & o; *(...) = f; |= m;   13
       f = o & ~f; store f; f=...|m;   14
       o = *(...) | m; store o; f = o; 20
       *(...) |= m;                    20
       m = *(...) | m; store m; f = m; 22

   FREE REFERENCES ON `f' DO NOT HELP EITHER, and here they COST A WORD.  All
   six positions in the clean body that mention `f', times a do-while and a
   duplicated-arm `if', measure 24 to 49 rows at 93/92 or worse -- the
   opposite of this block's usual result.  The reason is visible in the body:
   every statement mentioning f is already a read-modify-write, so a
   duplicated arm cannot cross-jump and the device pays twice and earns
   nothing.

   So the second set of `f' is load-bearing and these two rows are its price.
   This is a permuter target, not a hand-sweep target.
*/

typedef struct UnkDungeonMapHdr
{
    u8 *unk_00;
    u8 pad_04[0x10];
    s16 unk_14;
} UnkDungeonMapHdr;
void func_8009A3D0(s32 x, s32 y, s32 f)
{
    u8 *v;
    UnkDungeonMapHdr *w;
    s32 t;

    v = (u8 *)&D_80083160;
    w = (UnkDungeonMapHdr *)(v + 0x1DC);
    t = *(s32 *)(v + 0x1DC);
    if (f & 0x8832)
    {
        s32 i;
        u8 *c;

        if (f & 0x800)
        {
            s32 a;
            s32 b;
            s32 r;

            a = (s16)x;
            b = (s16)y;
            r = func_800A6E10(a, b);
            if ((s16)r >= 2)
            {
                return;
            }
            i = (a + (b << w->unk_14)) * 6;
        }
        else
        {
            i = ((s16)x + ((s16)y << w->unk_14)) * 6;
        }
        c = (u8 *)(i + t);
        *(u16 *)(c + 0x4) &= ~f;
    }
    else
    {
        s32 px;
        s32 py;
        u16 o;
        s32 n;
        s32 m;
        register s32 z0 __asm__("$0");

        px = (s16)x;
        py = (s16)y;
        o = *(u16 *)((px + (py << w->unk_14)) * 6 + t + 4);
        n = (o & f) - (f & 0x1100);
        *(u16 *)((px + (py << w->unk_14)) * 6 + t + 4) = o & ~f;
        m = n + z0;
        if ((s16)n < 0)
        {
            m = 0;
        }
        /* The second set of `f` is LEVER 5: it is what keeps
           update_equiv_regs from doubling f's live length, and it is worth
           the whole callee-saved rotation.  It costs the two words of this
           `or`/`sh` pair, which come out of $s0 instead of $v0. */
        f = *(u16 *)((px + (py << w->unk_14)) * 6 + t + 4) | m;
        *(u16 *)((px + (py << w->unk_14)) * 6 + t + 4) = f;
    }
}
