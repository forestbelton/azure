/* PARK func_b32_80174428  (ovl_dng_b32, agent h143, batch 33)

   340 target words, 340 candidate words, 15 ALIGNED DIFFERENCES.
   WORD-EXACT. Every statement, every call, every constant and every branch
   shape is confirmed; the residue is TWO register/scheduling groups and
   nothing else. dngfam: 1 member, 340 words, no other park existed.

   IT OWNS jtbl_b32_801708F0.  6 words = 6 cases + 0 pad, dispatch bound 6
   (`sltiu $v0, $a1, 0x6'), 6 DISTINCT targets, and NO DEFAULT AMONG THE
   ENTRIES: the range check's `beqz $v0' goes to .Lb32_80174948, which is the
   function's shared epilogue and not a table entry. So all six `.word's are
   real arms, cases 0..5 in table order, and the arms are written in that
   order below. Entry list:
     0 -> .Lb32_801744B8   1 -> .Lb32_80174558   2 -> .Lb32_801745E8
     3 -> .Lb32_80174674   4 -> .Lb32_801746E4   5 -> .Lb32_801747C4
   Case 0 FALLS THROUGH into case 1 (no `j' at 0x80174554).

   WHAT IT IS.  The bank's six-state "shoved along the corridor" machine.
   State 0 dresses from D_b32_80174EF8 and seeds the velocity at dx*0xC0000;
   state 1 counts p+0x96 down and reverses to -dx*0x40000; state 2 re-dresses
   from D_80174F00 and either arms state 5 or masks the facing to 0xFFF;
   state 3 steps the facing by +-0x80 until it reaches 0x400; state 4 searches
   up to 0x40 tiles with func_800A4E2C / func_800BCB04 for a landing square,
   restoring the original tile if it runs out; state 5 plays the 0x80174FC4 /
   D_80174FD4 animation pair and then does the full hand-back.

   SOURCES USED.  No twin at any ratio. CALLEE.txt's row func_b41_80173E64 is
   a -0x48 ten-save body against this -0x38 nine-save one; the useful donors
   were my own unit: func_b32_80174B14 / func_b32_8017224C for the
   D_80083228 + d[0x2A] dress index, csrc/ovl_dng_b02.c:690/786 for the
   D_8006CCD8 / D_8006CCE8 index-first facing idiom, and the lever file's
   `while (1) { n--; ... }' rule for state 4's loop.

   THE RESIDUE, both groups:

   GROUP A, 10 rows -- THE SECOND FACING-TABLE BASE IS HOISTED TOO FAR.
   The target materialises %hi/%lo(D_8006CCD8) at indices 8/9 (ahead of five
   prologue saves) and %hi/%lo(D_8006CCE8) at 20/21, AFTER the first index
   `addu'. Every spelling I tried hoists BOTH pairs to 8..11, which pushes the
   five saves down two slots and renames $v1/$a0/$v0 across thirteen
   instructions. The instruction MULTISET is identical; only that pair's
   position differs.

   GROUP B, 5 rows -- the two call results inside state 4's loop.
   `sra $a0, $v0, 16' twice in the target (func_800A4E2C's and
   func_800BCB04's); mine is `sra $v1' then `sra $v0'. Both are dead argument
   registers in the target, so it looks like allocation, but all the obvious
   levers are flat (below).

   MEASURED AND REJECTED -- every figure from a 340-word candidate:

   | spelling                                          | words | rows |
   | as written below                                   |  340  |  15  |
   | second lookup inline, no `u1' local                |  340  |  18  |
   | `u1' on the FIRST lookup instead (b02's own form)  |  340  |  18  |
   | ONE base local reused for both lookups             |  340  |  23  |
   | ... + `dy' declared before `dx'                    |  340  |  23  |
   | ... + `kk' computed first                          |  340  |  23  |
   | the 0x2A load split into its own `u16' local       |  340  |  18  |
   | `dy' assigned before `dx'                          |  340  |  18  |
   | `kk' inlined at both lookup sites                  |  340  |  18  |
   | first lookup written base-first                    |  340  |  18  |
   | `kk' declared last / `dy, dx, kk'                  |  340  |  18  |
   | `r' reused for func_800BCB04's result              |  340  |  15  |
   | `s32 r' with the `(s16)' cast kept                 |  340  |  15  |
   | `s16 dx' / `s16 dy'                                |  334  |  53  |
   | `dx * 0xC0000' instead of `(dx<<19)+(dx<<18)'      |  338  |  86  |
   | no `q' / `t' table locals in state 4               |  334  |  53  |
   | one `e' dress local shared by states 0,1,2         |  340  |  31  |
   | one `e' local PER state                            |  340  |  24  |
   | plain shared `kk', both lookups inline             |  340  |  18  |
   | ... + the mask split off as `kk &= 0xE'            |  340  |  18  |
   | ... + the second lookup as `kk += (s32)D_8006CCE8' |  340  |  18  |
   | ... with `dy' first and `dx' written in place      |  340  |  18  |

   ADDED AFTER func_b32_80172C90 LANDED IN THE SAME UNIT, and it is a
   NEGATIVE result worth having: that body's head has the IDENTICAL Group A
   fingerprint -- two facing-table bases, the target materialising one early
   and one after the first index `addu' -- and there the cure was a shared
   index local plus TWO in-place updates (`kk &= 0xE' as its own statement,
   then `kk += (s32)D_8006CCE8' so combine_regs ties the second `addu' to the
   dying index).  That took 80172C90 from 19 rows to 0.  Here all four
   variants of it measure 18, worse than the 15 this park keeps.  The two
   bodies differ in one structural detail that may be the whole story:
   80172C90 has a statement (`*(u16 *)(p + 0x96) -= 1') BETWEEN the facing
   reads and the switch dispatch, and this body has nothing there, so sched1's
   ready list at clock 0 is not the same shape.  Do not spend budget
   re-deriving the in-place lever here; it has been tried.

   READ THE LAST THREE ROWS FIRST, they are the transferable part.
     * `s16 dx' makes both facing loads `lhu' instead of `lh' and adds a
       sll/sra at each use -- the same fact func_b32_80172908 prices at 2 rows
       in this unit. `s32' with the `(s16 *)' cast in the load is right.
     * the state-0 velocity is `(dx << 19) + (dx << 18)', NOT `dx * 0xC0000'.
       The lever file's "a multiply shown as sll/addu/sll wants the plain
       constant" is the WRONG WAY ROUND for 0xC0000: expand_mult turns the
       plain constant into a single `sll 19' plus a `sll 1' and loses the
       `addu'. 2 words and 33 rows.
     * state 4 needs POINTER LOCALS for D_80082E80 and D_80081468, assigned
       in the preheader right after `n = 0x40'. Named inline inside the loop,
       each access is its own `lui' and the two preheader `addiu's are absent:
       6 words short, 53 rows. This is the SYMBOL_REF/LUID fact from the brief
       and it is worth more here than anywhere else in the unit.
     * a dress-table `e' local SHARED across three switch arms is 31 rows --
       `reg_n_sets' exactly as the brief describes. One local per arm is 24
       and NO local (the symbol named at both sites in each arm) is also 24,
       so those two are not distinguishable from here. No local is kept.

   WHAT I WOULD TRY NEXT, in order.
     1. GROUP A is a sched1 ready-list question and `-da' answers it in one
        compile: dump `.sched' and compare INSN_PRIORITY on the two
        `(set pseudo (symbol_ref))' insns. If they are equal, the tie falls to
        INSN_LUID and the only lever left is which statement expands the
        second symbol -- try making state 0's and state 1's dx/dy USES fewer
        (the chain length to the block end is what sets the priority), e.g.
        by reading dy ONLY in the arms that need it.
     2. The one shape I did NOT try: dx and dy computed inside case 0 with
        case 1 reached only by fall-through, plus a SECOND pair of statements
        at the top of case 1 for the direct entry. That changes the block
        the two `lui's live in, which is the only thing Group A is about.
        It costs words if wrong, so measure the word count first.
     3. For GROUP B, `-da' `.greg's allocation-order line on the two sra
        destinations. My declaration-order sweep (6 orders, all 18 or 15) rules
        out `allocno_compare' priority, which per the brief means the cause is
        arithmetic or statement order -- the candidate I did not test is
        moving `r = (s16)func_800A4E2C(...)' to the top of the loop body
        before the `n--'/`blez' pair.
     4. The `e' row is ambiguous at 24/24. Settle it only after Group A is
        fixed; one of the two may go to 0.

   NOT IN DOUBT, so do not re-derive: the whole switch shape and arm order,
   the fall-through from case 0 into case 1, case 2's early `p[0x9B] = 5'
   exit, state 3's `(u16)d[0x2A] >= 0x401 && <= 0xC00' range test (gcc's own
   range_test turns it into the `sltiu $v0, $v0, 0x800' the listing has),
   state 4's `while (1) { n--; if (n <= 0) ... }' shape with reorg stealing
   the decrement into every back branch's delay slot, and the c+0x2C RELOAD
   at state 5's dress site (cse does not forward the stored pointer across
   the func_800AD594 call).


   ===== AGENT h149, BATCH 34 (PARKS BATCH), 2026-11-25 =====
   STILL 340 WORDS / 15 ALIGNED DIFFERENCES, body unchanged.  All THREE of
   the author's next steps are now measured and all three are NEGATIVE, and
   the Group A diagnosis is wrong in its PASS.  Everything else above holds.

   NEXT STEP 1 IS ANSWERED AND GROUP A IS NOT A SCHEDULING QUESTION.
   `-da' on the whole unit (tools/decomp/dump.sh), reading `.sched',
   `.sched2', `.lreg' and `.greg'.  Three facts, in order:

   (a) THE PROLOGUE SAVES DO NOT EXIST AT sched1.  `.sched' prints
       ";; -- basic block number 0 from 4 to 823 --" and that block holds
       only the four parameter copies, the two symbol materialisations, the
       facing read, the index chain, the two `lh's and the range check.  The
       ten saves first appear in `.sched2', as insns 856..874.  So the
       save-versus-`lui' interleave Group A measures is produced by SCHED2,
       after both allocators, and no argument about "sched1's ready list at
       clock 0" can reach it -- including the structural difference from
       func_b32_80172C90 that this park attributes it to.

   (b) THE TWO symbol_ref PRIORITIES ARE EQUAL AND CANNOT BE MADE UNEQUAL.
       `.sched' prints, for the body below:
         insn 16 (high D_8006CCD8) priority 1   insn 38 (high D_8006CCE8) 1
         insn 17 (low  D_8006CCD8) priority 1   insn 39 (low  D_8006CCE8) 1
         insn 24 srl 2   insn 28 andi 2   insn 31 addu1 2   insn 42 addu2 2
       and `.sched2' prints the same numbers.  sched.c:1456's `priority' is
       the max over LOG_LINKS of priority(pred) + insn_cost - 1, i.e. the
       longest path from the BLOCK HEAD, so both lo_sum insns are 1 in any
       spelling of the head.  The target's order requires insn 39 to be
       scheduled ahead of insn 31 -- priority 1 ahead of priority 2 -- and
       SCHED_SORT is strict on priority.  The target's layout is therefore
       unreachable by scheduling.  Do not dump `.sched' for this again.

   (c) WHAT IT IS: local-alloc's `qty_compare', and it is ONE decision.
       The three competing quantities in block 0, every figure read off
       `.lreg' for the body below:
         A  index  = (r99 lhu, r100 srl, r84 andi) tied into ONE quantity by
            local-alloc.c's `combine_regs' (each input dies at the next
            insn):  7 refs over 6 insns, floor_log2(7) x 7 = 14, PRI 2.333
         C  base2  = (r105 high, r85 low+addu):  6 refs, 10 insns, PRI 1.200
         B  base1  = (r96  high, r86 low+addu):  6 refs, 11 insns, PRI 1.091
       QTY_CMP_PRI is local-alloc.c:1693 -- floor_log2(n_refs) x n_refs x
       size, divided by (death - birth) -- highest allocated first, and
       `find_free_reg' scans ascending with no REG_ALLOC_ORDER on this port.
       So A takes $v0, C takes $v1, B takes $a0, which is exactly what the
       body emits.  The target is A in $a0, C in $v0, B in $v1: a clean
       3-cycle rotation, whose allocation order is C, B, A.  The index
       quantity has to be the LOWEST priority of the three and here it is by
       far the highest.
       AND THE TEN ROWS ARE DOWNSTREAM OF THAT ONE CHOICE.  With base2 in
       $v1 -- a register nothing else in the block wants -- sched2 is free to
       hoist its `lui'/`addiu' up beside base1's.  With base2 in $v0, the
       register the `lhu' result and the `sltiu' both use, the output and
       anti dependences on $v0 PIN the pair between the `andi' and the
       `sltiu', which is the target's layout to the word.  Group A is not ten
       faults; it is one allocation, reported ten times.

   MEASURED AND REJECTED -- eight more head spellings, all 340 words, so
   WORD-EXACT still holds for every one of them:
   | u0 in place, u1 single-assignment (dy = *(s16 *)(u1 + kk))     | 18  |
   | u0 single-assignment (dx = *(s16 *)(u0 + kk)), u1 in place     | 16  |
   | both single-assignment, both bases materialised first          | 17  |
   | u0 in place, second lookup inline off the symbol + kk          | 18  |
   | u0 in place + `kk += (s32)D_8006CCE8; dy = *(s16 *)kk'         | 18  |
   | ... + `kk' split into `>> 8' then a separate `&= 0xE'          | 18  |
   | the landed func_b32_80172C90 head verbatim (no base local)     | 18  |
   | ... + `kk' split into two statements                           | 18  |
   Not one of these changes A's 7 refs over 6 insns, which is why they all
   land in 16..18.  With the author's eleven that is NINETEEN spellings of
   the same four lines.  Stop sweeping the head.

   NEXT STEP 2 IS MEASURED AND REFUTED.  dx/dy computed inside case 0 with a
   second pair at the top of case 1 is 351 WORDS / 47 -- eleven words over.
   It could not have worked: the target's two `lh's sit BEFORE the
   `beqz'/`jr' dispatch, so they are in the entry block and cannot live in an
   arm.  That also kills next step 1's suggestion of reading dy only in the
   arms that need it: both values are computed unconditionally in the target.

   NEXT STEP 3 IS MEASURED AND REFUTED.  `r = (s16)func_800A4E2C(...)' moved
   above the `n--'/`blez' pair is 339 WORDS / 30 -- a word short and twice as
   bad.  Two further Group B spellings, both word-exact:
   | `r = func_800A4E2C(...); r = (s16)r;' as two statements        | 15  |
   | `n -= 1' with the two restore stores in the other order        | 22  |
   Group B is unmoved at 5 rows.  Note it has the SAME SHAPE as Group A: the
   target puts both sign-extend results in $a0 and this body puts them in $v1
   and $v0, so it is `find_free_reg' scanning ascending one block down, and
   the brief's "fix the occupant, not the site" is the right frame for it.

   NEW NEXT STEP, in order.
     1. Lower the INDEX quantity's QTY_CMP_PRI below both bases' -- it needs
        to go under 1.091 from 2.333.  Over a 6-insn range the only
        numerators that reach that are 3 (3 refs) and 2 (2 refs), so the
        index must be its OWN quantity rather than the three-pseudo chain.
        `combine_regs' ties them because each input DIES at the next insn,
        and local-alloc gives no quantity at all to a pseudo that "is not
        local to this block or dies more than once".  READ THIS OFF THE
        LISTING FIRST, no dump needed: the target's `srl $a0, $v0, 8' writes
        a DIFFERENT register from the `lhu $v0' it reads, so in the target
        the lhu destination is ALREADY not in the index quantity -- the chain
        is broken at its first link.  The job is to find what gives the
        facing word its second death.
     2. Do not reach for a do-while(0) bump here.  A bump RAISES references
        and the index needs FEWER; going the other way, each base would need
        9 and 8 references to overtake it (floor_log2 steps at 8), and the
        four bump variants I measured cost a word each: 341 words at 26, 29
        and 28, and 340 words at 19 for a bump on the first lookup.
     3. If this goes to the permuter, permute the FOUR HEAD LINES only -- the
        rest of the body is confirmed by this park and by nineteen measured
        spellings -- and rank the candidates by whether they emit the `srl'
        into a register other than the `lhu's, not by score.

   DECLARATIONS
   extern u8 D_80174FC4[];
   extern u8 D_80174FD4[];
   extern u32 D_80081468[2];
   s32 func_800A4E2C(u8 *, u8 *);
   void func_800AA53C(u8 *);
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern s16 D_80083228;
   extern u8 D_80082E80[];
   extern u8 D_b32_80174EF8[];
   extern u8 D_80174F00[];
   extern u8 D_80174F58[];
   extern Unk80083462 D_80083460;
   void func_80047784(u8 *, s32, s32);
   void func_800A56E0(s32);
   void func_8009A3D0(s32, s32, s32);
   void func_8009A21C(s32, s32, s32);
   void func_800A2B04(UnkVec *, s32, s32);
   void func_800AD594(u8 *, s32);
   void func_800A4ACC(u8 *);
   s16 func_800BCB04h(s32, s32, s16) __asm__("func_800BCB04");
   void func_b32_801710F4();
   END DECLARATIONS
*/
void func_b32_80174428(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 kk;
    s32 u1;
    s32 u0;
    s32 dx;
    s32 dy;
    s16 ox;
    s16 oy;
    s32 n;
    s16 r;
    u8 *g;
    u8 *q;
    u8 *t;

    u0 = (s32)D_8006CCD8;
    kk = (*(u16 *)(d + 0x2A) >> 8) & 0xE;
    u0 += kk;
    dx = *(s16 *)u0;
    u1 = (s32)D_8006CCE8;
    u1 += kk;
    dy = *(s16 *)u1;
    switch (p[0x9B])
    {
    case 0:
        func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(u8 **)(c + 0x2C) = D_b32_80174EF8;
        func_80047784(c, D_b32_80174EF8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        b->unk_0C = (dx << 19) + (dx << 18);
        b->unk_10 = (dy << 19) + (dy << 18);
        *(s16 *)(p + 0x96) = 4;
        func_800A56E0(0x80E);
        p[0x9B] += 1;
        /* fall through */
    case 1:
        if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            *(s16 *)(p + 0x96) -= 1;
            if (*(s16 *)(p + 0x96) > 0)
            {
                return;
            }
        }
        b->unk_10 = 0;
        b->unk_0C = 0;
        *(u8 **)(c + 0x2C) = D_80174F58;
        func_80047784(c, D_80174F58[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s16 *)(p + 0x96) = 0;
        b->unk_0C = -dx * 0x40000;
        b->unk_10 = -dy * 0x40000;
        p[0x9B] += 1;
        break;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        b->unk_10 = 0;
        b->unk_0C = 0;
        *(u8 **)(c + 0x2C) = D_80174F00;
        func_80047784(c, D_80174F00[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        if ((*(s32 *)(d + 0x1C) & 0x2000) != 0)
        {
            *(s16 *)(p + 0x96) = 7;
            p[0x9B] = 5;
            return;
        }
        *(u16 *)(d + 0x2A) &= 0xFFF;
        p[0x9B] += 1;
        break;
    case 3:
        if (*(s16 *)(d + 0x2A) == 0x400 || (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            func_80047784(c, 0x37, 0);
            p[0x9B] += 1;
        }
        else if (*(u16 *)(d + 0x2A) >= 0x401 && *(u16 *)(d + 0x2A) <= 0xC00)
        {
            *(u16 *)(d + 0x2A) -= 0x80;
        }
        else
        {
            *(u16 *)(d + 0x2A) = (*(u16 *)(d + 0x2A) + 0x80) & 0xFFF;
        }
        break;
    case 4:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        func_80047784(c, 0x38, 0);
        n = 0x40;
        q = D_80082E80;
        t = (u8 *)D_80081468;
        *(s16 *)(p + 0x96) = 0;
        ox = c[0x24];
        oy = c[0x25];
        while (1)
        {
            n--;
            if (n <= 0)
            {
                c[0x24] = ox;
                c[0x25] = oy;
                break;
            }
            r = (s16)func_800A4E2C(c + 0x24, c + 0x25);
            if (r < 0)
            {
                continue;
            }
            if (r == *(s8 *)(q + 0x26) && *(s16 *)(t + 0x6) >= 2)
            {
                continue;
            }
            if (func_800BCB04h((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, *(u16 *)((u8 *)b + 0xA) - 0x80) >= 0x201)
            {
                continue;
            }
            break;
        }
        p[0x9B] += 1;
        break;
    case 5:
        *(s16 *)(c + 0x1C) = *(u16 *)(*(s16 *)(p + 0x96) * 2 + (s32)D_80174FC4);
        *(s16 *)(c + 0x1E) = *(u16 *)(*(s16 *)(p + 0x96) * 2 + (s32)D_80174FD4);
        if (*(s16 *)(p + 0x96) == 4)
        {
            func_800A56E0(0x705);
        }
        if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            *(s16 *)(p + 0x96) += 1;
            if (*(s16 *)(p + 0x96) < 8)
            {
                return;
            }
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        func_800AA53C(d);
        *(s16 *)(d + 0x88) = func_800BCB04h((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, *(u16 *)((u8 *)b + 0xA) - 0x80);
        *(u8 **)(c + 0x2C) = D_80174F00;
        func_800AD594(d, 0x200);
        func_80047784(c, (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        func_8009A21C(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        func_800A2B04(b, c[0x24], c[0x25]);
        *(s16 *)(c + 0x1E) = 0x1000;
        *(s16 *)(c + 0x1C) = 0x1000;
        *(s32 *)(p + 0x8C) = (s32)func_b32_801710F4;
        func_800A4ACC(d);
        d[0x6D] = 0;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        break;
    }
}
