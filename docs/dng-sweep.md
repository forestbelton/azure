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
