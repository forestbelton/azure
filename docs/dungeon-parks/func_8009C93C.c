/* PARK -- func_8009C93C (ovl_dungeon).  263 of 263 words, 9 ALIGNED
   DIFFERENCES (ovlverify reports 10 masked; one row is a transposed pair
   and misses twice).  Agent j64, 2026-11-16, rewriting j55's park (15 rows);
   j55 rewrote j49's (33 rows).  j55's 15 was reproduced EXACTLY before
   anything here was changed.

   SIGNATURE (unchanged from j55, and NOT changed further here):
     u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);
   The FOURTH parameter is a u16, not an s32 -- the prologue lever is worth
   5 rows.  The fifth argument arrives at 0x40($sp).  The five call sites
   ride an IMPLICIT declaration today.  j55 MEASURED THE DECLARATION
   AGAINST ALL FIVE ENCLOSING FUNCTIONS and none of them moves:
   func_8008C8BC 121/121, func_800973F0 59/59, func_80098864 47/47,
   func_800AC010 284/284 and func_800CA788 109/109, all still 0 differing.

   ----------------------------------------------- WHAT J64 ADDED, 15 -> 9

   THREE CHANGES, AND TWO OF THEM ONLY WORK TOGETHER.

   (1) THE `(X & 2^n) != 0' REWRITE IS expr.c's, AT THE TREE LEVEL, AND NO
   REPHRASING REACHES IT.  `do_store_flag' (expr.c, the block just before
   `if (! can_compare_p (operand_mode))') turns any comparison whose
   `TREE_CODE (arg0) == BIT_AND_EXPR && integer_zerop (arg1) &&
   integer_pow2p (TREE_OPERAND (arg0, 1))' into a SHIFT plus `and 1'.  It
   is unconditional -- it never consults `nonzero_bits', so j55's note that
   the `andi 0xFFFF' mask's nonzero_bits caused `(r >> 1) & 1' is wrong --
   and `fold' normalises every obvious rewording back onto that same tree.
   Measured byte-identical here, on top of j55's four: `((r & 1) != 0) +
   ((r & 2) != 0)', `(r & 1) + ((r & 2) ? 1 : 0)', `(r & 1) + ((r & 2) ==
   2)', `(r & 1) + !((r & 2) == 0)'.  `(r & 2) > 1' and `(r & 2) >= 2' are
   31.  THE CURE IS TO MAKE THE AND SOMETHING OTHER THAN THE COMPARISON'S
   OWN OPERAND -- bind it to a local, so `arg0' is a VAR_DECL and
   do_store_flag falls through to the scc path:

       t2 = (k & 1) + ((m = k & 2) != 0);

   `m' never survives; combine folds it straight back into the `ne'.  The
   EMBEDDED assignment also fixes the order -- written as a separate
   statement (`m = k & 2;' first) the `andi 0x2' is emitted before the
   `andi 0x1' and that costs a row.  (This is the same cure as
   docs/matching-levers.md's "THE `flags & MASK' COMBINE REWRITE FIRES ON A
   PLAIN BOOLEAN TEST"; that entry names combine as the pass, and the pass
   is expr.c.)

   (2) THE MASK-FREE MOVE NEEDS A 16-BIT MULTI-SET LOCAL, AND `k' IS THE
   ONLY ONE.  `s32 r = func_800A6D30()' is a u16-to-int conversion and the
   `andi 0xFFFF' is a real insn; the target has a plain `addu $s1, $v0,
   $zero'.  A 16-bit destination removes the mask, but a FRESH one has a
   single set, combine folds the copy into its use and the move vanishes
   too (`u16 r2' is 262 words / 32).  Reusing `k' -- 16-bit, many sets --
   gives exactly the target's move.

   (3) BUT `k' ALONE IS 29, AND THE DIFFERENCE IS ALLOCNO ORDER.  `k' and
   `t' both want $s0; `global_alloc' sorts by
   floor_log2(refs)*refs/live_length and hands out the lowest free hard
   register, so whichever is allocated FIRST takes $s0.  Three extra refs
   on `k' lift it past `t' and the two swap -- 14 rows of cascade, every
   one a plain $s0/$s1 exchange.  PAY FOR IT ON THE OTHER SIDE: split `t'
   so the accumulator block gets its own `t2'.  That halves old `t's live
   length, lifts it back above `k', and `t'/`t2'/`i'/`t3' share $s0 while
   `k' keeps $s1 -- the target's assignment.  Splitting `t' WITHOUT the `k'
   reuse is 92; the two changes are only correct together.  (12 rows here.)

   (4) THE OUTGOING ARGUMENT COPY DEVICE, worth the last 3 rows.  With
   12 rows left, `addu $a0, $s3, $zero' sat immediately before
   `jal func_800981F8' and reorg took it for the delay slot, while the
   target schedules it at word 176 and puts `addu $s0, $s0, $v0' in the
   slot.  This is EXACTLY docs/matching-levers.md's "AN OUTGOING ARGUMENT
   COPY CANNOT BE MOVED EARLIER FROM THE SOURCE -- IT TIES AT sched2
   AGAINST ... AND THE TIE BREAKS ON LUID, WHICH `calls.c' FIXES":
   `calls.c' emits register parameters at the very END of the call's
   expansion, so the copy always carries the highest LUID in the block and
   `rank_for_schedule's inverted comparator places it last.  The entry's
   device applies verbatim and lands it:

       register u8 *zz __asm__("$4");
       ...
       zz = a;
       __asm__("" : "=r"(zz) : "0"(zz));
       t2 = (k & 1) + ((m = k & 2) != 0);
       t2 += (a[0x26] + func_800981F8(zz)) * 2;

   THE HARD-REGISTER PIN IS LOAD-BEARING HERE: the same device with a
   plain `register u8 *zz' is 264 words / 15.

   Measured and rejected on this base, 263 words unless noted:
     reuse `t3' for the call (s32, 2 sets)        19  -- lands $s1, mask stays
     reuse `w' (u16, 3 sets)                      30  -- mask gone, $a1 not $s1
     reuse `dist' (u16 parameter)                 42
     reuse `i'                             265 / 48
     `u16 r' for all three call sites      264 / 36
     three separate `s32 r1/r2/r3'                20
     three separate `u16 r1/r2/r3'         260 / 38
     `u16 r2' shared by sites 1+2          264 / 32
     `u16 r2' shared by sites 2+3          261 / 41
     `m' temp with `s32 r' kept                   30
     `m' temp + `u16 r2'                   262 / 32
     `m' temp + `w' / + `t3'                   33 / 33
     `(t3 = r & 2) != 0' / `(i = ...)' / `(g = ...)' /
       `(w = ...)' / `(k = r & 2) != 0'   34/37/33/29/29
     declaration position of `m', all 8 slots     29 -- not a tie-break
     new `t2' for the accumulator, `r' kept       92
     `k = t;' between the call and the `if'       96
     `t = a[0x84]; k = t;' (lbu into `t')         14
     `k = a[0x84]; t = k; k = t;'                 14
     accumulator as ONE expression with the
       func_800981F8 call inside it         267 / 103
     `((m = k & 2) != 0) + (k & 1)'               12 -- byte-identical at 12
     `t2 = t2 + 2 * (a[0x26] + ...)'              12 -- byte-identical at 12
   And, on the 9-row body, aimed at words 140/144/145:
     `__asm__("" : "=r"(k) : "0"(t))' after the call   94 -- the copy DOES
       appear, but `k' lands in $a1 and the whole allocation cascades
     the same, before the call                         13
     `k = t; __asm__("" : "=r"(k) : "0"(k));'          94
     `register s16 k __asm__("$17")' pin alone         33
     the pin plus either copy device                33 / 36
     site 1 split into its own `s32 r1'               14
     ... pinned to $v1, no fence                      14
     ... pinned to $v1 WITH the fence                 10 -- see below
     ... that, plus `r1 %= a[3]; k = t + r1 * 4;'     10

   ---------------------------------------------------------------- residue

   FOUR OF THE 9 ARE THE PROVED-UNREACHABLE `|= 0x80000000' GATE, words
   19/21/22/23 -- docs/matching-levers.md, "A PROVED-UNREACHABLE RESIDUE:
   THE `|= 0x80000000' GATE'S `$v0'/`$v1' SWAP".  Not re-measured here;
   j55 spent four byte-identical spellings on it.

   TWO are words 33-35: the target emits `lui %hi(D_800DCEAC)' / `addiu
   %lo' and THEN `andi $s0, $v0, 0xE'; this emits the `andi' first.  IT IS
   NOT A sched1 TRANSPOSITION, as j55's park said.  `sched.py' shows sched2
   reproducing the order it was handed: in basic block 4, the ready list at
   T-32 is {84 (the `lo_sum'), 75 (the `andi'), 134} with equal priority
   AND equal ref_count, so `rank_for_schedule' falls through to INSN_LUID,
   whose inverted comparator picks the HIGHER luid first and places it
   LAST.  The `andi' has the lower luid because expand emits `i = ...'
   before it materialises the symbol address.  To match, the `andi' would
   need the HIGHER luid.  Measured and rejected here, all byte-identical:
   inlining the index into the first subscript; `(i = (w >> 8) & 0xE)'
   assigned INSIDE the subscript; dropping `i' entirely and writing
   `(w >> 8) & 0xE' at all four uses.  j55 measured `i = (w & 0xE00) >> 8'
   at 22 and a separate `g = (u8 *)D_800DCEAC' pointer local at 23.  This
   is the same family as the argument-copy tie that (4) above cures, so the
   fix is probably a fence device on the `andi', not a rephrasing.

   THREE are words 140/144/145: the target's mask lands in `$v1' and an
   extra `k = t' copy fills the `beqz $v0, .L8009CB94' delay slot, where
   this body leaves a `nop' and puts the mask in `$a0'.  AN EXPLICIT
   REDUNDANT `k = t;' IS DELETED BY cse -- after `t = k' the two pseudos
   are in one equivalence class, and the CALL between them does not break
   it, because `invalidate_for_call' invalidates hard registers and memory,
   not pseudos.  So every placement of it is an allocation change and never
   the wanted copy: 27/28 on j55's base (four placements), 96 on this one;
   `if'/`else' arms for the assignment are 96.  The target has THREE insns
   here -- `lbu $s1, 0x84($s3)', `addu $s0, $s1' in the jal slot, `addu
   $s1, $s0' in the beqz slot -- and every spelling with only TWO (lbu plus
   one copy) leaves the beqz slot a nop, which is what this body does.
   The copy device `__asm__("" : "=r"(k) : "0"(t))' DOES produce the copy
   and cse cannot delete it, but it also drags `k' into $a1 and costs 94.

   THE MASK HALF OF THIS CLUSTER IS SOLVED AND THE SOLUTION COSTS MORE THAN
   IT SAVES, which is worth knowing before anyone spends a run on it.
   Splitting site 1's call result into its own `register s32 r1
   __asm__("$3")' with a `__asm__("" : "=r"(r1) : "0"(r1))' fence puts the
   mask in `$v1' and the `div' reads `$v1' -- words 140 and 145 both match
   -- but `$v1' is then still r1's when `mfhi' runs, the quotient goes to
   `$v0', and words 146/148/149 break instead.  Net 10, one worse than this
   body.  Reusing r1 for the quotient (`r1 %= a[3]') does not free it
   either: also 10.

   TRIAGE: 9 rows, four of them documented permanent.  The other five are
   two independent single-decision ties -- one sched2 LUID order, one cse
   deletion -- inside blocks whose instruction streams already match
   exactly.  Permuter food. */

