# Where the decompilation is, and what to do next

Written 2026-10-22. Read this first, then `docs/compiler-version.md`.

## State

**146 of 191 game functions are C and the build is byte-identical.** `make
check` is green at HEAD and the working tree is clean. The remaining 45 are
`INCLUDE_ASM` stubs in `csrc/`.

Run `make check` before trusting anything. Do **not** pipe it through `tail` or
`head`: that masks make's exit code, and a broken state was committed that way
once. Use

    make check > /tmp/check.log 2>&1; echo rc=$?

`asm/` is generated and gitignored, and splat only *adds* `nonmatchings`
listings, so the reference listing for a function you have already decompiled
stays until the next `make splat` (which does `rm -rf asm/nonmatchings` first).
Do not run `make splat` while anything else is reading those listings.

Before writing a function, read **`docs/matching-levers.md`**. It is the
project's record of how this compiler behaves: the pass order and what each
pass decides, the measured source shapes that move it with the word cost of
reverting each, the dead ends not to retry, and the toolchain traps. It exists
so nothing gets re-derived, so **add to it whenever a session learns
something** -- that has been the single biggest waste of effort here.

## The 45 remaining stubs, in four groups

Run `tools/decomp/guard.py` for the stub count (`grep -cE '^[A-Za-z_][A-Za-z0-9_ \*]*\bfunc_[0-9A-Fa-f]+\([^;]*\)\s*\{' csrc/*.c` for the C count).
The classification below is `grep -c '%gp_rel'` and `grep -c 'jtbl_'` against
each listing:

| group | count | what it means |
|-------|-------|---------------|
| parked | 9 | logic reproduced, within a few instructions; see `docs/open-matching-problems.md` |
| jtbl-blocked | 4 | dispatch through a jump table in the opening bin blob; needs a spec change |
| gp-blocked | 31 | need `$gp`-relative and absolute addressing in one translation unit |
| not a function | 1 | func_8003CAA8; see below |

**There is no longer an "easy" group.** Every stub with nothing structural in
its way has been decompiled. What is left is nine near-misses, two blocked
groups, and one splat artefact, so a fresh session's choice is: work a parked
function with the mechanism named below, or make one of the two spec decisions.

`func_8003CAA8` is in the stub list but is **not a function**: it is `main`'s
unreachable epilogue, which splat split off because it follows `main`'s
infinite loop. Fixing it is a spec change (extend the `main` asm subsegment to
0x102c4), not a matching problem.

### Parked (9)

All written up in `docs/open-matching-problems.md`, with the source that got
each one close in `docs/near-misses/`. **Read the near-miss file before
touching one**: each lists the shapes already measured and ruled out with their
word costs, and re-deriving those is where the time goes.

Ordered by how reachable they look:

- func_8003D0F0 -- 10 of 222, eight of them the four `(s8)` byte conversions.
  The near-miss header says this needs "a third blocking mechanism" because the
  two known ones each cost a `move` the arm has no room for. That mechanism has
  since been found and is `volatile` (section 7 of `docs/matching-levers.md`,
  including two load-bearing caveats about where the pointer is declared); it
  closed func_80044D24's identical gap first try with no extra register. This
  is the best target in the set.
- func_80045CC4 -- 8 of 437. Four are the same `(s8)` fold, so the same
  mechanism applies; the other four are a loop.c hoisting choice between its
  two masks, with a specific hypothesis in the near-miss file.
- func_80042A80 -- one bit, `srl` where the target has `sra`.
- func_80042900 -- one instruction, a dead volatile load that wants `$v1`.
- func_800423C0 -- every instruction matches; one register pair is swapped on a
  7% priority gap, the closest margin in the project. The near-miss file names
  the three numbers that would flip it.
- func_80041E70 -- 4 of 270, cse folding a known constant.
- func_80042560 -- about 56 words, the largest remaining gap. Its mixed
  addressing is solved and worth keeping; loop 1's store/load order and three
  `$s` registers are not, and part of its statement structure is still
  unmodelled.
