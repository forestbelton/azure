/* func_8008E700 (ovl_dungeon) -- 208 of 208 words, 6 ALIGNED DIFFERENCES.
   Agents j42 and j54, 2026-11-16; REWRITTEN BY j84, 2026-11-16, with a new
   basin and a PROOF that closes the park's old open question.

   READ THIS FIRST.  The live body is the lowest-scoring one (6 rows, every
   register right), but **it provably cannot reach 0**, and neither can any
   other body that keeps `*(u32 *)(d + 0x1C) &= ~0x100000;` as ONE
   statement -- the dependence proof is two sections down.  The body worth
   spending a permuter run or another agent on is **ALTERNATIVE 1** under
   the `#if 0`: 13 rows, but the target's exact memory order, and the only
   remaining fault is a local-alloc register pair.

   ================================================================= THE RESIDUE

   The live body below is the `P A Q` order -- `s = *D_80081484;` then the
   +0x1C clear then `*D_80081484 = NULL;`.  Its ENTIRE residue is one
   three-instruction group five slots too late:

     cand 146 li   a2,5           target 141 addiu $a2, $zero, 0x5
     cand 147 sw   zero,0(a1)     target 142 sw  $zero, %lo(D_80081484)($a1)
     cand 148 lui  a1,0x0         target 143 lui $a1, %hi(D_800DCFD0)

   Words 0..140 and 149..207 are BYTE-EXACT and EVERY REGISTER IS CORRECT --
   `%hi(D_80081484)` is $a1 and the 0xFFEFFFFF mask is $v1, both as the
   target has them.  aligndiff prints exactly six rows (three `-`, three `+`)
   and nothing else.  That is a strictly better residue than the body this
   park carried before (j42's `P Q A`, also 6 rows, but with TWO WRONG
   REGISTERS and the misplacement spread over words 136..142).

   **j42's park claimed `PAQXDYVC` was "6 and byte-identical" to `PQAXDYVC`.
   IT IS NOT.**  The two are different instruction streams that happen to
   score the same; the claim is what kept this basin unexplored.  Re-measure
   a "byte-identical" claim before you believe it.

   ============================ WHY NO SINGLE-STATEMENT ORDER CAN EVER MATCH

   This is now PROVED, and it retires the park's old "SO THE OPEN QUESTION IS
   NARROW" paragraph.  Name the window's insns by their final slots:

       H = lw  $v0, 0x1C($s0)                  (the +0x1C read)
       I = sw  $v0, 0x1C($s0)                  (the +0x1C write-back)
       E = lw  $t0, %lo(D_80081484)($a1)       (`s = *D_80081484`)
       G = sw  $zero, %lo(D_80081484)($a1)     (`*D_80081484 = NULL`)

   The target emits H(139) ... G(142) ... I(147): **the +0x1C READ is before
   the D_80081484 STORE and the +0x1C WRITE is after it.**

   `*(u32 *)(d + 0x1C)` is `(mem (plus (reg) 28))` -- non-struct, varying --
   and `*(void **)D_80081484` is `(mem (lo_sum (reg) (symbol_ref)))`, which
   `rtx_varies_p` (rtlanal.c, case LO_SUM) calls NON-varying and which is
   non-struct.  Neither side gets the `MEM_IN_STRUCT_P && rtx_addr_varies_p`
   escape in `true_dependence`/`anti_dependence`/`output_dependence`
   (sched.c:846-911), and `memrefs_conflict_p` cannot prove two non-constant
   addresses apart, so BOTH edges exist:

     * A after Q  (`P Q A`) -> true_dependence pins H AFTER G.  Measured: H
       lands at 142 and G at 141.  H can never reach 139.
     * A before Q (`P A Q`) -> output_dependence pins I BEFORE G.  Measured:
       I lands at 144 and G at 147.  G can never reach 142.

   A single `*(u32 *)(d + 0x1C) &= ~0x100000;` statement puts H and I on the
   same side of Q, so one of the two edges always points the wrong way.
   **THE TARGET'S SOURCE MUST SPLIT THE READ-MODIFY-WRITE ACROSS THE
   `*D_80081484 = NULL;` STORE.**  That is not a preference; the dependence
   graph forbids everything else.  So:

       s = *(void **)D_80081484;
       <temp> = *(u32 *)(d + 0x1C);              -- H, before Q
       *(void **)D_80081484 = NULL;              -- G
       *(u32 *)(d + 0x1C) = <temp> & ~0x100000;  -- X and I, after G

   and the dependence set of THAT order is exactly the target's: H before G
   (anti), I after G (output), nothing else.  **The whole remaining problem
   is that the extra pseudo costs the register pair.**

   THE SPLIT, MEASURED (j84).  `L` = the read into the temp, `M` = the
   `and` + write-back, `N` = read-and-`and` into the temp, `O` = the bare
   write-back; letters otherwise as below.  All 208 words:

     PLQMXGYWVE 15   PLQXMGYWVE 15   PLQXGMYWVE 15   PLQXGYMWVE 18
     PLQXGYWMVE 23   PLQXGYWVME 13   PLQXGYWVEM 24
     PNQOXGYWVE 15   PNQXOGYWVE 15   PNQXGOYWVE 15   NPQOXGYWVE 13
     lPQMXGYWVE 15   PQlMXGYWVE 15   PlQMXGYWVE 15
     LPQmXGYWVE 19   lPQmXGYWVE 22
   temp reused instead of a fresh local (so the temp has TWO sets and loses
   the `birthing_insn_p` boost):
     r:  LPQXMGYWVE 14  PLQXMGYWVE 14  PLMQXGYWVE 13  LPQMXGYWVE 14
         PQLXMGYWVE 16  LPQXGYMWVE 20  PLQMXGYWVE 14
     v:  PLQMXGYWVE 17  PLQXMGYWVE 17
     g2 (as `u8 *`, with casts): PLQMXGYWVE 18
   temp TYPE makes no difference at all: `u32 m`, `s32 m`, `int m` and
   `unsigned long m` are all 15 on PLQMXGYWVE.
   Splitting the `and` off the write-back as well, so the temp gets a SECOND
   set and loses the birthing boost -- `m &= ~0x100000;` as its own statement
   -- changes nothing: LPQNOXGYWVE, LNPQOXGYWVE, LPNQOXGYWVE and
   LPQNXOGYWVE are all 13, the same as LPQMXGYWVE, and LPQONXGYWVE (storing
   before the `and`, which is a different program) is 205 words.
   `m & 0xFFEFFFFF` written out instead of `m & ~0x100000`: no change.

   `PLQMXGYWVE` (15) is the closest in SHAPE -- it is the only basin that
   gets H(139), G(141), I(147) on the right sides of each other -- and its
   stream is

       A B C E H D G Y1 Y2 F X W I Z     vs target
       A B D C H E F  G  W  X Y1 Y2 I Z

   with the mask in $a1 (target $v1) and `%hi(D_80081484)` in $v1 (target
   $a1), i.e. the pair swapped.  **THAT SWAP, AND NOT THE ORDER, IS THE
   THING TO ATTACK NEXT.**  sched1 runs before both allocators, so the LUID
   order it emits is what local-alloc prices (see docs/matching-levers.md,
   "alias.c RESOLVES A PSEUDO THROUGH ITS REG_EQUIV ...").

   THE SWAP IS A `local_alloc` QUANTITY-ORDER QUESTION, AND THE NUMBERS ARE
   READABLE.  `local_alloc` sorts quantities by

       QTY_CMP_PRI(q) = floor_log2(n_refs) * n_refs * size / (death - birth)

   (local-alloc.c:1693) and hands each, in that order, the lowest-numbered
   free register -- MIPS defines no `REG_ALLOC_ORDER`, so the order is
   $zero, $at, $v0, $v1, $a0, ... .  From `PLQMXGYWVE`'s own `.lreg` (a
   standalone file of the unit's declaration block + this body, compiled with
   `tools/decomp/dump.sh`; it reproduces the residue exactly):

       Register 92  (the +0x1C value)        2 refs / 5 insns  -> $v0
       Register 182 (%hi(D_80081484))        3 refs / 10 insns -> $v1
       Register 186 (the 0xFFEFFFFF mask)    2 refs / 8 insns  -> $a1
       Register 88  (`s`)                                      -> $t0

   i.e. 12/10 beats 8/8, so the `%hi` is priced first and takes $v1.  The
   target's allocation is value $v0, **mask $v1, `%hi` $a1**, so the MASK has
   to be priced first, which needs `QTY_CMP_PRI(mask) > QTY_CMP_PRI(%hi)`:
   either more references on the mask, or a `%hi` live range more than 1.5x
   the mask's.  Measured attempts at both, all still 13-24: moving the NULL
   store later in the source (LPXQMGYWVE 13, LPXGQMYWVE 13, LPXGYQMWVE 18,
   LPXGYWQMVE 24 -- sched closes the gap again), and three spellings of the
   D_80081484 pair that might have split the `%hi` into two pseudos
   (`*(u32 *)D_80081484 = 0;` for the store, `(void *)*(u32 *)D_80081484`
   for the load, `*(u8 **)D_80081484 = NULL;`): **every one is bit-identical
   to the baseline** at 6 / 6 / 13 for PQA / PAQ / LPQM.  cse2 merges the
   `(high (symbol_ref))` whatever you write, so the 3-reference `%hi` cannot
   be broken up from the C.  Binding the mask to its own `u32 k = ~0x100000;`
   local to try to give its quantity another reference is also no change:
   KLPQMXGYWVE, LKPQMXGYWVE and LPQKMXGYWVE are all 13.

   ====================================================== THE MEM_IN_STRUCT ROUTE

   `((u32 *)d)[7] &= ~0x100000;` DOES set `MEM_IN_STRUCT_P` -- confirmed in
   the compiler source, expr.c:5343-5351, `TREE_CODE (TREE_OPERAND (exp, 0))
   == PLUS_EXPR`, which `((u32 *)d)[7]` has (c-typeck.c's `build_array_ref`
   on a POINTER returns `build_indirect_ref (build_binary_op (PLUS_EXPR,
   ...))`) and which `*(u32 *)(d + 0x1C)` does NOT (its operand is a
   NOP_EXPR).  So the array ref really does delete BOTH edges above.

   It does not help: with no edge the source order stops mattering and every
   placement collapses to the same bad schedule, with the mask in $a3 and
   `%hi(D_80081484)` in $v0:

     aPQXGYWVE 22  PaQXGYWVE 22  PQaXGYWVE 22  PQXaGYWVE 22  PQXGaYWVE 22
     PQXGYaWVE 23  PQXGYWaVE 19  PQXGYWVaE 19  PQXGYWVEa 19

   and -- this is the decisive part -- **with the +0x1C load still AFTER the
   D_80081484 store**, at 144 against 143.  Deleting the edge does not hoist
   the load; it only lets everything else float, because sched's own priority
   order wants that load late.  (A struct FIELD -- `d->flags &= ~0x100000` --
   would take the same expr.c:5748 path and get the same flag, so retyping
   `d` is not a way out either.)

   **AND THE REGISTERS SAY THE TARGET DOES NOT USE THE ESCAPE HERE.**  The
   landed sibling `func_8008E504` is the one the array-ref rule was written
   against: it clears two +0x1C flag words as `((u32 *)q)[7] &= ~0x80000;`
   and `((u32 *)d)[7] &= ~0x100000;` around the same D_80081484 pair, lands
   at 0 differing, and its two masks come out in **$a2 and $a3**
   (`li $6,-589824` / `li $7,-1114112`).  Every array-ref spelling of THIS
   function puts the mask in $a3 too.  `func_8008E700`'s target has it in
   **$v1**, which is what every CAST spelling here produces.  So the two
   functions really do disagree, the sibling's comment is right about itself
   and wrong as a general rule, and this function's +0x1C clear is a cast.
   Together with the proof above that leaves exactly one shape: a CAST
   read-modify-write SPLIT ACROSS the NULL store.

   For reference, the sibling's landed stream -- useful because it shows
   what the escape does when it IS the answer (the clear is written BEFORE
   the pair and its load lands between the two D_80081484 accesses, its
   store after them):

       lui $3,%hi(D_80081484) / lw $4,292($16) / lw $5,%lo(D_80081484)($3)
       lw $2,28($4) / ori $7,$7,0xffff / sw $0,%lo(D_80081484)($3)
       and $2,$2,$6 / sw $2,28($4)

   MIXED FORMS, which put the escape on ONE HALF of the clear only -- a
   mechanism not in docs/matching-levers.md and worth adding whatever
   happens here, because it is the only way to break one of the two edges
   without the other.  `S` = `((u32 *)d)[7] = *(u32 *)(d + 0x1C) &
   ~0x100000;` (cast read keeps its edge, struct write loses its) and `T` =
   `*(u32 *)(d + 0x1C) = ((u32 *)d)[7] & ~0x100000;` (the reverse):

     SPQXGYWVE 19   PSQXGYWVE 21   PSQGXYWVE 21
     PQTXGYWVE 15   PQXTGYWVE 15   PQXGTYWVE 15

   `SPQXGYWVE` is the only no-temp body with the target's SHAPE -- H(141)
   before E(142), G(144) after both, I(148) after G -- so the mechanism
   works exactly as the dependence functions predict.  Its 19 rows are the
   interleaving of F, W, Y1, Y2, Z plus the mask in $a3.

   ALL 180 legal orders of this family (`SPQ` and `PSQ` + the 30 legal
   permutations of XGYWVE, and `T` in each of four slots after `PQ`), j84:
   **best 14**, all of them `SPQ` with the hoisted index `V` moved up among
   the t2/g statements --
     SPQGVXYWE SPQGVYXWE SPQGXVYWE SPQVGXYWE SPQVGYXWE SPQVXGYWE
     SPQXGVYWE SPQXVGYWE   all 14
     PQGTXYWVE PQGTYXWVE PQGXTYWVE PQTGXYWVE   all 15
   -- so the mixed form is worse than both the live body (6) and the split
   (13), and the mask stays in $a3 throughout.

   The comma trick that would split the RMW without a temp does not work:
   `*(u32 *)(d + 0x1C) &= (*(void **)D_80081484 = NULL, ~0x100000);` and
   `*(u32 *)(d+0x1C) = *(u32 *)(d+0x1C) & (*(void**)D_80081484 = NULL,
   ~0x100000);` are both 6 and byte-identical to j42's `P Q A` body --
   expand_expr evaluates the side-effecting operand of a commutative binop
   FIRST, so the NULL store still precedes the read.

   ================================================== SWEEPS THAT FOUND NOTHING

   Letters: P=`s = *D_80081484;`  Q=`*D_80081484 = NULL;`
            A=`*(u32 *)(d + 0x1C) &= ~0x100000;`  a= the same as `((u32*)d)[7]`
            X=`t2 = D_800DCFD0;`  G=`g = (u8 *)&D_80083460;`
            Y=`*(u16 *)(g + 0xA) += 1;`  W=`*(u8 **)(c + 0x2C) = t2;`
            V= the hoisted index `v = ...;`  E=`D_800E3540 = s;`
   (j42's shorter alphabet folded Y and W into one letter; the orders below
   are not interchangeable with its.)

   ALL 180 legal orders OF THE SPLIT BASIN -- `L` before or after `P`, `M` in
   each of the three slots after `Q`, times the 30 legal permutations of
   XGYWVE, with a fresh `u32 m` (j84): **best 13**, reached only by
     LPQGMXYWVE LPQGMYXWVE LPQGXMYWVE LPQMGXYWVE LPQMGYXWVE LPQMXGYWVE
     LPQXGMYWVE LPQXMGYWVE
   then 14 (LPQGMVXYWE, LPQGMVYXWE), and up from there.  So `L` must precede
   `P` -- the +0x1C read before the D_80081484 read -- and every 13 has the
   same stream and the same swapped register pair.

   ALL 180 legal orders of the ARRAY-REF clear with the index INLINE in the
   call instead of hoisted to `v` (j84): **best 17**, ten orders, all with
   the clear last or next to last.  The `v` hoist is still worth 11 rows
   under the escape, so it is not an artefact of the cast form.

   ALL 180 legal orders of `PAQ` + a permutation of XGYWVE (G before Y, X
   before W), j84: best 6, and only three orders reach it --
     PAQXGYWVE 6  PAQGXYWVE 6  PAQGYXWVE 6
     PAQXGYVWE 8  PAQGXYVWE 8  PAQGYVXWE 8  PAQGYXVWE 8
     then 15, 19, 22, 23, 24, 29, 32, 41 (worst).
   All three 6-row orders produce the SAME stream as the body below.

   j42/j54's earlier sweeps, kept because they are still the record for the
   `P Q A` and `A first` basins (their alphabet):
     tail statement order, all 60 legal permutations with the index INLINE:
       best 17 (AEBDC, BDECA), worst 38, median ~25.
     with the index hoisted to `v` and B split into P/Q:
       PQAXDYVC 6, PAQXDYVC 6, PQADXYVC 6, PQXADYVC 6, PAXQDYVC 6,
       APQXDYVC 8, PQAXDVYC 8, PQAVXDYC 12, PQAXYDVC 16, PQVAXDYC 27,
       VPQAXDYC 43.
     `A` FIRST: AXPQDYVC, APXQDYVC, ADPQXYVC, APQDXYVC all 8 and
       byte-identical; APQXDVYC 10, AVPQXDYC 13, APQXDYCV 16, APQXYDVC 21,
       AYPQXDVC 210 words / 23.
     splits with a fresh `m`: LPQXMDYVC 13, LPQMXDYVC 13, LPQXDYVMC 13,
       LPQXMVDYC 14, LPQXMDVYC 15, PLQXMDYVC 15, LPQXMYDVC 17,
       LPQXDMYVC 18, LPQDXMYVC 18, LPQXMDYCV 20, PLQXDYMVC 23,
       LPQXDYMCV 27, LPQYXMDVC 210 words / 28.
     splitting `s = *(void**)D_80081484` from the NULL store: no change.
     a `void **q` local for the D_80081484 base: no change (same md5).
     `u32 s` instead of `void *s`, `u8 *s`, `void **s`: no change.
     declaration ORDER of the seven locals, nine permutations: no change --
       gcc 2.7.2 numbers pseudos by first use, not by declaration.
     naming D_800DCFD0 twice instead of binding `t2`: no change.
     re-reading `(*(u8 **)(c + 0x2C))[...]` in the tail call: no change.
     a `u32 m = ~0x100000` local for the mask: no change.
     `&= 0xFFEFFFFF` written out instead of `&= ~0x100000`: no change.
     killing the `%hi(D_80081484)` pseudo's birthing boost by sharing a
       local: `t` for `t2` 31, `r` for `v` 14, `g` for `g2` 25.
     naming `&D_80083460` inline instead of binding `g` -- `*(u16 *)((u8 *)
       &D_80083460 + 0xA) += 1;` -- is **207 words**: PQA 12, PAQ 13,
       LPQM 17.  The `g` local is load-bearing for the word count, not just
       the order (j84).
     `volatile` on the read-read pair to pin source order
       (`*(volatile u32 *)(d + 0x1C)` and `*(void *volatile *)D_80081484`,
       which `read_dependence` honours only when BOTH are volatile): no
       effect at all -- lpQMXGYWVE, lPQMXGYWVE, LpQMXGYWVE and lpQXMGYWVE
       are all 13, the same as the non-volatile LPQMXGYWVE.

   ======================================================== WHAT GOT IT THIS FAR

   * `*(u16 *)0x80013714`, NOT the declared `D_80013714` (include/structs.h
     :2129).  RAW-VERIFIED (j84): `ovlverify --raw` shows words 100/101 as
     `3C028001` / `94423714` on BOTH sides, so the constant-address spelling
     is byte-identical to the target's relocated `%hi`/`%lo` and nothing is
     being hidden by the verifier's masking.  The declared symbol lets reorg
     lift its `lui` into the delay
     slot of the `(d+0x1C & 0x300000) == 0x100000` test, where the target
     leaves a `nop`: 207 of 208 words.  The constant address is a
     plus(REG, CONST_INT) split, the `lui` is not a separate movable insn,
     and the function reaches 208/208.  Six other spellings of that test --
     nested ifs, `!(x & 1)`, `x % 2`, `(u32)x & 1`, `!= 1`, and a `- 0x100000
     == 0` form -- are all byte-identical to each other at 207 words, so it
     is the SYMBOL spelling and nothing else.  (Worth 1 word + ~10 diffs.)
   * HOISTING THE SKIN INDEX INTO A LOCAL `v`, placed between the +0x2C store
     and `D_800E3540 = s`.  Inline in the call argument the function is 15-16
     aligned; as `v` computed at that point it is 6.  Every other placement of
     the same hoist is worse: before the D_80081484 pair 43, after `t2` 12,
     after the g increment 8, last 15.
   * THREE SEPARATE POINTER LOCALS (`t`, `t2`, `g`, `g2`) -- one per symbol
     materialisation site, per the "N materialisations mean N locals" rule.
   * `*(u32 *)(d + 0x1C) &= ~0x100000` as a CAST, not `((u32 *)d)[7]` -- see
     the MEM_IN_STRUCT section above for WHY, which j42's note did not have.
   * CASE 0 STORES THE SKIN TABLE IN BOTH ARMS, and the ternary
     `*(u8 **)(c+0x2C) = cond ? D_800DD0C8 : D_800DD048;` is wrong: the
     ternary puts the store in the merge block where cse then forwards it into
     `(*(u8 **)(c + 0x2C))[...]`, costing the target's `lw $v1, 0x2C($a0)`
     reload and two words.  Written as two stores in two arms, cross-jumping
     unifies the `sw` (which is where the target's `j .L8008E798` + delay-slot
     `addiu %lo` comes from) and the reload survives.
   * CASE 1 USES THE LOCAL-BOUND FORM `t = D_800DD060; ...; t[...]`, case 0
     the RE-READ form.  Both are in the listing and they are not interchangeable.

   WHAT THE CANDIDATE HOLDS FIXED (so the next reader knows the frame):
   the signature `(u8 *p, UnkVec *b, u8 *c, u8 *d)`; `func_8009AF18` riding its
   IMPLICIT declaration (it is defined below at :10174 as
   `s16 func_8009AF18(u16, UnkVec *, u8 *, s16)` -- a real prototype makes the
   first argument `lhu` where the target has `lh`, and makes the result
   pre-sign-extended where the target has `addu $a2, $v0, $zero` + `sll 16`);
   `n = ++*(u16 *)(p + 0x96)` as an s16 local; and the four +0x124 reads being
   written out inline rather than cached, which is what the target's four
   `lw 0x124($s1)` are.

   NEW DECLARATIONS this body needs at the top of csrc/ovl_dungeon.c:
     extern u8 D_800DD048[];
     extern u8 D_800DD060[];
     extern u8 D_800DCFD0[];
   (D_800DD0C8, D_80081484, D_800E3540, D_80083228, D_80083460, func_80048A44,
   func_8009F644, func_800A56E0, func_80099F04, func_80099F70, func_8008ACDC
   and func_8008EAC8 are all already declared.)

   Measured with
     python3 tools/decomp/aligndiff.py <unit copy> func_8008E700
     python3 tools/decomp/ovlverify.py <unit copy> func_8008E700
   -> 208 of 208 words, 6 aligned.

   PERMUTED 2026-11-16, 25 minutes, 6 jobs, ON THE OLD `P Q A` BODY: NO
   IMPROVEMENT.  It reported 280 -> 145 on its own metric and re-measured at
   208 words / 6 aligned rows, identical to that body, with the same six rows
   in the same place.  Its one idea was a pointer copy (`g2 = c;`).  See
   docs/matching-levers.md, "THE PERMUTER'S SCORE IS NOT THE ALIGNED
   DIFFERENCE COUNT".  **A permuter run on the `P Q A` body is spent budget,
   AND SO IS ONE ON THE BODY BELOW** -- read the proof above: anything that
   keeps the +0x1C clear as ONE statement cannot be the answer, whatever
   score it reports.  The only body worth a run is ALTERNATIVE 1
   (`LPQMXGYWVE`, 13 rows, the target's memory order, one register pair
   wrong), which has never been permuted, and the run is only useful if the
   permuter is allowed to keep the split.
 */

