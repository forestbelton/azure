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
| func_800368D0 | unk_72A8 | 10/10 words, 2 DIFF | The target loads `+0x44` (a pointer) before `+0x4A` (an s16 offset); every spelling loads the s16 first. Tried: one expression with 0x44 first, two locals with 0x44 declared first, and two locals with 0x4A first. Source order does not drive it -- it is a sched1 priority tie between two loads, so it wants `dump.sh`'s `.sched` ready lists, not more spellings. |
| func_8004B634 | unk_1C540 | 2 DIFF | `func_8003E4FC(6, D_80080B48, 0)` with the real signature `Unk80083968 *(u8, u8 *, s32)`. |
| func_80049DE8 | unk_1C540 | 6 DIFF | A modular add: `a += d; if (d > 0) { if (a >= m) a -= m; } else { if (a < 0) a += m; }`. The `a += d` lands in the first branch's delay slot so it happens on both paths, which this source gives; the rest of the control flow is close but not exact. |
| func_80033AA8 | unk_72A8 | 16/16 words, 7 DIFF | The bitmap set: two early returns (NOT `id != 0 && id != 1`, which combines into one `sltiu`), then the out-of-image constant base as a TWO-SET pointer (`p = (u8 *) 0x80010000; p += (id >> 5) * 4;`), which is what keeps 0x2D70 in the load's displacement instead of folding it into a `lui`/`ori` of 0x80012D70. Every opcode, immediate and displacement now matches; what is left is that the zero-extended id takes $a0 and the lui base $v0 in the target, and the other way round here. `s32` with an explicit `(u16)` local does not move it. |
| func_80034EC4 | unk_72A8 | 7 DIFF | Sets a byte, points two fields at `p + 8`, then copies two words out of D_8006ADBC. |

Cleared from this table on 2026-10-23, all of them by a DECLARATION rather
than a source shape: func_8004AC18, func_8005845C, func_80053DA8,
func_80053DCC, func_80053DF0, func_8004B4DC, func_8004EE90, func_8004F52C and
the four func_8005C8xx readers. The lesson is the one the near-miss README
already states -- the measurements held, the context around them moved -- and
here the context was always which declaration cc1psx compiled the symbol
against.

## Deferred for addressing, not for shape

`func_8004DCE0`, `func_8004DDB4` and `func_80033D44` store through
`%gp_rel(D_8008xxxx)`, which these units' forced `-G0` cannot produce. They
belong with the addressing group in `docs/addressing-and-translation-units.md`.
