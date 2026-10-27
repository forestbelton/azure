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
| `cse` | substitutes the cheapest equivalent per operand, then constant-folds | the gate is `COST(replacement) < COST(operand in place)` (cse.c:5246), and `COST` is **1 for any pseudo** against `rtx_cost(X,SET)*2` for a constant (cse.c:481). MIPS's `CONST_COSTS` returns **0 for every `CONST_INT`, whatever its magnitude** -- the macro says so, because MIPS has no different-sized instructions -- so a constant equivalent **always** displaces a pseudo operand, and `0x10 * 2` becomes `li 32`. **There is therefore no "make the constant too expensive to win" lever on this target**; do not look for one. What decides whether cse knows the constant at all is its **run boundary**, below |
| | the extent of one run | a run is a maximal **linear** insn range that stops only at a `CODE_LABEL` with `LABEL_NUSES != 1`. It does **not** stop at a conditional jump, a barrier, or a single-use label -- so an `if` body inherits the test's table whether it is the fall-through or the branch target |
| | memory equivalences | invalidation is **coarse**: a store through a register base kills entries at other offsets off the same base, which is why the target reloads a neighbouring field |
| `loop` | invariant motion | hoists a constant's set into the preheader, where `combine` can no longer fold it into an immediate (section 3) |
| | strength reduction | builds the giv list by **prepending**, so the *last* access in the body becomes `combine_givs`' representative (section 9) |
| `flow` | `reg_n_refs`, `reg_live_length` | refs are weighted by `loop_depth`, so a loop note reweights the whole function (section 2). Both are recomputed after `sched1`. |
| `combine` | substitutes an insn into its single user | blocked by a **second use**, by an intervening memory store (`use_crosses_set_p` / `mem_last_set`), and by basic-block boundaries. This is the most reachable pass from C. |
| | proves bits via `nonzero_bits` | **not** reachable from C -- see the dead ends |
| `sched1` | insn order **within one basic block** | priority, then a **dependence class** relative to the last-scheduled insn, and only then `LUID` (source order) as a stable tie-break -- so a dependency-free insn outranks a dependent one *before* source order is consulted, which is why a free address computation floats to the top of its block. Alias calls go through `MEM_IN_STRUCT_P` (section 5); a hazard heuristic reorders stores and cannot be steered |
| `local-alloc` | registers for block-local quantities, **before** `global-alloc` | a value live in one block only is out of reach of `global-alloc`'s levers, and it takes the register `global-alloc` would have wanted. But it has its OWN preference system -- see the suggestion phase below -- so "it just takes the first free register" is true only of an *unsuggested* quantity |
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
  constant fold at an unchanged 270 words.

  **But it is ONLY reliably a cse boundary -- do not count on it as a sched1 or
  allocation boundary, which an earlier version of this entry claimed.** An
  INSERTED identical-arms `if` has a redundant branch, and the `jump` pass that
  runs immediately after cse deletes it, so by sched1 there is no block split
  left. Measured on func_8003CCB0: four placements of
  `if (r != 0) { rr[3] = -4; } else { rr[3] = -4; }` inside a loop each gave
  272 words and 29 DIFF with the generated `.s` **byte-identical** to the
  baseline apart from label numbering -- no extra reference, no live-range
  extension, no boundary. The cse boundary still works because cse itself runs
  before that jump pass.

  **The distinction that matters is whether the branch is needed for its own
  sake.** Section 3's duplicate-an-insn lever duplicates a statement into both
  arms of an **existing** `if` whose condition is real: the branch survives to
  allocation, the pseudo genuinely gains references, and jump2 merges only the
  duplicated tail afterwards -- which is how func_800423C0 and func_80045CC4
  were closed. Inserting a new `if` purely to make a boundary does not buy that.
  **AND THE INVERSE IS A LEVER TOO, worth 126 words on func_80045CC4.** Because
  `reg_n_refs` is counted by `flow` and jump2 runs long after it, an `if`/`else`
  whose two arms both store to **the same field** charges the base pseudo two
  loop-weighted references for a store that exists only once in the output. That
  is a phantom reference at global-alloc time, and it can hand the race to the
  wrong pseudo. Spelling the same thing as a single store -- a ternary, or a
  temp set in both arms and stored once after the join, which is byte-identical
  -- removes it.

  On func_80045CC4 the if/else form gave the `q+4` giv 77 refs and
  `6*77/320 = 1.4438` against the scratchpad pointer's `7*148/726 = 1.4270`, so
  the giv was allocated first and `$s0`/`$s1` were exchanged across the whole
  function. One store drops the giv to 75 and `6*75/319 = 1.4107` against
  `7*148/724 = 1.4309`, and the scratchpad goes first, as the target has it.
  Note one reference was enough to flip it, and 75 and 77 are in the same
  `floor_log2` bucket, so this is a pure ref-count effect.

  **It is site-specific, so measure rather than apply it everywhere.** The same
  change at that function's other two-arm `q` store costs 6 words, because the
  target keeps that site's two `sh` in branch delay slots -- they must stay two
  stores. The lever applies only where the target itself cross-jumped the arms,
  which the listing tells you.

  So: use an existing branch to raise a reference count, **remove a two-arm
  store to shed a phantom one**, use inserted identical arms only for a cse run
  boundary, and **check the `.s` before relying on
  either for a schedule.**

  The other caveat is the price where it does apply: it is a real sched1 and
  sched2 split wherever the branch survives, so it only helps where the
  surrounding schedule can tolerate one. In func_80041E70 the body came out
  byte-exact and the first block then lost two words to scheduling.
  Arms holding a different insn, or placed a statement earlier or later, cost
  1-3 words -- the placement sweep is in its near-miss file.

  And it can be used to **prevent** a merge. Duplicate a run of insns into both
  arms whose *last* insn differs between them, and cross-jumping merges only
  the common part, leaving the differing insn in the arm where sched1 put it.
  func_80045CC4 needs its two halfword stores inside both arms so that each
  arm's own `subu`/`addu` stays last and lands in the branch's delay slot;
  without it, jump2 pulls all four stores into the join. Worth 4 words.

* **`reorg` STEALS a delay-slot insn or COPIES it, and `own_thread_p` decides
  which -- a copy leaves the original behind and costs a word.** It fails when
  the thread is reachable by fall-through: it scans back from the thread and
  must reach a BARRIER before any insn or label. So when the target emits a
  shared epilogue insn ONCE and you emit it twice, the fix is to give that insn
  its own thread rather than to move it. In func_80042900 that meant letting the
  0xD arm fall off the end of the function while the `else` holds the
  `return 0`: gcc lays out `bne -> L_else` / `lw` / `jump L_end` / barrier /
  `L_else: v0 = 0` / `L_end:`, `own_thread_p` holds, reorg steals the
  `addu $v0,$zero,$zero` and deletes the original, and the arm's jump-to-next
  goes away. Written the other way round -- the 0xD arm owning the only
  `return 0` -- the insn is copied: 34 words against 33. Note the winning form
  is formally UB (falling off the end of a value-returning function) and the
  emitted code returns 0 anyway, because the stolen insn is in a non-annulled
  delay slot and runs on both paths. A target with one such insn where two
  paths need it is evidence the original was written that way.

  **The converse is the diagnostic, and it bounds this lever: if the shared insn
  STILL EXISTS as its own instruction in the listing, with copies in delay slots
  elsewhere, then reorg COPIED and the thread was not owned -- and that is what
  the target wants.** Do not reach for `own_thread_p` there. func_8003F368 is
  the counterexample: its `addu $v0,$zero,$zero` survives at 0x8003F50C with
  three copies in slots, and the func_80042900 shape costs it 4 words (109
  against 113). Its remaining gap is the other half of the same decision --
  reorg declining ONE of four identical opportunities, which needs
  `insn_sets_resource_p (trial, &opposite_needed)` to reject the trial, i.e.
  `mark_target_live_regs` reporting the register live at that branch's
  fall-through, reachable only through `find_basic_block` returning -1. That is
  block-entry layout, and it costs a word to produce from C. **So the lever is
  which way the layout goes, and the listing tells you which way to want: a
  surviving original means copied, an absent one means stolen.**

  **Verified against 2.7.2's reorg.c, 2026-10-23.** The claim above was written
  from inference and is correct; only the identifier was misspelled
  (`opposite_needed`, not `opposite_needs`). `find_basic_block` does return -1
  as well as 0, and `mark_target_live_regs` takes that path: it can only use its
  cached liveness when `b != -1`, so a -1 forces a recompute. The
  `new_thread != thread` gate on `reorg_redirect_jump` is there too.

  **AND THE SAME CONDITION HAS A SECOND DISJUNCT THAT IS A LEVER IN ITS OWN
  RIGHT.** The full test is

      condition == const_true_rtx
      || (! insn_sets_resource_p (trial, &opposite_needed, 1)
          && ! may_trap_p (pat))

  and the trial is taken only when that holds. So for a **conditional** branch
  -- where `condition` is not `const_true_rtx` -- **an insn that may trap can
  never be stolen into the delay slot.** `may_trap_p` returns 1 for a memory
  reference unless it is a static variable or a stack slot (its own comment
  says so), and 0 for anything built from REG, CONST_INT, SYMBOL_REF and
  friends.

  That is a general tool for controlling delay-slot fills, and it cuts both
  ways: a register-or-constant computation at a branch target is stealable, a
  load through a pointer is not. When the target did NOT fill a slot it could
  have, check whether the insn it declined reads memory through a pointer
  before reaching for block layout. Note the escape does not help
  func_8003F368's own gap, because the block reorg steals from there is a
  `return 0` -- a constant, so `may_trap_p` is 0 -- and making it may-trap
  would mean returning a value loaded from memory, which changes what the
  function does.

**AN EMPTY DELAY SLOT BEFORE AN INSN THAT SETS `$v0` MEANS THE RETURN TYPE IS
NOT `void`.** This is the cheapest delay-slot lever found so far and it costs
one character. func_80058510's second guard is

    bnez  $v0, <exit>
     nop
    addiu $v0, $zero, 0x1          <- the fall-through's first insn

and a `void` function fills that slot with the `addiu`. Filling it is legal --
the slot runs after the branch is resolved, and on the taken path the register
is dead -- but only while it IS dead. Declare the function `s32` and never
return a value, and `$v0` is live out at the epilogue, so reorg cannot move an
insn that sets it across the branch and the `nop` stays. One word, and
func_80058510 went from 35 of 36 with 27 differing words to MATCH on nothing
else.

A SCAN over all the remaining stubs' listings found 22 with this signature --
an empty conditional-branch slot followed by an insn that writes `$v0` -- and
in 17 of them that insn is a LOAD, which the `may_trap_p` rule above already
explains, so they are not candidates. Of the five whose insn is ALU
(func_800345B8, func_800407C0, func_8004D294, func_800500B4, func_8005C2C0)
every one is already at the exact word count, i.e. the slot is empty in our
build too. And func_8003800C, the only `void` park that is a word SHORT, does
not respond to the change. So the lever is narrow: the signature to act on is
our build FILLING a slot the target leaves empty, with an ALU insn, in a
function declared `void`.

Read it the other way too: a function whose listing fills such a slot IS
`void`. And the giveaway for this shape is a function that never sets `$v0` on
any path yet leaves a slot empty before an insn that does -- the original
declared a return type it did not use, which is ordinary enough in this
codebase (func_800564A8 takes an argument it ignores, func_800552C8 the same).
The sweep row that preceded this guessed at reorg's `own_thread_p`; the answer
is liveness, which is also why no arrangement of the three blocks reached it.

## 2. Loop notes change every reference weight in the function

**A backward `goto` is not a loop to loop.c at all, and that is a
register-pressure lever rather than a style choice.** (It is also the way to
turn STRENGTH REDUCTION OFF outright: func_8003C67C's copy loop is reduced to
two pointers walked by -8 as a `while`, and as a guarded backward `goto` it
recomputes the index shift every iteration instead, which is what that
listing wants. A sweep row there had blamed loop.c's `loop_has_call`
threshold; the loop notes are the whole of it. It paid again the same day on
func_8004E9EC, where the `while` form gives the cursor a SECOND, biased giv
and the goto form leaves it one pointer: 34 words to 32. **`search.py
--only=goto_loop` now sweeps this**, rewriting a `while` or `for` as a guarded
backward goto with the body in its own block -- so a body that starts with
declarations stays legal -- and skipping any condition with a side effect,
since the condition is duplicated.) func_8003F368 repeats its
whole body on one error code. Written as `for (;;) { ... }` with a `break`,
loop.c sees a real outer loop and hoists an inner loop's invariant `li 1` past
the OUTER back edge, so that pseudo is live across the second half of the body,
conflicts with a pointer local, and the function needs a fourth callee-saved
register: **115 words, 112 differing**. Written as a label plus `goto restart`,
loop.c sees only the two inner loops, `move_movables` hoists the `li 1` into the
INNER loop's preheader -- which is inside the goto cycle, so it is
re-initialised on the back edge -- and its live range no longer overlaps the
pointer's. One register carries both: 113 words. Same source logic, two words
and the whole allocation.

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

**loop.c hoists an invariant when `threshold * savings * lifetime >=
insn_count`. "Set first" and "only one" are both artifacts of that test, not
rules.** The gate is in `move_movables` (2.7.2 loop.c:1631):

    already_moved[regno]
    || (threshold * savings * m->lifetime) >= insn_count
    || (m->forces && m->forces->done && n_times_used[m->forces->regno] == 1)

and its inputs are `m->savings = n_times_used[regno]` (loop.c:793) -- which is
**the number of SETS, not uses**: `n_times_used` is initialised by `bcopy` from
`n_times_set` (loop.c:597), so for any single-set pseudo it is 1 and **a second
use of the constant buys nothing**; `m->lifetime =
uid_luid[regno_last_uid] - uid_luid[regno_first_uid]`, its live span in logical
insn numbers, computed function-wide rather than clamped to the loop (791-792);
`insn_count`, the loop's insn count, doubled for a repeat move of the same
regno (1609-1611); and `threshold = (loop_has_call ? 1 : 2) * (1 +
n_non_fixed_regs)` (532) -- so a call inside the loop HALVES the threshold.
**`threshold` is then decremented by 3 after every successful move**
(loop.c:1718, commented "the more regs we move, the less we like moving them"),
which is the actual reason a second invariant usually fails: it is tested
against a smaller threshold than the first. On this target the starting value is
**29** (soft-float fixes the FP regs via `CONDITIONAL_REGISTER_USAGE`, leaving 25
GPRs plus MD regs 64-66; these loops contain calls, so the multiplier is 1), and
the second constant is therefore tested at 26.
`move_movables` loops over every movable and hoists each that passes
independently, so there is no one-hoist limit; the second constant is simply
failing the test. And the constant set first has the earlier `regno_first_uid`,
hence the larger `lifetime`, hence the larger product -- which is why "set
first" looked like the rule. `combine_movables` also ADDS the lifetimes and
savings of movables it matches (1222-1223, 1282-1283, 1330-1331), though equal
value and mode are required, so two different mask constants will not combine.

**Read the numbers rather than inferring them.** The `.loop` dump prints all
three: the loop header line gives `insn_count` ("Loop from 64 to 137: 23 real
insns"), and each movable prints as "Insn 42: regno 94 (life 1), move-insn
savings 1  moved to 176", where the trailing "moved to" appears only if it was
actually hoisted. `tools/decomp/dump.sh` already collects it.

So the only movable input is `lifetime`, and the way to move it is not to touch
the constant at all. **What actually solved both mask-tension functions was
recognising the statement as a BITFIELD STORE.** `addPrim` writes a 24-bit
field, so expmed.c's `store_fixed_bit_field` expands the VALUE first (load
`*ot`, set 0xFFFFFF, mask), then the lvalue's clear-mask (load `q->unk_00`, set
0xFF000000, mask), then `ior` with **the q side as operand 0**. That hands over
both things a hand-written `|` can never give together -- 0xFFFFFF gets the
earlier set and so the longer lifetime, clearing the threshold at 29, while
`q->unk_00 & 0xFF000000` still lands as the `or`'s operand 0 -- because
`expand_binop` always materialises operand 0's chain first. sched1 is a reverse
list scheduler (sched.c:46) and orders the loads and `and`s from that operand
order, which is where the target's q-load-first comes from.

The general lesson, and the reason this sat unsolved through dozens of operand
spellings: **when two requirements on one statement are in direct conflict from
C, check whether the original statement was not that C operator at all.** Here
no spelling of `|` could satisfy both; a bitfield assignment satisfies both by
construction.

The original measurement that produced the superseded claim still stands: Measured on func_80045CC4, which has
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

### Giv EMISSION is the REVERSE of creation; a second pointer at offset ZERO reorders it

Two strength-reduced pointers in one loop come out in an order the source
cannot state directly, because `record_giv` PREPENDS to the biv's giv list and
the reduction loop (loop.c:3856) walks that list: the initialisations before
`loop_start` and the increments before the biv's own increment are both emitted
in LIST order, so **emission is the reverse of creation, and creation is insn
order**. The pointer whose row address is the body's first statement is
therefore initialised and incremented LAST.

Worse, you cannot fix it by moving a reference earlier. `combine_givs`
(loop.c:5494) merges every giv in a group -- same mult, same add -- into one,
and the representative is the first eligible `g1` in the list, i.e. the group's
LAST-created member. So an earlier reference is simply combined INTO the later
one and loses its place. In func_80057D20's inner loop the other array's group
is represented by the copy `move_movables` hoists out of an inner do/while,
which the loop pass creates last of all, because loops are processed innermost
first -- nothing written in the body can be created after it.

**The lever is a SECOND pointer to the same row, at offset ZERO, declared
after that inner loop:**

```c
u8 *r = D_80085458 + i * 0x78;            /* conditions read through r */
if (...) {
    f2 |= D_80073740[i];
    do { ... D_80073740[i] ... } while (...);
    {
        u8 *r2 = D_80085458 + i * 0x78;   /* same address, created LAST */

        r2[0x1D] = 0;
        *(u16 *) (r2 + 0x1A) = 0;
    }
}
```

`r2`'s giv is created after the hoisted copy, so it heads the list and
`combine_givs` makes it the group's representative -- which swaps the two
emission orders. And because its add_val is the PLAIN BASE, the register stays
unbiased: that is the whole reason for offset zero. It costs nothing, since
`r` and `r2` are one group and one register. Six words and the last of
func_80057D20's 463.

