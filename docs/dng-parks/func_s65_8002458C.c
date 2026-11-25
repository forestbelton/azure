/* func_s65_8002458C -- PARK, WORD-EXACT: 873 built words against 873 target
   words, 50 aligned differences (tools/decomp/aligndiff.py), re-measured by the
   second agent in batch 34 -- see the SECOND AGENT section below for the five
   changes that took it there from the author's 875 / 85 and for the two
   allocation groups that remain.  The ORIGINAL HEADER FOLLOWS UNCHANGED; its
   85-row breakdown and its eleven-spelling table are still the record of what
   was measured first, and its row 1 diagnosis is overturned, not its rows.

   func_s65_8002458C -- PARK, 875 built words against 873 target words,
   85 aligned differences (tools/decomp/aligndiff.py).  1 member in its family
   (python3 tools/decomp/dngfam.py func_s65_8002458C), so it lands once.
   No carved sibling at ratio 0.30 or better anywhere -- dngtwin finds nothing
   at any floor -- and no resident twin either.  Every shape below came off the
   listing, off this unit's own func_s65_80024104 and func_s65_800243E4 (both
   landed, both 0 differing, and together they are the six dress blocks), off
   csrc/ovl_dng_s58.c's func_s58_8002407C, and off csrc/ovl_dng_s19.c's
   func_s19_80024050, which is the same genre of switch-driven room driver and
   is where func_8003DF74's signature comes from.

   OWNS jtbl_s65_80024048: 5 words = 5 cases + 0 pad, bound 5, 5 distinct
   targets .Ls65_800245F0 / .Ls65_80024674 / .Ls65_8002478C / .Ls65_80024D98 /
   .Ls65_8002512C for cases 0..4, with case 0 FALLING THROUGH into case 1.
   The range-check beqz lands on .Ls65_800251D8, the function's common TAIL,
   which is not a table entry -- so there is no default among the entries and
   this body carries no default label.  Checked by grep before any arm was
   written.

   WHAT IT DOES.  The 0x212 sparkle driver for this room script: tick the
   payload's +0x2A counter, then switch on the state at +0xA.  State 0
   publishes D_80025330 into the scene at +0xF4, sets the D_80082E86 mode to 6,
   whitens the D_8002536C tint record, rings it through func_800B835C over
   {0x1000340, 0x400040}, raises the D_80025384 tick and advances.  State 1
   waits on the 0x80 bit of **(u16 **)(p + 0x4), arms +0x20 to 0x14, spends one
   of the scene's +0xA6 charges, copies p[0x8] to the scene's +0xA8, zeroes
   +0x2A, records the scene's +0x2A at +0x26, calls func_s65_800243E4 and runs
   func_800B8FC8 over the two actors at +0x68 and +0x64.  States 2 and 3 are
   the burn: they tint the player prim through its -0x14 pointer, count +0x20
   down, and spawn 0x212 nodes stepped by func_s65_800244F0.  State 4 tears
   down: clears the 0x10000000 bit, greys both prims, zeroes D_80083460's +0xC
   word and the D_80082E86 mode, and raises the two 0x8000 kill bits.  The
   common tail re-tints D_8002536C twice more, at +0x2A >= 0xC and >= 0xF.

   WHAT THE RESIDUE IS, EXACTLY.  The body is word-exact apart from TWO NOPS,
   one in each colour block (target words 137 and 524).  Both are the
   load-delay slot of the volatile +0x98 read, which the TARGET fills with the
   next instruction -- the lhu 0x20($s2) feeding the colour ternary -- and
   which the BUILT version cannot fill because that load has been hoisted out
   of the arm.  Every remaining row follows from that one transform:

     * 40 rows, the two colour blocks.  gcc hoists the lhu $v0, 0x20($s2) that
       feeds the ternary condition OUT of the >= 5 / < 0xD arm into the
       dominator block that already holds that arm's lh $v1, 0x20($s2), four
       instructions and two branches earlier.  There it holds $a1 for the whole
       block, so the D_800814A8 value goes to $a0 where the target has $v1, the
       -0x14 prim pointer to $v0 where the target has $a2, and the colour
       constant to $v1 where the target has $a0.  CONFIRMED to belong to the
       condition: with a flat constant in both arms the extra load is gone and
       target word 129 is the nop it should be (847 built words, diagnostic).
     * 17 rows, the two *(u8 **)(D_800814A8 + 0x60) blocks in states 3 and 4 --
       caller-saved names only, in those same two arms, the same wake.
     * 12 rows, state 1's three argument copies.  The target fills the
       (flags & 0x80) beqz delay slot and two load-delay nops with
       addu $a0, $s2 / addu $a1, $s0 / addu $a2, $s1; the built version emits
       all three just before the call and leaves the nops, because its
       %hi(D_800814A8) pseudo takes $a1 and conflicts with the second copy --
       the target puts that %hi in $a3.
     * 3 rows, state 3's scale, register names only.
     * 2 rows, the two beqz .Ls65_800251D8 offsets, which ARE the two-word
       length difference and go with it.
     * 2 rows, the state 4 tail's -0x14 load register.

   NAMED NEXT STEP.  One transform, eleven measured spellings, none of which
   keeps the condition load in its arm.  It is NOT the volatile read: removing
   both volatile reads leaves the hoist in place.  It is NOT a missing block
   boundary: neither if (1) nor do { } while (0) moves it.  Next is
   tools/cdkgcc -da on a reduced probe of one colour block, reading .lreg and
   .greg to find which pass creates the second load -- or handing the permuter
   the colour statement alone.  Two words and 85 rows with one named cause is
   the shape a permuter run closes.

   MEASURED AND KEPT.  Every figure is built words / aligned differences on
   this function; each row is the delta from the state named beside it.

   1. THE BIGGEST FACT.  The two dress blocks whose listing reads
      sh $v0, 0x4A($s0) must still be written q = (u8 *)n + 0x20;
      *(s16 *)(q + 0x2a) = ...;  The 0x4A is combine FOLDING a q whose only use
      in that block is this one store, and the addiu goes with it.  Written as
      the folded *(s16 *)((u8 *)n + 0x4a), q loses six references,
      allocno_compare then ranks p ahead of q, and p takes $s1 with s in $s2
      where the target has s in $s1 and p in $s2 -- a rotation over all 873
      words.  A FOLDED OFFSET IN A LISTING IS NOT A SPELLING IN THE SOURCE.
          q spelling, all three blocks   884 / 148
          folded 0x4a                    890 / 243
   2. D_80083780 must be read as a SCALAR-declared struct, not off the
      tree-wide extern u8 D_80083780[].  As an incomplete array its address is
      its own quantity, gcc folds the FIRST offset into the symbol, and the
      base register holds D_80083780+0x2 with the three reads at 0/4/8 where
      the target holds D_80083780+0 and reads 0x2/0x6/0xA.  Five rows in each
      of six dress blocks.  A u8 * pointer local is NOT the fix and is far
      worse: it splits the lui and the addiu across two registers.
          UnkDngS65Pt scalar alias, field reads   876 / 111
          *(u16 *)(D_80083780 + 0x2)              876 / 141
          u8 *d = D_80083780, born after m        882 / 225
          u8 *d = D_80083780, born before m       882 / 225
   3. The prim colour store is an EXPRESSION-STATEMENT ternary with the store
      in BOTH arms.  That is what gives one -0x14 load per outer arm, one
      shared sw after the join, and the else constant in the delay slot.
          cond ? (MEM = A) : (MEM = B)              875 / 96
          MEM = cond ? A : B                        876 / 141
          MEM = cond ? A : B without the != 0       876 / 141
          MEM = cond ? A : B, if (1) wrapper        876 / 141
          cond ? (MEM = A) : (MEM = B), do-while    875 / 91
          condition through an s32 local            877 / 115
          volatile on the condition read            877 / 87
          prim-pointer local + if/else              879 / 139
          prim-pointer local + value ternary        876 / 103
          prim-pointer local + expression ternary   879 / 109
          range tests through an s32 local          876 / 100
   4. State 3's scale must be computed ONCE into its own s32 local, and that
      local must NOT be the one state 0 uses to carry p+0xA across the tick
      store.  Writing the expression twice for unk_1E and unk_1C costs SEVEN
      WORDS -- cse does not share it between the two stores.
          separate k2                    875 / 86
          k shared with state 0          875 / 96
          expression written twice       883 / 145
   5. e must not be shared between state 3 and state 4 either.
          split e / e2                   875 / 85
          one shared e                   875 / 86
   6. h = &D_80083460; as a struct-pointer local.  Same mechanism as fact 2
      with the OPPOSITE answer, and the declaration is why: D_80083460 is
      declared extern Unk80083462 D_80083460 in include/structs.h, a STRUCT,
      so the pointer local keeps the base at the symbol.  Without it the first
      reference in source order is +0xC, the base register holds
      D_80083460+0xC, and the +0xA read comes out at -0x2.
          h local             876 / 141
          direct &symbol      876 / 145
   7. D_80082E80 declared extern UnkSlot D_80082E80, with the call written
      func_8003DF74((u8 *)D_80082E80.unk_08, &D_80082E80, (s16 *)(p + 0xC), 0)
      so one materialised base serves both the +0x8 load and argument 2.
      Through extern u8 D_80082E80[] and *(u8 **)(D_80082E80 + 0x8) gcc
      materialises D_80082E80+8 for the load and then pays an
      addiu $a1, $a1, -8 to recover the base for argument 2 -- two rows at each
      of the two call sites.  Measured inside the 243 -> 148 step, not alone.
   8. The one-call conditional argument is
      func_800A56E0(func_80053EF0(4) != 2 ? 0x300 : 0x4300).  The == 2 form
      with the arms the other way round puts the wrong constant in the branch
      delay slot: two rows, measured inside the 243 -> 148 step.
   9. The four discarded reads of the scene's +0x98 are
      *(volatile u16 *)(D_800814A8 + 0x98);  -- docs/matching-levers.md:17324,
      the same field and the same idiom as func_800D1E34's.  Removing the two
      in state 2 measures 873 / 136 against 876 / 141, so they cost three words
      and buy five rows; two of those three words are the unfilled load-delay
      nops above, NOT the loads, and the target plainly has all four.
      DEVICE, labelled as one: the macro that wrapped the prim recolour is not
      recoverable, and volatile is how its discarded read survives flow.
  10. func_s65_800243E4 is called with THREE arguments here while its landed
      definition in csrc/ovl_dng_s65.c takes ONE, so this park carries a
      three-argument alias and leaves that definition alone.  MEASURED BOTH
      WAYS: widening the landed definition to (u8 *a, UnkVec *v, UnkSlot *s)
      also keeps it at 67 words / 0 differing, so either fix works.  The alias
      was chosen only because that function is a family representative the
      dispatcher replicates.
  11. func_8003DF74 is undeclared tree-wide; the signature here is
      csrc/ovl_dng_s19.c's (u8 *, UnkSlot *, s16 *, s16).

   MEASURED AND REJECTED, beyond the tables above: a volatile cast forcing
   state 1's second D_800814A8 read to reload -- 876 / 141, no change, because
   the reload is already there and only its position differs.

   NOT MEASURED: whether an explicit default: break; is free here, and the
   SVECTOR-versus-other declared type of the two func_800B8FC8 argument
   records at sp+0x20 and sp+0x28, whose four and two halfword stores are all
   the listing witnesses.

   ================  SECOND AGENT, batch 34 (h150).  WORD-EXACT NOW.  ================

   NEW FIGURE: 873 built words against 873 target, 50 aligned differences
   (ovlverify: 51 differing masked).  The park arrived at 875 / 85.  The two
   extra words are closed and 35 rows are gone.  The body below is the new one;
   the author's body is superseded, not the author's diagnosis of WHERE the
   residue is -- that was right about the blocks and wrong about the mechanism.

   THE AUTHOR'S NAMED NEXT STEP WAS RUN AND IT ANSWERED THE QUESTION.
   tools/decomp/dump.sh on a reduced probe of one colour block, reading .cse
   and .combine, overturns the park's "gcc hoists the lhu out of the arm":

     * NOTHING IS HOISTED.  cse DELETES the arm's condition load.  In .cse the
       and-with-1 reads reg:HI 81 -- the pseudo the ENTRY block's s16 gate load
       set -- so after cse there is exactly ONE load of p+0x20 in the whole
       region and it lives in the entry block.  cse_insn's lookup finds the
       identical mem:HI already in the table because the then-arm is the
       fall-through of the inner gate and therefore the SAME cse basic block
       (cse.c:8086, the block ends only at a CODE_LABEL).
     * COMBINE THEN RE-CREATES A LOAD, for the comparisons, not for the
       condition: .combine insn 12 is set reg:SI 82 (sign_extend (mem:HI ...)),
       a second reference to the same mem substituted into the sign extension.
       So the built pair is lhu-in-entry (the cse survivor, feeding the andi)
       plus lh-in-entry (combine's), and the arm has neither.
     * THE TARGET'S ARM LOAD IS A zero_extendhisi2, not a movhi.  That is what
       a u16 read promoted to int expands to when no equivalent value is
       available, and it is the shape the fix has to produce.

   THE FIX, three independent changes, each measured from the park's own body.

   12. volatile ON THE CONDITION READ is what denies cse the lookup.  cse.c:6506
       does the table lookup only when ! src_volatile, and cse.c:7226 keeps a
       volatile source out of the table, so the arm's read is forced to be its
       own zero_extendhisi2.  This is the whole of the author's row 1 of the
       85-row breakdown and it is the ONE change the eleven measured spellings
       in the park's table never made to the CONDITION as opposed to the store.
       Probe check before porting: with it, the arm's order becomes
       lhu 0x98 / lhu 0x20 / lw -0x14 / andi / beqz, which is the target's
       words 138..142 instruction for instruction.
           volatile condition read, park's store-in-both-arms   879 / 86
           park's body unchanged                                875 / 85
       On its own it is NOT an improvement.  It only pays with 13 below, which
       is why a one-change-at-a-time sweep could not find it.
   13. THE COLOUR STORE IS A VALUE TERNARY, not an expression-statement one,
       and the park's row 3 has it backwards.  The park's cond ? (MEM=A)
       : (MEM=B) evaluates the -0x14 prim-pointer load once PER ARM; the target
       has it once per OUTER arm, before the inner beqz, shared by both ternary
       arms -- target word 140 against the built's two.
           volatile cond + MEM = cond ? A : B                   877 / 76
           volatile cond + cond ? (MEM=A) : (MEM=B)             879 / 86
   14. STATE 1 IS A STATEMENT-ORDER FAULT, NOT AN ALLOCATION ONE, and this is
       the row that closes the word count.  The author's row "12 rows, state 1's
       three argument copies ... its %hi(D_800814A8) pseudo takes $a1" reads the
       symptom correctly and the cause backwards: local-alloc runs AFTER sched1
       (toplev.c:3548 against :3585), so the register cannot have caused the
       schedule.  sched1 failed to put the three argument copies in the beqz
       delay slot and the two load-delay slots, which is the +2 words, and that
       left $a1 free for the %hi.  The three statements before the call are
           A   p+0xA   increment
           B   p+0x2A  zero
           C   p+0x26 = D_800814A8's +0x2A
       and the park has them A B C.  All six orders measured, on the body that
       already had 12 and 13:
           C A B    875 / 63     <-- state 1 goes to ZERO rows
           B C A    875 / 65
           A C B    877 / 75
           C B A    876 / 75
           A B C    877 / 76     (the park's order)
           B A C    877 / 76
       C A B is the order the target's block reads bottom-up and it fills all
       three slots: addu $a0,$s2 in the beqz delay, addu $a1,$s0 after
       lhu 0xA6, addu $a2,$s1 after lbu 0x8.
   15. THE PRIM POINTER AND THE COLOUR VALUE ARE TWO LOCALS AND THE STORE IS
       ONE STATEMENT AFTER THE INNER if/else.  This is the last two words.  The
       target's single sw 0xC($a2) is reached from both outer arms with the
       same two registers, which two per-arm stores can only reach through
       jump2 cross-jumping, and cross-jumping runs after greg and so needs the
       registers to agree -- they do not.  One pseudo each makes the store one
       insn and the question disappears.
           c/cv locals, store after the if/else, C A B order    873 / 50
           same with per-block locals c2/cv2                    873 / 50
           store inside each arm, shared c/cv, C A B order      875 / 63
       The two spellings at 50 are indistinguishable, so this park keeps the
       shared pair; the split pair is equally correct and the target does not
       choose between them.

   MEASURED AND REJECTED FROM THE 873 / 50 BODY -- do not re-derive these.
       dropping the != 0 from the ternary condition          873 / 50  (free)
       (cond == 0) ? 0x80E080 : 0x709070, arms swapped       875 / 76
       the volatile 0x98 read BEFORE c = -0x14 in all four arms  875 / 61
       c and cv reused for the D_800814A8+0x60 blocks too    873 / 56
       c and cv reused for state 4's two stores only         873 / 56
       hoisting D_800814A8's +0x2A read into k before A      877 / 76  (free)
   The +0x60 result is worth keeping: the obvious unification of "every colour
   write goes through cv" is six rows WORSE, so the target does not share one
   value across the colour blocks and the +0x60 blocks.

   WHAT THE 50 ROWS ARE NOW, priced from the 873-word body.

     * 22 rows, 11 in each colour block: THE TARGET SHARES ONE
       lui %hi(D_800814A8) BETWEEN THE TWO ARMS and the built rematerialises it
       per arm.  Target word 134 is that lui, sitting in the inner gate's bnez
       DELAY SLOT, and the flat arm at word 147 reads lw %lo(D)($v0) off it.
       The built emits a high in each arm (its words 135 and 147) and in
       exchange loses the target's nop at 137 and its ori at 151, so the word
       count is unaffected -- this group cannot be found by counting words.
       For cse to share the high, the set reg (high (symbol_ref)) insn has to be
       in the DOMINATOR block: cse follows the bnez into the flat arm
       (cse.c:8150, LABEL_NUSES == 1 and a BARRIER before the label, which both
       hold here) and would then find it available.  No spelling that puts it
       there was found.  A u32 *dp = &D_800814A8 local is NOT it: that is
       lui/addiu plus lw 0($dp), a different encoding from the target's
       lw %lo(D)(reg), and costs a word.
     * 22 rows across the three D_800814A8+0x60 and state 4 blocks: caller-saved
       names only, no word delta, no instruction moved.  The target holds the
       actor pointer in $a1 where the built has $a0 or $a2, and the colour in
       $a0 where the built has $v1.  $a0 is free in the built, so the target's
       $a1 means $a0 conflicts with that range there and does not here.
     * 5 rows, state 3's scale: the target's lh $a0, 0x20($s2) and
       addiu $v1, $zero, 0x10 sit two words earlier than the built's, and the
       subu then reads $a0 against the built's $v0.  Register plus a two-word
       schedule shift, no word delta.
     * 1 row, rounding on the groups above.

   NEW NAMED NEXT STEP.  This is now a WORD-EXACT body with one named cause and
   two allocation groups, which is the shape the dispatcher's permuter closes.
   The statement to hand it is the colour block -- the eleven words from
   c = *(u8 **)(D_800814A8 - 0x14) through *(u32 *)(c + 0xC) = cv, in both
   state 2 and state 3, which are byte-identical to each other.  The specific
   question for a search is whether any spelling of those four statements puts
   the (high (symbol_ref D_800814A8)) insn in the block that holds the inner
   gate while keeping TWO lw %lo of the global, one per arm.  If it does, 22 of
   the 50 rows go with it and the +0x60 group is the author's "same wake", so it
   may follow.  Do NOT re-sweep the condition read, the ternary form or the
   state 1 order: all three are settled above with their losing scores.

DECLARATIONS
typedef struct UnkDngS65Pt
{
    u8 pad_00[0x2];
    u16 unk_02;
    u8 pad_04[0x2];
    u16 unk_06;
    u8 pad_08[0x2];
    u16 unk_0A;
} UnkDngS65Pt;
extern UnkDngS65Pt D_80083780p __asm__("D_80083780");
extern u8 D_80025330[];
extern u8 D_80025348[];
extern u8 D_80025360[];
extern u8 D_8002536C[];
extern UnkSlot D_80082E80;
extern s16 D_80082E86;
extern u16 D_80082E94;
void func_800B835C(u8 *, s32 *, s32, s32);
s32 func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
s32 func_80053EF0(s32);
s32 func_800A56E0(s32);
void func_s65_800243E4_3(u8 *, UnkVec *, UnkSlot *) __asm__("func_s65_800243E4");
END DECLARATIONS
*/
void func_s65_8002458C(u8 *p, UnkVec *v, UnkSlot *s)
{
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    u8 *q;
    u8 *e;
    u8 *e2;
    u8 *o;
    UnkVec *m;
    UnkSlot *r;
    Unk80083462 *h;
    u8 *c;
    s32 cv;
    s32 k;
    s32 k2;
    s32 arr[2];
    SVECTOR a;
    SVECTOR w;

    *(s16 *)(p + 0x2A) = *(u16 *)(p + 0x2A) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)(D_800814A8 + 0xF4) = (u32)D_80025330;
        D_80082E86 = 6;
        D_8002536C[0x10] = 0;
        D_8002536C[0x11] = 0;
        D_8002536C[0x12] = 0;
        D_8002536C[0x14] = 0xFF;
        D_8002536C[0x15] = 0xFF;
        D_8002536C[0x16] = 0xFF;
        arr[0] = 0x1000340;
        arr[1] = 0x400040;
        func_800B835C(D_8002536C, arr, 1, 0);
        k = *(u16 *)(p + 0xA);
        D_80025384 = 1;
        *(s16 *)(p + 0xA) = k + 1;
        /* fall through */
    case 1:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        o = (u8 *)D_800814A8;
        *(s16 *)(p + 0x20) = 0x14;
        *(s16 *)(o + 0xA6) = *(u16 *)(o + 0xA6) - 1;
        o[0xA8] = p[0x8];
        *(s16 *)(p + 0x26) = *(u16 *)(D_800814A8 + 0x2A);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0x2A) = 0;
        func_s65_800243E4_3(p, v, s);
        if (*(u8 **)(p + 0x68) != NULL)
        {
            a.vx = 0x340;
            a.vy = 0x100;
            a.vz = 0x40;
            a.pad = 0x40;
            w.vx = 0x360;
            w.vy = 0x120;
            func_800B8FC8(*(u8 **)(p + 0x68), (s32 *)&a, (s32 *)&w, 0, 1);
        }
        if (*(u8 **)(p + 0x64) != NULL)
        {
            a.vx = 0x340;
            a.vy = 0x100;
            a.vz = 0x40;
            a.pad = 0x40;
            w.vx = 0x360;
            w.vy = 0x120;
            func_800B8FC8(*(u8 **)(p + 0x64), (s32 *)&a, (s32 *)&w, 1, 1);
        }
        break;
    case 2:
        if (*(s16 *)(p + 0x20) < 9)
        {
            if (*(s16 *)(p + 0x20) >= 5)
            {
                c = *(u8 **)(D_800814A8 - 0x14);
                *(volatile u16 *)(D_800814A8 + 0x98);
                cv = ((*(volatile u16 *)(p + 0x20) & 1) != 0) ? 0x709070 : 0x80E080;
            }
            else
            {
                c = *(u8 **)(D_800814A8 - 0x14);
                *(volatile u16 *)(D_800814A8 + 0x98);
                cv = 0x80E080;
            }
            *(u32 *)(c + 0xC) = cv;
        }
        *(s16 *)(p + 0x20) = *(u16 *)(p + 0x20) - 1;
        if (*(s16 *)(p + 0x20) <= 0)
        {
            *(s16 *)(p + 0x20) = 0x10;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        if (*(s16 *)(p + 0x20) == 0x11)
        {
            func_800A56E0(func_80053EF0(4) != 2 ? 0x300 : 0x4300);
        }
        if (*(s16 *)(p + 0x20) < 9)
        {
            if (func_8003DF74((u8 *)D_80082E80.unk_08, &D_80082E80, (s16 *)(p + 0xC), 0) != 0)
            {
                if (*(s16 *)(p + 0x20) >= 7)
                {
                    n = func_8003FC64(0x212);
                    if (n != NULL)
                    {
                        q = (u8 *)n + 0x20;
                        *(s16 *)(q + 0x2a) = 1;
                        n->unk_10 = (u32)func_s65_800244F0;
                        func_8004491C(n, (u32)D_80045340);
                        r = (UnkSlot *)n->unk_0C;
                        r->unk_10 = 0x20;
                        r->unk_14 |= 0xC;
                        m = n->unk_08;
                        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0xC);
                        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0xE);
                        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x10);
                        *(s16 *)((u8 *)m + 0x2) += D_80083780p.unk_02;
                        *(s16 *)((u8 *)m + 0x6) += D_80083780p.unk_06;
                        *(s16 *)((u8 *)m + 0xA) += D_80083780p.unk_0A;
                        r->unk_1E = 0xC00;
                        r->unk_1C = 0xC00;
                        *((u8 *)r + 0xE) = 0xFF;
                        *((u8 *)r + 0xD) = 0xFF;
                        *((u8 *)r + 0xC) = 0xFF;
                        *(UnkDngS58Name12 *)((u8 *)n + 0x92) = *(UnkDngS58Name12 *)D_80025348;
                        r->unk_08 = (u32)((u8 *)n + 0x92);
                        q[0x7a] = 0x40;
                        q[0x7b] = 0x40;
                    }
                    if ((*(u16 *)(p + 0x20) & 1) == 0)
                    {
                        n = func_8003FC64(0x212);
                        if (n != NULL)
                        {
                            q = (u8 *)n + 0x20;
                            *(s16 *)(q + 0x2a) = 1;
                            n->unk_10 = (u32)func_s65_800244F0;
                            func_8004491C(n, (u32)D_80045340);
                            r = (UnkSlot *)n->unk_0C;
                            r->unk_10 = 0x20;
                            r->unk_06 = -4;
                            r->unk_14 |= 0xC;
                            m = n->unk_08;
                            *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0xC);
                            *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0xE);
                            *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x10);
                            *(s16 *)((u8 *)m + 0x2) += D_80083780p.unk_02;
                            *(s16 *)((u8 *)m + 0x6) += D_80083780p.unk_06;
                            *(s16 *)((u8 *)m + 0xA) += D_80083780p.unk_0A;
                            r->unk_1E = 0xC00;
                            r->unk_1C = 0xC00;
                            *((u8 *)r + 0xE) = 0x80;
                            *((u8 *)r + 0xD) = 0x80;
                            *((u8 *)r + 0xC) = 0x80;
                            *(UnkDngS58Name12 *)((u8 *)n + 0x92) = *(UnkDngS58Name12 *)D_80025360;
                            r->unk_08 = (u32)((u8 *)n + 0x92);
                        }
                    }
                }
                else
                {
                    n = func_8003FC64(0x212);
                    if (n != NULL)
                    {
                        q = (u8 *)n + 0x20;
                        *(s16 *)(q + 0x2a) = 0xC;
                        n->unk_10 = (u32)func_s65_800244F0;
                        func_8004491C(n, (u32)D_80045340);
                        r = (UnkSlot *)n->unk_0C;
                        r->unk_10 = 0x20;
                        r->unk_14 |= 0xC;
                        m = n->unk_08;
                        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0xC);
                        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0xE);
                        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x10);
                        *(s16 *)((u8 *)m + 0x2) += D_80083780p.unk_02;
                        *(s16 *)((u8 *)m + 0x6) += D_80083780p.unk_06;
                        *(s16 *)((u8 *)m + 0xA) += D_80083780p.unk_0A;
                        r->unk_1E = 0xC00;
                        r->unk_1C = 0xC00;
                        *((u8 *)r + 0xE) = 0xFF;
                        *((u8 *)r + 0xD) = 0xFF;
                        *((u8 *)r + 0xC) = 0xFF;
                        *(UnkDngS58Name12 *)((u8 *)n + 0x92) = *(UnkDngS58Name12 *)D_80025348;
                        r->unk_08 = (u32)((u8 *)n + 0x92);
                        q[0x7a] = 0;
                        q[0x7b] = 0;
                    }
                    n = func_8003FC64(0x212);
                    if (n != NULL)
                    {
                        q = (u8 *)n + 0x20;
                        *(s16 *)(q + 0x2a) = 1;
                        n->unk_10 = (u32)func_s65_800244F0;
                        func_8004491C(n, (u32)D_80045340);
                        r = (UnkSlot *)n->unk_0C;
                        r->unk_10 = 0x20;
                        r->unk_06 = -4;
                        r->unk_14 |= 0xC;
                        m = n->unk_08;
                        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0xC);
                        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0xE);
                        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x10);
                        *(s16 *)((u8 *)m + 0x2) += D_80083780p.unk_02;
                        *(s16 *)((u8 *)m + 0x6) += D_80083780p.unk_06;
                        *(s16 *)((u8 *)m + 0xA) += D_80083780p.unk_0A;
                        r->unk_1E = 0xC00;
                        r->unk_1C = 0xC00;
                        *((u8 *)r + 0xE) = 0x80;
                        *((u8 *)r + 0xD) = 0x80;
                        *((u8 *)r + 0xC) = 0x80;
                        *(UnkDngS58Name12 *)((u8 *)n + 0x92) = *(UnkDngS58Name12 *)D_80025360;
                        r->unk_08 = (u32)((u8 *)n + 0x92);
                    }
                }
                if (*(s16 *)(p + 0x20) < 4)
                {
                    *(s16 *)(p + 0x18) = (*(s16 *)(p + 0xC) + *(s16 *)(p + 0x12)) / 2;
                    *(s16 *)(p + 0x1A) = (*(s16 *)(p + 0xE) + *(s16 *)(p + 0x14)) / 2;
                    *(s16 *)(p + 0x1C) = (*(s16 *)(p + 0x10) + *(s16 *)(p + 0x16)) / 2;
                    n = func_8003FC64(0x212);
                    if (n != NULL)
                    {
                        q = (u8 *)n + 0x20;
                        *(s16 *)(q + 0x2a) = 0xC;
                        n->unk_10 = (u32)func_s65_800244F0;
                        func_8004491C(n, (u32)D_80045340);
                        r = (UnkSlot *)n->unk_0C;
                        r->unk_10 = 0x20;
                        r->unk_14 |= 0xC;
                        m = n->unk_08;
                        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0x18);
                        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0x1A);
                        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x1C);
                        *(s16 *)((u8 *)m + 0x2) += D_80083780p.unk_02;
                        *(s16 *)((u8 *)m + 0x6) += D_80083780p.unk_06;
                        *(s16 *)((u8 *)m + 0xA) += D_80083780p.unk_0A;
                        r->unk_1E = 0xC00;
                        r->unk_1C = 0xC00;
                        *((u8 *)r + 0xE) = 0xFF;
                        *((u8 *)r + 0xD) = 0xFF;
                        *((u8 *)r + 0xC) = 0xFF;
                        *(UnkDngS58Name12 *)((u8 *)n + 0x92) = *(UnkDngS58Name12 *)D_80025348;
                        r->unk_08 = (u32)((u8 *)n + 0x92);
                        q[0x7a] = 0;
                        q[0x7b] = 0;
                    }
                }
                *(s16 *)(p + 0x12) = *(u16 *)(p + 0xC);
                *(s16 *)(p + 0x14) = *(u16 *)(p + 0xE);
                *(s16 *)(p + 0x16) = *(u16 *)(p + 0x10);
            }
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x20) >= 7)
        {
            if (*(s16 *)(p + 0x20) < 0xD)
            {
                c = *(u8 **)(D_800814A8 - 0x14);
                *(volatile u16 *)(D_800814A8 + 0x98);
                cv = ((*(volatile u16 *)(p + 0x20) & 1) != 0) ? 0x709070 : 0x80E080;
            }
            else
            {
                c = *(u8 **)(D_800814A8 - 0x14);
                *(volatile u16 *)(D_800814A8 + 0x98);
                cv = 0x80E080;
            }
            *(u32 *)(c + 0xC) = cv;
        }
        if (*(s16 *)(p + 0x20) >= 0xD)
        {
            if (func_8003DF74((u8 *)D_80082E80.unk_08, &D_80082E80, (s16 *)(p + 0xC), 0) != 0)
            {
                n = func_8003FC64(0x212);
                if (n != NULL)
                {
                    q = (u8 *)n + 0x20;
                    *(s16 *)(q + 0x2a) = 0xC;
                    if (*(s16 *)(p + 0x20) != 0xD)
                    {
                        *(s16 *)(q + 0x2a) = 1;
                    }
                    n->unk_10 = (u32)func_s65_800244F0;
                    func_8004491C(n, (u32)D_80045340);
                    r = (UnkSlot *)n->unk_0C;
                    r->unk_10 = 0x20;
                    r->unk_14 |= 0xC;
                    m = n->unk_08;
                    *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0xC);
                    *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0xE);
                    *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x10);
                    *(s16 *)((u8 *)m + 0x2) += D_80083780p.unk_02;
                    *(s16 *)((u8 *)m + 0x6) += D_80083780p.unk_06;
                    *(s16 *)((u8 *)m + 0xA) += D_80083780p.unk_0A;
                    *((u8 *)r + 0xE) = 0xFF;
                    *((u8 *)r + 0xD) = 0xFF;
                    *((u8 *)r + 0xC) = 0xFF;
                    k2 = (0x10 - *(s16 *)(p + 0x20)) * 0x300 + 0xC00;
                    r->unk_1E = k2;
                    r->unk_1C = k2;
                    *(UnkDngS58Name12 *)((u8 *)n + 0x92) = *(UnkDngS58Name12 *)D_80025348;
                    r->unk_08 = (u32)((u8 *)n + 0x92);
                    q[0x7a] = 0x40;
                    q[0x7b] = 0x40;
                }
                if ((*(u16 *)(p + 0x20) & 1) == 0)
                {
                    n = func_8003FC64(0x212);
                    if (n != NULL)
                    {
                        q = (u8 *)n + 0x20;
                        *(s16 *)(q + 0x2a) = 1;
                        n->unk_10 = (u32)func_s65_800244F0;
                        func_8004491C(n, (u32)D_80045340);
                        r = (UnkSlot *)n->unk_0C;
                        r->unk_10 = 0x20;
                        r->unk_06 = -4;
                        r->unk_14 |= 0xC;
                        m = n->unk_08;
                        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0xC);
                        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0xE);
                        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x10);
                        *(s16 *)((u8 *)m + 0x2) += D_80083780p.unk_02;
                        *(s16 *)((u8 *)m + 0x6) += D_80083780p.unk_06;
                        *(s16 *)((u8 *)m + 0xA) += D_80083780p.unk_0A;
                        r->unk_1E = 0xC00;
                        r->unk_1C = 0xC00;
                        *((u8 *)r + 0xE) = 0x80;
                        *((u8 *)r + 0xD) = 0x80;
                        *((u8 *)r + 0xC) = 0x80;
                        *(UnkDngS58Name12 *)((u8 *)n + 0x92) = *(UnkDngS58Name12 *)D_80025360;
                        r->unk_08 = (u32)((u8 *)n + 0x92);
                    }
                }
            }
        }
        e2 = *(u8 **)(D_800814A8 + 0x60);
        if (e2 != NULL)
        {
            *(u32 *)(e2 + 0x1C) |= 0x10000000;
            *(u32 *)(*(u8 **)(e2 - 0x14) + 0xC) = ((g->unk_04 & 1) != 0) ? 0x80FF80 : 0;
        }
        if ((D_80082E94 & 0x8000) == 0)
        {
            *(s16 *)(p + 0x20) = *(u16 *)(p + 0x20) - 1;
            if (*(s16 *)(p + 0x20) < 0)
            {
                *(s16 *)(p + 0xA) = 4;
            }
        }
        else
        {
            *(s16 *)(p + 0xA) = 4;
        }
        break;
    case 4:
        if (D_80025384 == 0)
        {
            e = *(u8 **)(D_800814A8 + 0x60);
            if (e != NULL)
            {
                *(u32 *)(e + 0x1C) &= 0xEFFFFFFF;
                *(u32 *)(*(u8 **)(e - 0x14) + 0xC) = 0x808080;
            }
            h = &D_80083460;
            *(u32 *)((u8 *)h + 0xC) = 0;
            D_80082E86 = 0;
            *(s16 *)((u8 *)h + 0xA) = *(u16 *)((u8 *)h + 0xA) - 1;
            *(volatile u16 *)(D_800814A8 + 0x98);
            *(u32 *)(*(u8 **)(D_800814A8 - 0x14) + 0xC) = 0x808080;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            D_80025384 = 0;
        }
        break;
    }
    if (*(s16 *)(p + 0xA) >= 2)
    {
        if (*(s16 *)(p + 0x2A) >= 0xC)
        {
            if ((*(u16 *)(p + 0x20) & 1) == 0)
            {
                D_8002536C[0x10] = 0;
                D_8002536C[0x11] = 0;
                D_8002536C[0x12] = 0;
                D_8002536C[0x14] = 0xFF;
                D_8002536C[0x15] = 0xFF;
                D_8002536C[0x16] = 0xFF;
            }
            else
            {
                D_8002536C[0x10] = 0x60;
                D_8002536C[0x11] = 0x60;
                D_8002536C[0x12] = 0x60;
                D_8002536C[0x14] = 0x20;
                D_8002536C[0x15] = 0x20;
                D_8002536C[0x16] = 0x20;
            }
            arr[0] = 0x1400380;
            arr[1] = 0x400040;
            func_800B835C(D_8002536C, arr, 1, 0);
            if (*(s16 *)(p + 0x2A) >= 0xF)
            {
                if ((*(u16 *)(p + 0x20) & 1) == 0)
                {
                    D_8002536C[0x10] = 0;
                    D_8002536C[0x11] = 0;
                    D_8002536C[0x12] = 0;
                    D_8002536C[0x14] = 0x50;
                    D_8002536C[0x15] = 0x50;
                    D_8002536C[0x16] = 0x50;
                }
                else
                {
                    D_8002536C[0x10] = 0x50;
                    D_8002536C[0x11] = 0x50;
                    D_8002536C[0x12] = 0x50;
                    D_8002536C[0x14] = 0;
                    D_8002536C[0x15] = 0;
                    D_8002536C[0x16] = 0;
                }
                arr[0] = 0x1800380;
                arr[1] = 0x400040;
                func_800B835C(D_8002536C, arr, 1, 0);
            }
        }
    }
}
