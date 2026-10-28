# Sweeping the carved units

The four units carved on 2026-10-23 -- `unk_72A8` (294 functions), `unk_1C540`
(465), `unk_1AF38` (53) and `unk_301D4` (5) -- are being worked cheapest-first
by size. `tools/decomp/guard.py` has the live stub count.

## How to run the sweep

`tools/decomp/sweep.py` does it. Write a batch file defining `CANDS` as
`(name, unit, body)` triples, then:

    python3 tools/decomp/sweep.py verify batch.py    # prints each result,
                                                     # writes batch_ok.py
    python3 tools/decomp/sweep.py land  batch_ok.py  # edits csrc/<unit>.c
    make check                                       # the only authority

`verify` writes ONE candidate per scratch file, because `verify.py` treats the
whole `.text` of the file as the function -- a file holding several candidates
reports nonsense (a two-word function came back "8 DIFF"). `land` refuses a unit
unless every stub is present exactly once and no inline prototype collides with
a definition being landed.

`tools/decomp/advise.py cand.c UNIT FUNC` is the first thing to run on a
candidate that is not a MATCH. It aligns rather than positionally compares, so
it reports an inserted word as an insertion instead of turning every later word
into a difference, and it classifies the rest: signedness, inverted if/else
arms, base/index reassociation, extra load-delay nops, a missing callee-saved
register, a wrong constant, or "only registers differ" -- which is the one case
where rewriting the C is known not to help. Validated against six functions in
this table whose answers were already known.

`tools/decomp/magic.py 0x80010003 [post_shift]` tells you which DIVISOR a
constant-division magic came from, by running gcc 2.7.2's own
choose_multiplier over every divisor. Do not estimate it from the arithmetic:
the magic and the shift are picked together, and 2^(32+14)/0x180010003 comes
out at 10922.56, which sends you hunting for a non-integer divisor when the
answer is 32767. For an unsigned add-back the listing's second shift is one
less than the post_shift it prints (`srl 1` / `addu` / `srl 4` is post_shift 5,
division by 21).

`tools/decomp/perm.py cand.c UNIT FUNC` permutes the statements between a
`/* PERM */` and `/* ENDPERM */` pair in a scratch candidate and scores every
ordering with verify.py. Statement order is the lever this compiler is most
sensitive to and the one with nothing to derive, so it is worth spending the
compute instead of the tokens: five statements is 120 orderings in about 90
seconds, and reading 120 word counts by hand is most of a context window. It
found func_80047738 and func_800489F4 outright, and it is what measures a park
row honestly ("none of the 120 permutations produces it"). `--max N` caps the
run and samples above the cap.

`tools/decomp/landlate.py batch_ok.py` lands the candidates whose only
collision is a prototype BELOW their own stub -- a legal
prototype-after-definition, which the sweep creates for itself every time a
landed function declares a callee that is still a stub -- and writes
`batch_ok_rest.py` for `sweep.py land`. It asserts the position rather than
assuming it.

**Let `make check`'s exit code gate the commit.** Writing
`make check > log; echo rc=$?; git commit` puts an always-succeeding `echo`
between them, so an `&&` guards nothing -- that committed a broken tree once.
**A pipe does the same thing and is easier to miss:** `make check 2>&1 | tail -2
&& git commit` takes `tail`'s exit code, which is always 0, so the commit always
runs. That committed a second broken tree (func_8003C5A4 landed with a parameter
its caller did not pass). `make check` prints four lines; run it unpiped.

**Never widen or re-shape a declaration that landed code already uses.**
Changing `extern u32 D_800814A0;` to `extern u32 D_800814A0[];` -- to get the
split-addressing form for a function in this sweep -- broke `csrc/unk_12DEC.c`,
which passes `&D_800814A0` to a function expecting `u32 *`. As an array that
expression becomes `u32 (*)[]` and the build fails in a unit the sweep never
touched. The incomplete-array trick is for NEWLY declared globals; for one that
already has users, find a spelling that works with the existing type.

**Grep BOTH `include/structs.h` AND the unit's own `.c` file before declaring a
callee.** As the sweep proceeds, a callee may already have been DEFINED by it --
declaring `void func_80034FD0(s16 *)` inline when the same file already had
`s32 func_80034FD0(u32 *p)` 46 lines above broke the build, and the commit went
in anyway because `make check`'s exit code was masked by an `echo` before the
`&&`. Let the check result gate the commit directly.

**Grep `include/structs.h` for an existing prototype before guessing a
signature.** Several of these functions were already declared by callers that
landed earlier, and a mismatch surfaces as a compile error, not a diff --
func_80053E14 (`void(s16)`), func_80053DA8 (`void(s32)`), func_80038464
(`void(u8 *)`).

**A whole batch failing at once is one broken header, not bad candidates.** Ten
simultaneous ERRORs turned out to be a duplicate `extern` for `D_80083178`,
which was already declared as a struct 300 lines further down.

## Patterns that have paid

* **A global store needs an incomplete-array declaration.** `extern u32 D_X;`
  with `D_X = 0;` gives four words through `$at` and cannot fill the delay slot.
  `extern u32 D_X[];` with `D_X[0] = 0;` gives the compiler-split
  `lui %hi / store %lo` the targets have. Worth 4 DIFF -> MATCH, every time.
* **Callback installs put the store LAST in the source.** The listing shows the
  `lui`/`addiu` at the top and the `sw` in the delay slot, which reads like
  "address first", but the address materialisation floats to the front on its
  own -- `rank_for_schedule` prefers a dependency-free insn before source order
  is consulted. Byte stores first, callback store last.
* **A rebased pointer must not fold.** `u32 *q = (u32 *) (p + 0x20);` keeps the
  `addiu`; indexing the outer pointer folds the constant away. And chained
  assignment stores the INNER one first, so `q[6] = q[7] = v;` emits +0x1C
  before +0x18.
* **Thin wrappers are unmistakable.** gcc 2.7.2 has no tail-call optimisation,
  so a body that is one call keeps a full frame: `addiu $sp,-0x18` / `sw $ra` /
  `jal` / arg in the delay slot / `lw $ra` / `addiu $sp` / `jr $ra`. Thirty-two
  of these matched first try.
* **A field offset inside an index expression migrates onto the symbol.**
  `*(u32 *) &SYM[i * 0x9C + 0xC]` on `extern u8 SYM[]` gives
  `addiu %lo(SYM)+0xC` with `lw 0x0`, where the target has `%lo(SYM)` and
  `lw 0xC`: `fold` reassociates the constant next to the ADDR_EXPR. Declare the
  table as an incomplete array of STRUCTS and write `SYM[i].unk_0C` -- a
  COMPONENT_REF's offset is applied to the MEM at expand time and cannot
  migrate, and the base materialisation comes out first as well. Six functions
  on D_80084960, one on D_80082660, one on D_8007359C, five of them parked at
  3-7 DIFF as register-order puzzles.
* **Constant indices on an array name bias the base; a local pointer does
  not.** `s16 *b = SYM; *(s32 *) &b[36] = b[86];` materialises the symbol once
  and makes all four accesses displacements. Written as `SYM[36]` and `SYM[86]`
  the base comes out as `%lo(SYM)+0x48` and a second `addiu` undoes it
  (func_8003AAF8, 7 words against 8).
* **A bare `.size` record wants an INCOMPLETE-ARRAY declaration, or the
  address materialisation goes $gp-relative too.** The trick in
  docs/addressing-and-translation-units.md gives "$gp-relative loads and
  stores, absolute address materialisation" -- but only if cc1psx emits no
  size of its own. `extern u32 D_X;` makes it emit `.extern D_X, 4` and sets
  `SYMBOL_REF_FLAG`, so `&D_X` becomes a single `la`.

  **CORRECTED 2026-10-27: maspsx does NOT rewrite that `la`.** aspsx-2.56's
  config has `gp_allow_la = False`, so the `la` is passed to gas, which at -G0
  expands it to `lui`/`addiu` in the SAME register -- which is exactly the
  compiler-split shape, reached by another route. The scalar declaration costs
  no word on an address materialisation; what costs a word is `a = (u32) D_X`
  with the scalar spelling, which reads the VALUE (`lw %gp_rel`) instead of the
  address. Write the `&`. func_8004DF8C MATCHED with both symbols scalar, and
  as incomplete arrays it is 74 words against 72, because two `high` pseudos
  then cross the call and a fourth callee-saved register goes with them.
  `extern u32 D_X[];` with `D_X[0] = v` still emits no size and still gets the
  one-word store, so the array form remains right where the listing shows
  `%lo(SYM)($sN)` off a live high half -- read which one off the listing.

* **An `__asm__` alias gives one function its own declaration of a shared
  symbol.** `extern u32 X_arr[] __asm__("X");` is a second C name for the same
  assembler symbol, so a function can use the incomplete-array
  (compiler-split) spelling while everything already compiled against
  `extern u32 X;` stays as it is. That is what the declaration warning above
  wants instead of widening: it unparked func_8004EE90/func_8004F52C without
  touching the declaration csrc/unk_12DEC.c depends on. Remember WHY two
  functions in one unit can need different forms of one symbol: the carve
  stands in for several original translation units.
