# BATCH 35 BRIEF

## USE THE cfuncs MCP TOOLS FOR C -- DO NOT WRITE A SCRIPT FOR IT

**Before you write any Python to find, read or edit a function, check for these
tools.** They exist specifically to stop one-off scripts being written for
this, and they are backed by tree-sitter rather than a pattern:

* **`describe_functions`** -- every definition in a file, with exact source
  spans. (Renamed 2026-11-25; it was `list_functions`.)
* **`describe_function`** -- one function.
* **`edit_function`** -- change one function in place.

**FOUR TOOLS, ALL OF THEM VERIFIED BY THE DISPATCHER, AND THEY NOW COVER THE
WHOLE SWEEP LOOP.** They arrive **DEFERRED**: only the names are visible until
you load the schemas, so run this FIRST, once:

    ToolSearch select:mcp__cfuncs__describe_functions,mcp__cfuncs__describe_function,mcp__cfuncs__describe_stubs,mcp__cfuncs__edit_function

* `describe_functions(filepath, include_body=false)` -- every definition with
  exact spans. `include_body` is off by default because bodies are most of the
  bytes; leave it off unless you want the text.
* `describe_function(filepath, name, include_body=false)` -- one definition, or
  **null** if that name is not defined. It never matches a forward declaration
  or a mention in a comment, which is the bug that used to make park prose
  unparseable.
* `describe_stubs(filepath)` -- **the INCLUDE_ASM stubs**, which no C parser
  sees as functions. This is the other half of "what is in this file" and it is
  found structurally, so a comment or string mentioning `INCLUDE_ASM` is
  excluded. Use it instead of grepping; a text count once reported one stub for
  a unit that had none.
* `edit_function(filepath, name, body?, signature?, output?)` -- rewrite a body,
  a parameter list, or both. **`output` writes to a DIFFERENT path and leaves
  the input alone, which is exactly what a sweep wants:** read the committed
  unit, lay each cell down in its own working copy. Nothing is written if the
  result would add a syntax error. It returns the function as it reads
  afterwards, so you do not need a follow-up `describe` to confirm.

**AND THE HEADER COMMENT IS READABLE AND WRITABLE -- `include_comment` AND
`comment`, BOTH OPT-IN.** This is the one that stops you hand-splicing a park.

* `describe_function(filepath, name, include_comment=true)` returns the
  `/* ... */` block immediately above the definition, verbatim, as `comment`.
  Verified on a 27-line landed-body header in `csrc/` and on a 200-line PARK
  header. **It is off by default and your tool schema may not list it** -- pass
  it anyway, the server accepts it.
* `edit_function(filepath, name, comment="/* ... */")` rewrites that block and
  leaves the body alone; combine with `body=` to change both, and with
  `output=` to write the result elsewhere. Verified: comment replaced, body
  preserved byte for byte, input file untouched, sibling functions intact.

So a park write-up is now: `describe_function(..., include_comment=true)` to
read the header, then `edit_function(..., comment=...)` to replace it. **Do not
locate a header by counting lines backwards from the definition, and do not
regex for `/*`** -- that is what the dispatcher did an hour before this existed,
and agents doing it were getting the boundaries wrong.

**THE SWEEP LOOP, WITH NO PYTHON AT ALL:** `describe_function(..., include_body=true)`
to get the text, then one `edit_function(..., body=<cell>, output=<variant path>)`
per cell, then `ovlverify`/`aligndiff` on each variant. The dispatcher ran a
4-cell sweep this way and the tool-made variant measured **identically** to the
scripted one (283 words / 2 rows both ways).

**They work on ANY path in the repo**, and the agent scratch tree now lives at
`scratch/azure-dng/` inside it, so **your own working copy is readable too** --
the old outside-the-repo refusal is gone. Your copy's path is given below.

**Meanwhile, and regardless of the answer, the path that definitely works is
`uv run python tools/decomp/cfuncs.py`:**

    --stubs FILE          the INCLUDE_ASM names, from COMMENT-STRIPPED text
                          (a plain grep counts prose mentions -- that is how a
                          unit with zero stubs once read as having one)
    --written COPY CSRC   which stubs your copy has landed
    FILE NAME             one function's exact span

**AND YOU MUST, BECAUSE `ovlverify` CANNOT SEE ARM ORDER AT ALL.** Measured:
two arm orders of one 350-word switch BOTH read 350/350 and 0 aligned rows,
while the emitted table's last three entries were reversed between them.
ovlverify compares one function's words and says nothing about `.rodata`, and a
jump table IS `.rodata`. So a table owner can reach 0 differing with the wrong
arm order and only `make containers` -- which you cannot run -- would catch it.
**If you own a table, run the standalone compile and REPORT the emitted
entry-address order in your hand-back.** A zero is not proof for a table owner.

**SETTLE A SWITCH'S ARM ORDER BY COMPILING, NOT BY SWEEPING.** Run
`tools/cdkgcc -S <your unit>` -- a standalone compile, no build and no splat --
and read the emitted `.rdata` table's labels in ADDRESS order, then compare that
sequence to the listing's. That is a direct measurement of the arm order your
source produces, available BEFORE you finish the body, and it is the only way to
check an order whose arms cross-join to one block, where the gate is blind by
construction. Two agents found this independently; the figures are in
docs/matching-levers.md. Do not infer arm order from a word count.

**`cdkgcc` NEEDS `-o` BEFORE THE INPUT FILE, or it silently writes nothing.**
`tools/cdkgcc -O2 -G0 -I... -S <copy> -o out.s` is the working order; the reverse
produces no output and no error.

**DO NOT PASS A NAME LIST THROUGH A SHELL VARIABLE, AND "QUOTE IT" IS NOT THE
FIX.** This brief used to say to quote such a list; that advice was wrong. The
Bash tool runs **zsh**, which does not word-split an unquoted parameter at all,
so `ovlverify.py <copy> $NAMES` passes 33 names as ONE argument whether or not
you quote it. It prints "is in none of the split overlays" and exits 1 -- loud
enough -- but **`grep -c` over that output reports 0 rows and 0 zeros rather
than an error**, which is the plausible-wrong-number shape: an agent nearly
logged a clean sweep of nothing. Write the names literally on the command line.

**A WHOLE-UNIT `-da` COMPILE DUMPS ONLY THE FIRST FUNCTION IN THE FILE.** To
read `.greg`, `.lreg`, `.loop` or `.sched` for any other body you must build a
single-function probe. One agent lost a detour to this. And when you do read
them: **`.flow`'s refs and lengths are the wrong numbers to compute allocation
priority from** -- `global_alloc` runs after regmove, sched1 and local-alloc on
recomputed values that are never dumped, so use `-da` for the MECHANISM and the
printed `;; N regs to allocate:` order for the FACTS.

**A `?:` IS NOT A NEUTRAL SPELLING OF A TWO-ARMED CHOICE.** Two independent
passes punish it -- `jump.c:927` hoists one constant set above the comparison
when both arms are bare sets, and `fold-const.c:3736` distributes a unary or
constant-binary operation into BOTH arms of a `COND_EXPR`. Two `if`/`else`
stores cost no words, because `jump2` cross-joins whatever they duplicate.
Measured 678/0 against 678/3 and 678/5.

**A HEADER'S WORD COUNTS ARE MOSTLY REJECTED SPELLINGS -- do not compare them
to the tool and report a contradiction.** A landed body's comment prices every
alternative it measured, so most "N words" figures in it belong to spellings
that were NOT kept. One agent read `this body is 426 words with 124 aligned
differences' -- a priced rejected inline form -- as a stale figure contradicting
`ovlverify''s 425, when the same header states 425 twice as the landed result.
If you think a header disagrees with the tool, find the sentence that says what
was KEPT before reporting it.