void func_8008E700(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *g2;
    u8 *t;
    u8 *t2;
    void *s;
    s16 n;
    s32 r;
    s32 v;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            b->unk_14 = -0x160000;
            if ((*(u32 *)(d + 0x1C) & 0x100000) != 0)
            {
                *(u8 **)(c + 0x2C) = D_800DD0C8;
            }
            else
            {
                *(u8 **)(c + 0x2C) = D_800DD048;
            }
            func_80048A44(
                c, (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        break;
    case 1:
        n = ++*(u16 *)(p + 0x96);
        if ((*(u16 *)(p + 0xA2) & 0x10) != 0 && n >= 4)
        {
            b->unk_14 = 0;
            if ((*(u32 *)(d + 0x1C) & 0x100000) != 0)
            {
                *(u32 *)(p + 0x8C) = (u32)func_8008EAC8;
            }
            else
            {
                t = D_800DD060;
                *(u8 **)(c + 0x2C) = t;
                func_80048A44(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            }
            p[0x9B]++;
        }
        else if ((*(u32 *)(d + 0x1C) & 0x300000) == 0x100000 && (*(u16 *)0x80013714 & 1) == 0)
        {
            r = func_8009AF18(*(s16 *)(d + 0x2A), b, c, 8);
            if ((s16)r != 0)
            {
                ((u32 *)*(u8 **)(p + 0x124))[7] |= 0x200000;
                ((u32 *)d)[7] |= 0x200000;
                *(s16 *)(*(u8 **)(p + 0x124) + 0x8A) = r;
                (*(u8 **)(p + 0x124))[0x84] = 0x78;
                (*(u8 **)(p + 0x124))[0x85] = 2;
                func_8009F644(d, 0x30, *(s16 *)(p + 0x96), 0);
                s = *(void **)D_80081484;
                *(u32 *)(d + 0x1C) &= ~0x100000;
                *(void **)D_80081484 = NULL;
                t2 = D_800DCFD0;
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) += 1;
                *(u8 **)(c + 0x2C) = t2;
                v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
                D_800E3540 = s;
                func_80048A44(c, t2[v], 5, 1);
                func_800A56E0(0x512);
            }
        }
        break;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            g2 = (u8 *)&D_80083460;
            if (*(s16 *)(g2 + 0xA) == 0)
            {
                if ((*(u32 *)(d + 0x1C) & 0x200000) != 0)
                {
                    *(u32 *)(d + 0x1C) &= 0xFFDFFFFF;
                    *(u16 *)(g2 + 0x2) |= 0x412;
                    func_80099F70(*(void **)(d + 0x5C));
                    func_80099F04(*(void **)(d + 0x5C));
                }
                *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
            }
        }
        break;
    }
}

