# Where the decompilation is, and what to do next

Written 2026-10-22. Read this first, then `docs/compiler-version.md`.

## State

**123 of 191 game functions are C and the build is byte-identical.** `make check`
is green at HEAD and the working tree is clean. The remaining 68 are
`INCLUDE_ASM` stubs in `csrc/`.

Run `make check` before trusting anything. Do **not** pipe it through `tail` or
`head`: that masks make's exit code, and a broken state was committed that way
once. Use

    make check > /tmp/check.log 2>&1; echo rc=$?

`asm/` is generated and gitignored, and splat only emits a `nonmatchings`
listing for a function that is *currently* a stub. So decompiling a function
deletes its reference listing on the next build. **Read the listing before you
replace the stub**, and if you need it back, revert the stub and rebuild.

## The 68 remaining stubs, in three groups

Run `tools/decomp/guard.py` for the count and the classification below by hand
(`grep -c '%gp_rel'` against each listing):

| group | count | what it means |
|-------|-------|---------------|
| workable | 29 | nothing structural in the way; just decompile them |
| parked | 11 | logic reproduced, within a few instructions; see below |
| blocked | 32 | need `$gp`-relative and absolute addressing in one translation unit |

### Workable (29)

Smallest first, by instruction count: func_8004425C (72), func_80046AFC (73),
func_8004068C (77), func_8004491C (77), func_80046C20 (81), func_80046E38 (84),
func_8003E39C (88), func_80041CBC (91), func_80043FD0 (93), func_80044724 (102),
func_8003D5A4 (111), func_80046884 (118), func_80042710 (124), func_80040CBC
(145), func_80041344 (145), func_8003DBD0 (162), func_80041588 (164),
func_800407C0 (178), func_8003D0F0 (222), func_80041E70 (270), func_800463EC
(294), func_80044D24 (379), func_80045CC4 (437), func_80042BDC (543), plus the
larger ones in `unk_11940` and `unk_102A8`.

func_8004425C is already analysed and ready to write: it indexes
`D_8006E61C` with stride 0xC, derives `s3 = (s16) (e->unk_00 - 1)`, indexes
`D_80080AF0` by that, returns early when the byte there already equals
`arg0 + 1`, and otherwise runs a fixed call sequence ending in a
`do { } while (func_8005405C(e->unk_00) != 1)` spin. Note it reuses `$s0` for
the constant 1 after the pointer is dead.

`func_8003CAA8` is in the stub list but is **not a function**: it is `main`'s
unreachable epilogue, which splat split off because it follows `main`'s
infinite loop. Fixing it is a spec change (extend the `main` asm subsegment to
0x102c4), not a matching problem.

### Parked (11)

All in `docs/open-matching-problems.md`, with the source that got each one
close in `docs/near-misses/`. Several are one instruction or one *bit* away:

- func_80042A80 -- one bit, `srl` where the target has `sra`.
- func_80042900 -- one instruction, a dead volatile load that wants `$v1`.
- func_80047054 -- every instruction identical, five registers permuted.
- func_800423C0 -- four instructions, all about how three `lui 0x8001`s are
  materialised.

Read the near-miss file before touching one. Each lists the shapes already
measured and ruled out, and re-deriving those is where the time goes.

### Blocked (32)

The real obstacle, and the largest single group. `docs/addressing-and-translation-units.md`
has the analysis. Five of the 32 need only `$gp`-relative addressing
(func_8003E4FC, func_8003F5E0, func_80044618, func_80044698, func_800448BC) and
the other 27 need both modes in one compilation, which no per-file `-G` can
give.

Two of the five are cheap: `func_8003F5E0` sits immediately after the
carved-out `gprel_12DD4`, so extending that subsegment by one function picks it
up (it needs renaming `unk_12DE0` to `unk_12DEC` and re-extracting), and
`func_80044618`/`func_80044698` are adjacent inside `unk_14934` and could be
carved out together. Both want a splat spec change, a re-extraction and a
`MASPSXFLAGS_G8` line in the Makefile.

The 27 need the documented long-term fix: each C file must *define* the globals
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
| `verify.py zz.c UNIT FUNC` | the fast loop above |
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

`TODO.md` still lists only the unused-symbol scanner. The real queue is the
three groups above, in this order: the 29 workable ones, then a decision on the
27 blocked ones, then the parked near-misses as a background activity.
