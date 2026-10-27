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
  size of its own. `extern u32 D_X;` makes it emit `.extern D_X, 4`, `&D_X`
  becomes a single `la`, and maspsx rewrites that to $gp-relative as well, so
  a function that writes the ADDRESS out as data loses a word per
  materialisation. `extern u32 D_X[];` with `D_X[0] = v` emits no size, keeps
  the address as lui/addiu and still gets the one-word store. Worth 2 words on
  func_8004DF8C, which needs both forms of the same symbol at once.

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

| func_8004A330 | unk_1C540 | **76/77 words, 43 DIFF** | Was 70/77. Four of the seven words came from declaring arguments 5 and 6 `s16` rather than `s32` -- advise.py's CALLEE-SAVED row applied literally, and the same lever that landed func_8004A24C. As `s32` each parameter's pseudo is a plain copy of its incoming MEM, so `update_equiv_regs` substitutes it back and the `lw` sinks to its single use; as `s16` the entry insn is a PROMOTION, the substitution cannot fire, and both live in callee-saved registers across the two calls. The listing's two `lw` at ENTRY are the signature. What is left is three words on `m`: the listing spends a callee-saved register and a copy on `n + 0x20` where this puts it straight into $a0, which it prefers as the callee's first argument; `m` crosses no call, so only a lower-priority conflicting allocno preferring $a0 would move it. The earlier reading that the 0x20 folds into the displacements was wrong -- `m` is a VAR_DECL and nothing folds. |
| func_80054B08 | unk_264E0 | **84/84 words, 32 DIFF** | Was 82/84. Both missing words are paid for: the three stores to +0x10 and the three to +0x14 are VOLATILE -- the listing writes each field three times with a refined value and never reads it, so flow deletes the first two of each as dead, and no non-volatile spelling keeps them -- and the -0x20/+0x20 go in their own statements so fold cannot move the constant onto `base`. What is left is 32 words of schedule: the target emits the block in source order while this hoists two record loads and sinks the three byte/halfword stores. `docs/near-misses/func_80054B08.c`. |
| func_800398BC | unk_72A8 | **76/76 words, 50 DIFF** | First function worked from an m2c draft -- 75 of 76 on the first compile, then two documented fixes, and all branch spans match. The missing word is the D_800721C0 index scaling: the target spends `srl`/`andi 0x1F`/`sll 2` where gcc folds the mask and the scale into `srl 0x11`/`andi 0x7C`. Three spellings measured against it in `docs/near-misses/func_800398BC.c`. |

## Parked: tried and not matching

Each of these has the right instruction set and word count with the registers
or the order wrong, so they want the dumps rather than another guess.

