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
| `cse` | substitutes the cheapest equivalent per operand, then constant-folds | `rtx_cost` of a `CONST_INT` is ~0, so a known constant always beats an insn -- that is why `0x10 * 2` becomes `li 32` and cannot be talked out of it |
| | memory equivalences | invalidation is **coarse**: a store through a register base kills entries at other offsets off the same base, which is why the target reloads a neighbouring field |
| `loop` | invariant motion | hoists a constant's set into the preheader, where `combine` can no longer fold it into an immediate (section 3) |
| | strength reduction | builds the giv list by **prepending**, so the *last* access in the body becomes `combine_givs`' representative (section 9) |
| `flow` | `reg_n_refs`, `reg_live_length` | refs are weighted by `loop_depth`, so a loop note reweights the whole function (section 2). Both are recomputed after `sched1`. |
| `combine` | substitutes an insn into its single user | blocked by a **second use**, by an intervening memory store (`use_crosses_set_p` / `mem_last_set`), and by basic-block boundaries. This is the most reachable pass from C. |
| | proves bits via `nonzero_bits` | **not** reachable from C -- see the dead ends |
| `sched1` | insn order **within one basic block** | ties break by `LUID`, i.e. source order; alias calls go through `MEM_IN_STRUCT_P` (section 5); a hazard heuristic reorders stores and cannot be steered |
| `local-alloc` | registers for block-local quantities, **before** `global-alloc` | a value live in one block only is out of reach; it also takes the register `global-alloc` would have wanted |
| `global-alloc` | everything else, by `floor_log2(refs)*refs/live_length` | section 3; `find_reg` also avoids registers that *conflicting* allocnos prefer, and a call's argument preferences propagate |
| `jump2` | cross-jumping | merges identical tails, so duplicating a store into two arms costs nothing (section 1) |
| `sched2`, `reorg` | delay slots | `reorg` can steal the insn at a branch target and redirect the branch past it, which looks like a duplicated instruction in the listing but comes from one statement |

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

  And it can be used to **prevent** a merge. Duplicate a run of insns into both
  arms whose *last* insn differs between them, and cross-jumping merges only
  the common part, leaving the differing insn in the arm where sched1 put it.
  func_80045CC4 needs its two halfword stores inside both arms so that each
  arm's own `subu`/`addu` stays last and lands in the branch's delay slot;
  without it, jump2 pulls all four stores into the join. Worth 4 words.

## 2. Loop notes change every reference weight in the function

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

Two things that cannot be steered from C:

* `find_reg` avoids registers that *conflicting allocnos prefer*, so changing
  which allocno holds a preference cascades through the whole assignment.
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
* A big object reached through a pointer local keeps its address in a
  callee-saved register across calls; reached by symbol it rebuilds a
  `lui`/`%lo` pair per access (func_8003D5A4, 105 words; func_80046884, 116).
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

## 7. Narrow types

* An `s16` parameter lets a halfword store reuse the raw incoming register
  while the sign-extended copy is a separate pseudo; declared `s32` the copy is
  forced to the top of the function and a delay slot goes empty
  (func_8004425C). It also splits the parameter copy, which is schedulable and
  drifts (func_80046884, 8 words).
* A `u16` field read with `(s16)` casts yields a bare `lh`, because combine
  folds zero_extend-then-sign-extend-of-low (func_8004425C).
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
* **A value with more than one SET is opaque to combine, and that is a lever in
  its own right, not just a trap.** `nonzero_bits` gives up, so `i > 0` stays
  `blez` instead of narrowing to `bnez` (func_80041344, func_800407C0, and
  func_80041588's case 9, whose `for (i = *(u16 *) (p + 8); i > 0; i--)` gets
  `blez` while a sibling loop with a single set gets `beqz`).

  More usefully, combine also stops reducing a `zero_extend` to a `subreg` and
  stops substituting the following `ior`. So `flags = *(u16 *) (p + 0xC);` then
  `flags |= 2;` -- the same value, loaded and updated IN PLACE -- keeps the
  whole load/OR chain as one **global allocno** where the one-expression form
  is a block-local temp. In func_80041588 that is worth 81 differing words: it
  is what makes sched1 put the `lhu` first in two different blocks and what
  stops jump2 cross-jumping one insn too deep. Spelling it as a single
  statement, `flags = *(u16 *) (p + 0xC) | 2;`, already costs 3 words, so the
  split is doing the work, not the variable.

  **This is the lever to reach for when sched1 is wrong inside a block**, where
  section 10 says statement order will not help: changing the number of sets
  changes the dependency graph, which is the thing sched1 actually reads. Two
  sets of one pointer does the same job for register class -- func_80041588's
  `base` is masked, then copied back from the KSEG0 pointer, which is what puts
  the masked value in a callee-saved register instead of `$v0`.
