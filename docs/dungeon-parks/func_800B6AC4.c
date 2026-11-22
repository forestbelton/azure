/* PARK -- func_800B6AC4, the 12-case +0x9B state machine over jtbl_80089310.
   NOT LANDED.  CURRENT STATE, 2026-11-21 (the dated sections below are a
   log, newest first; this block is the only one that describes the body as
   it stands):

       812 words, target 812 words, 11 aligned differences
       (tools/decomp/aligndiff.py); ovlverify's positional count is
       14 differing (masked), and the extra three are the one-insn shift
       the `((u8 **)c)[0xB]` store causes at aligned 768.

   CASES 0-9 AND 11 ARE BYTE-EXACT, and so are the prologue, the switch
   dispatch, the epilogue, the shared `p[0x9B]++` tail
   (.L800B7718/.L800B771C) and the shared redraw tail (.L800B770C).
   THE WHOLE RESIDUE IS CASE 10, ONE CLUSTER, and it is ONE REGISTER:

     the D_800DD140 table pseudo takes $a3 where the target takes $a1,
     and case 10's `lh 0x2A($s3)` takes $a1 where the target takes $a3.

   That is three rows on its own -- aligned 778 (`lh a1,42(s3)` against
   `lh $a3, 0x2A($s3)`), 780 and 784 -- and the other eight are its
   consequences, in aligned-index order:

     750 / 752   the table's `lui %hi` and `addiu %lo` come out back to
                 back at 750 and 752; the target spreads them to 752 and
                 763, into the two load-delay holes after `lh $v0,0x8A($s3)`
                 and after `lw $a0,0x0($v0)`.
     763         so `addu $a2, $zero, $zero` -- the redraw call's third
                 argument -- lands at 763 instead of the target's 750.
     768 / 776   and the `((u8 **)c)[0xB]` store, whose only input is the
                 table address, is emitted at 768 as `sw $a3, 0x2C($s1)`
                 through `c` itself, where the target has it at 776 as
                 `sw $a1, 0x2C($a0)` after the `addu $a0, $s1, $zero` at
                 771 (which the candidate DOES get right).  Everything
                 between 769 and 776 is that one-insn shift, so the
                 D_800814A0 load reads at 776 instead of 774.

   WHY IT IS THE REGISTER AND NOT THE SCHEDULE.  Both case-10 pseudos are
   BLOCK-LOCAL (lreg prints them `in block 68`), so this is local-alloc, and
   local-alloc.c:1693's QTY_CMP_PRI is arithmetic:

       floor_log2(n_refs) * n_refs * size / (death - birth) * 10000

   The `lh 0x2A` quantity has 2 references over a 2-insn life, so 10000; the
   table pseudo has 4 (lui, addiu, the store's use, the final `addu`) over a
   34-insn life, so 2352.  The lh is therefore allocated FIRST and takes the
   lowest free register, $a1, and the table has to settle for $a3; the target
   allocates them the other way round.  To beat 10000 the table needs about
   twelve references (floor_log2(n)*n > 34).  THAT IS THE ARITHMETIC TO AIM
   AT, and the obvious way to reach it does not work: binding the table to a
   local `t10` used by both the store and the call is 11 (no change), and
   `t10++; t10--;` after it -- one pair or two, at every one of the eleven
   statement positions in case 10 -- is 12, i.e. worse, at every position.
   Shrinking the lh's priority instead needs its life to reach 5 insns, and
   hoisting the load into a local (`k`, `y`, `v` or a fresh `s16 i10`, at
   each of ten positions, in both the plain and the array spelling) is 11 at
   best and 13 or worse everywhere else.

   ALSO MEASURED AND REJECTED ON THE CURRENT BODY, all 11 rows (= no change)
   unless stated:
     * every `do { } while (0);` over every contiguous run of case 10's
       eleven statements (the whole arm is 13; two runs tie the base at 11);
     * a cancelled `v++; v--;` for all 26 names at all 12 statement positions
       in case 10;
     * a duplicated-arm `if (v) { s } else { s }` for all 26 names over each
       of case 10's last six statements;
     * all 180 legal orders of {B1, B2, D, C} x (Y before S) -- see the
       13 -> 11 entry;
     * `*(u8 **)(c + 0x2C)` and an array-typed cast for the c+0x2C store;
       the array-typed cast for d+0x8A, d+0x60, and the D_800E3DF0 entry;
     * a separate local for `e10[3] & 0x1F`, for the D_800E3DF0 entry, for
       the table, and dropping `e10` altogether;
     * merging each of the sixteen pointer locals into each other (240
       pairs) and all 96 legal assignments of {k,x,y,v} to the four scalar
       temp roles;
     * ALL FOUR SWEEPS COME BACK EMPTY ON THIS BODY, which is the brief's
       stopping condition for the sweep cycle: widthsweep "no width change
       helps", stmtsweep "no safe move helps", devsweep "no wrapper helps"
       (4563 candidates) and refsweep "no cancelled pair helps" (7079
       insertion points).  They were also empty on the 23-row body, and the
       case-10-only versions above were run by hand on this one.  The next
       thing to spend on this park is the permuter, not another sweep.
     * `(((D_80083228 + 0x100) + lh) >> 9) & 7` is 45 rows / 810 words and
       `(s16)` around the sum is 13, so the index expression as written is
       load-bearing too.

   LEVERS THAT WERE WORTH A LOT AND ARE ALREADY IN THE BODY:

   * EVERY D_80083460 BASE AND EVERY SCRATCH POINTER NEEDS ITS OWN LOCAL.
     One `h` shared by cases 2, 3 and 7 crosses a call in case 3, so global
     alloc gives the single pseudo a CALLEE-SAVED register everywhere and
     case 2's `lui/addiu D_80083460` comes out in $s0 where the target has
     $v1.  Splitting h/h2/h3/h4 (and r/e2/e7/e9/e10/n5/base9/base10) took
     the function from 280 aligned differences to 96 IN ONE STEP and, with
     it, fixed the whole parameter register map: before the split the
     allocation order was b, g, c, p, d and after it g, c, p, d, b, which
     is the target's.  Nothing else moved that map.

   * THE SIX FACING TABLES ARE NAMED TWICE, NOT BOUND TO A `u8 *t`.
     func_8008E264's rule transfers verbatim: with the local, the
     `addu $a0, $s1, $zero` the target emits BEFORE the table store is
     pushed past it at all six sites.  325 aligned differences with the
     local, 280 without.

   * CASE 2's FIRST D_80013714 READ MUST BE THE CONSTANT ADDRESS
     `*(u16 *)0x80013714`; the other four must be structs.h's SYMBOL.
     As the symbol, the `lui` is two insns and reorg steals it into the
     delay slot of the preceding `bnez $v0, .L800B7750`, where the target
     has a `nop` -- func_8008E0C4 records exactly this.  As the symbol in
     case 7 the `%hi` is CSE'd into $s3 and serves three reads, which the
     assembler macro cannot do, so that site must NOT be the constant.

   * CASE 1's THREE STORES ARE 0xC0, D_80081484, 0x8C -- in that order,
     with the +0x8C one a COMPONENT_REF (`((UnkDungeonActor *)p)->unk_8C`).
     The component ref is what lets sched sink the D_80081484 store past it
     (true_dependence's in-struct/varying exception); written
     `*(u32 *)(p + 0x8C)`, or with the 0x8C store ahead of the D_80081484
     one, the `lui %hi(func_800B69DC)` is stolen into the delay slot of the
     `bnez $v0, .L800B6C80` above and the block comes out in the wrong
     order.  Worth 10 aligned differences (75 -> 65).

   * CASE 4's +0x8C STORE IS ALSO A COMPONENT_REF, for func_800969B8's
     reason: through `*(u32 *)(p + 0x8C)` the store may alias D_800E296C
     and sched1 cannot lift that global's `lw` above it.

   * EVERY p+0xC0 ACCESS IS `((u8 **)p)[0x30]`, NOT A CAST.  The array form
     sets MEM_IN_STRUCT_P, which is what lets case 7's `lw $a0, 0xC0($s2)`
     move above the `sb` to D_800E2968.

   * CASE 9's e9[0] SOURCE BYTE IS READ INTO A TEMP BEFORE THE e9[3]/e9[2]
     STORES.  The target's `lw`/`nop`/`lbu` pair sits between `sb $v1, 0x1`
     and `sb $v0, 0x3`, and a byte store is QImode, which true_dependence
     never disambiguates -- so no spelling moves the load across it.  Only
     writing it out in that order puts it there.  Worth 10 (85 -> 75).

   * CASE 2's FIRST TEST IS WRITTEN `== 0` SO THE D_80083460 ARM IS THE
     FALL-THROUGH; written the other way round the branch is inverted.

   * D_80083228 IS D_80083160 + 0xC8, AND BOTH SPELLINGS APPEAR.  Case 0
     reads it as `q->unk_C8` off its own `Unk80083160 *q` (func_800927E4's
     measured shape, which this arm is a line-for-line copy of); every other
     arm reads the absolute `D_80083228`.

   * D_8001029C IS REACHED AS func_80098FF8 DOES IT: a `u8 *` local holding
     0x80010000 with 0x29C inside the index.

   Target 812 words = 0xCB0 bytes.  jtbl_80089310 is at rodata 0x89310,
   0 mod 8, so this function is safe to land (func_80095538 is the
   precedent). 
   DECLARATIONS (added 2026-11-16: this park did not compile from the repo
   alone -- seven symbols it uses were declared only in its author's
   transcript, so parkresidue could not measure it at all.)
     extern u32 D_800835E4[];
     extern u8 D_800DD108[];
     extern u8 D_800DD110[];
     extern u8 D_800DD118[];
     extern u8 D_800E0A2A[];
     extern u8 D_800E0A33[];
     extern u8 D_800E2968;
     typedef u8 *(*UnkDungeonSpawner)(s32, u8, u8, s16);
   END DECLARATIONS
   UnkDungeonSpawner was used by the body and defined nowhere -- reconstructed
   2026-11-16 from the `jalr $v0` at 800B7140: four arguments in $a0..$a3 as
   int / lbu c[0x24] / lbu c[0x25] / a halfword sign-extended by the sll+sra
   pair at 800B713C, and $v0 used as a pointer immediately after
   (`lbu 0x13($s0)`, `lw -0x14($s0)`).
   Shapes read off asm/nonmatchings/ovl_dungeon/func_800B6AC4.s: the six
   arrays are lui/addiu address pairs, D_800E2968 is lui + `sb %lo(..)` and
   so is a scalar byte, not an array.
   ====================================== 2026-11-21: 13 -> 11 rows at 812/812

   CASE 10's D_800E3DF0 ENTRY POINTER IS `e10' AGAIN, NOT A NEW PSEUDO:

       e10 = (u8 *)D_800E3DF0[e10[3] & 0x1F];
       *(u32 *)(e10 + 0x14) &= ~0x4000;

   The target holds both in $a0 (`lw $a0, %lo(D_8001029C)($v0)' at aligned
   755 and `lw $a0, 0x0($v0)' at 762); written as one expression the first
   one takes $v0 and costs aligned 755 and 757.  Measured on the 13-row body:
   this reuse 11, a SEPARATE local `q10' for the entry 13, a local for
   `e10[3] & 0x1F' 13, dropping `e10' entirely 13, `u8 **e10' 13.
   Re-sweeping the statement order afterwards keeps B1,B2,D,C,Y,S best at 11
   (B1 = the e10 reassignment): 1D2CYS and D12CYS are 13, anything with C
   ahead of D is 19 or worse.

   ====================================== 2026-11-21: 23 -> 13 rows at 812/812

   CASE 10's `*(s16 *)(d + 0x2A)' MUST BE AN IN-STRUCT REFERENCE, AND WITH IT
   THE ORDER BECOMES B, D, C, Y, S.  Two changes, measured together because
   neither is worth anything alone (the array cast alone is 46 rows / 807
   words; B,D,C,Y,S on the plain cast is 36).

   WHY.  The residue was one cascade, and `tools/decomp/sched.py' names its
   root: in sched1 the `D_800814A0' store (insn 1867) is a PREDECESSOR of the
   `lh 0x2A($s3)' load (insn 1878) -- sched.c's `true_dependence' cannot
   disambiguate a store whose address is `(lo_sum (reg) (symbol_ref))' from a
   load whose address is `(plus (reg) 42)'.  The store is therefore pinned
   ahead of that load, which pins the D_800814A0 base pseudo's live range
   short, which lets local-alloc give the base $a2 -- and then the arg-setup
   `addu $a2, $zero, $zero' has to follow the store (WAR), so it lands at the
   bottom of the block instead of the top, and the whole caller-saved file
   below it rotates.  The target has the store as the LAST insn of the arm.

   `true_dependence' (scratch/gcc-src/cdk-gcc-b18/sched.c) has exactly one
   exception that applies here: the load is MEM_IN_STRUCT_P and its address
   varies, the load is not QImode, and the STORE is not in-struct and its
   address does not vary.  `rtx_varies_p' (rtlanal.c:103) returns the
   varies-ness of a LO_SUM's *symbol*, so the D_800814A0 store's address
   already does not vary -- the only missing half is MEM_IN_STRUCT_P on the
   load, which `*(s16 *)(d + 0x2A)' does not have and an ARRAY_REF through an
   ARRAY-TYPED pointer does.  With the dependence gone the store schedules
   last, the base takes $t0, `addu $a2,$zero,$zero' moves to the top of the
   arm, `addu $a0,$s1,$zero' lands at its target position 771, and the
   D_800814A0 store itself is byte-exact.

   THE SPELLING IS ORDINARY C AND NEEDS NO WITNESS.  CORRECTED 2026-11-21:
   this paragraph used to say `((s16 *)d)[0x15]' was a placeholder for a
   COMPONENT_REF on an unwitnessed struct type.  That was wrong.
   MEM_IN_STRUCT_P is set when the TOP NODE of the INDIRECT_REF's address
   tree is a PLUS_EXPR (expr.c:5344) -- a question about where the cast sits,
   not about struct types -- so a pointer cast and an index is already the
   honest form and a struct would buy nothing.
   Measured equivalents, all 13 rows ON THE BODY OF THAT DATE: `u16' instead
   of `s16', and the same shape applied to d+0x8A, d+0x60, c+0x2C or the
   D_800E3DF0 entry (none of which changes anything -- the d+0x2A load is the
   only one whose dependence matters).  Two spellings that do NOT work:
   `(*(s16 (*)[1])(d + 0x2A))[0]' is 23, because a one-element array folds
   back to an INDIRECT_REF before expand; and the plain `*(s16 *)(d + 0x2A)'
   is 818 words / 46 rows on the current body, because the cast on top is a
   NOP_EXPR and the flag is never set.
   Writing the sum the other way round, `((s16 *)d)[0x15] + D_80083228',
   is 24.

   STATEMENT ORDER, re-measured on the in-struct spelling, all 60 legal orders of
   {B, D, C} x (Y before S): B,D,C,Y,S 13; D,B,C,Y,S 15; B,C,D,Y,S 21;
   B,C,Y,D,S 21; B,C,Y,S,D 21; Y,B,C,D,S 23.  On the flagless cast the same
   sweep makes B,Y,D,C,S and B,D,Y,C,S the joint best at 23, so the two
   changes have to be made together.

   WHAT IS LEFT -- 13 rows, all in case 10, one cluster:
     * the D_800DD140 pseudo takes $a3 and the `lh 0x2A' takes $a1; the
       target has them the other way round.  That is 3 rows directly
       (aligned 778, 780, 784) ...
     * ... and it drags the table's `addiu %lo' to aligned 752 (the target
       has it at 763, filling the load-delay after `lw $a0, 0x0($v0)'),
       which moves the `((u8 **)c)[0xB]' store to 768 where it is spelled
       `sw $a3, 0x2C($s1)' through `c' itself instead of the target's
       `sw $a1, 0x2C($a0)' at 776, and pushes `addu $a2,$zero,$zero' from
       750 to 763.
     * e10 takes $v0 where the target takes $a0 (aligned 755, 757).
   MEASURED AND REJECTED ON THIS BODY: every do-while(0) over every
   contiguous run of case 10's ten statements (best 13 = no change; 15 for
   the whole arm), a cancelled `v++; v--;' for all 26 names at all 11
   statement positions in case 10 (best 13 = no change), `*(u8 **)(c + 0x2C)'
   and the array form for the c+0x2C store (both 13), a separate local for
   the D_800E3DF0 entry or for the table (13), and widthsweep and stmtsweep
   over the whole body (no change).

   ====================================== 2026-11-21: 26 -> 23 rows at 812/812

   CASE 10's D_800814A0 TEMP IS `x', THE SAME LOCAL CASE 9 READS p+0xC0 INTO.
   Case 2's residue (the `addiu $v0, $v0, 0x1' / `subu' pair at aligned 224)
   is gone with it, so cases 0-9 and 11 are now byte-exact and the whole
   residue is case 10.  The sweep that found it enumerated which NAME each of
   four temp roles uses -- case 2's amount, case 9's func_80099090 result,
   case 9's p+0xC0 byte, case 10's read-modify-write -- over {k,x,y,v}, 96
   legal assignments.  Every assignment that gives the three roles
   (case2) / (case9 result) / (case9 byte + case10 rmw) three DISTINCT names
   with the last two SHARED measures 23; every one that instead shares
   case 2's with case 10's measures 26; sharing case 2 with case 9 is 37.
   The widths of the three names do not matter -- all six renamings of
   {x,y,v} onto those roles measure 23 exactly.

   ====================================== 2026-11-21: 37 -> 26 rows at 812/812

   CASE 2's `v' IS `y' -- the SAME LOCAL case 10 uses for its D_800814A0
   read-modify-write.  Residue items (1) and (3) are both gone: case 9 is now
   byte-exact.  The mechanism is which pseudo ends up a GLOBAL allocno.  With
   `v' shared by case 2 and case 9, `v' is global, conflicts with hard $v0,
   $v1 and $a0 (greg: ";; 105 conflicts: ... 2 3 4"), and global-alloc gives
   it $a1 -- so case 9 needs `move $a1, $v0' after the func_80099090 call and
   the freed $v0 lets the `lui %hi(D_800E3DF0)' float up into the load-delay
   stall the target leaves as a `nop'.  Moving case 2's temp onto `y' makes
   `v' BLOCK-LOCAL to case 9, local-alloc gives it $v0, and the whole of case
   9 falls out.  Measured on the 37-row body, case 2's temp as:
   `y' 26, `x' 31, `k' 30 (811 words), `v' (the park's own) 37.
   DO NOT re-split `v' into two locals: a separate `v9' declaration measured
   84 rows / 819 words (the frame grows 8 bytes) at every declaration
   position and every width tried.

   ====================================== 2026-11-21: 39 -> 37 rows at 812/812

   CASE 7's `D_800DCF4F = 1;' MOVES TO THE TOP OF ITS ARM, ahead of the two
   `h3' statements.  That is residue item (2) below closed: with the store
   written last the `lui %hi(D_800DCF4F)' is emitted after the whole
   `lui/addiu D_80083460' pair; written first it lands between them, which is
   where the target has it.  39 -> 37.  Measured on the same body:
   `D_800DCF4F = 1;' in the MIDDLE is 39 (no change), splitting the `+= 1'
   into an explicit load/store is 39, and dropping the `h3' local is 52/811.

   ====================================== 2026-11-20: 46 rows at 811/812 words
                                          -> 39 at 812/812

   ONE DECLARATION: `s32 k' became `s16 k'.  That is the whole change -- it
   supplies the missing word AND seven rows.  A permuter find, and the kind
   that is worth running the permuter for: nothing in the park's diagnosis
   pointed at `k''s width, and no amount of reading the residue would have.

   The park could not be measured AT ALL until today, for two reasons that had
   nothing to do with the body: csrc/ovl_dungeon_decls.h declared this
   function `void' while it returns s32 (so no parksplice placement compiled),
   and func_800B69DC -- defined in the PREVIOUS carved unit and called only
   from here -- had no prototype in this unit, because the carve generated
   cross-unit prototypes only for callees that LANDED code mentions.

   ====================================== 2026-11-21: the cast is no longer a
                                          device.  Still 11 rows, 812/812.

   The body said `(*(s16 (*)[0x16])d)[0x15]' and called it a device in
   disguise -- a two-dimensional array cast whose only purpose was to make the
   load an ARRAY_REF so MEM_IN_STRUCT_P would be set.  It is now
   `((s16 *)d)[0x15]', which measures IDENTICALLY (11 rows, 812/812) and is an
   ordinary pointer cast and index.

   WHY THE FANCY CAST WAS NEVER NEEDED.  expr.c:5344 sets MEM_IN_STRUCT_P when
   the TOP NODE of the INDIRECT_REF's address tree is a PLUS_EXPR.  That is a
   question about where the cast sits, not about struct types:

       *(s16 *)(d + 0x2A)     top node is the CAST (NOP_EXPR)  -> flag NOT set
       ((s16 *)d)[0x15]       top node is the PLUS_EXPR        -> flag SET

   No struct, no typedef and no usage witness is required.  The park's long
   note about not being able to witness a struct for `d' was answering a
   question that did not need to be asked.  See the CORRECTION entry in
   docs/matching-levers.md.

   For the record, on THIS body (with the B,D,C,Y,S statement order that the
   flag makes possible) the flagless form `*(s16 *)(d + 0x2A)' is 818 words
   and 46 rows -- the two halves are still only worth anything together.

   ====================================== 2026-11-21: PERMUTER EXHAUSTED, 11 rows

   150 minutes, 4 jobs, 219965 candidates, base score 645 -> best 485, eight
   finds.  MEASURED AGAINST THE PARK, every one is worse or equal:

       output-485-1 (best score)   19 rows, 812/812
       output-505-2                21
       output-505-1 / 515-1        22
       output-580-1                33 rows, 813 words
       output-550-1                64 rows, 814 words
       output-585-1                65 rows, 813 words
       output-630-1                11 rows, 812/812   (ties, no gain)

   `no find beats the park'.  This is the documented divergence: the permuter
   scores the whole object with its own diff, aligndiff counts rows in this
   function's words, and below roughly ten rows the two point in different
   directions -- the BEST-SCORING find is nearly the WORST on rows here.

   So all four sweeps are clean AND the permuter is spent on this body.  The
   remaining 11 rows are the single local-alloc choice described above
   ($a3 vs $a1 for the D_800DD140 table pseudo, both quantities block-local).
   The next spend should be hand analysis of QTY_CMP_PRI on a CHANGED body,
   not another search on this one.
 */