| function | unit | state | tried |
|----------|------|-------|-------|
| func_800368D0 + func_80035C80 + func_80035CE4 + func_80036900 | unk_72A8 | **LANDED** | All four. The +0x44 pointer takes a local (its `lw` then precedes the `lh`, sched1 tie-breaking on RTL order) and the +0x4A index stays INSIDE the subscript: given its own local it is a second quantity with the shorter live range, so local-alloc gives it $v0 and the pointer $v1, the reverse of all four listings. func_80036F24 is where that spelling was found. |
| func_80033AE8 | unk_72A8 | 17/17 words, 6 DIFF | The bitmap CLEAR; func_80033AA8 and func_80033B2C both landed from the same shape (the constant base assigned after the guards, the index inside the addressing expression). This one differs only in that the raw argument and the constant 1 hold each other's registers: the `nor` makes sched put the `sllv` ahead of the `lw`, which shortens the constant's live range by one insn and flips the two past each other in local-alloc's priority. A `u16` parameter matches it exactly -- but then func_80039258's `func_80033AE8((s16) v)` loses the `sll`/`sra` pair the target has there, so the parameter is at least 32 bits wide and this is NOT the spelling. A local for the mask (17 DIFF) and a local for the whole address (17) are both worse. |
| func_8004AFE8 | unk_1C540 | 14 DIFF | Counts entries whose `(*p)[1]` equals the first one's, with the loop's `p != 0` test at the bottom and a guard at the top. |
| func_800589B8 | unk_2879C | 19/19 words, 2 DIFF | The byte reader every func_80058xxx call goes through: `off = *(s32 *) q; c = *(u8 *)(D_80085FA4[0] + off); off++; *(s32 *) q = off; if (D_800869B0[0] < (u32) off) { *(s32 *)(q + 0x2C) = 1; return -1; } return c;`, with both globals read into locals so each load's delay slot gets an independent insn (18 and 19 DIFF without that). What is left is that the target's two first loads are in the other order: `lw %lo(D_80085FA4)` then `lw 0x0($a1)` where this has the dereference first. cc1psx's fill pass swaps a load with the next insn, so the pre-fill order must have been the dereference FIRST -- and sched1 puts the symbol load first whatever the source order (measured: both statement orders, `off + base`, a `u8 *` base, s32 and u32 offsets). |
| func_80058A04 | unk_2879C | **LANDED** | The 7-bit varint decoder. The three missing words were ONE variable for the byte, reused by the loop instead of a second variable for the loop's: that gives the allocno a live range where the block-local `c << 7` quantity holds $v0, so it cannot have the call's own return register and both `addu $a0, $v0, $zero` copies become real instructions. This is the conflict lever, not a shape. The two-variable spellings measured before -- a copy between them, a `u8` byte, a nested `if`, a `while (1)` -- are recorded in the comment on the function. |
| func_8004D690 | unk_1C540 | 29/31 words, 18 DIFF | `while (1) { if (*a != 0) { (*(void (**)(u8 *)) a)(a + 4); return; } if (*b == 0) return; func_8004D7E8(c); }` -- every instruction right, and two words SHORTER than the target: gcc moves the indirect-call block to the end of the function, where it falls into the epilogue, while the target leaves it first and spends `j` plus a `nop` to reach it. `break` instead of `return`, the two arms as if/else, and peeling the first test (32 words, cross-jumping does not merge the two call sites back) all leave it there. |
| func_8003C67C | unk_72A8 | 31/31 words, 28 DIFF | Moves D_8006B200s[i] to the front, shifting the rest up through an 8-byte stack temp. The word count is now right: written as STRUCT assignments on the `Unk8006B200 D_8006B200s[]` alias (`Unk8006B200 t = D_8006B200s[i]; ... D_8006B200s[i] = D_8006B200s[i - 1];`) the block moves cost exactly what the target spends. What is left is that loop.c still strength-reduces both addresses into pointers walked by -8, where the target recomputes `sll $a1, 3` for the destination and `sll $a1, 3` for the source every iteration with the base in $a2. The index-off-the-symbol form that unparked func_80056408 and func_8005A1D0 does NOT work here, and the difference is probably that those two loops contain CALLS: gcc's loop.c halves the strength-reduction threshold when `loop_has_call`, so a call-free copy loop is exactly the case it always reduces. Also measured: `while` with `i--`, a `for`, `p[i] = p[--i]`, a separate `j = i - 1`, field-by-field copies, and a `(u32)`-cast index. |
| func_800565D8 | unk_2879C | **31/31 words, 3 DIFF** | The two-sided scale. Both bytes are VOLATILE -- the target loads each one twice, for the zero test and for the multiply, and cse forwards the tested byte in every non-volatile spelling; that is where the old 30/31 came from. What is left is two sites, and both are the same question: which of two equal-priority chains sched1 ran first. Full write-up and the fourteen spellings measured in `docs/near-misses/func_800565D8.c`. | SINCE PARKED: `d = n - 0x40` into a local before the `p[0x20]` test, so reorg fills the branch's delay slot from the insn before it rather than from the shared `return 0` -- 6 DIFF to 3. The three left are the `* 2` binding to the byte rather than to `0x3F - n`.
| func_8004E9EC | unk_1C540 | 33/33 words, 27 DIFF | The template fill: `while (i <= n * 2) { if (*q == 0) { while (i < n * 2) { three s8 reads of D_80080B70 into locals, three stores, i += 2, q += 2; } r = 1; } i++; q++; }`, with `n * 2` re-evaluated in both conditions (the listing recomputes `sll $v1, $t2, 1` each outer iteration) and the three bytes read into locals before the stores (all three `lb`s precede all three `sb`s). D_80080B70 is read SIGNED through an alias. What is left is registers: the target copies the count out of $a1 so the result can live there, keeps only `%hi(D_80080B70)` outside the inner loop and re-materialises the `%lo` inside it, and initialises the counter and the cursor in the preheader; this keeps the count in $a1, hoists the whole address, and puts the counter's init ahead of the guard. |
| func_8003A3E8 + func_8003A374 | unk_72A8 | 32/32 words, 15 DIFF | Two bytecode opcodes that read a byte, a table index, then a little-endian 32-bit operand from the cursor, and store a word THROUGH A BYTE they load from `operand + table[index] * 4` -- `*(u32 *)(u32) *(u8 *)(...)`, which is what the `lbu`/`sw` pair at the end is (the game's own bug or a deliberate low-address write; either way that is the C). The statement order is pinned: the first byte and its increment, then the state pointer at +0x80, then the table lookup through `**q` WITHOUT incrementing, then the second increment, then the four operand bytes, then `*q += 4` (26 DIFF with the lookup after the operand). What is left is which registers the four byte reads and the accumulator use -- the target accumulates in $v0 where this uses $a1, with the `sll` of the table value interleaved differently. |
| func_8004CAA0 | unk_1C540 | not attempted | Switches `$sp` to the scratchpad at 0x1F8003FC around a call, saving the old stack pointer at that address. No C expression produces that; it wants inline asm, like func_8003E758's. |
| func_80036F24 + func_80037090 + func_80037394 | unk_72A8 | **LANDED** | The counter at +0x64 is VOLATILE: the target re-reads it after the increment and no non-volatile spelling stops cse forwarding the stored value (five measured, `MEM_IN_STRUCT_P` refuted). With `*(volatile u16 *)` on the re-read plus the pointer local above, all three match. func_80037394 is the same body with six copies instead of three. |
| func_8004D75C | unk_1C540 | 19 DIFF | A 0x44-byte block move (the odd word first, then sixteen words in a four-word loop -- so the shape is right) followed by `*(u32 *) s = 0;`. |
| func_80048FBC | unk_1C540 | 18/18 words, 7 DIFF | `p[func_80048F8C(p, n)] = func_8004B404(0x104);` -- both arguments of the first call pass through untouched, so neither move is emitted. Only which of `$s0`/`$s1` holds the pointer and which the index chain. Measured against local-alloc's own arithmetic: the chain (the result copy, the `sll` and the `addu`, all one quantity) has six references over four insns and the pointer two over nine, so the chain outranks the pointer and takes $s0 -- the target has it the other way round, which no spelling reaches. `p += f(...)` and a separate `q = p + i` both move the arithmetic ahead of the second call instead (16 DIFF). |
| func_80056C30 | unk_2879C | **LANDED** | The fourth fact was the one slot: `i++` goes AFTER func_8005E97C, not between the two calls. An argument setup cannot be scheduled earlier than its chain position, but giv updates CAN be hoisted across it and across the call, writing callee-saved registers the call does not clobber -- so put the increment after the call and let sched pull the updates forward. Found by reading the sched2 dump with tools/decomp/sched.py. |
| func_80057948 | unk_2879C | **64/64 words, 9 DIFF** | The match-and-stop sweep. Four loop facts paid for, 69 words to 64, all the D_80085458 recipe. What is left is one register swap that `--greg` prices exactly: `m` (the OR accumulator, pseudo 83) and the D_80073740 giv (116) both have 7 refs, so the giv's shorter live range (28 against 36) wins it $s2 where the target has `m` there. 22.2% margin, and `m`'s live range has to span the guard, so the only lever left is ONE more reference to `m` that costs no instruction. `docs/near-misses/func_80057948.c`. |
| func_8005C4D0 | unk_2CDC0 | **64/64 words, 22 DIFF** | The D_80085458 teardown. Four things paid for, 62 words to 64: BOTH tails out of line with the FAIL tail first, which needs labels and gotos; the +0x1A flag read into a local BEFORE the D_80085F98 store, so the store can reach the `beqz`'s delay slot (advise.py's DELAY-NOPS advice, two words); a pointer local for each of the two table bases; and func_80056D44 called with THREE arguments even though its own listing never reads $a1 or $a2 -- the two- and one-argument forms are both a word short, because the third is what makes gcc keep `e` in a register and copy it. What is left is 22 words of prologue allocation following from `e` landing in $a1 rather than $a2. `docs/near-misses/func_8005C4D0.c`. |
| func_8005B070 + func_8005B16C | unk_1C540 | **LANDED** | Both. The fourth fact, after the three loop-recipe ones, is that the stack primitive is a STRUCT (`Unk5F134SP`) and not `u8 buf[0x40]` with casts: struct accesses carry MEM_IN_STRUCT_P, every varying-address load then depends on them, and the body comes out in strict source order with both load-delay slots empty. The store order matters with it -- +0x04, then +0x00 from the table, then the two halfwords -- or the table load moves two slots earlier and takes a second register. |
| func_8004AC3C | unk_1C540 | **128/128 words, 90 DIFF** | The record resolver. The STRUCTURE is verified and is what the file is for: labels and gotos for the two shared blocks, the func_80049374 family's `n = p[0] * 4` / `* 2` with one shared `(n + p[0]) * 4`, `p[0] * 20` spelled out in the two arms whose final offsets differ, a RAW CONSTANT `(u8 *) 0x80010324` base for mode 1's 0x13 case, and one `return` at the single exit. What is left is that the target re-reads `p[1]` at the head of each of the four blocks that index D_80073414 while cse forwards it here -- only one of the four has a store to explain it. `docs/near-misses/func_8004AC3C.c`. |
| func_8004D294 | unk_1C540 | **114/114 words, 27 DIFF** | Three identical nine-word Euler-angle wraps. The eight words missing from the m2c draft were a SECOND func_8004D1EC call -- m2c truncated before it -- plus the $s2 save that only exists because the third parameter is used on both sides of the first call. D_80083178 is a `u8 *` base with byte displacements, since 0x140 is past the end of its struct, and the caller's vector is read `lhu` for the masks and `lh` for the difference at the same offset. What is left is ONE register choice replicated three times: which of the angle and the sign-extended reference gets $v0, which also decides where the spare `lhu` lands. Five spellings of the subtraction measured, all 27. `docs/near-misses/func_8004D294.c`. |
| func_800550E8 | unk_2879C | **119/120 words, 45 DIFF** | The turn-commit. Five things paid for, 122 words down to 119: the three-way store as a `switch` with `case 0x9000` FIRST (a two-case switch emits `beq`/`bne` and falls through to the body written first, which is the listing's shape -- four if-else orderings measured at 48-51 DIFF); the two common stores hoisted above the three-way test; `func_8005537C` called twice and cross-joined rather than once on a joined value; one `u8 *` local serving both tables; and `func_800552C8` called with no argument. One word left -- the target re-materialises the D_800847D0 base after the `|= 0x100` store and gcc deletes any redundant copy -- plus the allocation in the two lookups. `docs/near-misses/func_800550E8.c`. |
| func_80056654 | unk_2879C | **107/107 words, 21 DIFF** | Builds one sprite primitive in D_80084918 from the D_80084960 entry the +0x06 index selects. Five things paid for, 116 words down to 107: `func_800565D8` declared returning `s32` here though it really returns a sign-extended `s16` (the listing adds $v0 straight in); D_80084918's base a `u8 *` local assigned INSIDE the `if` and after the call, or it takes $s2 and its save; the +0x16 store through that local rather than `D_80084918 + 0x16`, which otherwise biases the shared base; the D_80073740 lookup into a local read early and stored late; and `x` refined in place with the shift term first. What is left is 21 words of schedule in the primitive-build block. `docs/near-misses/func_80056654.c`. |
| func_80057D20 | unk_2879C | **463/463 words, 6 DIFF** | The controller-change applier, 126 controllers through jtbl_80032F04 with 25 bodies. Every opcode, immediate, displacement AND register matches; the six words are the ORDER of two giv initialisations and their increments in case 64's inner loop, which follows loop.c emitting givs in reverse creation order and so wants the D_80073740 reference to precede the D_80085458 one -- it cannot, since three conditions read through the latter first. The 48 words before that came from THE SEVEN LOOP COUNTERS BEING SEVEN VARIABLES: one shared `i` is an allocno of 46 refs over 155 live insns (priority 1.484) that outranks the entry pointer and `d` and takes their registers, where four separate case-1 counters at 7/17 (0.824) leave $s1 to `e` and $s4 to `d`. Other shapes: `func_80055E7C` takes THREE arguments everywhere and is unprototyped -- the `addiu $a3, $zero, 1` is reload's register for the `f1 = 1` store, which case 12 proves by having it with no call; `d << 1` never `d * 2`; case 1 reads back `e->unk_40` and `e->unk_44` just stored, which is what makes the else arm's divide `divu`. Third of three on the 0x80032E14 run. `docs/near-misses/func_80057D20.c`. |
| func_8005914C | unk_2879C | **205/204 words, 131 DIFF** | The MIDI channel-message dispatcher, 204 words, seven status nibbles in a comparison tree with the 0xB0 controller change holding 128 of them and all three loops. Every word outside the first two loops matches. Five shapes paid for: the body order 0x80, 0x90, 0xA0, 0xB0, 0xE0, 0xC0, 0xD0 (read off the body addresses, since the tree order says nothing); `u8` parameters, which is what puts an `andi 0xFF` at each of the fourteen uses; the two save/restore loops as WHILE loops, so `duplicate_loop_exit_test` makes the guard and the bottom test ONE source expression and they share the D_800869B4 address pseudo; the entry pointer as the inline indexed form, since a pointer local costs loop.c a `move` off the giv and a walked pointer costs a second biased giv; and the third loop's OWN counter rather than sharing the first two's, one word, by the one-variable-per-loop lever (a reused counter is one allocno whose refs sum across disjoint ranges). What is left is ONE word behind two load-delay nops: the listing's in-loop bound load sits mid-body with `i++` behind it, and sched cannot hoist it there because the load's base is a symbol and the stores go through the giv. Reading the bound into a local mid-body does move it and kills both nops, but then it is a second expression and the address `move` comes back. The guard-plus-do-while ordering that killed the same nops on func_80057D20 does NOT transfer: 208 words, because this listing materialises its `%hi` once per loop where that one does it twice. `docs/near-misses/func_8005914C.c`. |
| func_8005BBFC | unk_2CDC0 | **333/333 words, 220 DIFF** | SPU voice start, 333 words: resolves the program record for (bank, prog, key), waits for the voice to go idle, sums the sample-table lengths for the wave address, fills a `SpuVoiceAttr` on the stack and the 0x78-byte voice record, and calls func_8005EC0C. Word count exact and the logic settled; nine shapes paid for, including the channel as an `s32` local off the `s16` parameter (12 words), `<< 1` not `* 2` for both pan doublings, the two adsr halfwords through a local, `attr.volume.left` READ BACK so its copy fills the next load's delay, and the voice-record pointer reassigned per block so the field offsets stay displacements. What is left is one allocation tie-break -- the listing spills `prog` and keeps `key` in $fp, this source does the opposite -- plus the prologue placement of the two stack parameters and the order of the record stores. `docs/near-misses/func_8005BBFC.c`. |
| func_8005B4D0 | unk_2CDC0 | **379/376 words, 367 DIFF** | func_8005BBFC's sibling, the note-on that has to FIND a voice: counts the bank's live programs, walks the +0x20 record's layers, and for each layer whose key range matches scans for an idle hardware voice and then for a free D_80085458 entry. Everything from func_80056DB4 on is func_8005BBFC's block, so that park's shapes all apply. The new lever here, worth 10 words, is that both scan loops need an explicit `goto` rather than `while` or `for (;;)`: the listing has ONE copy of the exit test with the back edge jumping to it, and either loop form lets duplicate_loop_exit_test copy the test to the bottom. What is left is three words and five nops, and the six memory-resident locals whose frame slots have to match -- including two source locals holding the same pointer value, which is not yet explained. `docs/near-misses/func_8005B4D0.c`. |
| func_8004B954 | unk_1C540 | **283/290 words, 261 DIFF** | The ring builder: walks a circle of vertices around a sprite's bounding box and writes one GPU primitive per step, 0x1C bytes or 0x14 depending on the seventh argument. Two drafts are parked, wrong in opposite directions. The frame census is the whole diagnostic: the listing's vertex is a FOUR-BYTE object at 0x10 written as two halfwords and read as a WORD, with a hole at 0x14 and four 32-bit locals at 0x18..0x24. Two `s16` locals make the second one dead and slide every later local down by four; `s16 v[2]` with `*(s32 *) v` reads gets the layout EXACTLY right but is 307 words with one extra local. A DVECTOR struct assignment (315), an SVECTOR (308) and `*(s32 *) &cx` (259) all measured. The 283-word draft is seven short: three in the flag chain and three in each primitive loop, where it keeps the vertex in registers and the listing stores both halves every iteration. `docs/near-misses/func_8004B954.c`. |
| func_80056E10 | unk_2879C | NOT ATTEMPTED, 718 words | **The third member of the func_8005BBFC / func_8005B4D0 family** and the largest function left in the executable: the full note-on, with THREE fallback scans for a voice (by channel in D_80085458, then by idle state through func_8005EB78, then a third that accepts state 2) before the same voice setup block the other two share. Read docs/near-misses/func_8005BBFC.c and func_8005B4D0.c first -- every shape they list applies, and m2c's reading already shows the unrotated `goto` loops func_8005B4D0's park documents, three of them. Nothing attempted beyond that reading. |
| func_800345B8 | unk_72A8 | NOT ATTEMPTED, 546 words | The GTE transform pass, and the only remaining function that is not ordinary C: it writes its operands straight into the SCRATCHPAD at 0x1F800000, calls RotTransPers out of it, and feeds D_8006CD10 / D_8006A84C / D_8006CD4C. 80 stores, one loop. The scratchpad addressing is the unknown -- how the original spelled those absolute addresses decides most of the function -- so expect to settle that before anything else. Nothing attempted. |
| func_8005947C | unk_2879C | **81/81 words, 12 DIFF** | The sequencer event dispatch. Four shape findings paid for -- two variables for the byte (the raw s32 and a `u8` copy), a `u32` index so the fold comes out, `i = r` last in its arm, and the three special cases as a `switch` so the 0xFF body is laid out before the 0xF0 one. What is left is two coupled allocation facts: `b` in $a2 against the target's $a0, which also decides whether the tail's mask is in place and so whether reorg can hoist it, and `t`/`c` holding each other's callee-saved registers on a near-tie in global-alloc's priority. `docs/near-misses/func_8005947C.c`. |
| func_80059D1C + func_8005C818 | unk_1C540 | 16/16 words, 9 DIFF | Sweeps over D_80085FA8's 0x50-byte entries. func_80059D60, the third of this family, LANDED once the bound was read in the loop condition rather than hoisted into a local -- but these two STORE in the loop body, so loop-invariant motion cannot lift that read, and the in-loop re-read stands where the target has the hoisted copy. A guard plus a second read inside the `if` (cse's copy) is worse at 14/16. So the question is why gcc could hoist a load across stores whose base is the loop's own giv: that is an alias-analysis question, not a source-shape one. |
| func_80047738 + func_8004827C | unk_1AF38 | 17 and 12 DIFF | Both shapes are close; func_8004827C's target copies the parameter into `$v0` before using it, which no spelling here reproduces. |
| func_8005C2C0 | unk_2CDC0 | **132/132 words, 10 DIFF** | Keys a voice on: claims the D_80085458 entry for the channel, fills it, and calls func_8005EC0C with a stack `SpuVoiceAttr` (the real Psy-Q type -- the mask 0x1FFEF is SPU_VOICE_* minus PITCH, which is the second witness; `libspu.h` is now in common.h for it). EVERY instruction matches; what is left is where sched2 puts ten of them in the entry block. Four things paid for, 84 DIFF down to 10: `s16 j = a` (a TRUNCATION, so cse leaves the in-place `sll`/`sra` on the parameter -- `s32 j` is a copy and cse rewrites the shift to read it); a pointer VARIABLE `q = D_80085458` for the base, because the symbol inline is TREE_CONSTANT and fold swaps it right, expanding the index chain first and costing p its $v0; `st.volume.left` read BACK from the entry, so cse turns the load into the listing's reg-to-reg move; and the four constant bytes in channel-interleaved order. `docs/near-misses/func_8005C2C0.c`. |
| func_8005A90C | unk_2CDC0 | **99/103 words, 92 DIFF** | The third of the func_8005A608/func_8005A778 family, with the longest tail (func_80059F8C, func_8005ECA0 and func_8005EC40, each bailing out with -1); the tail is landed func_8005AAA8's word for word, including the `u8 *` cast for the +0x0C store. The dispatch, the search loop and every instruction of the body are in place, on the family's shared `j = id; id = id << 16; id = id >> 16;` lever plus `s16 i` and a single `k = i` at the join. What is left is FOUR words -- two callee-saved registers. The listing keeps six values live across the calls and this keeps four, merging `c` with the entry pointer and `i` with func_8005ECA0's result; the merge is reload's, since `c` is still live at `e->unk_08 = c` (a store through the entry pointer) and the copy that frees the register is func_80059F8C's own argument load moved ahead of the stores. NOT blocked on the other two, which are stuck on the `-1` constant's register instead. `docs/near-misses/func_8005A90C.c`. |
| func_80038CB8 | unk_72A8 | **164/164 words, 12 DIFF** | The tile-row emitter: three near-identical blocks walking the 2-byte records at +0x1C through func_8003AB44. EVERY instruction matches; 12 words of the second loop's register naming are left. Four shapes paid for: the record pointer is TWO locals, because the listing's condition reads the next record as `lb 0x2($v1)` off the OLD pointer while the loop top reads the new one (ten words); the FIRST loop's carried pointer is a READ-BACK of the field it just stored, which is the `addu $a0, $v0, $zero` in its delay slot, while the SECOND loop's is a plain `r + 2` with a nop -- the two loops differ; the two loops need SEPARATE locals for the carried pointer (the listing gives them $a0 and $v1, which one pseudo cannot do) but SHARE the old one; and `p[0x28]` is `*(s8 *)` throughout. `docs/near-misses/func_80038CB8.c`. |
| func_800599B0 | unk_2CDC0 | **133/133 words, 1 DIFF** | The sequencer start: picks the format from the first byte at D_80085FA4 and initialises all D_800869B4 D_80085FA8 track entries, one way for formats 0 and 1 and another for format 2. ONE word differs -- a `sw` reads $v0 where the listing reads $v1. 126 DIFF down to 1 on six things: `switch ((s32) D_800737C8[0])` with the cast, because unsigned gives one `sltiu` where the listing has gcc's signed `bltz`/`slti`/`beq` tree; both entry pointers INDEXED inside the body (walked, loop.c adds a second giv biased to +0x40 -- 40 words); format 2's length halfword indexed off a base read before the loop, which is what puts the giv's initialisation in the preheader; `f = D_800737C4[0]` read into a local BETWEEN the byte and word stores of the shared tail (advise.py's DELAY-NOPS lever); format 2's +0x04/+0x38/+0x1C spelled as READ-BACKS, which is where the listing's four reg-to-reg moves come from; and the +0x18 store sitting between +0x04 and +0x38, which is the word that gets it to 133. `docs/near-misses/func_800599B0.c`. |
| func_80037714 | unk_72A8 | **LANDED** | The two-quad actor draw. 355 DIFF to a match. The last seven words were ONE shape: the u and v running values are TWO locals, not one reused -- the two chains sit in different basic blocks and the listing gives them different registers, which one pseudo cannot have. Before that: the four RotTransPers4 screen-coord slots are 8 bytes apart (not `long sxy[4]`); `a = p + 0xC` is one pointer and cse is what makes `a[0]` print through the parameter; the ot index is INLINE in `AddPrim` (the opposite of func_800369C8 in the same unit, because here the add has a hard argument register to tie to); `g = &D_80083160` used in the second half only; and five levers now in matching-levers section 8 -- chained assignment, inline `|` for the four `||` terms, the front end narrowing `% C`, and the rest. |
| func_8005C668 | unk_2CDC0 | 11 DIFF | Builds a 0x40-byte block on the stack (a word from D_80073740, the constant 3, two halfwords) and passes its address. |
| func_800489F4 | unk_1AF38 | not measured | func_80047738 plus an `ori 0x20`; park it with that one. |
| func_80049E1C | unk_1C540 | 16-18/20 words, 18 DIFF | The clamp, func_80049DE8's family -- and func_80049DE8 LANDED once its negative arm was written as a conditional expression (`a = (a < 0) ? m + a : a;`), which is what pays for the copies through $v0 that a plain `if` optimises away. This one needs three or four more such copies than any of six shapes produces: the target computes `a + d` into $v0 in two steps (`addu $v0, $a0, $zero` in the first guard's delay slot, `addu $v0, $v0, $a1` in the second's), then copies $v0 into $a0 before the high clamp and $a0 back into $v0 on each of its two exits, so there are two live variables where `x` alone is enough. Ternaries in one arm, both arms, a single result variable and a single return point were all measured at 16-18 words against 20. |
| func_80058510 | unk_2879C | 35/36 words, 27 DIFF | The frame tick's reentrancy guard: `if (D_80073824[0] != 0 && D_80085F98[0] == 0) { D_80085F98[0] = 1; func_800584E4(); if ((s32) D_800737A8[0] >= 0xB) { func_80056948(); func_8005ACDC(); D_800737A8[0] = 0; } v = D_800737A8[0]; D_80085F98[0] = 0; D_800737A8[0] = v + 1; }` -- every instruction matches position for position, including the split read-modify-write at the end. The ONE missing word is a `nop`: the target leaves the second guard's delay slot empty, where this fills it with the `addiu $v0, $zero, 1` from the fall-through (an insn that SETS the register the `bnez` tests, which reorg usually refuses -- so something makes that block not its own thread there). `&&`, nested ifs, `||` with an early return, two separate returns and a peeled inner arm all give 35 words. |
| func_8003A2E0 + func_8003A10C | unk_72A8 | 37-38/37-38 words, 7-23 DIFF | The bytecode opcode that writes a palette word into a caller-supplied address. Two findings, both already paid for: the cursor needs TWO locals -- `c` advanced in place for the two index bytes and `r = c + 1` stored before the four address bytes are read off `r` (one cursor re-reads the field and folds the offsets, 21-24 DIFF) -- and the tail is ONE `s32` at +0x48 read four ways with `>> 8`, `>> 16`, `>> 24`, not four declared fields; see section 7 of matching-levers. What is left is one `sll $t1, $t1, 2` (the destination's `index * 4`) that the target emits BEFORE the four shifts of the address composition and this emits after. All 120 permutations of the five statements between them were tried with perm.py, as was hoisting the multiply into its own local at the point of the read. func_8003A10C is the same opcode in the other direction -- it READS the word at the composed address and stores it to the table slot -- with the same cursor pair and the same one misplaced `sll`, plus register numbering shifted by one. |
| func_80055864 | unk_2879C | 38/38 words, 19 DIFF | Fills the three parallel tables at D_80084538 (four pointers into D_801B9000 at +0, +0x3000, +0x4020, +0x4E40), D_800847C0 and D_80084768, written as three grouped blocks in that order. Every instruction and the word count are right; the six `lui`/`ori` constants are HOISTED into the prologue and the target pairs them with registers in the order 0x3A980, 0x3B990, 0x1BD50, 0x576E0, 0x11940, 0x69020 -- an alternation between the last two tables -- where grouped source gives 0x3B990, 0x576E0, 0x69020, 0x3A980, 0x1BD50, 0x11940 (first-use order). Interleaving the two tables' stores per index (29 DIFF) and swapping their blocks (26 DIFF) both make it worse, so the alternation is not statement order. |
| func_80050A58 | unk_1C540 | 38/38 words, 18 DIFF | Rebuilds 17 pointers: `u32 c = D_8002E950[i]` (a u32 local, so the `lbu` value is not masked again), `c / 21` and `c % 21` by the 0x86186187 magic, the quotient as a `u8` index into the second argument's word table and the remainder times 12 added on. Two things pull against each other: writing the remainder as `c - d * 21` gets the target's ORDER (index chain before the multiply-by-21) but computes the 21 from the MASKED quotient, while `c % 21` gets the unmasked quotient the target multiplies but emits the chain the other way round. A local for the table address, an ARRAY_REF on a `u32 *` cast and a `(u8)` cast in the index expression all leave it at 22. |
| func_800353F4 | unk_72A8 | 36/36 words, 2 DIFF | The shape is settled: `if (func_800352FC() == 0 || D_80082A70 != (u32) D_8006B230) { <the five-slot clear, func_80035378, func_80035208> return 1; } func_80035378(...); return 1;` -- the `||` is what gives the first test a branch INTO the body and the second one past it (as nested ifs, 22 DIFF), and the clear is func_80035348's `b[0x12 + i] = 0` verbatim. Needs `func_800352FC` changed to return s32 (it tail-calls func_80034FFC, so the code is the same). What is left is two words: the target fills the `beqz`'s delay slot with `lui %hi(D_80082A70)` and loads through it, where this hoists `lui %hi(D_8006B230)` instead. Both operand orders, `&D_8006B230[0]`, a `u8 *` type for D_80082A70 and a local for the call's result all leave it at 2. |
| func_8003666C | unk_72A8 | 34/34 words, 7 DIFF | func_800364EC's twin with the half-width offset: `s32 n = ((s[0x26] + 1) >> 1) - 1;` as its own local (inline, fold moves the `- 1` onto the other operand, 13 DIFF) and `(w - n) * 2` for the `sll 17`/`sra 16` pair. Every instruction is right and the word count matches; what is left is that the target reads `lh 0x22` BETWEEN `lbu 0x27` and `lbu 0x26` where this reads 0x26 second. All 24 permutations of the four statements were tried with perm.py (best 7 DIFF), as were a local for the halfword, a local for the shift alone, and the argument passed inline instead of through `t` (14-17 DIFF). It is sched1 choosing between three loads off one base, not statement order. |
| func_8004827C + func_8004D880 + func_800537D0 + func_80055CF4 + func_8003BD84 + func_80058A04 | unk_1AF38, unk_1C540 | exactly 1-3 words | **The same missing word three times: an `addu $rX, $rY, $zero` that local-alloc here coalesces away and the target keeps.** In func_8004827C and func_8004D880 it is the PARAMETER: the target copies the incoming $a0 into $v1 and uses $v1 for everything, spending $a0 on the outgoing argument, while this gives the parameter pseudo $a0 itself and copies it out where it is needed. func_8004D880 is otherwise exact, including the one-insn 16-bit subtractions (`u16 d = c - 0x824F;` -- as `s32` the constant will not fit an `addiu`) and the unmasked sum reused for the return (`return d + 0x30;` folds to one `addiu` only if `d` is NOT a local). Measured on func_8004D880: a local for the parameter, three separate difference locals, an explicit `& 0xFFFF` form and a `u32` parameter all coalesce the same way. func_80058A04's row below was the same shape on a call's return value, and it LANDED: unifying the two bytes into one variable imports the loop's $v0 conflict. That is a fifth cure, and the first that works by making the register unavailable rather than by rearranging the source around the copy -- but it needs a second live range for the value, which a parameter does not have. The three known CURES for this copy are all elsewhere in the source: re-dereference instead of caching (func_80039730), an explicit guard plus do-while on a loop bound (func_80059D1C), and a statement BETWEEN the producer and the copy (func_80048734). None of them applies to a parameter, which is what makes these the hard ones. func_800537D0 (the decimal formatter) is 3 DIFF of exactly this: everything else is right once `u8 *d = buf + n;` stops loop.c strength-reducing `buf[n]` into a walked pointer and the digit arm comes FIRST (`if (blank == 0)`), but the target computes the quotient into its own register, tests THAT, and copies it into `v` in the branch's delay slot. `q = v / 10; if (q == 0) ...; v = q;` in every order, a local copy of the parameter, and `if (v / 10 == 0)` with the division repeated all coalesce. func_80055CF4 is the same word on a SYMBOL's address: the target
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
| func_80056DB4 + func_800563B0 + func_80056D44 | unk_1C540 | 2-3 words, 2-12 DIFF | Three functions that fill the parameter block at D_80084918 and pass its address. **The addressing is now solved**: the base is a POINTER LOCAL initialised in its declaration (`u8 *q = D_80084918;`), which materialises it first, while the +0 store goes through the SYMBOL (`*(u32 *) D_80084918 = v;`) and keeps the target's symbolic `sw $a0, %lo(D_80084918)($a1)` against the shared `(high)` pseudo. The D_80073740 lookup goes into its own local, also in a declaration, which is what gets it scheduled where the target has it. That took func_80056DB4 from 4 DIFF to 2, and all that is left there is the order of three stores -- `sh 0x3A`, `sh 0x3C`, `sw 0x24` against this `sw 0x24`, `sh 0x3C`, `sh 0x3A` -- which NONE of the 120 permutations of the five stores produces (measured with tools/decomp/perm.py; the best two are 2 DIFF). A struct type for the block is not the answer either: as COMPONENT_REFs the +0 store folds onto the register base and the function is a word short (21 DIFF). func_800563B0 is the same recipe with four halfword stores and sits at 12 DIFF, so it wants its own permutation run. |
| func_800491F4 | unk_1C540 | not measured in isolation | A list walk that threads each entry's +0xC to the previous one. |
| func_8004F330 | unk_1C540 | 40/40 words, 9 DIFF | Fills n 0x10-byte slots from D_8002E5D8 and three words from D_8002E5E8 after. Four things paid: `u32 *s = D_8002E5D8;` assigned INSIDE the loop body so its materialisation lands in the preheader after the `blez` (func_8004CCBC's lever) with the first word read through the symbol and the rest through `s`; `u32 *t = D_8002E5E8;` assigned AFTER the loop, or its base is hoisted ahead of the guard; the 0x2B8 store written before the 0x240/0x244 pair (9 DIFF against 19 the other way); and the loop's stores through the parameter `p` with the three trailing stores through a `u8 *q = p;` copy, which is what keeps the target's `addu $t3, $a0, $zero` alive -- with everything through `q` the copy coalesces and the function is a word short. What is left is which registers the two in-loop address VALUES take: the target computes `p + (0x2AC + i * 0x10)` first into $a0 (the parameter's own incoming register, so `p` is pushed to $t3) and `p + (0x23C + i * 0x10)` second into $v0, where this computes them the other way round into $v0/$v1 and leaves `p` in $a0. All 5040 orderings of the seven loop statements were sampled with perm.py --max 400; the source order above is the best. |
| func_8005A56C | unk_2CDC0 | 23 DIFF | Two stack blocks selected by the first argument, with `(x << 16) >> 8` scaling. |
| func_80047CD8 | unk_1AF38 | 28/27 words, 20 DIFF | The target keeps the constant -1 in `$s1` across two calls and compares both results against it; this puts the first result there instead. An explicit `s32 m = -1;` local does not move it. |
| func_8004D880 | unk_1C540 | 28/28 words, 22 DIFF | A Shift-JIS to ASCII classifier: three `(u16)(c + K) < N` range tests, each returning that sum plus an ASCII base. Two shapes now paid for: the SUM is its own `u16` local (inline in both the test and the return, `fold` merges the two constants, and because the return type is `u8` it merges them MOD 256 -- the listing's `addiu $v0, $a1, 0x30` comes out `addiu $v0, $a0, 0xE1`), and the third test REASSIGNS the parameter so its `addiu` writes $v1 in place. What is left is a three-way register permutation: the parameter and the call's argument both prefer $a0 and conflict, and the parameter wins it here on references. Note the listing spends TWO copies on that value -- in at entry, back out for the call -- so keeping it in $a0 is a word SHORT, not better. `docs/near-misses/func_8004D880.c`. |
| func_8003A858 + func_8003A060 | unk_72A8 | 43/43 words each, 2 DIFF | **Byte-identical listings** -- the same opcode twice, so one shape settles both. The func_8003A2E0 family's third member: reads the destination index and the table index with `*(*cp)++`, then the four operand bytes off `r = *cp`, then `*cp += 4`, and writes a little-endian word into `state[a].unk_48` as FOUR accumulating stores (cse forwards each re-read of the slot, so `*(u32 *) (s + 0x48) += q[k] << n` is the shape). Three things were paid for: the table index's `* 4` hoisted into its own local right after the read (17 DIFF inline), the destination computed as `a * 4 + (u32) state` and not `state + a * 4` (the `addu`'s operands, 8 DIFF), and `b * 4` written at the END of the operand sum (11 DIFF at the front). What is left is two adjacent insns: the target emits `sll $t0, $t0, 2` (the destination's index) before the operand chain's last `addu`, this one after. All 120 permutations of the five statements between the reads were run with perm.py -- best 2 DIFF, same pair. |
| func_80048B8C | unk_1AF38 | 44/44 words, 14 DIFF | Walks 8-byte entries from `p[0] + p[1] * 4` to `p[2]`, and for each whose +2 halfword is 2 chases the pointer at +4 and runs an inner loop over 0xC-byte records clearing the low 6 bits of the halfword at +4 and then setting them to 0xE. Three findings. The mask must be a 32-BIT constant reaching the loop as a register (`and $v1, $v0, $t1`): read the halfword into a `u32` local first, because `*(u16 *) x & ~0x3F` is narrowed by fold to `andi 0xffc0` and then there is nothing for loop.c to hoist. The two stores to the SAME halfword need the first one `volatile`, or gcc 2.7.2's flow.c deletes it as a dead store -- confirmed with a probe, and an intervening read of `*e` keeps it too but then sched fills the load-delay slot with it and the function is a word short. And both stores have to go through ONE `u16 *` pointer local advanced by 6, or the volatile store is not strength-reduced and uses `4($v1)` where the target has `0($a1)`. What is left is purely registers: the target has the entry pointer in $a2, the halfword cursor in $a1 and the flag byte in $v1, this one $a1/$v1/$a2, and the `addiu` of the cursor's base lands before the flag `sb` instead of after. perm.py over the six inner-loop statements (base order is the best) and a search.py sweep both found nothing better. |
| func_8003A904 | unk_72A8 | 46/45 words, 43 DIFF | The func_8003A2E0 family again: func_8003A1A4's state-table lookup (`m = state[B].unk_48 * 4`) with func_8003A858's four accumulating stores as the tail. Every instruction of the tail matches position for position; the whole diff is ONE word at the top. The target fills the load-delay slot of `lw $v0, 0x1C($a0)` with `lw $t2, 0x80($a0)`, i.e. sched1 has the state load second; here it lands seventh, after the cursor's increment store, whatever the source order -- the state read first, second, through an explicit cursor local, as `*(*cp)++` or as `c[0]` with `*cp = c + 1` were all measured at 43-44. |
| func_8003B714 | unk_72A8 | 45/45 words, 12 DIFF | Its twin func_8003B820 LANDED on 2026-10-24, and the addressing levers it found apply here: the 0x80010000 half in its own `u8 *` local with the list pointer built from it as `(u32 *) ((u32) base | 0x29C)`, which is the target's one hoisted %hi pseudo serving both constants, and the record index as `(a[3] & 0x1F) * 84`. Two differences from the twin. The comparison constant 0x13 must NOT be hoisted here (the target re-materialises it inside the loop, where func_8003B820 keeps it in $s2), and the only body that stops loop.c hoisting it reads the table byte TWICE -- once for the sign test and once for the `& 0x3F` -- where the target loads it once into $a0. A single `s32 v` local for it, a `u8 *rec` local for the third argument, an assignment inside the `&&`, a separate local for the masked index and a `u32` index were all measured at 36-40 DIFF and three words over, because each shortens the body enough that loop.c hoists the 0x13 and buys a third callee-saved register. What is left in the 12: that duplicated load and which of $v0/$v1 holds the masked byte against the index chain. |
| func_8004E4C0 | unk_1C540 | 46/47 words, 45 DIFF | The fixed-width decimal formatter: writes `n`'s digits least-significant-first into the buffer, pads the rest with the fourth argument, NUL-terminates and returns `out - w`. Division by 10 through the 0xCCCCCCCD magic. The shape is settled -- `*out++ = n % 10 + 0x30;` then `for (i = 1; i < w; i++) { n = n / 10; if (n == 0) break; *out++ = n % 10 + 0x30; }` then a pad loop on the same bound -- and every instruction is right. The ONE missing word is `addu $t2, $v1, $zero`: the target masks the `u16` width into $v1 for the guard and COPIES it to $t2 for the first loop's bottom test, because the loop body clobbers $v1, and it masks `w` a second time for the pad loop. Here cse shares one mask across all three uses and loop-invariant motion hoists it above the first digit store, so there is nothing to copy. Measured: an explicit guard plus do-while, `w > 1` as the guard, an `m = w` local for the first bound, for the pad bound, and before the guard -- all 46 words or a rotated loop at 48. |
| func_80048C3C | unk_1AF38 | 48/49 words, 15 DIFF | Reloads one of the D_80071210 pairs: `func_8003E4FC(6, e[0], 0)`, then through `e[1]` a func_8003F80C / DrawSync / func_80047200 / func_8003F80C / func_80046F88 / func_80048B8C sequence, clears D_80080A89 and returns `e + 1`. Every instruction is right. The one missing word is the return: the target does `addiu $s1, $s1, 0x4` IN PLACE and then `addu $v0, $s1, $zero`, where combine folds the pair into `addiu $v0, $s1, 4` whatever the spelling -- `return e + 1`, `e++; return e;`, `e = &e[1]; return e;`, the D_80080A89 store before or after, and a `u8 *` base with `e += 4` were all measured. For the in-place add to survive, `e` has to be live past the return-value move, and nothing in the tail keeps it so. |
| func_8005552C | unk_2879C | 48/52 words, 40 DIFF | Maps `k & 0xF00` to a mode (0x600->2, 0x700->3, 0x800->0, else 1), calls func_8005405C with it and then func_80055730 or func_8005560C with `(m << 8) | ((k & 0xFF) >> 4)` and `((k & 0xF) << 10) + 0x1C00`. Three things are settled: the parameter is `u32`, so the `>> 4` is an `srl` and not an `sra`; the dispatch is a NESTED-IF chain written with `!=` tests, `if (x != 0x600) { if (x > 0x600) { if (x != 0x700) { ... } else ... } else ... } else ...`, which is the only shape that branches INTO each arm and emits the target's `slti $a0, 0x601` low-bound test (a two- or three-case `switch` gives a decision tree rooted at 0x700 instead, 44-46 DIFF); and `m` must be `s16`, which is what makes the arms set $v0 and a shared `addu $s0, $v0, $zero` copy it, as the target does. What is left is the innermost pair: jump.c rewrites `if (x != 0x800) m = 1; else m = 0;` as `xori` plus `sltu` -- the store-flag transform -- where the target keeps the two branches and an `addu $v0, $zero, $zero`. Measured: `m` as s32/u32/u8, the arms swapped, the `m = 1` hoisted out as a pre-initialisation, an `else if` chain, and a separate `n` with `m = n` after. |
| func_8004A574 | unk_1C540 | 41/41 words, 5 DIFF | Scales a halfword of `D_80073414[p[1]].unk_0C + p[0] * 20` -- +0x10 or +0x12 by the second argument -- by `D_800713CC[e[3]]` and passes it to the function pointer at `e + 8`. Every instruction and the word count are right. The target has `mflo $v0` in the join block, before the `lw 0x8($a2)`, and spends `addu $a0, $v0, $zero` in the `beqz`'s delay slot; this coalesces the product into $a0 and emits the `mflo` BELOW that branch, in the call block, so there is no copy. Measured: both arms as if/else and as a ternary (5), the operands of the multiply swapped (7), the halfword into its own local with one multiply after the `if` (17), the multiply split from the load (12), the function pointer hoisted into a local before the test, an early `return` instead of a guarded block, an `s32` product and a temporary for the argument (all 5), plus a full `search.py` sweep -- 18 variants, none better than the base. |
| func_8004E130 | unk_1C540 | 22/22 words, 9 DIFF | $gp group. Every instruction matches; only which of $v1/$a0 holds the masked index and which the table address. |
| func_8003C634 | unk_72A8 | 7 DIFF | $gp group. Reading the bound in the loop condition took it from 18 DIFF to 7; what is left is the register pair, as in func_8005C818. |
| func_80033D44 + func_80033D54 | unk_72A8 | BLOCKED, not a shape | Both need `%gp_rel(D_8008099C)`, and func_80034EF8 and func_8003C0C8 in the same carved unit reach that symbol ABSOLUTELY and are landed. maspsx's rewrite keys on the symbol for the whole unit, so one unit cannot give it both forms: these two want the unit split, or those two reverted. |
| func_80039DBC | unk_72A8 | 25/24 words, 18 DIFF | Calls through a table at +0x40 with four arguments from +0x48..+0x54; the fifth load reuses the table's register in the target and this spends an extra one. |
| func_8003AB44 | unk_72A8 | **54/54 words, 9 DIFF** | A 3-cycle register rotation, source in `docs/near-misses/func_8003AB44.c`. Was 52/54 until argument 6 was declared `s16` instead of `s32`-with-a-cast (see the narrow-parameter entry in matching-levers section 7). Landing it means narrowing that argument in the committed prototype, which changes func_8003AAC4's call setup -- check that caller or use an `__asm__` alias. |
| func_8005500C | unk_2879C | **55/55 words, 33 DIFF** | Sparse `switch` on `& 0xF000`. One BRANCH-ARMS difference (the 0x9000 compare falls through into the shared body in the target, branches to it here) plus a 3-way register rotation. Source and everything measured against it in `docs/near-misses/func_8005500C.c`; `search.py` found nothing. |
| func_80059DAC | unk_2CDC0 | **58/58 words, 8 DIFF** | A bubble sort over D_800869C0. Four structural things were needed to reach the exact count and they pull against each other -- separate base locals per loop, the second materialised INSIDE the loop body so it lands after the `blez` guard, the index written first, and the comparison round the target's way; all four are written up in `docs/near-misses/func_80059DAC.c`. What is left is loop.c declining to make `i * 8 + base` an outer giv. The function makes no calls, so `loop_has_call` is false and the threshold is not halved. |
| func_80054104 | unk_264E0 | **LANDED** | It was the inner loop's FORM, not the subscript: written with `goto` it gets no NOTE_INSN_LOOP_BEG, so loop.c never strength-reduces `t[j]` or hoists `$sp + 0x10`, and the four extra words go away. `j = 0` first in the outer body supplies the back edge's delay slot. See the goto-loop lever in matching-levers section 9. |
| func_80055D84 | unk_2879C | **58/58 words, 5 DIFF** | A two-register swap around the final `or`: the target reuses the mask's own register as the destination and this source does not, and --greg shows why (the mask is allocated first on priority and takes the lowest free register instead of preferring the argument register). Getting to the exact count needed the cross-jump lever above; everything measured is in `docs/near-misses/func_80055D84.c`. |
| func_8004DE88 | unk_1C540 | **64/65 words, one register copy** | Command-buffer builder. Everything matches except `addu $a0,$v0,$zero`, a copy of strrchr's return: the target keeps the cursor in $a0 and the constants in $v0, this source keeps the cursor in $v0 so the copy is deleted. The suspect is local-alloc's SUGGESTION pass, which assigns a quantity born from a hard-register copy before the priority loop. See `docs/near-misses/func_8004DE88.c`. |
| func_8004DF8C | unk_1C540 | **70/72 words, two words** | func_8004DE88's sibling, and the same cursor-versus-address-value register contest. Its own contribution is the $gp-relative half: it needs both an absolute address materialisation and a one-word store for D_80081548/D_8008154C, which takes the bare `.size` records PLUS incomplete-array declarations (see the pattern above -- `extern u32 D_X;` costs two more words). Everything measured is in `docs/near-misses/func_8004DF8C.c`. |
| func_80036B10 | unk_72A8 | **LANDED** | The stack-parameter prologue interleave, and it was D_80082BC0 declared as an incomplete array and passed by name instead of `&` on a scalar pointer. See the SOLVED section above. |
| func_8003931C | unk_72A8 | **85/85 words, 10 DIFF** | func_80039470's sibling and the same three shapes (one-statement cursor read, error blocks as `else` arms not guard clauses, pointer-local table base). What is left is a two-register swap with the margin measured: the opcode byte is 4 refs/23 insns (0.348) against the +0x80 pointer's 6/32 (0.375), and ONE more reference on the byte would flip it -- see `docs/near-misses/func_8003931C.c` for why none is available free. |
| func_80036D4C | unk_72A8 | **LANDED** | The stack-parameter prologue interleave, and it was D_80082BC0 declared as an incomplete array and passed by name instead of `&` on a scalar pointer. See the SOLVED section above. |
| func_8004CAA0 | unk_1C540 | **NOT a C function as written** | 18 words, and the cheapest-looking row in the table until you read it: it saves `$sp` into the PSX scratchpad at 0x1F8003FC, switches `$sp` there, calls func_8004C36C and restores. Plain C cannot assign `$sp`, so it wants inline asm or a decomp device like func_8003E758's. `triage.py` now reports this shape as STACK-SWITCH rather than EASY. |
| func_8004CECC | unk_1C540 | **101/102 words, one word** | HSV->RGB. Every opcode, immediate, displacement, branch offset and delay slot matches, and the `beqz`->end delta agrees, so all six switch arms are byte-correct; the whole gap is the `v` parameter's entry copy `addu $v1,$a2,$zero`, and the 101 DIFF is that one-word shift rather than 101 wrong words. Diagnosed to the register from the dumps in `docs/near-misses/func_8004CECC.c`, which lists the eight shapes and the whole `search.py` sweep already measured against it and names the signature to aim at: the 0x80808081 magic belongs in $t0, not $a3, because the colour block had one fewer long-lived block-local than this source gives it. Landing it also needs jtbl_8002E5F8 placed. |

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

| func_80059814 | unk_2CDC0 | **94/103 words, 69 DIFF** | The +0x10 accumulator advance, ten case labels in six bodies. Structure verified instruction by instruction, including that flow keeps both `sw 0x10` per body because the overwrite is in another basic block and `mem_set_list` resets per block. The 9 words left are ONE sched1 decision: `schedule_select` takes both stores first in the backward pass (`insn 32 has a greater potential hazard`), which puts them LAST in the body, so cross-jumping merges them into the shared tail and the five `addu $v1, $v0, $zero` copies go with them. `docs/near-misses/func_80059814.c`. |

| func_8005A608 | unk_2CDC0 | **92/92 words, 11 DIFF** | Was 88/92 with 85 DIFF. The four words came from an `s32` index parameter RE-ASSIGNED its own truncation -- `j = id; id = (s16) id;` -- which forces the raw's copy and invalidates cse's equivalence so the else arm recomputes `(s16) i` instead of forwarding the entry's extension. What is left is one register chain: the extension's intermediate takes $v0 rather than being in place in $a1, which pushes the entry's -1 to $a2, which lets reload's find_equiv_reg copy it instead of materialising fresh. `docs/near-misses/func_8005A608.c`. |

| func_80052A90 | unk_25944 | **LANDED** | The Shift-JIS cell blitter. The last word was the FLAG: it is 32 bits wide and tested through a NARROW local, `u8 t = ok;`, which cse's `insert` does not see as a register copy (it looks for a REG of the same MODE in the source's class), so the quantities never merge and MIPS emits the QImode move as the listing's `addu $v0, $s3, $zero`. `s8` and `u16` work the same. `u8 ok` gives `andi 0xFF`, `u32 ok` tested directly drops the word, and a same-width `t = ok` is copy-propagated away. The other four shapes are on the function. |
| func_80037E58 | unk_72A8 | **70/70 words, 8 DIFF** | The D_80082A38 initialiser. Structure verified, including the shape that took finding: the descending loop's 0x48 only survives as the store's DISPLACEMENT when the base is a loop-invariant REGISTER, because symbol-plus-constant folds into loop.c's add_val and a pseudo cannot. What is left is one register pair with the margin measured -- the D_800809C0 address is 2 refs over 2 insns (4.000) against the D_80082A38 address's 3 over 4 (3.000), so local-alloc takes the short one first and the long one falls to $v1; the eighth word is sched2 then pulling the next `lui` up into the freed register. `docs/near-misses/func_80037E58.c`. |
| func_8005A778 | unk_2CDC0 | **101/101 words, 21 DIFF** | func_8005A608 with a third argument and a bound check, on the same lever, plus TWO locals for `p` (the listing's `addu $s3, $s4, $zero`) and `e->unk_10 = h` placed after `e->unk_08` so sched2 fills the `lhu` load-delay slot with it. The 21 words are the same register chain as func_8005A608's. `docs/near-misses/func_8005A778.c`. |
| func_80038F48 | unk_72A8 | **98/98 words, 12 DIFF** | The script VM's control-flow dispatcher. Word count exact on the first compile; everything structural is verified and written up, including the `(u8) (op + 3) < 3 || op == 0` loop condition, the if/else-if chain over -3/-2/-1, the +0x80 table read into a local so 0x48 stays a displacement, and the cross-jumped tail the 0xFE and 0xFF arms share. The 12 words are all inside the 0xFF arm: the listing interleaves the four byte loads with the shifts and reuses $v1 for the first and last byte, where this source issues all four loads first and spends four registers. `docs/near-misses/func_80038F48.c`. |
| func_8004E6F4 | unk_1C540 | **102/100 words, two words OVER** | The 15-bit colour-ramp builder. Every opcode, immediate and displacement in both arms and the shared tail is paid for, including `w[i - 1]` so the weight giv IS reduced (two adds, benefit 4) and three combined values rather than six product temps (six spill, 116 words). The two words are ONE register: the listing reuses $v1 for the weight and then the fifth product, which frees $t9 for `a`; this source keeps them apart, needs thirteen homes and pushes `a` into $s0. `docs/near-misses/func_8004E6F4.c`. |
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
| func_80047054 | 58/58 | SHAPE | -- | 27 |
| func_8004951C | 156/157 | LENGTH | EXTRA,LENGTH,MISSING,OPCODE | 3 |
| func_8004A330 | 76/77 | LENGTH | CALLEE-SAVED,EXTRA,IMMEDIATE,LENGTH,MISSING,OPCODE | 13 |
| func_8004AC3C | 128/128 | SHAPE | BRANCH-ARMS,DELAY-NOPS,EXTRA,IMMEDIATE,MISSING,OPCODE | 21 |
| func_8004CECC | 101/102 | LENGTH | -- | 42 |
| func_8004D294 | 114/114 | SHAPE | -- | 12 |
| func_8004DE88 | 64/65 | LENGTH | OPCODE | 31 |
| func_8004DF8C | 70/72 | LENGTH | IMMEDIATE,OPCODE | 36 |
| func_8004E6F4 | 102/100 | LENGTH | CALLEE-SAVED | 34 |
| func_800500B4 | 139/139 | SHAPE | DELAY-NOPS,OPCODE | 64 |
| func_80052A90 | 96/96 | SHAPE | OPCODE | - |
| func_80054B08 | 84/84 | SHAPE | IMMEDIATE,OPCODE | 10 |
| func_8005500C | 55/55 | SHAPE | BRANCH-ARMS | 14 |
| func_800550E8 | 119/120 | LENGTH | OPCODE | 27 |
| func_80055D84 | 58/58 | ALLOC | -- | 5 |
| func_800565D8 | 31/31 | SHAPE | OPCODE | 1 | (the row's one shape finding has since been paid: 6 DIFF -> 3)
| func_80056654 | 107/107 | SHAPE | OPCODE | 11 |
| func_80057948 | 64/64 | SHAPE | IMMEDIATE | 7 |
| func_8005947C | 81/81 | SHAPE | OPCODE | 10 |
| func_80059814 | 94/103 | LENGTH | COMMUTED | 25 |
| func_800599B0 | 133/133 | ALLOC | -- | 1 |
| func_80059DAC | 58/58 | SHAPE | IMMEDIATE | 3 |
| func_8005A608 | 92/92 | SHAPE | DELAY-NOPS,OPCODE | 8 |
| func_8005A778 | 101/101 | SHAPE | DELAY-NOPS,IMMEDIATE,OPCODE | 13 |
| func_8005C2C0 | 132/132 | SHAPE | OPCODE | 1 |
| func_8005C4D0 | 64/64 | SHAPE | -- | 6 |