#if 0
/* ALTERNATIVE 1 (j84, 2026-11-16): THE SPLIT -- `LPQMXGYWVE`, 208 of 208
   words, 13 aligned differences, the best of the 180-order sweep of this
   basin.  Worse by the count than the live body, but it is the ONLY basin
   whose dependence graph permits the target's order at all (see the proof in
   the header), and it realises it: the +0x1C read at 138, the D_80081484
   store at 141, the +0x1C write-back at 147, each on the correct side of the
   others, i.e. the SAME MEMORY ORDER AS THE TARGET.  Its stream is

       A B C H E D G  Y1 Y2 F X W  I Z    this body
       A B D C H E F  G  W  X Y1 Y2 I Z   target

   so what is left is the register pair -- the mask is $a1 where the target
   has $v1, and `%hi(D_80081484)` is $v1 where the target has $a1 -- and the
   interleaving of the four non-memory groups D, F, W and Y1/Y2 around it.
   Needs `u32 m;` added to the locals.  **THIS IS THE BASIN TO WORK, NOT THE
   6-ROW ONE**, and it has never been permuted. */
                func_8009F644(d, 0x30, *(s16 *)(p + 0x96), 0);
                m = *(u32 *)(d + 0x1C);
                s = *(void **)D_80081484;
                *(void **)D_80081484 = NULL;
                *(u32 *)(d + 0x1C) = m & ~0x100000;
                t2 = D_800DCFD0;
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) += 1;
                *(u8 **)(c + 0x2C) = t2;
                v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
                D_800E3540 = s;
                func_80048A44(c, t2[v], 5, 1);
                func_800A56E0(0x512);