The alternatives all fail on the bias. A source-level walked pointer (`r =
D_80085458; r += 0x78;`) does make the group's last member late, but that
member is the `+0x1A` store, so the pointer comes out biased to +0x1A with
negative displacements (13 DIFF). Respelling the trailing stores as fresh
`D_80085458[i * 0x78 + 0x1D]` index expressions is the same trap at +0x1D, and
writing the leading conditions inline so `r` is not computed first costs 14
words, because cse does not share the index chain.

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

  **AND THERE IS A SECOND ESCAPE, which is cheaper than moving a priority and
  which this project has never used.** `prune_preferences` (2.7.2 global.c, the
  loop at ~871) builds `regs_someone_prefers[A]` by walking the *lower*-priority
  conflicting allocnos and OR-ing in their `hard_reg_full_preferences` -- but
  first it SUBTRACTS A's own full preferences, and the source comment says why:
  if A also prefers one of those registers, it is not excluded. The subtraction
  is skipped only when the rival's `allocno_size` exceeds A's, so for two
  same-mode allocnos it always applies.

  So to stop A being pushed off register R, you do not have to reprice anybody:
  **make A prefer R itself.** A hard-register preference comes from a copy
  relating the pseudo to that hard register -- being the incoming parameter in
  it, being the call return value in it, or being copied into it as an outgoing
  argument -- which `set_preference` records. This inverts where to look: when
  allocno X is pushed off R by Y's preference, the fix is on **X's** side, not
  Y's, and every "no spelling of Y changes its preference" result is therefore
  beside the point.

  **The other half of `prune_preferences` explains the call-crossing cases.**
  Before that loop it takes the allocno's conflict set and, for an allocno that
  crosses a call, OR-s in `call_used_reg_set`, then `AND_COMPL`s all three
  preference sets with the result. That is the mechanism behind "the parameters'
  preferences are pruned by the call": any preference for a caller-saved
  register is deleted outright from an allocno live across a call. It is not a
  tie-break and no source shape softens it -- but note it prunes only the
  *preference*, so the self-preference escape above is unavailable precisely for
  allocnos that cross a call, and available for those that do not.

  **LOCAL-ALLOC HAS ITS OWN PREFERENCE SYSTEM, and the docs here had none.**
  `block_alloc` allocates in TWO phases (2.7.2 local-alloc.c, ~1463-1540):

  1. Sort by `qty_sugg_compare_1`, then give **every quantity that carries a
     suggestion** its suggested register -- `qty_phys_copy_sugg` (from a copy)
     is tried before `qty_phys_sugg` (arithmetic). These jump the queue, and the
     source comment says this is accepted in local allocation precisely because
     it may allocate registers that otherwise would not be.
  2. Sort what is left by *decreasing life length* (`qty_compare_1`) and give
     each the first register `find_free_reg` finds.

  So the rule recorded elsewhere -- a block-local quantity takes the first
  entry of `REG_ALLOC_ORDER` -- describes phase 2 only. A suggested quantity
  does not behave that way at all.

  **Where suggestions come from, and the trap in it.** `combine_regs` records
  them when a copy relates a pseudo to a hard register. But when the hard reg
  is the SOURCE and a pseudo the destination -- an incoming parameter -- it
  deliberately does **not** tie them: it records the hard reg in
  `qty_phys_sugg` and returns failure *on purpose*, so the hard register's
  lifespan terminates at that insn and nothing blocks the pseudo from later
  getting it. Do not expect an incoming parameter to hold its argument register
  by being tied to it; it holds it, when it does, by suggestion.

  **`find_free_reg`'s exclusion set** is every register live at any point from
  `born_index` to `dead_index`, plus `call_used_reg_set` if the quantity
  crosses a call (`fixed_reg_set` if it does not) -- the same call distinction
  `global.c` makes. So to keep a block-local quantity *off* a register, either
  something must be live in it across that quantity's whole lifetime, or the
  quantity must be suggested elsewhere.

  **The consequence worth checking first: no calls means no suggestions.** A
  function with no `jal` has no hard-register copies except its incoming
  parameters, so almost every block-local quantity in it goes through phase 2
  and the suggestion lever does not exist. func_80047054 has **zero** calls,
  which is why nothing moves its `p[0]` load off `$a0`.

  **But "the function has calls" is NOT enough -- corrected 2026-10-23 after
  testing it on both candidates and getting nothing.** The test is not whether
  the function contains a `jal`; it is whether *the quantity you need to move*
  can be made to flow through a hard register, which from C means being a call
  argument, a call's return value, or a returned value. A function can be full
  of calls and still offer no route to the one quantity that matters:

  * func_8004437C (6 calls): the quantity is the `%hi` half of
    `D_80080AF4[D_80080AFC]`'s address. That address is never an argument and
    never a return value -- the function's only hard-register copies are its
    two parameters and the arguments of its calls (`6`, the `D_8006E704`
    pointer, `0`, `arg1`, the `0x22`/`0x21` constant). No suggestion can reach
    it without changing what the function does.
  * func_8003F368 (12 calls): the quantity is a comparison RESULT, which is
    not a copy at all, so `combine_regs` never looks at it. Four spellings
    aimed at giving the function's return value a `$v0`-suggested pseudo that
    outlives the third comparison were measured and all are worse than the
    baseline -- see that file.

  So the lever is real and narrower than it first looks: it needs a quantity
  that is *already* on a hard-register copy path, and most interesting
  quantities are not.

  **And sched1's ordering is not just priority-then-source-order.**
  `rank_for_schedule` compares `INSN_PRIORITY` first; then classifies both
  insns against the **last scheduled** insn -- 3 for independent or latency-1,
  2 for anti/output dependent, 1 for data dependent -- and prefers the higher
  class; only then does it fall back to `INSN_LUID` difference, which the
  source says is there to make the sort stable and minimise movement. Since the
  list is built in reverse, an insn that depends on nothing floats toward the
  FRONT of its block, outranking a dependent neighbour regardless of where the
  source put it.

  That is the mechanism behind two measured observations: func_80047054's
  `p[2]` load being hoisted above `e`'s `addu`, and func_80045CC4's
  `p70 = p + 0x70` landing at the arm top. Both are class 3 against class 1 --
  not a tie-break. **The lever is therefore to make the insn data-dependent on
  its neighbour**, which is what "give it a predecessor chain" means, and it
  explains why a constant offset from a parameter cannot be moved: there is
  nothing in the block for `p + K` to depend on.


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

* **Giving an EXISTING global allocno a second role in another block imports
  that block's hard-register conflicts** -- which is how you keep a
  hard-register copy that would otherwise coalesce away. This is the inverse of
  the note below that promoting a block-local value to a global allocno gives
  it hard-reg conflicts, and it is free when the second role is an assignment
  the target already performs.

  func_8003CCB0's init loop was missing the target's `addu $v1,$v0,$zero`, the
  copy of a call's return value: the pseudo printed `preferences: 2`,
  conflicted with nothing caller-saved, took `$v0` and the copy was deleted as
  a self-move. Reusing that same variable for a `D_80080AB0` load in a later
  block gave one allocno two ranges, and in that block a block-local `(high)`
  quantity holds `$v0` across it -- so `global_conflicts` ORs reg 2 into its
  `hard_reg_conflicts`, `prune_preferences` can no longer honour the `$v0`
  preference, `find_reg` takes the next register in REG_ALLOC_ORDER, and the
  copy becomes a real instruction. 29 differing words to 6, at an unchanged
  word count.

  **Two facts make this the only route, and both are worth knowing on their
  own:**

  * **A pseudo set from a hard register does not conflict with it.**
    `global_conflicts` processes an insn's `REG_DEAD` notes *before* its
    stores, so the dying source never conflicts with the destination. That is
    why nothing written inside the block that contains the call can reach this,
    and the conflicting value has to live in a *different* block.
  * **A plain pointer copy is not a second pseudo.** cse's `canon_reg` rewrites
    the uses to the quantity's first register and the copy dies: all eight ways
    of distributing three uses across a variable and a copy of it gave
    byte-identical output. A copy only survives if the original has a later use
    of its own -- which costs a word. **Or if the copy's destination is
    NARROWER: see "A `u8` destination makes a register copy INVISIBLE to cse"
    below, which is free.**

  **There is a third lever, on the CONFLICT side, and it is the one to reach for
  when no lower-priority rival exists to populate `regs_someone_prefers` at
  all.** Unifying a source variable with a value computed in ANOTHER block
  imports that block's hard-register conflicts into the allocno, which pushes it
  off a register it prefers. func_80041E70's sign-extended call result preferred
  `$v0` and nothing conflicted with it, so `find_reg` simply gave it `$v0` --
  three words wrong. Making that variable also hold the doubled `unk_87` value
  computed in an earlier arm emits the identical two insns (cse still folds the
  product) but gives the allocno a second live range, where a local-allocated
  quantity already holds `$v0`; the union of the two ranges' conflicts therefore
  contains hard reg 2, `find_reg` skips `$v0` for the whole allocno, and both
  ranges land in `$v1` -- which is why the target holds the `unk_87` value and
  the call result in one register. **They are one variable.** Zero words, 4 DIFF
  to 1.

  Two measured cautions with it. The unification has to create a real overlap:
  assigning the call result into the loop counter's variable did nothing,
  because the pseudo is dead between that assignment and the counter's init. And
  it can be defeated by sched1: unifying with a `li 0x10` that is its own
  statement lets sched1 hoist the `li` above the preceding `lw`, the two
  block-0 quantities then need `$v0` AND `$v1`, and the union conflicts with
  both -- 11 DIFF, landing on `$a0`.

  **And this is why an intermediate variable cannot shed a preference**, which
  the entry above asserts without the mechanism: `global.c`'s
  `expand_preferences` merges preferences in BOTH directions between an allocno
  set by an insn and an allocno that DIES in that insn, whenever the two do not
  conflict. Measured on func_80041E70 -- splitting the cast so the `sll`'s dest
  became a global allocno in its own right left that allocno preferring `$v0`
  and the extension's dest preferring `$v0` as well.

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

* **An `asm` input operand is a zero-cost allocno reference**, and the only one
  that is free unconditionally. `__asm__ volatile ("" :: "r" (x))` emits nothing
  and adds one to `REG_N_REFS (x)`. On func_8003E758 that moved a pointer from
  4 refs / 34 insns (0.118) past a rival's 5 / 41 (0.122) and swapped `$s1` and
  `$s2`, worth 9 words. Two operands overshoot.

  **This is a decomp device, not something the original wrote**, so treat it as
  a last resort and say so where it ships: it is shaping the allocator directly
  rather than reproducing a source shape. The statement-splitting lever above is
  the one to reach for first, since it is a plausible thing for a human to have
  written.

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

### ONE variable per LOOP, not one counter reused -- refs add up, priority inflates

A variable used by several loops whose live ranges do not overlap is still ONE
allocno, and `allocno_compare`'s priority is
`floor_log2(refs) * refs / live_length` over the SUM of those ranges. Reusing a
counter therefore builds a single high-priority allocno that takes registers
the long-lived values need, and the registers it takes are the low ones.

func_80057D20 is the measurement: 48 of its 54 remaining differing words were
this. One `i` across four loops in case 1, case 64's outer loop and the
closing loop is 46 refs over 155 live insns -- priority 1.484, which outranks
the entry pointer (56/206, 1.359) and the `d` parameter (34/177, 0.960) and
takes $s1 and $s4 from them. Four separate counters for case 1's loops are 7
refs over 17 live each, priority 0.824 apiece; one of them also serving case
64's INNER loop, and a sixth for the outer and closing loops, put `e` back in
$s1 and `d` in $s4 and every other register where the listing has them. Zero
words either way -- the separate counters share one hard register because
their ranges are disjoint.

**Read the listing's register REUSE as evidence for the split, not against
it.** Seeing $s2 serve case 1's loops AND case 64's inner loop does NOT mean
one variable; two variables with disjoint ranges get the same register. That
inference was made twice on func_80057D20 and cost two rounds both times,
including a plausible-looking three-way repartition that measured 464 words
and 90 DIFF. The reuse tells you the ranges are disjoint; what it does not
tell you is how many variables there are. Count the allocnos with
`advise.py --greg` instead, and split whenever a counter's refs are summing
across regions.

### A `u8` destination makes a register copy INVISIBLE to cse

This is the free way to keep a copy of a value as a second pseudo, and it is
the whole of func_800595C0 (122 words, four of them this).

cse only merges two registers' quantities from `insert`, and `insert` calls
`make_regs_eqv` only when it finds a REG **of the same mode** in the source's
equivalence class:

```c
  if (GET_CODE (x) == REG && classp ...)
    for (elt = classp; elt; elt = elt->next_same_value)
      if (GET_CODE (elt->exp) == REG && GET_MODE (elt->exp) == GET_MODE (x))
        { make_regs_eqv (regno, REGNO (elt->exp)); ... }
```

`c = w` with `w` a `u32` and `c` a `u8` expands to
`(set (reg:QI c) (subreg:QI (reg:SI w)))`. The class holds a SUBREG, not a
QImode REG, so `make_regs_eqv` never runs, the two never share a quantity, and
`canon_reg` never rewrites `w`'s later uses to `c`. **MIPS emits a QImode
reg-reg move as `addu`, so the dodge is free** -- the insn is the same `move`
the `u32` spelling would have produced, and nothing masks.

With both `u32`, `make_regs_eqv` does run, and because `c`'s last reference is
past the end of every extended block that contains the copy it wins the
canonical slot (`uid_cuid[regno_last_uid[new]] > cse_basic_block_end` and
later than the source's last use), so the uses move to `c`, `w` dies at the
copy, combine folds `(set w $v0)` into it, and one register does the work of
two.

What that buys, measured on func_800595C0 by flipping exactly these
declarations:

* the second callee-saved register and its `sw`/`lw` pair, plus the
  `addu $s3, $s0, $zero` -- three words;
* the local uses then read the CALL'S RETURN REGISTER. A per-block `u32 w` is
  its own pseudo with `REG_BASIC_BLOCK` set to that block, local-alloc ties it
  to `$v0`, and `(set w $v0)` disappears entirely -- so an arm spends only the
  `addu $s2, $v0, $zero` and reads `$v0` for `w & 0x7F`. One function-scope
  temp instead is a global allocno, gets ONE hard register for every arm, and
  each arm pays a second copy;
* the fourth word is the arm where `$v0` is NOT available -- a `lui` for a
  global and the loop's `sltu` both want it -- so that arm's temp falls to
  `$v1` and its `addu $v1, $v0, $zero` is a real instruction.

Corollary for reading a listing: **`addu $sN, $v0, $zero` followed by a use of
`$v0` means two variables of different widths**, and a `u8` carried value is
the natural spelling for exactly the kind of byte this function handles. Where
the one-variable form would spend the same number of words it is only a
register difference; where it would not, it is the word.

**The same device buys a plain copy BEFORE A BRANCH**, which is the shape
`addu $v0, $sN, $zero` / `beqz $v0` -- a register copy and then a zero test,
where the value is already in a register the `beqz` could read. func_80052A90
was parked one word short of exactly that for two sessions. The flag is 32
bits wide and the test reads it through `u8 t = ok;`: `u8 ok` tested directly
gives `andi $v0, $s3, 0xFF` (the right word count, the wrong opcode), `u32 ok`
tested directly gives `beqz $s3` alone and the function is a word short, and a
SAME-WIDTH `u32 t = ok;` is copy-propagated away. `s8` and `u16` work as well
as `u8`, so it is the mode difference and nothing about the value.

Two notes on scope. **It needs the destination to be NARROWER, not just
different** -- signedness alone is the same mode and cse merges it. And it
does NOT reach a parameter's entry copy: that copy is `(set pseudo $a0)` from
a HARD register, where `make_regs_eqv` always makes the pseudo canonical, so
the question is which hard register the allocno gets instead (func_8004D880
and func_8004CECC are both parked on that, and
`docs/near-misses/func_8004D880.c` has the arithmetic). A narrow LOCAL copy of
a parameter does produce the entry copy -- measured on func_8004D880 -- but
then the local is what holds the value and the parameter keeps $a0, which is a
different register assignment from the listing's.

### Statement granularity is what orders the pieces of an address

`fold` puts an ADDR_EXPR second in a sum, so an address written as one
expression emits its INDEX first and the symbol after -- and no declaration
spelling moves that: a base local, a sized alias, a small-object alias, a
struct array and an index local all left func_80055ADC's `sll` ahead of its
`lui`. What moves it is giving each piece its own statement, in the order the
listing has them:

    s32 k = b & 0xFFFF;                     /* andi  */
    u8 *base = D_80084878;                  /* lui, addiu */
    u32 *q = (u32 *) (base + k * 0x40);     /* sll, addu  */

The same granularity decides the prologue: the three scalars initialised before
the address in that function come out in declaration order, so an initialiser
list IS a statement sequence for matching purposes.

**`search.py --only=global_decl` sweeps this.** The scalar/incomplete-array
flip is now a transform family: it rewrites a candidate's own file-scope
`extern` declaration and every use of the symbol with it (`&SYM` <-> `SYM`,
`SYM` <-> `SYM[0]`), both directions, one symbol at a time. It finds
func_800353F4's match in one pass and took func_80037E58 from 8 differing
words to 2. Globals declared in a header are left alone, since changing those
would change every other unit too.

**And a diagnostic caution, which cost a session here.** A two-word difference
that looks like a scheduling choice -- two adjacent insns swapped, both
priority 1, the memory-over-ALU rule explaining the order you got -- can still
be a DECLARATION. func_80037E58's last two words were written up as exactly
that, with the sched2 dump's insn numbers and the reasoning for why no source
order could reach it, and the cure was `extern u8 D_80012D6E;`. The
declaration changes which register each half of an address lands in, which
changes what the scheduler is choosing BETWEEN -- so run the sweep before
concluding that a schedule is unreachable.

One warning that cost a false positive on its first run: the use rewrite has
to be ONE pass. Rewriting `SYM[0]` to `SYM` first and bare names to `&SYM`
second turns `SYM[0] != 0` into `&SYM != 0`, which is a constant -- the test
vanishes and the candidate scores two words BETTER. The same caution as the
declaration-width family: this one can change VALUES, not just
representations, so read the diff of what it accepted.

### A two-term index adds the BASE LAST unless the row address is its own statement

The lever above is about a symbol in the sum; this one bites with the base
already in a variable. A lookup written as one expression off a pointer local
emits both shifts and then both adds:

```c
v = *(s16 *) (t + (x << 5) + c * 2);   /* sll x; sll c; addu x+t; addu   */
r = t + (x << 5); v = *(s16 *) (r + c * 2);
                                       /* sll x; addu x+t; sll c; addu   */
```

Both `addu`s keep the same operand order, so this is NOT the array-reference
lever -- only the POSITION of the row `addu` moves, and the register names of
everything after it follow, because the pseudos are born in a different order.
A store that sits between the two terms in the listing (`sh c` after the row
`addu`) is a second witness.

`expand_expr`'s `case INDIRECT_REF` expands the address under `EXPAND_SUM`,
and `case PLUS_EXPR` in ptr_mode under `EXPAND_SUM` expands both operands the
same way and returns a bare `(plus op0 op1)` from `both_summands` without
emitting an add. So the inner pointer sum is an unforced rtx -- only its
`sll` is emitted -- the second term's `sll` follows, and both `addu`s are
emitted together when `memory_address` finally forces the nested plus. An
assignment statement goes through `expand_assignment` with no `EXPAND_SUM`,
which forces the inner `addu` at its own statement.

In func_800550E8 the two row locals are worth 3 and 7 differing words. Note
they must be SEPARATE locals per lookup: one row local shared by both is two
sets in two blocks, a global allocno, and costs a word and 77 DIFF. The same
goes for the table bases -- one pointer local serving both tables puts its
`lui` and `addiu` in different registers where the listing ties them. And
grouping the index terms instead (`t + ((x << 5) + c * 2)`) is worse than the
flat sum: the base is then added LAST of all.

### A wider store invalidates cse's entry for the narrow read after it

Two word stores into adjacent scalars leave cse holding both values, so a read
of either one afterwards is free. An 8-BYTE struct copy into the same place
does not: the MEM it writes is a different size from the 4-byte MEM read after
it, cse cannot match them, and the read comes out as a real `lw`.

    *(Unk80081568 *) &D_80081568 = *(Unk80081568 *) p;   /* lw, lw, sw, sw */
    D_80081568 = (((D_80081568 + 0x7FF) >> 11) << 23) | (b & 0x7FFFFF);

func_80053CFC is a word short written as `D_80081568 = p[0]; D_8008156C = p[1];`
and matches as the struct copy. So an unexpected re-read of a global that was
just written is evidence about the WIDTH of the write, not only about aliasing.

### A switch's tree says which cases share a body, and whether there is a default

`emit_case_nodes` (stmt.c) tests a node's own value first, then dispatches to
its subtrees -- and the branch SENSE tells you which subtree needed no test of
its own. For a node with two children it normally tests `> high` and branches
to the RIGHT; it tests `< high` and branches to the LEFT only when the left
subtree is `node_is_bounded`, i.e. the parent tests have already narrowed the
index to exactly that node's values. So

    slti $v0, $a0, 4
    bnez $v0, .L80054AB0      # branch on the LEFT

means the left subtree is a range the remaining tests pin down exactly, which
happens when two case values share one body and there is no default:

    case 1:
    case 2:
        func_80055B44(k);
        break;

A trailing `j` to the EPILOGUE rather than to a body is the other half of the
same reading: the default does nothing, so there is no default label. Written
with a `default:` that calls, func_80054A7C came out with that test inverted
and the `j` pointing at the default's body (3 DIFF through six case orders).

### Two `beq`s that branch TO their bodies are a switch, not an if/else chain

`if (k == A) {...} else if (k == B) {...}` tests and falls INTO each body in
turn, so each test's branch jumps PAST its own body. A switch emits all the
tests first -- each `beq` jumping forward to its body -- and a `j` past them to
the default. func_800549FC is 23 DIFF as the chain and matches as the switch,
with a shared `lui` for the global both bodies store to sitting in the second
test's delay slot.

### A 0/1 pair ALWAYS triggers jump.c's store-flag transform; share the site

`if (c) m = 1; else m = 0;` never survives as two branches. jump_optimize's
store-flag gate (jump.c:1139) matches the pattern `x = a; if (...) x = b;` and
fires when

    ((reversep = 0, temp2 == const0_rtx)
     || (temp3 == const0_rtx && (reversep = can_reverse_comparison_p ...))
     || (BRANCH_COST >= 2 && both CONST_INT && bit-subset)
     || BRANCH_COST >= 3)

so with {0, 1} one of the two values IS `const0_rtx` whichever way round gcc
normalises the if/else, the first or second disjunct holds, and you get
`xori` + `sltu` instead of the branches. Reversing the test, hoisting one
value as a pre-initialisation (`m = 1; if (c) m = 0;`) and masking the subject
differently all leave it: they are all that same pattern.

**What does escape it is removing the PAIR.** If the function assigns the same
value somewhere else -- a default arm, another case -- make the two sites ONE
statement and reach it with a `goto`:

```c
            if (x == 0x800) {
                m = 0;
            } else {
                goto one;          /* the shared m = 1 */
            }
        ...
        } else {
        one:
            m = 1;
        }
```

The innermost `if` then has a single arm and a jump, and there is no
`x = a; if (...) x = b;` to match. Worth two of func_8005552C's four missing
words. The same reading applies in reverse: a listing that DOES show
`xori`/`sltu` or a bare `sltu $v0, $zero, $v0` is telling you the source had
the two assignments written out.

### A switch's case bodies are emitted in SOURCE order

The compare tree is built from the sorted case values, so it tells you nothing
about the order the cases were written in -- but the BODIES are laid out in
source order, and that is visible: whichever body the last test falls through
into was written first. func_80053E20 and func_80053E90 are a setter/getter
pair over three halfword globals with cases 4, 1, 2 in that order; written
2, 1, 4 every instruction is right and the three blocks are in the wrong
places (18 DIFF).

### The load-delay slot is filled by the NEXT insn only

cc1psx fills a load's delay slot by swapping the load with the insn immediately
after it, and nothing else: it does not search the block. So when a target has
no `nop` after a load, the insn that fills the slot was ALREADY written next in
the source. func_8004A464 fills two slots -- the `+0x20` base behind an `lhu`,
and an argument's `lw` behind a global's load -- and only one statement order
out of six puts both there (the flag update, then the base, then the argument
read into a local, then the global). Everything else leaves cc1psx emitting its
own `#nop`, visible in the `-S` output, and the function is a word long. When a
candidate has a `nop` the target does not, look at what stands after the load
in the source rather than at the scheduler.

### `?:` keeps the copies that `if` deletes

A conditional expression assigned to a variable produces a copy into the result
register on EACH arm and a copy back at the join; the equivalent `if` updates
the variable in place and gcc deletes them. Neither is "better" -- they are
different word counts, and the listing tells you which the source used.
func_80049DE8's negative arm is `a = (a < 0) ? m + a : a;`: 13 words with the
three copies, 12 and 6 DIFF as `if (a < 0) a = m + a;`. Its sibling
func_80049E1C wants three or four MORE such copies than any shape found so far,
so the same question is still open there.

### A store through a loaded pointer pins the argument loads after it

sched cannot disambiguate `sw $zero, 0xC($a2)` from `lw $a1, 0x8($s0)`, so once
a store goes through a pointer the compiler cannot prove distinct, no later load
moves above it. Arguments left inside the call are evaluated after such a store
and stay there; reading them into locals BEFORE it is the only way to get the
target's order. func_80037030: 7 DIFF of pure ordering. The same reasoning says
the pointer the store goes through must be a local too, or its load and the
argument's load are two separate `lw`s of the same address.

**The mirror case: a store to a GLOBAL pins a later load through a pointer**,
and that is the commonest single-nop defect in this repo. `GLOBAL |= bit;`
followed by `p->field |= bit;` cannot have the second load in the first's
delay slot, because sched cannot prove `(mem (lo_sum ...))` and
`(mem (plus p c))` distinct. Reading the field into a local BEFORE the global
store is the whole fix:

```c
k = *(u16 *) (r + 0x1E);         /* read first */
D_800814A0 |= 0x8000;
*(u16 *) (r + 0x1E) = k | 0x8000;
```

That is one word and the eleven permuted registers behind it: it took
func_8005313C from 30 DIFF to a MATCH outright, and func_800599B0 from 51 to
1 (there the global is read and the pointer stores are the ones written
first). **`advise.py` detects this from the nop count alone** -- its
DELAY-NOPS row fires whenever the candidate has more `nop`s than the target
and prints this recipe -- so run it before reasoning about the scheduler.

### Two address materialisations interleave only if BOTH are split pairs

`lui`, `lui`, `addiu`, `lh` -- two symbols' addresses taken apart and woven
together -- is proof that neither was the one-insn macro form. cc1psx emits a
small scalar's load as `lh $2,D_80086D50`, ONE insn, and maspsx expands it into
an adjacent `lui`/`lh` pair afterwards; nothing in the compiler can schedule
anything between them, so the pair sinks past the other symbol's `addiu` as a
block. func_80057A50 sat at 2 DIFF through six statement orderings for exactly
that reason, and matched when the scalar was read through an array alias
(`extern s16 D_80086D50a[] __asm__("D_80086D50")`, read as `[0]`), which gives
the split high/lo_sum pair the scheduler can interleave.

**A loop bound is the clearest case of the same rule.** Read through the
incomplete-array spelling, the compiler's own `lui` half is loop-invariant, so
LIM hoists it into a callee-saved register and the loop re-reads the word
through that -- two words of prologue and one register more than the target.
Read through the SCALAR spelling it is a single macro insn with nothing to
hoist, and the loop re-materialises `lui`/`lw` each iteration, which is what
func_80056558's listing has. (Its guard also says the bound is signed: `blez`,
not the `beqz` a u32 bound compiles to against a zero start.)

