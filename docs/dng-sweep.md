# Decompiling DUNGEON.BIN's 116 carved objects

This block is the 2,566 stubs that `docs/dungeon-map.md` counted and
`splat/dungeon.bin.yaml` carved on 2026-11-22. It is NOT the dungeon overlay
at 0x800894A0 -- that one is 100% landed and `docs/dungeon-sweep.md` is its
record. These are the objects DUNGEON.BIN streams into two RAM slots: 47
banked room scripts at 0x80170800, 66 at 0x80024000, and three low-memory
units.

## THE ONE FACT THAT SHAPES THE WHOLE SWEEP

**1199 of the 2575 stubs are exact copies of a sibling -- 93,302 of 327,714
words. 1376 distinct functions cover the block.**

The objects statically link a shared library, so one function appears,
relinked, in up to 216 of them. The copies differ only in the symbols the
relink changed.

    python3 tools/decomp/dngfam.py              # families by word yield
    python3 tools/decomp/dngfam.py FUNC         # FUNC's family, listed
    python3 tools/decomp/dngfam.py --diff A B   # why two bodies differ

So **the unit of work is a FAMILY, not a function.** Hand an agent the
representative, and replicate the landed body mechanically:

    python3 tools/decomp/dngland.py FUNC --from <agent copy>.c

which splices the agent's body into the real `csrc/<unit>.c`, verifies it
THERE, and then runs `dngclone.py` over every other member -- renaming each
copy's symbols and verifying each one against its own listing before writing
it. Nothing is written that does not come back `0 differing`.

Rank work by `yield`, not by word count. A 25-word function with 94 copies is
worth more than a 300-word function with one.

## The dispatch loop

Five agents at a time, one function each, from `tools/decomp/dng-brief.txt`
with `@FUNC@`/`@UNIT@`/`@WORKDIR@`/`@WORDS@`/`@COPIES@`/`@YIELD@` filled in.
Working directories go under `/home/case/scratch/azure-dng/agent-<id>/`,
**not** `/tmp` -- a routine cleanup once took four finished functions and five
unit copies with it (`docs/dungeon-parks/README.md`).

Two rules the brief repeats because they are the ones that cost other people
work:

* **No agent runs `make` in any form.** It re-runs splat, which deletes
  `asm/nonmatchings/` and rewrites it -- the ground truth every other agent is
  measuring against. The build gate is the dispatcher's, run once per batch.
* **No agent runs the permuter.** Its jobs are a machine-wide budget of 20 and
  only one search may run at a time; with five agents live that budget belongs
  to whoever is dispatching.

The agent's deliverable is the body, its comment, its declarations, and either
`0 differing` or a park in `docs/dng-parks/` with a measured residue.

## `ovlverify.py` resolves these by NAME, not by address

47 of the objects load at 0x80170800 and 66 at 0x80024000, so an address names
a RAM SLOT here and not an object: no range table can ever say which unit a
function belongs to. splat gives each segment `symbol_name_format: <tag>_$VRAM`
instead, so a function is `func_<tag>_<vram>` -- `func_b01_801708E8` -- and the
name itself carries the answer. `overlay_for()` consults that first.

The consequence for anything else that reads a function name: the old
`func_[0-9A-Fa-f]{8}` pattern does not match these, and `int(name.split("_")[1],
16)` parses `b01` as 0xB01 rather than failing. Tools that key on either need
checking before they are trusted on this block.

## The build gate is still the build gate

`ovlverify` compares ONE function's words. It says nothing about a unit's
.rodata, and a local array initialiser emitted into .rodata that an
`INCLUDE_RODATA` line also pulls in has silently moved every later function
in a unit before. A clean sweep here is a reason to build, not a substitute:

    make check > /tmp/check.log 2>&1; echo rc=$?
    make containers

and **count the `OK  byte-identical` lines: there must be four**, two of them
from `make containers` rebuilding town and main. Read the hashes, never pipe
make through `head`/`tail`, and do not `rm -rf build` -- splat generates the
linker scripts into it.

## THE COMMITTED TREE DID NOT RELINK, AND NOTHING CAUGHT IT FOR A DAY

