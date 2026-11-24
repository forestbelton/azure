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

# Batch 5 onward: ONE AGENT, ONE UNIT, SEVERAL FAMILIES

Batches 1-4 gave each agent one function. That shape was right while the
families were huge -- batch 1's five representatives were worth 15,994 words
-- but yield per batch fell to 6,134 by batch 4 as the 200-copy families ran
out, and the bodies themselves got small: several of batch 4's landings took
under two minutes. The limiting factor stopped being the work and became the
handover.

So from batch 5 the unit of dispatch is **one agent, one unit, every
worthwhile family in it**. `tools/decomp/dng-brief.txt` is now written for
that, and the grouping is not a convenience:

* **One copy, one `AZURE_SCRATCH`, one declaration block.** The agent's second
  function usually needs no new declarations because the first already brought
  its symbols in. That was already observable across batches
  (`func_b03_80171F64` landed with none at all); within one agent's run it is
  the normal case.
* **The best twin is often in the same unit.** `func_b01_80174070` -- 994
  words of yield -- landed in 74 seconds as an already-landed neighbour two
  listings away differing in one branch opcode. An agent holding four families
  from one bank has those neighbours in front of it, and the first function it
  lands becomes the twin for the next.
* **Ordering is part of the brief.** Highest yield-per-word first, stated as
  deliberate, so a run cut short has banked the valuable end. "Landing one is
  a success" is stated too: a bounded fight then a park and move on, rather
  than a whole run spent on the hardest of five.

## Choosing the groups

Rank families by copy count (which IS yield per word of body), group by unit,
and pack each unit to a budget of ~480 words of representative body. Batch 5:

    agent  unit          families  rep words   yield
    h1     ovl_dng_b01      4         442      3,312
    h2     ovl_dng_b03      2         455      3,198
    h3     ovl_dng_b05      3         355      2,193
    h4     ovl_dng_b09      5         460      2,181
    h5     ovl_dng_b04      5         433      2,130
                                               ------
                                               13,014

Double batch 4's yield for the same five agents. The renderer is
`/home/case/scratch/azure-dng/render-brief.py`; the table above is its input.

## `dngland` takes several functions now

    python3 tools/decomp/dngland.py FUNC... --from AGENT_COPY.c

All the names must be in one unit. The bodies are spliced together, the
declarations unioned (`needed_decls` over the CONCATENATED bodies), and
everything verified in ONE compile -- the requested functions plus every other
function the unit defines.

Two behaviours are deliberate and were tested before the batch landed, by
rolling `csrc/ovl_dng_b42.c` back to 795c383 and re-landing batch 4's two
functions into it from the committed file:

* **A failing requested function is DROPPED and the rest retried.** Four good
  bodies are worth banking and the fifth's residue is a separate problem. The
  dropped names are printed as `NOT LANDED:`. Verified by corrupting one body's
  offset: `0x8C` -> `0x90` gave `1 differing`, that function was dropped, and
  the other still landed with 6 declarations instead of 7.
* **A REGRESSION IN AN ALREADY-LANDED FUNCTION STOPS EVERYTHING.** It is not
  attributable to any one requested function, so dropping them one at a time
  cannot fix it; nothing is written and a human reads it. This is the one
  failure mode the wider shape adds, and guessing at it is how a landed body
  gets lost.

## `vary_consts` refused three good members: the two rules it was missing