**And the same lever runs backwards.** When a target's `lui`/`addiu` pair is
ADJACENT and an unrelated insn sits before or after it where the candidate has
one between, the pair is maspsx's, so the object must be small: a SIZED alias
of 8 bytes or fewer (`extern u8 D_80080B48s[8] __asm__("D_80080B48")`) puts the
address back into the one-insn `la` form that nothing can split. That is
func_8004B634's 2 DIFF -- an `addiu $a0, 6` scheduled into the middle of the
pair. The symbol only becomes $gp-relative if it is also in the unit's
`sdata_entries`, so a sized alias on its own is safe.

### Index the scan; a walked pointer gets eliminated and its base biased

A loop whose pointer is advanced by the element size and whose only other uses
are two loads off it is a pointer loop.c will DELETE: it replaces the pointer
with a giv of its own and biases that giv's initial value to one of the field
offsets, so the preheader gains an `ori` and the loads get displacements
relative to that field (`ori 0x3566`, `lh -2`, `lh 0`). One word long, and no
amount of rewriting around the pointer removes it.

Written with the counter as the index off a loop-invariant base, the giv is
`base + i * 4` and both field offsets stay in the loads' displacements:

    u8 *b = (u8 *) 0x80010000;
    for (i = 0; i < 0x14; i++)
        if (*(s16 *) (b + i * 4 + 0x3564) == x &&
            *(s16 *) (b + i * 4 + 0x3566) == y) break;   /* as nested ifs */

That is the unbiased `lui 0x8001` plus `addiu 4` in the branch delay slot the
listing shows. It unparked func_8004A6C0, func_8004A8D8 and func_8004A784 at
once. Note this is the opposite of section 9's advice for a loop that STORES
through its pointer -- there the walked pointer is what the target has, because
it survives. The question to ask is whether loop.c can delete the pointer.

A matching detail from the same family: a constant base declared INSIDE the
`if` makes its `lui` the body's first insn, which reorg then lifts into the
guard's delay slot. At function scope the index is emitted first instead,
because fold puts the constant operand second.

### A %lo in the displacement with a REGISTER index means a constant base

cse folds a symbol's `%lo` into a MEM only when what is left is a CONSTANT
offset off the `(high)` pseudo. With a register index in the address it never
does, so

    lui  $v0, %hi(SYM)
    addu $s0, $s0, $v0        # $s0 = i * 4
    sh   $zero, %lo(SYM)($s0)

is NOT a symbol reference at all. It is the constant base 0x80010000 -- whose
low half is zero, so the base is a bare `lui` -- with the field offset written
inside the index expression:

    u8 *b = (u8 *) 0x80010000;
    *(s16 *) (b + i * 4 + 0x3564) = 0;
    *(s16 *) (b + i * 4 + 0x3566) = 0;

splat prints those displacements as `%lo(D_80013564)` and `%lo(D_80013566)`
whenever both addresses are in its symbol list, which is what makes the listing
look symbolic. Five symbol spellings on func_8004A7D8 -- incomplete array,
sized array, struct array with an `__asm__` alias, an element pointer, and a
one-set pointer local -- all spend an `addiu %lo` the target does not have.
Probe it in isolation if in doubt: with constant indices cse DOES fold the
`%lo` (`sh $0, %lo(SYM)($16)` off a bare `lui`), and with a register index it
emits `lui`/`addiu`/`addu` every time.

### `p += x` is one long quantity; `q = p + x` is two short ones

Advancing a parameter in place makes the parameter and the advanced pointer a
single quantity, live from the prologue to the last use -- which, if that
crosses a call, is the function's only global allocno and takes the last
callee-saved register, pushing every other parameter round by one. Giving the
offset pointer its own local lets the parameter die at the `addu`, and the new
pointer is what lives:

    u8 *q = dst + i * 2;       /* not dst += i * 2; */

func_8004EAC4 was 13 DIFF of pure register naming ($s0/$s1/$s2 permuted across
its three parameters) and matched on that one line. The listing cannot show
which spelling it was -- `addu $s1, $s1, $s0` is in-place either way, since the
dying parameter's register is reused -- so try both whenever callee-saved
numbering is all that is left.

### `||` puts the body BETWEEN the two tests

Two tests that share one exit are often an `||`, and the layout proves it:
short-circuit expansion emits `if (A) goto body` -- a branch TAKEN to the if's
body -- then B's test branching PAST the body, then the body, then the
continuation. So the shared `return` sits in the middle of the function with
the real work last.

    if (b < 0 || (t = func_8003C5F0()) == 0) {
        return 0;
    }
    return *(u32 *) (t + b * 8) != 0;

No nesting of two ifs reproduces it. `if (A) return 0; ... if (B) return 0;`,
the same with `else`, and the `if (!A) { ... if (!B) ... } return 0;` form all
write two identical `return 0` blocks, and cross-jumping keeps the TRAILING one
and redirects the earlier branch to it -- which inverts the second branch and
puts the work in the middle. func_8003C890 sat at 13 DIFF across four such
spellings and matched on the `||`. An assignment inside the condition
(`(t = f()) == 0`) is part of the shape: it is how the called value reaches the
code after the body.

### Duplicate the tail per arm: cross-jumping puts it back

`jump_optimize` runs a second time after reload with cross-jumping on, so two
arms that end in the *same* instruction sequence are merged: the first arm
keeps only its own insns plus a `j` to the second arm's copy of the tail. The
merged output is word-for-word what a single join block would have produced --
which means **the listing cannot tell you which one the source was**, and the
two compile to different registers.

    if (c) return t[*(s16 *) (p->unk_0C + (j * 2 + j) * 4 + 2)];
    return     t[*(s16 *) (p->unk_0C + (j * 4 + j) * 4 + 2)];

With a join block (`if (c) k = 3*j; else k = 5*j;` then one tail) the index is
a **global** allocno: it is live across the join's `lw`, local-alloc has
already given that `lw` `$v0`, so global-alloc records the conflict and the
index is pushed to `$v1` -- and the pointer it was supposed to share a register
with is pushed again, to `$a0`. Written per arm, every insn of the tail is
block-local, local-alloc assigns the arm in insn order, the index chain keeps
`$v0`, and the `lw` falls to `$v1`, where the dying pointer also lives
(`lw $v1, 0xC($v1)`). func_8004A658: 29 DIFF with every opcode, immediate and
delay slot already correct, then MATCH.

A `return` in each arm does a second thing a shared join cannot: it makes the
HARD register `$v0` live from the arm to the epilogue. local-alloc runs before
global-alloc and knows nothing about pseudos it has not placed, but it does
know which hard registers are live, so the arm's own computation routes around
`$v0` -- which is how func_8005405C's second mask chain lands in `$v1`/`$a0`.
Assign a variable instead and the order reverses: local-alloc gives the chain
`$v0`, and the result, a global allocno, is pushed to `$a1` and copied into
`$v0` at the end. Any `move $v0, <reg>` on the last line of a listing that the
candidate does not have is this.

Watch for the opposite trap in the same shape: `r = 1;` before a test with
`r = 0;` after it is what jump.c store-flags into a single `sltu`, which no
amount of register juggling will undo. Nested ifs with one `return` at the end
keep the branches.

Reach for this whenever a diff is registers-only around a join and the join
contains a load. It is also the answer to the shape where sched1 and local-alloc
pull against each other -- sched1 hoists the load above the shift because the
load's latency makes its critical path longer, which hands the load the first
quantity in the block and with it `$v0`; you cannot reorder your way out of
that, but you can make the block contain the whole computation instead.

### A pass-through call that emits nothing still costs references

A call that hands a function its own incoming arguments unchanged --
`func_8004B834(a, b, c)` where the real callee is `void func_8004B834(void)` --
compiles to *the same zero instructions* either way: cse substitutes `a0` for
the pseudo, the move becomes `(set (reg a0) (reg a0))` and it is deleted. But
the moves are references while they exist, and `REG_N_REFS` / `REG_LIVE_LENGTH`
are what global-alloc's priority divides. In func_8004B854 the three spurious
refs lengthened `a` past `b`, so `a` won `$s1` and `b` took `$s2`; the target
has them the other way round. 6 DIFF, and the only visible symptom is the
callee-saved numbering -- every opcode and immediate already agreed.

So when a diff is nothing but a callee-saved swap between two parameters, check
what the callees really take before touching the body. `grep` the unit: a
`void f(void)` already decompiled elsewhere settles it.


### `a - C + b` reassociates -- a TEMPORARY is the only way to pin the constant

`x = x - 0x30 + d;` does NOT emit `addiu $v0, $s1, -0x30` then
`addu $s1, $v0, $v1`. fold moves the constant onto the other operand and you
get `addiu $v0, $v1, -0x30` / `addu $s1, $s1, $v0` -- the same two
instructions with the subtraction on `d` instead of on `x`. Parentheses do not
help (`(x - 0x30) + d` is identical), and nor does writing the sum the other
way round (`d + (x - 0x30)`).

What does is splitting the subexpression out:

```c
s32 t = x - 0x30;

x = t + d;
```

`t` has one set and one use, so it costs nothing, and the constant stays where
the listing has it. func_8004DA74's last 2 words, with everything else
matching.

Two statements on `x` itself (`x -= 0x30; x += d;`) is NOT the same -- that
does the subtraction in place and comes out a word longer.


### A shared tail cross-jumping does NOT reach wants a `goto`

Cross-jumping merges identical tails, but not reliably across arms of
different depth. func_80038AB8 has the same five-insn bail block in its
non-positive arm and inside its positive arm's `f == 0` branch; written out
twice, jump2 leaves BOTH and the function is 111 words against 98. A `goto`
into the inner block -- legal C, and gcc 2.7.2 takes it -- gives the one copy
the listing has, placed where the inner one was.

The second `goto` there is a different trick with the same flavour. A tail of
`if (c) { return 1; } return 0;` whose `return 0` has ONE predecessor gets
rewritten branchlessly by jump.c as `xori`/`sltiu`; the listing branches, so
the `return 0` must have a second predecessor. `goto done` from the other arm
supplies it and the branches come back. **A listing that BRANCHES to return 1
or 0 where you get a setcc is telling you that the `return 0` is shared.**


### A DEAD reference is still a reference -- the free way to flip an allocno

`allocno_priority` is `floor_log2(refs) * refs * size / live_length`, and
`refs` counts every mention in the RTL, including one whose value cse folds
away. So a loop guard written `i < n` where `i` is provably 0 costs nothing --
it still compiles to the `blez` that `0 < n` gives -- and buys the counter one
more reference.

func_800387D0 is 10 wrong words with `if (p[0x25] - 1 > 0)` and MATCH with
`if (i < p[0x25] - 1)`. The counter had 9 weighted references against the
+0x08 temporary's 6, which is `3 * 9 / 42 = 0.643` against
`2 * 6 / 17 = 0.706`; the tenth reference crosses the `floor_log2` step's own
denominator and gives `3 * 10 / 42 = 0.714`, and the two swap $t0 and $t1.

References inside a loop are weighted by loop depth, so one OUTSIDE the loop
is the finer instrument: it moves the count by one rather than two.

### Keep $v0 busy to make a `lui`/`addiu` pair share its register

`lui $a0, %hi(SYM)` / `addiu $a0, $a0, %lo(SYM)` and
`lui $v0, %hi(SYM)` / `addiu $a0, $v0, %lo(SYM)` are the same two
instructions; which you get is whether the `(high)` pseudo and the address
pseudo were allocated the same register. The high dies at the `lo_sum`, so
they CAN share, and local-alloc hands the high the lowest free register --
$v0 -- unless something is live across it.

So the lever is to put a value in $v0/$v1 across the materialisation. In
func_800387D0 that is reading the clamped counter into a local BEFORE the
table pointer is assigned: `w = (s16) p->counter; t = SYM;` instead of
`t = SYM;` first. 2 wrong words otherwise, and it is the last thing that
function needed.

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

**The bytecode cursor is the same copy, and it is a whole family.** unk_72A8's
opcode handlers read through the stream pointer at `p + 0x1C` and advance it in
the same statement pair, and the advance is a read-modify-write of the FIELD,
not of a local:

    u8 **q = (u8 **) (p + 0x1C);

    p[0x26] = **q;      /* -> lw p1 ; addu p2,p1,zero ; lbu */
    *q += 1;            /* -> addiu p2,p2,1 ; sw        */

Caching the cursor (`u8 *c = *q; ... *q = c + 1;`) collapses the pair to a
single `addiu p2,p1,1` and comes out exactly ONE WORD SHORT with every other
insn in the right order. That signature -- one missing word, nothing else
misplaced -- is worth recognising before re-deriving the logic: eleven source
shapes and all 24 permutations of the surrounding stores were tried on
func_80039730 before the re-dereference was, and only `**q` with `*q += n`
produces the copy. The two trailing stores have to come AFTER the pair.
Matched: func_80039730, func_80039C44, func_80039D10 (one byte) and
func_80039578, func_800395B0, whose halfword is `(*q)[0] + ((*q)[1] << 8)`
followed by `*q += 2`.

**A LOOP BOUND gets the same copy from an explicit guard.** A `for (i = 0;
i < SYM; i++)` reads the global once and uses the one pseudo for both the
entry test gcc's loop inversion builds and the bottom test, so there is no
copy. Writing the guard yourself gives the bound two reads: loop.c hoists the
in-loop one into the preheader, cse2 finds it redundant against the guard's and
rewrites it to a register copy, and that copy is what holds the bound for the
`sltu`:

    u32 i = 0;              /* NOT inside the if: see below */

    if (D_800869B4 != 0) {
        do {
            ...
            i++;
        } while (i < D_800869B4);
    }

Worth one word in func_80059D1C and func_8005C818. The declaration initialiser
is the second half: with `i = 0;` written as a statement inside the `if`, the
branch's delay slot gets a `nop` instead of the `addu $a0, $zero, $zero`,
because the move is then below the test and the filler will not sink it into
the slot -- above the test it is a candidate to be moved down. Two words apart,
one lever each.


* **The rule is about a FIXED-address scalar against a VARYING-address struct
  reference, in either order -- so it governs store placement as well as load
  delays, and the fix can be applied from either side.** gcc 2.7.2's
  `true_dependence` assumes those two do not conflict. Measured on
  func_8003E758, 12 words across five sites: as plain scalars its D_800814D3
  dispatch read lifted ABOVE a `D_80083958.unk_0A |= 1` store group, and four
  `sb D_800814D2` stores SANK PAST the array load they have to precede.
  Nothing there is a load-delay nop; it is pure store/load placement in sched.

  **Making the SCALAR in-struct is the other half of the lever**, and it is the
  half this doc did not have. Where the struct side has to stay a struct -- in
  func_8003E758 the array reference is what produces the target's index-first
  `addu`, see the entry below -- the scalar is what you change. But it needs a
  real `COMPONENT_REF` or an `ARRAY_REF` on an actual array DECL:
  **`((u8 *) &X)[0]` does not work**, because `build_array_ref` turns indexing a
  non-decl into an `INDIRECT_REF`, and `MEM_IN_STRUCT_P` for one of those is
  `AGGREGATE_TYPE_P (type)` -- 0 for a byte. Declaring the object `u8 X[1]` does
  work, and so does a one-byte struct cast; the array declaration is the one to
  prefer, because it keeps the cast out of the source.

  Also worth knowing for reading the other side: a pseudo holding `&SYM` via
  `lui` + `addiu` counts as a **varying** address, because
  `init_alias_analysis` does not canonicalise a `lo_sum`-set pseudo to its
  constant. That is why a struct reached through a materialised base is the
  "varying" side at all.