u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e)
{
    s16 k;
    s32 i;
    s32 r;
    s32 t;
    s32 t3;
    s32 t2;
    s32 m;
    register u8 *zz __asm__("$4");
    u8 *g;
    u16 w;

    w = ang;
    if (e == NULL)
    {
        if ((*(s32 *)(a + 0x1C) & 0x400) != 0 && *(s32 *)(a + 0x14) >= 0)
        {
            *(s32 *)(a + 0x14) |= 0x80000000;
            w = ang + ((func_800A6D30() & 7) << 9);
            *(s16 *)(a + 0x2A) = w;
        }
    }
    i = (w >> 8) & 0xE;
    func_800C78A0(a - 0x20, b[0x24] * 64 + (*(s16 *)((u8 *)D_800DCEAC + i) >> 1) + 0x20,
                  b[0x25] * 64 + (*(s16 *)((u8 *)D_800DCEBC + i) >> 1) + 0x20,
                  *(s16 *)(a + 0x88), 8, 0x300);
    *(u8 **)(a + 0x60) = e;
    *(s32 *)(a + 0x14) &= ~0x4000000;
    if (e == NULL)
    {
        if (func_8009A540((*(u16 *)(a + 0x2A) >> 9) & 7, b[0x24], b[0x25],
                          *(u16 *)(a + 0x88) - 0x20) == 0)
        {
            return NULL;
        }
        e = func_8009B25C(a, b[0x24] + *(u16 *)((u8 *)D_8006CCD8 + i) * dist,
                          b[0x25] + *(u16 *)((u8 *)D_8006CCE8 + i) * dist,
                          *(s16 *)(a + 0x88));
        *(u8 **)(a + 0x60) = e;
        if (e == NULL)
        {
            return NULL;
        }
    }
    *(u16 *)(e + 0x6A) = *(u16 *)(a + 0x2A);
    if (e[0x13] != 0)
    {
        if ((*(s32 *)(e + 0x1C) & 0x238) == 0 && (*(u16 *)(e + 0x46) & 0x8000) == 0)
        {
            *(u16 *)(e + 0x2A) = *(u16 *)(a + 0x2A) + 0x800;
        }
    }
    *(s32 *)(a + 0x14) |= 0x4000000;
    *(s32 *)(e + 0x14) = (*(s32 *)(e + 0x14) | 0x20000) & ~0x1000000;
    k = a[0x84];
    t = k;
    r = func_800A6D30();
    if (a[3] != 0)
    {
        k = t + (r % a[3]) * 4;
    }
    if (e[0x13] == 0 && *(u8 **)(e + 0x50) != NULL && **(u8 **)(e + 0x50) == 2)
    {
        k -= k >> 2;
    }
    if ((*(s32 *)(e + 0x1C) & 0x238) == 0 && k < 0x80)
    {
        return e;
    }
    k = func_800A6D30();
    zz = a;
    __asm__("" : "=r"(zz) : "0"(zz));
    t2 = (k & 1) + ((m = k & 2) != 0);
    t2 += (a[0x26] + func_800981F8(zz)) * 2;
    *(s16 *)(a + 0x20) = t2;
    if ((*(s32 *)(a + 0x1C) & 0x1000) != 0)
    {
        k = a[3] >> 1;
    }
    else
    {
        k = a[3] >> 6;
    }
    t3 = k;
    if (t3 >= (s32)(func_800A6D30() & 0x7F))
    {
        *(s32 *)(a + 0x1C) |= 0x1000000;
    }
    else
    {
        *(s32 *)(a + 0x1C) &= ~0x1000000;
    }
    if ((*(s32 *)(e + 0x1C) & 0x238) != 0)
    {
        r = func_800A6D30();
        if (e[3] != 0)
        {
            k = r % e[3];
        }
        else
        {
            k = 0;
        }
        if ((s16)(k + e[0x70]) >> 7 > 0)
        {
            return e;
        }
    }
    *(s32 *)(a + 0x14) &= ~0x4000000;
    g = D_800E3D7C;
    *(s32 *)(e + 0x14) &= ~0x20000;
    if (a == g)
    {
        func_8003E188(e[0x13], 0);
    }
    return e;
}
