/* func_s51_80024B38 -- the object's master state machine, a five-way jump
   table on the node's +0xA.  PARKED at 746 words / 746 words / 95 differing
   (ovlverify) and 94 ALIGNED differences (aligndiff), of which 92 are
   REGISTER-ONLY rows and 2 are one insert/delete pair.  Family: ONE member
   (dngfam: 1 member, 746 words), so nothing replicates off it.

   THE SHAPE IS SETTLED AND THE RESIDUE IS ENTIRELY REGISTER ALLOCATION.
   Every opcode, immediate and displacement matches except one scheduling
   position; see the MEASUREMENT TABLE for the twenty-odd spellings that were
   swept to establish that.

   The donor is the LANDED func_s58_80025D68 (csrc/ovl_dng_s58.c), found by
   dngcallee at 13 of 23 symbols with a clean gap to the next tier at 11/23;
   dngtwin found NO carved sibling at ratio >= 0.70 for this body at all.
   Frames: donor addiu $sp, -0x58 with EIGHT saves, this body addiu $sp, -0x58
   with TEN (s0..s7, fp, ra), so the donor's BUDGET facts are void here and
   only its SELECTION facts were taken.  Eight of those carried and are listed
   below.

   THIS FUNCTION OWNS jtbl_s51_80024048: FIVE words, cases 0..4, no pad,
   dispatch bound sltiu 0x5, entries
   .Ls51_80024BA8 / .Ls51_80024C4C / .Ls51_80024D60 / .Ls51_800253A8 /
   .Ls51_80025618, five DISTINCT targets, ascending case value == ascending
   entry address.  Case 0 FALLS THROUGH into case 1 and every other arm ends
   at the epilogue, so default: needs no label.  A standalone
   tools/cdkgcc -O2 -G0 -S compile of this body emits
   .rdata / .align 3 / $L70: .word $L3, $L4, $L8, $L49, $L66 with $L3 < $L4 <
   $L8 < $L49 < $L66 in text order -- i.e. the same five-entry ascending
   order, no pad word.  grep -c for a literal pool in that same .s is ZERO: no
   $LC is emitted anywhere in this body.

   WHAT THE RESIDUE IS, EXACTLY.  Nine callee-saved registers are live in
   case 3's spawn loop, so the assignment there is a pure allocno_compare
   ranking of nine allocnos, and exactly ONE of them is out of place:

       value                           target    this body
       s   (UnkSlot *, n->unk_0C)      $s0       $s0
       m   (UnkVec *, n->unk_08)       $s1       $s1
       q   ((u8 *)n + 0x20, case 3)    $s2       $s4
       n   (UnkNode *)                 $s3       $s2
       i   (loop counter)              $s4       $s3
       p   (parameter)                 $s5       $s5
       d1/d2/d3 (D_80083780)           $s6       $s6
       h3 (D_80082E80) / func ptr      $s7       $s7
       func ptr (case 3)               $fp       $fp

   So q has to be ranked between m and n; it is ranked two places LATER here,
   n and i each slide down one register, and the 92 rows are that three-cycle
   plus its consequences.  global.c find_reg gives the lowest free callee-saved
   register to the highest-priority allocno, and allocno_compare's priority is
   floor_log2(n_refs) * n_refs / live_length (allocno_size is PSEUDO_REGNO_SIZE
   in WORDS, so it is 1 for every allocno here and multiplies nothing).  The
   two loops' hoisted 0x80 constants and case 3's q all land in $s2 in the
   target through the SAME single rank change, because find_reg's pass 0 only
   offers registers already in regs_used_so_far: once q has taken $s2, the
   loop1 and loop3 constants -- which do not conflict with q -- pick $s2 up in
   pass 0 without needing any rank of their own.

   THE ARITHMETIC, AND IT IS WHY NO LEGAL SPELLING REACHES IT.  From -da on a
   single-function probe of this body, .flow gives

       q    (84)   10 refs / 51 insns   floor_log2 3   pri 3*10/51  = 0.588
       n    (83)   58 refs / 271 insns  floor_log2 5   pri 5*58/271 = 1.070
       i    (97)   47 refs / 380 insns  floor_log2 5   pri 5*47/380 = 0.618
       0x80 (269)  13 refs / 96 insns   floor_log2 3   pri 3*13/96  = 0.406
       m    (95)   82 refs / 186 insns                 pri          = 2.645
       s    (96)  122 refs / 144 insns                 pri          = 5.083

   and the printed ;; 24 regs to allocate: order is
   96 95 540 83 91 92 93 97 180 84 80 562 589 269 393 85 86 87 81 82 90 218
   344 466 -- s, m, a $v0 temp, n, the case 3/4 g,e pointers, i, ..., q, p,
   ... which is exactly the priority order above.  For q to reach the target's
   position it needs pri > 1.070, i.e. floor_log2(r)*r > 54.6 at length 51,
   i.e. r >= 16 against the 10 it has.

   AND EVERY ONE OF THOSE TEN REFERENCES IS FORCED BY THE LISTING.  q is read
   twice in case 3 (sh $v0, 0x2A($s2) at 800253FC and 800254A4) and set once;
   it is set once more and read three times in the 0xB block (lw $v1,
   -0x18($v1) then lw 0x0/0x4/0x8).  Every other n-relative access in the
   target is addressed off $s3 and not off $s2 -- sh 0x4A($s3), sw 0x10($s3),
   lw 0xC($s3) twice, lw 0x8($s3), the six swl/swr at 0x92..0x9D($s3) and
   addiu $v0, $s3, 0x92 -- so no reference can be moved from n to q without
   changing a base register, which is the trap below.  n's 58 and q's 10 are
   therefore both properties of the target's own instruction stream, not of
   this source, and the ranking they produce is the one measured here.

   flow.c:2122 adds loop_depth per reference, so the only lever left on those
   counts is a LOOP NOTE, and a do-while probe confirms the mechanism and its
   threshold to the digit -- see the DIAGNOSTIC PROBES table.  Nothing in the
   emitted stream can distinguish a do { } while (0) from its absence, so if
   the original really did wrap these stores in a macro, the macro layer is
   where this body's last 92 rows went.

   A BETTER SCORE THAT IS PROVABLY WRONG, recorded so the next agent does not
   bank it.  Addressing case 3's n->unk_10 and n->unk_0C through q as
   *(u32 *)(q - 0x10) and *(u32 *)(q - 0x14) moves q's references the right way
   and measures 746 words / 73 ALIGNED differences, 21 better than the kept
   body.  It is wrong: it emits sw $s8, -16($s3) and lw $s0, -20($s3) where
   the target has sw $fp, 0x10($s3) and lw $s0, 0xC($s3), and aligndiff reports
   those two as X rows rather than R.  The kept shape is the provably right
   one.

   MEASUREMENT TABLE -- every row measured with aligndiff on this body, which
   resynchronises; ovlverify's position-by-position figure is one higher (95)
   and is the landing gate only.  R = register-only rows.

   | spelling                                                | words | aligned |
   | THE KEPT BODY                                           |  746  |  94 (92 R) |
   | --- shape faults found and fixed, in the order found -- |       |         |
   | first compile                                           |  753  |  511 ovlverify / 163 |
   | ?: written cond == 2 ? 0x4300 : 0x300                   |  753  |  +2 rows |
   | loop1/loop3 0x80 byte stores written ONCE after the     |       |         |
   |   if/else instead of inside both arms                   |  747  |  127    |
   | the 0xB block's D_80082E80 as a bare symbol, no local   |  747  |  127    |
   | the 0xB block's m stores as m->unk_00 field refs        |  747  |  127    |
   | (0x10 - *(s16 *)(p + 0x20)) * 8 stored twice            |  753  |  +4 words |
   | --- the symbol-base locals -------------------------- |       |         |
   | d1/d2/d3 assigned BEFORE their loops                    |  746  |  104    |
   | d1/d2/d3 merged into ONE local                          |  741  |  233    |
   | h1/h2/h3 merged into ONE local                          |  745  |  208    |
   | d3 assigned before case 3's func_8003DF74 gate          |  746  |  82     |
   | --- the local SET ------------------------------------ |       |         |
   | a separate local for the 0xB block's -0x18 pointer      |  746  |  179    |
   |   (same figure whether declared 4th or last)            |       |         |
   | the 0xB block's pointer reusing h2                      |  747  |  195    |
   | the 0xB block's pointer reusing h1                      |  747  |  187    |
   | g1/g2 and e1/e2 merged across cases 3 and 4             |  747  |  107    |
   | i split per loop (5 locals)                             |  744  |  194    |
   | s split per assignment (8 locals)                       |  743  |  187    |
   | m split per block (4 locals)                            |  746  |  196    |
   | n split per block (4 locals)                            |  746  |  94  NOT MEASURABLE |
   | --- declaration ORDER, 5 permutations ---------------- |       |         |
   | i first / n last / q first / RECT+DVECTOR last          |  746  |  94 all four |
   | the whole list reversed                                 |  746  |  130    |
   |   (that one moves the two STACK objects and so moves    |       |         |
   |    code; it is not a tie-break result)                  |       |         |
   | --- widths and temps -------------------------------- |       |         |
   | case 0's s16 t temp removed                             |  747  |  100    |
   | s32 t instead of s16 t                                  |  746  |  94  FREE |
   | s16 k instead of s32 k                                  |  746  |  94  FREE |
   | k inlined, e1[0xD] = e1[0xE]                            |   -   |  31 on the probe, FREE |
   | --- case 3's q ---------------------------------------- |       |         |
   | the first 0x2A store off n, q only for the second       |  745  |  190    |
   | the second 0x2A store off n, q only for the first       |  743  |  197    |
   | q re-assigned before the second store (cse deletes it)  |  746  |  94  FREE |
   | q as an s16 * with y[0x15] array refs                   |  746  |  179    |
   | q's 0xB-block assignment moved after m = n->unk_08      |  746  |  94  FREE |
   | --- the 0x80 byte stores ----------------------------- |       |         |
   | as s->unk_0C[2]/[1]/[0] field refs (MEM_IN_STRUCT_P)    |  746  |  94  FREE |
   | as ((u8 *)s)[0xE]/[0xD]/[0xC] array refs                |  746  |  94  FREE |
   | --- case 3 and 4's g,e blocks (19 of the rows) -------- |       |         |
   |   priced on the 31-row probe body, where they dominate  |       |         |
   | e1 read BEFORE the 0x10000000 or                        |   -   |  31  FREE |
   | no e1 local, the three stores off the double deref      |  750  |  40     |
   | no g1 local, the symbol inline at all three sites       |  747  |  41     |
   | case 4 given an e local for its -0x14 pointer           |  747  |  49     |
   | case 4's 0x808080 store before the 0xEFFFFFFF and       |   -   |  33     |
   | --- conjunctions with the split the listing demands --- |       |         |
   |   the 0xB-block pointer as its own local is what the    |       |         |
   |   target has ($v1, call-clobbered, so block-local) and  |       |         |
   |   it is 179 ALONE, which is the signature of a fix that |       |         |
   |   is only right in combination -- so it was held and    |       |         |
   |   everything else varied on top of it:                  |       |         |
   | 0xB split alone                                         |  746  |  179    |
   | 0xB split + n split per block                           |  746  |  179    |
   | 0xB split + g1/g2,e1/e2 merged                          |  747  |  192    |
   | 0xB split + the byte stores as unk_0C[] field refs       |  746  |  179    |
   | 0xB split + i split per loop                            |  744  |  175    |
   | 0xB split + d3 before case 3's gate                     |  746  |  77  WRONG |
   | --- provably wrong, better score --------------------- |       |         |
   | case 3's unk_10 and unk_0C addressed through q          |  746  |  73  WRONG BASE |
   | d3 assigned at case 3's loop-body TOP LEVEL             |  746  |  82  WRONG |
   | the same, crossed with the 0xB split                    |  746  |  77  WRONG |

   THE TWO WRONG-SHAPE IMPROVEMENTS SHARE A TELL, and both are the trap the
   brief warns about: a better row count on a shape the listing rules out.
   The q-addressed one moves a base register (above).  The d3 one DROPS case
   3's preheader hoist: with d3 = D_80083780 at the loop body's top level
   loop.c does NOT move it, so target words 550 and 551
   (lui $v0, %hi(D_80083780) / addiu $s6, $v0, %lo(D_80083780)) are absent and
   the lui appears inside the loop instead, paying three further rows at
   80025454.  Assigned at its POINT OF USE, inside the func_8003DF74 then-arm,
   the same assignment IS hoisted and the preheader matches word for word.
   That direction is the opposite of the intuitive one and it is a measurement,
   not an explanation: the mechanism inside loop.c's move_movables was not
   established, only the two figures.  The same placement rule holds for d1
   and d2 -- assigned before their loops the body is 746 / 104, assigned at
   their point of use inside the loop it is 746 / 94.

   DIAGNOSTIC PROBES -- do { } while (0) wrappers, which emit nothing and only
   raise flow.c:2122's loop_depth.  These are probes, NOT candidate spellings,
   and they are listed because they pin the mechanism and the threshold:

   | probe                                           | refs | pri   | aligned |
   | none                                            |  13  | 0.406 |  94     |
   | 1 wrapper on loop1's three 0x80 stores          |  19  | 0.792 |  94     |
   | 2 wrappers                                      |  25  | 1.042 |  94     |
   | 3 wrappers                                      |  31  | 1.292 |  73     |
   | 1 wrapper on case 3's two q stores              |  12  | 0.706 |  79     |
   | 2 wrappers                                      |  14  | 0.824 |  79     |
   | 3 wrappers                                      |  16  | 1.255 |  31     |
   | 4 wrappers                                      |  18  | 1.412 |  31     |
   | 3 wrappers on q + a separate 0xB-block local    |   -   |   -   |  75     |
   | 5 wrappers on q + a separate 0xB-block local    |   -   |   -   |  27     |

   READ THE 0x80 COLUMN AGAINST pri(n) = 1.070: two wrappers give 1.042 and do
   NOT move the body, three give 1.292 and do.  The predicted crossing sits
   between them and the measurement lands on it, which is why the cause is
   stated as ARITHMETIC rather than as a guess.  The q column crosses the same
   1.070 between 0.824 and 1.255 and moves 94 -> 31 when it does, and 31 is
   where the two g,e blocks' 19 rows and case 3's 8-row schedule are all that
   is left.  No probe reaches 0, so a device would not land this body either.

   THE ONE NON-REGISTER ROW, which is upstream of the allocator and may be the
   real handle: case 3's addiu $s2, $s3, 0x20 sits at target word 559, between
   the addiu $a1, %lo(D_80045340) and the addiu $v0, 0xC, and this body emits
   it at word 556, before the lui $a1.  sched1 runs BEFORE local-alloc and
   global-alloc (toplev.c), so this schedule is not a consequence of the
   register residue and could be a cause of it: it changes q's live length,
   which is the denominator of the one priority that has to move.  Three
   statement orders were swept around it -- q before the stores (kept, 94),
   n->unk_10 before q (97), the first store off n (190) -- and none produced
   the target's slot.

   DONOR FACTS THAT CARRIED, from func_s58_80025D68's own comment, and all
   eight are in the kept body:
   state 1's p-accesses as ARRAY REFS ((s16 *)p)[0x10], [0x13], [0x5]; the
   +0x26 store BEFORE the +0xA bump with an s16 temp carrying the bump; the
   --*(s16 *)(p + 0x20) <= 0 compound decrement gate; the
   (D_80082E94 & 0x8000) != 0 || --*(s16 *)(p + 0x20) < 0 tail gate; one local
   per block for the D_80083780 and D_80082E80 bases; the mover written as a
   store then a += of the d base; case 4's g = (u8 *)&D_80083460 with the +0xC,
   +0xA and two 0x8000 bits; and (a + b) / 2 for the signed midpoint.
   The donor's rec[8] stack record table and its (rand() & 0x1FFF) << 6 jitter
   do not appear here at all.

   THE convert.c CAST IS LOAD-BEARING AT SEVEN SITES.  Every jitter store is
   *(s16 *)dst = (s16)(*(u16 *)src - K) + (rand() & M), with the cast around
   the SUBTRACTION and narrowing to the destination's width.  Without it
   convert_to_integer takes the narrowed type's signedness from the u16 first
   operand, -0x10 becomes 0xFFF0, and cc1psx emits ori + addu where the target
   has addiu with a negative immediate.  K/M are 0x10/0x1F in case 2's first
   spawn loop, 0x8/0xF for x and y and 0x10/0x1F for z in its second, and
   0x20/0x3F in case 3.

   AN ARITY ALIAS IS REQUIRED.  func_s51_800243D8 is DEFINED in this unit
   taking one parameter and is defined ABOVE this body, so its prototype is in
   scope; case 1's call site sets $a0, $a1 AND $a2 at 80024C64, 80024C7C and
   80024C8C, all three dead after the call and computed only for it.  The
   DECLARATIONS block therefore carries
   void func_s51_800243D8_3(u8 *, UnkVec *, u8 *) __asm__("func_s51_800243D8");
   and the landed one-parameter definition is left alone.  Calling it with one
   argument loses the two addu and two words.

   The three RotTransPers warnings declcheck reports for this unit are
   PRE-EXISTING in csrc/ovl_dng_s51.c at line 337, inside the landed
   func_s51_800244E4, and are not this body's.  relcand reports ok, 15 data
   symbols and 37 calls.  All ten already-landed bodies in the unit still
   measure 0 differing with this body and its declarations spliced in.

   THE ONE THING THE DECLARATIONS BLOCK BELOW DELIBERATELY OMITS, and the
   reason, because it is also a parkcheck gap worth knowing about.  This body
   needs the typedef UnkDngS58Name12 (u8 b[0xC], alignment 1, which is what
   forces the lwl/lwr + swl/swr pairs for the 12-byte D_800256EC copy at
   8002536C..80025398).  It is NOT in the block, because the LANDED
   func_s51_800243D8 in this same unit uses it too, so dngland keeps it and a
   copy in the park's block is a `redefinition of struct UnkDngS58Name12'
   that stops the whole unit compiling.  Everything else below is an extern or
   a prototype, and a duplicate of either is legal C, so the block is
   otherwise self-contained.
   parkcheck.py --splice reports this park clean WITH the typedef present as
   well as without it, so it is not checking the splice against the committed
   unit's own declarations; splicing by hand into csrc/ovl_dng_s51.c is what
   found it.

DECLARATIONS
extern s16 D_80082E86;
extern u16 D_80082E94;
extern u8 D_80082E80[];
extern u8 D_800256EC[];
extern u8 D_80025704[];
extern u8 D_80025710[];
extern u8 D_800DE870[];
extern u8 D_800DEB28[];
extern u8 D_800DED28[];
extern s16 D_8002571C;
extern u8 D_80045340[];
UnkNode *func_8003FC64(s32);
s32 func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
void func_8003DB94(void *, void *, s32);
void func_800A56E0(s32);
s32 func_80053EF0(s32);
void func_800B8FC8(u8 *, s32 *, s32 *, s32, s32);
s32 rand(void);
u8 *func_s51_800249F0(u8 *p, UnkVec *b, u8 *c, u8 *s, s32 e, s32 f, s32 g, s32 col);
void func_s51_80024798(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz);
void func_s51_800248B8(u8 *p, u8 *b, u8 *c);
void func_s51_8002492C(u8 *p, u8 *b, u8 *c);
void func_s51_800243D8_3(u8 *, UnkVec *, u8 *) __asm__("func_s51_800243D8");
END DECLARATIONS
*/
void func_s51_80024B38(u8 *p, UnkVec *b, u8 *c)
{
    RECT r;
    DVECTOR w;
    UnkNode *n;
    u8 *q;
    u8 *d1;
    u8 *d2;
    u8 *d3;
    u8 *h1;
    u8 *h2;
    u8 *h3;
    u8 *g1;
    u8 *g2;
    u8 *e1;
    u8 *e2;
    UnkVec *m;
    UnkSlot *s;
    s32 i;
    s32 k;
    s16 t;

    *(s16 *)(p + 0x2A) = *(u16 *)(p + 0x2A) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s32 *)(D_800814A8 + 0xF4) = 0;
        D_80082E86 = 6;
        *(u8 **)(p + 0x64) = func_s51_800249F0(p, b, c, D_80025704, 1, 0x1000, 0x1000, 0x808080);
        *(u8 **)(p + 0x68) = func_s51_800249F0(p, b, c, D_80025710, 2, 0xE00, 0xE00, 0xE0E0E0);
        t = *(u16 *)(p + 0xA) + 1;
        D_8002571C = 1;
        *(s16 *)(p + 0xA) = t;
    case 1:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        ((s16 *)p)[0x10] = 0x14;
        *(s16 *)(D_800814A8 + 0xA6) = *(u16 *)(D_800814A8 + 0xA6) - 1;
        *(u8 *)(D_800814A8 + 0xA8) = p[0x8];
        ((s16 *)p)[0x13] = *(u16 *)(D_800814A8 + 0x2A);
        ((s16 *)p)[0x5] = ((u16 *)p)[0x5] + 1;
        func_s51_800243D8_3(p, b, c);
        if (*(u8 **)(p + 0x68) != NULL)
        {
            r.x = 0x340;
            r.y = 0x100;
            r.w = 0x40;
            r.h = 0x40;
            w.vx = 0x360;
            w.vy = 0x120;
            func_800B8FC8(*(u8 **)(p + 0x68), (s32 *)&r, (s32 *)&w, 0, 1);
        }
        if (*(u8 **)(p + 0x64) != NULL)
        {
            r.x = 0x340;
            r.y = 0x100;
            r.w = 0x40;
            r.h = 0x40;
            w.vx = 0x360;
            w.vy = 0x120;
            func_800B8FC8(*(u8 **)(p + 0x64), (s32 *)&r, (s32 *)&w, 1, 1);
        }
        break;
    case 2:
        if (--*(s16 *)(p + 0x20) <= 0)
        {
            *(s16 *)(p + 0x20) = 0x10;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        if (*(s16 *)(p + 0x20) == 0x13)
        {
            func_800A56E0(func_80053EF0(4) != 2 ? 0x300 : 0x4300);
        }
        if (*(s16 *)(p + 0x20) < 0xB)
        {
            h1 = D_80082E80;
            if (func_8003DF74(*(u8 **)(h1 + 0x8), (UnkSlot *)h1, (s16 *)(p + 0xC), 0) != 0)
            {
                for (i = 0; i < 4; i++)
                {
                    func_s51_80024798((UnkNode *)(D_800814A8 - 0x20), *(s16 *)(p + 0x26), 0x2020F0,
                                      (rand() & 0x7F) | 0x60, *(s16 *)(p + 0xC), *(s16 *)(p + 0xE),
                                      *(s16 *)(p + 0x10));
                }
                for (i = 0; i < 4; i++)
                {
                    n = func_8003FC64(0x212);
                    if (n != NULL)
                    {
                        *(s16 *)((u8 *)n + 0x4A) = 8;
                        n->unk_10 = (u32)func_s51_8002492C;
                        func_8004491C(n, (u32)D_80045340);
                        s = (UnkSlot *)n->unk_0C;
                        s->unk_14 |= 0xC;
                        m = n->unk_08;
                        if ((rand() & 1) == 0)
                        {
                            s->unk_14 |= 1;
                        }
                        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0xC);
                        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0xE);
                        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x10);
                        d1 = D_80083780;
                        *(s16 *)((u8 *)m + 0x2) += *(u16 *)(d1 + 0x2);
                        *(s16 *)((u8 *)m + 0x6) += *(u16 *)(d1 + 0x6);
                        *(s16 *)((u8 *)m + 0xA) += *(u16 *)(d1 + 0xA);
                        if (i != 0)
                        {
                            *(s16 *)((u8 *)m + 0x2) =
                                (s16)(*(u16 *)((u8 *)m + 0x2) - 0x10) + (rand() & 0x1F);
                            *(s16 *)((u8 *)m + 0x6) =
                                (s16)(*(u16 *)((u8 *)m + 0x6) - 0x10) + (rand() & 0x1F);
                            *(s16 *)((u8 *)m + 0xA) =
                                (s16)(*(u16 *)((u8 *)m + 0xA) - 0x10) + (rand() & 0x1F);
                        }
                        s = (UnkSlot *)n->unk_0C;
                        if (i != 0)
                        {
                            s->unk_10 = 0x20;
                            s->unk_1E = 0x1000;
                            s->unk_1C = 0x1000;
                            func_8003DB94(s, D_800DED28, 0);
                            *((u8 *)s + 0xE) = 0x80;
                            *((u8 *)s + 0xD) = 0x80;
                            *((u8 *)s + 0xC) = 0x80;
                        }
                        else
                        {
                            s->unk_1E = 0x2000;
                            s->unk_1C = 0x2000;
                            s->unk_10 = 0x20;
                            func_8003DB94(s, D_800DEB28, 0);
                            *((u8 *)s + 0xE) = 0x80;
                            *((u8 *)s + 0xD) = 0x80;
                            *((u8 *)s + 0xC) = 0x80;
                        }
                    }
                }
                if (*(s16 *)(p + 0x20) < 6)
                {
                    *(s16 *)(p + 0x18) = (*(s16 *)(p + 0xC) + *(s16 *)(p + 0x12)) / 2;
                    *(s16 *)(p + 0x1A) = (*(s16 *)(p + 0xE) + *(s16 *)(p + 0x14)) / 2;
                    *(s16 *)(p + 0x1C) = (*(s16 *)(p + 0x10) + *(s16 *)(p + 0x16)) / 2;
                    for (i = 0; i < 4; i++)
                    {
                        func_s51_80024798((UnkNode *)(D_800814A8 - 0x20), *(s16 *)(p + 0x26),
                                          0x2020F0, (rand() & 0x7F) | 0x60, *(s16 *)(p + 0x18),
                                          *(s16 *)(p + 0x1A), *(s16 *)(p + 0x1C));
                    }
                    for (i = 0; i < 4; i++)
                    {
                        n = func_8003FC64(0x212);
                        if (n != NULL)
                        {
                            *(s16 *)((u8 *)n + 0x4A) = 8;
                            n->unk_10 = (u32)func_s51_8002492C;
                            func_8004491C(n, (u32)D_80045340);
                            s = (UnkSlot *)n->unk_0C;
                            s->unk_14 |= 0xC;
                            m = n->unk_08;
                            if ((rand() & 1) == 0)
                            {
                                s->unk_14 |= 1;
                            }
                            *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0x18);
                            *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0x1A);
                            *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x1C);
                            d2 = D_80083780;
                            *(s16 *)((u8 *)m + 0x2) += *(u16 *)(d2 + 0x2);
                            *(s16 *)((u8 *)m + 0x6) += *(u16 *)(d2 + 0x6);
                            *(s16 *)((u8 *)m + 0xA) += *(u16 *)(d2 + 0xA);
                            if (i != 0)
                            {
                                *(s16 *)((u8 *)m + 0x2) =
                                    (s16)(*(u16 *)((u8 *)m + 0x2) - 0x8) + (rand() & 0xF);
                                *(s16 *)((u8 *)m + 0x6) =
                                    (s16)(*(u16 *)((u8 *)m + 0x6) - 0x8) + (rand() & 0xF);
                                *(s16 *)((u8 *)m + 0xA) =
                                    (s16)(*(u16 *)((u8 *)m + 0xA) - 0x10) + (rand() & 0x1F);
                            }
                            s = (UnkSlot *)n->unk_0C;
                            if (i != 0)
                            {
                                s->unk_10 = 0x20;
                                s->unk_1E = 0x1000;
                                s->unk_1C = 0x1000;
                                *((u8 *)s + 0xE) = 0x80;
                                *((u8 *)s + 0xD) = 0x80;
                                *((u8 *)s + 0xC) = 0x80;
                                func_8003DB94(s, D_800DED28, 0);
                            }
                            else
                            {
                                s->unk_1E = 0x2000;
                                s->unk_1C = 0x2000;
                                s->unk_10 = 0x20;
                                *((u8 *)s + 0xE) = 0x80;
                                *((u8 *)s + 0xD) = 0x80;
                                *((u8 *)s + 0xC) = 0x80;
                                func_8003DB94(s, D_800DEB28, 0);
                            }
                        }
                    }
                }
                *(s16 *)(p + 0x12) = *(u16 *)(p + 0xC);
                *(s16 *)(p + 0x14) = *(u16 *)(p + 0xE);
                *(s16 *)(p + 0x16) = *(u16 *)(p + 0x10);
            }
        }
        if (*(s16 *)(p + 0x20) == 0xB)
        {
            n = func_8003FC64(0x212);
            if (n != NULL)
            {
                *(s16 *)((u8 *)n + 0x4A) = 6;
                n->unk_10 = (u32)func_s51_800248B8;
                func_8004491C(n, (u32)D_80045340);
                s = (UnkSlot *)n->unk_0C;
                s->unk_10 = 0x20;
                s->unk_06 = 8;
                s->unk_14 |= 0xC;
                q = *(u8 **)(D_800814A8 - 0x18);
                m = n->unk_08;
                *(s32 *)((u8 *)m + 0x0) = *(s32 *)(q + 0x0);
                *(s32 *)((u8 *)m + 0x4) = *(s32 *)(q + 0x4);
                *(s32 *)((u8 *)m + 0x8) = *(s32 *)(q + 0x8);
                h2 = D_80082E80;
                if (func_8003DF74(*(u8 **)(h2 + 0x8), (UnkSlot *)h2, (s16 *)(p + 0xC), 0) != 0)
                {
                    *(s32 *)((u8 *)m + 0x0) += *(s16 *)(p + 0xC) << 16;
                    *(s32 *)((u8 *)m + 0x4) += *(s16 *)(p + 0xE) << 16;
                    *(s32 *)((u8 *)m + 0x8) += *(s16 *)(p + 0x10) << 16;
                }
                s = (UnkSlot *)n->unk_0C;
                s->unk_1E = 0x80;
                s->unk_1C = 0x80;
                *((u8 *)s + 0xE) = 0x80;
                *((u8 *)s + 0xD) = 0x80;
                *((u8 *)s + 0xC) = 0x80;
                *(UnkDngS58Name12 *)((u8 *)n + 0x92) = *(UnkDngS58Name12 *)D_800256EC;
                s->unk_08 = (u32)((u8 *)n + 0x92);
            }
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x20) >= 0xD)
        {
            for (i = 0; i < 4; i++)
            {
                n = func_8003FC64(0x212);
                if (n != NULL)
                {
                    q = (u8 *)n + 0x20;
                    *(s16 *)(q + 0x2A) = 0xC;
                    n->unk_10 = (u32)func_s51_8002492C;
                    func_8004491C(n, (u32)D_80045340);
                    s = (UnkSlot *)n->unk_0C;
                    s->unk_10 = 0x60;
                    s->unk_14 |= 0xC;
                    if ((rand() & 1) == 0)
                    {
                        s->unk_14 |= 1;
                    }
                    m = n->unk_08;
                    h3 = D_80082E80;
                    if (func_8003DF74(*(u8 **)(h3 + 0x8), (UnkSlot *)h3, (s16 *)(p + 0xC), 0) != 0)
                    {
                        d3 = D_80083780;
                        *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) + *(u16 *)(d3 + 0x2);
                        *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + *(u16 *)(d3 + 0x6);
                        *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + *(u16 *)(d3 + 0xA);
                        if (i != 0)
                        {
                            *(s16 *)(q + 0x2A) = 8;
                            s->unk_10 = 0x20;
                            *(s16 *)(p + 0xC) = (s16)(*(u16 *)(p + 0xC) - 0x20) + (rand() & 0x3F);
                            *(s16 *)(p + 0xE) = (s16)(*(u16 *)(p + 0xE) - 0x20) + (rand() & 0x3F);
                            *(s16 *)(p + 0x10) =
                                (s16)(*(u16 *)(p + 0x10) - 0x20) + (rand() & 0x3F);
                        }
                    }
                    *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0xC);
                    *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0xE);
                    *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x10);
                    s = (UnkSlot *)n->unk_0C;
                    s->unk_1E = 0x1000;
                    s->unk_1C = 0x1000;
                    *((u8 *)s + 0xE) = 0x80;
                    *((u8 *)s + 0xD) = 0x80;
                    *((u8 *)s + 0xC) = 0x80;
                    if (i != 0)
                    {
                        func_8003DB94(s, D_800DEB28, 0);
                    }
                    else
                    {
                        func_8003DB94(s, D_800DE870, 0);
                    }
                }
            }
        }
        g1 = *(u8 **)(D_800814A8 + 0x60);
        if (g1 != NULL)
        {
            *(s32 *)(g1 + 0x1C) |= 0x10000000;
            e1 = *(u8 **)(g1 - 0x14);
            e1[0xC] = *(s16 *)(p + 0x20) * 127 / 16 - 0x80;
            k = (0x10 - *(s16 *)(p + 0x20)) * 8;
            e1[0xE] = k;
            e1[0xD] = k;
        }
        if ((D_80082E94 & 0x8000) != 0 || --*(s16 *)(p + 0x20) < 0)
        {
            *(s16 *)(p + 0xA) = 4;
        }
        break;
    case 4:
        if (D_8002571C == 0)
        {
            g2 = *(u8 **)(D_800814A8 + 0x60);
            if (g2 != NULL)
            {
                *(s32 *)(g2 + 0x1C) &= 0xEFFFFFFF;
                *(s32 *)(*(u8 **)(g2 - 0x14) + 0xC) = 0x808080;
            }
            e2 = (u8 *)&D_80083460;
            *(s32 *)(e2 + 0xC) = 0;
            D_80082E86 = 0;
            *(s16 *)(e2 + 0xA) = *(u16 *)(e2 + 0xA) - 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            D_8002571C = 0;
        }
        break;
    }
}