Batch 5's first integration cloned 29 of `agent-h1`'s 32 members and refused
three of `func_b01_80173AAC`'s family -- 639 words -- with `per-copy immediate
not placeable`. `dngfam --diff` called all three identical after
normalisation, so the refusal, not the family, was wrong. Both causes are
general and both are fixed:

* **A VALUE USED TWICE IN THE BODY IS NOT AMBIGUOUS WHEN BOTH USES MOVE.**
  `const_pairs` reports one entry per differing SITE, so the offset `0xa2` --
  read as a halfword and then cleared -- arrived as the same pair twice. The
  old rule demanded exactly one occurrence in the code and refused. The rule
  now is a COUNT MATCH: k identical pairs against exactly k occurrences means
  substitute all k. One `old` mapping to two different `new` values is still
  refused by name, and so is k pairs against j != k occurrences, because the
  value then also appears at a site the listings agree on and no amount of
  looking at a value says which is which.

* **A CLEAR-THESE-BITS MASK IS A NEGATIVE IMMEDIATE IN ASM AND A `~` IN C.**
  These three members want `&= ~0x208` where the representative has `&= ~8`.
  The listings differ at `-0x9` vs `-0x209` and `-0x9` appears NOWHERE in the
  source text, so the search found zero occurrences and refused. `_sites()`
  now also matches the one's-complement spelling, requiring the `~` to be the
  token's immediate predecessor so a bare `8` elsewhere cannot be taken for
  one, and writes the replacement back in the same form. The note records it
  as `-0x9 -> -0x209 (as ~0x8 -> ~0x208)`, which is what a reader of the
  clone needs.

All three then verified and were written. The note is also placed on its own
line now; it had been landing on the end of the representative's last
sentence, and these comments are the most valuable thing the sweep produces
after the code.

## THE FIRST AGENT TO INVENT A TYPEDEF BROKE THE INTEGRATION, THREE WAYS

`agent-h4` is the first of batch 5 to need new types -- a scratchpad layout
and a two-field table pair. All five of its functions verified in its own
copy; spliced into `csrc/`, the unit did not compile at all. Three separate
defects, and the worst of them had been reported as the opposite of what it
was:

* **`reconcile_decls` SUBSTITUTED ONE TYPEDEF FOR AN UNRELATED ONE.**
  `DECLARED` extracts the declared name as the first identifier that is not a
  type keyword; in `typedef struct X { u8 pad_00[0xC]; ... } X;` that is THE
  FIRST FIELD. `UnkDng708A4SP` and the unit's existing `UnkDngRoom2970` both
  begin `u8 pad_00[...]`, so they looked like two declarations of a symbol
  called `pad_00` and the unit's won. **This is the third defect of exactly
  this shape** -- after the function-pointer array named `void` and the stale
  forward declaration -- so the rule is now a named predicate,
  `is_type_decl()`, used at every call site: a type statement declares no
  symbol the unit could define or redeclare and is passed through untouched.

* **`needed_decls` missed a typedef that had a comment above it.** It kept
  types unconditionally by testing `d.startswith("typedef")`, but a
  hand-written declaration comes with a comment, and an invented typedef
  ALWAYS does -- the comment is the witness for the struct. So the statement's
  text starts `/*`. `decl_code()` strips leading comments and both tests run
  on that.

* **`dngland` reported a failed compile as every function failing**, then
  crashed printing it (`KeyError` on a function with no verdict). Not one
  verdict line means the unit did not compile, which is a different thing and
  cannot be fixed by dropping names: it now prints the compiler's own output
  whole and writes nothing. The useful diagnosis -- `UnkDng708A4SP'
  undeclared` -- was in that stderr the whole time and the first run threw it
  away.

All five then landed with 29 clones, 2,181 words, no failures. Worth noting
what did NOT go wrong: the agent checked its three new type names against
every `csrc/ovl_dng_*.c` and `docs/dng-parks/` before choosing them, which is
what the brief asks for, and there was no collision with the other four
agents. The hazard was never the names.

## A family member that is not a relink: say so, do not refuse

`func_b04_80174948`'s 9-copy family has one member, `func_b10_801751C0`, that
`const_pairs` reads as differing in a single immediate -- `0x17` -> `0x18` at
ONE site, where the body writes `p[0x9A] = 0x17` TWICE. Which of the two is
not knowable from a value.

`vary_consts` now RETURNS BOTH PLACEMENTS instead of refusing, and
`clone_unit` verifies them in turn. That is the sweep's rule everywhere else
and it holds here: a wrong guess costs a failed verify, never a wrong landing.
Capped at twelve candidates so a pathological family cannot become a build
farm, and the winning spelling is named in the result line.

Both placements came back at 22 differing, so that member is genuinely
different code and stays a stub -- 43 words of real work for a later batch.
The point is that the tool now distinguishes "I cannot place this constant"
from "this is not the same function", and only the first is a tool limit.

## THE BUILD GATE CAUGHT WHAT THE VERIFIER CANNOT: A JUMP TABLE

Every one of batch 5's 17 representatives verified at `0 differing`, all 124
clones verified, and `make containers` then said:

    size differs by +0x120
    FAIL  0xded004 of 0x1a86800 bytes differ (52.50%); first at 0x9e9974
        built  addiu $v0, $v0, 0x3c80
        ref    addiu $v0, $v0, 0x0808

`func_b03_80170E54` switches on 12 cases and the ROM indexes
`jtbl_b03_80170808`, which lives in the b03 segment's LEADING `data`
subsegment. Our compile emits its OWN 12-entry table into `.rodata` and the
linker appends it past the code: 0x30 bytes times the SIX members of the
family is the +0x120, and everything after the first table shifted, which is
the 52.5%.

**`ovlverify` cannot see this.** It measures a function's own words against
its listing and nothing else, so a body whose `.rodata` is wrong reports
`0 differing` and nothing but the container build catches it. This is the
clearest demonstration yet of why the build gate is the gate and not a
formality, and it is the first defect in five batches that `ovlverify` was
structurally unable to find.

The body is RIGHT -- 338 words, 0 differing, `aligndiff` 0, every `--raw`
difference a relocation -- so it is parked rather than abandoned, with its
three load-bearing spellings and the twin's relink deltas, at
`docs/dng-parks/func_b03_80170E54.c`. `tools/decomp/dngunsplice.py` is new and
returns a landed body to its `INCLUDE_ASM` stub, keeping a copy; the five
clones need no re-deriving, since landing the representative regenerates them.

### This is worth more than a batch of landings

**229 of the carved block's functions reference a `jtbl_` symbol, and 223 of
them are still stubs.** Every segment with a leading `data` subsegment will
hit this the moment a switch lands in it. The fix is structural: that
subsegment has to become the C file's own `.rodata` so the emitted table lands
at its ROM address instead of after the code, and the ROM's bytes must stop
being included separately -- for b03 that means 0x54 bytes of which the table
is 0x30 at +8, so whatever occupies 0x80170800..0x80170808 and
0x80170838..0x80170854 has to come from the C file too, in that order.

Until then a `switch` over five or more dense cases is the one shape an agent
can land and still not be able to ship, and the brief should say so.

### `parkcheck` was not scanning this directory

`docs/dng-parks/` was created with the carve and `parkcheck.py` only globbed
`docs/dungeon-parks/`, so every park written here for five batches would have
gone unchecked. No park had been written yet, which is the only reason it cost
nothing -- a checker that silently stops covering a directory is exactly the
kind of thing that is discovered too late. It scans both now.

# THE JUMP TABLE SPLIT, 2026-11-23

The batch-5 failure above is fixed for 52 of the 116 objects, and
`func_b03_80170E54` is landed in three units with all four containers
byte-identical. What follows is what the fix is, how far it reaches, and
exactly what blocks the rest, because the remainder is blocked by splat and
not by anything in this repo.

## What the fix is

A carved segment's leading subsegment was declared `data`. For 106 of the 116
it is not a data blob: after a short prefix of real data (usually two words of
function pointers) it is the unit's COMPILER-GENERATED JUMP TABLES, running
contiguously to the code start. So it belongs to the C unit:

    - [0x9e9000, data]                      # the two function pointers
    - [0x9e9008, .rodata, ovl_dng_b03]      # the jump tables
    - [0x9e9054, c, ovl_dng_b03]

`.rodata` WITH the dot, because the sibling C file exists.

With that, `spimdisasm` writes each table into its own function's listing
while the function is `INCLUDE_ASM` -- verifiably, `func_b03_80172A24.s` now
opens with a `.section .rodata` block defining `jtbl_b03_80170838` -- and
cc1psx emits the table once the function lands. Either way it lands at its ROM
address instead of being appended past the code.

`tools/decomp/dngrodata.py` does the analysis and the edit. It validates the
region AGAINST THE ROM, not against splat's output: every word in the proposed
`.rodata` must be an address inside that unit's own code, which is what a run
of jump table entries is and what a string or a float pool is not. A first cut
read `dlabel`s out of the listings instead and reported that every segment
owned no tables -- the dlabels only exist AFTER the split, so its contiguity
check passed vacuously on all 116.

## Alignment, which is what makes this cheap here

cc1psx writes `.align 3` before every table it compiles, so a table at a
4-mod-8 offset RELATIVE TO ITS UNIT'S RODATA START gains four bytes of pad the
reference does not have. That is what forced six separate carves in the
resident overlay (see the comments in `splat/dungeon.bin.yaml`).

Here every one of the 106 segments has its first table at 0 mod 8 absolutely,
so no `subalign` is needed, and in 104 of them every other table is 0 mod 8
relative to the first. The two exceptions are `ovl_dng_lo16`
(jtbl at 0x80016034) and `ovl_dng_b31f` (0x8016a8b4). Splitting those needs a
matching `c` split at the owning function, because cc1psx emits a unit's
rodata in FUNCTION ORDER and the two boundaries have to name the same
function. Until that function lands its table comes from asm with no `.align`
and costs nothing, so the tool holds them back and names them.

## How far it reaches: 52 of 116

    229 functions own a jump table
     63 are stubs in a split unit    -> landable now
    163 are stubs in an unsplit unit -> still blocked
      3 landed (func_b03_80170E54 and two of its family)

The 51 the tool split are the segments whose rodata is NOTHING BUT tables.
The other 53 have non-table rodata between the tables -- b05 is the pattern:
two words of data, `jtbl_b05_80170808` (0x30), then 0x20 bytes of packed
halfwords at `D_b05_80170838`, then `jtbl_b05_80170858`, then two more 8-byte
blobs before the code.

## WHY THE OTHER 53 ARE BLOCKED, AND IT IS SPLAT

Declaring b05's subsegment `.rodata` makes the link fail:

    undefined reference to `D_b05_80170838'
    undefined reference to `D_b05_80170874'
    undefined reference to `D_b05_8017087C'

From `splat/segtypes/common/rodata.py`:

    if self.sibling is not None or not options.opts.pair_rodata_to_text:
        return None

Migration to a text subsegment is OFF for a rodata subsegment that HAS a
sibling C file, which is exactly our configuration. A jump table survives
that, because spimdisasm writes a function's OWN rodata into its listing. A
symbol that is merely REFERENCED by a function and not owned by it -- which is
what these blobs are -- is written NOWHERE, and the link fails.

The documented lever for this is `force_not_migration:True` in
`symbol_addrs`, and **it cannot be used here.** All 116 objects share VRAM
0x80170800 under one `exclusive_ram_id: dng_slot`; `symbol_addrs` is keyed by
VRAM, so there is no way to say "b05's symbol at 0x80170838" and not the
other 115 segments' symbols at the same address. Tested: the entries were
written, splat parsed them, and not one standalone `.s` was produced in any
unit. That is the whole reason the carve needed `symbol_name_format` per
segment in the first place, and it cuts the other way here.

Two routes remain for those 53, neither attempted:

* **Write the blob as real C data in the unit** -- `const` arrays emitted into
  `.rodata` in function order. No splat change at all, and these blobs are
  small (0x8 to 0x20 bytes). It is ordinary decomp work and the build gate
  checks it. This is the cheaper route and probably the right one.
* **Teach splat per-segment symbol scoping**, or carve each affected segment's
  rodata so no non-table symbol falls inside it. The second is impossible as
  stated: one C object has ONE `.rodata` section and it cannot be placed at
  two addresses.

## Operational notes

* `tools/decomp/dngunsplice.py` returns a landed body to its `INCLUDE_ASM`
  stub, which is what made it safe to take `func_b03_80170E54`'s family out of
  batch 5 and put it back here.
* A `switch` over five or more dense cases is still unlandable in the 64
  unsplit objects. The brief should say so before an agent spends a run on one.
* `make splat` after a yaml change does NOT invalidate the objects: remove
  `build/csrc` before rebuilding, or a stale `.o` links against the old
  listings and the failure looks like a phantom undefined symbol.

## The const-data route finished it: 105 of 116

The 53 segments the first pass could not split are down to one, and 224 of the
229 jump-table functions are landable where 63 were. Three things were wrong
with the first pass's reasoning, and the third is the one worth remembering.

**1. The byte heuristic over-rejected by 31.** It asked whether every word in
the proposed `.rodata` was an address inside the unit's own code. 31 segments
failed on a word that was `0x00000000` -- a NULL entry for an unreachable
case INSIDE a jump table, which a run of tables is perfectly free to contain.
The test is now empirical and asks the question the LINKER will ask: is there
a data symbol in this region that the code REFERENCES? That is exactly the
set that would come out undefined, and it is 47 symbols across 22 units, not
85 blobs across 53.

**2. The blobs are small and ordinary.** 43 of the 47 are 4 to 32 bytes of
packed halfwords between two tables. They go into the unit as

    const u8 D_b05_80170838[] = { 0x00, 0x10, ... };

`const` is not cosmetic: without it the array goes to `.data` and the linker
puts it outside the segment's declared `.rodata`. They are spelled as BYTES
because a `lwl`/`lwr` pair is all the reference witnesses -- nothing here
claims to know they are halfwords. Two units already had a landed body
declaring the symbol `extern u8`, which conflicts with a `const` definition;
those declarations were corrected and all 51 landed functions in those two
units re-verified at `0 differing`.

**3. ORDER IS THE WHOLE THING, AND GETTING IT WRONG LOOKS LIKE WRONG DATA.**
cc1psx emits a unit's rodata in FUNCTION ORDER, so a `const` array declared
between two functions lands between their jump tables -- which is where these
blobs sit in the ROM. Each blob is therefore anchored to the owner of the
NEXT table after it and inserted before that function.

Inserting them one at a time reverses any group that shares an anchor: each
insert lands immediately above the anchor, so the LAST one written ends up
closest to it. `ovl_dng_b10`'s two blobs went in as 0x874 then 0x854 and the
container came out THE RIGHT SIZE with 0x185 bytes of rodata swapped. A
wrong-size failure points at padding or a missing symbol; a right-size
failure with swapped content is an ordering bug, and it is worth reading it
that way immediately. One insert per anchor, ascending inside it.

### `--prune` is a separate pass because the question cannot be asked earlier

Four of the 47 blobs -- `D_s18_80024074`, `D_s37_80024050`,
`D_s40_80024038`, `D_s42_80024064` -- are ones spimdisasm considers their
function's OWN and writes into its listing. A `const` array for one of those
is a double definition, and the assembler reports it as
`symbol D_s18_80024074 is already defined` pointing at `include/labels.inc`
rather than at anything a person wrote.

Whether a symbol gets migrated depends on the subsegment being `.rodata`, so
while `--const` runs the answer is still no for every symbol. The order is:

    python3 tools/decomp/dngrodata.py --apply    # the segments needing nothing
    splat split splat/dungeon.bin.yaml
    python3 tools/decomp/dngrodata.py --const    # const arrays + their splits
    splat split splat/dungeon.bin.yaml
    python3 tools/decomp/dngrodata.py --prune    # drop what splat emits itself
    rm -rf build/csrc && make check && make containers

### What is left

`ovl_dng_lo16` alone, holding two jump-table stubs. Its `jtbl_80016034` is 4
mod 8 relative to a rodata start of 0x80016000, so cc1psx's `.align 3` would
pad it by four bytes; splitting it needs a matching `c` split at the owning
function, because the rodata and code boundaries have to name the same one.
`ovl_dng_b31f`'s 4-mod-8 table (0x8016a8b4) is in a unit that IS split now --
safe, because a table whose function is still `INCLUDE_ASM` comes from asm
with no `.align` at all. It will need the same carve when that function lands,
and `dngrodata.py` reports both by name on every run.

`func_b03_80170E54`'s whole six-member family is landed.

# Batch 6: 15 of 15, and the jump tables pay off twice

    agent  unit          reps  clones  words
    h6     ovl_dng_b27     2      5    1,965
    h7     ovl_dng_b02     4      9    1,795
    h8     ovl_dng_b14     3      7    1,682
    h9     ovl_dng_b06     5     28    2,102
    h10    ovl_dng_b03     1      4    1,580
                          --     --    -----
                          15     53    9,124

54.0% -> 55.0%. Fifteen representatives, fifteen landed, twelve of them on the
first compile. No parks.

## The jump-table work compounded immediately

`func_b03_80170E54` was PARKED at the end of batch 5 because the container
would not take its table. This batch it was the TWIN for two separate
landings: h6's `func_b27_80171E20` (345 words -- that body plus exactly seven,
one unshared arm) and, through `func_b03_80172A24`, h8's
`func_b14_8017293C` (286 words). Both of those own tables of their own and
would have been unlandable the day before.

Ten compiled jump tables are now in the build across nine objects and all four
containers are byte-identical.

**h10 verified its own `.rdata` rather than trusting the ROM layout**, which
is the check I had not thought to ask for: it compiled its unit to `.s` and
confirmed exactly two `.rdata` blocks came out, 12 words then 7, in that
order, with `%hi`/`%lo(jtbl_b03_80170838)` a real relocation and not a folded
constant. Worth asking for every time.

And it found something the ROM cannot tell you: **a function can have two
switches on the same field and only one gets a table.**
`func_b03_80172A24`'s six-case switch does; the three-case `else` arm
compiles to a compare chain, and cross-jumping merges the table switch's 1/2/3
arms INTO the compare chain's -- which is why the table's first three entries
point past the `else` block, and why the inner switch's arms have to be
written 3, 2, 1. Ascending is 318 words / 10 differing, because their emission
order is what the table's `j`s point at. h8 re-measured the same thing in b14.

## Two more dngclone defects, both general

* **A NEGATIVE ASM IMMEDIATE CAN BE A POSITIVE C LITERAL UNDER A MINUS.**
  `func_b06_80174214` writes `(s16)(dx - 0x10)`; the listing says
  `addiu -0x10`. Three members want `+0x10`, so the search for -16 found
  nothing and they were refused with "appears only 0 times in the code". The
  matcher reads that spelling now, and because the sign lives in the OPERATOR
  the substitution rewrites `dx - 0x10` as `dx + 0x10`. Third spelling of the
  same lesson after the plain literal and the `~` mask.

* **A CLONE NEEDS DECLARATIONS THE REPRESENTATIVE NEVER DID.**
  `func_b06_80173E94` is defined EARLIER in ovl_dng_b06, so the rep needed no
  forward declaration and `needed_decls` had nothing to carry. In the target
  unit the renamed callee sits below the insertion point, and the compile
  failed with `func_s01_80024024' undeclared -- naming a symbol that exists,
  in a file nobody wrote by hand. `local_callee_sigs()` takes the prototype
  from the rep unit's own definition and renames it with the body.

`ovl_dng_s51` is the best advertisement yet for returning candidate spellings
instead of refusing: it wanted `0x1f -> 0xf` AND `-0x10 -> -0x8`, each at 2 of
3 sites, and the verifier picked which two.

## And one defect an AGENT found that the tools did not

h6 noticed `s32 func_b03_80171DA0(...)` declared in `csrc/ovl_dng_b27.c` --
another bank's function, in a unit that neither calls nor defines it. There
were **32 of them across 28 units**.

`needed_decls` searched the cloned body INCLUDING ITS LEADING COMMENT, and a
landed body's header names its twin: "TWIN: the landed carved sibling
func_b03_80171DA0". That pulled the twin's declaration into every clone of the
family. Harmless to the link, since an unused declaration emits nothing, and
wrong in every other way -- and exactly the sort of thing a later agent reads
and believes. The search runs on code only now, and all 32 are removed.

Same lesson as `code_mask` in `vary_consts`: **a comment is prose, not a
reference.** Twice now, in two different tools, the fix has been to stop
reading the comment as code. Anything in this pipeline that greps a body
should be asked which of the two it means.

## The audit I said I would do: three places, not two

After batch 6's stray declarations I went looking for every other place the
pipeline greps a function body, on the rule that **a comment is prose, not
code**. There was a third, and it is the most dangerous of the three.

`def_match` and `defined_funcs` find a DEFINITION with a line-anchored
`^[A-Za-z_]...name(` pattern. A comment line at column zero reading

    func_b03_80170854(p, b, c, d) is the twin

matches that exactly, and `definition()` would then take the COMMENT's span
and scan on for a balanced brace -- splicing over whatever lies between. That
is the same shape as two of this tool's worst defects (the forward
declaration that swallowed seven stubs, and the comment walk-back that
swallowed a whole previous function), both of which produced
`redefinition of ...` errors naming lines nobody had written.

Nothing in `csrc/` triggers it today -- I checked all 1,049 definitions and
every column-0 comment line in every unit, and found zero -- but agents write
comments full of code and clang-format does not reindent a comment's
interior, so it was a matter of time. `code_view()` returns the text with
every comment character replaced by a space, SAME LENGTH so match offsets
still index the original, and both patterns run on that. Regression: all
1,049 definitions still resolve and every one still ends in a closing brace;
the hostile comment above now yields nothing.

## Batch 7 and the inflection

**The replication multiplier is spent.** Recoverable-by-replication is down
to 7.3% from 13.4% at the carve, 1,321 distinct functions remain of 1,526
stubs, and most surviving families have TWO members. A 31-word body with 17
copies was worth a long fight; a 2-copy one is worth about its own words.

So the lever moved again -- from agents per batch (batches 1-4), to functions
per agent (batches 5-6), to WORDS per agent. Batch 7 packs each agent to ~800
words of representative body instead of ~480, which raised the batch from
5,868 words of yield to 7,441 for the same five agents. Ranking is by family
yield still, but the budget is what binds now.

The brief says this plainly rather than repeating the old "a 31-word body
deserves the same fight" line, which is no longer true of anything left.

# Batch 7, and what the twin map changed

    agent  unit         landed  parked  clones  words
    h11    ovl_dng_b09     3       2       3    1,064
    h12    ovl_dng_b02     4       -       4    1,614
    h14    ovl_dng_b15     6       -       7    1,319
    h15    ovl_dng_b11     1       -       5    1,302
    h13    ovl_dng_s20     2       2       -    (still running)

55.0% -> 56.0% with four of five integrated. **`ovl_dng_b11.c` is the first of
the 116 carved objects with no stubs left.**

Two measured parks, both from h11 and both honest about being one thing away:
`func_b09_80170A84` at 6 aligned differences (one load pair's output order)
and `func_b09_801745BC` at 19 (one missing register-to-register copy and the
18 renames it causes). Both name the permuter as the next step, which is
right: it is the dispatcher's budget and this is what it is for.

## THE TWIN MAP: 45% of the "plain decompilation" is not plain

h11's report named `tools/decomp/xsibs.py`, which the brief had never
mentioned. It compares a unit's stubs against every function LANDED ANYWHERE
in the project -- read out of the reference images, so it needs no surviving
listing -- on the opcode stream with registers, immediates and jump targets
masked. Running it across all 115 units with stubs takes 2.5 minutes and says:

    458 stubs have a landed near-sibling at ratio >= 0.70
     94,863 words -- 45% of the 212,075 words I had just called 1:1 work

And ten resident functions carry most of it:

    func_800D92C0   47 stubs  21,743 words
    func_800DA014   43         10,654
    func_800DA398   45          9,074
    func_800D991C   35          7,809
    func_800D8728   26          7,024
    func_800D904C   45          6,414

So the previous section's inflection was half wrong. Replication by BYTE
IDENTITY is spent -- 6.2% recoverable, and `dngfam` is right about that. But
replication by SHAPE is not: these are the same functions relinked with
per-bank deltas big enough to change the word count, which is exactly what
`dngfam`'s hex normalisation cannot see and what an opcode-stream ratio can.

### What that changes

* **Batch selection.** `dngpick.py` ranks by `copies * words` over all stubs;
  batch 8 instead merges that with the twin map and prefers a strong twin on
  equal yield. Family-only picked 5,797 words of yield, twin-only 4,442, the
  merge 8,124.
* **The brief leads with `xsibs` now**, and each function in the task table
  arrives with its twin and ratio already named. Two agents independently
  called it their most useful first command and it was not in the brief.
* **A miss is not a verdict.** h12 had two functions `xsibs` found nothing for
  at ratio 0.40 and landed both: `func_b02_80173560`'s state 0 is the landed
  `func_b14_8017293C`'s head and `func_b02_80174AD4`'s whole prologue is
  `func_800B8FC8`'s. Shape similarity cannot see a partial match; the callee
  set can. That is in the brief too.

### The obvious next lever, not taken yet

The work clusters by TWIN, not by unit -- 47 stubs across 47 banks are all
relinks of `func_800D92C0`. An agent given ten of those would land the first
and take the rest as deltas, which is the compounding h14 saw inside one unit
but an order of magnitude bigger. It needs one agent to hold several unit
copies at once, which the brief and `dngland` both assume away. That is the
change to make if these batches stop paying.

## CORRECTION to the twin map figures above

**The numbers in the previous section are wrong and the error was mine.**
`xsibs.py` reads the stub side out of `asm/nonmatchings/<unit>/`, and splat
regenerates a listing for every function in a segment -- including the ones
already LANDED. I did not filter its output against `dngfam.stubs()`, so the
map counted work already done. `func_b01_80172800` is in it and has been a
definition in `csrc/ovl_dng_b01.c` since batch 1.

Filtered to actual stubs:

                      claimed        actual
    stubs with a twin   458            419
    words            94,863         44,220
    share of the rest    45%            18%

    >=0.95        34 stubs   2,631 words
    0.88-0.95     50         4,224
    0.78-0.88    180        23,952
    0.70-0.78    155        13,413

A second, smaller error in the same analysis, in the other direction: the
first run passed `--min-words 40`, so 299 stubs under 40 words came back
twinless by construction. h17 landed two 22-word functions off a resident
twin the map had told me did not exist. Re-running with `--min-words 8` is
what produced the 419.

**What survives the correction.** The direction was right and the size was
not: shape replication is real and worth selecting on -- 419 stubs is 28% of
what is left, and the 264 at ratio >= 0.78 are a materially faster class of
work than the rest. Batch 8's dispatch is unaffected, because its function
list was built by iterating the STUB set and looking twins up, not by
iterating the map; only the labels and these statistics were wrong.

**What does not survive.** "45% of the plain decompilation is not plain" was
an overstatement. 82% of the remaining words have no shape template at
ratio 0.70, and the ten-resident-functions-carry-most-of-it claim was
inflated the same way -- `func_800D92C0`'s 47 stubs are 20 stubs once the
landed ones come out. The twin-cluster lever at the end of that section is
still the right next move, but it is worth roughly half what I said.

**The lesson for the tooling.** Every one of these tools has a notion of
"remaining work" and they do not agree: `dngfam` reads `csrc/`'s INCLUDE_ASM
lines (the authority), `xsibs` reads the listing directory, `guard.py` reads
both. Anything that joins two of them has to filter through `dngfam.stubs()`
first. `dngpick.py` does; my ad-hoc analysis did not.

## h13 closed out batch 7, and `decl_block` read a comment as code -- the FOURTH time

Its two landings would not splice: `syntax error before `is''. Its
declaration comment reads

    /* ... func_s20_80025340 writes it, func_s20_80024EF4 reads it;
       D_800264F8 is &D_80026478[0x10], its second bank. */

