# Where the decompilation is, and what to do next

Written 2026-10-22. Read this first, then `docs/compiler-version.md`.

## State

**182 of 191 functions in the RESIDENT EXECUTABLE are C and the build is
byte-identical.** `make check` is green at HEAD. The remaining 9 are
`INCLUDE_ASM` stubs in `csrc/`. `python3 tools/decomp/guard.py` is the
authority on that count; this file has been wrong about it before.

**Both of the structural blockers that shaped most of this project are gone**,
and neither needed the thing it was being built towards:

* **Jump-table placement.** Each table is in `.rdata` under `.align 3`, and the
  leading `.align 3` is a no-op at section offset 0 -- which explains the whole
  region and means no C file has to be split. Placement is a `.rodata`
  subsegment per unit (with the leading dot), one splat segment per run, and
  `align: 4`. See the jump-table section of `docs/open-matching-problems.md`.
* **One object reached both `$gp`-relatively and absolutely.** A bare
  `__asm__(".sdata\n\t.size SYM,N\n\t.text");` puts the symbol in maspsx's
  table without defining anything, so maspsx rewrites loads and stores to
  `%gp_rel` while leaving address materialisation absolute -- and there is no
  linker placement to arrange, because the symbol stays undefined. See
  `docs/addressing-and-translation-units.md`. **The window definition that
  document spends most of its length deriving is no longer needed.**

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

## The 9 remaining stubs

| group | count | what it means |
|-------|-------|---------------|
| open near-misses | 2 | logic reproduced; a register or scheduling difference is left |
| walls, re-examined and recorded | 6 | do not reopen without a new idea |
| not yet written | 1 | func_8003E758, the last jump-table function |

**Nothing is structurally blocked any more, and every remaining stub is a real
function.** Each is either ordinary decompilation or a documented near-miss.

### Near-misses, with their files in `docs/near-misses/`

Sorted by how open they look, not by DIFF -- a wall at 8 words is further away
than an open one at 47.

| function | unit | words | DIFF | state |
|----------|------|-------|------|-------|
| func_8003F368 | unk_12A70 | 113 of 113 | 4 | open; two independent two-word gaps, local-alloc and reorg |
| func_80041E70 | unk_154BC | 270 of 270 | 1 | **WALL**, one word, owned by cse and argued completely |
| func_8003CCB0 | gprel_10308 | 272 of 272 | 29 | open; two spots that cancel in the word count |
| func_800453E0 | gprel_18B40 | 533 of 533 | 5 | **wall-ish**: the hoist, split, association and alias families are all swept |
| func_80045CC4 | unk_19434 | 436 of 437 | 47 | **wall-ish**: the same mask tension, 4 of the 47; the rest is argued separately |
| func_8004437C | gprel_17B7C | 100 of 100 | 8 | **WALL**, diagnosed at pseudo level |
| func_800407C0 | unk_12DEC | 178 of 178 | 62 | **WALL**, a real copy preference plus a 5x priority gap |
| func_80047054 | unk_19434 | | 30 | **WALL-ish**, argued unreachable at runtime |

**The four marked WALL or wall-ish have each been re-examined with the levers
that closed the others, and the negative results are recorded in their files.**
Do not reopen one without an idea that is not already on its list; three
sessions have now been spent re-deriving entries that were already written
down. The two "mask tension" entries share one cause and are 4 and 5 words
apart from matching -- solving it in either closes both.

**Read the near-miss file before touching one.** Each lists the shapes already
measured with their word costs, and most record a `search.py` sweep that found
nothing -- so the remaining gap is allocation or ordering, not source shape.
`python3 tools/decomp/nmcheck.py` compiles them all and checks each against its
own claimed count.

**What closed the last four of these was one mechanism**, and it is the first
thing to try on the rest: `regs_someone_prefers` makes `find_reg` avoid a
register that a *lower*-priority conflicting allocno prefers, so an allocno
allocated first can be pushed off the register it should have had. The cheapest
way to move a priority is a reference that costs no instruction -- splitting one
assignment into two statements that compile to the same single insn. See
`docs/matching-levers.md` section 3, which has the arithmetic. Two functions had
been written off at "2 DIFF, a deliberate price" and "21 DIFF, one coupled
permutation" before that.

### Not yet written (3)

| function | unit | instructions | note |
|----------|------|--------------|------|
| func_8003F2A4 | unk_12A70 | | was dual-mode `$gp`; use the size-record route |
| func_8003F368 | unk_12A70 | | ditto, and may not need it -- see below |
| func_8003E758 | unk_11F0C | 698 | the largest left; also dispatches through jtbl_8002D5C0 |

**func_8003F368 is worth a look before reaching for any directive.** Its
absolute accesses are at +1, +2 and +3 of objects whose +0 is `$gp`-relative,
and splat names every referenced address as its own symbol -- so "D_80081451
reached absolutely" may just be the original writing bytes through one
materialised base, `((u8 *) &D_80081450)[1]`, which is ordinary C needing no
per-access mode at all.

**func_8003E758 is the last jump-table function**, and the only one of the
three runs still raw. Two of the three are placed -- unk_14934's and
unk_154BC's -- so this one follows the same two-line spec change. It needs both
solved blockers at once: its table is the
28-entry jtbl_8002D5C0 at 0x8002D5C0, the first of the three runs, and
`unk_11F0C` contributes exactly that one table -- so when it matches, the
placement is a two-line spec change like the others.

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