**AND MAKE EVERY PRICING EDIT ANCHOR ON THE FUNCTION'S OWN BYTE SPAN.** The
same agent's anchor -- a local assignment plus its following line -- matched
TWICE, the second hit being an already-landed neighbour that shares those two
lines, which would have silently edited a finished body. Slice the span
`describe_function' reports and substitute inside it; assert the match is
unique; and re-measure the neighbour. Relatedly, do NOT pass a name list through a shell
variable at all -- see the note above; quoting is not the fix, because zsh never
word-splits one.

**Do not hand-roll a Python script for any of these.** And **if `parkcheck` crashes,
report your park as UNCHECKED rather than assuming it is clean** -- it takes a
path argument, but that does not make it independent of the shared parser, and
one agent's first park went in unchecked for two hours because the parser was
mid-rename underneath it. One agent's own grep of
a unit's definitions did agree with the parser exactly -- 37 for 37, `diff`
clean -- which is luck rather than a method: the same habit has produced four
plausible wrong numbers in this project, listed below.

**Why this is a rule and not a preference.** Every hand-rolled pattern for this
has been wrong at least once, and each time it returned a **plausible wrong
number instead of an error**:

* a definition finder matched a park's own FORWARD DECLARATION, so `permimport`
  sliced the file from the wrong offset and fed prose to cc1psx;
* an `awk` scan for `^[a-zA-Z_].*\(` named a function **400 lines too early**,
  so three measurements in a row were about a body that did not contain the
  edit, and only `make containers` caught it;
* an `INCLUDE_ASM` pattern expecting three comma-separated fields where the
  macro takes **two** reported "3 written" when the truth was 8, 7, 6 and 5 --
  which nearly discarded twelve landed bodies;
* `grep -c INCLUDE_ASM` counted a **comment** and reported one stub for a unit
  with none.

**And a caveat worth knowing:** the tools were repo-scoped at one point and
refused paths outside the repository. If your working copy is refused, that is
why -- say so rather than working around it with a script.


## A CONSTANT-VERSUS-SYMBOL CLAIM IS PER ADDRESS, NEVER PER UNIT

**My own brief got this wrong and an agent lost time on it, so it is stated
here before anything else about addresses.** I told an agent that "every
`0x8001xxxx` address in this unit is a plain CONSTANT". That is true only of
addresses **outside the loaded image**. A unit's OWN data can sit in the same
numeric range and be a real relocated symbol -- `0x8001F5B4..0x8001F6E8` in
`ovl_dng_lo16` is reached as symbols by 53 landed bodies.

**The discriminator is the instruction pair, and it is per address:**

    lui 0x8002  +  addiu/%lo displacement   -> a RELOCATION, a real symbol
    lui         +  ori                      -> gas' `li' macro, a CONST_INT

So **run `ovlverify --raw` and read the pairs** rather than applying any
blanket rule. If a brief of mine states one, treat it as a hint about the
out-of-image addresses only, and say so in your hand-back if it misleads you --
one agent did, which is why this section exists.


## FUNCS.txt NEVER FLAGS A FRAGMENT -- THE PROVISIONER REMOVES THEM

A dispatcher error, corrected by an agent that checked instead of believing
the brief. `provision_batch.py` prints a `fragments:N` count, but it **EXCLUDES
those symbols from `FUNCS.txt` rather than marking them**, so a brief that says
"one fragment is flagged in your FUNCS.txt" is wrong twice: there is no flag
column, and the fragment is not in the file at all.

`FUNCS.txt` is plain `name<TAB>words` rows and **every row is a function you
are expected to land.** If a brief claims otherwise, check the file and say so
-- one agent did, and confirmed all twelve of its listings contained exactly
one `jr $ra`.

**Dispatcher note:** report the `fragments:N` number as *"N symbols were
dropped before your list was written"*, never as something in the file.


## THE SESSION SCRATCHPAD IS SHARED BY EVERY SIBLING AGENT -- NAME YOUR FILES

**Read this before you write a single temporary file.** The Claude Code session
scratchpad is **one directory shared by all sibling agents**, not private per
agent. It already holds hundreds of files with colliding generic names:
`body.c`, `best.c`, `base.c`, `splice.py`, `m.sh`, `ad.txt`, `all.txt`,
`check.log`.

**This has already caused a real mix-up.** One agent's `body.c` was overwritten
by a sibling's mid-run, and the next splice put the WRONG FUNCTION'S BODY into
its unit copy. It was caught on the next compile and nothing was lost -- but a
clobber like that defeats an all-functions verify **only if you measure before
you notice.**

So:

* **Keep every working file inside your own `agent-hNNN/` directory**, which is
  yours alone, and never in the shared scratchpad.
* If you must use the scratchpad, **qualify every name with your agent id** --
  `h186-body.c`, not `body.c`.
* **Re-verify your unit copy** (`ovlverify` over every defined body) after any
  gap in your run, not just at the end. A body you did not touch changing is
  the signature.

This is the `/tmp is not durable` lesson with **clobbering** rather than
cleanup as the mechanism, and clobbering is worse: cleanup leaves an absence
you notice immediately, while a clobber leaves a plausible file.

**And one clobber was the dispatcher's fault, not a sibling's.** The batch
provisioner used to `cp` the pristine `csrc/` unit over an agent's working copy
unconditionally, and on 2026-11-25 it did exactly that to a live agent's
`ovl_dng_lo16.c`. It now refuses to overwrite an existing unit file. **If your
unit copy is ever replaced by a pristine one, say so in your next status
message** -- it is a dispatcher bug and worth knowing about immediately.


## WHAT BATCH 34 ADDED -- A PARKS BATCH, AND IT CHANGED HOW TO PRICE

Read this first. Batch 34 took 11 parked functions -- the hardest bodies in the
project, several with three agents' work already in them -- landed two and
advanced the rest. Most of what it found is about METHOD, so it applies to you
even though you have fresh stubs.

**A FIX CAN BE WORSE ALONE AND NECESSARY IN COMBINATION. This is the big one.**
Every pricing sweep in this project is one-at-a-time: measure a change, keep it
if it improves, discard it if not. That procedure provably cannot reach one
park's body:

    the body as parked                                   875 words / 85 rows
    + `volatile' on the condition read, ALONE             879 / 86   <- WORSE
    + a value ternary instead of stores in both arms      877 / 76
    + the three pre-call statements reordered             875 / 63
    + two locals with one store after the if/else         873 / 50

The first change is worse than the body it is added to, so a linear sweep
discards it at step one and never tries the other three on top. **So: when you
have a NAMED MECHANISM and a spelling that should work by that mechanism
measures worse, hold it and vary something else on top of it.** A spelling with
no mechanism behind it can still die on one measurement. Several parks sat for
multiple agents at a figure no single change improved -- that is this problem's
signature, not a floor.

**REFERENCE COUNTS ARE LOOP-WEIGHTED.** `flow.c` adds `loop_depth` per
reference, so a body inside a `for` loop has roughly DOUBLE the counts you get
by reading the source. `qty_compare` wants `floor_log2(R) * R >= 48` and a TIE
suffices (`qty_compare_1` breaks on quantity number, earlier-born wins). Any
"add a reference" remedy priced from a source count is in the wrong units
inside a loop. And a split store adds ONE weighted reference, not two -- cse
collapses `c = c + 1; D = c;` before flow ever counts it.

**CHECK PASS ORDER BEFORE BLAMING A REGISTER FOR A SCHEDULE, OR VICE VERSA.**
`local-alloc` runs AFTER `sched1` (`toplev.c:3548` against `:3585`), so no
register assignment can cause a sched1 decision. Prologue saves do not exist at
sched1 at all -- they first appear in `.sched2`. Two parks blamed allocation for
a schedule and one blamed sched1 for something sched2 does. The pass order is
in `toplev.c` and reading it costs nothing.

**A cse BASIC BLOCK ENDS ONLY AT A `CODE_LABEL` (`cse.c:8086`).** So an `if`
arm that is a fall-through is the SAME cse block as its gate, and a load in the
arm is a redundant second read that cse DELETES -- it is not a hoist. "The load
is in the dominator block" therefore has two causes wanting opposite fixes.
Check whether the arm is a fall-through first.

**FOUR WAYS THE LISTING LIES, all measured, all expensive:**

1. **An argument register set before a `jal` is not evidence of an argument.**
   The discriminator: dead after the call and computed only for it means
   argument; live before and used elsewhere means scheduling artefact.
2. **A store's position inside a dress block is not readable**, because sched
   sinks an independent store into the call's delay slot. Source order measured
   0; the order the listing appears to show measured 14.
3. **A folded offset is not a spelling.** A base register holding `symbol + K`
   with no `addiu` computing it means `combine` folded a pointer local away --
   transcribing the folded form cost 243 rows on one body and rotated two
   registers across all 873 words.
4. **An emitted count-DOWN loop means the source counts UP** (`check_dbra_loop`
   reversed it), and whether the loop was reduced decides whether
   `(p + i)[K]` and `p[i + K]` differ at all.

**THE REGISTER DIAGNOSTIC LADDER, now complete.** For a single-register
residue, in this order:

* **wrong CLASS** (`$a1` where the target has `$s1`): the pseudo must span a
  call it does not yet span. `allocno_calls_crossed` is 0 if the assignment is
  after the call; move it BEFORE. One body's whole 6-row residue.
* **wrong register WITHIN a class** (`$a0` where the target has `$a2`): the
  range is right and something else should hold the low registers. Three
  answers, cheapest first -- **one local too many** (merge two of yours: two
  pointers with different expressions can be one pseudo in the target, and
  that landed an 834-word park), then add a pseudo, then move one.
* **`$v0` specifically**: ask which CALL'S RETURN VALUE is live across the
  block. `find_free_reg` runs in two rounds and a suggested register -- which
  includes the return register -- is reserved across its whole range in round 1
  before anything else is looked at.
* **a tied chain**: you can read a `combine_regs` break off the listing. If an
  instruction in the chain writes a DIFFERENT register from the one it reads,
  the chain is broken there in the original, so count the target's quantities
  before writing C.

**THE SAVE COUNT PREDICTS, AND NOT ONLY WHAT YOU ARE MISSING.** On a spill-free
body, saves = values living across a call, and the parameters are the ones you
know about; the remainder are locals you have not found. It also says which of
a donor's facts CANNOT apply to you: one agent called an inversion from the
prologue alone -- five saves, three live parameters, so the third value must be
a `$v0` temp -- and the donor's rejected spelling was the one that landed.

**TWO SMALL THINGS WORTH MONEY.** A `volatile` store blocks COMBINE as well as
cse, because combine calls `init_recog_no_volatile` (`combine.c:502`) so
`volatile_ok == 0` and it cannot re-recognise a volatile MEM -- that is what
kept one `addiu` alive and landed a 279-word park. And write a zero store
BEFORE the value that overwrites it, or cse deletes it and the body is one word
short.

## OLDER MATERIAL FOLLOWS -- STILL TRUE, LESS URGENT

## OLDER MATERIAL FOLLOWS -- STILL TRUE, LESS URGENT

## YOUR FUNCTIONS AND PATHS COME FROM THE DISPATCH MESSAGE, NOT FROM THIS FILE

This brief is the GENERAL material and is inherited from batch to batch. It
deliberately names no unit, no function list and no agent directory: a
per-agent section lived here for three batches and ten agents, naming
`<YOUR-UNIT>.c' and `agent-h136' -- batch 32's assignment -- long after those
were anyone's. Every agent worked its real unit from the dispatch message and
ignored this, and the tenth was the first to say so.

So: your unit, your function list and your scratch directory are in the
message that dispatched you. Where an example below spells a path, read
`<YOUR-DIR>' and `<YOUR-UNIT>.c'.

## Why these, and why they are worth more than their word count

DUNGEON.BIN was carved into 116 code objects on 2026-11-22. They are not 116
separate programs: the banked room scripts statically link a shared library,
so the same function appears, relinked, in up to 216 objects. 1199 of the
block's 2575 stubs are exact copies of a sibling.

Each function above is the REPRESENTATIVE of a family. When it lands I
replicate your body into the other members mechanically, so the list is worth
838 words -- no replication at all this batch: every one of these is a
one-member family, so a body you write is a body that lands once.

**THE BIG FAMILIES ARE GONE.** Early in this sweep a 31-word body could have
17 copies and deserved the same fight as a 500-word one. 7.3% of what is left
is recoverable by replication, down from 13.4%, and most families now have
TWO members. So the honest guidance is now the plain one: a function is worth
roughly its own word count, your list is longer than earlier batches' because
of that, and the way to finish it is the twin search rather than a long fight
on any one body.

    python3 tools/decomp/dngfam.py <func>        # that family's member list

**DO NOT TOUCH THE OTHER MEMBERS.** Do not open their units, do not try to
land them, do not "helpfully" do the copies. The replication is mine and it is
mechanical. Your unit's copy of your listed functions, nothing else.

## Setup, exactly

    cd /home/case/projects/azure
    mkdir -p <YOUR-DIR>/sp
    cp csrc/<YOUR-UNIT>.c <YOUR-DIR>/<YOUR-UNIT>.c
    export AZURE_SCRATCH=<YOUR-DIR>/sp

**KEEP THE BASENAME `<YOUR-UNIT>.c`.** `ovlverify.py` picks which unit (and so
which compiler flags, which container ELF and which listing directory) from
the source file's BASENAME. A copy called `work.c` silently resolves against
the wrong container and prints plausible nonsense instead of failing.

**SET `AZURE_SCRATCH`.** `asmlib` derives its intermediate `.s`/`.o` path from
the basename, and five agents sharing one path corrupt each other's compiles.
The failures are loud and misleading -- `unrecognized opcode 'macro'`, `junk
at end of line`, `symbol already defined` on unrelated functions -- and look
like a defect in your source. They are not.

**`tools/cdkgcc -da` WRITES ITS DUMPS TO THE CURRENT DIRECTORY**, so a probe
run from the repo root drops `probe.c.lreg`, `probe.c.greg` and the rest into
the repo. `cd` into `<YOUR-DIR>` first, or pass absolute `-I` paths and run it
from there, and check `git status` afterwards. An agent that leaves those
files behind puts them in the dispatcher's next commit.

**KEEP EVERY WORKING FILE INSIDE `<YOUR-DIR>/`.** That directory is yours.

## NEVER RUN `make`, `make splat`, `make containers`, OR ANY BUILD

`make` re-runs splat, which DELETES `asm/nonmatchings/` and rewrites it. That
destroys the ground truth every other agent is measuring against, including
yours. The verifier compiles your file itself; you do not need a build. The
integration build is mine.

Also do not run the permuter (`permrun.py`/`permimport.py`). Its jobs are a
machine-wide budget and with five agents live it is mine to spend. If you
cannot land a function, park it and report the residue; I run searches.

## The verifier is the only thing that counts

    python3 tools/decomp/ovlverify.py <YOUR-DIR>/<YOUR-UNIT>.c <func>

It prints `target N words, candidate M words, K differing`. **Landed means
`0 differing` AND candidate words == target words.** Anything else -- a build
that links, an agreeing word count alone, a body that "looks right" -- is not
a landing.

It takes SEVERAL names and compiles the unit once, so re-verifying the whole
list costs barely more than one function:

    python3 tools/decomp/ovlverify.py <YOUR-DIR>/<YOUR-UNIT>.c <your functions, all of them, in one run>

**RUN THAT AFTER EVERY LANDING, NOT ONLY AT THE END.** Adding a definition
can change what an earlier one compiles against -- a reconciled prototype, a
new declaration that shadows, a typedef that changes a field width -- and an
earlier function silently coming unstuck is the one failure this shape of work
adds. It has happened. The final numbers in your report must come from this
command, run last, with every body in place.

On anything over ~200 words size the residue with

    python3 tools/decomp/aligndiff.py <YOUR-DIR>/<YOUR-UNIT>.c <func>

which aligns the two streams before counting; `ovlverify` compares word N
against word N, so one transposed pair misses twice and inflates the number.
`ovlverify --raw` shows unmasked bytes, which is how you prove a `%hi`/`%lo`
the verifier masked is really a relocation and not a constant address.

## Getting a first draft

**DO NOT ASSUME YOUR UNIT IS FRESH -- IT ALMOST CERTAINLY IS NOT.** This
paragraph used to say every function was still `INCLUDE_ASM` and the unit had
no declaration block. That was true for the first few batches and is now
wrong for most units: by batch 15 a typical carved unit has fifteen to thirty
LANDED functions and a declaration block of a hundred lines or more
(`ovl_dng_b26` had 20 and 126 when an agent was handed it under the old
wording). Check, because three things follow from it:

* **Read the unit's own landed bodies before you go looking elsewhere.** The
  best twin is often two listings away in your own file, and it has already
  absorbed this bank's relink.
* **Most of your declarations are already there.** Add only what is missing,
  append to the ONE existing block, and never start a second one.
* **A park may exist for a function in your unit**, and it may carry a priced
  fact table for the whole unit rather than just that function. `ls
  docs/dng-parks/` for your tag before you start.

Run `grep -c INCLUDE_ASM csrc/<YOUR-UNIT>.c` against the number of `glabel`s in
`asm/nonmatchings/<YOUR-UNIT>/` if you want the split.

    tools/m2c asm/nonmatchings/<YOUR-UNIT>/<func>.s

That prints a first draft against `ctx.c`'s type context. Read
`docs/m2c-pitfalls.md` before you trust it. Get it compiling, then measure,
then work the residue.

## RUN `dngtwin.py` FIRST. IT SEES THE 2,500 LANDED CARVED BODIES `xsibs` CANNOT.

    python3 tools/decomp/dngtwin.py --unit <YOUR-UNIT>

For each remaining stub it ranks the CARVED bodies already landed in
`csrc/ovl_dng_*.c` by opcode-sequence similarity, and prints the file to open.
A hit here is better than a resident twin by construction: a landed carved
sibling has already absorbed the relink -- the bank-local facing table, the
`func_80047784` byte-table call, whatever device its allocation needed -- so
the only thing left is your delta.

**WHY THIS IS A SEPARATE TOOL AND NOT A FLAG ON `xsibs`.** `xsibs` builds its
landed side out of the REFERENCE IMAGES: `nm -n` for symbols, `objdump -h`
for the section map, image bytes for instructions. 115 of the 116 carved
objects are OVERLAYS SHARING ONE RAM WINDOW at 0x8017xxxx or 0x8002xxxx -- all
at the same addresses, all in the one dungeon.bin ELF -- so `nm -n`
interleaves 115 objects' symbols at colliding addresses and the "a function
ends at the next symbol" rule measures from one object's symbol to a
different object's. Measured: `xsibs`'s landed pool is 3,366 functions, of
which 189 are carved, and all 189 are `func_lo16_*` -- the one carved segment
outside the shared slot. Every landed body in the 115 banked objects, around
2,500 functions, is absent from every `xsibs` run. `dngtwin` compares LISTING
AGAINST LISTING instead, which works because splat regenerates a landed
function's listing too.

**HOW TO READ ITS RATIO.** 1.000 = same control flow, different registers or
constants, so expect to change operands and nothing else. 0.90-0.99 = a few
instructions' delta. 0.75-0.90 = same kind of routine, real differences,
worth reading. Below 0.75 = a lead, read the call sequence instead.

A row marked `[CLONE -- run dngclone]` is an exact `dngfam` family member: do
not read it, land yours and let `dngclone.py` write the other. A row marked
`[STILL A STUB]` is assembly, not an answer -- but it tells you two stubs are
one job, which is worth knowing before you measure the same residue twice.

**AND A RATIO IS A REASON TO OPEN THE C, NEVER TO COPY IT.** The twin's
`do { } while (0)`, its spread stores and its split statements are fixes for
ITS allocation. Strip them, compile, measure, and add back only what your own
listing asks for. See STRIP THE TWIN'S DEVICES BEFORE YOU COPY THEM below.

## THEN `xsibs.py`, FOR THE RESIDENT TWIN.

    python3 tools/decomp/xsibs.py <YOUR-UNIT> --min-words 18 --ratio 0.70 --top 200

It compares each of the unit's stubs against every function LANDED ANYWHERE
in the project -- read out of the reference images, so it does not need a
listing to have survived -- on the opcode stream with registers, immediates
and jump targets masked. That is exactly what survives a relink.

It compares against every function landed anywhere in the project, which for
the RESIDENT overlays -- dungeon.bin's own 0x800894A0 code, town.bin,
main.bin, slus_006.14 -- is exactly right and is what it is for. Two agents
independently reported it as the most useful first command and for eleven
batches it was. It is now the SECOND command, because the pool it cannot
reach is the one that has already absorbed the relink. Run both: a function
with no carved sibling often has a resident one, and the reverse.

**`--min-words 18`, NOT THE 40 THE TOOL'S OWN EXAMPLES USE.** A 22-word
forwarder has a twin too: `func_b28_80174460` and `func_b28_801744B8` both
came back "none found" in a brief rendered at 40 and both sit at ratio 0.900
to a landed 24-word body. The floor costs one flag and hid two twins.

**`ovlverify` CANNOT SEE WHICH DATA SYMBOL A RELOCATED LOAD NAMES, SO THE
LISTING IS THE ONLY AUTHORITY FOR A RELINKED TABLE'S IDENTITY.** It masks the
relocated fields, which is correct and which means a dress site loading the
WRONG bank-local table measures 0 differing. `func_b31_801716F4` has three
dress sites and its 0x80000 arm must load `D_8017555C` where the other two
load `D_80175554`; writing one table at all three also measures 365 words and
0 aligned differences, and `--raw` does not help -- the relocation COUNT is
unchanged by a wrong symbol of the same kind. So read every `%hi`/`%lo` pair
out of your listing and check the symbol, even when the body is already at 0.
`tools/decomp/relocheck.py` audits this across the block.

**WHEN `xsibs` NAMES A RESIDENT TWIN, INVERT THE INDEX BEFORE USING IT.**
(`dngtwin` now does this for you in the common case. This grep still finds
the sibling whose own ratio to you is below `dngtwin`'s floor but which names
your resident twin in its header, so it is the fallback, not a duplicate.)
Three agents have now found that the resident function `xsibs` names is the
WRONG body to start from, because a carved sibling has already absorbed the
relink -- and one grep finds it, because every carved landing names its own
twin in its header:

    grep -rn "func_800D07C8" csrc/ovl_dng_*.c

`func_s17_80024188` (351 words) is the case to remember: `xsibs` offered
`func_800D07C8` at 0.750 and the right body was the landed
`func_b14_80173B4C`, which that grep finds in one shot. It landed on the
first compile. Do this before you read the resident original, every time.

**A RATIO IS A LEAD, NOT A PROMISE, AND A MISS IS NOT A VERDICT.** 0.9x means
a starting body with deltas to read off your own listing, not an answer. And
when `xsibs` finds NOTHING, that is useful too -- it says go straight to plain
decompilation rather than hunting. But before you accept a miss:

**A FUNCTION WITH NO WHOLE-BODY TWIN CAN STILL HAVE A LANDED TWIN FOR ITS
FIRST EIGHTY WORDS.** `func_b02_80173560` came back empty at ratio 0.40 and
its state 0 is the landed `func_b14_8017293C`'s head verbatim;
`func_b02_80174AD4`'s entire prologue is `func_800B8FC8`'s. Both landed. The
way to find that is to grep the CALLEE SET -- pick four or five
`func_800xxxxx` your listing calls and find a landed body that calls the same
ones in the same order:

    grep -ln "func_800A9A0C" csrc/ovl_dungeon*.c csrc/ovl_dng_*.c

Shape similarity cannot see a partial match. The callee set can.

**THE RECIPE, which has now found a twin six times that `xsibs` scored below
threshold.** Take four or five of the most distinctive symbols out of your
listing, `grep -ln` each ONE AT A TIME over the landed side, and INTERSECT the
file lists:

    for s in func_800A94A0 func_800DA840 func_800A05A4 D_8006DE24; do
        grep -ln $s csrc/ovl_dungeon*.c csrc/ovl_dng_*.c
    done | sort | uniq -c | sort -rn | head

The file in every list has the function; then grep those same symbols inside
it to find which one. `func_b18_80172AB4` is 422 words and its twin is 316 --
a size ratio shape matching cannot see past -- and it landed on the FIRST
COMPILE because the twin's comment already carried four measured spelling
facts. That one grep was worth 499 of that agent's 844 words.

**PICK THE SYMBOLS THE *TAIL* USES, not just the callees.** For
`func_b35_801728C4` the `func_800xxxxx` callees found nothing and the two
distinctive globals `D_800E3DE8` and `D_8008346C` found it at once, because
those are what the shared teardown touches.

## IF THE RESIDUE IS A WHOLE BLOCK IN THE WRONG PLACE, CHECK WHICH ARM IS THE `then`

Two agents reached this independently in one batch. When a body is the right
word count (or one off) and the residue is a block emitted in the wrong
position, the likeliest cause is that a two-arm gate is written with the arms
the wrong way round -- `if (x == 0) { A } else { B }` where the target has
`if (x != 0) { B } else { A }`. It costs 7 and 17 aligned differences in the
two measured cases, with every row being the moved block and the inverted
branch. Check that before anything else; it is one character.

## FIND THE RESIDENT TWIN FIRST. IT IS USUALLY THE WHOLE ANSWER.

This is the single most useful fact about this block. The banked room scripts
are relinked copies of code that ALSO EXISTS in the resident dungeon overlay
at 0x800894A0 -- and that overlay is **100% decompiled**. So for most of these
functions the body is already written, in C, with a comment explaining what
made it land.

Nine of the first ten functions landed on or near the first compile this way.
Known pairs so far:

    func_b01_80171C34 <- func_800D92C0      func_b03_80173358 <- func_800DA014
    func_b03_80170A28 <- func_800D8728      func_b07_80171570 <- func_800D904C
    func_b03_801736F4 <- func_800DA398      func_b03_80171DA0 <- func_800CA788
    func_b01_80172800 <- func_800D9C18      func_b02_80174574 <- func_800DA660

**A LANDED `ovl_dng_*` SIBLING CAN BE A BETTER TWIN THAN THE RESIDENT
ORIGINAL**, and 857 functions into the sweep it usually is. It has already
absorbed the relink -- the bank-local facing table, the `func_80047784`
byte-table call, an inverted gate, whatever device the allocation needed -- so
the only thing left to find is the delta. `func_b03_8017139C` landed on the
first compile as `func_b07_80171570` plus five words; `func_b03_80171F64`
landed with NO new declarations because its 113-word sibling had already
brought every symbol into the unit. **And the best twin can be inside your own
unit, two listings away:** `func_b01_80174070` -- 994 words of yield -- landed
in 74 seconds as an already-landed neighbour in the same file differing in one
branch opcode. Check `csrc/ovl_dng_*.c`, YOUR OWN COPY FIRST, before you go to
the resident overlay.

How to find yours: read your listing's call sequence, then grep the resident
units -- and the landed carved ones -- for a function that calls the same
things in the same order.

    grep -ln "func_800A9A0C" csrc/ovl_dungeon*.c csrc/ovl_dng_*.c

`docs/dungeon-map.md` and the landed bodies in `csrc/ovl_dng_*.c` (each with a
comment naming its own twin) are the other way in.

The differences are small and local: an extra arm, a bank-local table in place
of a resident one, an inverted gate, one constant. **But RE-MEASURE the twin's
named spellings rather than assuming them.** On `func_b03_80170A28` all three
of the twin's facts held -- 3, 15 and 132 aligned differences when spelled
otherwise -- and on `func_b07_80171570` the gate turned out to be the twin's
*inverted*, worth 2 words and 46 differences to get wrong. The allocation can
also run the OTHER way: `func_b09_80171320` is a landed family's function with
its common tail written ONCE instead of duplicated, and spelled the siblings'
way it is 74 differences out -- three functions have needed references ADDED,
that one needed them REMOVED. Put your own numbers in your comment, not the
twin's.

## IF YOUR LISTING OPENS WITH `.section .rodata`, IT OWNS A JUMP TABLE

Look at the top of each listing. If it starts with a `.section .rodata` block
defining a `jtbl_<tag>_xxxxxxxx` before the `.section .text`, that function's
`switch` compiles to a JUMP TABLE and the table is part of your unit's rodata.

**That is now supported and it was not until 2026-11-23.** 105 of the 116
carved objects have a `.rodata` subsegment of their own, so cc1psx emits your
table at the ROM's address. Before that it appended a second copy past the
code and the container came out longer -- while `ovlverify` reported
`0 differing`, because it measures a function's own words and never its
rodata. `docs/dng-sweep.md` has the whole story.

What it means for you:

* **Write the `switch` as a `switch`.** The table's entries tell you which
  cases share an arm; `tools/decomp/dngfam.py` is not involved, just read the
  `.word`s at the top of your own listing. Cases that fall to `default` need
  no label of their own.
* **SAY IN YOUR REPORT THAT THE FUNCTION OWNS A TABLE**, and give the entry
  list. I check the container, you cannot, and a table is the one thing a
  clean `ovlverify` can still be wrong about.
* The one object still unsupported is `ovl_dng_lo16`. If that is your unit and
  a function of yours owns a table, park it and say so rather than fighting it.

A unit may also carry `const u8 D_<tag>_xxxxxxxx[] = { ... }` arrays near its
functions. Those are its rodata between the tables, they are deliberate, and
their POSITION IN THE FILE IS LOAD-BEARING -- cc1psx emits rodata in function
order. Do not move one, do not reorder them, and do not make one `static`.

## WHEN THE RESIDUE IS A REGISTER SWAP, READ `-da` RATHER THAN SWEEPING

If your body is the right length and the differences are all one pair of
locals holding each other's callee-saved register, that is
`allocno_compare`'s ranking and there is arithmetic for it. Do NOT sweep
spellings:

    tools/cdkgcc -da -S -O2 -G0 -o probe.s probe.c      # in YOUR directory

`.lreg` and `.greg` give every allocno's reference count and live length, and
`global.c` ranks on `floor_log2(n_refs) * n_refs / live_length`. One
two-second compile tells you which side needs lifting and by how much.

`func_b28_8017357C` is the worked example: `p` at 3*9/198 = 0.1364 against
`b`'s 2*5/74 = 0.1351, a margin of one part in a hundred. That ruled out the
obvious device -- a `do { } while (0)` around the state bump, which is where
an earlier lever puts it -- BEFORE compiling it, because that statement
references `p` and `p` is the wrong side. Wrapping a statement that
references `b` moved it.

**And the fix does not transfer between bodies of the same twin.**
`func_b44_80174110` and `func_b44_80174438` are the same pair of banked tails;
one measures 0 with the shared tail written once or twice, the other needs it
twice. A twin's documented FACTS carry over; its allocation fix is priced per
body. `docs/matching-levers.md` has the entry and its amendment -- read both,
because the devices also fail outright when the competition is local-alloc's
rather than global's.

## Where the callees are already written

The resident dungeon overlay is 100% decompiled. Anything your function calls
at `0x800xxxxx` already has a real definition with a real signature:

    grep -n "func_800ABDBC" csrc/ovl_dungeon*.c

Read the definition and copy its signature rather than inventing one. Note
that several are K&R-style with an arity the call sites disagree with; the
project's fix for that is a unit-local alias declaration, which leaves the
landed definition alone:

    s32 func_80096100_4(u8 *, UnkVec *, u8 *, u8 *) __asm__("func_80096100");

A `func_s20_xxxxxxxx` callee is a sibling inside your own object and is
still an `INCLUDE_ASM` stub -- just declare it.

**ONE SIGNATURE PER UNIT, AND IT MUST FIT EVERY USE IN IT.** Your functions
share a file, and two of them may use the same callee. Two agents once landed
the same callee with different arities and both verified -- taking a
function's address does not depend on its signature -- and the conflict only
appeared when the bodies met. Before you add a declaration, grep your copy for
that symbol; if a declaration is already there and disagrees with what you
want, reconcile it and RE-VERIFY EVERY FUNCTION IN THE UNIT, then say so in
your report under item 4.

## Read these

* `docs/landing-a-function.md` -- THE CONTRACT. It is written for one function;
  apply it once per function on your list. Two of its steps are overridden
  here and only two: you do not run the permuter (above), and the landing gate
  is `ovlverify` + your report rather than `make check` (the build is mine).
* `docs/m2c-pitfalls.md` -- before you trust the first draft.
* `docs/matching-levers.md` -- ~1100 measured compiler behaviours. SEARCH IT
  BEFORE SWEEPING SPELLINGS BY HAND:
  `python3 tools/decomp/leverindex.py --grep <opcode or idiom>`.
  Most of what you will hit is already in there. Do not read it end to end.
* `docs/overlays.md` -- these units are `-G0`, and that is not cosmetic.

## House rules

* C style: Allman braces, 4 spaces, `(u8 *)p` with no space after the cast.
  Before reporting, run
  `clang-format -style=file:/home/case/projects/azure/.clang-format -i <YOUR-DIR>/<YOUR-UNIT>.c`
  and confirm a re-run reports no replacements.
* New extern/function declarations go in ONE block at the TOP of your copy,
  after `#include "common.h"` and before the first `INCLUDE_ASM`. Add to that
  one block as you go; do not start a second.
* Keep types honest. If a struct field's offset and width are all you know, a
  cast through `u8 *` is fine. Do not invent a struct you cannot witness.
* **STRIP THE TWIN'S DEVICES BEFORE YOU COPY THEM.

A twin's comment may name a `do { } while (0)`, a duplicated arm, an `if (1)`
or a reference bump. Those are ALLOCATION FIXES for THAT body's register
pressure, not facts about the shape. A relinked copy usually has different
pressure, so the first thing to compile is the twin's body with every device
REMOVED -- then add one back only if a measurement asks for it.

Two agents hit this in one batch. One copied a twin's shared `goto` exit
label and paid 32 aligned differences; four plain `return -1;` statements
measured 0, because reorg rebuilds the shared block by itself. Another found
its twin was line-for-line correct with one callee swapped and landed at 0
differing only after deleting BOTH of the twin's devices.

The same split applies to a twin's STATEMENTS versus its LABELS and LOCALS:
copy the statements, re-derive the labels, the shared locals and the devices.

RE-MEASURE A PLAUSIBLE FACT BEFORE YOU WRITE IT DOWN.

When a body lands on the first compile, every spelling in it is UNMEASURED --
you chose them, they worked, and you do not know which ones mattered. Do not
write them up as levers. `ovlverify` on one name takes a couple of seconds,
so flip the two or three that look load-bearing and SAY WHICH ONES WERE FREE:

    "Measured and free at 91 words / 0 differing, so it is NOT the reason
     this lands: the two tail stores in the other order."

An agent that did this on five first-compile landings found TWO of its five
plausible facts were free. A comment that claims a lever the next agent then
builds on is worse than no comment at all, because the next agent will spend
its budget preserving something that never mattered -- and the comments in
this tree are the only record of what has actually been measured.

IF A CALLEE HAS A REAL NAME, USE IT.** Before declaring
  `s32 func_80069EF8();`, check:

      grep -iE "= 0x80069ef8;" splat/common_syms.txt

  That one is `rand`, and 129 sites across 21 units had been writing the
  address instead. `RotTransPers`, `RotTransSV`, `RotTransPers4`, `AddPrim`,
  `SetPolyG4`, `PushMatrix`, `PopMatrix`, `SetRotMatrix`, `SetTransMatrix`
  and `RotMatrix` were all being spelled as addresses too. The real name
  usually needs NO declaration -- it comes from `common.h` -- and it tells the
  next reader what the code does, which an address never will.
* **`common_syms.txt` AND NOTHING ELSE.** Do NOT take a name from
  `splat/<container>_symbol_addrs.txt`. Every name in `common_syms.txt` is a
  resident library or BIOS routine at 0x8003c9f8..0x8006a3a8, below the
  overlay slot, so it is valid whichever overlay is mapped. The
  per-container files name code INSIDE the slot at 0x80088760..0x800f7700,
  and an address in there is different code depending on which overlay is
  loaded: `plt_dm_kicked = 0x80098A98` is a true name of TOWN.BIN's code and
  a LIE in a dungeon unit, because when DUNGEON.BIN is mapped that address is
  dungeon code.
  It would LINK, the symbol exists, and the bytes would be identical -- so
  neither the build nor `ovlverify` can catch it. The name would be the only
  false thing in the file. `tools/decomp/symsafe.py` audits for exactly this.
* A NEW TYPEDEF NAME MUST NOT COLLIDE WITH ONE ANOTHER AGENT IS INVENTING
  RIGHT NOW. You cannot see the other four. Check both places before naming
  one, and prefer a name that says which function it belongs to:

  **EXCEPT `rand`, AND `declcheck` WILL TELL YOU: `rand` IS NOT IN
  `common.h`.** `psyq/include/rand.h` declares it but nothing includes that
  header, so 71 units call it implicitly and `declcheck` flags it on COMMITTED
  files. Add `s32 rand(void);` to your unit's block -- it has measured FREE
  every time (all 11 bodies of one unit still 0 differing afterwards) and 23
  other units already do it. **Do NOT try to fix it globally:** adding
  `#include "rand.h"` to `common.h` BREAKS THE BUILD, because
  `ovl_dungeon_800C2824.c` calls `rand(ii)` with an argument and `ovl_town.c`'s
  comments record `rand(0x28)`/`rand(0x40)` as measured facts naming a call's
  `$a0`. See `docs/undeclared-callees.md`.

      grep -ho 'UnkDng[A-Za-z0-9_]*' csrc/ovl_dng_*.c docs/dng-parks/*.c 2>/dev/null | sort -u

* Put a short comment above EACH landed function giving its word count,
  `0 differing`, and -- when you had to fight for it -- the ONE spelling fact
  that made it land and what the wrong spellings cost. Measured numbers only;
  never write a mechanism you did not verify. If function three landed because
  function one was already there, say that in function three's comment.
* **A MISSING register-to-register COPY IS USUALLY A PARAMETER THAT DIES AT
  ITS COPY, AND THE FIX IS FREE.** Read this before reaching for any device.
  If `h = p` is the last use of the parameter `p`, regmove folds `p = $a0`
  into it, your alias chain is one pseudo short and every callee-saved name
  after it sits one position early. Keeping `p` live past the copy -- most
  naturally by REUSING THE PARAMETER AS THE LOOP CURSOR instead of
  introducing a separate one -- makes the copy survive.
  `func_b09_801745BC` went from a 19-difference PARK to 145/145 at 0
  differing on that one change, with every other spelling in its park table
  already correct; `func_b09_80174800` in the same unit is 73 words / 15
  differences with a separate cursor and 74 words / 0 written this way.
  It costs no words, invents no statement, and is the more natural C.
  Tell the two failures apart off the residue before choosing a fix:
  an extra or missing `move` with every later register displaced is a WRONG
  PSEUDO COUNT, and a reference bump is the wrong tool for it; the same
  pseudos with two callee-saved names exchanged is a WRONG PRIORITY, which is
  what a bump is for.
* A decomp device is allowed and `docs/landing-a-function.md` says how to
  annotate one. Prefer a plain-C device (a `do { x; } while (0)` reference
  bump, a duplicated arm) to an `__asm__` one, and record the plain scores you
  rejected.
  **`if (1)` IS NOT A SUBSTITUTE FOR `do { } while (0)` IN A REFERENCE BUMP.**
  `flow.c:2122` counts a reference once per ENCLOSING LOOP, and `if (1)` is
  not a loop: on `func_b21_80172538` the `do`-wrapper lands at 0 differing and
  `if (1)` around the same statement is 7, the same as no device at all. Use
  `if (1)` for a pure scheduling barrier, not for an allocation priority.
  **AND DO NOT PRESENT A DEVICE'S SCOPE AS A MEASURED FACT** unless you
  measured the alternatives. On that same function three different scopes all
  measure 0 differing, so the target does not constrain the choice; say that
  in the comment instead of implying the kept scope is the macro's.
* **MOST PLAIN-C "DEVICES" ARE PROBABLY MACRO RESIDUE, AND THAT IS A SEARCH
  HINT.** We compile what the preprocessor emitted; the macro layer is gone
  and unrecoverable in principle. `do { ... } while (0)` is the canonical
  macro body, `if (1)` and a duplicated arm under an always-true guard are
  what a configuration macro collapses to, and a value referenced more times
  than the C needs is what a macro evaluating its argument repeatedly gives
  for free.
  So when a residue is `allocno_compare` wanting more references on one
  value, do not sweep statements looking for one to wrap. Ask which COHERENT
  UNIT OF WORK around that value would have been a macro -- a whole gate, a
  whole publish, a whole teardown -- and wrap that. The macro boundary tells
  you both where the braces go and how many references you get. That is why
  `func_b28_8017357C` needed its wrapper around the `func_800AA888` call and
  not around the state bump three lines up.
  Say which you think it is in the comment: "device" if you could not find the
  spelling, "macro residue" if you think you did and the macro is just not
  recoverable.

## If one will not land

A precise failure is worth more than a stalled agent, and on a multi-copy
representative a measured residue is a real asset. **Give one function a
bounded fight, then park it and move to the next on your list** -- the next
one is independent and may be a twin away. Do not spend your whole run on the
hardest of the five. Write your best body to

    docs/dng-parks/<func>.c

**BUT CHECK WHETHER A FAMILY MEMBER IS ALREADY PARKED FIRST.**

    python3 tools/decomp/dngfam.py <func>     # every member of the family
    ls docs/dng-parks/                        # is any member already there?

`func_s20_80026010` and `func_s21_80025BA0` are one family -- the same
170-word body -- and they sit in TWO park files, written by two agents, at 23
and 16 aligned differences. Each file's header says the other member "is
still an `INCLUDE_ASM`, so there was no landed sibling to read": both agents
found the twin STUB and neither looked for a PARK on it. That is an
agent-run of duplicated measurement. If a member is already parked, UPDATE
THAT FILE -- new body, new figure, your rejected spellings appended to its
table -- rather than writing a second one. Landing either member lands both.

If your figure came from a different C SHAPE than the existing park's, keep
BOTH tables and say which shape each belongs to. The disagreement is
evidence: those two parks look like they contradict each other on whether the
array-ref or the integer address form matches, and the resolution -- one
tested the array ref off the SYMBOL, the other off a POINTER LOCAL hoisted
into the preheader -- is a third spelling neither agent tried. You only see
that with both tables in front of you.

Write the file **the moment it is your best, not at the end of your run.** `/tmp` is not
durable -- a cleanup once took four finished functions and five unit copies
with it. Rewrite that file each time your best improves. Do not `git add` it.
A parked function must NOT be left spliced into your copy: revert it to its
`INCLUDE_ASM` stub so the rest of your unit still verifies.

Its header is hand-written and is the point. Give it the word counts, the
aligned difference count, exactly which instructions differ, the copy count
from `dngfam.py`, the mechanism if you know it, and **every spelling you
measured and rejected with its score**. Put the declarations your body needs
between literal `DECLARATIONS` / `END DECLARATIONS` lines inside that header
comment -- a tool reads them, and a park whose declarations live only in your
final report cannot be compiled from the repo at all. Do not write an inner
`/*` anywhere in the header; it ends the comment early. Then:

    python3 tools/decomp/parkcheck.py

## Report back -- a per-function table, then the detail, no transcript

Start with ONE table, one row per function on your list:

    func | target words | candidate words | differing | LANDED or PARKED

Those numbers must come from the final all-at-once `ovlverify` run. Then:

1. For each landed function: the complete body as ONE code block, with its
   comment, STRIPPED OF ANY SPLICE MARKERS you used while working. One code
   block per function, each labelled with its name.
2. The ONE declaration block your copy now has, complete, as a code block --
   not per function, the final state of it. A body whose two new typedefs went
   unreported has cost a round trip before.
3. Any prototype of an ALREADY-LANDED function you had to change or reconcile,
   with the callers you re-measured. That can move code outside your unit, so
   say it loudly.
4. For each parked function: the residue and the path of its park file, and
   confirm its stub is back in your copy.

Confirm in the report: your copy's path, that `csrc/` is unmodified, and that
clang-format leaves your copy untouched.

**APPEND ONE LINE TO `<your dir>/PROGRESS.txt` THE MOMENT EACH FUNCTION
LANDS**, before you write anything else up -- the function name, its target
words, its candidate words and its differing count, one line, nothing else.

    echo "func_s32_800245C8 200 200 0 LANDED" >> <your dir>/PROGRESS.txt

**This replaces an instruction that was impossible and sat in every brief
until 2026-11-26: "send a one-line interim message as each function lands".**
An agent cannot message the dispatcher -- `SendMessage` reaches only agents
that agent itself spawned -- so twelve batches were asked for something they
had no channel for, and one agent finally said so instead of quietly
skipping it.

A file serves the purpose and is checkable. Those lines are what survives if
your run is cut off: a usage limit once killed thirteen agents at once, and
recovering their work meant reading their unit copies and guessing what was
finished. Keep it in your own agent directory -- `/tmp` is **not durable** and a
cleanup once wiped five agents' work.

**It does NOT let me integrate before you finish, and I said otherwise in two
briefs on 2026-11-26 before watching it work.** Integrating means reading your
unit copy, and your copy is being written while you run -- taking a body out of
it mid-run is exactly the thing every other rule here forbids, because a
half-written definition measures as whatever it happens to be. What the file
actually buys is **visibility**: I can see the sweep is healthy, and which
functions are already closed, without interrupting you or guessing from
silence. That is worth having on its own, and it is what the file is for.


## EVERY FUNCTION ON YOUR LIST IS A FEW INSTRUCTIONS FROM EXISTING C

That is why these units were picked -- the picker now ranks on it. **Open the
landed sibling named beside each of your functions and read its comment
first.** Not the listing, not `xsibs`. Expect to change operands.

Batch 13 ran the same way and landed 24 of 24, 20 on the first compile. What
that batch learned, which is worth more to you than the ratios:

* **A twin's stated fact has THREE outcomes, not two.** It can HOLD, be FREE
  on your body though the twin priced it, or be INVERTED -- the twin's
  spelling wrong here and its REJECTED spelling right. Two inversions last
  batch, and BOTH were spotted because the residue matched the price the twin
  had recorded for the spelling it rejected. So when your difference count
  equals a number in the twin's comment, check whether you are looking at
  that fact with the sign flipped before you start searching.
* **So price BOTH spellings in your own comments.** That is what made the
  inversions findable. Write the rejected cost down even when it feels
  redundant.
* **A high ratio does not mean the difference is cheap.** 0.933 landed clean
  twice last batch while 0.956 needed a duplicated arm, and a 0.961 body
  landed first-compile while GAINING a whole state. Additive deltas -- a new
  arm, a dropped call, a renamed table -- are cheap at any size; a changed
  GATE SHAPE or changed statement order inside shared code is expensive
  however small. The ratio cannot tell you which you have.
* **If your body is SHORTER than its twin, references were removed.** That is
  the one thing the numbers do predict, and `dngtwin` prints the delta.
  `func_b42_80173730` is 178 words against a 198-word twin and the 20 missing
  words were three references on one pointer, which dropped it below its
  neighbour in `allocno_compare` for 19 rows. The fix is to write out a block
  the twin reaches by `goto`; jump2 cross-joins the copies for free.
* **References buy nothing until one crosses a power of two.**
  `allocno_compare` is `floor_log2(n_refs) * n_refs / live_length`, so 7 refs
  scoring `2*7/84` loses where 8 scoring `3*8/90` wins. Six would be worth no
  more than seven. Count to the next power of two and stop.
* **WHICH OF A TWIN'S FACTS WILL INVERT IS PREDICTABLE FROM THE PROLOGUE.**
  This is batch 14's main result and it is the first thing to do with a twin.
  Compare `addiu $sp, $sp, -N` and the saved-register list in YOUR listing
  against the twin's. **Allocation facts follow the FRAME; shape facts follow
  the SHAPE.** A 469-word body landed first-compile by taking one twin's loop
  shape and a different family's frame, and the fact belonging to the
  non-matching frame inverted at 471 words / 461 differing -- a twin that
  saves a seventh callee-saved register to hold a loop-invariant pointer
  cannot lend that device to a body with six saves. Confirmed again on a park
  where the same thing named its cheapest fact: a pointer local both twins use
  is 33 aligned differences here and the direct symbol is 11, because both
  twins have seven saves and that body has six. Eight facts for eight across
  two units.
  So: source-shape facts (a local's type, whether an index is inlined,
  statement order) carry over from the twin whose SHAPE matches. Reference
  bumps, hoists, barriers and spread stores carry over only from the twin
  whose FRAME matches, and INVERT if you take them from the other one.
* **A SHORTER BODY USUALLY MEANS MISSING STATEMENTS, NOT MISSING REFERENCES.**
  `dngtwin` prints the word-count delta and flags the shorter direction, but
  of six measured cases only ONE had lost references the surviving code still
  used. In the other five the deleted block's values died with it. So read the
  twin for the statements your listing does not have BEFORE reaching for
  `allocno_compare` -- an agent last batch went to the reference lever on this
  flag and the answer was two absent statements.
* **A TWIN'S FACTS COME IN THREE KINDS AND EACH HAS A DIFFERENT TEST.** Fifty
  priced facts across five units last batch put every inversion in one class:
    - SELECTION facts -- a local's or parameter's declared WIDTH, an idiom
      (`__builtin_abs` against two `if`s), a switch's arm emission order, a
      cast on a callee's result. These follow the SHAPE. **No inversion has
      ever been seen in this class**, and prices often reproduce the twin's to
      the digit.
    - BUDGET facts -- "this device needs a callee-saved register my frame has
      not got". These follow the FRAME, and a mismatch makes them IMPOSSIBLE
      rather than merely wrong. Compare `addiu $sp, -N` and the saved-register
      list; that rules them out before a compile.
    - REFERENCE-COUNT and STATEMENT-ORDER facts -- "write this block out N
      times", "put the decrement before the call". These follow NEITHER, and
      **every inversion measured so far is one of them.** One body with a
      byte-identical prologue and a 1.000 twin had its tail fact invert at 65
      aligned differences. Nothing about a twin predicts these; `-da` on your
      own body does, in one compile.
* **USE THE PROLOGUE TO PICK THE TWIN, NOT TO PREDICT IT.** Two agents last
  batch overruled `dngtwin`'s ranking on the saved-register list and landed
  first-compile: one took a 0.923 twin over a 0.937 one because the 0.937 has
  five saves where its listing has six. Compare `addiu $sp, -N`, the save LIST
  and the save ORDER across all your candidates before opening any C.
* **THE SAVE ORDER PREDICTS ALIAS SPLITS**, because which sites name `p` and
  which name an alias is a statement about which register holds what. A
  mismatch there means every alias-split fact will differ and the shape facts
  beside it will carry. It does NOT mean work is needed.
* **A STATEMENT-ORDER FAULT PRESENTS AS AN ALLOCATION FAULT.** A `$s1`/`$s2`
  exchange -- the textbook "wrong priority" residue -- turned out to be two
  misplaced statements worth 30 differences. **Re-check statement order
  against your own listing BEFORE reaching for a device**; otherwise you apply
  the wrong tool to a real fault, and it can measure as an improvement while
  leaving the cause in place.
* **THREE LEVERS IN THIS FILE ARE NARROWER THAN THEY READ -- check the
  precondition before paying for them.** All three were corrected by
  measurement last batch:
    - `fold_truthop` merging two tests across `&&` happens for two BIT TESTS
      ON ONE **OPERAND** -- corrected 2026-11-25, this said "one LOCAL" and
      that is too narrow. It also fires on the same MEMORY EXPRESSION written
      out twice: `(*(s32 *)(d + 0x1C) & 0x410)` and
      `(*(s32 *)(d + 0x1C) & 0x20000)` across `&&` were merged into one `and`
      against 0x20410, and nesting the `if`s was worth 10 rows. **Writing the
      read out twice does NOT protect you.** Two tests against DIFFERENT
      symbols are still free as `&&` or nested (measured 0 on three bodies).
      It also merges two RELATIONAL tests into an unsigned range check
      (`nx < 0x1020 && nx > 0` -> `addiu -1` / `sltiu 0x101F`), 2 words a
      site, and the converse fires just as often -- read whether the listing
      has one `sltiu` or two relational branches. Nesting a gate whose
      operands differ buys nothing.
    - A SUB-WORD parameter type only bites when the listing shows the
      parameter copied TWICE. With one copy and one use it is free in both
      directions. Count the copies in the prologue.
    - The plain-constant multiply rule is about NON-powers of two. `i * 0x400`
      and `i << 10` are the same tree and free either way.
* **THE PAD RULE IS STATED ON THE CASE COUNT, AND I HAD IT BACKWARDS IN FIVE
  BRIEFS -- including, still wrong, in last batch's.** Tables are 8-aligned
  unconditionally (`BIGGEST_ALIGNMENT` 64), so a pad word appears **iff the
  CASE count is odd AND another 8-aligned rodata object follows**. Since the
  listing shows cases PLUS pad, that means **an odd `.word` count never has a
  pad** and an even one may. Across all 147 tables in the tree: 38 have a pad
  and all 38 have an EVEN `.word` count; 55 more have an odd case count and no
  pad because they are last in `.rodata`. **Do not infer any of this from the
  word count alone -- read the dispatch `sltiu` bound, which is the case count
  outright**, and `python3 tools/decomp/jtcheck.py --func <yours>` prints both
  facts side by side and flags a disagreement.
* **IF AN EDIT TO PRICE A FACT RETURNS THE KEPT SPELLING'S FIGURE, CHECK IT
  APPLIED.** A one-shot text replace hit an already-landed sibling that shared
  two lines with the body being measured, and the result read as "this fact is
  free" when nothing had changed. Anchor the edit on text unique to your
  function, or diff the file, before recording a free.
* **`MEM_IN_STRUCT_P` CAN BE A DEPENDENCE, NOT AN ALLOCATION.** A
  read-modify-write written as a struct field (`b->unk_14 += ...`) lets gcc's
  fixed-scalar-versus-varying-struct rule hoist a file-scope scalar's load
  ABOVE the store, into a delay slot the target leaves as a `nop` -- 12 rows.
  The plain cast `*(s32 *)((u8 *)b + 0x14)` keeps the dependence. When a
  residue is a load filling a delay slot the target leaves empty, try the cast.
* **A SHARED JUMP-TABLE TARGET IS USUALLY gcc's DEFAULT LABEL, NOT ARM SHARING.**
  gcc pads a table to `maxval` with the default label, so an 18-entry table
  with 8 distinct targets last batch had TEN default entries and only seven
  real arms -- nothing to hand-settle. **The discriminator is one grep: if the
  shared label is also the target of the range-check `beqz', it is the
  default.** Writing those out as empty `break' arms would give you eleven
  phantom labels, which is the opposite of the missing-empty-case trap. The two
  errors bracket each other and the dispatch arithmetic settles both.
* **AND DISTINCT ENTRIES DO NOT MEAN DISTINGUISHABLE ARMS** -- I got this wrong
  in two briefs. An agent permuted two arms of a table whose targets were ALL
  distinct and measured **434/0 under ovlverify AND 0 aligned differences**
  while a standalone compile showed the emitted table's last two entries
  swapped; those arms were each `j <label>' plus `li 1'. **The test is per ARM: are the two
  arms' WHOLE emitted blocks word-identical once `j'/branch targets are
  masked?** Walking only to the first branch is too conservative and gives
  false alarms -- an agent last batch had two pairs it had called
  "distinguishable" on that reading, and exchanging them measured 7 and 72
  rows, i.e. they were in fact distinct. Cross-join the candidate arms and
  compare the full blocks.
* **`reg_n_sets' IS A BIGGER LEVER THAN ALLOCATION PRIORITY ON A LARGE BODY,
  and its residue looks exactly like a priority fault.** `k = X + (X >> 4)'
  written as two statements is **94 aligned rows with a whole-body $s1/$s2/$s3
  rotation**, because only a SINGLE-SET pseudo gets `adjust_priority's
  `birthing_insn_p' boost. A second instance arrived sideways: sharing one
  local between two arms 300 words apart put three sets on it, 20 rows. So
  **ask how many times the source SETS each local** -- a question about your C,
  not about `-da', and invisible in a reading of reference counts.
* **AN ALIAS LOCAL CAN BE WORTH 193 ROWS -- look for it before anything else.**
  A body read its early gate through the parameter and every site after a dress
  block through a plain `q = p' alias; spelled `p' throughout it is 193 aligned
  differences. The next-largest fact on that same body is 82. **A residue that
  big reads as "nothing matches" rather than as one missing line**, so if your
  first compile is hundreds of rows out, check for an alias before re-reading
  the shape.
* **THE PROLOGUE PICKS THE TWIN, NOT THE RATIO.** `dngtwin's top row was 0.967
  at +6 words and still a STUB; the right twin was row two at 0.868 and 74
  words SHORTER than the target. Seven callee-saved registers matching is what
  chose it, and eight of its facts then carried at their recorded prices.
  **Compare save counts and save ORDER against every LANDED candidate before
  reading ratios**, and note the free corollary: **saves minus parameters is
  the number of long-lived locals the target has** -- so if you have written
  more locals than that, you have one too many (collapsing four into two was
  worth 102 rows on one body).
* **RUN `python3 tools/decomp/declcheck.py s20 --src <your copy>` BEFORE YOU
  HAND BACK** -- and note the `--src`: without it the tool reads the
  COMMITTED `csrc/` file, not your working copy, so the instruction was
  vacuous in four of last batch's five briefs. One agent caught that; its
  very next use found a real `passing arg 1 ... discards const'.
  An undeclared callee is invisible to every other check: K&R C gives it
  `int (...)', promotes the arguments, and the body still measures 0 differing
  with `relcand' clean. The gap is structural -- a body calling a sibling
  DEFINED LOWER IN ITS OWN FILE has nothing in scope, and the definition
  satisfies the linker. Five such cases were found across the corpus last
  batch and one was introduced BY last batch. The same run also catches
  argument-type mismatches against a real prototype.
* **DUMP EVERY LANDED BODY AND READ IT BEFORE REPORTING.** A crashed restore
  left two pricing edits spliced into one agent's file for the rest of its run;
  both were measured-free, so the body still read 0 and every later figure came
  from a file that was not the landing. **Re-running the verifier is not
  enough** -- it agrees with a contaminated file whenever the contamination is
  free. Put the restore in a `finally' and treat a crashed sweep as
  invalidating the file.
* **AN `addu's OPERAND ORDER IS SET BY POINTER SUM versus INTEGER SUM**, the
  additive sibling of the comparison lever and the same presentation -- a
  two-register exchange with every instruction in place. `pointer_int_sum'
  always builds `PLUS(ptr, intop)', so `q + k * 6 + 0x64' can only emit
  base-first; a target that puts the INDEX first needs
  `(u8 *)(k * 6 + (s32)q + 0x64)'. Worth 2 rows.
* **A MISSING FALL-THROUGH COMES OUT SHORT, NOT LONG** -- 141 words against 144
  and 7 rows, because the `break' lets jump.c fold that arm's tail into the
  common exit. A body a few words short with a switch in it is worth checking
  for a swallowed fall-through before hunting missing statements.
* **A CONSTANT INSIDE A HOISTED ADDRESS IS UNREACHABLE BY ANY CAST.** Where the
  target hoists an address register that includes a constant, fold always
  floats that constant into the MEM's offset field: three different cast
  spellings all measure 335/14 and only a VAR_DECL holding the row base keeps
  the `addiu'. Do not spend measurements on casts.
* **A COMPARISON'S OPERAND ORDER IS A LEVER, AND ITS RESIDUE LOOKS LIKE AN
  ALLOCATION FAULT.** New last batch and the largest single lever on a 449-word
  body: `if (b->unk_0A > ceiling - 0x10)' lands at 2 rows where
  `if (ceiling - 0x10 < b->unk_0A)' is 16 -- a uniform `$v0'/`$v1' exchange
  across 13 instructions with **identical instruction order**. **The listing's
  `slt' operand order is NOT the source's**: the assembler tells you which
  register is compared against which, not which side the C wrote. So when a
  residue is a uniform two-register exchange with nothing moved, flip a
  comparison before reaching for `-da'. It costs one character, and the same
  clamp 40 words later wanted the opposite spelling.
* **A MISSING EMPTY CASE CAN COST YOU THE JUMP TABLE ITSELF.** A switch with
  real work only in cases 3, 4 and 5 must still write 0, 1, 2 and 6 as empty
  `break' arms: with three labels `expand_end_case' falls under gcc 2.7.2's
  `case_values_threshold' and emits a COMPARISON TREE -- 32 words / 19 rows --
  and the unit loses the `.rodata' table the ROM has. **Read the dispatch bound
  as the case SET, not the count:** a bare `sltiu $v0, $v1, N' with no minval
  subtraction means the set starts at 0, so every value below your lowest real
  arm is an empty case you must write out.
* **"NOT MEASURABLE" IS A THIRD VERDICT, DISTINCT FROM "FREE".** Both spellings
  at 0 is FREE only if the tools can SEE the difference; where it is masked the
  verdict is NOT MEASURABLE. Examples: passing three arguments versus none to a
  callee that reads them; a table owner's cross-joined arm order; a gate
  inversion whose arms are textually identical after cross-joining. **Writing
  "free" where you mean "not measurable" is how a wrong container gets
  blessed** -- one committed comment already recorded a table arm order as
  measured-free when it permutes two entries. And N rejected spellings with a
  bit-identical residue are not N facts.
* **THE DRESS-SITE FACT IS NOW PREDICTABLE, after six recorded outcomes.** The
  discriminator is neither the site count nor the save count but **whether the
  sharing sites lie ON ONE PATH or IN DISJOINT ARMS**. Many sites in disjoint
  arms: inline 0, one-per-site 0, SHARED 28 and 20. Two sites on one
  straight-line path: shared 0, inline 19. A local shared across disjoint arms
  extends a live range through code that does not use it and loses the
  register; shared along one path it saves a reload and wins it.
* **ASSERT YOUR ANCHOR IS UNIQUE *AND* INSIDE THE TARGET FUNCTION'S REGION.**
  Uniqueness alone is not enough: a one-shot replace whose tail text was an
  `INCLUDE_ASM' line was unique in the file, sat inside a DIFFERENT function,
  silently edited the wrong body, and both affected measurements read as the
  kept spelling's figure -- the direction that looks like success.
* **A SPELLING THAT *IMPROVES* IS NOT YET EVIDENCE.** One form read 18 rows,
  then 12, and was banked as load-bearing; once the real fault was fixed BOTH
  spellings measured 0. The two faults had been adding. **Bank a fact only when
  it reaches 0, or when it is the single remaining difference.**
* **`+ 0xFFE00000' IS A C89 SIGNEDNESS TRAP IN A DIVIDED EXPRESSION.** No `int'
  holds it, so the whole arithmetic goes unsigned and
  `(a - b + 0xFFE00000) / c' emits `divu' where the target has `div'.
  `- 0x200000' gives the same `lui'/`addu' and keeps it signed. The additive
  hex form is safe until the value feeds a division, a right shift or a
  comparison.
* **TWO BODIES IN ONE UNIT CAN WANT OPPOSITE ANSWERS.** Last batch two facts
  inverted between one agent's own three functions, 40 words apart -- a shared
  tail gate and a store's position -- with each body pricing the other's choice
  at 11-13 rows. **"What the rest of this unit does" is not evidence.**
* **FOR A TWO-VALUE REGISTER EXCHANGE, THE DIAGNOSTIC IS `-da' `.greg's
  ALLOCATION ORDER LINE, not `.lreg' reference counts** -- for an exchange the
  counts are equal by construction. And the answer may be a CROSS: on one body
  three of four share/split combinations measured the identical 12 rows, and
  what landed was shared for one value and split for the other, which no
  one-variable-at-a-time sweep reaches.
* **YOUR OWN MEASUREMENT OUTRANKS ANYTHING IN THIS BRIEF.** Last batch I put an
  INVERTED pad-word rule into all five briefs, and then sent one agent
  `jtcheck` output for a table I had never run the tool on -- I claimed 5 cases
  where it has 6. Two agents caught the rule from their own listings and one
  caught the fabricated figure. **If the brief and the listing disagree, the
  listing wins; say so in your hand-back and keep going.** Do not reconcile
  your reading to mine.
* **THE PAD-WORD RULE, STATED CORRECTLY: `.word count = cases + pad`.** The pad
  is present when the CASE count is ODD *and* another 8-aligned rodata object
  follows; a table that is LAST in its `.rodata` has none even at an odd count.
  So an EVEN `.word` count is the signal FOR a pad, not against one -- of 147
  tables, 38 have a pad and all 38 have an even word count, while 55 have an
  odd case count and no pad. **Read the dispatch bound**: `sltiu $vX, $vY, N`
  before the table's `%hi` load gives N cases, and it is not always on
  `$v0`/`$v1`. `python3 tools/decomp/jtcheck.py --func <yours>` prints both
  facts and flags disagreement.
* **convert.c BITES ON THREE DIFFERENT OPERATORS, all under one precondition:
  a NARROWING CONTEXT AROUND A CONSTANT.** (1) `*(s16 *)dst = <u16 load> -
  CONST + <expr>` folds the constant into unsigned short, giving
  `ori $reg, $zero, 0xFFxx`; put the constant on the other addend. (2) a
  constant MULTIPLY is not a shift here -- `(x << 6) + ((y+1) << 5)` is 4 rows
  against `x * 64 + (y+1) * 32`, because the `LSHIFT_EXPR` arm is guarded on
  `TREE_UNSIGNED (type)` and `MULT_EXPR`'s is not; **the diagnostic is `lhu`
  where the target has `lh`, with nothing else moving.** (3) `(x << 6) &
  0xFFC0` into a `u16` parameter makes the mask HImode -64 and emits
  `li $t0, -64`; write `0xFFFFFFC0`. **When a residue is "a constant
  materialised into a register where the target has an immediate", suspect
  this pass before suspecting allocation.**
* **AN UNDECLARED FUNCTION READS AS FREE** -- the fourth way a measurement
  lies. An agent renamed a call off its unit-local `__asm__` alias while the
  alias declaration was still in the block, leaving the callee UNDECLARED;
  implicit-int promoted the arguments and it read 0 differing. The real figure
  was 659 words / 439. **After any edit touching a call's name or its
  declaration, confirm the callee is still declared before believing the
  number.** The other three ways: an edit that did not apply, an edit that
  applied in the wrong place (assert your anchor occurs exactly once), and a
  comparison the tools cannot see.
* **`MEM_IN_STRUCT_P`: ASK WHETHER THE *TARGET* SERIALISES AT THIS SITE.** One
  656-word body needs BOTH polarities 40 words apart -- four sites want
  `r->unk_1E |= 0x8000` as a COMPONENT_REF (a plain cast is 9 words and 63
  rows, because `true_dependence` then refuses to excuse the field against the
  fixed-address scalar beside it and each read-modify-write serialises behind
  its own `nop`), while a fifth site must stay a plain cast because the target
  serialises there. **Read the `nop`s**, do not pick a house style.
* **A `do { } while (0)` IS TWO DIFFERENT TOOLS AND CAN BACKFIRE.** As a
  reference bump (`flow.c:2122`) it moved `allocno_compare` the WRONG way on
  one body -- 96/12, worse than doing nothing -- where moving ONE store one
  statement later landed at 0, because lengthening the right pseudo's live
  range beats buying refs for the wrong allocno. As a SCHEDULING barrier it
  can be half of a conjunction (5 and 2 rows alone, 0 together) reproducing a
  single reorg delay-slot choice. Try the reorder first; it is cheaper and it
  is not a device.
* **PRICE A TWIN'S "PAIR OF LOCALS" FACT AS TWO FACTS.** One stated a pair of
  `s16` locals at 28 rows and attributed it to a copy not being coalesced. On
  the relink the WIDTH reproduced (`s32` is 27 rows) but **one variable with no
  second local at all is 0** -- the pair was free and the width was everything.
* **AFTER A dngclone LANDING, READ THE REPLICATED COMMENT FOR SOURCE-UNIT
  SYMBOLS.** It renames what is in its `symbol_map` and carries the rest
  verbatim, so a replica named another unit's CALLER as if it were local. It
  also drags the source's declaration-block additions along.
* **A WORD DEFICIT CAN BE A MISSING *EMPTY* CASE.** A switch with real arms only
  for 1 and 2 still emits the balanced tree for {0,1,2}; without `case 0:
  break;` the body was two words SHORT with an `slti`/`bnez` pair absent.
  Count the tests in the target's compare chain against the arms you have
  before concluding you are a statement short.
* **A SELECTION FACT HAS NOW BEEN SEEN TO INVERT, so the taxonomy is a PRIOR
  and not a law.** A twin kept `s16 k' and priced `s32' at 324 words / 8 rows;
  on the relink `s16' is 327/7 and `s32' is what lands -- and BOTH bodies save
  seven callee-saved registers with the same frame, so the save-count rule
  predicts nothing. **The tell is the residue's SHAPE:** a handful of rows that
  are all one redundant `move'/widening copy, with no register rotation, means
  you have the sign wrong. A frame-driven inversion rotates registers through
  the whole body instead. Bet the twin's sign, but read the residue.
* **ALL THREE TOOLS ARE BLIND TO A TABLE OWNER'S ARM ORDER, relcand INCLUDED.**
  `relcand' compares the call SET, not the sequence, so two exchanged arms are
  the same set and read `ok'. (Proven live on the same body by a callee swap,
  which it does flag by name.) Never read a clean relcand as evidence about
  order.
* **BUT ONLY HAND-SETTLE THE ARMS THAT ARE WORD-IDENTICAL AFTER CROSS-JOINING.**
  Measured: exchanging two word-identical arms is 433 words / 0 rows, while
  writing a three-case arm ascending is 431 / 63 -- caught, because that arm
  reaches its shared block through its own label with a different argument. An
  arm with ANY distinguishing instruction is already covered by your
  measurement. **Every jump table in this batch has an EVEN entry count, so
  there is no pad word anywhere: every `.word' is a real case.**
* **"BOTH SPELLINGS MEASURE 0" IS A FREE FACT ONLY IF THE TOOLS CAN SEE THE
  DIFFERENCE AT ALL.** A committed comment recorded its table arms ascending as
  measured-free at 325/0; compiling both spellings standalone shows it permutes
  two of seven entries. On a cross-joined table arm, 0-versus-0 means NOT
  MEASURABLE, never free. Say which one you mean in your comment.
* **convert.c DISTRIBUTES A NARROWING TRUNCATION INTO A SUM, and the constant's
  side decides.** `*(s16 *)dst = <u16 load> - CONST + <expr>' sends the inner
  subtraction through `trunc1' with `typex = unsigned_type (short)', so fold
  rewrites `MINUS(half, 32)' as `PLUS(half, 65504)' in unsigned short; 65504 is
  no `addiu' immediate, cc1psx emits `ori $reg, $zero, 0xFFE0', and loop.c
  hoists it into an extra callee-saved register. Priced 447/14 wrong, 444/0
  with the constant on the OTHER addend, and 444/6 with an `s32' local that
  hides the symptom without fixing the cause. **Diagnostic: an
  `ori $reg, $zero, 0xFFxx' where the target has an `addiu' with a negative
  immediate. Move the constant, do not add a local.**
* **THE WRONG SPELLING COMPILING *SHORT* IS A POSITION DIAGNOSTIC.** An
  early-exit or escape arm placed FIRST is the function's opening code, nothing
  has spent `$a0..$a3' yet, and its argument `addu's are deleted as redundant --
  so it comes out two words short. An arm that is two words short is in the
  wrong position, not missing a statement.
* **A TWIN'S FACT CAN BE FREE *AND* ITS STATED REASON WRONG.** One insisted on
  `>> 3' over `/ 8' because "the bytes are u8 and the shift comes out srl" --
  but an unsigned divide by a power of two simply IS the `srl', so both measure
  0 and the fact was free on the twin too. When a twin's mechanism would apply
  to both spellings equally, suspect the fact.
* **BEFORE CARRYING A TWIN'S ARM-MERGE FACT, CHECK THE MERGE PRESERVES CONTROL
  FLOW.** One recorded a nested-if-versus-`&&' price that was never measurable:
  merging routed a case into a downstream `else if'. Different programs, no
  price to compare.
* **IF `ovlverify' DIES WITH `can't open ... .s for reading', STOP AND REPORT
  IT.** Do not work around it. That is a missing generated listing, and two
  were found that way -- the build does not notice them, and one had been gone
  for five batches.
* **DO NOT INHERIT A TWIN'S "ONE LOCAL PER DRESS SITE" FACT. PRICE IT.** The
  two root dispatchers this block descends from (`func_b01_801714D4` and
  `func_b03_80170E54`) both state it, and it has now measured FREE or INVERTED
  on bodies in FOUR units. On a one-dress-site body it is vacuous; on a
  two-site body the answer was NEITHER of its options -- shared local 89/2,
  one per site 89/10, **no local with the symbol inline at both sites 89/0**.
  And from the frame side the identical sentence HOLDS on a 5-save body and
  INVERTS on a 4-save one while the body stating it saves seven, with the
  price 6 either way -- so the residue cannot tell you the direction and the
  SAVE COUNT can, before any compile.
* **A RESIDUE THAT EQUALS A NUMBER IN THE TWIN'S REJECTED COLUMN IS THAT FACT
  WITH THE SIGN FLIPPED.** Fired cleanly for the first time last batch: a twin
  records 7 for the spelling it rejected, the body's residue was 7, and the
  rejected spelling was the right one. Read a twin's rejected prices as a
  lookup table for your own residue -- in a family where four other facts
  reproduce to the digit, that lookup is reliable.
* **THE WORD DELTA PREDICTS THE WORK BETTER THAN THE RATIO.** Across one
  agent's seven bodies: 0.933 (+15) and 0.912 (+14) landed first compile,
  0.832 (-13) took two, and 0.816 with an unexplained **+49** gap was the only
  real fight -- all 49 words in one duplicated block where the twin's
  reference-count fact inverted. Read `dngtwin`'s `(+N vs yours)` column
  alongside the ratio: a delta one statement accounts for is cheap either way,
  a delta nothing accounts for is where the fight is.
* **RUN `relcand` ON EVERY MEMBER OF A CLONE FAMILY, not just the one you
  wrote.** It caught its first real case last batch on a 120-word clone pair
  whose entire difference is TWO RELOCATED SYMBOLS -- both masked, so copying
  the first member's table into the second reads 120/120/0 and is wrong.
* **IF A FLAG-TESTING GATE VANISHES FROM THE OUTPUT, cse MAY BE WALKING
  THROUGH A ONE-USE LABEL.** `cse_end_of_basic_block`'s follow-jumps arm
  (`cse.c:8139`) hops a CODE_LABEL that is one-use and preceded by a BARRIER,
  carrying a known flag value into a later test and making the gate
  constant-true. Check the `-da` `.cse` dump against `.jump`: if the gate is in
  one and not the other, that is the cause, and a `do { } while (0)` round the
  LAST gate supplies the `NOTE_INSN_LOOP_END` the walk breaks on
  (`cse.c:8054`).
* **ARM ORDER IS MEASURABLE IF AND ONLY IF THE ARMS STILL DIFFER AFTER
  CROSS-JOINING -- and the paragraph below is the case where they do NOT.**
  Corrected 2026-11-25 from two measurements that first looked contradictory.
  On a 12-case owner whose arms differ in call targets and length, ascending by
  CASE VALUE is **360 words / 50 aligned rows** and **entry-address order is
  359 / 0** -- `aligndiff` reads it immediately. On another body's two tables,
  swept both ways, arm order was **NOT MEASURABLE**, because `jump2` collapsed
  the arms into the same blocks.
  **So: if the arms share tails and cross-join, skip the sweep; if they call
  different functions or differ in length, write them in ENTRY-ADDRESS order**
  -- sort the `.word` entries by address, group the case values sharing each
  address, and emit the groups in that order. One body's answer was
  `8/9 -> 5/6/7 -> 12 -> default`, which nobody would write from the case
  values.
  **The warning below still stands for the cross-joining case, and it is the
  nastier one:** there the figure is 0 while the emitted TABLE is permuted, so
  a clean 0 really is not evidence. Settle that case off the listing.

* **WHERE THE ARMS CROSS-JOIN, A TABLE OWNER'S ARM ORDER IS INVISIBLE TO BOTH
  TOOLS, and no tool checks it.** Measured: writing one body's jump-table arms ascending
  instead of descending reads **0 differing under `ovlverify` AND 0 aligned
  differences under `aligndiff`**, while the emitted table's entries come out
  permuted. Both mask a `j`'s target field and neither reads `.rodata`;
  `relcand` audits data symbols and call targets, not table contents. I tried
  twice to automate this and withdrew both attempts (one was blind to
  permutations, one reported the same mismatch on correct code) -- so **for a
  table owner, a clean 0 is NOT evidence about arm order.** Settle it off the
  listing: the arms emit in SOURCE order, so read the entry addresses and
  write the arms in that order. If you want to confirm, compile standalone
  with `tools/cdkgcc -S` and compare the emitted table's entry classes and
  order against the listing's `.word .L...` list. A compare-chain switch in
  the same body DOES price its order, so the two kinds need different
  treatment.
* **IF NEITHER SPELLING OF A TWIN'S FACT GIVES 0, SUSPECT A SECOND FAULT.** A
  real single fault gives 0 on one side. One body measured 83 rows written out
  and 23 shared -- neither zero -- and the twin's fact turned out FREE once an
  unrelated fault was fixed; the two had been masking each other.
* **A FACT CAN BE A CONJUNCTION, and one-at-a-time pricing cannot find it.**
  One body needs a local AND a `do { } while (0)` together: each alone is 4
  aligned differences, both is 0, and **all five failing spellings give the
  IDENTICAL four rows**. That identical residue is the signal. The mechanism:
  `flow.c:2122` counts a reference per enclosing loop so the wrapper does the
  bumping, but a literal constant gives it no pseudo to bump -- the local
  creates the pseudo, the wrapper doubles it. **If two candidate spellings
  each leave the same residue, try them together before trying a third.**
* **`MEM_IN_STRUCT_P` HAS BOTH POLARITIES.** From `cse.c`: a word store
  through a plain cast off a constant pointer invalidates nothing, so two
  loads of one address COLLAPSE; read as a struct field the load is
  `in_struct`, gets invalidated, and the RELOAD SURVIVES. So if the target
  loads one fixed address twice with a word store between and no call, the
  source used a STRUCT FIELD (13 rows the other way); if it loads once, the
  plain cast.
* **RUN `tools/decomp/relcand.py YOUR_COPY.c FUNC ...` BEFORE YOU HAND BACK.**
  New this batch, and it does mechanically what agents have been doing by
  hand: it audits every `%hi` data symbol and every `jal` target in your
  candidate against the listing, from the candidate alone, with no build. A
  wrong bank-local table or a wrong callee measures 0 differing under
  `ovlverify` -- validated with exactly that: a body with one call swapped for
  an unrelated function reads 174/174/0 and `relcand` names both symbols. It
  is the one check `ovlverify` provably cannot do, so it is no longer optional.
* **CARRY A TWIN'S MECHANISM, NOT ITS SPELLING.** A twin required its early
  `return 0` to be the last statement so a CODE_LABEL would sit between a
  `$sN = 0` and the return (`reload_cse_regs` forgets tracked values at a
  label). On the next body the nested form was 7 and the flat form 0 -- **and
  the mechanism still applied one level down**, where an inline `return -1`
  rather than a `goto` was what supplied the label. When a twin's fact
  inverts, **re-read its reason before discarding it**; the mechanism may be
  live in a different shape.
* **A FIRST-COMPILE LANDING'S COMMENT CONTAINS ASSERTIONS, NOT MEASUREMENTS.**
  A body that needed N compiles has priced alternatives; a body that landed
  first compile has none, and everything its comment calls a fact is inherited
  or inferred. Check which kind of twin you are reading, and price an
  inherited assertion before paying for it. Three "weaker than stated"
  findings last batch trace to this.
* **ON AN ACTOR HANDLER, A UNIFORM p/c `$s0`/`$s1` EXCHANGE MEANS LOOK FOR A
  DUPLICATED DEFAULT ARM** before reaching for `-da`. Measured twice at 65 and
  66 rows with the same cure: one shared block the gates `goto`, which drops
  two of `c`'s references and takes it back below the 32-reference
  `floor_log2` cliff. Note this is the OPPOSITE of the case where a block must
  be written out twice -- duplicating an arm adds references to everything it
  mentions, so the discriminator is which pseudo you are trying to win.
* **A COMPARE-CHAIN SWITCH'S ARMS EMIT IN SOURCE ORDER while its decision tree
  is balanced on value**, so a `{4,5}` leaf only falls through into case 5 when
  case 5 is written FIRST (ascending cost 15 rows). A switch that became a
  jump TABLE does not care about arm order at all.
* **READ WHICH ROWS *AGREE*, NOT ONLY WHICH DIFFER.** The best diagnostic in
  this block. One 261-word body came out of its twin at 49 aligned
  differences, every row a `p`/`q` register swap, with `-da` confirming the
  allocno margin -- and inside that swapped region FIVE instructions matched
  the target exactly. That is only possible if your source names the other
  variable at those sites. Moving those five from `p` to the alias reversed the
  rank and landed it at 0, with no device and more natural C. **When the
  residue is a uniform register swap, partition the rows instead of counting
  them: the ones that AGREE are naming evidence.**
* **cse UNIFIES ONLY WITHIN A BASIC BLOCK AND ACROSS NO BARRIER.** Measured
  three ways last batch: two identical expressions in two ARMS of a branch
  need the hoist (9 differences without it), a repeated load across a `div` is
  NOT unified so the source really repeats it (28), and a pointer the target
  reloads three times must NOT be hoisted (10). A call or a `div` is a
  barrier; an arm boundary is another block. **Cheap test: if a hoist makes
  your body SHORTER than the target, the target reloads and the repetition is
  real.**
* **A FRAME DELTA WITH AN IDENTICAL SAVE LIST AND ORDER IS THE ARGUMENT AREA,
  not the allocation.** One body is `-0x30` where all four twins are `-0x28`
  with the same six saves in the same order; the eight bytes are outgoing
  slots 5 and 6, and the body had a SIX-ARGUMENT call no twin makes. Count the
  `sw`/`lw` to `$sp` offsets below the first save slot -- they name the arity
  of the widest call, and that points at a statement before you compile.
* **A MULTIPLY THE LISTING SHOWS AS `sll`/`addu`/`sll` WANTS THE PLAIN
  CONSTANT.** Writing the decomposition you can read off the asm cost 17
  aligned differences; `* 640` is what cc1psx turns into exactly those three
  instructions.
* **NEITHER TOOL CAN SEE WHICH ROUTINE A `jal` NAMES.** A body calling `rand`
  spelled as a completely unrelated function still measures 0 differing.
  `relocheck` now audits call targets corpus-wide, but it needs a build --
  so read the `jal` symbol out of your listing at every call site and check it
  against your source by hand.
* **PRICE WITH `aligndiff`, NOT `ovlverify`.** On one body last batch the same
  change read **126** through `ovlverify` and **16** through `aligndiff`:
  `aligndiff` resynchronises after an inserted word, `ovlverify` compares
  position by position, so any spelling that changes the word count inflates
  wildly. `ovlverify` is the landing gate; `aligndiff` is the measuring tool.
* **ON A TABLE OWNER A WRONG SPELLING'S PRICE IS UNPREDICTABLE, NOT RELIABLY
  INFLATED.** A three-digit residue there can be one `s16` -- but it can also
  be the twin's exact figure. What decides it is whether your change shifts
  code the table's entries point PAST: a change early in the body moves every
  later block and every `j` with it, while one inside the last arm moves
  nothing. **Before reading a big number, look at WHERE the first differing
  row is**; if it is early, you are seeing one fault plus its relocation wake,
  so count distinct causes rather than rows.
* **A DEVICE THAT LEAVES EXACTLY ONE ROW STANDING IS COVERING FOR A MISSING
  STATEMENT.** Last batch a `do { } while (0)` reached 254 words / 1 aligned
  difference with `-da` confirming the mechanism -- and the real fault was a
  block that exists TWICE in the target and once in the source. Writing it out
  again was 254 / 0 with no device at all. **When a reference bump stalls at
  one or two rows, read those rows instead of accepting it**, and read
  `aligndiff`'s BRANCH-TARGET column: a branch to a block where you expect the
  epilogue means duplicated code.
* **PREFER A GUARD THAT NAMES THE COUNTER OVER A DEVICE.**
  `if (j < *(s16 *)(q + 0x2))` and `if (*(s16 *)(q + 0x2) > 0)` emit the
  identical `lh`/`blez`, but the first buys `j` an extra reference and flips
  `allocno_compare` against the loop's giv -- 0 against 6. A guard reads as
  source; a device reads as a device.
* **A SHARED REGISTER IS NOT EVIDENCE OF A SOURCE LOCAL.** cse unifies two
  loads of one symbol whether or not you hoist, so a register reused across
  two tests is free either way and so is a reload. Only price a hoist when the
  residue actually moves. (`fold_truthop` is the opposite: it merges TESTS on
  one local across `&&`, and nesting the `if`s to prevent that IS
  load-bearing, 11-12 differences.)
* **A TWIN'S FACT CAN BE WEAKER THAN THE TWIN STATES.** Its rejected table is
  evidence about the spellings it tried, not proof the kept one is forced. One
  agent found the third spelling its twin never tried also measures 0. When a
  fact costs nothing to satisfy, keep it; when it costs you a fight, **measure
  the spelling the twin never tried.**
* **AND A TWIN'S REJECTED TABLE IS A MENU WHEN YOUR REGISTERS ARE SWAPPED:**
  the spelling it measured and threw away may be the one your body wants. One
  agent's device had to be STRIPPED, and the stripped form was priced in the
  twin's own rejected rows.
* **A LISTING'S `%hi(SYMBOL)` IS NOT EVIDENCE THE SOURCE DECLARED ONE.** splat
  infers the name from the `lui`/`lhu` pair, so for an address outside the
  loaded image it names a symbol either way. One body lands only as the bare
  constant `(*(u16 *)0x80013714 & 8)`; the declared symbol is 1 aligned
  difference, a `%hi` that reorg lifts into a delay slot the target leaves
  empty.
* **"Strip the twin's devices" is a procedure, not a prediction.** Measure,
  don't assume: one body last batch needed its twin's `do { } while (0)`
  reference bump KEPT. But a twin's separate LOCALS are load-bearing until
  measured otherwise -- collapsing two into one cost 252 differing once and
  14 rows of $v0/$v1 swaps another time, 150 words from the cause.


## WHAT LAST BATCH ADDED (two parks batches: 10 of 13 parks resurrected)

* **A CLEAN TWO-REGISTER EXCHANGE WITH NOTHING MOVED IS RARELY AN ALLOCATION
  FAULT.** FIVE causes measured, `allocno_compare` the answer in NONE, and the
  directions conflict with each other:
  a comma in a `for` initialiser (16 rows); one subtraction's OPERAND ORDER
  (32); two locals that must be ONE (49); a dependence via `MEM_IN_STRUCT_P`
  (14); and a subtraction that must be SPLIT into three statements (10).
  **Permute the DECLARATION order first** -- if all permutations measure
  identically (8 of 8, 120 of 120 on two bodies), allocation priority is out
  and the cause is in the arithmetic or the statement order.
* **`-da` CORROBORATED A FALSE STORY THREE TIMES IN ONE BATCH.** It tells you
  WHAT the allocator did, never WHY its input looked that way. `.greg` will
  happily print a reference-count margin that fits a story whose real cause is
  an operand order. **Use `-da` to confirm a mechanism you already have a
  source change for.** Where it DID earn its place: establishing that
  `cost(high -> lw) == 1` so `rank_for_schedule` falls through to `INSN_LUID`.
  That is a question about the compiler's INPUT COSTS, not the outcome.
* **A `SYMBOL_REF` COSTS NO INSN AT EXPAND TIME, which is why hoisting a table
  base into a pointer local works.** `expand_expr`'s PLUS arm returns a bare
  `symbol_ref` with nothing emitted, so `expand_binop` forces it into a
  register AFTER the index -- so the index's `andi` always gets the lower
  `INSN_LUID` and always wins a scheduler tie. `u1 = D_8006CCD8; t1 = &u1[k];`
  gives the base a LUID at all. Only its position RELATIVE TO THE INDEX
  matters; moving it past an unrelated store is not measurable.
* **WHERE OPERATIONS ARE INDEPENDENT, THE EMITTED ORDER IS THE COMPILER'S
  CHOICE AND MUST BE PRICED, NOT READ.** Three members now: sched1 transposes
  adjacent STORES, `fold` reassociates adjacent SUBTRAHENDS (32 rows), sched2
  swaps two `lbu` off one base. Check for a dependence first -- that is what
  tells you which kind of order you are looking at.
* **A NINTH WAY A MEASUREMENT LIES: A STALE BUILD.** `ovlverify` without a
  rebuild measures the PREVIOUS file state -- 45 differing against
  `aligndiff`'s 0 on the same body. **The two tools disagreeing at EQUAL WORD
  COUNTS is a stale build, never a compiler effect.** Never call either tool
  except through whatever rebuilds your candidate.
* **AN EQUAL ROW COUNT CAN HIDE A DIFFERENT FAULT.** A park recorded a spelling
  as "FREE -- 2 rows either way". It is 2 rows either way and they are NOT THE
  SAME 2 ROWS: the store relocates and sched adds a dependence that was not
  there. **If a row count does not move under an edit, read the ROWS before
  calling it free.**
* **`convert.c` TAKES ITS UNSIGNEDNESS FROM THE FIRST OPERAND.**
  `*(s16 *)dst = *(u16 *)src - 0x10 + (rand() & 0x1F)` narrows to UNSIGNED
  short, so `-0x10` becomes `0xFFF0`, outside `addiu`'s range, and you get
  `ori` + `addu` where the target has `addiu -0x10`. The cast goes around the
  SUBTRACTION, not the load. Three preconditions, all required: destination
  narrower than `int`, first operand unsigned, TWO OR MORE operands -- with a
  single operand the cast is FREE.
* **`volatile` ON A STORE is the only spelling that stops cse forwarding a
  stored value to a same-base reload** (measured across seven pairs: every
  same-base pair forwards, every cross-base pair survives). It is a DEVICE, not
  macro residue -- last resort, and say so in your comment.
* **LAND THE SMALL SIBLINGS IN YOUR OWN UNIT FIRST -- now SIX for six**, and
  the two sources do different jobs: the IN-UNIT SIBLING gives you SPELLINGS
  (casts, local types, the bank's idiom), the RATIO TWIN gives you CONTROL
  FLOW. Read the twin for the skeleton and the sibling for the flesh; when they
  disagree about a spelling, the sibling is the one in your bank.

## OLDER MATERIAL FOLLOWS -- STILL TRUE, LESS URGENT

## THE FIRST PARKS BATCH (5 of 7 parks landed)

Batch 30 re-dispatched seven PARKED bodies and landed five. Read these first;
they are about how to use a park file and they override anything older.

* **A PARK'S DIAGNOSIS IS ITS BEST CONTENT. ITS REMEDY IS ITS WORST.** Two
  parks landed by KEEPING the analysis and DISCARDING the fix. One had the
  right handle (`CONFLICTP(heading, s)`) and a merge that cost 7 rows, where a
  FREE conflict was sitting in the prologue -- two values cse had already
  unified into the exact registers the loop wanted. One blamed
  `allocno_compare` and spent sixteen `do { } while (0)` pins on a fault that
  was `true_dependence` (sched.c:817), reachable by one
  array-ref-versus-plain-cast character. A third park's entire analysis
  described a symptom that does not exist.
* **I TOLD AGENTS A PARK'S REJECTED TABLE IS "MEASURED FACT -- DO NOT
  RE-MEASURE IT". THAT WAS WRONG.** A park never reached 0 differing, so
  **every figure in it was measured against a body with a live fault** --
  exactly the condition that invalidates a price. Two of one park's six HELD
  facts re-priced to **FREE**; two of another's **inverted**. Read a park's
  table for WHICH SPELLINGS WERE TRIED, which no re-measuring recovers, and
  re-price the ones you intend to keep.
* **THE THREE-KINDS TAXONOMY PREDICTS WHICH PARK FACTS SURVIVE -- 6 for 6 on
  one body.** All four SOURCE-SHAPE facts carried (a mask width, an operator,
  two store-versus-dependent-load placements); both ALLOCATION/ORDER facts
  inverted (a device, and the order of two symmetric assignments). So sort an
  inherited park's table before measuring: casts, masks, operators, widths and
  store placement should carry; devices, reference bumps and symmetric-order
  facts should be re-priced.
* **A PARK'S OWN OBSERVATIONS CAN CONTAIN THE ANSWER UNTRIED.** One park's
  author wrote down "the target packs o, g, cx AND v all into $s0" and tried
  reusing two of those locals. The one they did not try is the one that landed
  it, worth 9 rows. Read the observations as leads, not just the conclusions.
* **THE CALL-ARITY FACT IS NOW ON THREE BODIES, same figure every time: ONE
  WORD SHORT and 7 ALIGNED ROWS**, from a call written with one argument where
  the target passes two. The second argument gives the pseudo a copy preference
  on the LOWER-numbered register, so `find_reg`'s ascending scan takes `$a1`
  over the `$a2` its entry copy suggests. **One absent argument produces three
  symptoms at once** -- a missing entry `addu $a1, $a2, $zero`, a wrong base
  register on one store, and a two-register shift in an unrelated block 350
  words away. Invisible to `ovlverify`, `relcand` and `declcheck` alike. If
  your body is one word short with a missing entry copy, count your call
  arities before anything else.
* **WHEN A TWIN'S CALL HAS ONE MORE ARGUMENT THAN YOURS, THAT IS A LEAD ABOUT
  YOUR REGISTER ALLOCATION**, not a shape difference. It reads as innocuous in
  the twin's C and is invisible as a fact.
* **TRY `dngcallee.py` BEFORE `dngtwin`, NOT AFTER -- IT BEAT IT ON THREE
  UNITS IN ONE DAY, AND THE MARGIN WAS WHOLE BODIES.** Promoted 2026-11-25 on
  this evidence: on one unit `dngtwin --unit` found no carved sibling at 0.70
  for **seven of eight** stubs including a 709-word driver, while `dngcallee`
  matched **9 of 10 symbols with a clean gap to the 8/10 tier**, named two
  donors with the same frame and the same ten saves, and all thirteen of the
  donor's priced facts carried -- a 720-word body on the **first compile**. On
  another unit it found a landed *pair* in a different bank that gave two
  first-compile landings of 267 and 329 words, where `dngtwin` did not name
  them. A third unit had no twin at all above 0.70.
  **Order to use: (1) your unit's own landed bodies and their comments,
  (2) `dngcallee`, (3) the spawner/step partner, (4) `dngtwin`/`xsibs`,
  (5) grep an arithmetic idiom or a resolved real name.** `xsibs` has now been
  useless on six consecutive `s`-units.

* **`tools/decomp/dngcallee.py`** -- donors by CALLEE-SET overlap,
  per FUNCTION rather than per file, which is the detail that makes it work.
  It exists because one 531-word body has FOURTEEN landed relinks that
  `dngtwin` cannot see: they are all in its pool, but four of five are outside
  the 0.75-1.33 length band and the fifth scores 0.546, because these are
  state machines **whose arm count differs per bank**. Length-banded mnemonic
  similarity is the wrong metric for that family and no floor fixes it.
  Measured caveat: **sharp at 8-10 symbols (1-2 functions), NOISE at 2 (83 and
  89 functions).** Read the "best overlap, reached by N function(s)" line and
  the gap to the next tier -- a clean gap means the rows above are the family.
* **LAND THE SMALL SIBLINGS IN YOUR OWN UNIT FIRST -- now four for four.** A
  no-twin body needed no donor at all because a function landed an hour
  earlier in the same file BUILDS THE PAYLOAD it reads. Another landed FIRST
  COMPILE off an in-unit sibling while ignoring an available 0.785 carved twin.
  An in-unit landed body shares the bank's symbols, its struct layouts and
  often the exact idiom -- none of which a ratio measures.
* **AN IMPROVEMENT IS NOT EVIDENCE, caught four more times**, twice by agents
  on themselves: one nearly banked a 16-to-2-row "fact" that measures **FREE**
  from a clean body, because it perturbs the same allocation a statement-order
  fault decides. **Price only from a body at 0 and re-price anything you
  measured earlier.**
* **A STATEMENT-ORDER FAULT CAN PRESENT AS A CLEAN TWO-REGISTER EXCHANGE WITH
  `-da` CORROBORATING AN `allocno_compare` MARGIN.** One body showed a textbook
  `$s0`/`$s1` swap, nothing moved, no word gained, and `-da` even showed the
  reference-count margin -- all of it a red herring. The fix was one comma:
  `for (i = 7, t = p + 7; ...)` instead of a separate statement. **If you are
  sitting on a two-register exchange with nothing moved, re-check the order of
  two independent statements before spending anything on a device.**

## OLDER MATERIAL FOLLOWS -- STILL TRUE, LESS URGENT

## WHAT BATCH 29 ADDED (22 of 23, one park, 72.6%)

Read these before the older material. Several of them correct things earlier
briefs told agents, so where they conflict with a section further down, these
win.

* **A DONOR'S *FREE* VERDICT CAN INVERT -- this is new and it matters to you
  more than any price.** A donor recorded a statement order as "656/0 either
  way, FREE". On the relink the other order was **21 aligned rows**. The two
  values had identical reference counts and identical uses, so
  `allocno_compare` could separate them only on LIVE LENGTH -- set by which is
  written first -- and the donor simply had enough slack that it did not
  matter. **So a donor's FREE transfers only if your body has the same slack.**
  Re-price any FREE a donor states about the ORDER OF TWO SYMMETRIC statements
  (equal reference counts, equal uses); that is the class known to invert.
* **A FACT'S PRICE DEPENDS ON WHETHER YOUR BODY ALREADY DOES THE DEVICE'S
  JOB.** One construct priced on FOUR near-identical bodies in ONE FILE, all
  leaves with byte-identical prologues: **18, 12, 7 rows and FREE**. The
  discriminator is not the frame and not pressure -- it is whether anything
  else already separates the two things the device separates (a call, a gate,
  an unrelated statement). **Before paying for a separating device, look for
  something already between them.** A price of 0 for a known-good device is
  evidence about your BODY, not about the device.
* **A PRODUCT IS NOT THE DEFAULT.** Last batch I told agents to cross any facts
  that landed together. One ran three 2x2s and all three were ADDITIVE to
  within one row. **Cross only facts that touch the SAME VALUE or the same
  instruction** -- both measured conjunctions did, all three additive crosses
  did not. A blanket "always cross" doubles your measurement bill.
* **READ THE WORD COUNT BEFORE COMPARING A ROW TO A DONOR'S FIGURE.** A donor's
  30-row row came in at 62 rows AND THIRTY-SIX WORDS LONG, because the shared
  local's live range stopped jump2 cross-joining two copies, so the second copy
  survived into the output. If the word count MOVES, the spelling changed what
  jump2 could join and the two figures are not comparable at all; if it does
  not, the gap is pressure and the direction is the transferable part.
* **THE DUPLICATED-ARM LOCAL RULE IS TWO RULES COMPETING.** `combine_regs`
  (`local-alloc.c:1759`) wants PER-COPY scope for a local whose output ties to
  a dying input; `allocno_compare`'s reference count wants SHARED scope when
  the extra references cross a power-of-two cliff. One body needed its
  read-modify-write COUNTER shared (14 refs, crossing 8) while the accumulator
  beside it wanted per-arm scope. **Count the references both ways before
  choosing** -- that is arithmetic on your source, and it decides a 4-to-12-row
  question with no compile.
* **A LISTING'S STORE ORDER IS NOT THE SOURCE'S STORE ORDER.** sched1
  transposes independent adjacent stores: source `+0x1C, +0x1E, +0x1A` emits as
  `+0x1C, +0x1A, +0x1E`. Treat adjacent INDEPENDENT stores as an order you must
  PRICE, not read. Stores with a real dependence cannot be transposed and CAN
  be read directly. (Two separate bodies also had their two `lbu` emit in the
  opposite order from the source for the same reason.)
* **SWEEP A STATEMENT'S POSITION ACROSS EVERY SLOT AND READ THE CURVE.** Since
  you cannot read an independent statement's position off the listing, this is
  the recovery procedure. One body's sweep over seven slots gave
  `6 / 5 / 2 / 5 / 6 / 7 / 8` -- convex, single minimum, so you can BISECT
  rather than sweep exhaustively, and a FLAT sweep means the position is NOT
  MEASURABLE and should be recorded as such. Note that body's minimum was 2,
  not 0: a positional sweep locates one fault, it does not finish a body.
* **gcc MERGES TWO ADJACENT `|=` ON ONE LVALUE AND DELETES THE STATEMENT
  BETWEEN THEM.** So an interposed store is load-bearing -- its presence is
  what stops the two ors becoming one `ori`. Count the `ori`s in your listing
  against the number of `|=` you mean to write: two ors means two statements
  gcc was prevented from merging.
* **`MEM_IN_STRUCT_P` CUTS BOTH WAYS.** The plain cast KEEPS a dependence; the
  array/field ref (`((s16 *)p)[0x15]`) makes `MEM_IN_STRUCT_P` true so
  `true_dependence` goes false and reorg can SINK a store into a delay slot.
  The listing tells you which you need: a delay slot the target fills but yours
  leaves empty means the dependence has to go; a load yours hoists above a
  store means it has to stay.
* **IF YOU SEE ANY `div` IN A LISTING, THE SOURCE DIVIDED BY A VARIABLE.**
  `expand_divmod` strength-reduces a literal divisor at expand time, four
  passes before cse, so a literal never reaches `div`; the giveaway for having
  written one is `lui $reg, 0x2AAA` or a shift sequence. Confirmed on two
  bodies independently.
* **A SEVENTH WAY A MEASUREMENT LIES: A DECLARATION-ONLY EDIT.** Adding a local
  without rewriting its use sites applies perfectly, passes the
  anchor-uniqueness assert, and reads the KEPT spelling's figure. **A pricing
  edit must touch a USE SITE.** If your diff is one line ending in a semicolon
  with no operator in it, you have priced nothing.
* **AN IMPROVEMENT IS NOT EVIDENCE, confirmed three more times**, and twice the
  mid-fight figure was roughly DOUBLE the true one (10 against 6, 26 against
  11). One agent retracted a 4-row fact that measured 0 from a clean body.
  **Price only from a body at 0, and re-price anything you measured earlier.**
* **WHEN A K&R EMPTY-PAREN PROTOTYPE MAY BE WIDENED.** Safe when every existing
  reference only TAKES THE ADDRESS. **Must NOT** be widened when a landed
  caller CALLS it with a short argument list -- one such call is priced at
  433/0 in its own comment, and tightening the declaration stops that call site
  compiling. Grep the name and classify every reference first; this is the one
  place where satisfying `declcheck` is the wrong move.
* **A MISSING ENTRY `addu $aN, $a0, $zero` HAS THREE CAUSES.** Folded by
  regmove (narrow the local so the copy TRUNCATES -- regmove cannot fold a
  truncation); a SCHEDULING shadow (something upstream took that register); or
  an ABSENT ARGUMENT that would have pinned it. For the third: **the missing
  argument is whichever value is already sitting in the skipped register with
  no instruction setting it**, and a body ONE WORD SHORT with a copy missing
  should have its call arities counted first.
* **LAND THE SMALL SIBLINGS IN YOUR OWN UNIT FIRST.** One no-twin body needed
  no donor at all, because a function the same agent landed an hour earlier in
  the same file BUILDS THE PAYLOAD that body reads -- so the two listings
  interpret each other and no offset needed guessing. That route exists only
  because you have the whole unit.

## OLDER MATERIAL FOLLOWS -- STILL TRUE, LESS URGENT

## WHAT BATCH 28 ADDED (14 of 14, no parks, FOUR UNITS CLEARED)

Batch 28 landed everything it was given, 10 of 14 on the first compile, and
produced six results that change how you should work. Read these before the
older material below.

* **AN IMPROVEMENT IS NOT EVIDENCE. PRICE FACTS ONLY FROM A BODY THAT REACHES
  0.** An agent recorded 8 rows for a loop-test spelling while two unrelated
  faults were still live; once those were fixed, BOTH spellings measured 0. A
  figure taken mid-fight prices your spelling AND every remaining fault
  together, and it can land on exactly the magnitude of a known lever. **Go
  back and re-price anything you measured before you hit 0.** Three of the
  retractions in the levers file began life as improvements.
* **A ONE-AT-A-TIME FACT TABLE CANNOT SEE A CONJUNCTION -- cross the two or
  three facts that landed together.** A 2x2 on one body:
  phantom device + `s16` = **0**, phantom + `s32` = 5, no device + `s16` = 44,
  no device + `s32` = 49. In the no-device column the width looks like noise;
  in the phantom column it is everything, and neither half alone reaches 0.
  The twin's table had priced the width only in its no-device column -- an
  honest table that is still misleading on a relink, because the fact it
  needed to state is a PRODUCT. Four compiles for a 2x2 beats six
  one-at-a-time rows. A second, independent conjunction turned up on another
  body the same day (two equal constants' STORE ORDER times a read-back), so
  this is not a one-off.
* **A SEVENTH WAY A MEASUREMENT LIES: A DECLARATION-ONLY EDIT.** An agent added
  `u8 *tbl;` to price a hoist and never rewrote the use sites. It compiled, the
  local was unused, and it read the KEPT spelling's figure -- one line from
  being banked as FREE. The anchor-uniqueness assertion your harness runs does
  not catch this, because the edit applies perfectly. **A pricing edit must
  touch a USE SITE, not only a declaration**; if your diff is one line ending
  in a semicolon with no operator in it, you have priced nothing.
* **A DUPLICATED BLOCK NEEDS ITS OWN LOCAL ONLY FOR A VALUE THAT UPDATES
  ITSELF.** `local-alloc.c:1759` measured 4 rows on four bodies, and the
  sharpest reading came from TWO LOCALS IN ONE DUPLICATED BLOCK, where nothing
  else can differ: the read-modify-write local (`w = mem; mem = w + 1`) costs 4
  rows shared, and a local assigned once and thereafter only READ is FREE
  shared. `combine_regs` refuses to tie an output to a dying INPUT. So: own
  local per copy for counters and accumulators; a read-only local can be
  shared, measured. **Three other explanations of this fact were offered and
  all three were wrong while the measurement never moved** -- cite the fact,
  not the story.
* **A NARROW SIGNED CALL RESULT CUTS BOTH WAYS.** The older lever says a narrow
  declaration is what the target wants. The opposite also happens: with `s16
  hit = f()` gcc keeps TWO pseudos -- the raw result for byte stores and the
  sign-extended one for comparisons -- and pays a `move` to carry the raw
  across a later call (620 words / 37 rows), where `s32 hit = (s16)f()` is one
  pseudo and lands. **The discriminator is how the value is USED**: narrow uses
  only, declare it narrow; both narrow and wide uses, declare it wide with an
  explicit cast.
* **IF YOU SEE ANY `div` IN A LISTING, THE SOURCE DIVIDED BY A VARIABLE.**
  `expand_divmod` strength-reduces a literal divisor at expand time, four
  passes before cse, so a literal never reaches `div` at all -- the giveaway
  for having written one is `lui $reg, 0x2AAA` (the /3 magic multiply) or a
  shift sequence. `addiu $v1, $zero, 0xC` followed by `div` is cse
  constant-propagating a KNOWN STORE into a variable division. Confirmed on two
  bodies independently, worth 4 words / 23 rows and 19 rows respectively. The
  second division of a pair reloads the halfword because an intervening `sw`
  invalidates cse's entry.
* **A COUNTER LOOP IS ROTATED IFF ITS TEST IS IN THE CONDITION POSITION.**
  `jump.c` rotates only when the insn after `NOTE_INSN_LOOP_BEG` is a
  conditional jump. `while (--n > 0)` is rotated (the bound folds, the test
  moves to the bottom) -- 604 words / 42 rows. `while (1) { n--; if (n <= 0)
  goto out; ... }` is not, and then `reorg.c:3728` steals the decrement into
  EVERY back branch's delay slot with a compensating `addiu +1` on the
  fall-through. **None of those adjustments is source** -- one body had five,
  and the sixth site had none because the counter is dead where the loop falls
  out. A decrement inside delay slots means the `while (1)` shape.
* **A BLOCK BETWEEN TWO SWITCH ARMS IS A `goto` TARGET.** gcc emits a loop's
  exit code AFTER the loop, so a block sitting between one arm's closing `j`
  and the NEXT arm's table entry address belongs to neither arm. Read the arm
  entry addresses off the table first and treat anything below an arm's entry
  as outside it. Worth +2 words / 19 rows written in the wrong place.

## OLDER MATERIAL FOLLOWS -- STILL TRUE, LESS URGENT

## WHAT BATCH 13 ADDED (17 of 17, no parks, 12 on the first compile)

* **A DUPLICATED ARM NEEDS ITS OWN LOCAL, and the mechanism is
  `local-alloc.c:1759`, not `allocno_compare`.** When you write a block out
  twice to satisfy a reference count or a cross-join, a local SHARED between
  the two copies cannot keep its register: `combine_regs` refuses to tie an
  output to a dying input whose `reg_qty` is -1, because such a pseudo "is not
  local to this block **or dies more than once**". Nine alternative spellings
  on that body all measured 4 rows or worse; **only the SCOPE of the local
  moved it.** So when you duplicate a block, declare the local inside each
  copy. This is also why `reg_n_sets` and this fact look alike from the
  residue: both present as a register rotation and neither is a priority fault.
* **A NARROW SIGNED RETURN VALUE IS A SHAPE FACT, not an allocation one.** cse's
  `canon_reg` cannot equate the copy across a HI->SI widening, so declaring a
  callee's result `s16` rather than `s32` changes the emitted code and no
  device recovers it. `s8` lands the same way, so the fact is "narrow AND
  signed", not "`s16`". Check the width you gave every call result before
  pricing anything else.
* **AN UNUSED TYPEDEF IN YOUR UNIT'S DECLARATION BLOCK CAN NAME YOUR TWIN.** A
  typedef is dragged along with the idiom that needed it, so one sitting in
  your unit with no use is residue of a shape that has not landed here yet --
  grep the other units for bodies that USE it. One worked case last batch.
  There are 152 unused `UnkDng*` typedefs across 55 units; **no hit rate is
  claimed** and I will not pretend to one, but it costs a grep.
* **`aligndiff` TAKES EXACTLY ONE FUNCTION NAME -- unlike `ovlverify`.** It
  used to discard extras silently, and an agent lost a sweep of EIGHT
  spellings because all eight measured the other body in the file and read 0.
  It now refuses extra names outright, but the lesson stands for any tool:
  **if a sweep returns the same figure for every spelling, suspect the
  measurement, not the compiler.**
* **"FREE" AND "NOT MEASURABLE" ARE DIFFERENT VERDICTS AND ONLY ONE OF THEM IS
  INFORMATION.** FREE means you compiled both spellings and the row count was
  identical. NOT MEASURABLE means the tool could not see the difference -- the
  body was already 0, the edit did not apply, the function was not the one
  measured, or the construct cannot be spelled the other way. **Record which
  one you have**; a NOT MEASURABLE written down as FREE is how a wrong fact
  gets into this file, and three of the corrections above began that way.

## YOUR LEADS ARE COMPUTED AT A LOWER FLOOR THIS BATCH (0.60, not 0.70)

Every batch before this one searched for twins at ratio >= 0.70, and "no
carved sibling" has therefore always meant **"no sibling above 0.70"**. I
measured what that hid: re-sweeping at 0.60 exposes **135 stubs / 13,568
words** that previously reported nothing, 48 of those stubs between 0.65 and
0.70. One 709-word stub scores 0.678 against a landed body carrying one of
the richest fact tables in the tree.

So your `LEADS.txt` may show a twin in the 0.60s. **That is a real lead and it
is a weaker one**, and the difference matters to how you use it:

* A 0.9x twin is a body to EDIT. A 0.6x twin is a body to READ -- take its
  callee set, its locals' types, its gate shapes and its declaration block,
  and expect to write the control flow yourself.
* The three-kinds taxonomy still applies, but at 0.6x the FRAME almost
  certainly differs, so **assume every budget fact inverts** until the
  prologue says otherwise.
* A 0.6x twin is worth most when your body has NO better lead. If a 0.9x twin
  is on your list too, spend your budget there first -- a landed body pays
  forward (see below).

## FOUR LEAD SOURCES IN THIS ORDER -- AND I HAD THE ORDER WRONG LAST BATCH

    1. the ratio (`dngtwin`, your LEADS.txt)   -- always, it is free
    2. the CALLEE-SET INTERSECTION             -- the workhorse on a naked body
    3. `dngnear` (the call graph)              -- only if your unit is mostly
                                                  LANDED; it returns nothing on
                                                  a mostly-stub unit, measured
    4. the field-order grep                    -- last, when 2 found nothing

I told two agents last batch that an empty `dngnear` pushes them to the
FIELD-ORDER grep. That was wrong: the **callee-set intersection** solved a
465-word no-twin body outright on the first try and the field-order grep was
never needed. The procedure for 2 is: **grep each of your body's callees and
data symbols across `csrc/ovl_dng_*.c` and intersect the FILES.** On that body,
one file appeared in ALL SEVEN greps and held the donor -- 656 words against
465, far outside any length band, so no ratio at any floor could have reached
it. It works because it is an argument about WHAT YOUR BODY TALKS TO, not about
how its instructions look, so a donor 40% larger is as findable as one the same
size.

## THE OLD FRAMING OF THE LEAD SOURCES FOLLOWS -- ITEM 2 AND 3 ARE SWAPPED ABOVE

`dngtwin`'s ratio cannot reach two whole classes of donor, and batch 28 landed
bodies off both. **Use them in this order, and do not stop at the first.**

**1. The ratio** -- your `LEADS.txt`, computed at 0.60 (see below).

**2. THE CALL GRAPH, which is new tooling written for this batch:**

    python3 tools/decomp/dngnear.py <your func>
    python3 tools/decomp/dngnear.py --unit <YOUR-UNIT>

The inference is structural, not statistical: if a LANDED body in your unit
calls (or is called by) your stub, and that neighbour has a landed twin in
another unit, then **whatever bears the same call relation to that twin is a
donor for your stub -- at any word count, at any ratio.** The driver/spawner
pair is a stable unit in this block and the relation survives the relink even
when the word counts diverge past any shape metric.

Why it exists: one batch-28 body's real donor was **77 words against its 121**,
outside `dngtwin`'s 0.75-1.33 length band entirely and therefore invisible at
EVERY floor. The agent found it by hand -- land the driver off its twin, then
read that twin's CALLER -- and that body handed over the dress skeleton, the
declaration block AND the pointer alias split for zero measurements.

Read the output as DONORS, not twins. No ratio is printed for the stub-to-donor
pair because none is meaningful; confirm by reading the donor's SKELETON (its
callee sequence and dress sites), not by expecting word counts to agree. A
donor flagged **[N independent paths -- STRONG]** was named by several
different neighbour-twin routes, which is the signal worth trusting.

**3. THE FIELD-ORDER GREP, when your callees are all library-common.** The
callee-set recipe fails when a callee appears in thirty files. What works then
is the STORE ORDER of a publish block: one body's `p[0xB3] = 0; p[0xB1] = 0;
p[0xB2] = 0;` -- descending-then-ascending, not an order anyone writes by hand
-- occurs **exactly once in the whole corpus**, and that single hit was the
donor. An unusual field order is as distinctive as a call sequence, and BOTH
`dngtwin` and `relcand` are blind to it: one compares mnemonics, the other
compares call and symbol SETS.

And one thing a donor gives you that a twin often cannot: **a body that still
has TWO pointer parameters where yours has one is a map of your alias split.**
Group your sites the way it splits its parameters. An alias fault is worth up
to 193 rows in this file, so this is the cheapest 193 rows you will ever save.

## A TWIN'S FACT THAT TWO RELINKS BOTH MEASURE FREE IS A FACT ABOUT THE TWIN

Last batch, two independent relinks of one landed body both measured the same
fact FREE that its comment prices at **63 aligned differences** -- with
BYTE-IDENTICAL prologues, so the frame explains nothing. Meanwhile five of that
twin's other six facts reproduced at or within one row on those same two
bodies.

So it was one weak row in an otherwise exceptional table, and the lesson is how
to read such a table: **size is not reliability.** A large figure sitting beside
several transferable ones is the row to RE-PRICE first, not the row to trust
hardest. When you write your own comment, mark a row "priced on this body only"
if you have reason to think it is pressure-specific -- both of those agents did,
and that is why the disagreement was legible instead of looking like an error.

Your leads are in `LEADS.txt` beside this brief; re-run them with

    python3 tools/decomp/dngtwin.py --unit <YOUR-UNIT>

and read that file's docstring for what it CANNOT see: a BLEND, one
function's head with another's tail, has a low whole-body ratio against each
source. Three statistical ways to find those were measured and all failed;
what works is to account for your whole callee set and grep on whichever
callee the matched twin does not explain.

**A rodata-ordering question is answerable ONLY from the real build or the
container, never from a standalone compile of your copy.** `asmlib.compile_asm`
does not expand `INCLUDE_ASM`, so the still-stubbed tables that normally
interleave are absent and the layout you measure is not the one that ships. An
agent last batch concluded from that that its table landed at the wrong offset
and that a sweep-doc section was wrong; both were false. Report your table's
ENTRIES and leave placement to the dispatcher's build gate.


## FOUR OTHER UNITS ARE HELD BY LIVE AGENTS

    ovl_dng_s31, ovl_dng_s39, ovl_dng_s28, ovl_dng_s02

`csrc/` is READ-ONLY reference for the length of this run and stays as
committed, so a landed body you read there is real. Never write to any of
those files or a copy of one. You cannot see the other agents' new typedef
names either -- check before inventing one.

## ORDER YOUR LIST BY THE CALL GRAPH, NOT BY SIZE

Every brief up to batch 40 said "work smallest-first", and that advice was
right by accident. An agent that cleared a unit 12 for 12 explained why:

> the unit is four spawner/step pairs, and **the spawner tells you the payload
> offsets while the step tells you what they mean**, so no offset ever needed
> guessing. Working smallest-first happened to give me the spawners before the
> steps and that was most of the run.

`dngtwin` found one row across fourteen stubs in that unit and `xsibs` found
one; it still landed everything, because each pair interprets the other. A
107-word body landed FIRST COMPILE purely because the spawner above it builds
the payload it reads, and two bodies totalling 352 words landed first compile
off a third.

**So: before you start, group your list into spawner/step pairs and handler
families, and take each spawner before its step.** Size is a reasonable
tie-break inside a group and nothing more. If a body installs a handler with
`n->unk_10 = (u32)func_X`, you want to write that body before `func_X`.

## WHAT TO TRUST FROM A DONOR: CARRY THE CAUSE, NOT THE FIGURE

`docs/matching-levers.md` sorts a donor's facts into three kinds -- SELECTION
(a width, an idiom, an arm order), BUDGET (a device needing a register the
frame has not got) and REFERENCE-COUNT / STATEMENT-ORDER. That sort is still
the cheapest way to decide what to try first and what to doubt.

**But all three classes have now inverted on a measured body, so no class is a
warrant.** A selection fact cost 117 rows when the new body's operand was
sign-extended and the donor's was not. A budget fact was wrong on a frame that
HAD the register free, because a competing dependence chain decided it. And a
fact recorded "measured FREE" cost 20 rows on a body with five more stores for
the load to hoist over.

> **A FACT TRANSFERS WHEN THE THING THAT CAUSED IT TRANSFERS.** The cause is
> almost always visible in the listing before any compile -- a competing
> store, a saved-register count, an operand's width, a dependence chain.

So when you carry a fact from a donor or from the levers file: **state the
cause you believe makes it apply here, and re-measure the figure.** The best
evidence in the file looks like this -- one mechanism with five witnesses in
one unit, all the same direction, priced at 3, 40 and 86 rows. The direction
transferred across all five bodies; the magnitude across none of them.
