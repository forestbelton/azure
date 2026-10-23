# How this compiler behaves

The peculiarities of the PSY-Q 4.1 `cc1psx` (`cygnus-2.7.2-970404`) as this
project has measured them. **Read this before writing a function.** Every
numbered entry was measured: the cost in words is what reverting that shape did
to a function that otherwise matched, and the "dead ends" at the bottom are
things not to retry.

Keep it up to date. Anything learned in a session that is not written here
gets re-derived from scratch next time, and that has been the single biggest
waste of effort on this project.

Companion documents:

* `docs/where-we-are.md` -- state, workflow, the tools, and the queue.
* `docs/compiler-version.md` -- which binary this is, the evidence for it, and
  how to read `cc1psx`'s own RTL dumps.
* `docs/addressing-and-translation-units.md` -- the long form of section 6.
* `docs/open-matching-problems.md` -- the parked functions and the blocked
  groups, each with its own analysis.
* `docs/near-misses/` -- per-function source plus everything ruled out for it.
* `docs/m2c-pitfalls.md` -- where the decompiler's inferences are wrong.

## The pass order, and what each pass does to you

gcc 2.7.2 runs these in order. Knowing which pass owns a difference tells you
whether source can reach it at all.

| pass | what it decides | how it bites |
|------|-----------------|--------------|
| `jump` | block layout, which arm goes out of line, label deletion | an `if`/`else`'s arms are not interchangeable; see section 8 |
| `cse` | substitutes the cheapest equivalent per operand, then constant-folds | `rtx_cost` of a `CONST_INT` is ~0, so a known constant always beats an insn -- that is why `0x10 * 2` becomes `li 32`. Cost decides *which* equivalent wins; what decides whether cse knows the constant at all is its **run boundary**, below |
| | the extent of one run | a run is a maximal **linear** insn range that stops only at a `CODE_LABEL` with `LABEL_NUSES != 1`. It does **not** stop at a conditional jump, a barrier, or a single-use label -- so an `if` body inherits the test's table whether it is the fall-through or the branch target |
| | memory equivalences | invalidation is **coarse**: a store through a register base kills entries at other offsets off the same base, which is why the target reloads a neighbouring field |
| `loop` | invariant motion | hoists a constant's set into the preheader, where `combine` can no longer fold it into an immediate (section 3) |
| | strength reduction | builds the giv list by **prepending**, so the *last* access in the body becomes `combine_givs`' representative (section 9) |
| `flow` | `reg_n_refs`, `reg_live_length` | refs are weighted by `loop_depth`, so a loop note reweights the whole function (section 2). Both are recomputed after `sched1`. |
| `combine` | substitutes an insn into its single user | blocked by a **second use**, by an intervening memory store (`use_crosses_set_p` / `mem_last_set`), and by basic-block boundaries. This is the most reachable pass from C. |
| | proves bits via `nonzero_bits` | **not** reachable from C -- see the dead ends |
| `sched1` | insn order **within one basic block** | ties break by `LUID`, i.e. source order; alias calls go through `MEM_IN_STRUCT_P` (section 5); a hazard heuristic reorders stores and cannot be steered |
| `local-alloc` | registers for block-local quantities, **before** `global-alloc` | a value live in one block only is out of reach; it also takes the register `global-alloc` would have wanted |
| `global-alloc` | everything else, by `floor_log2(refs)*refs/live_length` | section 3; `find_reg` also avoids registers that *conflicting* allocnos prefer, and a call's argument preferences propagate |
| `sched2` | insn order again, after allocation | sees the code **before** cross-jumping, so a duplicate put in both arms is still two insns here |
| `jump2` | cross-jumping | merges identical tails, so duplicating a store into two arms costs nothing (section 1). **It runs after both schedulers and after allocation** -- measured, not assumed: the twin branch and its label are present in the `.sched2` dump and gone in `.jump2` |
| `shorten`, `reorg` | delay slots | `reorg` can steal the insn at a branch target and redirect the branch past it, which looks like a duplicated instruction in the listing but comes from one statement. It is the only pass that sees the merged block |

## 1. Block membership beats scheduling

sched1 only moves insns **inside** a basic block. So when an insn is in the
wrong place and no amount of statement reordering fixes it, the question is
usually which basic block it is in, and the lever is the source's control flow,
not its order.

Three instances:

* `x = abs(x);` instead of `if (x < 0) { x = -x; }`. The MIPS `abssi2` pattern
  is a **single insn** whose output template contains its own `bgez`/`subu`, so
  it creates no block boundary and the branch delay slot inside it stays a
  `nop`. With the abs as one insn, a whole arm becomes one block and sched1 can
  then hoist a reload up to the store it follows. `int abs(int)` is declared in
  `structs.h` for exactly this; func_80046C20 and func_8003F6F4 use it.
* **A small byte-aligned struct assignment is a single insn too.** `movstrsi`
  for a small constant size does not expand to loads and stores; it stays as
  one `movstrsi_internal` through combine, sched1, global-alloc and sched2, and
  the `lb`/`lb`/`sb`/`sb` appear only from its output template at final. So
  nothing can be scheduled between those bytes and the insn that follows them,
  and combine never sees the loads as separate insns -- which is why they are
  `lb` and not `lbu`. That is the whole of what func_80042560 was parked on:
  copying the two members one at a time costs it 44 words, because sched1 then
  steals the second `sb` for a branch delay slot. **Before concluding that a
  pass cannot be steered, check that the instructions in question are separate
  insns at all** -- that function was written off on a correct argument about
  `memrefs_conflict_p` aimed at a dependence that never needed to exist.
* Duplicating a store into **both** arms of an `if`, so it is in each arm's own
  block rather than in the join block. `jump2`'s cross-jumping merges the two
  copies back and plants the join label in front of the result, so the word
  count is unchanged -- but sched1 never saw the store and the constant in one
  ready list. That is how func_8004491C's last two words came right; the hazard
  heuristic that was reordering them is a pure function of the insn and cannot
  be steered from C.

  This works for an **arithmetic** insn too, not just a store: func_80044D24
  negates inside both arms rather than at the join, jump2 merges the two
  `negu`s back into one, the word count is unchanged, and the sra's destination
  stays a block-local temp -- worth 8 words there.

  Because the merge happens in jump2, **after** global-alloc, the duplicate is
  also a way to buy a pseudo one extra *reference* at allocation time at no
  cost in words. That is what closed func_800423C0; see section 3.

  **jump2 does not cross-jump a call**, so a duplicated call stays duplicated:
  func_80045CC4's RotAverage4 in both arms came out 15 words longer.

  **An `if`/`else` whose two arms are IDENTICAL costs zero words, and buys a
  basic-block boundary.** jump2 merges the arms, the now-pointless conditional
  branch becomes a jump-to-next and is deleted, and `delete_computation`
  removes the condition's own insns with it -- even a load:

      void ra(PX *p, s32 c) { if (c) { p->f6E = 0x100; } else { p->f6E = 0x100; } p->f87 = 1; }
          -> li $2,256 / sh / li $2,1 / j $31 / sb      (no branch, `c` never read)

  That makes it the one way to put a **cse run boundary** in the middle of a
  straight-line stretch for free, which is what defeated func_80041E70's
  constant fold at an unchanged 270 words. The caveat is the price: it is free
  in words but it is a real **sched1 and sched2 block split**, so it only helps
  where the surrounding schedule can tolerate one. In func_80041E70 the body
  came out byte-exact and the first block then lost two words to scheduling.
  Arms holding a different insn, or placed a statement earlier or later, cost
  1-3 words -- the placement sweep is in its near-miss file.

  And it can be used to **prevent** a merge. Duplicate a run of insns into both
  arms whose *last* insn differs between them, and cross-jumping merges only
  the common part, leaving the differing insn in the arm where sched1 put it.
  func_80045CC4 needs its two halfword stores inside both arms so that each
  arm's own `subu`/`addu` stays last and lands in the branch's delay slot;
  without it, jump2 pulls all four stores into the join. Worth 4 words.

## 2. Loop notes change every reference weight in the function

**In a register-starved function, dropping a loop note is destructive rather
than a lever.** Rewriting func_8003CCB0's init loop as a backward `goto` loop
costs 4 words and 250 differing; its main loop, 6 words and 263 differing.
Reach for this only where the function has registers to spare.

A `for` or `while` emits `NOTE_INSN_LOOP_BEG`, which puts its body at
`loop_depth` 2, and `flow.c` adds `loop_depth` to `reg_n_refs` for every
reference. A backward `goto` loop gets no note, so every reference outside an
inner loop drops from weight 2 to weight 1.

That rescales `global.c`'s allocation priorities **unequally** whenever
different variables' references sit at different depths, which is how
func_8004068C's register permutation came right: `len`'s references were split
across two depths while `off`'s and `p`'s each sat at one, so dropping the note
took `len` from last of the three to first.

But a loop whose invariants were hoisted must keep its note -- a hoisted base
pointer in the preheader is proof that loop-invariant motion ran.

**loop.c hoists the invariant constant that is SET FIRST in the body, not the
cheapest one, and it hoists only one.** Measured on func_80045CC4, which has
two invariant masks in one loop: with `(q->unk_00 & 0xFF000000) | (ot &
0xFFFFFF)`, expand materialises 0xFF000000 first, so that is the one hoisted --
as a single `lui` in the prologue -- and 0xFFFFFF stays inline as `lui` + `ori`
every iteration. **Swapping the two operands of the `|` flips which one gets
hoisted**, and the target's shape is the swapped one, down to the two prologue
positions the `lui`/`ori` are scheduled into. Reasoning from cost is backwards
here: the expensive constant is not the one preferred. Note also that the
hoisted constant is placed in the function prologue, not in a loop preheader,
so seeing it there is not evidence that the original declared a function-scope
local.

