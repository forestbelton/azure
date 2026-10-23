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

`tools/decomp/landlate.py batch_ok.py` lands the candidates whose only
collision is a prototype BELOW their own stub -- a legal
prototype-after-definition, which the sweep creates for itself every time a
landed function declares a callee that is still a stub -- and writes
`batch_ok_rest.py` for `sweep.py land`. It asserts the position rather than
assuming it.

**Let `make check`'s exit code gate the commit.** Writing
`make check > log; echo rc=$?; git commit` puts an always-succeeding `echo`
between them, so an `&&` guards nothing -- that committed a broken tree once.

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
| func_80039730 + func_80039C44 + func_80039D10 | unk_72A8 | 12/12 words, 6 DIFF | An identical TRIO (one cause), and func_80039578 + func_800395B0 are the same cause with two bytes instead of one. `u8 *c = (*q)++;` then `u8 b = *c;` as two statements gets the word count and the instruction set exactly, including the register copy that five earlier spellings could not produce. What is left is that the copy is the mirror image of the target's: the target increments the COPY in place and dereferences the ORIGINAL, which gives the incremented quantity 4 references against 3 and so `$v0`. matching-levers section 8 states the open question precisely. `search.py` finds nothing from this base. |
| func_800368D0 + func_80035C80 | unk_72A8 | 10/10 and 25/25 words, 2 DIFF each | The target loads `+0x44` (a pointer) before `+0x4A` (an s16 offset); every spelling loads the s16 first. Tried: one expression with 0x44 first, two locals in either order, and a `u8 **` local for the pointer. Source order does not drive it. **The useful datum is func_80035CE4, which LANDED with the identical expression** -- so the two loads' order is decided by what else is in the block: in func_80035CE4 a callback store precedes them and gives sched1 something to place first. Moving that store ahead of the pair in func_80035C80 makes it 23 DIFF, so it is not simply the statement order either. This wants `dump.sh`'s `.sched` ready lists. |
| func_8004B634 | unk_1C540 | 2 DIFF | `func_8003E4FC(6, D_80080B48, 0)` with the real signature `Unk80083968 *(u8, u8 *, s32)`. |
| func_80049DE8 | unk_1C540 | 6 DIFF | A modular add: `a += d; if (d > 0) { if (a >= m) a -= m; } else { if (a < 0) a += m; }`. The `a += d` lands in the first branch's delay slot so it happens on both paths, which this source gives; the rest of the control flow is close but not exact. |
| func_80033AA8 | unk_72A8 | 16/16 words, 7 DIFF | The bitmap set: two early returns (NOT `id != 0 && id != 1`, which combines into one `sltiu`), then the out-of-image constant base as a TWO-SET pointer (`p = (u8 *) 0x80010000; p += (id >> 5) * 4;`), which is what keeps 0x2D70 in the load's displacement instead of folding it into a `lui`/`ori` of 0x80012D70. Every opcode, immediate and displacement now matches; what is left is that the zero-extended id takes $a0 and the lui base $v0 in the target, and the other way round here. `s32` with an explicit `(u16)` local does not move it. |
| func_80033AE8 + func_80033B2C | unk_72A8 | 7 and 20 DIFF | The bitmap clear and test, func_80033AA8's family. Same two levers (two early returns, a two-set constant base) and the same unresolved register assignment. |
| func_8003C5A4 | unk_72A8 | 19/19 words, 13 DIFF | A scan of the -1-terminated byte table D_8006B1EC for a value, returning the index. Every opcode and immediate matches; the pointer and the counter hold each other's registers. |
| func_8004A6C0 + func_8004A8D8 | unk_1C540 | 18/16 words, 13 DIFF | A two-halfword search over 0x14 entries at the constant base 0x80010000 with the fields at +0x3564/+0x3566. The target's base register is the induction variable, advanced by 4, with both offsets staying as displacements. Measured: `u8 *p = (u8 *) 0x80010000` outside the loop plus `p += 4` strength-reduces but loop.c BIASES the initial value to the first field used (`ori 0x3566`, `lh -2`, 20 DIFF) -- a constant initial value is foldable, which is the whole problem. Assigning the base inside the loop body with two sets stops the bias and loses the strength reduction instead (13 DIFF, the base and index recomputed per iteration). `extern s16 D_80013564[]` indexed by the counter materialises the whole address and makes TWO induction variables (20 DIFF) -- the dead end `docs/near-misses/func_800423C0.c` already records, now confirmed here. What would produce both at once is a `lo_sum` as the induction variable, since nothing folds a constant into one. |
| func_8003B7C8 | unk_72A8 | 21/21 words, 9 DIFF | The same shape, and the LOOP is already exact -- unbiased base, 0xAC4 as the displacement, `addiu 0x54` in the branch delay slot. The only gap is that the `lui` for the base is hoisted ahead of the table lookup where the target has it in the loop's preheader, after the `blez`. An explicit `if (n > 0)` guard around the loop and the index form both make it worse (13 and 11). |
| func_8004AFE8 | unk_1C540 | 14 DIFF | Counts entries whose `(*p)[1]` equals the first one's, with the loop's `p != 0` test at the bottom and a guard at the top. |
| func_8004CAA0 | unk_1C540 | not attempted | Switches `$sp` to the scratchpad at 0x1F8003FC around a call, saving the old stack pointer at that address. No C expression produces that; it wants inline asm, like func_8003E758's. |
| func_8004D75C | unk_1C540 | 19 DIFF | A 0x44-byte block move (the odd word first, then sixteen words in a four-word loop -- so the shape is right) followed by `*(u32 *) s = 0;`. |
| func_80048FBC | unk_1C540 | 18/18 words, 7 DIFF | Only which of `$s0`/`$s1` holds the pointer and which the index. |
| func_8005405C | unk_1C540 | 13 DIFF | Two mask tests against D_800847D0's two words with the shift amount from an s16 parameter. Three arms assigning 0/1/0 keep the branches that `if (x) return 1; return 0;` store-flags into one `sltu`, which is most of it; the remainder is registers and the return conversion (and structs.h's `s16` return is load-bearing for two callers -- `s32` breaks them). |
| func_80055778 | unk_1C540 | 17/17 words, 7 DIFF | Push onto the D_80084778 ring. One variable assigned in both arms and returned once is the shape; the target computes it straight into `$v0` where this gives `$v1` and a copy. |
| func_80057A50 | unk_1C540 | 17/17 words, 2 DIFF | The two address materialisations interleave in the target (`lui`, `lui`, `addiu`, `lh`) and not here. A local for the index does not move it. |
| func_80059D1C + func_80059D60 + func_8005C818 | unk_1C540 | 14-18 DIFF | Sweeps over D_80085FA8's 0x50-byte entries. The struct spelling fixes the displacements (a byte array biases the induction variable), and what is left is the copy of the loop bound into `$a2` plus the register cascade behind it. |
| func_80047738 + func_8004827C | unk_1AF38 | 17 and 12 DIFF | Both shapes are close; func_8004827C's target copies the parameter into `$v0` before using it, which no spelling here reproduces. |
| func_8003C890 | unk_72A8 | 20/21 words, 13 DIFF | Two early exits that share one `return 0`, which the target reaches by fall-through from the second test and by branch from the first. `if (b >= 0) { t = f(); if (t != 0) return ...; } return 0;` and the two-early-return form both come out with the second branch inverted. |
| func_8004A784 | unk_1C540 | 23 DIFF | `(u8 *) 0x800133E8 + i * 19` guarded by `i < 0x14`, where i comes from func_8004A6C0 -- and 20 entries of 19 bytes from 0x800133E8 is exactly the 0x80013564 that func_8004A6C0 scans, so the two tables are one structure. Depends on the same constant-base handling as func_8004A6C0. |
| func_8005C668 | unk_1C540 | 11 DIFF | Builds a 0x40-byte block on the stack (a word from D_80073740, the constant 3, two halfwords) and passes its address. |
| func_800489F4 | unk_1AF38 | not measured | func_80047738 plus an `ori 0x20`; park it with that one. |
| func_80049E1C | unk_1C540 | 18 DIFF | A clamp: `x = a + d`, then `x` against `n - 1` when d >= 0 and against 0 when it is negative. Same family as func_80049DE8. |
| func_800563B0 + func_80056DB4 + func_8005C668 + func_8005C6B4 | unk_1C540 | 4-14 DIFF | Four functions that fill a parameter block (three at D_80084918, one on the stack) and pass its address. The blocks' contents are right; in each, cc1psx schedules the D_80073740 lookup earlier than the target does. Reading it into a local first, in the middle, or last does not move it -- the load has the longest chain. func_80056DB4 is the closest at 4 DIFF, where only the two halfword zeros and the load are swapped. |
| func_80037030 | unk_72A8 | 24/24 words, 7 DIFF | The argument setup and the `sw $zero` before the call are in the other order, and reorg picks a different insn for the jal's delay slot. |
| func_800491F4 | unk_1C540 | not measured in isolation | A list walk that threads each entry's +0xC to the previous one. |
| func_8005A56C | unk_1C540 | 23 DIFF | Two stack blocks selected by the first argument, with `(x << 16) >> 8` scaling. |
| func_8003C24C + func_8003C2F8 | unk_72A8 | 26/27 words, 11 DIFF | A pair. `u8 *b = (u8 *) D_80082D58;` with `*(u8 **) (b - 0x14)` is what stops the -0x14 folding into the load's address (26 DIFF -> 11), which is the whole structure; what is left is that the target computes `%hi(D_80082D58)` once and shares it between the base pointer and the final store's split form, where cse here folds the store onto the base pointer instead. |
| func_80047CD8 | unk_1AF38 | 28/27 words, 20 DIFF | The target keeps the constant -1 in `$s1` across two calls and compares both results against it; this puts the first result there instead. An explicit `s32 m = -1;` local does not move it. |
| func_8004D880 | unk_1C540 | 28/28 words, 22 DIFF | A Shift-JIS to ASCII classifier: three `(u16)(c + K) < N` range tests, each returning `c + K + delta`. The target computes `c + K` ONCE into a register and adds the delta to it; every spelling folds the two constants (`c + 0x7DE1`) instead, and the target's copy of the raw parameter into $v1 says the parameter is not masked either. An s32 parameter, a u16 one, and an explicit `s32 t` local used twice were all measured. |
| func_8004EAC4 | unk_1C540 | 27/27 words, 13 DIFF | Only which of $s0/$s1/$s2 holds each of the three parameters. |
| func_8003C06C | unk_72A8 | 14/14 words, 4 DIFF | Picks between two tables and indexes the winner. The target's two address materialisations each reuse ONE register (`lui $v1` then `addiu $v1, $v1`); this gives two (`lui $v0` then `addiu $v1, $v0`), which is the same split form with the %hi and the lo_sum not coalesced. Sized declarations (the `la` form) are worse at 14, a `?:` at 16, and initialising the pointer to one table and overwriting it in the if at 9. |
| func_80034EC4 | unk_72A8 | 7 DIFF | Sets a byte, points two fields at `p + 8`, then copies two words out of D_8006ADBC. |

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
  the unit already has it.

func_8004DCE0, func_8004DDB4, func_80050CAC, func_8004AE98 and func_80048DA0
all matched first try once the block was in place. unk_72A8 and unk_1AF38 need
their own blocks for their own symbols (D_8008099C, D_8008152C and the rest).