void func_800B6AC4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk80083160 *q;
    u8 *g;
    u8 *h;
    u8 *h2;
    u8 *h3;
    u8 *h4;
    u8 *n;
    u8 *m;
    u8 *r;
    u8 *e2;
    u8 *e7;
    u8 *e9;
    u8 *e10;
    u8 *n5;
    u8 *base9;
    u8 *base10;
    u8 *w;
    void *s;
    s16 k;
    s32 x;
    u32 y;
    s32 v;
    s16 out[4];

    g = (u8 *)&D_80083160;
    switch (p[0x9B])
    {
    case 0:
        q = &D_80083160;
        if ((((q->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7) == 2)
        {
            *(u8 **)(c + 0x2C) = D_800DD108;
            func_80048A44(c, D_800DD108[((q->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            *(s16 *)(p + 0x96) = 0x12;
            p[0x9B]++;
        }
        else
        {
            *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
        }
        break;
    case 1:
        if (--*(s16 *)(p + 0x96) == 0)
        {
            func_800A56E0(0x701);
        }
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            *(u8 **)(c + 0x2C) = D_800DD110;
            func_80048A44(c, D_800DD110[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
            if ((D_80013714 & 1) == 0)
            {
                func_800B1768(0x22, 0xF2, 0xBA, 0x105, 0, 8);
                func_800B1768(0x23, 0xE0, 0xCE, 0x104, 1, 8);
            }
            ((u8 **)p)[0x30] = *(u8 **)(p + 0xBC);
            *(u32 *)D_80081484 = **(u32 **)(p + 0xBC);
            ((UnkDungeonActor *)p)->unk_8C = func_800B69DC;
        }
        break;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            h = (u8 *)&D_80083460;
            *(u16 *)(h + 0x2) |= 4;
        }
        else
        {
            *(u16 *)(c + 0x14) |= 0x6800;
        }
        if (D_80083462 & 4)
        {
            break;
        }
        k = 0;
        if (*(u16 *)0x80013714 & 1)
        {
            r = func_8009F868();
            if (r == NULL)
            {
                func_800B1768(0x22, 0xF2, 0xBA, 0x105, 0, 8);
                func_800B1768(0x23, 0xE0, 0xCE, 0x104, 1, 8);
                break;
            }
            D_800E3544 = r[1] & 0xF8;
            if (D_800E3544 == 0x60)
            {
                k = 1;
            }
            else
            {
                func_8009F988();
                *(u8 **)(c + 0x2C) = D_800DD118;
                func_80048A44(c, D_800DD118[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
                *(u16 *)(c + 0x14) &= 0xF7FF;
                *(s32 *)(p + 0x8C) = 0;
                p[0x9B] = 4;
                break;
            }
        }
        else
        {
            if (*(s32 *)(g + 0x8) & 0x40)
            {
                k = 1;
            }
        }
        if (k)
        {
            func_8009F644(d, 0x60, 0, 0);
            func_800A56E0(0x500);
            *(u16 *)(c + 0x14) &= 0x97FF;
            p[0x9B] = 3;
            y = (func_800A6D30() & 3) + 1;
            (*(u8 **)(p + 0xBC))[2] -= y;
            e2 = *(u8 **)(p + 0xBC);
            if (*(s8 *)(e2 + 0x2) <= 0)
            {
                e2[2] = 0;
                *(u8 **)(c + 0x2C) = D_800DD118;
                func_80048A44(c, D_800DD118[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
                *(u16 *)(c + 0x14) &= 0xF7FF;
                p[0x9B] = 5;
                *(s16 *)(d + 0x8A) = func_8009904C(*(u8 **)(p + 0xBC));
            }
            func_800B4C7C(0xA4, d, (s16)(0x64 - *(s8 *)(*(u8 **)(p + 0xBC) + 0x2)), 0);
        }
        else
        {
            if (D_80013714 & 1)
            {
                break;
            }
            if ((*(s32 *)(g + 0x10) & 0x20) == 0)
            {
                break;
            }
            func_8009F644(d, 0xA8, 0, 0);
            *(u8 **)(c + 0x2C) = D_800DD118;
            func_80048A44(c, D_800DD118[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            *(u16 *)(c + 0x14) &= 0xF7FF;
            func_800A56E0(0x515);
            *(s32 *)(p + 0x8C) = 0;
            p[0x9B] = 4;
        }
        break;
    case 3:
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            h2 = (u8 *)&D_80083460;
            if ((*(u16 *)(h2 + 0x2) & 2) == 0)
            {
                func_80099F70(*(u8 **)(d + 0x5C));
                func_80099F04(*(u8 **)(d + 0x5C));
                *(u16 *)(h2 + 0x2) |= 0x812;
            }
            p[0x9B] = 2;
        }
        break;
    case 4:
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            func_80098B38(D_80081484);
            ((UnkDungeonActor *)p)->unk_8C = func_8008ACDC;
            D_800E296C &= 0xFFDFFFFF;
        }
        break;
    case 5:
        n5 = (u8 *)func_80098CF8(p, b, c, D_80081484);
        if (n5 != NULL)
        {
            *(s16 *)(n5 + 0xCC) = 2;
            p[0x9A] = 0x22;
            p[0x9B] = 6;
        }
        break;
    case 6:
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            if (func_8003DE58(*(u8 **)(c + 0x8), c, out, 0) == 0)
            {
                out[1] = 0;
                out[0] = 0;
                out[2] = -D_800DDC40[0];
            }
            func_800B8024(*(s16 *)((u8 *)b + 0x2) + out[0], *(s16 *)((u8 *)b + 0x6) + out[1],
                          *(s16 *)((u8 *)b + 0xA) + out[2]);
            p[0x9B]++;
        }
        break;
    case 7:
        D_800E2968 = 1;
        n = ((UnkDungeonSpawner)func_800A0B94((((u8 **)p)[0x30])[0],
                                              func_800A1618((((u8 **)p)[0x30])[0], 2), 1))(
            2, c[0x24], c[0x25], (s16)(*(u16 *)(d + 0x88) - 0x20));
        if (n == NULL)
        {
            break;
        }
        func_8003E188(n[0x13], 0);
        func_800A90E8(((u8 **)p)[0x30]);
        e7 = *(u8 **)(n - 0x14);
        func_8009A3D0(e7[0x24], e7[0x25], 0x300);
        if (func_8003DE58(*(u8 **)(c + 0x8), c, out, 0) == 0)
        {
            out[1] = 0;
            out[0] = 0;
            out[2] = -D_800DDC40[0];
        }
        m = *(u8 **)(n - 0x18);
        *(s16 *)(m + 0x2) = *(u16 *)((u8 *)b + 0x2) + (u16)out[0];
        *(s16 *)(m + 0x6) = *(u16 *)((u8 *)b + 0x6) + (u16)out[1];
        *(s16 *)(m + 0xA) = *(u16 *)((u8 *)b + 0xA) + (u16)out[2];
        *(s16 *)(n + 0x88) = *(s16 *)(m + 0xA);
        *(u8 **)(d + 0x60) = n;
        func_80042640((Unk42710 *)n, n[0x13]);
        func_800424E0(n, n[0x13], NULL);
        n[0x43] = 0xFF;
        *(u8 **)(n + 0x60) = d;
        *(u32 *)(n + 0x1C) |= 0x80000;
        *(u32 *)(n + 0x14) |= 0x104000;
        *(u16 *)(*(u8 **)(d + 0x60) + 0x98) |= 0xC;
        func_80069E78(n + 0x34, func_8003C06C(n[0x13]));
        *(u32 *)(n + 0x18) = D_800835E4[D_800E3D7C[0x11]];
        while (D_800835E4[n[0x11] + 1] <= *(u32 *)(n + 0x18))
        {
            func_800A1D4C(n, 0);
        }
        n[0x28] = n[0x29];
        n[0x26] = n[0x68];
        n[0x27] = n[0x69];
        if ((D_80013714 & 1) == 0)
        {
            D_800DCF4F = 1;
            h3 = (u8 *)&D_80083460;
            *(u16 *)(h3 + 0xA) += 1;
        }
        s = func_800990FC();
        w = func_8009929C(8, s);
        w = func_80099194((u8 *)D_8007359C[0][n[0x13]].unk_04, w);
        w = func_80099194(D_800E0A2A, w);
        w = func_80099734(n, w);
        w = func_80099194(D_800E0A33, w);
        if ((D_80013714 & 1) == 0)
        {
            w = func_8009929C(0x11, w);
            w = func_8009929C(0x4C, w);
            w = func_80099254(D_800E0458, w);
        }
        func_80099290(w);
        func_800A5720(s);
        *(s16 *)(p + 0x96) = 0x20;
        p[0x9B]++;
        if ((D_80013714 & 1) == 0)
        {
            h4 = (u8 *)&D_80083460;
            if (*(s16 *)(h4 + 0xA) >= 2)
            {
                *(u16 *)(h4 + 0xA) -= 1;
            }
        }
        else
        {
            h4 = (u8 *)&D_80083460;
            if (*(s16 *)(h4 + 0xA) > 0)
            {
                *(u16 *)(h4 + 0xA) -= 1;
            }
        }
        break;
    case 8:
        if (D_8008346A == 0)
        {
            if (--*(s16 *)(p + 0x96) > 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0 &&
                *(s32 *)(g + 0x10) == 0)
            {
                break;
            }
            *(u8 **)(c + 0x2C) = D_800DD130;
            func_80048A44(c, D_800DD130[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        break;
    case 9:
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            *(u8 **)(c + 0x2C) = D_800DD138;
            func_80048A44(c, D_800DD138[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            *(u16 *)(c + 0x14) |= 0x200;
            func_80093C70(p, (u8 *)b, c);
            func_80093D8C(p, (u8 *)b, c);
            v = func_80099090();
            base9 = (u8 *)0x80010000;
            e9 = *(u8 **)(base9 + *(s16 *)(d + 0x8A) * 4 + 0x29C);
            e9[1] = 0x13;
            x = (((u8 **)p)[0x30])[0];
            e9[3] = v;
            e9[2] = 0;
            e9[0] = x;
            D_800E3DF0[v] = (s32)&D_800E3E48[v];
            D_800E3E48[v] = *(Unk800E3E48 *)*(u8 **)(d + 0x60);
            *(u32 *)(*(u8 **)(d + 0x60) + 0x1C) |= 0x400000;
            *(u32 *)(*(u8 **)(d + 0x60) + 0x14) &= 0xFFEFFFFF;
            func_800A56E0(0x704);
            p[0x9B]++;
        }
        break;
    case 10:
        if (*(u32 *)(*(u8 **)(d + 0x60) + 0x1C) & 0x800000)
        {
            func_8009A028(*(u8 **)(d + 0x60));
            base10 = (u8 *)0x80010000;
            e10 = *(u8 **)(base10 + *(s16 *)(d + 0x8A) * 4 + 0x29C);
            e10 = (u8 *)D_800E3DF0[e10[3] & 0x1F];
            *(u32 *)(e10 + 0x14) &= ~0x4000;
            ((u8 **)c)[0xB] = D_800DD140;
            *(u16 *)(*(u8 **)(d + 0x60) - 0x2) |= 0x8000;
            x = D_800814A0 | 0x8000;
            D_800814A0 = x;
            /* The array-typed cast is the only part of this arm that is not
               an honest spelling: it is there so the d+0x2A load is an
               ARRAY_REF and gets MEM_IN_STRUCT_P, which is what lets
               true_dependence stop the D_800814A0 store depending on it. A
               COMPONENT_REF on a real typedef for `d' would do the same and
               would be the right form; this park has no witness for one.
               Written `*(s16 *)(d + 0x2A)' the arm is 23 rows, not 11. */
            func_80048A44(c, D_800DD140[((D_80083228 + ((s16 *)d)[0x15] + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        break;
    case 11:
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            *(u16 *)(c + 0x14) &= 0xFDFF;
            *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
        }
        break;
    }
}
