/* ovl_dng_s04's 0x112/0x312 debris-spawn director and the SPAWNER that
   installs func_s04_8002498C as the +0x10 step at three sites. A COMPARE-CHAIN
   switch (jtcheck reports 0 tables for this unit) on *(s16 *)(p + 0xA) over
   cases 0, 1 and 2, balanced on value so the emitted tests are 1, then < 2,
   then == 0, then == 2; arms written ascending.

   610 target words, 610 candidate words, 15 aligned differences (ovlverify
   also 15, so the build is not stale). PARKED. One-member family.

   Before the switch it reads the actor node `s' at p + 0x0, the node's -0x14
   record `o', the -0x20 window `w', the facing pair out of
   D_8006CCD8 / D_8006CCE8, and runs the p + 0x50 timer down one into a LOCAL
   `t'. State 0 gates on the p + 0x4 record's bit 0x80, publishes w + 0x8's
   position into b and b's two velocity words from the facing pair shifted
   left 19, then registers the party through func_800A05A4; on a NULL result
   it walks the facing ray with func_800A4688 until the ray leaves the room or
   func_800BCB04 reports an angle past 0x200, and on a non-NULL result it
   copies the found record's whole UnkVec and takes the grid delta to it.
   Either way it then spawns a j x h grid of 0x112 nodes, one 0x112 node
   carrying the saved UnkVec, and 41 jittered 0x312 nodes, all running
   func_s04_8002498C, and advances to state 1. State 1 advances b while the
   timer is above 0x16 and ends with func_8009CE1C; state 2 is the teardown.

   DONOR: csrc/ovl_dng_s06.c func_s06_80024004 (505 words), named by
   `dngcallee' at 12 of 15 symbols with a CLEAN GAP to the 10/15 tier.
   `dngtwin' was not needed. The donor's save LIST is identical -- s0..s7, fp
   and ra, ten saves -- but its frame is -0x58 against this body's -0x78, and
   this body SPILLS its `p' parameter to the incoming argument slot
   0x78($sp) and re-reads it eleven times, where the donor keeps p and b in
   $s6/$s7. So every BUDGET fact of the donor's is void here: this relink has
   0x50 bytes of locals against the donor's 0x30 and no spare callee-saved
   register at all. Of the donor's nine priced facts, FACT 1 (the facing index
   must be a variable, tell: the shift lands in a callee-saved register)
   carried and is load-bearing here too; the rest are about statements this
   relink does not have.

   DECLARATIONS
   void func_800478B8(u8 *);
   s32 func_8004491C(UnkNode *, u32);
   UnkNode *func_8003FD64(s32, void *);
   void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
   s32 func_800A05A4(u8 *, s32, s32, s16, s32);
   s32 func_800A3820(s16);
   s32 func_800A4688_s(u16, u16, s16, s16, s32) __asm__("func_800A4688");
   s32 func_800A56E0(s32);
   s32 func_800BCB04(s32, s32, s16);
   s32 rand(void);
   void func_s04_8002498C(u8 *p, UnkVec *b, u8 *c, u8 *d);
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern s32 D_8008346C;
   extern u8 D_80083498[];
   extern u32 D_800814A0;
   extern u8 D_800DE938[];
   extern u8 D_800DED28[];
   END DECLARATIONS

   THE RESIDUE, and it is ONE cluster. All 15 rows sit between candidate words
   281 and 298, inside the nested 0x112 spawn loop. The target materialises
   the func_s04_8002498C address and stores it as n->unk_10 in three
   CONTIGUOUS words at 277-279, i.e. the first thing in the `if (n != NULL)'
   body; this candidate emits the `lui'/`addiu' pair at 281-282 and the `sw'
   at 284, separated by `ox's `addiu'. The 3-word displacement then exchanges
   `ox' and the n->unk_08 pointer between $a0 and $v1 for the remaining 12
   rows. Nothing is moved, no word is gained or lost, and every other word in
   the body is exact.
   CAUSE, stated as ARITHMETIC: a SYMBOL_REF costs no insn at expand time, so
   `(u32)func_s04_8002498C' gets its register-forcing insn -- and therefore
   its INSN_LUID -- at the point of use rather than at the statement, and
   sched1 breaks the tie against the `mult'/`mflo' group on LUID. Three source
   orderings of that statement were measured and ALL THREE give 610 / 15
   (below), so the placement is NOT reachable from statement order; the next
   thing to try is giving the symbol a VAR_DECL of its own inside the loop
   body so it has a LUID at all, which is the mechanism the levers file
   records for a hoisted table base.

   EVERY FIGURE BELOW IS ONE-AT-A-TIME FROM THIS 610-WORD BODY, with the file
   restored and diffed back to byte-identical between edits, except where a
   row is explicitly marked as taken mid-fight.

   HELD -- facts that are load-bearing, each priced from this body:

   1. THE DELTA PAIR AND THE TWO LOOP COUNTERS ARE ONE PAIR OF VARIABLES --
      26 rows, the single largest fact on this body.
      `j' and `h' hold the grid delta, then the abs/max result's two branches
      assign them 0/3 or 1/0, and then they ARE the outer and inner loop
      counters. Written with separate `u'/`g' counters it is 610 words / 61
      aligned differences, with i/u exchanged between $s4 and $s5 and ay
      displaced from $s6 to $s3 across the whole body. The tell is in the
      listing before any compile: $s4 is written by the delta's `subu' AND by
      the counter's `addiu $s4, $s4, 1', and $s3 likewise, so the two roles
      share one allocno and therefore one source variable.
   2. THE FACING INDEX IS THE SAME VARIABLE AS THE OUTER COUNTER -- 8 rows,
      and it is the donor's FACT 1 reproducing.
      `j = *(u16 *)(s + 0x2A) >> 8' with the two table reads spelled
      `*(s16 *)((j & 0xE) + (s32)D_8006CCD8)' is 0 on this cluster; the same
      shift written into a variable that is NOT the counter (`i') is
      610 / 27. The tell is the donor's: `srl $s4, $v0, 8' puts a value that
      dies two instructions later into a CALLEE-SAVED register, which is only
      possible if the shift's pseudo is the counter's.
   3. THE FIRST LOOP'S TWO COORDINATES ARE ALSO `j' AND `h' -- part of the
      same 8 rows as fact 2; they are one measurement because both are the
      `everything funnels through two variables' change. Separate `x'/`y'
      locals put the coordinates in $a0/$a1 where the target has $s4/$s3.
   4. THE else-BRANCH RECORD POINTER MUST NOT BE THE DRESS BLOCKS' `q' --
      10 rows. `m = (u8 *)n->unk_0C' in the r != 0 arm is 0; spelling it `q',
      the same local the three dress blocks use, is 610 / 71 and puts it in
      CALLEE-SAVED $s1 where the target has caller-saved $v1. This is the
      exact inverse of fact 5, 40 words apart, so neither direction is a
      house style -- read the register CLASS off the listing at each site.
   5. THE r != 0 ARM'S NODE POINTER IS THE SAME VARIABLE AS THE SPAWN LOOPS'
      `n' -- 61 rows, the second largest fact.
      `n = (UnkNode *)((u8 *)r - 0x20); v = *n->unk_08; m = (u8 *)n->unk_0C;'
      is 0. With a separate `m = (u8 *)r - 0x20' local for that arm it is
      610 / 149, a clean n/q exchange ($s0/$s1) across every one of the three
      spawn blocks. The tell: the target's `addiu $s0, $v0, -0x20' puts this
      arm's pointer in the SAME register the spawn loops use for `n'.
   6. IN ALL THREE DRESS BLOCKS THE q + 0x8 STORE COMES FIRST -- 1 word and
      28 rows in total, measured in two steps.
      Writing `*(s32 *)(q + 0x8) = *(s32 *)(D_800DE938 + 0x4);' before the
      0x4/0x5 byte stores and the 0xC colour store is 0. With it last in the
      two post-loop dress blocks the body is 613 words / 190 rows; moving it
      first in those two is 612 / 177, and first in the nested loop's block
      too is 612 / 170. The mechanism is a load-delay stall: the load of
      D_800DE938 + 0x4 cannot be hoisted over the byte stores by sched
      (`true_dependence' will not excuse a fixed-address scalar against a
      varying plain-cast MEM), so putting the statement early is the only way
      to give the load the five slots the target gives it. The emitted ORDER
      still shows the 0x8 store last, because sched sinks the store; the
      listing's store order is not the source's.
   7. THE TWO DRESS TAILS WRITE e + 0x48 BEFORE e + 0x4E AND e + 0x4C --
      1 word and 21 rows. 612 / 170 with 0x48 last, 611 / 149 with it first,
      in both the single-node block and the 0x312 loop. Same mechanism as 6
      from the other side: the 0x48 statement is the one that carries the
      p + 0x50 load (and in the 0x312 loop the `rand' call), and it must be
      early enough for the load to clear its delay slot. The emitted order is
      0x4E, 0x4C, 0x48 in both.
   8. func_800A4688's ARGUMENTS 3 AND 4 ARE SIGNED 16-BIT, NOT `u16' -- see
      the alias declaration above. With the committed `u16' prototype the
      third argument emits `andi $a2, $v0, 0xFFFF' and the fourth `lhu $a3'
      where the target has `sll'/`sra $a2' and a bare `lh $a3'. Arguments 1
      and 2 ARE `u16' and the `andi $s0, ..., 0xFFFF' pair proves it. This is
      a statement about the CALLEE's real signature, not about this body.
   9. func_800A4688's RESULT IS TESTED AS s16 -- the target has
      `sll $v0, $v0, 16' then `bnez' after the call, so the zero test is
      `(s16)func_800A4688_s(...) != 0'. Without the cast that word is absent.
  10. THE NESTED LOOP'S `g * 16' MUST BE A VARIABLE, AND SO MUST
      `dx * gi + 0x20' -- 2 words and 16 rows together, measured as the
      `gi' step (617 / 193 to 613 / 177, mid-fight) and the `ox'/`oy' step
      (611 / 88 to 611 / 85, clean).
      Written `dx * (h * 16)' inline, `fold' reassociates the constants to
      `(dx * 16) * h' and emits `sll $v0, $t0, 4' TWICE -- once per
      coordinate -- where the target computes `sll $v0, $s3, 4' once and
      shares it. And with the `+ 0x20' inline, fold flattens
      `A * 64 + dx * gi + 0x20' to `(A * 64 + dx * gi) + 0x20' and puts the
      `addiu' on the sum; the target puts it on the `mflo'. A VAR_DECL is the
      one operand `split_tree' cannot decompose (fold-const.c `associate'),
      which is the levers file's entry used FORWARD, as the donor's facts 6
      and 7 do.
  11. THE TIMER DECREMENT IS A LOCAL, NOT A RE-READ. `t' is read in case 1
      (`t >= 0x16') and case 2 (`t <= 0') as well as stored to p + 0x50 in
      the prologue. It cannot be a reload: a cse basic block ends at a
      CODE_LABEL (cse.c:8086) and each switch arm begins with one, so no
      forwarding reaches the arms. `t' must be declared `s16' -- the arms
      sign-extend it with `sll'/`sra' before comparing.
  12. CASE 1's TIMER GATE READS THE VARIABLE, ITS SECOND GATE READS MEMORY.
      `if (t >= 0x16)' then `if (*(s16 *)(p + 0x50) < 0)' -- the first is the
      `sll'/`sra'/`slti' on $a0, the second is a fresh `lh $v0, 0x50($t0)'
      and `bgez'. Two reads of one value, one from the local and one from
      memory, 40 words apart.

   MEASURED FREE, so do NOT spend compiles on them:

   * The u16-versus-s16 read width on p + 0x4C and p + 0x4E in the step
     function, and by the same mechanism the `*(u16 *)' reads here that only
     feed a halfword store.
   * Passing `x'/`y' to func_800A4688_s with no cast and letting the `u16'
     parameters do the masking, against writing `(u16)j' explicitly:
     610 / 15 both ways.

   MEASURED NOT MEASURABLE -- these are NOT free, the tools cannot see them:

   * The three source orderings of `n->unk_10 = ...' and `q = ...' relative
     to the `gi'/`ox'/`oy' group: before the group, after it, and split
     around it. All three 610 / 15. This is the open residue.
   * Merging `ox' and `oy' into one variable assigned twice: 610 / 15.
   * `q[0x4] = 0' against `*(u8 *)(q + 0x4) = 0': BIT-IDENTICAL output. A
     subscript on a POINTER is not an ARRAY_REF -- `build_array_ref' lowers
     it to `*(q + 4)' -- so MEM_IN_STRUCT_P is NOT reachable this way and the
     levers file's array-ref spelling does nothing for a `u8 *'.
   * `ax = i' against `ax = 0' after `ay = i': cse's `canon_reg' rewrites
     either into a copy of the most recently established equivalent, so both
     emit `move ax, ay' where the target copies from `i'. 610 / 15.
   * Swapping the two coordinate statements in the first loop: 610 / 15.
   * Declaration order of `n' before `q' against `q' before `n': 610 / 149 at
     the time, unchanged. Per the levers file that rules allocation PRIORITY
     out as the cause of the n/q exchange -- and fact 5 then found the real
     one, which was a merge.

   REJECTED, with the trap each one sets:

   * `s16 j'/`s16 h' for the first loop's coordinates: 612 / 174, which is
     BETTER than the 613 / 190 the correct `s32' measured at that baseline --
     and it is WRONG. A 16-bit destination makes `combine' prove the low bits
     of `<< 6' clear and emit `ori $v1, $v1, 0x20' where the target has
     `addiu'. A standalone probe of seven spellings -- a cast inside, a cast
     outside, a split `+=', the constant on the left, an extra local -- gives
     `ori' for ALL SEVEN at 16 bits and `addiu' at 32. So the width is
     settled by one compile and no amount of casting reaches it; the better
     row count was compensating for facts 1-3, which were still live.
   * Splitting the coordinate loads into their own statements
     (`j = *(u8 *)(o + 0x24); j = (j + ax) * 64 + 0x20;'): 610 / 21 at the
     time, where the unsplit form was 610 / 27, so it read as a 6-row
     improvement AND it was the edit that first reached 610 words. Once facts
     1-3 landed, the unsplit form is 610 / 15 and the split is 610 / 21 --
     the split had been covering for them. This is the clearest
     `an improvement is not evidence' case on this body: the split gained a
     word by removing a load-delay nop that the correct variable funnelling
     removes anyway.
   * Writing the coordinate expressions INLINE at all three call sites with
     no locals: 623 words / 107 rows. cse does not unify across the two
     intervening calls, and the mask narrows to `andi 0xffe0' because each
     copy is single-use -- which is itself the proof that the target's
     `andi 0xFFFF' comes from a MULTI-USE variable.
   * Separate `xs'/`ys' locals for the masked coordinates: 610 / 15, i.e. not
     measurable, because combine folds a single-use conversion back in.

   WHAT THE LISTING SETTLED THAT NO SWEEP WOULD HAVE:

   * The two magic multiplies are `% 72' (0x38E38E39, post-shift 4,
     multiplied back by 9 then 8) and `% 12' (0x2AAAAAAB, post-shift 1,
     multiplied back by 3 then 4). I first read the second as `% 6' from a
     hex-stripped dump in which `sll $v1, $v1, 2' looked like `sll ..., 1',
     and that would have been a plausible wrong number rather than an error.
     Read the raw hex for a magic multiply; the same constant 0x2AAAAAAB is
     `/ 6' with post-shift 0 in ovl_dng_s09 and `/ 12' with post-shift 1 here.
   * The 24-byte stack object at 0x20($sp) is a `UnkVec' LOCAL, not three
     halfword temporaries. The `sh' at 0x22, 0x26 and 0x2A are the +0x2, +0x6
     and +0xA integer halves of unk_00/unk_04/unk_08 -- the same convention
     the donor uses on n->unk_08 -- and the six `lw'/`sw' pairs in the
     r != 0 arm and at the single-node spawn are one struct assignment each.
   * The r != 0 arm reads o + 0x24 for BOTH coordinates in the ray walk's
     seed (`lbu $v0, 0x24($fp)' twice, never 0x25). That is almost certainly
     an original bug; it is transcribed as written.
   * func_s04_8002498C's address is taken at three sites and is loop-invariant
     in the 0x312 loop (hoisted to $s5) but NOT in the nested 0x112 loop,
     where it is rematerialised per iteration. That asymmetry is the residue.
*/
void func_s04_80024004(u8 *p, UnkVec *b, u8 *c)
{
    UnkVec v;
    u8 *s;
    u8 *o;
    u8 *w;
    UnkNode *n;
    u8 *q;
    u8 *m;
    u8 *e;
    s32 r;
    s32 i;
    s32 j;
    s32 dx;
    s32 dy;
    s32 ax;
    s32 ay;
    s32 d;
    s32 gi;
    s32 ox;
    s32 oy;
    s32 k;
    s32 h;
    s16 t;

    s = *(u8 **)(p + 0x0);
    o = *(u8 **)(s - 0x14);
    j = *(u16 *)(s + 0x2A) >> 8;
    dx = *(s16 *)((j & 0xE) + (s32)D_8006CCD8);
    dy = *(s16 *)((j & 0xE) + (s32)D_8006CCE8);
    t = *(u16 *)(p + 0x50) - 1;
    *(s16 *)(p + 0x50) = t;
    w = s - 0x20;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        b->unk_00 = *(s32 *)(*(u8 **)(w + 0x8) + 0x0);
        b->unk_04 = *(s32 *)(*(u8 **)(w + 0x8) + 0x4);
        b->unk_08 = *(s32 *)(*(u8 **)(w + 0x8) + 0x8);
        b->unk_0C = dx << 19;
        b->unk_10 = dy << 19;
        r = func_800A05A4(s, *(u8 *)(o + 0x24), *(u8 *)(o + 0x25), *(s16 *)(s + 0x2A),
                          (s16)func_800A3820(4));
        *(s32 *)(s + 0x60) = r;
        if (r == 0)
        {
            *(s16 *)((u8 *)&v + 0x2) = (*(u8 *)(o + 0x24) + dx * func_800A3820(4)) * 64 + 0x20;
            *(s16 *)((u8 *)&v + 0x6) = (*(u8 *)(o + 0x24) + dy * func_800A3820(4)) * 64 + 0x20;
            *(s16 *)((u8 *)&v + 0xA) =
                func_800BCB04(*(u16 *)((u8 *)&v + 0x2), *(u16 *)((u8 *)&v + 0x6),
                              *(u16 *)(*(u8 **)(w + 0x8) + 0xA) - 0x30);
            if (*(s16 *)((u8 *)&v + 0xA) > 0)
            {
                *(s16 *)((u8 *)&v + 0xA) = 0;
            }
            i = 0;
            ay = 0;
            ax = 0;
            for (; i < func_800A3820(4); i++)
            {
                j = (*(u8 *)(o + 0x24) + ax) * 64 + 0x20;
                h = (*(u8 *)(o + 0x25) + ay) * 64 + 0x20;
                if ((s16)func_800A4688_s((u16)j, (u16)h, func_800BCB04((u16)j, (u16)h, -0x400),
                                         *(s16 *)(s + 0x2A), *(s32 *)(s + 0x60)) != 0)
                {
                    break;
                }
                if (!((s16)func_800BCB04((u16)j, (u16)h,
                                         *(u16 *)(*(u8 **)(w + 0x8) + 0xA) - 0x30) < 0x200))
                {
                    break;
                }
                ay += dy;
                ax += dx;
            }
            j = dx * i;
            h = dy * i;
        }
        else
        {
            n = (UnkNode *)((u8 *)r - 0x20);
            v = *n->unk_08;
            m = (u8 *)n->unk_0C;
            j = *(u8 *)(m + 0x24) - *(u8 *)(o + 0x24);
            h = *(u8 *)(m + 0x25) - *(u8 *)(o + 0x25);
            if ((*(u16 *)(m + 0x14) & 0x8000) != 0 && (*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                *(s16 *)(p + 0xA) = 1;
                *(s16 *)(p + 0x50) = 0;
                break;
            }
        }
        i = __builtin_abs(j);
        d = __builtin_abs(h);
        if (i < d)
        {
            i = d;
        }
        if (i < 2)
        {
            j = 0;
            h = 3;
        }
        else
        {
            j = 1;
            h = 0;
        }
        k = *(s16 *)(*(u8 **)(w + 0x8) + 0xA);
        for (; j < i; j++)
        {
            for (; h < 4; h++)
            {
                n = func_8003FD64(0x112, D_80083498);
                if (n != NULL)
                {
                    n->unk_10 = (u32)func_s04_8002498C;
                    q = (u8 *)n->unk_0C;
                    gi = h * 16;
                    ox = dx * gi + 0x20;
                    oy = dy * gi + 0x20;
                    *(s16 *)((u8 *)n->unk_08 + 0x2) = (*(u8 *)(o + 0x24) + j * dx) * 64 + ox;
                    *(s16 *)((u8 *)n->unk_08 + 0x6) = (*(u8 *)(o + 0x25) + j * dy) * 64 + oy;
                    *(s16 *)((u8 *)n->unk_08 + 0xA) =
                        func_800BCB04(*(u16 *)((u8 *)n->unk_08 + 0x2),
                                      *(u16 *)((u8 *)n->unk_08 + 0x6),
                                      *(u16 *)(*(u8 **)(w + 0x8) + 0xA) - 0x30);
                    e = (u8 *)n + 0x20;
                    if (*(s16 *)((u8 *)n->unk_08 + 0xA) >= 0x200)
                    {
                        *(s16 *)((u8 *)n->unk_08 + 0xA) = k;
                    }
                    k = *(s16 *)((u8 *)n->unk_08 + 0xA);
                    *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x1000;
                    *(u8 **)(q + 0x0) = D_800DE938;
                    *(s32 *)(q + 0x8) = *(s32 *)(D_800DE938 + 0x4);
                    *(s32 *)(q + 0xC) = 0x808080;
                    *(u8 *)(q + 0x4) = 0;
                    *(u8 *)(q + 0x5) = 0;
                    *(u8 **)((u8 *)n + 0x20) = p;
                    *(s16 *)(e + 0x48) = j * 4 + h * 2;
                    *(s16 *)(e + 0x4E) = 0;
                    *(s16 *)(e + 0x4C) = 0;
                }
            }
            h = 0;
        }
        *(s16 *)(p + 0x50) = i * 4 + 2;
        if (*(u8 **)(s + 0x60) != NULL)
        {
            n = func_8003FD64(0x112, D_80083498);
            if (n != NULL)
            {
                q = (u8 *)n->unk_0C;
                n->unk_10 = (u32)func_s04_8002498C;
                *n->unk_08 = v;
                *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x2000;
                *(s16 *)(q + 0x10) = 0;
                *(u8 **)(q + 0x0) = D_800DE938;
                *(u16 *)(q + 0x14) |= 0xC;
                *(s32 *)(q + 0x8) = *(s32 *)(D_800DE938 + 0x4);
                *(u8 *)(q + 0x4) = 0;
                *(u8 *)(q + 0x5) = 0;
                *(s32 *)(q + 0xC) = 0x808080;
                e = (u8 *)n + 0x20;
                *(u8 **)((u8 *)n + 0x20) = p;
                *(s16 *)(e + 0x48) = *(u16 *)(p + 0x50);
                *(s16 *)(e + 0x4E) = 1;
                *(s16 *)(e + 0x4C) = 0;
            }
            for (j = 0x28; j >= 0; j--)
            {
                n = func_8003FD64(0x312, D_80083498);
                if (n != NULL)
                {
                    q = (u8 *)n->unk_0C;
                    n->unk_10 = (u32)func_s04_8002498C;
                    *(s16 *)((u8 *)n->unk_08 + 0x2) =
                        *(u16 *)((u8 *)&v + 0x2) + (rand() & 0x3F) - 0x20;
                    *(s16 *)((u8 *)n->unk_08 + 0x6) =
                        *(u16 *)((u8 *)&v + 0x6) + (rand() & 0x3F) - 0x20;
                    *(s16 *)((u8 *)n->unk_08 + 0xA) =
                        *(u16 *)((u8 *)&v + 0xA) - rand() % 72 - 0x10;
                    *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x1000;
                    *(s16 *)(q + 0x10) = 0x60;
                    *(u8 **)(q + 0x0) = D_800DED28;
                    *(u16 *)(q + 0x14) |= 0xC;
                    *(s32 *)(q + 0x8) = *(s32 *)(D_800DED28 + 0x4);
                    *(u8 *)(q + 0x4) = 0;
                    *(u8 *)(q + 0x5) = 0;
                    *(s32 *)(q + 0xC) = 0x606060;
                    e = (u8 *)n + 0x20;
                    *(u8 **)((u8 *)n + 0x20) = p;
                    *(s16 *)(e + 0x48) = *(u16 *)(p + 0x50) + rand() % 12 + 2;
                    *(s16 *)(e + 0x4E) = 0;
                    *(s16 *)(e + 0x4C) = 0;
                }
            }
            *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) + 0x16;
        }
        func_800A56E0(0x300);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 1:
        if (t >= 0x16)
        {
            b->unk_00 += b->unk_0C;
            b->unk_04 += b->unk_10;
            *(s16 *)((u8 *)b + 0xA) =
                func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                              *(u16 *)(*(u8 **)(w + 0x8) + 0xA) - 0x30);
            if (*(s16 *)((u8 *)b + 0xA) > 0)
            {
                *(s16 *)((u8 *)b + 0xA) = 0;
            }
        }
        if (*(s16 *)(p + 0x50) < 0)
        {
            if (*(u8 **)(s + 0x60) != NULL)
            {
                func_8009CE1C(*(u8 **)(s + 0x60), 8, *(u8 *)(p + 0x9), 1, *(s16 *)(s + 0x2A),
                              (u32)s, 2);
            }
            *(s16 *)(p + 0x50) = 0x10;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 2:
        if ((*(s16 *)(p + 0x52) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x52) = *(u16 *)(p + 0x52) & 0x7FFF;
        }
        else if (t <= 0)
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}
