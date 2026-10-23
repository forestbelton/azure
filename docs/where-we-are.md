# Where the decompilation is, and what to do next

Written 2026-10-22. Read this first, then `docs/compiler-version.md`.

## State

**162 of 191 functions in the RESIDENT EXECUTABLE are C and the build is
byte-identical.** `make check` is green at HEAD. The remaining 29 are
`INCLUDE_ASM` stubs in `csrc/`.

### Scope: this is the engine, not the game

**Read this before quoting the number above.** "191 functions" is every
function in `data/slus_006.14`'s own code, which is 43 KB of a 512 KB
executable. It is *not* the game. Most of Azure Dreams' code lives in the
overlays, and essentially none of it is analysed:

| file | size | code-bearing (4 KB blocks, all distinct) |
|------|------|------------------------------------------|
| `slus_006.14` | 0.5 MB | ~216 KB -- 43 KB of game code plus PSY-Q library and crt0 |
| `main.bin` | 2.5 MB | ~80 KB |
| `town.bin` | 16.1 MB | ~1.0 MB |
| `dungeon.bin` | 26.5 MB | ~7.3 MB |

Measured by counting 4 KB blocks holding at least two `addiu $sp,$sp,-N`
prologues *and* two `jr $ra`, then deduplicating by block hash: nothing
repeats, so the volume is real. A sample from `dungeon.bin` at file offset
0xA6000 disassembles to obvious compiler output -- the same
`sll 16`/`sra 16`/`slti` clamp idiom, argument-register moves and filled delay
slots we match here. Treat the figures as order-of-magnitude: the heuristic can
count packed data and can miss sparse code, and `dungeon.bin` is certainly an
archive of many separately loaded entries rather than one image.

So the overlays hold roughly 8.4 MB of code against the executable's 216 KB.
`make check` verifies **only** `slus_006.14`; `dungeon.bin` has no splat spec at
all, `town.bin` has one `bin` blob covering all 16 MB, and `main.bin`'s 28
subsegments are all `bin`.

What the resident engine being nearly finished *does* mean: overlay code calls
into it, and the 23 called-but-undefined functions above 0x8008875F are the
engine calling out, so the engine genuinely had to come first. But the honest
framing is "the engine is nearly done, the game content is unmapped", not "the
game is 85% decompiled". See `docs/overlays.md`.

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

## The 32 remaining stubs

| group | count | what it means |
|-------|-------|---------------|
| **workable now** | 7 | in a carved `-G8` unit; nothing structural left, just decompile them |
| gp-blocked | 14 | need their `$gp` cluster carved first; see the recipe below |
| jtbl-blocked | 4 | dispatch through a jump table in the opening bin blob |
| parked | 6 | logic reproduced, within a few words; `docs/open-matching-problems.md` |
| not a function | 1 | func_8003CAA8, `main`'s unreachable epilogue |

**There is a workable queue again**, which there was not for most of the
addressing work. These seven sit in units already built with
`MASPSXFLAGS_G8`, so they need no further structural change:

| function | unit | instructions |
|----------|------|--------------|
| func_80041AE4 | gprel_152E4 | 45 |
| func_80041BE4 | gprel_152E4 | 32 |
| func_80043D04 | gprel_17504 | 45 |
| func_80044618 | gprel_17B7C | 32 |
| func_80044698 | gprel_17B7C | 35 |
| func_8004450C | gprel_17B7C | 53 |
| func_8004437C | gprel_17B7C | 100 |

**Two things to know before touching one.** Pass `--g8` to `verify.py` (these
units are not built at the project default `-G0`, and without the flag you
model the wrong assembler). And audit the symbols the function reaches
*absolutely*: at `-G8` gas makes any small **undefined** symbol
`$gp`-relative, so one living inside gp +/- 0x8000 must be declared in the
compiler-split form or it costs a word. D_80081480 and D_8008148C are the live
cases, and they must **not** be changed in `include/structs.h` -- already-matching
`-G0` functions depend on the scalar spelling -- so the carved unit gets its own
declaration through a `#define` shim. The unit headers record which hazards
were already checked.

### Blocked by addressing (14)