Fixed 2026-11-23 in `splat/dungeon.bin_extra_syms.txt`. Two addresses --
`0x80174674` in b31f and `0x8001E814` in lo20 -- are rodata islands that the
carve named as functions. spimdisasm emits `dlabel func_<tag>_<vram>` there
while the code reaching the address gets splat's data spelling
`D_<tag>_<vram>`; the two names never meet and the dungeon link dies on
`undefined reference to D_b31f_80174674'.

It is LATENT, which is the part worth remembering: the carve's own build was
byte-identical because the listings on disk had been written by an earlier
splat run, and the failure appears only when something re-runs splat. A
landing batch does, so the first batch after the carve inherited a broken
build and it looked like the batch had broken it. **If a dungeon link fails
right after a sweep, check whether it fails at HEAD too before reading a line
of the diff** -- `git stash push csrc/`, `rm -f build/dungeon.bin`, build.

A grep for `^dlabel func_` over the carved listing directories finds exactly
these two.

## Batch 1, 2026-11-23: five agents, 551 functions, 15,994 words

Five representatives, one agent each, all five landed -- four on the first
compile. Then `dngland.py` cloned each family:

| representative | words | copies | cloned | yield |
|---|---|---|---|---|
| `func_b01_801735D4` | 14 | 216 | 215/215 | 3,024 |
| `func_b01_80173568` | 8 | 119 | 118/118 | 952 |
| `func_b01_80172800` | 25 | 94 | 93/93 | 2,350 |
| `func_b01_80172628` | 81 | 46 | 45/45 | 3,726 |
| `func_b02_80174574` | 50 | 42 | 41/41 | 2,100 |
| `func_b03_80171DA0` | 113 | 34 | 33/33 | 3,842 |

The block went 4259/6825 to 4810/6825, 39.7% to 42.7%.

**A FAMILY CAN SPLIT ON ONE IMMEDIATE.** `func_b03_80171DA0`'s last five
members compare `0x3F`/`0x7E` where the other 29 compare `0x40`/`0x80`.
`dngfam.py` normalises hex literals, so they are one family there; `dngclone`
refused them at 2 differing and wrote nothing, and they landed from the same
body with the other constant. That is the normalisation working as designed:
the cost of a too-loose family is a failed verify, never a wrong landing.

Three defects the dry runs caught before anything reached `csrc/`, each now
written up at its site in `tools/decomp/dngclone.py`:

* the symbol pattern required the `<tag>`, and a per-copy data label is
  tagged inside the object's carved data subsegment but UNTAGGED in the `bin`
  tail -- one family had both spellings in the same instruction pair;
* the thread pool ran one thread per MEMBER, and a family with five members
  in one unit had them overwrite each other, every one reporting
  `0 differing` against a file that was dead by the time it was on disk;
* `\s*$` in the splice pattern ate the blank line after a stub, so two landed
  functions ended up adjacent, and the comment walk-back -- which looked back
  to the last blank line -- then swallowed the previous function whole and
  cloned it as a second definition.

And one that reached `csrc/` and had to be re-run: a clone carried its whole
unit's declaration block rather than the declarations its body uses, so each
family after the first copied the earlier families' declarations into every
member and renamed only the ones its own symbol map knew. 171 stray
declarations across 46 units, harmless to the link and wrong in every other
way. `needed_decls()` is the fix.

## Batch 2, 2026-11-23: five agents, 96 functions, 28,985 words

All five landed again, and every clone verified:

| representative | words | copies | cloned | yield |
|---|---|---|---|---|
| `func_b01_80171C34` | 466 | 35 | 34/34 | 16,310 |
| `func_b03_80170A28` | 267 | 15 | 14/14 | 4,005 |
| `func_b03_801736F4` | 207 | 17 | 16/16 | 3,519 |
| `func_b07_80171570` | 145 | 18 | 17/17 | 2,610 |
| `func_b03_80173358` | 231 | 11 | 10/10 | 2,541 |

42.7% to 48.0%.

## THE RESIDENT OVERLAY IS A BODY-LEVEL TEMPLATE, NOT JUST A SIGNATURE SOURCE

This is the single most useful thing the first two batches established, and it
is why nine of ten functions landed on or near the first compile.

The banked room scripts are relinked copies of code that also exists in the
resident dungeon overlay at 0x800894A0 -- **which is 100% landed**. So for
most of this block the answer is already written, with a comment explaining
what made it land:

| carved function | resident twin |
|---|---|
| `func_b01_80171C34` | `func_800D92C0` |
| `func_b03_80170A28` | `func_800D8728` |
| `func_b03_801736F4` | `func_800DA398` |
| `func_b03_80173358` | `func_800DA014` |
| `func_b07_80171570` | `func_800D904C` |
| `func_b03_80171DA0` | `func_800CA788` |
| `func_b01_80172800` | `func_800D9C18` |
| `func_b02_80174574` | `func_800DA660` |

Find the twin FIRST. The differences are small and local -- an extra arm, a
bank-local table in place of a resident one, an inverted gate -- and the
twin's own comment usually names every load-bearing spelling. **Re-measure
those spellings rather than assuming them**: on `func_b03_80170A28` all three
of the twin's named facts held (3, 15 and 132 aligned differences when spelled
otherwise), and on `func_b07_80171570` the gate was the twin's *inverted*,
worth 2 words and 46 differences to get wrong.

## Two more dngclone defects, both from batch 2

* **`definition()` matched the FORWARD DECLARATION, not the definition.** A
  landed body elsewhere in the unit may declare the function being cloned --
  `func_b01_80172628` declares `func_b01_80171C34` -- and the prototype comes
  first in the file. The old code took the first regex match, scanned on for
  the next `{`, and swallowed every INCLUDE_ASM and definition until the
  braces balanced; the compile then said `redefinition of
  __maspsx_include_asm_hack_...' about lines nobody had written. It now tries
  every candidate and accepts only one with `{` after its parameter list.
* **TWO AGENTS CHOOSE DIFFERENT PARAMETER TYPES FOR THE SAME CALLEE, AND BOTH
  ARE RIGHT.** `func_b01_80171C34`'s author declared its sibling
  `s32 f(u8 *, void *, void *, u8 *)`; that sibling's own landed body, from
  another family, is `s32 f(u8 *, UnkVec *, u8 *, u8 *)`. In the 26 units
  holding both, the renamed declaration was a `conflicting types' error --
  26 of 34 members refused. `reconcile_decls()` takes the prototype from the
  DEFINITION wherever the target unit has one, which is sound because a
  pointer argument converts without an instruction, and every member is
  verified anyway.

And `decl_block()` now splits on STATEMENTS rather than lines, because a
declaration block can hold a multi-line struct typedef and splitting on
newlines shredded it into a kept `typedef struct X` line and dropped fields.

## Batch 3, 2026-11-23: five agents, 60 functions, 9,812 words

All five landed; three on the first compile. 48.0% to 49.8%.

| representative | words | copies | cloned | yield |
|---|---|---|---|---|
| `func_b14_80171410` | 466 | 5 | 4/4 | 2,330 |
| `func_b09_80173D4C` | 228 | 9 | 8/8 | 2,052 |
| `func_b09_801740DC` | 195 | 10 | 9/9 | 1,950 |
| `func_b03_8017139C` | 150 | 12 | 11/11 | 1,800 |
| `func_b03_80171F64` | 70 | 24 | 23/23 | 1,680 |

**Pick on YIELD PER WORD from here on.** The 200-copy families are gone, so
the ranking by raw yield now puts a 466-word function with 5 copies above a
70-word function with 24. The second is better work: `func_b03_80171F64`
landed on the first compile with NO new declarations at all, because its
113-word sibling from batch 1 had already brought every symbol into the unit.

**A LANDED CARVED SIBLING IS OFTEN A BETTER TWIN THAN THE RESIDENT ORIGINAL**,
and increasingly so as the sweep goes on: it has already absorbed the relink.
`func_b03_8017139C` landed as `func_b07_80171570` plus five words;
`func_b09_801740DC` as the landed `func_b05_80173BEC` minus three flag
updates. The brief now says so.

**And re-measuring the twin's facts pays in BOTH directions.** On
`func_b03_80171F64` two of its sibling's three named spellings did not hold
(28 and 12 differing if assumed); on `func_b09_80173D4C` the twin's register
map came from a duplicated bump that here was not enough on its own.

## `dngfam --diff` NORMALISES LABEL NAMES, SO IT CAN HIDE A STRUCTURAL CHANGE

Fixed 2026-11-23. `sig_insn` replaces a label with `S`, so two bodies whose
branches go to DIFFERENT PLACES compare equal instruction for instruction. It
reported `func_b14_80171410` as exactly one `nop` away from the landed
`func_b01_80171C34`, that went into the agent's brief as "one word apart", and
the real difference was a brace -- b01's `0x8000` gate wraps two arms where
b14's wraps one. The agent found it from the branch offsets anyway, after
spending its first compiles looking for a spelling.

`--diff` now resolves every branch label to an instruction index and reports a
target mismatch separately, and when the streams match but the flow does not
it says in as many words to look for a structural difference rather than a
spelling.

## One more declaration-placement defect

New declarations were inserted after `#include "common.h"`, which put a new
`extern UnkDngRoom2970 D_800E2970_s[]` AHEAD of the `typedef struct
UnkDngRoom2970` the unit already carried further down: `syntax error before
D_800E2970_s' in three of one family's four members, while the fourth happened
to lack the typedef and so received both in dependency order. They append to
the END of the declaration block now, which leaves every existing declaration
in scope and still orders a new typedef before a new user of it.

## Batch 4, 2026-11-23: five agents, 123 functions, 6,134 words

**All five landed on the first compile**, and 20 of 20 agents have now landed
their function. 49.8% to 50.9%.

| representative | words | copies | cloned | yield |
|---|---|---|---|---|
| `func_b01_80174070` | 14 | 71 | 70/70 | 994 |
| `func_b09_80171320` | 258 | 7 | 6/6 | 1,806 |
| `func_b06_80173E94` | 148 | 10 | 9/9 | 1,480 |
| `func_b09_80171158` | 114 | 11 | 10/10 | 1,254 |
| `func_b03_80171D3C` | 25 | 24 | 23/23 | 600 |

`func_b01_80174070` is the sweep's best return: three lines of C in 74
seconds for 994 words. Its twin was not in the resident overlay but two
listings away in its own unit -- it sits between two already-landed 14-word
installers and differs from them in ONE branch opcode, `blez` where they have
`beqz`. Read your neighbours before you read anything else.

**THE ALLOCATION ARITHMETIC CUTS BOTH WAYS.** Three functions so far needed
duplication ADDED to buy references; `func_b09_80171320` needed it REMOVED.
It is the same function as the landed `func_b03_80170A28` and its thirteen
siblings, except that the ground-height arm's common tail is written once
here where theirs is duplicated -- and the duplicate's extra `q + 0x1C`
reference is exactly enough to move `q` above `p` in the allocno rank. Written
the siblings' way it is 267 words and 74 aligned differences.

## A FAMILY SPLITTING ON ONE IMMEDIATE IS COMMON -- `dngclone` DOES IT NOW

Three of the first twenty families. `dngfam` normalises hex literals, so they
sit in one family and `--diff` calls them identical, which is right: they ARE
the same function with a per-bank constant (an actor state, an angle window).
Until batch 4 each one cost a failed verify and a hand re-clone from a `sed`.

`const_pairs()` now reads the pair off the two listings and `vary_consts()`
substitutes it BY VALUE -- the listing says `0x9` where the C says `9` --
having masked comments first, because a landed body's header explains its
constants in prose and the value therefore occurs twice. A value that is not
in the code exactly once is refused by name rather than guessed at, and every
substitution is recorded in the clone's comment so an inherited write-up is
never quietly wrong about its own numbers.

## Three more dngclone defects from batch 4

* **`DECLARED` named every function-pointer array `void`.** In
  `extern void (*D_80174E5C[])(...)` the `void` is itself followed by `(`, so
  the pattern matched it, and `reconcile_decls` -- which keys on the name --
  substituted a unit's existing `D_80174E4C` table declaration for the one
  being cloned. The symptom was `D_80174E5C' undeclared, from a declaration
  that had been silently replaced by its neighbour. It skips type keywords now.
* **TWO AGENTS CAN DISAGREE ABOUT THE ARITY OF ONE FUNCTION AND BOTH LAND.**
  `func_b09_80171158` takes `func_b09_80171320`'s ADDRESS and declared it with
  four parameters; that function's own body is three. Taking an address does
  not depend on the signature, so both verified at 0 differing in their own
  copies and the conflict appeared only when they met in one unit.
  `fix_forward_decl()` rewrites the stale declaration from the definition,
  which is the authority.
* **A UNIT'S EXISTING DECLARATION NOW WINS over a cloned one** even when the
  unit does not define the symbol, for the same reason the definition does:
  it is what the unit's other landed bodies already compile against.

And the safety net that should have been there from the start: **`dngland` and
`dngclone` now re-verify EVERY function the unit defines**, not just the one
being added. `ovlverify` compiles once for any number of names, so naming the
others costs a comparison each -- and adding a definition can change what an
earlier landed body compiles against, which is exactly what a reconciled
prototype does.