and `decl_block` closes a statement on a line ending in `;` without asking
whether the `;` is in CODE. The first line closed one mid-comment, the
remainder became a statement beginning `D_800264F8 is &...', and the typedef
after it was orphaned.

That is the fourth tool in this pipeline to read a comment as code, after
`vary_consts` (code_mask), `needed_decls` (decl_code) and `def_match`
(code_view). Each one was found by a different agent's perfectly ordinary
prose. **The rule is now mechanical: anything in this pipeline that scans a
body or a block for syntax must mask comments first.** `decl_block` does.

## h13's two parks are the best residue work of the sweep

`func_s20_80026010` is 170 words with 23 aligned differences, every one two
locals exchanging `$s0` and `$s1`, and the agent took the `-da` probe recipe
and found something that AMENDS the lever file rather than using it: the sort
is `local-alloc`'s, not `global_alloc`'s -- `;; N regs to allocate:` lists
only local quantities -- and `combine_regs` has tied its opponent's whole
address chain into one quantity of ~32 refs. It then measured all three
free-reference devices and showed each one LOSES here, with the probe
explaining why: the `__asm__` barrier makes its own pseudo read 8 times
across 29 insns instead of 6 across 9, so the priority FALLS.

It also confirmed the duplicated-tail entry's zero-word claim and showed it
useless in this body, because cross-jumping merges a tail after the join
while the target wants that store in a branch's delay slot.

That is a better outcome than a landing: the lever file now says when its own
devices do not apply, and how to tell.

## A CLONE RUN LANDED WORK A LIVE AGENT WAS STILL DOING

agent-h17 finished its six `ovl_dng_b28` functions and then had to tell ME
that three of them were already landed. They were, and I landed them:
`func_b28_80173254` and `func_b28_8017357C` are family members of functions
other batch-8 agents landed, and cloning those reached into b28 while h17 was
working. 611 words of its run went on work that was already done.

Nothing was WRONG -- h17's own bodies measure 0 differing too, and its
`8017357C` is structurally identical to the clone's -- but this is a
dispatcher defect and it was mine, not the agent's. Two fixes:

* **`dngpick.py --busy` is family-aware now.** It already skipped a live
  agent's units; it also drops every stub elsewhere that shares a FAMILY with
  one of them, because a landing in a busy unit clones into its members
  wherever they live. On the next batch that held 28 extra stubs.
* **`dngclone.py --hold <tags>`** skips members in a named unit, so a clone
  cannot reach into a unit an agent is holding even when the family was
  picked before that agent started.

The general shape, worth remembering because the sweep's parallelism rests on
it: **the unit of exclusion is the FAMILY, not the unit.** Two agents in
different units are only independent if no family spans them, and 1199 of
this block's stubs started life as copies of a sibling.

## What h17 and h19 did with the reference devices, together

They are the best pair of measurements the sweep has produced on
`allocno_compare`, and they disagree in a way that resolves the lever:

* **h17 aimed a device.** `func_b28_8017357C`'s residue was 16 differences of
  `p`/`b` swapped, and off `-da` it read `p` at 3*9/198 = 0.1364 against `b`'s
  2*5/74 = 0.1351 -- a margin of one part in a hundred. A
  `do { } while (0)` around `func_800AA888(p, b, c, d)` puts `b` at 2*6/74 =
  0.1621 over `p`'s 3*10/198 = 0.1515. The SAME device around state 0's bump,
  which is where `func_b09_80173D4C`'s lever puts it, changes nothing at all,
  because it lifts `p` and `p` is not the one that needs lifting.
* **h19 priced a device against plain C and chose plain C.**
  `func_b44_80174438` is the same shape; it swept all fifteen `do-while`
  positions (three reach 0, the rest 15 to 136, two cost a word) and took the
  duplicated dress tail instead, because the twin already uses it and jump2
  makes it word-free.
* **And h19 showed the fix does not transfer.** `func_b44_80174110` and
  `func_b44_80174438` are the same pair of banked tails `ovl_dng_b03` carries,
  and they disagree: 80174110 measures 0 with the tail written once OR twice,
  80174438 needs it twice. A twin's documented FACTS transfer; its ALLOCATION
  fix must be priced per body.

Then h17 withdrew the device it had reported, because the duplicated tail
reached 0 without one and the house rule prefers plain C. Both spellings are
in its comment with scores.

## Three more `vary_consts` gaps, and the shape they share

Every one of batch 8's clone refusals was a spelling the substituter could not
see, and all three are now closed. The pattern across all seven of these
(four earlier, three here) is worth stating once: **`const_pairs` reports a
differing IMMEDIATE, and the question is always how that immediate is SPELLED
in C.** The answers so far:

1. the literal itself (`0x9`);
2. a one's-complement mask -- `&= ~8` is `addiu -0x9`;
3. a positive literal under a minus -- `dx - 0x10` is `addiu -0x10`, and the
   substitution has to rewrite the OPERATOR;
4. one C access emitting the immediate twice -- `*(s32 *)(p + 0xA8) +=` is a
   load and a store, so five asm sites against four spellings is normal;
5. fewer asm sites than spellings, which is the only genuinely ambiguous case
   and enumerates placements for the verifier to choose between;
6. **a DERIVED offset, new here.** `*(UnkDngS58Name12 *)((u8 *)n + 0x58)`
   writes 0x58 once and emits `lwl`/`lwr`/`swl`/`swr` at 0x58, 0x5B, 0x5C,
   0x5F, 0x60 and 0x63. A sibling with another base differs at all six and
   five appear nowhere in the source. Substituting 0x58 is the whole job; the
   test is that the unplaceable pair's DELTA matches one being substituted.
   Both members of `func_s58_8002553C`'s family were refused over this.

None of the seven ever produced a wrong landing -- each one cost a refusal,
which is what makes `dngfam`'s loose hex normalisation safe. But six of the
seven were found by a family failing rather than by design, so the rule going
in: when a refusal says a constant "does not appear in the code", ask how the
compiler could have derived it before assuming the family is wrong.

`func_s65_8002405C` stayed refused at 1 differing, which is the tool being
right: that member is genuinely different code and is 42 words of real work.

## h17's three corrections to the dispatcher

All three were things only an agent reading the actual files could see:

* **`xsibs --min-words 40` hides the short twins.** Both of h17's 22-word
  forwarders came back "none found" in a brief rendered at 40 and both sit at
  ratio 0.900 to a landed 24-word body. The brief says 18 now. (I had already
  hit this on the twin map and fixed it THERE without fixing the brief, which
  is its own small lesson.)
* **A duplicated declaration in `csrc/ovl_dng_b28.c`,** `func_b28_801737DC`
  twice in a row. `insert_decls` deduped on exact text, and clang-format
  reflows a long prototype after a landing, so the next clone adds its own
  copy. 11 had accumulated across 10 units. It compares on a
  whitespace-normalised form now.
* **`-da` belongs in the brief next to the twin search.** h17 used it to rule
  out a device BEFORE compiling it -- the obvious `do { } while (0)` lifts the
  wrong allocno -- and that is the second agent to reach for it unprompted.

## h20 wrote down the `func_800D92C0` relink, which is 47 stubs' worth

`func_b25_801726EC` is 456 words and the single most reusable landing of the
sweep so far, because `func_800D92C0` is the twin `xsibs` names for more
remaining stubs than any other. h20's comment records the 456-word variant as
`func_b01_80171C34`'s 466-word body with FOUR deltas, and accounts for the
whole ten-word saving:

1. `s32 i` not `s16 i` for the sweep index -- which enables loop strength
   reduction, so the `D_8006CD00` lookup becomes `lh $v0, 0($s1)` with an
   `addiu $s1, $s1, 2` in the tail and the three index tests lose their
   `sll`/`sra` pairs;
2. the index table named INLINE, with NO `s16 *tbl` local -- a local is 457
   words / 21 differences, because it drops the loop's insn count below gcc
   2.7.2's invariant threshold (`loop.c:532/1631`) and the halfword test's
   extension gets hoisted into the preheader;
3. `D_8006CCD8` IS hoisted, as a `u8 *tb` set between `i = 0;` and the loop
   and used `*(u8 *)(kk + (s32)tb)` -- kk FIRST, or the `addu` operands come
   out swapped. That is the OPPOSITE of `func_b01_80171C34`'s two-set device;
4. the tail is `i = (s16)func_800BCB04(...)` into the `s32 i`, not the
   `s16`-returning `func_800BCB04h` alias.

And the devices do not carry: `func_b01_80171C34`'s `do { return; } while (0)`
must be REMOVED (460 words / 30 differences with it) while its
`cc2 = 0x80000000` do-while is kept verbatim. That is the third independent
demonstration that **a twin's facts transfer and its devices must be
re-priced.**

## A clone can hit an arity disagreement in the TARGET unit

`func_b25_8017165C` cloned into one of its three members and failed the third:
`too few arguments to function func_b31f_8016ADB8`. A body already landed in
`ovl_dng_b31f` calls that function with fewer arguments than the cloned
definition declares -- the original is K&R and passes whatever is in the
argument registers, which compiles fine as asm and not at all against a
prototype.

`fix_forward_decl` handles the mirror of this (a stale forward DECLARATION of
the function being added); this is a landed CALLER, so the fix is a unit-local
alias declaration of the shorter arity, which leaves both landed bodies alone.
73 words, one member, so it is recorded rather than automated -- but if this
recurs, the alias is the pattern and `csrc/ovl_dng_b28.c` already carries a
worked example of two arities of one symbol coexisting (`func_800BCB04` and
`func_800BCB04h`).

## DO NOT INTEGRATE FROM A LIVE AGENT'S COPY

h20 sent an interim line saying all five of its functions were landed, and I
ran `dngland` against its working file before the hand-back arrived. The
BODIES were right -- all five verify at 0 differing and the clone of the
456-word one is byte-identical -- but two of them still carried `/* WIP3 */`
and `/* WIP5 */` where the real comments were about to go, and that is what
reached `csrc/`. The agent caught it and told me.

The comments are the most valuable thing this sweep produces after the code.
A body with a placeholder comment is worse than one with none, because the
next agent greps for a twin's header and finds nothing where something should
be. So: **an interim "LANDED" means the body verifies, not that the work is
finished.** Integrate on the hand-back.

Re-synced both comments from the agent's final copy, asserting the bodies were
byte-identical first so only the headers moved, and re-verified. I also
checked the one other agent I had integrated early (h9, five functions in
ovl_dng_b06) against its final copy: all five identical, so b25 was the only
casualty.

## Batch 8 complete: 57.3%, and one park closed itself

    agent  unit          landed  clones
    h16    ovl_dng_s58      7      10
    h17    ovl_dng_b28      6       2
    h18    ovl_dng_s21      7       1
    h19    ovl_dng_b44      6       6
    h20    ovl_dng_b25      5       3

31 of 31 representatives landed, no parks written. 56.1% -> 57.3%.

**h18 closed h13's park without being asked to.** `func_s21_800255E8` is the
two-member family whose other half was `docs/dng-parks/func_s20_80025A58.c`,
parked at 7 aligned differences, and h18 landed it -- so the clone carried the
fix across and `func_s20_80025A58` verified at 253/0 on its own. The park is
retired.

The fix is worth having in its own right: the residue was one window where a
counter's `lhu`/`addiu`/`sh` and a corner's `lhu`/`lhu`/`subu`/`sh` interleave
wrongly, and `sched.c`'s backward scan ties both stores on `INSN_PRIORITY`
(a store's destination is a MEM, so `birthing_insn_p` can never boost it) and
falls back to `INSN_LUID` -- source order. A single statement cannot split
finely enough. Reading the counter BEFORE the corner, computing the corner
into a local, storing the counter BETWEEN them and the corner LAST puts the
four LUID groups in the order the target wants. h18 also closed h13's open
question negatively: the "give the temporary a second flow-counted set"
experiment is what a shared local does in plain C, and every version is worse
(11, 19, 51), because the tie is decided before any boost can apply.

**And batch 8 hit the clone collision twice more**, h17 three times and h18
once, because the fix went into `dngpick` after batch 8 was already dispatched.
Both agents' duplicated work verified identically to the clones, so nothing
was lost but time. Batch 9 is the first picked with the family-aware `--busy`.

## The permuter: 6 aligned differences to 3, not a landing

45 minutes, 333,405 iterations, 8 jobs on `func_b09_80170A84`. Score 455 ->
20, which measures as **6 aligned differences -> 3**, and the three left are
one window of `$v0`/`$v1` exchanged with the loads already in the right order
-- register allocation, not scheduling or selection.

It found two things: a pointer local for one of three velocity stores, and a
DUPLICATED ARM under an always-true guard (`if (p || k) { t = -t; } else
{ t = -t; }` -- `p` is a non-null parameter). The second is this project's
duplicated-arm device arrived at mechanically.

I then tried three spellings of the second abs block, the treatment that
fixed the first, and none beat 3 -- which is the same asymmetry the park's own
table records, since one of the two loads is alias-dependent on the preceding
stores and the other is not.

Stopped there deliberately. It is a 123-word function with one sibling, 246
words of yield, against 1,433 stubs remaining. The park is better than it was
and says exactly what to measure next; that is the right outcome for the
budget spent, and the honest summary is that the permuter halved a residue it
could not close.

## 14 units were calling a function with NO DECLARATION AT ALL

h22 noticed, in passing, that `func_b18_8017087C` -- landed, 117 words, 0
differing -- was calling `func_8004491C` with no declaration in scope, i.e.
against gcc's implicit `int func_8004491C()`. It added the real prototype and
re-measured.

That is a class, not a one-off. `cdkgcc -Wall` over all 116 units:

    func_8003FD64   12 units   b06 s01 s02 s16 s32 s37 s39 s40 s41 s42 s51 s58
    func_80069EF8    2 units   b15 b19

`func_8003FD64` returns `UnkNode *`. An implicit `int` return happens to work
on this target because `sizeof(int) == sizeof(void *) == 4` and both come back
in `$v0`, which is exactly why it has been silent through nine batches: every
one of those bodies verified at 0 differing WITH the implicit declaration.

All 14 now carry the real prototype, and I re-verified every function in each
affected unit rather than just the callers -- 109 functions across the 14.
**None regressed**, so the implicit declarations were not load-bearing. That
is the result I wanted either way: if one of them HAD moved a word, the
implicit `int` would have been doing real work and the landing would have been
resting on it.

Worth noting what made this findable at all: `-Wall`. The project does not
build with it, gcc 2.7.2 says nothing without it, and `ovlverify` compiles the
same way the build does -- so nothing in the normal loop could ever have
reported this. A periodic `-Wall` sweep over the carved units is cheap and
belongs in the routine.

## Batch 9's methodological result: the callee-set grep is the primary tool now

It found a twin `xsibs` had scored below threshold SIX times across batches 8
and 9 -- h12 twice, h18, h21, h22 twice -- and h22's case is the strongest
argument the sweep has produced for the mandatory comments:
`func_b18_80172AB4` is 422 words, its twin `func_b03_80172A24` is 316, and the
size ratio puts it under any shape floor. It landed on the FIRST COMPILE
because h10 had written four measured spelling facts into that twin two
batches earlier. One grep was worth 499 of h22's 844 words.

So the brief now carries the operational recipe -- grep four or five
distinctive symbols ONE AT A TIME over the landed side and intersect the file
lists -- plus h25's refinement: **pick the symbols the TAIL uses, not just
the callees.** For `func_b35_801728C4` the `func_800xxxxx` callees found
nothing and two distinctive globals found it at once, because those are what
the shared teardown touches.

New lever (1112), also found twice independently in this batch: **a block
emitted in the wrong place at the right word count is usually an inverted
two-arm gate.** 17 and 7 aligned differences in the two measured cases, and it
is one character to test.

## THE CLONE COLLISION, THIRD TIME, AND THE ACTUAL FIX

Batch 9 gave `b12`, `b18` and `b20` to three different agents, and all three
units hold a member of one 345-word family. Whichever landed first cloned
into the other two, so h23 arrived to find `func_b20_8017102C` already done
and h21 and h22 duplicated each other's 345-word work.

I had "fixed" this after batch 8 and the fix was incomplete.
`dngpick --busy` excludes families that span THIS batch and a LIVE agent's
unit; it said nothing about two units inside the same batch. Batch 9 was
picked with no agents running, so the guard never engaged.

The picker now builds the batch by taking units in rank order and **skipping
any whose selection would re-use a family already claimed by a unit already
chosen**. On the next pick that rejected `s18`, `s59` and `b07` by name.

Three occurrences, and the lesson is about the shape of the fix rather than
the bug: twice I patched the case in front of me (first the clone tool, then
the cross-batch pick) without asking what the invariant was. The invariant is
that **a family is claimed by at most one agent, ever** -- not "not by a busy
unit", not "not by another batch". Both earlier fixes are special cases of it
and neither implied the other.

## Batch 9: 58.2% with FOUR of five agents in

    agent  unit          landed  clones
    h22    ovl_dng_b18      3       2
    h23    ovl_dng_b20      3       1    (its 4th was done by a clone)
    h24    ovl_dng_b23      5       6
    h25    ovl_dng_b35      5       3
    h21    ovl_dng_b12      -       -    STILL RUNNING

16 representatives landed, no parks. 57.3% -> 58.2%.

**CORRECTION.** The first version of this section said "21 of 21
representatives landed" and gave h21 four landings with a `?` for its clones.
That was wrong: h21 had not handed back, I never ran `dngland` on it, and
three of its four functions are still `INCLUDE_ASM` in `csrc/ovl_dng_b12.c`.
It has three landed in its own copy and is still working the fourth
(`func_b12_80173FDC`, 364 words, no twin at any ratio, 132 aligned
differences at last report).

I wrote a `?` into a results table instead of going to look, and then
summarised the table as complete. The `?` was the signal and I summarised
past it.

Three of h24's five and three of h23's four had no twin at any ratio and were
read off the listing; h23's `func_b20_80174D14` (330 words) landed using two
idioms from a function **in its own unit that it installs as a handler**,
which is where the twin search has ended up: read your neighbours before you
search the project.

## Two refinements to the callee-set recipe, from the agents that used it

* **h24: `grep -l` on single callees is useless in this block** -- one callee
  returns 15 to 35 files. What worked was scoring whole function bodies
  against five to seven callees at once, which ranked the right twin first
  both times it was tried. The intersection form in the brief works when the
  symbols are distinctive; the scorer works generally.
* **h25: pick the symbols the TAIL uses.** For `func_b35_801728C4` the
  `func_800xxxxx` callees found nothing and two distinctive globals found it
  immediately, because those are what the shared teardown touches.

## A redundant typedef, noted and left alone

`ovl_dng_s19`, `s20` and `s21` each define TWO typedefs of the identical
scratchpad shape -- `UnkDng73E94SP` (which 33 other units use) and a
locally-named second one. Both are referenced, so consolidating means
renaming uses inside landed bodies. Pure cosmetics against a re-verification
cost, so it is recorded here and not done. The agents' own choice to keep
`UnkDng26478` and `UnkDng265C8` separate IS justified: same shape, different
objects, and h13 said so explicitly.

## I DID IT AGAIN: integrated a live agent's copy, and now there is a guard

Two batches ago I integrated h20's working file on an interim "LANDED" line,
shipped two `/* WIP3 */` comments into `csrc/`, wrote the rule into this
document, and committed it. Then I did exactly the same thing to h23: three
`/* WIP ... */` markers in `csrc/ovl_dng_b20.c`, and this time **a clone
inherited one** -- `func_b42_80170DD0` carried `/* WIP func_b20_8017087C */`
into a unit no agent had ever opened.

Both agents found it and told me. Neither time did the code suffer: the bodies
verified, and re-syncing from the final copies with the bodies asserted
byte-identical changed only the headers.

Writing the rule down did not work, so the rule is now mechanical.
`placeholder_comment()` matches WIP / TODO / FIXME / XXX / HACK /
PLACEHOLDER in a body's leading comment, and:

* **`dngland` refuses to splice** such a body and writes nothing, saying that
  an interim "LANDED" means the body verifies and not that the write-up is
  done;
* **`dngclone` refuses to clone** such a representative, because that is how
  the marker reached b42.

The general point, which is the one I keep having to relearn: **when a
process failure repeats, the fix belongs in the tool and not in the
documentation.** I had already written "integrate on the hand-back" in this
file in a section titled DO NOT INTEGRATE FROM A LIVE AGENT'S COPY. It did
not survive the next batch's impatience.

h23 also noted the other half of the same mistake: the snapshot in `csrc/`
had my own reconciled prototype for `func_b20_8017102C` where its copy keeps
the unit's pre-existing unprototyped form. Both verify; the hazard is ending
up with both, which the re-sync resolved.

## 227 call sites were spelling a named function by its raw address

h21's park identifies `func_80065770` as libgte's `RotTransPersN`. It is
right -- and `splat/common_syms.txt` has carried
`RotTransPersN = 0x80065770;` all along. The identification was correct and
redundant, which is the useful part: it means the carved units have been
writing raw addresses for functions the project already names.

    rand            129 sites in 21 units
    RotTransPers     30 sites in  4
    RotTransSV       14 sites in  2
    AddPrim          12 sites in  2
    RotTransPers4     6, SetPolyG4 6, PushMatrix 6, PopMatrix 6,
    SetRotMatrix      6, SetTransMatrix 6, RotMatrix 6

`rand` is the one that matters. Agents have been writing
`func_80069EF8() & 0x3F` where `rand() & 0x3F` was available, and `rand()` was
ALREADY the established spelling -- the resident `ovl_dungeon_800D50FC.c` uses
it, and three carved units did too. So this was an inconsistency between
agents rather than a convention anyone had chosen, and half the block was
writing an address where the other half wrote what the code means.

All 227 are renamed, each unit re-verified function by function (248
functions across 22 units, all still 0 differing), and the units' now-dead
local declarations removed. Four containers byte-identical, 0 raw references
left.

**One thing the rename taught me about my own tooling.** Two units failed the
first pass with `conflicting types for RotTransPers4'. My removal regex
required the declaration's `;` on the same line, and clang-format had wrapped
that 10-argument prototype across two -- so the local declaration survived,
got renamed, and then collided with `libgte.h`'s. Removing declarations
STATEMENT-WISE through `decl_block` fixed both. That is the fifth time in this
sweep a line-oriented assumption about C source has been wrong; `decl_block`
exists precisely because of the first four, and I still reached for a regex.