Each needs its `$gp` cluster carved out first, which is now a recipe rather
than a research problem: `docs/addressing-and-translation-units.md` has it, and
`tools/decomp/carve.py` does the file surgery. By unit: unk_102A8 (func_8003CB08,
func_8003CCB0, func_8003D92C), unk_11940 (func_8003E34C, func_8003E4FC),
unk_12A70 (func_8003F2A4, func_8003F368, func_8003F5AC), unk_12DEC
(func_8003F624), unk_1811C (func_80045340, func_800453E0, func_8004713C,
func_80047338, func_80047468).

**func_8003F624 is the exception** and cannot be carved: it needs `%gp_rel` for
a store to D_800814D4 *and* an absolute `lui %hi`/`addiu %lo` pair for
`&D_800814D4` in the same function. `-G8` makes the address-of `$gp`-relative
too (18 differing words). maspsx's rewrite fires only on loads and stores, so
only the `.sdata` definition route gives both -- and that still wants linker
placement, which splat's legacy generation cannot express.

### Blocked by jump-table placement (4)

func_80041344 and func_80041588 (unk_14934), func_80042BDC (unk_154BC) and
func_8003E758 (unk_11940) dispatch through tables in the opening `[0x800, bin]`
subsegment. `docs/open-matching-problems.md` has the analysis, the recovered
case orders and the entry counts. Three of the four are already written and
close -- 2, 14 and 21 differing words -- so the remaining work is the splat and
linker-script change, not the matching.

### Parked (6)

All in `docs/open-matching-problems.md` with their sources in
`docs/near-misses/`. **Read the near-miss file before touching one**: each
lists the shapes already measured with their word costs, and four of them also
record a `search.py` sweep that found nothing, so the remaining gap is
allocation or ordering rather than source shape.

## How to work a function

0. `python3 tools/decomp/annotate.py <unit> <func>` -- **do this first.** It
   reads the target listing and prints every lever whose signature is present,
   with the sites, the doc section, the measured word cost of getting it wrong,
   the source shapes to try cheapest-first, and the shapes already measured
   *not* to work. It replaces reading `docs/matching-levers.md` end to end
   (9,000 words) with reading the few entries that can apply. It decides
   nothing -- see the note below on why that is deliberate.
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
3b. When the logic is right and only shapes are wrong, hand it to
   `python3 tools/decomp/search.py zz_foo.c <unit> <func>` rather than
   iterating by hand: a verify cycle is 0.7s, so a few hundred variants is
   minutes of compute and no thinking. Add `--only=width,cast,dup` (the
   families `annotate.py` implicated) once the function is big enough that one
   greedy pass is slow. It will not rescue wrong logic, and a candidate it
   flags as closing the word count *at the cost of* differing words wants
   judging by hand -- that is the shape of a semantic accident, not a lever.
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
| `annotate.py UNIT FUNC` | which levers this function's listing can possibly need, with costs and ruled-out shapes; `--sites` to print the matching instructions, `--self-test` to check every signature still fires on a function known to need it |
| `search.py BASE.c UNIT FUNC` | sweep the local source transforms this compiler is sensitive to -- declared widths, `(s16)` casts, splitting a statement in two, `<` against `>`, the `& 1` mask -- scoring each with verify.py. Greedy by default, `--sites` to list the candidate changes, `--exhaustive` when there are few. Prints what each accepted change was worth, which is the table a near-miss header wants |
| `nmcheck.py` | every `docs/near-misses/*.c` still compiles and still agrees with the word/diff count in its own header |

**When a function is one or two words short, compare the internal branches'
immediate fields against the target's**, rather than reading the diff. Every
branch whose span contains the missing word is off by exactly that count, so
the first branch whose delta changes brackets the defect. That localised
func_80042BDC's missing word to one arm of a 180-instruction `case` in a single
verify run, where the differing-word count said nothing useful at all -- it had
200 of 543 words "differing" purely from the shift.

**`annotate.py` deliberately does not choose.** It prints candidates, each
citing its measurement, because the two worst detours in this project came from
inheriting a confident claim rather than from failing to find one -- the "two
sign extensions" story cost two sessions, and "argument preferences are
unreachable" parked two functions on a reason that turned out to be half true.
A tool that ranked confidently would reproduce that failure faster. If you add
an entry, it needs a `cost` naming a real function and word count, and any
shape you measured as useless belongs in its `negative:` list, which is the
most valuable field in the file.

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

1. The six parked near-misses, best-first as ordered above. Two of them
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