- func_80047054 and func_800407C0 -- **argued unreachable from C**, both the
  same shared-register-preference case, with the arithmetic in the near-miss
  files. Do not start here; they are recorded so nobody re-opens them.

### Blocked by jump-table placement (4)

func_80041344, func_80041588 and func_80042BDC (unk_14934) and func_8003E758
(unk_11940) dispatch through tables that live in the opening `[0x800, bin]`
subsegment. `docs/open-matching-problems.md` has the full analysis, including
what the pad word at 0x8002D654 proves about where the original
translation-unit boundary was. The four things it needs are listed there; the
one that shapes the work is that all three unk_14934 functions have to land at
once, which means `csrc/unk_14934.c` must be split at func_80041588 first.
func_80041344's body is already written and verified as far as verify.py can
see it (145/145, with the caveat below about relocated words).

### Blocked by addressing (31)

The real obstacle, and the largest single group.
`docs/addressing-and-translation-units.md` has the analysis. Five of the 31
need only `$gp`-relative addressing (func_8003E4FC, func_8003F5E0,
func_80044618, func_80044698, func_800448BC) and the other 26 need both modes
in one compilation, which no per-file `-G` can give.

Two of the five are cheap: `func_8003F5E0` sits immediately after the
carved-out `gprel_12DD4`, so extending that subsegment by one function picks it
up (it needs renaming `unk_12DE0` to `unk_12DEC` and re-extracting), and
`func_80044618`/`func_80044698` are adjacent inside `unk_14934` and could be
carved out together. Both want a splat spec change, a re-extraction and a
`MASPSXFLAGS_G8` line in the Makefile. Both also mean `make splat`, so they
cannot be done while anything else is reading `asm/nonmatchings`.

The 26 need the documented long-term fix: each C file must *define* the globals
its functions reach `$gp`-relatively, rather than declaring them `extern`, so
maspsx can see the size. That means moving those objects out of the splat data
segments and into C. **This is a structural decision about where the project's
data lives and it has not been made** -- it was deliberately left for the
owner rather than started.

## How to work a function

1. Read `asm/nonmatchings/<unit>/<func>.s`.
2. Write a candidate into a `zz_*.c` scratch file in the project root
   (gitignored) containing `#include "common.h"`, any prototypes it needs, and
   just that function.
3. `python3 tools/decomp/verify.py zz_foo.c <unit> <func>` -- compiles with the
   real `ccpsx`, assembles through the project's maspsx + gas pipeline, and
   diffs every `.text` word against the reference listing, masking only
   relocated halfwords and 26-bit jump targets. It prints `MATCH` or
   `N DIFF` plus a side-by-side. This is the fast loop; it never touches
   `build/`.
4. When it says MATCH, replace the `INCLUDE_ASM` in `csrc/`, run
   `tools/decomp/guard.py` (nothing lost) and `make check` (byte-identical).
5. Commit, with the load-bearing source shapes named in the message and
   commented in the code.

Tools in `tools/decomp/` (they honour `AZURE_ROOT` and `AZURE_SCRATCH`):

| tool | use |
|------|-----|
| `verify.py zz.c UNIT FUNC` | the fast loop above; marks a relocated word whose addend differs from the listing with `?`, and diagnoses the wine case clash |
| `peek.sh UNIT FUNC` | print one already-decompiled function's asm |
| `sizes.py` | every C function whose compiled size differs from its target -- run this first after a failed `make check`, because one wrong size shifts the image and makes every later byte diff meaningless |
| `fdiff.py FUNC...` | built vs reference bytes side by side; only valid after a *successful* link, since a stale `build/slus_006.14` once produced a bogus MATCH |
| `guard.py` | no function was dropped from `csrc/` |
| `dump.sh file.c` | compile with gcc's RTL dumps and collect them |

`dump.sh` is the one to reach for when the logic is right and only registers or
the order of two adjacent instructions differ. `cc1psx` honours `-dS` (post
sched1), `-dR` (post sched2), `-dl` (local-alloc), `-dg` (global-alloc) and
`-dr` (initial RTL). `.sched` prints the ready list and each insn's priority
per cycle; `.greg` prints the allocation order, each allocno's conflicts and
its register preferences. Between them they settle a register question without
guessing -- that is how func_80047054's permutation and func_80042A80's one bit
were explained rather than fought.