**Why the brief should ask for the real name.** A raw `func_80069EF8()` tells
a later agent nothing; `rand()` tells it the function picks a random number,
which is the single most useful fact about the surrounding code. The check is
one grep against `splat/common_syms.txt`.

## A NAME FROM THE WRONG SYMBOL FILE WOULD BE UNCATCHABLE

The rename above was safe, and it was safe by accident of which file I happened
to read. Worth writing down properly, because the obvious next step -- "do the
other symbol files too" -- is the mistake.

`splat/common_syms.txt`'s 219 names are all at 0x8003c9f8..0x8006a3a8, below
the overlay slot: resident library and BIOS routines, valid whichever overlay
is mapped. The per-container `splat/<container>_symbol_addrs.txt` files name
code INSIDE the shared slot at 0x80088760..0x800f7700, and an address in there
is DIFFERENT CODE depending on which overlay is loaded. town.bin's
`plt_dm_kicked = 0x80098A98` is a true name of TOWN.BIN's code and a lie in a
dungeon unit, because with DUNGEON.BIN mapped that address is dungeon code.

**And nothing in this project could catch it.** The symbol exists, so it
links; the address is the same, so the bytes are identical; so `make
containers` is byte-identical and `ovlverify` reports 0 differing. The name
would be the only false thing in the file, and byte-exactness -- the one
check this whole sweep rests on -- is structurally blind to it.

`tools/decomp/symsafe.py` audits for it: per-container names that sit in the
slot, used in the CODE (not comments) of a unit belonging to a different
container. Currently 0. The brief now says `common_syms.txt` and nothing else,
with the reason.

That is the second check this session that byte-exactness cannot perform --
the first was the jump table in `.rodata`, which `ovlverify` cannot see
because it measures a function's own words. Both are worth remembering when
"four containers byte-identical" starts to feel like proof of everything.
