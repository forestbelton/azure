# Where the decompilation is, and what to do next

Written 2026-10-23. Read this first, then `docs/compiler-version.md`.

## State

**186 of 1008 functions in the RESIDENT EXECUTABLE are C and the build is
byte-identical.** `make check` is green at HEAD, and green after a full
`make splat`. The remaining 7 are
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

### And most of slus_006.14 itself is not accounted for either

The scope caveat above is about the overlays. There is a second one inside the
resident executable, and it is easy to miss because splat's summary line
("Split 98 KB (18.72%) in defined segments") does not break the remainder down.
Measured in 4 KB chunks over every `bin` subsegment, classifying each by zero
density, Shift-JIS density, MIPS prologue/`jr $ra` count and Shannon entropy:

| what | bytes | % of the 512 KB |
|------|-------|-----------------|
| high-entropy data (entropy 7.98, almost certainly packed assets) | 175,552 | 33.5% |
| **code -- PSY-Q library, unidentified** | **123,484** | **23.6%** |
| structured data | 95,180 | 18.2% |
| C translation units (incl. the 5 remaining stubs inside them) | 44,112 | 8.4% |
| identified library/asm (`psyq/*`, `main`) | 51,852 | 9.9% |
| Shift-JIS text | 30,144 | 5.7% |
| header, data, .rodata, split strings | ~4,000 | 0.7% |

**So about 120 KB of this file is code nobody has identified -- roughly three
times the volume of the game code that is decompiled.** It is not game code: it
sits in the library regions, mostly 0x0072A8-0x0101F8 (36 KB, 348 prologues,
immediately *before* `main`, so crt0 plus linked library) and
0x01C540-0x0301A8 (81 KB, 625 prologues, interleaved with the `libsnd` and
`libspu` modules splat already places by content). The 51 KB already attributed
as `asm, psyq/*` is where that identification succeeded.

**And it is not the mechanical gap it looks like.** Investigated 2026-10-23.
`azure libmatch psyq` confirms the library set is the right release -- 162 of
163 already-placed modules match exactly, mean fuzzy 0.998 -- yet
`azure libscan psyq` over both large code blocks (0x72a8-0x101f8 and
0x1c540-0x301a8) locates **zero** of 1257 modules examined, at `--min-invariant`
4 and 12 alike. The only hits are tiny stubs matching ambiguously at many sites
on 4-8 invariant words. So this code is not any module of the twenty libraries
in `psyq/lib`, although those are provably the ones the game linked against.

### The code accounting is complete: 1008 functions, 186 of them C

**Settled 2026-10-23, and this is the authoritative count.** It was revised
upward four times in one afternoon as blobs were carved -- 191, then ~421 from a
`jal` estimate, then 784, then 1008 -- so the number is stated here once, with
the survey that makes it final.

| unit | file range | functions |
|------|-----------|-----------|
| previously typed | various | 191 (186 C, 5 stubs) |
| `unk_72A8` | 0x072a8-0x101f8 | 294 |
| `unk_1C540` | 0x1c540-0x301a8 | 465 |
| `unk_1AF38` | 0x1af38-0x1c520 | 53 |
| `unk_301D4` | 0x301d4-0x30290 | 5 |
| **total** | | **1008** |

`guard.py` is the authority and reads 822 stubs. splat now reports `c` at 167 KB
(32.0%) and `asm` at 51 KB (9.9%); `make check` is byte-identical at e6bfbb9
after every carve.

**Why this is final rather than another revision.** Every remaining `bin` range
was surveyed for `addiu $sp` prologues, `jr $ra` returns, and how many `jal`
instructions target it from outside. Exactly one was code-bearing (188 bytes),
and it is now carved. The rest is data and matches the entropy profiles already
recorded: 25 KB of Shift-JIS text at 0x800 and 0xefc, ~5.5 KB of data between
0x31cc4 and 0x363a8, 90 KB of structured data at 0x3e01c, and 180 KB of
packed assets at 0x54240 (entropy 7.98). The 80 `jal` targets landing in that
last region are the engine calling **out-of-image overlay addresses**, which
`docs/overlays.md` covers.

So every code-bearing byte of `slus_006.14` now sits in a typed subsegment, and
what is left as "unknown" is data.

### These 817 functions are the game's own code

Resolved 2026-10-23, and it reframes the project. Both blocks are now carved as
C subsegments (`unk_72A8` and `unk_1C540`), `make check` is byte-identical, and
splat's own disassembly counts them:

| block | file range | vram | functions |
|-------|-----------|------|-----------|
| A | 0x072a8-0x101f8 | 0x80033aa8-0x8003c9f8 | **294** |
| B | 0x1c540-0x301a8 | 0x80048d40-0x8005c9a8 | **465** |

So 759 functions, plus the 191 previously typed, is **784** -- the headline
figure was a quarter of the executable's real function count. An earlier `jal`
scan estimated 421; it counted only direct-call TARGETS and so missed everything
reached indirectly.

