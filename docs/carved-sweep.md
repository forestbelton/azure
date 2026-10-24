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
* **An arithmetic constant binds to the term the source groups it with.**
  `(v & 0xF) + 0x7C80` then `+ ((v & 0x10) << 3)` must be split into a local;
  written as one expression the compiler folds the constant into the second
  term instead, and parentheses do not survive `fold`.

## Parked: tried and not matching

Each of these has the right instruction set and word count with the registers
or the order wrong, so they want the dumps rather than another guess.

| function | unit | state | tried |
|----------|------|-------|-------|
| func_800368D0 + func_80035C80 + func_80035CE4 + func_80036900 | unk_72A8 | 2 DIFF each, at the exact word count | Three instances of one coupling, now understood and still unsolved. The load ORDER is fixable: splitting the pointer at +0x44 and the s16 at +0x4A into two statements, pointer first, puts the `lw` ahead of the `lh` as the target has it -- sched's tie-break for equal-priority insns is `INSN_LUID`, i.e. RTL order (gcc 2.7.2 sched.c, `rank_for_schedule`). What is left is the two registers: local-alloc's priority is roughly `refs * log(refs) / live_length`, so the pointer -- whose load is now FURTHER from the `addu` that consumes it -- has the longer range, loses the tie and takes $v1 where the target has $v0. The two passes pull opposite ways: whichever load is written first is emitted first and is thereby allocated second. A source shape that satisfies both is what these three want; `(t + 0x50)[i]` and `*(u8 *)(t + 0x50 + i)` do not, since fold canonicalises the add either way -- nor does putting the index first (`*(s16 *)(p + 0x4A) + *(u8 **)(p + 0x44)`), an array subscript, or binding the 0x50 to the pointer, all measured on func_80036900. Splitting the two loads into statements AFTER the call moves them ahead of the function-pointer store instead (16 DIFF), and before the call 36. |
| func_80033AE8 | unk_72A8 | 17/17 words, 6 DIFF | The bitmap CLEAR; func_80033AA8 and func_80033B2C both landed from the same shape (the constant base assigned after the guards, the index inside the addressing expression). This one differs only in that the raw argument and the constant 1 hold each other's registers: the `nor` makes sched put the `sllv` ahead of the `lw`, which shortens the constant's live range by one insn and flips the two past each other in local-alloc's priority. A `u16` parameter matches it exactly -- but then func_80039258's `func_80033AE8((s16) v)` loses the `sll`/`sra` pair the target has there, so the parameter is at least 32 bits wide and this is NOT the spelling. A local for the mask (17 DIFF) and a local for the whole address (17) are both worse. |
| func_8004AFE8 | unk_1C540 | 14 DIFF | Counts entries whose `(*p)[1]` equals the first one's, with the loop's `p != 0` test at the bottom and a guard at the top. |
| func_800589B8 | unk_1C540 | 19/19 words, 2 DIFF | The byte reader every func_80058xxx call goes through: `off = *(s32 *) q; c = *(u8 *)(D_80085FA4[0] + off); off++; *(s32 *) q = off; if (D_800869B0[0] < (u32) off) { *(s32 *)(q + 0x2C) = 1; return -1; } return c;`, with both globals read into locals so each load's delay slot gets an independent insn (18 and 19 DIFF without that). What is left is that the target's two first loads are in the other order: `lw %lo(D_80085FA4)` then `lw 0x0($a1)` where this has the dereference first. cc1psx's fill pass swaps a load with the next insn, so the pre-fill order must have been the dereference FIRST -- and sched1 puts the symbol load first whatever the source order (measured: both statement orders, `off + base`, a `u8 *` base, s32 and u32 offsets). |
| func_80058A04 | unk_1C540 | 27/30 words, 24 DIFF | The 7-bit varint decoder: `c = f(p); if (c == 0) return 0; if ((c & 0x80) == 0) return c; c &= 0x7F; do { d = f(p); c = (c << 7) + (d & 0x7F); } while (d & 0x80); return c;`. Every instruction is right and this is three words SHORTER than the target, which spends `addu $a0, $v0, $zero` after each of its two calls plus a `j`: the byte's pseudo lives in $a0 there, where here it keeps $v0 (the call's own return register) and the copy is deleted. Nothing found makes $v0 unavailable to it -- two variables with a copy between them, a u8 byte, a nested `if`, a `while (1)` with the return inside, all 27 words. |
| func_8004D690 | unk_1C540 | 29/31 words, 18 DIFF | `while (1) { if (*a != 0) { (*(void (**)(u8 *)) a)(a + 4); return; } if (*b == 0) return; func_8004D7E8(c); }` -- every instruction right, and two words SHORTER than the target: gcc moves the indirect-call block to the end of the function, where it falls into the epilogue, while the target leaves it first and spends `j` plus a `nop` to reach it. `break` instead of `return`, the two arms as if/else, and peeling the first test (32 words, cross-jumping does not merge the two call sites back) all leave it there. |
| func_8003C67C | unk_72A8 | 31/31 words, 26-33 DIFF | Moves D_8006B200s[i] to the front, shifting the rest up through an 8-byte stack temp. The target does NOT strength-reduce the shift loop: it recomputes `sll $a0, $a1, 3` for the destination and `sll $v0, $a1, 3` for the source every iteration, with the decrement between them. Every spelling tried gets walked pointers from loop.c instead (`while` with `i--`, a `for`, `p[i] = p[--i]`, a separate `j = i - 1`, field-by-field copies). What stops loop.c reducing a two-address copy loop over a 4-byte-stride... 8-byte-stride array is the open question. |
| func_800565D8 | unk_1C540 | 30/31 words, 11 DIFF | The two-sided scale: `if (n < 0x40) { if (p[0x21] == 0) return 0; r = -((p[0x21] * 2) * (0x3F - n)); } else if (n == 0x40) return 0; else { if (p[0x20] == 0) return 0; r = (p[0x20] * 2) * (n - 0x40); } return r;` -- one result variable, so both multiply paths share the final `sll`/`sra` pair as the target does. Two things left, both one word: the target RE-LOADS `p[0x20]` for its multiply where cse here keeps the byte from the test, and it binds the `* 2` to the byte in the second arm where fold moves it onto `(n - 0x40)` (the first arm, with the negation, comes out right). `(p[0x20] << 1)`, `(p[0x20] + p[0x20])`, a local for the doubled byte and the operands swapped all make it shorter still. |
| func_8004E9EC | unk_1C540 | 33/33 words, 27 DIFF | The template fill: `while (i <= n * 2) { if (*q == 0) { while (i < n * 2) { three s8 reads of D_80080B70 into locals, three stores, i += 2, q += 2; } r = 1; } i++; q++; }`, with `n * 2` re-evaluated in both conditions (the listing recomputes `sll $v1, $t2, 1` each outer iteration) and the three bytes read into locals before the stores (all three `lb`s precede all three `sb`s). D_80080B70 is read SIGNED through an alias. What is left is registers: the target copies the count out of $a1 so the result can live there, keeps only `%hi(D_80080B70)` outside the inner loop and re-materialises the `%lo` inside it, and initialises the counter and the cursor in the preheader; this keeps the count in $a1, hoists the whole address, and puts the counter's init ahead of the guard. |
| func_8003A3E8 + func_8003A374 | unk_72A8 | 32/32 words, 15 DIFF | Two bytecode opcodes that read a byte, a table index, then a little-endian 32-bit operand from the cursor, and store a word THROUGH A BYTE they load from `operand + table[index] * 4` -- `*(u32 *)(u32) *(u8 *)(...)`, which is what the `lbu`/`sw` pair at the end is (the game's own bug or a deliberate low-address write; either way that is the C). The statement order is pinned: the first byte and its increment, then the state pointer at +0x80, then the table lookup through `**q` WITHOUT incrementing, then the second increment, then the four operand bytes, then `*q += 4` (26 DIFF with the lookup after the operand). What is left is which registers the four byte reads and the accumulator use -- the target accumulates in $v0 where this uses $a1, with the `sll` of the table value interleaved differently. |
| func_8004CAA0 | unk_1C540 | not attempted | Switches `$sp` to the scratchpad at 0x1F8003FC around a call, saving the old stack pointer at that address. No C expression produces that; it wants inline asm, like func_8003E758's. |
| func_8004D75C | unk_1C540 | 19 DIFF | A 0x44-byte block move (the odd word first, then sixteen words in a four-word loop -- so the shape is right) followed by `*(u32 *) s = 0;`. |
| func_80048FBC | unk_1C540 | 18/18 words, 7 DIFF | `p[func_80048F8C(p, n)] = func_8004B404(0x104);` -- both arguments of the first call pass through untouched, so neither move is emitted. Only which of `$s0`/`$s1` holds the pointer and which the index chain. Measured against local-alloc's own arithmetic: the chain (the result copy, the `sll` and the `addu`, all one quantity) has six references over four insns and the pointer two over nine, so the chain outranks the pointer and takes $s0 -- the target has it the other way round, which no spelling reaches. `p += f(...)` and a separate `q = p + i` both move the arithmetic ahead of the second call instead (16 DIFF). |
| func_80059D1C + func_8005C818 | unk_1C540 | 16/16 words, 9 DIFF | Sweeps over D_80085FA8's 0x50-byte entries. func_80059D60, the third of this family, LANDED once the bound was read in the loop condition rather than hoisted into a local -- but these two STORE in the loop body, so loop-invariant motion cannot lift that read, and the in-loop re-read stands where the target has the hoisted copy. A guard plus a second read inside the `if` (cse's copy) is worse at 14/16. So the question is why gcc could hoist a load across stores whose base is the loop's own giv: that is an alias-analysis question, not a source-shape one. |
| func_80047738 + func_8004827C | unk_1AF38 | 17 and 12 DIFF | Both shapes are close; func_8004827C's target copies the parameter into `$v0` before using it, which no spelling here reproduces. |
| func_8005C668 | unk_1C540 | 11 DIFF | Builds a 0x40-byte block on the stack (a word from D_80073740, the constant 3, two halfwords) and passes its address. |
| func_800489F4 | unk_1AF38 | not measured | func_80047738 plus an `ori 0x20`; park it with that one. |
| func_80049E1C | unk_1C540 | 16-18/20 words, 18 DIFF | The clamp, func_80049DE8's family -- and func_80049DE8 LANDED once its negative arm was written as a conditional expression (`a = (a < 0) ? m + a : a;`), which is what pays for the copies through $v0 that a plain `if` optimises away. This one needs three or four more such copies than any of six shapes produces: the target computes `a + d` into $v0 in two steps (`addu $v0, $a0, $zero` in the first guard's delay slot, `addu $v0, $v0, $a1` in the second's), then copies $v0 into $a0 before the high clamp and $a0 back into $v0 on each of its two exits, so there are two live variables where `x` alone is enough. Ternaries in one arm, both arms, a single result variable and a single return point were all measured at 16-18 words against 20. |
| func_80058510 | unk_1C540 | 35/36 words, 27 DIFF | The frame tick's reentrancy guard: `if (D_80073824[0] != 0 && D_80085F98[0] == 0) { D_80085F98[0] = 1; func_800584E4(); if ((s32) D_800737A8[0] >= 0xB) { func_80056948(); func_8005ACDC(); D_800737A8[0] = 0; } v = D_800737A8[0]; D_80085F98[0] = 0; D_800737A8[0] = v + 1; }` -- every instruction matches position for position, including the split read-modify-write at the end. The ONE missing word is a `nop`: the target leaves the second guard's delay slot empty, where this fills it with the `addiu $v0, $zero, 1` from the fall-through (an insn that SETS the register the `bnez` tests, which reorg usually refuses -- so something makes that block not its own thread there). `&&`, nested ifs, `||` with an early return, two separate returns and a peeled inner arm all give 35 words. |
| func_8003A2E0 + func_8003A10C | unk_72A8 | 37-38/37-38 words, 7-23 DIFF | The bytecode opcode that writes a palette word into a caller-supplied address. Two findings, both already paid for: the cursor needs TWO locals -- `c` advanced in place for the two index bytes and `r = c + 1` stored before the four address bytes are read off `r` (one cursor re-reads the field and folds the offsets, 21-24 DIFF) -- and the tail is ONE `s32` at +0x48 read four ways with `>> 8`, `>> 16`, `>> 24`, not four declared fields; see section 7 of matching-levers. What is left is one `sll $t1, $t1, 2` (the destination's `index * 4`) that the target emits BEFORE the four shifts of the address composition and this emits after. All 120 permutations of the five statements between them were tried with perm.py, as was hoisting the multiply into its own local at the point of the read. func_8003A10C is the same opcode in the other direction -- it READS the word at the composed address and stores it to the table slot -- with the same cursor pair and the same one misplaced `sll`, plus register numbering shifted by one. |
| func_80055864 | unk_1C540 | 38/38 words, 19 DIFF | Fills the three parallel tables at D_80084538 (four pointers into D_801B9000 at +0, +0x3000, +0x4020, +0x4E40), D_800847C0 and D_80084768, written as three grouped blocks in that order. Every instruction and the word count are right; the six `lui`/`ori` constants are HOISTED into the prologue and the target pairs them with registers in the order 0x3A980, 0x3B990, 0x1BD50, 0x576E0, 0x11940, 0x69020 -- an alternation between the last two tables -- where grouped source gives 0x3B990, 0x576E0, 0x69020, 0x3A980, 0x1BD50, 0x11940 (first-use order). Interleaving the two tables' stores per index (29 DIFF) and swapping their blocks (26 DIFF) both make it worse, so the alternation is not statement order. |
| func_80050A58 | unk_1C540 | 38/38 words, 18 DIFF | Rebuilds 17 pointers: `u32 c = D_8002E950[i]` (a u32 local, so the `lbu` value is not masked again), `c / 21` and `c % 21` by the 0x86186187 magic, the quotient as a `u8` index into the second argument's word table and the remainder times 12 added on. Two things pull against each other: writing the remainder as `c - d * 21` gets the target's ORDER (index chain before the multiply-by-21) but computes the 21 from the MASKED quotient, while `c % 21` gets the unmasked quotient the target multiplies but emits the chain the other way round. A local for the table address, an ARRAY_REF on a `u32 *` cast and a `(u8)` cast in the index expression all leave it at 22. |
| func_8005A1D0 | unk_1C540 | 35/39 words, 36 DIFF | Removes the slot whose first word matches, then closes the gap in the D_800869C0 pair table and calls func_80059DAC. The logic is right; the four missing words are STRENGTH REDUCTION: the target recomputes `i * 8 + base` and `(i + 1) * 8 + base` on every iteration of both loops, where loop.c here turns the inner one into a walked pointer (`addiu $a1, $a1, 8`). Indexing the symbol directly instead of a local base is 48 words, and two pointer locals for the source and destination rows is 42, so neither is the shape -- something stops loop.c reducing the giv at all. |
| func_800353F4 | unk_72A8 | 36/36 words, 2 DIFF | The shape is settled: `if (func_800352FC() == 0 || D_80082A70 != (u32) D_8006B230) { <the five-slot clear, func_80035378, func_80035208> return 1; } func_80035378(...); return 1;` -- the `||` is what gives the first test a branch INTO the body and the second one past it (as nested ifs, 22 DIFF), and the clear is func_80035348's `b[0x12 + i] = 0` verbatim. Needs `func_800352FC` changed to return s32 (it tail-calls func_80034FFC, so the code is the same). What is left is two words: the target fills the `beqz`'s delay slot with `lui %hi(D_80082A70)` and loads through it, where this hoists `lui %hi(D_8006B230)` instead. Both operand orders, `&D_8006B230[0]`, a `u8 *` type for D_80082A70 and a local for the call's result all leave it at 2. |
| func_8003666C | unk_72A8 | 34/34 words, 7 DIFF | func_800364EC's twin with the half-width offset: `s32 n = ((s[0x26] + 1) >> 1) - 1;` as its own local (inline, fold moves the `- 1` onto the other operand, 13 DIFF) and `(w - n) * 2` for the `sll 17`/`sra 16` pair. Every instruction is right and the word count matches; what is left is that the target reads `lh 0x22` BETWEEN `lbu 0x27` and `lbu 0x26` where this reads 0x26 second. All 24 permutations of the four statements were tried with perm.py (best 7 DIFF), as were a local for the halfword, a local for the shift alone, and the argument passed inline instead of through `t` (14-17 DIFF). It is sched1 choosing between three loads off one base, not statement order. |
| func_8004827C + func_8004D880 + func_800537D0 + func_80055CF4 + func_8003BD84 + func_80058A04 | unk_1AF38, unk_1C540 | exactly 1-3 words | **The same missing word three times: an `addu $rX, $rY, $zero` that local-alloc here coalesces away and the target keeps.** In func_8004827C and func_8004D880 it is the PARAMETER: the target copies the incoming $a0 into $v1 and uses $v1 for everything, spending $a0 on the outgoing argument, while this gives the parameter pseudo $a0 itself and copies it out where it is needed. func_8004D880 is otherwise exact, including the one-insn 16-bit subtractions (`u16 d = c - 0x824F;` -- as `s32` the constant will not fit an `addiu`) and the unmasked sum reused for the return (`return d + 0x30;` folds to one `addiu` only if `d` is NOT a local). Measured on func_8004D880: a local for the parameter, three separate difference locals, an explicit `& 0xFFFF` form and a `u32` parameter all coalesce the same way. func_80058A04's row below is the same shape on a call's return value. The three known CURES for this copy are all elsewhere in the source: re-dereference instead of caching (func_80039730), an explicit guard plus do-while on a loop bound (func_80059D1C), and a statement BETWEEN the producer and the copy (func_80048734). None of them applies to a parameter, which is what makes these the hard ones. func_800537D0 (the decimal formatter) is 3 DIFF of exactly this: everything else is right once `u8 *d = buf + n;` stops loop.c strength-reducing `buf[n]` into a walked pointer and the digit arm comes FIRST (`if (blank == 0)`), but the target computes the quotient into its own register, tests THAT, and copies it into `v` in the branch's delay slot. `q = v / 10; if (q == 0) ...; v = q;` in every order, a local copy of the parameter, and `if (v / 10 == 0)` with the division repeated all coalesce. func_80055CF4 is the same word on a SYMBOL's address: the target
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
| func_8005A56C | unk_1C540 | 23 DIFF | Two stack blocks selected by the first argument, with `(x << 16) >> 8` scaling. |
| func_80047CD8 | unk_1AF38 | 28/27 words, 20 DIFF | The target keeps the constant -1 in `$s1` across two calls and compares both results against it; this puts the first result there instead. An explicit `s32 m = -1;` local does not move it. |
| func_8004D880 | unk_1C540 | 28/28 words, 22 DIFF | A Shift-JIS to ASCII classifier: three `(u16)(c + K) < N` range tests, each returning `c + K + delta`. The target computes `c + K` ONCE into a register and adds the delta to it; every spelling folds the two constants (`c + 0x7DE1`) instead, and the target's copy of the raw parameter into $v1 says the parameter is not masked either. An s32 parameter, a u16 one, and an explicit `s32 t` local used twice were all measured. |
| func_8004E130 | unk_1C540 | 22/22 words, 9 DIFF | $gp group. Every instruction matches; only which of $v1/$a0 holds the masked index and which the table address. |
| func_8003C634 | unk_72A8 | 7 DIFF | $gp group. Reading the bound in the loop condition took it from 18 DIFF to 7; what is left is the register pair, as in func_8005C818. |
| func_80033D44 + func_80033D54 | unk_72A8 | BLOCKED, not a shape | Both need `%gp_rel(D_8008099C)`, and func_80034EF8 and func_8003C0C8 in the same carved unit reach that symbol ABSOLUTELY and are landed. maspsx's rewrite keys on the symbol for the whole unit, so one unit cannot give it both forms: these two want the unit split, or those two reverted. |
| func_80039DBC | unk_72A8 | 25/24 words, 18 DIFF | Calls through a table at +0x40 with four arguments from +0x48..+0x54; the fifth load reuses the table's register in the target and this spends an extra one. |

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
all matched first try once the block was in place. unk_72A8 and unk_1AF38 need
their own blocks for their own symbols (D_8008099C, D_8008152C and the rest).