## 3. Allocation priority is the commonest thing left

When every opcode, immediate and delay slot matches and only registers differ,
it is almost always one allocation-order decision. Read it out of the dumps
rather than guessing:

    tools/decomp/dump.sh zz_foo.c
    grep 'Register 8' /tmp/azure-decomp/dumps/zz_foo/lreg   # refs / live_length
    sed -n '/regs to allocate/,/dispositions/p' .../greg    # order and conflicts

`global.c` ranks by roughly `floor_log2(refs) * refs / live_length`, and both
numbers are measured **post-sched1**. Crossing a power-of-two boundary in the
reference count is a large jump; one insn of live length is a small one, and
adjacent priorities are often within 2% of each other.

Levers that move them, all with no change to the emitted code:

* Hoisting a subexpression into its own local **one statement earlier**. It
  pushes another value's last use one scheduling slot later, lengthening its
  live range by one insn. func_80046AFC.
* Narrowing a parameter to `s16`. The parameter copy splits into an SImode copy
  plus a HImode subreg copy, and the extra reference tips the order.
  func_8003F80C.
* Reusing one local for two unrelated purposes, so its pseudo stays in a
  callee-saved register across a call and the copy into the argument register
  survives instead of coalescing away. func_80040CBC.
* Where a local constant is **declared**. `s32 m = -0x40;` inside a loop body
  gets its single set hoisted by `loop.c` into the preheader, after the loop
  counter's initialisation; at function scope it is emitted in declaration
  order, ahead of it. func_80046C20.

* **Splitting one assignment into two statements that compile to the same
  instruction.** `op = (s32) base; op += e->unk_04;` and
  `op = (s32) (base + e->unk_04);` both emit a single `addu`, but the first
  gives `op` two more RTL references. This is the cheapest priority lever there
  is -- no extra insn, no extra live length to pay for, unlike the duplication
  below -- and it is what closed func_80041344 after 24 differing words.

  **Two preconditions, both measured.** First, **the split-out statement must
  not be a memory load.** A copy (`op = (s32) base;`) coalesces away; a load
  (`a1 = p->unk_20;`) leaves a multi-set pseudo that stops combine folding the
  address back into the MEM, and a real insn appears -- func_800453E0 measured
  that three ways at +1 word and 76 differing. So split at an arithmetic
  subterm, never at the base load. Second, **on a BLOCK-LOCAL quantity the
  currency is live length, not references.** local-alloc orders quantities with
  `qty_compare`, which compares `qty_death - qty_birth` and nothing else;
  references are global.c's. A free reference therefore buys nothing there by
  itself -- but hoisting a subterm into its own local still works, because the
  pseudo is BORN ONE INSN EARLIER, which is a live-length change. That is what
  took func_800453E0 from 10 differing words to 5, moving one global allocno
  and one block-local quantity in a single exchange. **So: on a global allocno
  reach for the extra reference, on a block-local quantity reach for the
  earlier birth, and the same one-line hoist often gives both.**

  **The reason a couple of references matter so much there is worth knowing in
  its own right**, because it inverts the usual intuition that a high priority
  is what you want. `find_reg` consults `regs_someone_prefers`, which collects
  the hard-register preferences of conflicting allocnos that are *lower*
  priority -- i.e. not yet allocated -- and avoids those registers if it can.
  So an allocno that goes first can be *pushed off* the register it should have
  had, by a rival that has not been allocated yet:

  * In func_80041344, `op` is copied into `$a3` as func_8003F80C's fourth
    argument, so it carries a preference for `$a3`. With one statement `op` had
    52 references over a live length of 56 -- `floor_log2(52) * 52 / 56 =
    4.64` -- just under the shared loop counter's `39 / 41 = 4.76`. The counter
    was therefore allocated first, saw `op`'s `$a3` preference in
    `regs_someone_prefers`, avoided `$a3`, took `$t0`, and the record pointer
    `e` was left with `$a3`. The target has the counter in `$a3` and `e` in
    `$t0`.
  * Two more references for `op` (`54 / 56 = 4.82`) flip the order. `op` is now
    allocated first and takes `$v1`; its preference is spent, so the counter
    takes `$a3`; `e` then finds `$v1` and `$a3` both taken and lands on `$t0`.
    Every register in the function falls into place at once.

  **A hard-reg COPY preference beats that ordering asymmetry, and cse can
  manufacture one for you.** `prune_preferences`/`find_reg` try an allocno's
  copy preferences -- registers it is actually copied to or from -- *before* the
  pass that excludes `regs_someone_prefers`, so a copy wins outright. In
  func_8003E4FC the `u8` parameter's allocno prefers `$a0` and the switch-index
  allocno has 26 references against 5, so the index goes first; with no copy it
  avoids `$a0` for the parameter's sake and takes `$v1`, which is the target.
  But writing `kind` at a call site whose case value cse cannot fold to a
  constant makes cse rewrite the argument *as the index pseudo*, creating
  `(set (reg:SI 4 a0) (reg:SI 106))` -- a copy preference for `$a0` on the
  index -- and the index then takes `$a0` with the parameter spilled to `$v1`
  behind a `move`. Writing the case's literal at those sites deletes the copy
  and the whole permutation flips: 93 differing words at an unchanged word
  count. **Diagnostic:** grep the `.lreg` dump for `(set (reg:SI 4 a0) (reg:SI
  N))`; `.greg`'s `;; N preferences: 4` line does *not* distinguish a copy
  preference from a plain one.

  **So the lever on an argument-register preference is allocation order, not
  the preference.** Trying to break the preference does not work and should not
  be retried: it propagates along the dataflow chain through arithmetic insns,
  splitting the chain through an intermediate variable leaves *both* allocnos
  carrying it, and a separate `s32 f = (s16) x;` is eliminated by combine
  outright. All three were measured on func_80041344 and func_80041588.

  The two other ways of buying `op` those references both cost words, which is
  the argument for preferring a split that emits nothing: splitting the switch
  index into `op = *(u16 *) p; op -= 1;` leaves the load in `op`'s own register
  where the target has a block-local `$v0` (2 words), and folding the loop
  tail's `size` into `op` takes over the tail load, which the target also keeps
  block-local (3 words). Both were at one point written up as the floor for
  that function; neither was.

* **Duplicating an insn into both arms of an `if`** (section 1) raises the
  reference count of every pseudo it mentions, and jump2 merges the duplicate
  away after allocation, so the word count does not change. This is the only
  lever here that raises a priority on demand, and it is what closed
  func_800423C0: the base pointer sat at `floor_log2(7) * 7 / 15 = 0.93`
  against the loop counter's `floor_log2(4) * 4 / 8 = 1.00`, a 7% gap, and
  whichever was allocated first took `$v1` -- every instruction, immediate and
  displacement already matched with 12 register fields wrong. Duplicating one
  byte store into both arms of an inner `if` took the pointer to 8 references
  across 18 insns, across the `floor_log2` boundary at `3 * 8 / 18 = 1.33`, and
  the allocation order flipped from `95 113 ...` to `113 95 ...`. Duplicating a
  different store in the same block worked identically. **Crossing a power of
  two in the reference count is the whole move**; adding a reference without
  crossing one lengthens live_length as well and usually makes things worse.

  **Aiming the duplicate took four tries on func_80045CC4, and each failure is
  a rule.** It closed a 0.9% gap there -- `s16 *p70;` with
  `p70 = &p->unk_70;` duplicated into both arms and passed as RotAverage4's
  first argument, 167 differing words down to 47 with the word count unchanged:

  * **Count the live-length cost, not just the references.** A duplicated
    4-insn statement gave the pseudo being raised +4 refs and +8 live length
    while giving its rival +2 and +4 -- the duplicate costs the longer-lived
    pseudo twice as much live length -- so the ratio converges slowly. The
    obvious word-neutral candidate there moved the gap from -0.90% to only
    -0.41%, and four such statements would have been needed where two existed.
  * **The duplicate must mention ONLY the pseudo you are raising.** Hoisting a
    condition into a variable fixed the pair being aimed at and flipped an
    adjacent pair nobody was watching, because its extra field read took a
    third allocno from 16 references to 18.
  * **Duplicate an address computation that is not already live**, not a reload
    of a field the enclosing block just read. cse rewrote one arm's copy of a
    reload as a `move` while the other kept its `lhu`; the copies then differ,
    jump2 cannot merge them, and it costs a real word.
  * **For a call argument, it must be argument 0.** Duplicating the address of
    arguments 1-3 gave 435 words and 273+ differing, because only argument 0's
    allocno carries the `$a0` preference that stops the duplicate being folded
    differently. Even then it can land in `$a1` and cost a `move`.

**Hard-register preferences propagate along the dataflow chain, including
through arithmetic insns, and they cannot be shed with an intermediate
variable.** This is more than "a call's argument preferences propagate". In
func_80041344 one allocno is printed `preferences: 3 7`: the 7 comes from
`(set <t> (ashift <op> 16))` whose consumer writes hard `$a3` directly, and the
3 from `(set <op> (ashift <t112> 4))` whose source local-alloc had put in `$v1`
-- neither insn is a copy. With both preferences it takes `$v1`; with only the 7
it takes `$a3`, and deleting the pref-3 source costs 51 differing words against
24. Splitting the chain through another variable leaves **both** allocnos
carrying `preferences: 3 7` and changes nothing, and a separate
`s32 f = (s16) op;` is eliminated by combine outright.

**But allocation ORDER decides whose preference wins, and that is reachable.**
`regs_someone_prefers` collects only the preferences of *lower-priority*
conflicting allocnos, so whoever is allocated first has its preferences
collected into the other's avoid set. func_80041344's entire remaining
permutation is one comparison -- 4.643 against 4.756 -- and moving it with the
levers above puts eight registers right at once, taking it from 24 differing
words to 2. So an argument-register preference is not a wall by itself; what
matters is which allocno reaches `find_reg` first.