**They are the game's own code, not SDK modules.** The decisive evidence is
what they CALL: block A calls `AddPrim`, `GetTPage`, `ClearImage`,
`RotTransPers4`, `RotMatrix`, `CompMatrix`, `PushMatrix`/`PopMatrix`,
`ScaleMatrix`, `LoadImage`, `VSync`; block B calls `DrawSync`, `SetSemiTrans`,
`SetRotMatrix`/`SetTransMatrix`, plus `bzero`, `memcpy`, `strcpy`, `strncpy`,
`strrchr` and `EnterCriticalSection`/`ExitCriticalSection`. That is libgpu,
libgte, libc and libapi being *consumed*, and it is the same vocabulary as the
engine code already decompiled -- func_800453E0 and func_80045CC4 call
`RotTransPers4` and `RotAverage4` and link primitives with the `addPrim`
bitfield store. Supporting evidence, gathered before the carve:
Both blocks are cohesive -- the overwhelming majority of calls into them come
from within themselves -- and both are called by the game code we have already
decompiled.

**Three independent reasons they are not library modules we could just place:**

1. `azure libmatch psyq` says the library set is right (162/163 exact, mean
   fuzzy 0.998), so a miss is not a wrong-release artifact.
2. `azure libscan psyq` locates **zero** of 1257 modules in either block.
3. Byte-exact search: the first function at 0x80033aa8 is a bitmap set/clear/
   test triple (reject id 0 and 1, `id >> 5` for the word, `id & 31` for the
   bit, table at 0x80012D70 -- one of the out-of-image constant addresses this
   project already documents as the game's own idiom). Its relocation-free
   instruction run appears **twice in the executable and in none of the twenty
   `.lib` files**, and content matching demonstrably works here, since that is
   how libmatch placed 162 modules.

Block A begins immediately after `psyq/libsn/SNMAIN` ends -- `__SN_ENTRY_POINT`,
`__main` and `__do_global_dtors` are the last symbols before it -- so it sits
exactly where a PSY-Q link puts the first linked objects, which is consistent
with it being the game's own earliest objects rather than libraries.

**What is not settled:** whether this is the game's own code or a third-party /
engine library statically linked from something not in `psyq/lib`. Either way it
is 117 KB of unmapped, game-reachable code in the file `make check` verifies,
and the function count in this document was badly wrong. The machinery is all here and working --
`azure lib --list`, `azure libmatch`, `azure libscan`, and `tools/lib2ar.sh`
for `nm`/`objdump` introspection; note `libmatch` and `libscan` read `.lib`
natively and do **not** need pcsx-redux's `psyq-obj-parser`, which only
`lib2ar.sh` uses.

What the resident engine being nearly finished *does* mean: overlay code calls
into it, and the 23 called-but-undefined functions above 0x8008875F are the
engine calling out, so the engine genuinely had to come first. But the honest
framing is "the engine is nearly done, the game content is unmapped", not "the
game is 85% decompiled". See `docs/overlays.md`.

**And "191 functions" is NOT this executable's function count.** That claim was
in this document until 2026-10-23 and it is wrong by about a factor of three --
see the section below. 186 of 191 counts the functions splat has *typed as C*,
which is 8.4% of the file's bytes.

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

## The 5 remaining stubs

| group | count | what it means |
|-------|-------|---------------|
| near-misses, source written and checked | 7 | each with a numeric bound or a named pass check |

**Nothing is structurally blocked any more, and every remaining stub is a real
function.** Each is either ordinary decompilation or a documented near-miss.

### Near-misses, with their files in `docs/near-misses/`

Sorted by how open they look, not by DIFF: a bounded gap at 8 words is further
away than an open one at 47. **BOUNDED** means a numeric bound or a named pass
check has been measured *under this project's current compilation assumptions*
-- not that no C source produces these bytes, which cannot be true. See below.

| function | unit | words | DIFF | state |
|----------|------|-------|------|-------|
| func_8003F368 | unk_12A70 | 113 of 113 | 4 | **BOUNDED**, both gaps named; each costs a word to produce from C |
| func_80041E70 | unk_154BC | 270 of 270 | 1 | **BOUNDED**, one word, owned by cse and argued completely |
| func_8004437C | gprel_17B7C | 100 of 100 | 8 | **BOUNDED**, diagnosed at pseudo level |
| func_800407C0 | unk_12DEC | 178 of 178 | 62 | **BOUNDED**, a real copy preference plus a 5x priority gap |
| func_80047054 | unk_19434 | 58 of 58 | 30 | **BOUNDED**, one common cause, not splittable by any shape yet measured |

### What 191 of 191 would actually take

Worth stating plainly, because the number is easy to assume is close. All five
are near-misses with the source written and checked, and between them they are
1, 4, 8, 30 and 62 differing words. Four of the five have every opcode,
immediate, displacement, branch offset and delay slot matching already and
differ only in which register holds what.

**Do not read BOUNDED as "unreachable".** These bytes were produced by cc1psx
from a C file, so a C source that reproduces them exists -- that is a fact about
how the ROM was built, not a hope. What the headers below have sometimes called
unreachable is always narrower: unreachable *given everything else this project
assumes*. The unit boundary, each symbol's declared type and size, the `-G`
value, which other functions share the translation unit and in what order, the
struct layout, the flags -- every one of those is this project's inference, not
evidence about the original source. "No spelling of `end` moves that
preference" can be a sound measurement and still rest on a fixed compilation
context that was never verified.

That distinction is where the next result comes from. Four conclusions in these
files fell in one day once the right mechanism arrived, each after two sessions
of sweeping had been written up as proof of unreachability -- see
`docs/near-misses/README.md`. In all four the *measurements* held and the
*context around them* was what moved. So expect the last five to come from a
mechanism or a changed assumption, not from more source-shape sweeping.

### Audit the context before sweeping the source

The questions that have actually paid, in rough order of how often this project
has had them wrong:

1. **Does this function share a translation unit with what I think it does?**
   Unit boundaries are inferred from `$gp` usage and jump-table runs, and both
   inferences have been wrong (see the jump-table section of
   `docs/open-matching-problems.md` for one that was not merely wrong but
   impossible). The boundary decides which constants cse has already seen, what
   loop.c has hoisted, and which allocnos live across a call.
2. **Is every symbol it touches declared at its real type and size?** The
   declaration, not the use, picks symbolic against split addressing, and a
   wrong size silently changes the mode at every site. See
   `docs/addressing-and-translation-units.md`.
3. **Is the `-G` value right for this unit?** `-G8` against `-G0` moves which
   objects are `$gp`-relative, which moves prologue register pressure.
4. **Is the struct layout right?** A field at the wrong offset yields the right
   instruction with the wrong displacement, and the diff then reads as a
   register problem.
5. **Is the function order within the unit right?** It is inferred from address
   order, which is usually but not always the source order.

Only once those are pinned is "the source shape is wrong" the live hypothesis.

**Read the near-miss file before touching one**, and read it the way its own
README says: the measurements are evidence, the verdict is a hypothesis. Each
lists the shapes already measured with their word costs, and most record a
`search.py` sweep that found nothing -- so if the context checks out, the
remaining gap is allocation or ordering rather than source shape.
`python3 tools/decomp/nmcheck.py` compiles all five and checks each against the
count in its own header.

**The mechanism that closed the last four** is still the first thing to try on
the rest: `regs_someone_prefers` makes `find_reg` avoid a register that a
*lower*-priority conflicting allocno prefers, so an allocno allocated first can
be pushed off the register it should have had. The cheapest way to move a
priority is a reference costing no instruction -- splitting one assignment into
two statements that compile to the same single insn. On a block-local quantity
the currency is live length rather than reference count, so hoisting an
arithmetic subterm works where an extra reference does not. The conflict side is
a second lever: giving an existing allocno a second role in another block
imports that block's hard-reg conflicts, which is what bought three of
func_80041E70's four words for free. `docs/matching-levers.md` section 3 has the
arithmetic.

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

1. The five near-misses, ordered above. **func_80041E70 at one word is the
   closest**, though its bound is the best-argued of the five; func_8003F368 at
   4 and func_8004437C at 8 are the next. Run the context audit above before
   sweeping any of them. The two functions that fell on 2026-10-23 both went to
   a mechanism read out of gcc 2.7.2's source rather than to a source sweep,
   which is the pattern to repeat.
2. A decision on func_8003E758's decomp device. It ships
   `__asm__ volatile ("" :: "r" (q) : "memory")`, worth 13 words, which shapes
   the allocator directly rather than reproducing a source shape. The honest
   fallback is 698 words / 11 DIFF with the memory clobber alone.
3. func_80042BDC's load-bearing source shapes were never written up -- the
   agent that closed it was cut off immediately after reporting. The code
   comment says so; the 14-DIFF near-miss is in git history if they need
   recovering.
4. Deferred type merges, all recorded where they matter: four scratchpad
   typedefs (Unk3DBD0SP/Unk463ECSP/Unk44D24SP/Unk3CCB0SP), three POLY_FT4s
   (Unk463ECP/Unk3D0F0P/Unk44D24P), Unk800E3E48 into Unk42710, and the
   RotAverage4 prototype disagreement (`s32 *` against `u32 *`). Each is held
   back for a stated reason -- usually that retyping a field risks an existing
   match for a cosmetic gain -- so read the reason before doing one.
5. The overlays, which are where the actual game is. `docs/overlays.md` has the
   analysis; nothing there is started.
6. `tools/lib2ar.sh` references a path under `$HOME/scratch` and does
   `basename $1 .LIB` against a lowercase `.lib`. Known, tangential.

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
