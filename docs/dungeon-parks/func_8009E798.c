/* ======================================== 2026-11-21: 55 rows -> 1 (358/358)

   ONE ROW LEFT, and it is the second AddPrim's argument:
     cand  266  8009EBC0  addu $a1, $s0, $zero      (q, still live)
     tgt   266  8009EBC0  addiu $a1, $s2, -0x28     (q rematerialised from r)
   Everything else in the function is byte-exact.

   FOUR CHANGES, in the order they were measured, each on the body the
   previous one produced:

   1. THE ROLLING CURSOR -- 55 rows -> 29 (but 359 words, see 4).
      The target carries a SECOND pointer local beside `r': a cursor
      re-assigned `r + 0x28', `r + 0x40', `r + 0x4C', `r + 0x58'.  The target
      puts `r' in $s2 and the cursor in $s1, and -- this is the part that was
      missed for five days -- THE WHOLE POLY_F4 BLOCK IS ADDRESSED OFF THE
      CURSOR, including the four source quads, which the target reads at
      NEGATIVE offsets `-0x20/-0x18/-0x10/-0x8' off the F4 rather than at
      `+0x8/+0x10/+0x18/+0x20' off `r'.  The final `+0x58' cursor value is
      what the target stores to the +0x8D0 write-back (`sw $s1, 0x8D0($v1)'),
      which is also where its spare `addu $v0, $zero, $zero' delay-slot filler
      comes from.
      The old park's note `z = r + 0x28 as its own local -> 362 words' was a
      true measurement of a DIFFERENT change: one local for one primitive,
      with the F4 block still addressed off `r'.

   2. THE POLY_F4 BLOCK'S STATEMENT ORDER -- 29 -> 15 (and the whole 18-row
      F4 cluster to zero).  Colour store, then the FOUR COPIES, then the three
      `>>= 1' shifts.  A 70-way interleaving sweep of [store, >>4, >>5, >>6]
      against [copy8, copyC, copy10, copy14] (each sequence kept in its own
      order) found five orders at 15 and NOTHING between 15 and 19: every
      order with a shift ahead of the last copy is 19 or worse.  Copies-first
      (store after them) also measures 15; copies-after-the-shifts, which is
      what the park had, is 33.
      THIS IS THE LEGITIMATE VERSION OF THE MOVE THE PERMUTER KEPT PROPOSING.
      Two independent runs wanted `r[0x2C] >>= 1'/`r[0x2D] >>= 1' hoisted
      ABOVE the store that initialises the colour word, which is a read of the
      previous frame's packet buffer and was rejected both times.  What the
      scheduler actually wanted was the QUAD READS earlier, and moving the
      four copies ahead of the shifts gives it exactly that while leaving the
      colour word's store-before-read intact.
      Note the legality boundary: the store must precede the three shifts, and
      nothing else in the block is ordered -- the copies touch c+0x8..c+0x17
      and read c-0x20..c-0x1, all disjoint from the colour word c+0x4..c+0x7,
      so sched1 reorders loads, stores and byte-RMWs across each other freely
      (memrefs_conflict_p resolves same-base/different-constant addresses;
      true_dependence's MEM_IN_STRUCT_P escape is never reached here).

   3. `c = r + 0x28;' IMMEDIATELY AFTER THE func_8003DB6C MEMCPY -- 15 -> 11.
      The target emits `addiu $s1, $s2, 0x28' in the slot BEFORE
      `lbu $v0, 0x7($s0)'; with the assignment anywhere later sched1 emits it
      one slot after.  A position sweep of all 9 legal slots: the first five
      (memcpy, `q[0x7] |= 2', `r[0xD] = r[0x15] += 0x40', the `|= 0x100', the
      `r[0x1D] = r[0x25] += 0x40') all give 11; either AddPrim gives 15.

   4. `s16 flag' INSTEAD OF `s32 flag' -- 11 -> 1, and it is the whole
      p/flag cluster.
      `flag' and `p' are both BLOCK-LOCAL quantities ($s3/$s4), neither is in
      `.greg's `regs to allocate' list, so local-alloc decides this, not
      global-alloc.  The value is `base != D_801C9E40', which is 0 or 1, so
      s16 is an HONEST declaration rather than a device.  `u32 flag' and
      `if (flag)' for `if (flag != 0)' both measure 11, so neither the
      signedness nor the comparison spelling matters -- only the WIDTH.
      (The old park's `u8 flag -> 359 words' is still true; u8 is one step too
      narrow and costs a word.)

      AND THE MECHANISM IS *NOT* THE OBVIOUS ONE -- worth recording because
      the obvious one is what this park would otherwise have claimed.
      It is NOT QTY_CMP_PRI's size term: `qty_size' comes from
      PSEUDO_REGNO_SIZE (local-alloc.c:2068), which regs.h:65 defines in
      WORDS -- `(GET_MODE_SIZE + UNITS_PER_WORD - 1) / UNITS_PER_WORD' -- so
      it is 1 for HImode and SImode alike and QTY_CMP_PRI (local-alloc.c:1694)
      cannot see the change.  And it is not a reference count either:
      diffing the two `.lreg' flow-info blocks, `p' is `5 times across 596
      insns; crosses 20 calls' and `flag' is `2 times across 287 insns;
      crosses 20 calls' in BOTH, byte for byte, and neither is printed with a
      `2 bytes' tag -- MIPS's PROMOTE_MODE keeps `flag' in an SImode pseudo
      either way.
      What actually differs is an EXTRA PSEUDO THAT THE EMITTED CODE DOES NOT
      CONTAIN.  `.greg's dispositions gain `266 in 19', every pseudo from 266
      up is renumbered, and five live lengths move by one or two insns
      (81: 618->620, 84: 630->632, 85: 612->614, 90: 95->96, 93: 9->10).
      That is enough to break local-alloc's ties the other way: `flag' takes
      $s3 and `p' is pushed to $s4, which is the target's assignment.  Both
      bodies emit 358 words and the 11 rows between them are ALL
      register-name-only.

   MEASURED AND REJECTED ON THE 1-ROW (candD) BODY -- the last row only:
     second AddPrim written `r - 0x28'                 360 words / 128 rows.
       It is semantically what the target does -- q's last use becomes
       `q[0x7] |= 2' exactly as in the target -- but it needs an EIGHTH
       callee-saved register ($s8 appears) and the whole file permutes.
       With a `q++; q--;' pair added: still 360 / 128, so it is not a
       reference count.
     second AddPrim written `c - 0x50'                 358 / 2  (one worse)
     dropping `__asm__("" : "=r"(r) : "0"(r));'        358 / 80
     dropping `__asm__("" : "=r"(q) : "0"(q));'        359 / 128
     dropping both                                     359 / 124
     adding the same device on `c'                     358 / 1   (no change)
     `u32 flag'                                        358 / 11
     `if (flag)' for `if (flag != 0)'                  358 / 11
     a cancelled `flag++; flag--;' at five positions   358 / 11 each
     `q += 0x28; r = q; q -= 0x28;' for `r = q + 0x28'
       with both asm devices                           359 / 104
       with only the asm on r / with neither           360 / 220 both
     `r = q; r += 0x28;' with the asm on r             361 / 221
     writing the +0x8D0 write-back through `base'
       instead of `g->unk_00'                          360 / 154
     DROPPING THE `base' LOCAL ENTIRELY (all four uses
       spelled `D_80083160.unk_00')                    358 / 1 -- EXACTLY
       EQUAL, so `base' is free either way and is kept only for legibility
     DROPPING the `r++; r--;' cancelled pair           358 / 10, so the pair
       is still worth NINE rows after all four changes above

   ALL FOUR SWEEPS ARE CLEAN ON THIS BODY (2026-11-21, measured on the
   1-row body itself, not on an ancestor):
     widthsweep  -- no width change helps
     devsweep    -- 1680 candidates/round, no wrapper helps
     stmtsweep   -- 1 movable assignment, no safe move helps
     refsweep    -- 1725 insertion points over 14 names, no cancelled pair
   (refsweep was also clean on the 15-row intermediate.)
   The permuter was NOT run: another session's search held the one-at-a-time
   slot for the whole of this run.  THIS PARK IS NOW A ONE-INSTRUCTION
   RESIDUE AT THE CORRECT WORD COUNT, which is the best shape to permute.

   STILL UNHAPPY ABOUT: the two `__asm__' lines.  They are the park's own
   long-standing device and they are still load-bearing (80 and 128 rows
   without them); the device-free alternatives `r = q; r += 0x28;' and a
   `do { } while (0)' after the assignment were measured at 223 rows on the
   pre-cursor body and are not worth re-measuring until something else moves.
   ----------------------------------------------------------------------

   PREVIOUS HEADER FOLLOWS (its "measured and rejected" list is scoped to the
   pre-cursor body and most of it is now void).

   ============================== 2026-11-21: 71 rows -> 70

   ====================================== 2026-11-21: 60 rows -> 55

   Two cancelled pairs on `r` from tools/decomp/refsweep.py, before
   `r[0xD] = r[0x15] += 0x40;` and before `r[0x2D] >>= 1;`: 60 rows -> 55.
   Note both are INSERTIONS ahead of those statements and neither moves them --
   this park has twice rejected a permuter find that moved `r[0x2C]`/`r[0x2D]`
   ahead of the store that initialises that word.

   ====================================== 2026-11-21: THE SAME REJECTION
                                          TWICE, ON A DIFFERENT BYTE

   The second run (base 6375, best 2530) found 63 rows, and its change is
   `r[0x2D] >>= 1;` moved ahead of `*(u32 *)(r + 0x2C) = 0x606060;` -- the
   same defect as the first run's `r[0x2C] >>= 1;`, one byte over.  The shift
   reads the previous frame's packet buffer and the store then overwrites the
   word, so the POLY_F4's green channel ends 0x60 where the target's is 0x30.

   TWO INDEPENDENT RUNS CONVERGED ON THE SAME ILLEGAL MOVE, which is what to
   expect: the permuter scores the object, the object does not care which value
   reaches the GPU, and the move happens to shorten a live range.  Any find
   here that touches the three `>>= 1' shifts needs reading before measuring.

   ====================================== 2026-11-21: 70 rows -> 60

   A duplicated-arm `if (flag)` around the cursor write-back
   `*(u32 *)(g->unk_00 + 0x8D0) = (u32)(r + 0x58);`, found by
   tools/decomp/devsweep.py.  Ten rows on one wrapper, and `flag' is the right
   condition because it is the one name in the function whose reference count
   this residue is about -- the sweep tried all fourteen.

   A `do { } while (0);` around `*(u32 *)(r + 0x2C) = 0x606060;` measures 68,
   so there are two independent wrappers here and the sweep kept the better.
   Worth re-running devsweep on this body for the second one.

   `u8 *dr = r + 0x4C;` computed between `ry = *(u16 *)(p + 0x6);` and
   `rc.y = ry;`, and passed to the final func_80067E2C instead of the inline
   `r + 0x4C`.  One row; the `addiu` moves up out of the call sequence.

   REJECTED FROM THE SAME PERMUTER RUN -- and this one is a correctness
   rejection, not a score one.  Its best find (64 rows, 7 better) moves
   `r[0x2C] >>= 1;` to BEFORE `*(u32 *)(r + 0x2C) = 0x606060;`.  The shift
   then reads whatever the previous frame left in the packet buffer and the
   store overwrites it, so the first colour channel ends at 0x60 where the
   target's is 0x30: the POLY_F4's red is wrong and the whole 7 rows is that
   dead statement.  The permuter cannot see it because the object it diffs
   does not care which value reaches the GPU.  Checking what a find MEANS, not
   just what it scores, is the second gate after measuring it.
   target 358 words, candidate 358 words, 70 aligned differences

   PARKED 2026-11-16, j50.  target 358 words, candidate 358 words,
   87 differing (ovlverify masked); aligndiff 71 aligned differences, EVERY
   instruction positionally aligned and most rows register-name-only.

   WHAT THIS FUNCTION IS: the renderable callback func_8009ED30 installs
   (csrc/ovl_dungeon.c:12997 hands it to func_8004491C).  It emits, off
   D_80083160.unk_00's +0x8D0 cursor: a DR_AREA set to the draw environment's
   own clip RECT, one POLY_FT4 projected through four func_80065320 calls, a
   COPY of that POLY_FT4 0x28 bytes later with its two u/v bytes biased by
   0x40 and 0x100 ORed into the second tpage, a POLY_F4 whose colour is
   0x606060 halved per channel and whose four screen pairs are copied out of
   the first quad, a 0xC-byte func_80067F20 primitive, and a second DR_AREA
   from the caller's own +0x04..+0x0B rect -- biased +0xE0 in y whenever
   D_80083160.unk_00 is not D_801C9E40.  All six hang on the OT row at
   sp->unk_20 + b->unk_0A * 4.

   DECLARATIONS THIS BODY NEEDS: the UnkDungeonSPB typedef (in the agent's
   report) and a PROTOTYPE CHANGE -- csrc/ovl_dungeon.c:1515 declares
   `void func_8009E798();`, but the function returns 0 and takes three
   arguments, so that line must become
   `s32 func_8009E798(u8 *, UnkVec *, UnkSlot *);`.  Its only use is
   `func_8004491C(n, (u32)func_8009E798);` inside the landed func_8009ED30,
   which re-measures 71 words / 0 differing with the change.

   THE RESIDUE is one callee-saved slot: `r` lands in $s1 where the target
   has $s2, and the target keeps a separate $s1 as a rolling cursor for the
   four later primitives (r+0x28, r+0x40, r+0x4C, r+0x58) while mine reuses
   $s0 after `q` dies.  `p` and `flag` are also exchanged ($s3/$s4).  Three
   smaller clusters follow from that: the `*(u16 *)(r + 0x16) |= 0x100` pair
   lands after the +0x25 byte update instead of between the two byte updates;
   the POLY_F4 copy loads three source words before its first store where the
   target loads four; and the second AddPrim passes `q` where the target
   rematerialises `r - 0x28`.

   THE TWO `__asm__` LINES ARE LOAD-BEARING AND EACH IS MEASURED.  Without
   them the function is 358 words / 223 differing.  cse folds `r = q + 0x28`
   into every later `r + N` as `q + (0x28 + N)` -- the .lreg dump shows the
   tail computing `(plus (reg/v:SI 89) (const_int 116))` and `(const_int 128)`
   straight off `q`, so `r` never reaches the allocator at all and `q` stays
   live to the end of the function, which makes it a GLOBAL allocno; the
   globals are then handed $s4..$s7 after local-alloc has already taken
   $s0..$s3, and the whole callee-saved file permutes.
     `__asm__("" : "=r"(r) : "0"(r));`   -- the matched-operand form from the
   catalogue's "A MATCHED-OPERAND `asm` IS A SECOND SET THAT IS *NOT* A
   SCHEDULING BARRIER" -- makes `r` opaque to cse, which alone takes it to
   357 words / 122 differing: right, but ONE WORD SHORT, because `r` now has
   two sets, loses `adjust_priority`'s single-set boost, and sched1 hoists the
   `addiu` into a load-delay nop 12 words earlier.
     `__asm__("" : "=r"(q) : "0"(q));`   -- the same device on `q`, placed
   immediately before `r = q + 0x28;` -- puts the `addiu` back where the
   target has it and the function is 358 words / 87 differing.  An
   `__asm__ __volatile__("")` barrier in the same place measures identically
   (358 / 87); an input-form `__asm__("" ::"r"(q))` is 359 / 331.

   MEASURED AND REJECTED:
     one `s16 a` shared by the x and y sign blocks    223 -> 87 is partly this:
       a shared local is live across both blocks, so it is a global allocno and
       cannot coalesce with the sign-extended byte; two locals give the target's
       `sll`/`sra`/`sh`/`sh`/`addu`/`sh`/`sh` in $v0 exactly (5 differing)
     `sp->unk_24 = s->unk_14;` before `e = s->unk_08;`  359 words / 309 --
       with `e` first the `lw` fills the `lhu 0x14` load delay and PushMatrix's
       delay slot takes the `sh 0x24`; the other way the `sh` is emitted before
       the call and `q += 0xC` (a callee-saved pseudo add, which sched2 moves
       freely across a call) steals the delay slot
     rc.y written `*(u16 *)(p + 0x6) + 0xE0` in the arm   361 words; the three
       stores to the address-taken `rc` kill cse's entry and the load returns
       with its own nop.  The `ry` local must be assigned AFTER `rc.x`, not
       before, or its load fills the first `lhu`'s delay slot (357 words)
     an explicit rolling cursor `c` for the four tail primitives   361 words
     `z = r + 0x28` as its own local                      362 words
     `r = q; r += 0x28;` (two sets, no asm)               223 (r still folds)
     a `do { } while (0)` after `r = q + 0x28;`           223 (ditto)
     second AddPrim written `r - 0x28`                    no change
     `q[0x7] |= 2;` moved before the memcpy               224
     dropping the `base` local                            no change
     declaration orders q/e/r, e/q/r, q/r/e               no change
     s32 for the sign-block locals                        356 words (`lb`)
     chained `sp->unk_80 = sp->unk_70 = a = ...`          no change
     `u8 flag`                                            359 words
     the `|= 0x100` moved first or last in its group      no change 
   ---------------------------------------------------------------- 2026-11-16

   THE UnkDungeonSPB TYPEDEF WAS LOST.  It existed only in its author's
   hand-back, so this park had not compiled from the repo at all and was
   invisible to parkresidue's ranking.  Recovered below from the target's own
   listing, which writes down every offset and every width:

     python3 - <<'EOF'
     grep -oE '(lw|lh|lhu|lb|lbu|sw|sh|sb) +\$[a-z0-9]+, \(0x1F800[0-9A-F]{3} & 0xFFFF\)' \
         asm/nonmatchings/ovl_dungeon/func_8009E798.s
     EOF

   offset  instructions seen        conclusion
     0x00  lh, lhu, sh              halfword, read BOTH ways -- see below
     0x02  lhu, sh                  u16
     0x08  lw, sw, lhu              UnkDungeonSPW (word written, halfword read)
     0x0C  lw, sw, lhu              UnkDungeonSPW
     0x10  lw, sw, lhu              UnkDungeonSPW
     0x14  lw, sw, lhu              UnkDungeonSPW
     0x20  lw, sw                   the OT base, a pointer
     0x24  sh                       written only; signedness unobservable
     0x28, 0x2A, 0x2C   sh          written only
     0x30, 0x34, 0x38   sw          s32
     0x48  sw                       s32
     0x70..0x8C         sh          the six screen pairs, written only
     0x98, 0x9A, 0x9C   sh          written only
     0xE4, 0xE8, 0xEC   sw          s32
     0x100, 0x102       lhu, sh     u16

   DECLARATIONS
   typedef struct UnkDungeonSPB { s16 unk_00; u16 unk_02; u8 pad_04[0x4]; UnkDungeonSPW unk_08; UnkDungeonSPW unk_0C; UnkDungeonSPW unk_10; UnkDungeonSPW unk_14; u8 pad_18[0x8]; u8 *unk_20; s16 unk_24; u8 pad_26[0x2]; s16 unk_28; s16 unk_2A; s16 unk_2C; u8 pad_2E[0x2]; s32 unk_30; s32 unk_34; s32 unk_38; u8 pad_3C[0xC]; s32 unk_48; u8 pad_4C[0x24]; s16 unk_70; s16 unk_72; s16 unk_74; u8 pad_76[0x2]; s16 unk_78; s16 unk_7A; s16 unk_7C; u8 pad_7E[0x2]; s16 unk_80; s16 unk_82; s16 unk_84; u8 pad_86[0x2]; s16 unk_88; s16 unk_8A; s16 unk_8C; u8 pad_8E[0xA]; s16 unk_98; s16 unk_9A; s16 unk_9C; u8 pad_9E[0x46]; s32 unk_E4; s32 unk_E8; s32 unk_EC; u8 pad_F0[0x10]; u16 unk_100; u16 unk_102; } UnkDungeonSPB;
   s32 func_8009E798(u8 *, UnkVec *, UnkSlot *);
   END DECLARATIONS

   (Written on one line because the DECLARATIONS block is read a line at a
   time.  Total size 0x104; every pad is derived, not guessed -- check the
   arithmetic before changing a field width.)

   WHAT IS STILL OPEN, and it is only the SPELLING, not the layout:

   `unk_00` is read `lh` at 8009E8A0 and `lhu` at 8009E914, so one face of it
   is wrong whichever way it is declared.  As `s16` the second read needs a
   `(u16)` cast; as `u16` the first needs `(s16)`.  Note that the body already
   writes `(s16)sp->unk_02` for a field the listing only ever reads `lhu`, so
   the cast does NOT fold to `lh` in this compiler -- which argues for `s16`
   here and a `(u16)` cast at the subtraction.  ONE COMPILE SETTLES IT; do
   that before anything else and record which way it went.

   The fields marked "written only" have no observable signedness.  s16 is the
   assumption; if a residue lands on one of them, that is the first thing to
   vary and it costs nothing.
*/
s32 func_8009E798(u8 *p, UnkVec *b, UnkSlot *s)
{
    Unk80083160 *g = &D_80083160;
    UnkDungeonSPB *sp = (UnkDungeonSPB *)0x1F800000;
    u8 *base = D_80083160.unk_00;
    u8 *q;
    u8 *r;
    u8 *e;
    s16 flag;
    s32 ry;
    s16 a;
    s16 bb;
    RECT rc;
    u8 *c;

    q = *(u8 **)(base + 0x8D0);
    sp->unk_20 = base + 0xB0;
    sp->unk_38 = 0x1000;
    flag = base != D_801C9E40;
    sp->unk_48 = 0;
    func_80067E2C((DR_AREA *)q, (RECT *)base);
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, q);
    sp->unk_30 = s->unk_1C;
    sp->unk_34 = s->unk_1E;
    sp->unk_00 = *(u16 *)((u8 *)b + 0x2);
    sp->unk_02 = *(u16 *)((u8 *)b + 0x6);
    e = (u8 *)s->unk_08;
    sp->unk_24 = s->unk_14;
    PushMatrix();
    sp->unk_98 = 0;
    sp->unk_9A = 0;
    sp->unk_9C = g->unk_C8;
    RotMatrix((SVECTOR *)0x1F800098, (MATRIX *)0x1F8000D0);
    ScaleMatrix((MATRIX *)0x1F8000D0, (VECTOR *)0x1F800030);
    sp->unk_E4 = sp->unk_00;
    sp->unk_E8 = (s16)sp->unk_02;
    sp->unk_EC = 0;
    func_80064CF0((MATRIX *)0x1F8000D0);
    func_80064D80((MATRIX *)0x1F8000D0);
    sp->unk_28 = 0x20 - s->unk_20;
    sp->unk_2A = 0x20 - s->unk_22;
    sp->unk_2C = 0;
    func_80065320((SVECTOR *)0x1F800028, (s32 *)0x1F800100, (s32 *)0x1F800094);
    q += 0xC;
    sp->unk_100 = sp->unk_100 - sp->unk_00;
    sp->unk_102 = sp->unk_102 - sp->unk_02;
    sp->unk_08.w = e[0x8];
    sp->unk_0C.w = e[0x9];
    sp->unk_10.w = e[0xA];
    sp->unk_14.w = e[0xB];
    a = (s8)e[0x2];
    sp->unk_80 = a;
    sp->unk_70 = a;
    a = a + sp->unk_10.h;
    sp->unk_88 = a;
    sp->unk_78 = a;
    bb = (s8)e[0x3];
    sp->unk_7A = bb;
    sp->unk_72 = bb;
    bb = bb + sp->unk_14.h;
    sp->unk_8A = bb;
    sp->unk_82 = bb;
    sp->unk_8C = 0;
    sp->unk_84 = 0;
    sp->unk_7C = 0;
    sp->unk_74 = 0;
    func_80065320((SVECTOR *)0x1F800070, (s32 *)(q + 0x8), (s32 *)0x1F800094);
    func_80065320((SVECTOR *)0x1F800078, (s32 *)(q + 0x10), (s32 *)0x1F800094);
    func_80065320((SVECTOR *)0x1F800080, (s32 *)(q + 0x18), (s32 *)0x1F800094);
    func_80065320((SVECTOR *)0x1F800088, (s32 *)(q + 0x20), (s32 *)0x1F800094);
    *(s16 *)(q + 0x8) = *(u16 *)(q + 0x8) + sp->unk_100;
    *(s16 *)(q + 0x10) = *(u16 *)(q + 0x10) + sp->unk_100;
    *(s16 *)(q + 0x18) = *(u16 *)(q + 0x18) + sp->unk_100;
    *(s16 *)(q + 0x20) = *(u16 *)(q + 0x20) + sp->unk_100;
    *(s16 *)(q + 0xA) = *(u16 *)(q + 0xA) + sp->unk_102;
    *(s16 *)(q + 0x12) = *(u16 *)(q + 0x12) + sp->unk_102;
    *(s16 *)(q + 0x1A) = *(u16 *)(q + 0x1A) + sp->unk_102;
    *(s16 *)(q + 0x22) = *(u16 *)(q + 0x22) + sp->unk_102;
    sp->unk_10.w = sp->unk_10.w + sp->unk_08.w;
    sp->unk_14.w = (sp->unk_14.w + sp->unk_0C.w) << 8;
    sp->unk_0C.w = sp->unk_0C.w << 8;
    *(s16 *)(q + 0xE) = s->unk_12 + *(u16 *)(e + 0x6);
    *(s16 *)(q + 0xC) = sp->unk_0C.h + sp->unk_08.h;
    *(u32 *)(q + 0x14) =
        sp->unk_0C.w + sp->unk_10.w + ((s->unk_10 | *(s16 *)(e + 0x4)) << 16);
    *(s16 *)(q + 0x1C) = sp->unk_14.h + sp->unk_08.h;
    *(s16 *)(q + 0x24) = sp->unk_14.h + sp->unk_10.h;
    *(u32 *)(q + 0x4) = *(u32 *)s->unk_0C;
    func_800666F4((POLY_FT4 *)q);
    __asm__("" : "=r"(q) : "0"(q));
    r = q + 0x28;
    __asm__("" : "=r"(r) : "0"(r));
    func_8003DB6C((int *)r, (int *)q, 0xA);
    c = r + 0x28;
    q[0x7] |= 2;
    r[0xD] = r[0x15] += 0x40;
    *(u16 *)(r + 0x16) |= 0x100;
    r++;
    r--;
    r[0x1D] = r[0x25] += 0x40;
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, r);
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, q);
    *(u32 *)(c + 0x4) = 0x606060;
    *(s32 *)(c + 0x8) = *(s32 *)(c - 0x20);
    *(s32 *)(c + 0xC) = *(s32 *)(c - 0x18);
    *(s32 *)(c + 0x10) = *(s32 *)(c - 0x10);
    *(s32 *)(c + 0x14) = *(s32 *)(c - 0x8);
    c[0x4] >>= 1;
    c[0x5] >>= 1;
    c[0x6] >>= 1;
    func_800666E0((POLY_F4 *)c);
    c[0x7] |= 2;
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, c);
    c = r + 0x40;
    func_80067F20((u32 *)c, 0, 0, 0x40, 0);
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, c);
    rc.x = *(u16 *)(p + 0x4);
    ry = *(u16 *)(p + 0x6);
    c = r + 0x4C;
    rc.y = ry;
    rc.w = *(u16 *)(p + 0x8);
    rc.h = *(u16 *)(p + 0xA);
    if (flag != 0)
    {
        rc.y = ry + 0xE0;
    }
    func_80067E2C((DR_AREA *)c, &rc);
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, c);
    PopMatrix();
    c = r + 0x58;
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)c;
    return 0;
}