* **The same flag decides whether cse keeps a cached load at all, which is a
  bigger consequence than ordering.** `true_dependence`'s differing-flags
  exception is what cse's `invalidate` consults too. In func_8003F2A4 the target
  reads `arg0->unk_04` TWICE around two stores to small `$gp` globals. Declared
  as plain `u32` scalars, each store is a non-struct reference at a non-varying
  address, the exception applies, `invalidate` does not drop `MEM(a0+4)` from the
  table, the second read is shared into a `move`, sched1 then hoists both loads
  above both stores and the function needs an extra `move` to free `$a0`: **29
  words against 31, all 31 differing**. Declared `u32 X[1]` and stored through
  `X[0]`, each store is an ARRAY_REF, both sides are in-struct, the base load is
  invalidated and re-emitted, the stores stay interleaved with the loads, and the
  function matches. So the array-element spelling buys a RE-DEREFERENCE, not just
  an ordering -- and it is the cheapest instance of section 4's "re-dereference
  rather than cache", because the declaration causes it rather than the source.

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

* **A re-read after a pointer store PINS the store, which is worth more than
  the copy it makes.** cse's `note_mem_written` sets `writes_ptr->var` for a
  store through any pointer, and `invalidate_memory` then drops EVERY
  `in_memory` entry whose address `cse_rtx_addr_varies_p` -- with no offset
  check at all, since the offset-aware `refers_to_mem_p` path in `invalidate`
  only handles the stored MEM's own entry. So after *any* store through *any*
  pointer, cse has forgotten every pointer-based load, and a C-level re-read
  survives expand as a real `(set rN (mem))`. That load has a true memory
  dependence on the earlier store of the same field and a consumer after it,
  so **sched1 can no longer sink that store to the end of the block** -- and
  in a switch arm, a store that reaches the end of its block is cross-jumped
  into the shared tail by jump2, costing a word per arm.

      *(u32 *) (p + 0x10) = *(u32 *) (p + 0x10) + *(u32 *) (p + 0x40);
      *(u32 *) (p + 0x40) = *(u32 *) (p + 0x10) & 3;   /* store through p   */
      *(u32 *) (p + 0x10) = *(u32 *) (p + 0x10) >> 2;  /* re-read after it  */

  gives `sw $v0,0x10 ; addu $v1,$v0,$zero ; andi $v0,$v0,3 ; srl $v1,$v1,2`,
  with the store AHEAD of the arithmetic it feeds; holding the sum in a local
  gives `andi $v1,$v0,3 ; sw $v0,0x10 ; srl $v0,$v0,2` and the store sinks.
  **Worth nine words and 25 register differences in func_80059814** -- one
  word per arm from the cross-join plus the copy per arm. Only the ORDER
  matters, not the locals: keeping `m` and `t` but storing `m` to +0x40
  *before* reading +0x10 again for `t` also matches.

  Read the direction off the listing: `sw $rA, OFF(base)` sitting before the
  ALU ops that consume `$rA`, followed by `addu $rB, $rA, $zero` with both
  registers then modified in place, is a re-read. A store at the END of a
  block with no copy is a local. The intervening store need not be through
  the same pointer -- a store through another pointer or to a fixed-address
  global makes the re-read survive too, and there it stays a real `lw`,
  because sched1 cannot hoist it over a possibly-aliasing store.

  One caveat on the copy itself: between the `.lreg` and `.greg` dumps the
  load is rewritten in place (same UID, same LOG_LINKS, `REG_EQUIV` kept)
  from `(set v1 (mem +0x10))` to `(set v1 v0)`. FSF 2.7.2.3's reload has no
  path that touches an insn needing no reload, so this looks like the gcc 2.8
  `reload_cse_regs` backported into this cygnus-2.7.2-970404 binary alongside
  its `regmove` pass (2.7.2 has none). Not confirmed from source -- the
  binary is all we have.

## 5. MEM_IN_STRUCT_P decides sched1's alias calls -- and cse's invalidation

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
* **The split addressing form forfeits the exception, because its MEM has a
  register base.** func_8004B500 is `if (p) { p->flags |= 0x8000;
  D_800814A0 |= 0x8000; }`, and with D_800814A0 a scalar (a non-varying symbol
  address) against a struct field it interleaves exactly as the target does.
  Its twins func_8004EE90 and func_8004F52C need the compiler-split spelling of
  the same symbol -- and then the global's address is `(lo_sum (reg) (symbol))`,
  which varies, so no spelling of the pointer side frees the load to cross the
  halfword store. It has to cross it in the SOURCE: read the global into a
  local before the store and write it back after. Section 8 has the shape.
  The interleave then also buys the copy of the argument into `$a1`, because
  four quantities are live at once instead of three.

### A conditional negate with a COPY in it means the result is a NARROW local

`u = n * 2; t = cond ? -u : u;` with both `s32` is ONE quantity: local-alloc
unifies them and the negation happens in place, so the listing would show
`negu $v1, $v1` and nothing in the branch's delay slot. A listing that has
`addu $v1, $a1, $zero` for the unchanged arm and `negu $v1, $a1` for the
negated one has TWO quantities, and declaring `t` as `s8` (with the
sign-extension then falling out of its use, not a cast) is what keeps them
apart. func_80056098, 2 words.

**With it: write the mask test on the UNDOUBLED value.** `(u & 0x80) != 0`
leaves the `andi`, because combine cannot fold a mask back through a pseudo
with several uses; `(n << 25) < 0` is the fold combine would have done for the
single-use case, and it is the two words the listing spends there.

### `x = x << 16; x = x >> 16;` is the IN-PLACE sign-extension; the cast is not

MIPS's `extendhisi2` expander allocates a FRESH pseudo for the intermediate
shift, so `x = (s16) x` gives `sll $v0, $a1, 16` / `sra $a1, $v0, 16` -- two
registers, and the intermediate competes for $v0 with whatever else is short
-lived there. Writing the two shifts as separate statements on an `s32` makes
them one pseudo set twice, and the listing's `sll $a1, $a1, 16` /
`sra $a1, $a1, 16` comes out instead.

It matters beyond the two words: with the intermediate off $v0, every other
short-lived value in that block is renumbered. func_8005A608 and
func_8005A778 both needed it.

### A parameter re-assigned its own truncation extends IN PLACE -- and resets cse

`sll $a1, $a1, 16` / `sra $a1, $a1, 16` with the raw value saved first --
`addu $v1, $a1, $zero` -- is not what an `s16` parameter gives you. There the
raw keeps its incoming register, nothing is copied, and the extension lands
somewhere else. The spelling that produces the listing's shape is an `s32`
parameter assigned its own truncation:

```c
j = id;              /* s16: the raw value, a plain copy */
id = (s16) id;       /* the sign-extension, back into id */
if (id == -1) { ...scan... }
i = j;               /* the other arm uses the raw */
```

Two separate things fall out of that, and both are words:

* **The copy.** `j = id` has to be a real insn, because the next statement
  overwrites `id`.
* **cse stops forwarding the extension.** Assigning to `id` invalidates that
  pseudo's equivalences, so a later `(s16) i` -- with `i` copied from the raw
  -- is no longer the same expression as the entry's `(s16) id` and is
  recomputed. Without the re-assignment cse forwards it across the branch (it
  follows a conditional jump whose label has one use and is preceded by a
  barrier, cse.c:8107), and the second `sll`/`sra` pair goes missing.

**The raw copy has to be a TRUNCATION, not a copy.** On func_8005C2C0 the
same `j = a; a = a << 16; a = a >> 16;` gave the extension on the COPY's
register, not in place. The difference is `j`'s type. `s32 j = a` is a plain
copy, so cse puts j and a in one quantity; `canon_reg` returns that quantity's
first register and rewrites `a << 16` into `j << 16`. The extension then reads
the callee-saved copy, which hands `a`'s pseudo a fourth `reg_n_refs` -- enough
to outrank the next parameter and swap two hard registers. `s16 j = a` is a
truncation, a DIFFERENT expression, so cse records j as the low part of a
rather than as a, leaves the shift reading the parameter, and the parameter
extends in place in its incoming register while the copy keeps the raw.

Three variables do NOT do it: `j = id; t = (s16) id; if (t == -1)` puts the
extension in its own pseudo, the parameter keeps its incoming register and the
copy disappears again. The assignment has to be back to the parameter.

Worth 3 of 4 missing words on func_8005A608 (85 DIFF to 11) and the same on
func_8005A778; func_8005A90C is a third instance.

**The knock-on to read next:** where the extension's INTERMEDIATE pseudo lands
decides what register the nearby constants get, and reload's `find_equiv_reg`
-- which scans back only as far as the nearest CODE_LABEL -- then either
re-materialises a constant or copies it from a register that happens to still
hold it. A listing that materialises the same small constant three times is
telling you those registers were clobbered in between.

### An `s16` parameter's sign-extension sits at its first INT USE, not at entry

`assign_parms` copies a narrow parameter into a pseudo with a plain move --
HImode values live unmasked in registers, so `addu $t2, $a0, $zero` is the
whole conversion. The `sll 16`/`sra 16` appears where the value is first used
as an `int`. So a listing whose entry block materialises something ELSE
between the parameter copies and the sign-extension is telling you the
extension belongs to a later statement: use the parameter directly rather than
copying it into an `s32` local as the first statement, which hoists the
extension to the top and pushes everything else behind it. Four words in
func_80052774, and the last thing wrong in it.

### A switch subject read as `s16` is declared `s32`

`s16 s = *(s16 *) (p + 4); switch (s)` gives the pseudo HImode, so the read is
a `lhu` and the switch sign-extends it with `sll`/`sra` -- three words where
the listing has one `lh`. Declaring the local `s32` makes the read
`(sign_extend:SI (mem:HI))`, which is the `lh`. func_80051F58, one word.

Its sibling fact, in the same function: the SAME halfword read both ways --
`*(s16 *)` for the switch and `*(u16 *)` for an increment -- is TWO loads in
the listing, `lh $v1, 0x4($a0)` and `lhu $a3, 0x4($a0)` back to back. cse
relates a sign-extend and a zero-extend of one `(mem:HI)` in only one
direction, so whichever one it folds costs the `sll`/`sra` pair; with the
`s32` spelling above it folds neither. Putting the unsigned read after a store
through the same base also separates them -- both addresses are bare pointer
dereferences with varying addresses, so `true_dependence` makes the store
invalidate everything -- but that costs a word elsewhere.

### A stack buffer whose loads are NOT hoisted is a STRUCT, not a byte array

When a loop body's loads each sit in front of an empty delay slot -- load, nop,
use, load, nop, use -- the body was not scheduled at all, and the reason is
alias analysis, not luck. Declare the buffer as a STRUCT and write its fields:
struct accesses carry `MEM_IN_STRUCT_P`, so section 5's rule makes every load
with a VARYING address depend on them and sched1 cannot move one above a store.
Through `u8 buf[0x40]` with `*(u32 *)(buf + 4)` casts the stores are plain
dereferences like the loads, sched1 interleaves them, and the function comes
out short by the nops it filled.

func_8005B16C: two words, and the whole body's order. func_8005B070, its
sibling, needs the struct AND the right store order -- the table load is the
SECOND store's source, and writing it last lets the load move two slots up and
take a second register (4 DIFF at the exact word count).

The type goes in `include/structs.h` as `Unk<callee>SP`, the convention the
other stack-parameter structs there use.

### A store that gcc deletes as dead is a VOLATILE field

The mirror of the entry below. When a listing writes one field two or three
times with a progressively refined value and never reads it back, gcc's flow
pass deletes every store but the last -- it keeps a `mem_set_list` and drops a
store to a location that is stored again with nothing reading it in between --
and the function comes out short by exactly the deleted stores.

Spelling the refinements as in-place field updates does NOT save them:
`*(u32 *) (b + 0x10) &= 0xFFFFFF;` reads the field back, cse forwards the value
just stored, the load disappears and the store is dead again. `volatile` on the
store is the only thing that keeps it. func_80054B08's two words, six stores
across two fields.

Qualify only the fields that are written repeatedly. Marking the whole base
volatile pins the stores that the target DOES schedule freely and costs more
than it saves (measured there: 32 DIFF against 36).

**Where to look for it.** advise.py's MISSING row names the signature
directly: "target has `lw`/`lbu`/`lhu` and built has nothing there" means the
listing loads something this source does not, and a cse-forwarded read is the
usual reason. Scanning all 44 parks on 2026-10-27, thirteen have that row:

  func_8003800C  func_80038CB8  func_800398BC  func_8004437C  func_80047054
  func_8004A330  func_8004AC3C  func_8004D294  func_80054B08  func_800565D8
  func_80056654  func_8005914C  func_8005BBFC

That is a work list, not a diagnosis -- a missing load can equally be a
different addressing shape or a read the source simply does not make. Eleven
of the thirteen have now been swept with `t_volatile`, and exactly ONE moved:
func_80047054, 30 differing words to 24. The other ten are ruled out as far
as a single qualifier goes --

  func_8003800C  func_80038CB8  func_800398BC  func_8004437C  func_8004A330
  func_8004AC3C  func_8004D294  func_800565D8  func_80056654  func_8005914C

-- so on those the missing load is something else, and the row should be read
as "the listing makes a load this source does not" rather than as a volatile
candidate. A one-in-eleven hit rate is about what a mechanical sweep over a
signature this broad should be expected to give; the value is in the ten
negatives being recorded rather than re-derived.

### A load of a value just stored is a VOLATILE field

When the listing stores a value and then loads the same address straight back
-- `lhu`, `nop`, `addiu`, `sh`, `lhu`, `nop`, `andi 1` -- cse forwarding is not
something a source spelling can switch off. Nothing about how the increment or
the test is written denies it: on func_80036F24's counter at +0x64, a `u16 *`
local, `(*p)++`, reading the value back as `s16`, the increment through a `u16`
temporary, and the two accesses split across statements were ALL two words
short, and `MEM_IN_STRUCT_P` is refuted as the cause (section 10 below, four
probes). A volatile load is never forwarded and never deleted, which is exactly
what the listing shows, so the field was `volatile` in the original struct:

```c
*(volatile u16 *) (p + 0x64) = *(u16 *) (p + 0x64) + 1;
if ((*(volatile u16 *) (p + 0x64) & 1) == 0) { ... }
```

Only the RE-READ needs the qualifier; marking the store's own load as well is
equivalent. This landed func_80036F24, func_80037090 and func_80037394, three
functions parked for exactly these two words.

Do not reach for it on every unforwarded load -- `volatile` also pins the
instruction where it is and empties its delay slot, so it will cost words if
the listing does not actually show the reload. The evidence to require is the
SECOND load of an address already stored, with no call or label between.

### Two bit tests that stayed separate are NESTED IFS

`fold_truthop` merges two comparisons against ONE operand into a single masked
compare, so `(x & 0x100) != 0 && (x & 0x1000) == 0` comes out as
`andi 0x1100` against `0x100` -- three insns where the listing has four. As
NESTED `if`s the two tests are separate statements, which fold cannot reach,
and cse still shares the single load. Two words per pair, four in
func_80054788. Try this before the two-spellings trick below: it costs nothing
and needs no second declaration.

### Two bit tests that stayed separate are two SPELLINGS of one lvalue

`fold` rewrites `(x & 0x2000) != 0 || (x & 0x8000) != 0` into
`(x & 0xA000) != 0`, one `andi` and one branch. A listing that spends two
`andi`s and two branches on the same word therefore cannot come from a plain
`||` of two masks -- and no amount of parenthesising or `!`-shuffling changes
it, because fold works on the tree.

What does work is writing the two tests through DIFFERENT spellings of the same
lvalue:

```c
if ((*(u32 *) (g + 0x10) & 0x2000) != 0 || (D_80083160.unk_10 & 0x8000) != 0)
```

fold sees an `INDIRECT_REF` on one side and a `COMPONENT_REF` on the other, so
it cannot merge them; cse then unifies the two loads, which is why the listing
has ONE `lw 0x10($s2)` feeding both `andi`s. func_800366F4, three words and
the whole branch structure.

A `goto` out of a combined test does NOT work -- fold gets there first and the
merged `andi` survives the restructuring (measured, still three words short).

The same function shows the companion fact for free: its flag word is re-read
before the second and third tests because both of those labels have more than
one predecessor, so cse's extended basic block ends there. Reading the word
into a local gives one load and loses the re-reads.

### Which spelling a global's bound takes is PER SITE: read the `lui`'s register

`extern s32 X` and `extern u32 X[]` both load X, and the project has matches
that need each. The difference is one insn's register:

* the SCALAR spelling emits the assembler's macro, which maspsx splits into
  `lui $r, %hi(X)` / `lw $r, %lo(X)($r)` -- the SAME register for the address
  and the value;
* the INCOMPLETE ARRAY spelling is compiler-split, `lui $v0, %hi(X)` /
  `lw $v1, %lo(X)($v0)` -- a separate address temp.

func_80056558 needs the scalar for `D_80073734`; func_8005B070 needs the array
for the same symbol, and getting it wrong there cost six of the ten words that
were left, including a register swap at the top of the function. So this is not
a project-wide choice. Look at the `lui`'s destination in the listing and match
it.

### A struct POINTER local gets both a materialised base and MEM_IN_STRUCT_P

`u8 *b = (u8 *) &D_80083160;` and `Unk80083160 *q = &D_80083160;` both put the
symbol's address in a register and share it across every access, so they look
interchangeable. They are not: the reads through `q` are COMPONENT_REFs, so
`MEM_IN_STRUCT_P` is set on them, and the reads through `b` are bare pointer
dereferences, so it is not. That decides whether a nearby store to an
unrelated pointer invalidates them in cse (section 5).

func_80050308 needs both halves at once: the listing shares one
`addiu $a1, $v0, %lo(D_80083160)` between three reads of the global AND
re-loads the +8 word after a store through the function's own `p`. Through
`q->unk_08` that is what comes out. Through `*(u32 *)(b + 8)` the re-load and
its delay nop go missing, because the read and the store are then both
non-struct and cse forwards the value already in $a0.

So when a global's base is materialised AND one of its fields is re-read
across a store, the spelling is a pointer to its real type.

### Reading one field twice beats a local when the value is wanted in two registers

`*(s32 *)(p + 0x18) = *(s32 *)(p + 0x1C);` followed by
`*(s32 *)(p + 0x1C) = f(*(s32 *)(p + 0x1C), ...);` compiles to ONE load plus a
register copy: cse forwards the second read, because two MEMs at constant
offsets off one base cannot alias, but the value is still wanted in two places
at once and that is the `addu $v0, $a0, $zero` before the call. Holding it in a
local instead gives one pseudo, which the allocator puts in the argument
register, and the copy disappears -- a word short. func_80050308.

### A reg-reg copy right after an `lbu` means the byte is a LOCAL

The mirror of the entry above, for QImode. `r[4] = r[3]; q[0x28] = r[3];` does
NOT share the `lbu`: `note_mem_written` sets cse's `all` flag for every QImode
store -- ANSI lets a character pointer alias anything, and that is the one
width cse refuses to disambiguate by offset -- so the first `sb` flushes the
table and the second `r[3]` is a new pseudo. Word count is unchanged, because
reload turns the second load into `addu $v0, $a1, $zero`, but the copy then
occupies a branch delay slot the target leaves as a nop and the allocation
shifts around it. `v = r[3]; r[4] = v; q[0x28] = v;` gives one pseudo and one
`lbu` serving both stores. Seven differing words in func_8003800C; its sibling
func_80037D9C already spells the same three lines that way.

So: **an unexplained copy immediately after a byte load whose value is stored
twice is not an allocation accident -- it is the second read.**

## 6. Declaration decides addressing

This is the subject of `docs/addressing-and-translation-units.md`; the short
form:

* A scalar, or an array inside `-G8`, gets the assembler-expanded `la`.
  Anything larger, or an incomplete `extern T X[];`, gets the compiler-split
  `lui`/`addiu` -- which is **two** insns, so reorg can put the `addiu` in a
  delay slot where the single `la` macro cannot be split (func_80040CBC, 3
  words).
* **A field offset written inside the index expression migrates onto the
  symbol; a COMPONENT_REF's cannot.** `*(s32 *) &SYM[i * STRIDE + K]` on
  `extern u8 SYM[]` makes `fold` reassociate K next to the ADDR_EXPR, so the
  base comes out as `addiu %lo(SYM)+K` with `lw 0x0` off the index, where the
  target has `%lo(SYM)` and `lw K`. Declaring the table as an incomplete array
  of structs and writing `SYM[i].unk_K` applies the offset to the MEM at expand
  time, where no reassociation can reach it -- and the base materialisation
  then also comes out FIRST, which is the other half of what such a listing
  shows. Worth 5 DIFF on each of the four func_8005C8xx readers, 3 on
  func_8005845C, and 7 on func_8004AC18, all of which had been parked as
  register-order puzzles. The same thing with constant indices wants a local
  pointer instead: `s16 *b = SYM; b[36] = b[86];` materialises the symbol once
  and makes every access a displacement, where `SYM[36]` biases the base to the
  first index and spends a second `addiu` undoing it (func_8003AAF8, 1 word).
