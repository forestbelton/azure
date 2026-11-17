/* PARK -- func_8009C93C (ovl_dungeon).  263 of 263 words, 4 ALIGNED
   DIFFERENCES, and ALL FOUR ARE THE PROVED-UNREACHABLE `|= 0x80000000'
   GATE.  Agent j85, 2026-11-16, rewriting j64's park (9 rows); j64 rewrote
   j55's (15), j55 rewrote j49's (33).  j64's 9 was reproduced EXACTLY
   before anything here was changed, and j64's whole write-up below is
   kept because every table in it is still a valid measurement -- except
   where section (1) of the j85 notes says otherwise.

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
     ... pinned to $v1   ======================================== WHAT J85 ADDED, 9 ROWS -> 4

   FIVE OF J64'S NINE ROWS LAND: words 33, 35, 140, 144 and 145.  Four
   changes.  (1)+(2)+(3) are one unit -- any subset of them is 90 rows --
   and (4) only works on top of them.

   (1) THE DELAY-SLOT COPY AT WORD 144 IS AN UNCONDITIONAL `k = t;' JUST
   BEFORE THE `if'.  j64's cse analysis is right that no *conditional*
   `k = t' survives, but it is the wrong question.  cc1psx fills MIPS
   delay slots itself, in `reorg', FROM THE INSN THAT PRECEDES THE BRANCH;
   `out.s' reads

       andi $4,$2,0xffff ; lbu $2,3($19)
       .set noreorder
       beq $2,$0,$L16    ; move $17,$16
       .set macro

   so word 144 is simply the statement written before the `if'.  It is
   also why word 142 is a `nop': once the copy is pulled into the slot,
   `lbu $v0, 0x3($s3)' is immediately followed by `beqz $v0' and the
   load-hazard nop appears.  No cse or scheduling argument is involved.
   READ THE `.set noreorder' BLOCKS IN out.s BEFORE THEORISING ABOUT A
   DELAY SLOT -- they say outright which insn the compiler put there.

   (2) BUT `k = t;' MAKES `k = a[0x84]' A DEAD STORE, so word 137's `lbu'
   moves into `t' and word 139 becomes a `nop'.  An input-only
   `__asm__("" ::"r"(k));' between `t = k;' and the call keeps it live for
   zero words, and then words 137..149 are the target's own insns, every
   remaining row register-only.

   (3) THE PRICE IS ONE EXTRA REFERENCE ON `k', PAID WITH A TRAILING
   FENCE.  With (1)+(2) alone the residue is 90 rows of pure register
   permutation: `a'/`b'/`e' 3-cycle around $s1/$s2/$s3 and `k' lands in
   $a1.  `allocno_compare' ranks by floor_log2(refs)*refs/live_length, so
   one extra reference lifts `k' past the three parameters.
   `__asm__("" ::"r"(k));' immediately before the FINAL `return e;'
   lengthens `k's live range to the end of the function -- the denominator
   grows faster than the numerator -- and `k' drops back below them:
   a=$s3, b=$s1, e=$s2, t=$s0, k=$s1 (sharing with `b', disjoint ranges).
   THE FENCE MUST BE ON `k', AT THE END.  A fence on `a', `b' or `e'
   leaves the 3-cycle untouched and any fence on `t' costs a word.  -> 8.

   (4) SITE 1'S CALL RESULT NEEDS ITS OWN `r1' *AND* A FENCE ON IT.  READ
   THE ASSEMBLER MACRO, NOT THE MNEMONIC: cc1psx emits `rem $3,$4,$2' for
   `r % a[3]' and gas expands it to `div $zero,$4,$2 / mfhi $3'.  The
   target's words 145/146 are `div $zero,$v1,$v0 / mfhi $v1', i.e.
   `rem $3,$3,$2' -- THE MASK AND THE REMAINDER SHARE $v1.  The shared `r'
   can never get $v1, because at SITE 3 the target itself puts e[3] in $v1
   (word 218, `lbu $v1, 0x3($s2)') while the mask is live; the `greg' dump
   records `88 conflicts: ... 2 3 ...' for exactly that reason.  Site 3's
   mask IS $a0 in the target (word 221), so the two call sites have
   different holders and one variable cannot be both.

   A BARE SPLIT IS 14 ROWS AND A WORD LONG, and the reason is worth
   keeping: it moves the `andi' AFTER the `lbu'/`beqz' pair, the call's
   return value is then live across the a[3] load, that load loses $v0 to
   $v1, and the SECOND a[3] load -- expand really emits two, insns 340 and
   345 in `.sched', and they collapse only when both get $v0 -- survives
   as `move $v0, $v1'.  An input-only `__asm__("" ::"r"(r1));' IMMEDIATELY
   AFTER `r1 = func_800A6D30();' holds the `andi' in front of the load,
   everything re-shares $v0, and 140/145 land.  -> 6.

   (5) WORDS 33-35 ARE A SOURCE-ORDER PROBLEM, NOT A sched2 TIE.  j64's
   LUID reading is right about the mechanism and wrong about the cure: the
   `andi' needs the HIGHER luid, and the way to give it one is to put two
   statements in front of it, in this order.  The target emits
   `srl $v0, $a1, 8' / `lui %hi' / `addiu %lo' / `andi $s0, $v0, 0xE', so
   the shift must be its own statement into a FRESH short-lived local
   (which local_alloc gives $v0, as the target has), the first symbol must
   be materialised into a user pointer local between the two halves, and
   the `andi' is what is left:

       sh = w >> 8;
       p = (u8 *)D_800DCEAC;
       i = sh & 0xE;
       func_800C78A0(a - 0x20, b[0x24] * 64 + (*(s16 *)(p + i) >> 1) + 0x20,
                     b[0x25] * 64 + (*(s16 *)((u8 *)D_800DCEBC + i) >> 1) ...

   Each of the three parts is load-bearing.  `i = w >> 8; p = ...;
   i &= 0xE;' reuses one pseudo for both halves and gives
   `andi $s0, $s0, 0xE' (13 rows); the SECOND symbol must stay inline,
   because it is already after the `andi' in both streams and hoisting it
   too is 14 rows.  -> 4.

   ---- measured and rejected, on j64's 9-row base ----
     `k = t;' before the `if', no fence                 263 / 90
     + `__asm__("" ::"r"(k));' before the call          263 / 90
     + the trailing fence on `k'                        263 /  8   <== (3)
     `k = t;' INSIDE the `if', before the add           263 /  9
       -- combine merges it; byte-identical to the base
     `if'/`else' arms for `k', with the `k' fence       265 / 96
     one fence `__asm__("" ::"r"(k), "r"(t));'          265 / 93
     a fence on `t' after the copy / twice              264 / 91 both
     a fence on `t' before `k = a[0x84]'                265 / 103
     a fence on `a' / on `b' / on `e'                   263 / 90 each
   ---- on the 8-row base, for words 140/145 ----
     a different HOLDER for site 1 in place of `r':
       `t2' 13,  `t3' 15,  `m' 10,  `i' 17
     a fresh `r1', declared in each of the 11 slots  264 / 12 -- IDENTICAL
       in all eleven: declaration position is not a tie-break here either
     SITE 3 split to a fresh `r2' instead, 4 slots   264 / 12 all
     site 3 split to `t2' / `t3' / `i' / `m'    264 / 16/18/20/17
     mask and remainder one variable, `r %= a[3]':
       shared `r' 11,  own `r1' 264/12,  `r' + site 3 split 264/12,
       `t2' 15,  `t3' 17,  `m' 12,  `i' 19
     one a[3] local `q = a[3]' with `if (q != 0)':
       shared `r' 11,  split `r1' 13,  split + fence 13,
       split + `r1 %= q' 13
     a fence on `r' after site 1's set, once / twice    263 /  8 both
     a fence on `r' at site 3                           262 / 17
     a trailing fence on `r'                            263 / 10
   ---- on the 6-row base, for words 33/35 ----
     `p = (u8 *)D_800DCEAC;' before `i', `p + i'        263 /  6
       -- lui/addiu precede the andi, but the `srl' goes with them
     the same, reusing `g' as the base local                  11
     `__asm__("" ::"r"((u8 *)D_800DCEAC));' before `i' 265 / 13
     ... both symbols that way                         267 / 15
     both symbols in pointer locals                    263 / 14
     then, on that `p' base:
     `i = w >> 8; p = ...; i &= 0xE;'                   263 / 13
     the same with `i = i & 0xE'                              13
     `m = w >> 8; p = ...; i = m & 0xE;'                      12
     `sh = w >> 8; p = ...; i = sh & 0xE;' FRESH LOCAL  263 /  4  <== (5)
       -- all four declaration slots tried: 4 rows in every one
     drop `i', inline `(w >> 8) & 0xE' at all four uses         6

   ---------------------------------------------------------------- residue

   FOUR ROWS, ALL ONE CLUSTER: words 19/21/22/23, the `|= 0x80000000'
   gate, a straight `$v0'/`$v1' swap on the loaded value and the constant
   (`lw $v1' + `lui $v0' + `or $v0,$v1,$v0' where the target has `lw $v0'
   + `lui $v1' + `or $v0,$v0,$v1').  docs/matching-levers.md's "A
   PROVED-UNREACHABLE RESIDUE: THE `|= 0x80000000' GATE'S `$v0'/`$v1'
   SWAP" says treat it as permanent "unless someone finds a
   zero-instruction `$v0' occupant".  J85 DID NOT FIND ONE, BUT DID FIND
   THE SPELLING THAT PRODUCES THE TARGET'S REGISTERS, AND IT COSTS ONE
   WORD -- which sharpens the entry considerably:

       if ((*(s32 *)(a + 0x1C) & 0x400) != 0
           && *(volatile s32 *)(a + 0x14) >= 0)

   A VOLATILE MEM IS `do_not_record' IN cse, so the `|='s own read is NOT
   folded into the test's pseudo.  Both loads are then BLOCK-LOCAL, both
   get $v0, `block_alloc' ties the `ior's destination to the body's load
   instead of to the constant, the constant is pushed to $v1, and words
   19/21/22/23 ALL MATCH -- `lw $v0,20($19) / nop / bltz $v0 / nop /
   lui $v1 / or $v0,$v0,$v1'.  THE ONE EXTRA WORD IS THE `bltz' DELAY
   SLOT: with the duplicate load first in the fall-through thread, reorg
   emits no `.set noreorder' block at all for this branch (confirmed in
   out.s), gas then has only the volatile `lw' -- which sets the register
   the branch reads -- as a slot candidate and emits a `nop', and the
   duplicate load is removed later.  264 words / 7 rows.

   So the missing piece is EXACTLY ONE THING: a way to keep the body's
   read a separate block-local pseudo without a trapping insn at the head
   of the fall-through thread.  Measured and rejected here:
     `*(volatile s32 *)(a + 0x14) >= 0' in the test       264 /  7
       -- registers all correct; the slot is the only loss
     + `cc = 0x80000000;' hoisted before the `|='         264 /  7
     + the same with a matched-operand fence on `cc'      265 / 11
     + `m = 0x80000000;'                                  264 / 11
     `0x80000000 | *(s32 *)(a + 0x14)' with the volatile  264 /  7
     `*(volatile s32 *)(a + 0x14) |= 0x80000000;'         266 / 11
     `= *(volatile s32 *)(a+0x14) | 0x80000000'           265 /  8
     `*(volatile s32 *)(a+0x14) = *(s32 *)(a+0x14) | ...' 264 /  9
   and, all NON-volatile, all byte-identical to this body at 4 rows:
     nested `if's in place of the `&&'
     `= *(s32 *)(a + 0x14) | 0x80000000' as a plain store
     `0x80000000 | *(s32 *)(a + 0x14)'
     `do { } while (0)' around the `|=', and around the whole inner `if'
     a `vv' local for the test's value, with nested `if's
     `vv' + an input-only fence on `vv'
     `cc = 0x80000000;' with a matched-operand fence on `cc'
     `cc = 0x80000000;' with an input-only fence on `cc'
   and worse:
     `__asm__("" ::: "memory");' before the `|='          265 /  8
     `__asm__("" ::"r"(a));' before the `|='              264 /  7
     `vv' + a MATCHED-OPERAND fence on `vv', nested ifs   265 /  8
       -- two `lw's both survive here; only the volatile form collapses
     a `pp = a' copy with a matched-operand fence, test
       through `pp + 0x14'                                265 /  7
     `cc = 0x80000000; *(s32*)(a+0x14) |= cc;' (folded)   263 /  4
     the same holding the constant in `m' / in `r'        263 /  6 both

   Permuter food, and a narrow target: four rows in one five-insn block
   whose instruction stream is already exact. */

u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e)
{
    s16 k;
    s32 i;
    s32 sh;
    s32 r;
    s32 r1;
    s32 t;
    s32 t3;
    s32 t2;
    s32 m;
    register u8 *zz __asm__("$4");
    u8 *g;
    u8 *p;
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
    sh = w >> 8;
    p = (u8 *)D_800DCEAC;
    i = sh & 0xE;
    func_800C78A0(a - 0x20, b[0x24] * 64 + (*(s16 *)(p + i) >> 1) + 0x20,
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
    __asm__("" ::"r"(k));
    r1 = func_800A6D30();
    __asm__("" ::"r"(r1));
    k = t;
    if (a[3] != 0)
    {
        k = t + (r1 % a[3]) * 4;
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
    __asm__("" ::"r"(k));
    return e;
}