**A dense switch's index is a separate pseudo only when the lowest case label
is non-zero.** Write `op = *(u16 *) p - 1;` and renumber the arms from `case 0`,
and `expand_end_case` skips its own MINUS because minval is 0 -- so `sltiu` and
`sll` read the user's pseudo and the index **is** a source variable. It then
carries whatever references that variable has, and conflicts with whatever it
conflicts with. In func_80041344 the index, the flag word, both loop bounds and
a pointer are deliberately one variable for exactly this reason. Second
instance of the rule below: block-local is a property of the pseudo, not of the
value.

Two things that cannot be steered from C:

* `find_reg` avoids registers that *conflicting allocnos prefer*, so changing
  which allocno holds a preference cascades through the whole assignment --
  though see the ordering asymmetry above before concluding a preference is
  fatal.
* **local-alloc runs before global-alloc** and assigns block-local quantities
  first, looking only inside the one block. A switch index, or any value live
  in one block only, is out of its own reach -- that is what is left in
  func_80041344 -- **but only while it has its own pseudo.** Giving the value a
  variable that lives across blocks promotes it from a local quantity to a
  global allocno, and every global allocno here conflicts with the hard
  registers the function uses, so find_reg then avoids them.

  Measured in func_80042900: a dead `volatile` load written as a bare statement
  is a one-reference, one-block quantity, local-alloc sees `$v0` free in that
  block (the return value's `$v0 = 0` is set in a different one) and takes it,
  the first register in REG_ALLOC_ORDER. Assigning the load to the local that
  holds the sign-extended argument both tests compare against -- a global
  allocno, conflicting with hard reg 2 -- moves it to the target's `$v1` at no
  cost in words, because that local's sign extension is a `sll`/`sra` pair the
  target already has. So "it is block-local, therefore unreachable" is only
  true of the pseudo, not of the value.

## 4. Re-dereference rather than cache

If a store invalidates cse's memory entry for another object, the target will
show a genuine reload. Caching the value in a local removes the reload and
costs a lot: 11 words in func_80043FD0, 22 in func_80043A68, 55 in
func_80046E38, 86 in func_80041CBC. The same goes for a value read either side
of a call.

Conversely, a value the target keeps in a register across a call wants a local;
one it recomputes wants the expression written out twice (func_8003F8F8
computes its size twice, once for the fit test and once for the accumulation).

**And a re-read that cse turns into a register copy can be load-bearing in its
own right.** When a redundant load's value is already in a pseudo, cse rewrites
the load's SET_SRC to that pseudo; if the statement then writes the *same*
pseudo in place, it has two sets, so combine will not substitute it away and
flow will not delete it. That is how a `(set p2 p1)` + `(set p2 (op p2 ...))`
pair survives into the output:

    v = *(s32 *) X;                 /* p1 */
    ...
    *(s32 *) X = *(s32 *) X << 8;   /* -> addu p2,p1,zero ; sll p2,p2,8 */

Taking the value from the local instead collapses it to a single `sll`. Worth 2
words in func_8003D0F0 -- the copy, plus the `nop` that the freed `sll` would
otherwise have filled in the next load's delay slot -- and it is sensitive to
statement order: only one of the six permutations of its three stores produces
the copy at all.


* **When sched1 lifts an arithmetic insn above stores that should precede it,
  hoist its operand's LOAD into a local one statement earlier.**
  `t = (s8) p[-2]; sp->a = t; sp->b = t; t = t + (u16) sp->f;` lets sched1 lift
  the `addu` above both `sh`s, because its dependence chain is longer. Reading
  the operand first -- `h = (u16) sp->f;` *before* the two stores, then
  `t = t + h;` -- leaves the `addu` where the target has it. Worth 10 differing
  words across two four-store groups in func_8003CCB0 at an unchanged word
  count. Section 1's lever for a wrong sched1 order is block membership and
  section 7's is the number of sets; this is a third and cheaper one for the
  specific case of an arithmetic insn floating above stores.

## 5. MEM_IN_STRUCT_P decides sched1's alias calls

A struct or array reference sets `MEM_IN_STRUCT_P`; a bare pointer dereference
does not. `true_dependence` returns "no dependence" when two MEMs' in-struct
flags **differ** AND the non-struct side's address does not vary -- which frees
sched1 to interleave things the target keeps sequential, and vice versa.

**Both halves are required**; do not simplify this to "differing flags make
them independent". Measured, each as `store; return load + 1;` so that sched1
has a load-delay slot it would fill if it could:

    s32 e1(S *p, s32 *q) { p->a = 5; return *q + 1; }       -> sw then lw: dependent
    s32 e5(S *p)         { p->a = 5; return D_80081480 + 1; } -> lw MOVES AHEAD

e1 differs in flags but the non-struct side is a register base, which varies,
so the exception does not apply. e5's non-struct side is a symbol, which does
not vary, so it does. Both-in-struct and both-non-struct are dependent as you
would expect.

**A register base does not always vary, and this is the half that was missing.**
`sched.c`'s `init_alias_analysis` builds `reg_known_value`, which records a
pseudo with a SINGLE set from a constant as equal to that constant. So a local
like `u8 *sp = (u8 *) 0x1F800000;` is a constant address to
`true_dependence`, not a register one, and every non-struct
`*(T *) (sp + off)` store is independent of every *in-struct* read around it --
sched1 will lift those reads up into the store group and fill their load-delay
slots. That is why the scratchpad functions are so sensitive to which accesses
are casts and which are struct fields, and it is **per-site, not blanket**:
func_8003D0F0 needs `*(u16 *) ((u8 *) e + 6)` to keep one `lhu` last (2 words,
and the target's `nop` only appears with the cast), while the neighbouring
`e->unk_04` read does not need the cast at all, because by then nothing is left
in the block to hoist it past.

* `*(u16 *) ((u8 *) p - 2)` keeps two read-modify-writes strictly sequential;
  every struct or array spelling of the same address lets sched1 interleave
  them and fill both load-delay slots. func_80043FD0, 2 words.
* Making a pointee a struct lets three loads interleave against two
  non-struct scalar stores. func_80040CBC, 5 words.
* A whole region reached through `*(T *)(base + off)` instead of struct fields
  lets sched1 hoist every unrelated load above the region's stores and shuffle
  the stores among themselves -- 123 of 162 words wrong in func_8003DBD0 with
  the word count unchanged. The constant-base canonicalisation above is the
  mechanism: the region's base is a pseudo set once from a constant, so the
  cast accesses are non-struct references at a non-varying address and the
  exception applies to every pairing with an in-struct read.
* **The case that was flagged as unexplained is now explained, and it is the
  rule above.** func_800463EC's match turns on which of its scratchpad stores
  and table loads are casts and which are struct fields -- four combinations
  measured at 294, 297, 293 and 286 words -- and this entry used to say that
  since both sides are register bases throughout, the in-struct exception
  should not apply to any of them. It does: the scratchpad base is a pseudo
  with one set from `0x1F800000`, which `init_alias_analysis` canonicalises to
  that constant, so those addresses do not vary. The measurements were always
  solid; the reasoning about them was wrong, and the probes `e1`/`e5` above did
  not discriminate because `e1`'s register base is a *parameter*, which really
  does vary.

* **Spelling a small `$gp` global as an array element makes its store
  in-struct, and that pins it.** `D_80080AFC[0] = x;` is an in-struct
  reference, so `true_dependence` holds it ahead of neighbouring array loads
  and a load-delay slot goes empty; as a plain scalar the store is a non-struct
  reference at a non-varying address, the exception applies, and sched1 drops
  it into the slot exactly as the target has it. Worth one word and 79
  differing ones in func_8004437C. **Corollary:** when one object must serve a
  `+0` and a `+2` access in the same unit, do not retype it to an array --
  spell the odd offset `*(u16 *) ((u8 *) &SYM + 2)`, which still yields the
  `%gp_rel(SYM + 0x2)` addend form and leaves every other access a scalar.
* **A parameter's type is the easiest way to set the flag.** func_80041284 was
  parked for four words on exactly this: as a `u32 *` both its loads and the
  `D_80081480` store are non-struct, the exception does not apply, and
  store-before-load is a true dependence that pins the store first. Declaring
  the parameter a one-field struct makes the loads in-struct, the symbol store
  stays non-struct with a non-varying address, the exception applies, and
  sched1 hoists the load above the store as the target has it. Nothing else
  about the function changed.

## 6. Declaration decides addressing

This is the subject of `docs/addressing-and-translation-units.md`; the short
form:

* A scalar, or an array inside `-G8`, gets the assembler-expanded `la`.
  Anything larger, or an incomplete `extern T X[];`, gets the compiler-split
  `lui`/`addiu` -- which is **two** insns, so reorg can put the `addiu` in a
  delay slot where the single `la` macro cannot be split (func_80040CBC, 3
  words).
* A global outside the loaded image (0x8002d000..0x8008875f) was a constant
  address reached through a constant base pointer with the offset written inside
  the index expression.
* splat renders a bare `lui reg, 0x8001` as `%hi(SYM)` whenever it can pair it
  with a later `%lo`, which hides the difference.
* Nothing folds a constant into a `lo_sum`, so `p += CONST` on a pointer local
  is not interchangeable with `SYM + CONST` (func_80041CBC, 57 words).
  **The mirror image: `&ARRAY[K]` DOES fold the constant into the address.** It
  emits `lui %hi(SYM+K*size)` / `addiu %lo(SYM+K*size)`, one pair per pointer,
  whereas `b = ARRAY; d = b + K1; s = b + K2;` materialises the symbol's
  address once and adds the constants with their own `addiu`s -- cse keeps the
  register because a REG costs less than a `CONST` in `rtx_cost` for an address
  operand. In func_8003CB08 that difference is 2 words and 62 differing, and
  the base-pointer form is the target's. So the rule covers both directions:
  a constant folds into an address *constant* and never into a `lo_sum`.
* A big object reached through a pointer local keeps its address in a
  callee-saved register across calls; reached by symbol it rebuilds a
  `lui`/`%lo` pair per access (func_8003D5A4, 105 words; func_80046884, 116).
  **A ONE-set pointer local does something different and useful: it collapses
  to a bare `lui %hi`, relocated into the function's ENTRY block.** expand
  emits the object's `la` there; cse then rewrites every `g->field` MEM as a
  `lo_sum` off the `(high)` pseudo, the `addiu %lo` goes dead, flow deletes it,
  and what is left is `lui $s0,%hi(SYM)` in the entry block with
  `lw %lo(SYM)($s0)` at each use. It is the only way found to get a large
  in-image object's `%hi` materialised **before the first conditional branch**,
  and in func_8003CB08 that one insn is worth a word and 104 differing words:
  the anti-dependence forces `sw $s0` early in the prologue, which leaves
  `sw $ra` for the first branch's delay slot and empties the second `beq`'s.
  So the two cases have distinct signatures -- one set gives a hoisted bare
  `lui`, two sets give the split `lui`/`addiu` pair per site.

  **And the two-set rule needs the set and the use in DIFFERENT basic blocks.**
  A plain `g = &SYM;` followed by a use in the same block folds back to
  `lui` + `lw %lo` *even when the pseudo has other sets elsewhere in the
  function*, because cse has the `(high)` pseudo live in that block. Section
  1's zero-word identical `if (c) { g = &SYM; } else { g = &SYM; }` buys the
  block boundary anywhere you need it, and that is what gives func_8003CCB0 the
  target's `lui %hi` / `addiu %lo` / `lw 0x0(reg)` at both of its sites:
  dropping the first site's arms costs 31 differing words at an unchanged word
  count, dropping the last site's costs a word.
  **Where the pointer local is DECLARED is a second lever on top of that one,
  because it decides whether the address materialisation is a source statement
  or a loop.c hoist.** Declared outside a loop it is an ordinary statement and
  lands before the loop GUARD; declared as the loop body's first statement it
  becomes a loop-invariant like any other and lands in the PREHEADER -- where it
  competes with the loop's other invariants, because loop.c hoists only some of
  them and takes them in the order it meets them. func_8004713C has three
  invariants (the RECT address, `arg1 << 4`, `arg1 << 6`) and loop.c hoists two:
  outside the loop, 47 words and 44 DIFF; no pointer local at all, 45 words and
  45 DIFF with the address rebuilt per iteration; declared first inside the
  body, a 49-word MATCH, with `arg1 << 6` the one left behind. Losing that
  shift is what gives the first `beqz` an empty delay slot while the hoisted
  `arg1 << 4` lets sched1 fill the second one. **So when the target hoists a
  surprising subset of a loop's invariants, the lever is the order they appear
  in the body, not the expressions themselves.**

  **That entry is about objects small enough for the assembler-expanded `la`.
  Once cc1psx has COMPILER-SPLIT the address into `lui %hi` + `addiu %lo`, the
  address is an ordinary pseudo and cse shares it by itself**, so a symbol used
  twice needs no pointer local: in func_800453E0, D_8006CD30 is reached by
  plain symbol at two sites separated by three calls and cse keeps the single
  pair in `$s5` unaided.
* `expand_block_move` `force_reg`s a fresh pointer when the address is a
  non-offsettable `(const (plus symbol off))`, so a block copy out of a global
  wants a pointer local (func_80044724, 106 words).

**But four functions deliberately MIX the two spellings for one object**, so do
not assume one spelling throughout. func_8003D5A4 reaches D_80083160 through a
pointer local and D_80083208 as a plain symbol; func_80044724 uses a pointer
local for the source and an array reference for the destination; func_80043A68
needs the word at offset 0 to get its `%lo` on the load while the halfword at
+0x14 comes off an `addiu` from the same `lui`; func_8003D0F0 uses a pointer
local for the scratchpad but passes literal constants to libgte.

An address that is neither a symbol nor in the image behaves the same way: the
0x1F800000 scratchpad wants one struct through one pointer local, because its
low half is zero so the materialisation is a single `lui` and everything else
is a displacement. Literal constants per access cost 194 words in
func_8003DBD0 and 262 in func_8003D0F0.

**`$at` in a `%hi`/`%lo` pair proves the reference was a BARE symbol left to
the assembler.** gas's macro expansion uses `$at` for a store and reuses the
destination register for a load (`lui $v0,%hi` / `lw $v0,%lo($v0)`), whereas
cc1psx's own split form always uses two allocatable registers and can have
other insns scheduled between them. splat prints both as `%hi`/`%lo`, so this
is the tell -- the companion to the "read the second opcode" rule below, and
what identified func_8004450C's addressing mode.

**A constant address used TWICE costs 2 words.** expand `force_reg`s a large
`CONST_INT` address at each use, cse merges the two materialisations, and
combine folds a constant address back into the MEM only when the pseudo has a
single use -- so a twice-used one survives in a callee-saved register across
any call, buying an `sw`/`lw` pair. Single-use constant addresses do collapse
to the two-word absolute form, which is the mechanism behind the
`lw $2,-2146946720` note below. Measured at 55 words against 53 in
func_8004450C.

**In a carved unit, the gas `-G` value picks the addressing mode BY SIZE, and
that is a lever.** gas emits GPREL16 only for a symbol whose `.extern SYM,size`
record gives a size in `(0, G]`; with no record at all the access is absolute.
cc1psx emits that record from the declared type, and uses the same threshold to
choose between the bare-symbol form (assembler-expanded) and its own
`lui %hi` + displacement split. So there is **no C declaration that gives
bare-symbol-but-absolute** -- and when one function needs `$gp` for a `u16` and
absolute for a `u32`, the way to get both is a per-unit `-G` between the two
sizes. `gprel_152E4` and `gprel_17504` are built at `-G2` for exactly that;
func_80041AE4 is 44 of 45 words wrong at `-G8` and matches at `-G2` with plain
scalar declarations. Measured against gas directly, and written up in
`docs/addressing-and-translation-units.md`.

**`-G` separates by size, so it cannot help when the two symbols are the same
size** -- and then the size record has to be overridden per symbol.
`gprel_17B7C` needs the 4-byte D_80081508 `$gp`-relative (func_800448BC) and
the equally 4-byte D_80081480/D_8008148C absolute (func_8004450C), so no `-G`
value separates them. gas decides small-vs-large at **relaxation** time, after
the whole file is read, and the LAST `.extern SYM,size` wins; cc1psx's own
`.extern X,4` sits in the file header. So a statement-level
`__asm__(".extern X, 16")` **inside any function body** of the unit flips that
symbol to absolute and the natural scalar source then matches. It emits no
bytes (`readelf -s` shows UND, value 0, size 0). Two caveats: a **file-scope**
`__asm__` does not work, because cc1psx buffers top-level asm and emits it
*ahead* of its own `.extern` block; and the directive is a scheduling barrier,
so put it somewhere inert -- in func_8004450C's `return 1` arm it is free, and
as the function's first statement it costs 6 words. Every declaration-based
spelling measured 2 words worse.

Worth knowing what this is standing in for: ASPSX did not use a size threshold
at all. It emitted `$gp`-relative only for symbols **defined in the same
translation unit**, which is why the original build's imports are all absolute.
The faithful reproduction is therefore the `.sdata` definition route, not a
`-G` value -- and that is blocked on linker placement splat cannot express, so
the override is the pragmatic stand-in.

**An out-of-image global is NOT always a constant address, and the second
instruction tells you which.** This project's rule of thumb -- that a global
outside 0x8002d000..0x8008875f was a constant address reached through a
constant base pointer -- is wrong at the opcode level whenever the symbol's low
half is nonzero and below 0x8000: a **constant** gives `lui` + **`ori`**, a
**symbol** gives `lui` + **`addiu`**. func_80042BDC references 15 string
symbols at 0x800E1D55..0x800E1F41 plus D_800E3D7C and D_800E3DF0; spelling them
as `(u8 *) 0x800E1D55` constants left 15 words differing with the word count
unchanged, and `extern u8 D_800E1D55[];` fixed all 15 at once. The symbols are
real -- `build/slus_006.14.map` has them -- they were simply missing from
`include/structs.h`. The constant-address rule still holds for a base whose low
half is zero (the 0x1F800000 scratchpad) and for offsets folded into an index
expression; it does not hold for a plain two-instruction address.

**A pointer-to-symbol local with TWO SETS selects the compiler-split form.**
This is the first spelling found that chooses `lui %hi` + `addiu %lo` + a `0x0`
displacement over `lui` + `lw %lo` for a symbol inside the loaded image, and it
is what lets one function mix both spellings of one object:

    void b3(u32 x, s32 c) { Unk80083160 *g;
        if (c) { g = &D_80083160; } else { g = &D_80083160; }
        *(u32 *) (g->unk_00 + 0x8D0) = x; }
    ->  lui %hi / addiu %lo / lw 0x0($2) / sw

With one set it folds back to `lui` + `lw %lo`. A CONST_INT folds through two
sets -- a two-set `(u8 **) 0x80083160` still folds -- so it is specifically a
SYMBOL_REF that two sets block, the same combine-opacity rule as section 7's
multi-set entry. It needs a real join, with both sets reaching the use.
Measured as folding, so do not retry: the plain expression, a `u8 *` local, a
block-local initialised pointer, `*(u8 **) &SYM`, an index expression, a
`volatile u8 **` local, a `volatile`-qualified struct pointer, and the constant
address `((T *) 0x80083160)->field`, which makes cc1psx emit an absolute
`lw $2,-2146946720` that the assembler still expands to two words.

## 7. Narrow types

* An `s16` parameter lets a halfword store reuse the raw incoming register
  while the sign-extended copy is a separate pseudo; declared `s32` the copy is
  forced to the top of the function and a delay slot goes empty
  (func_8004425C). It also splits the parameter copy, which is schedulable and
  drifts (func_80046884, 8 words).
* A `u16` field read with `(s16)` casts yields a bare `lh`, because combine
  folds zero_extend-then-sign-extend-of-low (func_8004425C). **It works the
  same way on a `u16` GLOBAL**, which makes it the cheap way to take a signed
  read off a declaration you do not want to retype: `D_80083120[(s16) D_800814E8]`
  gives `lh` and dropping the cast gives `lhu`, one differing word with the
  count unchanged (func_80041B98, and three others in that batch).

  **But only where the sign-extended value is actually consumed.** Cast a `u16`
  global and compare it against zero and the cast is folded away entirely, so
  the load stays `lhu` -- 10 differing words in func_8004437C. When one object
  needs `lh` at its comparisons and `lhu` at one arithmetic site, do the
  mirror image: declare it `s16` and put a `(u16)` cast at that one site.
  `(u16) D_80080AFC ^ 1` gives the single `lhu`/`xori`/`sh` the target has.
* **A narrow local is still an SImode pseudo** (`PROMOTE_MODE`), so there is no
  paradoxical-subreg escape from cse's `equiv_constant`: `u16 n`, `s16 n` and
  `u16 n <<= 1` all fold to `li 32` exactly as `s32` does (func_80041E70).
* **The byte-to-signed fold: `lbu`/`sll 24`/`sra 24` against a single `lb`.**
  This has been written down wrong twice, so here is the whole mechanism
  first. `extendqisi2`'s expander does `force_not_mem` under `-O`, which means
  `lbu` / `sll 24` / `sra 24` is the **natural** expansion of `(s8) p[2]` and
  `lb` is the optimised one: `combine` produces it with a 3-insn combine into
  `extendqisi2_internal`, whose operand must satisfy `memory_operand`. So the
  job is never to *find* a spelling that emits the shifts; it is to stop that
  one combine. Three things stop it, and they differ in cost:

    - **A HImode consumer. Free, and the first thing to try.** If the result of
      the chain is consumed in HImode -- an `s16` accumulator, a halfword store
      -- combine runs `force_to_mode` with a 16-bit mask down the chain instead
      and the refold never happens. No extra pseudo, no extra register, no
      extra word:

          typedef struct PB { s16 a; u16 c; s32 d; } PB;
          void fh(u8 *r, PB *q, s32 c) { if (c) q->a = -(s8) r[2] - q->c;
                                         else   q->a =  (s8) r[2] - q->c; }
                              -> lbu / sll 24 / sra 24   in each arm
          void fm(u8 *r, PB *q, s32 c) { ... q->d ... }   -> lb

      This is also the reason the already-matching **func_8003DBD0** is right
      (`p->unk_70 = -(s8) r[2] - p->unk_108;`, a single HImode store), which
      had been unexplained; it is what reproduced func_80045CC4's four arms;
      and it is what **matched func_8003D0F0**, which had been parked for two
      sessions on the belief that no cost-free blocker existed. Worth 8 words
      there. Declaring the accumulator `s32` and casting it `(s16)` at the store
      does **not** work -- the extensions stay eliminated and it costs 16 words.
    - **A memory store between the load and the cast.** `use_crosses_set_p`
      stops combine substituting the load into the sign extension. Also free of
      registers, but it needs a store you actually want in that position:

          s32 f(u8 *p, s32 *q) { u8 t = p[2]; *q = 5; return (s8) t; }
                              -> lbu / sw / sll 24 / sra 24

    - **`volatile`.** `gen_lowpart_for_combine` returns a CLOBBER for a
      volatile MEM, so the substitution cannot happen at all. This is the one
      place `volatile` is a real mechanism rather than a nudge (contrast
      section 10), and it closed func_80044D24's last 7 words first try with no
      extra register, where the consumer is SImode and no store was available.
      Two caveats, both of which cost real time:
        * **Do not qualify the field.** loop.c's `find_mem_givs` skips volatile
          MEMs, so a volatile field is addressed off the biv where the target
          reads off the combined giv. Take the address through a
          `volatile u8 *` local instead -- the *pointer* is an ordinary
          DEST_REG giv that `combine_givs` merges into the representative.
          Pointing it at the field and dereferencing directly costs 53 words,
          because cse eliminates the pointer.

          And **point that pointer at the field the representative is derived
          from, then index backwards** -- not at the field you want to read.
          `combine_givs` only merges a DEST_REG giv for free when its base
          offset equals the representative's, and the representative comes from
          the LAST nonzero-offset access in the body (section 9). Measured on
          func_8003D0F0, whose representative is at +4: pointers spelled
          `&e->unk_02` and `&e->unk_03` gave one pair addressed off the **biv**
          and the other its own induction register plus a `move`, even with
          caveat 2 obeyed, while `(volatile u8 *) &e->unk_04` dereferenced
          `[-2]` and `[-1]` merged cleanly into `lbu -0x2($s2)` /
          `lbu -0x1($s2)` with no extra register. Both reached 220 words there,
          against 222 for the `s16` route -- which is why the HImode consumer
          is the first thing to reach for and this is the fallback.
        * **Assign the pointer in the ENCLOSING block, one per block.** In the
          same block as its use, cse folds it into the address before `loop`
          runs and no giv is formed. Shared across two blocks it is not
          `replaceable` (loop.c's `last_use_this_basic_block`), so loop.c
          leaves a `move` copy and cse2 only propagates that away inside its
          path-length window. Two locals holding the same address, one per
          block, is what matched.

  **Two sign extensions of one loaded value is NOT a fourth mechanism**, and
  believing it was cost two sessions. It does change the output -- one shared
  `lbu` above the `if` and a shift pair per arm -- but that is a *different*
  instruction sequence, not the target's, and it was measured as wrong in both
  functions it was proposed for: 435 words against func_80045CC4's 437, whose
  target has a separate `lbu` **in each arm**, and no gain at all in
  func_8003D0F0, where the shared `sll` hoists into a delay slot. Reach for the
  HImode consumer instead.

  What also does not work: a second use that is not itself a sign extension
  (`*q = (s8) t; return t;`) gives two loads, `lb` plus `lbu`; one variable
  assigned in both arms dies at its own use and folds locally; a second use of
  the cast costs a redundant `move`; and an intervening **load** does nothing
  at all, so a neighbouring `lhu` between the `lbu` and the `sll` in a listing
  is scheduling, not the cause. Note that `u8 t = p[2]; *q = t; return (s8) t;`
  has both a store and a second use, so it does not discriminate between the
  mechanisms -- that conflation is how this entry was first written down wrong.
* **The same trick does NOT work on the `srl`/`sra` downgrade.** For
  `(s16) (x & 0x400)` combine proves bit 15 clear and emits `srl`, and a second
  use of either the masked value or the loaded halfword does not stop it -- it
  only adds an `andi 0xffff` that the target does not have. Nor does putting
  the mask in a different basic block. Four shapes measured in
  `docs/near-misses/func_80042A80.c`; that one is still open.
* **A block move's bytes are `lb`; explicit `s8` temporaries give `lbu`** --
  the opposite of what this line used to say. Copying a two-byte field through
  `s8` locals lets combine fold the `sign_extend` load into the `(subreg:QI)`
  store, so you get `lbu`; the whole-struct assignment is one insn whose bytes
  combine never sees, so they stay `lb` (func_80042710, func_80042560, and
  section 1).
* An all-`u8` struct has alignment 1, which makes a whole-struct assignment the
  byte-aligned `lwl`/`lwr` + `swl`/`swr` block move instead of `lw`/`sw`
  (Unk3E39C, Unk44724Blk). Keep one `u32` member to force alignment 4
  (UnkTemplate).
* A local array zeroed by an aggregate initialiser calls `bzero`, which also
  forces a parameter out of its argument register; explicit word stores do not
  (func_80046884, 126 words).
* **A value with more than one SET cannot be SUBSTITUTED by combine, and that
  is a lever in its own right. It is not "opaque" -- that wording was too
  strong and is corrected below.** Two things follow from multiple sets and
  they have different predicates, so keep them apart:

  **(a) Substitution stops -- but only ACROSS a basic-block boundary.**
  `can_combine_p` will not substitute a def into a use when the def's register
  has more than one set, so a multi-set value survives as its own pseudo --
  *unless* the last set is in the same basic block as the use, because
  `reg_last_set_value` accepts a multi-set register when
  `reg_last_set_label == label_tick`. Measured in func_80041344: a many-set
  function-scope variable loaded and passed straight to a call still folds to a
  bare `lh $a3,0xC($s0)` with no extra word. So this lever needs the sets and
  the use in different blocks; within one block it buys nothing.

  **(b) `nonzero_bits` does NOT give up -- it ACCUMULATES.**
  `set_nonzero_bits_and_sign_copies` ORs `nonzero_bits` and takes the MIN of
  `sign_bit_copies` over **every** set in the function, so two sets of small
  constants prove just as much as one: `k = 1; if (c) k = 3; f((s16) k);`
  folds the cast away exactly as a single set would. To defeat a bit proof the
  variable must receive, *somewhere in the function*, a value with few sign-bit
  copies -- and **a call's return value is the cheapest such source**, since
  `nonzero_bits` of a call result is the full mode mask.

  That is measured twice, from both directions. func_80042BDC shares one
  function-scope `s32` between a 1/3 constant (passed as an `s16` argument) and
  `func_800A1BD0`'s return value (also cast `(s16)`), and **both** casts then
  survive; giving each its own local collapses both `sll 16`/`sra 16` pairs and
  costs 542 words against 543 with 351 differing. And func_80042A80's probes
  agree: a second set of `0` leaves the bit proved (`srl`), while `0x8000`, a
  parameter or a load all defeat it (`sra`) -- see section 10.

  So `i > 0` staying `blez` instead of narrowing to `bnez` (func_80041344,
  func_800407C0, func_80041588's case 9) is **not** because two sets blind
  `nonzero_bits`; it is because the accumulated value of those particular sets
  does not prove the bit.

  Beyond substitution, combine also stops reducing a `zero_extend` to a
  `subreg` and stops substituting the following `ior`. So `flags = *(u16 *) (p + 0xC);` then
  stops substituting the following `ior`. So `flags = *(u16 *) (p + 0xC);` then
  `flags |= 2;` -- the same value, loaded and updated IN PLACE -- keeps the
  whole load/OR chain as one **global allocno** where the one-expression form
  is a block-local temp. In func_80041588 that is worth 81 differing words: it
  is what makes sched1 put the `lhu` first in two different blocks and what
  stops jump2 cross-jumping one insn too deep. Spelling it as a single
  statement, `flags = *(u16 *) (p + 0xC) | 2;`, already costs 3 words, so the
  split is doing the work, not the variable.

  **Re-assigning a local the SAME expression in a later block keeps both
  sets**, and that is the cheapest way to buy this. cse does not delete the
  second `t = v >> 8;`, so the value stops being a block-local quantity and
  becomes a global allocno, which reorders the whole allocation.
  func_80044618 is entirely this: one set gives 29 words and 19 differing, two
  sets match. Think of it as section 3's duplicate-into-both-arms lever for
  when what you need is a second *set* rather than a second reference.

  **This is the lever to reach for when sched1 is wrong inside a block**, where
  section 10 says statement order will not help: changing the number of sets
  changes the dependency graph, which is the thing sched1 actually reads. Two
  sets of one pointer does the same job for register class -- func_80041588's
  `base` is masked, then copied back from the KSEG0 pointer, which is what puts
  the masked value in a callee-saved register instead of `$v0`.
* **A `u8` local plus a redundant `& 1` keeps a one-insn materialisation of a
  0/1 value, and the local's declared width picks the opcode.** For an `s32`
  value gcc holds in a callee-saved register, `if (bit != 0)` compiles to a bare
  `beqz $s2` with no insn of its own -- **but for a `u8` local it does not**:
  `if (flag != 0)` emits `andi $v0,$s0,0xFF`, the QImode truncation. So on a
  `u8` all three spellings cost the same single word and only one is right:
  `& 1` gives `addu` (func_80044698's target), bare gives `andi 0xFF`, and an
  `s32` with the mask gives `andi 0x1`. The original claim here was measured on
  an `s32` local and stated too broadly. Add the mask and declare the local
  `s32` and you get a real `andi $v0,$s2,1`; declare it `u8` and gcc proves the
  QImode value's bits, folds the mask to the identity, and what survives is
  `addu $v0,$s2,$zero` -- a block-local SImode materialisation that local-alloc
  puts in `$v0`. Measured on func_80042BDC: 542 words with no insn, 543 with
  the `andi`, 543 with the `move`, and the `move` is what the target has. Note
  the mask does **not** select the opcode -- the width does. Duplicating into
  both arms (section 1) gives the same structure but makes the pseudo global,
  so it conflicts with the arm's registers and lands in `$a1`.
* `GetRCnt` has to return a signed type for `(x + 0xFF) >> 8` to be `sra`.
* **A `(u16)` cast on a `u32` memory field narrows the LOAD to `lhu`**, given a
  HImode consumer (a halfword store, or a sum destined for one). That is how one
  word can be written with `sw` and read with both `lw` and `lhu` in the same
  function without a union or a split declaration -- func_8003CCB0 does it at
  six sites, and dropping the casts costs 2 differing words.
* **A byte decrement is `addiu 0xFF` only if the arithmetic happens in the
  byte's own type.** For a `u8` lvalue, `x--` builds the constant in that type
  and emits `addiu rd,rs,0xFF`; going through an int-promoted temporary gives
  `addiu rd,rs,-1`, because combine narrows `t + 0xFF` to QImode for the byte
  store and `trunc_int_for_mode` rewrites the constant. `p->f = p->g--;` is the
  spelling that matches (func_8003D0F0, 2 words); a `u8` temp, or a separate
  `s32 u = t + 0xFF;`, works too (func_80045CC4, 2 words).

  **This generalises to HImode, and there the costs INVERT**, because 0xFF is a
  valid `addiu` immediate and 0xFF00 is not. The rule that covers both: for a
  narrow store, **a constant whose masked value is not a valid 16-bit immediate
  has to be applied in its own SImode statement.**

  * `p->unk_102 = (s->unk_18 - 0x100) + <other>;` -- combine's `force_to_mode`
    narrows the whole sum to HImode for the `sh`, recurses into the inner
    `plus` with a 16-bit mask, and rewrites the constant as the raw masked
    65280, which is not `SMALL_INT`. Result: `ori $a1,$zero,0xff00` plus
    `addu`, **two insns**.
  * `h = s->unk_18 - 0x100; p->unk_102 = h + <other>;` -- expand emits the
    subtraction in SImode as its own insn with the constant -256, combine
    cannot fold a three-operand `plus` into the HImode store, and
    `addiu $v1,$v1,-0x100` survives. **One insn**, and the target's
    (func_800453E0, 1 word).

  `p->unk_B8 = p->unk_B8 - 0xA0;` is not a counterexample: there the narrowed
  value is the OUTERMOST operation, so `trunc_int_for_mode` does rewrite 0xFF60
  to -160 and a single `addiu` is right. The distinction is whether the
  constant's `plus` is outermost under the store or nested inside another
  `plus`.

## 8. Statement order and expression shape

* **When a store's `lui %hi` sits one slot too early, give the RHS's first
  subexpression its own statement** -- do not try to move the store.
  `expand_assignment` evaluates the lvalue address before the RHS, so
  `ARRAY[0] = (t >> 7) + ((t >> 10) + K);` puts the store's `lui %hi` ahead of
  the first `sra` and sched1 keeps it there on the LUID tie-break. Splitting out
  `u = (t >> 7);` as its own statement moves the `lui` after the `sra`, puts
  `(t >> 7)` in `$a1` and the sum in `$v0`, and closed func_8003CB08's last 11
  differing words. The `u + ((t >> 10) + K)` grouping is a separate choice worth
  4 more.
* An exit block that should sit physically last wants `break` or `goto done`
  out of the loop with the `return` after it; an inline `return` in the arm
  costs a `j` plus a separate epilogue (func_8004068C, 49 words;
  func_8004491C, 1).
* Chained assignment `a = b = v;` stores `b` first and goes through
  `store_expr` with `want_value`: load to temp, store, copy -- and that copy
  often fills a delay slot. For the two-deep case the direction is pinned:
  `sp->a = sp->b = t;` emits the store to `b` and then the store to `a`, i.e.
  the INNER one first (func_8003CCB0, 2 differing words when inverted; two
  separate statements in listing order are equivalent to the correct chain). A four-deep chain evaluates innermost-first, which
  is how a descending run of stores comes out. Reversing a pair of a chain
  costs 8 words and writing the chain as separate statements 26
  (func_80046884).
* Try both polarities and both spellings of every if/else -- chain versus
  preset-then-nested, and which arm goes out of line. Two clamps in one
  function needed **opposite** spellings (func_80046C20). For a comparison
  chain, `||` of `!=` and `&&` of `==` are not interchangeable (func_80044724,
  49 words).
* When a cluster of statements is close but mis-ordered, **sweep the
  permutations**. func_8003DBD0's four-statement cluster took all 24; the three
  stores in func_80042640 took all 6. It is faster than reasoning about the
  scheduler.
* Writing a constant to memory twice, in two arms, rather than once from a
  variable, is worth 30 words in func_80046E38 -- with one store the jump
  optimizer hoists the assignment above the comparison and the pseudo's live
  range changes class.

**`a < b` and `b > a` are not interchangeable, though they emit the same
`slt`.** Operand evaluation follows the source, so which of the two loads is
emitted first does too. Worth 2 words in func_80042BDC (a byte load that has to
precede a halfword load) and about 2 at each of two sites in func_80045CC4.

**Dropping an intermediate pointer local can be worth a whole register chain.**
`w = base + idx;` then `w[0xD0]` and `w[0xAC]` makes `w` a block-local pseudo
that local-alloc assigns independently, and the chain around it permutes;
writing `base + idx + 0xD0` and `base + idx + 0xAC` out in full lets cse keep
one pseudo, which then coalesces with the value feeding it. 12 of 30 differing
words in func_80042BDC, with the word count unchanged, and hoisting the local to
function scope changes nothing. **This is the opposite direction from section
6's rule** that a large object reached through a pointer local keeps its address
in a callee-saved register: that applies to objects live across calls, while
inside one basic block the local costs you the coalesce.

**A switch on a narrow expression subtracts the minimum BEFORE extending, and
the prologue therefore tells you the switch variable's declared width.**
`c_expand_start_case` calls `get_unwidened` after `default_conversion`, so an
`s16`/`u16`/`s8` switch expression keeps its narrow type and gcc emits
`addiu -min` first and the `sll`/`sra` for the range check second -- the
opposite order from an `int` index, which extends first or not at all. Read
func_80042BDC's prologue, `addiu $a1,$a1,-1` / `sll $a1,$a1,16` /
`sra $v1,$a1,16`, and it says "the second parameter is `s16`" directly.
Measured: `s16` 200 DIFF, `s8` gives `sll 24`/`sra 24` at 202, `s32` emits no
extension at all.

**`cond ? A : B` as a call argument puts the branch AFTER the earlier
arguments; an `if` statement puts it before.** `expand_call` expands arguments
left to right, and a COND_EXPR's jump is emitted only when its own argument is
reached, so the earlier arguments' loads sit above the branch. Written as
`m = B; if (cond) m = A; f(a, b, m);` they sit below it. func_80042BDC selects
between two constants at seven call sites and needs the ternary at two of them
and the `if` form at the other five; all-ternary costs it a word and 105 extra
differing words. The two forms are not always the same length, so this is a
word-count lever as well as an ordering one -- the sites whose call cross-jumps
into a shared tail differ by a word between the two spellings.

* **Tree-level `fold` reassociates a constant onto the other operand of a
  chained subtraction, and splitting the statement defeats it.**
  `p->unk_C0 = p->unk_C0 - 0xA - arg3;` emits `addiu $s0,$s0,0xA` /
  `subu $v0,$v0,$s0` -- the constant moved onto `arg3` -- where the target has
  `addiu $v0,$v0,-0xA` / `subu $v0,$v0,$s0`. Writing the difference into its own
  local (`s32 w = p->unk_C0 - 0xA;`) stops it: RTL `combine` does not
  reassociate, and the three-operand `plus` it would have to build is
  unrecognizable, so the `addiu` survives (func_800453E0, 1 word).

* **A sparse switch's decision-tree SHAPE is a fingerprint of the case LIST,
  including case labels that emit nothing at all.** This is the one lever here
  that lets you recover source the output does not contain.

  `group_case_nodes` merges adjacent case values sharing one code label into a
  single RANGE node; `balance_case_nodes` then bisects the sorted node list with
  `i = (nodes + ranges + 1) / 2` and walks it decrementing 1 per node and 2 per
  range -- **so a range node counts double and pulls the bisect point left.** A
  run of consecutive do-nothing cases is therefore one node that weighs two, and
  when `node_is_bounded` holds (the neighbours' tests already fix both ends) it
  emits no test of its own, just a jump to the default label that jump_optimize
  folds into the surrounding default path. **A tree whose root sits one node to
  the left of where the case count says it should is evidence of a hidden
  contiguous group of case labels**, not of a different pass.

  Measured on func_8003E4FC: the high subtree is {0xD, 0xE, 0x15, 0x16, 0x1B,
  0xFC, 0xFE, 0xFF}; eight single nodes bisect at index 3, giving root 0x16,
  which is what the straightforward source emits -- but the target's root is
  0x15, index 2. Adding `case 0xF: case 0x10: case 0x11: case 0x12: case 0x13:`
  to the do-nothing group makes nine nodes with one range, the bisect moves to
  0x15, and the range contributes zero words. 13 differing words. Any range
  wholly inside 0xF..0x13 gives the same tree; one ending at 0x14 does NOT,
  because 0x14+1 == 0x15 bounds it above as well as below and gcc then emits
  `> 0xE -> right` plus an unconditional jump to the left child instead of the
  target's `< 0xE -> left` and fall-through.

  Corollary worth carrying: `emit_case_nodes` chooses between those two patterns
  on which CHILD is fully bounded -- right bounded gives `> node->high` jumping
  to the right label with the left subtree inline, else left bounded gives
  `< node->low` jumping to the left label with the right subtree inline -- so
  **which test a node emits also tells you about its neighbours.**

## 9. Induction variables

* gcc builds the giv list by **prepending** as it discovers them, so the LAST
  access to an object in the loop body becomes `combine_givs`' representative
  and sets the surviving pointer's base offset. func_80046C20's base is +0x1C
  and not +0x24 because of which store is written last; func_8003F8F8's is
  +0x0A and not +0x08 for the same reason. sched1 may then swap the accesses
  back, so the emitted order can look like the opposite of the source order.
* A source-level pointer increment makes a **biv**, which is what produces two
  induction registers where array indexing gives one (func_80046C20).
* An index expression recomputed per iteration, where the target recomputes it,
  must be written out per use -- `D_80083360[i]` style indexing is not
  interchangeable with a walking pointer.

* **How to get ONE induction register per base instead of two.** A source-level
  `p += STRIDE` makes a biv; if that biv also has a non-address use (passed to a
  call, stored after the loop) it cannot be eliminated, and loop.c
  *additionally* reduces its address givs into a second register -- so one base
  costs two registers. Writing `base[i * STRIDE + k]`, or
  `cc = base + i * STRIDE;` with constant displacements off `cc`, creates no biv
  for that base at all, just one reduced giv register. In func_8003CCB0 that is
  the difference between four induction registers -- which pushed the incoming
  parameter out of a register, spilling it to its stack home and reloading it
  every iteration -- and two. Measured at 2 words and 246 differing per base.

* **The surviving register's base offset is the REPRESENTATIVE giv's `add_val`,
  not the last field's offset.** When all the field accesses share one
  `&base[i]` subexpression the representative is that base address. So to
  reproduce a target's `$s3 = SYM + 4` with displacements -2..+7 you must bias
  the base pointer (`c = (u8 *) SYM + 4;`), not the field offsets:
  `recs = (Rec *) SYM` with `recs[i].field` gives base +0 and every displacement
  4 higher -- the same code shape, 20-odd differing words. The tell is the
  `.loop` dump's `giv reg N ... mult 12 add (reg/v:SI 84)`: `add` being a REG
  means the base is whatever that pointer holds. This refines the entry above,
  which says the representative is the last nonzero-offset access -- that holds
  when the accesses do not share a base subexpression.

## 10. Measured dead ends -- do not retry

* **`volatile` as a nudge** -- reaching for it hoping the shape shifts. Tried
  that way in func_80042900, func_80042A80, func_8003DBD0 and func_8003D0F0: it
  changes the output but never toward the target, and it usually adds a word.
  **But it is NOT a dead end where it has an exact mechanism**: against
  `combine`'s byte-to-signed refold it is the answer, and it matched
  func_80044D24. See section 7. The distinction worth carrying is whether you
  can name the pass and the predicate it defeats.
* **`combine` proving a bit.** `(s16) (x & 0x400)` comes out `srl`, never
  `sra`, because the mask proves bit 15 clear through `nonzero_bits`.

  **One thing does defeat it, and it costs a word.** `nonzero_bits` ORs its
  value and MINs `sign_bit_copies` over **every** set of the pseudo
  (`set_nonzero_bits_and_sign_copies`), so more sets do not help by themselves
  -- what helps is a set the accumulation cannot prove anything about: an
  opaque one, or a constant with bit 15 set (measured, 2026-10-22):

      s32 G(u16 *p, s32 c)         { s32 v = 0;      if (c) v = *p & 0x400; return (s16) v; }  /* srl */
      s32 E(u16 *p, s32 c, s32 w)  { s32 v = w;      if (c) v = *p & 0x400; return (s16) v; }  /* sra */
      s32 F(u16 *p, s32 c, s32 *q) { s32 v = *q;     if (c) v = *p & 0x400; return (s16) v; }  /* sra */
      s32 H(u16 *p, s32 c)         { s32 v = 0x8000; if (c) v = *p & 0x400; return (s16) v; }  /* sra */

  But **every** multi-set shape also emits an `andi 0xffff`, because combine
  cannot narrow both sets to HImode and inserts the truncation on one path
  instead. So this buys `sra` at the price of one word, which is no help where
  the word count is already exact -- func_80042A80 is 58 against 58. Use it
  where a spare word exists; where it does not, the target's value was
  single-set and something else is going on. Measured against, for the
  single-set case: every
  spelling of the cast, a second use of the masked value, a second use of the
  loaded halfword, the mask in a different basic block, the value tested against
  0 first, a union, a `volatile` load, and a function returning `s16`. Two of
  those also add an `andi 0xffff` the target does not have. `andi 0xC000` and
  `ori 0x400` do give `sra`, which confirms it is the proof and not the
  spelling. This blocks func_80042A80 and is the one difference in it.
* **`cse` folding a known constant, inside one run.** `p->field = 0x10;` then
  `field * 2` in a following block is always `li 32`, never `sll`. Measured
  against: the field read back, an `s32` local, a `u8` local, a `u16`/`s16`
  local (PROMOTE_MODE -- see section 7), the local given a second use or a real
  second set, the value derived as `other >> 4`, the local also feeding another
  field, a `(u8)` cast on the product, `<<= 1`, the `if` inverted, and the two
  entry stores swapped. `rtx_cost` is why the constant *wins* -- a `CONST_INT`
  costs ~0 and MIPS cannot store an immediate, so expand always materialises it
  into a register before the store and hands cse the equivalence.

  **But the thing to attack is the run, not the cost**, and that is now proved
  rather than guessed. cse's run ends only at a `CODE_LABEL` with
  `LABEL_NUSES != 1`; it follows a conditional jump's fall-through *and* its
  target, and it crosses a barrier and a single-use label. Four probes: body as
  fall-through, body as a `bne` target, body behind an unconditional jump plus
  barrier -- all fold; a body label with **two** uses -- the `sll` survives.
  `-fno-cse-follow-jumps`, `-fno-cse-skip-blocks`, `-fno-thread-jumps` and
  `-fno-rerun-cse-after-loop` change nothing, so this is not a flag question.

  Three further walls found while trying to buy that two-use label, each worth
  knowing in its own right:
    - **`thread_jumps` runs inside jump1, before cse**, so a *provably*
      redundant second predecessor is already gone: three equivalent tests
      OR-ed together compile to one `beqz` with the body as fall-through, and
      still `li 32`.
    - **jump1 also does dead-pseudo-set DCE**, so a dead assignment cannot hold
      an arm open -- the set, the jump and the label are all in `.rtl` and all
      gone in `.jump`.
    - **cse deletes a redundant store of the same value to the same address**,
      so a duplicated store cannot hold an arm open either.

  So func_80041E70 is blocked, and the honest reason is a chain rather than a
  cost: cse's boundary is a two-use label; a two-use label is also a scheduler
  block boundary; the target's first block provably has none (its prologue is
  interleaved with a store and a constant fills a load-delay slot, both of
  which only a scheduler does, and schedulers do not cross labels); and the one
  zero-word way to make a run boundary -- identical `if`/`else` arms, section 1
  -- is a scheduler boundary too. A shape with an opaque third test does
  reproduce the body byte-for-byte at +2 words, which is the proof that the
  rest of the function is right.
* **`sched1`'s hazard heuristic.** It is a pure function of the insn and the
  unit state; no statement order changes it. Move the basic-block boundary
  instead (section 1).
* **`local-alloc`'s choices.** A block-local quantity -- a switch index, a
  value used only inside one arm -- is assigned before `global-alloc` runs, and
  nothing in the source reaches it. This is what is left in func_80041344.
* **A call's argument preferences -- but this entry is now only half true, and
  the two functions parked on it deserve re-examination.** The propagation is
  real and cannot be broken by any spelling found (see section 3), but
  `regs_someone_prefers` collects only *lower-priority* conflicting allocnos'
  preferences, so allocation order decides whose preference wins -- and order is
  reachable through section 3's priority levers. func_80041344 went from 24
  differing words to 2 on exactly that. **func_800407C0 and func_80047054 are
  parked as unreachable because of this entry**, on five-fold and 2.5-fold
  priority gaps; those gaps may still be too wide, but the reason recorded for
  them is weaker than it was. The original claim follows. An allocno that feeds
  a call argument carries a preference for that register, `find_reg` avoids it
  for conflicting allocnos, and allocnos crossing the call have their own
  preferences pruned by `call_used_reg_set`. The whole assignment shifts around
  the call and no
  spelling moves it. This is what makes func_800407C0 unreachable.
* **decomp-permuter** cannot score a function that uses a constant address:
  splat's `target.o` carries an `R_MIPS_LO16` relocation the constant form
  cannot produce, so the score never reaches zero.

## 11. Toolchain traps

* **`make check` is the only authority.** `verify.py` compares a relocated word
  on its upper 16 bits only; it now marks a differing `%lo` addend with `?`, but
  it still cannot see a jump table landing at the wrong address.
  **A `?` on an INTERIOR array address is the expected output, not a
  difference.** Spelling an interior address through its container --
  `D_80083858[i * 2 + 2]` -- emits `%hi`/`%lo(D_80083858)+4` where the listing
  prints `D_8008385C`; both `%hi` are the same and `%lo(X)+4 == %lo(X+4)`, so
  the linked bytes are identical. There are three instances now (D_80083164,
  D_80083858/D_8008385C and D_800838D8/D_800838DC), and the container spelling
  is what keeps the symbol out of the `-G8` hazard, so the flag is the price of
  doing it right.
* **wine's filesystem is case-insensitive.** Two scratch files whose names
  differ only in case are one file to `ccpsx`, which then exits 0 having written
  the other one. `verify.py` diagnoses this now; name variants so they differ by
  more than case.
* **Concurrent `ccpsx` runs share one wine prefix** and can lose an output.
  Retry before believing a compile failure.
  Set `AZURE_WINEPREFIX` to a private copy of `~/.cache/azure-psyq` when more
  than one line of work is compiling at once; `tools/ccpsx` honours it.
* **maspsx drops the load-delay nop before a `%gp_rel` STORE of a symbol it
  knows only from an `.extern` record**, which costs exactly one word per site
  in a `-G8`/`-G2` unit and throws off every later branch offset. Its
  `_uses_gp` reads `.sdata`/`.comm`/`.lcomm` and explicitly skips `.extern`
  (`in_sdata = False; continue`), so it concludes there is no `$at` expansion
  and no real nop -- while gas, left at a nonzero `-G`, turns the small extern
  into a one-insn `%gp_rel` access anyway. gcc's own output has the `#nop` in
  the right place; maspsx is what loses it. The in-source workaround, which is
  the `.sdata` recipe minus the definition:

      __asm__(".sdata\n\t.size D_80080ADC,4\n\t.text");   /* file scope */

  That puts the symbol in maspsx's `sdata_entries`, so maspsx does the
  `%gp_rel` rewrite itself *and* emits the nop, while defining nothing --
  `readelf -sS` shows `.sdata` at 0 bytes and the symbol still `UND` with size
  4. Unlike annotate.py's gp-relative lever there is no `.globl`, no `.type`,
  no label and no `.word`, so there is no linker placement to arrange and
  nothing to collide with the linker script's own assignment. Worth one word in
  func_8003E4FC (131 against 132, with 59 differing). The principled fix is in
  `vendor/maspsx`: record `.extern SYM,size` and let `_uses_gp` consider those
  symbols when gas is left at a nonzero `-G`, gated on `--dont-force-G0` so the
  `-G0` bulk of the game is unaffected.
* **`cc1psx` output has CRLF line endings.** Strip the CR before matching
  labels.
* **`make splat` does `rm -rf asm/nonmatchings` first**, so never run it while
  anything else is reading those listings.
* **Do not pipe `make check` through `head` or `tail`** -- it masks make's exit
  code, and a broken state was committed that way once.

## 12. Keeping an agent's run cheap

A subagent working one function pays for its whole context on every turn, so
tool output compounds. Two things dominate:

**The recipe, in order.** Do not start by reading this document end to end:

    python3 tools/decomp/annotate.py <unit> <func>        # which levers can apply
    ... write a candidate with the right LOGIC ...
    python3 tools/decomp/search.py zz_foo.c <unit> <func> --only=<families>

`annotate.py` enumerates the lever families whose signature is in the listing;
`search.py` sweeps the source shapes mechanically at 0.7s a variant. The two
compose -- the first decides *where* to look, the second explores the choices --
and between them they replace the loop of "write a variant, read the diff,
reason, write another" that costs a model turn per variant. What neither does
is derive the logic: that is the irreducible part, and a structurally wrong
candidate will not be rescued by any combination of rewrites.

Pass `--only` once a function is large. `search.py` confines its transforms to
the function body, but a 437-word function still offers ~74 candidate changes
and one greedy pass compiles every one of them, so narrow to the families
`annotate.py` implicated.

* **This document, read end to end.** It is 9,000 words, every agent was told
  to read all of it, and most of it cannot apply to any one function. Run
  `python3 tools/decomp/annotate.py <unit> <func>` instead: it matches the
  listing against `tools/decomp/levers.yaml` and prints only the entries whose
  signature is actually present, with their costs and their ruled-out shapes.
  On func_8003D0F0 it emits, in a tenth of a second, the same HImode-consumer
  paragraph that was hand-written into a brief and closed that function first
  try after two sessions of being parked.
* **`verify.py` output.** The full side-by-side of a 437-word function is about
  10k tokens, and an iterating agent pays that again on every subsequent turn.
  It is abbreviated by default now -- differing words plus two of context, runs
  of matches collapsed -- and `--full` is there for when the word counts differ
  and you need to see the whole structure. A MATCH prints four lines.
* **Inherited context.** A `fork` subagent starts with the parent's entire
  conversation and is charged for it every turn. Now that this document exists,
  the compiler knowledge no longer has to come from the parent's context, so a
  fresh agent told to read it is the cheaper dispatch. Reserve forks for cases
  where the parent has state that is genuinely not written down.

Also: grep the reference listing and the dumps rather than reading them whole
(`grep 'Register 8' .../lreg`, `sed -n` a range of the `.s`), and do not re-read
a file you have already seen.

**Measured costs, so a dispatch can be budgeted rather than guessed** (one
function each, fresh general-purpose agents told to read this document):

| function | outcome | tokens | tool calls |
|----------|---------|--------|------------|
| func_80042560 | MATCH | 130K | 25 |
| func_80044D24 | MATCH | 212K | 47 |
| func_8003D0F0 | MATCH | 230K | 57 |
| func_80045CC4 | 436 of 437 | 316K | 85 |
| func_80041588 | 164 of 164, 21 DIFF | 316K | 82 |
| a fork of the main session | one function | ~890K | 326 |

The pattern: a function whose mechanism is already named costs 130-230K and
lands; an open-ended one costs about 315K and comes back short but with
measurements. Either is far cheaper than a fork, which pays for the parent's
whole conversation on every turn.

**Batch functions that share a mechanism.** Ten of the `$gp`-blocked functions,
all needing the same `.sdata` definition recipe, came back all-MATCH in one run
of 152K tokens and 52 tool calls -- about 15K per function, an order of
magnitude under the per-function column. Eight matched on the first compile,
and the second instance of each shape cost one tool call. The recipe was handed
over verbatim in the brief, which is the same lesson as below, applied to a
group.

Two things that make the difference between those columns:

* **Tell the agent the mechanism you think it needs, in full, in the prompt.**
  func_8003D0F0 had been parked for two sessions; the agent that landed it was
  sent one paragraph naming the HImode consumer and closed the gap first try.
  A brief that just says "finish this function" buys the 315K column.
* **Resume an agent with `SendMessage` instead of spawning a fresh one.**
  Everything it measured is in its context, and a new agent re-derives it.
  Killing a long-running agent throws all of it away for nothing -- asking it
  to report what it has now caps its remaining turns and keeps the
  measurements.