* `GetRCnt` has to return a signed type for `(x + 0xFF) >> 8` to be `sra`.
* **A byte decrement is `addiu 0xFF` only if the arithmetic happens in the
  byte's own type.** For a `u8` lvalue, `x--` builds the constant in that type
  and emits `addiu rd,rs,0xFF`; going through an int-promoted temporary gives
  `addiu rd,rs,-1`, because combine narrows `t + 0xFF` to QImode for the byte
  store and `trunc_int_for_mode` rewrites the constant. `p->f = p->g--;` is the
  spelling that matches (func_8003D0F0, 2 words); a `u8` temp, or a separate
  `s32 u = t + 0xFF;`, works too (func_80045CC4, 2 words).

## 8. Statement order and expression shape

* An exit block that should sit physically last wants `break` or `goto done`
  out of the loop with the `return` after it; an inline `return` in the arm
  costs a `j` plus a separate epilogue (func_8004068C, 49 words;
  func_8004491C, 1).
* Chained assignment `a = b = v;` stores `b` first and goes through
  `store_expr` with `want_value`: load to temp, store, copy -- and that copy
  often fills a delay slot. A four-deep chain evaluates innermost-first, which
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

## 10. Measured dead ends -- do not retry

* **`volatile` as a nudge** -- reaching for it hoping the shape shifts. Tried
  that way in func_80042900, func_80042A80, func_8003DBD0 and func_8003D0F0: it
  changes the output but never toward the target, and it usually adds a word.
  **But it is NOT a dead end where it has an exact mechanism**: against
  `combine`'s byte-to-signed refold it is the answer, and it matched
  func_80044D24. See section 7. The distinction worth carrying is whether you
  can name the pass and the predicate it defeats.
* **`combine` proving a bit, when the value is single-set.** `(s16) (x & 0x400)`
  comes out `srl`, never `sra`, because the mask proves bit 15 clear through
  `nonzero_bits`.

  **One thing does defeat it, and it costs a word.** `nonzero_bits` gives up on
  a value with more than one set unless every set is a known constant with the
  bit clear, so a second set that is *opaque* -- or a constant with bit 15 set
  -- preserves the `sra` (measured, 2026-10-22):

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
* **`cse` folding a known constant.** `p->field = 0x10;` then `field * 2` in a
  following block is always `li 32`, never `sll`. Measured against: the field
  read back, an `s32` local, a `u8` local, the local given a second use, the
  value derived as `other >> 4`, the local also feeding another field, a `(u8)`
  cast on the product, `<<= 1`, the `if` inverted, and the two entry stores
  swapped. `rtx_cost` is why: a `CONST_INT` costs ~0 and MIPS cannot store an
  immediate, so expand always materialises the constant into a register before
  the store and hands `cse` the equivalence. This blocks func_80041E70.
* **`sched1`'s hazard heuristic.** It is a pure function of the insn and the
  unit state; no statement order changes it. Move the basic-block boundary
  instead (section 1).
* **`local-alloc`'s choices.** A block-local quantity -- a switch index, a
  value used only inside one arm -- is assigned before `global-alloc` runs, and
  nothing in the source reaches it. This is what is left in func_80041344.
* **A call's argument preferences.** An allocno that feeds a call argument
  carries a preference for that register, `find_reg` avoids it for conflicting
  allocnos, and allocnos crossing the call have their own preferences pruned by
  `call_used_reg_set`. The whole assignment shifts around the call and no
  spelling moves it. This is what makes func_800407C0 unreachable.
* **decomp-permuter** cannot score a function that uses a constant address:
  splat's `target.o` carries an `R_MIPS_LO16` relocation the constant form
  cannot produce, so the score never reaches zero.

## 11. Toolchain traps

* **`make check` is the only authority.** `verify.py` compares a relocated word
  on its upper 16 bits only; it now marks a differing `%lo` addend with `?`, but
  it still cannot see a jump table landing at the wrong address.
* **wine's filesystem is case-insensitive.** Two scratch files whose names
  differ only in case are one file to `ccpsx`, which then exits 0 having written
  the other one. `verify.py` diagnoses this now; name variants so they differ by
  more than case.
* **Concurrent `ccpsx` runs share one wine prefix** and can lose an output.
  Retry before believing a compile failure.
* **`cc1psx` output has CRLF line endings.** Strip the CR before matching
  labels.
* **`make splat` does `rm -rf asm/nonmatchings` first**, so never run it while
  anything else is reading those listings.
* **Do not pipe `make check` through `head` or `tail`** -- it masks make's exit
  code, and a broken state was committed that way once.

## 12. Keeping an agent's run cheap

A subagent working one function pays for its whole context on every turn, so
tool output compounds. Two things dominate:

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
