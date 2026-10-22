# Source shapes that move this compiler

Written 2026-10-22, from the twenty functions matched that day. Every entry
here was *measured*: the cost in words is what reverting that shape did to a
function that otherwise matched. Read `docs/where-we-are.md` first for the
workflow and `docs/compiler-version.md` for which compiler this is.

The list is roughly in order of how often it has been the answer.

## 1. Block membership beats scheduling

sched1 only moves insns **inside** a basic block. So when an insn is in the
wrong place and no amount of statement reordering fixes it, the question is
usually which basic block it is in, and the lever is the source's control flow,
not its order.

Two instances:

* `x = abs(x);` instead of `if (x < 0) { x = -x; }`. The MIPS `abssi2` pattern
  is a **single insn** whose output template contains its own `bgez`/`subu`, so
  it creates no block boundary and the branch delay slot inside it stays a
  `nop`. With the abs as one insn, a whole arm becomes one block and sched1 can
  then hoist a reload up to the store it follows. `int abs(int)` is declared in
  `structs.h` for exactly this; func_80046C20 and func_8003F6F4 use it.
* Duplicating a store into **both** arms of an `if`, so it is in each arm's own
  block rather than in the join block. `jump2`'s cross-jumping merges the two
  copies back and plants the join label in front of the result, so the word
  count is unchanged -- but sched1 never saw the store and the constant in one
  ready list. That is how func_8004491C's last two words came right; the hazard
  heuristic that was reordering them is a pure function of the insn and cannot
  be steered from C.

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

Two things that cannot be steered from C:

* `find_reg` avoids registers that *conflicting allocnos prefer*, so changing
  which allocno holds a preference cascades through the whole assignment.
* **local-alloc runs before global-alloc** and assigns block-local quantities
  first. A switch index, or any value live in one block only, is out of reach.
  That is what is left in func_80041344.

## 4. Re-dereference rather than cache

If a store invalidates cse's memory entry for another object, the target will
show a genuine reload. Caching the value in a local removes the reload and
costs a lot: 11 words in func_80043FD0, 22 in func_80043A68, 55 in
func_80046E38, 86 in func_80041CBC. The same goes for a value read either side
of a call.

Conversely, a value the target keeps in a register across a call wants a local;
one it recomputes wants the expression written out twice (func_8003F8F8
computes its size twice, once for the fit test and once for the accumulation).

## 5. MEM_IN_STRUCT_P decides sched1's alias calls

A struct or array reference sets `MEM_IN_STRUCT_P`; a bare pointer dereference
does not. `true_dependence` returns "no dependence" when two MEMs' in-struct
flags **differ** and the non-struct side's address does not vary -- which frees
sched1 to interleave things the target keeps sequential, and vice versa.

* `*(u16 *) ((u8 *) p - 2)` keeps two read-modify-writes strictly sequential;
  every struct or array spelling of the same address lets sched1 interleave
  them and fill both load-delay slots. func_80043FD0, 2 words.
* Making a pointee a struct lets three loads interleave against two
  non-struct scalar stores. func_80040CBC, 5 words.
* A whole region reached through `*(T *)(base + off)` instead of struct fields
  lets sched1 hoist every unrelated load above the region's stores and shuffle
  the stores among themselves -- 123 of 162 words wrong in func_8003DBD0 with
  the word count unchanged.

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
  folds zero_extend-then-sign-extend-of-low (func_8004425C). The byte-load
  counterpart of that fold is what blocks func_8003D0F0: `(s8)` on a `u8` field
  always collapses to `lb`, and no spelling found so far keeps the target's
  `lbu`/`sll 24`/`sra 24`.
* A byte copy through an `s8` temporary gives `lb`; copying the field directly
  gives `lbu` (func_80042710, func_80042560).
* An all-`u8` struct has alignment 1, which makes a whole-struct assignment the
  byte-aligned `lwl`/`lwr` + `swl`/`swr` block move instead of `lw`/`sw`
  (Unk3E39C, Unk44724Blk). Keep one `u32` member to force alignment 4
  (UnkTemplate).
* A local array zeroed by an aggregate initialiser calls `bzero`, which also
  forces a parameter out of its argument register; explicit word stores do not
  (func_80046884, 126 words).
* A value with two sets defeats combine's `nonzero_bits`, so `i > 0` stays
  `blez` instead of narrowing to `bnez` (func_80041344, func_800407C0).
* `GetRCnt` has to return a signed type for `(x + 0xFF) >> 8` to be `sra`.

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