## Things that cost time once and should not again

- **Declaration decides addressing.** A scalar or an array within `-G8` gets
  the assembler-expanded symbolic form; anything larger or an incomplete
  `extern u16 X[];` gets the compiler split. Check the declaration before
  suspecting the source shape.
- **splat's `%hi`/`%lo` rendering hides the difference** between a symbol
  reference and a plain constant address. A bare `lui reg, 0x8001` is printed
  as `%hi(SYM)` whenever splat can pair the register with a later `%lo`.
  Globals outside the loaded image (0x8002d000..0x8008875f) were constant
  addresses in the original, reached through a constant base pointer with the
  offset written *inside the index expression* -- see func_80043568 in
  `csrc/unk_14934.c` for the idiom. A symbol array with a *variable* index
  always materialises the whole address instead, so declaring such a symbol is
  a dead end (measured; see `docs/near-misses/func_800423C0.c`).
- **decomp-permuter cannot score a function that uses a constant address**:
  splat's `target.o` carries an `R_MIPS_LO16` relocation the constant form
  cannot produce, so the score never reaches zero. `verify.py` and `make check`
  are the authority.
- cc1psx runs under wine and its asm output has CRLF line endings. Strip the
  CR before matching labels.
- **`verify.py` compares only the upper 16 bits of any relocated word.** A
  `%lo(SYM)+N` where the target has `%lo(SYM)` passes, and so does a jump-table
  reference that points at the wrong place. It bit func_8003F80C and it is why
  a MATCH on any of the four switch functions would be misleading. `make check`
  is the only authority; treat a verify MATCH as "worth running make check on".
- **wine's filesystem is case-insensitive, so two scratch files whose names
  differ only in case are the same file to `ccpsx`.** With `zz_gA.s` already in
  the scratch directory, `ccpsx -o .../zz_ga.s` writes `zz_gA.s`, exits 0, and
  `verify.py` then dies with FileNotFoundError on a file the compiler "wrote".
  Name scratch variants so they differ by more than case.

## Open TODOs

`TODO.md` has the live queue. In short:

1. The nine parked near-misses, best-first as ordered above. Two of them
   (func_8003D0F0 and func_80045CC4) have a mechanism named and not yet
   applied, which makes them the cheapest real progress available.
2. The jump-table spec change, which unblocks about 1550 instructions. The
   analysis is complete; it needs `csrc/unk_14934.c` split in two at
   func_80041588, the opening bin blob carved, linker-script placement of three
   units' `.rodata`, and all three unk_14934 switch functions landed together.
3. A decision on the 31 addressing-blocked ones -- still the owner's call,
   since it is about where the project's data lives. The two cheap carve-outs
   above could be done first, independently of that decision.
4. Deferred type merges, all recorded where they matter: three scratchpad
   typedefs (Unk3DBD0SP/Unk463ECSP/Unk44D24SP), three POLY_FT4s
   (Unk463ECP/Unk3D0F0P/Unk44D24P), Unk800E3E48 into Unk42710, and the
   RotAverage4 prototype disagreement (`s32 *` against `u32 *`). Each is held
   back for a stated reason -- usually that retyping a field risks an existing
   match for a cosmetic gain -- so read the reason before doing one.

## Running agents on this

Fresh `general-purpose` agents pointed at `docs/matching-levers.md` are the
cheap way to work a parked function: measured at 212K tokens / 47 tool calls
for func_80044D24, against about 890K / 326 for a fork of the main session,
which inherits the whole conversation and pays for it on every turn. Give the
agent the function, the three documents to read first, the mechanism you think
it needs, and an explicit instruction not to run `make splat`, not to touch
`csrc/`, and not to commit -- integration is the parent's job.

Do not kill an agent that has been running a while: everything it has measured
is in its context and is lost. Send it a message asking it to report what it
has now, which caps its remaining turns and still yields the measurements.