/* ALTERNATIVE 2 (j42/j54's original park body): `P Q A`, 208 of 208 words,
   6 aligned differences, but TWO WRONG REGISTERS ($v0 for %hi(D_80081484)
   where the target has $a1) and the misplacement spread over words
   136..142.  This is the body the permuter already failed on.  Kept only so
   the permuter's result stays attached to the body it was run against. */
                func_8009F644(d, 0x30, *(s16 *)(p + 0x96), 0);
                s = *(void **)D_80081484;
                *(void **)D_80081484 = NULL;
                *(u32 *)(d + 0x1C) &= ~0x100000;
                t2 = D_800DCFD0;
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) += 1;
                *(u8 **)(c + 0x2C) = t2;
                v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
                D_800E3540 = s;
                func_80048A44(c, t2[v], 5, 1);
                func_800A56E0(0x512);

/* ALTERNATIVE 3 (j54): `A` FIRST -- 208 of 208 words, 8 aligned differences,
   registers correct, words 149..207 byte-exact.  Same wall as the live body:
   the +0x1C write-back is pinned before the D_80081484 store. */
                func_8009F644(d, 0x30, *(s16 *)(p + 0x96), 0);
                *(u32 *)(d + 0x1C) &= ~0x100000;
                s = *(void **)D_80081484;
                *(void **)D_80081484 = NULL;
                t2 = D_800DCFD0;
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) += 1;
                *(u8 **)(c + 0x2C) = t2;
                v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
                D_800E3540 = s;
                func_80048A44(c, t2[v], 5, 1);
                func_800A56E0(0x512);
#endif