* **`&scalar` and an incomplete array schedule DIFFERENTLY as a call
  argument, even though both emit the same `lui`/`addiu`.** This is what the
  "stack-parameter prologue interleave" cluster was, four functions deep.
  `func_8003FF2C(0x11, (UnkNode *) p, 0x49, &D_80082BC0)` on
  `extern UnkNode *D_80082BC0;` puts the pair AFTER both of the entry block's
  parameter save/load pairs; `extern UnkNode *D_80082BC0[];` passed by name
  puts it BETWEEN them, which is what every one of those listings has. Nothing
  else changed and func_80036B10, func_80036D4C and func_8003719C all went
  from parked to MATCH; func_80035E58 went 8 DIFF to 2. It also pulls the
  preceding `addiu $a2, $zero, 0x49` one slot earlier, so a cluster of three
  to six wrong words in an entry block collapses at once.

  The mechanism is `schedule_select`'s `potential_hazard`, not priority: these
  functions are a single basic block, so every entry insn is priority 1 and
  the DEFINE_FUNCTION_UNIT model decides. **When an address is a call argument
  and the entry block interleaves it with the prologue differently, try the
  incomplete-array spelling before reading another scheduler dump.**
* **cc1psx runs at -G8 over this whole project** (`CFLAGS` in the Makefile;
  only gas's -G varies per unit), so a 4-byte scalar is always small data and
  always gets the symbolic form. That form is ONE insn until maspsx splits it,
  so its `lui` and its load cannot be scheduled apart: a listing with other
  instructions BETWEEN them is proof that the original declaration was one
  cc1psx could not judge small. func_8004AC18's target has two index insns
  between the `lui` and the `lw` of a pointer global, which `extern u32 *SYM;`
  cannot produce and `extern T *SYM[];` read as `SYM[0]` can.
* **An `__asm__` alias gives one function a different declaration of the same
  object.** `extern u32 X_arr[] __asm__("X");` declares a second C name for the
  same assembler symbol, so a function can have the incomplete-array
  (compiler-split) spelling while `extern u32 X;` and everything already
  compiled against it stay as they are. cc1psx honours asm labels on variables.
  This is what unparked func_8004EE90/func_8004F52C, whose target needs the
  split form of D_800814A0 while func_8004B500 -- in the same CARVED unit, on
  the same symbol -- needs the symbolic one. One carved unit stands in for
  several original translation units, and that is a contradiction only if you
  believe the carve.

  **The same alias works on FUNCTIONS, and arity is what it buys.**
  `void func_80054D64v(void) __asm__("func_80054D64");` lets one call site
  declare a callee as taking nothing while the rest of the unit keeps the real
  prototype. That is what a `jal` with NO argument setup means when the
  register holds a leftover: func_80054704's `$a0` still carries the `~0x200`
  mask from the `&=` two insns earlier. Passing the value explicitly is not the
  same -- cse does not delete a second `li` of a constant into `$a0`, so it
  costs a word.
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
  **A ONE-set pointer-to-symbol local can survive unfolded too, and then where
  it is ASSIGNED decides its register and which block materialises it.** The
  two-set rule above is about stopping combine folding the pointer back into the
  MEM; a single set suffices when the uses are negative-displacement
  (`q[-3]` off `&D_800814D3` stays `lbu -0x3($s1)` / `sb -0x3($s1)`). What the
  placement then buys is the rest: assigned in a block that DOMINATES the uses --
  an inner loop's preheader -- it becomes a global allocno in a callee-saved
  register, materialised where the listing has it, and func_8003F368 matches at
  113 words; assigned inside the handler alongside its uses it still survives
  unfolded but lands in caller-saved `$a0` and is materialised in the wrong
  block, same word count and 22 differing. Removing it and reaching the object by
  symbol throughout costs **7 words**, because the pointer-spelled accesses are a
  different memory expression from the `%gp_rel` ones -- which is what lets cse
  share one load between the test and the modify, and jump2 merge the two arms.

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
* **The compiler-split address form turns on the pseudo's USE count, not its
  SET count**, and with two uses the register is reload's spill register rather
  than an allocated one. An earlier version of this said the split form needs
  two sets reaching a join, and that one set folds back; both halves were
  incomplete.

  * **One set and ONE use in the same block folds** to `lui` + `lw %lo`, which
    costs the word. Measured four ways on func_8003CCB0, and duplicating the
    use into both arms of an `if` so each arm has its own set and its own use
    folds too. So the folding condition is one use per set in the same block.
  * **One set and TWO uses in different blocks gives the split form**, three
    words per use. A single-set pseudo with a `REG_EQUIV` constant is picked up
    by local-alloc's `update_equiv_regs`, its set insn is deleted, and reload
    rematerialises the value at each use -- and because cc1psx has split
    addresses on, a bare `SYMBOL_REF` is not a legitimate MEM address, so
    reload must build the whole address into a reload register: `lui` + `addiu`
    + a `0x0` displacement. The second use is what stops the address being
    folded into the MEM in the first place. The register follows from
    `reload1.c`'s `order_regs_for_reload`, which prefers registers no pseudo
    uses, in REG_ALLOC_ORDER -- in func_8003CCB0 that is `$a3`, and `.greg`
    prints `Spilling reg 7.` while the same function's other symbol addresses
    go through the identical path.
  * **Two sets reaching a join also select the split form, but then the pseudo
    is a genuine allocno**, inherits `preferences: 2` from the block-local
    `(high)` quantity and takes `$v0`. That is a different register from the
    one-set/two-use form, so the two are not interchangeable: func_8003CCB0
    needed `$a3` and the two-set shape gave it `$v0`.

  Two dead ends in this family: two single-set locals, one per site, interfere
  (cse merges them, the pseudo gets three references, is allocated and has its
  `lui` hoisted into the prologue); and mixing a two-set site with a
  single-set site is insensitive to where the single set goes.

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

### A walked pointer RE-LOWERS its symbol; a separate cursor does not

A listing with `lui $sA, %hi(SYM)` whose `%lo` is added TWICE -- once into
another register that is then incremented in place, and once back into `$sA`
after some calls -- is ONE pointer variable assigned the symbol and advanced
with `p++`:

```c
p = D_80083E18; *p++ = 8;               /* lui $s0; addiu $s1,$s0,%lo  */
strcpy((char *) p, (char *) s);
strcat((char *) p, (char *) D_80071404);
p = (u8 *) strrchr((char *) D_80083E18, 0);
                                        /* addiu $s0,$s0,%lo -- again  */
```

The in-place `p++` writes the pseudo that held the lowered symbol, so cse's
class {p, SYM} is invalidated and the next reference to SYM is lowered again
from the surviving `(high)` pseudo -- which is why `$sA` is the `lui`
register: the high dies into that second `lo_sum`. Write `b = &SYM[1]` with a
separate cursor instead and the symbol's pseudo stays valid, every later
`lo_sum` collapses onto it, one pseudo lives across all the calls, and the
second `addiu` is gone. The same reuse keeps the cursor in the callee-saved
register it already had, so the `addu $sB, $v0, $zero` copy after the call
survives too. Both together are worth 2 words and 69 DIFF in func_8004DF8C --
and the copy is the word func_8004DE88's park spent two sessions hunting in
local-alloc -- the same shape MATCHED it, because the cursor is then the walked
pointer's own pseudo rather than a quantity born from a copy of $v0.

### `sw X, -C($sN)` beside `sw Y, %gp_rel(SYM)` means ONE pointer, used

Two stores to adjacent small globals that come out in *different* shapes --
one off a callee-saved register with a small displacement, the next
`$gp`-relative to the symbol -- are `q[-1]` and `q[0]` through one pointer
whose VALUE was also used (written out byte-wise, passed, shifted) before the
call:

```c
extern u32 D_80081548;                  /* scalars, + .size for maspsx */
extern u32 D_8008154C;
q = &D_8008154C;  p[0] = (u32) q; ... ; f();
q[-1] = 0x2300030C;                     /* sw $v0, -4($s2)              */
q[0]  = 0x2300040C;                     /* sw $v0, %gp_rel(D_8008154C)  */
```

Three passes have to agree for this. The scalar declaration sets
`SYMBOL_REF_FLAG`, so cc1psx emits `la $sN, SYM` for the address -- and
maspsx's aspsx-2.56 config has `gp_allow_la` OFF, so gas expands the `la` at
-G0 into `lui`/`addiu` in the SAME register, which is the listing's pair.
cse's `find_best_addr` then replaces the bare `(mem (reg q))` with the symbol,
because `mips_address_cost` charges 4 for a REG and 1 for a flagged
SYMBOL_REF, giving the `%gp_rel` store; it leaves `(mem (plus q -4))` alone.
And combine folds `reg + const` into `SYM - 4` ONLY when the store is the
pointer's sole use, so the byte-write uses are what keep `-4($s2)` alive --
and what crosses the call in `$s2` is then q, the FULL address, not a high
half, which is one callee-saved register fewer than the symbolic stores need.

Measured in func_8004DF8C: plain `D_80081548 = X; D_8008154C = Y;` is 70 words
and 62 DIFF; incomplete arrays for both symbols are 74 words and 70 DIFF (two
`high` pseudos cross the call for `%lo(SYM)($sN)` stores, plus a fourth
callee-saved register); only the first symbol as an array is 72 words, 2 DIFF.
The offset's SIGN is irrelevant -- taking the addresses in the other order
with `q[+1]` matches the same way -- so read which symbol the pointer ends on
off the listing's displacement. One trap when switching a symbol from an
incomplete array to a scalar: `a = (u32) D_80081548` now reads the VALUE, not
the address. Write the `&`.

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
* **A 16-bit truncation keeps its `sra` when the extension's result cannot be
  combined into its consumer, and a basic-block boundary buys that for free.**
  combine does not downgrade `sra` to `srl` by looking at the extension insn; it
  does it when it SUBSTITUTES that insn into its single consumer -- typically the
  copy into `$v0` for a `return`. At that moment `simplify_shift_const` sees
  `(ashiftrt (reg) 16)` whose varop is a plain REG, consults `nonzero_bits`
  through `reg_last_set_nonzero_bits`, finds bit 31 clear and rewrites the code
  to LSHIFTRT. (It never simplifies the `sra` alone: `try_combine` of the two
  shifts fails because the merged `(sign_extend:SI (subreg:HI ...))` has no MIPS
  pattern, which is why both shifts survive at all.)

  combine's LOG_LINKS are built per basic block, so a consumer in a LATER block
  has no link, the insn is never re-examined and `nonzero_bits` is never
  consulted. Assign the value to a shared result variable and return it once --
  `if (c) { r = (s16) (*q & 0x400); } else { r = 0; }  return r;` -- or use a
  `goto out;` form. **Zero words**, and the value stays SINGLE-SET so none of
  the `andi 0xffff` that the multi-set shapes produce appears. That is what
  closed func_80042A80's last bit, and section 10's entry on the same subject
  was wrong for two sessions. **Expect it to apply to the `sll 24`/`sra 24` byte
  case too** -- func_8003D0F0's four `(s8)` conversions are parked on exactly
  this question.

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

**`>> 16` and `>> 24` on a word in memory ARE the narrow loads.** combine
turns `(ashiftrt (mem:SI x) 16)` into `(sign_extend:SI (mem:HI x+2))` on this
little-endian target, and `>> 24` into `(sign_extend:SI (mem:QI x+3))` -- so a
target that reads one word four ways looks like four differently-typed loads:

    d[0] = *(s32 *) (e + 0x48);          /* lbu 0x48          */
    d[1] = *(s32 *) (e + 0x48) >> 8;     /* lw 0x48 ; sra 8   */
    d[2] = *(s32 *) (e + 0x48) >> 16;    /* lh  0x4A          */
    d[3] = *(s32 *) (e + 0x48) >> 24;    /* lb  0x4B          */

Reading those as four FIELDS (`u8` at 0x48, `s16` at 0x4A, `s8` at 0x4B) gets
the same four addresses but narrows each one all the way to `lbu`, because the
only use is a byte store -- so an `lh`/`lb` whose value goes straight into an
`sb` is the tell that a SHIFT produced it, not a declared type. Measured on
func_8003A2E0: 14 DIFF as four fields, 7 as one word.

* **A narrow PARAMETER is not the same thing as a wide parameter with a cast,
  and the difference is a register.** `(u8 *p, s32 e)` with `(s16) e` at the
  use, and `(u8 *p, s16 e)` with a bare `e`, are the same arithmetic and
  different code. In the first, the parameter's pseudo is a plain copy of its
  incoming MEM (or argument register), so local-alloc's `update_equiv_regs`
  sees one set and one use and substitutes the MEM back into the use: the load
  SINKS to the point of use, `combine` folds the sign-extension into it as a
  single `lh`, and the value never needs a register. In the second, the entry
  insn is a promotion rather than a copy, the substitution cannot fire, the
  value lives from entry in a callee-saved register, and the sign-extension
  appears separately at the use.

  **The exact condition was read out of local-alloc.c on 2026-10-26** and it is
  narrower than "one set and one use". `update_equiv_regs` substitutes only
  when

      reg_n_refs[regno] == 2 && reg_basic_block[regno] < 0

  -- two references (the set plus ONE use) **and the pseudo spanning more than
  one basic block**. The comment there says so in as many words: "this case
  normally occurs when a parameter is read from memory and then used exactly
  once, not in a loop". So the substitution needs the use to be in a DIFFERENT
  block from the entry load, which is exactly the shape of a stack parameter
  used inside an `if`. A parameter used twice, or used in the same block it is
  loaded in, keeps its register.

  Two things follow that are easy to get wrong. Copying the parameter into a
  local at the top does NOT help: local-alloc ties the copy to the parameter,
  the combined quantity still spans blocks, and it is still substituted
  (measured on func_8003AB44 and func_8004A330). And a cse-merged second use
  does not count either, because `reg_n_refs` is computed by flow AFTER cse.

  Declaring the parameter narrow DOES block it -- measured twice, on
  func_8004A24C (55/57 to MATCH) and func_8003AB44 (52/54 to 54/54) -- but
  WHICH of the two conditions the promotion breaks is not established, only
  that it does. Do not reason further from that than the measurement supports.

  So a target that reads a stack parameter with `lw $sN, 0x44($sp)` at ENTRY
  and sign-extends it with `sll`/`sra` at the USE declared it narrow. A
  candidate that emits `lh 0x44($sp)` at the use instead, and saves one fewer
  callee-saved register, declared it wide. The cost is the `sw`/`lw` pair, two
  words: func_8004A24C went 55/57 -> MATCH on declaring arguments 4, 5 and 6
  `s16`, and func_8003AB44 went 52/54 -> 54/54 on argument 6 alone. It is not
  specific to stack parameters -- argument 4 of func_8004A24C is in `$a3` and
  behaves the same way.

  `advise.py` reports this as CALLEE-SAVED and names it, because the diff on
  its own reads as a frame-size difference and sends you looking for a spill.

### A narrow parameter a CALLER must not see: define under an `__asm__` alias

A parameter's width is read off the CALLEE, but declaring it narrow also
changes every caller that has the prototype in scope -- and in a carved unit
the callers were never in the same file as the definition. That looks like a
contradiction and it is not one; it is a declaration-scope problem with a
one-line fix:

```c
void func_80033AE8w(u16) __asm__("func_80033AE8");

void func_80033AE8w(u16 id) { ... }        /* emitted as func_80033AE8 */
```

Every existing `void func_80033AE8(s32);` in the unit, and every call site
behind it, is untouched: they name the assembler symbol this definition
provides. cc1psx accepts the asm label on a definition, and the body is
identical to the un-aliased one -- only the C name differs.

Give the alias a suffix that is NOT a hex digit (`w`, not `d`): `guard.py` and
`progress.py` recognise an aliased definition and count the LABEL, but their
`func_[0-9A-Fa-f]+` pattern would otherwise also match the C name and count
the function twice.

func_80033AE8 is the worked case. Its listing's `andi $a0, $v1, 0xFFFF` is the
PARAMETER's own truncation, which is what leaves the raw argument alive in
`$v1` for a later `& 0x1F` and lets the `sllv` take the constant 1 before the
`lw`; written `s32` with the mask in the source, the raw value and the
constant hold each other's registers and six of seventeen words differ. The
sweep row had rejected `u16` outright, because func_80039258's
`func_80033AE8((s16) v)` needs the `sll`/`sra` that a `u16` prototype in scope
replaces with a truncation. With the alias both sides are right at once and
the function MATCHES.

So when a narrow parameter matches the callee and breaks a caller, that is not
evidence against the narrow declaration. Check the callers' own listings, and
if they want the wide view, give the definition an alias. (carved-sweep.md's
`__asm__`-alias bullet is the same tool used for a DATA symbol's two
spellings.)

## 8. Statement order and expression shape

### sched1 defers a value SET EXACTLY ONCE, and that is a knob

sched1 has a register-pressure heuristic that sched2 does not: after it
schedules an insn it sets that insn's priority to `LAUNCH_PRIORITY`
(0x7f000001), and `schedule_insn` then calls `adjust_priority` on each
predecessor it releases. `adjust_priority` is a no-op once `reload_completed`,
but before reload it raises a **birthing insn** to `max_priority`, which at
that moment is `LAUNCH_PRIORITY`. The raised priority is cached, so from then
on that insn dominates every ready list it appears in.

`birthing_insn_p` (sched.c) is narrower than its name:

```c
if (GET_CODE (pat) == SET && GET_CODE (SET_DEST (pat)) == REG)
  if (bb_live_regs[offset] & bit)      /* dest live here */
    return (reg_n_sets[i] == 1);       /* set ONCE in the whole function */
```

So the insn qualifies only when its destination register is live at that point
AND is **set exactly once in the entire function**. Two consequences:

* **Call-argument setup never qualifies.** `addiu $a2, $zero, 0x49` sets the
  hard register $a2, which every other call in the function also sets, so
  `reg_n_sets` is far above 1 and the insn keeps its path priority.
* **A local assigned once does qualify**, and scheduling runs backward, so
  being picked first puts it as LATE as sched1 can place it. A local assigned
  TWICE does not qualify and stays where its dependencies put it.

That is a knob, and it is the opposite of the usual advice: splitting one
assignment into two, or merging two into one, changes `reg_n_sets` and so
changes where sched1 puts the insn -- without changing a single instruction.
Worth trying whenever a value's producer sits one slot from where the listing
has it and nothing about statement order moves it.

It is also the mechanism behind the stack-parameter prologue interleave
cluster (`docs/near-misses/func_8003719C.c`): a parameter read once is a
pseudo set once, so sched1 defers its `lw` behind the argument setup, while
the `addiu`s that set $a0-$a3 are pinned.

### A global's DECLARED SIZE also decides how many registers its address costs

The incomplete-array-versus-scalar choice is already known to decide `-G8`
placement and the `%gp_rel` form. It decides something else too, even for a
symbol that is absolute either way: how many registers the address takes.

```c
extern CVECTOR D_80080B28;      /* lui $a1 / addiu $a1,$a1,%lo -- ONE reg */
extern CVECTOR D_80080B28[];    /* lui $v0 / addiu $a2,$v0,%lo -- TWO     */
```

As a scalar the `(high sym)` pseudo dies into the `(lo_sum)` that follows it,
`combine_regs` ties them, and the pair shares one register -- which then
pushes whatever that address feeds (here the block copy's data word) down to
the next register. As an array the address is a separate quantity, the high
keeps the low-numbered register and everything after it shifts. Worth 13 words
on func_800499E8, where the struct copy's `lwl`/`lwr` reads through the
address and its `swl`/`swr` writes the frame.

So when a listing materialises a symbol's address into a register that is NOT
the one the `lui` wrote, try the array declaration before hunting for
pressure elsewhere.

### Truncating an `lbu` result is FREE -- an explicit `& 0xFF` is not

Two variables for one byte is a known shape (func_8005947C: the raw `s32` the
read returns, plus a narrow copy for the comparisons). Which spelling of the
narrow copy you need depends on what the listing shows:

```c
u8  c = d;        /* addu $v1, $v0, $zero -- the truncation is a no-op  */
s32 c = d & 0xFF; /* andi $v1, $v0, 0xFF  -- a real instruction         */
```

`d = *src` through an `lbu` already has clean upper bits, so a cast to `u8`
costs a plain move and the later comparisons use the narrow pseudo directly.
The explicit mask is an AND that nothing can delete, because the mask is on
the value and not on the type. A listing with `andi 0xFF` right after the load
wants the second form; one with `addu` wants the first. func_8004E298 needed
the mask; func_8005947C the copy.

### A value stored twice is a CHAINED assignment, because a store flushes cse

```c
v[0].vx = x;  v[2].vx = x;      /* TWO loads of x */
v[0].vx = v[2].vx = x;          /* one load, two stores */
```

cse's memory table is invalidated by any store it cannot prove distinct, and a
store to the frame against a load through a pointer is exactly that case. So
re-spelling the expression after a store re-loads it. The chain also fixes the
ORDER: C assigns right to left, so `v[0].x = v[2].x = e` stores v[2] first,
which is what a listing that writes the far slot first is telling you. A
ten-deep chain of zero stores comes out in exactly descending order, and on
func_80037714 that reproduced ten `sb $zero` and three colour bytes on the
first try.

### Four `||` terms whose `or`s sit in the delay slots are `|`, written INLINE

`a || b || c || d` over four int LOCALS computes all four and then emits the
three `or`s together at the end (or, worse, three branches). A listing that
interleaves each `or` into the NEXT term's branch delay slot is telling you the
terms are inline in one left-associative `|` expression:

```c
if ((A0 && B0) | (A1 && B1) | (A2 && B2) | (A3 && B3))
```

`|` folds the running value before the next term is evaluated, so the RTL order
is term, term, or, term, or, term, or -- and the inner `&&`s keep their own
short-circuit branches, which is where the `or`s then land. Worth 7 words on
func_80037714.

### `% C` is NARROWED by the front end, and a cast will not stop it

`build_binary_op` sets `shorten` for TRUNC_MOD_EXPR (and for `*`, `/`, `+`,
`-`), so `*(s16 *) p % 64` is computed in HImode and converted back to int,
which is a `sll 16`/`sra 16` pair -- combined with a following `* 4` into the
single `sll 16`/`sra 14` a listing shows. If the listing has NO such pair, the
operand has to be genuinely `int`, and a cast does not achieve that:
`get_narrower` strips NOP_EXPRs and finds the `short` underneath. An `s32`
local assigned the load does:

```c
u = *(s16 *) (p + 8) % 64 * 4;          /* narrowed:  sll 16 / sra 14 */
y = *(s16 *) (p + 0xA); u = y % 256;    /* not:       plain subu      */
```

**`d * 2` and `d << 1` are NOT the same instruction count on a `u8`, and the
difference is a mask.** Measured on func_80057D20, with `d` a `u8` parameter:

```c
e->unk_6C = d * 2;    /* andi $v0, $s4, 0xFF ; sll $v0, $v0, 1 */
e->unk_6C = d << 1;   /* sll  $v0, $s4, 1                      */
```

The multiply materialises the zero-extension as its own insn; the shift lets
combine fold it away, because the byte store truncates and only the low eight
bits of the result are wanted either way. On a WIDER destination the same
difference decides whether a neighbouring statement's mask is shared: with
`e->unk_08 = d;` ahead of it, `e->unk_40 = d * 2;` reuses that statement's
`andi` and `e->unk_40 = d << 1;` gets a second one, which is what the listing
has. So `<< 1` is right in both places and `* 2` is wrong in both, by one word
each -- and because the two errors cancel in the total, a word count alone
will not tell you. func_80057D20 read as 463 of 463 for a while with both
wrong.

### An index INLINE in a call argument gets the hard register; a local does not

For `AddPrim(base + n * 4 + 0x70, prim)` the five instructions are the same
either way, but which of the two operands ends up in `$a0` is not.
`block_alloc` walks the add's operands in order and ties operand 0 to the
insn's destination -- here the hard argument register -- so with the index in
its own local the TIE goes to the loaded base and the index chain is left in
`$v0`. Written inline the tie lands on the index chain instead and it gets
`$a0`, which is what func_80037714's three `AddPrim` calls want: 15 words.

This is the opposite of func_800369C8 and func_80036A70 in the same unit,
where the index MUST be a local (`o = n * 4 + 0x70;`) or the `0x70` folds onto
the loaded base. The difference is that there the base is a local too, so the
add has no hard register to tie to and only the constant's binding matters.

### cse rewrites `*q` into `*(p + C)` but leaves `*(q + 4)` alone

A listing that reads a struct's FIRST field through the parameter and every
other field through a second pointer looks like two source pointers and is
usually one. For `q = p + 0xC`, cse substitutes a MEM address that is a bare
register with that register's recorded equivalence, so `*(u16 *) q` becomes
`0xC($t0)`; for `*(u16 *) (q + 4)` the address is `(plus q 4)` and folding it
would need the equivalence of a REG inside an expression, which
`equiv_constant` only does for constants -- so it stays `4($s1)`. Write every
field through the second pointer and let cse produce the mixture.

### A symbol added to an index expands SECOND; a pointer variable expands first

`p = D_80085458 + b * 0x78` and `u8 *q = D_80085458; p = q + b * 0x78` compile
to the same five instructions in a different ORDER, and on func_8005C2C0 that
order was worth 26 words.

The array name decays to an `ADDR_EXPR`, which is `TREE_CONSTANT`, so `fold`
canonicalises the sum with the constant operand LAST: the tree is
`MULT_EXPR(b, 0x78) + ADDR_EXPR(D_80085458)` and the three index shifts expand
before the symbol's `high`/`lo_sum` pair. sched1 then has the longer of the
two chains available first and lifts it into the shadow of whatever load is
nearby -- here the `lw` that fetches `st.addr`. `combine_regs` in local-alloc
ties the index pseudo and `p` into one quantity, so that quantity is now BORN
while the load's destination is still live, and `p` cannot have `$v0`. Every
register in the block after it shifts.

Assigning the symbol to a pointer variable first makes it a variable, fold
leaves it as operand 0, the symbol pair expands first and fills the load's
shadow instead, and the index chain -- and so `p`'s quantity -- starts after
the load's value is dead.

Spelling the sum the other way round (`b * 0x78 + D_80085458`), using
`&D_80085458[b * 0x78]`, or hoisting the multiply into its own local all
change nothing: fold normalises them to the same tree. Only making the base a
variable moves it.

### Read a field BACK instead of respelling the expression -- cse makes a MOVE

A field stored and then copied into a second place has two spellings, and they
differ by one instruction:

```c
*(u16 *) (p + 0x10) = f << 7;
st.volume.left = f << 7;                    /* cse shares the shift: 0 words */
st.volume.left = *(u16 *) (p + 0x10);       /* cse makes a reg-to-reg MOVE   */
```

In the second, cse replaces the load's source with the register that was
stored, which leaves `(set (reg t) (reg stored))` -- a real `addu $a0, $v1,
$zero` in the output. That move is exactly what a listing shows when the same
value reaches two places through different registers, and it is otherwise
impossible to ask for: writing the expression twice shares one pseudo and the
move never exists.

The read-back is not always forwarded, and a listing that forwards one field
and reloads the next (`addu` for +0x10, `lhu` for +0x12) is telling you both
were written as read-backs -- not that one of them was spelled differently.

### Read the scheduler's own decisions -- it is not "priority, then LUID"

`cc1psx` is gcc 2.7.2 and still honours gcc's `-d` dump letters. The `ccpsx`
driver swallows them, so `tools/decomp/sched.py` runs `cc1psx` directly on
preprocessed input and prints the dump for one function:

    python3 tools/decomp/sched.py cand.c func_8003719C          # sched2
    python3 tools/decomp/sched.py cand.c func_8003719C --sched1
    python3 tools/decomp/sched.py cand.c func_8003719C --insn 27

`-dR` gives sched2 (after reload, hard registers -- the pass that interleaves
the prologue saves with the body) and `-dS` gives sched1. The dump names every
insn's priority, the ready list at each cycle, every hazard decision, and the
scheduled RTL, so an insn number in a ready list can be read back to an
instruction with `--insn`.

Several write-ups in this repo blamed a scheduling difference on
"sched's tie-break is `INSN_LUID`, i.e. RTL order". That is the THIRD key, and
reading `sched.c` with the dump next to it shows the order the keys actually
apply in:

1. **`priority (insn)`** (sched.c:1425) is the longest dependency path to the
   insn from the block START along `LOG_LINKS`, each edge contributing
   `insn_cost (x, link, insn) - 1`. So **when every latency on the path is 1
   the priority is 1**, and because the path is measured from the start the
   whole top-of-function region sits at priority 1 whatever the function does
   later: func_8003719C's priorities run 1 to 11 with every insn in its entry
   block at 1, and func_8005947C's are 1 throughout. Within a priority group
   this key does nothing.
2. **`schedule_select`** (sched.c, the `greater potential hazard` message)
   walks the rank-sorted ready list **in groups of equal priority**, queues
   every insn in the group whose `actual_hazard` is non-zero, and then picks,
   among what is left, the insn with the **largest `potential_hazard`** -- the
   `DEFINE_FUNCTION_UNIT` model, not the dependence graph. Memory operations
   therefore beat ALU operations inside a priority group, which is why prologue
   saves win against an `addiu` that sets up a call argument.
3. Only on a tie of all of the above does `rank_for_schedule` fall through to
   the dependence class relative to `last_scheduled_insn` (independent beats
   anti/output beats data) and then to `INSN_LUID`.

Scheduling runs BACKWARD: the dump's `T-N` counts cycles from the block end,
so the insn chosen at the LARGEST `T` is the one emitted first. That is also
why priority grows downstream, and why the block's last insn carries a
sentinel priority near `INT_MAX` -- the backward pass has to take it first.

Read the dumps `tools/decomp/dump.sh` collects, which is what `sched.py` does,
rather than invoking `cc1psx` by hand: the driver passes flags of its own, so
a hand-rolled invocation is a second compile path that can disagree with the
build.

The practical consequence: when a diff is two insns in the wrong order and one
of them touches memory, the lever is not statement order. It is whatever
changes the priority group they land in, or the hazard state at that cycle.


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

* **An array reference and pointer arithmetic produce the two operand orders of
  the index+base `addu`, and no source spelling overrides it.** `arr[i].field`
  always yields index-first (`addu $v0,$v0,$s2`); any pointer-deref spelling
  always yields base-first (`addu $v0,$s2,$v0`). Five spellings were measured on
  func_8003E758 -- `(u8 *) ARR + i*24 + 4`, `i*24 + (u8 *) ARR + 4`,
  `i*24 + (u32) ARR + 4`, `(u8 *) &ARR[i] + 4` and `(u8 *) (ARR + i) + 4` -- and
  all five came out base-first, because the front end canonicalises
  `int + pointer` to pointer-first and `expand_expr` builds an array address
  with `EXPAND_SUM`. That is 4 differing words if you pick the wrong one, and it
  forces a split within one function: func_8003E758's entry access and `unk_04`
  loads are array references while its `case 0xFF` `unk_08` read is a pointer
  deref, because the target computes that one address base-first.
  `docs/compiler-version.md` says the two spellings are "no longer equivalent";
  this is what differs.

* **`balance_case_nodes` counts case nodes whose tests are optimised away
  entirely, so a switch's decision tree is evidence for case labels that emit
  NOTHING.** A case whose body is `break` and whose value is a single point in a
  gap, bounded above and below by its siblings and sharing the default's label,
  has its test collapsed into the jump the default already needs -- invisible in
  the listing, and yet it shifts every split point in the tree. func_8003E758
  needs `case 0xF` and `case 0x16` for exactly this: without them its first
  switch roots at `case 0xA` instead of the `0xB`/`0xC` range and the right
  subtree roots at 0x1B instead of 0x15.

  **The recipe for recovering the set**, which is what closed that dispatch:
  compile a probe switch with distinct empty-bodied call arms, extract the
  sequence of `slti`/`li` constants from the cc1psx output, and search over
  candidate case sets until the sequence matches the target's. The split rule
  was confirmed empirically on the way -- for a three-node sublist the root is
  the middle node, so the target's `1B -> FC -> FF` chain required a fourth,
  invisible node in that sublist. See also the range-node entry above, which is
  the same pass counting a *run* of such cases double.

### A `nop` in a CONDITIONAL branch's delay slot can mean a LOAD comes next

Separate from the `jal` rule below. reorg will not move a `may_trap_p` insn
into a slot that executes on both paths, so when the fall-through block begins
with a `lw` the slot stays empty, and when it begins with a `lui` reorg fills
it. The nop after func_8003800C's loop-closing `bne` is therefore evidence
about what sched2 put FIRST in the next block, not about dependences -- read
it as "the next block starts with a load" and work backwards to the statement
order that puts one there.

### Two loads of one MEM collapse at greg unless they overlap at SCHED1 time

cse is not what merges two reads of the same memory: `invalidate_memory` drops
every varying-address MEM on ANY store, so one intervening store is enough to
keep both, and both survive to `lreg`. What deletes the second one is
local-alloc giving the two pseudos the same hard register, after which reload
sees `$v1 = $v1`; with different registers it degrades to a reg-reg copy
instead (a word, but the wrong word). The pseudos share a register exactly
when their live ranges do not overlap **in sched1's output**, which is not the
order you wrote and not the order sched2 finally emits.

So a one-word shortfall of this shape is a SCHEDULING question. Statement
order is still the knob, but score it against `sched.py --sched1`'s ready
lists rather than against the source: in func_8003800C every placement of the
second read among the block's five stores (sixteen combinations) leaves sched1
emitting it after the first read is already dead.

### An empty `jal` delay slot says the address was ONE instruction

An address materialised for a call argument comes out either as the
assembler-expanded `la` -- one insn before maspsx, so reorg cannot split it and
the `jal`'s delay slot goes empty -- or as the compiler-split lui/addiu, whose
`addiu` reorg drops straight into that slot. So three `jal`s with `nop` delay
slots (func_8003B06C) say the three symbols were small data, and a listing
whose `addiu %lo` sits in a branch delay slot with the `lui` elsewhere
(func_800494B4) says the opposite. Declaring the object `extern u8 X[8];`
rather than `extern u8 X[];` is what picks the first form. Only the size CLASS
is evidence -- small enough for -G8 -- not the exact size.

**The same rule governs CONDITIONAL branches.** `eligible_for_delay` rejects
any insn whose `get_attr_length` is not 1, and the assembler-expanded form is
ONE insn of length 2, so a `beq` whose fall-through block begins with such a
materialisation keeps its nop while a compiler-split `addiu` would drop
straight in. func_8003BAF8's `*(u32 *)(q + 4) = (u32) &D_800812F8;` needs the
scalar spelling for exactly that one word.

### N materialisations of ONE base mean N LOCALS, because cse restarts at a join

cse is an extended-basic-block pass: `cse_basic_block` follows a chain of
blocks with single predecessors and starts fresh at a join. So a table base
spelled through a pointer local is one pseudo for as far as that chain reaches,
and a SECOND assignment of the same symbol after a join is a second pseudo that
cse cannot prove equal to the first. Count the `lui`/`addiu` pairs for one
symbol in the listing and use that many locals, each assigned in the region
that uses it: func_8003BAF8 has three for D_8006ADEC and needs three. Reusing
one variable is one pseudo whose live range spans the calls between the
regions, which costs a callee-saved register and its save/restore pair.

**And a local's INITIALISER is evaluated at block entry, not where it is
written.** `u8 *b = SYM;` ahead of the function's first call puts the
materialisation before that call and makes the pointer live across it;
`u8 *b; ... f(); b = SYM;` puts it after. Two words of prologue in
func_8003BAF8, and it reads as an allocation problem.

### The PREHEADER's giv initialisations are in REVERSE body order

`record_giv` prepends to loop.c's giv list, and `strength_reduce` walks that
list emitting each initialisation immediately before the loop start -- so the
giv found LAST in the body gets its `lui`/`addiu` or `addu` FIRST in the
preheader. Read the preheader's order off the listing and put the
corresponding references in the body in the opposite order: func_8004F684's
listing starts its preheader with D_80071784's base and then `addu $t3, $t0`,
so the `p + i * 4` reference has to be the body's FIRST statement and the
`&D_80071784[i * 3]` its second. Three words, and the last thing wrong in it.

### Two offsets off a walked pointer become TWO induction variables

`u8 *p = SYM; ... *(u32 *) p ... *(u16 *) (p + 4) ... p += 8;` makes loop.c
create a giv per offset, so the listing gets two registers advancing in step.
Indexing a struct array by the counter instead -- `SYM[i].unk_00`,
`SYM[i].unk_04` -- leaves it ONE pointer with both offsets as displacements,
which is what these targets have. func_800481E0, 18 DIFF; it is also most of
what is left on func_8004A6C0, where the same change took 20 DIFF to 16 and
one word.

### Indexing a base pointer keeps it SET ONCE, which is a second lever

The lever above is about how many induction variables you get. Keeping the
base pointer un-walked buys a second thing on top of that: `reg_n_sets` for it
stays at 1, so sched1's birthing-insn rule (section 8) applies to its
initialisation and raises it over whatever else is sitting at priority 1 in
the preheader.

func_80059E94 is the whole of that. Its preheader is `i = 0`, a hoisted
`lui $a3, 0x8` for the 0x80000 bound, and `addu $a0, $t0, $zero` for the table
base. Written `p = D_800869C0; for (; i < 0x10; i++, p += 2)` with `p[0]` and
`p[1]`, the pointer is set TWICE, its initialisation stays at priority 1, the
ready list breaks the tie on RTL order, and the constant comes out one slot
LATE -- which costs the registers too, because `$a3` is then free to carry the
table address and the constant in turn instead of the listing's `$t0` and
`$a3`. 62 words, 4 DIFF, and nothing about statement order moves it: the
hoisted invariants go in before `loop_start`, so they are always behind a
for-init.

Written `p = D_800869C0; for (; i < 0x10; i++)` with `p[i * 2]` and
`p[i * 2 + 1]`, strength reduction supplies the single `addiu $a0, $a0, 0x8`
giv, `p` is set once, sched1 raises it, and the constant lands first. MATCH.

Spelling the base out at each row instead -- `D_800869C0[i * 2]` -- is not the
same thing: that gives TWO givs which `combine_givs` does not merge (64
words), and so does hoisting a row pointer `u32 *q = &p[i * 2]` out of the
references (63 words, with a `sll` left recomputing it).

### A loop-invariant pointer belongs INSIDE the loop body

Assigned as an initialiser before the loop, its materialisation lands ahead of
the loop's guard branch; written inside the body, loop-invariant motion hoists
it into the PREHEADER, after the guard, which is where these targets have it
(func_8004CCBC, func_8003B7C8's `lui`). This is the opposite of the rule below
for a pointer live across a CALL, and the two do not conflict: a guard branch
is not a call, and nothing has to survive it.

### A MUTATED cursor cannot be hoisted -- write the guard yourself

The lever above gets a loop-INVARIANT pointer into the preheader by writing it
inside the body. A cursor the loop advances cannot go there: written in the
body it would reset every iteration, and written in the for-init it is
evaluated BEFORE the guard, where sched2 floats it to the top of the entry
block. The listing shows which one the original had -- a cursor's
materialisation sitting in the preheader, between the guard branch and the
loop label, alongside loop.c's own giv inits.

The way to put it there is to write the guard in the source and make the loop
a `do`/`while`:

```c
i = 0;
if (n != 0)
{
    u8 *q = p;
    u8 *d = b;

    do { ... d += 4; q += 20; i++; } while (i < n);
}
```

An inner scope inside the `if` is a basic block of its own after the guard,
and the `do`/`while` contributes no second test, so the count is unchanged.
Wrapping a plain `for` in the same `if` gets the placement right but pays for
the loop's own redundant guard -- one word -- which is the tell that the
original was a `do`/`while` all along.

Keep the OTHER initialisers outside the guard. func_8004AA78 wants `k = 0` in
the function's first slot and `i = 0` in the guard branch's delay slot: with
`i = 0` inside the `if` there is nothing before the branch for reorg to take,
so it pulls `k = 0` down into the delay slot instead and the whole entry block
shifts.

### reorg fills a delay slot from the insn BEFORE the branch first

`fill_simple_delay_slots` searches backward from the branch before it looks at
the fall-through, so whatever statement you leave immediately ahead of a guard
is what lands in its slot. When the listing has the FALL-THROUGH's first insn
there instead, the pre-branch candidate was taken out of reach -- usually by
moving another statement in front of it.

func_80049004 stores the allocation and then advances its cursor. With `p++`
inside the `r != 0` arm the slot gets `sw $s1, 0x0($s0)` and the store and the
test come out swapped (2 DIFF). With `p++` ahead of the test, the slot gets
`addiu $s0, $s0, 0x4` and the store stays where the listing has it. MATCH, and
nothing else in 83 words moved.

The same reading in reverse: a guard whose slot holds something from INSIDE
the arm says the arm's first statement is the only candidate, which is a hint
about what is and is not before the test (func_8004AA78's `i = 0`).

### Reusing one counter beats a second one, and that decides its register

A loop that counts the same thing a previous loop counted is often the SAME
variable reset, not a fresh one. It is worth trying for the register, not just
the tidiness: one allocno across two loops has both loops' references, so it
outranks whatever else the second loop's preheader holds. func_8004AA78's
third loop re-walks the buffer it filled in the first; written with its own
`c`, `c` is allocated before the hoisted buffer base and takes $a0, and the
base falls to $a1 -- the listing has them the other way round. Resetting `k`
after the intervening call instead is 6 words, and MATCH.

**And the inverse, which is the same lever read backwards.** When the listing
gives two loops DIFFERENT registers for the same role, they were different
variables. func_800506BC walks $a0 then $v1 and accumulates in $a3 then $a0;
written with one cursor and one accumulator shared between the loops, each
role is a single allocno and both loops come out on the same pair -- 18 wrong
words with every opcode already right. So read the registers off the listing
before deciding: one variable or two is a measurable choice either way.

### `p = param` BEFORE the first branch moves that parameter's copy LAST

`assign_parms` emits the hard-register copies in declaration order, so the
first parameter's `addu $s3, $a0, $zero` is normally the first of them, and
sched2 interleaves the prologue saves in that order -- each save pulled in
directly ahead of the copy that clobbers its register. A listing whose
save/copy pairs run $a1, $a2, $a3, $a0 is telling you the first parameter's
copy is RTL-LAST, and the way to get that is to assign it to a local:

```c
p = base;               /* before anything that branches */
if (n == 0) { return; }
```

combine folds `(set p base)` into the parameter copy by substituting into the
LATER insn and deleting the earlier one, so a0's copy ends up at the
assignment's position -- behind a1, a2 and a3. Walking the parameter itself
instead leaves it first, which is 8 wrong words of prologue on
func_8004AEC4.

**The assignment has to be ahead of the first branch.** Put it after the
guard and the folded copy lands in the NEXT basic block, so it comes out
behind every remaining save and behind the guard branch itself -- 10 wrong
words rather than 8. Same lever, overshot.

This is not the `sched1` birthing rule: sched1 does not schedule the parameter
block at all (its block 0 is one insn, the branch), and at sched2
`adjust_priority` is a no-op. It is RTL order, and the assignment is what sets
it.

### Where a loop's counter is INITIALISED decides which register it gets

Two values that live the same shape of life can tie at global-alloc to within
a percent, and then one insn of live range settles them. func_8004AEC4's
inner loop has a counter `j` and a walking pointer `q`, 13 refs over 12 insns
against 14 over 13: `floor_log2(13) * 13 / 12` is 3.250 and
`floor_log2(14) * 14 / 13` is 3.231, a 0.6% gap, and the lower-numbered
callee-saved register goes to the winner.

`for (j = i + 1; j < n; j++)` gives the counter $s0 and the pointer $s1.
Lifting the initialiser out -- `m = p; j = i + 1; q = p + size;` and a
`while (j < n)` -- is the same instructions in the same order but one insn of
live range moved, and the registers swap to the listing's. Order the three
initialisers to match the listing as well: `j` first and `m = p` comes out a
slot late.

When `advise.py --greg` reports an adjacent pair inside a few percent, this is
the cheapest thing to try, because it costs nothing in the output.

### An early `return x` is another REFERENCE to x, and it can flip global-alloc

`allocno_priority` is `floor_log2(refs) * refs * size / live_length`, so every
extra reference counts -- and `return x;` in five arms is five references to
`x` even when jump.c later merges them. func_8004C080 was 187/187 words with
32 register-only differing words for exactly this: five `return q;` gave `q`
17 refs against `e`'s 17 with a shorter live range (152 against 188), so `q`
took $s3 where the listing has `e` there, and the whole prologue and every
base register followed. Rewritten as a plain if/else-if chain with ONE exit at
the end, the two swap and it matches.

So when `advise.py --greg` shows two allocnos tied on references and the
listing wants the longer-lived one first, count the function's `return`
statements before touching anything else -- collapsing them is free.

### A base pointer assigned before a call lives in a callee-saved register

`u8 *p = (u8 *) SYM;` ahead of two calls costs three words -- the `sw`, the
`lw` and the frame slot for `$s0` -- that a target which materialises the same
address AFTER the calls does not spend. The fix is to move the assignment below
them, which is a one-statement edit worth 15 differing words on func_8003B92C
and 3 words plus a register on func_800540A8. The same reading applies to any
address or loaded value whose first use is after a call: if the listing has no
callee-saved register in the frame, nothing in the source may be live across
the call.

### One call after the join, not a call in each arm

func_800494B4 sets a pointer in each arm of an if and calls once after it. With
the call inside both arms the argument setup for the OTHER parameter is
duplicated into each arm and the `jal`'s delay slot goes empty: 14 DIFF on a
17-word function, and nothing about it reads as a structural difference in the
diff.

### Two bounds written with `&&` become one range check

`if (x > 0 && x < 4)` is folded into `addiu -1` / `sltiu 3`, one branch where
the target has two; so is `if (id != 0 && id != 1)`, into a single
`sltiu id, 2`. Nested ifs keep the two branches, and cse still shares the one
load between them. Worth 13 DIFF on func_8005A33C, and it is half of what
parks func_80033AA8.

### Two shifts with mismatched counts are two shifts in the source

`(x << 16) >> 8` cannot be combined -- the arithmetic right shift fills from
bit 15, so the result is not `x << 8` -- and that pair is what a listing with
`sll 16` / `sra 8` is showing. Every spelling that means "sign-extend, then
shift" (`(s16) x << 8`, an s16 parameter, a multiply by 0x100) collapses to the
single `sll 8`, because the value is stored as a halfword and combine drops the
extension. func_8005B3D8, 2 words.

### `break` and a tested `while` condition pick DIFFERENT registers

`do { ...; q = next; p = q + K; } while (q != 0);` and
`while (1) { ...; q = next; if (q == 0) break; p = q + K; }` generate the same
instructions in the same order -- and give `q` a different hard register, $v0
for the tested condition and $a0 for the break. func_80053A88 was 109/109 with
three register-only differing words until the `break` form went in; the type
of `q`, its declaration position and reusing an existing local for it all left
it in $v0. Try the other form before reading an allocation dump.

### No re-test at the bottom means that is not the loop condition

A `while (COND) { ... if (X) break; ... }` re-tests COND at the bottom of the
loop. If the listing's backward branch tests only X and nothing re-tests COND,
then COND was an `if` around the loop and the loop itself is a `do`/`while` on
X -- and whatever the body does first belongs at the TOP of the body, where it
lands in the first branch's delay slot. func_8004CE68, 9 DIFF.

### Many arms rejoining at several places is LABELS AND GOTOS, laid out to match

When a dispatch's arms rejoin at more than one place, the listing's physical
block order is the thing to reproduce and nested if/else cannot produce it.
Write the arms with `goto`, put each join block where the listing has it, and
end a join block with `goto` only where the listing spends a `j`.

func_8003B470's layout is dispatch, five arms, `join1`, a one-call block,
`tail`, `other`, `join2`. The giveaway is that `other` -- whose branch is the
EARLIEST of the five -- comes LAST, because it falls through into `join2` where
every other path has to jump there. As nested if/else that block lands ahead of
`join1` and the whole tail shifts: 119 words and 48 DIFF against MATCH at 117.

Two rules that came out of it:

* a join block reached by a branch is written as `if (cond) goto label;` and
  not as an `else` arm, or gcc spends a `j` the listing does not have;
* the block the listing puts last is the one that FALLS THROUGH into the final
  join, so write it last and let the earlier join blocks `goto` past it.

`goto` is already used in csrc/unk_11F0C.c and unk_12DEC.c for the same reason
-- see the "NOT A LOOP" note there, where the label form is what keeps loop.c
out.

### A comparison TREE where you get a jump table means a case you have not found

cc1psx picks between a jump table and a comparison tree in `stmt.c:4817`:

```c
      else if (TREE_INT_CST_HIGH (range) != 0
	       || count < CASE_VALUES_THRESHOLD      /* 4 with casesi, else 5 */
	       || ((unsigned HOST_WIDE_INT) (TREE_INT_CST_LOW (range))
		   > 10 * count)
```

`count` is the number of case NODES after `group_case_nodes` merges adjacent
ones with the same label, and `range` is `maxval - minval` over the case
values. So a tree needs either fewer than five nodes or a span wider than ten
times the node count. func_80058E6C's five scattered meta-event cases
(0x2F, 0x51, 0x54, 0x58, 0x59) span 42 for a count of 5 -- 42 is not more than
50, so cc1psx emits a jump table and the candidate is ten words short with
nothing in common with the listing. The listing's tree only becomes reachable
with a SIXTH node low enough to widen the span: a case range 0x00..0x0F
sharing the default's label (the text meta-events), which makes the span 89
against a count of 6.

**A `bltz` in a switch's dispatch is the signature of such a range node.**
`emit_case_nodes` gives a range node `blt node->low -> default` followed by
`ble node->high -> node->code_label`; when `node->low` is 0 the first is a
bare `bltz`, and when the node shares the default's label the second branches
to the default too. Two low-bound checks and no matching equality test is
therefore not a mystery -- it is one range node whose body is the default's,
and the case values it covers are `low` and `high` read straight off the two
compares.

### A switch's range split means there is a case label you have not found

`emit_case_nodes` (stmt.c:5664) has its own shortcut: *"omit the conditional
branch to default if we avoid only one right child; it costs too much space to
save so little time"*. So a two-case switch comes out as two plain equality
tests and NO range comparison. A listing that tests one case for equality and
then splits the range -- `beq x, LO` then `slt t, x, LO+1` / `bnez t` to the
default -- has at least THREE case nodes, and the third is a label whose body
is the default's: gcc emits the root's equality, the `slt` to separate the two
subtrees, then the left child's equality, which jump.c deletes because its
label is already the default's, and inverts the branch over the `j default`
that is left.

So add a case label on the other side of the root and group it with
`default:`. func_80051228 needs one below 2, and the compiler pins it no
further than that -- `case 0:` and `case 1:` are byte-identical. Two words,
and it also tells you the original had a label there.

### A switch's range split means there is a case label you have not found

`emit_case_nodes` (stmt.c:5664) has its own shortcut: *"omit the conditional
branch to default if we avoid only one right child; it costs too much space to
save so little time"*. So a two-case switch comes out as two plain equality
tests and NO range comparison. A listing that tests one case for equality and
then splits the range -- `beq x, LO` then `slt t, x, LO+1` / `bnez t` to the
default -- has at least THREE case nodes, and the third is a label whose body
is the default's: gcc emits the root's equality, the `slt` to separate the two
subtrees, then the left child's equality, which jump.c deletes because its
label is already the default's, and inverts the branch over the `j default`
that is left.

So add a case label on the other side of the root and group it with
`default:`. func_80051228 needs one below 2, and the compiler pins it no
further than that -- `case 0:` and `case 1:` are byte-identical. Two words,
and it also tells you the original had a label there.

### A two-case store chain whose opcodes are `beq` then `bne` is a SWITCH

For three arms that each store a different constant to one field, an if-else
chain gives `bne`/`bne` and lays the bodies out in source order. A `switch`
gives something an if-else chain cannot: gcc's `emit_case_nodes` emits
`beq <low value> -> its body` and then `bne <high value> -> default`, FALLING
THROUGH to whichever case body is written first. So

```c
switch (m) {
case 0x9000: *(s16 *) (d + 0x16) = 4; break;   /* falls through to here */
case 0x8000: *(s16 *) (d + 0x16) = 2; break;   /* the `beq` target */
default:     *(s16 *) (d + 0x16) = 0xA; break; /* the `bne` target */
}
```

reproduces a listing whose tests read `beq 0x8000` / `bne 0x9000` with the
`= 4` arm in line -- note the case written first is NOT the one tested first.
func_800550E8: four if-else orderings measured at 48 to 51 DIFF, the switch at
45.

Hoist whatever the three arms share above the test; repeated in each arm it
costs two words per arm (five on that function).

### Read the block ORDER off the listing and write the arms to match

For an `if`/`else` whose arms all end the function, the listing's physical
order of the arms is the thing to reproduce, and which source order gives it
depends on how many arms there are. Two measured cases, both costing the branch
opcode plus every word after it:

* **Two arms, each a `return`:** the arm written FIRST becomes the branch-over
  block and the one written SECOND becomes the fall-through, so the arm the
  listing places LAST is written first. 6 DIFF on func_8003C8E4 (`bltz` where
  the target has `bgez`), 3 on func_8005AC30.
* **Three arms, i.e. an outer `if` and a nested one:** the outer arm the
  listing places last is written LAST, inverting the outer condition.
  func_800381D0, 18 DIFF, and func_80034FFC the same way.

So do not apply either rule from memory: find which arm the listing puts last,
try it in both positions, and keep the one that matches.

Related, on the same two functions: `return f(a != 0);` is not the same as two
returns of `f(1)` and `f(0)`. The first store-flags the condition into one
`sltu` (13 DIFF on func_8005AC30); `if (a) b = 1; else b = 0;` does the same,
because if-conversion does not care that the arms were written as statements.
Two returns of the same call keep the branch and crossjumping merges the tails.

### Spell the whole expression out in each arm, not a shared temp after the join

When both arms of an `if` compute the same shape with different constants and
feed one expression after the join, writing the common part once -- a shared
`n` set in each arm, then one `n * 4` and one dereference below -- gives the
SAME instructions as repeating the whole expression in each arm, because
`do_cross_jump` merges the identical tails either way. It does not give the
same REGISTERS. With the shared temp, `n` is set in two blocks, so it is a
global quantity and global-alloc assigns it after local-alloc has placed the
per-arm loads; the two end up in each other's registers. Written out per arm,
every quantity is block-local and they are allocated in listing order.

func_80049374: 11 DIFF at the exact word count with

```c
if (e->unk_00 == 0) { n = p[0] * 5; } else { n = p[0] * 3; }
s = *(u8 **) (e->unk_0C + n * 4 + 8);
```

and a MATCH with

```c
if (e->unk_00 == 0) {
    s = *(u8 **) (e->unk_0C + p[0] * 20 + 8);
} else {
    s = *(u8 **) (e->unk_0C + p[0] * 12 + 8);
}
```

Also measured on it, all 11 DIFF or worse: `n` declared at function scope, `n`
declared first in the block, the `+ 8` written before the index, a separate
`n *= 4;` statement, the arms swapped with the condition inverted, and the
`unk_0C` load hoisted into a local above the `if` (that one goes a word SHORT).
So try the per-arm spelling before hunting the allocation.

### Two masks of one word are two `&=` statements

`D_800847D0[0] = D_800847D0[0] & ~0x100 & ~0x1000;` is ONE `and`: fold merges
the constants into `addiu $v1, $zero, -0x1101`, and a listing that spends two
`addiu`/`and` pairs on it is then a word short. Written as two statements

```c
D_800847D0[0] &= ~0x100;
D_800847D0[0] &= ~0x1000;
```

the masks stay separate and the stores do NOT double: gcc 2.7.2's flow pass
keeps a `mem_set_list` and deletes a store to a location that is stored again
with no read in between, so what comes out is the listing's one `lw`, two
`and`s and one `sw`. A local with two `&=` on it gives the same four
instructions but lets sched1 hoist the `lw` above an unrelated store that the
target emits first, so the global is the thing to write twice.
func_800553D4, one word and then the load order.

### cse learns a case value inside a switch arm

cse follows a conditional jump into its target when the label has a single
reference, so inside `case 0x71:` it knows the switch subject equals 0x71 --
and `cse_insn` attaches that as a `REG_EQUAL` note, which combine then
rematerialises. A call in the arm that passes the subject therefore costs
`addiu $a0, $zero, 0x71` even though the subject is already in $a0, and
passing the literal emits the same word. So when an arm's `jal` has NO
argument move in front of it and the dispatch register already holds the
value, the call site is prototype-less in the original; there is no spelling
that passes a value and matches. func_800553D4 uses an `__asm__` alias to say
that for one call site without touching the declaration its other call sites
need.

### A value reassigned in several blocks is several block-scoped locals

A handle written once per slot -- `h = alloc(); if (h) { use(h); store(h); }`
five times over -- is ONE allocno when `h` is declared at function scope: all
its references add up and its live length does not, so it outranks a base
pointer that is live across the whole body and takes the lower-numbered
callee-saved register. Declared inside each block instead, each copy is its own
allocno and each one loses to the base, which is the order the listing has.
func_800525D4, 45 DIFF at the exact word count and every word of it those two
registers.

So when two long-lived pointers hold each other's callee-saved registers, look
at whether one of them is really several variables.

### When a call's argument setup is EARLIER than the loop's giv updates, move `i++` after the call

An argument setup cannot be scheduled earlier than its place in the insn
chain, because `expand_call` emits it where the call is. Induction-variable
updates CAN be hoisted across it, and across the call itself, because they
write callee-saved registers the call does not clobber. So when a listing has
a dependency-free argument setup -- a bare `addu $a0, $zero, $zero` -- sitting
AHEAD of the loop's `addiu` giv updates, the increment is after the call in the
source, not between the calls:

```c
func_80056DB4(i);
func_8005E97C(0, v);   /* its $a0 = 0 lands right after the first call */
i++;                   /* sched pulls the giv updates forward to just here */
m |= v;
```

With `i++` between the two calls the four insns come out in the other order,
and no spelling of the argument fixes it: a `s32 w = 0;` assigned before the
increment is constant-propagated straight back to the call. func_80056C30, 4
DIFF at the exact word count to MATCH.

The general form: a dependency-free insn that the listing places EARLIER than
this source does is pinned by its chain position, so move the statements around
it rather than trying to move it.

### A post-increment whose value outlives its statement costs a copy

`expand_increment` QUEUES the increment when the incremented thing is a true
register reference (a pointer local), and `protect_from_queue` then decides
what the old value costs:

* increment not yet emitted (the use is in the SAME statement, before
  `emit_queue`) -- the use gets the variable itself and the add is emitted
  after it. `u8 b = *c++;` is 10 words and no copy.
* increment already emitted -- `protect_from_queue` creates `QUEUED_COPY` and
  emits `move(QUEUED_COPY, QUEUED_VAR)` *before* the increment insn. That is a
  register-to-register copy, a whole extra word, and `cse` cannot fold it away
  because the copy's SOURCE is modified between the copy and the use.

The reachable shape is the second one: `u8 *c = (*q)++; u8 b = *c;` in two
statements, where the first statement's `(*q)++` takes `expand_increment`'s
`op0_is_copy` path, re-expands `*q` for the assignment, and cse rewrites that
second load as a copy. It brings the parked trio func_80039730/func_80039C44/
func_80039D10 from 10-11 words to the target's 12 with the right instruction
set, 6 DIFF.

**What is still open on those three** is the mirror image of the copy. The
target's RTL is `X = Y; X = X + 1; MEM(q) = X; byte = MEM(Y)` -- the COPY is
incremented in place and the ORIGINAL is dereferenced, so X carries 4 refs
against Y's 3 and local-alloc gives X `$v0`. Every spelling measured gives
`Y = MEM(q); X = Y; Y = Y + 1; MEM(q) = Y; byte = MEM(X)` instead, the exact
mirror, and the ref counts then hand `$v0` to the other one. `d = c; d++;`
would have the right shape and cse folds it to `d = c + 1` (10 words,
func_80039730 variant uA). `search.py` finds nothing from the 6-DIFF base.
So the remaining question is narrow and worth asking precisely: what makes
cc1psx increment a COPY in place while keeping the original live?

### Two primitives in one function are TWO variables, and that moves the mask

`func_80053858` allocates one sprite primitive per character inside a loop and
then one TPAGE primitive after it.  Both are `*(u8 **) (s->unk_00 + 0x8D0)`
bumped by the primitive's size, so reusing a single `u8 *e` for both is the
obvious spelling -- and it costs the last five words.

The two uses are linked by the same `addPrim` tail, which masks the pointer
into the ot word:

```
and  $s0, $s0, $s3          # the listing: masked IN PLACE on the primitive
```

With `e` reused, `e` is still live as "the loop's primitive" when the second
block runs, so combine cannot let the mask clobber it: the pointer is copied
to `$v1` and masked there, adding the copy and spilling the schedule around
it.  With a second variable `u8 *f` for the TPAGE primitive, `f`'s live range
ENDS at that `addPrim` -- it is the pointer's last use -- so the `and` writes
back into the primitive's own register and `$s0` carries it.

The general shape: **a pointer that dies into its own mask gets masked in
place; a pointer reused later does not.**  When a function builds two objects
of the same kind and the listing masks (or otherwise consumes destructively)
only one of them in place, that is two variables, not one reused.  This is the
same ref-count mechanism as "A value reassigned in several blocks is several
block-scoped locals", seen from the combine side rather than the allocator's.

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

* **A constant displacement survives on the STORE only when the giv's base is
  a REGISTER.** `simplify_giv_expr` folds a symbol and a constant together
  because symbol-plus-constant is itself a constant, so `SYM[i + K]` makes the
  giv `SYM + K*4 + i*4` and the store comes out at displacement 0. Assign the
  base to a local first -- `r = SYM;` then `r[i + K]` -- and `add_val` is a
  pseudo that the constant cannot fold into: the giv is `r + i*4`, the `K*4`
  stays as the MEM's displacement, and the preheader's base is biased by the
  index's own range instead. func_80037E58's descending loop is
  `addiu $v0, D_80082A38, 0x3C` with `sw $zero, 0x48($v0)`, which is only
  reachable the second way; five byte-cast regroupings of the by-name form all
  gave displacement 0, because the regrouping is undone before loop.c sees it.

* **A `goto` loop is INVISIBLE to loop.c.** The C front end emits
  `NOTE_INSN_LOOP_BEG`/`_END` only for a loop STATEMENT, and
  `find_and_verify_loops` keys entirely on those notes -- so a back edge built
  from `goto` gets no loop-invariant motion, no strength reduction and no
  inversion. When a listing recomputes something inside a loop that loop.c
  would obviously hoist, that is the tell, and the fix is to write the loop
  with `goto` instead of `for`/`while`.

  func_80059BC4's inner command pump is the worked example. As `for (;;)` with
  `continue`, loop-invariant motion lifts `(high D_800737C8)` into the
  preheader; the listing has it at the loop top. The knock-on is what actually
  cost the word: hoisting puts the loop's LABEL after the `lui` instead of
  before it, so the back edge's opposite thread begins with the `lw` that
  READS that register, and `fill_slots_from_thread` then refuses to move the
  fall-through's own `lui %hi(D_800737C4)` into the delay slot -- it copies the
  loop's `lui` in instead and nothing is saved. With `goto` the label precedes
  the `lui`, the opposite thread starts with an insn that only WRITES the
  register, and the move goes through. 87 words to 86 and MATCH.

  **Where a loop's label sits relative to a hoisted insn decides what reorg
  can put in its back edge's delay slot**, which is the second thing to read
  off such a listing.

* **`do { ... goto ... } while (0)` splits loop.c in half: hoisting WITHOUT
  strength reduction.** The two things loop.c does to a loop are not always
  both wanted. A listing can hoist a loop-invariant out of a sweep and still
  recompute the element address every iteration -- and neither a real loop
  statement nor a bare `goto` loop gives that, because the first gives both
  and the second gives neither.

  The device is to write the back edge as a `goto` and wrap the whole region
  in a `do { } while (0)`. The wrapper IS a loop statement, so the front end
  emits the loop notes, `scan_loop` runs over the region and
  `move_movables` hoists its invariants into the entry block; the back edge
  inside is a `goto`, so there is no biv and `strength_reduce` has nothing to
  reduce. `while (0)` costs no instruction -- jump.c deletes the test.

  func_8005C130's first sweep is the worked example. As a `do`/`while` the
  element-address giv has benefit 4, one `add_cost` over the cut in
  `strength_reduce` (`v->lifetime * threshold * benefit < insn_count`, loop.c
  line 3823), so it reduces to a walking pointer and the function is two words
  short. As a bare `goto` loop there is nothing to hoist, the
  `lui %hi(D_80073734)` stays inside the sweep, the pressure on the
  callee-saved registers drops and a parameter that the listing spills to its
  stack home no longer does -- two words short again, differently. Wrapped,
  it is MATCH.

  Remember the threshold is a near-constant: `(loop_has_call ? 1 : 2) *
  (3 + n_non_fixed_regs)`, and `n_non_fixed_regs` counts non-fixed HARD
  registers, so it is the same for every function on this target. What decides
  a giv is therefore just whether `benefit` -- the address's rtx cost, minus
  `add_cost * biv_count`, minus `copy_cost` when it is not replaceable --
  survives above zero. One operation (benefit 2) never does; two (benefit 4)
  always does. That is why no spelling of a two-operation address reaches the
  unreduced form, and why the loop's own form is the only lever.

## 10. Measured dead ends -- do not retry

* **`volatile` as a nudge** -- reaching for it hoping the shape shifts. Tried
  that way in func_80042900, func_80042A80, func_8003DBD0 and func_8003D0F0: it
  changes the output but never toward the target, and it usually adds a word.
  **But it is NOT a dead end where it has an exact mechanism**: against
  `combine`'s byte-to-signed refold it is the answer, and it matched
  func_80044D24. See section 7. The distinction worth carrying is whether you
  can name the pass and the predicate it defeats.
* **`combine` proving a bit -- SOLVED 2026-10-23, and this entry is kept only
  because its conclusion was wrong for two sessions.** The downgrade is real but
  it does not happen where this said. See section 7 for the lever; the short
  version is that combine rewrites `sra` to `srl` when it SUBSTITUTES the
  extension insn into its single consumer, and its LOG_LINKS are per basic
  block, so putting the consumer in a later block denies the substitution for
  free and keeps the value single-set. func_80042A80 landed that way. Everything
  below about `nonzero_bits` and multi-set shapes is accurate but was the wrong
  tree to be up.

  `(s16) (x & 0x400)` comes out `srl`, never
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
* **`MEM_IN_STRUCT_P` does NOT change whether cse forwards a stored value to
  a following load -- refuted 2026-10-26, and worth recording because section
  5 points straight at it.** Section 5 is about sched1's alias calls and cse's
  INVALIDATION, and it is easy to assume the same flag decides forwarding.
  Four probes on `f = f + 1; return f & 1;` over a u16 at +0x64 -- a cast
  pointer `*(u16 *) (p + 0x64)`, a real struct field `p->f`, `p->f++`, and the
  store and load through different base expressions -- all four emit
  `lhu`/`addu`/`sh`/`andi` with NO reload. So a target that reloads the field
  after storing it (func_80036F24 and func_80037090 both do, and it is the two
  words they are short) is not doing it because of the struct flag, and no
  spelling of the access reaches it. The mechanism is still unidentified: cse's
  run ends only at a CODE_LABEL with LABEL_NUSES != 1 and those targets have no
  label between the store and the load. This is the first case in the sweep
  where cse FORWARDING is the problem rather than caching, so section 4's
  "re-dereference rather than cache" has no counterpart to offer yet.

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
* **A tool that invokes `tools/ccpsx` needs `-Ipsyq/include` as well as
  `-Iinclude`.** `include/common.h` pulls in `sys/types.h`, `libcd.h`,
  `libgte.h` and `libgpu.h`, which exist only under `psyq/include`. The
  Makefile passes both paths; the decomp tools passed only the first, so
  adopting the Psy-Q types broke `verify.py` -- and therefore `search.py`,
  `sweep.py` and `perm.py`, which all shell out to it -- for every file at
  once, while `make` stayed green. The error names `gnu/stubs-32.h`, a host
  header, and not the real cause. Fixed 2026-10-26.
  **`python3 tools/decomp/nmcheck.py` is the one-command check that the fast
  loop works**; run it before concluding a candidate is wrong.
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

**Step zero, before any compile: give the agent its own wine prefix.**
Concurrent `ccpsx` runs share one prefix and silently lose an output -- the
compile exits 0 having written nothing, or having written another run's file --
and with several agents going at once that is not a rare race, it is the normal
case. Put this in the brief verbatim:

    cp -a ~/.cache/azure-psyq /tmp/azure-wine-$$ && export AZURE_WINEPREFIX=/tmp/azure-wine-$$

`tools/ccpsx` honours `AZURE_WINEPREFIX`, and it has to be exported in every
shell command the agent runs, not just the first. `AZURE_SCRATCH` is worth
setting the same way if the agent will use `dump.sh`, since that writes to a
shared `/tmp/azure-decomp` by default. An agent that measures a number under a
shared prefix cannot tell a lost output from a real regression, and will report
the regression.

**Tell it what has changed since this document was last read.** Three briefs in
a row went out without the levers that had landed that hour, and the agents
re-derived them or worked around them. A brief is cheap; a re-derivation is a
session. Name the specific sections that are new and say which functions they
closed, so the agent knows they are load-bearing rather than background.

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

**Rename with word boundaries when you test a variable SPLIT.** Giving one
reused running value two locals is a routine experiment, and the obvious
`text.replace(" t;", " w;")` leaves `w = t + x;` behind -- a candidate that
mixes the two and scores wildly worse than either shape. func_80037714 was
parked for a session on exactly that: the two-variable shape was recorded as
343 DIFF and discarded, and when re-run with `re.sub(r'\bt\b', 'w', block)`
it was a MATCH. Any measurement that comes back far worse than both
neighbours is worth re-reading as a broken edit before it is believed.

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

## Which of two givs is initialised first, and the bias on a walked pointer

When a loop walks two arrays, the order of the two `lui`/`addiu` pairs before
the loop and of the two increments at its end is decided entirely inside
loop.c, and the source cannot reach it by moving the references around.

`record_giv` PREPENDS to the biv's giv list; the reduction loop (loop.c:3856)
walks that list and emits each giv's initialisation before `loop_start` and
each increment before the biv's own increment, in list order. **Emission is
the reverse of creation, and creation is insn order.** The array whose first
reference comes LAST in the body is initialised FIRST.

That is not the whole story, because `combine_givs` (loop.c:5494) collapses
every giv with the same `mult_val` and an `add_val` differing by a constant
into one group, and the representative it keeps is the first eligible `g1` in
the giv list -- that is, the group's LAST-created member. Two consequences:

* Putting a reference to the second array ahead of the first does NOT move its
  initialisation. The early giv is combined into the late one and loses its
  place in the list. Nothing in the source body can change the order when one
  array's references are all inside a nested loop, because the nested loop's
  own invariant hoist (`move_movables`) creates copies LAST -- loops are
  processed innermost first.
* The representative's `add_val` becomes the base register's value, so a
  source-level WALKED pointer (`u8 *r = D_80085458;` with `r += 0x78`, which
  makes `r` a biv and every field access a giv on it) comes out biased to the
  offset of its LAST access, with negative displacements for the earlier
  fields. That is the "biased giv" seen on func_80057948 and func_8005914C,
  and it is why the matching form is almost always the block-scoped
  `u8 *q = BASE + i * STRIDE;`: a pointer that is itself a giv records no
  further givs for its field accesses, because `update_giv_derive` clears
  `cant_derive` at the first conditional jump in the body.

Read all of this out of `tools/decomp/dump.sh`'s `-dl` dump rather than
guessing: the loop pass prints every giv it records (with `mult`, `add`,
`replaceable` and lifetime), every `giv at N combined with giv at M`, and
every `giv at N reduced to (reg:SI R)`. Measured on func_80057D20's case 64,
where this is the last six words.

## fold's `associate:` moves a constant onto the wrong operand

`x - (y + K)` and `x - (y - K)` do not survive the front end. `fold`'s
`associate:` case (fold-const.c) calls `split_tree` on each operand; for an
operand that is a PLUS or MINUS with a constant it returns (var, con) and
rebuilds the expression as `(x -+ K) - y`. The four instructions are the same
but the constant lands on the OTHER operand: where the listing has
`lw; lui; addu; subu` -- the constant added to the loaded word -- gcc gives
`lui; lw; addu; subu`, the constant added to the other side.

Two things block it, and func_800517CC's case 5 needed both:

1. **Write the constant SIGNED.** The listing's literal is `+ 0xFF280000`,
   which in C is an `unsigned int`, so the whole expression goes unsigned and
   a following `/ 2` becomes one `srl` instead of the signed
   `srl 31 / addu / sra 1`. Spell it `- 0xD80000`. Eight words on that
   function.

2. **Put the second operand in a LOCAL.** `split_tree` strips NOP_EXPRs of
   the same machine mode, so no cast helps -- but a VAR_DECL gives it nothing
   to split. This also fixes the ORDER: the listing loads the subtrahend AFTER
   the call that computes the minuend, which no single statement can express,
   so the minuend needs a local too and the two assignments run in the
   listing's order. sched cannot move a load across a CALL_INSN, so the order
   has to come from the source.

And then **declare those locals in a brace of their own**, one per block. Two
function-scope locals reused across four such blocks are two allocnos whose
references SUM over all of them (the one-variable-per-loop rule above),
which inflates their priority and takes a register the listing reuses: 32
register-only words on func_800517CC, 28 with one brace per `case`, MATCH with
one brace per block. Keep the struct updates as `+=` too -- an explicit
`a = field + delta` local makes the second update read `a` instead of the
word just stored, and the permutation comes back.

### Both halves of this are in search.py now

`tools/decomp/search.py` has `t_decl_order` (transpose two adjacent
declarations), `t_decl_per_arm` (give each switch arm its own copy of a
shared local, in a brace of its own) and `t_volatile` (make one pointer
dereference `volatile`, reads only -- the section 5 lever, which was the one
documented lever the sweep could not reach). They are the two zero-cost knobs on
local-alloc's choice between equally-weighted locals, and the second is the
rule above. Run it over a park before hand-permuting anything: the sweep is
free and it records what each accepted change was worth.

## An explicit `goto` is how you get a loop that is NOT rotated

loop.c's `duplicate_loop_exit_test` copies a top-tested loop's exit test to
the bottom. Whether you want that is a question for the listing, and both
answers have now been measured:

* func_8005914C WANTS it. Its listing has the guard-plus-bottom-test shape
  with ONE `lui` serving both tests, which is exactly what the duplication
  produces from a `while` -- and a hand-written guarded do-while is two
  expressions, two `lui`s, two words worse.

* func_8005B4D0's two scan loops do NOT. Their listings have a single copy of
  the test at the top and the back edge jumping to it. `while (cond) { ...
  if (bound) break; ... }` and `for (;;) { if (!cond) break; ... }` both
  rotate and come out four words longer, with a second copy of the test.
  The shape that matches is an explicit label and `goto`:

      scan:
          if (cond)
          {
              i++;
              if (bound) { i = -1; }
              else { p += STRIDE; goto scan; }
          }

  A loop built this way gets no loop notes at all, so nothing rotates AND
  nothing is strength-reduced -- which is the other tell: that listing walks
  its pointer (`addiu $a0, $a0, 0x78`) where every indexed loop in the same
  unit gets a giv. Ten words over the two loops.

So: count the copies of the exit test in the listing before choosing the loop
form. Two copies means a `while`; one copy with a back edge to it means a
`goto`.

## Put each accumulation next to the channel it consumes

func_8004E6F4 matched on this and nothing else, after sitting at the right
word count with thirteen differing words.

Three channels are interpolated and folded into one accumulator. Written as
the listing's own grouping suggests -- all three sums, then all three
accumulations -- the merged tail computes both sums before processing either,
and no register assignment makes that agree with a listing that folds the
first sum into the accumulator before computing the second. Written
interleaved, and with all three accumulations INSIDE both arms of the
branch:

    r  = c[0] * w[i - 1] + b[0] * (4 - w[i - 1]);
    acc += r >> 5;
    g  = c[1] * w[i - 1] + b[1] * (4 - w[i - 1]);
    acc += g & 0x3E0;
    bl = c[2] * w[i - 1] + b[2] * (4 - w[i - 1]);
    acc += (bl & 0x3E0) << 5;

it matches exactly. jump2 cross-joins the identical suffixes of the two arms
at no cost in words, so the duplication is free; what it buys is that each
`addu` sum lands immediately before its own shift or mask. With the three
accumulations hoisted out of the arms it is thirteen words wrong, with only
the first inside thirteen, and with all three inside but the sums batched two
words over.

The companion shape on the same function: the weight and its complement
INLINE at all six products rather than through locals, because a local makes
the weight load first where the listing loads the first-named operand of the
first product first. cse shares both subexpressions either way, so the inline
form costs nothing and only fixes the order. Neither shape alone matches.

## sched2's tie-break inside a priority group is MEMORY OVER ALU

Section 8 says the keys are priority, then the dependence class, then
INSN_LUID. The middle key has a name and a shape, and
`tools/decomp/sched.py` prints it: `;; insn N has a greater potential
hazard`.

`schedule_select` (sched.c:2615) walks the ready list in runs of EQUAL
priority, queues anything blocked this cycle, and among what is left in the
FIRST such run picks the insn with the largest `potential_hazard`
(sched.c:1318). That function is zero unless the insn's function unit has a
`max_blockage` above 1 -- it reads the machine description, not the insn --
so on MIPS it splits the ready list in two: insns that touch memory, and
everything else.

Measured on func_80054B08, every promotion the dump reports, six of six:

    insn 107 (sw D_800847D0)   over 109 (high)
    insn 227 (sw ra, 20(sp))   over 196 (li 4096), 194 (and)
    insn 229 (sw s0, 16(sp))   over 196, 194, 227
    insn 23  (lhu 0(v0))       over 20 (lo_sum), 39, 220
    insn 39  (lw 12(v0))       over 20, 220
    insn 44  (sb 0x31(a3))     over 73 (lshiftrt)
    insn 68  (sw 0x18(a3))     over 73, 44

A load or store always wins; `and`, `li`, `high`, `lo_sum` and `lshiftrt`
always lose. Ties among memory insns fall through to ready-list order,
because `best_cost` starts at -1 and a later insn must be STRICTLY greater to
displace the first.

**What that means for a reordering you are trying to fix.** If the listing
has an ALU insn where your build has a load or a store, and `sched.py` shows
them sharing a priority, no statement reordering will swap them -- the
promotion is unconditional. The only thing that moves them is the PRIORITY,
and priority here is the longest LATENCY path from the insn to the end of its
basic block, not a count of instructions: `insn_cost` is zero for an
ALU-to-ALU edge, so only loads, multiplies and divides lengthen a chain. That
is why the whole top-of-function region sits at priority 1 and why a call can
sit there too. To move a memory insn later, give the insns after it a longer
latency chain; to move it earlier, shorten what follows it.

The other mechanism in the same dump is separate and easy to confuse with
it: `;; blocking insn N for M cycles` is `actual_hazard`, the memory unit
being busy in that cycle, which DEFERS an insn rather than promoting one.

## A declaration NARROWING can be a semantic break that scores better

search.py's `t_decl_width` family is one of the most productive it has, and
it is also the only one that can change the VALUE a local holds rather than
just its representation. The score cannot tell the difference, and neither
can a word count.

Measured the hard way on 2026-10-27. A sweep over all forty-four parks
proposed nine accepted changes; four were narrowings, and ALL FOUR were
wrong:

* func_80054B08, `u32 w` to `u8 w`, 32 DIFF to 29. `q->unk_08` is a `u32`
  field whose top byte the listing splits off with `srl $v0, $v1, 24`. At 8
  bits that shift folds to zero and the build emits `sb $zero, 0x31` where
  the listing has real code.
* func_8004DE88, `u32 a` to `s16 a`, 59 DIFF to 47. `a` holds the address of
  D_80083D88 and is written out as four bytes with `>> 8`, `>> 16`, `>> 24`.
  The top two shifts die.
* func_8004AC3C, `s32 r` to `u16 r`, 94 DIFF to 90. `r` takes
  `(s32) ((u8 *) 0x80010324 + ...)` and several `*(s32 *)` reads -- it holds
  ADDRESSES.
* func_8004A330, `s32 r` to `s16 r`, 74 DIFF to 43. `r` takes
  func_8004B404's return value, and that is `u32 func_8004B404(s32)`, used
  elsewhere as `(u8 *) func_8004B404(0x104)` -- an allocator returning a
  pointer.

Every one improved BOTH metrics. The pattern is the same each time:
narrowing deletes high bits the function actually uses, the deleted code
cannot differ from the listing because it is no longer there, and the score
reads the absence as agreement.

**How to check one in a few seconds.** Take the local the sweep narrowed and
ask what the LISTING does with that value: a shift by more than the new
width, a mask wider than it, a store through a wider pointer, or an
assignment from a function whose declared return type is wider. Any of those
and the narrowing is wrong however well it scores. A narrowing is safe when
the value genuinely is that narrow -- a `u8` field read into a `u8` local, or
an index compared against a small bound (func_8004951C's `k` reads a `u8`
struct field, and that one stands).

search.py now prints a REVIEW REQUIRED block naming every accepted
narrowing, so the check cannot be skipped by accident. It was skipped once,
because a batch script applied nine sweep results without anybody reading
the diffs.

**All 44 parks were then audited for the same hazard** -- every `s8`/`u8`/
`s16`/`u16` local whose park uses it at a wider width (a shift at or beyond
its width, an assignment from an `*(s32 *)`/`*(u32 *)` read or an `(s32)`
cast, or a store through a wider pointer). Four came back and none is a new
silent break:

* func_8005B4D0 and func_8005BBFC, `u16 t` stored through `*(u32 *)`: both
  FAITHFUL. The listing reads the field with `lhu` and stores it with `sw`,
  so the value really is sixteen bits.
* func_8005A90C, `s16 r`: already flagged in its park as unproven.
* func_8004B954, `s16 cx` stored through `*(s32 *)`: already known wrong, and
  its park now says outright that the 283-word draft is parked for its
  measurements rather than as a candidate.

So the scan is worth re-running after any width change, and two of its four
hits are the kind of false positive to expect: a genuinely narrow value
stored into a wider field is fine.
