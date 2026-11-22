/* PARK -- func_800B51C8, agent j59 2026-11-16..21, agent b51c8 2026-11-21.

   ================================= 2026-11-21 (pass 3): 228 rows -> 24 rows,
                                     and the word count is now EXACT.

       target 834 words, candidate 834 words, 24 aligned differences.

   Nine changes, each measured on the body before it.  None of them is a new
   device; TWO of the park's four existing devices were DELETED (they had been
   measured on a body whose register assignment they were compensating for).
   Everything below is `aligndiff.py` on a real unit copy, one candidate at a
   time; the unit is only ~1030 lines since the carve, so a candidate measures
   in 0.22 s and no standalone was needed.

   ---------------------------------------------------------------- THE NINE

   (1) 228 -> 209.  DELETE `u8 *c;`.  `c = e + 4` made `c` a SECOND BIV of
       each inner loop, and `.loop` shows what that costs:

           Insn 911/915/919/923/928/962: dest address src reg 91  add 4 5 6 7 -2 -1
           Insn 1276:                    dest address src reg 91  add 2
           giv at 962/928/923/919/915/911 combined with giv at 1276
           giv at 1276 reduced to (reg:SI 670)

       combine_givs (loop.c:5783) builds giv_array from `bl->giv`, which is a
       PREPEND list (`v->next_iv = bl->giv; bl->giv = v;`, loop.c:4692),
       so giv_array[0] is the LAST giv discovered in the body and
       that one becomes the base every other giv is expressed from.  The park's
       last `c` use was `*(u16 *)(c + 2)`, so the base came out at `e + 6` and a
       THIRD stepping register appeared (`addiu s2,s3,6` beside `addiu s5,s3,4`),
       giving `lbu v0,2(s2)` where the target has `lbu $v0, 0x4($s2)`.

       Write every access off `e` instead -- c[4]->e[8], c[5]->e[9], c[6]->e[10],
       c[7]->e[11], c[-2]->e[2], c[-1]->e[3], *(u16 *)(c+2)->*(u16 *)(e+6),
       *(u16 *)c->*(u16 *)(e+4) -- and the givs sit on biv `e` with adds
       2,3,4,5,6,7,8,9,10,11.  BOTH arms end on the add-4 giv (arm A's last e
       use is `*(u16 *)(q+0x16) = *(u16 *)(e+4)`, arm B's is the shadow's
       `*(s16 *)(e+4) << 16`), so the base is `e + 4` in both: exactly the
       target's `addiu $s2, $s3, 0x4` with offsets -2,-1,0,2,4,5,6,7.

       FRAGILE: if a LATER e-relative reference is ever added to either arm,
       or the add-4 read is moved earlier, the base moves with it.

   (2) 209 -> 218 (deliberately WORSE) -- but it is the only honest body.
       The park's live body declared `void *md = md; void *ms = ms; void *f0 =
       f0; void *f1 = f1;` -- self-initialising, i.e. four UNINITIALISED
       pointers handed to RotMatrix/CompMatrix/ScaleMatrix.  The four
       `lui`/`ori` pairs the target has at 800B51DC..800B5204 were therefore
       never emitted and the candidate was 7 words SHORT, making them up
       elsewhere.  Initialised honestly: 836 words / 218 rows.  Do not go back.

   (3) 218 -> 132.  `s` LOSES ITS CALLEE-SAVED REGISTER, which the old header
       correctly called "THE SINGLE BIGGEST REMAINING LEVER".

       It is an allocno_compare (global.c:587) tie and the dumps price it:

           pri = floor_log2(n_refs) * n_refs / live_length * 10000 * size

       with update_equiv_regs (local-alloc.c:1105) DOUBLING live_length for a
       pseudo that is set once and carries a REG_EQUIV -- true of a parameter
       (equiv = its 0x90($sp) home slot) AND of a pointer local initialised
       from a constant (equiv = the constant).  On the 218-row body, from
       dump_flow_info at the top of `.lreg`:

           s   (82): 16 refs / 1482 -> 4*16*10000/2964 = 215
           f1  (87): 10 refs /  735 -> 3*10*10000/1470 = 204
           f0  (86): 10 refs /  737 -> 3*10*10000/1474 = 203

       Ten quantities wanted the nine callee-saved registers; `s` beat f0 and
       f1 by eleven points and f0 was left in memory.

       NO DEVICE CAN FIX THIS.  Every device ADDS references, and a cancelled
       pair on a constant-initialised pointer is folded away by cse before flow
       counts it: measured on that body, `f0++/f0--` 213, `f1++/f1--` 218, both
       218, all four 218 -- noise.  What fixes it is that f0 and f1 ALREADY
       OCCUR under a second spelling.  The shadow pass's
       `func_80065420(p, &p->unk_B8, &p->unk_90, &p->unk_94)` passes 0x1F800090
       and 0x1F800094, and the target proves they are the same two variables:
       800B5B78 is `addu $a2, $fp, $zero` / `addu $a3, $s7, $zero`, a copy out
       of the constant registers, NOT an `addiu` off $s1.  Writing that one
       call `func_80065420(p, &p->unk_B8, f0, f1)` gives f0 and f1 four more
       references each, lifts both past `s`, and `s` -- now last -- goes to its
       home slot and is re-loaded with `lw $t0, 0x90($sp)` five times, exactly
       as the target does.  The prologue, the epilogue, the frame size (0x90 ->
       0x88) and every stack displacement come right in one step.

       AND THE OTHER CALL KEEPS THE LONG SPELLING.  800B569C is
       `addiu $a2, $s1, %lo(D_1F800090)` / `addiu $a3, $s1, %lo(D_1F800094)`,
       computed off `p`.  Measured on the 218-row body:
           shadow call only  132     both calls  134
           first call only   136     neither     218

   (4) 132 -> 120.  DELETE the two duplicated-arm devices, `if (ms) { q[0x7]
       |= 2; } else { ... }` and `if (md) { *(u16 *)(q+0x20) = ...; } else
       { ... }`.  They were worth +8 and +10 rows on the 228-row body and they
       are worth -8 and -10 on this one, because the `if (md)` condition is
       THREE weighted references to `md` (it is inside arm B's inner loop, so
       loop_depth 3) and that is exactly what lifts md past i in the same
       allocno_compare order:

           ms (85): 30 refs / 739 doubled -> 812
           md (84): 18 refs / 741 doubled -> 486     <- 15 without `if (md)`
           i  (91): 16 refs / 715                    -> between 215 and 486

       (`.greg`'s printed order on that body is `... 90 85 84 91 82 87 86 ...`
       = e, ms, md, i, s, f1, f0, and the first six of those prices reproduce
       it exactly.  `i` only fits that order if its live_length is doubled
       too, which I could NOT reconcile with reg_n_sets == 2 for a `for`
       counter -- so treat 447 as inferred from the order, not derived.  What
       IS measured is that taking md from 18 weighted references to 15 swaps
       md and i and is worth 10 rows.)

       Target is ms=$s4, i=$s5, md=$s6; the device made it ms=$s4, md=$s5,
       i=$s6 and every `move a1,s5` / `sll v1,s6,2` row in the residue was
       that.  Dropping `if (md)` takes md to 15 refs, floor_log2 steps 4 -> 3,
       486 -> 303, and i goes back to $s5.  Measured: -C 127, -D 125, -C-D 120.
       The two `a++; a--;` pairs were re-ablated on the same body and still pay
       (-A 137, -B 136, -A-B 141), so they STAY.

   (5) 120 -> 117.  The tail reads D_80083160 through `gg`:
       `*(u8 **)(gg->unk_00 + 0x8D0) = ...`.  The target's tail is
       `lui $t0, %hi` / `addiu $t0, $t0, %lo` / `lw $a0, 0x0($t0)` -- a pointer
       plus a zero-offset load -- where the bare `D_80083160.unk_00` gives
       `lui` / `lw %lo(...)`.  (`gg` for the two 0xB8 reads was already in the
       park; `cv[] via gg`, `p->unk_20 via gg` and the head's 0x8D0 read via
       `gg` all measure EXACTLY the same as the bare symbol -- only the tail
       moves.)

   (6) 117 -> 102 and 841 -> 835 words.  In arm A, the two `>= 0x100` tests
       read back the fields that were just stored:

           p->unk_08.w = e[8];
           p->unk_10.w = e[10];
           if (p->unk_08.w + p->unk_10.w >= 0x100) p->unk_10.w = p->unk_10.w - 1;

       Reading `e[8] + e[10]` again costs FOUR WORDS: the intervening stores
       through `p` stop cse re-using the two `lbu` results (both MEMs are
       MEM_IN_STRUCT_P and may alias), so the bytes are loaded twice.  A load
       of a location that was just STORED is re-used from the stored register,
       which is what the target does -- 800B53E0 `lbu $v0, 0x4($s2)` /
       800B53E8 `sw $v0, 0x08` / 800B53EC `lbu $v1, 0x6($s2)` / 800B53F4
       `addu $v0, $v0, $v1`, with `sw $v1, 0x10($s1)` sunk into the branch
       delay slot.  Variants: read-back on both 102, on one 110,
       `p->unk_10.w = e[10] - 1` in the arm 111, park's own 117.

   (7) 102 -> 89.  Preamble statement order: the 0x18 scratchpad store moves
       ABOVE `p->unk_20 = ...`.  All 120 orderings of the five preamble
       statements were swept; (gg, p18, u20, zer, cv) and its three siblings
       give 89, the next group 93, the park's own order 94.

   (8) 89 -> 71.  EVALUATE THE LIMIT FIRST.  Both post-clamp tests are written
       with the operand the target loads first on the LEFT:

           if (*(s16 *)(q + 0x8) > *(s16 *)(q + 0x20))     not  0x20 <  0x8
           if (*(s16 *)(q + 0xA) > *(s16 *)(q + 0x22))     not  0x22 <  0xA

       The target loads `lh $a1, 0x8($s0)` at 800B5A44, three insns before the
       `lhu 0x14` it is compared against, and `lh $v1, 0xA($s0)` at 800B5A84
       BEFORE `lh $v0, 0x22($s0)`.  Operand 0 of the comparison is expanded
       first, so the spelling decides it: A alone 75, B alone 85, both 71.

   (9) 71 -> 51 -> 42 -> 32 -> 24, four single statement facts:
         * move the shadow's `q[0x3] = 9;` ONE statement later, below
           `*(u16 *)(q + 0x22) = ...` (71 -> 59; one earlier 71, two later 73).
         * `p->unk_10.w += p->unk_08.w - 1;` instead of
           `p->unk_10.w = p->unk_10.w - 1 + p->unk_08.w;`, and the same for
           unk_14/unk_0C (59 -> 51; one of the two alone 55).
         * `sv.vz << 16 >> 16` instead of `(s16)sv.vz` at the func_800BCB04
           call (51 -> 42).  `(s16)` of an ADDRESSABLE s32 stack local that was
           just stored folds to a single `lh 0x58($sp)`; the target sign-extends
           the REGISTER (`sll $a2,$v0,16` / `sra $a2,$a2,16`) and puts the
           `sw $v0, 0x58($sp)` in the call's delay slot.  `(s16)(s32)sv.vz`,
           `(s16)(sv.vz + 0)` and `(s16)(sv.vz | 0)` are all still 51; only the
           explicit shift pair moves it.
         * `r->unk_10 = p->unk_04;` MOVED ABOVE the `e == NULL` test
           (42 -> 32, and the last missing word).  The target executes it in
           the delay slot of `beqz $s3`, i.e. unconditionally, and reuses the
           `lhu 0x12($s0)` that `p->unk_04 = r->unk_12;` loaded.  Reading
           `r->unk_12` a second time there is 43 rows; leaving it below the
           test is 42.
         * `gg = ...` and the four scratchpad constants assigned as STATEMENTS
           immediately AFTER `func_80064AE0(&m);` rather than at the top
           (32 -> 24).  This is the one shape I am unhappy about; see below.

   ---------------------------------------------------- WHAT IS LEFT: 24 ROWS

   Three clusters, all register-assignment / scheduler ties, no word delta:

   A. 6 rows, prologue.  `addu $s5, $zero, $zero` (i = 0) and its `sw $s5,
      0x74($sp)` are insns 2-3 in the target and 15-16 in the candidate, and
      the candidate has `move t0,v1` where the target has a second
      `lui $t0, %hi(D_80083160)` -- cse shares the one `(high (symbol_ref))`
      between `gg` and the `D_80083160.unk_00` load and pays a copy for it.
      Swept: all five preamble orders, the four constants as initialisers vs
      statements at six positions, `i = 0;` hoisted out of the `for`, and
      `gg` at every statement position in the preamble.  Best is the body
      below (24); initialisers at the declaration is 32, `gg` at the top with
      the constants after the call is 30, both at the top is 32.

   B. 8 rows, `r = &D_800E39C8[i];`.  The target computes the base first
      (`lui`/`addiu` at 800B52C4-C8, into $v0) and the index second (into
      $v1); the candidate does the reverse.  The base pseudo carries
      REG_EQUIV (symbol_ref D_800E39C8), so global_alloc never allocates it
      and reload REMATERIALISES `high`/`lo_sum` immediately before the use --
      giving them a HIGHER INSN_LUID than the `sll/addu/sll` chain.  All of
      sched2's insns here are priority 1 and ref_count 1-2, so
      rank_for_schedule (sched.c, third key) falls through to INSN_LUID and
      the higher LUID is placed later-from-the-end, i.e. the index chain comes
      out first.  Five spellings measured IDENTICAL (`&D_800E39C8[i]`,
      `D_800E39C8 + i`, `(u8 *)D_800E39C8 + i*0x18`, `(s32)D_800E39C8 +
      i*0x18`, `i*0x18 + (s32)D_800E39C8`), so the spelling is not the lever.

   C. 10 rows, the `b` block in BOTH arms (800B54B8 and 800B5824).  The
      target is `addu $v1, $v1, $v0` -- the sum lands in `b`'s own register --
      and the candidate is `addu $v0, $v1, $v0`, so the two `sh` that follow
      differ too.  The sum is a separate SImode pseudo (a HImode local
      receiving an SImode expression; the copy back to `b` is deleted), and
      which register it gets is decided by local_alloc's QTY_CMP_PRI
      (local-alloc.c:1693), `floor_log2(n_refs)*n_refs*size/(death-birth)`:
      `p->unk_14.h`'s two-insn quantity outscores `b`'s, takes $v0, and the
      sum then takes $v0 as the first free register at its birth.  The same
      block for `a` is CORRECT, and the difference is the `a++; a--;` pair
      (ablating it costs 5 rows).  The analogous pair on `b` does NOT work --
      measured at all six positions in the block: 78, 60, 42, 42, 42, 42
      (42 = no change), a doubled pair 47, arm A only 69.  Also measured and
      rejected on this body: `b` as s32 75, `u16` 24 (identical), `a` as s32
      59, both s32 100, declaring `b` before `a` 24 (identical), swapping
      `p->unk_7A`/`p->unk_72` 30, swapping `p->unk_8A`/`p->unk_82` 30, both
      32, `b += p->unk_14.h` 24 (identical), `b = p->unk_72 + p->unk_14.h`
      24 (identical), `b = p->unk_7A + p->unk_14.h` 89, a `do { } while (0)`
      round the add 87.

   ------------------------------------- ALSO MEASURED AND REJECTED ON THIS BODY

   * DECLARATION ORDER IS IRRELEVANT HERE.  70 random permutations of the
     seven uninitialised locals (r, q, e, i, a, b, gg) all measure exactly
     834 words / 24 rows, so the qty-number tie-break in qty_compare_1 and
     allocno_compare is not what decides cluster C.
   * `gg` INSIDE THE LOOP, to stop cse sharing its `(high (symbol_ref))` with
     the `D_80083160.unk_00` load (the `move t0,v1` in cluster A): before
     `r = &D_800E39C8[i];` 833/31, before `p->unk_00 = ...` 833/34, at the top
     of arm B 833/34, immediately before its own first use 833/34.  All of
     them lose a word as well as rows.
   * ROUTING THE OTHER D_80083160 SITES THROUGH `gg`: cv[] 833/37,
     `p->unk_20` 836/31, the head's 0x8D0 read 836/31, all three 833/28,
     the last two 836/34, and putting the TAIL back on the bare symbol
     833/27.  The body below is the only combination at 834/24.
   * `gg->unk_B8` instead of `*(u16 *)((u8 *)gg + 0xB8)`: 834/24, identical
     code -- fold-const folds the `+ 0xB8` into the symbol either way once the
     base is a pointer, so this one is a free choice.
   * STORE-READBACK FORMS IN THE `b` BLOCK: `p->unk_8A = b + p->unk_14.h;
     p->unk_82 = p->unk_8A;` 834/24 (identical code), `p->unk_72 =
     p->unk_7A;` 834/24 (identical), but writing the sum expression TWICE
     (`p->unk_82 = b + p->unk_14.h;`) is 838/72 -- the second add is not CSEd
     across the first store.
   * DROPPING THE `a++; a--;` PAIR and compensating with a readback
     (`p->unk_88 = a + p->unk_10.h; p->unk_78 = p->unk_88;`): 834/33, same as
     dropping it outright.  The pair is worth 9 rows on this body and there is
     no device-free substitute for it that I found.
   * MOVING `p->unk_14.h` INTO `a` (which is dead by then) so that its
     quantity is long and cheap, at four positions: 837/85 every time.

   ------------------------------------------- THE SHAPE I AM UNHAPPY ABOUT

   `gg` and the four scratchpad pointers are assigned as statements AFTER
   `func_80064AE0(&m);` rather than initialised at their declarations.  That
   is legal and the values are the same real constants, but no human writes it
   that way.  The device-free alternative -- initialise all five at the
   declaration, which is what the original surely looked like -- costs
   EIGHT ROWS (24 -> 32) and nothing else; the word count stays exact.  If you
   would rather have the honest shape, take the 32.
   Intermediate points measured: constants after the call + `gg` at the top
   30; `gg` after the call + constants at the top 27; both after the call 24.

   ----------------------------------------------- RE-MEASURED AND NOW FALSE

   * The old header's "(2) D_80083160 IS THE DECLARED STRUCT AT EVERY SITE,
     NOT A `g` POINTER LOCAL" is still right for the head and the cv[] reads
     and WRONG for the tail (see (5)).
   * Its "(3) ... THE NINE CALLEE-SAVED QUANTITIES THE TARGET WANTS" list is
     exactly right and is now achieved.
   * Its per-block row counts ("5 rows in 800B51xx, 14 in 52xx, ...") are
     stale; the current breakdown is the three clusters above.
   * Its four devices: the two `a++; a--;` pairs are KEPT (worth 5 and 4 rows
     here); the two duplicated-arm `if`s are DELETED (see (4)).
   * The permuter's rejected find (a halfword store rewritten as
     store/read-back/store) is still wrong, and the park's note that the SAFE
     HALF of a find beat the whole find still stands.

   ------------------------------------------------- SWEEPS AND THE PERMUTER

   On the 24-row body below:
     * devsweep -- 195 statements x 15 names, 3120 candidates a round --
       "no wrapper helps": 24 rows, 834/834 words.  Hand-checked separately:
       `do { } while (0)` round the b block's stores or its add is 73 to 95
       rows and 837-839 words, every placement.
     * refsweep -- 15 names, 3936 candidate insertion points --
       "no cancelled pair helps": 24 rows, 834/834 words.
     * stmtsweep: one movable assignment, "no safe move helps".
     * 70 random permutations of the seven uninitialised declarations: all 24.
   On the 42-row body, widthsweep fired only `u16 cv[3] -> u8` and that is 64
   rows, i.e. nothing.
   NO PERMUTER RUN has been made against this body, and it is the obvious next
   step: the three remaining clusters are all allocation / scheduler ties with
   no word delta, which is the shape the permuter is best at, and the park is
   word-exact so measure_finds.py's word-exactness ranking will be meaningful.
   Re-import before running it -- the body below is not the one any earlier
   permimport snapshot holds.

   ====================================================================
   EVERYTHING BELOW THIS LINE IS THE ORIGINAL 2026-11-16..21 HEADER, KEPT FOR
   ITS DECODE.  ITS SCORES BELONG TO BODIES THAT NO LONGER EXIST.
   ====================================================================

   WHAT IT IS.  The 32-slot overlay-sprite renderer.  It walks
   i = 0 .. 0x1F over the two parallel tables D_800E3648 (UnkDungeonSlot,
   4 bytes) and D_800E39C8 (UnkDungeonSpotB, 0x18 bytes) and, for each live
   slot, walks the slot's 0xC-byte part list at r->unk_08 emitting one
   POLY_FT4 per part into the scratchpad display list.

   ITS TEMPLATE IS THE LANDED func_800C6B40 (search csrc for
   `Unk453E0SP *p = (Unk453E0SP *)0x1F800000`).  That function is the same
   renderer for a node rather than a table slot: the 0x1F800070/78/80/88
   corner block, the 0x1F8000F0..FE projected-corner block, the four-corner
   `(u16)(x + 0x20) < 0x181 && (u16)(y + 0x20) < 0x121` visibility OR, the
   `p->unk_20 + p->unk_C0 * 4` AddPrim and the D_8006CD30.unk_1C store are
   all line-for-line the same idiom.  READ IT FIRST.

   PSY-Q IDENTIFICATIONS (all of these are called by their `func_` names in
   this unit already, and are used WITHOUT a declaration -- do not add
   prototypes, the landed func_800B7D74-era code calls func_8006658C bare):
     func_80064AE0(MATRIX *)                 saves the current matrix
     func_80065820(SVECTOR *, MATRIX *)      RotMatrix
     func_80064840(MATRIX *, MATRIX *, MATRIX *)   CompMatrix
     func_80064BC0(MATRIX *, VECTOR *)       ScaleMatrix
     func_80064D80(MATRIX *)                 SetTransMatrix
     func_80064CF0(MATRIX *)                 SetRotMatrix
     func_80065420(a,b,c,d)                  RotTransPers
     func_800654B0(10 args)                  RotTransPers4 -> 0x1F8000F0..FC
     func_80065590(10 args)                  RotTransPers4 straight into the
                                             primitive's 4 xy fields
     func_8006658C(ot, prim)                 AddPrim
     func_800666F4(prim)                     primitive init (sets code/len)

   PROTOTYPE CHANGE REQUIRED: the forward declaration at the top of the file
   is `void func_800B51C8();` and the function RETURNS 0
   (`addu $v0, $zero, $zero` at 800B5E94).  It must become
   `s32 func_800B51C8();` or the unit does not compile.  Its only reference
   is `func_8004491C(n, (u32)func_800B51C8);`, so nothing landed is affected.
   Its third parameter is the one it uses (UnkSlot *s, spilled to 0x90($sp));
   the first two are read only as the ABI requires them.

   FOUR MEASURED STEPS, in order, with their scores:

   (1) FRAME LAYOUT.  `MATRIX m; u16 cv[3]; VECTOR sv;` in that declaration
       order, giving m at sp+0x28, cv at sp+0x48 and sv at sp+0x50, frame
       0x88.  Three separate `u16 cx, cy, cz;` scalars instead get EIGHT
       bytes each (cx at 0x58, cy at 0x60) and the frame comes out 0x90:
       843 words / 327 differences against 846 / 267 with the array.
       `sv` is the ScaleMatrix VECTOR *and* the 0x58 scratch: the listing
       writes 0x58($sp) long before the ScaleMatrix call and then fills
       0x50/0x54 from it, so `sv.vz` is the scratch and
       `sv.vx = sv.vz; sv.vy = sv.vz / 2;` is the fill.

   (2) D_80083160 IS THE DECLARED STRUCT AT EVERY SITE, NOT A `g` POINTER
       LOCAL.  The listing recomputes `lui $t0 / addiu $t0, %lo` inside arm B
       and again in the tail, and loads unk_00 through its OWN
       `lui $v0 / lw %lo($v0)`.  With `Unk80083160 *g = &D_80083160;` the
       base takes a callee-saved register ($s8), which is one of the nine the
       scratchpad pointers need: 843 words / 327.

   (3) THE FOUR SCRATCHPAD CONSTANTS ARE POINTER LOCALS, assigned at the top
       in this order: 0x1F8000D0, 0x1F800050, 0x1F800090, 0x1F800094.  Each
       is a `lui`/`ori` pair in the target, held in $s6/$s4/$fp/$s7 for the
       whole function.  Written inline as `(void *)0x1F8000D0` at each call
       site: 846 words / 267.  As locals: 830 words / 262.
       THE NINE CALLEE-SAVED QUANTITIES THE TARGET WANTS ARE:
         $s0 primitive pointer   $s1 (Unk453E0SP *)0x1F800000
         $s2 part cursor (+4)    $s3 part cursor (+0)
         $s4 0x1F800050          $s5 the 0..0x1F slot index
         $s6 0x1F8000D0          $s7 0x1F800094      $fp 0x1F800090
       which is exactly nine, and it is why `s` is SPILLED to its incoming
       home slot 0x90($sp) and re-loaded with `lw $t0, 0x90($sp)` at each of
       its five uses.  The candidate still gives `s` $s8, so every arm is one
       register out of step -- THAT IS THE SINGLE BIGGEST REMAINING LEVER.

   (4) `*(u8 **)((u8 *)p + 0x18)` is the live primitive pointer: the
       scratchpad word at 0x1F800018 holds it across the whole function and
       it advances by 0x28 per quad.  structs.h's Unk453E0SP has `pad_18`
       there, so it has to be reached with the cast.  The tail writes it back
       to `*(u8 **)(D_80083160.unk_00 + 0x8D0)`.

   OTHER DECODED FACTS WORTH NOT RE-DERIVING:
     * p->unk_00/unk_02/unk_04 are read with `lhu` + `sll 16` + `sra 16`
       wherever a 32-bit value is wanted, i.e. as `(s16)*(u16 *)&p->unk_02`,
       even though structs.h declares them s16.  `p->unk_E4 = p->unk_00;`
       alone gives a plain `lh`, which is right for unk_00 and wrong for the
       other two.
     * `p->unk_100 = s->unk_16 + ((s16)cv[0] >> 1);` -- an ARITHMETIC SHIFT
       (`sra 17` fused with the lhu), not `/ 2`.  `sv.vy = sv.vz / 2;` in the
       shadow pass IS a divide (`srl 31 / addu / sra 1`).
     * The two arms are chosen by `r->unk_14 & 0x100`.  Arm A (set) builds
       the quad with func_80065590 straight into the primitive and gates on
       `(u32)p->unk_C0 < 0x1E0` AND the four-corner test; arm B (clear) does
       a RotTransPers first, `p->unk_C0 -= 4`, the same part loop with
       func_800654B0 into 0x1F8000F0, and then -- when the emitted code is
       not semi-transparent -- a SECOND shadow quad with its own
       func_800BCB04 height probe, a -3 OT bias and the 0x7FC00000 /
       `*(s16 *)c << 16` colour-and-tpage words written as 32-bit stores.
     * Both arms end at the shared `func_80064D80(&m); func_80064CF0(&m);`
       restore (label .L800B5E64), so it is written out in both.
     * The part cursor is TWO pointers 4 bytes apart: `e` reads e[0] for the
       0x20 skip and the sign test, `c = e + 4` reads c[-2], c[-1], c[0..7].
       Both step by 0xC.

   WHERE THE REMAINING 262 ROWS ARE: spread evenly over every block (5 rows
   in 800B51xx, 14 in 52xx, 13 in 53xx, 13 in 54xx, 2 in 55xx, 6 in 56xx,
   19 in 57xx, 5 in 58xx, 5 in 59xx, 10 in 5Axx, 5 in 5Bxx, 12 in 5Cxx,
   17 in 5Dxx, 16 in 5Exx, plus the insert/delete rows), which is the
   signature of the one-register-out-of-step problem in (3) rather than of
   wrong code.  FIX `s`'s SPILL FIRST.
*/
s32 func_800B51C8(u8 *arg0, UnkVec *v, UnkSlot *s)
{
    MATRIX m;
    u16 cv[3];
    VECTOR sv;
    Unk453E0SP *p = (Unk453E0SP *)0x1F800000;
    void *md;
    void *ms;
    void *f0;
    void *f1;
    UnkDungeonSpotB *r;
    u8 *q;
    u8 *e;
    s32 i;
    s16 a;
    s16 b;
    Unk80083160 *gg;

    *(u8 **)((u8 *)p + 0x18) = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    p->unk_20 = D_80083160.unk_00 + 0xB0;
    p->unk_8C = 0;
    p->unk_84 = 0;
    p->unk_7C = 0;
    p->unk_74 = 0;
    cv[0] = D_80083160.unk_C4;
    cv[1] = D_80083160.unk_C6;
    cv[2] = D_80083160.unk_C8;
    func_80064AE0(&m);
    gg = &D_80083160;
    md = (void *)0x1F8000D0;
    ms = (void *)0x1F800050;
    f0 = (void *)0x1F800090;
    f1 = (void *)0x1F800094;
    for (i = 0; i < 0x20; i++)
    {
        if ((D_800E3648_s[i].unk_03 & 0x80) != 0 && (D_800E296C & 8) == 0)
        {
            continue;
        }
        if (D_800E3648_s[i].unk_01 == 0)
        {
            continue;
        }
        if (D_800E3648_s[i].unk_00 == 0)
        {
            continue;
        }
        r = &D_800E39C8[i];
        if ((r->unk_14 & 0x40) != 0)
        {
            func_800478B8((u8 *)r);
        }
        p->unk_00 = (r->unk_06 << 6) + 0x20;
        p->unk_02 = (r->unk_07 << 6) + 0x20;
        p->unk_04 = r->unk_12;
        r->unk_10 = p->unk_04;
        e = (u8 *)r->unk_08;
        if (e == NULL)
        {
            continue;
        }
        if ((r->unk_14 & 0x100) != 0)
        {
            p->unk_30.w = 0x2000;
            p->unk_34.w = 0x2000;
            p->unk_38.w = 0x2000;
            p->unk_100 = 0;
            p->unk_102 = 0;
            p->unk_104 = 0;
            p->unk_E4 = p->unk_00;
            p->unk_E8 = (s16) * (u16 *)&p->unk_02;
            p->unk_EC = (s16) * (u16 *)&p->unk_04;
            func_80065820(&p->unk_100, md);
            func_80064840(&m, md, ms);
            func_80064BC0(ms, &p->unk_30);
            func_80064D80(ms);
            func_80064CF0(ms);
            for (;;)
            {
                if ((e[0] & 0x20) == 0)
                {
                    p->unk_08.w = e[8];
                    p->unk_10.w = e[10];
                    if (p->unk_08.w + p->unk_10.w >= 0x100)
                    {
                        p->unk_10.w = p->unk_10.w - 1;
                    }
                    p->unk_0C.w = e[9];
                    p->unk_14.w = e[11];
                    if (p->unk_0C.w + p->unk_14.w >= 0x100)
                    {
                        p->unk_14.w = p->unk_14.w - 1;
                    }
                    a = (s8)e[2];
                    p->unk_80 = a;
                    a++;
                    a--;
                    p->unk_70 = a;
                    a = a + p->unk_10.h;
                    p->unk_88 = a;
                    p->unk_78 = a;
                    q = *(u8 **)((u8 *)p + 0x18);
                    *(u8 **)((u8 *)p + 0x18) = q + 0x28;
                    b = (s8)e[3];
                    p->unk_7A = b;
                    p->unk_72 = b;
                    b = b + p->unk_14.h;
                    p->unk_8A = b;
                    p->unk_82 = b;
                    p->unk_C0 = func_80065590(&p->unk_70, &p->unk_78, &p->unk_80, &p->unk_88,
                                              (void *)(q + 0x08), (void *)(q + 0x10),
                                              (void *)(q + 0x18), (void *)(q + 0x20),
                                              f0, f1);
                    if ((u32)p->unk_C0 < 0x1E0 &&
                        (((u16)(*(u16 *)(q + 0x08) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x0A) + 0x20) < 0x121) |
                         ((u16)(*(u16 *)(q + 0x10) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x12) + 0x20) < 0x121) |
                         ((u16)(*(u16 *)(q + 0x18) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x1A) + 0x20) < 0x121) |
                         ((u16)(*(u16 *)(q + 0x20) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x22) + 0x20) < 0x121)) != 0)
                    {
                        p->unk_10.w += p->unk_08.w;
                        p->unk_14.w += p->unk_0C.w;
                        p->unk_0C.w <<= 8;
                        p->unk_14.w <<= 8;
                        *(u16 *)(q + 0x0E) = *(u16 *)(e + 6);
                        *(u16 *)(q + 0x0C) = p->unk_0C.h + p->unk_08.h;
                        *(u16 *)(q + 0x14) = p->unk_0C.h + p->unk_10.h;
                        *(u16 *)(q + 0x16) = *(u16 *)(e + 4);
                        *(u16 *)(q + 0x1C) = p->unk_14.h + p->unk_08.h;
                        *(u16 *)(q + 0x24) = p->unk_14.h + p->unk_10.h;
                        *(u32 *)(q + 0x4) = *(u32 *)s->unk_0C;
                        func_800666F4(q);
                        q[0x7] |= 2;
                        func_8006658C(p->unk_20 + p->unk_C0 * 4, q);
                    }
                }
                if (*(s8 *)e < 0)
                {
                    break;
                }
                e += 0xC;
            }
            func_80064D80(&m);
            func_80064CF0(&m);
        }
        else
        {
            p->unk_E4 = 0;
            p->unk_E8 = 0;
            p->unk_EC = 0;
            p->unk_C0 = func_80065420(p, &p->unk_B8, &p->unk_90, &p->unk_94);
            D_8006CD30.unk_1C = p->unk_C0 * 4;
            p->unk_C0 = p->unk_C0 - 4;
            if ((u32)p->unk_C0 >= 0x1E0)
            {
                continue;
            }
            p->unk_B8 = p->unk_B8 - 0xA0;
            p->unk_BA = p->unk_BA - 0x78;
            p->unk_100 = s->unk_16 + ((s16)cv[0] >> 1);
            p->unk_104 = *(u16 *)((u8 *)gg + 0xB8) + (s->unk_1A - cv[1]);
            p->unk_102 = s->unk_18;
            func_80065820(&p->unk_100, md);
            func_80064840(&D_8006CD30, md, ms);
            func_80064D80(ms);
            func_80064CF0(ms);
            for (;;)
            {
                if ((e[0] & 0x20) == 0)
                {
                    p->unk_08.w = e[8];
                    p->unk_0C.w = e[9];
                    p->unk_10.w = e[10];
                    p->unk_14.w = e[11];
                    a = (s8)e[2];
                    p->unk_80 = a;
                    a++;
                    a--;
                    p->unk_70 = a;
                    a = a + p->unk_10.h;
                    p->unk_88 = a;
                    p->unk_78 = a;
                    b = (s8)e[3];
                    p->unk_7A = b;
                    p->unk_72 = b;
                    b = b + p->unk_14.h;
                    p->unk_8A = b;
                    p->unk_82 = b;
                    func_800654B0(&p->unk_70, &p->unk_78, &p->unk_80, &p->unk_88, &p->unk_F0,
                                  &p->unk_F4, &p->unk_F8, &p->unk_FC, f0,
                                  f1);
                    q = *(u8 **)((u8 *)p + 0x18);
                    *(u8 **)((u8 *)p + 0x18) = q + 0x28;
                    *(u16 *)(q + 0x08) = p->unk_F0 + p->unk_B8;
                    *(u16 *)(q + 0x0A) = p->unk_F2 + p->unk_BA;
                    *(u16 *)(q + 0x10) = p->unk_F4 + p->unk_B8;
                    *(u16 *)(q + 0x12) = p->unk_F6 + p->unk_BA;
                    *(u16 *)(q + 0x18) = p->unk_F8 + p->unk_B8;
                    *(u16 *)(q + 0x1A) = p->unk_FA + p->unk_BA;
                    *(u16 *)(q + 0x20) = p->unk_FC + p->unk_B8;
                    *(u16 *)(q + 0x22) = p->unk_FE + p->unk_BA;
                    if ((((u16)(*(u16 *)(q + 0x08) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x0A) + 0x20) < 0x121) |
                         ((u16)(*(u16 *)(q + 0x10) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x12) + 0x20) < 0x121) |
                         ((u16)(*(u16 *)(q + 0x18) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x1A) + 0x20) < 0x121) |
                         ((u16)(*(u16 *)(q + 0x20) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x22) + 0x20) < 0x121)) != 0)
                    {
                        q[0x3] = 9;
                        p->unk_10.w += p->unk_08.w - 1;
                        if ((p->unk_10.w & 0x100) != 0)
                        {
                            p->unk_10.w -= 1;
                        }
                        p->unk_14.w += p->unk_0C.w - 1;
                        if ((p->unk_14.w & 0x100) != 0)
                        {
                            p->unk_14.w -= 1;
                        }
                        p->unk_0C.w <<= 8;
                        p->unk_14.w <<= 8;
                        *(u16 *)(q + 0x0C) = p->unk_0C.h + p->unk_08.h;
                        *(u16 *)(q + 0x0E) = *(u16 *)(e + 6);
                        *(u16 *)(q + 0x14) = p->unk_0C.h + p->unk_10.h;
                        *(u16 *)(q + 0x16) = *(u16 *)(e + 4);
                        *(u16 *)(q + 0x1C) = p->unk_14.h + p->unk_08.h;
                        *(u16 *)(q + 0x24) = p->unk_14.h + p->unk_10.h;
                        if (*(s16 *)(q + 0x8) > *(s16 *)(q + 0x20))
                        {
                            q[0x14] -= 1;
                            q[0x24] -= 1;
                        }
                        if (*(s16 *)(q + 0xA) > *(s16 *)(q + 0x22))
                        {
                            q[0x1D] -= 1;
                            q[0x25] -= 1;
                        }
                        *(u32 *)(q + 0x4) = *(u32 *)s->unk_0C;
                        q[0x7] = 0x2C;
                        if ((D_800E296C & 8) != 0 && (D_800E3648_s[i].unk_03 & 0x80) != 0)
                        {
                            q[0x7] = 0x2E;
                        }
                        func_8006658C(p->unk_20 + p->unk_C0 * 4, q);
                        if ((q[0x7] & 2) == 0)
                        {
                            func_80064D80(&m);
                            func_80064CF0(&m);
                            sv.vz = (s16) * (u16 *)&p->unk_04 - 4;
                            p->unk_04 = func_800BCB04(*(u16 *)&p->unk_00, *(u16 *)&p->unk_02,
                                                      sv.vz << 16 >> 16);
                            p->unk_C0 = func_80065420(p, &p->unk_B8, f0, f1);
                            D_8006CD30.unk_1C = p->unk_C0 * 4;
                            p->unk_C0 = p->unk_C0 - 3;
                            p->unk_B8 = p->unk_B8 - 0xA0;
                            p->unk_BA = p->unk_BA - 0x78;
                            sv.vz = ((sv.vz - (s16) * (u16 *)&p->unk_04) << 5) + 0x1000;
                            if (sv.vz < 0)
                            {
                                sv.vz = 0;
                            }
                            p->unk_102 = 0;
                            sv.vx = sv.vz;
                            sv.vy = sv.vz / 2;
                            p->unk_100 = -(s16)cv[0];
                            p->unk_104 = *(u16 *)((u8 *)gg + 0xB8) - cv[1];
                            func_80065820(&p->unk_100, md);
                            func_80064840(&D_8006CD30, md, ms);
                            func_80064BC0(ms, &sv);
                            func_80064D80(ms);
                            func_80064CF0(ms);
                            q = *(u8 **)((u8 *)p + 0x18);
                            *(u8 **)((u8 *)p + 0x18) = q + 0x28;
                            p->unk_70 = p->unk_70 + 6;
                            p->unk_78 = p->unk_78 + 6;
                            func_800654B0(&p->unk_70, &p->unk_78, &p->unk_80, &p->unk_88,
                                          &p->unk_F0, &p->unk_F4, &p->unk_F8, &p->unk_FC,
                                          f0, f1);
                            *(u16 *)(q + 0x08) = p->unk_F0 + p->unk_B8;
                            *(u16 *)(q + 0x0A) = p->unk_F2 + p->unk_BA;
                            *(u16 *)(q + 0x10) = p->unk_F4 + p->unk_B8;
                            *(u16 *)(q + 0x12) = p->unk_F6 + p->unk_BA;
                            *(u16 *)(q + 0x18) = p->unk_F8 + p->unk_B8;
                            *(u16 *)(q + 0x1A) = p->unk_FA + p->unk_BA;
                            *(u16 *)(q + 0x20) = p->unk_FC + p->unk_B8;
                            *(u16 *)(q + 0x22) = p->unk_FE + p->unk_BA;
                            q[0x3] = 9;
                            *(u32 *)(q + 0x0C) = p->unk_0C.w + p->unk_08.w + 0x7FC00000;
                            *(u32 *)(q + 0x14) = p->unk_0C.w + p->unk_10.w + (*(s16 *)(e + 4) << 16);
                            *(u16 *)(q + 0x1C) = p->unk_14.h + p->unk_08.h;
                            *(u16 *)(q + 0x24) = p->unk_14.h + p->unk_10.h;
                            if (*(s16 *)(q + 0x8) > *(s16 *)(q + 0x20))
                            {
                                q[0x14] -= 1;
                                q[0x24] -= 1;
                            }
                            if (*(s16 *)(q + 0xA) > *(s16 *)(q + 0x22))
                            {
                                q[0x1D] -= 1;
                                q[0x25] -= 1;
                            }
                            *(u32 *)(q + 0x4) = *(u32 *)s->unk_0C;
                            q[0x7] = 0x2C;
                            func_8006658C(p->unk_20 + p->unk_C0 * 4, q);
                        }
                    }
                }
                if (*(s8 *)e < 0)
                {
                    break;
                }
                e += 0xC;
            }
            func_80064D80(&m);
            func_80064CF0(&m);
        }
    }
    *(u8 **)(gg->unk_00 + 0x8D0) = *(u8 **)((u8 *)p + 0x18);
    return 0;
}
