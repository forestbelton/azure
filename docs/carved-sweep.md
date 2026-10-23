# Sweeping the carved units

The four units carved on 2026-10-23 -- `unk_72A8` (294 functions), `unk_1C540`
(465), `unk_1AF38` (53) and `unk_301D4` (5) -- are being worked cheapest-first
by size. `tools/decomp/guard.py` has the live stub count.

## How to run the sweep

`verify.py` takes the **whole `.text`** of the scratch file as the function, so
one candidate per file. A batched file reports nonsense (a two-word function
came back "8 DIFF"). The driver scripts used for this live in the session
scratchpad and are trivial to rewrite: write `#include "common.h"` plus one
candidate to `zzone.c`, run `verify.py zzone.c <unit> <func>`, collect the
MATCHes, then replace each `INCLUDE_ASM("asm/nonmatchings/<unit>", <func>);`
line with the body and run `make check`.

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
* **An arithmetic constant binds to the term the source groups it with.**
  `(v & 0xF) + 0x7C80` then `+ ((v & 0x10) << 3)` must be split into a local;
  written as one expression the compiler folds the constant into the second
  term instead, and parentheses do not survive `fold`.

## Parked: tried and not matching

Each of these has the right instruction set and word count with the registers
or the order wrong, so they want the dumps rather than another guess.

| function | unit | state | tried |
|----------|------|-------|-------|
| func_8004AC18 | unk_1C540 | 9/9 words, 7 DIFF | `D_8007359C[i * 5 + 1]`, a local base pointer, and a 20-byte struct array. The target materialises the base FIRST (`lui` is insn 1) and keeps the base in `$v0` with the index chain in `$v1`; all three spellings compute the index first and swap those registers. |
| func_80053DCC | unk_1C540 | 9 DIFF | `D_8008480C = a;` as a scalar and as `D_8008480C[0]`, both with `func_80054D64(a)` after. The target has the `sh` in the `jal`'s delay slot. |
| func_80053DF0 | unk_1C540 | 9 DIFF | Same shape as func_80053DCC and fails identically, so these are ONE family, not two puzzles: store a halfword into a global, then call a function with the same argument. Solve either and both land. |
| func_80053DA8 | unk_1C540 | 6 DIFF | `return func_80055778(a)` with `s16`/`u16`/`s32` signatures. structs.h declares it `void(s32)`, and the target sign-extends the callee's result (`sll 16` / `sra 16`) before returning, so the result is used despite the `void`. |
| func_800368D0 | unk_72A8 | 10/10 words, 2 DIFF | The target loads `+0x44` (a pointer) before `+0x4A` (an s16 offset); every spelling loads the s16 first. Tried: one expression with 0x44 first, two locals with 0x44 declared first, and two locals with 0x4A first. Source order does not drive it. |
| func_8004B634 | unk_1C540 | 2 DIFF | `func_8003E4FC(6, D_80080B48, 0)` with the real signature `Unk80083968 *(u8, u8 *, s32)`. |
| func_8004EE90 + func_8004F52C | unk_1C540 | 12-14 DIFF | An identical PAIR, so one cause. `if (p != 0) { *(u16 *) (p + 0x1E) \|= 0x8000; D_800814A0 \|= 0x8000; }`. D_800814A0 must stay a scalar (see the declaration warning above), which costs the split-addressing form the target uses; 12 DIFF as an array, 14 as a scalar. |
| func_8005845C | unk_1C540 | 3 DIFF | `*(u32 *) &D_80084960[(a & 0xFF) * 0x9C] = b & 0xFF;`. The 0x9C stride comes out as the target's shift/add/subtract chain, so the arithmetic is right. Splitting the index into a local makes it WORSE (8 DIFF), so the single expression is closer. |
| func_80039730 + func_80039C44 + func_80039D10 | unk_72A8 | 10-11 words vs 12 | An identical TRIO (one cause). Fetch a byte through the cursor at +0x1C, advance the cursor, clear +0x27, install a callback at +0x10, store the byte at +0x26. The target spends TWELVE words including an extra `addu $v0, $v1, $zero` -- a register copy of the cursor, so the byte load uses the original and the increment uses the copy. Five spellings all come out at 10 or 11 words, i.e. gcc reuses one register: `*(*q)++` via a pointer-to-pointer local; an explicit `u8 *c` with the byte read before the store; the same with the byte read after the store (10 words, and the lui hoists); a separate `u8 *n = c + 1`; and post-increment through the cast lvalue. The extra copy is the whole puzzle and reordering does not buy it. |
| func_8004B4DC | unk_1C540 | 9 DIFF | A `while (1) { n++; if (p[0] & 0x80) break; p += 0xC; }` count loop. The target advances `p` in the branch's delay slot, so it is advanced on the exiting iteration too. |

## Deferred for addressing, not for shape

`func_8004DCE0`, `func_8004DDB4` and `func_80033D44` store through
`%gp_rel(D_8008xxxx)`, which these units' forced `-G0` cannot produce. They
belong with the addressing group in `docs/addressing-and-translation-units.md`.