* **cc1psx runs at -G8 for every unit** (only gas's -G varies), so a 4-byte
  scalar always gets the symbolic form, which is one insn until maspsx splits
  it. Instructions BETWEEN a `lui` and its load in a target listing therefore
  prove the original declaration was one cc1psx could not judge small -- an
  incomplete array. func_8004AC18 is a pointer global read that way.
* **A global `|=` next to a store through a pointer: read the global first.**
  In the compiler-split form the global's MEM is register-based, so sched will
  not hoist its load above a store through a pointer it cannot disambiguate.
  Reading it into a local before the store and writing it back after is what
  interleaves the two chains. In the symbolic (scalar) form the exception in
  `docs/matching-levers.md` section 5 applies instead and the plain `|=` order
  works -- so the two spellings are not interchangeable.
* **`u8 *c = (*q)++;` and `u8 b = *c;` as two statements costs a copy that one
  statement does not.** `u8 b = *(*q)++;` is 10 words; the two-statement form
  is 12 and has the target's register copy of the cursor. The mechanism is
  `protect_from_queue`; see matching-levers section 8.
* **Index off the symbol inside the loop body; do not walk a pointer.**
  `for (...) { u8 *e = SYM + i * STRIDE; ... }` keeps ONE giv, which is what a
  listing that recomputes `sll`/`addu` every iteration has. `u8 *e = SYM;`
  before the loop with `e += STRIDE;` in it makes loop.c add a SECOND giv
  biased to the first field it sees (`e + 0x12`) and reach the others with
  negative displacements -- 48 DIFF, and it is what func_80056408,
  func_800564A8 and func_8005A1D0 were all parked on. Write the index FIRST in
  the sum as well: `(u8 *) (i * 8 + (u32) b)` gives the target's
  `addu $v0, $v0, $base`, where `b + i * 8` gives `addu $v0, $base, $v0`.
  It does not help in a loop with NO call (func_8003C67C): gcc's loop.c halves
  its strength-reduction threshold when `loop_has_call`, so a call-free copy
  loop gets reduced whatever the source says.

  **And "write the index first" needs a pointer LOCAL to hold the base, not the
  symbol's own name.** `(Unk *) (k * 12 + (u32) SYM)` still comes out
  `addu $s0, $base, $index`, because `(u32) SYM` is a constant address
  expression and `fold` reassociates it to the end; `t = SYM;` then
  `(Unk *) (k * 12 + (u32) t)` makes it pseudo-plus-pseudo and gives the
  target's `addu $s0, $index, $base`. That was the last word of func_8005AE90,
  and `&SYM[k]` or `SYM + k` both give the other order. advise.py reports it
  as COMMUTED.
* **A handler called through a function-pointer field takes all the
  parameters it passes on.** The unk_72A8 `+0x68` handlers are
  `(u8 *p, u8 *q, s32 r)` and end in `(*(...)(p + 0x68))(p, q, r)`; `q` and
  `r` are otherwise unused, and their allocnos are the only thing that keeps
  other values off $a1 and $a2 (func_8003605C, 3 DIFF without them).
* **A masked flag returned as a boolean needs its own local.**
  `return (f & 0x100) != 0;` is rewritten by combine as `srl 8` / `andi 1`,
  where the target has `andi 0x100` / `sltu $zero`. `g = f & 0x100;
  return g != 0;` is the spelling; `> 0` and `!!` do not help.
* **Shortening a temporary's live range is the cheapest way to free an
  argument register.** func_8004D12C reads three halfwords in sequence; with
  ONE local reused for them the quantity spans the whole tail, local-alloc
  gives it $a1 and the parameter keeps $a0. With three separate locals each is
  block-short, one takes $a0, and the parameter is pushed to $a1 -- which
  makes gcc emit the parameter move the target has as its first instruction.
  No explicit `u8 *p = r;` produces that copy, in any distribution of the two
  names (10 DIFF).
* **Decrement in place and test a COPY.** `n = *p; m = n--; *p = n;
  if (m <= 0)` gives the target's `addu $v1, $v0, $zero` followed by
  `addiu $v0, $v0, -1`; written `*p = n - 1;` with the test on `n` the
  decrement goes to a fresh register, there is no copy, and the function is a
  word short (func_8004CB2C).
* **`&D_X` on a scalar is ONE `la` insn; an incomplete array's decayed name is
  a split lui/addiu pair.** reorg can put the addiu of the split form into the
  following call's delay slot, so a listing with an EMPTY delay slot before a
  call whose only argument is a symbol address wants the scalar declaration and
  `&D_X` (func_8003B0B4, one word).
* **A flag word that is masked and then `|=`'d in branches wants a `u32`
  local.** As `u16` the masked value and the or-result are two pseudos, so the
  path that takes no branch needs a copy between them, where the target
  spends a `nop` (func_800477F4, 12 DIFF).
* **A store followed by a load of the same place is how to get a register
  copy.** `*pp = f(...); b = *pp;` makes cse forward the load to the value
  just stored, and the forwarding emits `addu $rD, $rS, $zero` -- the very
  copy the parked rows are each one word short of. Assigning the call result
  to `b` and storing *that* keeps both live instead and costs a callee-saved
  register (func_80048EE4, 41 DIFF the wrong way). The same trick, with a
  store through a pointer in between to invalidate the entry, gives
  func_8005084C its `addu $v1, $a2, $zero` and then two real re-loads of the
  same field. This is the fourth known cure for that missing copy and the only
  one that works on a value the function itself produced.
* **Four long-lived block-locals in one block will take $a0-$a3 and lock a
  global allocno out of them.** `local-alloc` runs before `global-alloc` and
  orders its quantities by refs/live_length, so the short two-reference scratch
  values cycle through $v0/$v1 and the few longer-lived ones take the argument
  registers -- and the `;; NN conflicts` line in the `.greg` dump then shows
  the global conflicting with hard regs 2..7, whatever its priority. When a
  target holds one long-lived value in an argument register for a whole
  function and puts a *constant* up in $t0, that is the signature: the original
  block had one fewer such local. Costs exactly the parameter copy that the
  displaced argument register then needs (func_8004CECC, one word).

* **When two arms end in the same call with different constants, assign the
  constant on its own line FIRST.** jump2's `do_cross_jump` walks backwards
  from the two arms' jumps while the insns are equivalent and stops at the
  first difference, so the differing `lui` must come before the shared work,
  not after it. `f = 0x1010000; m = (k << 8) & 0xFF00; g(f | m);` merges the
  `sll` and the `andi`; `g(((k << 8) & 0xFF00) | 0x1010000)` emits the `lui`
  last, the walk stops at once, and the mask is duplicated into both arms.
  Worth 2 words on func_80055D84, and writing the constant first INSIDE the
  expression does not help -- fold reorders it, so it has to be two
  statements.

* **An arithmetic constant binds to the term the source groups it with.**
  `(v & 0xF) + 0x7C80` then `+ ((v & 0x10) << 3)` must be split into a local;
  written as one expression the compiler folds the constant into the second
  term instead, and parentheses do not survive `fold`.

## SOLVED: the stack-parameter prologue interleave was D_80082BC0's SPELLING

Three functions sat at the exact word count with every differing word in the
entry block -- func_80036B10 (3 DIFF), func_80036D4C (6) and func_8003719C (6)
-- all placing the first call's argument setup differently among the parameter
save/load pairs: the target alternates setup, pair, setup, pair, setup, while
the candidates emitted both pairs first and caught up on the setup afterwards.
Two rounds of work refuted the RTL-order story (the prologue is emitted after
reload by `thread_prologue_and_epilogue_insns`, so the LUID order is fixed in
both builds) and the LUID tie-break story (`assign_parms` always copies a
parameter into a pseudo at function entry), and left it as a sched2 ready-list
question with every entry insn at priority 1 and `potential_hazard` deciding.

It is none of those. All three call

    func_8003FF2C(0x11, (UnkNode *) p, 0x49, <list head address>)

and the fourth argument is the address of D_80082BC0. Declared
`extern UnkNode *D_80082BC0;` and passed as `&D_80082BC0`, its `lui`/`addiu`
pair lands AFTER both save/load pairs. Declared `extern UnkNode *D_80082BC0[];`
and passed by name -- the same two instructions, the same %hi/%lo -- it lands
BETWEEN them, and all three functions go to MATCH with no other change. It also
pulls `addiu $a2, $zero, 0x49` ahead of the first save, which is why the DIFF
counts were 3 and 6 rather than 2 and 4.

So `potential_hazard` was the right diagnosis and the wrong conclusion: the
scheduler's view of the pair is what differs, and the array spelling is what
changes it. The general rule is in matching-levers section 6 -- **when an
address is a call argument and the entry block interleaves it with the
prologue, try the incomplete-array spelling before reading another dump.**

func_80035E58 is the fourth member of the family (six arguments, same
func_8003FF2C call) and the array spelling takes it from 8 DIFF to 2; what is
left there is unrelated. See `docs/near-misses/func_80035E58.c`.

| func_8004A330 | unk_1C540 | **74/77 words, 74 DIFF** | Was 70/77. Four of the seven words came from declaring arguments 5 and 6 `s16` rather than `s32` -- advise.py's CALLEE-SAVED row applied literally, and the same lever that landed func_8004A24C. As `s32` each parameter's pseudo is a plain copy of its incoming MEM, so `update_equiv_regs` substitutes it back and the `lw` sinks to its single use; as `s16` the entry insn is a PROMOTION, the substitution cannot fire, and both live in callee-saved registers across the two calls. The listing's two `lw` at ENTRY are the signature. What is left is three words on `m`: the listing spends a callee-saved register and a copy on `n + 0x20` where this puts it straight into $a0, which it prefers as the callee's first argument; `m` crosses no call, so only a lower-priority conflicting allocno preferring $a0 would move it. The earlier reading that the 0x20 folds into the displacements was wrong -- `m` is a VAR_DECL and nothing folds. |
| func_80054B08 | unk_264E0 | **84/84 words, 32 DIFF** | Was 82/84. Both missing words are paid for: the three stores to +0x10 and the three to +0x14 are VOLATILE -- the listing writes each field three times with a refined value and never reads it, so flow deletes the first two of each as dead, and no non-volatile spelling keeps them -- and the -0x20/+0x20 go in their own statements so fold cannot move the constant onto `base`. What is left is 32 words of schedule: the target emits the block in source order while this hoists two record loads and sinks the three byte/halfword stores. `docs/near-misses/func_80054B08.c`. |
| func_800398BC | unk_72A8 | **76/76 words, 50 DIFF** | First function worked from an m2c draft -- 75 of 76 on the first compile, then two documented fixes, and all branch spans match. The missing word is the D_800721C0 index scaling: the target spends `srl`/`andi 0x1F`/`sll 2` where gcc folds the mask and the scale into `srl 0x11`/`andi 0x7C`. Three spellings measured against it in `docs/near-misses/func_800398BC.c`. |

## Parked: tried and not matching

Each of these has the right instruction set and word count with the registers
or the order wrong, so they want the dumps rather than another guess.

| function | unit | state | tried |
|----------|------|-------|-------|
| func_800368D0 + func_80035C80 + func_80035CE4 + func_80036900 | unk_72A8 | **LANDED** | All four. The +0x44 pointer takes a local (its `lw` then precedes the `lh`, sched1 tie-breaking on RTL order) and the +0x4A index stays INSIDE the subscript: given its own local it is a second quantity with the shorter live range, so local-alloc gives it $v0 and the pointer $v1, the reverse of all four listings. func_80036F24 is where that spelling was found. |
| func_80033AE8 | unk_72A8 | **MATCHED and landed 2026-10-27** | The bitmap CLEAR. Its parameter IS `u16` -- the listing's `andi $a0, $v1, 0xFFFF` is the parameter's own truncation, which leaves the raw argument in $v1 for the `& 0x1F` and lets the `sllv` take the constant 1 before the `lw`; as `s32` with the mask in the source, six of seventeen words differ. This row previously rejected `u16` because func_80039258's `func_80033AE8((s16) v)` needs the `sll`/`sra` a u16 prototype in scope would replace with a truncation -- the fix is to DEFINE the function under an `__asm__` alias, so the unit's own `void func_80033AE8(s32);` declarations and their call sites keep the wide view. New lever in `docs/matching-levers.md` section 7. |
| func_8004AFE8 | unk_1C540 | 14 DIFF | Counts entries whose `(*p)[1]` equals the first one's, with the loop's `p != 0` test at the bottom and a guard at the top. |
| func_800589B8 | unk_2879C | **MATCHED and landed 2026-10-27** | The byte reader every func_80058xxx call goes through. The cursor at +0x00 is DEREFERENCED INLINE in the address sum and read a second time for the increment, rather than named in a local: cse forwards the second read for free, and what it buys is the order of the two loads -- the target loads `%lo(D_80085FA4)` first and fills its delay slot with `lw 0x0($a1)`, while a local makes the deref's insn ready a cycle earlier, sched2's `potential_hazard` promotes it over the symbol's `high`, and the two come out swapped (2 DIFF). The `lim` read must also sit ahead of the cursor store or both delay slots go empty (20 words). |
| func_80058A04 | unk_2879C | **LANDED** | The 7-bit varint decoder. The three missing words were ONE variable for the byte, reused by the loop instead of a second variable for the loop's: that gives the allocno a live range where the block-local `c << 7` quantity holds $v0, so it cannot have the call's own return register and both `addu $a0, $v0, $zero` copies become real instructions. This is the conflict lever, not a shape. The two-variable spellings measured before -- a copy between them, a `u8` byte, a nested `if`, a `while (1)` -- are recorded in the comment on the function. |
| func_8004D690 | unk_1C540 | 29/31 words, 18 DIFF | Now has a PARK FILE with the source: `docs/near-misses/func_8004D690.c`. Every instruction is right and the function is two words SHORT: gcc moves the arm that ends the function to the END of the function, where it falls into the epilogue, while the target leaves it first and spends `j` plus a `nop`. Four more spellings measured on 2026-10-27 (`goto out` with the label at the end, the two arms as if/else, the second test inverted with `continue`, `while (*a == 0)` with the call after the loop at 30 words and 26 DIFF) -- so this is not an arrangement of the three blocks but whatever condition blocks `jump_optimize`'s block move. |
| func_8003C67C | unk_72A8 | 31/31 words, 28 DIFF | Now has a PARK FILE with the source and the row's open question ANSWERED: a backward `goto` DOES stop the strength reduction (32 words, 32 DIFF, one `sll` per iteration), so the reason loop.c always reduces this loop is the loop notes and not `loop_has_call`'s threshold. Adding a base local on top of the goto gives ONE materialisation copied to a register -- the listing's `addu $a2, $a0, $zero` -- at 30 words and 31 DIFF, but gcc then shares one address and reaches the source with a `-8` displacement where the listing computes two. The target sits between those two forms; `docs/near-misses/func_8003C67C.c` has all six measurements. |
| func_800565D8 | unk_2879C | **31/31 words, 3 DIFF** | The two-sided scale. Both bytes are VOLATILE -- the target loads each one twice, for the zero test and for the multiply, and cse forwards the tested byte in every non-volatile spelling; that is where the old 30/31 came from. What is left is two sites, and both are the same question: which of two equal-priority chains sched1 ran first. Full write-up and the fourteen spellings measured in `docs/near-misses/func_800565D8.c`. | SINCE PARKED: `d = n - 0x40` into a local before the `p[0x20]` test, so reorg fills the branch's delay slot from the insn before it rather than from the shared `return 0` -- 6 DIFF to 3. The three left are the `* 2` binding to the byte rather than to `0x3F - n`.
| func_8004E9EC | unk_1C540 | 32/33 words, 26 DIFF | Now has a PARK FILE with the source: `docs/near-misses/func_8004E9EC.c`. This row's 33/33 and 27 DIFF COULD NOT BE REPRODUCED -- the shape it describes measures 34 and 32, because loop.c gives the walked cursor TWO givs in the inner loop, one biased +2. The INNER LOOP AS A BACKWARD `goto` stops that (32 words, one `addiu` per iteration, the listing's shape), and an inner-bound local with the outer condition still recomputing `n * 2` is worth three more. What is left is one word, and it is named: the target copies the count out of $a1 so the result can live there, and `s32 n = count;` does not keep the copy. |
| func_8003A3E8 + func_8003A374 | unk_72A8 | 32/32 and 29/29 words, 12 DIFF each | BOTH now have park files, three words better than this row's 15. The twins differ only in the table lookup (`m = *(u32 *) (s + **cp * 4 + 0x48) * 4` against `m = **cp * 4`). The statement order is the family's and is pinned; what remains is that the target accumulates the operand in $v0 from the first byte where this uses $a1, and a step-by-step accumulation (func_8003A1A4's shape for its second composition) is 15, so the one-expression form is right and the registers are an allocation question. |
| func_8004CAA0 | unk_1C540 | not attempted | Switches `$sp` to the scratchpad at 0x1F8003FC around a call, saving the old stack pointer at that address. No C expression produces that; it wants inline asm, like func_8003E758's. |
| func_80036F24 + func_80037090 + func_80037394 | unk_72A8 | **LANDED** | The counter at +0x64 is VOLATILE: the target re-reads it after the increment and no non-volatile spelling stops cse forwarding the stored value (five measured, `MEM_IN_STRUCT_P` refuted). With `*(volatile u16 *)` on the re-read plus the pointer local above, all three match. func_80037394 is the same body with six copies instead of three. |
| func_8004D75C | unk_1C540 | 19 DIFF | A 0x44-byte block move (the odd word first, then sixteen words in a four-word loop -- so the shape is right) followed by `*(u32 *) s = 0;`. |
| func_80048FBC | unk_1C540 | 18/18 words, 7 DIFF | Now has a PARK FILE with the source: `docs/near-misses/func_80048FBC.c`. Eighteen words, one statement, and only the two callee-saved registers differ -- the chain (six refs over four insns) outranks the pointer (two over nine) and takes $s0 where the target has the pointer there. Six more spellings measured on 2026-10-27, all 18 and 7, so it is not a spelling of the statement; and the pointer's range is the whole function, so raising its reference count is the only side with room. |
| func_80056C30 | unk_2879C | **LANDED** | The fourth fact was the one slot: `i++` goes AFTER func_8005E97C, not between the two calls. An argument setup cannot be scheduled earlier than its chain position, but giv updates CAN be hoisted across it and across the call, writing callee-saved registers the call does not clobber -- so put the increment after the call and let sched pull the updates forward. Found by reading the sched2 dump with tools/decomp/sched.py. |
| func_80057948 | unk_2879C | **64/64 words, 9 DIFF** | The match-and-stop sweep. Four loop facts paid for, 69 words to 64, all the D_80085458 recipe. What is left is one register swap that `--greg` prices exactly: `m` (the OR accumulator, pseudo 83) and the D_80073740 giv (116) both have 7 refs, so the giv's shorter live range (28 against 36) wins it $s2 where the target has `m` there. 22.2% margin, and `m`'s live range has to span the guard, so the only lever left is ONE more reference to `m` that costs no instruction. `docs/near-misses/func_80057948.c`. |
| func_8005C4D0 | unk_2CDC0 | **64/64 words, 22 DIFF** | The D_80085458 teardown. Four things paid for, 62 words to 64: BOTH tails out of line with the FAIL tail first, which needs labels and gotos; the +0x1A flag read into a local BEFORE the D_80085F98 store, so the store can reach the `beqz`'s delay slot (advise.py's DELAY-NOPS advice, two words); a pointer local for each of the two table bases; and func_80056D44 called with THREE arguments even though its own listing never reads $a1 or $a2 -- the two- and one-argument forms are both a word short, because the third is what makes gcc keep `e` in a register and copy it. What is left is 22 words of prologue allocation following from `e` landing in $a1 rather than $a2. `docs/near-misses/func_8005C4D0.c`. |
| func_8005B070 + func_8005B16C | unk_1C540 | **LANDED** | Both. The fourth fact, after the three loop-recipe ones, is that the stack primitive is a STRUCT (`Unk5F134SP`) and not `u8 buf[0x40]` with casts: struct accesses carry MEM_IN_STRUCT_P, every varying-address load then depends on them, and the body comes out in strict source order with both load-delay slots empty. The store order matters with it -- +0x04, then +0x00 from the table, then the two halfwords -- or the table load moves two slots earlier and takes a second register. |
| func_8004AC3C | unk_1C540 | **129/128 words, 94 DIFF** | The record resolver. The STRUCTURE is verified and is what the file is for: labels and gotos for the two shared blocks, the func_80049374 family's `n = p[0] * 4` / `* 2` with one shared `(n + p[0]) * 4`, `p[0] * 20` spelled out in the two arms whose final offsets differ, a RAW CONSTANT `(u8 *) 0x80010324` base for mode 1's 0x13 case, and one `return` at the single exit. What is left is that the target re-reads `p[1]` at the head of each of the four blocks that index D_80073414 while cse forwards it here -- only one of the four has a store to explain it. `docs/near-misses/func_8004AC3C.c`. |
| func_8004D294 | unk_1C540 | **114/114 words, 27 DIFF** | Three identical nine-word Euler-angle wraps. The eight words missing from the m2c draft were a SECOND func_8004D1EC call -- m2c truncated before it -- plus the $s2 save that only exists because the third parameter is used on both sides of the first call. D_80083178 is a `u8 *` base with byte displacements, since 0x140 is past the end of its struct, and the caller's vector is read `lhu` for the masks and `lh` for the difference at the same offset. What is left is ONE register choice replicated three times: which of the angle and the sign-extended reference gets $v0, which also decides where the spare `lhu` lands. Five spellings of the subtraction measured, all 27. `docs/near-misses/func_8004D294.c`. |
| func_800550E8 | unk_2879C | **MATCHED and landed 2026-10-27** | The turn-commit. The last word was a semantic error, not a lever: D_80084878 holds POINTERS, and the listing's `lw $a0, 0x0($v0)` off the computed slot means `*(u32 **)(r2 + ...)`, where the park passed the slot's address. The rest is addressing: each lookup's ROW ADDRESS is its own statement (3 and 7 DIFF), the two tables take SEPARATE pointer locals (one serving both is 121 words, 77 DIFF), and the D_800847D0 base is re-materialised inline for the last call so the `addiu` fills its delay slot. Both new levers are in `docs/matching-levers.md`. |
| func_80056654 | unk_2879C | **107/107 words, 21 DIFF** | Builds one sprite primitive in D_80084918 from the D_80084960 entry the +0x06 index selects. Five things paid for, 116 words down to 107: `func_800565D8` declared returning `s32` here though it really returns a sign-extended `s16` (the listing adds $v0 straight in); D_80084918's base a `u8 *` local assigned INSIDE the `if` and after the call, or it takes $s2 and its save; the +0x16 store through that local rather than `D_80084918 + 0x16`, which otherwise biases the shared base; the D_80073740 lookup into a local read early and stored late; and `x` refined in place with the shift term first. What is left is 21 words of schedule in the primitive-build block. `docs/near-misses/func_80056654.c`. |
| func_80057D20 | unk_2879C | **MATCHED and landed 2026-10-27** | The controller-change applier, 463 words, and with it jtbl_80032F04 is PLACED -- seven of the executable's eight jump-table runs. The last six words were a giv ordering: a SECOND row pointer at offset ZERO, declared after the inner do/while, is created after the copy move_movables hoists out of that loop, so `combine_givs` makes it the D_80085458 group's representative and the two emission orders swap -- with the base unbiased, because the representative's add_val is the plain base. New lever in `docs/matching-levers.md` section 2; everything else the park had paid for is in the function's header comment in csrc. |
| func_8005914C | unk_2879C | **205/204 words, 131 DIFF** | The MIDI channel-message dispatcher, 204 words, seven status nibbles in a comparison tree with the 0xB0 controller change holding 128 of them and all three loops. Every word outside the first two loops matches. Five shapes paid for: the body order 0x80, 0x90, 0xA0, 0xB0, 0xE0, 0xC0, 0xD0 (read off the body addresses, since the tree order says nothing); `u8` parameters, which is what puts an `andi 0xFF` at each of the fourteen uses; the two save/restore loops as WHILE loops, so `duplicate_loop_exit_test` makes the guard and the bottom test ONE source expression and they share the D_800869B4 address pseudo; the entry pointer as the inline indexed form, since a pointer local costs loop.c a `move` off the giv and a walked pointer costs a second biased giv; and the third loop's OWN counter rather than sharing the first two's, one word, by the one-variable-per-loop lever (a reused counter is one allocno whose refs sum across disjoint ranges). What is left is ONE word behind two load-delay nops: the listing's in-loop bound load sits mid-body with `i++` behind it, and sched cannot hoist it there because the load's base is a symbol and the stores go through the giv. Reading the bound into a local mid-body does move it and kills both nops, but then it is a second expression and the address `move` comes back. The guard-plus-do-while ordering that killed the same nops on func_80057D20 does NOT transfer: 208 words, because this listing materialises its `%hi` once per loop where that one does it twice. `docs/near-misses/func_8005914C.c`. |
| func_8005BBFC | unk_2CDC0 | **333/333 words, 220 DIFF** | SPU voice start, 333 words: resolves the program record for (bank, prog, key), waits for the voice to go idle, sums the sample-table lengths for the wave address, fills a `SpuVoiceAttr` on the stack and the 0x78-byte voice record, and calls func_8005EC0C. Word count exact and the logic settled; nine shapes paid for, including the channel as an `s32` local off the `s16` parameter (12 words), `<< 1` not `* 2` for both pan doublings, the two adsr halfwords through a local, `attr.volume.left` READ BACK so its copy fills the next load's delay, and the voice-record pointer reassigned per block so the field offsets stay displacements. What is left is one allocation tie-break -- the listing spills `prog` and keeps `key` in $fp, this source does the opposite -- plus the prologue placement of the two stack parameters and the order of the record stores. `docs/near-misses/func_8005BBFC.c`. |
| func_8005B4D0 | unk_2CDC0 | **379/376 words, 367 DIFF** | func_8005BBFC's sibling, the note-on that has to FIND a voice: counts the bank's live programs, walks the +0x20 record's layers, and for each layer whose key range matches scans for an idle hardware voice and then for a free D_80085458 entry. Everything from func_80056DB4 on is func_8005BBFC's block, so that park's shapes all apply. The new lever here, worth 10 words, is that both scan loops need an explicit `goto` rather than `while` or `for (;;)`: the listing has ONE copy of the exit test with the back edge jumping to it, and either loop form lets duplicate_loop_exit_test copy the test to the bottom. What is left is three words and five nops, and the six memory-resident locals whose frame slots have to match -- including two source locals holding the same pointer value, which is not yet explained. `docs/near-misses/func_8005B4D0.c`. |
| func_8004B954 | unk_1C540 | **289/290 words, 265 DIFF** | The ring builder: walks a circle of vertices around a sprite's bounding box and writes one GPU primitive per step, 0x1C bytes or 0x14 depending on the seventh argument. THE FRAME IS NOW EXACT, and the census found both shapes that did it: the vertex is `s16 v[2]` stored as a word, not two `s16` locals (which make the second dead, slide every later local down four, and are not even faithful); and ONE walking pointer shared by the two exclusive loop arms, not two (a second costs an extra spilled local, an extra saved register and eighteen words). A DVECTOR struct assignment is 315 words, an SVECTOR 308, `*(s32 *) &cx` 259. What is left is ONE word, and the only census rows still differing are INCOMING ARGUMENTS: the listing keeps arg0 and arg6 in their home slots and reloads them, where this build holds them in registers. `docs/near-misses/func_8004B954.c`. |
| func_80056E10 | unk_2879C | NOT ATTEMPTED, 718 words | **The third member of the func_8005BBFC / func_8005B4D0 family** and the largest function left in the executable: the full note-on, with THREE fallback scans for a voice (by channel in D_80085458, then by idle state through func_8005EB78, then a third that accepts state 2) before the same voice setup block the other two share. Read docs/near-misses/func_8005BBFC.c and func_8005B4D0.c first -- every shape they list applies, and m2c's reading already shows the unrotated `goto` loops func_8005B4D0's park documents, three of them. Nothing attempted beyond that reading. |
| func_800345B8 | unk_72A8 | NOT ATTEMPTED, 546 words | The GTE transform pass, and the only remaining function that is not ordinary C: it writes its operands straight into the SCRATCHPAD at 0x1F800000, calls RotTransPers out of it, and feeds D_8006CD10 / D_8006A84C / D_8006CD4C. 80 stores, one loop. The scratchpad addressing is the unknown -- how the original spelled those absolute addresses decides most of the function -- so expect to settle that before anything else. Nothing attempted. |
| func_8005947C | unk_2879C | **81/81 words, 12 DIFF** | The sequencer event dispatch. Four shape findings paid for -- two variables for the byte (the raw s32 and a `u8` copy), a `u32` index so the fold comes out, `i = r` last in its arm, and the three special cases as a `switch` so the 0xFF body is laid out before the 0xF0 one. What is left is two coupled allocation facts: `b` in $a2 against the target's $a0, which also decides whether the tail's mask is in place and so whether reorg can hoist it, and `t`/`c` holding each other's callee-saved registers on a near-tie in global-alloc's priority. `docs/near-misses/func_8005947C.c`. |
| func_80059D1C + func_8005C818 | unk_1C540 | 16/16 words, 9 DIFF | Sweeps over D_80085FA8's 0x50-byte entries. func_80059D60, the third of this family, LANDED once the bound was read in the loop condition rather than hoisted into a local -- but these two STORE in the loop body, so loop-invariant motion cannot lift that read, and the in-loop re-read stands where the target has the hoisted copy. A guard plus a second read inside the `if` (cse's copy) is worse at 14/16. So the question is why gcc could hoist a load across stores whose base is the loop's own giv: that is an alias-analysis question, not a source-shape one. |
| func_80047738 + func_8004827C | unk_1AF38 | 17 and 12 DIFF | Both shapes are close; func_8004827C's target copies the parameter into `$v0` before using it, which no spelling here reproduces. |
| func_8005C2C0 | unk_2CDC0 | **132/132 words, 10 DIFF** | Keys a voice on: claims the D_80085458 entry for the channel, fills it, and calls func_8005EC0C with a stack `SpuVoiceAttr` (the real Psy-Q type -- the mask 0x1FFEF is SPU_VOICE_* minus PITCH, which is the second witness; `libspu.h` is now in common.h for it). EVERY instruction matches; what is left is where sched2 puts ten of them in the entry block. Four things paid for, 84 DIFF down to 10: `s16 j = a` (a TRUNCATION, so cse leaves the in-place `sll`/`sra` on the parameter -- `s32 j` is a copy and cse rewrites the shift to read it); a pointer VARIABLE `q = D_80085458` for the base, because the symbol inline is TREE_CONSTANT and fold swaps it right, expanding the index chain first and costing p its $v0; `st.volume.left` read BACK from the entry, so cse turns the load into the listing's reg-to-reg move; and the four constant bytes in channel-interleaved order. `docs/near-misses/func_8005C2C0.c`. |
| func_8005A90C | unk_2CDC0 | **100/103 words, 92 DIFF** | The third of the func_8005A608/func_8005A778 family, with the longest tail (func_80059F8C, func_8005ECA0 and func_8005EC40, each bailing out with -1); the tail is landed func_8005AAA8's word for word, including the `u8 *` cast for the +0x0C store. The dispatch, the search loop and every instruction of the body are in place, on the family's shared `j = id; id = id << 16; id = id >> 16;` lever plus `s16 i` and a single `k = i` at the join. What is left is FOUR words -- two callee-saved registers. The listing keeps six values live across the calls and this keeps four, merging `c` with the entry pointer and `i` with func_8005ECA0's result; the merge is reload's, since `c` is still live at `e->unk_08 = c` (a store through the entry pointer) and the copy that frees the register is func_80059F8C's own argument load moved ahead of the stores. NOT blocked on the other two, which are stuck on the `-1` constant's register instead. `docs/near-misses/func_8005A90C.c`. |
| func_80038CB8 | unk_72A8 | **164/164 words, 12 DIFF** | The tile-row emitter: three near-identical blocks walking the 2-byte records at +0x1C through func_8003AB44. EVERY instruction matches; 12 words of the second loop's register naming are left. Four shapes paid for: the record pointer is TWO locals, because the listing's condition reads the next record as `lb 0x2($v1)` off the OLD pointer while the loop top reads the new one (ten words); the FIRST loop's carried pointer is a READ-BACK of the field it just stored, which is the `addu $a0, $v0, $zero` in its delay slot, while the SECOND loop's is a plain `r + 2` with a nop -- the two loops differ; the two loops need SEPARATE locals for the carried pointer (the listing gives them $a0 and $v1, which one pseudo cannot do) but SHARE the old one; and `p[0x28]` is `*(s8 *)` throughout. `docs/near-misses/func_80038CB8.c`. |
| func_800599B0 | unk_2CDC0 | **133/133 words, 1 DIFF** | The sequencer start: picks the format from the first byte at D_80085FA4 and initialises all D_800869B4 D_80085FA8 track entries, one way for formats 0 and 1 and another for format 2. ONE word differs -- a `sw` reads $v0 where the listing reads $v1. 126 DIFF down to 1 on six things: `switch ((s32) D_800737C8[0])` with the cast, because unsigned gives one `sltiu` where the listing has gcc's signed `bltz`/`slti`/`beq` tree; both entry pointers INDEXED inside the body (walked, loop.c adds a second giv biased to +0x40 -- 40 words); format 2's length halfword indexed off a base read before the loop, which is what puts the giv's initialisation in the preheader; `f = D_800737C4[0]` read into a local BETWEEN the byte and word stores of the shared tail (advise.py's DELAY-NOPS lever); format 2's +0x04/+0x38/+0x1C spelled as READ-BACKS, which is where the listing's four reg-to-reg moves come from; and the +0x18 store sitting between +0x04 and +0x38, which is the word that gets it to 133. `docs/near-misses/func_800599B0.c`. |
| func_80037714 | unk_72A8 | **LANDED** | The two-quad actor draw. 355 DIFF to a match. The last seven words were ONE shape: the u and v running values are TWO locals, not one reused -- the two chains sit in different basic blocks and the listing gives them different registers, which one pseudo cannot have. Before that: the four RotTransPers4 screen-coord slots are 8 bytes apart (not `long sxy[4]`); `a = p + 0xC` is one pointer and cse is what makes `a[0]` print through the parameter; the ot index is INLINE in `AddPrim` (the opposite of func_800369C8 in the same unit, because here the add has a hard argument register to tie to); `g = &D_80083160` used in the second half only; and five levers now in matching-levers section 8 -- chained assignment, inline `|` for the four `||` terms, the front end narrowing `% C`, and the rest. |
| func_8005C668 | unk_2CDC0 | 11 DIFF | Builds a 0x40-byte block on the stack (a word from D_80073740, the constant 3, two halfwords) and passes its address. |
| func_800489F4 | unk_1AF38 | not measured | func_80047738 plus an `ori 0x20`; park it with that one. |
| func_80049E1C | unk_1C540 | 20/20 words, 8 DIFF | Now has a PARK FILE, ten words better than this row's 18 and at the exact count. Three things buy them, all layout or the conditional-expression form: BOTH arms as conditional expressions (a plain `if` on the high clamp optimises the copies away, 17 of 20 words); the `m == 0` answer written LAST (`if (m != 0) { ... } return 0;`), since the listing's `beqz` jumps to the function's last block -- 8 differing words; and the POSITIVE arm as the fall-through, since `bltz $a1` branches to the low clamp -- 12 the other way. A separate `r` is needed too: returning from each arm directly is 17 words. What is left is which register each arm's value lands in. |
| func_80058510 | unk_2879C | **MATCHED and landed 2026-10-27** | The frame tick's reentrancy guard. It is declared `s32` and never returns a value, and that one word is the whole difference: the target leaves the second guard's `bnez` delay slot EMPTY where a `void` function fills it with the `addiu $v0, $zero, 0x1` from the fall-through. The fill is legal only while $v0 is dead on the taken path; a non-void return type makes it live out at the epilogue and reorg declines. This row had guessed at reorg's `own_thread_p` -- the answer is liveness. New lever in `docs/matching-levers.md` section 1. |
| func_8003A2E0 + func_8003A10C | unk_72A8 | **BOTH MATCHED and landed 2026-10-27** | This row had the tail as "ONE `s32` at +0x48 read four ways" and sat at 7-23 DIFF chasing a misplaced `sll` through all 120 permutations. The missing piece was the RE-READ: the four byte stores each read `*(s32 *) (t + 0x48)` again, because each store through `q` invalidates cse's memory table, so the next read is real and gcc narrows it to the width the shift needs -- `lbu 0x48`, `lw 0x48` with `sra 8`, `lh 0x4A`, `lb 0x4B`. A local for the field gives one load and three shifts. Both functions matched on the first compile with that. |
| func_80055864 | unk_2879C | **MATCHED and landed 2026-10-27** | Sixteen of its nineteen words were the SIX BIG CONSTANTS IN LOCALS, declared in the order the listing materialises them, which puts all six `lui`/`ori` pairs up front instead of one between each store (19 DIFF inline). The last three were the FIRST STORE'S VALUE: hoisted into a `u8 *b = D_801B9000;` local its `lui`/`addiu` is emitted at the declaration, ahead of the destination's `lui`; written `(u32) D_801B9000` inline at each of the four sites, expand_assignment takes the lvalue address first and the pair comes out in the listing's order, with cse still sharing one address pseudo across the four stores. NOTE: the park header had recorded that exact spelling as "3 DIFF" -- measured before the constants became locals. Re-measure a rejected spelling after any other shape in the function changes. New lever in matching-levers section 8. |
| func_80050A58 | unk_1C540 | **MATCHED and landed 2026-10-27** | The two things this row had pulling against each other are both satisfied by keeping `q` UNMASKED in a local and casting at each of its two uses, with the remainder written INLINE in the final expression: `*(u32 *) (e + 8) = tbl[(u8) q] + (u8) (c - q * 21) * 12;`. The remainder in a local of its own puts the 21-multiply before the index chain (22 differing words); masking `q` in the local makes the multiply read the masked quotient. |
| func_800353F4 | unk_72A8 | **MATCHED and landed 2026-10-27** | The retry gate. The `||` gives the first test a branch INTO the body and the second one a branch past it (nested ifs, 22 DIFF), and the clear is func_80035378's `&D_80082A38w[i]` plus a 0x48 displacement. The last two words were D_80082A70's DECLARATION: as a scalar the `lui` and the `lw` share one register and the pair cannot straddle the `beqz`, while `extern u32 D_80082A70[];` read through `[0]` gives the high half its own register, so reorg puts `lui $v0, %hi(D_80082A70)` in the branch's delay slot and the load reads `%lo(...)($v0)` after it. The row had tried a `u8 *` type but not the incomplete array. func_800352FC had to be widened to return s32 (a tail call, so its own code is unchanged). |
| func_8003666C | unk_72A8 | 34/34 words, 7 DIFF | func_800364EC's twin with the half-width offset. Now has a PARK FILE with the source: `docs/near-misses/func_8003666C.c`. Everything is paid for except which of the three loads off `s` comes last -- the last one overwrites the base register, and the target spends it on `lbu 0x26` where this spends it on `lh 0x22`. Four more LUID-changing spellings were measured on 2026-10-27 (the halfword in an s32 or s16 local, one `s16` reused for both, a local for the byte) and ALL leave the same seven words differing, so the decision is in the dependence graph, not the ready list. |
| func_8004827C + func_8004D880 + func_800537D0 + func_80055CF4 + func_8003BD84 + func_80058A04 | unk_1AF38, unk_1C540 | exactly 1-3 words | **The same missing word three times: an `addu $rX, $rY, $zero` that local-alloc here coalesces away and the target keeps.** In func_8004827C and func_8004D880 it is the PARAMETER: the target copies the incoming $a0 into $v1 and uses $v1 for everything, spending $a0 on the outgoing argument, while this gives the parameter pseudo $a0 itself and copies it out where it is needed. func_8004D880 is otherwise exact, including the one-insn 16-bit subtractions (`u16 d = c - 0x824F;` -- as `s32` the constant will not fit an `addiu`) and the unmasked sum reused for the return (`return d + 0x30;` folds to one `addiu` only if `d` is NOT a local). Measured on func_8004D880: a local for the parameter, three separate difference locals, an explicit `& 0xFFFF` form and a `u32` parameter all coalesce the same way. func_80058A04's row below was the same shape on a call's return value, and it LANDED: unifying the two bytes into one variable imports the loop's $v0 conflict. That is a fifth cure, and the first that works by making the register unavailable rather than by rearranging the source around the copy -- but it needs a second live range for the value, which a parameter does not have. The three known CURES for this copy are all elsewhere in the source: re-dereference instead of caching (func_80039730), an explicit guard plus do-while on a loop bound (func_80059D1C), and a statement BETWEEN the producer and the copy (func_80048734). None of them applies to a parameter, which is what makes these the hard ones. func_800537D0 (the decimal formatter) is 3 DIFF of exactly this: everything else is right once `u8 *d = buf + n;` stops loop.c strength-reducing `buf[n]` into a walked pointer and the digit arm comes FIRST (`if (blank == 0)`), but the target computes the quotient into its own register, tests THAT, and copies it into `v` in the branch's delay slot. `q = v / 10; if (q == 0) ...; v = q;` in every order, a local copy of the parameter, and `if (v / 10 == 0)` with the division repeated all coalesce. func_80055CF4 is the same word on a SYMBOL's address, and it NOW HAS A PARK FILE that says what the target does with it: the address is materialised once into a caller-saved temp and COPIED into $s1, because `b` is live across the call and must end up callee-saved, where this source has local-alloc produce the lui/addiu pair straight into $s1 and need no copy. None of the five cures applies and the return-type lever does not either (identical, 35/31). One variant scores 36/12 by adding a second materialisation, but its three address insns are not the target's -- the park says to resume from the faithful shape instead. **func_8004827C, func_800537D0, func_80055CF4 and func_8003BD84 now all have park files** (18/19 and 12 DIFF, 34/34 and 3, 35/36 and 31, 37/38 and 24 respectively), so every member of this row except func_8004D880 -- which already had one -- can be swept. func_8004827C's park (`docs/near-misses/func_8004827C.c`) at 18/19 and 12 DIFF, with the note that today's return-type lever (matching-levers section 1) is the "make the register unavailable" cure in a form a parameter CAN have, and that func_80055CF4 and func_8003BD84 should be checked against it first; it cannot help func_8004827C itself, whose copy wants to go INTO $v0.
materialises `&D_800847D0` into $v0 with `lui` + `addiu` and then copies it to $s1, where this
emits `addiu $s1, $v0, %lo` straight into the callee-saved register. Its logic is otherwise exact
once the `& 0xFF000000` of the field is its OWN local (inline, fold reassociates it onto the
shifted mask, 33 DIFF) -- `u32 *b` with `b[1]`, a second read of the symbol and a separate
assignment statement all coalesce. func_8003BD84 (the ratio: `|p| < |q|` picks which of
func_800644B8/func_80064584 divides) is 20 DIFF at the same length, and it is this copy TWICE:
the target takes the absolute values as `addu $v0, $s1, $zero` in the branch's delay slot
followed by `negu $v0, $v0`, where every spelling here -- `a = p; if (a < 0) a = -a;`,
the test on `p` instead, `0 - a`, a `?:` and a full if/else -- negates the PARAMETER's register
directly and leaves the copy above the branch. |
| func_80056DB4 + func_800563B0 + func_80056D44 | unk_1C540 | 3, 14 and 27 DIFF | ALL THREE now have park files. The shared addressing recipe is in `docs/near-misses/func_80056DB4.c`: the base a POINTER LOCAL initialised in its declaration, the +0 store through the SYMBOL, the D_80073740 lookup in its own local, and NOT a struct type (as COMPONENT_REFs the +0 store folds onto the register base and the function is a word short). func_80056DB4 reaches 2 DIFF with the +0x24 store written BEFORE the two halfword stores AND the two halfwords in the reverse of their emitted order (perm.py over all 120 orderings: 2, 3 and 4 DIFF for `1 4 3 2 5`, `1 4 2 3 5` and the listing's own `1 2 3 4 5`). func_80056D44's last two words are NOPS -- the target fills one of three load-delay slots and this fills all three, so the instruction count excluding nops is already right. func_800563B0 is at 14 where the row expected 12 and never said which arrangement gave them; it has had no permutation run of its own. |
| func_800491F4 | unk_1C540 | not measured in isolation | A list walk that threads each entry's +0xC to the previous one. |
| func_8004F330 | unk_1C540 | 40/40 words, 9 DIFF | Now has a PARK FILE with the source: `docs/near-misses/func_8004F330.c`. The p/q split is the REVERSE of the obvious one -- `u8 *q = p;` as a declaration initialiser, the LOOP's stores through `p`, the three trailing stores through `q`. The other way round is 40 words and 15 DIFF and it gets the REGISTERS right (the loop addresses off $t3, as the listing has them) while losing the insn order inside the body, so the 15-DIFF variant plus an order fix may be the shorter road than this one plus a register fix. Four more loop orderings measured at 30-41. |
| func_8005A56C | unk_2CDC0 | 23 DIFF | Two stack blocks selected by the first argument, with `(x << 16) >> 8` scaling. |
| func_80047CD8 | unk_1AF38 | **MATCHED and landed** (this row kept for its measurements; it was stale and is corrected 2026-10-27) | The target keeps the constant -1 in `$s1` across two calls and compares both results against it; this puts the first result there instead. An explicit `s32 m = -1;` local does not move it. |
| func_8004D880 | unk_1C540 | 28/28 words, 22 DIFF | A Shift-JIS to ASCII classifier: three `(u16)(c + K) < N` range tests, each returning that sum plus an ASCII base. Two shapes now paid for: the SUM is its own `u16` local (inline in both the test and the return, `fold` merges the two constants, and because the return type is `u8` it merges them MOD 256 -- the listing's `addiu $v0, $a1, 0x30` comes out `addiu $v0, $a0, 0xE1`), and the third test REASSIGNS the parameter so its `addiu` writes $v1 in place. What is left is a three-way register permutation: the parameter and the call's argument both prefer $a0 and conflict, and the parameter wins it here on references. Note the listing spends TWO copies on that value -- in at entry, back out for the call -- so keeping it in $a0 is a word SHORT, not better. `docs/near-misses/func_8004D880.c`. |
| func_8003A858 + func_8003A060 | unk_72A8 | 43/43 words each, 2 DIFF | **Byte-identical listings**, and BOTH now have a park file (`docs/near-misses/func_8003A858.c` and `func_8003A060.c`, identical but for the name). Everything is paid: the table index's `* 4` in its own local right after the read (17 DIFF inline), the destination as `a * 4 + (u32) state` and not `state + a * 4` (8), `m` at the END of the operand sum (11), and the four accumulating stores. The two that remain are one adjacent ALU pair -- the target emits the destination's `sll` before the operand chain's last `addu`, this source after -- and three more placements measured on 2026-10-27 (7, 7, 2) on top of perm.py's 120 orderings say it is not statement order but sched1 inside one priority group. |
| func_80048B8C | unk_1AF38 | 44/44 words, 14 DIFF | Now has a PARK FILE with the source: `docs/near-misses/func_80048B8C.c`. This row's first finding is sharper than it was stated: a `u32` local for the halfword is NOT sufficient, because `v & ~0x3F` is still narrowed by fold to `andi 0xffc0` and the function runs a word SHORT. The mask needs ITS OWN STATEMENT in place (`v &= ~0x3F;`), or the cast and full constant written out -- then it stays 32-bit, loop.c hoists `addiu $t1, $zero, -0x40` into the prologue and the loop gets the listing's register `and`. New lever in `docs/matching-levers.md`. The remaining fourteen are the three registers the row names. |
| func_8003A904 | unk_72A8 | 46/45 words, 44 DIFF | Now has a PARK FILE with the source: `docs/near-misses/func_8003A904.c`. The whole diff is ONE word at the top -- a `nop` this source has in the cursor load's delay slot where the target fills it with the state load. THE NEW FACT: its LANDED sibling func_8003A1A4 HAS THE SAME NOP (decoded out of the ROM at 0xD9A4), so the shape is right and matched there; the two differ only in the tail, one store against four accumulating ones. The hoist is the tail's extra insns changing sched1's ready list, not anything the top of the function asks for -- read the sched1 dump of both, not another spelling of the two loads. |
| func_8003B714 | unk_72A8 | **MATCHED and landed 2026-10-27** | This row had the right diagnosis and the wrong cure. The comparison constant 0x13 must not be hoisted, and the only body it found that stopped loop.c hoisting it read the table byte TWICE -- three words over. THE LOOP IS A BACKWARD `goto`: then there is no loop for loop.c to hoist out of, a single `s32 v` local for the byte is right here as in the twin, and the function matches. Third function in a day settled by the goto form (matching-levers section 2). |
| func_8004E4C0 | unk_1C540 | 46/47 words, 45 DIFF | Now has a PARK FILE with the source: `docs/near-misses/func_8004E4C0.c`. The one missing word is a REGISTER artifact, not a statement: both streams mask the `u16` width three times, but the target's first mask lands in $v1, which the loop body clobbers building the x10, so the bound must be copied to $t2 before the loop; here it lands in $t1, which nothing touches, so there is no copy to make. An `m = w` local is coalesced away. |
| func_80048C3C | unk_1AF38 | 48/49 words, 15 DIFF | Now has a PARK FILE with the source: `docs/near-misses/func_80048C3C.c`. Every instruction is right and the function is one word short: the target advances the pair pointer IN PLACE (`addiu $s1, $s1, 0x4`) and then copies it to $v0, where combine folds the pair into one `addiu $v0, $s1, 0x4`. Six more spellings measured on 2026-10-27 (`e++`, `e = e + 1`, `e += 1`, `return &e[0]`, `return (u32 *) e`, the D_80080A89 store on either side) all 48 and 15. The fold needs a second USE of the pointer after the add to block it; the park says where to read the condition in combine.c. |
| func_8005552C | unk_2879C | 50/52 words, 37 DIFF | Now has a PARK FILE with the source: `docs/near-misses/func_8005552C.c`. Two of the four missing words came from making the two `m = 1` SITES ONE STATEMENT reached by a `goto`: a 0/1 pair always triggers jump.c's store-flag transform, because the gate at jump.c:1139 fires whenever either value is `const0_rtx`, so no spelling of the PAIR escapes it -- removing the pair does. New lever in `docs/matching-levers.md`. What is left is two words and the arm ORDER: the listing lays the bodies out default(1), 0x600(2), 0x700(3), 0x800(0) and branches INTO the m=0 arm, where the nested-`!=` chain emits 1, 0, 3, 2 and branches away. |
| func_8004A574 | unk_1C540 | 41/41 words, 5 DIFF | Now has a PARK FILE with the source: `docs/near-misses/func_8004A574.c`. The arm order is `w == 0` FIRST, worth three words -- the listing's `bnez $a1` branches to the +0x12 arm and falls through into the +0x10 one, so the `w == 0` body must be written first (8 DIFF the other way). The five that remain are the `mflo`'s position: the target has it in the JOIN block above the function-pointer load with `addu $a0, $v0, $zero` in the `beqz`'s delay slot, where this sinks it past the branch straight into $a0 and has no copy to make. Two more negatives measured. |
| func_8004E130 | unk_1C540 | **MATCHED and landed** (this row kept for its measurements; it was stale and is corrected 2026-10-27) | $gp group. Every instruction matches; only which of $v1/$a0 holds the masked index and which the table address. |
| func_8003C634 | unk_72A8 | 7 DIFF | $gp group. Reading the bound in the loop condition took it from 18 DIFF to 7; what is left is the register pair, as in func_8005C818. |
| func_80033D44 + func_80033D54 | gprel_72A8 | **func_80033D44 MATCHED and landed 2026-10-27; func_80033D54 PARKED at 537 of 537 words, 46 DIFF** | Both were BLOCKED, and the cure was a unit carve rather than a spelling: they need `%gp_rel(D_8008099C)` while func_80034EF8 and func_8003C0C8, further down the old unk_72A8, reach that word ABSOLUTELY and are landed, and maspsx's size record is file-scope. The __asm__-alias trick was tried first and does not lift it (the absolute form those two match is gas expanding a BARE SYMBOL through $at, which no C spelling reproduces once the symbol is in sdata_entries), so the old unit was cut immediately after func_80033D54: splat yaml `[0x72a8, c, gprel_72A8]` + `[0x7db8, c, unk_72A8]`, the second piece keeping the old name and so its asm directory, park files and rows. Thirteen matched functions moved and all four containers stayed byte-identical. func_80033D44 then matched on the first draft: `s32 func_80033D44(s32 a) { return D_8008099C = a; }`. **func_80033D54, 537 words, now has a park file** (`docs/near-misses/func_80033D54.c`) at the exact word count with 46 DIFF, and EVERY INSTRUCTION IS RIGHT: 34 of the 46 are the names of four callee-saved registers (two adjacent pairs swapped in global-alloc's priority order, margins 5.6% and 16.6% in `advise.py --greg`) and the rest are two scheduler tie-breaks. Both allocation moves have been DEMONSTRATED with one deliberately wrong instruction each -- `q[-1]` for one `e->unk_00` read takes an inner-loop reference off `e`, and `((s32 *)m2)[-1]` for the +0xC4 store adds an outer-loop one to m2 -- which together take the same source to 12 DIFF, so what is missing is a spelling that moves those reference counts without moving an instruction. It is the fourth member of the `(u8 *p, UnkVec *v, UnkSlot *s)` family and its inner loop is func_80044D24's with the scratchpad offsets shifted by +4. Three new levers came out of it (matching-levers section 8): the +1 displacements are a REDUCED GIV rather than a second pointer in the source; a volatile MEM is not replaced by the reduced giv, so the two sign bytes need a pointer local with the cast at the use; and a struct copy is a four-register block move where the listing wants eight separate word assignments. |
| func_80039DBC | unk_72A8 | **MATCHED and landed** (this row kept for its measurements; it was stale and is corrected 2026-10-27) | Calls through a table at +0x40 with four arguments from +0x48..+0x54; the fifth load reuses the table's register in the target and this spends an extra one. |
| func_8003AB44 | unk_72A8 | **54/54 words, 9 DIFF** | A 3-cycle register rotation, source in `docs/near-misses/func_8003AB44.c`. Was 52/54 until argument 6 was declared `s16` instead of `s32`-with-a-cast (see the narrow-parameter entry in matching-levers section 7). Landing it means narrowing that argument in the committed prototype, which changes func_8003AAC4's call setup -- check that caller or use an `__asm__` alias. |
| func_8005500C | unk_2879C | **55/55 words, 33 DIFF** | Sparse `switch` on `& 0xF000`. One BRANCH-ARMS difference (the 0x9000 compare falls through into the shared body in the target, branches to it here) plus a 3-way register rotation. Source and everything measured against it in `docs/near-misses/func_8005500C.c`; `search.py` found nothing. |
| func_80059DAC | unk_2CDC0 | **58/58 words, 8 DIFF** | A bubble sort over D_800869C0. Four structural things were needed to reach the exact count and they pull against each other -- separate base locals per loop, the second materialised INSIDE the loop body so it lands after the `blez` guard, the index written first, and the comparison round the target's way; all four are written up in `docs/near-misses/func_80059DAC.c`. What is left is loop.c declining to make `i * 8 + base` an outer giv. The function makes no calls, so `loop_has_call` is false and the threshold is not halved. |
| func_80054104 | unk_264E0 | **LANDED** | It was the inner loop's FORM, not the subscript: written with `goto` it gets no NOTE_INSN_LOOP_BEG, so loop.c never strength-reduces `t[j]` or hoists `$sp + 0x10`, and the four extra words go away. `j = 0` first in the outer body supplies the back edge's delay slot. See the goto-loop lever in matching-levers section 9. |
| func_80055D84 | unk_2879C | **58/58 words, 5 DIFF** | A two-register swap around the final `or`: the target reuses the mask's own register as the destination and this source does not, and --greg shows why (the mask is allocated first on priority and takes the lowest free register instead of preferring the argument register). Getting to the exact count needed the cross-jump lever above; everything measured is in `docs/near-misses/func_80055D84.c`. |
| func_8004DE88 | unk_1C540 | **MATCHED and landed 2026-10-27** | Command-buffer builder, and the park was chasing a ghost. ONE walked `u8 *p` for buffer and cursor (`p = D_80083E18; *p++ = 8; strcpy((char *) p, s)`) makes the cursor after strrchr the SAME pseudo as the buffer pointer, so it is not born from a copy of $v0, has no local-alloc suggestion, and the `addu $a0, $v0, $zero` is simply there -- no allocation contest to win. The park ALSO measured a word short because a standalone file does not inherit the unit's `.size D_80081550,4`; verify.py and advise.py now warn about that. Lever in `docs/matching-levers.md` section 6. |
| func_8004DF8C | unk_1C540 | **MATCHED and landed 2026-10-27** | func_8004DE88's sibling. Two shapes, one word each: ONE `u8 *p` serves buffer and cursor, advanced in place (`p++` invalidates cse's register-equals-symbol class, so strrchr's argument is re-lowered from the surviving high, and the reused pseudo keeps its callee-saved register so the `addu` copy after the call survives); and the final stores go through the address pointer as `q[-1]`/`q[0]` with BOTH symbols scalar, where cse rewrites the bare `q[0]` to the symbol but combine cannot fold `q - 4` because the pointer has other uses. Both levers are in `docs/matching-levers.md`, and the `la` claim in the $gp-relative pattern above is corrected there and here. |
| func_80036B10 | unk_72A8 | **LANDED** | The stack-parameter prologue interleave, and it was D_80082BC0 declared as an incomplete array and passed by name instead of `&` on a scalar pointer. See the SOLVED section above. |
| func_8003931C | unk_72A8 | **85/85 words, 10 DIFF** | func_80039470's sibling and the same three shapes (one-statement cursor read, error blocks as `else` arms not guard clauses, pointer-local table base). What is left is a two-register swap with the margin measured: the opcode byte is 4 refs/23 insns (0.348) against the +0x80 pointer's 6/32 (0.375), and ONE more reference on the byte would flip it -- see `docs/near-misses/func_8003931C.c` for why none is available free. |
| func_80036D4C | unk_72A8 | **LANDED** | The stack-parameter prologue interleave, and it was D_80082BC0 declared as an incomplete array and passed by name instead of `&` on a scalar pointer. See the SOLVED section above. |
| func_8004CAA0 | unk_1C540 | **NOT a C function as written** | 18 words, and the cheapest-looking row in the table until you read it: it saves `$sp` into the PSX scratchpad at 0x1F8003FC, switches `$sp` there, calls func_8004C36C and restores. Plain C cannot assign `$sp`, so it wants inline asm or a decomp device like func_8003E758's. `triage.py` now reports this shape as STACK-SWITCH rather than EASY. |
| func_8004CECC | unk_1C540 | **102/102 words, 48 DIFF** | HSV->RGB. Every opcode, immediate, displacement, branch offset and delay slot matches, and the `beqz`->end delta agrees, so all six switch arms are byte-correct; the whole gap is the `v` parameter's entry copy `addu $v1,$a2,$zero`, and the 101 DIFF is that one-word shift rather than 101 wrong words. Diagnosed to the register from the dumps in `docs/near-misses/func_8004CECC.c`, which lists the eight shapes and the whole `search.py` sweep already measured against it and names the signature to aim at: the 0x80808081 magic belongs in $t0, not $a3, because the colour block had one fewer long-lived block-local than this source gives it. Landing it also needs jtbl_8002E5F8 placed. |

Cleared from this table on 2026-10-23, all of them by a DECLARATION rather
than a source shape: func_8004AC18, func_8005845C, func_80053DA8,
func_80053DCC, func_80053DF0, func_8004B4DC, func_8004EE90, func_8004F52C and
the four func_8005C8xx readers. The lesson is the one the near-miss README
already states -- the measurements held, the context around them moved -- and
here the context was always which declaration cc1psx compiled the symbol
against.

## The $gp group: no longer deferred

About 33 of the carved stubs reach a global `%gp_rel`, which these units' forced
gas `-G0` cannot produce -- but the bare file-scope size record can:

    __asm__(".sdata\n\t.size D_80081550,4\n\t.size D_80081554,4\n\t.text");

It puts each symbol in maspsx's `sdata_entries` while DEFINING nothing, so
`.sdata` stays 0 bytes, the symbols stay undefined, the linker script's own
assignments resolve them, and maspsx rewrites every load and store of them in
that unit to `%gp_rel`. The block is at the top of `csrc/unk_1C540.c`;
`csrc/unk_11F0C.c` has the original write-up of why it works.

Three things to know before adding a symbol to it:

* **It changes every access to that symbol in the WHOLE unit**, landed
  functions included, so `grep csrc/<unit>.c` first. As of this sweep none of
  the $gp symbols had a landed user, which is why the block could go in at
  once; that will not stay true.
* **Address materialisation stays absolute.** D_80080B18 is deliberately not in
  the block: func_80048DA0 indexes it through a materialised base, and a
  `%gp_rel` rewrite would not touch that anyway.
* **`verify.py` compiles a standalone file**, so a candidate has to carry a
  copy of the `__asm__` block to be verified, and the landed body must NOT --
  the unit already has it. A corollary that cost a round: once such a
  candidate is in a `sweep.py` batch file, `sweep.py verify` will report it
  WRONG, because the batch body has no block. Verify it in a scratch file with
  the block, then land the body without one and let `make check` be the check.

func_8004DCE0, func_8004DDB4, func_80050CAC, func_8004AE98 and func_80048DA0
all matched first try once the block was in place.

**unk_72A8 and unk_1AF38 have their own blocks now** -- unk_72A8's carries
D_80080A6C, D_80081440, D_80081448, D_80081460, D_800809B8 and D_800809BC, and
unk_1AF38's D_8008152C, D_80081528 and (added 2026-10-24, for func_80048660)
the three halfword atlas cursors D_80080B10/12/14 plus D_80081530. Before
adding a symbol, check BOTH the unit's `.c` and every listing in
`asm/nonmatchings/<unit>/` for an absolute `%hi`/`%lo` reference to it: that is
what makes D_8008099C impossible here, and the grep is two commands:

    grep -l "gp_rel(SYM)" asm/nonmatchings/<unit>/*.s
    grep -l "%hi(SYM)"    asm/nonmatchings/<unit>/*.s

The remaining $gp stubs in unk_72A8 are func_8003C920, func_80035484 and
func_80035888 (plus the two blocked on D_8008099C); unk_1AF38 has none left.

* `func_8003800C` (unk_72A8, 71w) -- 70/71, 29 DIFF, parked in
  `docs/near-misses/`. One word: the second `lw 0x80` of the owner pointer in
  the guarded block. greg collapses the two reads into one register because
  sched1 separates their live ranges; sixteen statement-order placements
  measured, all 70 words.

| func_80035E58 | unk_72A8 | **129/129 words, 2 DIFF** | The six-argument spawn, and the fourth member of the prologue-interleave family: the D_80082BC0 array spelling took it from 8 DIFF to 2. What is left is ONE swap -- func_80036988 two argument setups land in each other load-delay slots. They tie on sched2 priority, and `rank_for_schedule` last key sorts DESCENDING by LUID while expand_call emits $a0 setup first (calls.c:1850), so a0-later is unreachable through argument order; `e + 0xC` does not lengthen its priority path because fold rewrites it back to `n + 0x2C`. `docs/near-misses/func_80035E58.c`. |

| func_8004951C | unk_1C540 | **156/157 words, 97 DIFF** | The party status label. Structure verified and four spellings paid for: the D_80073414 base ASSIGNED rather than initialised (an initialiser is evaluated at block entry and lands ahead of the memset), D_80082E6A through an incomplete-array alias, an `s32` local for the `& 0x1F` value so combine emits `slti` instead of `sltiu`, and the 4 arm written out in full so cross-jumping merges its call pair with the kind-1 arm. Two words left, both from the kind-3 arm falling through into the shared `kind == 1` test where the listing reaches the call directly. `docs/near-misses/func_8004951C.c`. |

| func_80059814 | unk_2CDC0 | **MATCHED and landed 2026-10-27** | The +0x10 accumulator advance, ten case labels in six bodies. One shape bought all nine words and all 25 register differences: each body stores the mask straight into +0x40 and then READS +0x10 AGAIN for the shift, instead of holding the sum in a local. The re-read survives cse (`invalidate_memory` drops every pointer-based load after any pointer store, with no offset check) and pins the first `sw 0x10`, which sched1 can then no longer sink to the end of the arm for jump2 to cross-join. The old row's sched1 `potential_hazard` diagnosis was WRONG -- the dependence graph differed, not the scheduler's choice. Lever in `docs/matching-levers.md`. |

| func_8005A608 | unk_2CDC0 | **92/92 words, 11 DIFF** | Was 88/92 with 85 DIFF. The four words came from an `s32` index parameter RE-ASSIGNED its own truncation -- `j = id; id = (s16) id;` -- which forces the raw's copy and invalidates cse's equivalence so the else arm recomputes `(s16) i` instead of forwarding the entry's extension. What is left is one register chain: the extension's intermediate takes $v0 rather than being in place in $a1, which pushes the entry's -1 to $a2, which lets reload's find_equiv_reg copy it instead of materialising fresh. `docs/near-misses/func_8005A608.c`. |

| func_80052A90 | unk_25944 | **LANDED** | The Shift-JIS cell blitter. The last word was the FLAG: it is 32 bits wide and tested through a NARROW local, `u8 t = ok;`, which cse's `insert` does not see as a register copy (it looks for a REG of the same MODE in the source's class), so the quantities never merge and MIPS emits the QImode move as the listing's `addu $v0, $s3, $zero`. `s8` and `u16` work the same. `u8 ok` gives `andi 0xFF`, `u32 ok` tested directly drops the word, and a same-width `t = ok` is copy-propagated away. The other four shapes are on the function. |
| func_80037E58 | unk_72A8 | **MATCHED and landed 2026-10-27** | Both of its last two findings were DECLARATIONS, and `search.py --only=global_decl` found each in one pass: `extern u8 D_800809C0s __asm__("D_800809C0");` with `&` on the use (six words -- the park had asked for "D_800809C0's address one insn rather than two" without naming the declaration that does it) and then `extern u8 D_80012D6E;` for the last two, which had been written up as an unreachable sched2 choice inside priority 1. The alias is needed because func_80037D9C in the same unit is matched against the array spelling. The rest -- the descending loop's 0x48 displacement, the `q` asymmetry, the shared load in the last pair -- is in the function's header comment. |
| func_8005A778 | unk_2CDC0 | **101/101 words, 21 DIFF** | func_8005A608 with a third argument and a bound check, on the same lever, plus TWO locals for `p` (the listing's `addu $s3, $s4, $zero`) and `e->unk_10 = h` placed after `e->unk_08` so sched2 fills the `lhu` load-delay slot with it. The 21 words are the same register chain as func_8005A608's. `docs/near-misses/func_8005A778.c`. |
| func_80038F48 | unk_72A8 | **98/98 words, 12 DIFF** | The script VM's control-flow dispatcher. Word count exact on the first compile; everything structural is verified and written up, including the `(u8) (op + 3) < 3 || op == 0` loop condition, the if/else-if chain over -3/-2/-1, the +0x80 table read into a local so 0x48 stays a displacement, and the cross-jumped tail the 0xFE and 0xFF arms share. The 12 words are all inside the 0xFF arm: the listing interleaves the four byte loads with the shifts and reuses $v1 for the first and last byte, where this source issues all four loads first and spends four registers. `docs/near-misses/func_80038F48.c`. |
| func_8004E6F4 | unk_1C540 | **MATCHED and landed 2026-10-27** | The 15-bit colour-ramp builder. Every opcode, immediate and displacement in both arms and the shared tail is paid for, including `w[i - 1]` so the weight giv IS reduced (two adds, benefit 4) and three combined values rather than six product temps (six spill, 116 words). The two words are ONE register: the listing reuses $v1 for the weight and then the fifth product, which frees $t9 for `a`; this source keeps them apart, needs thirteen homes and pushes `a` into $s0. `docs/near-misses/func_8004E6F4.c`. |
| func_800500B4 | unk_1C540 | **139/139 words, 96 DIFF** | The dialogue-window placer. Word count exact and the structure verified: a `goto` loop so loop.c does not bias the two walking pointers onto their first access, the -0x28 attached to `b` rather than the sum, `pp[8]` for the third store so the bias cannot settle, and the counter store through a pointer (`addiu $t0, $a2, 0xA4` in the delay slot). The 96 words are one register cascade: `a` takes $a2 where the listing has $a1 and shifts it IN PLACE, which is what lets sched2 fill the `lw 0x20($a0)` delay slot. `docs/near-misses/func_800500B4.c`. |

## advise.py sweep of every park (run after the 2026-10-27 landings)

`advise.py` on each `docs/near-misses/*.c` as committed. The LEVER column
lists only the categories that name a MEASURED lever -- DELAY-NOPS,
BRANCH-ARMS, CALLEE-SAVED, COMMUTED, SIGNEDNESS, IMMEDIATE and OPCODE; the
EXTRA/MISSING/LENGTH/ALLOCATION rows appear on nearly everything and are
left out. REGS is advise.py's count of register-only differences, which is
what remains once every shape finding is paid for.

Where to start: a row with a LEVER and a small REGS is one experiment away.
A row with no LEVER and a large REGS is an allocation-order problem and
wants `--greg` before any source change.

| func_8004C36C | unk_1C540 | **PARKED 2026-10-27 at 464 of 461 words, 171 DIFF** | 461 words, frame 0x158, the GTE model draw, and the third of the three giants that had never been read. Source in `docs/near-misses/func_8004C36C.c`. THE STACK OBJECT IS REACHED THROUGH A POINTER LOCAL (`Unk4C36C pp;` with `Unk4C36C *p = &pp;`), which is the listing's `addiu $s1, $sp, 0x18` and one base register for all 0x110 bytes; accessed directly as `pp.unk_NN` gcc addresses every field off `$sp` and 436 words differ. Every node field is RE-DEREFERENCED rather than cached (a `q` local is twenty words short), and the third element word goes through a local so the store reaches the `beqz`'s delay slot. The GTE operations needed new macros in `include/gte_inline.h` (`gte_ldv3`, `gte_rtpt`, `gte_rtps`, `gte_stsxy3`, `gte_stsxy`), with RTPT/RTPS read out of this listing; psyq's inline_c.h spells those two as `.word 0x000000bf`/`0x0000007f` and assembles SILENTLY to the wrong bytes under modern gas. What is left is three `nop`s in the primitive-field block, where the listing fills each store's load-delay slot with the next statement's load: those loads are of the stack object and the stores go through `prim`, and both are QImode, which is the one case true_dependence's MEM_IN_STRUCT_P exception cannot reach. |
| func_800345B8 | unk_72A8 | **PARKED 2026-10-27 at 546 of 546 words, 128 DIFF** | 546 words, frame 0x58, the model/billboard draw with a wobble. Written straight out of func_80033D54's park the same day -- it is the SAME FAMILY and most of the shapes transferred unchanged, which is why a first attempt reached the exact word count. Source in `docs/near-misses/func_800345B8.c`. What is left is TWO ADJACENT PAIRS in global-alloc's priority order and the register names are nearly all of the 128 words: the +1 giv (63 refs / 313 live) needs ONE more reference to cross floor_log2's step at 64 and take $s2 from `s`, and `m2` (7 refs / 459 live) needs 11 references or a 351 live range to take $s6 from `y`. The target spills ONE value (`y`, as a HImode sh/lhu pair) where this build spills two, which is also why the frames differ (0x58 against 0x60). The four shapes that moved it most: the flag byte must be `e->unk_01` and not `q[0]`, or the giv group's representative becomes e+4 and a third pointer register appears; D_8006CD4C/D_8006CD10/D_8006A84C are INCOMPLETE ARRAYS indexed `[0]`; s->unk_20 and s->unk_22 go through locals because each is stored twice; and the flag byte's plain store is unconditional with only the OR in the arm. |
| func_80056E10 | unk_2879C | SURVEYED, never attempted | 718 words, frame 0x90, 47 blocks -- the largest stub left, and the third member of the func_8005BBFC / func_8005B4D0 voice family (both parked at 220 and 367 DIFF, so expect this one to be hard for the same reasons). A `u8` first parameter spilled to 0x30($sp) and another to 0x31($sp), i.e. the func_80057D20 register-pressure shape. Walks D_80085458 (0x78-byte entries) and D_80084960 (0x9C-byte entries, indexed by a 156-multiply), with D_80086A40 + D_80086D50 * 0x1C as a third table, and counts live voices in an opening loop over the entry's +0x00 halfword. Calls func_8005E4A0, func_8005E78C and func_8005EB78 four times each, func_8005E97C and func_80056DB4 twice, func_8005EC0C, func_800565D8, func_800561D8 and func_80055E74 once. |

| function | built/target | verdict | lever named | regs |
|---|---|---|---|---|
| func_80035E58 | 129/129 | SHAPE | OPCODE | - |
| func_80037E58 | 70/70 | SHAPE | -- | 6 |
| func_8003800C | 70/71 | LENGTH | -- | 1 |
| func_80038CB8 | 164/164 | SHAPE | OPCODE | 8 |
| func_80038F48 | 98/98 | SHAPE | OPCODE | 6 |
| func_8003931C | 85/85 | ALLOC | -- | 10 |
| func_800398BC | 76/76 | SHAPE | EXTRA,MISSING,OPCODE | 22 |
| func_8003AB44 | 54/54 | SHAPE | IMMEDIATE | 7 |
| func_8003F368 | 113/113 | SHAPE | IMMEDIATE,OPCODE | 2 |
| func_800407C0 | 178/178 | ALLOC | -- | 62 |
| func_80041E70 | 270/270 | SHAPE | OPCODE | - |
| func_8004437C | 107/100 | LENGTH | OPCODE | 7 |
| func_80047054 | 58/58 | SHAPE | EXTRA,IMMEDIATE,MISSING | 19 |
| func_8004951C | 156/157 | LENGTH | EXTRA,LENGTH,MISSING,OPCODE | 3 |
| func_8004A330 | 74/77 | LENGTH | CALLEE-SAVED,EXTRA,IMMEDIATE,LENGTH,MISSING,OPCODE | 13 |
| func_8004AC3C | 129/128 | LENGTH | BRANCH-ARMS,DELAY-NOPS,EXTRA,IMMEDIATE,LENGTH,MISSING,OPCODE | 22 |
| func_8004CECC | 102/102 | SHAPE | EXTRA,MISSING | 44 |
| func_8004D294 | 114/114 | SHAPE | -- | 12 |
| func_800500B4 | 139/139 | SHAPE | DELAY-NOPS,OPCODE | 64 |
| func_80052A90 | 96/96 | SHAPE | OPCODE | - |
| func_80054B08 | 84/84 | SHAPE | EXTRA,IMMEDIATE,MISSING,OPCODE | 10 |
| func_8005500C | 55/55 | SHAPE | BRANCH-ARMS | 14 |
| func_80055D84 | 58/58 | ALLOC | -- | 5 |
| func_800565D8 | 31/31 | SHAPE | OPCODE | 1 | (the row's one shape finding has since been paid: 6 DIFF -> 3)
| func_80056654 | 107/107 | SHAPE | OPCODE | 11 |
| func_80057948 | 64/64 | SHAPE | IMMEDIATE | 7 |
| func_8005947C | 81/81 | SHAPE | OPCODE | 10 |
| func_800599B0 | 133/133 | ALLOC | -- | 1 |
| func_80059DAC | 58/58 | SHAPE | IMMEDIATE | 3 |
| func_8005A608 | 92/92 | SHAPE | DELAY-NOPS,OPCODE | 8 |
| func_8005A778 | 101/101 | SHAPE | DELAY-NOPS,IMMEDIATE,OPCODE | 13 |
| func_8005C2C0 | 132/132 | SHAPE | OPCODE | 1 |
| func_8005C4D0 | 64/64 | SHAPE | -- | 6 |
